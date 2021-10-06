#include "SonicMania.h"

ObjectHangConveyor *HangConveyor;

void HangConveyor_Update(void)
{
    RSDK_THIS(HangConveyor);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        HangConveyor_Unknown2();
        entity->active = ACTIVE_NORMAL;
    }
    else {
        entity->field_A4[0].x = 0;
        entity->field_A4[0].y = 0;
        entity->field_A4[1].x = 0;
        entity->field_A4[1].y = 0;
        entity->field_A4[2].x = 0;
        entity->field_A4[2].y = 0;
        entity->field_A4[3].x = 0;
        entity->field_A4[3].y = 0;
        entity->active        = ACTIVE_BOUNDS;
    }
}

void HangConveyor_LateUpdate(void) {}

void HangConveyor_StaticUpdate(void) {}

void HangConveyor_Draw(void) { HangConveyor_DrawSprites(); }

void HangConveyor_Create(void *data)
{
    RSDK_THIS(HangConveyor);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    HangConveyor_Unknown1();
    entity->updateRange.x += (entity->hitbox1.right - entity->hitbox1.left) << 16;
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(HangConveyor->aniFrames, 2, &entity->animator3, true, 0);
}

void HangConveyor_StageLoad(void) { HangConveyor->aniFrames = RSDK.LoadSpriteAnimation("HCZ/HangConveyor.bin", SCOPE_STAGE); }

void HangConveyor_DrawSprites(void)
{
    RSDK_THIS(HangConveyor);
    Vector2 drawPos;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += -0x80000 * (entity->length != 1 ? entity->length - 1 : 0);
    int dirStore = entity->direction;

    for (int i = 0; i < entity->length; ++i) {
        entity->direction = dirStore != FLIP_NONE;
        drawPos.y         = entity->position.y - 0x150000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->direction = dirStore == FLIP_NONE;
        drawPos.y         = entity->position.y + 0x150000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        drawPos.x += 0x100000;
    }

    entity->direction = dirStore != FLIP_NONE;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;

    if (dirStore) {
        int len = entity->length + 3;
        if (entity->length == 1)
            len = 4;
        drawPos.x += len << 19;
    }
    else {
        int len = entity->length + 3;
        if (entity->length == 1)
            len = 4;
        drawPos.x += -0x80000 * len;
    }
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    entity->direction = dirStore == FLIP_NONE;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y;

    if (dirStore) {
        int len = entity->length + 3;
        if (entity->length == 1)
            len = 4;
        drawPos.x += -0x80000 * len;
    }
    else {
        int len = entity->length + 3;
        if (entity->length == 1)
            len = 4;
        drawPos.x += len << 19;
    }
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->direction = dirStore;
}

void HangConveyor_Unknown1(void)
{
    RSDK_THIS(HangConveyor);

    entity->hitbox3.left   = -8 * (entity->length != -6 ? entity->length + 6 : 0);
    entity->hitbox3.top    = -24;
    entity->hitbox3.right  = 8 * (entity->length != -6 ? entity->length + 6 : 0);
    entity->hitbox3.bottom = 24;

    entity->hitbox1.left   = -8 * (entity->length != -2 ? entity->length + 2 : 0);
    entity->hitbox1.top    = -26;
    entity->hitbox1.right  = 8 * (entity->length != -2 ? entity->length + 2 : 0);
    entity->hitbox1.bottom = -16;

    entity->field_64[0] = entity->position;
    entity->field_64[0].y -= 0x150000;
    entity->field_64[0].x += entity->hitbox1.left << 16;

    entity->field_64[1] = entity->position;
    entity->field_64[1].y -= 0x150000;
    entity->field_64[1].x += entity->hitbox1.right << 16;

    entity->hitbox2.left   = -4 - 8 * (entity->length != -2 ? entity->length + 2 : 0);
    entity->hitbox2.top    = 16;
    entity->hitbox2.right  = 8 * (entity->length != -2 ? entity->length + 2 : 0) + 4;
    entity->hitbox2.bottom = 26;

    entity->field_64[2] = entity->position;
    entity->field_64[2].x += entity->hitbox2.left << 16;
    entity->field_64[2].y += 0x150000;

    entity->field_64[3] = entity->position;
    entity->field_64[3].y += 0x150000;
    entity->field_64[3].x += entity->hitbox2.right << 16;
}

void HangConveyor_Unknown2(void)
{
    RSDK_THIS(HangConveyor);

    foreach_active(Player, player)
    {
        if (Player_CheckValidState(player)) {
            int playerID = RSDK.GetEntityID(player);
            int posY     = player->position.y;
            if (player->playerAnimator.animationID != ANI_POLESWINGH && player->playerAnimator.animationID != ANI_SHIMMYMOVE)
                posY -= 0x180000;

            int prevX                    = entity->field_A4[playerID].x;
            int prevY                    = entity->field_A4[playerID].y;
            entity->field_A4[playerID].x = player->position.x;
            entity->field_A4[playerID].y = posY;

            bool32 collided1 = true;
            if (abs(player->position.x - prevX) < 0x800000 && abs(posY - prevY) < 0x800000 && (prevX || prevY)) {
                bool32 cFlag = MathHelpers_Unknown12(player->position.x, posY, prevX, prevY, entity->field_64[0].x, entity->field_64[0].y,
                                                     entity->field_64[1].x, entity->field_64[1].y);
                if (entity->field_84[playerID] || !cFlag) {
                    collided1 = false;
                }

                bool32 collided2 = !entity->field_94[playerID]
                                   && MathHelpers_Unknown12(player->position.x, posY, prevX, prevY, entity->field_64[2].x, entity->field_64[2].y,
                                                            entity->field_64[3].x, entity->field_64[3].y);

                if (entity->field_84[playerID] > 0)
                    entity->field_84[playerID]--;
                if (entity->field_94[playerID] > 0)
                    entity->field_94[playerID]--;

                if (!((1 << playerID) & entity->activePlayers1)) {
                    if (!(entity->activePlayers4 & (1 << playerID))) {
                        if ((collided1 || collided2) && player->state != Player_State_None && player->playerAnimator.animationID != ANI_HURT) {
                            entity->activePlayers1 |= (1 << playerID);
                            entity->activePlayers4 |= (1 << playerID);
                            if (collided1) {
                                entity->activePlayers2 |= 1 << playerID;
                                player->position.y = entity->position.y - 0x150000;
                            }
                            if (collided2) {
                                entity->activePlayers3 |= 1 << playerID;
                                player->position.y = entity->position.y + 0x150000;
                            }
                            RSDK.PlaySfx(Player->sfx_Grab, false, 255);
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGH, &player->playerAnimator, true, 0);
                            player->playerAnimator.animationSpeed = 0;
                            player->nextGroundState               = StateMachine_None;
                            player->nextAirState                  = StateMachine_None;
                            player->velocity.x                    = 0;
                            player->velocity.y                    = 0;
                            player->rotation                      = 0;
                            player->state                         = Player_State_None;
                        }
                    }
                }

                int posY = player->position.y;
                if (player->playerAnimator.animationID != ANI_POLESWINGH && player->playerAnimator.animationID != ANI_SHIMMYMOVE) {
                    posY -= 0x180000;
                }

                collided1 =
                    MathHelpers_PointInHitbox(entity->direction, entity->position.x, entity->position.y, &entity->hitbox1, player->position.x, posY);
                collided2 =
                    MathHelpers_PointInHitbox(entity->direction, entity->position.x, entity->position.y, &entity->hitbox2, player->position.x, posY);

                bool32 passFlag = false;
                if (((1 << playerID) & entity->activePlayers1)) {
                    if (entity->field_C4[playerID] > 0) {
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGH, &player->playerAnimator, true, entity->field_C4[playerID] >> 1);
                        player->playerAnimator.animationSpeed = 0;
                    }
                    else if (player->left) {
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SHIMMYMOVE, &player->playerAnimator, false, 0);
                        player->direction = FLIP_X;
                    }
                    else if (player->right) {
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SHIMMYMOVE, &player->playerAnimator, false, 0);
                        player->direction = FLIP_NONE;
                    }
                    else {
                        if (!(player->playerAnimator.animationID == ANI_POLESWINGH && !player->playerAnimator.frameID)) {
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_POLESWINGH, &player->playerAnimator, false, 0);
                            player->playerAnimator.animationSpeed = 0;
                        }
                    }

                    if (((1 << playerID) & entity->activePlayers2)) {
                        player->position.y = entity->position.y - 0x150000;
                        if (entity->direction) {
                            if (player->right) {
                                player->position.x += 0x20000;
                            }
                            else if (player->left) {
                                player->position.x += 0xB000;
                            }
                            else {
                                player->position.x += 0x15555;
                            }
                        }
                        else if (player->left) {
                            player->position.x -= 0x20000;
                        }
                        else if (player->right) {
                            player->position.x -= 0xB000;
                        }
                        else {
                            player->position.x -= 0x15555;
                        }
                    }

                    if (((1 << playerID) & entity->activePlayers3)) {
                        player->position.y = entity->position.y + 0x150000;
                        if (entity->direction) {
                            if (player->left) {
                                player->position.x -= 0x20000;
                            }
                            else if (player->right) {
                                player->position.x -= 0xB000;
                            }
                            else {
                                player->position.x -= 0x15555;
                            }
                        }
                        else if (player->right) {
                            player->position.x += 0x20000;
                        }
                        else if (player->left) {
                            player->position.x += 0xB000;
                        }
                        else {
                            player->position.x += 0x15555;
                        }
                    }

                    bool32 flag = true;
                    if (!((1 << playerID) & entity->activePlayers2) || collided1)
                        flag = false;

                    int anim = player->playerAnimator.animationID;
                    if (player->jumpPress || (anim != ANI_POLESWINGH && anim != ANI_SHIMMYMOVE) || player->velocity.x || player->velocity.y || flag
                        || (((1 << playerID) & entity->activePlayers3) && !collided2)) {
                        entity->activePlayers1 &= ~(1 << playerID);
                        player->position.y += 0x180000;
                        if (!player->jumpPress || player->down) {
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                            player->state = Player_State_Air;
                        }
                        else {
                            Player_StartJump(player);
                        }
                        if (((1 << playerID) & entity->activePlayers2))
                            entity->field_84[playerID] = 60;
                        if (((1 << playerID) & entity->activePlayers3))
                            entity->field_94[playerID] = 60;
                    }
                    if (((1 << playerID) & entity->activePlayers1))
                        passFlag = true;
                }

                if (((1 << playerID) & entity->activePlayers4) && !passFlag) {
                    bool32 flag = false;
                    if (((1 << playerID) & entity->activePlayers2) && !Player_CheckCollisionTouch(player, entity, &entity->hitbox1)) {
                        entity->activePlayers2 &= ~(1 << playerID);
                        flag = true;
                    }

                    if (!((1 << playerID) & entity->activePlayers3) || Player_CheckCollisionTouch(player, entity, &entity->hitbox2)) {
                        if (flag)
                            entity->activePlayers4 &= ~(1 << playerID);
                    }
                    else {
                        entity->activePlayers3 &= ~(1 << playerID);
                        entity->activePlayers4 &= ~(1 << playerID);
                    }
                }

                if (!((1 << playerID) & entity->activePlayers1))
                    entity->field_C4[playerID] = 0;
            }
        }
    }
}

void HangConveyor_EditorDraw(void) {}

void HangConveyor_EditorLoad(void) {}

void HangConveyor_Serialize(void)
{
    RSDK_EDITABLE_VAR(HangConveyor, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(HangConveyor, VAR_UINT32, length);
}
