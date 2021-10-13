#include "SonicMania.h"

ObjectUIInfoLabel *UIInfoLabel;

void UIInfoLabel_Update(void) {}

void UIInfoLabel_LateUpdate(void) {}

void UIInfoLabel_StaticUpdate(void) {}

void UIInfoLabel_Draw(void) { UIInfoLabel_DrawSprites(); }

void UIInfoLabel_Create(void *data)
{
    RSDK_THIS(UIInfoLabel);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x300000;
    entity->width         = entity->size.y >> 0x10;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator2, true, 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->text);
    }
}

void UIInfoLabel_StageLoad(void) {}

void UIInfoLabel_EditorDraw(void) {}

void UIInfoLabel_SetText(EntityUIInfoLabel *label, char *text)
{
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(&label->text, text, 0);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &label->animator2, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &label->text);
        }
    }
}
void UIInfoLabel_SetString(EntityUIInfoLabel *entity, TextInfo *text)
{
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.CopyString(&entity->text, text);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator2, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->text);
        }
    }
}

void UIInfoLabel_DrawSprites(void)
{
    RSDK_THIS(UIInfoLabel);
    Vector2 drawPos;

    int32 size = (entity->size.y + entity->size.x) >> 16;
    UIWidgets_Unknown7(entity->size.y >> 0x10, size, entity->width, 240, 240, 240, entity->position.x, entity->position.y);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    UIWidgets_Unknown7(entity->size.y >> 0x10, size, entity->width, 0, 0, 0, entity->position.x, entity->position.y);

    if (RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIInfoLabel->spriteIndex, 12, &entity->animator2, true, 2);
        drawPos.y -= 0x40000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, 0);
    }
    else {
        drawPos.y -= 0x10000;
        drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->text, 0, entity->text.textLength, 0) << 15;
        RSDK.DrawText(&entity->animator2, &drawPos, &entity->text, 0, entity->text.textLength, 0, 0, 0, 0, 0);
    }
}

void UIInfoLabel_EditorLoad(void) {}

void UIInfoLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIInfoLabel, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIInfoLabel, VAR_STRING, text);
}
