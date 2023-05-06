// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Shiversaw Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectShiversaw *Shiversaw;

void Shiversaw_Update(void)
{
    RSDK_THIS(Shiversaw);

    StateMachine_Run(self->state);
}

void Shiversaw_LateUpdate(void) {}

void Shiversaw_StaticUpdate(void)
{
    if (Shiversaw->timer > 0)
        Shiversaw->timer--;
}

void Shiversaw_Draw(void)
{
    RSDK_THIS(Shiversaw);
    Vector2 drawPos;

    self->sawID = SHIVERSAW_SAW_COUNT;
    for (int32 s = 0; s < SHIVERSAW_SAW_COUNT; ++s) {
        --self->sawID;
        self->direction = self->sawID;

        RSDK.DrawSprite(&self->sawAnimator[self->sawID], &self->sawPos[self->sawID], false);

        self->rotation = self->sawAngles[self->sawID];
        if (self->sawID & 1)
            drawPos.x = self->position.x - 0x210000;
        else
            drawPos.x = self->position.x + 0x210000;
        drawPos.y = self->position.y + 0x70000;

        if (self->sawRadius[self->sawID] >> 19 < 0) {
            drawPos.x -= ((-self->sawRadius[self->sawID] >> 9) & 0x380) * RSDK.Cos512(self->rotation + (self->sawID << 8));
            drawPos.y -= ((-self->sawRadius[self->sawID] >> 9) & 0x380) * RSDK.Sin512(self->rotation + (self->sawID << 8));

            int32 cnt = ~(self->sawRadius[self->sawID] >> 19);
            for (int32 i = cnt; i > 0; --i) {
                drawPos.x += -0x400 * RSDK.Cos512(self->rotation + (self->sawID << 8));
                drawPos.y += -0x400 * RSDK.Sin512(self->rotation + (self->sawID << 8));
            }
        }
        else {
            drawPos.x += ((self->sawRadius[self->sawID] >> 9) & 0x380) * RSDK.Cos512(self->rotation + (self->sawID << 8));
            drawPos.y += ((self->sawRadius[self->sawID] >> 9) & 0x380) * RSDK.Sin512(self->rotation + (self->sawID << 8));
            RSDK.DrawSprite(&self->armAnimator, &drawPos, false);

            int32 cnt = (self->sawRadius[self->sawID] >> 19) - 1;
            for (int32 i = cnt; i >= 0; --i) {
                drawPos.x += RSDK.Cos512(self->rotation + (self->sawID << 8)) << 10;
                drawPos.y += RSDK.Sin512(self->rotation + (self->sawID << 8)) << 10;
                RSDK.DrawSprite(&self->armAnimator, &drawPos, false);
            }
        }

        drawPos.x += RSDK.Cos512(self->rotation + (self->sawID << 8)) << 10;
        drawPos.y += RSDK.Sin512(self->rotation + (self->sawID << 8)) << 10;
        RSDK.DrawSprite(&self->spinnerAnimator, &drawPos, false);

        if (self->sawID == FLIP_X)
            drawPos.x = self->position.x - 0x210000;
        else
            drawPos.x = self->position.x + 0x210000;
        drawPos.y = self->position.y + 0x70000;
        RSDK.DrawSprite(&self->connectorAnimator, &drawPos, false);
    }

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->tanksAnimator, NULL, false);

    if (Shiversaw->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->bellowsAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
    if (self->circleRadius > 0)
        RSDK.DrawCircle(self->position.x, self->position.y, self->circleRadius, 0xFFFFFF, (self->timer << 8) / 120, INK_ALPHA, false);
}

void Shiversaw_Create(void *data)
{
    RSDK_THIS(Shiversaw);

    self->drawFX = FX_ROTATE | FX_FLIP;

    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active    = ACTIVE_BOUNDS;
            self->visible   = false;
            self->drawGroup = Zone->objectDrawGroup[0];

            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 0, &self->tanksAnimator, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 1, &self->bellowsAnimator, true, 7);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 4, &self->connectorAnimator, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 5, &self->armAnimator, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 6, &self->spinnerAnimator, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[0], true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[1], true, 0);

            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            for (int32 s = 0; s < SHIVERSAW_SAW_COUNT; ++s) {
                self->stateSaw[s]  = Shiversaw_StateSaw_Setup;
                self->sawAngles[s] = 0x80 + ((s & 1) != 0) * 0x100;
            }

            Shiversaw_SetupSawPos();

            Shiversaw->timer = 480;
            self->startY     = self->position.y;
            self->state      = Shiversaw_State_Entry;
        }
        else {
            destroyEntity(self);
        }
    }
}

void Shiversaw_StageLoad(void)
{
    Shiversaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Shiversaw.bin", SCOPE_STAGE);

    Shiversaw->hitboxBoss.left   = -16;
    Shiversaw->hitboxBoss.top    = -22;
    Shiversaw->hitboxBoss.bottom = 22;
    Shiversaw->hitboxBoss.right  = 16;

    Shiversaw->hitboxSaw.left   = -16;
    Shiversaw->hitboxSaw.top    = -16;
    Shiversaw->hitboxSaw.right  = 16;
    Shiversaw->hitboxSaw.bottom = 16;

    Shiversaw->hitboxR.top    = -48;
    Shiversaw->hitboxR.left   = -44;
    Shiversaw->hitboxR.right  = 44;
    Shiversaw->hitboxR.bottom = -38;

    Shiversaw->hitboxB.left   = -44;
    Shiversaw->hitboxB.top    = 52;
    Shiversaw->hitboxB.right  = 44;
    Shiversaw->hitboxB.bottom = 62;

    Shiversaw->hitboxL.left   = 48;
    Shiversaw->hitboxL.top    = -36;
    Shiversaw->hitboxL.right  = 56;
    Shiversaw->hitboxL.bottom = 54;

    Shiversaw->hitboxT.left   = -56;
    Shiversaw->hitboxT.top    = -36;
    Shiversaw->hitboxT.right  = -48;
    Shiversaw->hitboxT.bottom = 54;

#if MANIA_USE_PLUS
    Shiversaw->hitboxEntryTrigger.left   = -192;
    Shiversaw->hitboxEntryTrigger.top    = -176;
    Shiversaw->hitboxEntryTrigger.right  = -144;
    Shiversaw->hitboxEntryTrigger.bottom = 112;

    Shiversaw->hitboxEntryBounds.left   = -64;
    Shiversaw->hitboxEntryBounds.top    = -176;
    Shiversaw->hitboxEntryBounds.right  = 0;
    Shiversaw->hitboxEntryBounds.bottom = 112;
#endif

    Shiversaw->health             = 6;
    Shiversaw->invincibilityTimer = 0;

    Shiversaw->sfxHit        = RSDK.GetSfx("Stage/BossHit.wav");
    Shiversaw->sfxExplosion2 = RSDK.GetSfx("Stage/Explosion2.wav");
    Shiversaw->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");
    Shiversaw->sfxTargeting  = RSDK.GetSfx("Stage/Targeting1.wav");
    Shiversaw->sfxRocketJet  = RSDK.GetSfx("Stage/RocketJet.wav");
}

bool32 Shiversaw_CheckSawHit(EntityPlayer *player, int32 sawID)
{
    RSDK_THIS(Shiversaw);

#if MANIA_USE_PLUS
    if (player->characterID == ID_MIGHTY) {
        int32 anim = player->animator.animationID;
        if (anim != ANI_JUMP && anim != ANI_SPINDASH && anim != ANI_HAMMERDROP)
            return Player_Hurt(player, &self->sawPos[sawID]);
        if (anim != ANI_HAMMERDROP)
            return Player_CheckMightyUnspin(player, 768, true, &player->uncurlTimer);

        if (!(player->direction & FLIP_X))
            player->velocity.x = -0x18000;
        else
            player->velocity.x = 0x18000;
        player->velocity.y = -0x28000;

        player->blinkTimer = 60;
        player->state      = Player_State_Hurt;
        RSDK.StopSfx(Player->sfxMightyDrill);
        player->onGround         = false;
        player->applyJumpCap     = false;
        player->jumpAbilityState = 0;

        if (player->state == Player_State_Hurt) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
        }

        int32 storeX     = self->position.x;
        int32 storeY     = self->position.y;
        self->position.x = self->sawPos[sawID].x;
        self->position.y = self->sawPos[sawID].y;
        RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
        Ice_ShatterGenerator(32, 24, 24, 0, 0, false);

        self->sawAngles[sawID] &= 0x1F0;
        self->sawTargetAngles[sawID] = 160;
        self->sawTimers[sawID]       = 180;
        RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[sawID], true, 0);

        self->stateSaw[sawID] = Shiversaw_StateSaw_Destroyed;
        self->state           = Shiversaw_State_HitRecoil;
        self->position.x      = storeX;
        self->position.y      = storeY;

        return true;
    }
#endif

    return Player_Hurt(player, &self->sawPos[sawID]);
}

void Shiversaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(Shiversaw);
    if (!Shiversaw->invincibilityTimer) {
        Vector2 storePos = self->position;

        foreach_active(Player, player)
        {
            for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
                if (self->sawAnimator[i].animationID == 3) {
                    self->position = self->sawPos[i];
                    if (Player_CheckCollisionTouch(player, self, &Shiversaw->hitboxSaw)) {
                        self->position = storePos;
                        Shiversaw_CheckSawHit(player, i);
                    }
                }
            }

            self->position = storePos;
            if (Player_CheckBadnikTouch(player, self, &Shiversaw->hitboxBoss) && Player_CheckBossHit(player, self)) {
                Shiversaw_Hit();
                foreach_break;
            }
        }
    }
    else {
        Shiversaw->invincibilityTimer--;
    }
}

void Shiversaw_SetupSawPos(void)
{
    RSDK_THIS(Shiversaw);

    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        if (i & 1)
            self->sawPos[i].x = self->position.x - 0x210000;
        else
            self->sawPos[i].x = self->position.x + 0x210000;
        self->sawPos[i].y = self->position.y + 0x70000;

        self->sawPos[i].x += (((self->sawRadius[i] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512((i << 8) + self->sawAngles[i]);
        self->sawPos[i].y += (((self->sawRadius[i] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512((i << 8) + self->sawAngles[i]);
    }
}

void Shiversaw_Hit(void)
{
    RSDK_THIS(Shiversaw);

    if (Shiversaw->health)
        Shiversaw->health--;

    if (Shiversaw->health) {
        RSDK.PlaySfx(Shiversaw->sfxHit, false, 255);
        Shiversaw->invincibilityTimer = 30;

        if (self->state != Shiversaw_State_HitRecoil) {
            self->timer = 120;
            self->state = Shiversaw_State_HitRecoil;
        }
    }
    else {
        RSDK.PlaySfx(Shiversaw->sfxExplosion2, false, 255);

        Shiversaw->invincibilityTimer = 30;
        int32 storeX                  = self->position.x;
        int32 storeY                  = self->position.y;

        self->sawID = 0;

        for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
            if (self->stateSaw[self->sawID] != Shiversaw_StateSaw_Destroyed) {
                RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                self->position.x = self->sawPos[self->sawID].x;
                self->position.y = self->sawPos[self->sawID].y;
                Ice_ShatterGenerator(32, 24, 24, 0, 0, false);

                self->sawAngles[self->sawID] &= 0x1F0;
                self->sawTargetAngles[self->sawID] = 160;
                RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[self->sawID], true, 0);
                self->stateSaw[self->sawID] = Shiversaw_StateSaw_Destroyed;
            }

            self->sawTimers[self->sawID++] = 0x7FFFFFFF;
        }

        self->position.x       = storeX;
        self->position.y       = storeY;
        self->timer            = 120;
        self->state            = Shiversaw_State_Destroyed;
        SceneInfo->timeEnabled = false;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        Player_GiveScore(player1, 1000);
    }
}

void Shiversaw_Explode(void)
{
    RSDK_THIS(Shiversaw);

    if (!(Zone->timer % 3)) {
        if (self->state != Shiversaw_State_Explode)
            RSDK.PlaySfx(Shiversaw->sfxExplosion2, false, 255);

        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(-19, 20) << 16);
            int32 y = self->position.y + (RSDK.Rand(-24, 25) << 16);

            EntityExplosion *explosion = CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y);
            explosion->drawGroup       = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void Shiversaw_CheckBoxCollisions(EntityPlayer *player)
{
    RSDK_THIS(Shiversaw);

    self->position.y = self->origin.y;

    if (self->state != Shiversaw_State_EnterShiversaw) {
        foreach_all(InvisibleBlock, block)
        {
            if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, self, &Shiversaw->hitboxT)) {
                if (self->velocity.x < 0) {
                    self->velocity.x = 0;
                }
                else {
                    self->velocity.x = 0x8000;
                    self->storedVelX = self->velocity.x;
                }
            }
            else if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, self, &Shiversaw->hitboxL)) {
                if (self->velocity.x > 0) {
                    self->velocity.x = 0;
                }
                else {
                    self->velocity.x = -0x8000;
                    self->storedVelX = self->velocity.x;
                }
            }
            else if (self->velocity.x) {
                self->storedVelX = self->velocity.x;
            }

            if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, self, &Shiversaw->hitboxR)) {
                if (self->velocity.y < 0)
                    self->velocity.y = 0;
                else
                    self->velocity.y = 0x8000;
            }
            else if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, self, &Shiversaw->hitboxB)) {
                if (self->velocity.y > 0)
                    self->velocity.y = 0;
                else
                    self->velocity.y = -0x8000;
            }
        }
    }

    uint8 collisionSideMask = 0;
    foreach_all(Crate, crate)
    {
        if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxT) && !(collisionSideMask & 1)) {
            if (player->position.x >= self->position.x) {
                if (self->velocity.x >= 0 && self->velocity.x < 0x8000) {
                    self->velocity.x = 0x8000;
                    collisionSideMask |= 1;
                }
                else if (self->velocity.x < 0) {
                    self->velocity.x = 0;
                    collisionSideMask |= 1;
                }
            }
            else {
                if (Shiversaw->timer > 0 && self->stateSaw[1] == Shiversaw_StateSaw_Active) {
                    self->sawTimers[1]    = 0;
                    self->sawRadiusVel[1] = -0x20000;
                    RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                    self->stateSaw[1] = Shiversaw_StateSaw_Extend;
                }

                self->velocity.x = 0;
                collisionSideMask |= 1;
            }
        }
        else if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxL) && !(collisionSideMask & 2)) {
            if (player->position.x <= self->position.x) {
                if (self->velocity.x <= 0 && self->velocity.x > -0x8000)
                    self->velocity.x = -0x8000;
                else if (self->velocity.x > 0)
                    self->velocity.x = 0;
            }
            else {
                if (Shiversaw->timer > 0 && self->stateSaw[0] == Shiversaw_StateSaw_Active) {
                    self->sawTimers[0]    = 0;
                    self->sawRadiusVel[0] = -0x20000;
                    RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                    self->stateSaw[0] = Shiversaw_StateSaw_Extend;
                }

                self->velocity.x = 0;
            }

            collisionSideMask |= 2;
        }

        if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxR) && !(collisionSideMask & 4)) {
            if (self->velocity.y >= 0 && self->velocity.y < 0x8000)
                self->velocity.y = 0x8000;
            else if (self->velocity.y < 0)
                self->velocity.y = 0;

            collisionSideMask |= 4;
        }
        else if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxB) && !(collisionSideMask & 8)) {
            if (self->velocity.y <= 0 && self->velocity.y > -0x8000)
                self->velocity.y = -0x8000;
            else if (self->velocity.y > 0)
                self->velocity.y = 0;

            collisionSideMask |= 8;
        }

        if (collisionSideMask == (1 | 2 | 4)) {
            foreach_break;
        }
    }
}

void Shiversaw_State_Entry(void)
{
    RSDK_THIS(Shiversaw);

    if (++self->timer >= 2) {
#if MANIA_USE_PLUS
        foreach_active(Player, player)
        {
            if (player->velocity.x <= 0) {
                Player_CheckCollisionBox(player, self, &Shiversaw->hitboxEntryTrigger);
                if (player->velocity.x < 0)
                    continue;
            }

            Player_CheckCollisionBox(player, self, &Shiversaw->hitboxEntryBounds);
        }

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->classID == Player->classID && player1->position.x < self->position.x - 0x380000) {
            if (player1->position.y > self->position.y + 0x400000 && player1->onGround) {
                self->timer                 = 0;
                Zone->playerBoundActiveR[0] = true;
                Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 416;
                Zone->playerBoundActiveB[0] = true;
                Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 128;
                self->position.y -= 0x1000000;
                self->position.x -= 0x1000000;
                Shiversaw_SetupSawPos();
                self->active  = ACTIVE_NORMAL;
                self->visible = true;

                player1->direction       = FLIP_X;
                player1->groundVel       = 0;
                player1->velocity.x      = 0;
                player1->stateInput      = StateMachine_None;
                player1->state           = Player_State_Ground;
                player1->nextGroundState = StateMachine_None;
                player1->nextAirState    = StateMachine_None;
                player1->jumpPress       = false;
                player1->jumpHold        = false;
                player1->left            = false;
                player1->right           = false;
                player1->down            = false;
                player1->up              = true;

                EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                if (player2->classID == Player->classID) {
                    if (Player_CheckValidState(player2)) {
                        player2->direction       = FLIP_X;
                        player2->groundVel       = 0;
                        player2->velocity.x      = 0;
                        player2->stateInput      = StateMachine_None;
                        player2->state           = Player_State_Ground;
                        player2->nextGroundState = StateMachine_None;
                        player2->nextAirState    = StateMachine_None;
                        player2->jumpPress       = false;
                        player2->jumpHold        = false;
                        player2->left            = false;
                        player2->right           = false;
                        player2->down            = false;
                        player2->up              = true;
                    }
                }

                Music_TransitionTrack(TRACK_MINIBOSS, 0.014);
                self->state = Shiversaw_State_SetupBounds;
            }
            else if (player->position.x < self->position.x - 0x380000) {
                if (player1->position.y > self->position.y - 0x1000000) {
                    player1->stateInput = StateMachine_None;
                    player1->jumpPress  = false;
                    player1->jumpHold   = false;
                    player1->left       = false;
                    player1->right      = false;
                    player1->down       = false;
                    player1->up         = true;

                    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
                    if (player2->classID == Player->classID) {
                        if (Player_CheckValidState(player2)) {
                            player2->stateInput = StateMachine_None;
                            player2->jumpPress  = false;
                            player2->jumpHold   = false;
                            player2->left       = false;
                            player2->right      = false;
                            player2->down       = false;
                            player2->up         = true;
                        }
                    }
                }
            }
        }
#else
        self->timer                 = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + 416;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 128;
        self->position.y -= 0x1000000;
        self->position.x -= 0x1000000;
        Shiversaw_SetupSawPos();
        self->active  = ACTIVE_NORMAL;
        self->visible = true;
        self->state   = Shiversaw_State_SetupBounds;
#endif
    }
}

void Shiversaw_State_SetupBounds(void)
{
    RSDK_THIS(Shiversaw);

#if MANIA_USE_PLUS
    if (++self->timer >= 60) {
        self->timer = 0;
#endif
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > self->position.x + 0x500000) {
            if (player1->position.y > self->startY) {
                Zone->playerBoundActiveL[0] = true;
                Zone->cameraBoundsL[0]      = (self->position.x >> 16) - 416;
                self->state                 = Shiversaw_State_EnterShiversaw;
#if !MANIA_USE_PLUS
                Music_TransitionTrack(TRACK_MINIBOSS, 0.014);
#endif
            }
        }
#if MANIA_USE_PLUS
    }
#endif
}

void Shiversaw_State_EnterShiversaw(void)
{
    RSDK_THIS(Shiversaw);

    self->position.y += 0x30000;
    self->origin.x = self->position.x;
    self->origin.y = self->position.y;

    Shiversaw_SetupSawPos();

    if (self->position.y >= self->startY) {
        self->position.y = self->startY;
        self->angle      = 0;
        self->timer      = 120;
        self->state      = Shiversaw_State_Idle;
    }

    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }
}

void Shiversaw_State_HitRecoil_Tutorial(void)
{
    RSDK_THIS(Shiversaw);

    RSDK.ProcessAnimation(&self->bellowsAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 10);

    int32 id = 0;
    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        self->sawID = i;

        if (self->stateSaw[i]) {
            if (self->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
                self->sawTimers[i] = 0;
            StateMachine_Run(self->stateSaw[i]);
        }

        if (self->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
            id++;
    }

    if (id == SHIVERSAW_SAW_COUNT) {
        self->timer = 120;
        self->state = Shiversaw_State_Idle;

        foreach_active(Player, player)
        {
            if (player->sidekick)
                player->stateInput = Player_Input_P2_AI;
            else
                player->stateInput = Player_Input_P1;
        }
    }

    Shiversaw_CheckPlayerCollisions();
}

void Shiversaw_State_Idle(void)
{
    RSDK_THIS(Shiversaw);

    if (self->bellowsAnimator.frameID != 7)
        RSDK.ProcessAnimation(&self->bellowsAnimator);

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 10);

    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }

    Shiversaw_CheckPlayerCollisions();

    if (!--self->timer)
        self->state = Shiversaw_State_MoveToPlayer;
}

void Shiversaw_State_MoveToPlayer(void)
{
    RSDK_THIS(Shiversaw);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    self->velocity.x      = 0;
    self->velocity.y      = 0;

    bool32 isActive = true;
    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) isActive &= self->stateSaw[i] == Shiversaw_StateSaw_Active;

    if (isActive) {
        int32 x = self->position.x;
        int32 y = self->origin.y;

        int32 distX = self->position.x - player1->position.x;
        if (distX >= 0) {
            if (distX < 0x380000)
                self->velocity.x = 0xC000;
            else if (distX > 0x390000)
                self->velocity.x = -0xC000;
        }
        else if (distX <= -0x380000) {
            if (distX < -0x390000)
                self->velocity.x = 0xC000;
        }
        else
            self->velocity.x = -0xC000;

        int32 distY = self->origin.y - player1->position.y;
        if (distY < 0) {
            if (distY <= -0x540000) {
                if (distY < -0x550000)
                    self->velocity.y = 0xC000;
            }
            else
                self->velocity.y = -0xC000;
        }
        else if (distY >= 0x540000) {
            if (distY > 0x550000)
                self->velocity.y = -0xC000;
        }
        else
            self->velocity.y = 0xC000;

        self->position.x += self->velocity.x;
        self->origin.y += self->velocity.y;

        Shiversaw_CheckBoxCollisions(player1);

        self->position.x = x + self->velocity.x;
        self->origin.y   = y + self->velocity.y;
    }

    if (self->velocity.x || self->velocity.y) {
        self->sawTimers[0] = 0;
        self->sawTimers[1] = 0;
    }

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 10);
    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }

    Shiversaw_CheckPlayerCollisions();
}

void Shiversaw_State_HitRecoil(void)
{
    RSDK_THIS(Shiversaw);

    RSDK.ProcessAnimation(&self->bellowsAnimator);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (self->position.x <= player1->position.x)
        self->velocity.x = -0x1000 * (11 - Shiversaw->health);
    else
        self->velocity.x = (11 - Shiversaw->health) << 12;

    self->velocity.y = (11 - Shiversaw->health) << 12;
    if (self->position.y <= player1->position.y)
        self->velocity.y = -self->velocity.y;

    int32 x = self->position.x;
    int32 y = self->origin.y;
    self->position.x += self->velocity.x;
    self->origin.y += self->velocity.y;

    Shiversaw_CheckBoxCollisions(player1);

    self->position.x = x + self->velocity.x;
    self->origin.y   = y + self->velocity.y;

    self->position.y = BadnikHelpers_Oscillate(self->origin.y, 2, 10);

    int32 id = 0;
    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        self->sawID = i;

        if (self->stateSaw[i]) {
            if (self->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
                self->sawTimers[i] = 0;
            StateMachine_Run(self->stateSaw[i]);
        }

        if (self->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
            id++;
    }

    if (self->timer) {
        self->timer--;
        Shiversaw_CheckPlayerCollisions();
    }
    else {
        if (id == SHIVERSAW_SAW_COUNT) {
            self->timer = 120;
            self->state = Shiversaw_State_Idle;
        }

        Shiversaw_CheckPlayerCollisions();
    }
}

void Shiversaw_State_Destroyed(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&self->bellowsAnimator);

    for (int32 i = 0; i < SHIVERSAW_SAW_COUNT; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }

    Shiversaw_Explode();
    if (!(Zone->timer % 5))
        Camera_ShakeScreen(0, 1, 1);

    if (--self->timer <= 0) {
        if (Zone->stageFinishCallback) {
            Music_SetMusicTrack("ShiversawExplosion.ogg", TRACK_EGGMAN1, false);
            Music_PlayTrack(TRACK_EGGMAN1);
            self->timer     = 0;
            self->drawGroup = Zone->playerDrawGroup[1];
            self->state     = Shiversaw_State_Explode;
        }
        else {
            self->timer        = 120;
            self->circleRadius = 0;
            self->velocity.y   = -0x20000;
            self->state        = Shiversaw_State_Explode_NoTransition;
        }
    }
}

void Shiversaw_State_StageFinishCB(void)
{
    RSDK_THIS(Shiversaw);

    StateMachine_Run(Zone->stageFinishCallback);
    self->state = StateMachine_None;
}

void Shiversaw_State_Explode(void)
{
    RSDK_THIS(Shiversaw);

    RSDK.ProcessAnimation(&self->bellowsAnimator);

    Shiversaw_Explode();
    if (!(Zone->timer % 5))
        Camera_ShakeScreen(0, 2, 2);

    if (self->timer < 120) {
        self->circleRadius = 6 * self->timer;
        self->timer++;
    }
    else {
        foreach_all(SignPost, signPost)
        {
            signPost->active     = ACTIVE_NORMAL;
            signPost->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
            signPost->position.y = (ScreenInfo->position.y - 64) << 16;
        }

        self->state = Shiversaw_State_StageFinishCB;
    }
}

void Shiversaw_State_Explode_NoTransition(void)
{
    RSDK_THIS(Shiversaw);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    Shiversaw_Explode();

    globals->enableIntro      = false;
    globals->atlEnabled       = false;
    Zone->stageFinishCallback = StateMachine_None;

    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->timer = 0;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->state = Shiversaw_State_DropSignPost;
    }
}

void Shiversaw_State_DropSignPost(void)
{
    RSDK_THIS(Shiversaw);

    if (++self->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->position.y = (ScreenInfo->position.y - 64) << 16;
            signPost->state      = SignPost_State_Falling;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 0xFF);
        }

        destroyEntity(self);
    }
}

void Shiversaw_CheckCrateCollisions(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID  = self->sawID;
    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->sawPos[sawID].x;
    self->position.y = self->sawPos[sawID].y;

    bool32 shatteredSaw = false;
    if (self->state != Shiversaw_State_EnterShiversaw) {
        foreach_all(InvisibleBlock, block)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &Shiversaw->hitboxSaw, block, &block->hitbox)) {
                RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                Ice_ShatterGenerator(32, 24, 24, 0, 0, false);

                self->sawAngles[self->sawID] &= 0x1F0;
                self->sawTargetAngles[self->sawID] = 160;
                self->sawTimers[self->sawID]       = 180;
                RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[self->sawID], true, 0);

                self->stateSaw[self->sawID] = Shiversaw_StateSaw_Destroyed;
                if (self->state == Shiversaw_State_EnterShiversaw)
                    self->state = Shiversaw_State_HitRecoil_Tutorial;
                else
                    self->state = Shiversaw_State_HitRecoil;

                shatteredSaw = true;
                foreach_break;
            }
        }
    }

    if (!shatteredSaw) {
        foreach_all(Crate, crate)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &crate->hitbox, crate, &crate->hitbox)) {
                RSDK.StopSfx(Shiversaw->sfxTargeting);
                if (!crate->animator.frameID) {
                    Crate_Break(crate);
                    if (self->stateSaw[sawID] == Shiversaw_StateSaw_Extend)
                        self->stateSaw[sawID] = Shiversaw_StateSaw_Retract;
                }
                else if (crate->animator.frameID == 1 || crate->animator.frameID == 2) {
                    if (crate->animator.frameID == 2) {
                        --crate->animator.frameID;
                        --crate->frameID;
                    }
                    else {
                        Crate_Break(crate);
                    }

                    RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                    Ice_ShatterGenerator(32, 24, 24, 0, 0, false);

                    self->sawAngles[self->sawID] &= 0x1F0;
                    self->sawTargetAngles[self->sawID] = 160;
                    self->sawTimers[self->sawID]       = 180;
                    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[self->sawID], true, 0);

                    self->stateSaw[self->sawID] = Shiversaw_StateSaw_Destroyed;
                    if (self->state == Shiversaw_State_EnterShiversaw)
                        self->state = Shiversaw_State_HitRecoil_Tutorial;
                    else
                        self->state = Shiversaw_State_HitRecoil;

                    foreach_break;
                }
            }
        }
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void Shiversaw_HandleSawMovement(int32 speed)
{
    RSDK_THIS(Shiversaw);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    int32 sawID           = self->sawID;

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    // Yeah
    self->position.x = self->sawPos[sawID].x;
    self->position.y = self->sawPos[sawID].y;
    self->position.x = storeX;
    self->position.y = storeY;

    if (self->stateSaw[sawID] == Shiversaw_StateSaw_Destroyed) {
        if (self->sawAngles[sawID] == self->sawTargetAngles[sawID]) {
            if (self->sawTargetAngles[sawID] == 160)
                self->sawTargetAngles[sawID] = 352;
            else
                self->sawTargetAngles[sawID] = 160;
        }
    }
    else {
        int32 angle                  = RSDK.ATan2(self->sawPos[sawID].x - player1->position.x, self->sawPos[sawID].y - player1->position.y);
        self->sawTargetAngles[sawID] = (2 * (angle - (~sawID << 7))) & 0x1FC;
    }

    if (self->sawAngles[sawID] != self->sawTargetAngles[sawID]) {
        if (((self->sawAngles[sawID] - 256) & 0x1FF) >= ((self->sawTargetAngles[sawID] - 256) & 0x1FF)) {
            if (self->sawAngles[sawID] <= 160 || self->sawAngles[sawID] > 352)
                self->sawAngles[sawID] -= speed;
        }
        else if (self->sawAngles[sawID] < 160 || self->sawAngles[sawID] >= 352) {
            self->sawAngles[sawID] += speed;
        }

        self->sawAngles[self->sawID] &= 0x1FF;
    }
}

void Shiversaw_StateSaw_Active(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    self->sawPos[sawID].y = self->origin.y + 0x70000;

    Shiversaw_HandleSawMovement(4);

    self->sawPos[sawID].y = self->position.y + 0x70000;

    self->sawPos[sawID].x += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    if (self->sawAngles[sawID] == self->sawTargetAngles[sawID]) {
        if (self->stateSaw[sawID ^ 1] == Shiversaw_StateSaw_Active) {

            ++self->sawTimers[sawID];
            if (self->sawTimers[sawID] >= 30) {
                self->sawTimers[sawID]    = 0;
                self->sawRadiusVel[sawID] = -0x20000;
                RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                self->stateSaw[sawID] = Shiversaw_StateSaw_Targeting;
            }
        }
    }
    else {
        self->sawTimers[sawID] = 0;
    }
}

void Shiversaw_StateSaw_Targeting(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;

    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (self->sawRadiusVel[sawID] < 0) {
        self->sawRadiusVel[sawID] += 0x3000;
        self->sawRadius[sawID] += self->sawRadiusVel[sawID];
    }

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;

    Shiversaw_HandleSawMovement(1);

    self->sawPos[sawID].y = self->position.y + 0x70000;

    self->sawPos[sawID].x += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    ++self->sawTimers[sawID];
    if (self->sawTimers[sawID] >= 45) {
        RSDK.StopSfx(Shiversaw->sfxTargeting);
        RSDK.PlaySfx(Shiversaw->sfxRocketJet, false, 255);
        self->stateSaw[sawID] = Shiversaw_StateSaw_Extend;
    }
}

void Shiversaw_StateSaw_Extend(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (self->sawRadiusVel[sawID] < 0x40000)
        self->sawRadiusVel[sawID] += 0x3000;

    self->sawRadius[sawID] += self->sawRadiusVel[sawID];

    if (self->sawRadius[sawID] >= 0x280000) {
        self->sawTimers[sawID]      = 0;
        self->stateSaw[self->sawID] = Shiversaw_StateSaw_Retract;
    }

    Shiversaw_ProcessSawMovement(sawID);
    Shiversaw_CheckCrateCollisions();
}

void Shiversaw_StateSaw_Retract(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;

    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (self->sawRadiusVel[sawID] > -0x80000)
        self->sawRadiusVel[sawID] -= 0x6000;

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    self->sawPos[self->sawID].y = self->position.y + 0x70000;

    self->sawRadius[sawID] += self->sawRadiusVel[sawID];

    if (self->sawRadius[sawID] <= 0) {
        self->sawRadius[sawID] = 0;
        self->sawAngles[sawID] &= 0x1FC;
        self->stateSaw[sawID] = Shiversaw_StateSaw_Active;
    }

    self->sawPos[sawID].x += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    Shiversaw_CheckCrateCollisions();
}

void Shiversaw_StateSaw_Destroyed(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    self->sawPos[sawID].y = self->position.y + 0x70000;

    Shiversaw_HandleSawMovement(16);

    self->sawRadius[sawID] -= 0x80000;
    if (self->sawRadius[sawID] <= 0)
        self->sawRadius[sawID] = 0;

    self->sawPos[sawID].x += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    if (!(Zone->timer & 3)) {
        EntityDust *dust = CREATE_ENTITY(Dust, self, self->sawPos[self->sawID].x, self->sawPos[self->sawID].y);
        dust->velocity.x = RSDK.Rand(-4, 5) << 15;
        dust->velocity.y = RSDK.Rand(-4, 5) << 15;
        dust->inkEffect  = INK_ALPHA;
        dust->drawGroup  = self->drawGroup - 1;
        dust->alpha      = 128;
        dust->state      = Shiversaw_StateDust_Debris;
    }

    if (!(Zone->timer & 0xF)) {
        EntityIce *ice           = CREATE_ENTITY(Ice, INT_TO_VOID(ICE_CHILD_SHARD), self->sawPos[self->sawID].x, self->sawPos[self->sawID].y);
        ice->velocity.x          = RSDK.Rand(-6, 8) << 15;
        ice->velocity.y          = RSDK.Rand(-10, 2) << 15;
        ice->direction           = RSDK.Rand(0, 4);
        ice->blockAnimator.speed = RSDK.Rand(1, 4);
        ice->drawGroup           = self->drawGroup - 1;
    }

    --self->sawTimers[sawID];
    if (!self->sawTimers[sawID]) {
        self->sawTimers[sawID] = 30;
        self->stateSaw[sawID]  = Shiversaw_StateSaw_Form;
    }
}

void Shiversaw_StateSaw_Form(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    Shiversaw_ProcessSawMovement(sawID);

    if (self->sawAnimator[sawID].frameID == self->sawAnimator[sawID].frameCount - 1) {
        RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[sawID], true, 0);
        self->stateSaw[sawID] = Shiversaw_StateSaw_Active;
    }
}

void Shiversaw_StateSaw_Setup(void)
{
    RSDK_THIS(Shiversaw);

    int32 sawID = self->sawID;

    if (self->state == Shiversaw_State_EnterShiversaw) {
        RSDK.ProcessAnimation(&self->sawAnimator[sawID]);
        Shiversaw_ProcessSawMovement(sawID);
        Shiversaw_CheckCrateCollisions();
    }
    else {
        self->stateSaw[sawID] = Shiversaw_StateSaw_Active;
        Shiversaw_StateSaw_Active();
    }
}

void Shiversaw_ProcessSawMovement(int32 sawID)
{
    RSDK_THIS(Shiversaw);

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    self->sawPos[sawID].y = self->position.y + 0x70000;

    self->sawPos[sawID].x += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->sawRadius[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));
}

void Shiversaw_StateDust_Debris(void)
{
    RSDK_THIS(Dust);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->visible ^= true;

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Shiversaw_EditorDraw(void)
{
    RSDK_THIS(Shiversaw);

    self->active    = ACTIVE_BOUNDS;
    self->visible   = false;
    self->drawGroup = Zone->objectDrawGroup[0];

    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 0, &self->tanksAnimator, true, 0);
    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 1, &self->bellowsAnimator, true, 7);
    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 4, &self->connectorAnimator, true, 0);
    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 5, &self->armAnimator, true, 0);
    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 6, &self->spinnerAnimator, true, 0);
    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[0], true, 0);
    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[1], true, 0);

    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    for (int32 s = 0; s < SHIVERSAW_SAW_COUNT; ++s) self->sawAngles[s] = 0x80 + ((s & 1) != 0) * 0x100;
    Shiversaw_SetupSawPos();

    Shiversaw_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-416, -SCREEN_YSIZE, 416, 128, 1 | 0 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Shiversaw_EditorLoad(void) { Shiversaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Shiversaw.bin", SCOPE_STAGE); }
#endif

void Shiversaw_Serialize(void) {}
