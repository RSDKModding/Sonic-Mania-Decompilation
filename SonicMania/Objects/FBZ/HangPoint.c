// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HangPoint Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHangPoint *HangPoint;

void HangPoint_Update(void)
{
    RSDK_THIS(HangPoint);

    if (self->length > 0) {
        bool32 isActive = true;
        if (self->activePlayers == 2) {
            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            if (player2->sidekick)
                isActive = false;
        }

        for (int32 i = 0; i < PLAYER_COUNT; ++i) {
            if (self->moveDistance[i])
                isActive = false;
        }

        if (self->pullPos > self->length) {
            if (self->pullPos <= 0) {
                self->activePlayers &= 0xF;
            }
            else {
                self->velocity.y -= 0x1800;
                if (self->velocity.y < -self->maxVel)
                    self->velocity.y = -self->maxVel;

                self->activePlayers |= 16 * self->activePlayers;
                self->pullPos += self->velocity.y;
            }
        }
        else {
            if (!self->activePlayers || !isActive) {
                self->active = ACTIVE_BOUNDS;
                if (self->direction == FLIP_NONE) {
                    if (self->pullPos > 0) {
                        self->velocity.y -= 0x1000;
                        if (self->velocity.y < -self->maxVel)
                            self->velocity.y = -self->maxVel;

                        self->pullPos += self->velocity.y;
                    }
                }
                else if (self->pullPos < self->length) {
                    self->velocity.y += 0x1000;
                    if (self->velocity.y > self->maxVel)
                        self->velocity.y = self->maxVel;

                    self->pullPos += self->velocity.y;
                    if (self->pullPos > self->length) {
                        self->pullPos    = self->length;
                        self->velocity.y = 0;
                    }
                }
            }
            else {
                if (self->direction) {
                    if (self->pullPos <= 0) {
                        self->activePlayers &= 0xF;
                    }
                    else {
                        self->velocity.y -= 0x1800;
                        if (self->velocity.y < -self->maxVel)
                            self->velocity.y = -self->maxVel;

                        self->activePlayers |= 16 * self->activePlayers;
                        self->pullPos += self->velocity.y;
                    }
                }
                else {
                    if (self->pullPos >= self->length) {
                        self->activePlayers &= 0xF;
                    }
                    else {
                        self->velocity.y += 0x1800;
                        if (self->velocity.y > self->maxVel)
                            self->velocity.y = self->maxVel;

                        self->pullPos += self->velocity.y;
                        if (self->pullPos > self->length) {
                            self->pullPos    = self->length;
                            self->velocity.y = 0;
                        }

                        self->activePlayers |= (self->activePlayers << 4);
                    }
                }
            }
        }

        if (self->pullPos < 0) {
            self->pullPos    = 0;
            self->velocity.y = 0;
        }
    }

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;
    self->position.y += self->pullPos;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerTimer[playerID] > 0)
            self->playerTimer[playerID]--;

        if (!((1 << playerID) & self->activePlayers)) {
            if (player->state != Player_State_Static && !self->playerTimer[playerID]) {
                Hitbox *playerHitbox = Player_GetHitbox(player);

                Hitbox hitboxPlayer;
                hitboxPlayer.left   = playerHitbox->left;
                hitboxPlayer.top    = playerHitbox->top;
                hitboxPlayer.right  = playerHitbox->right;
                hitboxPlayer.bottom = hitboxPlayer.top + 4;
                if (RSDK.CheckObjectCollisionTouchBox(self, &HangPoint->hitboxGrab, player, &hitboxPlayer)) {
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    player->onGround   = false;
                    player->angle      = 0;
                    player->rotation   = 0;
                    player->position.x = self->position.x;
                    player->position.y = self->position.y;
                    player->position.y += ((HangPoint->hitboxGrab.top - playerHitbox->top) << 16)
                                          + (((HangPoint->hitboxGrab.bottom - HangPoint->hitboxGrab.top) << 15) & 0xFFFF0000);
                    player->tileCollisions       = TILECOLLISION_NONE;
                    self->moveDistance[playerID] = 0;

                    if (!self->activePlayers) {
                        if (self->direction == FLIP_X) {
                            if (self->pullPos <= self->length)
                                self->velocity.y = 0x20000;
                        }
                        else {
                            self->velocity.y = 0;
                        }
                    }
                    self->activePlayers |= 1 << playerID;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);
                    player->nextAirState     = StateMachine_None;
                    player->nextGroundState  = StateMachine_None;
                    player->state            = Player_State_Static;
                    player->abilityValues[0] = RSDK.Rand(0, 2);
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                    self->active = ACTIVE_NORMAL;
                }
            }
        }
        else {
            if (player->state == Player_State_Hurt) {
                player->tileCollisions = TILECOLLISION_DOWN;
                self->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + PLAYER_COUNT));
                self->moveDistance[playerID] = 0;
                if (player->left || player->right || player->down || player->state == Player_State_Hurt)
                    self->playerTimer[playerID] = 64;
                else
                    self->playerTimer[playerID] = 16;
            }
            else {
                if (player->state != Player_State_Static || self->playerTimer[playerID]) {
                    self->activePlayers &= ~(1 << playerID);

                    if (player->classID == Player->classID && Player_CheckValidState(player))
                        player->tileCollisions = TILECOLLISION_DOWN;
                }
                else {
                    Hitbox *playerHitbox = Player_GetHitbox(player);
                    player->velocity.x   = 0;
                    player->velocity.y   = 0;
                    player->groundVel    = 0;
                    player->angle        = 0;
                    player->rotation     = 0;
                    player->position.x   = self->position.x;
                    player->position.y   = self->position.y;
                    player->position.y += (((HangPoint->hitboxGrab.bottom - HangPoint->hitboxGrab.top) << 15) & 0xFFFF0000)
                                          + ((HangPoint->hitboxGrab.top - playerHitbox->top) << 16);

                    if (!self->moveDistance[playerID]) {
                        if (player->left)
                            player->direction = FLIP_X;

                        if (player->right)
                            player->direction = FLIP_NONE;

                        if (player->jumpPress) {
                            player->velocity.y = -0x40000;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            player->animator.speed   = 48;
                            player->applyJumpCap     = false;
                            player->jumpAbilityState = 1;
                            player->state            = Player_State_Air;
                            player->tileCollisions   = TILECOLLISION_DOWN;
                            self->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + PLAYER_COUNT));

                            if (player->left || player->right || player->down || player->state == Player_State_Hurt) {
                                self->playerTimer[playerID] = 64;
                                HangPoint_HandlePlayerMovement(self, player, playerID);
                            }
                            else {
                                self->playerTimer[playerID] = 16;
                                HangPoint_HandlePlayerMovement(self, player, playerID);
                            }
                        }
                        else if ((!((1 << (playerID + 4)) & self->activePlayers) && (self->direction != FLIP_NONE || self->pullPos >= self->length)
                                  && (self->direction != FLIP_X || self->pullPos <= 0))
                                 || player->sidekick) {

                            bool32 changedHangPoint = false;
                            if (player->left) {
                                foreach_active(HangPoint, point)
                                {
                                    if (self != point) {
                                        int32 distance = self->position.x - point->position.x;
                                        if (distance < 0x220000 && distance > 0) {
                                            if (abs(self->position.y - point->position.y - point->pullPos) < 0x40000) {
                                                self->activePlayers &= ~(1 << playerID);
                                                self->playerTimer[playerID] = 16;
                                                point->activePlayers |= 1 << playerID;
                                                point->moveDistance[playerID] = 0x200000;
                                                point->playerTimer[playerID]  = 0;
#if MANIA_USE_PLUS
                                                if (player->characterID == ID_RAY) {
                                                    if (player->abilityValues[0])
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG_MOVE, &player->animator, false, 6);
                                                    else
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG_MOVE, &player->animator, false, 1);
                                                }
                                                else {
#endif
                                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG_MOVE, &player->animator, false, 3);
#if MANIA_USE_PLUS
                                                }
#endif

                                                player->position.x = point->position.x;

                                                if (RSDK.GetEntitySlot(point) < SceneInfo->entitySlot)
                                                    HangPoint_HandlePlayerMovement(point, player, playerID);
                                                HangPoint_HandlePlayerMovement(self, player, playerID);

                                                changedHangPoint = true;
                                                foreach_break;
                                            }
                                        }
                                    }
                                }
                            }
                            else if (player->right) {
                                foreach_active(HangPoint, point)
                                {
                                    if (self != point) {
                                        int32 distance = point->position.x - self->position.x;
                                        if (distance < 0x220000 && distance > 0) {
                                            if (abs(self->position.y - point->position.y - point->pullPos) < 0x40000) {
                                                self->activePlayers &= ~(1 << playerID);
                                                self->playerTimer[playerID] = 16;
                                                point->activePlayers |= 1 << playerID;
                                                point->moveDistance[playerID] = -0x200000;
                                                point->playerTimer[playerID]  = 0;

#if MANIA_USE_PLUS
                                                if (player->characterID == ID_RAY) {
                                                    if (!player->abilityValues[0]) {
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG_MOVE, &player->animator, false, 0);
                                                    }
                                                    else {
                                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG_MOVE, &player->animator, false, 6);
                                                    }
                                                }
                                                else {
#endif
                                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG_MOVE, &player->animator, false, 3);
#if MANIA_USE_PLUS
                                                }
#endif

                                                player->position.x = point->position.x;

                                                if (RSDK.GetEntitySlot(point) < SceneInfo->entitySlot)
                                                    HangPoint_HandlePlayerMovement(point, player, playerID);
                                                HangPoint_HandlePlayerMovement(self, player, playerID);

                                                changedHangPoint = true;
                                                foreach_break;
                                            }
                                        }
                                    }
                                }
                            }

                            if (!changedHangPoint) {
#if MANIA_USE_PLUS
                                if (player->characterID == ID_RAY && player->abilityValues[0] == 1)
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG2, &player->animator, false, 0);
                                else
#endif
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);

                                HangPoint_HandlePlayerMovement(self, player, playerID);
                            }
                        }
                        else {
#if MANIA_USE_PLUS
                            if (player->characterID == ID_RAY && player->abilityValues[0] == 1)
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG2, &player->animator, false, 0);
                            else
#endif
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, false, 0);

                            HangPoint_HandlePlayerMovement(self, player, playerID);
                        }
                    }
                    else {
                        HangPoint_HandlePlayerMovement(self, player, playerID);
                    }
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void HangPoint_LateUpdate(void) {}

void HangPoint_StaticUpdate(void) {}

void HangPoint_Draw(void)
{
    RSDK_THIS(HangPoint);

    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + self->pullPos;
    RSDK.DrawSprite(&HangPoint->animator, &drawPos, false);

    if (self->length) {
        HangPoint->animator.frameID = 1;
        int32 length                = (self->pullPos >> 16) & 0xFF00;

        SpriteFrame *frame = RSDK.GetFrame(HangPoint->aniFrames, 0, HangPoint->animator.frameID);

        int32 extraLength = (self->pullPos >> 16) & 0x00FF;
        frame->pivotY     = -(self->pullPos >> 16);
        frame->height     = extraLength;
        frame->sprY       = 257 - extraLength;
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

    if (!self->speed)
        self->speed = 2;

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x400000;
    self->visible       = true;
    self->updateRange.y = (self->length + 64) << 16;
    self->drawGroup     = Zone->objectDrawGroup[0];
    if (!SceneInfo->inEditor) {
        self->length = self->length << 16;
        self->maxVel = self->speed << 15;
        if (self->direction == FLIP_X)
            self->pullPos = self->length;
    }
}

void HangPoint_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("FBZ"))
        HangPoint->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangPoint.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(HangPoint->aniFrames, 0, &HangPoint->animator, true, 0);

    Hitbox *hitbox               = RSDK.GetHitbox(&HangPoint->animator, 0);
    HangPoint->hitboxGrab.top    = hitbox->top;
    HangPoint->hitboxGrab.left   = hitbox->left;
    HangPoint->hitboxGrab.bottom = hitbox->bottom;
    HangPoint->hitboxGrab.right  = hitbox->right;
}

void HangPoint_HandlePlayerMovement(EntityHangPoint *self, EntityPlayer *player, int32 playerID)
{
    if (self->moveDistance[playerID]) {
        if (self->moveDistance[playerID] < 0) {
            self->moveDistance[playerID] += 0x15556;
            if (self->moveDistance[playerID] >= 0) {
                self->moveDistance[playerID] = 0;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                player->abilityValues[0] ^= 1;
            }
        }
        else {
            self->moveDistance[playerID] -= 0x15556;
            if (self->moveDistance[playerID] <= 0) {
                self->moveDistance[playerID] = 0;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                player->abilityValues[0] ^= 1;
            }
        }

        player->position.x += self->moveDistance[playerID] & 0xFFFF0000;
        if (player->jumpPress) {
            player->velocity.y = -0x40000;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);

            player->applyJumpCap     = false;
            player->animator.speed   = 48;
            player->jumpAbilityState = 1;
            player->state            = Player_State_Air;
            player->tileCollisions   = TILECOLLISION_DOWN;
            self->activePlayers &= ~(1 << playerID) & ~(1 << (playerID + 4));
            self->moveDistance[playerID] = 0;

            if (player->left || player->right || player->down || player->state == Player_State_Hurt)
                self->playerTimer[playerID] = 64;
            else
                self->playerTimer[playerID] = 16;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void HangPoint_EditorDraw(void)
{
    RSDK_THIS(HangPoint);

    int32 len = self->length;
    self->length <<= 16;

    self->pullPos = self->direction == FLIP_X ? self->length : 0;
    HangPoint_Draw();

    if (self->length && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        self->inkEffect = INK_BLEND;

        int32 prevPullPos = self->pullPos;
        self->pullPos     = self->direction == FLIP_X ? 0 : self->length;
        HangPoint_Draw();

        DrawHelpers_DrawArrow(self->position.x, self->position.y + prevPullPos, self->position.x, self->position.y + self->pullPos, 0x00FF00,
                              INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
        self->inkEffect = INK_NONE;
    }

    self->length = len;
}

void HangPoint_EditorLoad(void)
{
    HangPoint->aniFrames = RSDK.LoadSpriteAnimation("FBZ/HangPoint.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(HangPoint->aniFrames, 0, &HangPoint->animator, true, 0);

    RSDK_ACTIVE_VAR(HangPoint, direction);
    RSDK_ENUM_VAR("Drop Down", FLIP_NONE);
    RSDK_ENUM_VAR("Pull Up", FLIP_X);
}
#endif

void HangPoint_Serialize(void)
{
    RSDK_EDITABLE_VAR(HangPoint, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(HangPoint, VAR_UINT8, speed);
    RSDK_EDITABLE_VAR(HangPoint, VAR_UINT8, direction);
}
