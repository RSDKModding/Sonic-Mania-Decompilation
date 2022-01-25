// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UISlider Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUISlider *UISlider;

void UISlider_Update(void)
{
    RSDK_THIS(UISlider);

    if (self->textFrames != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->textAnimator, true, self->frameID);
        self->textFrames = UIWidgets->textFrames;
    }

    self->touchPosOffsetS.y = self->buttonBounceOffset;
    self->touchPosOffsetS.x = 0x7A0000 + self->buttonBounceOffset;

    StateMachine_Run(self->state);

    EntityUIControl *control = (EntityUIControl *)self->parent;

    int32 id = -1;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (self == (EntityUISlider *)control->buttons[i]) {
            id = i;
            break;
        }
    }

    if (self->state == UISlider_State_HandleButtonEnter && (control->state != UIControl_ProcessInputs || control->buttonID != id)) {
        self->isSelected         = false;
        self->textBounceOffset   = 0;
        self->buttonBounceOffset = 0;
        if (self->isSelected) {
            self->isSelected  = false;
            self->state = UISlider_State_HandleButtonLeave;
        }
    }
}

void UISlider_LateUpdate(void) {}

void UISlider_StaticUpdate(void) {}

void UISlider_Draw(void)
{
    UISlider_DrawBGShapes();
    UISlider_DrawSlider();
}

void UISlider_Create(void *data)
{
    RSDK_THIS(UISlider);
    self->visible            = true;
    self->drawOrder          = 2;
    self->active             = ACTIVE_BOUNDS;
    self->updateRange.x      = 0x800000;
    self->updateRange.y      = 0x400000;
    self->touchPosOffsetS.y      = 0;
    self->touchPosOffsetS.x      = 0x7A0000;
    self->touchPosSizeS.x    = 0x940000;
    self->touchPosSizeS.y    = 0x180000;
    self->size.x             = 0x5A0000;
    self->size.y             = 0x180000;
    self->bgEdgeSize         = 24;
    self->processButtonCB    = UISlider_ButtonPressCB;
    self->touchCB            = UISlider_TouchCB;
    self->selectedCB         = StateMachine_None;
    self->failCB             = StateMachine_None;
    self->buttonEnterCB      = UISlider_ButtonEnterCB;
    self->buttonLeaveCB      = UISlider_ButtonLeaveCB;
    self->checkButtonEnterCB = UISlider_CheckButtonEnterCB;
    self->checkSelectedCB    = UISlider_CheckSelectedCB;
    self->textVisible        = true;
    self->sliderPos          = (UISlider_MaxVal - UISlider_MinVal) / 2;
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->textAnimator, true, self->frameID);
    self->textFrames = UIWidgets->textFrames;
}

void UISlider_StageLoad(void) {}

void UISlider_DrawBGShapes(void)
{
    RSDK_THIS(UISlider);
    Vector2 drawPos;

    int32 width = self->size.y + self->size.x;
    drawPos.x   = self->position.x;
    drawPos.y   = self->position.y;
    UIWidgets_DrawParallelogram((self->size.y >> 16), width >> 16, self->bgEdgeSize, 0, 0, 0, self->position.x, self->position.y);

    if (self->textVisible) {
        drawPos = self->position;
        drawPos.y += self->textBounceOffset;
        drawPos.x += -0x60000 - (self->size.x >> 1);
        RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
    }
}

void UISlider_DrawSlider(void)
{
    RSDK_THIS(UISlider);
    Vector2 drawPos;

    int32 drawX     = 0x7A0000 + self->position.x;
    int32 sliderPos = (((34048 * self->sliderPos) >> 2) & 0xFFFFFF00) - 0xB0000;
    int32 drawX2    = drawX - ((0x7A0000 - sliderPos) >> 1);
    UIWidgets_DrawParallelogram(12, 122, 12, 240, 240, 240, drawX - self->buttonBounceOffset, self->position.y - self->buttonBounceOffset);
    UIWidgets_DrawParallelogram(12, 122, 12, 0, 0, 0, self->buttonBounceOffset + drawX, self->buttonBounceOffset + self->position.y);
    UIWidgets_DrawParallelogram(12, sliderPos >> 16, 12, 232, 40, 88, self->buttonBounceOffset + drawX2, self->buttonBounceOffset + self->position.y);

    drawPos.x = drawX2 + (sliderPos >> 1) + self->buttonBounceOffset + 0x60000;
    drawPos.y = self->buttonBounceOffset + self->position.y;
    if (self->isSelected) {
        drawPos.x += 0x30000;
        drawPos.y += 0x30000;
        UIWidgets_DrawRectOutline_Blended(24, 14, drawPos.x, drawPos.y);
        drawPos.x -= 0x30000;
        drawPos.y -= 0x30000;
    }

    RSDK.DrawRect(drawPos.x - 0x70000, drawPos.y - 0xC0000, 0xE0000, 0x180000, 0xF0F0F0, 255, INK_NONE, false);
    if (self->isSelected)
        UIWidgets_DrawRectOutline_Flash(24, 14, drawPos.x, drawPos.y);
}

void UISlider_ButtonPressCB(void)
{
    RSDK_THIS(UISlider);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    int32 rowID = 0;
    if (parent->rowCount && parent->columnCount)
        rowID = parent->buttonID / parent->columnCount;
    else
        rowID = 0;

    int32 columnID = 0;
    if (parent->columnCount)
        columnID = parent->buttonID % parent->columnCount;
    else
        columnID = 0;

    bool32 moveV = false;
    if (parent->rowCount > 1) {
        if (UIControl->keyUp) {
            --rowID;
            moveV = true;
        }
        if (UIControl->keyDown) {
            ++rowID;
            moveV = true;
        }
    }

    bool32 valueChanged = false;
    if (UIControl->keyLeft && self->sliderPos > UISlider_MinVal) {
        self->sliderPos = (self->sliderPos & -UISlider_Increment) - UISlider_Increment;
        valueChanged    = true;
    }
    if (UIControl->keyRight && self->sliderPos < UISlider_MaxVal) {
        self->sliderPos = (self->sliderPos & -UISlider_Increment) + UISlider_Increment;
        valueChanged    = true;
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

        if (parent->buttonID != max) {
            parent->buttonID = max;
            if (self->isSelected) {
                self->isSelected  = false;
                self->state = UISlider_State_HandleButtonLeave;
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

        if (!self->isSelected && parent->buttonID == id && parent->state == UIControl_ProcessInputs && !parent->dialogHasFocus) {
            UISlider_ButtonEnterCB();
        }
    }
}

bool32 UISlider_TouchCB(void)
{
    RSDK_THIS(UISlider);

    bool32 touchFlag = false;
    if (TouchInfo->count) {
        if (!UISlider->activeEntity || UISlider->activeEntity == (Entity *)self) {
            int32 sizeX = self->touchPosSizeS.x >> 1;
            int32 sizeY = self->touchPosSizeS.y >> 1;
            for (int32 i = 0; i < TouchInfo->count; ++i) {
                int32 x = (ScreenInfo->position.x << 16) - ((TouchInfo->x[i] * ScreenInfo->width) * -65536.0f);
                int32 y = (ScreenInfo->position.y << 16) - ((TouchInfo->y[i] * ScreenInfo->height) * -65536.0f);

                int32 touchX = abs(self->touchPosOffsetS.x + self->position.x - x);
                int32 touchY = abs(self->touchPosOffsetS.y + self->position.y - y);
                if (!self->isTouchSelected && touchX < sizeX && touchY < sizeY) {
                    self->isTouchSelected = true;
                    RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
                }

                if (self->isTouchSelected) {
                    touchFlag              = true;
                    UISlider->activeEntity = (Entity *)self;

                    self->sliderPosTouch = maxVal(x + sizeX - self->position.x - self->touchPosOffsetS.x, 0x70000);
                    if (self->touchPosSizeS.x - 0x70000 < self->sliderPosTouch)
                        self->sliderPosTouch = self->touchPosSizeS.x - 0x70000;

                    int32 val = 16
                                * (minVal(((self->sliderPosTouch - 0x70000) >> 4 << 10) / (self->touchPosSizeS.x - 0xE0000) + 2, UISlider_MaxVal)
                                   & -(UISlider_Increment / 0x10));
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
        UISlider->activeEntity = NULL;
    }

    if (self->touchPressed)
        RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
    self->isTouchSelected = false;
    self->touchPressed    = false;
    return false;
}

void UISlider_ButtonEnterCB(void)
{
    RSDK_THIS(UISlider);
    if (!self->isSelected) {
        self->isSelected           = true;
        self->textBounceOffset     = 0;
        self->buttonBounceOffset   = 0;
        self->textBounceVelocity   = -0x20000;
        self->buttonBounceVelocity = -0x20000;
        self->state                = UISlider_State_HandleButtonEnter;
    }
}

void UISlider_ButtonLeaveCB(void)
{
    RSDK_THIS(UISlider);
    if (self->isSelected) {
        self->isSelected  = false;
        self->state = UISlider_State_HandleButtonLeave;
    }
}

bool32 UISlider_CheckButtonEnterCB(void)
{
    RSDK_THIS(UISlider);
    return self->isSelected;
}

bool32 UISlider_CheckSelectedCB(void) { return false; }

void UISlider_State_HandleButtonLeave(void)
{
    RSDK_THIS(UISlider);

    if (self->textBounceOffset) {
        int32 dist = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += dist << 15;
        if (dist < 0) {
            if (self->textBounceOffset < 0) {
                self->textBounceOffset = 0;
            }
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
            if (self->buttonBounceOffset < 0) {
                self->buttonBounceOffset = 0;
            }
            else if (dist > 0 && self->buttonBounceOffset > 0)
                self->buttonBounceOffset = 0;
        }
        else if (dist > 0 && self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UISlider_State_HandleButtonEnter(void)
{
    RSDK_THIS(UISlider);

    self->textBounceVelocity += 0x4000;
    self->textBounceOffset += self->textBounceVelocity;
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
