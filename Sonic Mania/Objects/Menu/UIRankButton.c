#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIRankButton *UIRankButton;

void UIRankButton_Update(void)
{
    RSDK_THIS(UIRankButton);
    self->touchPosStart.x = self->dword13C;
    self->touchPosEnd.x   = 0;
    self->touchPosEnd.y   = 0;
    self->touchPosStart.x += 3 * self->dword140;
    self->touchPosStart.y = self->dword140 + 0x60000;

    StateMachine_Run(self->state);

    if (self->leaderboardEntry) {
        if (self->leaderboardEntry != self->prevLeaderboardEntry || self->leaderboardEntry->status != self->prevLeaderboardEntryStatus) {
            UIRankButton_SetupRank(self, self->leaderboardEntry);
            self->prevLeaderboardEntry       = self->leaderboardEntry;
            self->prevLeaderboardEntryStatus = self->leaderboardEntry->status;
        }
    }

    EntityUIControl *control = (EntityUIControl *)self->parent;
    if (control && self->state == UIRankButton_Unknown13 && !control->popoverHasFocus
        && (control->state != UIControl_ProcessInputs || control->buttons[control->buttonID] != (EntityUIButton *)self)) {
        self->isSelected = false;
        self->state = UIRankButton_Unknown12;
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
    if (self->field_14C || self->field_148)
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
    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    self->visible       = true;
    self->drawOrder     = 2;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x400000;
    self->dword13C      = 0x180000;
    self->dword144      = 16;
    self->dword140      = 0x100000;
    self->dword12C      = 0;
    self->rank          = 0;
    self->score         = 0;
    self->replayID      = 0;
    if (!SceneInfo->inEditor) {
        RSDK.SetText(&self->rankText, "", 0);
        RSDK.SetText(&self->nameTimeText, "-----", 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->nameTimeText);
    }
    self->processButtonCB    = UIButton_ProcessButtonCB_Scroll;
    self->touchCB            = UIButton_ProcessTouchCB;
    self->selectedCB         = UIRankButton_SelectedCB;
    self->failCB             = UIRankButton_FailCB;
    self->buttonEnterCB      = UIRankButton_ButtonEnterCB;
    self->buttonLeaveCB      = UIRankButton_ButtonLeaveCB;
    self->checkButtonEnterCB = UIRankButton_CheckButtonEnterCB;
    self->checkSelectedCB    = UIRankButton_CheckSelectedCB;
    self->dword158           = true;
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 0, &self->animator1, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->animator2, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 9, &self->animator3, true, 12);
    RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 9, &self->animator4, true, 13);
    self->textFrames = UIWidgets->textFrames;
}

void UIRankButton_StageLoad(void) {}

void UIRankButton_SetRankText(EntityUIRankButton *button, int32 rank)
{
    char buffer[8];
    if (rank)
        sprintf(buffer, "%d", rank);
    else
        sprintf(buffer, "-");

    RSDK.PrependText(&button->rankText, buffer);
    RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &button->rankText);
}

void UIRankButton_Unknown2(int32 rank, EntityUIRankButton *button, int32 score, int32 replayID)
{
    button->rank  = rank;
    button->score = score;
    button->replayID = 0;
    if (replayID && globals->replayTableID != -1 && API.GetUserDBByID(globals->replayTableID, replayID) != -1)
        button->replayID = replayID;

    button->leaderboardEntry = NULL;
    button->dword12C         = 0;
    if (!SceneInfo->inEditor)
        UIRankButton_SetRankText(button, rank);
}

void UIRankButton_SetupRank(EntityUIRankButton *rankButton, LeaderboardEntry *entry)
{
    rankButton->leaderboardEntry = entry;
    if (entry) {
        rankButton->rank = entry->globalRank;
        if (entry->status == STATUS_OK) {
            rankButton->score    = entry->score;
            rankButton->dword12C = 0;
            RSDK.CopyString(&rankButton->nameTimeText, &entry->username);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &rankButton->nameTimeText);
            UIRankButton_SetRankText(rankButton, entry->globalRank);
        }
        else {
            rankButton->score    = 0;
            rankButton->dword12C = 0;
            RSDK.PrependText(&rankButton->nameTimeText, "-----");
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &rankButton->nameTimeText);
            UIRankButton_SetRankText(rankButton, rankButton->rank);
        }
    }
    else {
        rankButton->rank     = 0;
        rankButton->score    = 0;
        rankButton->dword12C = 0;
        RSDK.PrependText(&rankButton->nameTimeText, "-----");
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &rankButton->nameTimeText);
        UIRankButton_SetRankText(rankButton, rankButton->rank);
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
    if (self->state != UIRankButton_Unknown14 || !((self->timer >> 1) % 2)) {
        if (!SceneInfo->inEditor) {
            drawPos.x = drawPos.x + self->field_14C;
            drawPos.y = self->field_148 + drawPos.y + self->field_14C;

            if (self->rank && self->rank <= 3) {
                self->animator4.frameID = self->rank + 12;
                RSDK.DrawSprite(&self->animator4, &drawPos, false);
            }

            int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->rankText, 0, self->rankText.textLength, 0);
            drawPos.x -= width << 15;
            RSDK.DrawText(&self->animator2, &drawPos, &self->rankText, 0, self->rankText.textLength, ALIGN_LEFT, 0, 2, 0, false);
        }

        if (self->showsName && !SceneInfo->inEditor) {
            drawPos.x = drawX - (width >> 1) + 0x20000;
            drawPos.x += self->field_14C;
            drawPos.y = drawY + self->field_14C;
            drawPos.y += self->field_148;
            int32 len = self->nameTimeText.textLength;
            if (len >= 20)
                len = 20;
            RSDK.DrawText(&self->animator2, &drawPos, &self->nameTimeText, 0, len, ALIGN_LEFT, 0, 2, 0, false);
        }

        drawPos.x = (drawX + self->field_14C) - 0x200000;
        drawPos.y = (drawY + self->field_14C) + (self->field_148 - 0x20000);
        if (self->showsName)
            drawPos.x += 0x3C0000;

        UIWidgets_DrawTime(drawPos.x, drawPos.y, self->score / 6000, self->score % 6000 / 100, self->score % 100);

        if (!SceneInfo->inEditor) {
            if (API.CheckDLC(DLC_PLUS)) {
                if (self->replayID) {
                    drawPos.x = self->position.x + ((width + 0x2A0000) >> 1);
                    drawPos.y = self->position.y;
                    if (self->showsName)
                        drawPos.x += 0xC0000;
                    drawPos.x += self->field_14C;
                    drawPos.y += self->field_14C;
                    drawPos.y += self->field_148;
                    RSDK.DrawSprite(&self->animator3, &drawPos, false);

                    self->field_124 = drawPos.x;
                    self->field_128 = drawPos.y;
                    self->field_128 -= 0x80000;
                }
            }
        }
    }
}

void UIRankButton_DrawBackgroundShape(int32 x, int32 y, int32 width, int32 height, int32 colour)
{
    RSDK_THIS(UIRankButton);

    if (self->field_14C)
        UIWidgets_DrawParallelogram(height >> 16, width >> 16, height >> 16, (UIWidgets->buttonColour >> 16) & 0xFF, (UIWidgets->buttonColour >> 8) & 0xFF, UIWidgets->buttonColour & 0xFF,
                           x - self->field_14C, y - self->field_14C);
    UIWidgets_DrawParallelogram(height >> 16, width >> 16, height >> 16, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF, self->field_14C + x,
                       self->field_14C + y);
}

bool32 UIRankButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIRankButton);
    return self->state == UIRankButton_Unknown13;
}

bool32 UIRankButton_CheckSelectedCB(void)
{
    RSDK_THIS(UIRankButton);
    return self->state == UIRankButton_Unknown14;
}

void UIRankButton_ButtonEnterCB(void)
{
    RSDK_THIS(UIRankButton);
    if (self->state != UIRankButton_Unknown13) {
        self->field_148 = 0;
        self->field_14C = 0;
        self->field_150 = -0x20000;
        self->field_154 = -0x20000;
        self->state     = UIRankButton_Unknown13;
    }
}

void UIRankButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UIRankButton);
    self->state = UIRankButton_Unknown12;
}

void UIRankButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UIRankButton_SelectedCB(void)
{
    RSDK_THIS(UIRankButton);
    EntityUIControl *control = (EntityUIControl *)self->parent;
    control->backoutTimer    = 30;
    self->timer            = 0;
    self->state            = UIRankButton_Unknown14;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIRankButton_Unknown12(void)
{
    RSDK_THIS(UIRankButton);
    if (self->field_148) {
        int32 dist = -(self->field_148 / abs(self->field_148));
        self->field_148 += dist << 15;
        if (dist < 0) {
            if (self->field_148 < 0) {
                self->field_148 = 0;
            }
            else if (dist > 0 && self->field_148 > 0)
                self->field_148 = 0;
        }
        else if (dist > 0 && self->field_148 > 0)
            self->field_148 = 0;
    }

    if (self->field_14C) {
        int32 dist = -(self->field_14C / abs(self->field_14C));
        self->field_14C += dist << 16;
        if (dist < 0) {
            if (self->field_14C < 0) {
                self->field_14C = 0;
            }
            else if (dist > 0 && self->field_14C > 0)
                self->field_14C = 0;
        }
        else if (dist > 0 && self->field_14C > 0)
            self->field_14C = 0;
    }
}

void UIRankButton_Unknown13(void)
{
    RSDK_THIS(UIRankButton);
    self->field_150 += 0x4000;
    self->field_148 += self->field_150;
    if (self->field_148 >= 0 && self->field_150 > 0) {
        self->field_148 = 0;
        self->field_150 = 0;
    }

    self->field_154 += 0x4800;
    self->field_14C += self->field_154;
    if (self->field_14C >= -0x20000 && self->field_154 > 0) {
        self->field_14C = -0x20000;
        self->field_154 = 0;
    }
}

void UIRankButton_Unknown14(void)
{
    RSDK_THIS(UIRankButton);
    UIRankButton_Unknown13();
    if (++self->timer == 30) {
        self->timer = 0;
        StateMachine_Run(self->actionCB);
        self->state = UIRankButton_Unknown13;
    }

    self->dword158 = !((self->timer >> 1) & 1);
}

#if RETRO_INCLUDE_EDITOR
void UIRankButton_EditorDraw(void)
{
    RSDK_THIS(UIRankButton);
    UIRankButton_DrawSprites();
}

void UIRankButton_EditorLoad(void) {}
#endif

void UIRankButton_Serialize(void) { RSDK_EDITABLE_VAR(UIRankButton, VAR_BOOL, showsName); }
#endif
