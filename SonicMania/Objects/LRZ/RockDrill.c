// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RockDrill Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRockDrill *RockDrill;

void RockDrill_Update(void)
{
    RSDK_THIS(RockDrill);

    StateMachine_Run(self->state);
}

void RockDrill_LateUpdate(void) {}

void RockDrill_StaticUpdate(void)
{
    if (RockDrill->drillSfxTimer) {
        if (!RockDrill->playingDrillSfx) {
            RSDK.PlaySfx(RockDrill->sfxDrill, 43643, 0xFF);
            RockDrill->playingDrillSfx = true;
        }

        RockDrill->drillSfxTimer = 0;
    }
    else if (RockDrill->playingDrillSfx) {
        RSDK.StopSfx(RockDrill->sfxDrill);
        RockDrill->playingDrillSfx = false;
    }
}

void RockDrill_Draw(void)
{
    RSDK_THIS(RockDrill);

    if (self->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }

    // Piston (L1)
    self->animator.frameID = 1;
    Vector2 drawPos;
    drawPos.x = self->position.x - 0x260000;
    drawPos.y = self->position.y - self->pistonPos[0];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Drill (L1)
    self->animator.frameID = 2;
    drawPos.y              = self->position.y + self->drillPos[0];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Piston (L2)
    self->animator.frameID = 1;
    drawPos.x += 0xA0000;
    drawPos.y = self->position.y - self->pistonPos[1];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Drill (L2)
    self->animator.frameID = 2;
    drawPos.y              = self->position.y + self->drillPos[1];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Piston (R1)
    self->animator.frameID = 1;
    drawPos.x += 0x420000;
    drawPos.y = self->position.y - self->pistonPos[0];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Drill (R1)
    self->animator.frameID = 2;
    drawPos.y              = self->position.y + self->drillPos[0];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Piston (R2)
    self->animator.frameID = 1;
    drawPos.x -= 0xA0000;
    drawPos.y = self->position.y - self->pistonPos[1];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Drill (R2)
    self->animator.frameID = 2;
    drawPos.y              = self->position.y + self->drillPos[1];
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    // Main Body
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, NULL, false);

    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void RockDrill_Create(void *data)
{
    RSDK_THIS(RockDrill);

    if (!SceneInfo->inEditor) {
        self->active         = ACTIVE_BOUNDS;
        self->visible        = true;
        self->updateRange.x  = 0x800000;
        self->updateRange.y  = 0x800000;
        self->drillDelay[0]  = 4;
        self->pistonDelay[0] = 4;

        RSDK.SetSpriteAnimation(RockDrill->aniFrames, 0, &self->animator, true, 0);

        self->drawGroup = Zone->objectDrawGroup[0];
        self->state     = RockDrill_State_Init;
    }
}

void RockDrill_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        RockDrill->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/RockDrill.bin", SCOPE_STAGE);

    RockDrill->hitboxBody.left   = -47;
    RockDrill->hitboxBody.top    = -16;
    RockDrill->hitboxBody.right  = 47;
    RockDrill->hitboxBody.bottom = 16;

    RockDrill->hitboxPistonL.left   = -47;
    RockDrill->hitboxPistonL.top    = -56;
    RockDrill->hitboxPistonL.right  = -16;
    RockDrill->hitboxPistonL.bottom = -16;

    RockDrill->hitboxPistonR.left   = 16;
    RockDrill->hitboxPistonR.top    = -56;
    RockDrill->hitboxPistonR.right  = 47;
    RockDrill->hitboxPistonR.bottom = -16;

    RockDrill->hitboxDrills.left   = -47;
    RockDrill->hitboxDrills.top    = 16;
    RockDrill->hitboxDrills.right  = -47;
    RockDrill->hitboxDrills.bottom = 37;

    RockDrill->active = ACTIVE_ALWAYS;

    RockDrill->drillSfxTimer   = 0;
    RockDrill->playingDrillSfx = false;
    RockDrill->sfxHit          = RSDK.GetSfx("Stage/BossHit.wav");
    RockDrill->sfxExplosion    = RSDK.GetSfx("Stage/Explosion2.wav");
    RockDrill->sfxDrill        = RSDK.GetSfx("LRZ/Drill.wav");
}

void RockDrill_CheckPlayerCollisions(void)
{
    RSDK_THIS(RockDrill);

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckBadnikTouch(player, self, &RockDrill->hitboxBody) || Player_CheckBadnikTouch(player, self, &RockDrill->hitboxPistonL)
                || Player_CheckBadnikTouch(player, self, &RockDrill->hitboxPistonR)) {
                if (Player_CheckBossHit(player, self)) {
                    RockDrill_Hit();
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &RockDrill->hitboxDrills))
                    Player_Hurt(player, self);
            }
        }
    }
}

void RockDrill_Hit(void)
{
    RSDK_THIS(RockDrill);
    RSDK.PlaySfx(RockDrill->sfxHit, false, 255);
    self->invincibilityTimer = 30;
}

void RockDrill_Explode(void)
{
    RSDK_THIS(RockDrill);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);

        if (!(Zone->timer & 3)) {
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);

            explosion->drawGroup = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void RockDrill_SpawnDebris(int32 offset)
{
    RSDK_THIS(RockDrill);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x      = RSDK.Rand(0, 6) << 15;
    debris->velocity.y      = RSDK.Rand(-12, -8) << 15;
    debris->gravityStrength = 0x3800;
    debris->drawGroup       = Zone->objectDrawGroup[0];
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x      = RSDK.Rand(-6, 0) << 15;
    debris->velocity.y      = RSDK.Rand(-12, -8) << 15;
    debris->gravityStrength = 0x3800;
    debris->drawGroup       = Zone->objectDrawGroup[0];
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;
}

void RockDrill_State_Init(void)
{
    RSDK_THIS(RockDrill);

    int32 slot                         = SceneInfo->entitySlot - 1;
    EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
    while (platform->classID == CollapsingPlatform->classID) {
        platform->active = ACTIVE_NEVER;
        platform         = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
    }

    self->active = ACTIVE_NORMAL;
    self->state  = RockDrill_State_Drilling;

    if (self->lockCamera) {
        self->boundsL = Zone->cameraBoundsL[0];
        self->boundsR = Zone->cameraBoundsR[0];
        self->boundsT = Zone->cameraBoundsT[0];
        self->boundsB = Zone->cameraBoundsB[0];

        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x - 96;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->center.x - 96;
        Zone->cameraBoundsB[0] = (self->position.y >> 16) + 96;
    }

    self->timer = 120;
}

void RockDrill_State_Drilling(void)
{
    RSDK_THIS(RockDrill);

    ++RockDrill->drillSfxTimer;
    RSDK.ProcessAnimation(&self->animator);

    for (int32 i = 0; i < 2; ++i) {
        if (self->pistonDelay[i]) {
            self->pistonDelay[i]--;
        }
        else if (self->pistonMoveDir[i]) {
            self->pistonPos[i] -= 0x20000;
            if (!self->pistonPos[i]) {
                self->pistonMoveDir[i] = FLIP_NONE;
                self->pistonDelay[i]   = 10;
            }
        }
        else {
            self->pistonPos[i] += 0x40000;
            if (self->pistonPos[i] == 0x100000)
                self->pistonMoveDir[i] = FLIP_X;
        }

        if (self->drillDelay[i]) {
            self->drillDelay[i]--;
        }
        else if (self->drillMoveDir[i]) {
            self->drillPos[i] -= 0x40000;
            if (!self->drillPos[i]) {
                self->drillMoveDir[i] = FLIP_NONE;
                self->drillDelay[i]   = 5;
            }
        }
        else {
            self->drillPos[i] += 0x80000;
            if (self->drillPos[i] == 0x100000)
                self->drillMoveDir[i] = FLIP_X;
        }
    }

    if (self->drillPos[0] == 0x80000) {
        RockDrill_SpawnDebris(-0x260000);
        RockDrill_SpawnDebris(0x260000);
    }
    else if (self->drillPos[1] == 0x80000) {
        RockDrill_SpawnDebris(-0x1C0000);
        RockDrill_SpawnDebris(0x1C0000);
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;

    self->position.y ^= 0x10000;
    RockDrill_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->active = ACTIVE_BOUNDS;
        self->state  = RockDrill_State_Init;
    }

    if (--self->timer <= 0) {
        foreach_active(LRZRockPile, pile)
        {
            pile->canCollapse  = true;
            pile->active       = ACTIVE_NORMAL;
            pile->timer        = 0;
            pile->destroyTimer = 0;
        }

        if (self->lockCamera) {
            Zone->cameraBoundsL[0] = self->boundsL;
            Zone->cameraBoundsR[0] = self->boundsR;
            Zone->cameraBoundsT[0] = self->boundsT;
            Zone->cameraBoundsB[0] = self->boundsB;
        }

        self->timer  = 30;
        self->state  = RockDrill_State_Falling;
        self->active = ACTIVE_NORMAL;
    }
}

void RockDrill_State_Falling(void)
{
    RSDK_THIS(RockDrill);

    if (self->timer-- <= 0) {
        self->velocity.y += 0x3800;
        self->position.y += self->velocity.y;

        uint16 tileLow  = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 20, (self->position.y + 0x200000) >> 20);
        uint16 tileHigh = RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, (self->position.y + 0x200000) >> 20);

        if (RSDK.GetTileFlags(tileLow, 0) == LRZ2_TFLAGS_LAVA || RSDK.GetTileFlags(tileHigh, 0) == LRZ2_TFLAGS_LAVA) {
            self->timer      = 0;
            self->velocity.y = 0x8000;
            self->drawGroup  = Zone->objectDrawGroup[0] - 1;
            self->state      = RockDrill_State_Explode;
        }
        else {
            if (!RSDK.CheckOnScreen(self, &self->updateRange))
                destroyEntity(self);
        }
    }
}

void RockDrill_State_Explode(void)
{
    RSDK_THIS(RockDrill);

    self->position.y += self->velocity.y;

    RockDrill_Explode();

    if (++self->timer > 120) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x      = RSDK.Rand(-6, 6) << 15;
        debris->velocity.y      = RSDK.Rand(-10, -6) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[0];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void RockDrill_EditorDraw(void)
{
    RSDK_THIS(RockDrill);

    self->active         = ACTIVE_BOUNDS;
    self->updateRange.x  = 0x800000;
    self->updateRange.y  = 0x800000;
    self->drillDelay[0]  = 4;
    self->pistonDelay[0] = 4;

    RSDK.SetSpriteAnimation(RockDrill->aniFrames, 0, &self->animator, true, 0);

    RockDrill_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        if (self->lockCamera)
            DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER - 96, 0, WIDE_SCR_XCENTER - 96, 96, 1 | 0 | 4 | 8, 0x00C0F0);

        // Draw Arrows to all "child" platforms that'll be set to inactive
        if (CollapsingPlatform) {
            int32 slot                         = SceneInfo->entitySlot - 1;
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
            while (platform && platform->classID == CollapsingPlatform->classID) {
                DrawHelpers_DrawArrow(self->position.x, self->position.y, platform->position.x, platform->position.y, 0xFFFF00, INK_NONE, 0xFF);

                platform = RSDK_GET_ENTITY(slot--, CollapsingPlatform);
            }
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void RockDrill_EditorLoad(void) { RockDrill->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/RockDrill.bin", SCOPE_STAGE); }
#endif

void RockDrill_Serialize(void) { RSDK_EDITABLE_VAR(RockDrill, VAR_BOOL, lockCamera); }
