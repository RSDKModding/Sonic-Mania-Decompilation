#include "SonicMania.h"

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
    int32 storeX       = self->position.x;
    int32 storeY       = self->position.y;
    self->position = self->handlePos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (((1 << playerID) & self->activePlayers) || self->playerTimers[playerID]) {
            if (player->state == Player_State_None) {
                RSDK.GetHitbox(&player->animator, 0);
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = 0;
                player->rotation   = 0;
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->position.x += 0xE0000 * (2 * (self->direction == FLIP_NONE) - 1);
                player->position.y += 0xC0000;
                if (player->jumpPress) {
                    player->position.y -= 0x40000;
                    player->position.x += 0xA0000 * (2 * (self->direction == FLIP_NONE) - 1);
                    player->velocity.y = -0x40000;
                    player->velocity.x = (2 * (self->direction == FLIP_NONE) - 1) << 17;
                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 30;
                    player->tileCollisions         = true;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->animator.speed = 48;
                    player->onGround                      = false;
                    player->state                         = Player_State_Air;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
                player->tileCollisions = true;
            }
        }
        else if (player->state != Player_State_None && player->down == false && player->velocity.y >= 0) {
            Hitbox *playerHitbox = RSDK.GetHitbox(&player->animator, 0);

            Hitbox hitbox;
            hitbox.top    = playerHitbox->top;
            hitbox.left   = playerHitbox->left;
            hitbox.right  = playerHitbox->right;
            hitbox.bottom = hitbox.top + 4;
            if (RSDK.CheckObjectCollisionTouchBox(self, &YoyoPulley->hitbox, player, &hitbox)) {
                if (self->pullDir == 1)
                    self->field_C8 += 0x100;
                self->activePlayers |= 1 << playerID;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = 0;
                player->rotation   = 0;
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->direction  = self->direction ^ 1;
                player->position.x += 0xE0000 * (2 * (self->direction == FLIP_NONE) - 1);
                player->position.y += 0xC0000;
                player->tileCollisions = false;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_PULLEYHOLD, &player->animator, true, 0);
                player->state = Player_State_None;
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
        self->pullDir ^= 1;
        self->speed <<= 6;
        self->pulleyLength = 0;
        self->active       = ACTIVE_BOUNDS;
        self->visible      = true;
        self->drawFX        = FX_ROTATE | FX_FLIP;
        self->updateRange.x = 0x800000 + abs((self->length << 8) * RSDK.Cos512(self->angle));
        self->updateRange.y = 0x800000 + abs((self->length << 8) * RSDK.Sin512(self->angle));

        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 1, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 2, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 3, &self->animator4, true, 0);
        if (RSDK.GetFrameID(&self->animator1))
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = Zone->drawOrderLow;

        if (self->pullDir == 1) {
            self->pulleyLength = self->length << 8;
            self->handlePos    = self->position;
            self->handlePos.x += (self->pulleyLength + 0x2B00) * RSDK.Cos512(self->angle);
            self->handlePos.y += (self->pulleyLength + 0x2B00) * RSDK.Sin512(self->angle);
        }
    }
}

void YoyoPulley_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SSZ1"))
        YoyoPulley->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE);
    YoyoPulley->hitbox.top    = -12;
    YoyoPulley->hitbox.left   = -8;
    YoyoPulley->hitbox.right  = 8;
    YoyoPulley->hitbox.bottom = 12;
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
    RSDK.DrawSprite(&self->animator4, &self->handlePos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
}

int32 YoyoPulley_GetLength(void)
{
    RSDK_THIS(YoyoPulley);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (!self->activePlayers || (self->activePlayers == 2 && player2->sidekick)) {
        if (self->pullDir) {
            if (self->pulleyLength >= (self->length << 8)) {
                self->pulleyLength = self->length << 8;
                self->field_C8     = 0;
            }
            else {
                self->field_C8 = self->speed;
                self->pulleyLength += self->speed;
                return 32;
            }
        }
        else if (self->pulleyLength <= 0) {
            self->field_C8     = 0;
            self->pulleyLength = 0;
        }
        else {
            self->field_C8 = -self->speed;
            self->pulleyLength -= self->speed;
        }
    }
    else {
        if (!self->pullDir) {
            if (self->pulleyLength >= (self->length << 8)) {
                self->pulleyLength = self->length << 8;
                self->field_C8     = 0;
            }
            else {
                self->field_C8 = self->speed;
                self->pulleyLength += self->speed;
                return 32;
            }
        }

        if (self->pulleyLength <= 0) {
            self->field_C8     = 0;
            self->pulleyLength = 0;
        }
        else {
            if (self->field_C8 > -self->speed)
                self->field_C8 -= (self->speed >> 4);

            if (self->field_C8 < -self->speed)
                self->field_C8 = -self->speed;

            self->pulleyLength += self->field_C8;
            return (32 * ((self->field_C8 << 6) / self->speed)) >> 6;
        }
    }
    return self->pulleyLength;
}

void YoyoPulley_EditorDraw(void)
{
    RSDK_THIS(YoyoPulley);
    self->drawFX       = FX_ROTATE | FX_FLIP;

    self->updateRange.x = 0x800000 + abs((self->length << 8) * RSDK.Cos512(self->angle));
    self->updateRange.y = 0x800000 + abs((self->length << 8) * RSDK.Sin512(self->angle));

    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 1, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 2, &self->animator3, false, 0);
    RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 3, &self->animator4, false, 0);
    if (RSDK.GetFrameID(&self->animator1))
        self->drawOrder = Zone->drawOrderHigh;
    else
        self->drawOrder = Zone->drawOrderLow;

    self->pulleyLength = 0;
    if (self->pullDir == 0) {
        self->pulleyLength = self->length << 8;
        self->handlePos    = self->position;
        self->handlePos.x += (self->pulleyLength + 0x2B00) * RSDK.Cos512(self->angle);
        self->handlePos.y += (self->pulleyLength + 0x2B00) * RSDK.Sin512(self->angle);
    }

    YoyoPulley_UpdateHandlePos();

    YoyoPulley_DrawSprites();
}

void YoyoPulley_EditorLoad(void) { YoyoPulley->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE); }

void YoyoPulley_Serialize(void)
{
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_UINT8, pullDir);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, speed);
}
