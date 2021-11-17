#include "SonicMania.h"

ObjectGachapandora *Gachapandora;

void Gachapandora_Update(void)
{
    RSDK_THIS(Gachapandora);
    StateMachine_Run(entity->state);
}

void Gachapandora_LateUpdate(void) {}

void Gachapandora_StaticUpdate(void) { RSDK.AddDrawListRef(Zone->drawOrderLow + 1, RSDK.GetEntityID(Gachapandora->eggman)); }

void Gachapandora_Draw(void)
{
    RSDK_THIS(Gachapandora);
    StateMachine_Run(entity->stateDraw);
}

void Gachapandora_Create(void *data)
{
    RSDK_THIS(Gachapandora);

    entity->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            entity->active = ACTIVE_BOUNDS;
            if (data)
                entity->type = voidToInt(data);

            switch (entity->type) {
                case GACHAPANDORA_MAIN:
                    entity->visible   = false;
                    entity->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &entity->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &entity->animator2, true, 0);
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &entity->animator3, true, 7);
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &entity->animator4, true, 7);

                    entity->hitbox.left   = -33;
                    entity->hitbox.top    = -37;
                    entity->hitbox.right  = 33;
                    entity->hitbox.bottom = 6;
                    entity->startY        = entity->position.y;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    Gachapandora->eggman  = (Entity *)entity;
                    int val               = 55;

                    for (int i = Gachapandora_CapsuleCount - 1; i >= 0; --i) {
                        int id = 0;
                        do {
                            do {
                                Gachapandora->capsuleTypeL[i] = RSDK.Rand(0, 6);
                                id                            = Gachapandora->capsuleTypeL[i] % 3;
                            } while (id == Gachapandora->value19);
                        } while (Gachapandora->value20[id] == 3);

                        Gachapandora->value19 = Gachapandora->capsuleTypeL[i] % 3;
                        ++Gachapandora->value20[Gachapandora->value19];
                        Gachapandora->capsuleOffsetL[i] = 4 * val;
                        Gachapandora->capsuleMaxL[i]    = 4 * val;

                        do {
                            do {
                                Gachapandora->capsuleTypeR[i] = RSDK.Rand(0, 6);
                                id                            = Gachapandora->capsuleTypeR[i] % 3;
                            } while (id == Gachapandora->value19 && i);
                        } while (Gachapandora->value20[id] == 3);

                        Gachapandora->value19 = Gachapandora->capsuleTypeR[i] % 3;
                        ++Gachapandora->value20[Gachapandora->value19];
                        Gachapandora->capsuleOffsetR[i] = 4 * val;
                        Gachapandora->capsuleMaxR[i]    = 4 * val;

                        val -= 18;
                    }

                    Gachapandora->nextChildType = 0;
                    Gachapandora->value15       = 0x7FFF;
                    Gachapandora->nextCapsuleL  = 3;
                    Gachapandora->nextCapsuleR  = 3;
                    Gachapandora->capsuleSide   = RSDK.Rand(0, 2);
                    if (Gachapandora->capsuleSide == 1) {
                        for (int i = 0; i < Gachapandora_CapsuleCount; ++i) {
                            int store                     = Gachapandora->capsuleTypeL[i];
                            Gachapandora->capsuleTypeL[i] = Gachapandora->capsuleTypeR[i];
                            Gachapandora->capsuleTypeR[i] = store;
                        }
                    }
                    entity->state     = Gachapandora_State_SetupArena;
                    entity->stateDraw = Gachapandora_StateDraw_Unknown1;
                    break;
                case GACHAPANDORA_1:
                    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 4, &entity->animator1, true, 0);
                    entity->state         = Gachapandora_State1_Unknown1;
                    entity->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    break;
                case GACHAPANDORA_PRIZE:
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &entity->animator1, true, 0);
                    entity->state         = Gachapandora_State2_Unknown1;
                    entity->stateDraw     = Gachapandora_StateDraw2_Unknown;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    break;
                case GACHAPANDORA_FIREBALL:
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 11, &entity->animator1, true, 0);
                    entity->hitbox.left   = -2;
                    entity->hitbox.top    = -2;
                    entity->hitbox.right  = 2;
                    entity->hitbox.bottom = 2;
                    entity->state         = Gachapandora_State6_Unknown1;
                    entity->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    entity->updateRange.x = 0x800000;
                    entity->updateRange.y = 0x800000;
                    entity->active        = ACTIVE_NORMAL;
                    entity->visible       = true;
                    entity->drawOrder     = Zone->drawOrderLow;
                    break;
                case GACHAPANDORA_DEBRIS:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &entity->animator1, true, 0);
                    entity->state         = Gachapandora_State7_Unknown1;
                    entity->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    entity->updateRange.y = 0x800000;
                    entity->updateRange.x = 0x800000;
                    break;
                case GACHAPANDORA_SPARK:
                    entity->active    = ACTIVE_NORMAL;
                    entity->visible   = true;
                    entity->drawOrder = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 13, &entity->animator1, true, 0);
                    entity->state         = Gachapandora_State8_Unknown1;
                    entity->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    entity->updateRange.y = 0x800000;
                    entity->updateRange.x = 0x800000;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(entity);
        }
    }
}

void Gachapandora_StageLoad(void)
{
    Gachapandora->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Gachapandora.bin", SCOPE_STAGE);
    Gachapandora->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ2.bin", SCOPE_STAGE);

    Gachapandora->hitbox1.left   = -16;
    Gachapandora->hitbox1.top    = 22;
    Gachapandora->hitbox1.right  = 16;
    Gachapandora->hitbox1.bottom = 30;

    Gachapandora->hitbox2.left   = -128;
    Gachapandora->hitbox2.top    = -256;
    Gachapandora->hitbox2.right  = 128;
    Gachapandora->hitbox2.bottom = 256;

    Gachapandora->hitbox3.left   = -32;
    Gachapandora->hitbox3.top    = -32;
    Gachapandora->hitbox3.right  = 0;
    Gachapandora->hitbox3.bottom = 32;

    Gachapandora->hitbox4.left   = -5;
    Gachapandora->hitbox4.top    = -11;
    Gachapandora->hitbox4.right  = 5;
    Gachapandora->hitbox4.bottom = 0;

    Gachapandora->hitbox5.left   = -28;
    Gachapandora->hitbox5.top    = 3;
    Gachapandora->hitbox5.right  = -20;
    Gachapandora->hitbox5.bottom = 10;

    Gachapandora->hitbox6.left   = -2;
    Gachapandora->hitbox6.top    = 2;
    Gachapandora->hitbox6.right  = -2;
    Gachapandora->hitbox6.bottom = 2;

    Gachapandora->active     = ACTIVE_ALWAYS;
    Gachapandora->value26    = 0;
    Gachapandora->value27    = false;
    Gachapandora->activeToys = 0;
    Gachapandora->value19    = -1;
    Gachapandora->value20[0] = 0;
    Gachapandora->value20[1] = 0;
    Gachapandora->value20[2] = 0;

    Gachapandora->sfxHit       = RSDK.GetSFX("Stage/BossHit.wav");
    Gachapandora->sfxExplosion = RSDK.GetSFX("Stage/Explosion2.wav");
    Gachapandora->sfxUnravel   = RSDK.GetSFX("Stage/Unravel.wav");
    Gachapandora->sfxPon       = RSDK.GetSFX("Stage/Pon.wav");
    Gachapandora->sfxFireball  = RSDK.GetSFX("Stage/Fireball.wav");
    Gachapandora->sfxGiggle    = RSDK.GetSFX("MMZ/Giggle.wav");
}

void Gachapandora_CheckPlayerCollisions(void)
{
    RSDK_THIS(Gachapandora);

    foreach_active(Player, player)
    {
        if (player->playerAnimator.animationID != ANI_VICTORY) {
            if (Player_CheckBadnikTouch(player, entity, &entity->hitbox)) {
                int blink = player->blinkTimer;
                if (entity->type == GACHAPANDORA_AMY)
                    player->blinkTimer = 1;
                if (Player_CheckBossHit(player, entity)) {
                    if (entity->health)
                        entity->health--;

                    if (entity->health) {
                        RSDK.PlaySfx(Gachapandora->sfxHit, false, 255);
                    }
                    else {
                        EntityGachapandora *eggman = (EntityGachapandora *)Gachapandora->eggman;
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &eggman->animator2, true, 0);
                        RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
                        entity->state = Gachapandora_State_Destroyed;
                    }
                    entity->invincibilityTimer = 30;
                    if (entity->type == GACHAPANDORA_AMY)
                        player->blinkTimer = blink;
                    foreach_break;
                }
                else {
                    if (entity->type == GACHAPANDORA_AMY)
                        player->blinkTimer = blink;
                }
            }
            else {
                if (entity->type == GACHAPANDORA_DRILLER) {
                    if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox5))
                        Player_CheckHit(player, entity);
                }
            }
        }
    }
}

void Gachapandora_Explode(int xMax, int xMin, int yMin, int yMax)
{
    RSDK_THIS(Gachapandora);

    int interval = 4 * (entity->state == Gachapandora_State_Unknown5) + 3;
    if (!(Zone->timer % interval)) {
        RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
        if (Zone->timer & 4) {
            BadnikHelpers_HandleExplode(xMin, xMax, yMin, yMax, Zone->drawOrderHigh + 2);
        }
    }
}

void Gachapandora_Player_StateInput_P1Grabbed(void)
{
    RSDK_THIS(Player);
    Player_ProcessP1Input();
    entity->up         = false;
    entity->down       = false;
    entity->jumpPress  = false;
    entity->jumpHold   = false;
    entity->velocity.x = clampVal(entity->velocity.x, -0x20000, 0x20000);
    entity->groundVel  = clampVal(entity->groundVel, -0x20000, 0x20000);
}

void Gachapandora_Player_StateInput_P2PlayerGrabbed(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_Player();
    entity->up         = false;
    entity->down       = false;
    entity->jumpPress  = false;
    entity->jumpHold   = false;
    entity->velocity.x = clampVal(entity->velocity.x, -0x40000, 0x40000);
    entity->groundVel  = clampVal(entity->groundVel, -0x40000, 0x40000);
    if (entity->stateInput == Player_ProcessP2Input_AI)
        entity->stateInput = Gachapandora_Player_StateInput_P2AIGrabbed;
}

void Gachapandora_Player_StateInput_P2AIGrabbed(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_AI();
    entity->up         = false;
    entity->down       = false;
    entity->jumpPress  = false;
    entity->jumpHold   = false;
    entity->velocity.x = clampVal(entity->velocity.x, -0x40000, 0x40000);
    entity->groundVel  = clampVal(entity->groundVel, -0x40000, 0x40000);
    if (entity->stateInput == Player_ProcessP2Input_Player)
        entity->stateInput = Gachapandora_Player_StateInput_P2PlayerGrabbed;
}

void Gachapandora_HandleChildren(void)
{
    RSDK_THIS(Gachapandora);

    --Gachapandora->value22;
    if (!Gachapandora->value22) {
        switch (entity->health) {
            case 1:
            case 2: Gachapandora->value22 = RSDK.Rand(8, 16); break;
            case 3: Gachapandora->value22 = RSDK.Rand(15, 31); break;
            case 4: Gachapandora->value22 = RSDK.Rand(30, 61); break;
            case 5: Gachapandora->value22 = RSDK.Rand(60, 91); break;
            case 6:
            case 7: Gachapandora->value22 = RSDK.Rand(90, 121); break;
            case 8:
            case 9: return;
            default: break;
        }

        int type = RSDK.Rand(0, 3);
        switch (type) {
            case 2:
            case 0: {
                int off                   = RSDK.Rand(-12, 12);
                EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS),
                                                          entity->position.x + (off << 16) + entity->velocity.x, entity->position.y + 0x300000);
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, RSDK.Rand(0, 4) + 18, &child->animator1, true, 0);
                if (entity->state != Gachapandora_State_Unknown4 && !entity->invincibilityTimer)
                    child->velocity.x = entity->velocity.x >> 1;
                child->state = Gachapandora_State7_Unknown3;
                break;
            }
            case 1: {
                EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_SPARK), entity->position.x, entity->position.y);
                child->parent             = (Entity *)entity;
                if (RSDK.Rand(0, 2))
                    child->posUnknown.x = (RSDK.Rand(-6, 6) - 26) << 16;
                else
                    child->posUnknown.x = (RSDK.Rand(-6, 6) + 26) << 16;
                child->posUnknown.y = (RSDK.Rand(-8, 8) + 20) << 16;
                RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &child->animator1, true, 0);
                break;
            }
        }
    }

    if (entity->state == Gachapandora_State_Unknown8) {
        for (int i = 0; i < 2; ++i) {
            if (!--Gachapandora->value23[i]) {
                Gachapandora->value23[i]  = RSDK.Rand(16, 30);
                EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_SPARK), entity->position.x, entity->position.y);
                child->parent             = (Entity *)entity;
                if (i) {
                    child->posUnknown.x = (RSDK.Rand(-2, 2) - 34) << 16;
                }
                else {
                    child->direction    = FLIP_X;
                    child->posUnknown.x = (RSDK.Rand(-2, 2) + 34) << 16;
                }
                child->posUnknown.y = (RSDK.Rand(-8, 8) + 20) << 16;
                if (RSDK.Rand(0, 3) == 2)
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 14, &child->animator1, true, 0);
            }
        }
    }
}

void Gachapandora_HandleAnimations(void)
{
    RSDK_THIS(Gachapandora);

    if (entity->animator2.animationID) {
        if (entity->animator2.animationID == 1) {
            if (entity->animator2.frameID >= entity->animator2.frameCount - 1)
                RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &entity->animator2, true, 0);
        }
        else {
            if (entity->animator2.animationID == 2) {
                bool32 flag = false;
                foreach_active(Player, player)
                {
                    if (player->state == Player_State_Hit || player->state == Player_State_Die)
                        flag = true;
                }

                if (entity->animator2.frameID >= entity->animator2.frameCount - 1) {
                    if (flag)
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &entity->animator2, true, 6);
                    else
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &entity->animator2, true, 0);
                }
            }
        }
    }
    else {
        bool32 flag = false;
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hit || player->state == Player_State_Die)
                flag = true;
        }
        if (flag)
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &entity->animator2, true, 0);
    }
}

void Gachapandora_State_SetupArena(void)
{
    RSDK_THIS(Gachapandora);

    if (++entity->timer >= 2) {
        entity->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->screenBoundsR1[0]     = (entity->position.x >> 16) + ScreenInfo->centerX;
        Zone->screenBoundsB1[0]     = (entity->position.y >> 16) + 128;

        entity->position.y = (ScreenInfo->position.y - 192) << 16;
        entity->active     = ACTIVE_NORMAL;
        entity->visible    = true;
        entity->state      = Gachapandora_State_Unknown1;
    }
}

void Gachapandora_State_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    Zone->playerBoundActiveL[0] = true;
    Zone->screenBoundsL1[0]     = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->screenBoundsL1[0]     = (entity->position.x >> 16) - ScreenInfo->centerX;
        Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
        entity->health = 9;
        entity->state  = Gachapandora_State_Unknown2;
    }
}

void Gachapandora_State_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);

    entity->position.y = entity->position.y + 0x10000;
    if (entity->position.y < entity->startY) {
        if (entity->position.y >= entity->startY - 0x400000)
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &entity->animator2, false, 0);
    }
    else {
        entity->position.y   = entity->startY;
        entity->posUnknown.x = entity->position.x;
        entity->posUnknown.y = entity->position.y;
        entity->angle        = 0;
        entity->state        = Gachapandora_State_Unknown3;
    }
    RSDK.AddDrawListRef(Zone->drawOrderLow + 1, SceneInfo->entitySlot);
}

void Gachapandora_State_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    Gachapandora_HandleAnimations();
    entity->position.y = BadnikHelpers_Oscillate(entity->posUnknown.y, 2, 10);

    if (entity->invincibilityTimer) {
        entity->invincibilityTimer--;
        if (!--entity->invincibilityTimer && !entity->velocity.x) {
            if (entity->direction == FLIP_NONE)
                entity->velocity.x = -0x8000;
            else
                entity->velocity.x = 0x8000;
        }
    }
    else {
        if (entity->position.x < (ScreenInfo->position.x + 64) << 16
            || entity->position.x > (ScreenInfo->position.x + ScreenInfo->width - 64) << 16)
            entity->velocity.x = -entity->velocity.x;
        entity->position.x += entity->velocity.x;
    }

    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->value4 ^= 1;

    for (int i = 0; i < Gachapandora_CapsuleCount; ++i) {
        if (Gachapandora->capsuleDelayL[i]) {
            Gachapandora->capsuleDelayL[i]--;
        }
        else if (Gachapandora->capsuleOffsetL[i] != Gachapandora->capsuleMaxL[i]) {
            ++Gachapandora->capsuleSpeedL[i];
            Gachapandora->capsuleOffsetL[i] += Gachapandora->capsuleSpeedL[i];
            if (Gachapandora->capsuleOffsetL[i] > Gachapandora->capsuleMaxL[i])
                Gachapandora->capsuleOffsetL[i] = Gachapandora->capsuleMaxL[i];
        }

        if (Gachapandora->capsuleDelayR[i]) {
            Gachapandora->capsuleDelayR[i]--;
        }
        else if (Gachapandora->capsuleOffsetR[i] != Gachapandora->capsuleMaxR[i]) {
            ++Gachapandora->capsuleSpeedR[i];
            Gachapandora->capsuleOffsetR[i] += Gachapandora->capsuleSpeedR[i];
            if (Gachapandora->capsuleOffsetR[i] > Gachapandora->capsuleMaxR[i])
                Gachapandora->capsuleOffsetR[i] = Gachapandora->capsuleMaxR[i];
        }
    }

    if (entity->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox1)) {
                RSDK.PlaySfx(Gachapandora->sfxUnravel, false, 255);
                if (player->position.x >= entity->position.x)
                    Gachapandora->handleDir = player->velocity.y < 0;
                else
                    Gachapandora->handleDir = player->velocity.y > 0;

                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &entity->animator3, true, 0);
                if (Gachapandora->nextCapsuleL + Gachapandora->nextCapsuleR == 0x1FE) {
                    entity->invincibilityTimer = 25;
                    entity->state              = Gachapandora_State_Unknown9;
                }
                else {
                    entity->timer = 60;
                    int id        = 0;
                    if (Gachapandora->capsuleSide) {
                        Gachapandora->value15                                    = 148;
                        Gachapandora->nextChildType                              = Gachapandora->capsuleTypeR[Gachapandora->nextCapsuleR];
                        Gachapandora->capsuleOffsetR[Gachapandora->nextCapsuleR] = 0xFF;
                        Gachapandora->capsuleMaxR[Gachapandora->nextCapsuleR--]  = 0xFF;

                        while (id <= (int8)Gachapandora->nextCapsuleR) {
                            Gachapandora->capsuleDelayR[id] = 8 * (Gachapandora->nextCapsuleR - id + 2);
                            Gachapandora->capsuleMaxR[id] += 72;
                            Gachapandora->capsuleSpeedR[id++] = 0;
                        }
                    }
                    else {
                        Gachapandora->value15                                    = -148;
                        Gachapandora->nextChildType                              = Gachapandora->capsuleTypeL[Gachapandora->nextCapsuleL];
                        Gachapandora->capsuleOffsetL[Gachapandora->nextCapsuleL] = 0xFF;
                        Gachapandora->capsuleMaxL[Gachapandora->nextCapsuleL--]  = 0xFF;

                        while (id <= (int8)Gachapandora->nextCapsuleL) {
                            Gachapandora->capsuleDelayL[id] = 8 * (Gachapandora->nextCapsuleL - id + 2);
                            Gachapandora->capsuleMaxL[id] += 72;
                            Gachapandora->capsuleSpeedL[id++] = 0;
                        }
                    }
                    Gachapandora->capsuleSide ^= 1;
                    if (entity->velocity.x >= 0)
                        entity->velocity.x += 0x4000;
                    else
                        entity->velocity.x -= 0x4000;
                    entity->state = Gachapandora_State_Unknown4;
                }
                foreach_break;
            }
        }
    }
    else {
        entity->timer--;
    }
}

void Gachapandora_State_Unknown4(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    Gachapandora_HandleAnimations();
    entity->position.y = BadnikHelpers_Oscillate(entity->posUnknown.y, 2, 10);
    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->value4 ^= 1;

    for (int i = 0; i < Gachapandora_CapsuleCount; ++i) {
        if (Gachapandora->capsuleDelayL[i]) {
            Gachapandora->capsuleDelayL[i]--;
        }
        else if (Gachapandora->capsuleOffsetL[i] != Gachapandora->capsuleMaxL[i]) {
            ++Gachapandora->capsuleSpeedL[i];
            Gachapandora->capsuleOffsetL[i] += Gachapandora->capsuleSpeedL[i];
            if (Gachapandora->capsuleOffsetL[i] > Gachapandora->capsuleMaxL[i])
                Gachapandora->capsuleOffsetL[i] = Gachapandora->capsuleMaxL[i];
        }

        if (Gachapandora->capsuleDelayR[i]) {
            Gachapandora->capsuleDelayR[i]--;
        }
        else if (Gachapandora->capsuleOffsetR[i] != Gachapandora->capsuleMaxR[i]) {
            ++Gachapandora->capsuleSpeedR[i];
            Gachapandora->capsuleOffsetR[i] += Gachapandora->capsuleSpeedR[i];
            if (Gachapandora->capsuleOffsetR[i] > Gachapandora->capsuleMaxR[i])
                Gachapandora->capsuleOffsetR[i] = Gachapandora->capsuleMaxR[i];
        }
    }

    if (Gachapandora->value15) {
        if (Gachapandora->value15 >= 0) {
            Gachapandora->value15 -= 3;
            if (Gachapandora->value15 < 0)
                Gachapandora->value15 = 0;
        }
        else {
            Gachapandora->value15 += 3;
            if (Gachapandora->value15 > 0)
                Gachapandora->value15 = 0;
        }
    }
    else {
        EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_PRIZE), entity->position.x, entity->position.y + 0x1A0000);
        child->type               = Gachapandora->nextChildType;
        switch (child->type) {
            case GACHAPANDORA_MAIN:
            case GACHAPANDORA_1: child->animator1.frameID = 0; break;
            case GACHAPANDORA_PRIZE: child->animator1.frameID = 1; break;
            case GACHAPANDORA_AMY:
            case GACHAPANDORA_DRILLER: child->animator1.frameID = 2; break;
            case GACHAPANDORA_FIREDROPPER: child->animator1.frameID = 3; break;
            default: break;
        }

        Gachapandora->value15 = 0x7FFF;
        if (--entity->health == 1) {
            child                    = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), entity->position.x, entity->position.y);
            child->animator1.frameID = 4;
            child->timer             = 30;
            child->velocity.x        = 0x20000;
            child->velocity.y        = -0x20000;

            child                    = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), entity->position.x, entity->position.y);
            child->animator1.frameID = 5;
            child->timer             = 30;
            child->velocity.x        = -0x20000;
            child->velocity.y        = -0x20000;

            child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), entity->position.x, entity->position.y);
            ++child->drawOrder;
            child->animator1.frameID = 2;
            child->timer             = 90;
            child->velocity.x        = -0x20000;
            child->velocity.y        = -0x20000;

            child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), entity->position.x, entity->position.y);
            ++child->drawOrder;
            child->animator1.frameID = 3;
            child->timer             = 90;
            child->velocity.x        = 0x20000;
            child->velocity.y        = -0x20000;

            entity->invincibilityTimer = 120;
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &entity->animator2, true, 0);
            RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator4, true, 0);
            entity->state     = Gachapandora_State_Unknown5;
            entity->stateDraw = Gachapandora_StateDraw_Unknown2;
        }
        else {
            entity->invincibilityTimer = 30;
            entity->state              = Gachapandora_State_Unknown3;
        }
    }
}

void Gachapandora_State_Unknown5(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    Gachapandora_Explode(24, -24, -48, 48);
    Gachapandora_HandleChildren();
    if (--entity->invincibilityTimer <= 0)
        entity->state = Gachapandora_State_Unknown6;
}

void Gachapandora_State_Unknown6(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    Gachapandora_HandleChildren();
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x1800;
    if (entity->position.y < (ScreenInfo->position.y - 192) << 16) {
        entity->position.y = (ScreenInfo->position.y - 192) << 16;
        entity->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        entity->state      = Gachapandora_State_Unknown7;
    }
}

void Gachapandora_State_Unknown7(void)
{
    RSDK_THIS(Gachapandora);

    if (Gachapandora->activeToys >= 8) {
        entity->posUnknown.y = entity->position.y;
        entity->angle        = 0;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 17, &entity->animator4, true, 0);
        entity->timer = 60;
        entity->velocity.x >>= 1;
        entity->state = Gachapandora_State_Unknown8;
    }
}

void Gachapandora_State_Unknown8(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    Gachapandora_HandleAnimations();

    if (entity->posUnknown.y >= entity->startY) {
        entity->position.y = BadnikHelpers_Oscillate(entity->posUnknown.y, 2, 10);
    }
    else {
        entity->posUnknown.y += 0x10000;
        entity->position.y = entity->posUnknown.y;
    }
    Gachapandora_HandleChildren();

    if (entity->position.x < (ScreenInfo->position.x + 32) << 16
        || entity->position.x > (ScreenInfo->position.x + ScreenInfo->width - 32) << 16)
        entity->velocity.x = -entity->velocity.x;
    entity->position.x = entity->position.x + entity->velocity.x;

    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->value4 ^= 1;

    if (--entity->timer <= 0) {
        entity->timer = 60;
        for (int i = 0; i < 2; ++i) {
            EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_SPARK), entity->position.x, entity->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 15, &child->animator1, true, 0);
            if (i) {
                child->position.x += (RSDK.Rand(-2, 2) - 34) << 16;
                child->velocity.x = entity->velocity.x - 0x10000;
            }
            else {
                child->direction = FLIP_X;
                child->position.x += (RSDK.Rand(-2, 2) + 34) << 16;
                child->velocity.x = entity->velocity.x + 0x10000;
            }

            child->state = Gachapandora_State8_Unknown2;
            child->position.y += (RSDK.Rand(-8, 8) + 20) << 16;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox1)) {
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 4, &entity->animator2, true, 0);
            RSDK.PlaySfx(Gachapandora->sfxUnravel, false, 255);
            if (player->position.x >= entity->position.x)
                Gachapandora->handleDir = player->velocity.y < 0;
            else
                Gachapandora->handleDir = player->velocity.y > 0;

            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &entity->animator3, true, 0);
            entity->invincibilityTimer = 25;
            entity->velocity.y         = 0;
            entity->state              = Gachapandora_State_Unknown9;
            foreach_break;
        }
    }
}

void Gachapandora_State_Unknown9(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_HandleChildren();
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x800;
    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    Gachapandora_HandleChildren();
    entity->position.y = BadnikHelpers_Oscillate(entity->posUnknown.y, 2, 10);
    if (!--entity->invincibilityTimer) {
        CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_1), entity->position.x, entity->position.y - 0x100000)->parent = (Entity *)entity;
        RSDK.SetSpriteAnimation(0xFFFF, 0, &entity->animator2, true, 0);
        entity->invincibilityTimer = 60;
        entity->state              = Gachapandora_State_Unknown10;
    }
}

void Gachapandora_State_Unknown10(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    Gachapandora_HandleChildren();
    entity->position.y = BadnikHelpers_Oscillate(entity->posUnknown.y, 2, 10);
    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x800;
    if (!--entity->invincibilityTimer) {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        entity->invincibilityTimer = 120;
        entity->state              = Gachapandora_State_Finish;
    }
}

void Gachapandora_State_Finish(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator2);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator4);
    Gachapandora_HandleChildren();
    Gachapandora_Explode(24, -24, -48, 48);

    if (entity->invincibilityTimer) {
        if (!--entity->invincibilityTimer || entity->invincibilityTimer == 96)
            entity->velocity.y = -0x40000;
    }
    else {
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x1800;

        if (entity->position.y > (ScreenInfo->position.y + ScreenInfo->height + 128) << 16) {
            Zone->screenBoundsR1[0] += 424;
            entity->state = StateMachine_None;
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
        }
    }
}

void Gachapandora_StateDraw_Unknown1(void)
{
    RSDK_THIS(Gachapandora);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        entity->animator1.frameID = 4;
        RSDK.DrawSprite(&entity->animator1, NULL, false);
        entity->animator1.frameID = 5;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        for (int i = 0; i < Gachapandora_CapsuleCount; ++i) {
            if (Gachapandora->capsuleOffsetL[i] != 0xFF) {
                entity->animator4.frameID = ((Gachapandora->capsuleTypeL[i] % 3) >> 1) + 2 * (Gachapandora->capsuleTypeL[i] / 3);
                drawPos.x                 = entity->position.x - (Gachapandora->capsuleOffsets[56 - (Gachapandora->capsuleOffsetL[i] >> 2)] << 16);
                drawPos.y                 = entity->position.y + (((Gachapandora->capsuleOffsetL[i] & 0xFC) - 152) << 14);
                RSDK.DrawSprite(&entity->animator4, &drawPos, false);
            }
            if (Gachapandora->capsuleOffsetR[i] != 0xFF) {
                entity->animator4.frameID = ((Gachapandora->capsuleTypeR[i] % 3) >> 1) + 2 * (Gachapandora->capsuleTypeR[i] / 3);
                drawPos.x                 = entity->position.x + (Gachapandora->capsuleOffsets[56 - (Gachapandora->capsuleOffsetR[i] >> 2)] << 16);
                drawPos.y                 = entity->position.y + (((Gachapandora->capsuleOffsetR[i] & 0xFC) - 152) << 14);
                RSDK.DrawSprite(&entity->animator4, &drawPos, false);
            }
        }

        if (Gachapandora->value15 != 0x7FFF) {
            entity->animator4.frameID = ((Gachapandora->nextChildType % 3) >> 1) + 2 * (Gachapandora->nextChildType / 3);
            drawPos.x                 = entity->position.x + (Gachapandora->value15 << 14);
            drawPos.y                 = entity->position.y + 0x120000;
            RSDK.DrawSprite(&entity->animator4, &drawPos, false);
        }

        entity->animator1.frameID = 2;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->animator1.frameID = 3;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        int storeDir      = entity->direction;
        entity->direction = Gachapandora->value4;
        drawPos.x         = entity->position.x;
        drawPos.y         = entity->position.y - 0x100000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = storeDir;
    }
    else {
        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        int storeDir      = entity->direction;
        entity->direction = Gachapandora->handleDir;
        drawPos.y += 0x1A0000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->direction = storeDir;
    }
}

void Gachapandora_StateDraw_Unknown2(void)
{
    RSDK_THIS(Gachapandora);
    Vector2 drawPos;

    int storeDir = entity->direction;
    drawPos.x    = entity->position.x;
    drawPos.y    = entity->position.y;
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        entity->animator1.frameID = 0;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        entity->direction = Gachapandora->value4;
        drawPos.x         = entity->position.x;
        drawPos.y         = entity->position.y - 0x100000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
    else {
        entity->animator1.frameID = 1;
        RSDK.DrawSprite(&entity->animator1, NULL, false);

        storeDir          = entity->direction;
        entity->direction = Gachapandora->handleDir;
        drawPos.y += 0x1A0000;
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&entity->animator4, NULL, false);

        entity->direction = FLIP_X;
        RSDK.DrawSprite(&entity->animator4, NULL, false);
    }
    entity->direction = storeDir;
}

void Gachapandora_State2_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2000;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &debris->animator, true, 4);
        debris->velocity.x    = RSDK.Rand(-1, 2) << 16;
        debris->velocity.y    = -0x48000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, entity->position.x, entity->position.y);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &debris->animator, true, 5);
        if (entity->type > GACHAPANDORA_PRIZE)
            ++debris->animator.frameID;
        debris->velocity.x    = RSDK.Rand(-1, 2) << 15;
        debris->velocity.y    = -0x30000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        RSDK.PlaySfx(Gachapandora->sfxPon, false, 255);
        entity->velocity.y = -0x38000;
        switch (entity->type) {
            case 0:
            case 3:
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 3, &entity->animator1, true, 0);
                entity->type = GACHAPANDORA_DRILLER;
                if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x)
                    entity->direction = FLIP_X;
                break;
            case 1:
            case 4:
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 4, &entity->animator1, true, 0);
                entity->type = GACHAPANDORA_FIREDROPPER;
                break;
            case 2:
            case 5:
                if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > entity->position.x)
                    entity->velocity.x = -0x8000;
                else
                    entity->velocity.x = 0x8000;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 5, &entity->animator1, true, 0);
                entity->type = GACHAPANDORA_AMY;
                break;
            default: break;
        }
        entity->drawFX |= FX_SCALE;
        entity->scale.x   = 0x20;
        entity->scale.y   = 0x20;
        entity->drawOrder = Zone->drawOrderHigh + 1;
        if (++Gachapandora->value26 >= 8 && !Gachapandora->activeToys)
            Gachapandora->value27 = true;
        entity->state = Gachapandora_State2_Unknown2;
    }
}

void Gachapandora_State2_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2000;
    if (entity->scale.x < 0x200) {
        entity->scale.y += 16;
        entity->scale.x += 16;
    }

    switch (entity->type) {
        default: break;
        case GACHAPANDORA_AMY:
            RSDK.ProcessAnimation(&entity->animator1);
            entity->position.x += entity->velocity.x;
            if (entity->velocity.y >= 0) {
                if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
                    entity->hitbox.left   = -5;
                    entity->hitbox.top    = -11;
                    entity->velocity.y    = 0;
                    entity->hitbox.right  = 5;
                    entity->hitbox.bottom = 11;
                    entity->health        = 1;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &entity->animator1, true, 0);
                    entity->state = Gachapandora_State2_Unknown6;
                }
            }
            break;
        case GACHAPANDORA_DRILLER:
            if (entity->velocity.y >= 0) {
                if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x110000, true)) {
                    if (entity->direction == FLIP_NONE)
                        entity->velocity.x = -0x20000;
                    else
                        entity->velocity.x = 0x20000;
                    entity->velocity.y    = -0x18000;
                    entity->hitbox.left   = -16;
                    entity->hitbox.top    = -10;
                    entity->hitbox.right  = 25;
                    entity->hitbox.bottom = 17;
                    entity->health        = 3;
                    entity->state         = Gachapandora_State2_Unknown3;
                }
            }
            break;
        case GACHAPANDORA_FIREDROPPER:
            if (entity->velocity.y >= 0) {
                entity->posUnknown.y = entity->position.y;
                if (entity->direction == FLIP_NONE)
                    entity->velocity.x = -0x18000;
                else
                    entity->velocity.x = 0x18000;
                entity->velocity.y    = 0;
                entity->hitbox.left   = -13;
                entity->hitbox.top    = -10;
                entity->hitbox.right  = 13;
                entity->hitbox.bottom = 10;
                entity->health        = 3;
                entity->timer         = 120;
                entity->state         = Gachapandora_State2_Unknown5;
            }
            break;
    }
}

void Gachapandora_StateDraw2_Unknown(void)
{
    RSDK_THIS(Gachapandora);

    if (entity->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&entity->animator1, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void Gachapandora_State_Destroyed(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_Explode(8, -8, -8, 8);

    if (entity->invincibilityTimer) {
        entity->invincibilityTimer--;
    }
    else {
        if (++Gachapandora->activeToys >= 8 && Gachapandora->value27)
            API_UnlockAchievement("ACH_MMZ");
        destroyEntity(entity);
    }
}

void Gachapandora_State2_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_State2_Unknown4();

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x110000, true)) {
        entity->velocity.y = 0;
        entity->state      = Gachapandora_State2_Unknown4;
    }
    else {
        entity->velocity.y += 0x3800;
    }
}

void Gachapandora_State2_Unknown4(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);

    entity->position.y += entity->velocity.y;
    entity->position.x += entity->velocity.x;

    if (entity->position.x < (ScreenInfo->position.x - 64) << 16
        || entity->position.x > (ScreenInfo->position.x + ScreenInfo->width + 64) << 16) {
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }

    if (entity->invincibilityTimer)
        entity->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions();
}

void Gachapandora_State2_Unknown5(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.x += entity->velocity.x;
    if (entity->startY) {
        entity->position.y += entity->velocity.y;
        entity->velocity.y -= 0x800;
        if (entity->position.y <= entity->posUnknown.y) {
            entity->position.y = entity->posUnknown.y;
            entity->velocity.y = 0;
            entity->angle      = 0;
            entity->startY     = 0;
        }
    }
    else {
        entity->position.y = BadnikHelpers_Oscillate(entity->posUnknown.y, 4, 9);
    }

    if (entity->position.x < (ScreenInfo->position.x + 16) << 16
        || entity->position.x > (ScreenInfo->position.x + ScreenInfo->width - 16) << 16) {
        entity->startY = 1;
        entity->direction ^= FLIP_X;
        entity->velocity.y = 0x18000;
        entity->velocity.x = -entity->velocity.x;
    }
    if (!--entity->timer) {
        entity->timer = 120;
        RSDK.PlaySfx(Gachapandora->sfxFireball, false, 255);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 10, &entity->animator2, true, 0);
        EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_FIREBALL), entity->position.x, entity->position.y);
        child->parent             = (Entity *)entity;
        child->timer              = 12;
    }

    if (entity->invincibilityTimer)
        entity->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions();
}

void Gachapandora_State6_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);

    if (entity->timer) {
        Entity *parent = entity->parent;
        if (parent->direction)
            entity->position.x = parent->position.x + 0x20000;
        else
            entity->position.x = parent->position.x - 0x20000;
        entity->position.y += parent->velocity.y;
        entity->timer--;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox4)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }

    entity->position.y += 0x18000;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x20000, true)) {
        entity->timer  = 6;
        entity->health = 4;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &entity->animator1, true, 0);
        entity->state = Gachapandora_State6_Unknown2;
    }
}

void Gachapandora_State6_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);

    if (entity->timer) {
        if (!--entity->timer) {
            EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_FIREBALL), entity->position.x + 0x40000, entity->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &child->animator1, true, 0);
            child->state = Gachapandora_State6_Unknown2;
            if (entity->health) {
                child->timer  = 6;
                child->health = entity->health - 1;
            }

            child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_FIREBALL), entity->position.x - 0x40000, entity->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &child->animator1, true, 0);
            child->state = Gachapandora_State6_Unknown2;
            if (entity->health) {
                child->timer  = 6;
                child->health = entity->health - 1;
            }
        }
    }
    if (entity->animator1.frameID < 5) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox4)) {
                Player_CheckElementalHit(player, entity, SHIELD_FIRE);
            }
        }
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void Gachapandora_StateDraw8_Unknown(void)
{
    RSDK_THIS(Gachapandora);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Gachapandora_State2_Unknown7(void)
{
    RSDK_THIS(Gachapandora);

    EntityPlayer *player = Player_GetNearestPlayer();
    if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox2)) {
        entity->velocity.x = 0;
        entity->direction  = player->position.x > entity->position.x;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &entity->animator1, true, 0);
        entity->state = Gachapandora_State2_Unknown6;
        Gachapandora_State2_Unknown6();
    }

    if (entity->state == Gachapandora_State2_Unknown7) {
        RSDK.ProcessAnimation(&entity->animator1);
        if (entity->invincibilityTimer)
            entity->invincibilityTimer--;
        else
            Gachapandora_CheckPlayerCollisions();
    }
}

void Gachapandora_State2_Unknown6(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;

    if (entity->invincibilityTimer)
        entity->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions();

    EntityPlayer *player = Player_GetNearestPlayer();

    if (entity->position.x < (ScreenInfo->position.x + 8) << 16 || entity->position.x > (ScreenInfo->width + ScreenInfo->position.x - 8) << 16
        || Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox2)) {
        if (abs(player->position.x - entity->position.x) > 0x80000)
            entity->direction = player->position.x > entity->position.x;

        bool32 flag = false;
        if (entity->direction) {
            if (entity->velocity.x < 0x20000)
                entity->velocity.x += 0xC00;

            flag = entity->velocity.x >= 0x10000;
        }
        else {
            if (entity->velocity.x > -0x20000)
                entity->velocity.x -= 0xC00;

            flag = entity->velocity.x <= -0x10000;
        }

        if (flag && Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox3)) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 5, &entity->animator1, true, 0);
            entity->state      = Gachapandora_State2_Unknown8;
            entity->onGround   = false;
            entity->velocity.y = -0x24000;
        }
    }
    else {
        if (entity->velocity.x > 0) {
            entity->velocity.x -= 0xC00;
            if (entity->velocity.x < 0) {
                entity->velocity.x = 0;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &entity->animator1, true, 0);
                entity->state = Gachapandora_State2_Unknown7;
            }
        }
        else {
            entity->velocity.x += 0xC00;
            if (entity->velocity.x >= 0) {
                entity->velocity.x = 0;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &entity->animator1, true, 0);
                entity->state = Gachapandora_State2_Unknown7;
            }
        }
    }
}

void Gachapandora_State2_Unknown8(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (!entity->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox4)) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    if (Player_CheckBossHit(player, entity)) {
                        if (entity->health)
                            entity->health--;
                        if (entity->health) {
                            RSDK.PlaySfx(Gachapandora->sfxHit, false, 255);
                        }
                        else {
                            EntityGachapandora *eggman = (EntityGachapandora *)Gachapandora->eggman;
                            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &eggman->animator2, true, 0);
                            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
                            entity->state = Gachapandora_State_Destroyed;
                        }
                        entity->invincibilityTimer = 30;
                    }
                    foreach_break;
                }
                else {
#endif
                    if (!Player_CheckAttacking(player, entity) || player->stateInput == Gachapandora_Player_StateInput_P1Grabbed
                        || player->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed
                        || player->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed) {
                        RSDK.PlaySfx(Gachapandora->sfxGiggle, false, 255);
                        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 6, &entity->animator1, true, 0);

                        if (player->stateInput == Player_ProcessP1Input)
                            player->stateInput = Gachapandora_Player_StateInput_P1Grabbed;
                        else if (player->stateInput == Player_ProcessP2Input_Player)
                            player->stateInput = Gachapandora_Player_StateInput_P2PlayerGrabbed;
                        else if (player->stateInput == Player_ProcessP2Input_AI)
                            player->stateInput = Gachapandora_Player_StateInput_P2AIGrabbed;

                        entity->timer  = 90;
                        entity->parent = (Entity *)player;
                        entity->state  = Gachapandora_State2_Unknown10;
                        foreach_break;
                    }
                    else {
                        bool32 flag = entity->velocity.x >= 0;
                        if (player->position.x > entity->position.x) {
                            if (entity->velocity.x > 0) {
                                entity->velocity.x = -entity->velocity.x;
                                flag               = true;
                            }
                        }
                        if (player->position.x >= entity->position.x || flag) {
                            entity->velocity.y = -0x20000;
                            if (entity->position.x >= player->position.x) {
                                entity->direction  = FLIP_NONE;
                                player->velocity.x = -0x18000;
                            }
                            else {
                                entity->direction  = FLIP_X;
                                player->velocity.x = 0x18000;
                            }
                            player->groundVel  = player->velocity.x;
                            player->velocity.y = -player->velocity.y;
                            player->state      = Player_State_Air;
                            if (player->playerAnimator.animationID == ANI_SPINDASH)
                                player->playerAnimator.animationID = ANI_WALK;
                            entity->state = Gachapandora_State2_Unknown9;
                            foreach_break;
                        }
                    }
#if RETRO_USE_PLUS
                }
#endif
            }
        }
    }
    else {
        entity->invincibilityTimer--;
    }

    if (entity->state == Gachapandora_State2_Unknown8) {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
            entity->velocity.y   = 0;
            EntityPlayer *player = Player_GetNearestPlayer();
            if (entity->position.x < (ScreenInfo->position.x + 8) << 16
                || entity->position.x > (ScreenInfo->width + ScreenInfo->position.x - 8) << 16
                || Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox2)) {
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &entity->animator1, true, 0);
                entity->state = Gachapandora_State2_Unknown6;
            }
            else {
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &entity->animator1, true, 0);
                entity->state = Gachapandora_State2_Unknown7;
            }
        }
    }
}

void Gachapandora_State2_Unknown9(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (entity->invincibilityTimer)
        entity->invincibilityTimer--;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
        entity->velocity.y   = 0;
        EntityPlayer *player = Player_GetNearestPlayer();

        if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox2)) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &entity->animator1, true, 0);
            entity->state = Gachapandora_State2_Unknown6;
        }
        else {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &entity->animator1, true, 0);
            entity->state = Gachapandora_State2_Unknown7;
        }
    }
    if (entity->animator1.animationID == 7) {
        if (!--entity->timer) {
            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            entity->invincibilityTimer                                                                             = 30;
            entity->state                                                                                          = Gachapandora_State_Destroyed;
        }
    }
}

void Gachapandora_State2_Unknown10(void)
{
    RSDK_THIS(Gachapandora);
    EntityPlayer *parent = (EntityPlayer *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x = parent->position.x;
    entity->position.y = parent->position.y;

    entity->direction = parent->direction ^ FLIP_X;
    if (entity->direction)
        entity->position.x -= 0x60000;
    else
        entity->position.x += 0x60000;

    if (entity->prevShakeFlags) {
        if (entity->shakeTimer) {
            entity->shakeTimer--;
            uint8 flags = 0;
            if (parent->left)
                flags = 1;
            if (parent->right)
                flags |= 2;
            if (flags) {
                if (flags != 3 && flags != entity->prevShakeFlags) {
                    entity->prevShakeFlags = flags;
                    if (++entity->shakeCount >= 6) {
                        entity->shakeCount = 0;
                        entity->parent     = NULL;
                        if (parent->stateInput == Gachapandora_Player_StateInput_P1Grabbed)
                            parent->stateInput = Player_ProcessP1Input;
                        else if (parent->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed)
                            parent->stateInput = Player_ProcessP2Input_Player;
                        else if (parent->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed)
                            parent->stateInput = Player_ProcessP2Input_AI;

                        entity->velocity.y = -0x20000;
                        if (entity->direction == FLIP_NONE)
                            entity->velocity.x = 0x20000;
                        else
                            entity->velocity.x = -0x20000;
                        entity->state = Gachapandora_State2_Unknown9;
                    }
                }
            }
        }
        else {
            entity->shakeCount     = 0;
            entity->prevShakeFlags = 0;
        }
    }
    else {
        if (parent->left)
            entity->prevShakeFlags = 1;
        else if (parent->right)
            entity->prevShakeFlags = 2;

        entity->shakeTimer = 64;
    }

    if (entity->state == Gachapandora_State2_Unknown10) {
        if (!--entity->timer) {
            if (parent->stateInput == Gachapandora_Player_StateInput_P1Grabbed)
                parent->stateInput = Player_ProcessP1Input;
            else if (parent->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed)
                parent->stateInput = Player_ProcessP2Input_Player;
            else if (parent->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed)
                parent->stateInput = Player_ProcessP2Input_AI;

            Player_CheckHit(parent, entity);
            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            entity->invincibilityTimer                                                                             = 30;
            entity->state                                                                                          = Gachapandora_State_Destroyed;
        }
        else if (entity->timer == 30) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 7, &entity->animator1, true, 0);
        }
    }
}

void Gachapandora_State7_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    if (!--entity->timer) {
        entity->state     = Gachapandora_State7_Unknown2;
        entity->stateDraw = Gachapandora_StateDraw7_Unknown1;
    }
}

void Gachapandora_State7_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void Gachapandora_State7_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        entity->velocity.y = -0x20000;
        entity->velocity.x = RSDK.Rand(-2, 3) << 16;
        entity->state      = Gachapandora_State7_Unknown2;
        entity->stateDraw  = Gachapandora_StateDraw7_Unknown1;
    }
}

void Gachapandora_StateDraw7_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    if (Zone->timer & 1)
        RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Gachapandora_State8_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);

    Entity *parent = entity->parent;
    if (parent) {
        entity->position.x = parent->position.x + entity->posUnknown.x;
        entity->position.y = parent->position.y + entity->posUnknown.y;
    }

    if (entity->animator1.animationID >= 14 && (entity->animator1.animationID != 16 || entity->animator1.frameID < 2)) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox6)) {
                Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
            }
        }
    }

    if (entity->animator1.frameID == entity->animator1.frameCount - 1)
        destroyEntity(entity);
}

void Gachapandora_State8_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->velocity.y < 0x20000)
        entity->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Gachapandora->hitbox6)) {
            Player_CheckElementalHit(player, entity, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 16, &entity->animator1, true, 0);
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = Gachapandora_State8_Unknown1;
    }
}

void Gachapandora_State1_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    if (entity->position.y >= entity->parent->position.y + 0xE0000) {
        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 5, &entity->animator1, true, 0);
        entity->state = Gachapandora_State1_Unknown2;
    }
}

void Gachapandora_State1_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x180000, true)) {
        entity->velocity.x = 0;
        entity->velocity.y = 0;
        entity->state      = Gachapandora_State1_Unknown3;
    }
}

void Gachapandora_State1_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&entity->animator1);
    if (entity->animator1.frameID >= 4) {
        if (entity->velocity.x < 0x60000)
            entity->velocity.x += 0xC00;
        entity->position.x += entity->velocity.x;
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Gachapandora_EditorDraw(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &entity->animator1, false, 0);
    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &entity->animator2, false, 0);
    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &entity->animator3, false, 7);
    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &entity->animator4, false, 7);
    
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    entity->animator1.frameID = 4;
    RSDK.DrawSprite(&entity->animator1, NULL, false);
    entity->animator1.frameID = 5;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 2;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 3;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->animator1.frameID = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    int storeDir      = entity->direction;
    entity->direction = Gachapandora->value4;
    drawPos.x         = entity->position.x;
    drawPos.y         = entity->position.y - 0x100000;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    entity->direction = storeDir;

    entity->animator1.frameID = 1;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->direction = Gachapandora->handleDir;
    drawPos.y += 0x1A0000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    entity->direction = storeDir;
}

void Gachapandora_EditorLoad(void)
{
    Gachapandora->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Gachapandora.bin", SCOPE_STAGE);
    Gachapandora->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ2.bin", SCOPE_STAGE);
}
#endif

void Gachapandora_Serialize(void) {}
