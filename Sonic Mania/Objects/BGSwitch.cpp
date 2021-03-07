#include "../SonicMania.hpp"

ObjectBGSwitch *BGSwitch;

void BGSwitch_Update()
{
    EntityBGSwitch *entity = (EntityBGSwitch *)RSDK_sceneInfo->entity;
    for (BGSwitch->layerID = 0; BGSwitch->layerID < 4; BGSwitch->layerID++) {
        Entity* camera = RSDK.GetEntityByID(BGSwitch->layerID + SLOT_CAMERA1);
        if (!camera->objectID)
            break;

        if (abs(camera->position.x - entity->position.x) < entity->size.x && abs(camera->position.y - entity->position.y) < entity->size.y) {
            if (entity->bgID < LAYER_COUNT)
                BGSwitch->layerIDs[BGSwitch->layerID] = entity->bgID;
        }

        int id = BGSwitch->layerIDs[BGSwitch->layerID];
        if (id != BGSwitch->layerIDs[BGSwitch->layerID + 4]) {
            BGSwitch->layerIDs[BGSwitch->layerID + 4] = id;
            if (BGSwitch->switchCallback[id])
                BGSwitch->switchCallback[id]();
        }
    }
}

void BGSwitch_LateUpdate()
{

}

void BGSwitch_StaticUpdate()
{

}

void BGSwitch_Draw()
{

}

void BGSwitch_Create(void* data)
{
    EntityBGSwitch *entity = (EntityBGSwitch *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = entity->size.x;
        entity->updateRange.y = entity->size.y;
        entity->active        = ACTIVE_BOUNDS;
    }
}

void BGSwitch_StageLoad()
{
    BGSwitch->layerIDs[4] = -1;
    BGSwitch->layerIDs[5] = -1;
    BGSwitch->layerIDs[6] = -1;
    BGSwitch->layerIDs[7] = -1;
}

void BGSwitch_EditorDraw()
{

}

void BGSwitch_EditorLoad()
{

}

void BGSwitch_Serialize()
{
    RSDK_EDITABLE_VAR(BGSwitch, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(BGSwitch, VAR_UINT8, bgID);
}

