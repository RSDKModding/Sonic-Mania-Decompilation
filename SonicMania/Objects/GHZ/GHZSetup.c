// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GHZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGHZSetup *GHZSetup;

void GHZSetup_Update(void) {}

void GHZSetup_LateUpdate(void) {}

void GHZSetup_StaticUpdate(void)
{
    GHZSetup->paletteTimer += 42;
    if (GHZSetup->paletteTimer >= 0x100) {
        GHZSetup->paletteTimer -= 0x100;
        RSDK.RotatePalette(1, 181, 184, true);
        RSDK.RotatePalette(2, 181, 184, true);
        RSDK.RotatePalette(1, 197, 200, true);
        RSDK.RotatePalette(2, 197, 200, true);
    }
    RSDK.SetLimitedFade(0, 1, 2, GHZSetup->paletteTimer, 181, 184);
    RSDK.SetLimitedFade(0, 1, 2, GHZSetup->paletteTimer, 197, 200);

    --GHZSetup->sunFlowerTimer;
    if (GHZSetup->sunFlowerTimer < 1) {
        ++GHZSetup->sunFlowerFrame;
        GHZSetup->sunFlowerFrame &= 7;
        GHZSetup->sunFlowerTimer = GHZSetup->sunFlowerDurationTable[GHZSetup->sunFlowerFrame];
        RSDK.DrawAniTiles(GHZSetup->aniTiles, 427, 0, 32 * GHZSetup->sunFlowerFrame, 32, 32);
    }

    --GHZSetup->extendFlowerTimer;
    if (GHZSetup->extendFlowerTimer < 1) {
        ++GHZSetup->extendFlowerFrame;
        GHZSetup->extendFlowerFrame &= 0xF;
        GHZSetup->extendFlowerTimer = GHZSetup->extendFlowerDurationTable[GHZSetup->extendFlowerFrame];
        RSDK.DrawAniTiles(GHZSetup->aniTiles, 431, 0, 48 * GHZSetup->extendFlowerFrame + 256, 32, 48);
    }
}

void GHZSetup_Draw(void) {}

void GHZSetup_Create(void *data) {}

void GHZSetup_StageLoad(void)
{
    GHZSetup->aniTiles = RSDK.LoadSpriteSheet("GHZ/AniTiles.gif", SCOPE_STAGE);

    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;

    if (Zone->actID) {
        BGSwitch->switchCallback[GHZ_BG_OUTSIDE] = GHZSetup_BGSwitch_Outside_Act2;
        BGSwitch->switchCallback[GHZ_BG_CAVES]   = GHZSetup_BGSwitch_Caves_Act2;

        BGSwitch->layerIDs[0] = GHZ_BG_OUTSIDE;
        BGSwitch->layerIDs[1] = GHZ_BG_OUTSIDE;
        BGSwitch->layerIDs[2] = GHZ_BG_OUTSIDE;
        BGSwitch->layerIDs[3] = GHZ_BG_OUTSIDE;

        if (isMainGameMode() && globals->atlEnabled) {
            if (!CutsceneRules_CheckStageReload())
                GHZSetup_HandleActTransition();
        }

        if (isMainGameMode() && CutsceneRules_IsAct2())
            Zone->stageFinishCallback = GHZSetup_StageFinish_EndAct2;
    }
    else {
        TileLayer *backgroundOutside      = RSDK.GetTileLayer(0);
        backgroundOutside->scrollPos      = 0x180000;
        backgroundOutside->parallaxFactor = -backgroundOutside->parallaxFactor;

        BGSwitch->switchCallback[GHZ_BG_OUTSIDE] = GHZSetup_BGSwitch_Outside_Act1;
        BGSwitch->switchCallback[GHZ_BG_CAVES]   = GHZSetup_BGSwitch_Caves_Act1;

        if (!isMainGameMode() || !globals->atlEnabled || CutsceneRules_CheckStageReload()) {
            BGSwitch->layerIDs[0] = GHZ_BG_OUTSIDE;
            BGSwitch->layerIDs[1] = GHZ_BG_OUTSIDE;
            BGSwitch->layerIDs[2] = GHZ_BG_OUTSIDE;
            BGSwitch->layerIDs[3] = GHZ_BG_OUTSIDE;
        }
        else {
            GHZSetup_SetupAct1BG();
        }

        if (isMainGameMode() && CutsceneRules_IsAct1())
            Zone->stageFinishCallback = GHZSetup_StageFinish_EndAct1;
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE || (RSDK.CheckSceneFolder("GHZCutscene") && globals->gameMode == MODE_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreGHZ.act", 0b0000000011111111);
        RSDK.CopyPalette(0, 128, 1, 128, 80);
        RSDK.CopyPalette(0, 128, 2, 128, 80);
        RSDK.RotatePalette(2, 181, 184, true);
        RSDK.RotatePalette(2, 197, 200, true);

        if (Water) {
            foreach_all(Water, water)
            {
                water->r = 0xA0;
                water->g = 0xA0;
                water->b = 0xE0;
            }
        }
    }
#endif
}

void GHZSetup_SetupAct1BG(void)
{
    if (CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
        BGSwitch->layerIDs[0] = GHZ_BG_CAVES;
        BGSwitch->layerIDs[1] = GHZ_BG_CAVES;
        BGSwitch->layerIDs[2] = GHZ_BG_CAVES;
        BGSwitch->layerIDs[3] = GHZ_BG_CAVES;

        for (BGSwitch->screenID = 0; BGSwitch->screenID < RSDK.GetVideoSetting(VIDEOSETTING_SCREENCOUNT); ++BGSwitch->screenID) {
            GHZSetup_BGSwitch_Caves_Act2();
        }
        Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 1724 << 16, true);
    }
    else {
        Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 1004 << 16, true);
        TileLayer *outsideLayer = RSDK.GetTileLayer(RSDK.GetTileLayerID("BG Outside"));
        EntityPlayer *player    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        player->onGround = true;
        player->state    = Player_State_Ground;
        for (int32 s = 0; s < outsideLayer->scrollInfoCount; ++s) {
            outsideLayer->scrollInfo[s].scrollPos += globals->parallaxOffset[0] * outsideLayer->scrollInfo[s].scrollSpeed;
        }
    }
}
void GHZSetup_StageFinish_EndAct1(void)
{
    Zone_StoreEntities(15792 << 16, 1588 << 16);
    RSDK.LoadScene();
}
void GHZSetup_HandleActTransition(void)
{
    Zone->cameraBoundsL[0] = 256 - ScreenInfo->center.x;
    Zone->cameraBoundsB[0] = 1412;

    Zone_ReloadStoredEntities(256 << 16, 1412 << 16, true);

    TileLayer *bgCave1 = RSDK.GetTileLayer(2);
    bgCave1->scrollPos += 0xB000 * bgCave1->parallaxFactor;
    for (int32 s = 0; s < bgCave1->scrollInfoCount; ++s) {
        bgCave1->scrollInfo[s].scrollPos += 0x3CB000 * bgCave1->scrollInfo[s].parallaxFactor;
    }

    TileLayer *bgCave2 = RSDK.GetTileLayer(3);
    bgCave2->scrollPos += 0xB000 * bgCave2->parallaxFactor;
    for (int32 s = 0; s < bgCave2->scrollInfoCount; ++s) {
        bgCave2->scrollInfo[s].scrollPos += 0x3CB000 * bgCave2->scrollInfo[s].parallaxFactor;
    }

    for (BGSwitch->screenID = 0; BGSwitch->screenID < RSDK.GetVideoSetting(VIDEOSETTING_SCREENCOUNT); ++BGSwitch->screenID) {
        GHZSetup_BGSwitch_Caves_Act2();
    }
}
void GHZSetup_BGSwitch_Outside_Act2(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(3)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
}
void GHZSetup_BGSwitch_Caves_Act2(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(3)->drawGroup[BGSwitch->screenID] = 0;
}
void GHZSetup_BGSwitch_Outside_Act1(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
}
void GHZSetup_BGSwitch_Caves_Act1(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(2)->drawGroup[BGSwitch->screenID] = 0;
}
void GHZSetup_StageFinish_EndAct2(void) { CREATE_ENTITY(GHZ2Outro, INT_TO_VOID(true), 0, 0); }

#if GAME_INCLUDE_EDITOR
void GHZSetup_EditorDraw(void) {}

void GHZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Outside BG", GHZ_BG_OUTSIDE);
    RSDK_ENUM_VAR("Caves BG", GHZ_BG_CAVES);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Bridge Post", GHZ_DECORATION_BRIDGEPOST);
}
#endif

void GHZSetup_Serialize(void) {}
