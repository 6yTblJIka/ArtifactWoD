////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"

class instance_scenario_fate_of_karabor : public InstanceMapScript
{
    public:

        instance_scenario_fate_of_karabor() : InstanceMapScript("instance_scenario_fate_of_karabor", 1277) { }

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
        {
            return new instance_scenario_fate_of_karabor_instancemapscript(p_Map);
        }

        struct instance_scenario_fate_of_karabor_instancemapscript : public InstanceScript
        {
            instance_scenario_fate_of_karabor_instancemapscript(Map* p_Map) : InstanceScript(p_Map) {}

            InstanceScript* m_Instance = this;
        };
};

#ifndef __clang_analyzer__
void AddSC_instance_scenario_fate_of_karabor()
{
    new instance_scenario_fate_of_karabor();
}
#endif
