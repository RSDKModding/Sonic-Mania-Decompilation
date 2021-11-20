#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIShifter *UIShifter;

void UIShifter_Update(void) {}

void UIShifter_LateUpdate(void)
{
    RSDK_THIS(UIShifter);
    if (self->parent) {
        int32 x = 0;
        int32 y = 0;
        if (self->parent->field_D8 > 0) {
            x = self->parent->field_D8 * self->shift.x;
            y = self->parent->field_D8 * self->shift.y;
        }
        self->field_60 = x;
        self->field_64 = y;
        self->field_68 += (x - self->field_68) >> 2;
        self->field_6C = self->field_6C + ((y - self->field_6C) >> 2);
        UIShifter_Unknown();
    }
}

void UIShifter_StaticUpdate(void) {}

void UIShifter_Draw(void) {}

void UIShifter_Create(void *data)
{
    RSDK_THIS(UIShifter);
    if (!SceneInfo->inEditor) {
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->active        = ACTIVE_BOUNDS;
        self->field_60      = 0;
        self->field_64      = 0;
        self->field_68      = 0;
        self->field_6C      = 0;
        self->visible       = false;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
    }
}

void UIShifter_StageLoad(void) {}

void UIShifter_Unknown(void)
{
    RSDK_THIS(UIShifter);

    int32 shiftX = -self->shift.x;
    int32 shiftY = -self->shift.y;
    int32 x      = self->startPos.x + (self->field_68 & 0xFFFF0000);
    int32 y      = self->startPos.y + (self->field_6C & 0xFFFF0000);
    for (int32 i = 0; i < self->parent->buttonCount; ++i) {
        self->parent->buttons[i]->position.x = x;
        self->parent->buttons[i]->position.y = y;
        x += shiftX;
        y += shiftY;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIShifter_EditorDraw(void) {}

void UIShifter_EditorLoad(void) {}
#endif

void UIShifter_Serialize(void) { RSDK_EDITABLE_VAR(UIShifter, VAR_VECTOR2, shift); }
#endif
