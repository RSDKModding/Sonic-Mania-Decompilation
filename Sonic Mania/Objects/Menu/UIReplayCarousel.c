#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIReplayCarousel *UIReplayCarousel;

void UIReplayCarousel_Update(void)
{
    RSDK_THIS(UIReplayCarousel);
    self->dbUnknownCount = API.GetSortedUserDBRowCount(globals->replayTableID);
    UIReplayCarousel_SetupButtonCallbacks();
    UIReplayCarousel_HandleTouchPositions();
    self->active = ACTIVE_NORMAL;

    self->field_168 = self->field_16C != self->field_170;
    if (self->field_16C >= self->field_170) {
        if (self->field_16C > self->field_170) {
            self->field_16C -= 12;
            if (self->field_16C < self->field_170)
                self->field_16C = self->field_170;
        }
    }
    else {
        self->field_16C += 12;
        if (self->field_16C > self->field_170)
            self->field_16C = self->field_170;
    }
    UIReplayCarousel_Unknown2();

    StateMachine_Run(self->state);

    if (++self->sprX >= 192)
        self->sprX -= 192;
    if (self->dbUnknownID != self->prevDBUnknownID) {
        UIReplayCarousel_Unknown6();
        self->prevDBUnknownID = self->dbUnknownID;
    }

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->replayID > 0) {
        UIControl_Unknown15(parent, 0, self->position.y);

        if (102 * self->replayID - 102 > 0)
            self->field_170 = 102 * self->replayID - 102;
        else
            self->field_170 = 0;
    }
    else {
        UIControl_Unknown15(parent, 0, parent->startPos.y);
        self->field_170 = 0;
    }

    if (self->language != Localization->language && Localization->loaded) {
        self->language = Localization->language;
        Localization_GetString(&self->noReplaysText, STR_NOREPLAYS);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->noReplaysText);
        Localization_GetString(&self->loadingText, STR_LOADING);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->loadingText);
    }
    if (self->state == UIReplayCarousel_Unknown18 && !parent->popoverHasFocus && !parent->dialogHasFocus
        && (parent->buttons[parent->activeEntityID] != (EntityUIButton *)self || parent->state != UIControl_ProcessInputs)) {
        UIReplayCarousel_Unknown12();
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
    self->posUnknown2     = self->position;
    self->active          = ACTIVE_BOUNDS;
    self->drawOrder       = 2;
    self->visible         = true;
    self->drawFX          = FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x300000;
    self->processButtonCB = UIReplayCarousel_ProcessButtonCB;
    self->touchCB         = UIButton_TouchCB_Alt;
    self->state           = UIReplayCarousel_Unknown16;
    self->dbUnknownCount  = 0;
    UIReplayCarousel_Unknown2();
    if (!SceneInfo->inEditor) {
        for (int32 i = 0; i < 4; ++i) {
            RSDK.SetText(&self->zoneNameText[i], "", 0);
            RSDK.SetText(&self->createdAtText[i], "", 0);
        }

        UIReplayCarousel_Unknown6();
        self->prevDBUnknownID = self->dbUnknownID;
        self->language        = Localization->language;
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->animator9, true, 0);
        if (Localization->loaded) {
            Localization_GetString(&self->noReplaysText, STR_NOREPLAYS);
            Localization_GetString(&self->loadingText, STR_LOADING);
        }
        else {
            RSDK.SetText(&self->noReplaysText, "", 0);
            RSDK.SetText(&self->loadingText, "", 0);
        }
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->noReplaysText);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->loadingText);
    }
    RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 10, &self->animator2, false, 0);
    UIReplayCarousel_SetupButtonCallbacks();
}

void UIReplayCarousel_StageLoad(void) { UIReplayCarousel->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIReplayCarousel_ProcessButtonCB(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (!self->field_168 && self->state != UIReplayCarousel_Unknown19) {
        int32 rowID = 0;
        if (parent->rowCount && parent->columnCount)
            rowID = parent->activeEntityID / parent->columnCount;

        int32 columnID = 0;
        if (parent->columnCount)
            columnID = parent->activeEntityID % parent->columnCount;

        int32 id         = self->replayID;
        bool32 movedUp   = false;
        bool32 movedDown = false;
        if (UIControl->keyUp) {
            if (self->replayID) {
                --id;
                movedDown = true;
            }
            else {
                --rowID;
                movedUp = true;
            }
        }

        if (UIControl->keyDown && self->replayID < self->dbUnknownCount - 1) {
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

            if (parent->activeEntityID != selection) {
                parent->activeEntityID = selection;
                UIReplayCarousel_Unknown12();
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
            }
        }
        else {
            if (movedDown) {
                RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
                self->replayID = id;
            }

            if (UIControl->keyConfirm) {
                if (self->disabled) {
                    StateMachine_Run(self->failCB);
                }
                else {
                    if (self->options2) {
                        UIReplayCarousel_Unknown11();
                        return;
                    }
                }
            }

            if (!self->flag) {
                if (parent->buttons[parent->field_D8] == (EntityUIButton *)self && !parent->dialogHasFocus && !parent->popoverHasFocus) {
                    self->flag  = true;
                    self->state = UIReplayCarousel_Unknown18;
                }
            }
        }
    }
}

void UIReplayCarousel_Unknown2(void)
{
    RSDK_THIS(UIReplayCarousel);

    if (self->field_16C) {
        self->dbUnknownID = self->field_16C / 102;

        int32 pos           = 102 * (self->field_16C / 102) - self->field_16C - 102;
        self->field_17C = pos << 16;
        if (pos < 153) {
            self->field_174 = (152 - pos) / 0x66 + 1;
        }
        else {
            self->field_174 = 0;
        }
    }
    else {
        self->dbUnknownID = 0;
        self->field_174   = minVal(self->dbUnknownCount, 3);
        self->field_17C   = -0x660000;
    }
}

void UIReplayCarousel_SetupButtonCallbacks(void)
{
    RSDK_THIS(UIReplayCarousel);
    void *state = self->stateDraw;

    if (self->dbUnknownCount) {
        if (!API.GetUserDBRowsChanged(globals->replayTableID) && globals->replayTableLoaded == STATUS_OK) {
            self->stateDraw       = UIReplayCarousel_StateDraw_Unknown3;
            self->processButtonCB = UIReplayCarousel_ProcessButtonCB;
            self->touchCB         = UIButton_TouchCB_Alt;
        }
        else {
            self->stateDraw       = UIReplayCarousel_StateDraw_Unknown1;
            self->touchCB         = StateMachine_None;
            self->processButtonCB = StateMachine_None;
        }
    }
    else {
        self->stateDraw       = UIReplayCarousel_StateDraw_Unknown2;
        self->touchCB         = StateMachine_None;
        self->processButtonCB = StateMachine_None;
    }

    if (self->stateDraw != state && self->stateDraw == UIReplayCarousel_StateDraw_Unknown3)
        UIReplayCarousel_Unknown6();
}

void UIReplayCarousel_HandleTouchPositions(void)
{
    RSDK_THIS(UIReplayCarousel);

    if (self->touchCB) {
        self->touchPosCount = self->field_174;
        int32 posX              = self->position.x;
        int32 posY              = self->field_17C + self->position.y;

        int32 i = 0;
        for (; i < self->field_174; ++i) {
            if (i + self->dbUnknownID >= self->dbUnknownCount)
                break;

            self->touchPos2[i].x       = posX - self->position.x;
            self->touchPos2[i].y       = posY - self->position.y;
            self->touchPos1[i].x       = 0x1320000;
            self->touchPos1[i].y       = 0x550000;
            self->touchPosCallbacks[i] = UIReplayCarousel_TouchedCB;
            posY += 0x660000;
        }

        for (; i < self->field_174; ++i) {
            self->touchPos2[i].x       = 0;
            self->touchPos2[i].y       = 0;
            self->touchPos1[i].x       = 0;
            self->touchPos1[i].y       = 0;
            self->touchPosCallbacks[i] = StateMachine_None;
        }
    }
}

void UIReplayCarousel_TouchedCB(void)
{
    RSDK_THIS(UIReplayCarousel);
    int32 id = self->touchCountUnknown + self->dbUnknownID;
    if (self->replayID == id)
        UIReplayCarousel_Unknown11();
    else
        self->replayID = id;
}

void UIReplayCarousel_Unknown6(void)
{
    RSDK_THIS(UIReplayCarousel);
    if (!SceneInfo->inEditor) {
        char buffer[0x20];
        for (int32 i = 0; i < 4; ++i) {
            int32 id = i + self->dbUnknownID;
            if (id >= self->dbUnknownCount)
                break;
            int32 row    = API.GetSortedUserDBRowID(globals->replayTableID, id);
            int32 zoneID = 0xFF;
            API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "zoneID", &zoneID);
            API.GetUserDBCreationTime(globals->replayTableID, row, buffer, 31, "%D");
            if (zoneID != 0xFF) {
                RSDK.SetText(&self->zoneNameText[i], "", 0);
                Localization_GetZoneName(&self->zoneNameText[i], zoneID);
                if (zoneID == 5 || zoneID == 10)
                    RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 5, &self->zoneNameText[i]);
                else
                    RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &self->zoneNameText[i]);

                RSDK.SetText(&self->createdAtText[i], buffer, 0);
                RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->createdAtText[i]);
            }
        }
    }
}

void UIReplayCarousel_Unknown7(int32 a1, int16 a2, int32 a3, int32 a4)
{
    RSDK_THIS(UIReplayCarousel);
    if (!SceneInfo->inEditor)
        RSDK.DrawRect(a3 - 0x990000, a4 - 0x2A8000, 0x1320000, 0x550000, 0xFFFFFF, 127, INK_BLEND, false);

    UIWidgets_DrawRightTriangle(a3 + 0x790000, a4 + 0x298000, -76, 88, 112, 224);
    RSDK.DrawRect(a3 + 0x790000, (a4 + 0x298000) - 0x550000, 0x200000, 0x550000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(a3 - 10027008, a4 - 2785280, 20054016, 0x2C0000, 0, 255, INK_NONE, false);

    uint32 colour = 0xF0F0F0;
    if (a1)
        colour = 0xF26C4F;
    UIWidgets_DrawParallelogram(16, 128, 16, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF, a3 - 0xA0000, a4 + 0xE0000);

    colour = 0x5FA0B0;
    if (a1)
        colour = 0xF26C4F;
    UIWidgets_DrawRightTriangle(a3 + 0x990000, a4 + 0x298000, -76, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF);

    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(91, 312, a3 + 0x30000, a4 + 0x30000);

    if (self->replayID == a2)
        UIWidgets_DrawRectOutline_Flash(91, 312, a3, a4);
    else
        UIWidgets_DrawRectOutline_Black(91, 312, a3, a4);
}

void UIReplayCarousel_Unknown8(uint8 a1, uint8 a2, int32 a3, int32 time, int32 arg10, int32 a6, int32 a7)
{
    RSDK_THIS(UIReplayCarousel);
    Vector2 drawPos;
    uint32 colours[] = { 0xE82858, 0x5870E0, 0xF0D808, 0x010101 };
    int32 widths[]   = { 60, 82, 52, 70 };

    drawPos.x = arg10 - 0x990000;
    drawPos.y = a6 + 0x1D8000;

    Vector2 drawOffsets[4];
    for (int32 i = 0; i < 4; ++i) {
        RSDK.DrawRect(drawPos.x, drawPos.y, widths[i] << 16, 0xD0000, colours[i], 255, INK_NONE, false);

        drawOffsets[i].x = drawPos.x + 0x10000;
        drawOffsets[i].y = drawPos.y + 0xC0000;
        drawPos.x += (widths[i] << 16);
        if (i < 3) {
            UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 13, (colours[i] >> 16) & 0xFF, (colours[i] >> 8) & 0xFF, colours[i] & 0xFF);

            drawPos.x += 0xE0000;
            UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y + 0xC0000, -13, (colours[i + 1] >> 16) & 0xFF, (colours[i + 1] >> 8) & 0xFF, colours[i + 1] & 0xFF);
        }
    }

    if (!SceneInfo->inEditor) {
        int32 id    = a7 - self->dbUnknownID;
        drawPos.x = drawOffsets[0].x + 0x1E0000;
        drawPos.y = drawOffsets[0].y;
        int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->createdAtText[id], 0, self->createdAtText[id].textLength, 0);
        drawPos.x -= width << 15;
        drawPos.y -= 0x60000;
        RSDK.DrawText(&self->animator9, &drawPos, &self->createdAtText[id], 0, self->createdAtText[id].textLength, ALIGN_LEFT, 0, 2, 0, false);
    }

    drawPos.x = drawOffsets[1].x + 0x40000;
    drawPos.y = drawOffsets[1].y - 0x80000;
    UIWidgets_DrawTime(drawPos.x, drawPos.y, time / 6000, time % 6000 / 100, time % 100);

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &self->animator3, true, a1 + 5);
    drawPos.x = drawOffsets[2].x + 0x1A0000;
    drawPos.y = drawOffsets[2].y - 0x70000;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 8, &self->animator3, true, a2 - 1);
    drawPos.x = drawOffsets[3].x + 0x230000;
    drawPos.y = drawOffsets[3].y - 0x60000;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}

void UIReplayCarousel_Unknown9(uint8 a1, int16 a2, int32 a4, int32 a5)
{
    RSDK_THIS(UIReplayCarousel);
    Vector2 drawPos;

    drawPos.x = a4 - 0x690000;
    drawPos.y = a5 - 0x70000;
    UIWidgets_DrawRectOutline_Black(72, 96, a4 - 0x690000, a5 - 0x70000);

    if (SceneInfo->inEditor || self->replayID != a2 || self->disabled) {
        self->direction = self->field_158;
        self->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->direction = FLIP_NONE;
        self->drawFX    = FX_NONE;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UIReplayCarousel->aniFrames, 11, a1);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = self->sprX;

        if (self->sprX <= 102) {
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
        }
        else {
            int32 width    = self->sprX - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&self->animator1, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&self->animator1, &drawPos, false);
        }
    }
}

void UIReplayCarousel_Unknown10(uint8 zoneID, int32 a2, uint8 a3, uint8 a4, int32 a5, int32 a6, int32 drawX, int32 drawY)
{
    RSDK_THIS(UIReplayCarousel);
    UIReplayCarousel_Unknown8(a3, a4, a2, a6, drawX, drawY, a2);
    UIReplayCarousel_Unknown9(zoneID, a2, drawX, drawY);

    Vector2 drawPos;
    drawPos.x = drawX;
    drawPos.y = drawY - 0x160000;
    if ((self->state != UIReplayCarousel_Unknown19 || self->replayID != a2 || !(self->timer & 2)) && !SceneInfo->inEditor) {
        int32 id    = a2 - self->dbUnknownID;
        drawPos.x = drawX - 0x390000;
        if (zoneID == 5 || zoneID == 10) {
            RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 5, &self->animators[id], true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &self->animators[id], true, 0);
        }
        RSDK.DrawText(&self->animators[id], &drawPos, &self->zoneNameText[id], 0, self->zoneNameText[id].textLength, ALIGN_LEFT, 0, 2, 0,
                      false);
    }

    drawPos.x = drawX - 0x430000;
    drawPos.y = drawY + 0x60000;
    RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 9, &self->animator3, true, (a5 != 0) + 10);
    RSDK.DrawSprite(&self->animator3, &drawPos, false);
}

void UIReplayCarousel_Unknown11(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->timer             = 0;
    self->state             = UIReplayCarousel_Unknown19;
    self->processButtonCB   = StateMachine_None;
    parent->selectionDisabled = true;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIReplayCarousel_Unknown12(void)
{
    RSDK_THIS(UIReplayCarousel);
    self->flag  = false;
    self->state = UIReplayCarousel_Unknown17;
    if (!UIControl->field_C8)
        self->replayID = -1;
}

void UIReplayCarousel_StateDraw_Unknown1(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    Vector2 drawPos;
    drawPos.x = parent->position.x;
    drawPos.y = parent->position.y + 0x100000;
    UIWidgets_DrawParallelogram(16, 96, 16, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->loadingText, 0, self->loadingText.textLength, 0) << 15;
    RSDK.DrawText(&self->animator9, &drawPos, &self->loadingText, 0, self->loadingText.textLength, ALIGN_LEFT, 0, 2, 0, false);
}

void UIReplayCarousel_StateDraw_Unknown2(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    Vector2 drawPos;
    drawPos.x = parent->position.x;
    drawPos.y = parent->position.y + 0x100000;
    UIWidgets_DrawParallelogram(16, 96, 16, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->noReplaysText, 0, self->noReplaysText.textLength, 0) << 15;
    RSDK.DrawText(&self->animator9, &drawPos, &self->noReplaysText, 0, self->noReplaysText.textLength, ALIGN_LEFT, 0, 2, 0, false);
}

void UIReplayCarousel_StateDraw_Unknown3(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    int32 posY = self->field_17C + self->position.y;
    for (int32 i = 0; i < self->field_174; ++i) {
        int32 id = i + self->dbUnknownID;
        if (id >= self->dbUnknownCount)
            break;

        int32 score        = 0;
        uint16 zoneID      = 0;
        uint16 act         = 0;
        uint16 characterID = 0;
        uint16 encore      = 0;

        int32 row = API.GetSortedUserDBRowID(globals->replayTableID, id);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT32, "score", &score);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "zoneID", &zoneID);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "act", &act);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "characterID", &characterID);
        API.GetUserDBValue(globals->replayTableID, row, DBVAR_UINT8, "encore", &encore);
        if (id == self->replayID && parent->active == ACTIVE_ALWAYS) {
            int32 val = (zoneID % 12) >> 3;
            int32 pal = 0;
            if (encore)
                pal = val + 4;
            else
                pal = val + 1;
            RSDK.CopyPalette(pal, (32 * (zoneID % 12)), 0, 224, 32);
            RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 11, &self->animator1, true, zoneID % 12);
        }
        UIReplayCarousel_Unknown7(encore, id, self->position.x, posY);
        UIReplayCarousel_Unknown10(zoneID % 12, id, act, characterID, encore, score, self->position.x, posY);
        posY += 0x660000;
    }
}

void UIReplayCarousel_Unknown16(void)
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
        if (MathHelpers_PointInHitbox(FLIP_NONE, parent->startPos.x - parent->cameraOffset.x, parent->startPos.y - parent->cameraOffset.y, &hitbox,
                                      prompt->position.x, prompt->position.y)) {
            if (prompt->buttonID == 3) {
                UIReplayCarousel->prompt = prompt;
                foreach_break;
            }
        }
    }

    self->state = UIReplayCarousel_Unknown17;

    self->position.x = self->posUnknown2.x;
    RSDK.ProcessAnimation(&self->animator2);
    self->field_158 = self->animator2.frameID & 3;
}

void UIReplayCarousel_Unknown17(void)
{
    RSDK_THIS(UIReplayCarousel);

    self->position.x = self->posUnknown2.x;
    RSDK.ProcessAnimation(&self->animator2);
    self->field_158 = self->animator2.frameID & 3;
}

void UIReplayCarousel_Unknown18(void)
{
    RSDK_THIS(UIReplayCarousel);

    RSDK.ProcessAnimation(&self->animator2);
    self->field_158 = self->animator2.frameID & 3;
    if (self->replayID == -1)
        self->replayID = 0;
}

void UIReplayCarousel_Unknown19(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    RSDK.ProcessAnimation(&self->animator2);
    self->flag      = true;
    self->field_158 = self->animator2.frameID & 3;
    self->touchCB   = StateMachine_None;

    if (self->timer >= 30) {
        self->timer             = 0;
        parent->selectionDisabled = false;
        StateMachine_Run(self->options2);
        self->state = UIReplayCarousel_Unknown17;
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
    UIWidgets_DrawParallelogram(16, 96, 16, 0, 0, 0, drawPos.x, drawPos.y);
}

void UIReplayCarousel_EditorLoad(void) { UIReplayCarousel->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UIReplayCarousel_Serialize(void) { RSDK_EDITABLE_VAR(UIReplayCarousel, VAR_BOOL, disabled); }
#endif
