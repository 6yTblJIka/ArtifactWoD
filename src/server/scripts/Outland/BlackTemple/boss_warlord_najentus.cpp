////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Warlord_Najentus
SD%Complete: 95
SDComment:
SDCategory: Black Temple
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "black_temple.h"

enum eEnums
{
    SAY_AGGRO                       = -1564000,
    SAY_NEEDLE1                     = -1564001,
    SAY_NEEDLE2                     = -1564002,
    SAY_SLAY1                       = -1564003,
    SAY_SLAY2                       = -1564004,
    SAY_SPECIAL1                    = -1564005,
    SAY_SPECIAL2                    = -1564006,
    SAY_ENRAGE1                     = -1564007,           //is this text actually in use?
    SAY_ENRAGE2                     = -1564008,
    SAY_DEATH                       = -1564009,

    //Spells
    SPELL_NEEDLE_SPINE              = 39992,
    SPELL_TIDAL_BURST               = 39878,
    SPELL_TIDAL_SHIELD              = 39872,
    SPELL_IMPALING_SPINE            = 39837,
    SPELL_CREATE_NAJENTUS_SPINE     = 39956,
    SPELL_HURL_SPINE                = 39948,
    SPELL_BERSERK                   = 26662,

    GOBJECT_SPINE                   = 185584,

    EVENT_BERSERK                   = 1,
    EVENT_YELL                      = 2,
    EVENT_NEEDLE                    = 3,
    EVENT_SPINE                     = 4,
    EVENT_SHIELD                    = 5,

    GCD_CAST                        = 1,
    GCD_YELL                        = 2
};

class boss_najentus : public CreatureScript
{
public:
    boss_najentus() : CreatureScript("boss_najentus") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_najentusAI (creature);
    }

    struct boss_najentusAI : public ScriptedAI
    {
        boss_najentusAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;
        EventMap events;

        uint64 SpineTargetGUID;

        void Reset()
        {
            events.Reset();

            SpineTargetGUID = 0;

            if (instance)
                instance->SetData(DATA_HIGH_WARLORD_NAJENTUS_EVENT, NOT_STARTED);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, me);
            events.DelayEvents(5000, GCD_YELL);
        }

        void JustDied(Unit* /*killer*/)
        {
            if (instance)
                instance->SetData(DATA_HIGH_WARLORD_NAJENTUS_EVENT, DONE);

            DoScriptText(SAY_DEATH, me);
        }

        void SpellHit(Unit* /*caster*/, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_HURL_SPINE && me->HasAura(SPELL_TIDAL_SHIELD))
            {
                me->RemoveAurasDueToSpell(SPELL_TIDAL_SHIELD);
                DoCast(me, SPELL_TIDAL_BURST, true);
                ResetTimer();
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
                instance->SetData(DATA_HIGH_WARLORD_NAJENTUS_EVENT, IN_PROGRESS);

            DoScriptText(SAY_AGGRO, me);
            DoZoneInCombat();
            events.ScheduleEvent(EVENT_BERSERK, 480000, GCD_CAST);
            events.ScheduleEvent(EVENT_YELL, 45000 + (rand()%76)*1000, GCD_YELL);
            ResetTimer();
        }

        bool RemoveImpalingSpine()
        {
            if (!SpineTargetGUID)
                return false;

            Unit* target = Unit::GetUnit(*me, SpineTargetGUID);
            if (target && target->HasAura(SPELL_IMPALING_SPINE))
                target->RemoveAurasDueToSpell(SPELL_IMPALING_SPINE);
            SpineTargetGUID=0;
            return true;
        }

        void ResetTimer(uint32 inc = 0)
        {
            events.RescheduleEvent(EVENT_NEEDLE, 10000 + inc, GCD_CAST);
            events.RescheduleEvent(EVENT_SPINE, 20000 + inc, GCD_CAST);
            events.RescheduleEvent(EVENT_SHIELD, 60000 + inc);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_SHIELD:
                        DoCast(me, SPELL_TIDAL_SHIELD, true);
                        ResetTimer(45000);
                        break;
                    case EVENT_BERSERK:
                        DoScriptText(SAY_ENRAGE2, me);
                        DoCast(me, SPELL_BERSERK, true);
                        events.DelayEvents(15000, GCD_YELL);
                        break;
                    case EVENT_SPINE:
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1);
                        if (!target) target = me->getVictim();
                        if (target)
                        {
                            DoCast(target, SPELL_IMPALING_SPINE, true);
                            SpineTargetGUID = target->GetGUID();
                            //must let target summon, otherwise you cannot click the spine
                            target->SummonGameObject(GOBJECT_SPINE, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), me->GetOrientation(), 0, 0, 0, 0, 30);
                            DoScriptText(urand(0, 1) ? SAY_NEEDLE1 : SAY_NEEDLE2, me);
                            events.DelayEvents(1500, GCD_CAST);
                            events.DelayEvents(15000, GCD_YELL);
                        }
                        events.ScheduleEvent(EVENT_SPINE, 21000, GCD_CAST);
                        return;
                    }
                    case EVENT_NEEDLE:
                    {
                        //DoCast(me, SPELL_NEEDLE_SPINE, true);
                        std::list<Unit*> targets;
                        SelectTargetList(targets, 3, SELECT_TARGET_RANDOM, 80, true);
                        for (std::list<Unit*>::const_iterator i = targets.begin(); i != targets.end(); ++i)
                            DoCast(*i, 39835, true);
                        events.ScheduleEvent(EVENT_NEEDLE, urand(15000, 25000), GCD_CAST);
                        events.DelayEvents(1500, GCD_CAST);
                        return;
                    }
                    case EVENT_YELL:
                        DoScriptText(RAND(SAY_SPECIAL1, SAY_SPECIAL2), me);
                        events.ScheduleEvent(EVENT_YELL, urand(25000, 100000), GCD_YELL);
                        events.DelayEvents(15000, GCD_YELL);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

class go_najentus_spine : public GameObjectScript
{
public:
    go_najentus_spine() : GameObjectScript("go_najentus_spine") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (InstanceScript* instance = go->GetInstanceScript())
            if (Creature* Najentus = Unit::GetCreature(*go, instance->GetData64(DATA_HIGH_WARLORD_NAJENTUS_EVENT)))
                if (CAST_AI(boss_najentus::boss_najentusAI, Najentus->AI())->RemoveImpalingSpine())
                {
                    player->CastSpell(player, SPELL_CREATE_NAJENTUS_SPINE, true);
                    go->Delete();
                }
        return true;
    }

};

#ifndef __clang_analyzer__
void AddSC_boss_najentus()
{
    new boss_najentus();
    new go_najentus_spine();
}
#endif
