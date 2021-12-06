#include "SonicMania.h"

ObjectSpikes *Spikes;

void Spikes_Update(void)
{
    RSDK_THIS(Spikes);
    switch (self->stateMove) {
        case 1:
            if (self->stagger << 6 == (Zone->timer & 0x40)) {
                if ((Zone->timer & 0x3F) == self->timer) {
                    self->stateMove++;
                    if (self->activeScreens == 1)
                        RSDK.PlaySfx(Spikes->sfxMove, false, 255);
                }
            }
            break;
        case 2:
            if (self->moveOffset >= 0x200000) {
                self->stateMove++;
            }
            else {
                self->moveOffset += 0x80000;
                self->position.x += self->velocity.x;
                self->position.y += self->velocity.y;
            }
            break;
        case 3:
            if ((Zone->timer & 0x3F) == self->timer) {
                self->stateMove++;
                if (self->activeScreens == 1)
                    RSDK.PlaySfx(Spikes->sfxMove, false, 255);
            }
            break;
        case 4:
            if (self->moveOffset <= 0) {
                self->stateMove = 1;
            }
            else {
                self->moveOffset -= 0x80000;
                self->position.x -= self->velocity.x;
                self->position.y -= self->velocity.y;
            }
            break;
        case 5:
            if (self->moveOffset >= 0x280000) {
                self->stateMove = 6;
            }
            else {
                self->moveOffset += 0x80000;
                self->position.x -= self->velocity.x;
                self->position.y -= self->velocity.y;
            }
            break;
        default: break;
    }

    self->position.x -= self->offset.x;
    self->position.y -= self->offset.y;
    if (self->stateMove != 1) {
        foreach_active(Player, player)
        {
            if (self->planeFilter <= 0 || player->collisionPlane == (((uint8)self->planeFilter - 1) & 1)) {
                EntityShield *shield = (EntityShield *)RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player));
                if (!Ice || (player->state != Ice_State_FrozenPlayer && shield->animator.animationID != 2) || Press) {
                    int32 playerVelX = player->velocity.x;
                    int32 playerVelY = player->velocity.y;

                    uint8 side = Player_CheckCollisionBox(player, self, &self->hitbox);
                    if (side) {
                        if (Ice && player->shield == SHIELD_FIRE && player->invincibleTimer <= 0 && !Press && !self->shatterTimer) {
                            Ice_ShatterGenerator(16, 16, 8, 0, 0, false);
                            self->shatterTimer = 15;
                        }

                        bool32 flag = false;
                        if (side == C_BOTTOM) {
                            player->collisionFlagV |= 2;
                            flag = false;
                        }
                        else {
                            if (side == C_TOP) {
                                player->collisionFlagV |= 1;
                                if (self->moveOffset == 0x80000)
                                    player->onGround = false;
                            }
                            flag = false;
#if RETRO_USE_PLUS
                            if (side == C_TOP && player->state == Player_State_MightyHammerDrop) {
                                if (Ice) {
                                    if (!Press) {
                                        flag = true;
                                        if (self->type != C_TOP) {
                                            player->onGround   = false;
                                            player->velocity.y = playerVelY;
                                        }
                                    }
                                }
                            }
#endif
                        }

                        switch (side) {
                            case C_TOP:
                                player->collisionFlagV |= 1;
                                if (player->velocity.y >= 0 || self->stateMove == 2) {
                                    player->position.x += self->offset.x;
                                    player->position.y += self->offset.y;
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            case C_LEFT:
                                player->collisionFlagH |= 1;
                                if (player->velocity.x >= 0 || self->stateMove == 2) {
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            case C_RIGHT:
                                player->collisionFlagH |= 2;
                                if (player->velocity.x <= 0 || self->stateMove == 2) {
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            case C_BOTTOM:
                                player->collisionFlagV |= 2;
                                if (player->velocity.y <= 0 || self->stateMove == 2) {
                                    if (side == self->type)
                                        Spikes_CheckHit(player, playerVelX, playerVelY);
                                }
                                break;
                            default: break;
                        }

                        if (flag) {
                            RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                            Ice_ShatterGenerator(16, 16, 16, 0, 0, false);
                            destroyEntity(self);
                        }
                    }
                }
                else {
                    int32 posStoreX = player->position.x;
                    int32 posStoreY = player->position.y;

                    uint8 side = C_NONE;
                    if (player->state == Ice_State_FrozenPlayer) {
                        side = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, &Ice->hitboxPlayerBlockOuter, false);
                    }
                    else {
                        side = RSDK.CheckObjectCollisionBox(self, &self->hitbox, player, Player_GetHitbox(player), false);
                    }

                    switch (side) {
                        case C_TOP:
                            if (player->velocity.y < 0x40000) {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->offset.x;
                                    player->position.y += self->offset.y;
                                }
                            }
                            else {
                                Spikes_Shatter(0, 0);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                            break;
                        case C_LEFT:
                            if (player->velocity.x >= 0x20000) {
                                Spikes_Shatter(player->velocity.x, 0);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                            else {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->offset.x;
                                    player->position.y += self->offset.y;
                                }
                            }
                            break;
                        case C_RIGHT:
                            if (player->velocity.x > -0x20000) {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->offset.x;
                                    player->position.y += self->offset.y;
                                }
                            }
                            else {
                                Spikes_Shatter(player->velocity.x, 0);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                            break;
                        case C_BOTTOM:
                            if (player->velocity.y > -0x40000) {
                                if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                    player->position.x += self->offset.x;
                                    player->position.y += self->offset.y;
                                }
                            }
                            else {
                                Spikes_Shatter(0, player->velocity.y);
                                player->position.x = posStoreX;
                                player->position.y = posStoreY;
                                foreach_return;
                            }
                        default:
                            if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {
                                player->position.x += self->offset.x;
                                player->position.y += self->offset.y;
                            }
                            break;
                    }
                }
            }
        }
    }

    self->position.x += self->offset.x;
    self->position.y += self->offset.y;
    if (Ice && !Press) {
        RSDK.ProcessAnimation(&self->animator);
        if (--self->timer2 <= 0) {
            switch (self->type) {
                case C_TOP:
                case C_BOTTOM: RSDK.SetSpriteAnimation(Spikes->aniFrames, 2, &self->animator, true, 0); break;
                case C_LEFT:
                case C_RIGHT: RSDK.SetSpriteAnimation(Spikes->aniFrames, 3, &self->animator, true, 0); break;
                default: break;
            }
            self->timer2 = RSDK.Rand(0, 240) + 30;
        }
        if (self->shatterTimer > 0) {
            self->shatterTimer--;
            if (!self->shatterTimer)
                Spikes_Shatter(0, 0);
        }
    }
}

void Spikes_LateUpdate(void) {}

void Spikes_StaticUpdate(void) {}

void Spikes_Draw(void) { StateMachine_Run(Spikes->stateDraw); }

void Spikes_Create(void *data)
{
    RSDK_THIS(Spikes);
    self->drawFX = FX_FLIP;
    if (self->count < 2)
        self->count = 2;
    if (!SceneInfo->inEditor) {
        if (data)
            self->type = voidToInt(data);

        self->active  = ACTIVE_BOUNDS;
        int32 dir         = self->type & 1;
        self->visible = true;
        self->type    = (self->type >> 1) & 1;
        if (self->planeFilter > 0 && ((uint8)self->planeFilter - 1) & 2)
            self->drawOrder = Zone->drawOrderHigh;
        else
            self->drawOrder = Zone->drawOrderLow;
        self->alpha = 128;

        switch (self->type) {
            case 0: //vertical
                self->updateRange.x = (self->count + 6) << 20;
                self->updateRange.y = 0x600000;
                self->direction     = FLIP_Y * dir;
                if (self->direction) {
                    self->velocity.y = 0x80000;
                    self->type       = C_BOTTOM;
                }
                else {
                    self->velocity.y = -0x80000;
                    self->type       = C_TOP;
                }
                self->hitbox.left   = -8 * self->count;
                self->hitbox.top    = -16;
                self->hitbox.right  = 8 * self->count;
                self->hitbox.bottom = 16;
                break;
            case 1: //horizontal
                self->updateRange.x = 0x600000;
                self->updateRange.y = (self->count + 6) << 20;
                self->direction     = dir;
                if (self->direction) {
                    self->velocity.x = -0x80000;
                    self->type       = C_LEFT;
                }
                else {
                    self->velocity.x = 0x80000;
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
            self->stateMove = 1;
        }
    }
}

void Spikes_StageLoad(void)
{
    Spikes->stateDraw = Spikes_Draw_Stage;
    if (RSDK.CheckStageFolder("FBZ")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Spikes.bin", SCOPE_STAGE);
    }
    if (RSDK.CheckStageFolder("PSZ2")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Spikes.bin", SCOPE_STAGE);
    }
    else {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("Global/Spikes.bin", SCOPE_STAGE);
        Spikes->stateDraw   = Spikes_Draw_Global;
    }
    RSDK.SetSpriteAnimation(Spikes->aniFrames, 0, &Spikes->verticalAnimator, true, 0);
    RSDK.SetSpriteAnimation(Spikes->aniFrames, 1, &Spikes->horizontalAnimator, true, 0);
    Spikes->unused1   = 0x100000;
    Spikes->unused2   = 0x100000;
    Spikes->sfxMove  = RSDK.GetSfx("Global/SpikesMove.wav");
    Spikes->sfxSpike = RSDK.GetSfx("Global/Spike.wav");
}

void Spikes_Draw_Global(void)
{
    Vector2 drawPos;

    RSDK_THIS(Spikes);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    int32 cnt   = self->count >> 1;
    switch (self->type) {
        case 1:
        case 4:
            drawPos.x = 0x100000 - (self->count << 19) + self->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
                drawPos.x += 0x200000;
            }

            if (self->count & 1) {
                drawPos.x -= 0x100000;
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
            }
            break;
        case 2:
        case 3:
            drawPos.y = 0x100000 - (self->count << 19) + self->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
                drawPos.y += 0x200000;
            }

            if (self->count & 1) {
                drawPos.y -= 0x100000;
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
            }
            break;
        default: break;
    }
    self->inkEffect = INK_ADD;
    RSDK.DrawSprite(&self->animator, NULL, false);

    self->inkEffect = INK_NONE;
}

void Spikes_Draw_Stage(void)
{
    Vector2 drawPos;

    RSDK_THIS(Spikes);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    int32 cnt   = self->count >> 1;
    switch (self->type) {
        case 1:
        case 4:
            drawPos.x = (0x100000 - (self->count << 19)) + self->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
                drawPos.x += 0x200000;
            }
            if (self->count & 1) {
                drawPos.x -= 0x100000;
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
            }
            break;
        case 2:
        case 3:
            drawPos.y = (0x100000 - (self->count << 19)) + self->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
                drawPos.y += 0x200000;
            }
            if (self->count & 1) {
                drawPos.y -= 0x100000;
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
void Spikes_CheckHit(EntityPlayer *player, int32 playerVelX, int32 playerVelY)
{
    RSDK_THIS(Spikes);
    if (player->state == Player_State_Hit)
        return;
    if (!Player_CheckValidState(player) || player->invincibleTimer || player->blinkTimer > 0)
        return;

#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY
        && (player->animator.animationID == ANI_JUMP || player->animator.animationID == ANI_SPINDASH
            || player->animator.animationID == ANI_DROPDASH)) {

        if (abs(playerVelX) < 0x20000) {
            switch (self->type) {
                case 1:
                    if (player->animator.animationID != ANI_DROPDASH || (Ice && !Press)) {
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
                        player->state      = Player_State_Hit;
                        RSDK.StopSfx(Player->sfxMightyDrill);
                    }

                    player->velocity.x -= player->velocity.x >> 2;
                    break;
                case 2:
                    player->velocity.y = -0x40000;
                    player->velocity.x = -0x28000;
                    player->state      = Player_State_Air;
                    break;
                case 3:
                    player->velocity.y = -0x40000;
                    player->velocity.x = 0x28000;
                    player->state      = Player_State_Air;
                    break;
                case 4:
                    player->velocity.y = 0x20000;
                    player->state      = Player_State_Air;
                    break;
                default: break;
            }

            player->onGround         = false;
            player->jumpAbility      = 0;
            player->jumpAbilityTimer = 0;
            if (player->state == Player_State_Hit) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
            }
            else {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLY, &player->animator, false, 0);
                RSDK.PlaySfx(Player->sfxMightyUnspin, false, 255);
            }
            if (player->animator.animationID != ANI_FLY)
                RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            if (player->underwater) {
                player->velocity.x >>= 1;
                player->velocity.y >>= 1;
            }
        }
        else if (self->type == 1) {
            if (player->animator.animationID == ANI_DROPDASH) {
                player->velocity.y = -0x48000;
                if (!(player->direction & FLIP_X))
                    player->velocity.x = 0x48000;
                else
                    player->velocity.x = -0x48000;
                player->state = Player_State_Air;
                player->velocity.x -= player->velocity.x >> 2;
                player->onGround         = false;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                if (player->state == Player_State_Hit) {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
                    RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
                }
                else {
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FLY, &player->animator, false, 0);
                    RSDK.PlaySfx(Player->sfxMightyUnspin, false, 255);
                }
                if (player->animator.animationID != ANI_FLY)
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
                    player->jumpAbility      = 0;
                    player->jumpAbilityTimer = 0;
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
#endif

    if (player->position.x > self->position.x)
        player->velocity.x = 0x20000;
    else
        player->velocity.x = -0x20000;
    Player_Hit(player);
    if (player->deathType == PLAYER_DEATH_DIE_USESFX) {
        player->deathType = PLAYER_DEATH_DIE_NOSFX;
        RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
    }
    else if (player->state == Player_State_Hit && (player->shield || player->sidekick)) {
        RSDK.StopSfx(Player->sfxHurt);
        RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
    }
}

#if RETRO_INCLUDE_EDITOR
void Spikes_EditorDraw(void)
{
    RSDK_THIS(Spikes);

    int32 dir = self->type & 1;
    int32 type = 0;
    switch ((self->type >> 1) & 1) {
        case 0:
            self->direction = FLIP_Y * dir;
            if (self->direction)
                type = 4;
            else 
                type = 1;
            break;
        case 1:
            self->direction = dir;
            if (self->direction) 
                type = 2;
            else 
                type = 3;
            break;
    }

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    int32 cnt = self->count >> 1;
    switch (type) {
        case 1:
        case 4:
            drawPos.x = 0x100000 - (self->count << 19) + self->position.x;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
                drawPos.x += 0x200000;
            }

            if (self->count & 1) {
                drawPos.x -= 0x100000;
                RSDK.DrawSprite(&Spikes->verticalAnimator, &drawPos, false);
            }
            break;
        case 2:
        case 3:
            drawPos.y = 0x100000 - (self->count << 19) + self->position.y;
            for (; cnt; --cnt) {
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
                drawPos.y += 0x200000;
            }

            if (self->count & 1) {
                drawPos.y -= 0x100000;
                RSDK.DrawSprite(&Spikes->horizontalAnimator, &drawPos, false);
            }
            break;
        default: break;
    }

}

void Spikes_EditorLoad(void)
{
    Spikes->stateDraw = Spikes_Draw_Stage;
    if (RSDK.CheckStageFolder("FBZ")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Spikes.bin", SCOPE_STAGE);
    }
    if (RSDK.CheckStageFolder("PSZ2")) {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("PSZ2/Spikes.bin", SCOPE_STAGE);
    }
    else {
        Spikes->aniFrames = RSDK.LoadSpriteAnimation("Global/Spikes.bin", SCOPE_STAGE);
        Spikes->stateDraw   = Spikes_Draw_Global;
    }
    RSDK.SetSpriteAnimation(Spikes->aniFrames, 0, &Spikes->verticalAnimator, true, 0);
    RSDK.SetSpriteAnimation(Spikes->aniFrames, 1, &Spikes->horizontalAnimator, true, 0);

    RSDK_ACTIVE_VAR(Spikes, type);
    RSDK_ENUM_VAR("Up", SPIKES_UP);
    RSDK_ENUM_VAR("Down", SPIKES_DOWN);
    RSDK_ENUM_VAR("Left", SPIKES_LEFT);
    RSDK_ENUM_VAR("Right", SPIKES_RIGHT);

    RSDK_ACTIVE_VAR(Spikes, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);
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
