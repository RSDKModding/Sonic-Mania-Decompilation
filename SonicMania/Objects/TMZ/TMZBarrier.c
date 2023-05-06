// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TMZBarrier Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTMZBarrier *TMZBarrier;

void TMZBarrier_Update(void)
{
    RSDK_THIS(TMZBarrier);

    if (self->cleared) {
        TMZBarrier->clearedBarriers |= 1 << self->iD;

        if (self->type)
            RSDK.CopyTileLayer(Zone->fgLayer[0], self->position.x >> 20, self->position.y >> 20, Zone->moveLayer, 59, 32, 2, 4);
        else
            RSDK.CopyTileLayer(Zone->fgLayer[0], self->position.x >> 20, self->position.y >> 20, Zone->moveLayer, 56, 32, 2, 9);

        destroyEntity(self);
    }
}

void TMZBarrier_LateUpdate(void) {}

void TMZBarrier_StaticUpdate(void)
{
    globals->tempFlags = TMZBarrier->clearedBarriers;

    if (TMZBarrier->postID != StarPost->postIDs[0]) {
        TMZBarrier->postID    = StarPost->postIDs[0];
        globals->restartFlags = TMZBarrier->clearedBarriers;
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
            TMZBarrier->clearedBarriers = globals->restartFlags;
        }
        else {
            TMZBarrier->clearedBarriers = globals->tempFlags;
        }

        TMZBarrier->postID = StarPost->postIDs[0];
        foreach_all(TMZBarrier, barrier)
        {
            if ((1 << barrier->iD) & TMZBarrier->clearedBarriers) {
                if (barrier->type)
                    RSDK.CopyTileLayer(Zone->fgLayer[0], barrier->position.x >> 20, barrier->position.y >> 20, Zone->moveLayer, 59, 32, 2, 4);
                else
                    RSDK.CopyTileLayer(Zone->fgLayer[0], barrier->position.x >> 20, barrier->position.y >> 20, Zone->moveLayer, 56, 32, 2, 9);

                destroyEntity(barrier);
            }
        }
    }
    else {
        TMZBarrier->postID          = 0;
        TMZBarrier->clearedBarriers = 0;

        globals->restartFlags = 0;
        globals->tempFlags    = 0;
    }
}

#if GAME_INCLUDE_EDITOR
void TMZBarrier_EditorDraw(void)
{
    RSDK_THIS(TMZBarrier);

    Animator animator;
    RSDK.SetSpriteAnimation(TMZBarrier->postID, 0, &animator, true, self->type ? 3 : 4);

    RSDK.DrawSprite(&animator, NULL, false);

    if (showGizmos()) {
        EntityWarpDoor *taggedDoor = NULL;
        foreach_all(WarpDoor, door)
        {
            if (door->tag == self->warpTag) {
                taggedDoor = door;
                foreach_break;
            }
        }

        if (taggedDoor) {
            RSDK_DRAWING_OVERLAY(true);
            if (taggedDoor) {
                DrawHelpers_DrawArrow(taggedDoor->position.x, taggedDoor->position.y, self->position.x, self->position.y, 0xFFFF00, INK_NONE, 0xFF);
            }
            RSDK_DRAWING_OVERLAY(false);
        }
    }
}

void TMZBarrier_EditorLoad(void)
{
    // borrowing this for "aniFrames" storage
    TMZBarrier->postID = RSDK.LoadSpriteAnimation("TMZ1/Platform.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(TMZBarrier, type);
    RSDK_ENUM_VAR("Long", TMZBARRIER_LONG);
    RSDK_ENUM_VAR("Short", TMZBARRIER_SHORT);

    RSDK_ACTIVE_VAR(TMZBarrier, iD);
    RSDK_ENUM_VAR("Barrier 1", 0);
    RSDK_ENUM_VAR("Barrier 2", 1);
    RSDK_ENUM_VAR("Barrier 3", 2);
    RSDK_ENUM_VAR("Barrier 4", 3);
}
#endif

void TMZBarrier_Serialize(void)
{
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, warpTag);
    RSDK_EDITABLE_VAR(TMZBarrier, VAR_UINT8, iD);
}
