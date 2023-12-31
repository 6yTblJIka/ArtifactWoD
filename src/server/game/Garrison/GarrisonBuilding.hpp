////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef GARRISON_BUILDING_HPP_GARRISON
#define GARRISON_BUILDING_HPP_GARRISON

#include "Common.h"

namespace MS { namespace Garrison
{
    /// Garrison building
    struct GarrisonBuilding
    {
        uint32 DatabaseID;         ///< Building DB ID
        uint32 PlotInstanceID;     ///< Plot instance ID
        uint32 BuildingID;         ///< Building ID (GarrBuilding.db2)
        uint32 SpecID;             ///< Building spec ID
        uint32 TimeBuiltStart;     ///< Time built start
        uint32 TimeBuiltEnd;       ///< Time build end
        uint32 FollowerAssigned;   ///< Follower Assigned (Database ID)

        std::string GatheringData; ///< Building gathering data

        bool Active;               ///< Is building active
        bool BuiltNotified;        ///< Is built notified

        /// Reset building
        GarrisonBuilding()
        {
            DatabaseID       = 0;
            PlotInstanceID   = 0;
            BuildingID       = 0;
            SpecID           = 0;
            TimeBuiltStart   = 0;
            TimeBuiltEnd     = 0;
            FollowerAssigned = 0;

            GatheringData = "";

            Active          = false;
            BuiltNotified   = false;
        }
    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_BUILDING_HPP_GARRISON
#endif
