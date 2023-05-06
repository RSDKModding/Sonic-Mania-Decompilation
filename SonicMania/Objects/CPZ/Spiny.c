// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Spiny Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpiny *Spiny;

void Spiny_Update(void)
{
    RSDK_THIS(Spiny);

    StateMachine_Run(self->state);
}

void Spiny_LateUpdate(void) {}

void Spiny_StaticUpdate(void) {}

void Spiny_Draw(void)
{
    RSDK_THIS(Spiny);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Spiny_Create(void *data)
{
    RSDK_THIS(Spiny);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (data) {
        self->drawGroup = Zone->objectDrawGroup[0] - 1;
        RSDK.SetSpriteAnimation(Spiny->aniFrames, 4, &self->animator, true, 0);
        self->state = Spiny_State_Shot;
    }
    else {
        self->startPos = self->position;
        self->startDir = self->direction;
        self->drawFX   = FX_FLIP;

        if (self->type == SPINY_FLOOR) {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &self->animator, true, 0);
            self->state = Spiny_State_Init;

            if (!(self->direction & FLIP_X))
                self->velocity.x = -0x4000;
            else
                self->velocity.x = 0x4000;
        }
        else {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 2, &self->animator, true, 0);
            self->state = Spiny_State_Init;

            if (!(self->direction & FLIP_Y))
                self->velocity.y = -0x4000;
            else
                self->velocity.y = 0x4000;
        }
    }
}

void Spiny_StageLoad(void)
{
    Spiny->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Spiny.bin", SCOPE_STAGE);

    Spiny->hitboxSpiny.left   = -12;
    Spiny->hitboxSpiny.top    = -15;
    Spiny->hitboxSpiny.right  = 12;
    Spiny->hitboxSpiny.bottom = 15;

    Spiny->hitboxRange.left   = -96;
    Spiny->hitboxRange.top    = -256;
    Spiny->hitboxRange.right  = 96;
    Spiny->hitboxRange.bottom = 256;

    Spiny->hitboxShot.left   = -4;
    Spiny->hitboxShot.top    = -4;
    Spiny->hitboxShot.right  = 4;
    Spiny->hitboxShot.bottom = 4;

    DEBUGMODE_ADD_OBJ(Spiny);

    Spiny->sfxShot = RSDK.GetSfx("Stage/Shot.wav");
}

void Spiny_DebugSpawn(void)
{
    RSDK_THIS(Spiny);

    EntitySpiny *spiny = CREATE_ENTITY(Spiny, NULL, self->position.x, self->position.y);
    spiny->direction   = self->direction;
}

void Spiny_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Spiny_CheckPlayerCollisions(void)
{
    RSDK_THIS(Spiny);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Spiny->hitboxSpiny))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Spiny_CheckOffScreen(void)
{
    RSDK_THIS(Spiny);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position       = self->startPos;
        self->dirChangeTimer = 0;
        self->moveTimer      = 128;
        self->direction      = self->startDir;
        Spiny_Create(NULL);
    }
}

void Spiny_State_Init(void)
{
    RSDK_THIS(Spiny);

    self->active = ACTIVE_NORMAL;
    if (self->type == SPINY_FLOOR) {
        self->state = Spiny_State_Floor;
        Spiny_State_Floor();
    }
    else {
        self->state = Spiny_State_Wall;
        Spiny_State_Wall();
    }
}

void Spiny_State_Floor(void)
{
    RSDK_THIS(Spiny);

    RSDK.ProcessAnimation(&self->animator);

    if (++self->dirChangeTimer >= 256) {
        self->dirChangeTimer = 0;
        self->direction ^= FLIP_X;
        self->moveTimer  = 128;
        self->velocity.x = -self->velocity.x;
    }

    int32 distance = 0x7FFFFFFF;
    self->position.x += self->velocity.x;

    if (!self->moveTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Spiny->hitboxRange)) {
                self->moveTimer = 40;
                self->state     = Spiny_State_Shoot_Floor;
                RSDK.SetSpriteAnimation(Spiny->aniFrames, 1, &self->animator, true, 0);

                if (abs(player->position.x - self->position.x) < distance) {
                    distance = abs(player->position.x - self->position.x);
                    if (player->position.x > self->position.x)
                        self->shotVelocity = 0x10000;
                    else
                        self->shotVelocity = -0x10000;
                }
            }
        }
    }
    else {
        self->moveTimer--;
    }

    Spiny_CheckPlayerCollisions();
    Spiny_CheckOffScreen();
}

void Spiny_State_Shoot_Floor(void)
{
    RSDK_THIS(Spiny);

    RSDK.ProcessAnimation(&self->animator);

    if (--self->moveTimer == 20) {
        RSDK.PlaySfx(Spiny->sfxShot, false, 0xFF);

        EntitySpiny *shot = CREATE_ENTITY(Spiny, INT_TO_VOID(true), self->position.x, self->position.y);
        shot->velocity.x  = self->shotVelocity;
        if (!(self->direction & 2))
            shot->velocity.y = -0x30000;
        else
            shot->velocity.y = 0x20000;
    }
    else {
        if (self->moveTimer < 0) {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &self->animator, true, 0);
            self->moveTimer = 64;
            self->state     = Spiny_State_Floor;
        }
    }

    Spiny_CheckPlayerCollisions();
    Spiny_CheckOffScreen();
}

void Spiny_State_Wall(void)
{
    RSDK_THIS(Spiny);

    RSDK.ProcessAnimation(&self->animator);

    if (++self->dirChangeTimer >= 256) {
        self->dirChangeTimer = 0;
        self->direction ^= FLIP_Y;
        self->moveTimer  = 128;
        self->velocity.y = -self->velocity.y;
    }

    self->position.y += self->velocity.y;

    if (!self->moveTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Spiny->hitboxRange)) {
                self->moveTimer = 40;
                self->state     = Spiny_State_Shoot_Wall;
                RSDK.SetSpriteAnimation(Spiny->aniFrames, 3, &self->animator, true, 0);

                if (!(self->direction & 1))
                    self->shotVelocity = -0x30000;
                else
                    self->shotVelocity = 0x30000;
            }
        }
    }
    else {
        self->moveTimer--;
    }

    Spiny_CheckPlayerCollisions();
    Spiny_CheckOffScreen();
}

void Spiny_State_Shoot_Wall(void)
{
    RSDK_THIS(Spiny);

    RSDK.ProcessAnimation(&self->animator);

    if (--self->moveTimer == 20) {
        RSDK.PlaySfx(Spiny->sfxShot, false, 0xFF);
        CREATE_ENTITY(Spiny, INT_TO_VOID(true), self->position.x, self->position.y)->velocity.x = self->shotVelocity;
    }
    else {
        if (self->moveTimer < 0) {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 2, &self->animator, true, 0);
            self->moveTimer = 64;
            self->state     = Spiny_State_Wall;
        }
    }

    Spiny_CheckPlayerCollisions();
    Spiny_CheckOffScreen();
}

void Spiny_State_Shot(void)
{
    RSDK_THIS(Spiny);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2000;

    if (RSDK.CheckOnScreen(self, &self->updateRange)) {
        RSDK.ProcessAnimation(&self->animator);

        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0, true)) {
            self->inkEffect |= INK_ADD;
            self->alpha = 0x100;
            self->state = Spiny_State_ShotDisappear;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Spiny->hitboxShot))
                Player_ProjectileHurt(player, self);
        }
    }
    else {
        destroyEntity(self);
    }
}

void Spiny_State_ShotDisappear(void)
{
    RSDK_THIS(Spiny);

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->alpha -= 0x20;
    if (self->alpha <= 0)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Spiny_EditorDraw(void)
{
    RSDK_THIS(Spiny);
    self->drawFX = FX_FLIP;

    if (self->type == SPINY_FLOOR)
        RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &self->animator, false, 0);
    else
        RSDK.SetSpriteAnimation(Spiny->aniFrames, 2, &self->animator, false, 0);

    Spiny_Draw();
}

void Spiny_EditorLoad(void)
{
    Spiny->aniFrames = RSDK.LoadSpriteAnimation("CPZ/Spiny.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Spiny, type);
    RSDK_ENUM_VAR("Floor", SPINY_FLOOR);
    RSDK_ENUM_VAR("Wall", SPINY_WALL);

    RSDK_ACTIVE_VAR(Spiny, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void Spiny_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spiny, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Spiny, VAR_UINT8, direction);
}
