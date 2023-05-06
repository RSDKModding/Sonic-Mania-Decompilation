// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFBZSetup *FBZSetup;

void FBZSetup_Update(void) {}

void FBZSetup_LateUpdate(void) {}

void FBZSetup_StaticUpdate(void)
{
    if (!(Zone->timer & 1)) {
        ++FBZSetup->cylinderAniFrame;
        FBZSetup->cylinderAniFrame &= 7;

        if (Zone->actID == 1) // Cylinder H (left cylinder on the sheet)
            RSDK.DrawAniTiles(FBZSetup->aniTiles, 381, 0, 16 * FBZSetup->cylinderAniFrame, 256, 16);

        // Cylinder V (right cylinder on the sheet)
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 401, 16 * (FBZSetup->cylinderAniFrame + 16), 0, 16, 126);
    }

    if (!(Zone->timer & 3)) {
        ++FBZSetup->propellerShaftAniFrameUnused;
        FBZSetup->propellerShaftAniFrameUnused &= 3;

        ++FBZSetup->propellerShaftAniFrame;
        FBZSetup->propellerShaftAniFrame &= 7;

        ++FBZSetup->spiralPlatformAniFrame;
        FBZSetup->spiralPlatformAniFrame %= 6;

        // Propeller Shaft AniTiles (unused?, these don't match propellers, just some clouds)
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 377, 16 * (FBZSetup->propellerShaftAniFrameUnused + 24), 16, 16, 32);

        // Propeller Shaft AniTiles
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 379, 16 * (FBZSetup->propellerShaftAniFrame + 24), 0, 16, 32);

        // Spiral Platform AniTiles (unused in-game)
        RSDK.DrawAniTiles(FBZSetup->aniTiles, 397, 384, 16 * (FBZSetup->spiralPlatformAniFrame + 2), 64, 16);
    }

    RSDK.GetTileLayer(FBZSetup->backgroundOutside)->scrollPos = (RSDK.Sin512(Zone->persistentTimer) + 512) << 10;
}

void FBZSetup_Draw(void) {}

void FBZSetup_Create(void *data) {}

void FBZSetup_StageLoad(void)
{
    FBZSetup->aniTiles = RSDK.LoadSpriteSheet("FBZ/AniTiles.gif", SCOPE_STAGE);

    FBZSetup->backgroundOutside = RSDK.GetTileLayerID("Background Outside");

    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_RICKY;

    BGSwitch->switchCallback[FBZ_BG_INSIDE1]     = FBZSetup_BGSwitch_ShowInside1;
    BGSwitch->switchCallback[FBZ_BG_INSIDE2]     = FBZSetup_BGSwitch_ShowInside2;
    BGSwitch->switchCallback[FBZ_BG_INSIDE1_NOSTORM] = FBZSetup_BGSwitch_ShowInside1_NoStorm;

    TileLayer *backgroundInside    = RSDK.GetTileLayer(0);
    backgroundInside->drawGroup[0] = 0;
    backgroundInside->drawGroup[1] = 0;
    backgroundInside->drawGroup[2] = 0;
    backgroundInside->drawGroup[3] = 0;
    if (!Zone->actID)
        backgroundInside->scanlineCallback = FBZSetup_Scanline_BGInside;

    int32 ang       = -336;
    int32 scanlineY = 0;
    for (int32 i = 0; i < 0x400; ++i) {
        FBZSetup->scanlineDeformX[i] = 32 * (abs(RSDK.Sin1024(ang >> 1)) + 0x400);
        FBZSetup->positionYMove[i]   = 32 * (abs(RSDK.Cos1024(ang >> 1)) + 0x800);

        FBZSetup->scanlinePosY[i] = scanlineY & 0x1FF0000;
        ang += 3;
        scanlineY += FBZSetup->positionYMove[i];
    }

    // Yeah, this is actually here in the original
    // This (& the one in MSZSpotlight) is how I found out about what these funcs do LOL
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Show Exterior", FBZ_GENERICTRIGGER_EXTERIOR);
    RSDK_ENUM_VAR("Show Interior", FBZ_GENERICTRIGGER_INTERIOR);

    GenericTrigger->callbacks[FBZ_GENERICTRIGGER_EXTERIOR] = FBZSetup_Trigger_ShowExterior;
    GenericTrigger->callbacks[FBZ_GENERICTRIGGER_INTERIOR] = FBZSetup_Trigger_ShowInterior;

    if (globals->gameMode == MODE_COMPETITION) {
        foreach_all(ParallaxSprite, parallaxSprite) { destroyEntity(parallaxSprite); }
    }

    if (isMainGameMode() && CutsceneRules_IsAct1()) {
        foreach_all(FBZ1Outro, outro)
        {
            Zone->shouldRecoverPlayers = true;
            FBZSetup->outro            = outro;
            foreach_break;
        }
        Zone->stageFinishCallback = FBZSetup_StageFinish_EndAct1;
    }

    if (isMainGameMode() && globals->atlEnabled && !CutsceneRules_CheckStageReload()) {
        FBZSetup_ActTransitionLoad();
    }

#if MANIA_USE_PLUS
    if (isMainGameMode() && CutsceneRules_IsAct2())
        Zone->stageFinishCallback = FBZSetup_StageFinish_EndAct2;

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
    TileLayer *layer = RSDK.GetTileLayer(2);
    for (int32 i = 0; i < layer->scrollInfoCount; ++i) {
        layer->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->scrollPos.x = globals->parallaxOffset[id++]; }
}

void FBZSetup_AddDynamicBG(ScanlineInfo *scanlines, int32 parallaxFactorX, int32 parallaxFactorY, int32 startLine, int32 lineCount, int32 sourcePosY)
{
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    int32 scanlineX = (parallaxFactorX * screen->position.x) << 8;
    int32 screenY   = (parallaxFactorY * screen->position.y) >> 8;

    int32 start = startLine - screenY;
    int32 end   = start + lineCount;

    if (start < SCREEN_YSIZE && end > 0) {
        end = MIN(end, SCREEN_YSIZE);

        // sourcePosY = start layer line
        int32 scanlineY = sourcePosY;
        if (start < 0) {
            scanlineY = sourcePosY - (start << 16);
            start     = 0;
        }

        ScanlineInfo *scanlinePtr = &scanlines[start];
        int32 count               = end - start;
        for (int32 i = 0; i < count; ++i) {
            scanlinePtr->position.x = scanlineX;
            scanlinePtr->position.y = scanlineY;
            scanlinePtr->deform.x   = 0x10000;

            ++scanlinePtr;
            scanlineY += 0x10000;
        }
    }
}

void FBZSetup_Scanline_BGInside(ScanlineInfo *scanlines)
{
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    int32 y                = screen->position.y >> 3;

    // Handle the cool deformation on the BG Inside layer
    ScanlineInfo *scanlinePtr = scanlines;
    for (int32 i = 0; i < SCREEN_YSIZE; ++i) {
        int32 pos               = (i + y) & 0x3FF;
        scanlinePtr->deform.x   = FBZSetup->scanlineDeformX[pos];
        scanlinePtr->deform.y   = 0;
        scanlinePtr->position.x = ((screen->position.x << 14) - FBZSetup->scanlineDeformX[pos] * screen->center.x) & 0xFFFF8000;
        scanlinePtr->position.y = FBZSetup->scanlinePosY[pos];
        ++scanlinePtr;
    }

    // Add Other bits of the layer to overlay so it's not so barren
    FBZSetup_AddDynamicBG(scanlines, 128, 32, 0, 112, 576 << 16);
    FBZSetup_AddDynamicBG(scanlines, 128, 32, 456, 128, 512 << 16);
    FBZSetup_AddDynamicBG(scanlines, 112, 64, 420, 16, 816 << 16);
    FBZSetup_AddDynamicBG(scanlines, 144, 80, 640, 48, 768 << 16);
    FBZSetup_AddDynamicBG(scanlines, 160, 96, 512, 64, 704 << 16);
}

void FBZSetup_BGSwitch_ShowInside1(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;

    if (Zone->actID == 1) {
        RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
        RSDK.GetTileLayer(3)->drawGroup[BGSwitch->screenID] = 0;
    }
    else {
        RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
    }
}

void FBZSetup_BGSwitch_ShowInside2(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;

    if (Zone->actID == 1) {
        RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
        RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
        RSDK.GetTileLayer(3)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    }
    else {
        RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    }
}

void FBZSetup_BGSwitch_ShowInside1_NoStorm(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;

    if (Zone->actID == 1) {
        RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
        RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = 0;
    }
    else {
        RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
    }
}

void FBZSetup_Trigger_ShowExterior(void)
{
    int32 id                                                       = (2 * (Zone->actID != 0) + 3);
    RSDK.GetTileLayer(id)->drawGroup[GenericTrigger->playerID]     = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(id + 1)->drawGroup[GenericTrigger->playerID] = 6;

    foreach_active(ParallaxSprite, parallaxSprite)
    {
        if (!parallaxSprite->visible) {
            parallaxSprite->visible = true;
            parallaxSprite->state   = ParallaxSprite_State_FadeIntoHalf;
        }
    }
}

void FBZSetup_Trigger_ShowInterior(void)
{
    int32 id                                                       = (2 * (Zone->actID != 0) + 3);
    RSDK.GetTileLayer(id)->drawGroup[GenericTrigger->playerID]     = 6;
    RSDK.GetTileLayer(id + 1)->drawGroup[GenericTrigger->playerID] = DRAWGROUP_COUNT;

    foreach_active(ParallaxSprite, parallaxSprite) { parallaxSprite->state = ParallaxSprite_State_FadeOut; }
}

void FBZSetup_StageFinish_EndAct1(void) { FBZSetup->outro->active = ACTIVE_NORMAL; }
#if MANIA_USE_PLUS
void FBZSetup_StageFinish_EndAct2(void) { CREATE_ENTITY(FBZ2Outro, NULL, 0, 0); }
#endif

#if GAME_INCLUDE_EDITOR
void FBZSetup_EditorDraw(void) {}

void FBZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK_ENUM_VAR("Show Exterior", FBZ_GENERICTRIGGER_EXTERIOR);
    RSDK_ENUM_VAR("Show Interior", FBZ_GENERICTRIGGER_INTERIOR);

    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Show Inside 1", FBZ_BG_INSIDE1);
    RSDK_ENUM_VAR("Show Inside 2", FBZ_BG_INSIDE2);
    RSDK_ENUM_VAR("Show Inside 1 (No Storm)", FBZ_BG_INSIDE1_NOSTORM);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Blueprint 1", FBZ_DECORATION_BLUEPRINT1);
    RSDK_ENUM_VAR("Blueprint 2", FBZ_DECORATION_BLUEPRINT2);

    RSDK_ACTIVE_VAR(ParallaxSprite, aniID);
    RSDK_ENUM_VAR("Clouds", FBZ_PARALLAXSPRITE_CLOUDS);
}
#endif

void FBZSetup_Serialize(void) {}
