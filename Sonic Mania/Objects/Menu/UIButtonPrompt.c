#include "SonicMania.h"

ObjectUIButtonPrompt *UIButtonPrompt;

void UIButtonPrompt_Update(void)
{
    RSDK_THIS(UIButtonPrompt);

    bool32 textChanged = false;
    if (entity->textSprite != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 0, &entity->animator3, true, entity->promptID);
        textChanged        = true;
        entity->textSprite = UIWidgets->textSpriteIndex;
    }
    if (entity->scale.x == 0x200 && entity->scaleMax == 0x200 && entity->scaleSpeed)
        entity->scaleSpeed = 0;

    StateMachine_Run(entity->state);

    if (entity->scale.x >= entity->scaleMax) {
        if (entity->scale.x > entity->scaleMax) {
            entity->scale.x -= entity->scaleSpeed;
            if (entity->scale.x < entity->scaleMax)
                entity->scale.x = entity->scaleMax;
        }
    }
    else {
        entity->scale.x += entity->scaleSpeed;
        if (entity->scale.x > entity->scaleMax)
            entity->scale.x = entity->scaleMax;
    }
    entity->scale.y = entity->scale.x;

    if (entity->prevPrompt != entity->promptID) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 0, &entity->animator3, true, entity->promptID);
        entity->prevPrompt = entity->promptID;
    }

    int button = entity->buttonID;
    if (entity->prevButton != button) {
        UIButtonPrompt_Unknown4();
        button             = entity->buttonID;
        entity->prevButton = button;
    }

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        int mappings = UIButtonPrompt_GetButtonMappings(UIButtonPrompt->inputID, button);
        if (textChanged || entity->mappings != mappings) {
            UIButtonPrompt_Unknown4();
            entity->mappings = mappings;
        }
    }
}

void UIButtonPrompt_LateUpdate(void)
{
    RSDK_THIS(UIButtonPrompt);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    if (control && entity->headingAnchor) {
        EntityUIHeading *heading = (EntityUIHeading *)control->heading;
        if (heading) {
            switch (entity->headingAnchor) {
                default: break;
                case 1:
                    entity->position.x = heading->position.x - 0xBC0000;
                    entity->position.y = heading->position.y - 0x80000;
                    break;
                case 2:
                    entity->position.x = 0x7C0000 + heading->position.x;
                    entity->position.y = heading->position.y - 0x80000;
                    break;
                case 3:
                    entity->position.x = 0x7C0000 + heading->position.x;
                    entity->position.y = heading->position.y + 0x100000;
                    break;
                case 4:
                    entity->position.x = (heading->position.x - 0xBC0000);
                    entity->position.y = heading->position.y + 0x100000;
                    break;
            }
        }
    }
}

void UIButtonPrompt_StaticUpdate(void)
{
    UIButtonPrompt->type = UIButtonPrompt_GetGamepadType();
#if RETRO_USE_PLUS
    int id                       = API_MostRecentActiveControllerID(0, 0, 0);
#else
    int id = API_MostRecentActiveControllerID(0);
#endif
    int type                   = API_GetControllerType(id);
    if ((type & 0xFF00) == 0x100)
        UIButtonPrompt->inputID = type & 0xFF;
    else
        UIButtonPrompt->inputID = 1;
}

void UIButtonPrompt_Draw(void)
{
    RSDK_THIS(UIButtonPrompt);
    UIButtonPrompt_Unknown4();
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->drawFX = FX_SCALE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    entity->drawFX = FX_NONE;
    if (entity->flag)
        RSDK.DrawSprite(&entity->animator3, NULL, false);
}

void UIButtonPrompt_Create(void *data)
{
    RSDK_THIS(UIButtonPrompt);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos      = entity->position;
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->scaleMax      = 0x200;
        entity->scaleSpeed    = 0x10;
        entity->scale.x       = 0x200;
        entity->scale.y       = 0x200;
        entity->field_94      = 0;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x2000000;
        entity->updateRange.y = 0x800000;
        entity->flag          = true;
        RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 0, &entity->animator3, true, entity->promptID);
        UIButtonPrompt_Unknown4();
        entity->textSprite  = UIWidgets->textSpriteIndex;
        entity->state       = UIButtonPrompt_Unknown6;
        entity->parent      = 0;
        entity->touchSize.x = 0x580000;
        entity->touchSize.y = 0x100000;
        entity->touchPos.x  = 0x200000;
    }
}

void UIButtonPrompt_StageLoad(void)
{
    UIButtonPrompt->type      = 1;
    UIButtonPrompt->inputID   = 1;
    UIButtonPrompt->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);
}

int UIButtonPrompt_GetButtonMappings(int input, int button)
{
    switch (button) {
        case 0: return RSDK_controller[input].keyA.keyMap;
        case 1: return RSDK_controller[input].keyB.keyMap;
        case 2: return RSDK_controller[input].keyX.keyMap;
        case 3: return RSDK_controller[input].keyY.keyMap;
        case 4: return RSDK_controller[input].keyStart.keyMap;
        case 5: return RSDK_controller[input].keySelect.keyMap;
        default: break;
    }
    return 0;
}

int UIButtonPrompt_GetGamepadType(void)
{
#if RETRO_USE_PLUS 
    int id = API_MostRecentActiveControllerID(0, 0, 0);
#else
    int id = API_MostRecentActiveControllerID(0);
#endif
    int gamepadType = API_GetControllerType(id);

    int deviceType = (gamepadType >> 8) & 0xFF;
    if (deviceType == DEVICE_TYPE_KEYBOARD) {
        switch (Localization->language) {
            case LANGUAGE_FR: return 9;
            case LANGUAGE_IT: return 10;
            case LANGUAGE_GE: return 11;
            case LANGUAGE_SP: return 12;
            default: return 1;
        }
    }
    else if (deviceType == DEVICE_TYPE_CONTROLLER) {
        switch (gamepadType & 0xFF) {
            case DEVICE_PS4: return 3;
            case DEVICE_SATURN: return 6;
            case DEVICE_SWITCH:
            case DEVICE_SWITCH_PRO:
            case DEVICE_SWITCH_JOY_GRIP: return 4;
            case DEVICE_SWITCH_JOY_L: return 7;
            case DEVICE_SWITCH_JOY_R: return 8;
            default: break;
        }
    }
    return 2;
}

uint8 UIButtonPrompt_MappingsToFrame(int mappings)
{
    //case values: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
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

void UIButtonPrompt_Unknown4(void)
{
    RSDK_THIS(UIButtonPrompt);
    if (RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, 2, &entity->animator2, true, entity->buttonID);
    }
    else {
        int buttonID = entity->buttonID;
        if (API_GetConfirmButtonFlip() && buttonID <= 1)
            buttonID ^= 1;

        if (UIButtonPrompt->type != 1 && (UIButtonPrompt->type <= 8 || UIButtonPrompt->type > 12)) {
            RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, UIButtonPrompt->type, &entity->animator2, true, buttonID);
        }
        else {
            int mappings = UIButtonPrompt_GetButtonMappings(UIButtonPrompt->inputID, buttonID);
            int frame    = UIButtonPrompt_MappingsToFrame(mappings);
            RSDK.SetSpriteAnimation(UIButtonPrompt->aniFrames, 1, &entity->animator2, true, frame);
        }
    }
}

bool32 UIButtonPrompt_CheckTouch(void)
{
    RSDK_THIS(UIButtonPrompt);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    if (control && !control->dialogHasFocus && !control->selectionDisabled) {
        if (RSDK_touchMouse->count) {
            int screenX = (RSDK_screens->position.x << 16);
            int screenY = (RSDK_screens->position.y << 16);
            int sizeX   = entity->touchSize.x >> 1;
            int sizeY   = entity->touchSize.y >> 1;

            bool32 flag = false;
            for (int i = 0; i < RSDK_touchMouse->count; ++i) {
                int x = screenX - ((RSDK_touchMouse->x[i] * RSDK_screens->width) * -65536.0f);
                int y = screenY - ((RSDK_touchMouse->y[i] * RSDK_screens->height) * -65536.0f);

                int touchX = abs(entity->touchPos.x + entity->position.x - x);
                int touchY = abs(entity->touchPos.y + entity->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    flag = true;
                }
            }

            entity->touched = flag;
            return true;
        }
        else {
            if (entity->touched) {
                entity->timer = 0;
                entity->state = UIButtonPrompt_Unknown7;
            }
            entity->touched = false;
        }
    }
    return false;
}

void UIButtonPrompt_Unknown6(void)
{
    RSDK_THIS(UIButtonPrompt);
    if (entity->visible) {
        if (UIButtonPrompt_CheckTouch()) {
            entity->scaleMax = 640;
            if (entity->scaleSpeed < 16)
                entity->scaleSpeed = 16;
        }
        else if (!entity->field_94) {
            entity->scaleMax = 512;
        }
    }
}

void UIButtonPrompt_Unknown7(void)
{
    RSDK_THIS(UIButtonPrompt);

    entity->scaleMax = 640;
    if (entity->timer == 16) {
        entity->timer = 0;
        entity->flag  = true;
        entity->state = UIButtonPrompt_Unknown6;
        int buttonID  = entity->buttonID;
        if (API_GetConfirmButtonFlip() && buttonID <= 1)
            buttonID ^= 1;
        UIControl_ClearInputs(buttonID);
    }
    entity->flag = !((++entity->timer >> 1) & 1);
}

void UIButtonPrompt_EditorDraw(void) {}

void UIButtonPrompt_EditorLoad(void) {}

void UIButtonPrompt_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_ENUM, promptID);
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_ENUM, buttonID);
    RSDK_EDITABLE_VAR(UIButtonPrompt, VAR_UINT8, headingAnchor);
}
