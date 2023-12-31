////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_THESTONECORE_H
#define DEF_THESTONECORE_H

#define TSScriptName "instance_the_stonecore"

#define MAX_ENCOUNTERS 4

enum Data
{
    DATA_CORBORUS               = 0,
    DATA_SLABHIDE               = 1,
    DATA_OZRUK                  = 2,
    DATA_HIGH_PRIESTESS_AZIL    = 3,
    DATA_TEAM_IN_INSTANCE       = 4
};

enum CreatureIds
{
    NPC_CORBORUS                  = 43438,
    NPC_SLABHIDE                  = 43214,
    NPC_OZRUK                     = 42188,
    NPC_HIGH_PRIESTESS_AZIL       = 42333,

    NPC_CRYSTALSPAWN_GIANT         = 42810,
    NPC_IMP                        = 43014,
    NPC_MILLHOUSE_MANASTORM        = 43391,
    NPC_STONECORE_BERSERKER        = 43430,
    NPC_STONECORE_BRUISER          = 42692,
    NPC_STONECORE_EARTHSHAPER      = 43537,
    NPC_STONECORE_FLAYER           = 42808,
    NPC_MAGMALORD                  = 42789,
    NPC_RIFT_CONJURER              = 42691,
    NPC_STONECORE_SENTRY           = 42695,
    NPC_STONECORE_WARBRINGER       = 42696,

    NPC_EARTHWARDEN_YRSA           = 50048,
    NPC_STONECORE_TELEPORTER1      = 51396,
    NPC_STONECORE_TELEPORTER2      = 51397
};

enum GameObjectIds
{
    GO_BROKEN_PILLAR               = 207407,
    GO_TWILIGHT_DOCUMENTS          = 207415
};

#endif
