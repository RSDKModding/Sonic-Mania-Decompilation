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
    if (!SceneInfo->inEditor)
        self->interval *= 15;
    if (data) {
        self->state     = (Type_StateMachine)data;
        self->active    = ACTIVE_NORMAL;
        self->stateDraw = Fireball_StateDraw_Normal;
    }
    else {
        self->groundVel <<= 7;
        switch (self->type) {
            default: break;
            case FIREBALL_UPDOWN: self->state = Fireball_State_UpDown_Spawner; break;
            case FIREBALL_MOVE: self->state = Fireball_State_Move_Spawner; break;
            case FIREBALL_MOVEGRAVITY: self->state = Fireball_State_MoveGravity_Spawner; break;
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
    Fireball->sfxFireball   = RSDK.GetSfx("Stage/Fireball.wav");
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

void Fireball_State_UpDown_Spawner(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_UpDown, self->position.x, self->position.y);
        fireball->angle          = self->rotation;
        fireball->rotation       = self->rotation;
        fireball->groundVel      = -self->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(0x100 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_Move_Spawner(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_Move, self->position.x, self->position.y);
        fireball->angle          = self->rotation;
        fireball->rotation       = self->rotation;
        fireball->groundVel      = -self->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(0x100 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_MoveGravity_Spawner(void)
{
    RSDK_THIS(Fireball);
    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityFireball *fireball = CREATE_ENTITY(Fireball, Fireball_State_MoveGravity, self->position.x, self->position.y);
        fireball->angle          = self->rotation;
        fireball->rotation       = self->rotation;
        fireball->groundVel      = -self->groundVel;
        fireball->velocity.x     = fireball->groundVel * RSDK.Sin512(0x100 - fireball->angle);
        fireball->velocity.y     = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);
        RSDK.PlaySfx(Fireball->sfxFireball, false, 255);
    }
}

void Fireball_State_UpDown(void)
{
    RSDK_THIS(Fireball);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->groundVel += 24;
    self->velocity.x = self->groundVel * RSDK.Sin512(0x100 - self->angle);
    self->velocity.y = self->groundVel * RSDK.Cos512(0x100 - self->angle);
    if (self->groundVel > 0)
        self->rotation = self->angle + 256;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
    RSDK.ProcessAnimation(&self->animator);
    Fireball_HandlePlayerInteractions();
}

void Fireball_State_Move(void)
{
    RSDK_THIS(Fireball);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(512 - self->rotation) << 10,
                                 RSDK.Cos512(512 - self->rotation) << 10, true)) {
        self->state = Fireball_State_Dissipate;
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

void Fireball_State_MoveGravity(void)
{
    RSDK_THIS(Fireball);
    self->rotation = 2 * RSDK.ATan2((self->velocity.x >> 16), (self->velocity.y >> 16)) + 0x180;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, RSDK.Sin512(512 - self->rotation) << 10,
                                 RSDK.Cos512(512 - self->rotation) << 10, true)) {
        self->state = Fireball_State_Dissipate;
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

void Fireball_State_Dissipate(void)
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

void Fireball_EditorLoad(void)
{
    Fireball->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Fireball.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Fireball, type);
    RSDK_ENUM_VAR("Up Then Down", FIREBALL_UPDOWN);
    RSDK_ENUM_VAR("Move", FIREBALL_MOVE);
    RSDK_ENUM_VAR("Move (With Applied Gravity)", FIREBALL_MOVEGRAVITY);
}
#endif

void Fireball_Serialize(void)
{
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Fireball, VAR_INT32, rotation);
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(Fireball, VAR_ENUM, groundVel);
    RSDK_EDITABLE_VAR(Fireball, VAR_UINT8, intervalOffset);
}
