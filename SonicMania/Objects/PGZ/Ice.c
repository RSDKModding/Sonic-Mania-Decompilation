// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Ice Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
        self->drawGroup = Zone->playerDrawGroup[0] + 1;
        self->visible   = true;

        if (data) {
            self->active = ACTIVE_NORMAL;

            switch (VOID_TO_INT(data)) {
                case ICE_CHILD_PLAYER:
                    self->hitboxBlock.left   = -24;
                    self->hitboxBlock.top    = -24;
                    self->hitboxBlock.right  = 24;
                    self->hitboxBlock.bottom = 24;

                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERBLOCK, &self->blockAnimator, true, 0);

                    self->isPermanent = true;
                    self->state       = Ice_State_PlayerBlock;
                    self->stateDraw   = Ice_Draw_PlayerBlock;
                    break;

                case ICE_CHILD_PILLAR:
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;

                    self->hitboxBlock.left   = -19;
                    self->hitboxBlock.top    = -110;
                    self->hitboxBlock.right  = 19;
                    self->hitboxBlock.bottom = 0;

                    self->hitboxPlayerBlockCheck.left   = -19;
                    self->hitboxPlayerBlockCheck.top    = -110;
                    self->hitboxPlayerBlockCheck.right  = 19;
                    self->hitboxPlayerBlockCheck.bottom = 0;

                    self->glintTimer = 240;
                    self->alpha      = 0x80;

                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARBLOCK, &self->blockAnimator, true, 0);
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARGLINT, &self->glintAnimator, true, 0);

                    self->state     = Ice_State_IceBlock;
                    self->stateDraw = Ice_Draw_Pillar;
                    break;
                case ICE_CHILD_SHARD:
                    self->updateRange.x = 0x400000;
                    self->updateRange.y = 0x400000;

                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_SHARD, &self->blockAnimator, true, 0);

                    self->state     = Ice_State_Shard;
                    self->stateDraw = Ice_Draw_Shard;
                    break;
            }
        }
        else {
            self->active        = ACTIVE_BOUNDS;
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;

            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_ICEBLOCK, &self->blockAnimator, true, self->size);
            switch (self->type) {
                case ICE_BLOCK: RSDK.SetSpriteAnimation(-1, ICEANI_ICEBLOCK, &self->contentsAnimator, true, 0); break;
                case ICE_1RING: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->contentsAnimator, true, 0); break;
                case ICE_3RINGS: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->contentsAnimator, true, 1); break;
                case ICE_5RINGS: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->contentsAnimator, true, 2); break;

                case ICE_SPIKES:
                    self->subType = self->subType & 3;
                    RSDK.SetSpriteAnimation(Spikes->aniFrames, self->subType >> 1, &self->contentsAnimator, true, 0);
                    if (!self->size) {
                        // direction of the spikes, matches the spikes "type" variable
                        switch (self->subType) {
                            case 0:
                                self->contentsOffset.y = 0x40000;
                                self->subFlip          = FLIP_NONE;
                                break;

                            case 1:
                                self->contentsOffset.y = -0x40000;
                                self->subFlip          = FLIP_Y;
                                break;

                            case 2:
                                self->contentsOffset.x = -0x80000;
                                self->subFlip          = FLIP_NONE;
                                break;

                            case 3:
                                self->contentsOffset.x = 0x80000;
                                self->subFlip          = FLIP_X;
                                break;

                            default: break;
                        }
                    }
                    break;

                case ICE_SPRING:
                    // Ice Springs (0-2), Normal Springs (2-8)
                    if (self->subType >= 3)
                        RSDK.SetSpriteAnimation(Spring->aniFrames, (self->subType - 3), &self->contentsAnimator, true, 0);
                    else
                        RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->subType, &self->contentsAnimator, true, 0);

                    if (self->size) {
                        switch (self->subType) {
                            case 0:
                            case 3:
                            case 4:
                                self->contentsOffset.x = 0;
                                if (!(self->subFlip & FLIP_Y))
                                    self->contentsOffset.y = 0x80000;
                                else
                                    self->contentsOffset.y = -0x80000;
                                break;

                            case 1:
                            case 5:
                            case 6:
                                self->contentsOffset.y = 0;
                                if (!(self->subFlip & FLIP_X))
                                    self->contentsOffset.x = -0x80000;
                                else
                                    self->contentsOffset.x = 0x80000;
                                break;

                            case 2:
                            case 7:
                            case 8:
                                self->contentsOffset.x = 0;
                                if (!(self->subFlip & FLIP_Y))
                                    self->contentsOffset.y = -0x10000;
                                else
                                    self->contentsOffset.y = 0x10000;
                                break;

                            default: break;
                        }
                    }
                    else {
                        switch (self->subType) {
                            case 0:
                            case 3:
                            case 4:
                                self->contentsOffset.x = 0;
                                if (!(self->subFlip & FLIP_Y))
                                    self->contentsOffset.y = 0xC0000;
                                else
                                    self->contentsOffset.y = -0xC0000;
                                break;

                            case 1:
                            case 5:
                            case 6:
                                self->contentsOffset.y = 0;
                                if (!(self->subFlip & FLIP_X))
                                    self->contentsOffset.x = -0x100000;
                                else
                                    self->contentsOffset.x = 0x100000;
                                break;

                            case 2:
                            case 7:
                            case 8:
                                if (!(self->subFlip & FLIP_X))
                                    self->contentsOffset.x = -0x80000;
                                else
                                    self->contentsOffset.x = 0x80000;
                                if (!(self->subFlip & FLIP_Y))
                                    self->contentsOffset.y = 0x40000;
                                else
                                    self->contentsOffset.y = -0x40000;
                                break;

                            default: break;
                        }
                    }
                    break;

                case ICE_ITEMBOX_RINGS:
                case ICE_ITEMBOX_BLUESHIELD:
                case ICE_ITEMBOX_BUBBLESHIELD:
                case ICE_ITEMBOX_FIRESHIELD:
                case ICE_ITEMBOX_LIGHTNINGSHIELD:
                case ICE_ITEMBOX_INVINCIBILITY:
                case ICE_ITEMBOX_SNEAKERS:
                case ICE_ITEMBOX_1UP:
                case ICE_ITEMBOX_EGGMAN:
                case ICE_ITEMBOX_HYPERRING:
                case ICE_ITEMBOX_SWAP:
                case ICE_ITEMBOX_RANDOM:
                case ICE_ITEMBOX_SUPER:
                default:
                    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &self->contentsAnimator, true, 0);
                    RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &self->altContentsAnimator, true,
                                            (self->type > ICE_ITEMBOX_1UP ? 2 : 0) + self->type - 5);
                    if (self->type != ICE_ITEMBOX_1UP) {
                        if (globals->gameMode == MODE_COMPETITION) {
                            if (globals->itemMode == ITEMS_RANDOM) {
                                self->type                        = ICE_ITEMBOX_EGGMAN;
                                self->altContentsAnimator.frameID = 13;
                            }
                            else if (globals->itemMode == ITEMS_TELEPORT) {
                                self->type                        = ICE_ITEMBOX_SWAP;
                                self->altContentsAnimator.frameID = 12;
                            }
                        }
                    }
                    else if (globals->gameMode != MODE_TIMEATTACK) {
                        switch (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->characterID) {
                            default:
                            case ID_SONIC: break;

                            case ID_TAILS: ++self->altContentsAnimator.frameID; break;
                            case ID_KNUCKLES: self->altContentsAnimator.frameID += 2; break;
#if MANIA_USE_PLUS
                            case ID_MIGHTY: self->altContentsAnimator.frameID += 8; break;
                            case ID_RAY: self->altContentsAnimator.frameID += 9; break;
#endif
                        }

                        if (globals->gameMode == MODE_COMPETITION) {
                            if (globals->itemMode == ITEMS_RANDOM) {
                                self->type                        = ICE_ITEMBOX_RANDOM;
                                self->altContentsAnimator.frameID = 13;
                            }
                            else if (globals->itemMode == 2) {
                                self->type                        = ICE_ITEMBOX_SWAP;
                                self->altContentsAnimator.frameID = 12;
                            }
                        }
                    }
                    else {
                        self->type                        = ICE_ITEMBOX_RINGS;
                        self->altContentsAnimator.frameID = 0;
                    }
                    break;
            }

            if (self->size == ICE_SIZE_SMALL) {
                self->hitboxBlock.left   = -16;
                self->hitboxBlock.top    = -16;
                self->hitboxBlock.right  = 16;
                self->hitboxBlock.bottom = 16;
            }
            else if (!self->size) {
                self->hitboxBlock.left   = -24;
                self->hitboxBlock.top    = -20;
                self->hitboxBlock.right  = 24;
                self->hitboxBlock.bottom = 20;
            }

            self->hitboxFallCheck.left   = self->hitboxBlock.left - 8;
            self->hitboxFallCheck.top    = self->hitboxBlock.top;
            self->hitboxFallCheck.right  = self->hitboxBlock.right + 8;
            self->hitboxFallCheck.bottom = self->hitboxBlock.bottom + 8;

            self->hitboxPlayerBlockCheck.left   = self->hitboxBlock.left;
            self->hitboxPlayerBlockCheck.top    = self->hitboxBlock.top - 8;
            self->hitboxPlayerBlockCheck.right  = self->hitboxBlock.right;
            self->hitboxPlayerBlockCheck.bottom = self->hitboxBlock.bottom + 8;

            self->alpha     = 0x180;
            self->state     = Ice_State_IceBlock;
            self->stateDraw = Ice_Draw_IceBlock;
        }
    }
}

void Ice_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1")) // PGZ1 doesn't have ice anywhere, likely leftover from an earlier revision
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ice.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Ice.bin", SCOPE_STAGE);

    Ice->hitboxPlayerBlockInner.left   = -15;
    Ice->hitboxPlayerBlockInner.top    = -24;
    Ice->hitboxPlayerBlockInner.right  = 15;
    Ice->hitboxPlayerBlockInner.bottom = 24;

    Ice->hitboxPlayerBlockOuter.left   = -16;
    Ice->hitboxPlayerBlockOuter.top    = -24;
    Ice->hitboxPlayerBlockOuter.right  = 16;
    Ice->hitboxPlayerBlockOuter.bottom = 24;

    Ice->playerTimers[0] = 0;
    Ice->playerTimers[1] = 0;
    Ice->playerTimers[2] = 0;
    Ice->playerTimers[3] = 0;

    Ice->sfxFreeze        = RSDK.GetSfx("PSZ/Freeze.wav");
    Ice->sfxLedgeBreak    = RSDK.GetSfx("Stage/LedgeBreak.wav");
    Ice->sfxWindowShatter = RSDK.GetSfx("Stage/WindowShatter.wav");
    Ice->sfxStruggle      = RSDK.GetSfx("PSZ/Struggle.wav");

    Zone->timeOverCallback = Ice_TimeOver_CheckFrozen;
    Zone_AddVSSwapCallback(Ice_VSSwap_CheckFrozen);
}

void Ice_VSSwap_CheckFrozen(void)
{
#if MANIA_USE_PLUS
    EntityPlayer *player = RSDK_GET_ENTITY(Zone->swapPlayerID, Player);
#else
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
#endif

    if (player->state == Ice_PlayerState_Frozen) {
#if MANIA_USE_PLUS
        Zone->playerSwapEnabled[Zone->swapPlayerID] = false;
#else
        Ice_BreakPlayerBlock(player);
#endif
    }
}

void Ice_FreezePlayer(EntityPlayer *player)
{
    RSDK_THIS(Ice);

    if (!Zone->gotTimeOver && player->state != Ice_PlayerState_Frozen && (player->shield != SHIELD_FIRE || player->invincibleTimer > 0)) {
        EntityIce *ice = CREATE_ENTITY(Ice, INT_TO_VOID(ICE_CHILD_PLAYER), player->position.x, player->position.y);
        ice->playerPtr = player;

#if MANIA_USE_PLUS
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

        RSDK.SetSpriteAnimation(Ice->aniFrames, ice->animationID, &ice->contentsAnimator, true, 0);
        RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &ice->altContentsAnimator, true, 0);
        ice->alpha       = 0x80;
        ice->isPermanent = true;

        player->velocity.x >>= 1;
        player->groundVel >>= 1;
        player->animator.speed  = 0;
        player->visible         = false;
        player->nextAirState    = StateMachine_None;
        player->nextGroundState = StateMachine_None;
        player->state           = Ice_PlayerState_Frozen;
        player->velocity.y      = 0;

        RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        player->abilityPtrs[0] = self;
        player->abilityPtrs[1] = ice;
        if (!self->collisionMode)
            player->position.y += Player_GetHitbox(player)->bottom - Ice->hitboxPlayerBlockOuter.bottom;

        player->blinkTimer     = 0;
        player->outerbox       = &Ice->hitboxPlayerBlockOuter;
        player->innerbox       = &Ice->hitboxPlayerBlockInner;
        player->timer          = 0;
        player->abilityTimer   = 0;
        player->spindashCharge = 0;

        RSDK.PlaySfx(Ice->sfxFreeze, false, 255);
    }
}

bool32 Ice_CheckPlayerBlockSmashH(void)
{
    RSDK_THIS(Player);

    if (abs(self->skidding) >= 0x50000 && abs(self->pushing) <= abs(self->skidding)) {
        self->velocity.x = self->skidding < 0 ? 0x18000 : -0x18000;

        Ice_BreakPlayerBlock(self);

        return true;
    }

    return false;
}

bool32 Ice_CheckPlayerBlockSmashV(void)
{
    RSDK_THIS(Player);

    if (abs(self->pushing) >= 0x80000 && abs(self->skidding) <= abs(self->pushing)) {
        if (self->pushing > 0 && (self->angle & 0xFF) < 0x20) {
            Ice_BreakPlayerBlock(self);
            return true;
        }

        if (self->pushing && (self->angle + 0x80) < 0x20) {
            Ice_BreakPlayerBlock(self);
            return true;
        }
    }

    return false;
}

void Ice_PlayerState_Frozen(void)
{
    RSDK_THIS(Player);

    bool32 onGround = self->onGround;
    int32 xVel      = self->groundVel;
    int32 yVel      = self->groundedStore;

    if (!onGround) {
        onGround |= self->groundedStore || self->velocity.x;
        xVel = self->velocity.x;
        yVel = self->velocity.y;
    }

    if (!onGround) {
        onGround |= abs(self->skidding) < 0x50000 || !Ice_CheckPlayerBlockSmashH();
        xVel = self->velocity.x;
        yVel = self->velocity.y;
    }

    if (onGround) {
        if (self->onGround && !self->groundVel) {
            if (Ice_CheckPlayerBlockSmashH())
                return;
        }

        self->skidding = xVel;

        if (!yVel) {
            if (Ice_CheckPlayerBlockSmashV())
                return;
        }

        self->pushing = self->velocity.y;
        if (self->shield == SHIELD_FIRE && self->invincibleTimer <= 0) {
            self->state      = Player_State_Air;
            self->velocity.y = -self->jumpStrength >> 1;
            self->onGround   = false;
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JUMP, &self->animator, true, 0);

            Ice_ShatterGenerator(64, 24, 20, 0, 0, 2);
            RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
            Ice->playerTimers[RSDK.GetEntitySlot(self)] = 30;

            self->skidding       = 0;
            self->pushing        = 0;
            self->timer          = 0;
            self->abilityTimer   = 0;
            self->spindashCharge = 0;
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
                    Ice_ShatterGenerator(64, 24, 20, 0, 0, 2);
                    RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                    Ice->playerTimers[RSDK.GetEntitySlot(self)] = 30;
                    self->skidding                              = 0;
                    self->pushing                               = 0;
                    self->timer                                 = 0;
                    self->abilityTimer                          = 0;
                    self->spindashCharge                        = 0;
                }
                else {
                    Ice_ShatterGenerator(8, 24, 20, 0, 0, false);
                    RSDK.PlaySfx(Ice->sfxStruggle, false, 255);
                }
            }
        }

        if (self->state == Ice_PlayerState_Frozen) {
            if (self->onGround) {
                int32 rollFric        = self->rollingFriction;
                self->left            = false;
                self->right           = false;
                self->rollingFriction = 0;
                Player_HandleRollDeceleration();

                if (!self->groundVel)
                    self->groundVel += (5000 * RSDK.Sin256(self->angle)) >> 8;

                if (self->camera)
                    self->camera->disableYOffset = false;

                self->jumpAbilityState = 0;
                self->rollingFriction  = rollFric;

                if (self->state == Player_State_Ground)
                    self->state = Ice_PlayerState_Frozen;
            }
            else {
                self->velocity.y += 0x3800;
            }
        }
    }
}

void Ice_ShatterGenerator(int32 count, int32 sizeX, int32 sizeY, int32 velX, int32 velY, int32 canBreak)
{
    RSDK_THIS(Ice);

    if (canBreak > 0)
        count >>= 1;

    for (int32 i = 0; i < MAX(0, count); ++i) {
        int32 x        = self->position.x + (RSDK.Rand(-sizeX, sizeX + 1) << 16);
        int32 y        = self->position.y + (RSDK.Rand(-sizeY, sizeY + 1) << 16);
        EntityIce *ice = CREATE_ENTITY(Ice, INT_TO_VOID(ICE_CHILD_SHARD), x, y);

        ice->velocity.x          = velX + (RSDK.Rand(-6, 8) << 15);
        ice->velocity.y          = velY + (RSDK.Rand(-10, 2) << 15);
        ice->direction           = RSDK.Rand(0, 4);
        ice->blockAnimator.speed = RSDK.Rand(1, 4);

        if (canBreak) {
            if (RSDK.Rand(0, 2)) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PIECE, &ice->blockAnimator, true, 0);
                ice->velocity.x = (ice->velocity.x >> 1) + (ice->velocity.x >> 2);
                ice->velocity.y = (ice->velocity.y >> 1) + (ice->velocity.y >> 2);
            }
        }
    }
}

// Like Ice_Shatter, but "shatters" the contents too
void Ice_FullShatter(EntityPlayer *player, int32 velX, int32 velY)
{
    RSDK_THIS(Ice);

    if (self->type == ICE_SPIKES || (self->type == ICE_SPRING && self->subType < 3))
        self->type = ICE_BLOCK;

    EntityItemBox *itemBox = (EntityItemBox *)Ice_Shatter(self, velX, velY);
    if (player && itemBox) {
        if (player->sidekick)
            player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        itemBox->contentsPos.x = itemBox->position.x;
        itemBox->contentsPos.y = itemBox->position.y - 0x30000;
        ItemBox_Break(itemBox, player);
    }
}

void Ice_BreakPlayerBlock(EntityPlayer *player)
{
    RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
    Ice_ShatterGenerator(64, 24, 20, 0, 0, 2);
    Ice->playerTimers[RSDK.GetEntitySlot(player)] = 30;

    player->skidding       = 0;
    player->pushing        = 0;
    player->timer          = 0;
    player->abilityTimer   = 0;
    player->spindashCharge = 0;
    player->visible        = true;
    if (player->state != Player_State_Death) {
        player->outerbox = NULL;
        player->innerbox = NULL;
        player->state    = Player_State_Hurt;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, true, 0);

        player->velocity.y = -0x38000;
        player->onGround   = false;
        if (player->gravityStrength == 0x1000) {
            player->velocity.x >>= 1;
            player->velocity.y = -0x1C000;
        }
    }
}

EntityItemBox *Ice_Shatter(EntityIce *ice, int32 velX, int32 velY)
{
    RSDK_THIS(Ice);

    RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);

    if (self->blockAnimator.animationID == ICEANI_PILLARBLOCK) {
        self->position.y -= 0x370000;
        Ice_ShatterGenerator(96, 19, 55, velX, velY, 2);
    }
    else if (self->size == ICE_SIZE_SMALL) {
        Ice_ShatterGenerator(16, 16, 16, velX, velY, 2);
    }
    else {
        Ice_ShatterGenerator(64, 24, 20, velX, velY, 2);
    }

    EntityItemBox *itemBox = NULL;
    int32 count            = 0;
    switch (ice->type) {
        case ICE_BLOCK: break;

        case ICE_1RING: count = 1;
        // [Fallthrough]
        case ICE_3RINGS:
            if (ice->type == ICE_3RINGS)
                count = 3;
        // [Fallthrough]
        case ICE_5RINGS: {
            if (ice->type == ICE_5RINGS)
                count = 5;

            int32 angle = 16 * (12 - (count >> 1));
            for (int32 i = 0; i < count; ++i) {
                EntityRing *ring = CREATE_ENTITY(Ring, INT_TO_VOID(1), ice->position.x, ice->position.y);
                ring->velocity.x = velX + 640 * RSDK.Cos256(angle);
                ring->velocity.y = velY + 640 * RSDK.Sin256(angle);
                ring->position.x += ring->velocity.x * (3 * ice->type - 3);
                ring->position.y += ring->velocity.y * (3 * ice->type - 3);
                ring->timer          = 32;
                ring->collisionPlane = 0;
                ring->inkEffect      = INK_ALPHA;
                ring->alpha          = 256;
                ring->state          = Ring_State_Lost;
                ring->position.y += (0x50000 * ice->type) - 0x50000;
                ring->animator.speed = 512;
                ring->stateDraw      = Ring_Draw_Normal;
                angle += 16;
            }
            break;
        }

        case ICE_SPIKES:
            CREATE_ENTITY(Spikes, INT_TO_VOID(self->subType), ice->position.x + self->contentsOffset.x, ice->position.y + self->contentsOffset.y);
            break;

        case ICE_SPRING:
            if (self->subType >= 3)
                CREATE_ENTITY(Spring, INT_TO_VOID((self->subFlip << 8) - 3 + self->subType), ice->position.x + self->contentsOffset.x,
                              ice->position.y + self->contentsOffset.y);
            else
                CREATE_ENTITY(IceSpring, INT_TO_VOID(self->subType + (self->subFlip << 8)), ice->position.x + self->contentsOffset.x,
                              ice->position.y + self->contentsOffset.y);
            break;

        default: {
            int32 type          = ice->type - ICE_ITEMBOX_RINGS;
            itemBox             = CREATE_ENTITY(ItemBox, INT_TO_VOID(type + (ice->type > ICE_ITEMBOX_1UP ? 2 : 0)), ice->position.x, ice->position.y);
            itemBox->velocity.y = -0x18000;
            itemBox->state      = ItemBox_State_Falling;
            break;
        }
    }

    Ice_UpdateBlockGravity();
    destroyEntity(ice);

    return itemBox;
}

void Ice_TimeOver_CheckFrozen(void)
{
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (player->state == Ice_PlayerState_Frozen)
        Ice_BreakPlayerBlock(player);
}

void Ice_UpdateBlockGravity(void)
{
    RSDK_THIS(Ice);

    foreach_all(Ice, ice)
    {
        if (ice != self && ice->state == Ice_State_IceBlock
            && RSDK.CheckObjectCollisionTouchBox(self, &self->hitboxBlock, ice, &ice->hitboxFallCheck)) {
            ice->state = Ice_State_StartBlockFall;
        }
    }
}

void Ice_State_IceBlock(void)
{
    RSDK_THIS(Ice);

    foreach_active(Player, player)
    {
        bool32 noCollision = true;
        int32 playerX      = player->position.x;
        int32 playerY      = player->position.y;

        if (player->state == Ice_PlayerState_Frozen) {
            switch (MathHelpers_CheckBoxCollision(self, &self->hitboxPlayerBlockCheck, player, &Ice->hitboxPlayerBlockOuter)) {
                default:
                case C_NONE:
                    player->position.x = playerX;
                    player->position.y = playerY;

                    self->position.x -= self->playerMoveOffset.x;
                    self->position.y -= self->playerMoveOffset.y;
                    if (Player_CheckCollisionBox(player, self, &self->hitboxBlock) == C_TOP) {
                        player->position.x += self->playerMoveOffset.x;
                        player->position.y += self->playerMoveOffset.y;
                    }

                    self->position.x += self->playerMoveOffset.x;
                    self->position.y += self->playerMoveOffset.y;
                    noCollision = false;
                    break;

                case C_TOP:
                    if (player->velocity.y < 0x40000) {
                        player->position.x = playerX;
                        player->position.y = playerY;

                        self->position.x -= self->playerMoveOffset.x;
                        self->position.y -= self->playerMoveOffset.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitboxBlock) == C_TOP) {
                            player->position.x += self->playerMoveOffset.x;
                            player->position.y += self->playerMoveOffset.y;
                        }

                        self->position.x += self->playerMoveOffset.x;
                        self->position.y += self->playerMoveOffset.y;
                        noCollision = false;
                    }
                    else {
                        Ice_FullShatter(player, 0, 0);
                        player->velocity.y = -0x20000;

                        foreach_return;
                    }
                    break;

                case C_LEFT:
                    if (player->velocity.x < 0x20000) {
                        player->position.x = playerX;
                        player->position.y = playerY;

                        self->position.x -= self->playerMoveOffset.x;
                        self->position.y -= self->playerMoveOffset.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitboxBlock) == C_TOP) {
                            player->position.x += self->playerMoveOffset.x;
                            player->position.y += self->playerMoveOffset.y;
                        }

                        self->position.x += self->playerMoveOffset.x;
                        self->position.y += self->playerMoveOffset.y;
                        noCollision = false;
                    }
                    else {
                        Ice_FullShatter(player, player->velocity.x >> 1, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;

                        foreach_return;
                    }
                    break;

                case C_RIGHT:
                    if (player->velocity.x > -0x20000) {
                        player->position.x = playerX;
                        player->position.y = playerY;

                        self->position.x -= self->playerMoveOffset.x;
                        self->position.y -= self->playerMoveOffset.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitboxBlock) == C_TOP) {
                            player->position.x += self->playerMoveOffset.x;
                            player->position.y += self->playerMoveOffset.y;
                        }

                        self->position.x += self->playerMoveOffset.x;
                        self->position.y += self->playerMoveOffset.y;
                        noCollision = false;
                    }
                    else {
                        Ice_FullShatter(player, player->velocity.x >> 1, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;

                        foreach_return;
                    }
                    break;

                case C_BOTTOM:
                    if (player->velocity.y > -0x40000) {
                        player->position.x = playerX;
                        player->position.y = playerY;

                        self->position.x -= self->playerMoveOffset.x;
                        self->position.y -= self->playerMoveOffset.y;
                        if (Player_CheckCollisionBox(player, self, &self->hitboxBlock) == C_TOP) {
                            player->position.x += self->playerMoveOffset.x;
                            player->position.y += self->playerMoveOffset.y;
                        }

                        self->position.x += self->playerMoveOffset.x;
                        self->position.y += self->playerMoveOffset.y;
                        noCollision = false;
                    }
                    else {
                        Ice_FullShatter(player, 0, player->velocity.y >> 1);
                        player->position.x = playerX;
                        player->position.y = playerY;

                        foreach_return;
                    }
                    break;
            }
        }

        if (noCollision) {
            switch (MathHelpers_CheckBoxCollision(self, &self->hitboxBlock, player, &Ice->hitboxPlayerBlockOuter)) {
                default:
                case C_NONE:
                case C_TOP: break;

                case C_LEFT:
                case C_RIGHT:
                    if (self->knuxSmash && player->characterID == ID_KNUCKLES) {
                        Ice_Shatter(self, 0, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    else if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0) {
                        if (RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield)->shieldAnimator.animationID
                            == SHIELDANI_FIREATTACK) {
                            if (player->position.x >= self->position.x) {
                                if (player->velocity.x <= -0x78000) {
                                    Ice_FullShatter(player, player->velocity.x, 0);
                                    player->position.x = playerX;
                                    player->position.y = playerY;
                                    foreach_return;
                                }
                            }
                            else if (player->velocity.x >= 0x78000) {
                                Ice_FullShatter(player, player->velocity.x, 0);
                                player->position.x = playerX;
                                player->position.y = playerY;
                                foreach_return;
                            }
                        }
                    }
                    break;

                case C_BOTTOM:
                    if (self->bottomSmash) {
                        Ice_Shatter(self, 0, 0);
                        player->position.x = playerX;
                        player->position.y = playerY;
                        foreach_return;
                    }
                    break;
            }

            player->position.x = playerX;
            player->position.y = playerY;
            self->position.x -= self->playerMoveOffset.x;
            self->position.y -= self->playerMoveOffset.y;

#if MANIA_USE_PLUS
            int32 prevVel = player->velocity.y;
#endif
            int32 side = Player_CheckCollisionBox(player, self, &self->hitboxBlock);
            if (side) {
                if (player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !self->glintTimer) {
                    if (self->blockAnimator.animationID == ICEANI_PILLARBLOCK) {
                        self->position.y -= 0x370000;
                        Ice_ShatterGenerator(36, 19, 55, 0, 0, false);
                    }
                    else if (self->size == ICE_SIZE_SMALL) {
                        Ice_ShatterGenerator(6, 19, 17, 0, 0, false);
                    }
                    else {
                        Ice_ShatterGenerator(12, 19, 20, 0, 0, false);
                    }

                    self->blockAnimator.frameID += 2;
                    self->glintTimer = 15;
                    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_LARGEGLINT + self->size, &self->glintAnimator, true, 0);
                }

                if (side == C_BOTTOM) {
                    if (player->onGround && !player->collisionMode && self->playerMoveOffset.y >= 0) {
                        if (player->shield == SHIELD_FIRE || player->invincibleTimer) {
                            player->position.x = playerX;
                            player->position.y = playerY;
                            Ice_Shatter(self, 0, 0);
                            foreach_return;
                        }
                        player->deathType = PLAYER_DEATH_DIE_USESFX;
                    }
                }
                else if (side == C_TOP && player->velocity.y >= 0) {
                    if (self->blockAnimator.animationID == ICEANI_PILLARBLOCK) {
                        Player_Hurt(player, self);
                    }
                    else {
                        if ((player->animator.animationID == ANI_JUMP || player->state == Player_State_DropDash) && !player->sidekick) {
                            Ice_Shatter(self, 0, 0);
                            player->velocity.y = -0x30000;
                            player->onGround   = false;

                            foreach_return;
                        }
#if MANIA_USE_PLUS
                        else if (player->state == Player_State_MightyHammerDrop && !player->sidekick) {
                            Ice_FullShatter(player, 0, player->velocity.y);
                            player->velocity.y = prevVel - 0x10000;
                            player->onGround   = false;

                            foreach_return;
                        }
#endif
                    }

                    player->position.x += self->playerMoveOffset.x;
                    player->position.y += self->playerMoveOffset.y;
                }
            }

            self->position.x += self->playerMoveOffset.x;
            self->position.y += self->playerMoveOffset.y;
        }
    }

    RSDK.ProcessAnimation(&self->glintAnimator);

    if (self->blockAnimator.animationID == ICEANI_PILLARBLOCK) {
        if (!RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, self->hitboxBlock.bottom << 16, true)
            && self->state == Ice_State_IceBlock) {
            self->state = Ice_State_StartBlockFall;
        }

        RSDK.ProcessAnimation(&self->blockAnimator);

        if (--self->glintTimer == 54)
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PILLARGLINT, &self->glintAnimator, true, 1);
        else if (!self->glintTimer)
            Ice_Shatter(self, 0, 0);
    }
    else {
        if (self->glintTimer > 0) {
            self->glintTimer--;

            if (!self->glintTimer) {
                if (self->type == ICE_SPIKES || (self->type == ICE_SPRING && self->subType < 3))
                    self->type = ICE_BLOCK;

                Ice_Shatter(self, 0, 0);
            }
        }
    }
}

void Ice_State_StartBlockFall(void)
{
    RSDK_THIS(Ice);

    Ice_UpdateBlockGravity();

    self->timer  = 15;
    self->state  = Ice_State_BlockFallDelay;
    self->active = ACTIVE_NORMAL;

    Ice_State_IceBlock();
}

void Ice_State_BlockFallDelay(void)
{
    RSDK_THIS(Ice);

    if (!--self->timer) {
        Ice_UpdateBlockGravity();
        self->state = Ice_State_IceBlockFall;
    }

    Ice_State_IceBlockFall();
    self->velocity.y = 0;
}

void Ice_State_IceBlockFall(void)
{
    RSDK_THIS(Ice);

    self->position.y         = self->velocity.y + self->position.y;
    self->playerMoveOffset.y = -(int32)(self->position.y & 0xFFFF0000);
    self->playerMoveOffset.y += (self->velocity.y + self->position.y) & 0xFFFF0000;
    self->velocity.y += 0x3800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, self->hitboxBlock.bottom << 16, true)) {
        self->velocity.y = 0;

        self->state = Ice_State_IceBlock;
        Ice_State_IceBlock();
    }
    else {
        int32 velY = self->velocity.y;
        foreach_all(Ice, ice)
        {
            if (ice != self) {
                if ((ice->stateDraw == Ice_Draw_IceBlock || ice->stateDraw == Ice_Draw_PlayerBlock)
                    && RSDK.CheckObjectCollisionPlatform(ice, &ice->hitboxBlock, self, &self->hitboxBlock, true)) {
                    self->velocity.y = 0;

                    if (ice->state == Ice_State_IceBlock) {
                        self->active = ACTIVE_BOUNDS;
                        self->state  = Ice_State_IceBlock;
                    }
                }
            }
        }

        foreach_all(Spikes, spikes)
        {
            if (RSDK.CheckObjectCollisionPlatform(spikes, &spikes->hitbox, self, &self->hitboxBlock, true)) {
                self->velocity.y = 0;
                self->active     = ACTIVE_BOUNDS;
                self->state      = Ice_State_IceBlock;
            }
        }

        foreach_all(ItemBox, itemBox)
        {
            if ((itemBox->state == ItemBox_State_Idle || itemBox->state == ItemBox_State_Falling)
                && RSDK.CheckObjectCollisionPlatform(itemBox, &ItemBox->hitboxItemBox, self, &self->hitboxBlock, true)) {
                self->velocity.y = 0;

                if (itemBox->onGround) {
                    self->active = ACTIVE_BOUNDS;
                    self->state  = Ice_State_IceBlock;
                }
            }
        }

        if (!self->velocity.y && velY >= 0x60000) {
            if (ice) {
                if (ice->stateDraw == Ice_Draw_PlayerBlock) {
                    Ice_BreakPlayerBlock(ice->playerPtr);
                }
                else {
                    Entity *storeEntity = SceneInfo->entity;
                    SceneInfo->entity   = (Entity *)ice;

                    if (ice->type == ICE_SPIKES || (ice->type == ICE_SPRING && ice->subType < 3))
                        ice->type = ICE_BLOCK;

                    Ice_Shatter(ice, 0, 0);

                    SceneInfo->entity = storeEntity;
                }
            }

            if (self->type == ICE_SPIKES || (self->type == ICE_SPRING && self->subType < 3))
                self->type = ICE_BLOCK;

            Ice_Shatter(self, 0, 0);
        }

        Ice_State_IceBlock();
    }
}

void Ice_State_PlayerBlock(void)
{
    RSDK_THIS(Ice);

    RSDK.ProcessAnimation(&self->contentsAnimator);
    EntityPlayer *playerPtr = self->playerPtr;

    if (playerPtr->state == Ice_PlayerState_Frozen) {
        RSDK.ProcessAnimation(&self->altContentsAnimator);

#if MANIA_USE_PLUS
        if (self->contentsAnimator.animationID == self->animationID + 3) {
            if (self->contentsAnimator.frameID == self->contentsAnimator.frameCount - 1)
                RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID, &self->contentsAnimator, false, 0);

            if (--self->glintTimer <= 0) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &self->altContentsAnimator, true, 0);
                self->glintTimer = 30 * RSDK.Rand(1, 9);
            }

            self->blockAnimator.frameID = playerPtr->timer;

            if (playerPtr->spindashCharge) {
                self->contentsOffset.x = RSDK.Rand(-1, 2) << 16;
                self->contentsOffset.y = RSDK.Rand(-1, 2) << 16;
            }
            else {
                self->contentsOffset.x = 0;
                self->contentsOffset.y = 0;
            }

            self->playerMoveOffset.x = (playerPtr->position.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
            self->playerMoveOffset.y = (playerPtr->position.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

            self->position.x = playerPtr->position.x;
            self->position.y = playerPtr->position.y;
            self->drawGroup  = playerPtr->drawGroup + 1;
        }
        else {
            if (playerPtr->jumpPress) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 3, &self->contentsAnimator, false, 0);
            }
            else {
                if (playerPtr->velocity.x >= -0x60000) {
                    if (playerPtr->velocity.x <= 0x60000) {
                        if (playerPtr->left)
                            RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 1, &self->contentsAnimator, false, 0);
                        else if (playerPtr->right)
                            RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 2, &self->contentsAnimator, false, 0);
                    }
                    else {
                        RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 2, &self->contentsAnimator, false, 0);
                    }
                }
                else {
                    RSDK.SetSpriteAnimation(Ice->aniFrames, self->animationID + 1, &self->contentsAnimator, false, 0);
                }
            }

            if (--self->glintTimer <= 0) {
                RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &self->altContentsAnimator, true, 0);
                self->glintTimer = 30 * RSDK.Rand(1, 9);
            }

            self->blockAnimator.frameID = playerPtr->timer;

            if (playerPtr->spindashCharge) {
                self->contentsOffset.x = RSDK.Rand(-1, 2) << 16;
                self->contentsOffset.y = RSDK.Rand(-1, 2) << 16;
            }
            else {
                self->contentsOffset.x = 0;
                self->contentsOffset.y = 0;
            }

            self->playerMoveOffset.x = (playerPtr->position.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
            self->playerMoveOffset.y = (playerPtr->position.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

            self->position.x = playerPtr->position.x;
            self->position.y = playerPtr->position.y;
            self->drawGroup  = playerPtr->drawGroup + 1;
        }
    }
    else {
        if (playerPtr->state != Player_State_FlyToPlayer && playerPtr->state != Player_State_ReturnToPlayer) {
            if (!Player_CheckValidState(playerPtr)) {
                Ice_BreakPlayerBlock(self->playerPtr);
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
        if (--self->glintTimer <= 0) {
            RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_PLAYERGLINT, &self->altContentsAnimator, true, 0);
            self->glintTimer = 30 * RSDK.Rand(1, 9);
        }

        self->blockAnimator.frameID = playerPtr->timer;

        if (playerPtr->spindashCharge) {
            self->contentsOffset.x = RSDK.Rand(-1, 2) << 16;
            self->contentsOffset.y = RSDK.Rand(-1, 2) << 16;
        }
        else {
            self->contentsOffset.x = 0;
            self->contentsOffset.y = 0;
        }

        self->playerMoveOffset.x = (playerPtr->position.x & 0xFFFF0000) - (self->position.x & 0xFFFF0000);
        self->playerMoveOffset.y = (playerPtr->position.y & 0xFFFF0000) - (self->position.y & 0xFFFF0000);

        self->position.x = playerPtr->position.x;
        self->position.y = playerPtr->position.y;
        self->drawGroup  = playerPtr->drawGroup + 1;
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
        if (player != self->playerPtr && player->stateInput != Player_Input_P2_AI && player->state != Ice_PlayerState_Frozen && player->velocity.y > 0
            && !player->onGround && player->position.y < self->position.y - 0x200000 && Player_CheckBadnikTouch(player, self, &self->hitboxBlock)) {
            if (player->animator.animationID == ANI_JUMP || player->animator.animationID == ANI_DROPDASH) {
                Ice_BreakPlayerBlock(self->playerPtr);
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

    RSDK.ProcessAnimation(&self->blockAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->visible ^= true;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Ice_Draw_IceBlock(void)
{
    RSDK_THIS(Ice);
    Vector2 drawPos;

    int32 dirStore = self->direction;

    drawPos.x       = self->position.x + self->contentsOffset.x;
    drawPos.y       = self->position.y + self->contentsOffset.y;
    self->direction = self->subFlip;
    RSDK.DrawSprite(&self->contentsAnimator, &drawPos, false);

    self->direction = dirStore;
    drawPos.y -= 0x30000;
    RSDK.DrawSprite(&self->altContentsAnimator, &drawPos, false);

#if MANIA_USE_PLUS
    if (!SceneInfo->inEditor) {
        self->inkEffect = INK_ADD;
        self->blockAnimator.frameID += !self->glintTimer ? 4 : 2;
        RSDK.DrawSprite(&self->blockAnimator, NULL, false);

        self->blockAnimator.frameID -= !self->glintTimer ? 4 : 2;
        self->inkEffect = INK_NONE;
    }
#endif

    RSDK.DrawSprite(&self->blockAnimator, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->glintAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Ice_Draw_PlayerBlock(void)
{
    RSDK_THIS(Ice);
    Vector2 drawPos;

    drawPos.x = self->position.x + self->contentsOffset.x;
    drawPos.y = self->position.y + self->contentsOffset.y;

#if MANIA_USE_PLUS
    int32 frame                 = self->blockAnimator.frameID;
    self->inkEffect             = INK_SUB;
    self->blockAnimator.frameID = 5;
    RSDK.DrawSprite(&self->blockAnimator, NULL, false);

    self->blockAnimator.frameID = frame;
    self->inkEffect             = INK_NONE;
#endif

    RSDK.DrawSprite(&self->contentsAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->blockAnimator, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->altContentsAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Ice_Draw_Pillar(void)
{
    RSDK_THIS(Ice);

    RSDK.DrawSprite(&self->blockAnimator, NULL, false);

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->glintAnimator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Ice_Draw_Shard(void)
{
    RSDK_THIS(Ice);

    RSDK.DrawSprite(&self->blockAnimator, NULL, false);
}

#if GAME_INCLUDE_EDITOR
void Ice_EditorDraw(void)
{
    RSDK_THIS(Ice);

    self->drawFX        = FX_FLIP;
    self->drawGroup     = Zone->playerDrawGroup[0] + 1;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->alpha         = 0x180;

    int32 type    = self->type;
    int32 subType = self->subType;
    int32 subFlip = self->subFlip;

    RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_ICEBLOCK, &self->blockAnimator, true, self->size);
    RSDK.SetSpriteAnimation(-1, 0, &self->altContentsAnimator, true, 0);

    switch (self->type) {
        case ICE_BLOCK: RSDK.SetSpriteAnimation(-1, ICEANI_ICEBLOCK, &self->contentsAnimator, true, 0); break;
        case ICE_1RING: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->contentsAnimator, true, 0); break;
        case ICE_3RINGS: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->contentsAnimator, true, 1); break;
        case ICE_5RINGS: RSDK.SetSpriteAnimation(Ice->aniFrames, ICEANI_RINGS, &self->contentsAnimator, true, 2); break;

        case ICE_SPIKES:
            self->subType = self->subType & 3;
            RSDK.SetSpriteAnimation(Spikes->aniFrames, self->subType >> 1, &self->contentsAnimator, true, 0);

            if (!self->size) {
                // direction of da spikes, matches the spikes "type" variable
                switch (self->subType) {
                    case 0:
                        self->contentsOffset.y = 0x40000;
                        self->subFlip          = FLIP_NONE;
                        break;

                    case 1:
                        self->contentsOffset.y = -0x40000;
                        self->subFlip          = FLIP_Y;
                        break;

                    case 2:
                        self->contentsOffset.x = -0x80000;
                        self->subFlip          = FLIP_NONE;
                        break;

                    case 3:
                        self->contentsOffset.x = 0x80000;
                        self->subFlip          = FLIP_X;
                        break;

                    default: break;
                }
            }
            break;

        case ICE_SPRING:
            // Ice Springs (0-2), Normal Springs (2-8)
            if (self->subType >= 3)
                RSDK.SetSpriteAnimation(Spring->aniFrames, (self->subType - 3), &self->contentsAnimator, true, 0);
            else
                RSDK.SetSpriteAnimation(IceSpring->aniFrames, self->subType, &self->contentsAnimator, true, 0);

            if (self->size) {
                switch (self->subType) {
                    case 0:
                    case 3:
                    case 4:
                        self->contentsOffset.x = 0;
                        if (!(self->subFlip & FLIP_Y))
                            self->contentsOffset.y = 0x80000;
                        else
                            self->contentsOffset.y = -0x80000;
                        break;

                    case 1:
                    case 5:
                    case 6:
                        self->contentsOffset.y = 0;
                        if (!(self->subFlip & FLIP_X))
                            self->contentsOffset.x = -0x80000;
                        else
                            self->contentsOffset.x = 0x80000;
                        break;

                    case 2:
                    case 7:
                    case 8:
                        self->contentsOffset.x = 0;
                        if (!(self->subFlip & FLIP_Y))
                            self->contentsOffset.y = -0x10000;
                        else
                            self->contentsOffset.y = 0x10000;
                        break;

                    default: break;
                }
            }
            else {
                switch (self->subType) {
                    case 0:
                    case 3:
                    case 4:
                        self->contentsOffset.x = 0;
                        if (!(self->subFlip & FLIP_Y))
                            self->contentsOffset.y = 0xC0000;
                        else
                            self->contentsOffset.y = -0xC0000;
                        break;

                    case 1:
                    case 5:
                    case 6:
                        self->contentsOffset.y = 0;
                        if (!(self->subFlip & FLIP_X))
                            self->contentsOffset.x = -0x100000;
                        else
                            self->contentsOffset.x = 0x100000;
                        break;

                    case 2:
                    case 7:
                    case 8:
                        if (!(self->subFlip & FLIP_X))
                            self->contentsOffset.x = -0x80000;
                        else
                            self->contentsOffset.x = 0x80000;
                        if (!(self->subFlip & FLIP_Y))
                            self->contentsOffset.y = 0x40000;
                        else
                            self->contentsOffset.y = -0x40000;
                        break;

                    default: break;
                }
            }
            break;

        case ICE_ITEMBOX_RINGS:
        case ICE_ITEMBOX_BLUESHIELD:
        case ICE_ITEMBOX_BUBBLESHIELD:
        case ICE_ITEMBOX_FIRESHIELD:
        case ICE_ITEMBOX_LIGHTNINGSHIELD:
        case ICE_ITEMBOX_INVINCIBILITY:
        case ICE_ITEMBOX_SNEAKERS:
        case ICE_ITEMBOX_1UP:
        case ICE_ITEMBOX_EGGMAN:
        case ICE_ITEMBOX_HYPERRING:
        case ICE_ITEMBOX_SWAP:
        case ICE_ITEMBOX_RANDOM:
        case ICE_ITEMBOX_SUPER:
        default:
            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 0, &self->contentsAnimator, true, 0);
            RSDK.SetSpriteAnimation(ItemBox->aniFrames, 2, &self->altContentsAnimator, true, (self->type > ICE_ITEMBOX_1UP ? 2 : 0) + self->type - 5);

            if (self->type != ICE_ITEMBOX_1UP) {
                if (globals->gameMode == MODE_COMPETITION) {
                    if (globals->itemMode == ITEMS_RANDOM) {
                        self->type                        = ICE_ITEMBOX_EGGMAN;
                        self->altContentsAnimator.frameID = 13;
                    }
                    else if (globals->itemMode == ITEMS_TELEPORT) {
                        self->type                        = ICE_ITEMBOX_SWAP;
                        self->altContentsAnimator.frameID = 12;
                    }
                }
            }
            else if (globals->gameMode != MODE_TIMEATTACK) {
                switch (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->characterID) {
                    default:
                    case ID_SONIC: break;
                    case ID_TAILS: ++self->altContentsAnimator.frameID; break;
                    case ID_KNUCKLES: self->altContentsAnimator.frameID += 2; break;
#if MANIA_USE_PLUS
                    case ID_MIGHTY: self->altContentsAnimator.frameID += 8; break;
                    case ID_RAY: self->altContentsAnimator.frameID += 9; break;
#endif
                }

                if (globals->gameMode == MODE_COMPETITION) {
                    if (globals->itemMode == ITEMS_RANDOM) {
                        self->type                        = ICE_ITEMBOX_RANDOM;
                        self->altContentsAnimator.frameID = 13;
                    }
                    else if (globals->itemMode == 2) {
                        self->type                        = ICE_ITEMBOX_SWAP;
                        self->altContentsAnimator.frameID = 12;
                    }
                }
            }
            else {
                self->type                        = ICE_ITEMBOX_RINGS;
                self->altContentsAnimator.frameID = 0;
            }
            break;
    }

    Ice_Draw_IceBlock();

    self->type    = type;
    self->subType = subType;
    self->subFlip = subFlip;
}

void Ice_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("PSZ1"))
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Ice.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("PSZ2"))
        Ice->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Ice.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Ice, size);
    RSDK_ENUM_VAR("Large", ICE_SIZE_LARGE);
    RSDK_ENUM_VAR("Small", ICE_SIZE_SMALL);

    RSDK_ACTIVE_VAR(Ice, type);
    RSDK_ENUM_VAR("Empty Block", ICE_BLOCK);
    RSDK_ENUM_VAR("1 Ring", ICE_1RING);
    RSDK_ENUM_VAR("3 Rings", ICE_3RINGS);
    RSDK_ENUM_VAR("5 Rings", ICE_5RINGS);
    RSDK_ENUM_VAR("Spikes", ICE_SPIKES);
    RSDK_ENUM_VAR("ItemBox (Rings)", ICE_ITEMBOX_RINGS);
    RSDK_ENUM_VAR("ItemBox (Blue Shield)", ICE_ITEMBOX_BLUESHIELD);
    RSDK_ENUM_VAR("ItemBox (Bubble Shield)", ICE_ITEMBOX_BUBBLESHIELD);
    RSDK_ENUM_VAR("ItemBox (Fire Shield)", ICE_ITEMBOX_FIRESHIELD);
    RSDK_ENUM_VAR("ItemBox (Lightning Shield)", ICE_ITEMBOX_LIGHTNINGSHIELD);
    RSDK_ENUM_VAR("ItemBox (Invincibility)", ICE_ITEMBOX_INVINCIBILITY);
    RSDK_ENUM_VAR("ItemBox (Speed Shoes)", ICE_ITEMBOX_SNEAKERS);
    RSDK_ENUM_VAR("ItemBox (1UP)", ICE_ITEMBOX_1UP);
    RSDK_ENUM_VAR("ItemBox (Eggman)", ICE_ITEMBOX_EGGMAN);
    RSDK_ENUM_VAR("ItemBox (Hyper Ring)", ICE_ITEMBOX_HYPERRING);
    RSDK_ENUM_VAR("ItemBox (Swap)", ICE_ITEMBOX_SWAP);
    RSDK_ENUM_VAR("ItemBox (Random)", ICE_ITEMBOX_SUPER);
    RSDK_ENUM_VAR("ItemBox (Super)", ICE_ITEMBOX_RANDOM);
    RSDK_ENUM_VAR("Spring", ICE_SPRING);

    RSDK_ACTIVE_VAR(Ice, subFlip);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
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
