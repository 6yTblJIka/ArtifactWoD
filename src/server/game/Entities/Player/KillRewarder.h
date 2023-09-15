////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
//  Copyright 2015 ArtifactCore based on TC 6.2.X SARL
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef KillRewarder_h__
#define KillRewarder_h__

#include "Define.h"

class Player;
class Unit;
class Group;

class KillRewarder
{
    public:
        KillRewarder(Player* killer, Unit* victim, bool isBattleGround);
    
        void Reward();
    
    private:
        void _InitXP(Player* player);
        void _InitGroupData();
    
        void _RewardHonor(Player* player);
        void _RewardXP(Player* player, float rate);
        void _RewardReputation(Player* player, float rate);
        void _RewardKillCredit(Player* player);
        void _RewardPlayer(Player* player, bool isDungeon);
        void _RewardGroup();
    
        Player* _killer;
        Unit* _victim;
        Group* _group;
        float _groupRate;
        Player* _maxNotGrayMember;
        uint32 _count;
        uint32 _sumLevel;
        uint32 _xp;
        bool _isFullXP;
        uint8 _maxLevel;
        bool _isBattleGround;
        bool _isPvP;
};
#endif // KillRewarder_h__
