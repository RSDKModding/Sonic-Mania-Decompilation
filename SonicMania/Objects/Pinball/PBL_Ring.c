// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PBL_Ring Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectPBL_Ring *PBL_Ring;

void PBL_Ring_Update(void)
{
    RSDK_THIS(PBL_Ring);

    StateMachine_Run(self->state);
}

void PBL_Ring_LateUpdate(void)
{
    RSDK_THIS(PBL_Ring);

    int32 x = self->position.x >> 8;
    int32 y = self->height >> 8;
    int32 z = self->position.y >> 8;

    Matrix *m = &PBL_Camera->matWorld;

    self->worldX = m->values[0][3] + (y * m->values[0][1] >> 8) + (z * m->values[0][2] >> 8) + (x * m->values[0][0] >> 8);
    self->worldY = m->values[1][3] + (y * m->values[1][1] >> 8) + (z * m->values[1][2] >> 8) + (x * m->values[1][0] >> 8);
    self->zdepth = m->values[2][3] + (y * m->values[2][1] >> 8) + (z * m->values[2][2] >> 8) + (x * m->values[2][0] >> 8);
}

void PBL_Ring_StaticUpdate(void) {}

void PBL_Ring_Draw(void)
{
    RSDK_THIS(PBL_Ring);

    if (self->zdepth >= 0x100) {
        self->direction = self->animator.frameID > 8;

        Vector2 drawPos;
        drawPos.x = (ScreenInfo->center.x + (self->worldX << 8) / self->zdepth) << 16;
        drawPos.y = (ScreenInfo->center.y - (self->worldY << 8) / self->zdepth) << 16;

        self->scale.x = 0x1000000 / self->zdepth;
        self->scale.y = 0x1000000 / self->zdepth;

        if (self->state == PBL_Ring_State_Ring)
            self->animator.frameID = PBL_Setup->ringFrame;

        RSDK.DrawSprite(&self->animator, &drawPos, true);
    }
}

void PBL_Ring_Create(void *data)
{
    RSDK_THIS(PBL_Ring);

    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->drawFX        = FX_SCALE | FX_FLIP;
        self->drawGroup     = 4;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;

        if (!self->height)
            self->height = 12;
        self->height <<= 16;

        self->state = PBL_Ring_State_Ring;
        RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 1, &self->animator, true, 0);
    }
}

void PBL_Ring_StageLoad(void)
{
    PBL_Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE);

    PBL_Ring->sfxRing = RSDK.GetSfx("Global/Ring.wav");
}

void PBL_Ring_GiveRing(void)
{
    if (PBL_Setup->rings < 999)
        PBL_Setup->rings++;

    PBL_Setup_GiveScore(10);

    if (PBL_Setup->ringPan) {
        int32 channel = RSDK.PlaySfx(PBL_Ring->sfxRing, false, 0xFF);
        RSDK.SetChannelAttributes(channel, 1.0, -1.0, 1.0);
        PBL_Setup->ringPan = 0;
    }
    else {
        int32 channel = RSDK.PlaySfx(PBL_Ring->sfxRing, false, 0xFF);
        RSDK.SetChannelAttributes(channel, 1.0, 1.0, 1.0);
        PBL_Setup->ringPan = 1;
    }
}

void PBL_Ring_State_Ring(void)
{
    RSDK_THIS(PBL_Ring);

    foreach_active(PBL_Player, player)
    {
        int32 ry = (self->height - player->height - 0xA0000) >> 16;
        int32 rz = (self->position.y - player->position.y) >> 16;
        int32 rx = (self->position.x - player->position.x) >> 16;

        if (rx * rx + rz * rz + ry * ry < 0x100) {
            RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 2, &self->animator, true, 4);
            ++self->drawGroup;

            self->state = PBL_Ring_State_RingSparkle;
            PBL_Ring_GiveRing();
        }
    }
}

void PBL_Ring_State_RingSparkle(void)
{
    RSDK_THIS(PBL_Ring);

    RSDK.ProcessAnimation(&self->animator);

    if (self->animator.frameID == 15) {
        self->state  = StateMachine_None;
        self->active = ACTIVE_NEVER;
    }
}

#if GAME_INCLUDE_EDITOR
void PBL_Ring_EditorDraw(void)
{
    RSDK_THIS(PBL_Ring);

    RSDK.SetSpriteAnimation(PBL_Ring->aniFrames, 0, &self->animator, true, 0);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PBL_Ring_EditorLoad(void) { PBL_Ring->aniFrames = RSDK.LoadSpriteAnimation("Global/Ring.bin", SCOPE_STAGE); }
#endif

void PBL_Ring_Serialize(void) { RSDK_EDITABLE_VAR(PBL_Ring, VAR_ENUM, height); }
#endif
