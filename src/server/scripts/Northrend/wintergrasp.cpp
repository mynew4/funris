/*
 * Copyright (C) 2010-2011 Izb00shka <http://izbooshka.net/>
 * Copyright (C) 2008 - 2009 Trinity <http://www.trinitycore.org/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ScriptPCH.h"
#include "OutdoorPvPWG.h"

#define GOSSIP_HELLO_DEMO1  "Build catapult."
#define GOSSIP_HELLO_DEMO2  "Build demolisher."
#define GOSSIP_HELLO_DEMO3  "Build siege engine."
#define GOSSIP_HELLO_DEMO4  "I cannot build more!"

#define GOSSIP_HELLO_DEMO1_RU  "Построить катапульту"
#define GOSSIP_HELLO_DEMO2_RU  "Построить разрушитель"
#define GOSSIP_HELLO_DEMO3_RU  "Построить осадную машину"
#define GOSSIP_HELLO_DEMO4_RU  "Больше строить нельзя!"

class npc_demolisher_engineerer : public CreatureScript
{
public:
    npc_demolisher_engineerer() : CreatureScript("npc_demolisher_engineerer") { }


    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
        if (pCreature->isQuestGiver())
            pPlayer->PrepareQuestMenu(pCreature->GetGUID());

        if(!pCreature->IsNonMeleeSpellCasted(true) && (pPlayer->isGameMaster() || pCreature->GetZoneScript() && pCreature->GetZoneScript()->GetData(pCreature->GetDBTableGUIDLow())))
        {
            if (pPlayer->HasAura(SPELL_CORPORAL))
            {
                if (!pPlayer->HasSpellCooldown(56663))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (pPlayer->isRussianLocale()) ? GOSSIP_HELLO_DEMO1_RU : GOSSIP_HELLO_DEMO1, GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF);
            }
            else if (pPlayer->HasAura(SPELL_LIEUTENANT))
            {
                if (!pPlayer->HasSpellCooldown(56663))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (pPlayer->isRussianLocale()) ? GOSSIP_HELLO_DEMO1_RU : GOSSIP_HELLO_DEMO1, GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF);

                if (!pPlayer->HasSpellCooldown(56575))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (pPlayer->isRussianLocale()) ? GOSSIP_HELLO_DEMO2_RU : GOSSIP_HELLO_DEMO2, GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+1);

                if (!pPlayer->HasSpellCooldown(pPlayer->GetTeamId() ? 61408 : 56661))
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (pPlayer->isRussianLocale()) ? GOSSIP_HELLO_DEMO3_RU : GOSSIP_HELLO_DEMO3, GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+2);
            }
        }
        else
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (pPlayer->isRussianLocale()) ? GOSSIP_HELLO_DEMO4_RU : GOSSIP_HELLO_DEMO4, GOSSIP_SENDER_MAIN,   GOSSIP_ACTION_INFO_DEF+9);

        pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
    {
        pPlayer->PlayerTalkClass->ClearMenus();

        pPlayer->CLOSE_GOSSIP_MENU();
        if(pPlayer->isGameMaster() || pCreature->GetZoneScript() && pCreature->GetZoneScript()->GetData(pCreature->GetDBTableGUIDLow()))
        {
            switch(uiAction - GOSSIP_ACTION_INFO_DEF)
            {
            case 0:
                if (!pPlayer->HasSpellCooldown(56663))
                {
                    pCreature->CastSpell(pPlayer, 56663, false, NULL, NULL, pCreature->GetGUID());
                    pPlayer->AddSpellCooldown(56663, NULL, time(NULL) + 60);
                }                
                break;
            case 1: 
                if (!pPlayer->HasSpellCooldown(56575))
                {
                    pCreature->CastSpell(pPlayer, 56575, false, NULL, NULL, pCreature->GetGUID());
                    pPlayer->AddSpellCooldown(56575, NULL, time(NULL) + 60);
                } 
                break;
            case 2:
                uint32 spellID = pPlayer->GetTeamId() ? 61408 : 56661;
                if (!pPlayer->HasSpellCooldown(spellID))
                {
                    pCreature->CastSpell(pPlayer, spellID, false, NULL, NULL, pCreature->GetGUID());
                    pPlayer->AddSpellCooldown(spellID, NULL, time(NULL) + 60);
                }                
                break;
            }
        }

        return true;
    }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_demolisher_engineererAI(pCreature);
    }

    struct npc_demolisher_engineererAI : public ScriptedAI
    {
        npc_demolisher_engineererAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            me->SetReactState(REACT_PASSIVE);
        }
    };

};
void AddSC_wintergrasp()
{
    new npc_demolisher_engineerer();
}