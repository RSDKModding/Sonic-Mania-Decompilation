// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZBarrier Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTMZBarrier *TMZBarrier;

void TMZBarrier_Update(void)
{
    RSDK_THIS(TMZBarrier);
    if (self->field_5C) {
        TMZBarrier->flags |= 1 << self->iD;
        if (self->type)
            RSDK.CopyTileLayer(Zone->fgLow, self->position.x >> 20, self->position.y >> 20, Zone->moveLayer, 59, 32, 2, 4);
        else
            RSDK.CopyTileLayer(Zone->fgLow, self->position.x >> 20, self->position.y >> 20, Zone->moveLayer, 56, 32, 2, 9);
        destroyEntity(self);
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
    if (!SceneInfo->inEditor)
        self->active = ACTIVE_NORMAL;
}

void TMZBarrier_StageLoad(void)
{
    if (SceneInfo->minutes || SceneInfo->seconds || SceneInfo->milliseconds) {
        if (SceneInfo->minutes != globals->tempMinutes || SceneInfo->seconds != globals->tempSeconds
            || SceneInfo->milliseconds != globals->tempMilliseconds) {
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
                destroyEntity(barrier);
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

#if RETRO_INCLUDE_EDITOR
void TMZBarrier_EditorDraw(void) {}

void TMZBarrier_EditorLoad(void) {}
#endif

void TMZBarrier_Serialize(void)
{
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, warpTag);
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, iD);
}
