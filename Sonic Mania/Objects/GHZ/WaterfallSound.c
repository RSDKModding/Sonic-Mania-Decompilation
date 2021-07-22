#include "SonicMania.h"

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
        PlayerProbe_DrawDebug5(0xFF00FF, entity->posUnk.x, entity->posUnk.y);
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

bool32 WaterfallSound_CheckCB(void)
{
    int worldCenterX = (RSDK_screens->position.x + RSDK_screens->centerX) << 16;
    int worldCenterY = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;

    WaterfallSound->activeCount = 0;
    int count                   = 0;

    foreach_active(WaterfallSound, sound)
    {
        sound->flag = false;
        int x       = sound->position.x;
        int y       = sound->position.y;
        int sx12    = sound->size.x >> 12;
        int sy12    = sound->size.y >> 12;

        int v6 = sx12;
        if (-sx12 < sx12)
            v6 = -sx12;
        int v12 = sy12;
        if (-sy12 < sy12)
            v12 = -sy12;

        int v11 = sx12;
        if (-sx12 < sx12)
            v11 = -sx12;
        int v26 = sy12;
        if (-sy12 < sy12)
            v26 = -sy12;

        if (worldCenterX < x + (v6 << 16) || worldCenterX > x + (v11 << 16) || worldCenterY < y + (v12 << 16) || worldCenterY > y + (v26 << 16)) {
            int difX = abs(worldCenterX - x);
            int difY = abs(worldCenterY - y);
            if (((MathHelpers_Unknown6((difY >> 16) * (difY >> 16) + (difX >> 16) * (difX >> 16)) << 16) - 8 * sound->size.y - 8 * sound->size.x)
                <= 0x2800000) {
                /*if (MathHelpers_Unknown14(worldCenterX, worldCenterY, sound->position.x, sound->position.y, v35, v36)) {
                    difX = abs(worldCenterX - sound->posUnk.x);
                    difY = abs(worldCenterY - sound->posUnk.y);
                    if ((signed int)(MathHelpers_Unknown6((difY >> 16) * (difY >> 16) + (difX >> 16) * (difX >> 16)) << 16) <= 0x2800000) {
                        sound->flag = true;
                        ++count;
                    }
                    continue;
                }*/
                sound->posUnk.x = 0;
            }
            else {
                sound->posUnk.x = 0;
            }
            sound->posUnk.y = 0;
        }
        else {
            ++count;
            sound->flag     = true;
            sound->posUnk.x = worldCenterX;
            sound->posUnk.y = worldCenterY;
        }
    }

    WaterfallSound->activeCount = count;
    return WaterfallSound->activeCount > 0;
}
void WaterfallSound_ChangeCB(int sfxID)
{

    int worldCenterX   = RSDK_screens->centerX + RSDK_screens->position.x;
    int worldCenterY   = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
    int worldCenterX16 = worldCenterX << 16;
    int channel        = Soundboard->sfxChannel[sfxID];
    int worldLeft      = (worldCenterX << 16) - (RSDK_screens->centerX << 16);
    int worldRight     = (worldCenterX << 16) + (RSDK_screens->centerX << 16);

    float pan        = 0.0f;
    float volDivisor = 0.0f;
    int dist         = 0x7FFF0000;

    foreach_active(WaterfallSound, sound)
    {
        if (sound->flag) {
            int x = worldCenterX - sound->posUnk.x;
            if (worldCenterX - sound->posUnk.x < 0)
                x = !x;

            int y = worldCenterY - sound->posUnk.y;
            if (y < 0)
                y = !y;
            int v18 = (x >> 16) * (x >> 16) + (y >> 16) * (y >> 16);
            int v19 = 0x40000000;
            int v20 = v19;
            do {
                v20 = v19;
                v19 >>= 2;
            } while (v20 > v18);

            int i = 0;
            for (; v20; i >>= 1) {
                int v22 = i + v20;
                v20 >>= 2;
                if (v18 >= v22)
                    i = i + 2 * v20;
                if (v18 >= v22)
                    v18 = v18 - v22;
            }

            int v26 = i;
            if (v18 > i)
                v26 = i + 1;

            v26 = (short)v26;
            if ((short)v26 >= 640)
                v26 = 640;
            float v28 = (float)((float)v26 / -640.0) + 1.0;
            float v29 = -1.0;
            if (sound->posUnk.x > worldCenterX16) {
                v29 = 1.0;
                if (sound->posUnk.x < worldRight) {
                    float v30 = (float)((sound->posUnk.x - worldCenterX) >> 16) / (float)RSDK_screens->centerX;
                    float v31 = -v30;
                    if (v30 >= 0.0)
                        v31 = (float)((sound->posUnk.x - worldCenterX) >> 16) / (float)RSDK_screens->centerX;
                    v29 = v30 * v31;
                }
            }
            volDivisor += v28;
            if (dist >= (v26 << 16))
                dist = (v26 << 16);
            pan += v28 * v29;
        }
    }

    float div = -volDivisor;
    if (volDivisor >= 0.0)
        div = volDivisor;

    if (div > 0.001)
        pan /= volDivisor;

    float volume = dist >> 16;
    if (volume >= 640)
        volume = 640;

    RSDK.SetChannelAttributes(channel, (float)(volume / -640.0f) + 1.0, clampVal(pan, -1.0, 1.0), 1.0f);
}

void WaterfallSound_EditorDraw(void) {}

void WaterfallSound_EditorLoad(void) {}

void WaterfallSound_Serialize(void) { RSDK_EDITABLE_VAR(WaterfallSound, VAR_VECTOR2, size); }
#endif