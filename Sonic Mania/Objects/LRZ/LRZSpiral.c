#include "SonicMania.h"

ObjectLRZSpiral *LRZSpiral;

void LRZSpiral_Update(void)
{
    RSDK_THIS(LRZSpiral);
    StateMachine_Run(self->state);
}

void LRZSpiral_LateUpdate(void) {}

void LRZSpiral_StaticUpdate(void) {}

void LRZSpiral_Draw(void) {}

void LRZSpiral_Create(void *data)
{
    RSDK_THIS(LRZSpiral);
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_BOUNDS;
        self->visible   = false;
        self->drawOrder = Zone->drawOrderLow;

        switch (self->type) {
            default: break;
            case 0: {
                self->state         = LRZSpiral_Unknown2;
                self->updateRange.x = 0x800000;
                self->updateRange.y = (self->height + 1) << 22;
                self->hitbox.left   = -16;
                self->hitbox.right  = 16;
                int32 height          = self->height << 25 >> 19;
                self->hitbox.top    = -height;
                self->hitbox.bottom = 128 - height;
                self->height        = (self->height << 25) - 0x1000000;
                break;
            }
            case 1:
                self->direction     = FLIP_NONE;
                self->updateRange.y = (self->height + 64) << 16;
                self->hitbox.left   = -112;
                self->hitbox.top    = -180;
                self->hitbox.right  = -104;
                self->hitbox.bottom = -172;
                self->state         = LRZSpiral_Unknown3;
                self->updateRange.x = 0x800000;
                break;
            case 2:
                self->direction     = FLIP_X;
                self->updateRange.y = (self->height + 64) << 16;
                self->hitbox.left   = -146;
                self->hitbox.top    = -180;
                self->hitbox.right  = -138;
                self->hitbox.bottom = -172;
                self->state         = LRZSpiral_Unknown4;
                self->updateRange.x = 0x800000;
                break;
        }
    }
}

void LRZSpiral_StageLoad(void) {}

void LRZSpiral_Unknown1(void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    if (abs(player->groundVel) < player->maxRunSpeed) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 1);
        player->animator.speed = (abs(player->groundVel) >> 12) + 96;
        if (player->animator.speed > 0x200)
            player->animator.speed = 0x200;
        player->maxJogSpeed = 0x58000;
        player->maxRunSpeed = 0xC0000;
    }
    else {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_DASH, &player->animator, false, 1);
        player->maxRunSpeed = 0xB8000;
    }
}
void LRZSpiral_Unknown2(void)
{
    RSDK_THIS(LRZSpiral);
    foreach_active(Player, player)
    {
        int32 pID = RSDK.GetEntityID(player);
        if ((1 << pID) & self->activePlayers) {
            if (player->state == Player_State_None) {
                self->playerVelocity[pID] += player->groundVel;
                if (self->playerVelocity[pID] < 0) {
                    self->activePlayers &= ~(1 << pID);
                }
                else if (self->playerVelocity[pID] < self->height) {
                    if (player->groundVel < 0x100000)
                        player->groundVel += 0x1000;
                    if ((uint8)((self->playerVelocity[pID] >> 17) + 64) < 0x80)
                        player->drawOrder = Zone->playerDrawHigh;
                    else
                        player->drawOrder = Zone->playerDrawLow;
                    player->position.x = 0x4800 * RSDK.Sin256(self->playerVelocity[pID] >> 17) + self->position.x;
                    player->position.y = self->playerVelocity[pID] >> 2;
                    int32 pos          = (player->position.y >> 16) & 0x7F;
                    player->position.y = player->position.y & 0xFF800000;
                    player->position.y += 0x580000 + self->position.y - ((self->height + 0x1000000) >> 3);
                    if (self->playerVelocity[pID] >= self->height - 0x1000000)
                        player->position.y += LRZSpiral->array2[pos];
                    else
                        player->position.y += LRZSpiral->array1[pos];
                    player->animator.frameID = (pos << 8) / 0x555;
                    player->direction              = (player->animator.frameID + 6) % 24 > 12;
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
                    self->activePlayers &= ~(1 << pID);
                    LRZSpiral_Unknown1(player);
                    if (!self->activePlayers)
                        self->active = ACTIVE_BOUNDS;
                }
            }
            else {
                self->activePlayers &= ~(1 << pID);
            }
        }
        else if (player->position.x >= self->position.x) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (player->groundVel < 0x60000)
                    player->groundVel = 0x60000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPIRALRUN, &player->animator, false, 0);
                player->animator.speed = 0;
                player->state                         = Player_State_None;
                player->nextAirState                  = StateMachine_None;
                player->nextGroundState               = StateMachine_None;
                player->tileCollisions                = false;
                self->activePlayers |= 1 << pID;
                self->playerVelocity[pID] = 0;
                self->active              = ACTIVE_NORMAL;
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
        if ((1 << pID) & self->activePlayers) {
            if (player->state == Player_State_None) {
                self->playerVelocity[pID] += player->groundVel;
                if (self->playerVelocity[pID] < 0) {
                    self->activePlayers &= ~(1 << pID);
                }
                else {
                    self->playerVelocity[pID] += player->groundVel >> 17;
                    if (player->groundVel < 0x100000)
                        player->groundVel += 0x1000;
                    player->position.x = 0xA000 * RSDK.Sin256(self->playerVelocity[pID] >> 18);
                    player->position.x += self->position.x - 0x6C0000;
                    player->position.y = self->position.y - 0xB00000 + 0x15800 * ((256 - RSDK.Cos256(self->playerVelocity[pID] >> 18)) >> 1);
                    if (self->playerVelocity[pID] < 0 || self->playerVelocity[pID] >= 0x2000000) {
                        player->tileCollisions = true;
                        player->collisionPlane = 1;
                        player->controlLock    = 16;
                        player->direction      = FLIP_X;
                        player->onGround       = true;
                        player->groundedStore  = true;
                        player->state          = Player_State_Ground;
                        player->groundVel      = -player->groundVel;
                        self->activePlayers &= ~(1 << pID);
                        LRZSpiral_Unknown1(player);
                        if (!self->activePlayers)
                            self->active = ACTIVE_BOUNDS;
                    }
                    else {
                        player->animator.frameID = self->playerVelocity[pID] / 0x2AAAAA;
                    }
                }
            }
            else {
                self->activePlayers &= ~(1 << pID);
            }
        }
        else if (player->position.x >= self->position.x - 0x6C0000 && player->groundVel >= 0 && player->onGround) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (player->groundVel < 0x40000)
                    player->groundVel = 0x40000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTRUN, &player->animator, false, 0);
                player->animator.speed = 0;
                player->state                         = Player_State_None;
                player->nextAirState                  = StateMachine_None;
                player->nextGroundState               = StateMachine_None;
                player->tileCollisions                = false;
                self->activePlayers |= 1 << pID;
                self->playerVelocity[pID] = 0;
                self->active              = ACTIVE_NORMAL;
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
        if ((1 << playerID) & self->activePlayers) {
            if (player->state == Player_State_None) {
                self->playerVelocity[playerID] -= player->groundVel;
                if (self->playerVelocity[playerID] < 0) {
                    self->activePlayers &= ~(1 << playerID);
                }
                else {
                    self->playerVelocity[playerID] += player->groundVel >> 17;
                    if (player->groundVel > -0x100000)
                        player->groundVel -= 0x1000;
                    player->position.x = -0xA000 * RSDK.Sin256(self->playerVelocity[playerID] >> 18);
                    player->position.x += 0x8E0000 + self->position.x;
                    player->position.y =
                        self->position.y - 0xB40000 + 0x14200 * ((0x100 - RSDK.Cos256(self->playerVelocity[playerID] >> 18)) >> 1);
                    if (self->playerVelocity[playerID] < 0 || self->playerVelocity[playerID] >= 0x2000000) {
                        player->tileCollisions = true;
                        player->collisionPlane = 1;
                        player->controlLock    = 16;
                        player->direction      = FLIP_NONE;
                        player->onGround       = true;
                        player->groundedStore  = true;
                        player->state          = Player_State_Ground;
                        player->groundVel      = -player->groundVel;
                        self->activePlayers &= ~(1 << playerID);
                        LRZSpiral_Unknown1(player);
                        if (!self->activePlayers)
                            self->active = ACTIVE_BOUNDS;
                    }
                    else {
                        player->animator.frameID = self->playerVelocity[playerID] / 0x2AAAAA;
                    }
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
        else if (player->position.x <= 0x8E0000 + self->position.x && player->groundVel <= 0 && player->onGround) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                if (player->groundVel > -0x40000)
                    player->groundVel = -0x40000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTRUN, &player->animator, false, 0);
                player->animator.speed = 0;
                player->state                         = Player_State_None;
                player->nextAirState                  = StateMachine_None;
                player->nextGroundState               = StateMachine_None;
                player->tileCollisions                = false;
                self->activePlayers |= 1 << playerID;
                self->playerVelocity[playerID] = 0;
                self->active                   = ACTIVE_NORMAL;
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void LRZSpiral_EditorDraw(void)
{
    RSDK_THIS(LRZSpiral);

    int32 height = self->height + 64;
    switch (self->type) {
        default: break;
        case 0: {
            height                = self->height << 25 >> 19;
            self->updateRange.x = 0x800000;
            self->updateRange.y = (self->height + 1) << 22;
            self->hitbox.left   = -16;
            self->hitbox.right  = 16;
            self->hitbox.top    = -height;
            self->hitbox.bottom = 128 - height;
            height <<= 1;
            break;
        }
        case 1:
            self->direction     = FLIP_NONE;
            self->updateRange.y = height << 16;
            self->hitbox.left   = -112;
            self->hitbox.top    = -180;
            self->hitbox.right  = -104;
            self->hitbox.bottom = -172;
            self->updateRange.x = 0x800000;
            break;
        case 2:
            self->direction     = FLIP_X;
            self->updateRange.y = height << 16;
            self->updateRange.x = 0x800000;
            self->hitbox.left   = -146;
            self->hitbox.top    = -180;
            self->hitbox.right  = -138;
            self->hitbox.bottom = -172;
            break;
    }

    DrawHelpers_DrawHitboxOutline(0xFF0000, self->direction, self->position.x, self->position.y, &self->hitbox);
    DrawHelpers_DrawRectOutline(0xFFFF00, self->position.x, self->position.y, self->radius << 17, height << 16);
}

void LRZSpiral_EditorLoad(void) {}
#endif

void LRZSpiral_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_ENUM, radius);
}
