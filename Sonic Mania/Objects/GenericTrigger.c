#include "../SonicMania.h"

ObjectGenericTrigger *GenericTrigger;

void GenericTrigger_Update(void)
{
    EntityPlayer *player = NULL;
    EntityGenericTrigger *entity = (EntityGenericTrigger *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && !player->sidekick) {
            GenericTrigger->playerID = player->playerID;
            if (GenericTrigger->callbacks[entity->triggerID])
                GenericTrigger->callbacks[entity->triggerID]();
        }
    }
}

void GenericTrigger_LateUpdate(void)
{

}

void GenericTrigger_StaticUpdate(void)
{

}

void GenericTrigger_Draw(void)
{

}

void GenericTrigger_Create(void* data)
{
    EntityGenericTrigger *entity = (EntityGenericTrigger *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->triggerID &= 0xF;
        entity->updateRange.x = entity->size.x;
        entity->updateRange.y = entity->size.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->hitbox.left   = -(entity->size.x >> 0x10);
        entity->hitbox.right  = (entity->size.x >> 0x10);
        entity->hitbox.top    = -(entity->size.y >> 0x10);
        entity->hitbox.bottom = (entity->size.y >> 0x10);
    }
}

void GenericTrigger_StageLoad(void)
{

}

void GenericTrigger_EditorDraw(void)
{

}

void GenericTrigger_EditorLoad(void)
{

}

void GenericTrigger_Serialize(void)
{
    RSDK_EDITABLE_VAR(GenericTrigger, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(GenericTrigger, VAR_UINT8, triggerID);
}

