// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIDialog Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIDialog *UIDialog;

void UIDialog_Update(void)
{
    RSDK_THIS(UIDialog);

    StateMachine_Run(self->state);

    UIDialog_HandleButtonPositions();

    if (self->closeDelay > 0) {
        if (!--self->closeDelay)
            UIDialog_HandleAutoClose();
    }
}

void UIDialog_LateUpdate(void) {}

void UIDialog_StaticUpdate(void) {}

void UIDialog_Draw(void)
{
    RSDK_THIS(UIDialog);

    UIDialog_DrawBGShapes();

    Vector2 drawPos;
    switch (self->lineCount) {
        case 0:
        case 1:
            drawPos.x = (self->position.x + 0x100000) + 0x100000;
            drawPos.y = self->position.y - 0x200000;
            break;

        case 2:
            drawPos.x = (self->position.x + 0x100000) + 0x180000;
            drawPos.y = self->position.y - 0x280000;
            break;

        case 3:
            drawPos.x = (self->position.x + 0x100000) + 0x200000;
            drawPos.y = self->position.y - 0x300000;
            break;

        default: break;
    }

    drawPos.x += self->dialogPos.x;
    drawPos.y += self->dialogPos.y;

    int32 w     = 0;
    int32 count = self->lineCount + 1;
    if (count > 0) {
        for (int32 i = 0; i < count; ++i) {
            int32 start = !i ? 0 : self->lineLength[i - 1] + 1;
            int32 len   = i >= self->lineCount ? self->textInfo.length : self->lineLength[i];

            int32 width = RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->textInfo, start, len, 0);
            if (width > w)
                w = width;
        }
    }

    for (int32 i = 0; i < count; ++i) {
        int32 offset = -0x8000 * w;

        int32 start = !i ? 0 : self->lineLength[i - 1] + 1;
        int32 len   = i >= self->lineCount ? self->textInfo.length : self->lineLength[i];

        RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->textInfo, start, len, 0);

        drawPos.x += offset;
        RSDK.DrawText(&self->animator, &drawPos, &self->textInfo, start, len, 0, 0, NULL, NULL, false);

        drawPos.y += 0x120000;
        drawPos.x = drawPos.x - offset - 0x120000;
    }
}

void UIDialog_Create(void *data)
{
    RSDK_THIS(UIDialog);

    self->active    = ACTIVE_ALWAYS;
    self->drawGroup = 15;
    self->visible   = true;

    if (data)
        UIDialog_SetupText(self, data);

    self->buttonCount = 0;
}

void UIDialog_StageLoad(void)
{
    UIDialog->controlStore = NULL;
    UIDialog->activeDialog = NULL;
}

EntityUIDialog *UIDialog_CreateActiveDialog(String *msg)
{
    if (UIDialog->activeDialog) {
        LogHelpers_Print("EXCEPTION: Called CreateDialog when an activeDialog already existed.");
    }
    else {
        int32 id = RSDK_GET_ENTITY(SLOT_DIALOG, UIDialog)->classID;

        if (id) {
            LogHelpers_Print("Can't create UIDialog (%d), entity already exists in slot (class ID: %d)", UIDialog->classID, id);
        }
        else {
            RSDK.ResetEntitySlot(SLOT_DIALOG, UIDialog->classID, msg);

            EntityUIDialog *dialog = RSDK_GET_ENTITY(SLOT_DIALOG, UIDialog);
            dialog->position.x     = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
            dialog->position.y     = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
            UIDialog->activeDialog = dialog;

            return dialog;
        }
    }

    return NULL;
}

void UIDialog_SetupText(EntityUIDialog *dialog, String *text)
{
    if (text) {
        dialog->lineCount = 0;
        int32 charPos     = 0;

        for (int32 i = 0; i < text->length; ++i) {
            if (text->chars[charPos] == '\n' && dialog->lineCount < 3) {
                dialog->lineLength[dialog->lineCount] = charPos;
                ++dialog->lineCount;
            }

            ++charPos;
        }

        RSDK.CopyString(&dialog->textInfo, text);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &dialog->animator, true, 0);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &dialog->textInfo);
    }
}

void UIDialog_AddButton(uint8 frame, EntityUIDialog *dialog, void (*callback)(void), bool32 closeOnSelect)
{
    int32 id = dialog->buttonCount;

    if (dialog->buttonCount < UIDIALOG_OPTION_COUNT) {
        dialog->buttonFrames[dialog->buttonCount]  = frame;
        dialog->callbacks[dialog->buttonCount]     = callback;
        dialog->closeOnSelect[dialog->buttonCount] = closeOnSelect;

        RSDK.ResetEntitySlot(SLOT_DIALOG_BUTTONS + dialog->buttonCount, UIButton->classID, NULL);

        EntityUIButton *button = RSDK_GET_ENTITY(SLOT_DIALOG_BUTTONS + dialog->buttonCount, UIButton);
        button->position.x     = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        button->position.y     = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 9, &button->animator, true, frame);
        button->textFrames = UIWidgets->textFrames;

        if (frame == DIALOG_CONTINUE)
            button->size.x = 0x640000;
        else
            button->size.x = 0x320000;
        button->size.y                         = 0x180000;
        button->actionCB                       = UIDialog_ButtonActionCB;
        button->bgEdgeSize                     = 24;
        button->align                          = UIBUTTON_ALIGN_CENTER;
        button->active                         = ACTIVE_ALWAYS;
        button->drawGroup                      = dialog->drawGroup;
        dialog->buttons[dialog->buttonCount++] = button;

        EntityUIControl *parent = dialog->parent;
        if (parent) {
            button->parent      = (Entity *)parent;
            parent->buttons[id] = button;
            parent->buttonCount = dialog->buttonCount;
        }
    }
}

void UIDialog_Setup(EntityUIDialog *dialog)
{
    if (dialog) {
        bool32 tookFocus = false;

        Vector2 size;
        size.x = ScreenInfo->size.x << 16;
        size.y = ScreenInfo->size.y << 16;
        foreach_all(UIControl, control)
        {
            if (control->active == ACTIVE_ALWAYS) {
                tookFocus                   = true;
                control->dialogHasFocus     = true;
                UIDialog->controlStore      = control;
                UIDialog->controlStateStore = control->state;
                foreach_break;
            }
        }

        control = NULL;
        RSDK.ResetEntitySlot(SLOT_DIALOG_UICONTROL, UIControl->classID, &size);

        control = RSDK_GET_ENTITY(SLOT_DIALOG_UICONTROL, UIControl);
#if MANIA_USE_PLUS
        control->menuWasSetup = true;
#endif
        control->position.x        = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        control->position.y        = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
        control->rowCount          = 1;
        control->columnCount       = dialog->buttonCount;
        control->buttonID          = 0;
        control->backPressCB       = UIDialog_HandleAutoClose;
        control->selectionDisabled = true;

        dialog->parent = control;
        if (!tookFocus) {
            UIDialog->controlStore      = NULL;
            UIDialog->controlStateStore = StateMachine_None;
        }

        int32 i = 0;
        for (; i < UIDIALOG_OPTION_COUNT; ++i) {
            if (!dialog->buttons[i])
                break;

            dialog->buttons[i]->parent = (Entity *)control;
            control->buttons[i]        = dialog->buttons[i];
        }

        control->buttonCount = i;
        dialog->timer        = 0;
        dialog->state        = UIDialog_State_Appear;
    }
}

void UIDialog_CloseOnSel_HandleSelection(EntityUIDialog *dialog, void (*callback)(void))
{
    if (dialog && dialog->state != UIDialog_State_Close) {
        dialog->parent->selectionDisabled = true;
        dialog->timer                     = 0;
        dialog->state                     = UIDialog_State_Close;
        dialog->closeCB                   = callback;
    }
}

void UIDialog_DrawBGShapes(void)
{
    RSDK_THIS(UIDialog);

    RSDK.DrawRect(((ScreenInfo->position.x + ScreenInfo->center.x) << 16) - (self->bgRectSize.x >> 1),
                  ((ScreenInfo->position.y + ScreenInfo->center.y) << 16) - (self->bgRectSize.y >> 1), self->bgRectSize.x, self->bgRectSize.y,
                  self->useAltColor ? 0x282028 : 0x000000, 0xFF, INK_NONE, false);

    UIWidgets_DrawParallelogram(self->dialogPos.x + ((ScreenInfo->position.x + ScreenInfo->center.x) << 16),
                                self->dialogPos.y + ((ScreenInfo->position.y + ScreenInfo->center.y) << 16), 0xC8, 0x8F, 0x8F, 0x30, 0xA0, 0xF0);
}

void UIDialog_HandleButtonPositions(void)
{
    RSDK_THIS(UIDialog);

    int32 offsets[] = { 0, 0, 0x80, 0x70 };

    int32 offset = offsets[self->buttonCount] << 16;
    int32 x      = self->position.x - 0x240000 + self->dialogPos.x - ((offset * MAX(self->buttonCount - 1, 0)) >> 1);
    int32 y      = self->position.y + 0x2C0000 + self->dialogPos.y;

    for (int32 i = 0; i < UIDIALOG_OPTION_COUNT; ++i) {
        if (!self->buttons[i])
            break;

        EntityUIButton *button = self->buttons[i];
        button->startPos.x     = x;
        button->startPos.y     = y;
        button->position.x     = x;
        button->position.y     = y;

        x += offset;
    }
}

void UIDialog_Close(void)
{
    RSDK_THIS(UIDialog);

    EntityUIControl *control = self->parent;

    if (control) {
        UIControl_SetInactiveMenu(control);
        destroyEntity(control);
    }

    for (int32 i = 0; i < UIDIALOG_OPTION_COUNT; ++i) {
        if (self->buttons[i])
            destroyEntity(self->buttons[i]);
    }

    EntityUIControl *storedControl = UIDialog->controlStore;
    if (storedControl) {
        UIControl_SetMenuLostFocus(UIDialog->controlStore);
        storedControl->state          = UIDialog->controlStateStore;
        storedControl->dialogHasFocus = false;
    }

    UIDialog->controlStore      = NULL;
    UIDialog->controlStateStore = StateMachine_None;
    UIDialog->activeDialog      = NULL;

    StateMachine_Run(self->closeCB);
    destroyEntity(self);
}

bool32 UIDialog_HandleAutoClose(void)
{
    EntityUIDialog *entity   = UIDialog->activeDialog;
    EntityUIControl *control = entity->parent;

    for (int32 i = 0; i < control->buttonCount; ++i) {
        int32 frame = entity->buttonFrames[i];
        if (frame == DIALOG_NO || frame == DIALOG_CANCEL) {
            if (entity->closeOnSelect[i]) {
                UIDialog_CloseOnSel_HandleSelection(entity, entity->callbacks[i]);
            }
            else {
                StateMachine_Run(entity->callbacks[i]);
            }

            return true;
        }
    }

    return false;
}

void UIDialog_ButtonActionCB(void)
{
    EntityUIDialog *entity = UIDialog->activeDialog;

    if (entity->parent) {
        int32 id = entity->parent->buttonID;

        if (id >= 0 && id < entity->parent->buttonCount) {
            if (entity->closeOnSelect[id]) {
                UIDialog_CloseOnSel_HandleSelection(entity, entity->callbacks[id]);
            }
            else {
                StateMachine_Run(entity->callbacks[id]);
            }
        }
    }
}

void UIDialog_State_Appear(void)
{
    RSDK_THIS(UIDialog);

    Vector2 pos;
    pos.x = 0;
    pos.y = 0;

    if (self->timer == 1) {
        RSDK.PlaySfx(UIWidgets->sfxWoosh, false, 255);
        UIControl_HandleMenuLoseFocus(self->parent);
    }

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            if (self->timer >= 26) {
                self->parent->selectionDisabled = false;
                self->timer                     = 0;
                self->state                     = UIDialog_State_Idle;
            }
            else {
                self->bgRectSize.x = ScreenInfo->size.x << 16;
                self->bgRectSize.y = 0x900000;
                MathHelpers_Lerp2Sin1024(&pos, MAX(((self->timer - 16) << 8) / 10, 0), -0x400000 - (ScreenInfo->size.x << 16), 0, 0, 0);
                self->dialogPos = pos;

                if (self->timer - 16 == 1 && self->playEventSfx)
                    RSDK.PlaySfx(UIWidgets->sfxEvent, false, 255);

                ++self->timer;
            }
        }
        else {
            self->dialogPos.x = -0x400000 - (ScreenInfo->size.x << 16);
            self->dialogPos.y = 0;
            MathHelpers_Lerp(&pos, MAX(((self->timer - 8) << 8) / 8, 0), ScreenInfo->size.x << 16, 0x10000, ScreenInfo->size.x << 16, 0x900000);
            self->bgRectSize = pos;

            self->timer++;
        }
    }
    else {
        self->dialogPos.x = -0x400000 - (ScreenInfo->size.x << 16);
        self->dialogPos.y = 0;
        MathHelpers_Lerp2Sin1024(&pos, MAX((self->timer << 8) / 8, 0), 0, 0x10000, ScreenInfo->size.x << 16, 0x10000);
        self->bgRectSize = pos;

        ++self->timer;
    }
}

void UIDialog_State_Idle(void)
{
    RSDK_THIS(UIDialog);

    self->bgRectSize.x = ScreenInfo->size.x << 16;
    self->bgRectSize.y = 0x900000;
    self->dialogPos.x  = 0;
    self->dialogPos.y  = 0;
}

void UIDialog_State_Close(void)
{
    RSDK_THIS(UIDialog);

    if (self->timer >= 8) {
        if (self->timer >= 16) {
            UIDialog_Close();
        }
        else {
            self->dialogPos.x = (ScreenInfo->size.x + 64) << 16;
            self->dialogPos.y = 0;
            MathHelpers_Lerp2Sin1024(&self->bgRectSize, MAX(((self->timer - 8) << 8) / 8, 0), ScreenInfo->size.x << 16, 0x900000,
                                     ScreenInfo->size.x << 16, 0);

            ++self->timer;
        }
    }
    else {
        self->bgRectSize.x = ScreenInfo->size.x << 16;
        self->bgRectSize.y = 0x900000;
        MathHelpers_LerpSin1024(&self->dialogPos, MAX((self->timer << 8) / 8, 0), 0, 0, (ScreenInfo->size.x + 64) << 16, 0);

        ++self->timer;
    }
}

EntityUIDialog *UIDialog_CreateDialogOk(String *text, void (*callback)(void), bool32 closeOnSelect)
{
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(text);

    if (dialog) {
        UIDialog_AddButton(DIALOG_OK, dialog, callback, closeOnSelect);
        UIDialog_Setup(dialog);
    }

    return dialog;
}
EntityUIDialog *UIDialog_CreateDialogYesNo(String *text, void (*callbackYes)(void), void (*callbackNo)(void), bool32 closeOnSelect_Yes,
                                           bool32 closeOnSelect_No)
{
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(text);

    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, callbackNo, closeOnSelect_No);
        UIDialog_AddButton(DIALOG_YES, dialog, callbackYes, closeOnSelect_Yes);
        UIDialog_Setup(dialog);
    }

    return dialog;
}
EntityUIDialog *UIDialog_CreateDialogOkCancel(String *text, void (*callbackOk)(void), void (*callbackCancel)(void), bool32 closeOnSelect_Ok,
                                              bool32 closeOnSelect_Cancel)
{
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(text);

    if (dialog) {
        UIDialog_AddButton(DIALOG_OK, dialog, callbackOk, closeOnSelect_Ok);
        UIDialog_AddButton(DIALOG_CANCEL, dialog, callbackCancel, closeOnSelect_Cancel);
        UIDialog_Setup(dialog);
    }

    return dialog;
}

#if GAME_INCLUDE_EDITOR
void UIDialog_EditorDraw(void) {}

void UIDialog_EditorLoad(void) {}
#endif

void UIDialog_Serialize(void) {}
