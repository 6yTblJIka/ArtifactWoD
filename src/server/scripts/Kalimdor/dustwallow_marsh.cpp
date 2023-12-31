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
#include "SpellScript.h"

/*######
## npc_stinky
######*/

enum Stinky
{
    QUEST_STINKYS_ESCAPE_H                       = 1270,
    QUEST_STINKYS_ESCAPE_A                       = 1222,
    SAY_QUEST_ACCEPTED                           = -1000507,
    SAY_STAY_1                                   = -1000508,
    SAY_STAY_2                                   = -1000509,
    SAY_STAY_3                                   = -1000510,
    SAY_STAY_4                                   = -1000511,
    SAY_STAY_5                                   = -1000512,
    SAY_STAY_6                                   = -1000513,
    SAY_QUEST_COMPLETE                           = -1000514,
    SAY_ATTACKED_1                               = -1000515,
    EMOTE_DISAPPEAR                              = -1000516
};

class npc_stinky : public CreatureScript
{
public:
   npc_stinky() : CreatureScript("npc_stinky") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_stinkyAI(creature);
    }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
         if (quest->GetQuestId() == QUEST_STINKYS_ESCAPE_H || quest->GetQuestId() == QUEST_STINKYS_ESCAPE_A)
         {
             if (npc_stinkyAI* pEscortAI = CAST_AI(npc_stinky::npc_stinkyAI, creature->AI()))
             {
                 creature->setFaction(FACTION_ESCORT_N_NEUTRAL_ACTIVE);
                 creature->SetStandState(UNIT_STAND_STATE_STAND);
                 DoScriptText(SAY_QUEST_ACCEPTED, creature);
                 pEscortAI->Start(false, false, player->GetGUID());
             }
         }
         return true;
    }

    struct npc_stinkyAI : public npc_escortAI
    {
       npc_stinkyAI(Creature* creature) : npc_escortAI(creature) { }

        void WaypointReached(uint32 waypointId)
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch (waypointId)
            {
                case 7:
                    DoScriptText(SAY_STAY_1, me, player);
                    break;
                case 11:
                    DoScriptText(SAY_STAY_2, me, player);
                    break;
                case 25:
                    DoScriptText(SAY_STAY_3, me, player);
                    break;
                case 26:
                    DoScriptText(SAY_STAY_4, me, player);
                    break;
                case 27:
                    DoScriptText(SAY_STAY_5, me, player);
                    break;
                case 28:
                    DoScriptText(SAY_STAY_6, me, player);
                    me->SetStandState(UNIT_STAND_STATE_KNEEL);
                    break;
                case 29:
                    me->SetStandState(UNIT_STAND_STATE_STAND);
                    break;
                case 37:
                    DoScriptText(SAY_QUEST_COMPLETE, me, player);
                    me->SetSpeed(MOVE_RUN, 1.2f, true);
                    me->SetWalk(false);
                    if (player->GetQuestStatus(QUEST_STINKYS_ESCAPE_H))
                        player->GroupEventHappens(QUEST_STINKYS_ESCAPE_H, me);
                    if (player->GetQuestStatus(QUEST_STINKYS_ESCAPE_A))
                        player->GroupEventHappens(QUEST_STINKYS_ESCAPE_A, me);
                    break;
                case 39:
                    DoScriptText(EMOTE_DISAPPEAR, me);
                    break;
            }
        }

        void EnterCombat(Unit* who)
        {
            DoScriptText(SAY_ATTACKED_1, me, who);
        }

        void Reset() {}

        void JustDied(Unit* /*killer*/)
        {
            Player* player = GetPlayerForEscort();
            if (player && HasEscortState(STATE_ESCORT_ESCORTING))
            {
                if (player->GetQuestStatus(QUEST_STINKYS_ESCAPE_H))
                    player->FailQuest(QUEST_STINKYS_ESCAPE_H);

                if (player->GetQuestStatus(QUEST_STINKYS_ESCAPE_A))
                    player->FailQuest(QUEST_STINKYS_ESCAPE_A);
            }
        }

       void UpdateAI(const uint32 uiDiff)
        {
            npc_escortAI::UpdateAI(uiDiff);

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};


/// Theramore Prisoner - 23720
class npc_theramore_prisoner : public CreatureScript
{
public:
	npc_theramore_prisoner() : CreatureScript("npc_theramore_prisoner") { }

	enum eData
	{
		/// Text
		SAY_FREE = 0,

		/// Action
		SET_FREE = 1,
	};

	struct npc_theramore_prisonerAI : public ScriptedAI
	{
		npc_theramore_prisonerAI(Creature* p_Creature) : ScriptedAI(p_Creature) {}

		void Reset() override
		{
			ClearDelayedOperations();
		}


		void UpdateAI(const uint32 diff)
		{
			UpdateOperations(diff);
		}

		void DoAction(int32 const p_Action) override
		{
			switch (p_Action)
			{
			case SET_FREE:
			{
				me->AI()->Talk(SAY_FREE);

				AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
				{
					float x, y, z;
					me->GetClosePoint(x, y, z, me->GetObjectSize() / 3, 10.0f);
					me->GetMotionMaster()->MovePoint(1, x, y, z);
					me->DespawnOrUnsummon(7000);
				});
				break;
			}

			}
		}
	};

	CreatureAI* GetAI(Creature* p_Creature) const override
	{
		return new npc_theramore_prisonerAI(p_Creature);
	}

};

/// QUEST 27188: What's Haunting Witch Hill?
enum HauntingWitchHill
{
	/// Quest ID
	QUEST_WHATS_HAUNTING_WITCH_HILL = 27188,

	/// Spells
	SPELL_SUMMON_RESTLESS_APPARITION = 42511,
	SPELL_WITCH_HILL_INFORMATION_CREDIT = 42512,

	/// Risen Husk mechanics and ID
	SPELL_CONSUME_FLESH = 37933,
	NPC_RISEN_HUSK = 23555,

	/// Risen Spirit mechanics and ID
	SPELL_INTANGIBLE_PRESENCE = 43127,
	NPC_RISEN_SPIRIT = 23554,

	/// Events
	EVENT_CONSUME_FLESH = 0,
	EVENT_INTANGIBLE_PRESENCE = 1,
};

class mobs_risen_husk_spirit : public CreatureScript
{
public:
	mobs_risen_husk_spirit() : CreatureScript("mobs_risen_husk_spirit") { }

	struct mobs_risen_husk_spiritAI : public ScriptedAI
	{
		mobs_risen_husk_spiritAI(Creature* creature) : ScriptedAI(creature) { }

		void Reset()
		{
			events.Reset();
			if (me->GetEntry() == NPC_RISEN_HUSK)
				events.ScheduleEvent(EVENT_CONSUME_FLESH, 5000);
			else if (me->GetEntry() == NPC_RISEN_SPIRIT)
				events.ScheduleEvent(EVENT_INTANGIBLE_PRESENCE, 5000);
		}

		void JustDied(Unit* killer)
		{
			if (killer->GetTypeId() == TYPEID_PLAYER)
			{
				if (killer->ToPlayer()->GetQuestStatus(QUEST_WHATS_HAUNTING_WITCH_HILL) == QUEST_STATUS_INCOMPLETE)
				{
					DoCast(me, SPELL_SUMMON_RESTLESS_APPARITION, true);
					DoCast(killer, SPELL_WITCH_HILL_INFORMATION_CREDIT, true);
				}
			}
		}

		void UpdateAI(uint32 const diff)
		{
			if (!UpdateVictim())
				return;
			events.Update(diff);

			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_CONSUME_FLESH:
					DoCastVictim(SPELL_CONSUME_FLESH);
					events.ScheduleEvent(EVENT_CONSUME_FLESH, 15000);
					break;

				case EVENT_INTANGIBLE_PRESENCE:
					DoCastVictim(SPELL_INTANGIBLE_PRESENCE);
					events.ScheduleEvent(EVENT_INTANGIBLE_PRESENCE, 15000);
					break;

				default:
					break;
				}
			}

			DoMeleeAttackIfReady();
		}

	private:
		EventMap events;
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new mobs_risen_husk_spiritAI(creature);
	}
};


enum SpellScripts
{
    SPELL_OOZE_ZAP              = 42489,
    SPELL_OOZE_ZAP_CHANNEL_END  = 42485,
    SPELL_OOZE_CHANNEL_CREDIT   = 42486,
    SPELL_ENERGIZED             = 42492
};

class spell_ooze_zap: public SpellScriptLoader
{
    public:
        spell_ooze_zap() : SpellScriptLoader("spell_ooze_zap") { }

        class spell_ooze_zap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ooze_zap_SpellScript)

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_OOZE_ZAP))
                    return false;
                return true;
            }

            SpellCastResult CheckRequirement()
            {
                if (!GetCaster()->HasAura(GetSpellInfo()->Effects[EFFECT_1].CalcValue()))
                    return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW; // This is actually correct

                if (!GetExplTargetUnit())
                    return SPELL_FAILED_BAD_TARGETS;

                return SPELL_CAST_OK;
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (GetHitUnit())
                    GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ooze_zap_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
                OnCheckCast += SpellCheckCastFn(spell_ooze_zap_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ooze_zap_SpellScript();
        }
};

class spell_ooze_zap_channel_end: public SpellScriptLoader
{
    public:
        spell_ooze_zap_channel_end() : SpellScriptLoader("spell_ooze_zap_channel_end") { }

        class spell_ooze_zap_channel_end_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ooze_zap_channel_end_SpellScript)

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_OOZE_ZAP_CHANNEL_END))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (Player* player = GetCaster()->ToPlayer())
                    player->CastSpell(player, SPELL_OOZE_CHANNEL_CREDIT, true);
                GetHitUnit()->Kill(GetHitUnit());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_ooze_zap_channel_end_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ooze_zap_channel_end_SpellScript();
        }
};

class spell_energize_aoe: public SpellScriptLoader
{
    public:
        spell_energize_aoe() : SpellScriptLoader("spell_energize_aoe") { }

        class spell_energize_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_energize_aoe_SpellScript)

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ENERGIZED))
                    return false;
                return true;
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                for (std::list<WorldObject*>::iterator itr = targets.begin(); itr != targets.end();)
                {
                    if ((*itr)->IsPlayer() && (*itr)->ToPlayer()->GetQuestStatus(GetSpellInfo()->Effects[EFFECT_1].CalcValue()) == QUEST_STATUS_INCOMPLETE)
                        ++itr;
                    else
                        targets.erase(itr++);
                }
                targets.push_back(GetCaster());
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetCaster()->CastSpell(GetCaster(), uint32(GetEffectValue()), true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_energize_aoe_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_energize_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_energize_aoe_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_energize_aoe_SpellScript();
        }
};

/// Blackhoof Cage - 186287
enum eBackhoofCage
{
	/// Quest
	POTG_QUEST = 27245,

	/// Npc
	NPC_THERAMORE_PRISONER = 23720,

	/// Text
	SAY_FREE = 0,

	/// Action
	SET_FREE = 1,
};

class go_blackhoof_cage : public GameObjectScript
{
public:
    go_blackhoof_cage() : GameObjectScript("go_blackhoof_cage") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
		if (player->GetQuestStatus(POTG_QUEST) == QUEST_STATUS_INCOMPLETE)
			if (Creature* prisoner = go->FindNearestCreature(NPC_THERAMORE_PRISONER, 5.0f, true))
			{
				go->UseDoorOrButton();
				player->KilledMonsterCredit(NPC_THERAMORE_PRISONER, 0);

				prisoner->AI()->DoAction(SET_FREE);
			}
        return true;
    }
};


#ifndef __clang_analyzer__
void AddSC_dustwallow_marsh()
{
	/// Npcs
    new npc_stinky();
	new npc_theramore_prisoner();
	new mobs_risen_husk_spirit();

	/// Spells
    new spell_ooze_zap();
    new spell_ooze_zap_channel_end();
    new spell_energize_aoe();

	/// Object
    new go_blackhoof_cage();
}
#endif
