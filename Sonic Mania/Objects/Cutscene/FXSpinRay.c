#include "SonicMania.h"

ObjectFXSpinRay *FXSpinRay;

void FXSpinRay_Update(void)
{
    RSDK_THIS(FXSpinRay);

    entity->angle = ((entity->angle & 0xFF) + entity->offset) & 0xFF;
    int32 angle     = entity->angle;
    for (int32 i = 0; i < 20; i += 4) {
        for (int32 v = 0; v < 4; ++v) {
            entity->vertices[i + v].x = entity->field_80[v].x;
            entity->vertices[i + v].y = entity->field_80[v].y;
            int32 x                     = (entity->vertices[i + v].x - entity->field_140.x) >> 8;
            int32 y                     = (entity->vertices[i + v].y - entity->field_140.y) >> 8;
            entity->vertices[i + v].x = (y * RSDK.Sin256(angle)) + (x * RSDK.Cos256(angle)) + entity->field_140.x;
            entity->vertices[i + v].y = (y * RSDK.Cos256(angle)) - (x * RSDK.Sin256(angle)) + entity->field_140.y;
        }
        angle += 51;
    }

    StateMachine_Run(entity->state);

    Entity *parent = entity->parent;
    if (parent) {
        entity->position.x = parent->position.x + entity->field_78.x;
        entity->position.y = parent->position.y + entity->field_78.y;
    }
}

void FXSpinRay_LateUpdate(void) {}

void FXSpinRay_StaticUpdate(void) {}

void FXSpinRay_Draw(void)
{
    RSDK_THIS(FXSpinRay);

    int32 scrX = RSDK_sceneInfo->entity->position.x - (RSDK_screens->position.x << 16);
    int32 scrY = RSDK_sceneInfo->entity->position.y - (RSDK_screens->position.y << 16);
    for (int32 i = 0; i < 20; i += 4) {
        Vector2 vertices[4];
        vertices[0].x = scrX + entity->vertices[0].x;
        vertices[0].y = scrY + entity->vertices[0].y;
        vertices[1].x = scrX + entity->vertices[1].x;
        vertices[1].y = scrY + entity->vertices[1].y;
        vertices[2].x = scrX + entity->vertices[2].x;
        vertices[2].y = scrY + entity->vertices[2].y;
        vertices[3].x = scrX + entity->vertices[3].x;
        vertices[3].y = scrY + entity->vertices[3].y;
        RSDK.DrawQuad(vertices, 4, entity->r, entity->g, entity->b, entity->alpha, INK_ADD);
    }
}

void FXSpinRay_Create(void *data)
{
    RSDK_THIS(FXSpinRay);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible       = 1;
        entity->active        = ACTIVE_NORMAL;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->field_70      = 0x600000;
        entity->field_80[0].x = -0xC0000;
        entity->field_80[1].x = entity->field_70 >> 3;
        entity->field_80[0].y = -entity->field_70;
        entity->field_80[1].y = -entity->field_70;
        entity->field_80[2].x = entity->field_70 >> 6;
        entity->field_80[3].x = -entity->field_70 >> 6;
        entity->field_80[2].y = -entity->field_70 >> 4;
        entity->field_80[3].y = -entity->field_70 >> 4;
        entity->offset        = 8;
        entity->state         = FXSpinRay_State_FadeIn;
        entity->r             = 0xF0;
        entity->g             = 0xF0;
        entity->b             = 0xF0;
    }
}

void FXSpinRay_StageLoad(void) {}

void FXSpinRay_State_FadeIn(void)
{
    RSDK_THIS(FXSpinRay);

    if (entity->alpha >= 128)
        entity->state = FXSpinRay_State_Wait;
    else
        entity->alpha += 16;
}

void FXSpinRay_State_Wait(void)
{
    RSDK_THIS(FXSpinRay);
    if (++entity->timer == 16)
        entity->state = FXSpinRay_State_FadeOut;
}

void FXSpinRay_State_FadeOut(void)
{
    RSDK_THIS(FXSpinRay);

    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 16;
}

void FXSpinRay_EditorDraw(void) {}

void FXSpinRay_EditorLoad(void) {}

void FXSpinRay_Serialize(void) {}
