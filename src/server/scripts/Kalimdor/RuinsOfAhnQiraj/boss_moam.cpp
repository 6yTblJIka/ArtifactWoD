////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ruins_of_ahnqiraj.h"

enum Texts
{
    EMOTE_AGGRO             = -1509000,
    EMOTE_MANA_FULL         = -1509001
};

enum Spells
{
    SPELL_TRAMPLE               = 15550,
    SPELL_DRAIN_MANA            = 25671,
    SPELL_ARCANE_ERUPTION       = 25672,
    SPELL_SUMMON_MANA_FIEND_1   = 25681, // TARGET_DEST_CASTER_FRONT
    SPELL_SUMMON_MANA_FIEND_2   = 25682, // TARGET_DEST_CASTER_LEFT
    SPELL_SUMMON_MANA_FIEND_3   = 25683, // TARGET_DEST_CASTER_RIGHT
    SPELL_ENERGIZE              = 25685
};

enum Events
{
    EVENT_TRAMPLE           = 1,
    EVENT_DRAIN_MANA        = 2,
    EVENT_STONE_PHASE       = 3,
    EVENT_STONE_PHASE_END   = 4,
    EVENT_WIDE_SLASH        = 5
};

enum Actions
{
    ACTION_STONE_PHASE_START = 1,
    ACTION_STONE_PHASE_END   = 2
};

class boss_moam : public CreatureScript
{
    public:
        boss_moam() : CreatureScript("boss_moam") { }

        struct boss_moamAI : public BossAI
        {
            boss_moamAI(Creature* creature) : BossAI(creature, BOSS_MOAM)
            {
            }

            void Reset()
            {
                _Reset();
                me->SetPower(POWER_MANA, 0);
                _isStonePhase = false;
                events.ScheduleEvent(EVENT_STONE_PHASE, 90000);
                //events.ScheduleEvent(EVENT_WIDE_SLASH, 11000);
            }

            void DamageTaken(Unit* /*attacker*/, uint32& /*damage*/, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!_isStonePhase && HealthBelowPct(45))
                {
                    _isStonePhase = true;
                    DoAction(ACTION_STONE_PHASE_START);
                }
            }

            void DoAction(int32 const action)
            {
                switch (action)
                {
                    case ACTION_STONE_PHASE_END:
                    {
                        me->RemoveAurasDueToSpell(SPELL_ENERGIZE);
                        events.ScheduleEvent(EVENT_STONE_PHASE, 90000);
                        _isStonePhase = false;
                        break;
                    }
                    case ACTION_STONE_PHASE_START:
                    {
                        DoCast(me, SPELL_SUMMON_MANA_FIEND_1);
                        DoCast(me, SPELL_SUMMON_MANA_FIEND_2);
                        DoCast(me, SPELL_SUMMON_MANA_FIEND_3);
                        DoCast(me, SPELL_ENERGIZE);
                        events.ScheduleEvent(EVENT_STONE_PHASE_END, 90000);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->GetPower(POWER_MANA) == me->GetMaxPower(POWER_MANA))
                {
                    if (_isStonePhase)
                        DoAction(ACTION_STONE_PHASE_END);
                    DoCastAOE(SPELL_ARCANE_ERUPTION);
                    me->SetPower(POWER_MANA, 0);
                }

                if (_isStonePhase)
                {
                    if (events.ExecuteEvent() == EVENT_STONE_PHASE_END)
                        DoAction(ACTION_STONE_PHASE_END);
                    return;
                }

                // Messing up mana-drain channel
                //if (me->HasUnitState(UNIT_STATE_CASTING))
                //    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_STONE_PHASE:
                            DoAction(ACTION_STONE_PHASE_START);
                            break;
                        case EVENT_DRAIN_MANA:
                        {
                            std::list<Unit*> targetList;
                            {
                                const std::list<HostileReference*>& threatlist = me->getThreatManager().getThreatList();
                                for (std::list<HostileReference*>::const_iterator itr = threatlist.begin(); itr != threatlist.end(); ++itr)
                                    if ((*itr)->getTarget()->IsPlayer() && (*itr)->getTarget()->getPowerType() == POWER_MANA)
                                        targetList.push_back((*itr)->getTarget());
                            }

                            JadeCore::Containers::RandomResizeList(targetList, 5);

                            for (std::list<Unit*>::iterator itr = targetList.begin(); itr != targetList.end(); ++itr)
                                DoCast(*itr, SPELL_DRAIN_MANA);

                            events.ScheduleEvent(EVENT_DRAIN_MANA, urand(5000, 15000));
                            break;
                        }/*
                        case EVENT_WIDE_SLASH:
                            DoCast(me, SPELL_WIDE_SLASH);
                            events.ScheduleEvent(EVENT_WIDE_SLASH, 11000);
                            break;
                        case EVENT_TRASH:
                            DoCast(me, SPELL_TRASH);
                            events.ScheduleEvent(EVENT_WIDE_SLASH, 16000);
                            break;*/
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        private:
            bool _isStonePhase;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_moamAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_moam()
{
    new boss_moam();
}
#endif
