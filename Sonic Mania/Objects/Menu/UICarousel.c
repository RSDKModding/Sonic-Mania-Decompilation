#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUICarousel *UICarousel;

void UICarousel_Update(void) {}

void UICarousel_LateUpdate(void)
{
    RSDK_THIS(UICarousel);
    if (entity->parent) {
        UICarousel_Unknown1();
        UICarousel_Unknown2();
        entity->offset.x -= entity->offset.x >> 2;
        entity->offset.y -= entity->offset.y >> 2;
        if (abs(entity->offset.y) <= 0x10000)
            entity->offset.y = 0;
    }
}

void UICarousel_StaticUpdate(void) {}

void UICarousel_Draw(void) {}

void UICarousel_Create(void *data)
{
    RSDK_THIS(UICarousel);
    if (!RSDK_sceneInfo->inEditor) {
        entity->startPos.x    = entity->position.x;
        entity->startPos.y    = entity->position.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->field_70      = 0;
        entity->field_74      = 0;
        entity->field_78      = 0;
        entity->field_7C      = 0;
        entity->visible       = false;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->scrollOffset      = -1;
        entity->field_98      = -1;
        entity->minOffset      = 0;
        entity->maxOffset      = 20;
    }
}

void UICarousel_StageLoad(void) {}

void UICarousel_Unknown1(void)
{
    RSDK_THIS(UICarousel);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    int32 entityID             = entity->field_98;

    if (control->activeEntityID != entityID) {
        int32 max      = control->buttonCount - 1;
        bool32 flag  = 0;
        bool32 flag2 = 0;
        if (entityID != -1) {
            if (!entityID && control->activeEntityID == max) {
                flag = true;
            }
            else if (entityID != max || control->activeEntityID) {
                if (control->activeEntityID < entityID) {
                    flag = true;
                }
                else if (control->activeEntityID > entityID) {
                    flag2 = true;
                }
            }
            else {
                flag2 = true;
            }
        }

        int32 val = entity->scrollOffset;
        if (flag) {
            --entity->virtualIndex;
            entity->scrollOffset = entity->virtualIndex - (control->buttonCount >> 1);
        }
        else if (flag2) {
            ++entity->virtualIndex;
            entity->scrollOffset = entity->virtualIndex - (control->buttonCount >> 1);
        }

        if (entity->maxOffset != -1) {
            if (control->buttonCount + entity->scrollOffset > entity->maxOffset + 1)
                entity->scrollOffset = entity->maxOffset - control->buttonCount + 1;

            if (entity->virtualIndex > entity->maxOffset - 1) {
                entity->virtualIndex        = entity->maxOffset - 1;
                control->activeEntityID = entityID;
            }
        }

        if (entity->minOffset != -1) {
            if (entity->scrollOffset < entity->minOffset - 1)
                entity->scrollOffset = entity->minOffset - 1;
            if (entity->virtualIndex < entity->minOffset) {
                entity->virtualIndex        = entity->minOffset;
                control->activeEntityID = entityID;
            }
        }

        if (entity->scrollOffset >= val) {
            if (entity->scrollOffset > val)
                entity->offset.y += abs(entity->shift.y);
            entity->field_98 = control->activeEntityID;
        }
        else {
            entity->offset.y -= abs(entity->shift.y);
            entity->field_98 = control->activeEntityID;
        }
    }
}

void UICarousel_Unknown2(void)
{
    RSDK_THIS(UICarousel);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    Vector2 vecs[0x10];
    memset(vecs, 0, sizeof(vecs));

    for (int32 i = 0; i < control->buttonCount; ++i) {
        vecs[i].x = entity->position.x;
        vecs[i].y = entity->position.y - (entity->shift.y * i);
    }

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIButton *entPtr = control->buttons[i];

        int32 pos = (i - entity->scrollOffset) % control->buttonCount;
        if (pos < 0)
            pos += control->buttonCount;

        entPtr->position.x = vecs[pos].x;
        entPtr->position.y = vecs[pos].y;
        entPtr->position.x += entity->offset.x;
        entPtr->position.y += entity->offset.y;
        entPtr->drawOrder = pos - control->buttonCount + 12;
    }
}

#if RETRO_INCLUDE_EDITOR
void UICarousel_EditorDraw(void) {}

void UICarousel_EditorLoad(void) {}
#endif

void UICarousel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICarousel, VAR_VECTOR2, shift);
    RSDK_EDITABLE_VAR(UICarousel, VAR_VECTOR2, clipSize);
    RSDK_EDITABLE_VAR(UICarousel, VAR_VECTOR2, clipOffset);
}
#endif
