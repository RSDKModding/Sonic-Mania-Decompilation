// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Bloominator Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectBloominator *Bloominator;

void Bloominator_Update(void)
{
    RSDK_THIS(Bloominator);
    StateMachine_Run(self->state);
}

void Bloominator_LateUpdate(void) {}

void Bloominator_StaticUpdate(void) {}

void Bloominator_Draw(void)
{
    RSDK_THIS(Bloominator);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Bloominator_Create(void *data)
{
    RSDK_THIS(Bloominator);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (data) {
        --self->drawGroup;
        RSDK.SetSpriteAnimation(Bloominator->aniFrames, 2, &self->animator, true, 0);
        self->state = Bloominator_State_Spikeball;
    }
    else {
        RSDK.SetSpriteAnimation(Bloominator->aniFrames, 0, &self->animator, true, 0);
        self->state = Bloominator_State_Init;
    }
}

void Bloominator_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("AIZ"))
        Bloominator->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Bloominator.bin", SCOPE_STAGE);

    Bloominator->hitboxBadnik.left   = -12;
    Bloominator->hitboxBadnik.top    = -20;
    Bloominator->hitboxBadnik.right  = 12;
    Bloominator->hitboxBadnik.bottom = 20;

    Bloominator->hitboxProjectile.left   = -4;
    Bloominator->hitboxProjectile.top    = -4;
    Bloominator->hitboxProjectile.right  = 4;
    Bloominator->hitboxProjectile.bottom = 4;

    Bloominator->sfxShot = RSDK.GetSfx("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Bloominator);
}

void Bloominator_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bloominator->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Bloominator_DebugSpawn(void)
{
    RSDK_THIS(Bloominator);

    CREATE_ENTITY(Bloominator, NULL, self->position.x, self->position.y);
}

void Bloominator_CheckPlayerCollisions(void)
{
    RSDK_THIS(Bloominator);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Bloominator->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Bloominator_CheckOffScreen(void)
{
    RSDK_THIS(Bloominator);

    if (!RSDK.CheckOnScreen(self, NULL))
        Bloominator_Create(NULL);
}

void Bloominator_State_Init(void)
{
    RSDK_THIS(Bloominator);

    self->active = ACTIVE_NORMAL;
    self->timer  = 0;

    self->state = Bloominator_State_Idle;
    Bloominator_State_Idle();
}

void Bloominator_State_Idle(void)
{
    RSDK_THIS(Bloominator);

    if (self->onScreen) {
        if (++self->timer >= 60) {
            self->timer = 0;
            RSDK.SetSpriteAnimation(Bloominator->aniFrames, 1, &self->animator, true, 0);
            self->state = Bloominator_State_Firing;
        }
    }

    RSDK.ProcessAnimation(&self->animator);

    Bloominator_CheckPlayerCollisions();
    Bloominator_CheckOffScreen();
}

void Bloominator_State_Firing(void)
{
    RSDK_THIS(Bloominator);
    EntityBloominator *spikeBall = NULL;

    switch (++self->timer) {
        case 15:
            spikeBall             = CREATE_ENTITY(Bloominator, INT_TO_VOID(true), self->position.x - 0x10000, self->position.y - 0x160000);
            spikeBall->velocity.x = -0x10000;
            spikeBall->velocity.y = -0x50000;
            RSDK.PlaySfx(Bloominator->sfxShot, false, 255);
            break;

        case 45:
            spikeBall             = CREATE_ENTITY(Bloominator, INT_TO_VOID(true), self->position.x - 0x10000, self->position.y - 0x160000);
            spikeBall->velocity.x = 0x10000;
            spikeBall->velocity.y = -0x50000;
            RSDK.PlaySfx(Bloominator->sfxShot, false, 255);
            break;

        case 50:
            self->timer = -60;
            RSDK.SetSpriteAnimation(Bloominator->aniFrames, 0, &self->animator, true, 0);
            self->state = Bloominator_State_Idle;
            break;
    }

    RSDK.ProcessAnimation(&self->animator);

    Bloominator_CheckPlayerCollisions();
    Bloominator_CheckOffScreen();
}

void Bloominator_State_Spikeball(void)
{
    RSDK_THIS(Bloominator);

    if (RSDK.CheckOnScreen(self, NULL)) {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
        self->velocity.y += 0x3800;

        RSDK.ProcessAnimation(&self->animator);

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Bloominator->hitboxProjectile)) {
                Player_ProjectileHurt(player, self);
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Bloominator_EditorDraw(void) { Bloominator_Draw(); }

void Bloominator_EditorLoad(void) { Bloominator->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Bloominator.bin", SCOPE_STAGE); }
#endif

void Bloominator_Serialize(void) {}
#endif
