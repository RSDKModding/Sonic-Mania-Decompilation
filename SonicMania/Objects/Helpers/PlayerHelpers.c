// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlayerHelpers Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPlayerHelpers *PlayerHelpers = NULL;

void PlayerHelpers_Update(void) {}

void PlayerHelpers_LateUpdate(void) {}

void PlayerHelpers_StaticUpdate(void) {}

void PlayerHelpers_Draw(void) {}

void PlayerHelpers_Create(void *data) {}

void PlayerHelpers_StageLoad(void) {}

bool32 PlayerHelpers_CheckAct1(void)
{
    if ((RSDK.CheckSceneFolder("GHZ") && !Zone->actID) || (RSDK.CheckSceneFolder("CPZ") && !Zone->actID) || RSDK.CheckSceneFolder("SPZ1")
        || (RSDK.CheckSceneFolder("FBZ") && !Zone->actID) || RSDK.CheckSceneFolder("PSZ1") || RSDK.CheckSceneFolder("SSZ1")
        || (RSDK.CheckSceneFolder("HCZ") && !Zone->actID) || (RSDK.CheckSceneFolder("MSZ") && !Zone->actID)
        || (RSDK.CheckSceneFolder("OOZ") && !Zone->actID) || RSDK.CheckSceneFolder("LRZ1") || (RSDK.CheckSceneFolder("MMZ") && !Zone->actID)
        || RSDK.CheckSceneFolder("TMZ1")) {
        return true;
    }

    return false;
}
bool32 PlayerHelpers_CheckAct2(void)
{
    if ((RSDK.CheckSceneFolder("GHZ") && Zone->actID == 1) || (RSDK.CheckSceneFolder("CPZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("SPZ2")
        || (RSDK.CheckSceneFolder("FBZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("PSZ2") || RSDK.CheckSceneFolder("SSZ2")
        || (RSDK.CheckSceneFolder("HCZ") && Zone->actID == 1) || (RSDK.CheckSceneFolder("MSZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("OOZ2")
        || RSDK.CheckSceneFolder("LRZ3") || (RSDK.CheckSceneFolder("MMZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("TMZ2")) {
        return true;
    }

    return false;
}

bool32 PlayerHelpers_CheckIntro(void) { return isMainGameMode() && globals->enableIntro && !PlayerHelpers_CheckStageReload(); }

bool32 PlayerHelpers_CheckAct1Regular(void) { return isMainGameMode() && PlayerHelpers_CheckAct1(); }

bool32 PlayerHelpers_CheckStageReload(void)
{
    if (StarPost && Player->playerCount > 0) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            if (StarPost->postIDs[p])
                return true;
        }
    }

    if (SpecialRing && globals->specialRingID > 0) {
        foreach_all(SpecialRing, specialRing)
        {
            if (specialRing->id > 0 && globals->specialRingID == specialRing->id)
                return true;
        }
    }

    return false;
}
bool32 PlayerHelpers_CheckPlayerPos(int32 x1, int32 y1, int32 x2, int32 y2)
{
    int32 playerX = 0;
    int32 playerY = 0;

    if (Player) {
        foreach_all(Player, player)
        {
            if ((globals->playerID & player->characterID) > 0) {
                playerX = player->position.x;
                playerY = player->position.y;
            }
        }
    }

    return playerX >= x1 && playerY >= y1 && playerX <= x2 && playerY <= y2;
}

#if RETRO_INCLUDE_EDITOR
void PlayerHelpers_EditorDraw(void) {}

void PlayerHelpers_EditorLoad(void) {}
#endif

void PlayerHelpers_Serialize(void) {}
