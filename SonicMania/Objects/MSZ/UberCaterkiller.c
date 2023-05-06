// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UberCaterkiller Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUberCaterkiller *UberCaterkiller;

void UberCaterkiller_Update(void)
{
    RSDK_THIS(UberCaterkiller);

    StateMachine_Run(self->state);
}

void UberCaterkiller_LateUpdate(void) {}

void UberCaterkiller_StaticUpdate(void)
{
    foreach_all(UberCaterkiller, boss) { RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(boss)); }
}

void UberCaterkiller_Draw(void)
{
    RSDK_THIS(UberCaterkiller);

    int32 clipY2 = ScreenInfo->clipBound_Y2;

    if (self->aniID) {
        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        // Draw Head
        if (self->bodyScales[0] < 0x200 == (SceneInfo->currentDrawGroup == self->drawGroup)) {
            ScreenInfo->clipBound_Y2 = MIN(((self->bodyScales[0] - 0x100) >> 1) + 160, ScreenInfo->size.y);
            self->scale.x            = self->bodyScales[0];
            self->scale.y            = self->bodyScales[0];
            RSDK.DrawSprite(self->bodyAnimators[0], self->bodyPositions, false);
        }

        RSDK.SetPaletteEntry(0, 160, 0x200000);

        // Draw Body Segments
        self->direction ^= FLIP_X;
        for (int32 i = 1; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
            ScreenInfo->clipBound_Y2 = MIN(((self->bodyScales[i] - 0x100) >> 1) + 160, ScreenInfo->size.y);

            self->rotation = (2 * self->bodyAngles[i] - 15) & 0x1E;
            if (self->bodyScales[i] < 0x200 == (SceneInfo->currentDrawGroup == self->drawGroup)) {
                self->scale.x              = self->bodyScales[1];
                self->scale.y              = self->bodyScales[1];
                self->bodyAnimator.frameID = self->bodyAngles[i] >> 4;
                RSDK.DrawSprite(self->bodyAnimators[i], &self->bodyPositions[i], false);
            }
        }

        self->direction ^= FLIP_X;
        self->rotation = 0;
    }
    else {
        self->direction ^= FLIP_X;

        // Draw Body Segments
        for (int32 i = UBERCATERKILLER_SEGMENT_COUNT - 1; i > 0; --i) {
            ScreenInfo->clipBound_Y2 = MIN(((self->bodyScales[i] - 0x100) >> 1) + 160, ScreenInfo->size.y);
            self->rotation           = (2 * self->bodyAngles[i] - 15) & 0x1E;
            if (self->bodyScales[i] < 0x200 == (SceneInfo->currentDrawGroup == self->drawGroup)) {
                self->scale.x              = self->bodyScales[i];
                self->scale.y              = self->bodyScales[i];
                self->bodyAnimator.frameID = self->bodyAngles[i] >> 4;
                RSDK.DrawSprite(self->bodyAnimators[i], &self->bodyPositions[i], false);
            }
        }

        self->direction ^= FLIP_X;
        self->rotation = 0;

        if (self->invincibilityTimer & 1)
            RSDK.SetPaletteEntry(0, 160, 0xE0E0E0);

        // Draw Head
        if (self->bodyScales[0] < 0x200 == (SceneInfo->currentDrawGroup == self->drawGroup)) {
            ScreenInfo->clipBound_Y2 = MIN(((self->bodyScales[0] - 0x100) >> 1) + 160, ScreenInfo->size.y);
            self->scale.x            = self->bodyScales[0];
            self->scale.y            = self->bodyScales[0];
            RSDK.DrawSprite(self->bodyAnimators[0], &self->bodyPositions[0], false);
        }

        RSDK.SetPaletteEntry(0, 160, 0x200000);
    }

    ScreenInfo->clipBound_Y2 = clipY2;
}

void UberCaterkiller_Create(void *data)
{
    RSDK_THIS(UberCaterkiller);

    if (!SceneInfo->inEditor) {
        self->visible       = false;
        self->drawFX        = FX_SCALE | FX_ROTATE | FX_FLIP;
        self->drawGroup     = Zone->objectDrawGroup[0] - 1;
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->timer         = 0;

        for (int32 i = 1; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) self->bodyAnimators[i] = &self->bodyAnimator;
        self->bodyAnimators[0] = &self->headAnimator;

        self->health = 6;
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &self->headAnimator, true, 0);
        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 2, &self->bodyAnimator, true, 0);
        self->state = UberCaterkiller_State_SetupArena;
    }
}

void UberCaterkiller_StageLoad(void)
{
    UberCaterkiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE);

    // Unused, the same hitbox IS used in DBTower though
    UberCaterkiller->hitboxSegment.left   = -27;
    UberCaterkiller->hitboxSegment.top    = -27;
    UberCaterkiller->hitboxSegment.right  = 27;
    UberCaterkiller->hitboxSegment.bottom = 27;

    UberCaterkiller->defeated = false;
    UberCaterkiller->active   = ACTIVE_ALWAYS;

    UberCaterkiller->sfxRocketJet  = RSDK.GetSfx("Stage/RocketJet.wav");
    UberCaterkiller->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    UberCaterkiller->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    UberCaterkiller->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");
    UberCaterkiller->sfxSandSwim   = RSDK.GetSfx("MSZ/SandSwim.wav");
    UberCaterkiller->sfxBumper3    = RSDK.GetSfx("Stage/Bumper3.wav");
    UberCaterkiller->sfxCaterJump  = RSDK.GetSfx("MSZ/CaterJump.wav");

    DEBUGMODE_ADD_OBJ(UberCaterkiller);
}

void UberCaterkiller_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    CREATE_ENTITY(UberCaterkiller, INT_TO_VOID(-0x20000), self->position.x, self->position.y);
}

void UberCaterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void UberCaterkiller_CheckPlayerCollisions(void)
{
    RSDK_THIS(UberCaterkiller);

    if (self->invincibilityTimer > 0) {
        self->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (self->playerTimers[playerID]) {
                --self->playerTimers[playerID];
            }
            else {
                self->position.x = self->bodyPositions[0].x;
                self->position.y = self->bodyPositions[0].y;
                int32 scale      = self->bodyScales[0];

                Hitbox hitboxSegment;
                hitboxSegment.right  = (32 * scale) >> 9;
                hitboxSegment.bottom = hitboxSegment.right;
                hitboxSegment.left   = -hitboxSegment.right;
                hitboxSegment.top    = -hitboxSegment.right;

                if ((scale > 0x1C0 && scale < 0x240) && Player_CheckBadnikTouch(player, self, &hitboxSegment) && Player_CheckBossHit(player, self)) {
                    UberCaterkiller_Hit();
                    foreach_break;
                }
                else {
                    for (int32 i = 1; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
                        self->position.x = self->bodyPositions[i].x;
                        self->position.y = self->bodyPositions[i].y;
                        scale            = self->bodyScales[i];

                        if (scale > 0x1C0 && scale < 0x240) {
                            uint8 angle =
                                RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y) - self->bodyAngles[i];
                            hitboxSegment.right  = (28 * scale) >> 9;
                            hitboxSegment.bottom = hitboxSegment.right;
                            hitboxSegment.left   = -hitboxSegment.right;
                            hitboxSegment.top    = -hitboxSegment.right;
                            if (Player_CheckBadnikTouch(player, self, &hitboxSegment)) {
                                if (angle >= 0x80) {
#if MANIA_USE_PLUS
                                    if (!Player_CheckMightyUnspin(player, 0x400, true, &player->uncurlTimer))
#endif
                                        Player_Hurt(player, self);
                                }
                                else if (Player_CheckBossHit(player, self)) {
                                    RSDK.PlaySfx(UberCaterkiller->sfxBumper3, false, 255);
                                    self->playerTimers[playerID] = 30;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    self->position.x = self->bodyPositions[0].x;
    self->position.y = self->bodyPositions[0].y;
}

void UberCaterkiller_Hit(void)
{
    RSDK_THIS(UberCaterkiller);

    if (--self->health <= 0) {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);
        self->timer = 120;
        self->state = UberCaterkiller_State_Destroyed;
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(UberCaterkiller->sfxHit, false, 255);
    }
}

void UberCaterkiller_Explode(void)
{
    RSDK_THIS(UberCaterkiller);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(UberCaterkiller->sfxExplosion2, false, 255);

        if (!(Zone->timer & 7)) {
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void UberCaterkiller_HandleSegmentMoveFX(int32 segmentID)
{
    RSDK_THIS(UberCaterkiller);

    if (self->bodyVelocity[segmentID].y >= 0) {
        if (self->bodyInAir[segmentID]) {
            int32 targetY = MIN(((self->bodyScales[segmentID] - 0x100) >> 1) + 0xA0, 0x100);

            if ((self->bodyPositions[segmentID].y >> 16) - ScreenInfo->position.y > targetY) {
                RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 0xFF);
                int32 x = self->bodyPositions[segmentID].x;
                int32 y = (((self->bodyScales[segmentID] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16);
                EntityExplosion *sandParticles = CREATE_ENTITY(Explosion, NULL, x, y);

                RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &sandParticles->animator, true, 0);
                sandParticles->drawFX     = FX_SCALE;
                sandParticles->scale.x    = self->bodyScales[segmentID];
                sandParticles->scale.y    = self->bodyScales[segmentID];
                sandParticles->drawGroup  = self->drawGroup;
                sandParticles->velocity.x = -0x10000;

                self->bodyInAir[segmentID] = false;
            }
        }
    }
    else if (!self->bodyInAir[segmentID]) {
        int32 targetY = MIN(((self->bodyScales[segmentID] - 0x100) >> 1) + 0xA0, 0x100);

        if ((self->bodyPositions[segmentID].y >> 16) - ScreenInfo->position.y < targetY) {
            RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 0xFF);
            int32 x                        = self->bodyPositions[segmentID].x;
            int32 y                        = (((self->bodyScales[segmentID] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16);
            EntityExplosion *sandParticles = CREATE_ENTITY(Explosion, NULL, x, y);

            RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &sandParticles->animator, true, 0);
            sandParticles->drawFX      = FX_SCALE;
            sandParticles->scale.x     = self->bodyScales[segmentID];
            sandParticles->scale.y     = self->bodyScales[segmentID];
            sandParticles->drawGroup   = self->drawGroup;
            sandParticles->velocity.x  = -0x10000;
            self->bodyInAir[segmentID] = true;
        }
    }
}

void UberCaterkiller_SetupBodySegments(int32 x, int32 y)
{
    RSDK_THIS(UberCaterkiller);

    int32 delay = 0;
    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
        self->bodyVelocity[i].x  = x;
        self->bodyVelocity[i].y  = y;
        self->bodyTimers[i]      = delay;
        self->bodyPositions[i].x = self->position.x;
        self->bodyPositions[i].y = self->position.y;

        delay += 4;
    }

    self->direction = x < 0;
}

void UberCaterkiller_State_SetupArena(void)
{
    RSDK_THIS(UberCaterkiller);

    if (++self->timer == 60) {
        self->timer   = 0;
        self->visible = true;

        for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) self->bodyPositions[i].x = (ScreenInfo->position.x + 64) << 16;

        self->position.x  = self->bodyPositions[0].x;
        self->position.y  = self->bodyPositions[0].y;
        self->targetScale = 0x100;
        UberCaterkiller_PrepareMoveIntoBG();
    }
}

void UberCaterkiller_PrepareMoveIntoBG(void)
{
    RSDK_THIS(UberCaterkiller);

    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) self->bodyScales[i] = 0x280;

    self->state = UberCaterkiller_State_MoveIntoBG;
}

void UberCaterkiller_State_MoveIntoBG(void)
{
    RSDK_THIS(UberCaterkiller);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

    if (!(Zone->timer & 3)) {
        int32 x                    = self->bodyPositions[0].x;
        int32 y                    = (((self->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, NULL, x, y);

        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX     = FX_SCALE;
        explosion->scale.x    = self->bodyScales[0];
        explosion->scale.y    = self->bodyScales[0];
        explosion->drawGroup  = Zone->objectDrawGroup[0] - 1;
        explosion->velocity.x = -0x10000;
    }

    if (self->bodyScales[0] >= self->targetScale) {
        self->bodyScales[0] -= 4;
        if (self->bodyScales[0] < self->targetScale)
            self->bodyScales[0] = self->targetScale;
    }
    else {
        self->bodyScales[0] += 4;
        if (self->bodyScales[0] > self->targetScale)
            self->bodyScales[0] = self->targetScale;
    }

    if (self->bodyScales[0] == self->targetScale) {
        if (self->targetBodyPos) {
            self->state = UberCaterkiller_State_MoveToTargetPos;
        }
        else {
            self->timer = 60;
            self->state = UberCaterkiller_State_PrepareBGJump;
        }
    }
}

void UberCaterkiller_State_MoveToTargetPos(void)
{
    RSDK_THIS(UberCaterkiller);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

    if (!(Zone->timer & 3)) {
        int32 x                    = self->bodyPositions[0].x;
        int32 y                    = (((self->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, NULL, x, y);

        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX     = FX_SCALE;
        explosion->scale.x    = self->bodyScales[0];
        explosion->scale.y    = self->bodyScales[0];
        explosion->drawGroup  = Zone->objectDrawGroup[0] - 1;
        explosion->velocity.x = -0x10000;
    }

    if (self->bodyPositions[0].x >= self->targetBodyPos) {
        self->bodyPositions[0].x -= 0x40000;
        if (self->bodyPositions[0].x < self->targetBodyPos)
            self->bodyPositions[0].x = self->targetBodyPos;
    }
    else {
        self->bodyPositions[0].x += 0x20000;
        if (self->bodyPositions[0].x > self->targetBodyPos)
            self->bodyPositions[0].x = self->targetBodyPos;
    }

    if (self->bodyPositions[0].x == self->targetBodyPos) {
        self->timer = 60;
        self->state = UberCaterkiller_State_PrepareHorizontalJump;
    }
}

void UberCaterkiller_State_PrepareHorizontalJump(void)
{
    RSDK_THIS(UberCaterkiller);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

    if (--self->timer <= 0) {
        self->timer = 0;

        int32 delay = 0;
        for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
            self->bodyVelocity[i].x  = 0;
            self->bodyVelocity[i].y  = -0x80000;
            self->bodyTimers[i]      = delay;
            self->bodyPositions[i].x = self->targetBodyPos;
            self->bodyPositions[i].y = (ScreenInfo->position.y + SCREEN_YSIZE) << 16;
            self->bodyScales[i]      = 0x200;

            delay++;
        }

        self->position.x    = self->bodyPositions[0].x;
        self->position.y    = self->bodyPositions[0].y;
        self->targetBodyPos = 0;

        if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].x <= (ScreenInfo->position.x + ScreenInfo->center.x) << 16)
            UberCaterkiller_SetupBodySegments(0x40000, -0x60000);
        else
            UberCaterkiller_SetupBodySegments(-0x40000, -0x60000);

        RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        self->state = UberCaterkiller_State_HorizontalJump;
    }
}

void UberCaterkiller_State_HorizontalJump(void)
{
    RSDK_THIS(UberCaterkiller);

    RSDK.ProcessAnimation(&self->headAnimator);

    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
        UberCaterkiller_HandleSegmentMoveFX(i);

        if (self->bodyTimers[i] > 0) {
            self->bodyTimers[i]--;
            if (!self->bodyTimers[i])
                RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);
        }
        else {
            self->bodyVelocity[i].y += 0x1C00;
            self->bodyPositions[i].y += self->bodyVelocity[i].y;
            if (self->bodyInAir[i])
                self->bodyPositions[i].x += self->bodyVelocity[i].x;
        }
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].y > (ScreenInfo->position.y + ScreenInfo->size.y + 64) << 16) {
        self->targetScale = 0x100;
        UberCaterkiller_PrepareMoveIntoBG();

        switch (RSDK.Rand(0, 3)) {
            default:
            case 0: // jump from BG
                break;

            case 1: // jump from left
                self->targetBodyPos = (ScreenInfo->position.x - 32) << 16;
                break;

            case 2: // jump from right
                self->targetBodyPos = (ScreenInfo->position.x + ScreenInfo->size.x + 32) << 16;
                break;
        }
    }
}

void UberCaterkiller_State_PrepareBGJump(void)
{
    RSDK_THIS(UberCaterkiller);

    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) self->bodyPositions[i].x -= 0x10000;

    if (--self->timer <= 0) {
        self->timer = 0;

        int32 delay = 0;
        for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
            self->bodyVelocity[i].x  = 0;
            self->bodyVelocity[i].y  = -0x80000;
            self->bodyTimers[i]      = delay;
            self->bodyPositions[i].y = (ScreenInfo->position.y + 160) << 16;
            self->bodyScales[i]      = self->targetScale;

            delay++;
        }

        self->position.x  = self->bodyPositions[0].x;
        self->position.y  = self->bodyPositions[0].y;
        self->jumpsRemain = RSDK.Rand(0, 2);

        if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].x <= (ScreenInfo->position.x + ScreenInfo->center.x) << 16)
            UberCaterkiller_SetupBodySegments(0x18000, -0x80000);
        else
            UberCaterkiller_SetupBodySegments(-0x18000, -0x80000);

        RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        self->state = UberCaterkiller_State_FirstJump;
    }
}

void UberCaterkiller_State_FirstJump(void)
{
    RSDK_THIS(UberCaterkiller);

    RSDK.ProcessAnimation(&self->headAnimator);

    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
        if (self->bodyTimers[i] > 0) {
            self->bodyTimers[i]--;
        }
        else {
            self->bodyVelocity[i].y += 0x3800;

            self->bodyPositions[i].x += self->bodyVelocity[i].x;
            self->bodyPositions[i].y += self->bodyVelocity[i].y;
            if (self->bodyScales[i] < 0x400)
                self->bodyScales[i] += 8;
        }

        UberCaterkiller_HandleSegmentMoveFX(i);
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].y > (ScreenInfo->position.y + ScreenInfo->size.y + 64) << 16) {
        RSDK.PlaySfx(UberCaterkiller->sfxSandSwim, false, 255);

        int32 x                    = self->bodyPositions[0].x;
        int32 y                    = (((self->bodyScales[0] << 15) - 0x7F8001) & 0xFFFF0000) + ((ScreenInfo->position.y + 160) << 16);
        EntityExplosion *explosion = CREATE_ENTITY(Explosion, NULL, x, y);

        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 4, &explosion->animator, true, 0);
        explosion->drawFX    = FX_SCALE;
        explosion->scale.x   = 0x80;
        explosion->scale.y   = 0x80;
        explosion->drawGroup = self->drawGroup + 1;

        if (self->jumpsRemain) {
            self->position.y = (ScreenInfo->position.y + ScreenInfo->size.y + 64) << 16;
            if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].x <= (ScreenInfo->position.x + ScreenInfo->center.x) << 16)
                UberCaterkiller_SetupBodySegments(0x18000, -0xB8000);
            else
                UberCaterkiller_SetupBodySegments(-0x18000, -0xB8000);

            for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) self->bodyScales[i] = 0x260;

            self->state = UberCaterkiller_State_RepeatedJumps;
            RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
            self->aniID ^= 1;
        }
        else {
            UberCaterkiller_PrepareMoveIntoBG();
        }

        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, self->aniID, &self->headAnimator, true, 0);
    }
}

void UberCaterkiller_State_RepeatedJumps(void)
{
    RSDK_THIS(UberCaterkiller);

    RSDK.ProcessAnimation(&self->headAnimator);

    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
        UberCaterkiller_HandleSegmentMoveFX(i);

        if (self->bodyTimers[i] > 0) {
            self->bodyTimers[i]--;
        }
        else {
            self->bodyVelocity[i].y += 0x3800;
            self->bodyPositions[i].y += self->bodyVelocity[i].y;

            if (self->bodyInAir[i]) {
                self->bodyPositions[i].x += self->bodyVelocity[i].x;

                if (self->aniID) {
                    if (self->bodyScales[i] > 0x180)
                        self->bodyScales[i] -= 3;
                }
                else {
                    if (self->bodyScales[i] < 0x280)
                        self->bodyScales[i] += 3;
                }
            }
        }
    }

    UberCaterkiller_CheckPlayerCollisions();

    if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].y > (ScreenInfo->size.y + ScreenInfo->position.y + 64) << 16) {
        if (!self->aniID)
            --self->jumpsRemain;

        if (self->jumpsRemain) {
            self->position.y = (ScreenInfo->size.y + ScreenInfo->position.y + 64) << 16;
            if (self->bodyPositions[UBERCATERKILLER_SEGMENT_COUNT - 1].x <= (ScreenInfo->position.x + ScreenInfo->center.x) << 16)
                UberCaterkiller_SetupBodySegments(0x18000, -0xB8000);
            else
                UberCaterkiller_SetupBodySegments(-0x18000, -0xB8000);

            self->aniID ^= 1;
            RSDK.PlaySfx(UberCaterkiller->sfxCaterJump, false, 255);
        }
        else {
            self->targetScale = 0x100;
            UberCaterkiller_PrepareMoveIntoBG();

            switch (RSDK.Rand(0, 3)) {
                default:
                case 0: // jump from BG
                    break;

                case 1: // jump from left
                    self->targetBodyPos = (ScreenInfo->position.x - 32) << 16;
                    break;

                case 2: // jump from right
                    self->targetBodyPos = (ScreenInfo->position.x + ScreenInfo->size.x + 32) << 16;
                    break;
            }
        }

        RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, self->aniID, &self->headAnimator, true, 0);
    }
}

void UberCaterkiller_State_Destroyed(void)
{
    RSDK_THIS(UberCaterkiller);

    UberCaterkiller_Explode();

    for (int32 i = 0; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) self->bodyPositions[i].x -= 0x10000;

    self->position.x -= 0x10000;

    if (--self->timer <= 0) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 32;

        RSDK.PlaySfx(UberCaterkiller->sfxExplosion3, false, 255);

        for (int32 i = 1; i < UBERCATERKILLER_SEGMENT_COUNT; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->bodyPositions[i].x, self->bodyPositions[i].y);
            RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, self->bodyAnimators[i]->animationID, &debris->animator, true,
                                    self->bodyAnimators[i]->frameID);
            debris->velocity.x      = 4 * RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = 4 * RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x4800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->drawFX |= FX_SCALE;
            debris->updateRange.x = 0x400000;
            debris->updateRange.y = 0x400000;
            debris->scale.x       = self->bodyScales[i];
            debris->scale.y       = self->bodyScales[i];
            debris->scaleSpeed.y  = RSDK.Rand(-4, 5);
            debris->scaleSpeed.x  = debris->scaleSpeed.y;

            self->bodyPositions[i].x = -0x800000;
            self->bodyPositions[i].y = -0x800000;
        }

        self->velocity.y = -0x40000;
        self->state      = UberCaterkiller_State_Finish;
    }
}

void UberCaterkiller_State_Finish(void)
{
    RSDK_THIS(UberCaterkiller);

    UberCaterkiller_Explode();

    self->position.y += self->velocity.y;
    self->bodyPositions[0].y = self->position.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        UberCaterkiller->defeated = true;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void UberCaterkiller_EditorDraw(void)
{
    RSDK_THIS(UberCaterkiller);

    RSDK.SetSpriteAnimation(UberCaterkiller->aniFrames, 0, &self->headAnimator, true, 0);

    RSDK.DrawSprite(&self->headAnimator, NULL, false);
}

void UberCaterkiller_EditorLoad(void) { UberCaterkiller->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Sandworm.bin", SCOPE_STAGE); }
#endif

void UberCaterkiller_Serialize(void) {}
