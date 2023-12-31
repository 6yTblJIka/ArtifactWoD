////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////


#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"

/*######
## npc_draenei_survivor
######*/

enum draeneiSurvivor
{
    SAY_HEAL1           = -1000176,
    SAY_HEAL2           = -1000177,
    SAY_HEAL3           = -1000178,
    SAY_HEAL4           = -1000179,

    SAY_HELP1           = -1000180,
    SAY_HELP2           = -1000181,
    SAY_HELP3           = -1000182,
    SAY_HELP4           = -1000183,

    SPELL_IRRIDATION    = 35046,
    SPELL_STUNNED       = 28630
};

class npc_draenei_survivor : public CreatureScript
{
public:
    npc_draenei_survivor() : CreatureScript("npc_draenei_survivor") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_draenei_survivorAI (creature);
    }

    struct npc_draenei_survivorAI : public ScriptedAI
    {
        npc_draenei_survivorAI(Creature* creature) : ScriptedAI(creature) {}

        uint64 pCaster;

        uint32 SayThanksTimer;
        uint32 RunAwayTimer;
        uint32 SayHelpTimer;

        bool CanSayHelp;

        void Reset()
        {
            pCaster = 0;

            SayThanksTimer = 0;
            RunAwayTimer = 0;
            SayHelpTimer = 10000;

            CanSayHelp = true;

            DoCast(me, SPELL_IRRIDATION, true);

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            me->SetHealth(me->CountPctFromMaxHealth(10));
            me->SetStandState(UNIT_STAND_STATE_SLEEP);
        }

        void EnterCombat(Unit* /*who*/) {}

        void MoveInLineOfSight(Unit* who)
        {
            if (CanSayHelp && who->IsPlayer() && me->IsFriendlyTo(who) && me->IsWithinDistInMap(who, 25.0f))
            {
                //Random switch between 4 texts
				Talk(1);

                SayHelpTimer = 20000;
                CanSayHelp = false;
            }
        }

        void SpellHit(Unit* Caster, const SpellInfo* Spell)
        {
            if (Spell->SpellFamilyFlags[2] & 0x80000000)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);
                me->SetStandState(UNIT_STAND_STATE_STAND);

                DoCast(me, SPELL_STUNNED, true);

                pCaster = Caster->GetGUID();

                SayThanksTimer = 5000;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (SayThanksTimer)
            {
                if (SayThanksTimer <= diff)
                {
                    me->RemoveAurasDueToSpell(SPELL_IRRIDATION);

                    if (Player* player = Unit::GetPlayer(*me, pCaster))
                    {
						Talk(0);

						me->SetFacingToObject(player);

						player->KilledMonsterCredit(me->GetEntry());
                    }

                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(0, -4115.053711f, -13754.831055f, 73.508949f);

                    RunAwayTimer = 10000;
                    SayThanksTimer = 0;
                }
                else
                    SayThanksTimer -= diff;

                return;
            }

            if (RunAwayTimer)
            {
                if (RunAwayTimer <= diff)
                    me->DespawnOrUnsummon();
                else
                    RunAwayTimer -= diff;

                return;
            }

            if (SayHelpTimer <= diff)
            {
                CanSayHelp = true;
                SayHelpTimer = 20000;
            }
            else
                SayHelpTimer -= diff;
        }
    };

};

// Nestlewood Owlkin - 16518
class npc_nestlewood_owlkin_16518 : public CreatureScript
{
public:
	npc_nestlewood_owlkin_16518() : CreatureScript("npc_nestlewood_owlkin_16518") { }

	struct npc_nestlewood_owlkin_16518AI : public ScriptedAI
	{
		npc_nestlewood_owlkin_16518AI(Creature* creature) : ScriptedAI(creature) { }

		void SpellHit(Unit * Hitter, SpellInfo const* spell)
		{
			Player* player = Hitter->ToPlayer();

			if (!player)
				return;

			if (spell->Id == 29528)
			{
				if (player->GetQuestStatus(37444) == QUEST_STATUS_INCOMPLETE)
				{
					player->KilledMonsterCredit(16518);
					me->DespawnOrUnsummon(0);
				}
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_nestlewood_owlkin_16518AI(creature);
	}
};

/// npc_engineer_spark_overgrind - 17243

enum Overgrind
{
    SAY_TEXT        = -1000184,
    SAY_EMOTE       = -1000185,
    ATTACK_YELL     = -1000186,

    AREA_COVE       = 3579,
    AREA_ISLE       = 3639,
    QUEST_GNOMERCY  = 9537,
    FACTION_HOSTILE = 14,
    SPELL_DYNAMITE  = 7978
};

#define GOSSIP_FIGHT "Traitor! You will be brought to justice!"

class npc_engineer_spark_overgrind : public CreatureScript
{
public:
    npc_engineer_spark_overgrind() : CreatureScript("npc_engineer_spark_overgrind") { }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (action == GOSSIP_ACTION_INFO_DEF)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->setFaction(FACTION_HOSTILE);
            CAST_AI(npc_engineer_spark_overgrind::npc_engineer_spark_overgrindAI, creature->AI())->AttackStart(player);
        }
        return true;
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_GNOMERCY) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_FIGHT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_engineer_spark_overgrindAI (creature);
    }

    struct npc_engineer_spark_overgrindAI : public ScriptedAI
    {
        npc_engineer_spark_overgrindAI(Creature* creature) : ScriptedAI(creature)
        {
            NormFaction = creature->getFaction();
            NpcFlags = creature->GetUInt32Value(UNIT_FIELD_NPC_FLAGS);

            if (creature->GetAreaId() == AREA_COVE || creature->GetAreaId() == AREA_ISLE)
                IsTreeEvent = true;
        }

        uint32 NormFaction;
        uint32 NpcFlags;

        uint32 DynamiteTimer;
        uint32 EmoteTimer;

        bool IsTreeEvent;

        void Reset()
        {
            DynamiteTimer = 8000;
            EmoteTimer = urand(120000, 150000);

            me->setFaction(NormFaction);
            me->SetUInt32Value(UNIT_FIELD_NPC_FLAGS, NpcFlags);

            IsTreeEvent = false;
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(ATTACK_YELL, me, who);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!me->isInCombat() && !IsTreeEvent)
            {
                if (EmoteTimer <= diff)
                {
                    DoScriptText(SAY_TEXT, me);
                    DoScriptText(SAY_EMOTE, me);
                    EmoteTimer = urand(120000, 150000);
                }
                else
                    EmoteTimer -= diff;
            }
            else if (IsTreeEvent)
                return;

            if (!UpdateVictim())
                return;

            if (DynamiteTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_DYNAMITE);
                DynamiteTimer = 8000;
            }
            else
                DynamiteTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

/*######
## npc_injured_draenei
######*/

class npc_injured_draenei : public CreatureScript
{
public:
    npc_injured_draenei() : CreatureScript("npc_injured_draenei") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_injured_draeneiAI (creature);
    }

    struct npc_injured_draeneiAI : public ScriptedAI
    {
        npc_injured_draeneiAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
            me->SetHealth(me->CountPctFromMaxHealth(15));
            switch (urand(0, 1))
            {
                case 0:
                    me->SetStandState(UNIT_STAND_STATE_SIT);
                    break;

                case 1:
                    me->SetStandState(UNIT_STAND_STATE_SLEEP);
                    break;
            }
        }

        void EnterCombat(Unit* /*who*/) {}

        void MoveInLineOfSight(Unit* /*who*/) {}

        void UpdateAI(const uint32 /*diff*/) {}
    };

};

/*######
## npc_magwin
######*/

enum Magwin
{
    SAY_START                   = -1000111,
    SAY_AGGRO                   = -1000112,
    SAY_PROGRESS                = -1000113,
    SAY_END1                    = -1000114,
    SAY_END2                    = -1000115,
    EMOTE_HUG                   = -1000116,

    QUEST_A_CRY_FOR_SAY_HELP    = 9528
};

class npc_magwin : public CreatureScript
{
public:
    npc_magwin() : CreatureScript("npc_magwin") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_A_CRY_FOR_SAY_HELP)
        {
            creature->setFaction(113);
            if (npc_escortAI* pEscortAI = CAST_AI(npc_escortAI, creature->AI()))
                pEscortAI->Start(true, false, player->GetGUID());
        }
        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_magwinAI(creature);
    }

    struct npc_magwinAI : public npc_escortAI
    {
        npc_magwinAI(Creature* creature) : npc_escortAI(creature) {}

        void WaypointReached(uint32 waypointId)
        {
            if (Player* player = GetPlayerForEscort())
            {
                switch (waypointId)
                {
                    case 0:
                        DoScriptText(SAY_START, me, player);
                        break;
                    case 17:
                        DoScriptText(SAY_PROGRESS, me, player);
                        break;
                    case 28:
                        DoScriptText(SAY_END1, me, player);
                        break;
                    case 29:
                        DoScriptText(EMOTE_HUG, me, player);
                        DoScriptText(SAY_END2, me, player);
                        player->GroupEventHappens(QUEST_A_CRY_FOR_SAY_HELP, me);
                        break;
                }
            }
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(SAY_AGGRO, me, who);
        }

        void Reset() {}
    };

};

enum RavegerCage
{
    NPC_DEATH_RAVAGER       = 17556,

    SPELL_REND              = 13443,
    SPELL_ENRAGING_BITE     = 30736,

    QUEST_STRENGTH_ONE      = 9582
};

class go_ravager_cage : public GameObjectScript
{
public:
    go_ravager_cage() : GameObjectScript("go_ravager_cage") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->GetQuestStatus(QUEST_STRENGTH_ONE) == QUEST_STATUS_INCOMPLETE)
        {
            if (Creature* ravager = go->FindNearestCreature(NPC_DEATH_RAVAGER, 5.0f, true))
            {
                ravager->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                ravager->SetReactState(REACT_AGGRESSIVE);
                ravager->AI()->AttackStart(player);
            }
        }
        return true;
    }
};

class npc_death_ravager : public CreatureScript
{
public:
    npc_death_ravager() : CreatureScript("npc_death_ravager") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_death_ravagerAI(creature);
    }

    struct npc_death_ravagerAI : public ScriptedAI
    {
        npc_death_ravagerAI(Creature* creature) : ScriptedAI(creature){}

        uint32 RendTimer;
        uint32 EnragingBiteTimer;

        void Reset()
        {
            RendTimer = 30000;
            EnragingBiteTimer = 20000;

            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (RendTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_REND);
                RendTimer = 30000;
            }
            else
                RendTimer -= diff;

            if (EnragingBiteTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ENRAGING_BITE);
                EnragingBiteTimer = 15000;
            }
            else
                EnragingBiteTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

/*########
## Quest: The Prophecy of Akida
########*/

enum BristlelimbCage
{
    QUEST_THE_PROPHECY_OF_AKIDA         = 9544,
    NPC_STILLPINE_CAPITIVE              = 17375,
    GO_BRISTELIMB_CAGE                  = 181714,

    CAPITIVE_SAY_1                      = -1000474,
    CAPITIVE_SAY_2                      = -1000475,
    CAPITIVE_SAY_3                      = -1000476,

    POINT_INIT                          = 1,
    EVENT_DESPAWN                       = 1
};

class npc_stillpine_capitive : public CreatureScript
{
    public:
        npc_stillpine_capitive() : CreatureScript("npc_stillpine_capitive") { }

        struct npc_stillpine_capitiveAI : public ScriptedAI
        {
            npc_stillpine_capitiveAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                if (GameObject* cage = me->FindNearestGameObject(GO_BRISTELIMB_CAGE, 5.0f))
                {
                    cage->SetLootState(GO_JUST_DEACTIVATED);
                    cage->SetGoState(GO_STATE_READY);
                }
                _events.Reset();
                _player = NULL;
                _movementComplete = false;
            }

            void StartMoving(Player* owner)
            {
                if (owner)
                {
                    DoScriptText(RAND(CAPITIVE_SAY_1, CAPITIVE_SAY_2, CAPITIVE_SAY_3), me, owner);
                    _player = owner;
                }
                Position pos;
                me->GetNearPosition(pos, 3.0f, 0.0f);
                me->GetMotionMaster()->MovePoint(POINT_INIT, pos);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || id != POINT_INIT)
                    return;

                if (_player)
                    _player->KilledMonsterCredit(me->GetEntry(), 0);

                _movementComplete = true;
                _events.ScheduleEvent(EVENT_DESPAWN, 3500);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!_movementComplete)
                    return;

                _events.Update(diff);

                if (_events.ExecuteEvent() == EVENT_DESPAWN)
                    me->DespawnOrUnsummon();
            }

        private:
            Player* _player;
            EventMap _events;
            bool _movementComplete;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_stillpine_capitiveAI(creature);
        }
};

class go_bristlelimb_cage : public GameObjectScript
{
    public:
        go_bristlelimb_cage() : GameObjectScript("go_bristlelimb_cage") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (player->GetQuestStatus(QUEST_THE_PROPHECY_OF_AKIDA) == QUEST_STATUS_INCOMPLETE)
            {
                if (Creature* capitive = go->FindNearestCreature(NPC_STILLPINE_CAPITIVE, 5.0f, true))
                {
                    go->ResetDoorOrButton();
                    CAST_AI(npc_stillpine_capitive::npc_stillpine_capitiveAI, capitive->AI())->StartMoving(player);
                    return false;
                }
            }
            return true;
        }
};
#ifndef __clang_analyzer__
void AddSC_azuremyst_isle()
{
    new npc_draenei_survivor();
	new npc_nestlewood_owlkin_16518();
    new npc_engineer_spark_overgrind();
    new npc_injured_draenei();
    new npc_magwin();
    new npc_death_ravager();
    new go_ravager_cage();
    new npc_stillpine_capitive();
    new go_bristlelimb_cage();
}
#endif
