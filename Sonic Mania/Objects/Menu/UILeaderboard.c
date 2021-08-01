#include "SonicMania.h"

ObjectUILeaderboard *UILeaderboard;

void UILeaderboard_Update(void)
{
    RSDK_THIS(UILeaderboard);
    if (entity->textSpriteIndex != UIWidgets->textSpriteIndex) {
        UILeaderboard_SetupEntrySprites(entity);
        entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
    StateMachine_Run(entity->state);
    if (++entity->timer2 >= 192)
        entity->timer2 -= 192;

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (entity->state == UILeaderboard_State_Unknown3 && (parent->activeEntityID != entity->frameID || parent->state != UIControl_ProcessInputs)) {
        entity->flag  = false;
        entity->state = UILeaderboard_State_Unknown2;
    }
}

void UILeaderboard_LateUpdate(void) {}

void UILeaderboard_StaticUpdate(void) {}

void UILeaderboard_Draw(void)
{
    UILeaderboard_DrawPrimitives();
    UILeaderboard_DrawEntries();
}

void UILeaderboard_Create(void *data)
{
    RSDK_THIS(UILeaderboard);
    entity->posUnknown2.x      = entity->position.x;
    entity->posUnknown2.y      = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = 2;
    entity->visible       = 1;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x300000;
    entity->processButtonCB    = UILeaderboard_ProcessButtonCB;
    entity->state              = UILeaderboard_State_Unknown1;
    UILeaderboard_SetupEntrySprites(entity);
    entity->textSpriteIndex = UIWidgets->textSpriteIndex;
    if (!RSDK_sceneInfo->inEditor) {
        RSDK.SetText(entity->text1, "", 0);
        RSDK.SetText(&entity->text1[1], "", 0);
        RSDK.SetText(&entity->text1[2], "", 0);
        RSDK.SetText(&entity->text1[3], "", 0);
        RSDK.SetText(&entity->text1[4], "", 0);
        RSDK.SetText(entity->nameText, "", 0);
        RSDK.SetText(&entity->nameText[1], "", 0);
        RSDK.SetText(&entity->nameText[2], "", 0);
        RSDK.SetText(&entity->nameText[3], "", 0);
        RSDK.SetText(&entity->nameText[4], "", 0);
    }
}

void UILeaderboard_StageLoad(void) { UILeaderboard->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UILeaderboard_SetupEntrySprites(EntityUILeaderboard *entity)
{
    RSDK.SetSpriteAnimation(UILeaderboard->aniFrames, 12, &entity->animator6, true, 0);
    RSDK.SetSpriteAnimation(UILeaderboard->aniFrames, 13, &entity->animator3, true, entity->frameID);

    int frame = 0;
    if (entity->frameID2 <= 1)
        frame = 0;
    else
        frame = entity->frameID2 - 1;
    RSDK.SetSpriteAnimation(UILeaderboard->aniFrames, 1, &entity->animator7, true, frame);
    RSDK.SetSpriteAnimation(UILeaderboard->aniFrames, 2, &entity->animator8, true, frame);

    if (entity->frameID2 <= 1)
        frame = 5;
    else
        frame = entity->frameID2 + 4;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 11, &entity->animator9, true, frame);
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 11, &entity->animator10, true, entity->frameID3 + 3);
    RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator5, true, 0);
    UILeaderboard_LoadEntries(entity);
}

void UILeaderboard_LoadEntries(EntityUILeaderboard *entity)
{
    if (!RSDK_sceneInfo->inEditor) {
        for (int i = 0; i < 5; ++i) {
            LeaderboardEntry *entry = API.ReadLeaderboardEntry(i + entity->entryOffset);
            if (entry && entry->status == STATUS_OK) {
                char buffer[0x10];

                if (entry->globalRank < 1)
                    strcpy(buffer, "--");
                else
                    sprintf(buffer, "%d", entry->globalRank);
                RSDK.PrependText(&entity->rankText[i], buffer);
                RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->rankText[i]);

                RSDK.CopyString(&entity->nameText[i], &entry->username);
                if (entity->nameText[i].textLength > 19) {
                    entity->nameText[i].textLength = 19;
                    RSDK.AppendText(&entity->nameText[i], "...");
                }
                RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->nameText[i]);
                entity->times[i]  = entry->score;
                entity->ranks[i]  = entry->globalRank;
                entity->isUser[i] = entry->isUser;
            }
            else {
                RSDK.PrependText(&entity->rankText[i], "--");
                RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->rankText[i]);

                RSDK.PrependText(&entity->nameText[i], "--------");
                RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->nameText[i]);

                entity->times[i] = 0;
                if (entry) {
                    entity->ranks[i] = entry->globalRank;
                }
                else {
                    entity->ranks[i] = 0;
                }
                entity->isUser[i] = false;
            }
        }
    }
}

void UILeaderboard_DrawPrimitives(void)
{
    RSDK_THIS(UILeaderboard);
    Vector2 drawPos;

    if (!RSDK_sceneInfo->inEditor)
        UIWidgets_Unknown3(158, 414, RSDK_sceneInfo->entity->position.x + 0x30000, RSDK_sceneInfo->entity->position.y + 0x30000);

    UIWidgets_Unknown2(158, 414, entity->position.x, entity->position.y);

    drawPos.x = entity->position.x - 0x8C0000;
    drawPos.y = entity->position.y - 0x4C0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x1120000, 0x180000, 0, 255, INK_NONE, false);

    drawPos.y += 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x580000, 0x100000, 0xF0F0F0, 255, INK_NONE, false);

    drawPos.x += 0x580000;
    UIWidgets_Unknown5(240, 15, 240, 240, drawPos.x, drawPos.y);

    drawPos.x += 0xF0000;
    drawPos.y += 0xF0000;
    UIWidgets_Unknown5(232, -15, 40, 88, drawPos.x, drawPos.y);

    drawPos.y -= 0xF0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x3A0000, 0x100000, 0xE82858, 255, INK_NONE, false);

    drawPos.x += 0x3A0000;
    UIWidgets_Unknown5(232, 15, 40, 88, drawPos.x, drawPos.y);

    drawPos.x += 0xF0000;
    drawPos.y += 0xF0000;
    UIWidgets_Unknown5(96, -15, 160, 176, drawPos.x, drawPos.y);

    drawPos.y -= 0xF0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x3A0000, 0x100000, 0x60A0B0, 255, INK_NONE, false);

    drawPos.x += 0x3A0000;
    UIWidgets_Unknown5(96, 15, 160, 176, drawPos.x, drawPos.y);

    drawPos.y += 0xF0000;
    drawPos.x += 0x270000;
    UIWidgets_Unknown5(88, -39, 112, 224, drawPos.x, drawPos.y);

    RSDK.DrawRect(drawPos.x, drawPos.y - 0x270000, 0x3F0000, 0x280000, 0x5870E0, 255, INK_NONE, false);

    drawPos.y = entity->position.y - 0x240000;
    drawPos.x = entity->position.x - 0xCC0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x680000, 0x200000, 0x60A0AB, 255, INK_NONE, false);

    drawPos.x += 0x680000;
    UIWidgets_Unknown5(96, 31, 160, 176, drawPos.x, drawPos.y);

    drawPos.y += 0x1F0000;
    drawPos.x += 0x1F0000;
    UIWidgets_Unknown5(88, -31, 112, 224, drawPos.x, drawPos.y);

    drawPos.y -= 0x1F0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x790000, 0x200000, 0x5870E0, 255, INK_NONE, false);

    drawPos.x += 0x790000;
    UIWidgets_Unknown5(88, 31, 112, 224, drawPos.x, drawPos.y);

    drawPos.y += 0x1F0000;
    drawPos.x += 0x1F0000;
    UIWidgets_Unknown5(232, -31, 40, 88, drawPos.x, drawPos.y);

    RSDK.DrawRect(drawPos.x, drawPos.y - 0x1F0000, 0x790000, 0x200000, 0xE82858, 255, INK_NONE, false);

    RSDK.DrawRect(entity->position.x - 0xCC0000, entity->position.y - 0x40000, 0x1980000, 0x500000, 0, 255, INK_NONE, false);
}

void UILeaderboard_DrawEntries(void)
{
    RSDK_THIS(UILeaderboard);
    Vector2 drawPos;

    UILeaderboard_Unknown3();
    drawPos.x = entity->position.x + 0x8C0000;
    drawPos.y = entity->position.y - 0x4C0000;
    if (!RSDK_sceneInfo->inEditor) {
        drawPos.x += 0xC0000;
        drawPos.y += 0x70000;
        RSDK.DrawText(&entity->animator4, &drawPos, &entity->field_1E0, 0, entity->field_1E0.textLength, ALIGN_LEFT, 0, 0, 0, false);

        drawPos.x -= 0xC0000;
    }
    drawPos.y += 0x1C0000;
    RSDK.SetSpriteAnimation(UILeaderboard->aniFrames, 9, &entity->animator11, true, 10);
    RSDK.DrawSprite(&entity->animator11, &drawPos, false);

    drawPos.x += 0x6F0000;
    drawPos.y += 0x40000;
    RSDK.DrawSprite(&entity->animator10, &drawPos, false);

    drawPos.x += 0x490000;
    RSDK.DrawSprite(&entity->animator9, &drawPos, false);

    drawPos.x = entity->position.x + 0xA10000;
    drawPos.y = entity->position.y - 0x350000;
    RSDK.DrawSprite(&entity->animator8, &drawPos, false);

    drawPos.x -= 0x30000;
    drawPos.y -= 0x30000;
    RSDK.DrawSprite(&entity->animator7, &drawPos, false);

    drawPos.x = entity->position.x - 0x980000;
    drawPos.y = entity->position.y - 0x100000;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 11, &entity->animator11, true, 0);
    RSDK.DrawSprite(&entity->animator11, &drawPos, false);

    drawPos.x += 0x8E0000;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 11, &entity->animator11, true, 1);
    RSDK.DrawSprite(&entity->animator11, &drawPos, false);

    drawPos.x += 0x8E0000;
    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 11, &entity->animator11, true, 2);
    RSDK.DrawSprite(&entity->animator11, &drawPos, false);

    for (int i = 0; i < 5; ++i) UILeaderboard_DrawRank(i);
}

void UILeaderboard_Unknown3(void)
{
    RSDK_THIS(UILeaderboard);
    Vector2 drawPos;

    drawPos.x = entity->position.x = 0xA80000;
    drawPos.y                      = entity->position.y - 0x380000;
    UIWidgets_Unknown2(40, 72, drawPos.x, drawPos.x);

    if (RSDK_sceneInfo->inEditor) {
        entity->direction = entity->editorFlip;
        entity->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&entity->animator6, &drawPos, false);
        entity->direction = FLIP_NONE;
        entity->drawFX    = FX_NONE;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UILeaderboard->aniFrames, 13, entity->frameID);
        frame->pivotX      = -33;
        frame->width       = 66;
        frame->sprX        = entity->timer;
        if (entity->timer <= 126) {
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        }
        else {
            frame->width = 66 - (entity->timer - 126);
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = entity->timer - 126;
            RSDK.DrawSprite(&entity->animator3, &drawPos, false);
        }
    }
}

void UILeaderboard_DrawTime(int mins, int secs, int millisecs, int x, int y)
{
    RSDK_THIS(UILeaderboard);
    Vector2 drawPos;
    char scoreText[0x10];

    if (mins == 99 && secs == 99 && millisecs == 99)
        sprintf(scoreText, "<<:<<;<<");
    else if (!mins && !secs && !millisecs)
        sprintf(scoreText, "<<:<<;<<");
    else
        sprintf(scoreText, "%02d:%02d;%02d", mins, secs, millisecs);

    drawPos.x = x;
    drawPos.y = y;
    for (int i = 0; i < 8; ++i) {
        if (!scoreText[i])
            break;
        RSDK.SetSpriteAnimation(UILeaderboard->aniFrames, 8, &entity->animator12, true, (scoreText[i] - '0'));
        RSDK.DrawSprite(&entity->animator12, &drawPos, false);
        drawPos.x += 0x80000;
    }
}

void UILeaderboard_DrawRank(int id)
{
    RSDK_THIS(UILeaderboard);
    Vector2 drawPos;

    drawPos.x = (id << 20) + entity->position.x - 0x200000;
    drawPos.y = (id << 20) + entity->position.y + 0x40000;

    int colour = 0x5870E0;
    switch (entity->ranks[id]) {
        default: break;
        case 1: colour = 0xD9AD04; break;
        case 2: colour = 0x98C0C8; break;
        case 3: colour = 0xC05802; break;
    }
    UIWidgets_Unknown7(15, 169, 15, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF, drawPos.x, drawPos.y);

    drawPos.x -= 0x7A0000;
    if (!entity->isUser[id] || !(UIControl->timer & 4)) {
        if (RSDK_sceneInfo->inEditor) {
            drawPos.x += 0xE60000;
        }
        else {
            RSDK.DrawText(&entity->animator5, &drawPos, &entity->rankText[id], 0, entity->rankText[id].textLength, ALIGN_RIGHT, 0, 0, 0, false);
            drawPos.x += 0x320000;
            RSDK.DrawText(&entity->animator5, &drawPos, &entity->nameText[id], 0, entity->nameText[id].textLength, ALIGN_LEFT, 0, 0, 0, false);
            drawPos.x += 0xB40000;
        }

        int min, sec, ms;
        TimeAttackData_GetTimeFromValue(entity->times[id], &min, &sec, &ms);
        drawPos.y -= 0x40000;
        UILeaderboard_DrawTime(min, sec, ms, drawPos.x, drawPos.y);
    }
}

void UILeaderboard_State_Unknown1(void)
{
    RSDK_THIS(UILeaderboard);
    entity->state = UILeaderboard_State_Unknown2;
    RSDK.ProcessAnimation(&entity->animator6);
    entity->editorFlip = entity->animator6.frameID & 3;
}

void UILeaderboard_State_Unknown2(void)
{
    RSDK_THIS(UILeaderboard);
    RSDK.ProcessAnimation(&entity->animator6);
    entity->editorFlip = entity->animator6.frameID & 3;
}

void UILeaderboard_State_Unknown3(void)
{
    RSDK_THIS(UILeaderboard);
    RSDK.ProcessAnimation(&entity->animator6);
    entity->editorFlip = entity->animator6.frameID & 3;
}

void UILeaderboard_ProcessButtonCB(void)
{
    RSDK_THIS(UILeaderboard);
    /*int entryCount = API.LeaderboardEntryCount();

    v5             = v4;
    int oldID      = entity->entryOffset;
    int newID      = entity->entryOffset;
    if (UIControl->keyUp) {
        newID = oldID - 1;
    }
    else if (UIControl->keyDown) {
        newID = oldID + 1;
    }
    else if (UIControl->keyLeft) {
        newID = oldID - 5;
    }
    else if (UIControl->keyRight) {
        newID = oldID + 5;
    }

    v9     = entryCount + v5;
    if ((entryCount + v5 - 5) < newID)
        newID = v9 - 5;
    if (entryCount > newID)
        newID = entryCount;
    if (v5 && oldID != newID) {
        LogHelpers_Print("old: %d, new: %d", oldID, newID);
        entity->entryOffset = newID;
        UILeaderboard_LoadEntries(entity);
        RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
        if (entryCount <= 1 || newID >= entryCount + 2) {
            if (newID > (entryCount + v5 - 7)) {
                LogHelpers_Print("Load down");
                API.Unknown12(entryCount, v5 + 20, 2);
            }
        }
        else {
            LogHelpers_Print("Load up");
            API.Unknown12(entryCount - 20, v5 + 20, 1);
        }
    }*/

    if (UIControl->keyY && entity->field_1D8) {
        entity->yPressCB();
        RSDK.PlaySFX(UIWidgets->sfx_Bleep, false, 255);
    }
    if (entity->state == UILeaderboard_State_Unknown2 || !entity->flag) {
        entity->flag  = true;
        entity->state = UILeaderboard_State_Unknown3;
        entity->field_1CC  = 0;
    }
}

void UILeaderboard_EditorDraw(void) {}

void UILeaderboard_EditorLoad(void) {}

void UILeaderboard_Serialize(void) { RSDK_EDITABLE_VAR(UILeaderboard, VAR_BOOL, disabled); }