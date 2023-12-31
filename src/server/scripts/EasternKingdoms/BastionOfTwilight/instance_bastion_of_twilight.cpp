////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "bastion_of_twilight.h"

#define MAX_ENCOUNTER 5

static const DoorData doordata[] =
{
    {DOOR_HALFUS_ENTRANCE,  DATA_HALFUS,            DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {DOOR_HALFUS_EXIT,      DATA_HALFUS,            DOOR_TYPE_PASSAGE,  BOUNDARY_NONE},
    {DOOR_THER_ENTRANCE,    DATA_VALIONA_THERALION, DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {DOOR_THER_EXIT,        DATA_VALIONA_THERALION, DOOR_TYPE_PASSAGE,  BOUNDARY_NONE},
    {DOOR_COUNCIL_ENTRANCE, DATA_COUNCIL,           DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {DOOR_COUNCIL_EXIT,     DATA_COUNCIL,           DOOR_TYPE_PASSAGE,  BOUNDARY_NONE},
    {DOOR_CHOGALL_ENTRANCE, DATA_CHOGALL,           DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {GO_CHOGALL_FLOOR,      DATA_CHOGALL,           DOOR_TYPE_PASSAGE,  BOUNDARY_NONE},
    {GO_SINESTRA_DOOR,      DATA_SINESTRA,          DOOR_TYPE_ROOM,     BOUNDARY_NONE},
    {0,                     0,                      DOOR_TYPE_ROOM,       BOUNDARY_NONE}, // END
};

class instance_bastion_of_twilight : public InstanceMapScript
{
public:
    instance_bastion_of_twilight() : InstanceMapScript("instance_bastion_of_twilight", 671) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_bastion_of_twilight_InstanceMapScript(map);
    }

    struct instance_bastion_of_twilight_InstanceMapScript: public InstanceScript
    {
        instance_bastion_of_twilight_InstanceMapScript(Map* map) : InstanceScript(map)
        {
            SetBossNumber(MAX_ENCOUNTER);
            LoadDoorData(doordata);
            uiWyrmbreakerGUID = 0;
            uiValionaGUID = 0;
            uiTheralionGUID = 0;
            uiFeludiusGUID = 0;
            uiArionGUID = 0;
            uiIgnaciousGUID = 0;
            uiTerrastraGUID = 0;
            uiMonstrosityGUID = 0;
            uiWhelpCageGUID = 0;
            uiValionaTheralionHealth = 0;
            uiChogallGUID = 0;
            uiChogallFloorGUID = 0;
            memset(m_uiDialogs, 0, sizeof(m_uiDialogs));
            playerDied = 0;
        }

        void OnCreatureCreate(Creature* creature)
        {
            switch (creature->GetEntry())
            {
                case NPC_HALFUS_WYRMBREAKER:
                    uiWyrmbreakerGUID = creature->GetGUID();
                    break;
                case NPC_VALIONA:
                    uiValionaGUID = creature->GetGUID();
                    break;
                case NPC_THERALION:
                    uiTheralionGUID = creature->GetGUID();
                    break;
                case NPC_FELUDIUS:
                    uiFeludiusGUID = creature->GetGUID();
                    break;
                case NPC_IGNACIOUS:
                    uiIgnaciousGUID = creature->GetGUID();
                    break;
                case NPC_ARION:
                    uiArionGUID = creature->GetGUID();
                    break;
                case NPC_TERRASTRA:
                    uiTerrastraGUID = creature->GetGUID();
                    break;
                case NPC_MONSTROSITY:
                    uiMonstrosityGUID = creature->GetGUID();
                    break;
                case NPC_CHOGALL:
                    uiChogallGUID = creature->GetGUID();
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* pGo)
        {
            switch (pGo->GetEntry())
            {
            case GO_WHELP_CAGE:
                uiWhelpCageGUID = pGo->GetGUID();
                break;
            case DOOR_HALFUS_ENTRANCE:
                uiDoorHalfusEntranceGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case DOOR_HALFUS_EXIT:
                uiDoorHalfusExitGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case DOOR_THER_ENTRANCE:
                uiDoorTherEntranceGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case DOOR_THER_EXIT:
                uiDoorTherExitGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case DOOR_COUNCIL_ENTRANCE:
                uiDoorCouncilEntranceGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case DOOR_COUNCIL_EXIT:
                uiDoorCouncilExitGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case DOOR_CHOGALL_ENTRANCE:
                uiDoorChogallEntranceGUID = pGo->GetGUID();
                AddDoor(pGo, true);
                break;
            case GO_SINESTRA_DOOR:
                AddDoor(pGo, true);
                break;
            case GO_CHOGALL_FLOOR:
                AddDoor(pGo, true);
                uiChogallFloorGUID = pGo->GetGUID();
                break;
            }
        }

        void OnUnitDeath(Unit* unit)
        {
            if (unit->IsPlayer() && GetBossState(DATA_SINESTRA) == IN_PROGRESS)
            {
                if (playerDied == 0)
                {
                    playerDied = 1;
                    SaveToDB();
                }
            }
        }

        uint64 GetData64(uint32 identifier)
        {
            switch (identifier)
            {
            case DATA_HALFUS:
                return uiWyrmbreakerGUID;
            case DATA_VALIONA:
                return uiValionaGUID;
            case DATA_THERALION:
                return uiTheralionGUID;
            case DATA_FELUDIUS:
                return uiFeludiusGUID;
            case DATA_IGNACIOUS:
                return uiIgnaciousGUID;
            case DATA_ARION:
                return uiArionGUID;
            case DATA_TERRASTRA:
                return uiTerrastraGUID;
            case DATA_MONSTROSITY:
                return uiMonstrosityGUID;
            case DATA_WHELP_CAGE:
                return uiWhelpCageGUID;
            case DATA_CHOGALL:
                return uiChogallGUID;
            }
            return 0;
        }

        uint32 GetData(uint32 type)
        {
            switch (type)
            {
            case DATA_HEALTH_VALIONA_THERALION:
                return uiValionaTheralionHealth;
            case DATA_DLG_ENTRANCE:
                return m_uiDialogs[0];
            case DATA_DLG_HALFUS:
                return m_uiDialogs[1];
            case DATA_DLG_VALIONA_THERALION:
                return m_uiDialogs[2];
            case DATA_DLG_COUNCIL_1:
                return m_uiDialogs[3];
            case DATA_DLG_COUNCIL_2:
                return m_uiDialogs[4];
            case DATA_DLG_COUNCIL_3:
                return m_uiDialogs[5];
            case DATA_DLG_CHOGALL:
                return m_uiDialogs[6];
            case DATA_DLG_SINESTRA:
                return m_uiDialogs[7];
            case DATA_WIPE_COUNT:
                return playerDied;
            }
            return 0;
        }

        bool SetBossState(uint32 type, EncounterState state)
        {
            if (!InstanceScript::SetBossState(type, state))
                return false;

            switch (type)
            {
                case DATA_HALFUS:
                    break;
                case DATA_VALIONA_THERALION:
                    break;
                case DATA_COUNCIL:
                    break;
                case DATA_CHOGALL:
                    break;
                case DATA_SINESTRA:
                    break;
                default:
                    break;
            }
            return true;
        }

        void SetData(uint32 type, uint32 data)
        {
            switch (type)
            {
                case DATA_HEALTH_VALIONA_THERALION:
                    uiValionaTheralionHealth = data;
                    break;
                case DATA_DLG_ENTRANCE:
                    m_uiDialogs[0] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_HALFUS:
                    m_uiDialogs[1] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_VALIONA_THERALION:
                    m_uiDialogs[2] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_COUNCIL_1:
                    m_uiDialogs[3] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_COUNCIL_2:
                    m_uiDialogs[4] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_COUNCIL_3:
                    m_uiDialogs[5] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_CHOGALL:
                    m_uiDialogs[6] = data;
                    SaveToDB();
                    break;
                case DATA_DLG_SINESTRA:
                    m_uiDialogs[7] = data;
                    SaveToDB();
                    break;
            }
        }

        bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const
        {
            if (player && player->isGameMaster())
                return true;
        
            switch (bossId)
            {
                case DATA_VALIONA_THERALION:
                    if (GetBossState(DATA_HALFUS) != DONE)
                        return false;
                    break;
                case DATA_COUNCIL:
                    if (GetBossState(DATA_VALIONA_THERALION) != DONE)
                        return false;
                    break;
                case DATA_CHOGALL:
                    if (GetBossState(DATA_COUNCIL) != DONE)
                        return false;
                    break;
                case DATA_SINESTRA:
                    if (GetBossState(DATA_CHOGALL) != DONE)
                        return false;
                    break;
                default:
                    break;
            }
            return true;
        }

        std::string GetDialogSaveData()
        {
            std::ostringstream saveStream;
            for (uint8 i = 0; i < 8; i++)
                saveStream << (uint32)m_uiDialogs[i] << " ";
            return saveStream.str();
        }

        std::string GetSaveData()
        {
            OUT_SAVE_INST_DATA;

            std::string str_data;

            std::ostringstream saveStream;
            saveStream << "B T " << GetBossSaveData() << GetDialogSaveData() << playerDied << " ";

            str_data = saveStream.str();

            OUT_SAVE_INST_DATA_COMPLETE;
            return str_data;
        }

        void Load(const char* in)
        {
            if (!in)
            {
                OUT_LOAD_INST_DATA_FAIL;
                return;
            }

            OUT_LOAD_INST_DATA(in);

            char dataHead1, dataHead2;

            std::istringstream loadStream(in);
            loadStream >> dataHead1 >> dataHead2;

            if (dataHead1 == 'B' && dataHead2 == 'T')
            {
                for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                {
                    uint32 tmpState;
                    loadStream >> tmpState;
                    if (tmpState == IN_PROGRESS || tmpState > SPECIAL)
                        tmpState = NOT_STARTED;
                    SetBossState(i, EncounterState(tmpState));
                }
                for (uint8 i = 0; i < 8; i++)
                {
                    uint32 tmpDlg;
                    loadStream >> tmpDlg;
                    if (tmpDlg != DONE)
                        tmpDlg = NOT_STARTED;
                    m_uiDialogs[i] = tmpDlg;
                }

                loadStream >> playerDied;


            }
            else
                OUT_LOAD_INST_DATA_FAIL;

            OUT_LOAD_INST_DATA_COMPLETE;
        }

        private:
            uint64 uiWyrmbreakerGUID;
            uint32 m_uiDialogs[8];
            uint64 uiValionaGUID;
            uint64 uiTheralionGUID;
            uint64 uiFeludiusGUID;
            uint64 uiArionGUID;
            uint64 uiIgnaciousGUID;
            uint64 uiTerrastraGUID;
            uint64 uiMonstrosityGUID;
            uint32 uiValionaTheralionHealth;
            uint64 uiChogallGUID;
            uint64 uiWhelpCageGUID;
            uint64 uiDoorHalfusEntranceGUID;
            uint64 uiDoorHalfusExitGUID;
            uint64 uiDoorTherEntranceGUID;
            uint64 uiDoorTherExitGUID;
            uint64 uiDoorCouncilEntranceGUID;
            uint64 uiDoorCouncilExitGUID;
            uint64 uiDoorChogallEntranceGUID;
            uint64 uiChogallFloorGUID;
            uint32 playerDied;
    };
};

#ifndef __clang_analyzer__
void AddSC_instance_bastion_of_twilight()
{
    new instance_bastion_of_twilight();
}
#endif
