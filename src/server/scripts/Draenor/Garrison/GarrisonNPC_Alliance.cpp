////////////////////////////////////////////////////////////////////////////////
//
//  ArtifactCore based on TC 6.2.X
//  Copyright (C) 2012-2023 TrinityCore
//  All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "Common.h"
#include "Sites/GarrisonSiteBase.hpp"
#include "GarrisonScriptData.hpp"

namespace MS {
	namespace Garrison
	{
		/// Constructor
		npc_GarrisonCartRope::npc_GarrisonCartRope()
			: CreatureScript("npc_GarrisonCartRope")
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a CreatureAI object is needed for the creature.
		/// @p_Creature : Target creature instance
		CreatureAI* npc_GarrisonCartRope::GetAI(Creature* p_Creature) const
		{
			return new npc_GarrisonCartRopeAI(p_Creature);
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_GarrisonCartRope::npc_GarrisonCartRopeAI::npc_GarrisonCartRopeAI(Creature* p_Creature)
			: CreatureAI(p_Creature)
		{
			HasRope = false;
			me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// On reset
		void npc_GarrisonCartRope::npc_GarrisonCartRopeAI::Reset()
		{
			HasRope = false;
		}
		/// On AI Update
		/// @p_Diff : Time since last update
		void npc_GarrisonCartRope::npc_GarrisonCartRopeAI::UpdateAI(const uint32 /*p_Diff*/)
		{
			if (me->GetEntry() == NPCs::NpcGarrisonAllianceCart && !HasRope)
			{
				Creature* l_Horse = me->FindNearestCreature(NPCs::NpcGarrisonAllianceHorse, 2.5f);

				if (l_Horse)
				{
					me->CastSpell(l_Horse, Spells::SPELL_LEFT_ROPE, TRIGGERED_FULL_MASK);
					HasRope = true;
				}
			}
			else if (me->GetEntry() == NPCs::NpcGarrisonAllianceHorse && !HasRope)
			{
				Creature* l_Cart = me->FindNearestCreature(NPCs::NpcGarrisonAllianceCart, 4.0f);

				if (l_Cart && l_Cart->GetEntry() == NPCs::NpcGarrisonAllianceCart)
				{
					me->CastSpell(l_Cart, Spells::SPELL_RIGHT_ROPE, TRIGGERED_FULL_MASK);
					HasRope = true;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_AssistantBrightstone::npc_AssistantBrightstone()
			: CreatureScript("npc_AssistantBrightstone")
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a player opens a gossip dialog with the creature.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		bool npc_AssistantBrightstone::OnGossipHello(Player* p_Player, Creature* p_Creature)
		{
			if (p_Player->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER) && p_Player->GetQuestObjectiveCounter(273535) != 1)
			{
				p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::GarrisonCreation, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
				p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
			}
			else
			{
				p_Player->SEND_GOSSIP_MENU(RAND(NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_1, NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_2, NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_3), p_Creature->GetGUID());
			}

			return true;
		}
		/// Called when a player selects a gossip item in the creature's gossip menu.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		/// @p_Sender   : Sender menu
		/// @p_Action   : Action

		bool npc_AssistantBrightstone::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
		{
			if (p_Player->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER) && p_Player->GetQuestObjectiveCounter(273535) != 1)
			{
				p_Player->QuestObjectiveSatisfy(NPCs::NpcAssistantBrightstone, 1, QUEST_OBJECTIVE_TYPE_NPC_INTERACT, p_Creature->GetGUID());

				// @TODO move peon arround

				p_Player->CLOSE_GOSSIP_MENU();
				p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_VindicatorMaraad::npc_VindicatorMaraad()
			: CreatureScript("npc_VindicatorMaraad_GarrA")
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a player accepts a quest from the creature.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		/// @p_Quest    : Accepted quest
		bool npc_VindicatorMaraad::OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
		{
			if (p_Quest->GetQuestId() == Quests::QUEST_PALE_MOONLIGHT)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT);

			if (p_Quest->GetQuestId() == Quests::Alliance_QianaMoonshadow)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_QIANA_START_CHAT);

			return false;
		}
		/// Called when a player completes a quest with the creature.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		/// @p_Quest    : Completed quest

		bool npc_VindicatorMaraad::OnQuestComplete(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
		{
			if (p_Quest->GetQuestId() == Quests::QUEST_PALE_MOONLIGHT)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT);

			if (p_Quest->GetQuestId() == Quests::Alliance_BuildYourBarracks)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_BARRACKS_END_CHAT);

			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_ShellyHamby::npc_ShellyHamby()
			: CreatureScript("npc_ShellyHamby")
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a player opens a gossip dialog with the creature.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		bool npc_ShellyHamby::OnGossipHello(Player* p_Player, Creature* p_Creature)
		{
			if (!p_Player->HasItemCount(Items::ITEM_SHELLY_HAMBY_REPORT, 1))
				p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::ShellyQuestGossip2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

			p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_SHELLY_HAMBY_TEXT_QUEST, p_Creature->GetGUID());

			return true;
		}
		/// Called when a player selects a gossip item in the creature's gossip menu.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		/// @p_Sender   : Sender menu
		/// @p_Action   : Action

		bool npc_ShellyHamby::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
		{
			p_Player->CLOSE_GOSSIP_MENU();
			p_Creature->AI()->Talk(0);

			if (!p_Player->HasItemCount(Items::ITEM_SHELLY_HAMBY_REPORT, 1))
				p_Player->AddItem(Items::ITEM_SHELLY_HAMBY_REPORT, 1);

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_BarosAlexsom::npc_BarosAlexsom()
			: CreatureScript("npc_BarosAlexsom")
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a player accepts a quest from the creature.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		/// @p_Quest    : Accepted quest

		bool npc_BarosAlexsom::OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest)
		{
			if (p_Quest->GetQuestId() == Quests::QUEST_KEEPING_IT_TOGETHER)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_START_CHAT);

			if (p_Quest->GetQuestId() == Quests::QUEST_SHIP_SALVAGE)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_SHIP_SALVAGE_START_CHAT);

			if (p_Quest->GetQuestId() == Quests::Alliance_BuildYourBarracks)
				p_Player->SummonGameObject(231855, 1905.09f, 235.734f, 77.6232f, 2.93109f, 0.0f, 0.0f, 0.0f, 0.0f, 0);

			return false;
		}

		/// Called when a player completes a quest with the creature.
		/// @p_Player   : Source player instance
		/// @p_Creature : Target creature instance
		/// @p_Quest    : Completed quest

		bool npc_BarosAlexsom::OnQuestComplete(Player* /*p_Player*/, Creature* p_Creature, const Quest* p_Quest)
		{
			if (p_Quest->GetQuestId() == Quests::QUEST_ETABLISH_YOUR_GARRISON_A)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_ETABLISH_YOUR_GARRISON_CHAT);
			else if (p_Quest->GetQuestId() == Quests::QUEST_KEEPING_IT_TOGETHER)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_END_CHAT);
			else if (p_Quest->GetQuestId() == Quests::QUEST_SHIP_SALVAGE)
				p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_SHIP_SALVAGE_END_CHAT);

			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a CreatureAI object is needed for the creature.
		/// @p_Creature : Target creature instance
		CreatureAI* npc_BarosAlexsom::GetAI(Creature* p_Creature) const
		{
			return new npc_BarosAlexsomAI(p_Creature);
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_BarosAlexsom::npc_BarosAlexsomAI::npc_BarosAlexsomAI(Creature* p_Creature)
			: CreatureAI(p_Creature)
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

	 /// Called at waypoint reached or point movement finished
	 /// @p_Type : Movement Type
	 /// @p_ID   : Misc ID
		void npc_BarosAlexsom::npc_BarosAlexsomAI::MovementInform(uint32 p_Type, uint32 p_ID)
		{
			if (p_Type == POINT_MOTION_TYPE && p_ID == MovePointIDs::MOVE_POINT_BAROS_BARRACK_A)
			{
				if (Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript())
				{
					if (Player* l_Player = l_GarrisonSite->GetOwner())
					{
						l_Player->QuestObjectiveSatisfy(39422, 1, 14);
						l_Player->QuestObjectiveSatisfy(36163, 1, 14);
					}
				}

				m_DelayedOperations.push([this]() -> void
				{
					me->SetFacingTo(0.36f);
					me->GetMotionMaster()->MoveTargetedHome();
				});
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// On AI Update
		/// @p_Diff : Time since last update
		void npc_BarosAlexsom::npc_BarosAlexsomAI::UpdateAI(const uint32 /*p_Diff*/)
		{
			while (!m_DelayedOperations.empty())
			{
				m_DelayedOperations.front()();
				m_DelayedOperations.pop();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_LunarfallLaborer::npc_LunarfallLaborer()
			: CreatureScript("npc_LunarfallLaborer")
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Called when a CreatureAI object is needed for the creature.
		/// @p_Creature : Target creature instance
		CreatureAI* npc_LunarfallLaborer::GetAI(Creature* p_Creature) const
		{
			return new npc_LunarfallLaborerAI(p_Creature);
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// Constructor
		npc_LunarfallLaborer::npc_LunarfallLaborerAI::npc_LunarfallLaborerAI(Creature* p_Creature)
			: CreatureAI(p_Creature)
		{

		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////

		/// On AI Update
		/// @p_Diff : Time since last update
		void npc_LunarfallLaborer::npc_LunarfallLaborerAI::UpdateAI(const uint32 /*p_Diff*/)
		{

		}
		/// Set UInt32 value
		/// @p_ID    : Value ID
		/// @p_Value : Value
		void npc_LunarfallLaborer::npc_LunarfallLaborerAI::SetData(uint32 p_ID, uint32 /*p_Value*/)
		{
			if (p_ID == CreatureAIDataIDs::Builder)
			{
				me->LoadEquipment(1, true);
				me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
			}
		}

	}   ///< namespace Garrison
}   ///< namespace MS
#endif
