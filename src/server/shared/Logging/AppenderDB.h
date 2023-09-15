////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPENDERDB_H
#define APPENDERDB_H

#include "Appender.h"

class AppenderDB: public Appender
{
    public:
        AppenderDB(uint8 _id, std::string const& _name, LogLevel level, uint8 realmId);
        ~AppenderDB();
        void setEnable(bool enable);

    private:
        uint8 realm;
        bool enable;
        void _write(LogMessage& message);
};

#endif
