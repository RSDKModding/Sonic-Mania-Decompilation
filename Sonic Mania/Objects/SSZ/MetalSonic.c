#include "SonicMania.h"

ObjectMetalSonic *MetalSonic;

void MetalSonic_Update(void)
{
    RSDK_THIS(MetalSonic);
    if (self->field_94)
        self->field_94--;
    RSDK.ProcessAnimation(&self->animator);
    RSDK.ProcessAnimation(&self->animator2);
    StateMachine_Run(self->state);
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

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->position.x < 0x8000000)
        drawPos.x += 0xE000000;
    else
        drawPos.x -= 0xE000000;

    if (self->field_94 & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 8);
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator, NULL, false);
        RSDK.CopyPalette(1, 240, 0, 240, 8);
    }
    else {
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        RSDK.DrawSprite(&self->animator2, NULL, false);
        RSDK.DrawSprite(&self->animator, NULL, false);
    }
}

void MetalSonic_Create(void *data)
{
    RSDK_THIS(MetalSonic);
    if (!SceneInfo->inEditor) {
        if (globals->gameMode == MODE_TIMEATTACK) {
            destroyEntity(self);
        }
        else {
            Zone->autoScrollSpeed   = 0;
            self->active          = ACTIVE_BOUNDS;
            self->drawFX          = FX_FLIP | FX_ROTATE;
            self->visible         = false;
            self->updateRange.x   = 0x800000;
            self->updateRange.y   = 0x800000;
            self->tileCollisions  = true;
            self->collisionLayers = Zone->fgLayers;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &self->animator, true, 0);
            self->drawOrder = Zone->drawOrderLow;
            self->state     = MetalSonic_State_SetupArena;
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
    EntityMetalSonic *marker = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, MetalSonic);
    EntityPlatform *wall   = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);
    if (player->position.y < marker->position.y && !player->collisionPlane) {
        Zone->screenBoundsT1[0] = (marker->position.y >> 16) + 16 - ScreenInfo->height;
        Zone->screenBoundsB1[0] = (marker->position.y >> 16) + 16;
        Zone->deathBoundary[0]  = (marker->position.y >> 16) + 16;
        marker->position.y      = -0x200000;

        for (int i = 0; i < PLAYER_MAX; ++i) {
            Zone->screenBoundsL1[i]     = (wall->position.x >> 16) - 95;
            Zone->screenBoundsR1[i]     = (wall->position.x >> 16) + 392;
            Zone->playerBoundActiveL[i] = true;
            Zone->playerBoundActiveR[i] = true;
        }

        self->timer       = 59;
        self->targetPos.x = 0x9100000;
        self->targetPos.y = 0x23400000;

        int anim = 0;
        if (self->position.x >= 0x9100000) {
            if (self->direction != FLIP_X) {
                self->direction = FLIP_X;
                anim              = 4;
            }
            else {
                anim = 3;
            }
        }
        else if (self->direction) {
            self->direction = FLIP_NONE;
            anim              = 4;
        }
        else {
            anim = 3;
        }

        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, anim, &self->animator, true, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &self->animator2, false, 0);
        self->state = MetalSonic_State_Unknown16;
    }
#endif

    if (camera->position.x < 0xF000000) {
        if (camera->position.x <= 0x1000000) {
            player->position.x += 0xE000000;
            self->position.x += 0xE000000;
            camera->position.x += 0xE000000;
            camera->center.x += 0xE00;
            MetalSonic_ProcessBGParallax(-0xE0000);
#if RETRO_USE_PLUS
            if (self->objectID == GigaMetal->objectID) {
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
        self->position.x -= 0xE000000;
        camera->position.x -= 0xE000000;
        camera->center.x -= 0xE00;
        MetalSonic_ProcessBGParallax(0xE0000);
#if RETRO_USE_PLUS
        if (self->objectID == GigaMetal->objectID) {
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

    int angle = RSDK.ATan2((self->targetPos.x - self->position.x) >> 16, (self->targetPos.y - self->position.y) >> 16);
    int ang   = 2 * angle - self->angle;

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
    self->angle = (angle / 18 + self->angle) & 0x1FF;

    self->velocity.x = RSDK.Cos512(self->angle) << 9;
    self->velocity.y = RSDK.Sin512(self->angle) << 9;
}

void MetalSonic_HandleAnimDir(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (self->animator.animationID == 4) {
        if (self->animator.frameID == self->animator.frameCount - 1)
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, true, 0);
    }
    else if (self->position.x <= player1->position.x) {
        if (self->direction == FLIP_X) {
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
        }
    }
    else if (self->direction == FLIP_NONE) {
        self->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }
}

#if !RETRO_USE_PLUS
void MetalSonic_CheckPlayerCollisions(void)
{
    RSDK_THIS(MetalSonic);

    foreach_active(Player, player)
    {
        if (!self->field_94) {
            Hitbox *hitbox = self->animator.animationID == 5 ? &MetalSonic->hitbox2 : &MetalSonic->hitbox1;
            if (Player_CheckBadnikTouch(player, self, hitbox) && Player_CheckBossHit(player, self)) {
                if (player->velocity.x < 0)
                    player->velocity.x >>= 2;

                player->velocity.x >>= 1;
                player->velocity.y >>= 1;
                player->velocity.x += self->velocity.x;
                player->velocity.y += self->velocity.y;
                player->groundVel = player->velocity.x;
                MetalSonic_Hit();
            }
        }
    }
}

void MetalSonic_Hit(void)
{
    RSDK_THIS(MetalSonic);

    if (!--self->health) {
        self->timer               = 180;
        SceneInfo->timeEnabled = false;
        self->velocity.y          = -0x1800;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 10, &self->animator, false, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 13, &self->animator2, false, 0);
        self->drawFX |= FX_ROTATE;
        self->state = MetalSonic_State_Explode;
    }
    else {
        self->field_94 = 48;
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 0xFF);
    }
}

void MetalSonic_Explode(void)
{
    RSDK_THIS(MetalSonic);
    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 0xFF);
        if (!(Zone->timer & 0xF)) {
            int x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int y = self->position.y + (RSDK.Rand(-24, 25) << 16);

            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
        }
    }
}
#endif

void MetalSonic_State_SetupArena(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer >= 8) {
        self->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->screenBoundsT1[0]     = (self->position.y >> 16) - ScreenInfo->height + 52;
        Zone->screenBoundsB1[0]     = (self->position.y >> 16) + 52;
        self->state               = MetalSonic_State_WaitForPlayer;
    }
}

void MetalSonic_State_WaitForPlayer(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->position.x > self->position.x) {
        Music_FadeOut(0.0125);
        self->state = MetalSonic_State_WaitForHologram;
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
            self->position.y += 0x600000;
            self->velocity.y = -0x80000;
            self->visible    = true;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 9, &self->animator, false, 6);
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 13, &self->animator2, false, 0);
            self->direction               = FLIP_X;
            self->animator.animationSpeed = 0;
            self->active                  = ACTIVE_NORMAL;
            self->state                   = MetalSonic_State_Appear;
            Camera_ShakeScreen(0, 6, 6);
            foreach_break;
        }
    }
}

void MetalSonic_State_Appear(void)
{
    RSDK_THIS(MetalSonic);

    self->velocity.y += 0x2000;
    self->position.y += self->velocity.y;

    if (self->velocity.y >= 0) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, false, 3);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, false, 0);
        self->velocity.x = 0x18000;
        self->onGround   = false;
        self->state      = MetalSonic_State_Land;
    }
}

void MetalSonic_State_Land(void)
{
    RSDK_THIS(MetalSonic);

    self->velocity.y += 0x2000;
    self->outerBox = RSDK.GetHitbox(&self->animator, 0);
    self->innerBox = RSDK.GetHitbox(&self->animator, 1);

    RSDK.ProcessTileCollisions(self, self->outerBox, self->innerBox);
    if (self->onGround) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 1, &self->animator, false, 0);
        self->state                   = MetalSonic_State_Taunt;
        self->animator.animationSpeed = 0;
        self->direction               = FLIP_NONE;
    }
}

void MetalSonic_State_Taunt(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 60) {
        self->timer                   = 0;
        self->animator.animationSpeed = 1;
        self->state                   = MetalSonic_State_GetReady;
    }
}

void MetalSonic_State_GetReady(void)
{
    RSDK_THIS(MetalSonic);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 2, &self->animator, false, 0);
        self->state = MetalSonic_State_SetupBounds;
    }
}

void MetalSonic_State_SetupBounds(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &self->animator2, false, 0);
        self->state = MetalSonic_State_Start;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
        Zone->screenBoundsR1[0] = size.x;
        Zone->screenBoundsT1[0] = 0;
    }
}

void MetalSonic_State_Start(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 90) {
        self->timer           = 0;
        Zone->screenBoundsL1[0] = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 8, &self->animator, false, 0);
        self->velocity.x = 0x18000;
        self->velocity.y = -0x60000;
        RSDK.PlaySfx(MetalSonic->sfxJump2, false, 255);
        self->state = MetalSonic_State_Unknown1;
    }
}

void MetalSonic_State_Unknown1(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y >= 0) {
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &self->animator2, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, false, 6);
        self->animator.animationSpeed = 0;
        self->targetPos.x             = player1->position.x;
        self->targetPos.y             = player1->position.y;
        self->velocity.x              = 0;
        self->velocity.y              = 0;
        self->timer2                  = 240;
        self->state                   = MetalSonic_State_Unknown2;
    }
}

void MetalSonic_State_Unknown2(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (--self->timer <= 0) {
        self->timer = 60;
        int angle     = RSDK.Rand(0, 256);
        int power     = RSDK.Rand(64, 97) << 8;

        self->targetPos.x = (power + (power >> 2)) * RSDK.Cos256(angle);
        self->targetPos.y = power * RSDK.Sin256(angle);
    }

    self->targetPos.x += player1->position.x;
    self->targetPos.y += player1->position.y;
    MetalSonic_HandleVelocity();
    MetalSonic_HandleAnimDir();

    if (!self->activeScreens) {
        if (self->velocity.x <= 0) {
            if (player1->velocity.x < self->field_80)
                self->field_80 = player1->velocity.x;

            int dist = (self->targetPos.x - self->position.x) >> 5;
            if (dist < self->field_80)
                self->field_80 = dist;

            if (self->position.x < player1->position.x + 244) {
                if (player1->velocity.x < 0) {
                    if (self->field_80 < 2 * player1->velocity.x) {
                        self->field_78 = 2 * player1->velocity.x;
                        self->field_80 = 2 * player1->velocity.x;
                    }
                }
            }
        }
        else {
            if (player1->velocity.x > self->field_80)
                self->field_80 = player1->velocity.x;

            int dist = (self->targetPos.x - self->position.x) >> 5;
            if (dist > self->field_80)
                self->field_80 = dist;

            if (self->position.x > player1->position.x - 244) {
                if (player1->velocity.x > 0) {
                    if (self->field_80 > 2 * player1->velocity.x) {
                        self->field_78 = 2 * player1->velocity.x;
                        self->field_80 = 2 * player1->velocity.x;
                    }
                }
            }
        }

        if (self->velocity.y <= 0) {
            if (player1->velocity.y < self->field_84)
                self->field_84 = player1->velocity.y;

            int dist = (self->targetPos.y - self->position.y) >> 5;
            if (dist < self->field_84)
                self->field_84 = dist;

            if (self->position.y < player1->position.y + 152) {
                if (player1->velocity.y < 0) {
                    if (self->field_84 < 2 * player1->velocity.y) {
                        self->field_7C = 2 * player1->velocity.y;
                        self->field_84 = 2 * player1->velocity.y;
                    }
                }
            }
        }
        else {
            if (player1->velocity.y > self->field_84)
                self->field_84 = player1->velocity.y;

            int dist = (self->targetPos.y - self->position.y) >> 5;
            if (dist > self->field_84)
                self->field_84 = dist;

            if (self->position.y > player1->position.y - 152) {
                if (player1->velocity.y > 0) {
                    if (self->field_84 > 2 * player1->velocity.y) {
                        self->field_7C = 2 * player1->velocity.y;
                        self->field_84 = 2 * player1->velocity.y;
                    }
                }
            }
        }

        self->timer2--;
        if (self->timer2 < 4)
            self->timer2 = 4;
    }
    else {
        self->field_80 = player1->velocity.x;
        self->field_84 = player1->velocity.y;
        self->timer2--;
    }

    if (self->timer2 <= 0) {
        self->attackType = RSDK.Rand(0, 6);

        while ((1 << self->attackType) & MetalSonic->field_C) {
            self->attackType = RSDK.Rand(0, 6);
        }

        MetalSonic->field_C |= 1 << self->attackType;
        if (MetalSonic->field_C == 0x3F)
            MetalSonic->field_C = 0;

        self->attackType >>= 1;

        switch (self->attackType) {
            case 0:
                if (RSDK.Rand(0, 2) != 0)
                    self->targetPos.x = (ScreenInfo->width + 72) << 16;
                else
                    self->targetPos.x = -0x480000;
                self->targetPos.y = ScreenInfo->centerY << 16;
                self->state       = MetalSonic_State_Unknown3;
                break;
            case 1:
                if (player1->velocity.x >= 0) {
                    self->targetPos.y = 0x200000;
                    self->targetPos.x = (ScreenInfo->width - 32) << 16;
                }
                else {
                    self->targetPos.x = 0x200000;
                    self->targetPos.y = 0x200000;
                }
                self->state = MetalSonic_State_Unknown3;
                break;
            case 2: {
                int angle           = (RSDK.Rand(0, 2) + 3) << 6;
                self->targetPos.x = 0xA000 * RSDK.Cos256(angle) + player1->position.x - (ScreenInfo->position.x << 16);
                self->targetPos.y = player1->position.y + 0x7000 * RSDK.Sin256(angle) - (ScreenInfo->position.y << 16);
                self->state       = MetalSonic_State_Unknown3;
                break;
            }
        }
    }
    else {
        self->targetPos.x -= player1->position.x;
        self->targetPos.y -= player1->position.y;

        if (self->field_78 != self->field_80) {
            if (self->field_78 <= self->field_80) {
                self->field_78 += (0xC00 << (self->field_78 < 0));
                if (self->field_78 > self->field_80)
                    self->field_78 = self->field_80;
            }
            else {
                self->field_78 -= (0xC00 << (self->field_78 > 0));
                if (self->field_78 < self->field_80)
                    self->field_78 = self->field_80;
            }
        }

        if (self->field_7C != self->field_84) {
            if (self->field_7C <= self->field_84) {
                self->field_7C += (0xC00 << (self->field_7C < 0));
                if (self->field_7C > self->field_84)
                    self->field_7C = self->field_84;
            }
            else {
                self->field_7C -= (0xC00 << (self->field_7C > 0));

                if (self->field_7C < self->field_84)
                    self->field_7C = self->field_84;
            }
        }

        self->position.x += self->field_78 + (self->velocity.x >> 1) + (self->velocity.x >> 3);
        self->position.y += self->field_7C + (self->velocity.y >> 1) + (self->velocity.y >> 3);
    }

    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_Unknown3(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->targetPos.x += ScreenInfo->position.x << 16;
    self->targetPos.y += ScreenInfo->position.y << 16;
    MetalSonic_HandleVelocity();

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->position.x += player1->velocity.x;
    self->position.y += player1->velocity.y;
    MetalSonic_HandleAnimDir();

    int rx              = self->position.x - self->targetPos.x;
    int ry              = self->position.y - self->targetPos.y;
    self->targetPos.x = self->targetPos.x - (ScreenInfo->position.x << 16);
    self->targetPos.y = self->targetPos.y - (ScreenInfo->position.y << 16);
    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 0x1000) {
        self->timer2 = 0;
        self->position.x &= 0xFFFF0000;
        self->position.y &= 0xFFFF0000;
        self->targetPos.x &= 0xFFFF0000;
        self->targetPos.y &= 0xFFFF0000;
        self->state = MetalSonic_State_StartAttack;
    }
    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_StartAttack(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->targetPos.x += ScreenInfo->position.x << 16;
    self->targetPos.y += ScreenInfo->position.y << 16;

    self->velocity.x = (self->targetPos.x - self->position.x) >> 4;
    self->velocity.y = (self->targetPos.y - self->position.y) >> 4;

    if (self->position.x >= self->targetPos.x)
        self->velocity.x -= 0x10000;
    else
        self->velocity.x += 0x10000;

    if (self->position.y >= self->targetPos.y)
        self->velocity.y += 0x10000;
    else
        self->velocity.y += 0x10000;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->position.x += player1->velocity.x;
    self->position.y += player1->velocity.y;

    uint8 flags = 0;
    if (self->velocity.x > 0 && self->position.x > self->targetPos.x) {
        flags              = 1;
        self->position.x = self->targetPos.x;
    }
    else if (self->velocity.x < 0 && self->position.x < self->targetPos.x) {
        flags              = 1;
        self->position.x = self->targetPos.x;
    }

    if (self->velocity.y > 0 && self->position.y > self->targetPos.y) {
        ++flags;
        self->position.y = self->targetPos.y;
    }
    else if (self->velocity.y < 0 && self->position.y < self->targetPos.y) {
        ++flags;
        self->position.y = self->targetPos.y;
    }

    self->targetPos.x = self->targetPos.x - (ScreenInfo->position.x << 16);
    self->targetPos.y = self->targetPos.y - (ScreenInfo->position.y << 16);
    MetalSonic_HandleAnimDir();

    if (flags == 2) {
        if (self->attackType) {
            if (self->attackType == 1) {
                self->timer2 = 60;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 7, &self->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 255);
                self->state = MetalSonic_State_Unknown7;
            }
            if (self->attackType == 2) {
                self->timer2      = 60;
                self->targetPos.x = self->position.x - player1->position.x;
                self->targetPos.y = self->position.y - player1->position.y;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 8, &self->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSBall, false, 255);
                self->state = MetalSonic_State_Unknown5;
            }
        }
        else {
            self->timer2 = 60;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 6, &self->animator, true, 0);
            RSDK.PlaySfx(MetalSonic->sfxMSChargeFire, false, 255);
            self->state = MetalSonic_State_Unknown9;
        }
    }
    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_Unknown5(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = player1->position.x + self->targetPos.x;
    self->position.y = player1->position.y + self->targetPos.y;

    int angle          = RSDK.ATan2(player1->position.x - self->position.x, player1->position.y - self->position.y);
    self->field_9C.x = (RSDK.Cos256(angle + 192) << 12) + self->position.x;
    self->field_9C.y = (RSDK.Sin256(angle + 192) << 12) + self->position.y;
    self->field_A4.x = (RSDK.Cos256(angle + 64) << 12) + self->position.x;
    self->field_A4.y = (RSDK.Sin256(angle + 64) << 12) + self->position.y;
    self->field_AC.x = (RSDK.Cos256(angle) << 14) + self->position.x;
    self->field_AC.y = (RSDK.Sin256(angle) << 14) + self->position.y;

    if (--self->timer2 <= 0) {
        self->field_9C.x = -1;
        self->targetPos  = player1->position;
        self->velocity.x = 0xA00 * RSDK.Cos256(angle);
        self->velocity.y = 0xA00 * RSDK.Sin256(angle);
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);
        self->timer2 = 15;
        self->state  = MetalSonic_State_Unknown6;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}

void MetalSonic_State_Unknown6(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (!self->activeScreens && --self->timer2 <= 0) {
        self->targetPos  = player1->position;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->timer      = 0;
        self->timer2     = 150;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, true, 0);
        self->state = MetalSonic_State_Unknown2;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}

void MetalSonic_State_Unknown7(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = self->targetPos.x + (ScreenInfo->position.x << 16);
    self->position.y = self->targetPos.y + (ScreenInfo->position.y << 16);

    if (--self->timer2 <= 0) {
        self->targetPos.x = player1->position.x;
        self->targetPos.y = player1->position.y;
        RSDK.ATan2(self->targetPos.x - self->position.x, self->targetPos.y - self->position.y);

        self->velocity.y = 0x88000;
        if (self->position.x < player1->position.x)
            self->velocity.x = 0x40000;
        else
            self->velocity.x = -0x40000;
        self->timer2 = 15;
        self->state  = MetalSonic_State_Unknown8;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}

void MetalSonic_State_Unknown8(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;
    if (self->position.y < player1->position.y && !self->activeScreens && --self->timer2 <= 0) {
        self->targetPos  = player1->position;
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->timer      = 0;
        self->timer2     = 150;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, true, 0);
        self->state = MetalSonic_State_Unknown2;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}

void MetalSonic_State_Unknown9(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = self->targetPos.x + (ScreenInfo->position.x << 16);
    self->position.y = self->targetPos.y + (ScreenInfo->position.y << 16);

    int angle          = RSDK.ATan2(player1->position.x - self->position.x, player1->position.y - self->position.y);
    self->field_9C.x = (RSDK.Cos256(angle + 192) << 12) + self->position.x;
    self->field_9C.y = (RSDK.Sin256(angle + 192) << 12) + self->position.y;
    self->field_A4.x = (RSDK.Cos256(angle + 64) << 12) + self->position.x;
    self->field_A4.y = (RSDK.Sin256(angle + 64) << 12) + self->position.y;
    self->field_AC.x = (RSDK.Cos256(angle) << 15) + self->position.x;
    self->field_AC.y = (RSDK.Sin256(angle) << 15) + self->position.y;
    if (--self->timer2 <= 0) {
        self->field_9C.x = -1;
        self->targetPos  = player1->position;
        self->velocity.x = 0xC00 * RSDK.Cos256(angle);
        self->velocity.y = 0xC00 * RSDK.Sin256(angle);
        RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);
        self->timer2 = 15;
        self->state  = MetalSonic_State_Unknown10;
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}

void MetalSonic_State_Unknown10(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    bool32 flag = true;
    if (self->velocity.x <= 0) {
        if (self->velocity.x >= 0 || self->position.x >= player1->position.x) {
            flag = false;
        }
    }
    else if (self->position.x <= player1->position.x) {
        if (self->velocity.x >= 0 || self->position.x >= player1->position.x) {
            flag = false;
        }
    }

    if (flag) {
        if (!self->activeScreens && --self->timer2 <= 0) {
            self->targetPos  = player1->position;
            self->velocity.x = 0;
            self->velocity.y = 0;
            self->timer      = 0;
            self->timer2     = 150;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, true, 0);
            self->state = MetalSonic_State_Unknown2;
        }
    }

    MetalSonic_HandleStageWrap();
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}

void MetalSonic_State_EnterPanel(void)
{
    RSDK_THIS(MetalSonic);

    int velX = self->velocity.x;
    MetalSonic_HandleVelocity();

    if (self->velocity.x < 0x20000 && velX >= 0x20000) {
        self->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }
    else if (self->velocity.x > -0x20000 && velX <= -0x20000) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }

    ++self->timer;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    int rx = self->position.x - self->targetPos.x;
    int ry = self->position.y - self->targetPos.y;
    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 4096 && self->timer > 96) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, false, 0);
        self->state = MetalSonic_State_StartPanelSeq;
    }
}

void MetalSonic_State_StartPanelSeq(void)
{
    RSDK_THIS(MetalSonic);

    ++self->timer;
    self->position.x += (self->targetPos.x - self->position.x) >> 4;
    self->position.y += (self->targetPos.y - self->position.y) >> 4;
    if (self->timer == 32)
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 9, &self->animator, true, 0);
    if (self->timer == 59)
        RSDK.PlaySfx(MetalSonic->sfxSpecialRing, false, 255);

    if (self->timer == 64) {
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

        EntityMSPanel *panel = (EntityMSPanel *)self->panel;
        self->position     = self->targetPos;
        self->timer        = 16;
        self->timer2       = RETRO_USE_PLUS ? 240 : 180;
        self->health       = RETRO_USE_PLUS ? 6 : 4;
        self->state        = MetalSonic_State_ShowFactory;
        panel->state         = MSPanel_Unknown2;
    }
}

void MetalSonic_HandlePanelAttack(void)
{
    RSDK_THIS(MetalSonic);

#if RETRO_USE_PLUS
    if (!RSDK.GetEntityCount(MSBomb->objectID, true))
#endif
        --self->timer2;
    if (self->timer2 == 60) {
        RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 255);
        self->field_94 = 60;

#if RETRO_USE_PLUS
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, self, self->position.x, self->position.y);
        ring->field_78         = 24;
        ring->timer            = 24;
        ring->r                = 0x00;
        ring->g                = 0x80;
        ring->b                = 0xF0;
        ring->field_70         = 1;
#endif
    }
    if (self->timer2 <= 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        self->timer2   = 45 * self->health + (RETRO_USE_PLUS ? 195 : 135);
        int angle        = RSDK.ATan2(player1->position.x - self->position.x, player1->position.y - self->position.y);
        EntityMSOrb *orb = CREATE_ENTITY(MSOrb, NULL, self->position.x, self->position.y);
        orb->velocity.x  = 0x280 * RSDK.Cos256(angle);
        orb->velocity.y  = 0x280 * RSDK.Sin256(angle);
        RSDK.PlaySfx(MetalSonic->sfxMSShoot, false, 255);
    }
}

void MetalSonic_State_ShowFactory(void)
{
    RSDK_THIS(MetalSonic);

    ++self->timer;
    MetalSonic_HandlePanelAttack();
    if (self->timer == 60) {
        foreach_active(MSFactory, factory)
        {
            factory->visible = true;
            factory->state   = MSFactory_Unknown3;
        }

        self->timer = 0;
        self->state = MetalSonic_State_Unknown13;
    }
}

void MetalSonic_State_Unknown13(void)
{
    RSDK_THIS(MetalSonic);

    ++self->timer;
    MetalSonic_HandlePanelAttack();

#if RETRO_USE_PLUS
    if (!RSDK.GetEntityCount(SilverSonic->objectID, true) && !RSDK.GetEntityCount(MSBomb->objectID, true) && self->timer > 60) {
#else
    if (!RSDK.GetEntityCount(SilverSonic->objectID, true) && self->timer > 60) {
#endif
        self->timer = 0;
        self->state = MetalSonic_State_ShowFactory;
    }
}

void MetalSonic_State_PanelExplosion(void)
{
    RSDK_THIS(MetalSonic);
    EntityMSPanel *panel = (EntityMSPanel *)self->panel;

    self->position.y += 0x8000;
    panel->position.y += 0x8000;
    if (!self->timer)
        panel->state = MSPanel_Unknown3;
    if (++self->timer == 104) {
        self->timer = 0;
        panel->state  = MSPanel_Unknown4;
        self->state = MetalSonic_State_Unknown14;
    }
}

void MetalSonic_State_Unknown14(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, false, 3);
        self->velocity.x = 0x18000;
        self->onGround   = false;
        self->state      = MetalSonic_State_Unknown15;
    }
}

void MetalSonic_State_Unknown15(void)
{
    RSDK_THIS(MetalSonic);

    self->velocity.y += 0x2000;
    self->outerBox = RSDK.GetHitbox(&self->animator, 0);
    self->innerBox = RSDK.GetHitbox(&self->animator, 1);
    RSDK.ProcessTileCollisions(self, self->outerBox, self->innerBox);

    if (self->onGround) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 2, &self->animator, false, 0);
        self->direction = FLIP_NONE;
        self->groundVel = 0;
    }

    if (++self->timer == 120) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->wait         = 32;
        fxFade->speedOut     = 8;
        EntityMSPanel *panel = (EntityMSPanel *)self->panel;
        panel->state         = StateMachine_None;
        self->timer        = 0;
        self->timer2       = 240;
        self->state        = MetalSonic_State_Unknown2;
        foreach_active(Player, player)
        {
            RSDK.PlaySfx(SpeedBooster->sfxID, false, 255);
            player->velocity.x  = 0xE0000;
            player->groundVel   = 0xE0000;
            player->controlLock = 60;
            player->direction   = FLIP_NONE;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
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

    int velX = self->velocity.x;
    MetalSonic_HandleVelocity();

    if (self->velocity.x < 0x20000 && velX >= 0x20000) {
        self->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }
    else if (self->velocity.x > -0x20000 && velX <= -0x20000) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }

    ++self->timer;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    int rx = self->position.x - self->targetPos.x;
    int ry = self->position.y - self->targetPos.y;

    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 4096 && self->timer > 96) {
        self->timer     = 0;
        self->direction = FLIP_NONE;
        self->state     = MetalSonic_State_ObtainRuby;
    }
}

void MetalSonic_State_ObtainRuby(void)
{
    RSDK_THIS(MetalSonic);

    self->targetPos.y += RSDK.Sin256(4 * Zone->timer) << 8;

    self->position.x += (self->targetPos.x - self->position.x) >> 4;
    self->position.y += (self->targetPos.y - self->position.y) >> 4;

    foreach_active(PhantomRuby, ruby)
    {
        int rx = (self->position.x - ruby->position.x) >> 16;
        int ry = (self->position.y - ruby->position.y) >> 16;
        if (rx * rx + ry * ry < 0x100) {
            ruby->startPos.x = ruby->position.x;
            ruby->startPos.y = ruby->position.y;
            ruby->state      = PhantomRuby_Unknown5;
            self->state    = MetalSonic_State_Transform;
            RSDK.PlaySfx(MetalSonic->sfxMSTransform, false, 255);
        }
    }
}

void MetalSonic_State_Transform(void)
{
    RSDK_THIS(MetalSonic);

    self->position.y -= 0x2000;
    foreach_active(PhantomRuby, ruby) { ruby->startPos.y -= 0x2000; }

    if (++self->timer == 30) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0xFFFF, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 7, &self->animator, true, 0);
    }

    int timer = self->timer >> 1;
    if (timer > 96)
        timer = 96;

    if ((self->timer & 0x1F) == 30) {
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, self, self->position.x, self->position.y);
        ring->field_78         = timer;
        ring->timer            = 24;
        ring->r                = 0x00;
        ring->g                = 0xF0;
        ring->b                = 0xF0;
        ring->field_70         = 2;
    }

    if (timer >= 32 && (self->timer & 0xF) == 14) {
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, self, self->position.x, self->position.y);
        ring->field_78         = timer + 64;
        ring->timer            = 24;
        ring->r                = 0xF0;
        ring->g                = 0xF0;
        ring->b                = 0x60;
        ring->field_70         = 4;
    }

    if (self->timer == 240) {
        self->timer        = 0;
        self->active       = ACTIVE_NEVER;
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->wait         = 32;
        fxFade->speedOut     = 8;
        RSDK.PlaySfx(MetalSonic->sfxTransform2, false, 255);
    }
}

void MetalSonic_State_Defeated(void)
{
    RSDK_THIS(MetalSonic);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= true;
    if (!RSDK.CheckOnScreen(self, NULL)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), (ScreenInfo->position.x + ScreenInfo->centerX) << 16,
                                                (ScreenInfo->position.y - 48) << 16);
        prison->velocity.x      = 0x10000;
        prison->active          = ACTIVE_NORMAL;
        destroyEntity(self);
    }
}
#else
void MetalSonic_State_Unknown16(void)
{
    RSDK_THIS(MetalSonic);

    MetalSonic_HandleVelocity();
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    MetalSonic_HandleStageWrap();
    self->timer++;

    EntityPlatform *wall = NULL;
    switch (self->timer) {
        case 60: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform); break;
        case 90: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 3, Platform); break;
        case 120: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, Platform); break;
        case 150: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 5, Platform); break;
        case 180: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, Platform); break;
        case 210: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, Platform); break;
        case 240: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 8, Platform); break;
        case 270: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 9, Platform); break;
        case 300: {
            EntityPlatform *startWall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);
            self->targetPos.x = startWall->position.x + 0x800000;
            self->targetPos.y = (Zone->screenBoundsB1[0] << 16) - 0x340000;
            self->state = MetalSonic_State_Unknown17;
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
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    MetalSonic_HandleAnimDir();

    int rx = self->position.x - self->targetPos.x;
    int ry = self->position.y - self->targetPos.y;
    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 0x1000) {
        self->timer2 = 0;
        self->position.x &= 0xFFFF0000;
        self->position.y &= 0xFFFF0000;
        self->targetPos.x &= 0xFFFF0000;
        self->targetPos.y &= 0xFFFF0000;
        self->state = MetalSonic_State_Unknown18;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown18(void)
{
    RSDK_THIS(MetalSonic);

    self->velocity.x = (self->targetPos.x - self->position.x) >> 4;
    self->velocity.y = (self->targetPos.y - self->position.y) >> 4;

    if (self->position.x >= self->targetPos.x)
        self->velocity.x -= 0x10000;
    else
        self->velocity.x += 0x10000;

    if (self->position.y >= self->targetPos.y)
        self->velocity.y -= 0x10000;
    else
        self->velocity.y += 0x10000;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    uint8 flags = 0;
    if (self->velocity.x > 0) {
        if (self->position.x > self->targetPos.x) {
            flags = 1;
            self->position.x = self->targetPos.x;
        }
    }
    else if (self->position.x < self->targetPos.x) {
        flags = 1;
        self->position.x = self->targetPos.x;
    }

    if (self->velocity.y > 0) {
        if (self->position.y > self->targetPos.y) {
            ++flags;
            self->position.y = self->targetPos.y;
        }
    }
    else if (self->position.y < self->targetPos.y) {
        ++flags;
        self->position.y = self->targetPos.y;
    }

    MetalSonic_HandleAnimDir();
    if (flags == 2) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 1, &self->animator, false, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, false, 0);
        self->direction = 0;
        self->state = MetalSonic_State_Unknown19;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown19(void)
{
    RSDK_THIS(MetalSonic);

    if (self->animator.frameID == self->animator.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 2, &self->animator, false, 0);
        self->state = MetalSonic_State_Unknown20;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown20(void)
{
    RSDK_THIS(MetalSonic);

    self->timer++;
    if (self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &self->animator2, false, 0);
        self->state = MetalSonic_State_Unknown21;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown21(void)
{
    RSDK_THIS(MetalSonic);

    self->timer++;
    if (self->timer == 90) {
        int slot = SceneInfo->entitySlot;
        RSDK_GET_ENTITY(slot + 2, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 3, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 4, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 5, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 6, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 7, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 8, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 9, Platform)->state = MetalSonic_StateWall_Move;
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, false, 0);
        self->state = MetalSonic_State_Unknown22;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown22(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

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
            self->health = 8;
            self->timer2 = 120;
            self->state = MetalSonic_State_Unknown23;
        }
    }

    self->velocity.x = wall->velocity.x;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Unknown23(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    if (player1->position.x <= self->position.x) {
        if (self->velocity.x >= player1->velocity.x - 0x60000) {
            self->velocity.x -= 0xC00;
        }
        else
            self->velocity.x += 0xC00;
    }
    else if (self->velocity.x < 0x60000)
        self->velocity.x += 0xC00;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x140000, true))
        self->velocity.y = 0;
    if (self->direction == FLIP_X) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }
    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x800000)
        self->position.x = wall->position.x + 0x800000;

    if (--self->timer2 <= 0 && !player1->blinkTimer) {
        if (self->velocity.x < 0x60000)
            self->velocity.x = 0x60000;
        self->timer2 = 120;
        self->attackType = RSDK.Rand(0, 6);

        while ((1 << self->attackType) & MetalSonic->field_C) {
            self->attackType = RSDK.Rand(0, 6);
        }
        MetalSonic->field_C |= 1 << self->attackType;
        if (MetalSonic->field_C == 0x3F)
            MetalSonic->field_C = 0;

        self->attackType >>= 1;
        switch (self->attackType) {
            case 2:
                self->timer2 = 60;
                self->targetPos.x = (RSDK.Rand(-3, 7) << 14) + 0x78000;
                self->targetPos.y = -0x80000;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 8, &self->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSBall, false, 0xFF);
                self->state = MetalSonic_State_Unknown24;
                break;
            case 1:
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 7, &self->animator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 0xFF);
                self->state = MetalSonic_State_Unknown27;
                break;
            case 0:
                RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 0xFF);
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 6, &self->animator, false, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, false, 0);
                self->timer2 = 60;
                self->state = MetalSonic_State_Unknown30;
                break;
        }
    }
    MetalSonic_CheckPlayerCollisions();
}
void MetalSonic_State_Unknown27(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    if (self->velocity.x < 0xB0000)
        self->velocity.x += 0xC00;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y <= (ScreenInfo->centerY + ScreenInfo->position.y) << 16) {
        if (self->velocity.y < 0x10000)
            self->velocity.y += 0x1000;
    }
    else {
        if (self->velocity.y >= -0x10000)
            self->velocity.y -= 0x1000;
    }

    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000)
        self->position.x = wall->position.x + 0x410000;

    if (self->position.x > ((ScreenInfo->width + ScreenInfo->position.x) << 16) + 0x400000) {
        self->direction = FLIP_X;
        self->state = MetalSonic_State_Unknown28;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}
void MetalSonic_State_Unknown28(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->velocity.x = player1->velocity.x - 0x10000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y <= (ScreenInfo->centerY + ScreenInfo->position.y) << 16) {
        if (self->velocity.y < 0x10000)
            self->velocity.y += 0x1000;
    }
    else {
        if (self->velocity.y >= -0x10000)
            self->velocity.y -= 0x1000;
    }

    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000) {
        self->position.x = wall->position.x + 0x410000;
    }

    if (self->position.x < ((ScreenInfo->position.x + ScreenInfo->width) << 16) - 0x180000) {
        self->timer2 = 120;
        self->position.x = ((ScreenInfo->position.x + ScreenInfo->width) << 16) - 0x180000;
        self->state = MetalSonic_State_Unknown29;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}
void MetalSonic_State_Unknown29(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->velocity.x = player1->velocity.x;
    self->position.x = ((ScreenInfo->position.x + ScreenInfo->width) << 16) - 0x180000;
    self->position.y += self->velocity.y;

    if (self->position.y <= (ScreenInfo->centerY + ScreenInfo->position.y) << 16) {
        if (self->velocity.y < 0x10000)
            self->velocity.y += 0x1000;
    }
    else {
        if (self->velocity.y >= -0x10000)
            self->velocity.y -= 0x1000;
    }

    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000)
        self->position.x = wall->position.x + 0x410000;

    switch (--self->timer2) {
        case 0:
            self->timer2 = 120;
            self->velocity.y = 0x10000;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, false, 0);
            self->state = MetalSonic_State_Unknown26;
            break;
        case 30:
        case 60:
        case 90: {
            EntityMSOrb *orb = CREATE_ENTITY(MSOrb, NULL, self->position.x, self->position.y);
            orb->velocity.x = player1->velocity.x - 0x40000;
            orb->velocity.y = (RSDK.Rand(-1, 2) + 2) << 16;
            RSDK.PlaySfx(MetalSonic->sfxMSShoot, false, 0xFF);
            break;
        }
        default: break;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}
void MetalSonic_State_Unknown24(void)
{
    RSDK_THIS(MetalSonic);
    int angle = RSDK.ATan2(self->targetPos.x, self->targetPos.y);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->field_9C.x = self->position.x + (RSDK.Cos256(angle + 192) << 12);
    self->field_9C.y = self->position.y + (RSDK.Sin256(angle - 64) << 12);

    self->field_A4.x = self->position.x + (RSDK.Cos256(angle + 64) << 12);
    self->field_A4.y = self->position.y + (RSDK.Sin256(angle + 64) << 12);

    self->field_AC.x = self->position.x + (RSDK.Cos256(angle) << 14);
    self->field_AC.y = self->position.y + (RSDK.Sin256(angle) << 14);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000)
        self->position.x = wall->position.x + 0x410000;

    self->timer2--;
    if (self->timer2 <= 0) {
        self->field_9C.x = -1;
        self->timer2 = 8;
        self->velocity = self->targetPos;
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 0xFF);
        self->state = MetalSonic_State_Unknown25;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}
void MetalSonic_State_Unknown25(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->position.y >= 0x1F00000) {
        if (self->position.x > ((ScreenInfo->width + ScreenInfo->position.x) << 16) + 0x400000 || !--self->timer2) {
            self->timer2 = 120;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, false, 2);
            self->state = MetalSonic_State_Unknown26;
        }
        else {
            self->velocity.y = -0x80000;
            RSDK.PlaySfx(MetalSonic->sfxRockemSockem, false, 0xFF);
        }
    }
    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000)
        self->position.x = wall->position.x + 0x410000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}
void MetalSonic_State_Unknown30(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    if (self->velocity.x < 0xD0000)
        self->velocity.x += 0x1000;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000)
        self->position.x = wall->position.x + 0x410000;

    if (self->timer2) {
        self->timer2--;
    }
    else if (self->position.x > player1->position.x + 0x400000) {
        self->timer2 = 120;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 5, &self->animator, false, 2);
        self->state = MetalSonic_State_Unknown26;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitbox1)) {
            Player_CheckHit(player, self);
        }
    }
}
void MetalSonic_State_Unknown26(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (self->velocity.x > 0x20000)
        self->velocity.x -= 0x1800;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y >= 0x1EC0000) {
        self->position.y = 0x1EC0000;
        self->velocity.y = 0;
    }
    if (self->position.x < player1->position.x && self->direction == FLIP_X) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 4, &self->animator, true, 0);
    }
    MetalSonic_HandleStageWrap();

    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);
    if (self->position.x < wall->position.x + 0x900000) {
        self->position.x = wall->position.x + 0x900000;
        self->timer2 = 120;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 11, &self->animator2, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 3, &self->animator, false, 0);
        self->state = MetalSonic_State_Unknown23;
    }
    MetalSonic_CheckPlayerCollisions();
}
void MetalSonic_State_Explode(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlatform *wall1 = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.x -= 0x1000;
    self->rotation += 6;
    MetalSonic_Explode();
    MetalSonic_HandleStageWrap();

    if (self->position.x < wall1->position.x + 0x20000) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 0xFF);

        for (int i = 2; i < 10; ++i) {
            EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + i, Platform);
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
        self->velocity.x = 0x50000;
        self->velocity.y = -0x30000;
        self->rotation = 0;
        self->drawFX &= ~FX_ROTATE;
        self->state = MetalSonic_State_Defeated;
    }
}
void MetalSonic_State_Defeated(void)
{
    RSDK_THIS(MetalSonic);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    self->visible ^= true;
    MetalSonic_HandleStageWrap();

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->visible = true;
        self->timer = 90;
        self->state = MetalSonic_State_Finish;
    }
}
void MetalSonic_State_Finish(void)
{
    RSDK_THIS(MetalSonic);
    MetalSonic_HandleStageWrap();

    if (!--self->timer) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        Zone->screenBoundsL1[0] = ScreenInfo->position.x;
        Zone->screenBoundsR1[0] = ScreenInfo->width + ScreenInfo->position.x;
        Zone->screenBoundsR2[0] = (ScreenInfo->width + ScreenInfo->position.x) << 16;
        Zone->screenBoundsL2[0] = ScreenInfo->position.x << 16;

        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), (ScreenInfo->position.x + ScreenInfo->centerX) << 16,
                                                (ScreenInfo->position.y - 48) << 16);
        prison->velocity.x = 0x10000;
        prison->active = ACTIVE_NORMAL;
        self->state = MetalSonic_State_None;
    }
}
void MetalSonic_State_None(void) {}

void MetalSonic_StateWall_Fall(void)
{
    RSDK_THIS(Platform);

    self->drawPos.y += self->velocity.y;
    self->centerPos.y = self->drawPos.y;
    self->position.y = self->drawPos.y;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, (self->speed << 21) + 0x100000, true)) {
        self->velocity.y = 0;
        self->drawPos.y = self->position.y;
        self->centerPos.y = self->position.y;
        self->state = Platform_State_Fixed;
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
        self->type = 120;
        self->type--;
        if (self->velocity.x > 0x40000)
            self->velocity.x -= 0x1000;
    }
    else {
        if (self->type <= 0) {
            if (self->velocity.x < 0x54000) {
                self->velocity.x += 0x400;
            }
            else {
                self->type--;
                if (self->velocity.x > 0x40000)
                    self->velocity.x -= 0x1000;
            }
        }
        else {
            self->type--;
            if (self->velocity.x > 0x40000)
                self->velocity.x -= 0x1000;
        }
    }

    Zone->autoScrollSpeed = self->velocity.x;
    self->drawPos.x += self->velocity.x;
    self->centerPos.x = self->drawPos.x;
    self->position.x = self->drawPos.x;

    int x = player1->position.x - 0x2000000;
    if (x < self->position.x - 0x2000000 || x > self->position.x) {
        self->drawPos.x = x;
        self->centerPos.x = x;
        self->position.x = x;
    }
    if (self->speed == 1) {
        EntityPlatform *belowPlat = RSDK_GET_ENTITY(SceneInfo->entitySlot - 1, Platform);
        belowPlat->drawPos.x = self->position.x;
        belowPlat->centerPos.x = self->position.x;
        belowPlat->position.x = self->position.x;
    }
}
#endif

#if RETRO_INCLUDE_EDITOR
void MetalSonic_EditorDraw(void)
{
    RSDK_THIS(MetalSonic);
    Vector2 drawPos;

    self->drawFX         = FX_FLIP | FX_ROTATE;
    self->updateRange.x  = 0x800000;
    self->updateRange.y  = 0x800000;
    self->tileCollisions = true;
    RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &self->animator, false, 0);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->position.x < 0x8000000)
        drawPos.x += 0xE000000;
    else
        drawPos.x -= 0xE000000;

    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator, &drawPos, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void MetalSonic_EditorLoad(void) { MetalSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE); }
#endif

void MetalSonic_Serialize(void) {}
