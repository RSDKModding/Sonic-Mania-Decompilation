// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SPZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSPZ1Setup *SPZ1Setup;

void SPZ1Setup_Update(void) {}

void SPZ1Setup_LateUpdate(void) {}

void SPZ1Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        ++SPZ1Setup->cityBGLow->deformationOffset;
        ++SPZ1Setup->cityBGHigh->deformationOffset;
    }

    SPZ1Setup->conveyorFrame += 16;
    SPZ1Setup->conveyorFrame &= 0x3F;
    RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 613, 144, SPZ1Setup->conveyorFrame + 160, 112, 16);

    SPZ1Setup->fgLightsPalTimer += 6;
    if (SPZ1Setup->fgLightsPalTimer >= 0x100) {
        SPZ1Setup->fgLightsPalTimer -= 0x100;
        RSDK.RotatePalette(1, 128, 135, false);
        RSDK.RotatePalette(2, 128, 135, false);
    }
    RSDK.SetLimitedFade(0, 1, 2, SPZ1Setup->fgLightsPalTimer, 128, 135);

    ++SPZ1Setup->bgLightsPalTimer;
    if (SPZ1Setup->bgLightsPalTimer == 6) {
        SPZ1Setup->bgLightsPalTimer = 0;
        RSDK.RotatePalette(0, 240, 243, true);
    }

    SPZ1Setup->pulsePercent += 3;
    SPZ1Setup->pulsePercent &= 0x1FF;
    RSDK.SetLimitedFade(0, 1, 3, abs(RSDK.Sin512(SPZ1Setup->pulsePercent) >> 1), 152, 159);

    ++SPZ1Setup->streetLightTimer;
    if (SPZ1Setup->streetLightTimer == 3) {
        SPZ1Setup->streetLightTimer = 0;
        SPZ1Setup->streetLightFrame += 9;
        SPZ1Setup->streetLightFrame &= 0xF;
        RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 221, 8 * (SPZ1Setup->streetLightFrame & 0xFFFC), 32 * ((SPZ1Setup->streetLightFrame & 3) + 4), 32, 32);

        SPZ1Setup->streetLightFrame += 8;
        SPZ1Setup->streetLightFrame &= 0xF;
        RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 225, 8 * (SPZ1Setup->streetLightFrame & 0xFFFC), 32 * ((SPZ1Setup->streetLightFrame & 3) + 4), 32, 32);
    }

    if (!(Zone->timer & 0xF)) {
        SPZ1Setup->flashingLightsFrame += 0x10;
        SPZ1Setup->flashingLightsFrame &= 0x1F;
        RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 64, 128, SPZ1Setup->flashingLightsFrame + 128, 128, 16);
    }
}

void SPZ1Setup_Draw(void) {}

void SPZ1Setup_Create(void *data) {}

void SPZ1Setup_StageLoad(void)
{
    SPZ1Setup->aniTiles = RSDK.LoadSpriteSheet("SPZ1/AniTiles.gif", SCOPE_STAGE);

    SPZ1Setup->cityBGLow  = RSDK.GetTileLayer(0); // Background 1
    SPZ1Setup->cityBGHigh = RSDK.GetTileLayer(1); // Background 2

    int32 pos = 0;
    for (int32 i = 0; i < 0x200; ++i) {
        int32 off    = CLAMP(pos, 0, 0x200);
        int32 deform = RSDK.Rand(0, 4);

        int32 angle = 0;
        for (int32 d = 0; d < 0x10; ++d) {
            SPZ1Setup->cityBGLow->deformationData[off + d]  = deform * RSDK.Sin1024(angle) >> 10;
            SPZ1Setup->cityBGHigh->deformationData[off + d] = deform * RSDK.Sin1024(angle) >> 10;
            angle += 0x40;
        }
        pos += 0x10;
    }

    memcpy(&SPZ1Setup->cityBGLow->deformationData[0x200], &SPZ1Setup->cityBGLow->deformationData[0], 0x200 * sizeof(int32));
    memcpy(&SPZ1Setup->cityBGHigh->deformationData[0x200], &SPZ1Setup->cityBGHigh->deformationData[0], 0x200 * sizeof(int32));

    Animals->animalTypes[0] = ANIMAL_PECKY;
    Animals->animalTypes[1] = ANIMAL_MICKY;

    if (isMainGameMode() || !globals->enableIntro || CutsceneRules_CheckStageReload())
        FXFade_StopAll();

    if (isMainGameMode() && CutsceneRules_IsAct1()) {
        Zone->shouldRecoverPlayers = true;
        Zone->stageFinishCallback  = SPZ1Setup_StageFinish_EndAct1;
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreSPZ1.act", 0b0000000011111111);
        RSDK.LoadPalette(1, "EncoreSPZ1b1.act", 0b0000000011111111);
        RSDK.LoadPalette(2, "EncoreSPZ1b2.act", 0b0000000011111111);
        RSDK.LoadPalette(3, "EncoreSPZ1b3.act", 0b0000000011111111);
    }
#endif
}

void SPZ1Setup_StageFinish_EndAct1(void)
{
    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->center.x) << 16, Zone->cameraBoundsB[0] << 16);
    RSDK.LoadScene();
}

#if GAME_INCLUDE_EDITOR
void SPZ1Setup_EditorDraw(void) {}

void SPZ1Setup_EditorLoad(void) {}
#endif

void SPZ1Setup_Serialize(void) {}
