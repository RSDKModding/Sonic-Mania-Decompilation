#include "SonicMania.h"

ObjectUIResPicker *UIResPicker;

void UIResPicker_Update(void)
{
    RSDK_THIS(UIResPicker);
    entity->touchPosStart.x = entity->size.x;
    entity->touchPosStart.y = entity->size.y;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x = (entity->touchPosStart.x + 3 * entity->size.y) >> 1;
    entity->touchPosStart.y = entity->size.y + 0x60000;
    entity->touchPosEnd.x   = -entity->touchPosStart.x;
    if (entity->selection != entity->prevSelection) {
        UIResPicker_GetDisplayInfo(entity);
        entity->prevSelection = entity->selection;
    }

    StateMachine_Run(entity->state);

    EntityUIControl *control = (EntityUIControl *)entity->parent;
    if (control && control->state == UIButton_Unknown16)
        UIResPicker_Unknown3(entity);
    entity->visible = true;
}

void UIResPicker_LateUpdate(void) {}

void UIResPicker_StaticUpdate(void) {}

void UIResPicker_Draw(void)
{
    RSDK_THIS(UIResPicker);
    Vector2 drawPos;

    int32 size  = (entity->size.y + entity->size.x) >> 16;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->offset;
    drawPos.y -= entity->offset;
#if RETRO_USE_PLUS
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->field_11C, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF,
                       (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->field_11C, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif

    drawPos = entity->position;
    drawPos.x += entity->offset;
    drawPos.y += entity->offset;
    UIWidgets_Unknown7((entity->size.y >> 16), size, entity->field_11C, 0, 0, 0, drawPos.x, drawPos.y);

    if (entity->arrowWidth > 0 && entity->flag) {
        drawPos = entity->position;
        drawPos.y += entity->field_120;
        drawPos.x -= entity->arrowWidth << 15;
        drawPos.x += entity->offset;
        drawPos.y += entity->offset;
        RSDK.DrawSprite(&entity->animator1, &drawPos, false);

        drawPos = entity->position;
        drawPos.y += entity->field_120;
        drawPos.x += entity->arrowWidth << 15;
        drawPos.x += entity->offset;
        drawPos.y += entity->offset;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }

    if (entity->textFlag) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y + entity->field_120 + entity->offset;

        switch (entity->align) {
            case 0: drawPos.x = -0x60000 - (entity->size.x >> 1) + drawPos.x; break;
            case 2: drawPos.x = drawPos.x + (entity->size.x >> 1) - 0x60000; break;
        }

        int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->text, 0, entity->text.textLength, 0);
        drawPos.x -= width << 15;
        RSDK.DrawText(&entity->textAnimator, &drawPos, &entity->text, 0, entity->text.textLength, ALIGN_LEFT, 0, 0, 0, false);
    }
}

void UIResPicker_Create(void *data)
{
    RSDK_THIS(UIResPicker);
    if (!RSDK_sceneInfo->inEditor) {
        entity->selection     = -2;
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x400000;
        entity->field_11C     = entity->size.y >> 16;
        entity->size.y        = abs(entity->size.y);
        entity->textFlag      = true;
        RSDK.SetText(&entity->text, "", 0x100);
        entity->processButtonCB = UIResPicker_ProcessButtonCB;
        entity->touchCB         = UIResPicker_ProcessTouchCB;
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->textAnimator, true, 0);
    }
}

void UIResPicker_StageLoad(void) { UIResPicker->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIResPicker_GetDisplayInfo(EntityUIResPicker *entity)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        TextInfo info;
        RSDK.GetDisplayInfo(&entity->selection, &entity->displayWidth, &entity->displayHeight, &entity->displayRefreshRate, &info);

        char buffer[0x20];
        RSDK.GetCString(buffer, &info);
        if (strcmp(buffer, "DEFAULT") != 0)
            RSDK.CopyString(&entity->text, &info);
        else
            Localization_GetString(&entity->text, STR_DEFAULT);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->text);
    }
}

void UIResPicker_ApplySettings(void)
{
    RSDK_THIS(UIResPicker);
    RSDK.PlaySfx(UIWidgets->sfx_Bleep, false, 255);
    entity->field_128 = -0x20000;
    entity->field_12C = -0x20000;
    entity->field_120 = 0;
    entity->offset    = -0x20000;
    UIResPicker_GetDisplayInfo(entity);

    RSDK.SetSettingsValue(SETTINGS_FSWIDTH, entity->displayWidth);
    RSDK.SetSettingsValue(SETTINGS_FSHEIGHT, entity->displayHeight);
    if (!entity->displayRefreshRate)
        RSDK.SetSettingsValue(SETTINGS_REFRESHRATE, 60);
    else
        RSDK.SetSettingsValue(SETTINGS_REFRESHRATE, entity->displayRefreshRate);
#if RETRO_USE_PLUS
    RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
#else
    RSDK.UpdateWindow();
#endif
}

void UIResPicker_ProcessButtonCB(void)
{
    RSDK_THIS(UIResPicker);

    int32 sel = entity->selection;
    if (UIControl->keyLeft)
        entity->selection--;
    if (UIControl->keyRight)
        ++entity->selection;

    if (sel != entity->selection)
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

    touchStart[0].x = entity->touchPosStart.x;
    touchStart[0].y = entity->touchPosStart.y;
    touchStart[1].x = entity->touchPosStart.x;
    touchStart[1].y = entity->touchPosStart.y;

    touchEnd[0].x = entity->touchPosEnd.x;
    touchEnd[0].y = entity->touchPosEnd.y;
    touchEnd[1].x = -entity->touchPosEnd.x;
    touchEnd[1].y = entity->touchPosEnd.y;

    bool32 pressed = false;
    for (int32 i = 0; i < 2; ++i) {
        if (RSDK_touchMouse->count) {
            int32 sizeX = touchStart[i].x >> 1;
            int32 sizeY = touchStart[i].y >> 1;
            for (int32 t = 0; t < RSDK_touchMouse->count; ++t) {
                int32 x = (RSDK_screens->position.x << 16) - ((RSDK_touchMouse->x[t] * RSDK_screens->width) * -65536.0f);
                int32 y = (RSDK_screens->position.y << 16) - ((RSDK_touchMouse->y[t] * RSDK_screens->height) * -65536.0f);

                int32 touchX = abs(touchEnd[i].x + entity->position.x - x);
                int32 touchY = abs(touchEnd[i].y + entity->position.y - y);
                if (touchX < sizeX && touchY < sizeY) {
                    entity->field_13C = i;
                    pressed           = true;
                }
            }
        }
        else if (entity->touchPressed && entity->field_13C == i && !entity->disabled) {
            callbacks[i]();
        }
    }

    entity->touchPressed = pressed;
    return entity->touchPressed;
}

void UIResPicker_TouchedCB_Left(void)
{
    RSDK_THIS(UIResPicker);
    entity->selection--;
    if (entity->selection != entity->selection - 1)
        UIResPicker_ApplySettings();
}

void UIResPicker_TouchedCB_Right(void)
{
    RSDK_THIS(UIResPicker);

    entity->selection++;
    if (entity->selection != entity->selection + 1)
        UIResPicker_ApplySettings();
}

void UIResPicker_Unknown3(EntityUIResPicker *entity)
{
    if (entity) {
        entity->field_120 = 0;
        entity->offset    = 0;
        entity->field_128 = 0;
        entity->field_12C = 0;
        entity->flag      = false;
        entity->state     = UIResPicker_Unknown7;
    }
}

void UIResPicker_Unknown7(void)
{
    RSDK_THIS(UIResPicker);

    entity->textFlag = true;
    if (entity->field_120) {
        int32 dist = -(entity->field_120 / abs(entity->field_120));
        entity->field_120 += dist << 15;
        if (dist < 0) {
            if (entity->field_120 < 0) {
                entity->field_120 = 0;
            }
            else if (dist > 0 && entity->field_120 > 0)
                entity->field_120 = 0;
        }
        else if (dist > 0 && entity->field_120 > 0)
            entity->field_120 = 0;
    }

    if (entity->offset) {
        int32 dist = -(entity->offset / abs(entity->offset));
        entity->offset += dist << 16;
        if (dist < 0) {
            if (entity->offset < 0) {
                entity->offset = 0;
            }
            else if (dist > 0 && entity->offset > 0)
                entity->offset = 0;
        }
        else if (dist > 0 && entity->offset > 0)
            entity->offset = 0;
    }
}

void UIResPicker_Unknown8(void)
{
    RSDK_THIS(UIResPicker);

    entity->field_128 += 0x4000;
    entity->field_120 += entity->field_128;
    entity->textFlag = true;
    if (entity->field_120 >= 0 && entity->field_128 > 0) {
        entity->field_120 = 0;
        entity->field_128 = 0;
    }

    entity->field_12C += 0x4800;
    entity->offset += entity->field_12C;
    if (entity->offset >= -0x20000 && entity->field_12C > 0) {
        entity->offset    = -0x20000;
        entity->field_12C = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIResPicker_EditorDraw(void) {}

void UIResPicker_EditorLoad(void) {}
#endif

void UIResPicker_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIResPicker, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIResPicker, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIResPicker, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIResPicker, VAR_ENUM, arrowWidth);
}
