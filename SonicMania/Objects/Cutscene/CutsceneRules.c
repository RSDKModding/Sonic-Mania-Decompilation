// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CutsceneRules Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCutsceneRules *CutsceneRules = NULL;

// NOTE:
// I'm not actually sure *what* this object was for
// CutsceneRules_SetupEntity was always inlined so I can't say for sure that it was a real func
// CutsceneRules_DrawCutsceneBounds is a func I added for editor drawing help
// I've never seen definitive proof of any funcs this object may have once had so be it what you will

void CutsceneRules_Update(void) {}

void CutsceneRules_LateUpdate(void) {}

void CutsceneRules_StaticUpdate(void) {}

void CutsceneRules_Draw(void) {}

void CutsceneRules_Create(void *data) {}

void CutsceneRules_StageLoad(void) {}

bool32 CutsceneRules_IsAct1(void)
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
bool32 CutsceneRules_IsAct2(void)
{
    if ((RSDK.CheckSceneFolder("GHZ") && Zone->actID == 1) || (RSDK.CheckSceneFolder("CPZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("SPZ2")
        || (RSDK.CheckSceneFolder("FBZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("PSZ2") || RSDK.CheckSceneFolder("SSZ2")
        || (RSDK.CheckSceneFolder("HCZ") && Zone->actID == 1) || (RSDK.CheckSceneFolder("MSZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("OOZ2")
        || RSDK.CheckSceneFolder("LRZ3") || (RSDK.CheckSceneFolder("MMZ") && Zone->actID == 1) || RSDK.CheckSceneFolder("TMZ2")) {
        return true;
    }

    return false;
}

bool32 CutsceneRules_IsIntroEnabled(void) { return isMainGameMode() && globals->enableIntro && !CutsceneRules_CheckStageReload(); }

bool32 CutsceneRules_IsAct1Regular(void) { return isMainGameMode() && CutsceneRules_IsAct1(); }

bool32 CutsceneRules_CheckStageReload(void)
{
    if (StarPost) {
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
bool32 CutsceneRules_CheckPlayerPos(int32 x1, int32 y1, int32 x2, int32 y2)
{
    Vector2 playerPos;

    if (Player) {
        foreach_all(Player, player)
        {
            if (CHECK_CHARACTER_ID(player->characterID, 1)) {
                playerPos = player->position;
            }
        }
    }

    return playerPos.x >= x1 && playerPos.y >= y1 && playerPos.x <= x2 && playerPos.y <= y2;
}

// Extra Helpers
void CutsceneRules_SetupEntity(void *e, Vector2 *size, Hitbox *hitbox)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;

    if (!size->x)
        size->x = WIDE_SCR_XSIZE << 16;

    if (!size->y)
        size->y = SCREEN_YSIZE << 16;

    entity->updateRange.x = TO_FIXED(128) + size->x;
    entity->updateRange.y = TO_FIXED(128) + size->y;

    hitbox->left   = -size->x >> 17;
    hitbox->top    = -size->y >> 17;
    hitbox->right  = size->x >> 17;
    hitbox->bottom = size->y >> 17;
}

void CutsceneRules_DrawCutsceneBounds(void *e, Vector2 *size)
{
    EntityCutsceneRules *entity = (EntityCutsceneRules *)e;
    DrawHelpers_DrawRectOutline(entity->position.x, entity->position.y, size->x, size->y, 0xFFFF00);
}

#if GAME_INCLUDE_EDITOR
void CutsceneRules_EditorDraw(void) {}

void CutsceneRules_EditorLoad(void) {}
#endif

void CutsceneRules_Serialize(void) {}
