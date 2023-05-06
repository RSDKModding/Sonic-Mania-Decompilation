// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIText Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIText *UIText;

void UIText_Update(void)
{
    RSDK_THIS(UIText);

    if (self->animator.animationID != (int32)self->highlighted + self->listID) {
        if (!UIWidgets || UIText->aniFrames)
            RSDK.SetSpriteAnimation(UIText->aniFrames, self->highlighted + self->listID, &self->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->uiFrames, self->highlighted + self->listID, &self->animator, true, 0);
    }
}

void UIText_LateUpdate(void) {}

void UIText_StaticUpdate(void) {}

void UIText_Draw(void)
{
    RSDK_THIS(UIText);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    uint16 frames = (!UIWidgets || UIText->aniFrames) ? UIText->aniFrames : UIWidgets->uiFrames;

    int32 width = RSDK.GetStringWidth(frames, self->listID, &self->text, 0, self->text.length, 0);
    switch (self->align) {
        default:
        case UITEXT_ALIGN_LEFT: break;

        case UITEXT_ALIGN_CENTER: drawPos.x -= width << 15; break;

        case UITEXT_ALIGN_RIGHT: drawPos.x -= width << 16; break;
    }

    RSDK.DrawText(&self->animator, &drawPos, &self->text, 0, 0, ALIGN_LEFT, 0, NULL, NULL, false);
}

void UIText_Create(void *data)
{
    RSDK_THIS(UIText);

    if (!UIWidgets || UIText->aniFrames)
        RSDK.SetSpriteAnimation(UIText->aniFrames, self->listID, &self->animator, true, 0);
    else
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, self->listID, &self->animator, true, 0);

    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = 2;

        if (!self->text.chars)
            RSDK.InitString(&self->text, "UNTITLED", 0);

        if (!UIWidgets || UIText->aniFrames)
            RSDK.SetSpriteString(UIText->aniFrames, self->listID, &self->text);
        else
            RSDK.SetSpriteString(UIWidgets->uiFrames, self->listID, &self->text);
    }
}

void UIText_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void UIText_EditorDraw(void)
{
    RSDK_THIS(UIText);

    RSDK.SetSpriteAnimation(UIText->aniFrames, self->listID, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, &self->position, false);
}

void UIText_EditorLoad(void)
{
    if (!UIText->aniFrames)
        UIText->aniFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIText, align);
    RSDK_ENUM_VAR("Left", UITEXT_ALIGN_LEFT);
    RSDK_ENUM_VAR("Center", UITEXT_ALIGN_CENTER);
    RSDK_ENUM_VAR("Right", UITEXT_ALIGN_RIGHT);
}
#endif

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
