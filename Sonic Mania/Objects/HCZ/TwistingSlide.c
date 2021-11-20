#include "SonicMania.h"

ObjectTwistingSlide *TwistingSlide;

void TwistingSlide_Update(void)
{
    RSDK_THIS(TwistingSlide);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        int32 storeX   = player->position.x;
        int32 storeY   = player->position.y;
        if (player->state != Player_State_None)
            self->activePlayers &= ~(1 << playerID);

        if (!Player_CheckCollisionTouch(player, self, &self->hitbox1) || (player->position.y < self->field_74)
            || player->position.y > self->field_78) {
            self->activePlayers &= ~(1 << playerID);
        }
        else if (self->direction) {
            switch (self->type) {
                case 0:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->field_64[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        int32 val = 221 * self->field_64[playerID];
                        if (221 * self->field_64[playerID] >= 0x3FC0)
                            val = 170 * self->field_64[playerID];
                        player->position.x = 0x2800 * RSDK.Cos256((val >> 8) + 192) + self->position.x;
                        player->position.y = self->position.y + ((self->field_64[playerID] - 74) << 16);
                        if (player->groundVel > 0 && Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_ROOF;
                            player->tileCollisions = true;
                            player->angle          = 144;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel < 0) {
                        if (Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                            player->tileCollisions = false;
                            player->state          = Player_State_None;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            self->field_64[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= (1 << playerID);
                        }
                    }
                    break;
                case 1:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->field_64[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0x800000 + self->position.x - 0xAA00 * self->field_64[playerID];
                        player->position.y = self->position.y + ((self->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 2:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->field_64[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0x2800 * RSDK.Cos256(((170 * self->field_64[playerID]) >> 8) + 64) + self->position.x;
                        player->position.y = self->position.y + ((self->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 3:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->field_64[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = self->position.x + ((self->endLen / 3) << 16) - 0xAA00 * self->field_64[playerID];
                        player->position.y = self->position.y + (self->field_64[playerID] << 16) - (self->endLen << 15);
                        if (player->groundVel < 0 && Player_CheckCollisionTouch(player, self, &self->hitbox3)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_RWALL;
                            player->tileCollisions = true;
                            player->angle          = 216;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel > 0
                             && Player_CheckCollisionTouch(player, self, &self->hitbox3)) {
                        player->tileCollisions = false;
                        player->state          = Player_State_None;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        self->field_64[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                        self->activePlayers |= (1 << playerID);
                    }
                    break;
                default: break;
            }
        }
        else {
            switch (self->type) {
                case 0:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->field_64[playerID] = (player->position.y - self->position.y + 4849664) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        int32 val = 221 * self->field_64[playerID];
                        if (221 * self->field_64[playerID] >= 16320)
                            val = 170 * self->field_64[playerID];
                        player->position.x = self->position.x - 0x2800 * RSDK.Cos256((val >> 8) + 192);
                        player->position.y = self->position.y + ((self->field_64[playerID] - 74) << 16);
                        if (player->groundVel < 0 && Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_ROOF;
                            player->tileCollisions = true;
                            player->angle          = 112;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel > 0) {
                        if (Player_CheckCollisionTouch(player, self, &self->hitbox2)) {
                            player->tileCollisions = false;
                            player->state          = Player_State_None;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            self->field_64[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= (1 << playerID);
                        }
                    }
                    break;
                case 1:
                    if (player->state == Player_State_None) {
                        if (((1 << playerID) & self->activePlayers) == 0) {
                            self->field_64[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0xAA00 * self->field_64[playerID] + self->position.x - 0x800000;
                        player->position.y = self->position.y + ((self->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 2:
                    if (player->state == Player_State_None) {
                        if (((1 << playerID) & self->activePlayers) == 0) {
                            self->field_64[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = self->position.x - 0x2800 * RSDK.Cos256(((170 * self->field_64[playerID]) >> 8) + 64);
                        player->position.y = self->position.y + ((self->field_64[playerID] - 192) << 16);
                    }
                    break;
                case 3:
                    if (player->state == Player_State_None) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->field_64[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = self->position.x + 0xAA00 * self->field_64[playerID] - ((self->endLen / 3) << 16);
                        player->position.y = self->position.y + (self->field_64[playerID] << 16) - (self->endLen << 15);
                        if (player->groundVel > 0 && Player_CheckCollisionTouch(player, self, &self->hitbox3)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_RWALL;
                            player->tileCollisions = true;
                            player->angle          = 40;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel < 0
                             && Player_CheckCollisionTouch(player, self, &self->hitbox3)) {
                        player->tileCollisions = false;
                        player->state          = Player_State_None;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        self->field_64[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                        self->activePlayers |= (1 << playerID);
                    }
                    break;
                default: break;
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            if (self->direction) {
                self->field_64[playerID] += ((-player->groundVel >> 16) * RSDK.Sin256(40)) >> 8;
                if (player->groundVel <= 0)
                    player->groundVel -= 20 * RSDK.Cos256(40);
                else
                    player->groundVel -= 80 * RSDK.Cos256(40);
                if (player->groundVel < -0x180000)
                    player->groundVel = -0x180000;
            }
            else {
                self->field_64[playerID] += ((player->groundVel >> 16) * RSDK.Sin256(40)) >> 8;
                if (player->groundVel <= 0)
                    player->groundVel += 20 * RSDK.Cos256(40);
                else
                    player->groundVel += 80 * RSDK.Cos256(40);
                if (player->groundVel > 0x180000)
                    player->groundVel = 0x180000;
            }

            if (player->characterID == ID_TAILS) {
                player->animator.animationSpeed = 120;
            }
            else {
                player->animator.animationSpeed = ((abs(player->groundVel) * 0xF0) / 0x60000) + 0x30;
            }
            if (player->animator.animationSpeed > 0xF0)
                player->animator.animationSpeed = 0xF0;

            int32 x = player->position.x - storeX;
            int32 y = player->position.y - storeY;
            if (player->position.x != storeX && y) {
                if (player->groundVel <= 0) {
                    y = storeY - player->position.y;
                    x = storeX - player->position.x;
                }
                player->angle    = RSDK.ATan2(x, y);
                player->rotation = 2 * player->angle;
            }
        }
    }
}

void TwistingSlide_LateUpdate(void) {}

void TwistingSlide_StaticUpdate(void) {}

void TwistingSlide_Draw(void) {}

void TwistingSlide_Create(void *data)
{
    RSDK_THIS(TwistingSlide);
    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_BOUNDS;
        switch (self->type) {
            case 0:
                self->updateRange.y = 0x780000;
                self->updateRange.x = 0x800000;
                self->field_74      = self->position.y - 0x4A0000;
                self->field_78      = 0x780000 + self->position.y;
                break;
            case 1:
                self->updateRange.x = 0xC00000;
                self->updateRange.y = 0xC00000;
                self->field_74      = self->position.y - 0xC00000;
                self->field_78      = self->position.y + 0xC00000;
                break;
            case 2:
                self->updateRange.y = 0xC00000;
                self->updateRange.x = 0x800000;
                self->field_74      = self->position.y - 0xC00000;
                self->field_78      = self->position.y + 0xC00000;
                break;
            case 3:
                self->field_78      = self->position.y + (self->endLen << 15);
                self->field_74      = self->position.y - (self->endLen << 15);
                self->updateRange.y = 0xC00000;
                self->updateRange.x = 0x800000;
                break;
            default: break;
        }
        TwistingSlide_Unknown1();
    }
}

void TwistingSlide_StageLoad(void)
{
    TwistingSlide->hitbox.left   = -1;
    TwistingSlide->hitbox.top    = 1;
    TwistingSlide->hitbox.right  = 1;
    TwistingSlide->hitbox.bottom = 1;
}

void TwistingSlide_Unknown1(void)
{
    RSDK_THIS(TwistingSlide);
    switch (self->type) {
        case 0:
            self->hitbox1.left   = -56;
            self->hitbox1.top    = -120;
            self->hitbox1.right  = 52;
            self->hitbox1.bottom = 120;
            self->hitbox2.top    = -46;
            self->hitbox2.bottom = -38;
            self->hitbox2.left   = ((-40 * RSDK.Cos256(48)) >> 8) - 4;
            self->hitbox2.right  = ((-40 * RSDK.Cos256(48)) >> 8) + 4;
            break;
        case 1:
        case 2:
            self->hitbox1.left   = -132;
            self->hitbox1.top    = -196;
            self->hitbox1.right  = 132;
            self->hitbox1.bottom = 196;
            break;
        case 3:
            self->hitbox1.left   = self->endLen / -3;
            self->hitbox1.top    = -(self->endLen >> 1);
            self->hitbox1.right  = self->endLen / 3;
            self->hitbox1.bottom = self->endLen >> 1;
            self->hitbox3.left   = self->hitbox1.right - 4;
            self->hitbox3.top    = self->hitbox1.bottom - 4;
            self->hitbox3.right  = self->hitbox1.right + 4;
            self->hitbox3.bottom = self->hitbox1.bottom + 4;
            break;
        default: break;
    }
}

#if RETRO_INCLUDE_EDITOR
void TwistingSlide_EditorDraw(void) {}

void TwistingSlide_EditorLoad(void) {}
#endif

void TwistingSlide_Serialize(void)
{
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_ENUM, endLen);
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_UINT8, direction);
}
