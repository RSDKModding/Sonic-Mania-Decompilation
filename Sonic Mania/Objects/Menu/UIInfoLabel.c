#include "SonicMania.h"

ObjectUIInfoLabel *UIInfoLabel;

void UIInfoLabel_Update(void) {}

void UIInfoLabel_LateUpdate(void) {}

void UIInfoLabel_StaticUpdate(void) {}

void UIInfoLabel_Draw(void) { UIInfoLabel_DrawSprites(); }

void UIInfoLabel_Create(void *data)
{
    RSDK_THIS(UIInfoLabel);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    self->width         = self->size.y >> 0x10;
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->animator2, true, 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->text);
    }
}

void UIInfoLabel_StageLoad(void) {}

void UIInfoLabel_SetText(EntityUIInfoLabel *label, char *text)
{
    if (!SceneInfo->inEditor) {
        RSDK.SetText(&label->text, text, 0);
        if (!SceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &label->animator2, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &label->text);
        }
    }
}
void UIInfoLabel_SetString(EntityUIInfoLabel *self, TextInfo *text)
{
    if (!SceneInfo->inEditor) {
        RSDK.CopyString(&self->text, text);
        if (!SceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->animator2, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->text);
        }
    }
}

void UIInfoLabel_DrawSprites(void)
{
    RSDK_THIS(UIInfoLabel);
    Vector2 drawPos;

    int32 size = (self->size.y + self->size.x) >> 16;
    UIWidgets_Unknown7(self->size.y >> 0x10, size, self->width, 240, 240, 240, self->position.x, self->position.y);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    UIWidgets_Unknown7(self->size.y >> 0x10, size, self->width, 0, 0, 0, self->position.x, self->position.y);

    if (SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIInfoLabel->aniFrames, 12, &self->animator2, true, 2);
        drawPos.y -= 0x40000;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
    }
    else {
        drawPos.y -= 0x10000;
        drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->text, 0, self->text.textLength, 0) << 15;
        RSDK.DrawText(&self->animator2, &drawPos, &self->text, 0, self->text.textLength, 0, 0, 0, 0, 0);
    }
}

#if RETRO_INCLUDE_EDITOR
void UIInfoLabel_EditorDraw(void)
{
    RSDK_THIS(UIInfoLabel);
    RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->animator2, true, 0);

    UIInfoLabel_DrawSprites();
}

void UIInfoLabel_EditorLoad(void) {}
#endif

void UIInfoLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIInfoLabel, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIInfoLabel, VAR_STRING, text);
}
