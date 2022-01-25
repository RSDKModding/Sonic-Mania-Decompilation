// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UFO_Ring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUFO_Ring *UFO_Ring;

void UFO_Ring_Update(void)
{
    RSDK_THIS(UFO_Ring);
    StateMachine_Run(self->state);
}

void UFO_Ring_LateUpdate(void)
{
    RSDK_THIS(UFO_Ring);
    int32 x = self->position.x >> 8;
    int32 y = self->height >> 8;
    int32 z = self->position.y >> 8;

    Matrix *mat = &UFO_Camera->matWorld;

    self->worldX  = mat->values[0][3] + (y * mat->values[0][1] >> 8) + (z * mat->values[0][2] >> 8) + (x * mat->values[0][0] >> 8);
    self->worldY  = mat->values[1][3] + (y * mat->values[1][1] >> 8) + (z * mat->values[1][2] >> 8) + (x * mat->values[1][0] >> 8);
    self->depth3D = mat->values[2][3] + (y * mat->values[2][1] >> 8) + (z * mat->values[2][2] >> 8) + (x * mat->values[2][0] >> 8);
}

void UFO_Ring_StaticUpdate(void) {}

void UFO_Ring_Draw(void)
{
    RSDK_THIS(UFO_Ring);

    if (self->depth3D >= 0x100) {
        self->direction = self->animator.frameID > 8;
        Vector2 drawPos;
        drawPos.x = (ScreenInfo->centerX + (self->worldX << 8) / self->depth3D) << 16;
        drawPos.y = (ScreenInfo->centerY - (self->worldY << 8) / self->depth3D) << 16;

        self->scale.x = 0x1000000 / self->depth3D;
        self->scale.y = 0x1000000 / self->depth3D;
        if (self->state == UFO_Ring_State_NormalRing)
            self->animator.frameID = UFO_Setup->ringFrame;
        RSDK.DrawSprite(&self->animator, &drawPos, true);
    }
}

void UFO_Ring_Create(void *data)
{
    RSDK_THIS(UFO_Ring);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_FLIP | FX_SCALE;
        self->drawOrder     = 4;
        self->active        = ACTIVE_RBOUNDS;
        self->updateRange.x = 0x400;
        self->updateRange.y = 0x400;
        if (!self->height)
            self->height = 12;
        self->height <<= 16;
        self->state = UFO_Ring_State_NormalRing;
        RSDK.SetSpriteAnimation(UFO_Ring->aniFrames, 1, &self->animator, true, 0);
    }
}

void UFO_Ring_StageLoad(void)
{
    UFO_Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);
    UFO_Ring->sfxRing   = RSDK.GetSfx("Global/Ring.wav");
}

void UFO_Ring_PlayRingSFX(void)
{
    if (UFO_Setup->rings < 999) {
        UFO_Setup->rings++;
    }

    if (UFO_Setup->ringPan) {
        int32 channel = RSDK.PlaySfx(UFO_Ring->sfxRing, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        UFO_Setup->ringPan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(UFO_Ring->sfxRing, 0, 255);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        UFO_Setup->ringPan = 1;
    }
}

void UFO_Ring_LoseRings(EntityUFO_Player *player)
{
    UFO_Setup->rings -= 10;
    int32 ringCount = 10;
    if (UFO_Setup->rings < 0) {
        ringCount        = UFO_Setup->rings + 10;
        UFO_Setup->rings = 0;
    }

    for (; ringCount >= 0; --ringCount) {
        EntityUFO_Ring *ring          = CREATE_ENTITY(UFO_Ring, NULL, player->position.x, player->position.y);
        ring->height                  = player->height + 0x80000;
        ring->velocity.x              = player->velocity.x + RSDK.Rand(-0x40000, 0x40000);
        ring->velocity.y              = player->velocity.y + RSDK.Rand(-0x40000, 0x40000);
        ring->bounceVelocity          = RSDK.Rand(0x40000, 0x60000);
        ring->inkEffect               = INK_ALPHA;
        ring->alpha                   = 0x200;
        ring->animator.speed = 0x100;
        ring->state                   = UFO_Ring_State_LoseRing;
    }
}

void UFO_Ring_State_NormalRing(void)
{
    RSDK_THIS(UFO_Ring);
    foreach_active(UFO_Player, player)
    {
        int32 rx     = (self->height - player->height - 0xA0000) >> 16;
        int32 ry     = (self->position.y - player->position.y) >> 16;
        int32 radius = ry * ry + rx * rx;

        int32 pr = UFO_Player->maxSpeed >> 9;
        if (((self->position.x - player->position.x) >> 16) * ((self->position.x - player->position.x) >> 16) + radius < pr) {
            RSDK.SetSpriteAnimation(UFO_Ring->aniFrames, 2, &self->animator, true, 4);
            ++self->drawOrder;
            self->state = UFO_Ring_State_Sparkle;
            UFO_Ring_PlayRingSFX();
        }
    }
}

void UFO_Ring_State_Sparkle(void)
{
    RSDK_THIS(UFO_Ring);
    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == self->animator.frameCount - 1)
        destroyEntity(self);
}

void UFO_Ring_State_LoseRing(void)
{
    RSDK_THIS(UFO_Ring);
    RSDK.ProcessAnimation(&self->animator);
    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->bounceVelocity -= 0x3800;

    self->height += self->bounceVelocity;
    if (self->height < 0xC0000 && self->bounceVelocity < 0) {
        self->height         = 0xC0000;
        self->bounceVelocity = -(self->bounceVelocity >> 1);
    }

    if (self->animator.speed > 0x40)
        self->animator.speed--;

    self->alpha -= 4;
    if (!self->alpha)
        destroyEntity(self);
}

#if RETRO_INCLUDE_EDITOR
void UFO_Ring_EditorDraw(void)
{
    RSDK_THIS(UFO_Ring);
    RSDK.SetSpriteAnimation(UFO_Ring->aniFrames, 0, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void UFO_Ring_EditorLoad(void) { UFO_Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE); }
#endif

void UFO_Ring_Serialize(void) { RSDK_EDITABLE_VAR(UFO_Ring, VAR_ENUM, height); }
