////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WORKERTHREAD_H
#define _WORKERTHREAD_H

#include <ace/Task.h>
#include <ace/Activation_Queue.h>

class MySQLConnection;

class DatabaseWorker : protected ACE_Task_Base
{
    public:
        DatabaseWorker(ACE_Activation_Queue* new_queue, MySQLConnection* con);

        ///- Inherited from ACE_Task_Base
        int svc();
        int wait() { return ACE_Task_Base::wait(); }

    private:
        DatabaseWorker() : ACE_Task_Base() {}
        ACE_Activation_Queue* m_queue;
        MySQLConnection* m_conn;
};

#endif
