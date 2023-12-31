////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "krasarang_wilds.h"
#include "SpellScript.h"

#define CHECK_STATUS(a) (player->GetQuestStatus(a) == QUEST_STATUS_INCOMPLETE)
#define GOSSIP_CHOICE_1 "Let's fight!"
#define GOSSIP_CHOICE "<Reach out to touch Chi-Ji.>"

// Arness the Scale - 50787
class mob_arness_the_scale : public CreatureScript
{
    public:
        mob_arness_the_scale() : CreatureScript("mob_arness_the_scale")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_arness_the_scaleAI(creature);
        }

        struct mob_arness_the_scaleAI : public ScriptedAI
        {
            mob_arness_the_scaleAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 40000);
                events.ScheduleEvent(EVENT_VANISH, 20000);
                events.ScheduleEvent(EVENT_VICIOUS_REND, 15000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GRAPPLING_HOOK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GRAPPLING_HOOK, false);
                            events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 40000);
                            break;
                        case EVENT_VANISH:
                            me->CastSpell(me, SPELL_VANISH, false);
                            events.ScheduleEvent(EVENT_VANISH, 40000);
                            break;
                        case EVENT_VICIOUS_REND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VICIOUS_REND, false);
                            events.ScheduleEvent(EVENT_VICIOUS_REND, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Gaarn the Toxic - 50340
class mob_gaarn_the_toxic : public CreatureScript
{
    public:
        mob_gaarn_the_toxic() : CreatureScript("mob_gaarn_the_toxic")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gaarn_the_toxicAI(creature);
        }

        struct mob_gaarn_the_toxicAI : public ScriptedAI
        {
            mob_gaarn_the_toxicAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOWBOLT, 15000);
                events.ScheduleEvent(EVENT_VOIDCLOUD, 70000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOWBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOWBOLT, false);
                            events.ScheduleEvent(EVENT_SHADOWBOLT, 15000);
                            break;
                        case EVENT_VOIDCLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VOIDCLOUD, false);
                            events.ScheduleEvent(EVENT_VOIDCLOUD, 70000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Qu Nas - 50352
class mob_qu_nas : public CreatureScript
{
    public:
        mob_qu_nas() : CreatureScript("mob_qu_nas")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_qu_nasAI(creature);
        }

        struct mob_qu_nasAI : public ScriptedAI
        {
            mob_qu_nasAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEVASTATING_ARC, 25000);
                events.ScheduleEvent(EVENT_SUMMON_QUILEN, 15000);
                events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 70000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEVASTATING_ARC:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEVASTATING_ARC, false);
                            events.ScheduleEvent(EVENT_DEVASTATING_ARC, 30000);
                            break;
                        case EVENT_SUMMON_QUILEN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SUMMON_QUILEN, false);
                            events.ScheduleEvent(EVENT_SUMMON_QUILEN, 80000);
                            break;
                        case EVENT_TITANIC_STRENGTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TITANIC_STRENGTH, false);
                            events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 55000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Torik Ethis - 50388
class mob_torik_ethis : public CreatureScript
{
    public:
        mob_torik_ethis() : CreatureScript("mob_torik_ethis")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_torik_ethisAI(creature);
        }

        struct mob_torik_ethisAI : public ScriptedAI
        {
            mob_torik_ethisAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLADE_FURY, 8000);
                events.ScheduleEvent(EVENT_TORNADO, 40000);
                events.ScheduleEvent(EVENT_WINDSONG, 32000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == MOB_TORNADO)
                {
                    summon->AddAura(SPELL_TORNADO_DMG, summon);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->GetMotionMaster()->MoveRandom(20.0f);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLADE_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLADE_FURY, false);
                            events.ScheduleEvent(EVENT_BLADE_FURY, 8000);
                            break;
                        case EVENT_TORNADO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORNADO, false);
                            events.ScheduleEvent(EVENT_TORNADO, 40000);
                            break;
                        case EVENT_WINDSONG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WINDSONG, false);
                            events.ScheduleEvent(EVENT_WINDSONG, 32000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Go Kan - 50331
class mob_go_kan : public CreatureScript
{
    public:
        mob_go_kan() : CreatureScript("mob_go_kan")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_go_kanAI(creature);
        }

        struct mob_go_kanAI : public ScriptedAI
        {
            mob_go_kanAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetStandState(UNIT_STAND_STATE_SIT);
                events.Reset();
                events.ScheduleEvent(EVENT_BELLOWING_RAGE, 8000);
                events.ScheduleEvent(EVENT_RUSHING_CHARGE, 17000);
                events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 25000);
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                me->RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 0, UNIT_STAND_STATE_SIT);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BELLOWING_RAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BELLOWING_RAGE, false);
                            events.ScheduleEvent(EVENT_BELLOWING_RAGE, 35000);
                            break;
                        case EVENT_RUSHING_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RUSHING_CHARGE, false);
                            events.ScheduleEvent(EVENT_RUSHING_CHARGE, 35000);
                            break;
                        case EVENT_YAUNGOL_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_YAUNGOL_STOMP, false);
                            events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 35000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Spirit of the Crane - 60487
class mob_spirit_of_the_crane : public CreatureScript
{
    public:
        mob_spirit_of_the_crane() : CreatureScript("mob_spirit_of_the_crane")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(30716) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(60487);
                creature->DespawnOrUnsummon();
            }

            return true;
        }
};

// Champion of Chi Ji - 60546
class mob_champion_of_chi_ji : public CreatureScript
{
    public:
        mob_champion_of_chi_ji() : CreatureScript("mob_champion_of_chi_ji")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_champion_of_chi_jiAI(creature);
        }

        struct mob_champion_of_chi_jiAI : public ScriptedAI
        {
            mob_champion_of_chi_jiAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_CHI_TORPEDO, 4000);
                events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 17000);
                events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 27000);
                events.ScheduleEvent(EVENT_UPPERCUT, 35000);
            }

            void MoveInLineOfSight(Unit* who) // Dynamic Aggro !
            {
                if (who->IsPlayer())
                {
                    if (CAST_PLR(who)->GetQuestStatus(30740) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (me->GetDistance(who) >= 10.0f && me->GetDistance(who) <= 40.0f)
                            me->CastSpell(who, SPELL_FLYING_SERPENT_KICK, false);
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CHI_TORPEDO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CHI_TORPEDO, false);
                            events.ScheduleEvent(EVENT_CHI_TORPEDO, 40000);
                            break;
                        case EVENT_HUNDRED_HAND_SLAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HUNDRED_HAND_SLAP, false);
                            events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 40000);
                            break;
                        case EVENT_SPINNING_CRANE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SPINNING_CRANE_KICK, false);
                            events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 40000);
                            break;
                        case EVENT_UPPERCUT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_UPPERCUT, false);
                            events.ScheduleEvent(EVENT_UPPERCUT, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Chi Torpedo - 119539
class spell_chi_torpedo_periodic: public SpellScriptLoader
{
    public:
        spell_chi_torpedo_periodic() : SpellScriptLoader("spell_chi_torpedo_periodic") { }

        class spell_chi_torpedo_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_chi_torpedo_periodic_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = caster->getVictim())
                    {
                        caster->CastSpell(target, 119520, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_chi_torpedo_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_chi_torpedo_periodic_AuraScript();
        }
};

// Andruin Wrynn - 59608
class mob_anduin_wrynn : public CreatureScript
{
    public:
        mob_anduin_wrynn() : CreatureScript("mob_anduin_wrynn")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* /*p_Creature*/, Quest const* p_Quest)
        {
            if (p_Quest->GetQuestId() == QuestInTheHouseOfTheRedCrane)
                p_Player->SummonCreature(NPC_ANDUIN_WRYNN, p_Player->GetPositionX(), p_Player->GetPositionY(), p_Player->GetPositionZ(), 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, p_Player->GetGUID());

            return true;
        }
};

// Andruin Wrynn - 66975
class mob_anduin_wrynn_escort : public CreatureScript
{
    public:
        mob_anduin_wrynn_escort() : CreatureScript("mob_anduin_wrynn_escort")
        {
        }

        struct mob_anduin_wrynn_escortAI : public ScriptedAI
        {
            mob_anduin_wrynn_escortAI(Creature* creature) : ScriptedAI(creature)
            {
                m_PlayerGuid = 0;
            }

            uint64 m_PlayerGuid;
            EventMap m_Events;

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Events.Reset();

                if (Player* l_Plr = p_Summoner->ToPlayer())
                {
                    if (l_Plr->GetQuestStatus(QuestInTheHouseOfTheRedCrane) == QUEST_STATUS_INCOMPLETE)
                    {
                        me->GetMotionMaster()->MoveFollow(l_Plr, 2.0f, 2.0f, MOTION_SLOT_ACTIVE);
                        m_PlayerGuid = l_Plr->GetGUID();
                        m_Events.ScheduleEvent(EVENT_CHECK_TARGET, 1000);
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_CHECK_TARGET)
                {
                    if (Player* l_Summoner = sObjectAccessor->FindPlayer(m_PlayerGuid))
                    {
                        if (Unit* l_Target = l_Summoner->getVictim())
                        {
                            if (Creature* l_Creature = l_Target->ToCreature())
                            {
                                if (l_Creature->GetEntry() == 59651)
                                {
                                    me->Attack(l_Target, true);
                                    DoMeleeAttackIfReady();
                                }
                            }
                        }

                        m_Events.ScheduleEvent(EVENT_CHECK_TARGET, 1000);
                    }
                    else
                        me->DespawnOrUnsummon();

                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_anduin_wrynn_escortAI(creature);
        }
};

// Chi Ji - 59653
class npc_chi_ji : public CreatureScript
{
    public:
        npc_chi_ji() : CreatureScript("npc_chi_ji")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(30273) == QUEST_STATUS_INCOMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(30003, creature->GetGUID());
            }

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();
                creature->DespawnOrUnsummon();
                player->SummonCreature(59651, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ()); // , 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
            }

            return true;
        }
};

// Sha of Despair - 59651
class mob_sha_of_despair : public CreatureScript
{
    public:
        mob_sha_of_despair() : CreatureScript("mob_sha_of_despair")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_of_despairAI(creature);
        }

        struct mob_sha_of_despairAI : public ScriptedAI
        {
            mob_sha_of_despairAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                me->DespawnOrUnsummon();
                me->SummonCreature(59653, -1813.46f, 1052.33f, -31.115f, 0.0f, TEMPSUMMON_MANUAL_DESPAWN);

                std::list<Creature*> creatureList;
                GetCreatureListWithEntryInGrid(creatureList, me, 66975, 40.0f);

                for (auto creature: creatureList)
                    creature->DespawnOrUnsummon();
            }
        };
};

// Thelonius - 60506
class npc_thelonius : public CreatureScript
{
    public:
        npc_thelonius() : CreatureScript("npc_thelonius")
        {
        }

        bool OnQuestAccept(Player* player, Creature* /*p_Creature*/, Quest const* quest)
        {
            switch (quest->GetQuestId())
            {
                case 30725:
                    player->SummonCreature(60530, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30739:
                    player->SummonCreature(60545, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30727:
                    player->SummonCreature(60533, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30732:
                    player->SummonCreature(60538, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30728:
                    player->SummonCreature(60534, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30737:
                    player->SummonCreature(60543, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30734:
                    player->SummonCreature(60540, -2322.529f, 1624.929f, 0.381f, 5.288279f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30730:
                    player->SummonCreature(60536, -2615.302f, 1457.653f, 1.537f, 2.407458f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30729:
                    player->SummonCreature(60535, -2471.108f, 1594.382f, 0.267f, 5.878917f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30731:
                    player->SummonCreature(60537, -2473.856f, 1241.124f, 36.133f, 1.293773f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30735:
                    player->SummonCreature(60541, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30726:
                    player->SummonCreature(60532, -2502.585f, 1449.352f, 15.786f, 0.030857f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30738:
                    player->SummonCreature(60544, -2322.529f, 1624.929f, 0.381f, 5.288279f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30733:
                    player->SummonCreature(60539, -2457.983f, 1352.873f, 20.287f, 0.202678f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30736:
                    player->SummonCreature(60542, -2473.856f, 1241.124f, 36.133f, 1.293773f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                default:
                    break;
            }

            return true;
        }
};

//Ella Ravenmane - 60530/60545/60533/60538
class mob_ellia_ravenmane : public CreatureScript
{
    public:
        mob_ellia_ravenmane() : CreatureScript("mob_ellia_ravenmane")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30725) || CHECK_STATUS(30739) || CHECK_STATUS(30727) || CHECK_STATUS(30732))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ellia_ravenmaneAI(creature);
        }

        struct mob_ellia_ravenmaneAI : public ScriptedAI
        {
            mob_ellia_ravenmaneAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                switch (me->GetEntry())
                {
                    case 60530:
                        events.ScheduleEvent(EVENT_JAB, 3000);
                        events.ScheduleEvent(EVENT_KICK, 6000);
                        break;
                    case 60545:
                        events.ScheduleEvent(EVENT_CRY_OUT_IN_DESPAIR, 3000);
                        events.ScheduleEvent(EVENT_SHA_BLAST, 8000);
                        events.ScheduleEvent(EVENT_SHA_SPIKE, 13000);
                        break;
                    case 60533:
                        events.ScheduleEvent(EVENT_FIERCE_JAB, 3000);
                        events.ScheduleEvent(EVENT_FIERCE_KICK, 6000);
                        break;
                    case 60538:
                        events.ScheduleEvent(EVENT_FIERCE_JAB_2, 3000);
                        events.ScheduleEvent(EVENT_FISTS_OF_FURY, 8000);
                        events.ScheduleEvent(EVENT_VICIOUS_KICK, 13000);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    switch (me->GetEntry())
                    {
                        case 60530:
                            if (CHECK_STATUS(30725))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60530);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60545:
                            if (CHECK_STATUS(30739))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60545);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60533:
                            if (CHECK_STATUS(30727))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60533);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60538:
                            if (CHECK_STATUS(30732))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60538);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        default:
                            break;
                        
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    switch (me->GetEntry())
                    {
                        case 60530:
                            if (CHECK_STATUS(30725))
                                me->DespawnOrUnsummon();
                            break;
                        case 60545:
                            if (CHECK_STATUS(30739))
                                me->DespawnOrUnsummon();
                            break;
                        case 60533:
                            if (CHECK_STATUS(30727))
                                me->DespawnOrUnsummon();
                            break;
                        case 60538:
                            if (CHECK_STATUS(30732))
                                me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Entry 60530
                        case EVENT_JAB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_JAB, false);
                            events.ScheduleEvent(EVENT_JAB, 7000);
                            break;
                        case EVENT_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_KICK, false);
                            events.ScheduleEvent(EVENT_KICK, 7000);
                            break;
                            // Entry 60545
                        case EVENT_CRY_OUT_IN_DESPAIR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_CRY_OUT_IN_DESPAIR, false);
                            events.ScheduleEvent(EVENT_CRY_OUT_IN_DESPAIR, 15000);
                            break;
                        case EVENT_SHA_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_BLAST, false);
                            events.ScheduleEvent(EVENT_SHA_BLAST, 15000);
                            break;
                        case EVENT_SHA_SPIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_SPIKE, false);
                            events.ScheduleEvent(EVENT_SHA_SPIKE, 15000);
                            break;
                            // Entry 60533
                        case EVENT_FIERCE_JAB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FIERCE_JAB, false);
                            events.ScheduleEvent(EVENT_FIERCE_JAB, 7000);
                            break;
                        case EVENT_FIERCE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FIERCE_KICK, false);
                            events.ScheduleEvent(EVENT_FIERCE_KICK, 7000);
                            break;
                            // Entry 60538
                        case EVENT_FIERCE_JAB_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FIERCE_JAB, false);
                            events.ScheduleEvent(EVENT_FIERCE_JAB_2, 15000);
                            break;
                        case EVENT_FISTS_OF_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FISTS_OF_FURY, false);
                            events.ScheduleEvent(EVENT_FISTS_OF_FURY, 15000);
                            break;
                        case EVENT_VICIOUS_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_VICIOUS_KICK, false);
                            events.ScheduleEvent(EVENT_VICIOUS_KICK, 15000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Fat Long Fat - 60534/60543
class mob_fat_long_fat : public CreatureScript
{
    public:
        mob_fat_long_fat() : CreatureScript("mob_fat_long_fat")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30728) || CHECK_STATUS(30737))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_fat_long_fatAI(creature);
        }

        struct mob_fat_long_fatAI : public ScriptedAI
        {
            mob_fat_long_fatAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                switch (me->GetEntry())
                {
                    case 60534:
                        events.ScheduleEvent(EVENT_BREATH_OF_FIRE, 3000);
                        events.ScheduleEvent(EVENT_STORM_STOUT, 6000);
                        break;
                    case 60543:
                        events.ScheduleEvent(EVENT_FAT_LONG_FLOP, 3000);
                        events.ScheduleEvent(EVENT_BREATH_OF_FIRE_2, 8000);
                        events.ScheduleEvent(EVENT_STORM_STOUT_2, 13000);
                        break;
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    switch (me->GetEntry())
                    {
                        case 60534:
                            if (CHECK_STATUS(30728))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60534);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60543:
                            if (CHECK_STATUS(30737))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60543);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    switch (me->GetEntry())
                    {
                        case 60534:
                            if (!CHECK_STATUS(30728))
                                me->DespawnOrUnsummon();
                            break;
                        case 60543:
                            if (!CHECK_STATUS(30737))
                                me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Entry 60534
                        case EVENT_BREATH_OF_FIRE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BREATH_OF_FIRE, false);
                            events.ScheduleEvent(EVENT_BREATH_OF_FIRE, 7000);
                            break;
                        case EVENT_STORM_STOUT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_STORM_STOUT, false);
                            events.ScheduleEvent(EVENT_STORM_STOUT, 7000);
                            break;
                            // Entry 60543
                        case EVENT_FAT_LONG_FLOP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FAT_LONG_FLOP, false);
                            events.ScheduleEvent(EVENT_FAT_LONG_FLOP, 15000);
                            break;
                        case EVENT_BREATH_OF_FIRE_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_BREATH_OF_FIRE, false);
                            events.ScheduleEvent(EVENT_BREATH_OF_FIRE_2, 15000);
                            break;
                        case EVENT_STORM_STOUT_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_STORM_STOUT, false);
                            events.ScheduleEvent(EVENT_STORM_STOUT_2, 15000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Huck Wheelbarrow - 60540
class mob_huck_wheelbarrow : public CreatureScript
{
    public:
        mob_huck_wheelbarrow() : CreatureScript("mob_huck_wheelbarrow")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30734))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_huck_wheelbarrowAI(creature);
        }

        struct mob_huck_wheelbarrowAI : public ScriptedAI
        {
            mob_huck_wheelbarrowAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_WHEELBARROW_ROLL_2, 3000);
                events.ScheduleEvent(EVENT_WHEEL_PUNCH, 8000);
                events.ScheduleEvent(EVENT_ROLLING_THROW, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30734))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60540);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30734))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_WHEELBARROW_ROLL_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WHEELBARROW_ROLL_2, false);
                            events.ScheduleEvent(EVENT_WHEELBARROW_ROLL_2, 18000);
                            break;
                        case EVENT_WHEEL_PUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_WHEEL_PUNCH, false);
                            events.ScheduleEvent(EVENT_WHEEL_PUNCH, 18000);
                            break;
                        case EVENT_ROLLING_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_ROLLING_THROW, false);
                            events.ScheduleEvent(EVENT_ROLLING_THROW, 18000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Dextrous Izissha - 60536
class mob_dextrous_izissha : public CreatureScript
{
    public:
        mob_dextrous_izissha() : CreatureScript("mob_dextrous_izissha")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30730))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dextrous_izisshaAI(creature);
        }

        struct mob_dextrous_izisshaAI : public ScriptedAI
        {
            mob_dextrous_izisshaAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_EVASION, 3000);
                events.ScheduleEvent(EVENT_FLUID_LIKE_WATR, 8000);
                events.ScheduleEvent(EVENT_STILL_WATERS_RUN_DEEP, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30730))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60536);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30730))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_EVASION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_EVASION, false);
                            events.ScheduleEvent(EVENT_EVASION, 18000);
                            break;
                        case EVENT_FLUID_LIKE_WATR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FLUID_LIKE_WATR, false);
                            events.ScheduleEvent(EVENT_FLUID_LIKE_WATR, 18000);
                            break;
                        case EVENT_STILL_WATERS_RUN_DEEP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_STILL_WATERS_RUN_DEEP, false);
                            events.ScheduleEvent(EVENT_STILL_WATERS_RUN_DEEP, 18000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Julia Bates - 60535
class mob_julia_bates : public CreatureScript
{
    public:
        mob_julia_bates() : CreatureScript("mob_julia_bates")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30729))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_julia_batesAI(creature);
        }

        struct mob_julia_batesAI : public ScriptedAI
        {
            mob_julia_batesAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 3000);
                events.ScheduleEvent(EVENT_SOOTHING_MIST, 8000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30729))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60535);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30729))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_HUNDRED_HAND_SLAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HUNDRED_HAND_SLAP, false);
                            events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 7000);
                            break;
                        case EVENT_SOOTHING_MIST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_SOOTHING_MIST, false);
                            events.ScheduleEvent(EVENT_SOOTHING_MIST, 7000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Kuo-Na Quillpaw - 60537
class mob_kuo_na : public CreatureScript
{
    public:
        mob_kuo_na() : CreatureScript("mob_kuo_na")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30731))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kuo_naAI(creature);
        }

        struct mob_kuo_naAI : public ScriptedAI
        {
            mob_kuo_naAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK_2, 3000);
                events.ScheduleEvent(EVENT_PAW_PUNCH, 8000);
                events.ScheduleEvent(EVENT_UPPERCUT_2, 15000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30731))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60537);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30731))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FLYING_SERPENT_KICK_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FLYING_SERPENT_KICK, false);
                            events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK_2, 21000);
                            break;
                        case EVENT_PAW_PUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_PAW_PUNCH, false);
                            events.ScheduleEvent(EVENT_PAW_PUNCH, 21000);
                            break;
                        case EVENT_UPPERCUT_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_UPPERCUT, false);
                            events.ScheduleEvent(EVENT_UPPERCUT_2, 21000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Mindel Sunspeaker - 60541
class mob_mindel_sunspeaker : public CreatureScript
{
    public:
        mob_mindel_sunspeaker() : CreatureScript("mob_mindel_sunspeaker")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30735))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mindel_sunspeakerAI(creature);
        }

        struct mob_mindel_sunspeakerAI : public ScriptedAI
        {
            mob_mindel_sunspeakerAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SUNFLARE_KICK, 3000);
                events.ScheduleEvent(EVENT_SUNSTRIKE, 8000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30735))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60541);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30735))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUNFLARE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SUNFLARE_KICK, false);
                            events.ScheduleEvent(EVENT_SUNFLARE_KICK, 12000);
                            break;
                        case EVENT_SUNSTRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_SUNSTRIKE, false);
                            events.ScheduleEvent(EVENT_SUNSTRIKE, 12000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Minh Do-Tan - 60532
class mob_minh_do_tan : public CreatureScript
{
    public:
        mob_minh_do_tan() : CreatureScript("mob_minh_do_tan")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30726))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_minh_do_tanAI(creature);
        }

        struct mob_minh_do_tanAI : public ScriptedAI
        {
            mob_minh_do_tanAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SUNFLARE_KICK, 3000);
                events.ScheduleEvent(EVENT_SUNSTRIKE, 8000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30726))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60532);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30726))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SPINNING_CRANE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SPINNING_CRANE_KICK, false);
                            events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 12000);
                            break;
                        case EVENT_VICIOUS_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_VICIOUS_KICK, false);
                            events.ScheduleEvent(EVENT_VICIOUS_KICK, 12000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Thelonius - 60544
class mob_thelonius : public CreatureScript
{
    public:
        mob_thelonius() : CreatureScript("mob_thelonius")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30738))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_theloniusAI(creature);
        }

        struct mob_theloniusAI : public ScriptedAI
        {
            mob_theloniusAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_UPPERCUT, 3000);
                events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 8000);
                events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 13000);
                events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK, 20000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30738))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60544);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30738))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_UPPERCUT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_UPPERCUT, false);
                            events.ScheduleEvent(EVENT_UPPERCUT, 25000);
                            break;
                        case EVENT_SPINNING_CRANE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_SPINNING_CRANE_KICK, false);
                            events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 25000);
                            break;
                        case EVENT_HUNDRED_HAND_SLAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HUNDRED_HAND_SLAP, false);
                            events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 25000);
                            break;
                        case EVENT_FLYING_SERPENT_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_FLYING_SERPENT_KICK, false);
                            events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK, 25000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Tukka-Tuk - 60539
class mob_tukka_tuk : public CreatureScript
{
    public:
        mob_tukka_tuk() : CreatureScript("mob_tukka_tuk")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30733))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_tukka_tukAI(creature);
        }

        struct mob_tukka_tukAI : public ScriptedAI
        {
            mob_tukka_tukAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_POUND_YOU, 3000);
                events.ScheduleEvent(EVENT_KICK_YOU, 8000);
                events.ScheduleEvent(EVENT_PUNCH_YOU, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30733))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60539);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30733))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_POUND_YOU:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_POUND_YOU, false);
                            events.ScheduleEvent(EVENT_POUND_YOU, 20000);
                            break;
                        case EVENT_KICK_YOU:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_KICK_YOU, false);
                            events.ScheduleEvent(EVENT_KICK_YOU, 20000);
                            break;
                        case EVENT_PUNCH_YOU:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_PUNCH_YOU, false);
                            events.ScheduleEvent(EVENT_PUNCH_YOU, 20000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Yan Quillpaw - 60542
class mob_yan_quillpaw : public CreatureScript
{
    public:
        mob_yan_quillpaw() : CreatureScript("mob_yan_quillpaw")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30736))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_yan_quillpawAI(creature);
        }

        struct mob_yan_quillpawAI : public ScriptedAI
        {
            mob_yan_quillpawAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_UPPERCUT, 3000);
                events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK, 8000);
                events.ScheduleEvent(EVENT_PAW_PUNCH, 13000);
            }

            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (CHECK_STATUS(30736))
                    {
                        if (damage > me->GetHealth())
                        {
                            damage = 0;
                            me->SetFullHealth();
                            DoAction(ACTION_REINITIALIZE);
                            player->KilledMonsterCredit(60542);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                events.ScheduleEvent(EVENT_DESPAWN, 600000);
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }

                    if (!CHECK_STATUS(30736))
                        me->DespawnOrUnsummon();
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_UPPERCUT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_UPPERCUT, false);
                            events.ScheduleEvent(EVENT_UPPERCUT, 20000);
                            break;
                        case EVENT_FLYING_SERPENT_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(me, SPELL_FLYING_SERPENT_KICK, false);
                            events.ScheduleEvent(EVENT_FLYING_SERPENT_KICK, 20000);
                            break;
                        case EVENT_PAW_PUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_PAW_PUNCH, false);
                            events.ScheduleEvent(EVENT_PAW_PUNCH, 20000);
                            break;
                        case EVENT_DESPAWN:
                            me->DespawnOrUnsummon();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};


/// Signal Fire - 216274
class gob_signal_fire_krasarang : public GameObjectScript
{
public:
	gob_signal_fire_krasarang() : GameObjectScript("gob_signal_fire_krasarang") { }

	bool OnGossipHello(Player* p_Player, GameObject* p_GameObject) override
	{
		if (p_Player->GetQuestStatus(31483) == QUEST_STATUS_INCOMPLETE)
		{
			p_Player->KilledMonsterCredit(64681);
			p_Player->SendMovieStart(128);
		}

		return true;
	}

};

// Elder Anli - 58564
class npc_scout_layna : public CreatureScript
{
public:
	npc_scout_layna() : CreatureScript("npc_scout_layna") { }

	bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest const* p_Quest, uint32 p_Options) override
	{
		switch (p_Quest->GetQuestId())
		{
			/// Quest Meet the scout should launch an event at reward, can't find enough infos to script it atm
		case 32246: ///< Alliance
			break;
		case 32249: ///< Horde
			break;
		default:
			break;
		}

		return true;
	}
};


// Yan Quillpaw - 60542
class npc_warlord_bloodhilt : public CreatureScript
{
public:
	npc_warlord_bloodhilt() : CreatureScript("npc_warlord_bloodhilt")
	{
	}

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_warlord_bloodhiltAI(creature);
	}

	struct npc_warlord_bloodhiltAI : public ScriptedAI
	{
		npc_warlord_bloodhiltAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap m_Events;

		enum eEvents
		{
			EventBloodThistyStrike = 1,
			EventBloodyKnives,
			EventBrutalCharge,
			EventChokeEmOut,
			EventCoupDeGrace
		};

		enum eSpells
		{
			SpellBloodThistyStrike = 135396,
			SpellBloodyKnives = 135343,
			SpellBrutalCharge = 135421,
			SpellChokeEmOut = 135443,
			SpellCoupDeGrace = 135397
		};

		void Reset()
		{
			m_Events.Reset();
		}

		void EnterCombat(Unit* p_Attacker)
		{
			m_Events.ScheduleEvent(eEvents::EventBloodThistyStrike, 3000);
			m_Events.ScheduleEvent(eEvents::EventBloodyKnives, 8000);
			m_Events.ScheduleEvent(eEvents::EventBrutalCharge, 13000);
			m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 18000);
			m_Events.ScheduleEvent(eEvents::EventCoupDeGrace, 23000);
		}

		void UpdateAI(const uint32 p_Diff)
		{
			if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
				return;

			m_Events.Update(p_Diff);

			switch (m_Events.ExecuteEvent())
			{
			case eEvents::EventBloodThistyStrike:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
					me->CastSpell(l_Target, eSpells::SpellBloodThistyStrike, false);
				m_Events.ScheduleEvent(eEvents::EventBloodThistyStrike, 30000);
				break;
			case eEvents::EventBloodyKnives:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
					me->CastSpell(l_Target, eSpells::SpellBloodyKnives, false);
				m_Events.ScheduleEvent(eEvents::EventBloodyKnives, 30000);
				break;
			case eEvents::EventBrutalCharge:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
					me->CastSpell(l_Target, eSpells::SpellBrutalCharge, false);
				m_Events.ScheduleEvent(eEvents::EventBrutalCharge, 30000);
				break;
			case eEvents::EventChokeEmOut:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
					me->CastSpell(l_Target, eSpells::SpellChokeEmOut, false);
				m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 30000);
				break;
			case eEvents::EventCoupDeGrace:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
					me->CastSpell(l_Target, eSpells::SpellCoupDeGrace, false);
				m_Events.ScheduleEvent(eEvents::EventCoupDeGrace, 30000);
				break;
			default:
				break;
			}

			DoMeleeAttackIfReady();
		}
	};
};

class npc_high_marshal_twinbraid : public CreatureScript
{
public:
	npc_high_marshal_twinbraid() : CreatureScript("npc_high_marshal_twinbraid")
	{
	}

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_high_marshal_twinbraidAI(creature);
	}

	struct npc_high_marshal_twinbraidAI : public ScriptedAI
	{
		npc_high_marshal_twinbraidAI(Creature* creature) : ScriptedAI(creature)
		{
		}

		EventMap m_Events;

		enum eEvents
		{
			EventCannonballSpin = 1,
			EventChaingun,
			EventExplosivesPlunger,
			EventChokeEmOut
		};

		enum eSpells
		{
			SpellCannonballSpin = 135154,
			SpellChaingun = 135417,
			SpellExplosivesPlunger = 135292, ///< self
			SpellSoundTheAlarm = 135166  ///< self
		};

		void Reset()
		{
			m_Events.Reset();
		}

		void EnterCombat(Unit* p_Attacker)
		{
			m_Events.ScheduleEvent(eEvents::EventCannonballSpin, 3000);
			m_Events.ScheduleEvent(eEvents::EventChaingun, 8000);
			///                m_Events.ScheduleEvent(eEvents::EventExplosivesPlunger, 13000);
			m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 18000);
		}

		void EnterEvadeMode()
		{
			Reset();
		}

		void UpdateAI(const uint32 p_Diff)
		{
			if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
				return;

			m_Events.Update(p_Diff);

			switch (m_Events.ExecuteEvent())
			{
			case eEvents::EventCannonballSpin:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
					me->CastSpell(l_Target, eSpells::SpellCannonballSpin, false);
				m_Events.ScheduleEvent(eEvents::EventCannonballSpin, 30000);
				break;
			case eEvents::EventChaingun:
				if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
					me->CastSpell(l_Target, eSpells::SpellChaingun, false);
				m_Events.ScheduleEvent(eEvents::EventChaingun, 30000);
				break;
			case eEvents::EventExplosivesPlunger:
				me->CastSpell(me, eSpells::SpellExplosivesPlunger, false); ///< TO SCRIPT
				m_Events.ScheduleEvent(eEvents::EventExplosivesPlunger, 30000);
				break;
			case eEvents::EventChokeEmOut:
				me->CastSpell(me, eSpells::SpellSoundTheAlarm, false); ///< To check
				m_Events.ScheduleEvent(eEvents::EventChokeEmOut, 30000);
				break;
			default:
				break;
			}

			DoMeleeAttackIfReady();
		}
	};
};
/*
/// Krasarang entering
class playerScript_mapshift_krasarang : public PlayerScript
{
public:
	playerScript_mapshift_krasarang() : PlayerScript("playerScript_mapshift_krasarang") { }

	void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneId, uint32 p_NewAreaId) override
	{
		if (p_Player && p_NewZoneId == 6134) ///< Krasarang Wilds
		{
			std::set<uint32> l_PhaseId, l_Terrainswap;

			uint16 l_AllianceMapID = 1062; ///< 1103 seems also to be correct, can't find exact difference between those two ones
			uint16 l_HordeMapID = 1063; ///< 1102 seems also to be correct, can't find exact difference between those two ones
			l_Terrainswap.insert((uint32)l_AllianceMapID);
			l_Terrainswap.insert((uint32)l_HordeMapID);
			p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap);

			if (p_NewAreaId == 6369)
			{
			}
			//                 else if (p_NewAreaId == 6612)
			//                 {
			//                     l_PhaseId.insert(0);
			//                     p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap);
			//                 }
		}
	}
}; */

// Spirit of the Crane - 60487
class npc_hilda_hornswaggle : public CreatureScript
{
public:
	npc_hilda_hornswaggle() : CreatureScript("npc_hilda_hornswaggle")
	{
	}

	bool OnGossipHello(Player* p_Player, Creature* p_Creature)
	{
		if (p_Player->GetQuestStatus(eQuests::QuestAKingAmongMen) == QUEST_STATUS_INCOMPLETE)
			p_Player->KilledMonsterCredit(p_Creature->GetEntry());

		return true;
	}
};

// Spirit of the Crane - 60487
class npc_marshall_troteman : public CreatureScript
{
public:
	npc_marshall_troteman() : CreatureScript("npc_marshall_troteman")
	{
	}

	bool OnGossipHello(Player* p_Player, Creature* p_Creature)
	{
		if (p_Player->GetQuestStatus(eQuests::QuestAKingAmongMen) == QUEST_STATUS_INCOMPLETE)
			p_Player->KilledMonsterCredit(p_Creature->GetEntry());

		return true;
	}
};






#ifndef __clang_analyzer__
void AddSC_krasarang_wilds()
{
    new mob_gaarn_the_toxic();
    new mob_arness_the_scale();
    new mob_qu_nas();
    new mob_torik_ethis();
    new mob_go_kan();
    new mob_spirit_of_the_crane();
    new mob_champion_of_chi_ji();
    new spell_chi_torpedo_periodic();
    new mob_anduin_wrynn();
    new mob_anduin_wrynn_escort();
    new npc_chi_ji();
    new mob_sha_of_despair();
    new npc_thelonius();
    new mob_ellia_ravenmane();
    new mob_fat_long_fat();
    new mob_huck_wheelbarrow();
    new mob_dextrous_izissha();
    new mob_julia_bates();
    new mob_kuo_na();
    new mob_mindel_sunspeaker();
    new mob_minh_do_tan();
    new mob_thelonius();
    new mob_tukka_tuk();
    new mob_yan_quillpaw();

	new gob_signal_fire_krasarang();
	new npc_scout_layna();
	new npc_warlord_bloodhilt();
	new npc_high_marshal_twinbraid();
	new npc_hilda_hornswaggle();
	new npc_marshall_troteman();
	/// new playerScript_mapshift_krasarang(); ///< Causes client crash, can't add 2 terrainswaps
}
#endif
