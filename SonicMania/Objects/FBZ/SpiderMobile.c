// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SpiderMobile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpiderMobile *SpiderMobile;

void SpiderMobile_Update(void)
{
    RSDK_THIS(SpiderMobile);

    StateMachine_Run(self->state);

    self->groundedStore = self->onGround;
}

void SpiderMobile_LateUpdate(void) {}

void SpiderMobile_StaticUpdate(void) {}

void SpiderMobile_Draw(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->partAnimator, NULL, false);
    }
}

void SpiderMobile_Create(void *data)
{
    RSDK_THIS(SpiderMobile);
    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
    }
    else {
        self->drawFX = FX_FLIP;

        if (!SceneInfo->inEditor) {
            if (globals->gameMode >= MODE_TIMEATTACK) {
                destroyEntity(self);
            }
            else {
                self->alpha = 0xFF;
                switch (VOID_TO_INT(data)) {
                    case SPIDERMOBILE_BOSS:
                        self->visible           = false;
                        self->drawGroup         = Zone->objectDrawGroup[1];
                        self->drawFX            = FX_ROTATE | FX_FLIP;
                        self->webCurveDirection = 1;
                        self->health            = 6;
                        self->active            = ACTIVE_BOUNDS;
                        self->updateRange.x     = 0x400000;
                        self->updateRange.y     = 0x400000;

                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &self->bumperAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &self->starAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->headAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &self->orbAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 4, &self->pincerAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &self->legsAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 6, &self->webAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 7, &self->eggmanAnimator, true, 0);

                        self->stateDraw = SpiderMobile_Draw_Body;
                        self->state     = SpiderMobile_StateBody_AwaitPlayer;
                        break;

                    case SPIDERMOBILE_ORB:
                        self->active        = ACTIVE_NORMAL;
                        self->updateRange.x = 0x300000;
                        self->updateRange.y = 0x300000;
                        self->visible       = true;
                        self->drawGroup     = Zone->objectDrawGroup[1];

                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 14, &self->partAnimator, true, 0);

                        self->state = SpiderMobile_StateOrb_Charge;
                        break;

                    case SPIDERMOBILE_BUMPERDEBRIS:
                        self->visible       = true;
                        self->drawGroup     = Zone->objectDrawGroup[1];
                        self->drawFX        = FX_ROTATE | FX_FLIP;
                        self->active        = ACTIVE_NORMAL;
                        self->updateRange.x = 0x800000;
                        self->updateRange.y = 0x800000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &self->bumperAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &self->starAnimator, true, 0);

                        self->velocity.x = RSDK.Rand(-0x20000, 0x20000);
                        self->velocity.y = -0x40000;
                        self->stateDraw  = SpiderMobile_Draw_Bumper;
                        self->state      = SpiderMobile_State_Bumper;
                        break;

                    case SPIDERMOBILE_EGGMAN:
                        self->visible       = true;
                        self->drawGroup     = Zone->objectDrawGroup[0];
                        self->active        = ACTIVE_NORMAL;
                        self->updateRange.x = 0x800000;
                        self->updateRange.y = 0x800000;

                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 11, &self->partAnimator, true, 0);

                        self->velocity.x = 0x10000;
                        self->velocity.y = -0x20000;
                        self->state      = SpiderMobile_State_Eggman;
                        break;

                    default: break;
                }
            }
        }
    }
}

void SpiderMobile_StageLoad(void)
{
    SpiderMobile->aniFrames = RSDK.LoadSpriteAnimation("FBZ/SpiderMobile.bin", SCOPE_STAGE);

    SpiderMobile->hitboxBumper.left   = -24;
    SpiderMobile->hitboxBumper.top    = -24;
    SpiderMobile->hitboxBumper.right  = 24;
    SpiderMobile->hitboxBumper.bottom = 24;

    SpiderMobile->hitboxSpikes.left   = -24;
    SpiderMobile->hitboxSpikes.top    = 24;
    SpiderMobile->hitboxSpikes.right  = 24;
    SpiderMobile->hitboxSpikes.bottom = 24;

    SpiderMobile->hitboxOrb.left   = -8;
    SpiderMobile->hitboxOrb.top    = -8;
    SpiderMobile->hitboxOrb.right  = 8;
    SpiderMobile->hitboxOrb.bottom = 8;

    SpiderMobile->hitboxPlatform.left   = -512;
    SpiderMobile->hitboxPlatform.top    = 256;
    SpiderMobile->hitboxPlatform.right  = 512;
    SpiderMobile->hitboxPlatform.bottom = 288;

    SpiderMobile->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    SpiderMobile->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    SpiderMobile->sfxBumper    = RSDK.GetSfx("Stage/Bumper.wav");
    SpiderMobile->sfxCharge    = RSDK.GetSfx("FBZ/SpiderCharge.wav");
    SpiderMobile->sfxElecBall  = RSDK.GetSfx("FBZ/SpiderElecBall.wav");
    SpiderMobile->sfxFall      = RSDK.GetSfx("FBZ/SpiderFall.wav");
    SpiderMobile->sfxFlailing  = RSDK.GetSfx("FBZ/SpiderFlailing.wav");
    SpiderMobile->sfxHitGround = RSDK.GetSfx("FBZ/SpiderHitGround.wav");
    SpiderMobile->sfxRecovery  = RSDK.GetSfx("FBZ/SpiderRecovery.wav");
    SpiderMobile->sfxElevator  = RSDK.GetSfx("FBZ/Elevator.wav");
    SpiderMobile->sfxHullClose = RSDK.GetSfx("Stage/HullClose.wav");
    Soundboard_LoadSfx("FBZ/Elevator.wav", true, SpiderMobile_SfxCheck_Elevator, StateMachine_None);
}

void SpiderMobile_HandleFallingMovement(void)
{
    RSDK_THIS(SpiderMobile);

    self->velocity.y += 0x3000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 offsetX = self->position.x - self->origin.x;
    int32 offsetY = (self->position.y - self->origin.y) >> 16;
    if (self->position.y > self->origin.y)
        self->angle = 2 * RSDK.ATan2(offsetY, offsetX / -0x11000);

    self->webCurveAngle = offsetY * offsetY + (offsetX / 0x11000) * (offsetX / 0x11000);
    if (self->bounceDelay > 0)
        self->bounceDelay--;

    if (self->webCurveAngle > 0x9000 && !self->bounceDelay) {
        if (self->bounceCount == 2) {
            self->bounceCount = 0;
            self->onGround    = true;
            if (!self->angle)
                self->timer = 32;
            if (self->angle > 0x100)
                self->angle -= 0x200;

            self->velocity.x    = 0;
            self->bounceDelay   = 0;
            self->moveAmplitude = 0x6000;
            self->angleVel      = self->angle << 15;
        }
        else {
            RSDK.PlaySfx(SpiderMobile->sfxHitGround, false, 0xFF);
            Camera_ShakeScreen(0, 0, 3);
            ++self->bounceCount;

            int32 amp               = (abs(self->velocity.x) + abs(self->velocity.y)) >> 1;
            self->webCurveDirection = -self->webCurveDirection;
            self->bounceDelay       = 8;
            self->velocity.x        = (amp * RSDK.Sin512(self->angle)) >> 9;
            self->velocity.y        = -(amp * RSDK.Cos512(self->angle)) >> 9;
        }
    }

    self->webCurveAngle = self->webCurveDirection * (self->webCurveAngle - 0x9000);
}

void SpiderMobile_HandleRisingMovement(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->bounceDelay <= 0) {
        if (self->angleVel <= 0) {
            if (self->velocity.x < 0x20000)
                self->velocity.x += 0x600;
        }
        else {
            if (self->velocity.x > -0x20000)
                self->velocity.x -= 0x600;
        }
    }
    else {
        self->velocity.x += 0x200;
        self->bounceDelay--;
    }

    self->angleVel += self->velocity.x;
    self->webCurveAngle -= (self->webCurveAngle >> 2);

    self->angle      = self->angleVel >> 15;
    self->position.x = self->origin.x - self->moveAmplitude * RSDK.Sin512(self->angle);
    self->position.y = self->origin.y + self->moveAmplitude * RSDK.Cos512(self->angle);
}

void SpiderMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpiderMobile);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &SpiderMobile->hitboxBumper)) {
            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

            int32 amp = (abs(player->velocity.x) + abs(player->velocity.y)) >> 9;
            if (!self->knockbackCooldown && !player->sidekick && (amp > 0x600 || self->state != SpiderMobile_StateBody_HandleOrbAttack)) {
                self->velocity.x        = (3 * player->velocity.x) >> 2;
                self->velocity.y        = MIN((3 * player->velocity.y) >> 2, -0x10000);
                self->knockbackCooldown = 4;
                self->onGround          = false;
            }

            amp                = MAX((abs(player->velocity.x) + abs(player->velocity.y)) >> 9, 0x600);
            player->velocity.x = amp * RSDK.Cos256(angle);
            player->velocity.y = amp * RSDK.Sin256(angle);

            RSDK.PlaySfx(SpiderMobile->sfxBumper, false, 255);

            foreach_break;
        }
    }
}

void SpiderMobile_CheckSpikeHit(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->invincibilityTimer <= 0) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        self->position.x = self->headPos.x - (RSDK.Sin512(self->headRotation + self->angle) << 13);
        self->position.y = self->headPos.y + (RSDK.Cos512(self->headRotation + self->angle) << 13);

        foreach_active(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionTouchBox(spikes, &spikes->hitbox, self, &SpiderMobile->hitboxSpikes))
                SpiderMobile_Hit();
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
    else {
        self->invincibilityTimer--;
    }
}

void SpiderMobile_Hit(void)
{
    RSDK_THIS(SpiderMobile);

    if (--self->health <= 0) {
        self->state            = SpiderMobile_StateBody_Destroyed;
        self->timer            = 0;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 10, &self->eggmanAnimator, true, 0);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(SpiderMobile->sfxHit, false, 255);
        RSDK.PlaySfx(SpiderMobile->sfxFlailing, false, 255);
    }
}

void SpiderMobile_Explode(void)
{
    RSDK_THIS(SpiderMobile);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(SpiderMobile->sfxExplosion, false, 0xFF);

        if (Zone->timer & 4) {
            int32 x                    = self->headPos.x + (RSDK.Rand(-48, 48) << 16);
            int32 y                    = self->headPos.y + (RSDK.Rand(-48, 48) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
        }
    }
}

void SpiderMobile_HandlePlatformMovement(void)
{
    RSDK_THIS(SpiderMobile);

    int32 offsetY = 0x7000000;
    if (Zone->cameraBoundsB[0] > 5508 || self->state == SpiderMobile_StateBody_MovePlatformToEnd)
        offsetY = self->offsetY;

    Zone->cameraBoundsT[0] += offsetY >> 16;
    Zone->cameraBoundsB[0] += offsetY >> 16;
    Zone->deathBoundary[0] += offsetY;
    Zone->playerBoundsB[0] += offsetY;
    RSDK.GetTileLayer(Zone->fgLayer[1])->scrollPos -= offsetY;

    self->position.y += offsetY;
    self->origin.y += offsetY;

    int32 storeX     = self->position.x;
    int32 storeY     = self->position.y;
    self->position.x = self->origin.x;
    self->position.y = self->origin.y;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->classID == Player->classID && player->state == Player_State_Transform)
            Player_CheckCollisionPlatform(player, self, &SpiderMobile->hitboxPlatform);
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, &SpiderMobile->hitboxPlatform)) {
            player->collisionFlagV |= 1;
        }
        else if ((player->groundedStore || player->state == Player_State_Static) && offsetY == self->offsetY) {
            if (player->state == Player_State_Static)
                player->position.y += offsetY >> 1;
        }
        else {
            player->position.y += offsetY;
        }

        if (player->camera) {
            player->camera->position.y += offsetY;
            player->camera->boundsT += offsetY >> 16;
            player->camera->boundsB += offsetY >> 16;
        }

        if (player->state == Player_State_Static) {
            uint16 tile = RSDK.GetTile(Zone->fgLayer[0], player->position.x >> 20, player->position.y >> 20);
            if (tile == (uint16)-1 || (tile & 0x3FF) == 669 || (tile & 0x3FF) == 379) {
                player->drawGroup = Zone->playerDrawGroup[0];
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                player->state      = Player_State_Air;
                player->velocity.y = -0x20000;
            }
        }
    }

    foreach_active(Ring, ring)
    {
        if (ring->state == Ring_State_Lost) {
            ring->position.y += offsetY;
            if (ring->velocity.y >= 0) {
                int32 velY = ring->velocity.y;
                if (RSDK.CheckObjectCollisionPlatform(self, &SpiderMobile->hitboxPlatform, ring, &Ring->hitbox, true)) {
                    ring->velocity.y = (velY >> 2) - velY;
                    if (ring->velocity.y > -0x10000)
                        ring->velocity.y = -0x10000;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void SpiderMobile_HandleWebClimbArmMovement(void)
{
    RSDK_THIS(SpiderMobile);

    int32 angle = 16 * Zone->timer;
    if (self->armMoveAmplitude >= 0x40) {
        self->legAngles[0] = (RSDK.Sin256(angle) >> 3) - (self->velocity.y >> 14) - 64;
        self->legAngles[6] = (RSDK.Sin256(angle + 224) >> 3) + (self->velocity.y >> 14) + 64;
    }
    else {
        self->legAngles[0] = ((self->legAngles[0] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle) >> 3) - (self->velocity.y >> 14) - 0x40)) >> 6);
        self->legAngles[6] = ((self->legAngles[6] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((self->velocity.y >> 14) + 0x40 + (RSDK.Sin256(angle + 0xE0) >> 3))) >> 6);

        self->armMoveAmplitude += 4;
    }

    if (self->legAngles[2] < 0x28)
        self->legAngles[2] += 2;

    if (self->legAngles[4] < 0x40)
        self->legAngles[4] += 2;

    if (self->legAngles[8] > -0x28)
        self->legAngles[8] -= 2;

    if (self->legAngles[10] > -0x40)
        self->legAngles[10] -= 2;

    for (int32 i = 0; i < 12; i += 2) self->legAngles[i + 1] = self->legAngles[i];
}

void SpiderMobile_HandleDestroyedArmMovement(void)
{
    RSDK_THIS(SpiderMobile);

    int32 angle = 16 * Zone->timer;
    if (self->armMoveAmplitude >= 0x40) {
        self->legAngles[0]  = (RSDK.Sin256(angle + 0x00) >> 3) - (self->velocity.y >> 14) - 0x40;
        self->legAngles[2]  = (RSDK.Sin256(angle + 0x40) >> 4) - (self->velocity.y >> 15) - 0x10;
        self->legAngles[4]  = (RSDK.Sin256(angle + 0x80) >> 3) - (self->velocity.y >> 14) + 0x30;
        self->legAngles[6]  = (RSDK.Sin256(angle + 0xE0) >> 3) + (self->velocity.y >> 14) + 0x40;
        self->legAngles[8]  = (RSDK.Sin256(angle + 0x120) >> 4) + (self->velocity.y >> 15) + 0x10;
        self->legAngles[10] = (RSDK.Sin256(angle + 0x160) >> 3) + (self->velocity.y >> 14) - 0x30;
    }
    else {
        self->legAngles[0] = ((self->legAngles[0] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x00) >> 3) - (self->velocity.y >> 14) - 0x40)) >> 6);

        self->legAngles[2] = ((self->legAngles[2] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x40) >> 4) - (self->velocity.y >> 15) - 0x10)) >> 6);

        self->legAngles[4] = ((self->legAngles[4] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x80) >> 3) - (self->velocity.y >> 14) + 0x30)) >> 6);

        self->legAngles[6] = ((self->legAngles[6] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0xE0) >> 3) + (self->velocity.y >> 14) + 0x40)) >> 6);

        self->legAngles[8] = ((self->legAngles[8] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x120) >> 4) + (self->velocity.y >> 15) + 0x10)) >> 6);

        self->legAngles[10] = ((self->legAngles[10] * (0x40 - self->armMoveAmplitude)) >> 6)
                              + ((self->armMoveAmplitude * ((self->velocity.y >> 14) - 0x30 + (RSDK.Sin256(angle + 0x160) >> 3))) >> 6);

        self->armMoveAmplitude += 4;
    }

    for (int32 i = 0; i < 12; i += 2) self->legAngles[i + 1] = self->legAngles[i];
}

void SpiderMobile_HandleIdleArmMovement(void)
{
    RSDK_THIS(SpiderMobile);

    int32 angle = 4 * Zone->timer;
    if (self->armMoveAmplitude >= 0x40) {
        self->legAngles[0]  = (RSDK.Sin256(angle + 0x00) >> 5) - (self->velocity.y >> 14) - 0x40;
        self->legAngles[2]  = (RSDK.Sin256(angle + 0x40) >> 5) - (self->velocity.y >> 15) - 0x10;
        self->legAngles[4]  = (RSDK.Sin256(angle + 0x80) >> 5) - (self->velocity.y >> 14) + 0x30;
        self->legAngles[6]  = (RSDK.Sin256(angle + 0xE0) >> 5) + (self->velocity.y >> 14) + 0x40;
        self->legAngles[8]  = (RSDK.Sin256(angle + 0x120) >> 5) + (self->velocity.y >> 15) + 0x10;
        self->legAngles[10] = (RSDK.Sin256(angle + 0x160) >> 5) + (self->velocity.y >> 14) - 0x30;
    }
    else {
        self->legAngles[0] = ((self->legAngles[0] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x00) >> 5) - (self->velocity.y >> 14) - 0x40)) >> 6);

        self->legAngles[2] = ((self->legAngles[2] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x40) >> 5) - (self->velocity.y >> 15) - 0x10)) >> 6);

        self->legAngles[4] = ((self->legAngles[4] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x80) >> 5) - (self->velocity.y >> 14) + 0x30)) >> 6);

        self->legAngles[6] = ((self->legAngles[6] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0xE0) >> 5) + (self->velocity.y >> 14) + 0x40)) >> 6);

        self->legAngles[8] = ((self->legAngles[8] * (0x40 - self->armMoveAmplitude)) >> 6)
                             + ((self->armMoveAmplitude * ((RSDK.Sin256(angle + 0x120) >> 5) + (self->velocity.y >> 15) + 0x10)) >> 6);

        self->legAngles[10] = ((self->legAngles[10] * (0x40 - self->armMoveAmplitude)) >> 6)
                              + ((self->armMoveAmplitude * ((self->velocity.y >> 14) - 0x30 + (RSDK.Sin256(angle + 0x160) >> 5))) >> 6);

        self->armMoveAmplitude += 4;
    }

    for (int32 i = 0; i < 12; i += 2) {
        self->legAngles[i + 1] = self->legAngles[i];
    }
}

void SpiderMobile_UpdateLimbPositions(void)
{
    RSDK_THIS(SpiderMobile);

    self->headPos.x = self->position.x - (RSDK.Sin512(self->angle) << 12);
    self->headPos.y = self->position.y + (RSDK.Cos512(self->angle) << 12);

    self->legJointPos[0].x = (self->position.x - (RSDK.Sin512(self->angle) << 12)) + 0xC00 * RSDK.Cos512(self->angle);
    self->legJointPos[0].y = (self->position.y + (RSDK.Cos512(self->angle) << 12)) + 0xC00 * RSDK.Sin512(self->angle);

    self->legJointPos[1].x = (self->position.x - (RSDK.Sin512(self->angle) << 12)) - 0xC00 * RSDK.Cos512(self->angle);
    self->legJointPos[1].y = (self->position.y + (RSDK.Cos512(self->angle) << 12)) - 0xC00 * RSDK.Sin512(self->angle);

    int32 angle             = self->angle + self->headRotation;
    self->pincer1StartPos.x = (self->headPos.x - 0x1A00 * RSDK.Sin512(angle)) - 0x900 * RSDK.Cos512(angle);
    self->pincer1StartPos.y = (self->headPos.y + 0x1A00 * RSDK.Cos512(angle)) - 0x900 * RSDK.Sin512(angle);

    self->pincer1EndPos.x = self->pincer1StartPos.x - (0x600 * RSDK.Sin512(angle + self->pincerRotation));
    self->pincer1EndPos.y = self->pincer1StartPos.y + (0x600 * RSDK.Cos512(angle + self->pincerRotation));

    self->pincer2StartPos.x = (self->headPos.x - 0x1A00 * RSDK.Sin512(angle)) + 0x900 * RSDK.Cos512(angle);
    self->pincer2StartPos.y = (self->headPos.y + 0x1A00 * RSDK.Cos512(angle)) + 0x900 * RSDK.Sin512(angle);

    self->pincer2EndPos.x = self->pincer2StartPos.x - (0x600 * RSDK.Sin512(angle - self->pincerRotation));
    self->pincer2EndPos.y = self->pincer2StartPos.y + (0x600 * RSDK.Cos512(angle - self->pincerRotation));

    for (int32 i = 0; i < 6; i += 2) {
        self->legPositions[i + 0].x = self->legJointPos[0].x + (0x1100 * RSDK.Cos512(self->angle + self->legAngles[i]));
        self->legPositions[i + 0].y = self->legJointPos[0].y + (0x1100 * RSDK.Sin512(self->angle + self->legAngles[i]));

        self->legPositions[i + 1].x = self->legPositions[i].x + (0xF00 * RSDK.Cos512(self->angle + self->legAngles[i] + self->legAngles[i + 1]));
        self->legPositions[i + 1].y = self->legPositions[i].y + (0xF00 * RSDK.Sin512(self->angle + self->legAngles[i] + self->legAngles[i + 1]));
    }

    for (int32 i = 6; i < 12; i += 2) {
        self->legPositions[i + 0].x = self->legJointPos[1].x - (0x1100 * RSDK.Cos512(self->angle + self->legAngles[i]));
        self->legPositions[i + 0].y = self->legJointPos[1].y - (0x1100 * RSDK.Sin512(self->angle + self->legAngles[i]));

        self->legPositions[i + 1].x = self->legPositions[i].x - (0xF00 * RSDK.Cos512(self->angle + self->legAngles[i] + self->legAngles[i + 1]));
        self->legPositions[i + 1].y = self->legPositions[i].y - (0xF00 * RSDK.Sin512(self->angle + self->legAngles[i] + self->legAngles[i + 1]));
    }
}

void SpiderMobile_Draw_Body(void)
{
    RSDK_THIS(SpiderMobile);

    self->inkEffect = INK_NONE;
    for (int32 i = 0; i < 6; i += 2) {
        self->legsAnimator.frameID = 1;
        self->rotation             = self->angle + self->legAngles[i];
        RSDK.DrawSprite(&self->legsAnimator, &self->legPositions[i], false);

        self->legsAnimator.frameID = 0;
        self->rotation += self->legAngles[i + 1];
        RSDK.DrawSprite(&self->legsAnimator, &self->legPositions[i], false);
    }

    for (int32 i = 6; i < 12; i += 2) {
        self->legsAnimator.frameID = 0;
        self->rotation             = self->angle + self->legAngles[i];
        RSDK.DrawSprite(&self->legsAnimator, &self->legPositions[i], false);

        self->legsAnimator.frameID = 1;
        self->rotation += self->legAngles[i + 1];
        RSDK.DrawSprite(&self->legsAnimator, &self->legPositions[i], false);
    }

    for (int32 i = 0; i < 12; i += 2) {
        self->orbAnimator.frameID = 2;
        RSDK.DrawSprite(&self->orbAnimator, &self->legPositions[i + 0], false);

        self->orbAnimator.frameID = 1;
        RSDK.DrawSprite(&self->orbAnimator, &self->legPositions[i + 1], false);
    }

    self->orbAnimator.frameID = 0;
    RSDK.DrawSprite(&self->orbAnimator, &self->legJointPos[0], false);
    RSDK.DrawSprite(&self->orbAnimator, &self->legJointPos[1], false);

    self->rotation = self->headRotation + self->angle;
    if (!(self->invincibilityTimer & 1)) {
        RSDK.DrawSprite(&self->headAnimator, &self->headPos, false);
    }
    else {
        RSDK.SetPaletteEntry(0, 1, 0xF0F0F0);
        RSDK.CopyPalette(1, 32, 0, 32, 10);

        RSDK.DrawSprite(&self->headAnimator, &self->headPos, false);
        RSDK.SetPaletteEntry(0, 1, 0x000000);
        RSDK.CopyPalette(2, 32, 0, 32, 10);
    }
    RSDK.DrawSprite(&self->eggmanAnimator, &self->headPos, false);
    RSDK.DrawSprite(&self->electricAnimator, &self->headPos, false);

    self->rotation               = self->headRotation + self->angle + self->pincerRotation;
    self->pincerAnimator.frameID = 0;
    self->direction              = FLIP_NONE;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer1StartPos, false);

    self->direction = FLIP_X;
    self->rotation  = self->headRotation + self->angle - self->pincerRotation;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer2StartPos, false);

    self->pincerAnimator.frameID = 1;
    self->direction              = FLIP_NONE;
    self->rotation               = self->headRotation + (self->pincerRotation + (self->pincerRotation >> 1)) + self->angle;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer1EndPos, false);

    self->direction = FLIP_X;
    self->rotation  = self->headRotation + (self->angle - (self->pincerRotation >> 1)) - self->pincerRotation;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer2EndPos, false);

    self->direction           = FLIP_NONE;
    self->orbAnimator.frameID = 2;
    RSDK.DrawSprite(&self->orbAnimator, &self->pincer1StartPos, false);
    RSDK.DrawSprite(&self->orbAnimator, &self->pincer2StartPos, false);

    Vector2 drawPos;
    drawPos.x = self->position.x + (0x1300 * RSDK.Sin512(self->angle));
    drawPos.y = self->position.y + (-0x1300 * RSDK.Cos512(self->angle));

    self->webAnimator.frameID = 1;
    for (int32 i = 0; i < 16; ++i) {
        self->rotation = ((self->webCurveAngle * RSDK.Sin256(32 * i)) >> 16) + self->angle;
        RSDK.DrawSprite(&self->webAnimator, &drawPos, false);

        drawPos.x += 0x700 * RSDK.Sin512(self->rotation);
        drawPos.y += -0x700 * RSDK.Cos512(self->rotation);
    }

    self->webAnimator.frameID = 0;
    self->rotation            = self->angle;
    RSDK.DrawSprite(&self->webAnimator, NULL, false);

    if (self->knockbackCooldown > 0)
        RSDK.SetPaletteEntry(0, 18, 0xE0E040);

    self->bumperAnimator.frameID = 0;
    RSDK.DrawSprite(&self->bumperAnimator, NULL, false);
    RSDK.DrawSprite(&self->starAnimator, NULL, false);

    self->inkEffect              = INK_SUB;
    self->bumperAnimator.frameID = 1;
    RSDK.DrawSprite(&self->bumperAnimator, NULL, false);

    self->inkEffect              = INK_ADD;
    self->bumperAnimator.frameID = 2;
    RSDK.DrawSprite(&self->bumperAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 18, 0xE00000);
}

void SpiderMobile_Draw_Bumper(void)
{
    RSDK_THIS(SpiderMobile);

    if (!(Zone->timer & 1)) {
        self->inkEffect              = INK_NONE;
        self->bumperAnimator.frameID = 0;
        RSDK.DrawSprite(&self->bumperAnimator, NULL, false);
        RSDK.DrawSprite(&self->starAnimator, NULL, false);

        self->inkEffect              = INK_SUB;
        self->bumperAnimator.frameID = 1;
        RSDK.DrawSprite(&self->bumperAnimator, NULL, false);

        self->inkEffect              = INK_ADD;
        self->bumperAnimator.frameID = 2;
        RSDK.DrawSprite(&self->bumperAnimator, NULL, false);
    }
}

void SpiderMobile_Draw_Cockpit(void)
{
    RSDK_THIS(SpiderMobile);

    RSDK.DrawSprite(&self->headAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);
}

void SpiderMobile_StateBody_AwaitPlayer(void)
{
    RSDK_THIS(SpiderMobile);

    if (++self->timer >= 8) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (abs(player1->position.y - self->position.y) <= 0xA00000) {
            self->timer = 0;

            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            Zone->cameraBoundsL[0]      = FROM_FIXED(self->position.x) - WIDE_SCR_XCENTER;
            Zone->cameraBoundsR[0]      = FROM_FIXED(self->position.x) + WIDE_SCR_XCENTER;
            Zone->cameraBoundsT[0]      = FROM_FIXED(self->position.y) - ScreenInfo->size.y;
            Zone->cameraBoundsB[0]      = FROM_FIXED(self->position.y);

            SpiderMobile->boundsL = TO_FIXED(Zone->cameraBoundsL[0] + 64);
            SpiderMobile->boundsR = TO_FIXED(Zone->cameraBoundsR[0] - 64);
            SpiderMobile->boundsM = self->position.x;
            SpiderMobile->boundsT = TO_FIXED(Zone->cameraBoundsT[0] + 48);
            SpiderMobile->boundsB = TO_FIXED(Zone->cameraBoundsB[0] - 8);

            self->state = SpiderMobile_StateBody_SetupArena;
            FBZSetup_Trigger_ShowInterior();
        }
        else {
            self->timer = 8;
        }
    }
}

void SpiderMobile_StateBody_SetupArena(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->timer) {
        self->timer++;
        if (self->timer == 120) {
            self->timer   = 0;
            self->visible = true;
            RSDK.PlaySfx(SpiderMobile->sfxRecovery, false, 255);
            int32 offsetX = self->origin.x - self->position.x;
            int32 offsetY = self->origin.y - self->position.y;
            self->state   = SpiderMobile_StateBody_InitialDrop;

            self->position.x = self->origin.x;
            self->position.y += offsetY;

            foreach_all(Player, player)
            {
                player->position.x += offsetX;
                player->position.y += offsetY;

                if (player->camera) {
                    player->camera->position.x += offsetX;
                    player->camera->position.y += offsetY;
                    player->camera->boundsL += offsetX >> 16;
                    player->camera->boundsR += offsetX >> 16;
                    player->camera->boundsT += offsetY >> 16;
                    player->camera->boundsB += offsetY >> 16;
                }
            }

            foreach_all(LightBarrier, barrier)
            {
                barrier->position.x += offsetX;
                barrier->position.y += offsetY;
            }

            Zone->cameraBoundsL[0] += offsetX >> 16;
            Zone->cameraBoundsR[0] += offsetX >> 16;
            Zone->cameraBoundsT[0] += offsetY >> 16;
            Zone->cameraBoundsB[0] += offsetY >> 16;
            Zone->playerBoundsL[0] += offsetX;
            Zone->playerBoundsR[0] += offsetX;
            Zone->playerBoundsT[0] += offsetY;
            Zone->playerBoundsB[0] += offsetY;
            Zone->deathBoundary[0] = 0x7FFFFFFF;

            self->origin.y -= 0x1200000;
            self->offsetY = -0x10000;
            self->position.y += -0x800000 - (ScreenInfo->size.y << 16);
            self->armMoveAmplitude = 64;
            self->active           = ACTIVE_NORMAL;
            BGSwitch->screenID     = 0;
            FBZSetup_BGSwitch_ShowInside2();
            FBZSetup_BGSwitch_ShowInside1();

            TileLayer *overlay    = RSDK.GetTileLayer(RSDK.GetTileLayerID("Exterior Overlay"));
            overlay->drawGroup[0] = 0;
        }
    }
    else {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > self->position.x) {
            if (abs(player1->position.y - self->position.y) < 0xA00000) {
                Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);

                foreach_active(LightBarrier, barrier) { barrier->enabled = true; }
                ++self->timer;
            }
        }
    }
}

void SpiderMobile_StateBody_InitialDrop(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->onGround) {
        if (!self->groundedStore)
            RSDK.PlaySfx(SpiderMobile->sfxFall, false, 255);

        self->state = SpiderMobile_StateBody_Rise;
        Camera_ShakeScreen(0, 0, 4);
    }
    else {
        SpiderMobile_HandleFallingMovement();
    }

    SpiderMobile_HandleIdleArmMovement();
    SpiderMobile_UpdateLimbPositions();
}

// Rises for a bit, then uses the arms to climb
void SpiderMobile_StateBody_Rise(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->knockbackCooldown > 0)
        self->knockbackCooldown--;

    if (self->onGround) {
        SpiderMobile_HandleRisingMovement();
        SpiderMobile_CheckPlayerCollisions();

        self->velocity.y -= self->velocity.y >> 3;
        if (self->moveAmplitude > 0x4000)
            self->moveAmplitude -= 0x40;
    }
    else {
        if (!self->bounceDelay)
            SpiderMobile_CheckPlayerCollisions();

        SpiderMobile_HandleFallingMovement();
    }

    if (self->pincerRotation > 0)
        self->pincerRotation -= 4;

    if (self->headRotation < 0) {
        self->headRotation += 4;
        if (self->headRotation > 0)
            self->headRotation = 0;
    }
    else if (self->headRotation > 0) {
        self->headRotation -= 4;
        if (self->headRotation < 0)
            self->headRotation = 0;
    }

    if (++self->timer == 120) {
        self->timer            = 0;
        self->armMoveAmplitude = 0;
        self->state            = SpiderMobile_StateBody_Climb;
    }

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_HandleIdleArmMovement();
    SpiderMobile_UpdateLimbPositions();
    SpiderMobile_CheckSpikeHit();
}

void SpiderMobile_StateBody_Climb(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->knockbackCooldown > 0)
        self->knockbackCooldown--;

    if (self->pincerRotation > 0)
        self->pincerRotation -= 4;

    if (self->headRotation < 0) {
        self->headRotation += 4;

        if (self->headRotation > 0)
            self->headRotation = 0;
    }
    else if (self->headRotation > 0) {
        self->headRotation -= 4;

        if (self->headRotation < 0)
            self->headRotation = 0;
    }

    if (self->onGround) {
        SpiderMobile_HandleRisingMovement();
        SpiderMobile_CheckPlayerCollisions();

        self->velocity.y -= self->velocity.y >> 3;
        if (self->moveAmplitude <= 0x2800) {
            self->armMoveAmplitude = 0;
            self->state            = SpiderMobile_StateBody_HandleOrbAttack;
        }
        else {
            self->moveAmplitude -= 0x80;
        }
    }
    else {
        SpiderMobile_HandleFallingMovement();
    }

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_HandleWebClimbArmMovement();
    SpiderMobile_UpdateLimbPositions();
    SpiderMobile_CheckSpikeHit();
}

void SpiderMobile_StateBody_HandleOrbAttack(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    SpiderMobile_HandleRisingMovement();
    SpiderMobile_CheckPlayerCollisions();

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < self->headPos.x) {
        if (self->headRotation < 72)
            self->headRotation += 2;
    }
    else {
        if (self->headRotation > -72)
            self->headRotation -= 2;
    }

    if (self->pincerRotation >= 56) {
        RSDK.ProcessAnimation(&self->electricAnimator);

        if (!(self->timer % 60))
            CREATE_ENTITY(SpiderMobile, INT_TO_VOID(SPIDERMOBILE_ORB), self->position.x, self->position.y)->parent = self;
    }
    else {
        self->pincerRotation++;
        if (self->pincerRotation == 56)
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 12, &self->electricAnimator, false, 0);
    }

    if (++self->timer == 240) {
        self->timer    = 0;
        self->onGround = false;
        SpiderMobile_HandleFallingMovement();

        self->state = SpiderMobile_StateBody_Rise;
        RSDK.SetSpriteAnimation(-1, 0, &self->electricAnimator, false, 0);
    }
    else if (!self->onGround) {
        self->state = SpiderMobile_StateBody_Climb;
        RSDK.SetSpriteAnimation(-1, 0, &self->electricAnimator, false, 0);
    }

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_HandleIdleArmMovement();
    SpiderMobile_UpdateLimbPositions();
}

void SpiderMobile_StateBody_Destroyed(void)
{
    RSDK_THIS(SpiderMobile);

    SpiderMobile_Explode();
    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_HandleDestroyedArmMovement();
    SpiderMobile_UpdateLimbPositions();

    self->timer++;
    if (self->timer == 60) {
        EntityDebris *debris = NULL;

        // Leg Body Joint Debris
        for (int32 i = 0; i < 2; ++i) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legJointPos[i].x, self->legJointPos[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 0);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->drawFX          = FX_ROTATE | FX_FLIP;
        }

        // Leg Debris L
        for (int32 i = 0; i < 6; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 1);
            debris->rotation        = self->angle + self->legAngles[i];
            debris->rotSpeed        = RSDK.Rand(-8, 8);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->drawFX          = FX_ROTATE | FX_FLIP;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 0);
            debris->rotation        = self->angle + self->legAngles[i] + self->legAngles[i + 1];
            debris->rotSpeed        = RSDK.Rand(-8, 8);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->drawFX          = FX_ROTATE | FX_FLIP;
        }

        // Leg Debris R
        for (int32 i = 6; i < 12; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 0);
            debris->rotation        = self->angle + self->legAngles[i];
            debris->rotSpeed        = RSDK.Rand(-8, 8);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->drawFX          = FX_ROTATE | FX_FLIP;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 1);
            debris->rotation        = self->angle + self->legAngles[i] + self->legAngles[i + 1];
            debris->rotSpeed        = RSDK.Rand(-8, 8);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->drawFX          = FX_ROTATE | FX_FLIP;
        }

        // Leg Joints Debris
        for (int32 i = 0; i < 12; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 2);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i + 1].x, self->legPositions[i + 1].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 1);
            debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
        }

        RSDK.SetSpriteAnimation(-1, 0, &self->legsAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->orbAnimator, true, 0);
    }
    else if (self->timer == 90) {
        CREATE_ENTITY(SpiderMobile, INT_TO_VOID(SPIDERMOBILE_BUMPERDEBRIS), self->position.x, self->position.y)->rotation = self->angle;
        RSDK.SetSpriteAnimation(-1, 0, &self->bumperAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->starAnimator, true, 0);
        self->position.x = self->headPos.x - 0xC80 * RSDK.Sin512(self->angle + self->headRotation);
        self->position.y = self->headPos.y + 0xC80 * RSDK.Cos512(self->angle + self->headRotation);
        self->rotation   = self->angle + self->headRotation;
        self->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        self->velocity.y = -0x40000;
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->headAnimator, true, 1);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->eggmanAnimator, true, 2);
        self->timer     = 0;
        self->inkEffect = INK_NONE;
        self->state     = SpiderMobile_StateBody_CockpitExplode;
        self->stateDraw = SpiderMobile_Draw_Cockpit;
    }
}

void SpiderMobile_StateBody_CockpitExplode(void)
{
    RSDK_THIS(SpiderMobile);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation += 8;

    SpiderMobile_Explode();

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_UpdateLimbPositions();

    if (++self->timer == 120) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer               = 0;
        self->visible             = false;
        self->state               = SpiderMobile_StateBody_MovePlatformToEnd;
        EntitySpiderMobile *child = CREATE_ENTITY(SpiderMobile, INT_TO_VOID(SPIDERMOBILE_EGGMAN), self->origin.x, self->origin.y + 0x1400000);
        child->parent             = self;
    }
}

void SpiderMobile_StateBody_MovePlatformToEnd(void)
{
    RSDK_THIS(SpiderMobile);

    if (++self->timer == 30) {
        self->offsetY -= 0x10000;
        self->timer = 0;
    }

    foreach_active(Spikes, spikes)
    {
        if (spikes->stateMove < SPIKES_MOVE_DISAPPEAR_FOREVER) {
            spikes->stateMove = SPIKES_MOVE_DISAPPEAR_FOREVER;
            spikes->drawGroup = 1;
        }
    }

    SpiderMobile_HandlePlatformMovement();

    if (Zone->cameraBoundsB[0] <= 5248) {
        self->timer            = 0;
        Zone->cameraBoundsB[0] = 5248;
        Zone->cameraBoundsR[0] += 1024;
        Camera_ShakeScreen(0, 0, 6);
        self->visible = false;
        RSDK.PlaySfx(SpiderMobile->sfxHullClose, false, 255);
        self->state       = SpiderMobile_StateBody_FinishedMovingPlatform;
        TileLayer *fgHigh = RSDK.GetTileLayer(Zone->fgLayer[1]);
        fgHigh->scrollPos = 0x9200000;
        self->origin.y    = 0x13600000;
    }
}

void SpiderMobile_StateBody_FinishedMovingPlatform(void)
{
    RSDK_THIS(SpiderMobile);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->origin.x;
    self->position.y = self->origin.y;
    foreach_active(Player, player) { Player_CheckCollisionPlatform(player, self, &SpiderMobile->hitboxPlatform); }

    self->position.x = storeX;
    self->position.y = storeY;
}

void SpiderMobile_State_Bumper(void)
{
    RSDK_THIS(SpiderMobile);

    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation += 2;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void SpiderMobile_State_Eggman(void)
{
    RSDK_THIS(SpiderMobile);

    RSDK.ProcessAnimation(&self->partAnimator);

    self->velocity.y -= 0x1000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (++self->timer == 180)
        destroyEntity(self);
}

void SpiderMobile_StateOrb_Charge(void)
{
    RSDK_THIS(SpiderMobile);

    EntitySpiderMobile *parent = self->parent;

    RSDK.ProcessAnimation(&self->partAnimator);
    int32 angle = parent->headRotation + parent->angle;

    self->position.x = parent->headPos.x - 0x2500 * RSDK.Sin512(angle);
    self->position.y = parent->headPos.y + 0x2500 * RSDK.Cos512(angle);

    if (!self->timer)
        RSDK.PlaySfx(SpiderMobile->sfxCharge, false, 255);

    if (++self->timer == 32) {
        self->timer      = 0;
        self->velocity.x = -0x200 * RSDK.Sin512(parent->headRotation + parent->angle);
        self->velocity.y = 0x200 * RSDK.Cos512(parent->headRotation + parent->angle);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 15, &self->partAnimator, false, 0);
        self->inkEffect = INK_ADD;
        self->state     = SpiderMobile_StateOrb_Fired;
        RSDK.PlaySfx(SpiderMobile->sfxElecBall, false, 255);
    }
}

void SpiderMobile_StateOrb_Fired(void)
{
    RSDK_THIS(SpiderMobile);

    RSDK.ProcessAnimation(&self->partAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SpiderMobile->hitboxOrb))
            Player_Hurt(player, self);
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

bool32 SpiderMobile_SfxCheck_Elevator(void)
{
    bool32 elevatorActive = false;
    foreach_active(SpiderMobile, boss)
    {
        if (boss->state == SpiderMobile_StateBody_MovePlatformToEnd)
            elevatorActive = true;
    }

    return elevatorActive;
}

#if GAME_INCLUDE_EDITOR
void SpiderMobile_EditorDraw(void)
{
    RSDK_THIS(SpiderMobile);

    // self->alpha             = 0xFF;
    self->visible           = false;
    self->drawFX            = FX_ROTATE | FX_FLIP;
    self->webCurveDirection = 1;
    self->active            = ACTIVE_BOUNDS;
    self->updateRange.x     = 0x400000;
    self->updateRange.y     = 0x400000;

    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &self->bumperAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &self->starAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->headAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &self->orbAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 4, &self->pincerAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &self->legsAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 6, &self->webAnimator, true, 0);
    RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 7, &self->eggmanAnimator, true, 0);

    SpiderMobile_HandleIdleArmMovement();
    SpiderMobile_UpdateLimbPositions();

    SpiderMobile_Draw_Body();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 0, 1 | 2 | 4 | 8, 0x00C0F0);

        // Draw overrides inkEffect which doesn't look great
        // self->inkEffect = INK_BLEND;
        // Vector2 pos     = self->position;
        // self->position  = self->origin;
        //
        // SpiderMobile_HandleIdleArmMovement();
        // SpiderMobile_UpdateLimbPositions();
        //
        // SpiderMobile_Draw_Body();
        //
        // self->inkEffect = INK_NONE;
        // self->position  = pos;

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->origin.x, self->origin.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void SpiderMobile_EditorLoad(void) { SpiderMobile->aniFrames = RSDK.LoadSpriteAnimation("FBZ/SpiderMobile.bin", SCOPE_STAGE); }
#endif

void SpiderMobile_Serialize(void) { RSDK_EDITABLE_VAR(SpiderMobile, VAR_VECTOR2, origin); }
