#include "SonicMania.h"

ObjectUIVsRoundPicker *UIVsRoundPicker;

void UIVsRoundPicker_Update(void)
{
    RSDK_THIS(UIVsRoundPicker);
    self->touchPosStart.x = self->size.x;
    self->touchPosStart.y = self->size.y;
    self->touchPosEnd.y   = 0;
    self->touchPosStart.x = (self->touchPosStart.x + 3 * self->size.y) >> 1;
    self->touchPosStart.y = self->size.y + 0x60000;
    self->touchPosEnd.x   = -(self->touchPosStart.x >> 1);

    if (self->textSpriteIndex != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &self->animator1, true, 4);
        self->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
    if (self->val != self->prevVal) {
        UIVsRoundPicker_SetText();
        self->prevVal = self->val;
    }

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control) {
        if (control->state == UIButton_Unknown16)
            UIVsRoundPicker_Unknown4(self);
    }
}

void UIVsRoundPicker_LateUpdate(void) {}

void UIVsRoundPicker_StaticUpdate(void) {}

void UIVsRoundPicker_Draw(void)
{
    RSDK_THIS(UIVsRoundPicker);
    Vector2 drawPos;

    int32 size  = (self->size.y + self->size.x) >> 16;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= self->field_128;
    drawPos.y -= self->field_128;
#if RETRO_USE_PLUS
    UIWidgets_DrawParallelogram(self->size.y >> 16, size, self->dword120, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF,
                       (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_DrawParallelogram(self->size.y >> 16, size, self->dword120, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif

    drawPos = self->position;
    drawPos.x += self->field_128;
    drawPos.y += self->field_128;
    UIWidgets_DrawParallelogram((self->size.y >> 16), size, self->dword120, 0, 0, 0, drawPos.x, drawPos.y);

    if (self->arrowWidth > 0 && self->flag) {
        if (self->val >= 2) {
            drawPos = self->position;
            drawPos.y += self->field_124;
            drawPos.x -= self->arrowWidth << 15;
            RSDK.DrawSprite(&self->animator3, &drawPos, false);
        }
        if (self->val < self->maxVal) {
            drawPos = self->position;
            drawPos.y += self->field_124;
            drawPos.x += self->arrowWidth << 15;
            RSDK.DrawSprite(&self->animator4, &drawPos, false);
        }
    }
    if (self->textFlag)
        UIVsRoundPicker_Unknown2();
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
        self->dword120        = self->size.y >> 16;
        self->size.y          = abs(self->size.y);
        self->textFlag        = true;
        self->processButtonCB = UIVsRoundPicker_ProcessButtonCB;
        self->touchCB         = UIVsRoundPicker_ProcessTouchCB;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &self->animator1, true, 4);
        self->textSpriteIndex = UIWidgets->textSpriteIndex;
        RSDK.SetSpriteAnimation(UIVsRoundPicker->aniFrames, 15, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator4, true, 1);
        UIVsRoundPicker_SetText();
    }
}

void UIVsRoundPicker_StageLoad(void) { UIVsRoundPicker->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsRoundPicker_Unknown2(void)
{
    RSDK_THIS(UIVsRoundPicker);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + self->field_124 - 0x40000;
    switch (self->align) {
        case 0: drawPos.x = -0x60000 - (self->size.x >> 1) + self->position.x; break;
        case 2: drawPos.x = self->position.x + (self->size.x >> 1) - 0x60000; break;
    }

    int32 width = RSDK.GetStringWidth(UIVsRoundPicker->aniFrames, 15, &self->text, 0, self->text.textLength, 0) << 16;
    drawPos.x -= (width + 0x300000) >> 1;
    RSDK.DrawText(&self->animator2, &drawPos, &self->text, 0, self->text.textLength, ALIGN_LEFT, 0, 0, 0, false);

    drawPos.x += width + 0x40000;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
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

    self->field_12C = -0x20000;
    self->field_130 = -0x20000;
    self->field_124 = 0;
    self->field_128 = -0x20000;
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
                    self->touchID = i;
                    pressed         = true;
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

void UIVsRoundPicker_Unknown4(EntityUIVsRoundPicker *entity)
{
    if (entity) {
        entity->field_124 = 0;
        entity->field_128 = 0;
        entity->field_12C = 0;
        entity->field_130 = 0;
        entity->flag      = false;
        entity->state     = UIVsRoundPicker_Unknown8;
    }
}

void UIVsRoundPicker_Unknown8(void)
{
    RSDK_THIS(UIVsRoundPicker);

    self->textFlag = true;
    if (self->field_124) {
        int32 dist = -(self->field_124 / abs(self->field_124));
        self->field_124 += dist << 15;
        if (dist < 0) {
            if (self->field_124 < 0) {
                self->field_124 = 0;
            }
            else if (dist > 0 && self->field_124 > 0)
                self->field_124 = 0;
        }
        else if (dist > 0 && self->field_124 > 0)
            self->field_124 = 0;
    }

    if (self->field_128) {
        int32 dist = -(self->field_128 / abs(self->field_128));
        self->field_128 += dist << 16;
        if (dist < 0) {
            if (self->field_128 < 0) {
                self->field_128 = 0;
            }
            else if (dist > 0 && self->field_128 > 0)
                self->field_128 = 0;
        }
        else if (dist > 0 && self->field_128 > 0)
            self->field_128 = 0;
    }
}

void UIVsRoundPicker_Unknown9(void)
{
    RSDK_THIS(UIVsRoundPicker);

    self->field_12C += 0x4000;
    self->field_124 += self->field_12C;
    self->textFlag = true;
    if (self->field_124 >= 0 && self->field_12C > 0) {
        self->field_124 = 0;
        self->field_12C = 0;
    }

    self->field_130 += 0x4800;
    self->field_128 += self->field_130;
    if (self->field_128 >= -0x20000 && self->field_130 > 0) {
        self->field_128 = -0x20000;
        self->field_130 = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIVsRoundPicker_EditorDraw(void)
{
    RSDK_THIS(UIVsRoundPicker);
    self->visible         = true;
    self->drawOrder       = 2;
    self->active          = ACTIVE_BOUNDS;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x400000;
    self->dword120        = self->size.y >> 16;
    self->size.y          = abs(self->size.y);
    self->flag            = true;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &self->animator1, true, 4);
    self->textSpriteIndex = UIWidgets->textSpriteIndex;
    RSDK.SetSpriteAnimation(UIVsRoundPicker->aniFrames, 15, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator3, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &self->animator4, true, 1);

    UIVsRoundPicker_Draw();
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
