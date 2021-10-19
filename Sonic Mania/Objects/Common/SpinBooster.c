#include "SonicMania.h"

ObjectSpinBooster *SpinBooster;

void SpinBooster_Update(void)
{
    RSDK_THIS(SpinBooster);

    int32 negAngle = -entity->angle;

    foreach_active(Player, player)
    {
        int32 pid   = RSDK.GetEntityID(player);
        int32 distX = (player->position.x - entity->position.x) >> 8;
        int32 distY = (player->position.y - entity->position.y) >> 8;
        int32 x     = (distY * RSDK.Sin256(negAngle)) + distX * RSDK.Cos256(negAngle) + entity->position.x;
        int32 y     = (distY * RSDK.Cos256(negAngle)) - distX * RSDK.Sin256(negAngle) + entity->position.y;

        if (abs(x - entity->position.x) >= 0x180000 || abs(y - entity->position.y) >= entity->size << 19) {
            if (x < entity->position.x) {
                entity->activePlayers &= ~(1 << pid);
            }
            else {
                entity->activePlayers |= (1 << pid);
            }
        }
        else {
            if (x < entity->position.x) {
                if (((1 << pid) & entity->activePlayers) && !entity->forwardOnly) {
                    if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
                        player->nextAirState    = StateMachine_None;
                        player->nextGroundState = StateMachine_None;
                        if (!entity->allowTubeInput)
                            player->controlLock = 0;
                        player->tileCollisions = true;
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                    }
                }
                entity->activePlayers &= ~(1 << pid);
            }
            else if (!((1 << pid) & entity->activePlayers)) {
                SpinBooster_HandleForceRoll(player);
                entity->activePlayers |= (1 << pid);
            }
        }
    }

    entity->visible = DebugMode->debugActive;
}

void SpinBooster_LateUpdate(void) {}

void SpinBooster_StaticUpdate(void) {}

void SpinBooster_Draw(void) { SpinBooster_DrawSprites(); }

void SpinBooster_Create(void *data)
{
    RSDK_THIS(SpinBooster);
    RSDK.SetSpriteAnimation(SpinBooster->aniFrames, 0, &entity->animator, true, 0);
    entity->drawFX |= FX_FLIP;
    entity->animator.frameID = 4;
    entity->activePlayers    = 0;
    if (RSDK_sceneInfo->inEditor) {
        if (!entity->boostPower)
            entity->boostPower = 15;
    }
    else {
        switch (entity->direction) {
            case FLIP_NONE: entity->angle = 0; break;
            case FLIP_X: entity->angle = 0x40; break;
            case FLIP_Y: entity->angle = 0x80; break;
            case FLIP_XY: entity->angle = 0xC0; break;
        }
        entity->active = ACTIVE_BOUNDS;

        if (entity->size * RSDK.Sin256(entity->angle) << 11 >= 0)
            entity->updateRange.x = entity->size * RSDK.Sin256(entity->angle) << 11;
        else
            entity->updateRange.x = -0x800 * entity->size * RSDK.Sin256(entity->angle);
        entity->updateRange.x += 0x200000;

        if (entity->size * RSDK.Cos256(entity->angle) << 11 >= 0)
            entity->updateRange.y = entity->size * RSDK.Cos256(entity->angle) << 11;
        else
            entity->updateRange.y = -0x800 * entity->size * RSDK.Cos256(entity->angle);
        entity->updateRange.y += 0x200000;
        entity->visible   = false;
        entity->drawOrder = Zone->drawOrderLow;
    }
}

void SpinBooster_StageLoad(void) { SpinBooster->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

int32 SpinBooster_GetRollDir(EntityPlayer *player)
{
    RSDK_THIS(SpinBooster);

    switch (entity->direction) {
        case FLIP_NONE:
            if (entity->autoGrip != 5) {
                if ((entity->autoGrip == 7 || entity->autoGrip == 8) && player->up)
                    return CMODE_ROOF;
                return CMODE_FLOOR;
            }
            if (player->down) {
                return CMODE_FLOOR;
            }
            if (player->up) {
                return CMODE_ROOF;
            }

            if (!entity->bias)
                return CMODE_FLOOR;
            else
                return CMODE_ROOF;
        case FLIP_X:
            if (entity->autoGrip != 5) {
                if (entity->autoGrip == 7) {
                    if (!player->left) {
                        return -1;
                    }
                    return CMODE_RWALL;
                }

                if (entity->autoGrip == 6) {
                    if (!player->right)
                        return -1;
                    return CMODE_LWALL;
                }
                if (entity->autoGrip != 8)
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
                return CMODE_ROOF * (entity->bias != 0) + 1;
            }
        case FLIP_Y:
            if (entity->autoGrip != 5) {
                if ((entity->autoGrip == 6 || entity->autoGrip == 8) && player->up)
                    return CMODE_ROOF;
                return CMODE_FLOOR;
            }
            if (player->down)
                return CMODE_FLOOR;
            if (player->up)
                return CMODE_ROOF;

            if (!entity->bias)
                return CMODE_ROOF;
            else
                return CMODE_FLOOR;
        case FLIP_XY:
            if (entity->autoGrip == 5) {
                return 2 * (player->left != 0) + 1;
            }
            else if (entity->autoGrip == 6) {
                if (!player->left)
                    return -1;
                return CMODE_RWALL;
            }
            else {
                if (entity->autoGrip == 7) {
                    if (!player->right)
                        return -1;
                    return CMODE_LWALL;
                }
                if (entity->autoGrip != 8)
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

    if (entity->autoGrip) {
        int8 cMode = 0;
        if (entity->autoGrip < 5) {
            cMode = entity->autoGrip - 1;
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
        int32 entAng = RSDK.Sin256(entity->angle) + RSDK.Cos256(entity->angle);
        int32 plrAng = RSDK.Cos256(player->angle) - RSDK.Sin256(player->angle);
        int32 power  = (entity->boostPower << 15) * ((plrAng > 0) - (plrAng < 0)) * ((entAng > 0) - (entAng < 0));
        if (entity->boostPower >= 0)
            player->groundVel += power;
        else
            player->groundVel = power;
    }
    else {
        int32 x = ( 0x80 * RSDK.Cos256(entity->angle)) * entity->boostPower;
        int32 y = (-0x80 * RSDK.Sin256(entity->angle)) * entity->boostPower;
        if (entity->boostPower >= 0) {
            player->velocity.x += x;
            player->velocity.y += y;
        }
        else {
            player->velocity.x = x;
            player->velocity.y = y;
        }
    }

    if (entity->boostPower < 0 && !entity->forwardOnly) {
        if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            if (!entity->allowTubeInput)
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

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3(&entity->position, &drawPos, entity->angle);

    for (int32 i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    if (RSDK_sceneInfo->inEditor) {
        uint8 negAngle = -entity->angle;
        int32 power     = entity->boostPower;
        int32 x        = entity->position.x;
        int32 y        = entity->position.y;
        int32 clr       = ((power >> 31) & 0xFE0001) + 0xFFFF;
        if (!power)
            power = 1;
        SpinBooster_DrawArrow(clr, x, y, power * (RSDK.Cos256(negAngle) << 11) + entity->position.x,
                              power * (RSDK.Sin256(negAngle) << 11) + entity->position.y);

        switch (entity->autoGrip) {
            default: break;
            case 1:
            case 2:
            case 3:
            case 4: {
                int32 angle = 0;
                switch (entity->autoGrip) {
                    case 1: angle = 0x40; break;
                    case 2: angle = 0; break;
                    case 3: angle = -0x40; break;
                    case 4: angle = 0x80; break;
                }
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(angle) << 14) + entity->position.x,
                                     (RSDK.Sin256(angle) << 14) + entity->position.y);
                break;
            }
            case 5:
                if (!entity->bias)
                    clr = 0x00FF00;
                else
                    clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle + 64)) << 14) + entity->position.x,
                                     (RSDK.Sin256((uint8)(negAngle + 64)) << 14) + entity->position.y);
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle - 64)) << 14) + entity->position.x,
                                     (RSDK.Sin256((uint8)(negAngle - 64)) << 14) + entity->position.y);
                break;
            case 6:
                clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle + 64)) << 14) + entity->position.x,
                                     (RSDK.Sin256((uint8)(negAngle + 64)) << 14) + entity->position.y);
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(negAngle) << 14) + entity->position.x,
                                     (RSDK.Sin256(negAngle) << 14) + entity->position.y);
                break;
            case 0:
            case 7:
                clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle - 64)) << 14) + entity->position.x,
                                     (RSDK.Sin256((uint8)(negAngle - 64)) << 14) + entity->position.y);
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(negAngle) << 14) + entity->position.x,
                                     (RSDK.Sin256(negAngle) << 14) + entity->position.y);
                break;
            case 8:
                clr = 0xFFFF00;
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle + 64)) << 14) + entity->position.x,
                                     (RSDK.Sin256((uint8)(negAngle + 64)) << 14) + entity->position.y);
                SpinBooster_DrawArrow(clr, x, y, (RSDK.Cos256((uint8)(negAngle - 64)) << 14) + entity->position.x,
                                     (RSDK.Sin256((uint8)(negAngle - 64)) << 14) + entity->position.y);
                SpinBooster_DrawArrow(0xFF00, x, y, (RSDK.Cos256(negAngle) << 14) + entity->position.x,
                                     (RSDK.Sin256(negAngle) << 14) + entity->position.y);
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
        if (entity->boostAlways)
            SpinBooster_ApplyRollVelocity(player);
    }
    else {
        if (entity->playSound)
            RSDK.PlaySfx(Player->sfx_Roll, 0, 0xFF);
        if (player->playerAnimator.animationID != ANI_JUMP) {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
            if (!player->collisionMode && player->onGround)
                player->position.y += player->cameraOffset;
            player->pushing = false;
        }
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        if (!entity->allowTubeInput)
            player->controlLock = 0xFFFF;

        if (player->onGround)
            player->state = Player_State_ForceRoll_Ground;
        else
            player->state = Player_State_ForceRoll_Air;

        if (abs(player->groundVel) < 0x10000) {
            if (entity->direction & FLIP_X)
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
