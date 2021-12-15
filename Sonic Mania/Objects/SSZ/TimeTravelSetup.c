// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeTravelSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectTimeTravelSetup *TimeTravelSetup;

void TimeTravelSetup_Update(void) {}

void TimeTravelSetup_LateUpdate(void) {}

void TimeTravelSetup_StaticUpdate(void)
{
    if (!(Zone->timer % 5)) {
        foreach_all(Player, player)
        {
            Vector2 pos;
            pos.x = 0;
            pos.y = 0;
            if (RSDK.CheckOnScreen(player, &pos)) {
                ParticleHelpers_SetupParticleFX(Debris_State_Fall, TimeTravelSetup_Particle_CB, 0x1800, player->position.x, player->position.y, 0x200000,
                                         0x200000);
            }
        }
    }

    int32 id = (TimeTravelSetup->timer >> 8) + 1;
    if (id > 2)
        id = (TimeTravelSetup->timer >> 8) - 2;
    RSDK.SetLimitedFade(0, (TimeTravelSetup->timer >> 8) + 1, (id + 1) & 0xFF, TimeTravelSetup->timer & 0xFF, 128, 135);
    TimeTravelSetup->timer += 4;
    TimeTravelSetup->timer %= 768;

    TileLayer *layer = RSDK.GetSceneLayer(0);
    layer->deformationOffset += 2;
    int32 *deformData = layer->deformationData;

    for (int32 i = 0; i < 0x200; ++i) {
        int32 val               = (2 * RSDK.Sin256(i << 6) >> 8) + (2 * RSDK.Sin256(8 * i) >> 7);
        deformData[i]         = val;
        deformData[i + 0x200] = val;
    }

    layer = RSDK.GetSceneLayer(1);
    layer->deformationOffset += 2;
    deformData = layer->deformationData;

    for (int32 i = 0; i < 0x200; ++i) {
        int32 val               = (2 * RSDK.Sin256(2 * i) >> 7) + (2 * (RSDK.Sin256(2 * i)) >> 7);
        deformData[i]         = val;
        deformData[i + 0x200] = val;
    }
}

void TimeTravelSetup_Draw(void) {}

void TimeTravelSetup_Create(void *data) {}

void TimeTravelSetup_StageLoad(void)
{
    TileLayer *layer    = RSDK.GetSceneLayer(0);
    layer->drawLayer[0] = 0;
    layer->scrollSpeed  = -0x60000;

    layer               = RSDK.GetSceneLayer(1);
    layer->drawLayer[0] = 0;
    layer->scrollSpeed  = -0xA0000;

    TimeTravelSetup->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/TTSparkle.bin", SCOPE_STAGE);
}

void TimeTravelSetup_Particle_CB(EntityDebris *debris)
{
    RSDK.SetSpriteAnimation(TimeTravelSetup->aniFrames, 0, &debris->animator, true, 0);
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;
    debris->drawOrder     = Zone->drawOrderHigh;
}

#if RETRO_INCLUDE_EDITOR
void TimeTravelSetup_EditorDraw(void) {}

void TimeTravelSetup_EditorLoad(void) {}
#endif

void TimeTravelSetup_Serialize(void) {}
