// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TurboTurtle Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTurboTurtle *TurboTurtle;

void TurboTurtle_Update(void)
{
    RSDK_THIS(TurboTurtle);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.animationID == 1) {
        if (!self->animator.frameID && self->animator.timer == 1)
            RSDK.PlaySfx(TurboTurtle->sfxWalk, false, 255);

        if (self->animator.frameID == 5 && self->animator.timer == 1)
            RSDK.PlaySfx(TurboTurtle->sfxWalk2, false, 255);
    }

    self->fanActiveTop   = false;
    self->leftFanActive  = false;
    self->rightFanActive = false;

    StateMachine_Run(self->state);

    TurboTurtle_CheckPlayerCollisions();

    if (self->state != TurboTurtle_State_Init) {
        if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->direction = self->startDir;
            self->position  = self->startPos;
            TurboTurtle_Create(NULL);
        }
    }
}

void TurboTurtle_LateUpdate(void) {}

void TurboTurtle_StaticUpdate(void) {}

void TurboTurtle_Draw(void)
{
    RSDK_THIS(TurboTurtle);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void TurboTurtle_Create(void *data)
{
    RSDK_THIS(TurboTurtle);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->startDir      = self->direction;
    self->drawFX        = FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    self->updateRange.x = abs(0x70000 * (self->stepCount * (2 * (self->initialSide == 0) - 1) + self->startPos.x - self->startPos.x)) + 0x800000;

    if (!self->leftFanDuration)
        self->leftFanDuration = 60;

    if (!self->rightFanDuration)
        self->rightFanDuration = 60;

    if (!self->leftFanLength)
        self->leftFanLength = 128;

    if (!self->rightFanLength)
        self->rightFanLength = 128;

    if (!self->leftFanStrength)
        self->leftFanStrength = 18;

    if (!self->rightFanStrength)
        self->rightFanStrength = 18;

    self->hitboxFanT.left   = -24;
    self->hitboxFanT.top    = -19;
    self->hitboxFanT.right  = 24;
    self->hitboxFanT.bottom = -19;

    self->hitboxFanR.left   = -19;
    self->hitboxFanR.top    = -24;
    self->hitboxFanR.right  = 19;
    self->hitboxFanR.bottom = 24;

    self->hitboxFanL.left   = -20;
    self->hitboxFanL.top    = -24;
    self->hitboxFanL.right  = -20;
    self->hitboxFanL.bottom = 24;

    self->state = TurboTurtle_State_Init;
}

void TurboTurtle_StageLoad(void)
{
    TurboTurtle->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TurboTurtle.bin", SCOPE_STAGE);

    TurboTurtle->hitboxBadnik.left   = -13;
    TurboTurtle->hitboxBadnik.top    = -12;
    TurboTurtle->hitboxBadnik.right  = 13;
    TurboTurtle->hitboxBadnik.bottom = 16;

    DEBUGMODE_ADD_OBJ(TurboTurtle);

    TurboTurtle->sfxWalk  = RSDK.GetSfx("TMZ1/TurtleWalk.wav");
    TurboTurtle->sfxWalk2 = RSDK.GetSfx("TMZ1/TurtleWalk2.wav");

    Soundboard_LoadSfx("HCZ/SmallFan.wav", 37404, TurboTurtle_SfxCheck_SmallFan, StateMachine_None);
}

void TurboTurtle_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityTurboTurtle *turboTurtle = CREATE_ENTITY(TurboTurtle, NULL, self->position.x, self->position.y);
    turboTurtle->direction         = self->direction;
    turboTurtle->startDir          = self->direction;
}

void TurboTurtle_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void TurboTurtle_CheckPlayerCollisions(void)
{
    RSDK_THIS(TurboTurtle);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &TurboTurtle->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void TurboTurtle_SetupState(void)
{
    RSDK_THIS(TurboTurtle);

    int32 dir   = self->currentSide ? self->rightFanDir : self->leftFanDir;
    self->timer = 0;

    switch (dir) {
        case TURBOTURTLE_FANDIR_TOP:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 2, &self->animator, true, 0);
            self->state = TurboTurtle_State_FanTop;
            break;

        case TURBOTURTLE_FANDIR_RIGHT:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 4, &self->animator, true, 0);
            self->direction = FLIP_NONE;
            self->state     = TurboTurtle_State_TurnFanRight;
            break;

        case TURBOTURTLE_FANDIR_LEFT:
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 4, &self->animator, true, 0);
            self->direction = FLIP_X;
            self->state     = TurboTurtle_State_TurnFanLeft;
            break;
    }
}

void TurboTurtle_HandleFans(void)
{
    RSDK_THIS(TurboTurtle);

    int32 storeDir  = self->direction;
    self->direction = FLIP_NONE;

    int32 strength = self->currentSide ? self->rightFanStrength : self->leftFanStrength;

    int32 length = self->currentSide ? self->rightFanLength : self->leftFanLength;

    if (self->fanActiveTop) {
        length += RSDK.Sin256(2 * Zone->timer) >> 5;
        self->hitboxFanT.top = self->hitboxFanT.bottom - length;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxFanT)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                    if (!player->onGround) {
                        player->velocity.y -= player->gravityStrength;
                        if (player->velocity.y > 0)
                            player->velocity.y -= (player->velocity.y >> 3);
                    }

                    int32 thisY  = self->position.y;
                    int32 bottom = thisY + (self->hitboxFanT.bottom << 16);
                    int32 top    = (self->hitboxFanT.top << 16) + thisY;
                    if (player->position.y > top)
                        top = player->position.y;

                    player->position.y -= ((strength * (((length << 16) - bottom + top != 0 ? (length << 16) - bottom + top : 0) / length)) >> 1);
                }
            }
        }

        TurboTurtle_HandleFanParticles(TURBOTURTLE_FANDIR_TOP, strength, length << 16);
    }

    if (self->leftFanActive) {
        self->hitboxFanL.left = self->hitboxFanL.right - length;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxFanL)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                    int32 left = (self->hitboxFanL.left << 16) + self->position.x;
                    if (player->position.x > left)
                        left = player->position.x;

                    int32 pos = (length << 16) - (self->position.x + (self->hitboxFanL.right << 16)) + left;
                    player->position.x -= (strength * ((pos & -(pos != 0)) / length)) >> 1;
                }
            }
        }

        TurboTurtle_HandleFanParticles(TURBOTURTLE_FANDIR_LEFT, strength, length << 16);
    }

    if (self->rightFanActive) {
        self->hitboxFanR.right = length + self->hitboxFanR.left;

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitboxFanR)) {
                int32 anim = player->animator.animationID;
                if (anim != ANI_CLING && anim != ANI_SHAFT_SWING) {
                    int32 right = (self->hitboxFanR.right << 16) + self->position.x;
                    if (player->position.x < right)
                        right = player->position.x;

                    int32 pos = self->position.x + (self->hitboxFanR.left << 16);
                    player->position.x += (strength * ((((length << 16) - right + pos != 0) ? (length << 16) - right + pos : 0) / length)) >> 1;
                }
            }
        }

        TurboTurtle_HandleFanParticles(TURBOTURTLE_FANDIR_RIGHT, strength, length << 16);
    }

    self->direction = storeDir;
}

void TurboTurtle_HandleFanParticles(uint8 type, int32 strength, int32 length)
{
    RSDK_THIS(TurboTurtle);

    if (!(Zone->timer % 3)) {
        for (int32 i = 0; i < RSDK.Rand(1, 2); ++i) {
            int32 timer = 0;
            int32 anim  = (RSDK.Rand(0, 10) > 7) + 6;
            int32 frame = RSDK.Rand(0, 3);

            int32 spawnX = 0, spawnY = 0;
            int32 velX = 0, velY = 0;

            switch (type) {
                case TURBOTURTLE_FANDIR_TOP:
                    spawnY = self->position.y + (self->hitboxFanT.bottom << 16);
                    spawnX = (RSDK.Rand(-13, 13) << 16) + self->position.x;
                    velX   = (spawnX - self->position.x) >> 7;
                    RSDK.Rand(0, 4);
                    velY  = -0x8000 * strength;
                    timer = length / abs(-0x8000 * strength);
                    break;

                case TURBOTURTLE_FANDIR_RIGHT:
                    spawnX = self->position.x + (self->hitboxFanR.left << 16);
                    spawnY = (RSDK.Rand(-13, 13) << 16) + self->position.y;
                    RSDK.Rand(0, 4);
                    velX  = strength << 15;
                    velY  = (spawnY - self->position.y) >> 7;
                    timer = length / abs(strength << 15);
                    break;

                case TURBOTURTLE_FANDIR_LEFT:
                    spawnX = self->position.x - (self->hitboxFanR.left << 16);
                    spawnY = (RSDK.Rand(-13, 13) << 16) + self->position.y;
                    RSDK.Rand(0, 4);
                    velX  = -0x8000 * strength;
                    velY  = (spawnY - self->position.y) >> 7;
                    timer = length / abs(-0x8000 * strength);
                    break;
                default: break;
            }

            timer += RSDK.Rand(-5, 5);

            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, spawnX, spawnY);
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, anim, &debris->animator, true, frame);
            debris->velocity.x = velX;
            debris->velocity.y = velY;
            debris->drawGroup  = Zone->objectDrawGroup[0] - 1;
            debris->timer      = timer;
        }
    }
}

bool32 TurboTurtle_SfxCheck_SmallFan(void)
{
    int32 count = 0;

    foreach_all(TurboTurtle, turtle)
    {
        if (turtle->state == TurboTurtle_State_FanTop || turtle->state == TurboTurtle_State_FanRight || turtle->state == TurboTurtle_State_FanLeft)
            ++count;
    }

    return count > 0;
}

void TurboTurtle_State_Init(void)
{
    RSDK_THIS(TurboTurtle);

    self->active = ACTIVE_NORMAL;

    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &self->animator, true, 0);
    self->direction   = self->startDir;
    self->position    = self->startPos;
    self->currentSide = self->initialSide;

    TurboTurtle_SetupState();
}

void TurboTurtle_State_Walking(void)
{
    RSDK_THIS(TurboTurtle);

    int32 offsets[] = { 0, 0, 0, 0, 1, 1, 0, 0 };

    self->position.x += (2 * (self->currentSide == 1) - 1) * (offsets[self->animator.frameID] << 16);
    if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x100000, 2)) {
        self->stepTimer = 0;
        TurboTurtle_SetupState();
    }
    else if (self->animator.frameID == self->animator.frameCount - 1) {
        if (self->animator.timer == self->animator.frameDuration) {
            if (self->stepTimer < self->stepCount) {
                self->stepTimer++;
            }
            else {
                self->stepTimer = 0;
                TurboTurtle_SetupState();
            }
        }
    }
}

void TurboTurtle_State_TurnFanUp(void)
{
    RSDK_THIS(TurboTurtle);

    if (self->animator.animationID == 5) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            uint8 dir         = self->currentSide != FLIP_NONE;
            self->currentSide = !self->currentSide;
            self->direction   = dir;
            self->stepTimer   = 0;
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 1, &self->animator, true, 0);
            self->state = TurboTurtle_State_Walking;
        }
    }
}

void TurboTurtle_State_FanTop(void)
{
    RSDK_THIS(TurboTurtle);

    int32 duration = self->currentSide ? self->rightFanDuration : self->leftFanDuration;

    if (self->stationary || self->timer < duration) {
        self->fanActiveTop = true;
        TurboTurtle_HandleFans();

        if (!self->stationary)
            ++self->timer;
    }
    else {
        uint8 dir         = self->currentSide != 0;
        self->currentSide = !self->currentSide;
        self->direction   = dir;
        self->stepTimer   = 0;
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 1, &self->animator, true, 0);
        self->state = TurboTurtle_State_Walking;
    }
}

void TurboTurtle_State_TurnFanRight(void)
{
    RSDK_THIS(TurboTurtle);

    if (self->animator.animationID == 4) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 3, &self->animator, true, 0);
            self->state = TurboTurtle_State_FanRight;
        }
    }
}

void TurboTurtle_State_FanRight(void)
{
    RSDK_THIS(TurboTurtle);

    int32 duration = self->currentSide ? self->rightFanDuration : self->leftFanDuration;

    if (self->stationary || self->timer < duration) {
        self->rightFanActive = true;
        TurboTurtle_HandleFans();

        if (!self->stationary)
            ++self->timer;
    }
    else {
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 5, &self->animator, true, 0);
        self->state = TurboTurtle_State_TurnFanUp;
    }
}

void TurboTurtle_State_TurnFanLeft(void)
{
    RSDK_THIS(TurboTurtle);

    if (self->animator.animationID == 4) {
        if (self->animator.frameID == self->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 3, &self->animator, true, 0);
            self->state = TurboTurtle_State_FanLeft;
        }
    }
}

void TurboTurtle_State_FanLeft(void)
{
    RSDK_THIS(TurboTurtle);

    int32 duration = self->currentSide ? self->rightFanDuration : self->leftFanDuration;

    if (self->stationary || self->timer < duration) {
        self->leftFanActive = true;
        TurboTurtle_HandleFans();

        if (!self->stationary)
            ++self->timer;
    }
    else {
        RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 5, &self->animator, true, 0);
        self->state = TurboTurtle_State_TurnFanUp;
    }
}

#if GAME_INCLUDE_EDITOR
void TurboTurtle_EditorDraw(void)
{
    RSDK_THIS(TurboTurtle);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->startPos  = self->position;
    self->direction = self->initialSide ^ FLIP_X;
    self->startDir  = self->direction;

    self->updateRange.x = abs(0x70000 * (self->stepCount * (2 * (self->initialSide == 0) - 1) + self->startPos.x - self->startPos.x)) + 0x800000;

    RSDK.SetSpriteAnimation(TurboTurtle->aniFrames, 0, &self->animator, true, 0);
    self->direction = self->startDir;

    TurboTurtle_Draw();
}

void TurboTurtle_EditorLoad(void)
{
    TurboTurtle->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/TurboTurtle.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TurboTurtle, initialSide);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    RSDK_ACTIVE_VAR(TurboTurtle, leftFanDir);
    RSDK_ENUM_VAR("Top", TURBOTURTLE_FANDIR_TOP);
    RSDK_ENUM_VAR("Right", TURBOTURTLE_FANDIR_RIGHT);
    RSDK_ENUM_VAR("Left", TURBOTURTLE_FANDIR_LEFT);

    RSDK_ACTIVE_VAR(TurboTurtle, rightFanDir);
    RSDK_ENUM_VAR("Top", TURBOTURTLE_FANDIR_TOP);
    RSDK_ENUM_VAR("Right", TURBOTURTLE_FANDIR_RIGHT);
    RSDK_ENUM_VAR("Left", TURBOTURTLE_FANDIR_LEFT);
}
#endif

void TurboTurtle_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT16, stepCount);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, initialSide);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_BOOL, stationary);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, leftFanDir);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanDuration);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanStrength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, leftFanLength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT8, rightFanDir);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanDuration);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanStrength);
    RSDK_EDITABLE_VAR(TurboTurtle, VAR_UINT32, rightFanLength);
}
