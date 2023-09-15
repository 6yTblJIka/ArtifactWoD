////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Feralas
SD%Complete: 0
SDComment: Quest support:
SDCategory: Feralas
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"

class npc_homing_robot_oox_22_fe : public CreatureScript
{
    public:
        npc_homing_robot_oox_22_fe() : CreatureScript("npc_homing_robot_oox_22_fe")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_homing_robot_oox_22_feAI(creature);
        }

        struct npc_homing_robot_oox_22_feAI : public ScriptedAI
        {
            npc_homing_robot_oox_22_feAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_feralas()
{
    new npc_homing_robot_oox_22_fe();
}
#endif
