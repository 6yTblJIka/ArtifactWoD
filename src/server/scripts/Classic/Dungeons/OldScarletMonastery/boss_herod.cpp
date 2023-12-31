////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Herod
SD%Complete: 95
SDComment: Should in addition spawn Myrmidons in the hallway outside
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"

enum Says
{
    SAY_AGGRO                   = -1189000,
    SAY_WHIRLWIND               = -1189001,
    SAY_ENRAGE                  = -1189002,
    SAY_KILL                    = -1189003
};

enum Emotes
{
    EMOTE_ENRAGE                = -1189004
};

enum Spells
{
    SPELL_RUSHINGCHARGE         = 8260,
    SPELL_CLEAVE                = 15496,
    SPELL_WHIRLWIND             = 8989,
    SPELL_FRENZY                = 8269
};

enum Entry
{
    ENTRY_SCARLET_TRAINEE       = 6575,
    ENTRY_SCARLET_MYRMIDON      = 4295
};

class boss_herod : public CreatureScript
{
public:
    boss_herod() : CreatureScript("boss_herod") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_herodAI(creature);
    }

    struct boss_herodAI : public ScriptedAI
    {
        boss_herodAI(Creature* creature) : ScriptedAI(creature) {}

        bool Enrage;

        uint32 Cleave_Timer;
        uint32 Whirlwind_Timer;

        void Reset()
        {
            Enrage = false;
            Cleave_Timer = 12000;
            Whirlwind_Timer = 60000;
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
            DoCast(me, SPELL_RUSHINGCHARGE);
        }

         void KilledUnit(Unit* /*victim*/)
         {
             DoScriptText(SAY_KILL, me);
         }

         void JustDied(Unit* /*killer*/)
         {
             for (uint8 i = 0; i < 20; ++i)
                 me->SummonCreature(ENTRY_SCARLET_TRAINEE, 1939.18f, -431.58f, 17.09f, 6.22f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
         }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //If we are <30% hp goes Enraged
            if (!Enrage && !HealthAbovePct(30) && !me->IsNonMeleeSpellCasted(false))
            {
                DoScriptText(EMOTE_ENRAGE, me);
                DoScriptText(SAY_ENRAGE, me);
                DoCast(me, SPELL_FRENZY);
                Enrage = true;
            }

            //Cleave_Timer
            if (Cleave_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CLEAVE);
                Cleave_Timer = 12000;
            }
            else Cleave_Timer -= diff;

            // Whirlwind_Timer
            if (Whirlwind_Timer <= diff)
            {
                DoScriptText(SAY_WHIRLWIND, me);
                DoCast(me->getVictim(), SPELL_WHIRLWIND);
                Whirlwind_Timer = 30000;
            }
            else Whirlwind_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

class mob_scarlet_trainee : public CreatureScript
{
public:
    mob_scarlet_trainee() : CreatureScript("mob_scarlet_trainee") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_scarlet_traineeAI(creature);
    }

    struct mob_scarlet_traineeAI : public npc_escortAI
    {
        mob_scarlet_traineeAI(Creature* creature) : npc_escortAI(creature)
        {
            Start_Timer = urand(1000, 6000);
        }

        uint32 Start_Timer;

        void Reset() {}

        void WaypointReached(uint32 /*waypointId*/)
        {

        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (Start_Timer)
            {
                if (Start_Timer <= diff)
                {
                    Start(true, true);
                    Start_Timer = 0;
                } else Start_Timer -= diff;
            }

            npc_escortAI::UpdateAI(diff);
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_herod()
{
    new boss_herod();
    new mob_scarlet_trainee();
}
#endif
