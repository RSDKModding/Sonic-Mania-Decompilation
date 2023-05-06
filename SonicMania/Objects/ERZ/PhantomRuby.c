// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PhantomRuby Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPhantomRuby *PhantomRuby;

void PhantomRuby_Update(void)
{
    RSDK_THIS(PhantomRuby);

    StateMachine_Run(self->state);

    // flash
    if (self->rubyAnimator.animationID == 1 && self->rubyAnimator.frameID == self->rubyAnimator.frameCount - 1)
        RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 0, &self->rubyAnimator, true, 0);

    // flash add
    if (self->flashAnimator.animationID == 2 && self->flashAnimator.frameID == self->flashAnimator.frameCount - 1)
        RSDK.SetSpriteAnimation(-1, 0xFFFF, &self->flashAnimator, true, 0);

    RSDK.ProcessAnimation(&self->rubyAnimator);
    RSDK.ProcessAnimation(&self->flashAnimator);
}

void PhantomRuby_LateUpdate(void) {}

void PhantomRuby_StaticUpdate(void) {}

void PhantomRuby_Draw(void)
{
    RSDK_THIS(PhantomRuby);

    RSDK.DrawSprite(&self->rubyAnimator, NULL, false);

    if (self->flashAnimator.animationID != -1) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0xFF;
        RSDK.DrawSprite(&self->flashAnimator, NULL, false);

        self->inkEffect = INK_NONE;
    }
}

void PhantomRuby_Create(void *data)
{
    RSDK_THIS(PhantomRuby);

    self->active = ACTIVE_BOUNDS;
    if (!SceneInfo->inEditor)
        self->drawGroup = Zone->objectDrawGroup[1];

    self->startPos      = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = TO_FIXED(128);
    self->updateRange.y = TO_FIXED(128);
    self->state         = PhantomRuby_State_FinishedFlash;

    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 0, &self->rubyAnimator, true, 0);
}

void PhantomRuby_StageLoad(void)
{
    PhantomRuby->aniFrames = RSDK.LoadSpriteAnimation("Global/PhantomRuby.bin", SCOPE_STAGE);

    PhantomRuby->sfxL[RUBYSFX_ATTACK1 - 1] = RSDK.GetSfx("Ruby/Attack1_L.wav");
    PhantomRuby->sfxR[RUBYSFX_ATTACK1 - 1] = RSDK.GetSfx("Ruby/Attack1_R.wav");
    PhantomRuby->sfxL[RUBYSFX_ATTACK2 - 1] = RSDK.GetSfx("Ruby/Attack2_L.wav");
    PhantomRuby->sfxR[RUBYSFX_ATTACK2 - 1] = RSDK.GetSfx("Ruby/Attack2_R.wav");
    PhantomRuby->sfxL[RUBYSFX_ATTACK3 - 1] = RSDK.GetSfx("Ruby/Attack3_L.wav");
    PhantomRuby->sfxR[RUBYSFX_ATTACK3 - 1] = RSDK.GetSfx("Ruby/Attack3_R.wav");
    PhantomRuby->sfxL[RUBYSFX_ATTACK4 - 1] = RSDK.GetSfx("Ruby/Attack4_L.wav");
    PhantomRuby->sfxR[RUBYSFX_ATTACK4 - 1] = RSDK.GetSfx("Ruby/Attack4_R.wav");
    PhantomRuby->sfxL[RUBYSFX_ATTACK5 - 1] = RSDK.GetSfx("Ruby/Attack5_L.wav");
    PhantomRuby->sfxR[RUBYSFX_ATTACK5 - 1] = RSDK.GetSfx("Ruby/Attack5_R.wav");
    PhantomRuby->sfxL[RUBYSFX_ATTACK6 - 1] = RSDK.GetSfx("Ruby/Attack6_L.wav");
    PhantomRuby->sfxR[RUBYSFX_ATTACK6 - 1] = RSDK.GetSfx("Ruby/Attack6_R.wav");
    PhantomRuby->sfxL[RUBYSFX_REDCUBE - 1] = RSDK.GetSfx("Ruby/RedCube_L.wav");
    PhantomRuby->sfxR[RUBYSFX_REDCUBE - 1] = RSDK.GetSfx("Ruby/RedCube_R.wav");
}

void PhantomRuby_PlaySfx(uint8 sfxID)
{
    if (sfxID) {
        int32 channel = RSDK.PlaySfx(PhantomRuby->sfxL[sfxID - 1], false, 0x00);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);

        channel = RSDK.PlaySfx(PhantomRuby->sfxR[sfxID - 1], false, 0x00);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
    }
}

void PhantomRuby_SetupFlash(EntityPhantomRuby *ruby)
{
    ruby->flashFinished = false;
    ruby->hasFlashed    = false;
    ruby->timer         = 0;

    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 1, &ruby->rubyAnimator, true, 0);
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 2, &ruby->flashAnimator, true, 0);

    ruby->state = PhantomRuby_State_PlaySfx;
}

void PhantomRuby_State_FinishedFlash(void)
{
    RSDK_THIS(PhantomRuby);

    if (self->flashFinished)
        self->flashFinished = false;
}

void PhantomRuby_State_PlaySfx(void)
{
    RSDK_THIS(PhantomRuby);

    if (self->timer == 38) {
        PhantomRuby_PlaySfx(self->sfx);

        self->flashFinished = true;
        self->hasFlashed    = true;
        self->timer         = 0;
        self->state         = PhantomRuby_State_FinishedFlash;
    }
    else {
        self->timer++;
    }
}

void PhantomRuby_State_Oscillate(void)
{
    RSDK_THIS(PhantomRuby);

    self->position.y = BadnikHelpers_Oscillate(self->startPos.y, 2, 10);
}

void PhantomRuby_State_MoveGravity(void)
{
    RSDK_THIS(PhantomRuby);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (self->position.y > (Zone->cameraBoundsB[0] + 64) << 16)
        self->state = StateMachine_None;
}

void PhantomRuby_State_MoveRotateGravity(void)
{
    RSDK_THIS(PhantomRuby);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    self->rotation = (self->rotation + 5) & 0x1FF;
}

void PhantomRuby_State_MoveRotateGravity_CheckGround(void)
{
    RSDK_THIS(PhantomRuby);

    PhantomRuby_State_MoveRotateGravity();

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, TO_FIXED(8), true)) {
        self->velocity.x >>= 1;
        if (-(self->velocity.y >> 1) > -TO_FIXED(1))
            self->velocity.y = 0;
        else
            self->velocity.y = -(self->velocity.y >> 1);
    }
}

#if MANIA_USE_PLUS
void PhantomRuby_State_MoveToPos(void)
{
    RSDK_THIS(PhantomRuby);

    int32 rx    = (self->startPos.x - self->position.x) >> 16;
    int32 ry    = (self->startPos.y - self->position.y) >> 16;
    int32 angle = RSDK.ATan2(rx, ry);

    self->velocity.x += RSDK.Cos256(angle) << 3;
    self->velocity.y += RSDK.Sin256(angle) << 3;

    int32 r = rx * rx + ry * ry;
    if (r >= 0x10) {
        if (r < 0x900) {
            self->velocity.x = (self->startPos.x - self->position.x) >> 4;
            self->velocity.y = (self->startPos.y - self->position.y) >> 4;
        }

        self->position.x += self->velocity.x;
        self->position.y += self->velocity.y;
    }
    else {
        self->position.x = self->startPos.x;
        self->position.y = self->startPos.y;
        self->state      = PhantomRuby_State_Oscillate;
    }
}
#endif

void PhantomRuby_State_RotateToOrigin(void)
{
    RSDK_THIS(PhantomRuby);

    self->rotation += 6;

    if (self->rotation > 0x200) {
        self->rotation = 0;
        self->drawFX   = FX_NONE;
        self->state    = StateMachine_None;
    }
}

#if GAME_INCLUDE_EDITOR
void PhantomRuby_EditorDraw(void)
{
    RSDK_THIS(PhantomRuby);
    PhantomRuby_SetupFlash(self);

    PhantomRuby_Draw();
}

void PhantomRuby_EditorLoad(void)
{
    PhantomRuby->aniFrames = RSDK.LoadSpriteAnimation("Global/PhantomRuby.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PhantomRuby, sfx);
    RSDK_ENUM_VAR("None", RUBYSFX_NONE);
    RSDK_ENUM_VAR("Attack 1", RUBYSFX_ATTACK1);
    RSDK_ENUM_VAR("Attack 2", RUBYSFX_ATTACK2);
    RSDK_ENUM_VAR("Attack 3", RUBYSFX_ATTACK3);
    RSDK_ENUM_VAR("Attack 4", RUBYSFX_ATTACK4);
    RSDK_ENUM_VAR("Attack 5", RUBYSFX_ATTACK5);
    RSDK_ENUM_VAR("Attack 6", RUBYSFX_ATTACK6);
    RSDK_ENUM_VAR("Red Cube", RUBYSFX_REDCUBE);
}
#endif

void PhantomRuby_Serialize(void) { RSDK_EDITABLE_VAR(PhantomRuby, VAR_UINT8, sfx); }
