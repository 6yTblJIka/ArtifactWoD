////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _FOLLOWERREFMANAGER
#define _FOLLOWERREFMANAGER

#include "RefManager.h"

class Unit;
class TargetedMovementGeneratorBase;

class FollowerRefManager : public RefManager<Unit, TargetedMovementGeneratorBase>
{

};
#endif
