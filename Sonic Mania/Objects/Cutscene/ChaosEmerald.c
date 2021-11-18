#include "SonicMania.h"

ObjectChaosEmerald *ChaosEmerald;

void ChaosEmerald_Update(void)
{
    RSDK_THIS(ChaosEmerald);
    StateMachine_Run(self->state);
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

    RSDK.DrawSprite(&self->animator, NULL, false);

    for (int32 i = 0; i < 36; ++i) {
        RSDK.SetPaletteEntry(0, i + 0x80, ChaosEmerald->colourStorage[i]);
    }
}

void ChaosEmerald_Create(void *data)
{
    RSDK_THIS(ChaosEmerald);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = Zone->drawOrderHigh;
    self->originPos     = self->position;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = ChaosEmerald_State_Unknown1;
    RSDK.SetSpriteAnimation(ChaosEmerald->aniFrames, 0, &self->animator, true, self->type);
}

void ChaosEmerald_StageLoad(void) { ChaosEmerald->aniFrames = RSDK.LoadSpriteAnimation("Cutscene/Emeralds.bin", SCOPE_STAGE); }

void ChaosEmerald_State_Unknown1(void)
{
    // hello
}
void ChaosEmerald_State_Rotate(void)
{
    RSDK_THIS(ChaosEmerald);
    self->angle += self->groundVel;
    self->position.x = self->radius * RSDK.Cos256(self->angle >> 8) + self->originPos.x;
    self->position.y = self->radius * RSDK.Sin256(self->angle >> 8) + self->originPos.y;
}

#if RETRO_INCLUDE_EDITOR
void ChaosEmerald_EditorDraw(void)
{
    RSDK_THIS(ChaosEmerald);
    RSDK.SetSpriteAnimation(ChaosEmerald->aniFrames, 0, &self->animator, true, self->type);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void ChaosEmerald_EditorLoad(void) { ChaosEmerald->aniFrames = RSDK.LoadSpriteAnimation("Cutscene/Emeralds.bin", SCOPE_STAGE); }
#endif

void ChaosEmerald_Serialize(void) { RSDK_EDITABLE_VAR(ChaosEmerald, VAR_UINT8, type); }
