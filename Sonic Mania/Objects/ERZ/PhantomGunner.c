// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomGunner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPhantomGunner *PhantomGunner;

void PhantomGunner_Update(void)
{
    RSDK_THIS(PhantomGunner);
    StateMachine_Run(self->state);
}

void PhantomGunner_LateUpdate(void) {}

void PhantomGunner_StaticUpdate(void) {}

void PhantomGunner_Draw(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.SetActivePalette(4, 0, ScreenInfo[SceneInfo->currentScreenID].height);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }

    RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].height);
}

void PhantomGunner_Create(void *data)
{
    RSDK_THIS(PhantomGunner);

    if (!SceneInfo->inEditor) {
        self->startPos      = self->position;
        self->visible       = true;
        self->drawOrder     = 2;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->type = voidToInt(data);
        switch (self->type) {
            case PHANTOMGUNNER_BOSS:
                self->drawFX = FX_FLIP;
                self->active = ACTIVE_NEVER;
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &self->mainAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 1, &self->fxAnimator, true, 0);
                self->originPos   = self->position;
                self->screenPos.x = (self->position.x >> 16) - ScreenInfo->centerX;
                self->screenPos.y = (self->position.y >> 16) - ScreenInfo->centerY;
                self->stateDraw   = PhantomGunner_Draw_Gunner;
                self->state       = PhantomGunner_State_Idle;
                break;

            case PHANTOMGUNNER_LAUNCHROCKET:
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 10, &self->mainAnimator, true, 0);
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 11, &self->fxAnimator, true, 0);
                self->stateDraw = PhantomGunner_Draw_RocketLaunch;
                self->state     = PhantomGunner_State_LaunchedRocket;
                break;

            case PHANTOMGUNNER_NAPALM_EXPLOSION:
                self->originPos = self->position;
                self->state     = PhantomGunner_State_NapalmExplosion;
                break;

            case PHANTOMGUNNER_MORTAR_EXPLOSION:
                self->originPos = self->position;
                self->state     = PhantomGunner_State_MortarExplosion;
                break;

            default: break;
        }
    }
}

void PhantomGunner_StageLoad(void)
{
    PhantomGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE);

    PhantomGunner->hitboxNapalm.left   = -8;
    PhantomGunner->hitboxNapalm.top    = 4;
    PhantomGunner->hitboxNapalm.right  = 8;
    PhantomGunner->hitboxNapalm.bottom = 26;

    PhantomGunner->hitboxMortar.left   = -16;
    PhantomGunner->hitboxMortar.top    = -32;
    PhantomGunner->hitboxMortar.right  = 16;
    PhantomGunner->hitboxMortar.bottom = -16;

    PhantomGunner->hitboxDud.left   = -8;
    PhantomGunner->hitboxDud.top    = -8;
    PhantomGunner->hitboxDud.right  = 8;
    PhantomGunner->hitboxDud.bottom = 8;

    PhantomGunner->sfxCannonFire = RSDK.GetSfx("Stage/CannonFire.wav");
}

void PhantomGunner_HandleDudExhaust(void)
{
    RSDK_THIS(PhantomGunner);
    if (!(Zone->timer & 7)) {
        int32 x              = RSDK.Sin512(self->rotation) << 11;
        int32 y              = RSDK.Cos512(self->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + x, self->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(self->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(self->rotation) << 4;
        debris->drawOrder  = Zone->objectDrawLow;
        debris->timer      = 41;
    }
}

void PhantomGunner_HandleMalfunctionDudExhaust(void)
{
    RSDK_THIS(PhantomGunner);

    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int32 x              = 0x600 * RSDK.Sin512(self->rotation);
            int32 y              = 0x600 * RSDK.Cos512(self->rotation);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + x, self->position.y - y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->drawOrder = Zone->objectDrawHigh;
            debris->drawFX    = FX_SCALE;
            debris->timer     = 52;
            debris->scale.x   = (self->scale.x * RSDK.Rand(128, 384)) >> 9;
            debris->scale.y   = debris->scale.x;
        }
    }
    else {
        int32 x              = RSDK.Sin512(self->rotation) << 11;
        int32 y              = RSDK.Cos512(self->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + x, self->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(self->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(self->rotation) << 4;
        debris->drawOrder  = Zone->objectDrawLow;
        debris->drawFX     = FX_SCALE;
        debris->timer      = 41;
        debris->scale.y    = self->scale.x;
    }
}

void PhantomGunner_SpawnDust(void)
{
    RSDK_THIS(PhantomGunner);

    for (int32 i = 0; i < 4; ++i) {
        int32 x          = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int32 y          = self->position.y + RSDK.Rand(-0x280000, -0x180000);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);
        dust->state      = Dust_State_Move;
        dust->drawOrder  = Zone->objectDrawHigh;
    }
}

void PhantomGunner_HandleRotations(int angle)
{
    RSDK_THIS(PhantomGunner);

    int32 ang = angle - self->rotation;
    if (abs(angle - self->rotation) >= abs(angle - self->rotation - 0x200)) {
        if (abs(angle - self->rotation - 0x200) < abs(angle - self->rotation + 0x200)) {
            self->rotation += ((ang - 0x200) >> 4);
            self->rotation &= 0x1FF;
        }
        else {
            self->rotation += ((ang + 0x200) >> 4);
            self->rotation &= 0x1FF;
        }
    }
    else if (abs(angle - self->rotation) < abs(angle - self->rotation + 0x200)) {
        self->rotation += (ang >> 4);
        self->rotation &= 0x1FF;
    }
    else {
        self->rotation += ((ang + 0x200) >> 4);
        self->rotation &= 0x1FF;
    }
}

void PhantomGunner_CheckPlayerMissileCollisions(void)
{
    RSDK_THIS(PhantomGunner);

    foreach_active(Player, player)
    {
        if (self->parachuteAnimator.frameID > 0 && player->velocity.y >= 0 && Player_CheckBadnikTouch(player, self, &PhantomGunner->hitboxMortar)
            && player->animator.animationID != ANI_HURT) {
            RSDK.SetSpriteAnimation(-1, 0, &self->parachuteAnimator, true, 0);
            player->velocity.y = -0x60000;
            PhantomGunner_SpawnDust();
            self->state = PhantomGunner_State_Napalm;
        }
        else {
            if (Player_CheckBadnikTouch(player, self, &PhantomGunner->hitboxNapalm)) {
                int32 anim = player->animator.animationID;
                if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y);
                    explosion->interaction     = false;
                    explosion->drawOrder       = Zone->objectDrawHigh;
                    if (self->parachuteAnimator.animationID == 12 && self->parachuteAnimator.frameID > 0)
                        PhantomGunner_SpawnDust();
                    RSDK.PlaySfx(PhantomEgg->sfxExplosion2, false, 255);
                    destroyEntity(self);
                    foreach_break;
                }
            }
        }
    }
}

void PhantomGunner_CheckPlayerExplosionCollisions(void)
{
    foreach_active(Player, player)
    {
        foreach_active(Explosion, explosion)
        {
            if (explosion->animator.frameID <= 6) {
                if (Player_CheckCollisionTouch(player, explosion, &PhantomGunner->hitboxDud)) {
                    Player_CheckElementalHit(player, explosion, SHIELD_FIRE);
                }
            }
        }
    }
}

void PhantomGunner_Draw_Gunner(void)
{
    RSDK_THIS(PhantomGunner);
    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(6, 128, 4, 128, 128);
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);

        RSDK.CopyPalette(5, 128, 4, 128, 128);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);
    }
}

void PhantomGunner_Draw_RocketLaunch(void)
{
    RSDK_THIS(PhantomGunner);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ((self->originPos.y + parent->position.y) >> 16) - ScreenInfo->position.y);

    Vector2 drawPos;
    drawPos.x = parent->position.x + self->originPos.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    drawPos.y = parent->position.y + self->originPos.y;
    RSDK.DrawSprite(&self->fxAnimator, &drawPos, false);
}

void PhantomGunner_Draw_Rocket(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->tailAnimator, NULL, false);
    RSDK.DrawSprite(&self->parachuteAnimator, NULL, false);
}

void PhantomGunner_State_ResetState(void)
{
    RSDK_THIS(PhantomGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += (ScreenInfo->position.x - self->screenPos.x) << 15;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    self->screenPos = ScreenInfo->position;

    RSDK.ProcessAnimation(&self->fxAnimator);
    if (++self->timer == 60) {
        self->position          = self->startPos;
        self->originPos         = self->startPos;
        self->timer             = 0;
        self->rocketLaunchCount = 0;
        self->screenPos.x       = (self->position.x >> 16) - ScreenInfo->centerX;
        self->screenPos.y       = (self->position.y >> 16) - ScreenInfo->centerY;
        self->stateDraw         = PhantomGunner_Draw_Gunner;
        self->state             = PhantomGunner_State_Idle;
        self->active            = ACTIVE_NEVER;
    }
}

void PhantomGunner_State_Idle(void)
{
    RSDK_THIS(PhantomGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += (ScreenInfo->position.x - self->screenPos.x) << 15;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    self->screenPos = ScreenInfo->position;
    RSDK.ProcessAnimation(&self->fxAnimator);

    self->fireAnimTimer = 8;
    if (++self->timer < 120 || RSDK.GetEntityCount(PhantomGunner->objectID, true) >= 2) {
        PhantomGunner_CheckPlayerExplosionCollisions();
    }
    else {
        if (++self->rocketLaunchCount == 2) {
            PhantomEgg_SetupScanlineCB();
            self->timer = 0;
            self->state = PhantomGunner_State_ResetState;
        }
        else {
            PhantomGunner->launchedRocketID = 0;
            self->timer                     = 0;
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 2, &self->mainAnimator, true, 0);
            self->state = PhantomGunner_State_LaunchRockets;
            PhantomGunner_CheckPlayerExplosionCollisions();
        }
    }
}

void PhantomGunner_State_LaunchRockets(void)
{
    RSDK_THIS(PhantomGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += (ScreenInfo->position.x - self->screenPos.x) << 15;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    self->screenPos = ScreenInfo->position;
    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);

    if (++self->timer == 8) {
        EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(PHANTOMGUNNER_LAUNCHROCKET), self->position.x, self->position.y);
        child->parent              = (Entity *)self;
        child->originPos.x         = PhantomGunner->rocketOffsets[self->rocketOffsetID] << 16;
        child->originPos.y         = PhantomGunner->rocketOffsets[self->rocketOffsetID + 1] << 16;
        child->position.x += child->originPos.x;
        child->position.y += child->originPos.y + 0x100000;
        self->rocketOffsetID = (self->rocketOffsetID - 14) & 0x1F;
        RSDK.PlaySfx(PhantomGunner->sfxCannonFire, false, 255);
    }

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        self->timer = 0;
        if (--self->fireAnimTimer > 0) {
            if (!(self->fireAnimTimer & 1))
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 2, &self->mainAnimator, true, 0);
            else
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 3, &self->mainAnimator, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &self->mainAnimator, true, 0);
            self->state = PhantomGunner_State_Idle;
        }
    }
    PhantomGunner_CheckPlayerExplosionCollisions();
}

void PhantomGunner_State_LaunchedRocket(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);
    self->position.y -= 0x80000;
    if (++self->timer == 60) {
        EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

        self->timer = 0;
        if ((PhantomGunner->launchedRocketID & 3) == 3)
            self->type = PHANTOMGUNNER_DUD;
        else
            self->type = (PhantomGunner->launchedRocketID & 1) + PHANTOMGUNNER_MORTAR;
        ++PhantomGunner->launchedRocketID;
        self->drawOrder  = Zone->objectDrawLow;
        self->position.y = (ScreenInfo->position.y - 64) << 16;

        bool32 canFire = false;
        while (!canFire) {
            canFire          = true;
            self->position.x = parent->position.x + RSDK.Rand(-0x1000000, 0x1000000);

            foreach_active(PhantomGunner, gunner)
            {
                if (gunner != self && gunner->type >= PHANTOMGUNNER_MORTAR) {
                    int32 dist = abs(gunner->position.x - self->position.x);
                    if (dist < 0x180000 && gunner->position.y - self->position.y < 0x800000)
                        canFire = false;
                }
            }
        }

        if (self->type == PHANTOMGUNNER_MORTAR) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 4, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 5, &self->tailAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 12, &self->parachuteAnimator, true, 0);
            self->state     = PhantomGunner_State_Mortar;
            self->timer     = RSDK.Rand(0, 2) << 8;
            self->stateDraw = PhantomGunner_Draw_Rocket;
        }
        else if (self->type == PHANTOMGUNNER_NAPALM) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 6, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 7, &self->tailAnimator, true, 0);
            self->state     = PhantomGunner_State_Napalm;
            self->stateDraw = PhantomGunner_Draw_Rocket;
        }
        else if (self->type == PHANTOMGUNNER_DUD) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 8, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 9, &self->tailAnimator, true, 0);
            self->position.x = parent->position.x;
            self->drawFX     = FX_ROTATE;
            self->state      = PhantomGunner_State_Dud_Active;
            if (!(Zone->timer & 1))
                self->velocity.x = 0x40000;
            else
                self->velocity.x = -0x40000;
            self->stateDraw = PhantomGunner_Draw_Rocket;
        }
    }
}

void PhantomGunner_State_Mortar(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->parachuteAnimator);
    RSDK.ProcessAnimation(&self->tailAnimator);

    if (self->parachuteAnimator.frameID) {
        self->drawFX = FX_ROTATE;
        if (self->velocity.y > 0xC000)
            self->velocity.y -= 0x3800;
        self->rotation = RSDK.Sin512(self->timer) >> 6;
        self->timer    = (self->timer + 4) & 0x1FF;
    }
    else if (self->velocity.y < 0x40000)
        self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    PhantomGunner_CheckPlayerMissileCollisions();
    if (self->objectID) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == PHANTOMGUNNER_NAPALM) {
                EntityPhantomGunner *child =
                    CREATE_ENTITY(PhantomGunner, intToVoid(PHANTOMGUNNER_NAPALM_EXPLOSION), self->position.x, self->position.y);
                if (self->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(PhantomGunner, intToVoid(PHANTOMGUNNER_MORTAR_EXPLOSION), self->position.x, self->position.y);
                destroyEntity(self);
            }
        }
    }
}

void PhantomGunner_State_Napalm(void)
{
    RSDK_THIS(PhantomGunner);
    RSDK.ProcessAnimation(&self->tailAnimator);

    if (self->velocity.y < 0x40000)
        self->velocity.y += 0x1800;
    self->position.y += self->velocity.y;

    PhantomGunner_CheckPlayerMissileCollisions();
    if (self->objectID) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == PHANTOMGUNNER_NAPALM) {
                EntityPhantomGunner *child =
                    CREATE_ENTITY(PhantomGunner, intToVoid(PHANTOMGUNNER_NAPALM_EXPLOSION), self->position.x, self->position.y);
                if (self->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(PhantomGunner, intToVoid(PHANTOMGUNNER_MORTAR_EXPLOSION), self->position.x, self->position.y);
                destroyEntity(self);
            }
        }
    }
}

void PhantomGunner_State_Dud_Active(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

    ++self->timer;

    self->velocity.x = RSDK.Cos256(self->timer) << 11;
    if (self->position.y < parent->position.y - 0x800000) {
        if (self->velocity.y < 0x20000)
            self->velocity.y += 0x2000;
    }
    if (self->position.y > parent->position.y - 0x200000) {
        if (self->velocity.y > -0x20000)
            self->velocity.y -= 0x2000;
    }

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    PhantomGunner_HandleDudExhaust();

    int32 angle = RSDK.ATan2(self->velocity.y, -self->velocity.x);
    PhantomGunner_HandleRotations(2 * angle);
    if (self->timer == 320) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y);
        explosion->interaction     = false;
        explosion->drawOrder       = Zone->objectDrawHigh;
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &PhantomGunner->hitboxDud)) {
                self->angle     = self->rotation << 8;
                self->timer     = 0;
                self->drawFX    = FX_SCALE | FX_ROTATE;
                self->groundVel = 0x4000;
                self->scale.x   = 0x200;
                self->scale.y   = 0x200;
                self->state     = PhantomGunner_State_Dud_HitByPlayer;
                RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
            }
        }
    }
}

void PhantomGunner_State_Dud_HitByPlayer(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);

    self->angle += self->groundVel;
    self->rotation = self->angle >> 8;
    self->groundVel -= (self->groundVel >> 3);

    PhantomGunner_HandleDudExhaust();
    if (self->groundVel < 128) {
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);
        self->state = PhantomGunner_State_Dud_Malfunction;
    }
}

void PhantomGunner_State_Dud_Malfunction(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

    int32 angle = RSDK.ATan2((parent->position.y - self->position.y) >> 16, -((parent->position.x - self->position.x) >> 16));
    PhantomGunner_HandleRotations(2 * angle);
    self->rotation &= 0x1FF;
    self->position.x -= RSDK.Sin512(self->rotation) << 9;
    self->scale.x -= 6;
    self->position.y += RSDK.Cos512(self->rotation) << 9;
    self->scale.y = self->scale.x;

    PhantomGunner_HandleMalfunctionDudExhaust();
    if (self->scale.x < 128) {
        RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
        parent->invincibilityTimer = 48;
        self->position.x           = parent->position.x;
        self->position.y           = parent->position.y;
        self->visible              = false;
        self->state                = PhantomGunner_State_Dud_Explode;
    }
}

void PhantomGunner_State_Dud_Explode(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer % 3) && !(Zone->timer & 4)) {
        int32 x                    = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int32 y                    = self->position.y + RSDK.Rand(-0x100000, 0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
        explosion->interaction     = false;
        explosion->drawOrder       = 1;
        explosion->scale           = self->scale;
    }

    if (++self->timer == 16)
        destroyEntity(self);
}

void PhantomGunner_State_NapalmExplosion(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer & 3)) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->originPos.x - self->napalmExplosionPos, self->position.y);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->originPos.x + self->napalmExplosionPos, self->position.y);
    }

    self->napalmExplosionPos += self->velocity.x;
    if (++self->timer == 16)
        destroyEntity(self);
}

void PhantomGunner_State_MortarExplosion(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer & 3))
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y);

    self->position.y -= 0x40000;
    if (++self->timer == 16)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void PhantomGunner_EditorDraw(void)
{
    RSDK_THIS(PhantomGunner);
    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 1, &self->fxAnimator, false, 0);
    self->originPos   = self->position;
    self->screenPos.x = (self->position.x >> 16) - ScreenInfo->centerX;
    self->screenPos.y = (self->position.y >> 16) - ScreenInfo->centerY;

    PhantomGunner_Draw_Gunner();
}

void PhantomGunner_EditorLoad(void) { PhantomGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE); }
#endif

void PhantomGunner_Serialize(void) {}
