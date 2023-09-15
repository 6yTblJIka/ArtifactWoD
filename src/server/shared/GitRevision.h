////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __GITREVISION_H__
# define __GITREVISION_H__

#include "Common.h"

namespace GitRevision
{
    char const* GetPackageName();
    char const* GetHash();
    char const* GetDate();
    char const* GetBranch();
    char const* GetSourceDirectory();
    char const* GetMySQLExecutable();
    char const* GetFullDatabase();
    char const* GetFullVersion();
    char const* GetCompanyNameStr();
    char const* GetLegalCopyrightStr();
    char const* GetFileVersionStr();
    char const* GetProductVersionStr();
}

#endif
