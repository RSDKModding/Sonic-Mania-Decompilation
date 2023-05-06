// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SPZ2Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSPZ2Setup *SPZ2Setup;

void SPZ2Setup_Update(void)
{
    RSDK_THIS(SPZ2Setup);

    self->palBlendPercent += 4;
    self->fadeTimer -= 16;

    RSDK.SetLimitedFade(0, 4, 5, self->palBlendPercent, 128, 151);
    RSDK.SetLimitedFade(0, 4, 5, self->palBlendPercent, 160, 175);

    if (self->palBlendPercent == 0x100)
        destroyEntity(self);
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
        SPZ2Setup->musicSndDisplayFrameV += 0x10;
        SPZ2Setup->musicSndDisplayFrameV &= 0x1F;
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 1021, SPZ2Setup->musicSndDisplayFrameV + 96, 64, 16, 32);
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 192, SPZ2Setup->musicSndDisplayFrameV + 96, 96, 16, 32);
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 194, SPZ2Setup->musicSndDisplayFrameV + 64, 112, 16, 16);
    }

    if (!(Zone->timer & 1)) {
        ++SPZ2Setup->musicSndDisplayFrameH;
        SPZ2Setup->musicSndDisplayFrameH &= 0xF;
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 975, SPZ2Setup->musicSndDisplayHFramePos[SPZ2Setup->musicSndDisplayFrameH], 112, 16, 16);
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 976, SPZ2Setup->bgLightsFramePosX[SPZ2Setup->musicSndDisplayFrameH],
                          SPZ2Setup->bgLightsFramePosY[SPZ2Setup->musicSndDisplayFrameH], 16, 32);
    }

    SPZ2Setup->pulsePercent += 3;
    SPZ2Setup->pulsePercent &= 0x1FF;
    RSDK.SetLimitedFade(0, 1, 3, abs(RSDK.Sin512(SPZ2Setup->pulsePercent) >> 1), 144, 151);

    RSDK.SetLimitedFade(0, 1, 2, RSDK.Sin512(4 * Zone->timer), 192, 197);

    // Rotating thingy on the left of the AniTiles1 sheet
    RSDK.DrawAniTiles(SPZ2Setup->aniTiles1, 360, 16 * (Zone->timer & 7), 0, 16, 128);

    if (--SPZ2Setup->speakerTimer < 1) {
        ++SPZ2Setup->speakerFrame;
        SPZ2Setup->speakerFrame &= 7;
        SPZ2Setup->speakerTimer = SPZ2Setup->speakerDelays[SPZ2Setup->speakerFrame];
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles1, 113, 16 * (SPZ2Setup->speakerFrame + 8), 0, 16, 128);
    }

    if (--SPZ2Setup->stageLightsTimer < 1) {
        ++SPZ2Setup->stageLightsFrame;
        SPZ2Setup->stageLightsFrame %= 12;
        SPZ2Setup->stageLightsTimer = SPZ2Setup->stageLightsDelays[SPZ2Setup->stageLightsFrame];
        RSDK.DrawAniTiles(SPZ2Setup->aniTiles2, 836, 0, 16 * SPZ2Setup->stageLightsPos[SPZ2Setup->stageLightsFrame], 96, 16);
    }

    ++SPZ2Setup->fgLow->deformationOffset;
    ++SPZ2Setup->fgHigh->deformationOffset;
}

void SPZ2Setup_Draw(void)
{
    RSDK_THIS(SPZ2Setup);

    RSDK.FillScreen(0xF0F0F0, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
}

void SPZ2Setup_Create(void *data)
{
    RSDK_THIS(SPZ2Setup);

    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->fadeTimer = 384;
    self->drawGroup = Zone->hudDrawGroup + 1;
}

void SPZ2Setup_StageLoad(void)
{
    SPZ2Setup->aniTiles1 = RSDK.LoadSpriteSheet("SPZ2/AniTiles1.gif", SCOPE_STAGE);
    SPZ2Setup->aniTiles2 = RSDK.LoadSpriteSheet("SPZ2/AniTiles2.gif", SCOPE_STAGE);

    SPZ2Setup->fgLow  = RSDK.GetTileLayer(Zone->fgLayer[0]);
    SPZ2Setup->fgHigh = RSDK.GetTileLayer(Zone->fgLayer[1]);

    // Sun Attack Deform
    for (int32 i = 0; i < 0x400; ++i) {
        SPZ2Setup->fgLow->deformationData[i]  = SPZ2Setup->fgSunAtkDeform[i & 0x1F];
        SPZ2Setup->fgHigh->deformationData[i] = SPZ2Setup->fgSunAtkDeform[i & 0x1F];
    }

    Animals->animalTypes[0] = ANIMAL_PECKY;
    Animals->animalTypes[1] = ANIMAL_MICKY;

    if (isMainGameMode() && globals->atlEnabled) {
        if (!CutsceneRules_CheckStageReload()) {
            Zone->cameraBoundsL[0] = 0x100 - ScreenInfo->center.x;
            Zone_ReloadStoredEntities(256 << 16, 1376 << 16, true);

            CREATE_ENTITY(SPZ2Setup, NULL, 0, 0);
        }
    }

    if (isMainGameMode() && CutsceneRules_IsAct2()) {
        foreach_all(SPZ2Outro, entity) { foreach_break; }
        SPZ2Setup->outro = entity;

        Zone->stageFinishCallback = SPZ2Setup_StageFinish_EndAct2;
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(4, "EncoreSPZ1.act", 0b0000000011111111);
        RSDK.LoadPalette(0, "EncoreSPZ2.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 5, 128, 128);
    }
#endif

    SPZ2Setup->fgLow->scrollInfo[0].deform  = false;
    SPZ2Setup->fgHigh->scrollInfo[0].deform = false;
}

void SPZ2Setup_StageFinish_EndAct2(void) { SPZ2Setup->outro->active = ACTIVE_NORMAL; }

#if GAME_INCLUDE_EDITOR
void SPZ2Setup_EditorDraw(void) {}

void SPZ2Setup_EditorLoad(void) {}
#endif

void SPZ2Setup_Serialize(void) {}
