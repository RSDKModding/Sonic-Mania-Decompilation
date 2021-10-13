#include "SonicMania.h"

ObjectIce *Ice;

void Ice_Update(void)
{
    RSDK_THIS(Ice);
    StateMachine_Run(entity->state);
}

void Ice_LateUpdate(void) {}

void Ice_StaticUpdate(void)
{
    for (int32 i = 0; i < Player->playerCount; ++i) {
        if (Ice->playerTimers[i] > 0)
            Ice->playerTimers[i]--;
    }
}

void Ice_Draw(void)
{
    RSDK_THIS(Ice);
    StateMachine_Run(entity->stateDraw);
}

void Ice_Create(void *data)
{
    RSDK_THIS(Ice);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawFX    = FX_FLIP;
        entity->drawOrder = Zone->playerDrawLow + 1;
        entity->visible   = true;

        if (data) {
            entity->active = ACTIVE_NORMAL;

            switch (voidToInt(data)) {
                case 1:
                    entity->hitbox1.left   = -24;
                    entity->hitbox1.top    = -24;
                    entity->hitbox1.right  = 24;
                    entity->hitbox1.bottom = 24;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERBLOCK, &entity->animator1, true, 0);
                    entity->isPermanent = true;
                    entity->state       = Ice_State_PlayerBlock;
                    entity->stateDraw   = Ice_StateDraw_PlayerBlock;
                    break;
                case 2:
                    entity->updateRange.x  = 0x800000;
                    entity->updateRange.y  = 0x800000;
                    entity->hitbox1.left   = -19;
                    entity->hitbox1.top    = -110;
                    entity->hitbox1.right  = 19;
                    entity->hitbox1.bottom = 0;
                    entity->hitbox3.left   = -19;
                    entity->hitbox3.top    = -110;
                    entity->hitbox3.right  = 19;
                    entity->hitbox3.bottom = 0;
                    entity->dwordE4        = 240;
                    entity->alpha          = 128;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARBLOCK, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARGLINT, &entity->animator4, true, 0);
                    entity->state     = Ice_State_Pillar;
                    entity->stateDraw = Ice_StateDraw_Pillar;
                    break;
                case 3:
                    entity->updateRange.x = 0x400000;
                    entity->updateRange.y = 0x400000;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_SHARD, &entity->animator1, true, 0);
                    entity->state     = Ice_State_Shard;
                    entity->stateDraw = Ice_StateDraw_Shard;
                    break;
            }
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_ICEBLOCK, &entity->animator1, true, entity->size);
            switch (entity->type) {
                case 0: RSDK.SetSpriteAnimation(0xFFFF, ICEANI_ICEBLOCK, &entity->animator2, true, 0); break;
                case 1: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &entity->animator2, true, 0); break;
                case 2: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &entity->animator2, true, 1); break;
                case 3: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &entity->animator2, true, 2); break;
                case 4:
                    entity->subType = entity->subType & 3;
                    RSDK.SetSpriteAnimation(Spikes->spriteIndex, entity->subType >> 1, &entity->animator2, true, 0);
                    if (!entity->size) {
                        switch (entity->subType) {
                            case 0:
                                entity->dwordDC.y = 0x40000;
                                entity->subFlip   = 0;
                                break;
                            case 1:
                                entity->dwordDC.y = -0x40000;
                                entity->subFlip   = 2;
                                break;
                            case 2:
                                entity->dwordDC.x = -0x80000;
                                entity->subFlip   = 0;
                                break;
                            case 3:
                                entity->dwordDC.x = 0x80000;
                                entity->subFlip   = 1;
                                break;
                            default: break;
                        }
                    }
                    break;
                case 18:
                    if (entity->subType >= 3)
                        RSDK.SetSpriteAnimation(Spring->spriteIndex, (entity->subType - 3), &entity->animator2, true, 0);
                    else
                       RSDK.SetSpriteAnimation(IceSpring->animID, entity->subType, &entity->animator2, true, 0);
                    if (entity->size) {
                        switch (entity->subType) {
                            case 0:
                            case 3:
                            case 4:
                                entity->dwordDC.x = 0;
                                if (!(entity->subFlip & 2))
                                    entity->dwordDC.y = 0x80000;
                                else
                                    entity->dwordDC.y = -0x80000;
                                break;
                            case 1:
                            case 5:
                            case 6:
                                entity->dwordDC.y = 0;
                                if (!(entity->subFlip & 1))
                                    entity->dwordDC.x = -0x80000;
                                else
                                    entity->dwordDC.x = 0x80000;
                                break;
                            case 2:
                            case 7:
                            case 8:
                                entity->dwordDC.x = 0;
                                if (!(entity->subFlip & 2))
                                    entity->dwordDC.y = -0x10000;
                                else
                                    entity->dwordDC.y = 0x10000;
                                break;
                            default: break;
                        }
                    }
                    else {
                        switch (entity->subType) {
                            case 0:
                            case 3:
                            case 4:
                                entity->dwordDC.x = 0;
                                if (!(entity->subFlip & 2))
                                    entity->dwordDC.y = 0xC0000;
                                else
                                    entity->dwordDC.y = -0xC0000;
                                break;
                            case 1:
                            case 5:
                            case 6:
                                entity->dwordDC.y = 0;
                                if (!(entity->subFlip & 1))
                                    entity->dwordDC.x = -0x100000;
                                else
                                    entity->dwordDC.x = 0x100000;
                                break;
                            case 2:
                            case 7:
                            case 8:
                                if (!(entity->subFlip & 1))
                                    entity->dwordDC.x = -0x80000;
                                else
                                    entity->dwordDC.x = 0x80000;
                                if (!(entity->subFlip & 2))
                                    entity->dwordDC.y = 0x40000;
                                else
                                    entity->dwordDC.y = -0x40000;
                                break;
                            default: break;
                        }
                    }
                    break;
                default:
                    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 0, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 0, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(ItemBox->spriteIndex, 2, &entity->animator3, true, (entity->type > 12 ? 2 : 0) + entity->type - 5);
                    if (entity->type != 12) {
                        if (globals->gameMode == MODE_COMPETITION) {
                            if (globals->itemMode == 1) {
                                entity->type              = 16;
                                entity->animator3.frameID = 13;
                            }
                            else if (globals->itemMode == 2) {
                                entity->type              = 15;
                                entity->animator3.frameID = 12;
                            }
                        }
                    }
                    else if (globals->gameMode != MODE_TIMEATTACK) {
                        switch (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->characterID) {
                            case ID_TAILS: ++entity->animator3.frameID; break;
                            case ID_KNUCKLES: entity->animator3.frameID += 2; break;
#if RETRO_USE_PLUS
                            case ID_MIGHTY: entity->animator3.frameID += 8; break;
                            case ID_RAY: entity->animator3.frameID += 9; break;
#endif
                            default: break;
                        }

                        if (globals->gameMode == MODE_COMPETITION) {
                            if (globals->itemMode == 1) {
                                entity->type              = 16;
                                entity->animator3.frameID = 13;
                            }
                            else if (globals->itemMode == 2) {
                                entity->type              = 15;
                                entity->animator3.frameID = 12;
                            }
                        }
                    }
                    else {
                        entity->type              = 5;
                        entity->animator3.frameID = 0;
                    }
                    break;
            }

            if (entity->size) {
                if (entity->size == 1) {
                    entity->hitbox1.left   = -16;
                    entity->hitbox1.top    = -16;
                    entity->hitbox1.right  = 16;
                    entity->hitbox1.bottom = 16;
                }
            }
            else {
                entity->hitbox1.left   = -24;
                entity->hitbox1.top    = -20;
                entity->hitbox1.right  = 24;
                entity->hitbox1.bottom = 20;
            }

            entity->hitbox2.top    = entity->hitbox1.top;
            entity->hitbox3.left   = entity->hitbox1.left;
            entity->hitbox3.right  = entity->hitbox1.right;
            entity->hitbox3.bottom = entity->hitbox1.bottom + 8;
            entity->hitbox3.top    = entity->hitbox1.top - 8;
            entity->hitbox2.left   = entity->hitbox1.left - 8;
            entity->hitbox2.right  = entity->hitbox1.right + 8;
            entity->hitbox2.bottom = entity->hitbox1.bottom + 8;
            entity->alpha          = 384;
            entity->state          = Ice_State_Pillar;
            entity->stateDraw      = Ice_StateDraw_Unknown1;
        }
    }
}

void Ice_StageLoad(void)
{
    if (RSDK.CheckStageFolder("PSZ1")) {
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ice.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("PSZ2")) {
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Ice.bin", SCOPE_STAGE);
    }

    Ice->hitbox1.left     = -15;
    Ice->hitbox1.top      = -24;
    Ice->hitbox1.right    = 15;
    Ice->hitbox1.bottom   = 24;
    Ice->hitbox2.left     = -16;
    Ice->hitbox2.top      = -24;
    Ice->hitbox2.right    = 16;
    Ice->hitbox2.bottom   = 24;
    Ice->playerTimers[0]  = 0;
    Ice->playerTimers[1]  = 0;
    Ice->playerTimers[2]  = 0;
    Ice->playerTimers[3]  = 0;
    Ice->sfxFreeze        = RSDK.GetSFX("PSZ/Freeze.wav");
    Ice->sfxLedgeBreak    = RSDK.GetSFX("Stage/LedgeBreak.wav");
    Ice->sfxWindowShatter = RSDK.GetSFX("Stage/WindowShatter.wav");
    Ice->sfxStruggle      = RSDK.GetSFX("PSZ/Struggle.wav");
    Zone->timeOverState   = Ice_TimeOverCB;
    Zone_AddCallback(Ice_ZoneCB);
}

void Ice_ZoneCB(void)
{
#if RETRO_USE_PLUS
    EntityPlayer *player = RSDK_GET_ENTITY(Zone->playerID, Player);
#else
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#endif

    if (player->state == Ice_State_FrozenPlayer) {
#if RETRO_USE_PLUS
        Zone->playerFlags[Zone->playerID] = false;
#else
        Ice_Unknown8((Entity *)player);
#endif
    }
}

void Ice_FreezePlayer(void *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    RSDK_THIS(Ice);
    if (!Zone->field_15C && player->state != Ice_State_FrozenPlayer && (player->shield != SHIELD_FIRE || player->invincibleTimer > 0)) {
        EntityIce *ice = CREATE_ENTITY(Ice, intToVoid(1), player->position.x, player->position.y);
        ice->playerPtr = (Entity *)player;
#if RETRO_USE_PLUS
        switch (player->characterID) {
            case ID_SONIC: ice->animationID = 4 * (player->superState == SUPERSTATE_SUPER) + ICEANI_SONICIDLE; break;
            case ID_TAILS: ice->animationID = ICEANI_TAILSIDLE; break;
            case ID_KNUCKLES: ice->animationID = ICEANI_KNUXIDLE; break;
            case ID_MIGHTY: ice->animationID = ICEANI_MIGHTYIDLE; break;
            case ID_RAY: ice->animationID = ICEANI_RAYIDLE; break;
            default: break;
        }
#else
        switch (player->characterID) {
            case ID_SONIC: ice->animationID = ICEANI_SONIC; break;
            case ID_TAILS: ice->animationID = ICEANI_TAILS; break;
            case ID_KNUCKLES: ice->animationID = ICEANI_KNUX; break;
            default: break;
        }
#endif
        RSDK.SetSpriteAnimation(Ice->aniFrames, ice->animationID, &ice->animator2, true, 0);
        RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &ice->animator3, true, 0);
        ice->alpha       = 128;
        ice->isPermanent = true;
        player->velocity.x >>= 1;
        player->groundVel >>= 1;
        player->playerAnimator.animationSpeed = 0;
        player->visible                       = false;
        player->nextAirState                  = StateMachine_None;
        player->nextGroundState               = StateMachine_None;
        player->state                         = Ice_State_FrozenPlayer;
        player->velocity.y                    = 0;
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
        player->entPtr    = (Entity *)entity;
        player->field_218 = ice;
        if (!entity->collisionMode) {
            player->position.y += Player_GetHitbox(player)->bottom - Ice->hitbox2.bottom;
        }
        player->blinkTimer     = 0;
        player->outerbox       = &Ice->hitbox2;
        player->innerbox       = &Ice->hitbox1;
        player->timer          = 0;
        player->abilityTimer   = 0;
        player->spindashCharge = 0;
        RSDK.PlaySfx(Ice->sfxFreeze, false, 255);
    }
}

bool32 Ice_Unknown3(void)
{
    RSDK_THIS(Player);

    if (abs(entity->skidding) >= 0x50000) {
        if (abs(entity->pushing) <= abs(entity->skidding)) {
            if (entity->skidding < 0)
                entity->velocity.x = 0x18000;
            else
                entity->velocity.x = -0x18000;

            Ice_Unknown8((Entity *)entity);
            return true;
        }
    }
    return false;
}

bool32 Ice_Unknown4(void)
{
    RSDK_THIS(Player);

    if (abs(entity->pushing) >= 0x80000) {
        if (abs(entity->skidding) <= abs(entity->pushing)) {
            if (entity->pushing > 0) {
                if ((entity->angle & 0xFF) < 0x20) {
                    Ice_Unknown8(RSDK_sceneInfo->entity);
                    return true;
                }
            }
            if (entity->pushing && (entity->angle + 0x80) < 0x20) {
                Ice_Unknown8(RSDK_sceneInfo->entity);
                return true;
            }
        }
    }
    return false;
}

void Ice_State_FrozenPlayer(void)
{
    RSDK_THIS(Player);

    bool32 flag = entity->onGround;
    int32 skid    = entity->groundVel;
    bool32 groundFlag    = entity->groundedStore;

    if (!flag) {
        flag |= entity->groundedStore || entity->velocity.x;
        skid = entity->velocity.x;
        groundFlag = entity->velocity.y;
    }
    if (!flag) {
        flag |= abs(entity->skidding) < 0x50000 || !Ice_Unknown3();
        skid       = entity->velocity.x;
        groundFlag = entity->velocity.y;
    }

    if (flag) {
        if (entity->onGround) {
            if (!entity->groundVel) {
                if (Ice_Unknown3())
                    return;
            }
        }

        entity->skidding = skid;
        if (!groundFlag) {
            if (Ice_Unknown4())
                return;
        }

        entity->pushing = entity->velocity.y;
        if (entity->shield == SHIELD_FIRE && entity->invincibleTimer <= 0) {
            entity->state      = Player_State_Air;
            entity->velocity.y = -entity->jumpStrength >> 1;
            entity->onGround   = false;
            RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, true, 0);
            Ice_ShatterGenerator(24, 20, 64, 0, 0, 2);
            RSDK.PlaySfx(Ice->sfxWindowShatter, 0, 255);
            Ice->playerTimers[RSDK.GetEntityID(entity)] = 30;
            entity->skidding                            = 0;
            entity->pushing                             = 0;
            entity->timer                               = 0;
            entity->abilityTimer                        = 0;
            entity->spindashCharge                      = 0;
        }
        else {
            if (!entity->invincibleTimer)
                entity->invincibleTimer = -1;

            if (entity->spindashCharge > 0) {
                entity->spindashCharge--;
            }
            else if (entity->jumpPress) {
                entity->spindashCharge = 15;
                if (++entity->timer > 4) {
                    entity->state      = Player_State_Air;
                    entity->velocity.y = -entity->jumpStrength >> 1;
                    entity->onGround   = false;
                    RSDK.SetSpriteAnimation(entity->spriteIndex, ANI_JUMP, &entity->playerAnimator, true, 0);
                    Ice_ShatterGenerator(24, 20, 64, 0, 0, 2);
                    RSDK.PlaySfx(Ice->sfxWindowShatter, 0, 255);
                    Ice->playerTimers[RSDK.GetEntityID(entity)] = 30;
                    entity->skidding                            = 0;
                    entity->pushing                             = 0;
                    entity->timer                               = 0;
                    entity->abilityTimer                        = 0;
                    entity->spindashCharge                      = 0;
                }
                else {
                    Ice_ShatterGenerator(24, 20, 8, 0, 0, 0);
                    RSDK.PlaySfx(Ice->sfxStruggle, 0, 255);
                }
            }
        }

        if (entity->state == Ice_State_FrozenPlayer) {
            if (entity->onGround) {
                int32 rollFric            = entity->rollingFriction;
                entity->left            = false;
                entity->right           = false;
                entity->rollingFriction = 0;
                Player_HandleRollDeceleration();
                if (!entity->groundVel)
                    entity->groundVel += (5000 * RSDK.Sin256(entity->angle)) >> 8;
                if (entity->camera)
                    entity->camera->offsetYFlag = false;
                entity->jumpAbilityTimer = 0;
                entity->rollingFriction  = rollFric;
                if (entity->state == Player_State_Ground)
                    entity->state = Ice_State_FrozenPlayer;
            }
            else {
                entity->velocity.y += 0x3800;
            }
        }
    }
}

void Ice_ShatterGenerator(int32 xr, int32 yr, int32 count, int32 velX, int32 velY, int32 a6)
{
    RSDK_THIS(Ice);
    if (a6 > 0)
        count >>= 1;
    for (int32 i = 0; i < maxVal(0, count); ++i) {
        int32 randY                     = RSDK.Rand(-yr, yr + 1) << 16;
        int32 randX                     = RSDK.Rand(-xr, xr + 1) << 16;
        EntityIce *ice                = CREATE_ENTITY(Ice, intToVoid(3), randX + entity->position.x, randY + entity->position.y);
        ice->velocity.x               = velX + (RSDK.Rand(-6, 8) << 15);
        ice->velocity.y               = velY + (RSDK.Rand(-10, 2) << 15);
        ice->direction                = RSDK.Rand(0, 4);
        ice->animator1.animationSpeed = RSDK.Rand(1, 4);
        if (a6) {
            if (RSDK.Rand(0, 2)) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PIECE, &ice->animator1, true, 0);
                ice->velocity.x = (ice->velocity.x >> 1) + (ice->velocity.x >> 2);
                ice->velocity.y = (ice->velocity.y >> 1) + (ice->velocity.y >> 2);
            }
        }
    }
}

void Ice_Unknown7(int32 velX, Entity *p, int32 velY)
{
    EntityPlayer *player = (EntityPlayer *)p;
    RSDK_THIS(Ice);
    if (entity->type == 4 || (entity->type == 18 && entity->subType < 3))
        entity->type = 0;
    EntityItemBox *itemBox = (EntityItemBox *)Ice_Shatter(entity, velX, velY);
    if (player && itemBox) {
        if (player->sidekick)
            player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        itemBox->contentsPos.x = itemBox->position.x;
        itemBox->contentsPos.y = itemBox->position.y - 0x30000;
        ItemBox_Break(itemBox, player);
    }
}

void Ice_Unknown8(Entity *p)
{
    EntityPlayer *player = (EntityPlayer *)p;
    RSDK.PlaySfx(Ice->sfxWindowShatter, 0, 255);
    Ice_ShatterGenerator(24, 20, 64, 0, 0, 2);
    Ice->playerTimers[RSDK.GetEntityID(player)] = 30;

    player->skidding       = 0;
    player->pushing        = 0;
    player->timer          = 0;
    player->abilityTimer   = 0;
    player->spindashCharge = 0;
    player->visible        = true;
    if (player->state != Player_State_Die) {
        player->outerbox = NULL;
        player->innerbox = NULL;
        player->state    = Player_State_Hit;
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, true, 0);
        player->velocity.y = -0x38000;
        player->onGround   = false;
        if (player->gravityStrength == 0x1000) {
            player->velocity.x >>= 1;
            player->velocity.y = -0x1C000;
        }
    }
}

Entity *Ice_Shatter(EntityIce *ice, int32 velX, int32 velY)
{
    RSDK_THIS(Ice);
    EntityItemBox *itemBox = NULL;

    RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
    if (entity->animator1.animationID == ICEANI_PILLARBLOCK) {
        entity->position.y -= 0x370000;
        Ice_ShatterGenerator(19, 55, 96, velX, velY, 2);
    }
    else if (entity->size) {
        Ice_ShatterGenerator(16, 16, 16, velX, velY, 2);
    }
    else {
        Ice_ShatterGenerator(24, 20, 64, velX, velY, 2);
    }

    int32 count = 0;
    switch (ice->type) {
        case 0: break;
        case 1: count = 1;
        case 2:
            if (ice->type == 2)
                count = 3;
        case 3: {
            if (ice->type == 3)
                count = 5;

            int32 angle = 16 * (12 - (count >> 1));
            for (int32 i = 0; i < count; ++i) {
                EntityRing *ring = CREATE_ENTITY(Ring, intToVoid(1), ice->position.x, ice->position.y);
                ring->velocity.x = velX + 640 * RSDK.Cos256(angle);
                ring->velocity.y = velY + 640 * RSDK.Sin256(angle);
                ring->position.x += ring->velocity.x * (3 * ice->type - 3);
                ring->position.y += ring->velocity.y * (3 * ice->type - 3);
                ring->timer          = 32;
                ring->collisionPlane = 0;
                ring->inkEffect      = INK_ALPHA;
                ring->alpha          = 256;
                ring->state          = Ring_State_Bounce;
                ring->position.y += (0x50000 * ice->type) - 0x50000;
                ring->animator.animationSpeed = 512;
                ring->stateDraw               = Ring_StateDraw_Normal;
                angle += 16;
            }
            break;
        }
        case 4: CREATE_ENTITY(Spikes, intToVoid(entity->subType), ice->position.x + entity->dwordDC.x, ice->position.y + entity->dwordDC.y); break;
        case 18:
            if (entity->subType >= 3)
                CREATE_ENTITY(Spring, intToVoid((entity->subFlip << 8) - 3 + entity->subType), ice->position.x + entity->dwordDC.x,
                              ice->position.y + entity->dwordDC.y);
            else
                CREATE_ENTITY(IceSpring, intToVoid(entity->subType + (entity->subFlip << 8)), ice->position.x + entity->dwordDC.x,
                              ice->position.y + entity->dwordDC.y);
            break;
        default: {
            itemBox             = CREATE_ENTITY(ItemBox, intToVoid(ice->type - 5 + (ice->type > 12 ? 2 : 0)), ice->position.x, ice->position.y);
            itemBox->velocity.y = -0x18000;
            itemBox->state      = ItemBox_State_Falling;
            break;
        }
    }
    Ice_Unknown11();
    destroyEntity(ice);
    return (Entity *)itemBox;
}

void Ice_TimeOverCB(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player->state == Ice_State_FrozenPlayer)
        Ice_Unknown8((Entity *)player);
}

void Ice_Unknown11(void)
{
    RSDK_THIS(Ice);
    foreach_all(Ice, ice)
    {
        if (ice != entity && ice->state == Ice_State_Pillar && RSDK.CheckObjectCollisionTouchBox(entity, &entity->hitbox1, ice, &ice->hitbox2)) {
            ice->state = Ice_Unknown13;
        }
    }
}

void Ice_State_Pillar(void)
{
    RSDK_THIS(Ice);

    foreach_active(Player, player)
    {
        bool32 flag = true;
        int32 playerX = player->position.x;
        int32 playerY = player->position.y;

        if (player->state == Ice_State_FrozenPlayer) {
            int32 side = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox3, player, &Ice->hitbox2, false);
            switch (side) {
                case 1:
                    if (player->velocity.y < 0x40000) {
                        player->position.x = playerX;
                        player->position.y = playerY;
                        entity->position.x -= entity->playerPos.x;
                        entity->position.y -= entity->playerPos.y;
                        if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1) {
                            player->position.x += entity->playerPos.x;
                            player->position.y += entity->playerPos.y;
                        }
                        entity->position.x += entity->playerPos.x;
                        entity->position.y += entity->playerPos.y;
                        flag = false;
                    }
                    else {
                        Ice_Unknown7(0, (Entity *)player, 0);
                        player->velocity.y = -0x20000;
                        foreach_return;
                    }
                    break;
                case 2:
                    if (player->velocity.x < 0x20000) {
                        player->position.x = playerX;
                        player->position.y = playerY;
                        entity->position.x -= entity->playerPos.x;
                        entity->position.y -= entity->playerPos.y;
                        if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1) {
                            player->position.x += entity->playerPos.x;
                            player->position.y += entity->playerPos.y;
                        }
                        entity->position.x += entity->playerPos.x;
                        entity->position.y += entity->playerPos.y;
                        flag = false;
                    }
                    else {
                        Ice_Unknown7(player->velocity.x >> 1, (Entity *)player, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    break;
                case 3:
                    if (player->velocity.x > -0x20000) {
                        player->position.x = playerX;
                        player->position.y = playerY;
                        entity->position.x -= entity->playerPos.x;
                        entity->position.y -= entity->playerPos.y;
                        if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1) {
                            player->position.x += entity->playerPos.x;
                            player->position.y += entity->playerPos.y;
                        }
                        entity->position.x += entity->playerPos.x;
                        entity->position.y += entity->playerPos.y;
                        flag = false;
                    }
                    else {
                        Ice_Unknown7(player->velocity.x >> 1, (Entity *)player, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    break;
                case 4:
                    if (player->velocity.y > -0x40000) {
                        player->position.x = playerX;
                        player->position.y = playerY;
                        entity->position.x -= entity->playerPos.x;
                        entity->position.y -= entity->playerPos.y;
                        if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1) {
                            player->position.x += entity->playerPos.x;
                            player->position.y += entity->playerPos.y;
                        }
                        entity->position.x += entity->playerPos.x;
                        entity->position.y += entity->playerPos.y;
                        flag = false;
                    }
                    else {
                        Ice_Unknown7(0, (Entity *)player, player->velocity.y >> 1);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    break;
                default:
                    player->position.x = playerX;
                    player->position.y = playerY;
                    entity->position.x -= entity->playerPos.x;
                    entity->position.y -= entity->playerPos.y;
                    if (Player_CheckCollisionBox(player, entity, &entity->hitbox1) == 1) {
                        player->position.x += entity->playerPos.x;
                        player->position.y += entity->playerPos.y;
                    }
                    entity->position.x += entity->playerPos.x;
                    entity->position.y += entity->playerPos.y;
                    flag = false;
                    break;
            }
        }

        if (flag) {
            int32 side = RSDK.CheckObjectCollisionBox(entity, &entity->hitbox1, player, &Ice->hitbox2, false);
            if (side >= 2) {
                if (side <= 3) {
                    if (entity->knuxSmash && player->characterID == ID_KNUCKLES) {
                        Ice_Shatter(entity, 0, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    else if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0) {
                        if (RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield)->animator.animationID == 2) {
                            if (player->position.x >= entity->position.x) {
                                if (player->velocity.x <= -0x78000) {
                                    Ice_Unknown7(player->velocity.x, (Entity *)player, 0);
                                    player->position.x = playerX;
                                    player->position.y = playerY;
                                    foreach_return;
                                }
                            }
                            else if (player->velocity.x >= 0x78000) {
                                Ice_Unknown7(player->velocity.x, (Entity *)player, 0);
                                player->position.x = playerX;
                                player->position.y = playerY;
                                foreach_return;
                            }
                        }
                    }
                }
                else if (side == 4 && entity->bottomSmash) {
                    Ice_Shatter(entity, 0, 0);
                    player->position.x = playerX;
                    player->position.y = playerY;
                    foreach_return;
                }
            }

            player->position.x = playerX;
            player->position.y = playerY;
            entity->position.x -= entity->playerPos.x;
            entity->position.y -= entity->playerPos.y;
            int32 prevVel = player->velocity.y;
            side        = Player_CheckCollisionBox(player, entity, &entity->hitbox1);
            if (side) {
                if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !entity->dwordE4) {
                    if (entity->animator1.animationID == ICEANI_PILLARBLOCK) {
                        entity->position.y -= 0x370000;
                        Ice_ShatterGenerator(19, 55, 36, 0, 0, 0);
                    }
                    else if (entity->size) {
                        Ice_ShatterGenerator(19, 17, 6, 0, 0, 0);
                    }
                    else {
                        Ice_ShatterGenerator(19, 20, 12, 0, 0, 0);
                    }

                    entity->animator1.frameID += 2;
                    entity->dwordE4 = 15;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_LARGEGLINT + entity->size, &entity->animator4, true, 0);
                }

                if (side == 4) {
                    if (player->onGround && !player->collisionMode && entity->playerPos.y >= 0) {
                        if (player->shield == SHIELD_FIRE || player->invincibleTimer) {
                            player->position.x = playerX;
                            player->position.y = playerY;
                            Ice_Shatter(entity, 0, 0);
                            foreach_return;
                        }
                        player->hurtFlag = 1;
                    }
                }
                else if (side == 1 && player->velocity.y >= 0) {
                    if (entity->animator1.animationID == ICEANI_PILLARBLOCK) {
                        Player_CheckHit(player, entity);
                    }
                    else {
                        if ((player->playerAnimator.animationID == ANI_JUMP || player->state == Player_State_DropDash) && !player->sidekick) {
                            Ice_Shatter(entity, 0, 0);
                            player->velocity.y = -0x30000;
                            player->onGround   = false;
                            foreach_return;
                        }
#if RETRO_USE_PLUS
                        else if (player->state == Player_State_MightyHammerDrop && !player->sidekick) {
                            Ice_Unknown7(0, (Entity *)player, player->velocity.y);
                            player->velocity.y = prevVel - 0x10000;
                            player->onGround   = false;
                            foreach_return;
                        }
#endif
                    }
                    player->position.x += entity->playerPos.x;
                    player->position.y += entity->playerPos.y;
                }
            }
            entity->position.x += entity->playerPos.x;
            entity->position.y += entity->playerPos.y;
        }
    }

    RSDK.ProcessAnimation(&entity->animator4);
    if (entity->animator1.animationID == ICEANI_PILLARBLOCK) {
        if (!RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, entity->hitbox1.bottom << 16, true)
            && entity->state == Ice_State_Pillar) {
            entity->state = Ice_Unknown13;
        }
        RSDK.ProcessAnimation(&entity->animator1);
        entity->dwordE4--;
        if (entity->dwordE4 == 54) {
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARGLINT, &entity->animator4, true, 1);
        }
        else if (!entity->dwordE4)
            Ice_Shatter(entity, 0, 0);
    }
    else {
        if (entity->dwordE4 > 0) {
            entity->dwordE4--;
            if (!entity->dwordE4) {
                if (entity->type == 4 || (entity->type == 18 && entity->subType < 3))
                    entity->type = 0;
                Ice_Shatter(entity, 0, 0);
            }
        }
    }
}

void Ice_Unknown13(void)
{
    RSDK_THIS(Ice);
    Ice_Unknown11();
    entity->timer  = 15;
    entity->state  = Ice_Unknown14;
    entity->active = ACTIVE_NORMAL;
    Ice_State_Pillar();
}

void Ice_Unknown14(void)
{
    RSDK_THIS(Ice);
    if (!--entity->timer) {
        Ice_Unknown11();
        entity->state = Ice_Unknown15;
    }
    Ice_Unknown15();
    entity->velocity.y = 0;
}

void Ice_Unknown15(void)
{
    RSDK_THIS(Ice);

    entity->playerPos.y = -(int32)(entity->position.y & 0xFFFF0000);
    entity->position.y  = entity->velocity.y + entity->position.y;
    entity->playerPos.y += (entity->velocity.y + entity->position.y) & 0xFFFF0000;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, entity->hitbox1.bottom << 16, true)) {
        entity->velocity.y = 0;
        entity->state      = Ice_State_Pillar;
        Ice_State_Pillar();
    }
    else {
        int32 velY = entity->velocity.y;
        foreach_all(Ice, ice)
        {
            if (ice != entity) {
                if ((ice->stateDraw == Ice_StateDraw_Unknown1 || ice->stateDraw == Ice_StateDraw_PlayerBlock)
                    && RSDK.CheckObjectCollisionPlatform(ice, &ice->hitbox1, entity, &entity->hitbox1, true)) {
                    entity->velocity.y = 0;
                    if (ice->state == Ice_State_Pillar) {
                        entity->active = ACTIVE_BOUNDS;
                        entity->state  = Ice_State_Pillar;
                    }
                }
            }
        }

        foreach_all(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionPlatform(spikes, &spikes->hitbox, entity, &entity->hitbox1, true)) {
                entity->velocity.y = 0;
                entity->active     = ACTIVE_BOUNDS;
                entity->state      = Ice_State_Pillar;
            }
        }

        foreach_all(ItemBox, itemBox)
        {
            if ((itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling)
                && RSDK.CheckObjectCollisionPlatform(itemBox, &ItemBox->hitbox, entity, &entity->hitbox1, true)) {
                entity->velocity.y = 0;
                if (itemBox->onGround) {
                    entity->active = ACTIVE_BOUNDS;
                    entity->state  = Ice_State_Pillar;
                }
            }
        }

        if (!entity->velocity.y && velY >= 0x60000) {
            if (ice) {
                if (ice->stateDraw == Ice_StateDraw_PlayerBlock) {
                    Ice_Unknown8((Entity *)ice->playerPtr);
                }
                else {
                    Entity *storeEntity    = RSDK_sceneInfo->entity;
                    RSDK_sceneInfo->entity = (Entity *)ice;
                    if (ice->type == 4 || (ice->type == 18 && ice->subType < 3))
                        ice->type = 0;
                    Ice_Shatter(ice, 0, 0);
                    RSDK_sceneInfo->entity = storeEntity;
                }
            }

            if (entity->type == 4 || (entity->type == 18 && entity->subType < 3))
                entity->type = 0;
            Ice_Shatter(entity, 0, 0);
        }
        Ice_State_Pillar();
    }
}

void Ice_State_PlayerBlock(void)
{
    RSDK_THIS(Ice);

    RSDK.ProcessAnimation(&entity->animator2);
    EntityPlayer *playerPtr = (EntityPlayer *)entity->playerPtr;

    if (playerPtr->state == Ice_State_FrozenPlayer) {
        RSDK.ProcessAnimation(&entity->animator3);
#if RETRO_USE_PLUS
        if (entity->animator2.animationID == entity->animationID + 3) {
            if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, entity->animationID, &entity->animator2, false, 0);
            }

            if (--entity->dwordE4 <= 0) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &entity->animator3, true, 0);
                entity->dwordE4 = 30 * RSDK.Rand(1, 9);
            }
            entity->animator1.frameID = playerPtr->timer;
            if (playerPtr->spindashCharge) {
                entity->dwordDC.x = RSDK.Rand(-1, 2) << 16;
                entity->dwordDC.y = RSDK.Rand(-1, 2) << 16;
            }
            else {
                entity->dwordDC.x = 0;
                entity->dwordDC.y = 0;
            }
            entity->playerPos.x = (playerPtr->position.x & 0xFFFF0000) - (entity->position.x & 0xFFFF0000);
            entity->playerPos.y = (playerPtr->position.y & 0xFFFF0000) - (entity->position.y & 0xFFFF0000);

            entity->position.x = playerPtr->position.x;
            entity->position.y = playerPtr->position.y;
            entity->drawOrder  = playerPtr->drawOrder + 1;
        }
        else {
            if (playerPtr->jumpPress) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, entity->animationID + 3, &entity->animator2, false, 0);
            }
            else {
                if (playerPtr->velocity.x >= -0x60000) {
                    if (playerPtr->velocity.x <= 0x60000) {
                        if (playerPtr->left) {
                            RSDK.SetSpriteAnimation(Ice->aniFrames, entity->animationID + 1, &entity->animator2, false, 0);
                        }
                        else if (playerPtr->right) {
                            RSDK.SetSpriteAnimation(Ice->aniFrames, entity->animationID + 2, &entity->animator2, false, 0);
                        }
                    }
                    else {
                        RSDK.SetSpriteAnimation(Ice->aniFrames, entity->animationID + 2, &entity->animator2, false, 0);
                    }
                }
                else {
                    RSDK.SetSpriteAnimation(Ice->aniFrames, entity->animationID + 1, &entity->animator2, false, 0);
                }
            }

            if (--entity->dwordE4 <= 0) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &entity->animator3, true, 0);
                entity->dwordE4 = 30 * RSDK.Rand(1, 9);
            }
            entity->animator1.frameID = playerPtr->timer;
            if (playerPtr->spindashCharge) {
                entity->dwordDC.x = RSDK.Rand(-1, 2) << 16;
                entity->dwordDC.y = RSDK.Rand(-1, 2) << 16;
            }
            else {
                entity->dwordDC.x = 0;
                entity->dwordDC.y = 0;
            }
            entity->playerPos.x = (playerPtr->position.x & 0xFFFF0000) - (entity->position.x & 0xFFFF0000);
            entity->playerPos.y = (playerPtr->position.y & 0xFFFF0000) - (entity->position.y & 0xFFFF0000);

            entity->position.x = playerPtr->position.x;
            entity->position.y = playerPtr->position.y;
            entity->drawOrder  = playerPtr->drawOrder + 1;
        }
    }
    else {
        if (playerPtr->state != Player_State_FlyIn && playerPtr->state != Player_State_JumpIn) {
            if (!Player_CheckValidState(playerPtr)) {
                Ice_Unknown8((Entity *)entity->playerPtr);
            }
            else {
                playerPtr->outerbox = NULL;
                playerPtr->innerbox = NULL;
                playerPtr->visible  = true;
            }
        }
        destroyEntity(entity);
    }
#else
        if (--entity->dwordE4 <= 0) {
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &entity->animator3, true, 0);
            entity->dwordE4 = 30 * RSDK.Rand(1, 9);
        }
        entity->animator1.frameID = playerPtr->timer;
        if (playerPtr->spindashCharge) {
            entity->dwordDC.x = RSDK.Rand(-1, 2) << 16;
            entity->dwordDC.y = RSDK.Rand(-1, 2) << 16;
        }
        else {
            entity->dwordDC.x = 0;
            entity->dwordDC.y = 0;
        }
        entity->playerPos.x = (playerPtr->position.x & 0xFFFF0000) - (entity->position.x & 0xFFFF0000);
        entity->playerPos.y = (playerPtr->position.y & 0xFFFF0000) - (entity->position.y & 0xFFFF0000);

        entity->position.x = playerPtr->position.x;
        entity->position.y = playerPtr->position.y;
        entity->drawOrder  = playerPtr->drawOrder + 1;
    }
    else {
        playerPtr->outerbox = NULL;
        playerPtr->innerbox = NULL;
        playerPtr->visible  = true;
        destroyEntity(entity);
    }
#endif

    foreach_active(Player, player)
    {
        if (player != (EntityPlayer *)entity->playerPtr && player->stateInput != Player_ProcessP2Input_AI && player->state != Ice_State_FrozenPlayer
            && player->velocity.y > 0 && !player->onGround && player->position.y < entity->position.y - 0x200000
            && Player_CheckBadnikHit(player, entity, &entity->hitbox1)) {
            if (player->playerAnimator.animationID == ANI_JUMP || player->playerAnimator.animationID == ANI_DROPDASH) {
                Ice_Unknown8((Entity *)entity->playerPtr);
                player->velocity.y = -0x30000;
                player->onGround   = false;
                foreach_break;
            }
        }
    }

    if (entity->timer)
        entity->timer--;
}

void Ice_State_Shard(void)
{
    RSDK_THIS(Ice);
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->visible ^= 1;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

void Ice_StateDraw_Unknown1(void)
{
    RSDK_THIS(Ice);
    Vector2 drawPos;

    int32 dirStore = entity->direction;
    drawPos.x    = entity->position.x;
    drawPos.y    = entity->position.y;
    drawPos.x += entity->dwordDC.x;
    drawPos.y += entity->dwordDC.y;
    entity->direction = entity->subFlip;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->direction = dirStore;
    drawPos.y -= 0x30000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

#if RETRO_USE_PLUS
    if (!RSDK_sceneInfo->inEditor) {
        entity->inkEffect = INK_ADD;
        if (!entity->dwordE4)
            entity->animator1.frameID += 4;
        else
            entity->animator1.frameID += 2;
        RSDK.DrawSprite(&entity->animator1, 0, false);
        if (entity->dwordE4)
            entity->animator1.frameID -= 2;
        else
            entity->animator1.frameID -= 4;
        entity->inkEffect = INK_NONE;
    }
#endif
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->inkEffect = INK_NONE;
}

void Ice_StateDraw_PlayerBlock(void)
{
    RSDK_THIS(Ice);
    Vector2 drawPos;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->dwordDC.x;
    drawPos.y += entity->dwordDC.y;

#if RETRO_USE_PLUS
    int32 frame                 = entity->animator1.frameID;
    entity->inkEffect         = INK_SUB;
    entity->animator1.frameID = 5;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = frame;
    entity->inkEffect         = INK_NONE;
#endif
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    entity->inkEffect = INK_NONE;
}

void Ice_StateDraw_Pillar(void)
{
    RSDK_THIS(Ice);
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->inkEffect = INK_ADD;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->inkEffect = INK_NONE;
}

void Ice_StateDraw_Shard(void)
{
    RSDK_THIS(Ice);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Ice_EditorDraw(void) {}

void Ice_EditorLoad(void) {}

void Ice_Serialize(void)
{
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, size);
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, subType);
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, subFlip);
    RSDK_EDITABLE_VAR(Ice, VAR_BOOL, bottomSmash);
    RSDK_EDITABLE_VAR(Ice, VAR_BOOL, knuxSmash);
}
