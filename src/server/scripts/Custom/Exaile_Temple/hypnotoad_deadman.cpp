#include "ScriptPCH.h"
class  boss_hypnotoad_deadman : public CreatureScript
{
    public: 
    boss_hypnotoad_deadman() : CreatureScript("boss_hypnotoad_deadman"){};
    CreatureAI* GetAI(Creature *_Creature) const
    {
        return new boss_hypnotoad_deadmanAI (_Creature);
    }
    struct  boss_hypnotoad_deadmanAI : public ScriptedAI
    {
        boss_hypnotoad_deadmanAI(Creature *c) : ScriptedAI(c) {Reset();}
        void Reset()
        {
            me->setFaction(35);//friendly
            me->RemoveAurasDueToSpell(47496);
            me->GetMotionMaster()->MoveTargetedHome();
        }
        void EnterCombat(Unit *who)
        {
            DoCast(me, 45112); // visual mc buff
        }
        void UpdateAI(const uint32 diff)
        {
            if(!UpdateVictim())
                return;
            SpellEntry const* spellInfo=GetSpellStore()->LookupEntry(47496);
            if(me->getVictim()->IsImmunedToSpellEffect(spellInfo, 0) ||(me->getVictim()->GetTypeId() != TYPEID_PLAYER &&((Creature *)me->getVictim())->isTotem()))
            {
                me->AddThreat(me->getVictim(), -1000.0f);
                me->TauntFadeOut(me->getVictim());
            }
                
            if(me->GetDistance2d(me->getVictim())<=2.0f)
            {
                int32 bp = me->GetHealth()/8;
                me->CastCustomSpell(me->getVictim(), 47496, &bp, 0, 0, true);
            }
        }
    };
};
void AddSC_boss_hypnotoad_deadman()
{
    new boss_hypnotoad_deadman;
}
