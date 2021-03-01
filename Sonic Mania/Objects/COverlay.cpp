#include "../SonicMania.hpp"

ObjectCOverlay *COverlay;

void COverlay_Update()
{

}

void COverlay_LateUpdate()
{

}

void COverlay_StaticUpdate()
{

}

void COverlay_Draw()
{
    ScreenInfo *screen     = RSDK_screens;
    EntityCOverlay *entity = (EntityCOverlay *)RSDK_sceneInfo->entity;
    int tileX              = 0;
    for (entity->position.x = (screen->position.x & 0xFFFFFFF0) << 16; tileX < (screen->width >> 4) + 2; ++tileX) {
        int tileY = 0;
        for (entity->position.y = (screen->position.y & 0xFFFFFFF0) << 16; tileY < (screen->height >> 4) + 2;
             entity->position.y += 0x100000, ++tileY) {
            COverlay_DrawTile();
        }
        entity->position.x += 0x100000;
    }
}

void COverlay_Create(void* data)
{
    EntityCOverlay *entity = (EntityCOverlay *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderHigh;
    }
}

void COverlay_StageLoad()
{
    COverlay->spriteIndex = RSDK.LoadAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);
    if (DebugMode->itemCount < 256) {
        DebugMode->objectIDs[DebugMode->itemCount] = COverlay->objectID;
        DebugMode->spawn[DebugMode->itemCount]  = COverlay_DebugSpawn;
        DebugMode->draw[DebugMode->itemCount++] = COverlay_DebugDraw;
    }
}

void COverlay_DebugDraw()
{
    RSDK.SetSpriteAnimation(COverlay->spriteIndex, 0, &DebugMode->debugData, true, 0);
    DebugMode->debugData.frameID = 4;
    return RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}

void COverlay_DebugSpawn()
{
    Entity *entity = NULL;
    while (RSDK.GetActiveObjects(COverlay->objectID, (Entity **)&entity)) {
        RSDK.DestroyEntity(entity, NULL, 0);
    }
    RSDK.SpawnEntity(COverlay->objectID, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
}

void COverlay_DrawTile()
{
    EntityCOverlay *entity = (EntityCOverlay *)RSDK_sceneInfo->entity;
    EntityPlayer *player   = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
    int tx                 = 0;
    for (int x = 0; x < 0x10; ++x) {
        int y    = 0;
        int ty   = -1;
        int th2  = -1;
        int ty2  = -1;
        int th   = -1;
        int flip = FLIP_NONE;
        for (int y = 0; y < 0x10; ++y) {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 0, player->collisionPlane, x << 0x10, y << 0x10, false)) {
                flip |= FLIP_X;
                th2 = y;
                if (ty == -1)
                    ty = y - 1;
            }

            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, 2, player->collisionPlane, x << 0x10, y << 0x10, false)) {
                flip |= FLIP_Y;
                th = y;
                if (ty2 == -1) {
                    ty2 = y - 2;
                }
            }
        }

        if ((ty <= ty2 || ty == -1) && ty2 != -1)
            ty = ty2;
        if (th2 < th)
            th = th2;
        if (ty != -1) {
            switch (flip) {
                case 1:
                    RSDK.DrawLine(entity->position.x + tx, entity->position.y + (ty << 16), entity->position.x + tx, entity->position.y + (th << 16),
                                  0xE0E000, 0, INK_NONE, 0);
                    break;
                case 2:
                    RSDK.DrawLine(entity->position.x + tx, entity->position.y + (ty << 16), entity->position.x + tx, entity->position.y + (th << 16),
                                  0xE00000, 0, INK_NONE, 0);
                    break;
                case 3:
                    RSDK.DrawLine(entity->position.x + tx, entity->position.y + (ty << 16), entity->position.x + tx, entity->position.y + (th << 16),
                                  0xE0E0E0, 0, INK_NONE, 0);
                    break;
            }
        }

        tx += 0x10000;
    }
}

void COverlay_EditorDraw()
{

}

void COverlay_EditorLoad()
{

}

void COverlay_Serialize()
{

}

