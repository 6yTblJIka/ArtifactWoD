////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef APPENDERCONSOLE_H
#define APPENDERCONSOLE_H

#include "Appender.h"
#include "Common.h"

enum ColorTypes
{
    BLACK,
    RED,
    GREEN,
    BROWN,
    BLUE,
    MAGENTA,
    CYAN,
    GREY,
    YELLOW,
    LRED,
    LGREEN,
    LBLUE,
    LMAGENTA,
    LCYAN,
    WHITE
};

const uint8 MaxColors = uint8(WHITE) + 1;

class AppenderConsole: public Appender
{
    public:
        AppenderConsole(uint8 _id, std::string const& name, LogLevel level, AppenderFlags flags);
        void InitColors(const std::string& init_str);

    private:
        void SetColor(bool stdout_stream, ColorTypes color);
        void ResetColor(bool stdout_stream);
        void _write(LogMessage& message);
        bool _colored;
        ColorTypes _colors[MaxLogLevels];
};

#endif
