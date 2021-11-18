#include "SonicMania.h"

ObjectPauseMenu *PauseMenu;

void PauseMenu_Update(void)
{
    RSDK_THIS(PauseMenu);
    StateMachine_Run(self->state);

    self->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    self->position.y = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
    if (self->manager) {
        self->manager->position.x = self->position.x;
        self->manager->position.y = self->position.y;
        PauseMenu_HandleButtonPositions();
    }
}

void PauseMenu_LateUpdate(void)
{
    RSDK_THIS(PauseMenu);
    if (self->state) {
        if (RSDK.ChannelActive(Music->channelID))
            RSDK.PauseChannel(Music->channelID);
    }
    else {
        void *state = RSDK_GET_ENTITY(self->triggerPlayer, Player)->state;
        if (state == Player_State_Die || state == Player_State_Drown) {
            destroyEntity(self);
        }
        else {
            self->visible   = true;
            self->drawOrder = DRAWLAYER_COUNT - 1;
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 10, &self->animator, true, 3);
            PauseMenu_PauseSound();
            self->state = PauseMenu_SetupButtons;
        }
    }
}

void PauseMenu_StaticUpdate(void)
{
    if (SceneInfo->state == ENGINESTATE_REGULAR) {
        int32 cnt = 0;
        if (TitleCard)
            cnt = RSDK.GetEntityCount(TitleCard->objectID, true);
        if (ActClear)
            cnt += RSDK.GetEntityCount(ActClear->objectID, true);
        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

        bool32 flag = true;
        if (Zone)
            flag = Zone->timer > 1;

        if (!cnt && pauseMenu->objectID == TYPE_BLANK && flag && !PauseMenu->disableEvents) {
            if (API_GetUserAuthStatus() == STATUS_FORBIDDEN) {
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
                for (int32 i = 0; i < PauseMenu_GetPlayerCount(); ++i) {
#if RETRO_USE_PLUS
                    int32 id = API_ControllerIDForInputID(i + 1);
                    if (!RSDK.GetAssignedControllerID(id) && id != CONT_AUTOASSIGN) {
                        //PauseMenu->controllerDisconnect = true;
                        //RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                        //pauseMenu->triggerPlayer = i;
                    }
#else
                    if (API_InputIDIsDisconnected(i + 1)) {
                        // PauseMenu->controllerDisconnect = true;
                        // RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                        // pauseMenu->triggerPlayer = i;
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
    if (self->field_A4)
        RSDK.FillScreen(0, self->fillTimer, self->fillTimer - 128, self->fillTimer - 256);
    if (RSDK.GetSettingsValue(SETTINGS_SCREENCOUNT) <= 1) {
        StateMachine_Run(self->stateDraw);
    }
}

void PauseMenu_Create(void *data)
{
    RSDK_THIS(PauseMenu);
    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_ALWAYS;
        if (data == intToVoid(1)) {
            self->active    = ACTIVE_ALWAYS;
            self->visible   = true;
            self->drawOrder = DRAWLAYER_COUNT - 1;
            self->state     = PauseMenu_Unknown27;
        }
        else {
            self->state     = StateMachine_None;
            self->stateDraw = StateMachine_None;
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
#if RETRO_USE_PLUS
    PauseMenu->plusChanged          = false;
    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = API.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
#endif
    for (int32 i = 0; i < 0x10; ++i) {
        PauseMenu->channelFlags[i] = false;
    }
    PauseMenu_SetupLookupTable();
}

uint8 PauseMenu_GetPlayerCount(void)
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
    for (int32 i = 0; i < 0x10000; ++i) {
        int32 val = ((((0x103 * ((i >> 5) & 0x3F) + 33) >> 6) + ((0x20F * (i & 0x1F) + 0x17) >> 6) + ((0x20F * (i >> 11) + 0x17) >> 6)) << 8) / 0x2A8;
        val     = minVal(0xFF, val);
        PauseMenu->lookupTable[i] = (val >> 3) | ((val >> 3) << 11) | 8 * val & 0xFFE0;

        // found as the "default" lookup table in rev01, produces a similar (but lighter) effect
        // included here because I think it is neat :)
        // int32 val = ((i & 0x1F) + ((i >> 6) & 0x1F) + ((i >> 11) & 0x1F)) / 3 + 6;
        // PauseMenu->lookupTable[i] = 0x841 * minVal(0x1F, val);
    }
}

void PauseMenu_Unknown3(void)
{
    RSDK_THIS(PauseMenu);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y - 0x600000;
    drawPos.x += 0x640000 + self->field_68.x + -0x10000 * ScreenInfo->centerX;
    drawPos.y += self->field_68.y;
    UIWidgets_Unknown7(68, 200, 68, 232, 40, 88, drawPos.x, drawPos.y);
    drawPos.y += 0x60000;
    drawPos.x += 0xA0000;
    UIWidgets_Unknown7(24, 115, 24, 0, 0, 0, drawPos.x, drawPos.y);
    RSDK.DrawSprite(&self->animator, &drawPos, 0);
    UIWidgets_Unknown5(240, -232, 216, 8, self->field_70.x + (ScreenInfo->centerX << 16) + self->position.x,
                       self->field_70.y + (ScreenInfo->centerY << 16) + self->position.y);
}

void PauseMenu_HandleButtonPositions(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 pos;
    pos.x = ((ScreenInfo->centerX - 69) << 16) + self->position.x + self->field_70.x;
    pos.y = (self->position.y + 0x380000) + self->field_70.y - 0x240000;
    if (self->buttonCount == 2) {
        pos.x -= 0x240000;
        pos.y += 0x240000;
    }

    for (int32 i = 0; i < self->buttonCount; ++i) {
        if (!self->buttonPtrs[i])
            break;
        EntityUIButton *button = (EntityUIButton *)self->buttonPtrs[i];
        button->posUnknown2.x  = pos.x;
        button->posUnknown2.y  = pos.y;
        button->position.x     = pos.x;
        button->position.y     = pos.y;
        pos.x -= 0x240000;
        pos.y += 0x240000;
    }
}

void PauseMenu_AddButton(uint8 id, void *action)
{
    RSDK_THIS(PauseMenu);

    int32 buttonID = self->buttonCount;
    if (buttonID < 3) {
        self->buttonIDs[buttonID]     = id;
        self->buttonActions[buttonID] = action;

        int32 buttonSlot = self->buttonCount + 18;
        RSDK.ResetEntitySlot(buttonSlot, UIButton->objectID, NULL);
        EntityUIButton *button = RSDK_GET_ENTITY(buttonSlot, UIButton);

        button->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        button->position.y = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 10, &button->animator, true, id);
        button->options2             = PauseMenu_Unknown16;
        button->size.x               = 0x3C0000;
        button->size.y               = 0x150000;
        button->dword138             = 21;
        button->align                = ALIGN_LEFT;
        button->drawOrder            = self->drawOrder;
        button->active               = ACTIVE_ALWAYS;
        self->buttonPtrs[buttonID] = (Entity *)button;
        ++self->buttonCount;
    }
}

void PauseMenu_SetupMenu(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 size;
    size.x = ScreenInfo->width << 16;
    size.y = ScreenInfo->height << 16;
    RSDK.ResetEntitySlot(SLOT_PAUSEMENU_UICONTROL, UIControl->objectID, &size);

    EntityUIControl *control = RSDK_GET_ENTITY(SLOT_PAUSEMENU_UICONTROL, UIControl);

    control->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    control->position.y = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;

    control->rowCount       = 3;
    control->columnCount    = 1;
    control->activeEntityID = 0;
    self->manager         = (Entity *)control;

    int32 i = 0;
    for (; i < 3; ++i) {
        if (!self->buttonPtrs[i])
            break;
        EntityUIButton *button = (EntityUIButton *)self->buttonPtrs[i];
        button->parent         = (Entity *)control;
        control->buttons[i]   = button;
    }
    control->buttonCount = i;
}

void PauseMenu_ClearButtons(EntityPauseMenu *entity)
{
    if (entity->manager)
        destroyEntity(entity->manager);

    for (int32 i = 0; i < 3; ++i) {
        if (entity->buttonPtrs[i])
            destroyEntity(entity->buttonPtrs[i]);
    }

    destroyEntity(entity);
}

void PauseMenu_Unknown8(void)
{
    globals->specialRingID = 0;
    PauseMenu_StopSound();
    RSDK.LoadScene();
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
    RSDK.SetScene("Presentation", "Menu");
    PauseMenu_StopSound();
    RSDK.LoadScene();
}

void PauseMenu_FocusCamera(void)
{
    RSDK_THIS(PauseMenu);
    if (!Camera)
        return;

    LogHelpers_Print("FocusCamera(): triggerPlayer = %d", self->triggerPlayer);
    foreach_all(Camera, camera)
    {
        int32 id         = RSDK.GetEntityID(camera);
        int32 prevScreen = camera->screenID;
        if (id - SLOT_CAMERA1 == self->triggerPlayer) {
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

#if RETRO_USE_PLUS
    int32 strID = STR_AREYOUSURE;
    if (!ReplayRecorder || !ReplayRecorder->dword134)
        strID = STR_RESTARTWARNING;
    Localization_GetString(&textBuffer, strID);
#else
    Localization_GetString(&textBuffer, STR_RESTARTWARNING);
#endif

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
        StarPost->postIDs[2] = 0;
        StarPost->postIDs[3] = 0;
    }
    RSDK.StopChannel(Music->channelID);
    EntityPauseMenu *entity = CREATE_ENTITY(PauseMenu, intToVoid(1), (ScreenInfo->position.x + ScreenInfo->centerX) << 16,
                                             (ScreenInfo->position.y + ScreenInfo->centerY) << 16);
    entity->funcPtrUnknown = PauseMenu_Unknown8;
    entity->state          = PauseMenu_Unknown27;
}

void PauseMenu_Exit_CB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);
    TextInfo textBuffer;

#if RETRO_USE_PLUS
    int32 strID = STR_AREYOUSURE;
    if (!ReplayRecorder || !ReplayRecorder->dword134)
        strID = STR_QUITWARNINGLOSEPROGRESS;
    Localization_GetString(&textBuffer, strID);
#else
    Localization_GetString(&textBuffer, STR_QUITWARNINGLOSEPROGRESS);
#endif

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
    EntityPauseMenu *entity = CREATE_ENTITY(PauseMenu, intToVoid(1), (ScreenInfo->position.x + ScreenInfo->centerX) << 16,
                                            (ScreenInfo->position.y + ScreenInfo->centerY) << 16);
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
    for (int32 i = 0; i < 0x10; ++i) {
        if (RSDK.ChannelActive(i)) {
            RSDK.PauseChannel(i);
            PauseMenu->channelFlags[i] = true;
        }
    }
}

void PauseMenu_ResumeSound(void)
{
    for (int32 i = 0; i < 0x10; ++i) {
        if (PauseMenu->channelFlags[i]) {
            RSDK.ResumeChannel(i);
            PauseMenu->channelFlags[i] = false;
        }
    }
}

void PauseMenu_StopSound(void)
{
    for (int32 i = 0; i < 0x10; ++i) {
        if (PauseMenu->channelFlags[i]) {
            RSDK.StopChannel(i);
            PauseMenu->channelFlags[i] = false;
        }
    }
}

void PauseMenu_SetupButtons(void)
{
    RSDK_THIS(PauseMenu);
    self->timer      = 0;
    PauseMenu->dword10 = 0;
#if RETRO_USE_PLUS
    if (PauseMenu->controllerDisconnect || PauseMenu->signoutDetected || PauseMenu->plusChanged) {
#else
    if (PauseMenu->controllerDisconnect || PauseMenu->signoutDetected) {
#endif
        if (PauseMenu->controllerDisconnect)
            self->field_AC = PauseMenu_Unknown32;

        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            self->state = PauseMenu_Unknown33;
        else
            self->state = PauseMenu_Unknown34;
        self->stateDraw = PauseMenu_Unknown37;
    }
    else {
        RSDK.PlaySfx(PauseMenu->sfxAccept, 0, 255);
        PauseMenu_AddButton(0, PauseMenu_Resume_CB);
        if (!self->disableRestart)
            PauseMenu_AddButton(1, PauseMenu_Restart_CB);
        PauseMenu_AddButton(2, PauseMenu_Exit_CB);
        PauseMenu_HandleButtonPositions();
        PauseMenu_SetupMenu();
        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            self->state = PauseMenu_Unknown21;
        else
            self->state = PauseMenu_Unknown24;
        self->stateDraw = PauseMenu_Unknown36;
#if RETRO_USE_PLUS
        if (globals->gameMode < MODE_TIMEATTACK && API_ControllerIDForInputID(CONT_P2) == CONT_AUTOASSIGN)
            API_AssignControllerID(CONT_P2, CONT_ANY);
#endif
    }

    StateMachine_Run(self->state);
}

void PauseMenu_Unknown21(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = 0;
        UIControl_Unknown5((EntityUIControl *)self->manager);
    }

    if (self->timer >= 8) {
        self->field_68.x = 0;
        self->field_68.y = 0;
        self->field_70.x = 0;
        self->field_70.y = 0;
        self->timer      = 0;
        self->state      = PauseMenu_Unknown22;
    }
    else {
        Vector2 pos;

        int32 val = 32 * self->timer;
        MathHelpers_Lerp3(&pos, maxVal(0, val), -0xF00000, 0, 0, 0);

        self->field_68.x = pos.x;
        self->field_68.y = pos.y;
        MathHelpers_Lerp3(&pos, maxVal(0, val), 0xE80000, 0, 0, 0);

        ++self->timer;
        self->field_70.x = pos.x;
        self->field_70.y = pos.y;
        self->field_64   = val;
    }
}

void PauseMenu_Unknown22(void)
{
    RSDK_THIS(PauseMenu);
    self->field_64   = 255;
    self->field_68.x = 0;
    self->field_68.y = 0;
    self->field_70.x = 0;
    self->field_70.y = 0;

    EntityUIControl *manager = (EntityUIControl *)self->manager;
#if RETRO_USE_PLUS
    if (UnknownInfo->field_10 && !manager->dialogHasFocus) {
#else
    if (/*RSDK_touchMouse->flag10*/ false && !manager->dialogHasFocus) {
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
    if (!self->timer && globals->gameMode < MODE_TIMEATTACK && !API_ControllerIDForInputID(2))
        API_AssignControllerID(CONT_P2, CONT_AUTOASSIGN);

    if (self->timer >= 8) {
        self->field_68.y = 0;
        self->field_68.x = -0xF00000;
        self->field_70.y = 0;
        self->field_70.x = 0xE80000;
        self->timer      = 0;
        RSDK.SetGameMode(ENGINESTATE_REGULAR);
        PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
        PauseMenu_ResumeSound();
    }
    else {
        Vector2 pos;

        int32 val = 32 * self->timer;
        MathHelpers_Lerp3(&pos, maxVal(0, val), 0, 0, -0xF00000, 0);

        self->field_68.x = pos.x;
        self->field_68.y = pos.y;
        MathHelpers_Lerp3(&pos, maxVal(0, val), 0, 0, 0xE80000, 0);

        self->field_70.x = pos.x;
        self->field_70.y = pos.y;
        self->field_64   = ((0x100 - self->timer) << 8) / 8;
        ++self->timer;
    }
}

void PauseMenu_Unknown24(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer >= 8) {
        self->field_68.x = 0x000000;
        self->field_68.y = 0x000000;
        self->field_70.x = 0x000000;
        self->field_70.y = 0x000000;
        if (self->timer >= 16) {
            self->field_A4 = 0;
            self->timer    = 0;
            self->state    = PauseMenu_Unknown22;
        }
        else {
            int32 t            = self->timer - 8;
            self->field_A4 = 1;
            if (self->timer == 8) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++self->timer;
            self->fillTimer = (8 - t) << 6;
        }
    }
    else {
        self->field_68.x = 0xFFF0000;
        self->field_68.y = 0xFFF0000;
        self->field_70.x = 0xFFF0000;
        self->field_70.y = 0xFFF0000;
        self->field_A4   = 1;
        self->fillTimer  = self->timer << 6;
        self->timer      = self->timer + 1;
    }
}

void PauseMenu_Unknown26(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = 0;
        UIControl_Unknown5((EntityUIControl *)self->manager);
    }

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->field_A4 = 0;
            self->timer    = 0;
            RSDK.SetGameMode(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int32 t = self->timer - 8;
            if (self->timer == 8) {
                EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, session->playerCount);
                PauseMenu_UpdateCameras();
            }
            ++self->timer;
            self->fillTimer = (8 - t) << 6;
        }
    }
    else {
        self->field_68.x = 0;
        self->field_68.y = 0;
        self->field_70.x = 0;
        self->field_70.y = 0;
        self->field_A4   = 1;
        self->fillTimer  = self->timer << 6;
        self->timer      = self->timer + 1;
    }
}

void PauseMenu_Unknown27(void)
{
    RSDK_THIS(PauseMenu);
    self->fillTimer += 12;
    self->field_A4 = 1;

    if (self->fillTimer >= 1024) {
        if (self->funcPtrUnknown)
            self->funcPtrUnknown();
    }
}

void PauseMenu_Unknown28(void)
{
    RSDK_THIS(PauseMenu);
    if (!UIDialog->activeDialog) {
        if (!self->timer) {
            self->field_A4 = 1;
            Music_FadeOut(0.2);
        }

        if (self->timer >= 60) {
            self->fillTimer = 512;
            SaveGame_ClearRestartData();
            RSDK.SetScene("Presentation", "Title Screen");
            RSDK.LoadScene();
        }
        else {
            self->fillTimer = (self->timer << 9) / 60;
            self->timer++;
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

    int32 id = RSDK.MostRecentActiveControllerID(1, 1, 5);
#else
    int32 contID = API_ControllerIDForInputID((entity->triggerPlayer ^ 1) + 1);
    int32 id = API_MostRecentActiveControllerID(contID);
#endif

    if (id)
        API_AssignControllerID(entity->triggerPlayer + 1, id);
    else
        API_AssignControllerID(entity->triggerPlayer + 1, CONT_AUTOASSIGN);
    if (globals->gameMode < MODE_TIMEATTACK && !API_ControllerIDForInputID(2))
        API_AssignControllerID(CONT_P2, CONT_AUTOASSIGN);

    PauseMenu->dword10 = true;
}

bool32 PauseMenu_Unknown32(void)
{
    RSDK_THIS(PauseMenu);
    int32 id = API_ControllerIDForInputID(self->triggerPlayer + 1);
#if RETRO_USE_PLUS
    return RSDK.GetAssignedControllerID(id) || PauseMenu->dword10;
#else
    return id || PauseMenu->dword10;
#endif
}

void PauseMenu_Unknown33(void)
{
    RSDK_THIS(PauseMenu);
    TextInfo textBuffer;

    if (self->timer == 1) {
        UIControl->inputLocked = false;
        if (PauseMenu->controllerDisconnect) {
            int32 strID = STR_RECONNECTWIRELESSCONTROLLER;
            if (sku_platform == PLATFORM_SWITCH)
                strID = STR_RECONNECTCONTROLLER;
            Localization_GetString(&textBuffer, strID);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(4, dialog, PauseMenu_Unknown31, 0);
            UIDialog_Setup(dialog);
            if (globals->gameMode < MODE_TIMEATTACK && API_ControllerIDForInputID(2) == CONT_AUTOASSIGN)
                API_AssignControllerID(CONT_P2, CONT_ANY);
        }
#if RETRO_USE_PLUS
        else if (PauseMenu->signoutDetected || PauseMenu->plusChanged) {
            int32 strID = STR_TESTSTR;
            if (PauseMenu->signoutDetected) {
                strID = STR_SIGNOUTDETECTED;
            }
            else if (PauseMenu->plusChanged) {
                strID = STR_RETRURNINGTOTITLE;
            }

            Localization_GetString(&textBuffer, strID);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(2, dialog, PauseMenu_Unknown29, 1);
            UIDialog_Setup(dialog);
        }
#else
        else if (PauseMenu->signoutDetected) {
            Localization_GetString(&textBuffer, STR_SIGNOUTDETECTED);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(2, dialog, PauseMenu_Unknown29, 1);
            UIDialog_Setup(dialog);
        }
#endif
    }

    ++self->timer;
    if (!UIDialog->activeDialog) {
        if (self->field_B0) {
            if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo")) {
                RSDK.SetGameMode(ENGINESTATE_REGULAR);
                PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
                PauseMenu_ResumeSound();
            }
            else {
                self->timer = 0;
                self->state = PauseMenu_Unknown35;
            }
        }
    }

    if (!self->field_B0 && self->field_AC) {
        if (self->field_AC()) {
            if (PauseMenu->controllerDisconnect)
                PauseMenu->controllerDisconnect = false;

            EntityUIDialog *dialog = UIDialog->activeDialog;
            if (dialog) {
                if (dialog->state != UIDialog_Unknown13) {
                    dialog->parent->selectionDisabled = true;
                    dialog->timer                     = 0;
                    dialog->state                     = UIDialog_Unknown13;
                    dialog->curCallback               = NULL;
                }
            }
            self->field_B0 = true;
        }
    }
}

void PauseMenu_Unknown34(void)
{
    RSDK_THIS(PauseMenu);
    if (self->timer == 1) {
        UIControl->inputLocked = 0;
    }

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->field_A4  = 0;
            self->fillTimer = 0;
            self->timer     = 0;
            self->state     = PauseMenu_Unknown33;
        }
        else {
            int32 t            = self->timer - 8;
            self->field_A4 = 1;
            if (self->timer == 8) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++self->timer;
            self->fillTimer = (8 - t) << 6;
        }
    }
    else {
        self->field_A4  = 1;
        self->fillTimer = self->timer << 6;
        self->timer++;
    }
}

void PauseMenu_Unknown35(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->field_A4 = 0;
            self->timer    = 0;
            RSDK.SetGameMode(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int32 t            = self->timer - 8;
            self->field_A4 = 1;
            if (self->timer == 8) {
                EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, session->playerCount);
                PauseMenu_UpdateCameras();
            }
            ++self->timer;
            self->fillTimer = (8 - t) << 6;
        }
    }
    else {
        self->field_A4  = 1;
        self->fillTimer = self->timer << 6;
        self->timer++;
    }
}

void PauseMenu_Unknown36(void)
{
    RSDK_THIS(PauseMenu);
    if (self->state != PauseMenu_Unknown27) {
        RSDK.SetLookupTable(PauseMenu->lookupTable);
        RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, self->field_64, INK_LOOKUP, true);
        PauseMenu_Unknown3();
    }
}

void PauseMenu_Unknown37(void)
{
    RSDK_THIS(PauseMenu);
    if (self->state != PauseMenu_Unknown27) {
        RSDK.SetLookupTable(PauseMenu->lookupTable);
        RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, self->field_64, INK_LOOKUP, true);
    }
}

#if RETRO_INCLUDE_EDITOR
void PauseMenu_EditorDraw(void) {}

void PauseMenu_EditorLoad(void) {}
#endif

void PauseMenu_Serialize(void) {}
