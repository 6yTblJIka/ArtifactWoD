////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "Chat.h"
#include "DisableMgr.h"
#include "ObjectMgr.h"
#include "OutdoorPvP.h"
#include "SpellMgr.h"
#include "World.h"
#include "VMapManager2.h"
#ifndef CROSS
#include "GarrisonMgr.hpp"
#endif /* not CROSS */

namespace DisableMgr
{

namespace
{
    struct DisableData
    {
        uint8 flags;
        std::set<uint32> params[2];                             // params0, params1
    };

    // single disables here with optional data
    typedef std::map<uint32, DisableData> DisableTypeMap;
    // global disable map by source
    typedef std::map<DisableType, DisableTypeMap> DisableMap;

    DisableMap m_DisableMap;

    uint8 MAX_DISABLE_TYPES = 9;
}

void LoadDisables()
{
    uint32 oldMSTime = getMSTime();

    // reload case
    for (DisableMap::iterator itr = m_DisableMap.begin(); itr != m_DisableMap.end(); ++itr)
        itr->second.clear();

    m_DisableMap.clear();

    QueryResult result = WorldDatabase.Query("SELECT sourceType, entry, flags, params_0, params_1 FROM disables");

    uint32 total_count = 0;

    if (!result)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded 0 disables. DB table `disables` is empty!");

        return;
    }

    Field* fields;
    do
    {
        fields = result->Fetch();
        DisableType type = DisableType(fields[0].GetUInt32());
        if (type >= MAX_DISABLE_TYPES)
        {
            sLog->outError(LOG_FILTER_SQL, "Invalid type %u specified in `disables` table, skipped.", type);
            continue;
        }

        uint32 entry = fields[1].GetUInt32();
        uint8 flags = fields[2].GetUInt8();
        std::string params_0 = fields[3].GetString();
        std::string params_1 = fields[4].GetString();

        DisableData data;
        data.flags = flags;

        switch (type)
        {
            case DISABLE_TYPE_SPELL:
                if (!(sSpellMgr->GetSpellInfo(entry) || flags & SPELL_DISABLE_DEPRECATED_SPELL))
                {
                    sLog->outError(LOG_FILTER_SQL, "Spell entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }

                if (!flags || flags > MAX_SPELL_DISABLE_TYPE)
                {
                    sLog->outError(LOG_FILTER_SQL, "Disable flags for spell %u are invalid, skipped.", entry);
                    continue;
                }

                if (flags & SPELL_DISABLE_MAP)
                {
                    Tokenizer tokens(params_0, ',');
                    for (uint8 i = 0; i < tokens.size(); )
                        data.params[0].insert(atoi(tokens[i++]));
                }

                if (flags & SPELL_DISABLE_AREA)
                {
                    Tokenizer tokens(params_1, ',');
                    for (uint8 i = 0; i < tokens.size(); )
                        data.params[1].insert(atoi(tokens[i++]));
                }

                break;
            // checked later
            case DISABLE_TYPE_QUEST:
                break;
            case DISABLE_TYPE_MAP:
            {
                MapEntry const* mapEntry = sMapStore.LookupEntry(entry);
                if (!mapEntry)
                {
                    sLog->outError(LOG_FILTER_SQL, "Map entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                bool isFlagInvalid = false;
                switch (mapEntry->instanceType)
                {
                    case MAP_COMMON:
                        if (flags)
                            isFlagInvalid = true;
                        break;
                    case MAP_INSTANCE:
                    case MAP_RAID:
                        if (flags & DUNGEON_STATUSFLAG_HEROIC && !GetMapDifficultyData(entry, DifficultyRaidHeroic))
                            isFlagInvalid = true;
                        else if (flags & RAID_STATUSFLAG_10MAN_HEROIC && !GetMapDifficultyData(entry, Difficulty10HC))
                            isFlagInvalid = true;
                        else if (flags & RAID_STATUSFLAG_25MAN_HEROIC && !GetMapDifficultyData(entry, Difficulty25HC))
                            isFlagInvalid = true;
                        break;
                    case MAP_BATTLEGROUND:
                    case MAP_ARENA:
                        sLog->outError(LOG_FILTER_SQL, "Battleground map %u specified to be disabled in map case, skipped.", entry);
                        continue;
                }
                if (isFlagInvalid)
                {
                    sLog->outError(LOG_FILTER_SQL, "Disable flags for map %u are invalid, skipped.", entry);
                    continue;
                }
                break;
            }
            case DISABLE_TYPE_BATTLEGROUND:
                if (!sBattlemasterListStore.LookupEntry(entry))
                {
                    sLog->outError(LOG_FILTER_SQL, "Battleground entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                if (flags)
                    sLog->outError(LOG_FILTER_SQL, "Disable flags specified for battleground %u, useless data.", entry);
                break;
            case DISABLE_TYPE_OUTDOORPVP:
                if (entry > MAX_OUTDOORPVP_TYPES)
                {
                    sLog->outError(LOG_FILTER_SQL, "OutdoorPvPTypes value %u from `disables` is invalid, skipped.", entry);
                    continue;
                }
                if (flags)
                    sLog->outError(LOG_FILTER_SQL, "Disable flags specified for outdoor PvP %u, useless data.", entry);
                break;
            case DISABLE_TYPE_VMAP:
            {
                MapEntry const* mapEntry = sMapStore.LookupEntry(entry);
                if (!mapEntry)
                {
                    sLog->outError(LOG_FILTER_SQL, "Map entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                switch (mapEntry->instanceType)
                {
                    case MAP_COMMON:
                        if (flags & VMAP::VMAP_DISABLE_AREAFLAG)
                            sLog->outInfo(LOG_FILTER_GENERAL, "Areaflag disabled for world map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LIQUIDSTATUS)
                            sLog->outInfo(LOG_FILTER_GENERAL, "Liquid status disabled for world map %u.", entry);
                        break;
                    case MAP_INSTANCE:
                    case MAP_RAID:
                        if (flags & VMAP::VMAP_DISABLE_HEIGHT)
                            sLog->outInfo(LOG_FILTER_GENERAL, "Height disabled for instance map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LOS)
                            sLog->outInfo(LOG_FILTER_GENERAL, "LoS disabled for instance map %u.", entry);
                        break;
                    case MAP_BATTLEGROUND:
                        if (flags & VMAP::VMAP_DISABLE_HEIGHT)
                            sLog->outInfo(LOG_FILTER_GENERAL, "Height disabled for battleground map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LOS)
                            sLog->outInfo(LOG_FILTER_GENERAL, "LoS disabled for battleground map %u.", entry);
                        break;
                    case MAP_ARENA:
                        if (flags & VMAP::VMAP_DISABLE_HEIGHT)
                            sLog->outInfo(LOG_FILTER_GENERAL, "Height disabled for arena map %u.", entry);
                        if (flags & VMAP::VMAP_DISABLE_LOS)
                            sLog->outInfo(LOG_FILTER_GENERAL, "LoS disabled for arena map %u.", entry);
                        break;
                    default:
                        break;
                }
                break;
            }
            case DISABLE_TYPE_MMAP:
            {
                MapEntry const* mapEntry = sMapStore.LookupEntry(entry);
                if (!mapEntry)
                {
                    sLog->outError(LOG_FILTER_SQL, "Map entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                switch (mapEntry->instanceType)
                {
                    case MAP_COMMON:
                        sLog->outInfo(LOG_FILTER_GENERAL, "Pathfinding disabled for world map %u.", entry);
                        break;
                    case MAP_INSTANCE:
                    case MAP_RAID:
                        sLog->outInfo(LOG_FILTER_GENERAL, "Pathfinding disabled for instance map %u.", entry);
                        break;
                    case MAP_BATTLEGROUND:
                        sLog->outInfo(LOG_FILTER_GENERAL, "Pathfinding disabled for battleground map %u.", entry);
                        break;
                    case MAP_ARENA:
                        sLog->outInfo(LOG_FILTER_GENERAL, "Pathfinding disabled for arena map %u.", entry);
                        break;
                    default:
                        break;
                }
                break;
            }
            case DISABLE_TYPE_GARRISON_MISSION:
            {
                GarrMissionEntry const* l_MissionEntry = sGarrMissionStore.LookupEntry(entry);

                if (!l_MissionEntry)
                {
                    sLog->outError(LOG_FILTER_SQL, "Garrison mission entry %u from `disables` doesn't exist in dbc, skipped.", entry);
                    continue;
                }
                break;
            }
            default:
                break;
        }

        m_DisableMap[type].insert(DisableTypeMap::value_type(entry, data));
        ++total_count;
    }
    while (result->NextRow());

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Loaded %u disables in %u ms", total_count, GetMSTimeDiffToNow(oldMSTime));
}

bool IsVMAPDisabledFor(uint32 entry, uint8 flags)
{
    return IsDisabledFor(DISABLE_TYPE_VMAP, entry, NULL, flags);
}

bool IsPathfindingEnabled(uint32 mapId)
{
    return sWorld->getBoolConfig(CONFIG_ENABLE_MMAPS)
        && !IsDisabledFor(DISABLE_TYPE_MMAP, mapId, NULL, MMAP_DISABLE_PATHFINDING);
}

void CheckQuestDisables()
{
    uint32 oldMSTime = getMSTime();

    uint32 count = m_DisableMap[DISABLE_TYPE_QUEST].size();
    if (!count)
    {
        sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Checked 0 quest disables.");

        return;
    }

    // check only quests, rest already done at startup
    for (DisableTypeMap::iterator itr = m_DisableMap[DISABLE_TYPE_QUEST].begin(); itr != m_DisableMap[DISABLE_TYPE_QUEST].end();)
    {
        const uint32 entry = itr->first;
        if (!sObjectMgr->GetQuestTemplate(entry))
        {
            sLog->outError(LOG_FILTER_SQL, "Quest entry %u from `disables` doesn't exist, skipped.", entry);
            m_DisableMap[DISABLE_TYPE_QUEST].erase(itr++);
            continue;
        }
        if (itr->second.flags)
            sLog->outError(LOG_FILTER_SQL, "Disable flags specified for quest %u, useless data.", entry);
        ++itr;
    }

    sLog->outInfo(LOG_FILTER_SERVER_LOADING, ">> Checked %u quest disables in %u ms", count, GetMSTimeDiffToNow(oldMSTime));

}

bool IsDisabledFor(DisableType type, uint32 entry, Unit const* unit, uint8 flags)
{
    ASSERT(type < MAX_DISABLE_TYPES);
    if (m_DisableMap[type].empty())
        return false;

    DisableTypeMap::iterator itr = m_DisableMap[type].find(entry);
    if (itr == m_DisableMap[type].end())    // not disabled
        return false;

    switch (type)
    {
        case DISABLE_TYPE_SPELL:
        {
            uint8 spellFlags = itr->second.flags;
            if (unit)
            {
                if ((spellFlags & SPELL_DISABLE_PLAYER && unit->IsPlayer()) ||
                    (unit->GetTypeId() == TYPEID_UNIT && ((unit->ToCreature()->isPet() && spellFlags & SPELL_DISABLE_PET) || spellFlags & SPELL_DISABLE_CREATURE)))
                {
                    if (spellFlags & SPELL_DISABLE_MAP)
                    {
                        std::set<uint32> const& mapIds = itr->second.params[0];
                        if (mapIds.find(unit->GetMapId()) != mapIds.end())
                            return true;                                        // Spell is disabled on current map

                        if (!(spellFlags & SPELL_DISABLE_AREA))
                            return false;                                       // Spell is disabled on another map, but not this one, return false

                        // Spell is disabled in an area, but not explicitly our current mapId. Continue processing.
                    }

                    if (spellFlags & SPELL_DISABLE_AREA)
                    {
                        std::set<uint32> const& areaIds = itr->second.params[1];
                        if (areaIds.find(unit->GetAreaId()) != areaIds.end())
                            return true;                                        // Spell is disabled in this area
                        return false;                                           // Spell is disabled in another area, but not this one, return false
                    }
                    else
                        return true;                                            // Spell disabled for all maps
                }

                return false;
            }
            else if (spellFlags & SPELL_DISABLE_DEPRECATED_SPELL)    // call not from spellcast
                return true;

            break;
        }
        case DISABLE_TYPE_MAP:
            if (Player const* player = unit->ToPlayer())
            {
                MapEntry const* mapEntry = sMapStore.LookupEntry(entry);
                if (mapEntry->IsDungeon())
                {
                    uint8 disabledModes = itr->second.flags;
                    Difficulty targetDifficulty = player->GetDifficultyID(mapEntry);
                    GetDownscaledMapDifficultyData(entry, targetDifficulty);
                    switch (targetDifficulty)
                    {
                        case DifficultyNormal:
                            return disabledModes & DUNGEON_STATUSFLAG_NORMAL;
                        case DifficultyRaidHeroic:
                            return disabledModes & DUNGEON_STATUSFLAG_HEROIC;
                        case Difficulty10HC:
                            return disabledModes & RAID_STATUSFLAG_10MAN_HEROIC;
                        case Difficulty25HC:
                            return disabledModes & RAID_STATUSFLAG_25MAN_HEROIC;
                        default:
                            break;
                    }
                }
                else if (mapEntry->instanceType == MAP_COMMON)
                    return true;
            }
            return false;
        case DISABLE_TYPE_QUEST:
        {
            if (!unit)
                return true;

            if (Player const* l_Player = unit->ToPlayer())
            {
                if (l_Player->isGameMaster())
                    return false;

                if (l_Player->m_IsDebugQuestLogs && l_Player->GetSession())
                    ChatHandler(l_Player->GetSession()).PSendSysMessage(LANG_DEBUG_QUEST_LOGS_DISABLED);
            }

            return true;
        }
        case DISABLE_TYPE_BATTLEGROUND:
        case DISABLE_TYPE_OUTDOORPVP:
        case DISABLE_TYPE_ACHIEVEMENT_CRITERIA:
#ifndef CROSS
        case DISABLE_TYPE_GARRISON_MISSION:
#endif /* not CROSS */
            return true;
        case DISABLE_TYPE_VMAP:
           return flags & itr->second.flags;
    }

    return false;
}

} // Namespace
