// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Tubinaut Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTubinaut *Tubinaut;

void Tubinaut_Update(void)
{
    RSDK_THIS(Tubinaut);

    StateMachine_Run(self->state);
}

void Tubinaut_LateUpdate(void) {}

void Tubinaut_StaticUpdate(void) {}

void Tubinaut_Draw(void)
{
    RSDK_THIS(Tubinaut);

    if (self->attackTimer < 256 || self->orbCount <= 1) {
        for (int32 i = 0; i < TUBINAUT_ORB_COUNT; ++i) {
            if (self->ballsVisible[i]) {
                RSDK.DrawSprite(&self->ballAnimators[i], &self->orbPositions[i], false);
            }
        }
    }
    else {
        RSDK.DrawSprite(&self->fieldAnimator, NULL, false);
    }

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->bodyAnimator, NULL, false);
    self->drawFX = FX_NONE;
}

void Tubinaut_Create(void *data)
{
    RSDK_THIS(Tubinaut);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0];

    if (data) {
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, VOID_TO_INT(data), &self->bodyAnimator, true, 0);
        self->state = Tubinaut_Orb_BodyDeath;
    }
    else {
        self->startPos      = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 1, &self->ballAnimators[0], true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 2, &self->ballAnimators[1], true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 3, &self->ballAnimators[2], true, 0);

        int32 angle = 0x000;
        for (int32 o = 0; o < TUBINAUT_ORB_COUNT; ++o) {
            self->orbAngles[o]    = angle;
            self->ballsVisible[o] = true;

            angle += 0x500;
        }

        self->activeOrbs  = 0b111;
        self->orbCount    = 3;
        self->attackTimer = 64;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &self->bodyAnimator, true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 7, &self->fieldAnimator, true, 0);

        self->state    = Tubinaut_State_Init;
        self->orbState = Tubinaut_Orb_Relax;
    }
}

void Tubinaut_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Tubinaut.bin", SCOPE_STAGE);
    else
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Tubinaut.bin", SCOPE_STAGE);

    Tubinaut->hitboxFace.left   = -10;
    Tubinaut->hitboxFace.top    = -10;
    Tubinaut->hitboxFace.right  = 10;
    Tubinaut->hitboxFace.bottom = 10;

    Tubinaut->hitboxOrb.left   = -7;
    Tubinaut->hitboxOrb.top    = -7;
    Tubinaut->hitboxOrb.right  = 7;
    Tubinaut->hitboxOrb.bottom = 7;

    Tubinaut->hitboxSpin.left   = -20;
    Tubinaut->hitboxSpin.top    = -20;
    Tubinaut->hitboxSpin.right  = 20;
    Tubinaut->hitboxSpin.bottom = 20;

    DEBUGMODE_ADD_OBJ(Tubinaut);

    Tubinaut->sfxPowerdown = RSDK.GetSfx("Stage/PowerDown.wav");
    Tubinaut->sfxRepel     = RSDK.GetSfx("Stage/Repel.wav");
}

void Tubinaut_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityTubinaut *tubinaut = CREATE_ENTITY(Tubinaut, NULL, self->position.x, self->position.y);
    tubinaut->distanceRemain = 256;
    tubinaut->distance       = 256;
}

void Tubinaut_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Tubinaut_CheckPlayerCollisions(void)
{
    RSDK_THIS(Tubinaut);

    int32 storeX   = self->position.x;
    int32 storeY   = self->position.y;
    int32 playerID = 0;

    foreach_active(Player, player)
    {
        if (self->playerTimers[playerID]) {
            --self->playerTimers[playerID];
        }
        else if (self->orbState == Tubinaut_Orb_Attack && self->orbCount > 1) {
            if (Player_CheckCollisionTouch(player, self, &Tubinaut->hitboxSpin)
                && (self->orbCount == 2 || (self->orbCount == 3 && !Player_Hurt(player, self)))) {
                Tubinaut_HandleRepel(player, playerID);
            }
        }
        else {
            bool32 repelled = false;
            for (int32 i = 0; i < TUBINAUT_ORB_COUNT; ++i) {
                if (self->ballsVisible[i]) {
                    self->position.x = self->orbPositions[i].x;
                    self->position.y = self->orbPositions[i].y;

                    if (Player_CheckCollisionTouch(player, self, &Tubinaut->hitboxOrb)) {
                        Tubinaut_OrbHit(player, i);
#if MANIA_USE_PLUS
                        if (player->state != Player_State_MightyHammerDrop) {
#endif
                            repelled                     = true;
                            self->playerTimers[playerID] = 15;
#if MANIA_USE_PLUS
                        }
#endif

                        if (self->orbCount) {
                            if (self->orbCount == 1) {
                                self->bodyAnimator.frameID = 2;
                            }
                            else if (self->orbCount == 2) {
                                switch (self->activeOrbs) {
                                    case 0b011: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 4, &self->fieldAnimator, true, 0); break;
                                    case 0b101: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 6, &self->fieldAnimator, true, 0); break;
                                    case 0b110: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 5, &self->fieldAnimator, true, 0); break;
                                }

                                self->bodyAnimator.frameID = 0;
                            }
                        }
                        else {
                            self->bodyAnimator.frameID = 3;
                        }
                    }
                }
            }

            self->position.x = storeX;
            self->position.y = storeY;
            if (!repelled && Player_CheckBadnikTouch(player, self, &Tubinaut->hitboxFace) && Player_CheckBadnikBreak(player, self, false)) {
                for (int32 i = 0; i < TUBINAUT_ORB_COUNT; ++i) {
                    if (self->ballsVisible[i]) {
                        EntityTubinaut *orb = CREATE_ENTITY(Tubinaut, INT_TO_VOID(i + 1), self->orbPositions[i].x, self->orbPositions[i].y);
                        orb->velocity.x     = 0x380 * RSDK.Cos256(self->orbAngles[i] >> 4);
                        orb->velocity.y     = 0x380 * RSDK.Sin256(self->orbAngles[i] >> 4);
                    }
                }

                destroyEntity(self);
                self->active = ACTIVE_DISABLED;
            }
        }

        ++playerID;
    }
}

bool32 Tubinaut_CheckAttacking(EntityPlayer *player)
{
    RSDK_THIS(Tubinaut);

    bool32 isAttacking = Player_CheckAttacking(player, self);

#if MANIA_USE_PLUS
    if (!isAttacking && player->characterID == ID_MIGHTY && player->animator.animationID == ANI_CROUCH) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            player->uncurlTimer = 30;
            player->groundVel   = self->position.x > player->position.x ? -0x10000 : 0x10000;
            isAttacking         = true;
        }
    }
#endif

    if (!isAttacking)
        Player_Hurt(player, self);

    return isAttacking;
}

void Tubinaut_OrbHit(EntityPlayer *player, int32 orbID)
{
    RSDK_THIS(Tubinaut);

    if (Tubinaut_CheckAttacking(player)) {
        EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
        dust->state      = Dust_State_DustPuff;
        dust->drawGroup  = player->drawGroup;
        RSDK.PlaySfx(Tubinaut->sfxPowerdown, false, 255);
#if MANIA_USE_PLUS
        if (player->characterID != ID_MIGHTY || player->animator.animationID != ANI_HAMMERDROP) {
#endif
            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

            int32 velX = 0, velY = 0;
#if MANIA_USE_PLUS
            if (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_CROUCH) {
                velX = player->velocity.x;
                velY = player->velocity.y - 0x10000;
            }
            else {
#endif
                velX = 0x540 * RSDK.Cos256(angle);
                velY = 0x540 * RSDK.Sin256(angle);
#if MANIA_USE_PLUS
            }
#endif

            if (player->characterID != ID_KNUCKLES || player->animator.animationID != ANI_GLIDE) {
                player->velocity.x   = velX;
                player->groundVel    = velX;
                player->applyJumpCap = false;
            }

            player->velocity.y     = velY;
            player->onGround       = false;
            player->tileCollisions = TILECOLLISION_DOWN;
#if MANIA_USE_PLUS
        }
#endif

        self->ballsVisible[orbID] = false;
        --self->orbCount;
        self->activeOrbs &= ~(1 << orbID);

        if (self->orbCount == 2) {
            switch (orbID) {
                case 0:
                    self->reAdjustTimers[1] = -24;
                    self->reAdjustTimers[2] = 24;
                    break;

                case 1:
                    self->reAdjustTimers[2] = -24;
                    self->reAdjustTimers[0] = 24;
                    break;

                case 2:
                    self->reAdjustTimers[0] = -24;
                    self->reAdjustTimers[1] = 24;
                    break;
            }
        }
    }
}

void Tubinaut_HandleRepel(EntityPlayer *player, int32 playerID)
{
    RSDK_THIS(Tubinaut);

#if MANIA_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->animator.animationID == ANI_CROUCH) {
        self->playerTimers[playerID] = 15;
        RSDK.PlaySfx(Tubinaut->sfxRepel, false, 0xFF);

        player->groundVel = self->position.x > player->position.x ? -0x20000 : 0x20000;
    }
    else {
#endif
        if (Tubinaut_CheckAttacking(player)) {
            int32 angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
            int32 velX  = 0x700 * RSDK.Cos256(angle);
            int32 velY  = 0x700 * RSDK.Sin256(angle);

            if (player->characterID != ID_KNUCKLES || player->animator.animationID != ANI_GLIDE) {
                player->velocity.x   = velX;
                player->groundVel    = velX;
                player->applyJumpCap = false;
            }

            player->velocity.y           = velY;
            player->onGround             = false;
            player->tileCollisions       = TILECOLLISION_DOWN;
            self->playerTimers[playerID] = 15;
            RSDK.PlaySfx(Tubinaut->sfxRepel, false, 0xFF);
        }
#if MANIA_USE_PLUS
    }
#endif
}

void Tubinaut_HandleOrbs(void)
{
    RSDK_THIS(Tubinaut);

    self->position.y = (RSDK.Sin256(self->angle) << 10) + self->startPos.y;
    self->angle      = (self->angle + 4) & 0xFF;

    for (int32 i = 0; i < TUBINAUT_ORB_COUNT; ++i) {
        self->orbPositions[i].x = 0x1400 * RSDK.Cos256(self->orbAngles[i] >> 4) + self->position.x;
        self->orbPositions[i].y = 0x1400 * RSDK.Sin256(self->orbAngles[i] >> 4) + self->position.y;
        self->orbAngles[i] -= self->attackTimer;

        if (self->reAdjustTimers[i]) {
            if (self->reAdjustTimers[i] <= 0) {
                self->reAdjustTimers[i]++;
                self->orbAngles[i] -= 16;
            }
            else {
                self->reAdjustTimers[i]--;
                self->orbAngles[i] += 16;
            }
        }
        self->orbAngles[i] &= 0xFFF;

        if (self->orbState == Tubinaut_Orb_Cooldown || self->orbCount <= 1 || self->attackTimer < 192) {
            self->ballAnimators[i].frameID = (2 * (7 - ((((self->orbAngles[i] >> 4) + 112) >> 5) & 7)));
            self->ballAnimators[i].timer   = (self->ballAnimators[i].timer + 1) & 3;
            self->ballAnimators[i].frameID += (self->ballAnimators[i].timer >> 1);
        }
        else {
            int32 id                     = ((self->ballAnimators[i].timer + 1) & 7) >> 1;
            self->ballAnimators[i].timer = (self->ballAnimators[i].timer + 1) & 7;

            switch (id) {
                case 0:
                case 2: self->ballAnimators[i].frameID = (id + 2 * (7 - ((((self->orbAngles[i] >> 4) + 112) >> 5) & 7))); break;

                case 1: self->ballAnimators[i].frameID = 17; break;
                case 3: self->ballAnimators[i].frameID = 16; break;

                default: break;
            }
        }
    }
}

void Tubinaut_CheckOffScreen(void)
{
    RSDK_THIS(Tubinaut);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position = self->startPos;
        self->visible  = false;
        self->state    = Tubinaut_State_Init;
        self->active   = ACTIVE_BOUNDS;

        if (self->orbCount > 1) {
            self->attackTimer = 64;
            self->orbState    = Tubinaut_Orb_Relax;
        }
    }
}

void Tubinaut_State_Init(void)
{
    RSDK_THIS(Tubinaut);

    self->timer          = 0;
    self->distanceRemain = self->distance;
    self->direction      = FLIP_NONE;
    self->velocity.x     = -0x8000;
    self->visible        = true;
    self->active         = ACTIVE_NORMAL;

    self->state = Tubinaut_State_Move;
    Tubinaut_State_Move();
}

void Tubinaut_State_Move(void)
{
    RSDK_THIS(Tubinaut);

    if (self->distance) {
        self->position.x += self->velocity.x;

        if (!--self->distanceRemain) {
            self->distanceRemain = self->distance;
            self->direction ^= FLIP_X;
            self->velocity.x = -self->velocity.x;
        }
    }
    else {
        EntityPlayer *playerPtr = NULL;
        foreach_active(Player, player)
        {
            if (playerPtr) {
                if (abs(player->position.x - self->position.x) < abs(playerPtr->position.x - self->position.x))
                    playerPtr = player;
            }
            else {
                playerPtr = player;
            }
        }

        if (playerPtr)
            self->direction = playerPtr->position.x >= self->position.x;
    }

    Tubinaut_HandleOrbs();

    if (!self->orbCount)
        RSDK.ProcessAnimation(&self->bodyAnimator);

    RSDK.ProcessAnimation(&self->fieldAnimator);

#if RETRO_USE_MOD_LOADER
    StateMachine_Run(self->orbState);
#else
    // Explitly called, no StateMachine call for some reason
    self->orbState();
#endif

    Tubinaut_CheckPlayerCollisions();
    Tubinaut_CheckOffScreen();
}

void Tubinaut_Orb_Relax(void)
{
    RSDK_THIS(Tubinaut);

    if (++self->timer > 120) {
        self->orbState = Tubinaut_Orb_PrepareAttack;
        if (self->orbCount > 2)
            self->bodyAnimator.frameID = 1;
        self->timer = 0;
    }
}

void Tubinaut_Orb_PrepareAttack(void)
{
    RSDK_THIS(Tubinaut);

    if (self->attackTimer >= 256) {
        if (self->orbCount > 1)
            self->orbState = Tubinaut_Orb_Attack;
    }
    else {
        self->attackTimer++;
    }
}

void Tubinaut_Orb_Attack(void)
{
    RSDK_THIS(Tubinaut);

    if (++self->timer > 120) {
        self->orbState = Tubinaut_Orb_Cooldown;
        switch (self->orbCount) {
            case 0: self->bodyAnimator.frameID = 3; break;
            case 1: self->bodyAnimator.frameID = 2; break;

            case 2:
            case 3: self->bodyAnimator.frameID = 0; break;

            default: break;
        }

        self->timer = 0;
    }
}

void Tubinaut_Orb_Cooldown(void)
{
    RSDK_THIS(Tubinaut);

    if (self->orbCount > 1) {
        if (--self->attackTimer <= 64)
            self->orbState = Tubinaut_Orb_Relax;
    }
    else {
        self->orbState = Tubinaut_Orb_PrepareAttack;
    }
}

void Tubinaut_Orb_BodyDeath(void)
{
    RSDK_THIS(Tubinaut);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Tubinaut_EditorDraw(void)
{
    RSDK_THIS(Tubinaut);

    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 1, &self->ballAnimators[0], true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 2, &self->ballAnimators[1], true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 3, &self->ballAnimators[2], true, 0);

    int32 angle = 0x000;
    for (int32 o = 0; o < TUBINAUT_ORB_COUNT; ++o) {
        self->orbAngles[o]    = angle;
        self->ballsVisible[o] = true;

        angle += 0x500;
    }

    self->activeOrbs  = 0b111;
    self->orbCount    = 3;
    self->attackTimer = 64;
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &self->bodyAnimator, true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 7, &self->fieldAnimator, true, 0);
    self->state    = Tubinaut_State_Init;
    self->orbState = Tubinaut_Orb_Relax;

    for (int32 i = 0; i < TUBINAUT_ORB_COUNT; ++i) {
        self->orbPositions[i].x = 0x1400 * RSDK.Cos256(self->orbAngles[i] >> 4) + self->position.x;
        self->orbPositions[i].y = 0x1400 * RSDK.Sin256(self->orbAngles[i] >> 4) + self->position.y;
    }

    Tubinaut_Draw();

    if (showGizmos()) {
        int32 x = self->position.x;

        self->position.x += self->distance * -0x8000;

        for (int32 i = 0; i < TUBINAUT_ORB_COUNT; ++i) {
            self->orbPositions[i].x = self->position.x + 0x1400 * RSDK.Cos256(self->orbAngles[i] >> 4);
            self->orbPositions[i].y = self->position.y + 0x1400 * RSDK.Sin256(self->orbAngles[i] >> 4);
        }

        self->inkEffect = INK_BLEND;
        Tubinaut_Draw();
        self->inkEffect = INK_NONE;

        RSDK.DrawLine(x, self->position.y, self->position.x, self->position.y, 0x00FF00, 0xFF, INK_NONE, false);

        self->position.x = x;
    }
}

void Tubinaut_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Tubinaut.bin", SCOPE_STAGE);
    else
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Tubinaut.bin", SCOPE_STAGE);
}
#endif

void Tubinaut_Serialize(void) { RSDK_EDITABLE_VAR(Tubinaut, VAR_UINT16, distance); }
