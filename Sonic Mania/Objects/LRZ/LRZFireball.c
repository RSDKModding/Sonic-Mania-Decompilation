#include "SonicMania.h"

ObjectLRZFireball *LRZFireball;

void LRZFireball_Update(void)
{
    RSDK_THIS(LRZFireball);
    StateMachine_Run(self->state);
}

void LRZFireball_LateUpdate(void) {}

void LRZFireball_StaticUpdate(void) {}

void LRZFireball_Draw(void)
{
    RSDK_THIS(LRZFireball);
    StateMachine_Run(self->stateDraw);
}

void LRZFireball_Create(void *data)
{
    RSDK_THIS(LRZFireball);

    self->drawFX |= FX_FLIP | FX_ROTATE;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (data) {
        self->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 3, &self->animator, true, 0);
        self->state     = data;
        self->active    = ACTIVE_NORMAL;
        self->stateDraw = LRZFireball_StateDraw_Visible;
    }
    else {
        self->drawOrder = Zone->drawOrderLow + 1;

        switch (self->type) {
            case 0: self->state = LRZFireball_State_Type0; break;
            case 1:
                RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0);
                self->state     = LRZFireball_State_Type1;
                self->stateDraw = LRZFireball_StateDraw_Visible;
                break;
            case 2:
                RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0);
                self->state     = LRZFireball_State_Type2;
                self->stateDraw = LRZFireball_StateDraw_Visible;
                break;
        }

        if (!SceneInfo->inEditor)
            self->groundVel <<= 7;
    }
}

void LRZFireball_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        LRZFireball->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LRZFireball.bin", SCOPE_STAGE);
    LRZFireball->hitbox.left   = -6;
    LRZFireball->hitbox.top    = -6;
    LRZFireball->hitbox.right  = 6;
    LRZFireball->hitbox.bottom = 6;
    LRZFireball->sfxFireball   = RSDK.GetSfx("Stage/Fireball.wav");
}

void LRZFireball_CheckPlayerCollisions(void)
{
    RSDK_THIS(LRZFireball);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &LRZFireball->hitbox)) {
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }
}

void LRZFireball_CheckOnScreen(void)
{
    RSDK_THIS(LRZFireball);

    self->alpha -= 0x20;
    if (self->alpha <= 0 || (self->alpha && RSDK.CheckOnScreen(self, &self->updateRange))) {
        RSDK.ProcessAnimation(&self->animator);
        LRZFireball_CheckPlayerCollisions();
    }
    else {
        destroyEntity(self);
    }
}

void LRZFireball_CheckTileCollisions(void)
{
    RSDK_THIS(LRZFireball);

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(0x200 - self->rotation) << 11,
                                 RSDK.Cos512(0x200 - self->rotation) << 11, false)
        && !self->alpha) {
        self->inkEffect = INK_ALPHA;
        self->alpha     = 0x100;
    }
}

void LRZFireball_State_Type0(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityLRZFireball *child = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type0, self->position.x, self->position.y);

        child->angle      = self->rotation;
        child->rotation   = self->rotation;
        child->groundVel  = -self->groundVel;
        child->velocity.x = child->groundVel * RSDK.Sin512(0x100 - child->angle);
        child->velocity.y = child->groundVel * RSDK.Cos512(0x100 - child->angle);
        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
    }
}

void LRZFireball_State_Type1(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityLRZFireball *child = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type1, self->position.x, self->position.y);

        child->angle      = self->rotation;
        child->rotation   = self->rotation;
        child->groundVel  = -self->groundVel;
        child->velocity.x = child->groundVel * RSDK.Sin512(0x100 - child->angle);
        child->velocity.y = child->groundVel * RSDK.Cos512(0x100 - child->angle);
        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
    }
}

void LRZFireball_State_Type2(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityLRZFireball *child = CREATE_ENTITY(LRZFireball, LRZFireball_StateChild_Type2, self->position.x, self->position.y);

        child->angle      = self->rotation;
        child->rotation   = self->rotation;
        child->groundVel  = -self->groundVel;
        child->velocity.x = child->groundVel * RSDK.Sin512(0x100 - child->angle);
        child->velocity.y = child->groundVel * RSDK.Cos512(0x100 - child->angle);
        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 255);
    }
}

void LRZFireball_StateChild_Type0(void)
{
    RSDK_THIS(LRZFireball);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->groundVel += 0x18;
    self->velocity.x = self->groundVel * RSDK.Sin512(0x100 - self->angle);
    self->velocity.y = self->groundVel * RSDK.Cos512(0x100 - self->angle);
    if (self->groundVel > 0)
        self->rotation = self->angle + 0x100;

    LRZFireball_CheckOnScreen();
}

void LRZFireball_StateChild_Type1(void)
{
    RSDK_THIS(LRZFireball);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    LRZFireball_CheckTileCollisions();
    LRZFireball_CheckOnScreen();
}

void LRZFireball_StateChild_Type2(void)
{
    RSDK_THIS(LRZFireball);

    self->rotation = 2 * RSDK.ATan2(self->velocity.x >> 16, self->velocity.y >> 16) + 0x180;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    LRZFireball_CheckTileCollisions();
    LRZFireball_CheckOnScreen();
}

void LRZFireball_StateDraw_Visible(void)
{
    RSDK_THIS(LRZFireball);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void LRZFireball_EditorDraw(void)
{
    RSDK_THIS(LRZFireball);

    switch (self->type) {
        case 0: RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 3, &self->animator, true, 0); break;
        case 1:
            RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0);
            self->stateDraw = LRZFireball_StateDraw_Visible;
            break;
        case 2:
            RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0);
            self->stateDraw = LRZFireball_StateDraw_Visible;
            break;
    }

    LRZFireball_StateDraw_Visible();

    // TODO: transparent fireball strength indicator
}

void LRZFireball_EditorLoad(void) { LRZFireball->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LRZFireball.bin", SCOPE_STAGE); }
#endif

void LRZFireball_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_INT32, rotation);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_ENUM, groundVel);
}
