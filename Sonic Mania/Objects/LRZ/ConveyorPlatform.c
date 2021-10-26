#include "SonicMania.h"

ObjectConveyorPlatform *ConveyorPlatform;

void ConveyorPlatform_Update(void)
{
    RSDK_THIS(ConveyorPlatform);
    if (entity->state == Platform_Unknown4) {
        if (entity->collapseDelay || entity->timer) {
            if (++entity->timer == 24) {
                entity->stateCollide = Platform_CollisionState_AllSolid;
                entity->collision    = PLATFORM_C_1;
                entity->timer        = 0;
            }
            else {
                entity->stateCollide = Platform_CollisionState_Null;
                entity->collision    = PLATFORM_C_4;
            }
            entity->animator.frameID = ConveyorPlatform->frameIDs[entity->timer];
            entity->direction        = ConveyorPlatform->directionIDs[entity->timer];
        }
    }
    else {
        if (entity->timer) {
            entity->timer++;
            if (entity->timer >= entity->flipCount) {
                entity->stateCollide = Platform_CollisionState_AllSolid;
                entity->collision    = PLATFORM_C_1;
                entity->timer        = 0;
            }
            entity->animator.frameID = ConveyorPlatform->frameIDs[entity->timer % 24];
            entity->direction        = ConveyorPlatform->directionIDs[entity->timer % 24];
        }
        if (!((Zone->timer + entity->intervalOffset) % entity->interval) && !entity->timer) {
            entity->stateCollide = Platform_CollisionState_Null;
            entity->collision    = PLATFORM_C_4;
            entity->timer        = 1;
        }
    }
    Platform_Update();
}

void ConveyorPlatform_LateUpdate(void) {}

void ConveyorPlatform_StaticUpdate(void) {}

void ConveyorPlatform_Draw(void)
{
    RSDK_THIS(ConveyorPlatform);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void ConveyorPlatform_Create(void *data)
{
    RSDK_THIS(ConveyorPlatform);
    if (entity->type)
        entity->type = PLATFORM_5;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->spriteIndex, 2, &entity->animator, true, 0);
    entity->drawFX |= FX_FLIP;
    entity->stateCollide = Platform_CollisionState_AllSolid;
    entity->collision    = PLATFORM_C_1;
    entity->timer        = 0;
    if (!RSDK_sceneInfo->inEditor)
        entity->flipCount *= 12;
}

void ConveyorPlatform_StageLoad(void) {}

void ConveyorPlatform_EditorDraw(void) {}

void ConveyorPlatform_EditorLoad(void) {}

void ConveyorPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_ENUM, node);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(ConveyorPlatform, VAR_UINT16, flipCount);
}
