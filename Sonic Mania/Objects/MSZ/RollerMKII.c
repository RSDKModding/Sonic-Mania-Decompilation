#include "SonicMania.h"

ObjectRollerMKII *RollerMKII;

void RollerMKII_Update(void)
{
    RSDK_THIS(RollerMKII);
    StateMachine_Run(entity->state);
}

void RollerMKII_LateUpdate(void) {}

void RollerMKII_StaticUpdate(void) {}

void RollerMKII_Draw(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void RollerMKII_Create(void *data)
{
    RSDK_THIS(RollerMKII);
    entity->visible         = true;
    entity->drawOrder       = Zone->drawOrderLow;
    entity->startPos        = entity->position;
    entity->startDir        = entity->direction;
    entity->drawFX          = FX_FLIP;
    entity->active          = ACTIVE_BOUNDS;
    entity->updateRange.x   = 0x1000000;
    entity->updateRange.y   = 0x1000000;
    entity->onGround        = false;
    entity->tileCollisions  = true;
    entity->collisionLayers = Zone->fgLayers;
    entity->collisionPlane  = 0;
    RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &entity->animator, true, 5);
    entity->state = RollerMKII_State_Setup;
}

void RollerMKII_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        RollerMKII->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RollerMKII.bin", SCOPE_STAGE);
    RollerMKII->hitbox4.left       = -9;
    RollerMKII->hitbox4.top        = -14;
    RollerMKII->hitbox4.right      = 9;
    RollerMKII->hitbox4.bottom     = 14;
    RollerMKII->hitbox3.left       = -8;
    RollerMKII->hitbox3.top        = -14;
    RollerMKII->hitbox3.right      = 8;
    RollerMKII->hitbox3.bottom     = 14;
    RollerMKII->hitboxOuter.left   = -9;
    RollerMKII->hitboxOuter.top    = -14;
    RollerMKII->hitboxOuter.right  = 9;
    RollerMKII->hitboxOuter.bottom = 24;
    RollerMKII->hitboxInner.left   = -8;
    RollerMKII->hitboxInner.top    = -14;
    RollerMKII->hitboxInner.right  = 8;
    RollerMKII->hitboxInner.bottom = 24;
    RollerMKII->hitbox1.left       = -14;
    RollerMKII->hitbox1.top        = -14;
    RollerMKII->hitbox1.right      = 14;
    RollerMKII->hitbox1.bottom     = 14;
    RollerMKII->hitbox2.left       = -14;
    RollerMKII->hitbox2.top        = -14;
    RollerMKII->hitbox2.right      = 14;
    RollerMKII->hitbox2.bottom     = 14;
    RollerMKII->sfxBumper          = RSDK.GetSFX("Stage/Bumper3.wav");
    RollerMKII->sfxJump            = RSDK.GetSFX("Stage/Jump2.wav");
    RollerMKII->sfxDropDash        = RSDK.GetSFX("Global/DropDash.wav");
    RollerMKII->sfxRelease         = RSDK.GetSFX("Global/Release.wav");
    RollerMKII->sfxSkidding        = RSDK.GetSFX("Global/Skidding.wav");
    DEBUGMODE_ADD_OBJ(RollerMKII);
}

void RollerMKII_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityRollerMKII *rollerMKII = CREATE_ENTITY(RollerMKII, NULL, entity->position.x, entity->position.y);
    rollerMKII->direction        = entity->direction;
    rollerMKII->startDir         = entity->direction;
}

void RollerMKII_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &DebugMode->animator, true, 1);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void RollerMKII_CheckOnScreen(void)
{
    RSDK_THIS(RollerMKII);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        RollerMKII_Create(NULL);
    }
}

void RollerMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(RollerMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &RollerMKII->hitbox1))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void RollerMKII_CheckPlayerCollisions_Rolling(void)
{
    RSDK_THIS(RollerMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &RollerMKII->hitbox1)) {

            int anim    = player->playerAnimator.animationID;
            bool32 flag = false;
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                Player_CheckBadnikBreak(entity, player, true);
            }
            else {
                flag = player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || player->jumpAbilityTimer > 1);
#endif
                if (Player_CheckAttacking(player, entity) || flag) {
                    RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
                    int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);
                    int velX  = 0x380 * RSDK.Cos256(angle);
                    int velY  = 0x380 * RSDK.Sin256(angle);

                    if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED) {
                        if (player->state != Player_State_TailsFlight) {
                            if (player->state != Player_State_DropDash)
                                player->state = Player_State_Air;
                            if (anim != ANI_JUMP && anim != ANI_DASH)
                                player->playerAnimator.animationID = ANI_WALK;
                        }
                    }

#if RETRO_USE_PLUS
                    if (player->characterID != ID_MIGHTY || player->jumpAbilityTimer <= 1) {
#endif
                        player->velocity.x  = velX;
                        player->groundVel   = velX;
                        player->jumpAbility = 0;
                        if (player->characterID == ID_KNUCKLES && player->playerAnimator.animationID == ANI_FLY) {
                            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimator, false, 0);
                            player->state = Player_State_KnuxGlideDrop;
                        }
                        player->velocity.y     = velY;
                        player->onGround       = false;
                        player->tileCollisions = true;
#if RETRO_USE_PLUS
                    }
#endif
                    RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
                    RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 2, &entity->animator, true, 0);
                    entity->velocity.y = -0x40000;
                    entity->velocity.x = -velX;
                    entity->onGround   = false;
                    entity->field_68   = 0;
                    entity->timer      = 45;
                    entity->state      = RollerMKII_State_Unknown6;
                    entity->direction  = player->position.x < entity->position.x;
                }
                else {
                    Player_CheckHit(player, entity);
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}

int RollerMKII_HandleObjectCollisions(Entity *otherEntity, Hitbox *hitbox)
{
    RSDK_THIS(RollerMKII);
    int velX = entity->velocity.x;
    int side = RSDK.CheckObjectCollisionBox(otherEntity, hitbox, entity, &RollerMKII->hitbox2, true);

    if ((side == C_LEFT && velX > 0) || (side == C_RIGHT && velX < 0)) {
        if (entity->state != RollerMKII_State_Unknown6) {
            RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
            RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &entity->animator, true, 0);
            entity->velocity.y = -0x40000;
            entity->onGround   = false;
            entity->field_68   = 0;
            entity->timer      = 30;
            if (velX < 0)
                entity->velocity.x = 0x20000;
            else
                entity->velocity.x = -0x20000;
            entity->state     = RollerMKII_State_Unknown6;
            entity->direction = entity->position.x < Player_GetNearestPlayer()->position.x;
        }
    }
    return side;
}

bool32 RollerMKII_HandlePlatformCollisions(EntityPlatform *platform)
{
    RSDK_THIS(RollerMKII);
    bool32 flag = false;

    if (platform->state != Platform_State_Falling && platform->state != Platform_State_OffScreenReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        if (platform->collision) {
            if (platform->collision != PLATFORM_C_1) {
                if (platform->collision == PLATFORM_C_2
                    && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, entity, &RollerMKII->hitbox2)) {
                    if (entity->collisionLayers & Zone->moveID) {
                        TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                        move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                        move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                    }
                    if (entity->velocity.y >= 0x3800)
                        flag = true;
                }
            }
            else {
                Hitbox *hitbox = RSDK.GetHitbox(&platform->animator, 1);
                flag           = RollerMKII_HandleObjectCollisions((Entity *)platform, hitbox);
            }
        }
        else {
            Hitbox *hitbox = RSDK.GetHitbox(&platform->animator, 0);
            RSDK.CheckObjectCollisionPlatform(platform, hitbox, entity, &RollerMKII->hitbox2, true);
        }
        platform->position = platform->centerPos;
    }
    return flag;
}

void RollerMKII_HandleCollisions(void)
{
    RSDK_THIS(RollerMKII);

    foreach_all(PlaneSwitch, planeSwitch)
    {
        int32 x     = (entity->position.x - planeSwitch->position.x) >> 8;
        int32 y     = (entity->position.y - planeSwitch->position.y) >> 8;
        int32 scanX = (y * RSDK.Sin256(planeSwitch->negAngle)) + (x * RSDK.Cos256(planeSwitch->negAngle)) + planeSwitch->position.x;
        int32 scanY = (y * RSDK.Cos256(planeSwitch->negAngle)) - (x * RSDK.Sin256(planeSwitch->negAngle)) + planeSwitch->position.y;
        int32 pos = ((entity->velocity.y >> 8) * RSDK.Sin256(planeSwitch->negAngle)) + (entity->velocity.x >> 8) * RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Sin256(planeSwitch->negAngle);
        if (!(planeSwitch->onPath && !entity->onGround)) {
            int32 xDif = abs(scanX - planeSwitch->position.x);
            int32 yDif = abs(scanY - planeSwitch->position.y);

            if (xDif < 0x180000 && yDif < planeSwitch->size << 19) {
                if (scanX + pos >= planeSwitch->position.x) {
                    entity->collisionPlane = (planeSwitch->flags >> 3) & 1;
                    if (!(planeSwitch->flags & 4))
                        entity->drawOrder = Zone->playerDrawLow;
                    else
                        entity->drawOrder = Zone->playerDrawHigh;
                }
                else {
                    entity->collisionPlane = (planeSwitch->flags >> 1) & 1;
                    if (!(planeSwitch->flags & 1))
                        entity->drawOrder = Zone->playerDrawLow;
                    else
                        entity->drawOrder = Zone->playerDrawHigh;
                }
            }
        }
    }

    foreach_all(Platform, platform) { RollerMKII_HandlePlatformCollisions(platform); }
    foreach_all(Spikes, spikes) { RollerMKII_HandleObjectCollisions((Entity *)spikes, &spikes->hitbox); }
    foreach_all(BreakableWall, wall) { RollerMKII_HandleObjectCollisions((Entity *)wall, &wall->hitbox); }

    if (!entity->collisionMode && entity->state != RollerMKII_State_Unknown6) {
        bool32 collided = entity->direction == FLIP_X
                              ? RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, entity->collisionPlane, -0xA0000, 0, false)
                              : RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, entity->collisionPlane, 0xA0000, 0, false);
        if (collided) {
            RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
            RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &entity->animator, true, 0);
            entity->onGround   = false;
            entity->velocity.y = -0x40000;
            if (entity->direction == FLIP_X)
                entity->velocity.x = 0x20000;
            else
                entity->velocity.x = -0x20000;
            entity->field_68  = 0;
            entity->timer     = 30;
            entity->state     = RollerMKII_State_Unknown6;
            entity->direction = entity->position.x < Player_GetNearestPlayer()->position.x;
        }
    }
}

void RollerMKII_State_Setup(void)
{
    RSDK_THIS(RollerMKII);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
    entity->groundVel  = 0;
    entity->field_68   = 0;
    entity->state      = RollerMKII_State_Unknown1;
    RollerMKII_State_Unknown1();
}

void RollerMKII_State_Unknown1(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.ProcessAnimation(&entity->animator);

    Vector2 range;
    range.x = 0x200000;
    range.y = 0x200000;
    if (entity->timer) {
        entity->timer--;
    }
    else if (RSDK.CheckOnScreen(entity, &range)) {
        bool32 collided = false;
        if (entity->direction == FLIP_X)
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, entity->collisionPlane, -1572864, 0, false);
        else
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, entity->collisionPlane, 1572864, 0, false);
        if (collided)
            entity->direction ^= 1;

        foreach_active(Player, player)
        {
            if (entity->direction) {
                if (player->position.x < entity->position.x) {
                    if (entity->position.x - player->position.x < 0x800000) {
                        entity->velocity.y = -0x40000;
                        RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 1, &entity->animator, true, 0);
                        entity->playerPtr = player;
                        RSDK.PlaySfx(RollerMKII->sfxJump, false, 255);
                        entity->state = RollerMKII_State_Unknown2;
                    }
                }
            }
            else {
                if (player->position.x > entity->position.x) {
                    if (player->position.x - entity->position.x < 0x800000) {
                        entity->velocity.y = -0x40000;
                        RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 1, &entity->animator, true, 0);
                        entity->playerPtr = player;
                        RSDK.PlaySfx(RollerMKII->sfxJump, false, 255);
                        entity->state = RollerMKII_State_Unknown2;
                    }
                }
            }
        }
    }

    RollerMKII_CheckPlayerCollisions();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown2(void)
{
    RSDK_THIS(RollerMKII);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    RSDK.ProcessAnimation(&entity->animator);

    if (entity->velocity.y > 0) {
        entity->timer = 15;
        RSDK.PlaySfx(RollerMKII->sfxDropDash, false, 255);
        entity->state = RollerMKII_State_Unknown3;
    }
    if (entity->animator.frameID <= 4)
        RollerMKII_CheckPlayerCollisions();
    else
        RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown3(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.ProcessAnimation(&entity->animator);
    if (!--entity->timer)
        entity->state = RollerMKII_State_Unknown4;
    RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown4(void)
{
    RSDK_THIS(RollerMKII);
    if (!entity->field_68) {
        if (entity->position.x <= entity->playerPtr->position.x) {
            entity->direction = FLIP_NONE;
            if (entity->velocity.x < 0x80000)
                entity->velocity.x += 0x3800;
        }
        else {
            entity->direction = FLIP_X;
            if (entity->velocity.x > -0x80000)
                entity->velocity.x -= 0x3800;
        }
    }

    RSDK.ProcessTileCollisions(entity, &RollerMKII->hitbox4, &RollerMKII->hitbox3);
    entity->velocity.y += 0x3800;
    RollerMKII_HandleCollisions();
    if (entity->onGround) {
        if (abs(entity->angle) & 0x60) {
            entity->state = RollerMKII_State_Unknown5;
        }
        else {
            if (entity->velocity.x)
                entity->groundVel = entity->velocity.x;
            if (!entity->field_68) {
                entity->field_68 = true;
                if (entity->direction == FLIP_NONE) {
                    entity->groundVel  = 0x80000;
                    entity->velocity.x = 0x80000;
                }
                else {
                    entity->groundVel  = -0x80000;
                    entity->velocity.x = -0x80000;
                }
                entity->velocity.y = -0x20000;
                entity->onGround   = false;
                RSDK.StopSFX(RollerMKII->sfxDropDash);
                RSDK.PlaySfx(RollerMKII->sfxRelease, false, 255);
                Hitbox *hitbox = RSDK.GetHitbox(&entity->animator, 0);

                EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->position.x, entity->position.y + 0xE0000);
                RSDK.SetSpriteAnimation(Dust->spriteIndex, 2, &dust->animator, true, 0);
                dust->state = Dust_State_DropDash;
                dust->position.y += hitbox->bottom << 16;
                dust->direction = entity->direction;
                dust->drawOrder = entity->drawOrder;
                if (entity->direction)
                    dust->position.x += 0x90000;
                else
                    dust->position.x -= 0x90000;
            }
            else if (entity->velocity.y <= 0x10000) {
                entity->state = RollerMKII_State_Unknown5;
            }
            else {
                entity->velocity.y = -0x20000;
                entity->onGround   = false;
            }
        }
        entity->direction = entity->groundVel < 0;
    }

    RSDK.ProcessAnimation(&entity->animator);
    RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown5(void)
{
    RSDK_THIS(RollerMKII);
    if (!entity->collisionMode) {
        if (entity->position.x <= entity->playerPtr->position.x) {
            entity->direction = FLIP_NONE;
            if (entity->groundVel < 0x80000) {
                entity->groundVel += 0x3800;
                if (entity->groundVel > 0) {
                    entity->groundVel += 0x1400;
                    if (entity->groundVel >= -0x40000) {
                        entity->timer = 0;
                    }
                    else {
                        RSDK.PlaySfx(RollerMKII->sfxSkidding, false, 255);
                        entity->timer = (entity->timer + 1) & 3;
                        if (!entity->timer)
                            CREATE_ENTITY(Dust, NULL, entity->position.x, entity->position.y + 0xE0000)->state = Dust_State_DropDash;
                    }
                }
            }
        }
        else {
            entity->direction = FLIP_X;
            if (entity->groundVel > -0x80000) {
                entity->groundVel -= 0x3800;
                if (entity->groundVel > 0) {
                    entity->groundVel += 0x1400;
                    if (entity->groundVel <= 0x40000) {
                        entity->timer = 0;
                    }
                    else {
                        RSDK.PlaySfx(RollerMKII->sfxSkidding, false, 255);
                        entity->timer = (entity->timer + 1) & 3;
                        if (!entity->timer)
                            CREATE_ENTITY(Dust, NULL, entity->position.x, entity->position.y + 0xE0000)->state = Dust_State_DropDash;
                    }
                }
            }
        }
    }

    RSDK.ProcessTileCollisions(entity, &RollerMKII->hitbox4, &RollerMKII->hitbox3);
    entity->groundVel = (RSDK.Sin256(entity->angle) << 13 >> 8) + entity->groundVel;
    if (entity->collisionMode != CMODE_FLOOR) {
        if (entity->angle >= 64 && entity->angle <= 192 && (entity->groundVel + 0x1FFFF) <= 0x3FFFE) {
            entity->onGround      = false;
            entity->angle         = 0;
            entity->collisionMode = 0;
        }
    }
    RollerMKII_HandleCollisions();
    if (entity->onGround == false && entity->state != RollerMKII_State_Unknown6)
        entity->state = RollerMKII_State_Unknown4;
    RSDK.ProcessAnimation(&entity->animator);
    RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown6(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.ProcessTileCollisions(entity, &RollerMKII->hitboxOuter, &RollerMKII->hitboxInner);
    entity->velocity.y += 0x3800;
    RollerMKII_HandleCollisions();
    if (entity->onGround) {
        entity->groundVel  = 0;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = RollerMKII_State_Unknown1;
        RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &entity->animator, true, 0);
    }
    RSDK.ProcessAnimation(&entity->animator);
    RollerMKII_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void RollerMKII_EditorDraw(void) { RollerMKII_Draw(); }

void RollerMKII_EditorLoad(void) { RollerMKII->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RollerMKII.bin", SCOPE_STAGE); }
#endif

void RollerMKII_Serialize(void) { RSDK_EDITABLE_VAR(RollerMKII, VAR_UINT8, direction); }
