////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "Object.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Map.h"

class SceneObject : public WorldObject
{
    protected:
        SceneObject();

    public:
        virtual ~SceneObject();

        static SceneObject* CreateSceneObject(uint32 sceneId, WorldObject* spectator);
};

#endif
