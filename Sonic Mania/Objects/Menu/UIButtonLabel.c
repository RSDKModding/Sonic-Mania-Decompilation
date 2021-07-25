#include "SonicMania.h"

ObjectUIButtonLabel *UIButtonLabel;

void UIButtonLabel_Update(void)
{
    RSDK_THIS(UIButtonLabel);

    if (entity->textSprite != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator2, true, entity->frameID);
        entity->textSprite = UIWidgets->textSpriteIndex;
    }
}

void UIButtonLabel_LateUpdate(void) {}

void UIButtonLabel_StaticUpdate(void) {}

void UIButtonLabel_Draw(void)
{
    RSDK_THIS(UIButtonLabel);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);
}

void UIButtonLabel_Create(void *data)
{
    RSDK_THIS(UIButtonLabel);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x300000;
    RSDK.SetSpriteAnimation(UIButtonLabel->aniFrames, 0, &entity->animator1, true, entity->type);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator2, true, entity->frameID);
    entity->textSprite = UIWidgets->textSpriteIndex;
}

void UIButtonLabel_StageLoad(void) { UIButtonLabel->aniFrames = RSDK.LoadSpriteAnimation("UI/ButtonLabel.bin", SCOPE_STAGE); }

void UIButtonLabel_EditorDraw(void) {}

void UIButtonLabel_EditorLoad(void) {}

void UIButtonLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButtonLabel, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(UIButtonLabel, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIButtonLabel, VAR_ENUM, frameID);
}
