////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "LogOperation.h"
#include "Logger.h"

LogOperation::~LogOperation()
{
    delete msg;
}

int LogOperation::call()
{
    if (logger && msg)
        logger->write(*msg);
    return 0;
}
