#include "SonicMania.h"

ObjectLRZSpiral *LRZSpiral;

void LRZSpiral_Update(void)
{
    RSDK_THIS(LRZSpiral);
    StateMachine_Run(entity->state);
}

void LRZSpiral_LateUpdate(void) {}

void LRZSpiral_StaticUpdate(void) {}

void LRZSpiral_Draw(void) {}

void LRZSpiral_Create(void *data)
{
    RSDK_THIS(LRZSpiral);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_BOUNDS;
        entity->visible   = false;
        entity->drawOrder = Zone->drawOrderLow;

        switch (entity->type) {
            default: break;
            case 0: {
                entity->state         = LRZSpiral_Unknown2;
                entity->updateRange.x = 0x800000;
                entity->updateRange.y = (entity->height + 1) << 22;
                entity->hitbox.left   = -16;
                entity->hitbox.right  = 16;
                int32 height          = entity->height << 25 >> 19;
                entity->hitbox.top    = -height;
                entity->hitbox.bottom = 128 - height;
                entity->height        = (entity->height << 25) - 0x1000000;
                break;
            }
            case 1:
                entity->direction     = FLIP_NONE;
                entity->updateRange.y = (entity->height + 64) << 16;
                entity->hitbox.left   = -112;
                entity->hitbox.top    = -180;
                entity->hitbox.right  = -104;
                entity->hitbox.bottom = -172;
                entity->state         = LRZSpiral_Unknown3;
                entity->updateRange.x = 0x800000;
                break;
            case 2:
                entity->direction     = FLIP_X;
                entity->updateRange.y = (entity->height + 64) << 16;
                entity->hitbox.left   = -146;
                entity->hitbox.top    = -180;
                entity->hitbox.right  = -138;
                entity->hitbox.bottom = -172;
                entity->state         = LRZSpiral_Unknown4;
                entity->updateRange.x = 0x800000;
                break;
        }
    }
}

void LRZSpiral_StageLoad(void) {}

void LRZSpiral_Unknown1(void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (abs(player->groundVel) < player->maxRunSpeed) {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_RUN, &player->playerAnimator, false, 1);
        player->playerAnimator.animationSpeed = (abs(player->groundVel) >> 12) + 96;
        if (player->playerAnimator.animationSpeed > 0x200)
            player->playerAnimator.animationSpeed = 0x200;
        player->maxJogSpeed = 0x58000;
        player->maxRunSpeed = 0xC0000;
    }
    else {
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_DASH, &player->playerAnimator, false, 1);
        player->maxRunSpeed = 0xB8000;
    }
}
void LRZSpiral_Unknown2(void)
{
    RSDK_THIS(LRZSpiral);
    foreach_active(Player, player)
    {
        int32 pID = RSDK.GetEntityID(player);
        if ((1 << pID) & entity->activePlayers) {
            if (player->state == Player_State_None) {
                entity->playerVelocity[pID] += player->groundVel;
                if (entity->playerVelocity[pID] < 0) {
                    entity->activePlayers &= ~(1 << pID);
                }
                else if (entity->playerVelocity[pID] < entity->height) {
                    if (player->groundVel < 0x100000)
                        player->groundVel += 0x1000;
                    if ((uint8)((entity->playerVelocity[pID] >> 17) + 64) < 0x80)
                        player->drawOrder = Zone->playerDrawHigh;
                    else
                        player->drawOrder = Zone->playerDrawLow;
                    player->position.x = 0x4800 * RSDK.Sin256(entity->playerVelocity[pID] >> 17) + entity->position.x;
                    player->position.y = entity->playerVelocity[pID] >> 2;
                    int32 pos          = (player->position.y >> 16) & 0x7F;
                    player->position.y = player->position.y & 0xFF800000;
                    player->position.y += 0x580000 + entity->position.y - ((entity->height + 0x1000000) >> 3);
                    if (entity->playerVelocity[pID] >= entity->height - 0x1000000)
                        player->position.y += LRZSpiral->array2[pos];
                    else
                        player->position.y += LRZSpiral->array1[pos];
                    player->playerAnimator.frameID = (pos << 8) / 0x555;
                    player->direction              = (player->playerAnimator.frameID + 6) % 24 > 12;
                }
                else {
                    player->tileCollisions = true;
                    player->collisionPlane = 1;
                    player->controlLock    = 16;
                    player->direction      = FLIP_X;
                    player->onGround       = true;
                    player->groundedStore  = true;
                    player->state          = Player_State_Ground;
                    player->groundVel      = -player->groundVel;
                    entity->activePlayers &= ~(1 << pID);
                    LRZSpiral_Unknown1(player);
                    if (!entity->activePlayers)
                        entity->active = ACTIVE_BOUNDS;
                }
            }
            else {
                entity->activePlayers &= ~(1 << pID);
            }
        }
        else if (player->position.x >= entity->position.x) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (player->groundVel < 0x60000)
                    player->groundVel = 0x60000;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPIRALRUN, &player->playerAnimator, false, 0);
                player->playerAnimator.animationSpeed = 0;
                player->state                         = Player_State_None;
                player->nextAirState                  = StateMachine_None;
                player->nextGroundState               = StateMachine_None;
                player->tileCollisions                = false;
                entity->activePlayers |= 1 << pID;
                entity->playerVelocity[pID] = 0;
                entity->active              = ACTIVE_NORMAL;
            }
        }
    }
}
void LRZSpiral_Unknown3(void)
{
    RSDK_THIS(LRZSpiral);
    foreach_active(Player, player)
    {
        int32 pID = RSDK.GetEntityID(player);
        if ((1 << pID) & entity->activePlayers) {
            if (player->state == Player_State_None) {
                entity->playerVelocity[pID] += player->groundVel;
                if (entity->playerVelocity[pID] < 0) {
                    entity->activePlayers &= ~(1 << pID);
                }
                else {
                    entity->playerVelocity[pID] += player->groundVel >> 17;
                    if (player->groundVel < 0x100000)
                        player->groundVel += 0x1000;
                    player->position.x = 0xA000 * RSDK.Sin256(entity->playerVelocity[pID] >> 18);
                    player->position.x += entity->position.x - 0x6C0000;
                    player->position.y = entity->position.y - 0xB00000 + 0x15800 * ((256 - RSDK.Cos256(entity->playerVelocity[pID] >> 18)) >> 1);
                    if (entity->playerVelocity[pID] < 0 || entity->playerVelocity[pID] >= 0x2000000) {
                        player->tileCollisions = true;
                        player->collisionPlane = 1;
                        player->controlLock    = 16;
                        player->direction      = FLIP_X;
                        player->onGround       = true;
                        player->groundedStore  = true;
                        player->state          = Player_State_Ground;
                        player->groundVel      = -player->groundVel;
                        entity->activePlayers &= ~(1 << pID);
                        LRZSpiral_Unknown1(player);
                        if (!entity->activePlayers)
                            entity->active = ACTIVE_BOUNDS;
                    }
                    else {
                        player->playerAnimator.frameID = entity->playerVelocity[pID] / 0x2AAAAA;
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << pID);
            }
        }
        else if (player->position.x >= entity->position.x - 0x6C0000 && player->groundVel >= 0 && player->onGround) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (player->groundVel < 0x40000)
                    player->groundVel = 0x40000;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_TWISTRUN, &player->playerAnimator, false, 0);
                player->playerAnimator.animationSpeed = 0;
                player->state                         = Player_State_None;
                player->nextAirState                  = StateMachine_None;
                player->nextGroundState               = StateMachine_None;
                player->tileCollisions                = false;
                entity->activePlayers |= 1 << pID;
                entity->playerVelocity[pID] = 0;
                entity->active              = ACTIVE_NORMAL;
            }
        }
    }
}
void LRZSpiral_Unknown4(void)
{
    RSDK_THIS(LRZSpiral);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if ((1 << playerID) & entity->activePlayers) {
            if (player->state == Player_State_None) {
                entity->playerVelocity[playerID] -= player->groundVel;
                if (entity->playerVelocity[playerID] < 0) {
                    entity->activePlayers &= ~(1 << playerID);
                }
                else {
                    entity->playerVelocity[playerID] += player->groundVel >> 17;
                    if (player->groundVel > -0x100000)
                        player->groundVel -= 0x1000;
                    player->position.x = -0xA000 * RSDK.Sin256(entity->playerVelocity[playerID] >> 18);
                    player->position.x += 0x8E0000 + entity->position.x;
                    player->position.y =
                        entity->position.y - 0xB40000 + 0x14200 * ((0x100 - RSDK.Cos256(entity->playerVelocity[playerID] >> 18)) >> 1);
                    if (entity->playerVelocity[playerID] < 0 || entity->playerVelocity[playerID] >= 0x2000000) {
                        player->tileCollisions = true;
                        player->collisionPlane = 1;
                        player->controlLock    = 16;
                        player->direction      = FLIP_NONE;
                        player->onGround       = true;
                        player->groundedStore  = true;
                        player->state          = Player_State_Ground;
                        player->groundVel      = -player->groundVel;
                        entity->activePlayers &= ~(1 << playerID);
                        LRZSpiral_Unknown1(player);
                        if (!entity->activePlayers)
                            entity->active = ACTIVE_BOUNDS;
                    }
                    else {
                        player->playerAnimator.frameID = entity->playerVelocity[playerID] / 0x2AAAAA;
                    }
                }
            }
            else {
                entity->activePlayers &= ~(1 << playerID);
            }
        }
        else if (player->position.x <= 0x8E0000 + entity->position.x && player->groundVel <= 0 && player->onGround) {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (player->groundVel > -0x40000)
                    player->groundVel = -0x40000;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_TWISTRUN, &player->playerAnimator, false, 0);
                player->playerAnimator.animationSpeed = 0;
                player->state                         = Player_State_None;
                player->nextAirState                  = StateMachine_None;
                player->nextGroundState               = StateMachine_None;
                player->tileCollisions                = false;
                entity->activePlayers |= 1 << playerID;
                entity->playerVelocity[playerID] = 0;
                entity->active                   = ACTIVE_NORMAL;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZSpiral_EditorDraw(void)
{
    RSDK_THIS(LRZSpiral);

    int32 height = entity->height + 64;
    switch (entity->type) {
        default: break;
        case 0: {
            height                = entity->height << 25 >> 19;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = (entity->height + 1) << 22;
            entity->hitbox.left   = -16;
            entity->hitbox.right  = 16;
            entity->hitbox.top    = -height;
            entity->hitbox.bottom = 128 - height;
            height <<= 1;
            break;
        }
        case 1:
            entity->direction     = FLIP_NONE;
            entity->updateRange.y = height << 16;
            entity->hitbox.left   = -112;
            entity->hitbox.top    = -180;
            entity->hitbox.right  = -104;
            entity->hitbox.bottom = -172;
            entity->updateRange.x = 0x800000;
            break;
        case 2:
            entity->direction     = FLIP_X;
            entity->updateRange.y = height << 16;
            entity->updateRange.x = 0x800000;
            entity->hitbox.left   = -146;
            entity->hitbox.top    = -180;
            entity->hitbox.right  = -138;
            entity->hitbox.bottom = -172;
            break;
    }

    DrawHelpers_DrawHitboxOutline(0xFF0000, entity->direction, entity->position.x, entity->position.y, &entity->hitbox);
    DrawHelpers_DrawRectOutline(0xFFFF00, entity->position.x, entity->position.y, entity->radius << 17, height << 16);
}

void LRZSpiral_EditorLoad(void) {}
#endif

void LRZSpiral_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_ENUM, radius);
}
