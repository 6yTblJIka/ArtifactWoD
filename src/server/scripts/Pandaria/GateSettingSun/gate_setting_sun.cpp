////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "gate_setting_sun.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "Vehicle.h"

enum spells
{
    SPELL_MANTID_MUNITION_EXPLOSION     = 107153,
    SPELL_EXPLOSE_GATE                  = 115456,

    SPELL_BOMB_CAST_VISUAL              = 106729,
    SPELL_BOMB_AURA                     = 106875
};

class mob_serpent_spine_defender : public CreatureScript
{
public:
    mob_serpent_spine_defender() : CreatureScript("mob_serpent_spine_defender") { }

    struct mob_serpent_spine_defenderAI : public ScriptedAI
    {
        mob_serpent_spine_defenderAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 attackTimer;

        void Reset()
        {
            attackTimer = urand(1000, 5000);
        }

        void DamageDealt(Unit* /*target*/, uint32& damage, DamageEffectType /*damageType*/)
        {
            damage = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!me->isInCombat())
            {
                if (attackTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                        if (!target->IsFriendlyTo(me))
                            AttackStart(target);
                }
                else
                    attackTimer -= diff;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_serpent_spine_defenderAI(creature);
    }
};

class npc_krikthik_bombarder : public CreatureScript
{
public:
    npc_krikthik_bombarder() : CreatureScript("npc_krikthik_bombarder") { }

    struct npc_krikthik_bombarderAI : public ScriptedAI
    {
        npc_krikthik_bombarderAI(Creature* creature) : ScriptedAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        InstanceScript* pInstance;
        uint32 bombTimer;

        void Reset()
        {
            me->GetMotionMaster()->MoveRandom(5.0f);
            bombTimer = urand(1000, 7500);
        }

        // Called when spell hits a target
        void SpellHitTarget(Unit* target, SpellInfo const* /*spell*/)
        {
            if (target->GetEntry() == NPC_BOMB_STALKER)
                me->AddAura(SPELL_BOMB_AURA, target);
        }

        void UpdateAI(const uint32 diff)
        {
            if (bombTimer <= diff)
            {
                if (Unit* stalker = pInstance->instance->GetCreature(pInstance->GetData64(DATA_RANDOM_BOMB_STALKER)))
                    if (!stalker->HasAura(SPELL_BOMB_AURA))
                        me->CastSpell(stalker, SPELL_BOMB_CAST_VISUAL, true);

                bombTimer = urand(1000, 5000);
            }
            else bombTimer -= diff;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_krikthik_bombarderAI (creature);
    }
};

//8359
class AreaTrigger_at_first_door : public AreaTriggerScript
{
    public:
        AreaTrigger_at_first_door() : AreaTriggerScript("at_first_door") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if (player->GetInstanceScript())
                player->GetInstanceScript()->SetData(DATA_OPEN_FIRST_DOOR, DONE);

            return false;
        }
};

class go_setting_sun_brasier : public GameObjectScript
{
public:
    go_setting_sun_brasier() : GameObjectScript("go_setting_sun_brasier") { }

    bool OnGossipHello(Player* player, GameObject* /*go*/)
    {
        if (player->GetInstanceScript())
            player->GetInstanceScript()->SetData(DATA_BRASIER_CLICKED, DONE);

        return false;
    }
};

class go_setting_sun_temp_portal : public GameObjectScript
{
public:
    go_setting_sun_temp_portal() : GameObjectScript("go_setting_sun_temp_portal") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        switch (go->GetEntry())
        {
            case 400001:
                player->NearTeleportTo(1078.96f, 2305.48f, 381.55f, 0.01f);
                break;
            case 400002:
                if (go->GetPositionZ() < 400.0f)
                    player->NearTeleportTo(go->GetPositionX(), go->GetPositionY(), 431.0f, go->GetOrientation());
                else
                    player->NearTeleportTo(go->GetPositionX(), go->GetPositionY(), 388.5f, go->GetOrientation());
                break;
        }

        return false;
    }
};

class vehicle_artillery_to_wall : public VehicleScript
{
    public:
        vehicle_artillery_to_wall() : VehicleScript("vehicle_artillery_to_wall") {}

        void OnAddPassenger(Vehicle* veh, Unit* /*passenger*/, int8 /*seatId*/)
        {
            if (veh->GetBase())
                if (veh->GetBase()->ToCreature())
                    if (veh->GetBase()->ToCreature()->AI())
                        veh->GetBase()->ToCreature()->AI()->DoAction(0);
        }

        struct vehicle_artillery_to_wallAI : public ScriptedAI
        {
            vehicle_artillery_to_wallAI(Creature* creature) : ScriptedAI(creature)
            {}

            uint32 launchEventTimer;

            void Reset()
            {
                launchEventTimer = 0;
            }

            void DoAction(int32 const /*p_Action*/)
            {
                launchEventTimer = 2500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!launchEventTimer)
                    return;

                if (launchEventTimer <= diff)
                {
                    if (me->GetVehicleKit())
                    {
                        if (Unit* passenger = me->GetVehicleKit()->GetPassenger(0))
                        {
                            passenger->ExitVehicle();
                            passenger->GetMotionMaster()->MoveJump(1100.90f, 2304.58f, 381.23f, 30.0f, 50.0f);
                        }
                    }

                    launchEventTimer = 0;
                }
                else launchEventTimer -= diff;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new vehicle_artillery_to_wallAI(creature);
        }
};

class go_setting_sun_elevator : public GameObjectScript
{
    public:
        go_setting_sun_elevator() : GameObjectScript("go_setting_sun_elevator") { }

        InstanceScript* m_Instance;

        bool OnGossipHello(Player* p_Player, GameObject* /*go*/)
        {
            if (!p_Player)
                return false;

            std::list<Player*> l_PlayerList;
            GetPlayerListInGrid(l_PlayerList, p_Player, 45.0f);

            for (Player* player : l_PlayerList)
                player->TeleportTo(962, 1193.399f, 2290.80f, 430.87f, 1.52f);

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_gate_setting_sun()
{
    new mob_serpent_spine_defender();
    new npc_krikthik_bombarder();
    new AreaTrigger_at_first_door();
    new go_setting_sun_brasier();
    new go_setting_sun_temp_portal();
    new vehicle_artillery_to_wall();
    new go_setting_sun_elevator();
}
#endif
