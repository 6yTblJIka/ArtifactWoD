////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* Script Data Start
SDName: Boss salramm
SDAuthor: Tartalo
SD%Complete: 80
SDComment: TODO: Intro
SDCategory:
Script Data End */

#include "ScriptPCH.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CURSE_OF_TWISTED_FLESH                = 58845,
    SPELL_EXPLODE_GHOUL                         = 52480,
    H_SPELL_EXPLODE_GHOUL                       = 58825,
    SPELL_SHADOW_BOLT                           = 57725,
    H_SPELL_SHADOW_BOLT                         = 58828,
    SPELL_STEAL_FLESH                           = 52708,
    SPELL_SUMMON_GHOULS                         = 52451
};

enum Yells
{
    SAY_AGGRO                                   = -1595032,
    SAY_SPAWN                                   = -1595033,
    SAY_SLAY_1                                  = -1595034,
    SAY_SLAY_2                                  = -1595035,
    SAY_SLAY_3                                  = -1595036,
    SAY_DEATH                                   = -1595037,
    SAY_EXPLODE_GHOUL_1                         = -1595038,
    SAY_EXPLODE_GHOUL_2                         = -1595039,
    SAY_STEAL_FLESH_1                           = -1595040,
    SAY_STEAL_FLESH_2                           = -1595041,
    SAY_STEAL_FLESH_3                           = -1595042,
    SAY_SUMMON_GHOULS_1                         = -1595043,
    SAY_SUMMON_GHOULS_2                         = -1595044
};

class boss_salramm : public CreatureScript
{
public:
    boss_salramm() : CreatureScript("boss_salramm") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_salrammAI (creature);
    }

    struct boss_salrammAI : public ScriptedAI
    {
        boss_salrammAI(Creature* c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
            if (instance)
                DoScriptText(SAY_SPAWN, me);
        }

        uint32 uiCurseFleshTimer;
        uint32 uiExplodeGhoulTimer;
        uint32 uiShadowBoltTimer;
        uint32 uiStealFleshTimer;
        uint32 uiSummonGhoulsTimer;
        uint32 uiRespawnZombiesTimer;
        bool bTransformed;

        InstanceScript* instance;

        void Reset()
        {
             uiCurseFleshTimer = 30000;  //30s DBM
             uiExplodeGhoulTimer = urand(25000, 28000); //approx 6 sec after summon ghouls
             uiShadowBoltTimer = urand(8000, 12000); // approx 10s
             uiStealFleshTimer = 12345;
             uiSummonGhoulsTimer = urand(19000, 24000); //on a video approx 24s after aggro
             uiRespawnZombiesTimer = 200000;
             bTransformed = false;

             if (instance)
                 instance->SetData(DATA_SALRAMM_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if (instance)
                 instance->SetData(DATA_SALRAMM_EVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Curse of twisted flesh timer
            if (uiCurseFleshTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CURSE_OF_TWISTED_FLESH);
                uiCurseFleshTimer = 37000;
            } else uiCurseFleshTimer -= diff;

            //Shadow bolt timer
            if (uiShadowBoltTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_SHADOW_BOLT);
                uiShadowBoltTimer = urand(8000, 12000);
            } else uiShadowBoltTimer -= diff;

            //Steal Flesh timer
            if (uiStealFleshTimer <= diff)
            {
                DoScriptText(RAND(SAY_STEAL_FLESH_1, SAY_STEAL_FLESH_2, SAY_STEAL_FLESH_3), me);
                if (Unit* random_pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(random_pTarget, SPELL_STEAL_FLESH);
                uiStealFleshTimer = 10000;
            } else uiStealFleshTimer -= diff;

            //Summon ghouls timer
            if (uiSummonGhoulsTimer <= diff)
            {
                DoScriptText(RAND(SAY_SUMMON_GHOULS_1, SAY_SUMMON_GHOULS_2), me);
                if (Unit* random_pTarget = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(random_pTarget, SPELL_SUMMON_GHOULS);
                uiSummonGhoulsTimer = 10000;
            } else uiSummonGhoulsTimer -= diff;

            // Respawn risen zombies at town
            if(!bTransformed)
                if (uiRespawnZombiesTimer <= diff)
                {
                    instance->SetData(DATA_TRANSFORM_CITIZENS, SPECIAL);
                    bTransformed = true;
                }
                else uiRespawnZombiesTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
                instance->SetData(DATA_SALRAMM_EVENT, DONE);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3), me);
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_salramm()
{
    new boss_salramm();
}
#endif
