#include "SonicMania.h"

ObjectBumpalo *Bumpalo;

void Bumpalo_Update(void)
{
    RSDK_THIS(Bumpalo);
    RSDK.ProcessAnimation(&self->animator2);
    StateMachine_Run(self->state);
}

void Bumpalo_LateUpdate(void) {}

void Bumpalo_StaticUpdate(void) {}

void Bumpalo_Draw(void)
{
    RSDK_THIS(Bumpalo);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    if (self->state == Bumpalo_State_Unknown3)
        RSDK.DrawSprite(&self->animator3, NULL, false);
}

void Bumpalo_Create(void *data)
{
    RSDK_THIS(Bumpalo);
    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow + 1;
    self->drawFX        = FX_FLIP;
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &self->animator1, true, 0);
    self->state = Bumpalo_State_Setup;
}

void Bumpalo_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        Bumpalo->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Bumpalo.bin", SCOPE_STAGE);
    Bumpalo->hitboxBadnik.left   = -20;
    Bumpalo->hitboxBadnik.top    = 0;
    Bumpalo->hitboxBadnik.right  = 20;
    Bumpalo->hitboxBadnik.bottom = 16;

    Bumpalo->hitboxBumper.left   = -10;
    Bumpalo->hitboxBumper.top    = -16;
    Bumpalo->hitboxBumper.right  = 12;
    Bumpalo->hitboxBumper.bottom = 0;

    Bumpalo->hitboxClack.left   = -128;
    Bumpalo->hitboxClack.top    = -64;
    Bumpalo->hitboxClack.right  = -16;
    Bumpalo->hitboxClack.bottom = 96;

    Bumpalo->hitboxCharge.left   = -17;
    Bumpalo->hitboxCharge.top    = -14;
    Bumpalo->hitboxCharge.right  = 10;
    Bumpalo->hitboxCharge.bottom = 0;

    Bumpalo->hitbox5.left   = -24;
    Bumpalo->hitbox5.top    = -10;
    Bumpalo->hitbox5.right  = -8;
    Bumpalo->hitbox5.bottom = 0;

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

void Bumpalo_CheckOnScreen(void)
{
    RSDK_THIS(Bumpalo);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Bumpalo_Create(NULL);
    }
}

void Bumpalo_BumpPlayer(void *p)
{
    RSDK_THIS(Bumpalo);
    EntityPlayer *player = (EntityPlayer *)p;

    RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
    int angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
    int velX  = 0x700 * RSDK.Cos256(angle);
    int velY  = 0x700 * RSDK.Sin256(angle);

    if (player->state == Player_State_FlyCarried)
        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;

    int anim = player->animator.animationID;
    if (anim != ANI_FLY && anim != ANI_FLYLIFTTIRED) {
        if (player->state != Player_State_TailsFlight) {
            if (player->state != Player_State_DropDash)
                player->state = Player_State_Air;
            if (anim != ANI_JUMP && anim != ANI_JOG && anim != ANI_RUN && anim != ANI_DASH)
                player->animator.animationID = ANI_WALK;
        }
    }
    if (player->animator.animationID != ANI_FLY) {
        player->velocity.x  = velX;
        player->groundVel   = velX;
        player->jumpAbility = 0;
    }
    player->velocity.y     = velY;
    player->onGround       = false;
    player->tileCollisions = true;
    if (self->animator1.animationID == 3)
        self->velocity.x >>= 1;
    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->animator1, true, 0);

    if (self->state != Bumpalo_State_Unknown6 && self->state != Bumpalo_State_Unknown5)
        self->velocity.x = 0;
    self->state = Bumpalo_State_Unknown5;
}

void Bumpalo_CheckPlayerCollisions(void)
{
    RSDK_THIS(Bumpalo);

    foreach_active(Player, player)
    {
        if (self->animator1.animationID == 3) {
            if (abs(self->velocity.x) <= 0x10000) {
                if (!Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                    if (!Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBumper)) {
                        if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxCharge)) {
#if RETRO_USE_PLUS
                            if (Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer)) {
                                RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->animator1, true, 0);
                                self->state      = Bumpalo_State_Unknown5;
                                self->velocity.y = -0x20000;
                                if (self->direction == FLIP_NONE)
                                    self->velocity.x = 0x10000;
                                else
                                    self->velocity.x = -0x10000;
                            }
                            else
#endif
                                Player_CheckHit(player, self);
                        }
                    }
                    else {
#if RETRO_USE_PLUS
                        if (player->state == Player_State_MightyHammerDrop) {
                            RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                            Player_CheckBadnikBreak(self, player, true);
                        }
                        else
#endif
                            Bumpalo_BumpPlayer(player);
                    }
                }
                else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                    Player_CheckBadnikBreak(self, player, true);
                }
            }
            else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBumper)) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                    Player_CheckBadnikBreak(self, player, true);
                }
                else
#endif
                    Bumpalo_BumpPlayer(player);
            }
            else if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxCharge)) {
#if RETRO_USE_PLUS
                if (Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
                    RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                    RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->animator1, true, 0);
                    self->state      = Bumpalo_State_Unknown5;
                    self->velocity.y = -0x20000;
                    if (self->direction == FLIP_NONE)
                        self->velocity.x = 0x10000;
                    else
                        self->velocity.x = -0x10000;
                }
                else
#endif
                    Player_CheckHit(player, self);
            }
            else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                Player_CheckBadnikBreak(self, player, true);
            }
        }
        else {
            if (!Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                if (!Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBumper)) {
                    if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxCharge)) {
#if RETRO_USE_PLUS
                        if (Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer)) {
                            RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
                            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->animator1, true, 0);
                            self->state      = Bumpalo_State_Unknown5;
                            self->velocity.y = -0x20000;
                            if (self->direction == FLIP_NONE)
                                self->velocity.x = 0x10000;
                            else
                                self->velocity.x = -0x10000;
                        }
                        else
#endif
                            Player_CheckHit(player, self);
                    }
                }
                else {
#if RETRO_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop) {
                        RSDK.PlaySfx(Bumpalo->sfxBumper, false, 255);
                        Player_CheckBadnikBreak(self, player, true);
                    }
                    else
#endif
                        Bumpalo_BumpPlayer(player);
                }
            }
            else if (Player_CheckBadnikTouch(player, self, &Bumpalo->hitboxBadnik)) {
                Player_CheckBadnikBreak(self, player, true);
            }
        }
    }
}

void Bumpalo_HandlePlatformCollisions(void *p)
{
    RSDK_THIS(Bumpalo);
    EntityPlatform *platform = (EntityPlatform *)p;

    if (platform->state != Platform_State_Collapse_Falling && platform->state != Platform_State_Collapse_CheckReset) {
        platform->position.x = platform->drawPos.x - platform->collisionOffset.x;
        platform->position.y = platform->drawPos.y - platform->collisionOffset.y;

        bool32 flag = false;
        if (platform->collision) {
            if (platform->collision != PLATFORM_C_SOLID_ALL) {
                if (platform->collision == PLATFORM_C_USE_TILES
                    && RSDK.CheckObjectCollisionTouchBox(platform, &platform->hitbox, self, &Bumpalo->hitboxBadnik)) {
                    if ((self->collisionLayers & Zone->moveID) != 0) {
                        TileLayer *move  = RSDK.GetSceneLayer(Zone->moveLayer);
                        move->position.x = -(platform->drawPos.x + platform->tileOrigin.x) >> 16;
                        move->position.y = -(platform->drawPos.y + platform->tileOrigin.y) >> 16;
                    }
                    if (self->velocity.y >= 0)
                        self->onGround |= true;
                }
                flag = true;
            }
        }
        else {
            Hitbox *solid = RSDK.GetHitbox(&platform->animator, 0);
            self->onGround |= RSDK.CheckObjectCollisionPlatform(platform, solid, self, &Bumpalo->hitboxBadnik, true);
        }

        if (!flag) {
            Hitbox *platformHitbox = RSDK.GetHitbox(&platform->animator, 1);
            int velX               = self->velocity.x;
            int side               = RSDK.CheckObjectCollisionBox(platform, platformHitbox, self, &Bumpalo->hitboxBadnik, true);

            switch (side) {
                default: break;
                case C_TOP:
                    if (self->velocity.y > 0)
                        self->onGround |= true;
                    break;
                case C_LEFT:
                    if (velX > 0)
                        self->flag |= 1;
                    break;
                case C_RIGHT:
                    if (velX < 0)
                        self->flag |= 1;
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
        int velX = self->velocity.x;
        int side = RSDK.CheckObjectCollisionBox(platform, &platform->hitbox, self, &Bumpalo->hitboxBadnik, true);

        switch (side) {
            default: break;
            case C_TOP:
                if (self->velocity.y > 0)
                    self->onGround |= true;
                break;
            case C_LEFT:
                if (velX > 0)
                    self->flag |= 1;
                break;
            case C_RIGHT:
                if (velX < 0)
                    self->flag |= 1;
                break;
        }
    }

    foreach_all(BreakableWall, wall)
    {
        int velX = self->velocity.x;
        int side = RSDK.CheckObjectCollisionBox(wall, &wall->hitbox, self, &Bumpalo->hitboxBadnik, true);

        switch (side) {
            default: break;
            case C_TOP:
                if (self->velocity.y > 0)
                    self->onGround |= true;
                break;
            case C_LEFT:
                if (velX > 0)
                    self->flag |= 1;
                break;
            case C_RIGHT:
                if (velX < 0)
                    self->flag |= 1;
                break;
        }
    }

    if (self->direction)
        self->flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, 0x140000, 0, true);
    else
        self->flag |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, -0x140000, 0, true);
}

void Bumpalo_State_Setup(void)
{
    RSDK_THIS(Bumpalo);

    self->active = ACTIVE_NORMAL;
    if (self->direction == FLIP_NONE)
        self->velocity.x = -0x10000;
    else
        self->velocity.x = 0x10000;
    self->state = Bumpalo_State_Unknown1;
    Bumpalo_State_Unknown1();
}

void Bumpalo_State_Unknown1(void)
{
    RSDK_THIS(Bumpalo);

    self->position.x += self->velocity.x;
    self->onGround = false;
    self->flag     = false;
    Bumpalo_HandleObjectCollisions();

    self->onGround |= RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8);
    if (!self->onGround) {
        if (self->ignoreCliffs) {
            self->state = Bumpalo_State_Unknown6;
        }
        else {
            if (self->animator1.animationID == 3)
                self->velocity.x >>= 1;
            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &self->animator1, true, 0);
            self->timer   = 0;
            bool32 onGround = self->direction ? RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)
                                                : RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8);
            self->onGround = onGround;
            if (!self->onGround) {
                self->state = Bumpalo_State_Unknown6;
            }
            else {
                self->position.x -= self->velocity.x;
                self->state = Bumpalo_State_Unknown2;
            }
        }
    }
    else if (self->animator1.animationID == 3) {
        if (self->flag) {
            RSDK.PlaySfx(Bumpalo->sfxImpact, false, 255);
            RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->animator1, true, 0);
            self->state      = Bumpalo_State_Unknown5;
            self->velocity.y = -0x20000;
            if (self->direction == FLIP_NONE)
                self->velocity.x = 0x10000;
            else
                self->velocity.x = -0x10000;
        }
    }
    else if (self->flag) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 0, &self->animator1, true, 0);
        self->timer = 0;
        self->state = Bumpalo_State_Unknown2;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Bumpalo->hitboxClack)) {
                self->timer = 92;
                RSDK.PlaySfx(Bumpalo->sfxClack, false, 255);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 2, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 6, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 7, &self->animator3, true, 0);
                self->state = Bumpalo_State_Unknown3;
            }
        }
    }
    RSDK.ProcessAnimation(&self->animator1);
    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOnScreen();
}

void Bumpalo_State_Unknown2(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->animator1);
    if (++self->timer == 48) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 5, &self->animator1, true, 0);
        self->state = Bumpalo_State_Unknown4;
    }
    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOnScreen();
}

void Bumpalo_State_Unknown3(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator3);

    self->timer--;
    if (self->timer == 83) {
        RSDK.PlaySfx(Bumpalo->sfxHuff, false, 255);
    }
    else if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 3, &self->animator1, true, 0);
        self->state = Bumpalo_State_Unknown1;
        self->velocity.x *= 2;
    }
    Bumpalo_CheckPlayerCollisions();
    Bumpalo_CheckOnScreen();
}

void Bumpalo_State_Unknown4(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &self->animator1, true, 0);
        self->state = Bumpalo_State_Unknown1;
        self->direction ^= 1;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x10000;
        else
            self->velocity.x = 0x10000;
        Bumpalo_State_Unknown1();
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOnScreen();
    }
}

void Bumpalo_State_Unknown5(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.x;
    self->velocity.y += 0x3800;
    self->onGround = false;
    self->flag     = false;
    Bumpalo_HandleObjectCollisions();
    self->onGround |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, true);

    if (self->velocity.y >= 0 && self->onGround) {
        self->velocity.x = 0;
        self->velocity.y = 0;
    }
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 1, &self->animator1, true, 0);
        if (self->onGround) {
            if (self->direction == FLIP_NONE)
                self->velocity.x = -0x10000;
            else
                self->velocity.x = 0x10000;
            if (RSDK.ObjectTileGrip(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
                self->state = Bumpalo_State_Unknown1;
                Bumpalo_State_Unknown1();
            }
            else {
                RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 5, &self->animator1, true, 0);
                self->state = Bumpalo_State_Unknown4;
                Bumpalo_State_Unknown4();
            }
        }
        else {
            self->state = Bumpalo_State_Unknown6;
            Bumpalo_State_Unknown6();
        }
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOnScreen();
    }
}

void Bumpalo_State_Unknown6(void)
{
    RSDK_THIS(Bumpalo);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->onGround = false;
    self->flag     = false;
    Bumpalo_HandleObjectCollisions();
    self->onGround |= RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xF0000, true);
    if (self->onGround == true) {
        self->velocity.y = 0;
        RSDK.PlaySfx(Player->sfxLand, false, 255);
        RSDK.SetSpriteAnimation(Bumpalo->aniFrames, 4, &self->animator1, true, 0);
        self->state = Bumpalo_State_Unknown5;
    }
    else {
        Bumpalo_CheckPlayerCollisions();
        Bumpalo_CheckOnScreen();
    }
}

void Bumpalo_EditorDraw(void)
{
    RSDK_THIS(Bumpalo);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Bumpalo_EditorLoad(void) { Bumpalo->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Bumpalo.bin", SCOPE_STAGE); }

void Bumpalo_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bumpalo, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Bumpalo, VAR_BOOL, ignoreCliffs);
}
