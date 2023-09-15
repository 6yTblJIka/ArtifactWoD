////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _WARDEN_MAC_H
#define _WARDEN_MAC_H

#include "Cryptography/ARC4.h"
#include "Common.h"
#include "Cryptography/BigNumber.h"
#include "ByteBuffer.h"
#include "Warden.h"

class WorldSession;
class Warden;

class WardenMac : public Warden
{
    public:
        WardenMac();
        ~WardenMac();

        void Init(WorldSession* session, BigNumber* k);
        ClientWardenModule* GetModuleForClient();
        void InitializeModule();
        void RequestHash();
        void HandleHashResult(ByteBuffer& buff);
        void RequestData();
        void HandleData(ByteBuffer& buff);
};

#endif
