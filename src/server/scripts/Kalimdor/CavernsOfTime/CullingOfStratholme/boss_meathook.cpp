////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* Script Data Start
SDName: Boss meathook
SDAuthor: Tartalo
SD%Complete: 100
SDComment: It may need timer adjustment
SDCategory:
Script Data End */

#include "ScriptPCH.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CONSTRICTING_CHAINS                   = 52696, //Encases the targets in chains, dealing 1800 Physical damage every 1 sec. and stunning the target for 5 sec.
    H_SPELL_CONSTRICTING_CHAINS                 = 58823,
    SPELL_DISEASE_EXPULSION                     = 52666, //Meathook belches out a cloud of disease, dealing 1710 to 1890 Nature damage and interrupting the spell casting of nearby enemy targets for 4 sec.
    H_SPELL_DISEASE_EXPULSION                   = 58824,
    SPELL_FRENZY                                = 58841 //Increases the caster's Physical damage by 10% for 30 sec.
};

enum Yells
{
    SAY_AGGRO                                   = -1595026,
    SAY_SLAY_1                                  = -1595027,
    SAY_SLAY_2                                  = -1595028,
    SAY_SLAY_3                                  = -1595029,
    SAY_SPAWN                                   = -1595030,
    SAY_DEATH                                   = -1595031
};

class boss_meathook : public CreatureScript
{
public:
    boss_meathook() : CreatureScript("boss_meathook") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_meathookAI (creature);
    }

    struct boss_meathookAI : public ScriptedAI
    {
        boss_meathookAI(Creature* c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
            if (instance)
                DoScriptText(SAY_SPAWN, me);
        }

        uint32 uiChainTimer;
        uint32 uiDiseaseTimer;
        uint32 uiFrenzyTimer;

        InstanceScript* instance;

        void Reset()
        {
            uiChainTimer = urand(12000, 17000);   //seen on video 13, 17, 15, 12, 16
            uiDiseaseTimer = urand(2000, 4000);   //approx 3s
            uiFrenzyTimer = urand(21000, 26000);  //made it up

            if (instance)
                instance->SetData(DATA_MEATHOOK_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if (instance)
                instance->SetData(DATA_MEATHOOK_EVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiDiseaseTimer <= diff)
            {
                DoCastAOE(SPELL_DISEASE_EXPULSION);
                uiDiseaseTimer = urand(1500, 4000);
            } else uiDiseaseTimer -= diff;

            if (uiFrenzyTimer <= diff)
            {
                DoCast(me, SPELL_FRENZY);
                uiFrenzyTimer = urand(21000, 26000);
            } else uiFrenzyTimer -= diff;

            if (uiChainTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_CONSTRICTING_CHAINS); //anyone but the tank
                uiChainTimer = urand(2000, 4000);
            } else uiChainTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
                instance->SetData(DATA_MEATHOOK_EVENT, DONE);
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
void AddSC_boss_meathook()
{
    new boss_meathook();
}
#endif
