// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZSpiral Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup = Zone->objectDrawGroup[0];

        switch (self->type) {
            default: break;
            case LRZSPIRAL_CYLINDER: {
                int32 height = self->height << 25 >> 19;

                self->state = LRZSpiral_State_Cylinder;

                self->updateRange.x = 0x800000;
                self->updateRange.y = (self->height + 1) << 22;

                self->hitboxTrigger.left   = -16;
                self->hitboxTrigger.right  = 16;
                self->hitboxTrigger.top    = -height;
                self->hitboxTrigger.bottom = 128 - height;

                self->height = (self->height << 25) - 0x1000000;
                break;
            }

            case LRZSPIRAL_J_CURVE:
                self->direction     = FLIP_NONE;
                self->updateRange.x = 0x800000;
                self->updateRange.y = (self->height + 64) << 16;

                self->hitboxTrigger.left   = -112;
                self->hitboxTrigger.top    = -180;
                self->hitboxTrigger.right  = -104;
                self->hitboxTrigger.bottom = -172;

                self->state = LRZSpiral_State_J_Curve;
                break;

            case LRZSPIRAL_C_CURVE:
                self->direction     = FLIP_X;
                self->updateRange.x = 0x800000;
                self->updateRange.y = (self->height + 64) << 16;

                self->hitboxTrigger.left   = -146;
                self->hitboxTrigger.top    = -180;
                self->hitboxTrigger.right  = -138;
                self->hitboxTrigger.bottom = -172;

                self->state = LRZSpiral_State_C_Curve;
                break;
        }
    }
}

void LRZSpiral_StageLoad(void) {}

void LRZSpiral_HandlePlayerExit(EntityPlayer *player)
{
    if (abs(player->groundVel) < player->minDashVelocity) {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 1);

        player->animator.speed = (abs(player->groundVel) >> 12) + 96;
        if (player->animator.speed > 0x200)
            player->animator.speed = 0x200;

        player->minRunVelocity  = 0x58000;
        player->minDashVelocity = 0xC0000;
    }
    else {
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_DASH, &player->animator, false, 1);
        player->minDashVelocity = 0xB8000;
    }
}

void LRZSpiral_State_Cylinder(void)
{
    RSDK_THIS(LRZSpiral);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->activePlayers) {
            if (player->state == Player_State_Static) {
                self->playerSpiralPos[playerID] += player->groundVel;

                if (self->playerSpiralPos[playerID] < 0) {
                    self->activePlayers &= ~(1 << playerID);
                }
                else if (self->playerSpiralPos[playerID] < self->height) {
                    if (player->groundVel < 0x100000)
                        player->groundVel += 0x1000;

                    uint8 spiralPos = self->playerSpiralPos[playerID] >> 17;
                    if (spiralPos < 0x40 || spiralPos >= 0xC0)
                        player->drawGroup = Zone->playerDrawGroup[1];
                    else
                        player->drawGroup = Zone->playerDrawGroup[0];

                    player->position.x = 0x4800 * RSDK.Sin256(self->playerSpiralPos[playerID] >> 17) + self->position.x;
                    player->position.y = self->playerSpiralPos[playerID] >> 2;
                    int32 pos          = (player->position.y >> 16) & 0x7F;
                    player->position.y = player->position.y & 0xFF800000;
                    player->position.y += 0x580000 + self->position.y - ((self->height + 0x1000000) >> 3);

                    if (self->playerSpiralPos[playerID] >= self->height - 0x1000000)
                        player->position.y += LRZSpiral->cylinderExitYOffsetTable[pos];
                    else
                        player->position.y += LRZSpiral->cylinderSpiralYOffsetTable[pos];

                    player->animator.frameID = (pos << 8) / 0x555;
                    player->direction        = (player->animator.frameID + 6) % 24 > 12;
                }
                else {
                    player->tileCollisions = TILECOLLISION_DOWN;
                    player->collisionPlane = 1;
                    player->controlLock    = 16;
                    player->direction      = FLIP_X;
                    player->onGround       = true;
                    player->groundedStore  = true;
                    player->state          = Player_State_Ground;
                    player->groundVel      = -player->groundVel;

                    self->activePlayers &= ~(1 << playerID);
                    LRZSpiral_HandlePlayerExit(player);
                    if (!self->activePlayers)
                        self->active = ACTIVE_BOUNDS;
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
        else if (player->position.x >= self->position.x) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;

                if (player->groundVel < 0x60000)
                    player->groundVel = 0x60000;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPIRAL_RUN, &player->animator, false, 0);
                player->animator.speed  = 0;
                player->state           = Player_State_Static;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->tileCollisions  = TILECOLLISION_NONE;
                self->activePlayers |= 1 << playerID;
                self->playerSpiralPos[playerID] = 0;
                self->active                    = ACTIVE_NORMAL;
            }
        }
    }
}
void LRZSpiral_State_J_Curve(void)
{
    RSDK_THIS(LRZSpiral);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);
        if ((1 << playerID) & self->activePlayers) {
            if (player->state == Player_State_Static) {
                self->playerSpiralPos[playerID] += player->groundVel;

                if (self->playerSpiralPos[playerID] < 0) {
                    self->activePlayers &= ~(1 << playerID);
                }
                else {
                    self->playerSpiralPos[playerID] += player->groundVel >> 17;

                    if (player->groundVel < 0x100000)
                        player->groundVel += 0x1000;

                    player->position.x = 0xA000 * RSDK.Sin256(self->playerSpiralPos[playerID] >> 18);
                    player->position.x += self->position.x - 0x6C0000;
                    player->position.y = self->position.y - 0xB00000 + 0x15800 * ((256 - RSDK.Cos256(self->playerSpiralPos[playerID] >> 18)) >> 1);

                    if (self->playerSpiralPos[playerID] < 0 || self->playerSpiralPos[playerID] >= 0x2000000) {
                        player->tileCollisions = TILECOLLISION_DOWN;
                        player->collisionPlane = 1;
                        player->controlLock    = 16;
                        player->direction      = FLIP_X;
                        player->onGround       = true;
                        player->groundedStore  = true;
                        player->state          = Player_State_Ground;
                        player->groundVel      = -player->groundVel;

                        self->activePlayers &= ~(1 << playerID);
                        LRZSpiral_HandlePlayerExit(player);
                        if (!self->activePlayers)
                            self->active = ACTIVE_BOUNDS;
                    }
                    else {
                        player->animator.frameID = self->playerSpiralPos[playerID] / 0x2AAAAA;
                    }
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
        else if (player->position.x >= self->position.x - 0x6C0000 && player->groundVel >= 0 && player->onGround) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;

                if (player->groundVel < 0x40000)
                    player->groundVel = 0x40000;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWIST_RUN, &player->animator, false, 0);

                player->animator.speed  = 0;
                player->state           = Player_State_Static;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->tileCollisions  = TILECOLLISION_NONE;

                self->activePlayers |= 1 << playerID;
                self->playerSpiralPos[playerID] = 0;
                self->active                    = ACTIVE_NORMAL;
            }
        }
    }
}
void LRZSpiral_State_C_Curve(void)
{
    RSDK_THIS(LRZSpiral);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->activePlayers) {
            if (player->state == Player_State_Static) {
                self->playerSpiralPos[playerID] -= player->groundVel;

                if (self->playerSpiralPos[playerID] < 0) {
                    self->activePlayers &= ~(1 << playerID);
                }
                else {
                    self->playerSpiralPos[playerID] += player->groundVel >> 17;
                    if (player->groundVel > -0x100000)
                        player->groundVel -= 0x1000;

                    player->position.x = -0xA000 * RSDK.Sin256(self->playerSpiralPos[playerID] >> 18);
                    player->position.x += 0x8E0000 + self->position.x;
                    player->position.y = self->position.y - 0xB40000 + 0x14200 * ((0x100 - RSDK.Cos256(self->playerSpiralPos[playerID] >> 18)) >> 1);

                    if (self->playerSpiralPos[playerID] < 0 || self->playerSpiralPos[playerID] >= 0x2000000) {
                        player->tileCollisions = TILECOLLISION_DOWN;
                        player->collisionPlane = 1;
                        player->controlLock    = 16;
                        player->direction      = FLIP_NONE;
                        player->onGround       = true;
                        player->groundedStore  = true;
                        player->state          = Player_State_Ground;
                        player->groundVel      = -player->groundVel;
                        self->activePlayers &= ~(1 << playerID);
                        LRZSpiral_HandlePlayerExit(player);
                        if (!self->activePlayers)
                            self->active = ACTIVE_BOUNDS;
                    }
                    else {
                        player->animator.frameID = self->playerSpiralPos[playerID] / 0x2AAAAA;
                    }
                }
            }
            else {
                self->activePlayers &= ~(1 << playerID);
            }
        }
        else if (player->position.x <= 0x8E0000 + self->position.x && player->groundVel <= 0 && player->onGround) {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxTrigger)) {
                player->onGround   = false;
                player->velocity.x = 0;
                player->velocity.y = 0;

                if (player->groundVel > -0x40000)
                    player->groundVel = -0x40000;

                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWIST_RUN, &player->animator, false, 0);
                player->animator.speed  = 0;
                player->state           = Player_State_Static;
                player->nextAirState    = StateMachine_None;
                player->nextGroundState = StateMachine_None;
                player->tileCollisions  = TILECOLLISION_NONE;

                self->activePlayers |= 1 << playerID;
                self->playerSpiralPos[playerID] = 0;
                self->active                    = ACTIVE_NORMAL;
            }
        }
    }
}

#if GAME_INCLUDE_EDITOR
void LRZSpiral_EditorDraw(void)
{
    RSDK_THIS(LRZSpiral);

    int32 height = self->height + 64;
    switch (self->type) {
        default: break;
        case LRZSPIRAL_CYLINDER: {
            height              = self->height << 25 >> 19;
            self->updateRange.x = 0x800000;
            self->updateRange.y = (self->height + 1) << 22;

            self->hitboxTrigger.left   = -16;
            self->hitboxTrigger.right  = 16;
            self->hitboxTrigger.top    = -height;
            self->hitboxTrigger.bottom = 128 - height;

            height <<= 1;
            break;
        }

        case LRZSPIRAL_J_CURVE:
            self->direction     = FLIP_NONE;
            self->updateRange.x = 0x800000;
            self->updateRange.y = height << 16;

            self->hitboxTrigger.left   = -112;
            self->hitboxTrigger.top    = -180;
            self->hitboxTrigger.right  = -104;
            self->hitboxTrigger.bottom = -172;
            break;

        case LRZSPIRAL_C_CURVE:
            self->direction     = FLIP_X;
            self->updateRange.x = 0x800000;
            self->updateRange.y = height << 16;

            self->hitboxTrigger.left   = -146;
            self->hitboxTrigger.top    = -180;
            self->hitboxTrigger.right  = -138;
            self->hitboxTrigger.bottom = -172;
            break;
    }

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitboxTrigger, self->direction, 0xFF0000);
        RSDK_DRAWING_OVERLAY(false);
    }

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->radius << 17, height << 16, 0xFFFF00);
}

void LRZSpiral_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(LRZSpiral, type);
    RSDK_ENUM_VAR("Cylinder", LRZSPIRAL_CYLINDER);
    RSDK_ENUM_VAR("\"J\" Shaped Curve", LRZSPIRAL_J_CURVE);
    RSDK_ENUM_VAR("\"C\" Shaped Curve", LRZSPIRAL_C_CURVE);
}
#endif

void LRZSpiral_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_ENUM, height);
    RSDK_EDITABLE_VAR(LRZSpiral, VAR_ENUM, radius);
}
