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
    foreach_active(StarPost, post) { post->starTimer = 0; }
}

void MetalSonic_LateUpdate(void)
{

}

void MetalSonic_StaticUpdate(void)
{

}

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

void MetalSonic_Create(void* data)
{
    RSDK_THIS(MetalSonic);
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        else {
            Zone->field_154          = 0;
            entity->active          = ACTIVE_BOUNDS;
            entity->drawFX          = FX_FLIP | FX_ROTATE;
            entity->visible         = false;
            entity->updateRange.x   = 0x800000;
            entity->updateRange.y   = 0x800000;
            entity->tileCollisions  = true;
            entity->collisionLayers = Zone->fgLayers;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &entity->animator, true, 0);
            entity->drawOrder = Zone->drawOrderLow;
            //entity->state          = MetalSonic_Unknown5;
        }
    }
}

void MetalSonic_StageLoad(void)
{
    MetalSonic->aniFrames       = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE);
    MetalSonic->hitbox1.left    = -8;
    MetalSonic->hitbox1.top     = -112;
    MetalSonic->hitbox1.right   = 8;
    MetalSonic->hitbox1.bottom  = 128;
    MetalSonic->hitbox2.left    = -16;
    MetalSonic->hitbox2.top     = -112;
    MetalSonic->hitbox2.right   = 16;
    MetalSonic->hitbox2.bottom  = 128;
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
    MetalSonic->sfxMSTransform  = RSDK.GetSFX("SSZ2/MSTransform.wav");
    MetalSonic->sfxTransform2   = RSDK.GetSFX("Stage/Transform2.wav");
}

void MetalSonic_ProcessBGParallax(int32 mult)
{
    for (int32 i = 0; i < 2; ++i) {
        TileLayer *bg = RSDK.GetSceneLayer(i);
        for (int32 s = 0; s < bg->scrollInfoCount; ++s) {
            bg->scrollInfo[0].scrollPos += mult * bg->scrollInfo[0].parallaxFactor;
        }
    }

    TileLayer *bg3 = RSDK.GetSceneLayer(2);
    bg3->scrollPos += mult * bg3->parallaxFactor;

    foreach_active(EggTower, tower) { tower->rotationX += (mult >> 8); }
}

void MetalSonic_HandleStageWrap(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (camera->position.x < 0xF000000)
    {
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
            foreach_active(Firework, fireWork) { fireWork->position.x += 0xE000000; }
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
        foreach_active(Firework, fireWork) { fireWork->position.x -= 0xE000000; }
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
        }

        for (int32 i = 1; i < Player->playerCount; ++i) {
            RSDK_GET_ENTITY(i, Player)->position.x -= 0xE000000;
        }
    }
}

void MetalSonic_EditorDraw(void)
{

}

void MetalSonic_EditorLoad(void)
{

}

void MetalSonic_Serialize(void)
{

}

