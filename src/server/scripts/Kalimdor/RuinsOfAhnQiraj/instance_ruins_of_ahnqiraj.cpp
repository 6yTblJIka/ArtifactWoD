////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ruins_of_ahnqiraj.h"

class instance_ruins_of_ahnqiraj : public InstanceMapScript
{
    public:
        instance_ruins_of_ahnqiraj() : InstanceMapScript("instance_ruins_of_ahnqiraj", 509) {}

        struct instance_ruins_of_ahnqiraj_InstanceMapScript : public InstanceScript
        {
            instance_ruins_of_ahnqiraj_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                SetBossNumber(MAX_ENCOUNTER);

                _kurinaxxGUID   = 0;
                _rajaxxGUID     = 0;
                _moamGUID       = 0;
                _buruGUID       = 0;
                _ayamissGUID    = 0;
                _ossirianGUID   = 0;
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_KURINAXX:
                        _kurinaxxGUID = creature->GetGUID();
                        break;
                    case NPC_RAJAXX:
                        _rajaxxGUID = creature->GetGUID();
                        break;
                    case NPC_MOAM:
                        _moamGUID = creature->GetGUID();
                        break;
                    case NPC_BURU:
                        _buruGUID = creature->GetGUID();
                        break;
                    case NPC_AYAMISS:
                        _ayamissGUID = creature->GetGUID();
                        break;
                    case NPC_OSSIRIAN:
                        _ossirianGUID = creature->GetGUID();
                        break;
                }
            }

            bool SetBossState(uint32 bossId, EncounterState state)
            {
                if (!InstanceScript::SetBossState(bossId, state))
                    return false;

                return true;
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case BOSS_KURINNAXX:
                        return _kurinaxxGUID;
                    case BOSS_RAJAXX:
                        return _rajaxxGUID;
                    case BOSS_MOAM:
                        return _moamGUID;
                    case BOSS_BURU:
                        return _buruGUID;
                    case BOSS_AYAMISS:
                        return _ayamissGUID;
                    case BOSS_OSSIRIAN:
                        return _ossirianGUID;
                }

                return 0;
            }

            std::string GetSaveData()
            {
                OUT_SAVE_INST_DATA;

                std::ostringstream saveStream;
                saveStream << "R A" << GetBossSaveData();

                OUT_SAVE_INST_DATA_COMPLETE;
                return saveStream.str();
            }

            void Load(char const* data)
            {
                if (!data)
                {
                    OUT_LOAD_INST_DATA_FAIL;
                    return;
                }

                OUT_LOAD_INST_DATA(data);

                char dataHead1, dataHead2;

                std::istringstream loadStream(data);
                loadStream >> dataHead1 >> dataHead2;

                if (dataHead1 == 'R' && dataHead2 == 'A')
                {
                    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
                    {
                        uint32 tmpState;
                        loadStream >> tmpState;
                        if (tmpState == IN_PROGRESS || tmpState > TO_BE_DECIDED)
                            tmpState = NOT_STARTED;
                        SetBossState(i, EncounterState(tmpState));
                    }
                }
                else
                    OUT_LOAD_INST_DATA_FAIL;

                OUT_LOAD_INST_DATA_COMPLETE;
            }

        private:
            uint64 _kurinaxxGUID;
            uint64 _rajaxxGUID;
            uint64 _moamGUID;
            uint64 _buruGUID;
            uint64 _ayamissGUID;
            uint64 _ossirianGUID;
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_ruins_of_ahnqiraj_InstanceMapScript(map);
        }
};

#ifndef __clang_analyzer__
void AddSC_instance_ruins_of_ahnqiraj()
{
    new instance_ruins_of_ahnqiraj();
}
#endif
