// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIControl Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIControl *UIControl;

void UIControl_Update(void)
{
    RSDK_THIS(UIControl);

    if (self->buttonID >= 0 && self->buttonID != self->lastButtonID)
        self->lastButtonID = self->buttonID;

    if (!UIControl->hasTouchInput && self->buttonID == -1)
        self->buttonID = self->lastButtonID;

    StateMachine_Run(self->state);

    if (self->backoutTimer > 0)
        self->backoutTimer--;

    StateMachine_Run(self->menuUpdateCB);
}

void UIControl_LateUpdate(void) {}

void UIControl_StaticUpdate(void)
{
    if (UIControl->lockInput) {
        UIControl->lockInput   = false;
        UIControl->inputLocked = true;
    }
    else {
        UIControl->inputLocked = false;
    }

    UIControl->forceBackPress = false;

    ++UIControl->timer;
    UIControl->timer &= 0x7FFF;
}

void UIControl_Draw(void)
{
    RSDK_THIS(UIControl);

    ScreenInfo->position.x = FROM_FIXED(self->position.x) - ScreenInfo->center.x;
    ScreenInfo->position.y = FROM_FIXED(self->position.y) - ScreenInfo->center.y;
}

void UIControl_Create(void *data)
{
    RSDK_THIS(UIControl);

    if (!SceneInfo->inEditor) {
        if (data) {
            Vector2 *size = (Vector2 *)data;
            self->size.x  = size->x;
            self->size.y  = size->y;
        }

        self->updateRange.x = self->size.x >> 1;
        self->updateRange.y = self->size.y >> 1;
#if MANIA_USE_PLUS
        self->promptCount = 0;
#endif

        if (self->rowCount <= 1)
            self->rowCount = 1;

        if (self->columnCount <= 1)
            self->columnCount = 1;

        if (!self->hasStoredButton)
            self->buttonID = self->startingID;
        else
            self->buttonID = self->storedButtonID;

        self->position.x += self->cameraOffset.x;
        self->position.y += self->cameraOffset.y;
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;

#if MANIA_USE_PLUS
        int32 slotID = RSDK.GetEntitySlot(self);
        if (UIButtonPrompt && slotID != SLOT_DIALOG_UICONTROL) {
            foreach_all(UIButtonPrompt, prompt)
            {
                if (self->promptCount < UICONTROL_PROMPT_COUNT && UIControl_ContainsPos(self, &prompt->position)) {
                    prompt->parent                     = (Entity *)self;
                    self->prompts[self->promptCount++] = prompt;
                }
            }
        }
#endif

        AnalogStickInfoL[1].deadzone = 0.75f;
        AnalogStickInfoL[2].deadzone = 0.75f;
        AnalogStickInfoL[3].deadzone = 0.75f;
        AnalogStickInfoL[4].deadzone = 0.75f;

        UIControl_SetupButtons();

        if (self->noWidgets) {
            self->active  = ACTIVE_NORMAL;
            self->visible = true;
        }
        else {
#if MANIA_USE_PLUS
            self->menuWasSetup = false;
#endif

            if (self->activeOnLoad)
                UIControl_SetActiveMenu(self);
            else
                self->active = ACTIVE_NEVER;
        }
    }
}

void UIControl_StageLoad(void)
{
    UIControl->inputLocked       = false;
    UIControl->lockInput         = false;
    UIControl->active            = ACTIVE_ALWAYS;
    UIControl->isProcessingInput = false;
}

EntityUIControl *UIControl_GetUIControl(void)
{
    foreach_all(UIControl, control)
    {
        if (control->active == ACTIVE_ALWAYS) {
            foreach_return control;
        }
    }

    return NULL;
}

void UIControl_ClearInputs(uint8 buttonID)
{
    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        UIControl->upPress[i]      = false;
        UIControl->downPress[i]    = false;
        UIControl->leftPress[i]    = false;
        UIControl->rightPress[i]   = false;
        UIControl->yPress[i]       = false;
        UIControl->xPress[i]       = false;
        UIControl->backPress[i]    = false;
        UIControl->confirmPress[i] = false;
#if MANIA_USE_PLUS
        UIControl->startPress[i] = false;
#endif
    }

    UIControl->anyUpPress    = false;
    UIControl->anyDownPress  = false;
    UIControl->anyLeftPress  = false;
    UIControl->anyRightPress = false;
    UIControl->anyYPress     = buttonID == UIBUTTONPROMPT_BUTTON_Y;
    UIControl->anyXPress     = buttonID == UIBUTTONPROMPT_BUTTON_X;
#if MANIA_USE_PLUS
    UIControl->anyStartPress = buttonID == UIBUTTONPROMPT_BUTTON_SELECT;
#endif

    if (API_GetConfirmButtonFlip()) {
        UIControl->anyConfirmPress = buttonID == UIBUTTONPROMPT_BUTTON_B;
        UIControl->anyBackPress    = buttonID == UIBUTTONPROMPT_BUTTON_A;
        UIControl->forceBackPress  = buttonID == UIBUTTONPROMPT_BUTTON_A;
    }
    else {
        UIControl->anyConfirmPress = buttonID == UIBUTTONPROMPT_BUTTON_A;
        UIControl->anyBackPress    = buttonID == UIBUTTONPROMPT_BUTTON_B;
        UIControl->forceBackPress  = buttonID == UIBUTTONPROMPT_BUTTON_B;
    }

    UIControl->lockInput   = true;
    UIControl->inputLocked = true;
}

void UIControl_ProcessInputs(void)
{
    RSDK_THIS(UIControl);

    UIControl_HandlePosition();

    if (!UIControl->inputLocked) {
        for (int32 i = 0; i < PLAYER_COUNT; ++i) {
            UIControl->upPress[i]    = ControllerInfo[CONT_P1 + i].keyUp.press || AnalogStickInfoL[CONT_P1 + i].keyUp.press;
            UIControl->downPress[i]  = ControllerInfo[CONT_P1 + i].keyDown.press || AnalogStickInfoL[CONT_P1 + i].keyDown.press;
            UIControl->leftPress[i]  = ControllerInfo[CONT_P1 + i].keyLeft.press || AnalogStickInfoL[CONT_P1 + i].keyLeft.press;
            UIControl->rightPress[i] = ControllerInfo[CONT_P1 + i].keyRight.press || AnalogStickInfoL[CONT_P1 + i].keyRight.press;

            if (UIControl->upPress[i] && UIControl->downPress[i]) {
                UIControl->upPress[i]   = false;
                UIControl->downPress[i] = false;
            }

            if (UIControl->leftPress[i] && UIControl->rightPress[i]) {
                UIControl->leftPress[i]  = false;
                UIControl->rightPress[i] = false;
            }

            UIControl->yPress[i] = ControllerInfo[CONT_P1 + i].keyY.press;
            UIControl->xPress[i] = ControllerInfo[CONT_P1 + i].keyX.press;
#if MANIA_USE_PLUS
            UIControl->startPress[i] = ControllerInfo[CONT_P1 + i].keyStart.press;
#endif

            UIControl->confirmPress[i] = ControllerInfo[CONT_P1 + i].keyStart.press;
            if (API_GetConfirmButtonFlip()) {
                UIControl->confirmPress[i] |= ControllerInfo[CONT_P1 + i].keyB.press;
                UIControl->backPress[i] = ControllerInfo[CONT_P1 + i].keyA.press;
            }
            else {
                UIControl->confirmPress[i] |= ControllerInfo[CONT_P1 + i].keyA.press;
                UIControl->backPress[i] = ControllerInfo[CONT_P1 + i].keyB.press;
            }
        }

        UIControl->anyUpPress    = ControllerInfo->keyUp.press || AnalogStickInfoL->keyUp.press;
        UIControl->anyDownPress  = ControllerInfo->keyDown.press || AnalogStickInfoL->keyDown.press;
        UIControl->anyLeftPress  = ControllerInfo->keyLeft.press || AnalogStickInfoL->keyLeft.press;
        UIControl->anyRightPress = ControllerInfo->keyRight.press || AnalogStickInfoL->keyRight.press;
        UIControl->anyYPress     = ControllerInfo->keyY.press;
        UIControl->anyXPress     = ControllerInfo->keyX.press;
#if MANIA_USE_PLUS
        UIControl->anyStartPress = ControllerInfo->keyStart.press;
#endif

        UIControl->anyConfirmPress = ControllerInfo->keyStart.press;
        if (API_GetConfirmButtonFlip()) {
            UIControl->anyConfirmPress |= ControllerInfo->keyB.press;
            UIControl->anyBackPress = ControllerInfo->keyA.press;
        }
        else {
            UIControl->anyConfirmPress |= ControllerInfo->keyA.press;
            UIControl->anyBackPress = ControllerInfo->keyB.press;
        }

        UIControl->anyBackPress |= Unknown_pausePress;
        UIControl->anyBackPress |= UIControl->forceBackPress;

        if (UIControl->anyBackPress) {
            UIControl->anyConfirmPress = false;
            UIControl->anyYPress       = false;
        }

        if (UIControl->anyConfirmPress) {
            UIControl->anyYPress = false;
        }

        UIControl->inputLocked = true;
    }

    if (!self->selectionDisabled) {
        bool32 backPressed = false;

        if (UIControl->anyBackPress) {
            if (!self->childHasFocus && !self->dialogHasFocus
#if MANIA_USE_PLUS
                && !self->popoverHasFocus
#endif
                && self->backoutTimer <= 0) {
                if (self->backPressCB) {
                    backPressed = self->backPressCB();

                    if (!backPressed) {
                        UIControl->anyBackPress = false;
                    }
                    else {
                        if (self->buttons[self->buttonID])
                            self->buttons[self->buttonID]->isSelected = false;
                    }
                }
                else {
                    if (self->parentTag.length <= 0) {
                        UIControl->anyBackPress = false;
                    }
                    else {
                        self->selectionDisabled = true;
                        UITransition_StartTransition(UIControl_ReturnToParentMenu, 0);
                        backPressed = false;

                        if (self->buttons[self->buttonID])
                            self->buttons[self->buttonID]->isSelected = false;
                    }
                }

                if (backPressed)
                    return;
            }
#if MANIA_USE_PLUS
            else {
                LogHelpers_Print("Backout prevented");
                LogHelpers_Print("childHasFocus = %d", self->childHasFocus);
                LogHelpers_Print("dialogHasFocus = %d", self->dialogHasFocus);
                LogHelpers_Print("popoverHasFocus = %d", self->popoverHasFocus);
                LogHelpers_Print("backoutTimer = %d", self->backoutTimer);
            }
#endif
        }

        if (self->processButtonInputCB) {
            StateMachine_Run(self->processButtonInputCB);
        }
        else
            UIControl_ProcessButtonInput();

        if (!self->selectionDisabled) {
            if (UIControl->anyYPress) {
                if (!self->childHasFocus && !self->dialogHasFocus
#if MANIA_USE_PLUS
                    && !self->popoverHasFocus
#endif
                    && self->backoutTimer <= 0) {
                    StateMachine_Run(self->yPressCB);
                }

                UIControl->anyYPress = false;
            }

            if (UIControl->anyXPress) {
                if (!self->childHasFocus && !self->dialogHasFocus
#if MANIA_USE_PLUS
                    && !self->popoverHasFocus
#endif
                    && self->backoutTimer <= 0) {
                    StateMachine_Run(self->xPressCB);
                }

                UIControl->anyXPress = false;
            }
        }
    }
}

int32 UIControl_GetButtonID(EntityUIControl *control, EntityUIButton *entity)
{
    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (entity == control->buttons[i])
            return i;
    }

    return -1;
}

void UIControl_MenuChangeButtonInit(EntityUIControl *control)
{
    Entity *storeEntity = SceneInfo->entity;
    for (int32 i = 0; i < SCENEENTITY_COUNT; ++i) {
        EntityUIButton *entity = RSDK_GET_ENTITY(i, UIButton);

        if (entity) {
            int32 left   = MIN(-ScreenInfo->size.x >> 1, ScreenInfo->size.x >> 1) << 16;
            int32 right  = MAX(-ScreenInfo->size.x >> 1, ScreenInfo->size.x >> 1) << 16;
            int32 top    = MIN(-ScreenInfo->size.y >> 1, ScreenInfo->size.y >> 1) << 16;
            int32 bottom = MAX(-ScreenInfo->size.y >> 1, ScreenInfo->size.y >> 1) << 16;

            if (entity->position.x >= control->position.x + left && entity->position.x <= control->position.x + right) {
                if (entity->position.y >= control->position.y + top && entity->position.y <= control->position.y + bottom) {
                    int32 slot = RSDK.GetEntitySlot(entity);

                    SceneInfo->entity = (Entity *)entity;
                    if (UIButton && entity->classID == UIButton->classID) {
                        UIButton_ManageChoices(entity);
                        UIButton_Update();
                    }
                    else if (UIChoice && entity->classID == UIChoice->classID) {
                        UIChoice_Update();
                    }
                    else if (UITAZoneModule && entity->classID == UITAZoneModule->classID) {
                        UITAZoneModule_Update();
                    }
#if MANIA_USE_PLUS
                    else if (UIReplayCarousel && entity->classID == UIReplayCarousel->classID) {
                        UIReplayCarousel_Update();
                    }
#endif
                    else if (UIModeButton && entity->classID == UIModeButton->classID) {
#if MANIA_USE_PLUS
                        UIModeButton_Update();
#else
                        EntityUIModeButton *modeButton = (EntityUIModeButton *)SceneInfo->entity;
                        modeButton->touchPosSizeS.x    = 0xB80000;
                        modeButton->touchPosSizeS.y    = 0x3E0000;
                        modeButton->touchPosOffsetS.x  = 0;
                        modeButton->touchPosOffsetS.y  = -0x120000;
                        if (modeButton->textFrames != UIWidgets->textFrames || modeButton->wasDisabled != modeButton->disabled) {
                            UIModeButton_SetupSprites();
                            modeButton->textFrames  = UIWidgets->textFrames;
                            modeButton->wasDisabled = modeButton->disabled;
                        }
#endif
                    }
                    else if (UIVsZoneButton && entity->classID == UIVsZoneButton->classID) {
                        UIVsZoneButton_Update();
                    }
                    else if (UIHeading && entity->classID == UIHeading->classID) {
                        UIHeading_Update();
                    }

                    if (entity->visible)
                        RSDK.AddDrawListRef(entity->drawGroup, slot);

                    SceneInfo->entity = storeEntity;
                }
            }
        }
    }
}

#if MANIA_USE_PLUS
void UIControl_SetActiveMenuButtonPrompts(EntityUIControl *entity)
{
    for (int32 i = 0; i < entity->promptCount; ++i) entity->prompts[i]->active = ACTIVE_NORMAL;
}
#endif

void UIControl_SetActiveMenu(EntityUIControl *entity)
{
#if MANIA_USE_PLUS
    LogHelpers_PrintString(&entity->tag);
#endif

    entity->active  = ACTIVE_ALWAYS;
    entity->visible = true;

    if (entity->hasStoredButton) {
        entity->buttonID        = entity->storedButtonID;
        entity->storedButtonID  = 0;
        entity->hasStoredButton = false;
    }
    else if (entity->resetSelection) {
        entity->buttonID = entity->startingID;
    }

    RSDK.ClearCameras();
    RSDK.AddCamera(&entity->position, ScreenInfo->size.x << 16, ScreenInfo->size.y << 16, true);

    UIControl_MenuChangeButtonInit(entity);

#if MANIA_USE_PLUS
    if (!entity->childHasFocus && (entity->resetSelection || !entity->menuWasSetup)) {
#else
    if (!entity->childHasFocus) {
#endif
        entity->position.x  = entity->startPos.x;
        entity->position.y  = entity->startPos.y;
        entity->targetPos.x = entity->startPos.x;
        entity->targetPos.y = entity->startPos.y;
    }

    entity->state         = UIControl_ProcessInputs;
    entity->childHasFocus = false;

#if MANIA_USE_PLUS
    entity->menuWasSetup = true;

    for (int32 p = 0; p < entity->promptCount; ++p) entity->prompts[p]->active = ACTIVE_NORMAL;
#endif

    if (entity->menuSetupCB) {
        Entity *storeEntity = SceneInfo->entity;
        SceneInfo->entity   = (Entity *)entity;
#if RETRO_USE_MOD_LOADER
        StateMachine_Run(entity->menuSetupCB);
#else
        entity->menuSetupCB();
#endif
        SceneInfo->entity = storeEntity;
    }
}

void UIControl_SetMenuLostFocus(EntityUIControl *entity)
{
    entity->active  = ACTIVE_ALWAYS;
    entity->visible = true;

    if (!entity->dialogHasFocus && !entity->childHasFocus
#if MANIA_USE_PLUS
        && !entity->popoverHasFocus
#endif
    ) {
        if (entity->hasStoredButton) {
            entity->buttonID        = entity->storedButtonID;
            entity->storedButtonID  = 0;
            entity->hasStoredButton = false;
        }
        else if (entity->resetSelection) {
            entity->buttonID = entity->startingID;
#if !MANIA_USE_PLUS
            entity->position.x  = entity->startPos.x;
            entity->position.y  = entity->startPos.y;
            entity->targetPos.x = entity->startPos.x;
            entity->targetPos.y = entity->startPos.y;
            entity->state       = UIControl_ProcessInputs;
#endif
        }

#if MANIA_USE_PLUS
        if (entity->resetSelection || !entity->menuWasSetup) {
            entity->position.x  = entity->startPos.x;
            entity->position.y  = entity->startPos.y;
            entity->targetPos.x = entity->startPos.x;
            entity->targetPos.y = entity->startPos.y;
        }

        entity->menuWasSetup = true;
        UIControl_SetActiveMenuButtonPrompts(entity);
#endif
        entity->state = UIControl_ProcessInputs;
    }
}

void UIControl_SetInactiveMenu(EntityUIControl *control)
{
    UIControl->hasTouchInput = false;
    control->active          = ACTIVE_NEVER;
    control->visible         = false;
    control->state           = StateMachine_None;

#if MANIA_USE_PLUS
    RSDK_THIS(UIControl);

    if (self->promptCount) {
        for (int32 p = 0; p < control->promptCount; ++p) control->prompts[p]->active = ACTIVE_BOUNDS;
    }
#endif
}

void UIControl_SetupButtons(void)
{
    RSDK_THIS(UIControl);

    int32 slotID = RSDK.GetEntitySlot(self);

    if (UIHeading && slotID != SLOT_DIALOG_UICONTROL) {
        foreach_all(UIHeading, heading)
        {
            if (UIControl_ContainsPos(self, &heading->position))
                self->heading = heading;
        }
    }

#if MANIA_USE_PLUS
    if (UIShifter && slotID != SLOT_DIALOG_UICONTROL) {
        foreach_all(UIShifter, shifter)
        {
            if (UIControl_ContainsPos(self, &shifter->position)) {
                self->shifter   = shifter;
                shifter->parent = self;
            }
        }
    }

    if (UICarousel && slotID != SLOT_DIALOG_UICONTROL) {
        foreach_all(UICarousel, carousel)
        {
            if (UIControl_ContainsPos(self, &carousel->position)) {
                self->carousel   = carousel;
                carousel->parent = self;
            }
        }
    }
#endif

    for (int32 i = 0; i < SCENEENTITY_COUNT; ++i) {
        EntityUIButton *button = RSDK_GET_ENTITY(i, UIButton);

        if (button) {
            int32 classID = button->classID;
            if (classID != UIButton->classID && (!UIModeButton || classID != UIModeButton->classID) && (!UISaveSlot || classID != UISaveSlot->classID)
                && (!UICharButton || classID != UICharButton->classID) && (!UITAZoneModule || classID != UITAZoneModule->classID)
#if MANIA_USE_PLUS
                && (!UIRankButton || classID != UIRankButton->classID) && (!UIReplayCarousel || classID != UIReplayCarousel->classID)
#endif
                && (!UILeaderboard || classID != UILeaderboard->classID) && (!UIVsCharSelector || classID != UIVsCharSelector->classID)
                && (!UIVsZoneButton || classID != UIVsZoneButton->classID) && (!UIVsResults || classID != UIVsResults->classID)
                && (!UISlider || classID != UISlider->classID) && (!UIKeyBinder || classID != UIKeyBinder->classID)) {
            }
            else {
                if (self->buttonCount < UICONTROL_BUTTON_COUNT && UIControl_ContainsPos(self, &button->position)) {
                    if (!button->parent)
                        button->parent = (Entity *)self;

                    self->buttons[self->buttonCount++] = button;
                }
            }
        }
    }
}

bool32 UIControl_isMoving(EntityUIControl *entity)
{
    if (entity->scrollSpeed.x && entity->scrollSpeed.y) {
        return entity->position.x != entity->targetPos.x || entity->position.y != entity->targetPos.y;
    }
    else {
        if (entity->scrollSpeed.x) {
            return entity->position.x != entity->targetPos.x;
        }
        else if (entity->scrollSpeed.y) {
            return entity->position.y != entity->targetPos.y;
        }
    }

    return false;
}

void UIControl_MatchMenuTag(const char *text)
{
    String string;
    INIT_STRING(string);

    RSDK.SetString(&string, text);
    foreach_all(UIControl, entity)
    {
        if (entity->active == ACTIVE_ALWAYS || !RSDK.CompareStrings(&string, &entity->tag, false))
            UIControl_SetInactiveMenu(entity);
        else
            UIControl_SetActiveMenu(entity);
    }
}

void UIControl_HandleMenuChange(String *newMenuTag)
{
    if (newMenuTag->length) {
        foreach_all(UIControl, entity)
        {
            if (entity->active == ACTIVE_ALWAYS || !RSDK.CompareStrings(newMenuTag, &entity->tag, false))
                UIControl_SetInactiveMenu(entity);
            else
                UIControl_SetActiveMenu(entity);
        }
    }
}

void UIControl_HandleMenuLoseFocus(EntityUIControl *parent)
{
    foreach_all(UIControl, entity)
    {
        if (entity->active == ACTIVE_ALWAYS || entity != parent)
            UIControl_SetInactiveMenu(entity);
        else
            UIControl_SetMenuLostFocus(entity);
    }
}

void UIControl_ReturnToParentMenu(void)
{
    EntityUIControl *entity   = UIControl_GetUIControl();
    entity->selectionDisabled = false;

    UIControl_HandleMenuChange(&entity->parentTag);
}

#if MANIA_USE_PLUS
void UIControl_SetTargetPos(EntityUIControl *entity, int32 x, int32 y)
{
    int32 targetX = x;
    if (!x) {
        targetX = entity->position.x;
        x       = entity->position.x;
    }

    int32 targetY = y;
    if (!y) {
        targetY = entity->position.y;
        y       = entity->position.y;
    }

    if (!entity->noClamp) {
        int32 startX = entity->startPos.x - entity->cameraOffset.x;
        int32 startY = entity->startPos.y - entity->cameraOffset.y;
        int32 x1     = startX + (ScreenInfo->size.x << 15) - (entity->size.x >> 1);
        int32 x2     = startX + (entity->size.x >> 1) - (ScreenInfo->size.x << 15);
        int32 y1     = startY + (ScreenInfo->size.y << 15) - (entity->size.y >> 1);
        int32 y2     = startY + (entity->size.y >> 1) - (ScreenInfo->size.y << 15);

        if (x < x2)
            x2 = x;
        targetX = x2;

        if (x1 > x2)
            targetX = x1;

        if (y < y2)
            y2 = y;
        targetY = y2;

        if (y1 > y2)
            targetY = y1;
    }

    entity->targetPos.x = targetX;
    entity->targetPos.y = targetY;
}
#endif

void UIControl_HandlePosition(void)
{
    RSDK_THIS(UIControl);

    if (self->position.x < self->targetPos.x) {
        self->position.x += self->scrollSpeed.x;
        if (self->position.x > self->targetPos.x)
            self->position.x = self->targetPos.x;
    }
    else if (self->position.x > self->targetPos.x) {
        self->position.x -= self->scrollSpeed.x;
        if (self->position.x < self->targetPos.x)
            self->position.x = self->targetPos.x;
    }

    if (self->position.y < self->targetPos.y) {
        self->position.y += self->scrollSpeed.y;
        if (self->position.y > self->targetPos.y)
            self->position.y = self->targetPos.y;
    }
    else if (self->position.y > self->targetPos.y) {
        self->position.y -= self->scrollSpeed.y;
        if (self->position.y < self->targetPos.y)
            self->position.y = self->targetPos.y;
    }

    if (self->heading)
        self->heading->position.x = self->position.x;
}

void UIControl_ProcessButtonInput(void)
{
    RSDK_THIS(UIControl);

    bool32 allowAction = false;
    if (TouchInfo->count || UIControl->hasTouchInput) {
        EntityUIButton *activeButton = 0;
        UIControl->hasTouchInput     = TouchInfo->count != 0;
        UIControl->isProcessingInput = true;

        for (int32 i = 0; i < self->buttonCount; ++i) {
            if (self->buttons[i]) {
                EntityUIButton *button = self->buttons[i];

                Entity *storeEntity = SceneInfo->entity;
                SceneInfo->entity   = (Entity *)button;
                if (button->touchCB && !self->dialogHasFocus
#if MANIA_USE_PLUS
                    && !self->popoverHasFocus
#endif
                ) {
                    if (!button->checkSelectedCB || !button->checkSelectedCB()) {
                        bool32 wasTouched = button->touchCB();
                        if (allowAction || wasTouched) {
                            allowAction = true;
                            if (button->touchCB && !activeButton)
                                activeButton = button;
                        }
                        else {
                            allowAction = false;
                        }
                    }
                }

                SceneInfo->entity = storeEntity;
            }
        }

        if (TouchInfo->count) {
            if (allowAction) {
                int32 id = -1;

                for (int32 i = 0; i < self->buttonCount; ++i) {
                    if (activeButton == self->buttons[i]) {
                        id = i;
                        break;
                    }
                }

                self->buttonID = id;
                if (activeButton->isSelected) {
                    Entity *storeEntity      = SceneInfo->entity;
                    SceneInfo->entity        = (Entity *)activeButton;
                    activeButton->isSelected = true;
                    StateMachine_Run(activeButton->buttonEnterCB);
                    SceneInfo->entity = storeEntity;
                }
            }
            else {
                self->buttonID = -1;
            }
        }

        UIControl->isProcessingInput = false;
    }

    if (self->buttonID >= 0) {
        if (self->buttonID < self->buttonCount) {
            EntityUIButton *button = self->buttons[self->buttonID];

            if (button) {
                Entity *storeEntity = SceneInfo->entity;
                SceneInfo->entity   = (Entity *)button;
                if (button->processButtonCB) {
                    if (!button->checkSelectedCB || !button->checkSelectedCB()) {
#if RETRO_USE_MOD_LOADER
                        StateMachine_Run(button->processButtonCB);
#else
                        button->processButtonCB();
#endif
                    }
                }
                SceneInfo->entity = storeEntity;
            }
        }
    }
}

bool32 UIControl_ContainsPos(EntityUIControl *control, Vector2 *pos) {
    int32 x = control->startPos.x - control->cameraOffset.x;
    int32 y = control->startPos.y - control->cameraOffset.y;

    Hitbox hitbox;
    hitbox.top    = -(control->size.y >> 17);
    hitbox.left   = -(control->size.x >> 17);
    hitbox.right  = control->size.x >> 17;
    hitbox.bottom = control->size.y >> 17;

    if (MathHelpers_PointInHitbox(x, y, pos->x, pos->y, FLIP_NONE, &hitbox))
        return true;

    return false;
}

#if RETRO_INCLUDE_EDITOR
void UIControl_EditorDraw(void)
{
    RSDK_THIS(UIControl);

    self->updateRange.x = self->size.x >> 1;
    self->updateRange.y = self->size.y >> 1;

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

    Animator animator;
    RSDK.SetSpriteAnimation(UIControl->aniFrames, 0, &animator, false, 7);
    RSDK.DrawSprite(&animator, NULL, false);
}

void UIControl_EditorLoad(void) { UIControl->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void UIControl_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIControl, VAR_STRING, tag);
    RSDK_EDITABLE_VAR(UIControl, VAR_STRING, parentTag);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, activeOnLoad);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, noWidgets);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, resetSelection);
    RSDK_EDITABLE_VAR(UIControl, VAR_UINT8, rowCount);
    RSDK_EDITABLE_VAR(UIControl, VAR_UINT8, columnCount);
    RSDK_EDITABLE_VAR(UIControl, VAR_UINT8, startingID);
    RSDK_EDITABLE_VAR(UIControl, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIControl, VAR_VECTOR2, cameraOffset);
    RSDK_EDITABLE_VAR(UIControl, VAR_VECTOR2, scrollSpeed);
#if MANIA_USE_PLUS
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, noClamp);
    RSDK_EDITABLE_VAR(UIControl, VAR_BOOL, noWrap);
#endif
}
