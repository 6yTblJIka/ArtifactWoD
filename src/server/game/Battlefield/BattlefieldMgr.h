////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BATTLEFIELD_MGR_H_
#define BATTLEFIELD_MGR_H_

#include "Battlefield.h"
#include "ace/Singleton.h"

class Player;
class GameObject;
class Creature;
class ZoneScript;
struct GossipMenuItems;

// class to handle player enter / leave / areatrigger / GO use events
class BattlefieldMgr
{
  public:
    // ctor
    BattlefieldMgr();
    // dtor
    ~BattlefieldMgr();

    // create battlefield events
    void InitBattlefield();
    // called when a player enters an battlefield area
    void HandlePlayerEnterZone(Player * player, uint32 areaflag);
    // called when player leaves an battlefield area
    void HandlePlayerLeaveZone(Player * player, uint32 areaflag);
    // called when player resurrects
    void HandlePlayerResurrects(Player * player, uint32 areaflag);
    // return assigned battlefield
    Battlefield *GetBattlefieldToZoneId(uint32 zoneid);
    Battlefield *GetBattlefieldByBattleId(uint32 battleid);
    Battlefield *GetBattlefieldByGUID(uint64 guid);

    ZoneScript *GetZoneScript(uint32 zoneId);

    void AddZone(uint32 zoneid, Battlefield * handle);

    void Update(uint32 diff);

    void HandleGossipOption(Player * player, uint64 guid, uint32 gossipid);

    bool CanTalkTo(Player * player, Creature * creature, GossipMenuItems gso);

    void HandleDropFlag(Player * player, uint32 spellId);

    typedef std::vector < Battlefield * >BattlefieldSet;
    typedef std::map < uint32 /* zoneid */ , Battlefield * >BattlefieldMap;
  private:
    // contains all initiated battlefield events
    // used when initing / cleaning up
      BattlefieldSet m_BattlefieldSet;
    // maps the zone ids to an battlefield event
    // used in player event handling
    BattlefieldMap m_BattlefieldMap;
    // update interval
    uint32 m_UpdateTimer;
};

#define sBattlefieldMgr ACE_Singleton<BattlefieldMgr, ACE_Null_Mutex>::instance()

#endif
