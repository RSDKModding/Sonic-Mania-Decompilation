// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HCZSpikeBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

// NOTE:
// This is a mania-fied version of the spikeballs present on S3 hang conveyors.
// This object is 100% fully functional.
// However, they're never placed in any stage, Making them unused.

ObjectHCZSpikeBall *HCZSpikeBall;

void HCZSpikeBall_Update(void)
{
    HCZSpikeBall_HandleConveyorMovement();
    HCZSpikeBall_HandlePlayerInteractions();
}

void HCZSpikeBall_LateUpdate(void) {}

void HCZSpikeBall_StaticUpdate(void) {}

void HCZSpikeBall_Draw(void)
{
    RSDK_THIS(HCZSpikeBall);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void HCZSpikeBall_Create(void *data)
{
    RSDK_THIS(HCZSpikeBall);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    self->hitbox.left   = -10;
    self->hitbox.top    = -10;
    self->hitbox.right  = 10;
    self->hitbox.bottom = 10;

    HCZSpikeBall_HandleConveyorSetup();
    RSDK.SetSpriteAnimation(HCZSpikeBall->aniFrames, 0, &self->animator, true, 1);
}

void HCZSpikeBall_StageLoad(void) { HCZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE); }

void HCZSpikeBall_HandleConveyorSetup(void)
{
    RSDK_THIS(HCZSpikeBall);

    if (!SceneInfo->inEditor) {
        foreach_all(HangConveyor, conveyor)
        {
            if (MathHelpers_PointInHitbox(conveyor->position.x, conveyor->position.y, self->position.x, self->position.y, conveyor->direction,
                                          &conveyor->hitboxSpikeBallRange)) {
                self->conveyor    = conveyor;
                self->updateRange = conveyor->updateRange;
                foreach_break;
            }
        }

        if (self->conveyor)
            HCZSpikeBall_LinkToConveyor();
    }
}

void HCZSpikeBall_LinkToConveyor(void)
{
    RSDK_THIS(HCZSpikeBall);

    EntityHangConveyor *conveyor = self->conveyor;
    if (conveyor) {
        Hitbox hitboxTop, hitboxLeft, hitboxBottom, hitboxRight;

        int32 len = 0;
        if (conveyor->length != -3)
            len = (conveyor->length + 3) << 20;

        hitboxTop.top    = -26;
        hitboxTop.bottom = -16;
        hitboxTop.right  = len >> 17;
        hitboxTop.left   = -hitboxTop.right;

        hitboxLeft.left   = conveyor->hitboxSpikeBallRange.left;
        hitboxLeft.top    = conveyor->hitboxSpikeBallRange.top;
        hitboxLeft.right  = -hitboxTop.right;
        hitboxLeft.bottom = conveyor->hitboxSpikeBallRange.bottom;

        hitboxBottom.left   = -(len >> 17);
        hitboxBottom.top    = 16;
        hitboxBottom.right  = len >> 17;
        hitboxBottom.bottom = 26;

        hitboxRight.left   = len >> 17;
        hitboxRight.top    = hitboxLeft.top;
        hitboxRight.right  = conveyor->hitboxSpikeBallRange.right;
        hitboxRight.bottom = hitboxLeft.bottom;

        if (MathHelpers_PointInHitbox(conveyor->position.x, conveyor->position.y, self->position.x, self->position.y, conveyor->direction,
                                      &hitboxTop)) {
            int32 dist = conveyor->position.x - (len >> 1);
            if (conveyor->direction == FLIP_NONE)
                dist = conveyor->position.x + (len >> 1);
            self->intervalOffset = abs(self->position.x - (self->position.x - dist)) / 0x15555;
        }
        else if (MathHelpers_PointInHitbox(conveyor->position.x, conveyor->position.y, self->position.x, self->position.y, conveyor->direction,
                                           &hitboxLeft)) {
            int32 dist  = len / 0x15555;
            int32 angle = 0x180;
            if (conveyor->direction) {
                dist  = 2 * (len / 0x15555) + 51;
                angle = 0x80;
            }

            int32 atan           = RSDK.ATan2(self->position.x - conveyor->position.x - (len >> 1), self->position.y - conveyor->position.y);
            self->intervalOffset = (abs(angle - 2 * atan) / 5) + dist;
        }
        else if (MathHelpers_PointInHitbox(conveyor->position.x, conveyor->position.y, self->position.x, self->position.y, conveyor->direction,
                                           &hitboxBottom)) {
            int32 dist = (len >> 1) + conveyor->position.x;
            if (conveyor->direction == FLIP_NONE)
                dist = conveyor->position.x - (len >> 1);
            self->intervalOffset = abs((self->position.x - dist)) / 0x15555 + (len / 0x15555) + 51;
        }
        else if (MathHelpers_PointInHitbox(conveyor->position.x, conveyor->position.y, self->position.x, self->position.y, conveyor->direction,
                                           &hitboxRight)) {
            int32 dist  = len / 0x15555;
            int32 angle = 0x180;
            if (!conveyor->direction) {
                dist  = 2 * (len / 0x15555) + 51;
                angle = 0x80;
            }

            int32 atan           = RSDK.ATan2(self->position.x - (len >> 1) + conveyor->position.x, self->position.y - conveyor->position.y);
            self->intervalOffset = (abs(angle - 2 * atan) / 5) + dist;
        }
        else {
            self->intervalOffset = 0;
        }
    }
}

void HCZSpikeBall_HandlePlayerInteractions(void)
{
    RSDK_THIS(HCZSpikeBall);
    foreach_active(Player, player)
    {
        // Oversight(?) Note:
        // Due to this object being unused, it didn't get updated with mighty collision checks
        // It's not 100% certain that it would've, but since the rest of the game's spiked objects do, this one could've too
        if (Player_CheckCollisionTouch(player, self, &self->hitbox))
            Player_Hurt(player, self);
    }
}

void HCZSpikeBall_HandleConveyorMovement(void)
{
    RSDK_THIS(HCZSpikeBall);

    if (self->conveyor) {
        int32 len = 0;
        if (self->conveyor->length != -3)
            len = (self->conveyor->length + 3) << 20;

        int32 interval = (2 * len + 0x8A3AE6) / 0x15555;
        int32 timer    = (self->intervalOffset + Zone->timer) % interval;
        int32 conveyX  = self->conveyor->position.x;
        int32 conveyY  = self->conveyor->position.y;

        if (timer < len / 0x15555) {
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = conveyX + (len >> 1);
            else
                self->position.x = conveyX - (len >> 1);

            self->position.x += 0x15555 * timer * (2 * (self->conveyor->direction != FLIP_NONE) - 1);
            self->position.y = conveyY - 0x160000;
        }
        else if (timer < (len / 0x15555) + 51) {
            int32 angle = timer - (len / 0x15555);
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = conveyX - (len >> 1);
            else
                self->position.x = conveyX + (len >> 1);

            int32 mult = self->conveyor->direction == FLIP_NONE ? -5 : 5;

            self->position.y = conveyY;
            self->position.x += 0xB00 * RSDK.Cos512(angle * mult + 0x180);
            self->position.y += 0xB00 * RSDK.Sin512(angle * mult + 0x180);
        }
        else if (timer >= 2 * (len / 0x15555) + 51) {
            int32 angle = -51 - 2 * (len / 0x15555) + timer;
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = conveyX + (len >> 1);
            else
                self->position.x = conveyX - (len >> 1);

            int32 mult = self->conveyor->direction == FLIP_NONE ? -5 : 5;

            self->position.y = conveyY;
            self->position.x += 0xB00 * RSDK.Cos512(angle * mult + 0x80);
            self->position.y += 0xB00 * RSDK.Sin512(angle * mult + 0x80);
        }
        else {
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = conveyX - (len >> 1);
            else
                self->position.x = conveyX + (len >> 1);

            self->position.x += 0x15555 * (-51 - (len / 0x15555) + timer) * (2 * (self->conveyor->direction == FLIP_NONE) - 1);
            self->position.y = conveyY + 0x160000;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void HCZSpikeBall_EditorDraw(void) { HCZSpikeBall_Draw(); }

void HCZSpikeBall_EditorLoad(void) { HCZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE); }
#endif

void HCZSpikeBall_Serialize(void) {}
