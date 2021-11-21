#include "SonicMania.h"

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
                self->alpha = 255;
                switch (voidToInt(data)) {
                    case 0:
                        self->visible       = false;
                        self->drawOrder     = Zone->drawOrderHigh;
                        self->drawFX        = FX_ROTATE | FX_FLIP;
                        self->field_A0      = 1;
                        self->health        = 6;
                        self->active        = ACTIVE_BOUNDS;
                        self->updateRange.x = 0x400000;
                        self->updateRange.y = 0x400000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &self->bumperAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &self->starAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->headAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &self->orbAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 4, &self->pincerAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &self->legsAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 6, &self->webAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 7, &self->eggmanAnimator, true, 0);
                        self->stateDraw = SpiderMobile_StateDraw_Body;
                        self->state     = SpiderMobile_StateBody_SetupBounds;
                        break;
                    case 1:
                        self->active        = ACTIVE_NORMAL;
                        self->updateRange.x = 0x300000;
                        self->updateRange.y = 0x300000;
                        self->visible       = true;
                        self->drawOrder     = Zone->drawOrderHigh;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 14, &self->partAnimator, true, 0);
                        self->state = SpiderMobile_StateOrb_Unknown1;
                        break;
                    case 2:
                        self->visible       = true;
                        self->drawOrder     = Zone->drawOrderHigh;
                        self->drawFX        = FX_ROTATE | FX_FLIP;
                        self->active        = ACTIVE_NORMAL;
                        self->updateRange.x = 0x800000;
                        self->updateRange.y = 0x800000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &self->bumperAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &self->starAnimator, true, 0);
                        self->velocity.x = RSDK.Rand(-0x20000, 0x20000);
                        self->velocity.y = -0x40000;
                        self->stateDraw  = SpiderMobile_StateDraw2_Unknown1;
                        self->state      = SpiderMobile_State2_Unknown1;
                        break;
                    case 3:
                        self->visible       = true;
                        self->drawOrder     = Zone->drawOrderLow;
                        self->active        = ACTIVE_NORMAL;
                        self->updateRange.x = 0x800000;
                        self->updateRange.y = 0x800000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 11, &self->partAnimator, true, 0);
                        self->velocity.x = 0x10000;
                        self->velocity.y = -0x20000;
                        self->state      = SpiderMobile_State3_Unknown1;
                        break;
                    default: return;
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

    SpiderMobile->hitbox3.left   = -8;
    SpiderMobile->hitbox3.top    = -8;
    SpiderMobile->hitbox3.right  = 8;
    SpiderMobile->hitbox3.bottom = 8;

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
    Soundboard_LoadSFX("FBZ/Elevator.wav", true, SpiderMobile_CheckCB, NULL);
}

void SpiderMobile_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);

    self->position.x += self->velocity.x;
    int32 offsetX = self->position.x - self->origin.x;
    self->velocity.y += 0x3000;
    self->position.y += self->velocity.y;

    int32 offsetY = (self->position.y - self->origin.y) >> 16;
    if (self->position.y > self->origin.y)
        self->angle = 2 * RSDK.ATan2(offsetY, offsetX / -0x11000);

    self->field_98 = offsetY * offsetY + (offsetX / 0x11000) * (offsetX / 0x11000);
    if (self->field_7C > 0)
        self->field_7C--;

    if (self->field_98 > 0x9000 && !self->field_7C) {
        if (self->field_78 == 2) {
            self->field_78 = 0;
            self->onGround = true;
            if (!self->angle)
                self->timer = 32;
            if (self->angle > 256)
                self->angle -= 512;

            self->velocity.x = 0;
            self->field_7C   = 0;
            self->field_9C   = 0x6000;
            self->field_8C   = self->angle << 15;
        }
        else {
            RSDK.PlaySfx(SpiderMobile->sfxHitGround, false, 255);
            Camera_ShakeScreen(0, 0, 3);
            ++self->field_78;

            int32 amp            = (abs(self->velocity.x) + abs(self->velocity.y)) >> 1;
            self->field_A0   = -self->field_A0;
            self->field_7C   = 8;
            self->velocity.x = (amp * RSDK.Sin512(self->angle)) >> 9;
            self->velocity.y = -(amp * RSDK.Cos512(self->angle)) >> 9;
        }
    }

    self->field_98 = self->field_A0 * (self->field_98 - 0x9000);
}

void SpiderMobile_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->field_7C <= 0) {
        if (self->field_8C <= 0) {
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
        self->field_7C--;
    }

    self->field_8C += self->velocity.x;
    self->field_98   = self->field_98 - (self->field_98 >> 2);
    self->angle      = self->field_8C >> 15;
    self->position.x = self->origin.x - self->field_9C * RSDK.Sin512(self->angle);
    self->position.y = self->origin.y + self->field_9C * RSDK.Cos512(self->angle);
}

void SpiderMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpiderMobile);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, self, &SpiderMobile->hitboxBumper)) {
            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

            int32 amp = (abs(player->velocity.x) + abs(player->velocity.y)) >> 9;
            if (!self->field_80 && !player->sidekick && (amp > 0x600 || self->state != SpiderMobile_StateBody_Unknown4)) {
                self->velocity.x = (3 * player->velocity.x) >> 2;
                self->velocity.y = minVal((3 * player->velocity.y) >> 2, -0x10000);
                self->field_80   = 4;
                self->onGround   = false;
            }

            amp                = maxVal((abs(player->velocity.x) + abs(player->velocity.y)) >> 9, 0x600);
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

        self->position.x = self->headPos.x - (RSDK.Sin512(self->field_90 + self->angle) << 13);
        self->position.y = self->headPos.y + (RSDK.Cos512(self->field_90 + self->angle) << 13);

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
        self->state               = SpiderMobile_StateBody_Destroyed;
        self->timer               = 0;
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK.GetEntityByID(SLOT_PLAYER1), 1000);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 10, &self->eggmanAnimator, true, 0);
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(SpiderMobile->sfxHit, false, 255);
        RSDK.PlaySfx(SpiderMobile->sfxFlailing, false, 255);
    }
}

void SpiderMobile_HandlePlatformMovement(void)
{
    RSDK_THIS(SpiderMobile);

    int32 offsetY = 0x7000000;
    if (Zone->cameraBoundsB[0] > 5508 || self->state == SpiderMobile_StateEggman_Unknown2)
        offsetY = self->field_88;

    Zone->cameraBoundsT[0] += offsetY >> 16;
    Zone->cameraBoundsB[0] += offsetY >> 16;
    Zone->deathBoundary[0] += offsetY;
    Zone->playerBoundsB[0] += offsetY;
    RSDK.GetSceneLayer(Zone->fgHigh)->scrollPos -= offsetY;
    self->position.y += offsetY;
    self->origin.y += offsetY;

    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->origin.x;
    self->position.y = self->origin.y;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->objectID == Player->objectID && player->state == Player_State_Transform) {
            Player_CheckCollisionPlatform(player, self, &SpiderMobile->hitboxPlatform);
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, &SpiderMobile->hitboxPlatform)) {
            player->collisionFlagV |= 1;
        }
        else if ((player->groundedStore || player->state == Player_State_None) && offsetY == self->field_88) {
            if (player->state == Player_State_None)
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
        if (player->state == Player_State_None) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgLow, player->position.x >> 20, player->position.y >> 20);
            if (tile == 0xFFFF || (tile & 0x3FF) == 669 || (tile & 0x3FF) == 379) {
                player->drawOrder = Zone->playerDrawLow;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                player->state      = Player_State_Air;
                player->velocity.y = -0x20000;
            }
        }
    }

    foreach_active(Ring, ring)
    {
        if (ring->state == Ring_State_Bounce) {
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

void SpiderMobile_Unknown7(void)
{
    RSDK_THIS(SpiderMobile);

    int32 angle = 16 * Zone->timer;
    if (self->field_84 >= 64) {
        self->legAngles[0] = (RSDK.Sin256(angle) >> 3) - (self->velocity.y >> 14) - 64;
        self->legAngles[6] = (RSDK.Sin256(angle + 224) >> 3) + (self->velocity.y >> 14) + 64;
    }
    else {
        self->legAngles[0] = ((self->legAngles[0] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle) >> 3) - (self->velocity.y >> 14) - 64)) >> 6);
        self->legAngles[6] = ((self->legAngles[6] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((self->velocity.y >> 14) + 64 + (RSDK.Sin256(angle + 224) >> 3))) >> 6);
        self->field_84 += 4;
    }

    if (self->legAngles[2] < 40)
        self->legAngles[2] += 2;

    if (self->legAngles[4] < 64)
        self->legAngles[4] += 2;

    if (self->legAngles[8] > -40)
        self->legAngles[8] -= 2;

    if (self->legAngles[10] > -64)
        self->legAngles[10] -= 2;

    for (int32 i = 0; i < 12; i += 2) {
        self->legAngles[i + 1] = self->legAngles[i];
    }
}

void SpiderMobile_Unknown8(void)
{
    RSDK_THIS(SpiderMobile);

    int32 angle = 16 * Zone->timer;
    if (self->field_84 >= 64) {
        self->legAngles[0]  = (RSDK.Sin256(angle) >> 3) - (self->velocity.y >> 14) - 64;
        self->legAngles[2]  = (RSDK.Sin256(angle + 64) >> 4) - (self->velocity.y >> 15) - 16;
        self->legAngles[4]  = (RSDK.Sin256(angle + 128) >> 3) - (self->velocity.y >> 14) + 48;
        self->legAngles[6]  = (RSDK.Sin256(angle + 224) >> 3) + (self->velocity.y >> 14) + 64;
        self->legAngles[8]  = (RSDK.Sin256(angle + 288) >> 4) + (self->velocity.y >> 15) + 16;
        self->legAngles[10] = (RSDK.Sin256(angle + 352) >> 3) + (self->velocity.y >> 14) - 48;
    }
    else {
        self->legAngles[0] = ((self->legAngles[0] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle) >> 3) - (self->velocity.y >> 14) - 64)) >> 6);
        self->legAngles[2] = ((self->legAngles[2] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 64) >> 4) - (self->velocity.y >> 15) - 16)) >> 6);
        self->legAngles[4] = ((self->legAngles[4] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 128) >> 3) - (self->velocity.y >> 14) + 48)) >> 6);
        self->legAngles[6] = ((self->legAngles[6] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 224) >> 3) + (self->velocity.y >> 14) + 64)) >> 6);
        self->legAngles[8] = ((self->legAngles[8] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 288) >> 4) + (self->velocity.y >> 15) + 16)) >> 6);
        self->legAngles[10] = ((self->legAngles[10] * (64 - self->field_84)) >> 6)
                               + ((self->field_84 * ((self->velocity.y >> 14) - 48 + (RSDK.Sin256(angle + 352) >> 3))) >> 6);
        self->field_84 += 4;
    }

    for (int32 i = 0; i < 12; i += 2) {
        self->legAngles[i + 1] = self->legAngles[i];
    }
}

void SpiderMobile_Unknown9(void)
{
    RSDK_THIS(SpiderMobile);

    int32 angle = 4 * Zone->timer;
    if (self->field_84 >= 64) {
        self->legAngles[0]  = (RSDK.Sin256(angle) >> 5) - (self->velocity.y >> 14) - 64;
        self->legAngles[2]  = (RSDK.Sin256(angle + 64) >> 5) - (self->velocity.y >> 15) - 16;
        self->legAngles[4]  = (RSDK.Sin256(angle + 128) >> 5) - (self->velocity.y >> 14) + 48;
        self->legAngles[6]  = (RSDK.Sin256(angle + 224) >> 5) + (self->velocity.y >> 14) + 64;
        self->legAngles[8]  = (RSDK.Sin256(angle + 288) >> 5) + (self->velocity.y >> 15) + 16;
        self->legAngles[10] = (RSDK.Sin256(angle + 352) >> 5) + (self->velocity.y >> 14) - 48;
    }
    else {
        self->legAngles[0] = ((self->legAngles[0] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle) >> 5) - (self->velocity.y >> 14) - 64)) >> 6);
        self->legAngles[2] = ((self->legAngles[2] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 64) >> 5) - (self->velocity.y >> 15) - 16)) >> 6);
        self->legAngles[4] = ((self->legAngles[4] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 128) >> 5) - (self->velocity.y >> 14) + 48)) >> 6);
        self->legAngles[6] = ((self->legAngles[6] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 224) >> 5) + (self->velocity.y >> 14) + 64)) >> 6);
        self->legAngles[8] = ((self->legAngles[8] * (64 - self->field_84)) >> 6)
                              + ((self->field_84 * ((RSDK.Sin256(angle + 288) >> 5) + (self->velocity.y >> 15) + 16)) >> 6);
        self->legAngles[10] = ((self->legAngles[10] * (64 - self->field_84)) >> 6)
                               + ((self->field_84 * ((self->velocity.y >> 14) - 48 + (RSDK.Sin256(angle + 352) >> 5))) >> 6);
        self->field_84 += 4;
    }

    for (int32 i = 0; i < 12; i += 2) {
        self->legAngles[i + 1] = self->legAngles[i];
    }
}

void SpiderMobile_Unknown10(void)
{
    RSDK_THIS(SpiderMobile);

    self->headPos.x = self->position.x;
    self->headPos.y = self->position.y;
    self->headPos.x -= RSDK.Sin512(self->angle) << 12;
    self->headPos.y += RSDK.Cos512(self->angle) << 12;

    self->legJointPos[0].x = (self->position.x - (RSDK.Sin512(self->angle) << 12)) + 0xC00 * RSDK.Cos512(self->angle);
    self->legJointPos[0].y = (self->position.y + (RSDK.Cos512(self->angle) << 12)) + 0xC00 * RSDK.Sin512(self->angle);

    self->legJointPos[1].x = (self->position.x - (RSDK.Sin512(self->angle) << 12)) - 0xC00 * RSDK.Cos512(self->angle);
    self->legJointPos[1].y = (self->position.y + (RSDK.Cos512(self->angle) << 12)) - 0xC00 * RSDK.Sin512(self->angle);

    int32 angle           = self->angle + self->field_90;
    self->pincer1StartPos.x = (self->headPos.x - 0x1A00 * RSDK.Sin512(angle)) - 0x900 * RSDK.Cos512(angle);
    self->pincer1StartPos.y = (self->headPos.y + 0x1A00 * RSDK.Cos512(angle)) - 0x900 * RSDK.Sin512(angle);

    self->pincer1EndPos.x = self->pincer1StartPos.x;
    self->pincer1EndPos.y = self->pincer1StartPos.y;
    self->pincer1EndPos.x -= 0x600 * RSDK.Sin512(angle + self->field_94);
    self->pincer1EndPos.y += 0x600 * RSDK.Cos512(angle + self->field_94);

    self->pincer2StartPos.x = (self->headPos.x - 0x1A00 * RSDK.Sin512(angle)) + 0x900 * RSDK.Cos512(angle);
    self->pincer2StartPos.y = (self->headPos.y + 0x1A00 * RSDK.Cos512(angle)) + 0x900 * RSDK.Sin512(angle);

    self->pincer2EndPos.x = self->pincer2StartPos.x;
    self->pincer2EndPos.y = self->pincer2StartPos.y;
    self->pincer2EndPos.x -= 0x600 * RSDK.Sin512(angle - self->field_94);
    self->pincer2EndPos.y += 0x600 * RSDK.Cos512(angle - self->field_94);

    for (int32 i = 0; i < 6; i += 2) {
        self->legPositions[i + 0].x = self->legJointPos[0].x;
        self->legPositions[i + 0].y = self->legJointPos[0].y;
        self->legPositions[i + 0].x += 0x1100 * RSDK.Cos512(self->angle + self->legAngles[i]);
        self->legPositions[i + 0].y += 0x1100 * RSDK.Sin512(self->angle + self->legAngles[i]);

        self->legPositions[i + 1].x = self->legPositions[i].x;
        self->legPositions[i + 1].y = self->legPositions[i].y;
        self->legPositions[i + 1].x += 0xF00 * RSDK.Cos512(self->angle + self->legAngles[i] + self->legAngles[i + 1]);
        self->legPositions[i + 1].y += 0xF00 * RSDK.Sin512(self->angle + self->legAngles[i] + self->legAngles[i + 1]);
    }

    for (int32 i = 6; i < 12; i += 2) {
        self->legPositions[i + 0].x = self->legJointPos[1].x;
        self->legPositions[i + 0].y = self->legJointPos[1].y;
        self->legPositions[i + 0].x -= 0x1100 * RSDK.Cos512(self->angle + self->legAngles[i]);
        self->legPositions[i + 0].y -= 0x1100 * RSDK.Sin512(self->angle + self->legAngles[i]);

        self->legPositions[i + 1].x = self->legPositions[i].x;
        self->legPositions[i + 1].y = self->legPositions[i].y;
        self->legPositions[i + 1].x -= 0xF00 * RSDK.Cos512(self->angle + self->legAngles[i] + self->legAngles[i + 1]);
        self->legPositions[i + 1].y -= 0xF00 * RSDK.Sin512(self->angle + self->legAngles[i] + self->legAngles[i + 1]);
    }
}

void SpiderMobile_StateDraw_Body(void)
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

    self->rotation = self->field_90 + self->angle;
    if (!(self->invincibilityTimer & 1)) {
        RSDK.DrawSprite(&self->headAnimator, &self->headPos, false);
    }
    else {
        RSDK.SetPaletteEntry(0, 1, 0xF0F0F0);
        RSDK.CopyPalette(1, 32, 0, 32, 10);

        RSDK.DrawSprite(&self->headAnimator, &self->headPos, false);
        RSDK.SetPaletteEntry(0, 1, 0);
        RSDK.CopyPalette(2, 32, 0, 32, 10);
    }
    RSDK.DrawSprite(&self->eggmanAnimator, &self->headPos, false);
    RSDK.DrawSprite(&self->electricAnimator, &self->headPos, false);

    self->rotation               = self->field_90 + self->angle + self->field_94;
    self->pincerAnimator.frameID = 0;
    self->direction              = FLIP_NONE;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer1StartPos, false);

    self->direction = FLIP_X;
    self->rotation  = self->field_90 + self->angle - self->field_94;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer2StartPos, false);

    self->pincerAnimator.frameID = 1;
    self->direction              = FLIP_NONE;
    self->rotation               = self->field_90 + (self->field_94 + (self->field_94 >> 1)) + self->angle;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer1EndPos, false);

    self->direction = FLIP_X;
    self->rotation  = self->field_90 + (self->angle - (self->field_94 >> 1)) - self->field_94;
    RSDK.DrawSprite(&self->pincerAnimator, &self->pincer2EndPos, false);

    self->direction           = FLIP_NONE;
    self->orbAnimator.frameID = 2;
    RSDK.DrawSprite(&self->orbAnimator, &self->pincer1StartPos, false);
    RSDK.DrawSprite(&self->orbAnimator, &self->pincer2StartPos, false);

    Vector2 drawPos;
    drawPos = self->position;
    drawPos.x += 0x1300 * RSDK.Sin512(self->angle);
    drawPos.y += -0x1300 * RSDK.Cos512(self->angle);
    self->webAnimator.frameID = 1;

    for (int32 i = 0; i < 16; ++i) {
        self->rotation = ((self->field_98 * RSDK.Sin256(32 * i)) >> 16) + self->angle;
        RSDK.DrawSprite(&self->webAnimator, &drawPos, false);

        drawPos.x += 0x700 * RSDK.Sin512(self->rotation);
        drawPos.y += -0x700 * RSDK.Cos512(self->rotation);
    }

    self->webAnimator.frameID = 0;
    self->rotation            = self->angle;
    RSDK.DrawSprite(&self->webAnimator, 0, false);

    if (self->field_80 > 0)
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

void SpiderMobile_StateDraw2_Unknown1(void)
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

void SpiderMobile_StateDraw_Eggman(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.DrawSprite(&self->headAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);
}

void SpiderMobile_StateBody_SetupBounds(void)
{
    RSDK_THIS(SpiderMobile);
    if (++self->timer >= 8) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (abs(player1->position.y - self->position.y) <= 0xA00000) {
            self->timer               = 0;
            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            Zone->cameraBoundsL[0]     = (self->position.x >> 16) - 212;
            Zone->cameraBoundsR[0]     = (self->position.x >> 16) + 212;
            Zone->cameraBoundsT[0]     = (self->position.y >> 16) - ScreenInfo->height;
            Zone->cameraBoundsB[0]     = (self->position.y >> 16);
            SpiderMobile->boundL        = (Zone->cameraBoundsL[0] + 64) << 16;
            SpiderMobile->boundR        = (Zone->cameraBoundsR[0] - 64) << 16;
            SpiderMobile->startX        = self->position.x;
            SpiderMobile->boundT        = (Zone->cameraBoundsT[0] + 48) << 16;
            SpiderMobile->boundB        = (Zone->cameraBoundsB[0] - 8) << 16;
            self->state               = SpiderMobile_StateBody_SetupArena;
            FBZSetup_GenericTriggerCB_ShowInterior();
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
            int32 offsetX   = self->origin.x - self->position.x;
            int32 offsetY   = self->origin.y - self->position.y;
            self->state = SpiderMobile_StateBody_Unknown1;

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
            self->field_88 = -0x10000;
            self->position.y += -0x800000 - (ScreenInfo->height << 16);
            self->field_84   = 64;
            self->active     = ACTIVE_NORMAL;
            BGSwitch->screenID = 0;
            FBZSetup_BGSwitchCB_ShowInside2();
            FBZSetup_BGSwitchCB_ShowInside1();

            TileLayer *overlay    = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("Exterior Overlay"));
            overlay->drawLayer[0] = 0;
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

void SpiderMobile_StateBody_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    if (self->onGround) {
        if (!self->groundedStore)
            RSDK.PlaySfx(SpiderMobile->sfxFall, false, 255);
        self->state = SpiderMobile_StateBody_Unknown2;
        Camera_ShakeScreen(0, 0, 4);
        SpiderMobile_Unknown9();
    }
    else {
        SpiderMobile_Unknown1();
        SpiderMobile_Unknown9();
    }
    SpiderMobile_Unknown10();
}

void SpiderMobile_StateBody_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->field_80 > 0)
        self->field_80--;

    if (self->onGround) {
        SpiderMobile_Unknown2();
        SpiderMobile_CheckPlayerCollisions();

        self->velocity.y -= self->velocity.y >> 3;
        if (self->field_9C > 0x4000)
            self->field_9C -= 64;
    }
    else {
        if (!self->field_7C)
            SpiderMobile_CheckPlayerCollisions();
        SpiderMobile_Unknown1();
    }

    if (self->field_94 > 0)
        self->field_94 -= 4;

    if (self->field_90 < 0) {
        self->field_90 += 4;
        if (self->field_90 > 0)
            self->field_90 = 0;
    }
    else if (self->field_90 > 0) {
        self->field_90 -= 4;
        if (self->field_90 < 0)
            self->field_90 = 0;
    }

    if (++self->timer == 120) {
        self->timer    = 0;
        self->field_84 = 0;
        self->state    = SpiderMobile_StateBody_Unknown3;
    }
    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown9();
    SpiderMobile_Unknown10();
    SpiderMobile_CheckSpikeHit();
}

void SpiderMobile_StateBody_Unknown3(void)
{
    RSDK_THIS(SpiderMobile);
    if (self->field_80 > 0)
        self->field_80--;

    if (self->field_94 > 0)
        self->field_94 -= 4;

    if (self->field_90 < 0) {
        self->field_90 += 4;
        if (self->field_90 > 0)
            self->field_90 = 0;
    }
    else if (self->field_90 > 0) {
        self->field_90 -= 4;
        if (self->field_90 < 0)
            self->field_90 = 0;
    }

    if (self->onGround) {
        SpiderMobile_Unknown2();
        SpiderMobile_CheckPlayerCollisions();

        self->velocity.y -= self->velocity.y >> 3;
        if (self->field_9C <= 0x2800) {
            self->field_84 = 0;
            self->state    = SpiderMobile_StateBody_Unknown4;
        }
        else {
            self->field_9C -= 128;
        }
    }
    else {
        SpiderMobile_Unknown1();
    }
    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown7();
    SpiderMobile_Unknown10();
    SpiderMobile_CheckSpikeHit();
}

void SpiderMobile_StateBody_Unknown4(void)
{
    RSDK_THIS(SpiderMobile);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;
    SpiderMobile_Unknown2();
    SpiderMobile_CheckPlayerCollisions();

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < self->headPos.x) {
        if (self->field_90 < 72)
            self->field_90 += 2;
    }
    else {
        if (self->field_90 > -72)
            self->field_90 -= 2;
    }

    if (self->field_94 >= 56) {
        RSDK.ProcessAnimation(&self->electricAnimator);
        if (!(self->timer % 60))
            CREATE_ENTITY(SpiderMobile, intToVoid(1), self->position.x, self->position.y)->parent = (Entity *)self;
    }
    else {
        self->field_94++;
        if (self->field_94 == 56)
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 12, &self->electricAnimator, false, 0);
    }

    if (++self->timer == 240) {
        self->timer    = 0;
        self->onGround = false;
        SpiderMobile_Unknown1();
        self->state = SpiderMobile_StateBody_Unknown2;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->electricAnimator, false, 0);
    }
    else if (!self->onGround) {
        self->state = SpiderMobile_StateBody_Unknown3;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->electricAnimator, false, 0);
    }

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown9();
    SpiderMobile_Unknown10();
}

void SpiderMobile_StateBody_Destroyed(void)
{
    RSDK_THIS(SpiderMobile);
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(SpiderMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int32 x = RSDK.Rand(-48, 48) << 16;
            int32 y = RSDK.Rand(-48, 48) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), self->headPos.x + x, self->headPos.y + y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }
    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown8();
    SpiderMobile_Unknown10();

    self->timer++;
    if (self->timer == 60) {
        EntityDebris *debris = NULL;

        for (int32 i = 0; i < 2; ++i) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legJointPos[i].x, self->legJointPos[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 0);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;
        }

        for (int32 i = 0; i < 6; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 1);
            debris->rotation   = self->angle + self->legAngles[i];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 0);
            debris->rotation   = self->angle + self->legAngles[i] + self->legAngles[i + 1];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;
        }

        for (int32 i = 6; i < 12; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 0);
            debris->rotation   = self->angle + self->legAngles[i];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 1);
            debris->rotation   = self->angle + self->legAngles[i] + self->legAngles[i + 1];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;
        }

        for (int32 i = 0; i < 12; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i].x, self->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 2);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->legPositions[i + 1].x, self->legPositions[i + 1].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 1);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
        }
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->legsAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->orbAnimator, true, 0);
    }
    else if (self->timer == 90) {
        CREATE_ENTITY(SpiderMobile, intToVoid(2), self->position.x, self->position.y)->rotation = self->angle;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->bumperAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->starAnimator, true, 0);
        self->position.x = self->headPos.x;
        self->position.y = self->headPos.y;
        self->position.x -= 0xC80 * RSDK.Sin512(self->angle + self->field_90);
        self->position.y += 0xC80 * RSDK.Cos512(self->angle + self->field_90);
        self->rotation   = self->angle + self->field_90;
        self->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        self->velocity.y = -0x40000;
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->headAnimator, true, 1);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &self->eggmanAnimator, true, 2);
        self->timer     = 0;
        self->inkEffect = INK_NONE;
        self->state     = SpiderMobile_StateEggman_Unknown1;
        self->stateDraw = SpiderMobile_StateDraw_Eggman;
    }
}

void SpiderMobile_StateEggman_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x;
    self->rotation += 8;
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(SpiderMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int32 x = RSDK.Rand(-48, 48) << 16;
            int32 y = RSDK.Rand(-48, 48) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), self->headPos.x + x, self->headPos.y + y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown10();

    if (++self->timer == 120) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->timer             = 0;
        self->visible           = false;
        self->state             = SpiderMobile_StateEggman_Unknown2;
        EntitySpiderMobile *child = CREATE_ENTITY(SpiderMobile, intToVoid(3), self->origin.x, self->origin.y + 0x1400000);
        child->parent             = (Entity *)self;
    }
}

void SpiderMobile_StateEggman_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);
    if (++self->timer == 30) {
        self->field_88 -= 0x10000;
        self->timer = 0;
    }

    foreach_active(Spikes, spikes)
    {
        if (spikes->stateMove < 5) {
            spikes->stateMove = 5;
            spikes->drawOrder = 1;
        }
    }
    SpiderMobile_HandlePlatformMovement();

    if (Zone->cameraBoundsB[0] <= 5248) {
        self->timer           = 0;
        Zone->cameraBoundsB[0] = 5248;
        Zone->cameraBoundsR[0] += 1024;
        Camera_ShakeScreen(0, 0, 6);
        self->visible = false;
        RSDK.PlaySfx(SpiderMobile->sfxHullClose, false, 255);
        self->state     = SpiderMobile_StateEggman_Unknown3;
        TileLayer *fgHigh = RSDK.GetSceneLayer(Zone->fgHigh);
        fgHigh->scrollPos = 0x9200000;
        self->origin.y  = 0x13600000;
    }
}

void SpiderMobile_StateEggman_Unknown3(void)
{
    RSDK_THIS(SpiderMobile);
    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->origin.x;
    self->position.y = self->origin.y;

    foreach_active(Player, player) { Player_CheckCollisionPlatform(player, self, &SpiderMobile->hitboxPlatform); }
    self->position.x = storeX;
    self->position.y = storeY;
}

void SpiderMobile_State2_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    self->velocity.y += 0x3800;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->rotation += 2;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void SpiderMobile_State3_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.ProcessAnimation(&self->partAnimator);
    self->velocity.y -= 0x1000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (++self->timer == 180)
        destroyEntity(self);
}

void SpiderMobile_StateOrb_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.ProcessAnimation(&self->partAnimator);
    EntitySpiderMobile *parent = (EntitySpiderMobile *)self->parent;
    int32 angle                  = parent->field_90 + parent->angle;

    self->position.x = parent->headPos.x - 0x2500 * RSDK.Sin512(angle);
    self->position.y = parent->headPos.y + 0x2500 * RSDK.Cos512(angle);
    if (!self->timer)
        RSDK.PlaySfx(SpiderMobile->sfxCharge, false, 255);
    if (++self->timer == 32) {
        self->timer      = 0;
        self->velocity.x = -0x200 * RSDK.Sin512(parent->field_90 + parent->angle);
        self->velocity.y = 0x200 * RSDK.Cos512(parent->field_90 + parent->angle);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 15, &self->partAnimator, false, 0);
        self->inkEffect = INK_ADD;
        self->state     = SpiderMobile_StateOrb_Unknown2;
        RSDK.PlaySfx(SpiderMobile->sfxElecBall, false, 255);
    }
}

void SpiderMobile_StateOrb_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.ProcessAnimation(&self->partAnimator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &SpiderMobile->hitbox3)) {
            Player_CheckHit(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

bool32 SpiderMobile_CheckCB(void)
{
    bool32 flag = false;
    foreach_active(SpiderMobile, boss)
    {
        if (boss->state == SpiderMobile_StateEggman_Unknown2)
            flag = true;
    }

    return flag;
}

#if RETRO_INCLUDE_EDITOR
void SpiderMobile_EditorDraw(void) {}

void SpiderMobile_EditorLoad(void) {}
#endif

void SpiderMobile_Serialize(void) { RSDK_EDITABLE_VAR(SpiderMobile, VAR_VECTOR2, origin); }
