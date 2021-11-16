#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectEncoreRoute *EncoreRoute;

void EncoreRoute_Update(void)
{
    RSDK_THIS(EncoreRoute);

    int32 offX = entity->offset.x >> 16;
    int32 offY = entity->offset.y >> 16;

    int32 posY = entity->position.y >> 20;
    for (int32 y = 0; y < (entity->size.y >> 0x10); ++y) {
        int32 posX = entity->position.x >> 20;
        for (int32 x = 0; x < (entity->size.x >> 0x10); ++x) {
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

            RSDK.SetTileInfo(dst, x + posX, y + posY, RSDK.GetTileInfo(src, x + offX, y + offY));
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

#if RETRO_INCLUDE_EDITOR
void EncoreRoute_EditorDraw(void)
{
    RSDK_THIS(EncoreRoute);
    Vector2 drawPos;

    entity->drawFX = FX_FLIP;
    entity->active = ACTIVE_NORMAL;

    Vector2 *positions[2] = { &entity->position, &entity->offset };

    Vector2 size;
    size.x = TILE_SIZE * entity->size.x;
    size.y = TILE_SIZE * entity->size.y;

    int count = showGizmos() ? 2 : 1;
    for (int i = 0; i < count; ++i) {
        drawPos.x = positions[i]->x;
        drawPos.y = positions[i]->y;

        DrawHelpers_DrawRectOutline(0xFFFF00, positions[i]->x, positions[i]->y, size.x, size.y);

        entity->direction = FLIP_NONE;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

        drawPos.x += size.x;
        entity->direction = FLIP_X;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

        drawPos.y += size.y;
        entity->direction = FLIP_XY;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

        drawPos.x -= size.x;
        entity->direction = FLIP_Y;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);
    }

    if (showGizmos()) {
        DrawHelpers_DrawArrow(0xE0E0E0, entity->position.x + (size.x >> 1), entity->position.y + (size.y >> 1), entity->offset.x + (size.x >> 1),
                              entity->offset.y + (size.y >> 1));
    }
}

void EncoreRoute_EditorLoad(void)
{
    EncoreRoute->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EncoreRoute->aniFrames, 0, &EncoreRoute->animator, true, 0);
}
#endif

void EncoreRoute_Serialize(void)
{
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_UINT8, layerSrc);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_UINT8, layerDest);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_ENUM, frameID);
}
#endif
