#include "../SonicMania.h"

ObjectWater *Water;

void Water_Update(void)
{
    RSDK_THIS(Water);
    StateMachine_Run(entity->state);
}

void Water_LateUpdate(void) {}

void Water_StaticUpdate(void)
{
    EntityPauseMenu *pauseMenu = RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu);

    if (pauseMenu->objectID != PauseMenu->objectID) {
        if (Water->newWaterLevel == Water->targetWaterLevel) {
            Water->field_C8 = 0;
            if (Water->waterLevelVolume <= 30) {
                if (Water->waterLevelVolume > 0)
                    Water->waterLevelVolume--;
            }
            else {
                Water->waterLevelVolume = 30;
            }
        }
        else {
            if (RSDK.CheckStageFolder("HCZ") && Water->field_C8) {
                if (!(Water->waterLevelVolume % 5)) {
                    Camera_ShakeScreen(0, 0, 1);
                }
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

    if (Water->playingWaterLevelSFX) {
        if (Water->waterLevelVolume > 30)
            Water->waterLevelVolume = 30;
        float volume = (float)Water->waterLevelVolume / 30.0;
        RSDK.SetChannelAttributes(Water->waterLevelChannel_L, volume, -1.0, 1.0);
        RSDK.SetChannelAttributes(Water->waterLevelChannel_R, volume, 1.0, 1.0);
    }

    if (Water->waterLevelVolume > 0) {
        if (!Water->playingWaterLevelSFX) {
            Water->waterLevelChannel_L = RSDK.PlaySFX(Water->sfx_WaterLevelL, 1, 255);
            RSDK.SetChannelAttributes(Water->waterLevelChannel_L, 0.0, -1.0, 1.0);

            Water->waterLevelChannel_R = RSDK.PlaySFX(Water->sfx_WaterLevelR, 1, 255);
            RSDK.SetChannelAttributes(Water->waterLevelChannel_R, 0.0, 1.0, 1.0);

            Water->playingWaterLevelSFX = true;
        }
    }
    else if (!Water->waterLevelVolume && Water->playingWaterLevelSFX) {
        RSDK.StopSFX(Water->sfx_WaterLevelL);
        RSDK.StopSFX(Water->sfx_WaterLevelR);
        Water->playingWaterLevelSFX = false;
    }

    bool32 flag = false;
    for (int i = 0; i < 4; ++i) {
        flag |= Water->wakePosX[i] > 0;
    }

    if (flag) {
        if (pauseMenu->objectID != PauseMenu->objectID)
            RSDK.ProcessAnimation(&Water->wakeData);

        if (!Water->playingSkimSFX) {
            RSDK.PlaySFX(Water->sfx_Skim, 3570, 255);
            Water->playingSkimSFX = true;
        }
    }
    else if (Water->playingSkimSFX) {
        RSDK.StopSFX(Water->sfx_Skim);
        Water->playingSkimSFX = 0;
    }
}

void Water_Draw(void)
{
    RSDK_THIS(Water);
    StateMachine_Run(entity->stateDraw);
}

void Water_Create(void *data)
{
    RSDK_THIS(Water);

    entity->visible = true;
    if (!RSDK_sceneInfo->inEditor) {
        if (data)
            entity->type = voidToInt(data);
        switch (entity->type) {
            case 0:
                if (globals->gameMode == MODE_COMPETITION && RSDK.CheckStageFolder("CPZ")) {
                    RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
                }
                else {
                    entity->active    = ACTIVE_NORMAL;
                    entity->inkEffect = INK_ADD;
                    entity->drawOrder = Zone->hudDrawOrder - 1;
                    entity->alpha     = RSDK.CheckStageFolder("AIZ") ? 0x60 : 0xE0;
                    RSDK.SetSpriteAnimation(Water->spriteIndex, 0, &entity->waterData, true, 0);
                    entity->state     = Water_State_Palette;
                    entity->stateDraw = Water_State_Draw_Palette;
                    entity->size.x >>= 16;
                    Water->newWaterLevel    = entity->position.y;
                    Water->targetWaterLevel = entity->position.y;
                }
                break;
            case 1:
                entity->active = ACTIVE_BOUNDS;
                entity->drawFX = FX_FLIP;
                switch (entity->priority) {
                    case 0: entity->drawOrder = Zone->drawOrderLow - 1; break;
                    case 1: entity->drawOrder = Zone->playerDrawLow; break;
                    case 2: entity->drawOrder = Zone->playerDrawHigh; break;
                    case 3: entity->drawOrder = Zone->hudDrawOrder - 1; break;
                    default: break;
                }

                entity->updateRange.x = entity->size.x >> 1;
                entity->updateRange.y = entity->size.y >> 1;
                entity->hitbox.right  = (entity->size.x >> 17);
                entity->hitbox.left   = -(entity->size.x >> 17);
                entity->hitbox.bottom = (entity->size.y >> 17);
                entity->hitbox.top    = -(entity->size.y >> 17);
                entity->state         = Water_State_Tint;
                entity->stateDraw     = Water_State_Draw_Tint;
                break;
            case 2:
            case 4:
                entity->drawOrder     = Zone->drawOrderLow;
                entity->inkEffect     = INK_ADD;
                entity->alpha         = 0x100;
                entity->active        = ACTIVE_BOUNDS;
                entity->updateRange.x = 0x100000;
                entity->updateRange.y = 0x100000;
                RSDK.SetSpriteAnimation(Water->spriteIndex, 2, &entity->waterData, true, 0);
                entity->state     = Water_State_Bubbler;
                entity->stateDraw = Water_State_Draw_Bubbler;
                break;
            case 3:
                entity->active  = ACTIVE_BOUNDS;
                entity->visible = false;
                if (entity->buttonTag <= 0) {
                    entity->active = ACTIVE_NORMAL;
                    Water_CheckButtonTag();
                }
                entity->updateRange.x = entity->size.x >> 1;
                entity->updateRange.y = entity->size.y >> 1;
                entity->state         = Water_State_Adjustable;
                break;
            case 5:
                entity->drawOrder     = Zone->drawOrderLow;
                entity->active        = ACTIVE_BOUNDS;
                entity->updateRange.x = 0x100000;
                entity->updateRange.y = 0x100000;
                entity->state         = Water_Unknown9;
                entity->stateDraw     = 0;
                Water_CheckButtonTag();
                break;
            case 6:
                entity->active    = ACTIVE_NORMAL;
                entity->drawOrder = Zone->hudDrawOrder - 2;
                RSDK.SetSpriteAnimation(Water->spriteIndex, 1, &entity->waterData, true, 0);
                entity->state     = Water_State_Splash;
                entity->stateDraw = Water_State_Draw_Splash;
                break;
            case 7:
                entity->active        = ACTIVE_NORMAL;
                entity->drawOrder     = Zone->playerDrawHigh;
                entity->drawFX        = FX_SCALE;
                entity->inkEffect     = INK_ADD;
                entity->alpha         = 0x100;
                entity->updateRange.x = 0x800000;
                entity->updateRange.y = 0x800000;
                entity->scale.x       = 0x200;
                entity->scale.y       = 0x200;
                RSDK.SetSpriteAnimation(Water->spriteIndex, 5, &entity->waterData, true, 0);
                entity->state     = Water_State_Bubble;
                entity->isPermanent = true;
                entity->stateDraw = Water_State_Draw_Bubble;
                break;
            case 8:
                entity->active        = ACTIVE_NORMAL;
                entity->drawOrder     = Zone->playerDrawHigh;
                entity->drawFX        = FX_SCALE;
                entity->inkEffect     = INK_ADD;
                entity->alpha         = 0x100;
                entity->updateRange.x = 0x800000;
                entity->updateRange.y = 0x800000;
                entity->scale.x       = 0x200;
                entity->scale.y       = 0x200;
                RSDK.SetSpriteAnimation(Water->spriteIndex, 7, &entity->waterData, true, 0);
                entity->state     = Water_State_CountDownBubble;
                entity->stateDraw = Water_State_Draw_CountDownBubble;
                break;
            default: return;
        }
    }
}

void Water_StageLoad(void)
{
    Water->active           = ACTIVE_ALWAYS;
    Water->spriteIndex      = RSDK.LoadSpriteAnimation("Global/Water.bin", SCOPE_STAGE);
    Water->waterLevel       = 0x7FFFFFFF;
    Water->newWaterLevel    = Water->waterLevel;
    Water->targetWaterLevel = Water->waterLevel;
    Water->field_D4         = 0;
    Water->hitbox2.left     = -2;
    Water->hitbox2.top      = -2;
    Water->hitbox2.right    = 2;
    Water->hitbox2.bottom   = 2;
    Water->hitbox.left      = -1;
    Water->hitbox.top       = -1;
    Water->hitbox.right     = 1;
    Water->hitbox.bottom    = 1;
    Water->sfx_Splash       = RSDK.GetSFX("Stage/Splash.wav");
    Water->sfx_Breathe      = RSDK.GetSFX("Stage/Breathe.wav");
    Water->sfx_Warning      = RSDK.GetSFX("Stage/Warning.wav");
    Water->sfx_Drown        = RSDK.GetSFX("Stage/Drown.wav");
    Water->sfx_DrownAlert   = RSDK.GetSFX("Stage/DrownAlert.wav");
    Water->sfx_Skim         = RSDK.GetSFX("HCZ/Skim.wav");
    if (RSDK.CheckStageFolder("HCZ")) {
        Water->wakeSprite      = RSDK.LoadSpriteAnimation("HCZ/Wake.bin", SCOPE_STAGE);
        Water->bigBubbleSprite = RSDK.LoadSpriteAnimation("HCZ/BigBubble.bin", SCOPE_STAGE);
        Water->sfx_WaterLevelL = RSDK.GetSFX("HCZ/WaterLevel_L.wav");
        Water->sfx_WaterLevelR = RSDK.GetSFX("HCZ/WaterLevel_R.wav");
        Water->sfx_DNAGrab     = RSDK.GetSFX("CPZ/DNAGrab.wav");
        Water->sfx_DNABurst    = RSDK.GetSFX("CPZ/DNABurst.wav");
        RSDK.SetSpriteAnimation(Water->wakeSprite, 0, &Water->wakeData, true, 0);
    }
}

void Water_EditorDraw(void) {}

void Water_EditorLoad(void) {}

void Water_SetWaterLevel(void)
{
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    int waterDrawPos   = (Water->waterLevel >> 0x10) - screen->position.y;
    if (waterDrawPos >= 0) {
        if (waterDrawPos > screen->height)
            waterDrawPos = screen->height;
    }
    else {
        waterDrawPos = 0;
    }
    RSDK.SetActivePalette(Water->waterPalette, waterDrawPos, screen->height);
    RSDK_screens[RSDK_sceneInfo->currentScreenID].waterDrawPos = waterDrawPos;
}

void Water_RemoveWaterEffect(void) { RSDK.SetActivePalette(0, 0, RSDK_screens[RSDK_sceneInfo->currentScreenID].height); }

void Water_CheckButtonTag(void)
{
    RSDK_THIS(Water);

    if (entity->buttonTag <= 0) {
        entity->taggedObject = TYPE_BLANK;
    }
    else {
        bool32 flag = false;
        if (Button) {
            foreach_all(Button, button)
            {
                if (button->tag == entity->buttonTag) {
                    entity->taggedObject = button->objectID;
                    if (entity->updateRange.x < 0x800000 + abs(entity->position.x - button->position.x)) {
                        entity->updateRange.x = 0x800000 + abs(entity->position.x - button->position.x);
                    }

                    if (entity->updateRange.y < 0x800000 + abs(entity->position.y - button->position.y)) {
                        entity->updateRange.y = 0x800000 + abs(entity->position.y - button->position.y);
                    }
                    flag = true;
                    foreach_break;
                }
            }
        }

        if (PullChain && !flag) {
            foreach_all(PullChain, chain)
            {
                if (chain->tag == entity->buttonTag) {
                    entity->taggedObject = chain->objectID;
                    if (entity->updateRange.x < 0x800000 + abs(entity->position.x - chain->position.x)) {
                        entity->updateRange.x = 0x800000 + abs(entity->position.x - chain->position.x);
                    }

                    if (entity->updateRange.y < 0x800000 + abs(entity->position.y - chain->position.y)) {
                        entity->updateRange.y = 0x800000 + abs(entity->position.y - chain->position.y);
                    }
                    flag = true;
                    foreach_break;
                }
            }
        }

        if (!flag)
            entity->taggedObject = TYPE_BLANK;
    }
}

void Water_SpawnBubble(EntityPlayer *player, int id)
{
    if (Water->array3[id] <= 0) {
        if (RSDK.Rand(0, 3) == 1)
            Water->array4[id] = RSDK.Rand(0, 16) + 8;
        else
            Water->array4[id] = 0x200;
        Water->array3[id]   = 60;
        EntityWater *bubble = (EntityWater *)RSDK.CreateEntity(Water->objectID, (void *)7, player->position.x, player->position.y);
        if (player->direction) {
            bubble->position.x -= 0x60000;
            bubble->angle = 0x100;
        }
        else {
            bubble->position.x += 0x60000;
        }

        bubble->childPtr   = player;
        bubble->field_68   = bubble->position.x;
        bubble->velocity.y = -0x8800;
        bubble->drawOrder  = player->drawOrder + 1;
    }
    else {
        Water->array3[id]--;
        if (Water->array4[id] <= 0) {
            EntityWater *bubble = (EntityWater *)RSDK.CreateEntity(Water->objectID, (void *)7, player->position.x, player->position.y);
            if (player->direction) {
                bubble->position.x -= 0x60000;
                bubble->angle = 0x100;
            }
            else {
                bubble->position.x += 0x60000;
            }

            bubble->childPtr   = player;
            bubble->field_68   = bubble->position.x;
            bubble->velocity.y = -0x8800;
            bubble->drawOrder  = player->drawOrder + 1;
            Water->array4[id]  = 0x200;
        }
        else {
            Water->array4[id]--;
        }
    }
}

void Water_SpawnCountDownBubble(EntityPlayer *player, int id, byte bubbleID)
{
    EntityWater *bubble = (EntityWater *)RSDK.CreateEntity(Water->objectID, (void *)8, player->position.x, player->position.y);
    if (player->direction) {
        bubble->position.x -= 0x60000;
        bubble->angle = 256;
    }
    else {
        bubble->position.x += 0x60000;
    }
    bubble->field_68    = bubble->position.x;
    bubble->velocity.y  = -0x8800;
    bubble->childPtr    = player;
    bubble->countdownID = bubbleID;
    bubble->drawOrder   = Zone->playerDrawHigh + 1;
}

void Water_State_Palette(void)
{
    RSDK_THIS(Water);
    RSDK.ProcessAnimation(&entity->waterData);
    if (entity->type == 0)
        Water->waterLevel = (entity->size.x * RSDK.Sin512(2 * Zone->timer)) + Water->newWaterLevel;

    for (int p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

        if (player->state == Player_State_FlyIn && player->entPtr) {
            player->position.x = ((Entity *)player->entPtr)->position.x;
            player->position.y = ((Entity *)player->entPtr)->position.y;
        }

        Water->wakePosX[p] = 0;

        bool32 loopFlag = true;
        if (!Player_CheckValidState(player) || player->state == Player_Unknown7) {
            if (player->state != Player_State_FlyIn)
                loopFlag = false;
        }

        if (loopFlag) {
            EntityWater *childPtr = NULL;
            ushort underwater        = 0;
            foreach_active(Water, water)
            {
                if (water->type == 1) {
                    if (RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, player, &Water->hitbox)) {
                        childPtr        = water;
                        water->childPtr = player;
                        underwater      = RSDK.GetEntityID(water);
                    }
                    else if (water->childPtr == player) {
                        water->childPtr = NULL;
                        if (!childPtr)
                            childPtr = water;
                    }
                }
            }

            if (player->position.y > Water->waterLevel)
                underwater = true;
            if (!Player_CheckValidState(player)) {
                if (player->state != Player_State_FlyIn)
                    underwater = false;
            }
            int waterID = 0;
#if RETRO_USE_PLUS
            if (!player->isGhost)
                waterID = underwater;
#endif
            if (!waterID) {
                if (player->underwater) {
                    EntityWater *childPtr = NULL;
                    if (player->underwater > 1)
                        childPtr = RSDK_GET_ENTITY(player->underwater, Water);
                    player->underwater = false;
                    Player_ChangePhysicsState(player);
                    if (player->velocity.y /*&& (!Current || !((1 << RSDK_GetEntityID(player)) & Current->activePlayers))*/
                        && (Player_CheckValidState(player) || player->state == Player_State_FlyIn)) {
                        if (!Water->field_D4) {
                            if (childPtr) {
                                EntityWater *splash =
                                    (EntityWater*)RSDK.CreateEntity(Water->objectID, (void *)6, player->position.x, childPtr->position.y - (childPtr->size.y >> 1));
                                splash->childPtr  = childPtr;
                                splash->drawOrder = player->drawOrder;
                            }
                            else {
                                RSDK.CreateEntity(Water->objectID, (void *)6, player->position.x, Water->waterLevel);
                            }
                            RSDK.PlaySFX(Water->sfx_Splash, 0, 255);
                        }

                        if (player->velocity.y >= -0x40000) {
                            player->velocity.y <<= 1;
                            if (player->velocity.y < -0x100000)
                                player->velocity.y = -0x100000;
                        }
                    }
                }
                else {
                    if (abs(player->groundVel) >= 0x78000) {
                        Hitbox *playerHitbox = Player_GetHitbox(player);
                        if (abs(player->position.y + (playerHitbox->bottom << 16) - Water->waterLevel) <= 0x40000 && player->groundedStore) {
                            Water->wakePosX[p] = player->position.x;
                            Water->wakeDir[p]  = player->groundVel < 0;
                            if (!player->onGround) {
                                player->onGround   = true;
                                player->position.y = Water->waterLevel - (playerHitbox->bottom << 16);
                            }
                        }
                    }
                }
                if (!player->sidekick && player->airTimer >= 1080) {
                    player->airTimer = 0;
                    Music_ResumePrevTrack(TRACK_DROWNING, false);
                }
            }
            else {
                bool32 notUnderwater = player->underwater == 0;
                player->underwater   = waterID;
                if (notUnderwater) {
                    Player_ChangePhysicsState(player);
                    if (player->velocity.y /*&& (!Current || !((1 << RSDK.GetEntityID(player)) & Current->activePlayers))*/) {
                        if (!Water->field_D4) {
                            if (childPtr) {
                                EntityWater *splash = (EntityWater *)RSDK.CreateEntity(Water->objectID, (void *)6, player->position.x,
                                                                                       childPtr->position.y - (childPtr->size.x >> 1));
                                splash->drawOrder   = player->drawOrder;
                                splash->childPtr    = childPtr;
                            }
                            else {
                                RSDK.CreateEntity(Water->objectID, (void *)6, player->position.x, Water->waterLevel);
                            }
                            RSDK.PlaySFX(Water->sfx_Splash, 0, 255);
                        }
                        player->velocity.y >>= 2;
                    }
                    player->velocity.x >>= 1;
                    if (!player->collisionMode)
                        player->groundVel >>= 1;
                    player->airTimer = 0;
                    Water->array3[p] = 52;
                    Water->array5[p] = 0;
                }
                else {
                    if (player->invincibleTimer <= 0) {
                        if (player->shield == SHIELD_FIRE) {
                            player->shield = SHIELD_NONE;
                            RSDK.ResetEntityPtr(RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player)), 0, 0);
                        }
                        if (player->shield == SHIELD_LIGHTNING) {
                            player->shield = SHIELD_NONE;
                            RSDK.ResetEntityPtr(RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player)), 0, 0);
                        }
                    }
                    if (player->shield != SHIELD_BUBBLE) {
                        ++player->airTimer;
                        Water_SpawnBubble(player, p);

                        bool32 alertFlag = true;
                        switch (player->airTimer) {
                            default: alertFlag = false; break;
                            case 960:
                            case 660:
                            case 360:
                                if (!player->sidekick)
                                    RSDK.PlaySFX(Water->sfx_Warning, 0, 255);
                                break;
                            case 1080: Water_SpawnCountDownBubble(player, p, 5); break;
                            case 1200: Water_SpawnCountDownBubble(player, p, 4); break;
                            case 1320: Water_SpawnCountDownBubble(player, p, 3); break;
                            case 1440: Water_SpawnCountDownBubble(player, p, 2); break;
                            case 1560: Water_SpawnCountDownBubble(player, p, 1); break;
                            case 1680: Water_SpawnCountDownBubble(player, p, 0); break;
                            case 1800:
                                player->hurtFlag = 3;
                                if (!water)
                                    player->drawOrder = Zone->playerDrawHigh;
                                break;
                            case 1140:
                            case 1380:
                                // trust me dude
                                break;
                        }
                        if (alertFlag) {
                            if (
#if RETRO_USE_PLUS
                                globals->gameMode == MODE_ENCORE || 
#endif
                                globals->gameMode == MODE_COMPETITION)
                                RSDK.PlaySFX(Water->sfx_DrownAlert, 0, 255);
                        }
                    }
                }
            }
        }

        if (entity->state == Player_State_FlyIn) {
            if (player->entPtr) {
                entity->position.x = player->position.x;
                entity->position.y = player->position.y;
            }
        }
    }
}

void Water_State_Tint(void)
{
    // nothing, its handled by a "master" palette state
}

void Water_State_Splash(void)
{
    RSDK_THIS(Water);
    EntityWater *water = entity->childPtr;

    if (water) {
        if (water != (EntityWater *)1)
            entity->position.y = water->position.y - (water->size.y >> 1);
    }
    else {
        entity->position.y = Water->waterLevel;
    }

    RSDK.ProcessAnimation(&entity->waterData);
    if (entity->waterData.frameID == entity->waterData.frameCount - 1)
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
}

void Water_Unknown4(void)
{
    RSDK_THIS(Water);

    if (entity->field_BC || entity->field_C0) {
        entity->position.x += entity->field_BC;
        entity->position.y += entity->field_C0;
    }
    else {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
    }

    entity->position.x += entity->field_B4;
    entity->position.y += entity->field_B8;

    int anim = entity->waterData.animationID;
    if ((anim == 3 && entity->waterData.frameID < 12) || anim == 4 || anim == 5 || (anim == 7 && !entity->activePlayers)) {
        entity->position.x = (RSDK.Sin512(entity->angle) << 9) + entity->field_68;
        entity->angle      = (entity->angle + 4) & 0x1FF;
    }

    if (entity->position.y < Water->waterLevel) {
        bool32 flag = false;
        foreach_active(Water, water)
        {
            if (water->type == 1 && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Water->hitbox))
                flag = true;
        }

        if (!flag) {
            if (entity->waterData.animationID == 3) {
                if (entity->waterData.frameID > 12) {
                    RSDK.SetSpriteAnimation(Water->spriteIndex, 6, &entity->waterData, 0, 0);
                    entity->velocity.y = 0;
                }
            }
            else if (entity->waterData.animationID == 7) {
                Water_Unknown5(entity, 0);
            }
            else {
                RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
            }
        }
    }
}

void Water_Unknown5(EntityWater *entity, int a2)
{
    if (entity->timer <= 0) {
        RSDK.SetSpriteAnimation(Water->spriteIndex, 6, &entity->waterData, true, 0);
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        RSDK.PlaySFX(Water->sfx_DNABurst, 0, 255);

        foreach_active(Player, player)
        {
            int id = RSDK.GetEntityID(player);
            if ((1 << id) & entity->activePlayers) {
                if (a2) {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                }
                else {
                    if (player->state != Player_State_None || player->playerAnimator.animationID != ANI_BREATHE) {
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + id, Shield);
                        if (shield)
                            shield->visible = true;
                        entity->activePlayers &= ~(1 << id);
                    }
                    else {
                        RSDK.SetSpriteAnimation(player->spriteIndex, 6, &player->playerAnimator, true, 0);
                        EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + id, Shield);
                        if (shield)
                            shield->visible = 1;
                        entity->activePlayers &= ~(1 << id);
                    }
                }
            }
        }
    }
}

void Water_State_Bubble(void)
{
    RSDK_THIS(Water);
    EntityPlayer *player = (EntityPlayer *)entity->childPtr;
    if (entity->waterData.animationID == 6 && entity->waterData.frameID == entity->waterData.frameCount - 1)
        destroyEntity(entity);
    if (player && player->state == Player_State_Bubble && entity->waterData.frameID < 3)
        entity->field_68 += 0x40000;

    if (entity->speed != -1) {
        if (entity->speed) {
            entity->field_68 += entity->velocity.x;
            entity->velocity.x += entity->speed;
        }
        Water_Unknown4();
        if (entity->tileCollisions) {
            if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, false)) {
                while (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x100000, false)
                       && RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x100000, false)) {
                    entity->position.y += 0x10000;
                }
            }
            else {
                while (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 0)) {
                    entity->position.y -= 0x10000;
                }
            }
        }
    }

    RSDK.ProcessAnimation(&entity->waterData);
    if (entity->waterData.frameID >= 13 || entity->waterData.animationID == 7) {
        if (entity->gapA4) {
            if (entity->field_A8 == 16) {
                RSDK.SetSpriteAnimation(Water->bigBubbleSprite, 7, &entity->waterData, true, 0);
                entity->scale.x = 0x180;
                entity->scale.y = 0x180;
            }

            if (entity->field_A8 <= 0) {
                entity->activePlayers  = 0;
                entity->activePlayers2 = 0;
                entity->state          = Water_Unknown8;
            }
            else {
                entity->field_A8--;
            }
        }
        else {
            foreach_active(Player, player)
            {
                if (!Player_CheckValidState(player) && player->state != Player_State_FlyIn) {
                }
                else {
                    if (player->shield == SHIELD_BUBBLE || !player->underwater || Water_Unknown7(player)
                        || player->position.x < entity->position.x - 0x100000 || player->position.x > entity->position.x + 0x100000) {
                    }
                    else {
                        bool32 flag = false;
                        if (player->playerAnimator.animationID == ANI_FAN) {
                            flag = (player->position.y >= entity->position.y + 0x100000) && !(player->position.y < entity->position.y - 0x100000);
                        }
                        else {
                            flag = (player->position.y >= entity->position.y + 0x100000) && !(player->position.y <= entity->position.y);
                        }

                        if (!(entity->field_6E & 1)) {
                            entity->field_6E |= 1;
                            flag = true;
                        }

                        if (flag) {
                            if (!player->sidekick) {
                                entity->state       = Water_Unknown6;
                                entity->countdownID = 0;
                                entity->velocity.y  = 0;
                                entity->childPtr    = player;
                                if (/*player->state != Current_Unknown14 && player->state != Current_Unknown13 && player->state != Current_Unknown15
                                    && player->state != Current_Unknown16*/
                                    true) {
                                    player->velocity.x = 0;
                                    player->velocity.y = 0;
                                    player->groundVel  = 0;
                                    flag               = true;

                                    if (player->characterID == ID_TAILS) {
                                        int anim = player->playerAnimator.animationID;
                                        flag     = anim != ANI_FLY && anim != ANI_FLYTIRED && anim != ANI_FLYLIFT && anim != ANI_SWIM
                                               && anim != ANI_SWIMLIFT;
                                    }
                                    if (player->characterID == ID_KNUCKLES) {
                                        int anim = player->playerAnimator.animationID;
                                        flag     = anim != ANI_DROPDASH && anim != ANI_FLY && anim != ANI_FLYLIFTTIRED && anim != ANI_SWIM
                                               && anim != ANI_SWIMTIRED && anim != ANI_SWIMLIFT;
                                    }

                                    if (flag && (player->playerAnimator.animationID != ANI_FAN && player->playerAnimator.animationID != ANI_CLING)) {
                                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BUBBLE, &player->playerAnimator, false, 0);
                                        if (!player->sidekick)
                                            entity->field_78 = 1;
                                    }

                                    if (player->state == Player_State_FlyCarried) {
                                        player->state                                        = Player_State_Air;
                                        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->flyCarryTimer = 30;
                                    }
#if RETRO_USE_PLUS
                                    else if (player->state == Player_State_MightyHammerDrop) {
                                        player->state = Player_State_Air;
                                    }
#endif
                                }
                                player->airTimer = 0;
                                if (!player->sidekick)
                                    Music_ResumePrevTrack(TRACK_DROWNING, false);
                                RSDK.PlaySFX(Water->sfx_Breathe, 0, 255);
                            }
                        }
                    }
                }
            }
        }
    }

    if (entity->speed != -1) {
        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
}

void Water_Unknown6(void)
{
    RSDK_THIS(Water);
    EntityPlayer *player = (EntityPlayer *)entity->childPtr;
    if (player->state == Player_State_Hit || !Player_CheckValidState(player)) {
        entity->field_78 = 0;
    }

    if (entity->speed) {
        entity->position.x += entity->velocity.x;
        entity->velocity.x += entity->speed;
    }

    RSDK.ProcessAnimation(&entity->waterData);
    entity->scale.x -= 0x18;
    entity->scale.y -= 0x18;
    if (entity->scale.x > 0) {
        if (entity->field_78 == 1)
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BUBBLE, &player->playerAnimator, false, 0);
    }
    else {
        entity->scale.x = 0;
        entity->scale.y = 0;
        if (entity->field_78 == 1)
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_WALK, &player->playerAnimator, false, 0);
        destroyEntity(entity);
    }
}

EntityWater *Water_Unknown7(EntityPlayer *entityPtr)
{
    int playerID = RSDK.GetEntityID(entityPtr);

    foreach_active(Water, water)
    {
        if (water->state == Water_Unknown8 && !((1 << playerID) & water->activePlayers)) {
            foreach_return water;
        }
    }
    return NULL;
}

void Water_Unknown8(void)
{
    RSDK_THIS(Water);
    if (entity->waterData.animationID == 6 && entity->waterData.frameID == entity->waterData.frameCount - 1) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }

    Water_Unknown4();
    entity->countdownID = entity->waterData.frameID;
    RSDK.ProcessAnimation(&entity->waterData);
    if (entity->waterData.frameID == entity->countdownID || entity->waterData.animationID == 7) {
        if (entity->scale.x < 0x200) {
            entity->scale.y += 8;
            entity->scale.x += 8;
        }
    }
    else {
        entity->scale.x = 0x180;
        entity->scale.y = 0x180;
    }

    if (entity->waterData.animationID != 6) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->objectID != Player->objectID)
                continue;
            if (player->hurtFlag)
                continue;

            if (!Player_CheckValidState(player) || !player->underwater) {
                continue;
            }

            if (!(entity->activePlayers & (1 << playerID)) && !((1 << playerID) & entity->activePlayers2)) {
                if (globals->gameMode == MODE_COMPETITION && entity->activePlayers)
                    continue;

                if (Player_CheckCollisionTouch(player, &entity, &Water->hitbox2) && !Water_Unknown7(player)) {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BREATHE, &player->playerAnimator, true, 0);
                    player->state        = Player_State_None;
                    EntityShield *shield = RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player));
                    if (shield)
                        shield->visible = 0;

                    player->airTimer = 0;
                    if (!player->sidekick)
                        Music_ResumePrevTrack(TRACK_DROWNING, false);

                    RSDK.PlaySFX(Water->sfx_DNAGrab, 0, 255);
                    entity->activePlayers |= 1 << playerID;
                    entity->activePlayers2 |= 1 << playerID;
                    if (RSDK.GetEntityID(entity) >= 64) {
                        int id = 32;
                        while (RSDK_GET_ENTITY(id, )->objectID) {
                            if (++id >= 32 + PLAYER_MAX)
                                break;
                        }
                        if (id >= 0 && id < 32 + PLAYER_MAX) {
                            RSDK.AddDrawListRef(entity->drawOrder, id);
                            RSDK.CopyEntity(RSDK.GetEntityByID(id), entity, true);
                            return;
                        }
                    }
                }
            }

            if ((1 << playerID) & entity->activePlayers) {
                if (entity->timer > 0) {
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_BREATHE, &player->playerAnimator, 0, 0);
                    player->state = Player_State_None;
                }

                player->airTimer   = 0;
                player->position.x = entity->position.x;
                player->position.y = entity->position.y;
                player->velocity.x = entity->velocity.x;
                player->velocity.y = entity->velocity.y;
                if (player->sidekick) {
                    if (player->left) {
                        player->direction = FLIP_X;
                    }
                    else if (player->right) {
                        player->direction = FLIP_NONE;
                    }

                    if (player->jumpPress) {
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
                        player->state = Player_State_Air;
                        entity->activePlayers &= ~(1 << playerID);
                        entity->activePlayers2 |= 1 << playerID;
                    }
                }
                else {
                    entity->drawOrder      = player->drawOrder;
                    entity->collisionPlane = player->collisionPlane;
                    if (player->state != Player_State_None && player->playerAnimator.animationID != ANI_BREATHE) {
                        Water_Unknown5(entity, 0);
                        return;
                    }

                    int xVel = 0;
                    if (player->left) {
                        player->direction = FLIP_X;
                        xVel              = -0x11000;
                    }
                    else if (player->right) {
                        player->direction = FLIP_NONE;
                        xVel              = 0x11000;
                    }

                    int yVel = 0x8800;
                    if (player->down)
                        yVel = -0x4400;
                    else if (player->up)
                        yVel = -0x11000;

                    if (xVel - entity->velocity.x)
                        entity->velocity.x += (2 * ((xVel - entity->velocity.x) > 0) - 1) << 11;

                    if (yVel - entity->velocity.y)
                        entity->velocity.y += (2 * ((yVel - entity->velocity.y) > 0) - 1) << 11;

                    if (player->jumpPress) {
                        Water_Unknown5(entity, 1);
                        return;
                    }
                }
            }

            if ((1 << playerID) & entity->activePlayers2) {
                if (!Player_CheckCollisionTouch(player, entity, &Water->hitbox2))
                    entity->activePlayers2 &= ~(1 << playerID);
            }
        }

        if (entity->timer > 0)
            entity->timer--;

        Hitbox thisHitbox;
        thisHitbox.left   = -0x14;
        thisHitbox.top    = -0x13;
        thisHitbox.right  = 0x14;
        thisHitbox.bottom = 0x14;

        foreach_active(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionTouchBox(entity, &thisHitbox, spikes, &spikes->hitbox)) {
                Water_Unknown5(entity, 0);
                return;
            }
        }

        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0x180000, true)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, -0x180000, 0, true)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, 0x180000, 0, true)) {
            Water_Unknown5(entity, 0);
            return;
        }
    }
}

void Water_Unknown9(void)
{
    RSDK_THIS(Water);

    if (entity->taggedObject) {
        if (entity->timer <= 0) {
            Hitbox hitbox;
            hitbox.left   = -0x20;
            hitbox.top    = -0x1F;
            hitbox.right  = 0x20;
            hitbox.bottom = 0x20;

            if (/*((EntityButton *)entity->taggedObject)->flag*/ false) {
                foreach_active(Player, player)
                {
                    if (Player_CheckCollisionTouch(player, entity, &hitbox)) {
                        if (player->playerAnimator.animationID != ANI_BREATHE) {
                            EntityWater *bigBubble =
                                (EntityWater *)RSDK.CreateEntity(Water->objectID, (void *)7, entity->position.x, entity->position.y);
                            RSDK.SetSpriteAnimation(Water->bigBubbleSprite, 7, &bigBubble->waterData, true, 0);

                            bigBubble->state      = Water_Unknown8;
                            bigBubble->gapA4      = 1;
                            bigBubble->field_A8   = 0;
                            bigBubble->velocity.y = -0x8800;
                            bigBubble->childPtr   = 0;
                            bigBubble->field_68   = bigBubble->position.x;
                            bigBubble->timer      = 0x1E;
                            bigBubble->scale.x    = 0x180;
                            bigBubble->scale.y    = 0x180;
                            entity->timer         = 60;
                        }
                    }
                }
            }
        }
        else {
            entity->timer--;
        }
    }
}

void Water_State_Bubbler(void)
{
    RSDK_THIS(Water);

    entity->visible = false;
    if (entity->position.y > Water->waterLevel) {
        entity->visible = true;
    }

    foreach_active(Water, water)
    {
        if (water->type == 1 && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Water->hitbox))
            entity->visible = true;
    }

    if (entity->visible && RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        if (!entity->countdownID) {
            if (!entity->field_6E) {
                entity->field_6E    = 1;
                int rand            = RSDK.Rand(0, 0x10000);
                entity->bubbleType1 = rand % 6;
                entity->bubbleType2 = rand & 0xC;
                if (!entity->field_6D) {
                    entity->field_6E |= 2;
                    entity->field_6D = entity->numDuds;
                }
            }
            entity->countdownID = RSDK.Rand(0, 32);
            EntityWater *bubble = (EntityWater *)RSDK.CreateEntity(Water->objectID, (void *)7, entity->position.x, entity->position.y - 0x20000);
            int bubbleSize      = Water->bubbleSizes[entity->bubbleType1 + entity->bubbleType2];
            bubble->waterData.loopIndex  = bubbleSize;
            bubble->waterData.frameCount = bubbleSize + 1;
            if (entity->type == 4) {
                bubble->gapA4    = 1;
                bubble->field_A8 = 16;
            }
            bubble->position.x += RSDK.Rand(-8, 9) << 16;
            bubble->velocity.y = -0x8800;
            bubble->angle      = 2 * RSDK.Rand(0, 256);

            bubble->childPtr = NULL;
            bubble->field_68 = bubble->position.x;
            if (entity->field_6E & 2 && (!RSDK.Rand(0, 4) || !entity->bubbleType1) && !(entity->field_6E & 4)) {
                RSDK.SetSpriteAnimation(Water->spriteIndex, 3, &bubble->waterData, 0, 0);
                bubble->isPermanent = true;
                entity->field_6E |= 4;
            }

            if (--entity->bubbleType1 < 0) {
                entity->field_6E = 0;
                entity->countdownID += RSDK.Rand(0, 128) + 128;
            }
        }
        else {
            --entity->countdownID;
        }
    }
    RSDK.ProcessAnimation(&entity->waterData);
}

void Water_State_CountDownBubble(void)
{
    RSDK_THIS(Water);
    EntityPlayer *player = (EntityPlayer *)entity->childPtr;
    if (player->playerAnimator.animationID == ANI_FAN) {
        entity->field_68 += player->velocity.x;
        entity->position.y += player->velocity.y;
    }
    RSDK.ProcessAnimation(&entity->waterData);

    bool32 flag = false;
    if (entity->position.y >= Water->waterLevel) {
        flag = true;
    }
    else {
        foreach_active(Water, water)
        {
            if (water->type == 1 && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Water->hitbox)) {
                flag = true;
            }
        }
    }

    if ((entity->waterData.animationID != 7 || entity->waterData.frameID != entity->waterData.frameCount - 1) && flag) {
        entity->position.y += entity->velocity.y;
        entity->position.x = (RSDK.Sin512(entity->angle) << 9) + entity->field_68;
        entity->angle      = (entity->angle + 4) & 0x1FF;
    }
    else {
        RSDK.SetSpriteAnimation(Water->spriteIndex, entity->countdownID + 8, &entity->waterData, true, 0);
        if (player->camera) {
            entity->size.x = (entity->position.x & 0xFFFF0000) - (player->camera->position.x & 0xFFFF0000);
            entity->size.y = (entity->position.y & 0xFFFF0000) - (player->camera->position.y & 0xFFFF0000);
        }
        else {
            entity->size.x = (entity->position.x & 0xFFFF0000) - (player->position.x & 0xFFFF0000);
            entity->size.y = (entity->position.y & 0xFFFF0000) - (player->position.y & 0xFFFF0000);
        }
        entity->state = Water_State_BubbleMove;
    }
}

void Water_State_BubbleMove(void)
{
    RSDK_THIS(Water);

    RSDK.ProcessAnimation(&entity->waterData);
    if (entity->angle >= 640) {
        entity->scale.x -= 8;
        entity->scale.y -= 8;
        if (entity->scale.x <= 0)
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
    else {
        entity->scale.x = (RSDK.Sin256(entity->angle) >> 1) + 0x200;
        entity->scale.y = (RSDK.Sin256(entity->angle + 0x80) >> 1) + 0x200;
        entity->angle += 6;
    }
}

void Water_State_Adjustable(void)
{
    RSDK_THIS(Water);
    bool32 flag = false;

    if (entity->taggedObject) {
        /*EntityButton *button = (EntityButton *)entity->taggedObject;
        if (button->flag) {
            Water->field_C8 = 1;
            flag            = true;
        }*/
    }

    int px = entity->position.x + 1;
    if (!flag) {
        foreach_active(Player, player)
        {
            if (!RSDK.GetEntityID(player)) {
                if (abs(entity->position.x - player->position.x) < entity->updateRange.x) {
                    if (abs(entity->position.y - player->position.y) < entity->updateRange.y) {
                        flag = true;
                        px   = player->position.x;
                    }
                }
            }
        }
    }

    if (flag) {
        if (entity->taggedObject) {
            Water->targetWaterLevel = entity->position.y;
        }
        else if (px <= entity->position.x) {
            Water->targetWaterLevel = entity->height.x;
        }
        else {
            Water->targetWaterLevel = entity->height.y;
        }

        Water->waterMoveSpeed = entity->speed << 15;
        if (entity->destroyOnTrigger)
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, 0);
    }
}

void Water_State_Draw_Palette(void)
{
    Vector2 drawPos;

    RSDK_THIS(Water);
    ScreenInfo *screen = &RSDK_screens[RSDK_sceneInfo->currentScreenID];
    drawPos.x          = ((screen->position.x & 0xFFFFFFC0) + 32) << 16;
    drawPos.y          = Water->waterLevel;
    for (int i = (screen->width >> 6) + 2; i > 0; --i) {
        RSDK.DrawSprite(&entity->waterData, &drawPos, 0);
        drawPos.x += 0x400000;
    }

    entity->drawFX |= FX_FLIP;

    for (int i = 0; i < 4; ++i) {
        if (Water->wakePosX[i] > 0) {
            entity->direction = Water->wakeDir[i];
            drawPos.x         = Water->wakePosX[i];
            RSDK.DrawSprite(&Water->wakeData, &drawPos, 0);
        }
    }

    entity->drawFX &= ~FX_FLIP;
}

void Water_State_Draw_Tint(void)
{
    RSDK_THIS(Water);
    RSDK.DrawRect(entity->position.x - (entity->size.x >> 1), entity->position.y - (entity->size.y >> 1), entity->size.x, entity->size.y,
                  entity->b + ((entity->g + (entity->r << 8)) << 8), 0x100, INK_SUB, false);
}

void Water_State_Draw_Splash(void)
{
    RSDK_THIS(Water);
    RSDK.DrawSprite(&entity->waterData, NULL, false);
}

void Water_State_Draw_CountDownBubble(void)
{
    Vector2 drawPos;

    RSDK_THIS(Water);
    if (entity->state == Water_State_CountDownBubble) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
    }
    else {
        EntityPlayer *player = (EntityPlayer *)entity->childPtr;
        if (player->camera) {
            drawPos.x = player->camera->position.x + entity->size.x;
            drawPos.y = player->camera->position.y + entity->size.y;
        }
        else {
            drawPos.x = player->position.x + entity->size.x;
            drawPos.y = player->position.y + entity->size.y;
        }
    }

    RSDK.DrawSprite(&entity->waterData, &drawPos, false);
}

void Water_State_Draw_Bubbler(void)
{
    RSDK_THIS(Water);
    RSDK.DrawSprite(&entity->waterData, NULL, false);
}

void Water_State_Draw_Bubble(void)
{
    RSDK_THIS(Water);
    RSDK.DrawSprite(&entity->waterData, NULL, false);
}

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
