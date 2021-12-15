// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SPZ1Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSPZ1Setup *SPZ1Setup;

void SPZ1Setup_Update(void) {}

void SPZ1Setup_LateUpdate(void) {}

void SPZ1Setup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        ++SPZ1Setup->bgLayer->deformationOffset;
        ++SPZ1Setup->bgLayer2->deformationOffset;
    }

    SPZ1Setup->timerA += 16;
    SPZ1Setup->timerA &= 0x3F;
    RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 613, 144, SPZ1Setup->timerA + 160, 112, 16);

    SPZ1Setup->palRotateTimerA += 6;
    if (SPZ1Setup->palRotateTimerA >= 0x100) {
        SPZ1Setup->palRotateTimerA -= 0x100;
        RSDK.RotatePalette(1, 128, 135, false);
        RSDK.RotatePalette(2, 128, 135, false);
    }
    RSDK.SetLimitedFade(0, 1, 2, SPZ1Setup->palRotateTimerA, 128, 135);

    ++SPZ1Setup->palRotateTimerB;
    if (SPZ1Setup->palRotateTimerB == 6) {
        SPZ1Setup->palRotateTimerB = 0;
        RSDK.RotatePalette(0, 240, 243, true);
    }

    SPZ1Setup->angle += 3;
    SPZ1Setup->angle &= 0x1FF;
    int32 percent = RSDK.Sin512(SPZ1Setup->angle) >> 1;
    if (percent <= 0)
        RSDK.SetLimitedFade(0, 1, 2, -percent, 152, 159);
    else
        RSDK.SetLimitedFade(0, 1, 3, percent, 152, 159);
    ++SPZ1Setup->timerB;

    if (SPZ1Setup->timerB == 3) {
        SPZ1Setup->timerB = 0;
        SPZ1Setup->aniTilesFrameB += 9;
        SPZ1Setup->aniTilesFrameB &= 0xF;
        RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 221, 8 * (SPZ1Setup->aniTilesFrameB & 0xFFFC), 32 * ((SPZ1Setup->aniTilesFrameB & 3) + 4), 32, 32);
        SPZ1Setup->aniTilesFrameB += 8;
        SPZ1Setup->aniTilesFrameB &= 0xF;
        RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 225, 8 * (SPZ1Setup->aniTilesFrameB & 0xFFFC), 32 * ((SPZ1Setup->aniTilesFrameB & 3) + 4), 32, 32);
    }

    if (!(Zone->timer & 0xF)) {
        SPZ1Setup->aniTilesFrameC += 16;
        SPZ1Setup->aniTilesFrameC &= 0x1F;
        RSDK.DrawAniTiles(SPZ1Setup->aniTiles, 64, 128, SPZ1Setup->aniTilesFrameC + 128, 128, 16);
    }
}

void SPZ1Setup_Draw(void) {}

void SPZ1Setup_Create(void *data) {}

void SPZ1Setup_StageLoad(void)
{
    SPZ1Setup->aniTiles = RSDK.LoadSpriteSheet("SPZ1/AniTiles.gif", SCOPE_STAGE);
    SPZ1Setup->bgLayer  = RSDK.GetSceneLayer(0);
    SPZ1Setup->bgLayer2 = RSDK.GetSceneLayer(0);

    int32 id = 0;
    for (int32 i = 0; i < 0x200; ++i) {
        int32 off = 0;
        int32 val = RSDK.Rand(0, 4);
        if (id >= 0) {
            off = id;
            if (id > 512)
                off = 512;
        }
        else {
            off = 0;
        }

        int32 ang     = 0;
        int32 *deform = &SPZ1Setup->bgLayer->deformationData[off];
        for (int32 d = 0; d < 0x10; ++d) {
            *deform = val * RSDK.Sin1024(ang) >> 10;
            *deform = val * RSDK.Sin1024(ang) >> 10;
            ang += 64;
        }
        memcpy(SPZ1Setup->bgLayer->deformationData + 0x200, SPZ1Setup->bgLayer->deformationData, 0x200 * sizeof(int32));
        id += 16;
    }

    id = 0;
    for (int32 i = 0; i < 0x200; ++i) {
        int32 off = 0;
        int32 val = RSDK.Rand(0, 4);
        if (id >= 0) {
            off = id;
            if (id > 512)
                off = 512;
        }
        else {
            off = 0;
        }

        int32 ang     = 0;
        int32 *deform = &SPZ1Setup->bgLayer2->deformationData[off];
        for (int32 d = 0; d < 0x10; ++d) {
            *deform = val * RSDK.Sin1024(ang) >> 10;
            *deform = val * RSDK.Sin1024(ang) >> 10;
            ang += 64;
        }
        memcpy(SPZ1Setup->bgLayer2->deformationData + 0x200, SPZ1Setup->bgLayer2->deformationData, 0x200 * sizeof(int32));
        id += 16;
    }

    Animals->animalTypes[0] = ANIMAL_PECKY;
    Animals->animalTypes[1] = ANIMAL_MICKY;
    if (isMainGameMode() || !globals->enableIntro || PlayerHelpers_CheckStageReload())
        FXFade_StopAll();

    if (isMainGameMode() && PlayerHelpers_CheckAct1()) {
        Zone->forcePlayerOnScreenFlag = true;
        Zone->stageFinishCallback     = SPZ1Setup_SetupActTransition;
    }

#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreSPZ1.act", 0b0000000011111111);
        RSDK.LoadPalette(1, "EncoreSPZ1b1.act", 0b0000000011111111);
        RSDK.LoadPalette(2, "EncoreSPZ1b2.act", 0b0000000011111111);
        RSDK.LoadPalette(3, "EncoreSPZ1b3.act", 0b0000000011111111);
    }
#endif
}

void SPZ1Setup_SetupActTransition(void)
{
    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->centerX) << 16, Zone->cameraBoundsB[0] << 16);
    RSDK.LoadScene();
}

void SPZ1Setup_EditorDraw(void) {}

void SPZ1Setup_EditorLoad(void) {}

void SPZ1Setup_Serialize(void) {}
