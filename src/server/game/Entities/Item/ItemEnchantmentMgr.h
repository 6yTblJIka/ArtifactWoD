////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _ITEM_ENCHANTMENT_MGR_H
#define _ITEM_ENCHANTMENT_MGR_H

#include "Common.h"
#include "DBCStructure.h"

enum enchantmentRandomType
{
    ENCHANTMENT_RANDOM_SUFFIX,
    ENCHANTMENT_RANDOM_PROPERTY,
};

void LoadRandomEnchantmentsTable();
uint32 GetItemEnchantMod(int32 entry, uint32 type);
uint32 GenerateEnchSuffixFactor(uint32 item_id);
uint32 CalculateEnchantmentBonus(SpellItemEnchantmentEntry const* p_SpellItemEnchantement, uint32 p_Amount, uint8 p_Index, uint8 p_CharacterLevel);
uint32 GetRandomPropertyPoints(uint32 itemLevel, uint32 quality, uint32 inventoryType, uint32 subClass);
#endif
