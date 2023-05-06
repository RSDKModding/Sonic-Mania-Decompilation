// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FarPlane Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectFarPlane *FarPlane;

void FarPlane_Update(void) {}

void FarPlane_LateUpdate(void)
{
    RSDK_THIS(FarPlane);

    if (self->active == ACTIVE_ALWAYS) {
        FarPlane_SetupEntities();
    }
    else if (self->active == ACTIVE_BOUNDS) {
        self->active                                       = ACTIVE_NORMAL;
        RSDK.GetTileLayer(FarPlane->layerID)->drawGroup[0] = 0;

        FarPlane->originPos.x = self->origin.x;
        FarPlane->originPos.y = self->origin.y;

        FarPlane->position.x = self->position.x;
        FarPlane->position.y = self->position.y;

        RSDK.SetDrawGroupProperties(1, false, FarPlane_DrawHook_ApplyFarPlane);
        RSDK.SetDrawGroupProperties(3, false, FarPlane_DrawHook_RemoveFarPlane);

        FarPlane_SetEntityActivities(ACTIVE_NORMAL);
    }
    else if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active                                       = ACTIVE_BOUNDS;
        RSDK.GetTileLayer(FarPlane->layerID)->drawGroup[0] = DRAWGROUP_COUNT;

        RSDK.SetDrawGroupProperties(1, false, StateMachine_None);
        RSDK.SetDrawGroupProperties(3, false, StateMachine_None);

        FarPlane_SetEntityActivities(ACTIVE_NEVER);
    }
}

void FarPlane_StaticUpdate(void) {}

void FarPlane_Draw(void)
{
    RSDK_THIS(FarPlane);

    if (SceneInfo->currentDrawGroup) {
        RSDK.SetActivePalette(0, 0, ScreenInfo->size.y);
    }
    else {
        int32 x = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
        int32 y = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;

        FarPlane->screenPos.x = (x + self->origin.x - self->position.x) & 0xFFFE0000;
        FarPlane->screenPos.y = (y + self->origin.y - self->position.y) & 0xFFFE0000;

        FarPlane->worldPos.x = self->position.x - ((self->position.x - x) >> 1) + 0x8000;
        FarPlane->worldPos.y = self->position.y - ((self->position.y - y) >> 1) + 0x8000;

        if (!SceneInfo->currentScreenID)
            RSDK.AddDrawListRef(1, SceneInfo->entitySlot);
    }
}

void FarPlane_Create(void *data)
{
    RSDK_THIS(FarPlane);

    if (!SceneInfo->inEditor) {
        self->updateRange.x = self->size.x + (self->size.x >> 1);
        self->updateRange.y = self->size.y + (self->size.y >> 1);
        self->active        = ACTIVE_ALWAYS;
        self->visible       = true;
        self->drawGroup     = 0;
    }
}

void FarPlane_StageLoad(void)
{
    FarPlane->layerID = RSDK.GetTileLayerID("Far Plane");

    if (FarPlane->layerID != (uint16)-1) {
        TileLayer *farPlane        = RSDK.GetTileLayer(FarPlane->layerID);
        farPlane->drawGroup[0]     = DRAWGROUP_COUNT;
        farPlane->scanlineCallback = FarPlane_Scanline_FarPlaneView;

        RSDK.GetTileLayer(Zone->fgLayer[0])->drawGroup[0] = 2;
        RSDK.GetTileLayer(Zone->fgLayer[1])->drawGroup[0] = 7;

        RSDK.SetDrawGroupProperties(1, false, StateMachine_None);
        RSDK.SetDrawGroupProperties(2, false, StateMachine_None);

        RSDK.CopyPalette(0, 0, 3, 0, 128);
        RSDK.CopyPalette(0, 144, 3, 144, 112);
        RSDK.CopyPalette(3, 0, 4, 0, 255);

#if MANIA_USE_PLUS
        if (SceneInfo->filter & FILTER_MANIA)
#endif
            RSDK.SetLimitedFade(4, 0, 3, 96, 128, 143);

        RSDK.CopyTileLayer(FarPlane->layerID, 0, 192, Zone->fgLayer[0], 0, 192, 1024, 208);

        for (int32 s = 0; s < PLAYER_COUNT; ++s) {
            Zone->cameraBoundsB[s] -= 2048;
            Zone->deathBoundary[s] -= 2048 << 16;
        }

        ++Zone->objectDrawGroup[0];
        ++Zone->playerDrawGroup[0];
        ++Zone->fgDrawGroup[1];
        ++Zone->objectDrawGroup[1];
        ++Zone->playerDrawGroup[1];

        RSDK.ClearCameras();

        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        RSDK.AddCamera(&camera->center, ScreenInfo[camera->screenID].center.x << 16, ScreenInfo[camera->screenID].center.y << 16, 0);
    }
}

void FarPlane_SetupEntities(void)
{
    RSDK_THIS(FarPlane);

    self->entityCount = 0;
    for (int32 i = 0; i < SCENEENTITY_COUNT && self->entityCount < FARPLANE_ENTITY_COUNT; ++i) {
        Entity *child = RSDK_GET_ENTITY_GEN(i);
        if (abs(self->origin.x - child->position.x) < self->size.x && abs(self->origin.y - child->position.y) < self->size.y) {
            self->entitySlots[self->entityCount++] = i;
            child->active                          = ACTIVE_NEVER;
            child->drawGroup                       = 1;
        }
    }

    self->active = ACTIVE_BOUNDS;
}

void FarPlane_SetEntityActivities(uint8 active)
{
    RSDK_THIS(FarPlane);

    for (int32 i = 0; i < self->entityCount; ++i) {
        RSDK_GET_ENTITY_GEN(self->entitySlots[i])->active = active;
    }
}

void FarPlane_DrawHook_ApplyFarPlane(void)
{
    int32 id = 0;
    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *entity = RSDK.GetDrawListRef(1, i);
        if (!entity)
            break;

        FarPlane->positionList[id].x = entity->position.x;
        FarPlane->positionList[id].y = entity->position.y;

        entity->position.x = FarPlane->worldPos.x + ((entity->position.x - FarPlane->originPos.x) >> 1);
        entity->position.y = FarPlane->worldPos.y + ((entity->position.y - FarPlane->originPos.y) >> 1);
        entity->drawFX |= FX_SCALE;
        entity->scale.x = 0x100;
        entity->scale.y = 0x100;
        id++;
    }

    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *entity = RSDK.GetDrawListRef(2, i);
        if (!entity)
            break;

        FarPlane->positionList[id].x = entity->position.x;
        FarPlane->positionList[id].y = entity->position.y;
        entity->position.x           = FarPlane->worldPos.x + ((entity->position.x - FarPlane->originPos.x) >> 1);
        entity->position.y           = FarPlane->worldPos.y + ((entity->position.y - FarPlane->originPos.y) >> 1);
        entity->drawFX |= FX_SCALE;
        entity->scale.x = 0x100;
        entity->scale.y = 0x100;
        id++;
    }

    foreach_active(InvincibleStars, invincibleStars)
    {
        if (invincibleStars->drawGroup < 3 && id < 0x200) {
            invincibleStars->starOffset = 10;
            invincibleStars->drawFX     = FX_SCALE;
            invincibleStars->scale.x    = 0x100;
            invincibleStars->scale.y    = 0x100;

            for (int32 s = 0; s < 8; ++s) {
                FarPlane->positionList[id].x = invincibleStars->starPos[s].x;
                FarPlane->positionList[id].y = invincibleStars->starPos[s].y;

                invincibleStars->starPos[s].x = FarPlane->worldPos.x + ((invincibleStars->starPos[s].x - FarPlane->originPos.x) >> 1);
                invincibleStars->starPos[s].y = FarPlane->worldPos.y + ((invincibleStars->starPos[s].y - FarPlane->originPos.y) >> 1);

                id++;
            }
        }
    }

    foreach_active(ImageTrail, imageTrail)
    {
        if (imageTrail->drawGroup < 3 && id < 0x200) {
            imageTrail->scale.x = 0x100;
            imageTrail->scale.y = 0x100;

            FarPlane->positionList[id].x = imageTrail->currentPos.x;
            FarPlane->positionList[id].y = imageTrail->currentPos.y;

            imageTrail->currentPos.x = FarPlane->worldPos.x + ((imageTrail->currentPos.x - FarPlane->originPos.x) >> 1);
            imageTrail->currentPos.y = FarPlane->worldPos.y + ((imageTrail->currentPos.y - FarPlane->originPos.y) >> 1);

            id++;

            for (int32 s = 0; s < 7; ++s) {
                FarPlane->positionList[id].x = imageTrail->statePos[s].x;
                FarPlane->positionList[id].y = imageTrail->statePos[s].y;

                imageTrail->statePos[s].x = FarPlane->worldPos.x + ((imageTrail->statePos[s].x - FarPlane->originPos.x) >> 1);
                imageTrail->statePos[s].y = FarPlane->worldPos.y + ((imageTrail->statePos[s].y - FarPlane->originPos.y) >> 1);

                id++;
            }
        }
    }
}

void FarPlane_DrawHook_RemoveFarPlane(void)
{
    int32 id = 0;
    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *entity = RSDK.GetDrawListRef(1, i);
        if (!entity)
            break;

        entity->position.x = FarPlane->positionList[id].x;
        entity->position.y = FarPlane->positionList[id].y;
        id++;
    }

    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *entity = RSDK.GetDrawListRef(2, i);
        if (!entity)
            break;

        entity->position.x = FarPlane->positionList[id].x;
        entity->position.y = FarPlane->positionList[id].y;
        id++;
    }

    foreach_active(InvincibleStars, invincibleStars)
    {
        if (invincibleStars->drawGroup == 1 && id < 0x200) {
            for (int32 s = 0; s < 8; ++s) {
                invincibleStars->starPos[s].x = FarPlane->positionList[id].x;
                invincibleStars->starPos[s].y = FarPlane->positionList[id].y;
                id++;
            }
        }
    }

    foreach_active(ImageTrail, imageTrail)
    {
        if (imageTrail->drawGroup == 1 && id < 0x200) {
            imageTrail->currentPos.x = FarPlane->positionList[id].x;
            imageTrail->currentPos.y = FarPlane->positionList[id].y;
            id++;

            for (int32 s = 0; s < 7; ++s) {
                imageTrail->statePos[s].x = FarPlane->positionList[id].x;
                imageTrail->statePos[s].y = FarPlane->positionList[id].y;
                id++;
            }
        }
    }
}

void FarPlane_Scanline_FarPlaneView(ScanlineInfo *scanline)
{
    int32 x = FarPlane->screenPos.x - (ScreenInfo->center.x << 17);
    int32 y = FarPlane->screenPos.y - (ScreenInfo->center.y << 17);

    for (int32 h = 0; h < ScreenInfo->size.y; ++h) {
        scanline->position.x = x;
        scanline->position.y = y;

        scanline->deform.x = 0x20000;
        scanline->deform.y = 0;

        y += 0x20000;
        scanline++;
    }

    RSDK.CopyPalette(0, 0, 4, 0, 128);
    RSDK.SetActivePalette(4, 0, ScreenInfo->size.y);
}

#if GAME_INCLUDE_EDITOR
void FarPlane_EditorDraw(void)
{
    RSDK_THIS(FarPlane);

    self->updateRange.x = self->size.x << 1;
    self->updateRange.y = self->size.y << 1;

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x << 1, self->size.y << 1, 0xFF0000);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->origin.x, self->origin.y, self->size.x << 1, self->size.y << 1, 0xFFFF00);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->origin.x, self->origin.y, 0x00FF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void FarPlane_EditorLoad(void) {}
#endif

void FarPlane_Serialize(void)
{
    RSDK_EDITABLE_VAR(FarPlane, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(FarPlane, VAR_VECTOR2, origin);
}
