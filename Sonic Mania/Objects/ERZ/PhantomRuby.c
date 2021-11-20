#include "SonicMania.h"

ObjectPhantomRuby *PhantomRuby;

void PhantomRuby_Update(void)
{
    RSDK_THIS(PhantomRuby);
    StateMachine_Run(self->state);
    if (self->animator1.animationID == 1 && self->animator1.frameID == self->animator1.frameCount - 1)
        RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 0, &self->animator1, true, 0);
    if (self->animator2.animationID == 2 && self->animator2.frameID == self->animator2.frameCount - 1)
        RSDK.SetSpriteAnimation(0xFFFF, 0xFFFF, &self->animator2, true, 0);
    RSDK.ProcessAnimation(&self->animator1);
    RSDK.ProcessAnimation(&self->animator2);
}

void PhantomRuby_LateUpdate(void) {}

void PhantomRuby_StaticUpdate(void) {}

void PhantomRuby_Draw(void)
{
    RSDK_THIS(PhantomRuby);
    RSDK.DrawSprite(&self->animator1, NULL, false);
    if (self->animator2.animationID != -1) {
        self->inkEffect = INK_ADD;
        self->alpha     = 0xFF;
        RSDK.DrawSprite(&self->animator2, NULL, false);
        self->inkEffect = INK_NONE;
    }
}

void PhantomRuby_Create(void *data)
{
    RSDK_THIS(PhantomRuby);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderHigh;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = PhantomRuby_State_FinishedFlash;
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 0, &self->animator1, true, 0);
}

void PhantomRuby_StageLoad(void)
{
    PhantomRuby->aniFrames = RSDK.LoadSpriteAnimation("Global/PhantomRuby.bin", SCOPE_STAGE);
    PhantomRuby->sfxL[0]  = RSDK.GetSFX("Ruby/Attack1_L.wav");
    PhantomRuby->sfxR[0]  = RSDK.GetSFX("Ruby/Attack1_R.wav");
    PhantomRuby->sfxL[1]  = RSDK.GetSFX("Ruby/Attack2_L.wav");
    PhantomRuby->sfxR[1]  = RSDK.GetSFX("Ruby/Attack2_R.wav");
    PhantomRuby->sfxL[2]  = RSDK.GetSFX("Ruby/Attack3_L.wav");
    PhantomRuby->sfxR[2]  = RSDK.GetSFX("Ruby/Attack3_R.wav");
    PhantomRuby->sfxL[3]  = RSDK.GetSFX("Ruby/Attack4_L.wav");
    PhantomRuby->sfxR[3]  = RSDK.GetSFX("Ruby/Attack4_R.wav");
    PhantomRuby->sfxL[4]  = RSDK.GetSFX("Ruby/Attack5_L.wav");
    PhantomRuby->sfxR[4]  = RSDK.GetSFX("Ruby/Attack5_R.wav");
    PhantomRuby->sfxL[5]  = RSDK.GetSFX("Ruby/Attack6_L.wav");
    PhantomRuby->sfxR[5]  = RSDK.GetSFX("Ruby/Attack6_R.wav");
    PhantomRuby->sfxL[6]  = RSDK.GetSFX("Ruby/RedCube_L.wav");
    PhantomRuby->sfxR[6]  = RSDK.GetSFX("Ruby/RedCube_R.wav");
}

void PhantomRuby_PlaySFX(uint8 sfxID)
{
    if (sfxID) {
        uint8 sfx    = sfxID - 1;
        int32 channel = RSDK.PlaySfx(PhantomRuby->sfxL[sfx], 0, 0);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        channel = RSDK.PlaySfx(PhantomRuby->sfxR[sfx], 0, 0);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
    }
}

void PhantomRuby_SetupFlash(EntityPhantomRuby *ruby)
{
    ruby->flashFinished = false;
    ruby->hasFlashed    = false;
    ruby->timer         = 0;
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 1, &ruby->animator1, true, 0);
    RSDK.SetSpriteAnimation(PhantomRuby->aniFrames, 2, &ruby->animator2, true, 0);
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
        PhantomRuby_PlaySFX(self->sfx);
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

void PhantomRuby_State_FallOffScreen(void)
{
    RSDK_THIS(PhantomRuby);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;
    if (self->position.y > (Zone->screenBoundsB1[0] + 64) << 16)
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

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        self->velocity.x >>= 1;
        if (-(self->velocity.y >> 1) > -0x10000)
            self->velocity.y = 0;
        else
            self->velocity.y = -(self->velocity.y >> 1);
    }
}

#if RETRO_USE_PLUS
void PhantomRuby_State_MoveToPos(void)
{
    RSDK_THIS(PhantomRuby);
    int32 rx    = (self->startPos.x - self->position.x) >> 16;
    int32 ry    = (self->startPos.y - self->position.y) >> 16;
    int32 angle = RSDK.ATan2(rx, ry);
    self->velocity.x += RSDK.Cos256(angle) << 3;
    self->velocity.y += RSDK.Sin256(angle) << 3;
    int32 r = rx * rx + ry * ry;
    if (r >= 16) {
        if (r < 2304) {
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

#if RETRO_INCLUDE_EDITOR
void PhantomRuby_EditorDraw(void) { PhantomRuby_Draw(); }

void PhantomRuby_EditorLoad(void) { PhantomRuby->aniFrames = RSDK.LoadSpriteAnimation("Global/PhantomRuby.bin", SCOPE_STAGE); }
#endif

void PhantomRuby_Serialize(void) { RSDK_EDITABLE_VAR(PhantomRuby, VAR_UINT8, sfx); }
