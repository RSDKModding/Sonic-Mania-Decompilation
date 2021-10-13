#include "SonicMania.h"

ObjectYoyoPulley *YoyoPulley;

void YoyoPulley_Update(void)
{
    RSDK_THIS(YoyoPulley);

    int32 speed = YoYoPulley_GetLength();
    if (entity->direction)
        entity->rotation -= speed;
    else
        entity->rotation += speed;
    YoYoPulley_UpdateHandlePos();
    int32 storeX       = entity->position.x;
    int32 storeY       = entity->position.y;
    entity->position = entity->handlePos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        if (((1 << playerID) & entity->activePlayers) || entity->playerTimers[playerID]) {
            if (player->state == Player_State_None) {
                RSDK.GetHitbox(&player->playerAnimator, 0);
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = 0;
                player->rotation   = 0;
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->position.x += 0xE0000 * (2 * (entity->direction == FLIP_NONE) - 1);
                player->position.y += 0xC0000;
                if (player->jumpPress) {
                    player->position.y -= 0x40000;
                    player->position.x += 0xA0000 * (2 * (entity->direction == FLIP_NONE) - 1);
                    player->velocity.y = -0x40000;
                    player->velocity.x = (2 * (entity->direction == FLIP_NONE) - 1) << 17;
                    entity->activePlayers &= ~(1 << playerID);
                    entity->playerTimers[playerID] = 30;
                    player->tileCollisions         = true;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->playerAnimator.animationSpeed = 48;
                    player->onGround                      = false;
                    player->state                         = Player_State_Air;
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
                player->tileCollisions = true;
            }
        }
        else if (player->state != Player_State_None && player->down == false && player->velocity.y >= 0) {
            Hitbox *playerHitbox = RSDK.GetHitbox(&player->playerAnimator, 0);

            Hitbox hitbox;
            hitbox.top    = playerHitbox->top;
            hitbox.left   = playerHitbox->left;
            hitbox.right  = playerHitbox->right;
            hitbox.bottom = hitbox.top + 4;
            if (RSDK.CheckObjectCollisionTouchBox(entity, &YoyoPulley->hitbox, player, &hitbox)) {
                if (entity->pullDir == 1)
                    entity->field_C8 += 0x100;
                entity->activePlayers |= 1 << playerID;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->angle      = 0;
                player->rotation   = 0;
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->direction  = entity->direction ^ 1;
                player->position.x += 0xE0000 * (2 * (entity->direction == FLIP_NONE) - 1);
                player->position.y += 0xC0000;
                player->tileCollisions = false;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_PULLEYHOLD, &player->playerAnimator, true, 0);
                player->state = Player_State_None;
                RSDK.PlaySfx(Player->sfx_Grab, false, 255);
            }
        }

        if (entity->playerTimers[playerID] > 0)
            entity->playerTimers[playerID]--;
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void YoyoPulley_LateUpdate(void) {}

void YoyoPulley_StaticUpdate(void) {}

void YoyoPulley_Draw(void) { YoYoPulley_DrawSprites(); }

void YoyoPulley_Create(void *data)
{
    RSDK_THIS(YoyoPulley);
    if (!RSDK_sceneInfo->inEditor) {
        entity->pullDir ^= 1;
        entity->speed <<= 6;
        entity->pulleyLength = 0;
        entity->active       = ACTIVE_BOUNDS;
        entity->visible      = true;
        entity->drawFX       = FX_ROTATE | FX_FLIP;

        if ((entity->length << 8) * RSDK.Cos512(entity->angle) >= 0)
            entity->updateRange.x = 0x800000 + (entity->length << 8) * RSDK.Cos512(entity->angle);
        else
            entity->updateRange.x = 0x800000 + -((entity->length << 8) * RSDK.Cos512(entity->angle));

        if ((entity->length << 8) * RSDK.Sin512(entity->angle) >= 0)
            entity->updateRange.y = 0x800000 + (entity->length << 8) * RSDK.Sin512(entity->angle);
        else
            entity->updateRange.y = 0x800000 + -((entity->length << 8) * RSDK.Sin512(entity->angle));

        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 2, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(YoyoPulley->aniFrames, 3, &entity->animator4, true, 0);
        if (RSDK.GetFrameID(&entity->animator1))
            entity->drawOrder = Zone->drawOrderHigh;
        else
            entity->drawOrder = Zone->drawOrderLow;

        if (entity->pullDir == 1) {
            entity->pulleyLength = entity->length << 8;
            entity->handlePos    = entity->position;
            entity->handlePos.x += (entity->pulleyLength + 0x2B00) * RSDK.Cos512(entity->angle);
            entity->handlePos.y += (entity->pulleyLength + 0x2B00) * RSDK.Sin512(entity->angle);
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

void YoYoPulley_UpdateHandlePos(void)
{
    RSDK_THIS(YoyoPulley);

    entity->handlePos = entity->position;

    entity->handlePos.x += (RSDK.Cos512(entity->angle) << 7) * ((entity->pulleyLength >> 8) + 40);
    entity->handlePos.y += (RSDK.Sin512(entity->angle) << 7) * ((entity->pulleyLength >> 8) + 40);

    if (entity->direction)
        entity->handlePos.x -= 0x150000;
    else
        entity->handlePos.x += 0x150000;
}

void YoYoPulley_DrawSprites(void)
{
    RSDK_THIS(YoyoPulley);
    int32 x1 = entity->position.x;
    int32 y1 = entity->position.y;
    int32 x2 = (RSDK.Cos512(entity->angle) << 7) * ((entity->pulleyLength >> 8) + 32) + x1;
    int32 y2 = (RSDK.Sin512(entity->angle) << 7) * ((entity->pulleyLength >> 8) + 32) + y1;
    if (entity->direction) {
        RSDK.DrawLine(x1 - 0x120000, y1, x2 - 0x120000, y2, 0x2000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x130000, y1, x2 - 0x130000, y2, 0xC02000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x140000, y1, x2 - 0x140000, y2, 0xE08000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x150000, y1, x2 - 0x150000, y2, 0xC02000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 - 0x160000, y1, x2 - 0x160000, y2, 0x2000u, 0, INK_NONE, false);
    }
    else {
        RSDK.DrawLine(x1 + 0x120000, y1, x2 + 0x120000, y2, 0x2000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x130000, y1, x2 + 0x130000, y2, 0xC02000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x140000, y1, x2 + 0x140000, y2, 0xE08000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x150000, y1, x2 + 0x150000, y2, 0xC02000u, 0, INK_NONE, false);
        RSDK.DrawLine(x1 + 0x160000, y1, x2 + 0x160000, y2, 0x2000u, 0, INK_NONE, false);
    }
    RSDK.DrawSprite(&entity->animator4, &entity->handlePos, false);
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.DrawSprite(&entity->animator3, 0, false);
}

int32 YoYoPulley_GetLength(void)
{
    RSDK_THIS(YoyoPulley);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (!entity->activePlayers || (entity->activePlayers == 2 && player2->sidekick)) {
        if (entity->pullDir) {
            if (entity->pulleyLength >= (entity->length << 8)) {
                entity->pulleyLength = entity->length << 8;
                entity->field_C8     = 0;
            }
            else {
                entity->field_C8 = entity->speed;
                entity->pulleyLength += entity->speed;
                return 32;
            }
        }
        else if (entity->pulleyLength <= 0) {
            entity->field_C8     = 0;
            entity->pulleyLength = 0;
        }
        else {
            entity->field_C8 = -entity->speed;
            entity->pulleyLength -= entity->speed;
        }
    }
    else {
        if (!entity->pullDir) {
            if (entity->pulleyLength >= (entity->length << 8)) {
                entity->pulleyLength = entity->length << 8;
                entity->field_C8     = 0;
            }
            else {
                entity->field_C8 = entity->speed;
                entity->pulleyLength += entity->speed;
                return 32;
            }
        }

        if (entity->pulleyLength <= 0) {
            entity->field_C8     = 0;
            entity->pulleyLength = 0;
        }
        else {
            if (entity->field_C8 > -entity->speed)
                entity->field_C8 -= (entity->speed >> 4);

            if (entity->field_C8 < -entity->speed)
                entity->field_C8 = -entity->speed;

            entity->pulleyLength += entity->field_C8;
            return (32 * ((entity->field_C8 << 6) / entity->speed)) >> 6;
        }
    }
    return entity->pulleyLength;
}

void YoyoPulley_EditorDraw(void) { YoYoPulley_DrawSprites(); }

void YoyoPulley_EditorLoad(void) { YoyoPulley->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/SDashWheel.bin", SCOPE_STAGE); }

void YoyoPulley_Serialize(void)
{
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_UINT8, pullDir);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, angle);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(YoyoPulley, VAR_ENUM, speed);
}
