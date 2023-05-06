// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIVsScoreboard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIVsScoreboard *UIVsScoreboard;

void UIVsScoreboard_Update(void)
{
    RSDK_THIS(UIVsScoreboard);

    if (self->textFrames != UIWidgets->textFrames || self->showWinner) {
        UIVsScoreboard_SetupSprites();
        self->textFrames = UIWidgets->textFrames;
    }

    if (self->parentPos) {
        self->position.x = self->parentPos->x;
        self->position.y = self->parentPos->y;
    }
}

void UIVsScoreboard_LateUpdate(void) {}

void UIVsScoreboard_StaticUpdate(void) {}

void UIVsScoreboard_Draw(void) { UIVsScoreboard_DrawSprites(); }

void UIVsScoreboard_Create(void *data)
{
    RSDK_THIS(UIVsScoreboard);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    UIVsScoreboard_SetupSprites();
    UIVsScoreboard_SetScores(self, self->p1Score, self->p2Score);

    self->textFrames = UIWidgets->textFrames;
}

void UIVsScoreboard_StageLoad(void) { UIVsScoreboard->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsScoreboard_SetupSprites(void)
{
    RSDK_THIS(UIVsScoreboard);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 13, &self->totalAnimator, true, 9);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 13, &self->winnerAnimator, true, self->winnerID + 6);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 13, &self->winsAnimator, true, 8);

    self->bgEdgeSize = self->size.y >> 16;
}

void UIVsScoreboard_SetScores(EntityUIVsScoreboard *scoreboard, uint32 p1Score, uint32 p2Score)
{
    if (!SceneInfo->inEditor) {
        char text[64];
        sprintf_s(text, (int32)sizeof(text), "%d : %d", p1Score, p2Score);

        scoreboard->p1Score = p1Score;
        scoreboard->p2Score = p2Score;

        if (!SceneInfo->inEditor) {
            RSDK.InitString(&scoreboard->scoreText, text, 0);

            if (!SceneInfo->inEditor) {
                RSDK.SetSpriteAnimation(UIVsScoreboard->aniFrames, 18, &scoreboard->textAnimator, true, 0);
                RSDK.SetSpriteString(UIVsScoreboard->aniFrames, 18, &scoreboard->scoreText);
            }
        }
    }
}

void UIVsScoreboard_DrawSprites(void)
{
    RSDK_THIS(UIVsScoreboard);

    Vector2 drawPos;
    drawPos.x = self->position.x + self->offset.x;
    drawPos.y = self->position.y - 0x240000 + self->offset.y;

    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, (self->size.y + self->size.x) >> 16, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);
    RSDK.DrawSprite(&self->totalAnimator, &drawPos, false);

    drawPos.y += 0x120000;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, (self->size.y + self->size.x) >> 16, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

    if (!SceneInfo->inEditor) {
        int32 width = RSDK.GetStringWidth(UIVsScoreboard->aniFrames, 18, &self->scoreText, 0, self->scoreText.length, 0);
        drawPos.x -= width << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->scoreText, 0, self->scoreText.length, ALIGN_LEFT, 0, NULL, NULL, false);

        drawPos.x += width << 15;
    }

    if (self->showWinner) {
        drawPos.y += 0x360000;
        RSDK.DrawSprite(&self->winnerAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->winsAnimator, &drawPos, false);
    }

    if (SceneInfo->inEditor) {
        if (self->position.y != self->offset.y) {
            DrawHelpers_DrawArrowAdditive(self->position.x, self->position.y, self->offset.x + self->position.x, self->offset.y + self->position.y,
                                          0x00FFFF);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void UIVsScoreboard_EditorDraw(void) { UIVsScoreboard_DrawSprites(); }

void UIVsScoreboard_EditorLoad(void)
{
    UIVsScoreboard->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIVsScoreboard, winnerID);
    RSDK_ENUM_VAR("Player 1", UIVSCHARSELECTOR_1P);
    RSDK_ENUM_VAR("Player 2", UIVSCHARSELECTOR_2P);
}
#endif

void UIVsScoreboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_UINT8, p1Score);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_UINT8, p2Score);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_BOOL, showWinner);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_UINT8, winnerID);
}
