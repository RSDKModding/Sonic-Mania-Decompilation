// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GHZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectGHZSetup *GHZSetup;

void GHZSetup_Update(void) {}

void GHZSetup_LateUpdate(void) {}

void GHZSetup_StaticUpdate(void)
{
    GHZSetup->waterFadeAmount += 42;
    if (GHZSetup->waterFadeAmount > 255) {
        GHZSetup->waterFadeAmount -= 0x100;
        RSDK.RotatePalette(1, 181, 184, true);
        RSDK.RotatePalette(2, 181, 184, true);
        RSDK.RotatePalette(1, 197, 200, true);
        RSDK.RotatePalette(2, 197, 200, true);
    }
    RSDK.SetLimitedFade(0, 1, 2, GHZSetup->waterFadeAmount, 181, 184);
    RSDK.SetLimitedFade(0, 1, 2, GHZSetup->waterFadeAmount, 197, 200);

    --GHZSetup->delayA;
    if (GHZSetup->delayA < 1) {
        ++GHZSetup->frameA;
        GHZSetup->frameA &= 7;
        GHZSetup->delayA = GHZSetup->delayListA[GHZSetup->frameA];
        RSDK.DrawAniTiles(GHZSetup->aniTiles, 427, 0, 32 * GHZSetup->frameA, 32, 32);
    }

    --GHZSetup->delayB;
    if (GHZSetup->delayB < 1) {
        ++GHZSetup->frameB;
        GHZSetup->frameB &= 0xF;
        GHZSetup->delayB = GHZSetup->delayListB[GHZSetup->frameB];
        RSDK.DrawAniTiles(GHZSetup->aniTiles, 431, 0, 48 * GHZSetup->frameB + 256, 32, 48);
    }
}

void GHZSetup_Draw(void) {}

void GHZSetup_Create(void *data) {}

void GHZSetup_StageLoad(void)
{
    GHZSetup->aniTiles      = RSDK.LoadSpriteSheet("GHZ/AniTiles.gif", SCOPE_STAGE);
    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;
    if (Zone->actID) {
        BGSwitch->switchCallback[GHZ_BG_OUTSIDE] = GHZSetup_BGSwitch_CB_Outside_Act2;
        BGSwitch->switchCallback[GHZ_BG_CAVES]   = GHZSetup_BGSwitch_CB_Caves_Act2;
        BGSwitch->layerIDs[0]                    = GHZ_BG_OUTSIDE;
        BGSwitch->layerIDs[1]                    = GHZ_BG_OUTSIDE;
        BGSwitch->layerIDs[2]                    = GHZ_BG_OUTSIDE;
        BGSwitch->layerIDs[3]                    = GHZ_BG_OUTSIDE;
        if (isMainGameMode() && globals->atlEnabled) {
            if (!PlayerHelpers_CheckStageReload())
                GHZSetup_HandleActTransition();
        }

        if (isMainGameMode() && PlayerHelpers_CheckAct2())
            Zone->stageFinishCallback = GHZSetup_SpawnGHZ2Outro;
    }
    else {
        TileLayer *bg                            = RSDK.GetSceneLayer(0);
        bg->scrollPos                            = 0x180000;
        bg->parallaxFactor                       = -bg->parallaxFactor;
        BGSwitch->switchCallback[GHZ_BG_OUTSIDE] = GHZSetup_BGSwitch_CB_Outside_Act1;
        BGSwitch->switchCallback[GHZ_BG_CAVES]   = GHZSetup_BGSwitch_CB_Caves_Act1;
        if (!isMainGameMode() || !globals->atlEnabled || PlayerHelpers_CheckStageReload()) {
            BGSwitch->layerIDs[0] = GHZ_BG_OUTSIDE;
            BGSwitch->layerIDs[1] = GHZ_BG_OUTSIDE;
            BGSwitch->layerIDs[2] = GHZ_BG_OUTSIDE;
            BGSwitch->layerIDs[3] = GHZ_BG_OUTSIDE;
        }
        else {
            GHZSetup_SetupStartingBG();
        }

        if (isMainGameMode() && PlayerHelpers_CheckAct1())
            Zone->stageFinishCallback = GHZSetup_SetupActTransition;
    }

#if RETRO_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE || (RSDK.CheckStageFolder("GHZCutscene") && globals->gameMode == MODE_ENCORE)) {
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

void GHZSetup_SetupStartingBG(void)
{
    if (checkPlayerID(ID_KNUCKLES, 1)) {
        BGSwitch->layerIDs[0] = GHZ_BG_CAVES;
        BGSwitch->layerIDs[1] = GHZ_BG_CAVES;
        BGSwitch->layerIDs[2] = GHZ_BG_CAVES;
        BGSwitch->layerIDs[3] = GHZ_BG_CAVES;
        for (BGSwitch->screenID = 0; BGSwitch->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++BGSwitch->screenID) {
            GHZSetup_BGSwitch_CB_Caves_Act2();
        }
        Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 1724 << 16, true);
    }
    else {
        Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 1004 << 16, true);
        TileLayer *outsideLayer = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("BG Outside"));
        EntityPlayer *player    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        player->onGround = true;
        player->state    = Player_State_Ground;
        for (int32 s = 0; s < outsideLayer->scrollInfoCount; ++s) {
            outsideLayer->scrollInfo[s].scrollPos += globals->parallaxOffset[0] * outsideLayer->scrollInfo[s].scrollSpeed;
        }
    }
}
void GHZSetup_SetupActTransition(void)
{
    Zone_StoreEntities(15792 << 16, 1588 << 16);
    RSDK.LoadScene();
}
void GHZSetup_HandleActTransition(void)
{
    Zone->cameraBoundsL[0] = 0x100 - ScreenInfo->centerX;
    Zone->cameraBoundsB[0] = 1412;
    Zone_ReloadStoredEntities(256 << 16, 1412 << 16, true);

    TileLayer *layer2 = RSDK.GetSceneLayer(2);
    layer2->scrollPos += 0xB000 * layer2->parallaxFactor;
    for (int32 s = 0; s < layer2->scrollInfoCount; ++s) {
        layer2->scrollInfo[s].scrollPos += 0x3CB000 * layer2->scrollInfo[s].parallaxFactor;
    }

    TileLayer *layer3 = RSDK.GetSceneLayer(3);
    layer3->scrollPos += 0xB000 * layer3->parallaxFactor;
    for (int32 s = 0; s < layer3->scrollInfoCount; ++s) {
        layer3->scrollInfo[s].scrollPos += 0x3CB000 * layer3->scrollInfo[s].parallaxFactor;
    }

    for (BGSwitch->screenID = 0; BGSwitch->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++BGSwitch->screenID) {
        GHZSetup_BGSwitch_CB_Caves_Act2();
    }
}
void GHZSetup_BGSwitch_CB_Outside_Act2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
}
void GHZSetup_BGSwitch_CB_Caves_Act2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = 0;
}
void GHZSetup_BGSwitch_CB_Outside_Act1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
}
void GHZSetup_BGSwitch_CB_Caves_Act1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
}
void GHZSetup_SpawnGHZ2Outro(void) { CREATE_ENTITY(GHZ2Outro, intToVoid(1), 0, 0); }

#if RETRO_INCLUDE_EDITOR
void GHZSetup_EditorDraw(void) {}

void GHZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Outside BG", GHZ_BG_OUTSIDE);
    RSDK_ENUM_VAR("Caves BG", GHZ_BG_CAVES);
}
#endif

void GHZSetup_Serialize(void) {}
