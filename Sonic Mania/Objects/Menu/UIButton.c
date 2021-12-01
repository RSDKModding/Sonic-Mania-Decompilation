#include "SonicMania.h"

ObjectUIButton *UIButton;

void UIButton_Update(void)
{
    RSDK_THIS(UIButton);
    self->touchPosStart.x = self->size.x;
    self->touchPosEnd.x   = 0;
    self->touchPosEnd.y   = 0;
    self->touchPosStart.x += 3 * self->size.y;
    self->touchPosStart.y = self->size.y + 0x60000;
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
    EntityUIButton *entPtr = UIButton_GetChoicePtr(self, self->selection);
    if (entPtr)
        entPtr->visible = true;

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
    int32 width = self->size.y + self->size.x;
    drawPos.x   = self->position.x - self->buttonBounceOffset;
    drawPos.y   = self->position.y - self->buttonBounceOffset;
    width >>= 16;
#if RETRO_USE_PLUS
    UIWidgets_DrawParallelogram(self->size.y >> 16, width, self->bgEdgeSize, (UIWidgets->buttonColour >> 16) & 0xFF,
                                (UIWidgets->buttonColour >> 8) & 0xFF, (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_DrawParallelogram(self->size.y >> 16, width, self->bgEdgeSize, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif
    drawPos.x = self->position.x + self->buttonBounceOffset;
    drawPos.y = self->position.y + self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(self->size.y >> 16, width, self->bgEdgeSize, 0, 0, 0, drawPos.x, drawPos.y);
    if (self->textVisible) {
        drawPos.x = self->buttonBounceOffset + self->position.x;
        drawPos.y = self->buttonBounceOffset + self->position.y;
        drawPos.y += self->textBounceOffset;

        if (!self->align) {
            drawPos.x += -0x60000 - (self->size.x >> 1);
        }
        else if (self->align == 2) {
            drawPos.x -= 0x60000;
            drawPos.x += self->size.x >> 1;
        }
        if (self->disabled) {
            if (!self->align)
                drawPos.x += 0x150000;
        }
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

void UIButton_Create(void *data)
{
    RSDK_THIS(UIButton);
    if (!SceneInfo->inEditor) {
        self->drawOrder          = 2;
        self->visible            = !self->invisible;
        self->active             = ACTIVE_BOUNDS;
        self->updateRange.x      = 0x800000;
        self->updateRange.y      = 0x400000;
        self->bgEdgeSize         = self->size.y >> 16;
        self->size.y             = abs(self->size.y);
        self->processButtonCB    = UIButton_ProcessButtonCB;
        self->touchCB            = UIButton_ProcessTouchCB;
        self->selectedCB         = UIButton_SelectedCB;
        self->failCB             = UIButton_FailCB;
        self->buttonEnterCB      = UIButton_ButtonEnterCB;
        self->buttonLeaveCB      = UIButton_ButtonLeaveCB;
        self->checkButtonEnterCB = UIButton_CheckButtonEnterCB;
        self->checkSelectedCB    = UIButton_CheckSelectedCB;
        self->textVisible        = true;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
        self->textFrames   = UIWidgets->textFrames;
        self->startListID  = self->listID;
        self->startFrameID = self->frameID;

        int32 entID = RSDK.GetEntityID(self) - self->choiceCount;
        for (int32 i = 0; i < self->choiceCount; ++i) {
            EntityUIButton *item = RSDK.GetEntityByID(entID + i);
            if ((UIChoice && item->objectID == UIChoice->objectID) || (UIVsRoundPicker && item->objectID == UIVsRoundPicker->objectID)
                || (UIResPicker && item->objectID == UIResPicker->objectID) || (UIWinSize && item->objectID == UIWinSize->objectID)) {
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
        Entity *choice = RSDK.GetEntityByID(i % button->choiceCount - button->choiceCount + RSDK.GetEntityID(button));
        if (button->choiceCount > 0
            && (choice->objectID == UIChoice->objectID || choice->objectID == UIVsRoundPicker->objectID || choice->objectID == UIResPicker->objectID
                || choice->objectID == UIWinSize->objectID)) {
            choice->visible = i == button->selection;
            choice->active  = i == button->selection ? ACTIVE_NORMAL : ACTIVE_NEVER;
        }
    }
}

EntityUIButton *UIButton_GetChoicePtr(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount <= 0)
        return NULL;
    Entity *choice = RSDK.GetEntityByID(selection % button->choiceCount - button->choiceCount + RSDK.GetEntityID(button));
    if (choice->objectID == UIChoice->objectID || choice->objectID == UIVsRoundPicker->objectID || choice->objectID == UIResPicker->objectID
        || choice->objectID == UIWinSize->objectID) {
        return (EntityUIButton *)choice;
    }
    return NULL;
}

void UIButton_SetChoiceSelectionWithCB(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(button, button->selection);
        if (entPtr) {
            if (entPtr->objectID == UIChoice->objectID) {
                EntityUIChoice *choice     = (EntityUIChoice *)entPtr;
                choice->textBounceOffset   = 0;
                choice->buttonBounceOffset = 0;
                choice->isSelected         = false;
                choice->state              = UIChoice_State_HandleButtonLeave;
            }
            else if (entPtr->objectID == UIVsRoundPicker->objectID) {
                UIVsRoundPicker_SetChoiceInactive((EntityUIVsRoundPicker *)entPtr);
            }
            else if (entPtr->objectID == UIResPicker->objectID) {
                UIResPicker_SetChoiceInactive((EntityUIResPicker *)entPtr);
            }
            else if (entPtr->objectID == UIWinSize->objectID) {
                UIWinSize_SetChoiceInactive((EntityUIWinSize *)entPtr);
            }
            entPtr->active = ACTIVE_NEVER;
        }

        button->selection         = selection;
        EntityUIChoice *choicePtr = (EntityUIChoice *)UIButton_GetChoicePtr(button, selection);
        if (choicePtr) {
            if (button->state == UIButton_State_HandleButtonEnter || button->state == UIButton_State_Selected) {
                UIChoice_SetChoiceActive(choicePtr);
            }
            else {
                choicePtr->active  = ACTIVE_BOUNDS;
                choicePtr->visible = true;
                UIChoice_SetChoiceInactive(choicePtr);
            }
        }

        if (button->choiceChangeCB) {
            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)button;
            button->choiceChangeCB();
            SceneInfo->entity = entStore;
        }
    }
}

void UIButton_SetChoiceSelection(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(button, button->selection);
        if (entPtr) {
            if (entPtr->objectID == UIChoice->objectID) {
                EntityUIChoice *choice     = (EntityUIChoice *)entPtr;
                choice->textBounceOffset   = 0;
                choice->buttonBounceOffset = 0;
                choice->isSelected         = false;
                choice->state              = UIChoice_State_HandleButtonLeave;
            }
            else if (entPtr->objectID == UIVsRoundPicker->objectID) {
                UIVsRoundPicker_SetChoiceInactive((EntityUIVsRoundPicker *)entPtr);
            }
            else if (entPtr->objectID == UIResPicker->objectID) {
                UIResPicker_SetChoiceInactive((EntityUIResPicker *)entPtr);
            }
            else if (entPtr->objectID == UIWinSize->objectID) {
                UIWinSize_SetChoiceInactive((EntityUIWinSize *)entPtr);
            }
            entPtr->active = ACTIVE_NEVER;
        }

        button->selection         = selection;
        EntityUIButton *newEntPtr = UIButton_GetChoicePtr(button, selection);
        newEntPtr->active         = ACTIVE_NORMAL;
    }
}

void *UIButton_GetActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityUIButton *choice = UIButton_GetChoicePtr(self, self->selection);
    if (!choice)
        return self->actionCB;

    if (!UIChoice || self->choiceCount <= 0 || !choice->actionCB) {
        return self->actionCB;
    }
    return choice->actionCB;
}

void UIButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UIButton_ProcessButtonCB_Scroll(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)self->parent;

#if RETRO_USE_PLUS
    UIControl_SetTargetPos(control, self->position.x, self->position.y);
#else 
    control->targetPos.y = self->position.y;
#endif

    if (!UIControl_isMoving(control)) {
        int32 rowID = 0;
        int32 colID = 0;
        if (control->rowCount && control->columnCount) {
            rowID = control->buttonID / control->columnCount;
        }
        else {
            rowID = 0;
        }

        if (control->columnCount) {
            colID = control->buttonID % control->columnCount;
        }
        else {
            colID = 0;
        }

        bool32 flag = false;
        if (control->rowCount > 1) {
            if (UIControl->keyUp) {
                --rowID;
                flag = true;
            }
            if (UIControl->keyDown) {
                ++rowID;
                flag = true;
            }
        }
        if (UIControl->keyLeft) {
            --colID;
            flag = true;
        }
        if (UIControl->keyRight) {
            ++colID;
            flag = true;
        }

        if (flag) {
#if RETRO_USE_PLUS
            if (control->noWrap) {
                int32 rowCount = control->rowCount;
                if (rowID < control->rowCount)
                    rowCount = rowID;
                if (rowCount >= 0) {
                    if (rowID >= control->rowCount)
                        rowID = control->rowCount;
                }
                else {
                    rowID = 0;
                }

                int32 colCount = control->columnCount;
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
#if RETRO_USE_PLUS
            }
#endif

            int32 id = control->buttonCount - 1;
            if (colID + rowID * control->columnCount < id)
                id = colID + rowID * control->columnCount;
            if (control->buttonID != id) {
                control->buttonID = id;
                StateMachine_Run(self->buttonLeaveCB);
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
            }
        }
        else {
            bool32 hasNoAction = true;
            if (UIControl->keyConfirm) {
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

bool32 UIButton_ProcessTouchCB_Alt(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)self->parent;

    bool32 touchFlag  = false;
    int32 lastTouchID = -1;
    uint32 lastTouch  = 0xFFFFFFFF;

    for (int32 i = 0; i < self->touchPosCount; ++i) {
        Vector2 touchPos1 = self->touchPos1[i];
        Vector2 touchPos2 = self->touchPos2[i];

        if (TouchInfo->count) {
            int32 screenX = ScreenInfo->position.x << 16;
            int32 screenY = ScreenInfo->position.y << 16;
            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 x = abs(touchPos2.x + self->position.x - (screenX - (int32)((TouchInfo->x[t] * ScreenInfo->width) * -65536.0f)));
                int32 y = abs(touchPos2.y + self->position.y - (screenY - (int32)((TouchInfo->y[t] * ScreenInfo->height) * -65536.0f)));

                int32 x2 = touchPos1.x >> 1;
                int32 y2 = touchPos1.y >> 1;

                if (x < x2 && y < y2) {
                    touchFlag = true;
                    if ((touchPos1.x >> 16) * (touchPos1.y >> 16) < lastTouch) {
                        lastTouch   = (touchPos1.x >> 16) * (touchPos1.y >> 16);
                        lastTouchID = i;
                    }
                }
            }
        }
        else {
            if (self->touchPressed && self->touchCountUnknown == i && !self->disabled) {
                if (!UIControl_isMoving(control)) {
                    StateMachine_Run(self->touchPosCallbacks[i]);
                }
            }
        }
    }

    self->touchCountUnknown = lastTouchID;
    self->touchPressed      = touchFlag;
    return self->touchPressed;
}

bool32 UIButton_ProcessTouchCB(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)self->parent;

    bool32 touchFlag = false;
    if (self->objectID != UIButton->objectID || !self->invisible) {
        if (TouchInfo->count) {
            int32 screenX = (ScreenInfo->position.x << 16);
            int32 screenY = (ScreenInfo->position.y << 16);
            int32 sizeX   = self->touchPosStart.x >> 1;
            int32 sizeY   = self->touchPosStart.y >> 1;

            for (int32 i = 0; i < TouchInfo->count; ++i) {
                int32 x = screenX - ((TouchInfo->x[i] * ScreenInfo->width) * -65536.0f);
                int32 y = screenY - ((TouchInfo->y[i] * ScreenInfo->height) * -65536.0f);

                int32 touchX = abs(self->touchPosEnd.x + self->position.x - x);
                int32 touchY = abs(self->touchPosEnd.y + self->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    touchFlag = true;
                }
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
                    if (self->objectID == UIButton->objectID)
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

    if (!touchFlag) {
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

    bool32 childTouchFlag = false;
    self->touchPressed    = touchFlag;
    if (self->objectID == UIButton->objectID && self->choiceCount > 0) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(self, self->selection);
        if (entPtr) {
            Entity *entStore  = SceneInfo->entity;
            SceneInfo->entity = (Entity *)entPtr;
            if (entPtr->touchCB) {
                childTouchFlag = entPtr->touchCB();
            }
            SceneInfo->entity = entStore;
        }
    }
    return self->touchPressed || childTouchFlag;
}

void UIButton_ProcessButtonCB(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)self->parent;
    EntityUIButton *choice   = UIButton_GetChoicePtr(self, self->selection);

    int32 columnID = 0, rowID = 0;
    if (control->rowCount && control->columnCount)
        rowID = control->buttonID / control->columnCount;
    else
        rowID = 0;

    if (control->columnCount)
        columnID = control->buttonID % control->columnCount;
    else
        columnID = 0;

    bool32 flag = 0;
    if (control->rowCount > 1) {
        if (UIControl->keyUp) {
            flag = true;
            --rowID;
        }
        if (UIControl->keyDown) {
            flag = true;
            rowID++;
        }
    }

    int32 selection = self->selection;
    bool32 flag2    = 0;
    if (choice && self->choiceCount == 1 && choice->processButtonCB && !self->choiceDir && !self->disabled) {
        Entity *entStore  = SceneInfo->entity;
        SceneInfo->entity = (Entity *)choice;
        choice->processButtonCB();
        SceneInfo->entity = entStore;
    }
    else {
        if (UIControl->keyLeft) {
            if (self->choiceCount <= 0 || self->choiceDir || self->disabled) {
                if (control->columnCount > 1) {
                    flag = true;
                    columnID--;
                }
                flag2 = false;
            }
            else {
                do {
                    if (--selection < 0) {
                        do
                            selection += self->choiceCount;
                        while (selection < 0);
                    }
                } while (UIButton_GetChoicePtr(self, selection)->disabled && selection != self->selection);
                flag2 = true;
            }
        }

        if (UIControl->keyRight) {
            if (self->choiceCount <= 0 || self->choiceDir || self->disabled) {
                if (control->columnCount > 1) {
                    ++columnID;
                    flag = true;
                }
                flag2 = false;
            }
            else {
                do
                    selection = (selection + 1) % self->choiceCount;
                while (UIButton_GetChoicePtr(self, selection)->disabled && selection != self->selection);
                flag2 = true;
            }
        }
    }

    if (flag2) {
        if (selection < 0)
            selection += self->choiceCount;

        if (selection >= self->choiceCount)
            selection -= self->choiceCount;
        if (selection != self->selection) {
            UIButton_SetChoiceSelectionWithCB(self, selection);
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
    }

    if (flag) {
#if RETRO_USE_PLUS
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
#if RETRO_USE_PLUS
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

        if (UIControl->keyConfirm && (!UIChoice || actionCB)) {
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
                if (choicePtr->objectID == UIChoice->objectID) {
                    UIChoice_SetChoiceActive((EntityUIChoice *)choicePtr);
                }
                if (choicePtr->objectID == UIVsRoundPicker->objectID) {
                    UIVsRoundPicker_SetChoiceActive((EntityUIVsRoundPicker *)choicePtr);
                }
                if (choicePtr->objectID == UIResPicker->objectID) {
                    UIResPicker_SetChoiceActive((EntityUIResPicker *)choicePtr);
                }
                if (choicePtr->objectID == UIWinSize->objectID) {
                    UIWinSize_SetChoiceActive((EntityUIWinSize *)choicePtr);
                }
            }
        }
    }
}

void UIButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UIButton);

    self->state = UIButton_State_HandleButtonLeave;
    if (UIChoice) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(self, self->selection);
        if (entPtr) {
            if (entPtr->objectID == UIChoice->objectID) {
                ((EntityUIChoice *)entPtr)->textBounceOffset   = 0;
                ((EntityUIChoice *)entPtr)->buttonBounceOffset = 0;
                entPtr->isSelected                             = false;
                entPtr->state                                  = UIChoice_State_HandleButtonLeave;
            }
            else if (entPtr->objectID == UIVsRoundPicker->objectID) {
                UIVsRoundPicker_SetChoiceInactive((EntityUIVsRoundPicker *)entPtr);
            }
            else if (entPtr->objectID == UIResPicker->objectID) {
                UIResPicker_SetChoiceInactive((EntityUIResPicker *)entPtr);
            }
            else if (entPtr->objectID == UIWinSize->objectID) {
                UIWinSize_SetChoiceInactive((EntityUIWinSize *)entPtr);
            }
        }
    }
}

void UIButton_SelectedCB(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    EntityUIButton *choice  = UIButton_GetChoicePtr(self, self->selection);

    if (self->clearParentState || (choice && choice->objectID == UIChoice->objectID && choice->buttonBounceVelocity))
        parent->state = StateMachine_None;

    if (self->assignsP1) {
#if RETRO_USE_PLUS
        int32 id = API_MostRecentActiveControllerID(0, 0, 0);
#else
        int32 id = API_MostRecentActiveControllerID(0);
#endif
        API_ResetControllerAssignments();
        API_AssignControllerID(CONT_P1, id);
    }

    if (self->freeBindP2)
        API_AssignControllerID(CONT_P2, CONT_AUTOASSIGN);

    parent->backoutTimer = 30;
    if (self->transition) {
        StateMachine(actionCB) = self->actionCB;
        if (UIChoice && self->choiceCount > 0 && choice) {
            actionCB = choice->actionCB;
        }

        UITransition_StartTransition(actionCB, 14);
    }

    if (self->stopMusic)
        RSDK.StopChannel(Music->channelID);
    self->timer = 0;
    self->state = UIButton_State_Selected;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIButton_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIButton);

    if (self->textBounceOffset) {
        int32 val = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += val << 16;
        if (val < 0 && self->textBounceOffset < 0) {
            self->textBounceOffset = 0;
        }
        else if (val > 0 && self->textBounceOffset > 0)
            self->textBounceOffset = 0;
    }

    if (self->buttonBounceOffset) {
        int32 val = -(self->buttonBounceOffset / abs(self->buttonBounceOffset));
        self->buttonBounceOffset += val << 16;
        if (val < 0 && self->buttonBounceOffset < 0) {
            self->buttonBounceOffset = 0;
        }
        else if (val > 0 && self->buttonBounceOffset > 0)
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

#if RETRO_INCLUDE_EDITOR
void UIButton_EditorDraw(void)
{
    RSDK_THIS(UIButton);

    int sizeY = self->size.y;

    if (self->disabled)
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 7, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator, true, self->frameID);
    self->textFrames   = UIWidgets->textFrames;
    self->startListID  = self->listID;
    self->startFrameID = self->frameID;

    self->drawOrder     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    self->textVisible   = true;

    self->inkEffect = self->invisible ? INK_BLEND : INK_NONE;
    UIButton_Draw();

    self->size.y = sizeY;
}

void UIButton_EditorLoad(void) {}
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
