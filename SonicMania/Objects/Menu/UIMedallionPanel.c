// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIMedallionPanel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIMedallionPanel *UIMedallionPanel;

void UIMedallionPanel_Update(void) {}

void UIMedallionPanel_LateUpdate(void) {}

void UIMedallionPanel_StaticUpdate(void) {}

void UIMedallionPanel_Draw(void)
{
    if (!API_GetNoSave())
        UIMedallionPanel_DrawPanel();
}

void UIMedallionPanel_Create(void *data)
{
    RSDK_THIS(UIMedallionPanel);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
}

void UIMedallionPanel_StageLoad(void) { UIMedallionPanel->aniFrames = RSDK.LoadSpriteAnimation("UI/MedallionPanel.bin", SCOPE_STAGE); }

void UIMedallionPanel_DrawPanel(void)
{
    RSDK_THIS(UIMedallionPanel);
    Vector2 drawPos;

    ProgressRAM *progress = GameProgress_GetGameProgress();
    for (int32 m = 0; m < GAMEPROGRESS_MEDAL_COUNT; ++m) {
        drawPos.x = (self->position.x - 0x310000) + 0xE0000 * (m % 8);
        drawPos.y = (self->position.y - 0x150000) + 0xE0000 * (m / 8);

        if (SceneInfo->inEditor)
            RSDK.SetSpriteAnimation(UIMedallionPanel->aniFrames, 0, &self->animator, true, (m & 1) + 1);
        else if (progress)
            RSDK.SetSpriteAnimation(UIMedallionPanel->aniFrames, 0, &self->animator, true, progress->medals[m]);
        else
            RSDK.SetSpriteAnimation(UIMedallionPanel->aniFrames, 0, &self->animator, true, 0);

        if (!SceneInfo->inEditor && !self->animator.frameID)
            RSDK.DrawCircleOutline(drawPos.x, drawPos.y, 5, 6, 0x000000, 0xFF, INK_BLEND, false);

        drawPos.x += 0x10000;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
    }
}

#if RETRO_INCLUDE_EDITOR
void UIMedallionPanel_EditorDraw(void) { UIMedallionPanel_DrawPanel(); }

void UIMedallionPanel_EditorLoad(void) { UIMedallionPanel->aniFrames = RSDK.LoadSpriteAnimation("UI/MedallionPanel.bin", SCOPE_STAGE); }
#endif

void UIMedallionPanel_Serialize(void) {}
