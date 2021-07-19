#include "../SonicMania.h"

ObjectUIText *UIText;

void UIText_Update(void)
{
    EntityUIText *entity = (EntityUIText *)RSDK_sceneInfo->entity;
    if (entity->animator.animationID != entity->highlighted + entity->listID) {
        ushort spr = -1;
        if (!UIWidgets || UIText->spriteIndex)
            spr = UIText->spriteIndex;
        // else
        //    spr = UIWidgets->uiSpriteIndex;
        RSDK.SetSpriteAnimation(spr, entity->highlighted + entity->listID, &entity->animator, true, 0);
    }
}

void UIText_LateUpdate(void)
{

}

void UIText_StaticUpdate(void)
{

}

void UIText_Draw(void)
{
    EntityUIText *entity = (EntityUIText *)RSDK_sceneInfo->entity;

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    ushort spr = -1;
    if (!UIWidgets || UIText->spriteIndex)
        spr = UIText->spriteIndex;
    //else
    //    spr = UIWidgets->uiSpriteIndex;
    int width = RSDK.GetStringWidth(spr, entity->listID, &entity->text, 0, entity->text.textLength, 0);
    if (entity->align == ALIGN_RIGHT) {
        drawPos.x -= width << 15;
    }
    if (entity->align == ALIGN_CENTER) {
        drawPos.x -= width << 16;
    }
    RSDK.DrawText(&entity->animator, &drawPos, &entity->text, 0, 0, ALIGN_LEFT, 0, 0, 0, false);
}

void UIText_Create(void* data)
{
    EntityUIText *entity = (EntityUIText *)RSDK_sceneInfo->entity;
    if (!UIWidgets || UIText->spriteIndex)
        RSDK.SetSpriteAnimation(UIText->spriteIndex, entity->listID, &entity->animator, true, 0);
    //else
    //    RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, entity->listID, &entity->animator, true, 0);

    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = 2;
        entity->visible   = 1;
        entity->drawOrder = 2;
        if (!entity->text.text)
            RSDK.SetText(&entity->text, "UNTITLED", 0);
        if (!UIWidgets || UIText->spriteIndex)
            RSDK.SetSpriteString(UIText->spriteIndex, entity->listID, &entity->text);
        //else
        //    RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, entity->listID, &entity->text);
    }
}

void UIText_StageLoad(void)
{

}

void UIText_EditorDraw(void)
{

}

void UIText_EditorLoad(void)
{

}

void UIText_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIText, VAR_STRING, text);
    RSDK_EDITABLE_VAR(UIText, VAR_STRING, tag);
    RSDK_EDITABLE_VAR(UIText, VAR_BOOL, selectable);
    RSDK_EDITABLE_VAR(UIText, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIText, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIText, VAR_ENUM, data0);
    RSDK_EDITABLE_VAR(UIText, VAR_ENUM, data1);
    RSDK_EDITABLE_VAR(UIText, VAR_ENUM, data2);
    RSDK_EDITABLE_VAR(UIText, VAR_ENUM, data3);
    RSDK_EDITABLE_VAR(UIText, VAR_BOOL, highlighted);
}

