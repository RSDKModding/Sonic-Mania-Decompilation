// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIChoice Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIChoice *UIChoice;

void UIChoice_Update(void)
{
    RSDK_THIS(UIChoice);

    EntityUIButton *parent = (EntityUIButton *)self->parent;
    bool32 disabled        = self->disabled;

    self->touchPosSizeS.x   = self->size.x;
    self->touchPosSizeS.y   = self->size.y;
    self->touchPosOffsetS.y = 0;
    self->touchPosSizeS.x   = (self->touchPosSizeS.x + 3 * self->size.y) >> 1;
    self->touchPosSizeS.y   = self->size.y + 0x60000;
    self->touchPosOffsetS.x = -(self->touchPosSizeS.x >> 1);

    if (parent && (disabled || parent->disabled))
        disabled = true;

    if (self->aniFrames != UIWidgets->textFrames || self->isDisabled != disabled) {
        if (disabled)
            RSDK.SetSpriteAnimation(UIWidgets->textFrames, 7, &self->labelAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->labelAnimator, true, self->frameID);

        self->aniFrames  = UIWidgets->textFrames;
        self->isDisabled = disabled;
    }

    StateMachine_Run(self->state);

    if (parent && parent->state == UIButton_State_HandleButtonLeave) {
        self->textBounceOffset   = 0;
        self->buttonBounceOffset = 0;
        self->isSelected         = false;
        self->state              = UIChoice_State_HandleButtonLeave;
    }
}

void UIChoice_LateUpdate(void) {}

void UIChoice_StaticUpdate(void) {}

void UIChoice_Draw(void)
{
    RSDK_THIS(UIChoice);

    Vector2 drawPos;
    int32 size = (self->size.x + self->size.y) >> 16;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->buttonBounceOffset;
    drawPos.y -= self->buttonBounceOffset;
#if MANIA_USE_PLUS
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, size, self->size.y >> 16, self->bgEdgeSize, (UIWidgets->buttonColor >> 16) & 0xFF,
                                (UIWidgets->buttonColor >> 8) & 0xFF, (UIWidgets->buttonColor) & 0xFF);
#else
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, size, self->size.y >> 16, self->bgEdgeSize, 0xF0, 0xF0, 0xF0);
#endif

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, size, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

    EntityUIButton *parent = (EntityUIButton *)self->parent;
    if (self->arrowWidth > 0 && self->isSelected && !(self->disabled || parent->disabled)) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y += self->textBounceOffset;
        drawPos.x -= self->arrowWidth << 15;
        drawPos.x += self->buttonBounceOffset;
        drawPos.y += self->buttonBounceOffset;
        RSDK.DrawSprite(&self->leftArrowAnimator, &drawPos, false);

        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y += self->textBounceOffset;
        drawPos.x += self->arrowWidth << 15;
        drawPos.x += self->buttonBounceOffset;
        drawPos.y += self->buttonBounceOffset;
        RSDK.DrawSprite(&self->rightArrowAnimator, &drawPos, false);
    }

    if (self->textVisible) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        drawPos.y = self->textBounceOffset + self->position.y;
        drawPos.x = self->buttonBounceOffset + self->position.x;
        drawPos.y += self->buttonBounceOffset;

        switch (self->align) {
            case UIBUTTON_ALIGN_LEFT: drawPos.x += -0x60000 - (self->size.x >> 1); break;

            default:
            case UIBUTTON_ALIGN_CENTER: break;

            case UIBUTTON_ALIGN_RIGHT: drawPos.x += (self->size.x >> 1) - 0x60000; break;
        }

        if (self->auxIcon)
            RSDK.DrawSprite(&self->iconAnimator, &drawPos, false);

        if (!self->noText)
            RSDK.DrawSprite(&self->labelAnimator, &drawPos, false);
    }
}

void UIChoice_Create(void *data)
{
    RSDK_THIS(UIChoice);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 2;
        self->active        = ACTIVE_NEVER;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;
        self->bgEdgeSize    = self->size.y >> 16;
        self->size.y        = abs(self->size.y);
        self->textVisible   = true;

        self->touchCB = UIChoice_CheckTouch;

        self->aniFrames = UIWidgets->textFrames;
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->labelAnimator, true, self->frameID);
        RSDK.SetSpriteAnimation(UIChoice->aniFrames, self->auxListID, &self->iconAnimator, true, self->auxFrameID);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->leftArrowAnimator, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->rightArrowAnimator, true, 1);
    }
}

void UIChoice_StageLoad(void) { UIChoice->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIChoice_SetChoiceActive(EntityUIChoice *choice)
{
    if (choice) {
        EntityUIButton *parent = (EntityUIButton *)choice->parent;

        choice->active = ACTIVE_BOUNDS;
        if (!parent->disabled)
            choice->visible = true;

        choice->textBounceOffset     = 0;
        choice->buttonBounceOffset   = 0;
        choice->textBounceVelocity   = -0x20000;
        choice->buttonBounceVelocity = -0x20000;
        choice->isSelected           = true;
        choice->state                = UIChoice_State_HandleButtonEnter;
    }
}

void UIChoice_SetChoiceInactive(EntityUIChoice *choice)
{
    if (choice) {
        choice->textBounceOffset   = 0;
        choice->buttonBounceOffset = 0;
        choice->isSelected         = false;
        choice->state              = UIChoice_State_HandleButtonLeave;
    }
}

void UIChoice_TouchedCB_Left(void)
{
    RSDK_THIS(UIChoice);

    EntityUIButton *parent = (EntityUIButton *)self->parent;

    int32 selection = parent->selection;
    if (--selection < 0) {
        while (selection < 0) selection += parent->choiceCount;
    }
    EntityUIButton *choice = UIButton_GetChoicePtr(parent, selection);

    while ((choice && choice->disabled) && selection != parent->selection) {
        if (--selection < 0) {
            while (selection < 0) selection += parent->choiceCount;
        }

        choice = UIButton_GetChoicePtr(parent, selection);
    }

    if (selection != parent->selection) {
        UIButton_SetChoiceSelectionWithCB(parent, selection);
        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
    }
}

void UIChoice_TouchedCB_Right(void)
{
    RSDK_THIS(UIChoice);

    EntityUIButton *parent = (EntityUIButton *)self->parent;

    int32 selection        = (parent->selection + 1) % parent->choiceCount;
    EntityUIButton *choice = UIButton_GetChoicePtr(parent, selection);

    while ((choice && choice->disabled) && selection != parent->selection) {
        selection = (selection + 1) % parent->choiceCount;
        choice    = UIButton_GetChoicePtr(parent, selection);
    }

    if (selection != parent->selection) {
        UIButton_SetChoiceSelectionWithCB(parent, selection);
        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
    }
}

bool32 UIChoice_CheckTouch(void)
{
    RSDK_THIS(UIChoice);

    void (*callbacks[2])(void);
    Vector2 touchStart[2];
    Vector2 touchEnd[2];

    callbacks[0] = UIChoice_TouchedCB_Left;
    callbacks[1] = UIChoice_TouchedCB_Right;

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
                int32 x = (ScreenInfo->position.x << 16) - ((TouchInfo->x[t] * ScreenInfo->size.x) * -65536.0f);
                int32 y = (ScreenInfo->position.y << 16) - ((TouchInfo->y[t] * ScreenInfo->size.y) * -65536.0f);

                int32 touchX = abs(touchEnd[i].x + self->position.x - x);
                int32 touchY = abs(touchEnd[i].y + self->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    self->touchID = i;
                    pressed       = true;
                }
            }
        }
        else if (self->touchPressed && self->touchID == i && !self->disabled) {
#if RETRO_USE_MOD_LOADER
            StateMachine_Run(callbacks[i]);
#else
            callbacks[i]();
#endif
        }
    }

    self->touchPressed = pressed;

    return pressed;
}

void UIChoice_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIChoice);

    if (self->textBounceOffset) {
        int32 offset = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += offset << 15;

        if (offset < 0 && self->textBounceOffset < 0)
            self->textBounceOffset = 0;
        else if (offset > 0 && self->textBounceOffset > 0)
            self->textBounceOffset = 0;
    }

    if (self->buttonBounceOffset) {
        int32 offset = -(self->buttonBounceOffset / abs(self->buttonBounceOffset));
        self->buttonBounceOffset += offset << 16;

        if (offset < 0 && self->buttonBounceOffset < 0)
            self->buttonBounceOffset = 0;
        else if (offset > 0 && self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UIChoice_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIChoice);

    self->textBounceVelocity += 0x4000;
    self->textBounceOffset += self->textBounceVelocity;

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

#if GAME_INCLUDE_EDITOR
void UIChoice_EditorDraw(void)
{
    RSDK_THIS(UIChoice);

    int32 sizeY = self->size.y;

    self->drawGroup     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    self->textVisible   = true;
    self->aniFrames     = UIWidgets->textFrames;

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->labelAnimator, true, self->frameID);
    RSDK.SetSpriteAnimation(UIChoice->aniFrames, self->auxListID, &self->iconAnimator, true, self->auxFrameID);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->leftArrowAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->rightArrowAnimator, true, 1);

    self->isSelected = showGizmos();
    // Crash prevention
    self->parent = (Entity *)self;

    UIChoice_Draw();

    self->size.y = sizeY;
}

void UIChoice_EditorLoad(void)
{
    UIChoice->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIChoice, align);
    RSDK_ENUM_VAR("Left", UIBUTTON_ALIGN_LEFT);
    RSDK_ENUM_VAR("Center", UIBUTTON_ALIGN_CENTER);
    RSDK_ENUM_VAR("Right", UIBUTTON_ALIGN_RIGHT);
}
#endif

void UIChoice_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIChoice, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIChoice, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_BOOL, noText);
    RSDK_EDITABLE_VAR(UIChoice, VAR_BOOL, auxIcon);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, auxListID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, auxFrameID);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIChoice, VAR_ENUM, arrowWidth);
}
