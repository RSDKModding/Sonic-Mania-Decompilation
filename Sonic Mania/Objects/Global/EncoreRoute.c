#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectEncoreRoute *EncoreRoute;

void EncoreRoute_Update(void)
{
    RSDK_THIS(EncoreRoute);

    int32 offX = self->offset.x >> 16;
    int32 offY = self->offset.y >> 16;

    int32 posY = self->position.y >> 20;
    for (int32 y = 0; y < (self->size.y >> 0x10); ++y) {
        int32 posX = self->position.x >> 20;
        for (int32 x = 0; x < (self->size.x >> 0x10); ++x) {
            uint8 src = -1;
            uint8 dst = -1;
            switch (self->layerSrc) {
                case 0: src = Zone->fgLow; break;
                case 1: src = Zone->fgHigh; break;
                case 2: src = Zone->moveLayer; break;
                case 3: src = Zone->scratchLayer; break;
                default: break;
            }
            switch (self->layerDest) {
                case 0: dst = Zone->fgLow; break;
                case 1: dst = Zone->fgHigh; break;
                case 2: dst = Zone->moveLayer; break;
                case 3: dst = Zone->scratchLayer; break;
                default: break;
            }

            RSDK.SetTileInfo(dst, x + posX, y + posY, RSDK.GetTileInfo(src, x + offX, y + offY));
        }
    }
    destroyEntity(self);
}

void EncoreRoute_LateUpdate(void) {}

void EncoreRoute_StaticUpdate(void) {}

void EncoreRoute_Draw(void) {}

void EncoreRoute_Create(void *data)
{
    RSDK_THIS(EncoreRoute);
    self->active = ACTIVE_ALWAYS;
}

void EncoreRoute_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void EncoreRoute_EditorDraw(void)
{
    RSDK_THIS(EncoreRoute);
    Vector2 drawPos;

    self->drawFX = FX_FLIP;
    self->active = ACTIVE_NORMAL;

    Vector2 *positions[2] = { &self->position, &self->offset };

    Vector2 size;
    size.x = TILE_SIZE * self->size.x;
    size.y = TILE_SIZE * self->size.y;

    int count = showGizmos() ? 2 : 1;
    for (int i = 0; i < count; ++i) {
        drawPos.x = positions[i]->x + (size.x >> 1);
        drawPos.y = positions[i]->y + (size.y >> 1);

        DrawHelpers_DrawRectOutline(0xFFFF00, drawPos.x, drawPos.y, size.x, size.y);

        drawPos.x = positions[i]->x;
        drawPos.y = positions[i]->y;

        self->direction = FLIP_NONE;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

        drawPos.x += size.x;
        self->direction = FLIP_X;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

        drawPos.y += size.y;
        self->direction = FLIP_XY;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);

        drawPos.x -= size.x;
        self->direction = FLIP_Y;
        RSDK.DrawSprite(&EncoreRoute->animator, &drawPos, false);
    }

    if (showGizmos()) {
        DrawHelpers_DrawArrow(0xE0E0E0, self->position.x + (size.x >> 1), self->position.y + (size.y >> 1), self->offset.x + (size.x >> 1),
                              self->offset.y + (size.y >> 1));
    }
}

void EncoreRoute_EditorLoad(void)
{
    EncoreRoute->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EncoreRoute->aniFrames, 0, &EncoreRoute->animator, true, 0);

    RSDK_ACTIVE_VAR(EncoreRoute, frameID);
    RSDK_ENUM_VAR("(Unused)", EROUTE_FRAME_UNUSED);
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
