////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MODELHEADERS_H
#define MODELHEADERS_H

#include "mpqfile.h"    // integer typedefs

#pragma pack(push,1)

struct ModelHeader
{
    char id[4];
    uint8 version[4];
    uint32 nameLength;
    uint32 nameOfs;
    uint32 type;
    uint32 nGlobalSequences;
    uint32 ofsGlobalSequences;
    uint32 nAnimations;
    uint32 ofsAnimations;
    uint32 nAnimationLookup;
    uint32 ofsAnimationLookup;
    uint32 nBones;
    uint32 ofsBones;
    uint32 nKeyBoneLookup;
    uint32 ofsKeyBoneLookup;
    uint32 nVertices;
    uint32 ofsVertices;
    uint32 nViews;
    uint32 nColors;
    uint32 ofsColors;
    uint32 nTextures;
    uint32 ofsTextures;
    uint32 nTransparency;
    uint32 ofsTransparency;
    uint32 nTextureanimations;
    uint32 ofsTextureanimations;
    uint32 nTexReplace;
    uint32 ofsTexReplace;
    uint32 nRenderFlags;
    uint32 ofsRenderFlags;
    uint32 nBoneLookupTable;
    uint32 ofsBoneLookupTable;
    uint32 nTexLookup;
    uint32 ofsTexLookup;
    uint32 nTexUnits;
    uint32 ofsTexUnits;
    uint32 nTransLookup;
    uint32 ofsTransLookup;
    uint32 nTexAnimLookup;
    uint32 ofsTexAnimLookup;
    float floats[14];
    uint32 nBoundingTriangles;
    uint32 ofsBoundingTriangles;
    uint32 nBoundingVertices;
    uint32 ofsBoundingVertices;
    uint32 nBoundingNormals;
    uint32 ofsBoundingNormals;
    uint32 nAttachments;
    uint32 ofsAttachments;
    uint32 nAttachLookup;
    uint32 ofsAttachLookup;
    uint32 nAttachments_2;
    uint32 ofsAttachments_2;
    uint32 nLights;
    uint32 ofsLights;
    uint32 nCameras;
    uint32 ofsCameras;
    uint32 nCameraLookup;
    uint32 ofsCameraLookup;
    uint32 nRibbonEmitters;
    uint32 ofsRibbonEmitters;
    uint32 nParticleEmitters;
    uint32 ofsParticleEmitters;
};

#pragma pack(pop)
#endif
