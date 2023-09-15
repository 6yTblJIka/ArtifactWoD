////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_REACTORAI_H
#define TRINITY_REACTORAI_H

#include "CombatAI.h"

class Unit;

class ReactorAI : public AggressorAI
{
    public:

        explicit ReactorAI(Creature* c) : AggressorAI(c) {}

        void MoveInLineOfSight(Unit*) {};

        static int Permissible(const Creature*);
};
#endif

