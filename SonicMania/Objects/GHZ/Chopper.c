// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Chopper Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectChopper *Chopper;

void Chopper_Update(void)
{
    RSDK_THIS(Chopper);
    StateMachine_Run(self->state);
}

void Chopper_LateUpdate(void) {}

void Chopper_StaticUpdate(void) {}

void Chopper_Draw(void)
{
    RSDK_THIS(Chopper);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Chopper_Create(void *data)
{
    RSDK_THIS(Chopper);
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x1200000;
    self->drawFX        = FX_FLIP;
    self->state         = Chopper_State_Init;
}

void Chopper_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Chopper->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Chopper.bin", SCOPE_STAGE);

    Chopper->hitboxJump.left   = -10;
    Chopper->hitboxJump.top    = -20;
    Chopper->hitboxJump.right  = 6;
    Chopper->hitboxJump.bottom = 20;

    Chopper->hitboxSwim.left   = -20;
    Chopper->hitboxSwim.top    = -6;
    Chopper->hitboxSwim.right  = 20;
    Chopper->hitboxSwim.bottom = 10;

    Chopper->hitboxRange.left   = -160;
    Chopper->hitboxRange.top    = -32;
    Chopper->hitboxRange.right  = 16;
    Chopper->hitboxRange.bottom = 32;

    Chopper->hitboxWater.left   = -20;
    Chopper->hitboxWater.top    = -24;
    Chopper->hitboxWater.right  = 20;
    Chopper->hitboxWater.bottom = -16;

    DEBUGMODE_ADD_OBJ(Chopper);
}
void Chopper_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Chopper->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Chopper_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Chopper, NULL, self->position.x, self->position.y);
}

void Chopper_CheckOffScreen(void)
{
    RSDK_THIS(Chopper);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->direction  = self->startDir;
        Chopper_Create(NULL);
    }
}

void Chopper_CheckPlayerCollisions_Jump(void)
{
    RSDK_THIS(Chopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Chopper->hitboxJump))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Chopper_CheckPlayerCollisions_Swim(void)
{
    RSDK_THIS(Chopper);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Chopper->hitboxSwim))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Chopper_State_Init(void)
{
    RSDK_THIS(Chopper);

    self->active     = ACTIVE_NORMAL;
    self->velocity.x = -0x10000;

    if (self->type == CHOPPER_JUMP) {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 0, &self->animator, true, 0);
        self->state = Chopper_State_Jump;
        Chopper_State_Jump();
    }
    else {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, true, 0);
        self->state = Chopper_State_Swim;
        self->timer = 512;

        if (!self->direction)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        Chopper_State_Swim();
    }
}

void Chopper_State_Jump(void)
{
    RSDK_THIS(Chopper);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (self->velocity.y < -0x38000) {
        self->animator.speed = 1;
    }
    else {
        if (self->velocity.y > 0x38000) {
            self->animator.frameID = 0;
            self->animator.speed   = 0;
        }
        else {
            self->animator.speed = 2;
        }
    }

    if (self->position.y > self->startPos.y) {
        self->position.y = self->startPos.y;
        self->velocity.y = -0x70000;
    }

    RSDK.ProcessAnimation(&self->animator);

    Chopper_CheckPlayerCollisions_Jump();
    Chopper_CheckOffScreen();
}

void Chopper_State_Swim(void)
{
    RSDK_THIS(Chopper);

    self->position.x += self->velocity.x;

    bool32 hitWall = false;
    if (self->direction) {
        hitWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, 0, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, -0xF0000, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, 0xF0000, true);
    }
    else {
        hitWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, 0, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, -0xF0000, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, 0xF0000, true);
    }

    if (--self->timer <= 0 || hitWall) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
        self->timer      = 512;
    }

    if (self->charge) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Chopper->hitboxRange)) {
                self->state = Chopper_State_ChargeDelay;
                RSDK.SetSpriteAnimation(Chopper->aniFrames, 2, &self->animator, false, 0);
                self->timer = 16;
                if (!self->direction)
                    self->velocity.x = -0x20000;
                else
                    self->velocity.x = 0x20000;

                if (abs(player->position.y - self->position.y) > 0x100000) {
                    if (player->position.y < self->position.y)
                        self->velocity.y = -0x8000;
                    else
                        self->velocity.y = 0x8000;
                }
            }
        }
    }

    RSDK.ProcessAnimation(&self->animator);

    Chopper_CheckPlayerCollisions_Swim();
    Chopper_CheckOffScreen();
}

void Chopper_State_ChargeDelay(void)
{
    RSDK_THIS(Chopper);

    if (!self->timer)
        self->state = Chopper_State_Charge;
    else
        --self->timer;

    RSDK.ProcessAnimation(&self->animator);
    Chopper_CheckPlayerCollisions_Swim();
    Chopper_CheckOffScreen();
}

void Chopper_State_Charge(void)
{
    RSDK_THIS(Chopper);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    bool32 hitWall = false;
    if (self->direction) {
        hitWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, 0, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, -0xF0000, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, 0xF0000, true);
    }
    else {
        hitWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, 0, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, -0xF0000, true);
        hitWall |= RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, 0xF0000, true);
    }

    if (hitWall) {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, false, 0);
        self->timer      = 512;
        self->velocity.y = 0;
        if (self->direction == FLIP_X)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        self->direction ^= FLIP_X;
        self->state = Chopper_State_Swim;

        hitWall = false;
    }
    else if (self->velocity.y >= 0) {
        if (self->velocity.y > 0)
            hitWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, true);
    }
    else {
        hitWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -0x100000, true);
        foreach_active(Water, water)
        {
            if (water->type == WATER_POOL)
                hitWall |= !RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Chopper->hitboxWater);
        }
    }

    if (hitWall) {
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, false, 0);
        self->timer      = 512;
        self->velocity.y = 0;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x4000;
        else
            self->velocity.x = 0x4000;
        self->state = Chopper_State_Swim;
    }

    RSDK.ProcessAnimation(&self->animator);
    Chopper_CheckPlayerCollisions_Swim();
    Chopper_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Chopper_EditorDraw(void)
{
    RSDK_THIS(Chopper);
    if (self->type == CHOPPER_JUMP)
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 0, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(Chopper->aniFrames, 1, &self->animator, true, 0);

    Chopper_Draw();
}

void Chopper_EditorLoad(void)
{
    Chopper->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Chopper.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Chopper, type);
    RSDK_ENUM_VAR("Jump", CHOPPER_JUMP);
    RSDK_ENUM_VAR("Swim", CHOPPER_SWIM);

    // Only used with "Swim" type, "Jump" type ignores it
    RSDK_ACTIVE_VAR(Chopper, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Chopper_Serialize(void)
{
    RSDK_EDITABLE_VAR(Chopper, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Chopper, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Chopper, VAR_BOOL, charge);
}
