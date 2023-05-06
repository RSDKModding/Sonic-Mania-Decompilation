// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIModeButton Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIModeButton *UIModeButton;

void UIModeButton_Update(void)
{
    RSDK_THIS(UIModeButton);

    self->touchPosSizeS.y = 0x380000;
    self->touchPosSizeS.x = 0x800000;
    self->touchPosSizeS.x += self->touchPosSizeS.y;
    self->touchPosSizeS.y += 0x60000;
    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = -0x120000;

    if (self->textFrames != UIWidgets->textFrames || self->wasDisabled != self->disabled) {
        UIModeButton_SetupSprites();
        self->textFrames  = UIWidgets->textFrames;
        self->wasDisabled = self->disabled;
    }

    StateMachine_Run(self->state);
}

void UIModeButton_LateUpdate(void) {}

void UIModeButton_StaticUpdate(void) {}

void UIModeButton_Draw(void)
{
    RSDK_THIS(UIModeButton);

    Vector2 drawPos;
    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, (self->position.y >> 16) - ScreenInfo->position.y);

    drawPos = self->position;
    drawPos.y += self->iconBounceOffset;
    RSDK.DrawSprite(&self->altShadowAnimator, &drawPos, false);

    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset >> 1;
    RSDK.DrawSprite(&self->altIconAnimator, &drawPos, false);

    drawPos = self->position;
    drawPos.y += self->shadowBounceOffset;
    RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);

    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset >> 1;
    RSDK.DrawSprite(&self->iconAnimator, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);

    drawPos = self->position;
    drawPos.x -= self->buttonBounceOffset;
    drawPos.y -= self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 128, 24, 24, 0xF0, 0xF0, 0xF0);

    drawPos = self->position;
    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 128, 24, 24, 0x00, 0x00, 0x00);

    if (self->textVisible) {
        drawPos = self->position;
        drawPos.y += self->textBounceOffset;
        RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
    }
}

void UIModeButton_Create(void *data)
{
    RSDK_THIS(UIModeButton);

    if (!SceneInfo->inEditor) {
        self->visible            = true;
        self->drawGroup          = 2;
        self->active             = ACTIVE_BOUNDS;
        self->updateRange.x      = 0x800000;
        self->updateRange.y      = 0x400000;
        self->textVisible        = true;
        self->shadowBounceOffset = 0x280000;
        self->iconBounceOffset   = 0x280000;

        self->processButtonCB    = UIButton_ProcessButtonCB_Scroll;
        self->touchCB            = UIButton_ProcessTouchCB_Single;
        self->selectedCB         = UIModeButton_SelectedCB;
        self->failCB             = UIModeButton_FailCB;
        self->buttonEnterCB      = UIModeButton_ButtonEnterCB;
        self->buttonLeaveCB      = UIModeButton_ButtonLeaveCB;
        self->checkButtonEnterCB = UIModeButton_CheckButtonEnterCB;
        self->checkSelectedCB    = UIModeButton_CheckSelectedCB;

        UIModeButton_SetupSprites();

        self->textFrames = UIWidgets->textFrames;
    }
}

void UIModeButton_StageLoad(void) { UIModeButton->aniFrames = RSDK.LoadSpriteAnimation("UI/MainIcons.bin", SCOPE_STAGE); }

void UIModeButton_SetupSprites(void)
{
    RSDK_THIS(UIModeButton);

    if (self->disabled) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 7, &self->textAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->iconAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->altIconAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->shadowAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->altShadowAnimator, true, 0);
    }
    else {
        switch (self->buttonID) {
            default:
            case UIMODEBUTTON_MANIA:
                RSDK.SetSpriteAnimation(UIWidgets->textFrames, 1, &self->textAnimator, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->iconAnimator, true, 0);
                RSDK.SetSpriteAnimation(-1, 0, &self->altIconAnimator, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->shadowAnimator, true, 0);
                RSDK.SetSpriteAnimation(-1, 1, &self->altShadowAnimator, true, 0);
                break;

            case UIMODEBUTTON_TIMEATTACK:
                RSDK.SetSpriteAnimation(UIWidgets->textFrames, 1, &self->textAnimator, true, 1);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->iconAnimator, true, 1);
                RSDK.SetSpriteAnimation(-1, 0, &self->altIconAnimator, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->shadowAnimator, true, 1);
                RSDK.SetSpriteAnimation(-1, 1, &self->altShadowAnimator, true, 0);
                break;

            case UIMODEBUTTON_COMPETITION:
                RSDK.SetSpriteAnimation(UIWidgets->textFrames, 1, &self->textAnimator, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->iconAnimator, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->altIconAnimator, true, 3);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->shadowAnimator, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->altShadowAnimator, true, 3);
                break;

            case UIMODEBUTTON_OPTIONS:
                RSDK.SetSpriteAnimation(UIWidgets->textFrames, 1, &self->textAnimator, true, 3);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->iconAnimator, true, 4);
                RSDK.SetSpriteAnimation(-1, 0, &self->altIconAnimator, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->shadowAnimator, true, 4);
                RSDK.SetSpriteAnimation(-1, 1, &self->altShadowAnimator, true, 0);
                break;
        }
    }
}

bool32 UIModeButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIModeButton);

    return self->state == UIModeButton_State_HandleButtonEnter;
}
bool32 UIModeButton_CheckSelectedCB(void)
{
    RSDK_THIS(UIModeButton);

    return self->state == UIModeButton_State_Selected;
}

void UIModeButton_ButtonEnterCB(void)
{
    RSDK_THIS(UIModeButton);

    if (self->state != UIModeButton_State_HandleButtonEnter) {
        self->textBounceOffset     = 0;
        self->shadowBounceOffset   = 0x180000;
        self->iconBounceOffset     = 0x180000;
        self->buttonBounceOffset   = 0;
        self->textBounceVelocity   = -0x20000;
        self->iconBounceVelocity   = -0x80000;
        self->shadowBounceVelocity = -0x80000;
        self->buttonBounceVelocity = -0x20000;

        self->state = UIModeButton_State_HandleButtonEnter;
    }
}

void UIModeButton_SelectedCB(void)
{
    RSDK_THIS(UIModeButton);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (self->buttonID == UIMODEBUTTON_COMPETITION) {
#if MANIA_USE_PLUS
        int32 id = API_GetFilteredInputDeviceID(true, false, 5);
#else
        int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
#endif
        API_ResetInputSlotAssignments();
        API_AssignInputSlotToDevice(CONT_P1, id);
        API_AssignInputSlotToDevice(CONT_P2, INPUT_AUTOASSIGN);
        API_AssignInputSlotToDevice(CONT_P3, INPUT_AUTOASSIGN);
        API_AssignInputSlotToDevice(CONT_P4, INPUT_AUTOASSIGN);
    }

    UITransition_StartTransition(self->actionCB, 14);

    if (self->stopMusic)
        Music_Stop();

    self->isSelected     = false;
    self->state          = UIModeButton_State_Selected;
    parent->backoutTimer = 30;

    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIModeButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UIModeButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UIModeButton);

    self->state = UIModeButton_State_HandleButtonLeave;
}

void UIModeButton_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIModeButton);

    if (self->textBounceOffset < 0) {
        self->textBounceOffset += 0x8000;

        if (self->textBounceOffset > 0)
            self->textBounceOffset = 0;
    }

    self->shadowBounceOffset += (0x280000 - self->shadowBounceOffset) >> 3;
    self->iconBounceOffset += (0x280000 - self->iconBounceOffset) >> 3;

    if (self->buttonBounceOffset < 0) {
        self->buttonBounceOffset += 0x10000;

        if (self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UIModeButton_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIModeButton);

    self->textBounceVelocity += 0x4000;
    self->textBounceOffset += self->textBounceVelocity;

    if (self->textBounceOffset >= 0 && self->textBounceVelocity > 0) {
        self->textBounceOffset   = 0;
        self->textBounceVelocity = 0;
    }

    self->iconBounceVelocity += 0xA000;
    self->shadowBounceOffset += self->iconBounceVelocity;

    if (self->shadowBounceOffset >= -0xC0000 && self->iconBounceVelocity > 0) {
        self->shadowBounceOffset = -0xC0000;
        self->iconBounceVelocity = 0;
    }

    self->shadowBounceVelocity += 0x8000;
    self->iconBounceOffset += self->shadowBounceVelocity;

    if (self->iconBounceOffset >= -0xC0000 && self->shadowBounceVelocity > 0) {
        self->iconBounceOffset     = -0xC0000;
        self->shadowBounceVelocity = 0;
    }

    self->buttonBounceVelocity += 0x4800;
    self->buttonBounceOffset += self->buttonBounceVelocity;

    if (self->buttonBounceOffset >= -0x20000 && self->buttonBounceVelocity > 0) {
        self->buttonBounceOffset   = -0x20000;
        self->buttonBounceVelocity = 0;
    }
}

void UIModeButton_State_Selected(void)
{
    RSDK_THIS(UIModeButton);

    UIModeButton_State_HandleButtonEnter();

    if (++self->timer == 30) {
        self->timer = 0;
        self->state = UIModeButton_State_HandleButtonEnter;
    }

    self->textVisible = !((self->timer >> 1) & 1);
}

#if GAME_INCLUDE_EDITOR
void UIModeButton_EditorDraw(void)
{
    RSDK_THIS(UIModeButton);

    self->visible            = true;
    self->drawGroup          = 2;
    self->updateRange.x      = 0x800000;
    self->updateRange.y      = 0x400000;
    self->textVisible        = true;
    self->shadowBounceOffset = 0x280000;
    self->iconBounceOffset   = 0x280000;

    UIModeButton_SetupSprites();

    self->textFrames = UIWidgets->textFrames;

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;
    UIModeButton_Draw();
}

void UIModeButton_EditorLoad(void)
{
    UIModeButton->aniFrames = RSDK.LoadSpriteAnimation("UI/MainIcons.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIModeButton, buttonID);
    RSDK_ENUM_VAR("Mania Mode", UIMODEBUTTON_MANIA);
    RSDK_ENUM_VAR("Time Attack", UIMODEBUTTON_TIMEATTACK);
    RSDK_ENUM_VAR("Competition", UIMODEBUTTON_COMPETITION);
    RSDK_ENUM_VAR("Options", UIMODEBUTTON_OPTIONS);
}
#endif

void UIModeButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIModeButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIModeButton, VAR_ENUM, buttonID);
    RSDK_EDITABLE_VAR(UIModeButton, VAR_BOOL, stopMusic);
}
