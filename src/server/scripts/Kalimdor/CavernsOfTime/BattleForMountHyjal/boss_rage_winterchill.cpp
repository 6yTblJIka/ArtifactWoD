////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "hyjal.h"
#include "hyjal_trash.h"

enum Spells
{
    SPELL_FROST_ARMOR           = 31256,
    SPELL_DEATH_AND_DECAY       = 31258,
    SPELL_FROST_NOVA            = 31250,
    SPELL_ICEBOLT               = 31249
};

enum Texts
{
    SAY_ONDEATH         = 0,
    SAY_ONSLAY          = 1,
    SAY_DECAY           = 2,
    SAY_NOVA            = 3,
    SAY_ONAGGRO         = 4
};

class boss_rage_winterchill : public CreatureScript
{
public:
    boss_rage_winterchill() : CreatureScript("boss_rage_winterchill") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_rage_winterchillAI (creature);
    }

    struct boss_rage_winterchillAI : public hyjal_trashAI
    {
        boss_rage_winterchillAI(Creature* creature) : hyjal_trashAI(creature)
        {
            instance = creature->GetInstanceScript();
            go = false;
        }

        uint32 FrostArmorTimer;
        uint32 DecayTimer;
        uint32 NovaTimer;
        uint32 IceboltTimer;
        bool go;

        void Reset()
        {
            damageTaken = 0;
            FrostArmorTimer = 37000;
            DecayTimer = 45000;
            NovaTimer = 15000;
            IceboltTimer = 10000;

            if (instance && IsEvent)
                instance->SetData(DATA_RAGEWINTERCHILLEVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance && IsEvent)
                instance->SetData(DATA_RAGEWINTERCHILLEVENT, IN_PROGRESS);
            Talk(SAY_ONAGGRO);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            Talk(SAY_ONSLAY);
        }

        void WaypointReached(uint32 waypointId)
        {
            if (waypointId == 7 && instance)
            {
                Unit* target = Unit::GetUnit(*me, instance->GetData64(DATA_JAINAPROUDMOORE));
                if (target && target->isAlive())
                    me->AddThreat(target, 0.0f);
            }
        }

        void JustDied(Unit* killer)
        {
            hyjal_trashAI::JustDied(killer);
            if (instance && IsEvent)
                instance->SetData(DATA_RAGEWINTERCHILLEVENT, DONE);
            Talk(SAY_ONDEATH);
        }

        void UpdateAI(const uint32 diff)
        {
            if (IsEvent)
            {
                //Must update npc_escortAI
                npc_escortAI::UpdateAI(diff);
                if (!go)
                {
                    go = true;
                    if (instance)
                    {
                        AddWaypoint(0, 4896.08f,    -1576.35f,    1333.65f);
                        AddWaypoint(1, 4898.68f,    -1615.02f,    1329.48f);
                        AddWaypoint(2, 4907.12f,    -1667.08f,    1321.00f);
                        AddWaypoint(3, 4963.18f,    -1699.35f,    1340.51f);
                        AddWaypoint(4, 4989.16f,    -1716.67f,    1335.74f);
                        AddWaypoint(5, 5026.27f,    -1736.89f,    1323.02f);
                        AddWaypoint(6, 5037.77f,    -1770.56f,    1324.36f);
                        AddWaypoint(7, 5067.23f,    -1789.95f,    1321.17f);
                        Start(false, true);
                        SetDespawnAtEnd(false);
                    }
                }
            }

            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (FrostArmorTimer <= diff)
            {
                DoCast(me, SPELL_FROST_ARMOR);
                FrostArmorTimer = 40000+rand()%20000;
            } else FrostArmorTimer -= diff;
            if (DecayTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_DEATH_AND_DECAY);
                DecayTimer = 60000+rand()%20000;
                Talk(SAY_DECAY);
            } else DecayTimer -= diff;
            if (NovaTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_FROST_NOVA);
                NovaTimer = 30000+rand()%15000;
                Talk(SAY_NOVA);
            } else NovaTimer -= diff;
            if (IceboltTimer <= diff)
            {
                DoCast(SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true), SPELL_ICEBOLT);
                IceboltTimer = 11000+rand()%20000;
            } else IceboltTimer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_rage_winterchill()
{
    new boss_rage_winterchill();
}
#endif
