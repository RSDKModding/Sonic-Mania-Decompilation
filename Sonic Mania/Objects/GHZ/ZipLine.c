#include "SonicMania.h"

ObjectZipLine *ZipLine;

void ZipLine_Update(void)
{
    RSDK_THIS(ZipLine);
    StateMachine_Run(self->state);

    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->handlePos.x;
    self->position.y = self->handlePos.y;
    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);
        if (self->grabDelay[pid])
            self->grabDelay[pid]--;

        if ((1 << pid) & self->activePlayers) {
            if (Player_CheckValidState(player)) {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                if (player->state != Player_State_Hit) {
                    if (player->velocity.x) {
                        if ((uint8)(self->angle - 0x40) < 0x80)
                            self->groundVel = -player->groundVel;
                        else
                            self->groundVel = player->groundVel;

                        if (self->groundVel > 0xA0000)
                            self->groundVel = 0xA0000;
                        else if (self->groundVel < -0xA0000)
                            self->groundVel = -0xA0000;

                        self->velocity.x = self->groundVel * RSDK.Cos256(self->angle) >> 8;
                        self->velocity.y = self->groundVel * RSDK.Sin256(self->angle) >> 8;
                        player->velocity.x = 0;
                        player->velocity.y = 0;
                        player->groundVel  = 0;
                        player->angle      = 0;
                        player->rotation   = 0;
                    }
                    int32 prevX          = player->position.x;
                    player->position.x = self->position.x;
                    player->position.y = self->position.y;
                    player->position.y +=
                        (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000) + ((ZipLine->hitbox.top - playerHitbox->top) << 16);
                    if (abs(prevX - self->position.x) <= 0x100000) {
                        if (!self->grabDelay[pid] && player->jumpPress) {
                            player->velocity.y       = -0x40000;
                            player->jumpAbilityTimer = 1;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            player->animator.animationSpeed = 48;
                            player->state                         = Player_State_Air;
                            self->grabDelay[pid]                = 60;
                            self->activePlayers &= ~(1 << pid);
                            player->onGround       = false;
                            player->groundedStore  = false;
                            player->tileCollisions = true;
                        }
                    }
                    else {
                        player->state          = Player_State_Air;
                        self->grabDelay[pid] = 60;
                        self->activePlayers &= ~(1 << pid);
                        player->onGround       = false;
                        player->groundedStore  = false;
                        player->tileCollisions = true;
                    }
                }
                else {
                    self->grabDelay[pid] = 60;
                    self->activePlayers &= ~(1 << pid);
                    player->onGround       = false;
                    player->groundedStore  = false;
                    player->tileCollisions = true;
                }
            }
            else {
                self->activePlayers &= ~(1 << pid);
                if (player->state != Player_State_Die) {
                    player->tileCollisions = true;
                }
            }
        }
        else {
            if (!self->grabDelay[pid] && player->state != Player_State_None && !player->down) {
                Hitbox *playerHitbox = Player_GetHitbox(player);
                Hitbox otherHitbox;
                otherHitbox.top    = playerHitbox->top - 4;
                otherHitbox.left   = playerHitbox->left;
                otherHitbox.right  = playerHitbox->right;
                otherHitbox.bottom = otherHitbox.top + 8;
                if (RSDK.CheckObjectCollisionTouchBox(self, &ZipLine->hitbox, player, &otherHitbox)) {
                    if (player->sidekick || self->state == ZipLine_State_Moving) {
                        self->activePlayers |= 1 << pid;
                        player->onGround      = false;
                        player->groundedStore = false;
                        player->velocity.x    = 0;
                        player->velocity.y    = 0;
                        player->groundVel     = 0;
                        player->angle         = 0;
                        player->rotation      = 0;
                        player->position.x    = self->position.x;
                        player->position.y    = self->position.y;
                        player->position.y +=
                            ((ZipLine->hitbox.top - playerHitbox->top) << 16) + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                        player->tileCollisions = false;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 0);
                        player->state           = Player_State_None;
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);
                        self->grabDelay[pid] = 15;
                        self->active         = ACTIVE_NORMAL;
                    }
                    else if (!self->state) {
                        self->groundVel = player->groundVel;
                        if ((uint8)(self->angle - 0x40) < 0x80)
                            self->groundVel = -player->groundVel;

                        if (self->groundVel > 0xA0000)
                            self->groundVel = 0xA0000;
                        else if (self->groundVel < -0xA0000)
                            self->groundVel = -0xA0000;

                        self->velocity.x = self->groundVel * RSDK.Cos256(self->angle) >> 8;
                        self->velocity.y = self->groundVel * RSDK.Sin256(self->angle) >> 8;
                        if (self->angle & 0x7F) {
                            if ((uint8)self->angle < 0x80) {
                                if (self->handlePos.x == self->startPos.x) {
                                    if (self->velocity.y < 0) {
                                        self->velocity.x = 0;
                                        self->velocity.y = 0;
                                        self->groundVel  = 0;
                                    }
                                    self->state = ZipLine_State_Moving;
                                }

                                if (self->handlePos.x == self->endPos.x && self->velocity.y < 0) {
                                    self->state = ZipLine_State_Moving;
                                    self->activePlayers |= 1 << pid;
                                    player->onGround      = false;
                                    player->groundedStore = false;
                                    player->velocity.x    = 0;
                                    player->velocity.y    = 0;
                                    player->groundVel     = 0;
                                    player->angle         = 0;
                                    player->rotation      = 0;
                                    player->position.x    = self->position.x;
                                    player->position.y    = self->position.y;
                                    player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                          + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                    player->tileCollisions = false;
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 0);
                                    player->state           = Player_State_None;
                                    player->nextAirState    = StateMachine_None;
                                    player->nextGroundState = StateMachine_None;
                                    RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);
                                    self->grabDelay[pid] = 15;
                                    self->active         = ACTIVE_NORMAL;
                                }
                            }
                            else {
                                if (self->handlePos.x == self->endPos.x) {
                                    if (self->velocity.y < 0) {
                                        self->velocity.x = 0;
                                        self->velocity.y = 0;
                                        self->groundVel  = 0;
                                    }
                                    self->state = ZipLine_State_Moving;
                                }

                                if (self->handlePos.x == self->startPos.x && self->velocity.y < 0) {
                                    self->state = ZipLine_State_Moving;
                                    self->activePlayers |= 1 << pid;
                                    player->onGround      = false;
                                    player->groundedStore = false;
                                    player->velocity.x    = 0;
                                    player->velocity.y    = 0;
                                    player->groundVel     = 0;
                                    player->angle         = 0;
                                    player->rotation      = 0;
                                    player->position.x    = self->position.x;
                                    player->position.y    = self->position.y;
                                    player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                          + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                    player->tileCollisions = false;
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 0);
                                    player->state           = Player_State_None;
                                    player->nextAirState    = StateMachine_None;
                                    player->nextGroundState = StateMachine_None;
                                    RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);
                                    self->grabDelay[pid] = 15;
                                    self->active         = ACTIVE_NORMAL;
                                }
                            }
                        }
                        else {
                            if (self->groundVel) {
                                if (self->groundVel < 0) {
                                    if (self->handlePos.x != self->startPos.x) {
                                        self->state = ZipLine_State_Moving;
                                        self->activePlayers |= 1 << pid;
                                        player->onGround      = false;
                                        player->groundedStore = false;
                                        player->velocity.x    = 0;
                                        player->velocity.y    = 0;
                                        player->groundVel     = 0;
                                        player->angle         = 0;
                                        player->rotation      = 0;
                                        player->position.x    = self->position.x;
                                        player->position.y    = self->position.y;
                                        player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                              + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                        player->tileCollisions = false;
                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 0);
                                        player->state           = Player_State_None;
                                        player->nextAirState    = StateMachine_None;
                                        player->nextGroundState = StateMachine_None;
                                        RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);
                                        self->grabDelay[pid] = 15;
                                        self->active         = ACTIVE_NORMAL;
                                    }
                                }
                                else if (self->handlePos.x != self->endPos.x) {
                                    self->state = ZipLine_State_Moving;
                                    self->activePlayers |= 1 << pid;
                                    player->onGround      = false;
                                    player->groundedStore = false;
                                    player->velocity.x    = 0;
                                    player->velocity.y    = 0;
                                    player->groundVel     = 0;
                                    player->angle         = 0;
                                    player->rotation      = 0;
                                    player->position.x    = self->position.x;
                                    player->position.y    = self->position.y;
                                    player->position.y += ((ZipLine->hitbox.top - playerHitbox->top) << 16)
                                                          + (((ZipLine->hitbox.bottom - ZipLine->hitbox.top) << 15) & 0xFFFF0000);
                                    player->tileCollisions = false;
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HANG, &player->animator, true, 0);
                                    player->state           = Player_State_None;
                                    player->nextAirState    = StateMachine_None;
                                    player->nextGroundState = StateMachine_None;
                                    RSDK.PlaySfx(Player->sfxGrab, false, 0xFF);
                                    self->grabDelay[pid] = 15;
                                    self->active         = ACTIVE_NORMAL;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    self->position.x = storeX;
    self->position.y = storeY;
}

void ZipLine_LateUpdate(void) {}

void ZipLine_StaticUpdate(void) {}

void ZipLine_Draw(void)
{
    RSDK_THIS(ZipLine);
    RSDK.DrawLine(self->startPos.x, self->startPos.y, self->endPos.x, self->endPos.y, 0x6060A0, 0x00, INK_NONE, false);
    RSDK.DrawLine(self->startPos.x, self->startPos.y + 0x10000, self->endPos.x, self->endPos.y + 0x10000, 0x303070, 0x00, INK_NONE, false);

    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &self->handlePos, false);
    self->animator.frameID = 1;

    RSDK.DrawSprite(&self->animator, &self->startPos, false);
    RSDK.DrawSprite(&self->animator, &self->endPos, false);
}

void ZipLine_Create(void *data)
{
    RSDK_THIS(ZipLine);
    self->active  = ACTIVE_BOUNDS;
    self->visible = true;
    self->drawFX  = FX_FLIP;
    RSDK.SetSpriteAnimation(ZipLine->aniFrames, 0, &self->animator, true, 0);
    self->drawOrder   = Zone->playerDrawLow;
    self->handlePos.x = self->position.x;
    self->handlePos.y = self->position.y;
    self->startPos.x  = self->position.x;
    self->startPos.y  = self->position.y;
    self->endPos.x    = self->position.x + (self->length << 8) * RSDK.Cos256(self->angle);
    self->endPos.y    = self->position.y + (self->length << 8) * RSDK.Sin256(self->angle);

    if (!SceneInfo->inEditor) {
        self->position.x += (self->endPos.x - self->startPos.x) >> 1;
        self->position.y += (self->endPos.y - self->startPos.y) >> 1;
        self->updateRange.x = (abs(self->endPos.x - self->startPos.x) >> 1) + 0x400000;
        self->updateRange.y = (abs(self->endPos.y - self->startPos.y) >> 1) + 0x400000;
        self->joinPos      = ZipLine_GetJoinPos();
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

    Zone_AddCallback(ZipLine_ZoneCB);
}

void ZipLine_ZoneCB(void)
{
    foreach_active(ZipLine, zipline)
    {
#if RETRO_USE_PLUS
        if ((1 << Zone->playerID) & zipline->activePlayers)
            Zone->playerFlags[Zone->playerID] = false;
#else
        if (zipline->activePlayers)
           Zone->playerFlags = false;
#endif
    }
}

void ZipLine_ForceReleasePlayers(void)
{
    RSDK_THIS(ZipLine);

    foreach_active(Player, player)
    {
        int32 pid = RSDK.GetEntityID(player);
        if ((1 << pid) & self->activePlayers) {
            self->grabDelay[pid] = 60;
            player->velocity.y     = self->velocity.y;
            player->velocity.x     = self->velocity.x;
            player->groundVel      = player->velocity.x;
            self->activePlayers &= ~(1 << pid);
            player->onGround       = false;
            player->groundedStore  = false;
            player->tileCollisions = true;
            player->state          = Player_State_Air;
        }
    }
}

//this func actually rules, you can join any number of ZipLines together using this
Vector2 ZipLine_GetJoinPos(void)
{
    RSDK_THIS(ZipLine);
    EntityZipLine *endMarker = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ZipLine);
    Vector2 result;
    result.x = 0;
    result.y = 0;

    if (endMarker->objectID == ZipLine->objectID) {
        Hitbox thisHitbox, otherHitbox;

        if (self->startPos.y >= self->endPos.y) {
            otherHitbox.top    = (self->endPos.y - self->position.y) >> 16;
            otherHitbox.bottom = (self->startPos.y - self->position.y) >> 16;
        }
        else {
            otherHitbox.top    = (self->startPos.y - self->position.y) >> 16;
            otherHitbox.bottom = (self->endPos.y - self->position.y) >> 16;
        }

        if (self->startPos.x >= self->endPos.x) {
            otherHitbox.left  = (self->endPos.x - self->position.y) >> 16;
            otherHitbox.right = (self->startPos.x - self->position.y) >> 16;
        }
        else {
            otherHitbox.left  = (self->startPos.x - self->position.x) >> 16;
            otherHitbox.right = (self->endPos.x - self->position.x) >> 16;
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
        if (RSDK.CheckObjectCollisionTouchBox(endMarker, &thisHitbox, self, &otherHitbox)) {
            int32 distX1 = (self->startPos.x >> 17) - (self->endPos.x >> 17);
            int32 distY1 = (self->endPos.y >> 17) - (self->startPos.y >> 17);
            int32 distX2 = (endMarker->startPos.x >> 17) - (endMarker->endPos.x >> 17);
            int32 distY2 = (endMarker->endPos.y >> 17) - (endMarker->startPos.y >> 17);

            int32 val1    = distX1 * (self->startPos.y >> 17) + distY1 * (self->startPos.x >> 17);
            int32 val2    = distX2 * (endMarker->startPos.y >> 17) + distY2 * (endMarker->startPos.x >> 17);
            float divisor = (float)(distY1 * distX2 - distX1 * distY2);
            if (divisor != 0.0f) {
                endMarker->handlePos.x = -0x100000;
                result.x               = (int32)((float)(distX2 * val1 - distX1 * val2) / divisor) << 17;
                result.y               = (int32)((float)(distY1 * val2 - distY2 * val1) / divisor) << 17;
            }
        }
    }
    return result;
}
void ZipLine_State_Moving(void)
{
    RSDK_THIS(ZipLine);
    self->groundVel += (RSDK.Sin256(self->angle) << 14 >> 8);
    if (self->groundVel >= 0xA0000)
        self->groundVel = 0xA0000;

    self->velocity.x = self->groundVel * RSDK.Cos256(self->angle) >> 8;
    self->velocity.y = self->groundVel * RSDK.Sin256(self->angle) >> 8;

    if (self->joinPos.x) {
        int32 storeX           = self->position.x;
        int32 storeY           = self->position.y;
        self->position.x = self->handlePos.x;
        self->position.y = self->handlePos.y;

        Hitbox otherHitbox;
        otherHitbox.top    = ((self->joinPos.y - self->position.y - (self->velocity.y >> 1)) >> 16) + 8;
        otherHitbox.bottom = (((self->velocity.y >> 1) + (self->joinPos.y - self->position.y)) >> 16) + 16;

        if (self->velocity.x >= 0) {
            otherHitbox.left  = (self->joinPos.x - self->position.x) >> 16;
            otherHitbox.right = ((self->joinPos.x + self->velocity.x - self->position.x) >> 16) + 5;
        }
        else {
            otherHitbox.left  = (((self->joinPos.x - self->position.x) - self->velocity.x) >> 16) - 5;
            otherHitbox.right = (self->joinPos.x - self->position.x) >> 16;
        }

        if (RSDK.CheckObjectCollisionTouchBox(self, &ZipLine->hitbox, self, &otherHitbox)) {
            EntityZipLine *endMarker = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, ZipLine);
            endMarker->handlePos.x   = self->joinPos.x;
            endMarker->handlePos.y   = self->joinPos.y;
            endMarker->onGround      = true;
            endMarker->activePlayers = self->activePlayers;
            endMarker->groundVel     = self->groundVel;
            endMarker->state         = ZipLine_State_Moving;
            self->position.x       = storeX;
            self->position.y       = storeY;
            self->activePlayers    = 0;
            self->groundVel        = 0;
            self->handlePos.x      = -0x100000;
            self->state            = StateMachine_None;
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
        self->position.x = storeX;
        self->position.y = storeY;
    }

    self->handlePos.x += self->velocity.x;
    self->handlePos.y += self->velocity.y;
    if (self->groundVel < 0) {
        if (self->velocity.x < 0) {
            if (self->handlePos.x < self->startPos.x)
                self->handlePos.x = self->startPos.x;

            if (self->velocity.y < 0) {
                if (self->handlePos.y < self->startPos.y)
                    self->handlePos.y = self->startPos.y;
            }
            if (self->velocity.y > 0) {
                if (self->handlePos.y > self->startPos.y)
                    self->handlePos.y = self->startPos.y;
            }

            if (self->handlePos.x == self->startPos.x && self->handlePos.y == self->startPos.y) {
                self->groundVel = 0;
                if (self->startPos.y >= self->endPos.y || !self->onGround) {
                    self->onGround = false;
                    self->active   = ACTIVE_BOUNDS;
                    self->state    = StateMachine_None;
                    ZipLine_ForceReleasePlayers();
                }
            }
        }
        else if (self->velocity.x > 0) {
            if (self->handlePos.x > self->startPos.x)
                self->handlePos.x = self->startPos.x;

            if (self->velocity.y < 0) {
                if (self->handlePos.y < self->startPos.y)
                    self->handlePos.y = self->startPos.y;
            }
            if (self->velocity.y > 0) {
                if (self->handlePos.y > self->startPos.y)
                    self->handlePos.y = self->startPos.y;
            }

            if (self->handlePos.x == self->startPos.x && self->handlePos.y == self->startPos.y) {
                self->groundVel = 0;
                if (self->startPos.y >= self->endPos.y || !self->onGround) {
                    self->onGround = false;
                    self->active   = ACTIVE_BOUNDS;
                    self->state    = StateMachine_None;
                    ZipLine_ForceReleasePlayers();
                }
            }
        }
    }
    else if (self->groundVel > 0) {
        if (self->velocity.x > 0) {
            if (self->handlePos.x > self->endPos.x) {
                self->handlePos.x = self->endPos.x;
            }
        }
        else if (self->velocity.x < 0) {
            self->endPos.x = self->endPos.x;
            if (self->handlePos.x < self->endPos.x) {
                self->handlePos.x = self->endPos.x;
            }
        }

        if (self->velocity.y < 0) {
            if (self->handlePos.y < self->endPos.y)
                self->handlePos.y = self->endPos.y;
        }
        else if (self->velocity.y > 0) {
            if (self->handlePos.y > self->endPos.y)
                self->handlePos.y = self->endPos.y;
        }

        if (self->handlePos.x == self->endPos.x && self->handlePos.y == self->endPos.y) {
            self->groundVel = 0;
            if (self->endPos.y >= self->startPos.y || !self->onGround) {
                self->onGround = false;
                self->active   = ACTIVE_BOUNDS;
                self->state    = StateMachine_None;
                ZipLine_ForceReleasePlayers();
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void ZipLine_EditorDraw(void)
{
    RSDK_THIS(ZipLine);
    self->handlePos.x = self->position.x;
    self->handlePos.y = self->position.y;
    self->startPos.x  = self->position.x;
    self->startPos.y  = self->position.y;
    self->endPos.x    = self->position.x + (self->length << 8) * RSDK.Cos256(self->angle);
    self->endPos.y    = self->position.y + (self->length << 8) * RSDK.Sin256(self->angle);

    ZipLine_Draw();
}

void ZipLine_EditorLoad(void) { ZipLine->aniFrames = RSDK.LoadSpriteAnimation("GHZ/ZipLine.bin", SCOPE_STAGE); }
#endif

void ZipLine_Serialize(void)
{
    RSDK_EDITABLE_VAR(ZipLine, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(ZipLine, VAR_ENUM, length);
}
