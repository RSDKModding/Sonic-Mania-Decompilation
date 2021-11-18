#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectFXWaveRing *FXWaveRing;

void FXWaveRing_Update(void)
{
    RSDK_THIS(FXWaveRing);
    if (!(Zone->timer & 3)) {
        if (self->field_70 > 0)
            self->field_78 += RSDK.Rand(0, 2 * self->field_70);
        self->field_8C     = RSDK.Rand(8, 24);
        self->angleOffset  = 1;
        self->angle2Offset = RSDK.Rand(8, 32);
        self->field_6C     = RSDK.Rand(50, 199);
    }
    self->angle  = ((self->angle & 0xFF) + (self->angleOffset & 0xFF)) & 0xFf;
    self->angle2 = ((self->angle2 & 0xFF) + (self->angle2Offset & 0xFF)) & 0xFf;

    StateMachine_Run(self->state);

    Entity *parent = self->parent;
    if (parent) {
        self->position.x = parent->position.x + self->field_80.x;
        self->position.y = parent->position.y + self->field_80.y;
    }
}

void FXWaveRing_LateUpdate(void) {}

void FXWaveRing_StaticUpdate(void) {}

void FXWaveRing_Draw(void)
{
    RSDK_THIS(FXWaveRing);

    int32 angInc  = 256 / self->field_88;
    uint32 colour = (self->r << 16) | (self->g << 8) | self->b;
    int32 sin     = self->field_8C * RSDK.Sin256(self->angle2) + (self->field_78 << 8);

    int32 x = sin * RSDK.Sin256(self->angle) + self->position.x;
    int32 y = sin * RSDK.Cos256(self->angle) + self->position.y;

    int32 angle  = self->angle;
    int32 angle2 = self->angle2;
    int32 prevX  = x;
    int32 prevY  = y;
    for (int32 i = 0; i < self->field_88 - 1; ++i) {
        angle += angInc;
        angle2 += self->field_6C;
        sin   = self->field_8C * RSDK.Sin256(angle2) + (self->field_78 << 8);
        prevX = x;
        prevY = y;
        x     = sin * RSDK.Sin256(angle) + self->position.x;
        y     = sin * RSDK.Cos256(angle) + self->position.y;
        RSDK.DrawLine(prevX, prevY, x, y, colour, self->alpha, self->inkEffect, false);

        prevX = x;
        prevY = y;
    }

    sin = self->field_8C * RSDK.Sin256(self->angle2) + (self->field_78 << 8);
    RSDK.DrawLine(x, y, sin * RSDK.Sin256(self->angle) + self->position.x, sin * RSDK.Cos256(self->angle) + self->position.y, colour,
                  self->alpha, self->inkEffect, false);
}

void FXWaveRing_Create(void *data)
{
    RSDK_THIS(FXWaveRing);
    if (!SceneInfo->inEditor) {
        self->visible      = true;
        self->active       = ACTIVE_NORMAL;
        self->drawOrder    = Zone->drawOrderHigh;
        self->field_78     = 48;
        self->field_8C     = 8;
        self->angleOffset  = 2;
        self->angle2Offset = 32;
        self->field_6C     = 150;
        self->field_88     = 16;
        self->inkEffect    = INK_ADD;
        self->parent       = (Entity *)data;
        self->state        = FXWaveRing_State_FadeIn;
        self->r            = 0xF0;
        self->g            = 0xF0;
        self->b            = 0xF0;
    }
}

void FXWaveRing_StageLoad(void) {}
void FXWaveRing_State_FadeIn(void)
{
    RSDK_THIS(FXWaveRing);

    if (self->alpha >= 192) {
        self->field_78 -= self->field_70;
        self->state = FXWaveRing_State_Wait;
    }
    else {
        self->alpha += 8;
        self->field_78 -= self->field_70;
    }
}
void FXWaveRing_State_Wait(void)
{
    RSDK_THIS(FXWaveRing);

    if (self->timer > 0) {
        self->timer--;
        if (!self->timer)
            self->state = FXWaveRing_State_FadeOut;
    }
    self->field_78 -= self->field_70;
}
void FXWaveRing_State_FadeOut(void)
{
    RSDK_THIS(FXWaveRing);

    self->field_78 -= self->field_70;
    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 16;
}

#if RETRO_INCLUDE_EDITOR
void FXWaveRing_EditorDraw(void) {}

void FXWaveRing_EditorLoad(void) {}
#endif

void FXWaveRing_Serialize(void) {}
#endif
