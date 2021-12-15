// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SummaryEmerald Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectSummaryEmerald *SummaryEmerald;

void SummaryEmerald_Update(void) {}

void SummaryEmerald_LateUpdate(void) {}

void SummaryEmerald_StaticUpdate(void) {}

void SummaryEmerald_Draw(void)
{
    RSDK_THIS(SummaryEmerald);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void SummaryEmerald_Create(void *data)
{
    RSDK_THIS(SummaryEmerald);
    self->active        = ACTIVE_NORMAL;
    self->drawOrder     = 3;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    if (!SceneInfo->inEditor) {
        if ((1 << self->emeraldID) & SaveGame->saveRAM->chaosEmeralds)
            RSDK.SetSpriteAnimation(SummaryEmerald->aniFrames, 0, &self->animator, true, self->emeraldID % 7);
        else
            RSDK.SetSpriteAnimation(SummaryEmerald->aniFrames, 0, &self->animator, true, 7);
    }
}

void SummaryEmerald_StageLoad(void) { SummaryEmerald->aniFrames = RSDK.LoadSpriteAnimation("Summary/SummaryEmerald.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void SummaryEmerald_EditorDraw(void) {}

void SummaryEmerald_EditorLoad(void) {}
#endif

void SummaryEmerald_Serialize(void) { RSDK_EDITABLE_VAR(SummaryEmerald, VAR_ENUM, emeraldID); }
#endif
