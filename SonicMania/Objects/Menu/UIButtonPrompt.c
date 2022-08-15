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
    if (control && self->headingAnchor) {

        EntityUIHeading *heading = (EntityUIHeading *)control->heading;
        if (heading) {
            switch (self->headingAnchor) {
                default:
                case UIBUTTONPROMPT_ANCHOR_NONE: break;

                case UIBUTTONPROMPT_ANCHOR_TOPLEFT:
                    self->position.x = heading->position.x - 0xBC0000;
                    self->position.y = heading->position.y - 0x80000;
                    break;

                case UIBUTTONPROMPT_ANCHOR_TOPRIGHT:
                    self->position.x = heading->position.x + 0x7C0000;
                    self->position.y = heading->position.y - 0x80000;
                    break;

                case UIBUTTONPROMPT_ANCHOR_BOTTOMRIGHT:
                    self->position.x = heading->position.x + 0x7C0000;
                    self->position.y = heading->position.y + 0x100000;
                    break;

                case UIBUTTONPROMPT_ANCHOR_BOTTOMLEFT:
                    self->position.x = heading->position.x - 0xBC0000;
                    self->position.y = heading->position.y + 0x100000;
                    break;
            }
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
    switch (button) {
        case UIBUTTONPROMPT_BUTTON_A: return ControllerInfo[input].keyA.keyMap;
        case UIBUTTONPROMPT_BUTTON_B: return ControllerInfo[input].keyB.keyMap;
        case UIBUTTONPROMPT_BUTTON_X: return ControllerInfo[input].keyX.keyMap;
        case UIBUTTONPROMPT_BUTTON_Y: return ControllerInfo[input].keyY.keyMap;
        case UIBUTTONPROMPT_BUTTON_START: return ControllerInfo[input].keyStart.keyMap;
        case UIBUTTONPROMPT_BUTTON_SELECT: return ControllerInfo[input].keySelect.keyMap;
        default: break;
    }

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
    // case values: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    switch (mappings) {
        case 8: return 14;
        case 9: return 15;
        case 13: return 41;
        case 16:
        case 160: return 42;
        case 17:
        case 162: return 54;
        case 18:
        case 164: return 55;
        case 20: return 29;
        case 32: return 56;
        case 33: return 91;
        case 34: return 92;
        case 35: return 93;
        case 36: return 94;
        case 37: return 61;
        case 38: return 59;
        case 39: return 62;
        case 40: return 60;
        case 45: return 95;
        case 46: return 96;
        case 48: return 11;
        case 49: return 2;
        case 50: return 3;
        case 51: return 4;
        case 52: return 5;
        case 53: return 6;
        case 54: return 7;
        case 55: return 8;
        case 56: return 9;
        case 57: return 10;
        case 65: return 30;
        case 66: return 47;
        case 67: return 45;
        case 68: return 32;
        case 69: return 18;
        case 70: return 33;
        case 71: return 34;
        case 72: return 35;
        case 73: return 23;
        case 74: return 36;
        case 75: return 37;
        case 76: return 38;
        case 77: return 49;
        case 78: return 48;
        case 79: return 24;
        case 80: return 25;
        case 81: return 16;
        case 82: return 19;
        case 83: return 31;
        case 84: return 20;
        case 85: return 22;
        case 86: return 46;
        case 87: return 17;
        case 88: return 44;
        case 89: return 21;
        case 90: return 43;
        case 96: return 77;
        case 97: return 73;
        case 98: return 74;
        case 99: return 75;
        case 100: return 70;
        case 101: return 71;
        case 102: return 72;
        case 103: return 66;
        case 104: return 67;
        case 105: return 68;
        case 106: return 64;
        case 107: return 69;
        case 109: return 65;
        case 110: return 78;
        case 111: return 63;
        case 161: return 53;
        case 163: return 58;
        case 165: return 57;
        case 186: return 79;
        case 187: return 87;
        case 188: return 88;
        case 189: return 89;
        case 190: return 90;
        case 191: return 80;
        case 192: return 81;
        case 219: return 82;
        case 220: return 83;
        case 221: return 84;
        case 222: return 85;
        case 223: return 86;
        default: break;
    }
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
            return true;
        }
        else {
            if (self->touched) {
                self->timer = 0;
                self->state = UIButtonPrompt_State_Selected;
            }
            self->touched = false;
        }
    }

    return false;
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

#if RETRO_INCLUDE_EDITOR
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
    RSDK_ENUM_VAR("Top-Left", UIBUTTONPROMPT_ANCHOR_TOPLEFT);
    RSDK_ENUM_VAR("Top-Right", UIBUTTONPROMPT_ANCHOR_TOPRIGHT);
    RSDK_ENUM_VAR("Bottom-Right", UIBUTTONPROMPT_ANCHOR_BOTTOMRIGHT);
    RSDK_ENUM_VAR("Bottom-Left", UIBUTTONPROMPT_ANCHOR_BOTTOMLEFT);
}
#endif

void UIButtonPrompt_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_ENUM, promptID);
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_ENUM, buttonID);
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_UINT8, headingAnchor);
}
