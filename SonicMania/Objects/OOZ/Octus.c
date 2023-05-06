// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Octus Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOctus *Octus;

void Octus_Update(void)
{
    RSDK_THIS(Octus);

    StateMachine_Run(self->state);
}

void Octus_LateUpdate(void) {}

void Octus_StaticUpdate(void) {}

void Octus_Draw(void)
{
    RSDK_THIS(Octus);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Octus_Create(void *data)
{
    RSDK_THIS(Octus);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->drawFX |= FX_FLIP;
    self->startPos = self->position;
    self->startDir = self->direction;
    self->timer    = 128;
    self->unused1  = 0;
    self->unused2  = 0;

    if (data) {
        self->inkEffect     = INK_ADD;
        self->alpha         = 0xC0;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x200000;
        self->updateRange.y = 0x200000;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 3, &self->animator, true, 0);
        self->state = Octus_State_Shot;
    }
    else {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &self->animator, true, 0);
        self->state = Octus_State_Init;
        self->alpha = 0xC0;
    }
}

void Octus_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("OOZ1") || RSDK.CheckSceneFolder("OOZ2"))
        Octus->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Octus.bin", SCOPE_STAGE);

    Octus->hitboxBadnik.left   = -16;
    Octus->hitboxBadnik.top    = -12;
    Octus->hitboxBadnik.right  = 16;
    Octus->hitboxBadnik.bottom = 12;

    Octus->hitboxRange.left   = -128;
    Octus->hitboxRange.top    = -512;
    Octus->hitboxRange.right  = 128;
    Octus->hitboxRange.bottom = 512;

    Octus->hitboxProjectile.left   = -4;
    Octus->hitboxProjectile.top    = -4;
    Octus->hitboxProjectile.right  = 4;
    Octus->hitboxProjectile.bottom = 4;

    Octus->sfxShot = RSDK.GetSfx("Stage/Shot.wav");

    DEBUGMODE_ADD_OBJ(Octus);
}

void Octus_DebugSpawn(void)
{
    RSDK_THIS(Octus);

    EntityOctus *octus = CREATE_ENTITY(Octus, NULL, self->position.x, self->position.y);
    octus->direction   = self->direction;
    octus->startDir    = self->direction;
}

void Octus_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Octus_CheckPlayerCollisions(void)
{
    RSDK_THIS(Octus);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Octus->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Octus_CheckOffScreen(void)
{
    RSDK_THIS(Octus);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Octus_Create(NULL);
    }
}

void Octus_State_Init(void)
{
    RSDK_THIS(Octus);

    self->active = ACTIVE_NORMAL;

    self->state = Octus_State_CheckPlayerInRange;
    Octus_State_CheckPlayerInRange();
}

void Octus_State_CheckPlayerInRange(void)
{
    RSDK_THIS(Octus);

    RSDK.ProcessAnimation(&self->animator);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Octus->hitboxRange)) {
            self->timer = 32;
            RSDK.SetSpriteAnimation(Octus->aniFrames, 1, &self->animator, true, 0);
            self->state = Octus_State_JumpDelay;
            foreach_break;
        }
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOffScreen();
}

void Octus_State_JumpDelay(void)
{
    RSDK_THIS(Octus);

    RSDK.ProcessAnimation(&self->animator);

    if (--self->timer <= 0) {
        self->velocity.y = -0x20000;
        self->state      = Octus_State_Jump;
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOffScreen();
}

void Octus_State_Jump(void)
{
    RSDK_THIS(Octus);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1000;

    if (self->velocity.y >= 0) {
        self->timer = 60;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 2, &self->animator, true, 0);
        self->state = Octus_State_Shoot;
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOffScreen();
}

void Octus_State_Shoot(void)
{
    RSDK_THIS(Octus);

    RSDK.ProcessAnimation(&self->animator);

    if (--self->timer == 51) {
        EntityOctus *shot = CREATE_ENTITY(Octus, INT_TO_VOID(true), self->position.x, self->position.y);
        if (self->direction) {
            shot->position.x += 0xE0000;
            shot->velocity.x = 0x20000;
        }
        else {
            shot->position.x -= 0xE0000;
            shot->velocity.x = -0x20000;
        }

        RSDK.PlaySfx(Octus->sfxShot, false, 255);
    }
    else if (self->timer <= 0) {
        self->state = Octus_State_Fall;
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOffScreen();
}

void Octus_State_Fall(void)
{
    RSDK_THIS(Octus);

    RSDK.ProcessAnimation(&self->animator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1000;

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xD0000, 1)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Octus->aniFrames, 0, &self->animator, true, 0);
        self->state = Octus_State_CheckPlayerInRange;
    }

    Octus_CheckPlayerCollisions();
    Octus_CheckOffScreen();
}

void Octus_State_Shot(void)
{
    RSDK_THIS(Octus);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ProcessAnimation(&self->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Octus->hitboxProjectile))
                Player_ProjectileHurt(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void Octus_EditorDraw(void) { Octus_Draw(); }

void Octus_EditorLoad(void)
{
    Octus->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Octus.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Octus, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Octus_Serialize(void) { RSDK_EDITABLE_VAR(Octus, VAR_UINT8, direction); }
