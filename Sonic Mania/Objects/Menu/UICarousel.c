#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUICarousel *UICarousel;

void UICarousel_Update(void) {}

void UICarousel_LateUpdate(void)
{
    RSDK_THIS(UICarousel);
    if (self->parent) {
        UICarousel_Unknown1();
        UICarousel_Unknown2();
        self->offset.x -= self->offset.x >> 2;
        self->offset.y -= self->offset.y >> 2;
        if (abs(self->offset.y) <= 0x10000)
            self->offset.y = 0;
    }
}

void UICarousel_StaticUpdate(void) {}

void UICarousel_Draw(void) {}

void UICarousel_Create(void *data)
{
    RSDK_THIS(UICarousel);
    if (!SceneInfo->inEditor) {
        self->startPos.x    = self->position.x;
        self->startPos.y    = self->position.y;
        self->active        = ACTIVE_BOUNDS;
        self->field_70      = 0;
        self->field_74      = 0;
        self->field_78      = 0;
        self->field_7C      = 0;
        self->visible       = false;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->scrollOffset      = -1;
        self->field_98      = -1;
        self->minOffset      = 0;
        self->maxOffset      = 20;
    }
}

void UICarousel_StageLoad(void) {}

void UICarousel_Unknown1(void)
{
    RSDK_THIS(UICarousel);
    EntityUIControl *control = (EntityUIControl *)self->parent;
    int32 entityID             = self->field_98;

    if (control->buttonID != entityID) {
        int32 max      = control->buttonCount - 1;
        bool32 flag  = 0;
        bool32 flag2 = 0;
        if (entityID != -1) {
            if (!entityID && control->buttonID == max) {
                flag = true;
            }
            else if (entityID != max || control->buttonID) {
                if (control->buttonID < entityID) {
                    flag = true;
                }
                else if (control->buttonID > entityID) {
                    flag2 = true;
                }
            }
            else {
                flag2 = true;
            }
        }

        int32 val = self->scrollOffset;
        if (flag) {
            --self->virtualIndex;
            self->scrollOffset = self->virtualIndex - (control->buttonCount >> 1);
        }
        else if (flag2) {
            ++self->virtualIndex;
            self->scrollOffset = self->virtualIndex - (control->buttonCount >> 1);
        }

        if (self->maxOffset != -1) {
            if (control->buttonCount + self->scrollOffset > self->maxOffset + 1)
                self->scrollOffset = self->maxOffset - control->buttonCount + 1;

            if (self->virtualIndex > self->maxOffset - 1) {
                self->virtualIndex        = self->maxOffset - 1;
                control->buttonID = entityID;
            }
        }

        if (self->minOffset != -1) {
            if (self->scrollOffset < self->minOffset - 1)
                self->scrollOffset = self->minOffset - 1;
            if (self->virtualIndex < self->minOffset) {
                self->virtualIndex        = self->minOffset;
                control->buttonID = entityID;
            }
        }

        if (self->scrollOffset >= val) {
            if (self->scrollOffset > val)
                self->offset.y += abs(self->shift.y);
            self->field_98 = control->buttonID;
        }
        else {
            self->offset.y -= abs(self->shift.y);
            self->field_98 = control->buttonID;
        }
    }
}

void UICarousel_Unknown2(void)
{
    RSDK_THIS(UICarousel);
    EntityUIControl *control = (EntityUIControl *)self->parent;
    Vector2 vecs[0x10];
    memset(vecs, 0, sizeof(vecs));

    for (int32 i = 0; i < control->buttonCount; ++i) {
        vecs[i].x = self->position.x;
        vecs[i].y = self->position.y - (self->shift.y * i);
    }

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIButton *entPtr = control->buttons[i];

        int32 pos = (i - self->scrollOffset) % control->buttonCount;
        if (pos < 0)
            pos += control->buttonCount;

        entPtr->position.x = vecs[pos].x;
        entPtr->position.y = vecs[pos].y;
        entPtr->position.x += self->offset.x;
        entPtr->position.y += self->offset.y;
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
