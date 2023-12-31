////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

/// \addtogroup Trinityd
/// @{
/// \file

#include "Common.h"
#include "ObjectMgr.h"
#include "World.h"
#include "WorldSession.h"
#include "Configuration/Config.h"

#include "AccountMgr.h"
#include "Chat.h"
#include "CliRunnable.h"
#include "Language.h"
#include "Log.h"
#include "MapManager.h"
#include "Player.h"
#include "Util.h"

#if PLATFORM != PLATFORM_WINDOWS
#include <readline/readline.h>
#include <readline/history.h>

char * command_finder(const char* text, int state)
{
    static int idx, len;
    const char* ret;
    ChatCommand* cmd = ChatHandler::getCommandTable();

    if (!state)
    {
        idx = 0;
        len = strlen(text);
    }

    while ((ret = cmd[idx].Name))
    {
        if (!cmd[idx].AllowConsole)
        {
            idx++;
            continue;
        }

        idx++;
        //printf("Checking %s \n", cmd[idx].Name);
        if (strncmp(ret, text, len) == 0)
            return strdup(ret);
        if (cmd[idx].Name == NULL)
            break;
    }

    return ((char*)NULL);
}

char ** cli_completion(const char * text, int start, int /*end*/)
{
    char ** matches;
    matches = (char**)NULL;

    if (start == 0)
        matches = rl_completion_matches((char*)text, &command_finder);
/*#ifdef PLATFORM != PLATFORM_APPLE
    else
        rl_bind_key('\t', rl_abort);
#endif*/
    return (matches);
}

int cli_hook_func(void)
{
#if PLATFORM != PLATFORM_APPLE
       if (World::IsStopped())
           rl_done = 1;
#endif
       return 0;
}

#endif

void utf8print(void* /*arg*/, const char* str)
{
#if PLATFORM == PLATFORM_WINDOWS
    wchar_t wtemp_buf[6000];
    size_t wtemp_len = 6000-1;
    if (!Utf8toWStr(str, strlen(str), wtemp_buf, wtemp_len))
        return;

    char temp_buf[6000];
    CharToOemBuffW(&wtemp_buf[0], &temp_buf[0], wtemp_len+1);
    printf(temp_buf);
#else
{
    printf("%s", str);
    fflush(stdout);
}
#endif
}

void commandFinished(void*, bool /*success*/)
{
    printf("TC> ");
    fflush(stdout);
}

#ifdef linux
// Non-blocking keypress detector, when return pressed, return 1, else always return 0
int kb_hit_return()
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
#endif

/// %Thread start
void CliRunnable::run()
{
    ///- Display the list of available CLI functions then beep
    //sLog->outInfo(LOG_FILTER_WORLDSERVER, "");
#if PLATFORM != PLATFORM_WINDOWS
    rl_attempted_completion_function = cli_completion;
    #if PLATFORM != PLATFORM_APPLE
    rl_event_hook = cli_hook_func;
    #endif
#endif

    if (ConfigMgr::GetBoolDefault("BeepAtStart", true))
        printf("\a");                                       // \a = Alert

    // print this here the first time
    // later it will be printed after command queue updates
    printf("TC>");

    ///- As long as the World is running (no World::m_stopEvent), get the command line and handle it
    while (!World::IsStopped())
    {
        fflush(stdout);

        char *command_str ;             // = fgets(commandbuf, sizeof(commandbuf), stdin);

#if PLATFORM == PLATFORM_WINDOWS
        char commandbuf[256];
        command_str = fgets(commandbuf, sizeof(commandbuf), stdin);
#else
        command_str = readline("TC>");
        rl_bind_key('\t', rl_complete);
#endif

        if (command_str != NULL)
        {
            for (int x=0; command_str[x]; ++x)
                if (command_str[x] == '\r' || command_str[x] == '\n')
                {
                    command_str[x] = 0;
                    break;
                }

            if (!*command_str)
            {
#if PLATFORM == PLATFORM_WINDOWS
                printf("TC>");
#else
                free(command_str);
#endif
                continue;
            }

            std::string command;
            if (!consoleToUtf8(command_str, command))         // convert from console encoding to utf8
            {
#if PLATFORM == PLATFORM_WINDOWS
                printf("TC>");
#else
                free(command_str);
#endif
                continue;
            }

            fflush(stdout);
            sWorld->QueueCliCommand(new CliCommandHolder(NULL, command.c_str(), &utf8print, &commandFinished));
#if PLATFORM != PLATFORM_WINDOWS
            add_history(command.c_str());
            free(command_str);
#endif
        }
        else if (feof(stdin))
        {
            World::StopNow(SHUTDOWN_EXIT_CODE);
        }
    }
}
