////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BATTLEGROUNDRBG_H
#define __BATTLEGROUNDRBG_H

class Battleground;

class BattlegroundRBGScore : public BattlegroundScore
{
    public:
        BattlegroundRBGScore() {};
        virtual ~BattlegroundRBGScore() {};
};

class BattlegroundRBG : public Battleground
{
    public:
        BattlegroundRBG();
        ~BattlegroundRBG();

        virtual void AddPlayer(Player* player);
        virtual void StartingEventCloseDoors();
        virtual void StartingEventOpenDoors();

        void RemovePlayer(Player* player, uint64 guid, uint32 team);
        void HandleAreaTrigger(Player* Source, uint32 Trigger);

        /* Scorekeeping */
        void UpdatePlayerScore(Player* p_Source, Player* p_Victim, uint32 p_Type, uint32 p_Value, bool p_DoAddHonor = true, MS::Battlegrounds::RewardCurrencyType::Type p_RewardType = MS::Battlegrounds::RewardCurrencyType::Type::None);

    private:
};
#endif
