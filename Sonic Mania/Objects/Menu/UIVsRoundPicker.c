#include "SonicMania.h"

ObjectUIVsRoundPicker *UIVsRoundPicker;

void UIVsRoundPicker_Update(void)
{
    RSDK_THIS(UIVsRoundPicker);
    entity->touchPosStart.x = entity->size.x;
    entity->touchPosStart.y = entity->size.y;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x = (entity->touchPosStart.x + 3 * entity->size.y) >> 1;
    entity->touchPosStart.y = entity->size.y + 0x60000;
    entity->touchPosEnd.x   = -(entity->touchPosStart.x >> 1);

    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &entity->animator1, true, 4);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
    if (entity->val != entity->prevVal) {
        UIVsRoundPicker_SetText();
        entity->prevVal = entity->val;
    }

    StateMachine_Run(entity->state);

    EntityUIControl *control = (EntityUIControl *)entity->parent;
    if (control) {
        if (control->state == UIButton_Unknown16)
            UIVsRoundPicker_Unknown4(entity);
    }
}

void UIVsRoundPicker_LateUpdate(void) {}

void UIVsRoundPicker_StaticUpdate(void) {}

void UIVsRoundPicker_Draw(void)
{
    RSDK_THIS(UIVsRoundPicker);
    Vector2 drawPos;

    int size  = (entity->size.y + entity->size.x) >> 16;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->field_128;
    drawPos.y -= entity->field_128;
    UIWidgets_Unknown7((entity->size.y >> 16), size, entity->dword120, (UIWidgets->value >> 16) & 0xFF, (UIWidgets->value >> 8) & 0xFF,
                       (UIWidgets->value & 0xFF), drawPos.x, drawPos.y);

    drawPos = entity->position;
    drawPos.x += entity->field_128;
    drawPos.y += entity->field_128;
    UIWidgets_Unknown7((entity->size.y >> 16), size, entity->dword120, 0, 0, 0, drawPos.x, drawPos.y);

    if (entity->arrowWidth > 0 && entity->flag) {
        if (entity->val >= 2) {
            drawPos = entity->position;
            drawPos.y += entity->field_124;
            drawPos.x -= entity->arrowWidth << 15;
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        }
        if (entity->val < entity->maxVal) {
            drawPos = entity->position;
            drawPos.y += entity->field_124;
            drawPos.x += entity->arrowWidth << 15;
            RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        }
    }
    if (entity->textFlag)
        UIVsRoundPicker_Unknown2();
}

void UIVsRoundPicker_Create(void *data)
{
    RSDK_THIS(UIVsRoundPicker);
    if (!entity->val)
        entity->val = 3;
    if (!entity->maxVal)
        entity->maxVal = 12;
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible         = true;
        entity->drawOrder       = 2;
        entity->active          = ACTIVE_BOUNDS;
        entity->updateRange.x   = 0x800000;
        entity->updateRange.y   = 0x400000;
        entity->dword120        = entity->size.y >> 16;
        entity->size.y          = abs(entity->size.y);
        entity->textFlag        = true;
        entity->processButtonCB = UIVsRoundPicker_ProcessButtonCB;
        entity->touchCB         = UIVsRoundPicker_ProcessTouchCB;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &entity->animator1, true, 4);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        RSDK.SetSpriteAnimation(UIVsRoundPicker->aniFrames, 15, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator4, true, 1);
        UIVsRoundPicker_SetText();
    }
}

void UIVsRoundPicker_StageLoad(void) { UIVsRoundPicker->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsRoundPicker_Unknown2(void)
{
    RSDK_THIS(UIVsRoundPicker);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y + entity->field_124 - 0x40000;
    switch (entity->align) {
        case 0: drawPos.x = -0x60000 - (entity->size.x >> 1) + entity->position.x; break;
        case 2: drawPos.x = entity->position.x + (entity->size.x >> 1) - 0x60000; break;
    }

    int width = RSDK.GetStringWidth(UIVsRoundPicker->aniFrames, 15, &entity->text, 0, entity->text.textLength, 0) << 16;
    drawPos.x -= (width + 0x300000) >> 1;
    RSDK.DrawText(&entity->animator2, &drawPos, &entity->text, 0, entity->text.textLength, ALIGN_LEFT, 0, 0, 0, false);

    drawPos.x += width + 0x40000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
}

void UIVsRoundPicker_SetText(void)
{
    RSDK_THIS(UIVsRoundPicker);
    char buffer[0x10];
    sprintf(buffer, "%2d", entity->val);
    RSDK.PrependText(&entity->text, buffer);
    RSDK.SetSpriteString(UIVsRoundPicker->aniFrames, 15, &entity->text);
}

void UIVsRoundPicker_Apply(void)
{
    RSDK_THIS(UIVsRoundPicker);

    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);

    entity->field_12C = -0x20000;
    entity->field_130 = -0x20000;
    entity->field_124 = 0;
    entity->field_128 = -0x20000;
    UIVsRoundPicker_SetText();
}

void UIVsRoundPicker_ProcessButtonCB(void)
{
    RSDK_THIS(UIVsRoundPicker);

    int sel = entity->val;
    if (UIControl->keyLeft && entity->val > 1)
        entity->val--;
    if (UIControl->keyRight && entity->val < entity->maxVal)
        ++entity->val;

    if (sel != entity->val)
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

    touchStart[0].x = entity->touchPosStart.x;
    touchStart[0].y = entity->touchPosStart.y;
    touchStart[1].x = entity->touchPosStart.x;
    touchStart[1].y = entity->touchPosStart.y;

    touchEnd[0].x = entity->touchPosEnd.x;
    touchEnd[0].y = entity->touchPosEnd.y;
    touchEnd[1].x = -entity->touchPosEnd.x;
    touchEnd[1].y = entity->touchPosEnd.y;

    bool32 pressed = false;
    for (int i = 0; i < 2; ++i) {
        if (RSDK_touchMouse->count) {
            int sizeX = touchStart[i].x >> 1;
            int sizeY = touchStart[i].y >> 1;
            for (int t = 0; t < RSDK_touchMouse->count; ++t) {
                int x = (RSDK_screens->position.x << 16) - ((RSDK_touchMouse->x[t] * RSDK_screens->width) * -65536.0f);
                int y = (RSDK_screens->position.y << 16) - ((RSDK_touchMouse->y[t] * RSDK_screens->height) * -65536.0f);

                int touchX = abs(touchEnd[i].x + entity->position.x - x);
                int touchY = abs(touchEnd[i].y + entity->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    entity->touchID = i;
                    pressed         = true;
                }
            }
        }
        else if (entity->touchPressed && entity->touchID == i && !entity->disabled) {
            callbacks[i]();
        }
    }

    entity->touchPressed = pressed;
    return entity->touchPressed;
}

void UIVsRoundPicker_TouchedCB_Left(void)
{
    RSDK_THIS(UIVsRoundPicker);

    if (entity->val > 1) {
        entity->val--;
        UIVsRoundPicker_Apply();
    }
}

void UIVsRoundPicker_TouchedCB_Right(void)
{
    RSDK_THIS(UIVsRoundPicker);

    if (entity->val < entity->maxVal) {
        entity->val++;
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

    entity->textFlag = true;
    if (entity->field_124) {
        int dist = -(entity->field_124 / abs(entity->field_124));
        entity->field_124 += dist << 15;
        if (dist < 0) {
            if (entity->field_124 < 0) {
                entity->field_124 = 0;
            }
            else if (dist > 0 && entity->field_124 > 0)
                entity->field_124 = 0;
        }
        else if (dist > 0 && entity->field_124 > 0)
            entity->field_124 = 0;
    }

    if (entity->field_128) {
        int dist = -(entity->field_128 / abs(entity->field_128));
        entity->field_128 += dist << 16;
        if (dist < 0) {
            if (entity->field_128 < 0) {
                entity->field_128 = 0;
            }
            else if (dist > 0 && entity->field_128 > 0)
                entity->field_128 = 0;
        }
        else if (dist > 0 && entity->field_128 > 0)
            entity->field_128 = 0;
    }
}

void UIVsRoundPicker_Unknown9(void)
{
    RSDK_THIS(UIVsRoundPicker);

    entity->field_12C += 0x4000;
    entity->field_124 += entity->field_12C;
    entity->textFlag = true;
    if (entity->field_124 >= 0 && entity->field_12C > 0) {
        entity->field_124 = 0;
        entity->field_12C = 0;
    }

    entity->field_130 += 0x4800;
    entity->field_128 += entity->field_130;
    if (entity->field_128 >= -0x20000 && entity->field_130 > 0) {
        entity->field_128 = -0x20000;
        entity->field_130 = 0;
    }
}

void UIVsRoundPicker_EditorDraw(void) {}

void UIVsRoundPicker_EditorLoad(void) {}

void UIVsRoundPicker_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, arrowWidth);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, val);
    RSDK_EDITABLE_VAR(UIVsRoundPicker, VAR_ENUM, maxVal);
}
