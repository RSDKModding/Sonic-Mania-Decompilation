// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: YoyoPulley Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectYoyoPulley *YoyoPulley;

void YoyoPulley_Update(void)
{
    RSDK_THIS(YoyoPulley);

    int32 speed = YoyoPulley_GetLength();

    if (self->direction)
        self->rotation -= speed;
    else
        self->rotation += speed;

    YoyoPulley_UpdateHandlePos();
    int32 storeX   = self->position.x;
    int32 storeY   = self->position.y;
    self->position = self->handlePos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (((1 << playerID) & self->activePlayers) || self->playerTimers[playerID]) {
            if (player->state == Player_State_Static) {
                // ???
                RSDK.GetHitbox(&player->animator, 0);

                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = 0;
                player->rotation   = 0;
                player->position.x = self->position.x + (0xE0000 * (2 * (self->direction == FLIP_NONE) - 1));
                player->position.y = self->position.y + 0xC0000;

                if (player->jumpPress) {
                    player->position.x += 0xA0000 * (2 * (self->direction == FLIP_NONE) - 1);
                    player->position.y -= 0x40000;
                    player->velocity.x = (2 * (self->direction == FLIP_NONE) - 1) << 17;
                    player->velocity.y = -0x40000;

                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 30;
                    player->tileCollisions       = TILECOLLISION_DOWN;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->animator.speed = 48;

                    player->onGround = false;
                    player->state    = Player_State_Air;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
                player->tileCollisions = TILECOLLISION_DOWN;
            }
        }
        else if (player->state != Player_State_Static && player->down == false && player->velocity.y >= 0) {
            Hitbox *playerHitbox = RSDK.GetHitbox(&player->animator, 0);

            Hitbox hitboxPlayer;
            hitboxPlayer.top    = playerHitbox->top;
            hitboxPlayer.left   = playerHitbox->left;
            hitboxPlayer.right  = playerHitbox->right;
            hitboxPlayer.bottom = hitboxPlayer.top + 4;

            if (RSDK.CheckObjectCollisionTouchBox(self, &YoyoPulley->hitboxHandle, player, &hitboxPlayer)) {
                if (self->pullDir == FLIP_X)
                    self->pullVelocity += 0x100;

                self->activePlayers |= 1 << playerID;
                player->velocity.x     = 0;
                player->velocity.y     = 0;
                player->groundVel      = 0;
                player->angle          = 0;
                player->rotation       = 0;
                player->position.x     = self->position.x + (0xE0000 * (2 * (self->direction == FLIP_NONE) - 1));
                player->position.y     = self->position.y + 0xC0000;
                player->direction      = self->direction ^ FLIP_X;
                player->tileCollisions = TILECOLLISION_NONE;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_PULLEY_HOLD, &player->animator, true, 0);
                player->state = Player_State_Static;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
            }
        }

        if (self->playerTimers[playerID] > 0)
            self->playerTimers[playerID]--;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void YoyoPulley_LateUpdate(void) {}

void YoyoPulley_StaticUpdate(void) {}

void YoyoPulley_Draw(void) { YoyoPulley_DrawSprites(); }

void YoyoPulley_Create(void *data)
{
    RSDK_THIS(YoyoPulley);

    if (!SceneInfo->inEditor) {
        self->pullDir ^= FLIP_X;
        self->speed <<= 6;
        self->pulleyLength = 0;

        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->updateRange.x = 0x800000 + abs((self->length << 8) * RSDK.Cos512(self->angle));
        self->updateRange.y = 0x800000 + abs((self->length << 8) * RSDK.Sin512(self->angle));

        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 1, &self->shineAnimator, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 2, &self->knobAnimator, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 3, &self->handleAnimator, true, 0);

        // ideally use 'h' (for "high")
        self->drawGroup = RSDK.GetFrameID(&self->mainAnimator) ? Zone->objectDrawGroup[1] : Zone->objectDrawGroup[0];

        if (self->pullDir == FLIP_X) {
            self->pulleyLength = self->length << 8;
            self->handlePos    = self->position;
            self->handlePos.x += (self->pulleyLength + 0x2B00) * RSDK.Cos512(self->angle);
            self->handlePos.y += (self->pulleyLength + 0x2B00) * RSDK.Sin512(self->angle);
        }
    }
}

void YoyoPulley_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SSZ1"))
        YoyoPulley->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE);

    YoyoPulley->hitboxHandle.top    = -12;
    YoyoPulley->hitboxHandle.left   = -8;
    YoyoPulley->hitboxHandle.right  = 8;
    YoyoPulley->hitboxHandle.bottom = 12;
}

void YoyoPulley_UpdateHandlePos(void)
{
    RSDK_THIS(YoyoPulley);

    self->handlePos = self->position;

    self->handlePos.x += (RSDK.Cos512(self->angle) << 7) * ((self->pulleyLength >> 8) + 40);
    self->handlePos.y += (RSDK.Sin512(self->angle) << 7) * ((self->pulleyLength >> 8) + 40);

    if (self->direction)
        self->handlePos.x -= 0x150000;
    else
        self->handlePos.x += 0x150000;
}

void YoyoPulley_DrawSprites(void)
{
    RSDK_THIS(YoyoPulley);
    int32 x1 = self->position.x;
    int32 y1 = self->position.y;
    int32 x2 = (RSDK.Cos512(self->angle) << 7) * ((self->pulleyLength >> 8) + 32) + x1;
    int32 y2 = (RSDK.Sin512(self->angle) << 7) * ((self->pulleyLength >> 8) + 32) + y1;
    if (self->direction) {
        RSDK.DrawLine(x1 - 0x120000, y1, x2 - 0x120000, y2, 0x002000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x130000, y1, x2 - 0x130000, y2, 0xC02000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x140000, y1, x2 - 0x140000, y2, 0xE08000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x150000, y1, x2 - 0x150000, y2, 0xC02000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x160000, y1, x2 - 0x160000, y2, 0x002000, 0, INK_NONE, false);
    }
    else {
        RSDK.DrawLine(x1 + 0x120000, y1, x2 + 0x120000, y2, 0x002000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x130000, y1, x2 + 0x130000, y2, 0xC02000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x140000, y1, x2 + 0x140000, y2, 0xE08000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x150000, y1, x2 + 0x150000, y2, 0xC02000, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x160000, y1, x2 + 0x160000, y2, 0x002000, 0, INK_NONE, false);
    }
    RSDK.DrawSprite(&self->handleAnimator, &self->handlePos, false);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->knobAnimator, NULL, false);
}

int32 YoyoPulley_GetLength(void)
{
    RSDK_THIS(YoyoPulley);

    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (!self->activePlayers || (self->activePlayers == 0b10 && player2->sidekick)) {
        // Return to starting pos

        if (self->pullDir != FLIP_NONE) {
            if (self->pulleyLength >= (self->length << 8)) {
                self->pulleyLength = self->length << 8;
                self->pullVelocity = 0;
            }
            else {
                self->pullVelocity = self->speed;
                self->pulleyLength += self->speed;
                return 32;
            }
        }
        else {
            if (self->pulleyLength <= 0) {
                self->pullVelocity = 0;
                self->pulleyLength = 0;
            }
            else {
                self->pullVelocity = -self->speed;
                self->pulleyLength -= self->speed;
            }
        }
    }
    else {
        // Being pulled

        if (self->pullDir == FLIP_NONE) {
            if (self->pulleyLength >= (self->length << 8)) {
                self->pulleyLength = self->length << 8;
                self->pullVelocity = 0;
            }
            else {
                self->pullVelocity = self->speed;
                self->pulleyLength += self->speed;
                return 32;
            }
        }

        if (self->pulleyLength <= 0) {
            self->pullVelocity = 0;
            self->pulleyLength = 0;
        }
        else {
            if (self->pullVelocity > -self->speed)
                self->pullVelocity -= (self->speed >> 4);

            if (self->pullVelocity < -self->speed)
                self->pullVelocity = -self->speed;

            self->pulleyLength += self->pullVelocity;

            return (32 * ((self->pullVelocity << 6) / self->speed)) >> 6;
        }
    }

    return self->pulleyLength;
}

#if GAME_INCLUDE_EDITOR
void YoyoPulley_EditorDraw(void)
{
    RSDK_THIS(YoyoPulley);

    self->drawFX = FX_ROTATE | FX_FLIP;

    self->updateRange.x = 0x800000 + abs((self->length << 8) * RSDK.Cos512(self->angle));
    self->updateRange.y = 0x800000 + abs((self->length << 8) * RSDK.Sin512(self->angle));

    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 0, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 1, &self->shineAnimator, false, 0);
    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 2, &self->knobAnimator, false, 0);
    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 3, &self->handleAnimator, false, 0);

    // ideally use 'h' (for "high")
    self->drawGroup = RSDK.GetFrameID(&self->mainAnimator) ? Zone->objectDrawGroup[1] : Zone->objectDrawGroup[0];

    self->pulleyLength = 0;
    if (self->pullDir == FLIP_NONE) {
        self->pulleyLength = self->length << 8;
        self->handlePos    = self->position;
        self->handlePos.x += (self->pulleyLength + 0x2B00) * RSDK.Cos512(self->angle);
        self->handlePos.y += (self->pulleyLength + 0x2B00) * RSDK.Sin512(self->angle);
    }

    YoyoPulley_UpdateHandlePos();

    YoyoPulley_DrawSprites();
}

void YoyoPulley_EditorLoad(void)
{
    YoyoPulley->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(YoyoPulley, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);

    RSDK_ACTIVE_VAR(YoyoPulley, pullDir);
    RSDK_ENUM_VAR("Towards Center", FLIP_NONE);
    RSDK_ENUM_VAR("Away from Center", FLIP_X);
}
#endif

void YoyoPulley_Serialize(void)
{
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_UINT8, pullDir);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, speed);
}
