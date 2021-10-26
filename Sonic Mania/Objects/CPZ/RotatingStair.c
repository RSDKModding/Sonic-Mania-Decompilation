#include "SonicMania.h"

ObjectRotatingStair *RotatingStair;

void RotatingStair_Update(void) { Platform_Update(); }

void RotatingStair_LateUpdate(void) {}

void RotatingStair_StaticUpdate(void) {}

void RotatingStair_Draw(void)
{
    RSDK_THIS(RotatingStair);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void RotatingStair_Create(void *data)
{
    RSDK_THIS(RotatingStair);
    entity->frameID   = 2;
    entity->collision = PLATFORM_C_1;
    entity->speed     = 3;
    if (entity->direction)
        entity->amplitude.x = -entity->amplitude.x;
    if (entity->type & 1)
        entity->amplitude.x = -entity->amplitude.x;

    int32 typeStore = entity->type;
    entity->type    = PLATFORM_2;
    Platform_Create(NULL);
    entity->type = typeStore;

    if (entity->type >= 4) {
        entity->duration = 120 * entity->speed;
        entity->state    = RotatingStair_Unknown2;
        entity->interval = entity->duration + 512;
    }
    else {
        entity->state = RotatingStair_Unknown1;
    }
}

void RotatingStair_StageLoad(void) { RotatingStair->flag = 0; }

void RotatingStair_Unknown1(void)
{
    RSDK_THIS(RotatingStair);
    int32 timer = Zone->timer + entity->oscOff;
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;

    int32 dir = 0;
    if (entity->type & 1)
        dir = entity->type - (((3 * timer) >> 9) & 3) - 2;
    else
        dir = ((3 * timer) >> 9) + entity->type;
    switch (dir & 3) {
        case 0:
            entity->drawPos.x = entity->amplitude.x * RSDK.Cos1024(timer * entity->speed) + entity->centerPos.x;
            entity->drawPos.y = entity->centerPos.y + (entity->amplitude.y << 10);
            break;
        case 1:
            entity->drawPos.x = entity->centerPos.x - (entity->amplitude.x << 10);
            entity->drawPos.y = entity->centerPos.y - entity->amplitude.y * RSDK.Cos1024(timer * entity->speed);
            break;
        case 2:
            entity->drawPos.x = entity->centerPos.x - entity->amplitude.x * RSDK.Cos1024(timer * entity->speed);
            entity->drawPos.y = entity->centerPos.y - (entity->amplitude.y << 10);
            break;
        case 3:
            entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 10);
            entity->drawPos.y = entity->amplitude.y * RSDK.Cos1024(timer * entity->speed) + entity->centerPos.y;
            break;
    }
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void RotatingStair_Unknown2(void)
{
    RSDK_THIS(RotatingStair);
    int32 drawX = -entity->drawPos.x;
    int32 drawY = -entity->drawPos.y;

    int32 timer = 0;
    if (entity->speed * (Zone->timer + entity->oscOff) % entity->interval >= entity->duration)
        timer = entity->speed * (Zone->timer + entity->oscOff) % entity->interval - entity->duration;

    int32 dir = 0;
    if (entity->type & 1)
        dir = entity->type - ((entity->speed * (Zone->timer + entity->oscOff) / entity->interval) & 3) - 2;
    else
        dir = entity->speed * (Zone->timer + entity->oscOff) / entity->interval + entity->type;
    switch (dir) {
        case 0:
            entity->drawPos.x = entity->amplitude.x * RSDK.Cos1024(timer) + entity->centerPos.x;
            entity->drawPos.y = entity->centerPos.y + (entity->amplitude.y << 10);
            break;
        case 1:
            entity->drawPos.x = entity->centerPos.x - (entity->amplitude.x << 10);
            entity->drawPos.y = entity->centerPos.y - entity->amplitude.y * RSDK.Cos1024(timer + 512);
            break;
        case 2:
            entity->drawPos.x = entity->centerPos.x - entity->amplitude.x * RSDK.Cos1024(timer);
            entity->drawPos.y = entity->centerPos.y - (entity->amplitude.y << 10);
            break;
        case 3:
            entity->drawPos.x = entity->centerPos.x + (entity->amplitude.x << 10);
            entity->drawPos.y = entity->amplitude.y * RSDK.Cos1024(timer + 512) + entity->centerPos.y;
            break;
    }
    entity->velocity.x = drawX + entity->drawPos.x;
    entity->velocity.y = drawY + entity->drawPos.y;
}

void RotatingStair_EditorDraw(void) {}

void RotatingStair_EditorLoad(void) {}

void RotatingStair_Serialize(void)
{
    // manual override, it's usually "type"
    RSDK.SetEditableVar(VAR_ENUM, "mode", RotatingStair->objectID, offsetof(EntityRotatingStair, type));
    RSDK_EDITABLE_VAR(RotatingStair, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT16, oscOff);
}
