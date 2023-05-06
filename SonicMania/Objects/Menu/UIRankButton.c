// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIRankButton Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectUIRankButton *UIRankButton;

void UIRankButton_Update(void)
{
    RSDK_THIS(UIRankButton);

    self->touchPosSizeS.x   = self->size.x;
    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = 0;
    self->touchPosSizeS.x   = self->size.x + 3 * self->size.y;
    self->touchPosSizeS.y   = self->size.y + 0x60000;

    StateMachine_Run(self->state);

    if (self->leaderboardEntry) {
        if (self->leaderboardEntry != self->prevLeaderboardEntry || self->leaderboardEntry->status != self->prevLeaderboardEntryStatus) {
            UIRankButton_SetupLeaderboardRank(self, self->leaderboardEntry);
            self->prevLeaderboardEntry       = self->leaderboardEntry;
            self->prevLeaderboardEntryStatus = self->leaderboardEntry->status;
        }
    }

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && self->state == UIRankButton_State_HandleButtonEnter && !control->popoverHasFocus
        && (control->state != UIControl_ProcessInputs || control->buttons[control->buttonID] != (EntityUIButton *)self)) {
        self->isSelected = false;
        self->state      = UIRankButton_State_HandleButtonLeave;
    }
}

void UIRankButton_LateUpdate(void) {}

void UIRankButton_StaticUpdate(void) {}

void UIRankButton_Draw(void)
{
    RSDK_THIS(UIRankButton);

    EntityUIControl *control   = (EntityUIControl *)self->parent;
    EntityUICarousel *carousel = control->carousel;

    bool32 setClip = false;

    int32 newClipX1 = 0;
    int32 newClipY1 = 0;
    int32 newClipX2 = 0;
    int32 newClipY2 = 0;
    if (carousel && (carousel->clipSize.x > 0 || carousel->clipSize.y > 0)) {
        setClip   = true;
        newClipX2 = carousel->clipSize.x;
        newClipY2 = carousel->clipSize.y;
        newClipX1 = carousel->clipOffset.x + carousel->position.x;
        newClipY1 = carousel->clipOffset.y + carousel->position.y;
    }

    if (self->buttonBounceOffset || self->textBounceOffset)
        setClip = false;

    int32 clipX1 = ScreenInfo->clipBound_X1;
    int32 clipY1 = ScreenInfo->clipBound_Y1;
    int32 clipX2 = ScreenInfo->clipBound_X2;
    int32 clipY2 = ScreenInfo->clipBound_Y2;

    if (setClip)
        RSDK.SetClipBounds(SceneInfo->currentScreenID, (newClipX1 >> 16) - ScreenInfo->position.x - (newClipX2 >> 17),
                           (newClipY1 >> 16) - ScreenInfo->position.y - (newClipY2 >> 17),
                           (newClipX1 >> 16) - ScreenInfo->position.x + (newClipX2 >> 17),
                           (newClipX2 >> 17) + (newClipY1 >> 16) - ScreenInfo->position.y);

    UIRankButton_DrawSprites();

    if (setClip)
        RSDK.SetClipBounds(SceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void UIRankButton_Create(void *data)
{
    RSDK_THIS(UIRankButton);

    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->visible       = true;
    self->drawGroup     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->size.x        = 0x180000;
    self->size.y        = 0x100000;
    self->bgEdgeSize    = 16;
    self->hasChanged    = false;
    self->rank          = 0;
    self->score         = 0;
    self->replayID      = 0;

    if (!SceneInfo->inEditor) {
        RSDK.InitString(&self->rankText, "", 0);
        RSDK.InitString(&self->nameTimeText, "-----", 0);
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->nameTimeText);
    }

    self->processButtonCB    = UIButton_ProcessButtonCB_Scroll;
    self->touchCB            = UIButton_ProcessTouchCB_Single;
    self->selectedCB         = UIRankButton_SelectedCB;
    self->failCB             = UIRankButton_FailCB;
    self->buttonEnterCB      = UIRankButton_ButtonEnterCB;
    self->buttonLeaveCB      = UIRankButton_ButtonLeaveCB;
    self->checkButtonEnterCB = UIRankButton_CheckButtonEnterCB;
    self->checkSelectedCB    = UIRankButton_CheckSelectedCB;

    self->textVisible = true;
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 0, &self->textAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->fontAnimator, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->saveSelFrames, 9, &self->replayIconAnimator, true, 12);
    RSDK.SetSpriteAnimation(UIWidgets->saveSelFrames, 9, &self->rankingAnimator, true, 13);

    self->textFrames = UIWidgets->textFrames;
}

void UIRankButton_StageLoad(void) {}

void UIRankButton_SetRankText(EntityUIRankButton *button, int32 rank)
{
    char buffer[8];
    if (rank)
        sprintf_s(buffer, (int32)sizeof(buffer), "%d", rank);
    else
        sprintf_s(buffer, (int32)sizeof(buffer), "-");

    RSDK.SetString(&button->rankText, buffer);
    RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &button->rankText);
}

void UIRankButton_SetTimeAttackRank(EntityUIRankButton *button, int32 rank, int32 score, int32 replayID)
{
    button->rank     = rank;
    button->score    = score;
    button->replayID = 0;

    if (replayID && globals->replayTableID != -1 && API.GetUserDBRowByID(globals->replayTableID, replayID) != (uint16)-1)
        button->replayID = replayID;

    button->leaderboardEntry = NULL;
    button->hasChanged       = false;

    if (!SceneInfo->inEditor)
        UIRankButton_SetRankText(button, rank);
}

void UIRankButton_SetupLeaderboardRank(EntityUIRankButton *button, LeaderboardEntry *entry)
{
    button->leaderboardEntry = entry;

    if (entry) {
        button->rank = entry->globalRank;

        if (entry->status == STATUS_OK) {
            button->score      = entry->score;
            button->hasChanged = false;

            RSDK.CopyString(&button->nameTimeText, &entry->username);
            RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &button->nameTimeText);

            UIRankButton_SetRankText(button, entry->globalRank);
        }
        else {
            button->score      = 0;
            button->hasChanged = false;

            RSDK.SetString(&button->nameTimeText, "-----");
            RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &button->nameTimeText);

            UIRankButton_SetRankText(button, button->rank);
        }
    }
    else {
        button->rank       = 0;
        button->score      = 0;
        button->hasChanged = false;

        RSDK.SetString(&button->nameTimeText, "-----");
        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &button->nameTimeText);

        UIRankButton_SetRankText(button, button->rank);
    }
}

void UIRankButton_DrawSprites(void)
{
    RSDK_THIS(UIRankButton);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    int32 startX = 0xA40000;
    if (!self->showsName)
        startX = 0x840000;

    int32 width = 0xB20000;
    if (self->showsName)
        width = 0xF20000;

    drawPos.x = 0x110000 - ((width + 0x2A0000) >> 1) + self->position.x;
    UIRankButton_DrawBackgroundShape(drawPos.x, drawPos.y, 0x220000, 0x100000, 0x000000);

    int32 drawX = startX + drawPos.x;
    int32 drawY = drawPos.y;
    UIRankButton_DrawBackgroundShape(drawX, drawPos.y, width, 0x100000, 0x000000);

    if (self->state != UIRankButton_State_Selected || !((self->timer >> 1) % 2)) {
        if (!SceneInfo->inEditor) {
            drawPos.x = drawPos.x + self->buttonBounceOffset;
            drawPos.y = self->textBounceOffset + drawPos.y + self->buttonBounceOffset;

            if (self->rank && self->rank <= 3) {
                self->rankingAnimator.frameID = self->rank + 12;
                RSDK.DrawSprite(&self->rankingAnimator, &drawPos, false);
            }

            drawPos.x -= RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->rankText, 0, self->rankText.length, 0) << 15;
            RSDK.DrawText(&self->fontAnimator, &drawPos, &self->rankText, 0, self->rankText.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);
        }

        if (self->showsName && !SceneInfo->inEditor) {
            drawPos.x = drawX - (width >> 1) + 0x20000 + self->buttonBounceOffset;
            drawPos.y = drawY + self->buttonBounceOffset + self->textBounceOffset;
            int32 len = MIN(self->nameTimeText.length, 20);
            RSDK.DrawText(&self->fontAnimator, &drawPos, &self->nameTimeText, 0, len, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);
        }

        drawPos.x = (drawX + self->buttonBounceOffset) - 0x200000;
        drawPos.y = (drawY + self->buttonBounceOffset) + (self->textBounceOffset - 0x20000);
        if (self->showsName)
            drawPos.x += 0x3C0000;

        UIWidgets_DrawTime(drawPos.x, drawPos.y, self->score / 6000, self->score % 6000 / 100, self->score % 100);

        if (!SceneInfo->inEditor && API.CheckDLC(DLC_PLUS) && self->replayID) {
            drawPos.x = self->position.x + ((width + 0x2A0000) >> 1);
            drawPos.y = self->position.y;

            if (self->showsName)
                drawPos.x += 0xC0000;

            drawPos.x += self->buttonBounceOffset;
            drawPos.y += self->buttonBounceOffset;
            drawPos.y += self->textBounceOffset;
            RSDK.DrawSprite(&self->replayIconAnimator, &drawPos, false);

            self->popoverPos.x = drawPos.x;
            self->popoverPos.y = drawPos.y - 0x80000;
        }
    }
}

void UIRankButton_DrawBackgroundShape(int32 x, int32 y, int32 width, int32 height, int32 color)
{
    RSDK_THIS(UIRankButton);

    if (self->buttonBounceOffset)
        UIWidgets_DrawParallelogram(x - self->buttonBounceOffset, y - self->buttonBounceOffset, width >> 16, height >> 16, height >> 16,
                                    (UIWidgets->buttonColor >> 16) & 0xFF, (UIWidgets->buttonColor >> 8) & 0xFF, UIWidgets->buttonColor & 0xFF);

    UIWidgets_DrawParallelogram(x + self->buttonBounceOffset, y + self->buttonBounceOffset, width >> 16, height >> 16, height >> 16,
                                (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);
}

bool32 UIRankButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIRankButton);

    return self->state == UIRankButton_State_HandleButtonEnter;
}

bool32 UIRankButton_CheckSelectedCB(void)
{
    RSDK_THIS(UIRankButton);

    return self->state == UIRankButton_State_Selected;
}

void UIRankButton_ButtonEnterCB(void)
{
    RSDK_THIS(UIRankButton);

    if (self->state != UIRankButton_State_HandleButtonEnter) {
        self->textBounceOffset     = 0;
        self->buttonBounceOffset   = 0;
        self->textBounceVelocity   = -0x20000;
        self->buttonBounceVelocity = -0x20000;

        self->state = UIRankButton_State_HandleButtonEnter;
    }
}

void UIRankButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UIRankButton);

    self->state = UIRankButton_State_HandleButtonLeave;
}

void UIRankButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UIRankButton_SelectedCB(void)
{
    RSDK_THIS(UIRankButton);

    EntityUIControl *control = (EntityUIControl *)self->parent;
    control->backoutTimer    = 30;
    self->timer              = 0;
    self->state              = UIRankButton_State_Selected;

    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIRankButton_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIRankButton);

    if (self->textBounceOffset) {
        int32 dist = -(self->textBounceOffset / abs(self->textBounceOffset));
        self->textBounceOffset += dist << 15;

        if (dist < 0) {
            if (self->textBounceOffset < 0)
                self->textBounceOffset = 0;
            else if (dist > 0 && self->textBounceOffset > 0)
                self->textBounceOffset = 0;
        }
        else {
            if (dist > 0 && self->textBounceOffset > 0)
                self->textBounceOffset = 0;
        }
    }

    if (self->buttonBounceOffset) {
        int32 dist = -(self->buttonBounceOffset / abs(self->buttonBounceOffset));
        self->buttonBounceOffset += dist << 16;

        if (dist < 0) {
            if (self->buttonBounceOffset < 0)
                self->buttonBounceOffset = 0;
            else if (dist > 0 && self->buttonBounceOffset > 0)
                self->buttonBounceOffset = 0;
        }
        else if (dist > 0 && self->buttonBounceOffset > 0) {
            self->buttonBounceOffset = 0;
        }
    }
}

void UIRankButton_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIRankButton);

    self->textBounceVelocity += 0x4000;
    self->textBounceOffset += self->textBounceVelocity;

    if (self->textBounceOffset >= 0 && self->textBounceVelocity > 0) {
        self->textBounceOffset   = 0;
        self->textBounceVelocity = 0;
    }

    self->buttonBounceVelocity += 0x4800;
    self->buttonBounceOffset += self->buttonBounceVelocity;

    if (self->buttonBounceOffset >= -0x20000 && self->buttonBounceVelocity > 0) {
        self->buttonBounceOffset   = -0x20000;
        self->buttonBounceVelocity = 0;
    }
}

void UIRankButton_State_Selected(void)
{
    RSDK_THIS(UIRankButton);

    UIRankButton_State_HandleButtonEnter();

    if (++self->timer == 30) {
        self->timer = 0;
        StateMachine_Run(self->actionCB);
        self->state = UIRankButton_State_HandleButtonEnter;
    }

    self->textVisible = !((self->timer >> 1) & 1);
}

#if GAME_INCLUDE_EDITOR
void UIRankButton_EditorDraw(void) { UIRankButton_DrawSprites(); }

void UIRankButton_EditorLoad(void) {}
#endif

void UIRankButton_Serialize(void) { RSDK_EDITABLE_VAR(UIRankButton, VAR_BOOL, showsName); }
#endif
