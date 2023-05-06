// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: EncoreRoute Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectEncoreRoute *EncoreRoute;

void EncoreRoute_Update(void)
{
    RSDK_THIS(EncoreRoute);

    uint8 layerSrc  = -1;
    uint8 layerDest = -1;

    switch (self->layerSrc) {
        case EROUTE_LAYER_FGLOW: layerSrc = Zone->fgLayer[0]; break;
        case EROUTE_LAYER_FGHIGH: layerSrc = Zone->fgLayer[1]; break;
        case EROUTE_LAYER_MOVE: layerSrc = Zone->moveLayer; break;
        case EROUTE_LAYER_SCRATCH: layerSrc = Zone->scratchLayer; break;
        default: break;
    }

    switch (self->layerDest) {
        case EROUTE_LAYER_FGLOW: layerDest = Zone->fgLayer[0]; break;
        case EROUTE_LAYER_FGHIGH: layerDest = Zone->fgLayer[1]; break;
        case EROUTE_LAYER_MOVE: layerDest = Zone->moveLayer; break;
        case EROUTE_LAYER_SCRATCH: layerDest = Zone->scratchLayer; break;
        default: break;
    }

    int32 srcX = self->offset.x >> 16;
    int32 srcY = self->offset.y >> 16;
    int32 dstX = self->position.x >> 20;
    int32 dstY = self->position.y >> 20;

    for (int32 y = 0; y < (self->size.y >> 16); ++y) {
        for (int32 x = 0; x < (self->size.x >> 16); ++x) {
            uint16 tile = RSDK.GetTile(layerSrc, srcX + x, srcY + y);
            RSDK.SetTile(layerDest, dstX + x, dstY + y, tile);
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

#if GAME_INCLUDE_EDITOR
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

    int32 count = showGizmos() ? 2 : 1;
    for (int32 i = 0; i < count; ++i) {
        if (i >= 1)
            RSDK_DRAWING_OVERLAY(true);

        drawPos.x = positions[i]->x + (size.x >> 1);
        drawPos.y = positions[i]->y + (size.y >> 1);

        DrawHelpers_DrawRectOutline(drawPos.x, drawPos.y, size.x, size.y, 0xFFFF00);

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

        if (i >= 1)
            RSDK_DRAWING_OVERLAY(false);
    }

    if (showGizmos()) {
        // Point to where the tiles come from
        RSDK_DRAWING_OVERLAY(true);
        DrawHelpers_DrawArrow(self->offset.x + (size.x >> 1), self->offset.y + (size.y >> 1), self->position.x + (size.x >> 1),
                              self->position.y + (size.y >> 1), 0xE0E0E0, INK_NONE, 0xFF);
        RSDK_DRAWING_OVERLAY(false);
    }
}

void EncoreRoute_EditorLoad(void)
{
    EncoreRoute->aniFrames = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(EncoreRoute->aniFrames, 0, &EncoreRoute->animator, true, 0);

    RSDK_ACTIVE_VAR(EncoreRoute, layerSrc);
    RSDK_ENUM_VAR("FG Low", EROUTE_LAYER_FGLOW);
    RSDK_ENUM_VAR("FG High", EROUTE_LAYER_FGHIGH);
    RSDK_ENUM_VAR("Move", EROUTE_LAYER_MOVE);
    RSDK_ENUM_VAR("Scratch", EROUTE_LAYER_SCRATCH);

    RSDK_ACTIVE_VAR(EncoreRoute, layerDest);
    RSDK_ENUM_VAR("FG Low", EROUTE_LAYER_FGLOW);
    RSDK_ENUM_VAR("FG High", EROUTE_LAYER_FGHIGH);
    RSDK_ENUM_VAR("Move", EROUTE_LAYER_MOVE);
    RSDK_ENUM_VAR("Scratch", EROUTE_LAYER_SCRATCH);

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
