#include "SonicMania.h"

ObjectUIKeyBinder *UIKeyBinder;

void UIKeyBinder_Update(void)
{
    RSDK_THIS(UIKeyBinder);
    self->touchPosStart.x = self->field_110;
    self->touchPosEnd.x   = 0;
    self->touchPosEnd.y   = 0;
    self->touchPosStart.x += 3 * self->field_114;
    self->touchPosStart.y = self->field_114 + 0x60000;

    if (self->textFrames != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator1, true, self->frameID);
        self->textFrames = UIWidgets->textFrames;
    }

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    int32 input               = self->inputID + 1;
    int32 mappings            = UIKeyBinder_GetMappings(input, self->type);

    TextInfo info;
    INIT_TEXTINFO(info);
    bool32 flag = true;

    int32 frameID = -1;
    if (self->field_16C == mappings) {
        flag = false;
    }
    else if (mappings == -1) {
        RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->animator2, true, 0);
        self->field_16C = -1;
        flag              = false;
    }
    else {
        frameID = UIButtonPrompt_MappingsToFrame(mappings);
    }

    for (int32 b = 0; b <= 8 && flag; ++b) {
        for (int32 i = 1; i <= 2 && flag; ++i) {
            if ((b != self->type || i != input) && mappings) {
                if (UIKeyBinder_GetMappings(i, b) != mappings)
                    continue;
                if (self->state != UIKeyBinder_Unknown14)
                    break;

                UIKeyBinder->curInputID  = i;
                UIKeyBinder->curMappings = b;

                int32 str = -1;
                if (i == input) {
                    str = STR_KEYALREADYBOUND;
                }
                else if (i == 1) {
                    str = STR_KEYALREADYBOUNDP1;
                }
                else if (i == 2) {
                    str = STR_KEYALREADYBOUNDP2;
                }
                if (str != -1)
                    Localization_GetString(&info, str);

                UIKeyBinder_SetMappings(self->type, input, 0);
                self->field_16C      = 0;
                EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
                if (dialog) {
                    UIDialog_AddButton(DIALOG_NO, dialog, UIKeyBinder_Unknown15, true);
                    UIDialog_AddButton(DIALOG_YES, dialog, UIKeyBinder_Unknown16, true);
                    UIDialog_Setup(dialog);
                }
                flag = false;
            }
        }
    }

    if (flag) {
        if (frameID || self->state != UIKeyBinder_Unknown14) {
            RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->animator2, true, frameID);
            self->field_16C = mappings;
            if (self->state == UIKeyBinder_Unknown14) {
                LogHelpers_Print("bind = %d 0x%02x", mappings, mappings);
                UIKeyBinder->flag         = false;
                parent->selectionDisabled = false;
                self->processButtonCB   = UIButton_ProcessButtonCB_Alt;
                self->state             = UIKeyBinder_Unknown13;
                UIKeyBinder->activeBinder = 0;
                parent->childHasFocus     = false;
                RSDK.SetSettingsValue(SETTINGS_CHANGED, 1);
                RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
            }
        }
        else {
            LogHelpers_Print("bind = %d 0x%02x", mappings, mappings);
            int32 frame = UIButtonPrompt_MappingsToFrame(self->field_16C);
            RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->animator2, true, frame);
            UIKeyBinder_SetMappings(input, self->type, -1);
            RSDK.PlaySfx(UIKeyBinder->sfxFail, false, 255);
        }
    }

    StateMachine_Run(self->state);

    int32 id = -1;
    for (int32 i = 0; i < parent->buttonCount; ++i) {
        if (self == (EntityUIKeyBinder *)parent->buttons[i]) {
            id = i;
            break;
        }
    }

    if (self->state == UIKeyBinder_Unknown13 && (parent->state != UIControl_ProcessInputs || parent->buttonID != id)) {
        self->flag  = false;
        self->state = UIKeyBinder_Unknown12;
    }
}

void UIKeyBinder_LateUpdate(void) {}

void UIKeyBinder_StaticUpdate(void) {}

void UIKeyBinder_Draw(void) { UIKeyBinder_DrawSprites(); }

void UIKeyBinder_Create(void *data)
{
    RSDK_THIS(UIKeyBinder);
    self->visible            = true;
    self->drawOrder          = 2;
    self->active             = ACTIVE_BOUNDS;
    self->updateRange.x      = 0x800000;
    self->updateRange.y      = 0x400000;
    self->field_110          = 0x400000;
    self->field_118          = 12;
    self->field_114          = 0xC0000;
    self->processButtonCB    = UIButton_ProcessButtonCB_Alt;
    self->touchCB            = UIButton_ProcessTouchCB;
    self->actionCB           = UIKeyBinder_ActionCB;
    self->selectedCB         = UIKeyBinder_SelectedCB;
    self->failCB             = StateMachine_None;
    self->buttonEnterCB      = UIKeyBinder_ButtonEnterCB;
    self->buttonLeaveCB      = UIKeyBinder_ButtonLeaveCB;
    self->checkButtonEnterCB = UIKeyBinder_CheckButtonEnterCB;
    self->checkSelectedCB    = UIKeyBinder_CheckSelectedCB;
    self->textFlag           = true;
    self->field_12C          = 512;
    self->listID             = 0;
    self->frameID         = UIKeyBinder_Unknown4(self->type);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->animator1, true, self->frameID);
    self->textFrames = UIWidgets->textFrames;
    if (!SceneInfo->inEditor) {
        int32 mappings = UIKeyBinder_GetMappings(self->inputID + 1, self->type);
        int32 frame    = UIButtonPrompt_MappingsToFrame(mappings);
        RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->animator2, true, frame);
    }
}

void UIKeyBinder_StageLoad(void)
{
    UIKeyBinder->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);
    UIKeyBinder->sfxFail   = RSDK.GetSfx("Stage/Fail.wav");
}

int32 UIKeyBinder_GetButtonListID(void)
{
    switch (Localization->language) {
        case LANGUAGE_FR: return 9; break;
        case LANGUAGE_IT: return 10; break;
        case LANGUAGE_GE: return 11; break;
        case LANGUAGE_SP: return 12; break;
        default: break;
    }
    return 1;
}

int32 UIKeyBinder_GetMappings(int32 input, int32 button)
{
    switch (button) {
        case 0: return ControllerInfo[input].keyUp.keyMap; break;
        case 1: return ControllerInfo[input].keyDown.keyMap; break;
        case 2: return ControllerInfo[input].keyLeft.keyMap; break;
        case 3: return ControllerInfo[input].keyRight.keyMap; break;
        case 4: return ControllerInfo[input].keyA.keyMap; break;
        case 5: return ControllerInfo[input].keyB.keyMap; break;
        case 6: return ControllerInfo[input].keyX.keyMap; break;
        case 7: return ControllerInfo[input].keyY.keyMap; break;
        case 8: return ControllerInfo[input].keyStart.keyMap; break;
        default: break;
    }
    return 0;
}

void UIKeyBinder_SetMappings(int32 input, int32 button, int32 mappings)
{
    switch (button) {
        case 0: ControllerInfo[input].keyUp.keyMap = mappings; break;
        case 1: ControllerInfo[input].keyDown.keyMap = mappings; break;
        case 2: ControllerInfo[input].keyLeft.keyMap = mappings; break;
        case 3: ControllerInfo[input].keyRight.keyMap = mappings; break;
        case 4: ControllerInfo[input].keyA.keyMap = mappings; break;
        case 5: ControllerInfo[input].keyB.keyMap = mappings; break;
        case 6: ControllerInfo[input].keyX.keyMap = mappings; break;
        case 7: ControllerInfo[input].keyY.keyMap = mappings; break;
        case 8: ControllerInfo[input].keyStart.keyMap = mappings; break;
        default: break;
    }
}

int32 UIKeyBinder_Unknown4(int32 id)
{
    switch (id) {
        case 0: return 7;
        case 1: return 8;
        case 2: return 9;
        case 3: return 10;
        case 4: return 13;
        case 5: return 1;
        case 6: return 3;
        case 7: return 11;
        case 8: return 12;
        default: break;
    }
    return 0;
}

void UIKeyBinder_DrawSprites(void)
{
    RSDK_THIS(UIKeyBinder);
    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->field_120;
    drawPos.y -= self->field_120;
    UIWidgets_DrawParallelogram(self->field_114 >> 16, self->field_110 >> 16, self->field_118, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);

    drawPos.x += self->field_120;
    drawPos.y += self->field_120;
    drawPos.x += self->field_120;
    drawPos.y += self->field_120;
    UIWidgets_DrawParallelogram(self->field_114 >> 16, self->field_110 >> 16, self->field_118, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->field_120;
    drawPos.y += self->field_120;
    drawPos.y += self->field_11C;
    drawPos.x += 0xB0000 - (self->field_110 >> 1);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    if (self->textFlag) {
        drawPos.x += 0x60000;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
    }
}

void UIKeyBinder_ActionCB(void) {}

bool32 UIKeyBinder_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIKeyBinder);
    return self->state == UIKeyBinder_Unknown13;
}

bool32 UIKeyBinder_CheckSelectedCB(void)
{
    RSDK_THIS(UIKeyBinder);
    return self->state == UIKeyBinder_Unknown14;
}

void UIKeyBinder_ButtonEnterCB(void)
{
    RSDK_THIS(UIKeyBinder);
    if (self->state != UIKeyBinder_Unknown13) {
        self->field_11C = 0;
        self->field_120 = 0;
        self->field_124 = -0x20000;
        self->field_128 = -0x20000;
        self->state     = UIKeyBinder_Unknown13;
    }
}

void UIKeyBinder_ButtonLeaveCB(void)
{
    RSDK_THIS(UIKeyBinder);
    self->state = UIKeyBinder_Unknown12;
}

void UIKeyBinder_SelectedCB(void)
{
    RSDK_THIS(UIKeyBinder);
    if (!UIKeyBinder->flag) {
        UIKeyBinder->flag         = true;
        EntityUIControl *parent   = (EntityUIControl *)self->parent;
        parent->childHasFocus     = true;
        parent->selectionDisabled = true;
        UIKeyBinder->activeBinder = (Entity *)self;
        self->state             = UIKeyBinder_Unknown14;
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        UIKeyBinder_SetMappings(self->inputID + 1, self->type, -1);
    }
}

void UIKeyBinder_Unknown12(void)
{
    RSDK_THIS(UIKeyBinder);

    self->textFlag = true;
    if (self->field_11C) {
        int32 dist = -(self->field_11C / abs(self->field_11C));
        self->field_11C += dist << 15;
        if (dist < 0) {
            if (self->field_11C < 0) {
                self->field_11C = 0;
            }
            else if (dist > 0 && self->field_11C > 0)
                self->field_11C = 0;
        }
        else if (dist > 0 && self->field_11C > 0)
            self->field_11C = 0;
    }

    if (self->field_120) {
        int32 dist = -(self->field_120 / abs(self->field_120));
        self->field_120 += dist << 16;
        if (dist < 0) {
            if (self->field_120 < 0) {
                self->field_120 = 0;
            }
            else if (dist > 0 && self->field_120 > 0)
                self->field_120 = 0;
        }
        else if (dist > 0 && self->field_120 > 0)
            self->field_120 = 0;
    }
}

void UIKeyBinder_Unknown13(void)
{
    RSDK_THIS(UIKeyBinder);

    self->field_124 += 0x4000;
    self->field_11C += self->field_124;
    self->textFlag = true;
    if (self->field_11C >= 0 && self->field_124 > 0) {
        self->field_11C = 0;
        self->field_124 = 0;
    }

    self->field_128 += 0x4800;
    self->field_120 += self->field_128;
    if (self->field_120 >= -0x20000 && self->field_128 > 0) {
        self->field_120 = -0x20000;
        self->field_128 = 0;
    }
}

void UIKeyBinder_Unknown14(void)
{
    RSDK_THIS(UIKeyBinder);
    UIKeyBinder_Unknown13();

    self->processButtonCB = UIKeyBinder_ActionCB;
    self->textFlag        = !((self->timer >> 1) & 1);
    self->timer++;
}

void UIKeyBinder_Unknown15(void)
{
    EntityUIKeyBinder *binder = (EntityUIKeyBinder *)UIKeyBinder->activeBinder;
    if (binder->state == UIKeyBinder_Unknown14) {
        UIKeyBinder_SetMappings(binder->inputID + 1, binder->type, -1);
        UIKeyBinder->curInputID  = -1;
        UIKeyBinder->curMappings = -1;
    }
}

void UIKeyBinder_Unknown16(void)
{
    EntityUIKeyBinder *binder = (EntityUIKeyBinder *)UIKeyBinder->activeBinder;
    if (binder->state == UIKeyBinder_Unknown14) {
        int32 mappings = UIKeyBinder_GetMappings(UIKeyBinder->curInputID, UIKeyBinder->curMappings);
        // TODO: what is v3??
        // UIKeyBinder_SetMappings(UIKeyBinder->curInputID, v3, 0);
        UIKeyBinder_SetMappings(binder->inputID + 1, binder->type, mappings);

        EntityUIControl *parent   = (EntityUIControl *)binder->parent;
        UIKeyBinder->flag         = false;
        parent->selectionDisabled = false;
        binder->processButtonCB   = UIButton_ProcessButtonCB_Alt;
        binder->state             = UIKeyBinder_Unknown13;
        UIKeyBinder->activeBinder = NULL;
        parent->childHasFocus     = false;
        RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
        UIKeyBinder->curInputID  = -1;
        UIKeyBinder->curMappings = -1;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIKeyBinder_EditorDraw(void)
{
    RSDK_THIS(UIKeyBinder);
    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;
    UIKeyBinder_DrawSprites();
}

void UIKeyBinder_EditorLoad(void) { UIKeyBinder->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE); }
#endif

void UIKeyBinder_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_UINT8, inputID);
}
