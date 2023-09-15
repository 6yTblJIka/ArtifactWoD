////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef WDTFILE_H
#define WDTFILE_H

#include "mpqfile.h"
#include "wmo.h"
#include <string>
#include <vector>
#include <cstdlib>

class ADTFile;

class WDTFile
{
private:
    MPQFile WDT;
    std::string filename;
public:
    WDTFile(char* file_name, char* file_name1);
    ~WDTFile(void);
    bool init(char* map_id, unsigned int mapID);

    std::vector<std::string> gWmoInstansName;
    int gnWMO;

    ADTFile* GetMap(int x, int z);
};

#endif
