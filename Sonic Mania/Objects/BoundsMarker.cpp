#include "../SonicMania.hpp"

ObjectBoundsMarker *BoundsMarker;

void BoundsMarker_Update()
{
    EntityBoundsMarker *entity = (EntityBoundsMarker *)RSDK_sceneInfo->entity;
    for (int p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(p);
        BoundsMarker_CheckBounds(player, entity, true);
    }
}

void BoundsMarker_LateUpdate()
{

}

void BoundsMarker_StaticUpdate()
{

}

void BoundsMarker_Draw()
{

}

void BoundsMarker_Create(void* data)
{
    EntityBoundsMarker *entity = (EntityBoundsMarker *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        if (entity->vsDisable && options->gameMode == MODE_COMPETITION) {
            RSDK.ResetEntityPtr(entity, 0, 0);
        }
        else {
            entity->active = ACTIVE_XBOUNDS;
            if (entity->width)
                entity->width = entity->width << 15;
            else
                entity->width = 0x180000;
            entity->updateRange.x += entity->width << 15;

            for (int p = 0; p < Player->playerCount; ++p) {
                EntityPlayer *player = (EntityPlayer *)RSDK.GetEntityByID(p);
                BoundsMarker_CheckBounds(player, entity, true);
            }
        }
    }
}

void BoundsMarker_StageLoad()
{

}

void BoundsMarker_CheckBounds(Entity *p, EntityBoundsMarker *entity, bool32 setPos)
{
    EntityPlayer *player = (EntityPlayer*)p;
    ushort playerID = RSDK.GetEntityID(player);
    if (Player_CheckValidState(player) || player->objectID == DebugMode->objectID) {
        int w = 0;
        if (entity->position.x - player->position.x >= 0)
            w = entity->position.x - player->position.x;
        else
            w = player->position.x - entity->position.x;
        if (w < entity->width) {
            switch (entity->type) {
                case 0:
                    Zone->screenBoundsB2[playerID] = entity->position.y;
                    Zone->screenBoundsB1[playerID] = Zone->screenBoundsB2[playerID] >> 0x10;
                    Zone->screenUnknownA[playerID] = entity->position.y;
                    break;
                case 1:
                    if (player->position.y < entity->position.y - (entity->offset << 16)) {
                        Zone->screenBoundsB2[playerID] = entity->position.y;
                        Zone->screenBoundsB1[playerID] = Zone->screenBoundsB2[playerID] >> 0x10;
                        Zone->screenUnknownA[playerID] = entity->position.y;
                    }
                    break;
                case 2:
                    if (player->position.y > entity->position.y + (entity->offset << 16)) {
                        Zone->screenBoundsT2[playerID] = entity->position.y;
                        Zone->screenBoundsT1[playerID] = Zone->screenBoundsT2[playerID] >> 0x10;
                    }
                    break;
                case 3:
                    Zone->screenBoundsT2[playerID] = entity->position.y;
                    Zone->screenBoundsT1[playerID] = Zone->screenBoundsT2[playerID] >> 0x10;
                    break;
                default: break;
            }
        }
        if (setPos) {
            EntityCamera *camera = player->camera;
            if (camera) {
                camera->boundsL = Zone->screenBoundsL1[playerID];
                camera->boundsR = Zone->screenBoundsR1[playerID];
                camera->boundsT = Zone->screenBoundsT1[playerID];
                camera->boundsB = Zone->screenBoundsB1[playerID];
            }
        }
    }
}
void BoundsMarker_CheckAllBounds(Entity *p, bool32 setPos)
{
    EntityPlayer *player       = (EntityPlayer *)p;
    EntityBoundsMarker *entity = NULL;
    if (Player_CheckValidState(player) || player->objectID == DebugMode->objectID) {
        while (RSDK.GetEntities(BoundsMarker->objectID, (Entity **)&entity)) {
            BoundsMarker_CheckBounds(player, entity, setPos);
        }
    }
}

void BoundsMarker_EditorDraw()
{

}

void BoundsMarker_EditorLoad()
{

}

void BoundsMarker_Serialize()
{
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_ENUM, width);
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_BOOL, vsDisable);
    RSDK_EDITABLE_VAR(BoundsMarker, VAR_ENUM, offset);
}

