#include "SonicMania.h"

ObjectUISlider *UISlider;

void UISlider_Update(void)
{
    RSDK_THIS(UISlider);

    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->textAnimator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }

    entity->touchPosEnd.y = entity->field_11C;
    entity->touchPosEnd.x = 0x7A0000 + entity->field_11C;

    StateMachine_Run(entity->state);

    EntityUIControl *control = (EntityUIControl *)entity->parent;

    int32 id = -1;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (entity == (EntityUISlider *)control->buttons[i]) {
            id = i;
            break;
        }
    }

    if (entity->state == UISlider_Unknown10 && (control->state != UIControl_ProcessInputs || control->activeEntityID != id)) {
        entity->flag      = false;
        entity->field_118 = 0;
        entity->field_11C = 0;
        if (entity->flag) {
            entity->flag  = false;
            entity->state = UISlider_Unknown9;
        }
    }
}

void UISlider_LateUpdate(void) {}

void UISlider_StaticUpdate(void) {}

void UISlider_Draw(void)
{
    UISlider_Unknown1();
    UISlider_Unknown2();
}

void UISlider_Create(void *data)
{
    RSDK_THIS(UISlider);
    entity->visible         = true;
    entity->drawOrder       = 2;
    entity->active          = ACTIVE_BOUNDS;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x400000;
    entity->touchPosEnd.y   = 0;
    entity->touchPosEnd.x   = 0x7A0000;
    entity->touchPosStart.x = 0x940000;
    entity->touchPosStart.y = 0x180000;
    entity->field_10C       = 0x5A0000;
    entity->field_114       = 24;
    entity->field_110       = 0x180000;
    entity->processButtonCB = UISlider_ButtonPressCB;
    entity->touchCB         = UISlider_TouchCB;
    entity->options3        = 0;
    entity->failCB          = 0;
    entity->options5        = UISlider_Options5CB;
    entity->options6        = UISlider_Options6CB;
    entity->options7        = UISlider_Options7CB;
    entity->options8        = UISlider_Options8CB;
    entity->textFlag        = true;
    entity->sliderPos       = (UISlider_MaxVal - UISlider_MinVal) / 2;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->textAnimator, true, entity->frameID);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
}

void UISlider_StageLoad(void) {}

void UISlider_Unknown1(void)
{
    RSDK_THIS(UISlider);
    Vector2 drawPos;

    int32 size  = entity->field_110 + entity->field_10C;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    UIWidgets_Unknown7((entity->field_110 >> 16), size >> 16, entity->field_114, 0, 0, 0, entity->position.x, entity->position.y);

    if (entity->textFlag) {
        drawPos = entity->position;
        drawPos.y += entity->field_118;
        drawPos.x += -0x60000 - (entity->field_10C >> 1);
        RSDK.DrawSprite(&entity->textAnimator, &drawPos, false);
    }
}

void UISlider_Unknown2(void)
{
    RSDK_THIS(UISlider);
    Vector2 drawPos;

    int32 drawX = 0x7A0000 + entity->position.x;
    int32 sliderPos        = (((34048 * entity->sliderPos) >> 2) & 0xFFFFFF00) - 0xB0000;
    int32 drawX2            = drawX - ((0x7A0000 - sliderPos) >> 1);
    UIWidgets_Unknown7(12, 122, 12, 240, 240, 240, drawX - entity->field_11C, entity->position.y - entity->field_11C);
    UIWidgets_Unknown7(12, 122, 12, 0, 0, 0, entity->field_11C + drawX, entity->field_11C + entity->position.y);
    UIWidgets_Unknown7(12, sliderPos >> 16, 12, 232, 40, 88, entity->field_11C + drawX2, entity->field_11C + entity->position.y);

    drawPos.x = drawX2 + (sliderPos >> 1) + entity->field_11C + 0x60000;
    drawPos.y = entity->field_11C + entity->position.y;
    if (entity->flag) {
        drawPos.x += 0x30000;
        drawPos.y += 0x30000;
        UIWidgets_Unknown3(24, 14, drawPos.x, drawPos.y);
        drawPos.x -= 0x30000;
        drawPos.y -= 0x30000;
    }

    RSDK.DrawRect(drawPos.x - 0x70000, drawPos.y - 0xC0000, 0xE0000, 0x180000, 0xF0F0F0, 255, INK_NONE, false);
    if (entity->flag)
        UIWidgets_Unknown4(24, 14, drawPos.x, drawPos.y);
}

void UISlider_ButtonPressCB(void)
{
    RSDK_THIS(UISlider);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    int32 rowID = 0;
    if (parent->rowCount && parent->columnCount)
        rowID = parent->activeEntityID / parent->columnCount;
    else
        rowID = 0;

    int32 columnID = 0;
    if (parent->columnCount)
        columnID = parent->activeEntityID % parent->columnCount;
    else
        columnID = 0;

    bool32 moveV = false;
    if (parent->rowCount > 1) {
        if (UIControl->keyUp) {
            --rowID;
            moveV = 1;
        }
        if (UIControl->keyDown) {
            ++rowID;
            moveV = 1;
        }
    }

    bool32 valueChanged = false;
    if (UIControl->keyLeft && entity->sliderPos > UISlider_MinVal) {
        entity->sliderPos = (entity->sliderPos & -UISlider_Increment) - UISlider_Increment;
        valueChanged        = true;
    }
    if (UIControl->keyRight && entity->sliderPos < UISlider_MaxVal) {
        entity->sliderPos = (entity->sliderPos & -UISlider_Increment) + UISlider_Increment;
        valueChanged        = true;
    }

    if (valueChanged) {
        RSDK.PlaySfx(UIWidgets->sfx_Bleep, false, 255);
        StateMachine_Run(entity->sliderChangedCB);
    }

    if (moveV) {
        if (rowID < 0)
            rowID += parent->rowCount;
        if (rowID >= parent->rowCount)
            rowID -= parent->rowCount;

        int32 max = parent->buttonCount - 1;
        if (rowID * parent->columnCount + columnID < max)
            max = rowID * parent->columnCount + columnID;

        if (parent->activeEntityID != max) {
            parent->activeEntityID = max;
            if (entity->flag) {
                entity->flag  = false;
                entity->state = UISlider_Unknown9;
            }
            RSDK.PlaySfx(UIWidgets->sfx_Bleep, false, 255);
        }
    }
    else {
        int32 id = -1;
        for (int32 i = 0; i < parent->buttonCount; ++i) {
            if (entity == (EntityUISlider *)parent->buttons[i]) {
                id = i;
                break;
            }
        }

        if (!entity->flag && parent->activeEntityID == id && parent->state == UIControl_ProcessInputs && !parent->dialogHasFocus) {
            UISlider_Options5CB();
        }
    }
}

bool32 UISlider_TouchCB(void)
{
    RSDK_THIS(UISlider);

    bool32 touchFlag = false;
    if (RSDK_touchMouse->count) {
        if (!UISlider->entityPtr || UISlider->entityPtr == (Entity *)entity) {
            int32 sizeX = entity->touchPosStart.x >> 1;
            int32 sizeY = entity->touchPosStart.y >> 1;
            for (int32 i = 0; i < RSDK_touchMouse->count; ++i) {
                int32 x = (RSDK_screens->position.x << 16) - ((RSDK_touchMouse->x[i] * RSDK_screens->width) * -65536.0f);
                int32 y = (RSDK_screens->position.y << 16) - ((RSDK_touchMouse->y[i] * RSDK_screens->height) * -65536.0f);

                int32 touchX = abs(entity->touchPosEnd.x + entity->position.x - x);
                int32 touchY = abs(entity->touchPosEnd.y + entity->position.y - y);
                if (!entity->flagB && touchX < sizeX && touchY < sizeY) {
                    entity->flagB = true;
                    RSDK.PlaySfx(UIWidgets->sfx_Bleep, false, 255);
                }

                if (entity->flagB) {
                    touchFlag           = true;
                    UISlider->entityPtr = (Entity *)entity;

                    entity->field_134 = maxVal(x + sizeX - entity->position.x - entity->touchPosEnd.x, 0x70000);
                    if (entity->touchPosStart.x - 0x70000 < entity->field_134)
                        entity->field_134 = entity->touchPosStart.x - 0x70000;

                    int32 val = 16 * (minVal(((entity->field_134 - 0x70000) >> 4 << 10) / (entity->touchPosStart.x - 0xE0000) + 2, UISlider_MaxVal) & -(UISlider_Increment / 0x10));
                    if (val != entity->sliderPos) {
                        entity->sliderPos = val;
                        StateMachine_Run(entity->sliderChangedCB);
                    }
                }
            }

            entity->touchPressed = touchFlag;
            return touchFlag;
        }
    }
    else {
        UISlider->entityPtr = NULL;
    }
    if (entity->touchPressed)
        RSDK.PlaySfx(UIWidgets->sfx_Bleep, false, 255);
    entity->flagB        = false;
    entity->touchPressed = false;
    return false;
}

void UISlider_Options5CB(void)
{
    RSDK_THIS(UISlider);
    if (!entity->flag) {
        entity->flag      = true;
        entity->field_118 = 0;
        entity->field_11C = 0;
        entity->field_120 = -0x20000;
        entity->field_124 = -0x20000;
        entity->state     = UISlider_Unknown10;
    }
}

void UISlider_Options6CB(void)
{
    RSDK_THIS(UISlider);
    if (entity->flag) {
        entity->flag  = false;
        entity->state = UISlider_Unknown9;
    }
}

bool32 UISlider_Options7CB(void)
{
    RSDK_THIS(UISlider);
    return entity->flag;
}

bool32 UISlider_Options8CB(void) { return false; }

void UISlider_Unknown9(void)
{
    RSDK_THIS(UISlider);

    if (entity->field_118) {
        int32 dist = -(entity->field_118 / abs(entity->field_118));
        entity->field_118 += dist << 15;
        if (dist < 0) {
            if (entity->field_118 < 0) {
                entity->field_118 = 0;
            }
            else if (dist > 0 && entity->field_118 > 0)
                entity->field_118 = 0;
        }
        else if (dist > 0 && entity->field_118 > 0)
            entity->field_118 = 0;
    }

    if (entity->field_11C) {
        int32 dist = -(entity->field_11C / abs(entity->field_11C));
        entity->field_11C += dist << 16;
        if (dist < 0) {
            if (entity->field_11C < 0) {
                entity->field_11C = 0;
            }
            else if (dist > 0 && entity->field_11C > 0)
                entity->field_11C = 0;
        }
        else if (dist > 0 && entity->field_11C > 0)
            entity->field_11C = 0;
    }
}

void UISlider_Unknown10(void)
{
    RSDK_THIS(UISlider);

    entity->field_120 += 0x4000;
    entity->field_118 += entity->field_120;
    if (entity->field_118 >= 0 && entity->field_120 > 0) {
        entity->field_118 = 0;
        entity->field_120 = 0;
    }

    entity->field_124 += 0x4800;
    entity->field_11C += entity->field_124;
    if (entity->field_11C >= -0x20000 && entity->field_124 > 0) {
        entity->field_11C = -0x20000;
        entity->field_124 = 0;
    }
}

void UISlider_EditorDraw(void) {}

void UISlider_EditorLoad(void) {}

void UISlider_Serialize(void)
{
    RSDK_EDITABLE_VAR(UISlider, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UISlider, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UISlider, VAR_ENUM, frameID);
}
