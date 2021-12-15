// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MMZWheel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectMMZWheel *MMZWheel;

void MMZWheel_Update(void)
{
    RSDK_THIS(MMZWheel);
    RSDK.ProcessAnimation(&self->animator);
    self->offset.x = -self->position.x;
    self->offset.y = -self->position.y;

    switch (self->motionType) {
        default: break;
        case 0: {
            int32 val = 2 * (self->direction != FLIP_NONE) - 1;
            self->position.x += val * (self->speed << 14);
            if (abs(self->position.x - self->center.x) >= self->amplitude << 16) {
                self->direction  = self->direction == FLIP_NONE;
                self->position.x = self->center.x + (self->amplitude << 16) * val;
            }
            break;
        }
        case 1: self->position.x = (self->amplitude << 6) * RSDK.Sin1024(self->speed * Zone->timer) + self->center.x; break;
    }

    RSDK.ObjectTileGrip(self, Zone->fgLayers, 0, CMODE_FLOOR, 0, 0x180000, 8);

    self->offset.x += self->position.x;
    self->offset.y += self->position.y;
    int32 id = SceneInfo->entitySlot;
    for (int32 i = 0; i < self->childCount; ++i) {
        Entity *child = RSDK.GetEntityByID(++id);
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
    self->visible   = 1;
    self->drawOrder = Zone->drawOrderLow;
    if (SceneInfo->inEditor && !self->speed)
        self->speed = 1;
    self->updateRange.y = 0x800000;
    self->updateRange.x = (self->amplitude + 0x80) << 16;
    self->center.x      = self->position.x;
    self->center.y      = self->position.y;
    RSDK.SetSpriteAnimation(MMZWheel->aniFrames, 0, &self->animator, true, 0);
}

void MMZWheel_StageLoad(void) { MMZWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MMZWheel.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void MMZWheel_EditorDraw(void) { MMZWheel_Draw(); }

void MMZWheel_EditorLoad(void) { MMZWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MMZWheel.bin", SCOPE_STAGE); }
#endif

void MMZWheel_Serialize(void)
{
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT32, amplitude);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT8, childCount);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT8, motionType);
}
