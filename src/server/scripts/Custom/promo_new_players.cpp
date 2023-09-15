////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#include <ScriptPCH.h>
#include <ScriptMgr.h>

class PromotionNewPlayers : public PlayerScript
{
public:
    PromotionNewPlayers() :PlayerScript("PromotionNewPlayers") {}

    enum Values
    {
        January2024 = 1704105600,   ///< Timestamp of 1st of January 2024.
        March232024  = 1679808000,  ///< Timestamp of 23nd of March 2024.
        March152024  = 1678396800,  ///< Timestamp of 15th of March 2024.
        SwiftNetherDrake = 37015,   ///< SpellId.
        ImperialQuilen   = 85870,   ///< ItemId
        ApprenticeRidingSkill = 33388
    };

    void OnLogin(Player* p_Player)
    {
        if (time(NULL) < Values::January2024 && p_Player->GetTotalPlayedTime() == 0)
        {
            p_Player->learnSpell(Values::ApprenticeRidingSkill, false);
            p_Player->learnSpell(Values::SwiftNetherDrake, false);
        }

        if (time(nullptr) < Values::March232024
            && p_Player->GetSession()->GetAccountJoinDate() >= Values::March152024
            && p_Player->GetTotalPlayedTime() == 0)
            p_Player->AddItem(Values::ImperialQuilen, 1);
    }
};

#ifndef __clang_analyzer__
void AddSC_promotion_new_players()
{
    new PromotionNewPlayers();
};
#endif
