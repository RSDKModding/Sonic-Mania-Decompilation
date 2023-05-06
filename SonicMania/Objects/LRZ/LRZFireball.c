// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZFireball Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 3, &self->animator, true, 0);
        self->state     = data;
        self->active    = ACTIVE_NORMAL;
        self->stateDraw = LRZFireball_Draw_Simple;
    }
    else {
        self->drawGroup = Zone->objectDrawGroup[0] + 1;

        switch (self->type) {
            case LRZFIREBALL_SPAWNER: self->state = LRZFireball_State_Spawner; break;

            case LRZFIREBALL_LAUNCHER_STATIC:
                RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0);
                self->state     = LRZFireball_State_LauncherStatic;
                self->stateDraw = LRZFireball_Draw_Simple;
                break;

            case LRZFIREBALL_LAUNCHER_GRAVITY:
                RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0);
                self->state     = LRZFireball_State_LauncherGravity;
                self->stateDraw = LRZFireball_Draw_Simple;
                break;
        }

        if (!SceneInfo->inEditor)
            self->groundVel <<= 7;
    }
}

void LRZFireball_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        LRZFireball->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LRZFireball.bin", SCOPE_STAGE);

    LRZFireball->hitboxFireball.left   = -6;
    LRZFireball->hitboxFireball.top    = -6;
    LRZFireball->hitboxFireball.right  = 6;
    LRZFireball->hitboxFireball.bottom = 6;

    LRZFireball->sfxFireball = RSDK.GetSfx("Stage/Fireball.wav");
}

void LRZFireball_CheckPlayerCollisions(void)
{
    RSDK_THIS(LRZFireball);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &LRZFireball->hitboxFireball)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }
}

void LRZFireball_CheckOffScreen(void)
{
    RSDK_THIS(LRZFireball);

    if (self->alpha > 0) {
        self->alpha -= 0x20;
        if (self->alpha <= 0) {
            destroyEntity(self);
            return;
        }
    }

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
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

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, RSDK.Sin512(0x200 - self->rotation) << 11,
                                 RSDK.Cos512(0x200 - self->rotation) << 11, false)
        && !self->alpha) {
        self->inkEffect = INK_ALPHA;
        self->alpha     = 0x100;
    }
}

void LRZFireball_State_Spawner(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityLRZFireball *fireball = CREATE_ENTITY(LRZFireball, LRZFireball_StateFireball_Spawner, self->position.x, self->position.y);

        fireball->angle    = self->rotation;
        fireball->rotation = self->rotation;

        fireball->groundVel  = -self->groundVel;
        fireball->velocity.x = fireball->groundVel * RSDK.Sin512(0x100 - fireball->angle);
        fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);

        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 0xFF);
    }
}

void LRZFireball_State_LauncherStatic(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityLRZFireball *fireball = CREATE_ENTITY(LRZFireball, LRZFireball_StateFireball_LauncherStatic, self->position.x, self->position.y);

        fireball->angle    = self->rotation;
        fireball->rotation = self->rotation;

        fireball->groundVel  = -self->groundVel;
        fireball->velocity.x = fireball->groundVel * RSDK.Sin512(0x100 - fireball->angle);
        fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);

        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 0xFF);
    }
}

void LRZFireball_State_LauncherGravity(void)
{
    RSDK_THIS(LRZFireball);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        EntityLRZFireball *fireball = CREATE_ENTITY(LRZFireball, LRZFireball_StateFireball_LauncherGravity, self->position.x, self->position.y);

        fireball->angle    = self->rotation;
        fireball->rotation = self->rotation;

        fireball->groundVel  = -self->groundVel;
        fireball->velocity.x = fireball->groundVel * RSDK.Sin512(0x100 - fireball->angle);
        fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);

        RSDK.PlaySfx(LRZFireball->sfxFireball, false, 0xFF);
    }
}

void LRZFireball_StateFireball_Spawner(void)
{
    RSDK_THIS(LRZFireball);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->groundVel += 0x18;
    self->velocity.x = self->groundVel * RSDK.Sin512(0x100 - self->angle);
    self->velocity.y = self->groundVel * RSDK.Cos512(0x100 - self->angle);

    if (self->groundVel > 0)
        self->rotation = self->angle + 0x100;

    LRZFireball_CheckOffScreen();
}

void LRZFireball_StateFireball_LauncherStatic(void)
{
    RSDK_THIS(LRZFireball);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    LRZFireball_CheckTileCollisions();
    LRZFireball_CheckOffScreen();
}

void LRZFireball_StateFireball_LauncherGravity(void)
{
    RSDK_THIS(LRZFireball);

    self->rotation = 2 * RSDK.ATan2(self->velocity.x >> 16, self->velocity.y >> 16) + 0x180;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    LRZFireball_CheckTileCollisions();
    LRZFireball_CheckOffScreen();
}

void LRZFireball_Draw_Simple(void)
{
    RSDK_THIS(LRZFireball);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void LRZFireball_EditorDraw(void)
{
    RSDK_THIS(LRZFireball);

    switch (self->type) {
        case LRZFIREBALL_SPAWNER: RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 3, &self->animator, true, 0); break;
        case LRZFIREBALL_LAUNCHER_STATIC: RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0); break;
        case LRZFIREBALL_LAUNCHER_GRAVITY: RSDK.SetSpriteAnimation(LRZFireball->aniFrames, 1, &self->animator, true, 0); break;
    }

    LRZFireball_Draw_Simple();
}

void LRZFireball_EditorLoad(void)
{
    LRZFireball->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LRZFireball.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LRZFireball, type);
    RSDK_ENUM_VAR("Spawner", LRZFIREBALL_SPAWNER);
    RSDK_ENUM_VAR("Launcher (Static Movement)", LRZFIREBALL_LAUNCHER_STATIC);
    RSDK_ENUM_VAR("Launcher (Gravity-Applied Movement)", LRZFIREBALL_LAUNCHER_GRAVITY);
}
#endif

void LRZFireball_Serialize(void)
{
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_INT32, rotation);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LRZFireball, VAR_ENUM, groundVel);
}
