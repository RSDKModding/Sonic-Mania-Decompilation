#include "SonicMania.h"

ObjectIce *Ice;

void Ice_Update(void)
{
    RSDK_THIS(Ice);
    StateMachine_Run(self->state);
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
    StateMachine_Run(self->stateDraw);
}

void Ice_Create(void *data)
{
    RSDK_THIS(Ice);
    if (!SceneInfo->inEditor) {
        self->drawFX    = FX_FLIP;
        self->drawOrder = Zone->playerDrawLow + 1;
        self->visible   = true;

        if (data) {
            self->active = ACTIVE_NORMAL;

            switch (voidToInt(data)) {
                case 1:
                    self->hitbox1.left   = -24;
                    self->hitbox1.top    = -24;
                    self->hitbox1.right  = 24;
                    self->hitbox1.bottom = 24;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERBLOCK, &self->animator1, true, 0);
                    self->isPermanent = true;
                    self->state       = Ice_State_PlayerBlock;
                    self->stateDraw   = Ice_StateDraw_PlayerBlock;
                    break;
                case 2:
                    self->updateRange.x  = 0x800000;
                    self->updateRange.y  = 0x800000;
                    self->hitbox1.left   = -19;
                    self->hitbox1.top    = -110;
                    self->hitbox1.right  = 19;
                    self->hitbox1.bottom = 0;
                    self->hitbox3.left   = -19;
                    self->hitbox3.top    = -110;
                    self->hitbox3.right  = 19;
                    self->hitbox3.bottom = 0;
                    self->dwordE4        = 240;
                    self->alpha          = 128;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARBLOCK, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARGLINT, &self->animator4, true, 0);
                    self->state     = Ice_State_Pillar;
                    self->stateDraw = Ice_StateDraw_Pillar;
                    break;
                case 3:
                    self->updateRange.x = 0x400000;
                    self->updateRange.y = 0x400000;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_SHARD, &self->animator1, true, 0);
                    self->state     = Ice_State_Shard;
                    self->stateDraw = Ice_StateDraw_Shard;
                    break;
            }
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_ICEBLOCK, &self->animator1, true, self->size);
            switch (self->type) {
                case 0: RSDK.SetSpriteAnimation(0xFFFF, ICEANI_ICEBLOCK, &self->animator2, true, 0); break;
                case 1: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->animator2, true, 0); break;
                case 2: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->animator2, true, 1); break;
                case 3: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->animator2, true, 2); break;
                case 4:
                    self->subType = self->subType & 3;
                    RSDK.SetSpriteAnimation(Spikes->aniFrames, self->subType >> 1, &self->animator2, true, 0);
                    if (!self->size) {
                        switch (self->subType) {
                            case 0:
                                self->dwordDC.y = 0x40000;
                                self->subFlip   = 0;
                                break;
                            case 1:
                                self->dwordDC.y = -0x40000;
                                self->subFlip   = 2;
                                break;
                            case 2:
                                self->dwordDC.x = -0x80000;
                                self->subFlip   = 0;
                                break;
                            case 3:
                                self->dwordDC.x = 0x80000;
                                self->subFlip   = 1;
                                break;
                            default: break;
                        }
                    }
                    break;
                case 18:
                    if (self->subType >= 3)
                        RSDK.SetSpriteAnimation(Spring->aniFrames, (self->subType - 3), &self->animator2, true, 0);
                    else
                       RSDK.SetSpriteAnimation(IceSpring->animID, self->subType, &self->animator2, true, 0);
                    if (self->size) {
                        switch (self->subType) {
                            case 0:
                            case 3:
                            case 4:
                                self->dwordDC.x = 0;
                                if (!(self->subFlip & 2))
                                    self->dwordDC.y = 0x80000;
                                else
                                    self->dwordDC.y = -0x80000;
                                break;
                            case 1:
                            case 5:
                            case 6:
                                self->dwordDC.y = 0;
                                if (!(self->subFlip & 1))
                                    self->dwordDC.x = -0x80000;
                                else
                                    self->dwordDC.x = 0x80000;
                                break;
                            case 2:
                            case 7:
                            case 8:
                                self->dwordDC.x = 0;
                                if (!(self->subFlip & 2))
                                    self->dwordDC.y = -0x10000;
                                else
                                    self->dwordDC.y = 0x10000;
                                break;
                            default: break;
                        }
                    }
                    else {
                        switch (self->subType) {
                            case 0:
                            case 3:
                            case 4:
                                self->dwordDC.x = 0;
                                if (!(self->subFlip & 2))
                                    self->dwordDC.y = 0xC0000;
                                else
                                    self->dwordDC.y = -0xC0000;
                                break;
                            case 1:
                            case 5:
                            case 6:
                                self->dwordDC.y = 0;
                                if (!(self->subFlip & 1))
                                    self->dwordDC.x = -0x100000;
                                else
                                    self->dwordDC.x = 0x100000;
                                break;
                            case 2:
                            case 7:
                            case 8:
                                if (!(self->subFlip & 1))
                                    self->dwordDC.x = -0x80000;
                                else
                                    self->dwordDC.x = 0x80000;
                                if (!(self->subFlip & 2))
                                    self->dwordDC.y = 0x40000;
                                else
                                    self->dwordDC.y = -0x40000;
                                break;
                            default: break;
                        }
                    }
                    break;
                default:
                    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &self->animator2, true, 0);
                    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &self->animator2, true, 0);
                    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &self->animator3, true, (self->type > 12 ? 2 : 0) + self->type - 5);
                    if (self->type != 12) {
                        if (globals->gameMode == MODE_COMPETITION) {
                            if (globals->itemMode == 1) {
                                self->type              = 16;
                                self->animator3.frameID = 13;
                            }
                            else if (globals->itemMode == 2) {
                                self->type              = 15;
                                self->animator3.frameID = 12;
                            }
                        }
                    }
                    else if (globals->gameMode != MODE_TIMEATTACK) {
                        switch (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->characterID) {
                            case ID_TAILS: ++self->animator3.frameID; break;
                            case ID_KNUCKLES: self->animator3.frameID += 2; break;
#if RETRO_USE_PLUS
                            case ID_MIGHTY: self->animator3.frameID += 8; break;
                            case ID_RAY: self->animator3.frameID += 9; break;
#endif
                            default: break;
                        }

                        if (globals->gameMode == MODE_COMPETITION) {
                            if (globals->itemMode == 1) {
                                self->type              = 16;
                                self->animator3.frameID = 13;
                            }
                            else if (globals->itemMode == 2) {
                                self->type              = 15;
                                self->animator3.frameID = 12;
                            }
                        }
                    }
                    else {
                        self->type              = 5;
                        self->animator3.frameID = 0;
                    }
                    break;
            }

            if (self->size) {
                if (self->size == 1) {
                    self->hitbox1.left   = -16;
                    self->hitbox1.top    = -16;
                    self->hitbox1.right  = 16;
                    self->hitbox1.bottom = 16;
                }
            }
            else {
                self->hitbox1.left   = -24;
                self->hitbox1.top    = -20;
                self->hitbox1.right  = 24;
                self->hitbox1.bottom = 20;
            }

            self->hitbox2.top    = self->hitbox1.top;
            self->hitbox3.left   = self->hitbox1.left;
            self->hitbox3.right  = self->hitbox1.right;
            self->hitbox3.bottom = self->hitbox1.bottom + 8;
            self->hitbox3.top    = self->hitbox1.top - 8;
            self->hitbox2.left   = self->hitbox1.left - 8;
            self->hitbox2.right  = self->hitbox1.right + 8;
            self->hitbox2.bottom = self->hitbox1.bottom + 8;
            self->alpha          = 384;
            self->state          = Ice_State_Pillar;
            self->stateDraw      = Ice_StateDraw_Unknown1;
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
    Zone->timeOverCallback   = Ice_TimeOverCB;
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
    if (!Zone->gotTimeOver && player->state != Ice_State_FrozenPlayer && (player->shield != SHIELD_FIRE || player->invincibleTimer > 0)) {
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
        player->animator.animationSpeed = 0;
        player->visible                       = false;
        player->nextAirState                  = StateMachine_None;
        player->nextGroundState               = StateMachine_None;
        player->state                         = Ice_State_FrozenPlayer;
        player->velocity.y                    = 0;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        player->abilityPtrs[0] = self;
        player->abilityPtrs[1] = ice;
        if (!self->collisionMode) {
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

    if (abs(self->skidding) >= 0x50000) {
        if (abs(self->pushing) <= abs(self->skidding)) {
            if (self->skidding < 0)
                self->velocity.x = 0x18000;
            else
                self->velocity.x = -0x18000;

            Ice_Unknown8((Entity *)self);
            return true;
        }
    }
    return false;
}

bool32 Ice_Unknown4(void)
{
    RSDK_THIS(Player);

    if (abs(self->pushing) >= 0x80000) {
        if (abs(self->skidding) <= abs(self->pushing)) {
            if (self->pushing > 0) {
                if ((self->angle & 0xFF) < 0x20) {
                    Ice_Unknown8(SceneInfo->entity);
                    return true;
                }
            }
            if (self->pushing && (self->angle + 0x80) < 0x20) {
                Ice_Unknown8(SceneInfo->entity);
                return true;
            }
        }
    }
    return false;
}

void Ice_State_FrozenPlayer(void)
{
    RSDK_THIS(Player);

    bool32 flag = self->onGround;
    int32 skid    = self->groundVel;
    bool32 groundFlag    = self->groundedStore;

    if (!flag) {
        flag |= self->groundedStore || self->velocity.x;
        skid = self->velocity.x;
        groundFlag = self->velocity.y;
    }
    if (!flag) {
        flag |= abs(self->skidding) < 0x50000 || !Ice_Unknown3();
        skid       = self->velocity.x;
        groundFlag = self->velocity.y;
    }

    if (flag) {
        if (self->onGround) {
            if (!self->groundVel) {
                if (Ice_Unknown3())
                    return;
            }
        }

        self->skidding = skid;
        if (!groundFlag) {
            if (Ice_Unknown4())
                return;
        }

        self->pushing = self->velocity.y;
        if (self->shield == SHIELD_FIRE && self->invincibleTimer <= 0) {
            self->state      = Player_State_Air;
            self->velocity.y = -self->jumpStrength >> 1;
            self->onGround   = false;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, true, 0);
            Ice_ShatterGenerator(24, 20, 64, 0, 0, 2);
            RSDK.PlaySfx(Ice->sfxWindowShatter, 0, 255);
            Ice->playerTimers[RSDK.GetEntityID(self)] = 30;
            self->skidding                            = 0;
            self->pushing                             = 0;
            self->timer                               = 0;
            self->abilityTimer                        = 0;
            self->spindashCharge                      = 0;
        }
        else {
            if (!self->invincibleTimer)
                self->invincibleTimer = -1;

            if (self->spindashCharge > 0) {
                self->spindashCharge--;
            }
            else if (self->jumpPress) {
                self->spindashCharge = 15;
                if (++self->timer > 4) {
                    self->state      = Player_State_Air;
                    self->velocity.y = -self->jumpStrength >> 1;
                    self->onGround   = false;
                    RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, true, 0);
                    Ice_ShatterGenerator(24, 20, 64, 0, 0, 2);
                    RSDK.PlaySfx(Ice->sfxWindowShatter, 0, 255);
                    Ice->playerTimers[RSDK.GetEntityID(self)] = 30;
                    self->skidding                            = 0;
                    self->pushing                             = 0;
                    self->timer                               = 0;
                    self->abilityTimer                        = 0;
                    self->spindashCharge                      = 0;
                }
                else {
                    Ice_ShatterGenerator(24, 20, 8, 0, 0, 0);
                    RSDK.PlaySfx(Ice->sfxStruggle, 0, 255);
                }
            }
        }

        if (self->state == Ice_State_FrozenPlayer) {
            if (self->onGround) {
                int32 rollFric            = self->rollingFriction;
                self->left            = false;
                self->right           = false;
                self->rollingFriction = 0;
                Player_HandleRollDeceleration();
                if (!self->groundVel)
                    self->groundVel += (5000 * RSDK.Sin256(self->angle)) >> 8;
                if (self->camera)
                    self->camera->offsetYFlag = false;
                self->jumpAbilityTimer = 0;
                self->rollingFriction  = rollFric;
                if (self->state == Player_State_Ground)
                    self->state = Ice_State_FrozenPlayer;
            }
            else {
                self->velocity.y += 0x3800;
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
        EntityIce *ice                = CREATE_ENTITY(Ice, intToVoid(3), randX + self->position.x, randY + self->position.y);
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
    if (self->type == 4 || (self->type == 18 && self->subType < 3))
        self->type = 0;
    EntityItemBox *itemBox = (EntityItemBox *)Ice_Shatter(self, velX, velY);
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
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, true, 0);
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
    if (self->animator1.animationID == ICEANI_PILLARBLOCK) {
        self->position.y -= 0x370000;
        Ice_ShatterGenerator(19, 55, 96, velX, velY, 2);
    }
    else if (self->size) {
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
                ring->stateDraw               = Ring_Draw_Normal;
                angle += 16;
            }
            break;
        }
        case 4: CREATE_ENTITY(Spikes, intToVoid(self->subType), ice->position.x + self->dwordDC.x, ice->position.y + self->dwordDC.y); break;
        case 18:
            if (self->subType >= 3)
                CREATE_ENTITY(Spring, intToVoid((self->subFlip << 8) - 3 + self->subType), ice->position.x + self->dwordDC.x,
                              ice->position.y + self->dwordDC.y);
            else
                CREATE_ENTITY(IceSpring, intToVoid(self->subType + (self->subFlip << 8)), ice->position.x + self->dwordDC.x,
                              ice->position.y + self->dwordDC.y);
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
        if (ice != self && ice->state == Ice_State_Pillar && RSDK.CheckObjectCollisionTouchBox(self, &self->hitbox1, ice, &ice->hitbox2)) {
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
            int32 side = RSDK.CheckObjectCollisionBox(self, &self->hitbox3, player, &Ice->hitbox2, false);
            switch (side) {
                case 1:
                    if (player->velocity.y < 0x40000) {
                        player->position.x = playerX;
                        player->position.y = playerY;
                        self->position.x -= self->playerPos.x;
                        self->position.y -= self->playerPos.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitbox1) == 1) {
                            player->position.x += self->playerPos.x;
                            player->position.y += self->playerPos.y;
                        }
                        self->position.x += self->playerPos.x;
                        self->position.y += self->playerPos.y;
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
                        self->position.x -= self->playerPos.x;
                        self->position.y -= self->playerPos.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitbox1) == 1) {
                            player->position.x += self->playerPos.x;
                            player->position.y += self->playerPos.y;
                        }
                        self->position.x += self->playerPos.x;
                        self->position.y += self->playerPos.y;
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
                        self->position.x -= self->playerPos.x;
                        self->position.y -= self->playerPos.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitbox1) == 1) {
                            player->position.x += self->playerPos.x;
                            player->position.y += self->playerPos.y;
                        }
                        self->position.x += self->playerPos.x;
                        self->position.y += self->playerPos.y;
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
                        self->position.x -= self->playerPos.x;
                        self->position.y -= self->playerPos.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitbox1) == 1) {
                            player->position.x += self->playerPos.x;
                            player->position.y += self->playerPos.y;
                        }
                        self->position.x += self->playerPos.x;
                        self->position.y += self->playerPos.y;
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
                    self->position.x -= self->playerPos.x;
                    self->position.y -= self->playerPos.y;
                    if (Player_CheckCollisionBox(player, self, &self->hitbox1) == 1) {
                        player->position.x += self->playerPos.x;
                        player->position.y += self->playerPos.y;
                    }
                    self->position.x += self->playerPos.x;
                    self->position.y += self->playerPos.y;
                    flag = false;
                    break;
            }
        }

        if (flag) {
            int32 side = RSDK.CheckObjectCollisionBox(self, &self->hitbox1, player, &Ice->hitbox2, false);
            if (side >= 2) {
                if (side <= 3) {
                    if (self->knuxSmash && player->characterID == ID_KNUCKLES) {
                        Ice_Shatter(self, 0, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    else if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0) {
                        if (RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield)->animator.animationID == 2) {
                            if (player->position.x >= self->position.x) {
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
                else if (side == 4 && self->bottomSmash) {
                    Ice_Shatter(self, 0, 0);
                    player->position.x = playerX;
                    player->position.y = playerY;
                    foreach_return;
                }
            }

            player->position.x = playerX;
            player->position.y = playerY;
            self->position.x -= self->playerPos.x;
            self->position.y -= self->playerPos.y;
            int32 prevVel = player->velocity.y;
            side        = Player_CheckCollisionBox(player, self, &self->hitbox1);
            if (side) {
                if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !self->dwordE4) {
                    if (self->animator1.animationID == ICEANI_PILLARBLOCK) {
                        self->position.y -= 0x370000;
                        Ice_ShatterGenerator(19, 55, 36, 0, 0, 0);
                    }
                    else if (self->size) {
                        Ice_ShatterGenerator(19, 17, 6, 0, 0, 0);
                    }
                    else {
                        Ice_ShatterGenerator(19, 20, 12, 0, 0, 0);
                    }

                    self->animator1.frameID += 2;
                    self->dwordE4 = 15;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_LARGEGLINT + self->size, &self->animator4, true, 0);
                }

                if (side == 4) {
                    if (player->onGround && !player->collisionMode && self->playerPos.y >= 0) {
                        if (player->shield == SHIELD_FIRE || player->invincibleTimer) {
                            player->position.x = playerX;
                            player->position.y = playerY;
                            Ice_Shatter(self, 0, 0);
                            foreach_return;
                        }
                        player->hurtFlag = 1;
                    }
                }
                else if (side == 1 && player->velocity.y >= 0) {
                    if (self->animator1.animationID == ICEANI_PILLARBLOCK) {
                        Player_CheckHit(player, self);
                    }
                    else {
                        if ((player->animator.animationID == ANI_JUMP || player->state == Player_State_DropDash) && !player->sidekick) {
                            Ice_Shatter(self, 0, 0);
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
                    player->position.x += self->playerPos.x;
                    player->position.y += self->playerPos.y;
                }
            }
            self->position.x += self->playerPos.x;
            self->position.y += self->playerPos.y;
        }
    }

    RSDK.ProcessAnimation(&self->animator4);
    if (self->animator1.animationID == ICEANI_PILLARBLOCK) {
        if (!RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->hitbox1.bottom << 16, true)
            && self->state == Ice_State_Pillar) {
            self->state = Ice_Unknown13;
        }
        RSDK.ProcessAnimation(&self->animator1);
        self->dwordE4--;
        if (self->dwordE4 == 54) {
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARGLINT, &self->animator4, true, 1);
        }
        else if (!self->dwordE4)
            Ice_Shatter(self, 0, 0);
    }
    else {
        if (self->dwordE4 > 0) {
            self->dwordE4--;
            if (!self->dwordE4) {
                if (self->type == 4 || (self->type == 18 && self->subType < 3))
                    self->type = 0;
                Ice_Shatter(self, 0, 0);
            }
        }
    }
}

void Ice_Unknown13(void)
{
    RSDK_THIS(Ice);
    Ice_Unknown11();
    self->timer  = 15;
    self->state  = Ice_Unknown14;
    self->active = ACTIVE_NORMAL;
    Ice_State_Pillar();
}

void Ice_Unknown14(void)
{
    RSDK_THIS(Ice);
    if (!--self->timer) {
        Ice_Unknown11();
        self->state = Ice_Unknown15;
    }
    Ice_Unknown15();
    self->velocity.y = 0;
}

void Ice_Unknown15(void)
{
    RSDK_THIS(Ice);

    self->playerPos.y = -(int32)(self->position.y & 0xFFFF0000);
    self->position.y  = self->velocity.y + self->position.y;
    self->playerPos.y += (self->velocity.y + self->position.y) & 0xFFFF0000;
    self->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->hitbox1.bottom << 16, true)) {
        self->velocity.y = 0;
        self->state      = Ice_State_Pillar;
        Ice_State_Pillar();
    }
    else {
        int32 velY = self->velocity.y;
        foreach_all(Ice, ice)
        {
            if (ice != self) {
                if ((ice->stateDraw == Ice_StateDraw_Unknown1 || ice->stateDraw == Ice_StateDraw_PlayerBlock)
                    && RSDK.CheckObjectCollisionPlatform(ice, &ice->hitbox1, self, &self->hitbox1, true)) {
                    self->velocity.y = 0;
                    if (ice->state == Ice_State_Pillar) {
                        self->active = ACTIVE_BOUNDS;
                        self->state  = Ice_State_Pillar;
                    }
                }
            }
        }

        foreach_all(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionPlatform(spikes, &spikes->hitbox, self, &self->hitbox1, true)) {
                self->velocity.y = 0;
                self->active     = ACTIVE_BOUNDS;
                self->state      = Ice_State_Pillar;
            }
        }

        foreach_all(ItemBox, itemBox)
        {
            if ((itemBox->state == ItemBox_State_Normal || itemBox->state == ItemBox_State_Falling)
                && RSDK.CheckObjectCollisionPlatform(itemBox, &ItemBox->hitbox, self, &self->hitbox1, true)) {
                self->velocity.y = 0;
                if (itemBox->onGround) {
                    self->active = ACTIVE_BOUNDS;
                    self->state  = Ice_State_Pillar;
                }
            }
        }

        if (!self->velocity.y && velY >= 0x60000) {
            if (ice) {
                if (ice->stateDraw == Ice_StateDraw_PlayerBlock) {
                    Ice_Unknown8((Entity *)ice->playerPtr);
                }
                else {
                    Entity *storeEntity    = SceneInfo->entity;
                    SceneInfo->entity = (Entity *)ice;
                    if (ice->type == 4 || (ice->type == 18 && ice->subType < 3))
                        ice->type = 0;
                    Ice_Shatter(ice, 0, 0);
                    SceneInfo->entity = storeEntity;
                }
            }

            if (self->type == 4 || (self->type == 18 && self->subType < 3))
                self->type = 0;
            Ice_Shatter(self, 0, 0);
        }
        Ice_State_Pillar();
    }
}

void Ice_State_PlayerBlock(void)
{
    RSDK_THIS(Ice);

    RSDK.ProcessAnimation(&self->animator2);
    EntityPlayer *playerPtr = (EntityPlayer *)self->playerPtr;

    if (playerPtr->state == Ice_State_FrozenPlayer) {
        RSDK.ProcessAnimation(&self->animator3);
#if RETRO_USE_PLUS
        if (self->animator2.animationID == self->animationID + 3) {
            if (self->animator2.frameID == self->animator2.frameCount - 1) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID, &self->animator2, false, 0);
            }

            if (--self->dwordE4 <= 0) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &self->animator3, true, 0);
                self->dwordE4 = 30 * RSDK.Rand(1, 9);
            }
            self->animator1.frameID = playerPtr->timer;
            if (playerPtr->spindashCharge) {
                self->dwordDC.x = RSDK.Rand(-1, 2) << 16;
                self->dwordDC.y = RSDK.Rand(-1, 2) << 16;
            }
            else {
                self->dwordDC.x = 0;
                self->dwordDC.y = 0;
            }
            self->playerPos.x = (playerPtr->position.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
            self->playerPos.y = (playerPtr->position.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

            self->position.x = playerPtr->position.x;
            self->position.y = playerPtr->position.y;
            self->drawOrder  = playerPtr->drawOrder + 1;
        }
        else {
            if (playerPtr->jumpPress) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 3, &self->animator2, false, 0);
            }
            else {
                if (playerPtr->velocity.x >= -0x60000) {
                    if (playerPtr->velocity.x <= 0x60000) {
                        if (playerPtr->left) {
                            RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 1, &self->animator2, false, 0);
                        }
                        else if (playerPtr->right) {
                            RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 2, &self->animator2, false, 0);
                        }
                    }
                    else {
                        RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 2, &self->animator2, false, 0);
                    }
                }
                else {
                    RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 1, &self->animator2, false, 0);
                }
            }

            if (--self->dwordE4 <= 0) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &self->animator3, true, 0);
                self->dwordE4 = 30 * RSDK.Rand(1, 9);
            }
            self->animator1.frameID = playerPtr->timer;
            if (playerPtr->spindashCharge) {
                self->dwordDC.x = RSDK.Rand(-1, 2) << 16;
                self->dwordDC.y = RSDK.Rand(-1, 2) << 16;
            }
            else {
                self->dwordDC.x = 0;
                self->dwordDC.y = 0;
            }
            self->playerPos.x = (playerPtr->position.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
            self->playerPos.y = (playerPtr->position.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

            self->position.x = playerPtr->position.x;
            self->position.y = playerPtr->position.y;
            self->drawOrder  = playerPtr->drawOrder + 1;
        }
    }
    else {
        if (playerPtr->state != Player_State_FlyIn && playerPtr->state != Player_State_JumpIn) {
            if (!Player_CheckValidState(playerPtr)) {
                Ice_Unknown8((Entity *)self->playerPtr);
            }
            else {
                playerPtr->outerbox = NULL;
                playerPtr->innerbox = NULL;
                playerPtr->visible  = true;
            }
        }
        destroyEntity(self);
    }
#else
        if (--self->dwordE4 <= 0) {
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &self->animator3, true, 0);
            self->dwordE4 = 30 * RSDK.Rand(1, 9);
        }
        self->animator1.frameID = playerPtr->timer;
        if (playerPtr->spindashCharge) {
            self->dwordDC.x = RSDK.Rand(-1, 2) << 16;
            self->dwordDC.y = RSDK.Rand(-1, 2) << 16;
        }
        else {
            self->dwordDC.x = 0;
            self->dwordDC.y = 0;
        }
        self->playerPos.x = (playerPtr->position.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
        self->playerPos.y = (playerPtr->position.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

        self->position.x = playerPtr->position.x;
        self->position.y = playerPtr->position.y;
        self->drawOrder  = playerPtr->drawOrder + 1;
    }
    else {
        playerPtr->outerbox = NULL;
        playerPtr->innerbox = NULL;
        playerPtr->visible  = true;
        destroyEntity(self);
    }
#endif

    foreach_active(Player, player)
    {
        if (player != (EntityPlayer *)self->playerPtr && player->stateInput != Player_ProcessP2Input_AI && player->state != Ice_State_FrozenPlayer
            && player->velocity.y > 0 && !player->onGround && player->position.y < self->position.y - 0x200000
            && Player_CheckBadnikTouch(player, self, &self->hitbox1)) {
            if (player->animator.animationID == ANI_JUMP || player->animator.animationID == ANI_DROPDASH) {
                Ice_Unknown8((Entity *)self->playerPtr);
                player->velocity.y = -0x30000;
                player->onGround   = false;
                foreach_break;
            }
        }
    }

    if (self->timer)
        self->timer--;
}

void Ice_State_Shard(void)
{
    RSDK_THIS(Ice);
    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->visible ^= 1;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Ice_StateDraw_Unknown1(void)
{
    RSDK_THIS(Ice);
    Vector2 drawPos;

    int32 dirStore = self->direction;
    drawPos.x    = self->position.x;
    drawPos.y    = self->position.y;
    drawPos.x += self->dwordDC.x;
    drawPos.y += self->dwordDC.y;
    self->direction = self->subFlip;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->direction = dirStore;
    drawPos.y -= 0x30000;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

#if RETRO_USE_PLUS
    if (!SceneInfo->inEditor) {
        self->inkEffect = INK_ADD;
        if (!self->dwordE4)
            self->animator1.frameID += 4;
        else
            self->animator1.frameID += 2;
        RSDK.DrawSprite(&self->animator1, 0, false);
        if (self->dwordE4)
            self->animator1.frameID -= 2;
        else
            self->animator1.frameID -= 4;
        self->inkEffect = INK_NONE;
    }
#endif
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->inkEffect = INK_NONE;
}

void Ice_StateDraw_PlayerBlock(void)
{
    RSDK_THIS(Ice);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->dwordDC.x;
    drawPos.y += self->dwordDC.y;

#if RETRO_USE_PLUS
    int32 frame                 = self->animator1.frameID;
    self->inkEffect         = INK_SUB;
    self->animator1.frameID = 5;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = frame;
    self->inkEffect         = INK_NONE;
#endif
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator3, NULL, false);

    self->inkEffect = INK_NONE;
}

void Ice_StateDraw_Pillar(void)
{
    RSDK_THIS(Ice);
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator4, NULL, false);

    self->inkEffect = INK_NONE;
}

void Ice_StateDraw_Shard(void)
{
    RSDK_THIS(Ice);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

#if RETRO_INCLUDE_EDITOR
void Ice_EditorDraw(void) {}

void Ice_EditorLoad(void) {}
#endif

void Ice_Serialize(void)
{
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, size);
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, subType);
    RSDK_EDITABLE_VAR(Ice, VAR_UINT8, subFlip);
    RSDK_EDITABLE_VAR(Ice, VAR_BOOL, bottomSmash);
    RSDK_EDITABLE_VAR(Ice, VAR_BOOL, knuxSmash);
}
