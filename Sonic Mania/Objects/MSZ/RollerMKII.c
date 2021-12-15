// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RollerMKII Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectRollerMKII *RollerMKII;

void RollerMKII_Update(void)
{
    RSDK_THIS(RollerMKII);
    StateMachine_Run(self->state);
}

void RollerMKII_LateUpdate(void) {}

void RollerMKII_StaticUpdate(void) {}

void RollerMKII_Draw(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void RollerMKII_Create(void *data)
{
    RSDK_THIS(RollerMKII);
    self->visible         = true;
    self->drawOrder       = Zone->drawOrderLow;
    self->startPos        = self->position;
    self->startDir        = self->direction;
    self->drawFX          = FX_FLIP;
    self->active          = ACTIVE_BOUNDS;
    self->updateRange.x   = 0x1000000;
    self->updateRange.y   = 0x1000000;
    self->onGround        = false;
    self->tileCollisions  = true;
    self->collisionLayers = Zone->fgLayers;
    self->collisionPlane  = 0;
    RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &self->animator, true, 5);
    self->state = RollerMKII_State_Setup;
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
    RollerMKII->sfxBumper          = RSDK.GetSfx("Stage/Bumper3.wav");
    RollerMKII->sfxJump            = RSDK.GetSfx("Stage/Jump2.wav");
    RollerMKII->sfxDropDash        = RSDK.GetSfx("Global/DropDash.wav");
    RollerMKII->sfxRelease         = RSDK.GetSfx("Global/Release.wav");
    RollerMKII->sfxSkidding        = RSDK.GetSfx("Global/Skidding.wav");
    DEBUGMODE_ADD_OBJ(RollerMKII);
}

void RollerMKII_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    EntityRollerMKII *rollerMKII = CREATE_ENTITY(RollerMKII, NULL, self->position.x, self->position.y);
    rollerMKII->direction        = self->direction;
    rollerMKII->startDir         = self->direction;
}

void RollerMKII_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &DebugMode->animator, true, 1);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void RollerMKII_CheckOnScreen(void)
{
    RSDK_THIS(RollerMKII);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        RollerMKII_Create(NULL);
    }
}

void RollerMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(RollerMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &RollerMKII->hitbox1))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void RollerMKII_CheckPlayerCollisions_Rolling(void)
{
    RSDK_THIS(RollerMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &RollerMKII->hitbox1)) {

            int anim    = player->animator.animationID;
            bool32 flag = false;
#if RETRO_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop) {
                Player_CheckBadnikBreak(self, player, true);
            }
            else {
                flag = player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || player->jumpAbilityTimer > 1);
#endif
                if (Player_CheckAttacking(player, self) || flag) {
                    RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
                    int angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
                    int velX  = 0x380 * RSDK.Cos256(angle);
                    int velY  = 0x380 * RSDK.Sin256(angle);

                    if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED) {
                        if (player->state != Player_State_TailsFlight) {
                            if (player->state != Player_State_DropDash)
                                player->state = Player_State_Air;
                            if (anim != ANI_JUMP && anim != ANI_DASH)
                                player->animator.animationID = ANI_WALK;
                        }
                    }

#if RETRO_USE_PLUS
                    if (player->characterID != ID_MIGHTY || player->jumpAbilityTimer <= 1) {
#endif
                        player->velocity.x  = velX;
                        player->groundVel   = velX;
                        player->jumpAbility = 0;
                        if (player->characterID == ID_KNUCKLES && player->animator.animationID == ANI_FLY) {
                            RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLYTIRED, &player->animator, false, 0);
                            player->state = Player_State_KnuxGlideDrop;
                        }
                        player->velocity.y     = velY;
                        player->onGround       = false;
                        player->tileCollisions = true;
#if RETRO_USE_PLUS
                    }
#endif
                    RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
                    RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 2, &self->animator, true, 0);
                    self->velocity.y = -0x40000;
                    self->velocity.x = -velX;
                    self->onGround   = false;
                    self->field_68   = 0;
                    self->timer      = 45;
                    self->state      = RollerMKII_State_Unknown6;
                    self->direction  = player->position.x < self->position.x;
                }
                else {
                    Player_CheckHit(player, self);
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
    int velX = self->velocity.x;
    int side = RSDK.CheckObjectCollisionBox(otherEntity, hitbox, self, &RollerMKII->hitbox2, true);

    if ((side == C_LEFT && velX > 0) || (side == C_RIGHT && velX < 0)) {
        if (self->state != RollerMKII_State_Unknown6) {
            RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
            RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &self->animator, true, 0);
            self->velocity.y = -0x40000;
            self->onGround   = false;
            self->field_68   = 0;
            self->timer      = 30;
            if (velX < 0)
                self->velocity.x = 0x20000;
            else
                self->velocity.x = -0x20000;
            self->state     = RollerMKII_State_Unknown6;
            self->direction = self->position.x < Player_GetNearestPlayer()->position.x;
        }
    }
    return side;
}

bool32 RollerMKII_HandlePlatformCollisions(EntityPlatform *platform)
{
    RSDK_THIS(RollerMKII);
    bool32 flag = false;

    if (platform->state != Platform_State_Collapse_Falling && platform->state != Platform_State_Collapse_CheckReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;
        if (platform->collision) {
            if (platform->collision != PLATFORM_C_SOLID_ALL) {
                if (platform->collision == PLATFORM_C_USE_TILES
                    && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, self, &RollerMKII->hitbox2)) {
                    if (self->collisionLayers & Zone->moveID) {
                        TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                        move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                        move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                    }
                    if (self->velocity.y >= 0x3800)
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
            RSDK.CheckObjectCollisionPlatform(platform, hitbox, self, &RollerMKII->hitbox2, true);
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
        int32 x     = (self->position.x - planeSwitch->position.x) >> 8;
        int32 y     = (self->position.y - planeSwitch->position.y) >> 8;
        int32 scanX = (y * RSDK.Sin256(planeSwitch->negAngle)) + (x * RSDK.Cos256(planeSwitch->negAngle)) + planeSwitch->position.x;
        int32 scanY = (y * RSDK.Cos256(planeSwitch->negAngle)) - (x * RSDK.Sin256(planeSwitch->negAngle)) + planeSwitch->position.y;
        int32 pos = ((self->velocity.y >> 8) * RSDK.Sin256(planeSwitch->negAngle)) + (self->velocity.x >> 8) * RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Cos256(planeSwitch->negAngle);
        RSDK.Sin256(planeSwitch->negAngle);
        if (!(planeSwitch->onPath && !self->onGround)) {
            int32 xDif = abs(scanX - planeSwitch->position.x);
            int32 yDif = abs(scanY - planeSwitch->position.y);

            if (xDif < 0x180000 && yDif < planeSwitch->size << 19) {
                if (scanX + pos >= planeSwitch->position.x) {
                    self->collisionPlane = (planeSwitch->flags >> 3) & 1;
                    if (!(planeSwitch->flags & 4))
                        self->drawOrder = Zone->playerDrawLow;
                    else
                        self->drawOrder = Zone->playerDrawHigh;
                }
                else {
                    self->collisionPlane = (planeSwitch->flags >> 1) & 1;
                    if (!(planeSwitch->flags & 1))
                        self->drawOrder = Zone->playerDrawLow;
                    else
                        self->drawOrder = Zone->playerDrawHigh;
                }
            }
        }
    }

    foreach_all(Platform, platform) { RollerMKII_HandlePlatformCollisions(platform); }
    foreach_all(Spikes, spikes) { RollerMKII_HandleObjectCollisions((Entity *)spikes, &spikes->hitbox); }
    foreach_all(BreakableWall, wall) { RollerMKII_HandleObjectCollisions((Entity *)wall, &wall->hitbox); }

    if (!self->collisionMode && self->state != RollerMKII_State_Unknown6) {
        bool32 collided = self->direction == FLIP_X
                              ? RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, self->collisionPlane, -0xA0000, 0, false)
                              : RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, self->collisionPlane, 0xA0000, 0, false);
        if (collided) {
            RSDK.PlaySfx(RollerMKII->sfxBumper, false, 255);
            RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &self->animator, true, 0);
            self->onGround   = false;
            self->velocity.y = -0x40000;
            if (self->direction == FLIP_X)
                self->velocity.x = 0x20000;
            else
                self->velocity.x = -0x20000;
            self->field_68  = 0;
            self->timer     = 30;
            self->state     = RollerMKII_State_Unknown6;
            self->direction = self->position.x < Player_GetNearestPlayer()->position.x;
        }
    }
}

void RollerMKII_State_Setup(void)
{
    RSDK_THIS(RollerMKII);
    self->active     = ACTIVE_NORMAL;
    self->velocity.x = 0;
    self->velocity.y = 0;
    self->groundVel  = 0;
    self->field_68   = 0;
    self->state      = RollerMKII_State_Unknown1;
    RollerMKII_State_Unknown1();
}

void RollerMKII_State_Unknown1(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.ProcessAnimation(&self->animator);

    Vector2 range;
    range.x = 0x200000;
    range.y = 0x200000;
    if (self->timer) {
        self->timer--;
    }
    else if (RSDK.CheckOnScreen(self, &range)) {
        bool32 collided = false;
        if (self->direction == FLIP_X)
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, self->collisionPlane, -1572864, 0, false);
        else
            collided = RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, self->collisionPlane, 1572864, 0, false);
        if (collided)
            self->direction ^= 1;

        foreach_active(Player, player)
        {
            if (self->direction) {
                if (player->position.x < self->position.x) {
                    if (self->position.x - player->position.x < 0x800000) {
                        self->velocity.y = -0x40000;
                        RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 1, &self->animator, true, 0);
                        self->playerPtr = player;
                        RSDK.PlaySfx(RollerMKII->sfxJump, false, 255);
                        self->state = RollerMKII_State_Unknown2;
                    }
                }
            }
            else {
                if (player->position.x > self->position.x) {
                    if (player->position.x - self->position.x < 0x800000) {
                        self->velocity.y = -0x40000;
                        RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 1, &self->animator, true, 0);
                        self->playerPtr = player;
                        RSDK.PlaySfx(RollerMKII->sfxJump, false, 255);
                        self->state = RollerMKII_State_Unknown2;
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
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    RSDK.ProcessAnimation(&self->animator);

    if (self->velocity.y > 0) {
        self->timer = 15;
        RSDK.PlaySfx(RollerMKII->sfxDropDash, false, 255);
        self->state = RollerMKII_State_Unknown3;
    }
    if (self->animator.frameID <= 4)
        RollerMKII_CheckPlayerCollisions();
    else
        RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown3(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.ProcessAnimation(&self->animator);
    if (!--self->timer)
        self->state = RollerMKII_State_Unknown4;
    RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown4(void)
{
    RSDK_THIS(RollerMKII);
    if (!self->field_68) {
        if (self->position.x <= self->playerPtr->position.x) {
            self->direction = FLIP_NONE;
            if (self->velocity.x < 0x80000)
                self->velocity.x += 0x3800;
        }
        else {
            self->direction = FLIP_X;
            if (self->velocity.x > -0x80000)
                self->velocity.x -= 0x3800;
        }
    }

    RSDK.ProcessTileCollisions(self, &RollerMKII->hitbox4, &RollerMKII->hitbox3);
    self->velocity.y += 0x3800;
    RollerMKII_HandleCollisions();
    if (self->onGround) {
        if (abs(self->angle) & 0x60) {
            self->state = RollerMKII_State_Unknown5;
        }
        else {
            if (self->velocity.x)
                self->groundVel = self->velocity.x;
            if (!self->field_68) {
                self->field_68 = true;
                if (self->direction == FLIP_NONE) {
                    self->groundVel  = 0x80000;
                    self->velocity.x = 0x80000;
                }
                else {
                    self->groundVel  = -0x80000;
                    self->velocity.x = -0x80000;
                }
                self->velocity.y = -0x20000;
                self->onGround   = false;
                RSDK.StopSfx(RollerMKII->sfxDropDash);
                RSDK.PlaySfx(RollerMKII->sfxRelease, false, 255);
                Hitbox *hitbox = RSDK.GetHitbox(&self->animator, 0);

                EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y + 0xE0000);
                RSDK.SetSpriteAnimation(Dust->aniFrames, 2, &dust->animator, true, 0);
                dust->state = Dust_State_Move;
                dust->position.y += hitbox->bottom << 16;
                dust->direction = self->direction;
                dust->drawOrder = self->drawOrder;
                if (self->direction)
                    dust->position.x += 0x90000;
                else
                    dust->position.x -= 0x90000;
            }
            else if (self->velocity.y <= 0x10000) {
                self->state = RollerMKII_State_Unknown5;
            }
            else {
                self->velocity.y = -0x20000;
                self->onGround   = false;
            }
        }
        self->direction = self->groundVel < 0;
    }

    RSDK.ProcessAnimation(&self->animator);
    RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown5(void)
{
    RSDK_THIS(RollerMKII);
    if (!self->collisionMode) {
        if (self->position.x <= self->playerPtr->position.x) {
            self->direction = FLIP_NONE;
            if (self->groundVel < 0x80000) {
                self->groundVel += 0x3800;
                if (self->groundVel > 0) {
                    self->groundVel += 0x1400;
                    if (self->groundVel >= -0x40000) {
                        self->timer = 0;
                    }
                    else {
                        RSDK.PlaySfx(RollerMKII->sfxSkidding, false, 255);
                        self->timer = (self->timer + 1) & 3;
                        if (!self->timer)
                            CREATE_ENTITY(Dust, NULL, self->position.x, self->position.y + 0xE0000)->state = Dust_State_Move;
                    }
                }
            }
        }
        else {
            self->direction = FLIP_X;
            if (self->groundVel > -0x80000) {
                self->groundVel -= 0x3800;
                if (self->groundVel > 0) {
                    self->groundVel += 0x1400;
                    if (self->groundVel <= 0x40000) {
                        self->timer = 0;
                    }
                    else {
                        RSDK.PlaySfx(RollerMKII->sfxSkidding, false, 255);
                        self->timer = (self->timer + 1) & 3;
                        if (!self->timer)
                            CREATE_ENTITY(Dust, NULL, self->position.x, self->position.y + 0xE0000)->state = Dust_State_Move;
                    }
                }
            }
        }
    }

    RSDK.ProcessTileCollisions(self, &RollerMKII->hitbox4, &RollerMKII->hitbox3);
    self->groundVel = (RSDK.Sin256(self->angle) << 13 >> 8) + self->groundVel;
    if (self->collisionMode != CMODE_FLOOR) {
        if (self->angle >= 64 && self->angle <= 192 && (self->groundVel + 0x1FFFF) <= 0x3FFFE) {
            self->onGround      = false;
            self->angle         = 0;
            self->collisionMode = 0;
        }
    }
    RollerMKII_HandleCollisions();
    if (self->onGround == false && self->state != RollerMKII_State_Unknown6)
        self->state = RollerMKII_State_Unknown4;
    RSDK.ProcessAnimation(&self->animator);
    RollerMKII_CheckPlayerCollisions_Rolling();
    RollerMKII_CheckOnScreen();
}

void RollerMKII_State_Unknown6(void)
{
    RSDK_THIS(RollerMKII);
    RSDK.ProcessTileCollisions(self, &RollerMKII->hitboxOuter, &RollerMKII->hitboxInner);
    self->velocity.y += 0x3800;
    RollerMKII_HandleCollisions();
    if (self->onGround) {
        self->groundVel  = 0;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = RollerMKII_State_Unknown1;
        RSDK.SetSpriteAnimation(RollerMKII->aniFrames, 0, &self->animator, true, 0);
    }
    RSDK.ProcessAnimation(&self->animator);
    RollerMKII_CheckOnScreen();
}

#if RETRO_INCLUDE_EDITOR
void RollerMKII_EditorDraw(void) { RollerMKII_Draw(); }

void RollerMKII_EditorLoad(void) { RollerMKII->aniFrames = RSDK.LoadSpriteAnimation("MSZ/RollerMKII.bin", SCOPE_STAGE); }
#endif

void RollerMKII_Serialize(void) { RSDK_EDITABLE_VAR(RollerMKII, VAR_UINT8, direction); }
