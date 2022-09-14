// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIReplayCarousel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUIReplayCarousel *UIReplayCarousel;

void UIReplayCarousel_Update(void)
{
    RSDK_THIS(UIReplayCarousel);
    self->sortedRowCount = API.GetSortedUserDBRowCount(globals->replayTableID);
    UIReplayCarousel_SetupButtonCallbacks();
    UIReplayCarousel_HandleTouchPositions();
    self->active = ACTIVE_NORMAL;

    self->isMoving = self->curViewOffset != self->targetViewOffset;
    if (self->curViewOffset >= self->targetViewOffset) {
        if (self->curViewOffset > self->targetViewOffset) {
            self->curViewOffset -= 12;
            if (self->curViewOffset < self->targetViewOffset)
                self->curViewOffset = self->targetViewOffset;
        }
    }
    else {
        self->curViewOffset += 12;
        if (self->curViewOffset > self->targetViewOffset)
            self->curViewOffset = self->targetViewOffset;
    }
    UIReplayCarousel_HandlePositions();

    StateMachine_Run(self->state);

    if (++self->sprX >= 192)
        self->sprX -= 192;
    if (self->visibleReplayOffset != self->prevReplayOffset) {
        UIReplayCarousel_SetupVisibleReplayButtons();
        self->prevReplayOffset = self->visibleReplayOffset;
    }

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->curReplayID > 0) {
        UIControl_SetTargetPos(parent, 0, self->position.y);

        if (102 * self->curReplayID - 102 > 0)
            self->targetViewOffset = 102 * self->curReplayID - 102;
        else
            self->targetViewOffset = 0;
    }
    else {
        UIControl_SetTargetPos(parent, 0, parent->startPos.y);
        self->targetViewOffset = 0;
    }

    if (self->language != Localization->language && Localization->loaded) {
        self->language = Localization->language;
        Localization_GetString(&self->noReplaysText, STR_NOREPLAYS);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->noReplaysText);
        Localization_GetString(&self->loadingText, STR_LOADING);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->loadingText);
    }
    if (self->state == UIReplayCarousel_State_Selected && !parent->popoverHasFocus && !parent->dialogHasFocus
        && (parent->buttons[parent->buttonID] != (EntityUIButton *)self || parent->state != UIControl_ProcessInputs)) {
        UIReplayCarousel_SetUnselected();
    }
}

void UIReplayCarousel_LateUpdate(void) {}

void UIReplayCarousel_StaticUpdate(void) {}

void UIReplayCarousel_Draw(void)
{
    RSDK_THIS(UIReplayCarousel);
    StateMachine_Run(self->stateDraw);
}

void UIReplayCarousel_Create(void *data)
{
    RSDK_THIS(UIReplayCarousel);
    self->startPos        = self->position;
    self->active          = ACTIVE_BOUNDS;
    self->drawGroup       = 2;
    self->visible         = true;
    self->drawFX          = FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x300000;
    self->processButtonCB = UIReplayCarousel_ProcessButtonCB;
    self->touchCB         = UIButton_ProcessTouchCB_Multi;
    self->state           = UIReplayCarousel_State_Init;
    self->sortedRowCount  = 0;
    UIReplayCarousel_HandlePositions();
    if (!SceneInfo->inEditor) {
        for (int32 i = 0; i < 4; ++i) {
            RSDK.InitString(&self->zoneNameText[i], "", 0);
            RSDK.InitString(&self->createdAtText[i], "", 0);
        }

        UIReplayCarousel_SetupVisibleReplayButtons();
        self->prevReplayOffset = self->visibleReplayOffset;
        self->language         = Localization->language;
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->createTimeAnimator, true, 0);
        if (Localization->loaded) {
            Localization_GetString(&self->noReplaysText, STR_NOREPLAYS);
            Localization_GetString(&self->loadingText, STR_LOADING);
        }
        else {
            RSDK.InitString(&self->noReplaysText, "", 0);
            RSDK.InitString(&self->loadingText, "", 0);
        }
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->noReplaysText);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->loadingText);
    }
    RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 10, &self->fuzzAnimator, false, 0);
    UIReplayCarousel_SetupButtonCallbacks();
}

void UIReplayCarousel_StageLoad(void) { UIReplayCarousel->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIReplayCarousel_ProcessButtonCB(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (!self->isMoving && self->state != UIReplayCarousel_State_StartAction) {
        int32 rowID = 0;
        if (parent->rowCount && parent->columnCount)
            rowID = parent->buttonID / parent->columnCount;

        int32 columnID = 0;
        if (parent->columnCount)
            columnID = parent->buttonID % parent->columnCount;

        int32 id         = self->curReplayID;
        bool32 movedUp   = false;
        bool32 movedDown = false;
        if (UIControl->anyUpPress) {
            if (self->curReplayID) {
                --id;
                movedDown = true;
            }
            else {
                --rowID;
                movedUp = true;
            }
        }

        if (UIControl->anyDownPress && self->curReplayID < self->sortedRowCount - 1) {
            movedDown = true;
            id++;
        }

        if (movedUp) {
            if (rowID < 0)
                rowID += parent->rowCount;
            if (rowID >= parent->rowCount)
                rowID -= parent->rowCount;

            int32 selection = (parent->buttonCount - 1);
            if (rowID * parent->columnCount + columnID < selection)
                selection = (rowID * parent->columnCount + columnID);

            if (parent->buttonID != selection) {
                parent->buttonID = selection;
                UIReplayCarousel_SetUnselected();
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
            }
        }
        else {
            if (movedDown) {
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
                self->curReplayID = id;
            }

            if (UIControl->anyConfirmPress) {
                if (self->disabled) {
                    StateMachine_Run(self->failCB);
                }
                else {
                    if (self->actionCB) {
                        UIReplayCarousel_StartAction();
                        return;
                    }
                }
            }

            if (!self->isSelected) {
                if (parent->buttons[parent->lastButtonID] == (EntityUIButton *)self && !parent->dialogHasFocus && !parent->popoverHasFocus) {
                    self->isSelected = true;
                    self->state      = UIReplayCarousel_State_Selected;
                }
            }
        }
    }
}

void UIReplayCarousel_HandlePositions(void)
{
    RSDK_THIS(UIReplayCarousel);

    if (self->curViewOffset) {
        self->visibleReplayOffset = self->curViewOffset / 102;

        int32 pos        = 102 * (self->curViewOffset / 102) - self->curViewOffset - 102;
        self->popoverPos = pos << 16;
        if (pos < 153) {
            self->visibleReplayCount = (152 - pos) / 0x66 + 1;
        }
        else {
            self->visibleReplayCount = 0;
        }
    }
    else {
        self->visibleReplayOffset = 0;
        self->visibleReplayCount  = MIN(self->sortedRowCount, 3);
        self->popoverPos          = -0x660000;
    }
}

void UIReplayCarousel_SetupButtonCallbacks(void)
{
    RSDK_THIS(UIReplayCarousel);
    void *state = self->stateDraw;

    if (self->sortedRowCount) {
        if (!API.GetUserDBRowsChanged(globals->replayTableID) && globals->replayTableLoaded == STATUS_OK) {
            self->stateDraw       = UIReplayCarousel_Draw_Carousel;
            self->processButtonCB = UIReplayCarousel_ProcessButtonCB;
            self->touchCB         = UIButton_ProcessTouchCB_Multi;
        }
        else {
            self->stateDraw       = UIReplayCarousel_Draw_Loading;
            self->touchCB         = StateMachine_None;
            self->processButtonCB = StateMachine_None;
        }
    }
    else {
        self->stateDraw       = UIReplayCarousel_Draw_NoReplays;
        self->touchCB         = StateMachine_None;
        self->processButtonCB = StateMachine_None;
    }

    if (self->stateDraw != state && self->stateDraw == UIReplayCarousel_Draw_Carousel)
        UIReplayCarousel_SetupVisibleReplayButtons();
}

void UIReplayCarousel_HandleTouchPositions(void)
{
    RSDK_THIS(UIReplayCarousel);

    if (self->touchCB) {
        self->touchPosCount = self->visibleReplayCount;
        int32 posX          = self->position.x;
        int32 posY          = self->popoverPos + self->position.y;

        int32 i = 0;
        for (; i < self->visibleReplayCount; ++i) {
            if (i + self->visibleReplayOffset >= self->sortedRowCount)
                break;

            self->touchPosOffsetM[i].x = posX - self->position.x;
            self->touchPosOffsetM[i].y = posY - self->position.y;
            self->touchPosSizeM[i].x   = 0x1320000;
            self->touchPosSizeM[i].y   = 0x550000;
            self->touchPosCallbacks[i] = UIReplayCarousel_TouchedCB;
            posY += 0x660000;
        }

        for (; i < self->visibleReplayCount; ++i) {
            self->touchPosOffsetM[i].x = 0;
            self->touchPosOffsetM[i].y = 0;
            self->touchPosSizeM[i].x   = 0;
            self->touchPosSizeM[i].y   = 0;
            self->touchPosCallbacks[i] = StateMachine_None;
        }
    }
}

void UIReplayCarousel_TouchedCB(void)
{
    RSDK_THIS(UIReplayCarousel);
    int32 id = self->touchPosID + self->visibleReplayOffset;
    if (self->curReplayID == id)
        UIReplayCarousel_StartAction();
    else
        self->curReplayID = id;
}

void UIReplayCarousel_SetupVisibleReplayButtons(void)
{
    RSDK_THIS(UIReplayCarousel);
    if (!SceneInfo->inEditor) {
        char buffer[0x20];
        for (int32 i = 0; i < 4; ++i) {
            int32 id = i + self->visibleReplayOffset;
            if (id >= self->sortedRowCount)
                break;
            int32 row    = API.GetSortedUserDBRowID(globals->replayTableID, id);
            uint8 zoneID = 0xFF;
            API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "zoneID", &zoneID);
            API.GetUserDBRowCreationTime(globals->replayTableID, row, buffer, sizeof(buffer) - 1, "%D");
            if (zoneID != 0xFF) {
                RSDK.InitString(&self->zoneNameText[i], "", 0);
                Localization_GetZoneName(&self->zoneNameText[i], zoneID);
                if (zoneID == 5 || zoneID == 10)
                    RSDK.SetSpriteString(UIWidgets->uiFrames, 5, &self->zoneNameText[i]);
                else
                    RSDK.SetSpriteString(UIWidgets->uiFrames, 3, &self->zoneNameText[i]);

                RSDK.InitString(&self->createdAtText[i], buffer, 0);
                RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->createdAtText[i]);
            }
        }
    }
}

void UIReplayCarousel_DrawBGShapes(int32 drawX, int32 drawY, bool32 isEncore, int16 replayID)
{
    RSDK_THIS(UIReplayCarousel);
    if (!SceneInfo->inEditor)
        RSDK.DrawRect(drawX - 0x990000, drawY - 0x2A8000, 0x1320000, 0x550000, 0xFFFFFF, 127, INK_BLEND, false);

    UIWidgets_DrawRightTriangle(drawX + 0x790000, drawY + 0x298000, -76, 88, 112, 224);
    RSDK.DrawRect(drawX + 0x790000, (drawY + 0x298000) - 0x550000, 0x200000, 0x550000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(drawX - 10027008, drawY - 2785280, 20054016, 0x2C0000, 0, 255, INK_NONE, false);

    uint32 color = 0xF0F0F0;
    if (isEncore)
        color = 0xF26C4F;
    UIWidgets_DrawParallelogram(drawX - 0xA0000, drawY + 0xE0000, 128, 16, 16, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

    color = 0x5FA0B0;
    if (isEncore)
        color = 0xF26C4F;
    UIWidgets_DrawRightTriangle(drawX + 0x990000, drawY + 0x298000, -76, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(drawX + 0x30000, drawY + 0x30000, 312, 91);

    if (self->curReplayID == replayID)
        UIWidgets_DrawRectOutline_Flash(drawX, drawY, 312, 91);
    else
        UIWidgets_DrawRectOutline_Black(drawX, drawY, 312, 91);
}

void UIReplayCarousel_DrawStageInfo(int32 drawX, int32 drawY, uint8 act, uint8 characterID, int32 score, int32 replayID)
{
    RSDK_THIS(UIReplayCarousel);
    Vector2 drawPos;
    uint32 colors[] = { 0xE82858, 0x5870E0, 0xF0D808, 0x010101 };
    int32 widths[]  = { 60, 82, 52, 70 };

    drawPos.x = drawX - 0x990000;
    drawPos.y = drawY + 0x1D8000;

    Vector2 drawOffsets[4];
    for (int32 i = 0; i < 4; ++i) {
        RSDK.DrawRect(drawPos.x, drawPos.y, widths[i] << 16, 0xD0000, colors[i], 255, INK_NONE, false);

        drawOffsets[i].x = drawPos.x + 0x10000;
        drawOffsets[i].y = drawPos.y + 0xC0000;
        drawPos.x += (widths[i] << 16);
        if (i < 3) {
            UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 13, (colors[i] >> 16) & 0xFF, (colors[i] >> 8) & 0xFF, colors[i] & 0xFF);

            drawPos.x += 0xE0000;
            UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y + 0xC0000, -13, (colors[i + 1] >> 16) & 0xFF, (colors[i + 1] >> 8) & 0xFF,
                                        colors[i + 1] & 0xFF);
        }
    }

    if (!SceneInfo->inEditor) {
        int32 id    = replayID - self->visibleReplayOffset;
        drawPos.x   = drawOffsets[0].x + 0x1E0000;
        drawPos.y   = drawOffsets[0].y;
        int32 width = RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->createdAtText[id], 0, self->createdAtText[id].length, 0);
        drawPos.x -= width << 15;
        drawPos.y -= 0x60000;
        RSDK.DrawText(&self->createTimeAnimator, &drawPos, &self->createdAtText[id], 0, self->createdAtText[id].length, ALIGN_LEFT, 0, INT_TO_VOID(2),
                      NULL, false);
    }

    drawPos.x = drawOffsets[1].x + 0x40000;
    drawPos.y = drawOffsets[1].y - 0x80000;
    UIWidgets_DrawTime(drawPos.x, drawPos.y, score / 6000, score % 6000 / 100, score % 100);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->detailsAnimator, true, act + 5);
    drawPos.x = drawOffsets[2].x + 0x1A0000;
    drawPos.y = drawOffsets[2].y - 0x70000;
    RSDK.DrawSprite(&self->detailsAnimator, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->detailsAnimator, true, characterID - 1);
    drawPos.x = drawOffsets[3].x + 0x230000;
    drawPos.y = drawOffsets[3].y - 0x60000;
    RSDK.DrawSprite(&self->detailsAnimator, &drawPos, false);
}

void UIReplayCarousel_DrawZoneIcon(int32 drawX, int32 drawY, uint8 zoneID, int16 replayID)
{
    RSDK_THIS(UIReplayCarousel);
    Vector2 drawPos;

    drawPos.x = drawX - 0x690000;
    drawPos.y = drawY - 0x70000;
    UIWidgets_DrawRectOutline_Black(drawX - 0x690000, drawY - 0x70000, 96, 72);

    if (SceneInfo->inEditor || self->curReplayID != replayID || self->disabled) {
        self->direction = self->fuzzDirection;
        self->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&self->fuzzAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        self->drawFX    = FX_NONE;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UIReplayCarousel->aniFrames, 11, zoneID);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = self->sprX;

        if (self->sprX <= 102) {
            RSDK.DrawSprite(&self->zoneIconAnimator, &drawPos, false);
        }
        else {
            int32 width  = self->sprX - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&self->zoneIconAnimator, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&self->zoneIconAnimator, &drawPos, false);
        }
    }
}

void UIReplayCarousel_DrawReplayInfo(int32 drawX, int32 drawY, uint8 zoneID, uint8 act, uint8 characterID, bool32 isEncore, int32 score,
                                     int32 replayID)
{
    RSDK_THIS(UIReplayCarousel);
    UIReplayCarousel_DrawStageInfo(drawX, drawY, act, characterID, score, replayID);
    UIReplayCarousel_DrawZoneIcon(drawX, drawY, zoneID, replayID);

    Vector2 drawPos;
    drawPos.x = drawX;
    drawPos.y = drawY - 0x160000;
    if ((self->state != UIReplayCarousel_State_StartAction || self->curReplayID != replayID || !(self->timer & 2)) && !SceneInfo->inEditor) {
        int32 id  = replayID - self->visibleReplayOffset;
        drawPos.x = drawX - 0x390000;
        if (zoneID == 5 || zoneID == 10)
            RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 5, &self->zoneNameAnimators[id], true, 0);
        else
            RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 3, &self->zoneNameAnimators[id], true, 0);

        RSDK.DrawText(&self->zoneNameAnimators[id], &drawPos, &self->zoneNameText[id], 0, self->zoneNameText[id].length, ALIGN_LEFT, 0,
                      INT_TO_VOID(2), NULL, false);
    }

    drawPos.x = drawX - 0x430000;
    drawPos.y = drawY + 0x60000;
    RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 9, &self->detailsAnimator, true, (isEncore != 0) + 10);
    RSDK.DrawSprite(&self->detailsAnimator, &drawPos, false);
}

void UIReplayCarousel_StartAction(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->timer               = 0;
    self->state               = UIReplayCarousel_State_StartAction;
    self->processButtonCB     = StateMachine_None;
    parent->selectionDisabled = true;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIReplayCarousel_SetUnselected(void)
{
    RSDK_THIS(UIReplayCarousel);
    self->isSelected = false;
    self->state      = UIReplayCarousel_State_Unselected;
    if (!UIControl->hasTouchInput)
        self->curReplayID = -1;
}

void UIReplayCarousel_Draw_Loading(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    Vector2 drawPos;
    drawPos.x = parent->position.x;
    drawPos.y = parent->position.y + 0x100000;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 96, 16, 16, 0x00, 0x00, 0x00);

    drawPos.x -= RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->loadingText, 0, self->loadingText.length, 0) << 15;
    RSDK.DrawText(&self->createTimeAnimator, &drawPos, &self->loadingText, 0, self->loadingText.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);
}

void UIReplayCarousel_Draw_NoReplays(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    Vector2 drawPos;
    drawPos.x = parent->position.x;
    drawPos.y = parent->position.y + 0x100000;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 96, 16, 16, 0x00, 0x00, 0x00);

    drawPos.x -= RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->noReplaysText, 0, self->noReplaysText.length, 0) << 15;
    RSDK.DrawText(&self->createTimeAnimator, &drawPos, &self->noReplaysText, 0, self->noReplaysText.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL,
                  false);
}

void UIReplayCarousel_Draw_Carousel(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    int32 posY = self->popoverPos + self->position.y;
    for (int32 i = 0; i < self->visibleReplayCount; ++i) {
        int32 id = i + self->visibleReplayOffset;
        if (id >= self->sortedRowCount)
            break;

        int32 score       = 0;
        uint8 zoneID      = 0;
        uint8 act         = 0;
        uint8 characterID = 0;
        uint8 encore      = 0;

        int32 row = API.GetSortedUserDBRowID(globals->replayTableID, id);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT32, "score", &score);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "zoneID", &zoneID);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "act", &act);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "characterID", &characterID);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "encore", &encore);
        if (id == self->curReplayID && parent->active == ACTIVE_ALWAYS) {
            int32 palRow = (zoneID % 12) >> 3;
            int32 bankID = 0;
            if (encore)
                bankID = palRow + 4;
            else
                bankID = palRow + 1;
            RSDK.CopyPalette(bankID, (32 * (zoneID % 12)), 0, 224, 32);
            RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 11, &self->zoneIconAnimator, true, zoneID % 12);
        }
        UIReplayCarousel_DrawBGShapes(self->position.x, posY, encore, id);
        UIReplayCarousel_DrawReplayInfo(self->position.x, posY, zoneID % 12, act, characterID, encore, score, id);
        posY += 0x660000;
    }
}

void UIReplayCarousel_State_Init(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    foreach_all(UIButtonPrompt, prompt)
    {
        Hitbox hitbox;
        hitbox.left   = -(parent->size.x) >> 17;
        hitbox.top    = -(parent->size.y) >> 17;
        hitbox.right  = (parent->size.x) >> 17;
        hitbox.bottom = (parent->size.y) >> 17;
        if (MathHelpers_PointInHitbox(parent->startPos.x - parent->cameraOffset.x, parent->startPos.y - parent->cameraOffset.y, prompt->position.x,
                                      prompt->position.y, FLIP_NONE, &hitbox)) {
            if (prompt->buttonID == 3) {
                UIReplayCarousel->prompt = prompt;
                foreach_break;
            }
        }
    }

    self->state = UIReplayCarousel_State_Unselected;

    self->position.x = self->startPos.x;
    RSDK.ProcessAnimation(&self->fuzzAnimator);
    self->fuzzDirection = self->fuzzAnimator.frameID & 3;
}

void UIReplayCarousel_State_Unselected(void)
{
    RSDK_THIS(UIReplayCarousel);

    self->position.x = self->startPos.x;
    RSDK.ProcessAnimation(&self->fuzzAnimator);
    self->fuzzDirection = self->fuzzAnimator.frameID & 3;
}

void UIReplayCarousel_State_Selected(void)
{
    RSDK_THIS(UIReplayCarousel);

    RSDK.ProcessAnimation(&self->fuzzAnimator);
    self->fuzzDirection = self->fuzzAnimator.frameID & 3;
    if (self->curReplayID == -1)
        self->curReplayID = 0;
}

void UIReplayCarousel_State_StartAction(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    RSDK.ProcessAnimation(&self->fuzzAnimator);
    self->isSelected    = true;
    self->fuzzDirection = self->fuzzAnimator.frameID & 3;
    self->touchCB       = StateMachine_None;

    if (self->timer >= 30) {
        self->timer               = 0;
        parent->selectionDisabled = false;
        StateMachine_Run(self->actionCB);
        self->state = UIReplayCarousel_State_Unselected;
    }
    else {
        self->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIReplayCarousel_EditorDraw(void)
{
    RSDK_THIS(UIReplayCarousel);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x100000;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, 96, 16, 16, 0x00, 0x00, 0x00);
}

void UIReplayCarousel_EditorLoad(void) { UIReplayCarousel->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UIReplayCarousel_Serialize(void) { RSDK_EDITABLE_VAR(UIReplayCarousel, VAR_BOOL, disabled); }
#endif
