// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZGunner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectERZGunner *ERZGunner;

void ERZGunner_Update(void)
{
    RSDK_THIS(ERZGunner);
    StateMachine_Run(self->state);
}

void ERZGunner_LateUpdate(void) {}

void ERZGunner_StaticUpdate(void) {}

void ERZGunner_Draw(void)
{
    RSDK_THIS(ERZGunner);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);
    }
}

void ERZGunner_Create(void *data)
{
    RSDK_THIS(ERZGunner);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawOrder     = 1;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->type          = voidToInt(data);
        switch (self->type) {
            case 0:
                self->drawFX = FX_FLIP;
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 1, &self->animator2, true, 0);

                self->posUnknown1 = self->position;
                self->posUnknown2 = ScreenInfo->position;
                self->stateDraw   = ERZGunner_StateDraw_Unknown0;
                self->state       = ERZGunner_State_Unknown1;
                break;
            case 1:
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 10, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 11, &self->animator2, true, 0);
                self->stateDraw = ERZGunner_StateDraw_Unknown1;
                self->state     = ERZGunner_State1_Unknown1;
                break;
            case 5:
                self->posUnknown1 = self->position;
                self->state       = ERZGunner_State2_Unknown;
                break;
            case 6:
                self->posUnknown1 = self->position;
                self->state       = ERZGunner_State3_Unknown;
                break;
            default: break;
        }
    }
}

void ERZGunner_StageLoad(void)
{
    ERZGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE);

    ERZGunner->hitbox1.left   = -8;
    ERZGunner->hitbox1.top    = -4;
    ERZGunner->hitbox1.right  = 4;
    ERZGunner->hitbox1.bottom = 26;

    ERZGunner->hitbox2.left   = -16;
    ERZGunner->hitbox2.top    = -32;
    ERZGunner->hitbox2.right  = 16;
    ERZGunner->hitbox2.bottom = -16;

    ERZGunner->hitbox3.left   = -8;
    ERZGunner->hitbox3.top    = -8;
    ERZGunner->hitbox3.right  = 8;
    ERZGunner->hitbox3.bottom = 8;
}

void ERZGunner_Explode(void)
{
    RSDK_THIS(ERZGunner);
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

void ERZGunner_Explode2(void)
{
    RSDK_THIS(ERZGunner);

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

void ERZGunner_SpawnDust(void)
{
    RSDK_THIS(ERZGunner);

    for (int i = 0; i < 4; ++i) {
        int x            = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int y            = self->position.y + RSDK.Rand(-0x280000, -0x180000);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);
        dust->state      = Dust_State_Move;
        dust->drawOrder  = Zone->drawOrderHigh;
    }
}

void ERZGunner_HandleRotations(int angle)
{
    RSDK_THIS(ERZGunner);

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

void ERZGunner_CheckPlayerMissileCollisions(void)
{
    RSDK_THIS(ERZGunner);

    foreach_active(Player, player)
    {
        if (self->animator4.frameID > 0 && player->velocity.y >= 0 && Player_CheckBadnikTouch(player, self, &ERZGunner->hitbox2)
            && player->animator.animationID != ANI_HURT) {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
            player->velocity.y = -0x60000;
            ERZGunner_SpawnDust();
            self->state = ERZGunner_State1_Unknown3;
        }
        else {
            if (Player_CheckBadnikTouch(player, self, &ERZGunner->hitbox1)) {
                int anim = player->animator.animationID;
                if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), self->position.x, self->position.y);
                    explosion->interaction     = false;
                    explosion->drawOrder       = Zone->drawOrderHigh;
                    if (self->animator4.animationID == 12 && self->animator4.frameID > 0)
                        ERZGunner_SpawnDust();
                    RSDK.PlaySfx(ERZKing->sfxExplosion2, false, 255);
                    destroyEntity(self);
                    foreach_break;
                }
            }
        }
    }
}

void ERZGunner_CheckPlayerExplosionCollisions(void)
{
    foreach_active(Player, player)
    {
        foreach_active(Explosion, explosion)
        {
            if (explosion->animator.frameID <= 6) {
                if (Player_CheckCollisionTouch(player, explosion, &ERZGunner->hitbox3)) {
                    Player_CheckElementalHit(player, explosion, SHIELD_FIRE);
                }
            }
        }
    }
}

void ERZGunner_StateDraw_Unknown0(void)
{
    RSDK_THIS(ERZGunner);
    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 128, 0, 128, 128);
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator2, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }
}

void ERZGunner_StateDraw_Unknown1(void)
{
    RSDK_THIS(ERZGunner);
    EntityERZGunner *parent = (EntityERZGunner *)self->parent;

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ((self->posUnknown1.y + parent->position.y) >> 16) - ScreenInfo->position.y);

    Vector2 drawPos;
    drawPos.x = parent->position.x + self->posUnknown1.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    drawPos.y = self->posUnknown1.y + parent->position.y;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
}

void ERZGunner_StateDraw_Unknown2(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator3, NULL, false);
    RSDK.DrawSprite(&self->animator4, NULL, false);
}

void ERZGunner_State_Unknown1(void)
{
    RSDK_THIS(ERZGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown1.y;

    self->position.x += (ScreenInfo->position.x - self->posUnknown2.x) << 15;
    self->posUnknown2 = ScreenInfo->position;
    RSDK.ProcessAnimation(&self->animator2);

    self->timer3 = 8;
    if (++self->timer2 < 120 || RSDK.GetEntityCount(ERZGunner->objectID, true) >= 2) {
        ERZGunner_CheckPlayerExplosionCollisions();
    }
    else {
        if (++self->timer == 2) {
            foreach_all(ERZKing, king) { king->active = ACTIVE_NORMAL; }
            destroyEntity(self);
        }
        else {
            ERZGunner->value2 = 0;
            self->timer2    = 0;
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 2, &self->animator1, true, 0);
            self->state = ERZGunner_State_Unknown2;
            ERZGunner_CheckPlayerExplosionCollisions();
        }
    }
}

void ERZGunner_State_Unknown2(void)
{
    RSDK_THIS(ERZGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->angle      = (self->angle + 3) & 0xFF;
    self->position.y = (RSDK.Sin256(self->angle) << 11) + self->posUnknown1.y;

    self->position.x += (ScreenInfo->position.x - self->posUnknown2.x) << 15;
    self->posUnknown2 = ScreenInfo->position;
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    if (++self->timer2 == 8) {
        EntityERZGunner *child = CREATE_ENTITY(ERZGunner, intToVoid(1), self->position.x, self->position.y);
        child->parent          = (Entity *)self;
        child->posUnknown1.x   = ERZGunner->value1[self->field_80] << 16;
        child->posUnknown1.y   = ERZGunner->value1[self->field_80 + 1] << 16;
        child->position.x += child->posUnknown1.x;
        child->position.y += child->posUnknown1.y + 0x100000;
        self->field_80 = (self->field_80 - 14) & 0x1F;
    }

    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        self->timer2 = 0;
        if (--self->timer3 > 0) {
            if (!(self->timer3 & 1))
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 2, &self->animator1, true, 0);
            else
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 3, &self->animator1, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &self->animator1, true, 0);
            self->state = ERZGunner_State_Unknown1;
        }
    }
    ERZGunner_CheckPlayerExplosionCollisions();
}

void ERZGunner_State1_Unknown1(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    self->position.y -= 0x80000;
    if (++self->timer2 == 60) {
        EntityERZGunner *parent = (EntityERZGunner *)self->parent;

        self->timer2 = 0;
        if ((ERZGunner->value2 & 3) == 3)
            self->type = 4;
        else
            self->type = (ERZGunner->value2 & 1) + 2;
        ++ERZGunner->value2;
        self->drawOrder  = Zone->drawOrderLow;
        self->position.y = (ScreenInfo->position.y - 64) << 16;

        bool32 flag = false;
        while (!flag) {
            flag               = true;
            self->position.x = parent->position.x + RSDK.Rand(-0x1000000, 0x1000000);

            foreach_active(ERZGunner, gunner)
            {
                if (gunner != self && gunner->type >= 2) {
                    int dist = abs(gunner->position.x - self->position.x);
                    if (dist < 0x180000 && gunner->position.y - self->position.y < 0x800000)
                        flag = false;
                }
            }
        }

        if (self->type == 2) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 4, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 5, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 12, &self->animator4, true, 0);
            self->state     = ERZGunner_State1_Unknown2;
            self->timer2    = RSDK.Rand(0, 2) << 8;
            self->stateDraw = ERZGunner_StateDraw_Unknown2;
        }
        else if (self->type == 3) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 6, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 7, &self->animator3, true, 0);
            self->state     = ERZGunner_State1_Unknown3;
            self->stateDraw = ERZGunner_StateDraw_Unknown2;
        }
        else if (self->type == 4) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 8, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 9, &self->animator3, true, 0);
            self->position.x = parent->position.x;
            self->drawFX     = FX_ROTATE;
            self->state      = ERZGunner_State1_Unknown4;
            if (!(Zone->timer & 1))
                self->velocity.x = 0x40000;
            else
                self->velocity.x = -0x40000;
            self->stateDraw = ERZGunner_StateDraw_Unknown2;
        }
    }
}

void ERZGunner_State1_Unknown2(void)
{
    RSDK_THIS(ERZGunner);

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

    ERZGunner_CheckPlayerMissileCollisions();
    if (self->objectID) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == 3) {
                EntityERZGunner *child = CREATE_ENTITY(ERZGunner, intToVoid(5), self->position.x, self->position.y);
                if (self->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(ERZGunner, intToVoid(6), self->position.x, self->position.y);
                destroyEntity(self);
            }
        }
    }
}

void ERZGunner_State1_Unknown3(void)
{
    RSDK_THIS(ERZGunner);
    RSDK.ProcessAnimation(&self->animator3);

    if (self->velocity.y < 0x40000)
        self->velocity.y += 0x1800;
    self->position.y += self->velocity.y;

    ERZGunner_CheckPlayerMissileCollisions();
    if (self->objectID) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == 3) {
                EntityERZGunner *child = CREATE_ENTITY(ERZGunner, intToVoid(5), self->position.x, self->position.y);
                if (self->velocity.y > 0x20000)
                    child->velocity.x = 0x80000;
                else
                    child->velocity.x = 0x40000;
                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(ERZGunner, intToVoid(6), self->position.x, self->position.y);
                destroyEntity(self);
            }
        }
    }
}

void ERZGunner_State1_Unknown4(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->animator3);
    EntityERZGunner *parent = (EntityERZGunner *)self->parent;

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
    ERZGunner_Explode();

    int angle = RSDK.ATan2(self->velocity.y, -self->velocity.x);
    ERZGunner_HandleRotations(2 * angle);
    if (self->timer2 == 320) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y);
        explosion->interaction     = false;
        explosion->drawOrder       = Zone->drawOrderHigh;
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &ERZGunner->hitbox3)) {
                self->angle     = self->rotation << 8;
                self->timer2    = 0;
                self->drawFX    = FX_SCALE | FX_ROTATE;
                self->groundVel = 0x4000;
                self->scale.x   = 0x200;
                self->scale.y   = 0x200;
                self->state     = ERZGunner_State1_Unknown5;
                RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            }
        }
    }
}

void ERZGunner_State1_Unknown5(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->animator3);

    self->angle += self->groundVel;
    self->rotation = self->angle >> 8;
    self->groundVel -= (self->groundVel >> 3);

    ERZGunner_Explode();
    if (self->groundVel < 128)
        self->state = ERZGunner_State1_Unknown6;
}

void ERZGunner_State1_Unknown6(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->animator3);
    EntityERZGunner *parent = (EntityERZGunner *)self->parent;

    int angle = RSDK.ATan2((parent->position.y - self->position.y) >> 16, -((parent->position.x - self->position.x) >> 16));
    ERZGunner_HandleRotations(2 * angle);
    self->rotation &= 0x1FF;
    self->position.x -= RSDK.Sin512(self->rotation) << 9;
    self->scale.x -= 6;
    self->position.y += RSDK.Cos512(self->rotation) << 9;
    self->scale.y = self->scale.x;

    ERZGunner_Explode2();
    if (self->scale.x < 128) {
        RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
        parent->invincibilityTimer = 48;
        self->position.x         = parent->position.x;
        self->position.y         = parent->position.y;
        self->visible            = false;
        self->state              = ERZGunner_State1_Unknown7;
    }
}

void ERZGunner_State1_Unknown7(void)
{
    RSDK_THIS(ERZGunner);

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

void ERZGunner_State2_Unknown(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer & 3)) {
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->posUnknown1.x - self->field_84, self->position.y);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->posUnknown1.x + self->field_84, self->position.y);
    }
    self->field_84 += self->velocity.x;
    if (++self->timer2 == 16)
        destroyEntity(self);
}

void ERZGunner_State3_Unknown(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer & 3))
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ITEMBOX), self->position.x, self->position.y);
    self->position.y -= 0x40000;
    if (++self->timer2 == 16)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void ERZGunner_EditorDraw(void)
{
    RSDK_THIS(ERZGunner);
    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 1, &self->animator2, false, 0);

    self->posUnknown1 = self->position;
    self->posUnknown2 = ScreenInfo->position;

    ERZGunner_StateDraw_Unknown0();
}

void ERZGunner_EditorLoad(void) { ERZGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE); }
#endif

void ERZGunner_Serialize(void) {}
