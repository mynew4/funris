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

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "Corpse.h"
#include "Player.h"
#include "SpellAuras.h"
#include "MapManager.h"
#include "Transport.h"
#include "Battleground.h"
#include "WaypointMovementGenerator.h"
#include "InstanceSaveMgr.h"
#include "ObjectMgr.h"
#include "BattlegroundRV.h"
#include "WardenMgr.h"

/*Movement anticheat DEBUG defines */
//#define MOVEMENT_ANTICHEAT_DEBUG true
#define MOVEMENT_ANTICHEAT_ALARM_LOG true
//#define MOVEMENT_ANTICHEAT_DEBUG_FULL true
/*end Movement anticheate defines*/

void WorldSession::HandleMoveWorldportAckOpcode(WorldPacket & /*recv_data*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: got MSG_MOVE_WORLDPORT_ACK.");
    HandleMoveWorldportAckOpcode();
}

void WorldSession::HandleMoveWorldportAckOpcode()
{
    // ignore unexpected far teleports
    if (!GetPlayer()->IsBeingTeleportedFar())
        return;

    GetPlayer()->SetSemaphoreTeleportFar(false);

    // get the teleport destination
    WorldLocation &loc = GetPlayer()->GetTeleportDest();

    // possible errors in the coordinate validity check
    if (!MapManager::IsValidMapCoord(loc))
    {
        LogoutPlayer(false);
        return;
    }

    // get the destination map entry, not the current one, this will fix homebind and reset greeting
    MapEntry const* mEntry = sMapStore.LookupEntry(loc.GetMapId());
    InstanceTemplate const* mInstance = ObjectMgr::GetInstanceTemplate(loc.GetMapId());

    // reset instance validity, except if going to an instance inside an instance
    if (GetPlayer()->m_InstanceValid == false && !mInstance)
        GetPlayer()->m_InstanceValid = true;

    Map * oldMap = GetPlayer()->GetMap();
    ASSERT(oldMap);
    if (GetPlayer()->IsInWorld())
    {
        sLog->outCrash("Player is still in world when teleported from map %u! to new map %u", oldMap->GetId(), loc.GetMapId());
        oldMap->Remove(GetPlayer(), false);
    }

    uint32 Zone = GetPlayer()->GetZoneId();
    // relocate the player to the teleport destination
    Map * newMap = sMapMgr->CreateMap(loc.GetMapId(), GetPlayer(), 0);
    // the CanEnter checks are done in TeleporTo but conditions may change
    // while the player is in transit, for example the map may get full
    if (!newMap || !newMap->CanEnter(GetPlayer()))
    {
        sLog->outError("Map %d could not be created for player %d, porting player to homebind", loc.GetMapId(), GetPlayer()->GetGUIDLow());
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }
    else
        GetPlayer()->Relocate(&loc);

    GetPlayer()->ResetMap();
    GetPlayer()->SetMap(newMap);

    GetPlayer()->SendInitialPacketsBeforeAddToMap();
    if (!GetPlayer()->GetMap()->Add(GetPlayer()))
    {
        sLog->outError("WORLD: failed to teleport player %s (%d) to map %d because of unknown reason!", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow(), loc.GetMapId());
        GetPlayer()->ResetMap();
        GetPlayer()->SetMap(oldMap);
        GetPlayer()->TeleportTo(GetPlayer()->m_homebindMapId, GetPlayer()->m_homebindX, GetPlayer()->m_homebindY, GetPlayer()->m_homebindZ, GetPlayer()->GetOrientation());
        return;
    }

    // battleground state prepare (in case join to BG), at relogin/tele player not invited
    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();
    // only add to bg group and object, if the player was invited (else he entered through command)
    if (_player->InBattleground())
    {
        // cleanup setting if outdated
        if (!mEntry->IsBattlegroundOrArena())
        {
            // We're not in BG
            _player->SetBattlegroundId(0, BATTLEGROUND_TYPE_NONE);
            // reset destination bg team
            _player->SetBGTeam(0);
        }
        // join to bg case
        else if (Battleground *bg = _player->GetBattleground())
        {
            if (_player->IsInvitedForBattlegroundInstance(_player->GetBattlegroundId()))
                bg->AddPlayer(_player);
            if( bg->isArena() && sWorld->getBoolConfig(CONFIG_ANTIDODGE))
                _player->Oldzone = Zone;
        }
    }

    GetPlayer()->SendInitialPacketsAfterAddToMap();

    // flight fast teleport case
    if (GetPlayer()->GetMotionMaster()->GetCurrentMovementGeneratorType() == FLIGHT_MOTION_TYPE)
    {
        if (!_player->InBattleground())
        {
            // short preparations to continue flight
            FlightPathMovementGenerator* flight = (FlightPathMovementGenerator*)(GetPlayer()->GetMotionMaster()->top());
            flight->Initialize(*GetPlayer());
            return;
        }

        // battleground state prepare, stop flight
        GetPlayer()->GetMotionMaster()->MovementExpired();
        GetPlayer()->CleanupAfterTaxiFlight();
    }

    // resurrect character at enter into instance where his corpse exist after add to map
    Corpse *corpse = GetPlayer()->GetCorpse();
    if (corpse && corpse->GetType() != CORPSE_BONES && corpse->GetMapId() == GetPlayer()->GetMapId())
    {
        if (mEntry->IsDungeon())
        {
            GetPlayer()->ResurrectPlayer(0.5f,false);
            GetPlayer()->SpawnCorpseBones();
        }
    }

    bool allowMount = !mEntry->IsDungeon() || mEntry->IsBattlegroundOrArena();
    if (mInstance)
    {
        Difficulty diff = GetPlayer()->GetDifficulty(mEntry->IsRaid());
        if (MapDifficulty const* mapDiff = GetMapDifficultyData(mEntry->MapID, diff))
        {
            if (mapDiff->resetTime)
            {
                if (time_t timeReset = sInstanceSaveMgr->GetResetTimeFor(mEntry->MapID, diff))
                {
                    uint32 timeleft = uint32(timeReset - time(NULL));
                    GetPlayer()->SendInstanceResetWarning(mEntry->MapID, diff, timeleft);
                }
            }
        }
        allowMount = mInstance->allowMount;
    }

    // mount allow check
    if (!allowMount)
        _player->RemoveAurasByType(SPELL_AURA_MOUNTED);

    // update zone immediately, otherwise leave channel will cause crash in mtmap
    uint32 newzone, newarea;
    GetPlayer()->GetZoneAndAreaId(newzone, newarea);
    GetPlayer()->UpdateZone(newzone, newarea);

    // honorless target
    if (GetPlayer()->pvpInfo.inHostileArea)
        GetPlayer()->CastSpell(GetPlayer(), 2479, true);

    // in friendly area
    else if (GetPlayer()->IsPvP() && !GetPlayer()->HasFlag(PLAYER_FLAGS,PLAYER_FLAGS_IN_PVP))
        GetPlayer()->UpdatePvP(false, false);

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMoveTeleportAck(WorldPacket& recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "MSG_MOVE_TELEPORT_ACK");
    uint64 guid;

    recv_data.readPackGUID(guid);

    uint32 flags, time;
    recv_data >> flags >> time;
    sLog->outStaticDebug("Guid " UI64FMTD, guid);
    sLog->outStaticDebug("Flags %u, time %u", flags, time/IN_MILLISECONDS);

    Unit *mover = _player->m_mover;
    Player *plMover = mover->GetTypeId() == TYPEID_PLAYER ? (Player*)mover : NULL;

    if (!plMover || !plMover->IsBeingTeleportedNear())
        return;

    if (guid != plMover->GetGUID())
        return;

    plMover->SetSemaphoreTeleportNear(false);

    uint32 old_zone = plMover->GetZoneId();

    WorldLocation const& dest = plMover->GetTeleportDest();

    plMover->SetPosition(dest,true);

    uint32 newzone, newarea;
    plMover->GetZoneAndAreaId(newzone, newarea);
    plMover->UpdateZone(newzone, newarea);

    // new zone
    if (old_zone != newzone)
    {
        // honorless target
        if (plMover->pvpInfo.inHostileArea)
            plMover->CastSpell(plMover, 2479, true);

        // in friendly area
        else if (plMover->IsPvP() && !plMover->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_IN_PVP))
            plMover->UpdatePvP(false, false);
    }

    // resummon pet
    GetPlayer()->ResummonPetTemporaryUnSummonedIfAny();

    //lets process all delayed operations on successful teleport
    GetPlayer()->ProcessDelayedOperations();
}

void WorldSession::HandleMovementOpcodes(WorldPacket & recv_data)
{
    uint16 opcode = recv_data.GetOpcode();
    recv_data.hexlike();

    if (!_player)
        return;

    Unit *mover = _player->m_mover;

    ASSERT(mover != NULL);                                  // there must always be a mover

    Player *plMover = mover->ToPlayer();
    Vehicle *vehMover = mover->GetVehicleKit();

    if (vehMover)            
        if (mover->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PLAYER_CONTROLLED))
            if (Unit *charmer = mover->GetCharmer())
                if (charmer->GetTypeId() == TYPEID_PLAYER)
                    plMover = charmer->ToPlayer();


    // ignore, waiting processing in WorldSession::HandleMoveWorldportAckOpcode and WorldSession::HandleMoveTeleportAck
    if (plMover && plMover->IsBeingTeleported())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    /* extract packet */
    uint64 guid = 0;

    recv_data.readPackGUID(guid);

    if (!guid)
        return;

    MovementInfo movementInfo;
    movementInfo.guid = guid;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.rpos(recv_data.wpos());                   // prevent warnings spam

    // prevent tampered movement data
    if (!mover || guid != mover->GetGUID())
        return;

    if (!movementInfo.pos.IsPositionValid())
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    if (plMover && opcode == CMSG_MOVE_CHNG_TRANSPORT && plMover->m_anti_TransportGUID != 0)
    {
        plMover->addAnticheatTemporaryImmunity();
    }

    /* handle special cases */
    if (movementInfo.flags & MOVEMENTFLAG_ONTRANSPORT)
    {
        // transports size limited
        // (also received at zeppelin leave by some reason with t_* as absolute in continent coordinates, can be safely skipped)
        if( movementInfo.t_pos.GetPositionX() > 60 || movementInfo.t_pos.GetPositionY() > 60 || movementInfo.t_pos.GetPositionX() < -60 ||  movementInfo.t_pos.GetPositionY() < -60 )
        {
            recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
            return;
        }

        if (!Trinity::IsValidMapCoord(movementInfo.pos.GetPositionX() + movementInfo.t_pos.GetPositionX(), movementInfo.pos.GetPositionY() + movementInfo.t_pos.GetPositionY(),
            movementInfo.pos.GetPositionZ() + movementInfo.t_pos.GetPositionZ(), movementInfo.pos.GetOrientation() + movementInfo.t_pos.GetOrientation()))
        {
            recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
            return;
        }

        if (plMover && plMover->m_anti_TransportGUID == 0 && (movementInfo.t_guid != 0))
        {
            // if we boarded a transport, add us to it
            if (!plMover->GetTransport())
            {
                // elevators also cause the client to send MOVEMENTFLAG_ONTRANSPORT - just unmount if the guid can be found in the transport list
                for (MapManager::TransportSet::const_iterator iter = sMapMgr->m_Transports.begin(); iter != sMapMgr->m_Transports.end(); ++iter)
                {
                    if ((*iter)->GetGUID() == movementInfo.t_guid)
                    {
                        plMover->m_transport = (*iter);
                        (*iter)->AddPassenger(plMover);
                        break;
                    }
                }
            }
        }

        //movement anticheat;
        //Correct finding GO guid in DB (thanks to GriffonHeart)
        if (plMover)
        {
            if(GameObject *obj = HashMapHolder<GameObject>::Find(movementInfo.t_guid))
                plMover->m_anti_TransportGUID = obj->GetDBTableGUIDLow() ? obj->GetDBTableGUIDLow() : obj->GetGUID();
            else
                plMover->m_anti_TransportGUID = GUID_LOPART(movementInfo.t_guid);
        }
        // end movement anticheat

        if ( (!plMover && !mover->GetTransport() && !mover->GetVehicle()) || (plMover && !plMover->GetTransport() && !plMover->GetVehicle()) )
        {
            GameObject *go = mover->GetMap()->GetGameObject(movementInfo.t_guid);
            if (!go || go->GetGoType() != GAMEOBJECT_TYPE_TRANSPORT)
                movementInfo.flags &= ~MOVEMENTFLAG_ONTRANSPORT;
        }
    }
    else if (plMover && plMover->m_anti_TransportGUID != 0)       // if we were on a transport, leave
    {
        if (plMover->GetTransport())
        {
            plMover->m_transport->RemovePassenger(plMover);
            plMover->m_transport = NULL;
        }        
        movementInfo.t_pos.Relocate(0.0f, 0.0f, 0.0f, 0.0f);
        movementInfo.t_time = 0;
        movementInfo.t_seat = -1;
        plMover->m_anti_TransportGUID = 0;        
    }

    

    // fall damage generation (ignore in flight case that can be triggered also at lags in moment teleportation to another map).
    if (opcode == MSG_MOVE_FALL_LAND && plMover && !plMover->isInFlight())
    {
        //movement anticheat
        plMover->m_anti_JustJumped = 0;
        plMover->m_anti_JumpBaseZ = 0;
        //end movement anticheat
        plMover->HandleFall(movementInfo);
    }

    if (plMover && ((movementInfo.flags & MOVEMENTFLAG_SWIMMING) != 0) != plMover->IsInWater())
    {
        // now client not include swimming flag in case jumping under water
        plMover->SetInWater(!plMover->IsInWater() || plMover->GetBaseMap()->IsUnderWater(movementInfo.pos.GetPositionX(), movementInfo.pos.GetPositionY(), movementInfo.pos.GetPositionZ()));
    }

    /*----------------------*/
        //---- anti-cheat features -->>>
    bool check_passed = true;
    #ifdef MOVEMENT_ANTICHEAT_DEBUG_FULL
    if (plMover){
        sLog->outBasic("IAC-%s > client-time:%d fall-time:%d | xyzo: %f,%f,%fo(%f) flags[%X] opcode[%s]| transport (xyzo): %f,%f,%fo(%f)",
            plMover->GetName(),movementInfo.time,movementInfo.fallTime,movementInfo.x,movementInfo.y,movementInfo.z,movementInfo.o,
            movementInfo.flags, LookupOpcodeName(opcode),movementInfo.t_x,movementInfo.t_y,movementInfo.t_z,movementInfo.t_o);
        sLog->outBasic("IAC-%s Transport > server GUID: %d |  client GUID: (lo)%d - (hi)%d",
            plMover->GetName(),plMover->m_anti_TransportGUID, GUID_LOPART(movementInfo.t_guid), GUID_HIPART(movementInfo.t_guid));
    } else {
        sLog->outBasic("IAC > client-time:%d fall-time:%d | xyzo: %f,%f,%fo(%f) flags[%X] opcode[%s]| transport (xyzo): %f,%f,%fo(%f)",
            movementInfo.time,movementInfo.fallTime,movementInfo.x,movementInfo.y,movementInfo.z,movementInfo.o,
            movementInfo.flags, LookupOpcodeName(opcode),movementInfo.t_x,movementInfo.t_y,movementInfo.t_z,movementInfo.t_o);
        sLog->outBasic("IAC Transport > server GUID:  |  client GUID: (lo)%d - (hi)%d",
            GUID_LOPART(movementInfo.t_guid), GUID_HIPART(movementInfo.t_guid));
    }
    #endif

    if ( plMover && World::GetEnableMvAnticheat() && !plMover->HasAuraType(SPELL_AURA_MOD_POSSESS) && !plMover->HasAura(56266) && !(vehMover && vehMover->GetBase()->HasUnitState(UNIT_STAT_CHARGING)))
    {
        //calc time deltas
        int32 cClientTimeDelta = 1500;
        if (plMover->m_anti_LastClientTime !=0)
        {
            cClientTimeDelta = movementInfo.time - plMover->m_anti_LastClientTime;
            plMover->m_anti_DeltaClientTime += cClientTimeDelta;
            plMover->m_anti_LastClientTime = movementInfo.time;
        } 
        else 
        {
            plMover->m_anti_LastClientTime = movementInfo.time;
        }

        const uint32 cServerTime = getMSTime();
        uint32 cServerTimeDelta = 1500;

        if (plMover->m_anti_LastServerTime != 0)
        {
            cServerTimeDelta = cServerTime - plMover->m_anti_LastServerTime;
            plMover->m_anti_DeltaServerTime += cServerTimeDelta;
            plMover->m_anti_LastServerTime = cServerTime;
        } 
        else 
        {
            plMover->m_anti_LastServerTime = cServerTime;
        }

        //resync times on client login (first 15 sec for heavy areas)
        if (plMover->m_anti_DeltaServerTime < 15000 && plMover->m_anti_DeltaClientTime < 15000)
            plMover->m_anti_DeltaClientTime = plMover->m_anti_DeltaServerTime;

        const int32 sync_time = plMover->m_anti_DeltaClientTime - plMover->m_anti_DeltaServerTime;

        #ifdef MOVEMENT_ANTICHEAT_DEBUG_FULL
        sLog->outBasic("IAC-%s Time > cClientTimeDelta: %d, cServerTime: %d || deltaC: %d - deltaS: %d || SyncTime: %d",
                        plMover->GetName(),cClientTimeDelta, cServerTime,
                        plMover->m_anti_DeltaClientTime, plMover->m_anti_DeltaServerTime, sync_time);
        #endif

        //mistiming checks
        const int32 gmd = World::GetMistimingDelta();

        if (sync_time > gmd || sync_time < -gmd)
        {
            cClientTimeDelta = cServerTimeDelta;
            plMover->m_anti_MistimingCount++;

            #ifdef MOVEMENT_ANTICHEAT_DEBUG
            sLog->outError("IAC-%s, mistiming exception. #:%d, mistiming: %dms ",
                            plMover->GetName(), plMover->m_anti_MistimingCount, sync_time);
            #endif

            if (!sWardenMgr->IsEnabled() && (plMover->m_anti_MistimingCount > World::GetMistimingAlarms()))
            {
                #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                    sLog->outCheater("IAC-%s disconnected due to mistiming exceptions limit.", plMover->GetName());
                #endif
                plMover->GetSession()->KickPlayer();
                return;
            }

            check_passed = false;

            if (vehMover)
            {
                if (plMover && plMover->IsMounted())
                {
                    plMover->Unmount();
                }
                else
                    vehMover->Dismiss();
            }
        }
        // end mistiming checks

        uint32 curDest = plMover->m_taxi.GetTaxiDestination(); //check taxi flight
        if ( (plMover->m_anti_TransportGUID == 0) && !curDest )
        {

            UnitMoveType move_type;

            // calculating section ---------------------
            //current speed
            // if the three flags FLY/SWIM/WALK are set exclusively, this SWITCH is equivalent to IF/ELSE commented below
            switch(movementInfo.flags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_WALKING))
            {
                case MOVEMENTFLAG_FLYING:
                    move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
                    break;
                case MOVEMENTFLAG_SWIMMING:
                    move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
                    break;
                case MOVEMENTFLAG_WALKING:
                    move_type = MOVE_WALK;
                    break;
                default:
                    move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_RUN;
                    break;
            }
            //if (movementInfo.flags & MOVEMENTFLAG_FLYING) move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_FLIGHT_BACK : MOVE_FLIGHT;
            //else if (movementInfo.flags & MOVEMENTFLAG_SWIMMING) move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_SWIM;
            //else if (movementInfo.flags & MOVEMENTFLAG_WALK_MODE) move_type = MOVE_WALK;
            //hmm... in first time after login player has MOVE_SWIMBACK instead MOVE_WALKBACK
            //else move_type = movementInfo.flags & MOVEMENTFLAG_BACKWARD ? MOVE_SWIM_BACK : MOVE_RUN;

            float current_speed = plMover->HasUnitState(UNIT_STAT_CHARGING) ? plMover->m_TempSpeed : plMover->GetSpeed(move_type);

            bool vehicleCanFly = false;
            bool vehicleIsCreature = false;

            if ( plMover->GetVehicle() )
            {
                if (Creature *vehCreature  = plMover->GetVehicleCreatureBase())
                {    
                    vehicleIsCreature = true;
                    vehicleCanFly = vehCreature->canFly();
                }

                current_speed = plMover->GetVehicleBase()->GetSpeed(move_type);
            }
            // end current speed

            // movement distance
            float allowed_delta = 0;

            const float delta_x = plMover->GetPositionX() - movementInfo.pos.m_positionX;
            const float delta_y = plMover->GetPositionY() - movementInfo.pos.m_positionY;
            const float delta_z = plMover->GetPositionZ() - movementInfo.pos.m_positionZ;
            const float real_delta = delta_x * delta_x + delta_y * delta_y;
            float tg_z = -99999; //tangens
            // end movement distance

            if ( cClientTimeDelta < 0 ) 
                cClientTimeDelta = 0;

            float time_delta = (cClientTimeDelta < 1500) ? (float)cClientTimeDelta * 0.001f : 1.5f; //normalize time - 1.5 second allowed for heavy loaded server

            if ( !(movementInfo.flags & (MOVEMENTFLAG_FLYING | MOVEMENTFLAG_SWIMMING)) )
                tg_z = (real_delta != 0 ) ? (delta_z*delta_z / real_delta) : -99999;

            if ( current_speed < plMover->m_anti_Last_HSpeed )
            {
                allowed_delta = plMover->m_anti_Last_HSpeed;
                if (plMover->m_anti_LastSpeedChangeTime == 0 )
                    plMover->m_anti_LastSpeedChangeTime = movementInfo.time + (uint32)floor(((plMover->m_anti_Last_HSpeed / current_speed) * 1500)) + 200; //100ms above for random fluctuating =)))
            } 
            else
            {
                allowed_delta = current_speed;
            }
            
            allowed_delta = allowed_delta * time_delta;
            allowed_delta = allowed_delta * allowed_delta + 2;

            if ( movementInfo.time > plMover->m_anti_LastSpeedChangeTime )
            {
                plMover->m_anti_Last_HSpeed = current_speed; // store current speed
                plMover->m_anti_Last_VSpeed = -2.3f;
                if (plMover->m_anti_LastSpeedChangeTime != 0) plMover->m_anti_LastSpeedChangeTime = 0;
            }

            if (sWardenMgr->IsEnabled())
                allowed_delta *= 1.5f;

            if ( plMover->GetVehicle() )
            {
                allowed_delta *= (plMover->GetVehicle()->GetVehicleInfo()->m_ID == 349) ? 10.0f : 1.1f;    // hack for Argent mount charging
            }  

            const uint32 wardenImmunity = sWardenMgr->IsEnabled() ? 500 : 0;

            const uint32 immunityTime = plMover->m_anti_temporaryImmunity + cServerTimeDelta + sWorld->GetUpdateTime()
                + plMover->GetSession()->GetLatency() + wardenImmunity;
            // end calculating section ---------------------

            const bool flying_allowed = (plMover->HasAuraType(SPELL_AURA_FLY) || plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED)
                || plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) || plMover->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED)
                || plMover->HasAuraType(SPELL_AURA_MOD_MOUNTED_FLIGHT_SPEED_ALWAYS) || ( vehicleIsCreature && vehicleCanFly ));            

            const bool hasTimedImmunity = immunityTime >= cServerTime;

            //AntiGravitation
            if (!sWardenMgr->IsEnabled() && (plMover->m_anti_JumpBaseZ != 0))
            {
                float JumpHeight = plMover->m_anti_JumpBaseZ - movementInfo.pos.m_positionZ;
                if ( !(movementInfo.flags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_FLYING)) && (JumpHeight < plMover->m_anti_Last_VSpeed))
                {
                    if (!hasTimedImmunity)
                    {
                        #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                        sLog->outCheater("IAC: %s, AntiGravitation alert.", plMover->GetName());
                        #endif
                        check_passed = false;
                    }
                }
            }
 
            //multi jump checks
            if (opcode == MSG_MOVE_JUMP && !plMover->IsInWater())
            {
                if (plMover->m_anti_JustJumped >= 1)
                {
                    check_passed = false; //don't process new jump packet
                    #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                        sLog->outCheater("IAC: %s, multijump alert", plMover->GetName());
                    #endif
                }
                else
                {
                    plMover->m_anti_JustJumped += 1;
                    plMover->m_anti_JumpBaseZ = movementInfo.pos.m_positionZ;
                }
            }
            else if (plMover->IsInWater()) plMover->m_anti_JustJumped = 0;

            //speed hack checks
            if ((real_delta > allowed_delta && real_delta < 4900.0f) && (delta_z < (plMover->m_anti_Last_VSpeed * time_delta) || delta_z < 1) )
            {
                if (!hasTimedImmunity && !(sWardenMgr->IsEnabled() && plMover->GetMapId() == 617) )
                {
                    #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                    sLog->outCheater("IAC: %s, speedhack alert | map: %u | cDelta = %f aDelta = %f | cSpeed = %f lSpeed = %f deltaTime = %f | serverTime = %u, immunityTime = %u",
                        plMover->GetName(), plMover->GetMapId(), real_delta, allowed_delta, current_speed, plMover->m_anti_Last_HSpeed, time_delta, cServerTime, immunityTime);
                    #endif
                    check_passed = false;
                }                    
            }

            //teleport hack checks
            if ( (real_delta >= 4900.0f ) && !(real_delta < allowed_delta) )
            {
                #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                sLog->outCheater("IAC: %s, teleport alert | map: %u | cDelta = %f aDelta = %f | cSpeed = %f lSpeed = %f deltaTime = %f",
                    plMover->GetName(), plMover->GetMapId(), real_delta, allowed_delta, current_speed, plMover->m_anti_Last_HSpeed, time_delta);
                #endif
                check_passed = false;
            }

            //climb mountain hack checks // 1.56f (delta_z < GetPlayer()->m_anti_Last_VSpeed))
            if ((delta_z < plMover->m_anti_Last_VSpeed) && (plMover->m_anti_JustJumped == 0) && (tg_z > 2.37f) && ((movementInfo.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING)) == 0) )
            {
                if (!sWardenMgr->IsEnabled() && !hasTimedImmunity)
                {
                    #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                        sLog->outCheater("IAC: %s, wallclimb alert | tg_z = %f", plMover->GetName(), tg_z);
                    #endif
                    check_passed = false;
                }
            }

            //Fly hack checks
            if (((movementInfo.flags & (MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING)) != 0)
                  && !plMover->isGameMaster()
                  && !(movementInfo.flags & (MOVEMENTFLAG_SWIMMING)) // allow X button in water
                  && !flying_allowed)
            {
                if (!hasTimedImmunity)
                {
                    #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                        sLog->outCheater("IAC: %s, flyhack alert.", plMover->GetName());
                    #endif
                    check_passed = false;
                }
            }            

            //Water-Walk checks
            /*if (((movementInfo.flags & MOVEMENTFLAG_WATERWALKING) != 0)
                  && !plMover->isGameMaster()
                  && !(plMover->HasAuraType(SPELL_AURA_WATER_WALK) | plMover->HasAuraType(SPELL_AURA_GHOST)))
            {
                #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                    sLog->outCheater("IAC: %s, waterwalk alert. ", plMover->GetName());
                #endif
                check_passed = false;
            }*/

            //Teleport To Plane checks
            if (movementInfo.pos.m_positionZ < 0.0001f && movementInfo.pos.m_positionZ > -0.0001f
                && ((movementInfo.flags & (MOVEMENTFLAG_SWIMMING | MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_FLYING)) == 0)
                && !plMover->isGameMaster())
            {
                // Prevent using TeleportToPlane.
                if (Map *map = plMover->GetMap())
                {
                    float plane_z = map->GetHeight(movementInfo.pos.m_positionX, movementInfo.pos.m_positionY, MAX_HEIGHT) - movementInfo.pos.m_positionZ;
                    plane_z = (plane_z < -500.0f) ? 0 : plane_z; //check holes in heigth map
                    if(plane_z > 0.1f || plane_z < -0.1f)
                    {
                        plMover->m_anti_TeleToPlane_Count++;
                        check_passed = false;
                        #ifdef MOVEMENT_ANTICHEAT_ALARM_LOG
                        sLog->outCheater("IAC: %s, teleport to plane alert. plane_z: %f ",
                            plMover->GetName(), plane_z);
                        #endif
                        if (plMover->m_anti_TeleToPlane_Count > World::GetTeleportToPlaneAlarms())
                        {
                            sLog->outCheater("IAC: %s, teleport to plane alert. Disconnecting. Alerts count: %d ",
                                plMover->GetName(), plMover->m_anti_TeleToPlane_Count);
                            plMover->GetSession()->KickPlayer();
                            return;
                        }
                    }
                }
            }
            else
            {
                if (plMover->m_anti_TeleToPlane_Count != 0) plMover->m_anti_TeleToPlane_Count = 0;
            }
        } 
        else if (!sWardenMgr->IsEnabled() && (movementInfo.flags & MOVEMENTFLAG_ONTRANSPORT))
        {
            //antiwrap checks
            if (plMover->GetTransport() || plMover->GetVehicle() )
            {
                float trans_rad = movementInfo.t_pos.m_positionX*movementInfo.t_pos.m_positionX + movementInfo.t_pos.m_positionY*movementInfo.t_pos.m_positionY + movementInfo.t_pos.m_positionZ*movementInfo.pos.m_positionZ;
                if (trans_rad > 3600.0f)
                {
                    check_passed = false;
                    #ifdef MOVEMENT_ANTICHEAT_DEBUG
                        sLog->outError("IAC-%s, leave transport.", plMover->GetName());
                    #endif
                }
            }
            else
            {    
                GameObjectData const* go_data = sObjectMgr->GetGOData(plMover->m_anti_TransportGUID);
                GameObject const* go_transport = plMover->GetMap()->GetGameObject(plMover->m_anti_TransportGUID);

                if (go_data || go_transport)
                {
                    float delta_gox;
                    float delta_goy;
                    float delta_goz;
                    float gox;
                    float goy;
                    float goz;
                    uint32 mapid;

                    if (go_data)
                    {
                        gox = go_data->posX;
                        goy = go_data->posY;
                        goz = go_data->posZ;
                        delta_gox = gox - movementInfo.pos.m_positionX;
                        delta_goy = goy - movementInfo.pos.m_positionY;
                        delta_goz = goz - movementInfo.pos.m_positionZ;
                        mapid = go_data->mapid;
                    }
                    else
                    {
                        gox = go_transport->GetPositionX();
                        goy = go_transport->GetPositionY();
                        goz = go_transport->GetPositionZ();
                        delta_gox = gox - movementInfo.pos.m_positionX;
                        delta_goy = goy - movementInfo.pos.m_positionY;
                        delta_goz = goz - movementInfo.pos.m_positionZ;

                        //HACK: for SotA ships. Need more research
                        if (go_transport->GetEntry() == 193182 || go_transport->GetEntry() == 193183 || go_transport->GetEntry() == 193184 || go_transport->GetEntry() == 193185)
                        {
                            delta_gox = 0.0f;
                            delta_goy = 0.0f;
                        }
                        mapid = go_transport->GetMapId();
                    }

                    #ifdef MOVEMENT_ANTICHEAT_DEBUG
                    sLog->outError("IAC-%s, transport movement. GO xyz: %f, %f, %f",
                        plMover->GetName(), gox, goy, goz);
                    #endif
                    if ( plMover->GetMapId() != mapid )
                    {
                        check_passed = false;
                        #ifdef MOVEMENT_ANTICHEAT_DEBUG
                            sLog->outError("IAC-%s, incorrect mapID %u, need %u", plMover->GetName(), plMover->GetMapId(), mapid );
                        #endif
                    } 
                    else if (mapid != 369) 
                    {
                        float delta_go = delta_gox*delta_gox + delta_goy*delta_goy;
                        if (delta_go > 3600.0f) 
                        {
                            check_passed = false;
                            #ifdef MOVEMENT_ANTICHEAT_DEBUG
                                sLog->outError("IAC-%s, leave transport. GO xyz: %f, %f, %f", plMover->GetName(), gox, goy, goz);
                            #endif
                        }
                    }

                } 
                else
                {
                    #ifdef MOVEMENT_ANTICHEAT_DEBUG
                        sLog->outError("IAC-%s, undefined transport.", plMover->GetName());
                    #endif
                    check_passed = false;
                }
            }

            if (!check_passed)
            {
                if ( plMover->GetTransport() && !plMover->GetVehicle() )
                {
                    plMover->m_transport->RemovePassenger(plMover);
                    plMover->m_transport = NULL;
                    plMover->m_anti_TransportGUID = 0;
                }
                else if ( plMover->GetVehicle() && !plMover->GetVehicle()->GetVehicleInfo() )
                {
                    plMover->m_transport->RemovePassenger(plMover);
                    plMover->m_transport = NULL;
                    plMover->GetVehicle()->Dismiss();
                    plMover->m_anti_TransportGUID = 0;
                }

                if (vehMover)
                {
                    if (plMover && plMover->IsMounted())
                        plMover->Unmount();
                    else vehMover->Dismiss();
                }

                movementInfo.t_pos.m_positionX = 0.0f;
                movementInfo.t_pos.m_positionY = 0.0f;
                movementInfo.t_pos.m_positionZ = 0.0f;
                movementInfo.t_pos.m_orientation = 0.0f;
                movementInfo.t_time = 0;                    
            }
        }
    }

    /* process position-change */
    if (check_passed)
    {
        WorldPacket data(opcode, recv_data.size());
        movementInfo.time = getMSTime();
        movementInfo.guid = mover->GetGUID();
        WriteMovementInfo(&data, &movementInfo);
        mover->SendMessageToSet(&data, _player);

        mover->m_movementInfo = movementInfo;

        if (mover->GetVehicle())
        {
            mover->SetOrientation(movementInfo.pos.GetOrientation());
            return;
        }

        mover->SetPosition(movementInfo.pos);
        if(plMover)
            plMover->UpdateFallInformationIfNeed(movementInfo, opcode);

        if (plMover && !vehMover)                                            // nothing is charmed, or player charmed
        {            
            if (plMover->InBattleground()
                && plMover->GetBattleground()
                
            && plMover->GetBattleground()->isArena() && plMover->GetBattleground()->GetStatus() == STATUS_IN_PROGRESS)
            {
                float heigh=0.0f;
                switch(plMover->GetBattleground()->GetTypeID())
                {
                    case BATTLEGROUND_NA: heigh=10.0f; break;
                    case BATTLEGROUND_RV: heigh=((BattlegroundRV*)plMover->GetBattleground())->fencesopened ? 28.0f: 2.0f; break;
                    case BATTLEGROUND_RL: heigh=31.0f; break;
                    case BATTLEGROUND_DS: heigh=3.0f; break;
                    case BATTLEGROUND_BE: heigh=0.5f; break;
                    default:break;
                }
                if(heigh && movementInfo.pos.GetPositionZ()<heigh)
                    plMover->GetBattleground()->HandlePlayerUnderMap(plMover);
            }
            else if (movementInfo.pos.GetPositionZ() < -500.0f)
            {
                // NOTE: this is actually called many times while falling
                // even after the player has been teleported away
                // TODO: discard movement packets after the player is rooted
                if (plMover->isAlive())
                {
                    plMover->EnvironmentalDamage(DAMAGE_FALL_TO_VOID, GetPlayer()->GetMaxHealth());
                    // pl can be alive if GM/etc
                    if (!plMover->isAlive())
                    {
                        // change the death state to CORPSE to prevent the death timer from
                        // starting in the next player update
                        plMover->KillPlayer();
                        plMover->BuildPlayerRepop();
                    }
                }

                // cancel the death timer here if started
                plMover->RepopAtGraveyard();
            }
        }

            if(plMover && plMover->getStandState() == UNIT_STAND_STATE_SIT)
                plMover->SetStandState(UNIT_STAND_STATE_STAND);

            //movement anticheat >>>
            if (plMover && plMover->m_anti_AlarmCount > 0)
            {
                #ifdef MOVEMENT_ANTICHEAT_DEBUG
                    sLog->outError("IAC: %s produce %d anticheat alarms", plMover->GetName(), plMover->m_anti_AlarmCount);
                #endif
                plMover->m_anti_AlarmCount = 0;
            }
            // end movement anticheat
    }
    else if (plMover)
    {
        if (plMover->IsMounted())
            plMover->Unmount();

        if (vehMover)
            vehMover->Dismiss();

        plMover->m_anti_AlarmCount++;
        WorldPacket data;
        plMover->SetUnitMovementFlags(0);
        plMover->SendTeleportAckPacket();
        plMover->BuildHeartBeatMsg(&data);
        plMover->SendMessageToSet(&data, true);
    }
    else if (vehMover)
    {
        vehMover->Dismiss();
    }
}

void WorldSession::HandleForceSpeedChangeAck(WorldPacket &recv_data)
{
    uint32 opcode = recv_data.GetOpcode();
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd %s (%u, 0x%X) opcode", LookupOpcodeName(opcode), opcode, opcode);

    /* extract packet */
    uint64 guid;
    uint32 unk1;
    float  newspeed;

    recv_data.readPackGUID(guid);

    // now can skip not our packet
    if (_player->GetGUID() != guid)
    {
        recv_data.rpos(recv_data.wpos());                   // prevent warnings spam
        return;
    }

    // continue parse packet

    recv_data >> unk1;                                      // counter or moveEvent

    MovementInfo movementInfo;
    movementInfo.guid = guid;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data >> newspeed;
    /*----------------*/

    // client ACK send one packet for mounted/run case and need skip all except last from its
    // in other cases anti-cheat check can be fail in false case
    UnitMoveType move_type;
    UnitMoveType force_move_type;

    static char const* move_type_name[MAX_MOVE_TYPE] = {  "Walk", "Run", "RunBack", "Swim", "SwimBack", "TurnRate", "Flight", "FlightBack", "PitchRate" };

    switch(opcode)
    {
        case CMSG_FORCE_WALK_SPEED_CHANGE_ACK:          move_type = MOVE_WALK;          force_move_type = MOVE_WALK;        break;
        case CMSG_FORCE_RUN_SPEED_CHANGE_ACK:           move_type = MOVE_RUN;           force_move_type = MOVE_RUN;         break;
        case CMSG_FORCE_RUN_BACK_SPEED_CHANGE_ACK:      move_type = MOVE_RUN_BACK;      force_move_type = MOVE_RUN_BACK;    break;
        case CMSG_FORCE_SWIM_SPEED_CHANGE_ACK:          move_type = MOVE_SWIM;          force_move_type = MOVE_SWIM;        break;
        case CMSG_FORCE_SWIM_BACK_SPEED_CHANGE_ACK:     move_type = MOVE_SWIM_BACK;     force_move_type = MOVE_SWIM_BACK;   break;
        case CMSG_FORCE_TURN_RATE_CHANGE_ACK:           move_type = MOVE_TURN_RATE;     force_move_type = MOVE_TURN_RATE;   break;
        case CMSG_FORCE_FLIGHT_SPEED_CHANGE_ACK:        move_type = MOVE_FLIGHT;        force_move_type = MOVE_FLIGHT;      break;
        case CMSG_FORCE_FLIGHT_BACK_SPEED_CHANGE_ACK:   move_type = MOVE_FLIGHT_BACK;   force_move_type = MOVE_FLIGHT_BACK; break;
        case CMSG_FORCE_PITCH_RATE_CHANGE_ACK:          move_type = MOVE_PITCH_RATE;    force_move_type = MOVE_PITCH_RATE;  break;
        default:
            sLog->outError("WorldSession::HandleForceSpeedChangeAck: Unknown move type opcode: %u", opcode);
            return;
    }

    // skip all forced speed changes except last and unexpected
    // in run/mounted case used one ACK and it must be skipped.m_forced_speed_changes[MOVE_RUN} store both.
    if (_player->m_forced_speed_changes[force_move_type] > 0)
    {
        --_player->m_forced_speed_changes[force_move_type];
        if (_player->m_forced_speed_changes[force_move_type] > 0)
            return;
    }

    if (!_player->GetTransport() && fabs(_player->GetSpeed(move_type) - newspeed) > 0.01f)
    {
        if (_player->GetSpeed(move_type) > newspeed)         // must be greater - just correct
        {
            sLog->outError("%sSpeedChange player %s is NOT correct (must be %f instead %f), force set to correct value",
                move_type_name[move_type], _player->GetName(), _player->GetSpeed(move_type), newspeed);
            _player->SetSpeed(move_type,_player->GetSpeedRate(move_type),true);
        }
        else                                                // must be lesser - cheating
        {
            sLog->outBasic("Player %s from account id %u kicked for incorrect speed (must be %f instead %f)",
                _player->GetName(),_player->GetSession()->GetAccountId(),_player->GetSpeed(move_type), newspeed);
            _player->GetSession()->KickPlayer();
        }
    }
}

void WorldSession::HandleSetActiveMoverOpcode(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_SET_ACTIVE_MOVER");

    uint64 guid;
    recv_data >> guid;

    if (GetPlayer()->IsInWorld())
    {
        if (Unit *mover = ObjectAccessor::GetUnit(*GetPlayer(), guid))
        {
            if (mover->GetCharmer() != GetPlayer())
                return;
            GetPlayer()->SetMover(mover);
            if (mover != GetPlayer() && mover->canFly())
            {
                WorldPacket data(SMSG_MOVE_SET_CAN_FLY, 12);
                data.append(mover->GetPackGUID());
                data << uint32(0);
                SendPacket(&data);
            }
        }
        else
        {
            sLog->outError("HandleSetActiveMoverOpcode: incorrect mover guid: mover is " UI64FMTD " and should be " UI64FMTD, guid, _player->m_mover->GetGUID());
            GetPlayer()->SetMover(GetPlayer());
        }
    }
}

void WorldSession::HandleMoveNotActiveMover(WorldPacket &recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_MOVE_NOT_ACTIVE_MOVER");

    uint64 old_mover_guid;
    recv_data.readPackGUID(old_mover_guid);

    MovementInfo mi;
    mi.guid = old_mover_guid;
    ReadMovementInfo(recv_data, &mi);

    _player->m_movementInfo = mi;
}

void WorldSession::HandleMountSpecialAnimOpcode(WorldPacket& /*recv_data*/)
{
    WorldPacket data(SMSG_MOUNTSPECIAL_ANIM, 8);
    data << uint64(GetPlayer()->GetGUID());

    GetPlayer()->SendMessageToSet(&data, false);
}

void WorldSession::HandleMoveKnockBackAck(WorldPacket & recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_MOVE_KNOCK_BACK_ACK");

    uint64 guid;                                            // guid - unused
    recv_data.readPackGUID(guid);

    recv_data.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);
}

void WorldSession::HandleMoveHoverAck(WorldPacket& recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_MOVE_HOVER_ACK");

    uint64 guid;                                            // guid - unused
    recv_data.readPackGUID(guid);

    recv_data.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleMoveWaterWalkAck(WorldPacket& recv_data)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_MOVE_WATER_WALK_ACK");

    uint64 guid;                                            // guid - unused
    recv_data.readPackGUID(guid);

    recv_data.read_skip<uint32>();                          // unk

    MovementInfo movementInfo;
    ReadMovementInfo(recv_data, &movementInfo);

    recv_data.read_skip<uint32>();                          // unk2
}

void WorldSession::HandleSummonResponseOpcode(WorldPacket& recv_data)
{
    if (!_player->isAlive() || _player->isInCombat())
        return;

    uint64 summoner_guid;
    bool agree;
    recv_data >> summoner_guid;
    recv_data >> agree;

    _player->SummonIfPossible(agree);
}

