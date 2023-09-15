////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "LogWorker.h"

LogWorker::LogWorker()
    : m_queue(HIGH_WATERMARK, LOW_WATERMARK)
{
    ACE_Task_Base::activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED, 1);
}

LogWorker::~LogWorker()
{
    m_queue.deactivate();
    wait();
}

int LogWorker::enqueue(LogOperation* op)
{
    return m_queue.enqueue(op);
}

int LogWorker::svc()
{
    while (1)
    {
        LogOperation* request;
        if (m_queue.dequeue(request) == -1)
            break;

        request->call();
        delete request;
    }

    return 0;
}
