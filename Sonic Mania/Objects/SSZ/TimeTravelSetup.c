// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeTravelSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
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
                ParticleHelpers_SetupParticleFX(Debris_State_Fall, TimeTravelSetup_Particle_CB, 0x1800, player->position.x, player->position.y,
                                                0x200000, 0x200000);
            }
        }
    }

    int32 id = (TimeTravelSetup->timer >> 8) + 1;
    if (id > 2)
        id = (TimeTravelSetup->timer >> 8) - 2;

    RSDK.SetLimitedFade(0, (TimeTravelSetup->timer >> 8) + 1, (id + 1) & 0xFF, TimeTravelSetup->timer & 0xFF, 128, 135);
    TimeTravelSetup->timer += 4;
    TimeTravelSetup->timer %= 768;

    TileLayer *background1 = RSDK.GetSceneLayer(0);
    background1->deformationOffset += 2;

    for (int32 i = 0; i < 0x200; ++i) {
        int32 deform                            = (2 * RSDK.Sin256(i << 6) >> 8) + (2 * RSDK.Sin256(8 * i) >> 7);
        background1->deformationData[i]         = deform;
        background1->deformationData[i + 0x200] = deform;
    }

    TileLayer *background2 = RSDK.GetSceneLayer(1);
    background2->deformationOffset += 2;

    for (int32 i = 0; i < 0x200; ++i) {
        int32 deform                            = (2 * RSDK.Sin256(2 * i) >> 7) + (2 * (RSDK.Sin256(2 * i)) >> 7);
        background2->deformationData[i]         = deform;
        background2->deformationData[i + 0x200] = deform;
    }
}

void TimeTravelSetup_Draw(void) {}

void TimeTravelSetup_Create(void *data) {}

void TimeTravelSetup_StageLoad(void)
{
    TileLayer *background1    = RSDK.GetSceneLayer(0);
    background1->drawLayer[0] = 0;
    background1->scrollSpeed  = -0x60000;

    TileLayer *background2    = RSDK.GetSceneLayer(1);
    background2->drawLayer[0] = 0;
    background2->scrollSpeed  = -0xA0000;

    TimeTravelSetup->aniFrames = RSDK.LoadSpriteAnimation("SSZ1/TTSparkle.bin", SCOPE_STAGE);
}

void TimeTravelSetup_Particle_CB(EntityDebris *debris)
{
    RSDK.SetSpriteAnimation(TimeTravelSetup->aniFrames, 0, &debris->animator, true, 0);
    debris->updateRange.x = 0x800000;
    debris->updateRange.y = 0x800000;
    debris->drawOrder     = Zone->objectDrawHigh;
}

#if RETRO_INCLUDE_EDITOR
void TimeTravelSetup_EditorDraw(void) {}

void TimeTravelSetup_EditorLoad(void) {}
#endif

void TimeTravelSetup_Serialize(void) {}
