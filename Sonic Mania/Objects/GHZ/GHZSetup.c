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
        BGSwitch->switchCallback[0] = GHZSetup_SetupBGSwitchA1;
        BGSwitch->switchCallback[1] = GHZSetup_SetupBGSwitchA2;
        BGSwitch->layerIDs[0]       = 0;
        BGSwitch->layerIDs[1]       = 0;
        BGSwitch->layerIDs[2]       = 0;
        BGSwitch->layerIDs[3]       = 0;
        if (isMainGameMode() && globals->atlEnabled) {
            if (!PlayerHelpers_CheckStageReload())
                GHZSetup_HandleActTransition();
        }

        if (isMainGameMode() && PlayerHelpers_CheckAct2())
            Zone->stageFinishCallback = GHZSetup_SpawnGHZ2Outro;
    }
    else {
        TileLayer *bg               = RSDK.GetSceneLayer(0);
        bg->scrollPos               = 0x180000;
        bg->parallaxFactor          = -bg->parallaxFactor;
        BGSwitch->switchCallback[0] = GHZSetup_SetupBGSwitchB1;
        BGSwitch->switchCallback[1] = GHZSetup_SetupBGSwitchB2;
        if (!isMainGameMode() || !globals->atlEnabled || PlayerHelpers_CheckStageReload()) {
            BGSwitch->layerIDs[0] = 0;
            BGSwitch->layerIDs[1] = 0;
            BGSwitch->layerIDs[2] = 0;
            BGSwitch->layerIDs[3] = 0;
        }
        else {
            GHZSetup_SetupStartingBG();
        }

        if (isMainGameMode() && PlayerHelpers_CheckAct1())
            Zone->stageFinishCallback = GHZSetup_SetupActTransition;
    }

#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE || (RSDK.CheckStageFolder("GHZCutscene") && globals->gameMode == MODE_ENCORE)) {
        RSDK.LoadPalette(0, "EncoreGHZ.act", 0xFF);
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
    if ((globals->playerID & 0xFF) == ID_KNUCKLES) {
        BGSwitch->layerIDs[0] = 1;
        BGSwitch->layerIDs[1] = 1;
        BGSwitch->layerIDs[2] = 1;
        BGSwitch->layerIDs[3] = 1;
        for (BGSwitch->screenID = 0; BGSwitch->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); ++BGSwitch->screenID) {
            RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
            RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
            RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = 0;
        }
        Zone_ReloadStoredEntities(0x6BC0000, 0xD40000, true);
    }
    else {
        Zone_ReloadStoredEntities(0x3EC0000, 0xD40000, true);
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
    Zone_StoreEntities(0x3DB00000, 0x6340000);
    RSDK.LoadScene();
}
void GHZSetup_HandleActTransition(void)
{
    Zone->screenBoundsL1[0] = 0x100 - RSDK_screens->centerX;
    Zone->screenBoundsB1[0] = 1412;
    Zone_ReloadStoredEntities(0x5840000, 0x1000000, true);

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

    BGSwitch->screenID = 0;
    for (int32 s = RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); BGSwitch->screenID < s;) {
        RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID]   = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID]   = DRAWLAYER_COUNT;
        RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID]   = 0;
        RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID++] = 0;
    }
}
void GHZSetup_SetupBGSwitchA1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
}
void GHZSetup_SetupBGSwitchA2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = 0;
}
void GHZSetup_SetupBGSwitchB1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
}
void GHZSetup_SetupBGSwitchB2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
}
void GHZSetup_SpawnGHZ2Outro(void) { CREATE_ENTITY(GHZ2Outro, intToVoid(1), 0, 0); }

void GHZSetup_EditorDraw(void) {}

void GHZSetup_EditorLoad(void) {}

void GHZSetup_Serialize(void) {}
