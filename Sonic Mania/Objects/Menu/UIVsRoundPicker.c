// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIVsRoundPicker Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUIVsRoundPicker *UIVsRoundPicker;

void UIVsRoundPicker_Update(void)
{
    RSDK_THIS(UIVsRoundPicker);
    self->touchPosSizeS.x = self->size.x;
    self->touchPosSizeS.y = self->size.y;
    self->touchPosOffsetS.y   = 0;
    self->touchPosSizeS.x = (self->touchPosSizeS.x + 3 * self->size.y) >> 1;
    self->touchPosSizeS.y = self->size.y + 0x60000;
    self->touchPosOffsetS.x   = -(self->touchPosSizeS.x >> 1);

    if (self->textFrames != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->textAnimator, true, 4);
        self->textFrames = UIWidgets->textFrames;
    }
    if (self->val != self->prevVal) {
        UIVsRoundPicker_SetText();
        self->prevVal = self->val;
    }

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control) {
        if (control->state == UIButton_State_HandleButtonLeave)
            UIVsRoundPicker_SetChoiceInactive(self);
    }
}

void UIVsRoundPicker_LateUpdate(void) {}

void UIVsRoundPicker_StaticUpdate(void) {}

void UIVsRoundPicker_Draw(void)
{
    RSDK_THIS(UIVsRoundPicker);
    Vector2 drawPos;

    int32 width = (self->size.y + self->size.x) >> 16;
    drawPos.x   = self->position.x;
    drawPos.y   = self->position.y;
    drawPos.x -= self->buttonBounceOffset;
    drawPos.y -= self->buttonBounceOffset;
#if RETRO_USE_PLUS
    UIWidgets_DrawParallelogram(self->size.y >> 16, width, self->bgEdgeSize, (UIWidgets->buttonColour >> 16) & 0xFF,
                                (UIWidgets->buttonColour >> 8) & 0xFF, (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_DrawParallelogram(self->size.y >> 16, width, self->bgEdgeSize, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif

    drawPos = self->position;
    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    UIWidgets_DrawParallelogram((self->size.y >> 16), width, self->bgEdgeSize, 0, 0, 0, drawPos.x, drawPos.y);

    if (self->arrowWidth > 0 && self->isSelected) {
        if (self->val >= 2) {
            drawPos = self->position;
            drawPos.y += self->textBounceOffset;
            drawPos.x -= self->arrowWidth << 15;
            RSDK.DrawSprite(&self->arrowAnimatorL, &drawPos, false);
        }
        if (self->val < self->maxVal) {
            drawPos = self->position;
            drawPos.y += self->textBounceOffset;
            drawPos.x += self->arrowWidth << 15;
            RSDK.DrawSprite(&self->arrowAnimatorR, &drawPos, false);
        }
    }
    if (self->textVisible)
        UIVsRoundPicker_DrawText();
}

void UIVsRoundPicker_Create(void *data)
{
    RSDK_THIS(UIVsRoundPicker);
    if (!self->val)
        self->val = 3;
    if (!self->maxVal)
        self->maxVal = 12;

    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawOrder       = 2;
        self->active          = ACTIVE_BOUNDS;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x400000;
        self->bgEdgeSize      = self->size.y >> 16;
        self->size.y          = abs(self->size.y);
        self->textVisible     = true;
        self->processButtonCB = UIVsRoundPicker_ProcessButtonCB;
        self->touchCB         = UIVsRoundPicker_ProcessTouchCB;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->textAnimator, true, 4);
        self->textFrames = UIWidgets->textFrames;
        RSDK.SetSpriteAnimation(UIVsRoundPicker->aniFrames, 15, &self->numbersAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorL, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorR, true, 1);
        UIVsRoundPicker_SetText();
    }
}

void UIVsRoundPicker_StageLoad(void) { UIVsRoundPicker->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsRoundPicker_DrawText(void)
{
    RSDK_THIS(UIVsRoundPicker);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + self->textBounceOffset - 0x40000;
    switch (self->align) {
        case 0: drawPos.x = -0x60000 - (self->size.x >> 1) + self->position.x; break;
        case 2: drawPos.x = self->position.x + (self->size.x >> 1) - 0x60000; break;
    }

    int32 width = RSDK.GetStringWidth(UIVsRoundPicker->aniFrames, 15, &self->text, 0, self->text.length, 0) << 16;
    drawPos.x -= (width + 0x300000) >> 1;
    RSDK.DrawText(&self->numbersAnimator, &drawPos, &self->text, 0, self->text.length, ALIGN_LEFT, 0, 0, 0, false);

    drawPos.x += width + 0x40000;
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
}

void UIVsRoundPicker_SetText(void)
{
    RSDK_THIS(UIVsRoundPicker);
    char buffer[0x10];
    sprintf(buffer, "%2d", self->val);
    RSDK.PrependText(&self->text, buffer);
    RSDK.SetSpriteString(UIVsRoundPicker->aniFrames, 15, &self->text);
}

void UIVsRoundPicker_Apply(void)
{
    RSDK_THIS(UIVsRoundPicker);

    RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);

    self->textBounceVelocity   = -0x20000;
    self->buttonBounceVelocity = -0x20000;
    self->textBounceOffset     = 0;
    self->buttonBounceOffset   = -0x20000;
    UIVsRoundPicker_SetText();
}

void UIVsRoundPicker_ProcessButtonCB(void)
{
    RSDK_THIS(UIVsRoundPicker);

    int32 sel = self->val;
    if (UIControl->keyLeft && self->val > 1)
        self->val--;
    if (UIControl->keyRight && self->val < self->maxVal)
        ++self->val;

    if (sel != self->val)
        UIVsRoundPicker_Apply();
}

bool32 UIVsRoundPicker_ProcessTouchCB(void)
{
    RSDK_THIS(UIVsRoundPicker);

    void (*callbacks[2])(void);
    Vector2 touchStart[2];
    Vector2 touchEnd[2];

    callbacks[0] = UIVsRoundPicker_TouchedCB_Left;
    callbacks[1] = UIVsRoundPicker_TouchedCB_Right;

    touchStart[0].x = self->touchPosSizeS.x;
    touchStart[0].y = self->touchPosSizeS.y;
    touchStart[1].x = self->touchPosSizeS.x;
    touchStart[1].y = self->touchPosSizeS.y;

    touchEnd[0].x = self->touchPosOffsetS.x;
    touchEnd[0].y = self->touchPosOffsetS.y;
    touchEnd[1].x = -self->touchPosOffsetS.x;
    touchEnd[1].y = self->touchPosOffsetS.y;

    bool32 pressed = false;
    for (int32 i = 0; i < 2; ++i) {
        if (TouchInfo->count) {
            int32 sizeX = touchStart[i].x >> 1;
            int32 sizeY = touchStart[i].y >> 1;
            for (int32 t = 0; t < TouchInfo->count; ++t) {
                int32 x = (ScreenInfo->position.x << 16) - ((TouchInfo->x[t] * ScreenInfo->width) * -65536.0f);
                int32 y = (ScreenInfo->position.y << 16) - ((TouchInfo->y[t] * ScreenInfo->height) * -65536.0f);

                int32 touchX = abs(touchEnd[i].x + self->position.x - x);
                int32 touchY = abs(touchEnd[i].y + self->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    self->touchID = i;
                    pressed       = true;
                }
            }
        }
        else if (self->touchPressed && self->touchID == i && !self->disabled) {
            callbacks[i]();
        }
    }

    self->touchPressed = pressed;
    return self->touchPressed;
}

void UIVsRoundPicker_TouchedCB_Left(void)
{
    RSDK_THIS(UIVsRoundPicker);

    if (self->val > 1) {
        self->val--;
        UIVsRoundPicker_Apply();
    }
}

void UIVsRoundPicker_TouchedCB_Right(void)
{
    RSDK_THIS(UIVsRoundPicker);

    if (self->val < self->maxVal) {
        self->val++;
        UIVsRoundPicker_Apply();
    }
}

void UIVsRoundPicker_SetChoiceActive(EntityUIVsRoundPicker *entity)
{
    if (entity) {
        entity->active               = ACTIVE_BOUNDS;
        entity->textBounceOffset     = 0;
        entity->buttonBounceOffset   = 0;
        entity->textBounceVelocity   = -0x20000;
        entity->buttonBounceVelocity = -0x20000;
        entity->isSelected           = true;
        entity->state                = UIVsRoundPicker_State_HandleButtonEnter;
    }
}

void UIVsRoundPicker_SetChoiceInactive(EntityUIVsRoundPicker *entity)
{
    if (entity) {
        entity->textBounceOffset     = 0;
        entity->buttonBounceOffset   = 0;
        entity->textBounceVelocity   = 0;
        entity->buttonBounceVelocity = 0;
        entity->isSelected           = false;
        entity->state                = UIVsRoundPicker_State_HandleButtonLeave;
    }
}

void UIVsRoundPicker_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIVsRoundPicker);

    self->textVisible = true;
    if (self->textBounceOffset) {
        int32 dist = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += dist << 15;
        if (dist < 0) {
            if (self->textBounceOffset < 0) {
                self->textBounceOffset = 0;
            }
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
            if (self->buttonBounceOffset < 0) {
                self->buttonBounceOffset = 0;
            }
            else if (dist > 0 && self->buttonBounceOffset > 0)
                self->buttonBounceOffset = 0;
        }
        else if (dist > 0 && self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UIVsRoundPicker_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIVsRoundPicker);

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

#if RETRO_INCLUDE_EDITOR
void UIVsRoundPicker_EditorDraw(void)
{
    RSDK_THIS(UIVsRoundPicker);

    int sizeY = self->size.y;

    self->visible       = true;
    self->drawOrder     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    self->isSelected    = true;
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->textAnimator, true, 4);
    self->textFrames = UIWidgets->textFrames;
    RSDK.SetSpriteAnimation(UIVsRoundPicker->aniFrames, 15, &self->numbersAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorL, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorR, true, 1);

    UIVsRoundPicker_Draw();

    self->size.y = sizeY;
}

void UIVsRoundPicker_EditorLoad(void) { UIVsRoundPicker->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UIVsRoundPicker_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, arrowWidth);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, val);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, maxVal);
}
