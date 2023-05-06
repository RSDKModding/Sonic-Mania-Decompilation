// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Motobug Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMotobug *Motobug;

void Motobug_Update(void)
{
    RSDK_THIS(Motobug);
    StateMachine_Run(self->state);
}

void Motobug_LateUpdate(void) {}

void Motobug_StaticUpdate(void) {}

void Motobug_Draw(void)
{
    RSDK_THIS(Motobug);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Motobug_Create(void *data)
{
    RSDK_THIS(Motobug);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->wasTurning    = true;

    if (data) {
        RSDK.SetSpriteAnimation(Motobug->aniFrames, 3, &self->animator, true, 0);
        self->state = Motobug_State_Smoke;
    }
    else {
        self->timer = 16;
        RSDK.SetSpriteAnimation(Motobug->aniFrames, 0, &self->animator, true, 0);
        self->state = Motobug_State_Init;
    }
}

void Motobug_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Motobug->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Motobug.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        Motobug->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Motobug.bin", SCOPE_STAGE);

    Motobug->hitboxBadnik.left   = -14;
    Motobug->hitboxBadnik.top    = -14;
    Motobug->hitboxBadnik.right  = 14;
    Motobug->hitboxBadnik.bottom = 14;

    DEBUGMODE_ADD_OBJ(Motobug);
}

void Motobug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Motobug->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void Motobug_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Motobug, NULL, self->position.x, self->position.y);
}
void Motobug_CheckOffScreen(void)
{
    RSDK_THIS(Motobug);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Motobug_Create(NULL);
    }
}
void Motobug_CheckPlayerCollisions(void)
{
    RSDK_THIS(Motobug);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Motobug->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}
void Motobug_State_Fall(void)
{
    RSDK_THIS(Motobug);

    RSDK.ProcessAnimation(&self->animator);

    if (self->wasTurning)
        self->position.x += self->velocity.x;

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        self->wasTurning = true;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Motobug->aniFrames, 0, &self->animator, true, 0);
        self->state = Motobug_State_Move;
        Motobug_State_Move();
    }
    else {
        Motobug_CheckPlayerCollisions();
        Motobug_CheckOffScreen();
    }
}
void Motobug_State_Move(void)
{
    RSDK_THIS(Motobug);

    self->position.x += self->velocity.x;

    if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8)) {
        RSDK.SetSpriteAnimation(Motobug->aniFrames, 1, &self->animator, true, 0);
        self->turnTimer = 0;

        bool32 collided = false;
        if (self->direction)
            collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x10000, 0xF0000, 8);
        else
            collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x10000, 0xF0000, 8);

        if (collided)
            self->state = Motobug_State_Idle;
        else
            self->state = Motobug_State_Fall;
    }

    if (!--self->timer) {
        self->timer          = 16;
        EntityMotobug *smoke = CREATE_ENTITY(Motobug, INT_TO_VOID(true), self->position.x, self->position.y);
        if (self->direction == FLIP_X)
            smoke->position.x -= 0x140000;
        else
            smoke->position.x += 0x140000;
    }

    RSDK.ProcessAnimation(&self->animator);

    Motobug_CheckPlayerCollisions();
    Motobug_CheckOffScreen();
}
void Motobug_State_Idle(void)
{
    RSDK_THIS(Motobug);

    RSDK.ProcessAnimation(&self->animator);

    bool32 collided = false;
    if (self->direction)
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x10000, 0xF0000, 8);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x10000, 0xF0000, 8);

    if (collided) {
        ++self->turnTimer;
        if (self->turnTimer == 30) {
            RSDK.SetSpriteAnimation(Motobug->aniFrames, 2, &self->animator, true, 0);
            self->state = Motobug_State_Turn;
        }
    }
    else {
        self->wasTurning = false;
        RSDK.SetSpriteAnimation(Motobug->aniFrames, 0, &self->animator, true, 0);
        self->state = Motobug_State_Fall;
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    Motobug_CheckPlayerCollisions();
    Motobug_CheckOffScreen();
}
void Motobug_State_Init(void)
{
    RSDK_THIS(Motobug);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;
    self->velocity.y = 0;

    self->state = Motobug_State_Move;
    Motobug_State_Move();
}
void Motobug_State_Smoke(void)
{
    RSDK_THIS(Motobug);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}
void Motobug_State_Turn(void)
{
    RSDK_THIS(Motobug);

    RSDK.ProcessAnimation(&self->animator);

    bool32 collided = false;
    if (self->direction)
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x10000, 0xF0000, 8);
    else
        collided = RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x10000, 0xF0000, 8);

    if (collided) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(Motobug->aniFrames, 0, &self->animator, true, 0);
            self->direction ^= FLIP_X;
            self->velocity.x = -self->velocity.x;
            self->state      = Motobug_State_Move;
            Motobug_State_Move();
        }
        else {
            Motobug_CheckPlayerCollisions();
            Motobug_CheckOffScreen();
        }
    }
    else {
        self->wasTurning = false;
        RSDK.SetSpriteAnimation(Motobug->aniFrames, 0, &self->animator, true, 0);
        self->state = Motobug_State_Fall;
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }
}

#if GAME_INCLUDE_EDITOR
void Motobug_EditorDraw(void) { Motobug_Draw(); }

void Motobug_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Motobug->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Motobug.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("Blueprint"))
        Motobug->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/Motobug.bin", SCOPE_STAGE);
}
#endif

void Motobug_Serialize(void) {}
