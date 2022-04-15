// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFBZSetup *FBZSetup;

void FBZSetup_Update(void) {}

void FBZSetup_LateUpdate(void) {}

void FBZSetup_StaticUpdate(void)
{
    if (!(Zone->timer & 1)) {
        ++FBZSetup->frameA;
        FBZSetup->frameA &= 7;
        if (Zone->actID == 1)
            RSDK.DrawAniTiles(FBZSetup->aniTiles, 381, 0, 16 * FBZSetup->frameA, 256, 16);
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 401, 16 * (FBZSetup->frameA + 16), 0, 16, 126);
    }

    if (!(Zone->timer & 3)) {
        ++FBZSetup->frameB;
        FBZSetup->frameB &= 3;
        ++FBZSetup->frameC;
        FBZSetup->frameC &= 7;
        ++FBZSetup->frameD;
        FBZSetup->frameD %= 6;
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 377, 16 * (FBZSetup->frameB + 24), 16, 16, 32);
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 379, 16 * (FBZSetup->frameC + 24), 0, 16, 32);
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 397, 384, 16 * (FBZSetup->frameD + 2), 64, 16);
    }

    RSDK.GetSceneLayer(FBZSetup->bgOutsideLayer)->scrollPos = (RSDK.Sin512(Zone->timer2) + 512) << 10;
}

void FBZSetup_Draw(void) {}

void FBZSetup_Create(void *data) {}

void FBZSetup_StageLoad(void)
{
    FBZSetup->aniTiles                           = RSDK.LoadSpriteSheet("FBZ/AniTiles.gif", SCOPE_STAGE);

    FBZSetup->bgOutsideLayer                     = RSDK.GetSceneLayerID("Background Outside");

    Animals->animalTypes[0]                      = ANIMAL_FLICKY;
    Animals->animalTypes[1]                      = ANIMAL_RICKY;

    BGSwitch->switchCallback[FBZ_BG_INSIDE1]     = FBZSetup_BGSwitchCB_ShowInside1;
    BGSwitch->switchCallback[FBZ_BG_INSIDE2]     = FBZSetup_BGSwitchCB_ShowInside2;
    BGSwitch->switchCallback[FBZ_BG_INSIDE1_DUP] = FBZSetup_BGSwitchCB_ShowInside1_Dup;

    TileLayer *background1    = RSDK.GetSceneLayer(0);
    background1->drawLayer[0] = 0;
    background1->drawLayer[1] = 0;
    background1->drawLayer[2] = 0;
    background1->drawLayer[3] = 0;
    if (!Zone->actID)
        background1->scanlineCallback = FBZSetup_ScanlineCallback;

    int32 ang       = -336;
    int32 scanlineY = 0;
    for (int32 i = 0; i < 0x400; ++i) {
        int32 angle = ang >> 1;
        if (RSDK.Sin1024(angle) >= 0)
            FBZSetup->deformX[i] = 32 * (RSDK.Sin1024(angle) + 0x400);
        else
            FBZSetup->deformX[i] = 32 * (-RSDK.Sin1024(angle) + 0x400);

        if (RSDK.Cos1024(angle) >= 0)
            FBZSetup->positionYMove[i] = 32 * (RSDK.Cos1024(angle) + 0x800);
        else
            FBZSetup->positionYMove[i] = 32 * (-RSDK.Cos1024(angle) + 0x800);

        FBZSetup->positionY[i] = scanlineY & 0x1FF0000;
        ang += 3;
        scanlineY += FBZSetup->positionYMove[i];
    }

    // Yeah, this is actually here in the original
    // This (& the one in MSZSpotlight) is how I found out about what these funcs do LOL
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Show Exterior", FBZ_GENERICTRIGGER_EXTERIOR);
    RSDK_ENUM_VAR("Show Interior", FBZ_GENERICTRIGGER_INTERIOR);

    GenericTrigger->callbacks[FBZ_GENERICTRIGGER_EXTERIOR] = FBZSetup_GenericTriggerCB_ShowExterior;
    GenericTrigger->callbacks[FBZ_GENERICTRIGGER_INTERIOR] = FBZSetup_GenericTriggerCB_ShowInterior;

    if (globals->gameMode == MODE_COMPETITION) {
        foreach_all(ParallaxSprite, parallaxSprite) { destroyEntity(parallaxSprite); }
    }

    if (isMainGameMode() && PlayerHelpers_CheckAct1()) {
        foreach_all(FBZ1Outro, outro)
        {
            Zone->forcePlayerOnScreenFlag = true;
            FBZSetup->outroPtr            = outro;
            foreach_break;
        }
        Zone->stageFinishCallback = FBZSetup_StageFinishCB_Act1;
    }

    if (isMainGameMode() && globals->atlEnabled && !PlayerHelpers_CheckStageReload()) {
        FBZSetup_ActTransitionLoad();
    }

#if RETRO_USE_PLUS
    if (isMainGameMode() && PlayerHelpers_CheckAct2())
        Zone->stageFinishCallback = FBZSetup_StageFinishCB_Act2;

    if ((SceneInfo->filter & FILTER_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreFBZ.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 1, 1, 1, 0b0000000011111111);
        RSDK.LoadPalette(2, "EncoreFBZs.act", 0b0000000011111111);
        RSDK.LoadPalette(3, "EncoreFBZf.act", 0b0000000011111111);
        RSDK.LoadPalette(4, "EncoreFBZi.act", 0b0000000011111111);
    }
#endif
}

void FBZSetup_ActTransitionLoad(void)
{
    Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 4324 << 16, false);
    Zone->cameraBoundsL[0] = 0;
    Zone->cameraBoundsB[0] = 4324;
    Zone->cameraBoundsL[1] = 0;
    Zone->cameraBoundsB[1] = 4324;
    Zone->cameraBoundsL[2] = 0;
    Zone->cameraBoundsB[2] = 4324;
    Zone->cameraBoundsL[3] = 0;
    Zone->cameraBoundsB[3] = 4324;

    int32 id         = 0;
    TileLayer *layer = RSDK.GetSceneLayer(2);
    for (int32 i = 0; i < layer->scrollInfoCount; ++i) {
        layer->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->scrollPos.x = globals->parallaxOffset[id++]; }
}

void FBZSetup_HandleScanlines(ScanlineInfo *scanlines, int32 a1, int32 a3, int32 a4, int32 a5, int32 a6)
{
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int32 scroll          = (a3 * screen->position.y) >> 8;
    int32 start        = a4 - scroll;
    int32 end          = a4 - scroll + a5;
    int32 x            = (a1 * screen->position.x) << 8;
    if (start < SCREEN_YSIZE && end > 0) {
        int32 y = a6;
        if (start < 0) {
            y     = a6 - (start << 16);
            start = 0;
        }
        if (end > SCREEN_YSIZE)
            end = SCREEN_YSIZE;
        ScanlineInfo *scanlinePtr = &scanlines[start];

        for (int32 i = 0; i < end - start; ++i) {
            scanlinePtr->position.x = x;
            scanlinePtr->position.y = y;
            scanlinePtr->deform.x   = 0x10000;
            ++scanlinePtr;
            y += 0x10000;
        }
    }
}

void FBZSetup_ScanlineCallback(ScanlineInfo *scanlines)
{
    RSDKScreenInfo *screen        = &ScreenInfo[SceneInfo->currentScreenID];
    int32 y                   = screen->position.y >> 3;
    ScanlineInfo *scanlinePtr = scanlines;

    for (int32 i = 0; i < SCREEN_YSIZE; ++i) {
        int32 pos               = (i + y) & 0x3FF;
        scanlinePtr->deform.x   = FBZSetup->deformX[pos];
        scanlinePtr->deform.y   = 0;
        scanlinePtr->position.x = ((screen->position.x << 14) - FBZSetup->deformX[pos] * screen->centerX) & 0xFFFF8000;
        scanlinePtr->position.y = FBZSetup->positionY[pos];
        ++scanlinePtr;
    }

    FBZSetup_HandleScanlines(scanlines, 128, 32, 0, 112, 0x2400000);
    FBZSetup_HandleScanlines(scanlines, 128, 32, 456, 128, 0x2000000);
    FBZSetup_HandleScanlines(scanlines, 112, 64, 420, 16, 0x3300000);
    FBZSetup_HandleScanlines(scanlines, 144, 80, 640, 48, 0x3000000);
    FBZSetup_HandleScanlines(scanlines, 160, 96, 512, 64, 0x2C00000);
}

void FBZSetup_BGSwitchCB_ShowInside1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    if (Zone->actID == 1) {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = 0;
    }
    else {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    }
}

void FBZSetup_BGSwitchCB_ShowInside2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    if (Zone->actID == 1) {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
        RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    }
    else {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    }
}

void FBZSetup_BGSwitchCB_ShowInside1_Dup(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    if (Zone->actID == 1) {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
    }
    else {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    }
}

void FBZSetup_GenericTriggerCB_ShowExterior(void)
{
    int32 id                                                        = (2 * (Zone->actID != 0) + 3);
    RSDK.GetSceneLayer(id)->drawLayer[GenericTrigger->playerID]     = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(id + 1)->drawLayer[GenericTrigger->playerID] = 6;

    foreach_active(ParallaxSprite, parallaxSprite)
    {
        if (!parallaxSprite->visible) {
            parallaxSprite->visible = true;
            parallaxSprite->state   = ParallaxSprite_State_FadeIntoHalf;
        }
    }
}

void FBZSetup_GenericTriggerCB_ShowInterior(void)
{
    int32 id                                                        = (2 * (Zone->actID != 0) + 3);
    RSDK.GetSceneLayer(id)->drawLayer[GenericTrigger->playerID]     = 6;
    RSDK.GetSceneLayer(id + 1)->drawLayer[GenericTrigger->playerID] = DRAWLAYER_COUNT;

    foreach_active(ParallaxSprite, parallaxSprite) { parallaxSprite->state = ParallaxSprite_State_FadeOut; }
}

void FBZSetup_StageFinishCB_Act1(void) { FBZSetup->outroPtr->active = ACTIVE_NORMAL; }
#if RETRO_USE_PLUS
void FBZSetup_StageFinishCB_Act2(void) { CREATE_ENTITY(FBZ2Outro, NULL, 0, 0); }
#endif

#if RETRO_INCLUDE_EDITOR
void FBZSetup_EditorDraw(void) {}

void FBZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Show Exterior", FBZ_GENERICTRIGGER_EXTERIOR);
    RSDK_ENUM_VAR("Show Interior", FBZ_GENERICTRIGGER_INTERIOR);

    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Show Inside 1", FBZ_BG_INSIDE1);
    RSDK_ENUM_VAR("Show Inside 2", FBZ_BG_INSIDE2);
    RSDK_ENUM_VAR("Show Inside 1 (Duplicate)", FBZ_BG_INSIDE1_DUP);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Blueprint 1", FBZ_DECORATION_BLUEPRINT1);
    RSDK_ENUM_VAR("Blueprint 2", FBZ_DECORATION_BLUEPRINT2);
}
#endif

void FBZSetup_Serialize(void) {}
