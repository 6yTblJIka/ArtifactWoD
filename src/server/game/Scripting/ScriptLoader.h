////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef SC_SCRIPTLOADER_H
#define SC_SCRIPTLOADER_H

void AddScripts();
void AddWorldScripts();
void AddSpellScripts();
#ifndef CROSS
void AddCommandScripts();
#endif /* not CROSS */
void AddWorldScripts();
#ifdef CROSS
void AddCommandScripts();
#endif /* CROSS */
void AddEasternKingdomsScripts();
void AddKalimdorScripts();
void AddOutlandScripts();
void AddNorthrendScripts();
void AddPandarieScripts();
void AddDraenorScripts();
void AddBattlegroundScripts();
void AddOutdoorPvPScripts();
void AddCustomScripts();
#ifndef CROSS
void AddBattlePayScripts();
#endif /* not CROSS */

#endif
