#include "ScriptPCH.h"
//#include "SpellAuras.h"
class  boss_godzilla : public CreatureScript
{
public: 
    boss_godzilla(): CreatureScript("boss_godzilla"){};
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_godzillaAI (pCreature);
    }
    struct  boss_godzillaAI : public ScriptedAI
    {
        boss_godzillaAI(Creature *c) : ScriptedAI(c)
        {
            EnterEvadeMode();
        }
        Unit* victim;
        bool debug;
        uint32 beam_timer;
        int victims_count;
        uint32 vound_timer;
        uint32 fear_timer;
        uint32 charge_timer;
        uint32 tailswipe_timer;
        std::list<Creature*> victim_list;
        void Reset()
        {
            RespawnArround();
            charge_timer=20000;
            beam_timer=40000;
            fear_timer=15000;
            tailswipe_timer=20000;
            victims_count=10;
            vound_timer=0;
            debug=false;
            victim = NULL;
            me->SetHealth(me->GetMaxHealth());
        }
        /*void Aggro(Unit *who)
        {
        }*/
        void KilledUnit(Unit *who)
        {
            if(who->GetTypeId()!=TYPEID_PLAYER && ((Creature*)who)->GetEntry()==100006)
            {
                victims_count--;
            }
            if(!victims_count)
                DoCast(me, 26662);
            else
            {
                    //victim_list=DoFindFriendlyMissingBuff(100.0f, 45112);
                    for(std::list<Creature*>::iterator itr = victim_list.begin(); itr != victim_list.end(); ++itr)
                    {
                        if((*itr)->GetEntry()==100006 && (*itr)->isAlive())
                        {
                            victim=(*itr);
                            break;
                        }
                    }
                DoResetThreat();
                me->AddThreat(victim, 100000.0f);
                AttackStart(victim);
//                victim->Attack(me, true);
            }
        }
        bool UpdateVictim()
        {
            if (!me->isInCombat())
                return false;

            if (!me->HasReactState(REACT_PASSIVE))
            {
                if (!victim)
                    victim = me->SelectVictim() ? me->SelectVictim(): victim;
                AttackStart(victim);
                return victim;
            }
            else if (me->getThreatManager().isThreatListEmpty())
            {
                EnterEvadeMode();
                return false;
            }

            return true;
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!victims_count && victim->isDead())
                EnterEvadeMode();

            if(me->getVictim() && charge_timer < diff && me->GetDistance2d(me->getVictim())>=20.0f)
            {
                charge_timer=20000;
                DoCast(me->getVictim(), 58991);
            }else charge_timer-=diff;
            if(beam_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                for(int i=0; i!=10; i++)
                {
                    if(target && target->GetTypeId() == TYPEID_PLAYER)
                        break;
                    target=SelectUnit(SELECT_TARGET_RANDOM, 0);
                }
                const int32 damage=5000;
                me->CastCustomSpell(target, 59965, &damage, 0, 0, false);
                beam_timer=40000;
            } else beam_timer-=diff;
            if(fear_timer < diff)
            {
                if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0))
                {
                    for(int i=0; i!=10; i++)
                    {
                        if(target && target->GetTypeId() == TYPEID_PLAYER)
                            break;
                        target=SelectUnit(SELECT_TARGET_RANDOM, 0);
                    }
                    this->DoCast(target, 6215);
                }
                fear_timer=15000;
            }else fear_timer-=diff;
            if(!debug)
            {
                victim_list=DoFindFriendlyMissingBuff(100.0f, 45112);
                for(std::list<Creature*>::iterator itr = victim_list.begin(); itr != victim_list.end(); ++itr)
                {
                    if((*itr)->GetEntry()==100006)
                    {
                        victim=(*itr);
                        break;
                    }
                }
                DoResetThreat();
                me->AddThreat(victim, 100000.0f);
                AttackStart(victim);
//                victim->Attack(me, true);
                debug=true;
            }
            if(me->getVictim() && me->getVictim()!=victim && !me->HasAuraType(SPELL_AURA_MOD_TAUNT))
            {
                DoResetThreat();
                AttackStart(victim);
                me->AddThreat(victim, 100000.0f);
//                victim->Attack(me, true);
            }
            if(vound_timer < diff)
            {
                if(me->getVictim() && (me->getVictim()->GetHealth()*100 / me->getVictim()->GetMaxHealth())<=80.0f)
                {
                    vound_timer=20000;
                    this->DoCast(me->getVictim(), 59263);
                }
            }else vound_timer-=diff;
            if(tailswipe_timer < diff)
            {
                tailswipe_timer = 20000;
                DoCast(me->getVictim(), 59283);
            }else tailswipe_timer-=diff;
            DoMeleeAttackIfReady();
            if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 50)
                EnterEvadeMode();
        }
        void RespawnArround()
        {
            if(victim_list.size())
            {
                for(std::list<Creature*>::iterator itr = victim_list.begin(); itr != victim_list.end(); ++itr)
                {
                    if((*itr)->GetEntry()==100006)
                    {
                        (*itr)->Respawn();
                        (*itr)->GetMotionMaster()->MoveTargetedHome();
                    }
                }
            }
            victim_list = DoFindFriendlyMissingBuff(100.0f, 45112);
            if(victim_list.size())
            {
                for(std::list<Creature*>::iterator itr = victim_list.begin(); itr != victim_list.end(); ++itr)
                {
                    if((*itr)->GetEntry()==100006)
                    {
                        (*itr)->Respawn();
                        (*itr)->GetMotionMaster()->MoveTargetedHome();
                    }
                }
            }
        }
    };
};
void AddSC_boss_godzilla()
{
    new boss_godzilla;
}
