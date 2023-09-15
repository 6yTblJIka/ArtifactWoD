////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef LOGWORKER_H
#define LOGWORKER_H

#include "LogOperation.h"

#include <ace/Task.h>
#include <ace/Activation_Queue.h>

class LogWorker: protected ACE_Task_Base
{
    public:
        LogWorker();
        ~LogWorker();

        typedef ACE_Message_Queue_Ex<LogOperation, ACE_MT_SYNCH> LogMessageQueueType;

        enum
        {
            HIGH_WATERMARK = 8 * 1024 * 1024,
            LOW_WATERMARK  = 8 * 1024 * 1024
        };

        int enqueue(LogOperation *op);

    private:
        virtual int svc();
        LogMessageQueueType m_queue;
};

#endif
