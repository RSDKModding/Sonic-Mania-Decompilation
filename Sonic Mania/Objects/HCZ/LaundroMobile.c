#include "SonicMania.h"

ObjectLaundroMobile *LaundroMobile;

void LaundroMobile_Update(void)
{
    RSDK_THIS(LaundroMobile);
    StateMachine_Run(entity->state);
}

void LaundroMobile_LateUpdate(void) {}

void LaundroMobile_StaticUpdate(void)
{
    if (LaundroMobile->shouldPlayFanSfx) {
        if (!LaundroMobile->playingFanSfx) {
            if (LaundroMobile->health <= 8)
                RSDK.PlaySfx(LaundroMobile->sfxWash, true, 255);
            else
                RSDK.PlaySfx(LaundroMobile->sfxFan, 47208, 255);
            LaundroMobile->playingFanSfx = true;
        }
    }
    else {
        if (LaundroMobile->playingFanSfx) {
            RSDK.StopSFX(LaundroMobile->sfxFan);
            RSDK.StopSFX(LaundroMobile->sfxWash);
            LaundroMobile->playingFanSfx = false;
        }
    }

    LaundroMobile->shouldPlayFanSfx = false;
    if (RSDK_sceneInfo->state == ENGINESTATE_REGULAR && LaundroMobile->useStageWrap)
        LaundroMobile_HandleStageWrap();
}

void LaundroMobile_Draw(void)
{
    RSDK_THIS(LaundroMobile);
    StateMachine_Run(entity->stateDraw);
}

void LaundroMobile_Create(void *data)
{
    RSDK_THIS(LaundroMobile);

    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(entity);
    }
    else {
        entity->drawFX = FX_FLIP;
        if (!RSDK_sceneInfo->inEditor) {
            if (!entity->type)
                entity->type = voidToInt(data);

            switch (entity->type) {
                case LAUNDROMOBILE_BOSS:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = false;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 3, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &entity->animator3, true, 0);
                    entity->startY                    = entity->position.y;
                    entity->updateRange.x             = 0x800000;
                    entity->updateRange.y             = 0x800000;
                    LaundroMobile->health             = 5 + 8;
                    LaundroMobile->invincibilityTimer = 0;
                    LaundroMobile->loopPointCount     = 0;
                    LaundroMobile->attackDir          = FLIP_NONE;
                    LaundroMobile->currentVelocity    = 0x38000;
                    LaundroMobile->rocketActive       = false;
                    LaundroMobile->animSpeed          = 0;
                    LaundroMobile->travelledPaths     = 0;
                    LaundroMobile->useStageWrap       = false;
                    LaundroMobile->shouldPlayFanSfx   = 0;
                    LaundroMobile->playingFanSfx      = false;
                    RSDK.StopSFX(LaundroMobile->sfxFan);
                    RSDK.StopSFX(LaundroMobile->sfxWash);
                    LaundroMobile->rocketAngles[0] = 0;
                    LaundroMobile->rocketAngles[1] = 0x8000;
                    LaundroMobile->rocketAngles[2] = 0;
                    LaundroMobile->rocketAngles[3] = 0x8000;
                    LaundroMobile->rocketSpeeds[0] = 0;
                    LaundroMobile->rocketSpeeds[1] = 0;
                    LaundroMobile->underwaterFlag  = false;
                    entity->tileCollisions         = true;
                    entity->collisionLayers        = Zone->fgLayers;
                    entity->collisionPlane         = 0;
                    entity->state                  = LaundroMobile_State_SetupArena;
                    entity->stateDraw              = LaundroMobile_StateDraw_Unknown1;
                    break;
                case LAUNDROMOBILE_BOMB:
                    entity->active     = ACTIVE_BOUNDS;
                    entity->visible    = true;
                    entity->drawOrder  = Zone->drawOrderLow;
                    entity->startPos.x = entity->position.x;
                    entity->startPos.y = entity->position.y;
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 7, &entity->animator1, true, 0);
                    entity->updateRange.x = 0x1800000;
                    entity->updateRange.y = 0x1800000;
                    entity->state         = LaundroMobile_State1_Unknown1;
                    entity->stateDraw     = LaundroMobile_StateDraw4_Unknown1;
                    break;
                case LAUNDROMOBILE_PHASE2:
                    entity->active    = ACTIVE_XBOUNDS;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow + 1;
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 1, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 2, &entity->animator3, true, 0);
                    entity->updateRange.x = 0x2000000;
                    entity->updateRange.y = 0x800000;
                    entity->state         = LaundroMobile_State2_Unknown1;
                    entity->stateDraw     = LaundroMobile_StateDraw2_Unknown1;
                    break;
                case LAUNDROMOBILE_LOOPPOINT: LaundroMobile->loopPoints[LaundroMobile->loopPointCount++] = (Entity *)entity; break;
                case LAUNDROMOBILE_BLOCK:
                    entity->active     = ACTIVE_BOUNDS;
                    entity->visible    = true;
                    entity->drawOrder  = Zone->drawOrderLow;
                    entity->startPos.x = entity->position.x;
                    entity->startPos.y = entity->position.y;
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &entity->animator1, true, RSDK.Rand(0, 3));
                    entity->updateRange.x = 0x1800000;
                    entity->updateRange.y = 0x1800000;
                    entity->velocity.x    = LaundroMobile->currentVelocity - 0x18000;
                    entity->state         = LaundroMobile_State3_Unknown1;
                    entity->stateDraw     = LaundroMobile_StateDraw4_Unknown1;
                    break;
                case LAUNDROMOBILE_SPIKES:
                    entity->active     = ACTIVE_BOUNDS;
                    entity->visible    = true;
                    entity->drawOrder  = Zone->drawOrderLow;
                    entity->startPos.x = entity->position.x;
                    entity->startPos.y = entity->position.y;
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &entity->animator1, true, RSDK.Rand(0, 3) + 3);
                    entity->updateRange.x = 0x1800000;
                    entity->updateRange.y = 0x1800000;
                    entity->velocity.x    = LaundroMobile->currentVelocity - 0x18000;
                    entity->state         = LaundroMobile_State3_Unknown1;
                    entity->stateDraw     = LaundroMobile_StateDraw4_Unknown1;
                    break;
                case LAUNDROMOBILE_6:
                    entity->active = ACTIVE_NORMAL;
                    entity->timer  = 8;
                    entity->state  = LaundroMobile_State6_Unknown1;
                    break;
                default: break;
            }
        }
    }
}

void LaundroMobile_StageLoad(void)
{
    LaundroMobile->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/LaundroMobile.bin", SCOPE_STAGE);
    LaundroMobile->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ2.bin", SCOPE_STAGE);

    LaundroMobile->hitboxBoss.left   = -32;
    LaundroMobile->hitboxBoss.top    = -32;
    LaundroMobile->hitboxBoss.right  = 32;
    LaundroMobile->hitboxBoss.bottom = 32;

    LaundroMobile->hitboxMissile.left   = -24;
    LaundroMobile->hitboxMissile.top    = -24;
    LaundroMobile->hitboxMissile.right  = 24;
    LaundroMobile->hitboxMissile.bottom = 24;

    LaundroMobile->hitboxBox.left   = -16;
    LaundroMobile->hitboxBox.top    = -16;
    LaundroMobile->hitboxBox.right  = 16;
    LaundroMobile->hitboxBox.bottom = 16;

    LaundroMobile->hitboxBomb.left   = -32;
    LaundroMobile->hitboxBomb.top    = -16;
    LaundroMobile->hitboxBomb.right  = 32;
    LaundroMobile->hitboxBomb.bottom = 16;

    LaundroMobile->innerBox.left   = -39;
    LaundroMobile->innerBox.top    = -40;
    LaundroMobile->innerBox.right  = 39;
    LaundroMobile->innerBox.bottom = 40;

    LaundroMobile->outerBox.left   = -40;
    LaundroMobile->outerBox.top    = -40;
    LaundroMobile->outerBox.right  = 40;
    LaundroMobile->outerBox.bottom = 40;

    LaundroMobile->active           = ACTIVE_ALWAYS;
    LaundroMobile->playingFanSfx    = false;
    LaundroMobile->shouldPlayFanSfx = 0;
    LaundroMobile->sfxHit           = RSDK.GetSFX("Stage/BossHit.wav");
    LaundroMobile->sfxExplosion     = RSDK.GetSFX("Stage/Explosion2.wav");
    LaundroMobile->sfxButton2       = RSDK.GetSFX("Stage/Button2.wav");
    LaundroMobile->sfxFan           = RSDK.GetSFX("HCZ/BigFan.wav");
    LaundroMobile->sfxRoll          = RSDK.GetSFX("Global/Roll.wav");
    LaundroMobile->sfxWash          = RSDK.GetSFX("HCZ/Wash.wav");
    LaundroMobile->sfxHullClose     = RSDK.GetSFX("Stage/HullClose.wav");
    LaundroMobile->sfxPush          = RSDK.GetSFX("Stage/Push.wav");
    LaundroMobile->sfxFireball      = RSDK.GetSFX("Stage/Fireball.wav");
    LaundroMobile->sfxButton        = RSDK.GetSFX("Stage/Button.wav");
    LaundroMobile->sfxLedgeBreak    = RSDK.GetSFX("Stage/LedgeBreak3.wav");
    LaundroMobile->sfxPimPom        = RSDK.GetSFX("Stage/PimPom.wav");
#if RETRO_USE_PLUS
    LaundroMobile->sfxRumble = RSDK.GetSFX("Stage/Rumble.wav");
    LaundroMobile->sfxImpact = RSDK.GetSFX("Stage/Impact6.wav");
#endif
}

void LaundroMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(LaundroMobile);

    if (LaundroMobile->invincibilityTimer) {
        LaundroMobile->invincibilityTimer--;
    }
    else {
        int storeX = entity->position.x;
        int storeY = entity->position.y;
        foreach_active(Player, player)
        {
            for (int i = 0; i < 4 && LaundroMobile->rocketActive; ++i) {
                entity->position.x = LaundroMobile->rocketPositions[i].x;
                entity->position.y = LaundroMobile->rocketPositions[i].y;
                if (Player_CheckCollisionTouch(player, entity, &LaundroMobile->hitboxMissile)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(1024, player, 2, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, entity);
                    break;
                }
            }
            entity->position.x = storeX;
            entity->position.y = storeY;

            int velX = player->velocity.x;
            int velY = player->velocity.y;
            if (Player_CheckBadnikTouch(player, entity, &LaundroMobile->hitboxBoss) && Player_CheckBossHit(player, entity)) {
                if (LaundroMobile->health) {
                    LaundroMobile->health--;
                }

                if (LaundroMobile->health) {
                    if (LaundroMobile->health == 8) {
                        RSDK.StopSFX(LaundroMobile->sfxFan);
                        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 4, &entity->animator2, true, 0);
                        entity->timer = 60;
                        entity->state = LaundroMobile_State_Destroyed_Phase1;
                    }
                    else {
                        if (LaundroMobile->health < 8) {
                            RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 3, &entity->animator3, true, 0);
                        }
                        else {
                            RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 2, &entity->animator3, true, 0);
                            player->groundVel  = velX;
                            player->velocity.x = velX;
                            player->velocity.y = velY;
                        }
                        RSDK.PlaySfx(LaundroMobile->sfxHit, false, 255);
                        LaundroMobile->invincibilityTimer = 30;
                    }
                }
                else {
                    RSDK_sceneInfo->timeEnabled = false;
                    Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                    RSDK.PlaySfx(LaundroMobile->sfxExplosion, false, 255);
                    LaundroMobile->invincibilityTimer = 60;

                    EntityWhirlpool *whirlpool = (EntityWhirlpool *)entity->whirlpool;
                    entity->state              = LaundroMobile_State_Destroyed_Phase2;
                    if (whirlpool) {
                        if (whirlpool->objectID == Whirlpool->objectID)
                            whirlpool->activePlayers = -3;
                        entity->whirlpool = NULL;
                    }
                }
                foreach_break;
            }
        }
    }
}

void LaundroMobile_Explode(void)
{
    RSDK_THIS(LaundroMobile);

    int interval = LaundroMobile->health > 8 ? 7 : 3;

    if (!(Zone->timer % interval)) {
        RSDK.PlaySfx(LaundroMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int x                      = entity->position.x + (RSDK.Rand(-19, 20) << 16);
            int y                      = entity->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawOrder       = Zone->drawOrderHigh + 2;
            if (LaundroMobile->health > 8)
                explosion->velocity.x = 0x24000;
        }
    }
}

void LaundroMobile_HandleStageWrap(void)
{
    EntityLaundroMobile *entity = (EntityLaundroMobile *)LaundroMobile->bossPtr;

    if (!(Zone->timer & 3)) {
        EntityCurrent *current = CREATE_ENTITY(Current, intToVoid(2), RSDK_screens->position.x << 16,
                                               (8 * RSDK.Rand(0, RSDK_screens->height >> 3) + RSDK_screens->position.y) << 16);
        current->drawOrder     = Zone->playerDrawLow;
        current->strength      = 6;
        current->type          = 1;
        current->alpha         = 240;
        current->size.x        = (RSDK_screens->position.x + RSDK_screens->width + 0x1000) << 16;
    }

    foreach_active(Player, player)
    {
        if (player->state != Player_State_None) {
            if (player->position.x < entity->position.x)
                player->position.x = entity->position.x;
            if (Player_CheckValidState(player)) {
                if ((abs(player->velocity.y) <= 0x18000 || player->onGround) && player->position.x < 0x6D800000) {
                    player->collisionMode   = 0;
                    player->onGround        = false;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->state           = Current_Player_State_Type1;
                    if (player->playerAnimator.animationID != ANI_CLING && player->playerAnimator.animationID != ANI_SHAFTSWING) {
                        if (player->position.x >= entity->position.x + 0xC00000) {
                            player->velocity.x = LaundroMobile->currentVelocity;
                            player->groundVel  = player->velocity.x;
                        }
                        else {
                            player->velocity.x = LaundroMobile->currentVelocity + ((entity->position.x - player->position.x + 0xC00000) >> 6);
                            player->groundVel  = player->velocity.x;
                        }
                        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                    }
                    if (player->up)
                        player->velocity.y = -0x18000;
                    else
                        player->velocity.y = player->down ? 0x18000 : 0;
                }
            }
        }
    }

    if (LaundroMobile->currentVelocity < 0x80000)
        LaundroMobile->currentVelocity += 0x400;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (Player_CheckValidState(player1)) {
        if (LaundroMobile->loopPointCount == 5 && entity->timer <= 0) {
            if (LaundroMobile->currentVelocity < 0xC0000) {
                LaundroMobile->currentVelocity += 0x1000;
            }
        }
        else {
            Entity *loopPoint = LaundroMobile->loopPoints[LaundroMobile->loopPointCount];
            if (player1->position.x >= loopPoint->position.x + 0xE000000) {
                int startOffsetX = player1->position.x - loopPoint->position.x - 0xE000000;
                int startOffsetY = player1->position.y - loopPoint->position.y;
                if (LaundroMobile->health <= 8) {
                    LaundroMobile->loopPointCount = 5;
                    entity->state                 = LaundroMobile_State_Unknown5;
                }
                else {
                    if (LaundroMobile->travelledPaths == 0x1E) {
                        LaundroMobile->travelledPaths = 0;
                    }

                    int originalCount = LaundroMobile->loopPointCount;
                    while (LaundroMobile->loopPointCount == originalCount) {
                        do {
                            LaundroMobile->loopPointCount = RSDK.Rand(1, 5);
                        } while ((1 << LaundroMobile->loopPointCount) & LaundroMobile->travelledPaths);
                    }
                    LaundroMobile->travelledPaths |= 1 << LaundroMobile->loopPointCount;
                }
                loopPoint = LaundroMobile->loopPoints[LaundroMobile->loopPointCount];

                int offsetX = player1->position.x - startOffsetX - loopPoint->position.x;
                int offsetY = player1->position.y - loopPoint->position.y - startOffsetY;
                player1->position.x -= offsetX;
                player1->position.y -= offsetY;
                entity->position.y -= offsetY;
                entity->startPos.y -= offsetY;
                entity->unknownPos.y -= offsetY;
                entity->position.x -= offsetX;
                entity->startPos.x -= offsetX;
                entity->unknownPos.x -= offsetX;

                RSDK_screens->position.x -= offsetX >> 16;
                RSDK_screens->position.y -= offsetY >> 16;

                EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
                camera->position.x -= offsetX;
                camera->position.y -= offsetY;
                camera->center.x -= offsetX >> 16;
                camera->center.y -= offsetY >> 16;
                if (Player->playerCount >= 2) {
                    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    player2->position.x -= offsetX;
                    player2->position.y -= offsetY;
                }

                for (int i = 0; i < 0x1000; ++i) {
                    Entity *entPtr = RSDK.GetEntityByID(i);

                    if (entPtr->objectID == LaundroMobile->objectID) {
                        EntityLaundroMobile *laundroMobile = (EntityLaundroMobile *)entPtr;
                        if (laundroMobile->type == LAUNDROMOBILE_BOMB && laundroMobile->state != LaundroMobile_State1_Unknown1
                            || laundroMobile->state == LaundroMobile_State3_Unknown2) {
                            laundroMobile->position.x -= offsetX;
                            laundroMobile->position.y -= offsetY;
                        }
                    }
                    else if (entPtr->objectID == Ring->objectID) {
                        EntityRing *ring = (EntityRing *)entPtr;
                        if (ring->state != Ring_State_Normal) {
                            entPtr->position.x -= offsetX;
                            entPtr->position.y -= offsetY;
                        }
                    }
                    else if (entPtr->objectID == Debris->objectID) {
                        entPtr->position.x -= offsetX;
                        entPtr->position.y -= offsetY;
                    }
                    else if (entPtr->objectID == Water->objectID) {
                        EntityWater *water = (EntityWater *)entPtr;
                        if (water->type == WATER_BUBBLE) {
                            water->position.x -= offsetX;
                            water->position.y -= offsetY;
                            water->field_68 = water->position.x;
                        }
                    }
                    else if (entPtr->objectID == ImageTrail->objectID) {
                        EntityImageTrail *trail = (EntityImageTrail *)entPtr;
                        trail->position.x -= offsetX;
                        trail->position.y -= offsetY;
                        trail->currentPos.x -= offsetX;
                        trail->currentPos.y -= offsetY;
                        for (int i = 0; i < ImageTrail_TrackCount; ++i) {
                            trail->statePos[i].x -= offsetX;
                            trail->statePos[i].y -= offsetY;
                        }
                    }
                    else if (entPtr->objectID == Current->objectID) {
                        EntityCurrent *current = (EntityCurrent *)entPtr;
                        if (current->state == Current_State_LaundoMobile) {
                            current->position.x -= offsetX;
                            current->position.y -= offsetY;
                            current->size.x -= offsetX;
                        }
                    }
                }
            }
        }
    }
}

void LaundroMobile_HandleMissileMovement(void)
{
    RSDK_THIS(LaundroMobile);

    for (int i = 0; i < 2; ++i) {
        LaundroMobile->rocketPositions[i].x = entity->position.x + 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[i] >> 8);
        LaundroMobile->rocketPositions[i].y = entity->position.y + 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[i] >> 8);
        LaundroMobile->rocketAngles[i] += LaundroMobile->rocketSpeeds[0];
    }

    for (int i = 2; i < 4; ++i) {
        LaundroMobile->rocketPositions[i].x = entity->position.x - 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[i] >> 8);
        LaundroMobile->rocketPositions[i].y = entity->position.y + 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[i] >> 8);
        LaundroMobile->rocketAngles[i] += LaundroMobile->rocketSpeeds[1];
    }
}

void LaundroMobile_HandleAnimations(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.ProcessAnimation(&entity->animator3);
    bool32 flag = false;

    switch (entity->animator3.animationID) {
        case 0: {
            foreach_active(Player, player)
            {
                if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown)
                    flag = true;
            }
            if (flag)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 4, &entity->animator3, true, 0);
            break;
        }
        case 1: {
            foreach_active(Player, player)
            {
                if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown)
                    flag = true;
            }
            if (flag)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 5, &entity->animator3, true, 0);
            break;
        }
        case 2:
            if (!LaundroMobile->invincibilityTimer)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &entity->animator3, true, 0);
            break;
        case 3:
            if (!LaundroMobile->invincibilityTimer)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 1, &entity->animator3, true, 0);
            break;
        case 4: {
            foreach_active(Player, player)
            {
                if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown)
                    flag = true;
            }

            if (entity->animator3.frameID >= entity->animator3.frameCount - 1) {
                if (flag)
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 4, &entity->animator3, true, 6);
                else
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &entity->animator3, true, 0);
            }
            break;
        }
        case 5: {
            foreach_active(Player, player)
            {
                if (player->state == Player_State_Hit || player->state == Player_State_Die || player->state == Player_State_Drown)
                    flag = true;
            }

            if (entity->animator3.frameID >= entity->animator3.frameCount - 1) {
                if (flag)
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 5, &entity->animator3, true, 6);
                else
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 1, &entity->animator3, true, 0);
            }
            break;
        }
        default: break;
    }
}

void LaundroMobile_HandleTileCollisions(void)
{
    RSDK_THIS(LaundroMobile);

    uint8 flag = 0xFF;
    while (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0x200000, 0x200000, false) && flag >= 0) {
        entity->position.y -= 0x10000;
        flag = 0;
    }

    while (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0x200000, -0x200000, false) && flag >= 1) {
        entity->position.y += 0x10000;
        flag = 1;
    }

    while (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0x200000, 0x200000, false) && flag >= 2) {
        entity->position.y -= 0x10000;
        flag = 2;
    }

    while (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, -0x200000, -0x200000, false) && flag >= 3) {
        entity->position.y += 0x10000;
        flag = 3;
    }
}

void LaundroMobile_State_SetupArena(void)
{
    RSDK_THIS(LaundroMobile);

    if (++entity->timer >= 2) {
        entity->position.y += 0x500000;
        entity->startPos.y = entity->position.y;
        entity->position.x -= 0x100000;
        entity->timer  = 0;
        entity->active = ACTIVE_NORMAL;
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (entity->position.x >= 0x60800000) {
            // did this used to only have 12 health during mania dev?
            if (LaundroMobile->health == RETRO_USE_PLUS ? (5 + 8) : (4 + 8)) {
                LaundroMobile->health = 8;
                Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
                entity->visible = true;
                entity->state   = LaundroMobile_State_Unknown6;
            }
            else {
                destroyEntity(entity);
            }
        }
        else {
            LaundroMobile->bossPtr                                                  = (Entity *)entity;
            entity->state                                                           = LaundroMobile_State_SetupArena2;
            RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
            EntityPlayer *player1                                                   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->jumpPress                                                      = false;
        }
    }
}

void LaundroMobile_State_SetupArena2(void)
{
    RSDK_THIS(LaundroMobile);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
    player1->jumpPress                                                      = false;
    if (player1->position.x > entity->position.x + 0x1700000) {
        Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
        RSDK.GetSceneLayer(4)->drawLayer[0] = DRAWLAYER_COUNT;
        LaundroMobile->loopPointCount       = 0;
        Water->waterLevel                   = 0;
        Water->targetWaterLevel             = 0;
        entity->timer                       = 120;
        entity->animator2.animationSpeed    = 0;
        entity->visible                     = true;
        entity->state                       = LaundroMobile_State_Unknown1;
    }
}

void LaundroMobile_State_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);
    entity->position.x += 0x18000;
    RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
    RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->jumpPress                        = false;
    if (!--entity->timer) {
        RSDK.PlaySfx(LaundroMobile->sfxButton2, false, 255);
        entity->state               = LaundroMobile_State_Unknown2;
        LaundroMobile->useStageWrap = true;
    }
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Unknown2(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
    RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->jumpPress                        = false;
    RSDK.ProcessAnimation(&entity->animator2);

    if (entity->animator2.animationSpeed >= 0x200) {
        ++LaundroMobile->shouldPlayFanSfx;
        RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 240;
        entity->state                                                           = LaundroMobile_State_Unknown3;
    }
    else {
        entity->animator2.animationSpeed += 4;
        if (entity->animator2.animationSpeed >= 0x20)
            ++LaundroMobile->shouldPlayFanSfx;
    }
    entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Unknown3(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_HandleAnimations();
    RSDK.ProcessAnimation(&entity->animator2);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    entity->position.y = entity->startPos.y;

    if (abs(entity->startPos.y - player1->position.y) >= 0x100000) {
        if (player1->position.y >= entity->startPos.y) {
            if (entity->velocity.y < 0x18000)
                entity->velocity.y += 0x1000;
        }
        else {
            if (entity->velocity.y > -0x18000)
                entity->velocity.y -= 0x1000;
        }
    }
    else if (entity->velocity.y) {
        if (entity->velocity.y <= 0) {
            entity->velocity.y += 0x1000;
            if (entity->velocity.y > 0)
                entity->velocity.y = 0;
        }
        else {
            entity->velocity.y -= 0x1000;
            if (entity->velocity.y < 0)
                entity->velocity.y = 0;
        }
    }

    entity->position.y += entity->velocity.y;
    LaundroMobile_HandleTileCollisions();

    if (entity->position.y == entity->startPos.y) {
        entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);
    }
    else {
        entity->startPos.y = entity->position.y;
    }
    entity->position.x += LaundroMobile->currentVelocity;
    ++LaundroMobile->shouldPlayFanSfx;
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Destroyed_Phase1(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_Explode();
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.y = entity->startPos.y;
    LaundroMobile_HandleTileCollisions();

    if (entity->position.y == entity->startPos.y) {
        entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);
    }
    else {
        entity->startPos.y = entity->position.y;
    }

    entity->position.x += LaundroMobile->currentVelocity - 0x8000;

    LaundroMobile_CheckPlayerCollisions();
    if (--entity->timer <= 0) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 4, &debris->animator, true, 0);
        debris->velocity.x    = 0x50000;
        debris->velocity.y    = -0x28000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = LaundroMobile_State_Unknown4;
    }
}

void LaundroMobile_State_Unknown4(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_Explode();
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.y = entity->startPos.y;
    LaundroMobile_HandleTileCollisions();
    if (entity->position.y == entity->startPos.y) {
        entity->position.y = BadnikHelpers_Oscillate(entity->startPos.y, 2, 10);
    }
    else {
        entity->startPos.y = entity->position.y;
    }
    entity->position.x += LaundroMobile->currentVelocity - 0x10000;
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Unknown5(void)
{
    RSDK_THIS(LaundroMobile);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > 0x6D800000) {
        LaundroMobile->useStageWrap = false;
        foreach_active(Player, player) { player->state = Player_State_Air; }
        entity->state = LaundroMobile_State_Unknown6;
    }
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Unknown6(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->onGround && !player1->angle) {
        entity->timer = 60;
        RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 1, &entity->animator3, true, 0);
        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 5, &entity->animator2, true, 0);
        entity->state = LaundroMobile_State_Unknown7;
    }
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Unknown7(void)
{
    RSDK_THIS(LaundroMobile);

    if (--entity->timer <= 0) {
        entity->position.x = 0x6E000000;
        entity->timer      = 104;
        entity->position.y = (RSDK_screens->position.y - 64) << 16;
        LaundroMobile_HandleMissileMovement();
        entity->stateDraw = LaundroMobile_StateDraw_Unknown2;
        entity->state     = LaundroMobile_State_Unknown8;
    }
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_State_Unknown8(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.y += 0x10000;
    LaundroMobile_HandleMissileMovement();
    if (--entity->timer <= 0) {
        entity->timer = 32;
        entity->state = LaundroMobile_State_Unknown9;
    }
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown9(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_HandleMissileMovement();

    if (LaundroMobile->rocketSpeeds[0] < 0x200)
        LaundroMobile->rocketSpeeds[0] += 8;

    if (entity->timer <= 0) {
        if (LaundroMobile->rocketSpeeds[1] < 0x200)
            LaundroMobile->rocketSpeeds[1] += 8;
    }
    else {
        entity->timer--;
    }

    if (LaundroMobile->rocketSpeeds[0] + LaundroMobile->rocketSpeeds[1] == 0x400) {
        entity->timer = 96;
        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 6, &entity->animator4, true, 0);
        LaundroMobile->rocketActive = true;
        RSDK.PlaySfx(LaundroMobile->sfxPush, false, 255);
        entity->state = LaundroMobile_State_Unknown11;
    }
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown11(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_HandleMissileMovement();

    if (LaundroMobile->rocketSpeeds[0] < 0x400) {
        LaundroMobile->rocketSpeeds[1] += 8;
        LaundroMobile->rocketSpeeds[0] = LaundroMobile->rocketSpeeds[1];
    }
    if (--entity->timer <= 0) {
        entity->timer = 60;
        RSDK.PlaySfx(LaundroMobile->sfxFireball, false, 255);
        entity->state = LaundroMobile_State_Unknown12;
    }
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown12(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;
    LaundroMobile_HandleMissileMovement();
    if (--entity->timer <= 0) {
        if (LaundroMobile->attackDir)
            entity->position.x += 0x580000;
        else
            entity->position.x -= 0x580000;
        entity->velocity.y = 0x50000;
        RSDK.PlaySfx(LaundroMobile->sfxRoll, false, 255);
        LaundroMobile->attackDelay = LaundroMobile->attackDelays[RSDK.Rand(0, 8)];
        entity->state              = LaundroMobile_State_Unknown13;
    }
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown13(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.ProcessTileCollisions(entity, &LaundroMobile->outerBox, &LaundroMobile->innerBox);

    if (entity->velocity.y < 0 && entity->position.y < (RSDK_screens->position.y - 256) << 16) {
        entity->onGround   = false;
        entity->velocity.y = 0x50000;
        LaundroMobile->attackDir ^= FLIP_X;
        RSDK.PlaySfx(LaundroMobile->sfxRoll, false, 255);
    }

    if (entity->position.y <= Water->waterLevel) {
        if (LaundroMobile->underwaterFlag) {
            LaundroMobile->underwaterFlag = false;
            CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
            CREATE_ENTITY(LaundroMobile, intToVoid(LAUNDROMOBILE_6), entity->position.x - 0x100000, Water->waterLevel);
            CREATE_ENTITY(LaundroMobile, intToVoid(LAUNDROMOBILE_6), entity->position.x + 0x100000, Water->waterLevel);
            RSDK.PlaySfx(Water->sfx_Splash, false, 255);
            if (--LaundroMobile->attackDelay <= 0) {
                entity->onGround = false;
                entity->state    = LaundroMobile_State_Unknown14;
                if (LaundroMobile->attackDir == FLIP_NONE)
                    entity->velocity.x = -0x18000;
                else
                    entity->velocity.x = 0x18000;
            }
        }
    }
    else if (!LaundroMobile->underwaterFlag) {
        LaundroMobile->underwaterFlag = true;
        CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
        CREATE_ENTITY(LaundroMobile, intToVoid(LAUNDROMOBILE_6), entity->position.x - 0x100000, Water->waterLevel);
        CREATE_ENTITY(LaundroMobile, intToVoid(LAUNDROMOBILE_6), entity->position.x + 0x100000, Water->waterLevel);
        RSDK.PlaySfx(Water->sfx_Splash, false, 255);
    }
    LaundroMobile_HandleMissileMovement();
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown14(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;
    if ((LaundroMobile->attackDir && entity->position.x >= 0x6E000000) || (!LaundroMobile->attackDir && entity->position.x <= 0x6E000000)) {
        entity->position.x = 0x6E000000;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
        LaundroMobile->rocketActive = false;
        RSDK.PlaySfx(LaundroMobile->sfxHullClose, false, 255);
        entity->state = LaundroMobile_State_Unknown15;
    }

    LaundroMobile_HandleMissileMovement();
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown15(void)
{
    RSDK_THIS(LaundroMobile);

    if (LaundroMobile->rocketSpeeds[0] <= 0x180) {
        if (!(LaundroMobile->rocketAngles[0] & 0x7E00)) {
            LaundroMobile->rocketSpeeds[0] = 0;
            LaundroMobile->rocketAngles[0] &= 0xFE00;
            LaundroMobile->rocketAngles[1] &= 0xFE00;
        }
    }
    else {
        LaundroMobile->rocketSpeeds[0] -= 8;
    }

    if (LaundroMobile->rocketSpeeds[1] <= 0x180) {
        if (!(LaundroMobile->rocketAngles[2] & 0x7E00)) {
            LaundroMobile->rocketSpeeds[1] = 0;
            LaundroMobile->rocketAngles[2] &= 0xFE00;
            LaundroMobile->rocketAngles[3] &= 0xFE00;
        }
    }
    else {
        LaundroMobile->rocketSpeeds[1] -= 8;
    }

    if (!(LaundroMobile->rocketSpeeds[0] + LaundroMobile->rocketSpeeds[1])) {
        RSDK.PlaySfx(LaundroMobile->sfxFan, false, 255);
        entity->state = LaundroMobile_State_Unknown16;
    }
    LaundroMobile_HandleMissileMovement();
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown16(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile->animSpeed += 4;
    if (LaundroMobile->animSpeed < 256) {
        LaundroMobile_CheckPlayerCollisions();
        LaundroMobile_HandleAnimations();
    }
    else {
        EntityWhirlpool *whirlpool = (EntityWhirlpool *)entity->whirlpool;
        if (whirlpool) {
            if (whirlpool->objectID == Whirlpool->objectID) {
                whirlpool->position.x = entity->position.x;
            }
            else {
                entity->whirlpool = 0;
            }
            entity->timer = 480;
            entity->state = LaundroMobile_State_Unknown17;
        }
        else {
            Vector2 pos;
            pos.x                    = 0x1000000;
            pos.y                    = 0x1000000;
            whirlpool                = CREATE_ENTITY(Whirlpool, &pos, entity->position.x, entity->startPos.y - 0x200000);
            whirlpool->activePlayers = -1;
            whirlpool->angVel        = 10;
            whirlpool->alpha         = 0;
            whirlpool->drawOrder     = Zone->drawOrderLow + 1;
            whirlpool->isPermanent   = true;
            entity->whirlpool        = (Entity *)whirlpool;
            entity->timer            = 480;
            entity->state            = LaundroMobile_State_Unknown17;
        }
    }
}

void LaundroMobile_State_Unknown17(void)
{
    RSDK_THIS(LaundroMobile);

    ++LaundroMobile->shouldPlayFanSfx;
    --entity->timer;
    if (entity->timer > 0) {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->position.y > Water->waterLevel) {
                if (player->state == Player_State_None) {
                    player->onGround        = false;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;

                    if ((player->position.y & 0xFFFF0000) != (entity->position.y & 0xFFFF0000) + 0x580000) {
                        if (player->position.y >= entity->position.y + 0x580000)
                            player->position.y -= 0x10000;
                        else
                            player->position.y += 0x10000;
                    }

                    if (LaundroMobile->playerRadius[playerID] != 0x5000) {
                        if (LaundroMobile->playerRadius[playerID] >= 0x5000)
                            LaundroMobile->playerRadius[playerID] -= 0x100;
                        else
                            LaundroMobile->playerRadius[playerID] += 0x100;
                    }
                    if ((LaundroMobile->playerAngles[playerID] & 0xFF) >= 0x80)
                        player->drawOrder = Zone->playerDrawLow;
                    else
                        player->drawOrder = entity->drawOrder - 1;
                    LaundroMobile->playerAngles[playerID] += 3;
                    player->position.x =
                        LaundroMobile->playerRadius[playerID] * RSDK.Cos256(LaundroMobile->playerAngles[playerID]) + entity->position.x;
                }
                else if (Player_CheckValidState(player)) {
                    player->onGround = false;
                    ;
                    player->velocity.x                    = 0;
                    player->velocity.y                    = 0;
                    player->groundVel                     = 0;
                    player->nextGroundState               = StateMachine_None;
                    player->nextAirState                  = StateMachine_None;
                    LaundroMobile->playerRadius[playerID] = ((player->position.x - entity->position.x) & 0xFFFF0000);

                    if (LaundroMobile->playerRadius[playerID] <= 0) {
                        LaundroMobile->playerRadius[playerID] = -LaundroMobile->playerRadius[playerID];
                        LaundroMobile->playerAngles[playerID] = 128;
                    }
                    else {
                        LaundroMobile->playerAngles[playerID] = 0;
                    }
                    LaundroMobile->playerRadius[playerID] >>= 8;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                    player->state = Player_State_None;
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            int playerID = RSDK.GetEntityID(player);
            if (player->state == Player_State_None) {
                player->velocity.x = player->position.x
                                     - LaundroMobile->playerRadius[playerID] * RSDK.Cos256(LaundroMobile->playerAngles[playerID] - 3)
                                     - entity->position.x;
                player->drawOrder = Zone->playerDrawLow;
                player->state     = Player_State_Air;
            }
        }

        EntityWhirlpool *whirlpool = (EntityWhirlpool *)entity->whirlpool;
        if (whirlpool) {
            if (whirlpool->objectID == Whirlpool->objectID)
                whirlpool->activePlayers = -3;
            entity->whirlpool = NULL;
        }
        entity->state = LaundroMobile_State_Unknown18;
    }
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Unknown18(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile->animSpeed -= 4;
    if (LaundroMobile->animSpeed <= 0) {
        entity->velocity.y = 0;
        entity->timer      = 32;
        entity->state      = LaundroMobile_State_Unknown9;
    }
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleAnimations();
}

void LaundroMobile_State_Destroyed_Phase2(void)
{
    RSDK_THIS(LaundroMobile);

    foreach_active(Player, player)
    {
        int playerID = RSDK.GetEntityID(player);
        if (player->state == Player_State_None) {
            player->velocity.x = player->position.x - LaundroMobile->playerRadius[playerID] * RSDK.Cos256(LaundroMobile->playerAngles[playerID] - 3)
                                 - entity->position.x;
            player->drawOrder = Zone->playerDrawLow;
            player->state     = Player_State_Air;
        }
    }

    LaundroMobile_Explode();

    if (LaundroMobile->animSpeed > 0)
        LaundroMobile->animSpeed -= 4;

    if (--LaundroMobile->invincibilityTimer) {
        if (LaundroMobile->invincibilityTimer == 30) {
            for (int i = 0; i < 4; ++i) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, LaundroMobile->rocketPositions[i].x, LaundroMobile->rocketPositions[i].y);
                RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 5, &debris->animator, true, (LaundroMobile->rocketAngles[i] >> 12) & 0xF);
                debris->animator.animationSpeed = 0;

                if (debris->position.y >= entity->position.y) {
                    if (debris->position.x < entity->position.x)
                        debris->velocity.x = -0x20000;
                    else
                        debris->velocity.x = 0x20000;
                    debris->velocity.y = -0x10000;
                }
                else {
                    if (debris->position.x < entity->position.x)
                        debris->velocity.x = -0x40000;
                    else
                        debris->velocity.x = 0x40000;
                    debris->velocity.y = -0x20000;
                }
                debris->gravity                     = 0x3800;
                debris->drawOrder                   = Zone->drawOrderHigh;
                debris->updateRange.x               = 0x400000;
                debris->updateRange.y               = 0x400000;
                LaundroMobile->rocketPositions[i].x = 0;
                LaundroMobile->rocketPositions[i].y = 0;
            }
        }
    }
    else {
        entity->timer      = 60;
        entity->velocity.y = 0x20000;
        entity->state      = LaundroMobile_State_Finish;
    }
}

void LaundroMobile_State_Finish(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;
    LaundroMobile_Explode();

    if (--entity->timer <= 0) {
        Water->field_C8 = true;
        Water->targetWaterLevel += 0x780000;
        Water->waterMoveSpeed = 0x10000;
        entity->timer         = 0;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), (RSDK_screens->position.x + RSDK_screens->centerX) << 16,
                      (RSDK_screens->position.y - 48) << 16);
#if RETRO_USE_PLUS
        Zone->stageFinishCallback = LaundroMobile_StageFinishCB_Blank;
        entity->state             = LaundroMobile_State_StartOutro;
#else
        destroyEntity(entity);
#endif
    }
}

#if RETRO_USE_PLUS
void LaundroMobile_StageFinishCB_Blank(void) {}

void LaundroMobile_State_StartOutro(void)
{
    RSDK_THIS(LaundroMobile);

    EntityActClear *actClear = RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear);

    if (entity->timer) {
        if (actClear->objectID != ActClear->objectID) {
            entity->timer = 0;

            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->drawOrder    = Zone->playerDrawLow;
            player1->state        = Player_State_Ground;
            player1->direction    = FLIP_NONE;
            player1->stateInput   = StateMachine_None;
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);

            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            if (player2->objectID == Player->objectID) {
                player2->drawOrder  = Zone->playerDrawLow;
                player2->state      = Player_State_Ground;
                player2->direction  = FLIP_NONE;
                player2->stateInput = StateMachine_None;
                RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
            }
            entity->state = LaundroMobile_State_OutroRumble;
        }
    }
    else if (actClear->objectID == ActClear->objectID) {
        entity->timer = 1;
    }
}

void LaundroMobile_State_OutroRumble(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    player1->timer = 0;
    if (player2->objectID == Player->objectID)
        player2->timer = 0;

    if (!(Zone->timer & 3)) {
        Camera_ShakeScreen(0, 0, 2);
    }

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(LaundroMobile->sfxRumble, false, 255);

    if (++entity->timer == 90) {
        entity->timer = 0;
        foreach_active(WaterGush, gush)
        {
            if (gush->position.x > entity->position.x) {
                gush->flag      = true;
                gush->inkEffect = INK_ALPHA;
                gush->alpha     = 256;
                gush->drawOrder = Zone->playerDrawLow;
            }
        }

        for (int i = 0; i < 0x20; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, 0x6EB00000, 0xAE00000);
            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
            debris->position.x += 0x60000 * RSDK.Rand(-8, 8);
            debris->position.y += 0x60000 * RSDK.Rand(-8, 8);
            debris->velocity.x = RSDK.Rand(-8, 8) << 16;
            debris->velocity.y = RSDK.Rand(-8, 8) << 16;
            debris->velocity.x = RSDK.Rand(-8, 9) << 15;
            debris->velocity.y = RSDK.Rand(-8, 5) << 16;
            debris->direction  = RSDK.Rand(0, 4);
            debris->drawFX     = FX_FLIP;
            debris->drawOrder  = Zone->drawOrderHigh;
            debris->gravity    = 0x3800;
        }

        RSDK.CopyTileLayer(Zone->fgHigh, 1763, 172, Zone->fgHigh, 1919, 172, 11, 4);
        RSDK.PlaySfx(LaundroMobile->sfxImpact, false, 255);
        RSDK.PlaySfx(WaterGush->sfxGush, false, 255);
        RSDK.PlaySfx(Water->sfx_Splash, false, 255);
        entity->state = LaundroMobile_State_OutroWaterGush;
    }
}

void LaundroMobile_State_OutroWaterGush(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    player1->timer = 0;
    if (player2->objectID == Player->objectID)
        player2->timer = 0;

    if (++entity->timer < 75) {
        foreach_active(WaterGush, gush)
        {
            gush->position.y += 0x8000;
            gush->alpha -= 2;
            if (gush->position.x > entity->position.x) {
                if (gush->position.x - gush->field_78 < player1->position.x) {
                    player1->velocity.x = -0x30000;
                    player1->groundVel  = -0x30000;
                }

                if (player2->objectID == Player->objectID && gush->position.x - gush->field_78 < player2->position.x) {
                    player2->velocity.x = -0x30000;
                    player2->groundVel  = -0x30000;
                }
            }
        }
    }
    else {
        foreach_active(WaterGush, gush)
        {
            gush->position.y += 0x8000;
            gush->alpha -= 2;
            if (gush->position.x > entity->position.x && gush->alpha <= 0) {
                destroyEntity(gush);
                for (int p = 0; p < PLAYER_MAX; ++p) Zone->playerBoundActiveR[p] = false;
                entity->timer = 0;
                entity->state = LaundroMobile_State_ExitHCZ;
            }
        }
    }
}

void LaundroMobile_State_ExitHCZ(void)
{
    RSDK_THIS(LaundroMobile);

    if (++entity->timer > 120) {
        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->right = true;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->objectID == Player->objectID)
            player1->right = true;
        if (player1->position.x > (Zone->screenBoundsR1[0] + 64) << 16) {
            HCZSetup_StageFinishCB_Act2();
            destroyEntity(entity);
        }
    }
}
#endif

void LaundroMobile_StateDraw_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);
    Vector2 drawPos;

    if (LaundroMobile->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
        RSDK.SetPaletteEntry(1, 128, 0xE0E0E0);
    }
    drawPos.x = entity->position.x - 0x20000;
    drawPos.y = entity->position.y - 0x1B0000;

    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y += 0x300000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos = entity->position;
    drawPos.x += 0x100000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    drawPos.x                 = entity->position.x - 0x120000;
    drawPos.y                 = entity->position.y - 0x1B0000;
    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y += 0x300000;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
    RSDK.SetPaletteEntry(1, 128, 0x000000);
}

void LaundroMobile_StateDraw_Unknown2(void)
{
    RSDK_THIS(LaundroMobile);

    if (LaundroMobile->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
        RSDK.SetPaletteEntry(1, 128, 0xE0E0E0);
    }

    for (int p = 0; p < 4; ++p) {
        entity->direction = p >= 2 ? FLIP_X : FLIP_NONE;
        uint8 angle       = LaundroMobile->rocketAngles[p] >> 8;
        if (angle >= 0x80) {
            entity->animator2.frameID = angle >> 4;

            entity->animator4.frameID = 12;
            if (!(Zone->timer & 1))
                entity->animator4.frameID = entity->animator2.frameID;

            RSDK.DrawSprite(&entity->animator2, &LaundroMobile->rocketPositions[p], false);
            RSDK.DrawSprite(&entity->animator4, &LaundroMobile->rocketPositions[p], false);
        }
    }

    entity->direction         = FLIP_NONE;
    entity->animator1.frameID = 5;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    for (int p = 0; p < 4; ++p) {
        entity->direction = p >= 2 ? FLIP_X : FLIP_NONE;
        uint8 angle       = LaundroMobile->rocketAngles[p] >> 8;
        if (angle < 0x80) {
            entity->animator2.frameID = angle >> 4;

            entity->animator4.frameID = 12;
            if (!(Zone->timer & 1))
                entity->animator4.frameID = entity->animator2.frameID;

            angle = (angle + 64) & 0xFF;
            if (angle <= 0x80)
                RSDK.DrawSprite(&entity->animator4, &LaundroMobile->rocketPositions[p], false);

            RSDK.DrawSprite(&entity->animator2, &LaundroMobile->rocketPositions[p], false);

            if (angle > 0x80)
                RSDK.DrawSprite(&entity->animator4, &LaundroMobile->rocketPositions[p], false);
        }
    }

    entity->direction = FLIP_NONE;
    RSDK.SetPaletteEntry(0, 128, 0x000000);
    RSDK.SetPaletteEntry(1, 128, 0x000000);
}

void LaundroMobile_State1_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);

    if (RSDK_sceneInfo->entity->active == ACTIVE_BOUNDS) {
        if (entity->position.x + 0x200000 > RSDK_screens->position.x << 16) {
            if (entity->position.x - 0x200000 <= (RSDK_screens->position.x + RSDK_screens->width) << 16) {
                entity->visible            = false;
                EntityLaundroMobile *child = CREATE_ENTITY(LaundroMobile, intToVoid(LAUNDROMOBILE_BOMB), entity->position.x, entity->position.y);
                child->velocity.x          = LaundroMobile->currentVelocity - 0x20000;
                child->active              = ACTIVE_NORMAL;
                child->state               = LaundroMobile_State1_Unknown2;
                child->isPermanent         = true;
                entity->active             = ACTIVE_NORMAL;
            }
        }
    }
    else {
        if (entity->position.x + 0x200000 < RSDK_screens->position.x << 16) {
            entity->visible = true;
            entity->active  = ACTIVE_BOUNDS;
        }
    }
}

void LaundroMobile_State1_Unknown2(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.x += entity->velocity.x;

    foreach_active(Player, player)
    {
        int velX = player->velocity.x;
        if (Player_CheckCollisionBox(player, entity, &LaundroMobile->hitboxBox) == C_LEFT) {
            RSDK.PlaySfx(LaundroMobile->sfxButton, false, 255);
            entity->velocity.x = LaundroMobile->currentVelocity + 0x18000;
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 8, &entity->animator1, true, 0);
            entity->state = LaundroMobile_State1_Unknown3;
        }
        player->velocity.x = velX;
    }

    if (entity->position.x + 0x200000 < RSDK_screens->position.x << 16) {
        destroyEntity(entity);
    }
    else {
        foreach_active(LaundroMobile, boss)
        {
            if (boss->type == LAUNDROMOBILE_BOSS
                && RSDK.CheckObjectCollisionTouchBox(boss, &LaundroMobile->hitboxBoss, entity, &LaundroMobile->hitboxBox)
                && boss->state == LaundroMobile_State_Unknown4) {
                RSDK.PlaySfx(LaundroMobile->sfxPimPom, false, 255);

                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 7, &debris->animator, true, 0);
                debris->velocity.y    = -0x28000;
                debris->velocity.x    = LaundroMobile->currentVelocity + 0x28000;
                debris->gravity       = 0x3800;
                debris->drawOrder     = Zone->drawOrderHigh;
                debris->updateRange.x = 0x400000;
                debris->updateRange.y = 0x400000;
                destroyEntity(entity);
                foreach_break;
            }
        }
    }
}

void LaundroMobile_State1_Unknown3(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.x += entity->velocity.x;
    entity->velocity.x -= 0x800;
    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->position.x + 0x200000 < RSDK_screens->position.x << 16) {
        destroyEntity(entity);
    }
    else {
        foreach_active(Player, player)
        {
            int velX = player->velocity.x;
            Player_CheckCollisionBox(player, entity, &LaundroMobile->hitboxBox);
            player->velocity.x = velX;
        }

        EntityLaundroMobile *boss = (EntityLaundroMobile *)LaundroMobile->bossPtr;
        if (RSDK.CheckObjectCollisionTouchBox(boss, &LaundroMobile->hitboxBoss, entity, &LaundroMobile->hitboxBox)) {
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y);
            explosion->drawOrder       = Zone->drawOrderHigh;
            explosion->velocity.x      = LaundroMobile->currentVelocity - 0x10000;
            RSDK.PlaySfx(LaundroMobile->sfxExplosion, false, 255);

            EntityWater *water = CREATE_ENTITY(Water, intToVoid(WATER_BUBBLE), entity->position.x, entity->position.y);
            water->drawOrder   = Zone->drawOrderLow + 1;
            water->angle       = 2 * RSDK.Rand(0, 256);
            water->speed       = -0x1400;
            water->velocity.x  = (LaundroMobile->currentVelocity + (LaundroMobile->currentVelocity >> 2)) + (LaundroMobile->currentVelocity >> 1);
            water->field_68    = water->position.x;
            water->childPtr    = 0;
            RSDK.SetSpriteAnimation(Water->spriteIndex, 3, &water->animator, false, 5);
            water->tileCollisions = true;

            if (!LaundroMobile->invincibilityTimer) {
                --LaundroMobile->health;
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 2, &boss->animator3, true, 0);
                RSDK.PlaySfx(LaundroMobile->sfxHit, false, 255);
                LaundroMobile->invincibilityTimer = 30;
                if (LaundroMobile->health <= 8) {
                    RSDK.StopSFX(LaundroMobile->sfxFan);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 4, &boss->animator2, true, 0);
                    boss->timer = 60;
                    boss->state = LaundroMobile_State_Destroyed_Phase1;
                }
            }
            destroyEntity(entity);
        }
    }
}

void LaundroMobile_StateDraw4_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void LaundroMobile_State3_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);

    if (entity->active == ACTIVE_BOUNDS) {
        if (entity->position.x + 0x200000 > RSDK_screens->position.x << 16) {
            if (entity->position.x - 0x200000 <= (RSDK_screens->position.x + RSDK_screens->width) << 16) {
                entity->visible            = false;
                EntityLaundroMobile *child = CREATE_ENTITY(LaundroMobile, intToVoid(entity->type), entity->position.x, entity->position.y);
                child->velocity.x          = LaundroMobile->currentVelocity - 0x20000;
                child->active              = ACTIVE_NORMAL;
                child->state               = LaundroMobile_State3_Unknown2;
                child->isPermanent         = true;
                entity->active             = ACTIVE_NORMAL;
            }
        }
    }
    else {
        if (entity->position.x + 0x200000 < RSDK_screens->position.x << 16) {
            entity->visible = true;
            entity->active  = ACTIVE_BOUNDS;
        }
    }
}

void LaundroMobile_State3_Unknown2(void)
{
    RSDK_THIS(LaundroMobile);

    entity->position.x += entity->velocity.x;

    foreach_active(Player, player)
    {
        if (entity->type == LAUNDROMOBILE_SPIKES) {
            if (Player_CheckCollisionBox(player, entity, &LaundroMobile->hitboxBomb) == C_LEFT) {
                Player_CheckHit(player, entity);
            }
        }
        else {
            Player_CheckCollisionBox(player, entity, &LaundroMobile->hitboxBox);
        }
    }

    if (entity->position.x + 0x200000 >= RSDK_screens->position.x << 16) {
        foreach_active(LaundroMobile, laundroMobile)
        {
            if (laundroMobile->type == LAUNDROMOBILE_BOSS) {
                Hitbox *hitbox = &LaundroMobile->hitboxBomb;
                if (entity->type != LAUNDROMOBILE_SPIKES)
                    hitbox = &LaundroMobile->hitboxBox;
                if (RSDK.CheckObjectCollisionTouchBox(laundroMobile, &LaundroMobile->hitboxBoss, entity, hitbox)
                    || laundroMobile->state == LaundroMobile_State_Unknown4) {
                    RSDK.PlaySfx(LaundroMobile->sfxLedgeBreak, false, 255);

                    EntityDebris *debris = NULL;
                    if (entity->type == LAUNDROMOBILE_SPIKES) {
                        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y - 0x80000);
                        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 18);
                        debris->drawFX |= FX_ROTATE;
                        debris->velocity.x    = LaundroMobile->currentVelocity + 0x18000;
                        debris->velocity.y    = -0x28000;
                        debris->gravity       = 0x3800;
                        debris->rotSpeed      = RSDK.Rand(-8, 8);
                        debris->drawOrder     = Zone->drawOrderHigh;
                        debris->updateRange.x = 0x400000;
                        debris->updateRange.y = 0x400000;

                        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y + 0x80000);
                        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 18);
                        debris->drawFX |= FX_ROTATE;
                        debris->velocity.x    = LaundroMobile->currentVelocity + 0x18000;
                        debris->velocity.y    = -0x20000;
                        debris->gravity       = 0x3800;
                        debris->rotSpeed      = RSDK.Rand(-8, 8);
                        debris->drawOrder     = Zone->drawOrderHigh;
                        debris->updateRange.x = 0x400000;
                        debris->updateRange.y = 0x400000;
                        entity->position.x += 0x100000;
                    }

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (entity->animator1.frameID % 3) + 6);
                    debris->velocity.y    = -0x28000;
                    debris->velocity.x    = LaundroMobile->currentVelocity + 0x20000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (entity->animator1.frameID % 3) + 7);
                    debris->velocity.y    = -0x28000;
                    debris->velocity.x    = LaundroMobile->currentVelocity + 0x28000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (entity->animator1.frameID % 3) + 8);
                    debris->velocity.y    = -0x20000;
                    debris->velocity.x    = LaundroMobile->currentVelocity + 0x20000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (entity->animator1.frameID % 3) + 9);
                    debris->velocity.y    = -0x20000;
                    debris->velocity.x    = LaundroMobile->currentVelocity + 0x28000;
                    debris->gravity       = 0x3800;
                    debris->drawOrder     = Zone->drawOrderHigh;
                    debris->updateRange.x = 0x400000;
                    debris->updateRange.y = 0x400000;

                    destroyEntity(entity);
                    foreach_break;
                }
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void LaundroMobile_State2_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);

    entity->animator2.animationSpeed = LaundroMobile->animSpeed;
    entity->animator3.animationSpeed = LaundroMobile->animSpeed;
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    Zone->playerBoundActiveL[0] = true;
    Zone->playerBoundActiveR[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;
    Zone->screenBoundsR1[0]     = RSDK_screens->centerX + (entity->position.x >> 16);
    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x - 0xC00000)
        Zone->screenBoundsT1[0] = RSDK_screens->position.y;

    if (!LaundroMobile->health && !LaundroMobile->invincibilityTimer) {
        Debris_FallFlickerSetup(LaundroMobile->aniFrames, LaundroMobile->debrisInfo);
        destroyEntity(entity);
    }
}

void LaundroMobile_StateDraw2_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);
    Vector2 drawPos;

    drawPos.x                 = entity->position.x;
    drawPos.y                 = entity->position.y;
    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, 0, false);
    RSDK.DrawSprite(&entity->animator2, 0, false);

    drawPos.y += 0x200000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.y += 0x200000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->animator1.frameID = 3;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}

void LaundroMobile_State6_Unknown1(void)
{
    RSDK_THIS(LaundroMobile);

    if (--entity->timer <= 0) {
        CREATE_ENTITY(Water, intToVoid(WATER_SPLASH), entity->position.x, Water->waterLevel);
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void LaundroMobile_EditorDraw(void)
{
    RSDK_THIS(LaundroMobile);

    switch (entity->type) {
        case LAUNDROMOBILE_BOSS:
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 3, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &entity->animator3, true, 0);
            entity->startY        = entity->position.y;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            entity->stateDraw     = LaundroMobile_StateDraw_Unknown1;
            break;
        case LAUNDROMOBILE_BOMB:
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 7, &entity->animator1, true, 0);
            entity->updateRange.x = 0x1800000;
            entity->updateRange.y = 0x1800000;
            entity->stateDraw     = LaundroMobile_StateDraw4_Unknown1;
            break;
        case LAUNDROMOBILE_PHASE2:
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 1, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 2, &entity->animator3, true, 0);
            entity->updateRange.x = 0x2000000;
            entity->updateRange.y = 0x800000;
            entity->stateDraw     = LaundroMobile_StateDraw2_Unknown1;
            break;
        case LAUNDROMOBILE_LOOPPOINT:
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.DrawSprite(&entity->animator1, NULL, false);
            entity->stateDraw = StateMachine_None;
            break;
        case LAUNDROMOBILE_BLOCK:
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &entity->animator1, false, 0);
            entity->updateRange.x = 0x1800000;
            entity->updateRange.y = 0x1800000;
            entity->stateDraw     = LaundroMobile_StateDraw4_Unknown1;
            break;
        case LAUNDROMOBILE_SPIKES:
            entity->startPos.x = entity->position.x;
            entity->startPos.y = entity->position.y;
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &entity->animator1, false, 3);
            entity->updateRange.x = 0x1800000;
            entity->updateRange.y = 0x1800000;
            entity->stateDraw     = LaundroMobile_StateDraw4_Unknown1;
            break;
        default: break;
    }

    StateMachine_Run(entity->stateDraw);
}

void LaundroMobile_EditorLoad(void)
{
    LaundroMobile->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/LaundroMobile.bin", SCOPE_STAGE);
    LaundroMobile->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ2.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LaundroMobile, type);
    RSDK_ENUM_VAR(LAUNDROMOBILE_BOSS);
    RSDK_ENUM_VAR(LAUNDROMOBILE_BOMB);
    RSDK_ENUM_VAR(LAUNDROMOBILE_PHASE2);
    RSDK_ENUM_VAR(LAUNDROMOBILE_LOOPPOINT);
    RSDK_ENUM_VAR(LAUNDROMOBILE_BLOCK);
    RSDK_ENUM_VAR(LAUNDROMOBILE_SPIKES);
}
#endif

void LaundroMobile_Serialize(void) { RSDK_EDITABLE_VAR(LaundroMobile, VAR_UINT8, type); }
