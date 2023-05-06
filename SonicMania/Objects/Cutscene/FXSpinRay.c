// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FXSpinRay Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFXSpinRay *FXSpinRay;

void FXSpinRay_Update(void)
{
    RSDK_THIS(FXSpinRay);

    self->angle = ((self->angle & 0xFF) + self->spinSpeed) & 0xFF;
    int32 angle = self->angle;

    for (int32 i = 0; i < 20; i += 4) {
        for (int32 v = 0; v < 4; ++v) {
            self->vertices[i + v] = self->vertexPos[v];
            Zone_RotateOnPivot(&self->vertices[i + v] , &self->vertexOffset, angle);
        }

        angle += 51;
    }

    StateMachine_Run(self->state);

    Entity *parent = self->parent;
    if (parent) {
        self->position.x = parent->position.x + self->offsetPos.x;
        self->position.y = parent->position.y + self->offsetPos.y;
    }
}

void FXSpinRay_LateUpdate(void) {}

void FXSpinRay_StaticUpdate(void) {}

void FXSpinRay_Draw(void)
{
    RSDK_THIS(FXSpinRay);

    int32 scrX = self->position.x - (ScreenInfo->position.x << 16);
    int32 scrY = self->position.y - (ScreenInfo->position.y << 16);

    for (int32 i = 0; i < 20; i += 4) {
        Vector2 vertices[4];

        vertices[0].x = scrX + self->vertices[i + 0].x;
        vertices[0].y = scrY + self->vertices[i + 0].y;
        vertices[1].x = scrX + self->vertices[i + 1].x;
        vertices[1].y = scrY + self->vertices[i + 1].y;
        vertices[2].x = scrX + self->vertices[i + 2].x;
        vertices[2].y = scrY + self->vertices[i + 2].y;
        vertices[3].x = scrX + self->vertices[i + 3].x;
        vertices[3].y = scrY + self->vertices[i + 3].y;

        RSDK.DrawFace(vertices, 4, self->r, self->g, self->b, self->alpha, INK_ADD);
    }
}

void FXSpinRay_Create(void *data)
{
    RSDK_THIS(FXSpinRay);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->active    = ACTIVE_NORMAL;
        self->drawGroup = Zone->objectDrawGroup[1];
        self->size      = 0x600000;

        self->vertexPos[0].x = -0xC0000;
        self->vertexPos[1].x = self->size >> 3;
        self->vertexPos[0].y = -self->size;
        self->vertexPos[1].y = -self->size;
        self->vertexPos[2].x = self->size >> 6;
        self->vertexPos[3].x = -self->size >> 6;
        self->vertexPos[2].y = -self->size >> 4;
        self->vertexPos[3].y = -self->size >> 4;

        self->spinSpeed = 8;
        self->state     = FXSpinRay_State_FadeIn;

        self->r = 0xF0;
        self->g = 0xF0;
        self->b = 0xF0;
    }
}

void FXSpinRay_StageLoad(void) {}

void FXSpinRay_State_FadeIn(void)
{
    RSDK_THIS(FXSpinRay);

    if (self->alpha >= 0x80)
        self->state = FXSpinRay_State_Spinning;
    else
        self->alpha += 0x10;
}

void FXSpinRay_State_Spinning(void)
{
    RSDK_THIS(FXSpinRay);

    if (++self->timer == 16)
        self->state = FXSpinRay_State_FadeOut;
}

void FXSpinRay_State_FadeOut(void)
{
    RSDK_THIS(FXSpinRay);

    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 0x10;
}

#if GAME_INCLUDE_EDITOR
void FXSpinRay_EditorDraw(void) {}

void FXSpinRay_EditorLoad(void) {}
#endif

void FXSpinRay_Serialize(void) {}
