#include "ScriptPCH.h"
#define SPELL_FROSTBOLT 58535
class  boss_nazaryans_blood : public CreatureScript
{
public: 
    boss_nazaryans_blood() : CreatureScript("boss_nazaryans_blood"){}
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_nazaryans_bloodAI (pCreature);
    }
    struct boss_nazaryans_bloodAI : public ScriptedAI
    {
        boss_nazaryans_bloodAI(Creature *c) : ScriptedAI(c) {Reset();}
        uint32 FB_Timer;
        uint32 aggro_timer;
        void Reset()
        {
            FB_Timer=0;
            aggro_timer=1000;
        }
        void Aggro(Unit *who)
        {
            me->AttackerStateUpdate(who);
        }
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (!me->getVictim())
            {
                me->DealDamage(me, 200000, 0);
                return;
            }
            if(aggro_timer<diff)
            {
                me->AttackerStateUpdate(me->getVictim());
                aggro_timer=1000;
            }
            else
                aggro_timer-=diff;
            if(me->GetDistance(me->getVictim())<40.0f)
            {
                if (FB_Timer < diff)
                {
                    this->DoCast(me->getVictim(),SPELL_FROSTBOLT, false);
                    FB_Timer = 3000;
                }else    FB_Timer -= diff;
            }
        }
    };
};

void AddSC_boss_nazaryans_blood()
{
    new boss_nazaryans_blood;
}
