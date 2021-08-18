#include "SonicMania.h"

ObjectUFO_Setup *UFO_Setup;

void UFO_Setup_Update(void)
{
    RSDK_THIS(UFO_Setup);

    StateMachine_Run(entity->state);

#if RETRO_USE_PLUS
    if (entity->state != UFO_Setup_Unknown12 && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM->zoneTimes[29];
#endif
}

void UFO_Setup_LateUpdate(void) {}

void UFO_Setup_StaticUpdate(void)
{
    ++UFO_Setup->timer;
    UFO_Setup->timer &= 0x7FFF;
    if (!(UFO_Setup->timer & 1)) {
        ++UFO_Setup->ringFrame;
        UFO_Setup->ringFrame &= 0xF;
    }
    if (UFO_Setup->state)
        UFO_Setup->state();
}

void UFO_Setup_Draw(void)
{
    RSDK_THIS(UFO_Setup);
    RSDK.FillScreen(entity->fadeColour, entity->timer, entity->timer - 128, entity->timer - 256);
}

void UFO_Setup_Create(void *data)
{
    RSDK_THIS(UFO_Setup);
    entity->active     = ACTIVE_NORMAL;
    entity->visible    = true;
    entity->drawOrder  = 15;
    entity->fadeColour = 0xF0F0F0;
    entity->timer      = 512;
    entity->state      = UFO_Setup_Unknown11;
}

void UFO_Setup_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_BSS_SETUP, UFO_Setup->objectID, NULL);

    UFO_Setup->playFieldLayer                                       = RSDK.GetSceneLayerID("Playfield");
    UFO_Setup->machPoints                                           = 0;
    UFO_Setup->machLevel                                            = 0;
    UFO_Setup->rings                                                = 30;
    UFO_Setup->timedOut                                             = 0;
    UFO_Setup->resetToTitle                                         = false;
    RSDK.GetSceneLayer(UFO_Setup->playFieldLayer)->scanlineCallback = UFO_Setup_ScanlineCallback_Playfield;
    RSDK.SetDrawLayerProperties(1, false, UFO_Setup_DrawLayerCallback);
    RSDK.SetDrawLayerProperties(3, false, UFO_Setup_DrawLayerCallback);
    RSDK.SetDrawLayerProperties(4, true, NULL);
    UFO_Setup->sfx_BlueSphere = RSDK.GetSFX("Special/BlueSphere2.wav");
    UFO_Setup->sfx_SSExit     = RSDK.GetSFX("Special/SSExit.wav");
    UFO_Setup->sfx_Emerald    = RSDK.GetSFX("Special/Emerald.wav");
    UFO_Setup->sfx_Event      = RSDK.GetSFX("Special/Event.wav");
    RSDK.CopyPalette(0, 0, 7, 0, 128);
    RSDK.CopyPalette(1, 96, 0, 96, 32);
    RSDK.SetLimitedFade(1, 0, 7, 36, 160, 255);
    RSDK.SetLimitedFade(2, 0, 7, 72, 160, 255);
    RSDK.SetLimitedFade(3, 0, 7, 108, 160, 255);
    RSDK.SetLimitedFade(4, 0, 7, 144, 160, 255);
    RSDK.SetLimitedFade(5, 0, 7, 180, 160, 255);
    RSDK.SetLimitedFade(6, 0, 7, 216, 160, 255);

    int floorID = RSDK.GetSceneLayerID("3D Floor");
    if (floorID != 0xFFFF) {
        TileLayer *floor3D = RSDK.GetSceneLayer(floorID);
        if (floor3D) {
            floor3D->drawLayer[0]     = 0;
            floor3D->scanlineCallback = UFO_Setup_ScanlineCallback_3DFloor;
        }
    }

    int roofID = RSDK.GetSceneLayerID("3D Roof");
    if (roofID != 0xFFFF) {
        TileLayer *roof3D = RSDK.GetSceneLayer(roofID);
        if (roof3D) {
            roof3D->drawLayer[0]     = 0;
            roof3D->scanlineCallback = UFO_Setup_ScanlineCallback_3DRoof;
        }
    }

    if (RSDK.CheckStageFolder("UFO3")) {
        UFO_Setup->state = UFO_Setup_Unknown8;

        int *deformData = RSDK.GetSceneLayer(0)->deformationData;
        for (int i = 0; i < 0x200; i += 0x10) {
            int val = RSDK.Rand(0, 4);

            int deformPos = i;
            deformPos     = minVal(0x200, deformPos);
            deformPos     = maxVal(0x00, deformPos);

            int angle = 0;
            for (int d = 0; d < 0x10; ++d) {
                deformData[deformPos + d] = val * RSDK.Sin1024(angle) >> 10;
                angle += 0x40;
            }
        }
        memcpy(&deformData[0x200], deformData, (0x200 * sizeof(int)));

        deformData = RSDK.GetSceneLayer(1)->deformationData;
        for (int i = 0; i < 0x200; i += 0x10) {
            int val = RSDK.Rand(0, 4);

            int deformPos = i;
            deformPos     = minVal(0x200, deformPos);
            deformPos     = maxVal(0x00, deformPos);

            int angle = 0;
            for (int d = 0; d < 0x10; ++d) {
                deformData[deformPos + d] = val * RSDK.Sin1024(angle) >> 10;
                angle += 0x40;
            }
        }
        memcpy(&deformData[0x200], deformData, (0x200 * sizeof(int)));
    }
    else {
        int *deformData = NULL;
        if (RSDK.CheckStageFolder("UFO4")) {
            UFO_Setup->state = UFO_Setup_Unknown9;
            TileLayer *bg    = RSDK.GetSceneLayer(0);
            deformData       = bg->deformationData;
            int angle        = 0;
            for (int i = 0; i < 0x200; ++i) {
                bg->deformationData[i] = 8 * RSDK.Sin1024(angle) >> 10;
                angle += 8;
            }
        }
        else {
            if (RSDK.CheckStageFolder("UFO5")) {
                UFO_Setup->state = UFO_Setup_Unknown10;
                TileLayer *bg    = RSDK.GetSceneLayer(0);
                deformData       = bg->deformationData;
                int angle        = 0;
                for (int i = 0; i < 0x200; ++i) {
                    bg->deformationData[i] = 8 * RSDK.Sin1024(angle) >> 10;
                    angle += 16;
                }
            }
        }

        if (deformData)
            memcpy(&deformData[0x200], deformData, (0x200 * sizeof(int)));
    }

    int listPos = RSDK_sceneInfo->listPos;
    RSDK.SetScene("Special Stage", "");
    UFO_Setup->specialStageID = listPos - RSDK_sceneInfo->listPos;
    if (UFO_Setup->specialStageID >= 7) {
        UFO_Setup->specialStageID = UFO_Setup->specialStageID % 7;
        UFO_Setup->encoreStage    = true;
    }
    RSDK_sceneInfo->listPos = listPos;
}

void UFO_Setup_DrawLayerCallback(void)
{
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
}

void UFO_Setup_ScanlineCallback_Playfield(ScanlineInfo *scanlines)
{
    EntityUFO_Camera *camera = RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);
    RSDK.SetClipBounds(0, 0, camera->clipY, RSDK_screens->width, RSDK_screens->height);
    int sin  = RSDK.Sin1024(camera->angle) >> 2;
    int cos  = RSDK.Cos1024(camera->angle) >> 2;
    int sin2 = RSDK.Sin1024(-camera->angleX) >> 2;
    int cos2 = RSDK.Cos1024(-camera->angleX) >> 2;

    int cosVal = -SCREEN_YCENTER * cos2;

    for (int i = -SCREEN_YCENTER; i < SCREEN_YCENTER; ++i) {
        int div = sin2 + (cosVal >> 8);
        if (!div)
            div = 1;
        int h               = camera->height / div;
        scanlines->deform.x = (-cos * h) >> 8;
        scanlines->deform.y = (sin * h) >> 8;

        int val             = ((cos2 * h) >> 8) - (sin2 * ((i * h) >> 8) >> 8);
        RSDK.SetActivePalette(clampVal(abs(val) >> 15, 0, 7), i + SCREEN_YCENTER, i + SCREEN_YCENTER + 1);

        int px = sin * val - RSDK_screens->centerX * scanlines->deform.x;
        int py = cos * val - RSDK_screens->centerX * scanlines->deform.y;

        scanlines->position.x = px + camera->position.x;
        scanlines->position.y = py + camera->position.y;

        scanlines++;
        cosVal += cos2;
    }
}

void UFO_Setup_ScanlineCallback_3DFloor(ScanlineInfo *scanlines)
{
    EntityUFO_Camera *camera = RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);
    RSDK.SetClipBounds(0, 0, camera->clipY + 24, RSDK_screens->width, RSDK_screens->height);
    int sin  = RSDK.Sin1024(camera->angle) >> 2;
    int cos  = RSDK.Cos1024(camera->angle) >> 2;
    int sin2 = RSDK.Sin1024(-camera->angleX) >> 2;
    int cos2 = RSDK.Cos1024(-camera->angleX) >> 2;

    int cosVal = -SCREEN_YCENTER * cos2;

    for (int i = -SCREEN_YCENTER; i < SCREEN_YCENTER; ++i) {
        int div = sin2 + (cosVal >> 8);
        if (!div)
            div = 1;
        int h               = (camera->height + 0x1000000) / div;
        scanlines->deform.x = -(cos * h) >> 8;
        scanlines->deform.y = (sin * h) >> 8;
        int val             = ((cos2 * h) >> 8) - (sin2 * ((i * h) >> 8) >> 8);
        RSDK.SetActivePalette(clampVal((abs(val) >> 15) - 8, 0, 7), i + SCREEN_YCENTER, i + SCREEN_YCENTER + 1);

        int px = sin * val - RSDK_screens->centerX * scanlines->deform.x;
        int py = cos * val - RSDK_screens->centerX * scanlines->deform.y;

        scanlines->position.x = px + camera->position.x;
        scanlines->position.y = py + camera->position.y;

        scanlines++;
        cosVal += cos2;
    }
}
void UFO_Setup_ScanlineCallback_3DRoof(ScanlineInfo *scanlines)
{
    EntityUFO_Camera *camera = RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, camera->clipY - 48);
    int sin  = RSDK.Sin1024(camera->angle) >> 2;
    int cos  = RSDK.Cos1024(camera->angle) >> 2;
    int sin2 = RSDK.Sin1024(-camera->angleX) >> 2;
    int cos2 = RSDK.Cos1024(-camera->angleX) >> 2;

    int cosVal = -SCREEN_YCENTER * cos2;
    int height = (camera->height >> 2) - 0x600000;

    for (int i = -SCREEN_YCENTER; i < SCREEN_YCENTER; ++i) {
        int div = sin2 + (cosVal >> 8);
        if (!div)
            div = 1;
        int h               = height / div;
        scanlines->deform.x = -(cos * h) >> 8;
        scanlines->deform.y = (sin * h) >> 8;
        int val             = ((cos2 * h) >> 8) - (sin2 * ((i * h) >> 8) >> 8);
        RSDK.SetActivePalette(clampVal(abs(val) >> 14, 0, 7), i + SCREEN_YCENTER, i + SCREEN_YCENTER + 1);

        int px = sin * val - RSDK_screens->centerX * scanlines->deform.x;
        int py = cos * val - RSDK_screens->centerX * scanlines->deform.y;

        scanlines->position.x = px + (camera->position.x >> 3);
        scanlines->position.y = py + (camera->position.y >> 3);

        scanlines++;
        cosVal += cos2;
    }
}

void UFO_Setup_PlaySphereSFX(void)
{
    if (UFO_Setup->spherePan) {
        int channel = RSDK.PlaySFX(UFO_Setup->sfx_BlueSphere, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        UFO_Setup->spherePan = 0;
    }
    else {
        int channel = RSDK.PlaySFX(UFO_Setup->sfx_BlueSphere, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        UFO_Setup->spherePan = 1;
    }
}
void UFO_Setup_Finish_Win(void)
{
    EntityUFO_Setup *setup = RSDK_GET_ENTITY(SLOT_UFO_SETUP, UFO_Setup);

    EntitySaveGame *saveRAM = SaveGame->saveRAM;
    saveRAM->chaosEmeralds |= 1 << UFO_Setup->specialStageID;
    if (globals->saveSlotID != NO_SAVE_SLOT)
        GameProgress_GiveEmerald(saveRAM->nextSpecialStage);

    foreach_all(UFO_Player, player)
    {
        player->stateInput  = StateMachine_None;
        player->interaction = false;
    }

    saveRAM->nextSpecialStage = (saveRAM->nextSpecialStage + 1) % 7;
    setup->visible = true;
    setup->state   = UFO_Setup_Unknown12;
    RSDK.PlaySFX(UFO_Setup->sfx_SSExit, 0, 255);
    Music_FadeOut(0.025);
    PauseMenu->disableEvents = true;
}
void UFO_Setup_Finish_Fail(void)
{
    EntityUFO_Setup *setup = RSDK_GET_ENTITY(SLOT_UFO_SETUP, UFO_Setup);

    foreach_all(UFO_Player, player)
    {
        player->stateInput  = StateMachine_None;
        player->interaction = false;
    }

    setup->visible = true;
    setup->state   = UFO_Setup_Unknown12;
    RSDK.PlaySFX(UFO_Setup->sfx_SSExit, 0, 255);
    Music_FadeOut(0.025);
    PauseMenu->disableEvents = true;
}

void UFO_Setup_Unknown8(void)
{
    if (!(UFO_Setup->timer & 1)) {
        ++RSDK.GetSceneLayer(0)->deformationOffset;
        ++RSDK.GetSceneLayer(1)->deformationOffset;
    }
}

void UFO_Setup_Unknown9(void)
{
    if (!(UFO_Setup->timer & 1)) {
        ++RSDK.GetSceneLayer(0)->deformationOffset;
    }
}

void UFO_Setup_Unknown10(void)
{
    if (!(UFO_Setup->timer & 1)) {
        ++RSDK.GetSceneLayer(0)->deformationOffset;
    }
}

void UFO_Setup_Unknown11(void)
{
    RSDK_THIS(UFO_Setup);
    if (entity->timer <= 0) {
        entity->timer               = 0;
        entity->visible             = false;
        entity->state               = UFO_Setup_Unknown13;
        RSDK_sceneInfo->timeEnabled = true;
        RSDK.CreateEntity(UFO_Message->objectID, 0, entity->position.x, entity->position.y);
    }
    else {
        entity->timer -= 0x10;
    }
}

void UFO_Setup_Unknown12(void)
{
    RSDK_THIS(UFO_Setup);
    if (entity->timer >= 1024) {
        if (UFO_Setup->resetToTitle) {
            RSDK.SetScene("Presentation", "Title Screen");
            RSDK.LoadScene();
        }
        else {
            for (int i = 0; i < LAYER_COUNT; ++i) {
                TileLayer *layer = RSDK.GetSceneLayer(i);
                if (layer)
                    layer->drawLayer[0] = DRAWLAYER_COUNT;
            }

            for (int i = 0; i < 0x800; ++i) {
                Entity *ent = RSDK.GetEntityByID(i);
                if (ent->objectID != entity->objectID)
                    RSDK.ResetEntityPtr(ent, TYPE_BLANK, 0);
            }

            RSDK.ResetEntitySlot(0, UIBackground->objectID, 0);
            RSDK.ResetEntitySlot(1, SpecialClear->objectID, 0);
            RSDK.AddDrawListRef(14, 1);
#if RETRO_USE_PLUS
            if (globals->gameMode == MODE_ENCORE) {
                UIBackground->activeColours = &UIBackground->bgColours[18];
            }
#endif
            entity->visible = false;
            entity->state   = StateMachine_None;
        }
    }
    else {
        entity->timer += 8;
    }
}

void UFO_Setup_Unknown13(void)
{
    RSDK_THIS(UFO_Setup);
    if (++entity->timer == 60) {
        entity->timer = 0;

        if (UFO_Setup->rings > 0 && RSDK_sceneInfo->timeEnabled) {
            UFO_Setup->rings--;
        }

        if (!UFO_Setup->rings && !UFO_Setup->timedOut) {
            UFO_Setup->timedOut = true;
            RSDK.CreateEntity(UFO_Message->objectID, (void *)3, entity->position.x, entity->position.y);
            entity->state = UFO_Setup_Unknown14;
        }
    }
}

void UFO_Setup_Unknown14(void)
{
    RSDK_THIS(UFO_Setup);
    if (++entity->timer >= 90)
        UFO_Setup_Finish_Fail();
}

void UFO_Setup_EditorDraw(void) {}

void UFO_Setup_EditorLoad(void) {}

void UFO_Setup_Serialize(void) {}
