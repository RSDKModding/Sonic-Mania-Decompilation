#include "SonicMania.h"

ObjectChaosEmerald *ChaosEmerald;

void ChaosEmerald_Update(void)
{
    RSDK_THIS(ChaosEmerald);
    StateMachine_Run(entity->state);
}

void ChaosEmerald_LateUpdate(void) {}

void ChaosEmerald_StaticUpdate(void) {}

void ChaosEmerald_Draw(void)
{
    RSDK_THIS(ChaosEmerald);

    for (int32 i = 0; i < 36; ++i) {
        ChaosEmerald->colourStorage[i] = RSDK.GetPaletteEntry(0, i + 128);
        RSDK.SetPaletteEntry(0, i + 128, ChaosEmerald->emeraldColours[i]);
    }

    RSDK.DrawSprite(&entity->animator, NULL, false);

    for (int32 i = 0; i < 36; ++i) {
        RSDK.SetPaletteEntry(0, i + 0x80, ChaosEmerald->colourStorage[i]);
    }
}

void ChaosEmerald_Create(void *data)
{
    RSDK_THIS(ChaosEmerald);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderHigh;
    entity->originPos     = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->state         = ChaosEmerald_State_Unknown1;
    RSDK.SetSpriteAnimation(ChaosEmerald->aniFrames, 0, &entity->animator, true, entity->type);
}

void ChaosEmerald_StageLoad(void) { ChaosEmerald->aniFrames = RSDK.LoadSpriteAnimation("Cutscene/Emeralds.bin", SCOPE_STAGE); }

void ChaosEmerald_State_Unknown1(void)
{
    // hello
}
void ChaosEmerald_State_Rotate(void)
{
    RSDK_THIS(ChaosEmerald);
    entity->angle += entity->groundVel;
    entity->position.x = entity->radius * RSDK.Cos256(entity->angle >> 8) + entity->originPos.x;
    entity->position.y = entity->radius * RSDK.Sin256(entity->angle >> 8) + entity->originPos.y;
}

void ChaosEmerald_EditorDraw(void)
{
    RSDK_THIS(ChaosEmerald);
    RSDK.SetSpriteAnimation(ChaosEmerald->aniFrames, 0, &entity->animator, true, entity->type);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void ChaosEmerald_EditorLoad(void) { ChaosEmerald->aniFrames = RSDK.LoadSpriteAnimation("Cutscene/Emeralds.bin", SCOPE_STAGE); }

void ChaosEmerald_Serialize(void) { RSDK_EDITABLE_VAR(ChaosEmerald, VAR_UINT8, type); }
