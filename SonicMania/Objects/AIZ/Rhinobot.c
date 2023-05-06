// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Rhinobot Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectRhinobot *Rhinobot;

void Rhinobot_Update(void)
{
    RSDK_THIS(Rhinobot);
    StateMachine_Run(self->state);
}

void Rhinobot_LateUpdate(void) {}

void Rhinobot_StaticUpdate(void) {}

void Rhinobot_Draw(void)
{
    RSDK_THIS(Rhinobot);
    if (self->drawDust) {
        Vector2 drawPos;
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
        if (self->state == Rhinobot_State_Moving) {
            if (!self->moveDir)
                drawPos.x -= 0xD0000;
            else
                drawPos.x += 0xD0000;
        }
        RSDK.DrawSprite(&self->dustAnimator, &drawPos, false);
    }
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
}

void Rhinobot_Create(void *data)
{
    RSDK_THIS(Rhinobot);
    self->visible = true;
    self->drawFX |= FX_FLIP;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 2, &self->dustAnimator, true, 0);
    self->state = Rhinobot_State_Init;
}

void Rhinobot_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("AIZ"))
        Rhinobot->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Rhinobot.bin", SCOPE_STAGE);

    Rhinobot->hitboxBadnik.left   = -14;
    Rhinobot->hitboxBadnik.top    = -9;
    Rhinobot->hitboxBadnik.right  = 23;
    Rhinobot->hitboxBadnik.bottom = 15;

    Rhinobot->sfxHuff = RSDK.GetSfx("Stage/Huff.wav");

    DEBUGMODE_ADD_OBJ(Rhinobot);
}

void Rhinobot_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Rhinobot, NULL, self->position.x, self->position.y);
}

void Rhinobot_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

bool32 Rhinobot_CheckTileCollisions(void)
{
    RSDK_THIS(Rhinobot);

    if (self->moveDir) {
        if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x40000, 0xF0000, 8)) {
            if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x100000, 0, false))
                return false;
            if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
                return false;
            self->state = Rhinobot_State_Fall;
        }
    }
    else if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x40000, 0xF0000, 8)) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x100000, 0, false))
            return false;
        if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xF0000, 8))
            return false;
        self->state = Rhinobot_State_Fall;
    }

    return true;
}

void Rhinobot_CheckPlayerCollisions(void)
{
    RSDK_THIS(Rhinobot);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &Rhinobot->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Rhinobot_CheckOffScreen(void)
{
    RSDK_THIS(Rhinobot);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        Rhinobot_Create(NULL);
    }
}

void Rhinobot_Delay_Charge(void)
{
    RSDK_THIS(Rhinobot);

    self->drawDust = false;
    if (!self->moveDir)
        self->velocity.x = -0x40000;
    else
        self->velocity.x = 0x40000;

    self->state      = Rhinobot_State_Skidding;
    self->stateDelay = Rhinobot_Delay_Skidding;
}

void Rhinobot_Delay_Skidding(void)
{
    RSDK_THIS(Rhinobot);

    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 1, &self->bodyAnimator, true, 0);
    self->skidDir ^= FLIP_X;
    self->acceleration = -self->acceleration;
    self->topSpeed     = -self->topSpeed;
    self->stateDelay   = Rhinobot_Delay_SkidFinish;
}

void Rhinobot_Delay_SkidFinish(void)
{
    RSDK_THIS(Rhinobot);

    self->moveDir ^= FLIP_X;
    self->direction ^= FLIP_X;
    self->drawDust = false;
    RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->bodyAnimator, true, 0);
    self->stateDelay = Rhinobot_Delay_Skidding;
}

void Rhinobot_State_Init(void)
{
    RSDK_THIS(Rhinobot);

    self->active     = ACTIVE_NORMAL;
    self->stateDelay = Rhinobot_Delay_Skidding;
    self->moveDir    = self->startDir;
    self->skidDir    = self->startDir;

    if (self->direction == FLIP_NONE) {
        self->acceleration = -0x1000;
        self->topSpeed     = -0x30000;
    }
    else {
        self->acceleration = 0x1000;
        self->topSpeed     = 0x30000;
    }

    self->state = Rhinobot_State_Moving;
    Rhinobot_State_Moving();
}

void Rhinobot_State_Moving(void)
{
    RSDK_THIS(Rhinobot);

    EntityPlayer *player = Player_GetNearestPlayer();

    bool32 isRevingUp = false;
    if (abs(player->position.y - self->position.y) <= 0x200000) {
        if (abs(player->position.x - self->position.x) <= 0x600000) {
            if (self->skidDir == self->moveDir && (player->position.x > self->position.x) == self->skidDir) {
                self->state      = Rhinobot_State_Idle;
                self->velocity.x = 0;
                self->timer      = 32;
                self->stateDelay = Rhinobot_Delay_Charge;
                self->drawDust   = true;
                if (self->onScreen)
                    RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
                isRevingUp = true;
            }
        }
    }

    if (!isRevingUp) {
        if (Rhinobot_CheckTileCollisions()) {
            if (self->state == Rhinobot_State_Moving) {
                self->velocity.x += self->acceleration;
                self->position.x += self->velocity.x;

                if (!self->velocity.x || (self->skidDir && self->velocity.x >= self->topSpeed)
                    || (!self->skidDir && self->velocity.x <= self->topSpeed)) {
#if RETRO_USE_MOD_LOADER
                    StateMachine_Run(self->stateDelay);
#else
                    self->stateDelay();
#endif
                }
            }
        }
        else {
            self->velocity.x = 0;
            self->skidDir    = self->moveDir ^ FLIP_X;
            self->moveDir ^= FLIP_X;
            self->direction ^= FLIP_X;
            self->drawDust = false;
            RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->bodyAnimator, true, 0);
            self->stateDelay = Rhinobot_Delay_Skidding;
        }

        if (((self->moveDir && !self->skidDir && self->velocity.x < 0x28000) || (!self->moveDir && self->skidDir && self->velocity.x > -0x28000))
            && !self->drawDust) {
            self->drawDust = true;
            if (self->onScreen)
                RSDK.PlaySfx(Rhinobot->sfxHuff, false, 0xFF);
        }
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->dustAnimator);

    Rhinobot_CheckPlayerCollisions();
    Rhinobot_CheckOffScreen();
}

void Rhinobot_State_Skidding(void)
{
    RSDK_THIS(Rhinobot);

    self->position.x += self->velocity.x;

    if (!Rhinobot_CheckTileCollisions()) {
        self->velocity.x = 0;
        self->skidDir    = self->moveDir ^ FLIP_X;
        self->moveDir ^= FLIP_X;
        self->direction ^= FLIP_X;
        self->drawDust = false;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->bodyAnimator, true, 0);
        self->stateDelay = Rhinobot_Delay_Skidding;
        self->state      = Rhinobot_State_Moving;
    }

    RSDK.ProcessAnimation(&self->bodyAnimator);
    RSDK.ProcessAnimation(&self->dustAnimator);

    Rhinobot_CheckPlayerCollisions();
    Rhinobot_CheckOffScreen();
}

void Rhinobot_State_Idle(void)
{
    RSDK_THIS(Rhinobot);

    RSDK.ProcessAnimation(&self->dustAnimator);

    if (--self->timer <= 0) {
#if RETRO_USE_MOD_LOADER
        StateMachine_Run(self->stateDelay);
#else
        self->stateDelay();
#endif
    }

    Rhinobot_CheckPlayerCollisions();
    Rhinobot_CheckOffScreen();
}

void Rhinobot_State_Fall(void)
{
    RSDK_THIS(Rhinobot);

    RSDK.ProcessAnimation(&self->dustAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(Rhinobot->aniFrames, 0, &self->bodyAnimator, true, 0);
        self->drawDust = true;
        if (self->onScreen)
            RSDK.PlaySfx(Rhinobot->sfxHuff, false, 255);
        self->state = Rhinobot_State_Moving;
        Rhinobot_State_Moving();
    }
    else {
        Rhinobot_CheckPlayerCollisions();
        Rhinobot_CheckOffScreen();
    }
}

#if GAME_INCLUDE_EDITOR
void Rhinobot_EditorDraw(void) { Rhinobot_Draw(); }

void Rhinobot_EditorLoad(void) { Rhinobot->aniFrames = RSDK.LoadSpriteAnimation("AIZ/Rhinobot.bin", SCOPE_STAGE); }
#endif

void Rhinobot_Serialize(void) {}
#endif
