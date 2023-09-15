////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LOGGER_H
#define LOGGER_H

#include "Appender.h"
#include "Common.h"

class Logger
{
    public:
        Logger();
        ~Logger();

        void Create(std::string const& name, LogFilterType type, LogLevel level);
        void addAppender(uint8 type, Appender *);
        void delAppender(uint8 type);

        std::string const& getName() const;
        LogFilterType getType() const;
        LogLevel getLogLevel() const;
        void setLogLevel(LogLevel level);
        void write(LogMessage& message);

    private:
        std::string name;
        LogFilterType type;
        LogLevel level;
        AppenderMap appenders;
};

#endif
