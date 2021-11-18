#include "SonicMania.h"

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

    self->sawID = Shiversaw_SawCount;
    for (int32 s = 0; s < Shiversaw_SawCount; ++s) {
        --self->sawID;
        self->direction = self->sawID;

        RSDK.DrawSprite(&self->sawAnimator[self->sawID], &self->sawPos[self->sawID], false);
        self->rotation = self->sawAngles[self->sawID];
        if (self->sawID & 1)
            drawPos.x = self->position.x - 0x210000;
        else
            drawPos.x = self->position.x + 0x210000;
        drawPos.y = self->position.y + 0x70000;

        if (self->field_90[self->sawID] >> 19 < 0) {
            drawPos.x -= ((-self->field_90[self->sawID] >> 9) & 0x380) * RSDK.Cos512(self->rotation + (self->sawID << 8));
            drawPos.y -= ((-self->field_90[self->sawID] >> 9) & 0x380) * RSDK.Sin512(self->rotation + (self->sawID << 8));

            int32 cnt = ~(self->field_90[self->sawID] >> 19);
            for (int32 i = cnt; i > 0; --i) {
                drawPos.x += -0x400 * RSDK.Cos512(self->rotation + (self->sawID << 8));
                drawPos.y += -0x400 * RSDK.Sin512(self->rotation + (self->sawID << 8));
            }
        }
        else {
            drawPos.x += ((self->field_90[self->sawID] >> 9) & 0x380) * RSDK.Cos512(self->rotation + (self->sawID << 8));
            drawPos.y += ((self->field_90[self->sawID] >> 9) & 0x380) * RSDK.Sin512(self->rotation + (self->sawID << 8));
            RSDK.DrawSprite(&self->animator4, &drawPos, false);

            int32 cnt = (self->field_90[self->sawID] >> 19) - 1;
            for (int32 i = cnt; i >= 0; --i) {
                drawPos.x += RSDK.Cos512(self->rotation + (self->sawID << 8)) << 10;
                drawPos.y += RSDK.Sin512(self->rotation + (self->sawID << 8)) << 10;
                RSDK.DrawSprite(&self->animator4, &drawPos, false);
            }
        }

        drawPos.x += RSDK.Cos512(self->rotation + (self->sawID << 8)) << 10;
        drawPos.y += RSDK.Sin512(self->rotation + (self->sawID << 8)) << 10;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);

        if (self->sawID == FLIP_X)
            drawPos.x = self->position.x - 0x210000;
        else
            drawPos.x = self->position.x + 0x210000;
        drawPos.y = self->position.y + 0x70000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
    }

    self->direction = FLIP_NONE;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    if (Shiversaw->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    RSDK.DrawSprite(&self->animator2, NULL, false);

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
            self->drawOrder = Zone->drawOrderLow;
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 0, &self->animator1, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 1, &self->animator2, true, 7);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 4, &self->animator3, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 5, &self->animator4, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 6, &self->animator5, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, self->sawAnimator, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[1], true, 0);
            self->updateRange.x = 0x800000;
            self->updateRange.y = 0x800000;
            for (int32 s = 0; s < Shiversaw_SawCount; ++s) {
                self->stateSaw[s] = Shiversaw_StateSaw_Unknown1;
                self->sawAngles[s] = 0x80 + ((s & 1) != 0) * 0x100;
            }
            Shiversaw_Unknown3();
            Shiversaw->timer = 480;
            self->field_84 = self->position.y;
            self->state    = Shiversaw_State_Entry;
        }
        else {
            destroyEntity(self);
        }
    }
}

void Shiversaw_StageLoad(void)
{
    Shiversaw->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Shiversaw.bin", SCOPE_STAGE);

    Shiversaw->hitbox1.left   = -16;
    Shiversaw->hitbox1.top    = -22;
    Shiversaw->hitbox1.bottom = 22;
    Shiversaw->hitbox1.right  = 16;

    Shiversaw->hitbox2.left   = -16;
    Shiversaw->hitbox2.top    = -16;
    Shiversaw->hitbox2.right  = 16;
    Shiversaw->hitbox2.bottom = 16;

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

#if RETRO_USE_PLUS
    Shiversaw->hitbox7.left       = -192;
    Shiversaw->hitbox7.top        = -176;
    Shiversaw->hitbox7.right      = -144;
    Shiversaw->hitbox7.bottom     = 112;

    Shiversaw->hitbox8.left       = -64;
    Shiversaw->hitbox8.top        = -176;
    Shiversaw->hitbox8.right      = 0;
    Shiversaw->hitbox8.bottom     = 112;
#endif

    Shiversaw->health             = 6;
    Shiversaw->invincibilityTimer = 0;
    Shiversaw->sfxHit             = RSDK.GetSFX("Stage/BossHit.wav");
    Shiversaw->sfxExplosion2      = RSDK.GetSFX("Stage/Explosion2.wav");
    Shiversaw->sfxExplosion3      = RSDK.GetSFX("Stage/Explosion3.wav");
    Shiversaw->sfxTargeting       = RSDK.GetSFX("Stage/Targeting1.wav");
    Shiversaw->sfxRocketJet       = RSDK.GetSFX("Stage/RocketJet.wav");
}

bool32 Shiversaw_CheckSawHit(EntityPlayer *player, int32 sawID)
{
    RSDK_THIS(Shiversaw);
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY) {
        int32 anim = player->animator.animationID;
        if (anim != ANI_JUMP && anim != ANI_SPINDASH && anim != ANI_DROPDASH)
            return Player_CheckHit(player, &self->sawPos[sawID]);
        if (anim != ANI_DROPDASH)
            return Player_CheckMightyUnspin(768, player, true, &player->uncurlTimer);

        if (!(player->direction & FLIP_X))
            player->velocity.x = -0x18000;
        else
            player->velocity.x = 0x18000;
        player->velocity.y = -0x28000;
        player->blinkTimer = 60;
        player->state      = Player_State_Hit;
        RSDK.StopSFX(Player->sfxMightyDrill);
        player->onGround         = false;
        player->jumpAbility      = 0;
        player->jumpAbilityTimer = 0;
        if (player->state == Player_State_Hit) {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
            RSDK.PlaySfx(Spikes->sfxSpike, false, 255);
        }
        int32 storeX         = self->position.x;
        int32 storeY         = self->position.y;
        self->position.x = self->sawPos[sawID].x;
        self->position.y = self->sawPos[sawID].y;
        RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
        Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
        self->sawAngles[sawID] &= 0x1F0;
        self->field_A0[sawID]  = 160;
        self->sawTimers[sawID] = 180;
        RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[sawID], true, 0);
        self->stateSaw[sawID] = Shiversaw_StateSaw_Destroyed;
        self->state           = Shiversaw_State_HitRecoil;
        self->position.x      = storeX;
        self->position.y      = storeY;
        return true;
    }
#endif
    return Player_CheckHit(player, &self->sawPos[sawID]);
}

void Shiversaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(Shiversaw);
    if (!Shiversaw->invincibilityTimer) {
        foreach_active(Player, player)
        {
            for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
                if (self->sawAnimator[i].animationID == 3) {
                    if (Player_CheckCollisionTouch(player, &self->sawPos[i], &Shiversaw->hitbox2)) {
                        Shiversaw_CheckSawHit(player, i);
                    }
                }
            }

            if (Player_CheckBadnikTouch(player, self, &Shiversaw->hitbox1) && Player_CheckBossHit(player, self)) {
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
                    int32 v14                       = self->position.x;
                    int32 v15                       = self->position.y;
                    self->sawID                 = 0;
                    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
                        if (self->stateSaw[self->sawID] != Shiversaw_StateSaw_Destroyed) {
                            RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                            self->position.x = self->sawPos[self->sawID].x;
                            self->position.y = self->sawPos[self->sawID].y;
                            Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
                            self->sawAngles[self->sawID] &= 0x1F0;
                            self->field_A0[self->sawID] = 160;
                            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[self->sawID], true, 0);
                            self->stateSaw[self->sawID] = Shiversaw_StateSaw_Destroyed;
                        }
                        self->sawTimers[self->sawID++] = 0x7FFFFFFF;
                    }

                    self->position.x          = v14;
                    self->position.y          = v15;
                    self->timer               = 120;
                    self->state               = Shiversaw_State_Destroyed;
                    SceneInfo->timeEnabled = false;
                    EntityPlayer *player1       = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                    Player_GiveScore(player1, 1000);
                }
                foreach_break;
            }
        }
    }
    else {
        Shiversaw->invincibilityTimer--;
    }
}

void Shiversaw_Unknown3(void)
{
    RSDK_THIS(Shiversaw);
    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
        if (i & 1)
            self->sawPos[1].x = self->position.x - 0x210000;
        else
            self->sawPos[i].x = self->position.x + 0x210000;
        self->sawPos[i].y = self->position.y + 0x70000;
        self->sawPos[i].x += (((self->field_90[i] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512((i << 8) + self->sawAngles[i]);
        self->sawPos[i].y += (((self->field_90[i] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512((i << 8) + self->sawAngles[i]);
    }
}

void Shiversaw_Unknown4(void)
{
    RSDK_THIS(Shiversaw);
    if (!(Zone->timer % 3)) {
        if (self->state != Shiversaw_Unknown11)
            RSDK.PlaySfx(Shiversaw->sfxExplosion2, false, 255);

        if (Zone->timer & 4) {
            int32 x = RSDK.Rand(-19, 20) << 16;
            int32 y = RSDK.Rand(-24, 25) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), self->position.x + x, self->position.y + y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void Shiversaw_Unknown5(EntityPlayer *player)
{
    RSDK_THIS(Shiversaw);

    self->position.y = self->field_88.y;
    if (self->state != Shiversaw_State_Unknown1) {
        foreach_all(InvisibleBlock, block)
        {
            if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, self, &Shiversaw->hitboxT)) {
                if (self->velocity.x < 0) {
                    self->velocity.x = 0;
                }
                else {
                    self->velocity.x = 0x8000;
                    self->field_B8   = self->velocity.x;
                }
            }
            else if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, self, &Shiversaw->hitboxL)) {
                if (self->velocity.x > 0) {
                    self->velocity.x = 0;
                }
                else {
                    self->velocity.x = -0x8000;
                    self->field_B8   = self->velocity.x;
                }
            }
            else if (self->velocity.x) {
                self->field_B8 = self->velocity.x;
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

    uint8 flags = 0;
    foreach_all(Crate, crate)
    {
        if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxT) && !(flags & 1)) {
            if (player->position.x >= self->position.x) {
                if (self->velocity.x >= 0 && self->velocity.x < 0x8000) {
                    self->velocity.x = 0x8000;
                    flags |= 1;
                }
                else if (self->velocity.x < 0) {
                    self->velocity.x = 0;
                    flags |= 1;
                }
            }
            else {
                if (Shiversaw->timer > 0 && self->stateSaw[1] == Shiversaw_StateSaw_Unknown2) {
                    self->sawTimers[1] = 0;
                    self->field_74[1]  = -0x20000;
                    RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                    self->stateSaw[1] = Shiversaw_StateSaw_Unknown4;
                }
                self->velocity.x = 0;
                flags |= 1;
            }
        }
        else if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxL) && !(flags & 2)) {
            if (player->position.x <= self->position.x) {
                if (self->velocity.x <= 0 && self->velocity.x > -0x8000) {
                    self->velocity.x = -0x8000;
                }
                else if (self->velocity.x > 0) {
                    self->velocity.x = 0;
                }
            }
            else {
                if (Shiversaw->timer > 0 && self->stateSaw[0] == Shiversaw_StateSaw_Unknown2) {
                    self->sawTimers[0] = 0;
                    self->field_74[0]  = -0x20000;
                    RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                    self->stateSaw[0] = Shiversaw_StateSaw_Unknown4;
                }
                self->velocity.x = 0;
            }
            flags |= 2;
        }

        if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxR) && !(flags & 4)) {
            if (self->velocity.y >= 0 && self->velocity.y < 0x8000) {
                self->velocity.y = 0x8000;
            }
            else if (self->velocity.y < 0) {
                self->velocity.y = 0;
            }
            flags |= 4;
        }
        else if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, self, &Shiversaw->hitboxB) && !(flags & 8)) {
            if (self->velocity.y <= 0 && self->velocity.y > -0x8000) {
                self->velocity.y = -0x8000;
            }
            else if (self->velocity.y > 0) {
                self->velocity.y = 0;
            }
            flags |= 8;
        }
        if (flags == 7) {
            foreach_break;
        }
    }
}

void Shiversaw_State_Entry(void)
{
    RSDK_THIS(Shiversaw);
    if (++self->timer >= 2) {
#if RETRO_USE_PLUS
        foreach_active(Player, player)
        {
            if (player->velocity.x <= 0) {
                Player_CheckCollisionBox(player, self, &Shiversaw->hitbox7);
                if (player->velocity.x < 0)
                    continue;
            }
            Player_CheckCollisionBox(player, self, &Shiversaw->hitbox8);
        }

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->objectID == Player->objectID) {
            if (player1->position.x < self->position.x - 0x380000) {
                if (player1->position.y > self->position.y + 0x400000 && player1->onGround) {
                    self->timer               = 0;
                    Zone->playerBoundActiveR[0] = true;
                    Zone->screenBoundsR1[0]     = (self->position.x >> 16) + 416;
                    Zone->playerBoundActiveB[0] = true;
                    Zone->screenBoundsB1[0]     = (self->position.y >> 16) + 128;
                    self->position.y -= 0x1000000;
                    self->position.x -= 0x1000000;
                    Shiversaw_Unknown3();
                    self->active           = ACTIVE_NORMAL;
                    self->visible          = true;
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
                    if (player2->objectID == Player->objectID) {
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
                        if (player2->objectID == Player->objectID) {
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
        }
#else
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (self->position.x >> 16) + 416;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsB1[0]     = (self->position.y >> 16) + 128;
        self->position.y -= 0x1000000;
        self->position.x -= 0x1000000;
        Shiversaw_Unknown3();
        self->active  = ACTIVE_NORMAL;
        self->visible = true;
        self->state   = Shiversaw_State_SetupBounds;
#endif
    }
}

void Shiversaw_State_SetupBounds(void)
{
    RSDK_THIS(Shiversaw);
#if RETRO_USE_PLUS
    if (++self->timer >= 60) {
        self->timer               = 0;
#endif
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = ScreenInfo->position.x;
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > self->position.x + 0x500000) {
            if (player1->position.y > self->field_84) {
                Zone->playerBoundActiveL[0] = true;
                Zone->screenBoundsL1[0]     = (self->position.x >> 16) - 416;
                self->state               = Shiversaw_State_Unknown1;
#if !RETRO_USE_PLUS
                Music_TransitionTrack(TRACK_MINIBOSS, 0.014);
#endif
            }
        }
#if RETRO_USE_PLUS
    }
#endif
}

void Shiversaw_State_Unknown1(void)
{
    RSDK_THIS(Shiversaw);

    self->position.y += 0x30000;
    self->field_88.x = self->position.x;
    self->field_88.y = self->position.y;
    Shiversaw_Unknown3();

    if (self->position.y >= self->field_84) {
        self->position.y = self->field_84;
        self->angle      = 0;
        self->timer      = 120;
        self->state      = Shiversaw_State_Unknown2;
    }

    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }
}

void Shiversaw_Unknown9(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_88.y) & 0xFFFF0000;

    int32 id = 0;
    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
        self->sawID = i;
        if (self->stateSaw[i]) {
            if (self->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
                self->sawTimers[i] = 0;
            StateMachine_Run(self->stateSaw[i]);
        }
        if (self->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
            id++;
    }

    if (id == Shiversaw_SawCount) {
        self->timer = 120;
        self->state = Shiversaw_State_Unknown2;
        foreach_active(Player, player)
        {
            if (player->sidekick)
                player->stateInput = Player_ProcessP2Input_AI;
            else
                player->stateInput = Player_ProcessP1Input;
        }
    }
    Shiversaw_CheckPlayerCollisions();
}

void Shiversaw_State_Unknown2(void)
{
    RSDK_THIS(Shiversaw);
    if (self->animator2.frameID != 7)
        RSDK.ProcessAnimation(&self->animator2);

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_88.y) & 0xFFFF0000;
    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }

    Shiversaw_CheckPlayerCollisions();
    if (!--self->timer)
        self->state = Shiversaw_State_Unknown3;
}

void Shiversaw_State_Unknown3(void)
{
    RSDK_THIS(Shiversaw);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    self->velocity.x    = 0;
    self->velocity.y    = 0;

    bool32 flag = true;
    for (int32 i = 0; i < Shiversaw_SawCount; ++i) flag &= self->stateSaw[i] == Shiversaw_StateSaw_Unknown2;

    if (flag) {
        int32 x = self->position.x;
        int32 y = self->field_88.y;

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

        int32 distY = self->field_88.y - player1->position.y;
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
        self->field_88.y += self->velocity.y;
        Shiversaw_Unknown5(player1);
        self->position.x = x + self->velocity.x;
        self->field_88.y = y + self->velocity.y;
    }

    if (self->velocity.x || self->velocity.y) {
        self->sawTimers[0] = 0;
        self->sawTimers[1] = 0;
    }

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_88.y) & 0xFFFF0000;
    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }
    Shiversaw_CheckPlayerCollisions();
}

void Shiversaw_State_HitRecoil(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&self->animator2);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (self->position.x <= player1->position.x)
        self->velocity.x = -0x1000 * (11 - Shiversaw->health);
    else
        self->velocity.x = (11 - Shiversaw->health) << 12;

    self->velocity.y = (11 - Shiversaw->health) << 12;
    if (self->position.y <= player1->position.y)
        self->velocity.y = -self->velocity.y;

    int32 x = self->position.x;
    int32 y = self->field_88.y;
    self->position.x += self->velocity.x;
    self->field_88.y += self->velocity.y;
    Shiversaw_Unknown5(player1);
    self->position.x = x + self->velocity.x;
    self->field_88.y = y + self->velocity.y;

    self->angle      = (self->angle + 2) & 0xFF;
    self->position.y = ((RSDK.Sin256(self->angle) << 10) + self->field_88.y) & 0xFFFF0000;

    int32 id = 0;
    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
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
        if (id == Shiversaw_SawCount) {
            self->timer = 120;
            self->state = Shiversaw_State_Unknown2;
        }
        Shiversaw_CheckPlayerCollisions();
    }
}

void Shiversaw_State_Destroyed(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&self->animator2);

    for (int32 i = 0; i < Shiversaw_SawCount; ++i) {
        self->sawID = i;
        StateMachine_Run(self->stateSaw[i]);
    }

    Shiversaw_Unknown4();
    if (!(Zone->timer % 5))
        Camera_ShakeScreen(0, 1, 1);

    if (--self->timer <= 0) {
        if (Zone->stageFinishCallback) {
            Music_SetMusicTrack("ShiversawExplosion.ogg", TRACK_EGGMAN1, false);
            Music_PlayTrack(TRACK_EGGMAN1);
            self->timer     = 0;
            self->drawOrder = Zone->playerDrawHigh;
            self->state     = Shiversaw_Unknown11;
        }
        else {
            self->timer        = 120;
            self->circleRadius = 0;
            self->velocity.y   = -0x20000;
            self->state        = Shiversaw_Unknown12;
        }
    }
}

void Shiversaw_Unknown10(void)
{
    RSDK_THIS(Shiversaw);
    StateMachine_Run(Zone->stageFinishCallback);
    self->state = StateMachine_None;
}

void Shiversaw_Unknown11(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&self->animator2);
    Shiversaw_Unknown4();
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
            signPost->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
            signPost->position.y = (ScreenInfo->position.y - 64) << 16;
        }
        self->state = Shiversaw_Unknown10;
    }
}

void Shiversaw_Unknown12(void)
{
    RSDK_THIS(Shiversaw);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;
    Shiversaw_Unknown4();
    globals->enableIntro      = false;
    globals->atlEnabled       = false;
    Zone->stageFinishCallback = StateMachine_None;
    if (!RSDK.CheckOnScreen(self, &self->updateRange)) {
        self->timer = 0;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        self->state = Shiversaw_Unknown13;
    }
}

void Shiversaw_Unknown13(void)
{
    RSDK_THIS(Shiversaw);
    if (++self->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = self->position.x;
            signPost->position.y = (ScreenInfo->position.y - 64) << 16;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
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

    bool32 flag = false;
    if (self->state != Shiversaw_State_Unknown1) {
        foreach_all(InvisibleBlock, block)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &Shiversaw->hitbox2, block, &block->hitbox)) {
                RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
                self->sawAngles[self->sawID] &= 0x1F0;
                self->field_A0[self->sawID]  = 160;
                self->sawTimers[self->sawID] = 180;
                RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[self->sawID], true, 0);
                self->stateSaw[self->sawID] = Shiversaw_StateSaw_Destroyed;
                if (self->state == Shiversaw_State_Unknown1)
                    self->state = Shiversaw_Unknown9;
                else
                    self->state = Shiversaw_State_HitRecoil;
                flag = true;
                foreach_break;
            }
        }
    }

    if (!flag) {
        foreach_all(Crate, crate)
        {
            if (RSDK.CheckObjectCollisionTouchBox(self, &crate->hitbox, crate, &crate->hitbox)) {
                RSDK.StopSFX(Shiversaw->sfxTargeting);
                if (!crate->animator.frameID) {
                    Crate_Break(crate);
                    if (self->stateSaw[sawID] == Shiversaw_StateSaw_Unknown4)
                        self->stateSaw[sawID] = Shiversaw_StateSaw_Unknown5;
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
                    Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
                    self->sawAngles[self->sawID] &= 0x1F0;
                    self->field_A0[self->sawID]  = 160;
                    self->sawTimers[self->sawID] = 180;
                    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &self->sawAnimator[self->sawID], true, 0);
                    self->stateSaw[self->sawID] = Shiversaw_StateSaw_Destroyed;
                    if (self->state == Shiversaw_State_Unknown1)
                        self->state = Shiversaw_Unknown9;
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

void Shiversaw_Unknown14(int32 speed)
{
    RSDK_THIS(Shiversaw);
    int32 sawID             = self->sawID;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    int32 storeX         = self->position.x;
    int32 storeY         = self->position.y;
    self->position.x = self->sawPos[sawID].x;
    self->position.y = self->sawPos[sawID].y;
    self->position.x = storeX;
    self->position.y = storeY;
    if (self->stateSaw[sawID] == Shiversaw_StateSaw_Destroyed) {
        if (self->sawAngles[sawID] == self->field_A0[sawID]) {
            if (self->field_A0[sawID] == 160)
                self->field_A0[sawID] = 352;
            else
                self->field_A0[sawID] = 160;
        }
    }
    else {
        int32 angle               = RSDK.ATan2(self->sawPos[sawID].x - player1->position.x, self->sawPos[sawID].y - player1->position.y);
        self->field_A0[sawID] = (2 * (angle - (~sawID << 7))) & 0x1FC;
    }

    if (self->sawAngles[sawID] != self->field_A0[sawID]) {
        if (((self->sawAngles[sawID] - 256) & 0x1FF) >= ((self->field_A0[sawID] - 256) & 0x1FF)) {
            if (self->sawAngles[sawID] <= 160 || self->sawAngles[sawID] > 352)
                self->sawAngles[sawID] -= speed;
        }
        else if (self->sawAngles[sawID] < 160 || self->sawAngles[sawID] >= 352) {
            self->sawAngles[sawID] += speed;
        }
        self->sawAngles[self->sawID] &= 0x1FF;
    }
}

void Shiversaw_StateSaw_Unknown2(void)
{
    RSDK_THIS(Shiversaw);
    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    self->sawPos[sawID].y = self->field_88.y + 0x70000;
    Shiversaw_Unknown14(4);
    self->sawPos[sawID].y = self->position.y + 0x70000;

    self->sawPos[sawID].x += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    if (self->sawAngles[sawID] == self->field_A0[sawID]) {
        if (self->stateSaw[sawID ^ 1] == Shiversaw_StateSaw_Unknown2) {
            ++self->sawTimers[sawID];
            if (self->sawTimers[sawID] >= 30) {
                self->sawTimers[sawID] = 0;
                self->field_74[sawID]  = -0x20000;
                RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                self->stateSaw[sawID] = Shiversaw_StateSaw_Unknown3;
            }
        }
    }
    else {
        self->sawTimers[sawID] = 0;
    }
}

void Shiversaw_StateSaw_Unknown3(void)
{
    RSDK_THIS(Shiversaw);
    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (self->field_74[sawID] < 0) {
        self->field_74[sawID] += 0x3000;
        self->field_90[sawID] += self->field_74[sawID];
    }

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    Shiversaw_Unknown14(1);
    self->sawPos[sawID].y = self->position.y + 0x70000;

    self->sawPos[sawID].x += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    ++self->sawTimers[sawID];
    if (self->sawTimers[sawID] >= 45) {
        RSDK.StopSFX(Shiversaw->sfxTargeting);
        RSDK.PlaySfx(Shiversaw->sfxRocketJet, false, 255);
        self->stateSaw[sawID] = Shiversaw_StateSaw_Unknown4;
    }
}

void Shiversaw_StateSaw_Unknown4(void)
{
    RSDK_THIS(Shiversaw);
    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (self->field_74[sawID] < 0x40000)
        self->field_74[sawID] += 0x3000;

    self->field_90[sawID] += self->field_74[sawID];

    if (self->field_90[sawID] >= 0x280000) {
        self->sawTimers[sawID]        = 0;
        self->stateSaw[self->sawID] = Shiversaw_StateSaw_Unknown5;
    }

    Shiversaw_ProcessSawMovement(sawID);
    Shiversaw_CheckCrateCollisions();
}

void Shiversaw_StateSaw_Unknown5(void)
{
    RSDK_THIS(Shiversaw);
    int32 sawID = self->sawID;

    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);

    if (self->field_74[sawID] > -0x80000)
        self->field_74[sawID] -= 0x6000;

    if (sawID & 1)
        self->sawPos[sawID].x = self->position.x - 0x210000;
    else
        self->sawPos[sawID].x = self->position.x + 0x210000;
    self->sawPos[self->sawID].y = self->position.y + 0x70000;
    self->field_90[sawID] += self->field_74[sawID];
    if (self->field_90[sawID] <= 0) {
        self->field_90[sawID] = 0;
        self->sawAngles[sawID] &= 0x1FC;
        self->stateSaw[sawID] = Shiversaw_StateSaw_Unknown2;
    }

    self->sawPos[sawID].x += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));
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
    Shiversaw_Unknown14(16);
    self->field_90[sawID] -= 0x80000;
    if (self->field_90[sawID] <= 0)
        self->field_90[sawID] = 0;
    self->sawPos[sawID].x += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));

    if (!(Zone->timer & 3)) {
        EntityDust *dust = CREATE_ENTITY(Dust, self, self->sawPos[self->sawID].x, self->sawPos[self->sawID].y);
        dust->velocity.x = RSDK.Rand(-4, 5) << 15;
        dust->velocity.y = RSDK.Rand(-4, 5) << 15;
        dust->inkEffect  = INK_ALPHA;
        dust->drawOrder  = self->drawOrder - 1;
        dust->alpha      = 128;
        dust->state      = Shiversaw_StateDust_Unknown;
    }
    if (!(Zone->timer & 0xF)) {
        EntityIce *ice                = CREATE_ENTITY(Ice, intToVoid(3), self->sawPos[self->sawID].x, self->sawPos[self->sawID].y);
        ice->velocity.x               = RSDK.Rand(-6, 8) << 15;
        ice->velocity.y               = RSDK.Rand(-10, 2) << 15;
        ice->direction                = RSDK.Rand(0, 4);
        ice->animator1.animationSpeed = RSDK.Rand(1, 4);
        ice->drawOrder                = self->drawOrder - 1;
    }

    --self->sawTimers[sawID];
    if (!self->sawTimers[sawID]) {
        self->sawTimers[sawID] = 30;
        self->stateSaw[sawID]  = Shiversaw_StateSaw_Unknown7;
    }
}

void Shiversaw_StateSaw_Unknown7(void)
{
    RSDK_THIS(Shiversaw);
    int32 sawID = self->sawID;
    RSDK.ProcessAnimation(&self->sawAnimator[sawID]);
    Shiversaw_ProcessSawMovement(sawID);
    if (self->sawAnimator[sawID].frameID == self->sawAnimator[sawID].frameCount - 1) {
        RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &self->sawAnimator[sawID], true, 0);
        self->stateSaw[sawID] = Shiversaw_StateSaw_Unknown2;
    }
}

void Shiversaw_StateSaw_Unknown1(void)
{
    RSDK_THIS(Shiversaw);
    int32 sawID = self->sawID;
    if (self->state == Shiversaw_State_Unknown1) {
        RSDK.ProcessAnimation(&self->sawAnimator[sawID]);
        Shiversaw_ProcessSawMovement(sawID);
        Shiversaw_CheckCrateCollisions();
    }
    else {
        self->stateSaw[sawID] = Shiversaw_StateSaw_Unknown2;
        Shiversaw_StateSaw_Unknown2();
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
    self->sawPos[sawID].x += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(self->sawAngles[sawID] + (sawID << 8));
    self->sawPos[sawID].y += (((self->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(self->sawAngles[sawID] + (sawID << 8));
}

void Shiversaw_StateDust_Unknown(void)
{
    RSDK_THIS(Dust);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->visible ^= 1;
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    RSDK.ProcessAnimation(&self->animator);
    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void Shiversaw_EditorDraw(void) {}

void Shiversaw_EditorLoad(void) {}
#endif

void Shiversaw_Serialize(void) {}
