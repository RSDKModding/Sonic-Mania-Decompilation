// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIWinSize Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if GAME_VERSION != VER_100
ObjectUIWinSize *UIWinSize;

void UIWinSize_Update(void)
{
    RSDK_THIS(UIWinSize);

    self->touchPosSizeS.x   = self->size.x;
    self->touchPosSizeS.y   = self->size.y;
    self->touchPosOffsetS.y = 0;
    self->touchPosSizeS.x   = (self->touchPosSizeS.x + 3 * self->size.y) >> 1;
    self->touchPosSizeS.y   = self->size.y + 0x60000;
    self->touchPosOffsetS.x = -self->touchPosSizeS.x;

    if (self->selection != self->prevSelection) {
        UIWinSize_SetupText(self);
        self->prevSelection = self->selection;
    }

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && control->state == UIButton_State_HandleButtonLeave)
        UIWinSize_SetChoiceInactive(self);

    self->visible = true;
}

void UIWinSize_LateUpdate(void) {}

void UIWinSize_StaticUpdate(void) {}

void UIWinSize_Draw(void)
{
    RSDK_THIS(UIWinSize);

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
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

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

        drawPos.x -= RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->text, 0, self->text.length, 0) << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->text, 0, self->text.length, ALIGN_LEFT, 0, 0, 0, false);
    }
}

void UIWinSize_Create(void *data)
{
    RSDK_THIS(UIWinSize);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;

        self->selection  = RSDK.GetVideoSetting(VIDEOSETTING_WINDOW_HEIGHT) / SCREEN_YSIZE;
        self->bgEdgeSize = self->size.y >> 16;
        self->size.y     = abs(self->size.y);

        self->textVisible = true;
        RSDK.InitString(&self->text, "", 0x100);
        self->processButtonCB = UIWinSize_ProcessButtonCB;
        self->touchCB         = UIWinSize_ProcessTouchCB;

        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorL, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorR, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->textAnimator, true, 0);
    }
}

void UIWinSize_StageLoad(void) { UIWinSize->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIWinSize_SetupText(EntityUIWinSize *entityPtr)
{
    RSDK_THIS(UIWinSize);

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        int32 height = 0;
        RSDK.GetWindowSize(NULL, &height);

        self->maxScale = height / SCREEN_YSIZE;
        if (self->selection < 1)
            self->selection = self->maxScale - 1;

        if (self->selection >= self->maxScale)
            self->selection = 1;

        char buffer[0x10];
        sprintf_s(buffer, (int32)sizeof(buffer), "%ix", self->selection);

        RSDK.SetString(&entityPtr->text, buffer);
#if GAME_VERSION != VER_100
        if (Localization->language == LANGUAGE_TC) {
            for (int32 c = 0; c < entityPtr->text.length; ++c) {
                if (entityPtr->text.chars[c] == 'x')
                    entityPtr->text.chars[c] = 20493; // unicode character ID
            }
        }
#endif

        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &entityPtr->text);
    }
}

void UIWinSize_ApplySettings(void)
{
    RSDK_THIS(UIWinSize);

    RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);

    self->textBounceVelocity   = -0x20000;
    self->buttonBounceVelocity = -0x20000;
    self->textBounceOffset     = 0;
    self->buttonBounceOffset   = -0x20000;

    UIWinSize_SetupText(self);

    RSDK.SetVideoSetting(VIDEOSETTING_WINDOW_WIDTH, self->selection * ScreenInfo->size.x);
    RSDK.SetVideoSetting(VIDEOSETTING_WINDOW_HEIGHT, self->selection * ScreenInfo->size.y);

#if MANIA_USE_PLUS
    RSDK.SetVideoSetting(VIDEOSETTING_CHANGED, true);
#else
    RSDK.UpdateWindow();
#endif
}

void UIWinSize_ProcessButtonCB(void)
{
    RSDK_THIS(UIWinSize);

    int32 selection = self->selection;

    if (UIControl->anyLeftPress)
        self->selection--;

    if (UIControl->anyRightPress)
        ++self->selection;

    if (selection != self->selection)
        UIWinSize_ApplySettings();
}

bool32 UIWinSize_ProcessTouchCB(void)
{
    RSDK_THIS(UIWinSize);

    void (*callbacks[2])(void);
    Vector2 touchStart[2];
    Vector2 touchEnd[2];

    callbacks[0] = UIWinSize_TouchedCB_Left;
    callbacks[1] = UIWinSize_TouchedCB_Right;

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

void UIWinSize_TouchedCB_Left(void)
{
    RSDK_THIS(UIWinSize);

    self->selection--;

    if (self->selection != self->selection - 1)
        UIWinSize_ApplySettings();
}

void UIWinSize_TouchedCB_Right(void)
{
    RSDK_THIS(UIWinSize);

    self->selection++;

    if (self->selection != self->selection + 1)
        UIWinSize_ApplySettings();
}

void UIWinSize_SetChoiceActive(EntityUIWinSize *entity)
{
    if (entity) {
        entity->active  = ACTIVE_BOUNDS;
        entity->visible = true;

        entity->textBounceOffset     = 0;
        entity->buttonBounceOffset   = 0;
        entity->textBounceVelocity   = -0x20000;
        entity->buttonBounceVelocity = -0x20000;

        entity->isSelected = true;
        entity->state      = UIWinSize_State_HandleButtonEnter;
    }
}

void UIWinSize_SetChoiceInactive(EntityUIWinSize *entity)
{
    if (entity) {
        entity->textBounceOffset     = 0;
        entity->buttonBounceOffset   = 0;
        entity->textBounceVelocity   = 0;
        entity->buttonBounceVelocity = 0;

        entity->isSelected = false;
        entity->state      = UIWinSize_State_HandleButtonLeave;
    }
}

void UIWinSize_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIWinSize);

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

void UIWinSize_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIWinSize);

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
void UIWinSize_EditorDraw(void)
{
    RSDK_THIS(UIWinSize);
    int32 sizeY = self->size.y;

    self->visible       = true;
    self->drawGroup     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->selection     = 1;
    self->bgEdgeSize    = self->size.y >> 16;
    self->size.y        = abs(self->size.y);

    if (UIWidgets) {
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorL, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 2, &self->arrowAnimatorR, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->textAnimator, true, 0);
    }

    self->isSelected = showGizmos();
    self->inkEffect  = self->disabled ? INK_BLEND : INK_NONE;

    UIWinSize_Draw();

    self->size.y = sizeY;
}

void UIWinSize_EditorLoad(void)
{
    UIWinSize->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIWinSize, align);
    RSDK_ENUM_VAR("Left", UIBUTTON_ALIGN_LEFT);
    RSDK_ENUM_VAR("Center", UIBUTTON_ALIGN_CENTER);
    RSDK_ENUM_VAR("Right", UIBUTTON_ALIGN_RIGHT);
}
#endif

void UIWinSize_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIWinSize, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_ENUM, arrowWidth);
}
#endif