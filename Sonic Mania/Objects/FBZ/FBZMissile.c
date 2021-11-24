#include "SonicMania.h"

ObjectFBZMissile *FBZMissile;

void FBZMissile_Update(void)
{
    RSDK_THIS(FBZMissile);
    StateMachine_Run(self->state);
}

void FBZMissile_LateUpdate(void) {}

void FBZMissile_StaticUpdate(void) {}

void FBZMissile_Draw(void)
{
    RSDK_THIS(FBZMissile);
    StateMachine_Run(self->stateDraw);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void FBZMissile_Create(void *data)
{
    RSDK_THIS(FBZMissile);
    if (self->type != FBZMISSILE_HULL && !self->interval)
        self->interval = -16;

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderLow;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (data)
            self->type = voidToInt(data);

        switch (self->type) {
            case FBZMISSILE_LAUNCHER_V:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 0, &self->animator, true, 0);
                self->direction *= FLIP_Y;
                self->drawOrder = Zone->drawOrderHigh;
                self->state     = FBZMissile_StateLauncher_Delay;
                break;
            case FBZMISSILE_LAUNCHER_H:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 1, &self->animator, true, 0);
                self->state = FBZMissile_State_LauncherHFire;
                break;
            case FBZMISSILE_HULL:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 3, &self->animator, true, 0);
                self->drawOrder = Zone->drawOrderHigh;
                self->timer     = self->interval;
                self->state     = FBZMissile_State_Hull;
                break;
            case FBZMISSILE_VERTICAL:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 2, &self->animator, true, 0);
                self->state = FBZMissile_StateVertical_Rise;
                break;
            case FBZMISSILE_HORIZONTAL:
                RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 1, &self->animator, true, 0);
                self->state = FBZMissile_StateHorizontal_Move;
                break;
            default: break;
        }
    }
}

void FBZMissile_StageLoad(void)
{
    FBZMissile->aniFrames      = RSDK.LoadSpriteAnimation("FBZ/Missile.bin", SCOPE_STAGE);
    FBZMissile->hitbox2.left   = -16;
    FBZMissile->hitbox2.top    = -3;
    FBZMissile->hitbox2.right  = 16;
    FBZMissile->hitbox2.bottom = 3;
    FBZMissile->hitbox1.left   = -5;
    FBZMissile->hitbox1.top    = -8;
    FBZMissile->hitbox1.right  = 5;
    FBZMissile->hitbox1.bottom = 8;
    FBZMissile->hitbox3.left   = -32;
    FBZMissile->hitbox3.top    = -8;
    FBZMissile->hitbox3.right  = 32;
    FBZMissile->hitbox3.bottom = 8;
    FBZMissile->sfxPush        = RSDK.GetSfx("Stage/Push.wav");
    FBZMissile->sfxPush2       = RSDK.GetSfx("Stage/Push.wav");
    FBZMissile->sfxExplosion   = RSDK.GetSfx("Stage/Explosion2.wav");
}

void FBZMissile_StateLauncher_Delay(void)
{
    RSDK_THIS(FBZMissile);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->timer = 42;
        self->state = FBZMissile_StateLauncher_Launch;
    }
    if (!(Zone->timer & 1)) {
        if (self->animator.frameID > 0)
            self->animator.frameID--;
    }
}

void FBZMissile_StateLauncher_Launch(void)
{
    RSDK_THIS(FBZMissile);

    if (!(self->timer & 1)) {
        if (self->animator.frameID < 3)
            self->animator.frameID++;
    }

    switch (--self->timer) {
        case 1: self->state = FBZMissile_StateLauncher_Delay; break;
        case 9:
        case 25:
        case 41: {
            RSDK.PlaySfx(FBZMissile->sfxPush, false, 255);
            EntityFBZMissile *missile = CREATE_ENTITY(FBZMissile, intToVoid(FBZMISSILE_VERTICAL), self->position.x, self->position.y);
            missile->isPermanent      = true;
            missile->velocity.y       = -0x60000;
            missile->drawOrder        = self->drawOrder - 1;
            missile->velocity.x       = FBZMissile->velocities[FBZMissile->velocityID++];
            FBZMissile->velocityID &= 7;
            break;
        }
        default: break;
    }
}

void FBZMissile_State_LauncherHFire(void)
{
    RSDK_THIS(FBZMissile);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        RSDK.PlaySfx(FBZMissile->sfxPush2, false, 255);
        EntityFBZMissile *missile = CREATE_ENTITY(FBZMissile, intToVoid(FBZMISSILE_HORIZONTAL), self->position.x, self->position.y);
        missile->drawOrder        = self->drawOrder;
        missile->direction        = self->direction;
        if (self->direction) {
            missile->velocity.x = 0x40000;
            self->position.x -= 500000;
        }
        else {
            missile->velocity.x = -0x40000;
            self->position.x += 500000;
        }
        self->timer = 62;
        self->state = FBZMissile_State_LauncherHPrepare;
    }
}

void FBZMissile_State_LauncherHPrepare(void)
{
    RSDK_THIS(FBZMissile);
    if (--self->timer > 0) {
        if (self->direction)
            self->position.x += 0x2000;
        else
            self->position.x -= 0x2000;
    }
    else {
        self->state = FBZMissile_State_LauncherHFire;
    }
}

void FBZMissile_StateVertical_Rise(void)
{
    RSDK_THIS(FBZMissile);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;
    if (self->velocity.y >= 0) {
        self->drawOrder = Zone->drawOrderLow;
        self->state     = FBZMissile_StateVertical_Fall;
    }
    if (self->velocity.y > -0x1D000)
        self->position.x += self->velocity.x;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void FBZMissile_StateVertical_Fall(void)
{
    RSDK_THIS(FBZMissile);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1000;
    if (!self->timer) {
        if (self->animator.frameID < 5)
            self->animator.frameID++;
    }
    self->timer = (self->timer - 1) & 1;
    if (self->velocity.y < 0x1D000)
        self->position.x += self->velocity.x;

    if (RSDK.CheckOnScreen(self, NULL)) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &FBZMissile->hitbox1)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);

                CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
                RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                destroyEntity(self);

                foreach_break;
            }
        }

        if (!RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, false)) {
            foreach_active(FBZMissile, missile)
            {
                if (missile->type == FBZMISSILE_HULL && RSDK.CheckObjectCollisionTouchBox(missile, &FBZMissile->hitbox3, self, &FBZMissile->hitbox1)) {
                    if (--missile->timer <= 0) {
                        destroyEntity(missile);
                        RSDK.PlaySfx(Player->sfxRelease, false, 255);
                    }
                    CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
                    RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
                    destroyEntity(self);
                }
            }
        }
        else {
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSSPUFF), self->position.x, self->position.y + 0x30000)->drawOrder = Zone->drawOrderHigh;
            RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
            destroyEntity(self);
        }
    }
    else {
        destroyEntity(self);
    }
}

void FBZMissile_StateHorizontal_Move(void)
{
    RSDK_THIS(FBZMissile);
    self->position.x += self->velocity.x;
    if (RSDK.CheckOnScreen(self, 0)) {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &FBZMissile->hitbox2)) {
#if RETRO_USE_PLUS
                if (!Player_CheckMightyUnspin(0x300, player, 2, &player->uncurlTimer))
#endif
                    Player_CheckHit(player, self);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void FBZMissile_State_Hull(void)
{
    RSDK_THIS(FBZMissile);
    self->position.x += self->velocity.x;
    RSDK.ProcessAnimation(&self->animator);

#if RETRO_USE_PLUS
    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, self, &FBZMissile->hitbox3) && player->state == Player_State_MightyHammerDrop && !player->sidekick) {
            RSDK.PlaySfx(FBZMissile->sfxExplosion, false, 255);
            RSDK.PlaySfx(Player->sfxRelease, false, 255);
            player->velocity.y = velY - 0x10000;
            player->onGround   = false;
            destroyEntity(self);
            foreach_break;
        }
    }
#endif
}

#if RETRO_INCLUDE_EDITOR
void FBZMissile_EditorDraw(void)
{
    RSDK_THIS(FBZMissile);

    int dir = self->direction;
    switch (self->type) {
        case FBZMISSILE_LAUNCHER_V:
            RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 0, &self->animator, true, 0);
            self->direction *= FLIP_Y;
            self->drawOrder = Zone->drawOrderHigh;
            break;
        case FBZMISSILE_LAUNCHER_H:
            RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 1, &self->animator, true, 0);
            break;
        case FBZMISSILE_HULL:
            RSDK.SetSpriteAnimation(FBZMissile->aniFrames, 3, &self->animator, true, 0);
            self->drawOrder = Zone->drawOrderHigh;
            break;
        default: break;
    }

    FBZMissile_Draw();

    self->direction = dir;
}

void FBZMissile_EditorLoad(void)
{
    FBZMissile->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Missile.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(FBZMissile, type);
    RSDK_ENUM_VAR("Launcher (Vertical)", FBZMISSILE_LAUNCHER_V);
    RSDK_ENUM_VAR("Launcher (Horizontal)", FBZMISSILE_LAUNCHER_H);
    RSDK_ENUM_VAR("Hull", FBZMISSILE_HULL);

    RSDK_ACTIVE_VAR(FBZMissile, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip", FLIP_X);
}
#endif

void FBZMissile_Serialize(void)
{
    RSDK_EDITABLE_VAR(FBZMissile, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(FBZMissile, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(FBZMissile, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(FBZMissile, VAR_UINT8, direction);
}
