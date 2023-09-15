////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"

class instance_scenario_black_temple : public InstanceMapScript
{
public:

    instance_scenario_black_temple() : InstanceMapScript("instance_scenario_black_temple", 1112) { }

    InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
    {
        return new instance_scenario_black_temple_instancemapscript(p_Map);
    }

    struct instance_scenario_black_temple_instancemapscript : public InstanceScript
    {
        instance_scenario_black_temple_instancemapscript(Map* p_Map) : InstanceScript(p_Map) {}

        InstanceScript* m_Instance = this;
    };
};

#ifndef __clang_analyzer__
void AddSC_instance_scenario_black_temple()
{
    new instance_scenario_black_temple();
}
#endif