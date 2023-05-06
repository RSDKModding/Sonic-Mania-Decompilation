// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: HeavyRider Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectHeavyRider *HeavyRider;

void HeavyRider_Update(void)
{
    RSDK_THIS(HeavyRider);

    StateMachine_Run(self->state);
}

void HeavyRider_LateUpdate(void) {}

void HeavyRider_StaticUpdate(void) {}

void HeavyRider_Draw(void)
{
    RSDK_THIS(HeavyRider);

    StateMachine_Run(self->stateDraw);
}

void HeavyRider_Create(void *data)
{
    RSDK_THIS(HeavyRider);

    self->drawFX = FX_ROTATE | FX_FLIP;

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            if (!self->type)
                self->type = VOID_TO_INT(data);

            switch (self->type) {
                case HEAVYRIDER_RIDER:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[1];

                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &self->thrustAnimator, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &self->spikeBallAnimator, true, 0);
                    RSDK.SetSpriteAnimation(-1, 0, &self->fireballAnimator, true, 0);

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    HeavyRider->spikeBallAngle   = 0x80;
                    HeavyRider->spikeBallState   = HEAVYRIDER_SPIKEBALL_SWINGING;
                    HeavyRider->spawnDelay       = 16;
                    HeavyRider->spawnFireballs   = false;
                    HeavyRider->startY           = self->position.y;
                    HeavyRider->curAttack        = HEAVYRIDER_ATK_RIDING_SCREENBOUNDS;
                    HeavyRider->wheelExtendState = HEAVYRIDER_WHEEL_NONE;
                    HeavyRider->wheelLength      = 0;

                    self->state     = HeavyRider_State_SetupArena;
                    self->stateDraw = HeavyRider_Draw_Boss;
                    break;

                case HEAVYRIDER_PLANESWITCH:
                    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->mainAnimator, true, 0);

                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = (self->size + 0x400) << 11;
                    self->visible       = false;
                    self->drawGroup     = Zone->objectDrawGroup[0];

                    self->state     = HeavyRider_State_PlaneSwitch;
                    self->stateDraw = HeavyRider_Draw_PlaneSwitch;
                    break;

                case HEAVYRIDER_FORCECHARGE_LEFT:
                case HEAVYRIDER_FORCECHARGE_RIGHT:
                case HEAVYRIDER_UNUSED1:
                case HEAVYRIDER_UNUSED2:
                    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->mainAnimator, true, ((self->type - 2) >> 1) + 4);

                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = 0x200000;
                    self->visible       = false;
                    self->drawGroup     = Zone->objectDrawGroup[0];

                    self->state     = HeavyRider_State_ChargeTrigger;
                    self->stateDraw = HeavyRider_Draw_Simple;
                    break;

                case HEAVYRIDER_PUFF:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 10, &self->mainAnimator, true, 0);

                    self->active        = ACTIVE_NORMAL;
                    self->updateRange.x = 0x100000;
                    self->updateRange.y = 0x100000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[1] - 1;

                    self->state     = HeavyRider_State_Puff;
                    self->stateDraw = HeavyRider_Draw_Simple;
                    break;

                case HEAVYRIDER_FIREBALL:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 9, &self->mainAnimator, true, 0);

                    self->active        = ACTIVE_NORMAL;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = 0x200000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[1];

                    self->state     = HeavyRider_State_Fireball;
                    self->stateDraw = HeavyRider_Draw_Simple;
                    break;

                case HEAVYRIDER_JIMMY:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 4, &self->mainAnimator, true, 0);

                    self->drawGroup     = Zone->objectDrawGroup[1];
                    self->active        = ACTIVE_NORMAL;
                    self->updateRange.x = 0x1800000;
                    self->updateRange.y = 0x1800000;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[1];

                    self->state     = HeavyRider_StateJimmy_Idle;
                    self->stateDraw = HeavyRider_Draw_Simple;
                    break;

                default: break;
            }
        }
        else {
            destroyEntity(self);
        }
    }
}

void HeavyRider_StageLoad(void)
{
    HeavyRider->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/HeavyRider.bin", SCOPE_STAGE);

    HeavyRider->hitboxRider.left   = -16;
    HeavyRider->hitboxRider.top    = -25;
    HeavyRider->hitboxRider.right  = 16;
    HeavyRider->hitboxRider.bottom = 14;

    HeavyRider->hitboxSpikeBall.left   = -20;
    HeavyRider->hitboxSpikeBall.top    = -20;
    HeavyRider->hitboxSpikeBall.right  = 20;
    HeavyRider->hitboxSpikeBall.bottom = 20;

    HeavyRider->outerBox.left   = -5;
    HeavyRider->outerBox.top    = 0;
    HeavyRider->outerBox.right  = 5;
    HeavyRider->outerBox.bottom = 48;

    HeavyRider->innerBox.left   = -4;
    HeavyRider->innerBox.top    = 0;
    HeavyRider->innerBox.right  = 4;
    HeavyRider->innerBox.bottom = 48;

    HeavyRider->hitboxRiderTrigger.left   = -4;
    HeavyRider->hitboxRiderTrigger.top    = 46;
    HeavyRider->hitboxRiderTrigger.right  = 4;
    HeavyRider->hitboxRiderTrigger.bottom = 48;

    HeavyRider->hitboxTrigger.left   = -8;
    HeavyRider->hitboxTrigger.top    = -8;
    HeavyRider->hitboxTrigger.right  = 8;
    HeavyRider->hitboxTrigger.bottom = 8;

    HeavyRider->hitboxJimmy.left   = -10;
    HeavyRider->hitboxJimmy.top    = 14;
    HeavyRider->hitboxJimmy.right  = 22;
    HeavyRider->hitboxJimmy.bottom = 46;

    HeavyRider->hitboxFireball.left   = -2;
    HeavyRider->hitboxFireball.top    = 38;
    HeavyRider->hitboxFireball.right  = 14;
    HeavyRider->hitboxFireball.bottom = 46;

    HeavyRider->health             = 8;
    HeavyRider->invincibilityTimer = 0;

    HeavyRider->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyRider->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyRider->sfxCharge    = RSDK.GetSfx("LRZ/RiderCharge.wav");
    HeavyRider->sfxLaunch    = RSDK.GetSfx("LRZ/RiderLaunch.wav");
    HeavyRider->sfxSkid      = RSDK.GetSfx("LRZ/RiderSkid.wav");
    HeavyRider->sfxJump      = RSDK.GetSfx("LRZ/RiderJump.wav");
    HeavyRider->sfxCheer     = RSDK.GetSfx("LRZ/RiderCheer.wav");
    HeavyRider->sfxBumper    = RSDK.GetSfx("Stage/Bumper3.wav");
}

void HeavyRider_SpawnDebris(int32 frame, uint8 drawGroup, int32 x, int32 y)
{
    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, x, y);
    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &debris->animator, true, frame);

    debris->velocity.x      = RSDK.Rand(-0x20000, 0x20000);
    debris->velocity.y      = RSDK.Rand(-0x20000, -0x10000);
    debris->drawGroup       = drawGroup;
    debris->gravityStrength = 0x4800;
    debris->updateRange.x   = 0x400000;
    debris->updateRange.y   = 0x400000;
}

void HeavyRider_CheckObjectCollisions(void)
{
    RSDK_THIS(HeavyRider);

    if (HeavyRider->invincibilityTimer) {
        HeavyRider->invincibilityTimer--;
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (HeavyRider->playerTimers[playerID])
                HeavyRider->playerTimers[playerID]--;

            if (Player_CheckBadnikTouch(player, self, &HeavyRider->hitboxRider)) {
                if (Player_CheckBossHit(player, self)) {
                    HeavyRider_Hit();
                    foreach_break;
                }
            }
            else if (Player_CheckBadnikTouch(player, self, &HeavyRider->hitboxJimmy)) {
                if (!HeavyRider->playerTimers[playerID] && Player_CheckBossHit(player, self)) {
                    RSDK.PlaySfx(HeavyRider->sfxBumper, false, 255);

                    int32 angle = RSDK.ATan2(player->position.x - self->position.x - 0x60000, player->position.y - self->position.y - 0x1E0000);
                    player->velocity.x = 0x380 * RSDK.Cos256(angle);

                    if (player->position.x >= self->position.x) {
                        if (player->position.x > self->position.x) {
                            if (self->velocity.x > 0)
                                player->velocity.x += self->velocity.x;
                        }
                    }
                    else {
                        if (self->velocity.x >= 0) {
                            if (player->position.x > self->position.x) {
                                if (self->velocity.x > 0)
                                    player->velocity.x += self->velocity.x;
                            }
                        }
                        else {
                            player->velocity.x += self->velocity.x;
                        }
                    }

                    player->velocity.y = 0x380 * RSDK.Sin256(angle);
                    if (player->velocity.y >= 0)
                        player->groundVel = player->velocity.x;
                    else
                        player->onGround = false;

                    player->applyJumpCap               = false;
                    HeavyRider->playerTimers[playerID] = 15;
                }
            }
            else if ((uint32)(HeavyRider->spikeBallAngle & 0x7F) + 32 < 0x40) {
                int32 storeX     = self->position.x;
                int32 storeY     = self->position.y;
                self->position.x = HeavyRider->spikeBallPos.x;
                self->position.y = HeavyRider->spikeBallPos.y;

                if (Player_CheckCollisionTouch(player, self, &HeavyRider->hitboxSpikeBall)) {
#if MANIA_USE_PLUS
                    if (!Player_CheckMightyUnspin(player, 0x800, false, &player->uncurlTimer))
#endif
                        Player_Hurt(player, self);
                }

                self->position.x = storeX;
                self->position.y = storeY;
            }
        }
    }

    if (HeavyRider->curAttack == HEAVYRIDER_ATK_RIDING_ARENABOUNDS || HeavyRider->curAttack == HEAVYRIDER_ATK_RIDING_USETRIGGERS) {
        foreach_all(HeavyRider, rider)
        {
            if (rider->type == HEAVYRIDER_PLANESWITCH) {
                if (abs(self->position.x - rider->position.x) < 0x100000 && abs(self->position.y - rider->position.y) < rider->size << 19) {
                    if (self->velocity.x >= 0)
                        self->collisionPlane = (rider->flags >> 3) & 1;
                    else
                        self->collisionPlane = (rider->flags >> 1) & 1;
                }
            }
            else {
                if (HeavyRider->curAttack == HEAVYRIDER_ATK_RIDING_USETRIGGERS && self->onGround) {
                    switch (rider->type) {
                        default: break;

                        case HEAVYRIDER_FORCECHARGE_LEFT:
                            if (RSDK.CheckObjectCollisionTouchBox(rider, &HeavyRider->hitboxTrigger, self, &HeavyRider->hitboxRiderTrigger)) {
                                if (self->direction == FLIP_X) {
                                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
                                    RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);
                                    self->state = HeavyRider_State_Turning;
                                }
                                else if (self->state != HeavyRider_State_RampJump) {
                                    self->timer             = 38;
                                    HeavyRider->chargeTimer = 0;
                                    self->state             = HeavyRider_State_RampJump;
                                }
                            }
                            break;

                        case HEAVYRIDER_FORCECHARGE_RIGHT:
                            if (RSDK.CheckObjectCollisionTouchBox(rider, &HeavyRider->hitboxTrigger, self, &HeavyRider->hitboxRiderTrigger)) {
                                if (self->direction == FLIP_NONE) {
                                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
                                    RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);
                                    self->state = HeavyRider_State_Turning;
                                }
                                else if (self->state != HeavyRider_State_RampJump) {
                                    self->timer             = 38;
                                    HeavyRider->chargeTimer = 0;
                                    self->state             = HeavyRider_State_RampJump;
                                }
                            }
                            break;
                    }
                }
            }
        }

        foreach_all(PlaneSwitch, planeSwitch) { PlaneSwitch_CheckCollisions(planeSwitch, self, planeSwitch->flags, planeSwitch->size, false, 0, 0); }
    }
}

void HeavyRider_Hit(void)
{
    RSDK_THIS(HeavyRider);

    if (HeavyRider->health)
        HeavyRider->health--;

    if (!HeavyRider->health) {
        RSDK.PlaySfx(HeavyRider->sfxExplosion, false, 255);
        self->timer = 120;

        if (HeavyRider->spikeBallState == HEAVYRIDER_SPIKEBALL_SWINGING) {
            int32 spawnX = 0;
            if (self->direction) {
                if (self->mainAnimator.animationID == 2 && self->mainAnimator.frameID)
                    spawnX = self->position.x + 0x160000;
                else
                    spawnX = self->position.x - 0xE0000;
            }
            else {
                if (self->mainAnimator.animationID == 2 && self->mainAnimator.frameID)
                    spawnX = self->position.x - 0x160000;
                else
                    spawnX = self->position.x + 0xE0000;
            }

            int32 angle = 0x400;
            int32 cos   = RSDK.Cos256(HeavyRider->spikeBallAngle);
            for (int32 i = 0; i < 8; ++i) {
                HeavyRider_SpawnDebris(1, Zone->objectDrawGroup[1], spawnX + angle * cos, self->position.y - 0x210000);
                angle += 0x800;
            }

            HeavyRider_SpawnDebris(2, Zone->objectDrawGroup[1], spawnX + angle * cos, self->position.y - 0x210000);
            HeavyRider->spikeBallState = HEAVYRIDER_SPIKEBALL_NONE;

            RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Flamethrower)->interval = -1;
            RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Flamethrower)->interval = -1;

            HeavyRider->spawnFireballs = false;
            RSDK.SetSpriteAnimation(-1, 0, &self->fireballAnimator, true, 0);
        }
        else if (HeavyRider->spikeBallState == HEAVYRIDER_SPIKEBALL_THROWN) {
            HeavyRider_SpawnDebris(2, Zone->objectDrawGroup[1], HeavyRider->spikeBallPos.x, HeavyRider->spikeBallPos.y);
            HeavyRider->spikeBallState = HEAVYRIDER_SPIKEBALL_NONE;
        }

        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->state = HeavyRider_State_Destroyed;
    }

    RSDK.PlaySfx(HeavyRider->sfxHit, false, 255);
    self->timer -= 60;
    HeavyRider->invincibilityTimer = 30;
}

void HeavyRider_Explode(void)
{
    RSDK_THIS(HeavyRider);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyRider->sfxExplosion, false, 255);

        if (!(Zone->timer & 0xF)) {
            int32 x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y = self->position.y + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void HeavyRider_HandleTurn_ScreenEdges(void)
{
    RSDK_THIS(HeavyRider);

    if (self->direction == FLIP_X) {
        if (self->position.x < (ScreenInfo->position.x + 48) << 16) {
            HeavyRider_DecideNextAttack();

            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);

            self->state = HeavyRider_State_Turning;
        }
    }
    else {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 48) << 16) {
            HeavyRider_DecideNextAttack();

            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);

            self->state = HeavyRider_State_Turning;
        }
    }
}

void HeavyRider_HandleTurn_ArenaEdges(void)
{
    RSDK_THIS(HeavyRider);

    if (self->direction == FLIP_X) {
        if (self->position.x < (Zone->cameraBoundsL[0] - 48) << 16) {
            HeavyRider_DecideNextAttack();

            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);

            self->state = HeavyRider_State_Turning;
        }
    }
    else {
        if (self->position.x > (Zone->cameraBoundsR[0] + 48) << 16) {
            HeavyRider_DecideNextAttack();

            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);

            self->state = HeavyRider_State_Turning;
        }
    }
}

void HeavyRider_DecideNextAttack(void)
{
    RSDK_THIS(HeavyRider);

    if (HeavyRider->curAttack != HEAVYRIDER_ATK_RIDING_USETRIGGERS) {
        HeavyRider->spawnFireballs = 0;
        RSDK.SetSpriteAnimation(-1, 0, &self->fireballAnimator, true, 0);
    }

    int32 atkID = RSDK.Rand(0, 10);
    switch (HeavyRider->curAttack) {
        case HEAVYRIDER_ATK_RIDING_SCREENBOUNDS: {
            int32 nextAttacks[] = {
                HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_ARENABOUNDS,
                HEAVYRIDER_ATK_RIDING_ARENABOUNDS,  HEAVYRIDER_ATK_RIDING_ARENABOUNDS,  HEAVYRIDER_ATK_RIDING_ARENABOUNDS,
                HEAVYRIDER_ATK_RIDING_USETRIGGERS,  HEAVYRIDER_ATK_RIDING_USETRIGGERS,  HEAVYRIDER_ATK_RIDING_USETRIGGERS,
                HEAVYRIDER_ATK_RIDING_USETRIGGERS,
            };

            HeavyRider->curAttack = nextAttacks[atkID];
            break;
        }

        case HEAVYRIDER_ATK_RIDING_ARENABOUNDS: {
            int32 nextAttacks[] = {
                HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_ARENABOUNDS,
                HEAVYRIDER_ATK_RIDING_USETRIGGERS,  HEAVYRIDER_ATK_RIDING_USETRIGGERS,  HEAVYRIDER_ATK_RIDING_USETRIGGERS,
                HEAVYRIDER_ATK_PREPCHARGE,          HEAVYRIDER_ATK_PREPCHARGE,          HEAVYRIDER_ATK_PREPCHARGE,
                HEAVYRIDER_ATK_PREPCHARGE,
            };

            HeavyRider->curAttack = nextAttacks[atkID];
            break;
        }

        case HEAVYRIDER_ATK_RIDING_USETRIGGERS: {
            int32 nextAttacks[] = {
                HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_ARENABOUNDS,
                HEAVYRIDER_ATK_RIDING_ARENABOUNDS,  HEAVYRIDER_ATK_RIDING_ARENABOUNDS,  HEAVYRIDER_ATK_RIDING_USETRIGGERS,
                HEAVYRIDER_ATK_PREPCHARGE,          HEAVYRIDER_ATK_PREPCHARGE,          HEAVYRIDER_ATK_PREPCHARGE,
                HEAVYRIDER_ATK_PREPCHARGE,
            };

            HeavyRider->curAttack = nextAttacks[atkID];

            if (HeavyRider->curAttack != HEAVYRIDER_ATK_PREPCHARGE) {
                HeavyRider->spawnFireballs = false;
                RSDK.SetSpriteAnimation(-1, 0, &self->fireballAnimator, true, 0);
            }
            break;
        }

        case HEAVYRIDER_ATK_PREPCHARGE:
        case HEAVYRIDER_ATK_CHARGE: {
            int32 nextAttack[] = {
                HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_SCREENBOUNDS, HEAVYRIDER_ATK_RIDING_SCREENBOUNDS,
                HEAVYRIDER_ATK_PREPCHARGE,          HEAVYRIDER_ATK_PREPCHARGE,          HEAVYRIDER_ATK_RIDING_ARENABOUNDS,
                HEAVYRIDER_ATK_RIDING_ARENABOUNDS,  HEAVYRIDER_ATK_RIDING_ARENABOUNDS,  HEAVYRIDER_ATK_RIDING_USETRIGGERS,
                HEAVYRIDER_ATK_RIDING_USETRIGGERS,
            };

            HeavyRider->curAttack = nextAttack[atkID];
            break;
        }

        default: break;
    }
}

void HeavyRider_State_SetupArena(void)
{
    RSDK_THIS(HeavyRider);

    if (++self->timer >= 2) {
        self->timer                 = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + WIDE_SCR_XSIZE;
        self->active                = ACTIVE_NORMAL;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
        self->state = HeavyRider_State_AwaitPlayer;
    }
}

void HeavyRider_State_AwaitPlayer(void)
{
    RSDK_THIS(HeavyRider);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > self->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - WIDE_SCR_XSIZE;
#if MANIA_USE_PLUS
        HeavyRider->outro = CREATE_ENTITY(LRZ3Outro, NULL, self->position.x, self->position.y);
#endif
        self->state = HeavyRider_State_SetupRider;
    }
}

void HeavyRider_State_SetupRider(void)
{
    RSDK_THIS(HeavyRider);

    Zone->playerBoundActiveL[0] = true;

#if MANIA_USE_PLUS
    EntityLRZ3Outro *outro = HeavyRider->outro;
    if (outro->state == LRZ3Outro_State_EnterLittlePlanet) {
#else
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > self->position.x) {
#endif

        self->position.y -= 0x400000;
        self->position.x      = (Zone->cameraBoundsL[0] - 64) << 16;
        self->onGround        = false;
        self->tileCollisions  = TILECOLLISION_DOWN;
        self->collisionPlane  = 0;
        self->collisionLayers = (1 << Zone->fgLayer[0]) | (1 << Zone->fgLayer[1]);

        Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);

        self->visible    = true;
        self->state      = HeavyRider_State_Moving;
        self->velocity.x = 0;
        self->timer      = 0;
    }
}

void HeavyRider_State_Moving(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);
    RSDK.ProcessAnimation(&self->fireballAnimator);

    RSDK.ProcessObjectMovement(self, &HeavyRider->outerBox, &HeavyRider->innerBox);

    if (self->onGround) {
        if (self->velocity.y <= 0x20000) {
            self->velocity.y = 0;
        }
        else {
            self->velocity.y = (self->velocity.y >> 1) - self->velocity.y;
            if (self->velocity.y > 0) {
                if (RSDK.Rand(0, 2) == 1) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);
                    self->state = HeavyRider_State_Turning;
                }
                self->velocity.y = 0;
            }
            else {
                self->onGround                                                     = false;
                RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Flamethrower)->interval = -1;
                RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Flamethrower)->interval = -1;
            }
        }
    }
    else {
        self->velocity.y += 0x3800;
    }

    if (HeavyRider->wheelExtendState == HEAVYRIDER_WHEEL_EXTENDING) {
        if (HeavyRider->wheelLength < 0x100000)
            HeavyRider->wheelLength += 0x30000;

        if (HeavyRider->wheelLength > 0x100000) {
            HeavyRider->wheelLength      = 0x100000;
            HeavyRider->wheelExtendState = HEAVYRIDER_WHEEL_RETRACTING;
        }
    }
    else if (HeavyRider->wheelExtendState == HEAVYRIDER_WHEEL_RETRACTING) {
        if (HeavyRider->wheelLength > 0)
            HeavyRider->wheelLength -= 0x30000;

        if (HeavyRider->wheelLength < 0) {
            HeavyRider->wheelLength      = 0;
            HeavyRider->wheelExtendState = HEAVYRIDER_WHEEL_NONE;
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->wheelieAnimator, true, 0);
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (self->position.y == HeavyRider->startY && HeavyRider->curAttack != HEAVYRIDER_ATK_CHARGE && player1->onGround) {
        int32 dist = 0x7FFFFFFF;
        if (self->velocity.x <= 0) {
            if (player1->position.x < self->position.x) {
                dist = self->position.x - player1->position.x;

                if (player1->velocity.x > 0)
                    dist -= 8 * player1->velocity.x;
            }
        }
        else {
            if (player1->position.x > self->position.x) {
                dist = player1->position.x - self->position.x;

                if (player1->velocity.x < 0)
                    dist += 8 * player1->velocity.x;
            }
        }

        if (dist < 0x480000) {
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &self->wheelieAnimator, true, 0);

            RSDK.PlaySfx(HeavyRider->sfxJump, false, 255);
            self->onGround   = false;
            self->velocity.y = -0x58000;

            HeavyRider->wheelExtendState = HEAVYRIDER_WHEEL_EXTENDING;
            HeavyRider->wheelLength      = 0;
        }
    }

    if (self->direction) {
        if (self->groundVel > -0x40000)
            self->groundVel -= 0x4000;
    }
    else {
        if (self->groundVel < 0x40000)
            self->groundVel += 0x4000;
    }

    switch (HeavyRider->curAttack) {
        case HEAVYRIDER_ATK_RIDING_SCREENBOUNDS: HeavyRider_HandleTurn_ScreenEdges(); break;

        case HEAVYRIDER_ATK_RIDING_ARENABOUNDS:
        case HEAVYRIDER_ATK_RIDING_USETRIGGERS:
        case HEAVYRIDER_ATK_CHARGE: HeavyRider_HandleTurn_ArenaEdges(); break;

        case HEAVYRIDER_ATK_PREPCHARGE:
            HeavyRider_HandleTurn_ArenaEdges();

            HeavyRider->curAttack      = HEAVYRIDER_ATK_CHARGE;
            HeavyRider->spikeBallState = HEAVYRIDER_SPIKEBALL_THROWN;

            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 1, &self->mainAnimator, true, 2);

            self->timer             = 41;
            HeavyRider->chargeTimer = 0;
            self->state             = HeavyRider_State_Charging;
            break;

        default: break;
    }

    if (self->mainAnimator.animationID != 2) {
        if (abs(self->groundVel) > 0x48000 || HeavyRider->spawnFireballs) {
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &self->thrustAnimator, false, 0);

            if (HeavyRider->spawnFireballs && self->onGround) {
                if (!--HeavyRider->spawnDelay) {
                    HeavyRider->spawnDelay = 10;
                    CREATE_ENTITY(HeavyRider, INT_TO_VOID(HEAVYRIDER_FIREBALL), self->position.x, self->position.y);
                }
            }
        }
        else {
            RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);

            if (HeavyRider->spawnDelay > 16)
                HeavyRider->spawnDelay = 16;

            if (!--HeavyRider->spawnDelay) {
                HeavyRider->spawnDelay  = 16;
                EntityHeavyRider *child = CREATE_ENTITY(HeavyRider, INT_TO_VOID(HEAVYRIDER_PUFF), self->position.x, self->position.y + 0x1C0000);
                if (self->direction == FLIP_X)
                    child->position.x += 0x110000;
                else
                    child->position.x -= 0x110000;
            }
        }
    }

    if (HeavyRider->spikeBallState == HEAVYRIDER_SPIKEBALL_SWINGING)
        HeavyRider->spikeBallAngle += 6;

    if (HeavyRider->spikeBallState == HEAVYRIDER_SPIKEBALL_THROWN) {
        HeavyRider->spikeBallAngle = 0;
        HeavyRider->spikeBallPos.x = self->position.x;
        HeavyRider->spikeBallPos.y = self->position.y;

        if (self->direction)
            HeavyRider->spikeBallPos.x -= 0x200000;
        else
            HeavyRider->spikeBallPos.x += 0x200000;

        HeavyRider->spikeBallPos.y += 0xC0000;
    }

    HeavyRider_CheckObjectCollisions();
}

void HeavyRider_State_RampJump(void)
{
    RSDK_THIS(HeavyRider);

    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    HeavyRider_State_Moving();

    if (self->type != HEAVYRIDER_JIMMY && self->timer == 30) {
        RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Flamethrower)->interval = 0;
        RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Flamethrower)->interval = 0;
    }

    if (--self->timer > 0) {
        if (!(self->timer & 7)) {
            int32 channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        self->timer = 30;
        self->groundVel += (self->groundVel >> 2) + (self->groundVel >> 1);
        if (self->type == HEAVYRIDER_JIMMY)
            self->state = HeavyRider_State_Moving;
        else
            self->state = HeavyRider_State_ChargeDash;
    }
}

void HeavyRider_State_Charging(void)
{
    RSDK_THIS(HeavyRider);

    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    HeavyRider_State_Moving();

    if (--self->timer > 0) {
        if (!(self->timer & 7)) {
            int32 channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        self->timer = 30;
        self->groundVel += (self->groundVel >> 2) + (self->groundVel >> 1);
        self->state = HeavyRider_State_Moving;
    }
}

void HeavyRider_State_ChargeDash(void)
{
    RSDK_THIS(HeavyRider);

    HeavyRider_State_Moving();

    if (--self->timer == 16) {
        HeavyRider->spawnDelay     = 10;
        HeavyRider->spawnFireballs = true;
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 8, &self->fireballAnimator, true, 0);
    }
    else if (self->timer <= 0) {
        for (int32 i = 0; i < 0x60; ++i) {
            int32 x              = RSDK.Rand(-128, 129);
            int32 y              = 2 * RSDK.Rand(4, 16);
            EntityDebris *debris = CREATE_ENTITY(Debris, NULL, (x << 16) + self->position.x, (ScreenInfo->position.y - y) << 16);

            debris->updateRange.x   = 0x2000000;
            debris->updateRange.y   = 0x2000000;
            debris->state           = Debris_State_Fall;
            debris->gravityStrength = 0x1800;
            debris->velocity.x      = RSDK.Rand(-2, 3) << 16;
            debris->velocity.y      = RSDK.Rand(-2, 2) << 16;
            debris->drawFX          = FX_FLIP;
            debris->direction       = i & 3;
            debris->drawGroup       = Zone->objectDrawGroup[1];

            int32 frame = RSDK.Rand(0, 4);
            int32 anim  = RSDK.Rand(12, 15);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, anim, &debris->animator, true, frame);
        }

        RSDK.PlaySfx(HeavyRider->sfxCheer, false, 255);
        self->state = HeavyRider_State_Moving;
    }
}

void HeavyRider_State_Turning(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->fireballAnimator);

    RSDK.ProcessObjectMovement(self, &HeavyRider->outerBox, &HeavyRider->innerBox);

    if (self->onGround) {
        if (self->velocity.y <= 0x20000) {
            if (!(Zone->timer & 3))
                RSDK.PlaySfx(HeavyRider->sfxSkid, false, 255);

            self->velocity.y = 0;
        }
        else {
            self->velocity.y = (self->velocity.y >> 1) - self->velocity.y;

            if ((self->velocity.y >> 1) - self->velocity.y <= 0)
                self->onGround = false;
            else
                self->velocity.y = 0;
        }
    }
    else {
        self->velocity.y += 0x3800;
    }

    if (self->direction) {
        if (self->groundVel > -0x40000)
            self->groundVel -= 0x4000;
    }
    else {
        if (self->groundVel < 0x40000)
            self->groundVel += 0x4000;
    }

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->mainAnimator, true, 0);
        self->direction ^= FLIP_X;
        HeavyRider->spikeBallState = HEAVYRIDER_SPIKEBALL_SWINGING;
        self->state                = HeavyRider_State_Moving;
    }

    HeavyRider->spikeBallAngle += 4;

    HeavyRider_CheckObjectCollisions();
}

void HeavyRider_State_Destroyed(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    HeavyRider_Explode();

    if (--self->timer <= 0) {
        EntityHeavyRider *child = CREATE_ENTITY(HeavyRider, INT_TO_VOID(HEAVYRIDER_JIMMY), self->position.x, self->position.y);
        child->direction        = self->direction;
        child->onGround         = false;
        child->tileCollisions   = TILECOLLISION_DOWN;
        child->collisionLayers  = (1 << Zone->fgLayer[0]) | (1 << Zone->fgLayer[1]);
        child->collisionPlane   = 0;
        self->velocity.y        = -0x60000;

        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x20000;
        else
            self->velocity.x = 0x20000;

        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 3, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(-1, 0, &self->thrustAnimator, true, 0);

        self->state = HeavyRider_State_Finish;
    }
}

void HeavyRider_State_Finish(void)
{
    RSDK_THIS(HeavyRider);

    if (++self->timer < 45)
        HeavyRider_Explode();

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0)
        self->drawGroup = Zone->objectDrawGroup[1];

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        self->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        self->position.y = (ScreenInfo->position.y - 48) << 16;

        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, INT_TO_VOID(EGGPRISON_FLYING), self->position.x, self->position.y);
        prison->isPermanent     = true;
        prison->drawGroup       = Zone->objectDrawGroup[1];

        destroyEntity(self);
    }
}

void HeavyRider_Draw_Boss(void)
{
    RSDK_THIS(HeavyRider);

    if (HeavyRider->wheelExtendState != HEAVYRIDER_WHEEL_NONE) {
        Vector2 drawPos = self->position;
        drawPos.y += 2 * HeavyRider->wheelLength;
        self->wheelieAnimator.frameID = 3;
        RSDK.DrawSprite(&self->wheelieAnimator, &drawPos, false);

        self->wheelieAnimator.frameID = 2;
        RSDK.DrawSprite(&self->wheelieAnimator, &drawPos, false);

        drawPos.y -= HeavyRider->wheelLength;
        self->wheelieAnimator.frameID = 1;
        RSDK.DrawSprite(&self->wheelieAnimator, &drawPos, false);
    }

    int32 drawX = 0;
    if (self->direction) {
        if (self->mainAnimator.animationID == 2 && self->mainAnimator.frameID)
            drawX = self->position.x + 0x160000;
        else
            drawX = self->position.x - 0xE0000;
    }
    else if (self->mainAnimator.animationID == 2 && self->mainAnimator.frameID)
        drawX = self->position.x - 0x160000;
    else
        drawX = self->position.x + 0xE0000;

    int32 drawY = self->position.y - 0x210000;
    Vector2 drawPos;
    drawPos.x = drawX;
    drawPos.y = drawY;

    if (HeavyRider->spikeBallState != HEAVYRIDER_SPIKEBALL_NONE) {
        if (HeavyRider->spikeBallState <= HEAVYRIDER_SPIKEBALL_UNUSED3) {
            if (HeavyRider->spikeBallAngle >= 0x80) {
                self->spikeBallAnimator.frameID = 2;
                drawPos.x                       = drawX + 0x4400 * RSDK.Cos256(HeavyRider->spikeBallAngle);
                drawPos.y                       = drawY + 0x880 * RSDK.Sin256(2 * HeavyRider->spikeBallAngle);
                self->drawFX |= FX_SCALE;
                self->scale.x = (RSDK.Sin256(HeavyRider->spikeBallAngle) >> 2) + 0x200;
                self->scale.y = (RSDK.Sin256(HeavyRider->spikeBallAngle) >> 2) + 0x200;
                RSDK.DrawSprite(&self->spikeBallAnimator, &drawPos, false);

                self->drawFX &= ~FX_SCALE;
                HeavyRider->spikeBallPos = drawPos;

                self->spikeBallAnimator.frameID = 1;
                int32 angle                     = 0x4400;
                for (int32 i = 0; i < 8; ++i) {
                    angle -= 0x800;
                    drawPos.x = drawX + angle * RSDK.Cos256(HeavyRider->spikeBallAngle);
                    drawPos.y = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->spikeBallAngle);
                    RSDK.DrawSprite(&self->spikeBallAnimator, &drawPos, false);
                }
            }
        }
        else {
            if (HeavyRider->spikeBallState == HEAVYRIDER_SPIKEBALL_THROWN) {
                self->spikeBallAnimator.frameID = 2;
                RSDK.DrawSprite(&self->spikeBallAnimator, &HeavyRider->spikeBallPos, false);
            }
        }
    }

    if (HeavyRider->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);

    RSDK.DrawSprite(&self->thrustAnimator, NULL, false);

    switch (HeavyRider->spikeBallState) {
        default:
        case HEAVYRIDER_SPIKEBALL_NONE:
        case HEAVYRIDER_SPIKEBALL_THROWN: RSDK.DrawSprite(&self->fireballAnimator, NULL, false); break;

        case HEAVYRIDER_SPIKEBALL_SWINGING:
        case HEAVYRIDER_SPIKEBALL_UNUSED1:
        case HEAVYRIDER_SPIKEBALL_UNUSED2:
        case HEAVYRIDER_SPIKEBALL_UNUSED3:
            if (HeavyRider->spikeBallAngle < 0x80) {
                self->spikeBallAnimator.frameID = 1;
                int32 angle                     = 0x400;
                for (int32 i = 0; i < 8; ++i) {
                    angle += 0x800;
                    drawPos.x = drawX + angle * RSDK.Cos256(HeavyRider->spikeBallAngle);
                    drawPos.y = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->spikeBallAngle);
                    RSDK.DrawSprite(&self->spikeBallAnimator, &drawPos, false);
                }

                self->spikeBallAnimator.frameID = 2;
                drawPos.x                       = drawX + angle * RSDK.Cos256(HeavyRider->spikeBallAngle);
                drawPos.y                       = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->spikeBallAngle);
                self->drawFX |= FX_SCALE;
                self->scale.x = (RSDK.Sin256(HeavyRider->spikeBallAngle) >> 2) + 0x200;
                self->scale.y = (RSDK.Sin256(HeavyRider->spikeBallAngle) >> 2) + 0x200;
                RSDK.DrawSprite(&self->spikeBallAnimator, &drawPos, false);

                self->drawFX &= ~FX_SCALE;
                HeavyRider->spikeBallPos = drawPos;
            }

            RSDK.DrawSprite(&self->fireballAnimator, NULL, false);
            break;

        case HEAVYRIDER_SPIKEBALL_UNUSED4:
        case HEAVYRIDER_SPIKEBALL_UNUSED5:
            RSDK.DrawSprite(&self->spikeBallAnimator, &HeavyRider->spikeBallPos, false);
            RSDK.DrawSprite(&self->fireballAnimator, NULL, false);
            break;
    }
}

void HeavyRider_State_PlaneSwitch(void)
{
    RSDK_THIS(HeavyRider);

    if (self->affectPlayer) {
        foreach_active(Player, player)
        {
            if (abs(player->position.x - self->position.x) < 0x100000 && abs(player->position.y - self->position.y) < self->size << 19) {
                if (player->velocity.x >= 0) {
                    player->collisionPlane = (self->flags >> 3) & 1;

                    if (!(self->flags & 4)) {
                        player->collisionLayers = 1 << Zone->fgLayer[0];
                        Zone->collisionLayers   = 1 << Zone->fgLayer[0];
                    }
                    else {
                        player->collisionLayers = 1 << Zone->fgLayer[1];
                        Zone->collisionLayers   = 1 << Zone->fgLayer[1];
                    }
                }
                else {
                    player->collisionPlane = (self->flags >> 1) & 1;

                    if (!(self->flags & 1)) {
                        player->collisionLayers = 1 << Zone->fgLayer[0];
                        Zone->collisionLayers   = 1 << Zone->fgLayer[0];
                    }
                    else {
                        player->collisionLayers = 1 << Zone->fgLayer[1];
                        Zone->collisionLayers   = 1 << Zone->fgLayer[1];
                    }
                }
            }
        }
    }

    self->visible = DebugMode->debugActive;
}

void HeavyRider_Draw_PlaneSwitch(void)
{
    RSDK_THIS(HeavyRider);

    Vector2 drawPos = self->position;
    drawPos.x -= 0x80000;
    drawPos.y -= self->size << 19;
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    self->mainAnimator.frameID = self->flags & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->mainAnimator, &drawPos, 0);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    drawPos.x = self->position.x + TO_FIXED(8);
    drawPos.y = self->position.y - (self->size << 19);
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    self->mainAnimator.frameID = (self->flags >> 2) & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->mainAnimator, &drawPos, 0);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }
}

void HeavyRider_State_ChargeTrigger(void)
{
    RSDK_THIS(HeavyRider);

    self->visible = DebugMode->debugActive;
}

void HeavyRider_Draw_Simple(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void HeavyRider_State_Fireball(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1) {
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyRider->hitboxFireball)) {
                Player_ElementHurt(player, self, SHIELD_FIRE);
            }
        }
    }
}

void HeavyRider_State_Puff(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

void HeavyRider_StateJimmy_Idle(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (++self->timer >= 45) {
        self->timer             = 38;
        HeavyRider->chargeTimer = 0;
        self->state             = HeavyRider_StateJimmy_RevUp;
    }
}

void HeavyRider_StateJimmy_RevRelease(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->thrustAnimator);

    RSDK.ProcessObjectMovement(self, &HeavyRider->outerBox, &HeavyRider->innerBox);

    if (self->onGround) {
        if (self->velocity.y <= 0x20000)
            self->velocity.y = 0;
        else
            self->velocity.y = (self->velocity.y >> 1) - self->velocity.y;
    }
    else {
        self->velocity.y += 0x3800;
    }

    if (self->direction)
        self->groundVel -= 0x8000;
    else
        self->groundVel += 0x8000;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void HeavyRider_StateJimmy_RevUp(void)
{
    RSDK_THIS(HeavyRider);

    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (--self->timer > 0) {
        if (!(self->timer & 7)) {
            int32 channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        self->timer = 30;
        self->state = HeavyRider_StateJimmy_RevRelease;

        if (self->direction == FLIP_NONE)
            self->groundVel = 0x40000;
        else
            self->groundVel = -0x40000;
    }
}

#if GAME_INCLUDE_EDITOR
void HeavyRider_EditorDraw(void)
{
    RSDK_THIS(HeavyRider);

    switch (self->type) {
        case HEAVYRIDER_RIDER:
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->mainAnimator, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &self->thrustAnimator, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &self->spikeBallAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->fireballAnimator, true, 0);

            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            HeavyRider->spikeBallAngle   = 0x80;
            HeavyRider->spikeBallState   = HEAVYRIDER_SPIKEBALL_SWINGING;
            HeavyRider->spawnDelay       = 16;
            HeavyRider->spawnFireballs   = false;
            HeavyRider->startY           = self->position.y;
            HeavyRider->curAttack        = HEAVYRIDER_ATK_RIDING_SCREENBOUNDS;
            HeavyRider->wheelExtendState = HEAVYRIDER_WHEEL_NONE;
            HeavyRider->wheelLength      = 0;

            self->stateDraw = HeavyRider_Draw_Boss;
            break;

        case HEAVYRIDER_PLANESWITCH:
            RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->mainAnimator, true, 0);

            self->updateRange.x = 0x200000;
            self->updateRange.y = (self->size + 0x400) << 11;
            self->stateDraw     = HeavyRider_Draw_PlaneSwitch;
            break;

        case HEAVYRIDER_FORCECHARGE_LEFT:
        case HEAVYRIDER_FORCECHARGE_RIGHT:

            RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->mainAnimator, true, ((self->type - 2) >> 1) + 4);

            self->updateRange.x = 0x200000;
            self->updateRange.y = 0x200000;
            self->state         = HeavyRider_State_ChargeTrigger;
            self->stateDraw     = HeavyRider_Draw_Simple;
            break;

        default: break;
    }

    StateMachine_Run(self->stateDraw);

    if (self->type == HEAVYRIDER_RIDER && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XSIZE, -SCREEN_YSIZE, WIDE_SCR_XSIZE, SCREEN_YSIZE, 1 | 0 | 4 | 0, 0x00C0F0);
        RSDK_DRAWING_OVERLAY(false);
    }
}

void HeavyRider_EditorLoad(void)
{
    HeavyRider->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/HeavyRider.bin", SCOPE_STAGE);

    // I dont want clang format to do its magic here since I want a specific formatting so
    // clang-format off
    RSDK_ACTIVE_VAR(HeavyRider, flags);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: A, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: Low,  Plane L: B, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: A, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Draw Group R: High, Plane L: B, Draw Group L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: A, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: Low,  Plane L: B, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: A, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: A, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: B, Draw Group L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Draw Group R: High, Plane L: B, Draw Group L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    // clang-format on 
    
    RSDK_ACTIVE_VAR(HeavyRider, type);
    RSDK_ENUM_VAR("Rider", HEAVYRIDER_RIDER);
    RSDK_ENUM_VAR("PlaneSwitch", HEAVYRIDER_PLANESWITCH);
    RSDK_ENUM_VAR("Charge Trigger (Left)", HEAVYRIDER_FORCECHARGE_LEFT);
    RSDK_ENUM_VAR("Charge Trigger (Right)", HEAVYRIDER_FORCECHARGE_RIGHT);
}
#endif

void HeavyRider_Serialize(void)
{
    RSDK_EDITABLE_VAR(HeavyRider, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(HeavyRider, VAR_ENUM, flags);
    RSDK_EDITABLE_VAR(HeavyRider, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(HeavyRider, VAR_BOOL, affectPlayer);
}

