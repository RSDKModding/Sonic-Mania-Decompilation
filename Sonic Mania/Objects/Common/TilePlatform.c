// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TilePlatform Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    self->collision = PLATFORM_C_USE_TILES;
    Platform_Create(NULL);
    if (!SceneInfo->inEditor) {
        self->hitbox.bottom = self->size.y >> 17;
        self->hitbox.right  = self->size.x >> 17;
        self->updateRange.x += 2 * self->size.x;
        self->updateRange.y += 2 * self->size.y;
        self->hitbox.top  = -(self->size.y >> 17);
        self->hitbox.left = -self->size.x >> 17;
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
                        id                = x + y * (self->size.x >> 20);
                        self->tiles[id] = RSDK.GetTileInfo(Zone->moveLayer, originX + x, originY + y);
                    }
                }
            }
        }
    }
}

void TilePlatform_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void TilePlatform_EditorDraw(void)
{
    RSDK_THIS(TilePlatform);

    RSDK_DRAWING_OVERLAY(true);
    DrawHelpers_DrawRectOutline(0xFFFF00, self->position.x, self->position.y, self->size.x, self->size.y);
    RSDK_DRAWING_OVERLAY(false);
}

void TilePlatform_EditorLoad(void) {

    RSDK_ACTIVE_VAR(TilePlatform, type);
    RSDK_ENUM_VAR("Fixed", PLATFORM_FIXED);
    RSDK_ENUM_VAR("Collapsing", PLATFORM_COLLAPSING);
    RSDK_ENUM_VAR("Moving (Smooth)", PLATFORM_MOVING);
    RSDK_ENUM_VAR("Circlular", PLATFORM_CIRCULAR);
    RSDK_ENUM_VAR("Swinging", PLATFORM_SWINGING);
    RSDK_ENUM_VAR("Controlled", PLATFORM_CONTROLLED);
    RSDK_ENUM_VAR("Pushable", PLATFORM_PUSHABLE);
    RSDK_ENUM_VAR("Moving (Static)", PLATFORM_TRACK);
    RSDK_ENUM_VAR("Wait for Player", PLATFORM_WAIT);
    RSDK_ENUM_VAR("Wait for Player (Oscillate)", PLATFORM_WAIT_OSC);
    RSDK_ENUM_VAR("Activate when Above", PLATFORM_ACTIVEABOVE);
    RSDK_ENUM_VAR("Controlled (Activates PlatformControl)", PLATFORM_CONT_ACTIVATER);
    RSDK_ENUM_VAR("Wait for Player, then Move with Arc", PLATFORM_WAIT_ARC);
    RSDK_ENUM_VAR("Sticky", PLATFORM_STICKY);
    RSDK_ENUM_VAR("Swinging (Clackers)", PLATFORM_SWING_CLACK);
    RSDK_ENUM_VAR("Static", PLATFORM_STATIC);
    RSDK_ENUM_VAR("Wait for Player, then Sink", PLATFORM_SINKER);
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
