#include "ScriptPCH.h"
#define SAY_AGGRO -2000004
class boss_semargl: public CreatureScript
{
public:
    boss_semargl(): CreatureScript("boss_semargl"){};
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_semarglAI (pCreature);
    }
    struct  boss_semarglAI : public ScriptedAI
    {
        boss_semarglAI(Creature *c) : ScriptedAI(c) 
        {
            pInstance = c->GetInstanceScript();
            Reset();
        }
        InstanceScript* pInstance;
        uint32 shock_timer;
        uint32 fireball_timer;
        uint32 aoe_timer;
        uint8 faze_num; 
        int32 damage;
        uint8 cur_caster;
        uint8 _caster;
        CreatureAI* GetAI_boss_semargl(Creature *_Creature)
        {
            return new boss_semarglAI (_Creature);
        }
        void Reset()
        {
            //open door
            for (uint8 i=0; i!=4; i++)
                if(GameObject* semarglDoor = pInstance->instance->GetGameObject( pInstance->GetData64(i)))
                    semarglDoor->SetGoState(GO_STATE_ACTIVE);
            shock_timer=5000;
            aoe_timer=8000;
            fireball_timer=12000;
            faze_num=1;
            _caster=0;
            cur_caster=0;
        }
        void EnterCombat(Unit *who) //closing door
        {
            //close door
            for (uint8 i=0; i!=4; i++)
                if(GameObject* semarglDoor = pInstance->instance->GetGameObject( pInstance->GetData64(i)))
                    semarglDoor->SetGoState(GO_STATE_READY);

             me->MonsterYell("Your flesh will burn!",0, who->GetGUID());
        }
        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;

            if(faze_num!=3 && me->GetHealth()*100.0f/me->GetMaxHealth()<(100.0f-faze_num*33.3f))
            {
                faze_num++;
                me->MonsterYell("Feel the power of immolation!",0, me->getVictim()->GetGUID());
            }

            if(aoe_timer < diff)
            {
                _caster=cur_caster;
                uint32 entry[4]={400002, 400003, 400004, 400005};
                damage=30000;

                for (uint8 i=0; i!=faze_num; i++)
                {
                    std::list<Creature*> victim_list=DoFindFriendlyMissingBuff(100.0f, 45112);
                    for(std::list<Creature*>::iterator itr = victim_list.begin(); itr != victim_list.end(); ++itr)
                    {
                        if((*itr)->GetEntry()==entry[_caster])
                            (*itr)->CastCustomSpell((*itr), 30941, &damage, 0, 0, true);
                    }
                    _caster++;
                    if(_caster>3)
                        _caster-=4;
                }
                cur_caster++;
                if(cur_caster>3)
                    cur_caster-=4;
                aoe_timer=8000;
            } aoe_timer-=diff;

            if(shock_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                for(int i=0; i!=10; i++)
                {
                    if(target->GetTypeId() == TYPEID_PLAYER)
                        break;
                    target=SelectUnit(SELECT_TARGET_RANDOM, 0);
                }
                DoCast(target, 43303);
                shock_timer=5000;
            }else shock_timer-=diff;

            if(fireball_timer < diff)
            {
                Unit* target = SelectUnit(SELECT_TARGET_RANDOM, 0);
                for(int i=0; i!=10; i++)
                {
                    if(target->GetTypeId() == TYPEID_PLAYER)
                        break;
                    target=SelectUnit(SELECT_TARGET_RANDOM, 0);
                }
                damage=10000;
                me->CastCustomSpell(target, 41484, &damage, 0, 0, false);
                fireball_timer=5000;
            }else fireball_timer-=diff;

            DoMeleeAttackIfReady();
            if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 23)
                EnterEvadeMode();
        }
        void JustDied(Unit* Killer)
        {
            me->MonsterYell("I'll reborn like burning phoenix!",0, Killer->GetGUID());
            //open door
            for (uint8 i=0; i!=4; i++)
                if(GameObject* semarglDoor = pInstance->instance->GetGameObject( pInstance->GetData64(i)))
                    semarglDoor->SetGoState(GO_STATE_ACTIVE);
        }
    };
};
void AddSC_boss_semargl()
{
    new boss_semargl;
}
