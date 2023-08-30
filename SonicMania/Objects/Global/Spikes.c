// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Spikes Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSpikes *Spikes;

void Spikes_Update(void)
{
    RSDK_THIS(Spikes);

    switch (self->stateMove) {
        default:
        case SPIKES_MOVE_STATIC: break;

        case SPIKES_MOVE_HIDDEN:
            if (self->stagger << 6 == (Zone->timer & 0x40)) {
                if ((Zone->timer & 0x3F) == self->timer) {
                    self->stateMove++;
                    if (self->onScreen == 1)
                        RSDK.PlaySfx(Spikes->sfxMove, false, 255);
                }
            }
            break;

        case SPIKES_MOVE_APPEAR:
            if (self->moveOffset >= TO_FIXED(32)) {
                self->stateMove++;
            }
            else {
                self->moveOffset += TO_FIXED(8);
                self->position.x += self->velocity.x;
                self->position.y += self->velocity.y;
            }
            break;

        case SPIKES_MOVE_SHOWN:
            if ((Zone->timer & 0x3F) == self->timer) {
                self->stateMove++;
                if (self->onScreen == 1)
                    RSDK.PlaySfx(Spikes->sfxMove, false, 255);
            }
            break;

        case SPIKES_MOVE_DISAPPEAR:
            if (self->moveOffset <= 0) {
                self->stateMove = SPIKES_MOVE_HIDDEN;
            }
            else {
                self->moveOffset -= TO_FIXED(8);
                self->position.x -= self->velocity.x;
                self->position.y -= self->velocity.y;
            }
            break;

        // Used by FBZ/SpiderMobile to hide the spikes on the arena
        case SPIKES_MOVE_DISAPPEAR_FOREVER:
            if (self->moveOffset >= TO_FIXED(40)) {
                self->stateMove = SPIKES_MOVE_HIDDEN_FOREVER;
            }
            else {
                self->moveOffset += TO_FIXED(8);
                self->position.x -= self->velocity.x;
                self->position.y -= self->velocity.y;
            }
            break;

        case SPIKES_MOVE_HIDDEN_FOREVER: break;
    }

    self->position.x -= self->collisionOffset.x;
    self->position.y -= self->collisionOffset.y;

    if (self->stateMove != SPIKES_MOVE_HIDDEN) {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == (((uint8)self->planeFilter - 1) & 1)) {
                EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntitySlot(player), Shield);
                if (!Ice || (player->state != Ice_PlayerState_Frozen && shield->shieldAnimator.animationID != SHIELDANI_FIREATTACK) || Press) {
#if MANIA_USE_PLUS
                    int32 playerVelX = player->velocity.x;
                    int32 playerVelY = player->velocity.y;
#endif

                    uint8 side = Player_CheckCollisionBox(player, self, &self->hitbox);
                    if (side) {
                        if (Ice && player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !Press && !self->shatterTimer) {
                            Ice_ShatterGenerator(8, 16, 16, 0, 0, false);
                            self->shatterTimer = 15;
                        }

                        bool32 shouldShatter = false;
                        if (side == C_BOTTOM) {
                            player->collisionFlagV |= 2;
                            shouldShatter = false;
                        }
                        else {
                            if (side == C_TOP) {
                                player->collisionFlagV |= 1;
                                if (self->moveOffset == TO_FIXED(8))
                                    player->onGround = false;
                            }
                            shouldShatter = false;

#if MANIA_USE_PLUS
                            if (side == C_TOP && player->state == Player_State_MightyHammerDrop) {
                                if (Ice) {
                                    if (!Press) {
                                        shouldShatter = true;
                                        if (self->type != C_TOP) {
                                            player->onGround   = false;
                                            player->velocity.y = playerVelY;
                                        }
                                    }
                                }
                            }
#endif
                        }

#if MANIA_USE_PLUS
                        switch (side) {
                            case C_TOP:
                                player->collisionFlagV |= 1;
                                if (player->velocity.y >= 0 || self->stateMove == SPIKES_MOVE_APPEAR) {
                                    player->position.x += self->collisionOffset.x;
                                    player->position.y += self->collisionOffset.y;
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;

                            case C_LEFT:
                                player->collisionFlagH |= 1;
                                if (player->velocity.x >= 0 || self->stateMove == SPIKES_MOVE_APPEAR) {
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;

                            case C_RIGHT:
                                player->collisionFlagH |= 2;
                                if (player->velocity.x <= 0 || self->stateMove == SPIKES_MOVE_APPEAR) {
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;

                            case C_BOTTOM:
                                player->collisionFlagV |= 2;
                                if (player->velocity.y <= 0 || self->stateMove == SPIKES_MOVE_APPEAR) {
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;

                            default: break;
                        }
#else
                        if (side == self->type) {
                            switch (side) {
                                case C_TOP:
                                    if (!player->velocity.y) {
                                        player->position.x += self->collisionOffset.x;
                                        player->position.y += self->collisionOffset.y;
                                        Player_Hurt(player, self);
                                    }
                                    break;

                                case C_LEFT:
                                    if (!player->velocity.x)
                                        Player_Hurt(player, self);
                                    break;

                                case C_RIGHT:
                                    if (!player->velocity.x)
                                        Player_Hurt(player, self);
                                    break;

                                case C_BOTTOM:
                                    if (!player->velocity.y)
                                        Player_Hurt(player, self);
                                    break;

                                default: break;
                            }
                        }
#endif

                        if (shouldShatter) {
                            RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                            Ice_ShatterGenerator(16, 16, 16, 0, 0, false);
                            destroyEntity(self);
                        }
                    }
                }
                else {
                    int32 storedX = player->position.x;
                    int32 storedY = player->position.y;

                    uint8 side = C_NONE;

                    if (player->state == Ice_PlayerState_Frozen)
                        side = MathHelpers_CheckBoxCollision(self, &self->hitbox, player, &Ice->hitboxPlayerBlockOuter);
                    else
                        side = MathHelpers_CheckBoxCollision(self, &self->hitbox, player, Player_GetHitbox(player));

                    switch (side) {
                        case C_TOP:
                            if (player->velocity.y < TO_FIXED(4)) {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->collisionOffset.x;
                                    player->position.y += self->collisionOffset.y;
                                }
                            }
                            else {
                                Spikes_Shatter(0, 0);
                                player->position.x = storedX;
                                player->position.y = storedY;
                                foreach_return;
                            }
                            break;

                        case C_LEFT:
                            if (player->velocity.x >= TO_FIXED(2)) {
                                Spikes_Shatter(player->velocity.x, 0);
                                player->position.x = storedX;
                                player->position.y = storedY;
                                foreach_return;
                            }
                            else {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->collisionOffset.x;
                                    player->position.y += self->collisionOffset.y;
                                }
                            }
                            break;

                        case C_RIGHT:
                            if (player->velocity.x > -TO_FIXED(2)) {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->collisionOffset.x;
                                    player->position.y += self->collisionOffset.y;
                                }
                            }
                            else {
                                Spikes_Shatter(player->velocity.x, 0);
                                player->position.x = storedX;
                                player->position.y = storedY;
                                foreach_return;
                            }
                            break;

                        case C_BOTTOM:
                            if (player->velocity.y > -TO_FIXED(4)) {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->collisionOffset.x;
                                    player->position.y += self->collisionOffset.y;
                                }
                            }
                            else {
                                Spikes_Shatter(0, player->velocity.y);
                                player->position.x = storedX;
                                player->position.y = storedY;
                                foreach_return;
                            }

                        default:
                        case C_NONE:
                            if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                player->position.x += self->collisionOffset.x;
                                player->position.y += self->collisionOffset.y;
                            }
                            break;
                    }
                }
            }
        }
    }

    self->position.x += self->collisionOffset.x;
    self->position.y += self->collisionOffset.y;

    // if the Ice object is in the scene & the Press object isn't (aka if the stage is PGZ2)
    if (Ice && !Press) {
        RSDK.ProcessAnimation(&self->animator);

        if (--self->glintTimer <= 0) {
            switch (self->type) {
                case C_TOP:
                case C_BOTTOM: RSDK.SetSpriteAnimation(Spikes->aniFrames, 2, &self->animator, true, 0); break;
                case C_LEFT:
                case C_RIGHT: RSDK.SetSpriteAnimation(Spikes->aniFrames, 3, &self->animator, true, 0); break;
                default: break;
            }

            self->glintTimer = RSDK.Rand(0, 240) + 30;
        }

        if (self->shatterTimer > 0) {
            if (!--self->shatterTimer)
                Spikes_Shatter(0, 0);
        }
    }
}

void Spikes_LateUpdate(void) {}

void Spikes_StaticUpdate(void)
{
    RSDK.ProcessAnimation(&Spikes->verticalAnimator);
    RSDK.ProcessAnimation(&Spikes->horizontalAnimator);
}

void Spikes_Draw(void) { StateMachine_Run(Spikes->stateDraw); }

void Spikes_Create(void *data)
{
    RSDK_THIS(Spikes);

    self->drawFX = FX_FLIP;
    if (self->count < 2)
        self->count = 2;

    if (!SceneInfo->inEditor) {
        if (data)
            self->type = VOID_TO_INT(data);

        self->active  = ACTIVE_BOUNDS;
        int32 dir     = self->type & 1;
        self->visible = true;
        self->type    = (self->type >> 1) & 1;
        if (self->planeFilter > 0 && ((uint8)self->planeFilter - 1) & 2)
            self->drawGroup = Zone->objectDrawGroup[1];
        else
            self->drawGroup = Zone->objectDrawGroup[0];
        self->alpha = 0x80;

        switch (self->type) {
            case 0: // vertical
                self->updateRange.x = (self->count + 6) << 20;
                self->updateRange.y = TO_FIXED(96);
                self->direction     = FLIP_Y * dir;
                if (self->direction) {
                    self->velocity.y = TO_FIXED(8);
                    self->type       = C_BOTTOM;
                }
                else {
                    self->velocity.y = -TO_FIXED(8);
                    self->type       = C_TOP;
                }
                self->hitbox.left   = -8 * self->count;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8 * self->count;
                self->hitbox.bottom = 16;
                break;

            case 1: // horizontal
                self->updateRange.x = TO_FIXED(96);
                self->updateRange.y = (self->count + 6) << 20;
                self->direction     = dir;
                if (self->direction) {
                    self->velocity.x = -TO_FIXED(8);
                    self->type       = C_LEFT;
                }
                else {
                    self->velocity.x = TO_FIXED(8);
                    self->type       = C_RIGHT;
                }
                self->hitbox.left   = -16;
                self->hitbox.top    = -8 * self->count;
                self->hitbox.right  = 16;
                self->hitbox.bottom = 8 * self->count;
                break;
        }

        if (self->moving) {
            self->position.x -= 4 * self->velocity.x;
            self->position.y -= 4 * self->velocity.y;
            self->stateMove = SPIKES_MOVE_HIDDEN;
        }
    }
}

void Spikes_StageLoad(void)
{
    Spikes->stateDraw = Spikes_Draw_Global;

    if (RSDK.CheckSceneFolder("FBZ")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Spikes.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckSceneFolder("PSZ2")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Spikes.bin", SCOPE_STAGE);
        Spikes->stateDraw = Spikes_Draw_Glint;
    }
    else {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("Global/Spikes.bin", SCOPE_STAGE);
    }

    RSDK.SetSpriteAnimation(Spikes->aniFrames, 0, &Spikes->verticalAnimator, true, 0);
    RSDK.SetSpriteAnimation(Spikes->aniFrames, 1, &Spikes->horizontalAnimator, true, 0);

    Spikes->unused1 = TO_FIXED(16);
    Spikes->unused2 = TO_FIXED(16);

    Spikes->sfxMove  = RSDK.GetSfx("Global/SpikesMove.wav");
    Spikes->sfxSpike = RSDK.GetSfx("Global/Spike.wav");
}

void Spikes_Draw_Glint(void)
{
    RSDK_THIS(Spikes);

    Vector2 drawPos = self->position;
    int32 cnt       = self->count >> 1;
    switch (self->type) {
        case C_TOP:
        case C_BOTTOM:
            drawPos.x = TO_FIXED(16) - (self->count << 19) + self->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
                drawPos.x += TO_FIXED(32);
            }

            if (self->count & 1) {
                drawPos.x -= TO_FIXED(16);
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
            }
            break;

        case C_LEFT:
        case C_RIGHT:
            drawPos.y = TO_FIXED(16) - (self->count << 19) + self->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
                drawPos.y += TO_FIXED(32);
            }

            if (self->count & 1) {
                drawPos.y -= TO_FIXED(16);
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
            }
            break;

        default: break;
    }

    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Spikes_Draw_Global(void)
{
    RSDK_THIS(Spikes);

    Vector2 drawPos = self->position;
    int32 cnt       = self->count >> 1;

    switch (self->type) {
        case C_TOP:
        case C_BOTTOM:
            drawPos.x = (TO_FIXED(16) - (self->count << 19)) + self->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
                drawPos.x += TO_FIXED(32);
            }

            if (self->count & 1) {
                drawPos.x -= TO_FIXED(16);
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
            }
            break;

        case C_LEFT:
        case C_RIGHT:
            drawPos.y = (TO_FIXED(16) - (self->count << 19)) + self->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
                drawPos.y += TO_FIXED(32);
            }

            if (self->count & 1) {
                drawPos.y -= TO_FIXED(16);
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
            }
            break;

        default: break;
    }
}

void Spikes_Shatter(int32 velX, int32 velY)
{
    RSDK_THIS(Spikes);

    RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
    Ice_ShatterGenerator(16, 16, 16, velX, velY, false);
    destroyEntity(self);
}

#if MANIA_USE_PLUS
void Spikes_CheckHit(EntityPlayer *player, int32 playerVelX, int32 playerVelY)
{
    RSDK_THIS(Spikes);

    if (player->state == Player_State_Hurt)
        return;
    if (!Player_CheckValidState(player) || player->invincibleTimer || player->blinkTimer > 0)
        return;

    if (player->characterID == ID_MIGHTY
        && (player->animator.animationID == ANI_JUMP || player->animator.animationID == ANI_SPINDASH
            || player->animator.animationID == ANI_HAMMERDROP)) {

        if (abs(playerVelX) < 0x20000) {
            switch (self->type) {
                default: break;
                case C_TOP:
                    if (player->animator.animationID != ANI_HAMMERDROP || (Ice && !Press)) {
                        player->velocity.y = -0x48000;
                        if (!(player->direction & FLIP_X))
                            player->velocity.x = 0x48000;
                        else
                            player->velocity.x = -0x48000;
                        player->state = Player_State_Air;
                    }
                    else {
                        player->velocity.y = -0x48000;
                        if (!(player->direction & FLIP_X))
                            player->velocity.x = -0x28000;
                        else
                            player->velocity.x = 0x28000;
                        player->blinkTimer = 60;
                        player->state      = Player_State_Hurt;
                        RSDK.StopSfx(Player->sfxMightyDrill);
                    }

                    player->velocity.x -= player->velocity.x >> 2;
                    break;

                case C_LEFT:
                    player->velocity.y = -0x40000;
                    player->velocity.x = -0x28000;
                    player->state      = Player_State_Air;
                    break;

                case C_RIGHT:
                    player->velocity.y = -0x40000;
                    player->velocity.x = 0x28000;
                    player->state      = Player_State_Air;
                    break;

                case C_BOTTOM:
                    player->velocity.y = 0x20000;
                    player->state      = Player_State_Air;
                    break;
            }

            player->onGround         = false;
            player->applyJumpCap     = false;
            player->jumpAbilityState = 0;
            if (player->state == Player_State_Hurt) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
            }
            else {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_UNSPIN, &player->animator, false, 0);
                RSDK.PlaySfx(Player->sfxMightyUnspin, false, 255);
            }

            if (player->animator.animationID != ANI_UNSPIN)
                RSDK.PlaySfx(Player->sfxPimPom, false, 255);

            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y >>= 1;
            }
        }
        else if (self->type == C_TOP) {
            if (player->animator.animationID == ANI_HAMMERDROP) {
                player->velocity.y = -0x48000;
                if (!(player->direction & FLIP_X))
                    player->velocity.x = 0x48000;
                else
                    player->velocity.x = -0x48000;

                player->state = Player_State_Air;
                player->velocity.x -= player->velocity.x >> 2;
                player->onGround         = false;
                player->applyJumpCap     = false;
                player->jumpAbilityState = 0;

                if (player->state == Player_State_Hurt) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                    RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_UNSPIN, &player->animator, false, 0);
                    RSDK.PlaySfx(Player->sfxMightyUnspin, false, 255);
                }

                if (player->animator.animationID != ANI_UNSPIN)
                    RSDK.PlaySfx(Player->sfxPimPom, false, 255);

                if (player->underwater) {
                    player->velocity.x >>= 1;
                    player->velocity.y >>= 1;
                }
            }
            else {
                if (playerVelY > 0x28000) {
                    player->velocity.y       = -0x20000;
                    player->state            = Player_State_Air;
                    player->onGround         = false;
                    player->applyJumpCap     = false;
                    player->jumpAbilityState = 0;
                    RSDK.PlaySfx(Player->sfxPimPom, false, 255);
                }

                player->groundedStore   = true;
                player->state           = Player_State_Roll;
                player->nextGroundState = Player_State_Roll;
                player->nextAirState    = Player_State_Air;
            }
        }
        return; // dont do the code below
    }

    if (player->position.x > self->position.x)
        player->velocity.x = 0x20000;
    else
        player->velocity.x = -0x20000;

    Player_Hit(player);

    if (player->deathType == PLAYER_DEATH_DIE_USESFX) {
        player->deathType = PLAYER_DEATH_DIE_NOSFX;
        RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
    }
    else if (player->state == Player_State_Hurt && (player->shield || player->sidekick)) {
        RSDK.StopSfx(Player->sfxHurt);
        RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void Spikes_EditorDraw(void)
{
    RSDK_THIS(Spikes);

    if (self->count < 2)
        self->count = 2;

    int32 dir  = self->type & 1;
    int32 type = 0;
    switch ((self->type >> 1) & 1) {
        case 0:
            self->direction = FLIP_Y * dir;
            if (self->direction)
                type = C_BOTTOM;
            else
                type = C_TOP;
            break;

        case 1:
            self->direction = dir;
            if (self->direction)
                type = C_LEFT;
            else
                type = C_RIGHT;
            break;
    }

    Vector2 drawPos = self->position;
    int32 cnt       = self->count >> 1;

    switch (type) {
        case C_TOP:
        case C_BOTTOM:
            drawPos.x = TO_FIXED(16) - (self->count << 19) + self->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
                drawPos.x += TO_FIXED(32);
            }

            if (self->count & 1) {
                drawPos.x -= TO_FIXED(16);
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
            }
            break;

        case C_LEFT:
        case C_RIGHT:
            drawPos.y = TO_FIXED(16) - (self->count << 19) + self->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
                drawPos.y += TO_FIXED(32);
            }

            if (self->count & 1) {
                drawPos.y -= TO_FIXED(16);
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
            }
            break;

        default: break;
    }
}

void Spikes_EditorLoad(void)
{
    Spikes->stateDraw = Spikes_Draw_Global;

    if (RSDK.CheckSceneFolder("FBZ")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Spikes.bin", SCOPE_STAGE);
    }
    if (RSDK.CheckSceneFolder("PSZ2")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Spikes.bin", SCOPE_STAGE);
        Spikes->stateDraw = Spikes_Draw_Glint;
    }
    else {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("Global/Spikes.bin", SCOPE_STAGE);
    }

    RSDK.SetSpriteAnimation(Spikes->aniFrames, 0, &Spikes->verticalAnimator, true, 0);
    RSDK.SetSpriteAnimation(Spikes->aniFrames, 1, &Spikes->horizontalAnimator, true, 0);

    RSDK_ACTIVE_VAR(Spikes, type);
    RSDK_ENUM_VAR("Up", SPIKES_UP);
    RSDK_ENUM_VAR("Down", SPIKES_DOWN);
    RSDK_ENUM_VAR("Left", SPIKES_LEFT);
    RSDK_ENUM_VAR("Right", SPIKES_RIGHT);

    RSDK_ACTIVE_VAR(Spikes, planeFilter);
    RSDK_ENUM_VAR("None", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("AL", PLANEFILTER_AL);
    RSDK_ENUM_VAR("BL", PLANEFILTER_BL);
    RSDK_ENUM_VAR("AH", PLANEFILTER_AH);
    RSDK_ENUM_VAR("BH", PLANEFILTER_BH);
}
#endif

void Spikes_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spikes, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(Spikes, VAR_BOOL, moving);
    RSDK_EDITABLE_VAR(Spikes, VAR_UINT8, count);
    RSDK_EDITABLE_VAR(Spikes, VAR_UINT8, stagger);
    RSDK_EDITABLE_VAR(Spikes, VAR_UINT8, timer);
    RSDK_EDITABLE_VAR(Spikes, VAR_ENUM, planeFilter);
}
