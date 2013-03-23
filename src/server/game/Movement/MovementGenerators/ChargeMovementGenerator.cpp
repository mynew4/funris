/*
 * Copyright (C) 2010-2011 Izb00shka <http://izbooshka.net/>
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

#include "ChargeMovementGenerator.h"
#include "DestinationHolderImp.h"
#include "Creature.h"
#include "CreatureAI.h"
#include "World.h"
#include "Spell.h"
#include "SpellMgr.h"

//----- Charge Movement Generator


template<class T>
void ChargeMovementGenerator<T>::_setTargetPosition(T &unit)
{
    float x, y, z;
    GetDestination(x, y, z);

    // get the path to the destination
    PathInfo path(&unit, x, y, z, m_straightPath);
    i_path = path.getFullPath();
    if (i_path.GetTotalLength() > 40)
    {
        path.BuildShortcut();
        i_path = path.getFullPath();
    }
    // start movement
    Traveller<T> traveller(unit);
    MoveToNextNode(traveller);

    // send path to client
    float speed = traveller.Speed() * 0.001f; // in ms
    uint32 transitTime = uint32(i_path.GetTotalLength() / speed);

    if (unit.GetTypeId() == TYPEID_PLAYER)
        unit.ToPlayer()->addAnticheatTemporaryImmunity(transitTime + 500);

    unit.MonsterMoveByPath(i_path, 1, i_path.size(), transitTime);    
}

template<class T>
void ChargeMovementGenerator<T>::MoveToNextNode(T &unit)
{
    Traveller<T> traveller(unit);
    PathNode &node = i_path[i_currentNode];
    i_destinationHolder.SetDestination(traveller, node.x, node.y, node.z + 0.05f, false);
}

template<class T>
void ChargeMovementGenerator<T>::Initialize(T &unit)
{
    if (!unit.IsStopped())
        unit.StopMoving();

    _setTargetPosition(unit);
}


template<class T>
bool ChargeMovementGenerator<T>::Update(T &unit, const uint32 &diff)
{
    if (!&unit)
        return false;

    if (unit.HasUnitState(UNIT_STAT_NOT_MOVE))
    {
        return false;
    }

     // if there is no path, stop charge
    if (i_path.empty())
        return false;

    Traveller<T> traveller(unit);

    i_destinationHolder.UpdateTraveller(traveller, diff, false);

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
    return true;
}

template<class T>
void ChargeMovementGenerator<T>:: Finalize(T &unit)
{
    unit.ClearUnitState(UNIT_STAT_CHARGING | UNIT_STAT_JUMPING);

    if (arrived) // without this crash!
    {
        MovementInform(unit);
    }
}

template<class T>
void ChargeMovementGenerator<T>::MovementInform(T & /*unit*/)
{
}

template <> void ChargeMovementGenerator<Creature>::MovementInform(Creature &unit)
{
    unit.AI()->MovementInform(POINT_MOTION_TYPE, id);
}

template void ChargeMovementGenerator<Player>::Initialize(Player&);
template bool ChargeMovementGenerator<Player>::Update(Player &, const uint32 &diff);
template void ChargeMovementGenerator<Player>::MovementInform(Player&);
template void ChargeMovementGenerator<Player>::Finalize(Player&);
template void ChargeMovementGenerator<Player>::MoveToNextNode(Player&);

template void ChargeMovementGenerator<Creature>::Initialize(Creature&);
template bool ChargeMovementGenerator<Creature>::Update(Creature&, const uint32 &diff);
template void ChargeMovementGenerator<Creature>::Finalize(Creature&);
template void ChargeMovementGenerator<Creature>::MoveToNextNode(Creature&);

