// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RingField Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectRingField *RingField;

void RingField_Update(void)
{
    RSDK_THIS(RingField);
    if (self->running) {
        bool32 inRange = false;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                inRange = true;
            }
        }

        if (inRange) {
            if (self->timer <= 0) {
                Vector2 pos;
                RingField_GetPos(&pos);
                EntityRing *ring          = CREATE_ENTITY(Ring, &pos, pos.x, pos.y);
                ring->animator.speed = 512;
                ring->state               = Ring_State_Normal;
                ring->drawOrder           = Zone->drawOrderLow;
                ring->stateDraw           = Ring_Draw_Normal;
                ring->moveType            = RING_MOVE_FIXED;
                RSDK.SetSpriteAnimation(RingField->aniFrames, 0, &ring->animator, true, 0);

                int32 sx = (ScreenInfo->centerX + ScreenInfo->position.x) << 16;
                int32 sy = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
                int32 x  = (RSDK.Rand(-ScreenInfo->centerX, ScreenInfo->centerX) << 15) + sx;
                int32 y  = RSDK.Rand(-ScreenInfo->centerY, ScreenInfo->centerY);

                int32 angle = RSDK.ATan2(x - pos.x, (y << 15) + sy - pos.y);
                ring->velocity.x = RSDK.Cos256(angle) << 9;
                ring->velocity.y = RSDK.Sin256(angle) << 9;
                self->timer    = (self->fluctuation * RSDK.Sin256(Zone->timer) >> 8) + self->frequency;
            }
            else {
                self->timer--;
            }
        }
    }
}

void RingField_LateUpdate(void) {}

void RingField_StaticUpdate(void)
{
    Vector2 updateRange;
    updateRange.x = 0x400000;
    updateRange.y = 0x400000;

    foreach_all(Ring, ring)
    {
        if (ring->velocity.x || ring->velocity.y) {
            ring->position.x += ring->velocity.x;
            ring->position.y += ring->velocity.y;
        }
        if (!RSDK.CheckOnScreen(ring, &updateRange))
            destroyEntity(ring);
    }
}

void RingField_Draw(void) {}

void RingField_Create(void *data)
{
    RSDK_THIS(RingField);

    self->active        = ACTIVE_NORMAL;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = false;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (!self->size.x)
        self->size.x = 0x6400000;
    if (!self->size.y)
        self->size.y = 0x1E00000;
    if (!self->frequency)
        self->frequency = 60;
    if (!self->fluctuation)
        self->fluctuation = 20;

    self->hitbox.left   = -self->size.x >> 17;
    self->hitbox.top    = -self->size.y >> 17;
    self->hitbox.right  = self->size.x >> 17;
    self->hitbox.bottom = self->size.y >> 17;
}

void RingField_StageLoad(void) { RingField->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE); }

void RingField_GetPos(Vector2 *pos)
{
    int32 rand = RSDK.Rand(0, 16) % 4;
    int32 x    = 0;
    int32 y    = 0;
    switch (rand) {
        case 0:
        case 1:
            y = RSDK.Rand(-ScreenInfo->height, ScreenInfo->height) << 15;
            switch (rand) {
                case 0: x += -0x100000 - (ScreenInfo->centerX << 16); break;
                case 1: x += (ScreenInfo->centerX + 16) << 16; break;
                case 2: y += -0x100000 - (ScreenInfo->centerY << 16); break;
                case 3: y += (ScreenInfo->centerY + 16) << 16; break;
                default: break;
            }
            break;
        case 2:
        case 3:
            x = RSDK.Rand(-ScreenInfo->width, ScreenInfo->width) << 15;
            switch (rand) {
                case 0: x += -0x100000 - (ScreenInfo->centerX << 16); break;
                case 1: x += (ScreenInfo->centerX + 16) << 16; break;
                case 2: y += -0x100000 - (ScreenInfo->centerY << 16); break;
                case 3: y += (ScreenInfo->centerY + 16) << 16; break;
                default: break;
            }
            break;
        default: break;
    }
    pos->x = x + ((ScreenInfo->position.x + ScreenInfo->centerX) << 16);
    pos->y = y + ((ScreenInfo->position.y + ScreenInfo->centerY) << 16);
}

#if RETRO_INCLUDE_EDITOR
void RingField_EditorDraw(void)
{
    RSDK_THIS(RingField);

    if (showGizmos())
        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);
}

void RingField_EditorLoad(void) {}
#endif

void RingField_Serialize(void)
{
    RSDK_EDITABLE_VAR(RingField, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(RingField, VAR_ENUM, frequency);
    RSDK_EDITABLE_VAR(RingField, VAR_ENUM, fluctuation);
    RSDK_EDITABLE_VAR(RingField, VAR_BOOL, running);
}
