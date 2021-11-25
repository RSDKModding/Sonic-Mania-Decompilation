#include "SonicMania.h"

ObjectMSZSetup *MSZSetup;

void MSZSetup_Update(void)
{
    RSDK_THIS(MSZSetup);
    StateMachine_Run(self->state);
}

void MSZSetup_LateUpdate(void) {}

void MSZSetup_StaticUpdate(void)
{
    if (MSZSetup->flag) {
        if (!(Zone->timer & 3))
            ++MSZSetup->bg->deformationOffset;

        MSZSetup->fadeTimer += 6;
        if (MSZSetup->fadeTimer > 255) {
            MSZSetup->fadeTimer -= 256;
            RSDK.RotatePalette(1, 204, 207, false);
            RSDK.RotatePalette(2, 204, 207, false);
        }
        RSDK.SetLimitedFade(0, 1, 2, MSZSetup->fadeTimer, 204, 207);
    }
    if (!(Zone->timer & 3))
        RSDK.RotatePalette(0, 240, 243, false);

    --MSZSetup->aniTilesTimerA;
    if (MSZSetup->aniTilesTimerA < 1) {
        ++MSZSetup->aniTilesFrameA;
        MSZSetup->aniTilesFrameA %= 13;
        MSZSetup->aniTilesTimerA = MSZSetup->aniTilesDelayA[MSZSetup->aniTilesFrameA];
        RSDK.DrawAniTiles(MSZSetup->aniTiles, 720, 16 * ((MSZSetup->aniTilesSizeA[MSZSetup->aniTilesFrameA] & 0xFFFC) + 8),
                          32 * (MSZSetup->aniTilesSizeA[MSZSetup->aniTilesFrameA] & 3), 64, 32);
    }

    --MSZSetup->aniTilesTimerB;
    if (MSZSetup->aniTilesTimerB < 1) {
        ++MSZSetup->aniTilesFrameB;
        MSZSetup->aniTilesFrameB &= 7;
        MSZSetup->aniTilesTimerB = MSZSetup->aniTilesDelayB[MSZSetup->aniTilesFrameB];
        RSDK.DrawAniTiles(MSZSetup->aniTiles, 74, 0, 16 * (MSZSetup->aniTilesFrameB + 8), 80, 16);
    }

    if (!(Zone->timer & 3)) {
        MSZSetup->aniTilesFrameC += 16;
        MSZSetup->aniTilesFrameC &= 0x7F;
        RSDK.DrawAniTiles(MSZSetup->aniTiles, 89, MSZSetup->aniTilesFrameC, 96, 16, 32);
    }

    MSZSetup->parallaxPos += MSZSetup->parallaxMult;
    globals->tempFlags = MSZSetup->flag;

    if (MSZSetup->starPostID != StarPost->postIDs[0]) {
        MSZSetup->starPostID  = StarPost->postIDs[0];
        globals->restartFlags = MSZSetup->flag;
    }
}

void MSZSetup_Draw(void) {}

void MSZSetup_Create(void *data)
{
    RSDK_THIS(MSZSetup);

    self->active = ACTIVE_NORMAL;
    if (data)
        self->state = (Type_StateMachine)data;

    if (SceneInfo->minutes || SceneInfo->seconds || SceneInfo->milliseconds) {
        if (SceneInfo->minutes == globals->tempMinutes && SceneInfo->seconds == globals->tempSeconds
            && SceneInfo->milliseconds == globals->tempMilliseconds)
            MSZSetup->flag = (RETRO_GAMEVER != VER_100 ? globals->tempFlags : globals->restartMusicID);
        else
            MSZSetup->flag = globals->restartFlags;
    }
    else {
        globals->restartFlags = 0;
#if RETRO_GAMEVER != VER_100
        globals->tempFlags    = 0;
#else
        globals->restartMusicID = 0;
#endif
        MSZSetup->flag        = false;
    }

    if (MSZSetup->flag) {
#if RETRO_USE_PLUS
        if (SceneInfo->filter & FILTER_ENCORE) {
            RSDK.LoadPalette(0, "EncoreMSZ2.act", 0xFF);
            self->state = MSZSetup_Unknown10;
        }
        else {
#endif
            for (int i = 0; i < 0x400; ++i) {
                MSZSetup->bg->deformationData[i] = MSZSetup->deformData[i & 0x1F];
            }
            RSDK.CopyPalette(4, 128, 0, 128, 128);
            destroyEntity(self);
#if RETRO_USE_PLUS
        }
        RSDK.CopyPalette(0, 128, 1, 128, 128);
        RSDK.CopyPalette(0, 128, 2, 128, 128);
        RSDK.RotatePalette(2, 204, 207, false);
#endif
    }
#if RETRO_USE_PLUS
    else if (SceneInfo->filter & FILTER_ENCORE) {
        RSDK.LoadPalette(0, "EncoreMSZ1.act", 0xFF);
        RSDK.CopyPalette(0, 128, 3, 128, 80);
        RSDK.LoadPalette(4, "EncoreMSZ2.act", 0xFF);
    }
#endif
    else {
#if !RETRO_USE_PLUS 
        RSDK.CopyPalette(0, 204, 4, 204, 4);
#endif
        RSDK.CopyPalette(3, 128, 0, 128, 128);
    }
}

void MSZSetup_StageLoad(void)
{
    MSZSetup->aniTiles = RSDK.LoadSpriteSheet("MSZ/AniTiles.gif", SCOPE_STAGE);
    MSZSetup->bg       = RSDK.GetSceneLayer(0);
    if (RSDK.CheckStageFolder("MSZCutscene")) {
        RSDK.CopyPalette(0, 204, 4, 204, 4);
        RSDK.CopyPalette(3, 128, 0, 128, 128);
        MSZSetup_Unknown2();
        MSZSetup_Unknown4(1024);
    }
    else {
#if RETRO_USE_PLUS
        GiantPistol->flag      = false;
        MSZSetup->chuggaVolume = 0;
#endif

        if (Zone->actID) {
            MSZSetup->flag = true;
            RSDK.CopyPalette(4, 128, 0, 128, 128);
#if RETRO_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                RSDK.CopyPalette(0, 128, 5, 128, 128);
                RSDK.LoadPalette(0, "EncoreMSZ2.act", 255);
                RSDK.CopyPalette(0, 128, 1, 128, 128);
                RSDK.CopyPalette(0, 128, 2, 128, 128);
                RSDK.RotatePalette(2, 204, 207, false);
            }
            else {
                for (int32 i = 0; i < 0x400; ++i) MSZSetup->bg->deformationData[i] = MSZSetup->deformData[i & 0x1F];
            }
#else
            for (int32 i = 0; i < 0x400; ++i) MSZSetup->bg->deformationData[i] = MSZSetup->deformData[i & 0x1F];
#endif

#if RETRO_USE_PLUS
            if (!(SceneInfo->filter & FILTER_ENCORE) && (globals->playerID & ID_KNUCKLES)) {
#else
            if (globals->playerID & ID_KNUCKLES) {
#endif
                Zone->cameraBoundsL[0] = 0;
                Zone->cameraBoundsL[1] = 0;
                Zone->cameraBoundsL[2] = 0;
                Zone->cameraBoundsL[3] = 0;
                if (!PlayerHelpers_CheckStageReload() && PlayerHelpers_CheckPlayerPos(0x1440000, 0x4C0000, 0x1F40000, 0x2340000)) {
                    Zone->cameraBoundsB[0] = 564;
                    Zone->cameraBoundsB[1] = 564;
                    Zone->cameraBoundsB[2] = 564;
                    Zone->cameraBoundsB[3] = 564;
                }
                if (isMainGameMode() && globals->atlEnabled && !PlayerHelpers_CheckStageReload()) {
                    Zone_ReloadStoredEntities(288 << 16, 560 << 16, true);
                }
            }
            else if (isMainGameMode() && globals->atlEnabled) {
                if (!PlayerHelpers_CheckStageReload()) {
                    MSZSetup_HandleRestart();
                }
                else {
                    globals->parallaxOffset[0] = 0;
                    globals->parallaxOffset[1] = 0;
                }
            }
            else {
                globals->parallaxOffset[0] = 0;
                globals->parallaxOffset[1] = 0;
            }

            if (isMainGameMode() && PlayerHelpers_CheckAct2()) {
                foreach_all(MSZ2Cutscene, cutscene)
                {
                    MSZSetup->msz2Cutscene = (Entity *)cutscene;
                    foreach_break;
                }
                Zone->stageFinishCallback = MSZSetup_ActivateMSZ2Cutscene;
            }
            GenericTrigger->callbacks[0] = MSZSetup_GetAchievement;
        }
        else {
#if RETRO_USE_PLUS
            if ((SceneInfo->filter & FILTER_ENCORE)) {
                RSDK.ResetEntitySlot(32, MSZSetup->objectID, MSZSetup_ManageFadeE);
                if (isMainGameMode()) {
                    if (PlayerHelpers_CheckAct1())
                        Zone->stageFinishCallback = MSZSetup_StageFinishCB_E;
                }
            }
            else {
                if (RSDK.GetEntityCount(Tornado->objectID, false) <= 0) {
                    RSDK.ResetEntitySlot(32, MSZSetup->objectID, MSZSetup_ManageFadeK);
                    if (!PlayerHelpers_CheckIntro())
                        FXFade_StopAll();
                    if (PlayerHelpers_CheckAct1Regular()) {
                        Zone->forcePlayerOnScreenFlag = true;
                        Zone->stageFinishCallback     = MSZSetup_StageFinishCB_K;
                    }
                }
                else {
                    MSZSetup_Unknown2();
                    MSZSetup_Unknown4(1024);
                    RSDK.ResetEntitySlot(32, MSZSetup->objectID, MSZSetup_ManageFadeST);
                    if (PlayerHelpers_CheckAct1Regular())
                        Zone->stageFinishCallback = MSZSetup_StageFinishCB_ST;
                    GiantPistol->flag = true;
                }
            }
#else
            if (checkPlayerID(ID_KNUCKLES, 1)) {
                RSDK.ResetEntitySlot(17, MSZSetup->objectID, MSZSetup_ManageFadeK);
                if (!PlayerHelpers_CheckIntro())
                    FXFade_StopAll();
                if (PlayerHelpers_CheckAct1Regular()) {
                    Zone->forcePlayerOnScreenFlag = true;
                    Zone->stageFinishCallback     = MSZSetup_StageFinishCB_K;
                }
            }
            else {
                MSZSetup_Unknown2();
                MSZSetup_Unknown4(0x400);
                RSDK.ResetEntitySlot(17, MSZSetup->objectID, MSZSetup_ManageFadeST);

                if (PlayerHelpers_CheckAct1Regular())
                    Zone->stageFinishCallback = MSZSetup_StageFinishCB_ST;
            }
#endif
        }
    }

#if RETRO_USE_PLUS
    MSZSetup->sfxLocoChugga = RSDK.GetSfx("MSZ/LocoChugga.wav");
#endif
    Animals->animalTypes[0] = ANIMAL_LOCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;
}

void MSZSetup_Unknown1(int x, int y)
{
    int posX = x >> 8;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    bg1->scrollPos += (y >> 8) * bg1->parallaxFactor;
    for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
        bg1->scrollInfo[i].scrollPos = posX * bg1->scrollInfo[i].parallaxFactor;
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    bg2->scrollPos += (y >> 8) * bg2->parallaxFactor;
    for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
        bg2->scrollInfo[i].scrollPos = posX * bg2->scrollInfo[i].parallaxFactor;
    }

    foreach_all(ParallaxSprite, sprite)
    {
        if (sprite->aniID < 7)
            sprite->scrollPos.x += posX * (sprite->parallaxFactor.x >> 16);
    }
}

void MSZSetup_Unknown2(void)
{
    int32 id       = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
        MSZSetup->field_138[id++] = bg1->scrollInfo[i].parallaxFactor;
#if RETRO_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_MANIA))
#endif
            bg1->scrollInfo[i].parallaxFactor = 0;
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
        MSZSetup->field_138[id++] = bg2->scrollInfo[i].parallaxFactor;
#if RETRO_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_MANIA))
#endif
            bg2->scrollInfo[i].parallaxFactor = 0;
    }

    foreach_all(ParallaxSprite, parallaxSprite)
    {
        MSZSetup->field_138[id++] = (parallaxSprite->parallaxFactor.x >> 8);
#if RETRO_USE_PLUS
        if (SceneInfo->filter == (FILTER_BOTH | FILTER_MANIA))
#endif
            parallaxSprite->parallaxFactor.x = 0;
    }
}

void MSZSetup_Unknown3(void)
{
    int32 id       = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
        bg1->scrollInfo[i].scrollSpeed = MSZSetup->field_138[id++];
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
        bg2->scrollInfo[i].scrollSpeed = MSZSetup->field_138[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->scrollSpeed.x = MSZSetup->field_138[id++] << 8; }
}

void MSZSetup_Unknown4(int32 parallaxMultiplier)
{
    int32 id       = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
        bg1->scrollInfo[i].scrollSpeed = parallaxMultiplier * MSZSetup->field_138[id++];
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
        bg2->scrollInfo[i].scrollSpeed = parallaxMultiplier * MSZSetup->field_138[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite)
    {
        parallaxSprite->scrollSpeed.x = parallaxMultiplier * MSZSetup->field_138[id++];
#if RETRO_USE_PLUS
        if (!(SceneInfo->filter & FILTER_ENCORE))
#endif
            parallaxSprite->scrollSpeed.x >>= 8;
    }

    MSZSetup->parallaxMult = parallaxMultiplier << 8;
}

void MSZSetup_ManageFadeST(void)
{
    RSDK_THIS(MSZSetup);
    if (ScreenInfo->position.x + ScreenInfo->centerX > 0x1980) {
        self->state = MSZSetup_SwitchPalettes;
    }
}

void MSZSetup_ManageFadeK(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x < 0x21000000 && player1->position.y < 0x6400000)
        self->state = MSZSetup_SwitchPalettes;
}

#if RETRO_USE_PLUS
void MSZSetup_ManageFadeE(void)
{
    RSDK_THIS(MSZSetup);

    foreach_all(Decoration, decoration)
    {
        if (decoration->animator.animationID == 2 || decoration->animator.animationID == 3)
            decoration->animator.speed = 0;
    }
    self->state = MSZSetup_Unknown9;
}
#endif

void MSZSetup_StageFinishCB_ST(void) { MSZCutsceneST->actFinishFlag = true; }

void MSZSetup_StageFinishCB_K(void)
{
    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->centerX) << 16, Zone->cameraBoundsB[0] << 16);
    RSDK.LoadScene();
}

#if RETRO_USE_PLUS
void MSZSetup_StageFinishCB_E(void) {}
#endif

void MSZSetup_ActivateMSZ2Cutscene(void) { MSZSetup->msz2Cutscene->active = ACTIVE_NORMAL; }

void MSZSetup_GetAchievement(void)
{
    if (!MSZSetup->hasAchievement) {
        API_UnlockAchievement("ACH_MSZ");
        MSZSetup->hasAchievement = true;
    }
}

void MSZSetup_SwitchPalettes(void)
{
    RSDK_THIS(MSZSetup);
    self->timer += 4;
    RSDK.SetLimitedFade(0, 3, 4, self->timer, 128, 255);
    if (self->timer >= 256) {
#if RETRO_USE_PLUS
        if (!(SceneInfo->filter & FILTER_ENCORE)) {
#endif
            for (int32 i = 0; i < 0x400; ++i) {
                MSZSetup->bg->deformationData[i] = MSZSetup->deformData[i & 0x1F];
            }
#if RETRO_USE_PLUS
        }
#endif
        MSZSetup->flag = true;
        RSDK.CopyPalette(4, 128, 0, 128, 128);
#if RETRO_USE_PLUS
        RSDK.CopyPalette(4, 128, 1, 128, 128);
        RSDK.CopyPalette(4, 128, 2, 128, 128);
        RSDK.RotatePalette(2, 204, 207, false);
        if ((SceneInfo->filter & FILTER_ENCORE))
            self->state = MSZSetup_Unknown10;
        else
#endif
            destroyEntity(self);
    }
}

void MSZSetup_HandleRestart(void)
{
    Zone_ReloadStoredEntities(WIDE_SCR_XCENTER << 16, 1440 << 16, true);

    int32 id       = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int32 i = 0; i < bg1->scrollInfoCount; ++i) {
        bg1->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int32 i = 0; i < bg2->scrollInfoCount; ++i) {
        bg2->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->scrollPos.x = globals->parallaxOffset[id++]; }
}

void MSZSetup_Unknown9(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > 0x29800000) {
        if (player1->position.y > 0x4200000 && player1->position.y < 0x6800000)
            self->state = MSZSetup_SwitchPalettes;
    }
}

#if RETRO_USE_PLUS
void MSZSetup_Unknown10(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > 0x2B600000) {
        foreach_all(Decoration, decoration)
        {
            if (decoration->animator.animationID == 2 || decoration->animator.animationID == 3)
                decoration->animator.speed = 1;
        }
        Zone->cameraBoundsL[0] = 0x2980;
        Zone->playerBoundsL[0] = 0x29800000;
        MSZSetup->chuggaChannel = RSDK.PlaySfx(MSZSetup->sfxLocoChugga, 1, 255);
        RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, 0.0, 0.0, 1.0);
        MSZSetup_Unknown2();
        MSZSetup_Unknown4(0);
        Camera_ShakeScreen(0, 4, 4);
        RSDK.CopyTileLayer(4, 640, 94, 5, 0, 0, 62, 16);
        RSDK.CopyTileLayer(4, 655, 0, 5, 0, 0, 17, 70);
        RSDK.CopyTileLayer(3, 655, 0, 5, 0, 0, 17, 70);
        RSDK.CopyTileLayer(2, 655, 0, 5, 0, 0, 17, 70);
        self->state = MSZSetup_Unknown11;
    }
}

void MSZSetup_Unknown11(void)
{
    RSDK_THIS(MSZSetup);
    RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, ++MSZSetup->chuggaVolume * (1 / 256.0f), 0.0, 1.0);
    MSZSetup_Unknown4(8 * MSZSetup->chuggaVolume);
    if (MSZSetup->chuggaVolume >= 256)
        self->state = MSZSetup_Unknown12;
    RSDK.GetSceneLayer(3)->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;
}

void MSZSetup_Unknown12(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TileLayer *layer      = RSDK.GetSceneLayer(3);
    // HIBYTE(layer->scrollInfo[1].scrollPos)            = 0;
    layer->scrollInfo[1].scrollPos &= 0x00FFFFFF;
    layer->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;

    if (player1->position.x > 0x360C0000) {
        GiantPistol->flag = true;
        self->state     = MSZSetup_Unknown13;
    }
}

void MSZSetup_Unknown13(void)
{
    RSDK_THIS(MSZSetup);
    TileLayer *layer = RSDK.GetSceneLayer(3);
    // HIBYTE(layer->scrollInfo[1].scrollPos)            = 0;
    layer->scrollInfo[1].scrollPos &= 0x00FFFFFF;
    layer->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;

    if (!MSZSetup->parallaxMult) {
        MSZSetup->chuggaVolume -= 4;
        RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, MSZSetup->chuggaVolume * 0.00390625, 0.0, 1.0);
        if (MSZSetup->chuggaVolume <= 0) {
            layer->scrollInfo[1].scrollSpeed = 0;
            layer->scrollInfo[1].scrollPos   = 0;
            MSZSetup->chuggaVolume           = 0;
            self->state                    = MSZSetup_Unknown14;
        }
    }
}

void MSZSetup_Unknown14(void)
{
    RSDK_THIS(MSZSetup);
    if (RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear)->objectID == ActClear->objectID)
        self->state = MSZSetup_Unknown16;
}

void MSZSetup_Unknown16(void)
{
    RSDK_THIS(MSZSetup);

    if (RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear)->objectID != ActClear->objectID) {
        self->timer               = 0;
        Zone->cameraBoundsR[0]     = 17064;
        Zone->playerBoundActiveR[0] = false;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        player1->stateInput   = StateMachine_None;

        foreach_active(Player, player)
        {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, true, 0);
            player->up        = false;
            player->down      = false;
            player->left      = false;
            player->right     = false;
            player->jumpPress = false;
            player->jumpHold  = false;
            player->state     = Player_State_Ground;
        }
        self->state = MSZSetup_Unknown17;
    }
}

void MSZSetup_Unknown17(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->right        = true;

    if (player1->position.x < (Zone->cameraBoundsR[0] - ScreenInfo->centerX) << 16) {
        foreach_active(Player, player)
        {
            if (player->groundVel > 0x30000)
                player->groundVel = 0x30000;
        }
    }
    else {
        player1->right = false;
        player1->left  = true;
        self->state  = MSZSetup_Unknown18;
    }
}

void MSZSetup_Unknown18(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->groundVel <= 0) {
        player1->groundVel = 0;
        player1->left      = false;
        player1->direction = FLIP_NONE;
        self->timer      = 0;
        self->state      = MSZSetup_StoreMSZ1EScrollPos;
    }
}

void MSZSetup_StoreMSZ1EScrollPos(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    player1->direction    = FLIP_NONE;

    if (++self->timer >= 90) {
        int id = 0;

        TileLayer *bg1 = RSDK.GetSceneLayer(0);
        for (int i = 0; i < bg1->scrollInfoCount; ++i) {
            globals->parallaxOffset[id++] = bg1->scrollInfo[i].tilePos;
        }

        TileLayer *bg2 = RSDK.GetSceneLayer(0);
        for (int i = 0; i < bg2->scrollInfoCount; ++i) {
            globals->parallaxOffset[id++] = bg2->scrollInfo[i].tilePos;
        }

        foreach_all(ParallaxSprite, sprite)
        {
            globals->parallaxOffset[id++] =
                ((sprite->scrollPos.x + sprite->parallaxFactor.x * ScreenInfo->position.x) & 0x7FFF0000) % sprite->loopPoint.x;
        }

        Zone_StoreEntities((ScreenInfo->centerX + 16640) << 16, 1440 << 16);
        globals->atlEnabled = true;
        RSDK.LoadScene();
        destroyEntity(self);
    }
}
#endif

void MSZSetup_StoreMSZ1STScrollPos(void)
{
    int id = 0;

    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int i = 0; i < bg1->scrollInfoCount; ++i) {
        globals->parallaxOffset[id++] = bg1->scrollInfo[i].scrollPos;
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int i = 0; i < bg2->scrollInfoCount; ++i) {
        globals->parallaxOffset[id++] = bg2->scrollInfo[i].scrollPos;
    }

    foreach_all(ParallaxSprite, sprite) { globals->parallaxOffset[id++] = sprite->scrollPos.x; }

    Zone_StoreEntities((Zone->cameraBoundsL[0] + ScreenInfo->centerX) << 16, 1440 << 16);
    ++SceneInfo->listPos;
    globals->atlEnabled = true;
    RSDK.LoadScene();
}

void MSZSetup_Player_State_Pilot(void)
{
    RSDK_THIS(Player);
    self->position.x  = 0;
    self->position.y  = 0;
    self->active      = ACTIVE_NEVER;
    self->visible     = false;
    self->stateInput  = StateMachine_None;
    self->position.x  = ScreenInfo->position.x << 16;
    self->position.y  = ScreenInfo->position.y << 16;
    Player->jumpInDelay = 0;
}

void MSZSetup_Player_State_PostCrashJumpIn(void)
{
    RSDK_THIS(Player);
    self->active      = ACTIVE_NORMAL;
    self->visible     = true;
    self->stateInput  = Player_ProcessP2Input_AI;
    Player->jumpInDelay = 240;
    Player_P2JumpBackIn();
}

#if RETRO_INCLUDE_EDITOR
void MSZSetup_EditorDraw(void) {}

void MSZSetup_EditorLoad(void) {}
#endif

void MSZSetup_Serialize(void) {}
