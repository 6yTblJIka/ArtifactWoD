////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Desolace
SD%Complete: 100
SDComment: Quest support: 5561, 5581
SDCategory: Desolace
EndScriptData */

/* ContentData
npc_aged_dying_ancient_kodo
go_demon_portal
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"

enum DyingKodo
{
    // signed for 9999
    SAY_SMEED_HOME_1                = -1000348,
    SAY_SMEED_HOME_2                = -1000349,
    SAY_SMEED_HOME_3                = -1000350,

    QUEST_KODO                      = 5561,

    NPC_SMEED                       = 11596,
    NPC_AGED_KODO                   = 4700,
    NPC_DYING_KODO                  = 4701,
    NPC_ANCIENT_KODO                = 4702,
    NPC_TAMED_KODO                  = 11627,

    SPELL_KODO_KOMBO_ITEM           = 18153,
    SPELL_KODO_KOMBO_PLAYER_BUFF    = 18172,                //spells here have unclear function, but using them at least for visual parts and checks
    SPELL_KODO_KOMBO_DESPAWN_BUFF   = 18377,
    SPELL_KODO_KOMBO_GOSSIP         = 18362

};

class npc_aged_dying_ancient_kodo : public CreatureScript
{
public:
    npc_aged_dying_ancient_kodo() : CreatureScript("npc_aged_dying_ancient_kodo") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->HasAura(SPELL_KODO_KOMBO_PLAYER_BUFF) && creature->HasAura(SPELL_KODO_KOMBO_DESPAWN_BUFF))
        {
            //the expected quest objective
            player->TalkedToCreature(creature->GetEntry(), creature->GetGUID());

            player->RemoveAurasDueToSpell(SPELL_KODO_KOMBO_PLAYER_BUFF);
            creature->GetMotionMaster()->MoveIdle();
        }

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
        return true;
    }

    bool EffectDummyCreature(Unit* pCaster, uint32 spellId, uint32 effIndex, Creature* creatureTarget)
    {
        //always check spellid and effectindex
        if (spellId == SPELL_KODO_KOMBO_ITEM && effIndex == 0)
        {
            //no effect if player/creature already have aura from spells
            if (pCaster->HasAura(SPELL_KODO_KOMBO_PLAYER_BUFF) || creatureTarget->HasAura(SPELL_KODO_KOMBO_DESPAWN_BUFF))
                return true;

            if (creatureTarget->GetEntry() == NPC_AGED_KODO ||
                creatureTarget->GetEntry() == NPC_DYING_KODO ||
                creatureTarget->GetEntry() == NPC_ANCIENT_KODO)
            {
                pCaster->CastSpell(pCaster, SPELL_KODO_KOMBO_PLAYER_BUFF, true);

                creatureTarget->CastSpell(creatureTarget, SPELL_KODO_KOMBO_DESPAWN_BUFF, false);

                if (creatureTarget->GetMotionMaster()->GetCurrentMovementGeneratorType() == WAYPOINT_MOTION_TYPE)
                    creatureTarget->GetMotionMaster()->MoveIdle();

                creatureTarget->GetMotionMaster()->MoveFollow(pCaster, PET_FOLLOW_DIST,  creatureTarget->GetFollowAngle());
            }

            //always return true when we are handling this spell and effect
            return true;
        }
        return false;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_aged_dying_ancient_kodoAI(creature);
    }

    struct npc_aged_dying_ancient_kodoAI : public ScriptedAI
    {
        npc_aged_dying_ancient_kodoAI(Creature* creature) : ScriptedAI(creature) { Reset(); }

        uint32 DespawnTimer;

        void Reset()
        {
            DespawnTimer = 0;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetEntry() == NPC_SMEED)
            {
                if (me->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                    return;

                if (me->IsWithinDistInMap(who, 10.0f))
                {
					me->UpdateEntry(NPC_TAMED_KODO); // supposed to award kill credit to quest
                    DoScriptText(RAND(SAY_SMEED_HOME_1, SAY_SMEED_HOME_2, SAY_SMEED_HOME_3), who);

                    //spell have no implemented effect (dummy), so useful to notify spellHit
                    DoCast(me, SPELL_KODO_KOMBO_GOSSIP, true);
                }
            }
        }

        void SpellHit(Unit* /*pCaster*/, SpellInfo const* pSpell)
        {
            if (pSpell->Id == SPELL_KODO_KOMBO_GOSSIP)
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                DespawnTimer = 60000;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //timer should always be == 0 unless we already updated entry of creature. Then not expect this updated to ever be in combat.
            if (DespawnTimer && DespawnTimer <= diff)
            {
                if (!me->getVictim() && me->isAlive())
                {
                    Reset();
                    me->setDeathState(JUST_DIED);
                    me->Respawn();
                    return;
                }
            } else DespawnTimer -= diff;

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };

};

/*######
## go_demon_portal
######*/

enum DemonPortal
{
    NPC_DEMON_GUARDIAN          = 11937,

    QUEST_PORTAL_OF_THE_LEGION  = 5581
};

class go_demon_portal : public GameObjectScript
{
    public:
        go_demon_portal() : GameObjectScript("go_demon_portal") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (player->GetQuestStatus(QUEST_PORTAL_OF_THE_LEGION) == QUEST_STATUS_INCOMPLETE && !go->FindNearestCreature(NPC_DEMON_GUARDIAN, 5.0f, true))
            {
                if (Creature* guardian = player->SummonCreature(NPC_DEMON_GUARDIAN, go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0))
                    guardian->AI()->AttackStart(player);
            }

            return true;
        }
};

class npc_dalinda_malem : public CreatureScript
{
    public:
        npc_dalinda_malem() : CreatureScript("npc_dalinda_malem")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_dalinda_malemAI(creature);
        }

        struct npc_dalinda_malemAI : public ScriptedAI
        {
            npc_dalinda_malemAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_desolace()
{
    new npc_dalinda_malem();
    new npc_aged_dying_ancient_kodo();
    new go_demon_portal();
}
#endif
