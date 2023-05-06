// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIKeyBinder Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIKeyBinder *UIKeyBinder;

void UIKeyBinder_Update(void)
{
    RSDK_THIS(UIKeyBinder);

    self->touchPosSizeS.x   = self->size.x;
    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = 0;
    self->touchPosSizeS.x += 3 * self->size.y;
    self->touchPosSizeS.y = self->size.y + 0x60000;

    if (self->textFrames != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->labelAnimator, true, self->frameID);
        self->textFrames = UIWidgets->textFrames;
    }

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    int32 inputID           = self->inputID + 1;
    int32 keyMap            = UIKeyBinder_GetMappings(inputID, self->type);

    String string;
    INIT_STRING(string);
    bool32 keyMapChanged = true;

    int32 frameID = -1;
    if (self->lasyKeyMap == keyMap) {
        keyMapChanged = false;
    }
    else if (keyMap == KEYMAP_AUTO_MAPPING) {
        RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->keyAnimator, true, 0);
        self->lasyKeyMap = KEYMAP_AUTO_MAPPING;
        keyMapChanged    = false;
    }
    else {
        frameID = UIButtonPrompt_MappingsToFrame(keyMap);
    }

    for (int32 buttonID = 0; buttonID <= 8 && keyMapChanged; ++buttonID) {
        for (int32 inputSlot = 1; inputSlot <= 2 && keyMapChanged; ++inputSlot) {
            if ((buttonID != self->type || inputSlot != inputID) && keyMap) {
                if (UIKeyBinder_GetMappings(inputSlot, buttonID) != keyMap)
                    continue;

                if (self->state == UIKeyBinder_State_Selected) {
#if GAME_VERSION != VER_100
                    // Handle Key clashes
                    UIKeyBinder->activeInputID  = inputSlot;
                    UIKeyBinder->activeButtonID = buttonID;

                    int32 str = -1;
                    if (inputSlot == inputID)
                        str = STR_KEYALREADYBOUND;
                    else if (inputSlot == CONT_P1)
                        str = STR_KEYALREADYBOUNDP1;
                    else if (inputSlot == CONT_P2)
                        str = STR_KEYALREADYBOUNDP2;

                    if (str != -1)
                        Localization_GetString(&string, str);

                    UIKeyBinder_SetMappings(self->type, inputID, KEYMAP_NO_MAPPING);
                    self->lasyKeyMap = KEYMAP_NO_MAPPING;
                    UIDialog_CreateDialogYesNo(&string, UIKeyBinder_MoveKeyToActionCB_Yes, UIKeyBinder_MoveKeyToActionCB_No, true, true);
#else
                    // key clash!! just fail!!
                    int32 frame = UIButtonPrompt_MappingsToFrame(self->lasyKeyMap);
                    RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->keyAnimator, true, frame);
                    UIKeyBinder_SetMappings(inputID, self->type, KEYMAP_AUTO_MAPPING);

                    RSDK.PlaySfx(UIKeyBinder->sfxFail, false, 255);
#endif
                    keyMapChanged = false;
                }
            }
        }
    }

    if (keyMapChanged) {
        if (frameID || self->state != UIKeyBinder_State_Selected) {
            RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->keyAnimator, true, frameID);
            self->lasyKeyMap = keyMap;

            if (self->state == UIKeyBinder_State_Selected) {
#if GAME_VERSION != VER_100
                LogHelpers_Print("bind = %d 0x%02x", keyMap, keyMap);
                UIKeyBinder->isSelected   = false;
#endif

                parent->selectionDisabled = false;
                self->processButtonCB     = UIButton_ProcessButtonCB_Scroll;
                self->state               = UIKeyBinder_State_HandleButtonEnter;

                parent->childHasFocus     = false;

#if GAME_VERSION != VER_100
                UIKeyBinder->activeBinder = NULL;
                RSDK.SetVideoSetting(VIDEOSETTING_CHANGED, true);
#endif
                RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
            }
        }
        else {
#if GAME_VERSION != VER_100
            LogHelpers_Print("bind = %d 0x%02x", keyMap, keyMap);
#endif

            int32 frame = UIButtonPrompt_MappingsToFrame(self->lasyKeyMap);
            RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->keyAnimator, true, frame);
            UIKeyBinder_SetMappings(inputID, self->type, KEYMAP_AUTO_MAPPING);

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

    if (self->state == UIKeyBinder_State_HandleButtonEnter && (parent->state != UIControl_ProcessInputs || parent->buttonID != id)) {
        self->isSelected = false;
        self->state      = UIKeyBinder_State_HandleButtonLeave;
    }
}

void UIKeyBinder_LateUpdate(void) {}

void UIKeyBinder_StaticUpdate(void) {}

void UIKeyBinder_Draw(void) { UIKeyBinder_DrawSprites(); }

void UIKeyBinder_Create(void *data)
{
    RSDK_THIS(UIKeyBinder);

    self->visible       = true;
    self->drawGroup     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->size.x        = 0x400000;
    self->size.y        = 0xC0000;
    self->bgEdgeSize    = 12;

    self->processButtonCB    = UIButton_ProcessButtonCB_Scroll;
    self->touchCB            = UIButton_ProcessTouchCB_Single;
    self->actionCB           = UIKeyBinder_ActionCB;
    self->selectedCB         = UIKeyBinder_SelectedCB;
    self->failCB             = StateMachine_None;
    self->buttonEnterCB      = UIKeyBinder_ButtonEnterCB;
    self->buttonLeaveCB      = UIKeyBinder_ButtonLeaveCB;
    self->checkButtonEnterCB = UIKeyBinder_CheckButtonEnterCB;
    self->checkSelectedCB    = UIKeyBinder_CheckSelectedCB;
    self->textVisible        = true;

    self->unused1 = 512;
    self->listID  = 0; // this prolly could've been a constant, I don't think its ever set to anything but 0
    self->frameID = UIKeyBinder_GetKeyNameFrameID(self->type);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->labelAnimator, true, self->frameID);
    self->textFrames = UIWidgets->textFrames;

    if (!SceneInfo->inEditor) {
        int32 mappings = UIKeyBinder_GetMappings(self->inputID + 1, self->type);
        int32 frame    = UIButtonPrompt_MappingsToFrame(mappings);
        RSDK.SetSpriteAnimation(UIKeyBinder->aniFrames, UIKeyBinder_GetButtonListID(), &self->keyAnimator, true, frame);
    }
}

void UIKeyBinder_StageLoad(void)
{
    UIKeyBinder->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);

    UIKeyBinder->sfxFail = RSDK.GetSfx("Stage/Fail.wav");
}

int32 UIKeyBinder_GetButtonListID(void)
{
    switch (Localization->language) {
        default:
        case LANGUAGE_EN: return UIBUTTONPROMPT_KEYBOARD;
        case LANGUAGE_FR: return UIBUTTONPROMPT_KEYBOARD_FR;
        case LANGUAGE_IT: return UIBUTTONPROMPT_KEYBOARD_IT;
        case LANGUAGE_GE: return UIBUTTONPROMPT_KEYBOARD_GE;
        case LANGUAGE_SP: return UIBUTTONPROMPT_KEYBOARD_SP;
    }
}

int32 UIKeyBinder_GetMappings(int32 input, int32 button)
{
    switch (button) {
        case UIKEYBINDER_UP: return ControllerInfo[input].keyUp.keyMap; break;
        case UIKEYBINDER_DOWN: return ControllerInfo[input].keyDown.keyMap; break;
        case UIKEYBINDER_LEFT: return ControllerInfo[input].keyLeft.keyMap; break;
        case UIKEYBINDER_RIGHT: return ControllerInfo[input].keyRight.keyMap; break;
        case UIKEYBINDER_A: return ControllerInfo[input].keyA.keyMap; break;
        case UIKEYBINDER_B: return ControllerInfo[input].keyB.keyMap; break;
        case UIKEYBINDER_X: return ControllerInfo[input].keyX.keyMap; break;
        case UIKEYBINDER_Y: return ControllerInfo[input].keyY.keyMap; break;
        case UIKEYBINDER_START: return ControllerInfo[input].keyStart.keyMap; break;
        default: break;
    }

    return KEYMAP_NO_MAPPING;
}

void UIKeyBinder_SetMappings(int32 input, int32 button, int32 keyMap)
{
    switch (button) {
        case UIKEYBINDER_UP: ControllerInfo[input].keyUp.keyMap = keyMap; break;
        case UIKEYBINDER_DOWN: ControllerInfo[input].keyDown.keyMap = keyMap; break;
        case UIKEYBINDER_LEFT: ControllerInfo[input].keyLeft.keyMap = keyMap; break;
        case UIKEYBINDER_RIGHT: ControllerInfo[input].keyRight.keyMap = keyMap; break;
        case UIKEYBINDER_A: ControllerInfo[input].keyA.keyMap = keyMap; break;
        case UIKEYBINDER_B: ControllerInfo[input].keyB.keyMap = keyMap; break;
        case UIKEYBINDER_X: ControllerInfo[input].keyX.keyMap = keyMap; break;
        case UIKEYBINDER_Y: ControllerInfo[input].keyY.keyMap = keyMap; break;
        case UIKEYBINDER_START: ControllerInfo[input].keyStart.keyMap = keyMap; break;
        default: break;
    }
}

int32 UIKeyBinder_GetKeyNameFrameID(int32 id)
{
    switch (id) {
        case UIKEYBINDER_UP: return UIKEYBINDER_FRAME_UP;
        case UIKEYBINDER_DOWN: return UIKEYBINDER_FRAME_DOWN;
        case UIKEYBINDER_LEFT: return UIKEYBINDER_FRAME_LEFT;
        case UIKEYBINDER_RIGHT: return UIKEYBINDER_FRAME_RIGHT;
        case UIKEYBINDER_A: return UIKEYBINDER_FRAME_A;
        case UIKEYBINDER_B: return UIKEYBINDER_FRAME_B;
        case UIKEYBINDER_X: return UIKEYBINDER_FRAME_X;
        case UIKEYBINDER_Y: return UIKEYBINDER_FRAME_Y;
        case UIKEYBINDER_START: return UIKEYBINDER_FRAME_START;
        default: break;
    }

    return 0;
}

void UIKeyBinder_DrawSprites(void)
{
    RSDK_THIS(UIKeyBinder);

    Vector2 drawPos;
    drawPos.x = self->position.x - self->buttonBounceOffset;
    drawPos.y = self->position.y - self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, self->size.x >> 16, self->size.y >> 16, self->bgEdgeSize, 0xF0, 0xF0, 0xF0);

    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, self->size.x >> 16, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

    drawPos.x = self->position.x + self->buttonBounceOffset;
    drawPos.y = self->position.y + self->buttonBounceOffset;
    drawPos.x += 0xB0000 - (self->size.x >> 1);
    drawPos.y += self->textBounceOffset;
    RSDK.DrawSprite(&self->keyAnimator, &drawPos, false);

    if (self->textVisible) {
        drawPos.x += 0x60000;
        RSDK.DrawSprite(&self->labelAnimator, &drawPos, false);
    }
}

void UIKeyBinder_ActionCB(void) {}

bool32 UIKeyBinder_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIKeyBinder);

    return self->state == UIKeyBinder_State_HandleButtonEnter;
}

bool32 UIKeyBinder_CheckSelectedCB(void)
{
    RSDK_THIS(UIKeyBinder);

    return self->state == UIKeyBinder_State_Selected;
}

void UIKeyBinder_ButtonEnterCB(void)
{
    RSDK_THIS(UIKeyBinder);

    if (self->state != UIKeyBinder_State_HandleButtonEnter) {
        self->textBounceOffset     = 0;
        self->buttonBounceOffset   = 0;
        self->textBounceVelocity   = -0x20000;
        self->buttonBounceVelocity = -0x20000;

        self->state = UIKeyBinder_State_HandleButtonEnter;
    }
}

void UIKeyBinder_ButtonLeaveCB(void)
{
    RSDK_THIS(UIKeyBinder);

    self->state = UIKeyBinder_State_HandleButtonLeave;
}

void UIKeyBinder_SelectedCB(void)
{
    RSDK_THIS(UIKeyBinder);

#if GAME_VERSION != VER_100
    if (!UIKeyBinder->isSelected) {
        UIKeyBinder->isSelected = true;

        EntityUIControl *parent   = (EntityUIControl *)self->parent;
        parent->childHasFocus     = true;
        parent->selectionDisabled = true;

        UIKeyBinder->activeBinder = self;
        self->state               = UIKeyBinder_State_Selected;

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);

        UIKeyBinder_SetMappings(self->inputID + 1, self->type, KEYMAP_AUTO_MAPPING);
    }
#else
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    parent->childHasFocus = true;

    self->state = UIKeyBinder_State_Selected;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);

    UIKeyBinder_SetMappings(self->inputID + 1, self->type, KEYMAP_AUTO_MAPPING);
#endif
}

void UIKeyBinder_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIKeyBinder);

    self->textVisible = true;

    if (self->textBounceOffset) {
        int32 dist = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += dist << 15;

        if (dist < 0) {
            if (self->textBounceOffset < 0)
                self->textBounceOffset = 0;
            else if (dist > 0 && self->textBounceOffset > 0)
                self->textBounceOffset = 0;
        }
        else if (dist > 0 && self->textBounceOffset > 0)
            self->textBounceOffset = 0;
    }

    if (self->buttonBounceOffset) {
        int32 dist = -(self->buttonBounceOffset / abs(self->buttonBounceOffset));
        self->buttonBounceOffset += dist << 16;

        if (dist < 0) {
            if (self->buttonBounceOffset < 0)
                self->buttonBounceOffset = 0;
            else if (dist > 0 && self->buttonBounceOffset > 0)
                self->buttonBounceOffset = 0;
        }
        else if (dist > 0 && self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UIKeyBinder_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIKeyBinder);

    self->textBounceVelocity += 0x4000;
    self->textBounceOffset += self->textBounceVelocity;

    self->textVisible = true;
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

void UIKeyBinder_State_Selected(void)
{
    RSDK_THIS(UIKeyBinder);

    UIKeyBinder_State_HandleButtonEnter();

    self->processButtonCB = UIKeyBinder_ActionCB;
    self->textVisible     = !((self->timer >> 1) & 1);
    self->timer++;
}

#if GAME_VERSION != VER_100
void UIKeyBinder_MoveKeyToActionCB_No(void)
{
    EntityUIKeyBinder *binder = UIKeyBinder->activeBinder;

    if (binder->state == UIKeyBinder_State_Selected) {
        UIKeyBinder_SetMappings(binder->inputID + 1, binder->type, KEYMAP_AUTO_MAPPING);
        UIKeyBinder->activeInputID  = KEYMAP_AUTO_MAPPING;
        UIKeyBinder->activeButtonID = KEYMAP_AUTO_MAPPING;
    }
}

void UIKeyBinder_MoveKeyToActionCB_Yes(void)
{
    EntityUIKeyBinder *binder = UIKeyBinder->activeBinder;

    if (binder->state == UIKeyBinder_State_Selected) {
        // Store the keyMap from the other button
        int32 keyMap = UIKeyBinder_GetMappings(UIKeyBinder->activeInputID, UIKeyBinder->activeButtonID);

        // Remove other button's keyMap
        UIKeyBinder_SetMappings(UIKeyBinder->activeInputID, UIKeyBinder->activeButtonID, KEYMAP_NO_MAPPING);

        // Give the keyMap to the new button
        UIKeyBinder_SetMappings(binder->inputID + 1, binder->type, keyMap);

        EntityUIControl *parent   = (EntityUIControl *)binder->parent;
        parent->selectionDisabled = false;
        parent->childHasFocus     = false;

        UIKeyBinder->isSelected   = false;
        binder->processButtonCB   = UIButton_ProcessButtonCB_Scroll;
        binder->state             = UIKeyBinder_State_HandleButtonEnter;
        UIKeyBinder->activeBinder = NULL;

        RSDK.SetVideoSetting(VIDEOSETTING_CHANGED, false);
        UIKeyBinder->activeInputID  = KEYMAP_AUTO_MAPPING;
        UIKeyBinder->activeButtonID = KEYMAP_AUTO_MAPPING;
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void UIKeyBinder_EditorDraw(void)
{
    RSDK_THIS(UIKeyBinder);

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;
    UIKeyBinder_DrawSprites();
}

void UIKeyBinder_EditorLoad(void)
{
    UIKeyBinder->aniFrames = RSDK.LoadSpriteAnimation("UI/Buttons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIKeyBinder, inputID);
    RSDK_ENUM_VAR("Player 1", UIVSCHARSELECTOR_1P);
    RSDK_ENUM_VAR("Player 2", UIVSCHARSELECTOR_2P);

    RSDK_ACTIVE_VAR(UIKeyBinder, type);
    RSDK_ENUM_VAR("Up", UIKEYBINDER_UP);
    RSDK_ENUM_VAR("Down", UIKEYBINDER_DOWN);
    RSDK_ENUM_VAR("Left", UIKEYBINDER_LEFT);
    RSDK_ENUM_VAR("Right", UIKEYBINDER_RIGHT);
    RSDK_ENUM_VAR("A", UIKEYBINDER_A);
    RSDK_ENUM_VAR("B", UIKEYBINDER_B);
    RSDK_ENUM_VAR("X", UIKEYBINDER_X);
    RSDK_ENUM_VAR("Y", UIKEYBINDER_Y);
    RSDK_ENUM_VAR("Start", UIKEYBINDER_START);
}
#endif

void UIKeyBinder_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(UIKeyBinder, VAR_UINT8, inputID);
}
