////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "naxxramas.h"

enum Spells
{
    SPELL_NECROTIC_AURA                     = 55593,
    SPELL_WARN_NECROTIC_AURA                = 59481,
    SPELL_SUMMON_SPORE                      = 29234,
    SPELL_DEATHBLOOM                        = 29865,
    H_SPELL_DEATHBLOOM                      = 55053,
    SPELL_INEVITABLE_DOOM                   = 29204,
    H_SPELL_INEVITABLE_DOOM                 = 55052
};

enum Texts
{
    SAY_NECROTIC_AURA_APPLIED       = 0,
    SAY_NECROTIC_AURA_REMOVED       = 1,
    SAY_NECROTIC_AURA_FADING        = 2
};

enum Events
{
    EVENT_NECROTIC_AURA             = 1,
    EVENT_DEATHBLOOM                = 2,
    EVENT_INEVITABLE_DOOM           = 3,
    EVENT_SPORE                     = 4,
    EVENT_NECROTIC_AURA_FADING      = 5
};

enum Achievement
{
    DATA_ACHIEVEMENT_SPORE_LOSER    = 21822183
};

class boss_loatheb : public CreatureScript
{
    public:
        boss_loatheb() : CreatureScript("boss_loatheb") { }

        struct boss_loathebAI : public BossAI
        {
            boss_loathebAI(Creature* creature) : BossAI(creature, BOSS_LOATHEB)
            {
            }

            void Reset()
            {
                _Reset();
                _doomCounter = 0;
                _sporeLoserData = true;
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                events.ScheduleEvent(EVENT_NECROTIC_AURA, 17000);
                events.ScheduleEvent(EVENT_DEATHBLOOM, 5000);
                events.ScheduleEvent(EVENT_SPORE, IsHeroic() ? 18000 : 36000);
                events.ScheduleEvent(EVENT_INEVITABLE_DOOM, 120000);
            }

            void SummonedCreatureDies(Creature* /*summon*/, Unit* /*killer*/)
            {
                _sporeLoserData = false;
            }

            uint32 GetData(uint32 id)
            {
                if (id != DATA_ACHIEVEMENT_SPORE_LOSER)
                   return 0;

                return uint32(_sporeLoserData);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_NECROTIC_AURA:
                            DoCastAOE(SPELL_NECROTIC_AURA);
                            DoCast(me, SPELL_WARN_NECROTIC_AURA);
                            events.ScheduleEvent(EVENT_NECROTIC_AURA, 20000);
                            events.ScheduleEvent(EVENT_NECROTIC_AURA_FADING, 14000);
                            break;
                        case EVENT_DEATHBLOOM:
                            DoCastAOE(RAID_MODE(SPELL_DEATHBLOOM, H_SPELL_DEATHBLOOM));
                            events.ScheduleEvent(EVENT_DEATHBLOOM, 30000);
                            break;
                        case EVENT_INEVITABLE_DOOM:
                            _doomCounter++;
                            DoCastAOE(RAID_MODE(SPELL_INEVITABLE_DOOM, H_SPELL_INEVITABLE_DOOM));
                            events.ScheduleEvent(EVENT_INEVITABLE_DOOM, std::max(120000 - _doomCounter * 15000, 15000)); // needs to be confirmed
                            break;
                        case EVENT_SPORE:
                            DoCast(me, SPELL_SUMMON_SPORE, false);
                            events.ScheduleEvent(EVENT_SPORE, IsHeroic() ? 18000 : 36000);
                            break;
                        case EVENT_NECROTIC_AURA_FADING:
                            Talk(SAY_NECROTIC_AURA_FADING);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            bool _sporeLoserData;
            uint8 _doomCounter;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_loathebAI(creature);
        }
};

class achievement_spore_loser : public AchievementCriteriaScript
{
    public:
        achievement_spore_loser() : AchievementCriteriaScript("achievement_spore_loser") { }

        bool OnCheck(Player* /*source*/, Unit* target)
        {
            return target && target->GetAI()->GetData(DATA_ACHIEVEMENT_SPORE_LOSER);
        }
};

typedef boss_loatheb::boss_loathebAI LoathebAI;

class spell_loatheb_necrotic_aura_warning: public SpellScriptLoader
{
    public:
        spell_loatheb_necrotic_aura_warning() : SpellScriptLoader("spell_loatheb_necrotic_aura_warning") { }

        class spell_loatheb_necrotic_aura_warning_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_loatheb_necrotic_aura_warning_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_WARN_NECROTIC_AURA))
                    return false;
                return true;
            }

            void HandleEffectApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->IsAIEnabled)
                    CAST_AI(LoathebAI, GetTarget()->GetAI())->Talk(SAY_NECROTIC_AURA_APPLIED);
            }

            void HandleEffectRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->IsAIEnabled)
                    CAST_AI(LoathebAI, GetTarget()->GetAI())->Talk(SAY_NECROTIC_AURA_REMOVED);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_loatheb_necrotic_aura_warning_AuraScript::HandleEffectApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_loatheb_necrotic_aura_warning_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_loatheb_necrotic_aura_warning_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_loatheb()
{
    new boss_loatheb();
    new achievement_spore_loser();
    new spell_loatheb_necrotic_aura_warning();
}
#endif
