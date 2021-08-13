#include "SonicMania.h"

ObjectMSZSetup *MSZSetup;

void MSZSetup_Update(void) {}

void MSZSetup_LateUpdate(void) {}

void MSZSetup_StaticUpdate(void) {}

void MSZSetup_Draw(void) {}

void MSZSetup_Create(void *data) {}

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
        // GiantPistol->field_4   = false;
        MSZSetup->chuggaVolume = 0;
        if (Zone->actID) {
            MSZSetup->flag = true;
            RSDK.CopyPalette(4, 128, 0, 128, 128);
#if RETRO_USE_PLUS
            if ((RSDK_sceneInfo->filter & FILTER_ENCORE)) {
                RSDK.CopyPalette(0, 128, 5, 128, 128);
                RSDK.LoadPalette(0, "EncoreMSZ2.act", 255);
                RSDK.CopyPalette(0, 128, 1, 128, 128);
                RSDK.CopyPalette(0, 128, 2, 128, 128);
                RSDK.RotatePalette(2, 204, 207, false);
            }
            else {
#endif
                for (int i = 0; i < 0x400; ++i) {
                    MSZSetup->bg->deformationData[i] = MSZSetup->deformData[i & 0x1F];
                }
#if RETRO_USE_PLUS
            }
#endif

            if (!(RSDK_sceneInfo->filter & FILTER_ENCORE) && (globals->playerID & ID_KNUCKLES)) {
                Zone->screenBoundsL1[0] = 0;
                Zone->screenBoundsL1[1] = 0;
                Zone->screenBoundsL1[2] = 0;
                Zone->screenBoundsL1[3] = 0;
                if (!PlayerHelpers_CheckStageReload() && PlayerHelpers_CheckPlayerPos(0x1440000, 0x4C0000, 0x1F40000, 0x2340000)) {
                    Zone->screenBoundsB1[0] = 0x234;
                    Zone->screenBoundsB1[1] = 0x234;
                    Zone->screenBoundsB1[2] = 0x234;
                    Zone->screenBoundsB1[3] = 0x234;
                }
                if (isMainGameMode() && globals->atlEnabled && !PlayerHelpers_CheckStageReload()) {
                    Zone_ReloadStoredEntities(0x2300000, 0x1200000, true);
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
            if ((RSDK_sceneInfo->filter & FILTER_ENCORE)) {
                RSDK.ResetEntitySlot(32, MSZSetup->objectID, MSZSetup_ManageFadeE);
                if isMainGameMode () {
                    if (PlayerHelpers_CheckAct1())
                        Zone->stageFinishCallback = MSZSetup_StageFinishCB_E;
                }
            }
            else {
#endif
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
                    // GiantPistol->field_4 = true;
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }

    MSZSetup->sfxLocoChugga = RSDK.GetSFX("MSZ/LocoChugga.wav");
    Animals->animalTypes[0] = ANIMAL_LOCKY;
    Animals->animalTypes[1] = ANIMAL_POCKY;
}

void MSZSetup_Unknown2(void)
{
    int id         = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int i = 0; i < bg1->scrollInfoCount; ++i) {
        MSZSetup->field_138[id++] = bg1->scrollInfo[i].parallaxFactor;
#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->filter == SCN_FILTER_MANIA)
#endif
            bg1->scrollInfo[i].parallaxFactor = 0;
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int i = 0; i < bg2->scrollInfoCount; ++i) {
        MSZSetup->field_138[id++] = bg2->scrollInfo[i].parallaxFactor;
#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->filter == SCN_FILTER_MANIA)
#endif
            bg2->scrollInfo[i].parallaxFactor = 0;
    }

    foreach_all(ParallaxSprite, parallaxSprite)
    {
        MSZSetup->field_138[id++] = (parallaxSprite->parallaxFactor.x >> 8);
#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->filter == SCN_FILTER_MANIA)
#endif
            parallaxSprite->parallaxFactor.x = 0;
    }
}

void MSZSetup_Unknown4(int parallaxMultiplier)
{
    int id         = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int i = 0; i < bg1->scrollInfoCount; ++i) {
        bg1->scrollInfo[i].scrollSpeed = parallaxMultiplier * MSZSetup->field_138[id++];
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int i = 0; i < bg2->scrollInfoCount; ++i) {
        bg2->scrollInfo[i].scrollSpeed = parallaxMultiplier * MSZSetup->field_138[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite)
    {
        parallaxSprite->scrollSpeed.x = parallaxMultiplier * MSZSetup->field_138[id++];
#if RETRO_USE_PLUS
        if (!(RSDK_sceneInfo->filter & FILTER_ENCORE))
#endif
            parallaxSprite->scrollSpeed.x >>= 8;
    }

    MSZSetup->parallaxMult = parallaxMultiplier << 8;
}

void MSZSetup_ManageFadeST(void)
{
    RSDK_THIS(MSZSetup);
    if (RSDK_screens->position.x + RSDK_screens->centerX > 0x1980) {
        entity->state = MSZSetup_SwitchPalettes;
    }
}

void MSZSetup_ManageFadeK(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x < 0x21000000 && player1->position.y < 0x6400000)
        entity->state = MSZSetup_SwitchPalettes;
}

#if RETRO_USE_PLUS
void MSZSetup_ManageFadeE(void)
{
    RSDK_THIS(MSZSetup);

    foreach_all(Decoration, decoration)
    {
        if (decoration->animator.animationID == 2 || decoration->animator.animationID == 3)
            decoration->animator.animationSpeed = 0;
    }
    entity->state = MSZSetup_Unknown9;
}
#endif

void MSZSetup_StageFinishCB_ST(void)
{
    // MSZCutsceneST->actFinishFlag = true;
}

void MSZSetup_StageFinishCB_K(void)
{
    Zone_StoreEntities((Zone->screenBoundsL1[0] + RSDK_screens->centerX) << 16, Zone->screenBoundsB1[0] << 16);
    RSDK.LoadScene();
}

#if RETRO_USE_PLUS
void MSZSetup_StageFinishCB_E(void) {}
#endif

void MSZSetup_ActivateMSZ2Cutscene(void) { MSZSetup->msz2Cutscene->active = ACTIVE_NORMAL; }

void MSZSetup_GetAchievement(void)
{
    if (!MSZSetup->hasAchievement) {
        API.UnlockAchievement("ACH_MSZ");
        MSZSetup->hasAchievement = true;
    }
}

void MSZSetup_SwitchPalettes(void)
{
    RSDK_THIS(MSZSetup);
    entity->timer += 4;
    RSDK.SetLimitedFade(0, 3, 4, entity->timer, 128, 255);
    if (entity->timer >= 256) {
#if RETRO_USE_PLUS
        if (!(RSDK_sceneInfo->filter & FILTER_ENCORE)) {
#endif
            for (int i = 0; i < 0x400; ++i) {
                MSZSetup->bg->deformationData[i] = MSZSetup->deformData[i & 0x1F];
            }
#if RETRO_USE_PLUS
        }
#endif
        MSZSetup->flag = true;
        RSDK.CopyPalette(4, 128, 0, 128, 128);
        RSDK.CopyPalette(4, 128, 1, 128, 128);
        RSDK.CopyPalette(4, 128, 2, 128, 128);
        RSDK.RotatePalette(2, 204, 207, false);
#if RETRO_USE_PLUS
        if ((RSDK_sceneInfo->filter & FILTER_ENCORE))
            entity->state = MSZSetup_Unknown10;
        else
#endif
            destroyEntity(entity);
    }
}

void MSZSetup_HandleRestart(void)
{
    Zone_ReloadStoredEntities(0x5A00000, 0xD40000, true);

    int id         = 0;
    TileLayer *bg1 = RSDK.GetSceneLayer(0);
    for (int i = 0; i < bg1->scrollInfoCount; ++i) {
        bg1->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    TileLayer *bg2 = RSDK.GetSceneLayer(1);
    for (int i = 0; i < bg2->scrollInfoCount; ++i) {
        bg2->scrollInfo[i].scrollPos = globals->parallaxOffset[id++];
    }

    foreach_all(ParallaxSprite, parallaxSprite) { parallaxSprite->parallaxFactor.x = globals->parallaxOffset[id++]; }
}

void MSZSetup_Unknown9(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > 0x29800000) {
        if (player1->position.y > 0x4200000 && player1->position.y < 0x6800000)
            entity->state = MSZSetup_SwitchPalettes;
    }
}

void MSZSetup_Unknown10(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > 0x2B600000) {
        foreach_all(Decoration, decoration)
        {
            if (decoration->animator.animationID == 2 || decoration->animator.animationID == 3)
                decoration->animator.animationSpeed = 1;
        }
        Zone->screenBoundsL1[0] = 0x2980;
        Zone->screenBoundsL2[0] = 0x29800000;
        MSZSetup->chuggaChannel = RSDK.PlaySFX(MSZSetup->sfxLocoChugga, 1, 255);
        RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, 0.0, 0.0, 1.0);
        MSZSetup_Unknown2();
        MSZSetup_Unknown4(0);
        Camera_ShakeScreen(4, 0, 4);
        RSDK.CopyTileLayer(4, 640, 94, 5, 0, 0, 62, 16);
        RSDK.CopyTileLayer(4, 655, 0, 5, 0, 0, 17, 70);
        RSDK.CopyTileLayer(3, 655, 0, 5, 0, 0, 17, 70);
        RSDK.CopyTileLayer(2, 655, 0, 5, 0, 0, 17, 70);
        entity->state = MSZSetup_Unknown11;
    }
}

void MSZSetup_Unknown11(void)
{
    RSDK_THIS(MSZSetup);
    RSDK.SetChannelAttributes(MSZSetup->chuggaChannel, ++MSZSetup->chuggaVolume * (1 / 256.0f), 0.0, 1.0);
    MSZSetup_Unknown4(8 * MSZSetup->chuggaVolume);
    if (MSZSetup->chuggaVolume >= 256)
        entity->state = MSZSetup_Unknown12;
    RSDK.GetSceneLayer(3)->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;
}

void MSZSetup_Unknown12(void)
{
    RSDK_THIS(MSZSetup);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    TileLayer *layer      = RSDK.GetSceneLayer(3);
    // HIBYTE(layer->scrollInfo[1].scrollPos)            = 0;
    layer->scrollInfo[1].scrollSpeed = 0x600 * MSZSetup->chuggaVolume;

    if (player1->position.x > 0x360C0000) {
        // GiantPistol->field_4 = true;
        // entity->state            = MSZSetup_Unknown13;
    }
}

void MSZSetup_PlayerState_Pilot(void)
{
    RSDK_THIS(Player);
    entity->position.x  = 0;
    entity->position.y  = 0;
    entity->active      = ACTIVE_NEVER;
    entity->visible     = false;
    entity->stateInput  = StateMachine_None;
    entity->position.x  = RSDK_screens->position.x << 16;
    entity->position.y  = RSDK_screens->position.y << 16;
    Player->jumpInDelay = 0;
}

void MSZSetup_EditorDraw(void) {}

void MSZSetup_EditorLoad(void) {}

void MSZSetup_Serialize(void) {}
