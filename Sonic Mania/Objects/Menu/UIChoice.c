#include "SonicMania.h"

ObjectUIChoice *UIChoice;

void UIChoice_Update(void)
{
    RSDK_THIS(UIChoice);
    EntityUIButton *parent  = (EntityUIButton *)entity->parent;
    bool32 disabled         = entity->disabled;
    entity->touchPosStart.x = entity->size.x;
    entity->touchPosStart.y = entity->size.y;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x = (entity->touchPosStart.x + 3 * entity->size.y) >> 1;
    entity->touchPosStart.y = entity->size.y + 0x60000;
    entity->touchPosEnd.x   = -(entity->touchPosStart.x >> 1);
    if (parent && (disabled || parent->disabled))
        disabled = true;

    if (entity->spriteIndex != UIWidgets->textSpriteIndex || entity->isDisabled != disabled) {
        if (disabled)
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &entity->animator1, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator1, true, entity->frameID);
        entity->spriteIndex = UIWidgets->textSpriteIndex;
        entity->isDisabled  = disabled;
    }

    StateMachine_Run(entity->state);

    if (parent) {
        if (parent->state == UIButton_Unknown16) {
            entity->field_134.x = 0;
            entity->field_134.y = 0;
            entity->flag        = false;
            entity->state       = UIChoice_Unknown6;
        }
    }
}

void UIChoice_LateUpdate(void) {}

void UIChoice_StaticUpdate(void) {}

void UIChoice_Draw(void)
{
    RSDK_THIS(UIChoice);
    Vector2 drawPos;

    int32 size  = entity->size.y + entity->size.x;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->field_134.y;
    drawPos.y -= entity->field_134.y;
    size >>= 16;
#if RETRO_USE_PLUS
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->sizeIY, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF,
                       (UIWidgets->buttonColour) & 0xFF, drawPos.x, drawPos.y);
#else
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->sizeIY, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
#endif

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->field_134.y;
    drawPos.y += entity->field_134.y;
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->sizeIY, 0, 0, 0, drawPos.x, drawPos.y);

    EntityUIButton *parent = (EntityUIButton *)entity->parent;
    if (entity->arrowWidth > 0 && entity->flag && !(entity->disabled || parent->disabled)) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += entity->field_134.x;
        drawPos.x -= entity->arrowWidth << 15;
        drawPos.x += entity->field_134.y;
        drawPos.y += entity->field_134.y;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y += entity->field_134.x;
        drawPos.x += entity->arrowWidth << 15;
        drawPos.x += entity->field_134.y;
        drawPos.y += entity->field_134.y;
        RSDK.DrawSprite(&entity->animator4, &drawPos, false);
    }

    if (entity->textFlag) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.y = entity->field_134.x + entity->position.y;
        drawPos.x = entity->field_134.y + entity->position.x;
        drawPos.y += entity->field_134.y;
        if (entity->align) {
            if (entity->align == 2)
                drawPos.x += (entity->size.x >> 1) - 0x60000;
        }
        else {
            drawPos.x += -0x60000 - (entity->size.x >> 1);
        }
        if (entity->auxIcon)
            RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        if (!entity->noText)
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    }
}

void UIChoice_Create(void *data)
{
    RSDK_THIS(UIChoice);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = true;
        entity->drawOrder     = 2;
        entity->active        = ACTIVE_NEVER;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x400000;
        entity->sizeIY        = entity->size.y >> 16;
        entity->size.y        = abs(entity->size.y);
        entity->textFlag      = true;
        entity->touchCB       = UIChoice_CheckTouch;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator1, true, entity->frameID);
        entity->spriteIndex = UIWidgets->textSpriteIndex;
        RSDK.SetSpriteAnimation(UIChoice->aniFrames, entity->auxListID, &entity->animator2, true, entity->auxFrameID);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 2, &entity->animator4, true, 1);
    }
}

void UIChoice_StageLoad(void) { UIChoice->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIChoice_Unknown1(EntityUIChoice *entity)
{
    if (entity) {
        EntityUIButton *parent = (EntityUIButton *)entity->parent;
        entity->active         = ACTIVE_BOUNDS;
        if (!parent->disabled)
            entity->visible = true;
        entity->field_134.x = 0;
        entity->field_134.y = 0;
        entity->field_13C   = -0x20000;
        entity->field_140   = -0x20000;
        entity->flag        = true;
        entity->state       = UIChoice_Unknown7;
    }
}

void UIChoice_Unknown2(EntityUIButton *entity)
{
    if (entity) {
        entity->isDisabled = false;
        entity->dword138   = 0;
        entity->flag       = false;
        entity->state      = UIChoice_Unknown6;
    }
}

void UIChoice_TouchedCB_Left(void)
{
    RSDK_THIS(UIChoice);
    EntityUIButton *parent = (EntityUIButton *)entity->parent;
    int32 id                 = parent->selection;
    EntityUIButton *ent    = NULL;

    do {
        if (--id < 0) {
            do
                id += parent->choiceCount;
            while (id < 0);
        }
        if (parent->choiceCount > 0) {
            Entity *ent = RSDK.GetEntityByID(id % parent->choiceCount - parent->choiceCount + RSDK.GetEntityID(parent));
            if (ent->objectID == UIChoice->objectID || ent->objectID == UIVsRoundPicker->objectID || ent->objectID == UIResPicker->objectID
                || ent->objectID == UIWinSize->objectID)
                continue;
        }
        ent = 0;
    } while ((ent && ent->disabled) && id != parent->selection);

    if (id != parent->selection) {
        UIButton_SetChoiceSelectionWithCB(parent, id);
        RSDK.PlaySfx(UIWidgets->sfx_Bleep, 0, 255);
    }
}

void UIChoice_TouchedCB_Right(void)
{
    RSDK_THIS(UIChoice);
    EntityUIButton *parent = (EntityUIButton *)entity->parent;
    int32 id                 = parent->selection;
    EntityUIButton *ent    = NULL;
    do {
        id = (id + 1) % parent->choiceCount;
        if (parent->choiceCount > 0) {
            Entity *ent = RSDK.GetEntityByID(id % parent->choiceCount - parent->choiceCount + RSDK.GetEntityID(parent));
            if (ent->objectID == UIChoice->objectID || ent->objectID == UIVsRoundPicker->objectID || ent->objectID == UIResPicker->objectID
                || ent->objectID == UIWinSize->objectID)
                continue;
        }
        ent = NULL;
    } while ((ent && ent->disabled) && id != parent->selection);

    if (id != parent->selection) {
        UIButton_SetChoiceSelectionWithCB(parent, id);
        RSDK.PlaySfx(UIWidgets->sfx_Bleep, 0, 255);
    }
}

bool32 UIChoice_CheckTouch(void)
{
    void (*callbacks[2])(void);
    Vector2 touchStart[2];
    Vector2 touchEnd[2];

    RSDK_THIS(UIChoice);
    callbacks[0]  = UIChoice_TouchedCB_Left;
    callbacks[1] = UIChoice_TouchedCB_Right;

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
    return pressed;
}

void UIChoice_Unknown6(void)
{
    RSDK_THIS(UIChoice);

    if (entity->field_134.x) {
        int32 val = -(entity->field_134.x / abs(entity->field_134.x));
        entity->field_134.x += val << 15;
        if (val < 0 && entity->field_134.x < 0) {
            entity->field_134.x = 0;
        }
        else if (val > 0 && entity->field_134.x > 0) {
            entity->field_134.x = 0;
        }
    }

    if (entity->field_134.y) {
        int32 val = -(entity->field_134.y / abs(entity->field_134.y));
        entity->field_134.y += val << 16;
        if (val < 0 && entity->field_134.y < 0) {
            entity->field_134.y = 0;
        }
        else if (val > 0 && entity->field_134.y > 0) {
            entity->field_134.y = 0;
        }
    }
}

void UIChoice_Unknown7(void)
{
    RSDK_THIS(UIChoice);
    entity->field_13C += 0x4000;
    entity->field_134.x += entity->field_13C;
    if (entity->field_134.x >= 0 && entity->field_13C > 0) {
        entity->field_134.x = 0;
        entity->field_13C   = 0;
    }

    entity->field_140 += 0x4800;
    entity->field_134.y += entity->field_140;
    if (entity->field_134.y >= -0x20000 && entity->field_140 > 0) {
        entity->field_134.y = -0x20000;
        entity->field_140   = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIChoice_EditorDraw(void) {}

void UIChoice_EditorLoad(void) {}
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
