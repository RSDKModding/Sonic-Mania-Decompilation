// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIVsResults Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIVsResults *UIVsResults;

void UIVsResults_Update(void)
{
    RSDK_THIS(UIVsResults);

    if (self->textFrames != UIWidgets->textFrames) {
        UIVsResults_SetupSprites();
        self->textFrames = UIWidgets->textFrames;
    }

    if (self->numRows != self->numRowStore) {
        UIVsResults_SetupSprites();
        self->numRowStore = self->numRows;
    }

    StateMachine_Run(self->state);
}

void UIVsResults_LateUpdate(void) {}

void UIVsResults_StaticUpdate(void) {}

void UIVsResults_Draw(void)
{
#if MANIA_USE_PLUS
    RSDK_THIS(UIVsResults);

    RSDK.DrawRect(self->position.x - 0x300000, self->position.y - 0x208000, 0x600000, self->size.y, 0xFFFFFF, 0x7F, INK_BLEND, false);
#endif

    UIVsResults_DrawBG();
    UIVsResults_DrawOutline();
    UIVsResults_DrawResults();
}

void UIVsResults_Create(void *data)
{
    RSDK_THIS(UIVsResults);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    self->characterID   = self->playerID;

    self->state = UIVsResults_State_Blank;
    UIVsResults_SetupSprites();

    self->textFrames = UIWidgets->textFrames;

    if (!SceneInfo->inEditor) {
        for (int32 i = 0; i < self->numRows; ++i) {
            if (!SceneInfo->inEditor) {
                RSDK.InitString(&self->rowText[i], "00", 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &self->rowText[i]);
            }
        }
    }
}

void UIVsResults_StageLoad(void)
{
    UIVsResults->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsResults->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);
}

void UIVsResults_SetupSprites(void)
{
    RSDK_THIS(UIVsResults);

    EntityCompetitionSession *session = CompetitionSession_GetSession();

    self->characterID = self->playerID;
    if (!SceneInfo->inEditor) {
        switch (session->playerID[self->playerID]) {
            case ID_SONIC: self->characterID = UICHARBUTTON_SONIC; break;
            case ID_TAILS: self->characterID = UICHARBUTTON_TAILS; break;
            case ID_KNUCKLES: self->characterID = UICHARBUTTON_KNUX; break;
#if MANIA_USE_PLUS
            case ID_MIGHTY: self->characterID = UICHARBUTTON_MIGHTY; break;
            case ID_RAY: self->characterID = UICHARBUTTON_RAY; break;
#endif
            default: break;
        }
    }

#if MANIA_USE_PLUS
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &self->edgeAnimator, true, 1);
#else
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &self->edgeAnimator, true, self->playerID);
#endif

    int32 frame = self->characterID;
#if MANIA_USE_PLUS
    if (frame >= UICHARBUTTON_MIGHTY)
        ++frame;
#endif
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 1, &self->playerAnimator, true, frame);
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 2, &self->shadowAnimator, true, frame);
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 18, &self->numbersAnimator, true, 0);

    uint8 *rowLabels = &self->row0Label;
    for (int32 r = 0; r < self->numRows; ++r) RSDK.SetSpriteAnimation(UIWidgets->textFrames, 13, &self->rowAnimators[r], true, rowLabels[r]);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->textAnimator, true, self->playerID + 8);

    self->size.x             = 0x600000;
    self->triBounceOffset    = 0x10000;
    self->playerBounceOffset = 0x8000;
    self->size.y             = (0x120000 * self->numRows) + 0x3F0000;
}

void UIVsResults_DrawOutline(void)
{
    RSDK_THIS(UIVsResults);

    int32 y = (self->size.y >> 1) + self->position.y - 0x1D8000;
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(self->position.x + 0x30000, y, 96, self->size.y >> 16);

#if MANIA_USE_PLUS
    if (!self->isWinner)
        UIWidgets_DrawRectOutline_Black(self->position.x, y - 0x30000, 96, self->size.y >> 16);
    else
        UIWidgets_DrawRectOutline_Flash(self->position.x, y - 0x30000, 96, self->size.y >> 16);
#else
    UIWidgets_DrawRectOutline_Black(self->position.x, y - 0x30000, 96, self->size.y >> 16);
#endif
}

void UIVsResults_DrawBG(void)
{
    RSDK_THIS(UIVsResults);

    UIWidgets_DrawRightTriangle(self->position.x - 0x2D0000, self->position.y - 0x1D8000, (self->triBounceOffset >> 11), 0xE8, 0x28, 0x58);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y - 0x218000 + self->size.y, (-64 * self->triBounceOffset) >> 16, 0x60,
                                0xA0, 0xB0);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y - 0x218000 + self->size.y, (-44 * self->triBounceOffset) >> 16, 0x58,
                                0x70, 0xE0);
}

void UIVsResults_DrawRow(int32 row, int32 x, int32 y)
{
    RSDK_THIS(UIVsResults);

    Vector2 drawPos;
    bool32 *rowHighlight = &self->row0Highlight;

    drawPos.x = x;
    drawPos.y = y;
    RSDK.DrawRect(x, y, 0x5A0000, 0x100000, 0x000000, 0xFF, INK_NONE, false);

    if (rowHighlight[row]) {
        drawPos.y += 0xF0000;
        drawPos.x += 0x240000;
        UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, -15, 0xE8, 0x28, 0x58);

        RSDK.DrawRect(drawPos.x, drawPos.y - 0xF0000, 0x360000, 0x100000, 0xE82858, 0xFF, INK_NONE, false);
    }

    drawPos.x = x + 0x10000;
    drawPos.y = y + 0x80000;
    RSDK.DrawSprite(&self->rowAnimators[row], &drawPos, false);

    if (!SceneInfo->inEditor) {
        drawPos.y   = y + 0x80000;
        drawPos.x   = x + 0x590000;
        int32 width = RSDK.GetStringWidth(UIVsResults->aniFrames, 18, &self->rowText[row], 0, self->rowText[row].length, 0);
        drawPos.x -= width << 16;
        RSDK.DrawText(&self->numbersAnimator, &drawPos, &self->rowText[row], 0, self->rowText[row].length, ALIGN_LEFT, 0, NULL, NULL, false);
    }
}

#if MANIA_USE_PLUS
void UIVsResults_DrawTrophies(void)
{
    RSDK_THIS(UIVsResults);
    Vector2 drawPos;

    int32 count = self->trophyCount;
    drawPos.x   = self->position.x - 0x2B0000;
    drawPos.y   = self->position.y + 0x1C0000;

    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &self->trophyAnimator, true, 15);
    for (; count >= 10; drawPos.x += 0x150000) {
        count -= 10;

        if (count > 0 || !self->isLoser || (UIControl->timer & 0x10))
            RSDK.DrawSprite(&self->trophyAnimator, &drawPos, false);
    }

    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &self->trophyAnimator, true, 16);
    for (; count >= 5; drawPos.x += 0x100000) {
        count -= 5;

        if (count > 0 || !self->isLoser || (UIControl->timer & 0x10))
            RSDK.DrawSprite(&self->trophyAnimator, &drawPos, false);
    }

    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &self->trophyAnimator, true, 17);
    for (; count >= 1;) {
        count--;

        if (count > 0 || !self->isLoser || (UIControl->timer & 0x10))
            RSDK.DrawSprite(&self->trophyAnimator, &drawPos, false);

        drawPos.x += 0xE0000;
    }
}
#endif

void UIVsResults_DrawResults(void)
{
    RSDK_THIS(UIVsResults);

    Vector2 drawPos;

#if MANIA_USE_PLUS
    drawPos.x = self->position.x + 0x2D0000;
    drawPos.y = self->position.y - 0x1D8000;
    RSDK.DrawSprite(&self->edgeAnimator, &drawPos, false);

    drawPos.x -= 0xA0000;
    drawPos.y += 0x80000;
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);

    drawPos = self->position;
    drawPos.x += 4 * self->playerBounceOffset;
    drawPos.y += 4 * self->playerBounceOffset;
    RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);

    drawPos.x -= 8 * self->playerBounceOffset;
    drawPos.y -= 8 * self->playerBounceOffset;
    RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);

    drawPos.x = self->position.x - 0x2D0000;
    drawPos.y = self->position.y + 0x1D8000;
    for (int32 r = 0; r < self->numRows; ++r) {
        UIVsResults_DrawRow(r, drawPos.x, drawPos.y);

        drawPos.y += 0x100000;
        if (r < self->numRows - 1)
            drawPos.y += 0x20000;
    }

    UIVsResults_DrawTrophies();
#else
    drawPos.x = self->position.x - 0x2D0000;
    drawPos.y = self->position.y + 0x1D8000;
    for (int32 r = 0; r < self->numRows; ++r) {
        UIVsResults_DrawRow(r, drawPos.x, drawPos.y);

        drawPos.y += 0x100000;
        if (r < self->numRows - 1)
            drawPos.y += 0x20000;
    }

    drawPos.x = self->position.x + (self->playerID ? 0x2D0000 : -0x2D0000);
    drawPos.y = self->position.y - 0x1D8000;
    RSDK.DrawSprite(&self->edgeAnimator, &drawPos, false);

    drawPos.x += self->playerID ? -0xA0000 : 0xA0000;
    drawPos.y += 0x80000;
    RSDK.DrawSprite(&self->textAnimator, &drawPos, false);

    if (!self->isWinner || (UIControl->timer & 0x10)) {
        drawPos = self->position;
        drawPos.x += 4 * self->playerBounceOffset;
        drawPos.y += 4 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);

        drawPos.x -= 8 * self->playerBounceOffset;
        drawPos.y -= 8 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);
    }
#endif
}

void UIVsResults_State_Blank(void)
{
    // Everyone loses
}

#if RETRO_INCLUDE_EDITOR
void UIVsResults_EditorDraw(void)
{
    UIVsResults_SetupSprites();
    UIVsResults_Draw();
}

void UIVsResults_EditorLoad(void)
{
    UIVsResults->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsResults->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIVsResults, playerID);
    RSDK_ENUM_VAR("Player 1", UIVSCHARSELECTOR_1P);
    RSDK_ENUM_VAR("Player 2", UIVSCHARSELECTOR_2P);
#if MANIA_USE_PLUS
    RSDK_ENUM_VAR("Player 3", UIVSCHARSELECTOR_3P);
    RSDK_ENUM_VAR("Player 4", UIVSCHARSELECTOR_4P);
#endif
}
#endif

void UIVsResults_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, playerID);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_ENUM, numRows);
#if MANIA_USE_PLUS
    RSDK_EDITABLE_VAR(UIVsResults, VAR_ENUM, trophyCount);
#endif
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row0Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row1Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row2Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row3Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row4Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row5Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row6Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row7Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row8Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row9Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row10Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row11Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, row12Label);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row0Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row1Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row2Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row3Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row4Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row5Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row6Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row7Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row8Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row9Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row10Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row11Highlight);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, row12Highlight);
}
