#include "ScriptPCH.h"
#include "Unit.h"

#define SAY_AGGRO -2000004
class  boss_hypnotoad : public CreatureScript
{
public: 
    boss_hypnotoad(): CreatureScript("boss_hypnotoad"){};
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_hypnotoadAI (pCreature);
    }
    struct  boss_hypnotoadAI : public ScriptedAI
    {
        boss_hypnotoadAI(Creature *c) : ScriptedAI(c) 
        {
            pInstance = c->GetInstanceScript();
            EnterEvadeMode();
        }
        InstanceScript* pInstance;
        bool enraged;
        uint32 Jump_timer;
        uint32 Enrage_timer;
        uint32 Jumping_dummy_timer;
        bool deadmen_count;
        int Jumping_faze; //0-not jumping, 1- jumping up, 2 -jumping down
        Unit* target;
        Unit* deadman;
        float jt_x;
        float jt_y;
        float jt_z;
        float jdummy_x;
        float jdummy_y;
        float jdummy_z;
        int32 damage;
        uint32 deadman_timer;
        std::list<Creature*> deadmen;
        void Reset()
        {
            //open door
            for (uint8 i=0; i!=4; i++)
                if(GameObject* semarglDoor = pInstance->instance->GetGameObject( pInstance->GetData64(i)))
                    semarglDoor->SetGoState(GO_STATE_ACTIVE);
            RespawnArround();
            deadmen_count=false;
            jt_x=0.0f;
            jt_y=0.0f;
            jt_z=0.0f;
            jdummy_x=0.0f;
            jdummy_y=0.0f;
            jdummy_z=0.0f;
            Jump_timer=15000;
            Jumping_dummy_timer=0;
            Jumping_faze=0;
            deadman_timer=45000;
            Enrage_timer=210000;
            enraged=false;
            me->SetHealth(me->GetMaxHealth());
        }
        void EnterCombat(Unit *who) //closing door
        {
            Player *who_;
            if(who->GetTypeId()!=TYPEID_PLAYER)
                who_=((Player*)((Pet*) who)->GetOwner());
            else who_=((Player*)who);
            //close door
            GameObject* HypnotoadDoor = pInstance->instance->GetGameObject( pInstance->GetData64(0));
            if(HypnotoadDoor)
                HypnotoadDoor->SetGoState(GO_STATE_READY);
            HypnotoadDoor = pInstance->instance->GetGameObject( pInstance->GetData64(1));
            if(HypnotoadDoor)
                HypnotoadDoor->SetGoState(GO_STATE_READY);
            deadmen=DoFindFriendlyMissingBuff(100.0f, 45112);

            me->MonsterYell("Quack-quack",0, who->GetGUID());
        }
        void UpdateAI(const uint32 diff)
        {
            if (!Jumping_faze && !UpdateVictim())
                return;

            switch(Jumping_faze)
            {
                case 0: //start jumping
                {
                    if(Jump_timer < diff)
                    {
                        me->MonsterTextEmote("Hypnotoad attempt for jump", me->GetGUID(), true);
                        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, true);
                        me->ApplySpellImmune(1, IMMUNITY_EFFECT,SPELL_EFFECT_ATTACK_ME, true);
                        me->SetUnitMovementFlags(MOVEMENTFLAG_LEVITATING);
                        target=this->SelectUnit(SELECT_TARGET_RANDOM, 0);
                        if(!target)    target=me->getVictim();
                        while(target->GetTypeId() != TYPEID_PLAYER)
                            target=this->SelectUnit(SELECT_TARGET_RANDOM, 0);
                        jt_x=target->GetPositionX();
                        jt_y=target->GetPositionY();
                        jt_z=target->GetPositionZ();
                        jdummy_x=(me->GetPositionX()+jt_x)/2;
                        jdummy_y=(me->GetPositionY()+jt_y)/2;
                        jdummy_z=me->GetPositionZ()+20.0f;
                        me->GetMotionMaster()->Clear(false);
                        me->StopMoving();
                        me->SendMonsterMoveWithSpeed(jdummy_x, jdummy_y, jdummy_z);
                        Jumping_faze++;
                        Jumping_dummy_timer=1500;
                        break;
                    }else{
                        Jump_timer -= diff;
                        break;}
                }
                case 1: //center now
                {
                    if(Jumping_dummy_timer<diff)
                    {
                        me->Relocate(jdummy_x, jdummy_y, jdummy_z, 0.0f);
                        me->SendMonsterMoveWithSpeed(jt_x, jt_y,jt_z);
                        Jumping_dummy_timer=1500;
                        Jumping_faze++;
                        break;
                    }else{
                        Jumping_dummy_timer-=diff;
                        break;}
                }
                case 2: //falld down and casting aoe
                {
                    if(Jumping_dummy_timer<diff && me->GetMotionMaster()->GetCurrentMovementGeneratorType() == IDLE_MOTION_TYPE)
                    {
                        me->Relocate(jt_x, jt_y, jt_z, 0.0f);
                        me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_TAUNT, false);
                        me->ApplySpellImmune(1, IMMUNITY_EFFECT,SPELL_EFFECT_ATTACK_ME, false);
                        Jumping_dummy_timer=0;
                        damage=50000;
                        me->CastCustomSpell(target, 54959, 0 , &damage, 0, false);
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_LEVITATING);
                        Jump_timer=15000;
                        Jumping_faze=0;
                        me->GetMotionMaster()->Clear();
                        /*if(SelectUnit(SELECT_TARGET_TOPAGGRO, 0))
                            me->AttackerStateUpdate(SelectUnit(SELECT_TARGET_TOPAGGRO, 0));*/
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                        break;
                    }else{
                        Jumping_dummy_timer-=diff;
                        break;}
                }
                default:break;
            }
            if(deadman_timer<=diff && !enraged)//deadmen
            {
                //deadmen=DoFindFriendlyMissingBuff(100.0f, 45112);
                int count=3;
                if(deadmen_count)
                    count=5;
                for(std::list<Creature*>::iterator itr = deadmen.begin(); itr != deadmen.end(); ++itr)
                {
                    /*me->Say((*itr)->GetName(), 0, me->GetGUID());*/
                    if((*itr)->GetEntry()==100003 && (*itr)->isAlive())
                    {
                        deadman=(*itr);
                        deadman->setFaction(me->getFaction());
                        deadman->AddThreat(me->getVictim(), 1.0f);
                        deadman->GetAI()->AttackStart(me->getVictim());
                        count-=1;
                    }
                    if(!count)
                    {
                        ((Creature *)deadman)->MonsterYell("ALL GLORY TO THE HYPNOTAD!",0, me->GetGUID());
                        break;
                    }
                }
                deadmen_count=!deadmen_count;
                deadman_timer=45000;
            }else deadman_timer-=diff;
            if(Enrage_timer<=diff && !enraged)//enraged after 5.5 mins
            {
                DoCast(me,26662);
                enraged=true;
            }else Enrage_timer-=diff;
            if(!Jumping_faze)
            {
                DoMeleeAttackIfReady();
            }
            if (me->GetDistance2d(me->GetHomePosition().GetPositionX(), me->GetHomePosition().GetPositionY()) > 50)
                EnterEvadeMode();
        }
        void JustDied(Unit* Killer)
        {
            //open door
            for (uint8 i=0; i!=4; i++)
                if(GameObject* semarglDoor = pInstance->instance->GetGameObject( pInstance->GetData64(i)))
                    semarglDoor->SetGoState(GO_STATE_ACTIVE);
        }
        void RespawnArround()
        {
            if(deadmen.size())
            {
                for(std::list<Creature*>::iterator itr = deadmen.begin(); itr != deadmen.end(); ++itr)
                    (*itr)->Respawn();
            }
        }
    };
};
void AddSC_boss_hypnotoad()
{
    new boss_hypnotoad;
}
