#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectUIReplayCarousel *UIReplayCarousel;

void UIReplayCarousel_Update(void)
{
    RSDK_THIS(UIReplayCarousel);
    entity->dbUnknownCount = API.GetSortedUserDBRowCount(globals->replayTableID);
    UIReplayCarousel_SetupButtonCallbacks();
    UIReplayCarousel_HandleTouchPositions();
    entity->active = ACTIVE_NORMAL;

    entity->field_168 = entity->field_16C != entity->field_170;
    if (entity->field_16C >= entity->field_170) {
        if (entity->field_16C > entity->field_170) {
            entity->field_16C -= 12;
            if (entity->field_16C < entity->field_170)
                entity->field_16C = entity->field_170;
        }
    }
    else {
        entity->field_16C += 12;
        if (entity->field_16C > entity->field_170)
            entity->field_16C = entity->field_170;
    }
    UIReplayCarousel_Unknown2();

    StateMachine_Run(entity->state);

    if (++entity->sprX >= 192)
        entity->sprX -= 192;
    if (entity->dbUnknownID != entity->prevDBUnknownID) {
        UIReplayCarousel_Unknown6();
        entity->prevDBUnknownID = entity->dbUnknownID;
    }

    EntityUIControl *parent = (EntityUIControl *)entity->parent;
    if (entity->field_164 > 0) {
        UIControl_Unknown15(parent, 0, entity->position.y);

        if (102 * entity->field_164 - 102 > 0)
            entity->field_170 = 102 * entity->field_164 - 102;
        else
            entity->field_170 = 0;
    }
    else {
        UIControl_Unknown15(parent, 0, parent->startPos.y);
        entity->field_170 = 0;
    }

    if (entity->language != Localization->language && Localization->loaded) {
        entity->language = Localization->language;
        Localization_GetString(&entity->noReplaysText, STR_NOREPLAYS);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->noReplaysText);
        Localization_GetString(&entity->loadingText, STR_LOADING);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->loadingText);
    }
    if (entity->state == UIReplayCarousel_Unknown18 && !parent->popoverHasFocus && !parent->dialogHasFocus
        && (parent->buttons[parent->activeEntityID] != (EntityUIButton *)entity || parent->state != UIControl_ProcessInputs)) {
        UIReplayCarousel_Unknown12();
    }
}

void UIReplayCarousel_LateUpdate(void) {}

void UIReplayCarousel_StaticUpdate(void) {}

void UIReplayCarousel_Draw(void)
{
    RSDK_THIS(UIReplayCarousel);
    StateMachine_Run(entity->stateDraw);
}

void UIReplayCarousel_Create(void *data)
{
    RSDK_THIS(UIReplayCarousel);
    entity->posUnknown2     = entity->position;
    entity->active          = ACTIVE_BOUNDS;
    entity->drawOrder       = 2;
    entity->visible         = true;
    entity->drawFX          = FX_FLIP;
    entity->updateRange.x   = 0x800000;
    entity->updateRange.y   = 0x300000;
    entity->processButtonCB = UIReplayCarousel_ProcessButtonCB;
    entity->touchCB         = UIButton_TouchCB_Alt;
    entity->state           = UIReplayCarousel_Unknown16;
    entity->dbUnknownCount  = 0;
    UIReplayCarousel_Unknown2();
    if (!SceneInfo->inEditor) {
        for (int32 i = 0; i < 4; ++i) {
            RSDK.SetText(&entity->zoneNameText[i], "", 0);
            RSDK.SetText(&entity->createdAtText[i], "", 0);
        }

        UIReplayCarousel_Unknown6();
        entity->prevDBUnknownID = entity->dbUnknownID;
        entity->language        = Localization->language;
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator9, true, 0);
        if (Localization->loaded) {
            Localization_GetString(&entity->noReplaysText, STR_NOREPLAYS);
            Localization_GetString(&entity->loadingText, STR_LOADING);
        }
        else {
            RSDK.SetText(&entity->noReplaysText, "", 0);
            RSDK.SetText(&entity->loadingText, "", 0);
        }
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->noReplaysText);
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->loadingText);
    }
    RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 10, &entity->animator2, false, 0);
    UIReplayCarousel_SetupButtonCallbacks();
}

void UIReplayCarousel_StageLoad(void) { UIReplayCarousel->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIReplayCarousel_ProcessButtonCB(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    if (!entity->field_168 && entity->state != UIReplayCarousel_Unknown19) {
        int32 rowID = 0;
        if (parent->rowCount && parent->columnCount)
            rowID = parent->activeEntityID / parent->columnCount;

        int32 columnID = 0;
        if (parent->columnCount)
            columnID = parent->activeEntityID % parent->columnCount;

        int32 id           = entity->field_164;
        bool32 movedUp   = false;
        bool32 movedDown = false;
        if (UIControl->keyUp) {
            if (entity->field_164) {
                --id;
                movedDown = true;
            }
            else {
                --rowID;
                movedUp = true;
            }
        }

        if (UIControl->keyDown && entity->field_164 < entity->dbUnknownCount - 1) {
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
                entity->field_164 = id;
            }

            if (UIControl->keyConfirm) {
                if (entity->disabled) {
                    StateMachine_Run(entity->failCB);
                }
                else {
                    if (entity->options2) {
                        UIReplayCarousel_Unknown11();
                        return;
                    }
                }
            }

            if (!entity->flag) {
                if (parent->buttons[parent->field_D8] == (EntityUIButton *)entity && !parent->dialogHasFocus && !parent->popoverHasFocus) {
                    entity->flag  = true;
                    entity->state = UIReplayCarousel_Unknown18;
                }
            }
        }
    }
}

void UIReplayCarousel_Unknown2(void)
{
    RSDK_THIS(UIReplayCarousel);

    if (entity->field_16C) {
        entity->dbUnknownID = entity->field_16C / 102;

        int32 pos           = 102 * (entity->field_16C / 102) - entity->field_16C - 102;
        entity->field_17C = pos << 16;
        if (pos < 153) {
            entity->field_174 = (152 - pos) / 0x66 + 1;
        }
        else {
            entity->field_174 = 0;
        }
    }
    else {
        entity->dbUnknownID = 0;
        entity->field_174   = minVal(entity->dbUnknownCount, 3);
        entity->field_17C   = -0x660000;
    }
}

void UIReplayCarousel_SetupButtonCallbacks(void)
{
    RSDK_THIS(UIReplayCarousel);
    void *state = entity->stateDraw;

    if (entity->dbUnknownCount) {
        if (!API.GetUserDBRowsChanged(globals->replayTableID) && globals->replayTableLoaded == STATUS_OK) {
            entity->stateDraw       = UIReplayCarousel_StateDraw_Unknown3;
            entity->processButtonCB = UIReplayCarousel_ProcessButtonCB;
            entity->touchCB         = UIButton_TouchCB_Alt;
        }
        else {
            entity->stateDraw       = UIReplayCarousel_StateDraw_Unknown1;
            entity->touchCB         = StateMachine_None;
            entity->processButtonCB = StateMachine_None;
        }
    }
    else {
        entity->stateDraw       = UIReplayCarousel_StateDraw_Unknown2;
        entity->touchCB         = StateMachine_None;
        entity->processButtonCB = StateMachine_None;
    }

    if (entity->stateDraw != state && entity->stateDraw == UIReplayCarousel_StateDraw_Unknown3)
        UIReplayCarousel_Unknown6();
}

void UIReplayCarousel_HandleTouchPositions(void)
{
    RSDK_THIS(UIReplayCarousel);

    if (entity->touchCB) {
        entity->touchPosCount = entity->field_174;
        int32 posX              = entity->position.x;
        int32 posY              = entity->field_17C + entity->position.y;

        int32 i = 0;
        for (; i < entity->field_174; ++i) {
            if (i + entity->dbUnknownID >= entity->dbUnknownCount)
                break;

            entity->touchPos2[i].x       = posX - entity->position.x;
            entity->touchPos2[i].y       = posY - entity->position.y;
            entity->touchPos1[i].x       = 0x1320000;
            entity->touchPos1[i].y       = 0x550000;
            entity->touchPosCallbacks[i] = UIReplayCarousel_TouchedCB;
            posY += 0x660000;
        }

        for (; i < entity->field_174; ++i) {
            entity->touchPos2[i].x       = 0;
            entity->touchPos2[i].y       = 0;
            entity->touchPos1[i].x       = 0;
            entity->touchPos1[i].y       = 0;
            entity->touchPosCallbacks[i] = StateMachine_None;
        }
    }
}

void UIReplayCarousel_TouchedCB(void)
{
    RSDK_THIS(UIReplayCarousel);
    int32 id = entity->touchCountUnknown + entity->dbUnknownID;
    if (entity->field_164 == id)
        UIReplayCarousel_Unknown11();
    else
        entity->field_164 = id;
}

void UIReplayCarousel_Unknown6(void)
{
    RSDK_THIS(UIReplayCarousel);
    if (!SceneInfo->inEditor) {
        char buffer[0x20];
        for (int32 i = 0; i < 4; ++i) {
            int32 id = i + entity->dbUnknownID;
            if (id >= entity->dbUnknownCount)
                break;
            int32 row    = API.GetSortedUserDBRowID(globals->replayTableID, id);
            int32 zoneID = 0xFF;
            API.GetUserDBValue(globals->replayTableID, row, 2, "zoneID", &zoneID);
            API.GetUserDBCreationTime(globals->replayTableID, row, buffer, 31, "%D");
            if (zoneID != 0xFF) {
                RSDK.SetText(&entity->zoneNameText[i], "", 0);
                Localization_GetZoneName(&entity->zoneNameText[i], zoneID);
                if (zoneID == 5 || zoneID == 10)
                    RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 5, &entity->zoneNameText[i]);
                else
                    RSDK.SetSpriteString(UIWidgets->uiSpriteIndex, 3, &entity->zoneNameText[i]);

                RSDK.SetText(&entity->createdAtText[i], buffer, 0);
                RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->createdAtText[i]);
            }
        }
    }
}

void UIReplayCarousel_Unknown7(int32 a1, int16 a2, int32 a3, int32 a4)
{
    RSDK_THIS(UIReplayCarousel);
    if (!SceneInfo->inEditor)
        RSDK.DrawRect(a3 - 0x990000, a4 - 0x2A8000, 0x1320000, 0x550000, 0xFFFFFF, 127, INK_BLEND, false);

    UIWidgets_Unknown5(88, -76, 112, 224, a3 + 0x790000, a4 + 0x298000);
    RSDK.DrawRect(a3 + 0x790000, (a4 + 0x298000) - 0x550000, 0x200000, 0x550000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(a3 - 10027008, a4 - 2785280, 20054016, 0x2C0000, 0, 255, INK_NONE, false);

    uint32 colour = 0xF0F0F0;
    if (a1)
        colour = 0xF26C4F;
    UIWidgets_Unknown7(16, 128, 16, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, colour & 0xFF, a3 - 0xA0000, a4 + 0xE0000);

    colour = 0x5FA0B0;
    if (a1)
        colour = 0xF26C4F;
    UIWidgets_Unknown5((colour >> 16) & 0xFF, -76, (colour >> 8) & 0xFF, colour & 0xFF, a3 + 0x990000, a4 + 0x298000);

    if (!SceneInfo->inEditor)
        UIWidgets_Unknown3(91, 312, a3 + 0x30000, a4 + 0x30000);

    if (entity->field_164 == a2)
        UIWidgets_Unknown4(91, 312, a3, a4);
    else
        UIWidgets_Unknown2(91, 312, a3, a4);
}

void UIReplayCarousel_Unknown8(uint8 a1, uint8 a2, int32 a3, int32 a4, int32 arg10, int32 a6, int32 a7)
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
            UIWidgets_Unknown5((colours[i] >> 16) & 0xFF, 13, (colours[i] >> 8) & 0xFF, colours[i] & 0xFF, drawPos.x, drawPos.y);

            drawPos.x += 0xE0000;
            UIWidgets_Unknown5((colours[i + 1] >> 16) & 0xFF, -13, (colours[i + 1] >> 8) & 0xFF, colours[i + 1] & 0xFF, drawPos.x,
                               drawPos.y + 0xC0000);
        }
    }

    if (!SceneInfo->inEditor) {
        int32 id    = a7 - entity->dbUnknownID;
        drawPos.x = drawOffsets[0].x + 0x1E0000;
        drawPos.y = drawOffsets[0].y;
        int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->createdAtText[id], 0, entity->createdAtText[id].textLength, 0);
        drawPos.x -= width << 15;
        drawPos.y -= 0x60000;
        RSDK.DrawText(&entity->animator9, &drawPos, &entity->createdAtText[id], 0, entity->createdAtText[id].textLength, ALIGN_LEFT, 0, 2, 0, false);
    }

    drawPos.x = drawOffsets[1].x + 0x40000;
    drawPos.y = drawOffsets[1].y - 0x80000;
    UIWidgets_Unknown11(a4 / 6000, a4 % 6000 / 100, a4 % 100, drawPos.x, drawPos.y);

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 16, &entity->animator3, true, a1 + 5);
    drawPos.x = drawOffsets[2].x + 0x1A0000;
    drawPos.y = drawOffsets[2].y - 0x70000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);

    RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 8, &entity->animator3, true, a2 - 1);
    drawPos.x = drawOffsets[3].x + 0x230000;
    drawPos.y = drawOffsets[3].y - 0x60000;
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}

void UIReplayCarousel_Unknown9(uint8 a1, int16 a2, int32 a4, int32 a5)
{
    RSDK_THIS(UIReplayCarousel);
    Vector2 drawPos;

    drawPos.x = a4 - 0x690000;
    drawPos.y = a5 - 0x70000;
    UIWidgets_Unknown2(72, 96, a4 - 0x690000, a5 - 0x70000);

    if (SceneInfo->inEditor || entity->field_164 != a2 || entity->disabled) {
        entity->direction = entity->field_158;
        entity->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);

        entity->direction = FLIP_NONE;
        entity->drawFX    = FX_NONE;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UIReplayCarousel->aniFrames, 11, a1);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = entity->sprX;

        if (entity->sprX <= 102) {
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
        }
        else {
            int32 width    = entity->sprX - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&entity->animator1, &drawPos, false);
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
    if ((entity->state != UIReplayCarousel_Unknown19 || entity->field_164 != a2 || !(entity->timer & 2)) && !SceneInfo->inEditor) {
        int32 id    = a2 - entity->dbUnknownID;
        drawPos.x = drawX - 0x390000;
        if (zoneID == 5 || zoneID == 10) {
            RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 5, &entity->animators[id], true, 0);
        }
        else {
            RSDK.SetSpriteAnimation(UIWidgets->uiSpriteIndex, 3, &entity->animators[id], true, 0);
        }
        RSDK.DrawText(&entity->animators[id], &drawPos, &entity->zoneNameText[id], 0, entity->zoneNameText[id].textLength, ALIGN_LEFT, 0, 2, 0,
                      false);
    }

    drawPos.x = drawX - 0x430000;
    drawPos.y = drawY + 0x60000;
    RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 9, &entity->animator3, true, (a5 != 0) + 10);
    RSDK.DrawSprite(&entity->animator3, &drawPos, false);
}

void UIReplayCarousel_Unknown11(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    entity->timer             = 0;
    entity->state             = UIReplayCarousel_Unknown19;
    entity->processButtonCB   = StateMachine_None;
    parent->selectionDisabled = true;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIReplayCarousel_Unknown12(void)
{
    RSDK_THIS(UIReplayCarousel);
    entity->flag  = false;
    entity->state = UIReplayCarousel_Unknown17;
    if (!UIControl->field_C8)
        entity->field_164 = -1;
}

void UIReplayCarousel_StateDraw_Unknown1(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    Vector2 drawPos;
    drawPos.x = parent->position.x;
    drawPos.y = parent->position.y + 0x100000;
    UIWidgets_Unknown7(16, 96, 16, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->loadingText, 0, entity->loadingText.textLength, 0) << 15;
    RSDK.DrawText(&entity->animator9, &drawPos, &entity->loadingText, 0, entity->loadingText.textLength, ALIGN_LEFT, 0, 2, 0, false);
}

void UIReplayCarousel_StateDraw_Unknown2(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    Vector2 drawPos;
    drawPos.x = parent->position.x;
    drawPos.y = parent->position.y + 0x100000;
    UIWidgets_Unknown7(16, 96, 16, 0, 0, 0, drawPos.x, drawPos.y);

    drawPos.x -= RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->noReplaysText, 0, entity->noReplaysText.textLength, 0) << 15;
    RSDK.DrawText(&entity->animator9, &drawPos, &entity->noReplaysText, 0, entity->noReplaysText.textLength, ALIGN_LEFT, 0, 2, 0, false);
}

void UIReplayCarousel_StateDraw_Unknown3(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    int32 posY = entity->field_17C + entity->position.y;
    for (int32 i = 0; i < entity->field_174; ++i) {
        int32 id = i + entity->dbUnknownID;
        if (id >= entity->dbUnknownCount)
            break;

        int32 score          = 0;
        uint16 zoneID      = 0;
        uint16 act         = 0;
        uint16 characterID = 0;
        uint16 encore      = 0;

        int32 row = API.GetSortedUserDBRowID(globals->replayTableID, id);
        API.GetUserDBValue(globals->replayTableID, row, 4, "score", &score);
        API.GetUserDBValue(globals->replayTableID, row, 2, "zoneID", &zoneID);
        API.GetUserDBValue(globals->replayTableID, row, 2, "act", &act);
        API.GetUserDBValue(globals->replayTableID, row, 2, "characterID", &characterID);
        API.GetUserDBValue(globals->replayTableID, row, 2, "encore", &encore);
        if (id == entity->field_164 && parent->active == ACTIVE_ALWAYS) {
            int32 val = (zoneID % 12) >> 3;
            int32 pal = 0;
            if (encore)
                pal = val + 4;
            else
                pal = val + 1;
            RSDK.CopyPalette(pal, (32 * (zoneID % 12)), 0, 224, 32);
            RSDK.SetSpriteAnimation(UIReplayCarousel->aniFrames, 11, &entity->animator1, true, zoneID % 12);
        }
        UIReplayCarousel_Unknown7(encore, id, entity->position.x, posY);
        UIReplayCarousel_Unknown10(zoneID % 12, id, act, characterID, encore, score, entity->position.x, posY);
        posY += 0x660000;
    }
}

void UIReplayCarousel_Unknown16(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

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

    entity->state = UIReplayCarousel_Unknown17;

    entity->position.x = entity->posUnknown2.x;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->field_158 = entity->animator2.frameID & 3;
}

void UIReplayCarousel_Unknown17(void)
{
    RSDK_THIS(UIReplayCarousel);

    entity->position.x = entity->posUnknown2.x;
    RSDK.ProcessAnimation(&entity->animator2);
    entity->field_158 = entity->animator2.frameID & 3;
}

void UIReplayCarousel_Unknown18(void)
{
    RSDK_THIS(UIReplayCarousel);

    RSDK.ProcessAnimation(&entity->animator2);
    entity->field_158 = entity->animator2.frameID & 3;
    if (entity->field_164 == -1)
        entity->field_164 = 0;
}

void UIReplayCarousel_Unknown19(void)
{
    RSDK_THIS(UIReplayCarousel);
    EntityUIControl *parent = (EntityUIControl *)entity->parent;

    RSDK.ProcessAnimation(&entity->animator2);
    entity->flag      = true;
    entity->field_158 = entity->animator2.frameID & 3;
    entity->touchCB   = StateMachine_None;

    if (entity->timer >= 30) {
        entity->timer             = 0;
        parent->selectionDisabled = false;
        StateMachine_Run(entity->options2);
        entity->state = UIReplayCarousel_Unknown17;
    }
    else {
        entity->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIReplayCarousel_EditorDraw(void)
{
    RSDK_THIS(UIReplayCarousel);

    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y + 0x100000;
    UIWidgets_Unknown7(16, 96, 16, 0, 0, 0, drawPos.x, drawPos.y);
}

void UIReplayCarousel_EditorLoad(void) { UIReplayCarousel->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UIReplayCarousel_Serialize(void) { RSDK_EDITABLE_VAR(UIReplayCarousel, VAR_BOOL, disabled); }
#endif
