#include "SonicMania.h"

ObjectUIDialog *UIDialog;

void UIDialog_Update(void)
{
    RSDK_THIS(UIDialog);
    StateMachine_Run(entity->state);
    //UIDialog_Unknown7();

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
    Vector2 drawPos;
    //UIDialog_Unknown6();
    drawPos.y = entity->position.y;
    drawPos.x = entity->position.x + 0x100000;
    switch (entity->lineCount) {
        case 0:
        case 1:
            drawPos.x = drawPos.x + 0x100000;
            drawPos.y = (entity->position.y - 0x200000);
            drawPos.x += (drawPos.x + 0x100000);
            drawPos.y += (entity->position.y - 0x200000);
            break;
        case 2:
            drawPos.x = drawPos.x + 0x180000;
            drawPos.y = entity->position.y - 0x280000;
            drawPos.x += (drawPos.x + 0x180000);
            drawPos.y += (entity->position.y - 0x280000);
            break;
        case 3:
            drawPos.x = drawPos.x + 0x200000;
            drawPos.y = entity->position.y - 0x300000;
            drawPos.x += (drawPos.x + 0x200000);
            drawPos.y += (entity->position.y - 0x300000);
            break;
        default: break;
    }

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
        RSDK.DrawText(&entity->animator, &drawPos, &entity->textInfo, start, len, 0, 0, 0, 0, 0);
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
    UIDialog->field_8      = 0;
    UIDialog->activeDialog = 0;
}

EntityUIDialog *UIDialog_CreateActiveDialog(void *msg)
{
    if (UIDialog->activeDialog) {
        LogHelpers_Print("EXCEPTION: Called CreateDialog when an activeDialog already existed.");
    }
    else {
        int slot = RSDK_GET_ENTITY(SLOT_DIALOG, UIDialog)->objectID;
        if (slot) {
            LogHelpers_Print("Can't create UIDialog (%d), entity already exists in slot (class ID: %d)", UIDialog->objectID, slot);
        }
        else {
            RSDK.ResetEntitySlot(SLOT_DIALOG, UIDialog->objectID, msg);
            EntityUIDialog *dialog =  RSDK_GET_ENTITY(SLOT_DIALOG, UIDialog);
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

void UIDialog_Unknown2(byte a1, EntityUIDialog *dialog, int (*callback)(void), int a4)
{
    int id = dialog->id;
    if (dialog->id < 3) {
        dialog->field_88[dialog->id]  = a1;
        dialog->callbacks[dialog->id] = callback;
        dialog->field_98[dialog->id]  = a4;
        RSDK.ResetEntitySlot(dialog->id + 23, UIButton->objectID, NULL);
        EntityUIButton *button = RSDK.GetEntityByID(dialog->id + 23);
        button->position.x     = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        button->position.y     = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 9, &button->animator, true, a1);
        button->textSpriteIndex = UIWidgets->textSpriteIndex;

        if (a1 == 4)
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

void UIDialog_Unknown3(EntityUIDialog *dialog)
{
    if (dialog) {
        bool32 flag = false;
        int width   = RSDK_screens->width << 16;
        int height  = RSDK_screens->height << 16;
        foreach_all(UIControl, control)
        {
            if (control->active == ACTIVE_ALWAYS) {
                flag                    = true;
                control->dialogHasFocus = true;
                UIDialog->field_8       = control;
                UIDialog->field_C       = control->state;
                foreach_break;
            }
        }

        control = NULL;
        RSDK.ResetEntitySlot(SLOT_DIALOG_UICONTROL, UIControl->objectID, &width);
        control                      = RSDK.GetEntityByID(SLOT_DIALOG_UICONTROL);
        control->dwordCC             = 1;
        control->position.x          = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        control->position.y          = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        control->rowCount            = 1;
        control->columnCount         = dialog->id;
        control->activeEntityID      = 0;
        control->backPressCB         = UIDialog_Unknown9;
        control->userdataInitialized = true;
        dialog->parent               = control;
        if (!flag) {
            UIDialog->field_8 = 0;
            UIDialog->field_C = 0;
        }

        int i = 0;
        for (; i < 3; ++i) {
            if (!dialog->entPtrs[i])
                break;
            dialog->entPtrs[i]->parent = (Entity*)control;
            control->entities[i]       = dialog->entPtrs[i];
        }
        control->unknownCount1 = i;
        dialog->field_5C       = 0;
        //dialog->state          = UIDialog_Unknown11;
    }
}

bool32 UIDialog_Unknown9(void)
{
    EntityUIDialog *entity   = UIDialog->activeDialog;
    EntityUIControl *control = entity->parent;

    for (int i = 0; i < control->unknownCount1; ++i) {
        int val = entity->field_88[i];
        if (val == 1 || val == 3) {
            if (entity->field_98[i]) {
                //if (entity->state != UIDialog_Unknown13) {
                //    entity->parent->userdataInitialized = true;
                //    entity->curCallback                 = 1;
                //    entity->field_5C                    = 0;
                //    entity->state                       = UIDialog_Unknown13;
                //}
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
        if (id > 0) {
            if (id < entity->parent->unknownCount1) {
                if (entity->field_98[id]) {
                    //return UIDialog_Unknown4(entity, entity->callbacks[id]);
                }
                else if (entity->callbacks[id]) {
                    return entity->callbacks[id]();
                }
            }
        }
    }
    return 0;
}

void UIDialog_EditorDraw(void) {}

void UIDialog_EditorLoad(void) {}

void UIDialog_Serialize(void) {}
