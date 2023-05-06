// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BuckwildBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBuckwildBall *BuckwildBall;

void BuckwildBall_Update(void)
{
    RSDK_THIS(BuckwildBall);

    StateMachine_Run(self->state);

    if (self->state != BuckwildBall_State_AwaitDetection && self->state != BuckwildBall_State_Debris && self->state != BuckwildBall_State_Init) {
        if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x1C0000, 2)) {
            if (self->particleDelay-- <= 0) {
                BuckwildBall_SpawnDebris();
                BuckwildBall_HandleTimerSfx();
                self->particleDelay = RSDK.Rand(8, 15);
            }
        }

        BuckwildBall_CheckPlayerCollisions();
        BuckwildBall_HandleRollCrush();

        RSDK.ProcessAnimation(&self->animator);

        if (self->timerSfx > 0)
            self->timerSfx--;
    }
}

void BuckwildBall_LateUpdate(void) {}

void BuckwildBall_StaticUpdate(void) {}

void BuckwildBall_Draw(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void BuckwildBall_Create(void *data)
{
    RSDK_THIS(BuckwildBall);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->startPos      = self->position;
        self->visible       = true;
        self->drawFX        = FX_FLIP;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->bossBallSlot  = -1;

        if (self->mode != BUCKWILDBALL_ROLLING)
            self->updateRange.x = (self->amplitude + 0x80) << 16;

        self->state = BuckwildBall_State_Init;

        if (!self->speed)
            self->speed = 2;
    }
}

void BuckwildBall_StageLoad(void)
{
    BuckwildBall->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/BuckwildBall.bin", SCOPE_STAGE);
    BuckwildBall->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);

    BuckwildBall->sfxImpact = RSDK.GetSfx("Stage/Impact2.wav");
    BuckwildBall->sfxSharp  = RSDK.GetSfx("Stage/Sharp.wav");

    BuckwildBall->hitbox.left   = -28;
    BuckwildBall->hitbox.top    = -28;
    BuckwildBall->hitbox.right  = 28;
    BuckwildBall->hitbox.bottom = 28;
}

void BuckwildBall_HandleTimerSfx(void)
{
    RSDK_THIS(BuckwildBall);

    if (self->timerSfx > 0) {
        LogHelpers_Print("timerSfx = %d", self->timerSfx);
    }
    else {
        RSDK.PlaySfx(BuckwildBall->sfxImpact, false, 255);
        self->timerSfx = 8;
    }
}

void BuckwildBall_CheckOffScreen(void)
{
    RSDK_THIS(BuckwildBall);

    if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange) && !RSDK.CheckPosOnScreen(&self->position, &self->updateRange)) {
        if (self->respawn) {
            self->position = self->startPos;
            self->state    = BuckwildBall_State_Init;
            self->active   = ACTIVE_BOUNDS;
            self->visible  = false;
        }
        else {
            destroyEntity(self);
        }
    }
}

void BuckwildBall_SpawnDebris(void)
{
    RSDK_THIS(BuckwildBall);

    int32 x = self->position.x;
    int32 y = self->position.y + 0x1C0000;

    int32 sizeX  = (BuckwildBall->hitbox.right - BuckwildBall->hitbox.left) >> 1;
    int32 spawnX = x + ((RSDK.Rand(0, 64) - 32) << 16);
    int32 spawnY = y + ((RSDK.Rand(0, 8) - 4) << 15);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);
    RSDK.SetSpriteAnimation(BuckwildBall->particleFrames, 1, &debris->animator, true, 0);
    debris->drawGroup       = Zone->objectDrawGroup[1];
    debris->gravityStrength = 0x3800;
    debris->velocity.x      = 0x180 * (abs(spawnX - x) >> 8) / sizeX;

    if (debris->position.x < self->position.x) {
        debris->direction  = FLIP_X;
        debris->velocity.x = -debris->velocity.x;
    }

    debris->velocity.y = -0x1000 * RSDK.Rand(32, 54);
}

void BuckwildBall_CheckPlayerCollisions(void)
{
    RSDK_THIS(BuckwildBall);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &BuckwildBall->hitbox)) {
#if MANIA_USE_PLUS
            if (!Player_CheckMightyUnspin(player, 0x600, false, &player->uncurlTimer))
#endif
                Player_Hurt(player, self);
        }
    }
}

void BuckwildBall_HandleRollCrush(void)
{
    RSDK_THIS(BuckwildBall);

    Hitbox crushHitbox;
    crushHitbox.left   = -8;
    crushHitbox.top    = -8;
    crushHitbox.right  = 8;
    crushHitbox.bottom = 8;

    foreach_active(Iwamodoki, iwamodoki)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, iwamodoki, &crushHitbox)) {
            BadnikHelpers_BadnikBreak(iwamodoki, true, true);
        }
    }

    foreach_active(Fireworm, fireworm)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, fireworm, &crushHitbox)) {
            BadnikHelpers_BadnikBreak(fireworm, true, true);
        }
    }

    foreach_active(Toxomister, toxomister)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, toxomister, &crushHitbox)) {
            if (toxomister->state == Toxomister_State_CreateClouds) {
                BadnikHelpers_BadnikBreak(toxomister, true, true);
            }
            else if (!toxomister->grabbedPlayer) {
                destroyEntity(toxomister);
            }
        }
    }

    Hitbox spikeHitbox;
    spikeHitbox.left   = -16;
    spikeHitbox.top    = -16;
    spikeHitbox.right  = 16;
    spikeHitbox.bottom = 16;

    foreach_active(Spikes, spikes)
    {
        if (RSDK.CheckObjectCollisionTouchBox(self, &BuckwildBall->hitbox, spikes, &spikeHitbox)) {
            for (int32 i = 0; i < 2; ++i) {
                int32 x              = spikes->position.x + (((2 * (i != 0) - 1) * (spikes->type == SPIKES_UP)) << 19);
                int32 y              = spikes->position.y + (((2 * (i != 0) - 1) * (spikes->type != SPIKES_UP)) << 19);
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, x, y);

                RSDK.SetSpriteAnimation(BuckwildBall->particleFrames, 4, &debris->animator, true, spikes->type >> 1);
                debris->drawGroup = Zone->objectDrawGroup[1];
                debris->direction = spikes->direction;
                debris->drawFX |= FX_ROTATE;
                debris->gravityStrength = 0x3800;
                debris->rotSpeed        = RSDK.Rand(-32, 32);
                debris->velocity.x      = RSDK.Rand(-0x28000, 0x28000);
                debris->velocity.y      = -0x1000 * RSDK.Rand(32, 96);
            }

            destroyEntity(spikes);

            RSDK.PlaySfx(BuckwildBall->sfxSharp, false, 255);
            RSDK.PlaySfx(BuckwildBall->sfxImpact, false, 255);
            self->timerSfx = 8;
        }
    }
}

void BuckwildBall_State_Init(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &self->animator, true, 0);

    self->timerSfx = 0;

    switch (self->mode) {
        case BUCKWILDBALL_PATROLLING: self->state = BuckwildBall_State_Patrolling; break;

        case BUCKWILDBALL_ROLLING:
            self->visible   = false;
            self->state     = BuckwildBall_State_AwaitDetection;
            self->drawGroup = Zone->objectDrawGroup[0];

            self->detectHitbox.left   = -(self->detectSize.x >> 17);
            self->detectHitbox.top    = -(self->detectSize.y >> 17);
            self->detectHitbox.right  = self->detectSize.x >> 17;
            self->detectHitbox.bottom = self->detectSize.y >> 17;

            self->velocity.x = 0;
            self->ballPos.x  = self->startPos.x + self->detectOffset.x;
            self->ballPos.y  = self->startPos.y + self->detectOffset.y;
            self->velocity.y = 0;
            break;
    }
}

void BuckwildBall_State_Patrolling(void)
{
    RSDK_THIS(BuckwildBall);

    int32 angle      = ((self->speed & 0xFFFF) * (Zone->timer & 0xFFFF)) & 0x3FF;
    self->position.x = (self->amplitude << 6) * RSDK.Sin1024(angle) + self->startPos.x;
    self->direction  = (angle - 0x100) > 0x200;
}

void BuckwildBall_State_AwaitDetection(void)
{
    RSDK_THIS(BuckwildBall);

    self->position.x = self->ballPos.x;
    self->position.y = self->ballPos.y;

    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            if (Player_CheckCollisionTouch(player, self, &self->detectHitbox)) {
                self->visible = true;
                self->active  = ACTIVE_NORMAL;
                self->state   = BuckwildBall_State_Falling;
            }
        }
    }

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

void BuckwildBall_State_Falling(void)
{
    RSDK_THIS(BuckwildBall);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x1C0000, true)) {
        if (!RSDK.GetEntityCount(Drillerdroid->classID, true) || (Drillerdroid->platformActive[self->bossBallSlot] == true)) {
            self->velocity.y = 0;
            self->state      = BuckwildBall_State_Rolling;
            self->velocity.x = abs(self->speed << 15) * (2 * (self->direction != FLIP_NONE) - 1);
        }
        else if (Drillerdroid->platformActive[self->bossBallSlot]) {
            self->state          = BuckwildBall_State_Debris;
            self->velocity.x     = 0;
            self->velocity.y     = -0x40000;
            self->animator.speed = 0;
            self->drawGroup      = Zone->objectDrawGroup[1];
        }
        else {
            int32 slot                                                                        = RSDK.GetEntitySlot(Drillerdroid->boss);
            RSDK_GET_ENTITY(slot + 6 + self->bossBallSlot, CollapsingPlatform)->collapseDelay = 1;
            Drillerdroid->platformActive[self->bossBallSlot]                                  = -1;
            self->velocity.y                                                                  = -0x40000;
        }

        BuckwildBall_HandleTimerSfx();
        Camera_ShakeScreen(0, 0, 5);
    }

    if (self->bossBallSlot > -1)
        BuckwildBall_CheckOffScreen();
}

void BuckwildBall_State_Rolling(void)
{
    RSDK_THIS(BuckwildBall);

    self->velocity.x += abs(self->speed << 10) * (2 * !(self->direction == FLIP_NONE) - 1);
    self->velocity.y += 0x3800;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileGrip(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x1C0000, 2)) {
        self->velocity.y = 0;
        if (abs(self->velocity.x) > 0x20000 && RSDK.Rand(0, 100) > 0x50) {
            BuckwildBall_SpawnDebris();
            BuckwildBall_HandleTimerSfx();
            self->velocity.y = -0x18000;
        }
    }

    bool32 collidedWall = false;
    if (self->direction == FLIP_X)
        collidedWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, 0x1C0000, 0, true);
    else if (self->direction == FLIP_NONE)
        collidedWall = RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -0x1C0000, 0, true);

    if (collidedWall) {
        self->state = BuckwildBall_State_Debris;
        BuckwildBall_HandleTimerSfx();
        Camera_ShakeScreen(0, 0, 5);
        self->velocity.x     = 0;
        self->velocity.y     = -0x40000;
        self->animator.speed = 0;
        self->drawGroup      = Zone->objectDrawGroup[1];
    }

    if (self->bossBallSlot > -1)
        BuckwildBall_CheckOffScreen();
}

void BuckwildBall_State_Debris(void)
{
    RSDK_THIS(BuckwildBall);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    self->visible = (Zone->timer % 4) < 2;

    BuckwildBall_CheckOffScreen();
}

#if GAME_INCLUDE_EDITOR
void BuckwildBall_EditorDraw(void)
{
    RSDK_THIS(BuckwildBall);

    RSDK.SetSpriteAnimation(BuckwildBall->aniFrames, 0, &self->animator, true, 0);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->startPos      = self->position;

    BuckwildBall_Draw();

    if (showGizmos()) {

        RSDK_DRAWING_OVERLAY(true);
        switch (self->mode) {
            case BUCKWILDBALL_PATROLLING:
                self->updateRange.x = (self->amplitude + 0x80) << 16;

                self->inkEffect = INK_BLEND;

                self->position.x = (self->amplitude << 6) * RSDK.Sin1024(0x100) + self->startPos.x;
                int32 x1         = self->position.x;
                BuckwildBall_Draw();

                self->position.x = (self->amplitude << 6) * RSDK.Sin1024(0x300) + self->startPos.x;
                BuckwildBall_Draw();

                RSDK.DrawLine(x1, self->position.y, self->position.x, self->position.y, 0x00FF00, 0xFF, INK_BLEND, false);

                self->inkEffect = INK_NONE;
                break;

            case BUCKWILDBALL_ROLLING:
                self->detectHitbox.left   = -(self->detectSize.x >> 17);
                self->detectHitbox.top    = -(self->detectSize.y >> 17);
                self->detectHitbox.right  = self->detectSize.x >> 17;
                self->detectHitbox.bottom = self->detectSize.y >> 17;
                self->ballPos             = self->position;
                self->ballPos.x           = self->position.x + self->detectOffset.x;
                self->ballPos.y           = self->position.y + self->detectOffset.y;

                DrawHelpers_DrawHitboxOutline(self->ballPos.x, self->ballPos.y, &self->detectHitbox, FLIP_NONE, 0xFF0000);

                self->velocity.x = abs(self->speed << 15) * (2 * (self->direction != FLIP_NONE) - 1);
                DrawHelpers_DrawArrow(self->position.x, self->position.y, self->position.x + (self->velocity.x << 3), self->position.y, 0xFFFF00,
                                      INK_NONE, 0xFF);
                break;
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void BuckwildBall_EditorLoad(void)
{
    BuckwildBall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/BuckwildBall.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(BuckwildBall, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    RSDK_ACTIVE_VAR(BuckwildBall, mode);
    RSDK_ENUM_VAR("Patrolling", BUCKWILDBALL_PATROLLING);
    RSDK_ENUM_VAR("Rolling", BUCKWILDBALL_ROLLING);
}
#endif

void BuckwildBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT32, amplitude);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_UINT8, mode);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(BuckwildBall, VAR_BOOL, respawn);
}
