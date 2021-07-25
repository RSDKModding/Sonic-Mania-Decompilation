#include "SonicMania.h"

ObjectUIDialog *UIDialog;

void UIDialog_Update(void)
{
    RSDK_THIS(UIDialog);
    StateMachine_Run(entity->state);
    UIDialog_Unknown7();

    if (entity->timer > 0) {
        entity->timer--;
        if (entity->timer == 1)
            UIDialog_Unknown9();
    }
}

void UIDialog_LateUpdate(void) {}

void UIDialog_StaticUpdate(void) {}

void UIDialog_Draw(void)
{
    RSDK_THIS(UIDialog);
    UIDialog_Unknown6();

    Vector2 drawPos;
    switch (entity->lineCount) {
        case 0:
        case 1:
            drawPos.x = (entity->position.x + 0x100000) + 0x100000;
            drawPos.y = entity->position.y - 0x200000;
            break;
        case 2:
            drawPos.x = (entity->position.x + 0x100000) + 0x180000;
            drawPos.y = entity->position.y - 0x280000;
            break;
        case 3:
            drawPos.x = (entity->position.x + 0x100000) + 0x200000;
            drawPos.y = entity->position.y - 0x300000;
            break;
        default: break;
    }
    drawPos.x += entity->drawPos.x;
    drawPos.y += entity->drawPos.y;

    int w     = 0;
    int count = entity->lineCount + 1;
    if (count <= 0) {
        count = entity->lineCount + 1;
    }
    else {
        for (int i = 0; i < count; ++i) {
            int start = 0;
            if (i)
                start = entity->lineLength[i - 1] + 1;

            int len = 0;
            if (i >= entity->lineCount)
                len = entity->textInfo.textLength;
            else
                len = entity->lineLength[i];
            int width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->textInfo, start, len, 0);
            if (width > w)
                w = width;
        }
    }

    for (int i = 0; i < count; ++i) {
        int offset = -0x8000 * w;

        int start = 0;
        if (i)
            start = entity->lineLength[i - 1] + 1;

        int len = 0;
        if (i >= entity->lineCount)
            len = entity->textInfo.textLength;
        else
            len = entity->lineLength[i];
        RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->textInfo, start, len, 0);
        drawPos.x += offset;
        RSDK.DrawText(&entity->animator, &drawPos, &entity->textInfo, start, len, 0, 0, 0, NULL, false);
        drawPos.y += 0x120000;
        drawPos.x = drawPos.x - offset - 0x120000;
    }
}

void UIDialog_Create(void *data)
{
    RSDK_THIS(UIDialog);
    entity->active    = ACTIVE_ALWAYS;
    entity->drawOrder = 15;
    entity->visible   = true;
    if (data)
        UIDialog_SetupText(entity, data);
    entity->id = 0;
}

void UIDialog_StageLoad(void)
{
    UIDialog->controlStore = NULL;
    UIDialog->activeDialog = NULL;
}

EntityUIDialog *UIDialog_CreateActiveDialog(void *msg)
{
    if (UIDialog->activeDialog) {
        LogHelpers_Print("EXCEPTION: Called CreateDialog when an activeDialog already existed.");
    }
    else {
        int id = RSDK_GET_ENTITY(SLOT_DIALOG, UIDialog)->objectID;
        if (id) {
            LogHelpers_Print("Can't create UIDialog (%d), entity already exists in slot (class ID: %d)", UIDialog->objectID, id);
        }
        else {
            RSDK.ResetEntitySlot(SLOT_DIALOG, UIDialog->objectID, msg);
            EntityUIDialog *dialog = RSDK_GET_ENTITY(SLOT_DIALOG, UIDialog);
            dialog->position.x     = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
            dialog->position.y     = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
            UIDialog->activeDialog = dialog;
            return dialog;
        }
    }
    return NULL;
}

void UIDialog_SetupText(EntityUIDialog *dialog, TextInfo *text)
{
    if (text) {
        dialog->lineCount = 0;
        int charPos       = 0;
        for (int i = 0; i < text->textLength; ++i) {
            if (text->text[charPos] == '\n' && dialog->lineCount < 3) {
                dialog->lineLength[dialog->lineCount] = charPos;
                ++dialog->lineCount;
            }
            ++charPos;
        }
        RSDK.CopyString(&dialog->textInfo, text);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &dialog->animator, true, 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &dialog->textInfo);
    }
}

void UIDialog_AddButton(byte frame, EntityUIDialog *dialog, int (*callback)(void), bool32 flag)
{
    int id = dialog->id;
    if (dialog->id < 3) {
        dialog->buttonFrames[dialog->id] = frame;
        dialog->callbacks[dialog->id]    = callback;
        dialog->flags[dialog->id]        = flag;
        RSDK.ResetEntitySlot(dialog->id + 23, UIButton->objectID, NULL);
        EntityUIButton *button = RSDK.GetEntityByID(dialog->id + 23);
        button->position.x     = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        button->position.y     = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 9, &button->animator, true, frame);
        button->textSpriteIndex = UIWidgets->textSpriteIndex;

        if (frame == 4)
            button->size.x = 0x640000;
        else
            button->size.x = 0x320000;
        button->size.y              = 0x180000;
        button->options2            = UIDialog_Unknown10;
        button->dword138            = 24;
        button->align               = 1;
        button->active              = ACTIVE_ALWAYS;
        button->drawOrder           = dialog->drawOrder;
        dialog->entPtrs[dialog->id] = button;
        ++dialog->id;
        EntityUIControl *parent = dialog->parent;
        if (parent) {
            button->parent        = (Entity *)parent;
            parent->entities[id]  = button;
            parent->unknownCount1 = dialog->id;
        }
    }
}

void UIDialog_Setup(EntityUIDialog *dialog)
{
    if (dialog) {
        bool32 flag = false;
        Vector2 size;
        size.x = RSDK_screens->width << 16;
        size.y = RSDK_screens->height << 16;
        foreach_all(UIControl, control)
        {
            if (control->active == ACTIVE_ALWAYS) {
                flag                        = true;
                control->dialogHasFocus     = true;
                UIDialog->controlStore      = control;
                UIDialog->controlStateStore = control->state;
                foreach_break;
            }
        }

        control = NULL;
        RSDK.ResetEntitySlot(SLOT_DIALOG_UICONTROL, UIControl->objectID, &size);
        control                      = RSDK_GET_ENTITY(SLOT_DIALOG_UICONTROL, UIControl);
        control->dwordCC             = 1;
        control->position.x          = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        control->position.y          = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        control->rowCount            = 1;
        control->columnCount         = dialog->id;
        control->activeEntityID      = 0;
        control->backPressCB         = UIDialog_Unknown9;
        control->selectionDisabled = true;
        dialog->parent               = control;
        if (!flag) {
            UIDialog->controlStore      = NULL;
            UIDialog->controlStateStore = StateMachine_None;
        }

        int i = 0;
        for (; i < 3; ++i) {
            if (!dialog->entPtrs[i])
                break;
            dialog->entPtrs[i]->parent = (Entity *)control;
            control->entities[i]       = dialog->entPtrs[i];
        }
        control->unknownCount1 = i;
        dialog->field_5C       = 0;
        dialog->state          = UIDialog_Unknown11;
    }
}

void UIDialog_Unknown4(EntityUIDialog *entity, void (*callback)(void))
{
    if (entity) {
        if (entity->state != UIDialog_Unknown13) {
            entity->parent->selectionDisabled = true;
            entity->field_5C                    = 0;
            entity->state                       = UIDialog_Unknown13;
            entity->curCallback                 = callback;
        }
    }
}

void UIDialog_Unknown6(void)
{
    RSDK_THIS(UIDialog);
    RSDK.DrawRect(((RSDK_screens->position.x + RSDK_screens->centerX) << 16) - (entity->field_70.x >> 1),
                  ((RSDK_screens->position.y + RSDK_screens->centerY) << 16) - (entity->field_70.y >> 1), entity->field_70.x, entity->field_70.y,
                  entity->field_B8 ? 0x282028 : 0, 255, INK_NONE, false);
    UIWidgets_Unknown7(0x8F, 0xC8, 0x8F, 0x30, 0xA0, 0xF0, entity->drawPos.x + ((RSDK_screens->position.x + RSDK_screens->centerX) << 16),
                       entity->drawPos.y + ((RSDK_screens->position.y + RSDK_screens->centerY) << 16));
}

void UIDialog_Unknown7(void)
{
    int offsets[] = { 0, 0, 0x80, 0x70 };

    RSDK_THIS(UIDialog);
    int offset = offsets[entity->id] << 16;
    int x      = entity->position.x - 0x240000 + entity->drawPos.x - ((offset * max(entity->id - 1, 0)) >> 1);
    int y      = entity->position.y + 0x2C0000 + entity->drawPos.y;

    for (int i = 0; i < 3; ++i) {
        if (!entity->entPtrs[i])
            break;
        EntityUIButton *button = entity->entPtrs[i];
        button->posUnknown2.x  = x;
        button->position.x     = x;
        button->posUnknown2.y  = y;
        button->position.y     = y;
        x += offset;
    }
}

void UIDialog_Close(void)
{
    RSDK_THIS(UIDialog);
    EntityUIControl *control = entity->parent;

    if (control) {
        UIControl_Unknown6(control);
        destroyEntity(control);
    }

    for (int i = 0; i < 3; ++i) {
        if (entity->entPtrs[i])
            destroyEntity(entity->entPtrs[i]);
    }

    EntityUIControl *storedControl = UIDialog->controlStore;
    if (storedControl) {
        UIControl_Unknown5(UIDialog->controlStore);
        storedControl->state          = UIDialog->controlStateStore;
        storedControl->dialogHasFocus = false;
    }
    UIDialog->controlStore      = NULL;
    UIDialog->controlStateStore = StateMachine_None;
    StateMachine_Run(entity->curCallback);
    UIDialog->activeDialog = NULL;
    destroyEntity(entity);
}

bool32 UIDialog_Unknown9(void)
{
    EntityUIDialog *entity   = UIDialog->activeDialog;
    EntityUIControl *control = entity->parent;

    for (int i = 0; i < control->unknownCount1; ++i) {
        int frame = entity->buttonFrames[i];
        if (frame == 1 || frame == 3) {
            if (entity->flags[i]) {
                if (entity->state != UIDialog_Unknown13) {
                    entity->parent->selectionDisabled = true;
                    entity->curCallback                 = entity->callbacks[i];
                    entity->field_5C                    = 0;
                    entity->state                       = UIDialog_Unknown13;
                }
            }
            else if (entity->callbacks[i]) {
                entity->callbacks[i]();
            }
            return true;
        }
    }

    return false;
}

int UIDialog_Unknown10(void)
{
    EntityUIDialog *entity = UIDialog->activeDialog;
    if (entity->parent) {
        int id = entity->parent->activeEntityID;
        if (id >= 0 && id < entity->parent->unknownCount1) {
            if (entity->flags[id]) {
                UIDialog_Unknown4(entity, entity->callbacks[id]);
            }
            else if (entity->callbacks[id]) {
                entity->callbacks[id]();
            }
        }
    }
    return 0;
}

void UIDialog_Unknown11(void)
{
    RSDK_THIS(UIDialog);
    Vector2 pos;

    pos.x = 0;
    pos.y = 0;
    if (entity->field_5C == 1) {
        RSDK.PlaySFX(UIWidgets->sfx_Woosh, false, 255);
        UIControl_Unknown12((Entity *)entity->parent);
    }

    if (entity->field_5C >= 8) {
        if (entity->field_5C >= 16) {
            if (entity->field_5C >= 26) {
                entity->parent->selectionDisabled = false;
                entity->field_5C                    = 0;
                entity->state                       = UIDialog_Unknown12;
            }
            else {
                entity->field_70.x = RSDK_screens->width << 16;
                entity->field_70.y = 0x900000;
                MathHelpers_Unknown3(&pos, max(((entity->field_5C - 16) << 8) / 10, 0), -0x400000 - (RSDK_screens->width << 16), 0, 0, 0);
                entity->drawPos.x = pos.x;
                entity->drawPos.y = pos.y;
                if (entity->field_5C - 16 == 1 && entity->field_B4)
                    RSDK.PlaySFX(UIWidgets->sfx_Event, false, 255);
                ++entity->field_5C;
            }
        }
        else {
            entity->drawPos.x = -0x400000 - (RSDK_screens->width << 16);
            entity->drawPos.y = 0;
            MathHelpers_Unknown1(&pos, max(((entity->field_5C - 8) << 8) / 8, 0), RSDK_screens->width << 16, 0x10000, RSDK_screens->width << 16,
                                 0x900000);
            entity->field_70 = pos;
            entity->field_5C++;
        }
    }
    else {
        entity->drawPos.x = -0x400000 - (RSDK_screens->width << 16);
        entity->drawPos.y = 0;
        MathHelpers_Unknown3(&pos, max((entity->field_5C << 8) / 8, 0), 0, 0x10000, RSDK_screens->width << 16, 0x10000);
        ++entity->field_5C;
        entity->field_70.x = pos.x;
        entity->field_70.y = pos.y;
    }
}

void UIDialog_Unknown12(void)
{
    RSDK_THIS(UIDialog);

    entity->field_70.x = RSDK_screens->width << 16;
    entity->field_70.y = 0x900000;
    entity->drawPos.x  = 0;
    entity->drawPos.y  = 0;
}

void UIDialog_Unknown13(void)
{
    RSDK_THIS(UIDialog);
    Vector2 pos;

    if (entity->field_5C >= 8) {
        if (entity->field_5C >= 16) {
            UIDialog_Close();
        }
        else {
            entity->drawPos.x = (RSDK_screens->width + 64) << 16;
            entity->drawPos.y = 0;
            MathHelpers_Unknown3(&pos, max(((entity->field_5C - 8) << 8) / 8, 0), RSDK_screens->width << 16, 0x900000, RSDK_screens->width << 16, 0);
            ++entity->field_5C;
            entity->field_70.x = pos.x;
            entity->field_70.y = pos.y;
        }
    }
    else {
        entity->field_70.x = RSDK_screens->width << 16;
        entity->field_70.y = 0x900000;
        MathHelpers_Unknown2(&pos, max((entity->field_5C << 8) / 8, 0), 0, 0, (RSDK_screens->width + 64) << 16, 0);
        ++entity->field_5C;
        entity->drawPos.x = pos.x;
        entity->drawPos.y = pos.y;
    }
}

void UIDialog_EditorDraw(void) {}

void UIDialog_EditorLoad(void) {}

void UIDialog_Serialize(void) {}
