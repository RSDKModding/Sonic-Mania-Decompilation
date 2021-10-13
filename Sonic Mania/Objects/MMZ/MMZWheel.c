#include "SonicMania.h"

ObjectMMZWheel *MMZWheel;

void MMZWheel_Update(void)
{
    RSDK_THIS(MMZWheel);
    RSDK.ProcessAnimation(&entity->animator);
    entity->offset.x = -entity->position.x;
    entity->offset.y = -entity->position.y;

    switch (entity->motionType) {
        default: break;
        case 0: {
            int32 val = 2 * (entity->direction != FLIP_NONE) - 1;
            entity->position.x += val * (entity->speed << 14);
            if (abs(entity->position.x - entity->center.x) >= entity->amplitude << 16) {
                entity->direction  = entity->direction == FLIP_NONE;
                entity->position.x = entity->center.x + (entity->amplitude << 16) * val;
            }
            break;
        }
        case 1: entity->position.x = (entity->amplitude << 6) * RSDK.Sin1024(entity->speed * Zone->timer) + entity->center.x; break;
    }

    RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, CMODE_FLOOR, 0, 0x180000, 8);

    entity->offset.x += entity->position.x;
    entity->offset.y += entity->position.y;
    int32 id = RSDK_sceneInfo->entitySlot;
    for (int32 i = 0; i < entity->childCount; ++i) {
        Entity *child = RSDK.GetEntityByID(++id);
        child->position.x += entity->offset.x;
        child->position.y += entity->offset.y;
    }
}

void MMZWheel_LateUpdate(void) {}

void MMZWheel_StaticUpdate(void) {}

void MMZWheel_Draw(void)
{
    RSDK_THIS(MMZWheel);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void MMZWheel_Create(void *data)
{
    RSDK_THIS(MMZWheel);
    entity->active    = ACTIVE_BOUNDS;
    entity->visible   = 1;
    entity->drawOrder = Zone->drawOrderLow;
    if (RSDK_sceneInfo->inEditor && !entity->speed)
        entity->speed = 1;
    entity->updateRange.y = 0x800000;
    entity->updateRange.x = (entity->amplitude + 0x80) << 16;
    entity->center.x      = entity->position.x;
    entity->center.y      = entity->position.y;
    RSDK.SetSpriteAnimation(MMZWheel->aniFrames, 0, &entity->animator, true, 0);
}

void MMZWheel_StageLoad(void) { MMZWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/MMZWheel.bin", SCOPE_STAGE); }

void MMZWheel_EditorDraw(void) {}

void MMZWheel_EditorLoad(void) {}

void MMZWheel_Serialize(void)
{
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT32, amplitude);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT8, childCount);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(MMZWheel, VAR_UINT8, motionType);
}
