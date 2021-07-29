#include "SonicMania.h"

ObjectFBZSetup *FBZSetup;

void FBZSetup_Update(void)
{

}

void FBZSetup_LateUpdate(void)
{

}

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

void FBZSetup_Draw(void)
{

}

void FBZSetup_Create(void* data)
{

}

void FBZSetup_StageLoad(void)
{
    FBZSetup->aniTiles          = RSDK.LoadSpriteSheet("FBZ/AniTiles.gif", SCOPE_STAGE);
    FBZSetup->bgOutsideLayer    = RSDK.GetSceneLayerID("Background Outside");
    Animals->animalTypes[0]     = ANIMAL_FLICKY;
    Animals->animalTypes[1]     = ANIMAL_RICKY;
    BGSwitch->switchCallback[0] = FBZSetup_BGSwitchCB_A;
    BGSwitch->switchCallback[1] = FBZSetup_BGSwitchCB_B;
    BGSwitch->switchCallback[2] = FBZSetup_BGSwitchCB_C;
    TileLayer *bgLayer          = RSDK.GetSceneLayer(0);
    bgLayer->drawLayer[0]       = 0;
    bgLayer->drawLayer[1]       = 0;
    bgLayer->drawLayer[2]       = 0;
    bgLayer->drawLayer[3]       = 0;
    if (!Zone->actID)
        bgLayer->scanlineCallback = FBZSetup_ScanlineCallback;
    
    int val = -336;
    int val2 = 0;
    for (int i = 0; i < 0x400; ++i) {
        int angle = val >> 1;
        if (RSDK.Sin1024(angle) >= 0)
            FBZSetup->array1[i] = 32 * (RSDK.Sin1024(angle) + 0x400);
        else
            FBZSetup->array1[i] = 32 * (-RSDK.Sin1024(angle) + 0x400);

        if (RSDK.Cos1024(angle) >= 0)
            FBZSetup->array2[i] = 32 * (RSDK.Cos1024(angle) + 0x800);
        else
            FBZSetup->array2[i] = 32 * (-RSDK.Cos1024(angle) + 0x800);

        FBZSetup->array3[i] = val2 & 0x1FF0000;
        val += 3;
        val2 += FBZSetup->array2[i];
    }

    RSDK_ACTIVE_VAR(GenericTrigger, triggerID);
    RSDK.AddVarEnumValue("Show Exterior");
    RSDK.AddVarEnumValue("Show Interior");
    GenericTrigger->callbacks[0] = FBZSetup_GenericTriggerCB_A;
    GenericTrigger->callbacks[1] = FBZSetup_GenericTriggerCB_B;
    if (globals->gameMode == MODE_COMPETITION) {
        foreach_all(ParallaxSprite, parallaxSprite) { destroyEntity(parallaxSprite); }
    }

    if (isMainGameMode() && PlayerHelpers_CheckAct1()) {
        foreach_all(FBZ1Outro, outro) {
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

    if ((RSDK_sceneInfo->filter & FILTER_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreFBZ.act", 0xFF);
        RSDK.CopyPalette(0, 1, 1, 1, 0xFF);
        RSDK.LoadPalette(2, "EncoreFBZs.act", 0xFF);
        RSDK.LoadPalette(3, "EncoreFBZf.act", 0xFF);
        RSDK.LoadPalette(4, "EncoreFBZi.act", 0xFF);
    }
#endif
}

void FBZSetup_ActTransitionLoad(void)
{
    Zone_ReloadStoredEntities(0x10E40000, 0xD40000, false);
    Zone->screenBoundsL1[0] = 0;
    Zone->screenBoundsB1[0] = 4324;
    Zone->screenBoundsL1[1] = 0;
    Zone->screenBoundsB1[1] = 4324;
#if RETRO_USE_PLUS
    Zone->screenBoundsL1[2] = 0;
    Zone->screenBoundsB1[2] = 4324;
    Zone->screenBoundsL1[3] = 0;
    Zone->screenBoundsB1[3] = 4324;
#endif

    int id = 0;
    TileLayer *layer                          = RSDK.GetSceneLayer(2);
    for (int i = 0; i < layer->scrollInfoCount; ++i) {
        layer->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite) { 
        parallaxSprite->scrollPos.x = globals->parallaxOffset[id++];
    }
}

void FBZSetup_Unknown5(ScanlineInfo *scanlines, int a1, int a3, int a4, int a5, int a6) {
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    int val            = (a3 * screen->position.y) >> 8;
    int start          = a4 - val;
    int end            = a4 - val + a5;
    int x              = (a1 * screen->position.x) << 8;
    if (a4 - val < SCREEN_YSIZE && end > 0) {
        int y = a6;
        if (start < 0) {
            y     = a6 - (start << 16);
            start = 0;
        }
        if (end > SCREEN_YSIZE)
            end = SCREEN_YSIZE;
        ScanlineInfo *scanlinePtr = &scanlines[start];

        for (int i = 0; i < end - start; ++i) {
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
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    int y                  = screen->position.y >> 3;
    ScanlineInfo *scanlinePtr = scanlines;

    for (int i = 0; i < SCREEN_YSIZE; ++i) {
        int pos               = (i + y) & 0x3FF;
        scanlinePtr->deform.x   = FBZSetup->array1[pos];
        scanlinePtr->deform.y   = 0;
        scanlinePtr->position.x = ((screen->position.x << 14) - FBZSetup->array1[pos] * screen->centerX) & 0xFFFF8000;
        scanlinePtr->position.y = FBZSetup->array3[pos];
        ++scanlinePtr;
    }

    FBZSetup_Unknown5(scanlines, 128, 32, 0, 112, 0x2400000);
    FBZSetup_Unknown5(scanlines, 128, 32, 456, 128, 0x2000000);
    FBZSetup_Unknown5(scanlines, 112, 64, 420, 16, 0x3300000);
    FBZSetup_Unknown5(scanlines, 144, 80, 640, 48, 0x3000000);
    FBZSetup_Unknown5(scanlines, 160, 96, 512, 64, 0x2C00000);
}

void FBZSetup_BGSwitchCB_A(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
    if (Zone->actID == 1) {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->layerID] = 0;
    }
    else {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = 0;
    }
}

void FBZSetup_BGSwitchCB_B(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->layerID] = 0;
    if (Zone->actID == 1) {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = 0;
        RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
    }
    else {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
    }
}

void FBZSetup_BGSwitchCB_C(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
    if (Zone->actID == 1) {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->layerID] = 0;
    }
    else {
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->layerID] = 0;
    }
}

void FBZSetup_GenericTriggerCB_A(void)
{
    int id                                                          = (2 * (Zone->actID != 0) + 3);
    RSDK.GetSceneLayer(id)->drawLayer[GenericTrigger->playerID]     = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(id + 1)->drawLayer[GenericTrigger->playerID] = 6;

    foreach_active(ParallaxSprite, parallaxSprite)
    {
        if (!parallaxSprite->visible) {
            parallaxSprite->visible = true;
            parallaxSprite->state   = ParallaxSprite_Unknown4;
        }
    }
}

void FBZSetup_GenericTriggerCB_B(void)
{
    int id                                                          = (2 * (Zone->actID != 0) + 3);
    RSDK.GetSceneLayer(id)->drawLayer[GenericTrigger->playerID]     = 6;
    RSDK.GetSceneLayer(id + 1)->drawLayer[GenericTrigger->playerID] = DRAWLAYER_COUNT;

    foreach_active(ParallaxSprite, parallaxSprite) { parallaxSprite->state = ParallaxSprite_Unknown5; }
}

void FBZSetup_StageFinishCB_Act1(void) { FBZSetup->outroPtr->active = ACTIVE_NORMAL; }
#if RETRO_USE_PLUS
void FBZSetup_StageFinishCB_Act2(void) { RSDK.CreateEntity(FBZ2Outro->objectID, NULL, 0, 0); }
#endif

void FBZSetup_EditorDraw(void)
{

}

void FBZSetup_EditorLoad(void)
{

}

void FBZSetup_Serialize(void)
{

}

