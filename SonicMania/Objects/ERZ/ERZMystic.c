// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ERZMystic Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectERZMystic *ERZMystic;

void ERZMystic_Update(void)
{
    RSDK_THIS(ERZMystic);

    if (self->invincibilityTimer > 0)
        self->invincibilityTimer--;

    StateMachine_Run(self->state);

    RSDK.ProcessAnimation(&self->mysticAnimator);
}

void ERZMystic_LateUpdate(void) {}

void ERZMystic_StaticUpdate(void) {}

void ERZMystic_Draw(void)
{
    RSDK_THIS(ERZMystic);

    if (self->stateDraw) {
        StateMachine_Run(self->stateDraw);
    }
    else {
        RSDK.DrawSprite(&self->mysticAnimator, NULL, false);
    }
}

void ERZMystic_Create(void *data)
{
    RSDK_THIS(ERZMystic);

    self->drawFX = FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->active        = ACTIVE_NORMAL;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->unused1[0] = 0;
        self->unused1[1] = 1;
        self->unused1[2] = 2;

        self->cupAlpha[0] = 0xC0;
        self->cupAlpha[1] = 0x00;
        self->cupAlpha[2] = 0xC0;

        self->hitbox.left   = -12;
        self->hitbox.top    = -12;
        self->hitbox.right  = 12;
        self->hitbox.bottom = 12;

        self->correctCup = 1;
        self->state      = ERZMystic_State_Init;
        self->stateDraw  = ERZMystic_Draw_CupSetup;

        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 2, &self->mysticAnimator, true, 0);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->cupAnimator, true, 0);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->cupSilhouetteAnimator, true, 1);
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 1, &self->cupSpikeAnimator, true, 0);
    }
}

void ERZMystic_StageLoad(void) { ERZMystic->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomMystic.bin", SCOPE_STAGE); }

void ERZMystic_CheckPlayerCollisions(void)
{
    RSDK_THIS(ERZMystic);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    self->position.x = self->mysticPos.x;
    self->position.y = self->mysticPos.y;

    foreach_active(Player, player)
    {
        if (!self->invincibilityTimer && Player_CheckBadnikTouch(player, self, &self->hitbox) && Player_CheckBossHit(player, self)) {
            ERZMystic_Hit();
        }

        if (self->cupBlastAnimator.frameID > 8 && self->cupBlastAnimator.frameID < 26) {
            for (int32 i = 0; i < 3; ++i) {
                if (i != self->correctCup) {
                    self->position.x = storeX + self->cupPos[i];
                    if (abs(self->position.x - player->position.x) < 0x400000 && player->position.y > self->position.y)
                        Player_Hurt(player, self);
                }
            }
        }

        self->position.x = self->mysticPos.x;
    }

    self->position.x = storeX;
    self->position.y = storeY;
}

void ERZMystic_Hit(void)
{
    RSDK_THIS(ERZMystic);

    self->invincibilityTimer = 48;
    RSDK.PlaySfx(ERZKing->sfxHit, false, 255);
}

void ERZMystic_SetupNewCupSwap(void)
{
    RSDK_THIS(ERZMystic);

    self->swapCup1 = RSDK.Rand(0, 3);

    switch (self->swapCup1) {
        case 0: self->swapCup2 = (RSDK.Rand(0, 256) > 128) + 1; break;
        case 1: self->swapCup2 = RSDK.Rand(0, 256) <= 128 ? 2 : 0; break;
        case 2: self->swapCup2 = RSDK.Rand(0, 256) <= 128; break;
    }

    self->swapCup1Pos   = self->cupPos[self->swapCup1];
    self->swapCup1Alpha = self->cupAlpha[self->swapCup1];

    self->swapCup2Pos   = self->cupPos[self->swapCup2];
    self->swapCup2Alpha = self->cupAlpha[self->swapCup2];
}

void ERZMystic_Draw_CupSetup(void)
{
    RSDK_THIS(ERZMystic);

    RSDK.DrawSprite(&self->mysticAnimator, &self->mysticPos, false);

    for (int32 i = 0; i < 3; ++i) {
        Vector2 drawPos;
        drawPos.x = self->position.x + self->cupPos[i];
        if (i == 1) {
            drawPos.y    = self->middleCupY;
            self->drawFX = FX_ROTATE | FX_FLIP;
        }
        else {
            drawPos.y = self->position.y;
        }

        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->cupAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->cupAnimator, &drawPos, false);

        self->inkEffect = INK_ALPHA;
        self->drawFX    = FX_FLIP;
        self->alpha     = self->cupAlpha[i];
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->cupSilhouetteAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->cupSilhouetteAnimator, &drawPos, false);

        self->inkEffect = INK_NONE;
    }
}

void ERZMystic_Draw_CupSwap(void)
{
    RSDK_THIS(ERZMystic);

    Vector2 drawPos = self->position;
    if (self->invincibilityTimer & 1) {
        RSDK.CopyPalette(2, 128, 0, 128, 128);

        RSDK.DrawSprite(&self->mysticAnimator, &self->mysticPos, false);

        RSDK.CopyPalette(1, 128, 0, 128, 128);
    }
    else {
        RSDK.DrawSprite(&self->mysticAnimator, &self->mysticPos, false);
    }

    for (int32 i = 0; i < 3; ++i) {
        drawPos.x       = self->position.x + self->cupPos[i];
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->cupAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->cupAnimator, &drawPos, false);

        self->inkEffect = INK_ALPHA;
        self->alpha     = self->cupAlpha[i];
        self->direction = FLIP_X;
        RSDK.DrawSprite(&self->cupSilhouetteAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&self->cupSilhouetteAnimator, &drawPos, false);

        self->inkEffect = INK_NONE;
        if (i != self->correctCup)
            RSDK.DrawSprite(&self->cupBlastAnimator, &drawPos, false);
    }
}

void ERZMystic_State_Init(void)
{
    RSDK_THIS(ERZMystic);

    self->mysticPos.x = self->position.x;
    self->velocity.y  = 0x40000;
    self->mysticVelY  = -0x80000;
    self->mysticPos.y = self->position.y;
    self->middleCupY  = self->position.y;

    self->state = ERZMystic_State_SetupInitialCupPos;
}

void ERZMystic_State_SetupInitialCupPos(void)
{
    RSDK_THIS(ERZMystic);

    self->mysticVelY += 0x3800;
    self->mysticPos.y += self->mysticVelY;

    self->velocity.y -= 0x2800;
    self->middleCupY += self->velocity.y;

    if (self->middleCupY <= self->position.y) {
        self->velocity.y = 0;
        self->middleCupY = self->position.y;
    }

    self->cupPos[0] += (-0x800000 - self->cupPos[0]) >> 4;
    self->cupPos[2] += ((0x800000 - self->cupPos[2]) >> 4);

    if (self->rotation < 256) {
        self->rotation += 16;
    }

    if (++self->timer == 60) {
        self->cupPos[0] = -0x800000;
        self->cupPos[2] = 0x800000;
        self->timer     = 0;
        self->state     = ERZMystic_State_MoveCupsDownwards;
    }
}

void ERZMystic_State_MoveCupsDownwards(void)
{
    RSDK_THIS(ERZMystic);

    self->mysticVelY += 0x3800;
    self->mysticPos.y += self->mysticVelY;

    self->velocity.y += 0x3800;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x380000, true))
        self->velocity.y = 0;

    if (self->mysticPos.y >= self->position.y - 0x180000) {
        self->mysticPos.y = 0;
        self->mysticVelY  = 0;
        self->onGround    = false;
        self->state       = ERZMystic_State_RotateMiddleCup;
    }
}

void ERZMystic_State_RotateMiddleCup(void)
{
    RSDK_THIS(ERZMystic);

    // This does continue to move the side cups downwards too if they're not on the ground yet
    self->velocity.y += 0x4000;
    self->position.y += self->velocity.y;

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x380000, true)) {
        if (!self->onGround)
            Camera_ShakeScreen(0, 0, 4);

        self->velocity.y = 0;
        self->onGround   = true;
    }

    if (self->rotation >= 512) {
        self->rotation = 0;
        self->state    = ERZMystic_State_MoveMiddleCupToFloor;
    }
    else {
        self->rotation += 16;
    }
}

void ERZMystic_State_MoveMiddleCupToFloor(void)
{
    RSDK_THIS(ERZMystic);

    self->velocity.y += 0x4000;
    self->middleCupY += self->velocity.y;

    if (self->middleCupY >= self->position.y) {
        Camera_ShakeScreen(0, 0, 4);
        self->velocity.y = 0;
        self->middleCupY = self->position.y;
        self->stateDraw  = ERZMystic_Draw_CupSwap;
        self->state      = ERZMystic_State_PrepareCupSwap;
    }
}

void ERZMystic_State_PrepareCupSwap(void)
{
    RSDK_THIS(ERZMystic);

    ++self->timer;
    if (self->cupAlpha[0] < 0x90) {
        self->cupAlpha[0] += 4;
        self->cupAlpha[2] += 4;
    }

    if (self->timer == 30) {
        self->timer        = 0;
        self->cupSwapCount = 6;
        ERZMystic_SetupNewCupSwap();
        self->state = ERZMystic_State_CupSwapping;
    }
}

void ERZMystic_State_CupSwapping(void)
{
    RSDK_THIS(ERZMystic);

    if (abs(self->swapCup2Pos - self->swapCup1Pos) <= 0x800000)
        self->timer += 16;
    else
        self->timer += 8;

    int32 cup1 = self->swapCup1;
    int32 cup2 = self->swapCup2;

    // Use Lerp Math to move each cup to the other's initial position
    int32 cup1Pos = self->swapCup1Pos;
    if (self->timer > 0) {
        if (self->timer < 256)
            cup1Pos += ((self->swapCup2Pos - cup1Pos) >> 8) * ((RSDK.Sin512(self->timer + 0x180) >> 2) + 0x80);
        else
            cup1Pos = self->swapCup2Pos;
    }
    self->cupPos[cup1] = cup1Pos;

    int32 cup2Pos = self->swapCup2Pos;
    if (self->timer > 0) {
        if (self->timer < 256)
            cup2Pos += ((self->swapCup1Pos - cup2Pos) >> 8) * ((RSDK.Sin512(self->timer + 0x180) >> 2) + 0x80);
        else
            cup2Pos = self->swapCup1Pos;
    }
    self->cupPos[cup2] = cup2Pos;

    self->cupAlpha[cup1] = abs(self->cupPos[cup1]) / 0xE38E;
    self->cupAlpha[cup2] = abs(self->cupPos[cup2]) / 0xE38E;

    if (self->timer == 0x100) {
        self->timer = 0;

        if (self->cupSwapCount <= 1) {
            self->middleCupY -= 0x600000;
            self->mysticPos.x = self->position.x + self->cupPos[self->correctCup];
            self->mysticPos.y = self->position.y;
            self->originPos.x = self->mysticPos.x;
            self->originPos.y = self->mysticPos.y;
            self->velocity.y  = -0x10000;
            self->state       = ERZMystic_State_RevealMystic;
        }
        else {
            self->cupSwapCount--;
            RSDK.PlaySfx(PhantomMystic->sfxCupSwap, false, 255);
            ERZMystic_SetupNewCupSwap();
        }
    }
}

void ERZMystic_State_RevealMystic(void)
{
    RSDK_THIS(ERZMystic);

    self->velocity.y -= 0x3800;
    self->position.y += self->velocity.y;

    self->cupAlpha[0] -= self->cupAlpha[0] >> 4;
    self->cupAlpha[1] -= self->cupAlpha[1] >> 4;
    self->cupAlpha[2] -= self->cupAlpha[2] >> 4;

    if (self->position.y <= self->middleCupY) {
        RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 3, &self->cupBlastAnimator, true, 0);
        self->velocity.y = 0;
        self->position.y = self->middleCupY;
        self->state      = ERZMystic_State_CupBlast;
    }

    self->mysticPos.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 11);

    if (self->position.y - self->middleCupY < 0x200000)
        ERZMystic_CheckPlayerCollisions();
}

void ERZMystic_State_CupBlast(void)
{
    RSDK_THIS(ERZMystic);

    RSDK.ProcessAnimation(&self->cupBlastAnimator);

    self->cupAlpha[0] -= self->cupAlpha[0] >> 4;
    self->cupAlpha[1] -= self->cupAlpha[1] >> 4;
    self->cupAlpha[2] -= self->cupAlpha[2] >> 4;

    if (self->cupBlastAnimator.frameID == self->cupBlastAnimator.frameCount - 1) {
        self->mysticVelY = 0;
        self->state      = ERZMystic_State_MoveCupsToMystic;
    }

    self->mysticPos.y = BadnikHelpers_Oscillate(self->originPos.y, 4, 11);

    ERZMystic_CheckPlayerCollisions();
}

void ERZMystic_State_MoveCupsToMystic(void)
{
    RSDK_THIS(ERZMystic);

    self->cupPos[0] += (self->cupPos[self->correctCup] - self->cupPos[0]) >> 3;
    self->cupPos[1] += (self->cupPos[self->correctCup] - self->cupPos[1]) >> 3;
    self->cupPos[2] += (self->cupPos[self->correctCup] - self->cupPos[2]) >> 3;

    self->mysticVelY -= 0x3000;
    self->mysticPos.y += self->mysticVelY;

    if (self->mysticPos.y < self->position.y)
        self->mysticPos.y = self->position.y;

    ERZMystic_CheckPlayerCollisions();
    if (++self->timer == 60) {
        foreach_all(ERZKing, king)
        {
            king->active     = ACTIVE_NORMAL;
            king->position.x = self->mysticPos.x;
        }
        destroyEntity(self);
    }
}

#if GAME_INCLUDE_EDITOR
void ERZMystic_EditorDraw(void)
{
    RSDK_THIS(ERZMystic);

    self->mysticPos  = self->position;
    self->middleCupY = self->position.y;

    self->cupAlpha[0] = 0x60;
    self->cupAlpha[1] = 0;
    self->cupAlpha[2] = 0x60;

    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 2, &self->mysticAnimator, true, 0);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->cupAnimator, true, 0);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 0, &self->cupSilhouetteAnimator, true, 1);
    RSDK.SetSpriteAnimation(ERZMystic->aniFrames, 1, &self->cupSpikeAnimator, true, 0);

    ERZMystic_Draw_CupSetup();
}

void ERZMystic_EditorLoad(void) { ERZMystic->aniFrames = RSDK.LoadSpriteAnimation("Phantom/PhantomMystic.bin", SCOPE_STAGE); }
#endif

void ERZMystic_Serialize(void) {}
