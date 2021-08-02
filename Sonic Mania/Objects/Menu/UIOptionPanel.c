#include "SonicMania.h"

ObjectUIOptionPanel *UIOptionPanel;

void UIOptionPanel_Update(void)
{
    RSDK_THIS(UIOptionPanel);
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->topListID, &entity->animator, true, entity->topFrameID);
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->botListID, &entity->animator2, true, entity->botFrameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
}

void UIOptionPanel_LateUpdate(void) {}

void UIOptionPanel_StaticUpdate(void) {}

void UIOptionPanel_Draw(void)
{
    RSDK_THIS(UIOptionPanel);
    UIOptionPanel_Unknown1();
    RSDK.DrawSprite(&entity->animator, &entity->drawPosTop, false);
    if (!entity->botHidden)
        RSDK.DrawSprite(&entity->animator2, &entity->drawPosBottom, false);
}

void UIOptionPanel_Create(void *data)
{
    RSDK_THIS(UIOptionPanel);
    entity->field_74      = entity->position.x;
    entity->field_78      = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->topListID, &entity->animator, true, entity->topFrameID);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->botListID, &entity->animator2, true, entity->botFrameID);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    if (entity->panelSize < 136)
        entity->panelSize = 136;

    entity->field_7C.y   = entity->panelSize << 16;
    entity->drawPosTop.x = entity->position.x;
    entity->drawPosTop.x -= 0x920000;
    entity->drawPosTop.y = entity->position.y;
    entity->field_7C.x   = 0x1A80000;
    entity->field_94     = -0x4300D4;
    entity->field_98     = 0x4400D4;
    entity->drawPosTop.y += -0x370000 - (((entity->panelSize << 15) - 0x438001) & 0xFFFF0000);
    entity->drawPosBottom = entity->position;
    if (!entity->botAlignRight)
        entity->drawPosBottom.x -= 0xA0000;
    else
        entity->drawPosBottom.x += 0xA20000;
    entity->drawPosBottom.y += 0x390000;
}

void UIOptionPanel_StageLoad(void) {}

void UIOptionPanel_Unknown1(void)
{
    RSDK_THIS(UIOptionPanel);
    RSDK.DrawRect(entity->position.x - (entity->field_7C.x >> 1), entity->position.y - (entity->field_7C.y >> 1), entity->field_7C.x,
                  entity->field_7C.y, 0x30A0F0, 255, INK_NONE, false);

    int x = entity->position.x - (entity->field_7C.x >> 1);
    int y = entity->position.y - (entity->field_7C.y >> 1);
    RSDK.DrawRect(x, y, 0xE60000, 0x180000, 0, 255, INK_NONE, false);
    UIWidgets_Unknown5(0, 23, 0, 0, 0xE60000 + x, y);

    if (!entity->botHidden) {
        x = (entity->field_7C.x >> 1) + entity->position.x - 0xE60000;
        y = (entity->field_7C.y >> 1) + entity->position.y - 0x180000;
        RSDK.DrawRect(x, y, 0xE60000, 0x180000, 0, 255, INK_NONE, false);
        UIWidgets_Unknown5(0, -23, 0, 0, x, y + 0x170000);
    }
}

void UIOptionPanel_EditorDraw(void) {}

void UIOptionPanel_EditorLoad(void) {}

void UIOptionPanel_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_ENUM, panelSize);
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_ENUM, topListID);
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_ENUM, topFrameID);
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_ENUM, botListID);
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_ENUM, botFrameID);
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_BOOL, botAlignRight);
    RSDK_EDITABLE_VAR(UIOptionPanel, VAR_BOOL, botHidden);
}
