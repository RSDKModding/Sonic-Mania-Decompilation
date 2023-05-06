// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WeatherMobile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWeatherMobile *WeatherMobile;

void WeatherMobile_Update(void)
{
    RSDK_THIS(WeatherMobile);

    StateMachine_Run(self->state);
}

void WeatherMobile_LateUpdate(void) {}

void WeatherMobile_StaticUpdate(void)
{
    int32 windManagerCount   = 0;
    int32 windCount          = 0;
    int32 rainCount          = 0;
    int32 windFinishingCount = 0;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
        foreach_all(WeatherMobile, weatherMobile)
        {
            if (weatherMobile->type == WEATHERMOBILE_RAIN)
                rainCount++;

            if (weatherMobile->type == WEATHERMOBILE_WIND_MANAGER) {
                ++windManagerCount;
                if (weatherMobile->state == WeatherMobile_StateWindManager_FinishWind)
                    windFinishingCount++;
            }

            if (weatherMobile->type == WEATHERMOBILE_WIND)
                windCount++;
        }
    }

    if (rainCount) {
        if (!WeatherMobile->playingRainSfx) {
            RSDK.PlaySfx(WeatherMobile->sfxRain, true, 255);
            WeatherMobile->playingRainSfx = true;
        }
    }
    else if (WeatherMobile->playingRainSfx) {
        RSDK.StopSfx(WeatherMobile->sfxRain);
        WeatherMobile->playingRainSfx = false;
    }

    if (windFinishingCount || WeatherMobile->windSfxTimer > 0) {
        int32 timer = WeatherMobile->windSfxTimer;
        if (timer < 60)
            RSDK.SetChannelAttributes(WeatherMobile->windSfxChannel, (60 - timer) / 60.0, 0.0, (((60 - timer) * 0.5) / 60.0) + 0.5);

        ++WeatherMobile->windSfxTimer;
    }

    if ((windManagerCount || windCount) && !WeatherMobile->playingWindSfx) {
        WeatherMobile->windSfxChannel = RSDK.PlaySfx(WeatherMobile->sfxWind, 60067, 255);
        WeatherMobile->playingWindSfx = true;
        WeatherMobile->windSfxTimer   = 0;
    }
    else if (!windManagerCount && !windCount) {
        if (WeatherMobile->playingWindSfx) {
            RSDK.StopSfx(WeatherMobile->sfxWind);
            WeatherMobile->playingWindSfx = false;
        }
    }
}

void WeatherMobile_Draw(void)
{
    RSDK_THIS(WeatherMobile);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    }
}

void WeatherMobile_Create(void *data)
{
    RSDK_THIS(WeatherMobile);

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->type          = VOID_TO_INT(data);

            switch (self->type) {
                case WEATHERMOBILE_EGGMAN:
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    self->hitbox.left   = -22;
                    self->hitbox.top    = -22;
                    self->hitbox.right  = 22;
                    self->hitbox.bottom = 22;

                    self->drawFX = FX_FLIP;
                    self->health = 8;
                    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 4, &self->seatAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 6, &self->eggMobileAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 23, &self->decorAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 21, &self->radarDishLAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 22, &self->radarDishRAnimator, true, 0);
                    self->stateDraw = WeatherMobile_Draw_Eggman;
                    self->state     = WeatherMobile_State_SetupArena;
                    break;

                case WEATHERMOBILE_LIGHTS:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[1];
                    self->drawFX    = FX_FLIP;

                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 12, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 12, &self->eggmanAnimator, true, 2);
                    self->position.y = -0x180000;
                    RSDK.PlaySfx(WeatherMobile->sfxSun, false, 255);

                    self->stateDraw = WeatherMobile_Draw_Lights;
                    self->state     = WeatherMobile_StateLights_EnterLights;
                    break;

                case WEATHERMOBILE_WIND_MANAGER:
                    self->active     = ACTIVE_NORMAL;
                    self->velocity.y = -0x80000;
                    self->state      = WeatherMobile_StateWindManager_SetupWind;
                    break;

                case WEATHERMOBILE_WIND:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[1];

                    if (RSDK.Rand(0, 8) == 4) {
                        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 14, &self->mainAnimator, true, 0);
                    }
                    else {
                        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 13, &self->mainAnimator, true, RSDK.Rand(0, 3));
                        self->drawFX    = FX_SCALE;
                        self->inkEffect = INK_ADD;
                        self->alpha     = 0xC0;
                        self->scale.x   = 0x200;
                        self->scale.y   = 0x400;
                    }

                    self->state = WeatherMobile_State_Wind;
                    break;

                case WEATHERMOBILE_RAIN_MANAGER:
                    self->active = ACTIVE_NORMAL;
                    self->angle  = 32;
                    self->state  = WeatherMobile_State_RainManager;
                    break;

                case WEATHERMOBILE_RAIN:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[1];
                    self->drawFX    = FX_ROTATE;
                    self->groundVel = 0x200;

                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 19, &self->mainAnimator, true, 0);
                    self->state = WeatherMobile_State_RainDrop;
                    break;

                case WEATHERMOBILE_CLOUD:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[1];

                    self->hitbox.left   = -8;
                    self->hitbox.top    = 0;
                    self->hitbox.right  = 8;
                    self->hitbox.bottom = 128;

                    self->inkEffect = INK_ALPHA;
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 15, &self->mainAnimator, true, 0);
                    self->origin = self->position;

                    self->state = WeatherMobile_StateCloud_Idle;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void WeatherMobile_StageLoad(void)
{
    WeatherMobile->active = ACTIVE_ALWAYS;

    WeatherMobile->aniFrames    = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE);
    WeatherMobile->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggMobile.bin", SCOPE_STAGE);

    WeatherMobile->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    WeatherMobile->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    WeatherMobile->sfxCloud     = RSDK.GetSfx("SPZ2/Cloud.wav");
    WeatherMobile->sfxLightning = RSDK.GetSfx("SPZ2/Lightning.wav");
    WeatherMobile->sfxSun       = RSDK.GetSfx("SPZ2/Sun.wav");
    WeatherMobile->sfxWind      = RSDK.GetSfx("SPZ2/Wind.wav");
    WeatherMobile->sfxRain      = RSDK.GetSfx("FBZ/Rain.wav");
}

void WeatherMobile_HandleAnimations(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    if (self->eggmanAnimator.animationID) {
        if (self->eggmanAnimator.animationID == 1 || (self->eggmanAnimator.animationID - 1) == 1) {
            if (self->eggmanAnimator.frameID == self->eggmanAnimator.frameCount - 1)
                RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
        }
    }
    else {
        self->eggmanAnimator.speed = 0;
    }
}

void WeatherMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(WeatherMobile);

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            WeatherMobile_Hit();
        }
    }
}

void WeatherMobile_Hit(void)
{
    RSDK_THIS(WeatherMobile);

    if (--self->health <= 0) {
        WeatherMobile_HandleDefeat();
    }
    else {
        self->invincibilityTimer = 48;
        RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 2, &self->eggmanAnimator, true, 0);
        RSDK.PlaySfx(WeatherMobile->sfxHit, false, 255);
    }
}

void WeatherMobile_Explode(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(WeatherMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(self->hitbox.left, self->hitbox.right) << 16);
            int32 y = self->position.y + (RSDK.Rand(self->hitbox.top, self->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1];
        }
    }
}

void WeatherMobile_HandleDefeat(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 3, &self->eggmanAnimator, true, 0);
    self->state = WeatherMobile_State_Defeated;
    self->timer = 0;

    foreach_active(WeatherMobile, weatherMobile)
    {
        switch (weatherMobile->type) {
            case WEATHERMOBILE_LIGHTS:
                if (weatherMobile->state == WeatherMobile_StateLights_Shine) {
                    weatherMobile->timer = 0;
                    weatherMobile->state = WeatherMobile_StateLights_DimAndDissapear;
                }
                break;

            case WEATHERMOBILE_WIND_MANAGER:
            case WEATHERMOBILE_RAIN_MANAGER:
                destroyEntity(weatherMobile);
                Zone->cameraBoundsT[0] = Zone->cameraBoundsB[0] - ScreenInfo->size.y;
                break;

            case WEATHERMOBILE_CLOUD:
                if (weatherMobile->state != WeatherMobile_StateCloud_FadeAway) {
                    weatherMobile->timer     = 0;
                    weatherMobile->stateDraw = StateMachine_None;
                    weatherMobile->state     = WeatherMobile_StateCloud_FadeAway;
                }
                break;

            default: break;
        }
    }

    foreach_active(TVPole, pole) { pole->state = TVPole_State_ForceRelease; }

    SceneInfo->timeEnabled = false;
    Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
}

void WeatherMobile_Draw_Eggman(void)
{
    RSDK_THIS(WeatherMobile);

    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 32, 0, 32, 10);
        RSDK.SetPaletteEntry(0, 128, 0xF0F0F0);

        RSDK.DrawSprite(&self->radarDishLAnimator, NULL, false);
        RSDK.DrawSprite(&self->seatAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggMobileAnimator, NULL, false);
        RSDK.DrawSprite(&self->decorAnimator, NULL, false);
        RSDK.DrawSprite(&self->radarDishRAnimator, NULL, false);

        RSDK.CopyPalette(1, 32, 0, 32, 10);
        RSDK.SetPaletteEntry(0, 128, 0x000000);
    }
    else {
        RSDK.DrawSprite(&self->radarDishLAnimator, NULL, false);
        RSDK.DrawSprite(&self->seatAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);
        RSDK.DrawSprite(&self->eggMobileAnimator, NULL, false);
        RSDK.DrawSprite(&self->decorAnimator, NULL, false);
        RSDK.DrawSprite(&self->radarDishRAnimator, NULL, false);
    }
}

void WeatherMobile_Draw_Lights(void)
{
    RSDK_THIS(WeatherMobile);

    Vector2 drawPos;

    self->direction = FLIP_NONE;
    self->alpha     = 3 * self->lightsTimer;

    drawPos.y = self->position.y;
    for (drawPos.x = 0x200000; drawPos.x < (ScreenInfo->size.x + 32) << 16; drawPos.x += 0x400000) {
        self->inkEffect            = INK_NONE;
        self->mainAnimator.frameID = 0;
        RSDK.DrawSprite(&self->mainAnimator, &drawPos, true);

        self->inkEffect            = INK_ADD;
        self->mainAnimator.frameID = 1;
        RSDK.DrawSprite(&self->mainAnimator, &drawPos, true);
    }

    self->alpha     = self->lightsTimer;
    self->direction = self->parent->direction;
    RSDK.DrawSprite(&self->eggmanAnimator, &self->parent->position, false);

    drawPos.x   = (self->parent->position.x >> 16) - ScreenInfo->position.x;
    drawPos.y   = (self->parent->position.y >> 16) - ScreenInfo->position.y;
    color color = RSDK.GetPaletteEntry(0, 151);

    if (self->direction) {
        RSDK.DrawRect(0, 0, drawPos.x - 36, ScreenInfo->size.y, color, self->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x + 28, 0, ScreenInfo->size.x, ScreenInfo->size.y, color, self->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x - 36, 0, 64, drawPos.y - 29, color, self->alpha, INK_ADD, true);
    }
    else {
        RSDK.DrawRect(0, 0, drawPos.x - 28, ScreenInfo->size.y, color, self->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x + 36, 0, ScreenInfo->size.x, ScreenInfo->size.y, color, self->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x - 28, 0, 64, drawPos.y - 29, color, self->alpha, INK_ADD, true);
    }
}

void WeatherMobile_Draw_Lightning(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.DrawSprite(&self->decorAnimator, &self->origin, false);
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void WeatherMobile_State_SetupArena(void)
{
    RSDK_THIS(WeatherMobile);

    if (++self->timer >= 8) {
        self->timer                 = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;

        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0] = ScreenInfo->center.x + (self->position.x >> 16);
        Zone->cameraBoundsT[0] = (self->position.y >> 16) - ScreenInfo->size.y;
        Zone->cameraBoundsB[0] = (self->position.y >> 16);

        WeatherMobile->boundsL = (Zone->cameraBoundsL[0] + 64) << 16;
        WeatherMobile->boundsR = (Zone->cameraBoundsR[0] - 64) << 16;
        WeatherMobile->boundsM = self->position.x;
        WeatherMobile->boundsT = (Zone->cameraBoundsT[0] + 48) << 16;
        WeatherMobile->boundsB = (Zone->cameraBoundsB[0] - 96) << 16;

        self->state = WeatherMobile_State_StartBoss;
    }
}

void WeatherMobile_State_StartBoss(void)
{
    RSDK_THIS(WeatherMobile);

    if (self->timer) {
        if (++self->timer == 120) {
            self->position.x += 0x800000;
            self->timer   = 0;
            self->visible = true;
            self->state   = WeatherMobile_State_EnterEggman;
            self->position.y += -0x400000 - (ScreenInfo->size.y << 16);

            foreach_active(WeatherTV, weatherTV)
            {
                weatherTV->state     = WeatherTV_State_TurnOn;
                weatherTV->stateDraw = WeatherTV_Draw_TurningOn;
                weatherTV->eggman    = self;
                self->weatherTV      = weatherTV;
            }
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
            Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
            ++self->timer;
        }
    }
}

void WeatherMobile_State_EnterEggman(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->radarDishLAnimator);
    RSDK.ProcessAnimation(&self->radarDishRAnimator);

    self->position.y += 0x8000;

    if (self->position.y >= WeatherMobile->boundsT) {
        self->position.y = WeatherMobile->boundsT;
        self->origin.x   = self->position.x;
        self->origin.y   = self->position.y;
        self->velocity.x = -0xC000;
        self->state      = WeatherMobile_State_HandleMovement;
    }

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State_HandleMovement(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->radarDishLAnimator);
    RSDK.ProcessAnimation(&self->radarDishRAnimator);

    self->position.x += self->velocity.x;
    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 3, 11);

    if (self->origin.y < WeatherMobile->boundsT) {
        self->origin.y += 0x10000;
        if (self->origin.y >= WeatherMobile->boundsT) {
            self->velocity.y = 0;
            self->origin.y   = WeatherMobile->boundsT;
        }
    }
    else if (self->origin.y > WeatherMobile->boundsT) {
        self->origin.y -= 0x10000;
        if (self->origin.y <= WeatherMobile->boundsT) {
            self->velocity.y = 0;
            self->origin.y   = WeatherMobile->boundsT;
        }
    }

    if (self->direction) {
        if (self->position.x > WeatherMobile->boundsR) {
            self->direction ^= FLIP_X;
            self->velocity.x = -self->velocity.x;
        }
    }
    else if (self->position.x < WeatherMobile->boundsL) {
        self->direction  = FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State_WindAttack(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->radarDishLAnimator);
    RSDK.ProcessAnimation(&self->radarDishRAnimator);

    self->position.x += 4 * self->velocity.x;
    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 3, 11);

    if (self->origin.y > WeatherMobile->boundsT - 0x400000)
        self->origin.y -= 0x10000;

    if (self->direction) {
        if (self->position.x > WeatherMobile->boundsR) {
            self->direction ^= FLIP_X;
            self->velocity.x = -self->velocity.x;
        }
    }
    else if (self->position.x < WeatherMobile->boundsL) {
        self->direction  = FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State_SunAttack(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->radarDishLAnimator);
    RSDK.ProcessAnimation(&self->radarDishRAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 3, 11);
    self->origin.y += self->velocity.y;

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_StateLights_EnterLights(void)
{
    RSDK_THIS(WeatherMobile);

    EntityWeatherMobile *parent = self->parent;

    if (parent->state == WeatherMobile_State_Defeated || parent->state == WeatherMobile_State_DefeatFall
        || parent->state == WeatherMobile_State_FleeRise || parent->state == WeatherMobile_State_FleeAdjust
        || parent->state == WeatherMobile_State_Flee) {
        self->timer = 0;
        self->state = WeatherMobile_StateLights_DimAndDissapear;
    }
    else {
        if (self->position.y >= 0x100000) {
            if (self->lightsTimer >= 64) {
                parent->velocity.y = 0x8000;
                self->state        = WeatherMobile_StateLights_Shine;
            }
            else {
                self->lightsTimer++;
            }
        }
        else {
            self->position.y += 0x8000;
            if (self->position.y >= 0x100000) {
                SPZ2Setup->fgLow->scrollInfo[0].deform  = true;
                SPZ2Setup->fgHigh->scrollInfo[0].deform = true;
                parent->velocity.y                      = 0;
                parent->state                           = WeatherMobile_State_SunAttack;
            }
        }
    }
}

void WeatherMobile_StateLights_Shine(void)
{
    RSDK_THIS(WeatherMobile);

    EntityWeatherMobile *parent = self->parent;

    if (parent->state == WeatherMobile_State_Defeated || parent->state == WeatherMobile_State_DefeatFall
        || parent->state == WeatherMobile_State_FleeRise || parent->state == WeatherMobile_State_FleeAdjust
        || parent->state == WeatherMobile_State_Flee) {
        self->timer = 0;
        self->state = WeatherMobile_StateLights_DimAndDissapear;
    }
    else {
        foreach_active(Player, player)
        {
            if ((abs(parent->position.x - player->position.x) >= 0x200000 || player->position.y - parent->position.y + 0x200000 < 0))
                Player_ElementHurt(player, self, SHIELD_FIRE);
        }

        if (!(self->timer & 7)) {
            int32 x          = parent->position.x + RSDK.Rand(-0x140000, 0x140000);
            int32 y          = parent->position.y + RSDK.Rand(-0x180000, -0x140000);
            EntityDust *dust = CREATE_ENTITY(Dust, self, x, y);
            dust->state      = Dust_State_DustPuff;
            dust->drawGroup  = parent->drawGroup;
        }

        if (++self->timer == 120) {
            self->timer        = 0;
            parent->velocity.y = -0x10000;
            self->state        = WeatherMobile_StateLights_DimAndDissapear;
        }
    }
}

void WeatherMobile_StateLights_DimAndDissapear(void)
{
    RSDK_THIS(WeatherMobile);

    if (self->lightsTimer <= 0) {
        if (self->position.y <= -0x180000)
            destroyEntity(self);
        else
            self->position.y -= 0x10000;
    }
    else {
        if (!--self->lightsTimer) {
            SPZ2Setup->fgLow->scrollInfo[0].deform  = false;
            SPZ2Setup->fgHigh->scrollInfo[0].deform = false;

            EntityWeatherMobile *parent = self->parent;
            if (parent->health > 0)
                parent->state = WeatherMobile_State_HandleMovement;
        }
    }
}

void WeatherMobile_StateWindManager_SetupWind(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer & 3)) {
        for (int32 i = 0; i < 4; ++i) {
            int32 x                    = self->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int32 y                    = self->position.y + 0xD00000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_WIND), x, y);

            if (child->mainAnimator.animationID == 13)
                child->velocity.y = self->velocity.y;
            else
                child->velocity.y = self->velocity.y >> 1;
        }
    }

    if (!self->timer) {
        EntityWeatherMobile *parent = self->parent;
        parent->state               = WeatherMobile_State_WindAttack;
        ++self->timer;
    }

    if (self->velocity.y <= -0x180000) {
        self->timer = 0;
        Zone->cameraBoundsT[0] -= 128;
        self->state = WeatherMobile_StateWindManager_StrongWind;
        foreach_active(TVPole, pole) { pole->state = TVPole_State_CheckGrab; }
    }
    else {
        self->velocity.y -= 0x2000;
    }
}

void WeatherMobile_StateWindManager_StrongWind(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer & 3)) {
        for (int32 i = 0; i < 4; ++i) {
            int32 x                    = self->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int32 y                    = self->position.y + 0xD00000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_WIND), x, y);

            if (child->mainAnimator.animationID == 13)
                child->velocity.y = self->velocity.y;
            else
                child->velocity.y = self->velocity.y >> 1;
        }
    }

    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static && !player->blinkTimer) {
            if (player->animator.animationID != ANI_JUMP)
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);

            player->onGround = false;
            player->velocity.y -= 0x5800;
        }
    }

    if (++self->timer == 180) {
        EntityWeatherMobile *parent = self->parent;
        parent->state               = WeatherMobile_State_HandleMovement;
        self->timer                 = 0;
        self->state                 = WeatherMobile_StateWindManager_FinishWind;
    }
}

void WeatherMobile_StateWindManager_FinishWind(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer & 3)) {
        for (int32 i = 0; i < 4; ++i) {
            int32 x                    = self->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int32 y                    = self->position.y + 0xD00000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_WIND), x, y);

            if (child->mainAnimator.animationID == 13)
                child->velocity.y = self->velocity.y;
            else
                child->velocity.y = self->velocity.y >> 1;
        }
    }

    if (self->velocity.y >= -0x80000) {
        foreach_active(TVPole, pole) { pole->state = TVPole_State_ForceRelease; }
        Zone->cameraBoundsT[0] += 128;
        destroyEntity(self);
    }
    else {
        self->velocity.y += 0x4000;
    }
}

void WeatherMobile_State_Wind(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->position.y += self->velocity.y;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void WeatherMobile_State_RainManager(void)
{
    RSDK_THIS(WeatherMobile);

    self->angle = RSDK.Sin512(self->timer++) >> 4;

    if (!(Zone->timer & 1)) {
        for (int32 i = 0; i < 4; ++i) {
            int32 x                    = self->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int32 y                    = self->position.y - 0x800000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_RAIN), x, y);
            child->angle               = self->angle;
            child->rotation            = -2 * self->angle;
        }
    }

    if (!(self->timer % 112)) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(WeatherMobile, INT_TO_VOID(WEATHERMOBILE_CLOUD), RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x,
                      WeatherMobile->boundsB - 0x480000);
    }

    if (self->timer == 360)
        destroyEntity(self);
}

void WeatherMobile_State_RainDrop(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->groundVel * RSDK.Sin256(self->angle);
    self->position.y += self->groundVel * RSDK.Cos256(self->angle);
    self->groundVel += 0x80;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0, true)) {
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 20, &self->mainAnimator, true, 0);
        self->state = WeatherMobile_State_RainDropSplash;
    }
    else if (++self->timer > 180) {
        destroyEntity(self);
    }
}

void WeatherMobile_State_RainDropSplash(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

void WeatherMobile_StateCloud_Idle(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 9);

    if (self->alpha < 0x100)
        self->alpha += 0x10;

    if (!self->timer)
        RSDK.PlaySfx(WeatherMobile->sfxCloud, false, 255);

    if (++self->timer == 60) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 16, &self->mainAnimator, true, 0);
        self->state = WeatherMobile_StateCloud_ShowTell;
    }
}

void WeatherMobile_StateCloud_ShowTell(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 9);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        RSDK.PlaySfx(WeatherMobile->sfxLightning, false, 255);
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 17, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 18, &self->decorAnimator, true, 0);
        self->stateDraw = WeatherMobile_Draw_Lightning;
        self->state     = WeatherMobile_StateCloud_Strike;
    }
}

void WeatherMobile_StateCloud_Strike(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->decorAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 9);

    if (++self->timer == 4) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, INT_TO_VOID(0xF0F0F0), self->position.x, self->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 256;
    }

    if (self->timer > 2 && self->timer < 5) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox))
                Player_ElementHurt(player, self, SHIELD_LIGHTNING);
        }
    }

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        self->timer = 0;
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 15, &self->mainAnimator, true, 0);
        self->stateDraw = StateMachine_None;
        self->state     = WeatherMobile_StateCloud_FadeAway;
    }
}

void WeatherMobile_StateCloud_FadeAway(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 9);

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 8;
}

void WeatherMobile_State_Defeated(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    WeatherMobile_Explode();

    self->timer++;
    EntityDebris *debris = NULL;
    switch (self->timer) {
        case 48:
            debris                  = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
            debris->animator        = self->radarDishLAnimator;
            debris->velocity.x      = -0x20000;
            debris->velocity.y      = -0x10000;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x800000;
            debris->updateRange.y   = 0x800000;

            debris                  = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
            debris->animator        = self->radarDishRAnimator;
            debris->velocity.x      = 0x20000;
            debris->velocity.y      = -0x10000;
            debris->gravityStrength = 0x3800;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->updateRange.x   = 0x800000;
            debris->updateRange.y   = 0x800000;
            RSDK.SetSpriteAnimation(-1, 0, &self->radarDishLAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->radarDishRAnimator, true, 0);
            break;

        case 96:
            Debris_CreateFromEntries(WeatherMobile->aniFrames, WeatherMobile->debrisInfo, 24);
            RSDK.SetSpriteAnimation(-1, 0, &self->decorAnimator, true, 0);
            break;

        case 144:
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            self->timer = 0;
            self->state = WeatherMobile_State_DefeatFall;
            WeatherTV_ShutdownTV();
            break;
    }
}

void WeatherMobile_State_DefeatFall(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (++self->timer == 38) {
        self->timer      = 0;
        self->velocity.y = 0;
        self->state      = WeatherMobile_State_FleeRise;
    }
}

void WeatherMobile_State_FleeRise(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    if (self->timer >= 48) {
        self->origin.x   = self->position.x;
        self->origin.y   = self->position.y;
        self->timer      = 0;
        self->velocity.y = 0;
        self->angle      = 0;
        self->state      = WeatherMobile_State_FleeAdjust;
    }
    else {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x800;
        self->timer++;
    }
}

void WeatherMobile_State_FleeAdjust(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 3, 11);

    if (++self->timer == 8) {
        self->timer         = 0;
        self->velocity.y    = 0;
        self->direction     = FLIP_X;
        self->updateRange.x = 0x1000000;
        self->state         = WeatherMobile_State_Flee;
    }
}

void WeatherMobile_State_Flee(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 3, 11);

    self->position.x += 0x40000;
    self->origin.y -= 0x4000;

    if (!RSDK.CheckOnScreen(self, NULL)) {
        RSDK.ResetEntity(self, EggPrison->classID, INT_TO_VOID(EGGPRISON_FLYING));
        self->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        self->position.y = (ScreenInfo->position.y - 48) << 16;
    }
}

#if GAME_INCLUDE_EDITOR
void WeatherMobile_EditorDraw(void)
{
    RSDK_THIS(WeatherMobile);

    self->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 4, &self->seatAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 6, &self->eggMobileAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 23, &self->decorAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 21, &self->radarDishLAnimator, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 22, &self->radarDishRAnimator, true, 0);

    WeatherMobile_Draw_Eggman();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 0, 1 | 2 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void WeatherMobile_EditorLoad(void)
{
    WeatherMobile->aniFrames    = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE);
    WeatherMobile->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggMobile.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(WeatherMobile, type);
    RSDK_ENUM_VAR("Weather Mobile", WEATHERMOBILE_EGGMAN);
}
#endif

void WeatherMobile_Serialize(void) { RSDK_EDITABLE_VAR(WeatherMobile, VAR_ENUM, type); }
