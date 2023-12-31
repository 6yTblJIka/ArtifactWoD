////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Ouro
SD%Complete: 85
SDComment: No model for submerging. Currently just invisible.
SDCategory: Temple of Ahn'Qiraj
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "temple_of_ahnqiraj.h"

#define SPELL_SWEEP             26103
#define SPELL_SANDBLAST         26102
#define SPELL_GROUND_RUPTURE    26100
#define SPELL_BIRTH             26262                       //The Birth Animation

#define SPELL_DIRTMOUND_PASSIVE 26092

class boss_ouro : public CreatureScript
{
public:
    boss_ouro() : CreatureScript("boss_ouro") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_ouroAI (creature);
    }

    struct boss_ouroAI : public ScriptedAI
    {
        boss_ouroAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 Sweep_Timer;
        uint32 SandBlast_Timer;
        uint32 Submerge_Timer;
        uint32 Back_Timer;
        uint32 ChangeTarget_Timer;
        uint32 Spawn_Timer;

        bool Enrage;
        bool Submerged;

        void Reset()
        {
            Sweep_Timer = urand(5000, 10000);
            SandBlast_Timer = urand(20000, 35000);
            Submerge_Timer = urand(90000, 150000);
            Back_Timer = urand(30000, 45000);
            ChangeTarget_Timer = urand(5000, 8000);
            Spawn_Timer = urand(10000, 20000);

            Enrage = false;
            Submerged = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoCast(me->getVictim(), SPELL_BIRTH);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //Sweep_Timer
            if (!Submerged && Sweep_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SWEEP);
                Sweep_Timer = urand(15000, 30000);
            } else Sweep_Timer -= diff;

            //SandBlast_Timer
            if (!Submerged && SandBlast_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SANDBLAST);
                SandBlast_Timer = urand(20000, 35000);
            } else SandBlast_Timer -= diff;

            //Submerge_Timer
            if (!Submerged && Submerge_Timer <= diff)
            {
                //Cast
                me->HandleEmoteCommand(EMOTE_ONESHOT_SUBMERGE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(35);
                DoCast(me, SPELL_DIRTMOUND_PASSIVE);

                Submerged = true;
                Back_Timer = urand(30000, 45000);
            } else Submerge_Timer -= diff;

            //ChangeTarget_Timer
            if (Submerged && ChangeTarget_Timer <= diff)
            {
                Unit* target = NULL;
                target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                if (target)
                    DoTeleportTo(target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());

                ChangeTarget_Timer = urand(10000, 20000);
            } else ChangeTarget_Timer -= diff;

            //Back_Timer
            if (Submerged && Back_Timer <= diff)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                me->setFaction(14);

                DoCast(me->getVictim(), SPELL_GROUND_RUPTURE);

                Submerged = false;
                Submerge_Timer = urand(60000, 120000);
            } else Back_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_ouro()
{
    new boss_ouro();
}
#endif
