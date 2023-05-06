// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIButtonPrompt Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIButtonPrompt *UIButtonPrompt;

void UIButtonPrompt_Update(void)
{
    RSDK_THIS(UIButtonPrompt);

    bool32 textChanged = false;
    if (self->textSprite != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 0, &self->promptAnimator, true, self->promptID);
        textChanged      = true;
        self->textSprite = UIWidgets->textFrames;
    }

    if (self->scale.x == 0x200 && self->scaleMax == 0x200 && self->scaleSpeed)
        self->scaleSpeed = 0;

    StateMachine_Run(self->state);

    if (self->scale.x >= self->scaleMax) {
        if (self->scale.x > self->scaleMax) {
            self->scale.x -= self->scaleSpeed;
            if (self->scale.x < self->scaleMax)
                self->scale.x = self->scaleMax;
        }
    }
    else {
        self->scale.x += self->scaleSpeed;
        if (self->scale.x > self->scaleMax)
            self->scale.x = self->scaleMax;
    }

    self->scale.y = self->scale.x;

    if (self->prevPrompt != self->promptID) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 0, &self->promptAnimator, true, self->promptID);
        self->prevPrompt = self->promptID;
    }

    int32 button = self->buttonID;
    if (self->prevButton != button) {
        UIButtonPrompt_SetButtonSprites();
        button           = self->buttonID;
        self->prevButton = button;
    }

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        int32 mappings = UIButtonPrompt_GetButtonMappings(UIButtonPrompt->inputSlot, button);
        if (textChanged || self->mappings != mappings) {
            UIButtonPrompt_SetButtonSprites();
            self->mappings = mappings;
        }
    }
}

void UIButtonPrompt_LateUpdate(void)
{
    RSDK_THIS(UIButtonPrompt);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && control->heading && self->headingAnchor) {
        EntityUIHeading *heading = (EntityUIHeading *)control->heading;

        switch (self->headingAnchor) {
            default:
            case UIBUTTONPROMPT_ANCHOR_NONE: break;

            case UIBUTTONPROMPT_ANCHOR_TOPLEFT_ROW1:
                self->position.x = heading->position.x - TO_FIXED(188);
                self->position.y = heading->position.y - TO_FIXED(8);
                break;

            case UIBUTTONPROMPT_ANCHOR_TOPRIGHT_ROW1:
                self->position.x = heading->position.x + TO_FIXED(124);
                self->position.y = heading->position.y - TO_FIXED(8);
                break;

            case UIBUTTONPROMPT_ANCHOR_TOPLEFT_ROW2:
                self->position.x = heading->position.x - TO_FIXED(188);
                self->position.y = heading->position.y + TO_FIXED(16);
                break;

            case UIBUTTONPROMPT_ANCHOR_TOPRIGHT_ROW2:
                self->position.x = heading->position.x + TO_FIXED(124);
                self->position.y = heading->position.y + TO_FIXED(16);
                break;
        }
    }
}

void UIButtonPrompt_StaticUpdate(void)
{
    UIButtonPrompt->type = UIButtonPrompt_GetGamepadType();

#if MANIA_USE_PLUS
    int32 id = API_GetFilteredInputDeviceID(false, false, 0);
#else
    int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
#endif
    int32 gamepadType = API_GetInputDeviceType(id);
    int32 deviceType  = (gamepadType >> 8) & 0xFF;

    UIButtonPrompt->inputSlot = deviceType == DEVICE_TYPE_KEYBOARD ? (gamepadType & 0xFF) : CONT_P1;
}

void UIButtonPrompt_Draw(void)
{
    RSDK_THIS(UIButtonPrompt);

    UIButtonPrompt_SetButtonSprites();

    RSDK.DrawSprite(&self->decorAnimator, NULL, false);

    self->drawFX = FX_SCALE;
    RSDK.DrawSprite(&self->buttonAnimator, NULL, false);

    self->drawFX = FX_NONE;
    if (self->textVisible)
        RSDK.DrawSprite(&self->promptAnimator, NULL, false);
}

void UIButtonPrompt_Create(void *data)
{
    RSDK_THIS(UIButtonPrompt);

    if (!SceneInfo->inEditor) {
        self->startPos      = self->position;
        self->visible       = true;
        self->drawGroup     = 2;
        self->scaleMax      = 0x200;
        self->scaleSpeed    = 0x10;
        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->disableScale  = false;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x2000000;
        self->updateRange.y = 0x800000;
        self->textVisible   = true;

        RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, 0, &self->decorAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 0, &self->promptAnimator, true, self->promptID);

        UIButtonPrompt_SetButtonSprites();

        self->textSprite  = UIWidgets->textFrames;
        self->state       = UIButtonPrompt_State_CheckIfSelected;
        self->parent      = NULL;
        self->touchSize.x = 0x580000;
        self->touchSize.y = 0x100000;
        self->touchPos.x  = 0x200000;
    }
}

void UIButtonPrompt_StageLoad(void)
{
    UIButtonPrompt->type      = UIBUTTONPROMPT_KEYBOARD;
    UIButtonPrompt->inputSlot = CONT_P1;

    UIButtonPrompt->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);
}

int32 UIButtonPrompt_GetButtonMappings(int32 input, int32 button)
{
#if MANIA_USE_PLUS
    switch (button) {
        case UIBUTTONPROMPT_BUTTON_A: return ControllerInfo[input].keyA.keyMap;
        case UIBUTTONPROMPT_BUTTON_B: return ControllerInfo[input].keyB.keyMap;
        case UIBUTTONPROMPT_BUTTON_X: return ControllerInfo[input].keyX.keyMap;
        case UIBUTTONPROMPT_BUTTON_Y: return ControllerInfo[input].keyY.keyMap;
        case UIBUTTONPROMPT_BUTTON_START: return ControllerInfo[input].keyStart.keyMap;
        case UIBUTTONPROMPT_BUTTON_SELECT: return ControllerInfo[input].keySelect.keyMap;
        default: break;
    }
#else
    switch (button) {
        case UIBUTTONPROMPT_BUTTON_A: return ControllerInfo[CONT_P1].keyA.keyMap;
        case UIBUTTONPROMPT_BUTTON_B: return ControllerInfo[CONT_P1].keyB.keyMap;
        case UIBUTTONPROMPT_BUTTON_X: return ControllerInfo[CONT_P1].keyX.keyMap;
        case UIBUTTONPROMPT_BUTTON_Y: return ControllerInfo[CONT_P1].keyY.keyMap;
        case UIBUTTONPROMPT_BUTTON_START: return ControllerInfo[CONT_P1].keyStart.keyMap;
        case UIBUTTONPROMPT_BUTTON_SELECT: return ControllerInfo[CONT_P1].keySelect.keyMap;
        default: break;
    }
#endif

    return 0;
}

int32 UIButtonPrompt_GetGamepadType(void)
{
#if MANIA_USE_PLUS
    int32 id = API_GetFilteredInputDeviceID(false, false, 0);
#else
    int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
#endif
    int32 gamepadType = API_GetInputDeviceType(id);

    int32 deviceType = (gamepadType >> 8) & 0xFF;
    if (deviceType == DEVICE_TYPE_KEYBOARD) {
        switch (Localization->language) {
            case LANGUAGE_FR: return UIBUTTONPROMPT_KEYBOARD_FR;
            case LANGUAGE_IT: return UIBUTTONPROMPT_KEYBOARD_IT;
            case LANGUAGE_GE: return UIBUTTONPROMPT_KEYBOARD_GE;
            case LANGUAGE_SP: return UIBUTTONPROMPT_KEYBOARD_SP;
            default: return UIBUTTONPROMPT_KEYBOARD;
        }
    }
    else if (deviceType == DEVICE_TYPE_CONTROLLER) {
        // I don't actually think saturn type is ever set in-engine, neat that it exists though

        switch (gamepadType & 0xFF) {
            case DEVICE_XBOX: return UIBUTTONPROMPT_XBOX;
            case DEVICE_PS4: return UIBUTTONPROMPT_PS4;
            case DEVICE_SATURN: return UIBUTTONPROMPT_SATURN_WHITE;
            case DEVICE_SWITCH_HANDHELD:
            case DEVICE_SWITCH_JOY_GRIP:
            case DEVICE_SWITCH_PRO: return UIBUTTONPROMPT_SWITCH;
            case DEVICE_SWITCH_JOY_L: return UIBUTTONPROMPT_JOYCON_L;
            case DEVICE_SWITCH_JOY_R: return UIBUTTONPROMPT_JOYCON_R;
            default: break;
        }
    }
    return UIBUTTONPROMPT_XBOX;
}

uint8 UIButtonPrompt_MappingsToFrame(int32 mappings)
{
#if GAME_VERSION != VER_100
    switch (mappings) {
        case KEYMAP_BACK: return 14;
        case KEYMAP_TAB: return 15;
        case KEYMAP_RETURN: return 41;
        case KEYMAP_SHIFT:
        case KEYMAP_LSHIFT: return 42;
        case KEYMAP_CONTROL:
        case KEYMAP_LCONTROL: return 54;
        case KEYMAP_MENU:
        case KEYMAP_LMENU: return 55;
        case KEYMAP_CAPITAL: return 29;
        case KEYMAP_SPACE: return 56;
        case KEYMAP_PRIOR: return 91;
        case KEYMAP_NEXT: return 92;
        case KEYMAP_END: return 93;
        case KEYMAP_HOME: return 94;
        case KEYMAP_LEFT: return 61;
        case KEYMAP_UP: return 59;
        case KEYMAP_RIGHT: return 62;
        case KEYMAP_DOWN: return 60;
        case KEYMAP_INSERT: return 95;
        case KEYMAP_DELETE: return 96;
        case KEYMAP_0: return 11;
        case KEYMAP_1: return 2;
        case KEYMAP_2: return 3;
        case KEYMAP_3: return 4;
        case KEYMAP_4: return 5;
        case KEYMAP_5: return 6;
        case KEYMAP_6: return 7;
        case KEYMAP_7: return 8;
        case KEYMAP_8: return 9;
        case KEYMAP_9: return 10;
        case KEYMAP_A: return 30;
        case KEYMAP_B: return 47;
        case KEYMAP_C: return 45;
        case KEYMAP_D: return 32;
        case KEYMAP_E: return 18;
        case KEYMAP_F: return 33;
        case KEYMAP_G: return 34;
        case KEYMAP_H: return 35;
        case KEYMAP_I: return 23;
        case KEYMAP_J: return 36;
        case KEYMAP_K: return 37;
        case KEYMAP_L: return 38;
        case KEYMAP_M: return 49;
        case KEYMAP_N: return 48;
        case KEYMAP_O: return 24;
        case KEYMAP_P: return 25;
        case KEYMAP_Q: return 16;
        case KEYMAP_R: return 19;
        case KEYMAP_S: return 31;
        case KEYMAP_T: return 20;
        case KEYMAP_U: return 22;
        case KEYMAP_V: return 46;
        case KEYMAP_W: return 17;
        case KEYMAP_X: return 44;
        case KEYMAP_Y: return 21;
        case KEYMAP_Z: return 43;
        case KEYMAP_NUMPAD0: return 77;
        case KEYMAP_NUMPAD1: return 73;
        case KEYMAP_NUMPAD2: return 74;
        case KEYMAP_NUMPAD3: return 75;
        case KEYMAP_NUMPAD4: return 70;
        case KEYMAP_NUMPAD5: return 71;
        case KEYMAP_NUMPAD6: return 72;
        case KEYMAP_NUMPAD7: return 66;
        case KEYMAP_NUMPAD8: return 67;
        case KEYMAP_NUMPAD9: return 68;
        case KEYMAP_MULTIPLY: return 64;
        case KEYMAP_ADD: return 69;
        case KEYMAP_SUBTRACT: return 65;
        case KEYMAP_DECIMAL: return 78;
        case KEYMAP_DIVIDE: return 63;
        case KEYMAP_RSHIFT: return 53;
        case KEYMAP_RCONTROL: return 58;
        case KEYMAP_RMENU: return 57;
        case KEYMAP_OEM_1: return 79;
        case KEYMAP_OEM_PLUS: return 87;
        case KEYMAP_OEM_COMMA: return 88;
        case KEYMAP_OEM_MINUS: return 89;
        case KEYMAP_OEM_PERIOD: return 90;
        case KEYMAP_OEM_2: return 80;
        case KEYMAP_OEM_3: return 81;
        case KEYMAP_OEM_4: return 82;
        case KEYMAP_OEM_5: return 83;
        case KEYMAP_OEM_6: return 84;
        case KEYMAP_OEM_7: return 85;
        case KEYMAP_OEM_8: return 86;
        default: return 0;
    }
#else
    switch (mappings) {
        case KEYMAP_BACK: return 14;
        case KEYMAP_TAB: return 15;
        case KEYMAP_RETURN: return 41;
        case KEYMAP_SHIFT:
        case KEYMAP_LSHIFT: return 42;
        case KEYMAP_CONTROL:
        case KEYMAP_LCONTROL: return 54;
        case KEYMAP_MENU: return 55;
        case KEYMAP_CAPITAL: return 29;
        case KEYMAP_SPACE: return 56;
        case KEYMAP_LEFT: return 61;
        case KEYMAP_UP: return 59;
        case KEYMAP_RIGHT: return 62;
        case KEYMAP_DOWN: return 60;
        case KEYMAP_0: return 11;
        case KEYMAP_1: return 2;
        case KEYMAP_2: return 3;
        case KEYMAP_3: return 4;
        case KEYMAP_4: return 5;
        case KEYMAP_5: return 6;
        case KEYMAP_6: return 7;
        case KEYMAP_7: return 8;
        case KEYMAP_8: return 9;
        case KEYMAP_9: return 10;
        case KEYMAP_A: return 30;
        case KEYMAP_B: return 47;
        case KEYMAP_C: return 45;
        case KEYMAP_D: return 32;
        case KEYMAP_E: return 18;
        case KEYMAP_F: return 33;
        case KEYMAP_G: return 34;
        case KEYMAP_H: return 35;
        case KEYMAP_I: return 23;
        case KEYMAP_J: return 36;
        case KEYMAP_K: return 37;
        case KEYMAP_L: return 38;
        case KEYMAP_M: return 49;
        case KEYMAP_N: return 48;
        case KEYMAP_O: return 24;
        case KEYMAP_P: return 25;
        case KEYMAP_Q: return 16;
        case KEYMAP_R: return 19;
        case KEYMAP_S: return 31;
        case KEYMAP_T: return 20;
        case KEYMAP_U: return 22;
        case KEYMAP_V: return 46;
        case KEYMAP_W: return 17;
        case KEYMAP_X: return 44;
        case KEYMAP_Y: return 21;
        case KEYMAP_Z: return 43;
        case KEYMAP_NUMPAD0: return 77;
        case KEYMAP_NUMPAD1: return 73;
        case KEYMAP_NUMPAD2: return 74;
        case KEYMAP_NUMPAD3: return 75;
        case KEYMAP_NUMPAD4: return 70;
        case KEYMAP_NUMPAD5: return 71;
        case KEYMAP_NUMPAD6: return 72;
        case KEYMAP_NUMPAD7: return 66;
        case KEYMAP_NUMPAD8: return 67;
        case KEYMAP_NUMPAD9: return 68;
        case KEYMAP_MULTIPLY: return 64;
        case KEYMAP_ADD: return 69;
        case KEYMAP_SUBTRACT: return 65;
        case KEYMAP_DECIMAL: return 78;
        case KEYMAP_DIVIDE: return 63;
        case KEYMAP_RSHIFT: return 53;
        case KEYMAP_RCONTROL: return 58;
        default: return 0;
    }
#endif
    return 0;
}

void UIButtonPrompt_SetButtonSprites(void)
{
    RSDK_THIS(UIButtonPrompt);

    if (SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIBUTTONPROMPT_XBOX, &self->buttonAnimator, true, self->buttonID);
    }
    else {
        int32 buttonID = self->buttonID;
        if (API_GetConfirmButtonFlip() && buttonID <= 1)
            buttonID ^= 1;

#if MANIA_USE_PLUS
        if (UIButtonPrompt->type != UIBUTTONPROMPT_KEYBOARD
            && (UIButtonPrompt->type < UIBUTTONPROMPT_KEYBOARD_FR || UIButtonPrompt->type > UIBUTTONPROMPT_KEYBOARD_SP)) {
            RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIButtonPrompt->type, &self->buttonAnimator, true, buttonID);
        }
        else {
            // despite different languages existing here, the english one is always the one used
            int32 mappings = UIButtonPrompt_GetButtonMappings(UIButtonPrompt->inputSlot, buttonID);
            int32 frame    = UIButtonPrompt_MappingsToFrame(mappings);
            RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIBUTTONPROMPT_KEYBOARD, &self->buttonAnimator, true, frame);
        }
#else

        if (sku_platform != PLATFORM_PC && sku_platform != PLATFORM_DEV) {
            RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIButtonPrompt->type, &self->buttonAnimator, true, buttonID);
        }
        else {
            int32 mappings = UIButtonPrompt_GetButtonMappings(UIButtonPrompt->inputSlot, buttonID);
            if (UIButtonPrompt->type == UIBUTTONPROMPT_XBOX) {
                RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIBUTTONPROMPT_XBOX, &self->buttonAnimator, true, buttonID);
            }
            else {
                int32 frame = UIButtonPrompt_MappingsToFrame(mappings);
                RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIBUTTONPROMPT_KEYBOARD, &self->buttonAnimator, true, frame);
            }
        }
#endif
    }
}

bool32 UIButtonPrompt_CheckTouch(void)
{
    RSDK_THIS(UIButtonPrompt);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && !control->dialogHasFocus && !control->selectionDisabled) {
        if (TouchInfo->count) {
            int32 screenX = (ScreenInfo->position.x << 16);
            int32 screenY = (ScreenInfo->position.y << 16);
            int32 sizeX   = self->touchSize.x >> 1;
            int32 sizeY   = self->touchSize.y >> 1;

            bool32 wasTouched = false;
            for (int32 i = 0; i < TouchInfo->count; ++i) {
                int32 x = screenX - ((TouchInfo->x[i] * ScreenInfo->size.x) * -65536.0f);
                int32 y = screenY - ((TouchInfo->y[i] * ScreenInfo->size.y) * -65536.0f);

                int32 touchX = abs(self->touchPos.x + self->position.x - x);
                int32 touchY = abs(self->touchPos.y + self->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    wasTouched = true;
                }
            }

            self->touched = wasTouched;
        }
        else {
            if (self->touched) {
                self->timer = 0;
                self->state = UIButtonPrompt_State_Selected;
            }
            self->touched = false;
        }
    }

    return self->touched;
}

void UIButtonPrompt_State_CheckIfSelected(void)
{
    RSDK_THIS(UIButtonPrompt);

    if (self->visible) {
        if (UIButtonPrompt_CheckTouch()) {
            self->scaleMax = 0x280;

            if (self->scaleSpeed < 0x10)
                self->scaleSpeed = 0x10;
        }
        else if (!self->disableScale) {
            self->scaleMax = 0x200;
        }
    }
}

void UIButtonPrompt_State_Selected(void)
{
    RSDK_THIS(UIButtonPrompt);

    self->scaleMax = 0x280;

    if (++self->timer == 16) {
        self->timer       = 0;
        self->textVisible = true;
        self->state       = UIButtonPrompt_State_CheckIfSelected;

        int32 buttonID = self->buttonID;
        if (API_GetConfirmButtonFlip() && buttonID <= 1)
            buttonID ^= 1;

        UIControl_ClearInputs(buttonID);
    }

    self->textVisible = !((self->timer >> 1) & 1);
}

#if GAME_INCLUDE_EDITOR
void UIButtonPrompt_EditorDraw(void)
{
    RSDK_THIS(UIButtonPrompt);

    self->startPos      = self->position;
    self->drawGroup     = 2;
    self->disableScale  = false;
    self->updateRange.x = 0x2000000;
    self->updateRange.y = 0x800000;
    self->textVisible   = true;
    RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, 0, &self->decorAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 0, &self->promptAnimator, true, self->promptID);

    UIButtonPrompt_SetButtonSprites();
    self->textSprite = UIWidgets->textFrames;

    UIButtonPrompt_Draw();
}

void UIButtonPrompt_EditorLoad(void)
{
    UIButtonPrompt->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIButtonPrompt, buttonID);
    RSDK_ENUM_VAR("A", UIBUTTONPROMPT_BUTTON_A);
    RSDK_ENUM_VAR("B", UIBUTTONPROMPT_BUTTON_B);
    RSDK_ENUM_VAR("X", UIBUTTONPROMPT_BUTTON_X);
    RSDK_ENUM_VAR("Y", UIBUTTONPROMPT_BUTTON_Y);
    RSDK_ENUM_VAR("Start", UIBUTTONPROMPT_BUTTON_START);
    RSDK_ENUM_VAR("Select", UIBUTTONPROMPT_BUTTON_SELECT);

    RSDK_ACTIVE_VAR(UIButtonPrompt, headingAnchor);
    RSDK_ENUM_VAR("None", UIBUTTONPROMPT_ANCHOR_NONE);
    RSDK_ENUM_VAR("Top-Left (Row 1)", UIBUTTONPROMPT_ANCHOR_TOPLEFT_ROW1);
    RSDK_ENUM_VAR("Top-Right (Row 1)", UIBUTTONPROMPT_ANCHOR_TOPRIGHT_ROW1);
    RSDK_ENUM_VAR("Top-Left (Row 2)", UIBUTTONPROMPT_ANCHOR_TOPLEFT_ROW2);
    RSDK_ENUM_VAR("Top-Right (Row 2)", UIBUTTONPROMPT_ANCHOR_TOPRIGHT_ROW2);
}
#endif

void UIButtonPrompt_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_ENUM, promptID);
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_ENUM, buttonID);
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_UINT8, headingAnchor);
}
