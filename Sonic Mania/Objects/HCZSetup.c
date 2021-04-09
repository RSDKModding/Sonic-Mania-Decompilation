#include "../SonicMania.h"

ObjectHCZSetup *HCZSetup;

void HCZSetup_Update(void)
{

}

void HCZSetup_LateUpdate(void)
{

}

void HCZSetup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        ++HCZSetup->bg->deformationOffsetW;
        if (!(Zone->timer & 3)) {
            HCZSetup->timerA += 16;
            HCZSetup->timerA &= 0x3F;
            RSDK.DrawAniTiles(HCZSetup->aniTiles1, 386, 0, HCZSetup->timerA, 64, 16);

            HCZSetup->timerB += 32;
            HCZSetup->timerB &= 0x1FF;
            RSDK.DrawAniTiles(HCZSetup->aniTiles2, 820, HCZSetup->timerB, 0, 32, 96);
            RSDK.DrawAniTiles(HCZSetup->aniTiles2, 857, HCZSetup->timerB, 96, 32, 128);
            RSDK.RotatePalette(0, 176, 182, true);
        }
    }

    int *sizes = HCZSetup->aniTilesDelay;
    int sizeX         = 0;
    int sizeY         = 0;

    for (int i = 0; i < 7; ++i) {
        sizeX += sizes[0];
        sizeY += sizes[1];
        sizes += 2;
    }

    int val   = Zone->timer % (sizeY + sizeX);
    int valID = 0;

    while (val >= HCZSetup->aniTilesDelay[valID]) {
        val -= HCZSetup->aniTilesDelay[valID];
        ++valID;
    }

    if (!val) {
        RSDK.DrawAniTiles(HCZSetup->aniTiles3, 874, 0, 16 * valID, 32, 16);
    }

    if (!(Zone->timer & 1)) {
        for (int id = Zone->fgLow; id <= Zone->fgHigh; ++id) {
            TileLayer *layer = RSDK.GetSceneLayer(id);
            ++layer->deformationOffsetW;
        }
    }

    if (HCZSetup->activePlayerCount) {
        if (HCZSetup->playingLoopSFX) {
            if (!(HCZSetup->waterfallSFXTimer & 0x1F)) {
                RSDK.StopSFX(HCZSetup->sfxWaterfall);
                RSDK.PlaySFX(HCZSetup->sfxWaterfallLoop, 0, 255);
            }
            ++HCZSetup->waterfallSFXTimer;
        }
        else {
            HCZSetup->waterfallSFXTimer = 0;
            HCZSetup->playingLoopSFX = true;
            RSDK.PlaySFX(HCZSetup->sfxWaterfall, 0, 255);
            ++HCZSetup->waterfallSFXTimer;
        }
    }
    else {
        HCZSetup->playingLoopSFX = false;
    }

    HCZSetup->activePlayerCount = 0;
    foreach_active(Player, player) {
        RSDK.GetEntityID(player);
        if (player->state != Player_State_None) {
            Hitbox *hitbox = Player_GetHitbox(player);
            ushort tile = RSDK.GetTileInfo(Zone->fgHigh, player->position.x >> 20, ((hitbox->bottom << 16) + player->position.y - 0x10000) >> 20)
                  & 0x3FF;
            if ((tile >= 0xE2 && tile <= 0xF4 || tile - 0x370 <= 8) && player->collisionPlane == 1) {
                if (player->state != Player_State_BubbleBounce && player->state != Player_State_MightyHammerDrop) {
                    if (player->onGround) {
                        if (player->state != Player_State_WaterSlide) {
                            player->interaction    = true;
                            player->tileCollisions = true;
                            player->position.y += player->cameraOffset;
                            player->nextGroundState = StateMachine_None;
                            player->state           = Player_State_WaterSlide;
                        }

                        if (player->onGround) {
                            ++HCZSetup->activePlayerCount;
                        }
                    }
                    else if (player->playerAnimData.animationID == ANI_FLUME) {
                        ++HCZSetup->activePlayerCount;
                    }
                }
            }
        }
    }
}

void HCZSetup_Draw(void)
{

}

void HCZSetup_Create(void* data)
{

}

void HCZSetup_StageLoad(void)
{
    HCZSetup->aniTiles1     = RSDK.LoadSpriteSheet("HCZ/AniTiles.gif", SCOPE_STAGE);
    HCZSetup->aniTiles2     = RSDK.LoadSpriteSheet("HCZ/AniTiles2.gif", SCOPE_STAGE);
    HCZSetup->aniTiles3     = RSDK.LoadSpriteSheet("HCZ/AniTiles3.gif", SCOPE_STAGE);

    HCZSetup->bg            = RSDK.GetSceneLayer(1);
    for (int i = 0; i < 0x200; i += 0x10) {
        int val = RSDK.Rand(0, 4);

        int pos = i;
        pos     = minVal(0x200, pos);
        pos     = maxVal(0x000, pos);

        int *deformData = &HCZSetup->bg->deformationDataW[pos];

        int angle = 0;
        for (int d = 0; d < 0x10; ++d) {
            *deformData = val * RSDK.Sin1024(angle) >> 10;
            angle += 64;
        }

        memcpy(&HCZSetup->bg->deformationDataW[0x200], HCZSetup->bg->deformationDataW, 0x200 * sizeof(int));
    }

    if (Zone->actID == 1) {
        HCZSetup->bg->scanlineCallback = HCZSetup_ScanlineCallback;
    }

    int id = Zone->fgLow;
    while (id <= Zone->fgHigh) {
        TileLayer *layer = RSDK.GetSceneLayer(id);
        int* deformData              = layer->deformationDataW;

        for (int i = 0; i < 4; ++i) {
            deformData[0] = 1;
            deformData[1] = 1;
            deformData[2] = 2;
            deformData[3] = 2;
            deformData[4] = 3;
            deformData[5] = 3;
            deformData[6] = 3;
            deformData[7] = 3;
            deformData[8] = 2;
            deformData[9] = 2;
            deformData[10] = 1;
            deformData[11] = 1;

            deformData[128] = 1;
            deformData[129] = 1;
            deformData[130] = 2;
            deformData[131] = 2;
            deformData[132] = 3;
            deformData[133] = 3;
            deformData[134] = 3;
            deformData[135] = 3;
            deformData[136] = 2;
            deformData[137] = 2;
            deformData[138] = 1;
            deformData[139] = 1;

            deformData[160]  = -1;
            deformData[161]  = -1;
            deformData[162]  = -2;
            deformData[163]  = -2;
            deformData[164]  = -3;
            deformData[165]  = -3;
            deformData[166]  = -3;
            deformData[167]  = -3;
            deformData[168]  = -2;
            deformData[169]  = -2;
            deformData[170]  = -1;
            deformData[171]  = -1;

            deformData += 0x100;
        }
        id++;
    }
    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_ROCKY;
    RSDK.SetDrawLayerProperties(0, false, Water_SetWaterLevel);
    RSDK.SetDrawLayerProperties(Zone->uiDrawHigh, false, Water_RemoveWaterEffect);
    
    Water->waterPalette = 1;
    if (Zone->actID) {
        if (!PlayerHelpers_CheckStageReload())
            PlayerHelpers_CheckPlayerPos(0x5900000, 0xB00000, 0x2600000, 0x6800000);
        Zone->screenBoundsL1[0] = 0xA8;
        Zone->screenBoundsL1[1] = 0xA8;
        Zone->screenBoundsL1[2] = 0xA8;
        Zone->screenBoundsL1[3] = 0xA8;
        if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && globals->atlEnabled && !PlayerHelpers_CheckStageReload()) {
            Zone_ReloadStoredEntities(0x6A00000, 0x1840000, true);
        }
        Zone->stageFinishCallback = HCZSetup_HandleCutscene;
    }
    else if ((globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE) && PlayerHelpers_CheckAct1()) {
        Zone->forcePlayerOnScreenFlag = true;
        Zone->stageFinishCallback     = HCZSetup_HandleActTransition;
    }

    if (RSDK_sceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreHCZ.act", 255);
        RSDK.LoadPalette(1, "EncoreHCZw.act", 255);
    }

    HCZSetup->sfxWaterfall     = RSDK.GetSFX("Stage/Waterfall.wav");
    HCZSetup->sfxWaterfallLoop = RSDK.GetSFX("Stage/Waterfall2.wav");
}

void HCZSetup_ScanlineCallback(ScanlineInfo* scanlines) {
    RSDK.ProcessParallax(HCZSetup->bg);

    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    int scrH           = screen->height;
    int scrX           = screen->position.x;
    int scrY           = screen->position.y;
    int height         = 0x210 - (scrY >> 2);
    int waterPos       = (Water->waterLevel >> 0x10) - scrY;

    int val = 0x640000 / (abs(height - waterPos) < 1 ? 1 : abs(height - waterPos));
    if (val < 0x10000)
        val = 0x10000;

    height = minVal(scrH, height);
    height = maxVal(0, height);

    waterPos = minVal(scrH, waterPos);
    waterPos = maxVal(0, waterPos);

    ScanlineInfo *scanlinePtr = &scanlines[height];
    if (height >= waterPos) {
        int pos = 0x4D00000;
        if (waterPos < height) {
            for (int i = 0; i < height - waterPos; ++i) {
                scanlinePtr->position.y = pos;
                int distance            = 0x470 - (pos >> 16);
                // This is awful code, please find a way to do this with a cleaner algorithm
                int a = (int)((int64)(67553994424320LL * distance) >> 32) >> 5;
                int r = scrX * (a + 0x10000) & 0x1FFFFFF;
                scanlinePtr->position.x = r;
                pos -= val;
                --scanlinePtr;
            }
        }
    }
    else {
        int pos = 0x40C0000;
        for (int i = 0; i < waterPos - height; ++i) {
            scanlinePtr->position.y = pos;
            int distance            = (pos >> 16) - 0x46C;
            //This is awful code, please find a way to do this with a cleaner algorithm
            int a = ((int)((int64)(0x3D70A3D74000LL * distance) >> 32) >> 5) + ((int)((int64)(0x3D70A3D74000LL * distance) >> 32) >> 31);
            int b = scrX * (a + 0x10000) & 0x1FFFFFF;
            scanlinePtr->position.x = b;
            pos += val;
            ++scanlinePtr;
        }
    }
}

void HCZSetup_HandleActTransition(void)
{
    Zone_StoreEntities((Zone->screenBoundsL1[0] + RSDK_screens->centerX) << 16, Zone->screenBoundsB1[0] << 16);
    RSDK.InitSceneLoad();
}

void HCZSetup_HandleCutscene(void)
{
    if (globals->gameMode == MODE_MANIA && (globals->playerID & 0xFF) == ID_KNUCKLES)
        RSDK.LoadScene("Cutscenes", "Mirage Saloon K Intro");
    Zone_StartFadeOut(10, 0x000000);
}

void HCZSetup_EditorDraw(void)
{

}

void HCZSetup_EditorLoad(void)
{

}

void HCZSetup_Serialize(void)
{

}

