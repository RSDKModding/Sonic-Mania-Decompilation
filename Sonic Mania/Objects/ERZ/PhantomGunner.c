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
        RSDK.DrawSprite(&self->animator1, NULL, false);
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
        self->type          = voidToInt(data);
        switch (self->type) {
            case 0:
                self->drawFX = FX_FLIP;
                self->active = ACTIVE_NEVER;
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 1, &self->animator2, true, 0);
                self->posUnknown1   = self->position;
                self->posUnknown2.x = (self->position.x >> 16) - ScreenInfo->centerX;
                self->posUnknown2.y = (self->position.y >> 16) - ScreenInfo->centerY;
                self->stateDraw     = PhantomGunner_StateDraw_Unknown0;
                self->state         = PhantomGunner_State_Unknown1;
                break;
            case 1:
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 10, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 11, &self->animator2, true, 0);
                self->stateDraw = PhantomGunner_StateDraw_Unknown1;
                self->state     = PhantomGunner_State1_Unknown1;
                break;
            case 5:
                self->posUnknown1 = self->position;
                self->state       = PhantomGunner_State2_Unknown;
                break;
            case 6:
                self->posUnknown1 = self->position;
                self->state       = PhantomGunner_State3_Unknown;
                break;
            default: break;
        }
    }
}

void PhantomGunner_StageLoad(void)
{
    PhantomGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE);

    PhantomGunner->hitbox1.left   = -8;
    PhantomGunner->hitbox1.top    = 4;
    PhantomGunner->hitbox1.right  = 8;
    PhantomGunner->hitbox1.bottom = 26;

    PhantomGunner->hitbox2.left   = -16;
    PhantomGunner->hitbox2.top    = -32;
    PhantomGunner->hitbox2.right  = 16;
    PhantomGunner->hitbox2.bottom = -16;

    PhantomGunner->hitbox3.left   = -8;
    PhantomGunner->hitbox3.top    = -8;
    PhantomGunner->hitbox3.right  = 8;
    PhantomGunner->hitbox3.bottom = 8;

    PhantomGunner->sfxCannonFire = RSDK.GetSfx("Stage/CannonFire.wav");
}

void PhantomGunner_Explode(void)
{
    RSDK_THIS(PhantomGunner);
    if (!(Zone->timer & 7)) {
        int x                = RSDK.Sin512(self->rotation) << 11;
        int y                = RSDK.Cos512(self->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + x, self->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(self->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(self->rotation) << 4;
        debris->drawOrder  = Zone->drawOrderLow;
        debris->timer      = 41;
    }
}

void PhantomGunner_Explode2(void)
{
    RSDK_THIS(PhantomGunner);

    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int x                = 0x600 * RSDK.Sin512(self->rotation);
            int y                = 0x600 * RSDK.Cos512(self->rotation);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + x, self->position.y - y);
            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->drawOrder = Zone->drawOrderHigh;
            debris->drawFX    = FX_SCALE;
            debris->timer     = 52;
            debris->scale.x   = (self->scale.x * RSDK.Rand(128, 384)) >> 9;
            debris->scale.y   = debris->scale.x;
        }
    }
    else {
        int x                = RSDK.Sin512(self->rotation) << 11;
        int y                = RSDK.Cos512(self->rotation) << 11;
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, self->position.x + x, self->position.y - y);
        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(self->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(self->rotation) << 4;
        debris->drawOrder  = Zone->drawOrderLow;
        debris->drawFX     = FX_SCALE;
        debris->timer      = 41;
        debris->scale.y    = self->scale.x;
    }
}

void PhantomGunner_SpawnDust(void)
{
    RSDK_THIS(PhantomGunner);

    for (int i = 0; i < 4; ++i) {
        int x            = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int y            = self->position.y + RSDK.Rand(-0x280000, -0x180000);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);
        dust->state      = Dust_State_Move;
        dust->drawOrder  = Zone->drawOrderHigh;
    }
}

void PhantomGunner_HandleRotations(int angle)
{
    RSDK_THIS(PhantomGunner);

    int ang = angle - self->rotation;
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
        if (self->animator4.frameID > 0 && player->velocity.y >= 0 && Player_CheckBadnikTouch(player, self, &PhantomGunner->hitbox2)
            && player->animator.animationID != ANI_HURT) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
            player->velocity.y = -0x60000;
            PhantomGunner_SpawnDust();
            self->state = PhantomGunner_State1_Unknown3;
        }
        else {
            if (Player_CheckBadnikTouch(player, self, &PhantomGunner->hitbox1)) {
                int anim = player->animator.animationID;
                if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y);
                    explosion->interaction     = false;
                    explosion->drawOrder       = Zone->drawOrderHigh;
                    if (self->animator4.animationID == 12 && self->animator4.frameID > 0)
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
                if (Player_CheckCollisionTouch(player, explosion, &PhantomGunner->hitbox3)) {
                    Player_CheckElementalHit(player, explosion, SHIELD_FIRE);
                }
            }
        }
    }
}

void PhantomGunner_StateDraw_Unknown0(void)
{
    RSDK_THIS(PhantomGunner);
    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(6, 128, 4, 128, 128);
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        RSDK.CopyPalette(5, 128, 4, 128, 128);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
}

void PhantomGunner_StateDraw_Unknown1(void)
{
    RSDK_THIS(PhantomGunner);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ((self->posUnknown1.y + parent->position.y) >> 16) - ScreenInfo->position.y);

    Vector2 drawPos;
    drawPos.x = parent->position.x + self->posUnknown1.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    drawPos.y = self->posUnknown1.y + parent->position.y;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
}

void PhantomGunner_StateDraw_Unknown2(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
}

void PhantomGunner_State_ResetState(void)
{
    RSDK_THIS(PhantomGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown1.y;

    self->position.x += (ScreenInfo->position.x - self->posUnknown2.x) << 15;
    self->posUnknown2 = ScreenInfo->position;

    RSDK.ProcessAnimation(&self->animator2);
    if (++self->timer2 == 60) {
        self->position      = self->startPos;
        self->posUnknown1   = self->startPos;
        self->timer2        = 0;
        self->timer         = 0;
        self->posUnknown2.x = (self->position.x >> 16) - ScreenInfo->centerX;
        self->posUnknown2.y = (self->position.y >> 16) - ScreenInfo->centerY;
        self->stateDraw     = PhantomGunner_StateDraw_Unknown0;
        self->state         = PhantomGunner_State_Unknown1;
        self->active        = ACTIVE_NEVER;
    }
}

void PhantomGunner_State_Unknown1(void)
{
    RSDK_THIS(PhantomGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown1.y;

    self->position.x += (ScreenInfo->position.x - self->posUnknown2.x) << 15;
    self->posUnknown2 = ScreenInfo->position;
    RSDK.ProcessAnimation(&self->animator2);

    self->timer3 = 8;
    if (++self->timer2 < 120 || RSDK.GetEntityCount(PhantomGunner->objectID, true) >= 2) {
        PhantomGunner_CheckPlayerExplosionCollisions();
    }
    else {
        if (++self->timer == 2) {
            PhantomEgg_SetupScanlineCB();
            self->timer2 = 0;
            self->state  = PhantomGunner_State_ResetState;
        }
        else {
            PhantomGunner->value2 = 0;
            self->timer2        = 0;
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 2, &self->animator1, true, 0);
            self->state = PhantomGunner_State_Unknown2;
            PhantomGunner_CheckPlayerExplosionCollisions();
        }
    }
}

void PhantomGunner_State_Unknown2(void)
{
    RSDK_THIS(PhantomGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown1.y;

    self->position.x += (ScreenInfo->position.x - self->posUnknown2.x) << 15;
    self->posUnknown2 = ScreenInfo->position;
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    if (++self->timer2 == 8) {
        EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(1), self->position.x, self->position.y);
        child->parent              = (Entity *)self;
        child->posUnknown1.x       = PhantomGunner->value1[self->field_8C] << 16;
        child->posUnknown1.y       = PhantomGunner->value1[self->field_8C + 1] << 16;
        child->position.x += child->posUnknown1.x;
        child->position.y += child->posUnknown1.y + 0x100000;
        self->field_8C = (self->field_8C - 14) & 0x1F;
        RSDK.PlaySfx(PhantomGunner->sfxCannonFire, false, 255);
    }

    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        self->timer2 = 0;
        if (--self->timer3 > 0) {
            if (!(self->timer3 & 1))
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 2, &self->animator1, true, 0);
            else
                RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 3, &self->animator1, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &self->animator1, true, 0);
            self->state = PhantomGunner_State_Unknown1;
        }
    }
    PhantomGunner_CheckPlayerExplosionCollisions();
}

void PhantomGunner_State1_Unknown1(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    self->position.y -= 0x80000;
    if (++self->timer2 == 60) {
        EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

        self->timer2 = 0;
        if ((PhantomGunner->value2 & 3) == 3)
            self->type = 4;
        else
            self->type = (PhantomGunner->value2 & 1) + 2;
        ++PhantomGunner->value2;
        self->drawOrder  = Zone->drawOrderLow;
        self->position.y = (ScreenInfo->position.y - 64) << 16;

        bool32 flag = false;
        while (!flag) {
            flag               = true;
            self->position.x = parent->position.x + RSDK.Rand(-0x1000000, 0x1000000);

            foreach_active(PhantomGunner, gunner)
            {
                if (gunner != self && gunner->type >= 2) {
                    int dist = abs(gunner->position.x - self->position.x);
                    if (dist < 0x180000 && gunner->position.y - self->position.y < 0x800000)
                        flag = false;
                }
            }
        }

        if (self->type == 2) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 4, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 5, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 12, &self->animator4, true, 0);
            self->state     = PhantomGunner_State1_Unknown2;
            self->timer2    = RSDK.Rand(0, 2) << 8;
            self->stateDraw = PhantomGunner_StateDraw_Unknown2;
        }
        else if (self->type == 3) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 6, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 7, &self->animator3, true, 0);
            self->state     = PhantomGunner_State1_Unknown3;
            self->stateDraw = PhantomGunner_StateDraw_Unknown2;
        }
        else if (self->type == 4) {
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 8, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 9, &self->animator3, true, 0);
            self->position.x = parent->position.x;
            self->drawFX     = FX_ROTATE;
            self->state      = PhantomGunner_State1_Unknown4;
            if (!(Zone->timer & 1))
                self->velocity.x = 0x40000;
            else
                self->velocity.x = -0x40000;
            self->stateDraw = PhantomGunner_StateDraw_Unknown2;
        }
    }
}

void PhantomGunner_State1_Unknown2(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->animator4.frameID) {
        self->drawFX = FX_ROTATE;
        if (self->velocity.y > 0xC000)
            self->velocity.y -= 0x3800;
        self->rotation = RSDK.Sin512(self->timer2) >> 6;
        self->timer2   = (self->timer2 + 4) & 0x1FF;
    }
    else if (self->velocity.y < 0x40000)
        self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    PhantomGunner_CheckPlayerMissileCollisions();
    if (self->objectID) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == 3) {
                EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(5), self->position.x, self->position.y);
                if (self->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(PhantomGunner, intToVoid(6), self->position.x, self->position.y);
                destroyEntity(self);
            }
        }
    }
}

void PhantomGunner_State1_Unknown3(void)
{
    RSDK_THIS(PhantomGunner);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->velocity.y < 0x40000)
        self->velocity.y += 0x1800;
    self->position.y += self->velocity.y;

    PhantomGunner_CheckPlayerMissileCollisions();
    if (self->objectID) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == 3) {
                EntityPhantomGunner *child = CREATE_ENTITY(PhantomGunner, intToVoid(5), self->position.x, self->position.y);
                if (self->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(PhantomGunner, intToVoid(6), self->position.x, self->position.y);
                destroyEntity(self);
            }
        }
    }
}

void PhantomGunner_State1_Unknown4(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->animator3);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

    ++self->timer2;

    self->velocity.x = RSDK.Cos256(self->timer2) << 11;
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
    PhantomGunner_Explode();

    int angle = RSDK.ATan2(self->velocity.y, -self->velocity.x);
    PhantomGunner_HandleRotations(2 * angle);
    if (self->timer2 == 320) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y);
        explosion->interaction     = false;
        explosion->drawOrder       = Zone->drawOrderHigh;
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &PhantomGunner->hitbox3)) {
                self->angle     = self->rotation << 8;
                self->timer2    = 0;
                self->drawFX    = FX_SCALE | FX_ROTATE;
                self->groundVel = 0x4000;
                self->scale.x   = 0x200;
                self->scale.y   = 0x200;
                self->state     = PhantomGunner_State1_Unknown5;
                RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
            }
        }
    }
}

void PhantomGunner_State1_Unknown5(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->animator3);

    self->angle += self->groundVel;
    self->rotation = self->angle >> 8;
    self->groundVel -= (self->groundVel >> 3);

    PhantomGunner_Explode();
    if (self->groundVel < 128) {
        RSDK.PlaySfx(PhantomEgg->sfxRocketJet, false, 255);
        self->state = PhantomGunner_State1_Unknown6;
    }
}

void PhantomGunner_State1_Unknown6(void)
{
    RSDK_THIS(PhantomGunner);

    RSDK.ProcessAnimation(&self->animator3);
    EntityPhantomGunner *parent = (EntityPhantomGunner *)self->parent;

    int angle = RSDK.ATan2((parent->position.y - self->position.y) >> 16, -((parent->position.x - self->position.x) >> 16));
    PhantomGunner_HandleRotations(2 * angle);
    self->rotation &= 0x1FF;
    self->position.x -= RSDK.Sin512(self->rotation) << 9;
    self->scale.x -= 6;
    self->position.y += RSDK.Cos512(self->rotation) << 9;
    self->scale.y = self->scale.x;

    PhantomGunner_Explode2();
    if (self->scale.x < 128) {
        RSDK.PlaySfx(PhantomEgg->sfxHit, false, 255);
        parent->invincibilityTimer = 48;
        self->position.x         = parent->position.x;
        self->position.y         = parent->position.y;
        self->visible            = false;
        self->state              = PhantomGunner_State1_Unknown7;
    }
}

void PhantomGunner_State1_Unknown7(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer % 3) && !(Zone->timer & 4)) {
        int x                      = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int y                      = self->position.y + RSDK.Rand(-0x100000, 0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
        explosion->interaction     = false;
        explosion->drawOrder       = 1;
        explosion->scale           = self->scale;
    }
    if (++self->timer2 == 16)
        destroyEntity(self);
}

void PhantomGunner_State2_Unknown(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer & 3)) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->posUnknown1.x - self->field_90, self->position.y);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->posUnknown1.x + self->field_90, self->position.y);
    }
    self->field_90 += self->velocity.x;
    if (++self->timer2 == 16)
        destroyEntity(self);
}

void PhantomGunner_State3_Unknown(void)
{
    RSDK_THIS(PhantomGunner);

    if (!(Zone->timer & 3))
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y);
    self->position.y -= 0x40000;
    if (++self->timer2 == 16)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void PhantomGunner_EditorDraw(void)
{
    RSDK_THIS(PhantomGunner);
    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(PhantomGunner->aniFrames, 1, &self->animator2, false, 0);
    self->posUnknown1   = self->position;
    self->posUnknown2.x = (self->position.x >> 16) - ScreenInfo->centerX;
    self->posUnknown2.y = (self->position.y >> 16) - ScreenInfo->centerY;

    PhantomGunner_StateDraw_Unknown0();
}

void PhantomGunner_EditorLoad(void) { PhantomGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE); }
#endif

void PhantomGunner_Serialize(void) {}
