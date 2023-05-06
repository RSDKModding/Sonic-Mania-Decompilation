// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FXWaveRing Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectFXWaveRing *FXWaveRing;

void FXWaveRing_Update(void)
{
    RSDK_THIS(FXWaveRing);

    if (!(Zone->timer & 3)) {
        if (self->shrinkSpeed > 0)
            self->radiusOffset += RSDK.Rand(0, 2 * self->shrinkSpeed);

        self->radius    = RSDK.Rand(8, 24);
        self->angleVel  = 1;
        self->angleVel2 = RSDK.Rand(8, 32);
        self->angleInc  = RSDK.Rand(50, 199);
    }

    self->angle  = ((self->angle & 0xFF) + (self->angleVel & 0xFF)) & 0xFf;
    self->angle2 = ((self->angle2 & 0xFF) + (self->angleVel2 & 0xFF)) & 0xFf;

    StateMachine_Run(self->state);

    Entity *parent = self->parent;
    if (parent) {
        self->position.x = parent->position.x + self->offsetPos.x;
        self->position.y = parent->position.y + self->offsetPos.y;
    }
}

void FXWaveRing_LateUpdate(void) {}

void FXWaveRing_StaticUpdate(void) {}

void FXWaveRing_Draw(void)
{
    RSDK_THIS(FXWaveRing);

    int32 angInc = 256 / self->pointCount;
    uint32 color = (self->r << 16) | (self->g << 8) | (self->b << 8);
    int32 sin    = self->radius * RSDK.Sin256(self->angle2) + (self->radiusOffset << 8);

    int32 x = sin * RSDK.Sin256(self->angle) + self->position.x;
    int32 y = sin * RSDK.Cos256(self->angle) + self->position.y;

    int32 angle  = self->angle;
    int32 angle2 = self->angle2;
    int32 prevX  = x;
    int32 prevY  = y;
    for (int32 i = 0; i < self->pointCount - 1; ++i) {
        angle += angInc;
        angle2 += self->angleInc;
        sin   = self->radius * RSDK.Sin256(angle2) + (self->radiusOffset << 8);
        prevX = x;
        prevY = y;
        x     = sin * RSDK.Sin256(angle) + self->position.x;
        y     = sin * RSDK.Cos256(angle) + self->position.y;
        RSDK.DrawLine(prevX, prevY, x, y, color, self->alpha, self->inkEffect, false);

        prevX = x;
        prevY = y;
    }

    sin = self->radius * RSDK.Sin256(self->angle2) + (self->radiusOffset << 8);
    RSDK.DrawLine(x, y, sin * RSDK.Sin256(self->angle) + self->position.x, sin * RSDK.Cos256(self->angle) + self->position.y, color, self->alpha,
                  self->inkEffect, false);
}

void FXWaveRing_Create(void *data)
{
    RSDK_THIS(FXWaveRing);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->active    = ACTIVE_NORMAL;
        self->drawGroup = Zone->objectDrawGroup[1];

        self->radiusOffset = 48;
        self->radius       = 8;
        self->angleVel     = 2;
        self->angleVel2    = 32;
        self->angleInc     = 150;
        self->pointCount   = 16;

        self->inkEffect = INK_ADD;
        self->parent    = (Entity *)data;
        self->state     = FXWaveRing_State_FadeIn;

        self->r = 0xF0;
        self->g = 0xF0;
        self->b = 0xF0;
    }
}

void FXWaveRing_StageLoad(void) {}
void FXWaveRing_State_FadeIn(void)
{
    RSDK_THIS(FXWaveRing);

    if (self->alpha >= 0xC0) {
        self->radiusOffset -= self->shrinkSpeed;
        self->state = FXWaveRing_State_Wait;
    }
    else {
        self->alpha += 0x08;
        self->radiusOffset -= self->shrinkSpeed;
    }
}
void FXWaveRing_State_Wait(void)
{
    RSDK_THIS(FXWaveRing);

    if (self->timer > 0) {
        if (!--self->timer)
            self->state = FXWaveRing_State_FadeOut;
    }

    self->radiusOffset -= self->shrinkSpeed;
}
void FXWaveRing_State_FadeOut(void)
{
    RSDK_THIS(FXWaveRing);

    self->radiusOffset -= self->shrinkSpeed;

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 0x10;
}

#if GAME_INCLUDE_EDITOR
void FXWaveRing_EditorDraw(void) {}

void FXWaveRing_EditorLoad(void) {}
#endif

void FXWaveRing_Serialize(void) {}
#endif
