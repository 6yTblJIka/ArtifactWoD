////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
//  Copyright 2014 ArtifactCore based on TC 6.2.X SARL
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MS_SIGNAL_HANDLER_H
#define __MS_SIGNAL_HANDLER_H

#include "Common.h"

namespace MS
{
    namespace SignalHandler
    {
        const static uint32 gFailureCountLimit = 10;                            ///< How many signal can be receive by the signal handler before shutdown the server

        static thread_local bool gThrowExceptionAtFailure = false;              ///< Store per thread if the current context can be rescued
        static uint32 gFailureCounter = 0;                                      ///< Counter of signal receive by the signal handler

        void EnableThrowExceptionAtFailure();
        void DisableThrowExceptionAtFailure();
        void OnSignalReceive(int p_Signal);
    }
}

#endif
