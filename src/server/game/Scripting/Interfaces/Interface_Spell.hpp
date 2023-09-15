////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCRIPTING_INTERFACES_SPELL_HPP_INCLUDED
#define SCRIPTING_INTERFACES_SPELL_HPP_INCLUDED

#include "InterfaceBase.hpp"

/// Spell script interface
class SpellScriptLoader : public ScriptObjectImpl<true>
{
    protected:
        /// Constructor
        /// @p_Name : Script name
        SpellScriptLoader(const char * p_Name);

    public:
        /// Should return a fully valid SpellScript pointer.
        virtual SpellScript* GetSpellScript() const
        {
            return nullptr;
        }
        /// Should return a fully valid AuraScript pointer.
        virtual AuraScript* GetAuraScript() const
        {
            return nullptr;
        }

};

#endif  ///< SCRIPTING_INTERFACES_SPELL_HPP_INCLUDED
