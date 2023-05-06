// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Water Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWater *Water;

void Water_Update(void)
{
    RSDK_THIS(Water);

    StateMachine_Run(self->state);
}

void Water_LateUpdate(void) {}

void Water_StaticUpdate(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    if (pauseMenu->classID != PauseMenu->classID) {
        if (Water->newWaterLevel == Water->targetWaterLevel) {
            Water->moveWaterLevel = 0;

            if (Water->waterLevelVolume > 0)
                Water->waterLevelVolume--;
            Water->waterLevelVolume = CLAMP(Water->waterLevelVolume, 0, 30);
        }
        else {
            if (RSDK.CheckSceneFolder("HCZ") && Water->moveWaterLevel) {
                if (!(Water->waterLevelVolume % 5))
                    Camera_ShakeScreen(0, 0, 1);
                ++Water->waterLevelVolume;
            }

            if (Water->newWaterLevel >= Water->targetWaterLevel) {
                Water->newWaterLevel -= Water->waterMoveSpeed;
                if (Water->newWaterLevel < Water->targetWaterLevel)
                    Water->newWaterLevel = Water->targetWaterLevel;
            }
            else {
                Water->newWaterLevel += Water->waterMoveSpeed;
                if (Water->newWaterLevel > Water->targetWaterLevel)
                    Water->newWaterLevel = Water->targetWaterLevel;
            }
        }
    }

    if (Water->playingWaterLevelSfx) {
        if (Water->waterLevelVolume > 30)
            Water->waterLevelVolume = 30;
        float volume = Water->waterLevelVolume / 30.0;
        RSDK.SetChannelAttributes(Water->waterLevelChannelL, volume, -1.0, 1.0);
        RSDK.SetChannelAttributes(Water->waterLevelChannelR, volume, 1.0, 1.0);
    }

    if (Water->waterLevelVolume > 0) {
        if (!Water->playingWaterLevelSfx) {
            Water->waterLevelChannelL = RSDK.PlaySfx(Water->sfxWaterLevelL, 1, 255);
            RSDK.SetChannelAttributes(Water->waterLevelChannelL, 0.0, -1.0, 1.0);

            Water->waterLevelChannelR = RSDK.PlaySfx(Water->sfxWaterLevelR, 1, 255);
            RSDK.SetChannelAttributes(Water->waterLevelChannelR, 0.0, 1.0, 1.0);

            Water->playingWaterLevelSfx = true;
        }
    }
    else if (!Water->waterLevelVolume && Water->playingWaterLevelSfx) {
        RSDK.StopSfx(Water->sfxWaterLevelL);
        RSDK.StopSfx(Water->sfxWaterLevelR);
        Water->playingWaterLevelSfx = false;
    }

    bool32 wakeActive = false;
    for (int32 i = 0; i < PLAYER_COUNT; ++i) wakeActive |= Water->wakePosX[i] > 0;

    if (wakeActive) {
        if (pauseMenu->classID != PauseMenu->classID)
            RSDK.ProcessAnimation(&Water->wakeAnimator);

        if (!Water->playingSkimSfx) {
            RSDK.PlaySfx(Water->sfxSkim, 3570, 0xFF);
            Water->playingSkimSfx = true;
        }
    }
    else if (Water->playingSkimSfx) {
        RSDK.StopSfx(Water->sfxSkim);
        Water->playingSkimSfx = false;
    }
}

void Water_Draw(void)
{
    RSDK_THIS(Water);

    StateMachine_Run(self->stateDraw);
}

void Water_Create(void *data)
{
    RSDK_THIS(Water);

    self->visible = true;
    if (!SceneInfo->inEditor) {
        if (data)
            self->type = VOID_TO_INT(data);

        switch (self->type) {
            case WATER_WATERLEVEL:
                if (globals->gameMode == MODE_COMPETITION && RSDK.CheckSceneFolder("CPZ")) {
                    destroyEntity(self);
                }
                else {
                    self->active    = ACTIVE_NORMAL;
                    self->inkEffect = INK_ADD;
                    self->drawGroup = Zone->hudDrawGroup - 1;
                    self->alpha     = RSDK.CheckSceneFolder("AIZ") ? 0x60 : 0xE0;
                    RSDK.SetSpriteAnimation(Water->aniFrames, 0, &self->animator, true, 0);
                    self->state     = Water_State_Water;
                    self->stateDraw = Water_Draw_Water;
                    self->size.x >>= 16;
                    Water->newWaterLevel    = self->position.y;
                    Water->targetWaterLevel = self->position.y;
                }
                break;

            case WATER_POOL:
                self->active = ACTIVE_BOUNDS;
                self->drawFX = FX_FLIP;
                switch (self->priority) {
                    case WATER_PRIORITY_LOWEST: self->drawGroup = Zone->objectDrawGroup[0] - 1; break;
                    case WATER_PRIORITY_LOW: self->drawGroup = Zone->playerDrawGroup[0]; break;
                    case WATER_PRIORITY_HIGH: self->drawGroup = Zone->playerDrawGroup[1]; break;
                    case WATER_PRIORITY_HIGHEST: self->drawGroup = Zone->hudDrawGroup - 1; break;
                    default: break;
                }

                self->updateRange.x = self->size.x >> 1;
                self->updateRange.y = self->size.y >> 1;

                self->hitbox.right  = (self->size.x >> 17);
                self->hitbox.left   = -(self->size.x >> 17);
                self->hitbox.bottom = (self->size.y >> 17);
                self->hitbox.top    = -(self->size.y >> 17);

                self->state     = Water_State_Pool;
                self->stateDraw = Water_Draw_Pool;
                break;

            case WATER_BUBBLER:
            case WATER_BIG_BUBBLER:
                self->drawGroup     = Zone->objectDrawGroup[0];
                self->inkEffect     = INK_ADD;
                self->alpha         = 0x100;
                self->active        = ACTIVE_BOUNDS;
                self->updateRange.x = TO_FIXED(16);
                self->updateRange.y = TO_FIXED(16);

                RSDK.SetSpriteAnimation(Water->aniFrames, 2, &self->animator, true, 0);

                self->state     = Water_State_Bubbler;
                self->stateDraw = Water_Draw_Bubbler;
                break;

            case WATER_HEIGHT_TRIGGER:
                self->active  = ACTIVE_BOUNDS;
                self->visible = false;
                if (self->buttonTag > 0) {
                    self->active = ACTIVE_NORMAL;
                    Water_SetupTagLink();
                }

                self->updateRange.x = self->size.x >> 1;
                self->updateRange.y = self->size.y >> 1;

                self->state = Water_State_HeightTrigger;
                break;

            case WATER_BTN_BIG_BUBBLE:
                self->drawGroup     = Zone->objectDrawGroup[0];
                self->active        = ACTIVE_BOUNDS;
                self->updateRange.x = TO_FIXED(16);
                self->updateRange.y = TO_FIXED(16);

                self->state     = Water_State_BtnBigBubble;
                self->stateDraw = StateMachine_None;

                Water_SetupTagLink();
                break;

            case WATER_SPLASH:
                self->active    = ACTIVE_NORMAL;
                self->drawGroup = Zone->hudDrawGroup - 2;
                RSDK.SetSpriteAnimation(Water->aniFrames, 1, &self->animator, true, 0);

                self->state     = Water_State_Splash;
                self->stateDraw = Water_Draw_Splash;
                break;

            case WATER_BUBBLE:
                self->active        = ACTIVE_NORMAL;
                self->drawGroup     = Zone->playerDrawGroup[1];
                self->drawFX        = FX_SCALE;
                self->inkEffect     = INK_ADD;
                self->alpha         = 0x100;
                self->updateRange.x = TO_FIXED(128);
                self->updateRange.y = TO_FIXED(128);
                self->isPermanent   = true;

                self->scale.x = 0x200;
                self->scale.y = 0x200;
                RSDK.SetSpriteAnimation(Water->aniFrames, 5, &self->animator, true, 0);

                self->state     = Water_State_Bubble;
                self->stateDraw = Water_Draw_Bubble;
                break;

            case WATER_COUNTDOWN:
                self->active        = ACTIVE_NORMAL;
                self->drawGroup     = Zone->playerDrawGroup[1];
                self->drawFX        = FX_SCALE;
                self->inkEffect     = INK_ADD;
                self->alpha         = 0x100;
                self->updateRange.x = TO_FIXED(128);
                self->updateRange.y = TO_FIXED(128);

                self->scale.x = 0x200;
                self->scale.y = 0x200;
                RSDK.SetSpriteAnimation(Water->aniFrames, 7, &self->animator, true, 0);

                self->state     = Water_State_Countdown;
                self->stateDraw = Water_Draw_Countdown;
                break;

            default: break;
        }
    }
}

void Water_StageLoad(void)
{
    Water->active = ACTIVE_ALWAYS;

    Water->aniFrames = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);

    Water->waterLevel       = 0x7FFFFFFF;
    Water->newWaterLevel    = Water->waterLevel;
    Water->targetWaterLevel = Water->waterLevel;

    Water->disableWaterSplash = false;

    Water->hitboxPlayerBubble.left   = -2;
    Water->hitboxPlayerBubble.top    = -2;
    Water->hitboxPlayerBubble.right  = 2;
    Water->hitboxPlayerBubble.bottom = 2;

    Water->hitboxPoint.left   = -1;
    Water->hitboxPoint.top    = -1;
    Water->hitboxPoint.right  = 1;
    Water->hitboxPoint.bottom = 1;

    Water->sfxSplash     = RSDK.GetSfx("Stage/Splash.wav");
    Water->sfxBreathe    = RSDK.GetSfx("Stage/Breathe.wav");
    Water->sfxWarning    = RSDK.GetSfx("Stage/Warning.wav");
    Water->sfxDrown      = RSDK.GetSfx("Stage/Drown.wav");
    Water->sfxDrownAlert = RSDK.GetSfx("Stage/DrownAlert.wav");
    Water->sfxSkim       = RSDK.GetSfx("HCZ/Skim.wav");

    if (RSDK.CheckSceneFolder("HCZ")) {
        Water->wakeFrames      = RSDK.LoadSpriteAnimation("HCZ/Wake.bin", SCOPE_STAGE);
        Water->bigBubbleFrames = RSDK.LoadSpriteAnimation("HCZ/BigBubble.bin", SCOPE_STAGE);

        Water->sfxWaterLevelL = RSDK.GetSfx("HCZ/WaterLevel_L.wav");
        Water->sfxWaterLevelR = RSDK.GetSfx("HCZ/WaterLevel_R.wav");
        Water->sfxDNAGrab     = RSDK.GetSfx("CPZ/DNAGrab.wav");
        Water->sfxDNABurst    = RSDK.GetSfx("CPZ/DNABurst.wav");

        RSDK.SetSpriteAnimation(Water->wakeFrames, 0, &Water->wakeAnimator, true, 0);
    }
}

void Water_DrawHook_ApplyWaterPalette(void)
{
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];
    int32 waterDrawPos     = CLAMP(FROM_FIXED(Water->waterLevel) - screen->position.y, 0, screen->size.y);

    RSDK.SetActivePalette(Water->waterPalette, waterDrawPos, screen->size.y);
    ScreenInfo[SceneInfo->currentScreenID].waterDrawPos = waterDrawPos;
}

void Water_DrawHook_RemoveWaterPalette(void) { RSDK.SetActivePalette(0, 0, ScreenInfo[SceneInfo->currentScreenID].size.y); }

void Water_SetupTagLink(void)
{
    RSDK_THIS(Water);

    self->taggedButton = NULL;
    if (self->buttonTag > 0) {
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == self->buttonTag) {
                    self->taggedButton = button;
                    foreach_break;
                }
            }
        }

        if (PullChain && !self->taggedButton) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == self->buttonTag) {
                    self->taggedButton = (EntityButton *)chain;
                    foreach_break;
                }
            }
        }

        if (self->taggedButton) {
            if (self->updateRange.x < TO_FIXED(128) + abs(self->position.x - self->taggedButton->position.x))
                self->updateRange.x = TO_FIXED(128) + abs(self->position.x - self->taggedButton->position.x);

            if (self->updateRange.y < TO_FIXED(128) + abs(self->position.y - self->taggedButton->position.y))
                self->updateRange.y = TO_FIXED(128) + abs(self->position.y - self->taggedButton->position.y);
        }
    }
}

void Water_SpawnBubble(EntityPlayer *player, int32 id)
{
    if (Water->constBubbleTimer[id] <= 0) {
        Water->constBubbleTimer[id] = 60;
        if (RSDK.Rand(0, 3) == 1)
            Water->randBubbleTimer[id] = RSDK.Rand(0, 16) + 8;
        else
            Water->randBubbleTimer[id] = 0x200;
    }
    else {
        Water->constBubbleTimer[id]--;
        if (Water->randBubbleTimer[id] <= 0)
            Water->randBubbleTimer[id] = 0x200;
        else
            return;
    }

    EntityWater *bubble = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), player->position.x, player->position.y);
    if (player->direction) {
        bubble->position.x -= TO_FIXED(6);
        bubble->angle = 0x100;
    }
    else {
        bubble->position.x += TO_FIXED(6);
    }

    bubble->childPtr   = player;
    bubble->bubbleX    = bubble->position.x;
    bubble->velocity.y = -0x8800;
    bubble->drawGroup  = player->drawGroup + 1;
}

void Water_SpawnCountDownBubble(EntityPlayer *player, int32 id, uint8 bubbleID)
{
    EntityWater *bubble = CREATE_ENTITY(Water, INT_TO_VOID(WATER_COUNTDOWN), player->position.x, player->position.y);
    if (player->direction) {
        bubble->position.x -= TO_FIXED(6);
        bubble->angle = 0x100;
    }
    else {
        bubble->position.x += TO_FIXED(6);
    }
    bubble->bubbleX     = bubble->position.x;
    bubble->velocity.y  = -0x8800;
    bubble->childPtr    = player;
    bubble->countdownID = bubbleID;
    bubble->drawGroup   = Zone->playerDrawGroup[1] + 1;
}

void Water_State_Water(void)
{
    RSDK_THIS(Water);

    RSDK.ProcessAnimation(&self->animator);

    if (self->type == WATER_WATERLEVEL)
        Water->waterLevel = (self->size.x * RSDK.Sin512(2 * Zone->timer)) + Water->newWaterLevel;

    for (int32 playerID = 0; playerID < Player->playerCount; ++playerID) {
        EntityPlayer *player = RSDK_GET_ENTITY(playerID, Player);

        if (player->state == Player_State_FlyToPlayer && player->abilityPtrs[0]) {
            player->position.x = ((Entity *)player->abilityPtrs[0])->position.x;
            player->position.y = ((Entity *)player->abilityPtrs[0])->position.y;
        }

        Water->wakePosX[playerID] = 0;

        bool32 canEnterWater = true;
        if (!Player_CheckValidState(player) || player->state == Player_State_TransportTube) {
            if (player->state != Player_State_FlyToPlayer)
                canEnterWater = false;
        }

        if (canEnterWater) {
            EntityWater *currentPool = NULL;
            uint16 underwater        = 0;
            foreach_active(Water, pool)
            {
                if (pool->type == WATER_POOL) {
                    if (RSDK.CheckObjectCollisionTouchBox(pool, &pool->hitbox, player, &Water->hitboxPoint)) {
                        currentPool    = pool;
                        pool->childPtr = player;
                        underwater     = RSDK.GetEntitySlot(pool);
                    }
                    else if (pool->childPtr == player) {
                        pool->childPtr = NULL;
                        if (!currentPool)
                            currentPool = pool;
                    }
                }
            }

            if (player->position.y > Water->waterLevel)
                underwater = true;

            if (!Player_CheckValidState(player)) {
                if (player->state != Player_State_FlyToPlayer)
                    underwater = false;
            }

            int32 waterID = 0;
#if MANIA_USE_PLUS
            if (!player->isGhost)
#endif
                waterID = underwater;

            if (!waterID) {
                if (player->underwater) {
                    EntityWater *waterSection = NULL;
                    if (player->underwater > 1)
                        waterSection = RSDK_GET_ENTITY(player->underwater, Water);

                    player->underwater = false;
                    Player_UpdatePhysicsState(player);
                    if (player->velocity.y && (!Current || !((1 << RSDK.GetEntitySlot(player)) & Current->activePlayers))
                        && (Player_CheckValidState(player) || player->state == Player_State_FlyToPlayer)) {
                        if (!Water->disableWaterSplash) {
                            if (waterSection) {
                                EntityWater *splash = CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), player->position.x,
                                                                    waterSection->position.y - (waterSection->size.y >> 1));
                                splash->childPtr    = waterSection;
                                splash->drawGroup   = player->drawGroup;
                            }
                            else {
                                CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), player->position.x, Water->waterLevel);
                            }

                            RSDK.PlaySfx(Water->sfxSplash, false, 0xFF);
                        }

                        if (player->velocity.y >= -TO_FIXED(4)) {
                            player->velocity.y <<= 1;
                            if (player->velocity.y < -TO_FIXED(16))
                                player->velocity.y = -TO_FIXED(16);
                        }
                    }
                }
                else {
                    // if we're not underwater already but we would otherwise be
                    if (abs(player->groundVel) >= 0x78000) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        if (abs(player->position.y + (playerHitbox->bottom << 16) - Water->waterLevel) <= 0x40000 && player->groundedStore) {
                            Water->wakePosX[playerID] = player->position.x;
                            Water->wakeDir[playerID]  = player->groundVel < 0;
                            if (!player->onGround) {
                                player->onGround   = true;
                                player->position.y = Water->waterLevel - (playerHitbox->bottom << 16);
                            }
                        }
                    }
                }

                if (!player->sidekick && player->drownTimer >= 1080) {
                    player->drownTimer = 0;
                    Music_JingleFadeOut(TRACK_DROWNING, false);
                }
            }
            else {
                bool32 notUnderwater = player->underwater == 0;
                player->underwater   = waterID;

                if (notUnderwater) {
                    Player_UpdatePhysicsState(player);
                    if (player->velocity.y && (!Current || !((1 << RSDK.GetEntitySlot(player)) & Current->activePlayers))) {
                        if (!Water->disableWaterSplash) {
                            if (currentPool) {
                                EntityWater *splash = CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), player->position.x,
                                                                    currentPool->position.y - (currentPool->size.x >> 1));
                                splash->drawGroup   = player->drawGroup;
                                splash->childPtr    = currentPool;
                            }
                            else {
                                CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), player->position.x, Water->waterLevel);
                            }

                            RSDK.PlaySfx(Water->sfxSplash, false, 255);
                        }

                        player->velocity.y >>= 2;
                    }

                    player->velocity.x >>= 1;
                    if (player->collisionMode == CMODE_FLOOR)
                        player->groundVel >>= 1;

                    player->drownTimer                = 0;
                    Water->constBubbleTimer[playerID] = 52;
                    Water->unused1[playerID]          = 0;
                }
                else {
                    if (player->invincibleTimer <= 0) {
                        if (player->shield == SHIELD_FIRE) {
                            // No cool smoke fx in mania, just a loss of shield :(
                            player->shield = SHIELD_NONE;
                            destroyEntity(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield));
                        }

                        if (player->shield == SHIELD_LIGHTNING) {
                            // No cool electric flash fx in mania, just a loss of shield :(
                            player->shield = SHIELD_NONE;
                            destroyEntity(RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield));
                        }
                    }

                    if (player->shield != SHIELD_BUBBLE) {
                        Water_SpawnBubble(player, playerID);

                        bool32 playAlertSfx = false;
                        switch (++player->drownTimer) {
                            default: break;

                            case 360:
                            case 660:
                            case 960:
                                if (!player->sidekick)
                                    RSDK.PlaySfx(Water->sfxWarning, false, 0xFF);
                                break;

                            case 1080:
#if MANIA_USE_PLUS
                                if (globals->gameMode != MODE_COMPETITION && globals->gameMode != MODE_ENCORE) {
#else
                                if (globals->gameMode != MODE_COMPETITION) {
#endif
                                    if (!player->sidekick)
                                        Music_PlayJingle(TRACK_DROWNING);
                                }

                                Water_SpawnCountDownBubble(player, playerID, 5);
                                playAlertSfx = true;
                                break;

                            case 1140: playAlertSfx = true; break;

                            case 1200:
                                Water_SpawnCountDownBubble(player, playerID, 4);
                                playAlertSfx = true;
                                break;

                            case 1260: playAlertSfx = true; break;

                            case 1320:
                                Water_SpawnCountDownBubble(player, playerID, 3);
                                playAlertSfx = true;
                                break;

                            case 1380: playAlertSfx = true; break;

                            case 1440:
                                Water_SpawnCountDownBubble(player, playerID, 2);
                                playAlertSfx = true;
                                break;

                            case 1500: playAlertSfx = true; break;

                            case 1560:
                                Water_SpawnCountDownBubble(player, playerID, 1);
                                playAlertSfx = true;
                                break;

                            case 1620: playAlertSfx = true; break;

                            case 1680:
                                Water_SpawnCountDownBubble(player, playerID, 0);
                                playAlertSfx = true;
                                break;

                            case 1740: playAlertSfx = true; break;

                            case 1800:
                                player->deathType = PLAYER_DEATH_DROWN;
                                if (!pool)
                                    player->drawGroup = Zone->playerDrawGroup[1];
                                playAlertSfx = true;
                                break;
                        }

                        if (playAlertSfx) {
                            if (
#if MANIA_USE_PLUS
                                globals->gameMode == MODE_ENCORE ||
#endif
                                globals->gameMode == MODE_COMPETITION)
                                RSDK.PlaySfx(Water->sfxDrownAlert, false, 255);
                        }
                    }
                }
            }
        }

        if (self->state == Player_State_FlyToPlayer && player->abilityPtrs[0]) {
            self->position.x = player->position.x;
            self->position.y = player->position.y;
        }
    }
}

void Water_State_Pool(void)
{
    // nothing, its handled by the water level type
}

void Water_State_Splash(void)
{
    RSDK_THIS(Water);

    EntityWater *water = self->childPtr;
    if (water) {
        if (water != (EntityWater *)1)
            self->position.y = water->position.y - (water->size.y >> 1);
    }
    else {
        self->position.y = Water->waterLevel;
    }

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void Water_HandleBubbleMovement(void)
{
    RSDK_THIS(Water);

    if (self->bubbleVelocity.x || self->bubbleVelocity.y) {
        self->position.x += self->bubbleVelocity.x;
        self->position.y += self->bubbleVelocity.y;
    }
    else {
        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }

    self->position.x += self->bubbleOffset.x;
    self->position.y += self->bubbleOffset.y;

    int32 anim = self->animator.animationID;
    if ((anim == 3 && self->animator.frameID < 12) || anim == 4 || anim == 5 || (anim == 7 && !self->activePlayers)) {
        self->position.x = (RSDK.Sin512(self->angle) << 9) + self->bubbleX;
        self->angle      = (self->angle + 4) & 0x1FF;
    }

    if (self->position.y < Water->waterLevel) {
        bool32 inWater = false;
        foreach_active(Water, pool)
        {
            if (pool->type == WATER_POOL && RSDK.CheckObjectCollisionTouchBox(pool, &pool->hitbox, self, &Water->hitboxPoint))
                inWater = true;
        }

        if (!inWater) {
            if (self->animator.animationID == 3 && self->animator.frameID > 12) {
                RSDK.SetSpriteAnimation(Water->aniFrames, 6, &self->animator, false, 0);
                self->velocity.y = 0;
            }
            else if (self->animator.animationID == 7) {
                Water_PopBigBubble(self, false);
            }
            else {
                destroyEntity(self);
            }
        }
    }
}

void Water_PopBigBubble(EntityWater *self, bool32 jumpedOut)
{
    if (self->timer <= 0) {
        RSDK.SetSpriteAnimation(Water->aniFrames, 6, &self->animator, true, 0);
        self->velocity.x = 0;
        self->velocity.y = 0;

        RSDK.PlaySfx(Water->sfxDNABurst, false, 0xFF);

        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);
            if ((1 << playerID) & self->activePlayers) {
                if (jumpedOut) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                }
                else {
                    if (player->state != Player_State_Static || player->animator.animationID != ANI_BREATHE) {
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + playerID, Shield);
                        if (shield)
                            shield->visible = true;
                        self->activePlayers &= ~(1 << playerID);
                    }
                    else {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_AIR_WALK, &player->animator, true, 0);
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + playerID, Shield);
                        if (shield)
                            shield->visible = true;
                        self->activePlayers &= ~(1 << playerID);
                    }
                }

                player->state = Player_State_Air;
            }
        }
    }
}

void Water_State_Bubble(void)
{
    RSDK_THIS(Water);

    EntityPlayer *playerPtr = (EntityPlayer *)self->childPtr;

    if (self->animator.animationID == 6 && self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);

    if (playerPtr && playerPtr->state == Player_State_Bubble && self->animator.frameID < 3)
        self->bubbleX += 0x40000;

    if (self->speed != -1) {
        if (self->speed) {
            self->bubbleX += self->velocity.x;
            self->velocity.x += self->speed;
        }

        Water_HandleBubbleMovement();

        if (self->tileCollisions != TILECOLLISION_NONE) {
            if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, TO_FIXED(16), false)) {
                while (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, TO_FIXED(16), 0)) self->position.y -= TO_FIXED(1);
            }
            else {
                while (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -TO_FIXED(16), false)
                       && RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -TO_FIXED(16), false)) {
                    self->position.y += TO_FIXED(1);
                }
            }
        }
    }

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID >= 13 || self->animator.animationID == 7) {
        if (self->isBigBubble) {
            if (self->bigBubbleTimer == 16) {
                RSDK.SetSpriteAnimation(Water->bigBubbleFrames, 7, &self->animator, true, 0);
                self->scale.x = 0x180;
                self->scale.y = 0x180;
            }

            if (self->bigBubbleTimer <= 0) {
                self->activePlayers   = 0;
                self->releasedPlayers = 0;
                self->state           = Water_State_BigBubble;
            }
            else {
                self->bigBubbleTimer--;
            }
        }
        else {
            foreach_active(Player, player)
            {
                if (Player_CheckValidState(player) || player->state == Player_State_FlyToPlayer) {
                    if (player->shield != SHIELD_BUBBLE && player->underwater && !Water_GetPlayerBubble(player)
                        && player->position.x >= self->position.x - TO_FIXED(16) && player->position.x <= self->position.x + TO_FIXED(16)) {

                        bool32 inWater = false;
                        if (player->animator.animationID == ANI_FAN) {
                            if (player->position.y >= self->position.y - TO_FIXED(16))
                                inWater = (player->position.y <= self->position.y + TO_FIXED(16));
                        }
                        else {
                            if (player->position.y > self->position.y)
                                inWater = (player->position.y <= self->position.y + TO_FIXED(16));
                        }

                        if (inWater) {
                            bool32 inBubble = false;
                            if (!(self->bubbleFlags & 1) && player->sidekick) {
                                self->bubbleFlags |= 1;
                                inBubble = true;
                            }

                            if (!player->sidekick || inBubble) {
                                if (!inBubble) {
                                    self->state       = Water_State_BubbleBreathed;
                                    self->countdownID = 0;
                                    self->velocity.y  = 0;
                                    self->childPtr    = player;
                                }

                                if (player->state != Current_PlayerState_Right && player->state != Current_PlayerState_Left
                                    && player->state != Current_PlayerState_Up && player->state != Current_PlayerState_Down) {
                                    player->velocity.x = 0;
                                    player->velocity.y = 0;
                                    player->groundVel  = 0;
                                    bool32 canBreathe  = true;

                                    int32 anim = player->animator.animationID;
                                    if (player->characterID == ID_TAILS) {
                                        canBreathe = anim != ANI_FLY && anim != ANI_FLY_TIRED && anim != ANI_FLY_LIFT && anim != ANI_SWIM
                                                     && anim != ANI_SWIM_LIFT;
                                    }
                                    else if (player->characterID == ID_KNUCKLES) {
                                        canBreathe = anim != ANI_LEDGE_PULL_UP && anim != ANI_GLIDE && anim != ANI_GLIDE_SLIDE
                                                     && anim != ANI_CLIMB_IDLE && anim != ANI_CLIMB_UP && anim != ANI_CLIMB_DOWN;
                                    }

                                    if (canBreathe && (anim != ANI_FAN && anim != ANI_CLING)) {
                                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_BUBBLE, &player->animator, false, 0);

                                        if (!player->sidekick)
                                            self->playerInBubble = true;
                                    }

                                    if (player->state == Player_State_FlyCarried) {
                                        player->state                                        = Player_State_Air;
                                        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
                                    }
#if MANIA_USE_PLUS
                                    else if (player->state == Player_State_MightyHammerDrop) {
                                        player->state = Player_State_Air;
                                    }
#endif
                                }

                                player->drownTimer = 0;
                                if (!player->sidekick)
                                    Music_JingleFadeOut(TRACK_DROWNING, false);

                                RSDK.PlaySfx(Water->sfxBreathe, false, 255);
                            }
                        }
                    }
                }
            }
        }
    }

    if (self->speed != -1) {
        if (!RSDK.CheckOnScreen(self, &self->updateRange))
            destroyEntity(self);
    }
}

void Water_State_BubbleBreathed(void)
{
    RSDK_THIS(Water);

    EntityPlayer *player = (EntityPlayer *)self->childPtr;
    if (player->state == Player_State_Hurt || !Player_CheckValidState(player))
        self->playerInBubble = false;

    if (self->speed) {
        self->position.x += self->velocity.x;
        self->velocity.x += self->speed;
    }

    RSDK.ProcessAnimation(&self->animator);

    self->scale.x -= 0x18;
    self->scale.y -= 0x18;
    if (self->scale.x > 0) {
        if (self->playerInBubble)
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_BUBBLE, &player->animator, false, 0);
    }
    else {
        self->scale.x = 0;
        self->scale.y = 0;

        if (self->playerInBubble)
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_WALK, &player->animator, false, 0);

        destroyEntity(self);
    }
}

EntityWater *Water_GetPlayerBubble(EntityPlayer *player)
{
    int32 playerID = RSDK.GetEntitySlot(player);

    foreach_active(Water, water)
    {
        if (water->state == Water_State_BigBubble && ((1 << playerID) & water->activePlayers)) {
            foreach_return water;
        }
    }
    return NULL;
}

void Water_State_BigBubble(void)
{
    RSDK_THIS(Water);

    if (self->animator.animationID == 6 && self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);

    Water_HandleBubbleMovement();
    self->countdownID = self->animator.frameID;

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->countdownID || self->animator.animationID == 7) {
        if (self->scale.x < 0x200) {
            self->scale.y += 8;
            self->scale.x += 8;
        }
    }
    else {
        self->scale.x = 0x180;
        self->scale.y = 0x180;
    }

    if (self->animator.animationID != 6) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);
            if (!Player_CheckValidState(player) || !player->underwater) {
                continue;
            }

            if (!(self->activePlayers & (1 << playerID)) && !((1 << playerID) & self->releasedPlayers)) {
                if (globals->gameMode == MODE_COMPETITION && self->activePlayers)
                    continue;

                if (Player_CheckCollisionTouch(player, self, &Water->hitboxPlayerBubble) && !Water_GetPlayerBubble(player)) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_BREATHE, &player->animator, true, 0);
                    player->state        = Player_State_Static;
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                    if (shield)
                        shield->visible = false;

                    player->drownTimer = 0;
                    if (!player->sidekick)
                        Music_JingleFadeOut(TRACK_DROWNING, false);

                    RSDK.PlaySfx(Water->sfxDNAGrab, false, 255);
                    self->activePlayers |= 1 << playerID;
                    self->releasedPlayers |= 1 << playerID;
                    if (RSDK.GetEntitySlot(self) >= RESERVE_ENTITY_COUNT) {
                        int32 id = SLOT_BIGBUBBLE_P1;
                        for (; id < SLOT_BIGBUBBLE_P1 + PLAYER_COUNT; ++id) {
                            if (RSDK_GET_ENTITY_GEN(id)->classID == TYPE_BLANK)
                                break;
                        }

                        if (id >= 0 && id < SLOT_BIGBUBBLE_P1 + PLAYER_COUNT) {
                            RSDK.AddDrawListRef(self->drawGroup, id);
                            RSDK.CopyEntity(RSDK_GET_ENTITY(id, Water), self, true);
                            foreach_return;
                        }
                    }
                }
            }

            if ((1 << playerID) & self->activePlayers) {
                if (self->timer > 0) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_BREATHE, &player->animator, false, 0);
                    player->state = Player_State_Static;
                }

                player->drownTimer = 0;
                player->position.x = self->position.x;
                player->position.y = self->position.y;
                player->velocity.x = self->velocity.x;
                player->velocity.y = self->velocity.y;
                if (player->sidekick) {
                    if (player->left)
                        player->direction = FLIP_X;
                    else if (player->right)
                        player->direction = FLIP_NONE;

                    if (player->jumpPress) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                        player->state = Player_State_Air;
                        self->activePlayers &= ~(1 << playerID);
                        self->releasedPlayers |= 1 << playerID;
                    }
                }
                else {
                    self->drawGroup      = player->drawGroup;
                    self->collisionPlane = player->collisionPlane;
                    if (player->state != Player_State_Static && player->animator.animationID != ANI_BREATHE) {
                        Water_PopBigBubble(self, false);
                        foreach_return;
                    }

                    int32 xVel = 0;
                    if (player->left) {
                        player->direction = FLIP_X;
                        xVel              = -0x11000;
                    }
                    else if (player->right) {
                        player->direction = FLIP_NONE;
                        xVel              = 0x11000;
                    }

                    int32 yVel = -0x8800;
                    if (player->down)
                        yVel = -0x4400;
                    else if (player->up)
                        yVel = -0x11000;

                    if (xVel - self->velocity.x)
                        self->velocity.x += (2 * ((xVel - self->velocity.x) > 0) - 1) << 11;

                    if (yVel - self->velocity.y)
                        self->velocity.y += (2 * ((yVel - self->velocity.y) > 0) - 1) << 11;

                    if (player->jumpPress) {
                        Water_PopBigBubble(self, true);
                        foreach_return;
                    }
                }
            }

            if ((1 << playerID) & self->releasedPlayers) {
                if (!Player_CheckCollisionTouch(player, self, &Water->hitboxPlayerBubble))
                    self->releasedPlayers &= ~(1 << playerID);
            }
        }

        if (self->timer > 0)
            self->timer--;

        Hitbox hitboxSpike;
        hitboxSpike.left   = -20;
        hitboxSpike.top    = -20;
        hitboxSpike.right  = 20;
        hitboxSpike.bottom = 20;

        foreach_active(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &hitboxSpike, spikes, &spikes->hitbox)) {
                Water_PopBigBubble(self, false);
                foreach_return;
            }
        }

        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, -TO_FIXED(24), true)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, -TO_FIXED(24), 0, true)
            || RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, TO_FIXED(24), 0, true)) {
            Water_PopBigBubble(self, false);
        }
    }
}

void Water_State_BtnBigBubble(void)
{
    RSDK_THIS(Water);

    if (self->taggedButton) {
        if (self->timer <= 0) {
            Hitbox hitbox;
            hitbox.left   = -32;
            hitbox.top    = -32;
            hitbox.right  = 32;
            hitbox.bottom = 32;

            if (self->taggedButton->currentlyActive) {
                foreach_active(Player, player)
                {
                    if (!player->sidekick && Player_CheckCollisionTouch(player, self, &hitbox)) {
                        if (player->animator.animationID != ANI_BREATHE) {
                            EntityWater *bigBubble = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), self->position.x, self->position.y);
                            RSDK.SetSpriteAnimation(Water->bigBubbleFrames, 7, &bigBubble->animator, true, 0);

                            bigBubble->state          = Water_State_BigBubble;
                            bigBubble->isBigBubble    = true;
                            bigBubble->bigBubbleTimer = 0;
                            bigBubble->velocity.y     = -0x8800;
                            bigBubble->childPtr       = 0;
                            bigBubble->bubbleX        = bigBubble->position.x;
                            bigBubble->timer          = 0x1E;
                            bigBubble->scale.x        = 0x180;
                            bigBubble->scale.y        = 0x180;
                            self->timer               = 60;
                        }
                    }
                }
            }
        }
        else {
            self->timer--;
        }
    }
}

void Water_State_Bubbler(void)
{
    RSDK_THIS(Water);

    self->visible = false;
    if (self->position.y > Water->waterLevel)
        self->visible = true;

    foreach_active(Water, pool)
    {
        if (pool->type == WATER_POOL && RSDK.CheckObjectCollisionTouchBox(pool, &pool->hitbox, self, &Water->hitboxPoint))
            self->visible = true;
    }

    if (self->visible && RSDK.CheckOnScreen(self, &self->updateRange)) {
        if (!self->countdownID) {
            if (!self->bubbleFlags) {
                self->bubbleFlags = 1;
                int32 rand        = RSDK.Rand(0, 0x10000);
                self->bubbleType1 = rand % 6;
                self->bubbleType2 = rand & 12;

                if (!self->dudsRemaining--) {
                    self->bubbleFlags |= 2;
                    self->dudsRemaining = self->numDuds;
                }
            }

            self->countdownID = RSDK.Rand(0, 32);

            EntityWater *bubble         = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), self->position.x, self->position.y - 0x20000);
            int32 bubbleSize            = Water->bubbleSizes[self->bubbleType1 + self->bubbleType2];
            bubble->animator.loopIndex  = bubbleSize;
            bubble->animator.frameCount = bubbleSize + 1;
            if (self->type == WATER_BIG_BUBBLER) {
                bubble->isBigBubble    = true;
                bubble->bigBubbleTimer = 16;
            }
            bubble->position.x += RSDK.Rand(-8, 9) << 16;
            bubble->velocity.y = -0x8800;
            bubble->angle      = 2 * RSDK.Rand(0, 256);

            bubble->childPtr = NULL;
            bubble->bubbleX  = bubble->position.x;
            if (self->bubbleFlags & 2 && (!RSDK.Rand(0, 4) || !self->bubbleType1) && !(self->bubbleFlags & 4)) {
                RSDK.SetSpriteAnimation(Water->aniFrames, 3, &bubble->animator, false, 0);
                bubble->isPermanent = true;
                self->bubbleFlags |= 4;
            }

            if (--self->bubbleType1 < 0) {
                self->bubbleFlags = 0;
                self->countdownID += RSDK.Rand(0, 128) + 128;
            }
        }
        else {
            --self->countdownID;
        }
    }

    RSDK.ProcessAnimation(&self->animator);
}

void Water_State_Countdown(void)
{
    RSDK_THIS(Water);

    EntityPlayer *player = (EntityPlayer *)self->childPtr;

    if (player->animator.animationID == ANI_FAN) {
        self->bubbleX += player->velocity.x;
        self->position.y += player->velocity.y;
    }

    RSDK.ProcessAnimation(&self->animator);

    bool32 isActive = false;
    if (self->position.y >= Water->waterLevel) {
        isActive = true;
    }
    else {
        foreach_active(Water, pool)
        {
            if (pool->type == WATER_POOL && RSDK.CheckObjectCollisionTouchBox(pool, &pool->hitbox, self, &Water->hitboxPoint)) {
                isActive = true;
            }
        }
    }

    if ((self->animator.animationID != 7 || self->animator.frameID != self->animator.frameCount - 1) && isActive) {
        self->position.y += self->velocity.y;
        self->position.x = (RSDK.Sin512(self->angle) << 9) + self->bubbleX;
        self->angle      = (self->angle + 4) & 0x1FF;
    }
    else {
        RSDK.SetSpriteAnimation(Water->aniFrames, self->countdownID + 8, &self->animator, true, 0);

        if (player->camera) {
            self->size.x = (self->position.x & 0xFFFF0000) - (player->camera->position.x & 0xFFFF0000);
            self->size.y = (self->position.y & 0xFFFF0000) - (player->camera->position.y & 0xFFFF0000);
        }
        else {
            self->size.x = (self->position.x & 0xFFFF0000) - (player->position.x & 0xFFFF0000);
            self->size.y = (self->position.y & 0xFFFF0000) - (player->position.y & 0xFFFF0000);
        }

        self->state = Water_State_CountdownFollow;
    }
}

void Water_State_CountdownFollow(void)
{
    RSDK_THIS(Water);

    RSDK.ProcessAnimation(&self->animator);

    if (self->angle >= 0x280) {
        self->scale.x -= 8;
        self->scale.y -= 8;
        if (self->scale.x <= 0)
            destroyEntity(self);
    }
    else {
        self->scale.x = (RSDK.Sin256(self->angle) >> 1) + 0x200;
        self->scale.y = (RSDK.Sin256(self->angle + 0x80) >> 1) + 0x200;

        self->angle += 6;
    }
}

void Water_State_HeightTrigger(void)
{
    RSDK_THIS(Water);

    bool32 activated = false;
    if (self->taggedButton) {
        EntityButton *button = self->taggedButton;
        if (button->currentlyActive) {
            Water->moveWaterLevel = true;
            activated             = true;
        }
    }

    int32 px = self->position.x + 1;
    if (!activated) {
        foreach_active(Player, player)
        {
            if (!RSDK.GetEntitySlot(player)) {
                if (abs(self->position.x - player->position.x) < self->updateRange.x) {
                    if (abs(self->position.y - player->position.y) < self->updateRange.y) {
                        activated = true;
                        px        = player->position.x;
                    }
                }
            }
        }
    }

    if (activated) {
        if (self->taggedButton)
            Water->targetWaterLevel = self->position.y;
        else if (px <= self->position.x)
            Water->targetWaterLevel = self->height.x;
        else
            Water->targetWaterLevel = self->height.y;

        Water->waterMoveSpeed = self->speed << 15;
        if (self->destroyOnTrigger)
            destroyEntity(self);
    }
}

void Water_Draw_Water(void)
{
    RSDK_THIS(Water);
    RSDKScreenInfo *screen = &ScreenInfo[SceneInfo->currentScreenID];

    Vector2 drawPos;
    drawPos.x = ((screen->position.x & 0xFFFFFFC0) + 32) << 16;
    drawPos.y = Water->waterLevel;
    for (int32 i = (screen->size.x >> 6) + 2; i > 0; --i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += TO_FIXED(64);
    }

    self->drawFX |= FX_FLIP;
    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (Water->wakePosX[i] > 0) {
            self->direction = Water->wakeDir[i];
            drawPos.x       = Water->wakePosX[i];
            RSDK.DrawSprite(&Water->wakeAnimator, &drawPos, false);
        }
    }

    self->drawFX &= ~FX_FLIP;
}

void Water_Draw_Pool(void)
{
    RSDK_THIS(Water);

    RSDK.DrawRect(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1), self->size.x, self->size.y,
                  (self->r << 16) | (self->g << 8) | self->b, 0x100, INK_SUB, false);
}

void Water_Draw_Splash(void)
{
    RSDK_THIS(Water);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Water_Draw_Countdown(void)
{
    Vector2 drawPos;

    RSDK_THIS(Water);
    if (self->state == Water_State_Countdown) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y;
    }
    else {
        EntityPlayer *player = (EntityPlayer *)self->childPtr;
        if (player->camera) {
            drawPos.x = player->camera->position.x + self->size.x;
            drawPos.y = player->camera->position.y + self->size.y;
        }
        else {
            drawPos.x = player->position.x + self->size.x;
            drawPos.y = player->position.y + self->size.y;
        }
    }

    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void Water_Draw_Bubbler(void)
{
    RSDK_THIS(Water);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Water_Draw_Bubble(void)
{
    RSDK_THIS(Water);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void Water_EditorDraw(void)
{
    RSDK_THIS(Water);

    self->updateRange.x = TO_FIXED(128);
    self->updateRange.y = TO_FIXED(128);

    self->inkEffect = INK_NONE;
    switch (self->type) {
        case WATER_WATERLEVEL:
            self->inkEffect = INK_ADD;
            self->alpha     = RSDK.CheckSceneFolder("AIZ") ? 0x60 : 0xE0;
            RSDK.SetSpriteAnimation(Water->aniFrames, 0, &self->animator, true, 0);

            Water->waterLevel = self->position.y;

            Vector2 drawPos;
            drawPos.x = (((self->position.x >> 16) & 0xFFFFFFC0) + 32) << 16;
            drawPos.y = self->position.y;
            for (int32 i = (WIDE_SCR_XSIZE >> 6) + 2; i > 0; --i) {
                RSDK.DrawSprite(&self->animator, &drawPos, false);
                drawPos.x += TO_FIXED(64);
            }
            break;

        case WATER_POOL:
            self->drawFX        = FX_FLIP;
            self->updateRange.x = self->size.x >> 1;
            self->updateRange.y = self->size.y >> 1;

            self->inkEffect = INK_BLEND;
            RSDK.DrawRect(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1), self->size.x, self->size.y,
                          (self->r << 16) | (self->g << 8) | self->b, 0x20, INK_ALPHA, false);
            if (showGizmos()) {
                self->inkEffect = INK_NONE;
                DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);
            }
            break;

        case WATER_BUBBLER:
        case WATER_BIG_BUBBLER:
            self->inkEffect = INK_ADD;
            self->alpha     = 0x100;
            RSDK.SetSpriteAnimation(Water->aniFrames, 2, &self->animator, true, 0);

            Water_Draw_Bubbler();
            break;

        case WATER_HEIGHT_TRIGGER:
            self->active  = ACTIVE_BOUNDS;
            self->visible = false;

            RSDK.SetSpriteAnimation(Water->aniFrames, 0, &self->animator, true, 0);
            RSDK.DrawSprite(&self->animator, NULL, false);

            if (showGizmos()) {
                Water_SetupTagLink();

                RSDK_DRAWING_OVERLAY(true);
                if (self->taggedButton) {
                    DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y,
                                          0xFFFF00, INK_NONE, 0xFF);
                }
                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        case WATER_BTN_BIG_BUBBLE:
            self->drawFX    = FX_SCALE;
            self->inkEffect = INK_ADD;
            self->alpha     = 0x100;
            self->scale.x   = 0x200;
            self->scale.y   = 0x200;
            RSDK.SetSpriteAnimation(Water->bigBubbleFrames, 7, &self->animator, true, 0);

            Water_Draw_Bubble();

            if (showGizmos()) {
                Water_SetupTagLink();

                RSDK_DRAWING_OVERLAY(true);
                if (self->taggedButton) {
                    DrawHelpers_DrawArrow(self->taggedButton->position.x, self->taggedButton->position.y, self->position.x, self->position.y,
                                          0xFFFF00, INK_NONE, 0xFF);
                }
                RSDK_DRAWING_OVERLAY(false);
            }
            break;

        default: break;
    }
}

void Water_EditorLoad(void)
{
    Water->aniFrames = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("HCZ")) {
        Water->wakeFrames      = RSDK.LoadSpriteAnimation("HCZ/Wake.bin", SCOPE_STAGE);
        Water->bigBubbleFrames = RSDK.LoadSpriteAnimation("HCZ/BigBubble.bin", SCOPE_STAGE);
        RSDK.SetSpriteAnimation(Water->wakeFrames, 0, &Water->wakeAnimator, true, 0);
    }

    RSDK_ACTIVE_VAR(Water, type);
    RSDK_ENUM_VAR("Water Level", WATER_WATERLEVEL);
    RSDK_ENUM_VAR("Pool", WATER_POOL);
    RSDK_ENUM_VAR("Bubbler", WATER_BUBBLER);
    RSDK_ENUM_VAR("Height Trigger", WATER_HEIGHT_TRIGGER);
    RSDK_ENUM_VAR("Big Bubbler", WATER_BIG_BUBBLER);
    RSDK_ENUM_VAR("Btn Big Bubble", WATER_BTN_BIG_BUBBLE);

    RSDK_ACTIVE_VAR(Water, priority);
    RSDK_ENUM_VAR("Lowest", WATER_PRIORITY_LOWEST);
    RSDK_ENUM_VAR("Low", WATER_PRIORITY_LOW);
    RSDK_ENUM_VAR("High", WATER_PRIORITY_HIGH);
    RSDK_ENUM_VAR("Highest", WATER_PRIORITY_HIGHEST);
}
#endif

void Water_Serialize(void)
{
    RSDK_EDITABLE_VAR(Water, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, numDuds);
    RSDK_EDITABLE_VAR(Water, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(Water, VAR_VECTOR2, height);
    RSDK_EDITABLE_VAR(Water, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(Water, VAR_ENUM, buttonTag);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, r);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, g);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, b);
    RSDK_EDITABLE_VAR(Water, VAR_UINT8, priority);
    RSDK_EDITABLE_VAR(Water, VAR_BOOL, destroyOnTrigger);
}
