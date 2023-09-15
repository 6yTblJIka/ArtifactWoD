////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_ARCHAVON_H
#define DEF_ARCHAVON_H

enum Creatures
{
    CREATURE_ARCHAVON                           = 31125,
    CREATURE_EMALON                             = 33993,
    CREATURE_KORALON                            = 35013,
    CREATURE_TORAVON                            = 38433
};

enum Data
{
    DATA_ARCHAVON   = 0,
    DATA_EMALON     = 1,
    DATA_KORALON    = 2,
    DATA_TORAVON    = 3
};

#define MAX_ENCOUNTER 4

enum AchievementCriteriaIds
{
    CRITERIA_EARTH_WIND_FIRE_10 = 12018,
    CRITERIA_EARTH_WIND_FIRE_25 = 12019
};

enum AchievementSpells
{
    SPELL_EARTH_WIND_FIRE_ACHIEVEMENT_CHECK = 68308
};

#endif
