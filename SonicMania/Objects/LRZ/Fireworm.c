// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Fireworm Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFireworm *Fireworm;

void Fireworm_Update(void)
{
    RSDK_THIS(Fireworm);

    StateMachine_Run(self->state);
}

void Fireworm_LateUpdate(void) {}

void Fireworm_StaticUpdate(void) {}

void Fireworm_Draw(void)
{
    RSDK_THIS(Fireworm);

    self->drawFX &= ~FX_FLIP;
    Fireworm->holeAnimator.frameID = 0;
    RSDK.DrawSprite(&Fireworm->holeAnimator, &self->startPos, false);

    self->drawFX |= FX_FLIP;
    if (self->state) {
        // Draw Body
        for (int32 i = FIREWORM_SEGMENT_COUNT - 1; i > 0; --i) {
            self->direction = self->bodyDirections[i];
            RSDK.DrawSprite(&self->bodyAnimators[i], &self->bodyPositions[i], false);
            RSDK.DrawSprite(&self->flameAnimators[i], &self->bodyPositions[i], false);
        }

        // Draw Head
        self->direction = self->bodyDirections[0];
        RSDK.DrawSprite(&self->bodyAnimators[0], NULL, false);
    }
}

void Fireworm_Create(void *data)
{
    RSDK_THIS(Fireworm);

    self->drawFX |= FX_FLIP;

    // Head
    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 0, &self->bodyAnimators[0], true, 0);

    if (!SceneInfo->inEditor) {
        for (int32 i = 0; i < FIREWORM_SEGMENT_COUNT; ++i) {
            self->bodyPositions[i].x = self->position.x;
            self->bodyPositions[i].y = self->position.y;
            self->bodyOriginY[i]     = self->position.y;
        }

        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->visible       = true;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->state         = Fireworm_State_Init;
    }
}

void Fireworm_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Fireworm.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Fireworm.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 6, &Fireworm->holeAnimator, true, 0);

    Fireworm->hitboxBadnik.left   = -8;
    Fireworm->hitboxBadnik.top    = -8;
    Fireworm->hitboxBadnik.right  = 8;
    Fireworm->hitboxBadnik.bottom = 8;

    Fireworm->hitboxRange.left   = -128;
    Fireworm->hitboxRange.top    = -128;
    Fireworm->hitboxRange.right  = 128;
    Fireworm->hitboxRange.bottom = 128;

    DEBUGMODE_ADD_OBJ(Fireworm);
}

void Fireworm_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(Fireworm, NULL, self->position.x, self->position.y);
}

void Fireworm_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Fireworm_CheckPlayerCollisions(void)
{
    RSDK_THIS(Fireworm);

    foreach_active(Player, player)
    {
        // Collide with the head
        self->position.x = self->bodyPositions[0].x;
        self->position.y = self->bodyPositions[0].y;

        if (Player_CheckBadnikTouch(player, self, &Fireworm->hitboxBadnik) && Player_CheckBadnikBreak(player, self, false)) {
            for (int32 i = 1; i < FIREWORM_SEGMENT_COUNT; ++i) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPositions[i].x, self->bodyPositions[i].y);
                RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &debris->animator, true, 0);
                debris->velocity.x      = RSDK.Rand(-0x40000, 0x40000);
                debris->velocity.y      = RSDK.Rand(-0x40000, 0x40000);
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->objectDrawGroup[0];
                debris->updateRange.x   = 0x400000;
                debris->updateRange.y   = 0x400000;
            }

            self->state = StateMachine_None;
            foreach_break;
        }
        else {
            // Collide with the rest of the body
            for (int32 i = 1; i < FIREWORM_SEGMENT_COUNT; ++i) {
                self->position.x = self->bodyPositions[i].x;
                self->position.y = self->bodyPositions[i].y;

                if (Player_CheckCollisionTouch(player, self, &Fireworm->hitboxBadnik)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x300, 2, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                }
            }
        }
    }

    if (self->state) {
        self->position.x = self->bodyPositions[0].x;
        self->position.y = self->bodyPositions[0].y;
    }
}

void Fireworm_CheckOffScreen(void)
{
    RSDK_THIS(Fireworm);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->timer     = 0;
        self->position  = self->startPos;
        self->direction = self->startDir;

        for (int32 i = 0; i < FIREWORM_SEGMENT_COUNT; ++i) {
            self->bodyVelocities[i].x = 0;
            self->bodyAngles[i]       = 0;
            self->bodyTimers[i]       = 0;

            if (i > 0) {
                RSDK.SetSpriteAnimation(-1, 0, &self->bodyAnimators[i], true, 0);
                RSDK.SetSpriteAnimation(-1, 0, &self->flameAnimators[i], true, 0);
            }
        }

        Fireworm_Create(NULL);
    }
}

void Fireworm_State_Init(void)
{
    RSDK_THIS(Fireworm);

    self->active = ACTIVE_NORMAL;

    self->state = Fireworm_State_AwaitPlayer;
    Fireworm_State_AwaitPlayer();
}

void Fireworm_State_AwaitPlayer(void)
{
    RSDK_THIS(Fireworm);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Fireworm->hitboxRange)) {
            for (int32 i = 0; i < FIREWORM_SEGMENT_COUNT; ++i) {
                self->bodyDirections[i] = player->position.x >= self->position.x;
            }

            self->startDir = self->bodyDirections[0];

            if (self->startDir) {
                self->bodyVelocities[0].x = 0x10000;
                self->boundsL             = self->position.x - 0x3C0000;
                self->boundsR             = self->position.x + 0xBC0000;
            }
            else {
                self->bodyVelocities[0].x = -0x10000;
                self->boundsL             = self->position.x - 0xBC0000;
                self->boundsR             = self->position.x + 0x3C0000;
            }

            self->state = Fireworm_State_HeadAppear;
            foreach_break;
        }
    }

    if (self->state == Fireworm_State_BodyAppear)
        Fireworm_CheckOffScreen();
}

void Fireworm_State_HeadAppear(void)
{
    RSDK_THIS(Fireworm);

    RSDK.ProcessAnimation(&self->bodyAnimators[0]);

    if (self->bodyAnimators[0].frameID == 3) {
        for (int32 i = 1; i < FIREWORM_SEGMENT_COUNT; ++i) {
            RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &self->bodyAnimators[i], true, 0);
            self->flameExhaustDelays[i] = RSDK.Rand(0, 60);
        }

        self->state = Fireworm_State_BodyAppear;
    }
}

void Fireworm_State_BodyAppear(void)
{
    RSDK_THIS(Fireworm);

    ++self->timer;

    for (int32 i = 0; i < FIREWORM_SEGMENT_COUNT; ++i) {
        if (self->timer == (i * 10) + 4) {
            self->bodyVelocities[i].x = self->bodyDirections[i] ? 0x10000 : -0x10000;

            if (i == FIREWORM_SEGMENT_COUNT - 1) {
                self->timer = 0;
                self->state = Fireworm_State_FlyAround;
            }
        }
    }

    Fireworm_State_FlyAround();
}

void Fireworm_State_FlyAround(void)
{
    RSDK_THIS(Fireworm);

    for (int32 i = 0; i < FIREWORM_SEGMENT_COUNT; ++i) {
        if (!self->bodyVelocities[i].x)
            continue;

        RSDK.ProcessAnimation(&self->bodyAnimators[i]);
        RSDK.ProcessAnimation(&self->flameAnimators[i]);

        self->bodyPositions[i].x += self->bodyVelocities[i].x;

        if (self->bodyVelocities[i].x <= 0) {
            if (self->bodyPositions[i].x > self->boundsL) {
                self->bodyVelocities[i].x -= 0xC00;

                if (self->bodyVelocities[i].x < -0x10000)
                    self->bodyVelocities[i].x = -0x10000;
            }
            else {
                self->bodyVelocities[i].x += 0xC00;

                if (self->bodyVelocities[i].x > 0x10000)
                    self->bodyVelocities[i].x = 0x10000;

                if (!self->bodyTimers[i]) {
                    if (i)
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 4, &self->bodyAnimators[i], true, 0);
                    else
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 2, &self->bodyAnimators[i], true, 0);

                    self->bodyDirections[i]   = FLIP_X;
                    self->bodyVelocities[i].y = self->startDir ? 0x10000 : -0x10000;
                    self->bodyTimers[i]       = 32;
                }
            }
        }
        else {
            if (self->bodyPositions[i].x < self->boundsR) {
                self->bodyVelocities[i].x += 0xC00;

                if (self->bodyVelocities[i].x > 0x10000)
                    self->bodyVelocities[i].x = 0x10000;
            }
            else {
                self->bodyVelocities[i].x -= 0xC00;

                if (self->bodyVelocities[i].x < -0x10000)
                    self->bodyVelocities[i].x = -0x10000;

                if (!self->bodyTimers[i]) {
                    if (i)
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 4, &self->bodyAnimators[i], true, 0);
                    else
                        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 2, &self->bodyAnimators[i], true, 0);

                    self->bodyDirections[i]   = FLIP_NONE;
                    self->bodyVelocities[i].y = self->startDir ? -0x10000 : 0x10000;
                    self->bodyTimers[i]       = 32;
                }
            }
        }

        if (self->bodyTimers[i]) {
            self->bodyTimers[i]--;
            self->bodyOriginY[i] += self->bodyVelocities[i].y;
        }

        if (!--self->flameExhaustDelays[i]) {
            RSDK.SetSpriteAnimation(Fireworm->aniFrames, 5, &self->flameAnimators[i], true, 0);
            self->flameExhaustDelays[i] = RSDK.Rand(0, 60) + 32;
        }

        self->bodyAngles[i] += 16;
        self->bodyPositions[i].y = (RSDK.Sin1024(self->bodyAngles[i]) << 8) + self->bodyOriginY[i];
        self->bodyAngles[i] %= 0xC00;
    }

    Fireworm_CheckPlayerCollisions();
    Fireworm_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void Fireworm_EditorDraw(void)
{
    RSDK_THIS(Fireworm);

    for (int32 i = 0; i < FIREWORM_SEGMENT_COUNT; ++i) {
        self->bodyPositions[i].x = self->position.x + (self->direction ? 0x100000 : -0x100000) * i;
        self->bodyPositions[i].y = self->position.y;
        self->bodyOriginY[i]     = self->position.y;
    }

    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    self->startDir   = self->direction;

    for (int32 i = 1; i < FIREWORM_SEGMENT_COUNT; ++i) {
        RSDK.SetSpriteAnimation(Fireworm->aniFrames, 3, &self->bodyAnimators[i], true, 0);
    }

    Fireworm_Draw();
}

void Fireworm_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Fireworm.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("LRZ2") || RSDK.CheckSceneFolder("LRZ3"))
        Fireworm->aniFrames = RSDK.LoadSpriteAnimation("LRZ2/Fireworm.bin", SCOPE_STAGE);

    RSDK.SetSpriteAnimation(Fireworm->aniFrames, 6, &Fireworm->holeAnimator, true, 0);

    RSDK_ACTIVE_VAR(Fireworm, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void Fireworm_Serialize(void) { RSDK_EDITABLE_VAR(Fireworm, VAR_UINT8, direction); }
