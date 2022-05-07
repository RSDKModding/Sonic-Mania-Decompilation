// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Setup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUFO_Setup *UFO_Setup;

void UFO_Setup_Update(void)
{
    RSDK_THIS(UFO_Setup);

    StateMachine_Run(self->state);

#if RETRO_USE_PLUS
    if (self->state != UFO_Setup_State_FinishFadeout && globals->gameMode < MODE_TIMEATTACK)
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

    StateMachine_Run(UFO_Setup->deformCB);
}

void UFO_Setup_Draw(void)
{
    RSDK_THIS(UFO_Setup);
    RSDK.FillScreen(self->fadeColor, self->timer, self->timer - 128, self->timer - 256);
}

void UFO_Setup_Create(void *data)
{
    RSDK_THIS(UFO_Setup);
    self->active     = ACTIVE_NORMAL;
    self->visible    = true;
    self->drawOrder  = 15;
    self->fadeColor = 0xF0F0F0;
    self->timer      = 512;
    self->state      = UFO_Setup_State_ShowStartMessage;
}

void UFO_Setup_StageLoad(void)
{
    RSDK.ResetEntitySlot(SLOT_BSS_SETUP, UFO_Setup->classID, NULL);

    UFO_Setup->playFieldLayer                                       = RSDK.GetTileLayerID("Playfield");
    UFO_Setup->machPoints                                           = 0;
    UFO_Setup->machLevel                                            = 0;
    UFO_Setup->rings                                                = 30;
    UFO_Setup->timedOut                                             = 0;
    UFO_Setup->resetToTitle                                         = false;
    RSDK.GetTileLayer(UFO_Setup->playFieldLayer)->scanlineCallback = UFO_Setup_ScanlineCallback_Playfield;
    RSDK.SetDrawLayerProperties(1, false, UFO_Setup_DrawLayerCallback);
    RSDK.SetDrawLayerProperties(3, false, UFO_Setup_DrawLayerCallback);
    RSDK.SetDrawLayerProperties(4, true, NULL);
    UFO_Setup->sfxBlueSphere = RSDK.GetSfx("Special/BlueSphere2.wav");
    UFO_Setup->sfxSSExit     = RSDK.GetSfx("Special/SSExit.wav");
    UFO_Setup->sfxEmerald    = RSDK.GetSfx("Special/Emerald.wav");
    UFO_Setup->sfxEvent      = RSDK.GetSfx("Special/Event.wav");
    RSDK.CopyPalette(0, 0, 7, 0, 128);
    RSDK.CopyPalette(1, 96, 0, 96, 32);
    RSDK.SetLimitedFade(1, 0, 7, 36, 160, 255);
    RSDK.SetLimitedFade(2, 0, 7, 72, 160, 255);
    RSDK.SetLimitedFade(3, 0, 7, 108, 160, 255);
    RSDK.SetLimitedFade(4, 0, 7, 144, 160, 255);
    RSDK.SetLimitedFade(5, 0, 7, 180, 160, 255);
    RSDK.SetLimitedFade(6, 0, 7, 216, 160, 255);

    uint16 floorID = RSDK.GetTileLayerID("3D Floor");
    if (floorID != (uint16)-1) {
        TileLayer *floor3D = RSDK.GetTileLayer(floorID);
        if (floor3D) {
            floor3D->drawLayer[0]     = 0;
            floor3D->scanlineCallback = UFO_Setup_ScanlineCallback_3DFloor;
        }
    }

    uint16 roofID = RSDK.GetTileLayerID("3D Roof");
    if (roofID != (uint16)-1) {
        TileLayer *roof3D = RSDK.GetTileLayer(roofID);
        if (roof3D) {
            roof3D->drawLayer[0]     = 0;
            roof3D->scanlineCallback = UFO_Setup_ScanlineCallback_3DRoof;
        }
    }

    if (RSDK.CheckStageFolder("UFO3")) {
        UFO_Setup->deformCB = UFO_Setup_DeformCB_UFO3;

        int32 *deformData = RSDK.GetTileLayer(0)->deformationData;
        for (int32 i = 0; i < 0x200; i += 0x10) {
            int32 deform = RSDK.Rand(0, 4);

            int32 deformPos = i;
            deformPos       = minVal(0x200, deformPos);
            deformPos       = maxVal(0x00, deformPos);

            int32 angle = 0;
            for (int32 d = 0; d < 0x10; ++d) {
                deformData[deformPos + d] = deform * RSDK.Sin1024(angle) >> 10;
                angle += 0x40;
            }
        }
        memcpy(&deformData[0x200], deformData, (0x200 * sizeof(int32)));

        deformData = RSDK.GetTileLayer(1)->deformationData;
        for (int32 i = 0; i < 0x200; i += 0x10) {
            int32 deform = RSDK.Rand(0, 4);

            int32 deformPos = i;
            deformPos       = minVal(0x200, deformPos);
            deformPos       = maxVal(0x00, deformPos);

            int32 angle = 0;
            for (int32 d = 0; d < 0x10; ++d) {
                deformData[deformPos + d] = deform * RSDK.Sin1024(angle) >> 10;
                angle += 0x40;
            }
        }
        memcpy(&deformData[0x200], deformData, (0x200 * sizeof(int32)));
    }
    else {
        int32 *deformData = NULL;
        if (RSDK.CheckStageFolder("UFO4")) {
            UFO_Setup->deformCB = UFO_Setup_DeformCB_UFO4;
            TileLayer *bg       = RSDK.GetTileLayer(0);
            deformData          = bg->deformationData;
            int32 angle         = 0;
            for (int32 i = 0; i < 0x200; ++i) {
                bg->deformationData[i] = 8 * RSDK.Sin1024(angle) >> 10;
                angle += 8;
            }
        }
        else {
            if (RSDK.CheckStageFolder("UFO5")) {
                UFO_Setup->deformCB = UFO_Setup_DeformCB_UFO5;
                TileLayer *bg       = RSDK.GetTileLayer(0);
                deformData          = bg->deformationData;
                int32 angle         = 0;
                for (int32 i = 0; i < 0x200; ++i) {
                    bg->deformationData[i] = 8 * RSDK.Sin1024(angle) >> 10;
                    angle += 16;
                }
            }
        }

        if (deformData)
            memcpy(&deformData[0x200], deformData, (0x200 * sizeof(int32)));
    }

    int32 listPos = SceneInfo->listPos;
    RSDK.SetScene("Special Stage", "");
    UFO_Setup->specialStageID = listPos - SceneInfo->listPos;
    if (UFO_Setup->specialStageID >= 7) {
        UFO_Setup->specialStageID = UFO_Setup->specialStageID % 7;
        UFO_Setup->encoreStage    = true;
    }
    SceneInfo->listPos = listPos;
}

void UFO_Setup_DrawLayerCallback(void)
{
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    RSDK.SetActivePalette(0, 0, ScreenInfo->height);
}

void UFO_Setup_ScanlineCallback_Playfield(ScanlineInfo *scanlines)
{
    EntityUFO_Camera *camera = RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);
    RSDK.SetClipBounds(0, 0, camera->clipY, ScreenInfo->width, ScreenInfo->height);
    int32 sin  = RSDK.Sin1024(camera->angle) >> 2;
    int32 cos  = RSDK.Cos1024(camera->angle) >> 2;
    int32 sin2 = RSDK.Sin1024(-camera->angleX) >> 2;
    int32 cos2 = RSDK.Cos1024(-camera->angleX) >> 2;

    int32 cosVal = -SCREEN_YCENTER * cos2;

    for (int32 i = -SCREEN_YCENTER; i < SCREEN_YCENTER; ++i) {
        int32 div = sin2 + (cosVal >> 8);
        if (!div)
            div = 1;
        int32 h             = camera->height / div;
        scanlines->deform.x = (-cos * h) >> 8;
        scanlines->deform.y = (sin * h) >> 8;

        int32 pos = ((cos2 * h) >> 8) - (sin2 * ((i * h) >> 8) >> 8);
        RSDK.SetActivePalette(clampVal(abs(pos) >> 15, 0, 7), i + SCREEN_YCENTER, i + SCREEN_YCENTER + 1);

        int32 px = sin * pos - ScreenInfo->centerX * scanlines->deform.x;
        int32 py = cos * pos - ScreenInfo->centerX * scanlines->deform.y;

        scanlines->position.x = px + camera->position.x;
        scanlines->position.y = py + camera->position.y;

        scanlines++;
        cosVal += cos2;
    }
}

void UFO_Setup_ScanlineCallback_3DFloor(ScanlineInfo *scanlines)
{
    EntityUFO_Camera *camera = RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);
    RSDK.SetClipBounds(0, 0, camera->clipY + 24, ScreenInfo->width, ScreenInfo->height);
    int32 sin  = RSDK.Sin1024(camera->angle) >> 2;
    int32 cos  = RSDK.Cos1024(camera->angle) >> 2;
    int32 sin2 = RSDK.Sin1024(-camera->angleX) >> 2;
    int32 cos2 = RSDK.Cos1024(-camera->angleX) >> 2;

    int32 cosVal = -SCREEN_YCENTER * cos2;

    for (int32 i = -SCREEN_YCENTER; i < SCREEN_YCENTER; ++i) {
        int32 div = sin2 + (cosVal >> 8);
        if (!div)
            div = 1;
        int32 h             = (camera->height + 0x1000000) / div;
        scanlines->deform.x = -(cos * h) >> 8;
        scanlines->deform.y = (sin * h) >> 8;

        int32 pos           = ((cos2 * h) >> 8) - (sin2 * ((i * h) >> 8) >> 8);
        RSDK.SetActivePalette(clampVal((abs(pos) >> 15) - 8, 0, 7), i + SCREEN_YCENTER, i + SCREEN_YCENTER + 1);

        int32 px = sin * pos - ScreenInfo->centerX * scanlines->deform.x;
        int32 py = cos * pos - ScreenInfo->centerX * scanlines->deform.y;

        scanlines->position.x = px + camera->position.x;
        scanlines->position.y = py + camera->position.y;

        scanlines++;
        cosVal += cos2;
    }
}
void UFO_Setup_ScanlineCallback_3DRoof(ScanlineInfo *scanlines)
{
    EntityUFO_Camera *camera = RSDK_GET_ENTITY(SLOT_UFO_CAMERA, UFO_Camera);
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, camera->clipY - 48);
    int32 sin  = RSDK.Sin1024(camera->angle) >> 2;
    int32 cos  = RSDK.Cos1024(camera->angle) >> 2;
    int32 sin2 = RSDK.Sin1024(-camera->angleX) >> 2;
    int32 cos2 = RSDK.Cos1024(-camera->angleX) >> 2;

    int32 cosVal = -SCREEN_YCENTER * cos2;
    int32 height = (camera->height >> 2) - 0x600000;

    for (int32 i = -SCREEN_YCENTER; i < SCREEN_YCENTER; ++i) {
        int32 div = sin2 + (cosVal >> 8);
        if (!div)
            div = 1;
        int32 h             = height / div;
        scanlines->deform.x = -(cos * h) >> 8;
        scanlines->deform.y = (sin * h) >> 8;
        
        int32 pos           = ((cos2 * h) >> 8) - (sin2 * ((i * h) >> 8) >> 8);
        RSDK.SetActivePalette(clampVal(abs(pos) >> 14, 0, 7), i + SCREEN_YCENTER, i + SCREEN_YCENTER + 1);

        int32 px = sin * pos - ScreenInfo->centerX * scanlines->deform.x;
        int32 py = cos * pos - ScreenInfo->centerX * scanlines->deform.y;

        scanlines->position.x = px + (camera->position.x >> 3);
        scanlines->position.y = py + (camera->position.y >> 3);

        scanlines++;
        cosVal += cos2;
    }
}

void UFO_Setup_PlaySphereSFX(void)
{
    if (UFO_Setup->spherePan) {
        int32 channel = RSDK.PlaySfx(UFO_Setup->sfxBlueSphere, false, 0xFF);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        UFO_Setup->spherePan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(UFO_Setup->sfxBlueSphere, false, 0xFF);
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
    setup->visible            = true;
    setup->state              = UFO_Setup_State_FinishFadeout;
    RSDK.PlaySfx(UFO_Setup->sfxSSExit, false, 0xFF);
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
    setup->state   = UFO_Setup_State_FinishFadeout;
    RSDK.PlaySfx(UFO_Setup->sfxSSExit, false, 0xFF);
    Music_FadeOut(0.025);
    PauseMenu->disableEvents = true;
}

void UFO_Setup_DeformCB_UFO3(void)
{
    if (!(UFO_Setup->timer & 1)) {
        ++RSDK.GetTileLayer(0)->deformationOffset;
        ++RSDK.GetTileLayer(1)->deformationOffset;
    }
}

void UFO_Setup_DeformCB_UFO4(void)
{
    if (!(UFO_Setup->timer & 1)) {
        ++RSDK.GetTileLayer(0)->deformationOffset;
    }
}

void UFO_Setup_DeformCB_UFO5(void)
{
    if (!(UFO_Setup->timer & 1)) {
        ++RSDK.GetTileLayer(0)->deformationOffset;
    }
}

void UFO_Setup_State_ShowStartMessage(void)
{
    RSDK_THIS(UFO_Setup);
    if (self->timer <= 0) {
        self->timer            = 0;
        self->visible          = false;
        self->state            = UFO_Setup_State_HandleRingDrain;
        SceneInfo->timeEnabled = true;
        CREATE_ENTITY(UFO_Message, intToVoid(UFO_MESSAGE_CATCHUFO), self->position.x, self->position.y);
    }
    else {
        self->timer -= 0x10;
    }
}

void UFO_Setup_State_FinishFadeout(void)
{
    RSDK_THIS(UFO_Setup);

    if (self->timer >= 1024) {
        if (UFO_Setup->resetToTitle) {
            RSDK.SetScene("Presentation", "Title Screen");
            RSDK.LoadScene();
        }
        else {
            for (int32 i = 0; i < LAYER_COUNT; ++i) {
                TileLayer *layer = RSDK.GetTileLayer(i);
                if (layer)
                    layer->drawLayer[0] = DRAWGROUP_COUNT;
            }

            for (int32 i = 0; i < 0x800; ++i) {
                EntityUFO_Setup *entity = RSDK_GET_ENTITY(i, UFO_Setup);
                if (entity->classID != self->classID)
                    destroyEntity(entity);
            }

            RSDK.ResetEntitySlot(0, UIBackground->classID, NULL);
            RSDK.ResetEntitySlot(1, SpecialClear->classID, NULL);
            RSDK.AddDrawListRef(14, 1);
#if RETRO_USE_PLUS
            if (globals->gameMode == MODE_ENCORE)
                UIBackground->activeColors = &UIBackground->bgColors[18];
#endif
            self->visible = false;
            self->state   = StateMachine_None;
        }
    }
    else {
        self->timer += 8;
    }
}

void UFO_Setup_State_HandleRingDrain(void)
{
    RSDK_THIS(UFO_Setup);
    if (++self->timer == 60) {
        self->timer = 0;

        if (UFO_Setup->rings > 0 && SceneInfo->timeEnabled) {
            UFO_Setup->rings--;
        }

        if (!UFO_Setup->rings && !UFO_Setup->timedOut) {
            UFO_Setup->timedOut = true;
            CREATE_ENTITY(UFO_Message, intToVoid(UFO_MESSAGE_TIMEOVER), self->position.x, self->position.y);
            self->state = UFO_Setup_State_TimedOver;
        }
    }
}

void UFO_Setup_State_TimedOver(void)
{
    RSDK_THIS(UFO_Setup);
    if (++self->timer >= 90)
        UFO_Setup_Finish_Fail();
}

#if RETRO_INCLUDE_EDITOR
void UFO_Setup_EditorDraw(void) {}

void UFO_Setup_EditorLoad(void) {}
#endif

void UFO_Setup_Serialize(void) {}
