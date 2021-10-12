#include "SonicMania.h"

ObjectHangPoint *HangPoint;

void HangPoint_Update(void)
{
    RSDK_THIS(HangPoint);

    if (entity->length > 0) {
        bool32 flag = true;
        if (entity->activePlayers == 2) {
            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            if (player2->sidekick)
                flag = false;
        }

        for (int i = 0; i < 4; ++i) {
            if (entity->field_5C[i])
                flag = false;
        }

        if (entity->field_7C > entity->length) {
            if (entity->field_7C <= 0) {
                entity->activePlayers &= 0xF;
            }
            else {
                entity->velocity.y -= 0x1800;
                if (entity->velocity.y < -entity->field_80)
                    entity->velocity.y = -entity->field_80;

                entity->activePlayers |= 16 * entity->activePlayers;
                entity->field_7C += entity->velocity.y;
            }
        }
        else {
            if (!entity->activePlayers || !flag) {
                entity->active = ACTIVE_BOUNDS;
                if (entity->direction == FLIP_NONE) {
                    if (entity->field_7C > 0) {
                        entity->velocity.y -= 0x1000;
                        if (entity->velocity.y < -entity->field_80)
                            entity->velocity.y = -entity->field_80;

                        entity->field_7C += entity->velocity.y;
                    }
                }
                else if (entity->field_7C < entity->length) {
                    entity->velocity.y += 0x1000;
                    if (entity->velocity.y > entity->field_80)
                        entity->velocity.y = entity->field_80;

                    entity->field_7C += entity->velocity.y;
                    if (entity->field_7C > entity->length) {
                        entity->field_7C   = entity->length;
                        entity->velocity.y = 0;
                    }
                }
            }
            else {
                if (entity->direction) {
                    if (entity->field_7C <= 0) {
                        entity->activePlayers &= 0xF;
                    }
                    else {
                        entity->velocity.y -= 0x1800;
                        if (entity->velocity.y < -entity->field_80)
                            entity->velocity.y = -entity->field_80;

                        entity->activePlayers |= 16 * entity->activePlayers;
                        entity->field_7C += entity->velocity.y;
                    }
                }
                else if (entity->field_7C >= entity->length) {
                    entity->activePlayers &= 0xF;
                }
                else {
                    entity->velocity.y += 0x1800;
                    if (entity->velocity.y > entity->field_80)
                        entity->velocity.y = entity->field_80;

                    entity->field_7C += entity->velocity.y;
                    if (entity->field_7C > entity->length) {
                        entity->field_7C   = entity->length;
                        entity->velocity.y = 0;
                    }
                    entity->activePlayers |= (entity->activePlayers << 4);
                }
            }
        }

        if (entity->field_7C < 0) {
            entity->field_7C   = 0;
            entity->velocity.y = 0;
        }
    }

    int storeX = entity->position.x;
    int storeY = entity->position.y;
    entity->position.y += entity->field_7C;

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);

        if (entity->playerTimer[playerID] > 0)
            entity->playerTimer[playerID]--;
        if (!((1 << playerID) & entity->activePlayers)) {
            if (player->state != Player_State_None && !entity->playerTimer[playerID]) {
                Hitbox *playerHitbox = Player_GetHitbox(player);

                Hitbox hitbox;
                hitbox.top    = playerHitbox->top;
                hitbox.left   = playerHitbox->left;
                hitbox.right  = playerHitbox->right;
                hitbox.bottom = hitbox.top + 4;
                if (RSDK.CheckObjectCollisionTouchBox(entity, &HangPoint->hitbox, player, &hitbox)) {
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    player->onGround   = false;
                    player->angle      = 0;
                    player->rotation   = 0;
                    player->position.x = entity->position.x;
                    player->position.y = entity->position.y;
                    player->position.y += ((HangPoint->hitbox.top - playerHitbox->top) << 16)
                                          + (((HangPoint->hitbox.bottom - HangPoint->hitbox.top) << 15) & 0xFFFF0000);
                    player->tileCollisions     = false;
                    entity->field_5C[playerID] = 0;

                    if (!entity->activePlayers) {
                        if (entity->direction == FLIP_X) {
                            if (entity->field_7C <= entity->length)
                                entity->velocity.y = 0x20000;
                        }
                        else {
                            entity->velocity.y = 0;
                        }
                    }
                    entity->activePlayers |= (1 << playerID);
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, false, 0);
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->state           = Player_State_None;
                    player->abilityValue1   = RSDK.Rand(0, 2);
                    RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                    entity->active = ACTIVE_NORMAL;
                }
            }
        }
        else {
            if (player->state == Player_State_Hit) {
                player->tileCollisions = true;
                entity->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
                entity->field_5C[playerID] = 0;
                if (player->left || player->right || player->down || player->state == Player_State_Hit)
                    entity->playerTimer[playerID] = 64;
                else
                    entity->playerTimer[playerID] = 16;
            }
            else {
                if (player->state != Player_State_None || entity->playerTimer[playerID]) {
                    entity->activePlayers &= ~(1 << playerID);
                    if (player->objectID == Player->objectID && Player_CheckValidState(player)) {
                        player->tileCollisions = true;
                    }
                }
                else {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    player->velocity.x   = 0;
                    player->velocity.y   = 0;
                    player->groundVel    = 0;
                    player->angle        = 0;
                    player->rotation     = 0;
                    player->position.x   = entity->position.x;
                    player->position.y   = entity->position.y;
                    player->position.y += (((HangPoint->hitbox.bottom - HangPoint->hitbox.top) << 15) & 0xFFFF0000)
                                          + ((HangPoint->hitbox.top - playerHitbox->top) << 16);
                    if (!entity->field_5C[playerID]) {
                        if (player->left)
                            player->direction = FLIP_X;
                        if (player->right)
                            player->direction = FLIP_NONE;

                        if (player->jumpPress) {
                            player->velocity.y = -0x40000;
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                            player->playerAnimator.animationSpeed = 48;
                            player->jumpAbility                   = 0;
                            player->jumpAbilityTimer              = 1;
                            player->state                         = Player_State_Air;
                            player->tileCollisions                = true;
                            entity->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
                            if (player->left || player->right || player->down || player->state == Player_State_Hit) {
                                entity->playerTimer[playerID] = 64;
                                HangPoint_Unknown1(entity, player, playerID);
                            }
                            else {
                                entity->playerTimer[playerID] = 16;
                                HangPoint_Unknown1(entity, player, playerID);
                            }
                        }
                        else if ((!((1 << (playerID + 4)) & entity->activePlayers)
                                  && (entity->direction != FLIP_NONE || entity->field_7C >= entity->length)
                                  && (entity->direction != FLIP_X || entity->field_7C <= 0))
                                 || player->sidekick) {
                            bool32 flag = false;
                            if (player->left) {
                                foreach_active(HangPoint, point)
                                {
                                    if (entity != point) {
                                        int distance = entity->position.x - point->position.x;
                                        if (distance < 0x220000 && distance > 0) {
                                            if (abs(entity->position.y - point->position.y - point->field_7C) < 0x40000) {
                                                entity->activePlayers &= ~(1 << playerID);
                                                entity->playerTimer[playerID] = 16;
                                                point->activePlayers |= 1 << playerID;
                                                point->field_5C[playerID]    = 0x200000;
                                                point->playerTimer[playerID] = 0;
#if RETRO_USE_PLUS
                                                if (player->characterID == ID_RAY) {
                                                    if (player->abilityValue1)
                                                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANGMOVE, &player->playerAnimator, false, 6);
                                                    else
                                                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANGMOVE, &player->playerAnimator, false, 1);
                                                }
                                                else {
#endif
                                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANGMOVE, &player->playerAnimator, false, 3);
#if RETRO_USE_PLUS
                                                }
#endif

                                                player->position.x = point->position.x;
                                                if (RSDK.GetEntityID(point) < RSDK_sceneInfo->entitySlot)
                                                    HangPoint_Unknown1(point, player, playerID);
                                                HangPoint_Unknown1(entity, player, playerID);
                                                flag = true;
                                                foreach_break;
                                            }
                                        }
                                    }
                                }
                            }
                            else if (player->right) {
                                foreach_active(HangPoint, point)
                                {
                                    if (entity != point) {
                                        int distance = point->position.x - entity->position.x;
                                        if (distance < 0x220000 && distance > 0) {
                                            if (abs(entity->position.y - point->position.y - point->field_7C) < 0x40000) {
                                                entity->activePlayers &= ~(1 << playerID);
                                                entity->playerTimer[playerID] = 16;
                                                point->activePlayers |= 1 << playerID;
                                                point->field_5C[playerID]    = -0x200000;
                                                point->playerTimer[playerID] = 0;
#if RETRO_USE_PLUS
                                                if (player->characterID == ID_RAY) {
                                                    if (!player->abilityValue1) {
                                                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANGMOVE, &player->playerAnimator, false, 0);
                                                    }
                                                    else {
                                                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANGMOVE, &player->playerAnimator, false, 6);
                                                    }
                                                }
                                                else {
#endif
                                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANGMOVE, &player->playerAnimator, false, 3);
#if RETRO_USE_PLUS
                                                }
#endif

                                                player->position.x = point->position.x;
                                                if (RSDK.GetEntityID(point) < RSDK_sceneInfo->entitySlot)
                                                    HangPoint_Unknown1(point, player, playerID);
                                                HangPoint_Unknown1(entity, player, playerID);
                                                flag = true;
                                                foreach_break;
                                            }
                                        }
                                    }
                                }
                            }
                            if (!flag) {
#if RETRO_USE_PLUS
                                if (player->characterID == ID_RAY && player->abilityValue1 == 1)
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_DROPDASH, &player->playerAnimator, false, 0);
                                else
#endif
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, false, 0);
                                HangPoint_Unknown1(entity, player, playerID);
                            }
                        }
                        else {
#if RETRO_USE_PLUS
                            if (player->characterID == ID_RAY && player->abilityValue1 == 1)
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_DROPDASH, &player->playerAnimator, false, 0);
                            else
#endif
                                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HANG, &player->playerAnimator, false, 0);
                            HangPoint_Unknown1(entity, player, playerID);
                        }
                    }
                    else {
                        HangPoint_Unknown1(entity, player, playerID);
                    }
                }
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void HangPoint_LateUpdate(void) {}

void HangPoint_StaticUpdate(void) {}

void HangPoint_Draw(void)
{
    RSDK_THIS(HangPoint);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y += entity->field_7C;
    RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);
    if (entity->length) {
        HangPoint->animator.frameID = 1;
        int length                  = (entity->field_7C >> 16) & 0xFF00;

        SpriteFrame *frame = RSDK.GetFrame(HangPoint->aniFrames, 0, HangPoint->animator.frameID);

        int extraLength = (entity->field_7C >> 16) & 0x00FF;
        frame->pivotY   = -(entity->field_7C >> 16);
        frame->height   = extraLength;
        frame->sprY     = 257 - extraLength;
        RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);

        frame->height = 256;
        frame->sprY   = 1;
        while (length >= 256) {
            frame->pivotY = -length;
            RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);
            length -= 256;
        }
        HangPoint->animator.frameID = 0;
    }
}

void HangPoint_Create(void *data)
{
    RSDK_THIS(HangPoint);
    if (!entity->speed)
        entity->speed = 2;

    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x400000;
    entity->visible       = true;
    entity->updateRange.y = (entity->length + 64) << 16;
    entity->drawOrder     = Zone->drawOrderLow;
    if (!RSDK_sceneInfo->inEditor) {
        entity->length   = entity->length << 16;
        entity->field_80 = entity->speed << 15;
        if (entity->direction == FLIP_X)
            entity->field_7C = entity->length;
    }
}

void HangPoint_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        HangPoint->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangPoint.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(HangPoint->aniFrames, 0, &HangPoint->animator, true, 0);
    Hitbox *hitbox           = RSDK.GetHitbox(&HangPoint->animator, 0);
    HangPoint->hitbox.top    = hitbox->top;
    HangPoint->hitbox.left   = hitbox->left;
    HangPoint->hitbox.bottom = hitbox->bottom;
    HangPoint->hitbox.right  = hitbox->right;
}

void HangPoint_Unknown1(EntityHangPoint *entity, void *p, int playerID)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (entity->field_5C[playerID]) {
        if (entity->field_5C[playerID] < 0) {
            entity->field_5C[playerID] += 0x15556;
            if (entity->field_5C[playerID] >= 0) {
                entity->field_5C[playerID] = 0;
                RSDK.PlaySfxPlayer->sfx_Grab, false, 255);
                player->abilityValue1 ^= 1;
            }
        }
        else {
            entity->field_5C[playerID] -= 0x15556;
            if (entity->field_5C[playerID] <= 0) {
                entity->field_5C[playerID] = 0;
                RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                player->abilityValue1 ^= 1;
            }
        }

        player->position.x += entity->field_5C[playerID] & 0xFFFF0000;
        if (player->jumpPress) {
            player->velocity.y = -0x40000;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
            player->jumpAbility                   = 0;
            player->playerAnimator.animationSpeed = 48;
            player->jumpAbilityTimer              = 1;
            player->state                         = Player_State_Air;
            player->tileCollisions                = true;
            entity->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
            entity->field_5C[playerID] = 0;
            if (player->left || player->right || player->down || player->state == Player_State_Hit)
                entity->playerTimer[playerID] = 64;
            else
                entity->playerTimer[playerID] = 16;
        }
    }
}

void HangPoint_EditorDraw(void) {}

void HangPoint_EditorLoad(void) {}

void HangPoint_Serialize(void)
{
    RSDK_EDITABLE_VAR(HangPoint, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(HangPoint, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(HangPoint, VAR_UINT8, direction);
}
