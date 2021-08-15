#include "SonicMania.h"

ObjectUIVsResults *UIVsResults;

void UIVsResults_Update(void)
{
    RSDK_THIS(UIVsResults);
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        UIVsResults_SetupSprites();
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }

    if (entity->numRows != entity->numRowStore) {
        UIVsResults_SetupSprites();
        entity->numRowStore = entity->numRows;
    }

    StateMachine_Run(entity->state);
}

void UIVsResults_LateUpdate(void) {}

void UIVsResults_StaticUpdate(void) {}

void UIVsResults_Draw(void)
{
    RSDK_THIS(UIVsResults);
    RSDK.DrawRect(entity->position.x - 0x300000, entity->position.y - 0x208000, 0x600000, entity->field_1D0, 0xFFFFFF, 127, INK_BLEND, false);
    UIVsResults_Unknown3();
    UIVsResults_Unknown2();
    UIVsResults_Unknown6();
}

void UIVsResults_Create(void *data)
{
    RSDK_THIS(UIVsResults);

    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x300000;
    entity->characterID   = entity->playerID;
    entity->state         = UIVsResults_Unknown7;
    UIVsResults_SetupSprites();
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;

    if (!RSDK_sceneInfo->inEditor) {
        for (int i = 0; i < entity->numRows; ++i) {
            if (!RSDK_sceneInfo->inEditor) {
                RSDK.SetText(&entity->rowText[i], "00", 0);
                RSDK.SetSpriteString(UIVsResults->aniFrames, 18, &entity->rowText[i]);
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
    EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;

    entity->characterID = entity->playerID;
    if (!RSDK_sceneInfo->inEditor) {
        switch (session->characterFlags[entity->playerID]) {
            case ID_SONIC: entity->characterID = 0; break;
            case ID_TAILS: entity->characterID = 1; break;
            case ID_KNUCKLES: entity->characterID = 2; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: entity->characterID = 3; break;
            case ID_RAY: entity->characterID = 4; break;
#endif
            default: break;
        }
    }
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &entity->animator3, true, 1);

    int frame = entity->characterID;
    if (frame > 2)
        ++frame;
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 1, &entity->animator1, true, frame);
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 2, &entity->animator2, true, frame);
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 18, &entity->animator4, true, 0);

    byte *rowLabels = &entity->row0Label;
    for (int r = 0; r < entity->numRows; ++r) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 13, &entity->rowAnimators[r], true, rowLabels[r]);
    }
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 12, &entity->textAnimator, true, entity->playerID + 8);
    entity->field_1CC = 0x600000;
    entity->field_1C0 = 0x10000;
    entity->offset    = 0x8000;
    entity->field_1D0 = (0x120000 * entity->numRows) + 0x3F0000;
}

void UIVsResults_Unknown2(void)
{
    RSDK_THIS(UIVsResults);

    int y = (entity->field_1D0 >> 1) + entity->position.y - 0x1D8000;
    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(entity->field_1D0 >> 16, 96, entity->position.x + 0x30000, y);

    if (!entity->field_1D4)
        UIWidgets_Unknown2(entity->field_1D0 >> 16, 96, entity->position.x, y - 0x30000);
    else
        UIWidgets_Unknown4(entity->field_1D0 >> 16, 96, entity->position.x, y - 0x30000);
}

void UIVsResults_Unknown3(void)
{
    RSDK_THIS(UIVsResults);
    UIWidgets_Unknown5(232, (entity->field_1C0 >> 11), 40, 88, entity->position.x - 0x2D0000, entity->position.y - 0x1D8000);
    UIWidgets_Unknown5(96, (-64 * entity->field_1C0) >> 16, 160, 176, entity->position.x + 0x2D0000,
                       entity->position.y - 0x218000 + entity->field_1D0);
    UIWidgets_Unknown5(88, (-44 * entity->field_1C0) >> 16, 112, 224, entity->position.x + 0x2D0000,
                       entity->position.y - 0x218000 + entity->field_1D0);
}

void UIVsResults_DrawRow(int row, int posX, int posY)
{
    RSDK_THIS(UIVsResults);
    Vector2 drawPos;
    bool32 *rowHighlight = &entity->row0Highlight;

    drawPos.x = posX;
    drawPos.y = posY;
    RSDK.DrawRect(posX, posY, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);
    if (rowHighlight[row]) {
        drawPos.y += 0xF0000;
        drawPos.x += 0x240000;
        UIWidgets_Unknown5(232, -15, 40, 88, drawPos.x, drawPos.y);
        RSDK.DrawRect(drawPos.x, drawPos.y - 0xF0000, 0x360000, 0x100000, 0xE82858, 255, INK_NONE, false);
    }
    drawPos.y = posY + 0x80000;
    drawPos.x = posX + 0x10000;
    RSDK.DrawSprite(&entity->rowAnimators[row], &drawPos, false);
    if (!RSDK_sceneInfo->inEditor) {
        drawPos.y = posY + 0x80000;
        drawPos.x = posX + 0x590000;
        int width = RSDK.GetStringWidth(UIVsResults->aniFrames, 18, &entity->rowText[row], 0, entity->rowText[row].textLength, 0);
        drawPos.x -= width << 16;
        RSDK.DrawText(&entity->animator4, &drawPos, &entity->rowText[row], 0, entity->rowText[row].textLength, ALIGN_LEFT, 0, 0, 0, false);
    }
}

void UIVsResults_Unknown5(void)
{
    RSDK_THIS(UIVsResults);
    Vector2 drawPos;

    int count = entity->trophyCount;
    drawPos.x = entity->position.x - 0x2B0000;
    drawPos.y = entity->position.y + 0x1C0000;
    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &entity->animator5, true, 15);
    for (; count >= 10; drawPos.x += 0x150000) {
        count -= 10;
        if (count > 0 || !entity->field_1D8 || (UIControl->timer & 0x10))
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }

    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &entity->animator5, true, 16);
    for (; count >= 5; drawPos.x += 0x100000) {
        count -= 5;
        if (count > 0 || !entity->field_1D8 || (UIControl->timer & 0x10))
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }

    RSDK.SetSpriteAnimation(UIVsResults->aniFrames, 14, &entity->animator5, true, 17);
    for (; count >= 1; drawPos.x += 0xE0000) {
        count--;
        if (count > 0 || !entity->field_1D8 || (UIControl->timer & 0x10))
            RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }
}

void UIVsResults_Unknown6(void)
{
    RSDK_THIS(UIVsResults);
    Vector2 drawPos;

    drawPos.x = entity->position.x + 0x2D0000;
    drawPos.y = entity->position.y - 0x1D8000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    drawPos.y += 0x80000;
    drawPos.x -= 0xA0000;
    RSDK.DrawSprite(&entity->textAnimator, &drawPos, false);

    drawPos = entity->position;
    drawPos.x += 4 * entity->offset;
    drawPos.y += 4 * entity->offset;
    RSDK.DrawSprite(&entity->animator2, &drawPos, false);

    drawPos.x -= 8 * entity->offset;
    drawPos.y -= 8 * entity->offset;
    RSDK.DrawSprite(&entity->animator1, &drawPos, false);

    drawPos.x = entity->position.x - 0x2D0000;
    drawPos.y = entity->position.y + 0x1D8000;
    for (int r = 0; r < entity->numRows; ++r) {
        UIVsResults_DrawRow(r, drawPos.x, drawPos.y);
        drawPos.y += 0x100000;
        if (r < entity->numRows - 1) {
            drawPos.y += 0x20000;
        }
    }
    UIVsResults_Unknown5();
}

void UIVsResults_Unknown7(void) {}

void UIVsResults_EditorDraw(void) {}

void UIVsResults_EditorLoad(void) {}

void UIVsResults_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsResults, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_UINT8, playerID);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_ENUM, numRows);
    RSDK_EDITABLE_VAR(UIVsResults, VAR_ENUM, trophyCount);
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
