////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LFGMGR_H
#define _LFGMGR_H

#include "Common.h"
#include "LFG.h"
#include "LockedMap.h"
#include "LFGPlayerData.h"

class LfgGroupData;
class LfgPlayerData;
class Group;
class Player;

enum LFGenum
{
    LFG_TIME_ROLECHECK                           = 40*IN_MILLISECONDS,
    LFG_TIME_BOOT                                = 2*MINUTE,
    LFG_TIME_PROPOSAL                            = 2*MINUTE,
    LFG_TANKS_NEEDED                             = 1,
    LFG_HEALERS_NEEDED                           = 1,
    LFG_DPS_NEEDED                               = 3,
    LFG_QUEUEUPDATE_INTERVAL                     = 15*IN_MILLISECONDS,
    LFG_SPELL_DUNGEON_COOLDOWN                   = 71328,
    LFG_SPELL_DUNGEON_DESERTER                   = 71041,
    LFG_SPELL_LUCK_OF_THE_DRAW                   = 72221
};

enum LfgFlags
{
    LFG_FLAG_UNK1                                = 0x1,
    LFG_FLAG_UNK2                                = 0x2,
    LFG_FLAG_SEASONAL                            = 0x4,
    LFG_FLAG_UNK3                                = 0x8
};

/// Determines the type of instance
enum LfgType : uint8
{
    LFG_TYPE_NONE                                = 0,      // Internal use only
    TYPEID_DUNGEON                               = 1,
    TYPEID_RANDOM_DUNGEON                        = 6,

    LFG_SUBTYPEID_DUNGEON                        = 1,
    LFG_SUBTYPEID_HEROIC                         = 2,
    LFG_SUBTYPEID_RAID                           = 3,
    LFG_SUBTYPEID_SCENARIO                       = 4
};

enum LfgCategory
{
    LFG_CATEGORIE_NONE                          = 0,
    LFG_CATEGORIE_DUNGEON                       = 1,
    LFG_CATEGORIE_RAID                          = 2,
    LFG_CATEGORIE_SCENARIO                      = 3,
    LFG_CATEGORIE_DYNAMIC_RAID                  = 4
};

/// Proposal states
enum LfgProposalState : uint8
{
    LFG_PROPOSAL_INITIATING                      = 0,
    LFG_PROPOSAL_FAILED                          = 1,
    LFG_PROPOSAL_SUCCESS                         = 2
};

/// Teleport errors
enum LfgTeleportError : uint8
{
    // 1, 2, 7, 8 = "You can't do that right now" | 5 = No client reaction
    LFG_TELEPORTERROR_OK                         = 9,      // Internal use
    LFG_TELEPORTERROR_PLAYER_DEAD                = 14,
    LFG_TELEPORTERROR_FALLING                    = 5,
    LFG_TELEPORTERROR_DONT_REPORT                = 4,
    LFG_TELEPORTERROR_FATIGUE                    = 3,
    LFG_TELEPORTERROR_INVALID_LOCATION           = 0
};

/// Queue join results
enum LfgJoinResult : uint8
{
    // 3 = No client reaction | 18 = "Rolecheck failed"
    //44, 28 - leads to rolecheck has failed
    LFG_JOIN_OK                                  = 0x00,      // Joined (no client msg)
    LFG_JOIN_FAILED                              = 0x20,     // RoleCheck Failed
    LFG_JOIN_GROUPFULL                           = 0x1E,     // Your group is full
    LFG_JOIN_INTERNAL_ERROR                      = 0x20,     // Internal LFG Error
    LFG_JOIN_NOT_MEET_REQS                       = 0x21,     // You do not meet the requirements for the chosen dungeons
    LFG_JOIN_PARTY_NOT_MEET_REQS                 = 0x22,      // One or more party members do not meet the requirements for the chosen dungeons
    LFG_JOIN_MIXED_RAID_DUNGEON                  = 0x20,     // You cannot mix dungeons, raids, and random when picking dungeons
    LFG_JOIN_MULTI_REALM                         = 0x23,     // The dungeon you chose does not support players from multiple realms
    LFG_JOIN_DISCONNECTED                        = 0x24,     // One or more party members are pending invites or disconnected
    LFG_JOIN_PARTY_INFO_FAILED                   = 0x25,     // Could not retrieve information about some party members
    LFG_JOIN_DUNGEON_INVALID                     = 0x26,     // One or more dungeons was not valid
    LFG_JOIN_DESERTER                            = 0x27,     // You can not queue for dungeons until your deserter debuff wears off
    LFG_JOIN_PARTY_DESERTER                      = 0x28,     // One or more party members has a deserter debuff
    LFG_JOIN_RANDOM_COOLDOWN                     = 0x29,     // You can not queue for random dungeons while on random dungeon cooldown
    LFG_JOIN_PARTY_RANDOM_COOLDOWN               = 0x2A,     // One or more party members are on random dungeon cooldown
    LFG_JOIN_TOO_MUCH_MEMBERS                    = 0x2B,     // You can not enter dungeons with more that 5 party members
    LFG_JOIN_NOT_ENOUGH_MEMBERS                  = 0x33,     // You do not have enough group members to queue for that
    LFG_JOIN_USING_BG_SYSTEM                     = 0x2C,     // You can not use the dungeon system while in BG or arenas
    LFG_JOIN_QUEUED_TO_MANY_INSTANCES            = 0x34      // You are queued for too many instances
};

/// Role check states
enum LfgRoleCheckState : uint8
{
    LFG_ROLECHECK_DEFAULT                        = 0,      // Internal use = Not initialized.
    LFG_ROLECHECK_FINISHED                       = 1,      // Role check finished
    LFG_ROLECHECK_INITIALITING                   = 2,      // Role check begins
    LFG_ROLECHECK_MISSING_ROLE                   = 3,      // Someone didn't selected a role after 2 mins
    LFG_ROLECHECK_WRONG_ROLES                    = 4,      // Can't form a group with that role selection
    LFG_ROLECHECK_ABORTED                        = 5,      // Someone leave the group
    LFG_ROLECHECK_NO_ROLE                        = 6       // Someone selected no role
};

/// Answer state (Also used to check compatibilites)
enum LfgAnswer
{
    LFG_ANSWER_PENDING                           = -1,
    LFG_ANSWER_DENY                              = 0,
    LFG_ANSWER_AGREE                             = 1
};

enum LfgSlotRandomDungeonID
{
    LfgRandomClassicDungeon         = 258,
    LfgRandomBurningCrusadeDungeon  = 259,
    LfgRandomBurningCrusadeHeroic   = 260,
    LfgRandomLichKingDungeon        = 261,
    LfgRandomLichKingHeroic         = 262,
    LfgRandomCataclysmDungeon       = 300,
    LfgRandomCataclysmHeroic        = 301,
    LfgRandomMopDungeon             = 462,
    LfgRandomMopHeroic              = 463,
    LfgRandomHourOfTwilightHeoic    = 434,
    LfgRandomWodDungeon             = 788,
    LfgRandomWodHeroic              = 789,
    LfgRandomMopScenario            = 493,
    LfgRandomMopHeroicScenario      = 641,
    LfgRandomTimewalkingDungeonBC   = 744,
    LfgRandomTimewalkingDungeonTLK  = 995,
    LfgRandomTimewalkingDungeonCata = 1146
};

enum LfgGroupType
{
    LfgGroupTypeNone                     = 0,
    LfgGroupeTypeClassic                 = 1,
    LfgGroupeTypeDungeonBC               = 2,
    LfgGroupeTypeHeroicBC                = 3,
    LfgGroupeTypeDungeonTLK              = 4,
    LfgGroupeTypeHeroicTLK               = 5,
    LfgGroupeTypeHeroicCataclysm         = 12,
    LfgGroupeTypeDungeonCataclysm        = 13,
    LfgGroupeTypeHeroicHourOfTwilight    = 33,
    LfgGroupeTypeHeroicMop               = 36,
    LfgGroupeTypeDungeonMop              = 37,
    LfgGroupeTypeScenarioMop             = 38,
    LfgGroupeTypeHeroicScenarioMop       = 43,
    LfgGroupeTypeTimeWalking             = 44,
    LfgGroupeTypeDungeonWod              = 47,
    LfgGroupeTypeHeroicWod               = 48
};

// Forward declaration (just to have all typedef together)
struct LfgReward;
struct LfgLockStatus;
struct LfgQueueInfo;
struct LfgRoleCheck;
struct LfgProposal;
struct LfgProposalPlayer;
struct LfgPlayerBoot;
class LfgPlayerData;

typedef std::set<uint64> LfgGuidSet;
typedef std::list<uint64> LfgGuidList;
typedef std::map<uint8, LfgGuidList> LfgGuidListMap;
typedef std::set<Player*> PlayerSet;
typedef std::list<Player*> LfgPlayerList;
typedef std::map<uint32, LfgReward const*> LfgRewardMap;
typedef std::map<std::string, LfgAnswer> LfgCompatibleMap;
typedef std::map<uint64, LfgDungeonSet> LfgDungeonMap;
typedef std::map<uint64, uint8> LfgRolesMap;
typedef std::map<uint64, LfgAnswer> LfgAnswerMap;
typedef std::map<uint64, LfgRoleCheck*> LfgRoleCheckMap;
typedef std::map<uint64, LfgQueueInfo*> LfgQueueInfoMap;
typedef std::map<uint32, LfgProposal*> LfgProposalMap;
typedef std::map<uint64, LfgProposalPlayer*> LfgProposalPlayerMap;
typedef std::map<uint32, LfgPlayerBoot*> LfgPlayerBootMap;
typedef std::map<uint64, LfgGroupData> LfgGroupDataMap;
typedef std::map<uint32, Position> LfgEntrancePositionMap;
typedef ACE_Based::LockedMap<uint64, LfgPlayerData> LfgPlayerDataMap;

// Data needed by SMSG_LFG_JOIN_RESULT
struct LfgJoinResultData
{
    LfgJoinResultData(LfgJoinResult _result = LFG_JOIN_OK, LfgRoleCheckState _state = LFG_ROLECHECK_DEFAULT):
        result(_result), state(_state) {}
    LfgJoinResult result;
    LfgRoleCheckState state;
    LfgLockPartyMap lockmap;
};

// Data needed by SMSG_LFG_UPDATE_PARTY and SMSG_LFG_UPDATE_PLAYER
struct LfgUpdateData
{
    LfgUpdateData(LfgUpdateType _type = LFG_UPDATETYPE_DEFAULT): updateType(_type), comment("") {}
    LfgUpdateData(LfgUpdateType _type, const LfgDungeonSet& _dungeons, std::string _comment):
        updateType(_type), dungeons(_dungeons), comment(_comment) {}

    LfgUpdateType updateType;
    LfgDungeonSet dungeons;
    std::string comment;
};

/// Reward info
struct LfgReward
{
    uint32 maxLevel;
    struct
    {
        uint32 questId;
        uint32 variableMoney;
        uint32 variableXP;
    } reward[2];

    LfgReward(uint32 _maxLevel = 0, uint32 firstQuest = 0, uint32 firstVarMoney = 0, uint32 firstVarXp = 0, uint32 otherQuest = 0, uint32 otherVarMoney = 0, uint32 otherVarXp = 0)
        : maxLevel(_maxLevel)
    {
        reward[0].questId = firstQuest;
        reward[0].variableMoney = firstVarMoney;
        reward[0].variableXP = firstVarXp;
        reward[1].questId = otherQuest;
        reward[1].variableMoney = otherVarMoney;
        reward[1].variableXP = otherVarXp;
    }
};

/// Stores player or group queue info
struct LfgQueueInfo
{
    LfgQueueInfo(): joinTime(0), tanks(LFG_TANKS_NEEDED), healers(LFG_HEALERS_NEEDED), dps(LFG_DPS_NEEDED), category(0) {};
    time_t joinTime;                                       ///< Player queue join time (to calculate wait times)
    uint8 tanks;                                           ///< Tanks needed
    uint8 healers;                                         ///< Healers needed
    uint8 dps;                                             ///< Dps needed
    LfgDungeonSet dungeons;                                ///< Selected Player/Group Dungeon/s
    LfgRolesMap roles;                                     ///< Selected Player Role/s
    uint8 type;
    uint8 category;
};

/// Stores player data related to proposal to join
struct LfgProposalPlayer
{
    LfgProposalPlayer(): role(0), accept(LFG_ANSWER_PENDING), groupLowGuid(0) {};
    uint8 role;                                            ///< Proposed role
    LfgAnswer accept;                                      ///< Accept status (-1 not answer | 0 Not agree | 1 agree)
    uint32 groupLowGuid;                                   ///< Original group guid (Low guid) 0 if no original group
};

/// Stores group data related to proposal to join
struct LfgProposal
{
    LfgProposal(uint32 dungeon = 0): dungeonId(dungeon), state(LFG_PROPOSAL_INITIATING), groupLowGuid(0), leader(0), cancelTime(0) {}

    ~LfgProposal()
    {
        for (LfgProposalPlayerMap::iterator it = players.begin(); it != players.end(); ++it)
            delete it->second;
    };
    uint32 dungeonId;                                      ///< Dungeon to join
    LfgProposalState state;                                ///< State of the proposal
    uint32 groupLowGuid;                                   ///< Proposal group (0 if new)
    uint64 leader;                                         ///< Leader guid.
    time_t cancelTime;                                     ///< Time when we will cancel this proposal
    LfgGuidList queues;                                    ///< Queue Ids to remove/readd
    LfgProposalPlayerMap players;                          ///< Players data

};

/// Stores all rolecheck info of a group that wants to join
struct LfgRoleCheck
{
    time_t cancelTime;                                     ///< Time when the rolecheck will fail
    LfgRolesMap roles;                                     ///< Player selected roles
    LfgRoleCheckState state;                               ///< State of the rolecheck
    LfgDungeonSet dungeons;                                ///< Dungeons group is applying for (expanded random dungeons)
    uint32 rDungeonId;                                     ///< Random Dungeon Id.
    uint64 leader;                                         ///< Leader of the group
};

/// Stores information of a current vote to kick someone from a group
struct LfgPlayerBoot
{
    time_t cancelTime;                                     ///< Time left to vote
    bool inProgress;                                       ///< Vote in progress
    LfgAnswerMap votes;                                    ///< Player votes (-1 not answer | 0 Not agree | 1 agree)
    uint64 victim;                                         ///< Player guid to be kicked (can't vote)
    uint8 votedNeeded;                                     ///< Votes needed to kick the player
    std::string reason;                                    ///< kick reason
};

class LFGMgr
{
    friend class ACE_Singleton<LFGMgr, ACE_Null_Mutex>;

    private:
        LFGMgr();
        ~LFGMgr();

    public:
        void Update(uint32 diff);

        // Reward
        void LoadRewards();
        void RewardDungeonDoneFor(const uint32 dungeonId, Player* player);
        LfgReward const* GetRandomDungeonReward(uint32 dungeon, uint8 level);

        // Queue
        void Join(Player* player, uint8 roles, const LfgDungeonSet& dungeons, const std::string& comment);
        void Leave(Player* player, Group* grp = NULL);

        // Role Check
        void UpdateRoleCheck(uint64 gguid, uint64 guid = 0, uint8 roles = LFG_ROLEMASK_NONE);

        // Proposals
        void UpdateProposal(uint32 proposalId, uint64 guid, bool accept);

        // Teleportation
        void LoadEntrancePositions();
        void TeleportPlayer(Player* player, bool out, bool fromOpcode = false);

        // Vote kick
        void InitBoot(Group* grp, uint64 kguid, uint64 vguid, std::string reason);
        void UpdateBoot(Player* player, bool accept);
        void OfferContinue(Group* grp);

        HolidayIds GetDungeonSeason(uint32 dungeonId);

        void InitializeLockedDungeons(Player* player);

        void _LoadFromDB(Field* fields, uint64 guid);
        void _SaveToDB(uint64 guid, uint32 db_guid);

        void SetComment(uint64 guid, const std::string& comment);
        const LfgLockMap& GetLockedDungeons(uint64 guid);
        LfgState GetState(uint64 guid);
        const LfgDungeonSet& GetSelectedDungeons(uint64 guid);
        uint32 GetDungeon(uint64 guid, bool asId = true);
        void SetState(uint64 guid, LfgState state);
        void ClearState(uint64 guid);
        void RemovePlayerData(uint64 guid);
        void RemoveGroupData(uint64 guid);
        uint8 GetKicksLeft(uint64 gguid);
        uint8 GetVotesNeeded(uint64 gguid);
        bool IsTeleported(uint64 pguid);
        void SetRoles(uint64 guid, uint8 roles);
        void SetSelectedDungeons(uint64 guid, const LfgDungeonSet& dungeons);
        LfgUpdateData GetLfgStatus(uint64 guid);

        void SendUpdateStatus(Player*, const LfgUpdateData& updateData);

        LfgQueueInfo* GetLfgQueueInfo(uint64 guid) const
        {
            LfgQueueInfoMap::const_iterator itr = m_QueueInfoMap.find(guid);
            if (itr != m_QueueInfoMap.end())
                return itr->second;

            return NULL;
        }

        bool IsInDebug() const { return m_debug; }
        void SetDebug(bool p_Value) { m_debug = p_Value; }

        /////////////////////////////////////////////
        /// LFR
        /////////////////////////////////////////////
        /// This function automatically send personal items to players
        void AutomaticLootDistribution(Creature* p_Creature, Group* p_Group);
        /// This function automatically add personal loots of players on the creature
        void AutomaticLootAssignation(Creature* p_Creature, Group* p_Group);
        /// This function returns ItemTemplate corresponding to the default Mop LFR loot
        ItemTemplate const* GetDefaultAutomaticLootItem(Creature* p_Creature);
        /// This function returns ItemID corresponding to the WoD LFR runes
        uint32 GetAugmentRuneID(Player const* p_Player) const;

    private:

        uint8 GetRoles(uint64 guid);
        const std::string& GetComment(uint64 gguid);
        void RestoreState(uint64 guid);
        void SetDungeon(uint64 guid, uint32 dungeon);
        void SetLockedDungeons(uint64 guid, const LfgLockMap& lock);
        void DecreaseKicksLeft(uint64 guid);

        // Queue
        void AddToQueue(uint64 guid, uint8 queueId);
        bool RemoveFromQueue(uint64 guid);

        // Proposals
        void RemoveProposal(LfgProposalMap::iterator itProposal, LfgUpdateType type);

        // Group Matching
        LfgProposal* FindNewGroups(LfgGuidList& check, LfgGuidList& all, LfgCategory type);
        bool CheckGroupRoles(LfgRolesMap &groles, LfgCategory type, bool removeLeaderFlag = true);
        bool CheckCompatibility(LfgGuidList check, LfgProposal*& pProposal, LfgCategory type);
        void GetCompatibleDungeons(LfgDungeonSet& dungeons, const PlayerSet& players, LfgLockPartyMap& lockMap);
        void SetCompatibles(std::string const& concatenatedGuids, bool compatibles);
        LfgAnswer GetCompatibles(std::string const& concatenatedGuids);
        void RemoveFromCompatibles(uint64 guid);
        LfgProposal* CheckForSingle(LfgGuidList& check);

        // Generic
        const LfgDungeonSet& GetDungeonsByRandom(uint32 randomdungeon, bool check = false);
        LfgType GetDungeonType(uint32 dungeon);
        LfgCategory GetLfgCategorie(uint32 dungeon);
        std::string ConcatenateGuids(LfgGuidList const& check);

        // General variablesUpdateProposal
        bool m_debug;                                      ///< Num of players minimum is 1, for debug only (.lfg debug command)
        bool m_update;                                     ///< Doing an update?
        uint32 m_QueueTimer;                               ///< used to check interval of update
        uint32 m_lfgProposalId;                            ///< used as internal counter for proposals
        int32 m_WaitTimeAvg;                               ///< Average wait time to find a group queuing as multiple roles
        int32 m_WaitTimeTank;                              ///< Average wait time to find a group queuing as tank
        int32 m_WaitTimeHealer;                            ///< Average wait time to find a group queuing as healer
        int32 m_WaitTimeDps;                               ///< Average wait time to find a group queuing as dps
        uint32 m_NumWaitTimeAvg;                           ///< Num of players used to calc avs wait time
        uint32 m_NumWaitTimeTank;                          ///< Num of players used to calc tank wait time
        uint32 m_NumWaitTimeHealer;                        ///< Num of players used to calc healers wait time
        uint32 m_NumWaitTimeDps;                           ///< Num of players used to calc dps wait time
        LfgDungeonMap m_CachedDungeonMap;                  ///< Stores all dungeons by groupType
        LfgDungeonSet m_InvalidDungeons;                   ///< Stores dungeons which don't fill conditions for AccessRequirement, LFRAccessRequirement, LfgEntrancePositionMap
        LfgEntrancePositionMap m_entrancePositions;        ///< Stores special entrance positions
        // Reward System
        LfgRewardMap m_RewardMap;                          ///< Stores rewards for random dungeons
        // Queue
        LfgQueueInfoMap m_QueueInfoMap;                    ///< Queued groups
        LfgGuidListMap m_currentQueue;                     ///< Ordered list. Used to find groups
        LfgGuidListMap m_newToQueue;                       ///< New groups to add to queue
        LfgCompatibleMap m_CompatibleMap;                  ///< Compatible dungeons
        LfgGuidList m_teleport;                            ///< Players being teleported
        // Rolecheck - Proposal - Vote Kicks
        LfgRoleCheckMap m_RoleChecks;                      ///< Current Role checks
        LfgProposalMap m_Proposals;                        ///< Current Proposals
        LfgPlayerBootMap m_Boots;                          ///< Current player kicks
        LfgPlayerDataMap m_Players;                        ///< Player data
        LfgGroupDataMap m_Groups;                          ///< Group data
};

#define sLFGMgr ACE_Singleton<LFGMgr, ACE_Null_Mutex>::instance()
#endif
