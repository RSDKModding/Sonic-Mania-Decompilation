// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZGunner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void ERZGunner_Create(void *data)
{
    RSDK_THIS(ERZGunner);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = 1;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->type = VOID_TO_INT(data);
        switch (self->type) {
            case ERZGUNNER_BOSS:
                self->drawFX = FX_FLIP;
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &self->mainAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 1, &self->fxAnimator, true, 0);

                self->originPos = self->position;
                self->screenPos = ScreenInfo->position;

                self->stateDraw = ERZGunner_Draw_Gunner;
                self->state     = ERZGunner_State_Idle;
                break;

            case ERZGUNNER_LAUNCHROCKET:
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 10, &self->mainAnimator, true, 0);
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 11, &self->fxAnimator, true, 0);

                self->stateDraw = ERZGunner_Draw_RocketLaunch;
                self->state     = ERZGunner_State_LaunchedRocket;
                break;

            case ERZGUNNER_NAPALM_EXPLOSION:
                self->originPos = self->position;

                self->state = ERZGunner_State_NapalmExplosion;
                break;

            case ERZGUNNER_MORTAR_EXPLOSION:
                self->originPos = self->position;

                self->state = ERZGunner_State_MortarExplosion;
                break;

            default: break;
        }
    }
}

void ERZGunner_StageLoad(void)
{
    ERZGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE);

    ERZGunner->hitboxNapalm.left   = -8;
    ERZGunner->hitboxNapalm.top    = -4;
    ERZGunner->hitboxNapalm.right  = 4;
    ERZGunner->hitboxNapalm.bottom = 26;

    ERZGunner->hitboxMortar.left   = -16;
    ERZGunner->hitboxMortar.top    = -32;
    ERZGunner->hitboxMortar.right  = 16;
    ERZGunner->hitboxMortar.bottom = -16;

    ERZGunner->hitboxDud.left   = -8;
    ERZGunner->hitboxDud.top    = -8;
    ERZGunner->hitboxDud.right  = 8;
    ERZGunner->hitboxDud.bottom = 8;
}

void ERZGunner_HandleDudExhaust(void)
{
    RSDK_THIS(ERZGunner);
    if (!(Zone->timer & 7)) {
        int32 x              = self->position.x + (RSDK.Sin512(self->rotation) << 11);
        int32 y              = self->position.y - (RSDK.Cos512(self->rotation) << 11);
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);

        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(self->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(self->rotation) << 4;
        debris->drawGroup  = Zone->objectDrawGroup[0];
        debris->timer      = 41;
    }
}

void ERZGunner_HandleMalfunctionDudExhaust(void)
{
    RSDK_THIS(ERZGunner);

    if (Zone->timer & 3) {
        if ((Zone->timer & 3) == 2) {
            int32 x              = self->position.x + (RSDK.Sin512(self->rotation) * 0x600);
            int32 y              = self->position.y - (RSDK.Cos512(self->rotation) * 0x600);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);

            RSDK.SetSpriteAnimation(Explosion->aniFrames, 2, &debris->animator, true, 0);
            debris->drawGroup = Zone->objectDrawGroup[1];
            debris->drawFX    = FX_SCALE;
            debris->timer     = 52;
            debris->scale.x   = (self->scale.x * RSDK.Rand(128, 384)) >> 9;
            debris->scale.y   = debris->scale.x;
        }
    }
    else {
        int32 x              = self->position.x + (RSDK.Sin512(self->rotation) << 11);
        int32 y              = self->position.y - (RSDK.Cos512(self->rotation) << 11);
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);

        RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
        debris->velocity.x = RSDK.Sin512(self->rotation) << 8;
        debris->velocity.y = RSDK.Sin512(self->rotation) << 4;
        debris->drawGroup  = Zone->objectDrawGroup[0];
        debris->drawFX     = FX_SCALE;
        debris->timer      = 41;
        debris->scale.y    = self->scale.x;
    }
}

void ERZGunner_SpawnDust(void)
{
    RSDK_THIS(ERZGunner);

    for (int32 i = 0; i < 4; ++i) {
        int32 x          = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int32 y          = self->position.y + RSDK.Rand(-0x280000, -0x180000);
        EntityDust *dust = CREATE_ENTITY(Dust, NULL, x, y);

        dust->state     = Dust_State_DustPuff;
        dust->drawGroup = Zone->objectDrawGroup[1];
    }
}

void ERZGunner_HandleRotations(int32 angle)
{
    RSDK_THIS(ERZGunner);

    int32 ang = angle - self->rotation;

    if (abs(angle - self->rotation) >= abs(angle - self->rotation - 0x200)) {
        if (abs(angle - self->rotation - 0x200) < abs(angle - self->rotation + 0x200))
            self->rotation += (ang - 0x200) >> 4;
        else
            self->rotation += (ang + 0x200) >> 4;
    }
    else {
        if (abs(angle - self->rotation) < abs(angle - self->rotation + 0x200))
            self->rotation += ang >> 4;
        else
            self->rotation += (ang + 0x200) >> 4;
    }

    self->rotation &= 0x1FF;
}

void ERZGunner_CheckPlayerMissileCollisions(void)
{
    RSDK_THIS(ERZGunner);

    foreach_active(Player, player)
    {
        if (self->parachuteAnimator.frameID > 0 && player->velocity.y >= 0 && Player_CheckBadnikTouch(player, self, &ERZGunner->hitboxMortar)
            && player->animator.animationID != ANI_HURT) {
            RSDK.SetSpriteAnimation(-1, 0, &self->parachuteAnimator, true, 0);
            player->velocity.y = -0x60000;

            ERZGunner_SpawnDust();
            self->state = ERZGunner_State_Napalm;
        }
        else {
            if (Player_CheckBadnikTouch(player, self, &ERZGunner->hitboxNapalm)) {
                int32 anim = player->animator.animationID;
                if (anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH) {
                    EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ENEMY), self->position.x, self->position.y);
                    explosion->interaction     = false;
                    explosion->drawGroup       = Zone->objectDrawGroup[1];

                    if (self->parachuteAnimator.animationID == 12 && self->parachuteAnimator.frameID > 0)
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
                if (Player_CheckCollisionTouch(player, explosion, &ERZGunner->hitboxDud)) {
                    Player_ElementHurt(player, explosion, SHIELD_FIRE);
                }
            }
        }
    }
}

void ERZGunner_Hit(EntityERZGunner *entity)
{
    RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
    entity->invincibilityTimer = 48;
}

void ERZGunner_Draw_Gunner(void)
{
    RSDK_THIS(ERZGunner);

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 128, 0, 128, 128);

        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);

        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->fxAnimator, NULL, false);
    }
}

void ERZGunner_Draw_RocketLaunch(void)
{
    RSDK_THIS(ERZGunner);

    EntityERZGunner *parent = self->parent;

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ((self->originPos.y + parent->position.y) >> 16) - ScreenInfo->position.y);

    Vector2 drawPos;
    drawPos.x = parent->position.x + self->originPos.x;
    drawPos.y = self->position.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);

    drawPos.y = self->originPos.y + parent->position.y;
    RSDK.DrawSprite(&self->fxAnimator, &drawPos, false);
}

void ERZGunner_Draw_Rocket(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->tailAnimator, NULL, false);
    RSDK.DrawSprite(&self->parachuteAnimator, NULL, false);
}

void ERZGunner_State_Idle(void)
{
    RSDK_THIS(ERZGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += (ScreenInfo->position.x - self->screenPos.x) << 15;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    self->screenPos = ScreenInfo->position;

    RSDK.ProcessAnimation(&self->fxAnimator);

    self->fireAnimTimer = 8;
    if (++self->timer < 120 || RSDK.GetEntityCount(ERZGunner->classID, true) >= 2) {
        ERZGunner_CheckPlayerExplosionCollisions();
    }
    else {
        if (++self->rocketLaunchCount == 2) {
            foreach_all(ERZKing, king) { king->active = ACTIVE_NORMAL; }
            destroyEntity(self);
        }
        else {
            ERZGunner->launchedRocketID = 0;
            self->timer                 = 0;
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 2, &self->mainAnimator, true, 0);

            self->state = ERZGunner_State_LaunchRockets;
            ERZGunner_CheckPlayerExplosionCollisions();
        }
    }
}

void ERZGunner_State_LaunchRockets(void)
{
    RSDK_THIS(ERZGunner);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    self->position.x += (ScreenInfo->position.x - self->screenPos.x) << 15;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 3, 11);

    self->screenPos = ScreenInfo->position;

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);

    if (++self->timer == 8) {
        EntityERZGunner *rocket = CREATE_ENTITY(ERZGunner, INT_TO_VOID(ERZGUNNER_LAUNCHROCKET), self->position.x, self->position.y);
        rocket->parent          = self;
        rocket->originPos.x     = ERZGunner->rocketOffsets[self->rocketOffsetID] << 16;
        rocket->originPos.y     = ERZGunner->rocketOffsets[self->rocketOffsetID + 1] << 16;
        rocket->position.x += rocket->originPos.x;
        rocket->position.y += rocket->originPos.y + 0x100000;

        self->rocketOffsetID = (self->rocketOffsetID - 14) & 0x1F;
    }

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        self->timer = 0;

        if (--self->fireAnimTimer > 0) {
            if (!(self->fireAnimTimer & 1))
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 2, &self->mainAnimator, true, 0);
            else
                RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 3, &self->mainAnimator, true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &self->mainAnimator, true, 0);
            self->state = ERZGunner_State_Idle;
        }
    }

    ERZGunner_CheckPlayerExplosionCollisions();
}

void ERZGunner_State_LaunchedRocket(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fxAnimator);

    self->position.y -= 0x80000;

    if (++self->timer == 60) {
        EntityERZGunner *parent = self->parent;

        self->timer = 0;

        if ((ERZGunner->launchedRocketID & 3) == 3)
            self->type = ERZGUNNER_DUD;
        else
            self->type = (ERZGunner->launchedRocketID & 1) + ERZGUNNER_MORTAR;

        ++ERZGunner->launchedRocketID;

        self->drawGroup  = Zone->objectDrawGroup[0];
        self->position.y = (ScreenInfo->position.y - 64) << 16;

        bool32 canFire = false;
        while (!canFire) {
            canFire          = true;
            self->position.x = parent->position.x + RSDK.Rand(-0x1000000, 0x1000000);

            foreach_active(ERZGunner, gunner)
            {
                if (gunner != self && gunner->type >= ERZGUNNER_MORTAR) {
                    int32 dist = abs(gunner->position.x - self->position.x);
                    if (dist < 0x180000 && gunner->position.y - self->position.y < 0x800000)
                        canFire = false;
                }
            }
        }

        if (self->type == ERZGUNNER_MORTAR) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 4, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 5, &self->tailAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 12, &self->parachuteAnimator, true, 0);

            self->state     = ERZGunner_State_Mortar;
            self->timer     = RSDK.Rand(0, 2) << 8;
            self->stateDraw = ERZGunner_Draw_Rocket;
        }
        else if (self->type == ERZGUNNER_NAPALM) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 6, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 7, &self->tailAnimator, true, 0);

            self->state     = ERZGunner_State_Napalm;
            self->stateDraw = ERZGunner_Draw_Rocket;
        }
        else if (self->type == ERZGUNNER_DUD) {
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 8, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 9, &self->tailAnimator, true, 0);

            self->position.x = parent->position.x;
            self->drawFX     = FX_ROTATE;
            self->state      = ERZGunner_State_Dud_Active;
            self->velocity.x = (Zone->timer & 1) ? -0x40000 : 0x40000;
            self->stateDraw  = ERZGunner_Draw_Rocket;
        }
    }
}

void ERZGunner_State_Mortar(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->parachuteAnimator);
    RSDK.ProcessAnimation(&self->tailAnimator);

    if (self->parachuteAnimator.frameID) {
        self->drawFX = FX_ROTATE;
        if (self->velocity.y > 0xC000)
            self->velocity.y -= 0x3800;

        self->rotation = RSDK.Sin512(self->timer) >> 6;
        self->timer    = (self->timer + 4) & 0x1FF;
    }
    else {
        if (self->velocity.y < 0x40000)
            self->velocity.y += 0x3800;
    }

    self->position.y += self->velocity.y;

    ERZGunner_CheckPlayerMissileCollisions();

    if (self->classID) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == ERZGUNNER_NAPALM) {
                EntityERZGunner *napalm = CREATE_ENTITY(ERZGunner, INT_TO_VOID(ERZGUNNER_NAPALM_EXPLOSION), self->position.x, self->position.y);
                napalm->velocity.x      = self->velocity.y > 0x20000 ? 0x80000 : 0x40000;

                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(ERZGunner, INT_TO_VOID(ERZGUNNER_MORTAR_EXPLOSION), self->position.x, self->position.y);

                destroyEntity(self);
            }
        }
    }
}

void ERZGunner_State_Napalm(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);

    if (self->velocity.y < 0x40000)
        self->velocity.y += 0x1800;

    self->position.y += self->velocity.y;

    ERZGunner_CheckPlayerMissileCollisions();

    if (self->classID) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, true)) {
            if (self->type == ERZGUNNER_NAPALM) {
                EntityERZGunner *napalm = CREATE_ENTITY(ERZGunner, INT_TO_VOID(ERZGUNNER_NAPALM_EXPLOSION), self->position.x, self->position.y);
                napalm->velocity.x      = self->velocity.y > 0x20000 ? 0x80000 : 0x40000;

                destroyEntity(self);
            }
            else {
                CREATE_ENTITY(ERZGunner, INT_TO_VOID(ERZGUNNER_MORTAR_EXPLOSION), self->position.x, self->position.y);

                destroyEntity(self);
            }
        }
    }
}

void ERZGunner_State_Dud_Active(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);

    EntityERZGunner *parent = self->parent;

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

    ERZGunner_HandleDudExhaust();

    int32 angle = RSDK.ATan2(self->velocity.y, -self->velocity.x);
    ERZGunner_HandleRotations(2 * angle);

    if (self->timer == 320) {
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), self->position.x, self->position.y);
        explosion->interaction     = false;
        explosion->drawGroup       = Zone->objectDrawGroup[1];
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &ERZGunner->hitboxDud)) {
                self->angle     = self->rotation << 8;
                self->timer     = 0;
                self->drawFX    = FX_SCALE | FX_ROTATE;
                self->groundVel = 0x4000;
                self->scale.x   = 0x200;
                self->scale.y   = 0x200;
                self->state     = ERZGunner_State_Dud_HitByPlayer;
                RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
            }
        }
    }
}

void ERZGunner_State_Dud_HitByPlayer(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);

    self->angle += self->groundVel;
    self->rotation = self->angle >> 8;
    self->groundVel -= (self->groundVel >> 3);

    ERZGunner_HandleDudExhaust();

    if (self->groundVel < 128)
        self->state = ERZGunner_State_Dud_Malfunction;
}

void ERZGunner_State_Dud_Malfunction(void)
{
    RSDK_THIS(ERZGunner);

    RSDK.ProcessAnimation(&self->tailAnimator);

    EntityERZGunner *parent = self->parent;

    int32 angle = RSDK.ATan2((parent->position.y - self->position.y) >> 16, -((parent->position.x - self->position.x) >> 16));
    ERZGunner_HandleRotations(2 * angle);

    self->rotation &= 0x1FF;
    self->position.x -= RSDK.Sin512(self->rotation) << 9;
    self->position.y += RSDK.Cos512(self->rotation) << 9;
    self->scale.x -= 6;
    self->scale.y = self->scale.x;

    ERZGunner_HandleMalfunctionDudExhaust();

    if (self->scale.x < 0x80) {
        ERZGunner_Hit(parent);
        self->position.x = parent->position.x;
        self->position.y = parent->position.y;
        self->visible    = false;
        self->state      = ERZGunner_State_Dud_Explode;
    }
}

void ERZGunner_State_Dud_Explode(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer % 3) && !(Zone->timer & 4)) {
        int32 x                    = self->position.x + RSDK.Rand(-0x100000, 0x100000);
        int32 y                    = self->position.y + RSDK.Rand(-0x100000, 0x100000);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);

        explosion->interaction = false;
        explosion->drawGroup   = 1;
        explosion->scale       = self->scale;
    }

    if (++self->timer == 16)
        destroyEntity(self);
}

void ERZGunner_State_NapalmExplosion(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer & 3)) {
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->originPos.x - self->napalmExplosionPos, self->position.y);
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->originPos.x + self->napalmExplosionPos, self->position.y);
    }

    self->napalmExplosionPos += self->velocity.x;

    if (++self->timer == 16)
        destroyEntity(self);
}

void ERZGunner_State_MortarExplosion(void)
{
    RSDK_THIS(ERZGunner);

    if (!(Zone->timer & 3))
        CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_ITEMBOX), self->position.x, self->position.y);

    self->position.y -= 0x40000;

    if (++self->timer == 16)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void ERZGunner_EditorDraw(void)
{
    RSDK_THIS(ERZGunner);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 0, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(ERZGunner->aniFrames, 1, &self->fxAnimator, false, 0);

    self->originPos = self->position;
    self->screenPos = ScreenInfo->position;

    ERZGunner_Draw_Gunner();
}

void ERZGunner_EditorLoad(void) { ERZGunner->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomGunner.bin", SCOPE_STAGE); }
#endif

void ERZGunner_Serialize(void) {}
