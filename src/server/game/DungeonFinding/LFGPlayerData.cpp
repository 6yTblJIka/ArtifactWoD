////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "LFGPlayerData.h"

LfgPlayerData::LfgPlayerData():
m_State(LFG_STATE_NONE), m_OldState(LFG_STATE_NONE), m_Roles(0), m_Comment("")
{}

LfgPlayerData::~LfgPlayerData()
{
}

void LfgPlayerData::SetState(LfgState state)
{
    switch (state)
    {
        case LFG_STATE_NONE:
        case LFG_STATE_DUNGEON:
        case LFG_STATE_FINISHED_DUNGEON:
            m_OldState = m_State;
                    // No break on purpose
        default:
            m_State = state;
    }
}

void LfgPlayerData::ClearState()
{
    m_SelectedDungeons.clear();
    m_Roles = 0;
    m_State = m_OldState;
}

void LfgPlayerData::SetLockedDungeons(const LfgLockMap& lockStatus)
{
    m_LockedDungeons.clear();

    for (auto l_Iter : lockStatus)
        m_LockedDungeons.insert(l_Iter);
}

void LfgPlayerData::SetRoles(uint8 roles)
{
    m_Roles = roles;
}

void LfgPlayerData::SetComment(const std::string& comment)
{
    m_Comment = comment;
}

void LfgPlayerData::SetSelectedDungeons(const LfgDungeonSet& dungeons)
{
    m_SelectedDungeons = dungeons;
}

void LfgPlayerData::ClearSelectedDungeons()
{
    m_SelectedDungeons.clear();
}

LfgState LfgPlayerData::GetState() const
{
    return m_State;
}

const LfgLockMap & LfgPlayerData::GetLockedDungeons() const
{
    return m_LockedDungeons;
}

uint8 LfgPlayerData::GetRoles() const
{
    return m_Roles;
}

const std::string& LfgPlayerData::GetComment() const
{
    return m_Comment;
}

const LfgDungeonSet& LfgPlayerData::GetSelectedDungeons() const
{
    return m_SelectedDungeons;
}
