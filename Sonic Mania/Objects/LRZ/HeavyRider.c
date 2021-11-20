#include "SonicMania.h"

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
                self->type = voidToInt(data);

            switch (self->type) {
                case HEAVYRIDER_RIDER:
                    self->active    = ACTIVE_BOUNDS;
                    self->visible   = false;
                    self->drawOrder = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &self->animator2, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &self->animator3, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    HeavyRider->field_10  = 0x80;
                    HeavyRider->field_4   = 1;
                    HeavyRider->field_1B  = 16;
                    HeavyRider->field_20  = 0;
                    HeavyRider->field_24  = self->position.y;
                    HeavyRider->field_3   = 0;
                    HeavyRider->field_18  = 0;
                    HeavyRider->field_14  = 0;
                    self->state         = HeavyRider_State_SetupArena;
                    self->stateDraw     = HeavyRider_StateDraw_Unknown;
                    break;
                case HEAVYRIDER_PLANESWITCH:
                    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator1, true, 0);
                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = (self->size + 0x400) << 11;
                    self->visible       = false;
                    self->drawOrder     = Zone->drawOrderLow;
                    self->state         = HeavyRider_State_PlaneSwitch;
                    self->stateDraw     = HeavyRider_StateDraw1_Unknown;
                    break;
                case HEAVYRIDER_FORCECHARGE_LEFT:
                case HEAVYRIDER_FORCECHARGE_RIGHT:
                case HEAVYRIDER_4:
                case HEAVYRIDER_5:
                    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator1, true, ((self->type - 2) >> 1) + 4);
                    self->active        = ACTIVE_BOUNDS;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = 0x200000;
                    self->visible       = false;
                    self->drawOrder     = Zone->drawOrderLow;
                    self->state         = HeavyRider_State_ChargeTrigger;
                    self->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                case HEAVYRIDER_PUFF:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 10, &self->animator1, true, 0);
                    self->active        = ACTIVE_NORMAL;
                    self->updateRange.x = 0x100000;
                    self->updateRange.y = 0x100000;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderHigh - 1;
                    self->state         = HeavyRider_State_Puff;
                    self->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                case HEAVYRIDER_FIREBALL:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 9, &self->animator1, true, 0);
                    self->active        = ACTIVE_NORMAL;
                    self->updateRange.x = 0x200000;
                    self->updateRange.y = 0x200000;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderHigh;
                    self->state         = HeavyRider_State_Fireball;
                    self->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                case HEAVYRIDER_JIMMY:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 4, &self->animator1, true, 0);
                    self->drawOrder     = Zone->drawOrderHigh;
                    self->active        = ACTIVE_NORMAL;
                    self->updateRange.x = 0x1800000;
                    self->updateRange.y = 0x1800000;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderHigh;
                    self->state         = HeavyRider_StateJimmy_Idle;
                    self->stateDraw     = HeavyRider_StateDraw_Simple;
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

    HeavyRider->hitbox1.left   = -16;
    HeavyRider->hitbox1.top    = -25;
    HeavyRider->hitbox1.right  = 16;
    HeavyRider->hitbox1.bottom = 14;

    HeavyRider->hitbox2.left   = -20;
    HeavyRider->hitbox2.top    = -20;
    HeavyRider->hitbox2.right  = 20;
    HeavyRider->hitbox2.bottom = 20;

    HeavyRider->outerBox.left   = -5;
    HeavyRider->outerBox.top    = 0;
    HeavyRider->outerBox.right  = 5;
    HeavyRider->outerBox.bottom = 48;

    HeavyRider->innerBox.left   = -4;
    HeavyRider->innerBox.top    = 0;
    HeavyRider->innerBox.right  = 4;
    HeavyRider->innerBox.bottom = 48;

    HeavyRider->hitbox5.left   = -4;
    HeavyRider->hitbox5.top    = 46;
    HeavyRider->hitbox5.right  = 4;
    HeavyRider->hitbox5.bottom = 48;

    HeavyRider->hitbox6.left   = -8;
    HeavyRider->hitbox6.top    = -8;
    HeavyRider->hitbox6.right  = 8;
    HeavyRider->hitbox6.bottom = 8;

    HeavyRider->hitbox7.left   = -10;
    HeavyRider->hitbox7.top    = 14;
    HeavyRider->hitbox7.right  = 22;
    HeavyRider->hitbox7.bottom = 46;

    HeavyRider->hitbox8.left   = -2;
    HeavyRider->hitbox8.top    = 38;
    HeavyRider->hitbox8.right  = 14;
    HeavyRider->hitbox8.bottom = 46;

    HeavyRider->health             = 8;
    HeavyRider->invincibilityTimer = 0;
    HeavyRider->sfxHit             = RSDK.GetSfx("Stage/BossHit.wav");
    HeavyRider->sfxExplosion       = RSDK.GetSfx("Stage/Explosion2.wav");
    HeavyRider->sfxCharge          = RSDK.GetSfx("LRZ/RiderCharge.wav");
    HeavyRider->sfxLaunch          = RSDK.GetSfx("LRZ/RiderLaunch.wav");
    HeavyRider->sfxSkid            = RSDK.GetSfx("LRZ/RiderSkid.wav");
    HeavyRider->sfxJump            = RSDK.GetSfx("LRZ/RiderJump.wav");
    HeavyRider->sfxCheer           = RSDK.GetSfx("LRZ/RiderCheer.wav");
    HeavyRider->sfxBumper          = RSDK.GetSfx("Stage/Bumper3.wav");
}

void HeavyRider_SpawnDebris(int frame, uint8 drawOrder, int x, int y)
{
    EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, x, y);
    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &debris->animator, true, frame);
    debris->velocity.x    = RSDK.Rand(-0x20000, 0x20000);
    debris->velocity.y    = RSDK.Rand(-0x20000, -0x10000);
    debris->drawOrder     = drawOrder;
    debris->gravity       = 0x4800;
    debris->updateRange.x = 0x400000;
    debris->updateRange.y = 0x400000;
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
            int playerID = RSDK.GetEntityID(player);

            if (HeavyRider->playerTimers[playerID]) {
                HeavyRider->playerTimers[playerID]--;
            }

            if (Player_CheckBadnikTouch(player, self, &HeavyRider->hitbox1)) {
                if (Player_CheckBossHit(player, self)) {
                    if (HeavyRider->health)
                        HeavyRider->health--;

                    if (!HeavyRider->health) {
                        RSDK.PlaySfx(HeavyRider->sfxExplosion, false, 255);
                        self->timer = 120;
                        if (HeavyRider->field_4 == 1) {
                            int spawnX = 0;
                            if (self->direction) {
                                if (self->animator1.animationID == 2 && self->animator1.frameID)
                                    spawnX = self->position.x + 0x160000;
                                else
                                    spawnX = self->position.x - 0xE0000;
                            }
                            else if (self->animator1.animationID == 2 && self->animator1.frameID)
                                spawnX = self->position.x - 0x160000;
                            else
                                spawnX = self->position.x + 0xE0000;

                            int angle = 0x400;
                            int cos   = RSDK.Cos256(HeavyRider->field_10);
                            for (int i = 0; i < 8; ++i) {
                                HeavyRider_SpawnDebris(1, Zone->drawOrderHigh, spawnX + angle * cos, self->position.y - 0x210000);
                                angle += 0x800;
                            }

                            HeavyRider_SpawnDebris(2, Zone->drawOrderHigh, spawnX + angle * cos, self->position.y - 0x210000);
                            HeavyRider->field_4 = 0;

                            RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Flamethrower)->interval = -1;
                            RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Flamethrower)->interval = -1;
                            HeavyRider->field_20                                                    = 0;
                            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
                        }
                        else if (HeavyRider->field_4 == 7) {
                            HeavyRider_SpawnDebris(2, Zone->drawOrderHigh, HeavyRider->debrisPos.x, HeavyRider->debrisPos.y);
                            HeavyRider->field_4 = 0;
                        }
                        SceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                        self->state = HeavyRider_State_Destroyed;
                    }
                    RSDK.PlaySfx(HeavyRider->sfxHit, false, 255);
                    self->timer -= 60;
                    HeavyRider->invincibilityTimer = 30;
                    foreach_break;
                }
            }
            else if (Player_CheckBadnikTouch(player, self, &HeavyRider->hitbox7)) {
                if (!HeavyRider->playerTimers[playerID] && Player_CheckBossHit(player, self)) {
                    RSDK.PlaySfx(HeavyRider->sfxBumper, false, 255);
                    int angle = RSDK.ATan2(player->position.x - self->position.x - 0x60000, player->position.y - self->position.y - 0x1E0000);
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
                    player->jumpAbility                = 0;
                    HeavyRider->playerTimers[playerID] = 15;
                }
            }
            else if ((uint32)(HeavyRider->field_10 & 0x7F) + 32 < 0x40) {
                int storeX         = self->position.x;
                int storeY         = self->position.y;
                self->position.x = HeavyRider->debrisPos.x;
                self->position.y = HeavyRider->debrisPos.y;

                if (Player_CheckCollisionTouch(player, self, &HeavyRider->hitbox2)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x800, player, false, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, self);
                }

                self->position.x = storeX;
                self->position.y = storeY;
            }
        }
    }

    if (HeavyRider->field_3 == 1 || HeavyRider->field_3 == 2) {

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
            else if (rider->type == HEAVYRIDER_FORCECHARGE_LEFT) {
                if (HeavyRider->field_3 == 2 && self->onGround
                    && RSDK.CheckObjectCollisionTouchBox(rider, &HeavyRider->hitbox6, self, &HeavyRider->hitbox5)) {
                    if (self->direction == FLIP_X) {
                        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
                        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
                        self->state = HeavyRider_State_Unknown4;
                    }
                    else if (self->state != HeavyRider_State_Unknown2) {
                        self->timer           = 38;
                        HeavyRider->chargeTimer = 0;
                        self->state           = HeavyRider_State_Unknown2;
                    }
                }
            }
            else if (rider->type == HEAVYRIDER_FORCECHARGE_RIGHT && HeavyRider->field_3 == 2 && self->onGround
                     && RSDK.CheckObjectCollisionTouchBox(rider, &HeavyRider->hitbox6, self, &HeavyRider->hitbox5) == true) {
                if (self->direction == FLIP_NONE) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
                    self->state = HeavyRider_State_Unknown4;
                }
                else if (self->state != HeavyRider_State_Unknown2) {
                    self->timer           = 38;
                    HeavyRider->chargeTimer = 0;
                    self->state           = HeavyRider_State_Unknown2;
                }
            }
        }

        foreach_all(PlaneSwitch, planeSwitch)
        {
            PlaneSwitch_CheckCollisions(planeSwitch, self, planeSwitch->flags, planeSwitch->size, false, 0, 0);
        }
    }
}

void HeavyRider_Explode(void)
{
    RSDK_THIS(HeavyRider);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyRider->sfxExplosion, false, 255);
        if (!(Zone->timer & 0xF)) {
            int x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int y = self->position.y + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void HeavyRider_Unknown4(void)
{
    RSDK_THIS(HeavyRider);

    if (self->direction == FLIP_X) {
        if (self->position.x < (ScreenInfo->position.x + 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
            self->state = HeavyRider_State_Unknown4;
        }
    }
    else {
        if (self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
            self->state = HeavyRider_State_Unknown4;
        }
    }
}

void HeavyRider_Unknown5(void)
{
    RSDK_THIS(HeavyRider);

    if (self->direction == FLIP_X) {
        if (self->position.x < (Zone->screenBoundsL1[0] - 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
            self->state = HeavyRider_State_Unknown4;
        }
    }
    else {
        if (self->position.x > (Zone->screenBoundsR1[0] + 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
            self->state = HeavyRider_State_Unknown4;
        }
    }
}

void HeavyRider_Unknown6(void)
{
    RSDK_THIS(HeavyRider);

    if (HeavyRider->field_3 != 2) {
        HeavyRider->field_20 = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
    }

    int id = RSDK.Rand(0, 10);
    switch (HeavyRider->field_3) {
        case 0: {
            int ids[] = {
                0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
            };
            HeavyRider->field_3 = ids[id];
            break;
        }
        case 1: {
            int ids[] = {
                0, 0, 1, 2, 2, 2, 3, 3, 3, 3,
            };
            HeavyRider->field_3 = ids[id];
            break;
        }
        case 2: {
            int ids[] = {
                0, 0, 1, 1, 1, 2, 3, 3, 3, 3,
            };
            HeavyRider->field_3 = ids[id];
            if (id < 6) {
                HeavyRider->field_20 = 0;
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
            }
            break;
        } break;
        case 3:
        case 4: {
            int ids[] = {
                0, 0, 0, 3, 3, 1, 1, 1, 2, 2,
            };
            HeavyRider->field_3 = ids[id];
            break;
        }
        default: break;
    }
}

void HeavyRider_State_SetupArena(void)
{
    RSDK_THIS(HeavyRider);

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + 424;
        self->active              = ACTIVE_NORMAL;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
        self->state = HeavyRider_State_StartFight;
    }
}

void HeavyRider_State_StartFight(void)
{
    RSDK_THIS(HeavyRider);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;
    EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > self->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (self->position.x >> 16) - 424;
#if RETRO_USE_PLUS
        HeavyRider->outro = (Entity *)CREATE_ENTITY(LRZ3Outro, NULL, self->position.x, self->position.y);
#endif
        self->state = HeavyRider_State_SetupRider;
    }
}

void HeavyRider_State_SetupRider(void)
{
    RSDK_THIS(HeavyRider);

    Zone->playerBoundActiveL[0] = true;

#if RETRO_USE_PLUS
    EntityLRZ3Outro *outro = (EntityLRZ3Outro *)HeavyRider->outro;
    if (outro->state == LRZ3Outro_State_Unknown3) {
#else
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > self->position.x) {
#endif
        self->position.y -= 0x400000;
        self->position.x      = (Zone->screenBoundsL1[0] - 64) << 16;
        self->onGround        = false;
        self->tileCollisions  = true;
        self->collisionPlane  = 0;
        self->collisionLayers = (1 << Zone->fgLow) | (1 << Zone->fgHigh);
        Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
        self->visible    = true;
        self->state      = HeavyRider_State_Unknown1;
        self->velocity.x = 0;
        self->timer      = 0;
    }
}

void HeavyRider_State_Unknown1(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessTileCollisions(self, &HeavyRider->outerBox, &HeavyRider->innerBox);

    if (self->onGround) {
        if (self->velocity.y <= 0x20000) {
            self->velocity.y = 0;
        }
        else {
            self->velocity.y = (self->velocity.y >> 1) - self->velocity.y;
            if (self->velocity.y > 0) {
                if (RSDK.Rand(0, 2) == 1) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
                    self->state = HeavyRider_State_Unknown4;
                }
                self->velocity.y = 0;
            }
            else {
                self->onGround                                                        = false;
                RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Flamethrower)->interval = -1;
                RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Flamethrower)->interval = -1;
            }
        }
    }
    else {
        self->velocity.y += 0x3800;
    }

    if (HeavyRider->field_18 == 1) {
        if (HeavyRider->field_14 < 0x100000)
            HeavyRider->field_14 += 0x30000;

        if (HeavyRider->field_14 > 0x100000) {
            HeavyRider->field_14 = 0x100000;
            HeavyRider->field_18 = 2;
        }
    }
    else if (HeavyRider->field_18 == 2) {
        if (HeavyRider->field_14 > 0)
            HeavyRider->field_14 -= 0x30000;

        if (HeavyRider->field_14 < 0) {
            HeavyRider->field_14 = 0;
            HeavyRider->field_18 = 0;
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator5, true, 0);
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (self->position.y == HeavyRider->field_24 && HeavyRider->field_3 != 4 && player1->onGround) {
        if (self->velocity.x <= 0) {
            if (player1->position.x < self->position.x) {
                int dist = self->position.x - player1->position.x;
                if (player1->velocity.x > 0)
                    dist -= 8 * player1->velocity.x;
                if (dist < 0x480000) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &self->animator5, true, 0);
                    RSDK.PlaySfx(HeavyRider->sfxJump, false, 255);
                    self->onGround     = false;
                    self->velocity.y   = -0x58000;
                    HeavyRider->field_18 = 1;
                    HeavyRider->field_14 = 0;
                }
            }
        }
        else {
            if (player1->position.x > self->position.x) {
                int dist = player1->position.x - self->position.x;
                if (player1->velocity.x < 0)
                    dist += 8 * player1->velocity.x;
                if (dist < 0x480000) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &self->animator5, true, 0);
                    RSDK.PlaySfx(HeavyRider->sfxJump, false, 255);
                    self->onGround     = false;
                    self->velocity.y   = -0x58000;
                    HeavyRider->field_18 = 1;
                    HeavyRider->field_14 = 0;
                }
            }
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

    switch (HeavyRider->field_3) {
        case 0: HeavyRider_Unknown4(); break;
        case 1:
        case 2:
        case 4: HeavyRider_Unknown5(); break;
        case 3:
            HeavyRider_Unknown5();
            HeavyRider->field_3 = 4;
            HeavyRider->field_4 = 7;
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 1, &self->animator1, true, 2);
            self->timer           = 41;
            HeavyRider->chargeTimer = 0;
            self->state           = HeavyRider_State_Unknown3;
            break;
        default: break;
    }

    if (self->animator1.animationID != 2) {
        if (abs(self->groundVel) > 0x48000 || HeavyRider->field_20 == 1) {
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &self->animator2, false, 0);
            if (HeavyRider->field_20 == 1) {
                if (self->onGround) {
                    if (!--HeavyRider->field_1B) {
                        HeavyRider->field_1B = 10;
                        CREATE_ENTITY(HeavyRider, intToVoid(HEAVYRIDER_FIREBALL), self->position.x, self->position.y);
                    }
                }
            }
        }
        else {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
            if (HeavyRider->field_1B > 16)
                HeavyRider->field_1B = 16;

            if (!--HeavyRider->field_1B) {
                HeavyRider->field_1B    = 16;
                EntityHeavyRider *child = CREATE_ENTITY(HeavyRider, intToVoid(HEAVYRIDER_PUFF), self->position.x, self->position.y + 0x1C0000);
                if (self->direction == FLIP_X)
                    child->position.x += 0x110000;
                else
                    child->position.x -= 0x110000;
            }
        }
    }
    if (HeavyRider->field_4 == 1)
        HeavyRider->field_10 += 6;
    if (HeavyRider->field_4 == 7) {
        HeavyRider->field_10    = 0;
        HeavyRider->debrisPos.x = self->position.x;
        HeavyRider->debrisPos.y = self->position.y;
        if (self->direction)
            HeavyRider->debrisPos.x -= 0x200000;
        else
            HeavyRider->debrisPos.x += 0x200000;
        HeavyRider->debrisPos.y += 0xC0000;
    }
    HeavyRider_CheckObjectCollisions();
}

void HeavyRider_State_Unknown2(void)
{
    RSDK_THIS(HeavyRider);
    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    HeavyRider_State_Unknown1();
    if (self->type != HEAVYRIDER_JIMMY && self->timer == 30) {
        RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, Flamethrower)->interval = 0;
        RSDK_GET_ENTITY(SceneInfo->entitySlot + 2, Flamethrower)->interval = 0;
    }

    if (--self->timer > 0) {
        if (!(self->timer & 7)) {
            int channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        self->timer = 30;
        self->groundVel += (self->groundVel >> 2) + (self->groundVel >> 1);
        if (self->type == HEAVYRIDER_JIMMY)
            self->state = HeavyRider_State_Unknown1;
        else
            self->state = HeavyRider_State_Charge;
    }
}

void HeavyRider_State_Unknown3(void)
{
    RSDK_THIS(HeavyRider);
    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    HeavyRider_State_Unknown1();

    if (--self->timer > 0) {
        if (!(self->timer & 7)) {
            int channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        self->timer = 30;
        self->groundVel += (self->groundVel >> 2) + (self->groundVel >> 1);
        self->state = HeavyRider_State_Unknown1;
    }
}

void HeavyRider_State_Charge(void)
{
    RSDK_THIS(HeavyRider);

    HeavyRider_State_Unknown1();

    if (--self->timer == 16) {
        HeavyRider->field_1B = 10;
        HeavyRider->field_20 = 1;
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 8, &self->animator4, true, 0);
    }
    else if (self->timer <= 0) {
        for (int i = 0; i < 0x60; ++i) {
            int x                 = RSDK.Rand(-128, 129);
            int y                 = 2 * RSDK.Rand(4, 16);
            EntityDebris *debris  = CREATE_ENTITY(Debris, NULL, (x << 16) + self->position.x, (ScreenInfo->position.y - y) << 16);
            debris->updateRange.x = 0x2000000;
            debris->updateRange.y = 0x2000000;
            debris->state         = Debris_State_Fall;
            debris->gravity       = 0x1800;
            debris->velocity.x    = RSDK.Rand(-2, 3) << 16;
            debris->velocity.y    = RSDK.Rand(-2, 2) << 16;
            debris->drawFX        = FX_FLIP;
            debris->direction     = i & 3;
            debris->drawOrder     = Zone->drawOrderHigh;
            int frame             = RSDK.Rand(0, 4);
            int anim              = RSDK.Rand(12, 15);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, anim, &debris->animator, true, frame);
        }
        RSDK.PlaySfx(HeavyRider->sfxCheer, false, 255);
        self->state = HeavyRider_State_Unknown1;
    }
}

void HeavyRider_State_Unknown4(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator4);
    RSDK.ProcessTileCollisions(self, &HeavyRider->outerBox, &HeavyRider->innerBox);
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

    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->animator1, true, 0);
        self->direction ^= 1u;
        HeavyRider->field_4 = 1;
        self->state       = HeavyRider_State_Unknown1;
    }
    HeavyRider->field_10 += 4;
    HeavyRider_CheckObjectCollisions();
}

void HeavyRider_State_Destroyed(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    HeavyRider_Explode();

    if (--self->timer <= 0) {
        EntityHeavyRider *child = CREATE_ENTITY(HeavyRider, intToVoid(HEAVYRIDER_JIMMY), self->position.x, self->position.y);
        child->direction        = self->direction;
        child->onGround         = false;
        child->tileCollisions   = true;
        child->collisionLayers  = (1 << Zone->fgLow) | (1 << Zone->fgHigh);
        child->collisionPlane   = 0;
        self->velocity.y      = -0x60000;
        if (self->direction == FLIP_NONE)
            self->velocity.x = -0x20000;
        else
            self->velocity.x = 0x20000;
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 3, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        self->state = HeavyRider_State_Finish;
    }
}

void HeavyRider_State_Finish(void)
{
    RSDK_THIS(HeavyRider);

    if (++self->timer < 45)
        HeavyRider_Explode();
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->velocity.y > 0)
        self->drawOrder = Zone->drawOrderHigh;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        self->position.x      = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        self->position.y      = (ScreenInfo->position.y - 48) << 16;
        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), self->position.x, self->position.y);
        prison->isPermanent     = true;
        prison->drawOrder       = Zone->drawOrderHigh;
        destroyEntity(self);
    }
}

void HeavyRider_StateDraw_Unknown(void)
{
    RSDK_THIS(HeavyRider);
    Vector2 drawPos;

    if (HeavyRider->field_18) {
        drawPos = self->position;
        drawPos.y += 2 * HeavyRider->field_14;
        self->animator5.frameID = 3;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);

        self->animator5.frameID = 2;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);

        drawPos.y -= HeavyRider->field_14;
        self->animator5.frameID = 1;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);
    }

    int drawX = 0;
    if (self->direction) {
        if (self->animator1.animationID == 2 && self->animator1.frameID)
            drawX = self->position.x + 0x160000;
        else
            drawX = self->position.x - 0xE0000;
    }
    else if (self->animator1.animationID == 2 && self->animator1.frameID)
        drawX = self->position.x - 0x160000;
    else
        drawX = self->position.x + 0xE0000;

    int drawY = self->position.y - 0x210000;
    drawPos.x = drawX;
    drawPos.y = drawY;

    if (HeavyRider->field_4) {
        if (HeavyRider->field_4 <= 4) {
            if (HeavyRider->field_10 >= 0x80) {
                self->animator3.frameID = 2;
                drawPos.x                 = drawX + 0x4400 * RSDK.Cos256(HeavyRider->field_10);
                drawPos.y                 = drawY + 0x880 * RSDK.Sin256(2 * HeavyRider->field_10);
                self->drawFX |= FX_SCALE;
                self->scale.x = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                self->scale.y = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                RSDK.DrawSprite(&self->animator3, &drawPos, false);
                self->drawFX &= ~FX_SCALE;
                HeavyRider->debrisPos     = drawPos;
                self->animator3.frameID = 1;

                int angle = 0x4400;
                for (int i = 0; i < 8; ++i) {
                    angle -= 0x800;
                    drawPos.x = drawX + angle * RSDK.Cos256(HeavyRider->field_10);
                    drawPos.y = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->field_10);
                    RSDK.DrawSprite(&self->animator3, &drawPos, false);
                }
            }
        }
        else {
            if (HeavyRider->field_4 == 7) {
                self->animator3.frameID = 2;
                RSDK.DrawSprite(&self->animator3, &HeavyRider->debrisPos, false);
            }
        }
    }

    if (HeavyRider->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->animator1, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    switch (HeavyRider->field_4) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (HeavyRider->field_10 < 0x80) {
                self->animator3.frameID = 1;
                int angle                 = 0x400;
                for (int i = 0; i < 8; ++i) {
                    angle += 0x800;
                    drawPos.x = drawX + angle * RSDK.Cos256(HeavyRider->field_10);
                    drawPos.y = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->field_10);
                    RSDK.DrawSprite(&self->animator3, &drawPos, false);
                }

                self->animator3.frameID = 2;
                drawPos.x                 = drawX + angle * RSDK.Cos256(HeavyRider->field_10);
                drawPos.y                 = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->field_10);
                self->drawFX |= FX_SCALE;
                self->scale.x = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                self->scale.y = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                RSDK.DrawSprite(&self->animator3, &drawPos, false);
                self->drawFX &= ~FX_SCALE;
                HeavyRider->debrisPos = drawPos;
            }
            RSDK.DrawSprite(&self->animator4, NULL, false);
            break;
        case 5:
        case 6:
            RSDK.DrawSprite(&self->animator3, &HeavyRider->debrisPos, false);
            RSDK.DrawSprite(&self->animator4, NULL, false);
            break;
        default: RSDK.DrawSprite(&self->animator4, NULL, false);
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
                        player->collisionLayers = 1 << Zone->fgLow;
                        Zone->fgLayers          = 1 << Zone->fgLow;
                    }
                    else {
                        player->collisionLayers = 1 << Zone->fgHigh;
                        Zone->fgLayers          = 1 << Zone->fgHigh;
                    }
                }
                else {
                    player->collisionPlane = (self->flags >> 1) & 1;
                    if (!(self->flags & 1)) {
                        player->collisionLayers = 1 << Zone->fgLow;
                        Zone->fgLayers          = 1 << Zone->fgLow;
                    }
                    else {
                        player->collisionLayers = 1 << Zone->fgHigh;
                        Zone->fgLayers          = 1 << Zone->fgHigh;
                    }
                }
            }
        }
    }

    self->visible = DebugMode->debugActive;
}

void HeavyRider_StateDraw1_Unknown(void)
{
    RSDK_THIS(HeavyRider);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x -= 0x80000;
    drawPos.y -= self->size << 19;
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    self->animator1.frameID = self->flags & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator1, &drawPos, 0);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    drawPos.x = self->position.x + 0x80000;
    drawPos.y = self->position.y - (self->size << 19);

    int32 dx  = (drawPos.x + 0x80000 - self->position.x) >> 8;
    int32 dy  = (drawPos.y - self->position.y) >> 8;
    drawPos.x = (dy * RSDK.Sin256(self->angle)) + dx * RSDK.Cos256(self->angle) + self->position.x;
    drawPos.y = (dy * RSDK.Cos256(self->angle)) - dx * RSDK.Sin256(self->angle) + self->position.y;

    self->animator1.frameID = (self->flags >> 2) & 3;
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator1, &drawPos, 0);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }
}

void HeavyRider_State_ChargeTrigger(void)
{
    RSDK_THIS(HeavyRider);

    self->visible = DebugMode->debugActive;
}

void HeavyRider_StateDraw_Simple(void)
{
    RSDK_THIS(HeavyRider);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void HeavyRider_State_Fireball(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1) {
        destroyEntity(self);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &HeavyRider->hitbox8)) {
                Player_CheckElementalHit(player, self, SHIELD_FIRE);
            }
        }
    }
}

void HeavyRider_State_Puff(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID == self->animator1.frameCount - 1)
        destroyEntity(self);
}

void HeavyRider_StateJimmy_Idle(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    if (++self->timer >= 45) {
        self->timer           = 38;
        HeavyRider->chargeTimer = 0;
        self->state           = HeavyRider_StateJimmy_RevUp;
    }
}

void HeavyRider_StateJimmy_RevRelease(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessTileCollisions(self, &HeavyRider->outerBox, &HeavyRider->innerBox);

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

    RSDK.ProcessAnimation(&self->animator1);

    if (--self->timer > 0) {
        if (!(self->timer & 7)) {
            int channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
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

#if RETRO_INCLUDE_EDITOR
void HeavyRider_EditorDraw(void)
{
    RSDK_THIS(HeavyRider);

    switch (self->type) {
        case HEAVYRIDER_RIDER:
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            HeavyRider->field_10  = 0x80;
            HeavyRider->field_4   = 1;
            HeavyRider->field_1B  = 16;
            HeavyRider->field_20  = 0;
            HeavyRider->field_24  = self->position.y;
            HeavyRider->field_3   = 0;
            HeavyRider->field_18  = 0;
            HeavyRider->field_14  = 0;
            self->stateDraw     = HeavyRider_StateDraw_Unknown;
            break;
        case HEAVYRIDER_PLANESWITCH:
            RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator1, true, 0);
            self->updateRange.x = 0x200000;
            self->updateRange.y = (self->size + 0x400) << 11;
            self->stateDraw     = HeavyRider_StateDraw1_Unknown;
            break;
        case HEAVYRIDER_FORCECHARGE_LEFT:
        case HEAVYRIDER_FORCECHARGE_RIGHT:
            RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator1, true, ((self->type - 2) >> 1) + 4);
            self->updateRange.x = 0x200000;
            self->updateRange.y = 0x200000;
            self->state         = HeavyRider_State_ChargeTrigger;
            self->stateDraw     = HeavyRider_StateDraw_Simple;
            break;
        default: break;
    }

    StateMachine_Run(self->stateDraw);

    if (self->type == HEAVYRIDER_RIDER && showGizmos()) {
        DrawHelpers_DrawArenaBounds(0x00C0F0, 1 | 0 | 4 | 0, -424, -SCREEN_YSIZE, 424, SCREEN_YSIZE);
    }
}

void HeavyRider_EditorLoad(void)
{
    HeavyRider->aniFrames = RSDK.LoadSpriteAnimation("LRZ3/HeavyRider.bin", SCOPE_STAGE);

    // I dont want clang format to do its magic here since I want a specific formatting so
    // clang-format off
    RSDK_ACTIVE_VAR(HeavyRider, flags);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: A, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: A, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: B, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: Low,  Plane L: B, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: A, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: A, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: B, Layer L: Low ", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: A, Layer R: High, Plane L: B, Layer L: High", PLANESWITCH_PLANEA_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: A, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: A, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: B, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: Low,  Plane L: B, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_LOWLAYER_RIGHT  | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: A, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: A, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEA_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: B, Layer L: Low ", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_LOWLAYER_LEFT);
    RSDK_ENUM_VAR("Plane R: B, Layer R: High, Plane L: B, Layer L: High", PLANESWITCH_PLANEB_RIGHT | PLANESWITCH_HIGHLAYER_RIGHT | PLANESWITCH_PLANEB_LEFT | PLANESWITCH_HIGHLAYER_LEFT);
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

