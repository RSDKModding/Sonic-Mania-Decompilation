// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LaundroMobile Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLaundroMobile *LaundroMobile;

void LaundroMobile_Update(void)
{
    RSDK_THIS(LaundroMobile);

    StateMachine_Run(self->state);
}

void LaundroMobile_LateUpdate(void) {}

void LaundroMobile_StaticUpdate(void)
{
    if (LaundroMobile->loopSfxTimer) {
        if (!LaundroMobile->playingLoopSfx) {
            if (LaundroMobile->health <= 8)
                RSDK.PlaySfx(LaundroMobile->sfxWash, true, 0xFF);
            else
                RSDK.PlaySfx(LaundroMobile->sfxFan, 47208, 0xFF);

            LaundroMobile->playingLoopSfx = true;
        }
    }
    else {
        if (LaundroMobile->playingLoopSfx) {
            RSDK.StopSfx(LaundroMobile->sfxFan);
            RSDK.StopSfx(LaundroMobile->sfxWash);

            LaundroMobile->playingLoopSfx = false;
        }
    }

    LaundroMobile->loopSfxTimer = false;

    if (SceneInfo->state == ENGINESTATE_REGULAR && LaundroMobile->useStageWrap)
        LaundroMobile_HandleStageWrap();
}

void LaundroMobile_Draw(void)
{
    RSDK_THIS(LaundroMobile);

    StateMachine_Run(self->stateDraw);
}

void LaundroMobile_Create(void *data)
{
    RSDK_THIS(LaundroMobile);

    if (globals->gameMode == MODE_TIMEATTACK) {
        destroyEntity(self);
    }
    else {
        self->drawFX = FX_FLIP;
        if (!SceneInfo->inEditor) {
            if (!self->type)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case LAUNDROMOBILE_BOSS:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 3, &self->propellerAnimator, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
                    self->startY        = self->position.y;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    LaundroMobile->health             = 5 + 8; // phase1 health + phase 2 health
                    LaundroMobile->invincibilityTimer = 0;
                    LaundroMobile->nextLoopPoint      = 0;
                    LaundroMobile->attackDir          = FLIP_NONE;
                    LaundroMobile->currentVelocity    = 0x38000;
                    LaundroMobile->rocketActive       = false;
                    LaundroMobile->animSpeed          = 0;
                    LaundroMobile->travelledPaths     = 0;
                    LaundroMobile->useStageWrap       = false;
                    LaundroMobile->loopSfxTimer       = 0;
                    LaundroMobile->playingLoopSfx     = false;
                    RSDK.StopSfx(LaundroMobile->sfxFan);
                    RSDK.StopSfx(LaundroMobile->sfxWash);
                    LaundroMobile->rocketAngles[0] = 0;
                    LaundroMobile->rocketAngles[1] = 0x8000;
                    LaundroMobile->rocketAngles[2] = 0;
                    LaundroMobile->rocketAngles[3] = 0x8000;
                    LaundroMobile->rocketSpeeds[0] = 0;
                    LaundroMobile->rocketSpeeds[1] = 0;
                    LaundroMobile->isUnderwater    = false;

                    self->tileCollisions  = TILECOLLISION_DOWN;
                    self->collisionLayers = Zone->collisionLayers;
                    self->collisionPlane  = 0;
                    self->state           = LaundroMobile_StateBoss_AwaitPlayer_Phase1;
                    self->stateDraw       = LaundroMobile_Draw_Boss;
                    break;

                case LAUNDROMOBILE_BOMB:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    self->originPos = self->position;

                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 7, &self->mainAnimator, true, 0);

                    self->updateRange.x = 0x1800000;
                    self->updateRange.y = 0x1800000;
                    self->state         = LaundroMobile_StateBomb_Spawner;
                    self->stateDraw     = LaundroMobile_Draw_Simple;
                    break;

                case LAUNDROMOBILE_LAUNDRY:
                    self->active    = ACTIVE_XBOUNDS;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0] + 1;

                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 1, &self->propellerAnimator, true, 0);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 2, &self->eggmanAnimator, true, 0);

                    self->updateRange.x = 0x2000000;
                    self->updateRange.y = 0x800000;
                    self->state         = LaundroMobile_State_Laundry;
                    self->stateDraw     = LaundroMobile_Draw_Laundry;
                    break;

                case LAUNDROMOBILE_LOOPPOINT: LaundroMobile->loopPoints[LaundroMobile->nextLoopPoint++] = (Entity *)self; break;

                case LAUNDROMOBILE_BLOCK:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    self->originPos = self->position;

                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &self->mainAnimator, true, RSDK.Rand(0, 3));

                    self->updateRange.x = 0x1800000;
                    self->updateRange.y = 0x1800000;
                    self->velocity.x    = LaundroMobile->currentVelocity - 0x18000;
                    self->state         = LaundroMobile_StateBlock_Spawner;
                    self->stateDraw     = LaundroMobile_Draw_Simple;
                    break;

                case LAUNDROMOBILE_SPIKES:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];
                    self->originPos = self->position;

                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &self->mainAnimator, true, RSDK.Rand(0, 3) + 3);

                    self->updateRange.x = 0x1800000;
                    self->updateRange.y = 0x1800000;
                    self->velocity.x    = LaundroMobile->currentVelocity - 0x18000;
                    self->state         = LaundroMobile_StateBlock_Spawner;
                    self->stateDraw     = LaundroMobile_Draw_Simple;
                    break;

                case LAUNDROMOBILE_DELAYEDSPLASH:
                    self->active = ACTIVE_NORMAL;
                    self->timer  = 8;
                    self->state  = LaundroMobile_State_DelayedSplash;
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

    LaundroMobile->active         = ACTIVE_ALWAYS;
    LaundroMobile->playingLoopSfx = false;
    LaundroMobile->loopSfxTimer   = 0;

    LaundroMobile->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    LaundroMobile->sfxExplosion  = RSDK.GetSfx("Stage/Explosion2.wav");
    LaundroMobile->sfxButton2    = RSDK.GetSfx("Stage/Button2.wav");
    LaundroMobile->sfxFan        = RSDK.GetSfx("HCZ/BigFan.wav");
    LaundroMobile->sfxRoll       = RSDK.GetSfx("Global/Roll.wav");
    LaundroMobile->sfxWash       = RSDK.GetSfx("HCZ/Wash.wav");
    LaundroMobile->sfxHullClose  = RSDK.GetSfx("Stage/HullClose.wav");
    LaundroMobile->sfxPush       = RSDK.GetSfx("Stage/Push.wav");
    LaundroMobile->sfxFireball   = RSDK.GetSfx("Stage/Fireball.wav");
    LaundroMobile->sfxButton     = RSDK.GetSfx("Stage/Button.wav");
    LaundroMobile->sfxLedgeBreak = RSDK.GetSfx("Stage/LedgeBreak3.wav");
    LaundroMobile->sfxPimPom     = RSDK.GetSfx("Stage/PimPom.wav");
#if MANIA_USE_PLUS
    LaundroMobile->sfxRumble = RSDK.GetSfx("Stage/Rumble.wav");
    LaundroMobile->sfxImpact = RSDK.GetSfx("Stage/Impact6.wav");
#endif
}

void LaundroMobile_CheckPlayerCollisions(void)
{
    RSDK_THIS(LaundroMobile);

    if (LaundroMobile->invincibilityTimer) {
        LaundroMobile->invincibilityTimer--;
    }
    else {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;
        foreach_active(Player, player)
        {
            for (int32 i = 0; i < 4 && LaundroMobile->rocketActive; ++i) {
                self->position.x = LaundroMobile->rocketPositions[i].x;
                self->position.y = LaundroMobile->rocketPositions[i].y;

                if (Player_CheckCollisionTouch(player, self, &LaundroMobile->hitboxMissile)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x400, 2, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                    break;
                }
            }
            self->position.x = storeX;
            self->position.y = storeY;

            int32 velX = player->velocity.x;
            int32 velY = player->velocity.y;
            if (Player_CheckBadnikTouch(player, self, &LaundroMobile->hitboxBoss) && Player_CheckBossHit(player, self)) {
                if (LaundroMobile->health)
                    LaundroMobile->health--;

                if (LaundroMobile->health) {
                    if (LaundroMobile->health == 8) {
                        RSDK.StopSfx(LaundroMobile->sfxFan);
                        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 4, &self->propellerAnimator, true, 0);
                        self->timer = 60;
                        self->state = LaundroMobile_StateBoss_Destroyed_Phase1;
                    }
                    else {
                        if (LaundroMobile->health < 8) {
                            RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 3, &self->eggmanAnimator, true, 0);
                        }
                        else {
                            RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 2, &self->eggmanAnimator, true, 0);
                            player->groundVel  = velX;
                            player->velocity.x = velX;
                            player->velocity.y = velY;
                        }
                        RSDK.PlaySfx(LaundroMobile->sfxHit, false, 255);
                        LaundroMobile->invincibilityTimer = 30;
                    }
                }
                else {
                    SceneInfo->timeEnabled = false;
                    Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                    RSDK.PlaySfx(LaundroMobile->sfxExplosion, false, 255);
                    LaundroMobile->invincibilityTimer = 60;

                    EntityWhirlpool *whirlpool = self->whirlpool;
                    self->state                = LaundroMobile_StateBoss_Destroyed_Phase2;
                    if (whirlpool) {
                        if (whirlpool->classID == Whirlpool->classID)
                            whirlpool->activePlayers = -3;
                        self->whirlpool = NULL;
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

    int32 interval = LaundroMobile->health > 8 ? 7 : 3;

    if (!(Zone->timer % interval)) {
        RSDK.PlaySfx(LaundroMobile->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            int32 x                    = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y                    = self->position.y + (RSDK.Rand(-24, 25) << 16);
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);

            explosion->drawGroup = Zone->objectDrawGroup[1] + 2;
            if (LaundroMobile->health > 8)
                explosion->velocity.x = 0x24000;
        }
    }
}

void LaundroMobile_HandleStageWrap(void)
{
    EntityLaundroMobile *boss = LaundroMobile->laundroMobile;

    if (!(Zone->timer & 3)) {
        EntityCurrent *current = CREATE_ENTITY(Current, INT_TO_VOID(CURRENT_CHILD_BUBBLE), ScreenInfo->position.x << 16,
                                               (8 * RSDK.Rand(0, ScreenInfo->size.y >> 3) + ScreenInfo->position.y) << 16);

        current->drawGroup = Zone->playerDrawGroup[0];
        current->strength  = 6;
        current->type      = CURRENT_C_RIGHT;
        current->alpha     = 0xF0;
        current->size.x    = (ScreenInfo->position.x + ScreenInfo->size.x + 0x1000) << 16;
    }

    foreach_active(Player, player)
    {
        if (player->state != Player_State_Static) {
            if (player->position.x < boss->position.x)
                player->position.x = boss->position.x;

            if (Player_CheckValidState(player)) {
                if ((abs(player->velocity.y) <= 0x18000 || player->onGround) && player->position.x < 0x6D800000) {
                    player->collisionMode   = CMODE_FLOOR;
                    player->onGround        = false;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->state           = Current_PlayerState_Right;
                    if (player->animator.animationID != ANI_CLING && player->animator.animationID != ANI_SHAFT_SWING) {
                        if (player->position.x >= boss->position.x + 0xC00000) {
                            player->velocity.x = LaundroMobile->currentVelocity;
                            player->groundVel  = player->velocity.x;
                        }
                        else {
                            player->velocity.x = LaundroMobile->currentVelocity + ((boss->position.x - player->position.x + 0xC00000) >> 6);
                            player->groundVel  = player->velocity.x;
                        }
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                    }

                    player->velocity.y = 0;
                    if (player->up)
                        player->velocity.y = -0x18000;
                    else if (player->down)
                        player->velocity.y = 0x18000;
                }
            }
        }
    }

    if (LaundroMobile->currentVelocity < 0x80000)
        LaundroMobile->currentVelocity += 0x400;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (Player_CheckValidState(player1)) {
        if (LaundroMobile->nextLoopPoint == 5 && boss->timer <= 0) {
            if (LaundroMobile->currentVelocity < 0xC0000)
                LaundroMobile->currentVelocity += 0x1000;
        }
        else {
            Entity *loopPoint = LaundroMobile->loopPoints[LaundroMobile->nextLoopPoint];
            if (player1->position.x >= loopPoint->position.x + 0xE000000) {
                int32 startOffsetX = player1->position.x - loopPoint->position.x - 0xE000000;
                int32 startOffsetY = player1->position.y - loopPoint->position.y;
                if (LaundroMobile->health <= 8) {
                    LaundroMobile->nextLoopPoint = 5;
                    boss->state                  = LaundroMobile_StateBoss_WaitForLastStageWrap;
                }
                else {
                    if (LaundroMobile->travelledPaths == 0x1E) {
                        LaundroMobile->travelledPaths = 0;
                    }

                    int32 prevLoopPoint = LaundroMobile->nextLoopPoint;
                    while (LaundroMobile->nextLoopPoint == prevLoopPoint) {
                        // LoopPoints 1-4 are valid to use as attacks, 5 is reserved for the final stage wrap
                        LaundroMobile->nextLoopPoint = RSDK.Rand(1, 5);

                        while ((1 << LaundroMobile->nextLoopPoint) & LaundroMobile->travelledPaths) {
                            LaundroMobile->nextLoopPoint = RSDK.Rand(1, 5);
                        }
                    }

                    LaundroMobile->travelledPaths |= 1 << LaundroMobile->nextLoopPoint;
                }
                loopPoint = LaundroMobile->loopPoints[LaundroMobile->nextLoopPoint];

                int32 offsetX = player1->position.x - startOffsetX - loopPoint->position.x;
                int32 offsetY = player1->position.y - loopPoint->position.y - startOffsetY;
                player1->position.x -= offsetX;
                player1->position.y -= offsetY;

                boss->position.y -= offsetY;
                boss->originPos.y -= offsetY;
                boss->unusedPos.y -= offsetY;

                boss->position.x -= offsetX;
                boss->originPos.x -= offsetX;
                boss->unusedPos.x -= offsetX;

                ScreenInfo->position.x -= offsetX >> 16;
                ScreenInfo->position.y -= offsetY >> 16;

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

                for (int32 i = 0; i < 0x1000; ++i) {
                    Entity *entPtr = RSDK_GET_ENTITY_GEN(i);

                    if (entPtr->classID == LaundroMobile->classID) {
                        EntityLaundroMobile *laundroMobile = (EntityLaundroMobile *)entPtr;
                        if ((laundroMobile->type == LAUNDROMOBILE_BOMB && laundroMobile->state != LaundroMobile_StateBomb_Spawner)
                            || laundroMobile->state == LaundroMobile_StateBlock_Block) {
                            laundroMobile->position.x -= offsetX;
                            laundroMobile->position.y -= offsetY;
                        }
                    }
                    else if (entPtr->classID == Ring->classID) {
                        EntityRing *ring = (EntityRing *)entPtr;
                        if (ring->state != Ring_State_Normal) {
                            entPtr->position.x -= offsetX;
                            entPtr->position.y -= offsetY;
                        }
                    }
                    else if (entPtr->classID == Debris->classID) {
                        entPtr->position.x -= offsetX;
                        entPtr->position.y -= offsetY;
                    }
                    else if (entPtr->classID == Water->classID) {
                        EntityWater *water = (EntityWater *)entPtr;
                        if (water->type == WATER_BUBBLE) {
                            water->position.x -= offsetX;
                            water->position.y -= offsetY;
                            water->bubbleX = water->position.x;
                        }
                    }
                    else if (entPtr->classID == ImageTrail->classID) {
                        EntityImageTrail *trail = (EntityImageTrail *)entPtr;
                        trail->position.x -= offsetX;
                        trail->position.y -= offsetY;
                        trail->currentPos.x -= offsetX;
                        trail->currentPos.y -= offsetY;
                        for (int32 t = 0; t < IMAGETRAIL_TRACK_COUNT; ++t) {
                            trail->statePos[t].x -= offsetX;
                            trail->statePos[t].y -= offsetY;
                        }
                    }
                    else if (entPtr->classID == Current->classID) {
                        EntityCurrent *current = (EntityCurrent *)entPtr;
                        if (current->state == Current_State_Child) {
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

void LaundroMobile_HandleRocketMovement(void)
{
    RSDK_THIS(LaundroMobile);

    for (int32 r = 0; r < 2; ++r) {
        LaundroMobile->rocketPositions[r].x = self->position.x + 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[r] >> 8);
        LaundroMobile->rocketPositions[r].y = self->position.y + 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[r] >> 8);
        LaundroMobile->rocketAngles[r] += LaundroMobile->rocketSpeeds[0];
    }

    for (int32 r = 2; r < 4; ++r) {
        LaundroMobile->rocketPositions[r].x = self->position.x - 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[r] >> 8);
        LaundroMobile->rocketPositions[r].y = self->position.y + 0x1400 * RSDK.Cos256(LaundroMobile->rocketAngles[r] >> 8);
        LaundroMobile->rocketAngles[r] += LaundroMobile->rocketSpeeds[1];
    }
}

void LaundroMobile_HandleEggmanAnimations(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    bool32 playerHurt = false;
    foreach_active(Player, player)
    {
        if (player->state == Player_State_Hurt || player->state == Player_State_Death || player->state == Player_State_Drown)
            playerHurt = true;
    }

    switch (self->eggmanAnimator.animationID) {
        case 0:
            if (playerHurt)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 4, &self->eggmanAnimator, true, 0);
            break;

        case 1:
            if (playerHurt)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 5, &self->eggmanAnimator, true, 0);
            break;

        case 2:
            if (!LaundroMobile->invincibilityTimer)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
            break;

        case 3:
            if (!LaundroMobile->invincibilityTimer)
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 1, &self->eggmanAnimator, true, 0);
            break;

        case 4:
            if (self->eggmanAnimator.frameID >= self->eggmanAnimator.frameCount - 1) {
                if (playerHurt)
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 4, &self->eggmanAnimator, true, 6);
                else
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
            }
            break;

        case 5:
            if (self->eggmanAnimator.frameID >= self->eggmanAnimator.frameCount - 1) {
                if (playerHurt)
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 5, &self->eggmanAnimator, true, 6);
                else
                    RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 1, &self->eggmanAnimator, true, 0);
            }
            break;

        default: break;
    }
}

void LaundroMobile_HandleTileCollisions(void)
{
    RSDK_THIS(LaundroMobile);

    uint8 collisionLevel = 0xFF;
    while (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0x200000, 0x200000, false) && collisionLevel >= 0) {
        self->position.y -= 0x10000;
        collisionLevel = 0;
    }

    while (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0x200000, -0x200000, false) && collisionLevel >= 1) {
        self->position.y += 0x10000;
        collisionLevel = 1;
    }

    while (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, -0x200000, 0x200000, false) && collisionLevel >= 2) {
        self->position.y -= 0x10000;
        collisionLevel = 2;
    }

    while (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, -0x200000, -0x200000, false) && collisionLevel >= 3) {
        self->position.y += 0x10000;
        collisionLevel = 3;
    }
}

void LaundroMobile_StateBoss_AwaitPlayer_Phase1(void)
{
    RSDK_THIS(LaundroMobile);

    if (++self->timer >= 2) {
        self->position.y += 0x500000;
        self->originPos.y = self->position.y;
        self->position.x -= 0x100000;
        self->timer  = 0;
        self->active = ACTIVE_NORMAL;
        RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (self->position.x >= (24704 << 16)) {
            // did this use to only have 12 health at some point during dev?
            if (LaundroMobile->health == (MANIA_USE_PLUS ? (5 + 8) : (4 + 8))) {
                LaundroMobile->health = 8;
                Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
                self->visible = true;
                self->state   = LaundroMobile_StateBoss_AwaitPlayer_Phase2;
            }
            else {
                destroyEntity(self);
            }
        }
        else {
            LaundroMobile->laundroMobile                                       = self;
            self->state                                                        = LaundroMobile_StateBoss_SetupArena_Phase1;
            RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
            EntityPlayer *player1                                              = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->jumpPress                                                 = false;
        }
    }
}

void LaundroMobile_StateBoss_SetupArena_Phase1(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
    player1->jumpPress                                                 = false;

    if (player1->position.x > self->position.x + 0x1700000) {
        Music_TransitionTrack(TRACK_EGGMAN1, 0.0125);
        RSDK.GetTileLayer(4)->drawGroup[0] = DRAWGROUP_COUNT;
        LaundroMobile->nextLoopPoint       = 0;
        Water->waterLevel                  = 0;
        Water->targetWaterLevel            = 0;
        self->timer                        = 120;
        self->propellerAnimator.speed      = 0;
        self->visible                      = true;
        self->state                        = LaundroMobile_StateBoss_EnterEggman_Phase1;
    }
}

void LaundroMobile_StateBoss_EnterEggman_Phase1(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.ProcessAnimation(&self->propellerAnimator);

    self->position.x += 0x18000;
    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
    RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->jumpPress                   = false;

    if (!--self->timer) {
        RSDK.PlaySfx(LaundroMobile->sfxButton2, false, 255);
        self->state                 = LaundroMobile_StateBoss_StartupPropellers;
        LaundroMobile->useStageWrap = true;
    }

    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_StartupPropellers(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 0;
    RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->jumpPress                   = false;

    RSDK.ProcessAnimation(&self->propellerAnimator);

    if (self->propellerAnimator.speed >= 0x200) {
        ++LaundroMobile->loopSfxTimer;
        RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, BreakBar)->releaseTimer = 240;
        self->state                                                        = LaundroMobile_StateBoss_HandlePhase1;
    }
    else {
        self->propellerAnimator.speed += 4;
        if (self->propellerAnimator.speed >= 0x20)
            ++LaundroMobile->loopSfxTimer;
    }

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_HandlePhase1(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_HandleEggmanAnimations();

    RSDK.ProcessAnimation(&self->propellerAnimator);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    self->position.y = self->originPos.y;

    if (abs(self->originPos.y - player1->position.y) >= 0x100000) {
        if (player1->position.y >= self->originPos.y) {
            if (self->velocity.y < 0x18000)
                self->velocity.y += 0x1000;
        }
        else {
            if (self->velocity.y > -0x18000)
                self->velocity.y -= 0x1000;
        }
    }
    else if (self->velocity.y) {
        if (self->velocity.y <= 0) {
            self->velocity.y += 0x1000;

            if (self->velocity.y > 0)
                self->velocity.y = 0;
        }
        else {
            self->velocity.y -= 0x1000;

            if (self->velocity.y < 0)
                self->velocity.y = 0;
        }
    }

    self->position.y += self->velocity.y;
    LaundroMobile_HandleTileCollisions();

    if (self->position.y == self->originPos.y) {
        self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    }
    else {
        self->originPos.y = self->position.y;
    }

    self->position.x += LaundroMobile->currentVelocity;

    ++LaundroMobile->loopSfxTimer;
    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_Destroyed_Phase1(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_Explode();

    RSDK.ProcessAnimation(&self->propellerAnimator);

    self->position.y = self->originPos.y;
    LaundroMobile_HandleTileCollisions();

    self->position.x += LaundroMobile->currentVelocity - 0x8000;

    if (self->position.y == self->originPos.y)
        self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    else
        self->originPos.y = self->position.y;

    LaundroMobile_CheckPlayerCollisions();
    if (--self->timer <= 0) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);

        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 4, &debris->animator, true, 0);
        debris->velocity.x      = 0x50000;
        debris->velocity.y      = -0x28000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;
        RSDK.SetSpriteAnimation(-1, 0, &self->propellerAnimator, true, 0);
        self->state = LaundroMobile_StateBoss_Explode_Phase1;
    }
}

void LaundroMobile_StateBoss_Explode_Phase1(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_Explode();

    RSDK.ProcessAnimation(&self->propellerAnimator);

    self->position.y = self->originPos.y;
    LaundroMobile_HandleTileCollisions();

    self->position.x += LaundroMobile->currentVelocity - 0x10000;

    if (self->position.y == self->originPos.y)
        self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    else
        self->originPos.y = self->position.y;

    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_WaitForLastStageWrap(void)
{
    RSDK_THIS(LaundroMobile);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > (28032 << 16)) {
        LaundroMobile->useStageWrap = false;
        foreach_active(Player, player) { player->state = Player_State_Air; }
        self->state = LaundroMobile_StateBoss_AwaitPlayer_Phase2;
    }

    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_AwaitPlayer_Phase2(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player1->onGround && !player1->angle) { // wait till we're on flat ground
        self->timer = 60;
        RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 1, &self->eggmanAnimator, true, 0);
        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 5, &self->propellerAnimator, true, 0);
        self->state = LaundroMobile_StateBoss_SetupArena_Phase2;
    }

    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_SetupArena_Phase2(void)
{
    RSDK_THIS(LaundroMobile);

    if (--self->timer <= 0) {
        self->position.x = 28160 << 16;
        self->timer      = 104;
        self->position.y = (ScreenInfo->position.y - 64) << 16;

        LaundroMobile_HandleRocketMovement();

        self->stateDraw = LaundroMobile_Draw_Boss_Destroyed;
        self->state     = LaundroMobile_StateBoss_EnterEggman_Phase2;
    }

    LaundroMobile_CheckPlayerCollisions();
}

void LaundroMobile_StateBoss_EnterEggman_Phase2(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.y += 0x10000;

    LaundroMobile_HandleRocketMovement();

    if (--self->timer <= 0) {
        self->timer = 32;
        self->state = LaundroMobile_StateBoss_StartupRockets;
    }

    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_StartupRockets(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_HandleRocketMovement();

    if (LaundroMobile->rocketSpeeds[0] < 0x200)
        LaundroMobile->rocketSpeeds[0] += 8;

    if (self->timer <= 0) {
        if (LaundroMobile->rocketSpeeds[1] < 0x200)
            LaundroMobile->rocketSpeeds[1] += 8;
    }
    else {
        self->timer--;
    }

    if (LaundroMobile->rocketSpeeds[0] + LaundroMobile->rocketSpeeds[1] == 0x400) {
        self->timer = 96;
        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 6, &self->flameAnimator, true, 0);
        LaundroMobile->rocketActive = true;
        RSDK.PlaySfx(LaundroMobile->sfxPush, false, 255);
        self->state = LaundroMobile_StateBoss_SpeedUpRockets;
    }

    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_SpeedUpRockets(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile_HandleRocketMovement();

    if (LaundroMobile->rocketSpeeds[0] < 0x400) {
        LaundroMobile->rocketSpeeds[1] += 8;
        LaundroMobile->rocketSpeeds[0] = LaundroMobile->rocketSpeeds[1];
    }

    if (--self->timer <= 0) {
        self->timer = 60;
        RSDK.PlaySfx(LaundroMobile->sfxFireball, false, 255);
        self->state = LaundroMobile_StateBoss_RiseUpToAttack;
    }

    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_RiseUpToAttack(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    LaundroMobile_HandleRocketMovement();

    if (--self->timer <= 0) {
        self->position.x += LaundroMobile->attackDir ? 0x580000 : -0x580000;
        self->velocity.y = 0x50000;
        RSDK.PlaySfx(LaundroMobile->sfxRoll, false, 255);
        LaundroMobile->attackCount = LaundroMobile->attackCounts[RSDK.Rand(0, 8)];
        self->state                = LaundroMobile_StateBoss_Attacking;
    }

    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_Attacking(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.ProcessObjectMovement(self, &LaundroMobile->outerBox, &LaundroMobile->innerBox);

    if (self->velocity.y < 0 && self->position.y < (ScreenInfo->position.y - 256) << 16) {
        self->onGround   = false;
        self->velocity.y = 0x50000;
        LaundroMobile->attackDir ^= FLIP_X;
        RSDK.PlaySfx(LaundroMobile->sfxRoll, false, 255);
    }

    if (self->position.y <= Water->waterLevel) {
        if (LaundroMobile->isUnderwater) {
            LaundroMobile->isUnderwater = false;

            CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);
            CREATE_ENTITY(LaundroMobile, INT_TO_VOID(LAUNDROMOBILE_DELAYEDSPLASH), self->position.x - 0x100000, Water->waterLevel);
            CREATE_ENTITY(LaundroMobile, INT_TO_VOID(LAUNDROMOBILE_DELAYEDSPLASH), self->position.x + 0x100000, Water->waterLevel);

            RSDK.PlaySfx(Water->sfxSplash, false, 255);
            if (--LaundroMobile->attackCount <= 0) {
                self->onGround   = false;
                self->state      = LaundroMobile_StateBoss_ReturnToLaundry;
                self->velocity.x = LaundroMobile->attackDir == FLIP_NONE ? -0x18000 : 0x18000;
            }
        }
    }
    else if (!LaundroMobile->isUnderwater) {
        LaundroMobile->isUnderwater = true;

        CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);
        CREATE_ENTITY(LaundroMobile, INT_TO_VOID(LAUNDROMOBILE_DELAYEDSPLASH), self->position.x - 0x100000, Water->waterLevel);
        CREATE_ENTITY(LaundroMobile, INT_TO_VOID(LAUNDROMOBILE_DELAYEDSPLASH), self->position.x + 0x100000, Water->waterLevel);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
    }

    LaundroMobile_HandleRocketMovement();
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_ReturnToLaundry(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if ((LaundroMobile->attackDir && self->position.x >= (28160 << 16)) || (!LaundroMobile->attackDir && self->position.x <= (28160 << 16))) {
        self->position.x = 28160 << 16;
        RSDK.SetSpriteAnimation(-1, 0, &self->flameAnimator, true, 0);
        LaundroMobile->rocketActive = false;
        RSDK.PlaySfx(LaundroMobile->sfxHullClose, false, 255);
        self->state = LaundroMobile_StateBoss_PrepareWhirlpool;
    }

    LaundroMobile_HandleRocketMovement();
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_PrepareWhirlpool(void)
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
        self->state = LaundroMobile_StateBoss_StartupWhirlpool;
    }

    LaundroMobile_HandleRocketMovement();
    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_StartupWhirlpool(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile->animSpeed += 4;
    if (LaundroMobile->animSpeed < 0x100) {
        LaundroMobile_CheckPlayerCollisions();
        LaundroMobile_HandleEggmanAnimations();
    }
    else {
        EntityWhirlpool *whirlpool = self->whirlpool;
        if (whirlpool) {
            if (whirlpool->classID == Whirlpool->classID) {
                whirlpool->position.x = self->position.x;
            }
            else {
                self->whirlpool = NULL;
            }

            self->timer = 480;
            self->state = LaundroMobile_StateBoss_WhirlpoolActive;
        }
        else {
            Vector2 pos = { 0x1000000, 0x1000000 };
            whirlpool   = CREATE_ENTITY(Whirlpool, &pos, self->position.x, self->originPos.y - 0x200000);

            whirlpool->activePlayers = 0xFF;
            whirlpool->angVel        = 10;
            whirlpool->alpha         = 0;
            whirlpool->drawGroup     = Zone->objectDrawGroup[0] + 1;
            whirlpool->isPermanent   = true;
            self->whirlpool          = whirlpool;

            self->timer = 480;
            self->state = LaundroMobile_StateBoss_WhirlpoolActive;
        }
    }
}

void LaundroMobile_StateBoss_WhirlpoolActive(void)
{
    RSDK_THIS(LaundroMobile);

    ++LaundroMobile->loopSfxTimer;

    if (--self->timer > 0) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);
            if (player->position.y > Water->waterLevel) {
                if (player->state == Player_State_Static) {
                    player->onGround        = false;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;

                    if ((player->position.y & 0xFFFF0000) != (self->position.y & 0xFFFF0000) + 0x580000) {
                        if (player->position.y >= self->position.y + 0x580000)
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
                        player->drawGroup = Zone->playerDrawGroup[0];
                    else
                        player->drawGroup = self->drawGroup - 1;

                    LaundroMobile->playerAngles[playerID] += 3;
                    player->position.x =
                        LaundroMobile->playerRadius[playerID] * RSDK.Cos256(LaundroMobile->playerAngles[playerID]) + self->position.x;
                }
                else if (Player_CheckValidState(player)) {
                    player->onGround                      = false;
                    player->velocity.x                    = 0;
                    player->velocity.y                    = 0;
                    player->groundVel                     = 0;
                    player->nextGroundState               = StateMachine_None;
                    player->nextAirState                  = StateMachine_None;
                    LaundroMobile->playerRadius[playerID] = ((player->position.x - self->position.x) & 0xFFFF0000);

                    if (LaundroMobile->playerRadius[playerID] <= 0) {
                        LaundroMobile->playerRadius[playerID] = -LaundroMobile->playerRadius[playerID];
                        LaundroMobile->playerAngles[playerID] = 128;
                    }
                    else {
                        LaundroMobile->playerAngles[playerID] = 0;
                    }
                    LaundroMobile->playerRadius[playerID] >>= 8;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);

                    player->state = Player_State_Static;
                }
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);
            if (player->state == Player_State_Static) {
                player->velocity.x = player->position.x
                                     - LaundroMobile->playerRadius[playerID] * RSDK.Cos256(LaundroMobile->playerAngles[playerID] - 3)
                                     - self->position.x;
                player->drawGroup = Zone->playerDrawGroup[0];
                player->state     = Player_State_Air;
            }
        }

        EntityWhirlpool *whirlpool = self->whirlpool;
        if (whirlpool) {
            if (whirlpool->classID == Whirlpool->classID)
                whirlpool->activePlayers = -3;

            self->whirlpool = NULL;
        }

        self->state = LaundroMobile_StateBoss_PrepareRockets;
    }

    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_PrepareRockets(void)
{
    RSDK_THIS(LaundroMobile);

    LaundroMobile->animSpeed -= 4;
    if (LaundroMobile->animSpeed <= 0) {
        self->velocity.y = 0;
        self->timer      = 32;
        self->state      = LaundroMobile_StateBoss_StartupRockets;
    }

    LaundroMobile_CheckPlayerCollisions();
    LaundroMobile_HandleEggmanAnimations();
}

void LaundroMobile_StateBoss_Destroyed_Phase2(void)
{
    RSDK_THIS(LaundroMobile);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (player->state == Player_State_Static) {
            player->velocity.x = player->position.x - LaundroMobile->playerRadius[playerID] * RSDK.Cos256(LaundroMobile->playerAngles[playerID] - 3)
                                 - self->position.x;
            player->drawGroup = Zone->playerDrawGroup[0];
            player->state     = Player_State_Air;
        }
    }

    LaundroMobile_Explode();

    if (LaundroMobile->animSpeed > 0)
        LaundroMobile->animSpeed -= 4;

    if (--LaundroMobile->invincibilityTimer) {
        if (LaundroMobile->invincibilityTimer == 30) {
            for (int32 i = 0; i < 4; ++i) {
                EntityDebris *debris =
                    CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, LaundroMobile->rocketPositions[i].x, LaundroMobile->rocketPositions[i].y);
                RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 5, &debris->animator, true, (LaundroMobile->rocketAngles[i] >> 12) & 0xF);
                debris->animator.speed = 0;

                if (debris->position.y >= self->position.y) {
                    debris->velocity.x = debris->position.x < self->position.x ? -0x20000 : 0x20000;
                    debris->velocity.y = -0x10000;
                }
                else {
                    debris->velocity.x = debris->position.x < self->position.x ? -0x40000 : 0x40000;
                    debris->velocity.y = -0x20000;
                }

                debris->gravityStrength             = 0x3800;
                debris->drawGroup                   = Zone->objectDrawGroup[1];
                debris->updateRange.x               = 0x400000;
                debris->updateRange.y               = 0x400000;
                LaundroMobile->rocketPositions[i].x = 0;
                LaundroMobile->rocketPositions[i].y = 0;
            }
        }
    }
    else {
        self->timer      = 60;
        self->velocity.y = 0x20000;
        self->state      = LaundroMobile_StateBoss_Explode_Phase2;
    }
}

void LaundroMobile_StateBoss_Explode_Phase2(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x3800;

    LaundroMobile_Explode();

    if (--self->timer <= 0) {
        Water->moveWaterLevel = true;
        Water->targetWaterLevel += 0x780000;
        Water->waterMoveSpeed = 0x10000;
        self->timer           = 0;

        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        CREATE_ENTITY(EggPrison, INT_TO_VOID(EGGPRISON_FLYING), (ScreenInfo->position.x + ScreenInfo->center.x) << 16,
                      (ScreenInfo->position.y - 48) << 16);

#if MANIA_USE_PLUS
        Zone->stageFinishCallback = LaundroMobile_StageFinish_Wait;
        self->state               = LaundroMobile_StateOutro_StartCutscene;
#else
        destroyEntity(self);
#endif
    }
}

#if MANIA_USE_PLUS
void LaundroMobile_StageFinish_Wait(void) {}

void LaundroMobile_StateOutro_StartCutscene(void)
{
    RSDK_THIS(LaundroMobile);

    EntityActClear *actClear = RSDK_GET_ENTITY(SLOT_ACTCLEAR, ActClear);

    if (self->timer) {
        if (actClear->classID != ActClear->classID) {
            self->timer = 0;

            EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
            player1->drawGroup    = Zone->playerDrawGroup[0];
            player1->state        = Player_State_Ground;
            player1->direction    = FLIP_NONE;
            player1->stateInput   = StateMachine_None;
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);

            EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
            if (player2->classID == Player->classID) {
                player2->drawGroup  = Zone->playerDrawGroup[0];
                player2->state      = Player_State_Ground;
                player2->direction  = FLIP_NONE;
                player2->stateInput = StateMachine_None;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            }

            self->state = LaundroMobile_StateOutro_Rumble;
        }
    }
    else if (actClear->classID == ActClear->classID) {
        self->timer = 1;
    }
}

void LaundroMobile_StateOutro_Rumble(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    player1->timer = 0;
    if (player2->classID == Player->classID)
        player2->timer = 0;

    if (!(Zone->timer & 3)) {
        Camera_ShakeScreen(0, 0, 2);
    }

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(LaundroMobile->sfxRumble, false, 255);

    if (++self->timer == 90) {
        self->timer = 0;
        foreach_active(WaterGush, gush)
        {
            if (gush->position.x > self->position.x) {
                gush->activated = true;
                gush->inkEffect = INK_ALPHA;
                gush->alpha     = 256;
                gush->drawGroup = Zone->playerDrawGroup[0];
            }
        }

        for (int32 i = 0; i < 0x20; ++i) {
            EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Fall, 28336 << 16, 2784 << 16);

            RSDK.SetSpriteAnimation(WaterGush->aniFrames, 4, &debris->animator, true, 0);
            debris->position.x += 0x60000 * RSDK.Rand(-8, 8);
            debris->position.y += 0x60000 * RSDK.Rand(-8, 8);
            debris->velocity.x      = RSDK.Rand(-8, 8) << 16;
            debris->velocity.y      = RSDK.Rand(-8, 8) << 16;
            debris->velocity.x      = RSDK.Rand(-8, 9) << 15;
            debris->velocity.y      = RSDK.Rand(-8, 5) << 16;
            debris->direction       = RSDK.Rand(0, 4);
            debris->drawFX          = FX_FLIP;
            debris->drawGroup       = Zone->objectDrawGroup[1];
            debris->gravityStrength = 0x3800;
        }

        RSDK.CopyTileLayer(Zone->fgLayer[1], 1763, 172, Zone->fgLayer[1], 1919, 172, 11, 4);

        RSDK.PlaySfx(LaundroMobile->sfxImpact, false, 255);
        RSDK.PlaySfx(WaterGush->sfxGush, false, 255);
        RSDK.PlaySfx(Water->sfxSplash, false, 255);
        self->state = LaundroMobile_StateOutro_WaterGush;
    }
}

void LaundroMobile_StateOutro_WaterGush(void)
{
    RSDK_THIS(LaundroMobile);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    player1->timer = 0;
    if (player2->classID == Player->classID)
        player2->timer = 0;

    if (++self->timer < 75) {
        foreach_active(WaterGush, gush)
        {
            gush->position.y += 0x8000;
            gush->alpha -= 2;

            if (gush->position.x > self->position.x) {
                if (gush->position.x - gush->gushPos < player1->position.x) {
                    player1->velocity.x = -0x30000;
                    player1->groundVel  = -0x30000;
                }

                if (player2->classID == Player->classID && gush->position.x - gush->gushPos < player2->position.x) {
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

            if (gush->position.x > self->position.x && gush->alpha <= 0) {
                destroyEntity(gush);

                for (int32 p = 0; p < PLAYER_COUNT; ++p) Zone->playerBoundActiveR[p] = false;

                self->timer = 0;
                self->state = LaundroMobile_StateOutro_ExitHCZ;
            }
        }
    }
}

void LaundroMobile_StateOutro_ExitHCZ(void)
{
    RSDK_THIS(LaundroMobile);

    if (++self->timer > 120) {
        RSDK_GET_ENTITY(SLOT_PLAYER2, Player)->right = true;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->classID == Player->classID)
            player1->right = true;

        if (player1->position.x > (Zone->cameraBoundsR[0] + 64) << 16) {
            HCZSetup_StageFinish_EndAct2();
            destroyEntity(self);
        }
    }
}
#endif

void LaundroMobile_Draw_Boss(void)
{
    RSDK_THIS(LaundroMobile);

    if (LaundroMobile->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
        RSDK.SetPaletteEntry(1, 128, 0xE0E0E0);
    }

    Vector2 drawPos;
    drawPos.x = self->position.x - 0x20000;
    drawPos.y = self->position.y - 0x1B0000;

    self->mainAnimator.frameID = 4;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    drawPos.y += 0x300000;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    drawPos = self->position;
    drawPos.x += 0x100000;
    RSDK.DrawSprite(&self->eggmanAnimator, &drawPos, false);

    self->mainAnimator.frameID = 0;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->propellerAnimator, NULL, false);

    drawPos.x                  = self->position.x - 0x120000;
    drawPos.y                  = self->position.y - 0x1B0000;
    self->mainAnimator.frameID = 4;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    drawPos.y += 0x300000;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
    RSDK.SetPaletteEntry(1, 128, 0x000000);
}

void LaundroMobile_Draw_Boss_Destroyed(void)
{
    RSDK_THIS(LaundroMobile);

    if (LaundroMobile->invincibilityTimer & 1) {
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
        RSDK.SetPaletteEntry(1, 128, 0xE0E0E0);
    }

    for (int32 r = 0; r < 4; ++r) {
        self->direction = r >= 2 ? FLIP_X : FLIP_NONE;
        uint8 angle     = LaundroMobile->rocketAngles[r] >> 8;
        if (angle >= 0x80) {
            self->propellerAnimator.frameID = angle >> 4;

            self->flameAnimator.frameID = 12;
            if (!(Zone->timer & 1))
                self->flameAnimator.frameID = self->propellerAnimator.frameID;

            RSDK.DrawSprite(&self->propellerAnimator, &LaundroMobile->rocketPositions[r], false);
            RSDK.DrawSprite(&self->flameAnimator, &LaundroMobile->rocketPositions[r], false);
        }
    }

    self->direction            = FLIP_NONE;
    self->mainAnimator.frameID = 5;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

    self->mainAnimator.frameID = 1;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    for (int32 r = 0; r < 4; ++r) {
        self->direction = r >= 2 ? FLIP_X : FLIP_NONE;
        uint8 angle     = LaundroMobile->rocketAngles[r] >> 8;
        if (angle < 0x80) {
            self->propellerAnimator.frameID = angle >> 4;

            self->flameAnimator.frameID = 12;
            if (!(Zone->timer & 1))
                self->flameAnimator.frameID = self->propellerAnimator.frameID;

            angle = (angle + 64) & 0xFF;
            if (angle <= 0x80)
                RSDK.DrawSprite(&self->flameAnimator, &LaundroMobile->rocketPositions[r], false);

            RSDK.DrawSprite(&self->propellerAnimator, &LaundroMobile->rocketPositions[r], false);

            if (angle > 0x80)
                RSDK.DrawSprite(&self->flameAnimator, &LaundroMobile->rocketPositions[r], false);
        }
    }

    self->direction = FLIP_NONE;
    RSDK.SetPaletteEntry(0, 128, 0x000000);
    RSDK.SetPaletteEntry(1, 128, 0x000000);
}

void LaundroMobile_StateBomb_Spawner(void)
{
    RSDK_THIS(LaundroMobile);

    if (self->active == ACTIVE_BOUNDS) {
        if (self->position.x + 0x200000 > ScreenInfo->position.x << 16) {
            if (self->position.x - 0x200000 <= (ScreenInfo->position.x + ScreenInfo->size.x) << 16) {
                self->visible             = false;
                EntityLaundroMobile *bomb = CREATE_ENTITY(LaundroMobile, INT_TO_VOID(LAUNDROMOBILE_BOMB), self->position.x, self->position.y);
                bomb->velocity.x          = LaundroMobile->currentVelocity - 0x20000;
                bomb->active              = ACTIVE_NORMAL;
                bomb->state               = LaundroMobile_StateBomb_Bomb_Idle;
                bomb->isPermanent         = true;
                self->active              = ACTIVE_NORMAL;
            }
        }
    }
    else {
        if (self->position.x + 0x200000 < ScreenInfo->position.x << 16) {
            self->visible = true;
            self->active  = ACTIVE_BOUNDS;
        }
    }
}

void LaundroMobile_StateBomb_Bomb_Idle(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        int32 velX = player->velocity.x;

        if (Player_CheckCollisionBox(player, self, &LaundroMobile->hitboxBox) == C_LEFT) {
            RSDK.PlaySfx(LaundroMobile->sfxButton, false, 255);
            self->velocity.x = LaundroMobile->currentVelocity + 0x18000;
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 8, &self->mainAnimator, true, 0);
            self->state = LaundroMobile_StateBomb_Bomb_Activated;
        }

        player->velocity.x = velX;
    }

    if (self->position.x + 0x200000 < ScreenInfo->position.x << 16) {
        destroyEntity(self);
    }
    else {
        foreach_active(LaundroMobile, boss)
        {
            if (boss->type == LAUNDROMOBILE_BOSS
                && RSDK.CheckObjectCollisionTouchBox(boss, &LaundroMobile->hitboxBoss, self, &LaundroMobile->hitboxBox)
                && boss->state == LaundroMobile_StateBoss_Explode_Phase1) {
                RSDK.PlaySfx(LaundroMobile->sfxPimPom, false, 255);

                EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 7, &debris->animator, true, 0);
                debris->velocity.y      = -0x28000;
                debris->velocity.x      = LaundroMobile->currentVelocity + 0x28000;
                debris->gravityStrength = 0x3800;
                debris->drawGroup       = Zone->objectDrawGroup[1];
                debris->updateRange.x   = 0x400000;
                debris->updateRange.y   = 0x400000;

                destroyEntity(self);
                foreach_break;
            }
        }
    }
}

void LaundroMobile_StateBomb_Bomb_Activated(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.x += self->velocity.x;
    self->velocity.x -= 0x800;

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->position.x + 0x200000 < ScreenInfo->position.x << 16) {
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            int32 velX = player->velocity.x;
            Player_CheckCollisionBox(player, self, &LaundroMobile->hitboxBox);
            player->velocity.x = velX;
        }

        EntityLaundroMobile *boss = LaundroMobile->laundroMobile;
        if (RSDK.CheckObjectCollisionTouchBox(boss, &LaundroMobile->hitboxBoss, self, &LaundroMobile->hitboxBox)) {
            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y);
            explosion->drawGroup       = Zone->objectDrawGroup[1];
            explosion->velocity.x      = LaundroMobile->currentVelocity - 0x10000;
            RSDK.PlaySfx(LaundroMobile->sfxExplosion, false, 255);

            EntityWater *water = CREATE_ENTITY(Water, INT_TO_VOID(WATER_BUBBLE), self->position.x, self->position.y);
            water->drawGroup   = Zone->objectDrawGroup[0] + 1;
            water->angle       = 2 * RSDK.Rand(0, 256);
            water->speed       = -0x1400;
            water->velocity.x  = (LaundroMobile->currentVelocity + (LaundroMobile->currentVelocity >> 2)) + (LaundroMobile->currentVelocity >> 1);
            water->bubbleX     = water->position.x;
            water->childPtr    = 0;
            RSDK.SetSpriteAnimation(Water->aniFrames, 3, &water->animator, false, 5);
            water->tileCollisions = TILECOLLISION_DOWN;

            if (!LaundroMobile->invincibilityTimer) {
                --LaundroMobile->health;
                RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 2, &boss->eggmanAnimator, true, 0);
                RSDK.PlaySfx(LaundroMobile->sfxHit, false, 255);
                LaundroMobile->invincibilityTimer = 30;
                if (LaundroMobile->health <= 8) {
                    RSDK.StopSfx(LaundroMobile->sfxFan);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 4, &boss->propellerAnimator, true, 0);
                    boss->timer = 60;
                    boss->state = LaundroMobile_StateBoss_Destroyed_Phase1;
                }
            }
            destroyEntity(self);
        }
    }
}

void LaundroMobile_Draw_Simple(void)
{
    RSDK_THIS(LaundroMobile);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void LaundroMobile_StateBlock_Spawner(void)
{
    RSDK_THIS(LaundroMobile);

    if (self->active == ACTIVE_BOUNDS) {
        if (self->position.x + 0x200000 > ScreenInfo->position.x << 16) {
            if (self->position.x - 0x200000 <= (ScreenInfo->position.x + ScreenInfo->size.x) << 16) {
                self->visible = false;

                EntityLaundroMobile *block = CREATE_ENTITY(LaundroMobile, INT_TO_VOID(self->type), self->position.x, self->position.y);
                block->velocity.x          = LaundroMobile->currentVelocity - 0x20000;
                block->active              = ACTIVE_NORMAL;
                block->state               = LaundroMobile_StateBlock_Block;
                block->isPermanent         = true;

                self->active = ACTIVE_NORMAL;
            }
        }
    }
    else {
        if (self->position.x + 0x200000 < ScreenInfo->position.x << 16) {
            self->visible = true;
            self->active  = ACTIVE_BOUNDS;
        }
    }
}

void LaundroMobile_StateBlock_Block(void)
{
    RSDK_THIS(LaundroMobile);

    self->position.x += self->velocity.x;

    foreach_active(Player, player)
    {
        if (self->type == LAUNDROMOBILE_SPIKES) {
            if (Player_CheckCollisionBox(player, self, &LaundroMobile->hitboxBomb) == C_LEFT) {
                Player_Hurt(player, self);
            }
        }
        else {
            Player_CheckCollisionBox(player, self, &LaundroMobile->hitboxBox);
        }
    }

    if (self->position.x + 0x200000 >= ScreenInfo->position.x << 16) {
        foreach_active(LaundroMobile, laundroMobile)
        {
            if (laundroMobile->type == LAUNDROMOBILE_BOSS) {
                Hitbox *hitbox = &LaundroMobile->hitboxBomb;
                if (self->type != LAUNDROMOBILE_SPIKES)
                    hitbox = &LaundroMobile->hitboxBox;

                if (RSDK.CheckObjectCollisionTouchBox(laundroMobile, &LaundroMobile->hitboxBoss, self, hitbox)
                    || laundroMobile->state == LaundroMobile_StateBoss_Explode_Phase1) {
                    RSDK.PlaySfx(LaundroMobile->sfxLedgeBreak, false, 255);

                    EntityDebris *debris = NULL;
                    if (self->type == LAUNDROMOBILE_SPIKES) {
                        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y - 0x80000);
                        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 18);
                        debris->drawFX |= FX_ROTATE;
                        debris->velocity.x      = LaundroMobile->currentVelocity + 0x18000;
                        debris->velocity.y      = -0x28000;
                        debris->gravityStrength = 0x3800;
                        debris->rotSpeed        = RSDK.Rand(-8, 8);
                        debris->drawGroup       = Zone->objectDrawGroup[1];
                        debris->updateRange.x   = 0x400000;
                        debris->updateRange.y   = 0x400000;

                        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y + 0x80000);
                        RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 18);
                        debris->drawFX |= FX_ROTATE;
                        debris->velocity.x      = LaundroMobile->currentVelocity + 0x18000;
                        debris->velocity.y      = -0x20000;
                        debris->gravityStrength = 0x3800;
                        debris->rotSpeed        = RSDK.Rand(-8, 8);
                        debris->drawGroup       = Zone->objectDrawGroup[1];
                        debris->updateRange.x   = 0x400000;
                        debris->updateRange.y   = 0x400000;
                        self->position.x += 0x100000;
                    }

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (self->mainAnimator.frameID % 3) + 6);
                    debris->velocity.y      = -0x28000;
                    debris->velocity.x      = LaundroMobile->currentVelocity + 0x20000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (self->mainAnimator.frameID % 3) + 7);
                    debris->velocity.y      = -0x28000;
                    debris->velocity.x      = LaundroMobile->currentVelocity + 0x28000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (self->mainAnimator.frameID % 3) + 8);
                    debris->velocity.y      = -0x20000;
                    debris->velocity.x      = LaundroMobile->currentVelocity + 0x20000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
                    RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &debris->animator, true, 4 * (self->mainAnimator.frameID % 3) + 9);
                    debris->velocity.y      = -0x20000;
                    debris->velocity.x      = LaundroMobile->currentVelocity + 0x28000;
                    debris->gravityStrength = 0x3800;
                    debris->drawGroup       = Zone->objectDrawGroup[1];
                    debris->updateRange.x   = 0x400000;
                    debris->updateRange.y   = 0x400000;

                    destroyEntity(self);
                    foreach_break;
                }
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void LaundroMobile_State_Laundry(void)
{
    RSDK_THIS(LaundroMobile);

    self->propellerAnimator.speed = LaundroMobile->animSpeed;
    self->eggmanAnimator.speed    = LaundroMobile->animSpeed;

    RSDK.ProcessAnimation(&self->propellerAnimator);
    RSDK.ProcessAnimation(&self->eggmanAnimator);

    Zone->playerBoundActiveL[0] = true;
    Zone->playerBoundActiveR[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;
    Zone->cameraBoundsR[0]      = ScreenInfo->center.x + (self->position.x >> 16);

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x - 0xC00000)
        Zone->cameraBoundsT[0] = ScreenInfo->position.y;

    if (!LaundroMobile->health && !LaundroMobile->invincibilityTimer) {
        Debris_CreateFromEntries_UseOffset(LaundroMobile->aniFrames, LaundroMobile->debrisInfo);
        destroyEntity(self);
    }
}

void LaundroMobile_Draw_Laundry(void)
{
    RSDK_THIS(LaundroMobile);

    Vector2 drawPos            = self->position;
    self->mainAnimator.frameID = 2;
    RSDK.DrawSprite(&self->mainAnimator, 0, false);
    RSDK.DrawSprite(&self->propellerAnimator, 0, false);

    drawPos.y += 0x200000;
    RSDK.DrawSprite(&self->propellerAnimator, &drawPos, false);

    drawPos.y += 0x200000;
    RSDK.DrawSprite(&self->propellerAnimator, &drawPos, false);

    self->mainAnimator.frameID = 3;
    RSDK.DrawSprite(&self->mainAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->eggmanAnimator, &drawPos, false);
}

void LaundroMobile_State_DelayedSplash(void)
{
    RSDK_THIS(LaundroMobile);

    if (--self->timer <= 0) {
        CREATE_ENTITY(Water, INT_TO_VOID(WATER_SPLASH), self->position.x, Water->waterLevel);
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void LaundroMobile_EditorDraw(void)
{
    RSDK_THIS(LaundroMobile);

    switch (self->type) {
        case LAUNDROMOBILE_BOSS:
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 3, &self->propellerAnimator, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->eggmanFrames, 0, &self->eggmanAnimator, true, 0);

            self->startY        = self->position.y;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            self->stateDraw     = LaundroMobile_Draw_Boss;
            break;

        case LAUNDROMOBILE_BOMB:
            self->originPos.x = self->position.x;
            self->originPos.y = self->position.y;

            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 7, &self->mainAnimator, true, 0);

            self->updateRange.x = 0x1800000;
            self->updateRange.y = 0x1800000;
            self->stateDraw     = LaundroMobile_Draw_Simple;
            break;

        case LAUNDROMOBILE_LAUNDRY:
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 1, &self->propellerAnimator, true, 0);
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 2, &self->eggmanAnimator, true, 0);

            self->updateRange.x = 0x2000000;
            self->updateRange.y = 0x800000;
            self->stateDraw     = LaundroMobile_Draw_Laundry;
            break;

        case LAUNDROMOBILE_LOOPPOINT:
            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 0, &self->mainAnimator, true, 0);

            RSDK.DrawSprite(&self->mainAnimator, NULL, false);
            self->stateDraw = StateMachine_None;
            break;

        case LAUNDROMOBILE_BLOCK:
            self->originPos = self->position;

            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &self->mainAnimator, false, 0);

            self->updateRange.x = 0x1800000;
            self->updateRange.y = 0x1800000;
            self->stateDraw     = LaundroMobile_Draw_Simple;
            break;

        case LAUNDROMOBILE_SPIKES:
            self->originPos = self->position;

            RSDK.SetSpriteAnimation(LaundroMobile->aniFrames, 9, &self->mainAnimator, false, 3);

            self->updateRange.x = 0x1800000;
            self->updateRange.y = 0x1800000;
            self->stateDraw     = LaundroMobile_Draw_Simple;
            break;

        default: break;
    }

    StateMachine_Run(self->stateDraw);
}

void LaundroMobile_EditorLoad(void)
{
    LaundroMobile->aniFrames    = RSDK.LoadSpriteAnimation("HCZ/LaundroMobile.bin", SCOPE_STAGE);
    LaundroMobile->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanHCZ2.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(LaundroMobile, type);
    RSDK_ENUM_VAR("Boss", LAUNDROMOBILE_BOSS);
    RSDK_ENUM_VAR("Bomb", LAUNDROMOBILE_BOMB);
    RSDK_ENUM_VAR("Laundry", LAUNDROMOBILE_LAUNDRY);
    RSDK_ENUM_VAR("Loop Point", LAUNDROMOBILE_LOOPPOINT);
    RSDK_ENUM_VAR("Block", LAUNDROMOBILE_BLOCK);
    RSDK_ENUM_VAR("Spikes", LAUNDROMOBILE_SPIKES);
}
#endif

void LaundroMobile_Serialize(void) { RSDK_EDITABLE_VAR(LaundroMobile, VAR_UINT8, type); }
