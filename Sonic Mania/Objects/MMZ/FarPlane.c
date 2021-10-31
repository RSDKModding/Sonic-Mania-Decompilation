#include "SonicMania.h"

ObjectFarPlane *FarPlane;

void FarPlane_Update(void) {}

void FarPlane_LateUpdate(void)
{
    RSDK_THIS(FarPlane);
    if (entity->active == ACTIVE_ALWAYS) {
        FarPlane_SetupEntities();
    }
    else if (entity->active == ACTIVE_BOUNDS) {
        entity->active                                      = ACTIVE_NORMAL;
        RSDK.GetSceneLayer(FarPlane->layerID)->drawLayer[0] = 0;
        FarPlane->field_18.x                                = entity->origin.x;
        FarPlane->field_18.y                                = entity->origin.y;
        FarPlane->field_20.x                                = entity->position.x;
        FarPlane->field_20.y                                = entity->position.y;
        RSDK.SetDrawLayerProperties(1, false, FarPlane_DrawLayerCB_Low);
        RSDK.SetDrawLayerProperties(3, false, FarPlane_DrawLayerCB_High);
        FarPlane_SetEntityActivities(ACTIVE_NORMAL);
    }
    else if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->active                                      = ACTIVE_BOUNDS;
        RSDK.GetSceneLayer(FarPlane->layerID)->drawLayer[0] = DRAWLAYER_COUNT;
        RSDK.SetDrawLayerProperties(1, false, NULL);
        RSDK.SetDrawLayerProperties(3, false, NULL);
        FarPlane_SetEntityActivities(ACTIVE_NEVER);
    }
}

void FarPlane_StaticUpdate(void) {}

void FarPlane_Draw(void)
{
    RSDK_THIS(FarPlane);
    if (RSDK_sceneInfo->currentDrawGroup) {
        RSDK.SetActivePalette(0, 0, RSDK_screens->height);
    }
    else {
        int32 x                = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
        int32 y                = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
        FarPlane->field_8.x  = (x + entity->origin.x - entity->position.x) & 0xFFFE0000;
        FarPlane->field_8.y  = (y + entity->origin.y - entity->position.y) & 0xFFFE0000;
        FarPlane->field_10.x = entity->position.x - ((entity->position.x - x) >> 1) + 0x8000;
        FarPlane->field_10.y = entity->position.y - ((entity->position.y - y) >> 1) + 0x8000;
        if (!RSDK_sceneInfo->currentScreenID)
            RSDK.AddDrawListRef(1, RSDK_sceneInfo->entitySlot);
    }
}

void FarPlane_Create(void *data)
{
    RSDK_THIS(FarPlane);

    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = entity->size.x + (entity->size.x >> 1);
        entity->updateRange.y = entity->size.y + (entity->size.y >> 1);
        entity->active        = ACTIVE_ALWAYS;
        entity->visible       = true;
        entity->drawOrder     = 0;
    }
}

void FarPlane_StageLoad(void)
{
    FarPlane->layerID = RSDK.GetSceneLayerID("Far Plane");
    if (FarPlane->layerID != 0xFFFF) {
        TileLayer *layer                               = RSDK.GetSceneLayer(FarPlane->layerID);
        layer->drawLayer[0]                            = DRAWLAYER_COUNT;
        layer->scanlineCallback                        = FarPlane_ScanlineCB;
        RSDK.GetSceneLayer(Zone->fgLow)->drawLayer[0]  = 2;
        RSDK.GetSceneLayer(Zone->fgHigh)->drawLayer[0] = 7;
        RSDK.SetDrawLayerProperties(1, false, NULL);
        RSDK.SetDrawLayerProperties(2, false, NULL);
        RSDK.CopyPalette(0, 0, 3, 0, 128);
        RSDK.CopyPalette(0, 144, 3, 144, 112);
        RSDK.CopyPalette(3, 0, 4, 0, 255);
#if RETRO_USE_PLUS
        if (RSDK_sceneInfo->filter & FILTER_MANIA)
#endif
            RSDK.SetLimitedFade(4, 0, 3, 96, 128, 143);
        RSDK.CopyTileLayer(FarPlane->layerID, 0, 192, Zone->fgLow, 0, 192, 1024, 208);
        Zone->screenBoundsB1[0] -= 0x800;
        Zone->deathBoundary[0] -= 0x8000000;
        Zone->screenBoundsB1[1] -= 0x800;
        Zone->deathBoundary[1] -= 0x8000000;
        Zone->screenBoundsB1[2] -= 0x800;
        Zone->deathBoundary[2] -= 0x8000000;
        Zone->screenBoundsB1[3] -= 0x800;
        Zone->deathBoundary[3] -= 0x8000000;

        ++Zone->drawOrderLow;
        ++Zone->playerDrawLow;
        ++Zone->fgLayerHigh;
        ++Zone->drawOrderHigh;
        ++Zone->playerDrawHigh;
        RSDK.ClearCameras();
        EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        RSDK.AddCamera(&camera->center, RSDK_screens[camera->screenID].centerX << 16, RSDK_screens[camera->screenID].centerY << 16, 0);
    }
}

void FarPlane_SetupEntities(void)
{
    RSDK_THIS(FarPlane);
    entity->entityCount = 0;

    for (int32 i = 0; i < SCENEENTITY_COUNT && entity->entityCount < 0x100; ++i) {
        Entity *entPtr = RSDK.GetEntityByID(i);
        if (abs(entity->origin.x - entPtr->position.x) < entity->size.x) {
            if (abs(entity->origin.y - entPtr->position.y) < entity->size.y) {
                entity->entityIDs[entity->entityCount++] = i;
                entPtr->active                           = ACTIVE_NEVER;
                entPtr->drawOrder                        = 1;
            }
        }
    }
    entity->active = ACTIVE_BOUNDS;
}

void FarPlane_SetEntityActivities(uint8 active)
{
    RSDK_THIS(FarPlane);
    for (int32 i = 0; i < entity->entityCount; ++i) {
        RSDK_GET_ENTITY(entity->entityIDs[i], )->active = active;
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
        id++;
        ent->position.x -= FarPlane->field_18.x;
        ent->position.y -= FarPlane->field_18.y;
        ent->position.x >>= 1;
        ent->position.y >>= 1;
        ent->position.x += FarPlane->field_10.x;
        ent->position.y += FarPlane->field_10.y;
        ent->drawFX |= FX_SCALE;
        ent->scale.x = 0x100;
        ent->scale.y = 0x100;
    }

    for (int32 i = 0; i < 0x200 && id < 0x200; ++i) {
        Entity *ent = RSDK.GetDrawListRefPtr(2, i);
        if (!ent)
            break;
        FarPlane->positionList[id].x = ent->position.x;
        FarPlane->positionList[id].y = ent->position.y;
        id++;
        ent->position.x -= FarPlane->field_18.x;
        ent->position.y -= FarPlane->field_18.y;
        ent->position.x >>= 1;
        ent->position.y >>= 1;
        ent->position.x += FarPlane->field_10.x;
        ent->position.y += FarPlane->field_10.y;
        ent->drawFX |= FX_SCALE;
        ent->scale.x = 0x100;
        ent->scale.y = 0x100;
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
                id++;
                invincibleStars->starPos[s].x -= FarPlane->field_18.x;
                invincibleStars->starPos[s].y -= FarPlane->field_18.y;
                invincibleStars->starPos[s].x >>= 1;
                invincibleStars->starPos[s].y >>= 1;
                invincibleStars->starPos[s].x += FarPlane->field_10.x;
                invincibleStars->starPos[s].y += FarPlane->field_10.y;
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
            id++;
            imageTrail->currentPos.x -= FarPlane->field_18.x;
            imageTrail->currentPos.y -= FarPlane->field_18.y;
            imageTrail->currentPos.x >>= 1;
            imageTrail->currentPos.y >>= 1;
            imageTrail->currentPos.x += FarPlane->field_10.x;
            imageTrail->currentPos.y += FarPlane->field_10.y;

            for (int32 s = 0; s < 7; ++s) {
                FarPlane->positionList[id].x = imageTrail->statePos[s].x;
                FarPlane->positionList[id].y = imageTrail->statePos[s].y;
                id++;
                imageTrail->statePos[s].x -= FarPlane->field_18.x;
                imageTrail->statePos[s].y -= FarPlane->field_18.y;
                imageTrail->statePos[s].x >>= 1;
                imageTrail->statePos[s].y >>= 1;
                imageTrail->statePos[s].x += FarPlane->field_10.x;
                imageTrail->statePos[s].y += FarPlane->field_10.y;
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
        if (invincibleStars->drawOrder == 1 && id < 0x200) {
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
    int32 x = FarPlane->field_8.x - (RSDK_screens->centerX << 17);
    int32 y = FarPlane->field_8.y - (RSDK_screens->centerY << 17);
    for (int32 h = 0; h < RSDK_screens->height; ++h) {
        scanline->position.x = x;
        scanline->position.y = y;

        scanline->deform.x = 0x20000;
        scanline->deform.y = 0;
        y += 0x20000;
        scanline++;
    }
    RSDK.CopyPalette(0, 0, 4, 0, 128);
    RSDK.SetActivePalette(4, 0, RSDK_screens->height);
}

#if RETRO_INCLUDE_EDITOR
void FarPlane_EditorDraw(void)
{
    RSDK_THIS(FarPlane);

    entity->updateRange.x = entity->size.x << 1;
    entity->updateRange.y = entity->size.y << 1;

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);

    drawPos.x = entity->origin.x;
    drawPos.y = entity->origin.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);

    DrawHelpers_DrawArrow(0x00FF00, entity->position.x, entity->position.y, entity->origin.x, entity->origin.y);
}

void FarPlane_EditorLoad(void) {}
#endif

void FarPlane_Serialize(void)
{
    RSDK_EDITABLE_VAR(FarPlane, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(FarPlane, VAR_VECTOR2, origin);
}
