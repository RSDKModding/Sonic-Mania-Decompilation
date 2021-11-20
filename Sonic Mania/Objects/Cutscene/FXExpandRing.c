#include "SonicMania.h"

ObjectFXExpandRing *FXExpandRing;

void FXExpandRing_Update(void)
{
    RSDK_THIS(FXExpandRing);
    StateMachine_Run(self->state);

    Entity *parent = self->parent;
    if (parent) {
        if (self->drawFX == FX_FLIP && parent->direction) {
            self->position.x = parent->position.x - self->posOffset.x;
            self->position.y = parent->position.y + self->posOffset.y;
        }
        else {
            self->position.x = parent->position.x + self->posOffset.x;
            self->position.y = parent->position.y + self->posOffset.y;
        }
    }
}

void FXExpandRing_LateUpdate(void) {}

void FXExpandRing_StaticUpdate(void) {}

void FXExpandRing_Draw(void)
{
    RSDK_THIS(FXExpandRing);
    RSDK.DrawCircleOutline(self->position.x, self->position.y, self->innerRadius, self->outerRadius, self->colour, self->alpha, INK_ADD,
                           false);
}

void FXExpandRing_Create(void *data)
{
    RSDK_THIS(FXExpandRing);
    if (!SceneInfo->inEditor) {
        self->visible     = true;
        self->active      = ACTIVE_NORMAL;
        self->drawOrder   = Zone->drawOrderHigh;
        self->expandMax   = 64;
        self->expandSpeed = 8;
        self->state       = FXExpandRing_State_FadeIn;
        self->colour      = 0xF0F0F0;
    }
}

void FXExpandRing_StageLoad(void) {}

void FXExpandRing_State_FadeIn(void)
{
    RSDK_THIS(FXExpandRing);

    if (self->alpha < 192)
        self->alpha += 16;
    self->outerRadius += self->expandSpeed;
    if (self->outerRadius > self->expandMax - (self->expandMax >> 2))
        self->state = FXExpandRing_State_Expand;
}
void FXExpandRing_State_Expand(void)
{
    RSDK_THIS(FXExpandRing);

    if (self->alpha < 192)
        self->alpha += 16;

    if (self->outerRadius < self->expandMax)
        self->outerRadius += self->expandSpeed;
    if (self->innerRadius >= self->expandMax - (self->expandMax >> 3))
        self->state = FXExpandRing_State_FadeOut;
    else
        self->innerRadius += (self->expandSpeed >> 1);
}
void FXExpandRing_State_FadeOut(void)
{
    RSDK_THIS(FXExpandRing);
    if (self->alpha <= 0)
        destroyEntity(self);
    else
        self->alpha -= 16;
}

#if RETRO_INCLUDE_EDITOR
void FXExpandRing_EditorDraw(void) {}

void FXExpandRing_EditorLoad(void) {}
#endif

void FXExpandRing_Serialize(void) {}
