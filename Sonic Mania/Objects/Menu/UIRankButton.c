#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIRankButton *UIRankButton;

void UIRankButton_Update(void)
{
    RSDK_THIS(UIRankButton);
    entity->touchPosStart.x = entity->dword13C;
    entity->touchPosEnd.x   = 0;
    entity->touchPosEnd.y   = 0;
    entity->touchPosStart.x += 3 * entity->dword140;
    entity->touchPosStart.y = entity->dword140 + 0x60000;

    StateMachine_Run(entity->state);

    if (entity->leaderboardEntry) {
        if (entity->leaderboardEntry != entity->prevLeaderboardEntry || entity->leaderboardEntry->status != entity->prevLeaderboardEntryStatus) {
            UIRankButton_SetupRank(entity, entity->leaderboardEntry);
            entity->prevLeaderboardEntry       = entity->leaderboardEntry;
            entity->prevLeaderboardEntryStatus = entity->leaderboardEntry->status;
        }
    }

    EntityUIControl *control = (EntityUIControl *)entity->parent;
    if (control && entity->state == UIRankButton_Unknown13 && !control->popoverHasFocus
        && (control->state != UIControl_ProcessInputs || control->buttons[control->activeEntityID] != (EntityUIButton *)entity)) {
        entity->flag  = false;
        entity->state = UIRankButton_Unknown12;
    }
}

void UIRankButton_LateUpdate(void) {}

void UIRankButton_StaticUpdate(void) {}

void UIRankButton_Draw(void)
{
    RSDK_THIS(UIRankButton);
    EntityUIControl *control   = (EntityUIControl *)entity->parent;
    EntityUICarousel *carousel = control->carousel;

    bool32 setClip = false;

    int newClipX1 = 0;
    int newClipY1 = 0;
    int newClipX2 = 0;
    int newClipY2 = 0;
    if (carousel && (carousel->clipSize.x > 0 || carousel->clipSize.y > 0)) {
        setClip   = true;
        newClipX2 = carousel->clipSize.x;
        newClipY2 = carousel->clipSize.y;
        newClipX1 = carousel->clipOffset.x + carousel->position.x;
        newClipY1 = carousel->clipOffset.y + carousel->position.y;
    }
    if (entity->field_14C || entity->field_148)
        setClip = false;

    int clipX1 = RSDK_screens->clipBound_X1;
    int clipY1 = RSDK_screens->clipBound_Y1;
    int clipX2 = RSDK_screens->clipBound_X2;
    int clipY2 = RSDK_screens->clipBound_Y2;
    if (setClip)
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, (newClipX1 >> 16) - RSDK_screens->position.x - (newClipX2 >> 17),
                           (newClipY1 >> 16) - RSDK_screens->position.y - (newClipY2 >> 17),
                           (newClipX1 >> 16) - RSDK_screens->position.x + (newClipX2 >> 17),
                           (newClipX2 >> 17) + (newClipY1 >> 16) - RSDK_screens->position.y);
    UIRankButton_Unknown4();
    if (setClip)
        RSDK.SetClipBounds(RSDK_sceneInfo->currentScreenID, clipX1, clipY1, clipX2, clipY2);
}

void UIRankButton_Create(void *data)
{
    RSDK_THIS(UIRankButton);
    entity->posUnknown2.x = RSDK_sceneInfo->entity->position.x;
    entity->posUnknown2.y = entity->position.y;
    entity->visible       = true;
    entity->drawOrder     = 2;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x400000;
    entity->dword13C      = 0x180000;
    entity->dword144      = 16;
    entity->dword140      = 0x100000;
    entity->dword12C      = 0;
    entity->rank          = 0;
    entity->score         = 0;
    entity->row           = 0;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(&entity->rankText, "", 0);
        RSDK.SetText(&entity->nameTimeText, "-----", 0);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->nameTimeText);
    }
    entity->processButtonCB = UIButton_Unknown6;
    entity->touchCB         = UIButton_ProcessTouch;
    entity->options3        = UIRankButton_Options3CB;
    entity->failCB          = UIRankButton_FailCB;
    entity->options5        = UIRankButton_Options5CB;
    entity->options6        = UIRankButton_Options6CB;
    entity->options7        = UIRankButton_Options7CB;
    entity->options8        = UIRankButton_Options8CB;
    entity->dword158        = true;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 9, &entity->animator3, true, 12);
    RSDK.SetSpriteAnimation(UIWidgets->saveSelectSpriteIndex, 9, &entity->animator4, true, 13);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
}

void UIRankButton_StageLoad(void) {}

void UIRankButton_SetRankText(EntityUIRankButton *button, int rank)
{
    char buffer[8];
    if (rank)
        sprintf(buffer, "%d", rank);
    else
        sprintf(buffer, "-");

    RSDK.PrependText(&button->rankText, buffer);
    RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &button->rankText);
}

void UIRankButton_Unknown2(int rank, EntityUIRankButton *button, int score, int row)
{
    button->rank  = rank;
    button->score = score;
    button->row   = 0;
    if (row && globals->replayTableID != -1 && API.GetUserDBByID(globals->replayTableID, row) != -1)
        button->row = row;

    button->leaderboardEntry = NULL;
    button->dword12C         = 0;
    if (!RSDK_sceneInfo->inEditor)
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

void UIRankButton_Unknown4(void)
{
    RSDK_THIS(UIRankButton);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;

    int val2 = 0xA40000;
    if (!entity->showsName)
        val2 = 0x840000;

    int val = 0xB20000;
    if (entity->showsName)
        val = 0xF20000;

    drawPos.x = 0x110000 - ((val + 0x150000) >> 1) + drawPos.x;
    UIRankButton_Unknown5(0, drawPos.x, drawPos.y, 0x220000, 0x100000);

    int drawX = val2 + drawPos.x;
    int drawY = drawPos.y;
    UIRankButton_Unknown5(0x000000, drawX, drawPos.y, val, 0x100000);
    if (entity->state != UIRankButton_Unknown14 || !((entity->timer >> 1) % 2)) {
        if (!RSDK_sceneInfo->inEditor) {
            drawPos.x = drawPos.x + entity->field_14C;
            drawPos.y = entity->field_148 + drawPos.y + entity->field_14C;

            if (entity->rank && entity->rank <= 3) {
                entity->animator4.frameID = entity->rank + 12;
                RSDK.DrawSprite(&entity->animator4, &drawPos, false);
            }

            int width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->rankText, 0, entity->rankText.textLength, 0);
            drawPos.x -= width << 15;
            RSDK.DrawText(&entity->animator2, &drawPos, &entity->rankText, 0, entity->rankText.textLength, ALIGN_LEFT, 0, 2, 0, false);
        }

        if (entity->showsName && !RSDK_sceneInfo->inEditor) {
            drawPos.x = drawX - (val >> 1) + 0x20000;
            drawPos.x += entity->field_14C;
            drawPos.y = drawY + entity->field_14C;
            drawPos.y += entity->field_148;
            int len = entity->nameTimeText.textLength;
            if (len >= 20)
                len = 20;
            RSDK.DrawText(&entity->animator2, &drawPos, &entity->nameTimeText, 0, len, ALIGN_LEFT, 0, 2, 0, false);
        }

        drawPos.x = drawX + entity->field_14C;
        drawPos.x -= 0x200000;
        drawPos.y = (drawY + entity->field_14C) + (entity->field_148 - 0x20000);
        if (entity->showsName)
            drawPos.x += 0x3C0000;

        UIWidgets_Unknown11(entity->score / 6000, entity->score % 6000 / 100, entity->score % 100, drawPos.x, drawPos.y);

        if (!RSDK_sceneInfo->inEditor) {
            if (API.CheckDLC(DLC_PLUS)) {
                if (entity->row) {
                    drawPos.x = entity->position.x + ((val + 0x150000) >> 1);
                    drawPos.y = entity->position.y;
                    if (entity->showsName)
                        drawPos.x += 0xC0000;
                    drawPos.x += entity->field_14C;
                    drawPos.y += entity->field_14C;
                    drawPos.y += entity->field_148;
                    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

                    entity->field_124 = drawPos.x;
                    entity->field_128 = drawPos.y;
                    entity->field_128 -= 0x80000;
                }
            }
        }
    }
}

void UIRankButton_Unknown5(int colour, int a2, int a3, int a4, int a5)
{
    RSDK_THIS(UIRankButton);

    if (entity->field_14C)
        UIWidgets_Unknown7(a5 >> 16, a4 >> 16, a5 >> 16, (UIWidgets->value >> 16) & 0xFF, (UIWidgets->value >> 8) & 0xFF, UIWidgets->value & 0xFF,
                           a2 - entity->field_14C, a3 - entity->field_14C);
    UIWidgets_Unknown7(a5 >> 16, a4 >> 16, a5 >> 16, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF, entity->field_14C + a2,
                       entity->field_14C + a3);
}

bool32 UIRankButton_Options7CB(void)
{
    RSDK_THIS(UIRankButton);
    return entity->state == UIRankButton_Unknown13;
}

bool32 UIRankButton_Options8CB(void)
{
    RSDK_THIS(UIRankButton);
    return entity->state == UIRankButton_Unknown14;
}

void UIRankButton_Options5CB(void)
{
    RSDK_THIS(UIRankButton);
    if (entity->state != UIRankButton_Unknown13) {
        entity->field_148 = 0;
        entity->field_14C = 0;
        entity->field_150 = -0x20000;
        entity->field_154 = -0x20000;
        entity->state     = UIRankButton_Unknown13;
    }
}

void UIRankButton_Options6CB(void)
{
    RSDK_THIS(UIRankButton);
    entity->state = UIRankButton_Unknown12;
}

void UIRankButton_FailCB(void) { RSDK.PlaySFX(UIWidgets->sfx_Fail, false, 255); }

void UIRankButton_Options3CB(void)
{
    RSDK_THIS(UIRankButton);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    control->backoutTimer    = 30;
    entity->timer            = 0;
    entity->state            = UIRankButton_Unknown14;
    RSDK.PlaySFX(UIWidgets->sfx_Accept, false, 255);
}

void UIRankButton_Unknown12(void)
{
    RSDK_THIS(UIRankButton);
    if (entity->field_148) {
        int dist = -(entity->field_148 / abs(entity->field_148));
        entity->field_148 += dist << 15;
        if (dist < 0) {
            if (entity->field_148 < 0) {
                entity->field_148 = 0;
            }
            else if (dist > 0 && entity->field_148 > 0)
                entity->field_148 = 0;
        }
        else if (dist > 0 && entity->field_148 > 0)
            entity->field_148 = 0;
    }

    if (entity->field_14C) {
        int dist = -(entity->field_14C / abs(entity->field_14C));
        entity->field_14C += dist << 16;
        if (dist < 0) {
            if (entity->field_14C < 0) {
                entity->field_14C = 0;
            }
            else if (dist > 0 && entity->field_14C > 0)
                entity->field_14C = 0;
        }
        else if (dist > 0 && entity->field_14C > 0)
            entity->field_14C = 0;
    }
}

void UIRankButton_Unknown13(void)
{
    RSDK_THIS(UIRankButton);
    entity->field_150 += 0x4000;
    entity->field_148 += entity->field_150;
    if (entity->field_148 >= 0 && entity->field_150 > 0) {
        entity->field_148 = 0;
        entity->field_150 = 0;
    }

    entity->field_154 += 0x4800;
    entity->field_14C += entity->field_154;
    if (entity->field_14C >= -0x20000 && entity->field_154 > 0) {
        entity->field_14C = -0x20000;
        entity->field_154 = 0;
    }
}

void UIRankButton_Unknown14(void)
{
    RSDK_THIS(UIRankButton);
    UIRankButton_Unknown13();
    if (++entity->timer == 30) {
        entity->timer = 0;
        StateMachine_Run(entity->options2);
        entity->state = UIRankButton_Unknown13;
    }

    entity->dword158 = !((entity->timer >> 1) & 1);
}

void UIRankButton_EditorDraw(void) {}

void UIRankButton_EditorLoad(void) {}

void UIRankButton_Serialize(void) { RSDK_EDITABLE_VAR(UIRankButton, VAR_BOOL, showsName); }
#endif
