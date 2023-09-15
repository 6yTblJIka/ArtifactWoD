////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
//  Copyright 2014 ArtifactCore based on TC 6.2.X SARL
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _MEMORY_H
#define _MEMORY_H

#include "DetourAlloc.h"

//  memory management
inline void* dtCustomAlloc(int size, dtAllocHint /*hint*/)
{
    return (void*)new unsigned char[size];
}

inline void dtCustomFree(void* ptr)
{
    delete [] (unsigned char*)ptr;
}

#endif
