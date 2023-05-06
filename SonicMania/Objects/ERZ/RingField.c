// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RingField Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
                RingField_GetRingSpawnPos(&pos);

                EntityRing *ring     = CREATE_ENTITY(Ring, &pos, pos.x, pos.y);
                ring->animator.speed = 512;
                ring->state          = Ring_State_Normal;
                ring->drawGroup      = Zone->objectDrawGroup[0];
                ring->stateDraw      = Ring_Draw_Normal;
                ring->moveType       = RING_MOVE_FIXED;
                RSDK.SetSpriteAnimation(RingField->aniFrames, 0, &ring->animator, true, 0);

                int32 sx = (ScreenInfo->center.x + ScreenInfo->position.x) << 16;
                int32 sy = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
                int32 x  = sx + (RSDK.Rand(-ScreenInfo->center.x, ScreenInfo->center.x) << 15);
                int32 y  = sy + (RSDK.Rand(-ScreenInfo->center.y, ScreenInfo->center.y) << 15);

                int32 angle      = RSDK.ATan2(x - pos.x, y - pos.y);
                ring->velocity.x = RSDK.Cos256(angle) << 9;
                ring->velocity.y = RSDK.Sin256(angle) << 9;
                self->timer      = (self->fluctuation * RSDK.Sin256(Zone->timer) >> 8) + self->frequency;
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
    Vector2 range = { 0x400000, 0x400000 };

    foreach_all(Ring, ring)
    {
        if (ring->velocity.x || ring->velocity.y) {
            ring->position.x += ring->velocity.x;
            ring->position.y += ring->velocity.y;
        }

        if (!RSDK.CheckOnScreen(ring, &range))
            destroyEntity(ring);
    }
}

void RingField_Draw(void) {}

void RingField_Create(void *data)
{
    RSDK_THIS(RingField);

    self->active        = ACTIVE_NORMAL;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = false;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!self->size.x)
        self->size.x = 1600 << 16;

    if (!self->size.y)
        self->size.y = 480 << 16;

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

void RingField_GetRingSpawnPos(Vector2 *pos)
{
    int32 rand = RSDK.Rand(0, 16) % 4;
    int32 x    = 0;
    int32 y    = 0;

    switch (rand) {
        case 0:
        case 1:
            y = RSDK.Rand(-ScreenInfo->size.y, ScreenInfo->size.y) << 15;

            switch (rand) {
                case 0: x += -0x100000 - (ScreenInfo->center.x << 16); break;
                case 1: x += (ScreenInfo->center.x + 16) << 16; break;
                case 2: y += -0x100000 - (ScreenInfo->center.y << 16); break;
                case 3: y += (ScreenInfo->center.y + 16) << 16; break;
                default: break;
            }
            break;

        case 2:
        case 3:
            x = RSDK.Rand(-ScreenInfo->size.x, ScreenInfo->size.x) << 15;

            switch (rand) {
                case 0: x += -0x100000 - (ScreenInfo->center.x << 16); break;
                case 1: x += (ScreenInfo->center.x + 16) << 16; break;
                case 2: y += -0x100000 - (ScreenInfo->center.y << 16); break;
                case 3: y += (ScreenInfo->center.y + 16) << 16; break;
                default: break;
            }
            break;

        default: break;
    }

    pos->x = x + ((ScreenInfo->position.x + ScreenInfo->center.x) << 16);
    pos->y = y + ((ScreenInfo->position.y + ScreenInfo->center.y) << 16);
}

#if GAME_INCLUDE_EDITOR
void RingField_EditorDraw(void)
{
    RSDK_THIS(RingField);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        if (!self->size.x)
            self->size.x = 1600 << 16;

        if (!self->size.y)
            self->size.y = 480 << 16;

        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

        RSDK_DRAWING_OVERLAY(false);
    }
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
