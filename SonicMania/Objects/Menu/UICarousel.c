// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UICarousel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUICarousel *UICarousel;

void UICarousel_Update(void) {}

void UICarousel_LateUpdate(void)
{
    RSDK_THIS(UICarousel);

    if (self->parent) {
        UICarousel_HandleScrolling();
        UICarousel_HandleButtonPositions();

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
        self->startPos.x = self->position.x;
        self->startPos.y = self->position.y;
        self->active     = ACTIVE_BOUNDS;

        self->unused1 = 0;
        self->unused2 = 0;
        self->unused3 = 0;
        self->unused4 = 0;

        self->visible       = false;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->scrollOffset  = -1;
        self->buttonID      = -1;
        self->minOffset     = 0;
        self->maxOffset     = 20;
    }
}

void UICarousel_StageLoad(void) {}

void UICarousel_HandleScrolling(void)
{
    RSDK_THIS(UICarousel);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    int32 buttonID           = self->buttonID;

    if (control->buttonID != buttonID) {
        int32 max        = control->buttonCount - 1;
        bool32 movedUp   = false;
        bool32 movedDown = false;

        if (buttonID != -1) {
            if (!buttonID && control->buttonID == max) {
                movedUp = true;
            }
            else if (buttonID == max && !control->buttonID) {
                movedDown = true;
            }
            else {
                if (control->buttonID < buttonID)
                    movedUp = true;
                else if (control->buttonID > buttonID)
                    movedDown = true;
            }
        }

        // Bug Details:
        // This is HORRIBLY broken when using touch controls
        // the virtual scrolling expects buttons to only be incremented one at a time
        // HOWEVER, using touch you can jump to any visible button, which can be jumping 2 or more buttons at a time!!
        // as a result of that, the scrollOffset & virtualIndex will become misaligned from the visual buttonID and mess up controls and visuals
        // Fix:
        // find a way to increment by however many buttons we jumped, rather than just by 1
        // this is slightly harder than it sounds due to the virtual scroll & wrapping being applied

        int32 scrollOffset = self->scrollOffset;
        if (movedUp) {
            --self->virtualIndex;
            self->scrollOffset = self->virtualIndex - (control->buttonCount >> 1);
        }
        else if (movedDown) {
            ++self->virtualIndex;
            self->scrollOffset = self->virtualIndex - (control->buttonCount >> 1);
        }

        if (self->maxOffset != -1) {
            if (self->scrollOffset + control->buttonCount > self->maxOffset + 1)
                self->scrollOffset = self->maxOffset - control->buttonCount + 1;

            if (self->virtualIndex > self->maxOffset - 1) {
                self->virtualIndex = self->maxOffset - 1;
                control->buttonID  = buttonID;
            }
        }

        if (self->minOffset != -1) {
            if (self->scrollOffset < self->minOffset - 1)
                self->scrollOffset = self->minOffset - 1;

            if (self->virtualIndex < self->minOffset) {
                self->virtualIndex = self->minOffset;
                control->buttonID  = buttonID;
            }
        }

        if (self->scrollOffset >= scrollOffset) {
            if (self->scrollOffset > scrollOffset)
                self->offset.y += abs(self->shift.y);

            self->buttonID = control->buttonID;
        }
        else {
            self->offset.y -= abs(self->shift.y);
            self->buttonID = control->buttonID;
        }
    }
}

void UICarousel_HandleButtonPositions(void)
{
    RSDK_THIS(UICarousel);

    EntityUIControl *control = (EntityUIControl *)self->parent;

    Vector2 positions[0x10];
    memset(positions, 0, sizeof(positions));

    for (int32 i = 0; i < control->buttonCount; ++i) {
        positions[i].x = self->position.x;
        positions[i].y = self->position.y - (self->shift.y * i);
    }

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIButton *item = control->buttons[i];

        int32 id = (i - self->scrollOffset) % control->buttonCount;
        if (id < 0)
            id += control->buttonCount;

        item->position.x = positions[id].x + self->offset.x;
        item->position.y = positions[id].y + self->offset.y;
        item->drawGroup  = id - control->buttonCount + 12;
    }
}

#if GAME_INCLUDE_EDITOR
void UICarousel_EditorDraw(void)
{
    RSDK_THIS(UICarousel);

    DrawHelpers_DrawRectOutline(self->position.x + self->clipOffset.x, self->position.y + self->clipOffset.y, self->clipSize.x, self->clipSize.y,
                                0xFF0000);
}

void UICarousel_EditorLoad(void) {}
#endif

void UICarousel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICarousel, VAR_VECTOR2, shift);
    RSDK_EDITABLE_VAR(UICarousel, VAR_VECTOR2, clipSize);
    RSDK_EDITABLE_VAR(UICarousel, VAR_VECTOR2, clipOffset);
}
#endif
