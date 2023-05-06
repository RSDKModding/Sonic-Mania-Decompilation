// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIShifter Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUIShifter *UIShifter;

void UIShifter_Update(void) {}

void UIShifter_LateUpdate(void)
{
    RSDK_THIS(UIShifter);

    if (self->parent) {
        int32 x = 0;
        int32 y = 0;
        if (self->parent->lastButtonID > 0) {
            x = self->parent->lastButtonID * self->shift.x;
            y = self->parent->lastButtonID * self->shift.y;
        }

        self->lastButtonShift.x = x;
        self->lastButtonShift.y = y;
        self->shiftOffset.x += (x - self->shiftOffset.x) >> 2;
        self->shiftOffset.y += (y - self->shiftOffset.y) >> 2;
        UIShifter_HandleShift();
    }
}

void UIShifter_StaticUpdate(void) {}

void UIShifter_Draw(void) {}

void UIShifter_Create(void *data)
{
    RSDK_THIS(UIShifter);

    if (!SceneInfo->inEditor) {
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;

        self->active            = ACTIVE_BOUNDS;
        self->lastButtonShift.x = 0;
        self->lastButtonShift.y = 0;
        self->shiftOffset.x     = 0;
        self->shiftOffset.y     = 0;
        self->visible           = false;
        self->updateRange.x     = 0x800000;
        self->updateRange.y     = 0x800000;
    }
}

void UIShifter_StageLoad(void) {}

void UIShifter_HandleShift(void)
{
    RSDK_THIS(UIShifter);

    int32 shiftX = -self->shift.x;
    int32 shiftY = -self->shift.y;
    int32 x      = self->startPos.x + (self->shiftOffset.x & 0xFFFF0000);
    int32 y      = self->startPos.y + (self->shiftOffset.y & 0xFFFF0000);

    for (int32 i = 0; i < self->parent->buttonCount; ++i) {
        self->parent->buttons[i]->position.x = x;
        self->parent->buttons[i]->position.y = y;
        x += shiftX;
        y += shiftY;
    }
}

#if GAME_INCLUDE_EDITOR
void UIShifter_EditorDraw(void) {}

void UIShifter_EditorLoad(void) {}
#endif

void UIShifter_Serialize(void) { RSDK_EDITABLE_VAR(UIShifter, VAR_VECTOR2, shift); }
#endif
