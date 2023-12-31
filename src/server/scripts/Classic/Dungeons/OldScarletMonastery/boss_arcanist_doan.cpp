////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Arcanist_Doan
SD%Complete: 100
SDComment:
SDCategory: Scarlet Monastery
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eEnums
{
    SAY_AGGRO                   = -1189019,
    SAY_SPECIALAE               = -1189020,

    SPELL_POLYMORPH             = 13323,
    SPELL_AOESILENCE            = 8988,
    SPELL_ARCANEEXPLOSION       = 9433,
    SPELL_FIREAOE               = 9435,
    SPELL_ARCANEBUBBLE          = 9438
};

class boss_arcanist_doan : public CreatureScript
{
public:
    boss_arcanist_doan() : CreatureScript("boss_arcanist_doan") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_arcanist_doanAI (creature);
    }

    struct boss_arcanist_doanAI : public ScriptedAI
    {
        boss_arcanist_doanAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 Polymorph_Timer;
        uint32 AoESilence_Timer;
        uint32 ArcaneExplosion_Timer;
        bool bCanDetonate;
        bool bShielded;

        void Reset()
        {
            Polymorph_Timer = 20000;
            AoESilence_Timer = 15000;
            ArcaneExplosion_Timer = 3000;
            bCanDetonate = false;
            bShielded = false;
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (bShielded && bCanDetonate)
            {
                DoCast(me, SPELL_FIREAOE);
                bCanDetonate = false;
            }

            if (me->HasAura(SPELL_ARCANEBUBBLE))
                return;

            //If we are <50% hp cast Arcane Bubble
            if (!bShielded && !HealthAbovePct(50))
            {
                //wait if we already casting
                if (me->IsNonMeleeSpellCasted(false))
                    return;

                DoScriptText(SAY_SPECIALAE, me);
                DoCast(me, SPELL_ARCANEBUBBLE);

                bCanDetonate = true;
                bShielded = true;
            }

            if (Polymorph_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1))
                    DoCast(target, SPELL_POLYMORPH);

                Polymorph_Timer = 20000;
            }
            else Polymorph_Timer -= diff;

            //AoESilence_Timer
            if (AoESilence_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_AOESILENCE);
                AoESilence_Timer = urand(15000, 20000);
            }
            else AoESilence_Timer -= diff;

            //ArcaneExplosion_Timer
            if (ArcaneExplosion_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_ARCANEEXPLOSION);
                ArcaneExplosion_Timer = 8000;
            }
            else ArcaneExplosion_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_boss_arcanist_doan()
{
    new boss_arcanist_doan();
}
#endif
