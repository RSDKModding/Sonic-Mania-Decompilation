#include "SonicMania.h"

ObjectBSS_Horizon *BSS_Horizon;

void BSS_Horizon_Update(void) {}

void BSS_Horizon_LateUpdate(void) {}

void BSS_Horizon_StaticUpdate(void) {}

void BSS_Horizon_Draw(void)
{
    RSDK_THIS(BSS_Horizon);
    Vector2 drawPos;
    drawPos.x = ScreenInfo->centerX << 16;
    drawPos.y = 0;

    self->horizonAnimator.frameID = 0;
    self->alpha                   = BSS_Palette->skyAlpha;
    self->direction               = FLIP_NONE;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);

    self->direction = FLIP_X;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);

    self->horizonAnimator.frameID = 1;
    self->alpha                   = BSS_Palette->globeAlpha;
    self->direction               = FLIP_NONE;
    RSDK.DrawSprite(&self->horizonAnimator, &drawPos, true);

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
        self->drawOrder = 2;
        self->drawFX    = FX_FLIP;
        RSDK.SetSpriteAnimation(BSS_Horizon->aniFrames, 0, &self->horizonAnimator, true, 0);
    }
}

void BSS_Horizon_StageLoad(void)
{
    BSS_Horizon->aniFrames = RSDK.LoadSpriteAnimation("SpecialBS/Horizon.bin", SCOPE_STAGE);
    RSDK.ResetEntitySlot(SLOT_BSS_HORIZON, BSS_Horizon->objectID, 0);
}

#if RETRO_INCLUDE_EDITOR
void BSS_Horizon_EditorDraw(void) {}

void BSS_Horizon_EditorLoad(void) {}
#endif

void BSS_Horizon_Serialize(void) {}
