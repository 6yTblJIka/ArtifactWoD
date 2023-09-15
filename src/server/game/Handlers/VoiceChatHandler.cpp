////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"

void WorldSession::HandleVoiceSessionEnableOpcode(WorldPacket & p_Packet)
{
    p_Packet.rfinish();
}

void WorldSession::HandleChannelVoiceOnOpcode(WorldPacket & p_Packet)
{
    p_Packet.rfinish();
}

void WorldSession::HandleSetActiveVoiceChannel(WorldPacket& p_Packet)
{
    p_Packet.rfinish();
}

