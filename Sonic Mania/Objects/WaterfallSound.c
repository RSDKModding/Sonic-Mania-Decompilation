#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectWaterfallSound *WaterfallSound;

void WaterfallSound_Update(void) {}

void WaterfallSound_LateUpdate(void) {}

void WaterfallSound_StaticUpdate(void) {}

void WaterfallSound_Draw(void)
{
    RSDK_THIS(WaterfallSound);

    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x - TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    RSDK.DrawLine(entity->position.x + TILE_SIZE * entity->size.x, entity->position.y - TILE_SIZE * entity->size.y,
                  entity->position.x + TILE_SIZE * entity->size.x, entity->position.y + TILE_SIZE * entity->size.y, 0xFFFF00, 0xFF, INK_NONE, false);
    if (entity->flag)
        PlayerProbe_DrawDebug5(entity->posUnk.x, entity->posUnk.y);
}

void WaterfallSound_Create(void *data)
{
    RSDK_THIS(WaterfallSound);
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = 16 * entity->size.x;
        entity->updateRange.y = 16 * entity->size.y;
        entity->visible       = true;
        entity->active        = ACTIVE_NEVER;
        entity->posUnk.x      = 0;
        entity->posUnk.y      = 0;
        entity->drawOrder     = Zone->drawOrderHigh;
    }
}

void WaterfallSound_StageLoad(void)
{
    if (globals->gameMode != MODE_COMPETITION) {
        Soundboard_LoadSFX("Stage/WaterfallLoop.wav", 2820, WaterfallSound_CheckCB, WaterfallSound_ChangeCB);
    }
}

bool32 WaterfallSound_CheckCB(void) { return false; }
void WaterfallSound_ChangeCB(int sfxID) {}

void WaterfallSound_EditorDraw(void) {}

void WaterfallSound_EditorLoad(void) {}

void WaterfallSound_Serialize(void) { RSDK_EDITABLE_VAR(WaterfallSound, VAR_VECTOR2, size); }
#endif