#include "../SonicMania.h"

ObjectUIButton *UIButton;

void UIButton_Update(void)
{
    RSDK_THIS(UIButton);
    entity->touchPosStart.x = entity->size.x;
    entity->touchPosEnd.x   = 0;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x += 3 * entity->size.y;
    entity->touchPosStart.y = entity->size.y + 0x60000;
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex || entity->startListID != entity->listID || entity->startFrameID != entity->frameID
        || entity->isDisabled != entity->disabled) {
        if (entity->disabled)
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &entity->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->startListID     = entity->listID;
        entity->startFrameID    = entity->frameID;
        entity->isDisabled      = entity->disabled;
    }
    //Entity* ent = UIButton_Unknown2(entity, entity->field_15C);
    //if (ent)
    //    ent->visible = true;
    StateMachine_Run(entity->state);
    /*EntityUIControl *parent = entity->parent;
    if (parent && entity->state == UIButton_Unknown17
        && (parent->state != UIControl_ProcessInputs || parent->entities[parent->activeEntityID] != entity)) {
        entity->flag = false;
        UIButton_Unknown13();
    }*/
}

void UIButton_LateUpdate(void)
{

}

void UIButton_StaticUpdate(void)
{

}

void UIButton_Draw(void)
{
    RSDK_THIS(UIButton);

    Vector2 drawPos;
    int size      = entity->size.y + entity->size.x;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->field_140;
    drawPos.y -= entity->field_140;
    size >>= 16;
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->dword138, (UIWidgets->value >> 16) & 0xFF, (UIWidgets->value >> 8) & 0xFF,
                       (UIWidgets->value) & 0xFF, drawPos.x, drawPos.y);
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->field_140;
    drawPos.y += entity->field_140;
    UIWidgets_Unknown7(entity->size.y >> 16, size, entity->dword138, 0, 0, 0, drawPos.x, drawPos.y);
    if (entity->dword14C) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->position.y;
        drawPos.x = entity->field_140 + entity->position.x;
        drawPos.y = entity->field_140 + entity->position.y;
        drawPos.y += entity->field_13C;

        if (!entity->align) {
            drawPos.x += -0x60000 - (entity->size.x >> 1);
        }
        else if (entity->align == 2) {
            drawPos.x -= 0x60000;
            drawPos.x += entity->size.x >> 1;
        }
        if (entity->disabled) {
            if (!entity->align)
                drawPos.x += 0x150000;
        }
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
}

void UIButton_Create(void* data)
{
    RSDK_THIS(UIButton);
    if (!RSDK_sceneInfo->inEditor) {
        entity->drawOrder     = 2;
        entity->visible       = !entity->invisible;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x400000;
        entity->dword138      = entity->size.y >> 16;
        entity->size.y        = abs(entity->size.y);
        // entity->options0 = UIButton_Unknown9;
        // entity->touchCB  = UIButton_ProcessTouch;
        // entity->options3 = UIButton_Unknown15;
        // entity->failCB   = UIButton_Fail;
        // entity->options5 = UIButton_Unknown12;
        // entity->options6 = UIButton_Unknown13;
        // entity->options7 = UIButton_Unknown10;
        // entity->options8 = UIButton_Unknown11;
        entity->dword14C = 1;
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, entity->listID, &entity->animator, true, entity->frameID);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
        entity->startListID     = entity->listID;
        entity->startFrameID    = entity->frameID;

        int entID = RSDK.GetEntityID(entity) - entity->choiceCount;
        for (int i = 0; i < entity->choiceCount; ++i) {
            EntityUIButton *item = RSDK.GetEntityByID(entID + i);
            if (UIChoice && item->objectID == UIChoice->objectID || UIVsRoundPicker && item->objectID == UIVsRoundPicker->objectID
                || UIResPicker && item->objectID == UIResPicker->objectID || UIWinSize && item->objectID == UIWinSize->objectID) {
                item->parent = (Entity*)entity;
            }

            if (i) {
                item->position.x = entity->posUnknown.x;
                item->position.y = entity->posUnknown.y;
                item->active     = ACTIVE_NEVER;
            }
            else {
                entity->posUnknown.x = item->position.x;
                entity->posUnknown.y = item->position.y;
            }
        }
    }
}

void UIButton_StageLoad(void)
{

}

void UIButton_EditorDraw(void)
{

}

void UIButton_EditorLoad(void)
{

}

void UIButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIButton, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, align);
    RSDK_EDITABLE_VAR(UIButton, VAR_ENUM, choiceCount);
    RSDK_EDITABLE_VAR(UIButton, VAR_UINT8, choiceDir);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, invisible);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, assignsP1);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, freeBindP2);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, transition);
    RSDK_EDITABLE_VAR(UIButton, VAR_BOOL, stopMusic);
}

