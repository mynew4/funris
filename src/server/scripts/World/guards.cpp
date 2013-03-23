/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

/* ScriptData
SDName: Guards
SD%Complete: 100
SDComment:
SDCategory: Guards
EndScriptData */

/* ContentData
guard_generic
guard_shattrath_aldor
guard_shattrath_scryer
EndContentData */

#include "ScriptPCH.h"
#include "GuardAI.h"

enum gSpells
{
    AOE          = 61603,
    MS           = 68784,
    BUBLE        = 67251,
    HAMSTRING    = 62845,
    SPELL_HEAL   = 68013,
    BLADESTORM   = 10308,
    GRIP         = 49576,
    REND         = 49909,
    FREEDOM      = 68758,
    Trinket      = 42292,
    Charge       = 68764
};

enum GuardGeneric
{
    GENERIC_CREATURE_COOLDOWN       = 5000,

    SAY_GUARD_SIL_AGGRO1            = -1070001,
    SAY_GUARD_SIL_AGGRO2            = -1070002,
    SAY_GUARD_SIL_AGGRO3            = -1070003,

    NPC_CENARION_HOLD_INFANTRY      = 15184,
    NPC_STORMWIND_CITY_GUARD        = 68,
    NPC_STORMWIND_CITY_PATROLLER    = 1976,
    NPC_ORGRIMMAR_GRUNT             = 3296
};

class guard_generic : public CreatureScript
{
public:
    guard_generic() : CreatureScript("guard_generic") { }

    struct guard_genericAI : public GuardAI
    {
        guard_genericAI(Creature* creature) : GuardAI(creature) {}

        void Reset()
        {
            globalCooldown = 0;
            buffTimer = 0;
            MS_Timer = 5000;
            AOE_Timer = 7000;
            BUBLE_Timer = 37000;
            HAMSTRING_Timer = 1500;
            HEAL_Timer = 15500;
            BLADESTORM_Timer = 10100;
            GRIP_Timer = 10000;
            REND_Timer = 6000;
            Freedom_Timer = 4000;
            Charge_Timer = 9000;
        }

        void EnterCombat(Unit* who)
        {
            if (me->GetEntry() == NPC_CENARION_HOLD_INFANTRY)
                DoScriptText(RAND(SAY_GUARD_SIL_AGGRO1,SAY_GUARD_SIL_AGGRO2,SAY_GUARD_SIL_AGGRO3), me, who);
            if (SpellEntry const* spell = me->reachWithSpellAttack(who))
                DoCast(who, spell->Id);
        }

        void UpdateAI(const uint32 diff)
        {
             //Always decrease our global cooldown first
            if (globalCooldown > diff)
                globalCooldown -= diff;
            else
                globalCooldown = 0;

            //Buff timer (only buff when we are alive and not in combat
            if (me->isAlive() && !me->isInCombat())
            {
                if (buffTimer <= diff)
                {
                    //Find a spell that targets friendly and applies an aura (these are generally buffs)
                    SpellEntry const *info = SelectSpell(me, 0, 0, SELECT_TARGET_ANY_FRIEND, 0, 0, 0, 0, SELECT_EFFECT_AURA);

                    if (info && !globalCooldown)
                    {
                        //Cast the buff spell
                        DoCast(me, info->Id);

                        //Set our global cooldown
                        globalCooldown = GENERIC_CREATURE_COOLDOWN;

                        //Set our timer to 10 minutes before rebuff
                        buffTimer = 600000;
                    }                                                   //Try again in 30 seconds
                    else buffTimer = 30000;
                } else buffTimer -= diff;
            }

            //Return since we have no target
            if (!UpdateVictim())
                return;

            // Make sure our attack is ready and we arn't currently casting
            if (me->isAttackReady() && !me->IsNonMeleeSpellCasted(false))
            {
                //If we are within range melee the target
                if (me->IsWithinMeleeRange(me->getVictim()))
                {
                    bool healing = false;
                    SpellEntry const *info = NULL;

                    //Select a healing spell if less than 30% hp
                    if (me->HealthBelowPct(30))
                        info = SelectSpell(me, 0, 0, SELECT_TARGET_ANY_FRIEND, 0, 0, 0, 0, SELECT_EFFECT_HEALING);

                    //No healing spell available, select a hostile spell
                    if (info)
                        healing = true;
                    else
                        info = SelectSpell(me->getVictim(), 0, 0, SELECT_TARGET_ANY_ENEMY, 0, 0, 0, 0, SELECT_EFFECT_DONTCARE);

                    //20% chance to replace our white hit with a spell
                    if (info && urand(0,99) < 20 && !globalCooldown)
                    {
                        //Cast the spell
                        if (healing)
                            DoCast(me, info->Id);
                        else
                            DoCast(me->getVictim(), info->Id);

                        //Set our global cooldown
                        globalCooldown = GENERIC_CREATURE_COOLDOWN;
                    }
                    else
                        me->AttackerStateUpdate(me->getVictim());

                    me->resetAttackTimer();
                }
            }
            else
            {
                //Only run this code if we arn't already casting
                if (!me->IsNonMeleeSpellCasted(false))
                {
                    bool healing = false;
                    SpellEntry const *info = NULL;

                    //Select a healing spell if less than 30% hp ONLY 33% of the time
                    if (me->HealthBelowPct(30) && 33 > urand(0, 99))
                        info = SelectSpell(me, 0, 0, SELECT_TARGET_ANY_FRIEND, 0, 0, 0, 0, SELECT_EFFECT_HEALING);

                    //No healing spell available, See if we can cast a ranged spell (Range must be greater than ATTACK_DISTANCE)
                    if (info)
                        healing = true;
                    else
                        info = SelectSpell(me->getVictim(), 0, 0, SELECT_TARGET_ANY_ENEMY, 0, 0, NOMINAL_MELEE_RANGE, 0, SELECT_EFFECT_DONTCARE);

                    //Found a spell, check if we arn't on cooldown
                    if (info && !globalCooldown)
                    {
                        //If we are currently moving stop us and set the movement generator
                        if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() != IDLE_MOTION_TYPE)
                        {
                            me->GetMotionMaster()->Clear(false);
                            me->GetMotionMaster()->MoveIdle();
                        }

                        //Cast spell
                        if (healing)
                            DoCast(me,info->Id);
                        else
                            DoCast(me->getVictim(),info->Id);

                        //Set our global cooldown
                        globalCooldown = GENERIC_CREATURE_COOLDOWN;

                    }                                               //If no spells available and we arn't moving run to target
                    else if (me->GetMotionMaster()->GetCurrentMovementGeneratorType() != TARGETED_MOTION_TYPE)
                    {
                        //Cancel our current spell and then mutate new movement generator
                        me->InterruptNonMeleeSpells(false);
                        me->GetMotionMaster()->Clear(false);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                    }
                }
            }

            if (Freedom_Timer < diff)
            {
                DoCast(me, FREEDOM);
                Freedom_Timer = 5000;
            }
            else
            Freedom_Timer -=diff;
    
            if (Charge_Timer < diff)
            {
                DoCast(me->getVictim(), Charge);
                Charge_Timer = 5000;
            }
            else
            Charge_Timer -=diff;

            if (BLADESTORM_Timer < diff)
            {
                DoCast(me->getVictim(), BLADESTORM);
                BLADESTORM_Timer = 10000;
            }
            else
            BLADESTORM_Timer -=diff;
    
            if (MS_Timer < diff)
            {
                DoCast(me->getVictim(), MS);
                MS_Timer = 10000;
            }
            else
            MS_Timer -=diff;
    
            if (REND_Timer < diff)
            {
                DoCast(me->getVictim(), REND);
                REND_Timer = 11000;
            }
            else
            REND_Timer -=diff;
    
            if (GRIP_Timer < diff)
            {
                DoCast(me->getVictim(), GRIP);
                GRIP_Timer = 10000;
            }
            else
            GRIP_Timer -=diff;
    
            if (AOE_Timer < diff)
            {
                DoCast(me->getVictim(), AOE);
                AOE_Timer = 10000;
            }
            else
            AOE_Timer -=diff;
    
            if (BUBLE_Timer < diff)
            {
                DoCast(me, BUBLE);
                BUBLE_Timer = 37000;
            }
            else
            BUBLE_Timer -=diff;
    
            if (HAMSTRING_Timer < diff)
            {
                DoCast(me->getVictim(), HAMSTRING);
                HAMSTRING_Timer = 5000;
            }
            else
            HAMSTRING_Timer -=diff;
    
            if (HEAL_Timer < diff && HealthBelowPct(30))
            {
                DoCast(me, SPELL_HEAL);
                HEAL_Timer = 16000;
            }
            else
            HEAL_Timer -=diff;
            
            DoMeleeAttackIfReady();
        }

        void DoReplyToTextEmote(uint32 emote)
        {
            switch(emote)
            {
                case TEXTEMOTE_KISS:    me->HandleEmoteCommand(EMOTE_ONESHOT_BOW);    break;
                case TEXTEMOTE_WAVE:    me->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);   break;
                case TEXTEMOTE_SALUTE:  me->HandleEmoteCommand(EMOTE_ONESHOT_SALUTE); break;
                case TEXTEMOTE_SHY:     me->HandleEmoteCommand(EMOTE_ONESHOT_FLEX);   break;
                case TEXTEMOTE_RUDE:
                case TEXTEMOTE_CHICKEN: me->HandleEmoteCommand(EMOTE_ONESHOT_POINT);  break;
            }
        }

        void ReceiveEmote(Player* player, uint32 textEmote)
        {
            switch(me->GetEntry())
            {
                case NPC_STORMWIND_CITY_GUARD:
                case NPC_STORMWIND_CITY_PATROLLER:
                case NPC_ORGRIMMAR_GRUNT:
                    break;
                default:
                    return;
            }

            if (!me->IsFriendlyTo(player))
                return;

            DoReplyToTextEmote(textEmote);
        }

    private:
        uint32 globalCooldown;
        uint32 buffTimer;
        uint32 AOE_Timer;
        uint32 MS_Timer;
        uint32 BUBLE_Timer;
        uint32 HAMSTRING_Timer;
        uint32 HEAL_Timer;
        uint32 BLADESTORM_Timer;
        uint32 GRIP_Timer;
        uint32 REND_Timer;
        uint32 Freedom_Timer;
        uint32 Trinket_Timer;
        uint32 Charge_Timer;
    };

    CreatureAI *GetAI(Creature* creature) const
    {
       return new guard_genericAI(creature);
    }
};

enum GuardShattrath
{
    SPELL_BANISHED_SHATTRATH_A = 36642,
    SPELL_BANISHED_SHATTRATH_S = 36671,
    SPELL_BANISH_TELEPORT      = 36643,
    SPELL_EXILE                = 39533
};

class guard_shattrath_scryer : public CreatureScript
{
public:
    guard_shattrath_scryer() : CreatureScript("guard_shattrath_scryer") { }

    struct guard_shattrath_scryerAI : public GuardAI
    {
        guard_shattrath_scryerAI(Creature* creature) : GuardAI(creature) {}

        void Reset()
        {
            banishTimer = 5000;
            exileTimer = 8500;
            playerGUID = 0;
            canTeleport = false;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (canTeleport)
            {
                if (exileTimer <= diff)
                {
                    if (Unit* temp = Unit::GetUnit(*me,playerGUID))
                    {
                        temp->CastSpell(temp,SPELL_EXILE,true);
                        temp->CastSpell(temp,SPELL_BANISH_TELEPORT,true);
                    }
                    playerGUID = 0;
                    exileTimer = 8500;
                    canTeleport = false;
                } else exileTimer -= diff;
            }
            else if (banishTimer <= diff)
            {
                Unit* temp = me->getVictim();
                if (temp && temp->GetTypeId() == TYPEID_PLAYER)
                {
                    DoCast(temp, SPELL_BANISHED_SHATTRATH_A);
                    banishTimer = 9000;
                    playerGUID = temp->GetGUID();
                    if (playerGUID)
                        canTeleport = true;
                }
            } else banishTimer -= diff;

            DoMeleeAttackIfReady();
        }

    private:
        uint32 exileTimer;
        uint32 banishTimer;
        uint64 playerGUID;
        bool canTeleport;
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new guard_shattrath_scryerAI(creature);
    }
};

class guard_shattrath_aldor : public CreatureScript
{
public:
    guard_shattrath_aldor() : CreatureScript("guard_shattrath_aldor") { }

    struct guard_shattrath_aldorAI : public GuardAI
    {
        guard_shattrath_aldorAI(Creature* creature) : GuardAI(creature) {}

        void Reset()
        {
            banishTimer = 5000;
            exileTimer = 8500;
            playerGUID = 0;
            canTeleport = false;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (canTeleport)
            {
                if (exileTimer <= diff)
                {
                    if (Unit* temp = Unit::GetUnit(*me,playerGUID))
                    {
                        temp->CastSpell(temp,SPELL_EXILE,true);
                        temp->CastSpell(temp,SPELL_BANISH_TELEPORT,true);
                    }
                    playerGUID = 0;
                    exileTimer = 8500;
                    canTeleport = false;
                } else exileTimer -= diff;
            }
            else if (banishTimer <= diff)
            {
                Unit* temp = me->getVictim();
                if (temp && temp->GetTypeId() == TYPEID_PLAYER)
                {
                    DoCast(temp, SPELL_BANISHED_SHATTRATH_S);
                    banishTimer = 9000;
                    playerGUID = temp->GetGUID();
                    if (playerGUID)
                        canTeleport = true;
                }
            } else banishTimer -= diff;

            DoMeleeAttackIfReady();
        }
    private:
        uint32 exileTimer;
        uint32 banishTimer;
        uint64 playerGUID;
        bool canTeleport;
    };

    CreatureAI *GetAI(Creature *creature) const
    {
        return new guard_shattrath_aldorAI(creature);
    }
};

void AddSC_guards()
{
    new guard_generic;
    new guard_shattrath_aldor;
    new guard_shattrath_scryer;
}
