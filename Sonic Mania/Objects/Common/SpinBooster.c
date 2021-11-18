#include "SonicMania.h"

ObjectSpinBooster *SpinBooster;

void SpinBooster_Update(void)
{
    RSDK_THIS(SpinBooster);

    int32 negAngle = -self->angle;

    foreach_active(Player, player)
    {
        int32 pid   = RSDK.GetEntityID(player);
        int32 distX = (player->position.x - self->position.x) >> 8;
        int32 distY = (player->position.y - self->position.y) >> 8;
        int32 x     = (distY * RSDK.Sin256(negAngle)) + distX * RSDK.Cos256(negAngle) + self->position.x;
        int32 y     = (distY * RSDK.Cos256(negAngle)) - distX * RSDK.Sin256(negAngle) + self->position.y;

        if (abs(x - self->position.x) >= 0x180000 || abs(y - self->position.y) >= self->size << 19) {
            if (x < self->position.x) {
                self->activePlayers &= ~(1 << pid);
            }
            else {
                self->activePlayers |= (1 << pid);
            }
        }
        else {
            if (x < self->position.x) {
                if (((1 << pid) & self->activePlayers) && !self->forwardOnly) {
                    if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        if (!self->allowTubeInput)
                            player->controlLock = 0;
                        player->tileCollisions = true;
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                    }
                }
                self->activePlayers &= ~(1 << pid);
            }
            else if (!((1 << pid) & self->activePlayers)) {
                SpinBooster_HandleForceRoll(player);
                self->activePlayers |= (1 << pid);
            }
        }
    }

    self->visible = DebugMode->debugActive;
}

void SpinBooster_LateUpdate(void) {}

void SpinBooster_StaticUpdate(void) {}

void SpinBooster_Draw(void) { SpinBooster_DrawSprites(); }

void SpinBooster_Create(void *data)
{
    RSDK_THIS(SpinBooster);
    RSDK.SetSpriteAnimation(SpinBooster->aniFrames, 0, &self->animator, true, 0);
    self->drawFX |= FX_FLIP;
    self->animator.frameID = 4;
    self->activePlayers    = 0;
    if (SceneInfo->inEditor) {
        if (!self->boostPower)
            self->boostPower = 15;
    }
    else {
        switch (self->direction) {
            case FLIP_NONE: self->angle = 0; break;
            case FLIP_X: self->angle = 0x40; break;
            case FLIP_Y: self->angle = 0x80; break;
            case FLIP_XY: self->angle = 0xC0; break;
        }
        self->active = ACTIVE_BOUNDS;

        self->updateRange.x = self->size * abs(RSDK.Sin256(self->angle)) << 11;
        self->updateRange.x += 0x200000;

        self->updateRange.y = self->size * abs(RSDK.Cos256(self->angle)) << 11;
        self->updateRange.y += 0x200000;
        self->visible   = false;
        self->drawOrder = Zone->drawOrderLow;
    }
}

void SpinBooster_StageLoad(void) { SpinBooster->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

int32 SpinBooster_GetRollDir(EntityPlayer *player)
{
    RSDK_THIS(SpinBooster);

    switch (self->direction) {
        case FLIP_NONE:
            if (self->autoGrip != 5) {
                if ((self->autoGrip == 7 || self->autoGrip == 8) && player->up)
                    return CMODE_ROOF;
                return CMODE_FLOOR;
            }
            if (player->down) {
                return CMODE_FLOOR;
            }
            if (player->up) {
                return CMODE_ROOF;
            }

            if (!self->bias)
                return CMODE_FLOOR;
            else
                return CMODE_ROOF;
        case FLIP_X:
            if (self->autoGrip != 5) {
                if (self->autoGrip == 7) {
                    if (!player->left) {
                        return -1;
                    }
                    return CMODE_RWALL;
                }

                if (self->autoGrip == 6) {
                    if (!player->right)
                        return -1;
                    return CMODE_LWALL;
                }
                if (self->autoGrip != 8)
                    return -1;
                if (!player->left) {
                    if (!player->right)
                        return -1;
                    return CMODE_LWALL;
                }
                else {
                    return CMODE_RWALL;
                }
            }
            else {
                if (player->left)
                    return CMODE_RWALL;
                if (player->right)
                    return CMODE_LWALL;
                return CMODE_ROOF * (self->bias != 0) + 1;
            }
        case FLIP_Y:
            if (self->autoGrip != 5) {
                if ((self->autoGrip == 6 || self->autoGrip == 8) && player->up)
                    return CMODE_ROOF;
                return CMODE_FLOOR;
            }
            if (player->down)
                return CMODE_FLOOR;
            if (player->up)
                return CMODE_ROOF;

            if (!self->bias)
                return CMODE_ROOF;
            else
                return CMODE_FLOOR;
        case FLIP_XY:
            if (self->autoGrip == 5) {
                return 2 * (player->left != 0) + 1;
            }
            else if (self->autoGrip == 6) {
                if (!player->left)
                    return -1;
                return CMODE_RWALL;
            }
            else {
                if (self->autoGrip == 7) {
                    if (!player->right)
                        return -1;
                    return CMODE_LWALL;
                }
                if (self->autoGrip != 8)
                    return -1;
                if (!player->left) {
                    if (!player->right)
                        return -1;
                    return CMODE_LWALL;
                }
                else {
                    return CMODE_RWALL;
                }
            }
        default: break;
    }
    return -1;
}
void SpinBooster_HandleRollDir(EntityPlayer *player)
{
    RSDK_THIS(SpinBooster);

    if (self->autoGrip) {
        int8 cMode = 0;
        if (self->autoGrip < 5) {
            cMode = self->autoGrip - 1;
        }
        else {
            cMode = SpinBooster_GetRollDir(player);
        }

        if (cMode < 0) {
            player->onGround = false;
            return;
        }
        int32 offsetX  = 0;
        int32 offsetY  = 0;
        int32 plrAngle = 0;
        int32 angle    = 0;

        switch (cMode) {
            case CMODE_FLOOR:
                plrAngle = 0x00;
                angle    = 0x40;
                break;
            case CMODE_LWALL:
                plrAngle = -0x40;
                angle    = 0x00;
                break;
            case CMODE_ROOF:
                plrAngle = -0x80;
                angle    = -0x40;
                break;
            case CMODE_RWALL:
                plrAngle = 0x40;
                angle    = -0x80;
                break;
            default:
                plrAngle = 0x00;
                angle    = 0x00;
                break;
        }

        Hitbox *playerHitbox = Player_GetHitbox(player);
        switch (cMode) {
            case CMODE_FLOOR: offsetY = playerHitbox->bottom << 16; break;
            case CMODE_LWALL: offsetX = playerHitbox->right << 16; break;
            case CMODE_ROOF: offsetY = playerHitbox->top << 16; break;
            case CMODE_RWALL: offsetX = playerHitbox->left << 16; break;
            default: break;
        }
        player->position.x += RSDK.Cos256(angle) << 8;
        player->position.y += RSDK.Sin256(angle) << 8;
        RSDK.ObjectTileGrip(player, Zone->fgLayers, cMode, 0, offsetX, offsetY, 1);
        player->angle = plrAngle;
        if (player->onGround) {
            uint8 mode = 0;
            switch (player->collisionMode) {
                case CMODE_FLOOR: mode = CMODE_ROOF; break;
                case CMODE_LWALL: mode = CMODE_RWALL; break;
                case CMODE_ROOF: mode = CMODE_FLOOR; break;
                case CMODE_RWALL: mode = CMODE_LWALL; break;
            }

            if (cMode == mode) {
                player->groundVel = -player->groundVel;
            }
        }
        else {
            switch (cMode) {
                case CMODE_FLOOR: player->groundVel = player->velocity.x; break;
                case CMODE_LWALL: player->groundVel = -player->velocity.y; break;
                case CMODE_ROOF: player->groundVel = -player->velocity.x; break;
                case CMODE_RWALL: player->groundVel = player->velocity.y; break;
                default: break;
            }
        }
        player->collisionMode = cMode;
        player->onGround      = true;
    }
}
void SpinBooster_ApplyRollVelocity(EntityPlayer *player)
{
    RSDK_THIS(SpinBooster);
    if (player->onGround) {
        int32 entAng = RSDK.Sin256(self->angle) + RSDK.Cos256(self->angle);
        int32 plrAng = RSDK.Cos256(player->angle) - RSDK.Sin256(player->angle);
        int32 power  = (self->boostPower << 15) * ((plrAng > 0) - (plrAng < 0)) * ((entAng > 0) - (entAng < 0));
        if (self->boostPower >= 0)
            player->groundVel += power;
        else
            player->groundVel = power;
    }
    else {
        int32 x = ( 0x80 * RSDK.Cos256(self->angle)) * self->boostPower;
        int32 y = (-0x80 * RSDK.Sin256(self->angle)) * self->boostPower;
        if (self->boostPower >= 0) {
            player->velocity.x += x;
            player->velocity.y += y;
        }
        else {
            player->velocity.x = x;
            player->velocity.y = y;
        }
    }

    if (self->boostPower < 0 && !self->forwardOnly) {
        if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            if (!self->allowTubeInput)
                player->controlLock = 0;
            player->tileCollisions = true;
            if (player->onGround)
                player->state = Player_State_Roll;
            else
                player->state = Player_State_Air;
        }
    }
}
void SpinBooster_DrawArrow(uint32 colour, int32 X1, int32 Y1, int32 X2, int32 Y2)
{
    RSDK.DrawLine(X1, Y1, X2, Y2, colour, 127, INK_ADD, false);
    int32 angle = RSDK.ATan2(X1 - X2, Y1 - Y2);
    int32 c1    = RSDK.Cos256(angle + 12) << 12;
    int32 s1    = RSDK.Sin256(angle + 12) << 12;
    int32 c2    = RSDK.Cos256(angle - 12) << 12;
    int32 s2    = RSDK.Sin256(angle - 12) << 12;
    RSDK.DrawLine(X2, Y2, X2 + c1, Y2 + s1, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(X2, Y2, X2 + c2, Y2 + s2, colour, 0x7F, INK_ADD, false);
}
void SpinBooster_DrawSprites(void)
{
    RSDK_THIS(SpinBooster);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.y -= self->size << 19;
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    if (SceneInfo->inEditor) {
        uint8 negAngle = -self->angle;
        int32 power     = self->boostPower;
        int32 x        = self->position.x;
        int32 y        = self->position.y;
        int32 clr       = ((power >> 31) & 0xFE0001) + 0xFFFF;
        if (!power)
            power = 1;
        SpinBooster_DrawArrow(clr, x, y, power * (RSDK.Cos256(negAngle) << 11) + self->position.x,
                              power * (RSDK.Sin256(negAngle) << 11) + self->position.y);

        switch (self->autoGrip) {
            default: break;
            case 1:
            case 2:
            case 3:
            case 4: {
                int32 angle = 0;
                switch (self->autoGrip) {
                    case 1: angle = 0x40; break;
                    case 2: angle = 0; break;
                    case 3: angle = -0x40; break;
                    case 4: angle = 0x80; break;
                }
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(angle) << 14) + self->position.x,
                                     (RSDK.Sin256(angle) << 14) + self->position.y);
                break;
            }
            case 5:
                if (!self->bias)
                    clr = 0x00FF00;
                else
                    clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle + 64)) << 14) + self->position.x,
                                     (RSDK.Sin256((uint8)(negAngle + 64)) << 14) + self->position.y);
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle - 64)) << 14) + self->position.x,
                                     (RSDK.Sin256((uint8)(negAngle - 64)) << 14) + self->position.y);
                break;
            case 6:
                clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle + 64)) << 14) + self->position.x,
                                     (RSDK.Sin256((uint8)(negAngle + 64)) << 14) + self->position.y);
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(negAngle) << 14) + self->position.x,
                                     (RSDK.Sin256(negAngle) << 14) + self->position.y);
                break;
            case 0:
            case 7:
                clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle - 64)) << 14) + self->position.x,
                                     (RSDK.Sin256((uint8)(negAngle - 64)) << 14) + self->position.y);
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(negAngle) << 14) + self->position.x,
                                     (RSDK.Sin256(negAngle) << 14) + self->position.y);
                break;
            case 8:
                clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle + 64)) << 14) + self->position.x,
                                     (RSDK.Sin256((uint8)(negAngle + 64)) << 14) + self->position.y);
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle - 64)) << 14) + self->position.x,
                                     (RSDK.Sin256((uint8)(negAngle - 64)) << 14) + self->position.y);
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(negAngle) << 14) + self->position.x,
                                     (RSDK.Sin256(negAngle) << 14) + self->position.y);
                break;
        }
    }
}
void SpinBooster_HandleForceRoll(EntityPlayer *player)
{
    RSDK_THIS(SpinBooster);
    player->tileCollisions = true;
    SpinBooster_HandleRollDir(player);

    if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
        if (self->boostAlways)
            SpinBooster_ApplyRollVelocity(player);
    }
    else {
        if (self->playSound)
            RSDK.PlaySfx(Player->sfxRoll, 0, 0xFF);
        if (player->animator.animationID != ANI_JUMP) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            if (!player->collisionMode && player->onGround)
                player->position.y += player->cameraOffset;
            player->pushing = false;
        }
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        if (!self->allowTubeInput)
            player->controlLock = 0xFFFF;

        if (player->onGround)
            player->state = Player_State_ForceRoll_Ground;
        else
            player->state = Player_State_ForceRoll_Air;

        if (abs(player->groundVel) < 0x10000) {
            if (self->direction & FLIP_X)
                player->groundVel = -0x40000;
            else
                player->groundVel = 0x40000;
        }
        SpinBooster_ApplyRollVelocity(player);
    }
}

void SpinBooster_EditorDraw(void) { SpinBooster_DrawSprites(); }

void SpinBooster_EditorLoad(void) { SpinBooster->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void SpinBooster_Serialize(void)
{
    RSDK_EDITABLE_VAR(SpinBooster, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_UINT8, autoGrip);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_UINT8, bias);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_INT32, boostPower);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_BOOL, boostAlways);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_BOOL, forwardOnly);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_BOOL, playSound);
    RSDK_EDITABLE_VAR(SpinBooster, VAR_BOOL, allowTubeInput);
}
