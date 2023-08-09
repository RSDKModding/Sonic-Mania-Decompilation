// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WalkerLegs Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWalkerLegs *WalkerLegs;

void WalkerLegs_Update(void)
{
    RSDK_THIS(WalkerLegs);

    StateMachine_Run(self->state);

    ++self->smokeSpawnTimer[0];
    ++self->smokeSpawnTimer[1];

    self->position.x = self->legPos[0].x;
    self->position.y = self->legPos[0].y;

    if (self->timer > 0)
        self->timer--;
}

void WalkerLegs_LateUpdate(void) {}

void WalkerLegs_StaticUpdate(void) {}

void WalkerLegs_Draw(void) { WalkerLegs_DrawSprites(); }

void WalkerLegs_Create(void *data)
{
    RSDK_THIS(WalkerLegs);

    self->active    = ACTIVE_BOUNDS;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->startPos  = self->position;

    self->legPos[0] = self->position;
    self->legPos[1] = self->position;
    self->legPos[1].x += (2 * (self->direction == FLIP_NONE) - 1) << 22;

    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    self->state = WalkerLegs_State_Init;
}

void WalkerLegs_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("LRZ2")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ2/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ2/Particles.bin", SCOPE_STAGE);
    }

    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 0, &WalkerLegs->legAnimator, true, 0);
    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 1, &WalkerLegs->linkAnimator, true, 0);

    WalkerLegs->hitbox.left   = -26;
    WalkerLegs->hitbox.right  = 27;
    WalkerLegs->hitbox.top    = -40;
    WalkerLegs->hitbox.bottom = -32;

    WalkerLegs->sfxWalkerLegs  = RSDK.GetSfx("LRZ/WalkerLegs.wav");
    WalkerLegs->sfxWalkerLegs2 = RSDK.GetSfx("LRZ/WalkerLegs2.wav");
}

void WalkerLegs_DrawSprites(void)
{
    RSDK_THIS(WalkerLegs);

    RSDK.DrawSprite(&WalkerLegs->legAnimator, &self->legPos[0], false);
    RSDK.DrawSprite(&WalkerLegs->legAnimator, &self->legPos[1], false);

    int32 moveX = (self->legPos[1].x - self->legPos[0].x) >> 3;
    int32 moveY = (self->legPos[1].y - self->legPos[0].y) >> 3;

    Vector2 linkPos = self->legPos[0];
    for (int32 i = 0; i < 9; ++i) {
        RSDK.DrawSprite(&WalkerLegs->linkAnimator, &linkPos, false);

        linkPos.x += moveX;
        linkPos.y += moveY;
    }
}

void WalkerLegs_CheckOffScreen(void)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckOnScreen(self, NULL)) {
        if (!RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
            self->position = self->startPos;
            self->visible  = false;
            self->state    = WalkerLegs_State_TryToReset;
        }
    }
}

void WalkerLegs_HandlePlayerMovement(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *legPos[2];
    Vector2 *collisionOffset[2];

    legPos[0] = &self->legPos[0];
    legPos[1] = &self->legPos[1];

    collisionOffset[0] = &self->legCollisionOffset[0];
    collisionOffset[1] = &self->legCollisionOffset[1];

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        for (int32 l = 0; l < 2; ++l) {
            self->position.x = legPos[l]->x;
            self->position.y = legPos[l]->y;

            if ((1 << playerID) & self->activePlayers[l]) {
                player->position.x += collisionOffset[l]->x;
                player->position.y += collisionOffset[l]->y;
            }

            if (Player_CheckCollisionPlatform(player, self, &WalkerLegs->hitbox) == C_TOP)
                self->activePlayers[l] |= 1 << playerID;
            else
                self->activePlayers[l] &= ~(1 << playerID);
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void WalkerLegs_CheckObjectCrush(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *legPos[2];

    legPos[0] = &self->legPos[0];
    legPos[1] = &self->legPos[1];

    WalkerLegs->hitbox.top    = -WalkerLegs->hitbox.top;
    WalkerLegs->hitbox.bottom = -WalkerLegs->hitbox.bottom;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        for (int32 l = 0; l < 2; ++l) {
            self->position.x = legPos[l]->x;
            self->position.y = legPos[l]->y;

            if (Player_CheckCollisionBox(player, self, &WalkerLegs->hitbox) == C_BOTTOM)
                player->collisionFlagV |= 2;
        }
    }

    if (Rexon) {
        foreach_active(Rexon, rexon)
        {
            if (!rexon->type) {
                for (int32 l = 0; l < 2; ++l) {
                    self->position.x = legPos[l]->x;
                    self->position.y = legPos[l]->y;

                    if (RSDK.CheckObjectCollisionTouchBox(rexon, &Rexon->hitboxShell, self, &WalkerLegs->hitbox)) {
                        Rexon_Destroy(rexon, true);

                        if (!WalkerLegs->hasAchievement) {
                            API_UnlockAchievement(&achievementList[ACH_LRZ]);
                            WalkerLegs->hasAchievement = true;
                        }
                    }
                }
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
        for (int32 l = 0; l < 2; ++l) {
            self->position.x = legPos[l]->x;
            self->position.y = legPos[l]->y;

            if (RSDK.CheckObjectCollisionTouchBox(self, &WalkerLegs->hitbox, spikes, &spikeHitbox)) {
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
                self->timer = 8;
            }
        }
    }

    WalkerLegs->hitbox.top    = -WalkerLegs->hitbox.top;
    WalkerLegs->hitbox.bottom = -WalkerLegs->hitbox.bottom;

    self->position.x = storeX;
    self->position.y = storeY;
}

void WalkerLegs_CheckStepTrigger(void)
{
    RSDK_THIS(WalkerLegs);

    if (!self->steps || self->stepCount < self->steps) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (!player->sidekick && ((1 << playerID) & self->activePlayers[self->activeLeg != 0])) {
                self->startAngle = self->angle;
                self->active     = ACTIVE_NORMAL;
                self->state      = WalkerLegs_State_Stepping;
            }
        }
    }
}

void WalkerLegs_CheckTileCollisions(void)
{
    RSDK_THIS(WalkerLegs);

    self->angleVel += 0x2000;
    self->angle += self->angleVel;

    int32 entityAngle = self->angle;
    int32 angle       = abs(self->angle - self->startAngle) >> 16;
    int32 x = 0, y = 0;
    if (self->activeLeg) {
        x = self->legPos[0].x;
        y = self->legPos[0].y;
    }
    else {
        entityAngle -= 0x1000000;
        x = self->legPos[1].x;
        y = self->legPos[1].y;
    }

    int32 rx = abs(self->legPos[1].x - self->legPos[0].x) >> 16;
    int32 ry = abs(self->legPos[1].y - self->legPos[0].y) >> 16;

    uint16 radius = MathHelpers_SquareRoot(rx * rx + ry * ry) - 1;
    if (radius <= 0x40)
        radius = 0x40;

    int32 ang = entityAngle & 0x1FFFFFF;

    int32 newX = radius * (RSDK.Cos512(ang >> 16) << 7);
    int32 newY = radius * (RSDK.Sin512(ang >> 16) << 7);
    if (self->direction == FLIP_X)
        newX = -newX;

    self->position.x = (newX + x) & 0xFFFF0000;
    self->position.y = (newY + y) & 0xFFFF0000;

    bool32 hitGround = false;

    self->legPos[self->activeLeg != 0].x = self->position.x;
    self->legPos[self->activeLeg != 0].y = self->position.y;
    if (angle > 128 && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x280000, true)) {
        self->legPos[self->activeLeg != 0].x    = self->position.x;
        self->legPos[self->activeLeg != 0].y    = self->position.y;
        self->smokeSpawnY[self->activeLeg != 0] = self->position.y + 0x280000;
        hitGround                               = true;
    }

    if (hitGround) {
        self->angleVel = 0;
        Camera_ShakeScreen(0, 0, 5);

        ++self->stepCount;
        self->state        = WalkerLegs_State_Idle;
        self->finishedStep = true;
        uint16 tile        = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        if (tile == (uint16)-1)
            tile = RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        uint8 tileFlags = RSDK.GetTileFlags(tile, self->collisionPlane);
        // whats up here? why is it lava & conveyor??
        if (tileFlags == LRZ2_TFLAGS_LAVA || tileFlags == LRZ2_TFLAGS_CONVEYOR_L) {
            RSDK.PlaySfx(WalkerLegs->sfxWalkerLegs2, false, 0xFF);
            WalkerLegs_CreateDebris(self->activeLeg == 1, true);
        }
        else {
            RSDK.PlaySfx(WalkerLegs->sfxWalkerLegs, false, 0xFF);
            WalkerLegs_CreateDebris(self->activeLeg == 1, false);
        }
        self->smokeSpawnTimer[self->activeLeg == 1] = 0;

        self->activeLeg = self->activeLeg == 0;
    }

    self->position.x = self->legPos[0].x;
    self->position.y = self->legPos[0].y;
}

void WalkerLegs_CheckStoodLava(void)
{
    RSDK_THIS(WalkerLegs);

    Vector2 *legPos[2];
    legPos[0] = &self->legPos[0];
    legPos[1] = &self->legPos[1];

    for (int32 l = 0; l < 2; ++l) {
        int32 x      = legPos[l]->x;
        int32 y      = legPos[l]->y;
        int32 otherY = legPos[1 - l]->y;

        self->position.x = x;
        self->position.y = y;
        uint16 tile      = RSDK.GetTile(Zone->fgLayer[0], self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        if (tile == (uint16)-1)
            tile = RSDK.GetTile(Zone->fgLayer[1], self->position.x >> 20, (self->position.y + 0x280000) >> 20);

        uint8 tileFlags = RSDK.GetTileFlags(tile, self->collisionPlane);
        // whats up here? why is it lava AND conveyor L only???
        if ((tileFlags == LRZ2_TFLAGS_LAVA || tileFlags == LRZ2_TFLAGS_CONVEYOR_L) && y - otherY < 0x500000) {
            self->finishedStep = true;
            y += 0x2800;
            WalkerLegs_CreateSmoke(l == 1);
        }

        legPos[l]->x = x;
        legPos[l]->y = y;
    }

    self->position.x = self->legPos[0].x;
    self->position.y = self->legPos[0].y;
}

void WalkerLegs_CreateDebris(bool32 isRightLeg, bool32 isMagma)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckSceneFolder("LRZ2") || isMagma) {
        int32 x = self->legPos[isRightLeg != false].x;
        int32 y = self->legPos[isRightLeg != false].y;

        int32 size  = (WalkerLegs->hitbox.right - WalkerLegs->hitbox.left) >> 1;
        int32 count = RSDK.Rand(4, 6);
        int32 move  = (size << 17) / (count - 1);
        int32 pos   = 0;

        for (int32 i = 0; i < count; ++i) {
            int32 spawnX         = (self->position.x - (size << 16)) + pos + ((RSDK.Rand(0, 12) - 6) << 15);
            int32 spawnY         = ((RSDK.Rand(0, 8) - 4) << 15) + (y + 0x280000);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, spawnX, spawnY);

            RSDK.SetSpriteAnimation(WalkerLegs->particleFrames, !isMagma, &debris->animator, true, 0);
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->gravityStrength = 0x3800;
            debris->velocity.x      = 0x180 * (abs(spawnX - x) >> 8) / size;
            if (debris->position.x < self->position.x) {
                debris->direction  = FLIP_X;
                debris->velocity.x = -debris->velocity.x;
            }

            debris->velocity.y = -0x1000 * RSDK.Rand(32, 54);
            if (isMagma)
                debris->velocity.y >>= 1;

            pos += move;
        }
    }
}

void WalkerLegs_CreateSmoke(bool32 isRightLeg)
{
    RSDK_THIS(WalkerLegs);

    if (self->smokeSpawnTimer[isRightLeg != false] >= 5) {
        if (!(self->smokeSpawnTimer[isRightLeg != false] % 5)) {
            int32 count = RSDK.Rand(1, 2);
            for (int32 i = 0; i < count; ++i) {
                int32 spawnX = self->legPos[isRightLeg != false].x + (RSDK.Rand(WalkerLegs->hitbox.left, WalkerLegs->hitbox.right) << 16);
                int32 spawnY = self->smokeSpawnY[isRightLeg != false];

                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, spawnX, spawnY);
                RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &debris->animator, true, 0);
                debris->velocity.x = 0;
                debris->velocity.y = -0x1000 * RSDK.Rand(0, 4);
                debris->drawGroup  = Zone->objectDrawGroup[1] - 1;
                debris->timer      = 30;
            }
        }
    }
}

void WalkerLegs_State_Init(void)
{
    RSDK_THIS(WalkerLegs);

    self->stepCount        = 0;
    self->angleVel         = 0;
    self->activeLeg        = 0;
    self->angle            = 0;
    self->finishedStep     = false;
    self->visible          = true;
    self->active           = ACTIVE_BOUNDS;
    self->activePlayers[0] = 0;
    self->activePlayers[1] = 0;

    self->position       = self->startPos;
    self->legPos[0]      = self->startPos;
    self->legPos[1]      = self->startPos;
    self->smokeSpawnY[0] = self->legPos[0].y;

    self->legCollisionOffset[0].x = 0;
    self->legCollisionOffset[0].y = 0;
    self->legCollisionOffset[1].x = 0;
    self->legCollisionOffset[1].y = 0;

    self->legPos[1].x += (2 * (self->direction == FLIP_NONE) - 1) << 22;
    self->smokeSpawnY[0] -= (WalkerLegs->hitbox.top << 16);
    self->smokeSpawnY[1] = self->legPos[1].y - (WalkerLegs->hitbox.top << 16);

    self->state = WalkerLegs_State_Idle;
}

void WalkerLegs_State_Idle(void)
{
    RSDK_THIS(WalkerLegs);

    self->angle &= 0x1FFFFFF;

    self->prevLegPos[0] = self->legPos[0];
    self->prevLegPos[1] = self->legPos[1];
    WalkerLegs_CheckStoodLava();

    self->legCollisionOffset[0].x = self->legPos[0].x - self->prevLegPos[0].x;
    self->legCollisionOffset[0].y = self->legPos[0].y - self->prevLegPos[0].y;
    self->legCollisionOffset[1].x = self->legPos[1].x - self->prevLegPos[1].x;
    self->legCollisionOffset[1].y = self->legPos[1].y - self->prevLegPos[1].y;
    WalkerLegs_HandlePlayerMovement();

    if (self->finishedStep) {
        int32 x            = self->direction ? (self->legPos[0].x - self->legPos[1].x) : (self->legPos[1].x - self->legPos[0].x);
        self->finishedStep = false;
        self->angle        = RSDK.ATan2(x, self->legPos[1].y - self->legPos[0].y) << 17;
    }

    WalkerLegs_CheckStepTrigger();
    WalkerLegs_CheckOffScreen();
}

void WalkerLegs_State_Stepping(void)
{
    RSDK_THIS(WalkerLegs);

    self->prevLegPos[0] = self->legPos[0];
    self->prevLegPos[1] = self->legPos[1];
    self->finishedStep  = false;

    WalkerLegs_CheckStoodLava();
    WalkerLegs_CheckTileCollisions();

    self->legCollisionOffset[0].x = self->legPos[0].x - self->prevLegPos[0].x;
    self->legCollisionOffset[0].y = self->legPos[0].y - self->prevLegPos[0].y;
    self->legCollisionOffset[1].x = self->legPos[1].x - self->prevLegPos[1].x;
    self->legCollisionOffset[1].y = self->legPos[1].y - self->prevLegPos[1].y;

    WalkerLegs_HandlePlayerMovement();
    WalkerLegs_CheckObjectCrush();
    WalkerLegs_CheckOffScreen();
}

void WalkerLegs_State_TryToReset(void)
{
    RSDK_THIS(WalkerLegs);

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        self->state = WalkerLegs_State_Init;
}

#if GAME_INCLUDE_EDITOR
void WalkerLegs_EditorDraw(void)
{
    RSDK_THIS(WalkerLegs);

    self->legPos[0] = self->position;
    self->legPos[1] = self->position;
    self->legPos[1].x += (2 * (self->direction == FLIP_NONE) - 1) << 22;

    self->inkEffect = INK_NONE;
    WalkerLegs_DrawSprites();

    // Draw Distance
    if (showGizmos()) {
        int32 dist = (0x40 * self->steps) << 16;

        self->inkEffect = INK_BLEND;
        self->legPos[0].x += self->direction ? -dist : dist;
        self->legPos[1].x += self->direction ? -dist : dist;
        WalkerLegs_DrawSprites();

        RSDK.DrawLine(self->position.x, self->position.y, self->legPos[0].x, self->legPos[0].y, 0x00FF00, 0xFF, INK_NONE, false);
    }
}

void WalkerLegs_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ1/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ1/Particles.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("LRZ2")) {
        WalkerLegs->aniFrames      = RSDK.LoadSpriteAnimation("LRZ2/WalkerLegs.bin", SCOPE_STAGE);
        WalkerLegs->particleFrames = RSDK.LoadSpriteAnimation("LRZ2/Particles.bin", SCOPE_STAGE);
    }

    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 0, &WalkerLegs->legAnimator, true, 0);
    RSDK.SetSpriteAnimation(WalkerLegs->aniFrames, 1, &WalkerLegs->linkAnimator, true, 0);

    RSDK_ACTIVE_VAR(WalkerLegs, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void WalkerLegs_Serialize(void)
{
    RSDK_EDITABLE_VAR(WalkerLegs, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(WalkerLegs, VAR_UINT16, steps);
}
