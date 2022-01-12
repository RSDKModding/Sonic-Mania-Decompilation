// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Gachapandora Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectGachapandora *Gachapandora;

void Gachapandora_Update(void)
{
    RSDK_THIS(Gachapandora);
    StateMachine_Run(self->state);
}

void Gachapandora_LateUpdate(void) {}

void Gachapandora_StaticUpdate(void) { RSDK.AddDrawListRef(Zone->drawOrderLow + 1, RSDK.GetEntityID(Gachapandora->eggman)); }

void Gachapandora_Draw(void)
{
    RSDK_THIS(Gachapandora);
    StateMachine_Run(self->stateDraw);
}

void Gachapandora_Create(void *data)
{
    RSDK_THIS(Gachapandora);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        if (globals->gameMode < MODE_TIMEATTACK) {
            self->active = ACTIVE_BOUNDS;
            if (data)
                self->type = voidToInt(data);

            switch (self->type) {
                case GACHAPANDORA_MAIN:
                    self->visible   = false;
                    self->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &self->animator1, true, 0);
                    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->animator2, true, 0);
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->animator3, true, 7);
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &self->animator4, true, 7);

                    self->hitbox.left   = -33;
                    self->hitbox.top    = -37;
                    self->hitbox.right  = 33;
                    self->hitbox.bottom = 6;
                    self->startY        = self->position.y;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    Gachapandora->eggman = (Entity *)self;
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
                    self->state     = Gachapandora_State_SetupArena;
                    self->stateDraw = Gachapandora_StateDraw_Unknown1;
                    break;
                case GACHAPANDORA_1:
                    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 4, &self->animator1, true, 0);
                    self->state         = Gachapandora_State1_Unknown1;
                    self->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow;
                    break;
                case GACHAPANDORA_PRIZE:
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &self->animator1, true, 0);
                    self->state         = Gachapandora_State2_Unknown1;
                    self->stateDraw     = Gachapandora_StateDraw2_Unknown;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow;
                    break;
                case GACHAPANDORA_FIREBALL:
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 11, &self->animator1, true, 0);
                    self->hitbox.left   = -2;
                    self->hitbox.top    = -2;
                    self->hitbox.right  = 2;
                    self->hitbox.bottom = 2;
                    self->state         = Gachapandora_State6_Unknown1;
                    self->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawOrder     = Zone->drawOrderLow;
                    break;
                case GACHAPANDORA_DEBRIS:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderLow;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &self->animator1, true, 0);
                    self->state         = Gachapandora_State7_Unknown1;
                    self->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                case GACHAPANDORA_SPARK:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawOrder = Zone->drawOrderHigh;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 13, &self->animator1, true, 0);
                    self->state         = Gachapandora_State8_Unknown1;
                    self->stateDraw     = Gachapandora_StateDraw8_Unknown;
                    self->updateRange.y = 0x800000;
                    self->updateRange.x = 0x800000;
                    break;
                default: break;
            }
        }
        else {
            destroyEntity(self);
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

    Gachapandora->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    Gachapandora->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    Gachapandora->sfxUnravel   = RSDK.GetSfx("Stage/Unravel.wav");
    Gachapandora->sfxPon       = RSDK.GetSfx("Stage/Pon.wav");
    Gachapandora->sfxFireball  = RSDK.GetSfx("Stage/Fireball.wav");
    Gachapandora->sfxGiggle    = RSDK.GetSfx("MMZ/Giggle.wav");
}

void Gachapandora_CheckPlayerCollisions(void)
{
    RSDK_THIS(Gachapandora);

    foreach_active(Player, player)
    {
        if (player->animator.animationID != ANI_VICTORY) {
            if (Player_CheckBadnikTouch(player, self, &self->hitbox)) {
                int blink = player->blinkTimer;
                if (self->type == GACHAPANDORA_AMY)
                    player->blinkTimer = 1;
                if (Player_CheckBossHit(player, self)) {
                    if (self->health)
                        self->health--;

                    if (self->health) {
                        RSDK.PlaySfx(Gachapandora->sfxHit, false, 255);
                    }
                    else {
                        EntityGachapandora *eggman = (EntityGachapandora *)Gachapandora->eggman;
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &eggman->animator2, true, 0);
                        RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
                        self->state = Gachapandora_State_Destroyed;
                    }
                    self->invincibilityTimer = 30;
                    if (self->type == GACHAPANDORA_AMY)
                        player->blinkTimer = blink;
                    foreach_break;
                }
                else {
                    if (self->type == GACHAPANDORA_AMY)
                        player->blinkTimer = blink;
                }
            }
            else {
                if (self->type == GACHAPANDORA_DRILLER) {
                    if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox5))
                        Player_CheckHit(player, self);
                }
            }
        }
    }
}

void Gachapandora_Explode(int xMax, int xMin, int yMin, int yMax)
{
    RSDK_THIS(Gachapandora);

    int interval = 4 * (self->state == Gachapandora_State_Unknown5) + 3;
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
    self->up         = false;
    self->down       = false;
    self->jumpPress  = false;
    self->jumpHold   = false;
    self->velocity.x = clampVal(self->velocity.x, -0x20000, 0x20000);
    self->groundVel  = clampVal(self->groundVel, -0x20000, 0x20000);
}

void Gachapandora_Player_StateInput_P2PlayerGrabbed(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_Player();
    self->up         = false;
    self->down       = false;
    self->jumpPress  = false;
    self->jumpHold   = false;
    self->velocity.x = clampVal(self->velocity.x, -0x40000, 0x40000);
    self->groundVel  = clampVal(self->groundVel, -0x40000, 0x40000);
    if (self->stateInput == Player_ProcessP2Input_AI)
        self->stateInput = Gachapandora_Player_StateInput_P2AIGrabbed;
}

void Gachapandora_Player_StateInput_P2AIGrabbed(void)
{
    RSDK_THIS(Player);
    Player_ProcessP2Input_AI();
    self->up         = false;
    self->down       = false;
    self->jumpPress  = false;
    self->jumpHold   = false;
    self->velocity.x = clampVal(self->velocity.x, -0x40000, 0x40000);
    self->groundVel  = clampVal(self->groundVel, -0x40000, 0x40000);
    if (self->stateInput == Player_ProcessP2Input_Player)
        self->stateInput = Gachapandora_Player_StateInput_P2PlayerGrabbed;
}

void Gachapandora_HandleChildren(void)
{
    RSDK_THIS(Gachapandora);

    --Gachapandora->value22;
    if (!Gachapandora->value22) {
        switch (self->health) {
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
                                                          self->position.x + (off << 16) + self->velocity.x, self->position.y + 0x300000);
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, RSDK.Rand(0, 4) + 18, &child->animator1, true, 0);
                if (self->state != Gachapandora_State_Unknown4 && !self->invincibilityTimer)
                    child->velocity.x = self->velocity.x >> 1;
                child->state = Gachapandora_State7_Unknown3;
                break;
            }
            case 1: {
                EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_SPARK), self->position.x, self->position.y);
                child->parent             = (Entity *)self;
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

    if (self->state == Gachapandora_State_Unknown8) {
        for (int i = 0; i < 2; ++i) {
            if (!--Gachapandora->value23[i]) {
                Gachapandora->value23[i]  = RSDK.Rand(16, 30);
                EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_SPARK), self->position.x, self->position.y);
                child->parent             = (Entity *)self;
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

    if (self->animator2.animationID) {
        if (self->animator2.animationID == 1) {
            if (self->animator2.frameID >= self->animator2.frameCount - 1)
                RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->animator2, true, 0);
        }
        else {
            if (self->animator2.animationID == 2) {
                bool32 flag = false;
                foreach_active(Player, player)
                {
                    if (player->state == Player_State_Hit || player->state == Player_State_Die)
                        flag = true;
                }

                if (self->animator2.frameID >= self->animator2.frameCount - 1) {
                    if (flag)
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &self->animator2, true, 6);
                    else
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->animator2, true, 0);
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
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &self->animator2, true, 0);
    }
}

void Gachapandora_State_SetupArena(void)
{
    RSDK_THIS(Gachapandora);

    if (++self->timer >= 2) {
        self->timer               = 0;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]     = (self->position.x >> 16) + ScreenInfo->centerX;
        Zone->cameraBoundsB[0]     = (self->position.y >> 16) + 128;

        self->position.y = (ScreenInfo->position.y - 192) << 16;
        self->active     = ACTIVE_NORMAL;
        self->visible    = true;
        self->state      = Gachapandora_State_Unknown1;
    }
}

void Gachapandora_State_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]     = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]     = (self->position.x >> 16) - ScreenInfo->centerX;
        Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
        self->health = 9;
        self->state  = Gachapandora_State_Unknown2;
    }
}

void Gachapandora_State_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);

    self->position.y = self->position.y + 0x10000;
    if (self->position.y < self->startY) {
        if (self->position.y >= self->startY - 0x400000)
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &self->animator2, false, 0);
    }
    else {
        self->position.y   = self->startY;
        self->posUnknown.x = self->position.x;
        self->posUnknown.y = self->position.y;
        self->angle        = 0;
        self->state        = Gachapandora_State_Unknown3;
    }
    RSDK.AddDrawListRef(Zone->drawOrderLow + 1, SceneInfo->entitySlot);
}

void Gachapandora_State_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    Gachapandora_HandleAnimations();
    self->position.y = BadnikHelpers_Oscillate(self->posUnknown.y, 2, 10);

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
        if (!--self->invincibilityTimer && !self->velocity.x) {
            if (self->direction == FLIP_NONE)
                self->velocity.x = -0x8000;
            else
                self->velocity.x = 0x8000;
        }
    }
    else {
        if (self->position.x < (ScreenInfo->position.x + 64) << 16
            || self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 64) << 16)
            self->velocity.x = -self->velocity.x;
        self->position.x += self->velocity.x;
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

    if (self->timer <= 0) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox1)) {
                RSDK.PlaySfx(Gachapandora->sfxUnravel, false, 255);
                if (player->position.x >= self->position.x)
                    Gachapandora->handleDir = player->velocity.y < 0;
                else
                    Gachapandora->handleDir = player->velocity.y > 0;

                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->animator3, true, 0);
                if (Gachapandora->nextCapsuleL + Gachapandora->nextCapsuleR == 0x1FE) {
                    self->invincibilityTimer = 25;
                    self->state              = Gachapandora_State_Unknown9;
                }
                else {
                    self->timer = 60;
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
                    if (self->velocity.x >= 0)
                        self->velocity.x += 0x4000;
                    else
                        self->velocity.x -= 0x4000;
                    self->state = Gachapandora_State_Unknown4;
                }
                foreach_break;
            }
        }
    }
    else {
        self->timer--;
    }
}

void Gachapandora_State_Unknown4(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    Gachapandora_HandleAnimations();
    self->position.y = BadnikHelpers_Oscillate(self->posUnknown.y, 2, 10);
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
        EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_PRIZE), self->position.x, self->position.y + 0x1A0000);
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
        if (--self->health == 1) {
            child                    = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            child->animator1.frameID = 4;
            child->timer             = 30;
            child->velocity.x        = 0x20000;
            child->velocity.y        = -0x20000;

            child                    = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            child->animator1.frameID = 5;
            child->timer             = 30;
            child->velocity.x        = -0x20000;
            child->velocity.y        = -0x20000;

            child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            ++child->drawOrder;
            child->animator1.frameID = 2;
            child->timer             = 90;
            child->velocity.x        = -0x20000;
            child->velocity.y        = -0x20000;

            child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            ++child->drawOrder;
            child->animator1.frameID = 3;
            child->timer             = 90;
            child->velocity.x        = 0x20000;
            child->velocity.y        = -0x20000;

            self->invincibilityTimer = 120;
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &self->animator2, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->animator4, true, 0);
            self->state     = Gachapandora_State_Unknown5;
            self->stateDraw = Gachapandora_StateDraw_Unknown2;
        }
        else {
            self->invincibilityTimer = 30;
            self->state              = Gachapandora_State_Unknown3;
        }
    }
}

void Gachapandora_State_Unknown5(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    Gachapandora_Explode(24, -24, -48, 48);
    Gachapandora_HandleChildren();
    if (--self->invincibilityTimer <= 0)
        self->state = Gachapandora_State_Unknown6;
}

void Gachapandora_State_Unknown6(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    Gachapandora_HandleChildren();
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1800;
    if (self->position.y < (ScreenInfo->position.y - 192) << 16) {
        self->position.y = (ScreenInfo->position.y - 192) << 16;
        self->position.x = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        self->state      = Gachapandora_State_Unknown7;
    }
}

void Gachapandora_State_Unknown7(void)
{
    RSDK_THIS(Gachapandora);

    if (Gachapandora->activeToys >= 8) {
        self->posUnknown.y = self->position.y;
        self->angle        = 0;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 17, &self->animator4, true, 0);
        self->timer = 60;
        self->velocity.x >>= 1;
        self->state = Gachapandora_State_Unknown8;
    }
}

void Gachapandora_State_Unknown8(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    Gachapandora_HandleAnimations();

    if (self->posUnknown.y >= self->startY) {
        self->position.y = BadnikHelpers_Oscillate(self->posUnknown.y, 2, 10);
    }
    else {
        self->posUnknown.y += 0x10000;
        self->position.y = self->posUnknown.y;
    }
    Gachapandora_HandleChildren();

    if (self->position.x < (ScreenInfo->position.x + 32) << 16
        || self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 32) << 16)
        self->velocity.x = -self->velocity.x;
    self->position.x = self->position.x + self->velocity.x;

    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->value4 ^= 1;

    if (--self->timer <= 0) {
        self->timer = 60;
        for (int i = 0; i < 2; ++i) {
            EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_SPARK), self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 15, &child->animator1, true, 0);
            if (i) {
                child->position.x += (RSDK.Rand(-2, 2) - 34) << 16;
                child->velocity.x = self->velocity.x - 0x10000;
            }
            else {
                child->direction = FLIP_X;
                child->position.x += (RSDK.Rand(-2, 2) + 34) << 16;
                child->velocity.x = self->velocity.x + 0x10000;
            }

            child->state = Gachapandora_State8_Unknown2;
            child->position.y += (RSDK.Rand(-8, 8) + 20) << 16;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox1)) {
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 4, &self->animator2, true, 0);
            RSDK.PlaySfx(Gachapandora->sfxUnravel, false, 255);
            if (player->position.x >= self->position.x)
                Gachapandora->handleDir = player->velocity.y < 0;
            else
                Gachapandora->handleDir = player->velocity.y > 0;

            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->animator3, true, 0);
            self->invincibilityTimer = 25;
            self->velocity.y         = 0;
            self->state              = Gachapandora_State_Unknown9;
            foreach_break;
        }
    }
}

void Gachapandora_State_Unknown9(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_HandleChildren();
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x800;
    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    Gachapandora_HandleChildren();
    self->position.y = BadnikHelpers_Oscillate(self->posUnknown.y, 2, 10);
    if (!--self->invincibilityTimer) {
        CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_1), self->position.x, self->position.y - 0x100000)->parent = (Entity *)self;
        RSDK.SetSpriteAnimation(-1, 0, &self->animator2, true, 0);
        self->invincibilityTimer = 60;
        self->state              = Gachapandora_State_Unknown10;
    }
}

void Gachapandora_State_Unknown10(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    Gachapandora_HandleChildren();
    self->position.y = BadnikHelpers_Oscillate(self->posUnknown.y, 2, 10);
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x800;
    if (!--self->invincibilityTimer) {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->invincibilityTimer = 120;
        self->state              = Gachapandora_State_Finish;
    }
}

void Gachapandora_State_Finish(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator2);
    RSDK.ProcessAnimation(&self->animator3);
    RSDK.ProcessAnimation(&self->animator4);
    Gachapandora_HandleChildren();
    Gachapandora_Explode(24, -24, -48, 48);

    if (self->invincibilityTimer) {
        if (!--self->invincibilityTimer || self->invincibilityTimer == 96)
            self->velocity.y = -0x40000;
    }
    else {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x1800;

        if (self->position.y > (ScreenInfo->position.y + ScreenInfo->height + 128) << 16) {
            Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
            self->state = StateMachine_None;
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
        }
    }
}

void Gachapandora_StateDraw_Unknown1(void)
{
    RSDK_THIS(Gachapandora);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        self->animator1.frameID = 4;
        RSDK.DrawSprite(&self->animator1, NULL, false);
        self->animator1.frameID = 5;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        for (int i = 0; i < Gachapandora_CapsuleCount; ++i) {
            if (Gachapandora->capsuleOffsetL[i] != 0xFF) {
                self->animator4.frameID = ((Gachapandora->capsuleTypeL[i] % 3) >> 1) + 2 * (Gachapandora->capsuleTypeL[i] / 3);
                drawPos.x                 = self->position.x - (Gachapandora->capsuleOffsets[56 - (Gachapandora->capsuleOffsetL[i] >> 2)] << 16);
                drawPos.y                 = self->position.y + (((Gachapandora->capsuleOffsetL[i] & 0xFC) - 152) << 14);
                RSDK.DrawSprite(&self->animator4, &drawPos, false);
            }
            if (Gachapandora->capsuleOffsetR[i] != 0xFF) {
                self->animator4.frameID = ((Gachapandora->capsuleTypeR[i] % 3) >> 1) + 2 * (Gachapandora->capsuleTypeR[i] / 3);
                drawPos.x                 = self->position.x + (Gachapandora->capsuleOffsets[56 - (Gachapandora->capsuleOffsetR[i] >> 2)] << 16);
                drawPos.y                 = self->position.y + (((Gachapandora->capsuleOffsetR[i] & 0xFC) - 152) << 14);
                RSDK.DrawSprite(&self->animator4, &drawPos, false);
            }
        }

        if (Gachapandora->value15 != 0x7FFF) {
            self->animator4.frameID = ((Gachapandora->nextChildType % 3) >> 1) + 2 * (Gachapandora->nextChildType / 3);
            drawPos.x                 = self->position.x + (Gachapandora->value15 << 14);
            drawPos.y                 = self->position.y + 0x120000;
            RSDK.DrawSprite(&self->animator4, &drawPos, false);
        }

        self->animator1.frameID = 2;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->animator1.frameID = 3;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->animator1.frameID = 0;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        int storeDir      = self->direction;
        self->direction = Gachapandora->value4;
        drawPos.x         = self->position.x;
        drawPos.y         = self->position.y - 0x100000;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = storeDir;
    }
    else {
        self->animator1.frameID = 1;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        int storeDir      = self->direction;
        self->direction = Gachapandora->handleDir;
        drawPos.y += 0x1A0000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->direction = storeDir;
    }
}

void Gachapandora_StateDraw_Unknown2(void)
{
    RSDK_THIS(Gachapandora);
    Vector2 drawPos;

    int storeDir = self->direction;
    drawPos.x    = self->position.x;
    drawPos.y    = self->position.y;
    if (SceneInfo->currentDrawGroup == Zone->drawOrderLow) {
        self->animator1.frameID = 0;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        self->direction = Gachapandora->value4;
        drawPos.x         = self->position.x;
        drawPos.y         = self->position.y - 0x100000;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }
    else {
        self->animator1.frameID = 1;
        RSDK.DrawSprite(&self->animator1, NULL, false);

        storeDir          = self->direction;
        self->direction = Gachapandora->handleDir;
        drawPos.y += 0x1A0000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->animator4, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->animator4, NULL, false);
    }
    self->direction = storeDir;
}

void Gachapandora_State2_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2000;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &debris->animator, true, 4);
        debris->velocity.x    = RSDK.Rand(-1, 2) << 16;
        debris->velocity.y    = -0x48000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &debris->animator, true, 5);
        if (self->type > GACHAPANDORA_PRIZE)
            ++debris->animator.frameID;
        debris->velocity.x    = RSDK.Rand(-1, 2) << 15;
        debris->velocity.y    = -0x30000;
        debris->gravity       = 0x3800;
        debris->drawOrder     = Zone->drawOrderHigh;
        debris->updateRange.x = 0x400000;
        debris->updateRange.y = 0x400000;

        RSDK.PlaySfx(Gachapandora->sfxPon, false, 255);
        self->velocity.y = -0x38000;
        switch (self->type) {
            case 0:
            case 3:
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 3, &self->animator1, true, 0);
                self->type = GACHAPANDORA_DRILLER;
                if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x)
                    self->direction = FLIP_X;
                break;
            case 1:
            case 4:
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 4, &self->animator1, true, 0);
                self->type = GACHAPANDORA_FIREDROPPER;
                break;
            case 2:
            case 5:
                if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x)
                    self->velocity.x = -0x8000;
                else
                    self->velocity.x = 0x8000;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 5, &self->animator1, true, 0);
                self->type = GACHAPANDORA_AMY;
                break;
            default: break;
        }
        self->drawFX |= FX_SCALE;
        self->scale.x   = 0x20;
        self->scale.y   = 0x20;
        self->drawOrder = Zone->drawOrderHigh + 1;
        if (++Gachapandora->value26 >= 8 && !Gachapandora->activeToys)
            Gachapandora->value27 = true;
        self->state = Gachapandora_State2_Unknown2;
    }
}

void Gachapandora_State2_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2000;
    if (self->scale.x < 0x200) {
        self->scale.y += 16;
        self->scale.x += 16;
    }

    switch (self->type) {
        default: break;
        case GACHAPANDORA_AMY:
            RSDK.ProcessAnimation(&self->animator1);
            self->position.x += self->velocity.x;
            if (self->velocity.y >= 0) {
                if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
                    self->hitbox.left   = -5;
                    self->hitbox.top    = -11;
                    self->velocity.y    = 0;
                    self->hitbox.right  = 5;
                    self->hitbox.bottom = 11;
                    self->health        = 1;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->animator1, true, 0);
                    self->state = Gachapandora_State2_Unknown6;
                }
            }
            break;
        case GACHAPANDORA_DRILLER:
            if (self->velocity.y >= 0) {
                if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x110000, true)) {
                    if (self->direction == FLIP_NONE)
                        self->velocity.x = -0x20000;
                    else
                        self->velocity.x = 0x20000;
                    self->velocity.y    = -0x18000;
                    self->hitbox.left   = -16;
                    self->hitbox.top    = -10;
                    self->hitbox.right  = 25;
                    self->hitbox.bottom = 17;
                    self->health        = 3;
                    self->state         = Gachapandora_State2_Unknown3;
                }
            }
            break;
        case GACHAPANDORA_FIREDROPPER:
            if (self->velocity.y >= 0) {
                self->posUnknown.y = self->position.y;
                if (self->direction == FLIP_NONE)
                    self->velocity.x = -0x18000;
                else
                    self->velocity.x = 0x18000;
                self->velocity.y    = 0;
                self->hitbox.left   = -13;
                self->hitbox.top    = -10;
                self->hitbox.right  = 13;
                self->hitbox.bottom = 10;
                self->health        = 3;
                self->timer         = 120;
                self->state         = Gachapandora_State2_Unknown5;
            }
            break;
    }
}

void Gachapandora_StateDraw2_Unknown(void)
{
    RSDK_THIS(Gachapandora);

    if (self->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->animator1, NULL, false);
    RSDK.DrawSprite(&self->animator2, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void Gachapandora_State_Destroyed(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_Explode(8, -8, -8, 8);

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {
        if (++Gachapandora->activeToys >= 8 && Gachapandora->value27)
            API_UnlockAchievement("ACH_MMZ");
        destroyEntity(self);
    }
}

void Gachapandora_State2_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_State2_Unknown4();

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x110000, true)) {
        self->velocity.y = 0;
        self->state      = Gachapandora_State2_Unknown4;
    }
    else {
        self->velocity.y += 0x3800;
    }
}

void Gachapandora_State2_Unknown4(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);

    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x;

    if (self->position.x < (ScreenInfo->position.x - 64) << 16
        || self->position.x > (ScreenInfo->position.x + ScreenInfo->width + 64) << 16) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    if (self->invincibilityTimer)
        self->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions();
}

void Gachapandora_State2_Unknown5(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);

    self->position.x += self->velocity.x;
    if (self->startY) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x800;
        if (self->position.y <= self->posUnknown.y) {
            self->position.y = self->posUnknown.y;
            self->velocity.y = 0;
            self->angle      = 0;
            self->startY     = 0;
        }
    }
    else {
        self->position.y = BadnikHelpers_Oscillate(self->posUnknown.y, 4, 9);
    }

    if (self->position.x < (ScreenInfo->position.x + 16) << 16
        || self->position.x > (ScreenInfo->position.x + ScreenInfo->width - 16) << 16) {
        self->startY = 1;
        self->direction ^= FLIP_X;
        self->velocity.y = 0x18000;
        self->velocity.x = -self->velocity.x;
    }
    if (!--self->timer) {
        self->timer = 120;
        RSDK.PlaySfx(Gachapandora->sfxFireball, false, 255);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 10, &self->animator2, true, 0);
        EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_FIREBALL), self->position.x, self->position.y);
        child->parent             = (Entity *)self;
        child->timer              = 12;
    }

    if (self->invincibilityTimer)
        self->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions();
}

void Gachapandora_State6_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);

    if (self->timer) {
        Entity *parent = self->parent;
        if (parent->direction)
            self->position.x = parent->position.x + 0x20000;
        else
            self->position.x = parent->position.x - 0x20000;
        self->position.y += parent->velocity.y;
        self->timer--;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox4)) {
            Player_CheckElementalHit(player, self, SHIELD_FIRE);
        }
    }

    self->position.y += 0x18000;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x20000, true)) {
        self->timer  = 6;
        self->health = 4;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &self->animator1, true, 0);
        self->state = Gachapandora_State6_Unknown2;
    }
}

void Gachapandora_State6_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);

    if (self->timer) {
        if (!--self->timer) {
            EntityGachapandora *child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_FIREBALL), self->position.x + 0x40000, self->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &child->animator1, true, 0);
            child->state = Gachapandora_State6_Unknown2;
            if (self->health) {
                child->timer  = 6;
                child->health = self->health - 1;
            }

            child = CREATE_ENTITY(Gachapandora, intToVoid(GACHAPANDORA_FIREBALL), self->position.x - 0x40000, self->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &child->animator1, true, 0);
            child->state = Gachapandora_State6_Unknown2;
            if (self->health) {
                child->timer  = 6;
                child->health = self->health - 1;
            }
        }
    }
    if (self->animator1.frameID < 5) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox4)) {
                Player_CheckElementalHit(player, self, SHIELD_FIRE);
            }
        }
    }

    if (self->animator1.frameID == self->animator1.frameCount - 1)
        destroyEntity(self);
}

void Gachapandora_StateDraw8_Unknown(void)
{
    RSDK_THIS(Gachapandora);
    RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Gachapandora_State2_Unknown7(void)
{
    RSDK_THIS(Gachapandora);

    EntityPlayer *player = Player_GetNearestPlayer();
    if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox2)) {
        self->velocity.x = 0;
        self->direction  = player->position.x > self->position.x;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->animator1, true, 0);
        self->state = Gachapandora_State2_Unknown6;
        Gachapandora_State2_Unknown6();
    }

    if (self->state == Gachapandora_State2_Unknown7) {
        RSDK.ProcessAnimation(&self->animator1);
        if (self->invincibilityTimer)
            self->invincibilityTimer--;
        else
            Gachapandora_CheckPlayerCollisions();
    }
}

void Gachapandora_State2_Unknown6(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->invincibilityTimer)
        self->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions();

    EntityPlayer *player = Player_GetNearestPlayer();

    if (self->position.x < (ScreenInfo->position.x + 8) << 16 || self->position.x > (ScreenInfo->width + ScreenInfo->position.x - 8) << 16
        || Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox2)) {
        if (abs(player->position.x - self->position.x) > 0x80000)
            self->direction = player->position.x > self->position.x;

        bool32 flag = false;
        if (self->direction) {
            if (self->velocity.x < 0x20000)
                self->velocity.x += 0xC00;

            flag = self->velocity.x >= 0x10000;
        }
        else {
            if (self->velocity.x > -0x20000)
                self->velocity.x -= 0xC00;

            flag = self->velocity.x <= -0x10000;
        }

        if (flag && Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox3)) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 5, &self->animator1, true, 0);
            self->state      = Gachapandora_State2_Unknown8;
            self->onGround   = false;
            self->velocity.y = -0x24000;
        }
    }
    else {
        if (self->velocity.x > 0) {
            self->velocity.x -= 0xC00;
            if (self->velocity.x < 0) {
                self->velocity.x = 0;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->animator1, true, 0);
                self->state = Gachapandora_State2_Unknown7;
            }
        }
        else {
            self->velocity.x += 0xC00;
            if (self->velocity.x >= 0) {
                self->velocity.x = 0;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->animator1, true, 0);
                self->state = Gachapandora_State2_Unknown7;
            }
        }
    }
}

void Gachapandora_State2_Unknown8(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (!self->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox4)) {
#if RETRO_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    if (Player_CheckBossHit(player, self)) {
                        if (self->health)
                            self->health--;
                        if (self->health) {
                            RSDK.PlaySfx(Gachapandora->sfxHit, false, 255);
                        }
                        else {
                            EntityGachapandora *eggman = (EntityGachapandora *)Gachapandora->eggman;
                            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &eggman->animator2, true, 0);
                            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
                            self->state = Gachapandora_State_Destroyed;
                        }
                        self->invincibilityTimer = 30;
                    }
                    foreach_break;
                }
                else {
#endif
                    if (!Player_CheckAttacking(player, self) || player->stateInput == Gachapandora_Player_StateInput_P1Grabbed
                        || player->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed
                        || player->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed) {
                        RSDK.PlaySfx(Gachapandora->sfxGiggle, false, 255);
                        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 6, &self->animator1, true, 0);

                        if (player->stateInput == Player_ProcessP1Input)
                            player->stateInput = Gachapandora_Player_StateInput_P1Grabbed;
                        else if (player->stateInput == Player_ProcessP2Input_Player)
                            player->stateInput = Gachapandora_Player_StateInput_P2PlayerGrabbed;
                        else if (player->stateInput == Player_ProcessP2Input_AI)
                            player->stateInput = Gachapandora_Player_StateInput_P2AIGrabbed;

                        self->timer  = 90;
                        self->parent = (Entity *)player;
                        self->state  = Gachapandora_State2_Unknown10;
                        foreach_break;
                    }
                    else {
                        bool32 flag = self->velocity.x >= 0;
                        if (player->position.x > self->position.x) {
                            if (self->velocity.x > 0) {
                                self->velocity.x = -self->velocity.x;
                                flag               = true;
                            }
                        }
                        if (player->position.x >= self->position.x || flag) {
                            self->velocity.y = -0x20000;
                            if (self->position.x >= player->position.x) {
                                self->direction  = FLIP_NONE;
                                player->velocity.x = -0x18000;
                            }
                            else {
                                self->direction  = FLIP_X;
                                player->velocity.x = 0x18000;
                            }
                            player->groundVel  = player->velocity.x;
                            player->velocity.y = -player->velocity.y;
                            player->state      = Player_State_Air;
                            if (player->animator.animationID == ANI_SPINDASH)
                                player->animator.animationID = ANI_WALK;
                            self->state = Gachapandora_State2_Unknown9;
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
        self->invincibilityTimer--;
    }

    if (self->state == Gachapandora_State2_Unknown8) {
        if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
            self->velocity.y   = 0;
            EntityPlayer *player = Player_GetNearestPlayer();
            if (self->position.x < (ScreenInfo->position.x + 8) << 16
                || self->position.x > (ScreenInfo->width + ScreenInfo->position.x - 8) << 16
                || Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox2)) {
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->animator1, true, 0);
                self->state = Gachapandora_State2_Unknown6;
            }
            else {
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->animator1, true, 0);
                self->state = Gachapandora_State2_Unknown7;
            }
        }
    }
}

void Gachapandora_State2_Unknown9(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (self->invincibilityTimer)
        self->invincibilityTimer--;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
        self->velocity.y   = 0;
        EntityPlayer *player = Player_GetNearestPlayer();

        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox2)) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->animator1, true, 0);
            self->state = Gachapandora_State2_Unknown6;
        }
        else {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->animator1, true, 0);
            self->state = Gachapandora_State2_Unknown7;
        }
    }
    if (self->animator1.animationID == 7) {
        if (!--self->timer) {
            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            self->invincibilityTimer                                                                             = 30;
            self->state                                                                                          = Gachapandora_State_Destroyed;
        }
    }
}

void Gachapandora_State2_Unknown10(void)
{
    RSDK_THIS(Gachapandora);
    EntityPlayer *parent = (EntityPlayer *)self->parent;

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x = parent->position.x;
    self->position.y = parent->position.y;

    self->direction = parent->direction ^ FLIP_X;
    if (self->direction)
        self->position.x -= 0x60000;
    else
        self->position.x += 0x60000;

    if (self->prevShakeFlags) {
        if (self->shakeTimer) {
            self->shakeTimer--;
            uint8 flags = 0;
            if (parent->left)
                flags = 1;
            if (parent->right)
                flags |= 2;
            if (flags) {
                if (flags != 3 && flags != self->prevShakeFlags) {
                    self->prevShakeFlags = flags;
                    if (++self->shakeCount >= 6) {
                        self->shakeCount = 0;
                        self->parent     = NULL;
                        if (parent->stateInput == Gachapandora_Player_StateInput_P1Grabbed)
                            parent->stateInput = Player_ProcessP1Input;
                        else if (parent->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed)
                            parent->stateInput = Player_ProcessP2Input_Player;
                        else if (parent->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed)
                            parent->stateInput = Player_ProcessP2Input_AI;

                        self->velocity.y = -0x20000;
                        if (self->direction == FLIP_NONE)
                            self->velocity.x = 0x20000;
                        else
                            self->velocity.x = -0x20000;
                        self->state = Gachapandora_State2_Unknown9;
                    }
                }
            }
        }
        else {
            self->shakeCount     = 0;
            self->prevShakeFlags = 0;
        }
    }
    else {
        if (parent->left)
            self->prevShakeFlags = 1;
        else if (parent->right)
            self->prevShakeFlags = 2;

        self->shakeTimer = 64;
    }

    if (self->state == Gachapandora_State2_Unknown10) {
        if (!--self->timer) {
            if (parent->stateInput == Gachapandora_Player_StateInput_P1Grabbed)
                parent->stateInput = Player_ProcessP1Input;
            else if (parent->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed)
                parent->stateInput = Player_ProcessP2Input_Player;
            else if (parent->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed)
                parent->stateInput = Player_ProcessP2Input_AI;

            Player_CheckHit(parent, self);
            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_BOSS), self->position.x, self->position.y)->drawOrder = Zone->drawOrderHigh + 2;
            self->invincibilityTimer                                                                             = 30;
            self->state                                                                                          = Gachapandora_State_Destroyed;
        }
        else if (self->timer == 30) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 7, &self->animator1, true, 0);
        }
    }
}

void Gachapandora_State7_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    if (!--self->timer) {
        self->state     = Gachapandora_State7_Unknown2;
        self->stateDraw = Gachapandora_StateDraw7_Unknown1;
    }
}

void Gachapandora_State7_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Gachapandora_State7_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        self->velocity.y = -0x20000;
        self->velocity.x = RSDK.Rand(-2, 3) << 16;
        self->state      = Gachapandora_State7_Unknown2;
        self->stateDraw  = Gachapandora_StateDraw7_Unknown1;
    }
}

void Gachapandora_StateDraw7_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    if (Zone->timer & 1)
        RSDK.DrawSprite(&self->animator1, NULL, false);
}

void Gachapandora_State8_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);

    Entity *parent = self->parent;
    if (parent) {
        self->position.x = parent->position.x + self->posUnknown.x;
        self->position.y = parent->position.y + self->posUnknown.y;
    }

    if (self->animator1.animationID >= 14 && (self->animator1.animationID != 16 || self->animator1.frameID < 2)) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox6)) {
                Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
            }
        }
    }

    if (self->animator1.frameID == self->animator1.frameCount - 1)
        destroyEntity(self);
}

void Gachapandora_State8_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    if (self->velocity.y < 0x20000)
        self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitbox6)) {
            Player_CheckElementalHit(player, self, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 16, &self->animator1, true, 0);
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = Gachapandora_State8_Unknown1;
    }
}

void Gachapandora_State1_Unknown1(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (self->position.y >= self->parent->position.y + 0xE0000) {
        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 5, &self->animator1, true, 0);
        self->state = Gachapandora_State1_Unknown2;
    }
}

void Gachapandora_State1_Unknown2(void)
{
    RSDK_THIS(Gachapandora);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x180000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = Gachapandora_State1_Unknown3;
    }
}

void Gachapandora_State1_Unknown3(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->animator1);
    if (self->animator1.frameID >= 4) {
        if (self->velocity.x < 0x60000)
            self->velocity.x += 0xC00;
        self->position.x += self->velocity.x;
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void Gachapandora_EditorDraw(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &self->animator1, false, 0);
    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->animator2, false, 0);
    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->animator3, false, 7);
    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &self->animator4, false, 7);
    
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    self->animator1.frameID = 4;
    RSDK.DrawSprite(&self->animator1, NULL, false);
    self->animator1.frameID = 5;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 2;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 3;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->animator1.frameID = 0;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    int storeDir      = self->direction;
    self->direction = Gachapandora->value4;
    drawPos.x         = self->position.x;
    drawPos.y         = self->position.y - 0x100000;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    self->direction = storeDir;

    self->animator1.frameID = 1;
    RSDK.DrawSprite(&self->animator1, NULL, false);

    self->direction = Gachapandora->handleDir;
    drawPos.y += 0x1A0000;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    self->direction = storeDir;
}

void Gachapandora_EditorLoad(void)
{
    Gachapandora->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Gachapandora.bin", SCOPE_STAGE);
    Gachapandora->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ2.bin", SCOPE_STAGE);
}
#endif

void Gachapandora_Serialize(void) {}
