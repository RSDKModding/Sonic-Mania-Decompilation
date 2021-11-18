#include "SonicMania.h"

ObjectFXSpinRay *FXSpinRay;

void FXSpinRay_Update(void)
{
    RSDK_THIS(FXSpinRay);

    self->angle = ((self->angle & 0xFF) + self->offset) & 0xFF;
    int32 angle     = self->angle;
    for (int32 i = 0; i < 20; i += 4) {
        for (int32 v = 0; v < 4; ++v) {
            int32 x                   = (self->field_80[v].x - self->field_140.x) >> 8;
            int32 y                   = (self->field_80[v].y - self->field_140.y) >> 8;
            self->vertices[i + v].x = (y * RSDK.Sin256(angle)) + (x * RSDK.Cos256(angle)) + self->field_140.x;
            self->vertices[i + v].y = (y * RSDK.Cos256(angle)) - (x * RSDK.Sin256(angle)) + self->field_140.y;
        }
        angle += 51;
    }

    StateMachine_Run(self->state);

    Entity *parent = self->parent;
    if (parent) {
        self->position.x = parent->position.x + self->field_78.x;
        self->position.y = parent->position.y + self->field_78.y;
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
        RSDK.DrawQuad(vertices, 4, self->r, self->g, self->b, self->alpha, INK_ADD);
    }
}

void FXSpinRay_Create(void *data)
{
    RSDK_THIS(FXSpinRay);
    if (!SceneInfo->inEditor) {
        self->visible       = true;
        self->active        = ACTIVE_NORMAL;
        self->drawOrder     = Zone->drawOrderHigh;
        self->field_70      = 0x600000;
        self->field_80[0].x = -0xC0000;
        self->field_80[1].x = self->field_70 >> 3;
        self->field_80[0].y = -self->field_70;
        self->field_80[1].y = -self->field_70;
        self->field_80[2].x = self->field_70 >> 6;
        self->field_80[3].x = -self->field_70 >> 6;
        self->field_80[2].y = -self->field_70 >> 4;
        self->field_80[3].y = -self->field_70 >> 4;
        self->offset        = 8;
        self->state         = FXSpinRay_State_FadeIn;
        self->r             = 0xF0;
        self->g             = 0xF0;
        self->b             = 0xF0;
    }
}

void FXSpinRay_StageLoad(void) {}

void FXSpinRay_State_FadeIn(void)
{
    RSDK_THIS(FXSpinRay);

    if (self->alpha >= 128)
        self->state = FXSpinRay_State_Wait;
    else
        self->alpha += 16;
}

void FXSpinRay_State_Wait(void)
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
        self->alpha -= 16;
}

#if RETRO_INCLUDE_EDITOR
void FXSpinRay_EditorDraw(void) {}

void FXSpinRay_EditorLoad(void) {}
#endif

void FXSpinRay_Serialize(void) {}
