////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef DEF_GILNEAS_H
#define DEF_GILNEAS_H

enum Data
{
};

enum Data64
{
};


enum GameObjects
{
	GO_MERCHANT_DOOR = 195327
};
enum CreatureIds
{
	NPC_PANICKED_CITIZEN = 34851,
	NPC_RAMPAGING_WORGEN_34884 = 34884,
	NPC_PRINCE_LIAM = 34913,
	NPC_FRIGHTENED_CITIZEN_34981 = 34981,
	NPC_KING_GREYMANE_35112 = 35112,
	NPC_BLOODFANG_WORGEN_35118 = 35118,
	NPC_WORGEN_ALPHA_C2 = 35167,
	NPC_WORGEN_ALPHA_C1 = 35170,
	NPC_WORGEN_RUNT_C1 = 35188,
	NPC_BLOODFANG_STALKER = 35229,
	NPC_DARIUS_CROWLEY = 35230,
	NPC_CROWLEYS_HORSE = 35231,
	NPC_JOSIAH_AVERY_35370 = 35370,
	NPC_LORNA_CROWLEY_35378 = 35378,
	NPC_LORD_DARIUS_CROWLEY_C1 = 35077,
	NPC_WORGEN_RUNT_C2 = 35456,
	NPC_BLOODFANG_LURKER = 35463,
	NPC_BLOODFANG_RIPPER_35505 = 35505,
	NPC_PRINCE_GREYMANE = 35551,
	NPC_LORD_DARIUS_CROWLEY = 35552,
	NPC_BLOODFANG_STALKER_CREDIT = 35582,
	NPC_TOBIAS_MISTMANTLE = 35618,
	NPC_FRENZIED_STALKER = 35627,
	NPC_GILNEAN_MASTIFF = 35631,
	NPC_RAMPAGING_WORGEN_35660 = 35660,
	NPC_KRENNAN_ARANAS_TREE = 35753,
	NPC_GILNEAS_EVACUATION_FACING_MARKER_35830 = 35830,
	NPC_FRIGHTENED_CITIZEN_WORGEN = 35836,
	NPC_GRAYMANE_HORSE_35905 = 35905,
	NPC_KRENNAN_ARANAS = 35907,
	NPC_KING_GREYMANE = 35911,
	NPC_COMMANDEERED_CANNON = 35914,
	NPC_NORTHGATE_REBEL_1 = 36057,
	NPC_PANICKED_CITIZEN_GATE = 44086,
	NPC_HEALING_CREDIT = 47091,
	NPC_JOSIAH_AVERY_TRIGGER = 50415,

};

enum GameObjectIds
{
};

enum QuestIds
{
    QUEST_LOCKDOWN                                     = 14078,
    QUEST_EVAC_MERC_SQUA                               = 14098,
    QUEST_SOMETHINGS_AMISS                             = 14091,
    QUEST_ALL_HELL_BREAKS_LOOSE                        = 14093,
    QUEST_ROYAL_ORDERS                                 = 14099,
    QUEST_BY_THE_SKIN_ON_HIS_TEETH                     = 14154,
    QUEST_SAVE_KRENNAN_ARANAS                          = 14293,
    QUEST_SACRIFICES                                   = 14212,
    QUEST_THE_REBEL_LORDS_ARSENAL                      = 14159,
    QUEST_FROM_THE_SHADOWS                             = 14204,
	QUEST_LAST_STAND								   = 14222,
	QUEST_LAST_CHANCE_AT_HUMANITY					   = 14375
};

enum SpellIds
{
    SPELL_ENRAGE                                       = 8599,
    SPELL_FROSTBOLT_VISUAL_ONLY                        = 74277, // Dummy spell, visual only
    SPELL_SUMMON_CROWLEY                               = 67004,
    SPELL_RIDE_HORSE                                   = 43671,
    SPELL_THROW_TORCH                                  = 67063,
    SPELL_RIDE_VEHICLE_HARDCODED                       = 46598,
    SPELL_LEFT_HOOK                                    = 67825,
    SPELL_DEMORALIZING_SHOUT                           = 61044,
    SPELL_SNAP_KICK                                    = 67827,
    SPELL_BY_THE_SKIN_ON_HIS_TEETH                     = 66914,
    SPELL_SHOOT                                        = 6660,
    SPELL_WORGEN_BITE                                  = 72870,
    SPELL_INFECTED_BITE                                = 72872,
    SPELL_CANNON_FIRE                                  = 68235,

    SPELL_GILNEAS_CANNON_CAMERA                        = 93555,
    SPELL_SUMMON_JOSIAH_AVERY                          = 67350,
    SPELL_GET_SHOT                                     = 67349,
    SPELL_SUMMON_JOSIAH                                = 67350,
    SPELL_PULL_TO                                      = 67357,
    SPELL_PHASE_QUEST_2                                = 59073,
    SPELL_SUMMON_GILNEAN_MASTIFF                       = 67807,
    SPELL_DISMISS_GILNEAN_MASTIFF                      = 43511,
    SPELL_ATTACK_LURKER                                = 67805,
    SPELL_SHADOWSTALKER_STEALTH                        = 5916,
    SPELL_PING_GILNEAN_CROW                            = 93275,
	SPELL_RENEW										   = 139,
	SPELL_ENRAGE_56646								   = 56646,
	SPELL_FORCE_CAST_SUMMON_JOSIAH					   = 67352,
	SPELL_RESCUE_KRENNAN							   = 68219,
	SPELL_FORCECAST_SUMMON_GRAYMANE_HORSE			   = 68232,
	SPELL_CURSE_OF_THE_WORGEN						   = 68630,
	SPELL_TWO_FORMS									   = 68996,
	SPELL_IN_STOCKS									   = 69196,
	SPELL_COSMETIC_COMBAT_ATTACK					   = 69873,
	SPELL_TRANSFORMING								   = 72794,
	SPELL_HIDEOUS_BITE_WOUND			    		   = 76642,
	SPELL_RIDE_BUNNY_SEAT2							   = 84275,
	
	SPELL_FADE_OF_BLACK								   = 94053,
	SPELL_ALTERED_FORM								   = 94293,
	SPELL_ALTERED_FORM2								   = 97709,
	SPELL_FORCE_WORGEN_ALTERED_FORM					   = 98274,
	SPELL_SUMMON_CROWLEY_HORSE						   = 67001,
	ZONE_DUSKHAVEN									   = 4786, // Duskhaven Zone

	SPELL_GENERIC_QUEST_INVISIBILITY_DETECTION_1	   = 49416,
	SPELL_PHASE_QUEST_ZONE_SPECIFIC_01				   = 59073,

};

enum NpcTextIds
{
    SAY_PRINCE_LIAM_GREYMANE_1                         = -1638000,
    SAY_PRINCE_LIAM_GREYMANE_2                         = -1638001,
    SAY_PRINCE_LIAM_GREYMANE_3                         = -1638002,
    DELAY_SAY_PRINCE_LIAM_GREYMANE                     = 20000, // 20 seconds repetition time

    YELL_PRINCE_LIAM_GREYMANE_1                        = -1638025,
    YELL_PRINCE_LIAM_GREYMANE_2                        = -1638026,
    YELL_PRINCE_LIAM_GREYMANE_3                        = -1638027,
    YELL_PRINCE_LIAM_GREYMANE_4                        = -1638028,
    YELL_PRINCE_LIAM_GREYMANE_5                        = -1638029,
    DELAY_YELL_PRINCE_LIAM_GREYMANE                    = 2000,

    SAY_PANICKED_CITIZEN_1                             = -1638016,
    SAY_PANICKED_CITIZEN_2                             = -1638017,
    SAY_PANICKED_CITIZEN_3                             = -1638018,
    SAY_PANICKED_CITIZEN_4                             = -1638019,

    SAY_GILNEAS_CITY_GUARD_GATE_1                      = -1638022,
    SAY_GILNEAS_CITY_GUARD_GATE_2                      = -1638023,
    SAY_GILNEAS_CITY_GUARD_GATE_3                      = -1638024,

    SAY_CITIZEN_1                                      = -1638003,
    SAY_CITIZEN_2                                      = -1638004,
    SAY_CITIZEN_3                                      = -1638005,
    SAY_CITIZEN_4                                      = -1638006,
    SAY_CITIZEN_5                                      = -1638007,
    SAY_CITIZEN_6                                      = -1638008,
    SAY_CITIZEN_7                                      = -1638009,
    SAY_CITIZEN_8                                      = -1638010,
    SAY_CITIZEN_1b                                     = -1638011,
    SAY_CITIZEN_2b                                     = -1638012,
    SAY_CITIZEN_3b                                     = -1638013,
    SAY_CITIZEN_4b                                     = -1638014,
    SAY_CITIZEN_5b                                     = -1638015,

    SAY_KRENNAN_C2                                     = 0,
    SAY_GREYMANE_HORSE                                 = 0,
    SAY_CROWLEY_HORSE_1                                = 0,    // Let''s round up as many of them as we can.  Every worgen chasing us is one less worgen chasing the survivors!
    SAY_CROWLEY_HORSE_2                                = 1,    // You'll never catch us, you blasted mongrels! || Come and get us, you motherless beasts! || Over here, you flea bags!
    SAY_JOSIAH_AVERY_P2                                = 1,
    SAY_JOSAIH_AVERY_P4                                = 1,
    SAY_JOSAIH_AVERY_TRIGGER                           = 1,
    SAY_LORNA_CROWLEY_P4                               = 0,
    SAY_KING_GENN_GREYMANE_P4                          = 1,
    SAY_GILNEAS_CITY_GUARD_P8                          = 1,
    SAY_LORD_GODFREY_P4                                = 0,
    SAY_NPC_KRENNAN_ARANAS_TREE                        = 0
};

enum SoundIds
{
    SOUND_SWORD_FLESH                                 = 143,
    SOUND_SWORD_PLATE                                 = 147,
    SOUND_WORGEN_ATTACK                               = 558,
    DELAY_SOUND                                       = 500,
    DELAY_ANIMATE                                     = 2000
};


enum Events
{
	EVENT_ACTIVE_OBJECT_CD = 101,
	EVENT_ANIMATION_START,
	EVENT_ANIMATION_STOP,
	EVENT_ANIMATION_SUBJECT,
	EVENT_ATTACK_PART_00,
	EVENT_ATTACK_PART_01,
	EVENT_ATTACK_PART_02,
	EVENT_ATTACK_PART_03,
	EVENT_ATTACK_PART_04,
	EVENT_ATTACK_PART_05,
	EVENT_ATTACK_PART_06,
	EVENT_ATTACK_PART_07,
	EVENT_ATTACK_PART_08,
	EVENT_ATTACK_PART_09,
	EVENT_ATTACK_SPELL,
	EVENT_BALLON_MESSAGE,
	EVENT_BEGIN_FLYING,
	EVENT_BEGIN_LANDING,
	EVENT_CAST_BOULDER,
	EVENT_CAST_COOLDOWN,
	EVENT_CAST_COOLDOWN_01,
	EVENT_CAST_COOLDOWN_02,
	EVENT_CAST_COOLDOWN_03,
	EVENT_CAST_COOLDOWN_04,
	EVENT_CHANGE_PHASE,
	EVENT_CAST_PLAYER,
	EVENT_CAST_SPELL_00,
	EVENT_CAST_SPELL_01,
	EVENT_CHECK_ARRIVEL_PLAYER,
	EVENT_CHECK_ATTACK,
	EVENT_CHECK_AURA,
	EVENT_CHECK_BOAT,
	EVENT_CHECK_FIGHT,
	EVENT_CHECK_FIRE,
	EVENT_CHECK_FOLLOWING,
	EVENT_CHECK_FOR_CREATURE,
	EVENT_CHECK_FOR_PLAYER,
	EVENT_CHECK_PLAYER,
	EVENT_CHECK_PLAYER_NEAR,
	EVENT_CHECK_POSITION,
	EVENT_CHECK_START_PARTY,
	EVENT_CHECK_TIMEOUT,
	EVENT_CLEAR_INVITE,
	EVENT_COMBAT_STOP,
	EVENT_COOLDOWN_00,
	EVENT_COOLDOWN_01,
	EVENT_COOLDOWN_PLAYER_NEAR,
	EVENT_CREATE_WARRIER,
	EVENT_DELIVER_PACKET,
	EVENT_DESPAWN_PART_00,
	EVENT_DESPAWN_PART_01,
	EVENT_DESPAWN_PART_02,
	EVENT_DESPAWN_PART_03,
	EVENT_DESPAWN_PART_04,
	EVENT_EARTHQUAKE,
	EVENT_ENTER_CAR,
	EVENT_ENTER_VEHICLE,
	EVENT_EXIT_CAR,
	EVENT_EXIT_VEHICLE,
	EVENT_EXPLOSION,
	EVENT_FIGHT_PART_00,
	EVENT_FIGHT_PART_01,
	EVENT_FIGHT_PART_02,
	EVENT_FIGHT_PART_03,
	EVENT_FIND_TARGET,
	EVENT_FINISH,
	EVENT_GIVE_UP,
	EVENT_INIT_TARGET,
	EVENT_INIT_PLAYER,
	EVENT_INIT_VIDEO,
	EVENT_INIT_MOUNT,
	EVENT_INVISIBLE_OBJECT,
	EVENT_KILL_TARGET,
	EVENT_MASTER_RESET,
	EVENT_MELEE_ATTACK,
	EVENT_MOVE_ATTACK,
	EVENT_MOVE_HOME,
	EVENT_MOVE_PART0,
	EVENT_MOVE_PART1,
	EVENT_MOVE_PART2,
	EVENT_MOVE_PART3,
	EVENT_MOVE_PART4,
	EVENT_MOVE_PART5,
	EVENT_MOVE_PART6,
	EVENT_MOVE_PART7,
	EVENT_MOVEMENT_DELAY,
	EVENT_MOVEMENT_PAUSE,
	EVENT_MOVEMENT_START,
	EVENT_MOVEMENT_STOP,
	EVENT_MUSIC_PERIODIC,
	EVENT_PACKET_EXPLODE,
	EVENT_PERIODIC_CHEERING,
	EVENT_PARTY_COOLDOWN,
	EVENT_PHASE_UPDATE_00,
	EVENT_PLAY_COOLDOWN,
	EVENT_PLAY_PERIODIC_SOUND,
	EVENT_PLAY_PERIODIC_EMOTE,
	EVENT_PLAY_SOUND1,
	EVENT_PLAY_SOUND2,
	EVENT_PLAYING,
	EVENT_PLAYER_ENTER_VEHICLE,
	EVENT_PLAYER_EXIT,
	EVENT_PLAYER_LANDING,
	EVENT_PLAYER_LAUNCH,
	EVENT_RANDOM_EMOTE,
	EVENT_RANDOM_CAST,
	EVENT_RESET_TARGET,
	EVENT_RESTART,
	EVENT_RUN_AWAY,
	EVENT_SAY_ATTACK,
	EVENT_SAY_HAVE_UNDERSTAND,
	EVENT_SAY_OUTCH,
	EVENT_SAY_THANKS,
	EVENT_SET_ACTIVE_TO_TRUE,
	EVENT_SET_ACTIVE_TO_FALSE,
	EVENT_SHOOT,
	EVENT_SHOOT_ON_DUMMY,
	EVENT_SHOW_FIGHT,
	EVENT_SPAWN_OBJECT,
	EVENT_SPELLCAST,
	EVENT_START_ANIM,
	EVENT_START_ANIM_01,
	EVENT_START_ANIM_02,
	EVENT_START_ANIM_03,
	EVENT_START_ANIM_04,
	EVENT_START_ANIM_05,
	EVENT_START_ANIM_06,
	EVENT_START_ANIM_07,
	EVENT_START_ANIM_08,
	EVENT_START_ANIM_09,
	EVENT_START_ANIMATION_PACKET,
	EVENT_START_FIGHT,
	EVENT_START_FLY,
	EVENT_START_FLYING,
	EVENT_START_FOLLOW,
	EVENT_START_FOLLOWING,
	EVENT_START_MOVEMENT,
	EVENT_START_MOVIE,
	EVENT_START_PARTY,
	EVENT_START_PLAY,
	EVENT_START_PLAY_GAME,
	EVENT_START_RESCUE_LIVE,
	EVENT_START_SPELL_01,
	EVENT_START_SPELL_02,
	EVENT_START_SPELL_03,
	EVENT_START_SPELL_04,
	EVENT_START_SPELL_05,
	EVENT_START_TALK,
	EVENT_START_TELEPORT,
	EVENT_START_VIDEO,
	EVENT_START_WALK,
	EVENT_START_WAYPOINTS,
	EVENT_STOP_WAYPOINTS,
	EVENT_SUMMON_DEATHWING,
	EVENT_TALK,
	EVENT_TALK_COOLDOWN,
	EVENT_TALK_PART_00,
	EVENT_TALK_PART_01,
	EVENT_TALK_PART_02,
	EVENT_TALK_PART_03,
	EVENT_TALK_PART_04,
	EVENT_TALK_PART_05,
	EVENT_TALK_PART_06,
	EVENT_TALK_PART_07,
	EVENT_TALK_PART_08,
	EVENT_TALK_PART_09,
	EVENT_TALK_PART_10,
	EVENT_TALK_PART_11,
	EVENT_TALK_PART_12,
	EVENT_TALK_PART_13,
	EVENT_TALK_PART_14,
	EVENT_TALK_PART_15,
	EVENT_TALK_PART_16,
	EVENT_TALK_PART_17,
	EVENT_TALK_PART_18,
	EVENT_TALK_PART_19,
	EVENT_TALK_PART_20,
	EVENT_TALK_PART_21,
	EVENT_TALK_PART_22,
	EVENT_TALK_PART_23,
	EVENT_TALK_PART_24,
	EVENT_TALK_PART_25,
	EVENT_TALK_PERIODIC,
	EVENT_TALK_START,
	EVENT_TORCH_COOLDOWN,
	EVENT_TRIGGER_TRAP,
	EVENT_TELEPORT_00,
	EVENT_TELEPORT_01,
	EVENT_WAIT_FOR_NEW_SPEED,
	EVENT_WAIT_TO_MOVE,
	EVENT_WAIT_FOR_ACTION_00,
	EVENT_WAIT_FOR_ACTION_01,
	EVENT_WAIT_FOR_ACTION_02,
	EVENT_WAIT_FOR_ACTION_03,
	EVENT_WAIT_FOR_ACTION_04,
	EVENT_START_WALK_01,
	EVENT_START_WALK_02,
	EVENT_START_WALK_03,
	EVENT_START_WALK_04,
	EVENT_START_WALK_05,
	EVENT_START_WALK_06,
	EVENT_START_WALK_07,
	EVENT_START_WALK_08,
	EVENT_START_WALK_09,
	EVENT_WE_ARE_KILLED,
	ACTION_DELIVER_PACKET,
	ACTION_ENTER_CAR,
	ACTION_ERASE,
	ACTION_FOLLOW,
	ACTION_INSERT,
	ACTION_IS_PLAYER_NEAR,
	ACTION_MASTER_KILLED,
	ACTION_RESET_00,
	ACTION_RESET_01,
	ACTION_RESET_02,
	ACTION_RESET_03,
	ACTION_RUN_AWAY,
	ACTION_SIZE_OF_LIST,
	ACTION_START_ANIM_01,
	ACTION_START_ANIM_02,
	ACTION_START_ANIM_03,
	ACTION_START_ANIM_04,
	ACTION_START_ANIM_05,
	ACTION_START_ANIM_06,
	ACTION_START_ANIM_07,
	ACTION_START_ANIM_08,
	ACTION_START_ANIM_09,
	ACTION_START_HELP_PLAYER,
	ACTION_START_RESCUE_LIVE,
	ACTION_START_TALK,
	ACTION_START_WALK,
	ACTION_START_WALK_01,
	ACTION_START_WALK_02,
	ACTION_START_WALK_03,
	ACTION_START_WALK_04,
	ACTION_STOP_HELP_PLAYER,
	ACTION_TEST,
	ACTION_WALK_TO_HOME_POSITION,

	DATA_GET_EVENTID,
	DATA_GUARD_INDEX,

	PLAYER_GUID = 99991,
	PASSENGER_GUID = 99998,
};

struct Waypoint
{
    float X, Y, Z;
};

Waypoint NW_WAYPOINT_LOC1[2]=
{
    { -1630.62f, 1480.55f, 70.40f }, // Worgen Runt 1 top edge of Roof Waypoint
    { -1636.01f, 1475.81f, 64.51f }  // Worgen Runt 1 Edge of Roof Waypoint
};

Waypoint NW_WAYPOINT_LOC2[2]=
{
    { -1637.26f, 1488.86f, 69.95f }, // Worgen Runt 1 top edge of Roof Waypoint
    { -1642.45f, 1482.23f, 64.30f }  // Worgen Runt 1 Edge of Roof Waypoint
};

Waypoint SW_WAYPOINT_LOC1[2]=
{
    { -1718.31f, 1526.62f, 55.91f }, // Worgen Runt 2 Corner where we turn
    { -1717.86f, 1490.77f, 56.61f }  // Worgen Runt 2 Edge of Roof Waypoint
};

Waypoint SW_WAYPOINT_LOC2[2]=
{
    { -1718.31f, 1526.62f, 55.91f }, // Worgen Alpha 1 Corner where we turn
    { -1717.86f, 1487.00f, 57.07f }  // Worgen Alpha 1 Edge of Roof Waypoint
};

Waypoint N_WAYPOINT_LOC[1]=
{
    { -1593.38f, 1408.02f, 72.64f } // Worgen Runt 2 Edge of Roof Waypoint
};

#define DELAY_EMOTE_PANICKED_CITIZEN                  urand(5000, 15000)   // 5-15 second time
#define DELAY_SAY_PANICKED_CITIZEN                    urand(30000, 120000) // 30sec - 1.5min
#define DELAY_SAY_GILNEAS_CITY_GUARD_GATE             urand(30000, 120000) // 30sec - 1.5min
#define PATHS_COUNT_PANICKED_CITIZEN                  8
#define CD_ENRAGE                                     30000
#define SUMMON1_TTL                                   300000
#define PATHS_COUNT                                   2
#define DOOR_TIMER                                    30*IN_MILLISECONDS
#define KRENNAN_END_X                                 -1772.4172f
#define KRENNAN_END_Y                                 1430.6125f
#define KRENNAN_END_Z                                 19.79f
#define KRENNAN_END_O                                 2.79f
#define CROWLEY_SPEED                                 1.85f // if set much lower than this, the horse automatically despawns before reaching the end of his waypoints
#define AI_MIN_HP                                     85
#define Event_Time                                    118500
#define WORGEN_EVENT_SPAWNTIME                        20000 // Default Despawn Timer
#define NW_ROOF_SPAWN_LOC_1                           -1618.86f, 1505.68f, 70.24f, 3.91f
#define NW_ROOF_SPAWN_LOC_2                           -1611.40f, 1498.49f, 69.82f, 3.79f
#define SW_ROOF_SPAWN_LOC_1                           -1732.81f, 1526.34f, 55.39f, 0.01f
#define SW_ROOF_SPAWN_LOC_2                           -1737.49f, 1526.11f, 55.51f, 0.01f
#define N_ROOF_SPAWN_LOC                              -1562.59f, 1409.35f, 71.66f, 3.16f
#define PLATFORM_Z                                    52.29f

#endif
