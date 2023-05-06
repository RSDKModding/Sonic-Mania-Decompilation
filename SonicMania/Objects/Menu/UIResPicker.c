// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIResPicker Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if GAME_VERSION != VER_100
ObjectUIResPicker *UIResPicker;

void UIResPicker_Update(void)
{
    RSDK_THIS(UIResPicker);

    self->touchPosSizeS.x   = self->size.x;
    self->touchPosSizeS.y   = self->size.y;
    self->touchPosOffsetS.y = 0;
    self->touchPosSizeS.x   = (self->touchPosSizeS.x + 3 * self->size.y) >> 1;
    self->touchPosSizeS.y   = self->size.y + 0x60000;
    self->touchPosOffsetS.x = -self->touchPosSizeS.x;

    if (self->selection != self->prevSelection) {
        UIResPicker_GetDisplayInfo(self);
        self->prevSelection = self->selection;
    }

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && control->state == UIButton_State_HandleButtonLeave)
        UIResPicker_SetChoiceInactive(self);

    self->visible = true;
}

void UIResPicker_LateUpdate(void) {}

void UIResPicker_StaticUpdate(void) {}

void UIResPicker_Draw(void)
{
    RSDK_THIS(UIResPicker);

    Vector2 drawPos;
    int32 width = (self->size.y + self->size.x) >> 16;

    drawPos.x = self->position.x - self->buttonBounceOffset;
    drawPos.y = self->position.y - self->buttonBounceOffset;
#if MANIA_USE_PLUS
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, (UIWidgets->buttonColor >> 16) & 0xFF,
                                (UIWidgets->buttonColor >> 8) & 0xFF, (UIWidgets->buttonColor) & 0xFF);
#else
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, 0xF0, 0xF0, 0xF0);
#endif

    drawPos = self->position;
    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, (self->size.y >> 16), self->bgEdgeSize, 0x00, 0x00, 0x00);

    if (self->arrowWidth > 0 && self->isSelected) {
        drawPos = self->position;
        drawPos.y += self->textBounceOffset;
        drawPos.x -= self->arrowWidth << 15;
        drawPos.x += self->buttonBounceOffset;
        drawPos.y += self->buttonBounceOffset;
        RSDK.DrawSprite(&self->arrowAnimatorL, &drawPos, false);

        drawPos = self->position;
        drawPos.y += self->textBounceOffset;
        drawPos.x += self->arrowWidth << 15;
        drawPos.x += self->buttonBounceOffset;
        drawPos.y += self->buttonBounceOffset;
        RSDK.DrawSprite(&self->arrowAnimatorR, &drawPos, false);
    }

    if (self->textVisible) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + self->textBounceOffset + self->buttonBounceOffset;

        switch (self->align) {
            case UIBUTTON_ALIGN_LEFT: drawPos.x = -0x60000 - (self->size.x >> 1) + drawPos.x; break;

            default:
            case UIBUTTON_ALIGN_CENTER: break;

            case UIBUTTON_ALIGN_RIGHT: drawPos.x = drawPos.x + (self->size.x >> 1) - 0x60000; break;
        }

        width = RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->text, 0, self->text.length, 0);
        drawPos.x -= width << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->text, 0, self->text.length, ALIGN_LEFT, 0, 0, 0, false);
    }
}

void UIResPicker_Create(void *data)
{
    RSDK_THIS(UIResPicker);

    if (!SceneInfo->inEditor) {
        self->selection = -2;

        self->visible       = true;
        self->drawGroup     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;
        self->bgEdgeSize    = self->size.y >> 16;
        self->size.y        = abs(self->size.y);

        self->textVisible = true;
        RSDK.InitString(&self->text, "", 0x100);
        self->processButtonCB = UIResPicker_ProcessButtonCB;
        self->touchCB         = UIResPicker_ProcessTouchCB;

        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorL, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorR, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->textAnimator, true, 0);
    }
}

void UIResPicker_StageLoad(void) { UIResPicker->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIResPicker_GetDisplayInfo(EntityUIResPicker *entity)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        char buffer[0x40];
        RSDK.GetDisplayInfo(&entity->selection, &entity->displayWidth, &entity->displayHeight, &entity->displayRefreshRate, buffer);

        if (strcmp(buffer, "DEFAULT") != 0)
            RSDK.SetString(&entity->text, buffer);
        else
            Localization_GetString(&entity->text, STR_DEFAULT);

        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &entity->text);
    }
}

void UIResPicker_ApplySettings(void)
{
    RSDK_THIS(UIResPicker);

    RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);

    self->textBounceVelocity   = -0x20000;
    self->buttonBounceVelocity = -0x20000;
    self->textBounceOffset     = 0;
    self->buttonBounceOffset   = -0x20000;

    UIResPicker_GetDisplayInfo(self);

    RSDK.SetVideoSetting(VIDEOSETTING_FSWIDTH, self->displayWidth);
    RSDK.SetVideoSetting(VIDEOSETTING_FSHEIGHT, self->displayHeight);
    if (!self->displayRefreshRate)
        RSDK.SetVideoSetting(VIDEOSETTING_REFRESHRATE, 60);
    else
        RSDK.SetVideoSetting(VIDEOSETTING_REFRESHRATE, self->displayRefreshRate);

#if MANIA_USE_PLUS
    RSDK.SetVideoSetting(VIDEOSETTING_CHANGED, true);
#else
    RSDK.UpdateWindow();
#endif
}

void UIResPicker_ProcessButtonCB(void)
{
    RSDK_THIS(UIResPicker);

    int32 selection = self->selection;

    if (UIControl->anyLeftPress)
        self->selection--;
    if (UIControl->anyRightPress)
        ++self->selection;

    if (selection != self->selection)
        UIResPicker_ApplySettings();
}

bool32 UIResPicker_ProcessTouchCB(void)
{
    RSDK_THIS(UIResPicker);

    void (*callbacks[2])(void);
    Vector2 touchStart[2];
    Vector2 touchEnd[2];

    callbacks[0] = UIResPicker_TouchedCB_Left;
    callbacks[1] = UIResPicker_TouchedCB_Right;

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

    return self->touchPressed;
}

void UIResPicker_TouchedCB_Left(void)
{
    RSDK_THIS(UIResPicker);

    self->selection--;

    if (self->selection != self->selection - 1)
        UIResPicker_ApplySettings();
}

void UIResPicker_TouchedCB_Right(void)
{
    RSDK_THIS(UIResPicker);

    self->selection++;

    if (self->selection != self->selection + 1)
        UIResPicker_ApplySettings();
}

void UIResPicker_SetChoiceActive(EntityUIResPicker *entity)
{
    if (entity) {
        entity->active  = ACTIVE_BOUNDS;
        entity->visible = true;

        entity->textBounceOffset     = 0;
        entity->buttonBounceOffset   = 0;
        entity->textBounceVelocity   = -0x20000;
        entity->buttonBounceVelocity = -0x20000;

        entity->isSelected = true;
        entity->state      = UIResPicker_State_HandleButtonEnter;
    }
}

void UIResPicker_SetChoiceInactive(EntityUIResPicker *entity)
{
    if (entity) {
        entity->textBounceOffset     = 0;
        entity->buttonBounceOffset   = 0;
        entity->textBounceVelocity   = 0;
        entity->buttonBounceVelocity = 0;

        entity->isSelected = false;
        entity->state      = UIResPicker_State_HandleButtonLeave;
    }
}

void UIResPicker_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIResPicker);

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

void UIResPicker_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIResPicker);

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

#if GAME_INCLUDE_EDITOR
void UIResPicker_EditorDraw(void)
{
    RSDK_THIS(UIResPicker);

    int32 sizeY = self->size.y;

    self->selection     = -2;
    self->drawGroup     = 2;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);

    if (UIWidgets) {
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorL, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorR, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->textAnimator, true, 0);
    }

    self->isSelected = showGizmos();
    self->inkEffect  = self->disabled ? INK_BLEND : INK_NONE;

    UIResPicker_Draw();

    self->size.y = sizeY;
}

void UIResPicker_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(UIResPicker, align);
    RSDK_ENUM_VAR("Left", UIBUTTON_ALIGN_LEFT);
    RSDK_ENUM_VAR("Center", UIBUTTON_ALIGN_CENTER);
    RSDK_ENUM_VAR("Right", UIBUTTON_ALIGN_RIGHT);
}
#endif

void UIResPicker_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIResPicker, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIResPicker, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIResPicker, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIResPicker, VAR_ENUM, arrowWidth);
}
#endif