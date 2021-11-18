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
    self->collision = PLATFORM_C_2;
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

void TilePlatform_EditorDraw(void)
{
    RSDK_THIS(TilePlatform);

    RSDK.DrawLine(self->position.x - self->size.x, self->position.y - self->size.y, self->position.x + self->size.x,
                  self->position.y - self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - self->size.x, self->position.y + self->size.y, self->position.x + self->size.x,
                  self->position.y + self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x - self->size.x, self->position.y - self->size.y, self->position.x - self->size.x,
                  self->position.y + self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(self->position.x + self->size.x, self->position.y - self->size.y, self->position.x + self->size.x,
                  self->position.y + self->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
}

void TilePlatform_EditorLoad(void) {}

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
