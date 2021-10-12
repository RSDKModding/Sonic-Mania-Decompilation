#include "SonicMania.h"

ObjectCylinder *Cylinder;

void Cylinder_Update(void)
{
    RSDK_THIS(Cylinder);
    StateMachine_Run(entity->state);
}

void Cylinder_LateUpdate(void) {}

void Cylinder_StaticUpdate(void) {}

void Cylinder_Draw(void) {}

void Cylinder_Create(void *data)
{
    RSDK_THIS(Cylinder);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_BOUNDS;
        entity->visible   = false;
        entity->drawOrder = Zone->drawOrderLow;
        switch (entity->type) {
            case 0:
                entity->updateRange.x = entity->length << 16;
                entity->updateRange.y = entity->radius << 16;

                entity->hitbox1.left   = -entity->length;
                entity->hitbox1.top    = -entity->radius;
                entity->hitbox1.right  = entity->length;
                entity->hitbox1.bottom = entity->radius;
                entity->hitbox2.left   = -entity->length;
                entity->hitbox2.top    = -entity->radius;
                entity->hitbox2.right  = entity->length;
                entity->hitbox2.bottom = -entity->radius + 8;
                entity->state          = Cylinder_State_Unknown1;
                break;
            case 1:
                entity->updateRange.x  = entity->radius << 16;
                entity->updateRange.y  = entity->length << 16;
                entity->state          = Cylinder_State_Unknown2;
                entity->hitbox1.left   = -entity->radius;
                entity->hitbox1.top    = -entity->length;
                entity->hitbox1.right  = entity->radius;
                entity->hitbox1.bottom = entity->length;
                break;
            case 2:
                entity->updateRange.x  = (entity->radius + 112) << 16;
                entity->updateRange.y  = entity->length << 16;
                entity->hitbox1.left   = -64 - entity->radius;
                entity->hitbox1.right  = entity->radius + 64;
                entity->hitbox1.top    = -entity->length;
                entity->hitbox1.bottom = entity->length;
                entity->hitbox3.left   = -112 - entity->radius;
                entity->hitbox3.right  = -entity->radius;
                entity->hitbox3.top    = -entity->length;
                entity->hitbox3.bottom = 64 - entity->length;
                entity->hitbox4.left   = entity->radius;
                entity->hitbox4.top    = entity->length - 64;
                entity->hitbox4.right  = entity->length + 112;
                entity->hitbox4.bottom = entity->length;
                entity->state          = Cylinder_State_Unknown3;
                break;
            case 3:
                entity->updateRange.x = (entity->radius + 112) << 16;
                entity->updateRange.y = entity->length << 16;

                entity->hitbox1.left   = -64 - entity->radius;
                entity->hitbox1.top    = -entity->length;
                entity->hitbox1.right  = entity->radius + 64;
                entity->hitbox1.bottom = entity->length;
                entity->hitbox3.left   = -112 - entity->radius;
                entity->hitbox3.top    = entity->length - 64;
                entity->hitbox3.right  = -entity->radius;
                entity->hitbox3.bottom = entity->length;
                entity->hitbox4.left   = entity->radius;
                entity->hitbox4.top    = -entity->length;
                entity->hitbox4.right  = entity->radius + 112;
                entity->hitbox4.bottom = 64 - entity->length;
                entity->state          = Cylinder_State_Unknown3;
                break;
            case 4:
                entity->updateRange.x  = entity->length << 16;
                entity->updateRange.y  = entity->radius << 16;
                entity->hitbox1.left   = 16 - entity->length;
                entity->hitbox1.top    = ~entity->radius;
                entity->hitbox1.right  = entity->length - 16;
                entity->hitbox1.bottom = entity->radius + 1;
                entity->hitbox2.left   = -entity->length;
                entity->hitbox2.top    = -entity->radius;
                entity->hitbox2.right  = entity->length;
                entity->hitbox2.bottom = entity->radius;
                entity->state          = Cylinder_State_Unknown4;
                break;
            case 7:
                entity->visible        = true;
                entity->updateRange.x  = entity->radius << 17;
                entity->updateRange.y  = entity->length << 17;
                entity->hitbox1.left   = -16 - entity->radius;
                entity->hitbox1.top    = -entity->length;
                entity->hitbox1.right  = entity->radius + 16;
                entity->hitbox1.bottom = entity->length;
                entity->length         = 2 * entity->length;
                entity->state          = Cylinder_State_Unknown5;
                break;
            default: return;
        }
    }
}

void Cylinder_State_Unknown1(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int radius           = (entity->radius - playerHitbox->bottom) << 6;

        if (entity->playerFrameIDs[player->playerID]) {
            if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox1) || player->velocity.y <= -0x40000) {
                if (((entity->playerAngles[player->playerID] + 128) & 0x3FF) < 0x100)
                    player->velocity.y >>= 1;
                else
                    player->velocity.y = 0;
                if (!player->onGround && player->velocity.y >= 0 && player->state != Player_State_FlyCarried
                    && player->playerAnimator.animationID != ANI_SKID) {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true,
                                            24 * entity->playerAngles[player->playerID] / 1024);
                }
                player->onGround                         = false;
                entity->playerFrameIDs[player->playerID] = 0;
            }
            else {
                entity->playerAngles[player->playerID] = (entity->playerAngles[player->playerID] + 16) & 0x3FF;
                player->position.y                     = radius * RSDK.Cos1024(entity->playerAngles[player->playerID]) + entity->position.y;

                if (player->state == Player_State_LookUp || player->state == Player_State_Crouch)
                    player->state = Player_State_Ground;

                if (player->state != Player_State_Ground || player->onGround || player->playerAnimator.animationID == ANI_SKID) {
                    if (player->state == Player_State_Roll) {
                        if (player->velocity.x)
                            player->direction = player->velocity.x > 0;
                    }
                    player->onGround = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true,
                                            24 * entity->playerAngles[player->playerID] / 1024);
                    player->onGround = true;
                }
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                if (player->position.y - entity->position.y > radius << 10 && player->velocity.y >= 0) {
                    player->position.y                       = entity->position.y + (radius << 10);
                    player->onGround                         = true;
                    player->velocity.y                       = 0;
                    entity->playerAngles[player->playerID]   = 0;
                    entity->playerFrameIDs[player->playerID] = 1;
                }
            }
        }
    }
}

void Cylinder_State_Unknown2(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int radius           = (entity->radius - playerHitbox->bottom) << 6;

        if (entity->playerFrameIDs[player->playerID]) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                entity->playerAngles[player->playerID] = (entity->playerAngles[player->playerID] + 16) & 0x3FF;
                player->position.x                     = radius * RSDK.Cos1024(entity->playerAngles[player->playerID]) + entity->position.x;
                player->rotation                       = 128;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true,
                                        (24 * entity->playerAngles[player->playerID] / 1024 + 12) % 24);
                player->playerAnimator.rotationFlag = 1;
                player->direction                   = player->velocity.y < 0;
            }
            else {
                if (player->collisionMode == CMODE_LWALL) {
                    player->angle = 64;
                }
                else if (player->collisionMode == CMODE_RWALL) {
                    player->angle = 192;
                }
                player->onGround                         = false;
                player->state                            = Player_State_Air;
                entity->playerFrameIDs[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                if (abs(player->position.x - entity->position.x) > (radius - 896) << 10) {
                    if (player->position.x - entity->position.x <= 0) {
                        player->collisionMode                  = CMODE_RWALL;
                        entity->playerAngles[player->playerID] = 512;
                    }
                    else {
                        player->collisionMode                  = CMODE_LWALL;
                        entity->playerAngles[player->playerID] = 0;
                    }
                    player->state                            = Player_State_Air;
                    player->nextAirState                     = StateMachine_None;
                    player->nextGroundState                  = StateMachine_None;
                    entity->playerFrameIDs[player->playerID] = 1;
                }
            }
        }
    }
}

void Cylinder_State_Unknown3(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int radius           = (entity->radius - playerHitbox->bottom - 12) << 6;

        switch (entity->playerFrameIDs[player->playerID]) {
            case 0:
                if (player->onGround) {
                    bool32 flag = true;
                    if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox3) || player->groundVel <= 0x30000) {
                        if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox4) || player->groundVel >= -0x30000) {
                            flag = false;
                        }
                    }
                    if (flag) {
                        entity->playerFrameIDs[player->playerID] = 1;
                        player->state                            = Cylinder_Player_State_Unknown1;
                    }
                }
                break;
            case 1:
                if (player->groundVel <= 0x30000) {
                    if (player->groundVel >= -0x30000) {
                        player->onGround                         = false;
                        entity->playerFrameIDs[player->playerID] = 0;
                    }
                    else {
                        if (player->position.x <= entity->position.x) {
                            entity->playerAngles[player->playerID]   = 768;
                            player->tileCollisions                   = false;
                            entity->playerFrameIDs[player->playerID] = 2;
                        }
                        int distance = (player->position.x - entity->position.x) >> 16;
                        int right    = entity->hitbox4.right - 48;
                        if (distance < right) {
                            player->rotation  = 256;
                            player->direction = FLIP_NONE;
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, 17 - 6 * distance / right);
                            player->playerAnimator.rotationFlag = 1;
                        }
                        player->onGround   = true;
                        player->velocity.y = 0;
                    }
                }
                else {
                    if (player->position.x >= entity->position.x) {
                        entity->playerAngles[player->playerID]   = 768;
                        player->tileCollisions                   = false;
                        entity->playerFrameIDs[player->playerID] = 2;
                    }
                    int distance = (entity->position.x - player->position.x) >> 16;
                    int right    = -48 - entity->hitbox3.left;
                    if (distance < right) {
                        player->rotation  = 256;
                        player->direction = FLIP_X;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, 17 - 6 * distance / right);
                        player->playerAnimator.rotationFlag = 1;
                        player->onGround                    = true;
                        player->velocity.y                  = 0;
                    }
                    player->onGround   = true;
                    player->velocity.y = 0;
                }
                break;
            case 2:
                entity->playerAngles[player->playerID] = (entity->playerAngles[player->playerID] + (player->groundVel >> 15)) & 0x3FF;
                player->position.x                     = radius * RSDK.Cos1024(entity->playerAngles[player->playerID]) + entity->position.x;

                if (entity->hitbox3.top >= entity->hitbox4.top)
                    player->position.y -= (player->groundVel >> 2) & 0xFFFFE000;
                else
                    player->position.y += (player->groundVel >> 2) & 0xFFFFE000;
                player->direction = FLIP_X;

                int frame = 0;
                if (player->groundVel <= 0) {
                    player->rotation = 128;
                    frame            = 35 - 24 * entity->playerAngles[player->playerID] / 1024;
                }
                else {
                    player->rotation = 384;
                    frame            = 24 * entity->playerAngles[player->playerID] / 1024;
                }

                if (player->velocity.y < -0x40000) {
                    entity->playerFrameIDs[player->playerID] = 0;
                    player->tileCollisions                   = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, frame % 24);
                    player->playerAnimator.rotationFlag = 1;
                    player->onGround                    = true;
                    player->velocity.y                  = 0;

                    if (player->groundVel <= 0) {
                        if (player->groundVel >= -0x30000) {
                            entity->playerFrameIDs[player->playerID] = 0;
                            player->tileCollisions                   = true;
                        }
                        else {
                            if (entity->hitbox3.top < entity->hitbox4.top) {
                                int maxY = entity->position.y - (playerHitbox->bottom << 16) - entity->updateRange.y + 0x400000;
                                if (player->position.y <= maxY) {
                                    player->position.y                       = maxY;
                                    entity->playerFrameIDs[player->playerID] = 3;
                                    player->tileCollisions                   = true;
                                }
                            }
                            else {
                                if (player->position.y >= entity->updateRange.y - (playerHitbox->bottom << 16) + entity->position.y) {
                                    player->position.y = entity->updateRange.y - (playerHitbox->bottom << 16) + entity->position.y;
                                    entity->playerFrameIDs[player->playerID] = 3;
                                    player->tileCollisions                   = true;
                                }
                            }
                        }
                    }
                    else {
                        if (player->groundVel <= 0x30000) {
                            entity->playerFrameIDs[player->playerID] = 0;
                            player->tileCollisions                   = true;
                        }
                        else if (entity->hitbox3.top >= entity->hitbox4.top) {
                            int maxY = entity->position.y - (playerHitbox->bottom << 16) - entity->updateRange.y + 0x400000;
                            if (player->position.y <= maxY) {
                                player->position.y                       = maxY;
                                entity->playerFrameIDs[player->playerID] = 3;
                                player->tileCollisions                   = true;
                            }
                        }
                        else {
                            if (player->position.y >= entity->updateRange.y - (playerHitbox->bottom << 16) + entity->position.y) {
                                player->position.y                       = entity->updateRange.y - (playerHitbox->bottom << 16) + entity->position.y;
                                entity->playerFrameIDs[player->playerID] = 3;
                                player->tileCollisions                   = true;
                            }
                        }
                    }
                }
                break;
            case 3:
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                    if (abs(player->groundVel) <= 0x30000) {
                        entity->playerFrameIDs[player->playerID] = 0;
                        player->state                            = Player_State_Ground;
                        if (player->groundVel <= 0) {
                            if (player->groundVel < 0)
                                player->direction = FLIP_X;
                        }
                        else {
                            player->direction = FLIP_NONE;
                        }
                    }
                    else {
                        player->onGround   = true;
                        player->velocity.y = 0;
                        if (player->groundVel >= 0) {
                            int distance = (player->position.x - entity->position.x) >> 16;
                            int right    = entity->hitbox4.right - 48;
                            if (distance < right) {
                                player->rotation  = 256;
                                player->direction = FLIP_X;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, 17 - 6 * distance / right);
                                player->playerAnimator.rotationFlag = 1;
                            }
                        }
                        else {
                            int distance = (entity->position.x - player->position.x) >> 16;
                            int right    = -48 - entity->hitbox3.left;
                            if (distance < right) {
                                player->rotation  = 256;
                                player->direction = FLIP_NONE;
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, 17 - 6 * distance / right);
                                player->playerAnimator.rotationFlag = 1;
                            }
                        }
                    }
                }
                else {
                    entity->playerFrameIDs[player->playerID] = 0;
                    player->state                            = Player_State_Ground;
                    if (player->groundVel <= 0) {
                        if (player->groundVel < 0)
                            player->direction = FLIP_X;
                    }
                    else {
                        player->direction = FLIP_NONE;
                    }
                }
                break;
            default: break;
        }
    }
}

void Cylinder_State_Unknown4(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int radius           = (entity->radius + playerHitbox->bottom) << 6;
        if (entity->playerFrameIDs[player->playerID]) {
            int angle = (entity->playerAngles[player->playerID] + 8) & 0x3FF;
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1) && !player->onGround) {
                if ((player->position.y < entity->position.y && player->velocity.y >= 0)
                    || (player->position.y >= entity->position.y && player->velocity.y <= 0)) {

                    bool32 flag = true;
                    if (player->state == Cylinder_Player_State_Unknown2) {
                        int frame = (24 - (24 * angle) / 1024) % 24;
                        if (player->groundVel)
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, frame);
                        else
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_STANDCS, &player->playerAnimator, true, frame);
                        player->playerAnimator.rotationFlag = 1;
                        player->rotation                    = 256;

                        if (player->velocity.x)
                            player->direction = player->velocity.x > 0;
                    }
                    else {
                        if (player->state != Cylinder_Player_State_Unknown3) {
                            flag = true;
                        }
                        else {
                            if (!player->velocity.x)
                                player->direction = player->velocity.x >> 31;
                        }
                    }

                    if (flag) {
                        if (angle >= 0x200)
                            player->drawOrder = Zone->playerDrawHigh;
                        else
                            player->drawOrder = Zone->playerDrawLow;
                        entity->playerAngles[player->playerID] = angle;
                        player->position.y                     = radius * RSDK.Cos1024(angle) + entity->position.y;

                        if (player->position.y < entity->position.y)
                            player->abilityValue = 0;
                        else
                            player->abilityValue = 128;
                        player->onGround = true;
                    }
                    else {
                        entity->playerFrameIDs[player->playerID] = 0;
                    }
                }
            }
            else {
                if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn && player->state != Player_State_StartJumpIn) {
                    if (player->onGround || player->velocity.y < 0 || player->state == Player_State_FlyCarried
                        || player->playerAnimator.animationID == ANI_SKID) {
                        if (player->onGround) {
                            if (player->playerAnimator.animationID != ANI_JUMP)
                                player->direction ^= 1;
                            player->state = Player_State_Ground;
                        }
                    }
                    else {
                        player->state = Player_State_Air;
                    }
                    player->onGround = false;
                }

                entity->playerFrameIDs[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1) && !player->onGround) {
                if (
#if RETRO_USE_PLUS
                    player->state == Player_State_MightyHammerDrop ||
#endif
                    player->state == Player_State_BubbleBounce) {
                    player->onGround = true;
                }
                else if ((player->position.y < entity->position.y && player->velocity.y >= 0)
                         || (player->position.y >= entity->position.y && player->velocity.y <= 0)) {
                    if (player->position.y >= entity->position.y) {
                        player->position.y                     = entity->position.y + (radius << 10);
                        entity->playerAngles[player->playerID] = 0;
                    }
                    else {
                        player->position.y                     = entity->position.y - (radius << 10);
                        entity->playerAngles[player->playerID] = 512;
                    }
                    player->onGround                         = true;
                    player->velocity.y                       = 0;
                    entity->playerFrameIDs[player->playerID] = 1;
                    int frame = (ANI_STANDCS - 24 * ((entity->playerAngles[player->playerID] + 8) & 0x3FF) / 1024) % ANI_STANDCS;
                    if (player->groundVel)
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, frame);
                    else
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_STANDCS, &player->playerAnimator, true, frame);
                    player->playerAnimator.rotationFlag = 1;
                    player->rotation                    = 256;
                    player->direction ^= 1;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->state           = Cylinder_Player_State_Unknown2;
                }
            }
        }
    }
}

void Cylinder_State_Unknown5(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int top              = entity->position.y + (entity->hitbox1.top << 16);
        int radius           = (entity->radius + playerHitbox->bottom) << 6;

        if (entity->playerFrameIDs[player->playerID]) {
            bool32 inputFlag = false;
            if (player->groundVel > 0) {
                if (player->left)
                    inputFlag = 1;
            }
            else if (player->groundVel < 0) {
                if (!player->right) {
                    if (player->left)
                        inputFlag = 1;
                }
                else if (player->right) {
                    inputFlag = 1;
                }
            }

            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                if (player->onGround) {
                    if (inputFlag) {
                        if (player->velocity.y > 0x18000)
                            player->velocity.y = player->velocity.y - 0x2000;
                        if (player->velocity.y < 0x18000)
                            player->velocity.y = 0x18000;
                    }
                    else {
                        if (player->velocity.y < 0x38000)
                            player->velocity.y = player->velocity.y + 0x2000;
                        if (player->velocity.y > 0x38000)
                            player->velocity.y = 0x38000;
                    }
                    player->position.y += player->velocity.y;

                    int len   = minVal(abs(player->position.y - top) >> 16, entity->length);
                    int frame = 0;

                    int angle = ((len << 10) / 192 + (((uint)player->groundVel >> 22) & 0x200)) & 0x3FF;
                    if (player->groundVel <= 0) {
                        frame             = (24 - 24 * angle / 1024) % 24;
                        player->direction = FLIP_NONE;
                        player->rotation  = 128;
                    }
                    else {
                        angle             = 1023 - angle;
                        frame             = (24 * angle / 1024 + 12) % 24;
                        player->direction = FLIP_X;
                        player->rotation  = 384;
                    }

                    player->position.x                     = radius * RSDK.Cos1024(angle) + entity->position.x;
                    entity->playerAngles[player->playerID] = angle;
                    if (angle >= 512)
                        player->drawOrder = Zone->playerDrawHigh;
                    else
                        player->drawOrder = Zone->playerDrawLow;

                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPRINGCS, &player->playerAnimator, true, frame);

                    player->playerAnimator.rotationFlag = 1;
                    if (player->position.x < entity->position.x)
                        player->abilityValue = 0;
                    else
                        player->abilityValue = 128;
                    player->onGround = true;
                    if (player->jumpPress) {
                        RSDK.PlaySfx(Player->sfx_Jump, false, 255);
                        if (player->position.x < entity->position.x)
                            player->velocity.x = -0x60000;
                        else
                            player->velocity.x = 0x60000;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                        player->onGround = false;
                    }
                }
            }
            else {
                if (!player->onGround && player->state != Player_State_FlyCarried) {
                    player->state          = Player_State_Air;
                    player->onGround       = false;
                    player->tileCollisions = true;
                }
                else if (player->onGround) {
                    player->state = Player_State_Ground;
                    if (player->position.x >= entity->position.x) {
                        player->angle     = 64;
                        player->direction = FLIP_NONE;
                        if (inputFlag || player->groundVel < player->velocity.y)
                            player->groundVel = player->velocity.y;
                    }
                    else {
                        player->angle     = 192;
                        player->direction = FLIP_X;
                        if (inputFlag || player->groundVel > -player->velocity.y)
                            player->groundVel = -player->velocity.y;
                    }

                    if (player->playerAnimator.animationID == ANI_SPRINGCS) {
                        if (abs(player->groundVel) < 0xC0000) {
                            if (abs(player->groundVel) < 0x60000) {
                                if (abs(player->groundVel) < 0x40000)
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_WALK, &player->playerAnimator, false, 0);
                                else
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JOG, &player->playerAnimator, false, 0);
                            }
                            else {
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RUN, &player->playerAnimator, false, 1);
                            }
                        }
                        else {
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_DASH, &player->playerAnimator, false, 1);
                        }
                    }
                    player->onGround       = true;
                    player->tileCollisions = true;
                }
                entity->playerFrameIDs[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox1) && player->onGround) {
                if ((player->angle == 64 && player->groundVel > 0) || (player->angle == 192 && player->groundVel < 0)) {
                    if (player->angle == 64 && player->direction == FLIP_X)
                        player->direction = FLIP_NONE;
                    if (player->angle == 192 && player->direction == FLIP_NONE)
                        player->direction = FLIP_X;

                    if (player->position.x >= entity->position.x) {
                        player->position.x                     = entity->position.x + (radius << 10);
                        entity->playerAngles[player->playerID] = 0;
                    }
                    else {
                        player->position.x                     = entity->position.x - (radius << 10);
                        entity->playerAngles[player->playerID] = 512;
                    }
                    player->onGround                         = true;
                    player->velocity.x                       = 0;
                    entity->playerFrameIDs[player->playerID] = 1;
                    player->nextAirState                     = 0;
                    player->nextGroundState                  = 0;
                    player->state                            = Cylinder_Player_State_Unknown4;
                    player->tileCollisions                   = false;

                    if (abs(player->groundVel) > 0x18000) {
                        if (abs(player->groundVel) >= 0x38000) {
                            player->velocity.y = 0x38000;
                        }
                        else {
                            if (player->groundVel >= 0)
                                player->velocity.y = player->groundVel;
                            else
                                player->velocity.y = -player->groundVel;
                        }
                    }
                    else {
                        player->velocity.y = 0x18000;
                    }
                }
            }
        }
    }
}

void Cylinder_Player_State_Unknown2(void)
{
    RSDK_THIS(Player);
    Player_HandleGroundMovement();
    entity->skidding = 0;
    if (entity->camera)
        entity->camera->offsetYFlag = false;
    entity->jumpAbilityTimer = 0;

    if (entity->jumpPress) {
        Player_StartJump(entity);
        if (entity->abilityValue == 128) {
            entity->velocity.y = -entity->velocity.y;
        }
        entity->direction ^= 1;
        entity->timer = 0;
    }
    else {
        if (abs(entity->groundVel) >= 0x8800 && !entity->left && !entity->right && entity->down) {
            if (entity->abilityValue == 128)
                entity->position.y -= entity->cameraOffset;
            else
                entity->position.y += entity->cameraOffset;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, false, 0);
            entity->direction ^= 1;
            entity->state = Cylinder_Player_State_Unknown3;
            RSDK.PlaySfx(Player->sfx_Roll, false, 255);
        }
    }
}

void Cylinder_Player_State_Unknown3(void)
{
    RSDK_THIS(Player);
    int angle     = entity->angle;
    entity->angle = 0;
    Player_HandleRollDeceleration();
    if (entity->state == Player_State_Ground) {
        entity->direction ^= 1;
        entity->state = Cylinder_Player_State_Unknown2;
    }
    if (entity->characterID == ID_TAILS) {
        entity->playerAnimator.animationSpeed = 120;
    }
    else {
        entity->playerAnimator.animationSpeed = ((abs(entity->groundVel) * 0xF0) / 0x60000) + 0x30;
    }

    if (entity->playerAnimator.animationSpeed > 0xF0)
        entity->playerAnimator.animationSpeed = 0xF0;

    if (entity->camera)
        entity->camera->offsetYFlag = false;
    entity->jumpAbilityTimer = 0;
    entity->angle            = angle;
    if (entity->jumpPress) {
        Player_StartJump(entity);
        if (entity->abilityValue == 128) {
            entity->velocity.y = -entity->velocity.y;
        }
        entity->timer = 0;
    }
}

void Cylinder_Player_State_Unknown4(void)
{
    RSDK_THIS(Player);
    if (entity->onGround) {
        entity->rotation = (2 * entity->angle);
    }
}

void Cylinder_Player_State_Unknown1(void)
{
    RSDK_THIS(Player);
    entity->down = false;
    Player_State_Ground();
}

void Cylinder_StageLoad(void) {}

void Cylinder_EditorDraw(void) {}

void Cylinder_EditorLoad(void) {}

void Cylinder_Serialize(void)
{
    RSDK_EDITABLE_VAR(Cylinder, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Cylinder, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(Cylinder, VAR_ENUM, radius);
}
