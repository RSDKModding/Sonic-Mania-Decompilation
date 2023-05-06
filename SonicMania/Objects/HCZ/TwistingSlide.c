// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TwistingSlide Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTwistingSlide *TwistingSlide;

void TwistingSlide_Update(void)
{
    RSDK_THIS(TwistingSlide);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        int32 storeX = player->position.x;
        int32 storeY = player->position.y;

        if (player->state != Player_State_Static)
            self->activePlayers &= ~(1 << playerID);

        if (!Player_CheckCollisionTouch(player, self, &self->hitboxSlide) || (player->position.y < self->minY) || player->position.y > self->maxY) {
            self->activePlayers &= ~(1 << playerID);
        }
        else if (self->direction) {
            switch (self->type) {
                case TWISTINGSLIDE_START:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }

                        int32 angle = 221 * self->playerAngles[playerID];
                        if (221 * self->playerAngles[playerID] >= 0x3FC0)
                            angle = 170 * self->playerAngles[playerID];

                        player->position.x = 0x2800 * RSDK.Cos256((angle >> 8) + 0xC0) + self->position.x;
                        player->position.y = self->position.y + ((self->playerAngles[playerID] - 74) << 16);

                        if (player->groundVel > 0 && Player_CheckCollisionTouch(player, self, &self->hitboxSlideStart)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_ROOF;
                            player->tileCollisions = TILECOLLISION_DOWN;
                            player->angle          = 144;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel < 0) {
                        if (Player_CheckCollisionTouch(player, self, &self->hitboxSlideStart)) {
                            player->tileCollisions = TILECOLLISION_NONE;
                            player->state          = Player_State_Static;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            self->playerAngles[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= (1 << playerID);
                        }
                    }
                    break;

                case TWISTINGSLIDE_STRIP:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = 0x800000 + self->position.x - 0xAA00 * self->playerAngles[playerID];
                        player->position.y = self->position.y + ((self->playerAngles[playerID] - 0xC0) << 16);
                    }
                    break;

                case TWISTINGSLIDE_TWIST:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = self->position.x + 0x2800 * RSDK.Cos256(((170 * self->playerAngles[playerID]) >> 8) + 0x40);
                        player->position.y = self->position.y + ((self->playerAngles[playerID] - 0xC0) << 16);
                    }
                    break;

                case TWISTINGSLIDE_END:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                        player->position.x = self->position.x + ((self->endLen / 3) << 16) - 0xAA00 * self->playerAngles[playerID];
                        player->position.y = self->position.y + (self->playerAngles[playerID] << 16) - (self->endLen << 15);
                        if (player->groundVel < 0 && Player_CheckCollisionTouch(player, self, &self->hitboxSlideEnd)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_RWALL;
                            player->tileCollisions = TILECOLLISION_DOWN;
                            player->angle          = 216;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel > 0
                             && Player_CheckCollisionTouch(player, self, &self->hitboxSlideEnd)) {
                        player->tileCollisions = TILECOLLISION_NONE;
                        player->state          = Player_State_Static;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0;
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        self->playerAngles[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                        self->activePlayers |= (1 << playerID);
                    }
                    break;

                default: break;
            }
        }
        else {
            switch (self->type) {
                case TWISTINGSLIDE_START:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }

                        int32 angle = 221 * self->playerAngles[playerID];
                        if (221 * self->playerAngles[playerID] >= 0x3FC0)
                            angle = 170 * self->playerAngles[playerID];

                        player->position.x = self->position.x - 0x2800 * RSDK.Cos256((angle >> 8) + 0xC0);
                        player->position.y = self->position.y + ((self->playerAngles[playerID] - 74) << 16);
                        if (player->groundVel < 0 && Player_CheckCollisionTouch(player, self, &self->hitboxSlideStart)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_ROOF;
                            player->tileCollisions = TILECOLLISION_DOWN;
                            player->angle          = 0x70;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel > 0) {
                        if (Player_CheckCollisionTouch(player, self, &self->hitboxSlideStart)) {
                            player->tileCollisions = TILECOLLISION_NONE;
                            player->state          = Player_State_Static;
                            player->velocity.x     = 0;
                            player->velocity.y     = 0;
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                            self->playerAngles[playerID] = (player->position.y - self->position.y + 0x4A0000) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }
                    }
                    break;

                case TWISTINGSLIDE_STRIP:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }

                        player->position.x = 0xAA00 * self->playerAngles[playerID] + self->position.x - 0x800000;
                        player->position.y = self->position.y + ((self->playerAngles[playerID] - 0xC0) << 16);
                    }
                    break;

                case TWISTINGSLIDE_TWIST:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (0xC00000 + player->position.y - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }

                        player->position.x = self->position.x - 0x2800 * RSDK.Cos256(((170 * self->playerAngles[playerID]) >> 8) + 0x40);
                        player->position.y = self->position.y + ((self->playerAngles[playerID] - 0xC0) << 16);
                    }
                    break;

                case TWISTINGSLIDE_END:
                    if (player->state == Player_State_Static) {
                        if (!((1 << playerID) & self->activePlayers)) {
                            self->playerAngles[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                            self->activePlayers |= 1 << playerID;
                        }

                        player->position.x = self->position.x + 0xAA00 * self->playerAngles[playerID] - ((self->endLen / 3) << 16);
                        player->position.y = self->position.y + (self->playerAngles[playerID] << 16) - (self->endLen << 15);

                        if (player->groundVel > 0 && Player_CheckCollisionTouch(player, self, &self->hitboxSlideEnd)) {
                            self->activePlayers &= ~(1 << playerID);
                            player->state          = Player_State_Roll;
                            player->collisionMode  = CMODE_RWALL;
                            player->tileCollisions = TILECOLLISION_DOWN;
                            player->angle          = 40;
                        }
                    }
                    else if (!((1 << playerID) & self->activePlayers) && player->onGround && player->groundVel < 0
                             && Player_CheckCollisionTouch(player, self, &self->hitboxSlideEnd)) {
                        player->tileCollisions = TILECOLLISION_NONE;
                        player->state          = Player_State_Static;
                        player->velocity.x     = 0;
                        player->velocity.y     = 0;

                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                        self->playerAngles[playerID] = (player->position.y + (self->endLen << 15) - self->position.y) >> 16;
                        self->activePlayers |= 1 << playerID;
                    }
                    break;

                default: break;
            }
        }

        if (((1 << playerID) & self->activePlayers)) {
            if (self->direction) {
                self->playerAngles[playerID] += ((-player->groundVel >> 16) * RSDK.Sin256(40)) >> 8;

                if (player->groundVel <= 0)
                    player->groundVel -= 20 * RSDK.Cos256(40);
                else
                    player->groundVel -= 80 * RSDK.Cos256(40);

                if (player->groundVel < -0x180000)
                    player->groundVel = -0x180000;
            }
            else {
                self->playerAngles[playerID] += ((player->groundVel >> 16) * RSDK.Sin256(40)) >> 8;

                if (player->groundVel <= 0)
                    player->groundVel += 20 * RSDK.Cos256(40);
                else
                    player->groundVel += 80 * RSDK.Cos256(40);

                if (player->groundVel > 0x180000)
                    player->groundVel = 0x180000;
            }

            if (player->characterID == ID_TAILS)
                player->animator.speed = 120;
            else
                player->animator.speed = ((abs(player->groundVel) * 0xF0) / 0x60000) + 0x30;

            if (player->animator.speed > 0xF0)
                player->animator.speed = 0xF0;

            int32 x = player->position.x - storeX;
            int32 y = player->position.y - storeY;
            if (player->position.x != storeX && y) {
                if (player->groundVel <= 0) {
                    x = storeX - player->position.x;
                    y = storeY - player->position.y;
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
            case TWISTINGSLIDE_START:
                self->updateRange.y = 0x780000;
                self->updateRange.x = 0x800000;
                self->minY          = self->position.y - 0x4A0000;
                self->maxY          = self->position.y + 0x780000;
                break;

            case TWISTINGSLIDE_STRIP:
                self->updateRange.x = 0xC00000;
                self->updateRange.y = 0xC00000;
                self->minY          = self->position.y - 0xC00000;
                self->maxY          = self->position.y + 0xC00000;
                break;

            case TWISTINGSLIDE_TWIST:
                self->updateRange.y = 0xC00000;
                self->updateRange.x = 0x800000;
                self->minY          = self->position.y - 0xC00000;
                self->maxY          = self->position.y + 0xC00000;
                break;

            case TWISTINGSLIDE_END:
                self->minY          = self->position.y - (self->endLen << 15);
                self->maxY          = self->position.y + (self->endLen << 15);
                self->updateRange.y = 0xC00000;
                self->updateRange.x = 0x800000;
                break;

            default: break;
        }

        TwistingSlide_SetupHitboxes();
    }
}

void TwistingSlide_StageLoad(void)
{
    TwistingSlide->hitbox.left   = -1;
    TwistingSlide->hitbox.top    = 1;
    TwistingSlide->hitbox.right  = 1;
    TwistingSlide->hitbox.bottom = 1;
}

void TwistingSlide_SetupHitboxes(void)
{
    RSDK_THIS(TwistingSlide);

    switch (self->type) {
        case TWISTINGSLIDE_START:
            self->hitboxSlide.left   = -56;
            self->hitboxSlide.top    = -120;
            self->hitboxSlide.right  = 52;
            self->hitboxSlide.bottom = 120;

            self->hitboxSlideStart.top    = -46;
            self->hitboxSlideStart.bottom = -38;
            self->hitboxSlideStart.left   = ((-40 * RSDK.Cos256(48)) >> 8) - 4;
            self->hitboxSlideStart.right  = ((-40 * RSDK.Cos256(48)) >> 8) + 4;
            break;

        case TWISTINGSLIDE_STRIP:
        case TWISTINGSLIDE_TWIST:
            self->hitboxSlide.left   = -132;
            self->hitboxSlide.top    = -196;
            self->hitboxSlide.right  = 132;
            self->hitboxSlide.bottom = 196;
            break;

        case TWISTINGSLIDE_END:
            self->hitboxSlide.left   = self->endLen / -3;
            self->hitboxSlide.top    = -(self->endLen >> 1);
            self->hitboxSlide.right  = self->endLen / 3;
            self->hitboxSlide.bottom = self->endLen >> 1;

            self->hitboxSlideEnd.left   = self->hitboxSlide.right - 4;
            self->hitboxSlideEnd.top    = self->hitboxSlide.bottom - 4;
            self->hitboxSlideEnd.right  = self->hitboxSlide.right + 4;
            self->hitboxSlideEnd.bottom = self->hitboxSlide.bottom + 4;
            break;

        default: break;
    }
}

#if GAME_INCLUDE_EDITOR
void TwistingSlide_EditorDraw(void)
{
    RSDK_THIS(TwistingSlide);

    TwistingSlide_SetupHitboxes();

    if (showGizmos()) {
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxSlideStart, self->direction, 0xFF0000);
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxSlide, self->direction, 0xFF0000);
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxSlideEnd, self->direction, 0xFF0000);
    }
}

void TwistingSlide_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(TwistingSlide, type);
    RSDK_ENUM_VAR("Start", TWISTINGSLIDE_START);
    RSDK_ENUM_VAR("Long Strip", TWISTINGSLIDE_STRIP);
    RSDK_ENUM_VAR("Twist", TWISTINGSLIDE_TWIST);
    RSDK_ENUM_VAR("End", TWISTINGSLIDE_END);

    RSDK_ACTIVE_VAR(TwistingSlide, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void TwistingSlide_Serialize(void)
{
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_ENUM, endLen);
    RSDK_EDITABLE_VAR(TwistingSlide, VAR_UINT8, direction);
}
