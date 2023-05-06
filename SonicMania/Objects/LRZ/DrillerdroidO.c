// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: DrillerdroidO Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectDrillerdroidO *DrillerdroidO;

void DrillerdroidO_Update(void)
{
    RSDK_THIS(DrillerdroidO);

    StateMachine_Run(self->state);
}

void DrillerdroidO_LateUpdate(void) {}

void DrillerdroidO_StaticUpdate(void)
{
    if (DrillerdroidO->drillSfxTimer) {
        if (!DrillerdroidO->playingDrillSfx) {
            RSDK.PlaySfx(DrillerdroidO->sfxDrill, 43643, 0xFF);
            DrillerdroidO->playingDrillSfx = true;
        }

        DrillerdroidO->drillSfxTimer = 0;
    }
    else if (DrillerdroidO->playingDrillSfx) {
        RSDK.StopSfx(DrillerdroidO->sfxDrill);
        DrillerdroidO->playingDrillSfx = false;
    }
}

void DrillerdroidO_Draw(void)
{
    RSDK_THIS(DrillerdroidO);

    StateMachine_Run(self->stateDraw);
}

void DrillerdroidO_Create(void *data)
{
    RSDK_THIS(DrillerdroidO);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->visible = true;

            if (data)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case DRILLERDROIDO_MAIN:
                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 5, &self->armorAnimator, true, 0);

                    self->drawGroup = Zone->objectDrawGroup[1];

                    DrillerdroidO->boss           = self;
                    DrillerdroidO->childSlotStart = SceneInfo->entitySlot + 2;
                    DrillerdroidO->currentLevel   = 4;

                    self->state     = DrillerdroidO_State_SetupArena;
                    self->stateDraw = DrillerdroidO_Draw_Boss;
                    break;

                case DRILLERDROIDO_FIREBALLEMITTER:
                    self->active        = ACTIVE_BOUNDS;
                    self->drawFX        = FX_FLIP;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    RSDK.SetSpriteAnimation(DrillerdroidO->ticFrames, 0, &self->mainAnimator, true, 0);

                    self->drawGroup = Zone->objectDrawGroup[1];
                    self->state     = DrillerdroidO_State_FireballEmitter;
                    self->stateDraw = DrillerdroidO_Draw_FireballEmitter;
                    break;

                case DRILLERDROIDO_TARGET:
                case DRILLERDROIDO_UNUSED1:
                case DRILLERDROIDO_UNUSED2:
                    self->active        = ACTIVE_NORMAL;
                    self->drawFX        = FX_FLIP;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 2, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 3, &self->armorAnimator, true, 0);

                    self->drawGroup = Zone->objectDrawGroup[1];
                    self->state     = DrillerdroidO_State_Target;
                    self->stateDraw = DrillerdroidO_Draw_Target;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void DrillerdroidO_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        DrillerdroidO->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);

    DrillerdroidO->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    DrillerdroidO->hitboxCore.left   = -25;
    DrillerdroidO->hitboxCore.top    = -16;
    DrillerdroidO->hitboxCore.right  = 25;
    DrillerdroidO->hitboxCore.bottom = 16;

    DrillerdroidO->hitboxPistonL.left   = -58;
    DrillerdroidO->hitboxPistonL.top    = -74;
    DrillerdroidO->hitboxPistonL.right  = -25;
    DrillerdroidO->hitboxPistonL.bottom = 16;

    DrillerdroidO->hitboxPistonR.left   = 25;
    DrillerdroidO->hitboxPistonR.top    = -74;
    DrillerdroidO->hitboxPistonR.right  = 58;
    DrillerdroidO->hitboxPistonR.bottom = 16;

    DrillerdroidO->hitboxDrillL.left   = -58;
    DrillerdroidO->hitboxDrillL.top    = 16;
    DrillerdroidO->hitboxDrillL.right  = -25;
    DrillerdroidO->hitboxDrillL.bottom = 53;

    DrillerdroidO->hitboxDrillR.left   = 25;
    DrillerdroidO->hitboxDrillR.top    = 16;
    DrillerdroidO->hitboxDrillR.right  = 58;
    DrillerdroidO->hitboxDrillR.bottom = 53;

    DrillerdroidO->hitboxLandPosSegmentBreak.left   = -6;
    DrillerdroidO->hitboxLandPosSegmentBreak.top    = 16;
    DrillerdroidO->hitboxLandPosSegmentBreak.right  = 6;
    DrillerdroidO->hitboxLandPosSegmentBreak.bottom = 16;

    DrillerdroidO->hitboxLandPosR.left   = -58;
    DrillerdroidO->hitboxLandPosR.top    = 37;
    DrillerdroidO->hitboxLandPosR.right  = -42;
    DrillerdroidO->hitboxLandPosR.bottom = 53;

    DrillerdroidO->hitboxLandPosL.left   = 42;
    DrillerdroidO->hitboxLandPosL.top    = 37;
    DrillerdroidO->hitboxLandPosL.right  = 58;
    DrillerdroidO->hitboxLandPosL.bottom = 53;

    DrillerdroidO->hitboxEmitterRange.left   = -32;
    DrillerdroidO->hitboxEmitterRange.top    = -192;
    DrillerdroidO->hitboxEmitterRange.right  = 32;
    DrillerdroidO->hitboxEmitterRange.bottom = 192;

    DrillerdroidO->drillDelay[0]  = 4;
    DrillerdroidO->pistonDelay[0] = 4;

    DrillerdroidO->drillDelay[1]  = 0;
    DrillerdroidO->pistonDelay[1] = 0;

    DrillerdroidO->drillPos[0] = 0x100000;
    DrillerdroidO->drillPos[1] = 0x100000;

    DrillerdroidO->pistonPos[0] = 0;
    DrillerdroidO->pistonPos[1] = 0;

    DrillerdroidO->drillMoveDir[0] = FLIP_NONE;
    DrillerdroidO->drillMoveDir[1] = FLIP_NONE;

    DrillerdroidO->pistonMoveDir[0] = FLIP_NONE;
    DrillerdroidO->pistonMoveDir[1] = FLIP_NONE;

    DrillerdroidO->active = ACTIVE_ALWAYS;

    DrillerdroidO->drillSfxTimer   = 0;
    DrillerdroidO->playingDrillSfx = false;

    DrillerdroidO->emitFireballs = false;

    DrillerdroidO->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    DrillerdroidO->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    DrillerdroidO->sfxDrill     = RSDK.GetSfx("LRZ/Drill.wav");
    DrillerdroidO->sfxImpact    = RSDK.GetSfx("Stage/Impact4.wav");
    DrillerdroidO->sfxJump      = RSDK.GetSfx("LRZ/DrillJump.wav");
    DrillerdroidO->sfxTargeting = RSDK.GetSfx("Stage/Targeting1.wav");
    DrillerdroidO->sfxSizzle    = RSDK.GetSfx("LRZ/Sizzle.wav");
}

void DrillerdroidO_CheckPlayerCollisions(void)
{
    RSDK_THIS(DrillerdroidO);

    if (self->invincibilityTimer)
        self->invincibilityTimer--;

    foreach_active(Player, player)
    {
        int32 playerX = player->position.y;
        int32 playerY = player->position.x;
        int32 velX    = player->velocity.x;
        int32 velY    = player->velocity.y;
        int32 side    = Player_CheckCollisionBox(player, self, &DrillerdroidO->hitboxCore);

        switch (side) {
            default: break;

            case C_TOP:
                Player_Hurt(player, self);

                if (self->velocity.y <= 0)
                    player->collisionFlagV |= 1;
                break;

            case C_BOTTOM:
                if (self->rotation) {
                    if (!self->invincibilityTimer) {
                        player->velocity.x = velX;
                        player->velocity.y = velY;
                        player->position.x = playerY;
                        player->position.y = playerX;

                        if (Player_CheckBossHit(player, self)) {
                            if (!--self->health) {
                                SceneInfo->timeEnabled = false;
                                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                                self->invincibilityTimer     = 60;
                                DrillerdroidO->emitFireballs = false;
                                self->state                  = DrillerdroidO_State_Destroyed;
                            }
                            else {
                                RSDK.PlaySfx(DrillerdroidO->sfxHit, false, 255);
                                self->invincibilityTimer = 30;
                            }
                        }
                    }
                }
                else if (self->velocity.y >= 0) {
                    player->collisionFlagV |= 2;
                }
                break;
        }

        Player_CheckCollisionBox(player, self, &DrillerdroidO->hitboxPistonL);
        Player_CheckCollisionBox(player, self, &DrillerdroidO->hitboxPistonR);

        if (self->state == DrillerdroidO_State_Drilling) {
            if (Player_CheckCollisionTouch(player, self, &DrillerdroidO->hitboxDrillL)
                || Player_CheckCollisionTouch(player, self, &DrillerdroidO->hitboxDrillR)) {
                Player_Hurt(player, self);
            }
        }
        else {
            if (self->state == DrillerdroidO_State_Dropping
                && (Player_CheckCollisionBox(player, self, &DrillerdroidO->hitboxDrillL) == C_BOTTOM
                    || Player_CheckCollisionBox(player, self, &DrillerdroidO->hitboxDrillR) == C_BOTTOM)) {
                if (self->velocity.y >= 0)
                    player->collisionFlagV |= 2;
                else
                    Player_Hurt(player, self);
            }
        }
    }
}

void DrillerdroidO_Explode(void)
{
    RSDK_THIS(DrillerdroidO);

    if (!(Zone->timer & 3)) {
        RSDK.PlaySfx(Drillerdroid->sfxExplosion, false, 255);

        if (!(Zone->timer & 3)) {
            int32 x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y = self->position.y + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void DrillerdroidO_SpawnDebris(int32 offset)
{
    RSDK_THIS(DrillerdroidO);

    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x      = RSDK.Rand(0, 6) << 15;
    debris->velocity.y      = RSDK.Rand(-12, -8) << 15;
    debris->gravityStrength = 0x3800;
    debris->drawGroup       = Zone->objectDrawGroup[0];
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;

    debris = CREATE_ENTITY(Debris, Debris_State_Fall, offset + self->position.x, self->position.y + 0x400000);
    RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 1, &debris->animator, true, RSDK.Rand(0, 8));
    debris->velocity.x      = RSDK.Rand(-6, 0) << 15;
    debris->velocity.y      = RSDK.Rand(-12, -8) << 15;
    debris->gravityStrength = 0x3800;
    debris->drawGroup       = Zone->objectDrawGroup[0];
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;
}

void DrillerdroidO_State_SetupArena(void)
{
    RSDK_THIS(DrillerdroidO);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 96;
        Zone->cameraBoundsT[0]      = Zone->cameraBoundsB[0] - SCREEN_YSIZE;

        self->startY     = self->position.y;
        self->active     = ACTIVE_NORMAL;
        self->position.y = (ScreenInfo->position.y - 192) << 16;
        self->visible    = true;
        self->state      = DrillerdroidO_State_AwaitPlayer;
    }
}

void DrillerdroidO_State_AwaitPlayer(void)
{
    RSDK_THIS(DrillerdroidO);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;

        Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
        self->health = 6;

        CREATE_ENTITY(DrillerdroidO, INT_TO_VOID(DRILLERDROIDO_TARGET), self->position.x, self->startY);
        self->position.x = 0;

        RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
        self->state                    = DrillerdroidO_State_DecidingDropPos;
        DrillerdroidO->canBreakSegment = true;
        DrillerdroidO->emitFireballs   = true;
    }
}

void DrillerdroidO_State_Dropping(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    DrillerdroidO_CheckPlayerCollisions();

    if (self->position.y >= (Zone->cameraBoundsB[0] - 112) << 16) {
        self->position.y = (Zone->cameraBoundsB[0] - 112) << 16;
        RSDK.PlaySfx(DrillerdroidO->sfxImpact, false, 255);

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;

        self->velocity.y >>= 1;
        self->startY = self->position.y + 0x100000;

        if (!DrillerdroidO->canBreakSegment) {
            for (int32 i = 0; i < 4; ++i) {
                DrillerdroidO_SpawnDebris(-0x300000);
                DrillerdroidO_SpawnDebris(0x300000);
                DrillerdroidO_SpawnDebris(-0x190000);
                DrillerdroidO_SpawnDebris(0x190000);
            }
        }

        EntityLRZRockPile *pile = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, LRZRockPile);
        pile->timer             = 1;
        pile->canCollapse       = true;
        self->timer             = 60;
        self->angle             = 0;
        self->state             = DrillerdroidO_State_Landed;
    }
}

void DrillerdroidO_State_Landed(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0xE000;

    DrillerdroidO->drillPos[0] = self->startY - self->position.y;
    DrillerdroidO->drillPos[1] = self->startY - self->position.y;

    DrillerdroidO_CheckPlayerCollisions();

    if (self->velocity.y < 0) {
        if (self->position.y < self->startY)
            self->state = DrillerdroidO_State_LandRecoil;
    }
}

void DrillerdroidO_State_LandRecoil(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x14000;

    DrillerdroidO->drillPos[0] = self->startY - self->position.y;
    DrillerdroidO->drillPos[1] = self->startY - self->position.y;

    DrillerdroidO_CheckPlayerCollisions();

    if (self->velocity.y > 0) {
        if (self->position.y > self->startY) {
            self->position.y = self->startY;

            DrillerdroidO->drillPos[0] = 0;
            DrillerdroidO->drillPos[1] = 0;

            self->state = DrillerdroidO_State_BeginDrilling;
        }
    }
}

void DrillerdroidO_State_BeginDrilling(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        if (DrillerdroidO->canBreakSegment) {
            EntityLavaGeyser *geyser = CREATE_ENTITY(LavaGeyser, NULL, self->position.x, self->position.y + 0x580000);
            geyser->force            = 56 << 12;
            geyser->duration         = 120;
            geyser->drawGroup        = Zone->objectDrawGroup[1];
            geyser->state            = LavaGeyser_HandleSetup;
        }

        self->timer = 240;
        self->state = DrillerdroidO_State_Drilling;
    }
}

void DrillerdroidO_State_Drilling(void)
{
    RSDK_THIS(DrillerdroidO);

    ++DrillerdroidO->drillSfxTimer;
    RSDK.ProcessAnimation(&self->mainAnimator);

    for (int32 i = 0; i < 2; ++i) {
        if (DrillerdroidO->pistonDelay[i]) {
            DrillerdroidO->pistonDelay[i]--;
        }
        else if (DrillerdroidO->pistonMoveDir[i]) {
            DrillerdroidO->pistonPos[i] -= 0x20000;
            if (!DrillerdroidO->pistonPos[i]) {
                DrillerdroidO->pistonMoveDir[i] = FLIP_NONE;
                DrillerdroidO->pistonDelay[i]   = 10;
            }
        }
        else {
            DrillerdroidO->pistonPos[i] += 0x40000;
            if (DrillerdroidO->pistonPos[i] == 0x100000)
                DrillerdroidO->pistonMoveDir[i] = FLIP_X;
        }

        if (DrillerdroidO->drillDelay[i]) {
            DrillerdroidO->drillDelay[i]--;
        }
        else if (DrillerdroidO->drillMoveDir[i]) {
            DrillerdroidO->drillPos[i] -= 0x40000;
            if (!DrillerdroidO->drillPos[i]) {
                DrillerdroidO->drillMoveDir[i] = FLIP_NONE;
                DrillerdroidO->drillDelay[i]   = 5;
            }
        }
        else {
            DrillerdroidO->drillPos[i] += 0x80000;
            if (DrillerdroidO->drillPos[i] == 0x100000)
                DrillerdroidO->drillMoveDir[i] = FLIP_X;
        }
    }

    if (!DrillerdroidO->canBreakSegment) {
        if (DrillerdroidO->drillPos[0] == 0x80000) {
            DrillerdroidO_SpawnDebris(-0x300000);
            DrillerdroidO_SpawnDebris(0x300000);
        }
        else if (DrillerdroidO->drillPos[1] == 0x80000) {
            DrillerdroidO_SpawnDebris(-0x190000);
            DrillerdroidO_SpawnDebris(0x190000);
        }
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    if (!camera->shakePos.y)
        camera->shakePos.y = 4;

    self->position.y ^= 0x10000;
    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->canBreakSegment && self->timer < 180) {
        if (self->alpha < 0x200)
            self->alpha += 4;
    }

    if (--self->timer <= 0)
        self->state = DrillerdroidO_State_FinishDrilling;
}

void DrillerdroidO_State_FinishDrilling(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO->pistonPos[0] -= 0x20000;
    if (DrillerdroidO->pistonPos[0] < 0)
        DrillerdroidO->pistonPos[0] = 0;

    DrillerdroidO->pistonPos[1] -= 0x20000;
    if (DrillerdroidO->pistonPos[1] < 0)
        DrillerdroidO->pistonPos[1] = 0;

    DrillerdroidO->drillPos[0] -= 0x20000;
    if (DrillerdroidO->drillPos[0] < 0)
        DrillerdroidO->drillPos[0] = 0;

    DrillerdroidO->drillPos[1] -= 0x20000;
    if (DrillerdroidO->drillPos[1] < 0)
        DrillerdroidO->drillPos[1] = 0;

    if (!DrillerdroidO->pistonPos[0] && !DrillerdroidO->pistonPos[1] && !DrillerdroidO->drillPos[0] && !DrillerdroidO->drillPos[1]) {
        self->timer = 60;
        self->state = DrillerdroidO_State_Overheat;
    }
}

void DrillerdroidO_State_Overheat(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--self->timer <= 0) {
        if (DrillerdroidO->canBreakSegment) {
            DrillerdroidO->canBreakSegment = false;
            RSDK.PlaySfx(DrillerdroidO->sfxSizzle, false, 255);
            self->state = DrillerdroidO_State_OverheatRecoil_DestroyedSegment;
        }
        else {
            self->state = DrillerdroidO_State_OverheatRecoil;
        }
    }

    DrillerdroidO_CheckPlayerCollisions();
}

void DrillerdroidO_State_OverheatRecoil(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO->drillPos[0] += 0x10000;
    DrillerdroidO->drillPos[1] += 0x10000;

    self->position.y -= 0x10000;

    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->drillPos[0] == 0x100000) {
        self->timer = 30;
        self->state = DrillerdroidO_State_JumpTargetDelay;
    }
}

void DrillerdroidO_State_JumpTargetDelay(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();

    if (--self->timer <= 0) {
        self->velocity.y = 0x78000;
        RSDK.PlaySfx(DrillerdroidO->sfxJump, false, 0xFF);
        self->state = DrillerdroidO_State_PrepareJumpTarget;
    }
}

void DrillerdroidO_State_PrepareJumpTarget(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1C000;

    DrillerdroidO->drillPos[0] = self->startY - self->position.y;
    DrillerdroidO->drillPos[1] = self->startY - self->position.y;

    if (self->velocity.y < 0) {
        if (DrillerdroidO->drillPos[0] >= 0x100000) {
            DrillerdroidO->drillPos[0] = 0x100000;
            DrillerdroidO->drillPos[1] = 0x100000;

            self->state = DrillerdroidO_State_JumpTargeting;
        }
    }
}

void DrillerdroidO_State_JumpTargeting(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y >= 0) {
        DrillerdroidO->drillDelay[0]  = 4;
        DrillerdroidO->pistonDelay[0] = 4;

        DrillerdroidO->drillDelay[1]  = 0;
        DrillerdroidO->pistonDelay[1] = 0;

        DrillerdroidO->drillPos[0] = 0x100000;
        DrillerdroidO->drillPos[1] = 0x100000;

        DrillerdroidO->pistonPos[0] = 0;
        DrillerdroidO->pistonPos[1] = 0;

        DrillerdroidO->pistonMoveDir[0] = FLIP_NONE;
        DrillerdroidO->pistonMoveDir[1] = FLIP_NONE;

        DrillerdroidO->drillMoveDir[0] = FLIP_NONE;
        DrillerdroidO->drillMoveDir[1] = FLIP_NONE;

        self->position.x = 0;

        if (self->health <= DrillerdroidO->currentLevel && DrillerdroidO->currentLevel) {
            self->timer                  = 90;
            DrillerdroidO->emitFireballs = false;
            self->state                  = DrillerdroidO_State_DestroyRockPiles;
        }
        else {
            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            CREATE_ENTITY(DrillerdroidO, INT_TO_VOID(DRILLERDROIDO_TARGET), player1->position.x, player1->position.y)->target = player1;

            RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
            self->state = DrillerdroidO_State_DecidingDropPos;
        }
    }
}

void DrillerdroidO_State_DestroyRockPiles(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--self->timer <= 60) {
        ++DrillerdroidO->drillSfxTimer;

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        if (!camera->shakePos.y)
            camera->shakePos.y = 4;
    }

    if (self->timer <= 0) {
        DrillerdroidO->currentLevel -= 2;

        for (int32 i = 0; i < 4; ++i) {
            EntityLRZRockPile *pile = RSDK_GET_ENTITY(DrillerdroidO->childSlotStart++, LRZRockPile);
            pile->timer             = 1;
            pile->canCollapse       = true;
        }

        self->timer = 30;
        self->state = DrillerdroidO_State_NextLevelDelay;
    }
}

void DrillerdroidO_State_NextLevelDelay(void)
{
    RSDK_THIS(DrillerdroidO);

    if (--self->timer <= 0) {
        self->position.y += 0x1000000;
        Zone->cameraBoundsB[0] += 256;
        self->state = DrillerdroidO_State_MoveToNextLevel;
    }
}

void DrillerdroidO_State_MoveToNextLevel(void)
{
    RSDK_THIS(DrillerdroidO);

    Zone->cameraBoundsT[0] = ScreenInfo->position.y;

    if (Zone->cameraBoundsT[0] == Zone->cameraBoundsB[0] - SCREEN_YSIZE) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(DrillerdroidO, INT_TO_VOID(DRILLERDROIDO_TARGET), player1->position.x, player1->position.y)->target = player1;

        RSDK.PlaySfx(DrillerdroidO->sfxTargeting, false, 255);
        DrillerdroidO->emitFireballs = true;
        self->state                  = DrillerdroidO_State_DecidingDropPos;
    }
}

void DrillerdroidO_State_DecidingDropPos(void)
{
    RSDK_THIS(DrillerdroidO);

    if (self->position.x) {
        self->timer = 240;
        self->state = DrillerdroidO_State_Dropping;

        foreach_active(DrillerdroidO, fireballEmitter)
        {
            if (fireballEmitter->type == DRILLERDROIDO_FIREBALLEMITTER && self->position.y < fireballEmitter->position.y) {
                if (RSDK.CheckObjectCollisionTouchBox(fireballEmitter, &DrillerdroidO->hitboxEmitterRange, self,
                                                      &DrillerdroidO->hitboxLandPosSegmentBreak)) {
                    self->position.x               = fireballEmitter->position.x;
                    DrillerdroidO->canBreakSegment = true;
                    foreach_break;
                }
            }
        }

        foreach_active(DrillerdroidO, emitter)
        {
            if (emitter->type == DRILLERDROIDO_FIREBALLEMITTER && self->position.y < emitter->position.y) {
                if (RSDK.CheckObjectCollisionTouchBox(emitter, &DrillerdroidO->hitboxEmitterRange, self, &DrillerdroidO->hitboxLandPosR)) {
                    self->position.x = emitter->position.x + 0x550000;
                    foreach_break;
                }

                if (RSDK.CheckObjectCollisionTouchBox(emitter, &DrillerdroidO->hitboxEmitterRange, self, &DrillerdroidO->hitboxLandPosL)) {
                    self->position.x = emitter->position.x - 0x550000;
                    foreach_break;
                }
            }
        }
    }
}

void DrillerdroidO_State_OverheatRecoil_DestroyedSegment(void)
{
    RSDK_THIS(DrillerdroidO);

    ++self->rotation;

    DrillerdroidO->drillPos[0] += 0x4000;
    DrillerdroidO->drillPos[1] += 0x4000;

    DrillerdroidO->pistonPos[0] += 0x4000;
    DrillerdroidO->pistonPos[1] += 0x4000;

    self->position.y -= 0x4000;

    DrillerdroidO_CheckPlayerCollisions();

    if (DrillerdroidO->drillPos[0] == 0x100000) {
        self->timer = 90;
        self->state = DrillerdroidO_State_Cooldown;
    }
}

void DrillerdroidO_State_Cooldown(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_CheckPlayerCollisions();

    self->alpha -= 6;
    if (self->alpha < 0)
        self->alpha = 0;

    if (--self->timer <= 0)
        self->state = DrillerdroidO_State_ResetFromCooldown;
}

void DrillerdroidO_State_ResetFromCooldown(void)
{
    RSDK_THIS(DrillerdroidO);

    self->rotation -= 4;

    DrillerdroidO->pistonPos[0] -= 0x10000;
    DrillerdroidO->pistonPos[1] -= 0x10000;

    DrillerdroidO_CheckPlayerCollisions();

    if (!DrillerdroidO->pistonPos[0]) {
        self->timer = 60;
        self->state = DrillerdroidO_State_JumpTargetDelay;
    }
}

void DrillerdroidO_State_Destroyed(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO_Explode();

    if (!--self->invincibilityTimer) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(-8, -4) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(-8, -4) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x - 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(-8, -4) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x300000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 1);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(-8, -4) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x + 0x230000, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 2);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(0, 2) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 3);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(-8, -4) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &debris->animator, true, 5);
        debris->velocity.x      = RSDK.Rand(-4, 4) << 15;
        debris->velocity.y      = RSDK.Rand(-8, -4) << 15;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        self->velocity.x           = RSDK.Rand(-4, 4) << 15;
        self->velocity.y           = (RSDK.Rand(-8, -4) << 15);
        self->mainAnimator.frameID = 4;
        self->stateDraw            = DrillerdroidO_Draw_Simple;
        self->state                = DrillerdroidO_State_Finish;
    }
}

void DrillerdroidO_State_Finish(void)
{
    RSDK_THIS(DrillerdroidO);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    DrillerdroidO_Explode();

    if (!RSDK.CheckOnScreen(self, NULL)) {
        Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
        destroyEntity(self);
    }
}

void DrillerdroidO_Draw_Boss(void)
{
    RSDK_THIS(DrillerdroidO);

    RSDK.SetLimitedFade(0, 1, 2, self->alpha, 32, 41);

    if (self->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 32, 0xE0E0E0);
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    }

    // Piston (L1)
    self->mainAnimator.frameID = 1;
    Vector2 drawPos;
    drawPos.x = self->position.x - 0x300000;
    drawPos.y = self->position.y - DrillerdroidO->pistonPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (L1)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + DrillerdroidO->drillPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Piston (L2)
    self->mainAnimator.frameID = 1;
    drawPos.x += 0xD0000;
    drawPos.y = self->position.y - DrillerdroidO->pistonPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (L2)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + DrillerdroidO->drillPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Piston (R1)
    self->mainAnimator.frameID = 1;
    drawPos.x += 0x530000;
    drawPos.y = self->position.y - DrillerdroidO->pistonPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (R1)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + DrillerdroidO->drillPos[0];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Piston (R2)
    self->mainAnimator.frameID = 1;
    drawPos.x -= 0xD0000;
    drawPos.y = self->position.y - DrillerdroidO->pistonPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Drill (R2)
    self->mainAnimator.frameID = 2;
    drawPos.y                  = self->position.y + DrillerdroidO->drillPos[1];
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Main Body
    self->mainAnimator.frameID = 0;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->drawFX |= FX_ROTATE;

    // Core Armor (L)
    drawPos.x                   = self->position.x + 0x10000;
    drawPos.y                   = self->position.y - 0x140000;
    self->armorAnimator.frameID = 0;
    RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

    // Core Armor (R)
    self->armorAnimator.frameID = 1;
    self->rotation              = 0x200 - self->rotation;
    RSDK.DrawSprite(&self->armorAnimator, &drawPos, false);

    self->rotation = 0x200 - self->rotation;
    self->drawFX   = FX_NONE;

    RSDK.CopyPalette(1, 32, 0, 32, 10);

    RSDK.SetPaletteEntry(0, 32, 0x282028);
    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void DrillerdroidO_Draw_Simple(void)
{
    RSDK_THIS(DrillerdroidO);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void DrillerdroidO_State_Target(void)
{
    RSDK_THIS(DrillerdroidO);

    RSDK.ProcessAnimation(&self->armorAnimator);
    RSDK.ProcessAnimation(&self->targetLockAnimator);

    if (self->target) {
        self->position.x = self->target->position.x;
        self->position.y = self->target->position.y;
    }

    self->alpha += 0x20;

    self->targetEdgeOffset.x -= 0x20000;
    if (self->targetEdgeOffset.x <= 0xC0000) {
        self->alpha              = 0;
        self->targetEdgeOffset.x = 0x2C0000;
    }

    self->targetEdgeOffset.y = self->targetEdgeOffset.x;
    if (++self->timer == 60)
        RSDK.SetSpriteAnimation(Drillerdroid->aniFrames, 4, &self->targetLockAnimator, true, 0);

    if (self->timer == 96) {
        self->target                    = NULL;
        DrillerdroidO->boss->position.x = self->position.x;

        if (self->position.x < (Zone->cameraBoundsL[0] + 64) << 16 || self->position.x > (Zone->cameraBoundsR[0] - 64) << 16)
            DrillerdroidO->boss->position.x = (Zone->cameraBoundsL[0] + 64) << 16;
        else if (self->position.x > (Zone->cameraBoundsR[0] - 64) << 16)
            DrillerdroidO->boss->position.x = (Zone->cameraBoundsR[0] - 64) << 16;
    }

    if (self->timer == 128)
        destroyEntity(self);
}

void DrillerdroidO_Draw_Target(void)
{
    RSDK_THIS(DrillerdroidO);

    // Top-Left Edge
    self->mainAnimator.frameID = 0;
    self->inkEffect            = INK_ALPHA;
    self->direction            = FLIP_NONE;
    Vector2 drawPos;
    drawPos.x = self->position.x - self->targetEdgeOffset.x;
    drawPos.y = self->position.y - self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Top-Right Edge
    self->direction = FLIP_X;
    drawPos.x       = self->position.x + self->targetEdgeOffset.x;
    drawPos.y       = self->position.y - self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Bottom-Left Edge
    self->mainAnimator.frameID = 1;
    self->direction            = FLIP_NONE;
    drawPos.x                  = self->position.x - self->targetEdgeOffset.x;
    drawPos.y                  = self->position.y + self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Bottom-Right Edge
    self->direction = FLIP_X;
    drawPos.x       = self->position.x + self->targetEdgeOffset.x;
    drawPos.y       = self->position.y + self->targetEdgeOffset.y;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    // Target Reticle
    self->inkEffect = INK_NONE;
    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->armorAnimator, NULL, false);
    RSDK.DrawSprite(&self->targetLockAnimator, NULL, false);
}

void DrillerdroidO_State_FireballEmitter(void)
{
    RSDK_THIS(DrillerdroidO);

    bool32 collided = true;
    foreach_active(DrillerdroidO, drillerdroid)
    {
        if (drillerdroid->type == DRILLERDROIDO_MAIN
            && RSDK.CheckObjectCollisionTouchBox(drillerdroid, &DrillerdroidO->hitboxLandPosSegmentBreak, self, &DrillerdroidO->hitboxEmitterRange)) {
            collided = false;
            foreach_break;
        }
    }

    if (collided && DrillerdroidO->emitFireballs) {
        if (!(Zone->timer & 0x3F)) {
            EntityLRZFireball *fireball =
                CREATE_ENTITY(LRZFireball, LRZFireball_StateFireball_LauncherGravity, self->position.x, self->position.y + 0x940000);
            fireball->angle      = 0xEE;
            fireball->rotation   = 0xEE;
            fireball->groundVel  = -0x300;
            fireball->velocity.x = fireball->groundVel * RSDK.Sin512(0x12);
            fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);

            fireball           = CREATE_ENTITY(LRZFireball, LRZFireball_StateFireball_LauncherGravity, self->position.x, self->position.y + 0x940000);
            fireball->angle    = 0x116;
            fireball->rotation = 0x116;
            fireball->groundVel  = -0x300;
            fireball->velocity.x = fireball->groundVel * RSDK.Sin512(-0x16);
            fireball->velocity.y = fireball->groundVel * RSDK.Cos512(0x100 - fireball->angle);
            RSDK.PlaySfx(LRZFireball->sfxFireball, false, 0xFF);
        }
    }
}

void DrillerdroidO_Draw_FireballEmitter(void)
{ /* Yeah :) */
}

#if GAME_INCLUDE_EDITOR
void DrillerdroidO_EditorDraw(void)
{
    RSDK_THIS(DrillerdroidO);

    DrillerdroidO->drillDelay[0]  = 4;
    DrillerdroidO->pistonDelay[0] = 4;

    DrillerdroidO->drillDelay[1]  = 0;
    DrillerdroidO->pistonDelay[1] = 0;

    DrillerdroidO->drillPos[0] = 0x100000;
    DrillerdroidO->drillPos[1] = 0x100000;

    DrillerdroidO->pistonPos[0] = 0;
    DrillerdroidO->pistonPos[1] = 0;

    DrillerdroidO->drillMoveDir[0] = FLIP_NONE;
    DrillerdroidO->drillMoveDir[1] = FLIP_NONE;

    DrillerdroidO->pistonMoveDir[0] = FLIP_NONE;
    DrillerdroidO->pistonMoveDir[1] = FLIP_NONE;

    DrillerdroidO->childSlotStart = SceneInfo->entitySlot + 2;

    switch (self->type) {
        case DRILLERDROIDO_MAIN:
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(DrillerdroidO->aniFrames, 5, &self->armorAnimator, true, 0);
            DrillerdroidO_Draw_Boss();

            if (showGizmos()) {
                RSDK_DRAWING_OVERLAY(true);

                // Note: its actually centerX offset on left/right but mania is usually always at 424 width anyways so its pretty much the same
                DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 96, 1 | 2 | 4 | 8, 0x00C0F0);

                if (LRZRockPile) {
                    for (int32 i = 0; i < 9; ++i) {
                        EntityLRZRockPile *pile = RSDK_GET_ENTITY(DrillerdroidO->childSlotStart++, LRZRockPile);

                        if (pile && pile->classID == LRZRockPile->classID)
                            DrawHelpers_DrawArrow(self->position.x, self->position.y, pile->position.x, pile->position.y, 0xFFFF00, INK_NONE, 0xFF);
                    }
                }

                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case DRILLERDROIDO_FIREBALLEMITTER:
            RSDK.SetSpriteAnimation(DrillerdroidO->ticFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 3, &self->armorAnimator, true, 0);

            DrawHelpers_DrawRectOutline(self->position.x, self->position.y + (0xC0 << 15), 0x40 << 16, 0xC0 << 16, 0xFFFF00);
            break;

        default: break;
    }
}

void DrillerdroidO_EditorLoad(void)
{
    DrillerdroidO->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Drillerdroid.bin", SCOPE_STAGE);
    DrillerdroidO->ticFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(DrillerdroidO, type);
    RSDK_ENUM_VAR("Drillerdroid", DRILLERDROIDO_MAIN);
    RSDK_ENUM_VAR("Fireball Emitter", DRILLERDROIDO_FIREBALLEMITTER);
}
#endif

void DrillerdroidO_Serialize(void) { RSDK_EDITABLE_VAR(DrillerdroidO, VAR_UINT8, type); }
