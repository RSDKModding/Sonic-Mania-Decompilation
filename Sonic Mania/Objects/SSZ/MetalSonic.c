#include "SonicMania.h"

ObjectMetalSonic *MetalSonic;

void MetalSonic_Update(void)
{
    RSDK_THIS(MetalSonic);
    if (entity->field_94)
        entity->field_94--;
    RSDK.ProcessAnimation(&entity->animator);
    RSDK.ProcessAnimation(&entity->animator2);
    StateMachine_Run(entity->state);
    if (MetalSonic->field_8 > 0)
        MetalSonic->field_8--;
#if RETRO_USE_PLUS
    foreach_active(StarPost, post) { post->starTimer = 0; }
#endif
}

void MetalSonic_LateUpdate(void) {}

void MetalSonic_StaticUpdate(void) {}

void MetalSonic_Draw(void)
{
    RSDK_THIS(MetalSonic);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->position.x < 0x8000000)
        drawPos.x += 0xE000000;
    else
        drawPos.x -= 0xE000000;

    if (entity->field_94 & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 8);
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator, NULL, false);
        RSDK.CopyPalette(1, 240, 0, 240, 8);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator, NULL, false);
    }
}

void MetalSonic_Create(void *data)
{
    RSDK_THIS(MetalSonic);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            Zone->autoScrollSpeed   = 0;
            entity->active          = ACTIVE_BOUNDS;
            entity->drawFX          = FX_FLIP | FX_ROTATE;
            entity->visible         = false;
            entity->updateRange.x   = 0x800000;
            entity->updateRange.y   = 0x800000;
            entity->tileCollisions  = true;
            entity->collisionLayers = Zone->fgLayers;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &entity->animator, true, 0);
            entity->drawOrder = Zone->drawOrderLow;
            entity->state     = MetalSonic_State_SetupArena;
        }
    }
}

void MetalSonic_StageLoad(void)
{
    MetalSonic->aniFrames       = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE);
    MetalSonic->hitbox1.left    = -8;
    MetalSonic->hitbox1.top     = -8;
    MetalSonic->hitbox1.right   = 8;
    MetalSonic->hitbox1.bottom  = 8;
    MetalSonic->hitbox2.left    = -16;
    MetalSonic->hitbox2.top     = -8;
    MetalSonic->hitbox2.right   = 16;
    MetalSonic->hitbox2.bottom  = 8;
    MetalSonic->sfxHit          = RSDK.GetSFX("Stage/BossHit.wav");
    MetalSonic->sfxExplosion2   = RSDK.GetSFX("Stage/Explosion2.wav");
    MetalSonic->sfxExplosion3   = RSDK.GetSFX("Stage/Explosion3.wav");
    MetalSonic->sfxRumble       = RSDK.GetSFX("Stage/Rumble.wav");
    MetalSonic->sfxJump2        = RSDK.GetSFX("Stage/Jump2.wav");
    MetalSonic->sfxSpecialRing  = RSDK.GetSFX("Global/SpecialRing.wav");
    MetalSonic->sfxMSElecPulse  = RSDK.GetSFX("SSZ2/MSElecPulse.wav");
    MetalSonic->sfxMSBall       = RSDK.GetSFX("SSZ2/MSBall.wav");
    MetalSonic->sfxMSFireball   = RSDK.GetSFX("SSZ2/MSFireball.wav");
    MetalSonic->sfxBeep3        = RSDK.GetSFX("Stage/Beep3.wav");
    MetalSonic->sfxBeep4        = RSDK.GetSFX("Stage/Beep4.wav");
    MetalSonic->sfxRockemSockem = RSDK.GetSFX("Stage/RockemSockem.wav");
    MetalSonic->sfxMSShoot      = RSDK.GetSFX("SSZ2/MSShoot.wav");
    MetalSonic->sfxMSChargeFire = RSDK.GetSFX("SSZ2/MSChargeFire.wav");
#if RETRO_USE_PLUS
    MetalSonic->sfxMSTransform = RSDK.GetSFX("SSZ2/MSTransform.wav");
    MetalSonic->sfxTransform2  = RSDK.GetSFX("Stage/Transform2.wav");
#endif
}

void MetalSonic_HandleStageWrap(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

#if !RETRO_USE_PLUS
    EntityMetalSonic *marker = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, MetalSonic);
    EntityPlatform *wall   = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);
    if (player->position.y < marker->position.y && !player->collisionPlane) {
        Zone->screenBoundsT1[0] = (marker->position.y >> 16) + 16 - RSDK_screens->height;
        Zone->screenBoundsB1[0] = (marker->position.y >> 16) + 16;
        Zone->deathBoundary[0]  = (marker->position.y >> 16) + 16;
        marker->position.y      = -0x200000;

        for (int i = 0; i < PLAYER_MAX; ++i) {
            Zone->screenBoundsL1[i]     = (wall->position.x >> 16) - 95;
            Zone->screenBoundsR1[i]     = (wall->position.x >> 16) + 392;
            Zone->playerBoundActiveL[i] = true;
            Zone->playerBoundActiveR[i] = true;
        }

        entity->timer       = 59;
        entity->targetPos.x = 0x9100000;
        entity->targetPos.y = 0x23400000;

        int anim = 0;
        if (entity->position.x >= 0x9100000) {
            if (entity->direction != FLIP_X) {
                entity->direction = FLIP_X;
                anim              = 4;
            }
            else {
                anim = 3;
            }
        }
        else if (entity->direction) {
            entity->direction = FLIP_NONE;
            anim              = 4;
        }
        else {
            anim = 3;
        }

        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, anim, &entity->animator, true, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &entity->animator2, false, 0);
        entity->state = MetalSonic_State_Unknown16;
    }
#endif

    if (camera->position.x < 0xF000000) {
        if (camera->position.x <= 0x1000000) {
            player->position.x += 0xE000000;
            entity->position.x += 0xE000000;
            camera->position.x += 0xE000000;
            camera->center.x += 0xE00;
            MetalSonic_ProcessBGParallax(-0xE0000);
#if RETRO_USE_PLUS
            if (entity->objectID == GigaMetal->objectID) {
                camera->boundsL += 0xE00;
                camera->boundsR += 0xE00;
                Zone->screenBoundsL1[0] += 0xE00;
                Zone->screenBoundsR1[0] += 0xE00;
                Zone->screenBoundsL2[0] += 0xE000000;
                Zone->screenBoundsR2[0] += 0xE000000;
            }
#endif

            foreach_active(MSOrb, orb) { orb->position.x += 0xE000000; }
#if RETRO_USE_PLUS
            foreach_active(MSBomb, bomb) { bomb->position.x += 0xE000000; }
#endif
            foreach_active(Ring, ring) { ring->position.x += 0xE000000; }
            foreach_active(Spring, spring) { spring->position.x += 0xE000000; }
            foreach_active(Spikes, spikes) { spikes->position.x += 0xE000000; }
            foreach_active(Firework, firework) { firework->position.x += 0xE000000; }
            foreach_active(EggPrison, prison) { prison->position.x += 0xE000000; }
            foreach_active(SSZSpotlight, spotlight)
            {
                spotlight->position.x += 0xE000000;
                spotlight->offsetPos.x += 0xE000000;
            }
            foreach_active(ImageTrail, trail)
            {
                trail->position.x += 0xE000000;
                trail->currentPos.x += 0xE000000;
                for (int32 i = 0; i < ImageTrail_TrackCount; ++i) trail->statePos[i].x += 0xE000000;
            }
            foreach_active(Platform, platform)
            {
                platform->position.x += 0xE000000;
                platform->centerPos.x += 0xE000000;
                platform->drawPos.x += 0xE000000;
            }

            for (int32 i = 1; i < Player->playerCount; ++i) {
                RSDK_GET_ENTITY(i, Player)->position.x += 0xE000000;
            }
        }
    }
    else {
        player->position.x -= 0xE000000;
        entity->position.x -= 0xE000000;
        camera->position.x -= 0xE000000;
        camera->center.x -= 0xE00;
        MetalSonic_ProcessBGParallax(0xE0000);
#if RETRO_USE_PLUS
        if (entity->objectID == GigaMetal->objectID) {
            camera->boundsL -= 0xE00;
            camera->boundsR -= 0xE00;
            Zone->screenBoundsL1[0] -= 0xE00;
            Zone->screenBoundsR1[0] -= 0xE00;
            Zone->screenBoundsL2[0] -= 0xE000000;
            Zone->screenBoundsR2[0] -= 0xE000000;
        }
#endif

        foreach_active(MSOrb, orb) { orb->position.x -= 0xE000000; }
#if RETRO_USE_PLUS
        foreach_active(MSBomb, bomb) { bomb->position.x -= 0xE000000; }
#endif
        foreach_active(Ring, ring) { ring->position.x -= 0xE000000; }
        foreach_active(Spring, spring) { spring->position.x -= 0xE000000; }
        foreach_active(Spikes, spikes) { spikes->position.x -= 0xE000000; }
        foreach_active(Firework, firework) { firework->position.x -= 0xE000000; }
        foreach_active(EggPrison, prison) { prison->position.x -= 0xE000000; }
        foreach_active(SSZSpotlight, spotlight)
        {
            spotlight->position.x -= 0xE000000;
            spotlight->offsetPos.x -= 0xE000000;
        }
        foreach_active(ImageTrail, trail)
        {
            trail->position.x -= 0xE000000;
            trail->currentPos.x -= 0xE000000;
            for (int32 i = 0; i < ImageTrail_TrackCount; ++i) trail->statePos[i].x -= 0xE000000;
        }
        foreach_active(Platform, platform)
        {
            platform->position.x -= 0xE000000;
            platform->centerPos.x -= 0xE000000;
            platform->drawPos.x -= 0xE000000;
        }

        for (int32 i = 1; i < Player->playerCount; ++i) {
            RSDK_GET_ENTITY(i, Player)->position.x -= 0xE000000;
        }
    }
}

void MetalSonic_ProcessBGParallax(int32 mult)
{
    for (int32 i = 0; i < 2; ++i) {
        TileLayer *bg = RSDK.GetSceneLayer(i);
        for (int32 s = 0; s < bg->scrollInfoCount; ++s) {
            bg->scrollInfo[s].scrollPos += mult * bg->scrollInfo[s].parallaxFactor;
        }
    }

    TileLayer *bg3 = RSDK.GetSceneLayer(2);
    bg3->scrollPos += mult * bg3->parallaxFactor;

    foreach_active(EggTower, tower) { tower->rotationX += (mult >> 8); }
}

void MetalSonic_HandleVelocity(void)
{
    RSDK_THIS(MetalSonic);

    int angle = RSDK.ATan2((entity->targetPos.x - entity->position.x) >> 16, (entity->targetPos.y - entity->position.y) >> 16);
    int ang   = 2 * angle - entity->angle;

    if (abs(ang) >= abs(ang - 0x200)) {
        if (abs(ang - 0x200) < abs(ang + 0x200))
            angle = ang - 0x200;
        else
            angle = ang + 0x200;
    }
    else {
        if (abs(ang) < abs(ang + 0x200))
            angle = ang;
        else
            angle = ang + 0x200;
    }
    entity->angle = (angle / 18 + entity->angle) & 0x1FF;

    entity->velocity.x = RSDK.Cos512(entity->angle) << 9;
    entity->velocity.y = RSDK.Sin512(entity->angle) << 9;
}

void MetalSonic_HandleAnimDir(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (entity->animator.animationID == 4) {
        if (entity->animator.frameID == entity->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, true, 0);
    }
    else if (entity->position.x <= player1->position.x) {
        if (entity->direction == FLIP_X) {
            entity->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
        }
    }
    else if (entity->direction == FLIP_NONE) {
        entity->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }
}

#if !RETRO_USE_PLUS
void MetalSonic_CheckPlayerCollisions(void)
{
    RSDK_THIS(MetalSonic);

    foreach_active(Player, player)
    {
        if (!entity->field_94) {
            Hitbox *hitbox = entity->animator.animationID == 5 ? &MetalSonic->hitbox2 : &MetalSonic->hitbox1;
            if (Player_CheckBadnikTouch(player, entity, hitbox) && Player_CheckBossHit(player, entity)) {
                if (player->velocity.x < 0)
                    player->velocity.x >>= 2;

                player->velocity.x >>= 1;
                player->velocity.y >>= 1;
                player->velocity.x += entity->velocity.x;
                player->velocity.y += entity->velocity.y;
                player->groundVel = player->velocity.x;
                MetalSonic_Hit();
            }
        }
    }
}

void MetalSonic_Hit(void)
{
    RSDK_THIS(MetalSonic);

    if (!--entity->health) {
        entity->timer               = 180;
        RSDK_sceneInfo->timeEnabled = false;
        entity->velocity.y          = -0x1800;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 10, &entity->animator, false, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 13, &entity->animator2, false, 0);
        entity->drawFX |= FX_ROTATE;
        entity->state = MetalSonic_State_Explode;
    }
    else {
        entity->field_94 = 48;
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 0xFF);
    }
}

void MetalSonic_Explode(void)
{
    RSDK_THIS(MetalSonic);
    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 0xFF);
        if (!(Zone->timer & 0xF)) {
            int x = entity->position.x + (RSDK.Rand(-19, 20) << 16);
            int y = entity->position.y + (RSDK.Rand(-24, 25) << 16);

            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
        }
    }
}
#endif

void MetalSonic_State_SetupArena(void)
{
    RSDK_THIS(MetalSonic);

    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - RSDK_screens->height + 52;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 52;
        entity->state               = MetalSonic_State_WaitForPlayer;
    }
}

void MetalSonic_State_WaitForPlayer(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > entity->position.x) {
        Music_FadeOut(0.0125);
        entity->state = MetalSonic_State_WaitForHologram;
    }
}

void MetalSonic_State_WaitForHologram(void)
{
    RSDK_THIS(MetalSonic);

    foreach_active(MSHologram, hologram)
    {
        if (hologram->state == MSHologram_State_Destroyed && hologram->timer == 320) {
            Music_ClearMusicStack();
            Music_PlayTrack(TRACK_EGGMAN1);
            RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);
            entity->position.y += 0x600000;
            entity->velocity.y = -0x80000;
            entity->visible    = true;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 9, &entity->animator, false, 6);
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 13, &entity->animator2, false, 0);
            entity->direction               = FLIP_X;
            entity->animator.animationSpeed = 0;
            entity->active                  = ACTIVE_NORMAL;
            entity->state                   = MetalSonic_State_Appear;
            Camera_ShakeScreen(6, 0, 6);
            foreach_break;
        }
    }
}

void MetalSonic_State_Appear(void)
{
    RSDK_THIS(MetalSonic);

    entity->velocity.y += 0x2000;
    entity->position.y += entity->velocity.y;

    if (entity->velocity.y >= 0) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, false, 3);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, false, 0);
        entity->velocity.x = 0x18000;
        entity->onGround   = false;
        entity->state      = MetalSonic_State_Land;
    }
}

void MetalSonic_State_Land(void)
{
    RSDK_THIS(MetalSonic);

    entity->velocity.y += 0x2000;
    entity->outerBox = RSDK.GetHitbox(&entity->animator, 0);
    entity->innerBox = RSDK.GetHitbox(&entity->animator, 1);

    RSDK.ProcessTileCollisions(entity, entity->outerBox, entity->innerBox);
    if (entity->onGround) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 1, &entity->animator, false, 0);
        entity->state                   = MetalSonic_State_Taunt;
        entity->animator.animationSpeed = 0;
        entity->direction               = FLIP_NONE;
    }
}

void MetalSonic_State_Taunt(void)
{
    RSDK_THIS(MetalSonic);

    if (++entity->timer == 60) {
        entity->timer                   = 0;
        entity->animator.animationSpeed = 1;
        entity->state                   = MetalSonic_State_GetReady;
    }
}

void MetalSonic_State_GetReady(void)
{
    RSDK_THIS(MetalSonic);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 2, &entity->animator, false, 0);
        entity->state = MetalSonic_State_SetupBounds;
    }
}

void MetalSonic_State_SetupBounds(void)
{
    RSDK_THIS(MetalSonic);

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &entity->animator2, false, 0);
        entity->state = MetalSonic_State_Start;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
        Zone->screenBoundsR1[0] = size.x;
        Zone->screenBoundsT1[0] = 0;
    }
}

void MetalSonic_State_Start(void)
{
    RSDK_THIS(MetalSonic);

    if (++entity->timer == 90) {
        entity->timer           = 0;
        Zone->screenBoundsL1[0] = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 8, &entity->animator, false, 0);
        entity->velocity.x = 0x18000;
        entity->velocity.y = -0x60000;
        RSDK.PlaySfx(MetalSonic->sfxJump2, false, 255);
        entity->state = MetalSonic_State_Unknown1;
    }
}

void MetalSonic_State_Unknown1(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.y >= 0) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &entity->animator2, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, false, 6);
        entity->animator.animationSpeed = 0;
        entity->targetPos.x             = player1->position.x;
        entity->targetPos.y             = player1->position.y;
        entity->velocity.x              = 0;
        entity->velocity.y              = 0;
        entity->timer2                  = 240;
        entity->state                   = MetalSonic_State_Unknown2;
    }
}

void MetalSonic_State_Unknown2(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (--entity->timer <= 0) {
        entity->timer = 60;
        int angle     = RSDK.Rand(0, 256);
        int power     = RSDK.Rand(64, 97) << 8;

        entity->targetPos.x = (power + (power >> 2)) * RSDK.Cos256(angle);
        entity->targetPos.y = power * RSDK.Sin256(angle);
    }

    entity->targetPos.x += player1->position.x;
    entity->targetPos.y += player1->position.y;
    MetalSonic_HandleVelocity();
    MetalSonic_HandleAnimDir();

    if (!entity->activeScreens) {
        if (entity->velocity.x <= 0) {
            if (player1->velocity.x < entity->field_80)
                entity->field_80 = player1->velocity.x;

            int dist = (entity->targetPos.x - entity->position.x) >> 5;
            if (dist < entity->field_80)
                entity->field_80 = dist;

            if (entity->position.x < player1->position.x + 244) {
                if (player1->velocity.x < 0) {
                    if (entity->field_80 < 2 * player1->velocity.x) {
                        entity->field_78 = 2 * player1->velocity.x;
                        entity->field_80 = 2 * player1->velocity.x;
                    }
                }
            }
        }
        else {
            if (player1->velocity.x > entity->field_80)
                entity->field_80 = player1->velocity.x;

            int dist = (entity->targetPos.x - entity->position.x) >> 5;
            if (dist > entity->field_80)
                entity->field_80 = dist;

            if (entity->position.x > player1->position.x - 244) {
                if (player1->velocity.x > 0) {
                    if (entity->field_80 > 2 * player1->velocity.x) {
                        entity->field_78 = 2 * player1->velocity.x;
                        entity->field_80 = 2 * player1->velocity.x;
                    }
                }
            }
        }

        if (entity->velocity.y <= 0) {
            if (player1->velocity.y < entity->field_84)
                entity->field_84 = player1->velocity.y;

            int dist = (entity->targetPos.y - entity->position.y) >> 5;
            if (dist < entity->field_84)
                entity->field_84 = dist;

            if (entity->position.y < player1->position.y + 152) {
                if (player1->velocity.y < 0) {
                    if (entity->field_84 < 2 * player1->velocity.y) {
                        entity->field_7C = 2 * player1->velocity.y;
                        entity->field_84 = 2 * player1->velocity.y;
                    }
                }
            }
        }
        else {
            if (player1->velocity.y > entity->field_84)
                entity->field_84 = player1->velocity.y;

            int dist = (entity->targetPos.y - entity->position.y) >> 5;
            if (dist > entity->field_84)
                entity->field_84 = dist;

            if (entity->position.y > player1->position.y - 152) {
                if (player1->velocity.y > 0) {
                    if (entity->field_84 > 2 * player1->velocity.y) {
                        entity->field_7C = 2 * player1->velocity.y;
                        entity->field_84 = 2 * player1->velocity.y;
                    }
                }
            }
        }

        entity->timer2--;
        if (entity->timer2 < 4)
            entity->timer2 = 4;
    }
    else {
        entity->field_80 = player1->velocity.x;
        entity->field_84 = player1->velocity.y;
        entity->timer2--;
    }

    if (entity->timer2 <= 0) {
        entity->attackType = RSDK.Rand(0, 6);

        while ((1 << entity->attackType) & MetalSonic->field_C) {
            entity->attackType = RSDK.Rand(0, 6);
        }

        MetalSonic->field_C |= 1 << entity->attackType;
        if (MetalSonic->field_C == 0x3F)
            MetalSonic->field_C = 0;

        entity->attackType >>= 1;

        switch (entity->attackType) {
            case 0:
                if (RSDK.Rand(0, 2) != 0)
                    entity->targetPos.x = (RSDK_screens->width + 72) << 16;
                else
                    entity->targetPos.x = -0x480000;
                entity->targetPos.y = RSDK_screens->centerY << 16;
                entity->state       = MetalSonic_State_Unknown3;
                break;
            case 1:
                if (player1->velocity.x >= 0) {
                    entity->targetPos.y = 0x200000;
                    entity->targetPos.x = (RSDK_screens->width - 32) << 16;
                }
                else {
                    entity->targetPos.x = 0x200000;
                    entity->targetPos.y = 0x200000;
                }
                entity->state = MetalSonic_State_Unknown3;
                break;
            case 2: {
                int angle           = (RSDK.Rand(0, 2) + 3) << 6;
                entity->targetPos.x = 0xA000 * RSDK.Cos256(angle) + player1->position.x - (RSDK_screens->position.x << 16);
                entity->targetPos.y = player1->position.y + 0x7000 * RSDK.Sin256(angle) - (RSDK_screens->position.y << 16);
                entity->state       = MetalSonic_State_Unknown3;
                break;
            }
        }
    }
    else {
        entity->targetPos.x -= player1->position.x;
        entity->targetPos.y -= player1->position.y;

        if (entity->field_78 != entity->field_80) {
            if (entity->field_78 <= entity->field_80) {
                entity->field_78 += (0xC00 << (entity->field_78 < 0));
                if (entity->field_78 > entity->field_80)
                    entity->field_78 = entity->field_80;
            }
            else {
                entity->field_78 -= (0xC00 << (entity->field_78 > 0));
                if (entity->field_78 < entity->field_80)
                    entity->field_78 = entity->field_80;
            }
        }

        if (entity->field_7C != entity->field_84) {
            if (entity->field_7C <= entity->field_84) {
                entity->field_7C += (0xC00 << (entity->field_7C < 0));
                if (entity->field_7C > entity->field_84)
                    entity->field_7C = entity->field_84;
            }
            else {
                entity->field_7C -= (0xC00 << (entity->field_7C > 0));

                if (entity->field_7C < entity->field_84)
                    entity->field_7C = entity->field_84;
            }
        }

        entity->position.x += entity->field_78 + (entity->velocity.x >> 1) + (entity->velocity.x >> 3);
        entity->position.y += entity->field_7C + (entity->velocity.y >> 1) + (entity->velocity.y >> 3);
    }

    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_Unknown3(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->targetPos.x += RSDK_screens->position.x << 16;
    entity->targetPos.y += RSDK_screens->position.y << 16;
    MetalSonic_HandleVelocity();

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->position.x += player1->velocity.x;
    entity->position.y += player1->velocity.y;
    MetalSonic_HandleAnimDir();

    int rx              = entity->position.x - entity->targetPos.x;
    int ry              = entity->position.y - entity->targetPos.y;
    entity->targetPos.x = entity->targetPos.x - (RSDK_screens->position.x << 16);
    entity->targetPos.y = entity->targetPos.y - (RSDK_screens->position.y << 16);
    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 0x1000) {
        entity->timer2 = 0;
        entity->position.x &= 0xFFFF0000;
        entity->position.y &= 0xFFFF0000;
        entity->targetPos.x &= 0xFFFF0000;
        entity->targetPos.y &= 0xFFFF0000;
        entity->state = MetalSonic_State_StartAttack;
    }
    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_StartAttack(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->targetPos.x += RSDK_screens->position.x << 16;
    entity->targetPos.y += RSDK_screens->position.y << 16;

    entity->velocity.x = (entity->targetPos.x - entity->position.x) >> 4;
    entity->velocity.y = (entity->targetPos.y - entity->position.y) >> 4;

    if (entity->position.x >= entity->targetPos.x)
        entity->velocity.x -= 0x10000;
    else
        entity->velocity.x += 0x10000;

    if (entity->position.y >= entity->targetPos.y)
        entity->velocity.y += 0x10000;
    else
        entity->velocity.y += 0x10000;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->position.x += player1->velocity.x;
    entity->position.y += player1->velocity.y;

    uint8 flags = 0;
    if (entity->velocity.x > 0 && entity->position.x > entity->targetPos.x) {
        flags              = 1;
        entity->position.x = entity->targetPos.x;
    }
    else if (entity->velocity.x < 0 && entity->position.x < entity->targetPos.x) {
        flags              = 1;
        entity->position.x = entity->targetPos.x;
    }

    if (entity->velocity.y > 0 && entity->position.y > entity->targetPos.y) {
        ++flags;
        entity->position.y = entity->targetPos.y;
    }
    else if (entity->velocity.y < 0 && entity->position.y < entity->targetPos.y) {
        ++flags;
        entity->position.y = entity->targetPos.y;
    }

    entity->targetPos.x = entity->targetPos.x - (RSDK_screens->position.x << 16);
    entity->targetPos.y = entity->targetPos.y - (RSDK_screens->position.y << 16);
    MetalSonic_HandleAnimDir();

    if (flags == 2) {
        if (entity->attackType) {
            if (entity->attackType == 1) {
                entity->timer2 = 60;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 7, &entity->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 255);
                entity->state = MetalSonic_State_Unknown7;
            }
            if (entity->attackType == 2) {
                entity->timer2      = 60;
                entity->targetPos.x = entity->position.x - player1->position.x;
                entity->targetPos.y = entity->position.y - player1->position.y;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 8, &entity->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSBall, false, 255);
                entity->state = MetalSonic_State_Unknown5;
            }
        }
        else {
            entity->timer2 = 60;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 6, &entity->animator, true, 0);
            RSDK.PlaySfx(MetalSonic->sfxMSChargeFire, false, 255);
            entity->state = MetalSonic_State_Unknown9;
        }
    }
    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_Unknown5(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x = player1->position.x + entity->targetPos.x;
    entity->position.y = player1->position.y + entity->targetPos.y;

    int angle          = RSDK.ATan2(player1->position.x - entity->position.x, player1->position.y - entity->position.y);
    entity->field_9C.x = (RSDK.Cos256(angle + 192) << 12) + entity->position.x;
    entity->field_9C.y = (RSDK.Sin256(angle + 192) << 12) + entity->position.y;
    entity->field_A4.x = (RSDK.Cos256(angle + 64) << 12) + entity->position.x;
    entity->field_A4.y = (RSDK.Sin256(angle + 64) << 12) + entity->position.y;
    entity->field_AC.x = (RSDK.Cos256(angle) << 14) + entity->position.x;
    entity->field_AC.y = (RSDK.Sin256(angle) << 14) + entity->position.y;

    if (--entity->timer2 <= 0) {
        entity->field_9C.x = -1;
        entity->targetPos  = player1->position;
        entity->velocity.x = 0xA00 * RSDK.Cos256(angle);
        entity->velocity.y = 0xA00 * RSDK.Sin256(angle);
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);
        entity->timer2 = 15;
        entity->state  = MetalSonic_State_Unknown6;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}

void MetalSonic_State_Unknown6(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!entity->activeScreens && --entity->timer2 <= 0) {
        entity->targetPos  = player1->position;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->timer      = 0;
        entity->timer2     = 150;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, true, 0);
        entity->state = MetalSonic_State_Unknown2;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}

void MetalSonic_State_Unknown7(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x = entity->targetPos.x + (RSDK_screens->position.x << 16);
    entity->position.y = entity->targetPos.y + (RSDK_screens->position.y << 16);

    if (--entity->timer2 <= 0) {
        entity->targetPos.x = player1->position.x;
        entity->targetPos.y = player1->position.y;
        RSDK.ATan2(entity->targetPos.x - entity->position.x, entity->targetPos.y - entity->position.y);

        entity->velocity.y = 0x88000;
        if (entity->position.x < player1->position.x)
            entity->velocity.x = 0x40000;
        else
            entity->velocity.x = -0x40000;
        entity->timer2 = 15;
        entity->state  = MetalSonic_State_Unknown8;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}

void MetalSonic_State_Unknown8(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;
    if (entity->position.y < player1->position.y && !entity->activeScreens && --entity->timer2 <= 0) {
        entity->targetPos  = player1->position;
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->timer      = 0;
        entity->timer2     = 150;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, true, 0);
        entity->state = MetalSonic_State_Unknown2;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}

void MetalSonic_State_Unknown9(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x = entity->targetPos.x + (RSDK_screens->position.x << 16);
    entity->position.y = entity->targetPos.y + (RSDK_screens->position.y << 16);

    int angle          = RSDK.ATan2(player1->position.x - entity->position.x, player1->position.y - entity->position.y);
    entity->field_9C.x = (RSDK.Cos256(angle + 192) << 12) + entity->position.x;
    entity->field_9C.y = (RSDK.Sin256(angle + 192) << 12) + entity->position.y;
    entity->field_A4.x = (RSDK.Cos256(angle + 64) << 12) + entity->position.x;
    entity->field_A4.y = (RSDK.Sin256(angle + 64) << 12) + entity->position.y;
    entity->field_AC.x = (RSDK.Cos256(angle) << 15) + entity->position.x;
    entity->field_AC.y = (RSDK.Sin256(angle) << 15) + entity->position.y;
    if (--entity->timer2 <= 0) {
        entity->field_9C.x = -1;
        entity->targetPos  = player1->position;
        entity->velocity.x = 0xC00 * RSDK.Cos256(angle);
        entity->velocity.y = 0xC00 * RSDK.Sin256(angle);
        RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);
        entity->timer2 = 15;
        entity->state  = MetalSonic_State_Unknown10;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}

void MetalSonic_State_Unknown10(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    bool32 flag = true;
    if (entity->velocity.x <= 0) {
        if (entity->velocity.x >= 0 || entity->position.x >= player1->position.x) {
            flag = false;
        }
    }
    else if (entity->position.x <= player1->position.x) {
        if (entity->velocity.x >= 0 || entity->position.x >= player1->position.x) {
            flag = false;
        }
    }

    if (flag) {
        if (!entity->activeScreens && --entity->timer2 <= 0) {
            entity->targetPos  = player1->position;
            entity->velocity.x = 0;
            entity->velocity.y = 0;
            entity->timer      = 0;
            entity->timer2     = 150;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, true, 0);
            entity->state = MetalSonic_State_Unknown2;
        }
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}

void MetalSonic_State_EnterPanel(void)
{
    RSDK_THIS(MetalSonic);

    int velX = entity->velocity.x;
    MetalSonic_HandleVelocity();

    if (entity->velocity.x < 0x20000 && velX >= 0x20000) {
        entity->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }
    else if (entity->velocity.x > -0x20000 && velX <= -0x20000) {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }

    ++entity->timer;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    int rx = entity->position.x - entity->targetPos.x;
    int ry = entity->position.y - entity->targetPos.y;
    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 4096 && entity->timer > 96) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, false, 0);
        entity->state = MetalSonic_State_StartPanelSeq;
    }
}

void MetalSonic_State_StartPanelSeq(void)
{
    RSDK_THIS(MetalSonic);

    ++entity->timer;
    entity->position.x += (entity->targetPos.x - entity->position.x) >> 4;
    entity->position.y += (entity->targetPos.y - entity->position.y) >> 4;
    if (entity->timer == 32)
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 9, &entity->animator, true, 0);
    if (entity->timer == 59)
        RSDK.PlaySfx(MetalSonic->sfxSpecialRing, false, 255);

    if (entity->timer == 64) {
        int id = 0;
#if RETRO_USE_PLUS
        for (int i = 48; i < 82; i += 2) {
            if (id > 0)
                RSDK.CopyTileLayer(Zone->fgLow, 167, i, Zone->fgHigh, 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 169, i, Zone->fgHigh, 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 171, i, Zone->fgHigh, 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 173, i, Zone->fgHigh, 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 175, i, Zone->fgHigh, 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 177, i, Zone->fgHigh, 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 179, i, Zone->fgHigh, 222, 138, 2, 2);
            ++id;
        }
#else
        for (int i = 128; i < 82; i += 2) {
            int y = i;
            if (id > 0) {
                y = 2 * id + 128;
                RSDK.CopyTileLayer(Zone->fgLow, 167, i, Zone->fgHigh, 222, 218, 2, 2);
            }
            RSDK.CopyTileLayer(Zone->fgLow, 169, y, Zone->fgHigh, 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 171, y, Zone->fgHigh, 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 173, y, Zone->fgHigh, 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 175, y, Zone->fgHigh, 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 177, y, Zone->fgHigh, 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLow, 179, y, Zone->fgHigh, 222, 218, 2, 2);
            ++id;
        }
#endif

        EntityMSPanel *panel = (EntityMSPanel *)entity->panel;
        entity->position     = entity->targetPos;
        entity->timer        = 16;
        entity->timer2       = RETRO_USE_PLUS ? 240 : 180;
        entity->health       = RETRO_USE_PLUS ? 6 : 4;
        entity->state        = MetalSonic_State_ShowFactory;
        panel->state         = MSPanel_Unknown2;
    }
}

void MetalSonic_HandlePanelAttack(void)
{
    RSDK_THIS(MetalSonic);

#if RETRO_USE_PLUS
    if (!RSDK.GetEntityCount(MSBomb->objectID, true))
#endif
        --entity->timer2;
    if (entity->timer2 == 60) {
        RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 255);
        entity->field_94 = 60;

#if RETRO_USE_PLUS
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, entity, entity->position.x, entity->position.y);
        ring->field_78         = 24;
        ring->timer            = 24;
        ring->r                = 0x00;
        ring->g                = 0x80;
        ring->b                = 0xF0;
        ring->field_70         = 1;
#endif
    }
    if (entity->timer2 <= 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        entity->timer2   = 45 * entity->health + (RETRO_USE_PLUS ? 195 : 135);
        int angle        = RSDK.ATan2(player1->position.x - entity->position.x, player1->position.y - entity->position.y);
        EntityMSOrb *orb = CREATE_ENTITY(MSOrb, NULL, entity->position.x, entity->position.y);
        orb->velocity.x  = 0x280 * RSDK.Cos256(angle);
        orb->velocity.y  = 0x280 * RSDK.Sin256(angle);
        RSDK.PlaySfx(MetalSonic->sfxMSShoot, false, 255);
    }
}

void MetalSonic_State_ShowFactory(void)
{
    RSDK_THIS(MetalSonic);

    ++entity->timer;
    MetalSonic_HandlePanelAttack();
    if (entity->timer == 60) {
        foreach_active(MSFactory, factory)
        {
            factory->visible = true;
            factory->state   = MSFactory_Unknown3;
        }

        entity->timer = 0;
        entity->state = MetalSonic_State_Unknown13;
    }
}

void MetalSonic_State_Unknown13(void)
{
    RSDK_THIS(MetalSonic);

    ++entity->timer;
    MetalSonic_HandlePanelAttack();

#if RETRO_USE_PLUS
    if (!RSDK.GetEntityCount(SilverSonic->objectID, true) && !RSDK.GetEntityCount(MSBomb->objectID, true) && entity->timer > 60) {
#else
    if (!RSDK.GetEntityCount(SilverSonic->objectID, true) && entity->timer > 60) {
#endif
        entity->timer = 0;
        entity->state = MetalSonic_State_ShowFactory;
    }
}

void MetalSonic_State_PanelExplosion(void)
{
    RSDK_THIS(MetalSonic);
    EntityMSPanel *panel = (EntityMSPanel *)entity->panel;

    entity->position.y += 0x8000;
    panel->position.y += 0x8000;
    if (!entity->timer)
        panel->state = MSPanel_Unknown3;
    if (++entity->timer == 104) {
        entity->timer = 0;
        panel->state  = MSPanel_Unknown4;
        entity->state = MetalSonic_State_Unknown14;
    }
}

void MetalSonic_State_Unknown14(void)
{
    RSDK_THIS(MetalSonic);

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, false, 3);
        entity->velocity.x = 0x18000;
        entity->onGround   = false;
        entity->state      = MetalSonic_State_Unknown15;
    }
}

void MetalSonic_State_Unknown15(void)
{
    RSDK_THIS(MetalSonic);

    entity->velocity.y += 0x2000;
    entity->outerBox = RSDK.GetHitbox(&entity->animator, 0);
    entity->innerBox = RSDK.GetHitbox(&entity->animator, 1);
    RSDK.ProcessTileCollisions(entity, entity->outerBox, entity->innerBox);

    if (entity->onGround) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 2, &entity->animator, false, 0);
        entity->direction = FLIP_NONE;
        entity->groundVel = 0;
    }

    if (++entity->timer == 120) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->wait         = 32;
        fxFade->speedOut     = 8;
        EntityMSPanel *panel = (EntityMSPanel *)entity->panel;
        panel->state         = StateMachine_None;
        entity->timer        = 0;
        entity->timer2       = 240;
        entity->state        = MetalSonic_State_Unknown2;
        foreach_active(Player, player)
        {
            RSDK.PlaySfx(SpeedBooster->sfxID, false, 255);
            player->velocity.x  = 0xE0000;
            player->groundVel   = 0xE0000;
            player->controlLock = 60;
            player->direction   = FLIP_NONE;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->playerAnimator, false, 0);
            player->state = Player_State_Ground;
        }

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
        Zone->screenBoundsL1[0]     = 0;
        Zone->screenBoundsR1[0]     = size.x;
        Zone->screenBoundsT1[0]     = 0;
        Zone->playerBoundActiveL[0] = false;
        Zone->playerBoundActiveR[0] = false;
        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 255);
    }
}

#if RETRO_USE_PLUS
void MetalSonic_State_WaitForRuby(void)
{
    RSDK_THIS(MetalSonic);

    int velX = entity->velocity.x;
    MetalSonic_HandleVelocity();

    if (entity->velocity.x < 0x20000 && velX >= 0x20000) {
        entity->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }
    else if (entity->velocity.x > -0x20000 && velX <= -0x20000) {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }

    ++entity->timer;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    int rx = entity->position.x - entity->targetPos.x;
    int ry = entity->position.y - entity->targetPos.y;

    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 4096 && entity->timer > 96) {
        entity->timer     = 0;
        entity->direction = FLIP_NONE;
        entity->state     = MetalSonic_State_ObtainRuby;
    }
}

void MetalSonic_State_ObtainRuby(void)
{
    RSDK_THIS(MetalSonic);

    entity->targetPos.y += RSDK.Sin256(4 * Zone->timer) << 8;

    entity->position.x += (entity->targetPos.x - entity->position.x) >> 4;
    entity->position.y += (entity->targetPos.y - entity->position.y) >> 4;

    foreach_active(PhantomRuby, ruby)
    {
        int rx = (entity->position.x - ruby->position.x) >> 16;
        int ry = (entity->position.y - ruby->position.y) >> 16;
        if (rx * rx + ry * ry < 0x100) {
            ruby->startPos.x = ruby->position.x;
            ruby->startPos.y = ruby->position.y;
            ruby->state      = PhantomRuby_Unknown5;
            entity->state    = MetalSonic_State_Transform;
            RSDK.PlaySfx(MetalSonic->sfxMSTransform, false, 255);
        }
    }
}

void MetalSonic_State_Transform(void)
{
    RSDK_THIS(MetalSonic);

    entity->position.y -= 0x2000;
    foreach_active(PhantomRuby, ruby) { ruby->startPos.y -= 0x2000; }

    if (++entity->timer == 30) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0xFFFF, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 7, &entity->animator, true, 0);
    }

    int timer = entity->timer >> 1;
    if (timer > 96)
        timer = 96;

    if ((entity->timer & 0x1F) == 30) {
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, entity, entity->position.x, entity->position.y);
        ring->field_78         = timer;
        ring->timer            = 24;
        ring->r                = 0x00;
        ring->g                = 0xF0;
        ring->b                = 0xF0;
        ring->field_70         = 2;
    }

    if (timer >= 32 && (entity->timer & 0xF) == 14) {
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, entity, entity->position.x, entity->position.y);
        ring->field_78         = timer + 64;
        ring->timer            = 24;
        ring->r                = 0xF0;
        ring->g                = 0xF0;
        ring->b                = 0x60;
        ring->field_70         = 4;
    }

    if (entity->timer == 240) {
        entity->timer        = 0;
        entity->active       = ACTIVE_NEVER;
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->wait         = 32;
        fxFade->speedOut     = 8;
        RSDK.PlaySfx(MetalSonic->sfxTransform2, false, 255);
    }
}

void MetalSonic_State_Defeated(void)
{
    RSDK_THIS(MetalSonic);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= true;
    if (!RSDK.CheckOnScreen(entity, NULL)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), (RSDK_screens->position.x + RSDK_screens->centerX) << 16,
                                                (RSDK_screens->position.y - 48) << 16);
        prison->velocity.x      = 0x10000;
        prison->active          = ACTIVE_NORMAL;
        destroyEntity(entity);
    }
}
#else
void MetalSonic_State_Unknown16(void)
{
    RSDK_THIS(MetalSonic);

    MetalSonic_HandleVelocity();
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    MetalSonic_HandleStageWrap();
    entity->timer++;

    EntityPlatform *wall = NULL;
    switch (entity->timer) {
        case 60: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform); break;
        case 90: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 3, Platform); break;
        case 120: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 4, Platform); break;
        case 150: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 5, Platform); break;
        case 180: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 6, Platform); break;
        case 210: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 7, Platform); break;
        case 240: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 8, Platform); break;
        case 270: wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 9, Platform); break;
        case 300: {
            EntityPlatform *startWall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);
            entity->targetPos.x = startWall->position.x + 0x800000;
            entity->targetPos.y = (Zone->screenBoundsB1[0] << 16) - 0x340000;
            entity->state = MetalSonic_State_Unknown17;
            break;
        }
    }

    if (wall) {
        wall->active = ACTIVE_NORMAL;
        wall->state = MetalSonic_StateWall_Fall;
    }
}
void MetalSonic_State_Unknown17(void)
{
    RSDK_THIS(MetalSonic);

    MetalSonic_HandleVelocity();
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    MetalSonic_HandleAnimDir();

    int rx = entity->position.x - entity->targetPos.x;
    int ry = entity->position.y - entity->targetPos.y;
    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 0x1000) {
        entity->timer2 = 0;
        entity->position.x &= 0xFFFF0000;
        entity->position.y &= 0xFFFF0000;
        entity->targetPos.x &= 0xFFFF0000;
        entity->targetPos.y &= 0xFFFF0000;
        entity->state = MetalSonic_State_Unknown18;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown18(void)
{
    RSDK_THIS(MetalSonic);

    entity->velocity.x = (entity->targetPos.x - RSDK_sceneInfo->entity->position.x) >> 4;
    entity->velocity.y = (entity->targetPos.y - RSDK_sceneInfo->entity->position.y) >> 4;

    if (entity->position.x >= entity->targetPos.x)
        entity->velocity.x -= 0x10000;
    else
        entity->velocity.x += 0x10000;

    if (entity->position.y >= entity->targetPos.y)
        entity->velocity.y -= 0x10000;
    else
        entity->velocity.y += 0x10000;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    uint8 flags = 0;
    if (entity->velocity.x > 0) {
        if (entity->position.x > entity->targetPos.x) {
            flags = 1;
            entity->position.x = entity->targetPos.x;
        }
    }
    else if (entity->position.x < entity->targetPos.x) {
        flags = 1;
        entity->position.x = entity->targetPos.x;
    }

    if (entity->velocity.y > 0) {
        if (entity->position.y > entity->targetPos.y) {
            ++flags;
            entity->position.y = entity->targetPos.y;
        }
    }
    else if (entity->position.y < entity->targetPos.y) {
        ++flags;
        entity->position.y = entity->targetPos.y;
    }

    MetalSonic_HandleAnimDir();
    if (flags == 2) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 1, &entity->animator, false, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, false, 0);
        entity->direction = 0;
        entity->state = MetalSonic_State_Unknown19;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown19(void)
{
    RSDK_THIS(MetalSonic);

    if (entity->animator.frameID == entity->animator.frameCount - 1) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 2, &entity->animator, false, 0);
        entity->state = MetalSonic_State_Unknown20;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown20(void)
{
    RSDK_THIS(MetalSonic);

    entity->timer++;
    if (entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &entity->animator2, false, 0);
        entity->state = MetalSonic_State_Unknown21;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown21(void)
{
    RSDK_THIS(MetalSonic);

    entity->timer++;
    if (entity->timer == 90) {
        int slot = RSDK_sceneInfo->entitySlot;
        RSDK_GET_ENTITY(slot + 2, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 3, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 4, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 5, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 6, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 7, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 8, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 9, Platform)->state = MetalSonic_StateWall_Move;
        entity->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, false, 0);
        entity->state = MetalSonic_State_Unknown22;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown22(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    if (wall->velocity.x >= 0x1C000) {
        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);

        for (int i = 0; i < PLAYER_MAX; ++i) {
            Zone->screenBoundsL1[i] = 0;
            Zone->screenBoundsR1[i] = size.x + 0x400;
            Zone->playerBoundActiveL[i] = false;
            Zone->playerBoundActiveL[i] = false;
        }

        if (wall->velocity.x >= 0x54000) {
            entity->health = 8;
            entity->timer2 = 120;
            entity->state = MetalSonic_State_Unknown23;
        }
    }

    entity->velocity.x = wall->velocity.x;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown23(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    if (player1->position.x <= entity->position.x) {
        if (entity->velocity.x >= player1->velocity.x - 0x60000) {
            entity->velocity.x -= 0xC00;
        }
        else
            entity->velocity.x += 0xC00;
    }
    else if (entity->velocity.x < 0x60000)
        entity->velocity.x += 0xC00;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true))
        entity->velocity.y = 0;
    if (entity->direction == FLIP_X) {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }
    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x800000)
        entity->position.x = wall->position.x + 0x800000;

    if (--entity->timer2 <= 0 && !player1->blinkTimer) {
        if (entity->velocity.x < 0x60000)
            entity->velocity.x = 0x60000;
        entity->timer2 = 120;
        entity->attackType = RSDK.Rand(0, 6);

        while ((1 << entity->attackType) & MetalSonic->field_C) {
            entity->attackType = RSDK.Rand(0, 6);
        }
        MetalSonic->field_C |= 1 << entity->attackType;
        if (MetalSonic->field_C == 0x3F)
            MetalSonic->field_C = 0;

        entity->attackType >>= 1;
        switch (entity->attackType) {
            case 2:
                entity->timer2 = 60;
                entity->targetPos.x = (RSDK.Rand(-3, 7) << 14) + 0x78000;
                entity->targetPos.y = -0x80000;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 8, &entity->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSBall, false, 0xFF);
                entity->state = MetalSonic_State_Unknown24;
                break;
            case 1:
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 7, &entity->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 0xFF);
                entity->state = MetalSonic_State_Unknown27;
                break;
            case 0:
                RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 0xFF);
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 6, &entity->animator, false, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, false, 0);
                entity->timer2 = 60;
                entity->state = MetalSonic_State_Unknown30;
                break;
        }
    }
    MetalSonic_CheckPlayerCollisions();
}
void MetalSonic_State_Unknown27(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    if (entity->velocity.x < 0xB0000)
        entity->velocity.x += 0xC00;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->position.y <= (RSDK_screens->centerY + RSDK_screens->position.y) << 16) {
        if (entity->velocity.y < 0x10000)
            entity->velocity.y += 0x1000;
    }
    else {
        if (entity->velocity.y >= -0x10000)
            entity->velocity.y -= 0x1000;
    }

    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x410000)
        entity->position.x = wall->position.x + 0x410000;

    if (entity->position.x > ((RSDK_screens->width + RSDK_screens->position.x) << 16) + 0x400000) {
        entity->direction = FLIP_X;
        entity->state = MetalSonic_State_Unknown28;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}
void MetalSonic_State_Unknown28(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    entity->velocity.x = player1->velocity.x - 0x10000;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->position.y <= (RSDK_screens->centerY + RSDK_screens->position.y) << 16) {
        if (entity->velocity.y < 0x10000)
            entity->velocity.y += 0x1000;
    }
    else {
        if (entity->velocity.y >= -0x10000)
            entity->velocity.y -= 0x1000;
    }

    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x410000) {
        entity->position.x = wall->position.x + 0x410000;
    }

    if (entity->position.x < ((RSDK_screens->position.x + RSDK_screens->width) << 16) - 0x180000) {
        entity->timer2 = 120;
        entity->position.x = ((RSDK_screens->position.x + RSDK_screens->width) << 16) - 0x180000;
        entity->state = MetalSonic_State_Unknown29;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}
void MetalSonic_State_Unknown29(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    entity->velocity.x = player1->velocity.x;
    entity->position.x = ((RSDK_screens->position.x + RSDK_screens->width) << 16) - 0x180000;
    entity->position.y += entity->velocity.y;

    if (entity->position.y <= (RSDK_screens->centerY + RSDK_screens->position.y) << 16) {
        if (entity->velocity.y < 0x10000)
            entity->velocity.y += 0x1000;
    }
    else {
        if (entity->velocity.y >= -0x10000)
            entity->velocity.y -= 0x1000;
    }

    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x410000)
        entity->position.x = wall->position.x + 0x410000;

    switch (--entity->timer2) {
        case 0:
            entity->timer2 = 120;
            entity->velocity.y = 0x10000;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, false, 0);
            entity->state = MetalSonic_State_Unknown26;
            break;
        case 30:
        case 60:
        case 90: {
            EntityMSOrb *orb = CREATE_ENTITY(MSOrb, NULL, entity->position.x, entity->position.y);
            orb->velocity.x = player1->velocity.x - 0x40000;
            orb->velocity.y = (RSDK.Rand(-1, 2) + 2) << 16;
            RSDK.PlaySfx(MetalSonic->sfxMSShoot, false, 0xFF);
            break;
        }
        default: break;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}
void MetalSonic_State_Unknown24(void)
{
    RSDK_THIS(MetalSonic);
    int angle = RSDK.ATan2(entity->targetPos.x, entity->targetPos.y);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    entity->field_9C.x = entity->position.x + (RSDK.Cos256(angle + 192) << 12);
    entity->field_9C.y = entity->position.y + (RSDK.Sin256(angle - 64) << 12);

    entity->field_A4.x = entity->position.x + (RSDK.Cos256(angle + 64) << 12);
    entity->field_A4.y = entity->position.y + (RSDK.Sin256(angle + 64) << 12);

    entity->field_AC.x = entity->position.x + (RSDK.Cos256(angle) << 14);
    entity->field_AC.y = entity->position.y + (RSDK.Sin256(angle) << 14);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x410000)
        entity->position.x = wall->position.x + 0x410000;

    entity->timer2--;
    if (entity->timer2 <= 0) {
        entity->field_9C.x = -1;
        entity->timer2 = 8;
        entity->velocity = entity->targetPos;
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 0xFF);
        entity->state = MetalSonic_State_Unknown25;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}
void MetalSonic_State_Unknown25(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->position.y >= 0x1F00000) {
        if (entity->position.x > ((RSDK_screens->width + RSDK_screens->position.x) << 16) + 0x400000 || !--entity->timer2) {
            entity->timer2 = 120;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, false, 2);
            entity->state = MetalSonic_State_Unknown26;
        }
        else {
            entity->velocity.y = -0x80000;
            RSDK.PlaySfx(MetalSonic->sfxRockemSockem, false, 0xFF);
        }
    }
    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x410000)
        entity->position.x = wall->position.x + 0x410000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}
void MetalSonic_State_Unknown30(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    if (entity->velocity.x < 0xD0000)
        entity->velocity.x += 0x1000;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall->position.x + 0x410000)
        entity->position.x = wall->position.x + 0x410000;

    if (entity->timer2) {
        entity->timer2--;
    }
    else if (entity->position.x > player1->position.x + 0x400000) {
        entity->timer2 = 120;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 5, &entity->animator, false, 2);
        entity->state = MetalSonic_State_Unknown26;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, entity);
        }
    }
}
void MetalSonic_State_Unknown26(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (entity->velocity.x > 0x20000)
        entity->velocity.x -= 0x1800;

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->position.y >= 0x1EC0000) {
        entity->position.y = 0x1EC0000;
        entity->velocity.y = 0;
    }
    if (entity->position.x < player1->position.x && entity->direction == FLIP_X) {
        entity->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &entity->animator, true, 0);
    }
    MetalSonic_HandleStageWrap();

    EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);
    if (entity->position.x < wall->position.x + 0x900000) {
        entity->position.x = wall->position.x + 0x900000;
        entity->timer2 = 120;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &entity->animator2, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &entity->animator, false, 0);
        entity->state = MetalSonic_State_Unknown23;
    }
    MetalSonic_CheckPlayerCollisions();
}
void MetalSonic_State_Explode(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall1 = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Platform);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.x -= 0x1000;
    entity->rotation += 6;
    MetalSonic_Explode();
    MetalSonic_HandleStageWrap();

    if (entity->position.x < wall1->position.x + 0x20000) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 0xFF);

        for (int i = 2; i < 10; ++i) {
            EntityPlatform *wall = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + i, Platform);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, wall->position.x, wall->position.y);
            RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &debris->animator, true, 1);
            debris->velocity.x = RSDK.Rand(6, 11) << 16;
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravity = 0x4800;
            debris->drawOrder = wall->drawOrder;
            debris->updateRange.x = 0x800000;
            debris->updateRange.x = 0x800000;
            destroyEntity(wall);
        }

        Zone->autoScrollSpeed = 0;
        entity->velocity.x = 0x50000;
        entity->velocity.y = -0x30000;
        entity->rotation = 0;
        entity->drawFX &= ~FX_ROTATE;
        entity->state = MetalSonic_State_Defeated;
    }
}
void MetalSonic_State_Defeated(void)
{
    RSDK_THIS(MetalSonic);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    entity->visible ^= true;
    MetalSonic_HandleStageWrap();

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->visible = true;
        entity->timer = 90;
        entity->state = MetalSonic_State_Finish;
    }
}
void MetalSonic_State_Finish(void)
{
    RSDK_THIS(MetalSonic);
    MetalSonic_HandleStageWrap();

    if (!--entity->timer) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        Zone->screenBoundsL1[0] = RSDK_screens->position.x;
        Zone->screenBoundsR1[0] = RSDK_screens->width + RSDK_screens->position.x;
        Zone->screenBoundsR2[0] = (RSDK_screens->width + RSDK_screens->position.x) << 16;
        Zone->screenBoundsL2[0] = RSDK_screens->position.x << 16;

        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), (RSDK_screens->position.x + RSDK_screens->centerX) << 16,
                                                (RSDK_screens->position.y - 48) << 16);
        prison->velocity.x = 0x10000;
        prison->active = ACTIVE_NORMAL;
        entity->state = MetalSonic_State_None;
    }
}
void MetalSonic_State_None(void) {}

void MetalSonic_StateWall_Fall(void)
{
    RSDK_THIS(Platform);

    entity->drawPos.y += entity->velocity.y;
    entity->centerPos.y = entity->drawPos.y;
    entity->position.y = entity->drawPos.y;
    entity->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, (entity->speed << 21) + 0x100000, true)) {
        entity->velocity.y = 0;
        entity->drawPos.y = entity->position.y;
        entity->centerPos.y = entity->position.y;
        entity->state = Platform_State_Normal;
        Camera_ShakeScreen(0, 0, 4);
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 0xFF);
    }
}
void MetalSonic_StateWall_Move(void)
{
    RSDK_THIS(Platform);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    bool32 flag = false;
    foreach_active(MetalSonic, metal)
    {
        if (metal->field_94)
            flag = true;
    }

    if (flag || player1->blinkTimer) {
        entity->type = 120;
        entity->type--;
        if (entity->velocity.x > 0x40000)
            entity->velocity.x -= 0x1000;
    }
    else {
        if (entity->type <= 0) {
            if (entity->velocity.x < 0x54000) {
                entity->velocity.x += 0x400;
            }
            else {
                entity->type--;
                if (entity->velocity.x > 0x40000)
                    entity->velocity.x -= 0x1000;
            }
        }
        else {
            entity->type--;
            if (entity->velocity.x > 0x40000)
                entity->velocity.x -= 0x1000;
        }
    }

    Zone->autoScrollSpeed = entity->velocity.x;
    entity->drawPos.x += entity->velocity.x;
    entity->centerPos.x = entity->drawPos.x;
    entity->position.x = entity->drawPos.x;

    int x = player1->position.x - 0x2000000;
    if (x < entity->position.x - 0x2000000 || x > entity->position.x) {
        entity->drawPos.x = x;
        entity->centerPos.x = x;
        entity->position.x = x;
    }
    if (entity->speed == 1) {
        EntityPlatform *belowPlat = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, Platform);
        belowPlat->drawPos.x = entity->position.x;
        belowPlat->centerPos.x = entity->position.x;
        belowPlat->position.x = entity->position.x;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void MetalSonic_EditorDraw(void)
{
    RSDK_THIS(MetalSonic);
    Vector2 drawPos;

    entity->drawFX         = FX_FLIP | FX_ROTATE;
    entity->updateRange.x  = 0x800000;
    entity->updateRange.y  = 0x800000;
    entity->tileCollisions = true;
    RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &entity->animator, false, 0);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (entity->position.x < 0x8000000)
        drawPos.x += 0xE000000;
    else
        drawPos.x -= 0xE000000;

    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MetalSonic_EditorLoad(void) { MetalSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE); }
#endif

void MetalSonic_Serialize(void) {}
