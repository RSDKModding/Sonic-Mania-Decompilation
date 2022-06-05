// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HCZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHCZSetup *HCZSetup;

void HCZSetup_Update(void) {}

void HCZSetup_LateUpdate(void) {}

void HCZSetup_StaticUpdate(void)
{
    if (!(Zone->timer & 3)) {
        ++HCZSetup->background2Layer->deformationOffsetW;

        // Handle Animating the BG candle & waterline tiles
        if (!(Zone->timer & 3)) {
            HCZSetup->bgCandlesAniTileFrame += 0x10;
            HCZSetup->bgCandlesAniTileFrame &= 0x3F;
            RSDK.DrawAniTiles(HCZSetup->bgCandlesAniTiles, 386, 0, HCZSetup->bgCandlesAniTileFrame, 64, 16);

            HCZSetup->waterlineAniTileFrame += 0x20;
            HCZSetup->waterlineAniTileFrame &= 0x1FF;
            RSDK.DrawAniTiles(HCZSetup->waterlineAniTiles, 820, HCZSetup->waterlineAniTileFrame, 0, 32, 96);
            RSDK.DrawAniTiles(HCZSetup->waterlineAniTiles, 857, HCZSetup->waterlineAniTileFrame, 96, 32, 128);
            RSDK.RotatePalette(0, 176, 182, true);
        }
    }

    // Handle Animating the pendulum tiles
    int32 *durations = HCZSetup->pendulumAniTileDurations;
    int32 duration1  = 0;
    int32 duration2  = 0;

    for (int32 i = 0; i < 7; ++i) {
        duration1 += durations[0];
        duration2 += durations[1];
        durations += 2;
    }

    int32 pendulumTimer        = Zone->timer % (duration2 + duration1);
    int32 pendulumAniTileFrame = 0;

    while (pendulumTimer >= HCZSetup->pendulumAniTileDurations[pendulumAniTileFrame]) {
        pendulumTimer -= HCZSetup->pendulumAniTileDurations[pendulumAniTileFrame++];
    }

    if (!pendulumTimer)
        RSDK.DrawAniTiles(HCZSetup->pendulumAniTiles, 874, 0, 16 * pendulumAniTileFrame, 32, 16);

    if (!(Zone->timer & 1)) {
        for (int32 layerID = Zone->fgLow; layerID <= Zone->fgHigh; ++layerID) RSDK.GetTileLayer(layerID)->deformationOffsetW++;
    }

#if MANIA_USE_PLUS
    if (HCZSetup->waterslidingPlayerCount) {
        if (HCZSetup->playingWaterfallLoop) {
            if (!(HCZSetup->waterfallLoopTimer & 0x1F)) {
                RSDK.StopSfx(HCZSetup->sfxWaterfall);
                RSDK.PlaySfx(HCZSetup->sfxWaterfallLoop, false, 0xFF);
            }
        }
        else {
            HCZSetup->waterfallLoopTimer   = 0;
            HCZSetup->playingWaterfallLoop = true;
            RSDK.PlaySfx(HCZSetup->sfxWaterfall, false, 0xFF);
        }
        ++HCZSetup->waterfallLoopTimer;
    }
    else {
        HCZSetup->playingWaterfallLoop = false;
    }

    HCZSetup->waterslidingPlayerCount = 0;
    foreach_active(Player, player)
    {
        RSDK.GetEntitySlot(player);
        if (player->state != Player_State_None) {
            Hitbox *hitbox = Player_GetHitbox(player);
            uint16 tile =
                RSDK.GetTileInfo(Zone->fgHigh, player->position.x >> 20, ((hitbox->bottom << 16) + player->position.y - 0x10000) >> 20) & 0x3FF;

            if (((tile >= 226 && tile <= 224) || (tile >= 880 && tile <= 888)) && player->collisionPlane == 1) {
                if (player->state != Player_State_BubbleBounce && player->state != Player_State_MightyHammerDrop) {
                    if (player->onGround) {
                        if (player->state != Player_State_WaterSlide) {
                            player->interaction    = true;
                            player->tileCollisions = true;
                            player->position.y += player->jumpOffset;
                            player->nextGroundState = StateMachine_None;
                            player->state           = Player_State_WaterSlide;
                        }

                        if (player->onGround)
                            ++HCZSetup->waterslidingPlayerCount;
                    }
                    else if (player->animator.animationID == ANI_FLUME)
                        ++HCZSetup->waterslidingPlayerCount;
                }
            }
        }
    }
#endif
}

void HCZSetup_Draw(void) {}

void HCZSetup_Create(void *data) {}

void HCZSetup_StageLoad(void)
{
    HCZSetup->bgCandlesAniTiles = RSDK.LoadSpriteSheet("HCZ/AniTiles.gif", SCOPE_STAGE);
    HCZSetup->waterlineAniTiles = RSDK.LoadSpriteSheet("HCZ/AniTiles2.gif", SCOPE_STAGE);
    HCZSetup->pendulumAniTiles  = RSDK.LoadSpriteSheet("HCZ/AniTiles3.gif", SCOPE_STAGE);

    HCZSetup->background2Layer = RSDK.GetTileLayer(1);
    // Add some deformations to background 2
    for (int32 i = 0; i < 0x200; i += 0x10) {
        int32 deformation = RSDK.Rand(0, 4);

        int32 *deformDataW = &HCZSetup->background2Layer->deformationDataW[clampVal(i, 0, 0x200)];

        int32 angle = 0;
        for (int32 d = 0; d < 0x10; ++d) {
            *deformDataW = deformation * RSDK.Sin1024(angle) >> 10;
            angle += 0x40;
        }

        memcpy(&HCZSetup->background2Layer->deformationDataW[0x200], &HCZSetup->background2Layer->deformationDataW[0], 0x200 * sizeof(int32));
    }

    // if Act 2, Setup the waterline effect
    if (Zone->actID == 1)
        HCZSetup->background2Layer->scanlineCallback = HCZSetup_BGWaterLineScanlineCB;

    // All Layers between FG Low & FG High get foreground water deformation applied
    for (int32 layerID = Zone->fgLow; layerID <= Zone->fgHigh; ++layerID) {
        TileLayer *layer   = RSDK.GetTileLayer(layerID);
        int32 *deformDataW = layer->deformationDataW;

        // HCZ FG underwater deformation values
        for (int32 i = 0; i < 4; ++i) {
            deformDataW[0]  = 1;
            deformDataW[1]  = 1;
            deformDataW[2]  = 2;
            deformDataW[3]  = 2;
            deformDataW[4]  = 3;
            deformDataW[5]  = 3;
            deformDataW[6]  = 3;
            deformDataW[7]  = 3;
            deformDataW[8]  = 2;
            deformDataW[9]  = 2;
            deformDataW[10] = 1;
            deformDataW[11] = 1;

            deformDataW[128] = 1;
            deformDataW[129] = 1;
            deformDataW[130] = 2;
            deformDataW[131] = 2;
            deformDataW[132] = 3;
            deformDataW[133] = 3;
            deformDataW[134] = 3;
            deformDataW[135] = 3;
            deformDataW[136] = 2;
            deformDataW[137] = 2;
            deformDataW[138] = 1;
            deformDataW[139] = 1;

            deformDataW[160] = -1;
            deformDataW[161] = -1;
            deformDataW[162] = -2;
            deformDataW[163] = -2;
            deformDataW[164] = -3;
            deformDataW[165] = -3;
            deformDataW[166] = -3;
            deformDataW[167] = -3;
            deformDataW[168] = -2;
            deformDataW[169] = -2;
            deformDataW[170] = -1;
            deformDataW[171] = -1;

            deformDataW += 0x100;
        }
    }

    Animals->animalTypes[0] = ANIMAL_POCKY;
    Animals->animalTypes[1] = ANIMAL_ROCKY;

    RSDK.SetDrawLayerProperties(0, false, Water_SetWaterLevel);
    RSDK.SetDrawLayerProperties(Zone->hudDrawOrder, false, Water_RemoveWaterEffect);

    Water->waterPalette = 1;

    if (Zone->actID) {
        if (!PlayerHelpers_CheckStageReload())
            PlayerHelpers_CheckPlayerPos(1424 << 16, 176 << 16, 608 << 16, 1664 << 16);

        Zone->cameraBoundsL[0] = 168;
        Zone->cameraBoundsL[1] = 168;
        Zone->cameraBoundsL[2] = 168;
        Zone->cameraBoundsL[3] = 168;

        if (isMainGameMode() && globals->atlEnabled && !PlayerHelpers_CheckStageReload())
            Zone_ReloadStoredEntities(388 << 16, 1696 << 16, true);

        Zone->stageFinishCallback = HCZSetup_StageFinishCB_Act2;
    }
    else if (isMainGameMode() && PlayerHelpers_CheckAct1()) {
        Zone->forcePlayerOnScreen = true;
        Zone->stageFinishCallback = HCZSetup_HandleActTransition;
    }

#if MANIA_USE_PLUS
    if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreHCZ.act", 0b0000000011111111);
        RSDK.LoadPalette(1, "EncoreHCZw.act", 0b0000000011111111);
    }

    HCZSetup->sfxWaterfall     = RSDK.GetSfx("Stage/Waterfall.wav");
    HCZSetup->sfxWaterfallLoop = RSDK.GetSfx("Stage/Waterfall2.wav");
#endif
}

void HCZSetup_BGWaterLineScanlineCB(ScanlineInfo *scanlines)
{
    RSDK.ProcessParallax(HCZSetup->background2Layer);

    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    int32 screenY          = 0x210 - (screen->position.y >> 2);
    int32 waterLevel       = (Water->waterLevel >> 0x10) - screen->position.y;

    int32 distance       = maxVal(1, abs(screenY - waterLevel));
    int32 scanlineHeight = maxVal(0x10000, 0x640000 / distance);

    screenY    = clampVal(screenY, 0, screen->height);
    waterLevel = clampVal(waterLevel, 0, screen->height);

    ScanlineInfo *scanlinePtr = &scanlines[screenY];

    // scanlinePtr->position.x handles the horizontal scroll that gives water the parallax effect
    // scanlinePtr->position.y handles the cool "water line" effect

    // if above OR below, if it's equal, the water covers it all anyways
    if (screenY > waterLevel) {       // below the waterline
        int32 scanlineY = 1232 << 16; // bottom most tile of the water tiles
        for (int32 i = 0; i < screenY - waterLevel; ++i) {
            // 852 = (top most tile of the below water tiles)
            scanlinePtr->position.x = ((((852 << 16) - 0xC000 * (scanlineY >> 16)) / 100 + 0x10000) * screen->position.x) & 0x1FFFFFF;
            scanlinePtr->position.y = scanlineY;
            scanlineY -= scanlineHeight;
            --scanlinePtr;
        }
    }
    else if (screenY < waterLevel) {  // above the waterline
        int32 scanlineY = 1036 << 16; // top most tile of the water tiles
        for (int32 i = 0; i < waterLevel - screenY; ++i) {
            // 1132 = bottom most tile of the above water tiles
            int32 distance          = 1132 - (scanlineY >> 16);
            scanlinePtr->position.x = (((-0xC000 * distance) / 100 + 0x10000) * screen->position.x) & 0x1FFFFFF;
            scanlinePtr->position.y = scanlineY;
            scanlineY += scanlineHeight;
            ++scanlinePtr;
        }
    }
}

void HCZSetup_HandleActTransition(void)
{
    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->centerX) << 16, Zone->cameraBoundsB[0] << 16);
    RSDK.LoadScene();
}

void HCZSetup_StageFinishCB_Act2(void)
{
    if (globals->gameMode == MODE_MANIA && CHECK_CHARACTER_ID(ID_KNUCKLES, 1))
        RSDK.SetScene("Cutscenes", "Mirage Saloon K Intro");

    Zone_StartFadeOut(10, 0x000000);
}

#if RETRO_INCLUDE_EDITOR
void HCZSetup_EditorDraw(void) {}

void HCZSetup_EditorLoad(void) {}
#endif

void HCZSetup_Serialize(void) {}
