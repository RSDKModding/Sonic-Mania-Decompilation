// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PauseMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPauseMenu *PauseMenu;

void PauseMenu_Update(void)
{
    RSDK_THIS(PauseMenu);

    StateMachine_Run(self->state);

    self->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    self->position.y = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

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
        if (Music_IsPlaying())
            Music_Pause();
    }
    else {
        StateMachine(state) = RSDK_GET_ENTITY(self->triggerPlayer, Player)->state;

        if (state == Player_State_Death || state == Player_State_Drown) {
            destroyEntity(self);
        }
        else {
            self->visible   = true;
            self->drawGroup = DRAWGROUP_COUNT - 1;
            RSDK.SetEngineState(ENGINESTATE_FROZEN);
            RSDK.SetSpriteAnimation(UIWidgets->textFrames, 10, &self->animator, true, 3);
            PauseMenu_PauseSound();
#if !MANIA_USE_PLUS
            PauseMenu_SetupTintTable();
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
            cnt = RSDK.GetEntityCount(TitleCard->classID, true);
        if (ActClear)
            cnt += RSDK.GetEntityCount(ActClear->classID, true);

        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

        bool32 allowEvents = true;
        if (Zone)
            allowEvents = Zone->timer > 1;

        if (!cnt && pauseMenu->classID == TYPE_BLANK && allowEvents && !PauseMenu->disableEvents) {
            if (API_GetUserAuthStatus() == STATUS_FORBIDDEN) {
                PauseMenu->signOutDetected = true;
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                pauseMenu->triggerPlayer = 0;
            }
#if MANIA_USE_PLUS
            else if (API.CheckDLC(DLC_PLUS) != (bool32)globals->lastHasPlus) {
                PauseMenu->plusChanged = true;
                globals->lastHasPlus   = API.CheckDLC(DLC_PLUS);
                RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                pauseMenu->triggerPlayer = 0;
            }
#endif
            else {
                for (int32 i = 0; i < PauseMenu_GetPlayerCount(); ++i) {
#if MANIA_USE_PLUS
                    int32 id = API_GetInputDeviceID(CONT_P1 + i);
                    if (!RSDK.IsInputDeviceAssigned(id) && id != INPUT_AUTOASSIGN) {
                        PauseMenu->controllerDisconnect = true;
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
                        pauseMenu->triggerPlayer = i;
                    }
#else
                    if (!API_IsInputSlotAssigned(CONT_P1 + i)) {
                        PauseMenu->controllerDisconnect = true;
                        RSDK.ResetEntitySlot(SLOT_PAUSEMENU, PauseMenu->classID, NULL);
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
        RSDK.FillScreen(0x000000, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);

    if (RSDK.GetVideoSetting(VIDEOSETTING_SCREENCOUNT) <= 1) {
        StateMachine_Run(self->stateDraw);
    }
}

void PauseMenu_Create(void *data)
{
    RSDK_THIS(PauseMenu);

    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_ALWAYS;

        if (data == INT_TO_VOID(true)) {
            self->visible   = true;
            self->drawGroup = DRAWGROUP_COUNT - 1;
            self->state     = PauseMenu_State_HandleFadeout;
        }
        else {
            self->state     = StateMachine_None;
            self->stateDraw = StateMachine_None;
        }
    }
}

void PauseMenu_StageLoad(void)
{
    PauseMenu->active = ACTIVE_ALWAYS;

    PauseMenu->sfxBleep  = RSDK.GetSfx("Global/MenuBleep.wav");
    PauseMenu->sfxAccept = RSDK.GetSfx("Global/MenuAccept.wav");

    PauseMenu->disableEvents        = false;
    PauseMenu->controllerDisconnect = false;
    PauseMenu->forcedDisconnect     = false;
    PauseMenu->signOutDetected      = false;
#if MANIA_USE_PLUS
    PauseMenu->plusChanged = false;

    if (!globals->hasPlusInitial) {
        globals->lastHasPlus    = API.CheckDLC(DLC_PLUS);
        globals->hasPlusInitial = true;
    }
#endif

    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        PauseMenu->activeChannels[i] = false;
    }

#if MANIA_USE_PLUS
    PauseMenu_SetupTintTable();
#endif
}

void PauseMenu_SetupMenu(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 size;
    size.x = ScreenInfo->size.x << 16;
    size.y = ScreenInfo->size.y << 16;
    RSDK.ResetEntitySlot(SLOT_PAUSEMENU_UICONTROL, UIControl->classID, &size);

    EntityUIControl *control = RSDK_GET_ENTITY(SLOT_PAUSEMENU_UICONTROL, UIControl);

    control->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    control->position.y = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

    // Bug Details:
    // control->rowCount is slightly bugged, if `pauseMenu->disableRestart` is enabled then wrapping by pressing down is broken and wont work
    // this is due to rowCount being 3, while control->buttonCount is only 2
    // Fix:
    // set control->rowCount to control->buttonCount once it's been initialized, instead of using a constant value
    control->rowCount    = PAUSEMENU_BUTTON_COUNT;
    control->columnCount = 1;
    control->buttonID    = 0;
    self->manager        = control;

    int32 i = 0;
    for (; i < PAUSEMENU_BUTTON_COUNT; ++i) {
        if (!self->buttonPtrs[i])
            break;
        EntityUIButton *button = self->buttonPtrs[i];
        button->parent         = (Entity *)control;
        control->buttons[i]    = button;
    }
    control->buttonCount = i;
}

void PauseMenu_SetupTintTable(void)
{
#if MANIA_USE_PLUS
    for (int32 i = 0; i < 0x10000; ++i) {
        uint32 r = (0x20F * (i >> 11) + 23) >> 6;
        uint32 g = (0x103 * ((i >> 5) & 0x3F) + 33) >> 6;
        uint32 b = (0x20F * (i & 0x1F) + 23) >> 6;

        int32 brightness = MIN(((b + g + r) << 8) / 680, 0xFF);

        PauseMenu->tintLookupTable[i] = ColorHelpers_PackRGB(brightness, brightness, brightness);
    }
#else
    uint16 *tintLookupTable = RSDK.GetTintLookupTable();
    for (int32 i = 0; i < 0x10000; ++i) {
        uint32 r = (0x20F * (i >> 11) + 23) >> 6;
        uint32 g = (0x103 * ((i >> 5) & 0x3F) + 33) >> 6;
        uint32 b = (0x20F * (i & 0x1F) + 23) >> 7;

        uint32 hue = 0, saturation = 0, luminance = 0;
        ColorHelpers_RGBToHSL(r, g, b, &hue, &saturation, &luminance);
        ColorHelpers_HSLToRGB(hue, 0, MIN(13 * luminance / 16, 255), &r, &g, &b);

        tintLookupTable[i] = ColorHelpers_PackRGB(r, g, b);
    }
#endif
}

void PauseMenu_AddButton(uint8 id, void *action)
{
    RSDK_THIS(PauseMenu);

    int32 buttonID = self->buttonCount;
    if (buttonID < PAUSEMENU_BUTTON_COUNT) {
        self->buttonIDs[buttonID]     = id;
        self->buttonActions[buttonID] = action;

        int32 buttonSlot = self->buttonCount + 18;
        RSDK.ResetEntitySlot(buttonSlot, UIButton->classID, NULL);
        EntityUIButton *button = RSDK_GET_ENTITY(buttonSlot, UIButton);

        button->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        button->position.y = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 10, &button->animator, true, id);
        button->actionCB           = PauseMenu_ActionCB_Button;
        button->size.x             = 60 << 16;
        button->size.y             = 21 << 16;
        button->bgEdgeSize         = 21;
        button->align              = ALIGN_LEFT;
        button->drawGroup          = self->drawGroup;
        button->active             = ACTIVE_ALWAYS;
        self->buttonPtrs[buttonID] = button;
        ++self->buttonCount;
    }
}

void PauseMenu_ClearButtons(EntityPauseMenu *entity)
{
    if (entity->manager)
        destroyEntity(entity->manager);

    for (int32 i = 0; i < PAUSEMENU_BUTTON_COUNT; ++i) {
        if (entity->buttonPtrs[i])
            destroyEntity(entity->buttonPtrs[i]);
    }

    destroyEntity(entity);
}

void PauseMenu_HandleButtonPositions(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 pos;
    pos.x = TO_FIXED(ScreenInfo->center.x - 69) + self->position.x + self->yellowTrianglePos.x;
    pos.y = (self->position.y + TO_FIXED(56)) + self->yellowTrianglePos.y - TO_FIXED(36);
    if (self->buttonCount == (PAUSEMENU_BUTTON_COUNT - 1)) {
        pos.x -= TO_FIXED(36);
        pos.y += TO_FIXED(36);
    }

    for (int32 i = 0; i < self->buttonCount; ++i) {
        if (!self->buttonPtrs[i])
            break;

        EntityUIButton *button = self->buttonPtrs[i];
        button->startPos.x     = pos.x;
        button->startPos.y     = pos.y;
        button->position.x     = pos.x;
        button->position.y     = pos.y;
        pos.x -= TO_FIXED(36);
        pos.y += TO_FIXED(36);
    }
}

void PauseMenu_PauseSound(void)
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        if (RSDK.ChannelActive(i)) {
            RSDK.PauseChannel(i);
            PauseMenu->activeChannels[i] = true;
        }
    }
}

void PauseMenu_ResumeSound(void)
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        if (PauseMenu->activeChannels[i]) {
            RSDK.ResumeChannel(i);
            PauseMenu->activeChannels[i] = false;
        }
    }
}

void PauseMenu_StopSound(void)
{
    for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
        if (PauseMenu->activeChannels[i]) {
            RSDK.StopChannel(i);
            PauseMenu->activeChannels[i] = false;
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
        int32 id         = RSDK.GetEntitySlot(cameraPtr);
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
        camera->screenID = RSDK.GetEntitySlot(camera) - SLOT_CAMERA1;
        Camera_SetCameraBounds(camera);
    }
}

void PauseMenu_CheckAndReassignControllers(void)
{
    EntityPauseMenu *entity = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    // prolly a leftover from pre-plus
    int32 deviceID = API_GetInputDeviceID(CONT_P1 + (entity->triggerPlayer ^ 1));

#if MANIA_USE_PLUS
    UNUSED(deviceID); // be quiet compiler I know it aint used!!

    int32 id = API_GetFilteredInputDeviceID(true, true, 5);
#else
    int32 id = API_GetFilteredInputDeviceID(deviceID);
#endif

    if (id)
        API_AssignInputSlotToDevice(CONT_P1 + entity->triggerPlayer, id);
    else
        API_AssignInputSlotToDevice(CONT_P1 + entity->triggerPlayer, INPUT_AUTOASSIGN);

    if (globals->gameMode < MODE_TIMEATTACK && !API_GetInputDeviceID(CONT_P2))
        API_AssignInputSlotToDevice(CONT_P2, INPUT_AUTOASSIGN);

    PauseMenu->forcedDisconnect = true;
}

bool32 PauseMenu_IsDisconnected(void)
{
    RSDK_THIS(PauseMenu);

    int32 id = API_GetInputDeviceID(CONT_P1 + self->triggerPlayer);

#if MANIA_USE_PLUS
    return API_IsInputDeviceAssigned(id) || PauseMenu->forcedDisconnect;
#else
    return id != INPUT_NONE || PauseMenu->forcedDisconnect;
#endif
}

uint8 PauseMenu_GetPlayerCount(void)
{
    EntityMenuParam *param            = MenuParam_GetParam();
    EntityCompetitionSession *session = CompetitionSession_GetSession();

    if (RSDK.CheckSceneFolder("Puyo")) {
        if (param->puyoSelection >= PUYO_SELECTION_VS_2P)
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

    if (globals->gameMode != MODE_COMPETITION || RSDK.CheckSceneFolder("Puyo"))
        pauseMenu->state = PauseMenu_State_Resume;
    else
        pauseMenu->state = PauseMenu_State_ResumeCompetition;
}

void PauseMenu_RestartButtonCB(void)
{
    RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    String msg;
#if MANIA_USE_PLUS
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
    RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    String msg;
#if MANIA_USE_PLUS
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
    RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    UIDialog->activeDialog->parent->state = NULL;
    if (StarPost) {
        StarPost->postIDs[0] = 0;
        StarPost->postIDs[1] = 0;
        StarPost->postIDs[2] = 0;
        StarPost->postIDs[3] = 0;
    }
    Music_Stop();

    int32 x                  = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 y                  = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
    EntityPauseMenu *fadeout = CREATE_ENTITY(PauseMenu, INT_TO_VOID(true), x, y);
    fadeout->fadeoutCB       = PauseMenu_RestartFadeCB;
    fadeout->state           = PauseMenu_State_HandleFadeout;
}

void PauseMenu_ExitDialog_YesCB(void)
{
    RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    UIDialog->activeDialog->parent->state = NULL;
    globals->recallEntities               = false;
    globals->initCoolBonus                = false;

    if (StarPost) {
        StarPost->postIDs[0] = 0;
        StarPost->postIDs[1] = 0;
        StarPost->postIDs[2] = 0;
        StarPost->postIDs[3] = 0;
    }
    Music_Stop();

    int32 x                  = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 y                  = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
    EntityPauseMenu *fadeout = CREATE_ENTITY(PauseMenu, INT_TO_VOID(true), x, y);
    fadeout->fadeoutCB       = PauseMenu_ExitFadeCB;
    fadeout->state           = PauseMenu_State_HandleFadeout;
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
        EntityCompetitionSession *session = CompetitionSession_GetSession();
        EntityMenuParam *param            = MenuParam_GetParam();

        sprintf_s(param->menuTag, (int32)sizeof(param->menuTag), "Competition Zones");
        param->menuSelection = session->stageIndex;
    }

    RSDK.SetScene("Presentation", "Menu");
    PauseMenu_StopSound();
    RSDK.LoadScene();
}

void PauseMenu_ActionCB_Button(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
    EntityUIControl *manager   = pauseMenu->manager;

    if (manager->buttonID >= 0 && manager->buttonID < manager->buttonCount) {
        StateMachine_Run(pauseMenu->buttonActions[manager->buttonID]);
    }
}

void PauseMenu_State_SetupButtons(void)
{
    RSDK_THIS(PauseMenu);

    self->timer                 = 0;
    PauseMenu->forcedDisconnect = false;

#if MANIA_USE_PLUS
    if (PauseMenu->controllerDisconnect || PauseMenu->signOutDetected || PauseMenu->plusChanged) {
#else
    if (PauseMenu->controllerDisconnect || PauseMenu->signOutDetected) {
#endif
        if (PauseMenu->controllerDisconnect)
            self->disconnectCheck = PauseMenu_IsDisconnected;

        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckSceneFolder("Puyo"))
            self->state = PauseMenu_State_ForcedPause;
        else
            self->state = PauseMenu_State_ForcedPauseCompetition;

        self->stateDraw = PauseMenu_Draw_ForcePause;
    }
    else {
        RSDK.PlaySfx(PauseMenu->sfxAccept, false, 255);

        PauseMenu_AddButton(0, PauseMenu_ResumeButtonCB);

        if (!self->disableRestart)
            PauseMenu_AddButton(1, PauseMenu_RestartButtonCB);

        PauseMenu_AddButton(2, PauseMenu_ExitButtonCB);

        PauseMenu_HandleButtonPositions();
        PauseMenu_SetupMenu();

        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckSceneFolder("Puyo"))
            self->state = PauseMenu_State_StartPause;
        else
            self->state = PauseMenu_State_StartPauseCompetition;
        self->stateDraw = PauseMenu_Draw_RegularPause;

#if MANIA_USE_PLUS
        if (globals->gameMode < MODE_TIMEATTACK && API_GetInputDeviceID(CONT_P2) == (uint32)INPUT_AUTOASSIGN)
            API_AssignInputSlotToDevice(CONT_P2, INPUT_NONE);
#endif
    }

    StateMachine_Run(self->state);
}

void PauseMenu_State_StartPause(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = false;
        UIControl_SetMenuLostFocus(self->manager);
    }

    if (self->timer >= 8) {
        self->headerPos.x         = 0;
        self->headerPos.y         = 0;
        self->yellowTrianglePos.x = 0;
        self->yellowTrianglePos.y = 0;
        self->timer               = 0;
        self->state               = PauseMenu_State_Paused;
    }
    else {
        Vector2 pos;

        int32 alpha = 32 * self->timer;
        MathHelpers_Lerp2Sin1024(&pos, MAX(0, alpha), -TO_FIXED(240), 0, 0, 0);

        self->headerPos.x = pos.x;
        self->headerPos.y = pos.y;
        MathHelpers_Lerp2Sin1024(&pos, MAX(0, alpha), TO_FIXED(232), 0, 0, 0);

        ++self->timer;
        self->yellowTrianglePos.x = pos.x;
        self->yellowTrianglePos.y = pos.y;
        self->tintAlpha           = alpha;
    }
}

void PauseMenu_State_StartPauseCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = false;
        UIControl_SetMenuLostFocus(self->manager);
    }

    if (self->timer >= 8) {
        self->headerPos.x         = TO_FIXED(0);
        self->headerPos.y         = TO_FIXED(0);
        self->yellowTrianglePos.x = TO_FIXED(0);
        self->yellowTrianglePos.y = TO_FIXED(0);
        if (self->timer >= 16) {
            self->paused = false;
            self->timer  = 0;
            self->state  = PauseMenu_State_Paused;
        }
        else {
            int32 t      = self->timer - 8;
            self->paused = true;
            if (self->timer == 8) {
                RSDK.SetVideoSetting(VIDEOSETTING_SCREENCOUNT, 1);
                PauseMenu_FocusCamera();
            }
            ++self->timer;
            self->fadeTimer = (8 - t) << 6;
        }
    }
    else {
        self->headerPos.x         = TO_FIXED(0xFFF);
        self->headerPos.y         = TO_FIXED(0xFFF);
        self->yellowTrianglePos.x = TO_FIXED(0xFFF);
        self->yellowTrianglePos.y = TO_FIXED(0xFFF);
        self->paused              = true;
        self->fadeTimer           = self->timer << 6;
        self->timer               = self->timer + 1;
    }
}

void PauseMenu_State_Paused(void)
{
    RSDK_THIS(PauseMenu);

    self->tintAlpha           = 0xFF;
    self->headerPos.x         = 0;
    self->headerPos.y         = 0;
    self->yellowTrianglePos.x = 0;
    self->yellowTrianglePos.y = 0;

    EntityUIControl *manager = self->manager;
    if (Unknown_pausePress && !manager->dialogHasFocus) {
        EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);
        if (globals->gameMode != MODE_COMPETITION || RSDK.CheckSceneFolder("Puyo"))
            pauseMenu->state = PauseMenu_State_Resume;
        else
            pauseMenu->state = PauseMenu_State_ResumeCompetition;
    }
}

void PauseMenu_State_ForcedPause(void)
{
    RSDK_THIS(PauseMenu);
    String textBuffer;

    if (self->timer == 1) {
        UIControl->inputLocked = false;
        if (PauseMenu->controllerDisconnect) {
            int32 strID = STR_RECONNECTWIRELESSCONTROLLER;
            if (sku_platform == PLATFORM_SWITCH)
                strID = STR_RECONNECTCONTROLLER;
            Localization_GetString(&textBuffer, strID);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(DIALOG_CONTINUE, dialog, PauseMenu_CheckAndReassignControllers, 0);
            UIDialog_Setup(dialog);

            if (globals->gameMode < MODE_TIMEATTACK && API_GetInputDeviceID(2) == (uint32)INPUT_AUTOASSIGN)
                API_AssignInputSlotToDevice(CONT_P2, INPUT_NONE);
        }
#if MANIA_USE_PLUS
        else if (PauseMenu->signOutDetected || PauseMenu->plusChanged) {
            int32 strID = STR_TESTSTR;
            if (PauseMenu->signOutDetected)
                strID = STR_SIGNOUTDETECTED;
            else if (PauseMenu->plusChanged)
                strID = STR_RETRURNINGTOTITLE;

            Localization_GetString(&textBuffer, strID);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(DIALOG_OK, dialog, PauseMenu_State_SetupTitleFade, 1);
            UIDialog_Setup(dialog);
        }
#else
        else if (PauseMenu->signOutDetected) {
            Localization_GetString(&textBuffer, STR_SIGNOUTDETECTED);

            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&textBuffer);
            UIDialog_AddButton(DIALOG_OK, dialog, PauseMenu_State_SetupTitleFade, 1);
            UIDialog_Setup(dialog);
        }
#endif
    }

    ++self->timer;
    if (!UIDialog->activeDialog) {
        if (self->forcePaused) {
            if (globals->gameMode != MODE_COMPETITION || RSDK.CheckSceneFolder("Puyo")) {
                RSDK.SetEngineState(ENGINESTATE_REGULAR);
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
            if (dialog)
                UIDialog_CloseOnSel_HandleSelection(dialog, StateMachine_None);

            self->forcePaused = true;
        }
    }
}

void PauseMenu_State_ForcedPauseCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1)
        UIControl->inputLocked = 0;

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
                RSDK.SetVideoSetting(VIDEOSETTING_SCREENCOUNT, 1);
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

    if (!self->timer && globals->gameMode < MODE_TIMEATTACK && !API_GetInputDeviceID(CONT_P2))
        API_AssignInputSlotToDevice(CONT_P2, INPUT_AUTOASSIGN);

    if (self->timer >= 8) {
        self->headerPos.x         = -TO_FIXED(240);
        self->headerPos.y         = 0;
        self->yellowTrianglePos.x = TO_FIXED(232);
        self->yellowTrianglePos.y = 0;
        self->timer               = 0;
        RSDK.SetEngineState(ENGINESTATE_REGULAR);
        PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
        PauseMenu_ResumeSound();
    }
    else {
        Vector2 pos;

        int32 percent = 0x20 * self->timer;
        MathHelpers_Lerp2Sin1024(&pos, MAX(0, percent), 0, 0, -TO_FIXED(240), 0);

        self->headerPos.x = pos.x;
        self->headerPos.y = pos.y;
        MathHelpers_Lerp2Sin1024(&pos, MAX(0, percent), 0, 0, TO_FIXED(232), 0);

        self->yellowTrianglePos.x = pos.x;
        self->yellowTrianglePos.y = pos.y;
        self->tintAlpha           = ((0x100 - self->timer) << 8) / 8;
        ++self->timer;
    }
}

void PauseMenu_State_ResumeCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer == 1) {
        UIControl->inputLocked = 0;
        UIControl_SetMenuLostFocus(self->manager);
    }

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->paused = false;
            self->timer  = 0;
            RSDK.SetEngineState(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int32 t = self->timer - 8;

            if (self->timer == 8) {
                EntityCompetitionSession *session = CompetitionSession_GetSession();
                RSDK.SetVideoSetting(VIDEOSETTING_SCREENCOUNT, session->playerCount);
                PauseMenu_UpdateCameras();
            }

            ++self->timer;
            self->fadeTimer = (8 - t) << 6;
        }
    }
    else {
        self->headerPos.x         = 0;
        self->headerPos.y         = 0;
        self->yellowTrianglePos.x = 0;
        self->yellowTrianglePos.y = 0;
        self->paused              = true;
        self->fadeTimer           = self->timer << 6;
        self->timer               = self->timer + 1;
    }
}

void PauseMenu_State_ForcedResumeCompetition(void)
{
    RSDK_THIS(PauseMenu);

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            self->paused = false;
            self->timer  = 0;
            RSDK.SetEngineState(ENGINESTATE_REGULAR);
            PauseMenu_ClearButtons(RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu));
            PauseMenu_ResumeSound();
        }
        else {
            int32 t      = self->timer - 8;
            self->paused = true;

            if (self->timer == 8) {
                EntityCompetitionSession *session = CompetitionSession_GetSession();
                RSDK.SetVideoSetting(VIDEOSETTING_SCREENCOUNT, session->playerCount);
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

void PauseMenu_State_HandleFadeout(void)
{
    RSDK_THIS(PauseMenu);

    self->fadeTimer += 12;
    self->paused = true;

    if (self->fadeTimer >= 1024) {
        StateMachine_Run(self->fadeoutCB);
    }
}

void PauseMenu_DrawPauseMenu(void)
{
    RSDK_THIS(PauseMenu);

    Vector2 drawPos;
    drawPos.x = self->position.x + TO_FIXED(100) + self->headerPos.x + -TO_FIXED(1) * ScreenInfo->center.x;
    drawPos.y = self->position.y - TO_FIXED(96) + self->headerPos.y;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 200, 68, 68, 0xE8, 0x28, 0x58);

    drawPos.x += TO_FIXED(10);
    drawPos.y += TO_FIXED(6);
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 115, 24, 24, 0x00, 0x00, 0x00);

    // "PAUSED" text
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    UIWidgets_DrawRightTriangle(self->yellowTrianglePos.x + TO_FIXED(ScreenInfo->center.x) + self->position.x,
                                self->yellowTrianglePos.y + TO_FIXED(ScreenInfo->center.y) + self->position.y, -232, 0xF0, 0xD8, 0x08);
}

void PauseMenu_Draw_RegularPause(void)
{
    RSDK_THIS(PauseMenu);

    if (self->state != PauseMenu_State_HandleFadeout) {
#if MANIA_USE_PLUS
        RSDK.SetTintLookupTable(PauseMenu->tintLookupTable);
#endif
        RSDK.DrawRect(0, 0, ScreenInfo->size.x, ScreenInfo->size.y, 0, self->tintAlpha, INK_TINT, true);

        PauseMenu_DrawPauseMenu();
    }
}

void PauseMenu_Draw_ForcePause(void)
{
    RSDK_THIS(PauseMenu);

    if (self->state != PauseMenu_State_HandleFadeout) {
#if MANIA_USE_PLUS
        RSDK.SetTintLookupTable(PauseMenu->tintLookupTable);
#endif
        RSDK.DrawRect(0, 0, ScreenInfo->size.x, ScreenInfo->size.y, 0, self->tintAlpha, INK_TINT, true);
    }
}

#if GAME_INCLUDE_EDITOR
void PauseMenu_EditorDraw(void) {}

void PauseMenu_EditorLoad(void) {}
#endif

void PauseMenu_Serialize(void) {}
