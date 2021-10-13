#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectFXWaveRing *FXWaveRing;

void FXWaveRing_Update(void)
{
    RSDK_THIS(FXWaveRing);
    if (!(Zone->timer & 3)) {
        if (entity->field_70 > 0)
            entity->field_78 += RSDK.Rand(0, 2 * entity->field_70);
        entity->field_8C     = RSDK.Rand(8, 24);
        entity->angleOffset  = 1;
        entity->angle2Offset = RSDK.Rand(8, 32);
        entity->field_6C     = RSDK.Rand(50, 199);
    }
    entity->angle  = ((entity->angle & 0xFF) + (entity->angleOffset & 0xFF)) & 0xFf;
    entity->angle2 = ((entity->angle2 & 0xFF) + (entity->angle2Offset & 0xFF)) & 0xFf;

    StateMachine_Run(entity->state);

    Entity *parent = entity->parent;
    if (parent) {
        entity->position.x = parent->position.x + entity->field_80.x;
        entity->position.y = parent->position.y + entity->field_80.y;
    }
}

void FXWaveRing_LateUpdate(void) {}

void FXWaveRing_StaticUpdate(void) {}

void FXWaveRing_Draw(void)
{
    RSDK_THIS(FXWaveRing);

    int32 angInc  = 256 / entity->field_88;
    uint32 colour = (entity->r << 16) | (entity->g << 8) | entity->b;
    int32 sin     = entity->field_8C * RSDK.Sin256(entity->angle2) + (entity->field_78 << 8);

    int32 x = sin * RSDK.Sin256(entity->angle) + entity->position.x;
    int32 y = sin * RSDK.Cos256(entity->angle) + entity->position.y;

    int32 angle  = entity->angle;
    int32 angle2 = entity->angle2;
    int32 prevX  = x;
    int32 prevY  = y;
    for (int32 i = 0; i < entity->field_88 - 1; ++i) {
        angle += angInc;
        angle2 += entity->field_6C;
        sin   = entity->field_8C * RSDK.Sin256(angle2) + (entity->field_78 << 8);
        prevX = x;
        prevY = y;
        x     = sin * RSDK.Sin256(angle) + entity->position.x;
        y     = sin * RSDK.Cos256(angle) + entity->position.y;
        RSDK.DrawLine(prevX, prevY, x, y, colour, entity->alpha, entity->inkEffect, false);

        prevX = x;
        prevY = y;
    }

    sin = entity->field_8C * RSDK.Sin256(entity->angle2) + (entity->field_78 << 8);
    RSDK.DrawLine(x, y, sin * RSDK.Sin256(entity->angle) + entity->position.x, sin * RSDK.Cos256(entity->angle) + entity->position.y, colour,
                  entity->alpha, entity->inkEffect, false);
}

void FXWaveRing_Create(void *data)
{
    RSDK_THIS(FXWaveRing);
    if (!RSDK_sceneInfo->inEditor) {
        entity->visible      = true;
        entity->active       = ACTIVE_NORMAL;
        entity->drawOrder    = Zone->drawOrderHigh;
        entity->field_78     = 48;
        entity->field_8C     = 8;
        entity->angleOffset  = 2;
        entity->angle2Offset = 32;
        entity->field_6C     = 150;
        entity->field_88     = 16;
        entity->inkEffect    = INK_ADD;
        entity->parent       = (Entity *)data;
        entity->state        = FXWaveRing_State_FadeIn;
        entity->r            = 0xF0;
        entity->g            = 0xF0;
        entity->b            = 0xF0;
    }
}

void FXWaveRing_StageLoad(void) {}
void FXWaveRing_State_FadeIn(void)
{
    RSDK_THIS(FXWaveRing);

    if (entity->alpha >= 192) {
        entity->field_78 -= entity->field_70;
        entity->state = FXWaveRing_State_Wait;
    }
    else {
        entity->alpha += 8;
        entity->field_78 -= entity->field_70;
    }
}
void FXWaveRing_State_Wait(void)
{
    RSDK_THIS(FXWaveRing);

    if (entity->timer > 0) {
        entity->timer--;
        if (!entity->timer)
            entity->state = FXWaveRing_State_FadeOut;
    }
    entity->field_78 -= entity->field_70;
}
void FXWaveRing_State_FadeOut(void)
{
    RSDK_THIS(FXWaveRing);

    entity->field_78 -= entity->field_70;
    if (entity->alpha <= 0)
        destroyEntity(entity);
    else
        entity->alpha -= 16;
}

void FXWaveRing_EditorDraw(void) {}

void FXWaveRing_EditorLoad(void) {}

void FXWaveRing_Serialize(void) {}
#endif
