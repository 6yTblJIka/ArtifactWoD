////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "utgarde_keep.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"

uint32 entry_search[3] =
{
    186609,
    186610,
    186611
};

class npc_dragonflayer_forge_master : public CreatureScript
{
public:
    npc_dragonflayer_forge_master() : CreatureScript("npc_dragonflayer_forge_master") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_dragonflayer_forge_masterAI(creature);
    }

    struct npc_dragonflayer_forge_masterAI : public ScriptedAI
    {
        npc_dragonflayer_forge_masterAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
            fm_Type = 0;
        }

        InstanceScript* instance;
        uint8 fm_Type;

        void Reset()
        {
            if (fm_Type == 0)
                fm_Type = GetForgeMasterType();

            CheckForge();
        }

        void CheckForge()
        {
            if (instance)
            {
                switch (fm_Type)
                {
                    case 1:
                        instance->SetData(EVENT_FORGE_1, me->isAlive() ? NOT_STARTED : DONE);
                        break;

                    case 2:
                        instance->SetData(EVENT_FORGE_2, me->isAlive() ? NOT_STARTED : DONE);
                        break;

                    case 3:
                        instance->SetData(EVENT_FORGE_3, me->isAlive() ? NOT_STARTED : DONE);
                        break;
                }
            }
        }

        void JustDied(Unit* /*killer*/)
        {
            if (fm_Type == 0)
                fm_Type = GetForgeMasterType();

            if (instance)
            {
                switch (fm_Type)
                {
                    case 1:
                        instance->SetData(EVENT_FORGE_1, DONE);
                        break;

                    case 2:
                        instance->SetData(EVENT_FORGE_2, DONE);
                        break;

                    case 3:
                        instance->SetData(EVENT_FORGE_3, DONE);
                        break;
                }
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (fm_Type == 0)
                fm_Type = GetForgeMasterType();

            if (instance)
            {
                switch (fm_Type)
                {
                    case 1:
                        instance->SetData(EVENT_FORGE_1, IN_PROGRESS);
                        break;

                    case 2:
                        instance->SetData(EVENT_FORGE_2, IN_PROGRESS);
                        break;

                    case 3:
                        instance->SetData(EVENT_FORGE_3, IN_PROGRESS);
                        break;
                }
            }
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
        }

        uint8 GetForgeMasterType()
        {
            float diff = 30.0f;
            uint8 near_f = 0;

            for (uint8 i = 0; i < 3; ++i)
            {
                if (GameObject* go = me->FindNearestGameObject(entry_search[i], 30))
                {
                    if (me->IsWithinDist(go, diff, false))
                    {
                        near_f = i + 1;
                        diff = me->GetDistance2d(go);
                    }
                }
            }

            switch (near_f)
            {
                case 1:  return 1;
                case 2:  return 2;
                case 3:  return 3;
                default: return 0;
            }
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (fm_Type == 0)
                fm_Type = GetForgeMasterType();

            if (!UpdateVictim())
                return;

            DoMeleeAttackIfReady();
        }
    };
};

enum TickingTimeBomb
{
    SPELL_TICKING_TIME_BOMB_EXPLODE = 59687
};
class spell_ticking_time_bomb: public SpellScriptLoader
{
    public:
        spell_ticking_time_bomb() : SpellScriptLoader("spell_ticking_time_bomb") { }

        class spell_ticking_time_bomb_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_ticking_time_bomb_AuraScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                return (bool) sSpellMgr->GetSpellInfo(SPELL_TICKING_TIME_BOMB_EXPLODE);
            }

            void HandleOnEffectRemove(AuraEffect const* /* aurEff */, AuraEffectHandleModes /* mode */)
            {
                if (GetCaster() == GetTarget())
                {
                    GetTarget()->CastSpell(GetTarget(), SPELL_TICKING_TIME_BOMB_EXPLODE, true);
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_ticking_time_bomb_AuraScript::HandleOnEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_ticking_time_bomb_AuraScript();
        }
};

enum Fixate
{
    SPELL_FIXATE_TRIGGER = 40415
};
class spell_fixate: public SpellScriptLoader
{
    public:
        spell_fixate() : SpellScriptLoader("spell_fixate") { }

        class spell_fixate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_fixate_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                return (bool) sSpellMgr->GetSpellInfo(SPELL_FIXATE_TRIGGER);
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                // The unit has to cast the taunt on hisself, but we need the original caster for SPELL_AURA_MOD_TAUNT
                GetCaster()->CastSpell(GetCaster(), SPELL_FIXATE_TRIGGER, true, 0, 0, GetHitUnit()->GetGUID());
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_fixate_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_fixate_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_utgarde_keep()
{
    new npc_dragonflayer_forge_master();
    new spell_ticking_time_bomb();
    new spell_fixate();
}
#endif
