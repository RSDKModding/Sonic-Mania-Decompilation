#include "SonicMania.h"

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

    if (self->timer2 < 256 || self->orbCount <= 1) {
        for (int i = 0; i < Tubinaut_OrbCount; ++i) {
            if (self->field_C4[i]) {
                RSDK.DrawSprite(&self->animators[i], &self->orbPositions[i], false);
            }
        }
    }
    else {
        RSDK.DrawSprite(&self->animator2, NULL, false);
    }

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->animator1, NULL, false);
    self->drawFX = FX_NONE;
}

void Tubinaut_Create(void *data)
{
    RSDK_THIS(Tubinaut);

    self->visible   = true;
    self->drawOrder = Zone->drawOrderLow;
    if (data) {
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, voidToInt(data), &self->animator1, true, 0);
        self->state = Tubinaut_State2_Unknown;
    }
    else {
        self->startPos      = self->position;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 1, self->animators, true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 2, &self->animators[1], true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 3, &self->animators[2], true, 0);
        self->orbAngles[0] = 0;
        self->orbAngles[1] = 0x500;
        self->orbAngles[2] = 0xA00;
        self->field_C4[0]  = true;
        self->field_C4[1]  = true;
        self->field_C4[2]  = true;
        self->activeOrbs   = 1 | 2 | 4;
        self->orbCount     = 3;
        self->timer2       = 64;
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 7, &self->animator2, true, 0);
        self->state    = Tubinaut_State_Setup;
        self->stateOrb = Tubinaut_StateOrb_Unknown1;
    }
}

void Tubinaut_StageLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Tubinaut.bin", SCOPE_STAGE);
    else
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Tubinaut.bin", SCOPE_STAGE);
    Tubinaut->hitbox1.left   = -10;
    Tubinaut->hitbox1.top    = -10;
    Tubinaut->hitbox1.right  = 10;
    Tubinaut->hitbox1.bottom = 10;
    Tubinaut->hitbox2.left   = -7;
    Tubinaut->hitbox2.top    = -7;
    Tubinaut->hitbox2.right  = 7;
    Tubinaut->hitbox2.bottom = 7;
    Tubinaut->hitbox3.left   = -20;
    Tubinaut->hitbox3.top    = -20;
    Tubinaut->hitbox3.right  = 20;
    Tubinaut->hitbox3.bottom = 20;
    DEBUGMODE_ADD_OBJ(Tubinaut);
    Tubinaut->sfxPowerdown = RSDK.GetSFX("Stage/PowerDown.wav");
    Tubinaut->sfxRepel     = RSDK.GetSFX("Stage/Repel.wav");
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
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Tubinaut_CheckPlayerCollisions(void)
{
    RSDK_THIS(Tubinaut);

    int storeX   = self->position.x;
    int storeY   = self->position.y;
    int playerID = 0;

    foreach_active(Player, player)
    {
        if (self->playerTimers[playerID]) {
            --self->playerTimers[playerID];
        }
        else if (self->stateOrb == Tubinaut_StateOrb_Unknown3 && self->orbCount > 1) {
            if (Player_CheckCollisionTouch(player, self, &Tubinaut->hitbox3)
                && (self->orbCount == 2 || self->orbCount == 3 && !Player_CheckHit(player, self))) {
                Tubinaut_HandleRepel(player, playerID);
            }
        }
        else {
            bool32 flag = false;
            for (int i = 0; i < Tubinaut_OrbCount; ++i) {
                if (self->field_C4[i]) {
                    self->position.x = self->orbPositions[i].x;
                    self->position.y = self->orbPositions[i].y;

                    if (Player_CheckCollisionTouch(player, self, &Tubinaut->hitbox2)) {
                        Tubinaut_Unknown5(player, i);
#if RETRO_USE_PLUS
                        if (player->state != Player_State_MightyHammerDrop) {
#endif
                            flag                           = true;
                            self->playerTimers[playerID] = 15;
#if RETRO_USE_PLUS
                        }
#endif

                        if (self->orbCount) {
                            if (self->orbCount == 1) {
                                self->animator1.frameID = 2;
                            }
                            else if (self->orbCount == 2) {
                                switch (self->activeOrbs) {
                                    case 3: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 4, &self->animator2, true, 0); break;
                                    case 5: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 6, &self->animator2, true, 0); break;
                                    case 6: RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 5, &self->animator2, true, 0); break;
                                }
                                self->animator1.frameID = 0;
                            }
                        }
                        else {
                            self->animator1.frameID = 3;
                        }
                    }
                }
            }

            self->position.x = storeX;
            self->position.y = storeY;
            if (!flag && Player_CheckBadnikTouch(player, self, &Tubinaut->hitbox1) && Player_CheckBadnikBreak(self, player, false)) {
                for (int i = 0; i < Tubinaut_OrbCount; ++i) {
                    if (self->field_C4[i]) {
                        EntityTubinaut *orb = CREATE_ENTITY(Tubinaut, intToVoid(i + 1), self->orbPositions[i].x, self->orbPositions[i].y);
                        orb->velocity.x     = 0x380 * RSDK.Cos256(self->orbAngles[i] >> 4);
                        orb->velocity.y     = 0x380 * RSDK.Sin256(self->orbAngles[i] >> 4);
                    }
                }

                destroyEntity(self);
                self->active = ACTIVE_NEVER2;
            }
        }
        ++playerID;
    }
}

bool32 Tubinaut_CheckAttacking(EntityPlayer *player)
{
    RSDK_THIS(Tubinaut);

    bool32 flag = Player_CheckAttacking(player, self);

#if RETRO_USE_PLUS
    if (!flag && player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
        if (!player->uncurlTimer) {
            RSDK.PlaySfx(Player->sfxPimPom, false, 255);
            player->uncurlTimer = 30;
            if (self->position.x > player->position.x)
                player->groundVel = -0x10000;
            else
                player->groundVel = 0x10000;
            flag = true;
        }
    }
#endif

    if (!flag)
        Player_CheckHit(player, self);

    return flag;
}

void Tubinaut_Unknown5(EntityPlayer *player, int orbID)
{
    RSDK_THIS(Tubinaut);

    if (Tubinaut_CheckAttacking(player)) {
        EntityDust *dust = CREATE_ENTITY(Dust, self, self->position.x, self->position.y);
        dust->state      = Dust_State_DropDash;
        dust->drawOrder  = player->drawOrder;
        RSDK.PlaySfx(Tubinaut->sfxPowerdown, false, 255);
#if RETRO_USE_PLUS
        if (player->characterID != ID_MIGHTY || player->playerAnimator.animationID != ANI_DROPDASH) {
#endif
            int angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);

            int velX = 0, velY = 0;
#if RETRO_USE_PLUS
            if (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
                velX = player->velocity.x;
                velY = player->velocity.y - 0x10000;
            }
            else {
#endif
                velX = 0x540 * RSDK.Cos256(angle);
                velY = 0x540 * RSDK.Sin256(angle);
#if RETRO_USE_PLUS
            }
#endif

            if (player->characterID != ID_KNUCKLES || player->playerAnimator.animationID != ANI_FLY) {
                player->velocity.x  = velX;
                player->groundVel   = velX;
                player->jumpAbility = 0;
            }
            player->velocity.y     = velY;
            player->onGround       = false;
            player->tileCollisions = true;
#if RETRO_USE_PLUS
        }
#endif

        self->field_C4[orbID] = 0;
        --self->orbCount;
        self->activeOrbs &= ~(1 << orbID);

        if (self->orbCount == 2) {
            switch (orbID) {
                case 0:
                    self->field_BC[1] = -24;
                    self->field_BC[2] = 24;
                    break;
                case 1:
                    self->field_BC[2] = -24;
                    self->field_BC[0] = 24;
                    break;
                case 2:
                    self->field_BC[0] = -24;
                    self->field_BC[1] = 24;
                    break;
            }
        }
    }
}

void Tubinaut_HandleRepel(EntityPlayer *player, int playerID)
{
    RSDK_THIS(Tubinaut);

#if RETRO_USE_PLUS
    if (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_CROUCH) {
        self->playerTimers[playerID] = 15;
        RSDK.PlaySfx(Tubinaut->sfxRepel, false, 255);

        if (self->position.x > player->position.x)
            player->groundVel = -0x20000;
        else
            player->groundVel = 0x20000;
    }
    else {
#endif
        if (Tubinaut_CheckAttacking(player)) {
            int angle = RSDK.ATan2(player->position.x - self->position.x, player->position.y - self->position.y);
            int velX  = 0x700 * RSDK.Cos256(angle);
            int velY  = 0x700 * RSDK.Sin256(angle);
            if (player->characterID != ID_KNUCKLES || player->playerAnimator.animationID != ANI_FLY) {
                player->velocity.x  = velX;
                player->groundVel   = velX;
                player->jumpAbility = 0;
            }
            player->velocity.y             = velY;
            player->onGround               = false;
            player->tileCollisions         = true;
            self->playerTimers[playerID] = 15;
            RSDK.PlaySfx(Tubinaut->sfxRepel, false, 255);
        }
#if RETRO_USE_PLUS
    }
#endif
}

void Tubinaut_HandleOrbs(void)
{
    RSDK_THIS(Tubinaut);

    self->position.y = (RSDK.Sin256(self->angle) << 10) + self->startPos.y;
    self->angle      = (self->angle + 4) & 0xFF;

    for (int i = 0; i < Tubinaut_OrbCount; ++i) {
        self->orbPositions[i].x = 0x1400 * RSDK.Cos256(self->orbAngles[i] >> 4) + self->position.x;
        self->orbPositions[i].y = 0x1400 * RSDK.Sin256(self->orbAngles[i] >> 4) + self->position.y;
        self->orbAngles[i] -= self->timer2;

        if (self->field_BC[i]) {
            if (self->field_BC[i] <= 0) {
                self->field_BC[i]++;
                self->orbAngles[i] -= 16;
            }
            else {
                self->field_BC[i]--;
                self->orbAngles[i] += 16;
            }
        }
        self->orbAngles[i] &= 0xFFF;

        if (self->stateOrb == Tubinaut_StateOrb_Unknown4 || self->orbCount <= 1 || self->timer2 < 192) {
            self->animators[i].frameID        = (2 * (7 - ((((self->orbAngles[i] >> 4) + 112) >> 5) & 7)));
            self->animators[i].animationTimer = (self->animators[i].animationTimer + 1) & 3;
            self->animators[i].frameID += (self->animators[i].animationTimer >> 1);
        }
        else {
            int val                             = ((self->animators[i].animationTimer + 1) & 7) >> 1;
            self->animators[i].animationTimer = (self->animators[i].animationTimer + 1) & 7;
            switch (val) {
                case 0:
                case 2: self->animators[i].frameID = (val + 2 * (7 - ((((self->orbAngles[i] >> 4) + 112) >> 5) & 7))); break;
                case 1: self->animators[i].frameID = 17; break;
                case 3: self->animators[i].frameID = 16; break;
                default: break;
            }
        }
    }
}

void Tubinaut_CheckOnScreen(void)
{
    RSDK_THIS(Tubinaut);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->visible    = 0;
        self->state      = Tubinaut_State_Setup;
        self->active     = ACTIVE_BOUNDS;
        if (self->orbCount > 1) {
            self->timer2   = 64;
            self->stateOrb = Tubinaut_StateOrb_Unknown1;
        }
    }
}

void Tubinaut_State_Setup(void)
{
    RSDK_THIS(Tubinaut);

    self->timer          = 0;
    self->distanceRemain = self->distance;
    self->direction      = FLIP_NONE;
    self->velocity.x     = -0x8000;
    self->visible        = true;
    self->active         = ACTIVE_NORMAL;
    self->state          = Tubinaut_State_Move;
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
        RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
    self->stateOrb();
    Tubinaut_CheckPlayerCollisions();
    Tubinaut_CheckOnScreen();
}

void Tubinaut_StateOrb_Unknown1(void)
{
    RSDK_THIS(Tubinaut);

    if (++self->timer > 120) {
        self->stateOrb = Tubinaut_StateOrb_Unknown2;
        if (self->orbCount > 2)
            self->animator1.frameID = 1;
        self->timer = 0;
    }
}

void Tubinaut_StateOrb_Unknown2(void)
{
    RSDK_THIS(Tubinaut);

    if (self->timer2 >= 256) {
        if (self->orbCount > 1)
            self->stateOrb = Tubinaut_StateOrb_Unknown3;
    }
    else {
        self->timer2++;
    }
}

void Tubinaut_StateOrb_Unknown3(void)
{
    RSDK_THIS(Tubinaut);

    if (++self->timer > 120) {
        self->stateOrb = Tubinaut_StateOrb_Unknown4;
        switch (self->orbCount) {
            case 0: self->animator1.frameID = 3; break;
            case 1: self->animator1.frameID = 2; break;
            case 2:
            case 3: self->animator1.frameID = 0; break;
            default: break;
        }
        self->timer = 0;
    }
}

void Tubinaut_StateOrb_Unknown4(void)
{
    RSDK_THIS(Tubinaut);

    if (self->orbCount > 1) {
        if (--self->timer2 <= 64)
            self->stateOrb = Tubinaut_StateOrb_Unknown1;
    }
    else {
        self->stateOrb = Tubinaut_StateOrb_Unknown2;
    }
}

void Tubinaut_State2_Unknown(void)
{
    RSDK_THIS(Tubinaut);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void Tubinaut_EditorDraw(void)
{
    RSDK_THIS(Tubinaut);

    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 1, self->animators, true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 2, &self->animators[1], true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 3, &self->animators[2], true, 0);
    self->orbAngles[0] = 0;
    self->orbAngles[1] = 0x500;
    self->orbAngles[2] = 0xA00;
    self->field_C4[0]  = true;
    self->field_C4[1]  = true;
    self->field_C4[2]  = true;
    self->activeOrbs   = 1 | 2 | 4;
    self->orbCount     = 3;
    self->timer2       = 64;
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(Tubinaut->aniFrames, 7, &self->animator2, true, 0);
    self->state    = Tubinaut_State_Setup;
    self->stateOrb = Tubinaut_StateOrb_Unknown1;

    for (int i = 0; i < Tubinaut_OrbCount; ++i) {
        self->orbPositions[i].x = 0x1400 * RSDK.Cos256(self->orbAngles[i] >> 4) + self->position.x;
        self->orbPositions[i].y = 0x1400 * RSDK.Sin256(self->orbAngles[i] >> 4) + self->position.y;
    }

    Tubinaut_Draw();

    if (showGizmos()) {
        int x = self->position.x;

        self->position.x += self->distance * -0x8000;

        for (int i = 0; i < Tubinaut_OrbCount; ++i) {
            self->orbPositions[i].x = 0x1400 * RSDK.Cos256(self->orbAngles[i] >> 4) + self->position.x;
            self->orbPositions[i].y = 0x1400 * RSDK.Sin256(self->orbAngles[i] >> 4) + self->position.y;
        }

        self->inkEffect = INK_BLEND;
        Tubinaut_Draw();

        RSDK.DrawLine(x, self->position.y, self->position.x, self->position.y, 0x00FF00, 0xFF, INK_NONE, false);

        self->position.x = x;
    }
}

void Tubinaut_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("SPZ1"))
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Tubinaut.bin", SCOPE_STAGE);
    else
        Tubinaut->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Tubinaut.bin", SCOPE_STAGE);
}
#endif

void Tubinaut_Serialize(void) { RSDK_EDITABLE_VAR(Tubinaut, VAR_UINT16, distance); }
