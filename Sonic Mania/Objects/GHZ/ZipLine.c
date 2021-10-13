#include "SonicMania.h"

ObjectZipLine *ZipLine;

void ZipLine_Update(void)
{
    RSDK_THIS(ZipLine);
    StateMachine_Run(entity->state);

    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->handlePos.x;
    entity->position.y = entity->handlePos.y;
    foreach_active(Player, player)
    {
        int pid = RSDK.GetEntityID(player);
        if (entity->grabDelay[pid])
            entity->grabDelay[pid]--;

        if ((1 << pid) & entity->activePlayers) {
            if (player->objectID == Player->objectID && Player_CheckValidState(player)) {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                if (player->state != Player_State_Hit) {
                    if (player->velocity.x) {
                        if ((uint8)(entity->angle - 0x40) < 0x80)
                            entity->groundVel = -player->groundVel;
                        else
                            entity->groundVel = player->groundVel;

                        if (entity->groundVel > 0xA0000)
                            entity->groundVel = 0xA0000;
                        else if (entity->groundVel < -0xA0000)
                            entity->groundVel = -0xA0000;

                        entity->velocity.x = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
                        entity->velocity.y = entity->groundVel * RSDK.Sin256(entity->angle) >> 8;
                        player->velocity.x = 0;
                        player->velocity.y = 0;
                        player->groundVel  = 0;
                        player->angle      = 0;
                        player->rotation   = 0;
                    }
                    int prevX          = player->position.x;
                    player->position.x = entity->position.x;
                    player->position.y = entity->position.y;
                    player->position.y +=
                        (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000) + ((ZipLine->hitbox.top - playerHitbox->top) << 16);
                    if (abs(prevX - entity->position.x) <= 0x100000) {
                        if (!entity->grabDelay[pid] && player->jumpPress) {
                            player->velocity.y       = -0x40000;
                            player->jumpAbilityTimer = 1;
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, 0, 0);
                            player->playerAnimator.animationSpeed = 48;
                            player->state                         = Player_State_Air;
                            entity->grabDelay[pid]                = 60;
                            entity->activePlayers &= ~(1 << pid);
                            player->onGround       = false;
                            player->groundedStore  = false;
                            player->tileCollisions = true;
                        }
                    }
                    else {
                        player->state          = Player_State_Air;
                        entity->grabDelay[pid] = 60;
                        entity->activePlayers &= ~(1 << pid);
                        player->onGround       = false;
                        player->groundedStore  = false;
                        player->tileCollisions = true;
                    }
                }
                else {
                    entity->grabDelay[pid] = 60;
                    entity->activePlayers &= ~(1 << pid);
                    player->onGround       = false;
                    player->groundedStore  = 0;
                    player->tileCollisions = true;
                }
            }
            else {
                entity->activePlayers &= ~(1 << pid);
                if (player->state != Player_State_Die) {
                    player->tileCollisions = true;
                }
            }
        }
        else {
            if (!entity->grabDelay[pid] && player->state != Player_State_None && !player->down) {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                Hitbox otherHitbox;
                otherHitbox.top    = playerHitbox->top - 4;
                otherHitbox.left   = playerHitbox->left;
                otherHitbox.right  = playerHitbox->right;
                otherHitbox.bottom = otherHitbox.top + 8;
                if (RSDK.CheckObjectCollisionTouchBox(entity, &ZipLine->hitbox, player, &otherHitbox)) {
                    if (player->sidekick || entity->state == ZipLine_Unknown4) {
                        entity->activePlayers |= 1 << pid;
                        player->onGround      = false;
                        player->groundedStore = 0;
                        player->velocity.x    = 0;
                        player->velocity.y    = 0;
                        player->groundVel     = 0;
                        player->angle         = 0;
                        player->rotation      = 0;
                        player->position.x    = entity->position.x;
                        player->position.y    = entity->position.y;
                        player->position.y +=
                            ((ZipLine->hitbox.top - playerHitbox->top) << 16) + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                        player->tileCollisions = false;
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, true, 0);
                        player->state           = Player_State_None;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        RSDK.PlaySfx(Player->sfx_Grab, 0, 0xFF);
                        entity->grabDelay[pid] = 15;
                        entity->active         = ACTIVE_NORMAL;
                    }
                    else if (!entity->state) {
                        entity->groundVel = player->groundVel;
                        if ((uint8)(entity->angle - 64) < 0x80)
                            entity->groundVel = -player->groundVel;

                        if (entity->groundVel > 0xA0000)
                            entity->groundVel = 0xA0000;
                        else if (entity->groundVel < -0xA0000)
                            entity->groundVel = -0xA0000;

                        entity->velocity.x = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
                        entity->velocity.y = entity->groundVel * RSDK.Sin256(entity->angle) >> 8;
                        if (entity->angle & 0x7F) {
                            if ((uint8)entity->angle < 0x80) {
                                if (entity->handlePos.x == entity->startPos.x) {
                                    if (entity->velocity.y < 0) {
                                        entity->velocity.x = 0;
                                        entity->velocity.y = 0;
                                        entity->groundVel  = 0;
                                    }
                                    entity->state = ZipLine_Unknown4;
                                }

                                if (entity->handlePos.x == entity->endPos.x && entity->velocity.y < 0) {
                                    entity->state = ZipLine_Unknown4;
                                    entity->activePlayers |= 1 << pid;
                                    player->onGround      = false;
                                    player->groundedStore = 0;
                                    player->velocity.x    = 0;
                                    player->velocity.y    = 0;
                                    player->groundVel     = 0;
                                    player->angle         = 0;
                                    player->rotation      = 0;
                                    player->position.x    = entity->position.x;
                                    player->position.y    = entity->position.y;
                                    player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                          + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                    player->tileCollisions = false;
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, true, 0);
                                    player->state           = Player_State_None;
                                    player->nextAirState    = StateMachine_None;
                                    player->nextGroundState = StateMachine_None;
                                    RSDK.PlaySfx(Player->sfx_Grab, 0, 0xFF);
                                    entity->grabDelay[pid] = 15;
                                    entity->active         = ACTIVE_NORMAL;
                                }
                            }
                            else {
                                if (entity->handlePos.x == entity->endPos.x) {
                                    if (entity->velocity.y < 0) {
                                        entity->velocity.x = 0;
                                        entity->velocity.y = 0;
                                        entity->groundVel  = 0;
                                    }
                                    entity->state = ZipLine_Unknown4;
                                }

                                if (entity->handlePos.x == entity->startPos.x && entity->velocity.y < 0) {
                                    entity->state = ZipLine_Unknown4;
                                    entity->activePlayers |= 1 << pid;
                                    player->onGround      = false;
                                    player->groundedStore = 0;
                                    player->velocity.x    = 0;
                                    player->velocity.y    = 0;
                                    player->groundVel     = 0;
                                    player->angle         = 0;
                                    player->rotation      = 0;
                                    player->position.x    = entity->position.x;
                                    player->position.y    = entity->position.y;
                                    player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                          + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                    player->tileCollisions = false;
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, true, 0);
                                    player->state           = Player_State_None;
                                    player->nextAirState    = StateMachine_None;
                                    player->nextGroundState = StateMachine_None;
                                    RSDK.PlaySfx(Player->sfx_Grab, 0, 0xFF);
                                    entity->grabDelay[pid] = 15;
                                    entity->active         = ACTIVE_NORMAL;
                                }
                            }
                        }
                        else {
                            if (entity->groundVel) {
                                if (entity->groundVel < 0) {
                                    if (entity->handlePos.x != entity->startPos.x) {
                                        entity->state = ZipLine_Unknown4;
                                        entity->activePlayers |= 1 << pid;
                                        player->onGround      = false;
                                        player->groundedStore = 0;
                                        player->velocity.x    = 0;
                                        player->velocity.y    = 0;
                                        player->groundVel     = 0;
                                        player->angle         = 0;
                                        player->rotation      = 0;
                                        player->position.x    = entity->position.x;
                                        player->position.y    = entity->position.y;
                                        player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                              + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                        player->tileCollisions = false;
                                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, true, 0);
                                        player->state           = Player_State_None;
                                        player->nextAirState    = StateMachine_None;
                                        player->nextGroundState = StateMachine_None;
                                        RSDK.PlaySfx(Player->sfx_Grab, 0, 0xFF);
                                        entity->grabDelay[pid] = 15;
                                        entity->active         = ACTIVE_NORMAL;
                                    }
                                }
                                else if (entity->handlePos.x != entity->endPos.x) {
                                    entity->state = ZipLine_Unknown4;
                                    entity->activePlayers |= 1 << pid;
                                    player->onGround      = false;
                                    player->groundedStore = 0;
                                    player->velocity.x    = 0;
                                    player->velocity.y    = 0;
                                    player->groundVel     = 0;
                                    player->angle         = 0;
                                    player->rotation      = 0;
                                    player->position.x    = entity->position.x;
                                    player->position.y    = entity->position.y;
                                    player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                          + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                    player->tileCollisions = false;
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, true, 0);
                                    player->state           = Player_State_None;
                                    player->nextAirState    = StateMachine_None;
                                    player->nextGroundState = StateMachine_None;
                                    RSDK.PlaySfx(Player->sfx_Grab, 0, 0xFF);
                                    entity->grabDelay[pid] = 15;
                                    entity->active         = ACTIVE_NORMAL;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void ZipLine_LateUpdate(void) {}

void ZipLine_StaticUpdate(void) {}

void ZipLine_Draw(void)
{
    RSDK_THIS(ZipLine);
    RSDK.DrawLine(entity->startPos.x, entity->startPos.y, entity->endPos.x, entity->endPos.y, 0x6060A0, 0x00, INK_NONE, false);
    RSDK.DrawLine(entity->startPos.x, entity->startPos.y + 0x10000, entity->endPos.x, entity->endPos.y + 0x10000, 0x303070, 0x00, INK_NONE, false);

    entity->animator.frameID = 0;
    RSDK.DrawSprite(&entity->animator, &entity->handlePos, false);
    entity->animator.frameID = 1;

    RSDK.DrawSprite(&entity->animator, &entity->startPos, false);
    RSDK.DrawSprite(&entity->animator, &entity->endPos, false);
}

void ZipLine_Create(void *data)
{
    RSDK_THIS(ZipLine);
    entity->active  = ACTIVE_BOUNDS;
    entity->visible = true;
    entity->drawFX  = FX_FLIP;
    RSDK.SetSpriteAnimation(ZipLine->aniFrames, 0, &entity->animator, true, 0);
    entity->drawOrder   = Zone->playerDrawLow;
    entity->handlePos.x = entity->position.x;
    entity->handlePos.y = entity->position.y;
    entity->startPos.x  = entity->position.x;
    entity->startPos.y  = entity->position.y;
    entity->endPos.x    = entity->position.x + (entity->length << 8) * RSDK.Cos256(entity->angle);
    entity->endPos.y    = entity->position.y + (entity->length << 8) * RSDK.Sin256(entity->angle);

    if (!RSDK_sceneInfo->inEditor) {
        entity->position.x += (entity->endPos.x - entity->startPos.x) >> 1;
        entity->position.y += (entity->endPos.y - entity->startPos.y) >> 1;
        entity->updateRange.x = (abs(entity->endPos.x - entity->startPos.x) >> 1) + 0x400000;
        entity->updateRange.y = (abs(entity->endPos.y - entity->startPos.y) >> 1) + 0x400000;
        Vector2 pos           = ZipLine_Unknown3();
        entity->field_98.x    = pos.x;
        entity->field_98.y    = pos.y;
    }
}

void ZipLine_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        ZipLine->aniFrames = RSDK.LoadSpriteAnimation("GHZ/ZipLine.bin", SCOPE_STAGE);
    ZipLine->hitbox.top    = 0;
    ZipLine->hitbox.left   = -8;
    ZipLine->hitbox.bottom = 24;
    ZipLine->hitbox.right  = 8;

    Zone_AddCallback(ZipLine_Unknown1);
}

void ZipLine_Unknown1(void)
{
    foreach_active(ZipLine, zipline)
    {
#if RETRO_USE_PLUS
        if ((1 << Zone->playerID) & zipline->activePlayers)
            Zone->playerFlags[Zone->playerID] = false;
#else
        // if (zipline->activePlayers)
        //     Zone->playerFlags = false;
#endif
    }
}

void ZipLine_Unknown2(void)
{
    RSDK_THIS(ZipLine);

    foreach_active(Player, player)
    {
        int pid = RSDK.GetEntityID(player);
        if ((1 << pid) & entity->activePlayers) {
            entity->grabDelay[pid] = 60;
            player->velocity.y     = entity->velocity.y;
            player->velocity.x     = entity->velocity.x;
            player->groundVel      = player->velocity.x;
            entity->activePlayers &= ~(1 << pid);
            player->onGround       = false;
            player->groundedStore  = false;
            player->tileCollisions = true;
            player->state          = Player_State_Air;
        }
    }
}

Vector2 ZipLine_Unknown3(void)
{
    RSDK_THIS(ZipLine);
    EntityZipLine *endMarker = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, ZipLine);
    Vector2 result;
    result.x = 0;
    result.y = 0;

    if (endMarker->objectID == ZipLine->objectID) {
        Hitbox thisHitbox, otherHitbox;

        if (entity->startPos.y >= entity->endPos.y) {
            otherHitbox.top    = (entity->endPos.y - entity->position.y) >> 16;
            otherHitbox.bottom = (entity->startPos.y - entity->position.y) >> 16;
        }
        else {
            otherHitbox.top    = (entity->startPos.y - entity->position.y) >> 16;
            otherHitbox.bottom = (entity->endPos.y - entity->position.y) >> 16;
        }

        if (entity->startPos.x >= entity->endPos.x) {
            otherHitbox.left  = (entity->endPos.x - entity->position.y) >> 16;
            otherHitbox.right = (entity->startPos.x - entity->position.y) >> 16;
        }
        else {
            otherHitbox.left  = (entity->startPos.x - entity->position.x) >> 16;
            otherHitbox.right = (entity->endPos.x - entity->position.x) >> 16;
        }

        if (endMarker->startPos.y >= endMarker->endPos.y) {
            thisHitbox.top    = (endMarker->endPos.y - endMarker->position.y) >> 16;
            thisHitbox.bottom = (endMarker->startPos.y - endMarker->position.y) >> 16;
        }
        else {
            thisHitbox.top    = (endMarker->startPos.y - endMarker->position.y) >> 16;
            thisHitbox.bottom = (endMarker->endPos.y - endMarker->position.y) >> 16;
        }

        if (endMarker->startPos.x >= endMarker->endPos.x) {
            thisHitbox.left  = (endMarker->endPos.x - endMarker->position.y) >> 16;
            thisHitbox.right = (endMarker->startPos.x - endMarker->position.y) >> 16;
        }
        else {
            thisHitbox.left  = (endMarker->startPos.x - endMarker->position.x) >> 16;
            thisHitbox.right = (endMarker->endPos.x - endMarker->position.x) >> 16;
        }

        otherHitbox.left -= 8;
        otherHitbox.top -= 8;
        otherHitbox.right += 8;
        otherHitbox.bottom += 8;
        thisHitbox.left -= 8;
        thisHitbox.top -= 8;
        thisHitbox.right += 8;
        thisHitbox.bottom += 8;
        if (RSDK.CheckObjectCollisionTouchBox(endMarker, &thisHitbox, entity, &otherHitbox)) {
            int distX1 = (entity->startPos.x >> 17) - (entity->endPos.x >> 17);
            int distY1 = (entity->endPos.y >> 17) - (entity->startPos.y >> 17);
            int distX2 = (endMarker->startPos.x >> 17) - (endMarker->endPos.x >> 17);
            int distY2 = (endMarker->endPos.y >> 17) - (endMarker->startPos.y >> 17);

            int val1      = distX1 * (entity->startPos.y >> 17) + distY1 * (entity->startPos.x >> 17);
            int val2      = distX2 * (endMarker->startPos.y >> 17) + distY2 * (endMarker->startPos.x >> 17);
            float divisor = (float)(distY1 * distX2 - distX1 * distY2);
            if (divisor != 0.0f) {
                endMarker->handlePos.x = -0x100000;
                result.x               = (int)((float)(distX2 * val1 - distX1 * val2) / divisor) << 17;
                result.y               = (int)((float)(distY1 * val2 - distY2 * val1) / divisor) << 17;
            }
        }
    }
    return result;
}
void ZipLine_Unknown4(void)
{
    RSDK_THIS(ZipLine);
    entity->groundVel += (RSDK.Sin256(entity->angle) << 14 >> 8);
    if (entity->groundVel >= 0xA0000)
        entity->groundVel = 0xA0000;

    entity->velocity.x = entity->groundVel * RSDK.Cos256(entity->angle) >> 8;
    entity->velocity.y = entity->groundVel * RSDK.Sin256(entity->angle) >> 8;

    if (entity->field_98.x) {
        int entX           = entity->position.x;
        int entY           = entity->position.y;
        entity->position.x = entity->handlePos.x;
        entity->position.y = entity->handlePos.y;

        Hitbox otherHitbox;
        otherHitbox.top    = ((entity->field_98.y - entity->position.y - (entity->velocity.y >> 1)) >> 16) + 8;
        otherHitbox.bottom = (((entity->velocity.y >> 1) + (entity->field_98.y - entity->position.y)) >> 16) + 16;

        if (entity->velocity.x >= 0) {
            otherHitbox.left  = (entity->field_98.x - entity->position.x) >> 16;
            otherHitbox.right = ((entity->field_98.x + entity->velocity.x - entity->position.x) >> 16) + 5;
        }
        else {
            otherHitbox.left  = (((entity->field_98.x - entity->position.x) - entity->velocity.x) >> 16) - 5;
            otherHitbox.right = (entity->field_98.x - entity->position.x) >> 16;
        }

        if (RSDK.CheckObjectCollisionTouchBox(entity, &ZipLine->hitbox, entity, &otherHitbox)) {
            EntityZipLine *endMarker = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, ZipLine);
            endMarker->handlePos.x   = entity->field_98.x;
            endMarker->handlePos.y   = entity->field_98.y;
            endMarker->onGround      = true;
            endMarker->activePlayers = entity->activePlayers;
            endMarker->groundVel     = entity->groundVel;
            endMarker->state         = ZipLine_Unknown4;
            entity->position.x       = entX;
            entity->position.y       = entY;
            entity->activePlayers    = 0;
            entity->groundVel        = 0;
            entity->handlePos.x      = -0x100000;
            entity->state            = StateMachine_None;
            foreach_active(Player, player)
            {
                if ((1 << RSDK.GetEntityID(player)) & endMarker->activePlayers) {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    player->velocity.x   = 0;
                    player->velocity.y   = 0;
                    player->groundVel    = 0;
                    player->angle        = 0;
                    player->rotation     = 0;
                    player->position.x   = endMarker->handlePos.x;
                    player->position.y   = endMarker->handlePos.y;
                    player->position.y +=
                        (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000) + ((ZipLine->hitbox.top - playerHitbox->top) << 16);
                }
            }
            return;
        }
        entity->position.x = entX;
        entity->position.y = entY;
    }

    entity->handlePos.x += entity->velocity.x;
    entity->handlePos.y += entity->velocity.y;
    if (entity->groundVel < 0) {
        if (entity->velocity.x < 0) {
            if (entity->handlePos.x < entity->startPos.x)
                entity->handlePos.x = entity->startPos.x;

            if (entity->velocity.y < 0) {
                if (entity->handlePos.y < entity->startPos.y)
                    entity->handlePos.y = entity->startPos.y;
            }
            if (entity->velocity.y > 0) {
                if (entity->handlePos.y > entity->startPos.y)
                    entity->handlePos.y = entity->startPos.y;
            }

            if (entity->handlePos.x == entity->startPos.x && entity->handlePos.y == entity->startPos.y) {
                entity->groundVel = 0;
                if (entity->startPos.y <= entity->endPos.y || entity->onGround) {
                    entity->onGround = true;
                    entity->active   = ACTIVE_BOUNDS;
                    entity->state    = StateMachine_None;
                    ZipLine_Unknown2();
                }
            }
        }
        else if (entity->velocity.x > 0) {
            if (entity->handlePos.x > entity->startPos.x)
                entity->handlePos.x = entity->startPos.x;

            if (entity->velocity.y < 0) {
                if (entity->handlePos.y < entity->startPos.y)
                    entity->handlePos.y = entity->startPos.y;
            }
            if (entity->velocity.y > 0) {
                if (entity->handlePos.y > entity->startPos.y)
                    entity->handlePos.y = entity->startPos.y;
            }

            if (entity->handlePos.x == entity->startPos.x && entity->handlePos.y == entity->startPos.y) {
                entity->groundVel = 0;
                if (entity->startPos.y <= entity->endPos.y || entity->onGround) {
                    entity->onGround = true;
                    entity->active   = ACTIVE_BOUNDS;
                    entity->state    = StateMachine_None;
                    ZipLine_Unknown2();
                }
            }
        }
    }
    else if (entity->groundVel > 0) {
        if (entity->velocity.x > 0) {
            if (entity->handlePos.x > entity->endPos.x) {
                entity->handlePos.x = entity->endPos.x;
            }
        }
        else if (entity->velocity.x < 0) {
            entity->endPos.x = entity->endPos.x;
            if (entity->handlePos.x < entity->endPos.x) {
                entity->handlePos.x = entity->endPos.x;
            }
        }

        if (entity->velocity.y < 0) {
            if (entity->handlePos.y < entity->endPos.y)
                entity->handlePos.y = entity->endPos.y;
        }
        else if (entity->velocity.y > 0) {
            if (entity->handlePos.y > entity->endPos.y)
                entity->handlePos.y = entity->endPos.y;
        }

        if (entity->handlePos.x == entity->endPos.x && entity->handlePos.y == entity->endPos.y) {
            entity->groundVel = 0;
            if (entity->endPos.y >= entity->startPos.y || entity->onGround) {
                entity->onGround = true;
                entity->active   = ACTIVE_BOUNDS;
                entity->state    = StateMachine_None;
                ZipLine_Unknown2();
            }
        }
    }
}

void ZipLine_EditorDraw(void) {}

void ZipLine_EditorLoad(void) {}

void ZipLine_Serialize(void)
{
    RSDK_EDITABLE_VAR(ZipLine, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(ZipLine, VAR_ENUM, length);
}
