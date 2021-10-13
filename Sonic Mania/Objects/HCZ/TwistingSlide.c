#include "SonicMania.h"

ObjectTwistingSlide *TwistingSlide;

void TwistingSlide_Update(void)
{
    RSDK_THIS(TwistingSlide);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        int32 storeX   = player->position.x;
        int32 storeY   = player->position.y;
        if (player->state != Player_State_None)
            entity->activePlayers &= ~(1 << playerID);

        if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox1) || (player->position.y < entity->field_74)
            || player->position.y > entity->field_78) {
            entity->activePlayers &= ~(1 << playerID);
        }
        else if (entity->direction) {
            switch (entity->type) {
                case 0:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & entity->activePlayers)) {
                            entity->field_64[playerID] = (player->position.y - entity->position.y + 0x4A0000) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        int32 val = 221 * entity->field_64[playerID];
                        if (221 * entity->field_64[playerID] >= 0x3FC0)
                            val = 170 * entity->field_64[playerID];
                        player->position.x = 0x2800 * RSDK.Cos256((val >> 8) + 192) + entity->position.x;
                        player->position.y = entity->position.y + ((entity->field_64[playerID] - 74) << 16);
                        if (player->groundVel > 0 && Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                            entity->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_ROOF;
                            player->tileCollisions = true;
                            player->angle          = 144;
                        }
                    }
                    else if (!((1 << playerID) & entity->activePlayers) && player->onGround && player->groundVel < 0) {
                        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                            player->tileCollisions = false;
                            player->state          = Player_State_None;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                            entity->field_64[playerID] = (player->position.y - entity->position.y + 0x4A0000) >> 16;
                            entity->activePlayers |= (1 << playerID);
                        }
                    }
                    break;
                case 1:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & entity->activePlayers)) {
                            entity->field_64[playerID] = (0xC00000 + player->position.y - entity->position.y) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0x800000 + entity->position.x - 0xAA00 * entity->field_64[playerID];
                        player->position.y = entity->position.y + ((entity->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 2:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & entity->activePlayers)) {
                            entity->field_64[playerID] = (0xC00000 + player->position.y - entity->position.y) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0x2800 * RSDK.Cos256(((170 * entity->field_64[playerID]) >> 8) + 64) + entity->position.x;
                        player->position.y = entity->position.y + ((entity->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 3:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & entity->activePlayers)) {
                            entity->field_64[playerID] = (player->position.y + (entity->endLen << 15) - entity->position.y) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        player->position.x = entity->position.x + ((entity->endLen / 3) << 16) - 0xAA00 * entity->field_64[playerID];
                        player->position.y = entity->position.y + (entity->field_64[playerID] << 16) - (entity->endLen << 15);
                        if (player->groundVel < 0 && Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                            entity->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_RWALL;
                            player->tileCollisions = true;
                            player->angle          = 216;
                        }
                    }
                    else if (!((1 << playerID) & entity->activePlayers) && player->onGround && player->groundVel > 0
                             && Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                        player->tileCollisions = false;
                        player->state          = Player_State_None;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                        entity->field_64[playerID] = (player->position.y + (entity->endLen << 15) - entity->position.y) >> 16;
                        entity->activePlayers |= (1 << playerID);
                    }
                    break;
                default: break;
            }
        }
        else {
            switch (entity->type) {
                case 0:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & entity->activePlayers)) {
                            entity->field_64[playerID] = (player->position.y - entity->position.y + 4849664) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        int32 val = 221 * entity->field_64[playerID];
                        if (221 * entity->field_64[playerID] >= 16320)
                            val = 170 * entity->field_64[playerID];
                        player->position.x = entity->position.x - 0x2800 * RSDK.Cos256((val >> 8) + 192);
                        player->position.y = entity->position.y + ((entity->field_64[playerID] - 74) << 16);
                        if (player->groundVel < 0 && Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                            entity->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_ROOF;
                            player->tileCollisions = true;
                            player->angle          = 112;
                        }
                    }
                    else if (!((1 << playerID) & entity->activePlayers) && player->onGround && player->groundVel > 0) {
                        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                            player->tileCollisions = false;
                            player->state          = Player_State_None;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                            entity->field_64[playerID] = (player->position.y - entity->position.y + 0x4A0000) >> 16;
                            entity->activePlayers |= (1 << playerID);
                        }
                    }
                    break;
                case 1:
                    if (player->state == Player_State_None) {
                        if (((1 << playerID) & entity->activePlayers) == 0) {
                            entity->field_64[playerID] = (0xC00000 + player->position.y - entity->position.y) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0xAA00 * entity->field_64[playerID] + entity->position.x - 0x800000;
                        player->position.y = entity->position.y + ((entity->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 2:
                    if (player->state == Player_State_None) {
                        if (((1 << playerID) & entity->activePlayers) == 0) {
                            entity->field_64[playerID] = (0xC00000 + player->position.y - entity->position.y) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        player->position.x = entity->position.x - 0x2800 * RSDK.Cos256(((170 * entity->field_64[playerID]) >> 8) + 64);
                        player->position.y = entity->position.y + ((entity->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 3:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & entity->activePlayers)) {
                            entity->field_64[playerID] = (player->position.y + (entity->endLen << 15) - entity->position.y) >> 16;
                            entity->activePlayers |= 1 << playerID;
                        }
                        player->position.x = entity->position.x + 0xAA00 * entity->field_64[playerID] - ((entity->endLen / 3) << 16);
                        player->position.y = entity->position.y + (entity->field_64[playerID] << 16) - (entity->endLen << 15);
                        if (player->groundVel > 0 && Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                            entity->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_RWALL;
                            player->tileCollisions = true;
                            player->angle          = 40;
                        }
                    }
                    else if (!((1 << playerID) & entity->activePlayers) && player->onGround && player->groundVel < 0
                             && Player_CheckCollisionTouch(player, entity, &entity->hitbox3)) {
                        player->tileCollisions = false;
                        player->state          = Player_State_None;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                        entity->field_64[playerID] = (player->position.y + (entity->endLen << 15) - entity->position.y) >> 16;
                        entity->activePlayers |= (1 << playerID);
                    }
                    break;
                default: break;
            }
        }

        if (((1 << playerID) & entity->activePlayers)) {
            if (entity->direction) {
                entity->field_64[playerID] += ((-player->groundVel >> 16) * RSDK.Sin256(40)) >> 8;
                if (player->groundVel <= 0)
                    player->groundVel -= 20 * RSDK.Cos256(40);
                else
                    player->groundVel -= 80 * RSDK.Cos256(40);
                if (player->groundVel < -0x180000)
                    player->groundVel = -0x180000;
            }
            else {
                entity->field_64[playerID] += ((player->groundVel >> 16) * RSDK.Sin256(40)) >> 8;
                if (player->groundVel <= 0)
                    player->groundVel += 20 * RSDK.Cos256(40);
                else
                    player->groundVel += 80 * RSDK.Cos256(40);
                if (player->groundVel > 0x180000)
                    player->groundVel = 0x180000;
            }

            if (player->characterID == ID_TAILS) {
                player->playerAnimator.animationSpeed = 120;
            }
            else {
                player->playerAnimator.animationSpeed = ((abs(player->groundVel) * 0xF0) / 0x60000) + 0x30;
            }
            if (player->playerAnimator.animationSpeed > 0xF0)
                player->playerAnimator.animationSpeed = 0xF0;

            int32 x = player->position.x - storeX;
            int32 y = player->position.y - storeY;
            if (player->position.x != storeX && y) {
                if (player->groundVel <= 0) {
                    y = storeY - player->position.y;
                    x = storeX - player->position.x;
                }
                player->angle    = RSDK.ATan2(x, y);
                player->rotation = 2 * player->angle;
            }
        }
    }
}

void TwistingSlide_LateUpdate(void) {}

void TwistingSlide_StaticUpdate(void) {}

void TwistingSlide_Draw(void) {}

void TwistingSlide_Create(void *data)
{
    RSDK_THIS(TwistingSlide);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_BOUNDS;
        switch (entity->type) {
            case 0:
                entity->updateRange.y = 0x780000;
                entity->updateRange.x = 0x800000;
                entity->field_74      = entity->position.y - 0x4A0000;
                entity->field_78      = 0x780000 + entity->position.y;
                break;
            case 1:
                entity->updateRange.x = 0xC00000;
                entity->updateRange.y = 0xC00000;
                entity->field_74      = entity->position.y - 0xC00000;
                entity->field_78      = entity->position.y + 0xC00000;
                break;
            case 2:
                entity->updateRange.y = 0xC00000;
                entity->updateRange.x = 0x800000;
                entity->field_74      = entity->position.y - 0xC00000;
                entity->field_78      = entity->position.y + 0xC00000;
                break;
            case 3:
                entity->field_78      = entity->position.y + (entity->endLen << 15);
                entity->field_74      = entity->position.y - (entity->endLen << 15);
                entity->updateRange.y = 0xC00000;
                entity->updateRange.x = 0x800000;
                break;
            default: break;
        }
        TwistingSlide_Unknown1();
    }
}

void TwistingSlide_StageLoad(void)
{
    TwistingSlide->hitbox.left   = -1;
    TwistingSlide->hitbox.top    = 1;
    TwistingSlide->hitbox.right  = 1;
    TwistingSlide->hitbox.bottom = 1;
}

void TwistingSlide_Unknown1(void)
{
    RSDK_THIS(TwistingSlide);
    switch (entity->type) {
        case 0:
            entity->hitbox1.left   = -56;
            entity->hitbox1.top    = -120;
            entity->hitbox1.right  = 52;
            entity->hitbox1.bottom = 120;
            entity->hitbox2.top    = -46;
            entity->hitbox2.bottom = -38;
            entity->hitbox2.left   = ((-40 * RSDK.Cos256(48)) >> 8) - 4;
            entity->hitbox2.right  = ((-40 * RSDK.Cos256(48)) >> 8) + 4;
            break;
        case 1:
        case 2:
            entity->hitbox1.left   = -132;
            entity->hitbox1.top    = -196;
            entity->hitbox1.right  = 132;
            entity->hitbox1.bottom = 196;
            break;
        case 3:
            entity->hitbox1.left   = entity->endLen / -3;
            entity->hitbox1.top    = -(entity->endLen >> 1);
            entity->hitbox1.right  = entity->endLen / 3;
            entity->hitbox1.bottom = entity->endLen >> 1;
            entity->hitbox3.left   = entity->hitbox1.right - 4;
            entity->hitbox3.top    = entity->hitbox1.bottom - 4;
            entity->hitbox3.right  = entity->hitbox1.right + 4;
            entity->hitbox3.bottom = entity->hitbox1.bottom + 4;
            break;
        default: break;
    }
}

void TwistingSlide_EditorDraw(void) {}

void TwistingSlide_EditorLoad(void) {}

void TwistingSlide_Serialize(void)
{
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_ENUM, endLen);
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_UINT8, direction);
}
