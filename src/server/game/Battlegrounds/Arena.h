////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ARENATEAM_H
#define ARENATEAM_H

#include "QueryResult.h"
#include "Common.h"

class WorldSession;
class WorldPacket;
class Player;
class Group;

enum ArenaTeamCommandTypes
{
    ERR_ARENA_TEAM_CREATE_S                 = 0x00,
    ERR_ARENA_TEAM_INVITE_SS                = 0x01,
    ERR_ARENA_TEAM_QUIT_S                   = 0x03,
    ERR_ARENA_TEAM_FOUNDER_S                = 0x0E
};

enum ArenaType
{
    None      = 0,
    Arena2v2  = 2,
    Arena3v3  = 3,
    Arena5v5  = 5
};

enum ArenaTeamCommandErrors
{
    ERR_ARENA_TEAM_CREATED                  = 0x01,
    ERR_ALREADY_IN_ARENA_TEAM               = 0x02,
    ERR_ALREADY_IN_ARENA_TEAM_S             = 0x03,
    ERR_INVITED_TO_ARENA_TEAM               = 0x04,
    ERR_ALREADY_INVITED_TO_ARENA_TEAM_S     = 0x05,
    ERR_ARENA_TEAM_NAME_INVALID             = 0x06,
    ERR_ARENA_TEAM_NAME_EXISTS_S            = 0x07,
    ERR_ARENA_TEAM_LEADER_LEAVE_S           = 0x08,
    ERR_ARENA_TEAM_PERMISSIONS              = 0x08,
    ERR_ARENA_TEAM_PLAYER_NOT_IN_TEAM       = 0x09,
    ERR_ARENA_TEAM_PLAYER_NOT_IN_TEAM_SS    = 0x0A,
    ERR_ARENA_TEAM_PLAYER_NOT_FOUND_S       = 0x0B,
    ERR_ARENA_TEAM_NOT_ALLIED               = 0x0C,
    ERR_ARENA_TEAM_IGNORING_YOU_S           = 0x13,
    ERR_ARENA_TEAM_TARGET_TOO_LOW_S         = 0x15,
    ERR_ARENA_TEAM_TARGET_TOO_HIGH_S        = 0x16,
    ERR_ARENA_TEAM_TOO_MANY_MEMBERS_S       = 0x17,
    ERR_ARENA_TEAM_NOT_FOUND                = 0x1B,
    ERR_ARENA_TEAMS_LOCKED                  = 0x1E,
    ERR_ARENA_TEAM_TOO_MANY_CREATE          = 0x21
};

enum ArenaTeamEvents
{
    ERR_ARENA_TEAM_JOIN_SS                  = 4,            // player name + arena team name
    ERR_ARENA_TEAM_LEAVE_SS                 = 5,            // player name + arena team name
    ERR_ARENA_TEAM_REMOVE_SSS               = 6,            // player name + arena team name + captain name
    ERR_ARENA_TEAM_LEADER_IS_SS             = 7,            // player name + arena team name
    ERR_ARENA_TEAM_LEADER_CHANGED_SSS       = 8,            // old captain + new captain + arena team name
    ERR_ARENA_TEAM_DISBANDED_S              = 9             // captain name + arena team name
};

/*
need info how to send these ones:
ERR_ARENA_TEAM_YOU_JOIN_S - client show it automatically when accept invite
ERR_ARENA_TEAM_TARGET_TOO_LOW_S
ERR_ARENA_TEAM_TOO_MANY_MEMBERS_S
ERR_ARENA_TEAM_LEVEL_TOO_LOW_I
*/

enum ArenaTeamTypes
{
    ARENA_TEAM_2v2      = 2,
    ARENA_TEAM_3v3      = 3,
    ARENA_TEAM_5v5      = 5
};

struct ArenaTeamMember
{
    uint64 Guid;
    std::string Name;
    uint8 Class;
    uint16 WeekGames;
    uint16 WeekWins;
    uint16 SeasonGames;
    uint16 SeasonWins;
    uint16 PersonalRating;
    uint16 MatchMakerRating;

    void ModifyPersonalRating(Player* player, int32 mod, uint32 type);
    void ModifyMatchmakerRating(int32 mod, uint32 slot);
};

struct ArenaTeamStats
{
    uint16 Rating;
    uint16 WeekGames;
    uint16 WeekWins;
    uint16 SeasonGames;
    uint16 SeasonWins;
    uint32 Rank;
};

enum ArenaSlots
{
    SLOT_ARENA_2V2 = 0,
    SLOT_ARENA_3V3 = 1,
    SLOT_ARENA_5V5 = 2,
    MAX_ARENA_SLOT = 3,
    SLOT_RBG       = 3,
    MAX_PVP_SLOT   = 6
};

#define MAX_ARENA_TYPE 6                                    // type : 2, 3 or 5

namespace Arena
{
    const float g_PvpMinCPPerWeek = 1500.0f;
    const float g_PvpMaxCPPerWeek = 3000.0f;
    const float g_PvpCPNumerator = 1511.26f;
    const float g_PvpCPBaseCoefficient = 1639.28f;
    const float g_PvpCPExpCoefficient = 0.00412f;

    inline float CalculateRatingFactor(int p_Rating)
    {
        return g_PvpCPNumerator / (expf(p_Rating * g_PvpCPExpCoefficient * -1.0f) * g_PvpCPBaseCoefficient + 1.0f);
    }

    inline uint32 GetConquestCapFromRating(int p_Rating)
    {
        float v2;
        float v3;
        float v4;
        float v5;

        if (g_PvpMinCPPerWeek )
        {
            v5 = CalculateRatingFactor(1500);
            v3 = CalculateRatingFactor(3000);
            v4 = CalculateRatingFactor(p_Rating);

            v2 = v5 <= v4 ? (v4 > v3 ? v3 : v4) : v5;
            return uint32(g_PvpMinCPPerWeek + floor(((v2 - v5) / (v3 - v5)) * (float)(g_PvpMaxCPPerWeek - g_PvpMinCPPerWeek)));
        }
        else
            return 0;
    }

    inline uint8 GetSlotByType(uint32 type)
    {
        switch (type)
        {
            case ARENA_TEAM_2v2: return SLOT_ARENA_2V2;
            case ARENA_TEAM_3v3: return SLOT_ARENA_3V3;
            case ARENA_TEAM_5v5: return SLOT_ARENA_5V5;
            default:
                break;
        }
        sLog->outError(LOG_FILTER_ARENAS, "FATAL: Unknown arena team type %u for some arena team", type);
        return 0xFF;
    }

    inline ArenaType GetTypeBySlot(uint8 slot)
    {
        switch (slot)
        {
            case 0:
                return ArenaType::Arena2v2;
            case 1:
                return ArenaType::Arena3v3;
            case 2:
                return ArenaType::Arena5v5;
            default:
                break;
        }
        sLog->outError(LOG_FILTER_ARENAS, "FATAL: Unknown arena team slot %u for some arena team", slot);
        return ArenaType::None;
    }

    inline float GetChanceAgainst(uint32 ownRating, uint32 opponentRating)
    {
        // Returns the chance to win against a team with the given rating, used in the rating adjustment calculation
        // ELO system
        return 1.0f / (1.0f + exp(log(10.0f) * (float)((float)opponentRating - (float)ownRating) / 650.0f));
    }

    inline int32 GetRatingMod(uint32 ownRating, uint32 opponentRating, bool won /*, float confidence_factor*/, bool rbg = false)
    {
        // 'Chance' calculation - to beat the opponent
        // This is a simulation. Not much info on how it really works
        float chance = GetChanceAgainst(ownRating, opponentRating);
        float won_mod = (won) ? 1.0f : 0.0f;
		int differenceInRating = opponentRating - ownRating;

        // Calculate the rating modification
        float k;

        if (rbg)
        {
            if (won && ownRating < 1500)
                k = 192.0f * (won_mod - chance);
            else
                k = 24.0f * (won_mod - chance);

            return (int32)ceil(k);
        }

		k = 96.0f - (won_mod - chance);
        // TODO: Replace this hack with using the confidence factor (limiting the factor to 2.0f)
		if (won)
		{
			if (ownRating >= 1500 && ownRating < 1750)
				k = 40.0f;
			else if (ownRating >= 1750 && ownRating < 2500)
				k = 32.0f;
			else if (ownRating >= 2500 && ownRating < 2750)
				k = 20.0f;
			else if (ownRating >= 2750 && ownRating < 3000)
				k = 15.0f;
			else if (ownRating >= 3000)
				k = 10.0f;

			if (ownRating > 1000)
			{
				if (differenceInRating > 500)
					k += 20;
				else if (differenceInRating > 250)
					k += 10;
			}
		}
		else
			if (ownRating >= 1500 && ownRating<1750)
				k = 20.0f;
			else if (ownRating >= 1750 && ownRating<2000)
				k = 25.0f;
			else if (ownRating >= 2000)
				k = 32.0f;

		if (k > 96.0f)
			k = 96.0f;

		k = k * (won_mod - chance);

		if (!won && k > 23)
			k = 24;

        return (int32)ceil(k);
    }

    inline int32 GetMatchmakerRatingMod(uint32 ownRating, uint32 opponentRating, bool won /*, float& confidence_factor*/)
    {
        // 'Chance' calculation - to beat the opponent
        // This is a simulation. Not much info on how it really works
        float chance = GetChanceAgainst(ownRating, opponentRating);
        float won_mod = (won) ? 1.0f : 0.0f;
        float k = won_mod - chance;
		int differenceInRating = opponentRating - ownRating;

		if (won)
		{
			if (ownRating >= 1500 && ownRating<1750)
				k = 50.0f - (won_mod - chance);
			else if (ownRating >= 1750 && ownRating<2500)
				k = 32.0f - (won_mod - chance);
			else if (ownRating >= 2500 && ownRating<2750)
				k = 20.0f - (won_mod - chance);
			else if (ownRating >= 2750 && ownRating<3000)
				k = 15.0f - (won_mod - chance);
			else if (ownRating >= 3000)
				k = 10.0f - (won_mod - chance);

			if (ownRating > 1000)
			{
				if (differenceInRating > 500)
					k += 20;
				else if (differenceInRating > 250)
					k += 10;
			}
		}


		if (k > 96.0f)
			k = 96.0f;

        return (int32)ceil(k);
    }
};

#endif
