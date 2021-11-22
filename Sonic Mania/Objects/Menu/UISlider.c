#include "SonicMania.h"

ObjectUISlider *UISlider;

void UISlider_Update(void)
{
    RSDK_THIS(UISlider);

    if (self->textSpriteIndex != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->textAnimator, true, self->frameID);
        self->textSpriteIndex = UIWidgets->textSpriteIndex;
    }

    self->touchPosEnd.y = self->field_11C;
    self->touchPosEnd.x = 0x7A0000 + self->field_11C;

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;

    int32 id = -1;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (self == (EntityUISlider *)control->buttons[i]) {
            id = i;
            break;
        }
    }

    if (self->state == UISlider_Unknown10 && (control->state != UIControl_ProcessInputs || control->activeEntityID != id)) {
        self->flag      = false;
        self->field_118 = 0;
        self->field_11C = 0;
        if (self->flag) {
            self->flag  = false;
            self->state = UISlider_Unknown9;
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
    self->visible         = true;
    self->drawOrder       = 2;
    self->active          = ACTIVE_BOUNDS;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x400000;
    self->touchPosEnd.y   = 0;
    self->touchPosEnd.x   = 0x7A0000;
    self->touchPosStart.x = 0x940000;
    self->touchPosStart.y = 0x180000;
    self->field_10C       = 0x5A0000;
    self->field_114       = 24;
    self->field_110       = 0x180000;
    self->processButtonCB = UISlider_ButtonPressCB;
    self->touchCB         = UISlider_TouchCB;
    self->options3        = 0;
    self->failCB          = 0;
    self->options5        = UISlider_Options5CB;
    self->options6        = UISlider_Options6CB;
    self->options7        = UISlider_Options7CB;
    self->options8        = UISlider_Options8CB;
    self->textFlag        = true;
    self->sliderPos       = (UISlider_MaxVal - UISlider_MinVal) / 2;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->textAnimator, true, self->frameID);
    self->textSpriteIndex = UIWidgets->textSpriteIndex;
}

void UISlider_StageLoad(void) {}

void UISlider_Unknown1(void)
{
    RSDK_THIS(UISlider);
    Vector2 drawPos;

    int32 size  = self->field_110 + self->field_10C;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    UIWidgets_DrawParallelogram((self->field_110 >> 16), size >> 16, self->field_114, 0, 0, 0, self->position.x, self->position.y);

    if (self->textFlag) {
        drawPos = self->position;
        drawPos.y += self->field_118;
        drawPos.x += -0x60000 - (self->field_10C >> 1);
        RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
    }
}

void UISlider_Unknown2(void)
{
    RSDK_THIS(UISlider);
    Vector2 drawPos;

    int32 drawX = 0x7A0000 + self->position.x;
    int32 sliderPos        = (((34048 * self->sliderPos) >> 2) & 0xFFFFFF00) - 0xB0000;
    int32 drawX2            = drawX - ((0x7A0000 - sliderPos) >> 1);
    UIWidgets_DrawParallelogram(12, 122, 12, 240, 240, 240, drawX - self->field_11C, self->position.y - self->field_11C);
    UIWidgets_DrawParallelogram(12, 122, 12, 0, 0, 0, self->field_11C + drawX, self->field_11C + self->position.y);
    UIWidgets_DrawParallelogram(12, sliderPos >> 16, 12, 232, 40, 88, self->field_11C + drawX2, self->field_11C + self->position.y);

    drawPos.x = drawX2 + (sliderPos >> 1) + self->field_11C + 0x60000;
    drawPos.y = self->field_11C + self->position.y;
    if (self->flag) {
        drawPos.x += 0x30000;
        drawPos.y += 0x30000;
        UIWidgets_DrawRectOutline_Blended(24, 14, drawPos.x, drawPos.y);
        drawPos.x -= 0x30000;
        drawPos.y -= 0x30000;
    }

    RSDK.DrawRect(drawPos.x - 0x70000, drawPos.y - 0xC0000, 0xE0000, 0x180000, 0xF0F0F0, 255, INK_NONE, false);
    if (self->flag)
        UIWidgets_DrawRectOutline_Flash(24, 14, drawPos.x, drawPos.y);
}

void UISlider_ButtonPressCB(void)
{
    RSDK_THIS(UISlider);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

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
    if (UIControl->keyLeft && self->sliderPos > UISlider_MinVal) {
        self->sliderPos = (self->sliderPos & -UISlider_Increment) - UISlider_Increment;
        valueChanged        = true;
    }
    if (UIControl->keyRight && self->sliderPos < UISlider_MaxVal) {
        self->sliderPos = (self->sliderPos & -UISlider_Increment) + UISlider_Increment;
        valueChanged        = true;
    }

    if (valueChanged) {
        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        StateMachine_Run(self->sliderChangedCB);
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
            if (self->flag) {
                self->flag  = false;
                self->state = UISlider_Unknown9;
            }
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
    }
    else {
        int32 id = -1;
        for (int32 i = 0; i < parent->buttonCount; ++i) {
            if (self == (EntityUISlider *)parent->buttons[i]) {
                id = i;
                break;
            }
        }

        if (!self->flag && parent->activeEntityID == id && parent->state == UIControl_ProcessInputs && !parent->dialogHasFocus) {
            UISlider_Options5CB();
        }
    }
}

bool32 UISlider_TouchCB(void)
{
    RSDK_THIS(UISlider);

    bool32 touchFlag = false;
    if (TouchInfo->count) {
        if (!UISlider->entityPtr || UISlider->entityPtr == (Entity *)self) {
            int32 sizeX = self->touchPosStart.x >> 1;
            int32 sizeY = self->touchPosStart.y >> 1;
            for (int32 i = 0; i < TouchInfo->count; ++i) {
                int32 x = (ScreenInfo->position.x << 16) - ((TouchInfo->x[i] * ScreenInfo->width) * -65536.0f);
                int32 y = (ScreenInfo->position.y << 16) - ((TouchInfo->y[i] * ScreenInfo->height) * -65536.0f);

                int32 touchX = abs(self->touchPosEnd.x + self->position.x - x);
                int32 touchY = abs(self->touchPosEnd.y + self->position.y - y);
                if (!self->flagB && touchX < sizeX && touchY < sizeY) {
                    self->flagB = true;
                    RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
                }

                if (self->flagB) {
                    touchFlag           = true;
                    UISlider->entityPtr = (Entity *)self;

                    self->field_134 = maxVal(x + sizeX - self->position.x - self->touchPosEnd.x, 0x70000);
                    if (self->touchPosStart.x - 0x70000 < self->field_134)
                        self->field_134 = self->touchPosStart.x - 0x70000;

                    int32 val = 16 * (minVal(((self->field_134 - 0x70000) >> 4 << 10) / (self->touchPosStart.x - 0xE0000) + 2, UISlider_MaxVal) & -(UISlider_Increment / 0x10));
                    if (val != self->sliderPos) {
                        self->sliderPos = val;
                        StateMachine_Run(self->sliderChangedCB);
                    }
                }
            }

            self->touchPressed = touchFlag;
            return touchFlag;
        }
    }
    else {
        UISlider->entityPtr = NULL;
    }
    if (self->touchPressed)
        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
    self->flagB        = false;
    self->touchPressed = false;
    return false;
}

void UISlider_Options5CB(void)
{
    RSDK_THIS(UISlider);
    if (!self->flag) {
        self->flag      = true;
        self->field_118 = 0;
        self->field_11C = 0;
        self->field_120 = -0x20000;
        self->field_124 = -0x20000;
        self->state     = UISlider_Unknown10;
    }
}

void UISlider_Options6CB(void)
{
    RSDK_THIS(UISlider);
    if (self->flag) {
        self->flag  = false;
        self->state = UISlider_Unknown9;
    }
}

bool32 UISlider_Options7CB(void)
{
    RSDK_THIS(UISlider);
    return self->flag;
}

bool32 UISlider_Options8CB(void) { return false; }

void UISlider_Unknown9(void)
{
    RSDK_THIS(UISlider);

    if (self->field_118) {
        int32 dist = -(self->field_118 / abs(self->field_118));
        self->field_118 += dist << 15;
        if (dist < 0) {
            if (self->field_118 < 0) {
                self->field_118 = 0;
            }
            else if (dist > 0 && self->field_118 > 0)
                self->field_118 = 0;
        }
        else if (dist > 0 && self->field_118 > 0)
            self->field_118 = 0;
    }

    if (self->field_11C) {
        int32 dist = -(self->field_11C / abs(self->field_11C));
        self->field_11C += dist << 16;
        if (dist < 0) {
            if (self->field_11C < 0) {
                self->field_11C = 0;
            }
            else if (dist > 0 && self->field_11C > 0)
                self->field_11C = 0;
        }
        else if (dist > 0 && self->field_11C > 0)
            self->field_11C = 0;
    }
}

void UISlider_Unknown10(void)
{
    RSDK_THIS(UISlider);

    self->field_120 += 0x4000;
    self->field_118 += self->field_120;
    if (self->field_118 >= 0 && self->field_120 > 0) {
        self->field_118 = 0;
        self->field_120 = 0;
    }

    self->field_124 += 0x4800;
    self->field_11C += self->field_124;
    if (self->field_11C >= -0x20000 && self->field_124 > 0) {
        self->field_11C = -0x20000;
        self->field_124 = 0;
    }
}

#if RETRO_INCLUDE_EDITOR
void UISlider_EditorDraw(void) { UISlider_Draw(); }

void UISlider_EditorLoad(void) {}
#endif

void UISlider_Serialize(void)
{
    RSDK_EDITABLE_VAR(UISlider, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UISlider, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UISlider, VAR_ENUM, frameID);
}
