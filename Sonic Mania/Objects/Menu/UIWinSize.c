#include "SonicMania.h"

ObjectUIWinSize *UIWinSize;

void UIWinSize_Update(void)
{
    RSDK_THIS(UIWinSize);
    self->touchPosStart.x = self->size.x;
    self->touchPosStart.y = self->size.y;
    self->touchPosEnd.y   = 0;
    self->touchPosStart.x = (self->touchPosStart.x + 3 * self->size.y) >> 1;
    self->touchPosStart.y = self->size.y + 0x60000;
    self->touchPosEnd.x   = -self->touchPosStart.x;
    if (self->selection != self->prevSelection) {
        UIWinSize_SetupText(self);
        self->prevSelection = self->selection;
    }

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && control->state == UIButton_Unknown16)
        UIWinSize_Unknown3(self);
    self->visible = true;
}

void UIWinSize_LateUpdate(void) {}

void UIWinSize_StaticUpdate(void) {}

void UIWinSize_Draw(void)
{
    RSDK_THIS(UIWinSize);
    Vector2 drawPos;

    int32 size  = (self->size.y + self->size.x) >> 16;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->offset;
    drawPos.y -= self->offset;
#if RETRO_USE_PLUS
    UIWidgets_Unknown7(self->size.y >> 16, size, self->height, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF,
                       (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_Unknown7(self->size.y >> 16, size, self->height, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif

    drawPos = self->position;
    drawPos.x += self->offset;
    drawPos.y += self->offset;
    UIWidgets_Unknown7((self->size.y >> 16), size, self->height, 0, 0, 0, drawPos.x, drawPos.y);

    if (self->arrowWidth > 0 && self->flag) {
        drawPos = self->position;
        drawPos.y += self->field_120;
        drawPos.x -= self->arrowWidth << 15;
        drawPos.x += self->offset;
        drawPos.y += self->offset;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        drawPos = self->position;
        drawPos.y += self->field_120;
        drawPos.x += self->arrowWidth << 15;
        drawPos.x += self->offset;
        drawPos.y += self->offset;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }

    if (self->textFlag) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + self->field_120 + self->offset;

        switch (self->align) {
            case 0: drawPos.x = -0x60000 - (self->size.x >> 1) + drawPos.x; break;
            case 2: drawPos.x = drawPos.x + (self->size.x >> 1) - 0x60000; break;
        }

        drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->text, 0, self->text.textLength, 0) << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->text, 0, self->text.textLength, ALIGN_LEFT, 0, 0, 0, false);
    }
}

void UIWinSize_Create(void *data)
{
    RSDK_THIS(UIWinSize);
    if (!SceneInfo->inEditor) {
        int32 winHeight         = RSDK.GetSettingsValue(SETTINGS_WINDOW_HEIGHT);
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x400000;
        self->selection     = winHeight / SCREEN_YSIZE;
        self->height        = self->size.y >> 16;
        self->size.y        = abs(self->size.y);
        self->textFlag      = true;
        RSDK.SetText(&self->text, "", 0x100);
        self->processButtonCB = UIWinSize_ProcessButtonCB;
        self->touchCB         = UIWinSize_ProcessTouchCB;
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator2, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->textAnimator, true, 0);
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
        sprintf(buffer, "%ix", self->selection);
        RSDK.PrependText(&entityPtr->text, buffer);
#if RETRO_GAMEVER != VER_100
        if (Localization->language == LANGUAGE_TC) {
            for (int32 c = 0; c < entityPtr->text.textLength; ++c) {
                if (entityPtr->text.text[c] == 'x')
                    entityPtr->text.text[c] = 20493; //unicode character ID
            }
        }
#endif
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entityPtr->text);
    }
}

void UIWinSize_ApplySettings(void)
{
    RSDK_THIS(UIWinSize);
    RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
    self->field_128 = -0x20000;
    self->field_12C = -0x20000;
    self->field_120 = 0;
    self->offset    = -0x20000;
    UIWinSize_SetupText(self);

    RSDK.SetSettingsValue(SETTINGS_WINDOW_WIDTH, self->selection * ScreenInfo->width);
    RSDK.SetSettingsValue(SETTINGS_WINDOW_HEIGHT, self->selection * ScreenInfo->height);
#if RETRO_USE_PLUS
    RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
#else
    RSDK.UpdateWindow();
#endif
}

void UIWinSize_ProcessButtonCB(void)
{
    RSDK_THIS(UIWinSize);

    int32 sel = self->selection;
    if (UIControl->keyLeft)
        self->selection--;
    if (UIControl->keyRight)
        ++self->selection;

    if (sel != self->selection)
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

    touchStart[0].x = self->touchPosStart.x;
    touchStart[0].y = self->touchPosStart.y;
    touchStart[1].x = self->touchPosStart.x;
    touchStart[1].y = self->touchPosStart.y;

    touchEnd[0].x = self->touchPosEnd.x;
    touchEnd[0].y = self->touchPosEnd.y;
    touchEnd[1].x = -self->touchPosEnd.x;
    touchEnd[1].y = self->touchPosEnd.y;

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
                    self->field_13C = i;
                    pressed           = true;
                }
            }
        }
        else if (self->touchPressed && self->field_13C == i && !self->disabled) {
            callbacks[i]();
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

void UIWinSize_Unknown3(EntityUIWinSize *entity)
{
    if (entity) {
        entity->field_120 = 0;
        entity->offset    = 0;
        entity->field_128 = 0;
        entity->field_12C = 0;
        entity->flag      = false;
        entity->state     = UIWinSize_Unknown7;
    }
}

void UIWinSize_Unknown7(void)
{
    RSDK_THIS(UIWinSize);

    self->textFlag = true;
    if (self->field_120) {
        int32 dist = -(self->field_120 / abs(self->field_120));
        self->field_120 += dist << 15;
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

    if (self->offset) {
        int32 dist = -(self->offset / abs(self->offset));
        self->offset += dist << 16;
        if (dist < 0) {
            if (self->offset < 0) {
                self->offset = 0;
            }
            else if (dist > 0 && self->offset > 0)
                self->offset = 0;
        }
        else if (dist > 0 && self->offset > 0)
            self->offset = 0;
    }
}

void UIWinSize_Unknown8(void)
{
    RSDK_THIS(UIWinSize);

    self->field_128 += 0x4000;
    self->field_120 += self->field_128;
    self->textFlag = true;
    if (self->field_120 >= 0 && self->field_128 > 0) {
        self->field_120 = 0;
        self->field_128 = 0;
    }

    self->field_12C += 0x4800;
    self->offset += self->field_12C;
    if (self->offset >= -0x20000 && self->field_12C > 0) {
        self->offset    = -0x20000;
        self->field_12C = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIWinSize_EditorDraw(void)
{
    RSDK_THIS(UIWinSize);
    int32 winHeight       = SCREEN_YSIZE;
    self->visible       = true;
    self->drawOrder     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->selection     = winHeight / SCREEN_YSIZE;
    self->height        = self->size.y >> 16;
    self->size.y        = abs(self->size.y);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator2, true, 1);
    RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->textAnimator, true, 0);

    UIWinSize_Draw();
}

void UIWinSize_EditorLoad(void) { UIWinSize->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UIWinSize_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIWinSize, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_ENUM, arrowWidth);
}
