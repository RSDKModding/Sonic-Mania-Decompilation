// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Bumpalo Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBumpalo *Bumpalo;

void Bumpalo_Update(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->huffAnimator);

    StateMachine_Run(self->state);
}

void Bumpalo_LateUpdate(void) {}

void Bumpalo_StaticUpdate(void) {}

void Bumpalo_Draw(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.DrawSprite(&self->badnikAnimator, NULL, false);
    RSDK.DrawSprite(&self->huffAnimator, NULL, false);

    if (self->state == Bumpalo_State_Charging)
        RSDK.DrawSprite(&self->dustAnimator, NULL, false);
}

void Bumpalo_Create(void *data)
{
    RSDK_THIS(Bumpalo);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0] + 1;
    self->drawFX        = FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &self->badnikAnimator, true, 0);
    self->state = Bumpalo_State_Init;
}

void Bumpalo_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MSZ"))
        Bumpalo->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Bumpalo.bin", SCOPE_STAGE);

    Bumpalo->hitboxBadnik.left   = -20;
    Bumpalo->hitboxBadnik.top    = 0;
    Bumpalo->hitboxBadnik.right  = 20;
    Bumpalo->hitboxBadnik.bottom = 16;

    Bumpalo->hitboxBumper.left   = -10;
    Bumpalo->hitboxBumper.top    = -16;
    Bumpalo->hitboxBumper.right  = 12;
    Bumpalo->hitboxBumper.bottom = 0;

    Bumpalo->hitboxRange.left   = -128;
    Bumpalo->hitboxRange.top    = -64;
    Bumpalo->hitboxRange.right  = -16;
    Bumpalo->hitboxRange.bottom = 96;

    Bumpalo->hitboxCharge.left   = -17;
    Bumpalo->hitboxCharge.top    = -14;
    Bumpalo->hitboxCharge.right  = 10;
    Bumpalo->hitboxCharge.bottom = 0;

    Bumpalo->hitboxUnused.left   = -24;
    Bumpalo->hitboxUnused.top    = -10;
    Bumpalo->hitboxUnused.right  = -8;
    Bumpalo->hitboxUnused.bottom = 0;

    Bumpalo->sfxBumper = RSDK.GetSfx("Stage/Bumper.wav");
    Bumpalo->sfxHuff   = RSDK.GetSfx("Stage/Huff.wav");
    Bumpalo->sfxClack  = RSDK.GetSfx("Stage/Clack.wav");
    Bumpalo->sfxImpact = RSDK.GetSfx("Stage/Impact5.wav");

    DEBUGMODE_ADD_OBJ(Bumpalo);
}

void Bumpalo_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityBumpalo *bumpalo = CREATE_ENTITY(Bumpalo, NULL, self->position.x, self->position.y);
    bumpalo->direction     = self->direction;
    bumpalo->startDir      = self->direction;
}

void Bumpalo_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Bumpalo_CheckOffScreen(void)
{
    RSDK_THIS(Bumpalo);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Bumpalo_Create(NULL);
    }
}

void Bumpalo_BumpPlayer(EntityPlayer *player)
{
    RSDK_THIS(Bumpalo);

    RSDK.PlaySfx(Bumpalo->sfxBumper, false, 0xFF);

    int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
    int32 velX  = 0x700 * RSDK.Cos256(angle);
    int32 velY  = 0x700 * RSDK.Sin256(angle);

    if (player->state == Player_State_FlyCarried)
        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

    int32 anim = player->animator.animationID;
    if (anim != ANI_FLY && anim != ANI_FLY_LIFT_TIRED) {
        if (player->state != Player_State_TailsFlight) {
            if (player->state != Player_State_DropDash)
                player->state = Player_State_Air;
            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                player->animator.animationID = ANI_WALK;
        }
    }

    if (player->animator.animationID != ANI_FLY) {
        player->velocity.x   = velX;
        player->groundVel    = velX;
        player->applyJumpCap = false;
    }

    player->velocity.y     = velY;
    player->onGround       = false;
    player->tileCollisions = TILECOLLISION_DOWN;

    if (self->badnikAnimator.animationID == 3)
        self->velocity.x >>= 1;

    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->badnikAnimator, true, 0);

    if (self->state != Bumpalo_State_Falling && self->state != Bumpalo_State_Bumped)
        self->velocity.x = 0;

    self->state = Bumpalo_State_Bumped;
}

void Bumpalo_CheckPlayerCollisions(void)
{
    RSDK_THIS(Bumpalo);

    foreach_active(Player, player)
    {
        if (self->badnikAnimator.animationID == 3) {
            if (abs(self->velocity.x) <= 0x10000) {
                if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                    if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                        Player_CheckBadnikBreak(player, self, true);
                    }
                }
                else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBumper)) {
#if MANIA_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop) {
                        RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                        Player_CheckBadnikBreak(player, self, true);
                    }
                    else
#endif
                        Bumpalo_BumpPlayer(player);
                }
                else if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxCharge)) {
#if MANIA_USE_PLUS
                    if (Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer)) {
                        RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->badnikAnimator, true, 0);
                        self->state      = Bumpalo_State_Bumped;
                        self->velocity.x = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
                        self->velocity.y = -0x20000;
                    }
                    else
#endif
                        Player_Hurt(player, self);
                }
            }
            else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBumper)) {
#if MANIA_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                    Player_CheckBadnikBreak(player, self, true);
                }
                else
#endif
                    Bumpalo_BumpPlayer(player);
            }
            else if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxCharge)) {
#if MANIA_USE_PLUS
                if (Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer)) {
                    RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->badnikAnimator, true, 0);
                    self->state      = Bumpalo_State_Bumped;
                    self->velocity.x = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
                    self->velocity.y = -0x20000;
                }
                else
#endif
                    Player_Hurt(player, self);
            }
            else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                Player_CheckBadnikBreak(player, self, true);
            }
        }
        else {
            if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                    Player_CheckBadnikBreak(player, self, true);
                }
            }
            else {
                if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBumper)) {
#if MANIA_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop) {
                        RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                        Player_CheckBadnikBreak(player, self, true);
                    }
                    else
#endif
                        Bumpalo_BumpPlayer(player);
                }
                else {
                    if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxCharge)) {
#if MANIA_USE_PLUS
                        if (Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer)) {
                            RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->badnikAnimator, true, 0);
                            self->state      = Bumpalo_State_Bumped;
                            self->velocity.x = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
                            self->velocity.y = -0x20000;
                        }
                        else
#endif
                            Player_Hurt(player, self);
                    }
                }
            }
        }
    }
}

void Bumpalo_HandlePlatformCollisions(EntityPlatform *platform)
{
    RSDK_THIS(Bumpalo);

    if (platform->state != Platform_State_Falling2 && platform->state != Platform_State_Hold) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;

        bool32 collided = false;
        if (platform->collision) {
            if (platform->collision != PLATFORM_C_SOLID) {
                if (platform->collision == PLATFORM_C_TILED
                    && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, self, &Bumpalo->hitboxBadnik)) {
                    if ((self->collisionLayers & Zone->moveLayerMask) != 0) {
                        TileLayer *move  = RSDK.GetTileLayer(Zone->moveLayer);
                        move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                        move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                    }

                    if (self->velocity.y >= 0)
                        self->onGround |= true;
                }
                collided = true;
            }
        }
        else {
            Hitbox *solid = RSDK.GetHitbox(&platform->animator, 0);
            self->onGround |= RSDK.CheckObjectCollisionPlatform(platform, solid, self, &Bumpalo->hitboxBadnik, true);
        }

        if (!collided) {
            Hitbox *platformHitbox = RSDK.GetHitbox(&platform->animator, 1);
            int32 velX             = self->velocity.x;

            switch (RSDK.CheckObjectCollisionBox(platform, platformHitbox, self, &Bumpalo->hitboxBadnik, true)) {
                default:
                case C_NONE:
                case C_BOTTOM: break;

                case C_TOP:
                    if (self->velocity.y > 0)
                        self->onGround |= true;
                    break;

                case C_LEFT:
                    if (velX > 0)
                        self->wallCollided |= true;
                    break;

                case C_RIGHT:
                    if (velX < 0)
                        self->wallCollided |= true;
                    break;
            }
        }

        platform->position.x = platform->centerPos.x;
        platform->position.y = platform->centerPos.y;
    }
}

void Bumpalo_HandleObjectCollisions(void)
{
    RSDK_THIS(Bumpalo);

    foreach_all(Platform, platform) { Bumpalo_HandlePlatformCollisions(platform); }

    foreach_all(Spikes, spikes)
    {
        int32 velX = self->velocity.x;

        switch (RSDK.CheckObjectCollisionBox(platform, &platform->hitbox, self, &Bumpalo->hitboxBadnik, true)) {
            default:
            case C_NONE:
            case C_BOTTOM: break;

            case C_TOP:
                if (self->velocity.y > 0)
                    self->onGround |= true;
                break;

            case C_LEFT:
                if (velX > 0)
                    self->wallCollided |= true;
                break;

            case C_RIGHT:
                if (velX < 0)
                    self->wallCollided |= true;
                break;
        }
    }

    foreach_all(BreakableWall, wall)
    {
        int32 velX = self->velocity.x;

        switch (RSDK.CheckObjectCollisionBox(wall, &wall->hitbox, self, &Bumpalo->hitboxBadnik, true)) {
            default:
            case C_NONE:
            case C_BOTTOM: break;

            case C_TOP:
                if (self->velocity.y > 0)
                    self->onGround |= true;
                break;

            case C_LEFT:
                if (velX > 0)
                    self->wallCollided |= true;
                break;

            case C_RIGHT:
                if (velX < 0)
                    self->wallCollided |= true;
                break;
        }
    }

    if (self->direction)
        self->wallCollided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x140000, 0, true);
    else
        self->wallCollided |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x140000, 0, true);
}

void Bumpalo_State_Init(void)
{
    RSDK_THIS(Bumpalo);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = self->direction == FLIP_NONE ? -0x10000 : 0x10000;

    self->state = Bumpalo_State_Moving;
    Bumpalo_State_Moving();
}

void Bumpalo_State_Moving(void)
{
    RSDK_THIS(Bumpalo);

    self->position.x += self->velocity.x;
    self->onGround     = false;
    self->wallCollided = false;
    Bumpalo_HandleObjectCollisions();

    self->onGround |= RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8);
    if (!self->onGround) {
        if (self->ignoreCliffs) {
            self->state = Bumpalo_State_Falling;
        }
        else {
            if (self->badnikAnimator.animationID == 3)
                self->velocity.x >>= 1;

            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &self->badnikAnimator, true, 0);
            self->timer    = 0;
            self->onGround = self->direction ? RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)
                                             : RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8);
            if (!self->onGround) {
                self->state = Bumpalo_State_Falling;
            }
            else {
                self->position.x -= self->velocity.x;
                self->state = Bumpalo_State_Idle;
            }
        }
    }
    else if (self->badnikAnimator.animationID == 3) {
        if (self->wallCollided) {
            RSDK.PlaySfx(Bumpalo->sfxImpact, false, 0xFF);
            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->badnikAnimator, true, 0);

            self->state      = Bumpalo_State_Bumped;
            self->velocity.x = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
            self->velocity.y = -0x20000;
        }
    }
    else if (self->wallCollided) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &self->badnikAnimator, true, 0);
        self->timer = 0;
        self->state = Bumpalo_State_Idle;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxRange)) {
                self->timer = 92;
                RSDK.PlaySfx(Bumpalo->sfxClack, false, 0xFF);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 2, &self->badnikAnimator, true, 0);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 6, &self->huffAnimator, true, 0);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 7, &self->dustAnimator, true, 0);
                self->state = Bumpalo_State_Charging;
            }
        }
    }

    RSDK.ProcessAnimation(&self->badnikAnimator);

    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOffScreen();
}

void Bumpalo_State_Idle(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->badnikAnimator);

    if (++self->timer == 48) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 5, &self->badnikAnimator, true, 0);
        self->state = Bumpalo_State_Turning;
    }

    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOffScreen();
}

void Bumpalo_State_Charging(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->badnikAnimator);
    RSDK.ProcessAnimation(&self->dustAnimator);

    if (--self->timer == 83) {
        RSDK.PlaySfx(Bumpalo->sfxHuff, false, 0xFF);
    }
    else if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 3, &self->badnikAnimator, true, 0);
        self->state = Bumpalo_State_Moving;
        self->velocity.x *= 2;
    }

    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOffScreen();
}

void Bumpalo_State_Turning(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->badnikAnimator);

    if (self->badnikAnimator.frameID == self->badnikAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &self->badnikAnimator, true, 0);

        self->state = Bumpalo_State_Moving;
        self->direction ^= FLIP_X;
        self->velocity.x = self->direction == FLIP_NONE ? -0x10000 : 0x10000;
        Bumpalo_State_Moving();
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOffScreen();
    }
}

void Bumpalo_State_Bumped(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->badnikAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->onGround     = false;
    self->wallCollided = false;
    Bumpalo_HandleObjectCollisions();
    self->onGround |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, true);

    if (self->velocity.y >= 0 && self->onGround) {
        self->velocity.x = 0;
        self->velocity.y = 0;
    }

    if (self->badnikAnimator.frameID == self->badnikAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &self->badnikAnimator, true, 0);
        if (self->onGround) {
            self->velocity.x = self->direction == FLIP_NONE ? -0x10000 : 0x10000;

            if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
                self->state = Bumpalo_State_Moving;
                Bumpalo_State_Moving();
            }
            else {
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 5, &self->badnikAnimator, true, 0);
                self->state = Bumpalo_State_Turning;
                Bumpalo_State_Turning();
            }
        }
        else {
            self->state = Bumpalo_State_Falling;
            Bumpalo_State_Falling();
        }
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOffScreen();
    }
}

void Bumpalo_State_Falling(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->badnikAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->onGround     = false;
    self->wallCollided = false;
    Bumpalo_HandleObjectCollisions();

    self->onGround |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, true);
    if (self->onGround) {
        self->velocity.y = 0;
        RSDK.PlaySfx(Player->sfxLand, false, 255);
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->badnikAnimator, true, 0);
        self->state = Bumpalo_State_Bumped;
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOffScreen();
    }
}

#if GAME_INCLUDE_EDITOR
void Bumpalo_EditorDraw(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.DrawSprite(&self->badnikAnimator, NULL, false);
}

void Bumpalo_EditorLoad(void)
{
    Bumpalo->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Bumpalo.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Bumpalo, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Bumpalo_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bumpalo, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Bumpalo, VAR_BOOL, ignoreCliffs);
}
