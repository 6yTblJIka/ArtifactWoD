////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef SCRIPTING_INTERFACES_ENCOUNTER_HPP_INCLUDED
 #define SCRIPTING_INTERFACES_ENCOUNTER_HPP_INCLUDED

 #include "InterfaceBase.hpp"

/// Encounter Script Interface
class EncounterScript : public ScriptObjectImpl<false>
{
    protected:
        /// Constructor
        EncounterScript(char const* p_Name);

    public:
        /// Called when an Encounter is ended
        virtual void OnEncounterEnd(EncounterDatas const* p_EncounterDatas)
        {
            UNUSED(p_EncounterDatas);
        }
};

#endif  ///< SCRIPTING_INTERFACES_ENCOUNTER_HPP_INCLUDED
