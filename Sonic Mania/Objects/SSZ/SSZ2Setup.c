#include "SonicMania.h"

ObjectSSZ2Setup *SSZ2Setup;

void SSZ2Setup_Update(void) {}

void SSZ2Setup_LateUpdate(void) {}

void SSZ2Setup_StaticUpdate(void)
{
    if (SSZ2Setup->sparkTimer <= 0) {
        foreach_active(Player, player)
        {
            Hitbox *hitbox = Player_GetHitbox(player);
            uint16 tile    = RSDK.GetTileInfo(Zone->fgLow, player->position.x >> 20, ((hitbox->bottom << 16) + player->position.y - 0x10000) >> 20);
            if (tile == 0xFFFF)
                tile = RSDK.GetTileInfo(Zone->fgLow, player->position.x >> 20, ((hitbox->bottom << 16) + player->position.y - 0x10000) >> 20);

            if (RSDK.GetTileBehaviour(tile, player->collisionPlane) && (abs(player->groundVel) > 0x80000 && player->onGround)) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_Move, player->position.x, player->position.y + (hitbox->bottom << 16));
                RSDK.SetSpriteAnimation(SparkRail->aniFrames, 0, &debris->animator, true, 0);
                debris->drawFX     = FX_FLIP;
                debris->direction  = player->velocity.x < 0;
                debris->drawOrder  = Zone->drawOrderLow;
                debris->position.x = player->position.x;
                debris->position.y = player->position.y + (hitbox->bottom << 16);
                debris->timer      = 30;
                RSDK.PlaySfx(SSZ2Setup->sfxSpark, false, 255);
                SSZ2Setup->sparkTimer = 3;
            }
        }
    }
    else {
        SSZ2Setup->sparkTimer--;
    }
}

void SSZ2Setup_Draw(void) {}

void SSZ2Setup_Create(void *data) {}

void SSZ2Setup_StageLoad(void)
{
    SSZ2Setup->sfxSpark     = RSDK.GetSfx("SSZ2/Spark.wav");
    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_PICKY;
    SSZ2Setup->towerID      = RSDK.GetSceneLayerID("Tower");
    if (SSZ2Setup->towerID < 8) {
        SSZ2Setup->towerLayer                   = RSDK.GetSceneLayer(SSZ2Setup->towerID);
        SSZ2Setup->towerLayer->scanlineCallback = SSZ2Setup_TowerScanlineCallback;
        RSDK.SetDrawLayerProperties(1, false, SSZ2Setup_TowerDrawLayerCallback);
        RSDK.SetLimitedFade(3, 0, 4, 96, 0, 256);
        if (globals->suppressTitlecard >= true) {
            SaveGame_LoadPlayerState();
            Zone_StartFadeIn(10, 0x000000);
        }
        CREATE_ENTITY(SSZ3Cutscene, intToVoid(false), 0, 0);
#if RETRO_USE_PLUS
        Zone->stageFinishCallback = SSZ2Setup_StageFinishCallback;
#endif
    }
#if RETRO_USE_PLUS
    if ((SceneInfo->filter & FILTER_ENCORE))
        RSDK.LoadPalette(0, "EncoreSSZ2.act", 0b0000000011111111);
#endif
    GenericTrigger->callbacks[0] = SSZ2Setup_GenericTriggerCallback1;
    GenericTrigger->callbacks[1] = SSZ2Setup_GenericTriggerCallback2;
    GenericTrigger->callbacks[2] = SSZ2Setup_GenericTriggerCallback3;
}

#if RETRO_USE_PLUS
void SSZ2Setup_StageFinishCallback(void) { CREATE_ENTITY(SSZ3Cutscene, intToVoid(true), 0, 0); }
#endif

void SSZ2Setup_TowerDrawLayerCallback(void)
{
    RSDK.SetActivePalette(0, 0, ScreenInfo->height);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
}

void SSZ2Setup_TowerScanlineCallback(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, ScreenInfo->centerX - 144, 0, ScreenInfo->centerX + 144, ScreenInfo->height);
    RSDK.ProcessParallax(SSZ2Setup->towerLayer);
    RSDK.SetActivePalette(3, 0, ScreenInfo->height);

    ScanlineInfo *scanlinePtr = &scanlines[ScreenInfo->centerX - 64];
    int32 x1                    = scanlinePtr->position.x;
    int32 offset                = 0x10000;
    for (int32 i = 2; i - 2 < 80;) {
        scanlinePtr -= 5;
        scanlinePtr[5].position.x = x1 & 0x1FFFFFF;

        int32 x2                    = x1 - offset;
        offset                    = (i - 2) * (i - 2) + offset;
        scanlinePtr[4].position.x = x2 & 0x1FFFFFF;

        int32 x3                    = x2 - offset;
        offset                    = (i - 1) * (i - 1) + offset;
        scanlinePtr[3].position.x = x3 & 0x1FFFFFF;

        int32 x4                    = x3 - offset;
        offset                    = i * i + offset;
        scanlinePtr[2].position.x = x4 & 0x1FFFFFF;

        int32 x5                    = x4 - offset;
        offset                    = (i + 1) * (i + 1) + offset;
        scanlinePtr[1].position.x = x5 & 0x1FFFFFF;

        x1     = x5 - offset;
        offset = (i + 2) * (i + 2) + offset;

        i += 5;
    }

    scanlinePtr = &scanlines[ScreenInfo->centerX + 64];
    x1          = scanlinePtr->position.x;
    offset      = 0x10000;

    for (int32 i = 2; i - 2 < 80;) {
        scanlinePtr += 5;
        scanlinePtr[-5].position.x = x1 & 0x1FFFFFF;

        int32 x2                     = x1 + offset;
        offset                     = (i - 2) * (i - 2) + offset;
        scanlinePtr[-4].position.x = x2 & 0x1FFFFFF;

        int32 x3                     = x2 + offset;
        offset                     = (i - 1) * (i - 1) + offset;
        scanlinePtr[-3].position.x = x3 & 0x1FFFFFF;

        int32 x4                     = x3 + offset;
        offset                     = i * i + offset;
        scanlinePtr[-2].position.x = x4 & 0x1FFFFFF;

        int32 x5                     = x4 + offset;
        offset                     = (i + 1) * (i + 1) + offset;
        scanlinePtr[-1].position.x = x5 & 0x1FFFFFF;

        x1     = x5 + offset;
        offset = (i + 2) * (i + 2) + offset;

        i += 5;
    }
}

void SSZ2Setup_GenericTriggerCallback1(void)
{
    foreach_active(HotaruMKII, hotaru)
    {
        if (!hotaru->type) {
#if RETRO_USE_PLUS
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.RandSeeded(0, 32, &Zone->randSeed) >> 4] + 1), hotaru->position.x,
                          hotaru->position.y);
#else
            CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), hotaru->position.x, hotaru->position.y);
#endif
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), hotaru->position.x, hotaru->position.y)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
            destroyEntity(hotaru);
        }
    }
}

void SSZ2Setup_GenericTriggerCallback2(void)
{
    if (!SSZ2Setup->hasAchievement) {
        if (!SceneInfo->minutes) {
            API_UnlockAchievement("ACH_SSZ");
            SSZ2Setup->hasAchievement = true;
        }
    }
}

void SSZ2Setup_GenericTriggerCallback3(void)
{
    RSDK_THIS(GenericTrigger);

    if (isMainGameMode()) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player->stateInput) {
            player->stateInput      = 0;
            player->left            = false;
            player->right           = true;
            Zone->cameraBoundsR[0] = ScreenInfo->centerX + (self->position.x >> 16);
            Zone->cameraBoundsR[1] = ScreenInfo->centerX + (self->position.x >> 16);
#if RETRO_USE_PLUS
            Zone->cameraBoundsR[2] = ScreenInfo->centerX + (self->position.x >> 16);
            Zone->cameraBoundsR[3] = ScreenInfo->centerX + (self->position.x >> 16);
#endif

            for (int32 i = 0; i < Player->playerCount; ++i) {
                StarPost->postIDs[i] = 0;
            }

            SaveGame_SavePlayerState();
            globals->suppressAutoMusic = true;
            globals->suppressTitlecard = true;
            ++SceneInfo->listPos;
            if (!RSDK.CheckValidScene())
                RSDK.SetScene("Presentation", "Title Screen");
            Zone_StartFadeOut(10, 0x000000);
        }
        if (player->superState == SUPERSTATE_SUPER || player->state == Player_State_Transform)
            globals->restartPowerups |= 0x80;
        globals->restartMusicID = Music->activeTrack;
    }
}

void SSZ2Setup_EditorDraw(void) {}

void SSZ2Setup_EditorLoad(void) {}

void SSZ2Setup_Serialize(void) {}
