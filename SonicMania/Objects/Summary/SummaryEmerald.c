// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SummaryEmerald Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
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
    self->drawGroup     = 3;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!SceneInfo->inEditor) {
        if (SaveGame_GetEmerald(self->emeraldID))
            RSDK.SetSpriteAnimation(SummaryEmerald->aniFrames, 0, &self->animator, true, self->emeraldID % 7);
        else
            RSDK.SetSpriteAnimation(SummaryEmerald->aniFrames, 0, &self->animator, true, 7);
    }
}

void SummaryEmerald_StageLoad(void) { SummaryEmerald->aniFrames = RSDK.LoadSpriteAnimation("Summary/SummaryEmerald.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void SummaryEmerald_EditorDraw(void)
{
    RSDK_THIS(SummaryEmerald);

    RSDK.SetSpriteAnimation(SummaryEmerald->aniFrames, 0, &self->animator, true, self->emeraldID % 7);

    SummaryEmerald_Draw();
}

void SummaryEmerald_EditorLoad(void)
{
    SummaryEmerald->aniFrames = RSDK.LoadSpriteAnimation("Summary/SummaryEmerald.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(SummaryEmerald, emeraldID);
    RSDK_ENUM_VAR("Green", CHAOSEMERALD_GREEN);
    RSDK_ENUM_VAR("Yellow", CHAOSEMERALD_YELLOW);
    RSDK_ENUM_VAR("Blue", CHAOSEMERALD_BLUE);
    RSDK_ENUM_VAR("Purple", CHAOSEMERALD_PURPLE);
    RSDK_ENUM_VAR("Gray", CHAOSEMERALD_GRAY);
    RSDK_ENUM_VAR("Cyan", CHAOSEMERALD_CYAN);
    RSDK_ENUM_VAR("Red", CHAOSEMERALD_RED);
}
#endif

void SummaryEmerald_Serialize(void) { RSDK_EDITABLE_VAR(SummaryEmerald, VAR_ENUM, emeraldID); }
#endif
