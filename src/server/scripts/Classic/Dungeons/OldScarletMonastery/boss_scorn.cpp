////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Scorn
SD%Complete: 100
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_LICHSLAP                  = 28873,
    SPELL_FROSTBOLTVOLLEY           = 8398,
    SPELL_MINDFLAY                  = 17313,
    SPELL_FROSTNOVA                 = 15531
};

class boss_scorn : public CreatureScript
{
public:
    boss_scorn() : CreatureScript("boss_scorn") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_scornAI (creature);
    }

    struct boss_scornAI : public ScriptedAI
    {
        boss_scornAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 LichSlap_Timer;
        uint32 FrostboltVolley_Timer;
        uint32 MindFlay_Timer;
        uint32 FrostNova_Timer;

        void Reset()
        {
            LichSlap_Timer = 45000;
            FrostboltVolley_Timer = 30000;
            MindFlay_Timer = 30000;
            FrostNova_Timer = 30000;
        }

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //LichSlap_Timer
            if (LichSlap_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_LICHSLAP);
                LichSlap_Timer = 45000;
            }
            else LichSlap_Timer -= diff;

            //FrostboltVolley_Timer
            if (FrostboltVolley_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROSTBOLTVOLLEY);
                FrostboltVolley_Timer = 20000;
            }
            else FrostboltVolley_Timer -= diff;

            //MindFlay_Timer
            if (MindFlay_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MINDFLAY);
                MindFlay_Timer = 20000;
            }
            else MindFlay_Timer -= diff;

            //FrostNova_Timer
            if (FrostNova_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROSTNOVA);
                FrostNova_Timer = 15000;
            }
            else FrostNova_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_scorn()
{
    new boss_scorn();
}
#endif
