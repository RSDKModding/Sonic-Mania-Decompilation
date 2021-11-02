#include "SonicMania.h"

ObjectMeterDroid *MeterDroid;

void MeterDroid_Update(void)
{
    RSDK_THIS(MeterDroid);
    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;
    StateMachine_Run(entity->state);
}

void MeterDroid_LateUpdate(void) {}

void MeterDroid_StaticUpdate(void) {}

void MeterDroid_Draw(void)
{
    RSDK_THIS(MeterDroid);

    if (entity->field_6C > 0) {
        RSDK.SetLimitedFade(0, 1, 3, entity->field_6C, 32, 47);

        if (entity->stateDraw) {
            StateMachine_Run(entity->stateDraw);
        }
        else {
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }
        RSDK.CopyPalette(1, 32, 0, 32, 16);
    }
    else if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 32, 0, 32, 16);

        if (entity->stateDraw) {
            StateMachine_Run(entity->stateDraw);
            RSDK.CopyPalette(1, 32, 0, 32, 16);
        }
        else {
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }
    }
    else {
        if (entity->stateDraw) {
            StateMachine_Run(entity->stateDraw);
        }
        else {
            RSDK.DrawSprite(&entity->animator1, NULL, false);
        }
    }
}

void MeterDroid_Create(void *data)
{
    RSDK_THIS(MeterDroid);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            entity->field_70      = entity->position;
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->health        = 6;
            entity->drawOrder     = Zone->drawOrderLow;
            entity->state         = MeterDroid_State_Setup;
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 8, &entity->animator2, true, 0);
        }
    }
}

void MeterDroid_StageLoad(void)
{
    MeterDroid->aniFrames      = RSDK.LoadSpriteAnimation("OOZ/MeterDroid.bin", SCOPE_STAGE);
    MeterDroid->hitbox1.left   = -14;
    MeterDroid->hitbox1.top    = -14;
    MeterDroid->hitbox1.right  = 14;
    MeterDroid->hitbox1.bottom = 14;
    MeterDroid->hitbox2.left   = 14;
    MeterDroid->hitbox2.top    = -12;
    MeterDroid->hitbox2.right  = 32;
    MeterDroid->hitbox2.bottom = 12;
    MeterDroid->hitbox3.left   = -14;
    MeterDroid->hitbox3.top    = -14;
    MeterDroid->hitbox3.right  = 14;
    MeterDroid->hitbox3.bottom = 14;
    MeterDroid->sfxHit         = RSDK.GetSFX("Stage/BossHit.wav");
    MeterDroid->sfxExplosion   = RSDK.GetSFX("Stage/Explosion2.wav");
    MeterDroid->sfxDestroy     = RSDK.GetSFX("Global/Destroy.wav");
    MeterDroid->sfxToss        = RSDK.GetSFX("OOZ/Toss.wav");
    MeterDroid->sfxGrab        = RSDK.GetSFX("Global/Grab.wav");
    MeterDroid->sfxValve       = RSDK.GetSFX("OOZ/Valve.wav");
    MeterDroid->sfxWrench      = RSDK.GetSFX("OOZ/Wrench.wav");
}

void MeterDroid_CheckPlayerCollisions(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, entity, &MeterDroid->hitbox1) && Player_CheckBossHit(player, entity)) {
                MeterDroid_Hit();
            }
            if (!entity->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, entity, &MeterDroid->hitbox2)
#if RETRO_USE_PLUS
                    && !Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer)
#endif
                ) {
                    Player_CheckHit(player, entity);
                }
            }
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    entity->direction     = player1->position.x >= entity->position.x;
}

void MeterDroid_CheckPlayerCollisions2(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        int32 storeX = entity->position.x;
        int32 storeY = entity->position.y;
        if (!entity->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, entity, &MeterDroid->hitbox1) && Player_CheckBossHit(player, entity)) {
                MeterDroid_Hit();
            }
            if (!entity->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, entity, &MeterDroid->hitbox2)
#if RETRO_USE_PLUS
                    && !Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer)
#endif
                ) {
                    Player_CheckHit(player, entity);
                }
            }
        }
        entity->position.x = entity->field_88.x;
        entity->position.y = entity->field_88.y;
        if (Player_CheckCollisionTouch(player, entity, &MeterDroid->hitbox3)
#if RETRO_USE_PLUS
            && !Player_CheckMightyUnspin(0x600, player, false, &player->uncurlTimer)
#endif
        ) {
            Player_CheckHit(player, entity);
        }

        entity->position.x = storeX;
        entity->position.y = storeY;
    }
}

void MeterDroid_CheckPlayerCollisions3(void)
{
    RSDK_THIS(MeterDroid);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer) {
            if (Player_CheckBadnikTouch(player, entity, &MeterDroid->hitbox1) && Player_CheckBossHit(player, entity)) {
                MeterDroid_Hit();
            }
            if (!entity->invincibilityTimer) {
                if (Player_CheckCollisionTouch(player, entity, &MeterDroid->hitbox2)
#if RETRO_USE_PLUS
                    && !Player_CheckMightyUnspin(0x400, player, false, &player->uncurlTimer)
#endif
                ) {
                    Player_CheckHit(player, entity);
                }
            }
        }
    }
}

void MeterDroid_Hit(void)
{
    RSDK_THIS(MeterDroid);

    if (--entity->health <= 0) {
        entity->state = MeterDroid_State_Die;
        entity->timer = 0;
        if (entity->stateDraw == MeterDroid_StateDraw_Unknown2) {
            int32 x = RSDK.Rand(MeterDroid->hitbox1.left, MeterDroid->hitbox1.right) << 16;
            int32 y = RSDK.Rand(MeterDroid->hitbox1.top, MeterDroid->hitbox1.bottom) << 16;
            CREATE_ENTITY(Explosion, intToVoid(2), x + entity->field_88.x, y + entity->field_88.y)->drawOrder = Zone->drawOrderHigh;
            entity->stateDraw                                                                                 = MeterDroid_StateDraw_Unknown1;
        }
        RSDK_sceneInfo->timeEnabled = false;
        EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Player_GiveScore(player1, 1000);
    }
    else {
        entity->invincibilityTimer = 48;
        RSDK.PlaySfx(MeterDroid->sfxHit, false, 255);
    }
}

void MeterDroid_Unknown5(void)
{
    EntityValve *valves[5];

    RSDK_THIS(MeterDroid);

    valves[0] = NULL;
    int32 id   = 0;
    foreach_active(Valve, valve) { valves[id++ + 1] = valve; }

    valves[0]         = valves[RSDK.Rand(0, id) + 1];
    entity->valvePtr  = (Entity*)valves[0];
    entity->direction = valves[0]->direction ^ 1;
    entity->field_78  = valves[0]->position.x;
    entity->field_7C  = valves[0]->position.y;
    if (valves[0]->direction)
        entity->field_78 -= 0x2E0000;
    else
        entity->field_78 += 0x2E0000;
    entity->field_7C -= 0xC0000;

    entity->field_80   = (entity->field_78 - entity->position.x) >> 11;
    entity->field_84   = (entity->field_7C - entity->position.y) >> 11;
    entity->velocity.x = 0;
    entity->velocity.y = 0;
}

void MeterDroid_PopPlatforms(void)
{
    int32 delay = 30;
    foreach_active(GasPlatform, platform)
    {
        platform->type          = 2;
        platform->collapseDelay = delay;
        delay += 16;
        platform->state = GasPlatform_Unknown4;
    }
}

void MeterDroid_StateDraw_Unknown1(void)
{
    RSDK_THIS(MeterDroid);

    RSDK_sceneInfo->entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void MeterDroid_StateDraw_Unknown3(void)
{
    RSDK_THIS(MeterDroid);

    entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);
}

void MeterDroid_StateDraw_Unknown2(void)
{
    RSDK_THIS(MeterDroid);

    RSDK_sceneInfo->entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->drawFX = FX_NONE;
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    entity->drawFX = FX_FLIP;
    RSDK.DrawSprite(&entity->animator3, &entity->field_88, false);
}

void MeterDroid_State_Setup(void)
{
    RSDK_THIS(MeterDroid);
    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsT1[0]     = Zone->screenBoundsB1[0] - RSDK_screens->height - 64;
        MeterDroid->boundsL         = (Zone->screenBoundsL1[0] + 64) << 16;
        MeterDroid->boundsR         = (Zone->screenBoundsR1[0] - 64) << 16;
        MeterDroid->startX          = entity->position.x;
        MeterDroid->boundsT         = (Zone->screenBoundsT1[0] + 48) << 16;
        MeterDroid->boundsB         = (Zone->screenBoundsB1[0] - 8) << 16;
        entity->position.x += 0x800000;
        entity->visible   = true;
        entity->stateDraw = MeterDroid_StateDraw_Unknown1;
        entity->state     = MeterDroid_State_Unknown1;
    }
}

void MeterDroid_State_Unknown1(void)
{
    RSDK_THIS(MeterDroid);
    RSDK_sceneInfo->entity->angle = (entity->angle + 4) & 0xFF;
    entity->position.y            = ((RSDK.Sin256(entity->angle) << 10) + entity->field_70.y) & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 120) {
            entity->timer = 0;
            entity->state = MeterDroid_State_Unknown2;
        }
    }
    else {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > entity->position.x - 0x800000) {
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);
            ++entity->timer;
        }
    }
}

void MeterDroid_State_Unknown2(void)
{
    RSDK_THIS(MeterDroid);
    RSDK_sceneInfo->entity->angle = (entity->angle + 4) & 0xFF;
    entity->position.y            = ((RSDK.Sin256(entity->angle) << 10) + entity->field_70.y) & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (++entity->timer == 90) {
        entity->timer = 0;
        entity->state = MeterDroid_State_Unknown3;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown3(void)
{
    RSDK_THIS(MeterDroid);
    RSDK_sceneInfo->entity->angle = (entity->angle + 4) & 0xFF;
    entity->position.y            = ((RSDK.Sin256(entity->angle) << 10) + entity->field_70.y) & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (++entity->timer == 60) {
        entity->timer    = 0;
        entity->field_A0 = 0;
        entity->angle    = 64;
        if (entity->position.x < MeterDroid->startX) {
            entity->direction = FLIP_X;
            entity->state     = MeterDroid_State_Unknown5;
        }
        else {
            entity->direction = FLIP_NONE;
            entity->state     = MeterDroid_State_Unknown4;
        }
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown4(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.x += (entity->field_A0 * RSDK.Cos256(entity->angle)) >> 8;
    entity->position.y += (entity->field_A0 * RSDK.Sin256(entity->angle)) >> 8;
    if (entity->angle > 128) {
        entity->field_A0 -= 0x1400;
        if (entity->position.y < entity->field_70.y)
            entity->position.y = entity->field_70.y;
    }
    else {
        entity->field_A0 += 0x1400;
    }

    entity->angle++;
    if (entity->angle == 192) {
        entity->field_70.x = entity->position.x;
        entity->field_70.y = entity->position.y;
        entity->angle      = 0;
        entity->state      = MeterDroid_State_Unknown6;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown5(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.x += (entity->field_A0 * RSDK.Cos256(entity->angle)) >> 8;
    entity->position.y += (entity->field_A0 * RSDK.Sin256(entity->angle)) >> 8;
    if (entity->angle < 0) {
        entity->field_A0 -= 0x1400;
        if (entity->position.y < entity->field_70.y)
            entity->position.y = entity->field_70.y;
    }
    else {
        entity->field_A0 += 0x1400;
    }

    entity->angle--;
    if (entity->angle == -64) {
        entity->field_70.x = entity->position.x;
        entity->field_70.y = entity->position.y;
        entity->angle      = 0;
        entity->state      = MeterDroid_State_Unknown6;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown6(void)
{
    RSDK_THIS(MeterDroid);
    RSDK_sceneInfo->entity->angle = (entity->angle + 4) & 0xFF;
    entity->position.y            = ((RSDK.Sin256(entity->angle) << 10) + entity->field_70.y) & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    ++entity->timer;
    if (entity->timer >= 18) {
        if (entity->animator1.animationID == 2 && entity->animator1.frameID == entity->animator1.frameCount - 1)
            RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 0, &entity->animator1, true, 0);
    }
    else if (entity->timer == 16) {
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 2, &entity->animator1, true, 0);
    }
    if (entity->timer == 36) {
        entity->field_88.x = entity->position.x;
        entity->field_88.y = entity->position.y;
        if (entity->direction == FLIP_NONE)
            entity->field_88.x -= 0x300000;
        else
            entity->field_88.x += 0x300000;
        entity->field_88.y -= 0x100000;
        entity->field_78 = entity->field_88.x;
        entity->field_7C = entity->field_88.y;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        entity->field_90.x    = (player1->position.x + 16 * player1->velocity.x - entity->field_88.x) >> 5;
        entity->field_90.y    = (player1->position.y + 16 * player1->velocity.y - entity->field_88.y) >> 5;
        entity->field_98.x    = -(entity->field_90.x >> 6);
        entity->field_98.y    = -(entity->field_90.y >> 6);
        RSDK.PlaySfx(MeterDroid->sfxToss, false, 255);
        RSDK.PlaySfx(MeterDroid->sfxWrench, false, 255);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 9, &entity->animator3, true, 0);
        entity->stateDraw = MeterDroid_StateDraw_Unknown2;
    }

    if (entity->stateDraw == MeterDroid_StateDraw_Unknown2) {
        int32 x = entity->field_90.x + entity->field_98.x;
        int32 y = entity->field_90.y + entity->field_98.y;
        entity->field_88.x += x;
        entity->field_88.y += y;
        entity->field_90.x = x;
        entity->field_90.y = y;
        if (entity->timer == 60)
            entity->state = MeterDroid_State_Unknown7;
        MeterDroid_CheckPlayerCollisions2();
    }
    else {
        MeterDroid_CheckPlayerCollisions();
    }
}

void MeterDroid_State_Unknown7(void)
{
    RSDK_THIS(MeterDroid);
    RSDK_sceneInfo->entity->angle = (entity->angle + 4) & 0xFF;
    entity->position.y            = ((RSDK.Sin256(entity->angle) << 10) + entity->field_70.y) & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->field_90.x += entity->field_98.x;
    entity->field_90.y += entity->field_98.y;
    entity->field_88.x += entity->field_90.x;
    entity->field_88.y += entity->field_90.y;

    int32 rx = abs(entity->field_78 - entity->field_88.x) >> 16;
    int32 ry = abs(entity->field_7C - entity->field_88.y) >> 16;

    if (rx * rx + ry * ry < 512) {
        RSDK.PlaySfx(MeterDroid->sfxGrab, false, 255);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 3, &entity->animator1, true, 0);
        entity->stateDraw = MeterDroid_StateDraw_Unknown1;
        entity->state     = MeterDroid_State_Unknown8;
    }
    MeterDroid_CheckPlayerCollisions2();
}

void MeterDroid_State_Unknown8(void)
{
    RSDK_THIS(MeterDroid);
    RSDK_sceneInfo->entity->angle = (entity->angle + 4) & 0xFF;
    entity->position.y            = ((RSDK.Sin256(entity->angle) << 10) + entity->field_70.y) & 0xFFFF0000;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    MeterDroid_CheckPlayerCollisions();
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &entity->animator1, true, 0);
        MeterDroid_Unknown5();
        entity->state = MeterDroid_State_Unknown9;
    }
    MeterDroid_CheckPlayerCollisions();
}

void MeterDroid_State_Unknown9(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    if (abs(entity->velocity.x) < 0x40000) {
        if (abs(entity->velocity.x) < 0x40000)
            entity->velocity.x += entity->field_80;
    }
    entity->velocity.y += entity->field_84;
    entity->position.y += entity->velocity.y;
    entity->position.x += entity->velocity.x;

    int32 rx = abs(entity->field_78 - entity->position.x) >> 16;
    int32 ry = abs(entity->field_7C - entity->position.y) >> 16;

    if (rx * rx + ry * ry < 96) {
        entity->direction = entity->valvePtr->direction;
        entity->drawOrder = Zone->drawOrderLow - 1;
        entity->state     = MeterDroid_State_Unknown10;
    }
    MeterDroid_CheckPlayerCollisions3();
}

void MeterDroid_State_Unknown10(void)
{
    RSDK_THIS(MeterDroid);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->field_6C += 10;
    ++entity->timer;

    entity->velocity.x = entity->velocity.x - (entity->velocity.x >> 3);
    entity->velocity.y = entity->velocity.y - (entity->velocity.y >> 3);
    entity->position.x += ((entity->field_78 - entity->position.x) >> 3) + entity->velocity.x;
    entity->position.y += ((entity->field_7C - entity->position.y) >> 3) + entity->velocity.y;
    if (entity->timer == 24) {
        entity->position.x = entity->field_78;
        entity->position.y = entity->field_7C;
        entity->timer      = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 4, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 6, &entity->animator4, true, 0);
        EntityValve *valve = (EntityValve *)entity->valvePtr;
        RSDK.SetSpriteAnimation(Valve->aniFrames, 0, &valve->animator1, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 2, &valve->animator2, true, 0);
        RSDK.PlaySfx(MeterDroid->sfxValve, false, 255);
        entity->state     = MeterDroid_State_Unknown11;
        entity->stateDraw = MeterDroid_StateDraw_Unknown3;
    }
    MeterDroid_CheckPlayerCollisions3();
}

void MeterDroid_State_Unknown11(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator2);

    if (++entity->timer == 60)
        MeterDroid_PopPlatforms();
    if (entity->timer == 90) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 5, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 7, &entity->animator4, true, 0);
        EntityValve *valve = (EntityValve *)entity->valvePtr;
        RSDK.SetSpriteAnimation(Valve->aniFrames, 1, &valve->animator1, true, 0);
        RSDK.SetSpriteAnimation(Valve->aniFrames, 3, &valve->animator2, true, 0);
        entity->state = MeterDroid_State_Unknown12;
    }
}

void MeterDroid_State_Unknown12(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        entity->state = MeterDroid_State_Unknown13;
}

void MeterDroid_State_Unknown13(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.ProcessAnimation(&entity->animator2);

    ++entity->timer;
    if (entity->timer > 30) {
        if (entity->field_6C > 0) {
            entity->field_6C -= 16;
        }
    }
    if (entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &entity->animator1, true, 0);
        entity->field_70.x = entity->position.x;
        entity->field_70.y = entity->position.y;
        entity->drawOrder  = Zone->drawOrderLow;
        entity->state      = MeterDroid_State_Unknown3;
        entity->stateDraw  = MeterDroid_StateDraw_Unknown1;
    }
}

void MeterDroid_State_Die(void)
{
    RSDK_THIS(MeterDroid);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(MeterDroid->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + 1),
                                                       (RSDK.Rand(-208, 208) + RSDK_screens->centerX + RSDK_screens->position.x) << 16,
                                                       (RSDK.Rand(-112, 112) + RSDK_screens->centerY + RSDK_screens->position.y) << 16);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }

    ++entity->timer;
    if (entity->timer == 60) {
        MeterDroid->debrisSpeeds[2]  = entity->direction;
        MeterDroid->debrisSpeeds[6]  = entity->direction;
        MeterDroid->debrisSpeeds[10] = entity->direction;
        MeterDroid->debrisSpeeds[14] = entity->direction;
        if (entity->direction == FLIP_X) {
            MeterDroid->debrisSpeeds[3]  = -MeterDroid->debrisSpeeds[3];
            MeterDroid->debrisSpeeds[7]  = -MeterDroid->debrisSpeeds[7];
            MeterDroid->debrisSpeeds[11] = -MeterDroid->debrisSpeeds[11];
            MeterDroid->debrisSpeeds[15] = -MeterDroid->debrisSpeeds[15];
        }
        Debris_FallFlickerAnimSetup(MeterDroid->aniFrames, MeterDroid->debrisSpeeds, 10);
        entity->visible = false;
    }
    else if (entity->timer == 90) {
        entity->timer   = 0;
        entity->visible = false;
        Music_FadeOut(0.025);
        CREATE_ENTITY(OOZFlames, NULL, entity->position.x, entity->position.y);
        entity->state = MeterDroid_State_FinishAct;
    }
}

void MeterDroid_State_FinishAct(void)
{
    RSDK_THIS(MeterDroid);
    if (!(Zone->timer % 3)) {
        if (entity->timer < 180) {
            if (entity->timer <= 60)
                RSDK.PlaySfx(MeterDroid->sfxExplosion, Zone->timer % 3, 255);
            else
                RSDK.PlaySfx(MeterDroid->sfxDestroy, Zone->timer % 3, 255);
        }

        if (Zone->timer & 4) {
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(2 * (RSDK.Rand(0, 256) > 192) + 1),
                                                       (RSDK.Rand(-208, 208) + RSDK_screens->centerX + RSDK_screens->position.x) << 16,
                                                       (RSDK.Rand(-112, 112) + RSDK_screens->centerY + RSDK_screens->position.y) << 16);
            explosion->drawOrder       = Zone->drawOrderHigh;
        }
    }
    if (++entity->timer == 180) {
        for (int32 p = 0; p < Player->playerCount; ++p) {
            StarPost->postIDs[p] = 0;
        }

        SaveGame_SavePlayerState();
        globals->enableIntro = true;
        ++RSDK_sceneInfo->listPos;
        if (!RSDK.CheckValidScene())
            RSDK.SetScene("Presentation", "Title Screen");
        Zone_StartFadeOut(10, 0xF0F0F0);
    }
    if (entity->timer == 240)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void MeterDroid_EditorDraw(void)
{
    RSDK_THIS(MeterDroid);
    RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 1, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(MeterDroid->aniFrames, 8, &entity->animator2, false, 0);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void MeterDroid_EditorLoad(void) { MeterDroid->aniFrames = RSDK.LoadSpriteAnimation("OOZ/MeterDroid.bin", SCOPE_STAGE); }
#endif

void MeterDroid_Serialize(void) {}
