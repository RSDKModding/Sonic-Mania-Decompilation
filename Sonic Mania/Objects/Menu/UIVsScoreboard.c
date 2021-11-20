#include "SonicMania.h"

ObjectUIVsScoreboard *UIVsScoreboard;

void UIVsScoreboard_Update(void)
{
    RSDK_THIS(UIVsScoreboard);

    if (self->textSpriteIndex != UIWidgets->textSpriteIndex || self->showWinner) {
        UIVsScoreboard_SetupSprites();
        self->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
    if (self->posPtr) {
        self->position.x = self->posPtr->x;
        self->position.y = self->posPtr->y;
    }
}

void UIVsScoreboard_LateUpdate(void) {}

void UIVsScoreboard_StaticUpdate(void) {}

void UIVsScoreboard_Draw(void) { UIVsScoreboard_DrawSprites(); }

void UIVsScoreboard_Create(void *data)
{
    RSDK_THIS(UIVsScoreboard);
    self->active        = ACTIVE_BOUNDS;
    self->drawOrder     = 2;
    self->visible       = 1;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;
    UIVsScoreboard_SetupSprites();
    if (!SceneInfo->inEditor) {
        char buffer[0x40];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%d : %d", self->p1Score, self->p2Score);
        UIVsScoreboard_SetText(self, buffer);
    }
    self->textSpriteIndex = UIWidgets->textSpriteIndex;
}

void UIVsScoreboard_StageLoad(void) { UIVsScoreboard->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsScoreboard_SetupSprites(void)
{
    RSDK_THIS(UIVsScoreboard);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &self->animator1, true, 9);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &self->animator2, true, self->winnerID + 6);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &self->animator3, true, 8);
    self->sizeY = (self->size.y >> 16);
}

void UIVsScoreboard_SetText(EntityUIVsScoreboard *self, const char *text)
{
    if (!SceneInfo->inEditor) {
        RSDK.SetText(&self->scoreText, text, 0);
        if (!SceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIVsScoreboard->aniFrames, 18, &self->textAnimator, true, 0);
            RSDK.SetSpriteString(UIVsScoreboard->aniFrames, 18, &self->scoreText);
        }
    }
}

void UIVsScoreboard_DrawSprites(void)
{
    RSDK_THIS(UIVsScoreboard);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += self->offset.x;
    drawPos.y = drawPos.y - 0x240000 + self->offset.y;
    UIWidgets_DrawRhombus(self->size.y >> 16, (self->size.y + self->size.x) >> 16, self->sizeY, 0, 0, 0, drawPos.x, drawPos.y);
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    drawPos.y += 0x120000;
    UIWidgets_DrawRhombus(self->size.y >> 16, (self->size.y + self->size.x) >> 16, self->sizeY, 0, 0, 0, drawPos.x, drawPos.y);

    if (!SceneInfo->inEditor) {
        int32 width = RSDK.GetStringWidth(UIVsScoreboard->aniFrames, 18, &self->scoreText, 0, self->scoreText.textLength, 0);
        drawPos.x -= width << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->scoreText, 0, self->scoreText.textLength, ALIGN_LEFT, 0, 0, 0, false);

        drawPos.x += width << 15;
    }

    if (self->showWinner) {
        drawPos.y += 0x360000;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
    }

    if (SceneInfo->inEditor) {
        if (self->position.y != self->offset.y)
            DrawHelpers_DrawArrow(0xFFFF, self->position.x, self->position.y, self->offset.x + self->position.x,
                                   self->offset.y + self->position.y);
    }
}

#if RETRO_INCLUDE_EDITOR
void UIVsScoreboard_EditorDraw(void) { UIVsScoreboard_DrawSprites(); }

void UIVsScoreboard_EditorLoad(void) { UIVsScoreboard->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
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
