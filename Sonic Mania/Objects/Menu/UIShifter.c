#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIShifter *UIShifter;

void UIShifter_Update(void) {}

void UIShifter_LateUpdate(void)
{
    RSDK_THIS(UIShifter);
    if (entity->parent) {
        int x = 0;
        int y = 0;
        if (entity->parent->field_D8 > 0) {
            x = entity->parent->field_D8 * entity->shift.x;
            y = entity->parent->field_D8 * entity->shift.y;
        }
        entity->field_60 = x;
        entity->field_64 = y;
        entity->field_68 += (x - entity->field_68) >> 2;
        entity->field_6C = entity->field_6C + ((y - entity->field_6C) >> 2);
        UIShifter_Unknown();
    }
}

void UIShifter_StaticUpdate(void) {}

void UIShifter_Draw(void) {}

void UIShifter_Create(void *data)
{
    RSDK_THIS(UIShifter);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->field_60      = 0;
        entity->field_64      = 0;
        entity->field_68      = 0;
        entity->field_6C      = 0;
        entity->visible       = false;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
    }
}

void UIShifter_StageLoad(void) {}

void UIShifter_Unknown(void)
{
    RSDK_THIS(UIShifter);

    int shiftX = -entity->shift.x;
    int shiftY = -entity->shift.y;
    int x      = entity->startPos.x + (entity->field_68 & 0xFFFF0000);
    int y      = entity->startPos.y + (entity->field_6C & 0xFFFF0000);
    for (int i = 0; i < entity->parent->unknownCount1; ++i) {
        entity->parent->entities[i]->position.x = x;
        entity->parent->entities[i]->position.y = y;
        x += shiftX;
        y += shiftY;
    }
}

void UIShifter_EditorDraw(void) {}

void UIShifter_EditorLoad(void) {}

void UIShifter_Serialize(void) { RSDK_EDITABLE_VAR(UIShifter, VAR_VECTOR2, shift); }
#endif
