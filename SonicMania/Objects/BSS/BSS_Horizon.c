// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: BSS_Horizon Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBSS_Horizon *BSS_Horizon;

void BSS_Horizon_Update(void) {}

void BSS_Horizon_LateUpdate(void) {}

void BSS_Horizon_StaticUpdate(void) {}

void BSS_Horizon_Draw(void)
{
    RSDK_THIS(BSS_Horizon);

    Vector2 drawPos;
    drawPos.x = TO_FIXED(ScreenInfo->center.x);
    drawPos.y = 0;

    // Draw Sky Horizon (Left)
    self->horizonAnimator.frameID = 0;
    self->alpha                   = BSS_Palette->skyAlpha;
    self->direction               = FLIP_NONE;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);

    // Draw Sky Horizon (Right)
    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);

    // Draw Globe Horizon (Left)
    self->horizonAnimator.frameID = 1;
    self->alpha                   = BSS_Palette->globeAlpha;
    self->direction               = FLIP_NONE;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);

    // Draw Globe Horizon (Right)
    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);
}

void BSS_Horizon_Create(void *data)
{
    RSDK_THIS(BSS_Horizon);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->inkEffect = INK_ADD;
        self->visible   = true;
        self->drawGroup = 2;
        self->drawFX    = FX_FLIP;

        RSDK.SetSpriteAnimation(BSS_Horizon->aniFrames, 0, &self->horizonAnimator, true, 0);
    }
}

void BSS_Horizon_StageLoad(void)
{
    BSS_Horizon->aniFrames = RSDK.LoadSpriteAnimation("SpecialBS/Horizon.bin", SCOPE_STAGE);

    RSDK.ResetEntitySlot(SLOT_BSS_HORIZON, BSS_Horizon->classID, NULL);
}

#if GAME_INCLUDE_EDITOR
void BSS_Horizon_EditorDraw(void) {}

void BSS_Horizon_EditorLoad(void) {}
#endif

void BSS_Horizon_Serialize(void) {}
