#include "../SonicMania.h"

ObjectSPZ1Setup *SPZ1Setup;

void SPZ1Setup_Update(void)
{

}

void SPZ1Setup_LateUpdate(void)
{

}

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
    if (SPZ1Setup->palRotateTimerA > 255) {
        SPZ1Setup->palRotateTimerA -= 256;
        RSDK.RotatePalette(1, 128, 135, 0);
        RSDK.RotatePalette(2, 128, 135, 0);
    }
    RSDK.SetLimitedFade(0, 1, 2, SPZ1Setup->palRotateTimerA, 128, 135);
    ++SPZ1Setup->palRotateTimerB;

    if (SPZ1Setup->palRotateTimerB == 6) {
        SPZ1Setup->palRotateTimerB = 0;
        RSDK.RotatePalette(0, 240, 243, true);
    }

    SPZ1Setup->angle += 3;
    SPZ1Setup->angle &= 0x1FF;
    RSDK.SetLimitedFade(0, 1, 3, abs(RSDK.Sin512(SPZ1Setup->angle) >> 1), 152, 159);
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

void SPZ1Setup_Draw(void)
{

}

void SPZ1Setup_Create(void* data)
{

}

void SPZ1Setup_StageLoad(void)
{
    SPZ1Setup->aniTiles = RSDK.LoadSpriteSheet("SPZ1/AniTiles.gif", SCOPE_STAGE);
    SPZ1Setup->bgLayer      = RSDK.GetSceneLayer(0);
    SPZ1Setup->bgLayer2      = RSDK.GetSceneLayer(0);

    int id = 0;
    for (int i = 0; i < 0x200; ++i) {
        int off = 0;
        int val = RSDK.Rand(0, 4);
        if (id >= 0) {
            off = id;
            if (id > 512)
                off = 512;
        }
        else {
            off = 0;
        }

        int ang = 0;
        int *deform = &SPZ1Setup->bgLayer->deformationData[off];
        for (int d = 0; d < 0x10; ++d) {
            *deform = val * RSDK.Sin1024(ang) >> 10;
            *deform = val * RSDK.Sin1024(ang) >> 10;
            ang += 64;
        }
        memcpy(SPZ1Setup->bgLayer->deformationData + 0x200, SPZ1Setup->bgLayer->deformationData, 0x200 * sizeof(int));
        id += 16;
    }

    id = 0;
    for (int i = 0; i < 0x200; ++i) {
        int off = 0;
        int val = RSDK.Rand(0, 4);
        if (id >= 0) {
            off = id;
            if (id > 512)
                off = 512;
        }
        else {
            off = 0;
        }

        int ang     = 0;
        int *deform = &SPZ1Setup->bgLayer2->deformationData[off];
        for (int d = 0; d < 0x10; ++d) {
            *deform = val * RSDK.Sin1024(ang) >> 10;
            *deform = val * RSDK.Sin1024(ang) >> 10;
            ang += 64;
        }
        memcpy(SPZ1Setup->bgLayer2->deformationData + 0x200, SPZ1Setup->bgLayer2->deformationData, 0x200 * sizeof(int));
        id += 16;
    }
    
    Animals->animalTypes[0] = ANIMAL_PECKY;
    Animals->animalTypes[1] = ANIMAL_MICKY;
#if RETRO_USE_PLUS
    if ((globals->gameMode && globals->gameMode != MODE_ENCORE) || !globals->enableIntro || PlayerHelpers_CheckStageReload())
#else
    if ((globals->gameMode && globals->gameMode != MODE_MANIA) || !globals->enableIntro || PlayerHelpers_CheckStageReload())
#endif
        FXFade_StopAll();

#if RETRO_USE_PLUS
    if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && PlayerHelpers_CheckAct1()) {
#else
    if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_NOSAVE) && PlayerHelpers_CheckAct1()) {
#endif
        Zone->forcePlayerOnScreenFlag = true;
        Zone->stageFinishCallback     = SPZ1Setup_SetupActTransition;
    }

#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreSPZ1.act", 255);
        RSDK.LoadPalette(1, "EncoreSPZ1b1.act", 255);
        RSDK.LoadPalette(2, "EncoreSPZ1b2.act", 255);
        RSDK.LoadPalette(3, "EncoreSPZ1b3.act", 255);
    }
#endif
}

void SPZ1Setup_SetupActTransition(void)
{
    Zone_StoreEntities((Zone->screenBoundsL1[0] + RSDK_screens->centerX) << 16, Zone->screenBoundsB1[0] << 16);
    RSDK.InitSceneLoad();
}

void SPZ1Setup_EditorDraw(void)
{

}

void SPZ1Setup_EditorLoad(void)
{

}

void SPZ1Setup_Serialize(void)
{

}

