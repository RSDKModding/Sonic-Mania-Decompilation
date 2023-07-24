// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MetalSonic Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectMetalSonic *MetalSonic;

void MetalSonic_Update(void)
{
    RSDK_THIS(MetalSonic);

    if (self->invincibilityTimer)
        self->invincibilityTimer--;

    RSDK.ProcessAnimation(&self->metalSonicAnimator);
    RSDK.ProcessAnimation(&self->boosterAnimator);

    StateMachine_Run(self->state);

    if (MetalSonic->invincibilityTimerPanel > 0)
        MetalSonic->invincibilityTimerPanel--;

#if MANIA_USE_PLUS
    foreach_active(StarPost, post) { post->bonusStageID = 0; }
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

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 240, 0, 240, 8);

        RSDK.DrawSprite(&self->boosterAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->metalSonicAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->boosterAnimator, NULL, false);
        RSDK.DrawSprite(&self->metalSonicAnimator, NULL, false);

        RSDK.CopyPalette(1, 240, 0, 240, 8);
    }
    else {
        RSDK.DrawSprite(&self->boosterAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->metalSonicAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->boosterAnimator, NULL, false);
        RSDK.DrawSprite(&self->metalSonicAnimator, NULL, false);
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
            Zone->autoScrollSpeed = 0;
            self->active          = ACTIVE_BOUNDS;
            self->drawFX          = FX_FLIP | FX_ROTATE;
            self->visible         = false;
            self->updateRange.x   = 0x800000;
            self->updateRange.y   = 0x800000;
            self->tileCollisions  = TILECOLLISION_DOWN;
            self->collisionLayers = Zone->collisionLayers;

            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, 0, &self->metalSonicAnimator, true, 0);
            self->drawGroup = Zone->objectDrawGroup[0];
            self->state     = MetalSonic_State_SetupArena;
        }
    }
}

void MetalSonic_StageLoad(void)
{
    MetalSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE);

    MetalSonic->hitboxHover.left   = -8;
    MetalSonic->hitboxHover.top    = -8;
    MetalSonic->hitboxHover.right  = 8;
    MetalSonic->hitboxHover.bottom = 8;

    MetalSonic->hitboxDash.left   = -16;
    MetalSonic->hitboxDash.top    = -8;
    MetalSonic->hitboxDash.right  = 16;
    MetalSonic->hitboxDash.bottom = 8;

    MetalSonic->sfxHit          = RSDK.GetSfx("Stage/BossHit.wav");
    MetalSonic->sfxExplosion2   = RSDK.GetSfx("Stage/Explosion2.wav");
    MetalSonic->sfxExplosion3   = RSDK.GetSfx("Stage/Explosion3.wav");
    MetalSonic->sfxRumble       = RSDK.GetSfx("Stage/Rumble.wav");
    MetalSonic->sfxJump2        = RSDK.GetSfx("Stage/Jump2.wav");
    MetalSonic->sfxSpecialRing  = RSDK.GetSfx("Global/SpecialRing.wav");
    MetalSonic->sfxMSElecPulse  = RSDK.GetSfx("SSZ2/MSElecPulse.wav");
    MetalSonic->sfxMSBall       = RSDK.GetSfx("SSZ2/MSBall.wav");
    MetalSonic->sfxMSFireball   = RSDK.GetSfx("SSZ2/MSFireball.wav");
    MetalSonic->sfxBeep3        = RSDK.GetSfx("Stage/Beep3.wav");
    MetalSonic->sfxBeep4        = RSDK.GetSfx("Stage/Beep4.wav");
    MetalSonic->sfxRockemSockem = RSDK.GetSfx("Stage/RockemSockem.wav");
    MetalSonic->sfxMSShoot      = RSDK.GetSfx("SSZ2/MSShoot.wav");
    MetalSonic->sfxMSChargeFire = RSDK.GetSfx("SSZ2/MSChargeFire.wav");
#if MANIA_USE_PLUS
    MetalSonic->sfxMSTransform = RSDK.GetSfx("SSZ2/MSTransform.wav");
    MetalSonic->sfxTransform2  = RSDK.GetSfx("Stage/Transform2.wav");
#endif
}

void MetalSonic_HandleStageWrap(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

#if !MANIA_USE_PLUS
    EntityMetalSonic *marker = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, MetalSonic);
    EntityPlatform *wall     = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);
    if (player->position.y < marker->position.y && !player->collisionPlane) {
        Zone->cameraBoundsT[0] = (marker->position.y >> 16) + 16 - ScreenInfo->size.y;
        Zone->cameraBoundsB[0] = (marker->position.y >> 16) + 16;
        Zone->deathBoundary[0] = (marker->position.y >> 16) + 16;
        marker->position.y     = -0x200000;

        for (int32 i = 0; i < PLAYER_COUNT; ++i) {
            Zone->cameraBoundsL[i]      = (wall->position.x >> 16) - 95;
            Zone->cameraBoundsR[i]      = (wall->position.x >> 16) + 392;
            Zone->playerBoundActiveL[i] = true;
            Zone->playerBoundActiveR[i] = true;
        }

        self->timer       = 59;
        self->targetPos.x = 0x9100000;
        self->targetPos.y = 0x23400000;

        int32 anim = MS_ANI_IDLE;
        if (self->position.x >= 0x9100000) {
            if (self->direction != FLIP_X) {
                self->direction = FLIP_X;
                anim            = MS_ANI_HOVERTURN;
            }
            else {
                anim = MS_ANI_HOVER;
            }
        }
        else if (self->direction) {
            self->direction = FLIP_NONE;
            anim            = MS_ANI_HOVERTURN;
        }
        else {
            anim = MS_ANI_HOVER;
        }

        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, anim, &self->metalSonicAnimator, true, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_WEAK, &self->boosterAnimator, false, 0);
        self->state = MetalSonic_State_SetupSpikeWall;
    }
#endif

    if (camera->position.x < 0xF000000) {
        if (camera->position.x <= 0x1000000) {
            player->position.x += 0xE000000;
            self->position.x += 0xE000000;
            camera->position.x += 0xE000000;
            camera->center.x += 0xE00;
            MetalSonic_ProcessBGParallax(-0xE0000);

#if MANIA_USE_PLUS
            if (self->classID == GigaMetal->classID) {
                camera->boundsL += 0xE00;
                camera->boundsR += 0xE00;
                Zone->cameraBoundsL[0] += 0xE00;
                Zone->cameraBoundsR[0] += 0xE00;
                Zone->playerBoundsL[0] += 0xE000000;
                Zone->playerBoundsR[0] += 0xE000000;
            }
#endif

            foreach_active(MSOrb, orb) { orb->position.x += 0xE000000; }

#if MANIA_USE_PLUS
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
                spotlight->originPos.x += 0xE000000;
            }

            foreach_active(ImageTrail, trail)
            {
                trail->position.x += 0xE000000;
                trail->currentPos.x += 0xE000000;
                for (int32 i = 0; i < IMAGETRAIL_TRACK_COUNT; ++i) trail->statePos[i].x += 0xE000000;
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

#if MANIA_USE_PLUS
        if (self->classID == GigaMetal->classID) {
            camera->boundsL -= 0xE00;
            camera->boundsR -= 0xE00;
            Zone->cameraBoundsL[0] -= 0xE00;
            Zone->cameraBoundsR[0] -= 0xE00;
            Zone->playerBoundsL[0] -= 0xE000000;
            Zone->playerBoundsR[0] -= 0xE000000;
        }
#endif

        foreach_active(MSOrb, orb) { orb->position.x -= 0xE000000; }

#if MANIA_USE_PLUS
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
            spotlight->originPos.x -= 0xE000000;
        }

        foreach_active(ImageTrail, trail)
        {
            trail->position.x -= 0xE000000;
            trail->currentPos.x -= 0xE000000;
            for (int32 i = 0; i < IMAGETRAIL_TRACK_COUNT; ++i) trail->statePos[i].x -= 0xE000000;
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
        TileLayer *background = RSDK.GetTileLayer(i);
        for (int32 s = 0; s < background->scrollInfoCount; ++s) {
            background->scrollInfo[s].scrollPos += mult * background->scrollInfo[s].parallaxFactor;
        }
    }

    TileLayer *tower = RSDK.GetTileLayer(2);
    tower->scrollPos += mult * tower->parallaxFactor;

    foreach_active(EggTower, eggTower) { eggTower->rotationX += (mult >> 8); }
}

void MetalSonic_HandleVelocity(void)
{
    RSDK_THIS(MetalSonic);

    int32 angle = RSDK.ATan2((self->targetPos.x - self->position.x) >> 16, (self->targetPos.y - self->position.y) >> 16);
    int32 ang   = 2 * angle - self->angle;

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

    if (self->metalSonicAnimator.animationID == MS_ANI_HOVERTURN) {
        if (self->metalSonicAnimator.frameID == self->metalSonicAnimator.frameCount - 1)
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, true, 0);
    }
    else if (self->position.x <= player1->position.x) {
        if (self->direction == FLIP_X) {
            self->direction = FLIP_NONE;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
        }
    }
    else if (self->direction == FLIP_NONE) {
        self->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }
}

#if !MANIA_USE_PLUS
void MetalSonic_CheckPlayerCollisions(void)
{
    RSDK_THIS(MetalSonic);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer) {
            Hitbox *hitbox = self->metalSonicAnimator.animationID == MS_ANI_FLY ? &MetalSonic->hitboxDash : &MetalSonic->hitboxHover;

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
        self->timer            = 180;
        SceneInfo->timeEnabled = false;
        self->velocity.y       = -0x1800;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);

        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_DEFEATED, &self->metalSonicAnimator, false, 0);
        RSDK.SetSpriteAnimation(-1, MS_ANI_BOOSTER_INTRO, &self->boosterAnimator, false, 0);
        self->drawFX |= FX_ROTATE;
        self->state = MetalSonic_State_Explode;
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.PlaySfx(MetalSonic->sfxHit, false, 0xFF);
    }
}

void MetalSonic_Explode(void)
{
    RSDK_THIS(MetalSonic);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(MetalSonic->sfxExplosion2, false, 0xFF);

        if (!(Zone->timer & 0xF)) {
            int32 x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y = self->position.y + (RSDK.Rand(-24, 25) << 16);

            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}
#endif

void MetalSonic_State_SetupArena(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer >= 8) {
        self->timer = 0;

        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;

        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsT[0] = (self->position.y >> 16) - ScreenInfo->size.y + 52;
        Zone->cameraBoundsB[0] = (self->position.y >> 16) + 52;

        self->state = MetalSonic_State_AwaitPlayer;
    }
}

void MetalSonic_State_AwaitPlayer(void)
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
#if MANIA_USE_PLUS
            Music_ClearMusicStack();
#endif
            Music_PlayTrack(TRACK_METALSONIC);
            RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);

            self->position.y += 0x600000;
            self->velocity.y = -0x80000;
            self->visible    = true;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_ENTERPANEL, &self->metalSonicAnimator, false, 6);
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_INTRO, &self->boosterAnimator, false, 0);

            self->direction                = FLIP_X;
            self->metalSonicAnimator.speed = 0;
            self->active                   = ACTIVE_NORMAL;
            self->state                    = MetalSonic_State_Appear;
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
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, false, 3);
        RSDK.SetSpriteAnimation(-1, 0, &self->boosterAnimator, false, 0);

        self->velocity.x = 0x18000;
        self->onGround   = false;
        self->state      = MetalSonic_State_Land;
    }
}

void MetalSonic_State_Land(void)
{
    RSDK_THIS(MetalSonic);

    self->velocity.y += 0x2000;
    self->outerBox = RSDK.GetHitbox(&self->metalSonicAnimator, 0);
    self->innerBox = RSDK.GetHitbox(&self->metalSonicAnimator, 1);

    RSDK.ProcessObjectMovement(self, self->outerBox, self->innerBox);
    if (self->onGround) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_TAUNT, &self->metalSonicAnimator, false, 0);
        self->state                    = MetalSonic_State_Taunt;
        self->metalSonicAnimator.speed = 0;
        self->direction                = FLIP_NONE;
    }
}

void MetalSonic_State_Taunt(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 60) {
        self->timer                    = 0;
        self->metalSonicAnimator.speed = 1;
        self->state                    = MetalSonic_State_GetReady;
    }
}

void MetalSonic_State_GetReady(void)
{
    RSDK_THIS(MetalSonic);

    if (self->metalSonicAnimator.frameID == self->metalSonicAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_READY, &self->metalSonicAnimator, false, 0);
        self->state = MetalSonic_State_Ready;
    }
}

void MetalSonic_State_Ready(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_WEAK, &self->boosterAnimator, false, 0);
        self->state = MetalSonic_State_Start;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);
        Zone->cameraBoundsR[0] = size.x;
        Zone->cameraBoundsT[0] = 0;
    }
}

void MetalSonic_State_Start(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 90) {
        self->timer            = 0;
        Zone->cameraBoundsL[0] = 0;

        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BALLATTACK, &self->metalSonicAnimator, false, 0);
        self->velocity.x = 0x18000;
        self->velocity.y = -0x60000;
        RSDK.PlaySfx(MetalSonic->sfxJump2, false, 255);

        self->state = MetalSonic_State_EnterHoverMode;
    }
}

void MetalSonic_State_EnterHoverMode(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y >= 0) {
        RSDK.SetSpriteAnimation(-1, 0, &self->boosterAnimator, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_WEAK, &self->boosterAnimator, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, false, 6);
        self->metalSonicAnimator.speed = 0;

        self->targetPos.x = player1->position.x;
        self->targetPos.y = player1->position.y;
        self->velocity.x  = 0;
        self->velocity.y  = 0;
        self->attackTimer = 240;
        self->state       = MetalSonic_State_Hovering;
    }
}

void MetalSonic_State_Hovering(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (--self->timer <= 0) {
        self->timer = 60;
        int32 angle = RSDK.Rand(0, 256);
        int32 power = RSDK.Rand(64, 97) << 8;

        self->targetPos.x = (power + (power >> 2)) * RSDK.Cos256(angle);
        self->targetPos.y = power * RSDK.Sin256(angle);
    }

    self->targetPos.x += player1->position.x;
    self->targetPos.y += player1->position.y;

    MetalSonic_HandleVelocity();
    MetalSonic_HandleAnimDir();

    if (!self->onScreen) {
        if (self->velocity.x <= 0) {
            if (player1->velocity.x < self->targetVelocity.x)
                self->targetVelocity.x = player1->velocity.x;

            int32 dist = (self->targetPos.x - self->position.x) >> 5;
            if (dist < self->targetVelocity.x)
                self->targetVelocity.x = dist;

            if (self->position.x < player1->position.x + 244) {
                if (player1->velocity.x < 0) {
                    if (self->targetVelocity.x < 2 * player1->velocity.x) {
                        self->hoverVelocity.x  = 2 * player1->velocity.x;
                        self->targetVelocity.x = 2 * player1->velocity.x;
                    }
                }
            }
        }
        else {
            if (player1->velocity.x > self->targetVelocity.x)
                self->targetVelocity.x = player1->velocity.x;

            int32 dist = (self->targetPos.x - self->position.x) >> 5;
            if (dist > self->targetVelocity.x)
                self->targetVelocity.x = dist;

            if (self->position.x > player1->position.x - 244) {
                if (player1->velocity.x > 0) {
                    if (self->targetVelocity.x > 2 * player1->velocity.x) {
                        self->hoverVelocity.x  = 2 * player1->velocity.x;
                        self->targetVelocity.x = 2 * player1->velocity.x;
                    }
                }
            }
        }

        if (self->velocity.y <= 0) {
            if (player1->velocity.y < self->targetVelocity.y)
                self->targetVelocity.y = player1->velocity.y;

            int32 dist = (self->targetPos.y - self->position.y) >> 5;
            if (dist < self->targetVelocity.y)
                self->targetVelocity.y = dist;

            if (self->position.y < player1->position.y + 152) {
                if (player1->velocity.y < 0) {
                    if (self->targetVelocity.y < 2 * player1->velocity.y) {
                        self->hoverVelocity.y  = 2 * player1->velocity.y;
                        self->targetVelocity.y = 2 * player1->velocity.y;
                    }
                }
            }
        }
        else {
            if (player1->velocity.y > self->targetVelocity.y)
                self->targetVelocity.y = player1->velocity.y;

            int32 dist = (self->targetPos.y - self->position.y) >> 5;
            if (dist > self->targetVelocity.y)
                self->targetVelocity.y = dist;

            if (self->position.y > player1->position.y - 152) {
                if (player1->velocity.y > 0) {
                    if (self->targetVelocity.y > 2 * player1->velocity.y) {
                        self->hoverVelocity.y  = 2 * player1->velocity.y;
                        self->targetVelocity.y = 2 * player1->velocity.y;
                    }
                }
            }
        }

        self->attackTimer--;
        if (self->attackTimer < 4)
            self->attackTimer = 4;
    }
    else {
        self->targetVelocity.x = player1->velocity.x;
        self->targetVelocity.y = player1->velocity.y;
        self->attackTimer--;
    }

    if (self->attackTimer <= 0) {
        self->attackType = RSDK.Rand(0, 6);
        while ((1 << self->attackType) & MetalSonic->finishedAttacks) self->attackType = RSDK.Rand(0, 6);

        MetalSonic->finishedAttacks |= 1 << self->attackType;
        if (MetalSonic->finishedAttacks == 0x3F)
            MetalSonic->finishedAttacks = 0;

        self->attackType >>= 1;

        switch (self->attackType) {
            default: break;

            case MS_ATTACK_DASH:
                if (RSDK.Rand(0, 2) != 0)
                    self->targetPos.x = (ScreenInfo->size.x + 72) << 16;
                else
                    self->targetPos.x = -0x480000;
                self->targetPos.y = ScreenInfo->center.y << 16;
                self->state       = MetalSonic_State_PrepareAttack;
                break;

            case MS_ATTACK_ELECTRIC:
                if (player1->velocity.x >= 0) {
                    self->targetPos.y = 0x200000;
                    self->targetPos.x = (ScreenInfo->size.x - 32) << 16;
                }
                else {
                    self->targetPos.x = 0x200000;
                    self->targetPos.y = 0x200000;
                }
                self->state = MetalSonic_State_PrepareAttack;
                break;

            case MS_ATTACK_BALL: {
                int32 angle       = (RSDK.Rand(0, 2) + 3) << 6;
                self->targetPos.x = 0xA000 * RSDK.Cos256(angle) + player1->position.x - (ScreenInfo->position.x << 16);
                self->targetPos.y = player1->position.y + 0x7000 * RSDK.Sin256(angle) - (ScreenInfo->position.y << 16);
                self->state       = MetalSonic_State_PrepareAttack;
                break;
            }
        }
    }
    else {
        self->targetPos.x -= player1->position.x;
        self->targetPos.y -= player1->position.y;

        if (self->hoverVelocity.x != self->targetVelocity.x) {
            if (self->hoverVelocity.x <= self->targetVelocity.x) {
                self->hoverVelocity.x += (0xC00 << (self->hoverVelocity.x < 0));
                if (self->hoverVelocity.x > self->targetVelocity.x)
                    self->hoverVelocity.x = self->targetVelocity.x;
            }
            else {
                self->hoverVelocity.x -= (0xC00 << (self->hoverVelocity.x > 0));
                if (self->hoverVelocity.x < self->targetVelocity.x)
                    self->hoverVelocity.x = self->targetVelocity.x;
            }
        }

        if (self->hoverVelocity.y != self->targetVelocity.y) {
            if (self->hoverVelocity.y <= self->targetVelocity.y) {
                self->hoverVelocity.y += (0xC00 << (self->hoverVelocity.y < 0));
                if (self->hoverVelocity.y > self->targetVelocity.y)
                    self->hoverVelocity.y = self->targetVelocity.y;
            }
            else {
                self->hoverVelocity.y -= (0xC00 << (self->hoverVelocity.y > 0));

                if (self->hoverVelocity.y < self->targetVelocity.y)
                    self->hoverVelocity.y = self->targetVelocity.y;
            }
        }

        self->position.x += self->hoverVelocity.x + (self->velocity.x >> 1) + (self->velocity.x >> 3);
        self->position.y += self->hoverVelocity.y + (self->velocity.y >> 1) + (self->velocity.y >> 3);
    }

    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_PrepareAttack(void)
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

    int32 rx          = self->position.x - self->targetPos.x;
    int32 ry          = self->position.y - self->targetPos.y;
    self->targetPos.x = self->targetPos.x - (ScreenInfo->position.x << 16);
    self->targetPos.y = self->targetPos.y - (ScreenInfo->position.y << 16);

    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 0x1000) {
        self->attackTimer = 0;
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

    uint8 axisReady = 0;
    if (self->velocity.x > 0 && self->position.x > self->targetPos.x) {
        axisReady        = 1;
        self->position.x = self->targetPos.x;
    }
    else if (self->velocity.x < 0 && self->position.x < self->targetPos.x) {
        axisReady        = 1;
        self->position.x = self->targetPos.x;
    }

    if (self->velocity.y > 0 && self->position.y > self->targetPos.y) {
        ++axisReady;
        self->position.y = self->targetPos.y;
    }
    else if (self->velocity.y < 0 && self->position.y < self->targetPos.y) {
        ++axisReady;
        self->position.y = self->targetPos.y;
    }

    self->targetPos.x = self->targetPos.x - (ScreenInfo->position.x << 16);
    self->targetPos.y = self->targetPos.y - (ScreenInfo->position.y << 16);
    MetalSonic_HandleAnimDir();

    if (axisReady == 2) {
        switch (self->attackType) {
            default: break;

            case MS_ATTACK_DASH:
                self->attackTimer = 60;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_DASHATTACK, &self->metalSonicAnimator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSChargeFire, false, 255);
                self->state = MetalSonic_State_SetupDashAttack_Phase1;
                break;

            case MS_ATTACK_ELECTRIC:
                self->attackTimer = 60;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_ELECTRICATTACK, &self->metalSonicAnimator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 255);
                self->state = MetalSonic_State_SetupElectricAttack_Phase1;
                break;

            case MS_ATTACK_BALL:
                self->attackTimer = 60;
                self->targetPos.x = self->position.x - player1->position.x;
                self->targetPos.y = self->position.y - player1->position.y;
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BALLATTACK, &self->metalSonicAnimator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSBall, false, 255);
                self->state = MetalSonic_State_SetupBallAttack_Phase1;
                break;
        }
    }

    MetalSonic_HandleStageWrap();
}

void MetalSonic_State_SetupBallAttack_Phase1(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = player1->position.x + self->targetPos.x;
    self->position.y = player1->position.y + self->targetPos.y;

    int32 angle        = RSDK.ATan2(player1->position.x - self->position.x, player1->position.y - self->position.y);
    self->unusedVec1.x = self->position.x + (RSDK.Cos256(angle + 0xC0) << 12);
    self->unusedVec1.y = self->position.y + (RSDK.Sin256(angle + 0xC0) << 12);
    self->unusedVec2.x = self->position.x + (RSDK.Cos256(angle + 0x40) << 12);
    self->unusedVec2.y = self->position.y + (RSDK.Sin256(angle + 0x40) << 12);
    self->unusedVec3.x = self->position.x + (RSDK.Cos256(angle + 0x00) << 14);
    self->unusedVec3.y = self->position.y + (RSDK.Sin256(angle + 0x00) << 14);

    if (--self->attackTimer <= 0) {
        self->unusedVec1.x = -1;
        self->targetPos    = player1->position;
        self->velocity.x   = 0xA00 * RSDK.Cos256(angle);
        self->velocity.y   = 0xA00 * RSDK.Sin256(angle);
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 255);
        self->attackTimer = 15;
        self->state       = MetalSonic_State_BallAttack_Phase1;
    }

    MetalSonic_HandleStageWrap();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}

void MetalSonic_State_BallAttack_Phase1(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (!self->onScreen && --self->attackTimer <= 0) {
        self->targetPos   = player1->position;
        self->velocity.x  = 0;
        self->velocity.y  = 0;
        self->timer       = 0;
        self->attackTimer = 150;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, true, 0);
        self->state = MetalSonic_State_Hovering;
    }

    MetalSonic_HandleStageWrap();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}

void MetalSonic_State_SetupElectricAttack_Phase1(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = self->targetPos.x + (ScreenInfo->position.x << 16);
    self->position.y = self->targetPos.y + (ScreenInfo->position.y << 16);

    if (--self->attackTimer <= 0) {
        self->targetPos.x = player1->position.x;
        self->targetPos.y = player1->position.y;
        RSDK.ATan2(self->targetPos.x - self->position.x, self->targetPos.y - self->position.y);

        self->velocity.x  = self->position.x < player1->position.x ? 0x40000 : -0x40000;
        self->velocity.y  = 0x88000;
        self->attackTimer = 15;
        self->state       = MetalSonic_State_ElectricAttack_Phase1;
    }

    MetalSonic_HandleStageWrap();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}

void MetalSonic_State_ElectricAttack_Phase1(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    if (self->position.y < player1->position.y && !self->onScreen && --self->attackTimer <= 0) {
        self->targetPos   = player1->position;
        self->velocity.x  = 0;
        self->velocity.y  = 0;
        self->timer       = 0;
        self->attackTimer = 150;

        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, true, 0);
        self->state = MetalSonic_State_Hovering;
    }

    MetalSonic_HandleStageWrap();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}

void MetalSonic_State_SetupDashAttack_Phase1(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x = self->targetPos.x + (ScreenInfo->position.x << 16);
    self->position.y = self->targetPos.y + (ScreenInfo->position.y << 16);

    int32 angle        = RSDK.ATan2(player1->position.x - self->position.x, player1->position.y - self->position.y);
    self->unusedVec1.x = self->position.x + (RSDK.Cos256(angle + 0xC0) << 12);
    self->unusedVec1.y = self->position.y + (RSDK.Sin256(angle + 0xC0) << 12);
    self->unusedVec2.x = self->position.x + (RSDK.Cos256(angle + 0x40) << 12);
    self->unusedVec2.y = self->position.y + (RSDK.Sin256(angle + 0x40) << 12);
    self->unusedVec3.x = self->position.x + (RSDK.Cos256(angle + 0x00) << 15);
    self->unusedVec3.y = self->position.y + (RSDK.Sin256(angle + 0x00) << 15);

    if (--self->attackTimer <= 0) {
        self->unusedVec1.x = -1;
        self->targetPos    = player1->position;
        self->velocity.x   = 0xC00 * RSDK.Cos256(angle);
        self->velocity.y   = 0xC00 * RSDK.Sin256(angle);
        RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 255);

        self->attackTimer = 15;
        self->state       = MetalSonic_State_DashAttack_Phase1;
    }

    MetalSonic_HandleStageWrap();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}

void MetalSonic_State_DashAttack_Phase1(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    bool32 finished = true;
    if (self->velocity.x <= 0) {
        if (self->velocity.x >= 0 || self->position.x >= player1->position.x)
            finished = false;
    }
    else if (self->position.x <= player1->position.x) {
        if (self->velocity.x >= 0 || self->position.x >= player1->position.x)
            finished = false;
    }

    if (finished) {
        if (!self->onScreen && --self->attackTimer <= 0) {
            self->targetPos   = player1->position;
            self->velocity.x  = 0;
            self->velocity.y  = 0;
            self->timer       = 0;
            self->attackTimer = 150;

            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, true, 0);
            self->state = MetalSonic_State_Hovering;
        }
    }

    MetalSonic_HandleStageWrap();

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}

void MetalSonic_State_EnterPanel(void)
{
    RSDK_THIS(MetalSonic);

    int32 velX = self->velocity.x;

    MetalSonic_HandleVelocity();

    if (self->velocity.x < 0x20000 && velX >= 0x20000) {
        self->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }
    else if (self->velocity.x > -0x20000 && velX <= -0x20000) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }

    ++self->timer;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    int32 rx = self->position.x - self->targetPos.x;
    int32 ry = self->position.y - self->targetPos.y;

    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 4096 && self->timer > 96) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(-1, 0, &self->boosterAnimator, false, 0);
        self->state = MetalSonic_State_StartPanelSequence;
    }
}

void MetalSonic_State_StartPanelSequence(void)
{
    RSDK_THIS(MetalSonic);

    ++self->timer;
    self->position.x += (self->targetPos.x - self->position.x) >> 4;
    self->position.y += (self->targetPos.y - self->position.y) >> 4;

    if (self->timer == 32)
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_ENTERPANEL, &self->metalSonicAnimator, true, 0);

    if (self->timer == 59)
        RSDK.PlaySfx(MetalSonic->sfxSpecialRing, false, 255);

    if (self->timer == 64) {
        int32 id = 0;
#if MANIA_USE_PLUS
        for (int32 i = 48; i < 82; i += 2) {
            if (id > 0)
                RSDK.CopyTileLayer(Zone->fgLayer[0], 167, i, Zone->fgLayer[1], 222, 218, 2, 2);

            RSDK.CopyTileLayer(Zone->fgLayer[0], 169, i, Zone->fgLayer[1], 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 171, i, Zone->fgLayer[1], 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 173, i, Zone->fgLayer[1], 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 175, i, Zone->fgLayer[1], 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 177, i, Zone->fgLayer[1], 222, 138, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 179, i, Zone->fgLayer[1], 222, 138, 2, 2);
            ++id;
        }
#else
        for (int32 i = 128; i < 82; i += 2) {
            int32 y = i;
            if (id > 0) {
                y = 2 * id + 128;
                RSDK.CopyTileLayer(Zone->fgLayer[0], 167, i, Zone->fgLayer[1], 222, 218, 2, 2);
            }

            RSDK.CopyTileLayer(Zone->fgLayer[0], 169, y, Zone->fgLayer[1], 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 171, y, Zone->fgLayer[1], 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 173, y, Zone->fgLayer[1], 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 175, y, Zone->fgLayer[1], 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 177, y, Zone->fgLayer[1], 222, 218, 2, 2);
            RSDK.CopyTileLayer(Zone->fgLayer[0], 179, y, Zone->fgLayer[1], 222, 218, 2, 2);
            ++id;
        }
#endif

        EntityMSPanel *panel = self->panel;
        panel->state         = MSPanel_State_Active;

        self->position    = self->targetPos;
        self->timer       = 16;
        self->attackTimer = MANIA_USE_PLUS ? 240 : 180;
        self->health      = MANIA_USE_PLUS ? 6 : 4;
        self->state       = MetalSonic_State_OpenFactoryDoor;
    }
}

void MetalSonic_HandlePanelAttack(void)
{
    RSDK_THIS(MetalSonic);

#if MANIA_USE_PLUS
    if (!RSDK.GetEntityCount(MSBomb->classID, true))
#endif
        --self->attackTimer;

    if (self->attackTimer == 60) {
        RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 255);
        self->invincibilityTimer = 60;

#if MANIA_USE_PLUS
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, self, self->position.x, self->position.y);
        ring->radiusOffset     = 24;
        ring->timer            = 24;
        ring->r                = 0xF0;
        ring->g                = 0x80;
        ring->b                = 0x00;
        ring->shrinkSpeed      = 1;
#endif
    }

    if (self->attackTimer <= 0) {
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        self->attackTimer = 45 * self->health + (MANIA_USE_PLUS ? 195 : 135);
        int32 angle       = RSDK.ATan2(player1->position.x - self->position.x, player1->position.y - self->position.y);

        EntityMSOrb *orb = CREATE_ENTITY(MSOrb, NULL, self->position.x, self->position.y);
        orb->velocity.x  = 0x280 * RSDK.Cos256(angle);
        orb->velocity.y  = 0x280 * RSDK.Sin256(angle);
        RSDK.PlaySfx(MetalSonic->sfxMSShoot, false, 255);
    }
}

void MetalSonic_State_OpenFactoryDoor(void)
{
    RSDK_THIS(MetalSonic);

    ++self->timer;

    MetalSonic_HandlePanelAttack();

    if (self->timer == 60) {
        foreach_active(MSFactory, factory)
        {
            factory->visible = true;
            factory->state   = MSFactory_State_OpeningDoor;
        }

        self->timer = 0;
        self->state = MetalSonic_State_HandleSilverSonics;
    }
}

void MetalSonic_State_HandleSilverSonics(void)
{
    RSDK_THIS(MetalSonic);

    ++self->timer;
    MetalSonic_HandlePanelAttack();

#if MANIA_USE_PLUS
    if (!RSDK.GetEntityCount(SilverSonic->classID, true) && !RSDK.GetEntityCount(MSBomb->classID, true) && self->timer > 60) {
#else
    if (!RSDK.GetEntityCount(SilverSonic->classID, true) && self->timer > 60) {
#endif
        self->timer = 0;
        self->state = MetalSonic_State_OpenFactoryDoor;
    }
}

void MetalSonic_State_PanelExplosion(void)
{
    RSDK_THIS(MetalSonic);

    EntityMSPanel *panel = self->panel;

    self->position.y += 0x8000;
    panel->position.y += 0x8000;

    if (!self->timer)
        panel->state = MSPanel_State_Explode;

    if (++self->timer == 104) {
        self->timer  = 0;
        panel->state = MSPanel_State_Rumbling;
        self->state  = MetalSonic_State_ExitFactory;
    }
}

void MetalSonic_State_ExitFactory(void)
{
    RSDK_THIS(MetalSonic);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, false, 3);

        self->velocity.x = 0x18000;
        self->onGround   = false;
        self->state      = MetalSonic_State_PrepareFinalChase;
    }
}

void MetalSonic_State_PrepareFinalChase(void)
{
    RSDK_THIS(MetalSonic);

    self->velocity.y += 0x2000;

    self->outerBox = RSDK.GetHitbox(&self->metalSonicAnimator, 0);
    self->innerBox = RSDK.GetHitbox(&self->metalSonicAnimator, 1);
    RSDK.ProcessObjectMovement(self, self->outerBox, self->innerBox);

    if (self->onGround) {
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_READY, &self->metalSonicAnimator, false, 0);
        self->direction = FLIP_NONE;
        self->groundVel = 0;
    }

    if (++self->timer == 120) {
        EntityFXFade *fade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fade->speedIn      = 256;
        fade->wait         = 32;
        fade->speedOut     = 8;

        self->panel->state = StateMachine_None;
        self->timer        = 0;
        self->attackTimer  = 240;
        self->state        = MetalSonic_State_Hovering;

        foreach_active(Player, player)
        {
            RSDK.PlaySfx(SpeedBooster->sfxSpeedBooster, false, 255);
            player->velocity.x  = 0xE0000;
            player->groundVel   = 0xE0000;
            player->controlLock = 60;
            player->direction   = FLIP_NONE;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_RUN, &player->animator, false, 0);
            player->state = Player_State_Ground;
        }

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);

        Zone->cameraBoundsL[0] = 0;
        Zone->cameraBoundsR[0] = size.x;
        Zone->cameraBoundsT[0] = 0;

        Zone->playerBoundActiveL[0] = false;
        Zone->playerBoundActiveR[0] = false;

        RSDK.PlaySfx(MetalSonic->sfxExplosion3, false, 255);
    }
}

#if MANIA_USE_PLUS
void MetalSonic_State_WaitForRuby(void)
{
    RSDK_THIS(MetalSonic);

    int32 velX = self->velocity.x;
    MetalSonic_HandleVelocity();

    if (self->velocity.x < 0x20000 && velX >= 0x20000) {
        self->direction = FLIP_X;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }
    else if (self->velocity.x > -0x20000 && velX <= -0x20000) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }

    ++self->timer;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    int32 rx = self->position.x - self->targetPos.x;
    int32 ry = self->position.y - self->targetPos.y;

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
        int32 rx = (self->position.x - ruby->position.x) >> 16;
        int32 ry = (self->position.y - ruby->position.y) >> 16;

        if (rx * rx + ry * ry < 0x100) {
            ruby->startPos.x = ruby->position.x;
            ruby->startPos.y = ruby->position.y;
            ruby->state      = PhantomRuby_State_Oscillate;
            self->state      = MetalSonic_State_Transform;
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
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, -1, &self->boosterAnimator, true, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_ELECTRICATTACK, &self->metalSonicAnimator, true, 0);
    }

    int32 timer = MIN(self->timer >> 1, 96);

    if ((self->timer & 0x1F) == 30) {
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, self, self->position.x, self->position.y);
        ring->radiusOffset     = timer;
        ring->timer            = 24;
        ring->r                = 0xF0;
        ring->g                = 0x00;
        ring->b                = 0xF0;
        ring->shrinkSpeed      = 2;
    }

    if (timer >= 32 && (self->timer & 0xF) == 14) {
        EntityFXWaveRing *ring = CREATE_ENTITY(FXWaveRing, self, self->position.x, self->position.y);
        ring->radiusOffset     = timer + 64;
        ring->timer            = 24;
        ring->r                = 0xF0;
        ring->g                = 0xF0;
        ring->b                = 0x60;
        ring->shrinkSpeed      = 4;
    }

    if (self->timer == 240) {
        self->timer  = 0;
        self->active = ACTIVE_NEVER;

        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
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
        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, INT_TO_VOID(EGGPRISON_FLYING), (ScreenInfo->position.x + ScreenInfo->center.x) << 16,
                                                (ScreenInfo->position.y - 48) << 16);
        prison->velocity.x      = 0x10000;
        prison->active          = ACTIVE_NORMAL;
        destroyEntity(self);
    }
}
#else
void MetalSonic_State_SetupSpikeWall(void)
{
    RSDK_THIS(MetalSonic);

    MetalSonic_HandleVelocity();

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    MetalSonic_HandleStageWrap();

    self->timer++;

    EntityPlatform *wall = NULL;
    EntityPlatform *startWall = NULL;

    switch (self->timer) {
        case 60: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform); break;
        case 90: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 3, Platform); break;
        case 120: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 4, Platform); break;
        case 150: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 5, Platform); break;
        case 180: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 6, Platform); break;
        case 210: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 7, Platform); break;
        case 240: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 8, Platform); break;
        case 270: wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 9, Platform); break;
        case 300:
            startWall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

            self->targetPos.x = startWall->position.x + 0x800000;
            self->targetPos.y = (Zone->cameraBoundsB[0] << 16) - 0x340000;
            self->state = MetalSonic_State_FlyToSpikeWall;
            break;
    }

    if (wall) {
        wall->active = ACTIVE_NORMAL;
        wall->state = MetalSonic_StateWall_Fall;
    }
}
void MetalSonic_State_FlyToSpikeWall(void)
{
    RSDK_THIS(MetalSonic);

    MetalSonic_HandleVelocity();

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    MetalSonic_HandleAnimDir();

    int32 rx = self->position.x - self->targetPos.x;
    int32 ry = self->position.y - self->targetPos.y;

    if ((rx >> 16) * (rx >> 16) + (ry >> 16) * (ry >> 16) < 0x1000) {
        self->attackTimer = 0;
        self->position.x &= 0xFFFF0000;
        self->position.y &= 0xFFFF0000;
        self->targetPos.x &= 0xFFFF0000;
        self->targetPos.y &= 0xFFFF0000;
        self->state = MetalSonic_State_LandNearSpikeWall;
    }

    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_LandNearSpikeWall(void)
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

    uint8 axisFinished = 0;
    if (self->velocity.x > 0) {
        if (self->position.x > self->targetPos.x) {
            axisFinished = 1;
            self->position.x = self->targetPos.x;
        }
    }
    else if (self->position.x < self->targetPos.x) {
        axisFinished = 1;
        self->position.x = self->targetPos.x;
    }

    if (self->velocity.y > 0) {
        if (self->position.y > self->targetPos.y) {
            ++axisFinished;
            self->position.y = self->targetPos.y;
        }
    }
    else if (self->position.y < self->targetPos.y) {
        ++axisFinished;
        self->position.y = self->targetPos.y;
    }

    MetalSonic_HandleAnimDir();

    if (axisFinished == 2) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_TAUNT, &self->metalSonicAnimator, false, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->boosterAnimator, false, 0);

        self->direction = FLIP_NONE;
        self->state = MetalSonic_State_Taunt_Phase2;
    }

    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Taunt_Phase2(void)
{
    RSDK_THIS(MetalSonic);

    if (self->metalSonicAnimator.frameID == self->metalSonicAnimator.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_READY, &self->metalSonicAnimator, false, 0);
        self->state = MetalSonic_State_Ready_Phase2;
    }
    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Ready_Phase2(void)
{
    RSDK_THIS(MetalSonic);

    self->timer++;
    if (self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_WEAK, &self->boosterAnimator, false, 0);
        self->state = MetalSonic_State_StartSpikeWallMovement;

        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);
    }

    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_StartSpikeWallMovement(void)
{
    RSDK_THIS(MetalSonic);

    self->timer++;
    if (self->timer == 90) {
        int32 slot = SceneInfo->entitySlot;
        RSDK_GET_ENTITY(slot + 2, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 3, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 4, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 5, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 6, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 7, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 8, Platform)->state = MetalSonic_StateWall_Move;
        RSDK_GET_ENTITY(slot + 9, Platform)->state = MetalSonic_StateWall_Move;

        self->timer = 0;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, false, 0);
        self->state = MetalSonic_State_AccelerateSpikeWall;
    }

    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_AccelerateSpikeWall(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    if (wall->velocity.x >= 0x1C000) {
        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);

        for (int32 i = 0; i < PLAYER_COUNT; ++i) {
            Zone->cameraBoundsL[i] = 0;
            Zone->cameraBoundsR[i] = size.x + 0x400;
            Zone->playerBoundActiveL[i] = false;
            Zone->playerBoundActiveL[i] = false;
        }

        if (wall->velocity.x >= 0x54000) {
            self->health = 8;
            self->attackTimer = 120;
            self->state = MetalSonic_State_Hover_Phase2;
        }
    }

    self->velocity.x = wall->velocity.x;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    MetalSonic_HandleStageWrap();
}
void MetalSonic_State_Hover_Phase2(void)
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

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x140000, true))
        self->velocity.y = 0;

    if (self->direction == FLIP_X) {
        self->direction = FLIP_NONE;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }

    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x800000)
        self->position.x = wall->position.x + 0x800000;

    if (--self->attackTimer <= 0 && !player1->blinkTimer) {
        if (self->velocity.x < 0x60000)
            self->velocity.x = 0x60000;
        self->attackTimer = 120;

        self->attackType = RSDK.Rand(0, 6);
        while ((1 << self->attackType) & MetalSonic->finishedAttacks) self->attackType = RSDK.Rand(0, 6);

        MetalSonic->finishedAttacks |= 1 << self->attackType;
        if (MetalSonic->finishedAttacks == 0x3F)
            MetalSonic->finishedAttacks = 0;

        self->attackType >>= 1;
        switch (self->attackType) {
            case MS_ATTACK_DASH:
                RSDK.PlaySfx(MetalSonic->sfxMSFireball, false, 0xFF);

                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_DASHATTACK, &self->metalSonicAnimator, false, 0);
                RSDK.SetSpriteAnimation(-1, 0, &self->boosterAnimator, false, 0);
                self->attackTimer = 60;
                self->state = MetalSonic_State_DashAttack_Phase2;
                break;

            case MS_ATTACK_ELECTRIC:
                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_ELECTRICATTACK, &self->metalSonicAnimator, true, 0);

                RSDK.PlaySfx(MetalSonic->sfxMSElecPulse, false, 0xFF);
                self->state = MetalSonic_State_SetupElectricAttack_Phase2;
                break;

            case MS_ATTACK_BALL:
                self->attackTimer = 60;
                self->targetPos.x = (RSDK.Rand(-3, 7) << 14) + 0x78000;
                self->targetPos.y = -0x80000;

                RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BALLATTACK, &self->metalSonicAnimator, true, 0);
                RSDK.PlaySfx(MetalSonic->sfxMSBall, false, 0xFF);
                self->state = MetalSonic_State_SetupBallAttack_Phase2;
                break;
        }
    }

    MetalSonic_CheckPlayerCollisions();
}
void MetalSonic_State_SetupElectricAttack_Phase2(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    if (self->velocity.x < 0xB0000)
        self->velocity.x += 0xC00;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y <= (ScreenInfo->center.y + ScreenInfo->position.y) << 16) {
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

    if (self->position.x > ((ScreenInfo->size.x + ScreenInfo->position.x) << 16) + 0x400000) {
        self->direction = FLIP_X;
        self->state = MetalSonic_State_StartElectricAttack_Phase2;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}
void MetalSonic_State_StartElectricAttack_Phase2(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->velocity.x = player1->velocity.x - 0x10000;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->position.y <= (ScreenInfo->center.y + ScreenInfo->position.y) << 16) {
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

    if (self->position.x < ((ScreenInfo->position.x + ScreenInfo->size.x) << 16) - 0x180000) {
        self->attackTimer = 120;
        self->position.x = ((ScreenInfo->position.x + ScreenInfo->size.x) << 16) - 0x180000;
        self->state = MetalSonic_State_ElectricAttack_Phase2;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}
void MetalSonic_State_ElectricAttack_Phase2(void)
{
    RSDK_THIS(MetalSonic);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->velocity.x = player1->velocity.x;
    self->position.x = ((ScreenInfo->position.x + ScreenInfo->size.x) << 16) - 0x180000;
    self->position.y += self->velocity.y;

    if (self->position.y <= (ScreenInfo->center.y + ScreenInfo->position.y) << 16) {
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

    switch (--self->attackTimer) {
        case 0:
            self->attackTimer = 120;
            self->velocity.y = 0x10000;

            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, false, 0);
            self->state = MetalSonic_State_FinishAttack_Phase2;
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
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}
void MetalSonic_State_SetupBallAttack_Phase2(void)
{
    RSDK_THIS(MetalSonic);

    int32 angle = RSDK.ATan2(self->targetPos.x, self->targetPos.y);
    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->unusedVec1.x = self->position.x + (RSDK.Cos256(angle + 0xC0) << 12);
    self->unusedVec1.y = self->position.y + (RSDK.Sin256(angle - 0x40) << 12);
    self->unusedVec2.x = self->position.x + (RSDK.Cos256(angle + 0x40) << 12);
    self->unusedVec2.y = self->position.y + (RSDK.Sin256(angle + 0x40) << 12);
    self->unusedVec3.x = self->position.x + (RSDK.Cos256(angle + 0x00) << 14);
    self->unusedVec3.y = self->position.y + (RSDK.Sin256(angle + 0x00) << 14);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    MetalSonic_HandleStageWrap();

    if (self->position.x < wall->position.x + 0x410000)
        self->position.x = wall->position.x + 0x410000;

    self->attackTimer--;
    if (self->attackTimer <= 0) {
        self->unusedVec1.x = -1;
        self->attackTimer = 8;
        self->velocity = self->targetPos;
        RSDK.PlaySfx(Player->sfxPeelRelease, false, 0xFF);
        self->state = MetalSonic_State_BallAttack_Phase2;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}
void MetalSonic_State_BallAttack_Phase2(void)
{
    RSDK_THIS(MetalSonic);

    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->position.y >= 0x1F00000) {
        if (self->position.x > ((ScreenInfo->size.x + ScreenInfo->position.x) << 16) + 0x400000 || !--self->attackTimer) {
            self->attackTimer = 120;
            RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, false, 2);
            self->state = MetalSonic_State_FinishAttack_Phase2;
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
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}
void MetalSonic_State_DashAttack_Phase2(void)
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

    if (self->attackTimer) {
        self->attackTimer--;
    }
    else if (self->position.x > player1->position.x + 0x400000) {
        self->attackTimer = 120;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_FLY, &self->metalSonicAnimator, false, 2);
        self->state = MetalSonic_State_FinishAttack_Phase2;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &MetalSonic->hitboxHover))
            Player_Hurt(player, self);
    }
}
void MetalSonic_State_FinishAttack_Phase2(void)
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
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVERTURN, &self->metalSonicAnimator, true, 0);
    }

    MetalSonic_HandleStageWrap();

    EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Platform);

    if (self->position.x < wall->position.x + 0x900000) {
        self->position.x = wall->position.x + 0x900000;
        self->attackTimer = 120;
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_BOOSTER_WEAK, &self->boosterAnimator, false, 0);
        RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_HOVER, &self->metalSonicAnimator, false, 0);
        self->state = MetalSonic_State_Hover_Phase2;
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

        for (int32 i = 2; i < 10; ++i) {
            EntityPlatform *wall = RSDK_GET_ENTITY(SceneInfo->entitySlot + i, Platform);
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, wall->position.x, wall->position.y);

            RSDK.SetSpriteAnimation(Platform->aniFrames, 0, &debris->animator, true, 1);
            debris->velocity.x = RSDK.Rand(6, 11) << 16;
            debris->velocity.y = RSDK.Rand(-0x20000, -0x10000);
            debris->gravityStrength = 0x4800;
            debris->drawGroup = wall->drawGroup;
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

        Zone->cameraBoundsL[0] = ScreenInfo->position.x;
        Zone->cameraBoundsR[0] = ScreenInfo->size.x + ScreenInfo->position.x;
        Zone->playerBoundsR[0] = (ScreenInfo->size.x + ScreenInfo->position.x) << 16;
        Zone->playerBoundsL[0] = ScreenInfo->position.x << 16;

        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, INT_TO_VOID(EGGPRISON_FLYING), (ScreenInfo->position.x + ScreenInfo->center.x) << 16,
                                                (ScreenInfo->position.y - 48) << 16);

        prison->velocity.x = 0x10000;
        prison->active = ACTIVE_NORMAL;
        self->state = MetalSonic_State_None;
    }
}

void MetalSonic_State_None(void)
{
    // common preplus L
}

void MetalSonic_StateWall_Fall(void)
{
    RSDK_THIS(Platform);

    self->drawPos.y += self->velocity.y;
    self->centerPos.y = self->drawPos.y;
    self->position.y = self->drawPos.y;

    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, (self->speed << 21) + 0x100000, true)) {
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

    bool32 metalInvincibile = false;
    foreach_active(MetalSonic, metal)
    {
        if (metal->invincibilityTimer)
            metalInvincibile = true;
    }

    if (metalInvincibile || player1->blinkTimer) {
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

    int32 x = player1->position.x - 0x2000000;
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

#if GAME_INCLUDE_EDITOR
void MetalSonic_EditorDraw(void)
{
    RSDK_THIS(MetalSonic);

    self->drawFX         = FX_FLIP | FX_ROTATE;
    self->updateRange.x  = 0x800000;
    self->updateRange.y  = 0x800000;
    self->tileCollisions = TILECOLLISION_DOWN;
    RSDK.SetSpriteAnimation(MetalSonic->aniFrames, MS_ANI_IDLE, &self->metalSonicAnimator, false, 0);

    RSDK.DrawSprite(&self->boosterAnimator, NULL, false);
    RSDK.DrawSprite(&self->metalSonicAnimator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE + 52, WIDE_SCR_XCENTER, 52, 1 | 2 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void MetalSonic_EditorLoad(void) { MetalSonic->aniFrames = RSDK.LoadSpriteAnimation("SSZ2/MetalSonic.bin", SCOPE_STAGE); }
#endif

void MetalSonic_Serialize(void) {}
