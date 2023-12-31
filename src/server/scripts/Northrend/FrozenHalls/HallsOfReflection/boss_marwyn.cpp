////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_reflection.h"

enum Texts
{
    SAY_AGGRO                                     = 0,
    SAY_SLAY                                      = 1,
    SAY_DEATH                                     = 2,
    SAY_CORRUPTED_FLESH                           = 3
};

enum Spells
{
    SPELL_OBLITERATE                              = 72360,
    SPELL_WELL_OF_CORRUPTION                      = 72362,
    SPELL_CORRUPTED_FLESH                         = 72363,
    SPELL_SHARED_SUFFERING                        = 72368
};

enum Events
{
    EVENT_NONE,
    EVENT_OBLITERATE,
    EVENT_WELL_OF_CORRUPTION,
    EVENT_CORRUPTED_FLESH,
    EVENT_SHARED_SUFFERING
};

class boss_marwyn : public CreatureScript
{
public:
    boss_marwyn() : CreatureScript("boss_marwyn") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_marwynAI(creature);
    }

    struct boss_marwynAI : public boss_horAI
    {
        boss_marwynAI(Creature* creature) : boss_horAI(creature) {}

        void Reset()
        {
            boss_horAI::Reset();

            if (instance)
                instance->SetBossState(DATA_MARWYN_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            Talk(SAY_AGGRO);
            if (instance)
                instance->SetBossState(DATA_MARWYN_EVENT, IN_PROGRESS);

            events.ScheduleEvent(EVENT_OBLITERATE, urand(2000,3000));          /// @todo Check timer
            events.ScheduleEvent(EVENT_WELL_OF_CORRUPTION, 5000);
            events.ScheduleEvent(EVENT_CORRUPTED_FLESH, 20000);
            events.ScheduleEvent(EVENT_SHARED_SUFFERING, 20000);    /// @todo Check timer
        }

        void JustDied(Unit* /*killer*/)
        {
            Talk(SAY_DEATH);

            if (instance)
                instance->SetBossState(DATA_MARWYN_EVENT, DONE);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            Talk(SAY_SLAY);
        }

        void UpdateAI(uint32 const diff)
        {
            // Return since we have no target
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case EVENT_OBLITERATE:
                    DoCast(SPELL_OBLITERATE);
                    events.ScheduleEvent(EVENT_OBLITERATE, urand(13000,15000));
                    break;
                case EVENT_WELL_OF_CORRUPTION:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_WELL_OF_CORRUPTION);
                    events.ScheduleEvent(EVENT_WELL_OF_CORRUPTION, urand(13000,15000));
                    break;
                case EVENT_CORRUPTED_FLESH:
                    Talk(SAY_CORRUPTED_FLESH);
                    DoCast(SPELL_CORRUPTED_FLESH);
                    events.ScheduleEvent(EVENT_CORRUPTED_FLESH, 20000);
                    break;
                case EVENT_SHARED_SUFFERING:
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        DoCast(target, SPELL_SHARED_SUFFERING);
                    events.ScheduleEvent(EVENT_SHARED_SUFFERING, 20000);
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_marwyn()
{
    new boss_marwyn();
}
#endif
