// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZ1Setup *LRZ1Setup;

void LRZ1Setup_Update(void) {}

void LRZ1Setup_LateUpdate(void) {}

void LRZ1Setup_StaticUpdate(void)
{
    LRZ1Setup->palTimer += 24;
    if (LRZ1Setup->palTimer >= 256) {
        LRZ1Setup->palTimer -= 256;

        RSDK.RotatePalette(1, 208, 211, true);
        RSDK.RotatePalette(2, 208, 211, true);
        RSDK.RotatePalette(3, 208, 211, true);
        RSDK.RotatePalette(4, 208, 211, true);
    }

    if (LRZ1Setup->fadeTimer <= 0) {
        int32 blend = RSDK.Cos1024(2 * (Zone->timer & 0x1FF)) >> 3;
        RSDK.SetLimitedFade(5, 1, blend >= 0 ? 4 : 3, abs(blend), 208, 211);
        RSDK.RotatePalette(3, 208, 211, true);
        RSDK.RotatePalette(4, 208, 211, true);

        RSDK.SetLimitedFade(6, 2, blend >= 0 ? 4 : 3, abs(blend), 208, 211);
        RSDK.RotatePalette(3, 208, 211, false);
        RSDK.RotatePalette(4, 208, 211, false);

        RSDK.SetLimitedFade(0, 5, 6, LRZ1Setup->palTimer, 208, 211);
    }
    else {
        if (LRZ1Setup->fadeTimer == 1)
            RSDK.CopyPalette(0, 208, 1, 208, 5);

        RSDK.SetLimitedFade(0, 1, 7, LRZ1Setup->fadeTimer, 208, 212);

        if (LRZ1Setup->fadeTimer < 256)
            LRZ1Setup->fadeTimer += 2;
    }

    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer) >> 1) + 128, 140, 141);
    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(2 * Zone->timer + 128) >> 1) + 128, 142, 143);
    RSDK.SetLimitedFade(0, 1, 2, (RSDK.Sin256(Zone->timer) >> 1) + 128, 156, 158);

    foreach_active(Player, player)
    {
        if (player->onGround) {
            Hitbox *hitbox = Player_GetHitbox(player);

            int32 tileInfo  = 0;
            uint8 behaviour = LRZ1_TFLAGS_NORMAL;
            LRZ2Setup_GetTileInfo(player->position.x, (hitbox->bottom << 16) + player->position.y, player->moveLayerPosition.x,
                                  player->moveLayerPosition.y, player->collisionPlane, &tileInfo, &behaviour);

            if (behaviour == LRZ1_TFLAGS_LAVA && player->shield != SHIELD_FIRE && !LRZ1Setup->fadeTimer) {
                Player_HurtFlip(player);
            }
        }
    }

#if MANIA_USE_PLUS
    if (!(Zone->timer & 3)) {
        ++LRZ1Setup->background1->deformationOffset;
        ++LRZ1Setup->background2->deformationOffset;
    }

    if (!(Zone->timer & 1)) {
        ++LRZ1Setup->fgLow->deformationOffset;
        ++LRZ1Setup->fgHigh->deformationOffset;
    }
#endif
}

void LRZ1Setup_Draw(void) {}

void LRZ1Setup_Create(void *data) {}

void LRZ1Setup_StageLoad(void)
{
    LRZ1Setup->fadeTimer = 0;

    LRZ1Setup->background1 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 1"));
    LRZ1Setup->background2 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 2"));

#if MANIA_USE_PLUS
    LRZ1Setup->fgLow  = RSDK.GetTileLayer(Zone->fgLayer[0]);
    LRZ1Setup->fgHigh = RSDK.GetTileLayer(Zone->fgLayer[1]);
#endif

    // Slows the vScroll of the BG when the lava lake is visible to give a nicer effect :]
    RSDK.SetDrawGroupProperties(0, false, LRZ1Setup_DrawHook_ApplyBGSmoothing);

    Animals->animalTypes[0] = ANIMAL_FLICKY;
#if MANIA_USE_PLUS
    Animals->animalTypes[1] = ANIMAL_CUCKY;
#else
    Animals->animalTypes[1] = ANIMAL_RICKY;
#endif

    if (isMainGameMode() && CutsceneRules_IsAct1())
        Zone->stageFinishCallback = LRZ1Setup_StageFinish_EndAct1;

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreLRZ1.act", 0b0000000011111111);

        for (int32 i = 0; i < 0x400; ++i) LRZ1Setup->fgLow->deformationData[i] = LRZ1Setup->deformFG[i & 0x1F];
        for (int32 i = 0; i < 0x400; ++i) LRZ1Setup->fgHigh->deformationData[i] = LRZ1Setup->deformFG[i & 0x1F];
        for (int32 i = 0; i < 0x400; ++i) LRZ1Setup->background1->deformationData[i] = LRZ1Setup->deformBG[i & 0x1F];
        for (int32 i = 0; i < 0x400; ++i) LRZ1Setup->background2->deformationData[i] = LRZ1Setup->deformBG[i & 0x1F];
    }

    if (!isMainGameMode() || !globals->enableIntro || CutsceneRules_CheckStageReload()) {
        Zone->cameraBoundsL[0] = 2732;
        Zone->cameraBoundsL[1] = 2732;
        Zone->cameraBoundsL[2] = 2732;
        Zone->cameraBoundsL[3] = 2732;
        foreach_all(LRZ1Intro, intro) { destroyEntity(intro); }
    }
#else
    Zone->cameraBoundsL[0] = 648;
    Zone->cameraBoundsL[1] = 648;
    Zone->cameraBoundsL[2] = 648;
    Zone->cameraBoundsL[3] = 648;
#endif
}

void LRZ1Setup_StageFinish_EndAct1(void) { CREATE_ENTITY(LRZ1Outro, NULL, 0, 0); }

void LRZ1Setup_DrawHook_ApplyBGSmoothing(void)
{
    int32 scroll = MIN(0x800000 - 8 * ScreenInfo->position.y * ScreenInfo->position.y, 0);

    LRZ1Setup->background1->scrollPos = scroll;
    LRZ1Setup->background2->scrollPos = scroll;
}

#if GAME_INCLUDE_EDITOR
void LRZ1Setup_EditorDraw(void) {}

void LRZ1Setup_EditorLoad(void) {}
#endif

void LRZ1Setup_Serialize(void) {}
