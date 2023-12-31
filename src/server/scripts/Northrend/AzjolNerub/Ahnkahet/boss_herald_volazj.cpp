////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/*
 * Comment: Missing AI for Twisted Visages
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ahnkahet.h"

enum Spells
{
    SPELL_INSANITY                                = 57496, //Dummy
    INSANITY_VISUAL                               = 57561,
    SPELL_INSANITY_TARGET                         = 57508,
    SPELL_MIND_FLAY                               = 57941,
    SPELL_SHADOW_BOLT_VOLLEY                      = 57942,
    SPELL_SHIVER                                  = 57949,
    SPELL_CLONE_PLAYER                            = 57507, //casted on player during insanity
    SPELL_INSANITY_PHASING_1                      = 57508,
    SPELL_INSANITY_PHASING_2                      = 57509,
    SPELL_INSANITY_PHASING_3                      = 57510,
    SPELL_INSANITY_PHASING_4                      = 57511,
    SPELL_INSANITY_PHASING_5                      = 57512
};

enum Creatures
{
    MOB_TWISTED_VISAGE                            = 30625
};


enum Yells
{
    SAY_AGGRO   = 0,
    SAY_SLAY    = 1,
    SAY_DEATH   = 2,
    SAY_PHASE   = 3
};

enum Achievements
{
    ACHIEV_QUICK_DEMISE_START_EVENT               = 20382
};

class boss_volazj : public CreatureScript
{
public:
    boss_volazj() : CreatureScript("boss_volazj") { }

    struct boss_volazjAI : public ScriptedAI
    {
        boss_volazjAI(Creature* creature) : ScriptedAI(creature), Summons(me)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 uiMindFlayTimer;
        uint32 uiShadowBoltVolleyTimer;
        uint32 uiShiverTimer;
        uint32 insanityHandled;
        SummonList Summons;

        // returns the percentage of health after taking the given damage.
        uint32 GetHealthPct(uint32 damage)
        {
            if (damage > me->GetHealth())
                return 0;
            return 100*(me->GetHealth()-damage)/me->GetMaxHealth();
        }

        void DamageTaken(Unit* /*pAttacker*/, uint32 &damage, SpellInfo const*  /*p_SpellInfo*/)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                damage = 0;

            if ((GetHealthPct(0) >= 66 && GetHealthPct(damage) < 66)||
                (GetHealthPct(0) >= 33 && GetHealthPct(damage) < 33))
            {
                me->InterruptNonMeleeSpells(false);
                DoCast(me, SPELL_INSANITY, false);
            }
        }

        void SpellHitTarget(Unit* target, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_INSANITY)
            {
                // Not good target or too many players
                if (target->GetTypeId() != TYPEID_PLAYER || insanityHandled > 4)
                    return;
                // First target - start channel visual and set self as unnattackable
                if (!insanityHandled)
                {
                    // Channel visual
                    DoCast(me, INSANITY_VISUAL, true);
                    // Unattackable
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetControlled(true, UNIT_STATE_STUNNED);
                }
                // phase mask
                target->CastSpell(target, SPELL_INSANITY_TARGET+insanityHandled, true);
                // summon twisted party members for this target
                Map::PlayerList const &players = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                {
                    Player* player = i->getSource();
                    if (!player || !player->isAlive())
                        continue;
                    // Summon clone
                    if (Unit* summon = me->SummonCreature(MOB_TWISTED_VISAGE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_CORPSE_DESPAWN, 0))
                    {
                        // clone
                        player->CastSpell(summon, SPELL_CLONE_PLAYER, true);
                        // set phase
                        summon->SetPhaseMask((1<<(4+insanityHandled)), true);
                    }
                }
                ++insanityHandled;
            }
        }

        void ResetPlayersPhaseMask()
        {
            Map::PlayerList const &players = me->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
            {
                Player* player = i->getSource();
                player->RemoveAurasDueToSpell(GetSpellForPhaseMask(player->GetPhaseMask()));
            }
        }

        void Reset()
        {
            uiMindFlayTimer = 8*IN_MILLISECONDS;
            uiShadowBoltVolleyTimer = 5*IN_MILLISECONDS;
            uiShiverTimer = 15*IN_MILLISECONDS;

            if (instance)
            {
                instance->SetData(DATA_HERALD_VOLAZJ, NOT_STARTED);
                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_QUICK_DEMISE_START_EVENT);
            }

            // Visible for all players in insanity
            me->SetPhaseMask((1|16|32|64|128|256), true);
            // Used for Insanity handling
            insanityHandled = 0;

            ResetPlayersPhaseMask();

            // Cleanup
            Summons.DespawnAll();
            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetControlled(false, UNIT_STATE_STUNNED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);

            if (instance)
            {
                instance->SetData(DATA_HERALD_VOLAZJ, IN_PROGRESS);
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_QUICK_DEMISE_START_EVENT);
            }
        }

        void JustSummoned(Creature* summon)
        {
            Summons.Summon(summon);
        }

        uint32 GetSpellForPhaseMask(uint32 phase)
        {
            uint32 spell = 0;
            switch (phase)
            {
                case 16:
                    spell = SPELL_INSANITY_PHASING_1;
                    break;
                case 32:
                    spell = SPELL_INSANITY_PHASING_2;
                    break;
                case 64:
                    spell = SPELL_INSANITY_PHASING_3;
                    break;
                case 128:
                    spell = SPELL_INSANITY_PHASING_4;
                    break;
                case 256:
                    spell = SPELL_INSANITY_PHASING_5;
                    break;
            }
            return spell;
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            uint32 phase = summon->GetPhaseMask();
            uint32 nextPhase = 0;
            Summons.Despawn(summon);

            // Check if all summons in this phase killed
            for (SummonList::const_iterator iter = Summons.begin(); iter != Summons.end(); ++iter)
            {
                if (Creature* visage = Unit::GetCreature(*me, *iter))
                {
                    // Not all are dead
                    if (phase == visage->GetPhaseMask())
                        return;
                    else
                        nextPhase = visage->GetPhaseMask();
                }
            }

            // Roll Insanity
            uint32 spell = GetSpellForPhaseMask(phase);
            uint32 spell2 = GetSpellForPhaseMask(nextPhase);
            Map* map = me->GetMap();
            if (!map)
                return;

            Map::PlayerList const &PlayerList = map->GetPlayers();
            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (Player* player = i->getSource())
                    {
                        if (player->HasAura(spell))
                        {
                            player->RemoveAurasDueToSpell(spell);
                            if (spell2) // if there is still some different mask cast spell for it
                                player->CastSpell(player, spell2, true);
                        }
                    }
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (insanityHandled)
            {
                if (!Summons.empty())
                    return;

                insanityHandled = 0;
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->SetControlled(false, UNIT_STATE_STUNNED);
                me->RemoveAurasDueToSpell(INSANITY_VISUAL);
            }

            if (uiMindFlayTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MIND_FLAY);
                uiMindFlayTimer = 20*IN_MILLISECONDS;
            } else uiMindFlayTimer -= diff;

            if (uiShadowBoltVolleyTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SHADOW_BOLT_VOLLEY);
                uiShadowBoltVolleyTimer = 5*IN_MILLISECONDS;
            } else uiShadowBoltVolleyTimer -= diff;

            if (uiShiverTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SHIVER);
                uiShiverTimer = 15*IN_MILLISECONDS;
            } else uiShiverTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetData(DATA_HERALD_VOLAZJ, DONE);

            Summons.DespawnAll();
            ResetPlayersPhaseMask();
        }

        void KilledUnit(Unit* /*victim*/)
        {
            Talk(SAY_SLAY);
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_volazjAI(creature);
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_volazj()
{
    new boss_volazj;
}
#endif
