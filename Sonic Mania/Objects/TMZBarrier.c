#include "../SonicMania.h"

ObjectTMZBarrier *TMZBarrier;

void TMZBarrier_Update(void)
{
    RSDK_THIS(TMZBarrier);
    if (entity->field_5C) {
        TMZBarrier->flags |= 1 << entity->iD;
        if (entity->type)
            RSDK.CopyTileLayer(Zone->fgLow, entity->position.x >> 20, entity->position.y >> 20, Zone->moveLayer, 59, 32, 2, 4);
        else
            RSDK.CopyTileLayer(Zone->fgLow, entity->position.x >> 20, entity->position.y >> 20, Zone->moveLayer, 56, 32, 2, 9);
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void TMZBarrier_LateUpdate(void) {}

void TMZBarrier_StaticUpdate(void)
{
    globals->tempFlags = TMZBarrier->flags;
    if (TMZBarrier->postID != StarPost->postIDs[0]) {
        TMZBarrier->postID    = StarPost->postIDs[0];
        globals->restartFlags = TMZBarrier->flags;
    }
}

void TMZBarrier_Draw(void) {}

void TMZBarrier_Create(void *data)
{
    RSDK_THIS(TMZBarrier);
    if (!RSDK_sceneInfo->inEditor)
        entity->active = ACTIVE_NORMAL;
}

void TMZBarrier_StageLoad(void)
{
    if (RSDK_sceneInfo->minutes || RSDK_sceneInfo->seconds || RSDK_sceneInfo->milliseconds) {
        if (RSDK_sceneInfo->minutes != globals->tempMinutes || RSDK_sceneInfo->seconds != globals->tempSeconds
            || RSDK_sceneInfo->milliseconds != globals->tempMilliseconds) {
            TMZBarrier->flags = globals->restartFlags;
        }
        else {
            TMZBarrier->flags = globals->tempFlags;
        }

        TMZBarrier->postID = StarPost->postIDs[0];
        foreach_all(TMZBarrier, barrier)
        {
            if ((1 << barrier->iD) & TMZBarrier->flags) {
                if (barrier->type)
                    RSDK.CopyTileLayer(Zone->fgLow, barrier->position.x >> 20, barrier->position.y >> 20, Zone->moveLayer, 59, 32, 2, 4);
                else
                    RSDK.CopyTileLayer(Zone->fgLow, barrier->position.x >> 20, barrier->position.y >> 20, Zone->moveLayer, 56, 32, 2, 9);
                RSDK.ResetEntityPtr(barrier, TYPE_BLANK, NULL);
            }
        }
    }
    else {
        TMZBarrier->postID    = 0;
        TMZBarrier->flags     = 0;
        globals->restartFlags = 0;
        globals->tempFlags    = 0;
    }
}

void TMZBarrier_EditorDraw(void) {}

void TMZBarrier_EditorLoad(void) {}

void TMZBarrier_Serialize(void)
{
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, warpTag);
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, iD);
}
