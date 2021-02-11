#include "../SonicMania.hpp"

ObjectRing *Ring;

void Ring_Update()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void Ring_LateUpdate()
{

}

void Ring_StaticUpdate()
{

}

void Ring_Draw()
{
    EntityRing *entity = (EntityRing *)RSDK_sceneInfo->entity;
    if (entity->stateDraw)
        entity->stateDraw();
}

void Ring_Create(void* data)
{

}

void Ring_StageLoad()
{
    Ring->spriteIndex   = RSDK_LoadAnimation("Global/Ring.bin", SCOPE_STAGE);
    Ring->hitbox.left   = -8;
    Ring->hitbox.top    = -8;
    Ring->hitbox.right  = 8;
    Ring->hitbox.bottom = 8;
    if (DebugMode->itemCount < 0x100) {
        DebugMode->objectIDs[DebugMode->itemCount] = Ring->objectID;
        DebugMode->spawn[DebugMode->itemCount]     = Ring_DebugSpawn;
        DebugMode->draw[DebugMode->itemCount++]    = Ring_DebugDraw;
    }
    Ring->sfx_Ring = RSDK_GetSFX("Global/Ring.wav");
}

void Ring_DebugSpawn() {

}
void Ring_DebugDraw() {

}

void Ring_EditorDraw()
{

}

void Ring_EditorLoad()
{

}

void Ring_Serialize()
{
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "type", Ring->objectID, 96);
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "planeFilter", Ring->objectID, 100);
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "moveType", Ring->objectID, 124);
    RSDK_SetEditableVar(ATTRIBUTE_VECTOR2, "amplitude", Ring->objectID, 128);
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "speed", Ring->objectID, 136);
    RSDK_SetEditableVar(ATTRIBUTE_S32, "angle", Ring->objectID, 32);
}

