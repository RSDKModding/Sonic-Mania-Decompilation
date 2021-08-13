#include "SonicMania.h"

ObjectCPZSetup *CPZSetup;

void CPZSetup_Update(void) {}

void CPZSetup_LateUpdate(void) {}

void CPZSetup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        --CPZSetup->aniTileFrame;
        CPZSetup->aniTileFrame &= 0xF;
        RSDK.DrawAniTiles(CPZSetup->aniTiles, 762, 109, CPZSetup->aniTileFrame + 48, 16, 16);
    }

    if (!(Zone->timer & 7)) {
        ++CPZSetup->layerPtr->deformationOffset;

        ++CPZSetup->palAnimFrameA;
        CPZSetup->palAnimFrameA &= 0xF;
        RSDK.SetPaletteEntry(0, 159, RSDK.GetPaletteEntry(3, CPZSetup->palAnimFrameA));

        ++CPZSetup->palAnimFrameB;
        CPZSetup->palAnimFrameB %= 21;
        RSDK.SetPaletteEntry(0, 158, RSDK.GetPaletteEntry(3, CPZSetup->palAnimFrameB + 16));

        CPZSetup->palAnimFrameC += 3;
        CPZSetup->palAnimFrameC %= 27;
        RSDK.SetPaletteEntry(0, 141, RSDK.GetPaletteEntry(3, CPZSetup->palAnimFrameC + 48));

        CPZSetup->palAnimFrameD += 3;
        CPZSetup->palAnimFrameD %= 27;
        RSDK.SetPaletteEntry(0, 142, RSDK.GetPaletteEntry(3, CPZSetup->palAnimFrameD + 48));

        CPZSetup->palAnimFrameE += 3;
        CPZSetup->palAnimFrameE %= 27;
        RSDK.SetPaletteEntry(0, 143, RSDK.GetPaletteEntry(3, CPZSetup->palAnimFrameE + 48));
    }
}

void CPZSetup_Draw(void) {}

void CPZSetup_Create(void *data) {}

void CPZSetup_StageLoad(void)
{
    CPZSetup->aniTiles = RSDK.LoadSpriteSheet("CPZ/Objects.gif", SCOPE_STAGE);
    RSDK.SetDrawLayerProperties(0, 0, Water_SetWaterLevel);
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, 0, Water_RemoveWaterEffect);
    Water->waterPalette     = 2;
    CPZSetup->palAnimFrameC = 0;
    CPZSetup->palAnimFrameD = 1;
    CPZSetup->palAnimFrameE = 2;
    CPZSetup->layerPtr      = RSDK.GetSceneLayer(0);
    for (int i = 0; i < 0x400; ++i) {
        CPZSetup->layerPtr->deformationData[i] = CPZSetup->deformation[i & 0x3F];
    }

#if RETRO_USE_PLUS
    if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreCPZ.act", 0xFF);
        RSDK.LoadPalette(2, "EncoreCPZw.act", 0xFF);
        RSDK.CopyPalette(0, 128, 1, 0, 10);
    }
#endif
    Animals->animalTypes[0] = ANIMAL_LOCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;
    if (Zone->actID) {
        RSDK.SetPaletteMask(0xF000);
        RSDK.SetPaletteEntry(0, 0xFF, 0xF000);
        BGSwitch->switchCallback[0] = CPZSetup_BGSwitchCB1;
        BGSwitch->switchCallback[1] = CPZSetup_BGSwitchCB2;
        BGSwitch->layerIDs[0]       = 1;
        BGSwitch->layerIDs[1]       = 1;
        BGSwitch->layerIDs[2]       = 1;
        BGSwitch->layerIDs[3]       = 1;

        bool32 flag = false;
        if (PlayerHelpers_CheckStageReload() || !PlayerHelpers_CheckPlayerPos(0x18900000, 0xAC0000, 0x2560000, 0x19800000)) {
            flag = false;
        }
        else {
            flag                    = true;
            Zone->screenBoundsB1[0] = 0x1980;
            Zone->screenBoundsB1[1] = 0x1980;
#if RETRO_USE_PLUS
            Zone->screenBoundsB1[2] = 0x1980;
            Zone->screenBoundsB1[3] = 0x1980;
#endif
        }

        if (isMainGameMode() && globals->atlEnabled) {
            if (!PlayerHelpers_CheckStageReload())
                Zone_ReloadStoredEntities(0x19800000, 0x1800000, true);
        }

        if (isMainGameMode() && PlayerHelpers_CheckAct2()) {
            foreach_all(CPZ2Outro, outro)
            {
                CPZSetup->outro           = (Entity *)outro;
                Zone->stageFinishCallback = CPZSetup_StageFinishCBAct2;
                foreach_break;
            }
        }

        BGSwitch->screenID = 0;
        if (flag) {
            for (; BGSwitch->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); BGSwitch->screenID++) {
                RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID]   = DRAWLAYER_COUNT;
                RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID]   = DRAWLAYER_COUNT;
                RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID]   = DRAWLAYER_COUNT;
                RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID++] = 0;
            }
            BGSwitch->layerIDs[0] = 1;
            BGSwitch->layerIDs[1] = 1;
            BGSwitch->layerIDs[2] = 1;
            BGSwitch->layerIDs[3] = 1;

            TileLayer *layer = RSDK.GetSceneLayer(3);
            layer->scrollPos += -0x118000 * layer->parallaxFactor;

            for (int i = 0; i < layer->scrollInfoCount; ++i) {
                layer->scrollInfo[i].parallaxFactor += 0x470000 * layer->scrollInfo[i].parallaxFactor;
            }
        }
        else {
            for (; BGSwitch->screenID < RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT); BGSwitch->screenID++) {
                RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID]   = 0;
                RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID]   = 0;
                RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID]   = 0;
                RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID++] = DRAWLAYER_COUNT;
            }
            BGSwitch->layerIDs[0] = 0;
            BGSwitch->layerIDs[1] = 0;
            BGSwitch->layerIDs[2] = 0;
            BGSwitch->layerIDs[3] = 0;
        }
    }
    else {
        if (isMainGameMode()) {
            if (globals->enableIntro) {
                if (!PlayerHelpers_CheckStageReload()) {
                    globals->suppressAutoMusic = true;
                    globals->suppressTitlecard = true;
                }
            }
        }

        if (isMainGameMode()) {
            if (PlayerHelpers_CheckAct1())
                Zone->stageFinishCallback = CPZSetup_StageFinishCBAct1;
        }
        RSDK.SetPaletteEntry(0, 0xFF, 0xF0F0F0);
        RSDK.SetPaletteEntry(2, 0xFF, RSDK.GetPaletteEntry(2, 137));
    }
}

void CPZSetup_BGSwitchCB1(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = 0;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
}

void CPZSetup_BGSwitchCB2(void)
{
    RSDK.GetSceneLayer(0)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(1)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(2)->drawLayer[BGSwitch->screenID] = DRAWLAYER_COUNT;
    RSDK.GetSceneLayer(3)->drawLayer[BGSwitch->screenID] = 0;
}

void CPZSetup_StageFinishCBAct1(void)
{
    RSDK.GetSceneLayer(0);
    Zone_StoreEntities((RSDK_screens->position.x + RSDK_screens->centerX) << 16, (RSDK_screens->height + RSDK_screens->position.y) << 16);
    RSDK.LoadScene();
}

void CPZSetup_StageFinishCBAct2(void) { CPZSetup->outro->active = ACTIVE_NORMAL; }

void CPZSetup_EditorDraw(void) {}

void CPZSetup_EditorLoad(void) {}

void CPZSetup_Serialize(void) {}
