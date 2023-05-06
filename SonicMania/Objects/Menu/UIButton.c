// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIButton Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIButton *UIButton;

void UIButton_Update(void)
{
    RSDK_THIS(UIButton);

    self->touchPosSizeS.x   = self->size.x;
    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = 0;
    self->touchPosSizeS.x += 3 * self->size.y;
    self->touchPosSizeS.y = self->size.y + 0x60000;

    if (self->textFrames != UIWidgets->textFrames || self->startListID != self->listID || self->startFrameID != self->frameID
        || self->isDisabled != self->disabled) {
        if (self->disabled)
            RSDK.SetSpriteAnimation(UIWidgets->textFrames, 7, &self->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);

        self->textFrames   = UIWidgets->textFrames;
        self->startListID  = self->listID;
        self->startFrameID = self->frameID;
        self->isDisabled   = self->disabled;
    }

    EntityUIButton *choice = UIButton_GetChoicePtr(self, self->selection);
    if (choice)
        choice->visible = true;

    StateMachine_Run(self->state);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (parent && self->state == UIButton_State_HandleButtonEnter
        && (parent->state != UIControl_ProcessInputs || parent->buttons[parent->buttonID] != self)) {
        self->isSelected = false;
        UIButton_ButtonLeaveCB();
    }
}

void UIButton_LateUpdate(void) {}

void UIButton_StaticUpdate(void) {}

void UIButton_Draw(void)
{
    RSDK_THIS(UIButton);

    Vector2 drawPos;
    int32 width = (self->size.x + self->size.y) >> 16;

    drawPos.x = self->position.x - self->buttonBounceOffset;
    drawPos.y = self->position.y - self->buttonBounceOffset;
#if MANIA_USE_PLUS
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, (UIWidgets->buttonColor >> 16) & 0xFF,
                                (UIWidgets->buttonColor >> 8) & 0xFF, (UIWidgets->buttonColor) & 0xFF);
#else
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, 0xF0, 0xF0, 0xF0);
#endif

    drawPos.x = self->position.x + self->buttonBounceOffset;
    drawPos.y = self->position.y + self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

    if (self->textVisible) {
        drawPos.x = self->buttonBounceOffset + self->position.x;
        drawPos.y = self->buttonBounceOffset + self->position.y;
        drawPos.y += self->textBounceOffset;

        switch (self->align) {
            case UIBUTTON_ALIGN_LEFT: drawPos.x += -0x60000 - (self->size.x >> 1); break;

            case UIBUTTON_ALIGN_CENTER: break;

            case UIBUTTON_ALIGN_RIGHT:
                drawPos.x -= 0x60000;
                drawPos.x += self->size.x >> 1;
                break;
        }

        if (self->disabled && self->align == UIBUTTON_ALIGN_LEFT)
            drawPos.x += 0x150000;

        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

void UIButton_Create(void *data)
{
    RSDK_THIS(UIButton);

    if (!SceneInfo->inEditor) {
        self->drawGroup     = 2;
        self->visible       = !self->invisible;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = TO_FIXED(128);
        self->updateRange.y = TO_FIXED(64);
        self->bgEdgeSize    = self->size.y >> 16;
        self->size.y        = abs(self->size.y);

        self->processButtonCB    = UIButton_ProcessButtonCB;
        self->touchCB            = UIButton_ProcessTouchCB_Single;
        self->selectedCB         = UIButton_SelectedCB;
        self->failCB             = UIButton_FailCB;
        self->buttonEnterCB      = UIButton_ButtonEnterCB;
        self->buttonLeaveCB      = UIButton_ButtonLeaveCB;
        self->checkButtonEnterCB = UIButton_CheckButtonEnterCB;
        self->checkSelectedCB    = UIButton_CheckSelectedCB;

        self->textVisible = true;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
        self->textFrames   = UIWidgets->textFrames;
        self->startListID  = self->listID;
        self->startFrameID = self->frameID;

        int32 slot = RSDK.GetEntitySlot(self) - self->choiceCount;
        for (int32 i = 0; i < self->choiceCount; ++i) {
            EntityUIButton *item = RSDK_GET_ENTITY(slot + i, UIButton);

            if ((UIChoice && item->classID == UIChoice->classID) || (UIVsRoundPicker && item->classID == UIVsRoundPicker->classID)
#if GAME_VERSION != VER_100
                || (UIResPicker && item->classID == UIResPicker->classID) || (UIWinSize && item->classID == UIWinSize->classID)
#endif
                ) {
                item->parent = (Entity *)self;
            }

            if (i) {
                item->position.x = self->firstChoicePos.x;
                item->position.y = self->firstChoicePos.y;
                item->active     = ACTIVE_NEVER;
            }
            else {
                self->firstChoicePos.x = item->position.x;
                self->firstChoicePos.y = item->position.y;
            }
        }
    }
}

void UIButton_StageLoad(void) {}

void UIButton_ManageChoices(EntityUIButton *button)
{
    for (int32 i = 0; i < button->choiceCount; ++i) {
        EntityUIButton *choice = RSDK_GET_ENTITY(i % button->choiceCount - button->choiceCount + RSDK.GetEntitySlot(button), UIButton);
        if (button->choiceCount > 0
            && (choice->classID == UIChoice->classID || choice->classID == UIVsRoundPicker->classID
#if GAME_VERSION != VER_100
                || choice->classID == UIResPicker->classID || choice->classID == UIWinSize->classID
#endif
                )) {

            choice->visible = i == button->selection;
            choice->active  = i == button->selection ? ACTIVE_NORMAL : ACTIVE_NEVER;
        }
    }
}

EntityUIButton *UIButton_GetChoicePtr(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount <= 0)
        return NULL;

    EntityUIButton *choice = RSDK_GET_ENTITY(RSDK.GetEntitySlot(button) - button->choiceCount + (selection % button->choiceCount), UIButton);
    if (choice->classID == UIChoice->classID || choice->classID == UIVsRoundPicker->classID
#if GAME_VERSION != VER_100
        || choice->classID == UIResPicker->classID || choice->classID == UIWinSize->classID
#endif
    ) {
        return choice;
    }

    return NULL;
}

void UIButton_SetChoiceSelectionWithCB(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount) {

        EntityUIButton *curChoice = UIButton_GetChoicePtr(button, button->selection);
        if (curChoice) {
            if (curChoice->classID == UIChoice->classID) {
                EntityUIChoice *choice     = (EntityUIChoice *)curChoice;
                choice->textBounceOffset   = 0;
                choice->buttonBounceOffset = 0;
                choice->isSelected         = false;
                choice->state              = UIChoice_State_HandleButtonLeave;
            }
            else if (curChoice->classID == UIVsRoundPicker->classID) {
                UIVsRoundPicker_SetChoiceInactive((EntityUIVsRoundPicker *)curChoice);
            }
#if GAME_VERSION != VER_100
            else if (curChoice->classID == UIResPicker->classID) {
                UIResPicker_SetChoiceInactive((EntityUIResPicker *)curChoice);
            }
            else if (curChoice->classID == UIWinSize->classID) {
                UIWinSize_SetChoiceInactive((EntityUIWinSize *)curChoice);
            }
#endif

            curChoice->active = ACTIVE_NEVER;
        }

        button->selection = selection;

        EntityUIChoice *newChoice = (EntityUIChoice *)UIButton_GetChoicePtr(button, selection);
        if (newChoice) {
            if (button->state == UIButton_State_HandleButtonEnter || button->state == UIButton_State_Selected) {
                UIChoice_SetChoiceActive(newChoice);
            }
            else {
                newChoice->active  = ACTIVE_BOUNDS;
                newChoice->visible = true;
                UIChoice_SetChoiceInactive(newChoice);
            }
        }

        if (button->choiceChangeCB) {
            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)button;
#if RETRO_USE_MOD_LOADER
            StateMachine_Run(button->choiceChangeCB);
#else
            button->choiceChangeCB();
#endif
            SceneInfo->entity = entStore;
        }
    }
}

void UIButton_SetChoiceSelection(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount) {

        EntityUIButton *choicePtr = UIButton_GetChoicePtr(button, button->selection);
        if (choicePtr) {
            if (choicePtr->classID == UIChoice->classID) {
                EntityUIChoice *choice     = (EntityUIChoice *)choicePtr;
                choice->textBounceOffset   = 0;
                choice->buttonBounceOffset = 0;
                choice->isSelected         = false;
                choice->state              = UIChoice_State_HandleButtonLeave;
            }
            else if (choicePtr->classID == UIVsRoundPicker->classID) {
                UIVsRoundPicker_SetChoiceInactive((EntityUIVsRoundPicker *)choicePtr);
            }
#if GAME_VERSION != VER_100
            else if (choicePtr->classID == UIResPicker->classID) {
                UIResPicker_SetChoiceInactive((EntityUIResPicker *)choicePtr);
            }
            else if (choicePtr->classID == UIWinSize->classID) {
                UIWinSize_SetChoiceInactive((EntityUIWinSize *)choicePtr);
            }
#endif

            choicePtr->active = ACTIVE_NEVER;
        }

        button->selection         = selection;
        EntityUIButton *newChoice = UIButton_GetChoicePtr(button, selection);
        newChoice->active         = ACTIVE_NORMAL;
    }
}

void *UIButton_GetActionCB(void)
{
    RSDK_THIS(UIButton);

    EntityUIButton *choice = UIButton_GetChoicePtr(self, self->selection);
    if (!choice)
        return self->actionCB;

    if (!UIChoice || self->choiceCount <= 0 || !choice->actionCB)
        return self->actionCB;

    return choice->actionCB;
}

void UIButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UIButton_ProcessButtonCB_Scroll(void)
{
    RSDK_THIS(UIButton);

    EntityUIControl *control = (EntityUIControl *)self->parent;

#if MANIA_USE_PLUS
    UIControl_SetTargetPos(control, self->position.x, self->position.y);
#else
    control->targetPos.y = self->position.y;
#endif

    if (!UIControl_isMoving(control)) {
        int32 rowID = 0;
        int32 colID = 0;

        if (control->rowCount && control->columnCount)
            rowID = control->buttonID / control->columnCount;

        if (control->columnCount)
            colID = control->buttonID % control->columnCount;

        bool32 changedSelection = false;
        if (control->rowCount > 1) {
            if (UIControl->anyUpPress) {
                --rowID;
                changedSelection = true;
            }

            if (UIControl->anyDownPress) {
                ++rowID;
                changedSelection = true;
            }
        }

        if (UIControl->anyLeftPress) {
            --colID;
            changedSelection = true;
        }

        if (UIControl->anyRightPress) {
            ++colID;
            changedSelection = true;
        }

        if (changedSelection) {
#if MANIA_USE_PLUS
            if (control->noWrap) {
                int32 rowCount = control->rowCount;
                int32 colCount = control->columnCount;

                if (rowID < control->rowCount)
                    rowCount = rowID;

                if (rowCount >= 0) {
                    if (rowID >= control->rowCount)
                        rowID = control->rowCount;
                }
                else {
                    rowID = 0;
                }

                if (colID < control->columnCount)
                    colCount = colID;

                if (colCount >= 0) {
                    if (colID >= control->columnCount)
                        colID = control->columnCount;
                }
                else {
                    colID = 0;
                }
            }
            else {
#endif
                if (rowID < 0)
                    rowID += control->rowCount;

                if (rowID >= control->rowCount)
                    rowID -= control->rowCount;

                if (colID < 0)
                    colID += control->columnCount;

                if (colID >= control->columnCount)
                    colID -= control->columnCount;
#if MANIA_USE_PLUS
            }
#endif

            int32 id = control->buttonCount - 1;
            if (colID + rowID * control->columnCount < id)
                id = colID + rowID * control->columnCount;

            if (control->buttonID != id) {
                control->buttonID = id;
                StateMachine_Run(self->buttonLeaveCB);
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 0xFF);
            }
        }
        else {
            bool32 hasNoAction = true;
            if (UIControl->anyConfirmPress) {
                if (self->disabled) {
                    StateMachine_Run(self->failCB);
                }
                else {
                    hasNoAction = !self->actionCB;
                }
            }

            if (hasNoAction) {
                if (!self->isSelected) {
                    if (control->buttonID == UIControl_GetButtonID(control, self) && control->state == UIControl_ProcessInputs
                        && !control->dialogHasFocus) {
                        StateMachine_Run(self->buttonEnterCB);
                    }
                }
            }
            else {
                StateMachine_Run(self->selectedCB);
            }
        }
    }
}

bool32 UIButton_ProcessTouchCB_Multi(void)
{
    RSDK_THIS(UIButton);

    EntityUIControl *control = (EntityUIControl *)self->parent;

    bool32 touched       = false;
    int32 lastTouchID    = -1;
    uint32 lastTouchDist = 0xFFFFFFFF;

    for (int32 i = 0; i < self->touchPosCount; ++i) {
        Vector2 touchPosSize   = self->touchPosSizeM[i];
        Vector2 touchPosOffset = self->touchPosOffsetM[i];

        if (TouchInfo->count) {
            int32 screenX = ScreenInfo->position.x << 16;
            int32 screenY = ScreenInfo->position.y << 16;

            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 x  = abs(touchPosOffset.x + self->position.x - (screenX - (int32)((TouchInfo->x[t] * ScreenInfo->size.x) * -65536.0f)));
                int32 y  = abs(touchPosOffset.y + self->position.y - (screenY - (int32)((TouchInfo->y[t] * ScreenInfo->size.y) * -65536.0f)));
                int32 x1 = touchPosSize.x >> 1;
                int32 y1 = touchPosSize.y >> 1;

                if (x < x1 && y < y1) {
                    touched = true;

                    if ((uint32)((touchPosSize.x >> 16) * (touchPosSize.y >> 16)) < lastTouchDist) {
                        lastTouchDist = (touchPosSize.x >> 16) * (touchPosSize.y >> 16);
                        lastTouchID   = i;
                    }
                }
            }
        }
        else {
            if (self->touchPressed && self->touchPosID == i && !self->disabled) {
                if (!UIControl_isMoving(control)) {
                    StateMachine_Run(self->touchPosCallbacks[i]);
                }
            }
        }
    }

    self->touchPosID   = lastTouchID;
    self->touchPressed = touched;

    return touched;
}

bool32 UIButton_ProcessTouchCB_Single(void)
{
    RSDK_THIS(UIButton);

    EntityUIControl *control = (EntityUIControl *)self->parent;

    bool32 touched = false;
    if (self->classID != UIButton->classID || !self->invisible) {
        if (TouchInfo->count) {
            int32 screenX = (ScreenInfo->position.x << 16);
            int32 screenY = (ScreenInfo->position.y << 16);
            int32 sizeX   = self->touchPosSizeS.x >> 1;
            int32 sizeY   = self->touchPosSizeS.y >> 1;

            for (int32 i = 0; i < TouchInfo->count; ++i) {
                int32 x = screenX - ((TouchInfo->x[i] * ScreenInfo->size.x) * -65536.0f);
                int32 y = screenY - ((TouchInfo->y[i] * ScreenInfo->size.y) * -65536.0f);

                int32 touchX = abs(self->touchPosOffsetS.x + self->position.x - x);
                int32 touchY = abs(self->touchPosOffsetS.y + self->position.y - y);
                if (touchX < sizeX && touchY < sizeY)
                    touched = true;
            }
        }
        else {
            if (self->touchPressed && !UIControl_isMoving(control)) {
                if (self->disabled) {
                    StateMachine_Run(self->failCB);
                }
                else {
                    self->isSelected       = false;
                    StateMachine(actionCB) = NULL;

                    if (self->classID == UIButton->classID)
                        actionCB = UIButton_GetActionCB();
                    else
                        actionCB = self->actionCB;

                    if (actionCB) {
                        StateMachine_Run(self->selectedCB);
                    }
                }
            }
        }
    }

    if (!touched) {
        if (!self->touchPressed && self->checkButtonEnterCB()) {
            for (int32 i = 0; i < control->buttonCount; ++i) {
                if (self == control->buttons[i] && control->buttonID != i) {
                    self->isSelected = false;
                    StateMachine_Run(self->buttonLeaveCB);

                    break;
                }
            }
        }
    }

    bool32 childTouched = false;
    self->touchPressed  = touched;
    if (self->classID == UIButton->classID && self->choiceCount > 0) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(self, self->selection);
        if (entPtr) {
            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)entPtr;
            if (entPtr->touchCB)
                childTouched = entPtr->touchCB();
            SceneInfo->entity = entStore;
        }
    }

    return touched || childTouched;
}

void UIButton_ProcessButtonCB(void)
{
    RSDK_THIS(UIButton);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    EntityUIButton *choice   = UIButton_GetChoicePtr(self, self->selection);

    int32 columnID = 0, rowID = 0;
    if (control->rowCount && control->columnCount)
        rowID = control->buttonID / control->columnCount;

    if (control->columnCount)
        columnID = control->buttonID % control->columnCount;

    bool32 movedV = false;
    if (control->rowCount > 1) {
        if (UIControl->anyUpPress) {
            movedV = true;
            --rowID;
        }

        if (UIControl->anyDownPress) {
            movedV = true;
            rowID++;
        }
    }

    int32 selection = self->selection;
    bool32 movedH   = 0;

    if (choice && self->choiceCount == 1 && choice->processButtonCB && !self->choiceDir && !self->disabled) {
        Entity *entStore  = SceneInfo->entity;
        SceneInfo->entity = (Entity *)choice;
#if RETRO_USE_MOD_LOADER
        StateMachine_Run(choice->processButtonCB);
#else
        choice->processButtonCB();
#endif
        SceneInfo->entity = entStore;
    }
    else {
        if (UIControl->anyLeftPress) {
            if (self->choiceCount <= 0 || self->choiceDir || self->disabled) {
                if (control->columnCount > 1) {
                    movedV = true;
                    columnID--;
                }

                movedH = false;
            }
            else {
                if (--selection < 0) {
                    while (selection < 0) selection += self->choiceCount;
                }
                choice = UIButton_GetChoicePtr(self, selection);

                while ((choice && choice->disabled) && selection != self->selection) {
                    if (--selection < 0) {
                        while (selection < 0) selection += self->choiceCount;
                    }

                    choice = UIButton_GetChoicePtr(self, selection);
                }

                movedH = true;
            }
        }

        if (UIControl->anyRightPress) {
            if (self->choiceCount <= 0 || self->choiceDir || self->disabled) {
                if (control->columnCount > 1) {
                    ++columnID;
                    movedV = true;
                }

                movedH = false;
            }
            else {
                selection              = (selection + 1) % self->choiceCount;
                choice = UIButton_GetChoicePtr(self, selection);

                while ((choice && choice->disabled) && selection != self->selection) {
                    selection = (selection + 1) % self->choiceCount;

                    choice = UIButton_GetChoicePtr(self, selection);
                }

                movedH = true;
            }
        }
    }

    if (movedH) {
        if (selection < 0)
            selection += self->choiceCount;

        if (selection >= self->choiceCount)
            selection -= self->choiceCount;

        if (selection != self->selection) {
            UIButton_SetChoiceSelectionWithCB(self, selection);
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
    }

    if (movedV) {
#if MANIA_USE_PLUS
        if (control->noWrap) {
            int32 count = control->rowCount;
            if (rowID < control->rowCount)
                count = rowID;

            if (rowID >= control->rowCount)
                rowID = control->rowCount;
            else if (count < 0)
                rowID = 0;

            count = control->columnCount;
            if (columnID < control->columnCount)
                count = columnID;

            if (columnID >= control->columnCount)
                columnID = control->columnCount;
            else if (count < 0)
                columnID = 0;
        }
        else {
#endif
            if (rowID < 0)
                rowID += control->rowCount;

            if (rowID >= control->rowCount)
                rowID -= control->rowCount;

            if (columnID < 0)
                columnID += control->columnCount;

            if (columnID >= control->columnCount)
                columnID -= control->columnCount;
#if MANIA_USE_PLUS
        }
#endif

        int32 id = columnID + control->columnCount * rowID;

        if (id >= control->buttonCount - 1)
            id = control->buttonCount - 1;

        if (control->buttonID != id && self != control->buttons[id]) {
            control->buttonID = id;
            UIButton_ButtonLeaveCB();
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
    }
    else {
        StateMachine(actionCB) = UIButton_GetActionCB();

        if (UIControl->anyConfirmPress && (!UIChoice || actionCB)) {
            if (self->disabled || (self->choiceCount > 0 && choice->disabled))
                RSDK.PlaySfx(UIWidgets->sfxFail, false, 255);
            else
                UIButton_SelectedCB();
        }
        else {
            if (self->state != UIButton_State_HandleButtonEnter && self->state != UIButton_State_Selected) {
                if (control->buttonID == columnID + rowID * control->columnCount && control->state == UIControl_ProcessInputs)
                    UIButton_ButtonEnterCB();
            }
        }
    }
}

bool32 UIButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIButton);

    return self->state == UIButton_State_HandleButtonEnter;
}

bool32 UIButton_CheckSelectedCB(void)
{
    RSDK_THIS(UIButton);

    return self->state == UIButton_State_Selected;
}

void UIButton_ButtonEnterCB(void)
{
    RSDK_THIS(UIButton);

    if (self->state != UIButton_State_HandleButtonEnter) {
        self->textBounceOffset     = 0;
        self->buttonBounceOffset   = 0;
        self->textBounceVelocity   = -0x20000;
        self->buttonBounceVelocity = -0x20000;
        self->state                = UIButton_State_HandleButtonEnter;

        if (UIChoice) {
            EntityUIButton *choicePtr = UIButton_GetChoicePtr(self, self->selection);

            if (choicePtr) {
                if (choicePtr->classID == UIChoice->classID) {
                    UIChoice_SetChoiceActive((EntityUIChoice *)choicePtr);
                }

                if (choicePtr->classID == UIVsRoundPicker->classID) {
                    UIVsRoundPicker_SetChoiceActive((EntityUIVsRoundPicker *)choicePtr);
                }

#if GAME_VERSION != VER_100
                if (choicePtr->classID == UIResPicker->classID) {
                    UIResPicker_SetChoiceActive((EntityUIResPicker *)choicePtr);
                }

                if (choicePtr->classID == UIWinSize->classID) {
                    UIWinSize_SetChoiceActive((EntityUIWinSize *)choicePtr);
                }
#endif
            }
        }
    }
}

void UIButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UIButton);

    self->state = UIButton_State_HandleButtonLeave;

    if (UIChoice) {
        EntityUIButton *widget = UIButton_GetChoicePtr(self, self->selection);
        if (widget) {
            if (widget->classID == UIChoice->classID) {
                ((EntityUIChoice *)widget)->textBounceOffset   = 0;
                ((EntityUIChoice *)widget)->buttonBounceOffset = 0;
                widget->isSelected                             = false;
                widget->state                                  = UIChoice_State_HandleButtonLeave;
            }
            else if (widget->classID == UIVsRoundPicker->classID) {
                UIVsRoundPicker_SetChoiceInactive((EntityUIVsRoundPicker *)widget);
            }
#if GAME_VERSION != VER_100
            else if (widget->classID == UIResPicker->classID) {
                UIResPicker_SetChoiceInactive((EntityUIResPicker *)widget);
            }
            else if (widget->classID == UIWinSize->classID) {
                UIWinSize_SetChoiceInactive((EntityUIWinSize *)widget);
            }
#endif
        }
    }
}

void UIButton_SelectedCB(void)
{
    RSDK_THIS(UIButton);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    EntityUIButton *choice  = UIButton_GetChoicePtr(self, self->selection);

    if (self->clearParentState || (choice && choice->classID == UIChoice->classID && choice->buttonBounceVelocity))
        parent->state = StateMachine_None;

    if (self->assignsP1) {
#if MANIA_USE_PLUS
        int32 id = API_GetFilteredInputDeviceID(false, false, 0);
#else
        int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
#endif
        API_ResetInputSlotAssignments();
        API_AssignInputSlotToDevice(CONT_P1, id);
    }

    if (self->freeBindP2)
        API_AssignInputSlotToDevice(CONT_P2, INPUT_AUTOASSIGN);

    parent->backoutTimer = 30;

    if (self->transition) {
        StateMachine(actionCB) = self->actionCB;
        if (UIChoice && self->choiceCount > 0 && choice)
            actionCB = choice->actionCB;

        UITransition_StartTransition(actionCB, 14);
    }

    if (self->stopMusic)
        Music_Stop();

    self->timer = 0;
    self->state = UIButton_State_Selected;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIButton_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIButton);

    if (self->textBounceOffset) {
        int32 offset = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += offset << 16;

        if (offset < 0 && self->textBounceOffset < 0)
            self->textBounceOffset = 0;
        else if (offset > 0 && self->textBounceOffset > 0)
            self->textBounceOffset = 0;
    }

    if (self->buttonBounceOffset) {
        int32 offset = -(self->buttonBounceOffset / abs(self->buttonBounceOffset));
        self->buttonBounceOffset += offset << 16;

        if (offset < 0 && self->buttonBounceOffset < 0)
            self->buttonBounceOffset = 0;
        else if (offset > 0 && self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UIButton_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIButton);

    self->textBounceVelocity += 0x4000;
    self->textBounceOffset += self->textBounceVelocity;

    if (self->textBounceOffset >= 0 && self->textBounceVelocity > 0) {
        self->textBounceOffset   = 0;
        self->textBounceVelocity = 0;
    }

    self->buttonBounceVelocity += 0x4800;
    self->buttonBounceOffset += self->buttonBounceVelocity;

    if (self->buttonBounceOffset >= -0x20000 && self->buttonBounceVelocity > 0) {
        self->buttonBounceOffset   = -0x20000;
        self->buttonBounceVelocity = 0;
    }
}

void UIButton_State_Selected(void)
{
    RSDK_THIS(UIButton);

    UIButton_State_HandleButtonEnter();

    if (++self->timer == 30) {
        self->timer = 0;

        if (!self->transition) {
            StateMachine(actionCB) = UIButton_GetActionCB();
            StateMachine_Run(actionCB);
        }

        self->state = UIButton_State_HandleButtonEnter;
    }

    self->textVisible = !((self->timer >> 1) & 1);
}

#if GAME_INCLUDE_EDITOR
void UIButton_EditorDraw(void)
{
    RSDK_THIS(UIButton);

    int32 sizeY = self->size.y;

    if (self->disabled)
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 7, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);

    self->textFrames   = UIWidgets->textFrames;
    self->startListID  = self->listID;
    self->startFrameID = self->frameID;

    self->drawGroup     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    self->textVisible   = true;

    self->inkEffect = self->invisible ? INK_BLEND : INK_NONE;
    UIButton_Draw();

    self->size.y = sizeY;

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 slot = RSDK.GetEntitySlot(self) - self->choiceCount;
        for (int32 i = 0; i < self->choiceCount; ++i) {
            EntityUIButton *item = RSDK_GET_ENTITY(slot + i, UIButton);

            if (item)
                DrawHelpers_DrawArrow(self->position.x, self->position.y, item->position.x, item->position.y, i ? 0xFFFF00 : 0xE0E0E0, INK_NONE,
                                      0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void UIButton_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(UIButton, align);
    RSDK_ENUM_VAR("Left", UIBUTTON_ALIGN_LEFT);
    RSDK_ENUM_VAR("Center", UIBUTTON_ALIGN_CENTER);
    RSDK_ENUM_VAR("Right", UIBUTTON_ALIGN_RIGHT);
}
#endif

void UIButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIButton, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, choiceCount);
    RSDK_EDITABLE_VAR(UIButton, VAR_UINT8, choiceDir);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, invisible);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, assignsP1);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, freeBindP2);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, transition);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, stopMusic);
}
