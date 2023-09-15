////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LOGOPERATION_H
#define LOGOPERATION_H

class Logger;
struct LogMessage;

class LogOperation
{
    public:
        LogOperation(Logger* _logger, LogMessage* _msg)
            : logger(_logger)
            , msg(_msg)
        { }

        ~LogOperation();

        int call();

    protected:
        Logger *logger;
        LogMessage *msg;
};

#endif
