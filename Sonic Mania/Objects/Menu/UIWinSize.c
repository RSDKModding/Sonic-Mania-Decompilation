#include "SonicMania.h"

ObjectUIWinSize *UIWinSize;

void UIWinSize_Update(void)
{
    RSDK_THIS(UIWinSize);
    entity->touchPosStart.x = entity->size.x;
    entity->touchPosStart.y = entity->size.y;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x = (entity->touchPosStart.x + 3 * entity->size.y) >> 1;
    entity->touchPosStart.y = entity->size.y + 0x60000;
    entity->touchPosEnd.x   = -entity->touchPosStart.x;
    if (entity->selection != entity->prevSelection) {
        UIWinSize_SetupText(entity);
        entity->prevSelection = entity->selection;
    }

    StateMachine_Run(entity->state);

    EntityUIControl *control = (EntityUIControl *)entity->parent;
    if (control && control->state == UIButton_Unknown16)
        UIWinSize_Unknown3(entity);
    entity->visible = true;
}

void UIWinSize_LateUpdate(void) {}

void UIWinSize_StaticUpdate(void) {}

void UIWinSize_Draw(void)
{
    RSDK_THIS(UIWinSize);
    Vector2 drawPos;

    int size  = (entity->size.y + entity->size.x) >> 16;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->offset;
    drawPos.y -= entity->offset;
    UIWidgets_Unknown7((entity->size.y >> 16), size, entity->height, (UIWidgets->value >> 16) & 0xFF, (UIWidgets->value >> 8) & 0xFF,
                       (UIWidgets->value & 0xFF), drawPos.x, drawPos.y);

    drawPos = entity->position;
    drawPos.x += entity->offset;
    drawPos.y += entity->offset;
    UIWidgets_Unknown7((entity->size.y >> 16), size, entity->height, 0, 0, 0, drawPos.x, drawPos.y);

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

        drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->text, 0, entity->text.textLength, 0) << 15;
        RSDK.DrawText(&entity->textAnimator, &drawPos, &entity->text, 0, entity->text.textLength, ALIGN_LEFT, 0, 0, 0, false);
    }
}

void UIWinSize_Create(void *data)
{
    RSDK_THIS(UIWinSize);
    if (!RSDK_sceneInfo->inEditor) {
        int winHeight         = RSDK.GetSettingsValue(SETTINGS_WINDOW_HEIGHT);
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x400000;
        entity->selection     = winHeight / 240;
        entity->height        = entity->size.y >> 16;
        entity->size.y        = abs(entity->size.y);
        entity->textFlag      = true;
        RSDK.SetText(&entity->text, "", 0x100);
        entity->processButtonCB = UIWinSize_ProcessButtonCB;
        entity->touchCB         = UIWinSize_ProcessTouchCB;
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator2, true, 1);
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->textAnimator, true, 0);
    }
}

void UIWinSize_StageLoad(void) { UIWinSize->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIWinSize_SetupText(EntityUIWinSize *entityPtr)
{
    RSDK_THIS(UIWinSize);
    if (RSDK_sku->platform == PLATFORM_PC || RSDK_sku->platform == PLATFORM_DEV) {
        int height = 0;
        RSDK.GetWindowSize(NULL, &height);
        entity->maxScale = height / 240;
        if (entity->selection < 1)
            entity->selection = entity->maxScale - 1;
        if (entity->selection >= entity->maxScale)
            entity->selection = 1;

        char buffer[0x10];
        sprintf(buffer, "%ix", entity->selection);
        RSDK.PrependText(&entityPtr->text, buffer);
#if RETRO_GAMEVER != VER_100
        if (Localization->language == LANGUAGE_TC) {
            for (int c = 0; c < entityPtr->text.textLength; ++c) {
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
    RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    entity->field_128 = -0x20000;
    entity->field_12C = -0x20000;
    entity->field_120 = 0;
    entity->offset    = -0x20000;
    UIWinSize_SetupText(entity);

    RSDK.SetSettingsValue(SETTINGS_WINDOW_WIDTH, entity->selection * RSDK_screens->width);
    RSDK.SetSettingsValue(SETTINGS_WINDOW_HEIGHT, entity->selection * RSDK_screens->height);
    RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
}

void UIWinSize_ProcessButtonCB(void)
{
    RSDK_THIS(UIWinSize);

    int sel = entity->selection;
    if (UIControl->keyLeft)
        entity->selection--;
    if (UIControl->keyRight)
        ++entity->selection;

    if (sel != entity->selection)
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
            int sizeX = entity->touchPosStart.x >> 1;
            int sizeY = entity->touchPosStart.y >> 1;
            for (int t = 0; t < RSDK_touchMouse->count; ++t) {
                int x = (RSDK_screens->position.x << 16) - ((RSDK_touchMouse->x[i] * RSDK_screens->width) * -65536.0f);
                int y = (RSDK_screens->position.y << 16) - ((RSDK_touchMouse->y[i] * RSDK_screens->height) * -65536.0f);

                int touchX = abs(touchEnd[i].x + entity->position.x - x);
                int touchY = abs(touchEnd[i].y + entity->position.y - y);
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

void UIWinSize_TouchedCB_Left(void)
{
    RSDK_THIS(UIWinSize);
    entity->selection--;
    if (entity->selection != entity->selection - 1)
        UIWinSize_ApplySettings();
}

void UIWinSize_TouchedCB_Right(void)
{
    RSDK_THIS(UIWinSize);

    entity->selection++;
    if (entity->selection != entity->selection + 1)
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

    entity->textFlag = true;
    if (entity->field_120) {
        int dist = -(entity->field_120 / abs(entity->field_120));
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
        int dist = -(entity->offset / abs(entity->offset));
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

void UIWinSize_Unknown8(void)
{
    RSDK_THIS(UIWinSize);

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

void UIWinSize_EditorDraw(void) {}

void UIWinSize_EditorLoad(void) {}

void UIWinSize_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIWinSize, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIWinSize, VAR_ENUM, arrowWidth);
}
