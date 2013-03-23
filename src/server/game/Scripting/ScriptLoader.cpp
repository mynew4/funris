/*
 * Copyright (C) 2010-2011 Izb00shka <http://izbooshka.net/>
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptLoader.h"

//custom
void AddSC_petguy();
void AddSC_npc_titlevendor();
void AddSC_shadowmorn_vendor();
void AddSC_teleguy();
void AddSC_crw_scripts();
void AddSC_strand_of_the_ancients();
void AddSC_spectator();
void AddSC_prof_master();
void AddSC_renamer();
void AddSC_flyer();
void AddSC_npc_renaissance();
void AddSC_npc_amegawow();
void AddSC_bosses_instance();
void AddSC_DuelReset();
void AddSC_challenge_script();
void AddSC_fast_arena_start();
void AddSC_npc_helper();
void AddSC_coolown_object();
void AddSC_npc_other();
void AddSC_npc_premium_master();
void AddSC_System_Censure();
void AddSC_mob_gurubashi_ressurect();
void AddSC_BountyHunter();
void AddSC_custom_misc();

// exaile temple
void AddSC_instance_my_instance();
void AddSC_boss_nazaryan();
void AddSC_boss_nazaryans_blood();
void AddSC_boss_hypnotoad_deadman();
void AddSC_boss_hypnotoad();
void AddSC_boss_godzilla();
void AddSC_boss_semargl();

// Transmogrify
void AddSC_transmogrify_script();
void AddSC_trans();
// spells
void AddSC_deathknight_spell_scripts();
void AddSC_druid_spell_scripts();
void AddSC_generic_spell_scripts();
void AddSC_hunter_spell_scripts();
void AddSC_mage_spell_scripts();
void AddSC_paladin_spell_scripts();
void AddSC_priest_spell_scripts();
void AddSC_rogue_spell_scripts();
void AddSC_shaman_spell_scripts();
void AddSC_warlock_spell_scripts();
void AddSC_warrior_spell_scripts();
void AddSC_item_spell_scripts();

void AddSC_SmartSCripts();

//Commands
void AddSC_account_commandscript();
void AddSC_achievement_commandscript();
void AddSC_debug_commandscript();
void AddSC_event_commandscript();
void AddSC_gm_commandscript();
void AddSC_go_commandscript();
void AddSC_gobject_commandscript();
void AddSC_honor_commandscript();
void AddSC_learn_commandscript();
void AddSC_modify_commandscript();
void AddSC_npc_commandscript();
void AddSC_quest_commandscript();
void AddSC_reload_commandscript();
void AddSC_tele_commandscript();
void AddSC_titles_commandscript();
void AddSC_wp_commandscript();

#ifdef SCRIPTS
//world
void AddSC_areatrigger_scripts();
void AddSC_generic_creature();
void AddSC_go_scripts();
void AddSC_guards();
void AddSC_item_scripts();
void AddSC_npc_professions();
void AddSC_npc_innkeeper();
void AddSC_npcs_special();
void AddSC_npc_taxi();
void AddSC_achievement_scripts();

//northrend
void AddSC_isle_of_conquest();
void AddSC_wintergrasp();

// battlegrounds

// outdoor pvp
void AddSC_outdoorpvp_ep();
void AddSC_outdoorpvp_hp();
void AddSC_outdoorpvp_na();
void AddSC_outdoorpvp_si();
void AddSC_outdoorpvp_tf();
void AddSC_outdoorpvp_zm();
void AddSC_outdoorpvp_wg();

// player
void AddSC_chat_log();

#endif

void AddScripts()
{
    AddSpellScripts();
    AddSC_SmartSCripts();
    AddCommandScripts();
#ifdef SCRIPTS
    AddWorldScripts();
    AddNorthrendScripts();
    AddBattlegroundScripts();
    AddOutdoorPvPScripts();
    AddCustomScripts();
#endif
}

void AddSpellScripts()
{
    AddSC_deathknight_spell_scripts();
    AddSC_druid_spell_scripts();
    AddSC_generic_spell_scripts();
    AddSC_hunter_spell_scripts();
    AddSC_mage_spell_scripts();
    AddSC_paladin_spell_scripts();
    AddSC_priest_spell_scripts();
    AddSC_rogue_spell_scripts();
    AddSC_shaman_spell_scripts();
    AddSC_warlock_spell_scripts();
    AddSC_warrior_spell_scripts();
    AddSC_item_spell_scripts();
}

void AddCommandScripts()
{
    AddSC_account_commandscript();
    AddSC_achievement_commandscript();
    AddSC_debug_commandscript();
    AddSC_event_commandscript();
    AddSC_gm_commandscript();
    AddSC_go_commandscript();
    AddSC_gobject_commandscript();
    AddSC_honor_commandscript();
    AddSC_learn_commandscript();
    AddSC_modify_commandscript();
    AddSC_npc_commandscript();
    AddSC_quest_commandscript();
    AddSC_reload_commandscript();
    AddSC_tele_commandscript();
    AddSC_titles_commandscript();
    AddSC_wp_commandscript();
}

void AddWorldScripts()
{
#ifdef SCRIPTS
    AddSC_areatrigger_scripts();
    AddSC_generic_creature();
    AddSC_go_scripts();
    AddSC_guards();
    AddSC_item_scripts();
    AddSC_npc_professions();
    AddSC_npc_innkeeper();
    AddSC_npcs_special();
    AddSC_npc_taxi();
    AddSC_achievement_scripts();
    AddSC_chat_log();
#endif
}

void AddNorthrendScripts()
{
#ifdef SCRIPTS
    AddSC_wintergrasp();
    AddSC_isle_of_conquest();
#endif
}

void AddOutdoorPvPScripts()
{
#ifdef SCRIPTS
    AddSC_outdoorpvp_ep();
    AddSC_outdoorpvp_hp();
    AddSC_outdoorpvp_na();
    AddSC_outdoorpvp_si();
    AddSC_outdoorpvp_tf();
    AddSC_outdoorpvp_zm();
    AddSC_outdoorpvp_wg();
#endif
}

void AddBattlegroundScripts()
{
#ifdef SCRIPTS
#endif
}

#ifdef SCRIPTS
    
/* This is where custom scripts' loading functions should be declared. */
#endif

void AddCustomScripts()
{
#ifdef SCRIPTS
    // custom scripts
    AddSC_custom_misc();
    AddSC_petguy();
    AddSC_npc_titlevendor();
    AddSC_shadowmorn_vendor();
    AddSC_teleguy();
    AddSC_crw_scripts();
    AddSC_strand_of_the_ancients();
    AddSC_spectator();
    AddSC_renamer();
    AddSC_prof_master();
    AddSC_flyer();
	AddSC_npc_renaissance();
	AddSC_npc_amegawow();
	AddSC_bosses_instance();
	AddSC_DuelReset();
    AddSC_challenge_script();
    AddSC_fast_arena_start();
    AddSC_npc_helper();
    AddSC_transmogrify_script();
    AddSC_trans();
    AddSC_mob_gurubashi_ressurect();
    AddSC_BountyHunter();
 

   
	AddSC_coolown_object();
    AddSC_npc_premium_master();
    AddSC_npc_other();	
    // exaile temple
    AddSC_instance_my_instance();
    AddSC_boss_nazaryan();
    AddSC_boss_nazaryans_blood();
    AddSC_boss_hypnotoad_deadman();
    AddSC_boss_hypnotoad();
    AddSC_boss_godzilla();
    AddSC_boss_semargl();
    AddSC_System_Censure();
#endif
}
