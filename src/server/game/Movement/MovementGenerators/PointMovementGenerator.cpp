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

#include "PointMovementGenerator.h"
#include "Errors.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "DestinationHolderImp.h"
#include "TemporarySummon.h"
#include "World.h"

//----- Point Movement Generator

template<class T>
void PointMovementGenerator<T>::_setTargetPosition(T &unit)
{
    float x, y, z;
    GetDestination(x, y, z);

    // get the path to the destination
    PathInfo path(&unit, x, y, z, m_straightPath);
    i_path = path.getFullPath();

    // start movement
    Traveller<T> traveller(unit);
    MoveToNextNode(traveller);

    // send path to client
    float speed = traveller.Speed() * 0.001f; // in ms
    uint32 transitTime = uint32(i_path.GetTotalLength() / speed);

    if (unit.GetTypeId() == TYPEID_PLAYER)
        unit.ToPlayer()->addAnticheatTemporaryImmunity(transitTime + 250);

    unit.SendMonsterMoveByPath(i_path, 1, i_path.size(), transitTime);    
}

template<class T>
void PointMovementGenerator<T>::MoveToNextNode(T &unit)
{
    Traveller<T> traveller(unit);
    PathNode &node = i_path[i_currentNode];
    i_destinationHolder.SetDestination(traveller, node.x, node.y, node.z, false);
}

template<class T>
void PointMovementGenerator<T>::Initialize(T &unit)
{
    if (!unit.IsStopped())
        unit.StopMoving();

    Traveller<T> traveller(unit);

    if(m_usePathfinding)
    {
        _setTargetPosition(unit);
    }
    else
    {
         i_destinationHolder.SetDestination(traveller, i_x, i_y, i_z, true);
    }
}


template<class T>
bool PointMovementGenerator<T>::Update(T &unit, const uint32 &diff)
{
    if (!&unit)
        return false;

    if (unit.HasUnitState(UNIT_STAT_ROOT | UNIT_STAT_STUNNED))
    {
        if (unit.HasUnitState(UNIT_STAT_CHARGING))
            return false;
        else
            return true;
    }

    if (i_path.empty() && m_usePathfinding)
        return false;

    Traveller<T> traveller(unit);

    i_destinationHolder.UpdateTraveller(traveller, diff, false);

    if (m_usePathfinding)
    {
        if (i_destinationHolder.HasArrived())
        {
            ++i_currentNode;

            // if we are at the last node, stop charge
            if (i_currentNode >= i_path.size())
            {
                unit.ClearUnitState(UNIT_STAT_MOVE);
                arrived = true;
                return false;
            }

            MoveToNextNode(traveller);
        }
    }
    else
    {
        if (i_destinationHolder.HasArrived())
        {
            unit.ClearUnitState(UNIT_STAT_MOVE);
            arrived = true;
            return false;
        }
    }
    return true;
}

template<class T>
void PointMovementGenerator<T>::Finalize(T &unit)
{
    if (unit.HasUnitState(UNIT_STAT_CHARGING))
        unit.ClearUnitState(UNIT_STAT_CHARGING | UNIT_STAT_JUMPING);

    if (arrived) // without this crash!
    {
        MovementInform(unit);
    }
}

template<class T>
void PointMovementGenerator<T>::MovementInform(T & /*unit*/)
{
}

template <> void PointMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    if (id == EVENT_FALL_GROUND)
    {
        unit.setDeathState(JUST_DIED);
        unit.SetFlying(true);
    }
    unit.AI()->MovementInform(POINT_MOTION_TYPE, id);
}

template void PointMovementGenerator<Player>::Initialize(Player&);
template bool PointMovementGenerator<Player>::Update(Player &, const uint32 &diff);
template void PointMovementGenerator<Player>::MovementInform(Player&);
template void PointMovementGenerator<Player>::Finalize(Player&);
template void PointMovementGenerator<Player>::MoveToNextNode(Player&);

template void PointMovementGenerator<Creature>::Initialize(Creature&);
template bool PointMovementGenerator<Creature>::Update(Creature&, const uint32 &diff);
template void PointMovementGenerator<Creature>::Finalize(Creature&);
template void PointMovementGenerator<Creature>::MoveToNextNode(Creature&);

void AssistanceMovementGenerator::Finalize(Unit &unit)
{
    unit.ToCreature()->SetNoCallAssistance(false);
    unit.ToCreature()->CallAssistance();
    if (unit.isAlive())
        unit.GetMotionMaster()->MoveSeekAssistanceDistract(sWorld->getIntConfig(CONFIG_CREATURE_FAMILY_ASSISTANCE_DELAY));
}

