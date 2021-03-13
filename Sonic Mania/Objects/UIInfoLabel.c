#include "../SonicMania.h"

ObjectUIInfoLabel *UIInfoLabel;

void UIInfoLabel_Update() {}

void UIInfoLabel_LateUpdate() {}

void UIInfoLabel_StaticUpdate() {}

void UIInfoLabel_Draw() { UIInfoLabel_DrawSprites(); }

void UIInfoLabel_Create(void *data)
{
    EntityUIInfoLabel *entity = (EntityUIInfoLabel *)RSDK_sceneInfo->entity;
    entity->active            = ACTIVE_BOUNDS;
    entity->drawOrder         = 2;
    entity->visible           = 1;
    entity->drawFX            = FX_FLIP;
    entity->updateRange.x     = 0x800000;
    entity->updateRange.y     = 0x300000;
    entity->width             = entity->size.x >> 0x10;
    if (RSDK_sceneInfo->inEditor == false) {
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->data2, true, 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->text);
    }
}

void UIInfoLabel_StageLoad() {}

void UIInfoLabel_EditorDraw() {}

void UIInfoLabel_Unknown1(EntityUIInfoLabel *label, char *text)
{
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(&label->text, text, 0);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &label->data2, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &label->text);
        }
    }
}
void UIInfoLabel_Unknown2(EntityUIInfoLabel *entity, char *text)
{
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.Unknown64(&entity->text, text);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->data2, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->text);
        }
    }
}

void UIInfoLabel_DrawSprites()
{
    Vector2 drawPos;

    EntityUIInfoLabel *entity = (EntityUIInfoLabel *)RSDK_sceneInfo->entity;
    int size                  = (entity->size.y + entity->size.x) >> 16;
    UIWidgets_Unknown7(entity->size.y >> 0x10, size, entity->width, 240, 240, 240, entity->position.x, entity->position.y);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    UIWidgets_Unknown7(entity->size.y >> 0x10, size, entity->width, 0, 0, 0, entity->position.x, entity->position.y);

    if (RSDK_sceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIInfoLabel->spriteIndex, 12, &entity->data2, true, 2);
        drawPos.y -= 0x40000;
        RSDK.DrawSprite(&entity->data2, &drawPos, 0);
    }
    else {
        drawPos.y -= 0x10000;
        drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->text, 0, entity->text.textLength, 0) << 15;
        RSDK.DrawText(&entity->data2, &drawPos, &entity->text, 0, entity->text.textLength, 0, 0, 0, 0, 0);
    }
}

void UIInfoLabel_EditorLoad() {}

void UIInfoLabel_Serialize() {}
