#include "SonicMania.h"

ObjectUIButton *UIButton;

void UIButton_Update(void)
{
    RSDK_THIS(UIButton);
    entity->touchPosStart.x = entity->size.x;
    entity->touchPosEnd.x   = 0;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x += 3 * entity->size.y;
    entity->touchPosStart.y = entity->size.y + 0x60000;
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex || entity->startListID != entity->listID || entity->startFrameID != entity->frameID
        || entity->isDisabled != entity->disabled) {
        if (entity->disabled)
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &entity->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->startListID     = entity->listID;
        entity->startFrameID    = entity->frameID;
        entity->isDisabled      = entity->disabled;
    }
    EntityUIButton *entPtr = UIButton_GetChoicePtr(entity, entity->selection);
    if (entPtr)
        entPtr->visible = true;

    StateMachine_Run(entity->state);

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (parent && entity->state == UIButton_Unknown17
        && (parent->state != UIControl_ProcessInputs || parent->buttons[parent->activeEntityID] != entity)) {
        entity->flag = false;
        UIButton_Unknown13();
    }
}

void UIButton_LateUpdate(void) {}

void UIButton_StaticUpdate(void) {}

void UIButton_Draw(void)
{
    RSDK_THIS(UIButton);

    Vector2 drawPos;
    int32 size  = entity->size.y + entity->size.x;
    drawPos.x = entity->position.x - entity->field_140;
    drawPos.y = entity->position.y - entity->field_140;
    size >>= 16;
#if RETRO_USE_PLUS
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->dword138, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF,
                       (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->dword138, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif
    drawPos.x = entity->position.x + entity->field_140;
    drawPos.y = entity->position.y + entity->field_140;
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->dword138, 0, 0, 0, drawPos.x, drawPos.y);
    if (entity->dword14C) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.x = entity->field_140 + entity->position.x;
        drawPos.y = entity->field_140 + entity->position.y;
        drawPos.y += entity->field_13C;

        if (!entity->align) {
            drawPos.x += -0x60000 - (entity->size.x >> 1);
        }
        else if (entity->align == 2) {
            drawPos.x -= 0x60000;
            drawPos.x += entity->size.x >> 1;
        }
        if (entity->disabled) {
            if (!entity->align)
                drawPos.x += 0x150000;
        }
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
}

void UIButton_Create(void *data)
{
    RSDK_THIS(UIButton);
    if (!SceneInfo->inEditor) {
        entity->drawOrder       = 2;
        entity->visible         = !entity->invisible;
        entity->active          = ACTIVE_BOUNDS;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x400000;
        entity->dword138        = entity->size.y >> 16;
        entity->size.y          = abs(entity->size.y);
        entity->processButtonCB = UIButton_ProcessButtonInputs;
        entity->touchCB         = UIButton_ProcessTouch;
        entity->options3        = UIButton_Unknown15;
        entity->failCB          = UIButton_Fail;
        entity->options5        = UIButton_Unknown12;
        entity->options6        = UIButton_Unknown13;
        entity->options7        = UIButton_Unknown10;
        entity->options8        = UIButton_Unknown11;
        entity->dword14C        = 1;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->startListID     = entity->listID;
        entity->startFrameID    = entity->frameID;

        int32 entID = RSDK.GetEntityID(entity) - entity->choiceCount;
        for (int32 i = 0; i < entity->choiceCount; ++i) {
            EntityUIButton *item = RSDK.GetEntityByID(entID + i);
            if ((UIChoice && item->objectID == UIChoice->objectID) || (UIVsRoundPicker && item->objectID == UIVsRoundPicker->objectID)
                || (UIResPicker && item->objectID == UIResPicker->objectID) || (UIWinSize && item->objectID == UIWinSize->objectID)) {
                item->parent = (Entity *)entity;
            }

            if (i) {
                item->position.x = entity->posUnknown.x;
                item->position.y = entity->posUnknown.y;
                item->active     = ACTIVE_NEVER;
            }
            else {
                entity->posUnknown.x = item->position.x;
                entity->posUnknown.y = item->position.y;
            }
        }
    }
}

void UIButton_StageLoad(void) {}

void UIButton_Unknown1(EntityUIButton *button)
{
    for (int32 i = 0; i < button->choiceCount; ++i) {
        Entity *entity = RSDK.GetEntityByID(i % button->choiceCount - button->choiceCount + RSDK.GetEntityID(button));
        if (button->choiceCount > 0
            && (entity->objectID == UIChoice->objectID || entity->objectID == UIVsRoundPicker->objectID || entity->objectID == UIResPicker->objectID
                || entity->objectID == UIWinSize->objectID)) {
            entity->visible = i == button->selection;
            entity->active  = i == button->selection ? ACTIVE_NORMAL : ACTIVE_NEVER;
        }
    }
}

EntityUIButton *UIButton_GetChoicePtr(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount <= 0)
        return NULL;
    Entity *entity = RSDK.GetEntityByID(selection % button->choiceCount - button->choiceCount + RSDK.GetEntityID(button));
    if (entity->objectID == UIChoice->objectID || entity->objectID == UIVsRoundPicker->objectID || entity->objectID == UIResPicker->objectID
        || entity->objectID == UIWinSize->objectID) {
        return (EntityUIButton *)entity;
    }
    return NULL;
}

void UIButton_SetChoiceSelectionWithCB(EntityUIButton *button, int32 selection)
{
    if (button->choiceCount) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(button, button->selection);
        if (entPtr) {
            if (entPtr->objectID == UIChoice->objectID) {
                EntityUIChoice *choice = (EntityUIChoice *)entPtr;
                choice->field_134.x    = 0;
                choice->field_134.y    = 0;
                choice->flag           = false;
                choice->state          = UIChoice_Unknown6;
            }
            else if (entPtr->objectID == UIVsRoundPicker->objectID) {
                UIVsRoundPicker_Unknown4((EntityUIVsRoundPicker *)entPtr);
            }
            else if (entPtr->objectID == UIResPicker->objectID) {
                UIResPicker_Unknown3((EntityUIResPicker *)entPtr);
            }
            else if (entPtr->objectID == UIWinSize->objectID) {
                UIWinSize_Unknown3((EntityUIWinSize *)entPtr);
            }
            entPtr->active = ACTIVE_NEVER;
        }

        button->selection         = selection;
        EntityUIButton *newEntPtr = UIButton_GetChoicePtr(button, selection);
        if (newEntPtr) {
            if (button->state == UIButton_Unknown17 || button->state == UIButton_Unknown18) {
                UIChoice_Unknown1((EntityUIChoice *)newEntPtr);
            }
            else {
                newEntPtr->active  = ACTIVE_BOUNDS;
                newEntPtr->visible = false;
                UIChoice_Unknown2(newEntPtr);
            }
        }

        if (button->choiceChangeCB) {
            Entity *entStore       = SceneInfo->entity;
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
                EntityUIChoice *choice = (EntityUIChoice *)entPtr;
                choice->field_134.x    = 0;
                choice->field_134.y    = 0;
                choice->flag           = false;
                choice->state          = UIChoice_Unknown6;
            }
            else if (entPtr->objectID == UIVsRoundPicker->objectID) {
                UIVsRoundPicker_Unknown4((EntityUIVsRoundPicker *)entPtr);
            }
            else if (entPtr->objectID == UIResPicker->objectID) {
                UIResPicker_Unknown3((EntityUIResPicker *)entPtr);
            }
            else if (entPtr->objectID == UIWinSize->objectID) {
                UIWinSize_Unknown3((EntityUIWinSize *)entPtr);
            }
            entPtr->active = ACTIVE_NEVER;
        }

        button->selection         = selection;
        EntityUIButton *newEntPtr = UIButton_GetChoicePtr(button, selection);
        newEntPtr->active         = ACTIVE_NORMAL;
    }
}

void *UIButton_GetOptions2(void)
{
    RSDK_THIS(UIButton);
    EntityUIButton *entPtr = UIButton_GetChoicePtr(entity, entity->selection);
    if (!entPtr)
        return entity->options2;

    if (!UIChoice || entity->choiceCount <= 0 || !entPtr->options2) {
        return entity->options2;
    }
    return entPtr->options2;
}

void UIButton_Fail(void) { RSDK.PlaySfx(UIWidgets->sfxFail, 0, 255); }

void UIButton_Unknown6(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    UIControl_Unknown15(control, entity->position.x, entity->position.y);
    if (!UIControl_Unknown9(control)) {
        int32 rowID = 0;
        int32 colID = 0;
        if (control->rowCount && control->columnCount) {
            rowID = control->activeEntityID / control->columnCount;
        }
        else {
            rowID = 0;
        }

        if (control->columnCount) {
            colID = control->activeEntityID % control->columnCount;
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
            if (control->activeEntityID != id) {
                control->activeEntityID = id;
                StateMachine_Run(entity->options6);
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
            }
        }
        else {
            bool32 flag = true;
            if (UIControl->keyConfirm) {
                if (entity->disabled) {
                    StateMachine_Run(entity->failCB);
                }
                else {
                    flag = !entity->options2;
                }
            }

            if (flag) {
                if (!entity->flag) {
                    if (control->activeEntityID == UIControl_Unknown1(control, entity) && control->state == UIControl_ProcessInputs
                        && !control->dialogHasFocus) {
                        StateMachine_Run(entity->options5);
                    }
                }
            }
            else {
                StateMachine_Run(entity->options3);
            }
        }
    }
}

bool32 UIButton_TouchCB_Alt(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    bool32 touchFlag = false;
    int32 lastTouchID  = -1;
    uint32 lastTouch   = 0xFFFFFFFF;

    for (int32 i = 0; i < entity->touchPosCount; ++i) {
        Vector2 touchPos1 = entity->touchPos1[i];
        Vector2 touchPos2 = entity->touchPos2[i];

        if (TouchInfo->count) {
            int32 screenX = ScreenInfo->position.x << 16;
            int32 screenY = ScreenInfo->position.y << 16;
            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 x = abs(touchPos2.x + entity->position.x - (screenX - (int32)((TouchInfo->x[t] * ScreenInfo->width) * -65536.0f)));
                int32 y = abs(touchPos2.y + entity->position.y - (screenY - (int32)((TouchInfo->y[t] * ScreenInfo->height) * -65536.0f)));

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
            if (entity->touchPressed && entity->touchCountUnknown == i && !entity->disabled) {
                if (!UIControl_Unknown9(control)) {
                    StateMachine_Run(entity->touchPosCallbacks[i]);
                }
            }
        }
    }

    entity->touchCountUnknown = lastTouchID;
    entity->touchPressed      = touchFlag;
    return entity->touchPressed;
}

bool32 UIButton_ProcessTouch(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)entity->parent;

    bool32 touchFlag = false;
    if (entity->objectID != UIButton->objectID || !entity->invisible) {
        if (TouchInfo->count) {
            int32 screenX = (ScreenInfo->position.x << 16);
            int32 screenY = (ScreenInfo->position.y << 16);
            int32 sizeX   = entity->touchPosStart.x >> 1;
            int32 sizeY   = entity->touchPosStart.y >> 1;

            for (int32 i = 0; i < TouchInfo->count; ++i) {
                int32 x = screenX - ((TouchInfo->x[i] * ScreenInfo->width) * -65536.0f);
                int32 y = screenY - ((TouchInfo->y[i] * ScreenInfo->height) * -65536.0f);

                int32 touchX = abs(entity->touchPosEnd.x + entity->position.x - x);
                int32 touchY = abs(entity->touchPosEnd.y + entity->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    touchFlag = true;
                }
            }
        }
        else {
            if (entity->touchPressed && !UIControl_Unknown9(control)) {
                if (entity->disabled) {
                    StateMachine_Run(entity->failCB);
                }
                else {
                    entity->flag = false;
                    void *cb     = NULL;
                    if (entity->objectID == UIButton->objectID)
                        cb = UIButton_GetOptions2();
                    else
                        cb = entity->options2;

                    if (cb) {
                        StateMachine_Run(entity->options3);
                    }
                }
            }
        }
    }

    if (!touchFlag) {
        if (!entity->touchPressed && entity->options7()) {
            for (int32 i = 0; i < control->buttonCount; ++i) {
                if (entity == control->buttons[i] && control->activeEntityID != i) {
                    entity->flag = false;
                    StateMachine_Run(entity->options6);
                    break;
                }
            }
        }
    }

    bool32 childTouchFlag = false;
    entity->touchPressed  = touchFlag;
    if (entity->objectID == UIButton->objectID && entity->choiceCount > 0) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(entity, entity->selection);
        if (entPtr) {
            Entity *entStore       = SceneInfo->entity;
            SceneInfo->entity = (Entity *)entPtr;
            if (entPtr->touchCB) {
                childTouchFlag = entPtr->touchCB();
            }
            SceneInfo->entity = entStore;
        }
    }
    return entity->touchPressed || childTouchFlag;
}

void UIButton_ProcessButtonInputs(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    EntityUIButton *entPtr   = UIButton_GetChoicePtr(entity, entity->selection);

    int32 columnID = 0, rowID = 0;
    if (control->rowCount && control->columnCount)
        rowID = control->activeEntityID / control->columnCount;
    else
        rowID = 0;

    if (control->columnCount)
        columnID = control->activeEntityID % control->columnCount;
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

    int32 selection = entity->selection;
    bool32 flag2  = 0;
    if (entPtr && entity->choiceCount == 1 && entPtr->processButtonCB && !entity->choiceDir && !entity->disabled) {
        Entity *entStore       = SceneInfo->entity;
        SceneInfo->entity = (Entity *)entPtr;
        entPtr->processButtonCB();
        SceneInfo->entity = entStore;
    }
    else {
        if (UIControl->keyLeft) {
            if (entity->choiceCount <= 0 || entity->choiceDir || entity->disabled) {
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
                            selection += entity->choiceCount;
                        while (selection < 0);
                    }
                } while (UIButton_GetChoicePtr(entity, selection)->disabled && selection != entity->selection);
                flag2 = true;
            }
        }

        if (UIControl->keyRight) {
            if (entity->choiceCount <= 0 || entity->choiceDir || entity->disabled) {
                if (control->columnCount > 1) {
                    ++columnID;
                    flag = true;
                }
                flag2 = false;
            }
            else {
                do
                    selection = (selection + 1) % entity->choiceCount;
                while (UIButton_GetChoicePtr(entity, selection)->disabled && selection != entity->selection);
                flag2 = true;
            }
        }
    }

    if (flag2) {
        if (selection < 0)
            selection += entity->choiceCount;

        if (selection >= entity->choiceCount)
            selection -= entity->choiceCount;
        if (selection != entity->selection) {
            UIButton_SetChoiceSelectionWithCB(entity, selection);
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
        if (control->activeEntityID != id && entity != control->buttons[id]) {
            control->activeEntityID = id;
            UIButton_Unknown13();
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
    }
    else {
        void *options2 = UIButton_GetOptions2();

        if (UIControl->keyConfirm && (!UIChoice || options2)) {
            if (entity->disabled || (entity->choiceCount > 0 && entPtr->disabled))
                RSDK.PlaySfx(UIWidgets->sfxFail, false, 255);
            else
                UIButton_Unknown15();
        }
        else {
            if (entity->state != UIButton_Unknown17 && entity->state != UIButton_Unknown18) {
                if (control->activeEntityID == columnID + rowID * control->columnCount && control->state == UIControl_ProcessInputs)
                    UIButton_Unknown12();
            }
        }
    }
}

bool32 UIButton_Unknown10(void)
{
    RSDK_THIS(UIButton);
    return entity->state == UIButton_Unknown17;
}

bool32 UIButton_Unknown11(void)
{
    RSDK_THIS(UIButton);
    return entity->state == UIButton_Unknown18;
}

void UIButton_Unknown12(void)
{
    RSDK_THIS(UIButton);
    if (entity->state != UIButton_Unknown17) {
        entity->field_13C = 0;
        entity->field_140 = 0;
        entity->field_144 = -0x20000;
        entity->field_148 = -0x20000;
        entity->state     = UIButton_Unknown17;

        if (UIChoice) {
            EntityUIButton *entPtr = UIButton_GetChoicePtr(entity, entity->selection);

            if (entPtr) {
                if (entPtr->objectID == UIChoice->objectID) {
                    UIChoice_Unknown1((EntityUIChoice *)entPtr);
                }
                else {
                    EntityUIVsRoundPicker *roundPicker = (EntityUIVsRoundPicker *)entPtr;
                    if (roundPicker->objectID == UIVsRoundPicker->objectID) {
                        roundPicker->active    = ACTIVE_BOUNDS;
                        roundPicker->field_124 = 0;
                        roundPicker->field_128 = 0;
                        roundPicker->field_12C = -0x20000;
                        roundPicker->field_130 = -0x20000;
                        roundPicker->flag      = true;
                        roundPicker->state     = UIVsRoundPicker_Unknown9;
                    }
                    else {
                        EntityUIResPicker *resPicker = (EntityUIResPicker *)entPtr;
                        if (resPicker->objectID == UIResPicker->objectID) {
                            resPicker->active    = ACTIVE_BOUNDS;
                            resPicker->visible   = true;
                            resPicker->field_120 = 0;
                            resPicker->offset    = 0;
                            resPicker->field_128 = -0x20000;
                            resPicker->field_12C = -0x20000;
                            resPicker->flag      = true;
                            resPicker->state     = UIResPicker_Unknown8;
                        }
                        else {
                            EntityUIWinSize *winSize = (EntityUIWinSize *)entPtr;
                            if (winSize->objectID == UIWinSize->objectID) {
                                winSize->active    = ACTIVE_BOUNDS;
                                winSize->visible   = true;
                                winSize->field_120 = 0;
                                winSize->offset    = 0;
                                winSize->field_128 = -0x20000;
                                winSize->field_12C = -0x20000;
                                winSize->flag      = true;
                                winSize->state     = UIWinSize_Unknown8;
                            }
                        }
                    }
                }
            }
        }
    }
}

void UIButton_Unknown13(void)
{
    RSDK_THIS(UIButton);

    entity->state = UIButton_Unknown16;
    if (UIChoice) {
        EntityUIButton *entPtr = UIButton_GetChoicePtr(entity, entity->selection);
        if (entPtr) {
            if (entPtr->objectID == UIChoice->objectID) {
                ((EntityUIChoice *)entPtr)->field_134.x = 0;
                ((EntityUIChoice *)entPtr)->field_134.y = 0;
                entPtr->flag                            = false;
                entPtr->state                           = UIChoice_Unknown6;
            }
            else if (entPtr->objectID == UIVsRoundPicker->objectID) {
                UIVsRoundPicker_Unknown4((EntityUIVsRoundPicker *)entPtr);
            }
            else if (entPtr->objectID == UIResPicker->objectID) {
                UIResPicker_Unknown3((EntityUIResPicker *)entPtr);
            }
            else if (entPtr->objectID == UIWinSize->objectID) {
                UIWinSize_Unknown3((EntityUIWinSize *)entPtr);
            }
        }
    }
}

void UIButton_Unknown15(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    EntityUIButton *entPtr  = UIButton_GetChoicePtr(entity, entity->selection);

    if (entity->field_150 || (entPtr && entPtr->objectID == UIChoice->objectID && entPtr->field_148))
        parent->state = StateMachine_None;

    if (entity->assignsP1) {
#if RETRO_USE_PLUS
        int32 id = API_MostRecentActiveControllerID(0, 0, 0);
#else
        int32 id = API_MostRecentActiveControllerID(0);
#endif
        API_ResetControllerAssignments();
        API_AssignControllerID(CONT_P1, id);
    }

    if (entity->freeBindP2)
        API_AssignControllerID(CONT_P2, CONT_AUTOASSIGN);

    parent->backoutTimer = 30;
    if (entity->transition) {
        StateMachine(callback) = entity->options2;
        if (UIChoice && entity->choiceCount > 0 && entPtr) {
            callback = entPtr->options2;
        }

        UITransition_StartTransition(callback, 14);
    }

    if (entity->stopMusic)
        RSDK.StopChannel(Music->channelID);
    entity->timer = 0;
    entity->state = UIButton_Unknown18;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIButton_Unknown16(void)
{
    RSDK_THIS(UIButton);

    if (entity->field_13C) {
        int32 val = -(entity->field_13C / abs(entity->field_13C));
        entity->field_13C += val << 16;
        if (val < 0 && entity->field_13C < 0) {
            entity->field_13C = 0;
        }
        else if (val > 0 && entity->field_13C > 0)
            entity->field_13C = 0;
    }

    if (entity->field_140) {
        int32 val = -(entity->field_140 / abs(entity->field_140));
        entity->field_140 += val << 16;
        if (val < 0 && entity->field_140 < 0) {
            entity->field_140 = 0;
        }
        else if (val > 0 && entity->field_140 > 0)
            entity->field_140 = 0;
    }
}

void UIButton_Unknown17(void)
{
    RSDK_THIS(UIButton);

    entity->field_144 += 0x4000;
    entity->field_13C += entity->field_144;

    if (entity->field_13C >= 0 && entity->field_144 > 0) {
        entity->field_13C = 0;
        entity->field_144 = 0;
    }

    entity->field_148 += 0x4800;
    entity->field_140 += entity->field_148;
    if (entity->field_140 >= -0x20000 && entity->field_148 > 0) {
        entity->field_140 = -0x20000;
        entity->field_148 = 0;
    }
}

void UIButton_Unknown18(void)
{
    RSDK_THIS(UIButton);
    UIButton_Unknown17();
    if (++entity->timer == 30) {
        entity->timer = 0;
        if (!entity->transition) {
            StateMachine(options2) = UIButton_GetOptions2();
            StateMachine_Run(options2);
        }
        entity->state = UIButton_Unknown17;
    }

    entity->dword14C = !((entity->timer >> 1) & 1);
}

#if RETRO_INCLUDE_EDITOR
void UIButton_EditorDraw(void)
{
    RSDK_THIS(UIButton);
    if (entity->disabled)
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &entity->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    entity->startListID     = entity->listID;
    entity->startFrameID    = entity->frameID;

    entity->drawOrder       = 2;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x400000;
    entity->dword138        = entity->size.y >> 16;
    entity->size.y          = abs(entity->size.y);
    entity->dword14C        = 1;

    entity->inkEffect = entity->invisible ? INK_BLEND : INK_NONE;
    UIButton_Draw();
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
