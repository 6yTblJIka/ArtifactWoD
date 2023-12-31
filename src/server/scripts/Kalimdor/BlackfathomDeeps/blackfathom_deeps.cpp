////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "blackfathom_deeps.h"
#include "ScriptedEscortAI.h"

enum Spells
{
    SPELL_BLESSING_OF_BLACKFATHOM                           = 8733,
    SPELL_RAVAGE                                            = 8391,
    SPELL_FROST_NOVA                                        = 865,
    SPELL_FROST_BOLT_VOLLEY                                 = 8398,
    SPELL_TELEPORT_DARNASSUS                                = 9268
};

#define GOSSIP_ITEM_MORRIDUNE "Please port me to Darnassus"

const Position HomePosition = {-815.817f, -145.299f, -25.870f, 0};

class go_blackfathom_altar : public GameObjectScript
{
public:
    go_blackfathom_altar() : GameObjectScript("go_blackfathom_altar") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
        if (!player->HasAura(SPELL_BLESSING_OF_BLACKFATHOM))
            player->AddAura(SPELL_BLESSING_OF_BLACKFATHOM, player);
        return true;
    }
};

class go_blackfathom_fire : public GameObjectScript
{
public:
    go_blackfathom_fire() : GameObjectScript("go_blackfathom_fire") { }

    bool OnGossipHello(Player* /*player*/, GameObject* go)
    {
        InstanceScript* instance = go->GetInstanceScript();

        if (instance)
        {
            go->SetGoState(GO_STATE_ACTIVE);
            go->SetFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_NOT_SELECTABLE);
            instance->SetData(DATA_FIRE, instance->GetData(DATA_FIRE) + 1);
            return true;
        }
        return false;
    }
};

class npc_blackfathom_deeps_event : public CreatureScript
{
public:
    npc_blackfathom_deeps_event() : CreatureScript("npc_blackfathom_deeps_event") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_blackfathom_deeps_eventAI (creature);
    }

    struct npc_blackfathom_deeps_eventAI : public ScriptedAI
    {
        npc_blackfathom_deeps_eventAI(Creature* creature) : ScriptedAI(creature)
        {
            if (creature->isSummon())
            {
                creature->SetHomePosition(HomePosition);
                AttackPlayer();
            }

            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 ravageTimer;
        uint32 frostNovaTimer;
        uint32 frostBoltVolleyTimer;

        bool Flee;

        void Reset()
        {
            Flee = false;

            ravageTimer           = urand(5000, 8000);
            frostNovaTimer        = urand(9000, 12000);
            frostBoltVolleyTimer  = urand(2000, 4000);
        }

        void AttackPlayer()
        {
            Map::PlayerList const &PlList = me->GetMap()->GetPlayers();

            if (PlList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
            {
                if (Player* player = i->getSource())
                {
                    if (player->isGameMaster())
                        continue;

                    if (player->isAlive())
                    {
                        me->SetInCombatWith(player);
                        player->SetInCombatWith(me);
                        me->AddThreat(player, 0.0f);
                    }
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            switch (me->GetEntry())
            {
                case NPC_AKU_MAI_SNAPJAW:
                {
                    if (ravageTimer <= diff)
                    {
                        DoCastVictim(SPELL_RAVAGE);
                        ravageTimer = urand(9000, 14000);
                    }
                    else
                        ravageTimer -= diff;
                    break;
                }
                case NPC_MURKSHALLOW_SOFTSHELL:
                case NPC_BARBED_CRUSTACEAN:
                {
                    if (!Flee && HealthBelowPct(15))
                    {
                        Flee = true;
                        me->DoFleeToGetAssistance();
                    }
                    break;
                }
                case NPC_AKU_MAI_SERVANT:
                {
                    if (frostBoltVolleyTimer <= diff)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                            DoCast(target, SPELL_FROST_BOLT_VOLLEY);
                        frostBoltVolleyTimer = urand(5000, 8000);
                    }
                    else
                        frostBoltVolleyTimer -= diff;

                    if (frostNovaTimer <= diff)
                    {
                        DoCastAOE(SPELL_FROST_NOVA, false);
                        frostNovaTimer = urand(25000, 30000);
                    }
                    else
                        frostNovaTimer -= diff;
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (me->isSummon()) //we are not a normal spawn.
                if (instance)
                    instance->SetData(DATA_EVENT, instance->GetData(DATA_EVENT) + 1);
        }
    };
};

enum Morridune
{
    SAY_MORRIDUNE_1 = -1048003,
    SAY_MORRIDUNE_2 = -1048004
};

class npc_morridune : public CreatureScript
{
public:
    npc_morridune() : CreatureScript("npc_morridune") { }

    bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        switch (action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
                player->TeleportTo(1, 9952.239f, 2284.277f, 1341.394f, 1.595f);
                player->CLOSE_GOSSIP_MENU();
                break;
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_MORRIDUNE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_morriduneAI (creature);
    }

    struct npc_morriduneAI : public npc_escortAI
    {
        npc_morriduneAI(Creature* creature) : npc_escortAI(creature)
        {
            DoScriptText(SAY_MORRIDUNE_1, creature);
            me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            Start(false, false, 0);
        }

        void WaypointReached(uint32 waypointId)
        {
            switch (waypointId)
            {
                case 4:
                    SetEscortPaused(true);
                    me->SetFacingTo(1.775791f);
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    DoScriptText(SAY_MORRIDUNE_2, me);
                    break;
            }
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_blackfathom_deeps()
{
    new go_blackfathom_altar();
    new go_blackfathom_fire();
    new npc_blackfathom_deeps_event();
    new npc_morridune();
}
#endif
