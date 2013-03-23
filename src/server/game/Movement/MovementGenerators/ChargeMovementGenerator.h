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

#ifndef TRINITY_CHARGEMOVEMENTGENERATOR_H
#define TRINITY_CHARGEMOVEMENTGENERATOR_H

#include "MovementGenerator.h"
#include "DestinationHolder.h"
#include "Path.h"
#include "Traveller.h"
#include "PathFinder.h"

template<class T>

class ChargeMovementGenerator
    : public MovementGeneratorMedium< T, ChargeMovementGenerator<T> >
{
    public:
        ChargeMovementGenerator(uint32 _id, float _x, float _y, float _z, bool _straightPath, Unit* _target, uint32 _chargeSpell) : i_currentNode(0), id(_id),
            i_x(_x), i_y(_y), i_z(_z), arrived(false), casted(false), m_straightPath(_straightPath),
            m_target(_target), m_chargeSpell(_chargeSpell) {}

        void Initialize(T &);
        void Finalize(T &unit);
        void Reset(T &unit){unit.StopMoving();}
        bool Update(T &, const uint32 &diff);

        void MovementInform(T &);

        MovementGeneratorType GetMovementGeneratorType() { return CHARGE_MOTION_TYPE; }

        bool GetDestination(float& x, float& y, float& z) const { x = i_x; y = i_y; z = i_z; return true; }
    private:
        uint32 id;
        float i_x, i_y, i_z;
        bool m_straightPath;
        uint32 m_chargeSpell;
        Unit* m_target;
        bool arrived;
        bool casted;
        void MoveToNextNode(T &);
        void _setTargetPosition(T &);

        uint32 i_currentNode;
        DestinationHolder< Traveller<T> > i_destinationHolder;
        PointPath i_path;
};

#endif
