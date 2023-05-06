// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TilePlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTilePlatform *TilePlatform;

void TilePlatform_Update(void) { Platform_Update(); }

void TilePlatform_LateUpdate(void) {}

void TilePlatform_StaticUpdate(void) {}

void TilePlatform_Draw(void)
{
    RSDK_THIS(TilePlatform);

    RSDK.DrawTile(self->tiles, self->size.x >> 20, self->size.y >> 20, &self->drawPos, NULL, false);
}

void TilePlatform_Create(void *data)
{
    RSDK_THIS(TilePlatform);
    self->collision = PLATFORM_C_TILED;
    Platform_Create(NULL);

    if (!SceneInfo->inEditor) {
        self->updateRange.x += 2 * self->size.x;
        self->updateRange.y += 2 * self->size.y;

        self->hitbox.left   = -(self->size.x >> 17);
        self->hitbox.top    = -(self->size.y >> 17);
        self->hitbox.right  = self->size.x >> 17;
        self->hitbox.bottom = self->size.y >> 17;

        self->size.x += 0x80000;
        self->size.y += 0x80000;
        int32 originX = (self->targetPos.x >> 20) - (self->size.x >> 21);
        int32 originY = (self->targetPos.y >> 20) - (self->size.y >> 21);

        int32 h = self->size.y >> 20;
        int32 w = self->size.x >> 20;

        int32 id = 0;
        if ((self->size.y & 0xFFF00000) > 0) {
            for (int32 y = 0; y < h; ++y) {
                if ((self->size.x & 0xFFF00000) > 0) {
                    for (int32 x = 0; x < w; ++x) {
                        id              = x + y * (self->size.x >> 20);
                        self->tiles[id] = RSDK.GetTile(Zone->moveLayer, originX + x, originY + y);
                    }
                }
            }
        }
    }
}

void TilePlatform_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void TilePlatform_EditorDraw(void)
{
    RSDK_THIS(TilePlatform);

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->targetPos.x, self->targetPos.y, self->size.x, self->size.y, 0xFFFF00);

        DrawHelpers_DrawArrow(self->targetPos.x, self->targetPos.y, self->position.x, self->position.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void TilePlatform_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(TilePlatform, type);
    RSDK_ENUM_VAR("Fixed", PLATFORM_FIXED);
    RSDK_ENUM_VAR("Fall", PLATFORM_FALL);
    RSDK_ENUM_VAR("Linear", PLATFORM_LINEAR);
    RSDK_ENUM_VAR("Circular", PLATFORM_CIRCULAR);
    RSDK_ENUM_VAR("Swing", PLATFORM_SWING);
    RSDK_ENUM_VAR("Path", PLATFORM_PATH);
    RSDK_ENUM_VAR("Push", PLATFORM_PUSH);
}
#endif

void TilePlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(TilePlatform, VAR_ENUM, type);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_ENUM, speed);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_BOOL, hasTension);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_VECTOR2, targetPos);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(TilePlatform, VAR_INT32, angle);
}
