////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPENDER_H
#define APPENDER_H

#include "Define.h"
#include "Common.h"

enum LogFilterType
{
    LOG_FILTER_GENERAL,                                    // This one should only be used inside Log.cpp
    LOG_FILTER_UNITS,                                      // Anything related to units that doesn't fit in other categories. ie. creature formations
    LOG_FILTER_PETS,
    LOG_FILTER_VEHICLES,
    LOG_FILTER_TSCR,                                       // C++ AI, instance scripts, etc.
    LOG_FILTER_DATABASE_AI,                                // SmartAI, Creature* * AI
    LOG_FILTER_MAPSCRIPTS,
    LOG_FILTER_NETWORKIO,
    LOG_FILTER_SPELLS_AURAS,
    LOG_FILTER_ACHIEVEMENTSYS,
    LOG_FILTER_CONDITIONSYS,
    LOG_FILTER_POOLSYS,
    LOG_FILTER_AUCTIONHOUSE,
    LOG_FILTER_BATTLEGROUND,
    LOG_FILTER_OUTDOORPVP,
    LOG_FILTER_CHATSYS,
    LOG_FILTER_LFG,
    LOG_FILTER_MAPS,
    LOG_FILTER_PLAYER,                                     // Any player log that does not fit in other player filters
    LOG_FILTER_PLAYER_LOADING,                             // Debug output from Player::_Load functions
    LOG_FILTER_PLAYER_ITEMS,
    LOG_FILTER_PLAYER_SKILLS,
    LOG_FILTER_PLAYER_CHATLOG,
    LOG_FILTER_LOOT,
    LOG_FILTER_GUILD,
    LOG_FILTER_TRANSPORTS,
    LOG_FILTER_SQL,
    LOG_FILTER_GMCOMMAND,
    LOG_FILTER_REMOTECOMMAND,
    LOG_FILTER_WARDEN,
    LOG_FILTER_AUTHSERVER,
    LOG_FILTER_WORLDSERVER,
    LOG_FILTER_GAMEEVENTS,
    LOG_FILTER_CALENDAR,
    LOG_FILTER_CHARACTER,
    LOG_FILTER_ARENAS,
    LOG_FILTER_SQL_DRIVER,
    LOG_FILTER_SQL_DEV,
    LOG_FILTER_PLAYER_DUMP,
    LOG_FILTER_BATTLEFIELD,
    LOG_FILTER_SERVER_LOADING,
    LOG_FILTER_OPCODES,
    LOG_FILTER_INTERREALM,
    LOG_FILTER_PROFILING
};

const uint8 MaxLogFilter = uint8(LOG_FILTER_OPCODES) + 1;
#define MAX_LOG_FILTER (LOG_FILTER_OPCODES + 1)

// Values assigned have their equivalent in enum ACE_Log_Priority
enum LogLevel
{
    LOG_LEVEL_DISABLED                           = 0,
    LOG_LEVEL_TRACE                              = 1,
    LOG_LEVEL_DEBUG                              = 2,
    LOG_LEVEL_INFO                               = 3,
    LOG_LEVEL_WARN                               = 4,
    LOG_LEVEL_ERROR                              = 5,
    LOG_LEVEL_FATAL                              = 6,
};

const uint8 MaxLogLevels = 6;

enum AppenderType
{
    APPENDER_NONE,
    APPENDER_CONSOLE,
    APPENDER_FILE,
    APPENDER_DB,
};

enum AppenderFlags
{
    APPENDER_FLAGS_NONE                 = 0x00,
    APPENDER_FLAGS_PREFIX_TIMESTAMP     = 0x01,
    APPENDER_FLAGS_PREFIX_LOGLEVEL      = 0x02,
    APPENDER_FLAGS_PREFIX_LOGFILTERTYPE = 0x04,
    APPENDER_FLAGS_USE_TIMESTAMP        = 0x08, // only used by FileAppender
    APPENDER_FLAGS_MAKE_FILE_BACKUP     = 0x10  // only used by FileAppender
};

struct LogMessage
{
    LogMessage(LogLevel _level, LogFilterType _type, std::string _text)
        : level(_level)
        , type(_type)
        , text(_text)
    {
        mtime = time(NULL);
    }
    LogMessage(LogMessage const& /*other*/) = delete;
    LogMessage& operator=(LogMessage const& /*other*/) = delete;

    static std::string getTimeStr(time_t time);
    std::string getTimeStr();

    LogLevel level;
    LogFilterType type;
    std::string text;
    std::string prefix;
    std::string param1;
    time_t mtime;
};

class Appender
{
    public:
        Appender(uint8 _id, std::string const& name, AppenderType type = APPENDER_NONE, LogLevel level = LOG_LEVEL_DISABLED, AppenderFlags flags = APPENDER_FLAGS_NONE);
        virtual ~Appender();

        uint8 getId() const;
        std::string const& getName() const;
        AppenderType getType() const;
        LogLevel getLogLevel() const;
        AppenderFlags getFlags() const;

        void setLogLevel(LogLevel);
        void write(LogMessage& message);
        static const char* getLogLevelString(LogLevel level);
        static const char* getLogFilterTypeString(LogFilterType type);

    private:
        virtual void _write(LogMessage& /*message*/) = 0;

        uint8 id;
        std::string name;
        AppenderType type;
        LogLevel level;
        AppenderFlags flags;
};

typedef std::map<uint8, Appender*> AppenderMap;

#endif
