// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Gachapandora Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGachapandora *Gachapandora;

void Gachapandora_Update(void)
{
    RSDK_THIS(Gachapandora);

    StateMachine_Run(self->state);
}

void Gachapandora_LateUpdate(void) {}

void Gachapandora_StaticUpdate(void)
{
    // Bug Details:
    // there's no NULL check here for Gachapandora->eggman so this will be called even on MMZ1 and its NULL.
    // Gachapandora->eggman also isn't cleared on StageLoad so this can point to a random entity
    RSDK.AddDrawListRef(Zone->objectDrawGroup[0] + 1, RSDK.GetEntitySlot(Gachapandora->eggman));
}

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
                self->type = VOID_TO_INT(data);

            int32 capsuleOffset = 0;
            switch (self->type) {
                case GACHAPANDORA_MAIN:
                    self->visible   = false;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &self->mainAnimator, true, 0);
                    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->handleAnimator, true, 7);
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &self->capsuleAnimator, true, 7);

                    self->hitbox.left   = -33;
                    self->hitbox.top    = -37;
                    self->hitbox.right  = 33;
                    self->hitbox.bottom = 6;

                    self->startY         = self->position.y;
                    self->updateRange.x  = 0x800000;
                    self->updateRange.y  = 0x800000;
                    Gachapandora->eggman = self;

                    capsuleOffset = 55;
                    for (int32 i = GACHAPANDORA_CAPSULE_COUNT - 1; i >= 0; --i) {
                        int32 id = 0;

                        Gachapandora->capsuleTypeL[i] = RSDK.Rand(0, 6);
                        id                            = Gachapandora->capsuleTypeL[i] % 3;

                        while (id == Gachapandora->lastCapsuleType && Gachapandora->capsuleCount[id] == 3) {
                            Gachapandora->capsuleTypeL[i] = RSDK.Rand(0, 6);
                            id                            = Gachapandora->capsuleTypeL[i] % 3;
                        }

                        Gachapandora->lastCapsuleType = Gachapandora->capsuleTypeL[i] % 3;
                        ++Gachapandora->capsuleCount[Gachapandora->lastCapsuleType];
                        Gachapandora->capsuleOffsetL[i] = 4 * capsuleOffset;
                        Gachapandora->capsuleMaxL[i]    = 4 * capsuleOffset;

                        Gachapandora->capsuleTypeR[i] = RSDK.Rand(0, 6);
                        id                            = Gachapandora->capsuleTypeR[i] % 3;

                        while (id == Gachapandora->lastCapsuleType && i && Gachapandora->capsuleCount[id] == 3) {
                            Gachapandora->capsuleTypeR[i] = RSDK.Rand(0, 6);
                            id                            = Gachapandora->capsuleTypeR[i] % 3;
                        }

                        Gachapandora->lastCapsuleType = Gachapandora->capsuleTypeR[i] % 3;
                        ++Gachapandora->capsuleCount[Gachapandora->lastCapsuleType];
                        Gachapandora->capsuleOffsetR[i] = 4 * capsuleOffset;
                        Gachapandora->capsuleMaxR[i]    = 4 * capsuleOffset;

                        capsuleOffset -= 18;
                    }

                    Gachapandora->nextPrizeType   = 0;
                    Gachapandora->handleSpinTimer = 0x7FFF;
                    Gachapandora->nextCapsuleL    = 3;
                    Gachapandora->nextCapsuleR    = 3;
                    Gachapandora->capsuleSide     = RSDK.Rand(0, 2);

                    if (Gachapandora->capsuleSide == 1) {
                        for (int32 i = 0; i < GACHAPANDORA_CAPSULE_COUNT; ++i) {
                            int32 store                   = Gachapandora->capsuleTypeL[i];
                            Gachapandora->capsuleTypeL[i] = Gachapandora->capsuleTypeR[i];
                            Gachapandora->capsuleTypeR[i] = store;
                        }
                    }

                    self->state     = Gachapandora_StateBoss_SetupArena;
                    self->stateDraw = Gachapandora_Draw_Boss;
                    break;

                case GACHAPANDORA_EGGMAN:
                    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 4, &self->mainAnimator, true, 0);

                    self->state     = Gachapandora_StateEggman_Falling;
                    self->stateDraw = Gachapandora_Draw_Simple;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[0];
                    break;

                case GACHAPANDORA_PRIZE:
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &self->mainAnimator, true, 0);

                    self->state     = Gachapandora_StatePrize_CapsuleFall;
                    self->stateDraw = Gachapandora_Draw_Prize;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[0];
                    break;

                case GACHAPANDORA_AMY:
                case GACHAPANDORA_DRILLER:
                case GACHAPANDORA_FIREDROPPER: break;

                case GACHAPANDORA_FIREBALL:
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 11, &self->mainAnimator, true, 0);

                    self->hitbox.left   = -2;
                    self->hitbox.top    = -2;
                    self->hitbox.right  = 2;
                    self->hitbox.bottom = 2;
                    self->state         = Gachapandora_StateFireball_Falling;
                    self->stateDraw     = Gachapandora_Draw_Simple;
                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    self->active        = ACTIVE_NORMAL;
                    self->visible       = true;
                    self->drawGroup     = Zone->objectDrawGroup[0];
                    break;

                case GACHAPANDORA_DEBRIS:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[0];

                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &self->mainAnimator, true, 0);

                    self->state     = Gachapandora_StateDebris_Delay;
                    self->stateDraw = Gachapandora_Draw_Simple;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
                    break;

                case GACHAPANDORA_SPARK:
                    self->active    = ACTIVE_NORMAL;
                    self->visible   = true;
                    self->drawGroup = Zone->objectDrawGroup[1];

                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 13, &self->mainAnimator, true, 0);

                    self->state     = Gachapandora_StateSpark_Attatched;
                    self->stateDraw = Gachapandora_Draw_Simple;

                    self->updateRange.x = 0x800000;
                    self->updateRange.y = 0x800000;
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

    Gachapandora->hitboxHandle.left   = -16;
    Gachapandora->hitboxHandle.top    = 22;
    Gachapandora->hitboxHandle.right  = 16;
    Gachapandora->hitboxHandle.bottom = 30;

    Gachapandora->hitboxAmyRange.left   = -128;
    Gachapandora->hitboxAmyRange.top    = -256;
    Gachapandora->hitboxAmyRange.right  = 128;
    Gachapandora->hitboxAmyRange.bottom = 256;

    Gachapandora->hitboxAmy.left   = -32;
    Gachapandora->hitboxAmy.top    = -32;
    Gachapandora->hitboxAmy.right  = 0;
    Gachapandora->hitboxAmy.bottom = 32;

    Gachapandora->hitboxFireball.left   = -5;
    Gachapandora->hitboxFireball.top    = -11;
    Gachapandora->hitboxFireball.right  = 5;
    Gachapandora->hitboxFireball.bottom = 0;

    Gachapandora->hitboxDrill.left   = -28;
    Gachapandora->hitboxDrill.top    = 3;
    Gachapandora->hitboxDrill.right  = -20;
    Gachapandora->hitboxDrill.bottom = 10;

    Gachapandora->hitboxSpark.left   = -2;
    Gachapandora->hitboxSpark.top    = 2;
    Gachapandora->hitboxSpark.right  = -2;
    Gachapandora->hitboxSpark.bottom = 2;

    Gachapandora->active           = ACTIVE_ALWAYS;
    Gachapandora->activeToys       = 0;
    Gachapandora->awardAchievement = false;
    Gachapandora->destroyedToys    = 0;
    Gachapandora->lastCapsuleType  = -1;
    Gachapandora->capsuleCount[0]  = 0;
    Gachapandora->capsuleCount[1]  = 0;
    Gachapandora->capsuleCount[2]  = 0;

    Gachapandora->sfxHit       = RSDK.GetSfx("Stage/BossHit.wav");
    Gachapandora->sfxExplosion = RSDK.GetSfx("Stage/Explosion2.wav");
    Gachapandora->sfxUnravel   = RSDK.GetSfx("Stage/Unravel.wav");
    Gachapandora->sfxPon       = RSDK.GetSfx("Stage/Pon.wav");
    Gachapandora->sfxFireball  = RSDK.GetSfx("Stage/Fireball.wav");
    Gachapandora->sfxGiggle    = RSDK.GetSfx("MMZ/Giggle.wav");
}

void Gachapandora_CheckPlayerCollisions_Prize(void)
{
    RSDK_THIS(Gachapandora);

    foreach_active(Player, player)
    {
        if (player->animator.animationID != ANI_VICTORY) {
            if (Player_CheckBadnikTouch(player, self, &self->hitbox)) {
                int32 blink = player->blinkTimer;
                if (self->type == GACHAPANDORA_AMY)
                    player->blinkTimer = 1;

                bool32 wasHit = false;
                if (Player_CheckBossHit(player, self)) {
                    if (self->health)
                        self->health--;

                    if (self->health) {
                        RSDK.PlaySfx(Gachapandora->sfxHit, false, 255);
                    }
                    else {
                        EntityGachapandora *eggman = Gachapandora->eggman;
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &eggman->eggmanAnimator, true, 0);
                        RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
                        self->state = Gachapandora_StatePrize_Destroyed;
                    }
                    self->invincibilityTimer = 30;
                    wasHit                   = true;
                }

                if (self->type == GACHAPANDORA_AMY)
                    player->blinkTimer = blink;

                if (wasHit)
                    foreach_break;
            }
            else {
                if (self->type == GACHAPANDORA_DRILLER) {
                    if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxDrill))
                        Player_Hurt(player, self);
                }
            }
        }
    }
}

void Gachapandora_Explode(int32 xMin, int32 xMax, int32 yMin, int32 yMax)
{
    RSDK_THIS(Gachapandora);

    int32 interval = 4 * (self->state == Gachapandora_StateBoss_Explode) + 3;

    if (!(Zone->timer % interval)) {
        RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);

        if (Zone->timer & 4) {
            int32 x = self->position.x + (RSDK.Rand(xMin, xMax) << 16);
            int32 y = self->position.y + (RSDK.Rand(yMin, yMax) << 16);
            CREATE_ENTITY(Explosion, INT_TO_VOID((RSDK.Rand(0, 256) > 192) + EXPLOSION_BOSS), x, y)->drawGroup = Zone->objectDrawGroup[1] + 2;
        }
    }
}

void Gachapandora_Player_StateInput_P1Grabbed(void)
{
    RSDK_THIS(Player);

    Player_Input_P1();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    self->velocity.x = CLAMP(self->velocity.x, -0x20000, 0x20000);
    self->groundVel  = CLAMP(self->groundVel, -0x20000, 0x20000);
}

void Gachapandora_Player_StateInput_P2PlayerGrabbed(void)
{
    RSDK_THIS(Player);

    Player_Input_P2_Player();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    self->velocity.x = CLAMP(self->velocity.x, -0x40000, 0x40000);
    self->groundVel  = CLAMP(self->groundVel, -0x40000, 0x40000);

    if (self->stateInput == Player_Input_P2_AI)
        self->stateInput = Gachapandora_Player_StateInput_P2AIGrabbed;
}

void Gachapandora_Player_StateInput_P2AIGrabbed(void)
{
    RSDK_THIS(Player);

    Player_Input_P2_AI();

    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    self->velocity.x = CLAMP(self->velocity.x, -0x40000, 0x40000);
    self->groundVel  = CLAMP(self->groundVel, -0x40000, 0x40000);

    if (self->stateInput == Player_Input_P2_Player)
        self->stateInput = Gachapandora_Player_StateInput_P2PlayerGrabbed;
}

void Gachapandora_HandleSparksAndDebris(void)
{
    RSDK_THIS(Gachapandora);

    // is this line meant to be here??
    // there's no check if this is 0 so it automatically goes to -1/255 (which is far too long)
    // which would be fine, but the fact there's a switch case for health implies otherwise
    --Gachapandora->debrisTimer;

    if (!Gachapandora->debrisTimer) {
        switch (self->health) {
            default:
            case 0: break;

            case 1:
            case 2: Gachapandora->debrisTimer = RSDK.Rand(8, 16); break;

            case 3: Gachapandora->debrisTimer = RSDK.Rand(15, 31); break;
            case 4: Gachapandora->debrisTimer = RSDK.Rand(30, 61); break;
            case 5: Gachapandora->debrisTimer = RSDK.Rand(60, 91); break;

            case 6:
            case 7: Gachapandora->debrisTimer = RSDK.Rand(90, 121); break;

            case 8:
            case 9: return;
        }

        int32 type = RSDK.Rand(0, 3);
        switch (type) {
            case 2:
            case 0: {
                int32 off                  = RSDK.Rand(-12, 12);
                EntityGachapandora *debris = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_DEBRIS),
                                                           self->position.x + (off << 16) + self->velocity.x, self->position.y + 0x300000);

                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, RSDK.Rand(0, 4) + 18, &debris->mainAnimator, true, 0);

                if (self->state != Gachapandora_StateBoss_HandleSpun && !self->invincibilityTimer)
                    debris->velocity.x = self->velocity.x >> 1;

                debris->state = Gachapandora_StateDebris_BossDebris;
                break;
            }

            case 1: {
                EntityGachapandora *spark = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_SPARK), self->position.x, self->position.y);
                spark->parent             = self;

                if (RSDK.Rand(0, 2))
                    spark->originPos.x = (RSDK.Rand(-6, 6) - 26) << 16;
                else
                    spark->originPos.x = (RSDK.Rand(-6, 6) + 26) << 16;
                spark->originPos.y = (RSDK.Rand(-8, 8) + 20) << 16;

                RSDK.SetSpriteAnimation(Explosion->aniFrames, 3, &spark->mainAnimator, true, 0);
                break;
            }
        }
    }

    if (self->state == Gachapandora_StateBoss_LastDitchAttack) {
        for (int32 i = 0; i < 2; ++i) {
            if (!--Gachapandora->sideSparkTimer[i]) {
                Gachapandora->sideSparkTimer[i] = RSDK.Rand(16, 30);

                EntityGachapandora *spark = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_SPARK), self->position.x, self->position.y);
                spark->parent             = self;

                if (i) {
                    spark->originPos.x = (RSDK.Rand(-2, 2) - 34) << 16;
                }
                else {
                    spark->direction   = FLIP_X;
                    spark->originPos.x = (RSDK.Rand(-2, 2) + 34) << 16;
                }
                spark->originPos.y = (RSDK.Rand(-8, 8) + 20) << 16;

                if (RSDK.Rand(0, 3) == 2)
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 14, &spark->mainAnimator, true, 0);
            }
        }
    }
}

void Gachapandora_HandleAnimations(void)
{
    RSDK_THIS(Gachapandora);

    if (self->eggmanAnimator.animationID) {
        if (self->eggmanAnimator.animationID == 1) {
            if (self->eggmanAnimator.frameID >= self->eggmanAnimator.frameCount - 1)
                RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
        }
        else {
            if (self->eggmanAnimator.animationID == 2) {
                bool32 wasHit = false;
                foreach_active(Player, player)
                {
                    if (player->state == Player_State_Hurt || player->state == Player_State_Death)
                        wasHit = true;
                }

                if (self->eggmanAnimator.frameID >= self->eggmanAnimator.frameCount - 1) {
                    if (wasHit)
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &self->eggmanAnimator, true, 6);
                    else
                        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->eggmanAnimator, true, 0);
                }
            }
        }
    }
    else {
        bool32 wasHit = false;
        foreach_active(Player, player)
        {
            if (player->state == Player_State_Hurt || player->state == Player_State_Death)
                wasHit = true;
        }

        if (wasHit)
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &self->eggmanAnimator, true, 0);
    }
}

void Gachapandora_StateBoss_SetupArena(void)
{
    RSDK_THIS(Gachapandora);

    if (++self->timer >= 2) {
        self->timer = 0;

        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveB[0] = true;
        Zone->cameraBoundsR[0]      = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsB[0]      = (self->position.y >> 16) + 128;

        self->position.y = (ScreenInfo->position.y - 192) << 16;
        self->active     = ACTIVE_NORMAL;
        self->visible    = true;
        self->state      = Gachapandora_StateBoss_AwaitPlayer;
    }
}

void Gachapandora_StateBoss_AwaitPlayer(void)
{
    RSDK_THIS(Gachapandora);

    Zone->playerBoundActiveL[0] = true;
    Zone->cameraBoundsL[0]      = ScreenInfo->position.x;

    if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x) {
        Zone->playerBoundActiveL[0] = true;
        Zone->cameraBoundsL[0]      = (self->position.x >> 16) - ScreenInfo->center.x;

        Music_TransitionTrack(TRACK_EGGMAN2, 0.0125);
        self->health = 9;
        self->state  = Gachapandora_StateBoss_EnterEggman;
    }
}

void Gachapandora_StateBoss_EnterEggman(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);

    self->position.y = self->position.y + 0x10000;

    if (self->position.y < self->startY) {
        if (self->position.y >= self->startY - 0x400000)
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 2, &self->eggmanAnimator, false, 0);
    }
    else {
        self->position.y  = self->startY;
        self->originPos.x = self->position.x;
        self->originPos.y = self->position.y;
        self->angle       = 0;
        self->state       = Gachapandora_StateBoss_FloatAround;
    }

    RSDK.AddDrawListRef(Zone->objectDrawGroup[0] + 1, SceneInfo->entitySlot);
}

void Gachapandora_StateBoss_FloatAround(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);

    Gachapandora_HandleAnimations();

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (self->invincibilityTimer) {
        if (!--self->invincibilityTimer && !self->velocity.x) {
            self->velocity.x = self->direction == FLIP_NONE ? -0x8000 : 0x8000;
        }
    }
    else {
        if (self->position.x < (ScreenInfo->position.x + 64) << 16 || self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 64) << 16)
            self->velocity.x = -self->velocity.x;

        self->position.x += self->velocity.x;
    }

    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->eggmanDir ^= FLIP_X;

    for (int32 i = 0; i < GACHAPANDORA_CAPSULE_COUNT; ++i) {
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
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxHandle)) {
                RSDK.PlaySfx(Gachapandora->sfxUnravel, false, 255);

                if (player->position.x >= self->position.x)
                    Gachapandora->handleDir = player->velocity.y < 0;
                else
                    Gachapandora->handleDir = player->velocity.y > 0;

                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->handleAnimator, true, 0);
                if (Gachapandora->nextCapsuleL + Gachapandora->nextCapsuleR == 0x1FE) {
                    self->invincibilityTimer = 25;
                    self->state              = Gachapandora_StateBoss_Defeated;
                }
                else {
                    self->timer = 60;
                    int32 id    = 0;
                    if (Gachapandora->capsuleSide) {
                        Gachapandora->handleSpinTimer                            = 148;
                        Gachapandora->nextPrizeType                              = Gachapandora->capsuleTypeR[Gachapandora->nextCapsuleR];
                        Gachapandora->capsuleOffsetR[Gachapandora->nextCapsuleR] = 0xFF;
                        Gachapandora->capsuleMaxR[Gachapandora->nextCapsuleR--]  = 0xFF;

                        while (id <= Gachapandora->nextCapsuleR) {
                            Gachapandora->capsuleDelayR[id] = 8 * (Gachapandora->nextCapsuleR - id + 2);
                            Gachapandora->capsuleMaxR[id] += 72;
                            Gachapandora->capsuleSpeedR[id++] = 0;
                        }
                    }
                    else {
                        Gachapandora->handleSpinTimer                            = -148;
                        Gachapandora->nextPrizeType                              = Gachapandora->capsuleTypeL[Gachapandora->nextCapsuleL];
                        Gachapandora->capsuleOffsetL[Gachapandora->nextCapsuleL] = 0xFF;
                        Gachapandora->capsuleMaxL[Gachapandora->nextCapsuleL--]  = 0xFF;

                        while (id <= Gachapandora->nextCapsuleL) {
                            Gachapandora->capsuleDelayL[id] = 8 * (Gachapandora->nextCapsuleL - id + 2);
                            Gachapandora->capsuleMaxL[id] += 72;
                            Gachapandora->capsuleSpeedL[id++] = 0;
                        }
                    }

                    Gachapandora->capsuleSide ^= 1;
                    self->velocity.x += self->velocity.x >= 0 ? 0x4000 : -0x4000;
                    self->state = Gachapandora_StateBoss_HandleSpun;
                }
                foreach_break;
            }
        }
    }
    else {
        self->timer--;
    }
}

void Gachapandora_StateBoss_HandleSpun(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);

    Gachapandora_HandleAnimations();

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);

    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->eggmanDir ^= FLIP_X;

    for (int32 i = 0; i < GACHAPANDORA_CAPSULE_COUNT; ++i) {
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

    if (Gachapandora->handleSpinTimer) {
        if (Gachapandora->handleSpinTimer >= 0) {
            Gachapandora->handleSpinTimer -= 3;

            if (Gachapandora->handleSpinTimer < 0)
                Gachapandora->handleSpinTimer = 0;
        }
        else {
            Gachapandora->handleSpinTimer += 3;
            if (Gachapandora->handleSpinTimer > 0)
                Gachapandora->handleSpinTimer = 0;
        }
    }
    else {
        EntityGachapandora *prize = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_PRIZE), self->position.x, self->position.y + 0x1A0000);
        prize->type               = Gachapandora->nextPrizeType;

        switch (prize->type) {
            default: break;
            case 0:
            case 1: prize->mainAnimator.frameID = 0; break;

            case 2: prize->mainAnimator.frameID = 1; break;

            case 3:
            case 4: prize->mainAnimator.frameID = 2; break;

            case 5: prize->mainAnimator.frameID = 3; break;
        }

        Gachapandora->handleSpinTimer = 0x7FFF;

        if (--self->health == 1) {
            EntityGachapandora *debris   = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            debris->mainAnimator.frameID = 4;
            debris->timer                = 30;
            debris->velocity.x           = 0x20000;
            debris->velocity.y           = -0x20000;

            debris                       = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            debris->mainAnimator.frameID = 5;
            debris->timer                = 30;
            debris->velocity.x           = -0x20000;
            debris->velocity.y           = -0x20000;

            debris = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            ++debris->drawGroup;
            debris->mainAnimator.frameID = 2;
            debris->timer                = 90;
            debris->velocity.x           = -0x20000;
            debris->velocity.y           = -0x20000;

            debris = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_DEBRIS), self->position.x, self->position.y);
            ++debris->drawGroup;
            debris->mainAnimator.frameID = 3;
            debris->timer                = 90;
            debris->velocity.x           = 0x20000;
            debris->velocity.y           = -0x20000;

            self->invincibilityTimer = 120;

            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &self->eggmanAnimator, true, 0);
            RSDK.SetSpriteAnimation(-1, 0, &self->capsuleAnimator, true, 0);

            self->state     = Gachapandora_StateBoss_Explode;
            self->stateDraw = Gachapandora_Draw_BossDestroyed;
        }
        else {
            self->invincibilityTimer = 30;
            self->state              = Gachapandora_StateBoss_FloatAround;
        }
    }
}

void Gachapandora_StateBoss_Explode(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    Gachapandora_Explode(-24, 24, -48, 48);
    Gachapandora_HandleSparksAndDebris();

    if (--self->invincibilityTimer <= 0)
        self->state = Gachapandora_StateBoss_PanicFlee;
}

void Gachapandora_StateBoss_PanicFlee(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);

    Gachapandora_HandleSparksAndDebris();

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x1800;

    if (self->position.y < (ScreenInfo->position.y - 192) << 16) {
        self->position.y = (ScreenInfo->position.y - 192) << 16;
        self->position.x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        self->state      = Gachapandora_StateBoss_EnsureAllCapsulesDestroyed;
    }
}

void Gachapandora_StateBoss_EnsureAllCapsulesDestroyed(void)
{
    RSDK_THIS(Gachapandora);

    if (Gachapandora->destroyedToys >= 8) {
        self->originPos.y = self->position.y;
        self->angle       = 0;

        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 17, &self->capsuleAnimator, true, 0);

        self->timer = 60;
        self->velocity.x >>= 1;
        self->state = Gachapandora_StateBoss_LastDitchAttack;
    }
}

void Gachapandora_StateBoss_LastDitchAttack(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);
    RSDK.ProcessAnimation(&self->capsuleAnimator);

    Gachapandora_HandleAnimations();

    if (self->originPos.y >= self->startY) {
        self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    }
    else {
        self->originPos.y += 0x10000;
        self->position.y = self->originPos.y;
    }

    Gachapandora_HandleSparksAndDebris();

    if (self->position.x < (ScreenInfo->position.x + 32) << 16 || self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 32) << 16)
        self->velocity.x = -self->velocity.x;

    self->position.x = self->position.x + self->velocity.x;

    if (RSDK.Rand(0, 64) == 21)
        Gachapandora->eggmanDir ^= FLIP_X;

    if (--self->timer <= 0) {
        self->timer = 60;
        for (int32 i = 0; i < 2; ++i) {
            EntityGachapandora *spark = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_SPARK), self->position.x, self->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 15, &spark->mainAnimator, true, 0);

            if (i) {
                spark->position.x += (RSDK.Rand(-2, 2) - 34) << 16;
                spark->velocity.x = self->velocity.x - 0x10000;
            }
            else {
                spark->direction = FLIP_X;
                spark->position.x += (RSDK.Rand(-2, 2) + 34) << 16;
                spark->velocity.x = self->velocity.x + 0x10000;
            }

            spark->state = Gachapandora_StateSpark_Detatched;
            spark->position.y += (RSDK.Rand(-8, 8) + 20) << 16;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxHandle)) {
            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 4, &self->eggmanAnimator, true, 0);
            RSDK.PlaySfx(Gachapandora->sfxUnravel, false, 255);

            if (player->position.x >= self->position.x)
                Gachapandora->handleDir = player->velocity.y < 0;
            else
                Gachapandora->handleDir = player->velocity.y > 0;

            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->handleAnimator, true, 0);
            self->invincibilityTimer = 25;
            self->velocity.y         = 0;
            self->state              = Gachapandora_StateBoss_Defeated;
            foreach_break;
        }
    }
}

void Gachapandora_StateBoss_Defeated(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_HandleSparksAndDebris();

    self->position.y += self->velocity.y;
    self->velocity.y -= 0x800;

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);
    RSDK.ProcessAnimation(&self->capsuleAnimator);

    Gachapandora_HandleSparksAndDebris();

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    if (!--self->invincibilityTimer) {
        CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_EGGMAN), self->position.x, self->position.y - 0x100000)->parent = self;
        RSDK.SetSpriteAnimation(-1, 0, &self->eggmanAnimator, true, 0);
        self->invincibilityTimer = 60;
        self->state              = Gachapandora_StateBoss_EggmanFallOut;
    }
}

void Gachapandora_StateBoss_EggmanFallOut(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);
    RSDK.ProcessAnimation(&self->capsuleAnimator);

    Gachapandora_HandleSparksAndDebris();

    self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 2, 10);
    self->position.y += self->velocity.y;
    self->velocity.y -= 0x800;

    if (!--self->invincibilityTimer) {
        SceneInfo->timeEnabled = false;
        Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 1000);
        self->invincibilityTimer = 120;
        self->state              = Gachapandora_StateBoss_Finish;
    }
}

void Gachapandora_StateBoss_Finish(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->eggmanAnimator);
    RSDK.ProcessAnimation(&self->handleAnimator);
    RSDK.ProcessAnimation(&self->capsuleAnimator);

    Gachapandora_HandleSparksAndDebris();
    Gachapandora_Explode(-24, 24, -48, 48);

    if (self->invincibilityTimer) {
        if (!--self->invincibilityTimer || self->invincibilityTimer == 96)
            self->velocity.y = -0x40000;
    }
    else {
        self->position.y += self->velocity.y;
        self->velocity.y += 0x1800;

        if (self->position.y > (ScreenInfo->position.y + ScreenInfo->size.y + 128) << 16) {
            Zone->cameraBoundsR[0] += WIDE_SCR_XSIZE;
            self->state = StateMachine_None;
            Music_TransitionTrack(TRACK_STAGE, 0.0125);
        }
    }
}

void Gachapandora_Draw_Boss(void)
{
    RSDK_THIS(Gachapandora);

    Vector2 drawPos = self->position;
    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0]) {
        self->mainAnimator.frameID = 4;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->mainAnimator.frameID = 5;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        for (int32 i = 0; i < GACHAPANDORA_CAPSULE_COUNT; ++i) {
            if (Gachapandora->capsuleOffsetL[i] != 0xFF) {
                self->capsuleAnimator.frameID = ((Gachapandora->capsuleTypeL[i] % 3) >> 1) + 2 * (Gachapandora->capsuleTypeL[i] / 3);
                drawPos.x                     = self->position.x - (Gachapandora->capsuleOffsets[56 - (Gachapandora->capsuleOffsetL[i] >> 2)] << 16);
                drawPos.y                     = self->position.y + (((Gachapandora->capsuleOffsetL[i] & 0xFC) - 152) << 14);
                RSDK.DrawSprite(&self->capsuleAnimator, &drawPos, false);
            }

            if (Gachapandora->capsuleOffsetR[i] != 0xFF) {
                self->capsuleAnimator.frameID = ((Gachapandora->capsuleTypeR[i] % 3) >> 1) + 2 * (Gachapandora->capsuleTypeR[i] / 3);
                drawPos.x                     = self->position.x + (Gachapandora->capsuleOffsets[56 - (Gachapandora->capsuleOffsetR[i] >> 2)] << 16);
                drawPos.y                     = self->position.y + (((Gachapandora->capsuleOffsetR[i] & 0xFC) - 152) << 14);
                RSDK.DrawSprite(&self->capsuleAnimator, &drawPos, false);
            }
        }

        if (Gachapandora->handleSpinTimer != 0x7FFF) {
            self->capsuleAnimator.frameID = ((Gachapandora->nextPrizeType % 3) >> 1) + 2 * (Gachapandora->nextPrizeType / 3);
            drawPos.x                     = self->position.x + (Gachapandora->handleSpinTimer << 14);
            drawPos.y                     = self->position.y + 0x120000;
            RSDK.DrawSprite(&self->capsuleAnimator, &drawPos, false);
        }

        self->mainAnimator.frameID = 2;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->mainAnimator.frameID = 3;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->mainAnimator.frameID = 0;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        int32 storeDir  = self->direction;
        self->direction = Gachapandora->eggmanDir;
        drawPos.x       = self->position.x;
        drawPos.y       = self->position.y - 0x100000;
        RSDK.DrawSprite(&self->eggmanAnimator, &drawPos, false);

        self->direction = storeDir;
    }
    else {
        self->mainAnimator.frameID = 1;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        int32 storeDir  = self->direction;
        self->direction = Gachapandora->handleDir;
        drawPos.y += 0x1A0000;
        RSDK.DrawSprite(&self->handleAnimator, &drawPos, false);

        self->direction = storeDir;
    }
}

void Gachapandora_Draw_BossDestroyed(void)
{
    RSDK_THIS(Gachapandora);

    int32 storeDir  = self->direction;
    Vector2 drawPos = self->position;
    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0]) {
        self->mainAnimator.frameID = 0;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        self->direction = Gachapandora->eggmanDir;
        drawPos.x       = self->position.x;
        drawPos.y       = self->position.y - 0x100000;
        RSDK.DrawSprite(&self->eggmanAnimator, &drawPos, false);
    }
    else {
        self->mainAnimator.frameID = 1;
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);

        storeDir        = self->direction;
        self->direction = Gachapandora->handleDir;
        drawPos.y += 0x1A0000;
        RSDK.DrawSprite(&self->handleAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->capsuleAnimator, NULL, false);

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->capsuleAnimator, NULL, false);
    }

    self->direction = storeDir;
}

void Gachapandora_StatePrize_CapsuleFall(void)
{
    RSDK_THIS(Gachapandora);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x2000;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &debris->animator, true, 4);

        debris->velocity.x      = RSDK.Rand(-1, 2) << 16;
        debris->velocity.y      = -0x48000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        debris = CREATE_ENTITY(Debris, Debris_State_FallAndFlicker, self->position.x, self->position.y);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &debris->animator, true, 5);

        if (self->type > GACHAPANDORA_PRIZE)
            ++debris->animator.frameID;
        debris->velocity.x      = RSDK.Rand(-1, 2) << 15;
        debris->velocity.y      = -0x30000;
        debris->gravityStrength = 0x3800;
        debris->drawGroup       = Zone->objectDrawGroup[1];
        debris->updateRange.x   = 0x400000;
        debris->updateRange.y   = 0x400000;

        RSDK.PlaySfx(Gachapandora->sfxPon, false, 255);
        self->velocity.y = -0x38000;
        switch (self->type) {
            case 0:
            case 3:
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 3, &self->mainAnimator, true, 0);
                self->type = GACHAPANDORA_DRILLER;
                if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x)
                    self->direction = FLIP_X;
                break;

            case 1:
            case 4:
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 4, &self->mainAnimator, true, 0);
                self->type = GACHAPANDORA_FIREDROPPER;
                break;

            case 2:
            case 5:
                if (RSDK_GET_ENTITY(SLOT_PLAYER1, Player)->position.x > self->position.x)
                    self->velocity.x = -0x8000;
                else
                    self->velocity.x = 0x8000;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 5, &self->mainAnimator, true, 0);
                self->type = GACHAPANDORA_AMY;
                break;

            default: break;
        }

        self->drawFX |= FX_SCALE;
        self->scale.x   = 0x20;
        self->scale.y   = 0x20;
        self->drawGroup = Zone->objectDrawGroup[1] + 1;
        if (++Gachapandora->activeToys >= 8 && !Gachapandora->destroyedToys)
            Gachapandora->awardAchievement = true;

        self->state = Gachapandora_StatePrize_ExitCapsule;
    }
}

void Gachapandora_StatePrize_ExitCapsule(void)
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
            RSDK.ProcessAnimation(&self->mainAnimator);

            self->position.x += self->velocity.x;

            if (self->velocity.y >= 0) {
                if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
                    self->velocity.y = 0;

                    self->hitbox.left   = -5;
                    self->hitbox.top    = -11;
                    self->hitbox.right  = 5;
                    self->hitbox.bottom = 11;

                    self->health = 1;
                    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->mainAnimator, true, 0);
                    self->state = Gachapandora_StatePrize_AmyWalk;
                }
            }
            break;

        case GACHAPANDORA_DRILLER:
            if (self->velocity.y >= 0) {
                if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x110000, true)) {
                    self->velocity.x = self->direction == FLIP_NONE ? -0x20000 : 0x20000;
                    self->velocity.y = -0x18000;

                    self->hitbox.left   = -16;
                    self->hitbox.top    = -10;
                    self->hitbox.right  = 25;
                    self->hitbox.bottom = 17;

                    self->health = 3;
                    self->state  = Gachapandora_StatePrize_DrillerCapsuleExitBounce;
                }
            }
            break;

        case GACHAPANDORA_FIREDROPPER:
            if (self->velocity.y >= 0) {
                self->originPos.y = self->position.y;
                self->velocity.x  = self->direction == FLIP_NONE ? -0x18000 : 0x18000;
                self->velocity.y  = 0;

                self->hitbox.left   = -13;
                self->hitbox.top    = -10;
                self->hitbox.right  = 13;
                self->hitbox.bottom = 10;

                self->health = 3;
                self->timer  = 120;
                self->state  = Gachapandora_StatePrize_FireDropperMove;
            }
            break;
    }
}

void Gachapandora_StatePrize_Destroyed(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_Explode(-8, 8, -8, 8);

    if (self->invincibilityTimer) {
        self->invincibilityTimer--;
    }
    else {
        if (++Gachapandora->destroyedToys >= 8 && Gachapandora->awardAchievement)
            API_UnlockAchievement(&achievementList[ACH_MMZ]);

        destroyEntity(self);
    }
}

void Gachapandora_StatePrize_DrillerCapsuleExitBounce(void)
{
    RSDK_THIS(Gachapandora);

    Gachapandora_StatePrize_DrillerMove();

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x110000, true)) {
        self->velocity.y = 0;
        self->state      = Gachapandora_StatePrize_DrillerMove;
    }
    else {
        self->velocity.y += 0x3800;
    }
}

void Gachapandora_StatePrize_DrillerMove(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y += self->velocity.y;
    self->position.x += self->velocity.x;

    if (self->position.x < (ScreenInfo->position.x - 64) << 16 || self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x + 64) << 16) {
        self->direction ^= FLIP_X;
        self->velocity.x = -self->velocity.x;
    }

    if (self->invincibilityTimer)
        self->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions_Prize();
}

void Gachapandora_StatePrize_FireDropperMove(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);
    RSDK.ProcessAnimation(&self->eggmanAnimator);

    self->position.x += self->velocity.x;
    if (self->startY) {
        self->position.y += self->velocity.y;
        self->velocity.y -= 0x800;

        if (self->position.y <= self->originPos.y) {
            self->position.y = self->originPos.y;
            self->velocity.y = 0;
            self->angle      = 0;
            self->startY     = 0;
        }
    }
    else {
        self->position.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 9);
    }

    if (self->position.x < (ScreenInfo->position.x + 16) << 16 || self->position.x > (ScreenInfo->position.x + ScreenInfo->size.x - 16) << 16) {
        self->startY = 1;
        self->direction ^= FLIP_X;
        self->velocity.y = 0x18000;
        self->velocity.x = -self->velocity.x;
    }

    if (!--self->timer) {
        self->timer = 120;
        RSDK.PlaySfx(Gachapandora->sfxFireball, false, 255);
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 10, &self->eggmanAnimator, true, 0);

        EntityGachapandora *fireball = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_FIREBALL), self->position.x, self->position.y);
        fireball->parent             = self;
        fireball->timer              = 12;
    }

    if (self->invincibilityTimer)
        self->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions_Prize();
}

void Gachapandora_StatePrize_AmyWalk(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->invincibilityTimer)
        self->invincibilityTimer--;
    else
        Gachapandora_CheckPlayerCollisions_Prize();

    EntityPlayer *player = Player_GetNearestPlayer();

    if (self->position.x < (ScreenInfo->position.x + 8) << 16 || self->position.x > (ScreenInfo->size.x + ScreenInfo->position.x - 8) << 16
        || Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxAmyRange)) {
        if (abs(player->position.x - self->position.x) > 0x80000)
            self->direction = player->position.x > self->position.x;

        bool32 canJump = false;
        if (self->direction) {
            if (self->velocity.x < 0x20000)
                self->velocity.x += 0xC00;

            canJump = self->velocity.x >= 0x10000;
        }
        else {
            if (self->velocity.x > -0x20000)
                self->velocity.x -= 0xC00;

            canJump = self->velocity.x <= -0x10000;
        }

        if (canJump && Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxAmy)) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 5, &self->mainAnimator, true, 0);
            self->state      = Gachapandora_StatePrize_AmyJump;
            self->onGround   = false;
            self->velocity.y = -0x24000;
        }
    }
    else {
        if (self->velocity.x > 0) {
            self->velocity.x -= 0xC00;

            if (self->velocity.x < 0) {
                self->velocity.x = 0;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->mainAnimator, true, 0);
                self->state = Gachapandora_StatePrize_AmyIdle;
            }
        }
        else {
            self->velocity.x += 0xC00;
            if (self->velocity.x >= 0) {
                self->velocity.x = 0;
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->mainAnimator, true, 0);
                self->state = Gachapandora_StatePrize_AmyIdle;
            }
        }
    }
}

void Gachapandora_StatePrize_AmyIdle(void)
{
    RSDK_THIS(Gachapandora);

    EntityPlayer *player = Player_GetNearestPlayer();

    if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxAmyRange)) {
        self->velocity.x = 0;
        self->direction  = player->position.x > self->position.x;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->mainAnimator, true, 0);

        self->state = Gachapandora_StatePrize_AmyWalk;
        Gachapandora_StatePrize_AmyWalk();
    }

    if (self->state == Gachapandora_StatePrize_AmyIdle) {
        RSDK.ProcessAnimation(&self->mainAnimator);

        if (self->invincibilityTimer)
            self->invincibilityTimer--;
        else
            Gachapandora_CheckPlayerCollisions_Prize();
    }
}

void Gachapandora_StatePrize_AmyJump(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (!self->invincibilityTimer) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxFireball)) {
#if MANIA_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop) {
                    if (Player_CheckBossHit(player, self)) {
                        if (self->health)
                            self->health--;

                        if (self->health) {
                            RSDK.PlaySfx(Gachapandora->sfxHit, false, 255);
                        }
                        else {
                            EntityGachapandora *eggman = Gachapandora->eggman;
                            RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 1, &eggman->eggmanAnimator, true, 0);
                            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
                            self->state = Gachapandora_StatePrize_Destroyed;
                        }

                        self->invincibilityTimer = 30;
                    }
                    foreach_break;
                }
                else {
#endif
                    if (Player_CheckAttacking(player, self) && player->stateInput != Gachapandora_Player_StateInput_P1Grabbed
                        && player->stateInput != Gachapandora_Player_StateInput_P2PlayerGrabbed
                        && player->stateInput != Gachapandora_Player_StateInput_P2AIGrabbed) {
                        bool32 shouldRebound = self->velocity.x >= 0;
                        if (player->position.x > self->position.x) {
                            if (self->velocity.x > 0) {
                                self->velocity.x = -self->velocity.x;
                                shouldRebound    = true;
                            }
                        }

                        if (player->position.x >= self->position.x || shouldRebound) {
                            self->velocity.y = -0x20000;
                            if (self->position.x >= player->position.x) {
                                self->direction    = FLIP_NONE;
                                player->velocity.x = -0x18000;
                            }
                            else {
                                self->direction    = FLIP_X;
                                player->velocity.x = 0x18000;
                            }
                            player->groundVel  = player->velocity.x;
                            player->velocity.y = -player->velocity.y;
                            player->state      = Player_State_Air;
                            if (player->animator.animationID == ANI_SPINDASH)
                                player->animator.animationID = ANI_WALK;
                            self->state = Gachapandora_StatePrize_AmyRebound;
                            foreach_break;
                        }
                    }
                    else {
                        RSDK.PlaySfx(Gachapandora->sfxGiggle, false, 0xFF);
                        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 6, &self->mainAnimator, true, 0);

                        if (player->stateInput == Player_Input_P1)
                            player->stateInput = Gachapandora_Player_StateInput_P1Grabbed;
                        else if (player->stateInput == Player_Input_P2_Player)
                            player->stateInput = Gachapandora_Player_StateInput_P2PlayerGrabbed;
                        else if (player->stateInput == Player_Input_P2_AI)
                            player->stateInput = Gachapandora_Player_StateInput_P2AIGrabbed;

                        self->timer  = 90;
                        self->parent = (EntityGachapandora *)player;
                        self->state  = Gachapandora_StatePrize_AmyGrabbed;
                        foreach_break;
                    }
#if MANIA_USE_PLUS
                }
#endif
            }
        }
    }
    else {
        self->invincibilityTimer--;
    }

    if (self->state == Gachapandora_StatePrize_AmyJump) {
        if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
            self->velocity.y     = 0;
            EntityPlayer *player = Player_GetNearestPlayer();
            if (self->position.x < (ScreenInfo->position.x + 8) << 16 || self->position.x > (ScreenInfo->size.x + ScreenInfo->position.x - 8) << 16
                || Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxAmyRange)) {
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->mainAnimator, true, 0);
                self->state = Gachapandora_StatePrize_AmyWalk;
            }
            else {
                RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->mainAnimator, true, 0);
                self->state = Gachapandora_StatePrize_AmyIdle;
            }
        }
    }
}

void Gachapandora_StatePrize_AmyRebound(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (self->invincibilityTimer)
        self->invincibilityTimer--;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0xC0000, true)) {
        self->velocity.y     = 0;
        EntityPlayer *player = Player_GetNearestPlayer();

        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxAmyRange)) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 9, &self->mainAnimator, true, 0);
            self->state = Gachapandora_StatePrize_AmyWalk;
        }
        else {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 8, &self->mainAnimator, true, 0);
            self->state = Gachapandora_StatePrize_AmyIdle;
        }
    }

    if (self->mainAnimator.animationID == 7) {
        if (!--self->timer) {
            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;

            self->invincibilityTimer = 30;
            self->state              = Gachapandora_StatePrize_Destroyed;
        }
    }
}

void Gachapandora_StatePrize_AmyGrabbed(void)
{
    RSDK_THIS(Gachapandora);

    EntityPlayer *parent = (EntityPlayer *)self->parent;

    RSDK.ProcessAnimation(&self->mainAnimator);
    self->position.x = parent->position.x;
    self->position.y = parent->position.y;

    self->direction = parent->direction ^ FLIP_X;
    self->position.x += self->direction ? -0x60000 : 0x60000;

    if (self->prevShakeFlags) {
        if (self->shakeTimer) {
            self->shakeTimer--;

            uint8 shakeFlags = 0;
            if (parent->left)
                shakeFlags = 1;
            if (parent->right)
                shakeFlags |= 2;

            if (shakeFlags) {
                if (shakeFlags != 3 && shakeFlags != self->prevShakeFlags) {
                    self->prevShakeFlags = shakeFlags;
                    if (++self->shakeCount >= 6) {
                        self->shakeCount = 0;
                        self->parent     = NULL;

                        if (parent->stateInput == Gachapandora_Player_StateInput_P1Grabbed)
                            parent->stateInput = Player_Input_P1;
                        else if (parent->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed)
                            parent->stateInput = Player_Input_P2_Player;
                        else if (parent->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed)
                            parent->stateInput = Player_Input_P2_AI;

                        self->velocity.x = self->direction == FLIP_NONE ? 0x20000 : -0x20000;
                        self->velocity.y = -0x20000;
                        self->state      = Gachapandora_StatePrize_AmyRebound;
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

    if (self->state == Gachapandora_StatePrize_AmyGrabbed) {
        if (!--self->timer) {
            if (parent->stateInput == Gachapandora_Player_StateInput_P1Grabbed)
                parent->stateInput = Player_Input_P1;
            else if (parent->stateInput == Gachapandora_Player_StateInput_P2PlayerGrabbed)
                parent->stateInput = Player_Input_P2_Player;
            else if (parent->stateInput == Gachapandora_Player_StateInput_P2AIGrabbed)
                parent->stateInput = Player_Input_P2_AI;

            Player_Hurt(parent, self);

            RSDK.PlaySfx(Gachapandora->sfxExplosion, false, 255);
            CREATE_ENTITY(Explosion, INT_TO_VOID(EXPLOSION_BOSS), self->position.x, self->position.y)->drawGroup = Zone->objectDrawGroup[1] + 2;

            self->invincibilityTimer = 30;
            self->state              = Gachapandora_StatePrize_Destroyed;
        }
        else if (self->timer == 30) {
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 7, &self->mainAnimator, true, 0);
        }
    }
}

void Gachapandora_Draw_Prize(void)
{
    RSDK_THIS(Gachapandora);

    if (self->invincibilityTimer & 1)
        RSDK.SetPaletteEntry(0, 128, 0xE0E0E0);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->eggmanAnimator, NULL, false);

    RSDK.SetPaletteEntry(0, 128, 0x000000);
}

void Gachapandora_StateFireball_Falling(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->timer) {
        EntityGachapandora *parent = self->parent;
        if (parent->direction)
            self->position.x = parent->position.x + 0x20000;
        else
            self->position.x = parent->position.x - 0x20000;
        self->position.y += parent->velocity.y;
        self->timer--;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxFireball)) {
            Player_ElementHurt(player, self, SHIELD_FIRE);
        }
    }

    self->position.y += 0x18000;
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x20000, true)) {
        self->timer  = 6;
        self->health = 4;
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &self->mainAnimator, true, 0);
        self->state = Gachapandora_StateFireball_BurnGround;
    }
}

void Gachapandora_StateFireball_BurnGround(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->timer) {
        if (!--self->timer) {
            EntityGachapandora *fireball =
                CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_FIREBALL), self->position.x + 0x40000, self->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &fireball->mainAnimator, true, 0);
            fireball->state = Gachapandora_StateFireball_BurnGround;
            if (self->health) {
                fireball->timer  = 6;
                fireball->health = self->health - 1;
            }

            fireball = CREATE_ENTITY(Gachapandora, INT_TO_VOID(GACHAPANDORA_FIREBALL), self->position.x - 0x40000, self->position.y);
            RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 12, &fireball->mainAnimator, true, 0);
            fireball->state = Gachapandora_StateFireball_BurnGround;
            if (self->health) {
                fireball->timer  = 6;
                fireball->health = self->health - 1;
            }
        }
    }
    if (self->mainAnimator.frameID < 5) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxFireball)) {
                Player_ElementHurt(player, self, SHIELD_FIRE);
            }
        }
    }

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

void Gachapandora_Draw_Simple(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void Gachapandora_StateDebris_Delay(void)
{
    RSDK_THIS(Gachapandora);

    if (!--self->timer) {
        self->state     = Gachapandora_StateDebris_Falling;
        self->stateDraw = Gachapandora_Draw_BossDebris;
    }
}

void Gachapandora_StateDebris_Falling(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Gachapandora_StateDebris_BossDebris(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x2800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        self->velocity.y = -0x20000;
        self->velocity.x = RSDK.Rand(-2, 3) << 16;
        self->state      = Gachapandora_StateDebris_Falling;
        self->stateDraw  = Gachapandora_Draw_BossDebris;
    }
}

void Gachapandora_Draw_BossDebris(void)
{
    RSDK_THIS(Gachapandora);

    if (Zone->timer & 1)
        RSDK.DrawSprite(&self->mainAnimator, NULL, false);
}

void Gachapandora_StateSpark_Attatched(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    EntityGachapandora *parent = self->parent;
    if (parent) {
        self->position.x = parent->position.x + self->originPos.x;
        self->position.y = parent->position.y + self->originPos.y;
    }

    if (self->mainAnimator.animationID >= 14 && (self->mainAnimator.animationID != 16 || self->mainAnimator.frameID < 2)) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxSpark)) {
                Player_ElementHurt(player, self, SHIELD_LIGHTNING);
            }
        }
    }

    if (self->mainAnimator.frameID == self->mainAnimator.frameCount - 1)
        destroyEntity(self);
}

void Gachapandora_StateSpark_Detatched(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;

    if (self->velocity.y < 0x20000)
        self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Gachapandora->hitboxSpark)) {
            Player_ElementHurt(player, self, SHIELD_LIGHTNING);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x40000, true)) {
        RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 16, &self->mainAnimator, true, 0);
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = Gachapandora_StateSpark_Attatched;
    }
}

void Gachapandora_StateEggman_Falling(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (self->position.y >= self->parent->position.y + 0xE0000) {
        RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 5, &self->mainAnimator, true, 0);
        self->state = Gachapandora_StateEggman_Escape;
    }
}

void Gachapandora_StateEggman_Escape(void)
{
    RSDK_THIS(Gachapandora);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x180000, true)) {
        self->velocity.x = 0;
        self->velocity.y = 0;
        self->state      = Gachapandora_StateEggman_RunAway;
    }
}

void Gachapandora_StateEggman_RunAway(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->mainAnimator.frameID >= 4) {
        if (self->velocity.x < 0x60000)
            self->velocity.x += 0xC00;

        self->position.x += self->velocity.x;
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Gachapandora_EditorDraw(void)
{
    RSDK_THIS(Gachapandora);

    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 0, &self->mainAnimator, false, 0);
    RSDK.SetSpriteAnimation(Gachapandora->eggmanFrames, 0, &self->eggmanAnimator, false, 0);
    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 1, &self->handleAnimator, false, 7);
    RSDK.SetSpriteAnimation(Gachapandora->aniFrames, 2, &self->capsuleAnimator, false, 7);

    Vector2 drawPos            = self->position;
    self->mainAnimator.frameID = 4;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->mainAnimator.frameID = 5;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->mainAnimator.frameID = 2;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->mainAnimator.frameID = 3;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->mainAnimator.frameID = 0;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    int32 storeDir  = self->direction;
    self->direction = Gachapandora->eggmanDir;
    drawPos.x       = self->position.x;
    drawPos.y       = self->position.y - 0x100000;
    RSDK.DrawSprite(&self->eggmanAnimator, &drawPos, false);

    self->direction = storeDir;

    self->mainAnimator.frameID = 1;
    RSDK.DrawSprite(&self->mainAnimator, NULL, false);

    self->direction = Gachapandora->handleDir;
    drawPos.y += 0x1A0000;
    RSDK.DrawSprite(&self->handleAnimator, &drawPos, false);

    self->direction = storeDir;

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawArenaBounds(-WIDE_SCR_XCENTER, -SCREEN_YSIZE, WIDE_SCR_XCENTER, 128, 1 | 0 | 4 | 8, 0x00C0F0);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Gachapandora_EditorLoad(void)
{
    Gachapandora->aniFrames    = RSDK.LoadSpriteAnimation("MMZ/Gachapandora.bin", SCOPE_STAGE);
    Gachapandora->eggmanFrames = RSDK.LoadSpriteAnimation("Eggman/EggmanMMZ2.bin", SCOPE_STAGE);
}
#endif

void Gachapandora_Serialize(void) {}
