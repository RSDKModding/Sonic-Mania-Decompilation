#include "SonicMania.h"

ObjectShiversaw *Shiversaw;

void Shiversaw_Update(void)
{
    RSDK_THIS(Shiversaw);
    StateMachine_Run(entity->state);
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

    entity->sawID = Shiversaw_SawCount;
    for (int s = 0; s < Shiversaw_SawCount; ++s) {
        --entity->sawID;
        entity->direction = entity->sawID;

        RSDK.DrawSprite(&entity->sawAnimator[entity->sawID], &entity->sawPos[entity->sawID], false);
        entity->rotation = entity->sawAngles[entity->sawID];
        if (entity->sawID & 1)
            drawPos.x = entity->position.x - 0x210000;
        else
            drawPos.x = entity->position.x + 0x210000;
        drawPos.y = entity->position.y + 0x70000;

        if (entity->field_90[entity->sawID] >> 19 < 0) {
            drawPos.x -= ((-entity->field_90[entity->sawID] >> 9) & 0x380) * RSDK.Cos512(entity->rotation + (entity->sawID << 8));
            drawPos.y -= ((-entity->field_90[entity->sawID] >> 9) & 0x380) * RSDK.Sin512(entity->rotation + (entity->sawID << 8));

            int cnt = ~(entity->field_90[entity->sawID] >> 19);
            for (int i = cnt; i > 0; --i) {
                drawPos.x += -0x400 * RSDK.Cos512(entity->rotation + (entity->sawID << 8));
                drawPos.y += -0x400 * RSDK.Sin512(entity->rotation + (entity->sawID << 8));
            }
        }
        else {
            drawPos.x += ((entity->field_90[entity->sawID] >> 9) & 0x380) * RSDK.Cos512(entity->rotation + (entity->sawID << 8));
            drawPos.y += ((entity->field_90[entity->sawID] >> 9) & 0x380) * RSDK.Sin512(entity->rotation + (entity->sawID << 8));
            RSDK.DrawSprite(&entity->animator4, &drawPos, false);

            int cnt = (entity->field_90[entity->sawID] >> 19) - 1;
            for (int i = cnt; i >= 0; --i) {
                drawPos.x += RSDK.Cos512(entity->rotation + (entity->sawID << 8)) << 10;
                drawPos.y += RSDK.Sin512(entity->rotation + (entity->sawID << 8)) << 10;
                RSDK.DrawSprite(&entity->animator4, &drawPos, false);
            }
        }

        drawPos.x += RSDK.Cos512(entity->rotation + (entity->sawID << 8)) << 10;
        drawPos.y += RSDK.Sin512(entity->rotation + (entity->sawID << 8)) << 10;
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);

        if (entity->sawID == FLIP_X)
            drawPos.x = entity->position.x - 0x210000;
        else
            drawPos.x = entity->position.x + 0x210000;
        drawPos.y = entity->position.y + 0x70000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    }

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    if (Shiversaw->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
    if (entity->circleRadius > 0)
        RSDK.DrawCircle(entity->position.x, entity->position.y, entity->circleRadius, 0xFFFFFF, (entity->timer << 8) / 120, INK_ALPHA, false);
}

void Shiversaw_Create(void *data)
{
    RSDK_THIS(Shiversaw);
    entity->drawFX = FX_ROTATE | FX_FLIP;

    if (!RSDK_sceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active    = ACTIVE_BOUNDS;
            entity->visible   = false;
            entity->drawOrder = Zone->drawOrderLow;
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 0, &entity->animator1, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 1, &entity->animator2, true, 7);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 4, &entity->animator3, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 5, &entity->animator4, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 6, &entity->animator5, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, entity->sawAnimator, true, 0);
            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &entity->sawAnimator[1], true, 0);
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            for (int s = 0; s < Shiversaw_SawCount; ++s) {
                entity->stateSaw[s] = Shiversaw_StateSaw_Unknown1;
                entity->sawAngles[s] = 0x80 + ((s & 1) != 0) * 0x100;
            }
            Shiversaw_Unknown3();
            Shiversaw->timer = 480;
            entity->field_84 = entity->position.y;
            entity->state    = Shiversaw_State_Entry;
        }
        else {
            destroyEntity(entity);
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

bool32 Shiversaw_CheckSawHit(EntityPlayer *player, int sawID)
{
    RSDK_THIS(Shiversaw);
#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY) {
        int anim = player->playerAnimator.animationID;
        if (anim != ANI_JUMP && anim != ANI_SPINDASH && anim != ANI_DROPDASH)
            return Player_CheckHit(player, &entity->sawPos[sawID]);
        if (anim != ANI_DROPDASH)
            return Player_CheckMightyUnspin(768, player, true, &player->uncurlTimer);

        if (!(player->direction & FLIP_X))
            player->velocity.x = -0x18000;
        else
            player->velocity.x = 0x18000;
        player->velocity.y = -0x28000;
        player->blinkTimer = 60;
        player->state      = Player_State_Hit;
        RSDK.StopSFX(Player->sfx_MightyDrill);
        player->onGround         = false;
        player->jumpAbility      = 0;
        player->jumpAbilityTimer = 0;
        if (player->state == Player_State_Hit) {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
            RSDK.PlaySfx(Spikes->sfx_Spike, false, 255);
        }
        int storeX         = entity->position.x;
        int storeY         = entity->position.y;
        entity->position.x = entity->sawPos[sawID].x;
        entity->position.y = entity->sawPos[sawID].y;
        RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
        Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
        entity->sawAngles[sawID] &= 0x1F0;
        entity->field_A0[sawID]  = 160;
        entity->sawTimers[sawID] = 180;
        RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &entity->sawAnimator[sawID], true, 0);
        entity->stateSaw[sawID] = Shiversaw_StateSaw_Destroyed;
        entity->state           = Shiversaw_State_HitRecoil;
        entity->position.x      = storeX;
        entity->position.y      = storeY;
        return true;
    }
#endif
    return Player_CheckHit(player, &entity->sawPos[sawID]);
}

void Shiversaw_CheckPlayerCollisions(void)
{
    RSDK_THIS(Shiversaw);
    if (!Shiversaw->invincibilityTimer) {
        foreach_active(Player, player)
        {
            for (int i = 0; i < Shiversaw_SawCount; ++i) {
                if (entity->sawAnimator[i].animationID == 3) {
                    if (Player_CheckCollisionTouch(player, &entity->sawPos[i], &Shiversaw->hitbox2)) {
                        Shiversaw_CheckSawHit(player, i);
                    }
                }
            }

            if (Player_CheckBadnikHit(player, entity, &Shiversaw->hitbox1) && Player_CheckBossHit(player, entity)) {
                if (Shiversaw->health)
                    Shiversaw->health--;

                if (Shiversaw->health) {
                    RSDK.PlaySfx(Shiversaw->sfxHit, false, 255);
                    Shiversaw->invincibilityTimer = 30;
                    if (entity->state != Shiversaw_State_HitRecoil) {
                        entity->timer = 120;
                        entity->state = Shiversaw_State_HitRecoil;
                    }
                }
                else {
                    RSDK.PlaySfx(Shiversaw->sfxExplosion2, false, 255);
                    Shiversaw->invincibilityTimer = 30;
                    int v14                       = entity->position.x;
                    int v15                       = entity->position.y;
                    entity->sawID                 = 0;
                    for (int i = 0; i < Shiversaw_SawCount; ++i) {
                        if (entity->stateSaw[entity->sawID] != Shiversaw_StateSaw_Destroyed) {
                            RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                            entity->position.x = entity->sawPos[entity->sawID].x;
                            entity->position.y = entity->sawPos[entity->sawID].y;
                            Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
                            entity->sawAngles[entity->sawID] &= 0x1F0;
                            entity->field_A0[entity->sawID] = 160;
                            RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &entity->sawAnimator[entity->sawID], true, 0);
                            entity->stateSaw[entity->sawID] = Shiversaw_StateSaw_Destroyed;
                        }
                        entity->sawTimers[entity->sawID++] = 0x7FFFFFFF;
                    }

                    entity->position.x          = v14;
                    entity->position.y          = v15;
                    entity->timer               = 120;
                    entity->state               = Shiversaw_State_Destroyed;
                    RSDK_sceneInfo->timeEnabled = false;
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
    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        if (i & 1)
            entity->sawPos[1].x = entity->position.x - 0x210000;
        else
            entity->sawPos[i].x = entity->position.x + 0x210000;
        entity->sawPos[i].y = entity->position.y + 0x70000;
        entity->sawPos[i].x += (((entity->field_90[i] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512((i << 8) + entity->sawAngles[i]);
        entity->sawPos[i].y += (((entity->field_90[i] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512((i << 8) + entity->sawAngles[i]);
    }
}

void Shiversaw_Unknown4(void)
{
    RSDK_THIS(Shiversaw);
    if (!(Zone->timer % 3)) {
        if (entity->state != Shiversaw_Unknown11)
            RSDK.PlaySfx(Shiversaw->sfxExplosion2, false, 255);

        if (Zone->timer & 4) {
            int x = RSDK.Rand(-19, 20) << 16;
            int y = RSDK.Rand(-24, 25) << 16;
            EntityExplosion *explosion =
                CREATE_ENTITY(Explosion, intToVoid((RSDK.Rand(0, 256) > 192) + 2), entity->position.x + x, entity->position.y + y);
            explosion->drawOrder = Zone->drawOrderHigh + 2;
        }
    }
}

void Shiversaw_Unknown5(EntityPlayer *player)
{
    RSDK_THIS(Shiversaw);

    entity->position.y = entity->field_88.y;
    if (entity->state != Shiversaw_State_Unknown1) {
        foreach_all(InvisibleBlock, block)
        {
            if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, entity, &Shiversaw->hitboxT)) {
                if (entity->velocity.x < 0) {
                    entity->velocity.x = 0;
                }
                else {
                    entity->velocity.x = 0x8000;
                    entity->field_B8   = entity->velocity.x;
                }
            }
            else if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, entity, &Shiversaw->hitboxL)) {
                if (entity->velocity.x > 0) {
                    entity->velocity.x = 0;
                }
                else {
                    entity->velocity.x = -0x8000;
                    entity->field_B8   = entity->velocity.x;
                }
            }
            else if (entity->velocity.x) {
                entity->field_B8 = entity->velocity.x;
            }

            if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, entity, &Shiversaw->hitboxR)) {
                if (entity->velocity.y < 0)
                    entity->velocity.y = 0;
                else
                    entity->velocity.y = 0x8000;
            }
            else if (RSDK.CheckObjectCollisionTouchBox(block, &block->hitbox, entity, &Shiversaw->hitboxB)) {
                if (entity->velocity.y > 0)
                    entity->velocity.y = 0;
                else
                    entity->velocity.y = -0x8000;
            }
        }
    }

    byte flags = 0;
    foreach_all(Crate, crate)
    {
        if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, entity, &Shiversaw->hitboxT) && !(flags & 1)) {
            if (player->position.x >= entity->position.x) {
                if (entity->velocity.x >= 0 && entity->velocity.x < 0x8000) {
                    entity->velocity.x = 0x8000;
                    flags |= 1;
                }
                else if (entity->velocity.x < 0) {
                    entity->velocity.x = 0;
                    flags |= 1;
                }
            }
            else {
                if (Shiversaw->timer > 0 && entity->stateSaw[1] == Shiversaw_StateSaw_Unknown2) {
                    entity->sawTimers[1] = 0;
                    entity->field_74[1]  = -0x20000;
                    RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                    entity->stateSaw[1] = Shiversaw_StateSaw_Unknown4;
                }
                entity->velocity.x = 0;
                flags |= 1;
            }
        }
        else if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, entity, &Shiversaw->hitboxL) && !(flags & 2)) {
            if (player->position.x <= entity->position.x) {
                if (entity->velocity.x <= 0 && entity->velocity.x > -0x8000) {
                    entity->velocity.x = -0x8000;
                }
                else if (entity->velocity.x > 0) {
                    entity->velocity.x = 0;
                }
            }
            else {
                if (Shiversaw->timer > 0 && entity->stateSaw[0] == Shiversaw_StateSaw_Unknown2) {
                    entity->sawTimers[0] = 0;
                    entity->field_74[0]  = -0x20000;
                    RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                    entity->stateSaw[0] = Shiversaw_StateSaw_Unknown4;
                }
                entity->velocity.x = 0;
            }
            flags |= 2;
        }

        if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, entity, &Shiversaw->hitboxR) && !(flags & 4)) {
            if (entity->velocity.y >= 0 && entity->velocity.y < 0x8000) {
                entity->velocity.y = 0x8000;
            }
            else if (entity->velocity.y < 0) {
                entity->velocity.y = 0;
            }
            flags |= 4;
        }
        else if (RSDK.CheckObjectCollisionTouchBox(crate, &crate->hitbox, entity, &Shiversaw->hitboxB) && !(flags & 8)) {
            if (entity->velocity.y <= 0 && entity->velocity.y > -0x8000) {
                entity->velocity.y = -0x8000;
            }
            else if (entity->velocity.y > 0) {
                entity->velocity.y = 0;
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
    if (++entity->timer >= 2) {
#if RETRO_USE_PLUS
        foreach_active(Player, player)
        {
            if (player->velocity.x <= 0) {
                Player_CheckCollisionBox(player, entity, &Shiversaw->hitbox7);
                if (player->velocity.x < 0)
                    continue;
            }
            Player_CheckCollisionBox(player, entity, &Shiversaw->hitbox8);
        }

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->objectID == Player->objectID) {
            if (player1->position.x < entity->position.x - 0x380000) {
                if (player1->position.y > entity->position.y + 0x400000 && player1->onGround) {
                    entity->timer               = 0;
                    Zone->playerBoundActiveR[0] = true;
                    Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 416;
                    Zone->playerBoundActiveB[0] = true;
                    Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 128;
                    entity->position.y -= 0x1000000;
                    entity->position.x -= 0x1000000;
                    Shiversaw_Unknown3();
                    entity->active           = ACTIVE_NORMAL;
                    entity->visible          = true;
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
                    entity->state = Shiversaw_State_SetupBounds;
                }
                else if (player->position.x < entity->position.x - 0x380000) {
                    if (player1->position.y > entity->position.y - 0x1000000) {
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
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + 416;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 128;
        entity->position.y -= 0x1000000;
        entity->position.x -= 0x1000000;
        Shiversaw_Unknown3();
        entity->active  = ACTIVE_NORMAL;
        entity->visible = true;
        entity->state   = Shiversaw_State_SetupBounds;
#endif
    }
}

void Shiversaw_State_SetupBounds(void)
{
    RSDK_THIS(Shiversaw);
#if RETRO_USE_PLUS
    if (++entity->timer >= 60) {
        entity->timer               = 0;
#endif
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = RSDK_screens->position.x;
        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (player1->position.x > entity->position.x + 0x500000) {
            if (player1->position.y > entity->field_84) {
                Zone->playerBoundActiveL[0] = true;
                Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - 416;
                entity->state               = Shiversaw_State_Unknown1;
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

    entity->position.y += 0x30000;
    entity->field_88.x = entity->position.x;
    entity->field_88.y = entity->position.y;
    Shiversaw_Unknown3();

    if (entity->position.y >= entity->field_84) {
        entity->position.y = entity->field_84;
        entity->angle      = 0;
        entity->timer      = 120;
        entity->state      = Shiversaw_State_Unknown2;
    }

    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        entity->sawID = i;
        StateMachine_Run(entity->stateSaw[i]);
    }
}

void Shiversaw_Unknown9(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_88.y) & 0xFFFF0000;

    int id = 0;
    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        entity->sawID = i;
        if (entity->stateSaw[i]) {
            if (entity->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
                entity->sawTimers[i] = 0;
            StateMachine_Run(entity->stateSaw[i]);
        }
        if (entity->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
            id++;
    }

    if (id == Shiversaw_SawCount) {
        entity->timer = 120;
        entity->state = Shiversaw_State_Unknown2;
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
    if (entity->animator2.frameID != 7)
        RSDK.ProcessAnimation(&entity->animator2);

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_88.y) & 0xFFFF0000;
    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        entity->sawID = i;
        StateMachine_Run(entity->stateSaw[i]);
    }

    Shiversaw_CheckPlayerCollisions();
    if (!--entity->timer)
        entity->state = Shiversaw_State_Unknown3;
}

void Shiversaw_State_Unknown3(void)
{
    RSDK_THIS(Shiversaw);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    entity->velocity.x    = 0;
    entity->velocity.y    = 0;

    bool32 flag = true;
    for (int i = 0; i < Shiversaw_SawCount; ++i) flag &= entity->stateSaw[i] == Shiversaw_StateSaw_Unknown2;

    if (flag) {
        int x = entity->position.x;
        int y = entity->field_88.y;

        int distX = entity->position.x - player1->position.x;
        if (distX >= 0) {
            if (distX < 0x380000)
                entity->velocity.x = 0xC000;
            else if (distX > 0x390000)
                entity->velocity.x = -0xC000;
        }
        else if (distX <= -0x380000) {
            if (distX < -0x390000)
                entity->velocity.x = 0xC000;
        }
        else
            entity->velocity.x = -0xC000;

        int distY = entity->field_88.y - player1->position.y;
        if (distY < 0) {
            if (distY <= -0x540000) {
                if (distY < -0x550000)
                    entity->velocity.y = 0xC000;
            }
            else
                entity->velocity.y = -0xC000;
        }
        else if (distY >= 0x540000) {
            if (distY > 0x550000)
                entity->velocity.y = -0xC000;
        }
        else
            entity->velocity.y = 0xC000;

        entity->position.x += entity->velocity.x;
        entity->field_88.y += entity->velocity.y;
        Shiversaw_Unknown5(player1);
        entity->position.x = x + entity->velocity.x;
        entity->field_88.y = y + entity->velocity.y;
    }

    if (entity->velocity.x || entity->velocity.y) {
        entity->sawTimers[0] = 0;
        entity->sawTimers[1] = 0;
    }

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_88.y) & 0xFFFF0000;
    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        entity->sawID = i;
        StateMachine_Run(entity->stateSaw[i]);
    }
    Shiversaw_CheckPlayerCollisions();
}

void Shiversaw_State_HitRecoil(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&entity->animator2);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (entity->position.x <= player1->position.x)
        entity->velocity.x = -0x1000 * (11 - Shiversaw->health);
    else
        entity->velocity.x = (11 - Shiversaw->health) << 12;

    entity->velocity.y = (11 - Shiversaw->health) << 12;
    if (entity->position.y <= player1->position.y)
        entity->velocity.y = -entity->velocity.y;

    int x = entity->position.x;
    int y = entity->field_88.y;
    entity->position.x += entity->velocity.x;
    entity->field_88.y += entity->velocity.y;
    Shiversaw_Unknown5(player1);
    entity->position.x = x + entity->velocity.x;
    entity->field_88.y = y + entity->velocity.y;

    entity->angle      = (entity->angle + 2) & 0xFF;
    entity->position.y = ((RSDK.Sin256(entity->angle) << 10) + entity->field_88.y) & 0xFFFF0000;

    int id = 0;
    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        entity->sawID = i;
        if (entity->stateSaw[i]) {
            if (entity->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
                entity->sawTimers[i] = 0;
            StateMachine_Run(entity->stateSaw[i]);
        }
        if (entity->stateSaw[i] != Shiversaw_StateSaw_Destroyed)
            id++;
    }

    if (entity->timer) {
        entity->timer--;
        Shiversaw_CheckPlayerCollisions();
    }
    else {
        if (id == Shiversaw_SawCount) {
            entity->timer = 120;
            entity->state = Shiversaw_State_Unknown2;
        }
        Shiversaw_CheckPlayerCollisions();
    }
}

void Shiversaw_State_Destroyed(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&entity->animator2);

    for (int i = 0; i < Shiversaw_SawCount; ++i) {
        entity->sawID = i;
        StateMachine_Run(entity->stateSaw[i]);
    }

    Shiversaw_Unknown4();
    if (!(Zone->timer % 5))
        Camera_ShakeScreen(1, 0, 1);

    if (--entity->timer <= 0) {
        if (Zone->stageFinishCallback) {
            Music_SetMusicTrack("ShiversawExplosion.ogg", TRACK_EGGMAN1, false);
            Music_PlayTrack(TRACK_EGGMAN1);
            entity->timer     = 0;
            entity->drawOrder = Zone->playerDrawHigh;
            entity->state     = Shiversaw_Unknown11;
        }
        else {
            entity->timer        = 120;
            entity->circleRadius = 0;
            entity->velocity.y   = -0x20000;
            entity->state        = Shiversaw_Unknown12;
        }
    }
}

void Shiversaw_Unknown10(void)
{
    RSDK_THIS(Shiversaw);
    StateMachine_Run(Zone->stageFinishCallback);
    entity->state = StateMachine_None;
}

void Shiversaw_Unknown11(void)
{
    RSDK_THIS(Shiversaw);
    RSDK.ProcessAnimation(&entity->animator2);
    Shiversaw_Unknown4();
    if (!(Zone->timer % 5))
        Camera_ShakeScreen(2, 0, 2);

    if (entity->timer < 120) {
        entity->circleRadius = 6 * entity->timer;
        entity->timer++;
    }
    else {
        foreach_all(SignPost, signPost)
        {
            signPost->active     = ACTIVE_NORMAL;
            signPost->position.x = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
            signPost->position.y = (RSDK_screens->position.y - 64) << 16;
        }
        entity->state = Shiversaw_Unknown10;
    }
}

void Shiversaw_Unknown12(void)
{
    RSDK_THIS(Shiversaw);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;
    Shiversaw_Unknown4();
    globals->enableIntro      = false;
    globals->atlEnabled       = false;
    Zone->stageFinishCallback = StateMachine_None;
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->timer = 0;
        Music_TransitionTrack(TRACK_STAGE, 0.0125);
        entity->state = Shiversaw_Unknown13;
    }
}

void Shiversaw_Unknown13(void)
{
    RSDK_THIS(Shiversaw);
    if (++entity->timer == 48) {
        foreach_all(SignPost, signPost)
        {
            signPost->position.x = entity->position.x;
            signPost->position.y = (RSDK_screens->position.y - 64) << 16;
            signPost->state      = SignPost_State_Fall;
            RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        }
        destroyEntity(entity);
    }
}

void Shiversaw_CheckCrateCollisions(void)
{
    RSDK_THIS(Shiversaw);
    int sawID  = entity->sawID;
    int storeX = entity->position.x;
    int storeY = entity->position.y;

    entity->position.x = entity->sawPos[sawID].x;
    entity->position.y = entity->sawPos[sawID].y;

    bool32 flag = false;
    if (entity->state != Shiversaw_State_Unknown1) {
        foreach_all(InvisibleBlock, block)
        {
            if (RSDK.CheckObjectCollisionTouchBox(entity, &Shiversaw->hitbox2, block, &block->hitbox)) {
                RSDK.PlaySfx(Ice->sfxWindowShatter, false, 255);
                Ice_ShatterGenerator(24, 24, 32, 0, 0, 0);
                entity->sawAngles[entity->sawID] &= 0x1F0;
                entity->field_A0[entity->sawID]  = 160;
                entity->sawTimers[entity->sawID] = 180;
                RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &entity->sawAnimator[entity->sawID], true, 0);
                entity->stateSaw[entity->sawID] = Shiversaw_StateSaw_Destroyed;
                if (entity->state == Shiversaw_State_Unknown1)
                    entity->state = Shiversaw_Unknown9;
                else
                    entity->state = Shiversaw_State_HitRecoil;
                flag = true;
                foreach_break;
            }
        }
    }

    if (!flag) {
        foreach_all(Crate, crate)
        {
            if (RSDK.CheckObjectCollisionTouchBox(entity, &crate->hitbox, crate, &crate->hitbox)) {
                RSDK.StopSFX(Shiversaw->sfxTargeting);
                if (!crate->animator.frameID) {
                    Crate_Break(crate);
                    if (entity->stateSaw[sawID] == Shiversaw_StateSaw_Unknown4)
                        entity->stateSaw[sawID] = Shiversaw_StateSaw_Unknown5;
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
                    entity->sawAngles[entity->sawID] &= 0x1F0;
                    entity->field_A0[entity->sawID]  = 160;
                    entity->sawTimers[entity->sawID] = 180;
                    RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 2, &entity->sawAnimator[entity->sawID], true, 0);
                    entity->stateSaw[entity->sawID] = Shiversaw_StateSaw_Destroyed;
                    if (entity->state == Shiversaw_State_Unknown1)
                        entity->state = Shiversaw_Unknown9;
                    else
                        entity->state = Shiversaw_State_HitRecoil;
                    foreach_break;
                }
            }
        }
    }
    entity->position.x = storeX;
    entity->position.y = storeY;
}

void Shiversaw_Unknown14(int speed)
{
    RSDK_THIS(Shiversaw);
    int sawID             = entity->sawID;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    int storeX         = entity->position.x;
    int storeY         = entity->position.y;
    entity->position.x = entity->sawPos[sawID].x;
    entity->position.y = entity->sawPos[sawID].y;
    entity->position.x = storeX;
    entity->position.y = storeY;
    if (entity->stateSaw[sawID] == Shiversaw_StateSaw_Destroyed) {
        if (entity->sawAngles[sawID] == entity->field_A0[sawID]) {
            if (entity->field_A0[sawID] == 160)
                entity->field_A0[sawID] = 352;
            else
                entity->field_A0[sawID] = 160;
        }
    }
    else {
        int angle               = RSDK.ATan2(entity->sawPos[sawID].x - player1->position.x, entity->sawPos[sawID].y - player1->position.y);
        entity->field_A0[sawID] = (2 * (angle - (~sawID << 7))) & 0x1FC;
    }

    if (entity->sawAngles[sawID] != entity->field_A0[sawID]) {
        if (((entity->sawAngles[sawID] - 256) & 0x1FF) >= ((entity->field_A0[sawID] - 256) & 0x1FF)) {
            if (entity->sawAngles[sawID] <= 160 || entity->sawAngles[sawID] > 352)
                entity->sawAngles[sawID] -= speed;
        }
        else if (entity->sawAngles[sawID] < 160 || entity->sawAngles[sawID] >= 352) {
            entity->sawAngles[sawID] += speed;
        }
        entity->sawAngles[entity->sawID] &= 0x1FF;
    }
}

void Shiversaw_StateSaw_Unknown2(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;
    RSDK.ProcessAnimation(&entity->sawAnimator[sawID]);

    if (sawID & 1)
        entity->sawPos[sawID].x = entity->position.x - 0x210000;
    else
        entity->sawPos[sawID].x = entity->position.x + 0x210000;
    entity->sawPos[sawID].y = entity->field_88.y + 0x70000;
    Shiversaw_Unknown14(4);
    entity->sawPos[sawID].y = entity->position.y + 0x70000;

    entity->sawPos[sawID].x += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(entity->sawAngles[sawID] + (sawID << 8));
    entity->sawPos[sawID].y += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(entity->sawAngles[sawID] + (sawID << 8));

    if (entity->sawAngles[sawID] == entity->field_A0[sawID]) {
        if (entity->stateSaw[sawID ^ 1] == Shiversaw_StateSaw_Unknown2) {
            ++entity->sawTimers[sawID];
            if (entity->sawTimers[sawID] >= 30) {
                entity->sawTimers[sawID] = 0;
                entity->field_74[sawID]  = -0x20000;
                RSDK.PlaySfx(Shiversaw->sfxTargeting, false, 255);
                entity->stateSaw[sawID] = Shiversaw_StateSaw_Unknown3;
            }
        }
    }
    else {
        entity->sawTimers[sawID] = 0;
    }
}

void Shiversaw_StateSaw_Unknown3(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;
    RSDK.ProcessAnimation(&entity->sawAnimator[sawID]);

    if (entity->field_74[sawID] < 0) {
        entity->field_74[sawID] += 0x3000;
        entity->field_90[sawID] += entity->field_74[sawID];
    }

    if (sawID & 1)
        entity->sawPos[sawID].x = entity->position.x - 0x210000;
    else
        entity->sawPos[sawID].x = entity->position.x + 0x210000;
    Shiversaw_Unknown14(1);
    entity->sawPos[sawID].y = entity->position.y + 0x70000;

    entity->sawPos[sawID].x += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(entity->sawAngles[sawID] + (sawID << 8));
    entity->sawPos[sawID].y += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(entity->sawAngles[sawID] + (sawID << 8));

    ++entity->sawTimers[sawID];
    if (entity->sawTimers[sawID] >= 45) {
        RSDK.StopSFX(Shiversaw->sfxTargeting);
        RSDK.PlaySfx(Shiversaw->sfxRocketJet, false, 255);
        entity->stateSaw[sawID] = Shiversaw_StateSaw_Unknown4;
    }
}

void Shiversaw_StateSaw_Unknown4(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;
    RSDK.ProcessAnimation(&entity->sawAnimator[sawID]);

    if (entity->field_74[sawID] < 0x40000)
        entity->field_74[sawID] += 0x3000;

    entity->field_90[sawID] += entity->field_74[sawID];

    if (entity->field_90[sawID] >= 0x280000) {
        entity->sawTimers[sawID]        = 0;
        entity->stateSaw[entity->sawID] = Shiversaw_StateSaw_Unknown5;
    }

    Shiversaw_ProcessSawMovement(sawID);
    Shiversaw_CheckCrateCollisions();
}

void Shiversaw_StateSaw_Unknown5(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;

    RSDK.ProcessAnimation(&entity->sawAnimator[sawID]);

    if (entity->field_74[sawID] > -0x80000)
        entity->field_74[sawID] -= 0x6000;

    if (sawID & 1)
        entity->sawPos[sawID].x = entity->position.x - 0x210000;
    else
        entity->sawPos[sawID].x = entity->position.x + 0x210000;
    entity->sawPos[entity->sawID].y = entity->position.y + 0x70000;
    entity->field_90[sawID] += entity->field_74[sawID];
    if (entity->field_90[sawID] <= 0) {
        entity->field_90[sawID] = 0;
        entity->sawAngles[sawID] &= 0x1FC;
        entity->stateSaw[sawID] = Shiversaw_StateSaw_Unknown2;
    }

    entity->sawPos[sawID].x += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(entity->sawAngles[sawID] + (sawID << 8));
    entity->sawPos[sawID].y += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(entity->sawAngles[sawID] + (sawID << 8));
    Shiversaw_CheckCrateCollisions();
}

void Shiversaw_StateSaw_Destroyed(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;
    if (sawID & 1)
        entity->sawPos[sawID].x = entity->position.x - 0x210000;
    else
        entity->sawPos[sawID].x = entity->position.x + 0x210000;
    entity->sawPos[sawID].y = entity->position.y + 0x70000;
    Shiversaw_Unknown14(16);
    entity->field_90[sawID] -= 0x80000;
    if (entity->field_90[sawID] <= 0)
        entity->field_90[sawID] = 0;
    entity->sawPos[sawID].x += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(entity->sawAngles[sawID] + (sawID << 8));
    entity->sawPos[sawID].y += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(entity->sawAngles[sawID] + (sawID << 8));

    if (!(Zone->timer & 3)) {
        EntityDust *dust = CREATE_ENTITY(Dust, entity, entity->sawPos[entity->sawID].x, entity->sawPos[entity->sawID].y);
        dust->velocity.x = RSDK.Rand(-4, 5) << 15;
        dust->velocity.y = RSDK.Rand(-4, 5) << 15;
        dust->inkEffect  = INK_ALPHA;
        dust->drawOrder  = entity->drawOrder - 1;
        dust->alpha      = 128;
        dust->state      = Shiversaw_StateDust_Unknown;
    }
    if (!(Zone->timer & 0xF)) {
        EntityIce *ice                = CREATE_ENTITY(Ice, intToVoid(3), entity->sawPos[entity->sawID].x, entity->sawPos[entity->sawID].y);
        ice->velocity.x               = RSDK.Rand(-6, 8) << 15;
        ice->velocity.y               = RSDK.Rand(-10, 2) << 15;
        ice->direction                = RSDK.Rand(0, 4);
        ice->animator1.animationSpeed = RSDK.Rand(1, 4);
        ice->drawOrder                = entity->drawOrder - 1;
    }

    --entity->sawTimers[sawID];
    if (!entity->sawTimers[sawID]) {
        entity->sawTimers[sawID] = 30;
        entity->stateSaw[sawID]  = Shiversaw_StateSaw_Unknown7;
    }
}

void Shiversaw_StateSaw_Unknown7(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;
    RSDK.ProcessAnimation(&entity->sawAnimator[sawID]);
    Shiversaw_ProcessSawMovement(sawID);
    if (entity->sawAnimator[sawID].frameID == entity->sawAnimator[sawID].frameCount - 1) {
        RSDK.SetSpriteAnimation(Shiversaw->aniFrames, 3, &entity->sawAnimator[sawID], true, 0);
        entity->stateSaw[sawID] = Shiversaw_StateSaw_Unknown2;
    }
}

void Shiversaw_StateSaw_Unknown1(void)
{
    RSDK_THIS(Shiversaw);
    int sawID = entity->sawID;
    if (entity->state == Shiversaw_State_Unknown1) {
        RSDK.ProcessAnimation(&entity->sawAnimator[sawID]);
        Shiversaw_ProcessSawMovement(sawID);
        Shiversaw_CheckCrateCollisions();
    }
    else {
        entity->stateSaw[sawID] = Shiversaw_StateSaw_Unknown2;
        Shiversaw_StateSaw_Unknown2();
    }
}

void Shiversaw_ProcessSawMovement(int sawID)
{
    RSDK_THIS(Shiversaw);
    if (sawID & 1)
        entity->sawPos[sawID].x = entity->position.x - 0x210000;
    else
        entity->sawPos[sawID].x = entity->position.x + 0x210000;
    entity->sawPos[sawID].y = entity->position.y + 0x70000;
    entity->sawPos[sawID].x += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Cos512(entity->sawAngles[sawID] + (sawID << 8));
    entity->sawPos[sawID].y += (((entity->field_90[sawID] >> 9) & 0xFFFFFF80) + 0x11B0) * RSDK.Sin512(entity->sawAngles[sawID] + (sawID << 8));
}

void Shiversaw_StateDust_Unknown(void)
{
    RSDK_THIS(Dust);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->visible ^= 1;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        destroyEntity(entity);
}

void Shiversaw_EditorDraw(void) {}

void Shiversaw_EditorLoad(void) {}

void Shiversaw_Serialize(void) {}
