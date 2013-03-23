/* https://bitbucket.org/cemak/amegawow/
© Cemak 2011*/

#include "ScriptPCH.h"

enum LikantropoSpells
{
    ENRAGE = 48138,
    FRENZY = 48142,
    MORTAL_WOUND = 59265,
    WORGEN_BITE = 55266,
    WORGEN_CALL = 53095,
    DECREPIT_FEVER = 54098,
    SUMMON_WORGEN = 27020
};

// Likantropo yells
#define SAY_LIKANTROPO_AGGRO "Попробуйте Уничтожить меня!"
#define SAY_LIKANTROPO_SLAY "А вот и первая жертва!"
#define SAY_LIKANTROPO_DEATH "Этого просто не может быть...."
#define SAY_LIKANTROPO_MORPH "мухаха"

enum LavaBoostSpells
{
    HEALING_WAVE = 51586,
    LIGHTNING_BOLT = 59024,
    BLOODLUST = 41185,
    LIGHTNING_SHIELD = 59025,
    SUMMON_AIR_ELEMENTAL = 30418,
    LAVA_BURST = 59519,
    CHAIN_LIGHTNING = 54531,
    FLAME_SHOCK = 43303,
    EARTH_SHOCK = 43305,
    LIGHTNING_RING = 59848
};

// Lava Boost yells
#define SAY_LAVA_AGGRO "Вы сможете меня остановить ?!"
#define SAY_LAVA_SLAY "Умри смертный!"
#define SAY_LAVA_DEATH "Жалкие людишки! Мы еще с вами встретимся!"
#define SAY_LAVA_BLOOD "Жажда крови наполняет мои силы!"

enum QuelNagasSpells
{
    SPELL_MAGIC_BARRIER = 38112,
    SPELL_TOXIC_SPORES = 38575,
    TOXIC_SPORES_TRIGGER = 22207,
    SUMMONED_MURLOC = 27649,
    SPELL_POISON_BOLT_VOLLEY = 54098,
    SPELL_ENTANGLE = 38316,
    SPELL_WRATH = 20698
};

// Quel'Nagas yells
#define SAY_NAGAS_AGGRO "Кто побеспокоил мои покои?!"
#define SAY_NAGAS_SLAY "И это все, на что ты способен ?"
#define SAY_NAGAS_DEATH "Да как это смогло произойти?!"
#define SAY_NAGAS_SUMMON "Мурлоки в атаку!"

enum SylvanasSpells
{
    SPELL_MULTISHOT = 40872,
    SPELL_VOLLEY = 38633,
    SUMMONED_SNAKE = 29444,
    SPELL_BOLT_VOLLEY = 34780,
    SPELL_BEWITCHING_AURA = 29486,
    SPELL_REJUVENATION = 71142,
    SPELL_ICE_TRAP = 71249,
    SPELL_AURA_ROT = 25818
};

// Might of Sylvanas yells
#define SAY_SYLVANAS_AGGRO "Сможете ли вы одолеть меня? жалкие людишки!"
#define SAY_SYLVANAS_SLAY "Шоре Ларан!"
#define SAY_SYLVANAS_DEATH "Ну я так не играю... =)"
#define SAY_SYLVANAS_ICE "Заморозь их! "

enum TrollzillaSpells
{
    SPELL_FRENZY = 28131,
    SPELL_CLEAVE = 33480,
    SPELL_FIRE_SHIELD = 30513,
    SPELL_WAR_STOMP = 56427,
    SUMMON_FIRE_ELEMENTAL = 30416
};

// Trollzilla yells
#define SAY_TROLLZILLA_AGGRO "Я.. уничтожу.. вас..."
#define SAY_TROLLZILLA_SLAY "Хаккар... Наказывает и жестоко наказывает!..."
#define SAY_TROLLZILLA_DEATH "Ах... aaaa...ну как же...."
#define SAY_TROLLZILLA_FRENZY "Я... медленно... покидаю свое тело!"

enum PerotharnSpells
{
    STORM_OF_GRIEF = 59772,
    SHADOW_BURST = 34436,
    PIERCING_SHADOW = 36698,
    SHADOW_FLAME = 22539,
    HARVEST_SOUL = 28679,
    SHADOW_BOLT = 41957
};

// Perotharn yells
#define SAY_PEROTHARN_AGGRO "Я буду смотреть, как вы медленно умрете!"
#define SAY_PEROTHARN_SLAY "Скоро вы будете одним из нас!"
#define SAY_PEROTHARN_DEATH "Мастер ... Прости меня ..."
#define SAY_PEROTHARN_SOUL "Мастер, сюда приходят ваши души!"

enum UznamSpells
{
    EARTHQUAKE = 46240,
    WHIRLWIND = 52027,
    SHOCK_OF_SORROW = 50760,
    ENTROPIC_AURA = 36784,
    AURAL_SHOCK = 14538
};

// Uznam yells
#define SAY_UZNAM_AGGRO "Кто оскверенил мое святилище?"
#define SAY_UZNAM_SLAY "Нечего было лезть в мое логово!"
#define SAY_UZNAM_DEATH "Воскресший? Я тут после крови"
#define SAY_UZNAM_SPECIAL "Я немного..."
#define EMOTE_WHIRL "Я умираю изнутри..!"

enum Spells
{
    //===================FireSpells==========================
    SPELL_SUNBEAM_FIRE                               = 62872,
    SPELL_SCORCH_FIRE                                = 63474,
    SPELL_JETS_FIRE                                  = 63472,
    SPELL_FIREBALL_FIRE                              = 68926,
    SPELL_FLAMING_CINDER_FIRE                        = 67332,
    SPELL_BURNING_BITE_FIRE                          = 67626,
    //===================IceSpells===========================
    SPELL_ICEBOLT_ICE                                = 31249,
    SPELL_ICECHAINS_ICE                              = 22744,
    SPELL_FROST_BLAST_ICE                            = 72123,
    SPELL_FROST_BLAST_1_ICE                          = 27808,
    SPELL_FROST_SHIELD_ICE                           = 62650,
    SPELL_ICEBLAST_ICE                               = 28522,
    //===================EarthSpells=========================
    SPELL_NATURE_EARTH                               = 62519, 
    SPELL_GROUND_EARTH                               = 62859,
    SPELL_LIFEBLOOM_EARTH                            = 67959, 
    SPELL_WRATH_EARTH                                = 67953,
    SPELL_STUN_EARTH                                 = 52402,
    SPELL_POISON_NOVA_EARTH                          = 68989,
    SPELL_PETRIFYING_BREATH_EARTH                    = 62030,
    //===================DarkSpells==========================
    SPELL_SOULS_SMALL_DARK                           = 72521,
    SPELL_SOULS_LARGE_DARK                           = 72523,
    SPELL_SOULS_SUMMON_DARK                          = 68967,
    SPELL_MIRRORED_DARK                              = 69051,
    SPELL_DEATH_AND_DECAY_DARK                       = 72110,
    SPELL_DARK_VOLLEY_DARK                           = 63038,
    //___________________NPCSpells___________________________
    //===================FireElemental=======================
    SPELL_CURSE_OF_FLAMES_ELEMENTAL                  = 38010,
    SPELL_FLAME_BREATH_ELEMENTAL                     = 64021,
    //===================IceMage=============================
    SPELL_BLIZZARD                                   = 71118,
    SPELL_CONE_OF_COLD                               = 64655,
    SPELL_COUNTERSPELL                               = 59111,
    SPELL_FROST_ARMOR                                = 31256, //вначале боя
    SPELL_ICE_BARRIER                                = 43039, //вначале боя
    //===================FireMage============================
    SPELL_HEAL                                       = 71120,
    SPELL_FEL_FIREBALL                               = 66532,
    SPELL_FIRE_NOVA                                  = 68958,
    SPELL_MOLTEN_ARMOR                               = 43046, //вначале боя
    SPELL_MANA_SHIELD                                = 35064, //вначале боя
    //===================DeathKnight=========================
    SPELL_FROST_STRIKE                               = 67937,
    SPELL_DEATH_COIL                                 = 67929,
    SPELL_GLACIAL_STRIKE                             = 71317,
    SPELL_ANTIMAGIC_ZONE                             = 51052,
    //===================Warrior=============================
    SPELL_SHOCKWAVE                                  = 75418,
    SPELL_BLOODTHRIST                                = 33964,
    SPELL_DISARM                                     = 15752,
    SPELL_BATTLE_SHOUT                               = 47436, //вначале боя
    SPELL_CHARGE                                     = 38461, //вначале боя
	//===================Hunter=============================
    SPELL_AIMEDSHOT                                  = 59243,
    SPELL_SERPENTSTRING                              = 49001,
    SPELL_MULTISHOT_1                                 = 59244,
    SPELL_ASPECT_DRAGONHAWK                          = 61847, //вначале боя
    //===================Priest=============================
    SPELL_MASSHEAL                                   = 56161,
    SPELL_FLESH_HEAL                                 = 68023,
    SPELL_INER_FIRE                                  = 48168, //вначале боя
    SPELL_SHIELD                                     = 66099, //вначале боя
    SPELL_FORTITUDE                                  = 48161, //вначале боя
    SPELL_RENEW                                      = 68036, //вначале боя
    SPELL_VAMPIRIC_TOUCH                             = 68091, 
    SPELL_MIND_FLY                                   = 54339,
    SPELL_FEAR                                       = 10890,
    //===================Common==============================
    SPELL_FRENZY_1                                   = 47774,  
    SPELL_FURY                                       = 66721,  
};

enum eEnums
{
    NPC_FIRE_ELEMENTAL                               = 99007,
};

#define FIRE_SAY_AGGRO                                "Вам нечего здесь делать. Убирайтесь, пока живы!"
#define FIRE_SAY_FRENZY                               "Ярость наполняет меня!"
#define FIRE_SAY_SUMMON_TITAN                         "Слуги мои, помогите!"
#define FIRE_SAY_KILL                                 "Этим всё и закончится!"
#define FIRE_SAY_DIE                                  "На этот раз... вам повезло..."

class boss_firelord_instance : public CreatureScript
{
    public:

        boss_firelord_instance()
            : CreatureScript("boss_firelord_instance")
        {
        }

        struct boss_firelord_instanceAI : public ScriptedAI
        {
            boss_firelord_instanceAI(Creature *c) : ScriptedAI(c),  summons(c) {}
			
            SummonList summons;

            uint32 m_uiSunbeamTimer;
            uint32 m_uiScorchTimer;
            uint32 m_uiJetsTimer;
            uint32 m_uiFireballTimer;
            uint32 m_uiFrenzyTimer;
            uint32 m_uiSummonTimer;
            uint32 m_uiFlamingCinderTimer;
            uint32 m_uiPhase;
            uint32 m_uiSummonCheck;
            uint32 m_uiFuryTimer;
            uint32 m_uiBurningBiteTimer;

            void Reset()
            {
                m_uiSunbeamTimer       =  urand(10000, 30000);
                m_uiScorchTimer        =  urand(10000, 35000);
                m_uiJetsTimer          =  urand(10000, 35000);
                m_uiFireballTimer      =  urand(10000, 28000);
                m_uiFrenzyTimer        =  70000;              
                m_uiSummonTimer        =  60000;
                m_uiFlamingCinderTimer =  urand(10000, 15000);
                m_uiPhase              =  1;
                m_uiSummonCheck        =  0;
                m_uiFuryTimer          =  urand(20000, 30000);
                m_uiBurningBiteTimer   =  urand(12000, 25000);
                me->SetReactState(REACT_DEFENSIVE);
            }

            void JustSummoned(Creature *summon)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                {
                    summon->CastSpell(pTarget, SPELL_CURSE_OF_FLAMES_ELEMENTAL, true);
                    summon->CastSpell(pTarget, SPELL_FLAME_BREATH_ELEMENTAL, true);
                }

                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    summon->AI()->AttackStart(pTarget);
                summons.Summon(summon);
            }

            void EnterCombat(Unit* pWho)
            {
                me->MonsterYell(FIRE_SAY_AGGRO, LANG_UNIVERSAL, 0);
                summons.DespawnAll();
            }

            void EnterEvadeMode()
            {
                ScriptedAI::EnterEvadeMode();
            }
			
            void KilledUnit(Unit *victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    me->MonsterYell(FIRE_SAY_KILL, LANG_UNIVERSAL, 0);
            }
			
            void JustDied(Unit* /*killer*/)
            {
                me->MonsterYell(FIRE_SAY_DIE, LANG_UNIVERSAL, 0);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if (!me->getVictim())
                {
                    me->CombatStop(true);
                    EnterEvadeMode();
                }

                if (m_uiPhase > 1)
                {
                    //Jets Timer
                    if (m_uiJetsTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_JETS_FIRE);

                        m_uiJetsTimer = urand(10000, 30000);
                    }
                    else
                        m_uiJetsTimer -= uiDiff;

                    //Sunbeam Timer
                    if (m_uiSunbeamTimer <= uiDiff)
                    {
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                            DoCast(pTarget, SPELL_SUNBEAM_FIRE);

                        m_uiSunbeamTimer = urand(10000, 20000);
                    }
                    else
                        m_uiSunbeamTimer -= uiDiff;

                    //Scorch Timer
                    if (m_uiScorchTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_SCORCH_FIRE);
                        m_uiScorchTimer = urand(10000, 30000);
                    }
                    else
                        m_uiScorchTimer -= uiDiff;

                    //Fireball Timer
                    if (m_uiFireballTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_FIREBALL_FIRE);
                        m_uiFireballTimer = urand(10000, 25000);
                    }
                    else
                        m_uiFireballTimer -= uiDiff;

                    if (m_uiFrenzyTimer <= uiDiff)
                    {
                        me->MonsterSay(FIRE_SAY_FRENZY, LANG_UNIVERSAL, 0);
                        DoCast(me, SPELL_FRENZY_1);
                        m_uiFrenzyTimer = urand(20000, 40000);
                    }
                    else
                        m_uiFrenzyTimer -= uiDiff;

                    //Flaming Cinder Timer
                    if (m_uiFlamingCinderTimer <= uiDiff)
                    {
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(pTarget, SPELL_FLAMING_CINDER_FIRE, true);

                        m_uiFlamingCinderTimer = urand(10000, 15000);
                    }
                    else
                        m_uiFlamingCinderTimer -= uiDiff;

                }
                else if (m_uiPhase == 1)                            //Phase timer
                {
                    //Frenzy Timer
                    if (m_uiFrenzyTimer <= uiDiff)
                    {
                        ++m_uiPhase;
                        me->MonsterSay(FIRE_SAY_FRENZY, LANG_UNIVERSAL, 0);
                        DoCast(me, SPELL_FRENZY);
                        m_uiFrenzyTimer = urand(30000, 50000);
                    }
                    else
                        m_uiFrenzyTimer -= uiDiff;

                    //Sunbeam Timer
                    if (m_uiSunbeamTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_SUNBEAM_FIRE);

                        m_uiSunbeamTimer = urand(10000, 30000);
                    }
                    else
                        m_uiSunbeamTimer -= uiDiff;

                    //Scorch Timer
                    if (m_uiScorchTimer <= uiDiff)
                    {
                        DoCast(me->getVictim(), SPELL_SCORCH_FIRE);
                        m_uiScorchTimer = urand(10000, 35000);
                    }
                    else
                        m_uiScorchTimer -= uiDiff;
                }
				
                //Summon Timer
                if (m_uiSummonTimer <= uiDiff)
                {
                    me->MonsterYell(FIRE_SAY_SUMMON_TITAN, LANG_TITAN, 0);
                    me->SummonCreature(NPC_FIRE_ELEMENTAL, me->GetPositionX()+2, me->GetPositionY()+2, me->GetPositionZ()+1, 0, TEMPSUMMON_TIMED_DESPAWN, 50000);
                    m_uiSummonTimer = 60000;
                    m_uiSummonCheck = 50000;
                }
                else
                    m_uiSummonTimer -= uiDiff;
					
                //Summon Heal
                if (m_uiSummonCheck <= uiDiff)
                {
                    if (Creature *pElemental = GetClosestCreatureWithEntry(me, NPC_FIRE_ELEMENTAL, INTERACTION_DISTANCE*100))
                    {
                        for (uint8 i = 0; i < 4; ++i)
                            me->CastSpell(me, 71783, true);
                        me->CastSpell(me, SPELL_FURY, true);
                    }

                    m_uiSummonCheck = 1000000;    //Hack
                }
                else
                    m_uiSummonCheck -= uiDiff;

                //Burning Bite Timer
                if (m_uiBurningBiteTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_BURNING_BITE_FIRE, true);

                    m_uiBurningBiteTimer = urand(12000, 25000);
                }
                else
                    m_uiBurningBiteTimer -= uiDiff;

                //Fury Timer
                if (m_uiFuryTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_FURY, true);

                    m_uiFuryTimer = urand(25000, 40000);
                }
                else
                    m_uiFuryTimer -= uiDiff;


                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_firelord_instanceAI(creature);
        }

};

#define ICE_SAY_AGGRO                                "Я проморожу вас насквозь!"
#define ICE_SAY_KILL                                 "Ещё одна бесполезная ледышка!"
#define ICE_SAY_DIE                                  "Но! Я же был бессмертен..."

#define EMOTE_ICE_SHIELD_ICE                         "Кожа Стража льда Алкида покрывается коркой льда"

class boss_icelord_instance : public CreatureScript
{
    public:

        boss_icelord_instance()
            : CreatureScript("boss_icelord_instance")
        {
        }

        struct boss_icelord_instanceAI : public ScriptedAI
        {
            boss_icelord_instanceAI(Creature *c) : ScriptedAI(c) {}


            uint32 m_uiIceBoltTimer;
            uint32 m_uiIceChainsTimer;
            uint32 m_uiFrostBlastTimer;
            uint32 m_uiFrostShieldDuration;
            uint32 m_uiFrostShieldCooldown;
            uint32 m_uiFrostBlast;
            uint32 m_uiIceBlastTimer;

            void Reset()
            {
                m_uiIceBoltTimer          = urand(5000, 20000);
                m_uiIceChainsTimer        = urand(10000,25000);
                m_uiFrostBlastTimer       = urand(10000,20000);
                m_uiFrostShieldCooldown   = urand(20000,22000);
                m_uiFrostBlast            = urand(40000,80000);
                m_uiIceBlastTimer         = urand(20000,40000);
                me->SetReactState(REACT_DEFENSIVE);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
                me->MonsterYell(ICE_SAY_AGGRO, LANG_UNIVERSAL, 0);
            }

            void EnterEvadeMode()
            {
                ScriptedAI::EnterEvadeMode();
            }
			
            void KilledUnit(Unit *victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    me->MonsterYell(ICE_SAY_KILL, LANG_UNIVERSAL, 0);
            }
			
            void JustDied(Unit* /*killer*/)
            {
                me->MonsterYell(ICE_SAY_DIE, LANG_UNIVERSAL, 0);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if (!me->getVictim())
                {
                    me->CombatStop(true);
                    EnterEvadeMode();
                }

                //Icebolt Timer
                if (m_uiIceBoltTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_ICEBOLT_ICE, true);

                    m_uiIceBoltTimer = urand(5000, 15000);
                }
                else
                    m_uiIceBoltTimer -= uiDiff;

                //IceBlast Timer
                if (m_uiIceBlastTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_ICEBLAST_ICE, true);

                    m_uiIceBlastTimer = urand(20000,40000);
                }
                else
                    m_uiIceBlastTimer -= uiDiff;

                //Ice Chains Timer
                if (m_uiIceChainsTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_ICECHAINS_ICE);

                    m_uiIceChainsTimer = urand(10000, 17000);
                }
                else
                    m_uiIceChainsTimer -= uiDiff;

                //Frost Blast (AOE) Timer
                if (m_uiFrostBlast <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_FROST_BLAST_1_ICE);

                    m_uiFrostBlast = urand(40000,80000);
                }
                else
                    m_uiFrostBlast -= uiDiff;

                //Frost Blast Timer
                if (m_uiFrostBlastTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_FROST_BLAST_ICE);

                    m_uiFrostBlastTimer = urand(10000,20000);
                }
                else
                    m_uiFrostBlastTimer -= uiDiff;

                //Ice Shield duration
                if (m_uiFrostShieldDuration <= uiDiff && me->HasAura(SPELL_FROST_SHIELD_ICE))
                {
                    me->RemoveAurasDueToSpell(SPELL_FROST_SHIELD_ICE);
                }
                else
                    m_uiFrostShieldDuration -= uiDiff;

                //Ice Shield cooldown
                if (m_uiFrostShieldCooldown <= uiDiff)
                {
                    DoCast(me, SPELL_FROST_SHIELD_ICE);
                    me->MonsterTextEmote(EMOTE_ICE_SHIELD_ICE, 0);
                    m_uiFrostShieldCooldown = urand(20000, 22000);
                    m_uiFrostShieldDuration = urand(10000, 15000);
                }
                else
                    m_uiFrostShieldCooldown -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_icelord_instanceAI(creature);
        }

};

#define EARTH_SAY_AGGRO                               "Вам не следовало сюда приходить! Это место станет вашей могилой!"
#define EARTH_EMOTE_NATURE                            "Стража замли Акрилия наполняют силы земли"
#define EARTH_SAY_KILL                                "Отправляйся в землю!"
#define EARTH_SAY_DIE                                 "Мать земля, помо... ох..."

class boss_earthlord_instance : public CreatureScript
{
    public:

        boss_earthlord_instance()
            : CreatureScript("boss_earthlord_instance")
        {
        }

        struct boss_earthlord_instanceAI : public ScriptedAI
        {
            boss_earthlord_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiNatureTimer;
            uint32 m_uiGroundTimer;
            uint32 m_uiLifebloomTimer;
            uint32 m_uiWrathTimer;
            uint32 m_uiStunTimer;
            uint32 m_uiPoisonNovaTimer;
            uint32 m_uiFuryTimer;
            uint32 m_uiPetrifyingBreathTimer;

            void Reset()
            {
                m_uiNatureTimer           = urand(20000, 30000);
                m_uiGroundTimer           = urand(10000, 30000);
                m_uiLifebloomTimer        = urand(10000, 30000);
                m_uiWrathTimer            = urand(5000,  10000);
                m_uiStunTimer             = urand(10000, 20000);
                m_uiPoisonNovaTimer       = urand(5000,  20000);
                m_uiFuryTimer             = urand(25000, 40000);
                m_uiPetrifyingBreathTimer = urand(10000, 20000);
                me->SetReactState(REACT_DEFENSIVE);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
                me->MonsterYell(EARTH_SAY_AGGRO, LANG_UNIVERSAL, 0);
                me->RemoveAurasDueToSpell(SPELL_FURY);
            }

            void EnterEvadeMode()
            {
                ScriptedAI::EnterEvadeMode();
            }
			
            void KilledUnit(Unit *victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    me->MonsterYell(EARTH_SAY_KILL, LANG_UNIVERSAL, 0);
            }
			
            void JustDied(Unit* /*killer*/)
            {
                me->MonsterYell(EARTH_SAY_DIE, LANG_UNIVERSAL, 0);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if (!me->getVictim())
                {
                    me->CombatStop(true);
                    EnterEvadeMode();
                }
				
                //Attuned to Nature Timer
                if (m_uiNatureTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_NATURE_EARTH, true);
                    me->MonsterTextEmote(EARTH_EMOTE_NATURE, 0);

                    m_uiNatureTimer = urand(20000, 30000);
                }
                else
                    m_uiNatureTimer -= uiDiff;
					
                //Fury Timer
                if (m_uiFuryTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_FURY, true);

                    m_uiFuryTimer = urand(25000, 40000);
                }
                else
                    m_uiFuryTimer -= uiDiff;

                //Ground Tremor Timer
                if (m_uiGroundTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_GROUND_EARTH, true);

                    m_uiGroundTimer = urand(15000, 30000);
                }
                else
                    m_uiGroundTimer -= uiDiff;

                //Poison Nova Timer
                if (m_uiPoisonNovaTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_POISON_NOVA_EARTH, true);

                    m_uiPoisonNovaTimer = urand(5000, 20000);
                }
                else
                    m_uiPoisonNovaTimer -= uiDiff;

                //Lifebloom Timer
                if (m_uiLifebloomTimer <= uiDiff)
                {
                    for (uint8 i = 0; i < 4; ++i)
                        me->CastSpell(me, SPELL_LIFEBLOOM_EARTH, true);

                    m_uiLifebloomTimer = urand(10000, 30000);
                }
                else
                    m_uiLifebloomTimer -= uiDiff;

                //Wrath Timer
                if (m_uiWrathTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_WRATH_EARTH, true);

                    m_uiWrathTimer = urand(5000,  10000);
                }
                else
                    m_uiWrathTimer -= uiDiff;

                //Petrifying Breath Timer
                if (m_uiPetrifyingBreathTimer <= uiDiff)
                {
                    me->CastSpell(me->getVictim(), SPELL_PETRIFYING_BREATH_EARTH, true);

                    m_uiPetrifyingBreathTimer = urand(10000,  20000);
                }
                else
                    m_uiPetrifyingBreathTimer -= uiDiff;
					
                //Stun Timer
                if (m_uiStunTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_STUN_EARTH, true);

                    m_uiStunTimer = urand(10000, 20000);
                }
                else
                    m_uiStunTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_earthlord_instanceAI(creature);
        }

};

#define DARK_SAY_AGGRO                                "Гости? Неожиданно..."
#define DARK_SAY_FRENZY                               "О да..! Я чувствую как ваша ярость наполняет меня!"
#define DARK_SAY_SUMMON                               "Посмотрите на души тех, кто был здесь до вас."
#define DARK_SAY_KILL                                 "Твоя душа пополнит мою армию!"
#define DARK_SAY_DIE                                  "Я слишком стар для таких битв..."

class boss_darklord_instance : public CreatureScript
{
    public:

        boss_darklord_instance()
            : CreatureScript("boss_darklord_instance")
        {
        }

        struct boss_darklord_instanceAI : public ScriptedAI
        {
            boss_darklord_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiRandom1Timer;
            uint32 m_uiRandom2Timer;
            uint32 m_uiRandom3Timer;
            uint32 m_uiSummonSoulsTimer;
            uint32 m_uiMirroredTimer;
            uint32 m_uiDecayTimer;
            uint32 m_uiVolleyTimer;
            uint32 m_uiFrenzy;
            uint32 m_uiFuryTimer;
            uint32 dPhase;

            void Reset()
            {
                m_uiRandom1Timer        = urand(10000,12000);
                m_uiRandom2Timer        = urand(7000, 12000);
                m_uiRandom3Timer        = urand(11000,15000);
                m_uiFrenzy              = urand(40000,55000);
                m_uiSummonSoulsTimer    = urand(12000,20000);
                m_uiMirroredTimer       = urand(20000,27000);
                m_uiDecayTimer          = urand(15000,45000);
                m_uiVolleyTimer         = urand(15000,37000);
                m_uiFuryTimer           = urand(25000,40000);
                dPhase = 1;
                me->SetReactState(REACT_DEFENSIVE);
            }

            void JustSummoned(Creature *summon)
            {
                summon->CastSpell(summon, SPELL_FRENZY, true);
                summon->CastSpell(summon, SPELL_FURY, true);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                    summon->AI()->AttackStart(pTarget);
            }

            void EnterCombat(Unit* pWho)
            {
                me->MonsterYell(DARK_SAY_AGGRO, LANG_UNIVERSAL, 0);
                me->RemoveAurasDueToSpell(SPELL_FURY);
            }

            void EnterEvadeMode()
            {
                ScriptedAI::EnterEvadeMode();
            }

            void KilledUnit(Unit *victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                    me->MonsterYell(DARK_SAY_KILL, LANG_UNIVERSAL, 0);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->MonsterYell(DARK_SAY_DIE, LANG_UNIVERSAL, 0);
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;

                if (!me->getVictim())
                {
                    me->CombatStop(true);
                    EnterEvadeMode();
                }

                //Random 1 Timer
                if (m_uiRandom1Timer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, urand(0, 1) ? SPELL_SUNBEAM_FIRE : SPELL_FROST_BLAST_ICE, true);

                    m_uiRandom1Timer = urand(10000, 30000);
                }
                else
                    m_uiRandom1Timer -= uiDiff;

                //Random 2 Timer
                if (m_uiRandom2Timer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, urand(0, 1) ? SPELL_STUN_EARTH : SPELL_GROUND_EARTH, true);

                    m_uiRandom2Timer = urand(7000, 12000);
                }
                else
                    m_uiRandom2Timer -= uiDiff;

                //Random 3 Timer
                if (m_uiRandom3Timer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, urand(0, 1) ? SPELL_SCORCH_FIRE : SPELL_ICECHAINS_ICE, true);

                    m_uiRandom3Timer = urand(11000,15000);
                }
                else
                    m_uiRandom3Timer -= uiDiff;

                //Mirrored Timer
                if (m_uiMirroredTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_MIRRORED_DARK, true);

                    m_uiMirroredTimer = urand(20000,27000);
                }
                else
                    m_uiMirroredTimer -= uiDiff;

                //Death and Decay Timer
                if (m_uiDecayTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(pTarget, SPELL_DEATH_AND_DECAY_DARK, true);

                    m_uiDecayTimer = urand(15000,45000);
                }
                else
                    m_uiDecayTimer -= uiDiff;

                //Dark Volley Timer
                if (m_uiVolleyTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_DARK_VOLLEY_DARK, true);

                    m_uiVolleyTimer = urand(15000,45000);
                }
                else
                    m_uiVolleyTimer -= uiDiff;

                //Summon Timer
                if (m_uiSummonSoulsTimer <= uiDiff)
                {
                    for (uint8 n = 0; n < dPhase*4; ++n)
                        if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(pTarget, SPELL_SOULS_SUMMON_DARK, true);

                    me->MonsterSay(DARK_SAY_SUMMON, LANG_UNIVERSAL, 0);
                    m_uiSummonSoulsTimer = urand(12000,20000);
                }
                else
                    m_uiSummonSoulsTimer -= uiDiff;

                //Fury Timer
                if (m_uiFuryTimer <= uiDiff)
                {
                    me->CastSpell(me, SPELL_FURY, true);

                    m_uiFuryTimer = urand(25000, 40000);
                }
                else
                    m_uiFuryTimer -= uiDiff;

                if (dPhase == 1)
                {
                    //Frenzy & Phase Timer
                    if (m_uiFrenzy <= uiDiff)
                    {
                        ++dPhase;
                        me->MonsterSay(DARK_SAY_FRENZY, LANG_UNIVERSAL, 0);
                        me->CastSpell(me, SPELL_FRENZY, true);
                        me->CastSpell(me, SPELL_SOULS_SMALL_DARK, true);
                        m_uiFrenzy = urand(40000,55000);
                    }
                    else
                        m_uiFrenzy -= uiDiff;

                } else if (dPhase < 5 && dPhase !=1) {
                    //Frenzy & Phase Timer
                    if (m_uiFrenzy <= uiDiff)
                    {
                        ++dPhase;
                        me->MonsterSay(DARK_SAY_FRENZY, LANG_UNIVERSAL, 0);
                        me->CastSpell(me, SPELL_FRENZY, true);
                        m_uiFrenzy = urand(40000,50000);
                    }
                    else
                        m_uiFrenzy -= uiDiff;

                } else if (dPhase == 5) {
                    //Frenzy & Phase Timer
                    if (m_uiFrenzy <= uiDiff)
                    {
                        ++dPhase;
                        me->MonsterSay(DARK_SAY_FRENZY, LANG_UNIVERSAL, 0);
                        me->CastSpell(me, SPELL_FRENZY, true);
                        me->CastSpell(me, SPELL_SOULS_LARGE_DARK, true);
                        m_uiFrenzy = urand(40000,55000);
                    }
                    else
                        m_uiFrenzy -= uiDiff;
                } else if (dPhase > 5) {
                    //Frenzy & Phase Timer
                    if (m_uiFrenzy <= uiDiff)
                    {
                        ++dPhase;
                        me->MonsterSay(DARK_SAY_FRENZY, LANG_UNIVERSAL, 0);
                        me->CastSpell(me, SPELL_FRENZY, true);
                        m_uiFrenzy = urand(35000,45000);
                    }
                    else
                        m_uiFrenzy -= uiDiff;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_darklord_instanceAI(creature);
        }

};

class npc_mage_ice_instance : public CreatureScript
{
    public:

        npc_mage_ice_instance()
            : CreatureScript("npc_mage_ice_instance")
        {
        }

        struct npc_mage_ice_instanceAI : public ScriptedAI
        {
            npc_mage_ice_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiBlizzardTimer;
            uint32 m_uiConeTimer;
            uint32 m_uiCounterspellTimer;

            void Reset()
            {
                m_uiBlizzardTimer      = urand(15000, 30000);
                m_uiConeTimer          = urand(10000, 15000);
                m_uiCounterspellTimer  = urand(10000, 20000);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
			    DoCast (me, SPELL_FROST_ARMOR);
			    DoCast (me, SPELL_ICE_BARRIER);
            }

            void EnterEvadeMode()
            {
            }
			
            void KilledUnit(Unit *victim)
            {
            }
			
            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
				
                //Blizzard Timer
                if (m_uiBlizzardTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_BLIZZARD);

                    m_uiBlizzardTimer = urand(15000, 30000);
                }
                else
                    m_uiBlizzardTimer -= uiDiff;

                //Cone of Cold Timer
                if (m_uiConeTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_CONE_OF_COLD);

                    m_uiConeTimer = urand(10000, 15000);
                }
                else
                    m_uiConeTimer -= uiDiff;

                //Counterspell Timer
                if (m_uiCounterspellTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_COUNTERSPELL);

                    m_uiCounterspellTimer = urand(10000, 20000);
                }
                else
                    m_uiCounterspellTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mage_ice_instanceAI(creature);
        }

};

class npc_mage_fire_instance : public CreatureScript
{
    public:

        npc_mage_fire_instance()
            : CreatureScript("npc_mage_fire_instance")
        {
        }

        struct npc_mage_fire_instanceAI : public ScriptedAI
        {
            npc_mage_fire_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiHealTimer;
            uint32 m_uiFFireballTimer;
            uint32 m_uiFireNovaTimer;

            void Reset()
            {
                m_uiHealTimer      = urand(15000, 20000);
                m_uiFFireballTimer = urand(10000, 15000);
                m_uiFireNovaTimer  = urand(10000, 20000);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
			    DoCast (me, SPELL_MOLTEN_ARMOR);
			    DoCast (me, SPELL_MANA_SHIELD);
            }

            void EnterEvadeMode()
            {
            }
			
            void KilledUnit(Unit *victim)
            {
            }
			
            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
				
                //Chain heal Timer
                if (m_uiHealTimer <= uiDiff)
                {
                    if (Unit *pAlly = DoSelectLowestHpFriendly(INTERACTION_DISTANCE*100))
                        DoCast(pAlly, SPELL_HEAL);

                    m_uiHealTimer = urand(15000, 20000);
                }
                else
                    m_uiHealTimer -= uiDiff;

                //Fel Fireball Timer
                if (m_uiFFireballTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_FEL_FIREBALL);

                    m_uiFFireballTimer = urand(10000, 15000);
                }
                else
                    m_uiFFireballTimer -= uiDiff;

                //Fire Nova timer
                if (m_uiFireNovaTimer <= uiDiff)
                {
                    DoCast(me, SPELL_FIRE_NOVA);

                    m_uiFireNovaTimer = urand(8000,  15000);
                }
                else
                    m_uiFireNovaTimer -= uiDiff;


                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mage_fire_instanceAI(creature);
        }

};

class npc_dk_instance : public CreatureScript
{
    public:

        npc_dk_instance()
            : CreatureScript("npc_dk_instance")
        {
        }

        struct npc_dk_instanceAI : public ScriptedAI
        {
            npc_dk_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiFrostStrikeTimer;
            uint32 m_uiDeathCoilTimer;
            uint32 m_uiGlacialStrikeTimer;
            uint32 m_uiTapTimer;

            void Reset()
            {
                m_uiFrostStrikeTimer      = urand(6000, 10000);
                m_uiDeathCoilTimer        = urand(5000, 15000);
                m_uiGlacialStrikeTimer    = urand(10000,17000);
                m_uiTapTimer              = urand(12000,17000);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
            }

            void EnterEvadeMode()
            {
            }
			
            void KilledUnit(Unit *victim)
            {
            }
			
            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
				
                //Death Coil Timer
                if (m_uiDeathCoilTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_DEATH_COIL);

                    m_uiDeathCoilTimer = urand(5000, 15000);
                }
                else
                    m_uiDeathCoilTimer -= uiDiff;

                //Frost Strike Timer
                if (m_uiFrostStrikeTimer <= uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_FROST_STRIKE);

                    m_uiFrostStrikeTimer = urand(7000, 10000);
                }
                else
                    m_uiFrostStrikeTimer -= uiDiff;

                //Glacial Strike Timer
                if (m_uiGlacialStrikeTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_GLACIAL_STRIKE);

                    m_uiGlacialStrikeTimer = urand(7000, 17000);
                }
                else
                    m_uiGlacialStrikeTimer -= uiDiff;
					
                // Rune Tap Timer
                if (m_uiTapTimer <= uiDiff && !HealthAbovePct(35))
                {
                    me->CastSpell(me, SPELL_ANTIMAGIC_ZONE, true);

                    m_uiTapTimer = urand(12000, 17000);
                }
                else
                    m_uiTapTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_dk_instanceAI(creature);
        }

};

class npc_warrior_instance : public CreatureScript
{
    public:

        npc_warrior_instance()
            : CreatureScript("npc_warrior_instance")
        {
        }

        struct npc_warrior_instanceAI : public ScriptedAI
        {
            npc_warrior_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiShockwaveTimer;
            uint32 m_uiBloodthristTimer;
            uint32 m_uiDisarmTimer;
			uint8 stage;
          
		 void setStage(uint8 phase)
        {
            stage = phase;
        }

        uint8 getStage()
        {
            return stage;
        }
		
            void Reset()
            {
                m_uiShockwaveTimer      = urand(8000,12000);
                m_uiBloodthristTimer    = urand(5000, 9000);
                m_uiDisarmTimer         = urand(8000,20000);
				setStage(0);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
			 switch (getStage()){
			  case 0:
			   DoCast(me->getVictim(), SPELL_CHARGE);
			  setStage(1);
			  break;
			  case 1:
				DoCast(me, SPELL_BATTLE_SHOUT); 
				setStage(2);
			   }
            }

            void EnterEvadeMode()
            {
            }
			
            void KilledUnit(Unit *victim)
            {
            }
			
            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
				
                //Shockwave Timer
                if (m_uiShockwaveTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_SHOCKWAVE);

                    m_uiShockwaveTimer = urand(8000, 10000);
                }
                else
                    m_uiShockwaveTimer -= uiDiff;

                //Bloddthrist Timer
                if (m_uiBloodthristTimer <= uiDiff)
                {
                    DoCast(me->getVictim(), SPELL_BLOODTHRIST);

                    m_uiBloodthristTimer = urand(5000, 9000);
                }
                else
                    m_uiBloodthristTimer -= uiDiff;

                //Disarm Timer
                if (m_uiDisarmTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_DISARM);

                    m_uiDisarmTimer = urand(8000, 10000);
                }
                else
                    m_uiDisarmTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_warrior_instanceAI(creature);
        }

};

class npc_hunter_instance : public CreatureScript
{
    public:

        npc_hunter_instance()
            : CreatureScript("npc_hunter_instance")
        {
        }

        struct npc_hunter_instanceAI : public ScriptedAI
        {
            npc_hunter_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiAimedShotTimer;
            uint32 m_uiSerpentStringTimer;
            uint32 m_uiMultiShotTimer;

            void Reset()
            {
                m_uiAimedShotTimer             = urand(4000, 6000);
                m_uiSerpentStringTimer         = urand(5000, 9000);
                m_uiMultiShotTimer             = urand(8000, 20000);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
			  DoCast(me, SPELL_ASPECT_DRAGONHAWK);
            }

            void EnterEvadeMode()
            {
            }
			
            void KilledUnit(Unit *victim)
            {
            }
			
            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
				
                //AimedShot Timer
                if (m_uiAimedShotTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_AIMEDSHOT);

                    m_uiAimedShotTimer = urand(4000, 6000);
                }
                else
                    m_uiAimedShotTimer -= uiDiff;

                //SerpentString Timer
                if (m_uiSerpentStringTimer <= uiDiff)
                {
                        DoCast(me->getVictim(), SPELL_SERPENTSTRING);

                    m_uiSerpentStringTimer = urand(5000, 9000);
                }
                else
                    m_uiSerpentStringTimer -= uiDiff;

                //MultiShot Timer
                if (m_uiMultiShotTimer <= uiDiff)
                {
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(pTarget, SPELL_MULTISHOT_1);

                    m_uiMultiShotTimer = urand(8000, 20000);
                }
                else
                    m_uiMultiShotTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_hunter_instanceAI(creature);
        }

};

class npc_priest_instance : public CreatureScript
{
    public:

        npc_priest_instance()
            : CreatureScript("npc_priest_instance")
        {
        }

        struct npc_priest_instanceAI : public ScriptedAI
        {
            npc_priest_instanceAI(Creature *c) : ScriptedAI(c) {}

            uint32 m_uiMassHealTimer;
            uint32 m_uiFleshHealTimer;
            uint32 m_uiVampiricTouchTimer;
	     uint32 m_uiMindFlyTimer;
	     uint32 m_uiFearTimer;
		 uint8 stage;
		 
		 void setStage(uint8 phase)
        {
            stage = phase;
        }

        uint8 getStage()
        {
            return stage;
        }

            void Reset()
            {
			    m_uiMassHealTimer                   = urand(24000, 26000);
			    m_uiFleshHealTimer                  = urand(15000, 19000);
			    m_uiVampiricTouchTimer              = urand(3000, 5000);
			    m_uiMindFlyTimer                    = urand(7000, 9000);
			    m_uiFearTimer                       = urand(11000, 15000);
				setStage(0);
            }

            void JustSummoned(Creature *summon)
            {
            }

            void EnterCombat(Unit* pWho)
            {
			   switch (getStage()){
			  case 0:
		      DoCast(me, SPELL_INER_FIRE);
			  setStage(1);
			break;
			case 1:
		      DoCast(me, SPELL_SHIELD);
			  setStage(2);
			  break;
			case 2: 
		      DoCast(me, SPELL_FORTITUDE);
			  setStage(3);
			  break;
			  case 3: 
		      DoCast(me, SPELL_RENEW);
			  setStage(4);
			  }
            }

            void EnterEvadeMode()
            {
            }
			
            void KilledUnit(Unit *victim)
            {
            }
			
            void JustDied(Unit* /*killer*/)
            {
            }

            void UpdateAI(const uint32 uiDiff)
            {
                if (!UpdateVictim())
                    return;
					
                if (me->HasUnitState(UNIT_STAT_CASTING))
                    return;
				
                //Heal Timer
                if (m_uiMassHealTimer <= uiDiff)
                {
                   DoCast(me, SPELL_MASSHEAL);

                   m_uiMassHealTimer = urand(24000, 26000);
                }
                else
                    m_uiMassHealTimer -= uiDiff;

                //FleshHeal Timer
                if (m_uiFleshHealTimer <= uiDiff)
                {
                    DoCast(me, SPELL_FLESH_HEAL);

                    m_uiFleshHealTimer = urand(15000, 19000);
                }
                else
                    m_uiFleshHealTimer -= uiDiff;

                //VampiricTouch Timer
                if (m_uiVampiricTouchTimer <= uiDiff)
                {
                        DoCast(me->getVictim(), SPELL_VAMPIRIC_TOUCH);

                    m_uiVampiricTouchTimer = urand(3000, 5000);
                }
                else
                    m_uiVampiricTouchTimer -= uiDiff;
					
				//Mind Fly Timer
                if (m_uiMindFlyTimer <= uiDiff)
                {
                        DoCast(me->getVictim(), SPELL_MIND_FLY);

                    m_uiMindFlyTimer = urand(7000, 9000);
                }
                else
                    m_uiMindFlyTimer -= uiDiff;

				//Fear Timer
                if (m_uiFearTimer <= uiDiff)
                {
                        DoCast(me->getVictim(), SPELL_FEAR);

                    m_uiFearTimer = urand(11000, 15000);
                }
                else
                    m_uiFearTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_priest_instanceAI(creature);
        }

};

class npc_likantropo : public CreatureScript
{
public:
    npc_likantropo() : CreatureScript("npc_likantropo") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_likantropoAI (pCreature);
    }

    struct npc_likantropoAI : public ScriptedAI
    {
        npc_likantropoAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 EnrageTimer;
        uint32 WoundTimer;
        uint32 BiteTimer;
        uint32 FeverTimer;
        bool Enraged;
        bool Worgen;

        void Reset()
        {
            me->RestoreDisplayId();
            EnrageTimer = 15000;
            WoundTimer = 5000;
            BiteTimer = 45000;
            FeverTimer = 12000;
            Enraged = false;
            Worgen = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_LIKANTROPO_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_LIKANTROPO_SLAY, LANG_UNIVERSAL, 0);
            // In caso di morte di un pg, evoca un worgen dal suo corpo <3 ale
            if (victim->GetTypeId() == TYPEID_PLAYER)
                victim->SummonCreature(SUMMON_WORGEN, victim->GetPositionX(), victim->GetPositionY(), victim->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_LIKANTROPO_DEATH, LANG_UNIVERSAL, 0);
            me->RestoreDisplayId();
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            // Piccoli Enrage (durata 8 secondi) che raddoppiano il danno ogni 20 sec circa
            if (EnrageTimer <= uiDiff)
            {
                DoCast(me, ENRAGE);
                EnrageTimer = urand(20000,25000);
            } else EnrageTimer -= uiDiff;

            // Ogni 5 sec applica uno stack di wound che riduce del 5% le cure (max 15 stack)
            if (WoundTimer <= uiDiff)
            {
                DoCast(me->getVictim(), MORTAL_WOUND);
                WoundTimer = 5000;
            } else WoundTimer -= uiDiff;

            // Ogni 20 sec applica un disease che riduce del 50% gli hp e fa 3k ogni 3 sec per 21 sec
            if (FeverTimer <= uiDiff)
            {
                DoCastAOE(DECREPIT_FEVER);
                FeverTimer = 20000;
            } else FeverTimer -= uiDiff;

            // Ogni 45 secondi Whirlwind che colpisce ad area facendo il danno normale
            // e contemporaneamente trasforma un target a caso in Worgen, disattivandogli le spell
            if (BiteTimer <= uiDiff)
            {
                DoCast(me, WORGEN_BITE);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    me->AddAura(WORGEN_CALL, pTarget);
                BiteTimer = 45000;
            } else BiteTimer -= uiDiff;

            // Ad 80% hp si trasforma in Worgen
            if (HealthBelowPct(80) && !Worgen)
            {
                me->SetDisplayId(26787);
                me->MonsterYell(SAY_LIKANTROPO_MORPH, LANG_UNIVERSAL, 0);
                DoCast(me, ENRAGE);
                EnrageTimer = urand(25000,35000);
                Worgen = true;
            }

            // Enrage a 15% di vita (aumenta 50% attack speed e 50% damage)
            if (HealthBelowPct(15) && !Enraged)
            {
                DoCast(me, FRENZY);
                Enraged = true;
            }

            DoMeleeAttackIfReady();
        }
    };

};


/*#########
# Lava Boost
#########*/

class npc_lava_boost : public CreatureScript
{
public:
    npc_lava_boost() : CreatureScript("npc_lava_boost") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_lava_boostAI (pCreature);
    }

    struct npc_lava_boostAI : public ScriptedAI
    {
        npc_lava_boostAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 ChainTimer;
        uint32 LavaTimer;
        uint32 FlameTimer;
        uint32 ShockTimer;
        bool Haste;
        bool Healed;

        void Reset()
        {
            ChainTimer = 8000;
            LavaTimer = 15000;
            FlameTimer = 4000;
            ShockTimer = 20000;
            Haste = false;
            Healed = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_LAVA_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_LAVA_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_LAVA_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            // Chain Lightning ogni 10/15 sec
            if (ChainTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me->getVictim(), CHAIN_LIGHTNING);
                ChainTimer = urand(10000,15000);
            } else ChainTimer -= uiDiff;

            // Ogni 15 sec casta Lava Burst su un target random da 8/10k
            if (LavaTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    DoCast(pTarget, LAVA_BURST);
                DoCast(me, LIGHTNING_SHIELD);
                LavaTimer = 15000;
            } else LavaTimer -= uiDiff;

            // Ogni 20 sec casta Earth Shock su un target random
            if (ShockTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,1))
                    DoCast(pTarget, EARTH_SHOCK);
                DoCast(me, LIGHTNING_RING);
                ShockTimer = 20000;
            } else ShockTimer -= uiDiff;

            // Ogni 12 sec applica Flame Shock sul tank
            if (FlameTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me->getVictim(), FLAME_SHOCK);
                FlameTimer = 12000;
            } else FlameTimer -= uiDiff;

            // A 25% hp si cura con Healing Wave (solo una volta)
            if (HealthBelowPct(25) && !Healed)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me, HEALING_WAVE);
                ChainTimer = 8000;
                LavaTimer = 10000;
                FlameTimer = 4000;
                ShockTimer = 15000;
                Healed = true;
            }

            // Bloodlust a 50% di vita e Summon Air Elemental
            if (HealthBelowPct(50) && !Haste)
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me, BLOODLUST);
                me->MonsterYell(SAY_LAVA_BLOOD, LANG_UNIVERSAL, 0);
                me->SummonCreature(SUMMON_AIR_ELEMENTAL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                Haste = true;
            }

            DoSpellAttackIfReady(LIGHTNING_BOLT);
        }
    };

};


/*#########
# Quel'Nagas
#########*/

class npc_quel_nagas : public CreatureScript
{
public:
    npc_quel_nagas() : CreatureScript("npc_quel_nagas") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_quel_nagasAI (pCreature);
    }

    struct npc_quel_nagasAI : public ScriptedAI
    {
        npc_quel_nagasAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 Phase2Timer;
        uint32 Phase1Timer;
        uint32 SporeTimer;
        uint32 PoisonTimer;
        uint32 RootTimer;
        bool Phase1;

        void Reset()
        {
            Phase2Timer = 60000;
            Phase1Timer = 90000;
            SporeTimer = 6000;
            PoisonTimer = 15000;
            RootTimer = 25000;
            Phase1 = true;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_NAGAS_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_NAGAS_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_NAGAS_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            // Summon Phase
            if (Phase2Timer <= uiDiff)
            {
                Phase1 = false;
                me->InterruptNonMeleeSpells(false);
                me->SetReactState(REACT_PASSIVE);
                me->AttackStop();
                me->RemoveAllAuras();
                // In Fase 2 evoca 3 Murloc
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    for (uint8 i = 1; i <= 3; i++)
                    {
                        me->SummonCreature(SUMMONED_MURLOC, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }
                me->AddAura(SPELL_MAGIC_BARRIER, me);
                me->MonsterYell(SAY_NAGAS_SUMMON, LANG_UNIVERSAL, 0);
                Phase2Timer = 90000;
            } else Phase2Timer -= uiDiff;

            // Normal Phase
            if (Phase1Timer <= uiDiff)
            {
                Phase1 = true;
                me->InterruptNonMeleeSpells(false);
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveAurasDueToSpell(SPELL_MAGIC_BARRIER);
                DoZoneInCombat();
                if (me->getThreatManager().isThreatListEmpty())
                    EnterEvadeMode();
                Phase1Timer = 90000;
            } else Phase1Timer -= uiDiff;

            if (Phase1)
            {
                // Ogni 15 sec lancia una spora velenosa ai piedi di un player random
                if (SporeTimer <= uiDiff)
                {
                    me->InterruptNonMeleeSpells(false);
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    {
                        Creature* trig = me->SummonCreature(TOXIC_SPORES_TRIGGER, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000);
                        if (trig)
                        {
                            trig->setFaction(14);
                            trig->CastSpell(trig, SPELL_TOXIC_SPORES,true);
                        }
                    }
                    SporeTimer = 15000;
                } else SporeTimer -= uiDiff;

                // Poison ad area ogni 10 sec
                if (PoisonTimer <= uiDiff)
                {
                    me->InterruptNonMeleeSpells(false);
                    DoCastAOE(SPELL_POISON_BOLT_VOLLEY);
                    PoisonTimer = 10000;
                } else PoisonTimer -= uiDiff;

                // Radici su un target random ogni 25 sec
                if (RootTimer <= uiDiff)
                {
                    me->InterruptNonMeleeSpells(false);
                    if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(pTarget, SPELL_ENTANGLE);
                    RootTimer = 25000;
                } else RootTimer -= uiDiff;
            }

            DoSpellAttackIfReady(SPELL_WRATH);
        }
    };

};


/*#########
# Might of Sylvanas
#########*/

class npc_might_of_sylvanas : public CreatureScript
{
public:
    npc_might_of_sylvanas() : CreatureScript("npc_might_of_sylvanas") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_might_of_sylvanasAI (pCreature);
    }

    struct npc_might_of_sylvanasAI : public ScriptedAI
    {
        npc_might_of_sylvanasAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 MultiTimer;
        uint32 VolleyTimer;
        uint32 SnakeTimer;
        uint32 PoisonTimer;
        uint32 TrapTimer;
        uint32 HealTimer;
        bool Haste;
        bool Healed;

        void Reset()
        {
            MultiTimer = 2000;
            VolleyTimer = 15000;
            SnakeTimer = 5000;
            PoisonTimer = 12000;
            TrapTimer = 20000;
            HealTimer = 25000;
            Haste = false;
            Healed = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_SYLVANAS_AGGRO, LANG_UNIVERSAL, 0);
            me->AddAura(SPELL_BEWITCHING_AURA, me);
            me->AddAura(SPELL_AURA_ROT, me);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_SYLVANAS_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_SYLVANAS_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UNIT_STAT_CASTING))
                return;

            // Multishot ogni 2 sec
            if (MultiTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(pTarget, SPELL_MULTISHOT);
                MultiTimer = 4000;
            } else MultiTimer -= uiDiff;

            // Ogni 10 sec casta Volley su un target random
            if (VolleyTimer <= uiDiff)
            {
                DoCastAOE(SPELL_VOLLEY);
                VolleyTimer = 15000;
            } else VolleyTimer -= uiDiff;

            // Ogni 5 sec casta Snake Trap su un target random
            if (SnakeTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    for (uint8 i = 1; i <= 3; i++)
                    {
                        me->SummonCreature(SUMMONED_SNAKE, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                    }
                SnakeTimer = 5000;
            } else SnakeTimer -= uiDiff;

            // Poison ad area ogni 10 sec
            if (PoisonTimer <= uiDiff)
            {
                DoCastAOE(SPELL_BOLT_VOLLEY);
                PoisonTimer = 12000;
            } else PoisonTimer -= uiDiff;

            // Ogni 20 sec casta Frost Trap sul tank
            if (TrapTimer <= uiDiff)
            {
                me->MonsterYell(SAY_SYLVANAS_ICE, LANG_UNIVERSAL, 0);
                DoCast(me->getVictim(), SPELL_ICE_TRAP);
                TrapTimer = 20000;
            } else TrapTimer -= uiDiff;

            // Ogni 25 sec casta Rejuvenation
            if (HealTimer <= uiDiff)
            {
                DoCast(me, SPELL_REJUVENATION);
                HealTimer = 25000;
            } else HealTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};


/*#########
# Trollzilla
#########*/

class npc_trollzilla : public CreatureScript
{
public:
    npc_trollzilla() : CreatureScript("npc_trollzilla") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_trollzillaAI (pCreature);
    }

    struct npc_trollzillaAI : public ScriptedAI
    {
        npc_trollzillaAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 CleaveTimer;
        uint32 StompTimer;
        uint32 FireTimer;
        bool Enraged;

        void Reset()
        {
            CleaveTimer = 5000;
            StompTimer = 20000;
            FireTimer = 30000;
            Enraged = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_TROLLZILLA_AGGRO, LANG_UNIVERSAL, 0);
            me->AddAura(SPELL_FIRE_SHIELD, me);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_TROLLZILLA_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_TROLLZILLA_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            // Ogni 5 sec fa Cleave
            if (CleaveTimer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_CLEAVE);
                CleaveTimer = 5000;
            } else CleaveTimer -= uiDiff;

            // Ogni 20 sec fa un War Stomp che stunna ad area
            if (StompTimer <= uiDiff)
            {
                DoCastAOE(SPELL_WAR_STOMP);
                StompTimer = 20000;
            } else StompTimer -= uiDiff;

            // Summon Fire Elemental ogni 30 sec
            if (FireTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    me->SummonCreature(SUMMON_FIRE_ELEMENTAL, pTarget->GetPositionX(), pTarget->GetPositionY(), pTarget->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
                FireTimer = 30000;
            } else FireTimer -= uiDiff;

            // Enrage a 25% di vita (aumenta 40% attack speed e 25% damage)
            if (HealthBelowPct(25) && !Enraged)
            {
                me->MonsterYell(SAY_TROLLZILLA_FRENZY, LANG_UNIVERSAL, 0);
                DoCast(me, SPELL_FRENZY);
                Enraged = true;
            }

            DoMeleeAttackIfReady();
        }
    };

};


/*#########
# Perotharn
#########*/

class npc_perotharn : public CreatureScript
{
public:
    npc_perotharn() : CreatureScript("npc_perotharn") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_perotharnAI (pCreature);
    }

    struct npc_perotharnAI : public ScriptedAI
    {
        npc_perotharnAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 BurstTimer;
        uint32 GriefTimer;
        uint32 FlameTimer;
        uint32 PiercingTimer;
        uint32 HarvestTimer;

        void Reset()
        {
            BurstTimer = 30000;
            GriefTimer = 20000;
            FlameTimer = 15000;
            PiercingTimer = 10000;
            HarvestTimer = 45000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_PEROTHARN_AGGRO, LANG_UNIVERSAL, 0);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_PEROTHARN_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_PEROTHARN_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            // Shadow Burst ad area e reset threat
            if (BurstTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                me->DeleteThreatList();
                DoCast(me->getVictim(), SHADOW_BURST);
                BurstTimer = urand(25000,35000);
            } else BurstTimer -= uiDiff;

            // Storm of Grief ogni 20 sec colpisce ad area
            if (GriefTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, STORM_OF_GRIEF);
                GriefTimer = 20000;
            } else GriefTimer -= uiDiff;

            // Ogni 10 sec casta Piercing Shadow sul target a maggior aggro dopo il tank
            if (PiercingTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_TOPAGGRO,0))
                    DoCast(pTarget, PIERCING_SHADOW);
                PiercingTimer = 10000;
            } else PiercingTimer -= uiDiff;

            // Ogni 15 sec casta Shadow Flame che fa danno a cono
            if (FlameTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                DoCastAOE(SHADOW_FLAME);
                FlameTimer = 15000;
            } else FlameTimer -= uiDiff;

            // Ogni 45 sec casta Harvest Soul su tutto il raid
            if (HarvestTimer <= uiDiff)
            {
                me->InterruptNonMeleeSpells(false);
                me->MonsterYell(SAY_PEROTHARN_SOUL, LANG_UNIVERSAL, 0);
                DoCastAOE(HARVEST_SOUL);
                HarvestTimer = 45000;
            } else HarvestTimer -= uiDiff;

            DoSpellAttackIfReady(SHADOW_BOLT);
        }
    };

};


/*#########
# Uznam the Watcher
#########*/

class npc_uznam_the_watcher : public CreatureScript
{
public:
    npc_uznam_the_watcher() : CreatureScript("npc_uznam_the_watcher") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_uznam_the_watcherAI (pCreature);
    }

    struct npc_uznam_the_watcherAI : public ScriptedAI
    {
        npc_uznam_the_watcherAI(Creature *pCreature) : ScriptedAI(pCreature){}

        uint32 EarthquakeTimer;
        uint32 WhirlwindTimer;
        uint32 ShockTimer;
        uint32 AuralShockTimer;

        void Reset()
        {
            EarthquakeTimer = 8000;
            WhirlwindTimer = 20000;
            ShockTimer = 30000;
            AuralShockTimer = 50000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            me->MonsterYell(SAY_UZNAM_AGGRO, LANG_UNIVERSAL, 0);
            DoCast(me, ENTROPIC_AURA);
        }

        void KilledUnit(Unit* victim)
        {
            me->MonsterYell(SAY_UZNAM_SLAY, LANG_UNIVERSAL, 0);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->MonsterYell(SAY_UZNAM_DEATH, LANG_UNIVERSAL, 0);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            if (EarthquakeTimer <= uiDiff)
            {
                DoCast(EARTHQUAKE);
                EarthquakeTimer = 20000;
            } else EarthquakeTimer -= uiDiff;

            // Whirlwind Ogni 30 sec
            if (WhirlwindTimer <= uiDiff)
            {
                me->MonsterYell(SAY_UZNAM_SPECIAL, LANG_UNIVERSAL, 0);
                me->MonsterTextEmote(EMOTE_WHIRL, 0, true);
                DoCast(me, WHIRLWIND);
                WhirlwindTimer = 30000;
            } else WhirlwindTimer -= uiDiff;

            if (ShockTimer <= uiDiff)
            {
                if (Unit *pTarget = SelectTarget(SELECT_TARGET_RANDOM,0))
                    DoCast(pTarget, SHOCK_OF_SORROW);
                ShockTimer = 40000;
            } else ShockTimer -= uiDiff;
            
            if (AuralShockTimer <= uiDiff)
            {
                DoCast(AURAL_SHOCK);
                AuralShockTimer = 60000;
            } else AuralShockTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_bosses_instance()
{
    new boss_firelord_instance();
    new boss_icelord_instance();
    new boss_earthlord_instance();
    new boss_darklord_instance();
    new npc_mage_ice_instance();
    new npc_mage_fire_instance();
    new npc_dk_instance();
    new npc_warrior_instance();
    new npc_hunter_instance();
    new npc_priest_instance();
	new npc_likantropo;
    new npc_lava_boost;
    new npc_quel_nagas;
    new npc_might_of_sylvanas;
    new npc_trollzilla;
    new npc_perotharn;
    new npc_uznam_the_watcher;
}