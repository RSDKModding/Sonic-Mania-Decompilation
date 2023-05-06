// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIInfoLabel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIInfoLabel *UIInfoLabel;

void UIInfoLabel_Update(void) {}

void UIInfoLabel_LateUpdate(void) {}

void UIInfoLabel_StaticUpdate(void) {}

void UIInfoLabel_Draw(void) { UIInfoLabel_DrawSprites(); }

void UIInfoLabel_Create(void *data)
{
    RSDK_THIS(UIInfoLabel);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    self->width         = FROM_FIXED(self->size.y);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->textAnimator, true, 0);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->text);
    }
}

void UIInfoLabel_StageLoad(void) {}

void UIInfoLabel_SetText(EntityUIInfoLabel *label, char *text)
{
    if (!SceneInfo->inEditor) {
        RSDK.InitString(&label->text, text, 0);
        if (!SceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &label->textAnimator, true, 0);
            RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &label->text);
        }
    }
}
void UIInfoLabel_SetString(EntityUIInfoLabel *entity, String *text)
{
    if (!SceneInfo->inEditor) {
        RSDK.CopyString(&entity->text, text);
        if (!SceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &entity->textAnimator, true, 0);
            RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &entity->text);
        }
    }
}

void UIInfoLabel_DrawSprites(void)
{
    RSDK_THIS(UIInfoLabel);

    Vector2 drawPos;
    int32 size = (self->size.y + self->size.x) >> 16;

    UIWidgets_DrawParallelogram(self->position.x, self->position.y, size, FROM_FIXED(self->size.y), self->width, 0xF0, 0xF0, 0xF0);

    UIWidgets_DrawParallelogram(self->position.x, self->position.y, size, FROM_FIXED(self->size.y), self->width, 0x00, 0x00, 0x00);

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    if (SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIInfoLabel->aniFrames, 12, &self->textAnimator, true, 2);
        drawPos.y -= 0x40000;
        RSDK.DrawSprite(&self->textAnimator, &drawPos, false);
    }
    else {
        drawPos.y -= 0x10000;
        drawPos.x -= RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->text, 0, self->text.length, 0) << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->text, 0, self->text.length, 0, 0, 0, 0, 0);
    }
}

#if GAME_INCLUDE_EDITOR
void UIInfoLabel_EditorDraw(void) { UIInfoLabel_DrawSprites(); }

void UIInfoLabel_EditorLoad(void) { UIInfoLabel->aniFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE); }
#endif

void UIInfoLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIInfoLabel, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIInfoLabel, VAR_STRING, text);
}
