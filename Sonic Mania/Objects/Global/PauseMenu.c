#include "SonicMania.h"

ObjectPauseMenu *PauseMenu;

void PauseMenu_Update(void)
{
    RSDK_THIS(PauseMenu);
    StateMachine_Run(entity->state);

    entity->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    entity->position.y = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
    if (entity->manager) {
        entity->manager->position.x = entity->position.x;
        entity->manager->position.y = entity->position.y;
        PauseMenu_HandleButtonPositions();
    }
}

void PauseMenu_LateUpdate(void)
{
    RSDK_THIS(PauseMenu);
    if (entity->state) {
        if (RSDK.ChannelActive(Music->channelID))
            RSDK.PauseChannel(Music->channelID);
    }
    else {
        void *state = RSDK_GET_ENTITY(entity->triggerPlayer, Player)->state;
        if (state == Player_State_Die || state == Player_State_Drown) {
            destroyEntity(entity);
        }
        else {
            entity->visible   = true;
            entity->drawOrder = DRAWLAYER_COUNT - 1;
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 10, &entity->animator, true, 3);
            PauseMenu_PauseSound();
            entity->state = PauseMenu_SetupButtons;
        }
    }
}

void PauseMenu_StaticUpdate(void)
{
    if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR) {
        int cnt = 0;
        if (TitleCard)
            cnt = RSDK.GetEntityCount(TitleCard->objectID, true);
        if (ActClear)
            cnt += RSDK.GetEntityCount(ActClear->objectID, true);
        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

        bool32 flag = true;
        if (Zone)
            flag = Zone->timer > 1;

        if (!cnt && pauseMenu->objectID == TYPE_BLANK && flag && !PauseMenu->disableEvents) {
            if (API.GetUserAuthStatus() == STATUS_FORBIDDEN) {
                PauseMenu->signoutDetected = true;
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                pauseMenu->triggerPlayer = 0;
            }
#if RETRO_USE_PLUS
            else if (API.CheckDLC(DLC_PLUS) != globals->lastHasPlus) {
                PauseMenu->plusChanged = true;
                globals->lastHasPlus   = API.CheckDLC(DLC_PLUS);
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                pauseMenu->triggerPlayer = 0;
            }
#endif
            else {
                for (int i = 0; i < PauseMenu_GetPlayerCount(); ++i) {
#if RETRO_USE_PLUS
                    int id = RSDK.ControllerIDForInputID(i + 1);
                    if (!RSDK.GetAssignedControllerID(id) && id != CONT_AUTOASSIGN) {
                        //PauseMenu->controllerDisconnect = true;
                        //RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                        //pauseMenu->triggerPlayer = i;
                    }
#else
                    if (APICallback_InputIDIsDisconnected(i + 1)) {
                        PauseMenu->controllerDisconnect = true;
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                        pauseMenu->triggerPlayer = i;
                    }
#endif
                }
            }
        }
    }
}

void PauseMenu_Draw(void)
{
    RSDK_THIS(PauseMenu);
    if (entity->field_A4)
        RSDK.FillScreen(0, entity->fillTimer, entity->fillTimer - 128, entity->fillTimer - 256);
    if (RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT) <= 1) {
        StateMachine_Run(entity->stateDraw);
    }
}

void PauseMenu_Create(void *data)
{
    RSDK_THIS(PauseMenu);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_ALWAYS;
        if (data == intToVoid(1)) {
            entity->active    = ACTIVE_ALWAYS;
            entity->visible   = true;
            entity->drawOrder = DRAWLAYER_COUNT - 1;
            entity->state     = PauseMenu_Unknown27;
        }
        else {
            entity->state     = StateMachine_None;
            entity->stateDraw = StateMachine_None;
        }
    }
}

void PauseMenu_StageLoad(void)
{
    PauseMenu->active               = ACTIVE_ALWAYS;
    PauseMenu->sfxBleep             = RSDK.GetSFX("Global/MenuBleep.wav");
    PauseMenu->sfxAccept            = RSDK.GetSFX("Global/MenuAccept.wav");
    PauseMenu->disableEvents        = false;
    PauseMenu->controllerDisconnect = false;
    PauseMenu->dword10              = false;
    PauseMenu->signoutDetected      = false;
    PauseMenu->plusChanged          = false;
#if RETRO_USE_PLUS
    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = API.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
#endif
    for (int i = 0; i < 0x10; ++i) {
        PauseMenu->channelFlags[i] = false;
    }
    PauseMenu_SetupLookupTable();
}

byte PauseMenu_GetPlayerCount(void)
{
    EntityMenuParam *param            = (EntityMenuParam *)globals->menuParam;
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
    if (RSDK.CheckStageFolder("Puyo")) {
        if (param->selectionFlag != 1 && param->selectionFlag)
            return 2;
    }
    else if (globals->gameMode == MODE_COMPETITION) {
        return session->playerCount;
    }
    return 1;
}

void PauseMenu_SetupLookupTable(void)
{
    for (int i = 0; i < 0x10000; ++i) {
        int val = ((((0x103 * ((i >> 5) & 0x3F) + 33) >> 6) + ((0x20F * (i & 0x1F) + 0x17) >> 6) + ((0x20F * (i >> 11) + 0x17) >> 6)) << 8) / 0x2A8;
        val     = minVal(0xFF, val);
        PauseMenu->lookupTable[i] = (val >> 3) | ((val >> 3) << 11) | 8 * val & 0xFFE0;

        // found as the "default" lookup table in rev01, produces a similar (but lighter) effect
        // included here because I think it is neat :)
        // int val = ((i & 0x1F) + ((i >> 6) & 0x1F) + ((i >> 11) & 0x1F)) / 3 + 6;
        // PauseMenu->lookupTable[i] = 0x841 * minVal(0x1F, val);
    }
}

void PauseMenu_Unknown3(void)
{
    RSDK_THIS(PauseMenu);
    Vector2 drawPos;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y - 0x600000;
    drawPos.x += 0x640000 + entity->field_68.x + -0x10000 * RSDK_screens->centerX;
    drawPos.y += entity->field_68.y;
    UIWidgets_Unknown7(68, 200, 68, 232, 40, 88, drawPos.x, drawPos.y);
    drawPos.y += 0x60000;
    drawPos.x += 0xA0000;
    UIWidgets_Unknown7(24, 115, 24, 0, 0, 0, drawPos.x, drawPos.y);
    RSDK.DrawSprite(&entity->animator, &drawPos, 0);
    UIWidgets_Unknown5(240, -232, 216, 8, entity->field_70.x + (RSDK_screens->centerX << 16) + entity->position.x,
                       entity->field_70.y + (RSDK_screens->centerY << 16) + entity->position.y);
}

void PauseMenu_HandleButtonPositions(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 pos;
    pos.x = ((RSDK_screens->centerX - 69) << 16) + entity->position.x + entity->field_70.x;
    pos.y = (RSDK_sceneInfo->entity->position.y + 0x380000) + entity->field_70.y - 0x240000;
    if (entity->buttonCount == 2) {
        pos.x -= 0x240000;
        pos.y += 0x240000;
    }

    for (int i = 0; i < entity->buttonCount; ++i) {
        if (!entity->buttonPtrs[i])
            break;
        EntityUIButton *button = (EntityUIButton *)entity->buttonPtrs[i];
        button->posUnknown2.x  = pos.x;
        button->posUnknown2.y  = pos.y;
        button->position.x     = pos.x;
        button->position.y     = pos.y;
        pos.x -= 0x240000;
        pos.y += 0x240000;
    }
}

void PauseMenu_AddButton(byte id, void *action)
{
    RSDK_THIS(PauseMenu);

    int buttonID = entity->buttonCount;
    if (buttonID < 3) {
        entity->buttonIDs[buttonID]     = id;
        entity->buttonActions[buttonID] = action;

        int buttonSlot = entity->buttonCount + 18;
        RSDK.ResetEntitySlot(buttonSlot, UIButton->objectID, NULL);
        EntityUIButton *button = RSDK_GET_ENTITY(buttonSlot, UIButton);

        button->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        button->position.y = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 10, &button->animator, true, id);
        button->options2             = PauseMenu_Unknown16;
        button->size.x               = 0x3C0000;
        button->size.y               = 0x150000;
        button->dword138             = 21;
        button->align                = ALIGN_LEFT;
        button->drawOrder            = entity->drawOrder;
        button->active               = ACTIVE_ALWAYS;
        entity->buttonPtrs[buttonID] = (Entity *)button;
        ++entity->buttonCount;
    }
}

void PauseMenu_SetupMenu(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 size;
    size.x = RSDK_screens->width << 16;
    size.y = RSDK_screens->height << 16;
    RSDK.ResetEntitySlot(SLOT_PAUSEMENU_UICONTROL, UIControl->objectID, &size);

    EntityUIControl *control = RSDK_GET_ENTITY(SLOT_PAUSEMENU_UICONTROL, UIControl);

    control->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    control->position.y = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

    control->rowCount       = 3;
    control->columnCount    = 1;
    control->activeEntityID = 0;
    entity->manager         = (Entity *)control;

    int i = 0;
    for (; i < 3; ++i) {
        if (!entity->buttonPtrs[i])
            break;
        EntityUIButton *button = (EntityUIButton *)entity->buttonPtrs[i];
        button->parent         = (Entity *)control;
        control->buttons[i]   = button;
    }
    control->buttonCount = i;
}

void PauseMenu_ClearButtons(EntityPauseMenu *entity)
{
    if (entity->manager)
        destroyEntity(entity->manager);

    for (int i = 0; i < 3; ++i) {
        if (entity->buttonPtrs[i])
            destroyEntity(entity->buttonPtrs[i]);
    }

    destroyEntity(entity);
}

void PauseMenu_Unknown8(void)
{
    globals->specialRingID = 0;
    PauseMenu_StopSound();
    RSDK.InitSceneLoad();
}

void PauseMenu_Unknown9(void)
{
    if (StarPost) {
        StarPost->postIDs[0] = 0;
        StarPost->postIDs[1] = 0;
        StarPost->postIDs[2] = 0;
        StarPost->postIDs[3] = 0;
    }
    globals->specialRingID = 0;

    if (globals->gameMode == MODE_COMPETITION) {
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        EntityMenuParam *param            = (EntityMenuParam *)globals->menuParam;
        sprintf(param->menuTag, "Competition Zones");
        param->selectionID = session->levelIndex;
    }
    RSDK.LoadScene("Presentation", "Menu");
    PauseMenu_StopSound();
    RSDK.InitSceneLoad();
}

void PauseMenu_FocusCamera(void)
{
    RSDK_THIS(PauseMenu);
    if (!Camera)
        return;

    LogHelpers_Print("FocusCamera(): triggerPlayer = %d", entity->triggerPlayer);
    foreach_all(Camera, camera)
    {
        int id         = RSDK.GetEntityID(camera);
        int prevScreen = camera->screenID;
        if (id - SLOT_CAMERA1 == entity->triggerPlayer) {
            camera->screenID = 0;
            Camera_SetCameraBounds(camera);
        }
        else {
            camera->screenID = 1;
        }
        LogHelpers_Print("cameraPtr->screenID %d => %d", prevScreen, camera->screenID);
    }
}

void PauseMenu_UpdateCameras(void)
{
    if (!Camera)
        return;

    foreach_all(Camera, camera)
    {
        camera->screenID = RSDK.GetEntityID(camera) - SLOT_CAMERA1;
        Camera_SetCameraBounds(camera);
    }
}

void PauseMenu_Resume_CB(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
        pauseMenu->state = PauseMenu_Unknown23;
    else
        pauseMenu->state = PauseMenu_Unknown26;
}

void PauseMenu_Restart_CB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);
    TextInfo textBuffer;

    int strID = STR_AREYOUSURE;
#if RETRO_USE_PLUS
    if (!ReplayRecorder || !ReplayRecorder->dword134)
        strID = STR_RESTARTWARNING;
#endif
    Localization_GetString(&textBuffer, strID);

    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, 1);
        UIDialog_AddButton(DIALOG_YES, dialog, PauseMenu_RestartDialog_YesCB, 0);
        UIDialog_Setup(dialog);
    }
}

void PauseMenu_RestartDialog_YesCB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);

    ((EntityUIDialog *)UIDialog->activeDialog)->parent->state = NULL;
    if (StarPost) {
        StarPost->postIDs[0] = 0;
        StarPost->postIDs[1] = 0;
#if RETRO_USE_PLUS
        StarPost->postIDs[2] = 0;
        StarPost->postIDs[3] = 0;
#endif
    }
    RSDK.StopChannel(Music->channelID);
    EntityPauseMenu *entity = CREATE_ENTITY(PauseMenu, intToVoid(1), (RSDK_screens->position.x + RSDK_screens->centerX) << 16,
                                             (RSDK_screens->position.y + RSDK_screens->centerY) << 16);
    entity->funcPtrUnknown = PauseMenu_Unknown8;
    entity->state          = PauseMenu_Unknown27;
}

void PauseMenu_Exit_CB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);
    TextInfo textBuffer;

    int strID = STR_AREYOUSURE;
#if RETRO_USE_PLUS
    if (!ReplayRecorder || !ReplayRecorder->dword134)
        strID = STR_QUITWARNINGLOSEPROGRESS;
#endif
    Localization_GetString(&textBuffer, strID);

    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, 1);
        UIDialog_AddButton(DIALOG_YES, dialog, PauseMenu_ExitDialog_YesCB, 0);
        UIDialog_Setup(dialog);
    }
}

void PauseMenu_ExitDialog_YesCB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);
    ((EntityUIDialog *)UIDialog->activeDialog)->parent->state = NULL;
    globals->recallEntities                                   = false;
    globals->initCoolBonus                                    = false;

    if (StarPost) {
        StarPost->postIDs[0] = 0;
        StarPost->postIDs[1] = 0;
        StarPost->postIDs[2] = 0;
        StarPost->postIDs[3] = 0;
    }

    RSDK.StopChannel(Music->channelID);
    EntityPauseMenu *entity = CREATE_ENTITY(PauseMenu, intToVoid(1), (RSDK_screens->position.x + RSDK_screens->centerX) << 16,
                                            (RSDK_screens->position.y + RSDK_screens->centerY) << 16);
    entity->funcPtrUnknown = PauseMenu_Unknown9;
    entity->state          = PauseMenu_Unknown27;
}

void PauseMenu_Unknown16(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    EntityUIControl *manager   = (EntityUIControl *)pauseMenu->manager;

    if (manager->activeEntityID >= 0 && manager->activeEntityID < manager->buttonCount) {
        StateMachine_Run(pauseMenu->buttonActions[manager->activeEntityID]);
    }
}

void PauseMenu_PauseSound(void)
{
    for (int i = 0; i < 0x10; ++i) {
        if (RSDK.ChannelActive(i)) {
            RSDK.PauseChannel(i);
            PauseMenu->channelFlags[i] = true;
        }
    }
}

void PauseMenu_ResumeSound(void)
{
    for (int i = 0; i < 0x10; ++i) {
        if (PauseMenu->channelFlags[i]) {
            RSDK.ResumeChannel(i);
            PauseMenu->channelFlags[i] = false;
        }
    }
}

void PauseMenu_StopSound(void)
{
    for (int i = 0; i < 0x10; ++i) {
        if (PauseMenu->channelFlags[i]) {
            RSDK.StopChannel(i);
            PauseMenu->channelFlags[i] = false;
        }
    }
}

void PauseMenu_SetupButtons(void)
{
    RSDK_THIS(PauseMenu);
    entity->timer      = 0;
    PauseMenu->dword10 = 0;
    if (PauseMenu->controllerDisconnect || PauseMenu->signoutDetected || PauseMenu->plusChanged) {
        if (PauseMenu->controllerDisconnect)
            entity->field_AC = PauseMenu_Unknown32;

        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            entity->state = PauseMenu_Unknown33;
        else
            entity->state = PauseMenu_Unknown34;
        entity->stateDraw = PauseMenu_Unknown37;
    }
    else {
        RSDK.PlaySFX(PauseMenu->sfxAccept, 0, 255);
        PauseMenu_AddButton(0, PauseMenu_Resume_CB);
        if (!entity->disableRestart)
            PauseMenu_AddButton(1, PauseMenu_Restart_CB);
        PauseMenu_AddButton(2, PauseMenu_Exit_CB);
        PauseMenu_HandleButtonPositions();
        PauseMenu_SetupMenu();
        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            entity->state = PauseMenu_Unknown21;
        else
            entity->state = PauseMenu_Unknown24;
        entity->stateDraw = PauseMenu_Unknown36;
#if RETRO_USE_PLUS
        if (globals->gameMode < MODE_TIMEATTACK && RSDK.ControllerIDForInputID(CONT_P2) == CONT_AUTOASSIGN)
            RSDK.AssignControllerID(CONT_P2, CONT_ANY);
#endif
    }

    StateMachine_Run(entity->state);
}

void PauseMenu_Unknown21(void)
{
    RSDK_THIS(PauseMenu);

    if (entity->timer == 1) {
        UIControl->inputLocked = 0;
        UIControl_Unknown5((EntityUIControl *)entity->manager);
    }

    if (entity->timer >= 8) {
        entity->field_68.x = 0;
        entity->field_68.y = 0;
        entity->field_70.x = 0;
        entity->field_70.y = 0;
        entity->timer      = 0;
        entity->state      = PauseMenu_Unknown22;
    }
    else {
        Vector2 pos;

        int val = 32 * entity->timer;
        MathHelpers_Unknown3(&pos, maxVal(0, val), -0xF00000, 0, 0, 0);

        entity->field_68.x = pos.x;
        entity->field_68.y = pos.y;
        MathHelpers_Unknown3(&pos, maxVal(0, val), 0xE80000, 0, 0, 0);

        ++entity->timer;
        entity->field_70.x = pos.x;
        entity->field_70.y = pos.y;
        entity->field_64   = val;
    }
}

void PauseMenu_Unknown22(void)
{
    RSDK_THIS(PauseMenu);
    entity->field_64   = 255;
    entity->field_68.x = 0;
    entity->field_68.y = 0;
    entity->field_70.x = 0;
    entity->field_70.y = 0;

    EntityUIControl *manager = (EntityUIControl *)entity->manager;
#if RETRO_USE_PLUS
    if (RSDK_unknown->field_10 && !manager->dialogHasFocus) {
#else
    if (/*RSDK_touchMouse->flag10 &&*/ !manager->dialogHasFocus) {
#endif
        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            pauseMenu->state = PauseMenu_Unknown23;
        else
            pauseMenu->state = PauseMenu_Unknown26;
    }
}

void PauseMenu_Unknown23(void)
{
    RSDK_THIS(PauseMenu);
#if RETRO_USE_PLUS
    if (!entity->timer && globals->gameMode < MODE_TIMEATTACK && !RSDK.ControllerIDForInputID(2))
        RSDK.AssignControllerID(CONT_P2, CONT_AUTOASSIGN);
#else
    if (!entity->timer && globals->gameMode < MODE_TIMEATTACK && !APICallback->ControllerIDForInputID(2))
        APICallback->AssignControllerID(CONT_P2, CONT_AUTOASSIGN);
#endif

    if (entity->timer >= 8) {
        entity->field_68.y = 0;
        entity->field_68.x = -0xF00000;
        entity->field_70.y = 0;
        entity->field_70.x = 0xE80000;
        entity->timer      = 0;
        RSDK.SetGameMode(ENGINESTATE_REGULAR);
        PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
        PauseMenu_ResumeSound();
    }
    else {
        Vector2 pos;

        int val = 32 * entity->timer;
        MathHelpers_Unknown3(&pos, maxVal(0, val), 0, 0, -0xF00000, 0);

        entity->field_68.x = pos.x;
        entity->field_68.y = pos.y;
        MathHelpers_Unknown3(&pos, maxVal(0, val), 0, 0, 0xE80000, 0);

        entity->field_70.x = pos.x;
        entity->field_70.y = pos.y;
        entity->field_64   = ((0x100 - entity->timer) << 8) / 8;
        ++entity->timer;
    }
}

void PauseMenu_Unknown24(void)
{
    RSDK_THIS(PauseMenu);

    if (entity->timer >= 8) {
        entity->field_68.x = 0x000000;
        entity->field_68.y = 0x000000;
        entity->field_70.x = 0x000000;
        entity->field_70.y = 0x000000;
        if (entity->timer >= 16) {
            entity->field_A4 = 0;
            entity->timer    = 0;
            entity->state    = PauseMenu_Unknown22;
        }
        else {
            int t            = entity->timer - 8;
            entity->field_A4 = 1;
            if (entity->timer == 8) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++entity->timer;
            entity->fillTimer = (8 - t) << 6;
        }
    }
    else {
        entity->field_68.x = 0xFFF0000;
        entity->field_68.y = 0xFFF0000;
        entity->field_70.x = 0xFFF0000;
        entity->field_70.y = 0xFFF0000;
        entity->field_A4   = 1;
        entity->fillTimer  = entity->timer << 6;
        entity->timer      = entity->timer + 1;
    }
}

void PauseMenu_Unknown26(void)
{
    RSDK_THIS(PauseMenu);

    if (entity->timer == 1) {
        UIControl->inputLocked = 0;
        UIControl_Unknown5((EntityUIControl *)entity->manager);
    }

    if (entity->timer >= 8) {
        if (entity->timer >= 16) {
            entity->field_A4 = 0;
            entity->timer    = 0;
            RSDK.SetGameMode(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int t = entity->timer - 8;
            if (entity->timer == 8) {
                EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, session->playerCount);
                PauseMenu_UpdateCameras();
            }
            ++entity->timer;
            entity->fillTimer = (8 - t) << 6;
        }
    }
    else {
        entity->field_68.x = 0;
        entity->field_68.y = 0;
        entity->field_70.x = 0;
        entity->field_70.y = 0;
        entity->field_A4   = 1;
        entity->fillTimer  = entity->timer << 6;
        entity->timer      = entity->timer + 1;
    }
}

void PauseMenu_Unknown27(void)
{
    RSDK_THIS(PauseMenu);
    entity->fillTimer += 12;
    entity->field_A4 = 1;

    if (entity->fillTimer >= 1024) {
        if (entity->funcPtrUnknown)
            entity->funcPtrUnknown();
    }
}

void PauseMenu_Unknown28(void)
{
    RSDK_THIS(PauseMenu);
    if (!UIDialog->activeDialog) {
        if (!entity->timer) {
            entity->field_A4 = 1;
            Music_FadeOut(0.2);
        }

        if (entity->timer >= 60) {
            entity->fillTimer = 512;
            SaveGame_ClearRestartData();
            RSDK.LoadScene("Presentation", "Title Screen");
            RSDK.InitSceneLoad();
        }
        else {
            entity->fillTimer = (entity->timer << 9) / 60;
            entity->timer++;
        }
    }
}

void PauseMenu_Unknown29(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    pauseMenu->timer           = 0;
    pauseMenu->fillTimer       = 0;
    pauseMenu->state           = PauseMenu_Unknown28;
}

void PauseMenu_Unknown31(void)
{
    EntityPauseMenu *entity = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

#if RETRO_USE_PLUS
    RSDK.ControllerIDForInputID((entity->triggerPlayer ^ 1) + 1);

    int id = RSDK.MostRecentActiveControllerID(1, 1, 5);
    if (id)
        RSDK.AssignControllerID(entity->triggerPlayer + 1, id);
    else
        RSDK.AssignControllerID(entity->triggerPlayer + 1, CONT_AUTOASSIGN);
    if (globals->gameMode < MODE_TIMEATTACK && !RSDK.ControllerIDForInputID(2))
        RSDK.AssignControllerID(CONT_P2, CONT_AUTOASSIGN);
#else
    APICallback->ControllerIDForInputID((entity->triggerPlayer ^ 1) + 1);

    int id = APICallback->MostRecentActiveControllerID(1, 1, 5);
    if (id)
        APICallback->AssignControllerID(entity->triggerPlayer + 1, id);
    else
        APICallback->AssignControllerID(entity->triggerPlayer + 1, CONT_AUTOASSIGN);
    if (globals->gameMode < MODE_TIMEATTACK && !APICallback->ControllerIDForInputID(2))
        APICallback->AssignControllerID(CONT_P2, CONT_AUTOASSIGN);
#endif

    PauseMenu->dword10 = true;
}

bool32 PauseMenu_Unknown32(void)
{
    RSDK_THIS(PauseMenu);
#if RETRO_USE_PLUS
    int id = RSDK.ControllerIDForInputID(entity->triggerPlayer + 1);
    return RSDK.GetAssignedControllerID(id) || PauseMenu->dword10;
#else
    int id = APICallback->ControllerIDForInputID(entity->triggerPlayer + 1);
    return /*APICallback->GetAssignedControllerID(id) ||*/ PauseMenu->dword10;
#endif
}

void PauseMenu_Unknown33(void)
{
    RSDK_THIS(PauseMenu);
    TextInfo textBuffer;

    if (entity->timer == 1) {
        UIControl->inputLocked = false;
        if (PauseMenu->controllerDisconnect) {
            int strID = STR_RECONNECTWIRELESSCONTROLLER;
#if RETRO_USE_PLUS
            if (RSDK_sku->platform == PLATFORM_SWITCH)
#else
            if (RSDK_info->platform == PLATFORM_SWITCH)
#endif
                strID = STR_RECONNECTCONTROLLER;
            Localization_GetString(&textBuffer, strID);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(4, dialog, PauseMenu_Unknown31, 0);
            UIDialog_Setup(dialog);
#if RETRO_USE_PLUS
            if (globals->gameMode < MODE_TIMEATTACK && RSDK.ControllerIDForInputID(2) == CONT_AUTOASSIGN)
                RSDK.AssignControllerID(CONT_P2, CONT_ANY);
#else
            if (globals->gameMode < MODE_TIMEATTACK && APICallback->ControllerIDForInputID(2) == CONT_AUTOASSIGN)
                APICallback->AssignControllerID(CONT_P2, CONT_ANY);
#endif
        }
        else if (PauseMenu->signoutDetected || PauseMenu->plusChanged) {
            int strID = STR_TESTSTR;
            if (PauseMenu->signoutDetected) {
                strID = STR_SIGNOUTDETECTED;
            }
#if RETRO_USE_PLUS
            else if (PauseMenu->plusChanged) {
                strID = STR_RETRURNINGTOTITLE;
            }
#endif
            Localization_GetString(&textBuffer, strID);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(2, dialog, PauseMenu_Unknown29, 1);
            UIDialog_Setup(dialog);
        }
    }

    ++entity->timer;
    if (!UIDialog->activeDialog) {
        if (entity->field_B0) {
            if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo")) {
                RSDK.SetGameMode(ENGINESTATE_REGULAR);
                PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
                PauseMenu_ResumeSound();
            }
            else {
                entity->timer = 0;
                entity->state = PauseMenu_Unknown35;
            }
        }
    }

    if (!entity->field_B0 && entity->field_AC) {
        if (entity->field_AC()) {
            if (PauseMenu->controllerDisconnect)
                PauseMenu->controllerDisconnect = false;

            EntityUIDialog *dialog = UIDialog->activeDialog;
            if (dialog) {
                if (dialog->state != UIDialog_Unknown13) {
                    dialog->parent->selectionDisabled = true;
                    dialog->field_5C                  = 0;
                    dialog->state                     = UIDialog_Unknown13;
                    dialog->curCallback               = NULL;
                }
            }
            entity->field_B0 = true;
        }
    }
}

void PauseMenu_Unknown34(void)
{
    RSDK_THIS(PauseMenu);
    if (entity->timer == 1) {
        UIControl->inputLocked = 0;
    }

    if (entity->timer >= 8) {
        if (entity->timer >= 16) {
            entity->field_A4  = 0;
            entity->fillTimer = 0;
            entity->timer     = 0;
            entity->state     = PauseMenu_Unknown33;
        }
        else {
            int t            = entity->timer - 8;
            entity->field_A4 = 1;
            if (entity->timer == 8) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++entity->timer;
            entity->fillTimer = (8 - t) << 6;
        }
    }
    else {
        entity->field_A4  = 1;
        entity->fillTimer = entity->timer << 6;
        entity->timer++;
    }
}

void PauseMenu_Unknown35(void)
{
    RSDK_THIS(PauseMenu);

    if (entity->timer >= 8) {
        if (entity->timer >= 16) {
            entity->field_A4 = 0;
            entity->timer    = 0;
            RSDK.SetGameMode(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int t            = entity->timer - 8;
            entity->field_A4 = 1;
            if (entity->timer == 8) {
                EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, session->playerCount);
                PauseMenu_UpdateCameras();
            }
            ++entity->timer;
            entity->fillTimer = (8 - t) << 6;
        }
    }
    else {
        entity->field_A4  = 1;
        entity->fillTimer = entity->timer << 6;
        entity->timer++;
    }
}

void PauseMenu_Unknown36(void)
{
    RSDK_THIS(PauseMenu);
    if (entity->state != PauseMenu_Unknown27) {
        RSDK.SetLookupTable(PauseMenu->lookupTable);
        RSDK.DrawRect(0, 0, RSDK_screens->width, RSDK_screens->height, 0, entity->field_64, INK_LOOKUP, true);
        PauseMenu_Unknown3();
    }
}

void PauseMenu_Unknown37(void)
{
    RSDK_THIS(PauseMenu);
    if (entity->state != PauseMenu_Unknown27) {
        RSDK.SetLookupTable(PauseMenu->lookupTable);
        RSDK.DrawRect(0, 0, RSDK_screens->width, RSDK_screens->height, 0, entity->field_64, INK_LOOKUP, true);
    }
}

void PauseMenu_EditorDraw(void) {}

void PauseMenu_EditorLoad(void) {}

void PauseMenu_Serialize(void) {}
