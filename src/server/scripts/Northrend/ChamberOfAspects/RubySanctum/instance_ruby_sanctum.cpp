////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ruby_sanctum.h"
#include "Player.h"
#include "TemporarySummon.h"
#include "WorldPacket.h"

DoorData const doorData[] =
{
    {GO_FIRE_FIELD,     DATA_BALTHARUS_THE_WARBORN, DOOR_TYPE_PASSAGE,  BOUNDARY_E   },
    {0,                 0,                          DOOR_TYPE_ROOM,     BOUNDARY_NONE}
};

class instance_ruby_sanctum : public InstanceMapScript
{
    public:
        instance_ruby_sanctum() : InstanceMapScript(RSScriptName, 724) { }

        struct instance_ruby_sanctum_InstanceMapScript : public InstanceScript
        {
            instance_ruby_sanctum_InstanceMapScript(InstanceMap* map) : InstanceScript(map)
            {
                SetBossNumber(EncounterCount);
                LoadDoorData(doorData);
                BaltharusTheWarbornGUID  = 0;
                GeneralZarithrianGUID    = 0;
                SavianaRagefireGUID      = 0;
                HalionGUID               = 0;
                TwilightHalionGUID       = 0;
                OrbCarrierGUID           = 0;
                OrbRotationFocusGUID     = 0;
                HalionControllerGUID     = 0;
                CrystalChannelTargetGUID = 0;
                XerestraszaGUID          = 0;
                BaltharusSharedHealth    = 0;
                FlameWallsGUID           = 0;
                FlameRingGUID            = 0;

                memset(ShadowOrbsGUIDs, 0, 4 * sizeof(uint64));
                memset(ZarithrianSpawnStalkerGUID, 0, 2 * sizeof(uint64));
                memset(BurningTreeGUID, 0, 4 * sizeof(uint64));
            }

            void OnPlayerEnter(Player* /*player*/)
            {
                if (!GetData64(DATA_HALION_CONTROLLER) && GetBossState(DATA_HALION) != DONE && GetBossState(DATA_GENERAL_ZARITHRIAN) == DONE)
                {
                    instance->LoadGrid(HalionControllerSpawnPos.GetPositionX(), HalionControllerSpawnPos.GetPositionY());
                    if (Creature* halionController = instance->SummonCreature(NPC_HALION_CONTROLLER, HalionControllerSpawnPos))
                        halionController->AI()->DoAction(ACTION_INTRO_HALION);
                }
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_BALTHARUS_THE_WARBORN:
                        BaltharusTheWarbornGUID = creature->GetGUID();
                        break;
                    case NPC_GENERAL_ZARITHRIAN:
                        GeneralZarithrianGUID = creature->GetGUID();
                        break;
                    case NPC_SAVIANA_RAGEFIRE:
                        SavianaRagefireGUID = creature->GetGUID();
                        break;
                    case NPC_HALION:
                        HalionGUID = creature->GetGUID();
                        break;
                    case NPC_TWILIGHT_HALION:
                        TwilightHalionGUID = creature->GetGUID();
                        break;
                    case NPC_HALION_CONTROLLER:
                        HalionControllerGUID = creature->GetGUID();
                        break;
                    case NPC_ORB_CARRIER:
                        OrbCarrierGUID = creature->GetGUID();
                        break;
                    case NPC_ORB_ROTATION_FOCUS:
                        OrbRotationFocusGUID = creature->GetGUID();
                        break;
                    case NPC_SHADOW_ORB_N:
                        ShadowOrbsGUIDs[0] = creature->GetGUID();
                        break;
                    case NPC_SHADOW_ORB_S:
                        ShadowOrbsGUIDs[1] = creature->GetGUID();
                        break;
                    case NPC_SHADOW_ORB_E:
                        ShadowOrbsGUIDs[2] = creature->GetGUID();
                        break;
                    case NPC_SHADOW_ORB_W:
                        ShadowOrbsGUIDs[3] = creature->GetGUID();
                        break;
                    case NPC_BALTHARUS_TARGET:
                        CrystalChannelTargetGUID = creature->GetGUID();
                        break;
                    case NPC_XERESTRASZA:
                        XerestraszaGUID = creature->GetGUID();
                        break;
                    case NPC_ZARITHRIAN_SPAWN_STALKER:
                        if (!ZarithrianSpawnStalkerGUID[0])
                            ZarithrianSpawnStalkerGUID[0] = creature->GetGUID();
                        else
                            ZarithrianSpawnStalkerGUID[1] = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GO_FIRE_FIELD:
                        AddDoor(go, true);
                        break;
                    case GO_FLAME_WALLS:
                        FlameWallsGUID = go->GetGUID();
                        if (GetBossState(DATA_SAVIANA_RAGEFIRE) == DONE && GetBossState(DATA_BALTHARUS_THE_WARBORN) == DONE)
                            HandleGameObject(FlameWallsGUID, true, go);
                        break;
                    case GO_FLAME_RING:
                        FlameRingGUID = go->GetGUID();
                        break;
                    case GO_TWILIGHT_FLAME_RING:
                        TwilightFlameRingGUID = go->GetGUID();
                        break;
                    case GO_BURNING_TREE_1:
                        BurningTreeGUID[0] = go->GetGUID();
                        if (GetBossState(DATA_GENERAL_ZARITHRIAN) == DONE)
                            HandleGameObject(BurningTreeGUID[0], true);
                        break;
                    case GO_BURNING_TREE_2:
                        BurningTreeGUID[1] = go->GetGUID();
                        if (GetBossState(DATA_GENERAL_ZARITHRIAN) == DONE)
                            HandleGameObject(BurningTreeGUID[1], true);
                        break;
                    case GO_BURNING_TREE_3:
                        BurningTreeGUID[2] = go->GetGUID();
                        if (GetBossState(DATA_GENERAL_ZARITHRIAN) == DONE)
                            HandleGameObject(BurningTreeGUID[2], true);
                        break;
                    case GO_BURNING_TREE_4:
                        BurningTreeGUID[3] = go->GetGUID();
                        if (GetBossState(DATA_GENERAL_ZARITHRIAN) == DONE)
                            HandleGameObject(BurningTreeGUID[3], true);
                        break;
                    default:
                        break;
                }
            }

            void OnUnitDeath(Unit* unit)
            {
                Creature* creature = unit->ToCreature();
                if (!creature)
                    return;

                if (creature->GetEntry() == NPC_GENERAL_ZARITHRIAN && GetBossState(DATA_HALION) != DONE)
                {
                    instance->LoadGrid(HalionControllerSpawnPos.GetPositionX(), HalionControllerSpawnPos.GetPositionY());
                    if (Creature* halionController = instance->SummonCreature(NPC_HALION_CONTROLLER, HalionControllerSpawnPos))
                        halionController->AI()->DoAction(ACTION_INTRO_HALION);
                }
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case DATA_BALTHARUS_THE_WARBORN:
                        return BaltharusTheWarbornGUID;
                    case DATA_CRYSTAL_CHANNEL_TARGET:
                        return CrystalChannelTargetGUID;
                    case DATA_XERESTRASZA:
                        return XerestraszaGUID;
                    case DATA_SAVIANA_RAGEFIRE:
                        return SavianaRagefireGUID;
                    case DATA_GENERAL_ZARITHRIAN:
                        return GeneralZarithrianGUID;
                    case DATA_ZARITHRIAN_SPAWN_STALKER_1:
                    case DATA_ZARITHRIAN_SPAWN_STALKER_2:
                        return ZarithrianSpawnStalkerGUID[type - DATA_ZARITHRIAN_SPAWN_STALKER_1];
                    case DATA_HALION:
                        return HalionGUID;
                    case DATA_TWILIGHT_HALION:
                        return TwilightHalionGUID;
                    case DATA_ORB_CARRIER:
                        return OrbCarrierGUID;
                    case DATA_ORB_ROTATION_FOCUS:
                        return OrbRotationFocusGUID;
                    case DATA_SHADOW_ORB_N:
                        return ShadowOrbsGUIDs[0];
                    case DATA_SHADOW_ORB_S:
                        return ShadowOrbsGUIDs[1];
                    case DATA_SHADOW_ORB_E:
                        return ShadowOrbsGUIDs[2];
                    case DATA_SHADOW_ORB_W:
                        return ShadowOrbsGUIDs[3];
                    case DATA_HALION_CONTROLLER:
                        return HalionControllerGUID;
                    case DATA_BURNING_TREE_1:
                    case DATA_BURNING_TREE_2:
                    case DATA_BURNING_TREE_3:
                    case DATA_BURNING_TREE_4:
                        return BurningTreeGUID[type - DATA_BURNING_TREE_1];
                    case DATA_FLAME_RING:
                        return FlameRingGUID;
                    case DATA_TWILIGHT_FLAME_RING:
                        return TwilightFlameRingGUID;
                    default:
                        break;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state)
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case DATA_BALTHARUS_THE_WARBORN:
                    {
                        if (state == DONE && GetBossState(DATA_SAVIANA_RAGEFIRE) == DONE)
                        {
                            HandleGameObject(FlameWallsGUID, true);
                            if (Creature* zarithrian = instance->GetCreature(GeneralZarithrianGUID))
                                zarithrian->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                        }
                        break;
                    }
                    case DATA_SAVIANA_RAGEFIRE:
                    {
                        if (state == DONE && GetBossState(DATA_BALTHARUS_THE_WARBORN) == DONE)
                        {
                            HandleGameObject(FlameWallsGUID, true);
                            if (Creature* zarithrian = instance->GetCreature(GeneralZarithrianGUID))
                                zarithrian->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE);
                        }
                        break;
                    }
                    case DATA_GENERAL_ZARITHRIAN:
                    {
                        if (GetBossState(DATA_SAVIANA_RAGEFIRE) == DONE && GetBossState(DATA_BALTHARUS_THE_WARBORN) == DONE)
                            HandleGameObject(FlameWallsGUID, state != IN_PROGRESS);
                        break;
                    }
                    case DATA_HALION:
                    {
                        DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TOGGLE, 0);
                        DoUpdateWorldState(WORLDSTATE_CORPOREALITY_TWILIGHT, 0);
                        DoUpdateWorldState(WORLDSTATE_CORPOREALITY_MATERIAL, 0);

                        // Reopen rings on wipe or success
                        if (state == DONE || state == FAIL)
                        {
                            HandleGameObject(FlameRingGUID, true);
                            HandleGameObject(TwilightFlameRingGUID, true);
                        }
                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                if (type != DATA_BALTHARUS_SHARED_HEALTH)
                    return;

                BaltharusSharedHealth = data;
            }

            uint32 GetData(uint32 type)
            {
                if (type != DATA_BALTHARUS_SHARED_HEALTH)
                    return 0;

                return BaltharusSharedHealth;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "R S " << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void FillInitialWorldStates(ByteBuffer& data)
            {
                data << uint32(WORLDSTATE_CORPOREALITY_MATERIAL) << uint32(50);
                data << uint32(WORLDSTATE_CORPOREALITY_TWILIGHT) << uint32(50);
                data << uint32(WORLDSTATE_CORPOREALITY_TOGGLE) << uint32(0);
            }

            void Load(char const* str)
            {
                if (!str)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(str);

                char dataHead1, dataHead2;

                std::istringstream loadStream(str);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'R' && dataHead2 == 'S')
                {
                    for (uint8 i = 0; i < EncounterCount; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                            tmpState = NOT_STARTED;

                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        protected:
            uint64 BaltharusTheWarbornGUID;
            uint64 GeneralZarithrianGUID;
            uint64 SavianaRagefireGUID;
            uint64 HalionGUID;
            uint64 TwilightHalionGUID;
            uint64 HalionControllerGUID;
            uint64 OrbCarrierGUID;
            uint64 OrbRotationFocusGUID;
            uint64 CrystalChannelTargetGUID;
            uint64 ShadowOrbsGUIDs[4];
            uint64 XerestraszaGUID;
            uint64 FlameWallsGUID;
            uint64 ZarithrianSpawnStalkerGUID[2];
            uint64 BurningTreeGUID[4];
            uint64 FlameRingGUID;
            uint64 TwilightFlameRingGUID;

            uint32 BaltharusSharedHealth;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_ruby_sanctum_InstanceMapScript(map);
        }
};

#ifndef __clang_analyzer__
void AddSC_instance_ruby_sanctum()
{
    new instance_ruby_sanctum();
}
#endif
