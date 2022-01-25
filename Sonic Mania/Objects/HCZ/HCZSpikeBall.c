// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HCZSpikeBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectHCZSpikeBall *HCZSpikeBall;

void HCZSpikeBall_Update(void)
{
    HCZSpikeBall_Unknown4();
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
    self->drawOrder     = Zone->drawOrderLow;
    self->field_5C.x    = self->position.x;
    self->field_5C.y    = self->position.y;
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
            if (MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &conveyor->hitbox3, self->position.x,
                                          self->position.y)) {
                self->conveyor    = conveyor;
                self->updateRange = conveyor->updateRange;
                foreach_break;
            }
        }

        if (self->conveyor)
            HCZSpikeBall_Unknown2();
    }
}

void HCZSpikeBall_Unknown2(void)
{
    RSDK_THIS(HCZSpikeBall);

    EntityHangConveyor *conveyor = self->conveyor;
    if (conveyor) {
        Hitbox hitbox1, hitbox2, hitbox3, hitbox4;

        int32 len = 0;
        if (conveyor->length != -3)
            len = (conveyor->length + 3) << 20;

        hitbox1.top    = -26;
        hitbox1.bottom = -16;
        hitbox1.right  = len >> 17;
        hitbox1.left   = -hitbox1.right;

        hitbox2.left   = conveyor->hitbox3.left;
        hitbox2.top    = conveyor->hitbox3.top;
        hitbox2.right  = -hitbox1.right;
        hitbox2.bottom = conveyor->hitbox3.bottom;

        hitbox3.left   = -(len >> 17);
        hitbox3.top    = 16;
        hitbox3.right  = len >> 17;
        hitbox3.bottom = 26;

        hitbox4.left   = len >> 17;
        hitbox4.top    = hitbox2.top;
        hitbox4.right  = conveyor->hitbox3.right;
        hitbox4.bottom = hitbox2.bottom;

        if (MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox1, self->position.x,
                                      self->position.y)) {
            int32 dist = conveyor->position.x - (len >> 1);
            if (conveyor->direction == FLIP_NONE)
                dist = conveyor->position.x + (len >> 1);
            self->field_68 = abs(self->position.x - (self->position.x - dist)) / 0x15555;
        }
        else if (MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox2, self->position.x,
                                           self->position.y)) {
            int32 off   = len / 0x15555;
            int32 angle = 384;
            if (conveyor->direction) {
                off   = 2 * (len / 0x15555) + 51;
                angle = 128;
            }

            self->field_68 =
                (abs(angle - 2 * RSDK.ATan2(self->position.x - conveyor->position.x - (len >> 1), self->position.y - conveyor->position.y)) / 5)
                + off;
        }
        else {
            if (!MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox3, self->position.x,
                                           self->position.y)) {

                if (!MathHelpers_PointInHitbox(conveyor->direction, conveyor->position.x, conveyor->position.y, &hitbox4, self->position.x,
                                               self->position.y)) {
                    self->field_68 = 0;
                }
                else {
                    int32 dist  = len / 0x15555;
                    int32 angle = 384;
                    if (!conveyor->direction) {
                        dist  = 2 * (len / 0x15555) + 51;
                        angle = 128;
                    }

                    self->field_68 =
                        (abs(angle
                             - 2 * RSDK.ATan2(self->position.x - (len >> 1) + conveyor->position.x, self->position.y - conveyor->position.y))
                         / 5)
                        + dist;
                }
            }
            else {
                int32 dist = (len >> 1) + conveyor->position.x;
                if (conveyor->direction == FLIP_NONE)
                    dist = conveyor->position.x - (len >> 1);
                self->field_68 = abs((self->position.x - dist)) / 0x15555 + (len / 0x15555) + 51;
            }
        }
    }
}

void HCZSpikeBall_HandlePlayerInteractions(void)
{
    RSDK_THIS(HCZSpikeBall);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            Player_CheckHit(player, self);
        }
    }
}

void HCZSpikeBall_Unknown4(void)
{
    RSDK_THIS(HCZSpikeBall);

    if (self->conveyor) {
        int32 len = 0;
        if (self->conveyor->length != -3)
            len = (self->conveyor->length + 3) << 20;

        int32 timer   = (self->field_68 + Zone->timer) % ((2 * len + 0x8A3AE6) / 0x15555);
        int32 conveyX = self->conveyor->position.x;
        int32 conveyY = self->conveyor->position.y;
        if (timer < len / 0x15555) {
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = (len >> 1) + conveyX;
            else
                self->position.x = conveyX - (len >> 1);
            self->position.y = conveyY - 0x160000;
            self->position.x += 0x15555 * timer * (2 * (self->conveyor->direction != FLIP_NONE) - 1);
        }
        else if (timer < len / 0x15555 + 51) {
            int32 val = timer - (len / 0x15555);
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = conveyX - (len >> 1);
            else
                self->position.x = (len >> 1) + conveyX;

            int32 mult = 5;
            if (self->conveyor->direction == FLIP_NONE)
                mult = -5;

            self->position.y = conveyY;
            self->position.x += 0xB00 * RSDK.Cos512(val * mult + 384);
            self->position.y += 0xB00 * RSDK.Sin512(val * mult + 384);
        }
        else if (timer >= 2 * (len / 0x15555) + 51) {
            self->position.y = -51 - 2 * (len / 0x15555) + timer;
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = conveyX + (len >> 1);
            else
                self->position.x = conveyX - (len >> 1);

            int32 mult = 5;
            if (self->conveyor->direction == FLIP_NONE)
                mult = -5;

            self->position.x += 0xB00 * RSDK.Cos512((-51 - 2 * (len / 0x15555) + timer) * mult + 128);
            self->position.y += 0xB00 * RSDK.Sin512((-51 - 2 * (len / 0x15555) + timer) * mult + 128);
        }
        else {
            self->position.y = conveyY + 0x160000;
            if (self->conveyor->direction == FLIP_NONE)
                self->position.x = (conveyX - (len >> 1));
            else
                self->position.x = (len >> 1) + conveyX;
            self->position.x += 0x15555 * (-51 - (len / 0x15555) + timer) * (2 * (self->conveyor->direction == FLIP_NONE) - 1);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void HCZSpikeBall_EditorDraw(void) { HCZSpikeBall_Draw(); }

void HCZSpikeBall_EditorLoad(void) { HCZSpikeBall->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Platform.bin", SCOPE_STAGE); }
#endif

void HCZSpikeBall_Serialize(void) {}
