#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectPBL_Setup *PBL_Setup;

void PBL_Setup_Update(void)
{
    RSDK_THIS(PBL_Setup);
    StateMachine_Run(entity->state);
    if (entity->state != PBL_Setup_SaveAndChangeScene && globals->gameMode < MODE_TIMEATTACK)
        ++SaveGame->saveRAM[62];
}

void PBL_Setup_LateUpdate(void) {}

void PBL_Setup_StaticUpdate(void)
{
    Entity *entity = &RSDK_sceneInfo->entity;
    ++PBL_Setup->timer;
    PBL_Setup->timer &= 0x7FFF;

    if (!(PBL_Setup->timer & 1)) {
        ++PBL_Setup->timer2;
        PBL_Setup->timer2 &= 0xF;
    }

    if (!(PBL_Setup->timer & 0xF)) {
        for (int i = 0; i < 8; ++i) {
            RSDK.RotatePalette(i, 176, 181, true);
            RSDK.RotatePalette(i, 176, 181, true);
        }
    }

    PBL_Setup->timer3 += 0x8000;
    PBL_Setup->timer3 &= 0x7FFFFFFF;
    if ((RSDK_controller->keyStart.press || RSDK_unknown->field_10 == 1) && RSDK_sceneInfo->state == ENGINESTATE_REGULAR
        && !RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID) {
        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
        pauseMenu->disableRestart  = true;
        pauseMenu->triggerPlayer   = RSDK.GetEntityID(entity);
    }
}

void PBL_Setup_Draw(void)
{
    RSDK_THIS(PBL_Setup);
    RSDK.FillScreen(entity->colour, entity->timer, entity->timer - 128, entity->timer - 256);
}

void PBL_Setup_Create(void *data)
{
    RSDK_THIS(PBL_Setup);
    entity->active    = ACTIVE_NORMAL;
    entity->visible   = true;
    entity->drawOrder = DRAWLAYER_COUNT - 1;
    entity->colour    = 0xF0F0F0;
    entity->timer     = 512;
    entity->state     = PBL_Setup_Unknown9;
}

void PBL_Setup_StageLoad(void)
{
    PBL_Setup->tableLow                                        = RSDK.GetSceneLayerID("Table Low");
    PBL_Setup->tableHigh                                       = RSDK.GetSceneLayerID("Table High");
    PBL_Setup->rings                                           = 0;
    PBL_Setup->dword20                                         = 0;
    PBL_Setup->dword24                                         = 0;
    PBL_Setup->score                                           = 0;
    PBL_Setup->score1UP                                        = 10000;
    RSDK.GetSceneLayer(PBL_Setup->tableLow)->scanlineCallback  = PBL_Setup_TableLow_ScanlineCallback;
    RSDK.GetSceneLayer(PBL_Setup->tableHigh)->scanlineCallback = PBL_Setup_TableHigh_ScanlineCallback;
    RSDK.GetSceneLayer(1)->scanlineCallback                    = PBL_Setup_BG_ScanlineCallback;
    RSDK.SetDrawLayerProperties(1, false, PBL_Setup_DrawLayer_Callback);
    RSDK.SetDrawLayerProperties(3, false, PBL_Setup_DrawLayer_Callback);
    RSDK.SetDrawLayerProperties(4, true, NULL);
    RSDK.SetLimitedFade(1, 0, 7, 36, 0, 255);
    RSDK.SetLimitedFade(2, 0, 7, 72, 0, 255);
    RSDK.SetLimitedFade(3, 0, 7, 108, 0, 255);
    RSDK.SetLimitedFade(4, 0, 7, 144, 0, 255);
    RSDK.SetLimitedFade(5, 0, 7, 180, 0, 255);
    RSDK.SetLimitedFade(6, 0, 7, 216, 0, 255);
    RSDK.ResetEntitySlot(SLOT_PBL_SETUP, PBL_Setup->objectID, NULL);
    PBL_Setup->sfxContinue = RSDK.GetSFX("Special/Continue.wav");
}

void PBL_Setup_TableLow_ScanlineCallback(ScanlineInfo *scanlines) {}
void PBL_Setup_TableHigh_ScanlineCallback(ScanlineInfo *scanlines) {}
void PBL_Setup_BG_ScanlineCallback(ScanlineInfo *scanlines)
{
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, 112);
    int x       = 0x74C6BA;
    int centerX = RSDK_screens->centerX;
    int sin     = RSDK.Sin256(32);
    int cos     = RSDK.Cos256(32);
    int timer   = PBL_Setup->timer3 >> 1;

    int clr = 0;
    for (int i = 160; i > 40; --i) {
        x += 0xE000;
        int val               = x / (8 * i);
        scanlines->deform.x   = -(cos * val) >> 7;
        scanlines->deform.y   = sin * val >> 7;
        scanlines->position.x = timer + sin * val - centerX * scanlines->deform.x;
        scanlines->position.y = timer + cos * val - centerX * (sin * val >> 7);
        int palID             = (abs(val) >> 11) - 16;
        palID                 = min(7, palID);
        palID                 = max(0, palID);
        RSDK.SetActivePalette(palID, clr, clr + 1);
        scanlines++;
        ++clr;
    }
}

void PBL_Setup_DrawLayer_Callback(void)
{
    RSDK.SetClipBounds(0, 0, 0, RSDK_screens->width, RSDK_screens->height);
    RSDK.SetActivePalette(0, 0, RSDK_screens->height);
}

void PBL_Setup_Unknown5(void)
{
    EntityPBL_Setup *setup = RSDK_GET_ENTITY(SLOT_BSS_SETUP, PBL_Setup);
    setup->visible         = true;
    setup->state           = PBL_Setup_SaveAndChangeScene;
    setup->colour          = 0;
    Music_FadeOut(0.025);
    PauseMenu->disableEvents = true;
    int oldScore             = globals->restartScore;
    globals->restartScore += PBL_Setup->score;
    if (globals->restartScore > 9999999 || globals->restartScore < oldScore) {
        globals->restartScore = 9999999;
    }

    while (globals->restartScore1UP < globals->restartScore) {
        globals->restartScore1UP += 50000;
    }
}

void PBL_Setup_Unknown7(int score)
{
    PBL_Setup->score += score;
    if (PBL_Setup->score > 9999999) {
        PBL_Setup->score = 9999999;
    }

    if (PBL_Setup->score > PBL_Setup->score1UP) {
        RSDK.PlaySFX(PBL_Setup->sfxContinue, 0, 255);

        int *saveRAM = SaveGame->saveRAM;
        if (saveRAM[29] < 20)
            saveRAM[29]++;
        while (PBL_Setup->score1UP < PBL_Setup->score) {
            PBL_Setup->score1UP += 10000;
        }

        foreach_active(PBL_HUD, hud)
        {
            // PBL_HUD_DisplayMessage(entity, "!CONTINUE!", true);
        }
    }
}

void PBL_Setup_GiveLife(void)
{
    int *saveRAM = SaveGame->saveRAM;
    if (globals->gameMode != MODE_TIMEATTACK && globals->gameMode != MODE_ENCORE) {
        if (saveRAM[25] < 99)
            saveRAM[25]++;
        Music_PlayMusicTrack(TRACK_1UP);
    }
}

void PBL_Setup_Unknown9(void)
{
    RSDK_THIS(PBL_Setup);
    if (entity->timer <= 0) {
        entity->timer               = 0;
        entity->visible             = false;
        entity->state               = PBL_Setup_Unknown10;
        RSDK_sceneInfo->timeEnabled = true;
    }
    else {
        entity->timer -= 16;
    }
}

void PBL_Setup_Unknown10(void)
{
    EntityPBL_Camera *camera = RSDK_GET_ENTITY(SLOT_PBL_CAMERA, PBL_Camera);
    foreach_active(PBL_Player, player)
    {
        if (player->position.y > camera->dword70 + 0x400000) {
            PBL_Setup_Unknown5();
            player->state = StateMachine_None;
        }
    }
}

void PBL_Setup_SaveAndChangeScene(void)
{
    RSDK_THIS(PBL_Setup);
    if (entity->timer >= 1024) {
        int *saveRAM = SaveGame->saveRAM;
        if (saveRAM) {
            saveRAM[66] = globals->characterFlags;
            saveRAM[67] = globals->stock;
            saveRAM[68] = globals->playerID;
            if (globals->gameMode == MODE_ENCORE)
                RSDK.LoadScene("Encore Mode", "");
            else
                RSDK.LoadScene("Mania Mode", "");
            RSDK_sceneInfo->listPos = saveRAM[30];
        }
        else {
            RSDK.LoadScene("Presentation", "Title Screen");
        }
        RSDK.InitSceneLoad();
    }
    else {
        entity->timer += 16;
    }
}

void PBL_Setup_EditorDraw(void) {}

void PBL_Setup_EditorLoad(void) {}

void PBL_Setup_Serialize(void) {}
#endif
