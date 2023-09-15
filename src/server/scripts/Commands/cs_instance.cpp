////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
Name: instance_commandscript
%Complete: 100
Comment: All instance related commands
Category: commandscripts
EndScriptData */

#include "ScriptMgr.h"
#include "Chat.h"
#include "Group.h"
#include "InstanceSaveMgr.h"
#include "InstanceScript.h"
#include "MapManager.h"

class instance_commandscript: public CommandScript
{
public:
    instance_commandscript() : CommandScript("instance_commandscript") { }

    ChatCommand* GetCommands() const
    {
        static ChatCommand instanceCommandTable[] =
        {
            { "listbinds",      SEC_ADMINISTRATOR,  false,  &HandleInstanceListBindsCommand,    "", NULL },
            { "unbind",         SEC_ADMINISTRATOR,  false,  &HandleInstanceUnbindCommand,       "", NULL },
            { "stats",          SEC_ADMINISTRATOR,  true,   &HandleInstanceStatsCommand,        "", NULL },
            { "savedata",       SEC_ADMINISTRATOR,  false,  &HandleInstanceSaveDataCommand,     "", NULL },
            { NULL,             0,                  false,  NULL,                               "", NULL }
        };

        static ChatCommand commandTable[] =
        {
            { "instance",       SEC_ADMINISTRATOR,  true,   NULL,                               "", instanceCommandTable },
            { NULL,             0,                  false,  NULL,                               "", NULL }
        };

        return commandTable;
    }

    static std::string GetTimeString(uint64 time)
    {
        uint64 days = time / DAY, hours = (time % DAY) / HOUR, minute = (time % HOUR) / MINUTE;
        std::ostringstream ss;
        if (days)
            ss << days << "d ";
        if (hours)
            ss << hours << "h ";
        ss << minute << 'm';
        return ss.str();
    }

    static bool HandleInstanceListBindsCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->getSelectedPlayer();
        if (!player)
            player = handler->GetSession()->GetPlayer();

        uint32 counter = 0;
        for (uint8 i = 0; i < Difficulty::MaxDifficulties; ++i)
        {
            Player::BoundInstancesMap &binds = player->GetBoundInstances(Difficulty(i));
            for (Player::BoundInstancesMap::const_iterator itr = binds.begin(); itr != binds.end(); ++itr)
            {
                InstanceSave* save = itr->second.save;
                std::string timeleft = GetTimeString(save->GetResetTime() - time(NULL));
                handler->PSendSysMessage("map: %d inst: %d perm: %s diff: %d canReset: %s TTR: %s", itr->first, save->GetInstanceId(), itr->second.perm ? "yes" : "no",  save->GetDifficultyID(), save->CanReset() ? "yes" : "no", timeleft.c_str());
                counter++;
            }
        }
        handler->PSendSysMessage("player binds: %d", counter);

        counter = 0;
        if (Group* group = player->GetGroup())
        {
            for (uint8 i = 0; i < Difficulty::MaxDifficulties; ++i)
            {
                Group::BoundInstancesMap &binds = group->GetBoundInstances(Difficulty(i));
                for (Group::BoundInstancesMap::const_iterator itr = binds.begin(); itr != binds.end(); ++itr)
                {
                    InstanceSave* save = itr->second.save;
                    std::string timeleft = GetTimeString(save->GetResetTime() - time(NULL));
                    handler->PSendSysMessage("map: %d inst: %d perm: %s diff: %d canReset: %s TTR: %s", itr->first, save->GetInstanceId(), itr->second.perm ? "yes" : "no",  save->GetDifficultyID(), save->CanReset() ? "yes" : "no", timeleft.c_str());
                    counter++;
                }
            }
        }
        handler->PSendSysMessage("group binds: %d", counter);

        return true;
    }

    static bool HandleInstanceUnbindCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        Player* player = handler->getSelectedPlayer();
        if (!player)
            player = handler->GetSession()->GetPlayer();

        char* map = strtok((char*)args, " ");
        char* pDiff = strtok(NULL, " ");
        int8 diff = -1;
        if (pDiff)
            diff = atoi(pDiff);
        uint16 counter = 0;
        uint16 MapId = 0;

        if (strcmp(map, "all"))
        {
            MapId = uint16(atoi(map));
            if (!MapId)
                return false;
        }

        for (uint8 i = 0; i < Difficulty::MaxDifficulties; ++i)
        {
            Player::BoundInstancesMap &binds = player->GetBoundInstances(Difficulty(i));
            for (Player::BoundInstancesMap::iterator itr = binds.begin(); itr != binds.end();)
            {
                InstanceSave* save = itr->second.save;
                if (itr->first != player->GetMapId() && (!MapId || MapId == itr->first) && (diff == -1 || diff == save->GetDifficultyID()))
                {
                    std::string timeleft = GetTimeString(save->GetResetTime() - time(NULL));
                    handler->PSendSysMessage("unbinding map: %d inst: %d perm: %s diff: %d canReset: %s TTR: %s", itr->first, save->GetInstanceId(), itr->second.perm ? "yes" : "no", save->GetDifficultyID(), save->CanReset() ? "yes" : "no", timeleft.c_str());
                    player->UnbindInstance(itr, Difficulty(i));
                    counter++;
                }
                else
                    ++itr;
            }
        }
        handler->PSendSysMessage("instances unbound: %d", counter);

        return true;
    }

    static bool HandleInstanceStatsCommand(ChatHandler* handler, char const* /*args*/)
    {
        handler->PSendSysMessage("instances loaded: %d", sMapMgr->GetNumInstances());
        handler->PSendSysMessage("players in instances: %d", sMapMgr->GetNumPlayersInInstances());
        handler->PSendSysMessage("instance saves: %d", sInstanceSaveMgr->GetNumInstanceSaves());
        handler->PSendSysMessage("players bound: %d", sInstanceSaveMgr->GetNumBoundPlayersTotal());
        handler->PSendSysMessage("groups bound: %d", sInstanceSaveMgr->GetNumBoundGroupsTotal());

        return true;
    }

    static bool HandleInstanceSaveDataCommand(ChatHandler* handler, char const* /*args*/)
    {
        Player* player = handler->GetSession()->GetPlayer();
        Map* map = player->GetMap();
        if (!map->IsDungeon())
        {
            handler->PSendSysMessage("Map is not a dungeon.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        if (!((InstanceMap*)map)->GetInstanceScript())
        {
            handler->PSendSysMessage("Map has no instance data.");
            handler->SetSentErrorMessage(true);
            return false;
        }

        ((InstanceMap*)map)->GetInstanceScript()->SaveToDB();

        return true;
    }
};

#ifndef __clang_analyzer__
void AddSC_instance_commandscript()
{
    new instance_commandscript();
}
#endif
