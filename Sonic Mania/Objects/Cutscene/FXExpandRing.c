#include "SonicMania.h"

ObjectFXExpandRing *FXExpandRing;

void FXExpandRing_Update(void)
{
    RSDK_THIS(FXExpandRing);
    StateMachine_Run(entity->state);

    Entity *parent = entity->parent;
    if (parent) {
        if (entity->drawFX == FX_FLIP && parent->direction) {
            entity->position.x = parent->position.x - entity->posOffset.x;
            entity->position.y = parent->position.y + entity->posOffset.y;
        }
        else {
            entity->position.x = parent->position.x + entity->posOffset.x;
            entity->position.y = parent->position.y + entity->posOffset.y;
        }
    }
}

void FXExpandRing_LateUpdate(void) {}

void FXExpandRing_StaticUpdate(void) {}

void FXExpandRing_Draw(void)
{
    RSDK_THIS(FXExpandRing);
    RSDK.DrawCircleOutline(entity->position.x, entity->position.y, entity->innerRadius, entity->outerRadius, entity->colour, entity->alpha, INK_ADD,
                           false);
}

void FXExpandRing_Create(void *data)
{
    RSDK_THIS(FXExpandRing);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible     = true;
        entity->active      = ACTIVE_NORMAL;
        entity->drawOrder   = Zone->drawOrderHigh;
        entity->expandMax   = 64;
        entity->expandSpeed = 8;
        entity->state       = FXExpandRing_State_FadeIn;
        entity->colour      = 0xF0F0F0;
    }
}

void FXExpandRing_StageLoad(void) {}

void FXExpandRing_State_FadeIn(void)
{
    RSDK_THIS(FXExpandRing);

    if (entity->alpha < 192)
        entity->alpha += 16;
    entity->outerRadius += entity->expandSpeed;
    if (entity->outerRadius > entity->expandMax - (entity->expandMax >> 2))
        entity->state = FXExpandRing_State_Expand;
}
void FXExpandRing_State_Expand(void)
{
    RSDK_THIS(FXExpandRing);

    if (entity->alpha < 192)
        entity->alpha += 16;

    if (entity->outerRadius < entity->expandMax)
        entity->outerRadius += entity->expandSpeed;
    if (entity->innerRadius >= entity->expandMax - (entity->expandMax >> 3))
        entity->state = FXExpandRing_State_FadeOut;
    else
        entity->innerRadius += (entity->expandSpeed >> 1);
}
void FXExpandRing_State_FadeOut(void)
{
    RSDK_THIS(FXExpandRing);
    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 16;
}

void FXExpandRing_EditorDraw(void) {}

void FXExpandRing_EditorLoad(void) {}

void FXExpandRing_Serialize(void) {}
