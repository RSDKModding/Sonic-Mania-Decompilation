// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FarPlane Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectFarPlane *FarPlane;

void FarPlane_Update(void) {}

void FarPlane_LateUpdate(void)
{
    RSDK_THIS(FarPlane);
    if (self->active == ACTIVE_ALWAYS) {
        FarPlane_SetupEntities();
    }
    else if (self->active == ACTIVE_BOUNDS) {
        self->active                                        = ACTIVE_NORMAL;
        RSDK.GetTileLayer(FarPlane->layerID)->drawLayer[0] = 0;
        FarPlane->originPos.x                                = self->origin.x;
        FarPlane->originPos.y                                = self->origin.y;
        FarPlane->position.x                                = self->position.x;
        FarPlane->position.y                                = self->position.y;
        RSDK.SetDrawLayerProperties(1, false, FarPlane_DrawLayerCB_Low);
        RSDK.SetDrawLayerProperties(3, false, FarPlane_DrawLayerCB_High);
        FarPlane_SetEntityActivities(ACTIVE_NORMAL);
    }
    else if (!RSDK.CheckOnScreen(self, NULL)) {
        self->active                                        = ACTIVE_BOUNDS;
        RSDK.GetTileLayer(FarPlane->layerID)->drawLayer[0] = DRAWGROUP_COUNT;
        RSDK.SetDrawLayerProperties(1, false, NULL);
        RSDK.SetDrawLayerProperties(3, false, NULL);
        FarPlane_SetEntityActivities(ACTIVE_NEVER);
    }
}

void FarPlane_StaticUpdate(void) {}

void FarPlane_Draw(void)
{
    RSDK_THIS(FarPlane);
    if (SceneInfo->currentDrawGroup) {
        RSDK.SetActivePalette(0, 0, ScreenInfo->height);
    }
    else {
        int32 x                = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
        int32 y                = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
        FarPlane->screenPos.x  = (x + self->origin.x - self->position.x) & 0xFFFE0000;
        FarPlane->screenPos.y  = (y + self->origin.y - self->position.y) & 0xFFFE0000;
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
        self->drawOrder     = 0;
    }
}

void FarPlane_StageLoad(void)
{
    FarPlane->layerID = RSDK.GetTileLayerID("Far Plane");

    if (FarPlane->layerID != (uint16)-1) {
        TileLayer *layer                               = RSDK.GetTileLayer(FarPlane->layerID);
        layer->drawLayer[0]                            = DRAWGROUP_COUNT;
        layer->scanlineCallback                        = FarPlane_ScanlineCB;
        RSDK.GetTileLayer(Zone->fgLow)->drawLayer[0]  = 2;
        RSDK.GetTileLayer(Zone->fgHigh)->drawLayer[0] = 7;
        RSDK.SetDrawLayerProperties(1, false, NULL);
        RSDK.SetDrawLayerProperties(2, false, NULL);
        RSDK.CopyPalette(0, 0, 3, 0, 128);
        RSDK.CopyPalette(0, 144, 3, 144, 112);
        RSDK.CopyPalette(3, 0, 4, 0, 255);
#if RETRO_USE_PLUS
        if (SceneInfo->filter & FILTER_MANIA)
#endif
            RSDK.SetLimitedFade(4, 0, 3, 96, 128, 143);
        RSDK.CopyTileLayer(FarPlane->layerID, 0, 192, Zone->fgLow, 0, 192, 1024, 208);
        for (int s = 0; s < PLAYER_MAX; ++s) {
            Zone->cameraBoundsB[s] -= 0x800;
            Zone->deathBoundary[s] -= 0x8000000;
        }

        ++Zone->objectDrawLow;
        ++Zone->playerDrawLow;
        ++Zone->fgLayerHigh;
        ++Zone->objectDrawHigh;
        ++Zone->playerDrawHigh;
        RSDK.ClearCameras();
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        RSDK.AddCamera(&camera->center, ScreenInfo[camera->screenID].centerX << 16, ScreenInfo[camera->screenID].centerY << 16, 0);
    }
}

void FarPlane_SetupEntities(void)
{
    RSDK_THIS(FarPlane);
    self->entityCount = 0;

    for (int32 i = 0; i < SCENEENTITY_COUNT && self->entityCount < 0x100; ++i) {
        Entity *entPtr = RSDK.GetEntity(i);
        if (abs(self->origin.x - entPtr->position.x) < self->size.x) {
            if (abs(self->origin.y - entPtr->position.y) < self->size.y) {
                self->entityIDs[self->entityCount++] = i;
                entPtr->active                       = ACTIVE_NEVER;
                entPtr->drawOrder                    = 1;
            }
        }
    }
    self->active = ACTIVE_BOUNDS;
}

void FarPlane_SetEntityActivities(uint8 active)
{
    RSDK_THIS(FarPlane);
    for (int32 i = 0; i < self->entityCount; ++i) {
        RSDK_GET_ENTITY(self->entityIDs[i], )->active = active;
    }
}

void FarPlane_DrawLayerCB_Low(void)
{
    int32 id = 0;
    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *ent = RSDK.GetDrawListRefPtr(1, i);
        if (!ent)
            break;
        FarPlane->positionList[id].x = ent->position.x;
        FarPlane->positionList[id].y = ent->position.y;
        ent->position.x -= FarPlane->originPos.x;
        ent->position.y -= FarPlane->originPos.y;
        ent->position.x >>= 1;
        ent->position.y >>= 1;
        ent->position.x += FarPlane->worldPos.x;
        ent->position.y += FarPlane->worldPos.y;
        ent->drawFX |= FX_SCALE;
        ent->scale.x = 0x100;
        ent->scale.y = 0x100;
        id++;
    }

    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *ent = RSDK.GetDrawListRefPtr(2, i);
        if (!ent)
            break;
        FarPlane->positionList[id].x = ent->position.x;
        FarPlane->positionList[id].y = ent->position.y;
        ent->position.x -= FarPlane->originPos.x;
        ent->position.y -= FarPlane->originPos.y;
        ent->position.x >>= 1;
        ent->position.y >>= 1;
        ent->position.x += FarPlane->worldPos.x;
        ent->position.y += FarPlane->worldPos.y;
        ent->drawFX |= FX_SCALE;
        ent->scale.x = 0x100;
        ent->scale.y = 0x100;
        id++;
    }

    foreach_active(InvincibleStars, invincibleStars)
    {
        if (invincibleStars->drawOrder < 3 && id < 0x200) {
            invincibleStars->starOffset = 10;
            invincibleStars->drawFX     = FX_SCALE;
            invincibleStars->scale.x    = 0x100;
            invincibleStars->scale.y    = 0x100;

            for (int32 s = 0; s < 8; ++s) {
                FarPlane->positionList[id].x = invincibleStars->starPos[s].x;
                FarPlane->positionList[id].y = invincibleStars->starPos[s].y;
                invincibleStars->starPos[s].x -= FarPlane->originPos.x;
                invincibleStars->starPos[s].y -= FarPlane->originPos.y;
                invincibleStars->starPos[s].x >>= 1;
                invincibleStars->starPos[s].y >>= 1;
                invincibleStars->starPos[s].x += FarPlane->worldPos.x;
                invincibleStars->starPos[s].y += FarPlane->worldPos.y;
                id++;
            }
        }
    }

    foreach_active(ImageTrail, imageTrail)
    {
        if (imageTrail->drawOrder < 3 && id < 0x200) {
            imageTrail->scale.x = 0x100;
            imageTrail->scale.y = 0x100;

            FarPlane->positionList[id].x = imageTrail->currentPos.x;
            FarPlane->positionList[id].y = imageTrail->currentPos.y;
            imageTrail->currentPos.x -= FarPlane->originPos.x;
            imageTrail->currentPos.y -= FarPlane->originPos.y;
            imageTrail->currentPos.x >>= 1;
            imageTrail->currentPos.y >>= 1;
            imageTrail->currentPos.x += FarPlane->worldPos.x;
            imageTrail->currentPos.y += FarPlane->worldPos.y;
            id++;
            for (int32 s = 0; s < 7; ++s) {
                FarPlane->positionList[id].x = imageTrail->statePos[s].x;
                FarPlane->positionList[id].y = imageTrail->statePos[s].y;
                id++;
                imageTrail->statePos[s].x -= FarPlane->originPos.x;
                imageTrail->statePos[s].y -= FarPlane->originPos.y;
                imageTrail->statePos[s].x >>= 1;
                imageTrail->statePos[s].y >>= 1;
                imageTrail->statePos[s].x += FarPlane->worldPos.x;
                imageTrail->statePos[s].y += FarPlane->worldPos.y;
            }
        }
    }
}

void FarPlane_DrawLayerCB_High(void)
{
    int32 id = 0;
    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *ent = RSDK.GetDrawListRefPtr(1, i);
        if (!ent)
            break;
        ent->position.x = FarPlane->positionList[id].x;
        ent->position.y = FarPlane->positionList[id].y;
        id++;
    }

    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *ent = RSDK.GetDrawListRefPtr(2, i);
        if (!ent)
            break;
        ent->position.x = FarPlane->positionList[id].x;
        ent->position.y = FarPlane->positionList[id].y;
        id++;
    }

    foreach_active(InvincibleStars, invincibleStars)
    {
        if (invincibleStars->drawOrder == 1 && id < 0x200) {
            for (int32 s = 0; s < 8; ++s) {
                invincibleStars->starPos[s].x = FarPlane->positionList[id].x;
                invincibleStars->starPos[s].y = FarPlane->positionList[id].y;
                id++;
            }
        }
    }

    foreach_active(ImageTrail, imageTrail)
    {
        if (imageTrail->drawOrder == 1 && id < 0x200) {
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

void FarPlane_ScanlineCB(ScanlineInfo *scanline)
{
    int32 x = FarPlane->screenPos.x - (ScreenInfo->centerX << 17);
    int32 y = FarPlane->screenPos.y - (ScreenInfo->centerY << 17);
    for (int32 h = 0; h < ScreenInfo->height; ++h) {
        scanline->position.x = x;
        scanline->position.y = y;

        scanline->deform.x = 0x20000;
        scanline->deform.y = 0;
        y += 0x20000;
        scanline++;
    }
    RSDK.CopyPalette(0, 0, 4, 0, 128);
    RSDK.SetActivePalette(4, 0, ScreenInfo->height);
}

#if RETRO_INCLUDE_EDITOR
void FarPlane_EditorDraw(void)
{
    RSDK_THIS(FarPlane);

    self->updateRange.x = self->size.x << 1;
    self->updateRange.y = self->size.y << 1;

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

    DrawHelpers_DrawRectOutline(self->origin.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

    DrawHelpers_DrawArrow(self->position.x, self->position.y, self->origin.x, self->origin.y, 0x00FF00, INK_NONE, 0xFF);
}

void FarPlane_EditorLoad(void) {}
#endif

void FarPlane_Serialize(void)
{
    RSDK_EDITABLE_VAR(FarPlane, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(FarPlane, VAR_VECTOR2, origin);
}
