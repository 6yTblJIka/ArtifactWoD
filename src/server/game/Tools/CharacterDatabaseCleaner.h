////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#ifndef CHARACTERDATABASECLEANER_H
#define CHARACTERDATABASECLEANER_H

namespace CharacterDatabaseCleaner
{
    enum CleaningFlags
    {
        CLEANING_FLAG_ACHIEVEMENT_PROGRESS  = 0x1,
        CLEANING_FLAG_SKILLS                = 0x2,
        CLEANING_FLAG_SPELLS                = 0x4,
        CLEANING_FLAG_TALENTS               = 0x8,
        CLEANING_FLAG_QUESTSTATUS           = 0x10,
        CLEANING_FLAG_AUTO_LEARNED_SPELLS   = 0x20
    };

    void CleanDatabase();

    void CheckUnique(const char* column, const char* table, bool (*check)(uint32));

    bool SkillCheck(uint32 skill);
    bool SpellCheck(uint32 spell_id);
    bool TalentCheck(uint32 talent_id);
    bool NotAutoLearnedSpell(uint32 spell_id);

    void CleanCharacterSkills();
    void CleanCharacterSpell();
    void CleanCharacterTalent();
    void CleanCharacterQuestStatus();
    void CleanCharacterAutoLearnedSpells();
}

#endif
#endif
