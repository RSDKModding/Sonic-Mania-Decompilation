#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectEncoreRoute *EncoreRoute;

void EncoreRoute_Update(void)
{
    RSDK_THIS(EncoreRoute);

    int offX = entity->offset.x >> 16;
    int offY = entity->offset.y >> 16;

    int posY = entity->position.y >> 20;
    for (int y = 0; y < (entity->size.y >> 0x10); ++y, ++posY) {
        int posX = entity->position.x >> 20;
        for (int x = 0; x < (entity->size.x >> 0x10); ++x, ++posX) {
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

void EncoreRoute_EditorDraw(void) {}

void EncoreRoute_EditorLoad(void) {}

void EncoreRoute_Serialize(void)
{
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_UINT8, layerSrc);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_UINT8, layerDest);
    RSDK_EDITABLE_VAR(EncoreRoute, VAR_ENUM, frameID);
}
#endif
