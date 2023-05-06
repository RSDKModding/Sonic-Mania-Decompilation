// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Clapperboard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectClapperboard *Clapperboard;

void Clapperboard_Update(void)
{
    RSDK_THIS(Clapperboard);

    StateMachine_Run(self->state);

    self->rotation     = self->angle >> 1;
    self->stoodPlayers = 0;
    StateMachine_Run(self->stateCollide);
}

void Clapperboard_LateUpdate(void) {}

void Clapperboard_StaticUpdate(void) {}

void Clapperboard_Draw(void)
{
    RSDK_THIS(Clapperboard);
    Vector2 drawPos;

    self->drawFX = FX_ROTATE | FX_FLIP;
    RSDK.DrawSprite(&self->clapperTopAnimator, NULL, false);

    self->drawFX = FX_FLIP;
    RSDK.DrawSprite(&self->clapperBottomAnimator, NULL, false);

    self->drawFX = FX_NONE;
    RSDK.DrawSprite(&self->boardAnimator, NULL, false);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (self->direction == FLIP_X)
        drawPos.x -= 0x670000;
    else
        drawPos.x += 0x10000;
    self->numbersAnimator.frameID = self->takeID / 100;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0xD0000;
    self->numbersAnimator.frameID = (self->takeID / 10) % 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0xD0000;
    self->numbersAnimator.frameID = self->takeID % 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0x120000;
    self->numbersAnimator.frameID = SceneInfo->minutes % 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0x120000;
    self->numbersAnimator.frameID = SceneInfo->seconds / 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0xD0000;
    self->numbersAnimator.frameID = SceneInfo->seconds % 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);

    drawPos.x += 0x110000;
    self->numbersAnimator.frameID = SceneInfo->milliseconds / 10;
    RSDK.DrawSprite(&self->numbersAnimator, &drawPos, false);
}

void Clapperboard_Create(void *data)
{
    RSDK_THIS(Clapperboard);

    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &self->clapperTopAnimator, true, 0);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &self->clapperBottomAnimator, true, 1);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 0, &self->boardAnimator, true, 2);
    RSDK.SetSpriteAnimation(Clapperboard->aniFrames, 1, &self->numbersAnimator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        if (self->direction == FLIP_NONE) {
            self->amplitudeL.x = -0x40000;
            self->amplitudeR.x = 0x6C0000;
            self->angle        = 0x3C0;
            self->stateCollide = Clapperboard_Collide_Right;
        }
        else {
            ++self->boardAnimator.frameID;
            self->amplitudeL.x = -0x6C0000;
            self->amplitudeR.x = 0x40000;
            self->angle        = 0x040;
            self->stateCollide = Clapperboard_Collide_Left;
        }

        self->amplitudeL.y = -0x40000;
        self->amplitudeR.y = -0x40000;
        self->state        = Clapperboard_State_Idle;
        self->visible      = true;
        self->drawGroup    = Zone->objectDrawGroup[0];
    }
}

void Clapperboard_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Clapperboard.bin", SCOPE_STAGE);
    else
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Clapperboard.bin", SCOPE_STAGE);

    Clapperboard->sfxWhack = RSDK.GetSfx("Stage/Whack.wav");
}

void Clapperboard_Collide_Left(void)
{
    RSDK_THIS(Clapperboard);

    uint8 negAngle = -(-self->angle >> 2);

    int32 ampL = (self->amplitudeL.y >> 8) * RSDK.Cos256(negAngle) - (self->amplitudeL.x >> 8) * RSDK.Sin256(negAngle);
    int32 ampR = (self->amplitudeR.y >> 8) * RSDK.Cos256(negAngle) - (self->amplitudeR.x >> 8) * RSDK.Sin256(negAngle);

    self->hitbox.left  = ((self->amplitudeL.y >> 8) * RSDK.Sin256(negAngle) + (self->amplitudeL.x >> 8) * RSDK.Cos256(negAngle)) >> 16;
    self->hitbox.right = ((self->amplitudeR.y >> 8) * RSDK.Sin256(negAngle) + (self->amplitudeR.x >> 8) * RSDK.Cos256(negAngle)) >> 16;

    int32 clapStartPos = self->hitbox.right - self->hitbox.left;
    self->direction ^= FLIP_X;
    bool32 clapped = false;

    foreach_active(Player, player)
    {
        int32 standPos      = self->hitbox.right - CLAMP((player->position.x - self->position.x) >> 16, self->hitbox.left, self->hitbox.right);
        int32 top           = (ampR >> 16) + ((ampR >> 16) - (ampL >> 16)) * standPos / clapStartPos - (self->clapSpeed & 0xFFFF);
        self->hitbox.top    = top;
        self->hitbox.bottom = top + 24;
        if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif

            player->position.y += (self->clapSpeed + 2) << 16;
            if (standPos > (clapStartPos >> 2)) {
                self->stoodPlayers |= RSDK.GetEntitySlot(player) + 1;
                if (self->state == Clapperboard_State_ClapReboundL
                    || (abs(player->groundVel) > 0x80000 && standPos > clapStartPos - (clapStartPos >> 2))) {
                    player->onGround   = false;
                    player->state      = Player_State_Air;
                    player->velocity.y = -0x40000 - 0x60000 * standPos / clapStartPos;

                    int32 anim = player->animator.animationID;
                    if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                        player->animationReserve = player->animator.animationID;
                    else
                        player->animationReserve = ANI_WALK;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, true, 0);
                    RSDK.PlaySfx(Clapperboard->sfxWhack, false, 255);
                    clapped = true;
                }
                else {
                    int32 clapSpeed = 16;
                    if (abs(player->groundVel) < 0xA0000)
                        clapSpeed = 16 * standPos / clapStartPos;

                    if (clapSpeed > self->clapSpeed)
                        self->clapSpeed = clapSpeed;
                }
            }
        }
    }

    self->direction ^= FLIP_X;
    if (clapped)
        ++self->takeID;
}

void Clapperboard_Collide_Right(void)
{
    RSDK_THIS(Clapperboard);

    int32 negAngle = -(-self->angle >> 2);

    int32 ampL = (self->amplitudeL.y >> 8) * RSDK.Cos256(negAngle) - (self->amplitudeL.x >> 8) * RSDK.Sin256(negAngle);
    int32 ampR = (self->amplitudeR.y >> 8) * RSDK.Cos256(negAngle) - (self->amplitudeR.x >> 8) * RSDK.Sin256(negAngle);

    self->hitbox.left  = (((self->amplitudeL.y >> 8) * RSDK.Sin256(negAngle)) + (self->amplitudeL.x >> 8) * RSDK.Cos256(negAngle)) >> 16;
    self->hitbox.right = (((self->amplitudeR.y >> 8) * RSDK.Sin256(negAngle)) + (self->amplitudeR.x >> 8) * RSDK.Cos256(negAngle)) >> 16;
    int32 clapStartPos = self->hitbox.right - self->hitbox.left;
    bool32 clapped     = false;

    foreach_active(Player, player)
    {
        int32 standPos      = CLAMP((player->position.x - self->position.x) >> 16, self->hitbox.left, self->hitbox.right) - self->hitbox.left;
        int32 top           = (ampL >> 16) + ((ampL >> 16) - (ampR >> 16)) * standPos / clapStartPos - (self->clapSpeed & 0xFFFF);
        self->hitbox.top    = top;
        self->hitbox.bottom = top + 24;

        if (Player_CheckCollisionPlatform(player, self, &self->hitbox)) {
#if MANIA_USE_PLUS
            if (player->state == Player_State_MightyHammerDrop)
                player->state = Player_State_Air;
#endif

            player->position.y += (self->clapSpeed + 2) << 16;
            if (standPos > clapStartPos >> 2) {
                self->stoodPlayers |= RSDK.GetEntitySlot(player) + 1;
                if (self->state == Clapperboard_State_ClapReboundR
                    || (abs(player->groundVel) > 0x80000 && standPos > clapStartPos - (clapStartPos >> 2))) {
                    player->onGround   = false;
                    player->state      = Player_State_Air;
                    player->velocity.y = -0x40000 - 0x60000 * standPos / clapStartPos;

                    int32 anim = player->animator.animationID;
                    if (anim == ANI_WALK || (anim > ANI_AIR_WALK && anim <= ANI_DASH))
                        player->animationReserve = player->animator.animationID;
                    else
                        player->animationReserve = ANI_WALK;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPRING_DIAGONAL, &player->animator, true, 0);
                    RSDK.PlaySfx(Clapperboard->sfxWhack, false, 0xFF);
                    clapped = true;
                }
                else {
                    int32 clapSpeed = 16;
                    if (abs(player->groundVel) < 0xA0000)
                        clapSpeed = 16 * standPos / clapStartPos;

                    if (clapSpeed > self->clapSpeed)
                        self->clapSpeed = clapSpeed;
                }
            }
        }
    }

    if (clapped)
        ++self->takeID;
}

void Clapperboard_State_Idle(void)
{
    RSDK_THIS(Clapperboard);

    if (self->stoodPlayers > 0)
        self->state = self->direction == FLIP_NONE ? Clapperboard_State_ClappingR : Clapperboard_State_ClappingL;
}

void Clapperboard_State_ClappingL(void)
{
    RSDK_THIS(Clapperboard);

    self->angle -= self->clapSpeed;
    if (self->angle <= 0x000) {
        self->angle = 0x000;
        self->state = Clapperboard_State_ClapReboundL;
    }
}

void Clapperboard_State_ClapReboundL(void)
{
    RSDK_THIS(Clapperboard);

    self->angle += self->clapSpeed;
    if (self->angle >= 0x040) {
        self->angle     = 0x040;
        self->clapSpeed = 0;
        self->state     = Clapperboard_State_Idle;
    }
}

void Clapperboard_State_ClappingR(void)
{
    RSDK_THIS(Clapperboard);

    self->angle += self->clapSpeed;
    if (self->angle >= 0x400) {
        self->angle = 0x400;
        self->state = Clapperboard_State_ClapReboundR;
    }
}

void Clapperboard_State_ClapReboundR(void)
{
    RSDK_THIS(Clapperboard);

    self->angle -= self->clapSpeed;
    if (self->angle <= 0x3C0) {
        self->angle     = 0x3C0;
        self->clapSpeed = 0;
        self->state     = Clapperboard_State_Idle;
    }
}

#if GAME_INCLUDE_EDITOR
void Clapperboard_EditorDraw(void)
{
    RSDK_THIS(Clapperboard);

    self->boardAnimator.frameID = self->direction == FLIP_NONE ? 2 : 3;

    Clapperboard_Draw();
}

void Clapperboard_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Clapperboard.bin", SCOPE_STAGE);
    else
        Clapperboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Clapperboard.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Clapperboard, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Clapperboard_Serialize(void) { RSDK_EDITABLE_VAR(Clapperboard, VAR_UINT8, direction); }
