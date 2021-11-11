#include "SonicMania.h"

ObjectEggPistonsMKII *EggPistonsMKII;

void EggPistonsMKII_Update(void)
{
    RSDK_THIS(EggPistonsMKII);
    StateMachine_Run(entity->state);
}

void EggPistonsMKII_LateUpdate(void) {}

void EggPistonsMKII_StaticUpdate(void) {}

void EggPistonsMKII_Draw(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (entity->type == EGGPISTON_ALARM) {
        RSDK.FillScreen(0x880000, EggPistonsMKII->alarmTimer, EggPistonsMKII->alarmTimer, EggPistonsMKII->alarmTimer);
    }
    else {
        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.DrawSprite(&entity->animator1, NULL, false);
    }
}

void EggPistonsMKII_Create(void *data)
{
    RSDK_THIS(EggPistonsMKII);

    entity->drawFX = FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active = ACTIVE_BOUNDS;
            if (data)
                entity->type = voidToInt(data);

            switch (entity->type) {
                case EGGPISTON_PISTON:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 0, &entity->animator1, true, 0);
                    entity->updateRange.x                                  = 0xA00000;
                    entity->updateRange.y                                  = 0xEC0000;
                    entity->hitbox.left                                    = -32;
                    entity->hitbox.top                                     = -96;
                    entity->hitbox.right                                   = 32;
                    entity->hitbox.bottom                                  = 60;
                    entity->state                                          = EggPistonsMKII_StatePiston_Unknown1;
                    entity->visible                                        = true;
                    entity->drawOrder                                      = Zone->fgLayerHigh - 1;
                    entity->pistonID                                       = entity->position.y;
                    EggPistonsMKII->pistons[EggPistonsMKII->pistonCount++] = (Entity *)entity;
                    break;
                case EGGPISTON_CONTROL:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &entity->animator1, true, 0);
                    entity->updateRange.x      = 0x800000;
                    entity->updateRange.y      = 0x800000;
                    entity->hitbox.left        = -33;
                    entity->hitbox.top         = -37;
                    entity->hitbox.right       = 33;
                    entity->hitbox.bottom      = 6;
                    entity->state              = EggPistonsMKII_State_SetupArena;
                    entity->visible            = false;
                    entity->drawOrder          = Zone->drawOrderLow;
                    EggPistonsMKII->controller = (Entity *)entity;
                    break;
                case EGGPISTON_EMITTER:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 1, &entity->animator1, true, 0);
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->hitbox.left   = -8;
                    entity->hitbox.top    = -8;
                    entity->hitbox.right  = 8;
                    entity->hitbox.bottom = 8;
                    entity->state         = EggPistonsMKII_StatePiston_Unknown1;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow + 1;
                    break;
                case EGGPISTON_BARRIER:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &entity->animator1, true, 0);
                    entity->hitbox.left   = -32;
                    entity->hitbox.top    = -32;
                    entity->hitbox.right  = 32;
                    entity->hitbox.bottom = 32;
                    entity->state         = EggPistonsMKII_StateOrbGenerator_Idle;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    break;
                case EGGPISTON_PLASMABALL:
                    RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 2, &entity->animator1, true, 0);
                    entity->hitbox.left   = -12;
                    entity->hitbox.top    = -12;
                    entity->hitbox.right  = 12;
                    entity->hitbox.bottom = 12;
                    entity->state         = EggPistonsMKII_StateOrb_Unknown1;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    break;
                case EGGPISTON_ALARM:
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->state         = EggPistonsMKII_StateAlarm_Active;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void EggPistonsMKII_StageLoad(void)
{
    EggPistonsMKII->aniFrames          = RSDK.LoadSpriteAnimation("MMZ/EggPistonsMKII.bin", SCOPE_STAGE);
    EggPistonsMKII->eggmanFrames       = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ1.bin", SCOPE_STAGE);
    EggPistonsMKII->pistonCount        = 0;
    EggPistonsMKII->invincibilityTimer = 0;
    EggPistonsMKII->field_2D           = 0;
    EggPistonsMKII->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    EggPistonsMKII->sfxWall            = RSDK.GetSFX("Stage/Wall.wav");
    EggPistonsMKII->sfxElectrify       = RSDK.GetSFX("Stage/Electrify.wav");
    EggPistonsMKII->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    EggPistonsMKII->sfxAlarm           = RSDK.GetSFX("MMZ/Alarm.wav");
    RSDK.StopSFX(EggPistonsMKII->sfxAlarm);
}

void EggPistonsMKII_CheckPlayerCollisions_Piston(void)
{
    RSDK_THIS(EggPistonsMKII);
    EntityEggPistonsMKII *parent = (EntityEggPistonsMKII *)entity->parent;

    foreach_active(Player, player)
    {
        int side = Player_CheckCollisionBox(player, entity, &entity->hitbox);
        if (side == C_TOP) {
            player->position.y += entity->velocity.y;
            player->position.y &= 0xFFFF0000;
        }
        else if (side == C_BOTTOM && player->onGround)
            player->hurtFlag = 1;
    }

    entity->position.y += entity->velocity.y;
    if (parent) {
        parent->position.x = entity->position.x;
        parent->position.y = entity->position.y;
        if (entity->direction)
            parent->position.y += 0xE0000;
        else
            parent->position.y -= 0xA0000;
    }
}

void EggPistonsMKII_CheckPlayerCollisions(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (!EggPistonsMKII->invincibilityTimer && Player_CheckBadnikTouch(player, entity, &entity->hitbox)
            && EggPistonsMKII_CheckPlayerAttacking(player)) {
            EggPistonsMKII->invincibilityTimer = 120;
            if (EggPistonsMKII->health > 0)
                EggPistonsMKII->health--;

            if (EggPistonsMKII->health) {
                RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 1, &entity->animator1, true, 0);
                RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 4, &entity->animator2, true, 0);
                RSDK.PlaySfx(EggPistonsMKII->sfxHit, false, 255);
            }
            else {
                RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 2, &entity->animator1, true, 0);
                RSDK_sceneInfo->timeEnabled = false;
                Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                entity->animator1.frameID = 1;
                RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
                entity->state = EggPistonsMKII_State_Finish;
            }
        }
    }
}

void EggPistonsMKII_CheckPlayerCollisions_Ball(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
        }
    }
}

void EggPistonsMKII_HandlePlayerCollisions(void)
{
    RSDK_THIS(EggPistonsMKII);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == C_BOTTOM) {
            if (player->onGround)
                player->hurtFlag = 1;
        }
    }
}

EntityEggPistonsMKII *EggPistonsMKII_GetNextPiston(void)
{
    RSDK_THIS(EggPistonsMKII);

    EntityEggPistonsMKII *piston = NULL;
    int pistonID                 = 0;

    do {
        do {
            pistonID = RSDK.Rand(0, 5);
            piston   = (EntityEggPistonsMKII *)EggPistonsMKII->pistons[pistonID];
        } while (piston->state != EggPistonsMKII_StatePiston_Unknown1);
    } while (pistonID == EggPistonsMKII->pistonID - 1 || pistonID == EggPistonsMKII->pistonID + 1);

    EggPistonsMKII->pistonID = pistonID;

    entity->visible = true;
    piston->timer   = 15;
    piston->state   = EggPistonsMKII_StatePiston_Unknown2;
    return piston;
}

void EggPistonsMKII_SpawnElecBall(void)
{
    EntityEggPistonsMKII *spawner = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 6, EggPistonsMKII);
    spawner->state                = EggPistonsMKII_StateOrbGenerator_Warning;
    spawner->timer                = 330;
    int offset                    = 0;

    for (int i = 0; i < 4; ++i) {
        EntityEggPistonsMKII *child = CREATE_ENTITY(EggPistonsMKII, intToVoid(EGGPISTON_PLASMABALL), spawner->position.x, spawner->position.y);
        child->pistonID             = offset + spawner->position.x + ((RSDK.Rand(0, 48) - 48) << 16);
        child->inkEffect            = INK_ADD;
        child->velocity.x           = (child->pistonID - child->position.x) >> 4;
        child->alpha                = 0;
        offset -= 0x570000;
    }
}

bool32 EggPistonsMKII_CheckPlayerAttacking(void *p)
{
    RSDK_THIS(EggPistonsMKII);
    EntityPlayer *player = (EntityPlayer *)p;

    if (Player_CheckAttacking(player, NULL)) {
        if (player->position.x >= entity->position.x) {
            player->velocity.x = 0x30000;
            player->groundVel  = 0x30000;
        }
        else {
            player->velocity.x = -0x30000;
            player->groundVel  = -0x30000;
        }
        if (player->characterID == ID_KNUCKLES && player->playerAnimator.animationID == ANI_FLY) {
            RSDK.SetSpriteAnimation(Player->knuxSpriteIndex, ANI_FLYTIRED, &player->playerAnimator, true, 0);
            player->state = Player_State_KnuxGlideDrop;
        }
        return true;
    }
    return false;
}

void EggPistonsMKII_State_SetupArena(void)
{
    RSDK_THIS(EggPistonsMKII);

    for (int i = 0; i < 6; ++i) {
        EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 8 + i, CollapsingPlatform);
        if (platform->objectID == CollapsingPlatform->objectID)
            platform->active = ACTIVE_NEVER;
    }

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 71;
        Zone->screenBoundsT1[0]     = (Zone->screenBoundsB1[0] - RSDK_screens->height);
        entity->active              = ACTIVE_NORMAL;
        entity->state               = EggPistonsMKII_State_EnterBoss;
    }
}

void EggPistonsMKII_State_EnterBoss(void)
{
    RSDK_THIS(EggPistonsMKII);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;

    int pos = RETRO_USE_PLUS ? (entity->position.x - 0xC00000) : entity->position.x;
    if (player1->position.x > pos) {
#if RETRO_USE_PLUS
        if (player1->onGround)
            player1->state = Player_State_Ground;
        player1->stateInput = 0;
        player1->jumpPress  = false;
        player1->jumpHold   = false;
        player1->left       = false;
        player1->right      = true;
        player1->up         = false;
        player1->down       = false;
        if (player1->groundVel > 0x30000)
            player1->groundVel = 0x30000;

        if (player1->position.x > (entity->position.x - 0x80000)) {
#endif
            Zone->playerBoundActiveL[0] = true;
            Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - RSDK_screens->centerX;
            Music_TransitionTrack(TRACK_MINIBOSS, 0.0125);

            EggPistonsMKII->health = 8;
            entity->timer          = 142;
            for (int i = 0; i < 2; ++i) {
                EntityEggPistonsMKII *barrier = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 14 + i, EggPistonsMKII);
                barrier->velocity.y           = -0x20000;
                barrier->timer                = 64;
                barrier->state                = EggPistonsMKII_State_Unknown1;
            }

            player1->right = false;
            player1->left  = true;
            entity->state  = EggPistonsMKII_State_PistonReveal;
#if RETRO_USE_PLUS
        }
#endif
    }
}

void EggPistonsMKII_State_PistonReveal(void)
{
    RSDK_THIS(EggPistonsMKII);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (!player1->stateInput && player1->groundVel <= 0) {
        player1->groundVel  = 0;
        player1->velocity.x = 0;
        player1->left       = false;
        player1->direction  = FLIP_NONE;
    }

    if (--entity->timer <= 0) {
        player1->stateInput = Player_ProcessP1Input;

        for (int i = 0; i < 6; ++i) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 8 + i, CollapsingPlatform);
            if (platform->objectID == CollapsingPlatform->objectID) {
                platform->active        = ACTIVE_NORMAL;
                platform->collapseDelay = 8;
                platform->playerPos.x   = entity->position.x;
            }
        }

        for (int i = 0; i < 2; ++i) {
            EntityEggPistonsMKII *piston = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 14 + i, EggPistonsMKII);
            piston->velocity.y           = 0x80000;
            piston->timer                = 8;
            piston->state                = EggPistonsMKII_State_Unknown1;
        }

        entity->position.y -= 0x800000;
        entity->timer = 120;
        entity->state = EggPistonsMKII_State_ClassicMode;
    }
}

void EggPistonsMKII_State_ClassicMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--entity->timer <= 0) {
        if (entity->pistonID) {
            if (EggPistonsMKII->health > 4) {
                EggPistonsMKII_SpawnElecBall();
                RSDK.PlaySfx(EggPistonsMKII->sfxElectrify, false, 255);
                entity->timer = 345;
            }
            else {
                entity->timer = 120;
                entity->state = EggPistonsMKII_State_StartPinchMode;
            }
        }
        else {
            EggPistonsMKII_GetNextPiston();
            EggPistonsMKII_GetNextPiston()->parent = (Entity *)entity;
            RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            entity->timer = 165;
            RSDK.PlaySfx(EggPistonsMKII->sfxWall, false, 255);
        }
        entity->pistonID ^= 1;
    }

    if (EggPistonsMKII->invincibilityTimer)
        EggPistonsMKII->invincibilityTimer--;
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= entity->position.x;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->visible)
        EggPistonsMKII_CheckPlayerCollisions();
}

void EggPistonsMKII_State_StartPinchMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--entity->timer <= 0) {
        CREATE_ENTITY(EggPistonsMKII, intToVoid(EGGPISTON_ALARM), entity->position.x, entity->position.y);

        EntityEggPistonsMKII *orbSpawner = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 6, EggPistonsMKII);
        RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
        CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), orbSpawner->position.x, orbSpawner->position.y)->drawOrder = Zone->drawOrderHigh;
        EggPistonsMKII->field_2D                                                                                        = 1;
        destroyEntity(orbSpawner);

        entity->timer    = 120;
        entity->pistonID = 1;
        RSDK.PlaySfx(EggPistonsMKII->sfxAlarm, true, 255);
        entity->state = EggPistonsMKII_State_PinchMode;
    }
}

void EggPistonsMKII_State_PinchMode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (--entity->timer <= 0) {
        EntityEggPistonsMKII *piston = EggPistonsMKII_GetNextPiston();
        if (!entity->pistonID) {
            piston->parent = (Entity *)entity;
            RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        }
        entity->pistonID = (entity->pistonID + 1) & 3;
        RSDK.PlaySfx(EggPistonsMKII->sfxWall, false, 255);
        entity->timer = 64;
    }

    if (EggPistonsMKII->invincibilityTimer)
        EggPistonsMKII->invincibilityTimer--;
    entity->direction = RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x <= entity->position.x;
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    if (entity->visible)
        EggPistonsMKII_CheckPlayerCollisions();

    if (!EggPistonsMKII->health)
        entity->state = EggPistonsMKII_State_Destroyed;
}

void EggPistonsMKII_State_Destroyed(void)
{
    RSDK_THIS(EggPistonsMKII);

    int id = 0;
    for (int i = 0; i < 5; ++i) {
        EntityEggPistonsMKII *piston = (EntityEggPistonsMKII *)EggPistonsMKII->pistons[i];

        if (piston->state == EggPistonsMKII_StatePiston_Unknown1) {
            ++id;
        }
    }

    if (++id >= 5) {
        EggPistonsMKII->field_2D = 0;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        RSDK.StopSFX(EggPistonsMKII->sfxAlarm);
        entity->state = EggPistonsMKII_State_Finish;
    }
}

void EggPistonsMKII_State_Finish(void) {}

void EggPistonsMKII_StatePiston_Unknown1(void) { EggPistonsMKII_CheckPlayerCollisions_Piston(); }

void EggPistonsMKII_StatePiston_Unknown2(void)
{
    RSDK_THIS(EggPistonsMKII);

    entity->position.y = (RSDK.Rand(-2, 3) << 16) + entity->pistonID;
    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--entity->timer) {
        entity->position.y = entity->pistonID;
        if (entity->direction == FLIP_NONE)
            entity->velocity.y = -0x8000;
        else
            entity->velocity.y = 0x8000;

        entity->timer = 32;
        entity->state = EggPistonsMKII_StatePiston_Unknown3;
    }

    if (!EggPistonsMKII->health) {
        entity->position.y = entity->pistonID;
        entity->state      = EggPistonsMKII_StatePiston_Explode;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = entity->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = entity->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Unknown3(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();
    if (!--entity->timer) {
        entity->timer = 48;
        if (entity->direction == FLIP_NONE)
            entity->velocity.y = -0x30000;
        else
            entity->velocity.y = 0x30000;
        entity->state = EggPistonsMKII_StatePiston_Unknown4;
    }

    if (!EggPistonsMKII->health) {
        if (entity->direction == FLIP_NONE)
            entity->velocity.y = 0x10000;
        else
            entity->velocity.y = -0x10000;
        entity->timer >>= 1;
        entity->state = EggPistonsMKII_StatePiston_Explode;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = entity->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = entity->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Unknown4(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--entity->timer) {
        entity->timer = 80;
        if (entity->direction == FLIP_NONE)
            entity->velocity.y = 0x20000;
        else
            entity->velocity.y = -0x20000;
        entity->state = EggPistonsMKII_StatePiston_Unknown5;
    }

    if (!EggPistonsMKII->health) {
        entity->state = EggPistonsMKII_StatePiston_Explode;
        if (entity->direction == FLIP_NONE)
            entity->velocity.y = 0x10000;
        else
            entity->velocity.y = -0x10000;
        entity->timer >>= 1;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = entity->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = entity->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Unknown5(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_CheckPlayerCollisions_Piston();

    if (!--entity->timer) {
        entity->velocity.y = 0;
        entity->parent     = NULL;
        entity->state      = EggPistonsMKII_StatePiston_Unknown1;
    }

    if (!EggPistonsMKII->health || (EggPistonsMKII->field_2D == 0 && EggPistonsMKII->health == 4)) {
        entity->state = EggPistonsMKII_StatePiston_Explode;
        if (entity->direction == FLIP_NONE)
            entity->velocity.y = 0x10000;
        else
            entity->velocity.y = -0x10000;

        int interval = 4 * (EggPistonsMKII->health != 0) + 3;
        if (!(Zone->timer % interval)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = entity->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = entity->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_StatePiston_Explode(void)
{
    RSDK_THIS(EggPistonsMKII);

    if ((entity->direction || entity->position.y < entity->pistonID) && (entity->direction != FLIP_Y || entity->position.y > entity->pistonID)) {
        EggPistonsMKII_CheckPlayerCollisions_Piston();
        if (!EggPistonsMKII->health || (((EggPistonsMKII->field_2D == 0) & (EggPistonsMKII->health == 4)) != 0)) {
            int interval = 4 * (EggPistonsMKII->health != 0) + 3;
            if (!(Zone->timer % interval)) {
                RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
                if (Zone->timer & 4) {
                    int x = entity->position.x + (RSDK.Rand(-24, 24) << 16);
                    int y = entity->position.y + (RSDK.Rand(-48, 48) << 16);
                    CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
                }
            }
        }
    }
    else {
        entity->position.y = entity->pistonID;
        entity->velocity.y = 0;
        entity->parent     = NULL;
        entity->state      = EggPistonsMKII_StatePiston_Unknown1;
    }
}

void EggPistonsMKII_StateOrbGenerator_Idle(void) {}

void EggPistonsMKII_StateOrbGenerator_Warning(void)
{
    RSDK_THIS(EggPistonsMKII);

    RSDK.ProcessAnimation(&entity->animator1);

    if ((entity->timer & 7) == 4)
        RSDK.PlaySfx(EggPistonsMKII->sfxElectrify, false, 255);

    if (!--entity->timer) {
        entity->animator1.frameID = 0;
        entity->state             = EggPistonsMKII_StateOrbGenerator_Idle;
    }
}

void EggPistonsMKII_StateOrb_Unknown1(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (entity->alpha < 0xC0)
        entity->alpha += 0x10;
    entity->position.x += entity->velocity.x;
    if (entity->position.x <= entity->pistonID) {
        entity->timer = 180;
        entity->state = EggPistonsMKII_StateOrb_Unknown2;
    }
    RSDK.ProcessAnimation(&entity->animator1);
}

void EggPistonsMKII_StateOrb_Unknown2(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (!--entity->timer) {
        entity->alpha      = 128;
        entity->velocity.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x - entity->position.x) >> 7;
        entity->velocity.y = 0x14000;
        entity->timer      = 133;
        RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 3, &entity->animator2, true, 0);
        entity->state = EggPistonsMKII_StateOrb_Unknown3;
        entity->animator1.animationSpeed *= 2;
    }
    RSDK.ProcessAnimation(&entity->animator1);
}

void EggPistonsMKII_StateOrb_Unknown3(void)
{
    RSDK_THIS(EggPistonsMKII);

    entity->velocity.x = (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x - entity->position.x) >> 7;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (!--entity->timer)
        destroyEntity(entity);
    RSDK.ProcessAnimation(&entity->animator1);
    EggPistonsMKII_CheckPlayerCollisions_Ball();
}

void EggPistonsMKII_StateAlarm_Active(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII->alarmTimer = (abs(RSDK.Sin256(2 * EggPistonsMKII->alarmAngle)) >> 1) + 32;
    ++EggPistonsMKII->alarmAngle;

    if (!EggPistonsMKII->health && !EggPistonsMKII->field_2D && EggPistonsMKII->alarmTimer == 32)
        entity->state = EggPistonsMKII_StateAlarm_Destroyed;
}

void EggPistonsMKII_StateAlarm_Destroyed(void)
{
    RSDK_THIS(EggPistonsMKII);

    if (!--EggPistonsMKII->alarmTimer) {

        for (int i = 0; i < 2; ++i) {
            int slot                      = RSDK.GetEntityID(EggPistonsMKII->controller);
            EntityEggPistonsMKII *barrier = RSDK_GET_ENTITY(slot + 14 + i, EggPistonsMKII);

            for (int d = 0; d < 4; ++d) {
                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, barrier->position.x, barrier->position.y);
                RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &debris->animator, true, i + 1);
                debris->gravity       = 0x3800;
                debris->velocity.x    = ((i & 1) << 17 >> (i >> 1)) - 0x10000;
                debris->velocity.y    = 0x18000 * ((i >> 1) - 2);
                debris->drawOrder     = Zone->drawOrderHigh;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
            }

            destroyEntity(barrier);
        }

        Zone->screenBoundsR1[0] += 424;
        Zone->playerBoundActiveR[0] = false;
        destroyEntity(entity);
    }
}

void EggPistonsMKII_State_Explode(void)
{
    RSDK_THIS(EggPistonsMKII);

    EggPistonsMKII_HandlePlayerCollisions();
    if (!EggPistonsMKII->health) {
        if (!(Zone->timer % 3)) {
            RSDK.PlaySfx(EggPistonsMKII->sfxExplosion, false, 255);
            if (Zone->timer & 4) {
                int x = entity->position.x + (RSDK.Rand(-24, 24) << 16);
                int y = entity->position.y + (RSDK.Rand(-48, 48) << 16);
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh;
            }
        }
    }
}

void EggPistonsMKII_State_Unknown1(void)
{
    RSDK_THIS(EggPistonsMKII);

    entity->position.y += entity->velocity.y;
    if (--entity->timer <= 0) {
        entity->state = EggPistonsMKII_State_Explode;
        if (entity->velocity.y == 0x80000) {
            RSDK_GET_ENTITY(SLOT_CAMERA1, Camera)->shakePos.y = 4;
            entity->velocity.y                                = 0;
        }
        else if (entity->velocity.y == 0x20000) {
            entity->velocity.y = -0x20000;
            entity->timer      = 32;
            entity->state      = EggPistonsMKII_State_Unknown1;
        }
    }
    EggPistonsMKII_HandlePlayerCollisions();
}

#if RETRO_INCLUDE_EDITOR
void EggPistonsMKII_EditorDraw(void)
{
    RSDK_THIS(EggPistonsMKII);

    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;

    switch (entity->type) {
        case EGGPISTON_PISTON: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 0, &entity->animator1, true, 0); break;
        case EGGPISTON_CONTROL: RSDK.SetSpriteAnimation(EggPistonsMKII->eggmanFrames, 0, &entity->animator1, true, 0); break;
        case EGGPISTON_EMITTER: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 1, &entity->animator1, true, 0); break;
        case EGGPISTON_BARRIER: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 5, &entity->animator1, true, 0); break;
        case EGGPISTON_PLASMABALL: RSDK.SetSpriteAnimation(EggPistonsMKII->aniFrames, 2, &entity->animator1, true, 0); break;
        case EGGPISTON_ALARM: RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator1, true, 0); break;
        default: break;
    }

    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void EggPistonsMKII_EditorLoad(void)
{
    EggPistonsMKII->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/EggPistonsMKII.bin", SCOPE_STAGE);
    EggPistonsMKII->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ1.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(EggPistonsMKII, type);
    RSDK_ENUM_VAR("Piston", EGGPISTON_PISTON);
    RSDK_ENUM_VAR("Control", EGGPISTON_CONTROL);
    RSDK_ENUM_VAR("Emitter", EGGPISTON_EMITTER);
    RSDK_ENUM_VAR("Barrier", EGGPISTON_BARRIER);
}
#endif

void EggPistonsMKII_Serialize(void)
{
    RSDK_EDITABLE_VAR(EggPistonsMKII, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(EggPistonsMKII, VAR_UINT8, type);
}
