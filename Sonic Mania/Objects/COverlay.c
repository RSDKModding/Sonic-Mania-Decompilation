#include "../SonicMania.h"

ObjectCOverlay *COverlay;

void COverlay_Update(void) {}

void COverlay_LateUpdate(void) {}

void COverlay_StaticUpdate(void) {}

void COverlay_Draw(void)
{
    RSDK_THIS(COverlay);
    ScreenInfo *screen = RSDK_screens;
    int tileX          = 0;
    for (entity->position.x = (screen->position.x & 0xFFFFFFF0) << 16; tileX < (screen->width >> 4) + 2; ++tileX) {
        int tileY = 0;
        for (entity->position.y = (screen->position.y & 0xFFFFFFF0) << 16; tileY < (screen->height >> 4) + 2; ++tileY) {
            COverlay_DrawTile();
            entity->position.y += 16 << 0x10;
        }
        entity->position.x += 16 << 0x10;
    }
}

void COverlay_Create(void *data)
{
    RSDK_THIS(COverlay);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_ALWAYS;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderHigh;
    }
}

void COverlay_StageLoad(void)
{
    COverlay->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);
    DEBUGMODE_ADD_OBJ(COverlay);
}

void COverlay_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(COverlay->spriteIndex, 0, &DebugMode->animator, true, 0);
    DebugMode->animator.frameID = 4;
    RSDK.DrawSprite(&DebugMode->animator, 0, 0);
}

void COverlay_DebugSpawn(void)
{
    int count = 0;
    foreach_all(COverlay, entity) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
        ++count;
    }
    if (!count)
        RSDK.CreateEntity(COverlay->objectID, NULL, RSDK_sceneInfo->entity->position.x, RSDK_sceneInfo->entity->position.y);
}

void COverlay_DrawTile(void)
{
    RSDK_THIS(COverlay);
    EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    int tx               = 0;
    for (int x = 0; x < 0x10; ++x) {
        byte ty   = -1;
        byte th2  = -1;
        byte ty2  = -1;
        byte th   = -1;
        byte solid = 0;
        for (int y = 0; y < 0x10; ++y) {
            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, player->collisionPlane, x << 0x10, y << 0x10, false)) {
                solid |= 1;
                th2 = y + 1;
                if (ty == 0xFF)
                    ty = y;
            }

            if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, player->collisionPlane, x << 0x10, y << 0x10, false)) {
                solid |= 2;
                th = y + 1;
                if (ty2 == 0xFF) {
                    ty2 = y - 1;
                }
            }
        }

        if ((ty <= ty2 || ty == 0xFF) && ty2 != 0xFF)
            ty = ty2;
        if (th2 < th)
            th = th2;

        if (ty != 0xFF) {
            switch (solid) {
                case 1:
                    RSDK.DrawLine(entity->position.x + tx, entity->position.y + (ty << 16), entity->position.x + tx, entity->position.y + (th << 16),
                                  0xE0E000, 0xFF, INK_NONE, false);
                    break;
                case 2:
                    RSDK.DrawLine(entity->position.x + tx, entity->position.y + (ty << 16), entity->position.x + tx, entity->position.y + (th << 16),
                                  0xE00000, 0xFF, INK_NONE, false);
                    break;
                case 3:
                    RSDK.DrawLine(entity->position.x + tx, entity->position.y + (ty << 16), entity->position.x + tx, entity->position.y + (th << 16),
                                  0xE0E0E0, 0xFF, INK_NONE, false);
                    break;
            }
        }

        tx += 0x10000;
    }
}

void COverlay_EditorDraw(void) {}

void COverlay_EditorLoad(void) {}

void COverlay_Serialize(void) {}
