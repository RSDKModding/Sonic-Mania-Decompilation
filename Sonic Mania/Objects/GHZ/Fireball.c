#include "SonicMania.h"

ObjectFireball *Fireball = NULL;

void Fireball_Update(void)
{
    RSDK_THIS(Fireball);
    StateMachine_Run(self->state);
}

void Fireball_LateUpdate(void) {}

void Fireball_StaticUpdate(void) {}

void Fireball_Draw(void)
{
    RSDK_THIS(Fireball);
    StateMachine_Run(self->stateDraw);
}

void Fireball_Create(void *data)
{
    RSDK_THIS(Fireball);
    self->drawFX |= FX_ROTATE | FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawOrder     = Zone->drawOrderLow;
    RSDK.SetSpriteAnimation(Fireball->aniFrames, 0, &self->animator, true, 0);
    self->interval *= 15;
    if (data) {
        self->state     = (Type_StateMachine)data;
        self->active    = ACTIVE_NORMAL;
        self->stateDraw = Fireball_StateDraw_Normal;
    }
    else {
        switch (self->type) {
            default: self->groundVel <<= 7;
            case 0: self->state = Fireball_State_Unknown1; break;
            case 1:
                self->groundVel <<= 7;
                self->state = Fireball_State_Unknown2;
                break;
            case 2:
                self->groundVel <<= 7;
                self->state = Fireball_State_Unknown3;
                break;
        }
    }
}

void Fireball_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Fireball->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE);
    Fireball->hitbox.left   = -6;
    Fireball->hitbox.top    = -6;
    Fireball->hitbox.right  = 6;
    Fireball->hitbox.bottom = 6;
    Fireball->sfxFireball   = RSDK.GetSFX("Stage/Fireball.wav");
}

void Fireball_HandlePlayerInteractions(void)
{
    RSDK_THIS(Fireball);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Fireball->hitbox)) {
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }
}

void Fireball_State_Unknown1(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Unknown4, self->position.x, self->position.y);
        fireball->angle          = self->rotation;
        fireball->rotation       = self->rotation;
        fireball->groundVel      = -self->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(256 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(256 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Unknown2(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Unknown5, self->position.x, self->position.y);
        fireball->angle          = self->rotation;
        fireball->rotation       = self->rotation;
        fireball->groundVel      = -self->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(256 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(256 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Unknown3(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Unknown6, self->position.x, self->position.y);
        fireball->angle          = self->rotation;
        fireball->rotation       = self->rotation;
        fireball->groundVel      = -self->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(256 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(256 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Unknown4(void)
{
    RSDK_THIS(Fireball);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->groundVel += 24;
    self->velocity.x = self->groundVel * RSDK.Sin512(256 - self->angle);
    self->velocity.y = self->groundVel * RSDK.Cos512(256 - self->angle);
    if (self->groundVel > 0)
        self->rotation = self->angle + 256;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
    RSDK.ProcessAnimation(&self->animator);
    Fireball_HandlePlayerInteractions();
}

void Fireball_State_Unknown5(void)
{
    RSDK_THIS(Fireball);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(512 - self->rotation) << 10,
                                 RSDK.Cos512(512 - self->rotation) << 10, true)) {
        self->state = Fireball_State_Unknown7;
        RSDK.SetSpriteAnimation(Fireball->aniFrames, 1, &self->animator, true, 0);
    }
    else {
        if (RSDK.CheckOnScreen(self, &self->updateRange)) {
            RSDK.ProcessAnimation(&self->animator);
            Fireball_HandlePlayerInteractions();
        }
        else {
            destroyEntity(self);
        }
    }
}

void Fireball_State_Unknown6(void)
{
    RSDK_THIS(Fireball);
    self->rotation = 2 * RSDK.ATan2((self->velocity.x >> 16), (self->velocity.y >> 16)) + 384;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(512 - self->rotation) << 10,
                                 RSDK.Cos512(512 - self->rotation) << 10, true)) {
        self->state = Fireball_State_Unknown7;
        RSDK.SetSpriteAnimation(Fireball->aniFrames, 1, &self->animator, true, 0);
    }
    else {
        if (RSDK.CheckOnScreen(self, &self->updateRange)) {
            RSDK.ProcessAnimation(&self->animator);
            Fireball_HandlePlayerInteractions();
        }
        else {
            destroyEntity(self);
        }
    }
}

void Fireball_State_Unknown7(void)
{
    RSDK_THIS(Fireball);
    RSDK.ProcessAnimation(&self->animator);
    Fireball_HandlePlayerInteractions();
    if (self->animator.frameID == 2)
        destroyEntity(self);
}

void Fireball_StateDraw_Normal(void)
{
    RSDK_THIS(Fireball);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void Fireball_EditorDraw(void)
{
    RSDK_THIS(Fireball);
    RSDK.SetSpriteAnimation(Fireball->aniFrames, 0, &self->animator, true, 0);
    self->stateDraw = Fireball_StateDraw_Normal;
}

void Fireball_EditorLoad(void) { Fireball->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE); }
#endif

void Fireball_Serialize(void)
{
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Fireball, VAR_INT32, rotation);
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(Fireball, VAR_ENUM, groundVel);
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, intervalOffset);
}
