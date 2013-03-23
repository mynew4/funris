/*
 * Copyright (C) 2010-2011 Izb00shka <http://izbooshka.net/>
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Map.h"
#include "GridStates.h"
#include "ScriptMgr.h"
#include "VMapFactory.h"
#include "MapInstanced.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Transport.h"
#include "InstanceScript.h"
#include "ObjectAccessor.h"
#include "MapManager.h"
#include "ObjectMgr.h"
#include "Group.h"

#include "MoveMap.h"

#define DEFAULT_GRID_EXPIRY     300
#define MAX_GRID_LOAD_TIME      50
#define MAX_CREATURE_ATTACK_RADIUS  (45.0f * sWorld->getRate(RATE_CREATURE_AGGRO))

GridState* si_GridStates[MAX_GRID_STATE];

Map::~Map()
{
    sScriptMgr->OnDestroyMap(this);

    UnloadAll();

    while (!i_worldObjects.empty())
    {
        WorldObject *obj = *i_worldObjects.begin();
        ASSERT(obj->m_isWorldObject);

        obj->RemoveFromWorld();
        obj->ResetMap();
    }

    // unload instance specific navigation data
    MMAP::MMapFactory::createOrGetMMapManager()->unloadMapInstance(GetId(), GetInstanceId());

    if (!m_scriptSchedule.empty())
        sWorld->DecreaseScheduledScriptCount(m_scriptSchedule.size());
}

void Map::LoadVMap(int gx,int gy)
{
                                                            // x and y are swapped !!
    int vmapLoadResult = VMAP::VMapFactory::createOrGetVMapManager()->loadMap((sWorld->GetDataPath()+ "vmaps").c_str(),  GetId(), gx,gy);
    switch(vmapLoadResult)
    {
        case VMAP::VMAP_LOAD_RESULT_OK:
            sLog->outDetail("VMAP loaded name:%s, id:%d, x:%d, y:%d (vmap rep.: x:%d, y:%d)", GetMapName(), GetId(), gx,gy,gx,gy);
            break;
        case VMAP::VMAP_LOAD_RESULT_ERROR:
            sLog->outDetail("Could not load VMAP name:%s, id:%d, x:%d, y:%d (vmap rep.: x:%d, y:%d)", GetMapName(), GetId(), gx,gy,gx,gy);
            break;
        case VMAP::VMAP_LOAD_RESULT_IGNORED:
            sLog->outStaticDebug("Ignored VMAP name:%s, id:%d, x:%d, y:%d (vmap rep.: x:%d, y:%d)", GetMapName(), GetId(), gx,gy,gx,gy);
            break;
    }
}

void Map::LoadMap(int gx,int gy, bool reload)
{
    if (i_InstanceId != 0)
    {
        if (GridMaps[gx][gy])
            return;

        // load grid map for base map
        if (!m_parentMap->GridMaps[gx][gy])
            m_parentMap->EnsureGridCreated(GridPair(63-gx,63-gy));

        ((MapInstanced*)(m_parentMap))->AddGridMapReference(GridPair(gx,gy));
        GridMaps[gx][gy] = m_parentMap->GridMaps[gx][gy];
        return;
    }

    if (GridMaps[gx][gy] && !reload)
        return;

    //map already load, delete it before reloading (Is it necessary? Do we really need the ability the reload maps during runtime?)
    if (GridMaps[gx][gy])
    {
        sLog->outDetail("Unloading previously loaded map %u before reloading.",GetId());
        sScriptMgr->OnUnloadGridMap(this, GridMaps[gx][gy], gx, gy);

        delete (GridMaps[gx][gy]);
        GridMaps[gx][gy]=NULL;
    }

    // map file name
    char *tmp=NULL;
    int len = sWorld->GetDataPath().length()+strlen("maps/%03u%02u%02u.map")+1;
    tmp = new char[len];
    snprintf(tmp, len, (char *)(sWorld->GetDataPath()+"maps/%03u%02u%02u.map").c_str(),GetId(),gx,gy);
    sLog->outDetail("Loading map %s",tmp);
    // loading data
    GridMaps[gx][gy] = new GridMap();
    if (!GridMaps[gx][gy]->loadData(tmp))
    {
        sLog->outError("Error loading map file: \n %s\n", tmp);
    }
    delete [] tmp;

    sScriptMgr->OnLoadGridMap(this, GridMaps[gx][gy], gx, gy);
}

void Map::LoadMapAndVMap(int gx,int gy)
{
    LoadMap(gx,gy);
    if (i_InstanceId == 0)
    {
        LoadVMap(gx, gy);                                   // Only load the data for the base map
        MMAP::MMapFactory::createOrGetMMapManager()->loadMap(GetId(), gx, gy);
    }
}

void Map::InitStateMachine()
{
    si_GridStates[GRID_STATE_INVALID] = new InvalidState;
    si_GridStates[GRID_STATE_ACTIVE] = new ActiveState;
    si_GridStates[GRID_STATE_IDLE] = new IdleState;
    si_GridStates[GRID_STATE_REMOVAL] = new RemovalState;
}

void Map::DeleteStateMachine()
{
    delete si_GridStates[GRID_STATE_INVALID];
    delete si_GridStates[GRID_STATE_ACTIVE];
    delete si_GridStates[GRID_STATE_IDLE];
    delete si_GridStates[GRID_STATE_REMOVAL];
}

Map::Map(uint32 id, time_t expiry, uint32 InstanceId, uint8 SpawnMode, Map* _parent):
i_mapEntry (sMapStore.LookupEntry(id)), i_spawnMode(SpawnMode), i_InstanceId(InstanceId),
m_unloadTimer(0), m_VisibleDistance(DEFAULT_VISIBILITY_DISTANCE),
m_VisibilityNotifyPeriod(DEFAULT_VISIBILITY_NOTIFY_PERIOD),
m_activeNonPlayersIter(m_activeNonPlayers.end()), i_gridExpiry(expiry), i_scriptLock(false)
{
    m_parentMap = (_parent ? _parent : this);
    for (unsigned int idx=0; idx < MAX_NUMBER_OF_GRIDS; ++idx)
    {
        for (unsigned int j=0; j < MAX_NUMBER_OF_GRIDS; ++j)
        {
            //z code
            GridMaps[idx][j] =NULL;
            setNGrid(NULL, idx, j);
        }
    }

    //lets initialize visibility distance for map
    Map::InitVisibilityDistance();

    sScriptMgr->OnCreateMap(this);
}

void Map::InitVisibilityDistance()
{
    //init visibility for continents
    m_VisibleDistance = World::GetMaxVisibleDistanceOnContinents();
    m_VisibilityNotifyPeriod = World::GetVisibilityNotifyPeriodOnContinents();
}

// Template specialization of utility methods
template<class T>
void Map::AddToGrid(T* obj, NGridType *grid, Cell const& cell)
{
    if (obj->m_isWorldObject)
        (*grid)(cell.CellX(), cell.CellY()).template AddWorldObject<T>(obj);
    else
        (*grid)(cell.CellX(), cell.CellY()).template AddGridObject<T>(obj);
}

template<>
void Map::AddToGrid(Creature* obj, NGridType *grid, Cell const& cell)
{
    if (obj->m_isWorldObject)
        (*grid)(cell.CellX(), cell.CellY()).AddWorldObject(obj);
    else
        (*grid)(cell.CellX(), cell.CellY()).AddGridObject(obj);

    obj->SetCurrentCell(cell);
}

template<class T>
void Map::RemoveFromGrid(T* obj, NGridType *grid, Cell const& cell)
{
    if (obj->m_isWorldObject)
        (*grid)(cell.CellX(), cell.CellY()).template RemoveWorldObject<T>(obj);
    else
        (*grid)(cell.CellX(), cell.CellY()).template RemoveGridObject<T>(obj);
}

template<class T>
void Map::SwitchGridContainers(T* obj, bool on)
{
    CellPair p = Trinity::ComputeCellPair(obj->GetPositionX(), obj->GetPositionY());
    if (p.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || p.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
    {
        sLog->outError("Map::SwitchGridContainers: Object " UI64FMTD " has invalid coordinates X:%f Y:%f grid cell [%u:%u]", obj->GetGUID(), obj->GetPositionX(), obj->GetPositionY(), p.x_coord, p.y_coord);
        return;
    }

    Cell cell(p);
    if (!loaded(GridPair(cell.data.Part.grid_x, cell.data.Part.grid_y)))
        return;

    sLog->outStaticDebug("Switch object " UI64FMTD " from grid[%u,%u] %u", obj->GetGUID(), cell.data.Part.grid_x, cell.data.Part.grid_y, on);
    NGridType *ngrid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(ngrid != NULL);

    GridType &grid = (*ngrid)(cell.CellX(), cell.CellY());

    if (on)
    {
        grid.RemoveGridObject<T>(obj);
        grid.AddWorldObject<T>(obj);
    }
    else
    {
        grid.RemoveWorldObject<T>(obj);
        grid.AddGridObject<T>(obj);
    }
    obj->m_isWorldObject = on;
}

template void Map::SwitchGridContainers(Creature *, bool);

template<class T>
void Map::DeleteFromWorld(T* obj)
{
    // Note: In case resurrectable corpse and pet its removed from global lists in own destructor
    delete obj;
}

template<>
void Map::DeleteFromWorld(Player* pl)
{
    sObjectAccessor->RemoveObject(pl);
    delete pl;
}

void
Map::EnsureGridCreated(const GridPair &p)
{
    if (!getNGrid(p.x_coord, p.y_coord))
    {
        ACE_GUARD(ACE_Thread_Mutex, Guard, Lock);
        if (!getNGrid(p.x_coord, p.y_coord))
        {
            sLog->outDebug(LOG_FILTER_MAPS, "Creating grid[%u,%u] for map %u instance %u", p.x_coord, p.y_coord, GetId(), i_InstanceId);

            setNGrid(new NGridType(p.x_coord*MAX_NUMBER_OF_GRIDS + p.y_coord, p.x_coord, p.y_coord, i_gridExpiry, sWorld->getBoolConfig(CONFIG_GRID_UNLOAD)),
                p.x_coord, p.y_coord);

            // build a linkage between this map and NGridType
            buildNGridLinkage(getNGrid(p.x_coord, p.y_coord));

            getNGrid(p.x_coord, p.y_coord)->SetGridState(GRID_STATE_IDLE);

            //z coord
            int gx = (MAX_NUMBER_OF_GRIDS - 1) - p.x_coord;
            int gy = (MAX_NUMBER_OF_GRIDS - 1) - p.y_coord;

            if (!GridMaps[gx][gy])
                LoadMapAndVMap(gx,gy);
        }
    }
}

void
Map::EnsureGridLoadedAtEnter(const Cell &cell, Player *player)
{
    EnsureGridLoaded(cell);
    NGridType *grid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(grid != NULL);

    // refresh grid state & timer
    if (grid->GetGridState() != GRID_STATE_ACTIVE)
    {
        if (player)
        {
            sLog->outStaticDebug("Player %s enter cell[%u,%u] triggers loading of grid[%u,%u] on map %u", player->GetName(), cell.CellX(), cell.CellY(), cell.GridX(), cell.GridY(), GetId());
        }
        else
        {
            sLog->outStaticDebug("Active object nearby triggers loading of grid [%u,%u] on map %u", cell.GridX(), cell.GridY(), GetId());
        }

        ResetGridExpiry(*grid, 0.1f);
        grid->SetGridState(GRID_STATE_ACTIVE);
    }
}

bool Map::EnsureGridLoaded(const Cell &cell)
{
    EnsureGridCreated(GridPair(cell.GridX(), cell.GridY()));
    NGridType *grid = getNGrid(cell.GridX(), cell.GridY());

    ASSERT(grid != NULL);
    if (!isGridObjectDataLoaded(cell.GridX(), cell.GridY()))
    {
        sLog->outDebug(LOG_FILTER_MAPS, "Loading grid[%u,%u] for map %u instance %u", cell.GridX(), cell.GridY(), GetId(), i_InstanceId);

        setGridObjectDataLoaded(true,cell.GridX(), cell.GridY());

        ObjectGridLoader loader(*grid, this, cell);
        loader.LoadN();

        // Add resurrectable corpses to world object list in grid
        sObjectAccessor->AddCorpsesToGrid(GridPair(cell.GridX(),cell.GridY()),(*grid)(cell.CellX(), cell.CellY()), this);
        return true;
    }

    return false;
}

void Map::LoadGrid(float x, float y)
{
    CellPair pair = Trinity::ComputeCellPair(x, y);
    Cell cell(pair);
    EnsureGridLoaded(cell);
}

bool Map::Add(Player *player)
{
    // Check if we are adding to correct map
    ASSERT (player->GetMap() == this);
    CellPair p = Trinity::ComputeCellPair(player->GetPositionX(), player->GetPositionY());
    if (p.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || p.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
    {
        sLog->outError("Map::Add: Player (GUID: %u) has invalid coordinates X:%f Y:%f grid cell [%u:%u]", player->GetGUIDLow(), player->GetPositionX(), player->GetPositionY(), p.x_coord, p.y_coord);
        return false;
    }

    player->SetMap(this);

    Cell cell(p);
    EnsureGridLoadedAtEnter(cell, player);
    NGridType *grid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(grid != NULL);
    AddToGrid(player, grid, cell);

    player->AddToWorld();

    SendInitSelf(player);
    SendInitTransports(player);

    player->m_clientGUIDs.clear();
    player->UpdateObjectVisibility(false);

    sScriptMgr->OnPlayerEnterMap(this, player);
    return true;
}

template<class T>
void
Map::Add(T *obj)
{
    CellPair p = Trinity::ComputeCellPair(obj->GetPositionX(), obj->GetPositionY());
    if (p.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || p.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
    {
        sLog->outError("Map::Add: Object " UI64FMTD " has invalid coordinates X:%f Y:%f grid cell [%u:%u]", obj->GetGUID(), obj->GetPositionX(), obj->GetPositionY(), p.x_coord, p.y_coord);
        return;
    }

    Cell cell(p);
    if (obj->IsInWorld()) // need some clean up later
    {
        obj->UpdateObjectVisibility(true);
        return;
    }

    if (obj->isActiveObject())
        EnsureGridLoadedAtEnter(cell);
    else
        EnsureGridCreated(GridPair(cell.GridX(), cell.GridY()));

    NGridType *grid = getNGrid(cell.GridX(), cell.GridY());
    ASSERT(grid != NULL);

    AddToGrid(obj,grid,cell);
    //obj->SetMap(this);
    obj->AddToWorld();

    if (obj->isActiveObject())
        AddToActive(obj);

    sLog->outStaticDebug("Object %u enters grid[%u,%u]", GUID_LOPART(obj->GetGUID()), cell.GridX(), cell.GridY());

    //something, such as vehicle, needs to be update immediately
    //also, trigger needs to cast spell, if not update, cannot see visual
    obj->UpdateObjectVisibility(true);
}

bool Map::loaded(const GridPair &p) const
{
    return (getNGrid(p.x_coord, p.y_coord) && isGridObjectDataLoaded(p.x_coord, p.y_coord));
}

void Map::VisitNearbyCellsOf(WorldObject* obj, TypeContainerVisitor<Trinity::ObjectUpdater, GridTypeMapContainer> &gridVisitor, TypeContainerVisitor<Trinity::ObjectUpdater, WorldTypeMapContainer> &worldVisitor)
{
    CellPair standing_cell(Trinity::ComputeCellPair(obj->GetPositionX(), obj->GetPositionY()));

    // Check for correctness of standing_cell, it also avoids problems with update_cell
    if (standing_cell.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || standing_cell.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
        return;

    // the overloaded operators handle range checking
    // so there's no need for range checking inside the loop
    CellPair begin_cell(standing_cell), end_cell(standing_cell);
    //lets update mobs/objects in ALL visible cells around object!
    CellArea area = Cell::CalculateCellArea(*obj, obj->GetGridActivationRange());
    area.ResizeBorders(begin_cell, end_cell);

    for (uint32 x = begin_cell.x_coord; x <= end_cell.x_coord; ++x)
    {
        for (uint32 y = begin_cell.y_coord; y <= end_cell.y_coord; ++y)
        {
            // marked cells are those that have been visited
            // don't visit the same cell twice
            uint32 cell_id = (y * TOTAL_NUMBER_OF_CELLS_PER_MAP) + x;
            if (isCellMarked(cell_id))
                continue;

            markCell(cell_id);
            CellPair pair(x,y);
            Cell cell(pair);
            cell.data.Part.reserved = CENTER_DISTRICT;
            cell.Visit(pair, gridVisitor,  *this);
            cell.Visit(pair, worldVisitor, *this);
        }
    }
}

void Map::Update(const uint32 &t_diff)
{
    /// update worldsessions for existing players
    for(m_mapRefIter = m_mapRefManager.begin(); m_mapRefIter != m_mapRefManager.end(); ++m_mapRefIter)
    {
        Player* plr = m_mapRefIter->getSource();
        if(plr && plr->IsInWorld())
        {
            //plr->Update(t_diff);
            WorldSession * pSession = plr->GetSession();
            MapSessionFilter updater(pSession);
            pSession->Update(t_diff, updater);
        }
    }
    /// update active cells around players and active objects
    resetMarkedCells();

    Trinity::ObjectUpdater updater(t_diff);
    // for creature
    TypeContainerVisitor<Trinity::ObjectUpdater, GridTypeMapContainer  > grid_object_update(updater);
    // for pets
    TypeContainerVisitor<Trinity::ObjectUpdater, WorldTypeMapContainer > world_object_update(updater);

    // the player iterator is stored in the map object
    // to make sure calls to Map::Remove don't invalidate it
    for (m_mapRefIter = m_mapRefManager.begin(); m_mapRefIter != m_mapRefManager.end(); ++m_mapRefIter)
    {
        Player* plr = m_mapRefIter->getSource();

        if (!plr->IsInWorld())
            continue;

        // update players at tick
        plr->Update(t_diff);

        VisitNearbyCellsOf(plr, grid_object_update, world_object_update);
    }

    // non-player active objects, increasing iterator in the loop in case of object removal
    for (m_activeNonPlayersIter = m_activeNonPlayers.begin(); m_activeNonPlayersIter != m_activeNonPlayers.end();)
    {
        WorldObject* obj = *m_activeNonPlayersIter;
        ++m_activeNonPlayersIter;

        if (!obj->IsInWorld())
            continue;

        VisitNearbyCellsOf(obj, grid_object_update, world_object_update);
    }

    ///- Process necessary scripts
    if (!m_scriptSchedule.empty())
    {
        i_scriptLock = true;
        ScriptsProcess();
        i_scriptLock = false;
    }

    MoveAllCreaturesInMoveList();

    if (!m_mapRefManager.isEmpty() || !m_activeNonPlayers.empty())
        ProcessRelocationNotifies(t_diff);

    sScriptMgr->OnMapUpdate(this, t_diff);
}

struct ResetNotifier
{
    template<class T>inline void resetNotify(GridRefManager<T> &m)
    {
        for (typename GridRefManager<T>::iterator iter=m.begin(); iter != m.end(); ++iter)
            iter->getSource()->ResetAllNotifies();
    }
    template<class T> void Visit(GridRefManager<T> &) {}
    void Visit(CreatureMapType &m) { resetNotify<Creature>(m);}
    void Visit(PlayerMapType &m) { resetNotify<Player>(m);}
};

void Map::ProcessRelocationNotifies(const uint32 & diff)
{
    for (GridRefManager<NGridType>::iterator i = GridRefManager<NGridType>::begin(); i != GridRefManager<NGridType>::end(); ++i)
    {
        NGridType *grid = i->getSource();

        if (grid->GetGridState() != GRID_STATE_ACTIVE)
            continue;

        grid->getGridInfoRef()->getRelocationTimer().TUpdate(diff);
        if (!grid->getGridInfoRef()->getRelocationTimer().TPassed())
            continue;

        uint32 gx = grid->getX(), gy = grid->getY();

        CellPair cell_min(gx*MAX_NUMBER_OF_CELLS, gy*MAX_NUMBER_OF_CELLS);
        CellPair cell_max(cell_min.x_coord + MAX_NUMBER_OF_CELLS, cell_min.y_coord+MAX_NUMBER_OF_CELLS);

        for (uint32 x = cell_min.x_coord; x < cell_max.x_coord; ++x)
        {
            for (uint32 y = cell_min.y_coord; y < cell_max.y_coord; ++y)
            {
                uint32 cell_id = (y * TOTAL_NUMBER_OF_CELLS_PER_MAP) + x;
                if (!isCellMarked(cell_id))
                    continue;

                CellPair pair(x,y);
                Cell cell(pair);
                cell.SetNoCreate();

                Trinity::DelayedUnitRelocation cell_relocation(cell, pair, *this, MAX_VISIBILITY_DISTANCE);
                TypeContainerVisitor<Trinity::DelayedUnitRelocation, GridTypeMapContainer  > grid_object_relocation(cell_relocation);
                TypeContainerVisitor<Trinity::DelayedUnitRelocation, WorldTypeMapContainer > world_object_relocation(cell_relocation);
                Visit(cell, grid_object_relocation);
                Visit(cell, world_object_relocation);
            }
        }
    }

    ResetNotifier reset;
    TypeContainerVisitor<ResetNotifier, GridTypeMapContainer >  grid_notifier(reset);
    TypeContainerVisitor<ResetNotifier, WorldTypeMapContainer > world_notifier(reset);
    for (GridRefManager<NGridType>::iterator i = GridRefManager<NGridType>::begin(); i != GridRefManager<NGridType>::end(); ++i)
    {
        NGridType *grid = i->getSource();

        if (grid->GetGridState() != GRID_STATE_ACTIVE)
            continue;

        if (!grid->getGridInfoRef()->getRelocationTimer().TPassed())
            continue;

        grid->getGridInfoRef()->getRelocationTimer().TReset(diff, m_VisibilityNotifyPeriod);

        uint32 gx = grid->getX(), gy = grid->getY();

        CellPair cell_min(gx*MAX_NUMBER_OF_CELLS, gy*MAX_NUMBER_OF_CELLS);
        CellPair cell_max(cell_min.x_coord + MAX_NUMBER_OF_CELLS, cell_min.y_coord+MAX_NUMBER_OF_CELLS);

        for (uint32 x = cell_min.x_coord; x < cell_max.x_coord; ++x)
        {
            for (uint32 y = cell_min.y_coord; y < cell_max.y_coord; ++y)
            {
                uint32 cell_id = (y * TOTAL_NUMBER_OF_CELLS_PER_MAP) + x;
                if (!isCellMarked(cell_id))
                    continue;

                CellPair pair(x,y);
                Cell cell(pair);
                cell.SetNoCreate();
                Visit(cell, grid_notifier);
                Visit(cell, world_notifier);
            }
        }
    }
}

void Map::Remove(Player *player, bool remove)
{
    player->RemoveFromWorld();
    SendRemoveTransports(player);

    CellPair p = Trinity::ComputeCellPair(player->GetPositionX(), player->GetPositionY());
    if (p.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || p.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
        sLog->outCrash("Map::Remove: Player is in invalid cell!");
    else
    {
        Cell cell(p);
        if (!getNGrid(cell.data.Part.grid_x, cell.data.Part.grid_y))
            sLog->outError("Map::Remove() i_grids was NULL x:%d, y:%d",cell.data.Part.grid_x,cell.data.Part.grid_y);
        else
        {
            sLog->outStaticDebug("Remove player %s from grid[%u,%u]", player->GetName(), cell.GridX(), cell.GridY());
            NGridType *grid = getNGrid(cell.GridX(), cell.GridY());
            ASSERT(grid != NULL);

            player->UpdateObjectVisibility(true);
            RemoveFromGrid(player,grid,cell);
        }
    }

    if (remove)
        DeleteFromWorld(player);

    sScriptMgr->OnPlayerLeaveMap(this, player);
}

template<class T>
void
Map::Remove(T *obj, bool remove)
{
    obj->RemoveFromWorld();
    if (obj->isActiveObject())
        RemoveFromActive(obj);

    CellPair p = Trinity::ComputeCellPair(obj->GetPositionX(), obj->GetPositionY());
    if (p.x_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP || p.y_coord >= TOTAL_NUMBER_OF_CELLS_PER_MAP)
        sLog->outError("Map::Remove: Object " UI64FMTD " has invalid coordinates X:%f Y:%f grid cell [%u:%u]", obj->GetGUID(), obj->GetPositionX(), obj->GetPositionY(), p.x_coord, p.y_coord);
    else
    {
        Cell cell(p);
        if (loaded(GridPair(cell.data.Part.grid_x, cell.data.Part.grid_y)))
        {
            sLog->outStaticDebug("Remove object " UI64FMTD " from grid[%u,%u]", obj->GetGUID(), cell.data.Part.grid_x, cell.data.Part.grid_y);
            NGridType *grid = getNGrid(cell.GridX(), cell.GridY());
            ASSERT(grid != NULL);

            obj->UpdateObjectVisibility(true);
            RemoveFromGrid(obj,grid,cell);
        }
    }

    obj->ResetMap();

    if (remove)
    {
        // if option set then object already saved at this moment
        if (!sWorld->getBoolConfig(CONFIG_SAVE_RESPAWN_TIME_IMMEDIATELY))
            obj->SaveRespawnTime();
        DeleteFromWorld(obj);
    }
}

void
Map::PlayerRelocation(Player *player, float x, float y, float z, float orientation)
{
    ASSERT(player);

    CellPair old_val = Trinity::ComputeCellPair(player->GetPositionX(), player->GetPositionY());
    CellPair new_val = Trinity::ComputeCellPair(x, y);

    Cell old_cell(old_val);
    Cell new_cell(new_val);

    player->Relocate(x, y, z, orientation);

    if (old_cell.DiffGrid(new_cell) || old_cell.DiffCell(new_cell))
    {
        sLog->outStaticDebug("Player %s relocation grid[%u,%u]cell[%u,%u]->grid[%u,%u]cell[%u,%u]", player->GetName(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());

        NGridType* oldGrid = getNGrid(old_cell.GridX(), old_cell.GridY());
        RemoveFromGrid(player, oldGrid,old_cell);

        if (old_cell.DiffGrid(new_cell))
            EnsureGridLoadedAtEnter(new_cell, player);

        NGridType* newGrid = getNGrid(new_cell.GridX(), new_cell.GridY());
        AddToGrid(player, newGrid,new_cell);
    }

    player->UpdateObjectVisibility(false);
}

void
Map::CreatureRelocation(Creature *creature, float x, float y, float z, float ang, bool respawnRelocationOnFail)
{
    ASSERT(CheckGridIntegrity(creature,false));

    Cell old_cell = creature->GetCurrentCell();

    CellPair new_val = Trinity::ComputeCellPair(x, y);
    Cell new_cell(new_val);

    if (!respawnRelocationOnFail && !getNGrid(new_cell.GridX(), new_cell.GridY()))
        return;

    // delay creature move for grid/cell to grid/cell moves
    if (old_cell.DiffCell(new_cell) || old_cell.DiffGrid(new_cell))
    {
        #ifdef TRINITY_DEBUG
            sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) added to moving list from grid[%u,%u]cell[%u,%u] to grid[%u,%u]cell[%u,%u].", creature->GetGUIDLow(), creature->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
        #endif
        AddCreatureToMoveList(creature, x, y, z, ang);
        // in diffcell/diffgrid case notifiers called at finishing move creature in Map::MoveAllCreaturesInMoveList
    }
    else
    {
        creature->Relocate(x, y, z, ang);
        creature->UpdateObjectVisibility(false);
    }

    ASSERT(CheckGridIntegrity(creature,true));
}

void Map::AddCreatureToMoveList(Creature *c, float x, float y, float z, float ang)
{
    if (!c)
        return;

    i_creaturesToMove[c] = CreatureMover(x, y, z, ang);
}

void Map::MoveAllCreaturesInMoveList()
{
    while (!i_creaturesToMove.empty())
    {
        // get data and remove element;
        CreatureMoveList::iterator iter = i_creaturesToMove.begin();
        Creature* c = iter->first;
        CreatureMover cm = iter->second;
        i_creaturesToMove.erase(iter);

        // calculate cells
        CellPair new_val = Trinity::ComputeCellPair(cm.x, cm.y);
        Cell new_cell(new_val);

        // do move or do move to respawn or remove creature if previous all fail
        if (CreatureCellRelocation(c,new_cell))
        {
            // update pos
            c->Relocate(cm.x, cm.y, cm.z, cm.ang);
            c->UpdateObjectVisibility(false);
        }
        else
        {
            // if creature can't be move in new cell/grid (not loaded) move it to repawn cell/grid
            // creature coordinates will be updated and notifiers send
            if (!CreatureRespawnRelocation(c))
            {
                // ... or unload (if respawn grid also not loaded)
                #ifdef TRINITY_DEBUG
                    sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) cannot be move to unloaded respawn grid.",c->GetGUIDLow(),c->GetEntry());
                #endif
//                AddObjectToRemoveList(c);
                //AddObjectToRemoveList(Pet*) should only be called in Pet::Remove
                //This may happen when a player just logs in and a pet moves to a nearby unloaded cell
                //To avoid this, we can load nearby cells when player log in
                //But this check is always needed to ensure safety
                //TODO: pets will disappear if this is outside CreatureRespawnRelocation
                //need to check why pet is frequently relocated to an unloaded cell
                if (c->isPet())
                    ((Pet*)c)->Remove(PET_SAVE_NOT_IN_SLOT, true);
                else
                    AddObjectToRemoveList(c);
            }
        }
    }
}

bool Map::CreatureCellRelocation(Creature *c, Cell new_cell)
{
    Cell const& old_cell = c->GetCurrentCell();
    if (!old_cell.DiffGrid(new_cell))                       // in same grid
    {
        // if in same cell then none do
        if (old_cell.DiffCell(new_cell))
        {
            #ifdef TRINITY_DEBUG
                sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) moved in grid[%u,%u] from cell[%u,%u] to cell[%u,%u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.CellX(), new_cell.CellY());
            #endif

            RemoveFromGrid(c,getNGrid(old_cell.GridX(), old_cell.GridY()),old_cell);
            AddToGrid(c,getNGrid(new_cell.GridX(), new_cell.GridY()),new_cell);
        }
        else
        {
            #ifdef TRINITY_DEBUG
                sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) moved in same grid[%u,%u]cell[%u,%u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY());
            #endif
        }

        return true;
    }

    // in diff. grids but active creature
    if (c->isActiveObject())
    {
        EnsureGridLoadedAtEnter(new_cell);

        #ifdef TRINITY_DEBUG
            sLog->outDebug(LOG_FILTER_MAPS, "Active creature (GUID: %u Entry: %u) moved from grid[%u,%u]cell[%u,%u] to grid[%u,%u]cell[%u,%u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
        #endif

        RemoveFromGrid(c,getNGrid(old_cell.GridX(), old_cell.GridY()),old_cell);
        AddToGrid(c,getNGrid(new_cell.GridX(), new_cell.GridY()),new_cell);

        return true;
    }

    // in diff. loaded grid normal creature
    if (loaded(GridPair(new_cell.GridX(), new_cell.GridY())))
    {
        #ifdef TRINITY_DEBUG
            sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) moved from grid[%u,%u]cell[%u,%u] to grid[%u,%u]cell[%u,%u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
        #endif

        RemoveFromGrid(c,getNGrid(old_cell.GridX(), old_cell.GridY()),old_cell);
        EnsureGridCreated(GridPair(new_cell.GridX(), new_cell.GridY()));
        AddToGrid(c,getNGrid(new_cell.GridX(), new_cell.GridY()),new_cell);

        return true;
    }

    // fail to move: normal creature attempt move to unloaded grid
    #ifdef TRINITY_DEBUG
        sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) attempted to move from grid[%u,%u]cell[%u,%u] to unloaded grid[%u,%u]cell[%u,%u].", c->GetGUIDLow(), c->GetEntry(), old_cell.GridX(), old_cell.GridY(), old_cell.CellX(), old_cell.CellY(), new_cell.GridX(), new_cell.GridY(), new_cell.CellX(), new_cell.CellY());
    #endif
    return false;
}

bool Map::CreatureRespawnRelocation(Creature *c)
{
    float resp_x, resp_y, resp_z, resp_o;
    c->GetRespawnCoord(resp_x, resp_y, resp_z, &resp_o);

    CellPair resp_val = Trinity::ComputeCellPair(resp_x, resp_y);
    Cell resp_cell(resp_val);

    c->CombatStop();
    c->GetMotionMaster()->Clear();

    #ifdef TRINITY_DEBUG
        sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u Entry: %u) moved from grid[%u,%u]cell[%u,%u] to respawn grid[%u,%u]cell[%u,%u].", c->GetGUIDLow(), c->GetEntry(), c->GetCurrentCell().GridX(), c->GetCurrentCell().GridY(), c->GetCurrentCell().CellX(), c->GetCurrentCell().CellY(), resp_cell.GridX(), resp_cell.GridY(), resp_cell.CellX(), resp_cell.CellY());
    #endif

    // teleport it to respawn point (like normal respawn if player see)
    if (CreatureCellRelocation(c,resp_cell))
    {
        c->Relocate(resp_x, resp_y, resp_z, resp_o);
        c->GetMotionMaster()->Initialize();                 // prevent possible problems with default move generators
        c->UpdateObjectVisibility(false);
        return true;
    }
	return false;
}

bool Map::UnloadGrid(const uint32 &x, const uint32 &y, bool unloadAll)
{
    NGridType *grid = getNGrid(x, y);
    ASSERT(grid != NULL);

    {
        if (!unloadAll && ActiveObjectsNearGrid(x, y))
             return false;

        sLog->outDebug(LOG_FILTER_MAPS, "Unloading grid[%u,%u] for map %u", x,y, GetId());

        ObjectGridUnloader unloader(*grid);

        if (!unloadAll)
        {
            // Finish creature moves, remove and delete all creatures with delayed remove before moving to respawn grids
            // Must know real mob position before move
            MoveAllCreaturesInMoveList();

            // move creatures to respawn grids if this is diff.grid or to remove list
            unloader.MoveToRespawnN();

            // Finish creature moves, remove and delete all creatures with delayed remove before unload
            MoveAllCreaturesInMoveList();
        }

        ObjectGridCleaner cleaner(*grid);
        cleaner.CleanN();

        RemoveAllObjectsInRemoveList();

        unloader.UnloadN();

        ASSERT(i_objectsToRemove.empty());

        delete grid;
        setNGrid(NULL, x, y);
    }
    int gx = (MAX_NUMBER_OF_GRIDS - 1) - x;
    int gy = (MAX_NUMBER_OF_GRIDS - 1) - y;

    // delete grid map, but don't delete if it is from parent map (and thus only reference)
    //+++if (GridMaps[gx][gy]) don't check for GridMaps[gx][gy], we might have to unload vmaps
    {
        if (i_InstanceId == 0)
        {
            if (GridMaps[gx][gy])
            {
                GridMaps[gx][gy]->unloadData();
                delete GridMaps[gx][gy];
            }
            // x and y are swapped
            VMAP::VMapFactory::createOrGetVMapManager()->unloadMap(GetId(), gx, gy);
            MMAP::MMapFactory::createOrGetMMapManager()->unloadMap(GetId(), gx, gy);
        }
        else
            ((MapInstanced*)m_parentMap)->RemoveGridMapReference(GridPair(gx, gy));

        GridMaps[gx][gy] = NULL;
    }
    sLog->outStaticDebug("Unloading grid[%u,%u] for map %u finished", x,y, GetId());
    return true;
}

void Map::RemoveAllPlayers()
{
    if (HavePlayers())
    {
        for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        {
            Player* plr = itr->getSource();
            if (!plr->IsBeingTeleportedFar())
            {
                // this is happening for bg
                sLog->outError("Map::UnloadAll: player %s is still in map %u during unload, this should not happen!", plr->GetName(), GetId());
                plr->TeleportTo(plr->m_homebindMapId, plr->m_homebindX, plr->m_homebindY, plr->m_homebindZ, plr->GetOrientation());
            }
        }
    }
}

void Map::UnloadAll()
{
    // clear all delayed moves, useless anyway do this moves before map unload.
    i_creaturesToMove.clear();

    for (GridRefManager<NGridType>::iterator i = GridRefManager<NGridType>::begin(); i != GridRefManager<NGridType>::end();)
    {
        NGridType &grid(*i->getSource());
        ++i;
        UnloadGrid(grid.getX(), grid.getY(), true);       // deletes the grid and removes it from the GridRefManager
    }
}


inline GridMap *Map::GetGrid(float x, float y)
{
    // half opt method
    int gx=(int)(32-x/SIZE_OF_GRIDS);                       //grid x
    int gy=(int)(32-y/SIZE_OF_GRIDS);                       //grid y

    // ensure GridMap is loaded
    EnsureGridCreated(GridPair(63-gx,63-gy));

    return GridMaps[gx][gy];
}

float Map::GetHeight(float x, float y, float maxZ, bool pUseVmaps, float maxSearchDist) const
{
    // find raw .map surface under Z coordinates
    float mapHeight;

    float z2 = maxZ + 2.0f;

    if (GridMap *gmap = const_cast<Map*>(this)->GetGrid(x, y))
    {
        float _mapheight = gmap->getHeight(x,y);

        // look from a bit higher pos to find the floor, ignore under surface case
        if (z2 > _mapheight)
            mapHeight = _mapheight;
        else
            mapHeight = VMAP_INVALID_HEIGHT_VALUE;
    }
    else
        mapHeight = VMAP_INVALID_HEIGHT_VALUE;

    float vmapHeight;
    if (pUseVmaps)
    {
        VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
        if (vmgr->isHeightCalcEnabled())
        {
            if (mapHeight > INVALID_HEIGHT && z2 - mapHeight > maxSearchDist)
                maxSearchDist = z2 - mapHeight + 1.0f;      // 1.0 make sure that we not fail for case when map height near but above for vamp height
            // look from a bit higher pos to find the floor
            vmapHeight = vmgr->getHeight(GetId(), x, y, z2, maxSearchDist);
        }
        else
            vmapHeight = VMAP_INVALID_HEIGHT_VALUE;
    }
    else
        vmapHeight = VMAP_INVALID_HEIGHT_VALUE;

    // mapHeight set for any above raw ground Z or <= INVALID_HEIGHT
    // vmapheight set for any under Z value or <= INVALID_HEIGHT

    if (vmapHeight > INVALID_HEIGHT)
    {
        if (mapHeight > INVALID_HEIGHT)
        {
            // we have mapheight and vmapheight and must select more appropriate

            // we are already under the surface or vmap height above map heigt
            // or if the distance of the vmap height is less the land height distance
            if (maxZ < mapHeight || vmapHeight > mapHeight || fabs(mapHeight-maxZ) > fabs(vmapHeight-maxZ))
                return vmapHeight;
            else
                return mapHeight;                           // better use .map surface height

        }
        else
            return vmapHeight;                              // we have only vmapHeight (if have)
    }
    return mapHeight;
}

inline bool IsOutdoorWMO(uint32 mogpFlags, int32 /*adtId*/, int32 /*rootId*/, int32 /*groupId*/, WMOAreaTableEntry const* wmoEntry, AreaTableEntry const* atEntry)
{
    bool outdoor = true;

    if(wmoEntry && atEntry)
    {
        if(atEntry->flags & AREA_FLAG_OUTSIDE)
            return true;
        if(atEntry->flags & AREA_FLAG_INSIDE)
            return false;
    }

    outdoor = mogpFlags&0x8;

    if(wmoEntry)
    {
        if(wmoEntry->Flags & 4)
            return true;
        if((wmoEntry->Flags & 2)!=0)
            outdoor = false;
    }
    return outdoor;
}

bool Map::IsOutdoors(float x, float y, float z) const
{
    uint32 mogpFlags;
    int32 adtId, rootId, groupId;

    // no wmo found? -> outside by default
    if(!GetAreaInfo(x, y, z, mogpFlags, adtId, rootId, groupId))
        return true;

    AreaTableEntry const* atEntry = 0;
    WMOAreaTableEntry const* wmoEntry= GetWMOAreaTableEntryByTripple(rootId, adtId, groupId);
    if(wmoEntry)
    {
        sLog->outStaticDebug("Got WMOAreaTableEntry! flag %u, areaid %u", wmoEntry->Flags, wmoEntry->areaId);
        atEntry = GetAreaEntryByAreaID(wmoEntry->areaId);
    }
    return IsOutdoorWMO(mogpFlags, adtId, rootId, groupId, wmoEntry, atEntry);
}

bool Map::GetAreaInfo(float x, float y, float z, uint32 &flags, int32 &adtId, int32 &rootId, int32 &groupId) const
{
    float vmap_z = z;
    VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
    if (vmgr->getAreaInfo(GetId(), x, y, vmap_z, flags, adtId, rootId, groupId))
    {
        // check if there's terrain between player height and object height
        if(GridMap *gmap = const_cast<Map*>(this)->GetGrid(x, y))
        {
            float _mapheight = gmap->getHeight(x,y);
            // z + 2.0f condition taken from GetHeight(), not sure if it's such a great choice...
            if(z + 2.0f > _mapheight &&  _mapheight > vmap_z)
                return false;
        }
        return true;
    }
    return false;
}

uint16 Map::GetAreaFlag(float x, float y, float z, bool *isOutdoors) const
{
    uint32 mogpFlags;
    int32 adtId, rootId, groupId;
    WMOAreaTableEntry const* wmoEntry = 0;
    AreaTableEntry const* atEntry = 0;
    bool haveAreaInfo = false;

    if (GetAreaInfo(x, y, z, mogpFlags, adtId, rootId, groupId))
    {
        haveAreaInfo = true;
        wmoEntry = GetWMOAreaTableEntryByTripple(rootId, adtId, groupId);
        if (wmoEntry)
            atEntry = GetAreaEntryByAreaID(wmoEntry->areaId);
    }

    uint16 areaflag;

    if (atEntry)
        areaflag = atEntry->exploreFlag;
    else
    {
        if (GridMap *gmap = const_cast<Map*>(this)->GetGrid(x, y))
            areaflag = gmap->getArea(x, y);
        // this used while not all *.map files generated (instances)
        else
            areaflag = GetAreaFlagByMapId(i_mapEntry->MapID);
    }

    if (isOutdoors)
    {
        if (haveAreaInfo)
            *isOutdoors = IsOutdoorWMO(mogpFlags, adtId, rootId, groupId, wmoEntry, atEntry);
        else
            *isOutdoors = true;
    }
    return areaflag;
 }

uint8 Map::GetTerrainType(float x, float y) const
{
    if (GridMap *gmap = const_cast<Map*>(this)->GetGrid(x, y))
        return gmap->getTerrainType(x, y);
    else
        return 0;
}

GridMapLiquidStatus Map::getLiquidStatus(float x, float y, float z, uint8 ReqLiquidType, GridMapLiquidData *data) const
{
    GridMapLiquidStatus result = LIQUID_MAP_NO_WATER;
    VMAP::IVMapManager* vmgr = VMAP::VMapFactory::createOrGetVMapManager();
    float liquid_level, ground_level = INVALID_HEIGHT_VALUE;
    uint32 liquid_type;
    if (vmgr->GetLiquidLevel(GetId(), x, y, z, ReqLiquidType, liquid_level, ground_level, liquid_type))
    {
        sLog->outDebug(LOG_FILTER_MAPS, "getLiquidStatus(): vmap liquid level: %f ground: %f type: %u", liquid_level, ground_level, liquid_type);
        // Check water level and ground level
        if (liquid_level > ground_level && z > ground_level - 2)
        {
            // All ok in water -> store data
            if (data)
            {
                data->type  = liquid_type;
                data->level = liquid_level;
                data->depth_level = ground_level;
            }

            // For speed check as int values
            int delta = int((liquid_level - z) * 10);

            // Get position delta
            if (delta > 20)                   // Under water
                return LIQUID_MAP_UNDER_WATER;
            if (delta > 0 )                   // In water
                return LIQUID_MAP_IN_WATER;
            if (delta > -1)                   // Walk on water
                return LIQUID_MAP_WATER_WALK;
            result = LIQUID_MAP_ABOVE_WATER;
        }
    }

    if(GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
    {
        GridMapLiquidData map_data;
        GridMapLiquidStatus map_result = gmap->getLiquidStatus(x, y, z, ReqLiquidType, &map_data);
        // Not override LIQUID_MAP_ABOVE_WATER with LIQUID_MAP_NO_WATER:
        if (map_result != LIQUID_MAP_NO_WATER && (map_data.level > ground_level))
        {
            if (data)
                *data = map_data;
            return map_result;
        }
    }
    return result;
}

float Map::GetWaterLevel(float x, float y) const
{
    if (GridMap* gmap = const_cast<Map*>(this)->GetGrid(x, y))
        return gmap->getLiquidLevel(x, y);
    else
        return 0;
}

float Map::GetWaterOrGroundLevel(float x, float y, float z, float* pGround /*= NULL*/, bool swim /*= false*/) const
{
    if (const_cast<Map*>(this)->GetGrid(x, y))
    {
        // we need ground level (including grid height version) for proper return water level in point
        float ground_z = GetHeight(x, y, z, true, DEFAULT_WATER_SEARCH);
        if (pGround)
            *pGround = ground_z;

        GridMapLiquidData liquid_status;

        GridMapLiquidStatus res = getLiquidStatus(x, y, ground_z, MAP_ALL_LIQUIDS, &liquid_status);
        return res ? ( swim ? liquid_status.level - 2.0f : liquid_status.level) : ground_z;
    }

    return VMAP_INVALID_HEIGHT_VALUE;
}

uint32 Map::GetAreaIdByAreaFlag(uint16 areaflag,uint32 map_id)
{
    AreaTableEntry const *entry = GetAreaEntryByAreaFlagAndMap(areaflag,map_id);

    if (entry)
        return entry->ID;
    else
        return 0;
}

uint32 Map::GetZoneIdByAreaFlag(uint16 areaflag,uint32 map_id)
{
    AreaTableEntry const *entry = GetAreaEntryByAreaFlagAndMap(areaflag,map_id);

    if (entry)
        return (entry->zone != 0) ? entry->zone : entry->ID;
    else
        return 0;
}

void Map::GetZoneAndAreaIdByAreaFlag(uint32& zoneid, uint32& areaid, uint16 areaflag,uint32 map_id)
{
    AreaTableEntry const *entry = GetAreaEntryByAreaFlagAndMap(areaflag,map_id);

    areaid = entry ? entry->ID : 0;
    zoneid = entry ? ((entry->zone != 0) ? entry->zone : entry->ID) : 0;
    if (map_id == 572)
    {
        zoneid = 3968;
        areaid = 3968;
    }
}

bool Map::IsInWater(float x, float y, float pZ, GridMapLiquidData *data) const
{
    // Check surface in x, y point for liquid
    if (const_cast<Map*>(this)->GetGrid(x, y))
    {
        GridMapLiquidData liquid_status;
        GridMapLiquidData *liquid_ptr = data ? data : &liquid_status;
        if (getLiquidStatus(x, y, pZ, MAP_ALL_LIQUIDS, liquid_ptr))
                return true;
    }
    return false;
}

bool Map::IsUnderWater(float x, float y, float z) const
{
    if (const_cast<Map*>(this)->GetGrid(x, y))
    {
        if (getLiquidStatus(x, y, z, MAP_LIQUID_TYPE_WATER|MAP_LIQUID_TYPE_OCEAN)&LIQUID_MAP_UNDER_WATER)
            return true;
    }
    return false;
}

bool Map::CheckGridIntegrity(Creature* c, bool moved) const
{
    Cell const& cur_cell = c->GetCurrentCell();

    CellPair xy_val = Trinity::ComputeCellPair(c->GetPositionX(), c->GetPositionY());
    Cell xy_cell(xy_val);
    if (xy_cell != cur_cell)
    {
        sLog->outDebug(LOG_FILTER_MAPS, "Creature (GUID: %u) X: %f Y: %f (%s) is in grid[%u,%u]cell[%u,%u] instead of grid[%u,%u]cell[%u,%u]",
            c->GetGUIDLow(),
            c->GetPositionX(),c->GetPositionY(),(moved ? "final" : "original"),
            cur_cell.GridX(), cur_cell.GridY(), cur_cell.CellX(), cur_cell.CellY(),
            xy_cell.GridX(),  xy_cell.GridY(),  xy_cell.CellX(),  xy_cell.CellY());
        return true;                                        // not crash at error, just output error in debug mode
    }

    return true;
}

const char* Map::GetMapName() const
{
    return i_mapEntry ? i_mapEntry->name[sWorld->GetDefaultDbcLocale()] : "UNNAMEDMAP\x0";
}

void Map::UpdateObjectVisibility(WorldObject* obj, Cell cell, CellPair cellpair)
{
    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();
    Trinity::VisibleChangesNotifier notifier(*obj);
    TypeContainerVisitor<Trinity::VisibleChangesNotifier, WorldTypeMapContainer > player_notifier(notifier);
    cell.Visit(cellpair, player_notifier, *this, *obj, obj->GetVisibilityRange());
}

void Map::UpdateObjectsVisibilityFor(Player* player, Cell cell, CellPair cellpair)
{
    Trinity::VisibleNotifier notifier(*player);

    cell.data.Part.reserved = ALL_DISTRICT;
    cell.SetNoCreate();
    TypeContainerVisitor<Trinity::VisibleNotifier, WorldTypeMapContainer > world_notifier(notifier);
    TypeContainerVisitor<Trinity::VisibleNotifier, GridTypeMapContainer  > grid_notifier(notifier);
    cell.Visit(cellpair, world_notifier, *this, *player, player->GetVisibilityRange());
    cell.Visit(cellpair, grid_notifier,  *this, *player, player->GetVisibilityRange());

    // send data
    notifier.SendToSelf();
}

void Map::SendInitSelf(Player * player)
{
    sLog->outDetail("Creating player data for himself %u", player->GetGUIDLow());

    UpdateData data;

    // attach to player data current transport data
    if (Transport* transport = player->GetTransport())
    {
        transport->BuildCreateUpdateBlockForPlayer(&data, player);
    }

    // build data for self presence in world at own client (one time for map)
    player->BuildCreateUpdateBlockForPlayer(&data, player);

    // build other passengers at transport also (they always visible and marked as visible and will not send at visibility update at add to map
    if (Transport* transport = player->GetTransport())
    {
        for (Transport::PlayerSet::const_iterator itr = transport->GetPassengers().begin(); itr != transport->GetPassengers().end(); ++itr)
        {
            if (player != (*itr) && player->HaveAtClient(*itr))
            {
                (*itr)->BuildCreateUpdateBlockForPlayer(&data, player);
            }
        }
    }

    WorldPacket packet;
    data.BuildPacket(&packet);
    player->GetSession()->SendPacket(&packet);
}

void Map::SendInitTransports(Player * player)
{
    // Hack to send out transports
    MapManager::TransportMap& tmap = sMapMgr->m_TransportsByMap;

    // no transports at map
    if (tmap.find(player->GetMapId()) == tmap.end())
        return;

    UpdateData transData;

    MapManager::TransportSet& tset = tmap[player->GetMapId()];

    for (MapManager::TransportSet::const_iterator i = tset.begin(); i != tset.end(); ++i)
    {
        // send data for current transport in other place
        if ((*i) != player->GetTransport() && (*i)->GetMapId() == GetId())
        {
            (*i)->BuildCreateUpdateBlockForPlayer(&transData, player);
        }
    }

    WorldPacket packet;
    transData.BuildPacket(&packet);
    player->GetSession()->SendPacket(&packet);
}

void Map::SendRemoveTransports(Player * player)
{
    // Hack to send out transports
    MapManager::TransportMap& tmap = sMapMgr->m_TransportsByMap;

    // no transports at map
    if (tmap.find(player->GetMapId()) == tmap.end())
        return;

    UpdateData transData;

    MapManager::TransportSet& tset = tmap[player->GetMapId()];

    // except used transport
    for (MapManager::TransportSet::const_iterator i = tset.begin(); i != tset.end(); ++i)
        if ((*i) != player->GetTransport() && (*i)->GetMapId() != GetId())
            (*i)->BuildOutOfRangeUpdateBlock(&transData);

    WorldPacket packet;
    transData.BuildPacket(&packet);
    player->GetSession()->SendPacket(&packet);
}

inline void Map::setNGrid(NGridType *grid, uint32 x, uint32 y)
{
    if (x >= MAX_NUMBER_OF_GRIDS || y >= MAX_NUMBER_OF_GRIDS)
    {
        sLog->outError("map::setNGrid() Invalid grid coordinates found: %d, %d!",x,y);
        ASSERT(false);
    }
    i_grids[x][y] = grid;
}

void Map::DelayedUpdate(const uint32 t_diff)
{
    RemoveAllObjectsInRemoveList();

    // Don't unload grids if it's battleground, since we may have manually added GOs,creatures, those doesn't load from DB at grid re-load !
    // This isn't really bother us, since as soon as we have instanced BG-s, the whole map unloads as the BG gets ended
    if (!IsBattlegroundOrArena())
    {
        for (GridRefManager<NGridType>::iterator i = GridRefManager<NGridType>::begin(); i != GridRefManager<NGridType>::end();)
        {
            NGridType *grid = i->getSource();
            GridInfo *info = i->getSource()->getGridInfoRef();
            ++i;                                                // The update might delete the map and we need the next map before the iterator gets invalid
            ASSERT(grid->GetGridState() >= 0 && grid->GetGridState() < MAX_GRID_STATE);
            si_GridStates[grid->GetGridState()]->Update(*this, *grid, *info, grid->getX(), grid->getY(), t_diff);
        }
    }
}

void Map::AddObjectToRemoveList(WorldObject *obj)
{
    ASSERT(obj->GetMapId() == GetId() && obj->GetInstanceId() == GetInstanceId());

    obj->CleanupsBeforeDelete(false);                            // remove or simplify at least cross referenced links

    i_objectsToRemove.insert(obj);
    //sLog->outDebug(LOG_FILTER_MAPS, "Object (GUID: %u TypeId: %u) added to removing list.",obj->GetGUIDLow(),obj->GetTypeId());
}

void Map::AddObjectToSwitchList(WorldObject *obj, bool on)
{
    ASSERT(obj->GetMapId() == GetId() && obj->GetInstanceId() == GetInstanceId());

    std::map<WorldObject*, bool>::iterator itr = i_objectsToSwitch.find(obj);
    if (itr == i_objectsToSwitch.end())
        i_objectsToSwitch.insert(itr, std::make_pair(obj, on));
    else if (itr->second != on)
        i_objectsToSwitch.erase(itr);
    else
        ASSERT(false);
}

void Map::RemoveAllObjectsInRemoveList()
{
    while (!i_objectsToSwitch.empty())
    {
        std::map<WorldObject*, bool>::iterator itr = i_objectsToSwitch.begin();
        WorldObject *obj = itr->first;
        bool on = itr->second;
        i_objectsToSwitch.erase(itr);

        switch(obj->GetTypeId())
        {
            case TYPEID_UNIT:
                if (!obj->ToCreature()->isPet())
                    SwitchGridContainers(obj->ToCreature(), on);
                break;
            default:
                break;
        }
    }

    while (!i_objectsToRemove.empty())
    {
        std::set<WorldObject*>::iterator itr = i_objectsToRemove.begin();
        WorldObject* obj = *itr;

        switch(obj->GetTypeId())
        {
            case TYPEID_CORPSE:
            {
                Corpse* corpse = ObjectAccessor::GetCorpse(*obj, obj->GetGUID());
                if (!corpse)
                    sLog->outError("Tried to delete corpse/bones %u that is not in map.", obj->GetGUIDLow());
                else
                    Remove(corpse,true);
                break;
            }
        case TYPEID_DYNAMICOBJECT:
            Remove((DynamicObject*)obj,true);
            break;
        case TYPEID_GAMEOBJECT:
            Remove((GameObject*)obj,true);
            break;
        case TYPEID_UNIT:
            // in case triggered sequence some spell can continue casting after prev CleanupsBeforeDelete call
            // make sure that like sources auras/etc removed before destructor start
            obj->ToCreature()->CleanupsBeforeDelete();
            Remove(obj->ToCreature(),true);
            break;
        default:
            sLog->outError("Non-grid object (TypeId: %u) is in grid object remove list, ignored.",obj->GetTypeId());
            break;
        }

        i_objectsToRemove.erase(itr);
    }
}

uint32 Map::GetPlayersCountExceptGMs() const
{
    uint32 count = 0;
    for (MapRefManager::const_iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        if (!itr->getSource()->isGameMaster())
            ++count;
    return count;
}

void Map::SendToPlayers(WorldPacket const* data) const
{
    for (MapRefManager::const_iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        itr->getSource()->GetSession()->SendPacket(data);
}

bool Map::ActiveObjectsNearGrid(uint32 x, uint32 y) const
{
    ASSERT(x < MAX_NUMBER_OF_GRIDS);
    ASSERT(y < MAX_NUMBER_OF_GRIDS);

    CellPair cell_min(x*MAX_NUMBER_OF_CELLS, y*MAX_NUMBER_OF_CELLS);
    CellPair cell_max(cell_min.x_coord + MAX_NUMBER_OF_CELLS, cell_min.y_coord+MAX_NUMBER_OF_CELLS);

    //we must find visible range in cells so we unload only non-visible cells...
    float viewDist = GetVisibilityRange();
    int cell_range = (int)ceilf(viewDist / SIZE_OF_GRID_CELL) + 1;

    cell_min << cell_range;
    cell_min -= cell_range;
    cell_max >> cell_range;
    cell_max += cell_range;

    for (MapRefManager::const_iterator iter = m_mapRefManager.begin(); iter != m_mapRefManager.end(); ++iter)
    {
        Player* plr = iter->getSource();

        CellPair p = Trinity::ComputeCellPair(plr->GetPositionX(), plr->GetPositionY());
        if ((cell_min.x_coord <= p.x_coord && p.x_coord <= cell_max.x_coord) &&
            (cell_min.y_coord <= p.y_coord && p.y_coord <= cell_max.y_coord))
            return true;
    }

    for (ActiveNonPlayers::const_iterator iter = m_activeNonPlayers.begin(); iter != m_activeNonPlayers.end(); ++iter)
    {
        WorldObject* obj = *iter;

        CellPair p = Trinity::ComputeCellPair(obj->GetPositionX(), obj->GetPositionY());
        if ((cell_min.x_coord <= p.x_coord && p.x_coord <= cell_max.x_coord) &&
            (cell_min.y_coord <= p.y_coord && p.y_coord <= cell_max.y_coord))
            return true;
    }

    return false;
}

void Map::AddToActive(Creature* c)
{
    AddToActiveHelper(c);

    // also not allow unloading spawn grid to prevent creating creature clone at load
    if (!c->isPet() && c->GetDBTableGUIDLow())
    {
        float x,y,z;
        c->GetRespawnCoord(x,y,z);
        GridPair p = Trinity::ComputeGridPair(x, y);
        if (getNGrid(p.x_coord, p.y_coord))
            getNGrid(p.x_coord, p.y_coord)->incUnloadActiveLock();
        else
        {
            GridPair p2 = Trinity::ComputeGridPair(c->GetPositionX(), c->GetPositionY());
            sLog->outError("Active creature (GUID: %u Entry: %u) added to grid[%u,%u] but spawn grid[%u,%u] was not loaded.",
                c->GetGUIDLow(), c->GetEntry(), p.x_coord, p.y_coord, p2.x_coord, p2.y_coord);
        }
    }
}

void Map::RemoveFromActive(Creature* c)
{
    RemoveFromActiveHelper(c);

    // also allow unloading spawn grid
    if (!c->isPet() && c->GetDBTableGUIDLow())
    {
        float x,y,z;
        c->GetRespawnCoord(x,y,z);
        GridPair p = Trinity::ComputeGridPair(x, y);
        if (getNGrid(p.x_coord, p.y_coord))
            getNGrid(p.x_coord, p.y_coord)->decUnloadActiveLock();
        else
        {
            GridPair p2 = Trinity::ComputeGridPair(c->GetPositionX(), c->GetPositionY());
            sLog->outError("Active creature (GUID: %u Entry: %u) removed from grid[%u,%u] but spawn grid[%u,%u] was not loaded.",
                c->GetGUIDLow(), c->GetEntry(), p.x_coord, p.y_coord, p2.x_coord, p2.y_coord);
        }
    }
}

template void Map::Add(Corpse *);
template void Map::Add(Creature *);
template void Map::Add(GameObject *);
template void Map::Add(DynamicObject *);

template void Map::Remove(Corpse *,bool);
template void Map::Remove(Creature *,bool);
template void Map::Remove(GameObject *, bool);
template void Map::Remove(DynamicObject *, bool);

/* ******* Dungeon Instance Maps ******* */

InstanceMap::InstanceMap(uint32 id, time_t expiry, uint32 InstanceId, uint8 SpawnMode, Map* _parent)
  : Map(id, expiry, InstanceId, SpawnMode, _parent),
    m_resetAfterUnload(false), m_unloadWhenEmpty(false),
    i_data(NULL), i_script_id(0)
{
    //lets initialize visibility distance for dungeons
    InstanceMap::InitVisibilityDistance();

    // the timer is started by default, and stopped when the first player joins
    // this make sure it gets unloaded if for some reason no player joins
    m_unloadTimer = std::max(sWorld->getIntConfig(CONFIG_INSTANCE_UNLOAD_DELAY), (uint32)MIN_UNLOAD_DELAY);
}

InstanceMap::~InstanceMap()
{
    delete i_data;
    i_data = NULL;
}

void InstanceMap::InitVisibilityDistance()
{
    //init visibility distance for instances
    m_VisibleDistance = World::GetMaxVisibleDistanceInInstances();
    m_VisibilityNotifyPeriod = World::GetVisibilityNotifyPeriodInInstances();
}

/*
    Do map specific checks to see if the player can enter
*/
bool InstanceMap::CanEnter(Player *player)
{
    if (player->GetMapRef().getTarget() == this)
    {
        sLog->outError("InstanceMap::CanEnter - player %s(%u) already in map %d,%d,%d!", player->GetName(), player->GetGUIDLow(), GetId(), GetInstanceId(), GetSpawnMode());
        ASSERT(false);
        return false;
    }

    // allow GM's to enter
    if (player->isGameMaster())
        return Map::CanEnter(player);

    // cannot enter if the instance is full (player cap), GMs don't count
    uint32 maxPlayers = GetMaxPlayers();
    if (GetPlayersCountExceptGMs() >= maxPlayers)
    {
        sLog->outDetail("MAP: Instance '%u' of map '%s' cannot have more than '%u' players. Player '%s' rejected", GetInstanceId(), GetMapName(), maxPlayers, player->GetName());
        player->SendTransferAborted(GetId(), TRANSFER_ABORT_MAX_PLAYERS);
        return false;
    }

    // cannot enter while an encounter is in progress on raids
    if (IsRaid() && GetInstanceScript() && GetInstanceScript()->IsEncounterInProgress())
    {
        player->SendTransferAborted(GetId(), TRANSFER_ABORT_ZONE_IN_COMBAT);
        return false;
    }

    // cannot enter if instance is in use by another party/soloer that have a
    // permanent save in the same instance id

    PlayerList const &playerList = GetPlayers();

    if (!playerList.isEmpty())
        for (PlayerList::const_iterator i = playerList.begin(); i != playerList.end(); ++i)
            if (Player *iPlayer = i->getSource())
            {
                if (iPlayer->isGameMaster()) // bypass GMs
                    continue;
                if (!player->GetGroup()) // player has not group and there is someone inside, deny entry
                {
                    player->SendTransferAborted(GetId(), TRANSFER_ABORT_MAX_PLAYERS);
                    return false;
                }
                // player inside instance has no group or his groups is different to entering player's one, deny entry
                if (!iPlayer->GetGroup() || iPlayer->GetGroup() != player->GetGroup() )
                {
                    player->SendTransferAborted(GetId(), TRANSFER_ABORT_MAX_PLAYERS);
                    return false;
                }
                break;
            }

    return Map::CanEnter(player);
}

/*
    Do map specific checks and add the player to the map if successful.
*/
bool InstanceMap::Add(Player *player)
{
    // TODO: Not sure about checking player level: already done in HandleAreaTriggerOpcode
    // GMs still can teleport player in instance.
    // Is it needed?

    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, Lock, false);
        // Check moved to void WorldSession::HandleMoveWorldportAckOpcode()

        // Dungeon only code
        if (IsDungeon())
        {
            Group *group = player->GetGroup();

            // increase current instances (hourly limit)
            if (!group || !group->isLFGGroup())
                player->AddInstanceEnterTime(GetInstanceId(), time(NULL));

            // get or create an instance save for the map
            InstanceSave *mapSave = sInstanceSaveMgr->GetInstanceSave(GetInstanceId());
            if (!mapSave)
            {
                sLog->outDetail("InstanceMap::Add: creating instance save for map %d spawnmode %d with instance id %d", GetId(), GetSpawnMode(), GetInstanceId());
                mapSave = sInstanceSaveMgr->AddInstanceSave(GetId(), GetInstanceId(), Difficulty(GetSpawnMode()), 0, true);
            }

            // check for existing instance binds
            InstancePlayerBind *playerBind = player->GetBoundInstance(GetId(), Difficulty(GetSpawnMode()));
            if (playerBind && playerBind->perm)
            {
                // cannot enter other instances if bound permanently
                if (playerBind->save != mapSave)
                {
                    sLog->outError("InstanceMap::Add: player %s(%d) is permanently bound to instance %d,%d,%d,%d,%d,%d but he is being put into instance %d,%d,%d,%d,%d,%d", player->GetName(), player->GetGUIDLow(), playerBind->save->GetMapId(), playerBind->save->GetInstanceId(), playerBind->save->GetDifficulty(), playerBind->save->GetPlayerCount(), playerBind->save->GetGroupCount(), playerBind->save->CanReset(), mapSave->GetMapId(), mapSave->GetInstanceId(), mapSave->GetDifficulty(), mapSave->GetPlayerCount(), mapSave->GetGroupCount(), mapSave->CanReset());
                    return false;
                }
            }
            else
            {
                if (group)
                {
                    // solo saves should be reset when entering a group
                    InstanceGroupBind *groupBind = group->GetBoundInstance(this);
                    if (playerBind)
                    {
                        sLog->outError("InstanceMap::Add: player %s(%d) is being put into instance %d,%d,%d,%d,%d,%d but he is in group %d and is bound to instance %d,%d,%d,%d,%d,%d!", player->GetName(), player->GetGUIDLow(), mapSave->GetMapId(), mapSave->GetInstanceId(), mapSave->GetDifficulty(), mapSave->GetPlayerCount(), mapSave->GetGroupCount(), mapSave->CanReset(), GUID_LOPART(group->GetLeaderGUID()), playerBind->save->GetMapId(), playerBind->save->GetInstanceId(), playerBind->save->GetDifficulty(), playerBind->save->GetPlayerCount(), playerBind->save->GetGroupCount(), playerBind->save->CanReset());
                        if (groupBind)
                            sLog->outError("InstanceMap::Add: the group is bound to the instance %d,%d,%d,%d,%d,%d", groupBind->save->GetMapId(), groupBind->save->GetInstanceId(), groupBind->save->GetDifficulty(), groupBind->save->GetPlayerCount(), groupBind->save->GetGroupCount(), groupBind->save->CanReset());
                        return false;
                    }
                    // bind to the group or keep using the group save
                    if (!groupBind)
                        group->BindToInstance(mapSave, false);
                    else
                    {
                        // cannot jump to a different instance without resetting it
                        if (groupBind->save != mapSave)
                        {
                            sLog->outError("InstanceMap::Add: player %s(%d) is being put into instance %d,%d,%d but he is in group %d which is bound to instance %d,%d,%d!", player->GetName(), player->GetGUIDLow(), mapSave->GetMapId(), mapSave->GetInstanceId(), mapSave->GetDifficulty(), GUID_LOPART(group->GetLeaderGUID()), groupBind->save->GetMapId(), groupBind->save->GetInstanceId(), groupBind->save->GetDifficulty());
                            if (mapSave)
                                sLog->outError("MapSave players: %d, group count: %d", mapSave->GetPlayerCount(), mapSave->GetGroupCount());
                            else
                                sLog->outError("MapSave NULL");
                            if (groupBind->save)
                                sLog->outError("GroupBind save players: %d, group count: %d", groupBind->save->GetPlayerCount(), groupBind->save->GetGroupCount());
                            else
                                sLog->outError("GroupBind save NULL");
                            return false;
                        }
                        // if the group/leader is permanently bound to the instance
                        // players also become permanently bound when they enter
                        if (groupBind->perm)
                        {
                            WorldPacket data(SMSG_INSTANCE_LOCK_WARNING_QUERY, 9);
                            data << uint32(60000);
                            data << uint32(i_data ? i_data->GetCompletedEncounterMask() : 0);
                            data << uint8(0);
                            player->GetSession()->SendPacket(&data);
                            player->SetPendingBind(mapSave, 60000);
                        }
                    }
                }
                else
                {
                    // set up a solo bind or continue using it
                    if (!playerBind)
                        player->BindToInstance(mapSave, false);
                    else
                        // cannot jump to a different instance without resetting it
                        ASSERT(playerBind->save == mapSave);
                }
            }
        }

        // for normal instances cancel the reset schedule when the
        // first player enters (no players yet)
        SetResetSchedule(false);

        sLog->outDetail("MAP: Player '%s' entered instance '%u' of map '%s'", player->GetName(), GetInstanceId(), GetMapName());
        // initialize unload state
        m_unloadTimer = 0;
        m_resetAfterUnload = false;
        m_unloadWhenEmpty = false;
    }

    // this will acquire the same mutex so it cannot be in the previous block
    Map::Add(player);

    if (i_data)
        i_data->OnPlayerEnter(player);

    return true;
}

void InstanceMap::Update(const uint32& t_diff)
{
    Map::Update(t_diff);

    if (i_data)
        i_data->Update(t_diff);
}

void InstanceMap::Remove(Player *player, bool remove)
{
    sLog->outDetail("MAP: Removing player '%s' from instance '%u' of map '%s' before relocating to another map", player->GetName(), GetInstanceId(), GetMapName());
    //if last player set unload timer
    if (!m_unloadTimer && m_mapRefManager.getSize() == 1)
        m_unloadTimer = m_unloadWhenEmpty ? MIN_UNLOAD_DELAY : std::max(sWorld->getIntConfig(CONFIG_INSTANCE_UNLOAD_DELAY), (uint32)MIN_UNLOAD_DELAY);
    Map::Remove(player, remove);
    // for normal instances schedule the reset after all players have left
    SetResetSchedule(true);
}

void InstanceMap::CreateInstanceData(bool load)
{
    if (i_data != NULL)
        return;

    InstanceTemplate const* mInstance = ObjectMgr::GetInstanceTemplate(GetId());
    if (mInstance)
    {
        i_script_id = mInstance->script_id;
        i_data = sScriptMgr->CreateInstanceData(this);
    }

    if (!i_data)
        return;

    i_data->Initialize();

    if (load)
    {
        // TODO: make a global storage for this
        QueryResult result = CharacterDatabase.PQuery("SELECT data, completedEncounters FROM instance WHERE map = '%u' AND id = '%u'", GetId(), i_InstanceId);
        if (result)
        {
            Field* fields = result->Fetch();
            std::string data = fields[0].GetString();
            i_data->SetCompletedEncountersMask(fields[1].GetUInt32());
            if (data != "")
            {
                sLog->outDebug(LOG_FILTER_MAPS, "Loading instance data for `%s` with id %u", sObjectMgr->GetScriptName(i_script_id), i_InstanceId);
                i_data->Load(data.c_str());
            }
        }
    }
}

/*
    Returns true if there are no players in the instance
*/
bool InstanceMap::Reset(uint8 method)
{
    // note: since the map may not be loaded when the instance needs to be reset
    // the instance must be deleted from the DB by InstanceSaveManager

    if (HavePlayers())
    {
        if (method == INSTANCE_RESET_ALL || method == INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // notify the players to leave the instance so it can be reset
            for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
                itr->getSource()->SendResetFailedNotify(GetId());
        }
        else
        {
            if (method == INSTANCE_RESET_GLOBAL)
                // set the homebind timer for players inside (1 minute)
                for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
                    itr->getSource()->m_InstanceValid = false;

            // the unload timer is not started
            // instead the map will unload immediately after the players have left
            m_unloadWhenEmpty = true;
            m_resetAfterUnload = true;
        }
    }
    else
    {
        // unloaded at next update
        m_unloadTimer = MIN_UNLOAD_DELAY;
        m_resetAfterUnload = true;
    }

    return m_mapRefManager.isEmpty();
}

void InstanceMap::PermBindAllPlayers(Player *player)
{
    if (!IsDungeon())
        return;

    InstanceSave *save = sInstanceSaveMgr->GetInstanceSave(GetInstanceId());
    if (!save)
    {
        sLog->outError("Cannot bind players, no instance save available for map!");
        return;
    }

    Group *group = player->GetGroup();
    // group members outside the instance group don't get bound
    for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
    {
        Player* plr = itr->getSource();
        // players inside an instance cannot be bound to other instances
        // some players may already be permanently bound, in this case nothing happens
        InstancePlayerBind *bind = plr->GetBoundInstance(save->GetMapId(), save->GetDifficulty());
        if (!bind || !bind->perm)
        {
            plr->BindToInstance(save, true);
            WorldPacket data(SMSG_INSTANCE_SAVE_CREATED, 4);
            data << uint32(0);
            plr->GetSession()->SendPacket(&data);
        }

        // if the leader is not in the instance the group will not get a perm bind
        if (group && group->GetLeaderGUID() == plr->GetGUID())
            group->BindToInstance(save, true);
    }
}

void InstanceMap::UnloadAll()
{
    ASSERT(!HavePlayers());

    if (m_resetAfterUnload == true)
        sObjectMgr->DeleteRespawnTimeForInstance(GetInstanceId());

    Map::UnloadAll();
}

void InstanceMap::SendResetWarnings(uint32 timeLeft) const
{
    for (MapRefManager::const_iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
        itr->getSource()->SendInstanceResetWarning(GetId(), itr->getSource()->GetDifficulty(IsRaid()), timeLeft);
}

void InstanceMap::SetResetSchedule(bool on)
{
    // only for normal instances
    // the reset time is only scheduled when there are no payers inside
    // it is assumed that the reset time will rarely (if ever) change while the reset is scheduled
    if (IsDungeon() && !HavePlayers() && !IsRaidOrHeroicDungeon())
    {
        if (InstanceSave *save = sInstanceSaveMgr->GetInstanceSave(GetInstanceId()))
            sInstanceSaveMgr->ScheduleReset(on, save->GetResetTime(), InstanceSaveManager::InstResetEvent(0, GetId(), Difficulty(GetSpawnMode()), GetInstanceId()));
        else
        {          
            sLog->outError("InstanceMap::SetResetSchedule: cannot turn schedule %s, no save available for instance (map [id: %u, name: %s], instance id: %u)",
                on ? "on" : "off", GetId(), GetMapName(), GetInstanceId());
        }
    }
}

MapDifficulty const* Map::GetMapDifficulty() const
{
    return GetMapDifficultyData(GetId(),GetDifficulty());
}

uint32 InstanceMap::GetMaxPlayers() const
{
    if (MapDifficulty const* mapDiff = GetMapDifficulty())
    {
        if (mapDiff->maxPlayers || IsRegularDifficulty())    // Normal case (expect that regular difficulty always have correct maxplayers)
            return mapDiff->maxPlayers;
        else                                                // DBC have 0 maxplayers for heroic instances with expansion < 2
        {                                                   // The heroic entry exists, so we don't have to check anything, simply return normal max players
            MapDifficulty const* normalDiff = GetMapDifficultyData(GetId(), REGULAR_DIFFICULTY);
            return normalDiff ? normalDiff->maxPlayers : 0;
        }
    }
    else                                                    // I'd rather ASSERT(false);
        return 0;
}

uint32 InstanceMap::GetMaxResetDelay() const
{
    MapDifficulty const* mapDiff = GetMapDifficulty();
    return mapDiff ? mapDiff->resetTime : 0;
}

/* ******* Battleground Instance Maps ******* */

BattlegroundMap::BattlegroundMap(uint32 id, time_t expiry, uint32 InstanceId, Map* _parent, uint8 spawnMode)
  : Map(id, expiry, InstanceId, spawnMode, _parent)
{
    //lets initialize visibility distance for BG/Arenas
    BattlegroundMap::InitVisibilityDistance();
}

BattlegroundMap::~BattlegroundMap()
{
}

void BattlegroundMap::InitVisibilityDistance()
{
    //init visibility distance for BG/Arenas
    m_VisibleDistance = World::GetMaxVisibleDistanceInBGArenas();
    m_VisibilityNotifyPeriod = World::GetVisibilityNotifyPeriodInBGArenas();
}

bool BattlegroundMap::CanEnter(Player * player)
{
    if (player->GetMapRef().getTarget() == this)
    {
        sLog->outError("BGMap::CanEnter - player %u is already in map!", player->GetGUIDLow());
        ASSERT(false);
        return false;
    }

    if (player->GetBattlegroundId() != GetInstanceId())
        return false;

    // player number limit is checked in bgmgr, no need to do it here

    return Map::CanEnter(player);
}

bool BattlegroundMap::Add(Player * player)
{
    {
        ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, Lock, false);
        // reset instance validity, battleground maps do not homebind
        player->m_InstanceValid = true;
    }
    return Map::Add(player);
}

void BattlegroundMap::Remove(Player *player, bool remove)
{
    sLog->outDetail("MAP: Removing player '%s' from bg '%u' of map '%s' before relocating to another map", player->GetName(), GetInstanceId(), GetMapName());
    Map::Remove(player, remove);
}

void BattlegroundMap::SetUnload()
{
    m_unloadTimer = MIN_UNLOAD_DELAY;
}

void BattlegroundMap::RemoveAllPlayers()
{
    if (HavePlayers())
        for (MapRefManager::iterator itr = m_mapRefManager.begin(); itr != m_mapRefManager.end(); ++itr)
            if (Player* plr = itr->getSource())
                if (!plr->IsBeingTeleportedFar())
                {
                    plr->TeleportTo(plr->GetBattlegroundEntryPoint());
                    plr->ResurrectPlayer(100);
                }
}

Creature*
Map::GetCreature(uint64 guid)
{
    return ObjectAccessor::GetObjectInMap(guid, this, (Creature*)NULL);
}

GameObject*
Map::GetGameObject(uint64 guid)
{
    return ObjectAccessor::GetObjectInMap(guid, this, (GameObject*)NULL);
}

DynamicObject*
Map::GetDynamicObject(uint64 guid)
{
    return ObjectAccessor::GetObjectInMap(guid, this, (DynamicObject*)NULL);
}

void Map::UpdateIteratorBack(Player *player)
{
    if (m_mapRefIter == player->GetMapRef())
        m_mapRefIter = m_mapRefIter->nocheck_prev();
}
