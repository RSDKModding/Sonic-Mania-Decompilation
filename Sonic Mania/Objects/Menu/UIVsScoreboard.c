#include "SonicMania.h"

ObjectUIVsScoreboard *UIVsScoreboard;

void UIVsScoreboard_Update(void)
{
    RSDK_THIS(UIVsScoreboard);

    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex || entity->showWinner) {
        UIVsScoreboard_SetupSprites();
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
    if (entity->posPtr) {
        entity->position.x = entity->posPtr->x;
        entity->position.y = entity->posPtr->y;
    }
}

void UIVsScoreboard_LateUpdate(void) {}

void UIVsScoreboard_StaticUpdate(void) {}

void UIVsScoreboard_Draw(void) { UIVsScoreboard_DrawSprites(); }

void UIVsScoreboard_Create(void *data)
{
    RSDK_THIS(UIVsScoreboard);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = 1;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x300000;
    UIVsScoreboard_SetupSprites();
    if (!RSDK_sceneInfo->inEditor) {
        char buffer[0x40];
        memset(buffer, 0, sizeof(buffer));
        sprintf(buffer, "%d : %d", entity->p1Score, entity->p2Score);
        UIVsScoreboard_SetText(entity, buffer);
    }
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
}

void UIVsScoreboard_StageLoad(void) { UIVsScoreboard->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsScoreboard_SetupSprites(void)
{
    RSDK_THIS(UIVsScoreboard);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &entity->animator1, true, 9);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &entity->animator2, true, entity->winnerID + 6);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &entity->animator3, true, 8);
    entity->sizeY = (entity->size.y >> 16);
}

void UIVsScoreboard_SetText(EntityUIVsScoreboard *entity, const char *text)
{
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(&entity->scoreText, text, 0);
        if (!RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIVsScoreboard->aniFrames, 18, &entity->textAnimator, true, 0);
            RSDK.SetSpriteString(UIVsScoreboard->aniFrames, 18, &entity->scoreText);
        }
    }
}

void UIVsScoreboard_DrawSprites(void)
{
    RSDK_THIS(UIVsScoreboard);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x += entity->offset.x;
    drawPos.y = drawPos.y - 0x240000 + entity->offset.y;
    UIWidgets_Unknown7(entity->size.y >> 16, (entity->size.y + entity->size.x) >> 16, entity->sizeY, 0, 0, 0, drawPos.x, drawPos.y);
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.y += 0x120000;
    UIWidgets_Unknown7(entity->size.y >> 16, (entity->size.y + entity->size.x) >> 16, entity->sizeY, 0, 0, 0, drawPos.x, drawPos.y);

    if (!RSDK_sceneInfo->inEditor) {
        int32 width = RSDK.GetStringWidth(UIVsScoreboard->aniFrames, 18, &entity->scoreText, 0, entity->scoreText.textLength, 0);
        drawPos.x -= width << 15;
        RSDK.DrawText(&entity->textAnimator, &drawPos, &entity->scoreText, 0, entity->scoreText.textLength, ALIGN_LEFT, 0, 0, 0, false);

        drawPos.x += width << 15;
    }

    if (entity->showWinner) {
        drawPos.y += 0x360000;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
        RSDK.DrawSprite(&entity->animator3, &drawPos, false);
    }

    if (RSDK_sceneInfo->inEditor) {
        if (entity->position.y != entity->offset.y)
            DrawHelpers_DrawArrow(0xFFFF, entity->position.x, entity->position.y, entity->offset.x + entity->position.x,
                                   entity->offset.y + entity->position.y);
    }
}

void UIVsScoreboard_EditorDraw(void) {}

void UIVsScoreboard_EditorLoad(void) {}

void UIVsScoreboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_UINT8, p1Score);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_UINT8, p2Score);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_BOOL, showWinner);
    RSDK_EDITABLE_VAR(UIVsScoreboard, VAR_UINT8, winnerID);
}
