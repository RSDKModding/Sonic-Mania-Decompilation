#include "SonicMania.h"

ObjectSPZ2Setup *SPZ2Setup;

void SPZ2Setup_Update(void)
{
    RSDK_THIS(SPZ2Setup);
    self->palTimer += 4;
    self->timer -= 16;
    RSDK.SetLimitedFade(0, 4, 5, self->palTimer, 128, 151);
    RSDK.SetLimitedFade(0, 4, 5, self->palTimer, 160, 175);
    if (self->palTimer == 256)
        RSDK.ResetEntityPtr(self, 0, 0);
}

void SPZ2Setup_LateUpdate(void) {}

void SPZ2Setup_StaticUpdate(void)
{
    ++SPZ2Setup->palTimer;
    if (SPZ2Setup->palTimer > 2) {
        SPZ2Setup->palTimer = 0;
        RSDK.RotatePalette(0, 152, 159, true);
    }

    if (!(Zone->timer & 0xF)) {
        SPZ2Setup->aniTilesFrameA += 16;
        SPZ2Setup->aniTilesFrameA &= 0x1F;
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 1021, SPZ2Setup->aniTilesFrameA + 96, 64, 16, 32);
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 192, SPZ2Setup->aniTilesFrameA + 96, 96, 16, 32);
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 194, SPZ2Setup->aniTilesFrameA + 64, 112, 16, 16);
    }

    if (!(Zone->timer & 1)) {
        ++SPZ2Setup->aniTilesFrameB;
        SPZ2Setup->aniTilesFrameB &= 0xF;
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 975, SPZ2Setup->aniTileSrcXA[SPZ2Setup->aniTilesFrameB], 112, 16, 16);
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 976, SPZ2Setup->aniTileSrcXB[SPZ2Setup->aniTilesFrameB],
                          SPZ2Setup->aniTileSrcYB[SPZ2Setup->aniTilesFrameB], 16, 32);
    }

    SPZ2Setup->angle += 3;
    SPZ2Setup->angle &= 0x1FF;
    RSDK.SetLimitedFade(0, 1, 3, abs(RSDK.Sin512(SPZ2Setup->angle) >> 1), 144, 151);
    RSDK.SetLimitedFade(0, 1, 2, RSDK.Sin512(4 * Zone->timer), 192, 197);
    RSDK.DrawAniTiles(SPZ2Setup->aniTiles, 360, 16 * (Zone->timer & 7), 0, 16, 128);

    --SPZ2Setup->timerC;
    if (SPZ2Setup->timerC < 1) {
        ++SPZ2Setup->aniTilesFrameC;
        SPZ2Setup->aniTilesFrameC &= 7;
        SPZ2Setup->timerC = SPZ2Setup->aniTileDelaysC[SPZ2Setup->aniTilesFrameC];
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles, 113, 16 * (SPZ2Setup->aniTilesFrameC + 8), 0, 16, 128);
    }

    --SPZ2Setup->delayD;
    if (SPZ2Setup->delayD < 1) {
        ++SPZ2Setup->frameD;
        SPZ2Setup->frameD %= 12;
        SPZ2Setup->delayD = SPZ2Setup->aniTilesDelaysD[SPZ2Setup->frameD];
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 836, 0, 16 * SPZ2Setup->aniTilesSrcYD[SPZ2Setup->frameD], 96, 16);
    }

    ++SPZ2Setup->fgLow->deformationOffset;
    ++SPZ2Setup->fgHigh->deformationOffset;
}

void SPZ2Setup_Draw(void)
{
    RSDK_THIS(SPZ2Setup);
    RSDK.FillScreen(0xF0F0F0, self->timer, self->timer - 128, self->timer - 256);
}

void SPZ2Setup_Create(void *data)
{
    RSDK_THIS(SPZ2Setup);
    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->timer     = 384;
    self->drawOrder = Zone->hudDrawOrder + 1;
}

void SPZ2Setup_StageLoad(void)
{
    SPZ2Setup->aniTiles  = RSDK.LoadSpriteSheet("SPZ2/AniTiles1.gif", SCOPE_STAGE);
    SPZ2Setup->aniTiles2 = RSDK.LoadSpriteSheet("SPZ2/AniTiles2.gif", SCOPE_STAGE);
    SPZ2Setup->fgLow     = RSDK.GetSceneLayer(Zone->fgLow);
    SPZ2Setup->fgHigh    = RSDK.GetSceneLayer(Zone->fgHigh);

    for (int32 i = 0; i < 0x400; ++i) {
        SPZ2Setup->fgLow->deformationData[i]  = SPZ2Setup->layerDeform[i & 0x1F];
        SPZ2Setup->fgHigh->deformationData[i] = SPZ2Setup->layerDeform[i & 0x1F];
    }

    Animals->animalTypes[0] = ANIMAL_PECKY;
    Animals->animalTypes[1] = ANIMAL_MICKY;

    if (isMainGameMode() && globals->atlEnabled) {
        if (!PlayerHelpers_CheckStageReload()) {
            Zone->screenBoundsL1[0] = 256 - ScreenInfo->centerX;
            Zone_ReloadStoredEntities(0x5600000, 0x1000000, true);
            RSDK.CreateEntity(SPZ2Setup->objectID, 0, 0, 0);
        }
    }

    if (isMainGameMode() && PlayerHelpers_CheckAct2()) {
        foreach_all(SPZ2Outro, entity) { foreach_break; }
        SPZ2Setup->outroPtr       = (Entity*)entity;
        Zone->stageFinishCallback = SPZ2Setup_SetupOutro;
    }

#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(4, "EncoreSPZ1.act", 255);
        RSDK.LoadPalette(0, "EncoreSPZ2.act", 255);
        RSDK.CopyPalette(0, 128, 5, 128, 128);
    }
#endif

    SPZ2Setup->fgLow->scrollInfo[0].deform  = false;
    SPZ2Setup->fgHigh->scrollInfo[0].deform = false;
}

void SPZ2Setup_SetupOutro(void) { SPZ2Setup->outroPtr->active = ACTIVE_NORMAL; }

void SPZ2Setup_EditorDraw(void) {}

void SPZ2Setup_EditorLoad(void) {}

void SPZ2Setup_Serialize(void) {}
