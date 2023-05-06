// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIButtonLabel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIButtonLabel *UIButtonLabel;

void UIButtonLabel_Update(void)
{
    RSDK_THIS(UIButtonLabel);

    if (self->textSprite != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->textAnimator, true, self->frameID);
        self->textSprite = UIWidgets->textFrames;
    }
}

void UIButtonLabel_LateUpdate(void) {}

void UIButtonLabel_StaticUpdate(void) {}

void UIButtonLabel_Draw(void)
{
    RSDK_THIS(UIButtonLabel);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    RSDK.DrawSprite(&self->frameAnimator, &drawPos, false);
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
}

void UIButtonLabel_Create(void *data)
{
    RSDK_THIS(UIButtonLabel);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    RSDK.SetSpriteAnimation(UIButtonLabel->aniFrames, 0, &self->frameAnimator, true, self->type);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->textAnimator, true, self->frameID);

    self->textSprite = UIWidgets->textFrames;
}

void UIButtonLabel_StageLoad(void) { UIButtonLabel->aniFrames = RSDK.LoadSpriteAnimation("UI/ButtonLabel.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void UIButtonLabel_EditorDraw(void)
{
    RSDK_THIS(UIButtonLabel);

    self->drawGroup     = 2;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    RSDK.SetSpriteAnimation(UIButtonLabel->aniFrames, 0, &self->frameAnimator, true, self->type);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->listID, &self->textAnimator, true, self->frameID);

    self->textSprite = UIWidgets->textFrames;

    UIButtonLabel_Draw();
}

void UIButtonLabel_EditorLoad(void) { UIButtonLabel->aniFrames = RSDK.LoadSpriteAnimation("UI/ButtonLabel.bin", SCOPE_STAGE); }
#endif

void UIButtonLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButtonLabel, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(UIButtonLabel, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIButtonLabel, VAR_ENUM, frameID);
}
