#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectEncoreRoute *EncoreRoute;

void EncoreRoute_Update(void)
{
    RSDK_THIS(EncoreRoute);

    int32 offX = entity->offset.x >> 16;
    int32 offY = entity->offset.y >> 16;

    int32 posY = entity->position.y >> 20;
    for (int32 y = 0; y < (entity->size.y >> 0x10); ++y, ++posY) {
        int32 posX = entity->position.x >> 20;
        for (int32 x = 0; x < (entity->size.x >> 0x10); ++x, ++posX) {
            uint8 src = -1;
            uint8 dst = -1;
            switch (entity->layerSrc) {
                case 0: src = Zone->fgLow; break;
                case 1: src = Zone->fgHigh; break;
                case 2: src = Zone->moveLayer; break;
                case 3: src = Zone->scratchLayer; break;
                default: break;
            }
            switch (entity->layerDest) {
                case 0: dst = Zone->fgLow; break;
                case 1: dst = Zone->fgHigh; break;
                case 2: dst = Zone->moveLayer; break;
                case 3: dst = Zone->scratchLayer; break;
                default: break;
            }

            RSDK.SetTileInfo(dst, posX, posY, RSDK.GetTileInfo(src, x + offX, y + offY));
        }
    }
    destroyEntity(entity);
}

void EncoreRoute_LateUpdate(void) {}

void EncoreRoute_StaticUpdate(void) {}

void EncoreRoute_Draw(void) {}

void EncoreRoute_Create(void *data)
{
    RSDK_THIS(EncoreRoute);
    entity->active = ACTIVE_ALWAYS;
}

void EncoreRoute_StageLoad(void) {}

void EncoreRoute_EditorDraw(void)
{
    RSDK_THIS(EncoreRoute);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= (TILE_SIZE * entity->size.x) >> 1;
    drawPos.y -= (TILE_SIZE * entity->size.y) >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x - 0x10000, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xE0E0E0, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + entity->size.y, 0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xE0E0E0, 0, INK_NONE,
                  false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

    drawPos.x += TILE_SIZE * entity->size.x;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

    drawPos.y += TILE_SIZE * entity->size.y;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

    drawPos.x -= TILE_SIZE * entity->size.x;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

}

void EncoreRoute_EditorLoad(void)
{
    EncoreRoute->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EncoreRoute->aniFrames, 0, &EncoreRoute->animator, true, 0);
}

void EncoreRoute_Serialize(void)
{
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_UINT8, layerSrc);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_UINT8, layerDest);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_ENUM, frameID);
}
#endif
