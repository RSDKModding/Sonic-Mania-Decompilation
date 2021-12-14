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
        StateMachine(state) = RSDK_GET_ENTITY(self->triggerPlayer, Player)->state;
        if (state == Player_State_Die || state == Player_State_Drown) {
            destroyEntity(self);
        }
        else {
            self->visible   = true;
            self->drawOrder = DRAWLAYER_COUNT - 1;
            RSDK.SetGameMode(ENGINESTATE_FROZEN);
            RSDK.SetSpriteAnimation(UIWidgets->textFrames, 10, &self->animator, true, 3);
            PauseMenu_PauseSound();
#if !RETRO_USE_PLUS
            PauseMenu_SetupLookupTable();
#endif
            self->state = PauseMenu_State_SetupButtons;
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
                        PauseMenu->controllerDisconnect = true;
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->objectID, NULL);
                        pauseMenu->triggerPlayer = i;
                    }
#else
                    if (API_InputIDIsDisconnected(i + 1)) {
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
    if (self->paused)
        RSDK.FillScreen(0, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
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
            self->state     = PauseMenu_State_FadeToCB;
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
    PauseMenu->sfxBleep             = RSDK.GetSfx("Global/MenuBleep.wav");
    PauseMenu->sfxAccept            = RSDK.GetSfx("Global/MenuAccept.wav");
    PauseMenu->disableEvents        = false;
    PauseMenu->controllerDisconnect = false;
    PauseMenu->forcedDisconnect     = false;
    PauseMenu->signoutDetected      = false;
#if RETRO_USE_PLUS
    PauseMenu->plusChanged = false;
    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = API.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
#endif
    for (int32 i = 0; i < 0x10; ++i) {
        PauseMenu->channelFlags[i] = false;
    }
#if RETRO_USE_PLUS
    PauseMenu_SetupLookupTable();
#endif
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

    // Bug Details:
    // control->rowCount is slightly bugged, if `pauseMenu->disableRestart` is enabled then wrapping by pressing down is broken and wont work
    // Fix:
    // set it properly like buttonCount is done below
    control->rowCount       = 3;
    control->columnCount    = 1;
    control->buttonID = 0;
    self->manager           = (Entity *)control;

    int32 i = 0;
    for (; i < 3; ++i) {
        if (!self->buttonPtrs[i])
            break;
        EntityUIButton *button = (EntityUIButton *)self->buttonPtrs[i];
        button->parent         = (Entity *)control;
        control->buttons[i]    = button;
    }
    control->buttonCount = i;
}

void PauseMenu_SetupLookupTable(void)
{
#if RETRO_USE_PLUS
    for (int32 i = 0; i < 0x10000; ++i) {
        int32 brightness =
            ((((0x103 * ((i >> 5) & 0x3F) + 33) >> 6) + ((0x20F * (i & 0x1F) + 0x17) >> 6) + ((0x20F * (i >> 11) + 0x17) >> 6)) << 8) / 0x2A8;
        brightness                = minVal(0xFF, brightness);
        PauseMenu->lookupTable[i] = (brightness >> 3) | ((brightness >> 3) << 11) | 8 * brightness & 0xFFE0;
    }
#else
    uint16 *lookupTable = RSDK.GetLookupTable();
    for (int32 i = 0; i < 0x10000; ++i) {
        uint32 r = (527 * (i >> 11) + 23) >> 6;
        uint32 g = (527 * (i & 0x1F) + 23) >> 7;
        uint32 b = (259 * ((i >> 5) & 0x3F) + 33) >> 6;

        int32 rVal = 0, gVal = 0, bVal = 0;
        ColorHelpers_Unknown1(r, g, b, &rVal, &gVal, &bVal);

        uint32 brightness = 13 * bVal / 16;
        if (brightness > 255)
            brightness = 255;

        ColorHelpers_Unknown2(0, rVal, brightness, &r, &g, &b);

        lookupTable[i] = ColorHelpers_PackRGB(r, g, b);
    }
#endif
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
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 10, &button->animator, true, id);
        button->actionCB           = PauseMenu_ButtonActionCB;
        button->size.x             = 0x3C0000;
        button->size.y             = 0x150000;
        button->bgEdgeSize         = 21;
        button->align              = ALIGN_LEFT;
        button->drawOrder          = self->drawOrder;
        button->active             = ACTIVE_ALWAYS;
        self->buttonPtrs[buttonID] = (Entity *)button;
        ++self->buttonCount;
    }
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

void PauseMenu_HandleButtonPositions(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 pos;
    pos.x = ((ScreenInfo->centerX - 69) << 16) + self->position.x + self->yellowTrianglePos.x;
    pos.y = (self->position.y + 0x380000) + self->yellowTrianglePos.y - 0x240000;
    if (self->buttonCount == 2) {
        pos.x -= 0x240000;
        pos.y += 0x240000;
    }

    for (int32 i = 0; i < self->buttonCount; ++i) {
        if (!self->buttonPtrs[i])
            break;
        EntityUIButton *button = (EntityUIButton *)self->buttonPtrs[i];
        button->startPos.x     = pos.x;
        button->startPos.y     = pos.y;
        button->position.x     = pos.x;
        button->position.y     = pos.y;
        pos.x -= 0x240000;
        pos.y += 0x240000;
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

void PauseMenu_FocusCamera(void)
{
    RSDK_THIS(PauseMenu);
    if (!Camera)
        return;

    LogHelpers_Print("FocusCamera(): triggerPlayer = %d", self->triggerPlayer);
    foreach_all(Camera, cameraPtr)
    {
        int32 id         = RSDK.GetEntityID(cameraPtr);
        int32 prevScreen = cameraPtr->screenID;
        if (id - SLOT_CAMERA1 == self->triggerPlayer) {
            cameraPtr->screenID = 0;
            Camera_SetCameraBounds(cameraPtr);
        }
        else {
            cameraPtr->screenID = 1;
        }
        LogHelpers_Print("cameraPtr->screenID %d => %d", prevScreen, cameraPtr->screenID);
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

void PauseMenu_CheckAndReassignControllers(void)
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
    if (globals->gameMode < MODE_TIMEATTACK && !API_ControllerIDForInputID(CONT_P2))
        API_AssignControllerID(CONT_P2, CONT_AUTOASSIGN);

    PauseMenu->forcedDisconnect = true;
}

bool32 PauseMenu_IsDisconnected(void)
{
    RSDK_THIS(PauseMenu);
    int32 id = API_ControllerIDForInputID(self->triggerPlayer + 1);
#if RETRO_USE_PLUS
    return RSDK.GetAssignedControllerID(id) || PauseMenu->forcedDisconnect;
#else
    return id || PauseMenu->forcedDisconnect;
#endif
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


void PauseMenu_ResumeButtonCB(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
        pauseMenu->state = PauseMenu_State_Resume;
    else
        pauseMenu->state = PauseMenu_State_ResumeCompetition;
}

void PauseMenu_RestartButtonCB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);
    TextInfo msg;

#if RETRO_USE_PLUS
    int32 strID = STR_AREYOUSURE;
    if (!ReplayRecorder || !ReplayRecorder->isReplaying)
        strID = STR_RESTARTWARNING;
    Localization_GetString(&msg, strID);
#else
    Localization_GetString(&msg, STR_RESTARTWARNING);
#endif

    UIDialog_CreateDialogYesNo(&msg, PauseMenu_RestartDialog_YesCB, NULL, false, true);
}

void PauseMenu_ExitButtonCB(void)
{
    RSDK.GetEntityByID(SLOT_PAUSEMENU);
    TextInfo msg;

#if RETRO_USE_PLUS
    int32 strID = STR_AREYOUSURE;
    if (!ReplayRecorder || !ReplayRecorder->isReplaying)
        strID = STR_QUITWARNINGLOSEPROGRESS;
    Localization_GetString(&msg, strID);
#else
    Localization_GetString(&msg, STR_QUITWARNINGLOSEPROGRESS);
#endif

    UIDialog_CreateDialogYesNo(&msg, PauseMenu_ExitDialog_YesCB, NULL, false, true);
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
    entity->fadeCB          = PauseMenu_RestartFadeCB;
    entity->state           = PauseMenu_State_FadeToCB;
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
    entity->fadeCB          = PauseMenu_ExitFadeCB;
    entity->state           = PauseMenu_State_FadeToCB;
}

void PauseMenu_RestartFadeCB(void)
{
    globals->specialRingID = 0;
    PauseMenu_StopSound();
    RSDK.LoadScene();
}

void PauseMenu_ExitFadeCB(void)
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

void PauseMenu_ButtonActionCB(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    EntityUIControl *manager   = (EntityUIControl *)pauseMenu->manager;

    if (manager->buttonID >= 0 && manager->buttonID < manager->buttonCount) {
        StateMachine_Run(pauseMenu->buttonActions[manager->buttonID]);
    }
}


void PauseMenu_State_SetupButtons(void)
{
    RSDK_THIS(PauseMenu);
    self->timer                 = 0;
    PauseMenu->forcedDisconnect = false;
#if RETRO_USE_PLUS
    if (PauseMenu->controllerDisconnect || PauseMenu->signoutDetected || PauseMenu->plusChanged) {
#else
    if (PauseMenu->controllerDisconnect || PauseMenu->signoutDetected) {
#endif
        if (PauseMenu->controllerDisconnect)
            self->disconnectCheck = PauseMenu_IsDisconnected;

        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            self->state = PauseMenu_State_ForcedPause;
        else
            self->state = PauseMenu_State_ForcedPauseCompetition;
        self->stateDraw = PauseMenu_Draw_JustLookup;
    }
    else {
        RSDK.PlaySfx(PauseMenu->sfxAccept, false, 255);
        PauseMenu_AddButton(0, PauseMenu_ResumeButtonCB);
        if (!self->disableRestart)
            PauseMenu_AddButton(1, PauseMenu_RestartButtonCB);
        PauseMenu_AddButton(2, PauseMenu_ExitButtonCB);
        PauseMenu_HandleButtonPositions();
        PauseMenu_SetupMenu();
        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            self->state = PauseMenu_State_StartPause;
        else
            self->state = PauseMenu_State_StartPauseCompetition;
        self->stateDraw = PauseMenu_Draw_Default;
#if RETRO_USE_PLUS
        if (globals->gameMode < MODE_TIMEATTACK && API_ControllerIDForInputID(CONT_P2) == CONT_AUTOASSIGN)
            API_AssignControllerID(CONT_P2, CONT_ANY);
#endif
    }

    StateMachine_Run(self->state);
}

void PauseMenu_State_StartPause(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = false;
        UIControl_SetMenuLostFocus((EntityUIControl *)self->manager);
    }

    if (self->timer >= 8) {
        self->headerPos.x = 0;
        self->headerPos.y = 0;
        self->yellowTrianglePos.x = 0;
        self->yellowTrianglePos.y = 0;
        self->timer      = 0;
        self->state      = PauseMenu_State_Paused;
    }
    else {
        Vector2 pos;

        int32 val = 32 * self->timer;
        MathHelpers_Lerp2Sin1024(&pos, maxVal(0, val), -0xF00000, 0, 0, 0);

        self->headerPos.x = pos.x;
        self->headerPos.y = pos.y;
        MathHelpers_Lerp2Sin1024(&pos, maxVal(0, val), 0xE80000, 0, 0, 0);

        ++self->timer;
        self->yellowTrianglePos.x = pos.x;
        self->yellowTrianglePos.y = pos.y;
        self->lookupAlpha   = val;
    }
}

void PauseMenu_State_StartPauseCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = false;
        UIControl_SetMenuLostFocus((EntityUIControl *)self->manager);
    }

    if (self->timer >= 8) {
        self->headerPos.x = 0x000000;
        self->headerPos.y = 0x000000;
        self->yellowTrianglePos.x = 0x000000;
        self->yellowTrianglePos.y = 0x000000;
        if (self->timer >= 16) {
            self->paused = false;
            self->timer  = 0;
            self->state  = PauseMenu_State_Paused;
        }
        else {
            int32 t      = self->timer - 8;
            self->paused = true;
            if (self->timer == 8) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++self->timer;
            self->fadeTimer = (8 - t) << 6;
        }
    }
    else {
        self->headerPos.x = 0xFFF0000;
        self->headerPos.y = 0xFFF0000;
        self->yellowTrianglePos.x = 0xFFF0000;
        self->yellowTrianglePos.y = 0xFFF0000;
        self->paused     = true;
        self->fadeTimer  = self->timer << 6;
        self->timer      = self->timer + 1;
    }
}

void PauseMenu_State_Paused(void)
{
    RSDK_THIS(PauseMenu);
    self->lookupAlpha   = 255;
    self->headerPos.x = 0;
    self->headerPos.y = 0;
    self->yellowTrianglePos.x = 0;
    self->yellowTrianglePos.y = 0;

    EntityUIControl *manager = (EntityUIControl *)self->manager;
    if (Unknown_pausePress && !manager->dialogHasFocus) {
        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo"))
            pauseMenu->state = PauseMenu_State_Resume;
        else
            pauseMenu->state = PauseMenu_State_ResumeCompetition;
    }
}

void PauseMenu_State_ForcedPause(void)
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
            UIDialog_AddButton(4, dialog, PauseMenu_CheckAndReassignControllers, 0);
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
            UIDialog_AddButton(2, dialog, PauseMenu_State_SetupTitleFade, 1);
            UIDialog_Setup(dialog);
        }
#else
        else if (PauseMenu->signoutDetected) {
            Localization_GetString(&textBuffer, STR_SIGNOUTDETECTED);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(2, dialog, PauseMenu_State_SetupTitleFade, 1);
            UIDialog_Setup(dialog);
        }
#endif
    }

    ++self->timer;
    if (!UIDialog->activeDialog) {
        if (self->forcePaused) {
            if (globals->gameMode != MODE_COMPETITION || RSDK.CheckStageFolder("Puyo")) {
                RSDK.SetGameMode(ENGINESTATE_REGULAR);
                PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
                PauseMenu_ResumeSound();
            }
            else {
                self->timer = 0;
                self->state = PauseMenu_State_ForcedResumeCompetition;
            }
        }
    }

    if (!self->forcePaused && self->disconnectCheck) {
        if (self->disconnectCheck()) {
            if (PauseMenu->controllerDisconnect)
                PauseMenu->controllerDisconnect = false;

            EntityUIDialog *dialog = UIDialog->activeDialog;
            if (dialog) {
                UIDialog_CloseOnSel_HandleSelection(dialog, StateMachine_None);
            }
            self->forcePaused = true;
        }
    }
}

void PauseMenu_State_ForcedPauseCompetition(void)
{
    RSDK_THIS(PauseMenu);
    if (self->timer == 1) {
        UIControl->inputLocked = 0;
    }

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->paused    = false;
            self->fadeTimer = 0;
            self->timer     = 0;
            self->state     = PauseMenu_State_ForcedPause;
        }
        else {
            int32 t      = self->timer - 8;
            self->paused = true;
            if (self->timer == 8) {
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++self->timer;
            self->fadeTimer = (8 - t) << 6;
        }
    }
    else {
        self->paused    = true;
        self->fadeTimer = self->timer << 6;
        self->timer++;
    }
}

void PauseMenu_State_Resume(void)
{
    RSDK_THIS(PauseMenu);
    if (!self->timer && globals->gameMode < MODE_TIMEATTACK && !API_ControllerIDForInputID(2))
        API_AssignControllerID(CONT_P2, CONT_AUTOASSIGN);

    if (self->timer >= 8) {
        self->headerPos.y = 0;
        self->headerPos.x = -0xF00000;
        self->yellowTrianglePos.y = 0;
        self->yellowTrianglePos.x = 0xE80000;
        self->timer      = 0;
        RSDK.SetGameMode(ENGINESTATE_REGULAR);
        PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
        PauseMenu_ResumeSound();
    }
    else {
        Vector2 pos;

        int32 val = 32 * self->timer;
        MathHelpers_Lerp2Sin1024(&pos, maxVal(0, val), 0, 0, -0xF00000, 0);

        self->headerPos.x = pos.x;
        self->headerPos.y = pos.y;
        MathHelpers_Lerp2Sin1024(&pos, maxVal(0, val), 0, 0, 0xE80000, 0);

        self->yellowTrianglePos.x = pos.x;
        self->yellowTrianglePos.y = pos.y;
        self->lookupAlpha   = ((0x100 - self->timer) << 8) / 8;
        ++self->timer;
    }
}

void PauseMenu_State_ResumeCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = 0;
        UIControl_SetMenuLostFocus((EntityUIControl *)self->manager);
    }

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->paused = false;
            self->timer  = 0;
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
            self->fadeTimer = (8 - t) << 6;
        }
    }
    else {
        self->headerPos.x = 0;
        self->headerPos.y = 0;
        self->yellowTrianglePos.x = 0;
        self->yellowTrianglePos.y = 0;
        self->paused     = true;
        self->fadeTimer  = self->timer << 6;
        self->timer      = self->timer + 1;
    }
}

void PauseMenu_State_ForcedResumeCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->paused = false;
            self->timer  = 0;
            RSDK.SetGameMode(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int32 t      = self->timer - 8;
            self->paused = true;
            if (self->timer == 8) {
                EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
                RSDK.SetSettingsValue(SETTINGS_SCREENCOUNT, session->playerCount);
                PauseMenu_UpdateCameras();
            }
            ++self->timer;
            self->fadeTimer = (8 - t) << 6;
        }
    }
    else {
        self->paused    = true;
        self->fadeTimer = self->timer << 6;
        self->timer++;
    }
}

void PauseMenu_State_SetupTitleFade(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    pauseMenu->timer           = 0;
    pauseMenu->fadeTimer       = 0;
    pauseMenu->state           = PauseMenu_State_FadeToTitle;
}

void PauseMenu_State_FadeToTitle(void)
{
    RSDK_THIS(PauseMenu);
    if (!UIDialog->activeDialog) {
        if (!self->timer) {
            self->paused = true;
            Music_FadeOut(0.2);
        }

        if (self->timer >= 60) {
            self->fadeTimer = 512;
            SaveGame_ClearRestartData();
            RSDK.SetScene("Presentation", "Title Screen");
            RSDK.LoadScene();
        }
        else {
            self->fadeTimer = (self->timer << 9) / 60;
            self->timer++;
        }
    }
}

void PauseMenu_State_FadeToCB(void)
{
    RSDK_THIS(PauseMenu);
    self->fadeTimer += 12;
    self->paused = true;

    if (self->fadeTimer >= 1024) {
        if (self->fadeCB)
            self->fadeCB();
    }
}


void PauseMenu_DrawPauseQuads(void)
{
    RSDK_THIS(PauseMenu);
    Vector2 drawPos;

    drawPos.x = self->position.x + 0x640000 + self->headerPos.x + -0x10000 * ScreenInfo->centerX;
    drawPos.y = self->position.y - 0x600000 + self->headerPos.y;
    UIWidgets_DrawParallelogram(68, 200, 68, 232, 40, 88, drawPos.x, drawPos.y);
    drawPos.y += 0x60000;
    drawPos.x += 0xA0000;
    UIWidgets_DrawParallelogram(24, 115, 24, 0, 0, 0, drawPos.x, drawPos.y);
    RSDK.DrawSprite(&self->animator, &drawPos, 0);
    UIWidgets_DrawRightTriangle(self->yellowTrianglePos.x + (ScreenInfo->centerX << 16) + self->position.x, self->yellowTrianglePos.y + (ScreenInfo->centerY << 16) + self->position.y, -232, 240, 216, 8);
}

void PauseMenu_Draw_Default(void)
{
    RSDK_THIS(PauseMenu);
    if (self->state != PauseMenu_State_FadeToCB) {
#if RETRO_USE_PLUS
        RSDK.SetLookupTable(PauseMenu->lookupTable);
#endif
        RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, self->lookupAlpha, INK_LOOKUP, true);
        PauseMenu_DrawPauseQuads();
    }
}

void PauseMenu_Draw_JustLookup(void)
{
    RSDK_THIS(PauseMenu);
    if (self->state != PauseMenu_State_FadeToCB) {
#if RETRO_USE_PLUS
        RSDK.SetLookupTable(PauseMenu->lookupTable);
#endif
        RSDK.DrawRect(0, 0, ScreenInfo->width, ScreenInfo->height, 0, self->lookupAlpha, INK_LOOKUP, true);
    }
}

#if RETRO_INCLUDE_EDITOR
void PauseMenu_EditorDraw(void) {}

void PauseMenu_EditorLoad(void) {}
#endif

void PauseMenu_Serialize(void) {}
