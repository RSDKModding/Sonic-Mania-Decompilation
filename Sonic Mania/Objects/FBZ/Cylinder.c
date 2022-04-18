// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Cylinder Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

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
        self->drawOrder = Zone->objectDrawLow;
        switch (self->type) {
            case CYLINDER_TUBE_H:
                self->updateRange.x = self->length << 16;
                self->updateRange.y = self->radius << 16;

                self->hitboxRange.left   = -self->length;
                self->hitboxRange.top    = -self->radius;
                self->hitboxRange.right  = self->length;
                self->hitboxRange.bottom = self->radius;

                self->hitboxSize.left   = -self->length;
                self->hitboxSize.top    = -self->radius;
                self->hitboxSize.right  = self->length;
                self->hitboxSize.bottom = -self->radius + 8;

                self->state          = Cylinder_State_TubeH;
                break;
            case CYLINDER_TUBE_V:
                self->updateRange.x  = self->radius << 16;
                self->updateRange.y  = self->length << 16;

                self->hitboxRange.left   = -self->radius;
                self->hitboxRange.top    = -self->length;
                self->hitboxRange.right  = self->radius;
                self->hitboxRange.bottom = self->length;

                self->state          = Cylinder_State_TubeV;
                break;
            case CYLINDER_SPIRAL:
                self->updateRange.x  = (self->radius + 112) << 16;
                self->updateRange.y  = self->length << 16;

                self->hitboxRange.left   = -64 - self->radius;
                self->hitboxRange.right  = self->radius + 64;
                self->hitboxRange.top    = -self->length;
                self->hitboxRange.bottom = self->length;

                self->hitboxL.left   = -112 - self->radius;
                self->hitboxL.right  = -self->radius;
                self->hitboxL.top    = -self->length;
                self->hitboxL.bottom = 64 - self->length;

                self->hitboxR.left   = self->radius;
                self->hitboxR.top    = self->length - 64;
                self->hitboxR.right  = self->length + 112;
                self->hitboxR.bottom = self->length;

                self->state          = Cylinder_State_Spiral;
                break;
            case CYLINDER_SPIRAL_FLIPPED:
                self->updateRange.x = (self->radius + 112) << 16;
                self->updateRange.y = self->length << 16;

                self->hitboxRange.left   = -64 - self->radius;
                self->hitboxRange.top    = -self->length;
                self->hitboxRange.right  = self->radius + 64;
                self->hitboxRange.bottom = self->length;

                self->hitboxL.left   = -112 - self->radius;
                self->hitboxL.top    = self->length - 64;
                self->hitboxL.right  = -self->radius;
                self->hitboxL.bottom = self->length;

                self->hitboxR.left   = self->radius;
                self->hitboxR.top    = -self->length;
                self->hitboxR.right  = self->radius + 112;
                self->hitboxR.bottom = 64 - self->length;

                self->state = Cylinder_State_Spiral;
                break;
            case CYLINDER_INKROLLER:
                self->updateRange.x  = self->length << 16;
                self->updateRange.y  = self->radius << 16;

                self->hitboxRange.left   = 16 - self->length;
                self->hitboxRange.top    = ~self->radius;
                self->hitboxRange.right  = self->length - 16;
                self->hitboxRange.bottom = self->radius + 1;

                self->hitboxSize.left   = -self->length;
                self->hitboxSize.top    = -self->radius;
                self->hitboxSize.right  = self->length;
                self->hitboxSize.bottom = self->radius;

                self->state          = Cylinder_State_InkRoller;
                break;
            case CYLINDER_PILLAR:
                self->visible        = true;
                self->updateRange.x  = self->radius << 17;
                self->updateRange.y  = self->length << 17;

                self->hitboxRange.left   = -16 - self->radius;
                self->hitboxRange.top    = -self->length;
                self->hitboxRange.right  = self->radius + 16;
                self->hitboxRange.bottom = self->length;
                self->length         = 2 * self->length;

                self->state          = Cylinder_State_Pillar;
                break;
            default: break;
        }
    }
}

void Cylinder_StageLoad(void) {}

void Cylinder_State_TubeH(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 radius           = (self->radius - playerHitbox->bottom) << 6;

        if (self->playerStatuses[player->playerID]) {
            if (!Player_CheckCollisionTouch(player, self, &self->hitboxRange) || player->velocity.y <= -0x40000) {
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
                self->playerStatuses[player->playerID] = 0;
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
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
                if (player->position.y - self->position.y > radius << 10 && player->velocity.y >= 0) {
                    player->position.y                       = self->position.y + (radius << 10);
                    player->onGround                         = true;
                    player->velocity.y                       = 0;
                    self->playerAngles[player->playerID]   = 0;
                    self->playerStatuses[player->playerID] = 1;
                }
            }
        }
    }
}

void Cylinder_State_TubeV(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 radius           = (self->radius - playerHitbox->bottom) << 6;

        if (self->playerStatuses[player->playerID]) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
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
                self->playerStatuses[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
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
                    self->playerStatuses[player->playerID] = 1;
                }
            }
        }
    }
}

void Cylinder_State_Spiral(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 radius           = (self->radius - playerHitbox->bottom - 12) << 6;

        switch (self->playerStatuses[player->playerID]) {
            case 0:
                if (player->onGround) {
                    bool32 collided = false;
                    if (Player_CheckCollisionTouch(player, self, &self->hitboxL) && player->groundVel > 0x30000) 
                        collided = true;
                    if (Player_CheckCollisionTouch(player, self, &self->hitboxR) && player->groundVel < -0x30000)
                        collided = true;

                    if (collided) {
                        self->playerStatuses[player->playerID] = 1;
                        player->state                            = Cylinder_Player_State_Spiral;
                    }
                }
                break;

            case 1:
                if (player->groundVel <= 0x30000) {
                    if (player->groundVel >= -0x30000) {
                        player->onGround                         = false;
                        self->playerStatuses[player->playerID] = 0;
                    }
                    else {
                        if (player->position.x <= self->position.x) {
                            self->playerAngles[player->playerID]   = 768;
                            player->tileCollisions                   = false;
                            self->playerStatuses[player->playerID] = 2;
                        }
                        int32 distance = (player->position.x - self->position.x) >> 16;
                        int32 right    = self->hitboxR.right - 48;
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
                        self->playerStatuses[player->playerID] = 2;
                    }
                    int32 distance = (self->position.x - player->position.x) >> 16;
                    int32 right    = -48 - self->hitboxL.left;
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

            case 2: {
                self->playerAngles[player->playerID] = (self->playerAngles[player->playerID] + (player->groundVel >> 15)) & 0x3FF;
                player->position.x                     = radius * RSDK.Cos1024(self->playerAngles[player->playerID]) + self->position.x;

                if (self->hitboxL.top >= self->hitboxR.top)
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
                    self->playerStatuses[player->playerID] = 0;
                    player->tileCollisions                 = true;
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame % 24);
                    player->animator.rotationFlag = 1;
                    player->onGround                    = true;
                    player->velocity.y                  = 0;

                    if (player->groundVel <= 0) {
                        if (player->groundVel >= -0x30000) {
                            self->playerStatuses[player->playerID] = 0;
                            player->tileCollisions                   = true;
                        }
                        else {
                            if (self->hitboxL.top < self->hitboxR.top) {
                                int32 maxY = self->position.y - (playerHitbox->bottom << 16) - self->updateRange.y + 0x400000;
                                if (player->position.y <= maxY) {
                                    player->position.y                       = maxY;
                                    self->playerStatuses[player->playerID] = 3;
                                    player->tileCollisions                   = true;
                                }
                            }
                            else {
                                if (player->position.y >= self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y) {
                                    player->position.y = self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y;
                                    self->playerStatuses[player->playerID] = 3;
                                    player->tileCollisions                   = true;
                                }
                            }
                        }
                    }
                    else {
                        if (player->groundVel <= 0x30000) {
                            self->playerStatuses[player->playerID] = 0;
                            player->tileCollisions                   = true;
                        }
                        else if (self->hitboxL.top >= self->hitboxR.top) {
                            int32 maxY = self->position.y - (playerHitbox->bottom << 16) - self->updateRange.y + 0x400000;
                            if (player->position.y <= maxY) {
                                player->position.y                       = maxY;
                                self->playerStatuses[player->playerID] = 3;
                                player->tileCollisions                   = true;
                            }
                        }
                        else {
                            if (player->position.y >= self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y) {
                                player->position.y                       = self->updateRange.y - (playerHitbox->bottom << 16) + self->position.y;
                                self->playerStatuses[player->playerID] = 3;
                                player->tileCollisions                   = true;
                            }
                        }
                    }
                }
                break;
            }

            case 3: {
                if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
                    if (abs(player->groundVel) <= 0x30000) {
                        self->playerStatuses[player->playerID] = 0;
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
                            int32 right    = self->hitboxR.right - 48;
                            if (distance < right) {
                                player->rotation  = 256;
                                player->direction = FLIP_X;
                                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, 17 - 6 * distance / right);
                                player->animator.rotationFlag = 1;
                            }
                        }
                        else {
                            int32 distance = (self->position.x - player->position.x) >> 16;
                            int32 right    = -48 - self->hitboxL.left;
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
                    self->playerStatuses[player->playerID] = 0;
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
            }

            default: break;
        }
    }
}

void Cylinder_State_InkRoller(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 radius           = (self->radius + playerHitbox->bottom) << 6;
        if (self->playerStatuses[player->playerID]) {
            int32 angle = (self->playerAngles[player->playerID] + 8) & 0x3FF;
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange) && !player->onGround) {
                if ((player->position.y < self->position.y && player->velocity.y >= 0)
                    || (player->position.y >= self->position.y && player->velocity.y <= 0)) {

                    bool32 collided = true;
                    if (player->state == Cylinder_Player_State_InkRoller_Stand) {
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
                        if (player->state != Cylinder_Player_State_InkRoller_Roll) {
                            collided = false;
                        }
                        else {
                            if (!player->velocity.x)
                                player->direction = player->velocity.x >> 31;
                        }
                    }

                    if (collided) {
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
                        self->playerStatuses[player->playerID] = 0;
                    }
                }
            }
            else {
                if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn && player->state != Player_State_StartJumpIn) {
                    if (player->onGround || player->velocity.y < 0 || player->state == Player_State_FlyCarried
                        || player->animator.animationID == ANI_SKID) {
                        if (player->onGround) {
                            if (player->animator.animationID != ANI_JUMP)
                                player->direction ^= FLIP_X;
                            player->state = Player_State_Ground;
                        }
                    }
                    else {
                        player->state = Player_State_Air;
                    }
                    player->onGround = false;
                }

                self->playerStatuses[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange) && !player->onGround) {
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
                    player->onGround                       = true;
                    player->velocity.y                     = 0;
                    self->playerStatuses[player->playerID] = 1;
                    int32 frame = (ANI_STANDCS - 24 * ((self->playerAngles[player->playerID] + 8) & 0x3FF) / 1024) % ANI_STANDCS;
                    if (player->groundVel)
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRINGCS, &player->animator, true, frame);
                    else
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_STANDCS, &player->animator, true, frame);
                    player->animator.rotationFlag = 1;
                    player->rotation              = 256;
                    player->direction ^= FLIP_X;
                    player->nextAirState    = StateMachine_None;
                    player->nextGroundState = StateMachine_None;
                    player->state           = Cylinder_Player_State_InkRoller_Stand;
                }
            }
        }
    }
}

void Cylinder_State_Pillar(void)
{
    RSDK_THIS(Cylinder);

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        int32 top              = self->position.y + (self->hitboxRange.top << 16);
        int32 radius           = (self->radius + playerHitbox->bottom) << 6;

        if (self->playerStatuses[player->playerID]) {
            bool32 hasInput = false;
            if (player->groundVel > 0) {
                if (player->left)
                    hasInput = true;
            }
            else if (player->groundVel < 0) {
                if (!player->right) {
                    if (player->left)
                        hasInput = true;
                }
                else if (player->right) {
                    hasInput = true;
                }
            }

            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange)) {
                if (player->onGround) {
                    if (hasInput) {
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
                        if (hasInput || player->groundVel < player->velocity.y)
                            player->groundVel = player->velocity.y;
                    }
                    else {
                        player->angle     = 192;
                        player->direction = FLIP_X;
                        if (hasInput || player->groundVel > -player->velocity.y)
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
                self->playerStatuses[player->playerID] = 0;
            }
        }
        else {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxRange) && player->onGround) {
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
                    self->playerStatuses[player->playerID] = 1;
                    player->nextAirState                     = 0;
                    player->nextGroundState                  = 0;
                    player->state                            = Cylinder_Player_State_Pillar;
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

void Cylinder_Player_State_InkRoller_Stand(void)
{
    RSDK_THIS(Player);
    Player_HandleGroundMovement();
    self->skidding = 0;
    if (self->camera)
        self->camera->disableYOffset = false;
    self->jumpAbilityState = 0;

    if (self->jumpPress) {
        Player_StartJump(self);
        if (self->abilityValue == 128) {
            self->velocity.y = -self->velocity.y;
        }
        self->direction ^= FLIP_X;
        self->timer = 0;
    }
    else {
        if (abs(self->groundVel) >= 0x8800 && !self->left && !self->right && self->down) {
            if (self->abilityValue == 128)
                self->position.y -= self->jumpOffset;
            else
                self->position.y += self->jumpOffset;

            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, false, 0);
            self->direction ^= FLIP_X;
            self->state = Cylinder_Player_State_InkRoller_Roll;
            RSDK.PlaySfx(Player->sfxRoll, false, 255);
        }
    }
}

void Cylinder_Player_State_InkRoller_Roll(void)
{
    RSDK_THIS(Player);
    int32 angle     = self->angle;
    self->angle = 0;
    Player_HandleRollDeceleration();
    if (self->state == Player_State_Ground) {
        self->direction ^= FLIP_X;
        self->state = Cylinder_Player_State_InkRoller_Stand;
    }
    if (self->characterID == ID_TAILS) {
        self->animator.speed = 120;
    }
    else {
        self->animator.speed = ((abs(self->groundVel) * 0xF0) / 0x60000) + 0x30;
    }

    if (self->animator.speed > 0xF0)
        self->animator.speed = 0xF0;

    if (self->camera)
        self->camera->disableYOffset = false;
    self->jumpAbilityState = 0;
    self->angle            = angle;
    if (self->jumpPress) {
        Player_StartJump(self);
        if (self->abilityValue == 128) {
            self->velocity.y = -self->velocity.y;
        }
        self->timer = 0;
    }
}

void Cylinder_Player_State_Pillar(void)
{
    RSDK_THIS(Player);
    if (self->onGround) {
        self->rotation = (2 * self->angle);
    }
}

void Cylinder_Player_State_Spiral(void)
{
    RSDK_THIS(Player);
    self->down = false;
    Player_State_Ground();
}

#if RETRO_INCLUDE_EDITOR
void Cylinder_EditorDraw(void)
{
    RSDK_THIS(Cylinder);

    RSDK.DrawSprite(&Cylinder->animator, NULL, false);

    
        switch (self->type) {
        case CYLINDER_TUBE_H:
            self->updateRange.x = self->length << 16;
            self->updateRange.y = self->radius << 16;

            self->hitboxRange.left   = -self->length;
            self->hitboxRange.top    = -self->radius;
            self->hitboxRange.right  = self->length;
            self->hitboxRange.bottom = self->radius;
            break;
        case CYLINDER_TUBE_V:
            self->updateRange.x = self->radius << 16;
            self->updateRange.y = self->length << 16;

            self->hitboxRange.left   = -self->radius;
            self->hitboxRange.top    = -self->length;
            self->hitboxRange.right  = self->radius;
            self->hitboxRange.bottom = self->length;
            break;
        case CYLINDER_SPIRAL:
            self->updateRange.x = (self->radius + 112) << 16;
            self->updateRange.y = self->length << 16;

            self->hitboxRange.left   = -64 - self->radius;
            self->hitboxRange.right  = self->radius + 64;
            self->hitboxRange.top    = -self->length;
            self->hitboxRange.bottom = self->length;

            self->hitboxL.left   = -112 - self->radius;
            self->hitboxL.right  = -self->radius;
            self->hitboxL.top    = -self->length;
            self->hitboxL.bottom = 64 - self->length;

            self->hitboxR.left   = self->radius;
            self->hitboxR.top    = self->length - 64;
            self->hitboxR.right  = self->length + 112;
            self->hitboxR.bottom = self->length;
            break;
        case CYLINDER_SPIRAL_FLIPPED:
            self->updateRange.x = (self->radius + 112) << 16;
            self->updateRange.y = self->length << 16;

            self->hitboxRange.left   = -64 - self->radius;
            self->hitboxRange.top    = -self->length;
            self->hitboxRange.right  = self->radius + 64;
            self->hitboxRange.bottom = self->length;

            self->hitboxL.left   = -112 - self->radius;
            self->hitboxL.top    = self->length - 64;
            self->hitboxL.right  = -self->radius;
            self->hitboxL.bottom = self->length;

            self->hitboxR.left   = self->radius;
            self->hitboxR.top    = -self->length;
            self->hitboxR.right  = self->radius + 112;
            self->hitboxR.bottom = 64 - self->length;
            break;
        case CYLINDER_INKROLLER:
            self->updateRange.x = self->length << 16;
            self->updateRange.y = self->radius << 16;

            self->hitboxRange.left   = 16 - self->length;
            self->hitboxRange.top    = ~self->radius;
            self->hitboxRange.right  = self->length - 16;
            self->hitboxRange.bottom = self->radius + 1;
            break;
        case CYLINDER_PILLAR:
            self->updateRange.x = self->radius << 17;
            self->updateRange.y = self->length << 17;

            self->hitboxRange.left   = -16 - self->radius;
            self->hitboxRange.top    = -self->length;
            self->hitboxRange.right  = self->radius + 16;
            self->hitboxRange.bottom = self->length;
            break;
        default: break;
    }

    if (showGizmos()) {
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxRange, FLIP_NONE, 0xFFFF00);

        if (self->type == CYLINDER_SPIRAL || self->type == CYLINDER_SPIRAL_FLIPPED) {
            DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxL, FLIP_NONE, 0xFF0000);
            DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxR, FLIP_NONE, 0xFF0000);
        }
    }
}

void Cylinder_EditorLoad(void)
{
    Cylinder->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Cylinder.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(Cylinder->aniFrames, 0, &Cylinder->animator, true, 0);

    RSDK_ACTIVE_VAR(Cylinder, type);
    RSDK_ENUM_VAR("Tube H", CYLINDER_TUBE_H);
    RSDK_ENUM_VAR("Tube V", CYLINDER_TUBE_V);
    RSDK_ENUM_VAR("FBZ Spiral", CYLINDER_SPIRAL);
    RSDK_ENUM_VAR("FBZ Spiral (Flipped)", CYLINDER_SPIRAL_FLIPPED);
    RSDK_ENUM_VAR("Ink Roller", CYLINDER_INKROLLER);
    RSDK_ENUM_VAR("(Unused)", CYLINDER_UNUSED1);
    RSDK_ENUM_VAR("(Unused)", CYLINDER_UNUSED2);
    RSDK_ENUM_VAR("SSZ1 Pillar", CYLINDER_PILLAR);
}
#endif

void Cylinder_Serialize(void)
{
    RSDK_EDITABLE_VAR(Cylinder, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Cylinder, VAR_ENUM, length);
    RSDK_EDITABLE_VAR(Cylinder, VAR_ENUM, radius);
}
