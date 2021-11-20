#include "SonicMania.h"

ObjectCylinder *Cylinder;

void Cylinder_Update(void)
{
    RSDK_THIS(Cylinder);
    StateMachine_Run(self->state);
}

void Cylinder_LateUpdate(void) {}

void Cylinder_StaticUpdate(void) {}

void Cylinder_Draw(void) {}

void Cylinder_Create(void *data)
{
    RSDK_THIS(Cylinder);
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_BOUNDS;
        self->visible   = false;
        self->drawOrder = Zone->drawOrderLow;
        switch (self->type) {
            case 0:
                self->updateRange.x = self->length << 16;
                self->updateRange.y = self->radius << 16;

                self->hitbox1.left   = -self->length;
                self->hitbox1.top    = -self->radius;
                self->hitbox1.right  = self->length;
                self->hitbox1.bottom = self->radius;
                self->hitbox2.left   = -self->length;
                self->hitbox2.top    = -self->radius;
                self->hitbox2.right  = self->length;
                self->hitbox2.bottom = -self->radius + 8;
                self->state          = Cylinder_State_Unknown1;
                break;
            case 1:
                self->updateRange.x  = self->radius << 16;
                self->updateRange.y  = self->length << 16;
                self->state          = Cylinder_State_Unknown2;
                self->hitbox1.left   = -self->radius;
                self->hitbox1.top    = -self->length;
                self->hitbox1.right  = self->radius;
                self->hitbox1.bottom = self->length;
                break;
            case 2:
                self->updateRange.x  = (self->radius + 112) << 16;
                self->updateRange.y  = self->length << 16;
                self->hitbox1.left   = -64 - self->radius;
                self->hitbox1.right  = self->radius + 64;
                self->hitbox1.top    = -self->length;
                self->hitbox1.bottom = self->length;
                self->hitbox3.left   = -112 - self->radius;
                self->hitbox3.right  = -self->radius;
                self->hitbox3.top    = -self->length;
                self->hitbox3.bottom = 64 - self->length;
                self->hitbox4.left   = self->radius;
                self->hitbox4.top    = self->length - 64;
                self->hitbox4.right  = self->length + 112;
                self->hitbox4.bottom = self->length;
                self->state          = Cylinder_State_Unknown3;
                break;
            case 3:
                self->updateRange.x = (self->radius + 112) << 16;
                self->updateRange.y = self->length << 16;

                self->hitbox1.left   = -64 - self->radius;
                self->hitbox1.top    = -self->length;
                self->hitbox1.right  = self->radius + 64;
                self->hitbox1.bottom = self->length;
                self->hitbox3.left   = -112 - self->radius;
                self->hitbox3.top    = self->length - 64;
                self->hitbox3.right  = -self->radius;
                self->hitbox3.bottom = self->length;
                self->hitbox4.left   = self->radius;
                self->hitbox4.top    = -self->length;
                self->hitbox4.right  = self->radius + 112;
                self->hitbox4.bottom = 64 - self->length;
                self->state          = Cylinder_State_Unknown3;
                break;
            case 4:
                self->updateRange.x  = self->length << 16;
                self->updateRange.y  = self->radius << 16;
                self->hitbox1.left   = 16 - self->length;
                self->hitbox1.top    = ~self->radius;
                self->hitbox1.right  = self->length - 16;
                self->hitbox1.bottom = self->radius + 1;
                self->hitbox2.left   = -self->length;
                self->hitbox2.top    = -self->radius;
                self->hitbox2.right  = self->length;
                self->hitbox2.bottom = self->radius;
                self->state          = Cylinder_State_Unknown4;
                break;
            case 7:
                self->visible        = true;
                self->updateRange.x  = self->radius << 17;
                self->updateRange.y  = self->length << 17;
                self->hitbox1.left   = -16 - self->radius;
                self->hitbox1.top    = -self->length;
                self->hitbox1.right  = self->radius + 16;
                self->hitbox1.bottom = self->length;
                self->length         = 2 * self->length;
                self->state          = Cylinder_State_Unknown5;
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
        int32 radius           = (self->radius - playerHitbox->bottom) << 6;

        if (self->playerFrameIDs[player->playerID]) {
            if (!Player_CheckCollisionTouch(player, self, &self->hitbox1) || player->velocity.y <= -0x40000) {
                if (((self->playerAngles[player->playerID] + 128) & 0x3FF) < 0x100)
                    player->velocity.y >>= 1;
                else
                    player->velocity.y = 0;
                if (!player->onGround && player->velocity.y >= 0 && player->state != Player_State_FlyCarried
                    && player->animator.animationID != ANI_SKID) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true,
                                            24 * self->playerAngles[player->playerID] / 1024);
                }
                player->onGround                         = false;
                self->playerFrameIDs[player->playerID] = 0;
            }
            else {
                self->playerAngles[player->playerID] = (self->playerAngles[player->playerID] + 16) & 0x3FF;
                player->position.y                     = radius * RSDK.Cos1024(self->playerAngles[player->playerID]) + self->position.y;

                if (player->state == Player_State_LookUp || player->state == Player_State_Crouch)
                    player->state = Player_State_Ground;

                if (player->state != Player_State_Ground || player->onGround || player->animator.animationID == ANI_SKID) {
                    if (player->state == Player_State_Roll) {
                        if (player->velocity.x)
                            player->direction = player->velocity.x > 0;
                    }
                    player->onGround = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true,
                                            24 * self->playerAngles[player->playerID] / 1024);
                    player->onGround = true;
                }
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                if (player->position.y - self->position.y > radius << 10 && player->velocity.y >= 0) {
                    player->position.y                       = self->position.y + (radius << 10);
                    player->onGround                         = true;
                    player->velocity.y                       = 0;
                    self->playerAngles[player->playerID]   = 0;
                    self->playerFrameIDs[player->playerID] = 1;
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
        int32 radius           = (self->radius - playerHitbox->bottom) << 6;

        if (self->playerFrameIDs[player->playerID]) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                self->playerAngles[player->playerID] = (self->playerAngles[player->playerID] + 16) & 0x3FF;
                player->position.x                     = radius * RSDK.Cos1024(self->playerAngles[player->playerID]) + self->position.x;
                player->rotation                       = 128;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true,
                                        (24 * self->playerAngles[player->playerID] / 1024 + 12) % 24);
                player->animator.rotationFlag = 1;
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
                self->playerFrameIDs[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                if (abs(player->position.x - self->position.x) > (radius - 896) << 10) {
                    if (player->position.x - self->position.x <= 0) {
                        player->collisionMode                  = CMODE_RWALL;
                        self->playerAngles[player->playerID] = 512;
                    }
                    else {
                        player->collisionMode                  = CMODE_LWALL;
                        self->playerAngles[player->playerID] = 0;
                    }
                    player->state                            = Player_State_Air;
                    player->nextAirState                     = StateMachine_None;
                    player->nextGroundState                  = StateMachine_None;
                    self->playerFrameIDs[player->playerID] = 1;
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
        int32 radius           = (self->radius - playerHitbox->bottom - 12) << 6;

        switch (self->playerFrameIDs[player->playerID]) {
            case 0:
                if (player->onGround) {
                    bool32 flag = true;
                    if (!Player_CheckCollisionTouch(player, self, &self->hitbox3) || player->groundVel <= 0x30000) {
                        if (!Player_CheckCollisionTouch(player, self, &self->hitbox4) || player->groundVel >= -0x30000) {
                            flag = false;
                        }
                    }
                    if (flag) {
                        self->playerFrameIDs[player->playerID] = 1;
                        player->state                            = Cylinder_Player_State_Unknown1;
                    }
                }
                break;
            case 1:
                if (player->groundVel <= 0x30000) {
                    if (player->groundVel >= -0x30000) {
                        player->onGround                         = false;
                        self->playerFrameIDs[player->playerID] = 0;
                    }
                    else {
                        if (player->position.x <= self->position.x) {
                            self->playerAngles[player->playerID]   = 768;
                            player->tileCollisions                   = false;
                            self->playerFrameIDs[player->playerID] = 2;
                        }
                        int32 distance = (player->position.x - self->position.x) >> 16;
                        int32 right    = self->hitbox4.right - 48;
                        if (distance < right) {
                            player->rotation  = 256;
                            player->direction = FLIP_NONE;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 17 - 6 * distance / right);
                            player->animator.rotationFlag = 1;
                        }
                        player->onGround   = true;
                        player->velocity.y = 0;
                    }
                }
                else {
                    if (player->position.x >= self->position.x) {
                        self->playerAngles[player->playerID]   = 768;
                        player->tileCollisions                   = false;
                        self->playerFrameIDs[player->playerID] = 2;
                    }
                    int32 distance = (self->position.x - player->position.x) >> 16;
                    int32 right    = -48 - self->hitbox3.left;
                    if (distance < right) {
                        player->rotation  = 256;
                        player->direction = FLIP_X;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 17 - 6 * distance / right);
                        player->animator.rotationFlag = 1;
                        player->onGround                    = true;
                        player->velocity.y                  = 0;
                    }
                    player->onGround   = true;
                    player->velocity.y = 0;
                }
                break;
            case 2:
                self->playerAngles[player->playerID] = (self->playerAngles[player->playerID] + (player->groundVel >> 15)) & 0x3FF;
                player->position.x                     = radius * RSDK.Cos1024(self->playerAngles[player->playerID]) + self->position.x;

                if (self->hitbox3.top >= self->hitbox4.top)
                    player->position.y -= (player->groundVel >> 2) & 0xFFFFE000;
                else
                    player->position.y += (player->groundVel >> 2) & 0xFFFFE000;
                player->direction = FLIP_X;

                int32 frame = 0;
                if (player->groundVel <= 0) {
                    player->rotation = 128;
                    frame            = 35 - 24 * self->playerAngles[player->playerID] / 1024;
                }
                else {
                    player->rotation = 384;
                    frame            = 24 * self->playerAngles[player->playerID] / 1024;
                }

                if (player->velocity.y < -0x40000) {
                    self->playerFrameIDs[player->playerID] = 0;
                    player->tileCollisions                   = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame % 24);
                    player->animator.rotationFlag = 1;
                    player->onGround                    = true;
                    player->velocity.y                  = 0;

                    if (player->groundVel <= 0) {
                        if (player->groundVel >= -0x30000) {
                            self->playerFrameIDs[player->playerID] = 0;
                            player->tileCollisions                   = true;
                        }
                        else {
                            if (self->hitbox3.top < self->hitbox4.top) {
                                int32 maxY = self->position.y - (playerHitbox->bottom << 16) - self->updateRange.y + 0x400000;
                                if (player->position.y <= maxY) {
                                    player->position.y                       = maxY;
                                    self->playerFrameIDs[player->playerID] = 3;
                                    player->tileCollisions                   = true;
                                }
                            }
                            else {
                                if (player->position.y >= self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y) {
                                    player->position.y = self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y;
                                    self->playerFrameIDs[player->playerID] = 3;
                                    player->tileCollisions                   = true;
                                }
                            }
                        }
                    }
                    else {
                        if (player->groundVel <= 0x30000) {
                            self->playerFrameIDs[player->playerID] = 0;
                            player->tileCollisions                   = true;
                        }
                        else if (self->hitbox3.top >= self->hitbox4.top) {
                            int32 maxY = self->position.y - (playerHitbox->bottom << 16) - self->updateRange.y + 0x400000;
                            if (player->position.y <= maxY) {
                                player->position.y                       = maxY;
                                self->playerFrameIDs[player->playerID] = 3;
                                player->tileCollisions                   = true;
                            }
                        }
                        else {
                            if (player->position.y >= self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y) {
                                player->position.y                       = self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y;
                                self->playerFrameIDs[player->playerID] = 3;
                                player->tileCollisions                   = true;
                            }
                        }
                    }
                }
                break;
            case 3:
                if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
                    if (abs(player->groundVel) <= 0x30000) {
                        self->playerFrameIDs[player->playerID] = 0;
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
                            int32 distance = (player->position.x - self->position.x) >> 16;
                            int32 right    = self->hitbox4.right - 48;
                            if (distance < right) {
                                player->rotation  = 256;
                                player->direction = FLIP_X;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 17 - 6 * distance / right);
                                player->animator.rotationFlag = 1;
                            }
                        }
                        else {
                            int32 distance = (self->position.x - player->position.x) >> 16;
                            int32 right    = -48 - self->hitbox3.left;
                            if (distance < right) {
                                player->rotation  = 256;
                                player->direction = FLIP_NONE;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 17 - 6 * distance / right);
                                player->animator.rotationFlag = 1;
                            }
                        }
                    }
                }
                else {
                    self->playerFrameIDs[player->playerID] = 0;
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
        int32 radius           = (self->radius + playerHitbox->bottom) << 6;
        if (self->playerFrameIDs[player->playerID]) {
            int32 angle = (self->playerAngles[player->playerID] + 8) & 0x3FF;
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1) && !player->onGround) {
                if ((player->position.y < self->position.y && player->velocity.y >= 0)
                    || (player->position.y >= self->position.y && player->velocity.y <= 0)) {

                    bool32 flag = true;
                    if (player->state == Cylinder_Player_State_Unknown2) {
                        int32 frame = (24 - (24 * angle) / 1024) % 24;
                        if (player->groundVel)
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame);
                        else
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_STANDCS, &player->animator, true, frame);
                        player->animator.rotationFlag = 1;
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
                        self->playerAngles[player->playerID] = angle;
                        player->position.y                     = radius * RSDK.Cos1024(angle) + self->position.y;

                        if (player->position.y < self->position.y)
                            player->abilityValue = 0;
                        else
                            player->abilityValue = 128;
                        player->onGround = true;
                    }
                    else {
                        self->playerFrameIDs[player->playerID] = 0;
                    }
                }
            }
            else {
                if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn && player->state != Player_State_StartJumpIn) {
                    if (player->onGround || player->velocity.y < 0 || player->state == Player_State_FlyCarried
                        || player->animator.animationID == ANI_SKID) {
                        if (player->onGround) {
                            if (player->animator.animationID != ANI_JUMP)
                                player->direction ^= 1;
                            player->state = Player_State_Ground;
                        }
                    }
                    else {
                        player->state = Player_State_Air;
                    }
                    player->onGround = false;
                }

                self->playerFrameIDs[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1) && !player->onGround) {
                if (
#if RETRO_USE_PLUS
                    player->state == Player_State_MightyHammerDrop ||
#endif
                    player->state == Player_State_BubbleBounce) {
                    player->onGround = true;
                }
                else if ((player->position.y < self->position.y && player->velocity.y >= 0)
                         || (player->position.y >= self->position.y && player->velocity.y <= 0)) {
                    if (player->position.y >= self->position.y) {
                        player->position.y                     = self->position.y + (radius << 10);
                        self->playerAngles[player->playerID] = 0;
                    }
                    else {
                        player->position.y                     = self->position.y - (radius << 10);
                        self->playerAngles[player->playerID] = 512;
                    }
                    player->onGround                         = true;
                    player->velocity.y                       = 0;
                    self->playerFrameIDs[player->playerID] = 1;
                    int32 frame = (ANI_STANDCS - 24 * ((self->playerAngles[player->playerID] + 8) & 0x3FF) / 1024) % ANI_STANDCS;
                    if (player->groundVel)
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame);
                    else
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_STANDCS, &player->animator, true, frame);
                    player->animator.rotationFlag = 1;
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
        int32 top              = self->position.y + (self->hitbox1.top << 16);
        int32 radius           = (self->radius + playerHitbox->bottom) << 6;

        if (self->playerFrameIDs[player->playerID]) {
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

            if (Player_CheckCollisionTouch(player, self, &self->hitbox1)) {
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

                    int32 len   = minVal(abs(player->position.y - top) >> 16, self->length);
                    int32 frame = 0;

                    int32 angle = ((len << 10) / 192 + (((uint32)player->groundVel >> 22) & 0x200)) & 0x3FF;
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

                    player->position.x                     = radius * RSDK.Cos1024(angle) + self->position.x;
                    self->playerAngles[player->playerID] = angle;
                    if (angle >= 512)
                        player->drawOrder = Zone->playerDrawHigh;
                    else
                        player->drawOrder = Zone->playerDrawLow;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame);

                    player->animator.rotationFlag = 1;
                    if (player->position.x < self->position.x)
                        player->abilityValue = 0;
                    else
                        player->abilityValue = 128;
                    player->onGround = true;
                    if (player->jumpPress) {
                        RSDK.PlaySfx(Player->sfxJump, false, 255);
                        if (player->position.x < self->position.x)
                            player->velocity.x = -0x60000;
                        else
                            player->velocity.x = 0x60000;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
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
                    if (player->position.x >= self->position.x) {
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

                    if (player->animator.animationID == ANI_SPRINGCS) {
                        if (abs(player->groundVel) < 0xC0000) {
                            if (abs(player->groundVel) < 0x60000) {
                                if (abs(player->groundVel) < 0x40000)
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_WALK, &player->animator, false, 0);
                                else
                                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JOG, &player->animator, false, 0);
                            }
                            else {
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 1);
                            }
                        }
                        else {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_DASH, &player->animator, false, 1);
                        }
                    }
                    player->onGround       = true;
                    player->tileCollisions = true;
                }
                self->playerFrameIDs[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox1) && player->onGround) {
                if ((player->angle == 64 && player->groundVel > 0) || (player->angle == 192 && player->groundVel < 0)) {
                    if (player->angle == 64 && player->direction == FLIP_X)
                        player->direction = FLIP_NONE;
                    if (player->angle == 192 && player->direction == FLIP_NONE)
                        player->direction = FLIP_X;

                    if (player->position.x >= self->position.x) {
                        player->position.x                     = self->position.x + (radius << 10);
                        self->playerAngles[player->playerID] = 0;
                    }
                    else {
                        player->position.x                     = self->position.x - (radius << 10);
                        self->playerAngles[player->playerID] = 512;
                    }
                    player->onGround                         = true;
                    player->velocity.x                       = 0;
                    self->playerFrameIDs[player->playerID] = 1;
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
    self->skidding = 0;
    if (self->camera)
        self->camera->offsetYFlag = false;
    self->jumpAbilityTimer = 0;

    if (self->jumpPress) {
        Player_StartJump(self);
        if (self->abilityValue == 128) {
            self->velocity.y = -self->velocity.y;
        }
        self->direction ^= 1;
        self->timer = 0;
    }
    else {
        if (abs(self->groundVel) >= 0x8800 && !self->left && !self->right && self->down) {
            if (self->abilityValue == 128)
                self->position.y -= self->cameraOffset;
            else
                self->position.y += self->cameraOffset;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
            self->direction ^= 1;
            self->state = Cylinder_Player_State_Unknown3;
            RSDK.PlaySfx(Player->sfxRoll, false, 255);
        }
    }
}

void Cylinder_Player_State_Unknown3(void)
{
    RSDK_THIS(Player);
    int32 angle     = self->angle;
    self->angle = 0;
    Player_HandleRollDeceleration();
    if (self->state == Player_State_Ground) {
        self->direction ^= 1;
        self->state = Cylinder_Player_State_Unknown2;
    }
    if (self->characterID == ID_TAILS) {
        self->animator.animationSpeed = 120;
    }
    else {
        self->animator.animationSpeed = ((abs(self->groundVel) * 0xF0) / 0x60000) + 0x30;
    }

    if (self->animator.animationSpeed > 0xF0)
        self->animator.animationSpeed = 0xF0;

    if (self->camera)
        self->camera->offsetYFlag = false;
    self->jumpAbilityTimer = 0;
    self->angle            = angle;
    if (self->jumpPress) {
        Player_StartJump(self);
        if (self->abilityValue == 128) {
            self->velocity.y = -self->velocity.y;
        }
        self->timer = 0;
    }
}

void Cylinder_Player_State_Unknown4(void)
{
    RSDK_THIS(Player);
    if (self->onGround) {
        self->rotation = (2 * self->angle);
    }
}

void Cylinder_Player_State_Unknown1(void)
{
    RSDK_THIS(Player);
    self->down = false;
    Player_State_Ground();
}

void Cylinder_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void Cylinder_EditorDraw(void) {}

void Cylinder_EditorLoad(void) {}
#endif

void Cylinder_Serialize(void)
{
    RSDK_EDITABLE_VAR(Cylinder, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Cylinder, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(Cylinder, VAR_ENUM, radius);
}
