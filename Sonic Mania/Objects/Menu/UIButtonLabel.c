#include "SonicMania.h"

ObjectUIButtonLabel *UIButtonLabel;

void UIButtonLabel_Update(void)
{
    RSDK_THIS(UIButtonLabel);

    if (self->textSprite != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->animator2, true, self->frameID);
        self->textSprite = UIWidgets->textSpriteIndex;
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
    RSDK.DrawSprite(&self->animator1, &drawPos, false);
    RSDK.DrawSprite(&self->animator2, &drawPos, false);
}

void UIButtonLabel_Create(void *data)
{
    RSDK_THIS(UIButtonLabel);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    RSDK.SetSpriteAnimation(UIButtonLabel->aniFrames, 0, &self->animator1, true, self->type);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->animator2, true, self->frameID);
    self->textSprite = UIWidgets->textSpriteIndex;
}

void UIButtonLabel_StageLoad(void) { UIButtonLabel->aniFrames = RSDK.LoadSpriteAnimation("UI/ButtonLabel.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void UIButtonLabel_EditorDraw(void)
{
    RSDK_THIS(UIButtonLabel);

    self->drawOrder     = 2;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    RSDK.SetSpriteAnimation(UIButtonLabel->aniFrames, 0, &self->animator1, true, self->type);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, self->listID, &self->animator2, true, self->frameID);
    self->textSprite = UIWidgets->textSpriteIndex;

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
