// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIOptionPanel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIOptionPanel *UIOptionPanel;

void UIOptionPanel_Update(void)
{
    RSDK_THIS(UIOptionPanel);

    if (self->textFrames != UIWidgets->textFrames) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->topListID, &self->topAnimator, true, self->topFrameID);
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->botListID, &self->botAnimator, true, self->botFrameID);

        self->textFrames = UIWidgets->textFrames;
    }
}

void UIOptionPanel_LateUpdate(void) {}

void UIOptionPanel_StaticUpdate(void) {}

void UIOptionPanel_Draw(void)
{
    RSDK_THIS(UIOptionPanel);

    UIOptionPanel_DrawBG();

    RSDK.DrawSprite(&self->topAnimator, &self->drawPosTop, false);

    if (!self->botHidden)
        RSDK.DrawSprite(&self->botAnimator, &self->drawPosBottom, false);
}

void UIOptionPanel_Create(void *data)
{
    RSDK_THIS(UIOptionPanel);

    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->topListID, &self->topAnimator, true, self->topFrameID);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->botListID, &self->botAnimator, true, self->botFrameID);
    self->textFrames = UIWidgets->textFrames;

    if (self->panelSize < 136)
        self->panelSize = 136;

    self->size.x       = WIDE_SCR_XSIZE << 16;
    self->size.y       = self->panelSize << 16;
    self->drawPosTop.x = self->position.x - 0x920000;
    self->drawPosTop.y = self->position.y + (-0x370000 - (((self->panelSize << 15) - 0x438001) & 0xFFFF0000));

    self->hitbox.left   = -212;
    self->hitbox.top    = -68;
    self->hitbox.right  = 212;
    self->hitbox.bottom = 68;

    self->drawPosBottom = self->position;
    if (!self->botAlignRight)
        self->drawPosBottom.x -= 0xA0000;
    else
        self->drawPosBottom.x += 0xA20000;
    self->drawPosBottom.y += 0x390000;
}

void UIOptionPanel_StageLoad(void) {}

void UIOptionPanel_DrawBG(void)
{
    RSDK_THIS(UIOptionPanel);

    RSDK.DrawRect(self->position.x - (self->size.x >> 1), self->position.y - (self->size.y >> 1), self->size.x, self->size.y, 0x30A0F0, 255, INK_NONE,
                  false);

    int32 x = self->position.x - (self->size.x >> 1);
    int32 y = self->position.y - (self->size.y >> 1);
    RSDK.DrawRect(x, y, 0xE60000, 0x180000, 0, 255, INK_NONE, false);

    UIWidgets_DrawRightTriangle(0xE60000 + x, y, 23, 0x00, 0x00, 0x00);

    if (!self->botHidden) {
        x = (self->size.x >> 1) + self->position.x - 0xE60000;
        y = (self->size.y >> 1) + self->position.y - 0x180000;
        RSDK.DrawRect(x, y, 0xE60000, 0x180000, 0, 255, INK_NONE, false);

        UIWidgets_DrawRightTriangle(x, y + 0x170000, -23, 0x00, 0x00, 0x00);
    }
}

#if GAME_INCLUDE_EDITOR
void UIOptionPanel_EditorDraw(void)
{
    RSDK_THIS(UIOptionPanel);

    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->topListID, &self->topAnimator, true, self->topFrameID);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, self->botListID, &self->botAnimator, true, self->botFrameID);
    self->textFrames = UIWidgets->textFrames;

    if (self->panelSize < 136)
        self->panelSize = 136;

    self->size.x = WIDE_SCR_XSIZE << 16;
    self->size.y = self->panelSize << 16;

    self->drawPosTop.x = self->position.x - 0x920000;
    self->drawPosTop.y = self->position.y + (-0x370000 - (((self->panelSize << 15) - 0x438001) & 0xFFFF0000));

    self->drawPosBottom = self->position;
    if (!self->botAlignRight)
        self->drawPosBottom.x -= 0xA0000;
    else
        self->drawPosBottom.x += 0xA20000;
    self->drawPosBottom.y += 0x390000;

    UIOptionPanel_Draw();
}

void UIOptionPanel_EditorLoad(void) {}
#endif

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
