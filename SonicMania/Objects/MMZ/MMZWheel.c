// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MMZWheel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMMZWheel *MMZWheel;

void MMZWheel_Update(void)
{
    RSDK_THIS(MMZWheel);

    RSDK.ProcessAnimation(&self->animator);

    self->offset.x = -self->position.x;
    self->offset.y = -self->position.y;

    switch (self->motionType) {
        default: break;

        case MMZWHEEL_MOTION_STIFF: {
            int32 direction = 2 * (self->direction != FLIP_NONE) - 1;
            self->position.x += direction * (self->speed << 14);

            if (abs(self->position.x - self->center.x) >= self->amplitude << 16) {
                self->direction  = self->direction == FLIP_NONE;
                self->position.x = self->center.x + (self->amplitude << 16) * direction;
            }
            break;
        }

        case MMZWHEEL_MOTION_SMOOTH: self->position.x = self->center.x + (self->amplitude << 6) * RSDK.Sin1024(self->speed * Zone->timer); break;
    }

    RSDK.ObjectTileGrip(self, Zone->collisionLayers, 0, CMODE_FLOOR, 0, 0x180000, 8);

    self->offset.x += self->position.x;
    self->offset.y += self->position.y;

    int32 id = SceneInfo->entitySlot;
    for (int32 i = 0; i < self->childCount; ++i) {
        Entity *child = RSDK_GET_ENTITY_GEN(++id);
        child->position.x += self->offset.x;
        child->position.y += self->offset.y;
    }
}

void MMZWheel_LateUpdate(void) {}

void MMZWheel_StaticUpdate(void) {}

void MMZWheel_Draw(void)
{
    RSDK_THIS(MMZWheel);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MMZWheel_Create(void *data)
{
    RSDK_THIS(MMZWheel);

    self->active    = ACTIVE_BOUNDS;
    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    if (SceneInfo->inEditor && !self->speed)
        self->speed = 1;

    self->updateRange.y = 0x800000;
    self->updateRange.x = (self->amplitude + 0x80) << 16;
    self->center.x      = self->position.x;
    self->center.y      = self->position.y;
    RSDK.SetSpriteAnimation(MMZWheel->aniFrames, 0, &self->animator, true, 0);
}

void MMZWheel_StageLoad(void) { MMZWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MMZWheel.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void MMZWheel_EditorDraw(void)
{
    RSDK_THIS(MMZWheel);

    MMZWheel_Create(NULL);
    MMZWheel_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        for (int32 s = SceneInfo->entitySlot + 1, i = 0; i < self->childCount; ++i) {
            Entity *child = RSDK_GET_ENTITY_GEN(s + i);
            if (!child)
                continue;

            DrawHelpers_DrawArrow(self->position.x, self->position.y, child->position.x, child->position.y, 0xE0E0E0, INK_NONE, 0xFF);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void MMZWheel_EditorLoad(void)
{
    MMZWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MMZWheel.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(MMZWheel, motionType);
    RSDK_ENUM_VAR("Stiff", MMZWHEEL_MOTION_STIFF);
    RSDK_ENUM_VAR("Smooth", MMZWHEEL_MOTION_SMOOTH);
}
#endif

void MMZWheel_Serialize(void)
{
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT32, amplitude);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT8, childCount);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT8, motionType);
}
