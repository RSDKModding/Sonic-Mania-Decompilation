#include "SonicMania.h"

ObjectHeavyRider *HeavyRider;

void HeavyRider_Update(void)
{
    RSDK_THIS(HeavyRider);
    StateMachine_Run(entity->state);
}

void HeavyRider_LateUpdate(void) {}

void HeavyRider_StaticUpdate(void) {}

void HeavyRider_Draw(void)
{
    RSDK_THIS(HeavyRider);
    StateMachine_Run(entity->stateDraw);
}

void HeavyRider_Create(void *data)
{
    RSDK_THIS(HeavyRider);

    entity->drawFX = FX_ROTATE | FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            if (!entity->type)
                entity->type = voidToInt(data);

            switch (entity->type) {
                case HEAVYRIDER_RIDER:
                    entity->active    = ACTIVE_BOUNDS;
                    entity->visible   = false;
                    entity->drawOrder = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &entity->animator3, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    HeavyRider->field_10  = 0x80;
                    HeavyRider->field_4   = 1;
                    HeavyRider->field_1B  = 16;
                    HeavyRider->field_20  = 0;
                    HeavyRider->field_24  = entity->position.y;
                    HeavyRider->field_3   = 0;
                    HeavyRider->field_18  = 0;
                    HeavyRider->field_14  = 0;
                    entity->state         = HeavyRider_State_SetupArena;
                    entity->stateDraw     = HeavyRider_StateDraw_Unknown;
                    break;
                case HEAVYRIDER_PLANESWITCH:
                    RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->animator1, true, 0);
                    entity->active        = ACTIVE_BOUNDS;
                    entity->updateRange.x = 0x200000;
                    entity->updateRange.y = (entity->size + 0x400) << 11;
                    entity->visible       = false;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->state         = HeavyRider_State_PlaneSwitch;
                    entity->stateDraw     = HeavyRider_StateDraw1_Unknown;
                    break;
                case HEAVYRIDER_FORCECHARGE_LEFT:
                case HEAVYRIDER_FORCECHARGE_RIGHT:
                case HEAVYRIDER_4:
                case HEAVYRIDER_5:
                    RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->animator1, true, ((entity->type - 2) >> 1) + 4);
                    entity->active        = ACTIVE_BOUNDS;
                    entity->updateRange.x = 0x200000;
                    entity->updateRange.y = 0x200000;
                    entity->visible       = false;
                    entity->drawOrder     = Zone->drawOrderLow;
                    entity->state         = HeavyRider_State_ChargeTrigger;
                    entity->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                case HEAVYRIDER_PUFF:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 10, &entity->animator1, true, 0);
                    entity->active        = ACTIVE_NORMAL;
                    entity->updateRange.x = 0x100000;
                    entity->updateRange.y = 0x100000;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh - 1;
                    entity->state         = HeavyRider_State_Puff;
                    entity->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                case HEAVYRIDER_FIREBALL:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 9, &entity->animator1, true, 0);
                    entity->active        = ACTIVE_NORMAL;
                    entity->updateRange.x = 0x200000;
                    entity->updateRange.y = 0x200000;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    entity->state         = HeavyRider_State_Fireball;
                    entity->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                case HEAVYRIDER_JIMMY:
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 4, &entity->animator1, true, 0);
                    entity->drawOrder     = Zone->drawOrderHigh;
                    entity->active        = ACTIVE_NORMAL;
                    entity->updateRange.x = 0x1800000;
                    entity->updateRange.y = 0x1800000;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderHigh;
                    entity->state         = HeavyRider_StateJimmy_Idle;
                    entity->stateDraw     = HeavyRider_StateDraw_Simple;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
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
    HeavyRider->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    HeavyRider->sfxExplosion       = RSDK.GetSFX("Stage/Explosion2.wav");
    HeavyRider->sfxCharge          = RSDK.GetSFX("LRZ/RiderCharge.wav");
    HeavyRider->sfxLaunch          = RSDK.GetSFX("LRZ/RiderLaunch.wav");
    HeavyRider->sfxSkid            = RSDK.GetSFX("LRZ/RiderSkid.wav");
    HeavyRider->sfxJump            = RSDK.GetSFX("LRZ/RiderJump.wav");
    HeavyRider->sfxCheer           = RSDK.GetSFX("LRZ/RiderCheer.wav");
    HeavyRider->sfxBumper          = RSDK.GetSFX("Stage/Bumper3.wav");
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

            if (Player_CheckBadnikTouch(player, entity, &HeavyRider->hitbox1)) {
                if (Player_CheckBossHit(player, entity)) {
                    if (HeavyRider->health)
                        HeavyRider->health--;

                    if (!HeavyRider->health) {
                        RSDK.PlaySfx(HeavyRider->sfxExplosion, false, 255);
                        entity->timer = 120;
                        if (HeavyRider->field_4 == 1) {
                            int spawnX = 0;
                            if (entity->direction) {
                                if (entity->animator1.animationID == 2 && entity->animator1.frameID)
                                    spawnX = entity->position.x + 0x160000;
                                else
                                    spawnX = entity->position.x - 0xE0000;
                            }
                            else if (entity->animator1.animationID == 2 && entity->animator1.frameID)
                                spawnX = entity->position.x - 0x160000;
                            else
                                spawnX = entity->position.x + 0xE0000;

                            int angle = 0x400;
                            int cos   = RSDK.Cos256(HeavyRider->field_10);
                            for (int i = 0; i < 8; ++i) {
                                HeavyRider_SpawnDebris(1, Zone->drawOrderHigh, spawnX + angle * cos, entity->position.y - 0x210000);
                                angle += 0x800;
                            }

                            HeavyRider_SpawnDebris(2, Zone->drawOrderHigh, spawnX + angle * cos, entity->position.y - 0x210000);
                            HeavyRider->field_4 = 0;

                            RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, Flamethrower)->interval = -1;
                            RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Flamethrower)->interval = -1;
                            HeavyRider->field_20                                                    = 0;
                            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
                        }
                        else if (HeavyRider->field_4 == 7) {
                            HeavyRider_SpawnDebris(2, Zone->drawOrderHigh, HeavyRider->debrisPos.x, HeavyRider->debrisPos.y);
                            HeavyRider->field_4 = 0;
                        }
                        RSDK_sceneInfo->timeEnabled = false;
                        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
                        entity->state = HeavyRider_State_Destroyed;
                    }
                    RSDK.PlaySfx(HeavyRider->sfxHit, false, 255);
                    entity->timer -= 60;
                    HeavyRider->invincibilityTimer = 30;
                    foreach_break;
                }
            }
            else if (Player_CheckBadnikTouch(player, entity, &HeavyRider->hitbox7)) {
                if (!HeavyRider->playerTimers[playerID] && Player_CheckBossHit(player, entity)) {
                    RSDK.PlaySfx(HeavyRider->sfxBumper, false, 255);
                    int angle = RSDK.ATan2(player->position.x - entity->position.x - 0x60000, player->position.y - entity->position.y - 0x1E0000);
                    player->velocity.x = 0x380 * RSDK.Cos256(angle);

                    if (player->position.x >= entity->position.x) {
                        if (player->position.x > entity->position.x) {
                            if (entity->velocity.x > 0)
                                player->velocity.x += entity->velocity.x;
                        }
                    }
                    else {
                        if (entity->velocity.x >= 0) {
                            if (player->position.x > entity->position.x) {
                                if (entity->velocity.x > 0)
                                    player->velocity.x += entity->velocity.x;
                            }
                        }
                        else {
                            player->velocity.x += entity->velocity.x;
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
                int storeX         = entity->position.x;
                int storeY         = entity->position.y;
                entity->position.x = HeavyRider->debrisPos.x;
                entity->position.y = HeavyRider->debrisPos.y;

                if (Player_CheckCollisionTouch(player, entity, &HeavyRider->hitbox2)) {
#if RETRO_USE_PLUS
                    if (!Player_CheckMightyUnspin(0x800, player, false, &player->uncurlTimer))
#endif
                        Player_CheckHit(player, entity);
                }

                entity->position.x = storeX;
                entity->position.y = storeY;
            }
        }
    }

    if (HeavyRider->field_3 == 1 || HeavyRider->field_3 == 2) {

        foreach_all(HeavyRider, rider)
        {
            if (rider->type == HEAVYRIDER_PLANESWITCH) {
                if (abs(entity->position.x - rider->position.x) < 0x100000 && abs(entity->position.y - rider->position.y) < rider->size << 19) {
                    if (entity->velocity.x >= 0)
                        entity->collisionPlane = (rider->flags >> 3) & 1;
                    else
                        entity->collisionPlane = (rider->flags >> 1) & 1;
                }
            }
            else if (rider->type == HEAVYRIDER_FORCECHARGE_LEFT) {
                if (HeavyRider->field_3 == 2 && entity->onGround
                    && RSDK.CheckObjectCollisionTouchBox(rider, &HeavyRider->hitbox6, entity, &HeavyRider->hitbox5)) {
                    if (entity->direction == FLIP_X) {
                        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
                        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                        entity->state = HeavyRider_State_Unknown4;
                    }
                    else if (entity->state != HeavyRider_State_Unknown2) {
                        entity->timer           = 38;
                        HeavyRider->chargeTimer = 0;
                        entity->state           = HeavyRider_State_Unknown2;
                    }
                }
            }
            else if (rider->type == HEAVYRIDER_FORCECHARGE_RIGHT && HeavyRider->field_3 == 2 && entity->onGround
                     && RSDK.CheckObjectCollisionTouchBox(rider, &HeavyRider->hitbox6, entity, &HeavyRider->hitbox5) == true) {
                if (entity->direction == FLIP_NONE) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                    entity->state = HeavyRider_State_Unknown4;
                }
                else if (entity->state != HeavyRider_State_Unknown2) {
                    entity->timer           = 38;
                    HeavyRider->chargeTimer = 0;
                    entity->state           = HeavyRider_State_Unknown2;
                }
            }
        }

        foreach_all(PlaneSwitch, planeSwitch)
        {
            PlaneSwitch_CheckCollisions(planeSwitch, entity, planeSwitch->flags, planeSwitch->size, false, 0, 0);
        }
    }
}

void HeavyRider_Explode(void)
{
    RSDK_THIS(HeavyRider);

    if (!(Zone->timer & 7)) {
        RSDK.PlaySfx(HeavyRider->sfxExplosion, false, 255);
        if (!(Zone->timer & 0xF)) {
            int x = entity->position.x + (RSDK.Rand(-19, 20) << 16);
            int y = entity->position.y + (RSDK.Rand(-24, 25) << 16);
            CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void HeavyRider_Unknown4(void)
{
    RSDK_THIS(HeavyRider);

    if (entity->direction == FLIP_X) {
        if (entity->position.x < (RSDK_screens->position.x + 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            entity->state = HeavyRider_State_Unknown4;
        }
    }
    else {
        if (entity->position.x > (RSDK_screens->position.x + RSDK_screens->width - 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            entity->state = HeavyRider_State_Unknown4;
        }
    }
}

void HeavyRider_Unknown5(void)
{
    RSDK_THIS(HeavyRider);

    if (entity->direction == FLIP_X) {
        if (entity->position.x < (Zone->screenBoundsL1[0] - 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            entity->state = HeavyRider_State_Unknown4;
        }
    }
    else {
        if (entity->position.x > (Zone->screenBoundsR1[0] + 48) << 16) {
            HeavyRider_Unknown6();
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            entity->state = HeavyRider_State_Unknown4;
        }
    }
}

void HeavyRider_Unknown6(void)
{
    RSDK_THIS(HeavyRider);

    if (HeavyRider->field_3 != 2) {
        HeavyRider->field_20 = 0;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
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
                RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
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

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 424;
        entity->active              = ACTIVE_NORMAL;

        foreach_all(BoundsMarker, marker) { destroyEntity(marker); }
        entity->state = HeavyRider_State_StartFight;
    }
}

void HeavyRider_State_StartFight(void)
{
    RSDK_THIS(HeavyRider);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = RSDK_screens->position.x;
    EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->position.x > entity->position.x - 0x500000) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - 424;
#if RETRO_USE_PLUS
        HeavyRider->outro = (Entity *)CREATE_ENTITY(LRZ3Outro, NULL, entity->position.x, entity->position.y);
#endif
        entity->state = HeavyRider_State_SetupRider;
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
    if (player1->position.x > entity->position.x) {
#endif
        entity->position.y -= 0x400000;
        entity->position.x      = (Zone->screenBoundsL1[0] - 64) << 16;
        entity->onGround        = false;
        entity->tileCollisions  = true;
        entity->collisionPlane  = 0;
        entity->collisionLayers = (1 << Zone->fgLow) | (1 << Zone->fgHigh);
        Music_TransitionTrack(TRACK_HBHBOSS, 0.0125);
        entity->visible    = true;
        entity->state      = HeavyRider_State_Unknown1;
        entity->velocity.x = 0;
        entity->timer      = 0;
    }
}

void HeavyRider_State_Unknown1(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessTileCollisions(entity, &HeavyRider->outerBox, &HeavyRider->innerBox);

    if (entity->onGround) {
        if (entity->velocity.y <= 0x20000) {
            entity->velocity.y = 0;
        }
        else {
            entity->velocity.y = (entity->velocity.y >> 1) - entity->velocity.y;
            if (entity->velocity.y > 0) {
                if (RSDK.Rand(0, 2) == 1) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 2, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
                    entity->state = HeavyRider_State_Unknown4;
                }
                entity->velocity.y = 0;
            }
            else {
                entity->onGround                                                        = false;
                RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, Flamethrower)->interval = -1;
                RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Flamethrower)->interval = -1;
            }
        }
    }
    else {
        entity->velocity.y += 0x3800;
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
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator5, true, 0);
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (entity->position.y == HeavyRider->field_24 && HeavyRider->field_3 != 4 && player1->onGround) {
        if (entity->velocity.x <= 0) {
            if (player1->position.x < entity->position.x) {
                int dist = entity->position.x - player1->position.x;
                if (player1->velocity.x > 0)
                    dist -= 8 * player1->velocity.x;
                if (dist < 0x480000) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &entity->animator5, true, 0);
                    RSDK.PlaySfx(HeavyRider->sfxJump, false, 255);
                    entity->onGround     = false;
                    entity->velocity.y   = -0x58000;
                    HeavyRider->field_18 = 1;
                    HeavyRider->field_14 = 0;
                }
            }
        }
        else {
            if (player1->position.x > entity->position.x) {
                int dist = player1->position.x - entity->position.x;
                if (player1->velocity.x < 0)
                    dist += 8 * player1->velocity.x;
                if (dist < 0x480000) {
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 11, &entity->animator5, true, 0);
                    RSDK.PlaySfx(HeavyRider->sfxJump, false, 255);
                    entity->onGround     = false;
                    entity->velocity.y   = -0x58000;
                    HeavyRider->field_18 = 1;
                    HeavyRider->field_14 = 0;
                }
            }
        }
    }

    if (entity->direction) {
        if (entity->groundVel > -0x40000)
            entity->groundVel -= 0x4000;
    }
    else {
        if (entity->groundVel < 0x40000)
            entity->groundVel += 0x4000;
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
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 1, &entity->animator1, true, 2);
            entity->timer           = 41;
            HeavyRider->chargeTimer = 0;
            entity->state           = HeavyRider_State_Unknown3;
            break;
        default: break;
    }

    if (entity->animator1.animationID != 2) {
        if (abs(entity->groundVel) > 0x48000 || HeavyRider->field_20 == 1) {
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &entity->animator2, false, 0);
            if (HeavyRider->field_20 == 1) {
                if (entity->onGround) {
                    if (!--HeavyRider->field_1B) {
                        HeavyRider->field_1B = 10;
                        CREATE_ENTITY(HeavyRider, intToVoid(HEAVYRIDER_FIREBALL), entity->position.x, entity->position.y);
                    }
                }
            }
        }
        else {
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
            if (HeavyRider->field_1B > 16)
                HeavyRider->field_1B = 16;

            if (!--HeavyRider->field_1B) {
                HeavyRider->field_1B    = 16;
                EntityHeavyRider *child = CREATE_ENTITY(HeavyRider, intToVoid(HEAVYRIDER_PUFF), entity->position.x, entity->position.y + 0x1C0000);
                if (entity->direction == FLIP_X)
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
        HeavyRider->debrisPos.x = entity->position.x;
        HeavyRider->debrisPos.y = entity->position.y;
        if (entity->direction)
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
    if (entity->type != HEAVYRIDER_JIMMY && entity->timer == 30) {
        RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 1, Flamethrower)->interval = 0;
        RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot + 2, Flamethrower)->interval = 0;
    }

    if (--entity->timer > 0) {
        if (!(entity->timer & 7)) {
            int channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        entity->timer = 30;
        entity->groundVel += (entity->groundVel >> 2) + (entity->groundVel >> 1);
        if (entity->type == HEAVYRIDER_JIMMY)
            entity->state = HeavyRider_State_Unknown1;
        else
            entity->state = HeavyRider_State_Charge;
    }
}

void HeavyRider_State_Unknown3(void)
{
    RSDK_THIS(HeavyRider);
    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    HeavyRider_State_Unknown1();

    if (--entity->timer > 0) {
        if (!(entity->timer & 7)) {
            int channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        entity->timer = 30;
        entity->groundVel += (entity->groundVel >> 2) + (entity->groundVel >> 1);
        entity->state = HeavyRider_State_Unknown1;
    }
}

void HeavyRider_State_Charge(void)
{
    RSDK_THIS(HeavyRider);

    HeavyRider_State_Unknown1();

    if (--entity->timer == 16) {
        HeavyRider->field_1B = 10;
        HeavyRider->field_20 = 1;
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 8, &entity->animator4, true, 0);
    }
    else if (entity->timer <= 0) {
        for (int i = 0; i < 0x60; ++i) {
            int x                 = RSDK.Rand(-128, 129);
            int y                 = 2 * RSDK.Rand(4, 16);
            EntityDebris *debris  = CREATE_ENTITY(Debris, NULL, (x << 16) + entity->position.x, (RSDK_screens->position.y - y) << 16);
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
        entity->state = HeavyRider_State_Unknown1;
    }
}

void HeavyRider_State_Unknown4(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator4);
    RSDK.ProcessTileCollisions(entity, &HeavyRider->outerBox, &HeavyRider->innerBox);
    if (entity->onGround) {
        if (entity->velocity.y <= 0x20000) {
            if (!(Zone->timer & 3))
                RSDK.PlaySfx(HeavyRider->sfxSkid, false, 255);
            entity->velocity.y = 0;
        }
        else {
            entity->velocity.y = (entity->velocity.y >> 1) - entity->velocity.y;
            if ((entity->velocity.y >> 1) - entity->velocity.y <= 0)
                entity->onGround = false;
            else
                entity->velocity.y = 0;
        }
    }
    else {
        entity->velocity.y += 0x3800;
    }

    if (entity->direction) {
        if (entity->groundVel > -0x40000)
            entity->groundVel -= 0x4000;
    }
    else {
        if (entity->groundVel < 0x40000)
            entity->groundVel += 0x4000;
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &entity->animator1, true, 0);
        entity->direction ^= 1u;
        HeavyRider->field_4 = 1;
        entity->state       = HeavyRider_State_Unknown1;
    }
    HeavyRider->field_10 += 4;
    HeavyRider_CheckObjectCollisions();
}

void HeavyRider_State_Destroyed(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    HeavyRider_Explode();

    if (--entity->timer <= 0) {
        EntityHeavyRider *child = CREATE_ENTITY(HeavyRider, intToVoid(HEAVYRIDER_JIMMY), entity->position.x, entity->position.y);
        child->direction        = entity->direction;
        child->onGround         = false;
        child->tileCollisions   = true;
        child->collisionLayers  = (1 << Zone->fgLow) | (1 << Zone->fgHigh);
        child->collisionPlane   = 0;
        entity->velocity.y      = -0x60000;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x20000;
        else
            entity->velocity.x = 0x20000;
        RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 3, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->state = HeavyRider_State_Finish;
    }
}

void HeavyRider_State_Finish(void)
{
    RSDK_THIS(HeavyRider);

    if (++entity->timer < 45)
        HeavyRider_Explode();
    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (entity->velocity.y > 0)
        entity->drawOrder = Zone->drawOrderHigh;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        Music_TransitionTrack(TRACK_STAGE, 0.0125);

        entity->position.x      = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        entity->position.y      = (RSDK_screens->position.y - 48) << 16;
        EntityEggPrison *prison = CREATE_ENTITY(EggPrison, intToVoid(EGGPRISON_FLYING), entity->position.x, entity->position.y);
        prison->isPermanent     = true;
        prison->drawOrder       = Zone->drawOrderHigh;
        destroyEntity(entity);
    }
}

void HeavyRider_StateDraw_Unknown(void)
{
    RSDK_THIS(HeavyRider);
    Vector2 drawPos;

    if (HeavyRider->field_18) {
        drawPos = entity->position;
        drawPos.y += 2 * HeavyRider->field_14;
        entity->animator5.frameID = 3;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);

        entity->animator5.frameID = 2;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);

        drawPos.y -= HeavyRider->field_14;
        entity->animator5.frameID = 1;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }

    int drawX = 0;
    if (entity->direction) {
        if (entity->animator1.animationID == 2 && entity->animator1.frameID)
            drawX = entity->position.x + 0x160000;
        else
            drawX = entity->position.x - 0xE0000;
    }
    else if (entity->animator1.animationID == 2 && entity->animator1.frameID)
        drawX = entity->position.x - 0x160000;
    else
        drawX = entity->position.x + 0xE0000;

    int drawY = entity->position.y - 0x210000;
    drawPos.x = drawX;
    drawPos.y = drawY;

    if (HeavyRider->field_4) {
        if (HeavyRider->field_4 <= 4) {
            if (HeavyRider->field_10 >= 0x80) {
                entity->animator3.frameID = 2;
                drawPos.x                 = drawX + 0x4400 * RSDK.Cos256(HeavyRider->field_10);
                drawPos.y                 = drawY + 0x880 * RSDK.Sin256(2 * HeavyRider->field_10);
                entity->drawFX |= FX_SCALE;
                entity->scale.x = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                entity->scale.y = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                RSDK.DrawSprite(&entity->animator3, &drawPos, false);
                entity->drawFX &= ~FX_SCALE;
                HeavyRider->debrisPos     = drawPos;
                entity->animator3.frameID = 1;

                int angle = 0x4400;
                for (int i = 0; i < 8; ++i) {
                    angle -= 0x800;
                    drawPos.x = drawX + angle * RSDK.Cos256(HeavyRider->field_10);
                    drawPos.y = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->field_10);
                    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
                }
            }
        }
        else {
            if (HeavyRider->field_4 == 7) {
                entity->animator3.frameID = 2;
                RSDK.DrawSprite(&entity->animator3, &HeavyRider->debrisPos, false);
            }
        }
    }

    if (HeavyRider->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&entity->animator1, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    switch (HeavyRider->field_4) {
        case 1:
        case 2:
        case 3:
        case 4:
            if (HeavyRider->field_10 < 0x80) {
                entity->animator3.frameID = 1;
                int angle                 = 0x400;
                for (int i = 0; i < 8; ++i) {
                    angle += 0x800;
                    drawPos.x = drawX + angle * RSDK.Cos256(HeavyRider->field_10);
                    drawPos.y = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->field_10);
                    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
                }

                entity->animator3.frameID = 2;
                drawPos.x                 = drawX + angle * RSDK.Cos256(HeavyRider->field_10);
                drawPos.y                 = drawY + (angle >> 3) * RSDK.Sin256(2 * HeavyRider->field_10);
                entity->drawFX |= FX_SCALE;
                entity->scale.x = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                entity->scale.y = (RSDK.Sin256(HeavyRider->field_10) >> 2) + 0x200;
                RSDK.DrawSprite(&entity->animator3, &drawPos, false);
                entity->drawFX &= ~FX_SCALE;
                HeavyRider->debrisPos = drawPos;
            }
            RSDK.DrawSprite(&entity->animator4, NULL, false);
            break;
        case 5:
        case 6:
            RSDK.DrawSprite(&entity->animator3, &HeavyRider->debrisPos, false);
            RSDK.DrawSprite(&entity->animator4, NULL, false);
            break;
        default: RSDK.DrawSprite(&entity->animator4, NULL, false);
    }
}

void HeavyRider_State_PlaneSwitch(void)
{
    RSDK_THIS(HeavyRider);
    if (entity->affectPlayer) {
        foreach_active(Player, player)
        {
            if (abs(player->position.x - entity->position.x) < 0x100000 && abs(player->position.y - entity->position.y) < entity->size << 19) {
                if (player->velocity.x >= 0) {
                    player->collisionPlane = (entity->flags >> 3) & 1;
                    if (!(entity->flags & 4)) {
                        player->collisionLayers = 1 << Zone->fgLow;
                        Zone->fgLayers          = 1 << Zone->fgLow;
                    }
                    else {
                        player->collisionLayers = 1 << Zone->fgHigh;
                        Zone->fgLayers          = 1 << Zone->fgHigh;
                    }
                }
                else {
                    player->collisionPlane = (entity->flags >> 1) & 1;
                    if (!(entity->flags & 1)) {
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

    entity->visible = DebugMode->debugActive;
}

void HeavyRider_StateDraw1_Unknown(void)
{
    RSDK_THIS(HeavyRider);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= 0x80000;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3(&entity->position, &drawPos, entity->angle);

    entity->animator1.frameID = entity->flags & 3;
    for (int32 i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator1, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    drawPos.x = entity->position.x + 0x80000;
    drawPos.y = entity->position.y - (entity->size << 19);

    int32 dx  = (drawPos.x + 0x80000 - entity->position.x) >> 8;
    int32 dy  = (drawPos.y - entity->position.y) >> 8;
    drawPos.x = (dy * RSDK.Sin256(entity->angle)) + dx * RSDK.Cos256(entity->angle) + entity->position.x;
    drawPos.y = (dy * RSDK.Cos256(entity->angle)) - dx * RSDK.Sin256(entity->angle) + entity->position.y;

    entity->animator1.frameID = (entity->flags >> 2) & 3;
    for (int32 i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator1, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }
}

void HeavyRider_State_ChargeTrigger(void)
{
    RSDK_THIS(HeavyRider);

    entity->visible = DebugMode->debugActive;
}

void HeavyRider_StateDraw_Simple(void)
{
    RSDK_THIS(HeavyRider);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void HeavyRider_State_Fireball(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1) {
        destroyEntity(entity);
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &HeavyRider->hitbox8)) {
                Player_CheckElementalHit(player, entity, SHIELD_FIRE);
            }
        }
    }
}

void HeavyRider_State_Puff(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void HeavyRider_StateJimmy_Idle(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    if (++entity->timer >= 45) {
        entity->timer           = 38;
        HeavyRider->chargeTimer = 0;
        entity->state           = HeavyRider_StateJimmy_RevUp;
    }
}

void HeavyRider_StateJimmy_RevRelease(void)
{
    RSDK_THIS(HeavyRider);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessTileCollisions(entity, &HeavyRider->outerBox, &HeavyRider->innerBox);

    if (entity->onGround) {
        if (entity->velocity.y <= 0x20000)
            entity->velocity.y = 0;
        else
            entity->velocity.y = (entity->velocity.y >> 1) - entity->velocity.y;
    }
    else {
        entity->velocity.y += 0x3800;
    }

    if (entity->direction)
        entity->groundVel -= 0x8000;
    else
        entity->groundVel += 0x8000;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void HeavyRider_StateJimmy_RevUp(void)
{
    RSDK_THIS(HeavyRider);
    float chargeSpeeds[] = { 1.0, 1.1926451, 1.4150375, 1.6780719, 2.0 };

    RSDK.ProcessAnimation(&entity->animator1);

    if (--entity->timer > 0) {
        if (!(entity->timer & 7)) {
            int channel = RSDK.PlaySfx(HeavyRider->sfxCharge, false, 255);
            RSDK.SetChannelAttributes(channel, 1.0, 0.0, chargeSpeeds[HeavyRider->chargeTimer]);
            ++HeavyRider->chargeTimer;
        }
    }
    else {
        RSDK.PlaySfx(HeavyRider->sfxLaunch, false, 255);
        entity->timer = 30;
        entity->state = HeavyRider_StateJimmy_RevRelease;
        if (entity->direction == FLIP_NONE)
            entity->groundVel = 0x40000;
        else
            entity->groundVel = -0x40000;
    }
}

#if RETRO_INCLUDE_EDITOR
void HeavyRider_EditorDraw(void)
{
    RSDK_THIS(HeavyRider);

    switch (entity->type) {
        case HEAVYRIDER_RIDER:
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 5, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(HeavyRider->aniFrames, 7, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            HeavyRider->field_10  = 0x80;
            HeavyRider->field_4   = 1;
            HeavyRider->field_1B  = 16;
            HeavyRider->field_20  = 0;
            HeavyRider->field_24  = entity->position.y;
            HeavyRider->field_3   = 0;
            HeavyRider->field_18  = 0;
            HeavyRider->field_14  = 0;
            entity->stateDraw     = HeavyRider_StateDraw_Unknown;
            break;
        case HEAVYRIDER_PLANESWITCH:
            RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->animator1, true, 0);
            entity->updateRange.x = 0x200000;
            entity->updateRange.y = (entity->size + 0x400) << 11;
            entity->stateDraw     = HeavyRider_StateDraw1_Unknown;
            break;
        case HEAVYRIDER_FORCECHARGE_LEFT:
        case HEAVYRIDER_FORCECHARGE_RIGHT:
            RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->animator1, true, ((entity->type - 2) >> 1) + 4);
            entity->updateRange.x = 0x200000;
            entity->updateRange.y = 0x200000;
            entity->state         = HeavyRider_State_ChargeTrigger;
            entity->stateDraw     = HeavyRider_StateDraw_Simple;
            break;
        default: break;
    }

    StateMachine_Run(entity->stateDraw);

    if (entity->type == HEAVYRIDER_RIDER && showGizmos()) {
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

