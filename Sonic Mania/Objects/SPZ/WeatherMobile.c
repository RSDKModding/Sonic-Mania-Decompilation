#include "SonicMania.h"

ObjectWeatherMobile *WeatherMobile;

void WeatherMobile_Update(void)
{
    RSDK_THIS(WeatherMobile);
    StateMachine_Run(entity->state);
}

void WeatherMobile_LateUpdate(void) {}

void WeatherMobile_StaticUpdate(void)
{
    int windCount1    = 0;
    int windCount2    = 0;
    int rainCount = 0;
    int windCount = 0;

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        foreach_all(WeatherMobile, weatherMobile)
        {
            if (weatherMobile->type == WEATHERMOBILE_RAIN)
                rainCount++;

            if (weatherMobile->type == WEATHERMOBILE_WIND_MANAGER) {
                ++windCount1;
                if (weatherMobile->state == WeatherMobile_State2_Unknown3)
                    windCount++;
            }

            if (weatherMobile->type == WEATHERMOBILE_WIND)
                windCount2++;
        }
    }

    if (rainCount) {
        if (!WeatherMobile->playingRainSfx) {
            RSDK.PlaySfx(WeatherMobile->sfxRain, true, 255);
            WeatherMobile->playingRainSfx = true;
        }
    }
    else if (WeatherMobile->playingRainSfx) {
        RSDK.StopSFX(WeatherMobile->sfxRain);
        WeatherMobile->playingRainSfx = false;
    }

    if (windCount || WeatherMobile->windSfxTimer > 0) {
        int timer = WeatherMobile->windSfxTimer;
        if (timer < 60)
            RSDK.SetChannelAttributes(WeatherMobile->windSfxChannel, (60 - timer) / 60.0, 0.0, (((60 - timer) * 0.5) / 60.0) + 0.5);
        ++WeatherMobile->windSfxTimer;
    }
    if ((windCount1 || windCount2) && !WeatherMobile->playingWindSfx) {
        WeatherMobile->windSfxChannel = RSDK.PlaySfx(WeatherMobile->sfxWind, 60067, 255);
        WeatherMobile->playingWindSfx = true;
        WeatherMobile->windSfxTimer   = 0;
    }
    else if (!windCount1 && !windCount2) {
        if (WeatherMobile->playingWindSfx) {
            RSDK.StopSFX(WeatherMobile->sfxWind);
            WeatherMobile->playingWindSfx = false;
        }
    }
}

void WeatherMobile_Draw(void)
{
    RSDK_THIS(WeatherMobile);
    if (entity->stateDraw) {
        StateMachine_Run(entity->stateDraw);
    }
    else {
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void WeatherMobile_Create(void *data)
{
    RSDK_THIS(WeatherMobile);

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->type          = voidToInt(data);
            switch (entity->type) {
                case WEATHERMOBILE_EGGMAN:
                    entity->visible       = false;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->hitbox.left   = -22;
                    entity->hitbox.top    = -22;
                    entity->hitbox.right  = 22;
                    entity->hitbox.bottom = 22;
                    entity->drawFX        = FX_FLIP;
                    entity->health        = 8;
                    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 4, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 0, &entity->animator3, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 6, &entity->animator4, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 23, &entity->animator5, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 21, &entity->animator6, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 22, &entity->animator7, true, 0);
                    entity->stateDraw = WeatherMobile_StateDraw_Unknown1;
                    entity->state     = WeatherMobile_State_SetupArena;
                    break;
                case WEATHERMOBILE_LIGHTS:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderHigh;
                    entity->drawFX    = FX_FLIP;
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 12, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 12, &entity->animator3, true, 2);
                    entity->position.y = -0x180000;
                    RSDK.PlaySfx(WeatherMobile->sfxSun, false, 255);
                    entity->stateDraw = WeatherMobile_StateDraw1_Unknown;
                    entity->state     = WeatherMobile_State1_Unknown1;
                    break;
                case WEATHERMOBILE_WIND_MANAGER:
                    entity->active     = ACTIVE_NORMAL;
                    entity->velocity.y = -0x80000;
                    entity->state      = WeatherMobile_State2_Unknown1;
                    break;
                case WEATHERMOBILE_WIND:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderHigh;
                    if (RSDK.Rand(0, 8) == 4) {
                        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 14, &entity->animator1, true, 0);
                    }
                    else {
                        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 13, &entity->animator1, true, RSDK.Rand(0, 3));
                        entity->drawFX    = FX_SCALE;
                        entity->inkEffect = INK_ADD;
                        entity->alpha     = 0xC0;
                        entity->scale.x   = 0x200;
                        entity->scale.y   = 0x400;
                    }
                    entity->state = WeatherMobile_State3_Unknown;
                    break;
                case WEATHERMOBILE_RAIN_MANAGER:
                    entity->active = ACTIVE_NORMAL;
                    entity->angle  = 32;
                    entity->state  = WeatherMobile_State4_Unknown;
                    break;
                case WEATHERMOBILE_RAIN:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderHigh;
                    entity->drawFX    = FX_ROTATE;
                    entity->groundVel = 512;
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 19, &entity->animator1, true, 0);
                    entity->state = WeatherMobile_State5_Unknown1;
                    break;
                case WEATHERMOBILE_CLOUD:
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = 0;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 128;
                    entity->inkEffect     = INK_ALPHA;
                    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 15, &entity->animator1, true, 0);
                    entity->drawPos = entity->position;
                    entity->state   = WeatherMobile_State6_Unknown1;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void WeatherMobile_StageLoad(void)
{
    WeatherMobile->active       = ACTIVE_ALWAYS;
    WeatherMobile->aniFrames    = RSDK.LoadSpriteAnimation("SPZ2/WeatherMobile.bin", SCOPE_STAGE);
    WeatherMobile->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggMobile.bin", SCOPE_STAGE);
    WeatherMobile->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    WeatherMobile->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    WeatherMobile->sfxCloud     = RSDK.GetSFX("SPZ2/Cloud.wav");
    WeatherMobile->sfxLightning = RSDK.GetSFX("SPZ2/Lightning.wav");
    WeatherMobile->sfxSun       = RSDK.GetSFX("SPZ2/Sun.wav");
    WeatherMobile->sfxWind      = RSDK.GetSFX("SPZ2/Wind.wav");
    WeatherMobile->sfxRain      = RSDK.GetSFX("FBZ/Rain.wav");
}

void WeatherMobile_HandleAnimations(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->invincibilityTimer > 0)
        entity->invincibilityTimer--;

    if (entity->animator3.animationID) {
        if (entity->animator3.animationID == 1 || (entity->animator3.animationID - 1) == 1) {
            if (entity->animator3.frameID == entity->animator3.frameCount - 1)
                RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 0, &entity->animator3, true, 0);
        }
    }
    else {
        entity->animator3.animationSpeed = 0;
    }
}

void WeatherMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(WeatherMobile);

    foreach_active(Player, player)
    {
        if (!entity->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBossHit(player, entity)) {
            if (--entity->health <= 0) {
                WeatherMobile_HandleDefeat();
            }
            else {
                entity->invincibilityTimer = 48;
                RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 2, &entity->animator3, true, 0);
                RSDK.PlaySfx(WeatherMobile->sfxHit, false, 255);
            }
        }
    }
}

void WeatherMobile_HandleDefeat(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 3, &entity->animator3, true, 0);
    entity->state = WeatherMobile_State_Defeated;
    entity->timer = 0;

    foreach_active(WeatherMobile, weatherMobile)
    {
        switch (weatherMobile->type) {
            case WEATHERMOBILE_LIGHTS:
                if (weatherMobile->state == WeatherMobile_State1_Unknown2) {
                    weatherMobile->timer = 0;
                    weatherMobile->state = WeatherMobile_State1_Unknown3;
                }
                break;
            case WEATHERMOBILE_WIND_MANAGER:
            case WEATHERMOBILE_RAIN_MANAGER:
                destroyEntity(weatherMobile);
                Zone->screenBoundsT1[0] = Zone->screenBoundsB1[0] - RSDK_screens->height;
                break;
            case WEATHERMOBILE_CLOUD:
                if (weatherMobile->state != WeatherMobile_State6_Unknown4) {
                    weatherMobile->timer     = 0;
                    weatherMobile->stateDraw = StateMachine_None;
                    weatherMobile->state     = WeatherMobile_State6_Unknown4;
                }
                break;
            default: break;
        }
    }

    foreach_active(TVPole, pole) { pole->state = TVPole_State_ForceRelease; }
    RSDK_sceneInfo->timeEnabled = false;
    Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
}

void WeatherMobile_StateDraw_Unknown1(void)
{
    RSDK_THIS(WeatherMobile);

    if (entity->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 32, 0, 32, 10);
        RSDK.SetPaletteEntry(0, 128, 0xF0F0F0);
        RSDK.DrawSprite(&entity->animator6, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator5, NULL, false);
        RSDK.DrawSprite(&entity->animator7, NULL, false);
        RSDK.CopyPalette(1, 32, 0, 32, 10);
        RSDK.SetPaletteEntry(0, 128, 0x000000);
    }
    else {
        RSDK.DrawSprite(&entity->animator6, NULL, false);
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator3, NULL, false);
        RSDK.DrawSprite(&entity->animator4, NULL, false);
        RSDK.DrawSprite(&entity->animator5, NULL, false);
        RSDK.DrawSprite(&entity->animator7, NULL, false);
    }
}

void WeatherMobile_StateDraw1_Unknown(void)
{
    RSDK_THIS(WeatherMobile);
    Vector2 drawPos;

    entity->direction = FLIP_NONE;
    entity->alpha     = 3 * entity->timer2;
    drawPos.y         = entity->position.y;
    for (drawPos.x = 0x200000; drawPos.x < (RSDK_screens->width + 32) << 16; drawPos.x += 0x400000) {
        entity->inkEffect         = INK_NONE;
        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, &drawPos, true);

        entity->inkEffect         = INK_ADD;
        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, &drawPos, true);
    }

    entity->alpha     = entity->timer2;
    entity->direction = entity->parent->direction;
    RSDK.DrawSprite(&entity->animator3, &entity->parent->position, false);

    drawPos.x     = (entity->parent->position.x >> 16) - RSDK_screens->position.x;
    drawPos.y     = (entity->parent->position.y >> 16) - RSDK_screens->position.y;
    colour colour = RSDK.GetPaletteEntry(0, 151);
    if (entity->direction) {
        RSDK.DrawRect(0, 0, drawPos.x - 36, RSDK_screens->height, colour, entity->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x + 28, 0, RSDK_screens->width, RSDK_screens->height, colour, entity->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x - 36, 0, 64, drawPos.y - 29, colour, entity->alpha, INK_ADD, true);
    }
    else {
        RSDK.DrawRect(0, 0, drawPos.x - 28, RSDK_screens->height, colour, entity->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x + 36, 0, RSDK_screens->width, RSDK_screens->height, colour, entity->alpha, INK_ADD, true);
        RSDK.DrawRect(drawPos.x - 28, 0, 64, drawPos.y - 29, colour, entity->alpha, INK_ADD, true);
    }
}

void WeatherMobile_StateDraw6_Unknown1(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.DrawSprite(&entity->animator5, &entity->drawPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void WeatherMobile_State_SetupArena(void)
{
    RSDK_THIS(WeatherMobile);

    if (++entity->timer >= 8) {
        entity->timer               = 0;
        Zone->playerBoundActiveL[0] = true;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->screenBoundsR1[0]     = RSDK_screens->centerX + (entity->position.x >> 16);
        Zone->screenBoundsT1[0]     = (entity->position.y >> 16) - RSDK_screens->height;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16);
        WeatherMobile->boundsL      = (Zone->screenBoundsL1[0] + 64) << 16;
        WeatherMobile->boundsR      = (Zone->screenBoundsR1[0] - 64) << 16;
        WeatherMobile->boundsM      = entity->position.x;
        WeatherMobile->boundsT      = (Zone->screenBoundsT1[0] + 48) << 16;
        WeatherMobile->boundsB      = (Zone->screenBoundsB1[0] - 96) << 16;
        entity->state               = WeatherMobile_State_StartBoss;
    }
}

void WeatherMobile_State_StartBoss(void)
{
    RSDK_THIS(WeatherMobile);

    if (entity->timer) {
        entity->timer++;
        if (entity->timer == 120) {
            entity->position.x += 0x800000;
            entity->timer   = 0;
            entity->visible = true;
            entity->state   = WeatherMobile_State_Unknown2;
            entity->position.y += -0x400000 - (RSDK_screens->height << 16);

            foreach_active(WeatherTV, weatherTV)
            {
                weatherTV->state     = WeatherTV_State_Unknown1;
                weatherTV->stateDraw = WeatherTV_StateDraw_Unknown2;
                weatherTV->eggman    = entity;
                entity->weatherTV    = (Entity *)weatherTV;
            }
        }
    }
    else {
        if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
            Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
            ++entity->timer;
        }
    }
}

void WeatherMobile_State_Unknown2(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator6);
    RSDK.ProcessAnimation(&entity->animator7);
    entity->position.y += 0x8000;

    if (entity->position.y >= WeatherMobile->boundsT) {
        entity->position.y = WeatherMobile->boundsT;
        entity->drawPos.x  = entity->position.x;
        entity->drawPos.y  = entity->position.y;
        entity->velocity.x = -0xC000;
        entity->state      = WeatherMobile_State_Unknown3;
    }
    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State_Unknown3(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator6);
    RSDK.ProcessAnimation(&entity->animator7);

    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 3, 11);
    entity->position.x += entity->velocity.x;

    if (entity->drawPos.y < WeatherMobile->boundsT) {
        entity->drawPos.y += 0x10000;
        if (entity->drawPos.y >= WeatherMobile->boundsT) {
            entity->velocity.y = 0;
            entity->drawPos.y  = WeatherMobile->boundsT;
        }
    }
    else if (entity->drawPos.y > WeatherMobile->boundsT) {
        entity->drawPos.y -= 0x10000;
        if (entity->drawPos.y <= WeatherMobile->boundsT) {
            entity->velocity.y = 0;
            entity->drawPos.y  = WeatherMobile->boundsT;
        }
    }

    if (entity->direction) {
        if (entity->position.x > WeatherMobile->boundsR) {
            entity->direction ^= FLIP_X;
            entity->velocity.x = -entity->velocity.x;
        }
    }
    else if (entity->position.x < WeatherMobile->boundsL) {
        entity->direction  = FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State_Unknown4(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator6);
    RSDK.ProcessAnimation(&entity->animator7);

    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 3, 11);
    entity->position.x += 4 * entity->velocity.x;

    if (entity->drawPos.y > WeatherMobile->boundsT - 0x400000)
        entity->drawPos.y -= 0x10000;

    if (entity->direction) {
        if (entity->position.x > WeatherMobile->boundsR) {
            entity->direction ^= FLIP_X;
            entity->velocity.x = -entity->velocity.x;
        }
    }
    else if (entity->position.x < WeatherMobile->boundsL) {
        entity->direction  = FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State_Unknown5(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator6);
    RSDK.ProcessAnimation(&entity->animator7);

    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 3, 11);
    entity->drawPos.y += entity->velocity.y;

    WeatherMobile_HandleAnimations();
    WeatherMobile_CheckPlayerCollisions();
}

void WeatherMobile_State1_Unknown1(void)
{
    RSDK_THIS(WeatherMobile);
    EntityWeatherMobile *parent = (EntityWeatherMobile *)entity->parent;

    if (parent->state == WeatherMobile_State_Defeated || parent->state == WeatherMobile_State_Unknown6
        || parent->state == WeatherMobile_State_Unknown7 || parent->state == WeatherMobile_State_Unknown8
        || parent->state == WeatherMobile_State_Unknown9) {
        entity->timer = 0;
        entity->state = WeatherMobile_State1_Unknown3;
    }
    else {
        if (entity->position.y >= 0x100000) {
            if (entity->timer2 >= 64) {
                parent->velocity.y = 0x8000;
                entity->state      = WeatherMobile_State1_Unknown2;
            }
            else {
                entity->timer2++;
            }
        }
        else {
            entity->position.y += 0x8000;
            if (entity->position.y >= 0x100000) {
                SPZ2Setup->fgLow->scrollInfo[0].deform  = true;
                SPZ2Setup->fgHigh->scrollInfo[0].deform = true;
                parent->velocity.y                      = 0;
                parent->state                           = WeatherMobile_State_Unknown5;
            }
        }
    }
}

void WeatherMobile_State1_Unknown2(void)
{
    RSDK_THIS(WeatherMobile);
    EntityWeatherMobile *parent = (EntityWeatherMobile *)entity->parent;

    if (parent->state == WeatherMobile_State_Defeated || parent->state == WeatherMobile_State_Unknown6
        || parent->state == WeatherMobile_State_Unknown7 || parent->state == WeatherMobile_State_Unknown8
        || parent->state == WeatherMobile_State_Unknown9) {
        entity->timer = 0;
        entity->state = WeatherMobile_State1_Unknown3;
    }
    else {
        foreach_active(Player, player)
        {
            if ((abs(parent->position.x - player->position.x) >= 0x200000 || player->position.y - parent->position.y + 0x200000 < 0)) {
                Player_CheckElementalHit(player, entity, SHIELD_FIRE);
            }
        }

        if (!(entity->timer & 7)) {
            int x            = parent->position.x + RSDK.Rand(-0x140000, 0x140000);
            int y            = parent->position.y + RSDK.Rand(-0x180000, -0x140000);
            EntityDust *dust = CREATE_ENTITY(Dust, entity, x, y);
            dust->state      = Dust_State_DropDash;
            dust->drawOrder  = parent->drawOrder;
        }
        if (++entity->timer == 120) {
            entity->timer      = 0;
            parent->velocity.y = -0x10000;
            entity->state      = WeatherMobile_State1_Unknown3;
        }
    }
}

void WeatherMobile_State1_Unknown3(void)
{
    RSDK_THIS(WeatherMobile);

    if (entity->timer2 <= 0) {
        if (entity->position.y <= -0x180000) {
            destroyEntity(entity);
        }
        else {
            entity->position.y -= 0x10000;
        }
    }
    else {
        if (!--entity->timer2) {
            SPZ2Setup->fgLow->scrollInfo[0].deform  = false;
            SPZ2Setup->fgHigh->scrollInfo[0].deform = false;
            EntityWeatherMobile *parent             = (EntityWeatherMobile *)entity->parent;
            if (parent->health > 0)
                parent->state = WeatherMobile_State_Unknown3;
        }
    }
}

void WeatherMobile_State2_Unknown1(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer & 3)) {
        for (int i = 0; i < 4; ++i) {
            int x                      = entity->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int y                      = entity->position.y + 0xD00000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_WIND), x, y);
            if (child->animator1.animationID == 13)
                child->velocity.y = entity->velocity.y;
            else
                child->velocity.y = entity->velocity.y >> 1;
        }
    }

    if (!entity->timer) {
        EntityWeatherMobile *parent = (EntityWeatherMobile *)entity->parent;
        parent->state               = WeatherMobile_State_Unknown4;
        ++entity->timer;
    }

    if (entity->velocity.y <= -0x180000) {
        entity->timer = 0;
        Zone->screenBoundsT1[0] -= 128;
        entity->state = WeatherMobile_State2_Unknown2;
        foreach_active(TVPole, pole) { pole->state = TVPole_State_CheckGrab; }
    }
    else {
        entity->velocity.y -= 0x2000;
    }
}

void WeatherMobile_State2_Unknown2(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer & 3)) {
        for (int i = 0; i < 4; ++i) {
            int x                      = entity->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int y                      = entity->position.y + 0xD00000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_WIND), x, y);
            if (child->animator1.animationID == 13)
                child->velocity.y = entity->velocity.y;
            else
                child->velocity.y = entity->velocity.y >> 1;
        }
    }

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None && !player->blinkTimer) {
            if (player->playerAnimator.animationID != ANI_JUMP)
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->playerAnimator, false, 0);
            player->onGround = false;
            player->velocity.y -= 0x5800;
        }
    }

    if (++entity->timer == 180) {
        EntityWeatherMobile *parent = (EntityWeatherMobile *)entity->parent;
        parent->state               = WeatherMobile_State_Unknown3;
        entity->timer               = 0;
        entity->state               = WeatherMobile_State2_Unknown3;
    }
}

void WeatherMobile_State2_Unknown3(void)
{
    RSDK_THIS(WeatherMobile);

    if (!(Zone->timer & 3)) {
        for (int i = 0; i < 4; ++i) {
            int x                      = entity->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int y                      = entity->position.y + 0xD00000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_WIND), x, y);
            if (child->animator1.animationID == 13)
                child->velocity.y = entity->velocity.y;
            else
                child->velocity.y = entity->velocity.y >> 1;
        }
    }

    if (entity->velocity.y >= -0x80000) {
        foreach_active(TVPole, pole) { pole->state = TVPole_State_ForceRelease; }
        Zone->screenBoundsT1[0] += 128;
        destroyEntity(entity);
    }
    else {
        entity->velocity.y += 0x4000;
    }
}

void WeatherMobile_State3_Unknown(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y += entity->velocity.y;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void WeatherMobile_State4_Unknown(void)
{
    RSDK_THIS(WeatherMobile);
    entity->angle = RSDK.Sin512(entity->timer++) >> 4;

    if (!(Zone->timer & 1)) {
        for (int i = 0; i < 4; ++i) {
            int x                      = entity->position.x + RSDK.Rand(-0x1C00000, 0x1C00000);
            int y                      = entity->position.y - 0x800000;
            EntityWeatherMobile *child = CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_RAIN), x, y);
            child->angle               = entity->angle;
            child->rotation            = -2 * entity->angle;
        }
    }

    if (!(entity->timer % 112)) {
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        CREATE_ENTITY(WeatherMobile, intToVoid(WEATHERMOBILE_CLOUD), RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x,
                      WeatherMobile->boundsB - 0x480000);
    }
    if (entity->timer == 360)
        destroyEntity(entity);
}

void WeatherMobile_State5_Unknown1(void)
{
    RSDK_THIS(WeatherMobile);
    RSDK.ProcessAnimation(&entity->animator1);

    entity->position.x += entity->groundVel * RSDK.Sin256(entity->angle);
    entity->position.y += entity->groundVel * RSDK.Cos256(entity->angle);
    entity->groundVel += 0x80;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true)) {
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 20, &entity->animator1, true, 0);
        entity->state = WeatherMobile_State5_Unknown2;
    }
    else if (++entity->timer > 180) {
        destroyEntity(entity);
    }
}

void WeatherMobile_State5_Unknown2(void)
{
    RSDK_THIS(WeatherMobile);
    RSDK.ProcessAnimation(&entity->animator1);

    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void WeatherMobile_State6_Unknown1(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 2, 9);

    if (entity->alpha < 0x100)
        entity->alpha += 0x10;

    if (!entity->timer)
        RSDK.PlaySfx(WeatherMobile->sfxCloud, false, 255);

    if (++entity->timer == 60) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 16, &entity->animator1, true, 0);
        entity->state = WeatherMobile_State6_Unknown2;
    }
}

void WeatherMobile_State6_Unknown2(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 2, 9);

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.PlaySfx(WeatherMobile->sfxLightning, false, 255);
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 17, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 18, &entity->animator5, true, 0);
        entity->stateDraw = WeatherMobile_StateDraw6_Unknown1;
        entity->state     = WeatherMobile_State6_Unknown3;
    }
}

void WeatherMobile_State6_Unknown3(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator5);
    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 2, 9);

    if (++entity->timer == 4) {
        EntityFXFade *fxFade = CREATE_ENTITY(FXFade, intToVoid(0xF0F0F0), entity->position.x, entity->position.y);
        fxFade->speedIn      = 256;
        fxFade->speedOut     = 256;
    }

    if (entity->timer > 2 && entity->timer < 5) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
            }
        }
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        entity->timer = 0;
        RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 15, &entity->animator1, true, 0);
        entity->stateDraw = 0;
        entity->state     = WeatherMobile_State6_Unknown4;
    }
}

void WeatherMobile_State6_Unknown4(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 2, 9);

    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 8;
}

void WeatherMobile_State_Defeated(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator3);

    if (!(Zone->timer % 3)) {
        RSDK.PlaySfx(WeatherMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x = entity->position.x + (RSDK.Rand(entity->hitbox.left, entity->hitbox.right) << 16);
            int y = entity->position.y + (RSDK.Rand(entity->hitbox.top, entity->hitbox.bottom) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
        }
    }

    entity->timer++;
    EntityDebris *debris = NULL;
    switch (entity->timer) {
        case 48:
            debris                = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
            debris->animator      = entity->animator6;
            debris->velocity.x    = -0x20000;
            debris->velocity.y    = -0x10000;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;

            debris                = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
            debris->animator      = entity->animator7;
            debris->velocity.x    = 0x20000;
            debris->velocity.y    = -0x10000;
            debris->gravity       = 0x3800;
            debris->drawOrder     = Zone->drawOrderHigh;
            debris->updateRange.x = 0x800000;
            debris->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator6, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator7, true, 0);
            break;
        case 96:
            Debris_FallFlickerAnimSetup(WeatherMobile->aniFrames, WeatherMobile->debrisInfo, 24);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator5, true, 0);
            break;
        case 144:
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
            entity->timer = 0;
            entity->state = WeatherMobile_State_Unknown6;
            WeatherTV_ShutdownTV();
            break;
    }
}

void WeatherMobile_State_Unknown6(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator3);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    if (++entity->timer == 38) {
        entity->timer      = 0;
        entity->velocity.y = 0;
        entity->state      = WeatherMobile_State_Unknown7;
    }
}

void WeatherMobile_State_Unknown7(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator3);

    if (entity->timer >= 48) {
        entity->drawPos.x  = entity->position.x;
        entity->drawPos.y  = entity->position.y;
        entity->timer      = 0;
        entity->velocity.y = 0;
        entity->angle      = 0;
        entity->state      = WeatherMobile_State_Unknown8;
    }
    else {
        entity->position.y += entity->velocity.y;
        entity->velocity.y -= 0x800;
        entity->timer++;
    }
}

void WeatherMobile_State_Unknown8(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator3);
    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 3, 11);
    if (++entity->timer == 8) {
        entity->timer         = 0;
        entity->velocity.y    = 0;
        entity->direction     = FLIP_X;
        entity->updateRange.x = 0x1000000;
        entity->state         = WeatherMobile_State_Unknown9;
    }
}

void WeatherMobile_State_Unknown9(void)
{
    RSDK_THIS(WeatherMobile);

    RSDK.ProcessAnimation(&entity->animator3);
    entity->position.y = BadnikHelpers_Oscillate(entity->drawPos.y, 3, 11);

    entity->position.x += 0x40000;
    entity->drawPos.y -= 0x4000;

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        RSDK.ResetEntityPtr(entity, EggPrison->objectID, intToVoid(EGGPRISON_FLYING));
        entity->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        entity->position.y = (RSDK_screens->position.y - 48) << 16;
    }
}

#if RETRO_INCLUDE_EDITOR
void WeatherMobile_EditorDraw(void)
{
    RSDK_THIS(WeatherMobile);

    entity->drawFX = FX_FLIP;
    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 4, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 0, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->eggmanFrames, 6, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 23, &entity->animator5, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 21, &entity->animator6, true, 0);
    RSDK.SetSpriteAnimation(WeatherMobile->aniFrames, 22, &entity->animator7, true, 0);
    WeatherMobile_StateDraw_Unknown1();

    if (showGizmos()) {
        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 2 | 4 | 8, -212, -SCREEN_YSIZE, 212, 0);
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
