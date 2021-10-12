#include "SonicMania.h"

ObjectSpiderMobile *SpiderMobile;

void SpiderMobile_Update(void)
{
    RSDK_THIS(SpiderMobile);
    StateMachine_Run(entity->state);
    entity->groundedStore = entity->onGround;
}

void SpiderMobile_LateUpdate(void) {}

void SpiderMobile_StaticUpdate(void) {}

void SpiderMobile_Draw(void)
{
    RSDK_THIS(SpiderMobile);
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->partAnimator, NULL, false);
    }
}

void SpiderMobile_Create(void *data)
{
    RSDK_THIS(SpiderMobile);
    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(entity);
    }
    else {
        entity->drawFX = FX_FLIP;
        if (!RSDK_sceneInfo->inEditor) {
            if (globals->gameMode >= MODE_TIMEATTACK) {
                destroyEntity(entity);
            }
            else {
                entity->alpha = 255;
                switch (voidToInt(data)) {
                    case 0:
                        entity->visible       = false;
                        entity->drawOrder     = Zone->drawOrderHigh;
                        entity->drawFX        = FX_ROTATE | FX_FLIP;
                        entity->field_A0      = 1;
                        entity->health        = 6;
                        entity->active        = ACTIVE_BOUNDS;
                        entity->updateRange.x = 0x400000;
                        entity->updateRange.y = 0x400000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &entity->bumperAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &entity->starAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &entity->headAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &entity->orbAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 4, &entity->pincerAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &entity->legsAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 6, &entity->webAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 7, &entity->eggmanAnimator, true, 0);
                        entity->stateDraw = SpiderMobile_StateDraw_Body;
                        entity->state     = SpiderMobile_StateBody_SetupBounds;
                        break;
                    case 1:
                        entity->active        = ACTIVE_NORMAL;
                        entity->updateRange.x = 0x300000;
                        entity->updateRange.y = 0x300000;
                        entity->visible       = true;
                        entity->drawOrder     = Zone->drawOrderHigh;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 14, &entity->partAnimator, true, 0);
                        entity->state = SpiderMobile_StateOrb_Unknown1;
                        break;
                    case 2:
                        entity->visible       = true;
                        entity->drawOrder     = Zone->drawOrderHigh;
                        entity->drawFX        = FX_ROTATE | FX_FLIP;
                        entity->active        = ACTIVE_NORMAL;
                        entity->updateRange.x = 0x800000;
                        entity->updateRange.y = 0x800000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 0, &entity->bumperAnimator, true, 0);
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 1, &entity->starAnimator, true, 0);
                        entity->velocity.x = RSDK.Rand(-0x20000, 0x20000);
                        entity->velocity.y = -0x40000;
                        entity->stateDraw  = SpiderMobile_StateDraw2_Unknown1;
                        entity->state      = SpiderMobile_State2_Unknown1;
                        break;
                    case 3:
                        entity->visible       = true;
                        entity->drawOrder     = Zone->drawOrderLow;
                        entity->active        = ACTIVE_NORMAL;
                        entity->updateRange.x = 0x800000;
                        entity->updateRange.y = 0x800000;
                        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 11, &entity->partAnimator, true, 0);
                        entity->velocity.x = 0x10000;
                        entity->velocity.y = -0x20000;
                        entity->state      = SpiderMobile_State3_Unknown1;
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

    SpiderMobile->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    SpiderMobile->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    SpiderMobile->sfxBumper    = RSDK.GetSFX("Stage/Bumper.wav");
    SpiderMobile->sfxCharge    = RSDK.GetSFX("FBZ/SpiderCharge.wav");
    SpiderMobile->sfxElecBall  = RSDK.GetSFX("FBZ/SpiderElecBall.wav");
    SpiderMobile->sfxFall      = RSDK.GetSFX("FBZ/SpiderFall.wav");
    SpiderMobile->sfxFlailing  = RSDK.GetSFX("FBZ/SpiderFlailing.wav");
    SpiderMobile->sfxHitGround = RSDK.GetSFX("FBZ/SpiderHitGround.wav");
    SpiderMobile->sfxRecovery  = RSDK.GetSFX("FBZ/SpiderRecovery.wav");
    SpiderMobile->sfxElevator  = RSDK.GetSFX("FBZ/Elevator.wav");
    SpiderMobile->sfxHullClose = RSDK.GetSFX("Stage/HullClose.wav");
    Soundboard_LoadSFX("FBZ/Elevator.wav", true, SpiderMobile_CheckCB, NULL);
}

void SpiderMobile_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);

    entity->position.x += entity->velocity.x;
    int offsetX = entity->position.x - entity->origin.x;
    entity->velocity.y += 0x3000;
    entity->position.y += entity->velocity.y;

    int offsetY = (entity->position.y - entity->origin.y) >> 16;
    if (entity->position.y > entity->origin.y)
        entity->angle = 2 * RSDK.ATan2(offsetY, offsetX / -0x11000);

    entity->field_98 = offsetY * offsetY + (offsetX / 0x11000) * (offsetX / 0x11000);
    if (entity->field_7C > 0)
        entity->field_7C--;

    if (entity->field_98 > 0x9000 && !entity->field_7C) {
        if (entity->field_78 == 2) {
            entity->field_78 = 0;
            entity->onGround = true;
            if (!entity->angle)
                entity->timer = 32;
            if (entity->angle > 256)
                entity->angle -= 512;

            entity->velocity.x = 0;
            entity->field_7C   = 0;
            entity->field_9C   = 0x6000;
            entity->field_8C   = entity->angle << 15;
        }
        else {
            RSDK.PlaySfx(SpiderMobile->sfxHitGround, false, 255);
            Camera_ShakeScreen(0, 0, 3);
            ++entity->field_78;

            int amp            = (abs(entity->velocity.x) + abs(entity->velocity.y)) >> 1;
            entity->field_A0   = -entity->field_A0;
            entity->field_7C   = 8;
            entity->velocity.x = (amp * RSDK.Sin512(entity->angle)) >> 9;
            entity->velocity.y = -(amp * RSDK.Cos512(entity->angle)) >> 9;
        }
    }

    entity->field_98 = entity->field_A0 * (entity->field_98 - 0x9000);
}

void SpiderMobile_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);

    if (entity->field_7C <= 0) {
        if (entity->field_8C <= 0) {
            if (entity->velocity.x < 0x20000)
                entity->velocity.x += 0x600;
        }
        else {
            if (entity->velocity.x > -0x20000)
                entity->velocity.x -= 0x600;
        }
    }
    else {
        entity->velocity.x += 0x200;
        entity->field_7C--;
    }

    entity->field_8C += entity->velocity.x;
    entity->field_98   = entity->field_98 - (entity->field_98 >> 2);
    entity->angle      = entity->field_8C >> 15;
    entity->position.x = entity->origin.x - entity->field_9C * RSDK.Sin512(entity->angle);
    entity->position.y = entity->origin.y + entity->field_9C * RSDK.Cos512(entity->angle);
}

void SpiderMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpiderMobile);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &SpiderMobile->hitboxBumper)) {
            int angle = RSDK.ATan2(player->position.x - entity->position.x, player->position.y - entity->position.y);

            int amp = (abs(player->velocity.x) + abs(player->velocity.y)) >> 9;
            if (!entity->field_80 && !player->sidekick && (amp > 0x600 || entity->state != SpiderMobile_StateBody_Unknown4)) {
                entity->velocity.x = (3 * player->velocity.x) >> 2;
                entity->velocity.y = minVal((3 * player->velocity.y) >> 2, -0x10000);
                entity->field_80   = 4;
                entity->onGround   = false;
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

    if (entity->invincibilityTimer <= 0) {
        int storeX = entity->position.x;
        int storeY = entity->position.y;

        entity->position.x = entity->headPos.x - (RSDK.Sin512(entity->field_90 + entity->angle) << 13);
        entity->position.y = entity->headPos.y + (RSDK.Cos512(entity->field_90 + entity->angle) << 13);

        foreach_active(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionTouchBox(spikes, &spikes->hitbox, entity, &SpiderMobile->hitboxSpikes))
                SpiderMobile_Hit();
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
    }
    else {
        entity->invincibilityTimer--;
    }
}

void SpiderMobile_Hit(void)
{
    RSDK_THIS(SpiderMobile);

    if (--entity->health <= 0) {
        entity->state               = SpiderMobile_StateBody_Destroyed;
        entity->timer               = 0;
        RSDK_sceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK.GetEntityByID(SLOT_PLAYER1), 1000);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 10, &entity->eggmanAnimator, true, 0);
    }
    else {
        entity->invincibilityTimer = 48;
        RSDK.PlaySfx(SpiderMobile->sfxHit, false, 255);
        RSDK.PlaySfx(SpiderMobile->sfxFlailing, false, 255);
    }
}

void SpiderMobile_HandlePlatformMovement(void)
{
    RSDK_THIS(SpiderMobile);

    int offsetY = 0x7000000;
    if (Zone->screenBoundsB1[0] > 5508 || entity->state == SpiderMobile_StateEggman_Unknown2)
        offsetY = entity->field_88;

    Zone->screenBoundsT1[0] += offsetY >> 16;
    Zone->screenBoundsB1[0] += offsetY >> 16;
    Zone->deathBoundary[0] += offsetY;
    Zone->screenBoundsB2[0] += offsetY;
    RSDK.GetSceneLayer(Zone->fgHigh)->scrollPos -= offsetY;
    entity->position.y += offsetY;
    entity->origin.y += offsetY;

    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->origin.x;
    entity->position.y = entity->origin.y;

    for (int p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);
        if (player->objectID == Player->objectID && player->state == Player_State_Transform) {
            Player_CheckCollisionPlatform(player, entity, &SpiderMobile->hitboxPlatform);
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, &SpiderMobile->hitboxPlatform)) {
            player->collisionFlagV |= 1;
        }
        else if ((player->groundedStore || player->state == Player_State_None) && offsetY == entity->field_88) {
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
            ushort tile = RSDK.GetTileInfo(Zone->fgLow, player->position.x >> 20, player->position.y >> 20);
            if (tile == -1 || (tile & 0x3FF) == 669 || (tile & 0x3FF) == 379) {
                player->drawOrder = Zone->playerDrawLow;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
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
                int velY = ring->velocity.y;
                if (RSDK.CheckObjectCollisionPlatform(entity, &SpiderMobile->hitboxPlatform, ring, &Ring->hitbox, true)) {
                    ring->velocity.y = (velY >> 2) - velY;
                    if (ring->velocity.y > -0x10000)
                        ring->velocity.y = -0x10000;
                }
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void SpiderMobile_Unknown7(void)
{
    RSDK_THIS(SpiderMobile);

    int angle = 16 * Zone->timer;
    if (entity->field_84 >= 64) {
        entity->legAngles[0] = (RSDK.Sin256(angle) >> 3) - (entity->velocity.y >> 14) - 64;
        entity->legAngles[6] = (RSDK.Sin256(angle + 224) >> 3) + (entity->velocity.y >> 14) + 64;
    }
    else {
        entity->legAngles[0] = ((entity->legAngles[0] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle) >> 3) - (entity->velocity.y >> 14) - 64)) >> 6);
        entity->legAngles[6] = ((entity->legAngles[6] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((entity->velocity.y >> 14) + 64 + (RSDK.Sin256(angle + 224) >> 3))) >> 6);
        entity->field_84 += 4;
    }

    if (entity->legAngles[2] < 40)
        entity->legAngles[2] += 2;

    if (entity->legAngles[4] < 64)
        entity->legAngles[4] += 2;

    if (entity->legAngles[8] > -40)
        entity->legAngles[8] -= 2;

    if (entity->legAngles[10] > -64)
        entity->legAngles[10] -= 2;

    for (int i = 0; i < 12; i += 2) {
        entity->legAngles[i + 1] = entity->legAngles[i];
    }
}

void SpiderMobile_Unknown8(void)
{
    RSDK_THIS(SpiderMobile);

    int angle = 16 * Zone->timer;
    if (entity->field_84 >= 64) {
        entity->legAngles[0]  = (RSDK.Sin256(angle) >> 3) - (entity->velocity.y >> 14) - 64;
        entity->legAngles[2]  = (RSDK.Sin256(angle + 64) >> 4) - (entity->velocity.y >> 15) - 16;
        entity->legAngles[4]  = (RSDK.Sin256(angle + 128) >> 3) - (entity->velocity.y >> 14) + 48;
        entity->legAngles[6]  = (RSDK.Sin256(angle + 224) >> 3) + (entity->velocity.y >> 14) + 64;
        entity->legAngles[8]  = (RSDK.Sin256(angle + 288) >> 4) + (entity->velocity.y >> 15) + 16;
        entity->legAngles[10] = (RSDK.Sin256(angle + 352) >> 3) + (entity->velocity.y >> 14) - 48;
    }
    else {
        entity->legAngles[0] = ((entity->legAngles[0] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle) >> 3) - (entity->velocity.y >> 14) - 64)) >> 6);
        entity->legAngles[2] = ((entity->legAngles[2] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 64) >> 4) - (entity->velocity.y >> 15) - 16)) >> 6);
        entity->legAngles[4] = ((entity->legAngles[4] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 128) >> 3) - (entity->velocity.y >> 14) + 48)) >> 6);
        entity->legAngles[6] = ((entity->legAngles[6] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 224) >> 3) + (entity->velocity.y >> 14) + 64)) >> 6);
        entity->legAngles[8] = ((entity->legAngles[8] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 288) >> 4) + (entity->velocity.y >> 15) + 16)) >> 6);
        entity->legAngles[10] = ((entity->legAngles[10] * (64 - entity->field_84)) >> 6)
                               + ((entity->field_84 * ((entity->velocity.y >> 14) - 48 + (RSDK.Sin256(angle + 352) >> 3))) >> 6);
        entity->field_84 += 4;
    }

    for (int i = 0; i < 12; i += 2) {
        entity->legAngles[i + 1] = entity->legAngles[i];
    }
}

void SpiderMobile_Unknown9(void)
{
    RSDK_THIS(SpiderMobile);

    int angle = 4 * Zone->timer;
    if (entity->field_84 >= 64) {
        entity->legAngles[0]  = (RSDK.Sin256(angle) >> 5) - (entity->velocity.y >> 14) - 64;
        entity->legAngles[2]  = (RSDK.Sin256(angle + 64) >> 5) - (entity->velocity.y >> 15) - 16;
        entity->legAngles[4]  = (RSDK.Sin256(angle + 128) >> 5) - (entity->velocity.y >> 14) + 48;
        entity->legAngles[6]  = (RSDK.Sin256(angle + 224) >> 5) + (entity->velocity.y >> 14) + 64;
        entity->legAngles[8]  = (RSDK.Sin256(angle + 288) >> 5) + (entity->velocity.y >> 15) + 16;
        entity->legAngles[10] = (RSDK.Sin256(angle + 352) >> 5) + (entity->velocity.y >> 14) - 48;
    }
    else {
        entity->legAngles[0] = ((entity->legAngles[0] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle) >> 5) - (entity->velocity.y >> 14) - 64)) >> 6);
        entity->legAngles[2] = ((entity->legAngles[2] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 64) >> 5) - (entity->velocity.y >> 15) - 16)) >> 6);
        entity->legAngles[4] = ((entity->legAngles[4] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 128) >> 5) - (entity->velocity.y >> 14) + 48)) >> 6);
        entity->legAngles[6] = ((entity->legAngles[6] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 224) >> 5) + (entity->velocity.y >> 14) + 64)) >> 6);
        entity->legAngles[8] = ((entity->legAngles[8] * (64 - entity->field_84)) >> 6)
                              + ((entity->field_84 * ((RSDK.Sin256(angle + 288) >> 5) + (entity->velocity.y >> 15) + 16)) >> 6);
        entity->legAngles[10] = ((entity->legAngles[10] * (64 - entity->field_84)) >> 6)
                               + ((entity->field_84 * ((entity->velocity.y >> 14) - 48 + (RSDK.Sin256(angle + 352) >> 5))) >> 6);
        entity->field_84 += 4;
    }

    for (int i = 0; i < 12; i += 2) {
        entity->legAngles[i + 1] = entity->legAngles[i];
    }
}

void SpiderMobile_Unknown10(void)
{
    RSDK_THIS(SpiderMobile);

    entity->headPos.x = entity->position.x;
    entity->headPos.y = entity->position.y;
    entity->headPos.x -= RSDK.Sin512(entity->angle) << 12;
    entity->headPos.y += RSDK.Cos512(entity->angle) << 12;

    entity->legJointPos[0].x = (entity->position.x - (RSDK.Sin512(entity->angle) << 12)) + 0xC00 * RSDK.Cos512(entity->angle);
    entity->legJointPos[0].y = (entity->position.y + (RSDK.Cos512(entity->angle) << 12)) + 0xC00 * RSDK.Sin512(entity->angle);

    entity->legJointPos[1].x = (entity->position.x - (RSDK.Sin512(entity->angle) << 12)) - 0xC00 * RSDK.Cos512(entity->angle);
    entity->legJointPos[1].y = (entity->position.y + (RSDK.Cos512(entity->angle) << 12)) - 0xC00 * RSDK.Sin512(entity->angle);

    int angle           = entity->angle + entity->field_90;
    entity->pincer1StartPos.x = (entity->headPos.x - 0x1A00 * RSDK.Sin512(angle)) - 0x900 * RSDK.Cos512(angle);
    entity->pincer1StartPos.y = (entity->headPos.y + 0x1A00 * RSDK.Cos512(angle)) - 0x900 * RSDK.Sin512(angle);

    entity->pincer1EndPos.x = entity->pincer1StartPos.x;
    entity->pincer1EndPos.y = entity->pincer1StartPos.y;
    entity->pincer1EndPos.x -= 0x600 * RSDK.Sin512(angle + entity->field_94);
    entity->pincer1EndPos.y += 0x600 * RSDK.Cos512(angle + entity->field_94);

    entity->pincer2StartPos.x = (entity->headPos.x - 0x1A00 * RSDK.Sin512(angle)) + 0x900 * RSDK.Cos512(angle);
    entity->pincer2StartPos.y = (entity->headPos.y + 0x1A00 * RSDK.Cos512(angle)) + 0x900 * RSDK.Sin512(angle);

    entity->pincer2EndPos.x = entity->pincer2StartPos.x;
    entity->pincer2EndPos.y = entity->pincer2StartPos.y;
    entity->pincer2EndPos.x -= 0x600 * RSDK.Sin512(angle - entity->field_94);
    entity->pincer2EndPos.y += 0x600 * RSDK.Cos512(angle - entity->field_94);

    for (int i = 0; i < 6; i += 2) {
        entity->legPositions[i + 0].x = entity->legJointPos[0].x;
        entity->legPositions[i + 0].y = entity->legJointPos[0].y;
        entity->legPositions[i + 0].x += 0x1100 * RSDK.Cos512(entity->angle + entity->legAngles[i]);
        entity->legPositions[i + 0].y += 0x1100 * RSDK.Sin512(entity->angle + entity->legAngles[i]);

        entity->legPositions[i + 1].x = entity->legPositions[i].x;
        entity->legPositions[i + 1].y = entity->legPositions[i].y;
        entity->legPositions[i + 1].x += 0xF00 * RSDK.Cos512(entity->angle + entity->legAngles[i] + entity->legAngles[i + 1]);
        entity->legPositions[i + 1].y += 0xF00 * RSDK.Sin512(entity->angle + entity->legAngles[i] + entity->legAngles[i + 1]);
    }

    for (int i = 6; i < 12; i += 2) {
        entity->legPositions[i + 0].x = entity->legJointPos[1].x;
        entity->legPositions[i + 0].y = entity->legJointPos[1].y;
        entity->legPositions[i + 0].x -= 0x1100 * RSDK.Cos512(entity->angle + entity->legAngles[i]);
        entity->legPositions[i + 0].y -= 0x1100 * RSDK.Sin512(entity->angle + entity->legAngles[i]);

        entity->legPositions[i + 1].x = entity->legPositions[i].x;
        entity->legPositions[i + 1].y = entity->legPositions[i].y;
        entity->legPositions[i + 1].x -= 0xF00 * RSDK.Cos512(entity->angle + entity->legAngles[i] + entity->legAngles[i + 1]);
        entity->legPositions[i + 1].y -= 0xF00 * RSDK.Sin512(entity->angle + entity->legAngles[i] + entity->legAngles[i + 1]);
    }
}

void SpiderMobile_StateDraw_Body(void)
{
    RSDK_THIS(SpiderMobile);

    entity->inkEffect = INK_NONE;
    for (int i = 0; i < 6; i += 2) {
        entity->legsAnimator.frameID = 1;
        entity->rotation             = entity->angle + entity->legAngles[i];
        RSDK.DrawSprite(&entity->legsAnimator, &entity->legPositions[i], false);

        entity->legsAnimator.frameID = 0;
        entity->rotation += entity->legAngles[i + 1];
        RSDK.DrawSprite(&entity->legsAnimator, &entity->legPositions[i], false);
    }

    for (int i = 6; i < 12; i += 2) {
        entity->legsAnimator.frameID = 0;
        entity->rotation             = entity->angle + entity->legAngles[i];
        RSDK.DrawSprite(&entity->legsAnimator, &entity->legPositions[i], false);

        entity->legsAnimator.frameID = 1;
        entity->rotation += entity->legAngles[i + 1];
        RSDK.DrawSprite(&entity->legsAnimator, &entity->legPositions[i], false);
    }

    for (int i = 0; i < 12; i += 2) {
        entity->orbAnimator.frameID = 2;
        RSDK.DrawSprite(&entity->orbAnimator, &entity->legPositions[i + 0], false);

        entity->orbAnimator.frameID = 1;
        RSDK.DrawSprite(&entity->orbAnimator, &entity->legPositions[i + 1], false);
    }

    entity->orbAnimator.frameID = 0;
    RSDK.DrawSprite(&entity->orbAnimator, &entity->legJointPos[0], false);
    RSDK.DrawSprite(&entity->orbAnimator, &entity->legJointPos[1], false);

    entity->rotation = entity->field_90 + entity->angle;
    if (!(entity->invincibilityTimer & 1)) {
        RSDK.DrawSprite(&entity->headAnimator, &entity->headPos, false);
    }
    else {
        RSDK.SetPaletteEntry(0, 1, 0xF0F0F0);
        RSDK.CopyPalette(1, 32, 0, 32, 10);

        RSDK.DrawSprite(&entity->headAnimator, &entity->headPos, false);
        RSDK.SetPaletteEntry(0, 1, 0);
        RSDK.CopyPalette(2, 32, 0, 32, 10);
    }
    RSDK.DrawSprite(&entity->eggmanAnimator, &entity->headPos, false);
    RSDK.DrawSprite(&entity->electricAnimator, &entity->headPos, false);

    entity->rotation               = entity->field_90 + entity->angle + entity->field_94;
    entity->pincerAnimator.frameID = 0;
    entity->direction              = FLIP_NONE;
    RSDK.DrawSprite(&entity->pincerAnimator, &entity->pincer1StartPos, false);

    entity->direction = FLIP_X;
    entity->rotation  = entity->field_90 + entity->angle - entity->field_94;
    RSDK.DrawSprite(&entity->pincerAnimator, &entity->pincer2StartPos, false);

    entity->pincerAnimator.frameID = 1;
    entity->direction              = FLIP_NONE;
    entity->rotation               = entity->field_90 + (entity->field_94 + (entity->field_94 >> 1)) + entity->angle;
    RSDK.DrawSprite(&entity->pincerAnimator, &entity->pincer1EndPos, false);

    entity->direction = FLIP_X;
    entity->rotation  = entity->field_90 + (entity->angle - (entity->field_94 >> 1)) - entity->field_94;
    RSDK.DrawSprite(&entity->pincerAnimator, &entity->pincer2EndPos, false);

    entity->direction           = FLIP_NONE;
    entity->orbAnimator.frameID = 2;
    RSDK.DrawSprite(&entity->orbAnimator, &entity->pincer1StartPos, false);
    RSDK.DrawSprite(&entity->orbAnimator, &entity->pincer2StartPos, false);

    Vector2 drawPos;
    drawPos = entity->position;
    drawPos.x += 0x1300 * RSDK.Sin512(entity->angle);
    drawPos.y += -0x1300 * RSDK.Cos512(entity->angle);
    entity->webAnimator.frameID = 1;

    for (int i = 0; i < 16; ++i) {
        entity->rotation = ((entity->field_98 * RSDK.Sin256(32 * i)) >> 16) + entity->angle;
        RSDK.DrawSprite(&entity->webAnimator, &drawPos, false);

        drawPos.x += 0x700 * RSDK.Sin512(entity->rotation);
        drawPos.y += -0x700 * RSDK.Cos512(entity->rotation);
    }

    entity->webAnimator.frameID = 0;
    entity->rotation            = entity->angle;
    RSDK.DrawSprite(&entity->webAnimator, 0, false);

    if (entity->field_80 > 0)
        RSDK.SetPaletteEntry(0, 18, 0xE0E040);
    entity->bumperAnimator.frameID = 0;
    RSDK.DrawSprite(&entity->bumperAnimator, NULL, false);
    RSDK.DrawSprite(&entity->starAnimator, NULL, false);

    entity->inkEffect              = INK_SUB;
    entity->bumperAnimator.frameID = 1;
    RSDK.DrawSprite(&entity->bumperAnimator, NULL, false);

    entity->inkEffect              = INK_ADD;
    entity->bumperAnimator.frameID = 2;
    RSDK.DrawSprite(&entity->bumperAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 18, 0xE00000);
}

void SpiderMobile_StateDraw2_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    if (!(Zone->timer & 1)) {
        entity->inkEffect              = INK_NONE;
        entity->bumperAnimator.frameID = 0;
        RSDK.DrawSprite(&entity->bumperAnimator, NULL, false);
        RSDK.DrawSprite(&entity->starAnimator, NULL, false);

        entity->inkEffect              = INK_SUB;
        entity->bumperAnimator.frameID = 1;
        RSDK.DrawSprite(&entity->bumperAnimator, NULL, false);

        entity->inkEffect              = INK_ADD;
        entity->bumperAnimator.frameID = 2;
        RSDK.DrawSprite(&entity->bumperAnimator, NULL, false);
    }
}

void SpiderMobile_StateDraw_Eggman(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.DrawSprite(&entity->headAnimator, NULL, false);
    RSDK.DrawSprite(&entity->eggmanAnimator, NULL, false);
}

void SpiderMobile_StateBody_SetupBounds(void)
{
    RSDK_THIS(SpiderMobile);
    if (++entity->timer >= 8) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (abs(player1->position.y - entity->position.y) <= 0xA00000) {
            entity->timer               = 0;
            Zone->playerBoundActiveL[0] = true;
            Zone->playerBoundActiveR[0] = true;
            Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - 212;
            Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 212;
            Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - RSDK_screens->height;
            Zone->screenBoundsB1[0]     = (entity->position.y >> 16);
            SpiderMobile->boundL        = (Zone->screenBoundsL1[0] + 64) << 16;
            SpiderMobile->boundR        = (Zone->screenBoundsR1[0] - 64) << 16;
            SpiderMobile->startX        = entity->position.x;
            SpiderMobile->boundT        = (Zone->screenBoundsT1[0] + 48) << 16;
            SpiderMobile->boundB        = (Zone->screenBoundsB1[0] - 8) << 16;
            entity->state               = SpiderMobile_StateBody_SetupArena;
            FBZSetup_GenericTriggerCB_B();
        }
        else {
            entity->timer = 8;
        }
    }
}

void SpiderMobile_StateBody_SetupArena(void)
{
    RSDK_THIS(SpiderMobile);

    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 120) {
            entity->timer   = 0;
            entity->visible = true;
            RSDK.PlaySfx(SpiderMobile->sfxRecovery, false, 255);
            int offsetX   = entity->origin.x - entity->position.x;
            int offsetY   = entity->origin.y - entity->position.y;
            entity->state = SpiderMobile_StateBody_Unknown1;

            entity->position.x = entity->origin.x;
            entity->position.y += offsetY;

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

            Zone->screenBoundsL1[0] += offsetX >> 16;
            Zone->screenBoundsR1[0] += offsetX >> 16;
            Zone->screenBoundsT1[0] += offsetY >> 16;
            Zone->screenBoundsB1[0] += offsetY >> 16;
            Zone->screenBoundsL2[0] += offsetX;
            Zone->screenBoundsR2[0] += offsetX;
            Zone->screenBoundsT2[0] += offsetY;
            Zone->screenBoundsB2[0] += offsetY;
            Zone->deathBoundary[0] = 0x7FFFFFFF;

            entity->origin.y -= 0x1200000;
            entity->field_88 = -0x10000;
            entity->position.y += -0x800000 - (RSDK_screens->height << 16);
            entity->field_84   = 64;
            entity->active     = ACTIVE_NORMAL;
            BGSwitch->screenID = 0;
            FBZSetup_BGSwitchCB_B();
            FBZSetup_BGSwitchCB_A();

            TileLayer *overlay    = RSDK.GetSceneLayer(RSDK.GetSceneLayerID("Exterior Overlay"));
            overlay->drawLayer[0] = 0;
        }
    }
    else {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > entity->position.x) {
            if (abs(player1->position.y - entity->position.y) < 0xA00000) {
                Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);

                foreach_active(LightBarrier, barrier) { barrier->enabled = true; }
                ++entity->timer;
            }
        }
    }
}

void SpiderMobile_StateBody_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    if (entity->onGround) {
        if (!entity->groundedStore)
            RSDK.PlaySfx(SpiderMobile->sfxFall, false, 255);
        entity->state = SpiderMobile_StateBody_Unknown2;
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

    if (entity->field_80 > 0)
        entity->field_80--;

    if (entity->onGround) {
        SpiderMobile_Unknown2();
        SpiderMobile_CheckPlayerCollisions();

        entity->velocity.y -= entity->velocity.y >> 3;
        if (entity->field_9C > 0x4000)
            entity->field_9C -= 64;
    }
    else {
        if (!entity->field_7C)
            SpiderMobile_CheckPlayerCollisions();
        SpiderMobile_Unknown1();
    }

    if (entity->field_94 > 0)
        entity->field_94 -= 4;

    if (entity->field_90 < 0) {
        entity->field_90 += 4;
        if (entity->field_90 > 0)
            entity->field_90 = 0;
    }
    else if (entity->field_90 > 0) {
        entity->field_90 -= 4;
        if (entity->field_90 < 0)
            entity->field_90 = 0;
    }

    if (++entity->timer == 120) {
        entity->timer    = 0;
        entity->field_84 = 0;
        entity->state    = SpiderMobile_StateBody_Unknown3;
    }
    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown9();
    SpiderMobile_Unknown10();
    SpiderMobile_CheckSpikeHit();
}

void SpiderMobile_StateBody_Unknown3(void)
{
    RSDK_THIS(SpiderMobile);
    if (entity->field_80 > 0)
        entity->field_80--;

    if (entity->field_94 > 0)
        entity->field_94 -= 4;

    if (entity->field_90 < 0) {
        entity->field_90 += 4;
        if (entity->field_90 > 0)
            entity->field_90 = 0;
    }
    else if (entity->field_90 > 0) {
        entity->field_90 -= 4;
        if (entity->field_90 < 0)
            entity->field_90 = 0;
    }

    if (entity->onGround) {
        SpiderMobile_Unknown2();
        SpiderMobile_CheckPlayerCollisions();

        entity->velocity.y -= entity->velocity.y >> 3;
        if (entity->field_9C <= 0x2800) {
            entity->field_84 = 0;
            entity->state    = SpiderMobile_StateBody_Unknown4;
        }
        else {
            entity->field_9C -= 128;
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

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
    SpiderMobile_Unknown2();
    SpiderMobile_CheckPlayerCollisions();

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x < entity->headPos.x) {
        if (entity->field_90 < 72)
            entity->field_90 += 2;
    }
    else {
        if (entity->field_90 > -72)
            entity->field_90 -= 2;
    }

    if (entity->field_94 >= 56) {
        RSDK.ProcessAnimation(&entity->electricAnimator);
        if (!(entity->timer % 60))
            CREATE_ENTITY(SpiderMobile, intToVoid(1), entity->position.x, entity->position.y)->parent = (Entity *)entity;
    }
    else {
        entity->field_94++;
        if (entity->field_94 == 56)
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 12, &entity->electricAnimator, false, 0);
    }

    if (++entity->timer == 240) {
        entity->timer    = 0;
        entity->onGround = false;
        SpiderMobile_Unknown1();
        entity->state = SpiderMobile_StateBody_Unknown2;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->electricAnimator, false, 0);
    }
    else if (!entity->onGround) {
        entity->state = SpiderMobile_StateBody_Unknown3;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->electricAnimator, false, 0);
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
            int x = RSDK.Rand(-48, 48) << 16;
            int y = RSDK.Rand(-48, 48) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), entity->headPos.x + x, entity->headPos.y + y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }
    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown8();
    SpiderMobile_Unknown10();

    entity->timer++;
    if (entity->timer == 60) {
        EntityDebris *debris = NULL;

        for (int i = 0; i < 2; ++i) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legJointPos[i].x, entity->legJointPos[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 0);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;
        }

        for (int i = 0; i < 6; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legPositions[i].x, entity->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 1);
            debris->rotation   = entity->angle + entity->legAngles[i];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legPositions[i].x, entity->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 0);
            debris->rotation   = entity->angle + entity->legAngles[i] + entity->legAngles[i + 1];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;
        }

        for (int i = 6; i < 12; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legPositions[i].x, entity->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 0);
            debris->rotation   = entity->angle + entity->legAngles[i];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legPositions[i].x, entity->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 5, &debris->animator, false, 1);
            debris->rotation   = entity->angle + entity->legAngles[i] + entity->legAngles[i + 1];
            debris->rotSpeed   = RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->drawFX     = FX_ROTATE | FX_FLIP;
        }

        for (int i = 0; i < 12; i += 2) {
            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legPositions[i].x, entity->legPositions[i].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 2);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;

            debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->legPositions[i + 1].x, entity->legPositions[i + 1].y);
            RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 3, &debris->animator, false, 1);
            debris->velocity.x = RSDK.Rand(-0x20000, 0x20000);
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity    = 0x3800;
            debris->drawOrder  = Zone->drawOrderHigh;
        }
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->legsAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->orbAnimator, true, 0);
    }
    else if (entity->timer == 90) {
        CREATE_ENTITY(SpiderMobile, intToVoid(2), entity->position.x, entity->position.y)->rotation = entity->angle;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->bumperAnimator, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->starAnimator, true, 0);
        entity->position.x = entity->headPos.x;
        entity->position.y = entity->headPos.y;
        entity->position.x -= 0xC80 * RSDK.Sin512(entity->angle + entity->field_90);
        entity->position.y += 0xC80 * RSDK.Cos512(entity->angle + entity->field_90);
        entity->rotation   = entity->angle + entity->field_90;
        entity->velocity.x = RSDK.Rand(-0x20000, 0x20000);
        entity->velocity.y = -0x40000;
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &entity->headAnimator, true, 1);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 2, &entity->eggmanAnimator, true, 2);
        entity->timer     = 0;
        entity->inkEffect = INK_NONE;
        entity->state     = SpiderMobile_StateEggman_Unknown1;
        entity->stateDraw = SpiderMobile_StateDraw_Eggman;
    }
}

void SpiderMobile_StateEggman_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    entity->velocity.y += 0x3800;
    entity->position.y += entity->velocity.y;
    entity->position.x += entity->velocity.x;
    entity->rotation += 8;
    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(SpiderMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = RSDK.Rand(-48, 48) << 16;
            int y = RSDK.Rand(-48, 48) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), entity->headPos.x + x, entity->headPos.y + y);
            explosion->drawOrder = Zone->drawOrderHigh;
        }
    }

    SpiderMobile_HandlePlatformMovement();
    SpiderMobile_Unknown10();

    if (++entity->timer == 120) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->timer             = 0;
        entity->visible           = false;
        entity->state             = SpiderMobile_StateEggman_Unknown2;
        EntitySpiderMobile *child = CREATE_ENTITY(SpiderMobile, intToVoid(3), entity->origin.x, entity->origin.y + 0x1400000);
        child->parent             = (Entity *)entity;
    }
}

void SpiderMobile_StateEggman_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);
    if (++entity->timer == 30) {
        entity->field_88 -= 0x10000;
        entity->timer = 0;
    }

    foreach_active(Spikes, spikes)
    {
        if (spikes->stateMove < 5) {
            spikes->stateMove = 5;
            spikes->drawOrder = 1;
        }
    }
    SpiderMobile_HandlePlatformMovement();

    if (Zone->screenBoundsB1[0] <= 5248) {
        entity->timer           = 0;
        Zone->screenBoundsB1[0] = 5248;
        Zone->screenBoundsR1[0] += 1024;
        Camera_ShakeScreen(0, 0, 6);
        entity->visible = false;
        RSDK.PlaySfx(SpiderMobile->sfxHullClose, false, 255);
        entity->state     = SpiderMobile_StateEggman_Unknown3;
        TileLayer *fgHigh = RSDK.GetSceneLayer(Zone->fgHigh);
        fgHigh->scrollPos = 0x9200000;
        entity->origin.y  = 0x13600000;
    }
}

void SpiderMobile_StateEggman_Unknown3(void)
{
    RSDK_THIS(SpiderMobile);
    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->origin.x;
    entity->position.y = entity->origin.y;

    foreach_active(Player, player) { Player_CheckCollisionPlatform(player, entity, &SpiderMobile->hitboxPlatform); }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void SpiderMobile_State2_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    entity->velocity.y += 0x3800;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->rotation += 2;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void SpiderMobile_State3_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.ProcessAnimation(&entity->partAnimator);
    entity->velocity.y -= 0x1000;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (++entity->timer == 180)
        destroyEntity(entity);
}

void SpiderMobile_StateOrb_Unknown1(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.ProcessAnimation(&entity->partAnimator);
    EntitySpiderMobile *parent = (EntitySpiderMobile *)entity->parent;
    int angle                  = parent->field_90 + parent->angle;

    entity->position.x = parent->headPos.x - 0x2500 * RSDK.Sin512(angle);
    entity->position.y = parent->headPos.y + 0x2500 * RSDK.Cos512(angle);
    if (!entity->timer)
        RSDK.PlaySfx(SpiderMobile->sfxCharge, false, 255);
    if (++entity->timer == 32) {
        entity->timer      = 0;
        entity->velocity.x = -0x200 * RSDK.Sin512(parent->field_90 + parent->angle);
        entity->velocity.y = 0x200 * RSDK.Cos512(parent->field_90 + parent->angle);
        RSDK.SetSpriteAnimation(SpiderMobile->aniFrames, 15, &entity->partAnimator, false, 0);
        entity->inkEffect = INK_ADD;
        entity->state     = SpiderMobile_StateOrb_Unknown2;
        RSDK.PlaySfx(SpiderMobile->sfxElecBall, false, 255);
    }
}

void SpiderMobile_StateOrb_Unknown2(void)
{
    RSDK_THIS(SpiderMobile);
    RSDK.ProcessAnimation(&entity->partAnimator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &SpiderMobile->hitbox3)) {
            Player_CheckHit(player, entity);
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
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

void SpiderMobile_EditorDraw(void) {}

void SpiderMobile_EditorLoad(void) {}

void SpiderMobile_Serialize(void) { RSDK_EDITABLE_VAR(SpiderMobile, VAR_VECTOR2, origin); }
