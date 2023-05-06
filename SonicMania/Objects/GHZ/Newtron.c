// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Newtron Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectNewtron *Newtron;

void Newtron_Update(void)
{
    RSDK_THIS(Newtron);
    StateMachine_Run(self->state);
}

void Newtron_LateUpdate(void) {}

void Newtron_StaticUpdate(void) {}

void Newtron_Draw(void)
{
    RSDK_THIS(Newtron);
    RSDK.DrawSprite(&self->animator, NULL, false);
    if (self->state == Newtron_State_Fly)
        RSDK.DrawSprite(&self->flameAnimator, NULL, false);
}

void Newtron_Create(void *data)
{
    RSDK_THIS(Newtron);
    if (!SceneInfo->inEditor) {
        self->drawFX |= FX_FLIP;
        self->startPos = self->position;
        self->visible  = true;

        if (data == INT_TO_VOID(NEWTRON_FLY)) {
            self->type          = NEWTRON_FLY;
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
        }
        else if (data) {
            self->inkEffect = INK_ADD;
            self->alpha     = 0xC0;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 6, &self->animator, true, 0);
            self->state         = Newtron_State_Projectile;
            self->active        = ACTIVE_NORMAL;
            self->updateRange.x = 0x200000;
            self->updateRange.y = 0x200000;
            self->drawGroup     = Zone->objectDrawGroup[1];
            return;
        }

        self->inkEffect     = INK_ALPHA;
        self->alpha         = 0x00;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        if (self->type == NEWTRON_FLY) {
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 2, &self->animator, true, 0);
            self->drawGroup = Zone->objectDrawGroup[0];
        }
        else {
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &self->animator, true, 0);
            self->drawGroup = Zone->objectDrawGroup[1];
        }
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 5, &self->flameAnimator, true, 0);
        self->state = Newtron_State_Init;
    }
}

void Newtron_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Newtron->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Newtron.bin", SCOPE_STAGE);

    Newtron->hitboxShoot.left   = -12;
    Newtron->hitboxShoot.top    = -14;
    Newtron->hitboxShoot.right  = 12;
    Newtron->hitboxShoot.bottom = 14;

    // hitbox for the flying variant
    // goes unused in this object because..... ???
    // at least GHZ/CheckerBall uses it
    Newtron->hitboxFly.left   = -16;
    Newtron->hitboxFly.top    = -8;
    Newtron->hitboxFly.right  = 16;
    Newtron->hitboxFly.bottom = 8;

    Newtron->hitboxProjectile.left   = -6;
    Newtron->hitboxProjectile.top    = -6;
    Newtron->hitboxProjectile.right  = 6;
    Newtron->hitboxProjectile.bottom = 6;

    Newtron->hitboxRange.left   = -128;
    Newtron->hitboxRange.top    = -64;
    Newtron->hitboxRange.right  = 128;
    Newtron->hitboxRange.bottom = 64;

    DEBUGMODE_ADD_OBJ(Newtron);
}

void Newtron_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Newtron_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Newtron, NULL, self->position.x, self->position.y);
}

void Newtron_CheckPlayerCollisions(void)
{
    RSDK_THIS(Newtron);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Newtron->hitboxShoot))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Newtron_CheckOffScreen(void)
{
    RSDK_THIS(Newtron);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position   = self->startPos;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->timer      = 0;
        Newtron_Create(NULL);
    }
}

void Newtron_GetTargetDir(void)
{
    RSDK_THIS(Newtron);

    EntityPlayer *targetPlayer = NULL;
    foreach_active(Player, player)
    {
        if (targetPlayer) {
            if (abs(player->position.x - self->position.x) < abs(targetPlayer->position.x - self->position.x))
                targetPlayer = player;
        }
        else {
            targetPlayer = player;
        }
    }

    if (targetPlayer)
        self->direction = targetPlayer->position.x < self->position.x;
}

void Newtron_State_Init(void)
{
    RSDK_THIS(Newtron);

    self->active = ACTIVE_NORMAL;

    self->state = Newtron_State_CheckPlayerInRange;
    Newtron_State_CheckPlayerInRange();
}

void Newtron_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Newtron);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Newtron->hitboxRange))
            self->state = Newtron_State_Appear;
    }

    Newtron_CheckOffScreen();
}

void Newtron_State_Appear(void)
{
    RSDK_THIS(Newtron);

    if (self->alpha >= 0xF8) {
        self->alpha = 0xFF;
        if (self->type == NEWTRON_FLY) {
            self->state = Newtron_State_StartFly;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 3, &self->animator, true, 0);
        }
        else
            self->state = Newtron_State_Shoot;
    }
    else {
        self->alpha += 4;
        if (self->type == NEWTRON_FLY)
            Newtron_GetTargetDir();
    }

    RSDK.ProcessAnimation(&self->animator);

    Newtron_CheckOffScreen();
}

void Newtron_State_StartFly(void)
{
    RSDK_THIS(Newtron);

    Newtron_GetTargetDir();

    if (self->animator.frameID >= 2) {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;

        if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8)) {
            self->velocity.y = 0;
            if (self->direction == FLIP_NONE)
                self->velocity.x = 0x20000;
            else
                self->velocity.x = -0x20000;
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 4, &self->animator, true, 0);
            self->state = Newtron_State_Fly;
        }
    }

    RSDK.ProcessAnimation(&self->animator);

    Newtron_CheckPlayerCollisions();
    Newtron_CheckOffScreen();
}

void Newtron_State_Fly(void)
{
    RSDK_THIS(Newtron);

    self->position.x += self->velocity.x;
    RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, 8);

    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->flameAnimator);

    Newtron_CheckPlayerCollisions();
    Newtron_CheckOffScreen();
}

void Newtron_State_Shoot(void)
{
    RSDK_THIS(Newtron);

    switch (++self->timer) {
        case 30:
            RSDK.SetSpriteAnimation(Newtron->aniFrames, 1, &self->animator, true, 0);
            if (self->direction)
                CREATE_ENTITY(Newtron, INT_TO_VOID(NEWTRON_PROJECTILE), self->position.x - 0x140000, self->position.y - 0x80000)->velocity.x =
                    -0x20000;
            else
                CREATE_ENTITY(Newtron, INT_TO_VOID(NEWTRON_PROJECTILE), self->position.x + 0x140000, self->position.y - 0x80000)->velocity.x =
                    0x20000;
            break;

        case 45: RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &self->animator, true, 0); break;

        case 90: self->state = Newtron_State_FadeAway; break;
    }

    RSDK.ProcessAnimation(&self->animator);

    Newtron_CheckPlayerCollisions();
    Newtron_CheckOffScreen();
}

void Newtron_State_FadeAway(void)
{
    RSDK_THIS(Newtron);

    RSDK.ProcessAnimation(&self->animator);
    Newtron_CheckOffScreen();

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 4;
}

void Newtron_State_Projectile(void)
{
    RSDK_THIS(Newtron);

    self->position.x += self->velocity.x;

    if (!RSDK.CheckOnScreen(self, NULL)) {
        destroyEntity(self);
    }
    else {
        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Newtron->hitboxProjectile))
                Player_ProjectileHurt(player, self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Newtron_EditorDraw(void)
{
    RSDK_THIS(Newtron);

    self->drawFX = FX_FLIP;
    if (self->type == NEWTRON_FLY)
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 2, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Newtron->aniFrames, 0, &self->animator, true, 0);
    RSDK.SetSpriteAnimation(Newtron->aniFrames, 5, &self->flameAnimator, true, 0);

    Newtron_Draw();
}

void Newtron_EditorLoad(void)
{
    Newtron->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Newtron.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Newtron, type);
    RSDK_ENUM_VAR("Shoot", NEWTRON_SHOOT);
    RSDK_ENUM_VAR("Fly", NEWTRON_FLY);

    // Only for "Shoot" Variant, fly variant direction is based on the target's position
    RSDK_ACTIVE_VAR(Newtron, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Newtron_Serialize(void)
{
    RSDK_EDITABLE_VAR(Newtron, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Newtron, VAR_UINT8, direction);
}
