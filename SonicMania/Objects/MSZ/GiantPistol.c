// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GiantPistol Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGiantPistol *GiantPistol;

void GiantPistol_Update(void)
{
    RSDK_THIS(GiantPistol);

    StateMachine_Run(self->state);
}

void GiantPistol_LateUpdate(void) {}

void GiantPistol_StaticUpdate(void) {}

void GiantPistol_Draw(void)
{
    RSDK_THIS(GiantPistol);

    if (SceneInfo->currentDrawGroup == Zone->objectDrawGroup[0]) {
        RSDK.DrawSprite(&self->pistolEntryAnimator, NULL, false);
        RSDK.DrawSprite(&self->handLowAnimator, NULL, false);

        RSDK.AddDrawListRef(Zone->objectDrawGroup[1], SceneInfo->entitySlot);
    }
    else {
        RSDK.DrawSprite(&self->triggerAnimator, NULL, false);
        RSDK.DrawSprite(&self->hammerAnimator, NULL, false);
        RSDK.DrawSprite(&self->chamberAnimator, NULL, false);
        RSDK.DrawSprite(&self->pistolAnimator, NULL, false);
        RSDK.DrawSprite(&self->handHighAnimator, NULL, false);
        RSDK.DrawSprite(&self->triggerFingerAnimator, NULL, false);
    }
}

void GiantPistol_Create(void *data)
{
    RSDK_THIS(GiantPistol);

    self->drawFX = FX_FLIP;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->pistolAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->pistolEntryAnimator, true, 1);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &self->chamberAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 6, &self->hammerAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 4, &self->triggerAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &self->handLowAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &self->handHighAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 3, &self->triggerFingerAnimator, true, 0);

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->startPos      = self->position;

        self->pivot.x = self->position.x + (self->direction == FLIP_NONE ? 0x300000 : -0x300000);
        self->pivot.y = self->position.y + 0x180000;
        self->state   = GiantPistol_State_AwaitPlayerEntry;
    }
}

void GiantPistol_StageLoad(void)
{
    GiantPistol->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pistol.bin", SCOPE_STAGE);

    GiantPistol->hitboxFront.left   = -80;
    GiantPistol->hitboxFront.top    = -58;
    GiantPistol->hitboxFront.right  = 4;
    GiantPistol->hitboxFront.bottom = -16;

    GiantPistol->hitboxEntry.left   = 4;
    GiantPistol->hitboxEntry.top    = -26;
    GiantPistol->hitboxEntry.right  = 44;
    GiantPistol->hitboxEntry.bottom = -16;

    GiantPistol->hitboxRear.left   = 44;
    GiantPistol->hitboxRear.top    = -58;
    GiantPistol->hitboxRear.right  = 64;
    GiantPistol->hitboxRear.bottom = -16;

    GiantPistol->sfxClack      = RSDK.GetSfx("Stage/Clack.wav");
    GiantPistol->sfxCannonFire = RSDK.GetSfx("Stage/CannonFire.wav");
}

void GiantPistol_State_AwaitPlayerEntry(void)
{
    RSDK_THIS(GiantPistol);

    foreach_active(Player, player)
    {
        int32 side = Player_CheckCollisionBox(player, self, &GiantPistol->hitboxEntry);

        if (side == C_TOP && (!player->sidekick || self->state == GiantPistol_State_CloseChamber)) {
            player->state           = Player_State_Static;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            player->velocity.x      = 0;
            player->velocity.y      = 0;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            self->activePlayers |= 1 << player->playerID;
            self->state     = GiantPistol_State_CloseChamber;
            self->playerPos = self->direction == FLIP_NONE ? 0x180000 : -0x180000;
            RSDK.PlaySfx(GiantPistol->sfxClack, false, 255);
        }

        Player_CheckCollisionBox(player, self, &GiantPistol->hitboxFront);
        Player_CheckCollisionBox(player, self, &GiantPistol->hitboxRear);
    }
}

void GiantPistol_State_CloseChamber(void)
{
    RSDK_THIS(GiantPistol);

    RSDK.ProcessAnimation(&self->chamberAnimator);

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & self->activePlayers) > 0) {
            player->position.x += (self->position.x + self->playerPos - player->position.x) >> 3;
            player->position.y += (self->position.y - player->position.y - 0x200000) >> 3;
            player->velocity.x = 0;
            player->velocity.y = 0;
        }
    }

    if (self->chamberAnimator.frameID >= self->chamberAnimator.frameCount - 1) {
        self->state                    = GiantPistol_State_SpinGun;
        self->drawFX                   = FX_ROTATE | FX_FLIP;
        self->handLowAnimator.frameID  = 1;
        self->handHighAnimator.frameID = 1;
        self->velocity.y               = -0x20000;
        self->rotationVel              = 6;

        foreach_active(Player, playerPtr)
        {
            if (((1 << (playerPtr->playerID)) & self->activePlayers) > 0) {
                playerPtr->blinkTimer = 0;
                playerPtr->visible    = false;
                if (playerPtr->camera)
                    playerPtr->camera->state = StateMachine_None;
            }
        }
    }
}

void GiantPistol_State_SpinGun(void)
{
    RSDK_THIS(GiantPistol);

    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    if (self->position.y >= self->startPos.y) {
        self->position.y = self->startPos.y;
        self->velocity.y = 0;
    }

    if (self->rotationVel > -64)
        self->rotationVel--;

    self->angle += self->direction ? -self->rotationVel : self->rotationVel;

    self->rotation = self->angle & 0x1FF;
    int32 angle    = 0x100 - (self->rotation >> 1);

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & self->activePlayers) > 0) {
            player->position.x = self->position.x + self->playerPos;
            player->position.y = self->position.y - 0x200000;
            
            Zone_RotateOnPivot(&player->position, &self->position, angle);
        }
    }

    if (self->angle <= -0x1FC || self->angle >= 0x1FC) {
        self->angle       = 0;
        self->rotationVel = -(self->rotationVel >> 1);
        self->rotation    = 0;
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 7, &self->handLowAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 7, &self->handHighAnimator, true, 1);
        self->state  = GiantPistol_State_Aiming;
        self->active = ACTIVE_NORMAL;

        foreach_active(Player, playerPtr)
        {
            if (((1 << (playerPtr->playerID)) & self->activePlayers) > 0) {
                if (playerPtr->camera)
                    playerPtr->camera->state = Camera_State_FollowXY;
            }
        }
    }
}

void GiantPistol_State_Aiming(void)
{
    RSDK_THIS(GiantPistol);

    if (self->timer) {
        self->timer--;
    }
    else {
        if (self->rotationVel > 4)
            self->rotationVel--;

        self->angle += self->direction ? -self->rotationVel : self->rotationVel;
        self->angle &= 0x1FF;
    }

    self->position = self->startPos;
    self->rotation = (-RSDK.Sin512(self->angle) >> 3) & 0x1FF;
    int32 angle    = 0x100 - (self->rotation >> 1);
    Zone_RotateOnPivot(&self->position, &self->pivot, angle);

    self->position.x -= RSDK.Sin512(self->rotation) << 10;
    self->position.y -= RSDK.Sin512(self->rotation) << 10;

    if (self->direction) {
        if (self->playerPos <= 0x500000)
            self->playerPos += 0x40000;
    }
    else {
        if (self->playerPos > -0x500000)
            self->playerPos -= 0x40000;
    }

    uint8 jumpPressed = 0;
#if MANIA_USE_PLUS
    if (GiantPistol->inCutscene) {
#else
    if (!Zone->actID && !CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
#endif
        if (self->angle == 0x76 && self->activePlayers > 0) {
#if MANIA_USE_PLUS
            if (SceneInfo->filter & FILTER_ENCORE) {
                MSZSetup_ReloadBGParallax();
                MSZSetup_ReloadBGParallax_Multiply(0x000);
            }
            else
#endif
                MSZSetup_ReloadBGParallax_Multiply(0x400);

            jumpPressed = true;
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (((1 << (player->playerID)) & self->activePlayers) > 0)
                jumpPressed |= player->jumpPress;
        }
    }

    foreach_active(Player, player)
    {
        if (((1 << (player->playerID)) & self->activePlayers) > 0) {
            player->position.x = self->position.x + self->playerPos;
            player->position.y = self->position.y - 0x200000;
            
            Zone_RotateOnPivot(&player->position, &self->position, angle);
            if (jumpPressed) {
                player->state            = Player_State_Air;
                player->onGround         = false;
                player->jumpAbilityState = 0;
                player->applyJumpCap     = false;
                player->jumpPress        = false;
                player->jumpHold         = false;

                if (self->direction) {
                    player->velocity.x = 0xC00 * RSDK.Cos512(self->rotation);
                    player->velocity.y = 0xC00 * RSDK.Sin512(self->rotation);
                }
                else {
                    player->velocity.x = -0xC00 * RSDK.Cos512(self->rotation);
                    player->velocity.y = -0xC00 * RSDK.Sin512(self->rotation);
                }

#if MANIA_USE_PLUS
                if (SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE) && GiantPistol->inCutscene) {
                    player->velocity.x += 0x18000;
                    player->state           = GiantPistol_PlayerState_PistolAir;
                    player->nextGroundState = GiantPistol_PlayerState_PistolGround;
                }
                else if (!Zone->actID && !CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
                    player->jumpAbilityState = 0;
                }
#else
                if (!Zone->actID && !CHECK_CHARACTER_ID(ID_KNUCKLES, 1))
                    player->jumpAbilityState = 0;
#endif

                self->activePlayers &= ~(1 << player->playerID);
                self->timer = 16;

                player->velocity.x = CLAMP(player->velocity.x, -0x120000, 0x120000);
                player->visible    = true;
                Camera_ShakeScreen(player->playerID, -8, -8);
                RSDK.PlaySfx(GiantPistol->sfxCannonFire, false, 255);
            }
        }
    }

    if (!self->activePlayers && !self->timer) {
        if (self->rotation > 0x100)
            self->rotation -= 0x200;

        self->state = GiantPistol_State_FiredPlayers;
    }
}

void GiantPistol_State_FiredPlayers(void)
{
    RSDK_THIS(GiantPistol);

    if (self->rotation <= 0) {
        self->rotation += 4;

        if (self->rotation > 0)
            self->rotation = 0;
    }
    else {
        self->rotation -= 4;

        if (self->rotation < 0)
            self->rotation = 0;
    }

    if (self->chamberAnimator.frameID > 0)
        self->chamberAnimator.frameID--;

    self->position.x += (self->startPos.x - self->position.x) >> 3;
    self->position.y += (self->startPos.y - self->position.y) >> 3;

    if (++self->timer == 60) {
        self->position = self->startPos;
        self->timer    = 0;
        self->angle    = 0;
        self->drawFX   = FX_FLIP;
        self->active   = ACTIVE_BOUNDS;
        self->state    = GiantPistol_State_AwaitPlayerEntry;

        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &self->chamberAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &self->handLowAnimator, true, 0);
        RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &self->handHighAnimator, true, 0);
    }
}

#if MANIA_USE_PLUS
void GiantPistol_PlayerState_PistolAir(void)
{
    RSDK_THIS(Player);

    self->left      = false;
    self->right     = true;
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    Player_State_Air();

    self->nextGroundState = GiantPistol_PlayerState_PistolGround;
}

void GiantPistol_PlayerState_PistolGround(void)
{
    RSDK_THIS(Player);

    self->left      = true;
    self->right     = false;
    self->up        = false;
    self->down      = false;
    self->jumpPress = false;
    self->jumpHold  = false;

    Player_State_Ground();

    self->state           = GiantPistol_PlayerState_PistolGround;
    self->nextGroundState = GiantPistol_PlayerState_PistolGround;

    if (self->groundVel <= 0) {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_IDLE, &self->animator, true, 0);
        self->left            = false;
        self->skidding        = 0;
        self->groundVel       = 0;
        self->velocity.x      = 0;
        self->direction       = FLIP_NONE;
        self->state           = Player_State_Ground;
        self->nextGroundState = Player_State_Ground;
    }
}
#endif

#if GAME_INCLUDE_EDITOR
void GiantPistol_EditorDraw(void)
{
    RSDK_THIS(GiantPistol);

    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->pistolAnimator, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 0, &self->pistolEntryAnimator, true, 1);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 5, &self->chamberAnimator, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 6, &self->hammerAnimator, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 4, &self->triggerAnimator, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 1, &self->handLowAnimator, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 2, &self->handHighAnimator, true, 0);
    RSDK.SetSpriteAnimation(GiantPistol->aniFrames, 3, &self->triggerFingerAnimator, true, 0);

    RSDK.DrawSprite(&self->pistolEntryAnimator, NULL, false);
    RSDK.DrawSprite(&self->handLowAnimator, NULL, false);
    RSDK.DrawSprite(&self->triggerAnimator, NULL, false);
    RSDK.DrawSprite(&self->hammerAnimator, NULL, false);
    RSDK.DrawSprite(&self->chamberAnimator, NULL, false);
    RSDK.DrawSprite(&self->pistolAnimator, NULL, false);
    RSDK.DrawSprite(&self->handHighAnimator, NULL, false);
    RSDK.DrawSprite(&self->triggerFingerAnimator, NULL, false);
}

void GiantPistol_EditorLoad(void)
{
    GiantPistol->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Pistol.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(GiantPistol, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void GiantPistol_Serialize(void) { RSDK_EDITABLE_VAR(GiantPistol, VAR_UINT8, direction); }
