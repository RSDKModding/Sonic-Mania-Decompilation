// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UITAZoneModule Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUITAZoneModule *UITAZoneModule;

void UITAZoneModule_Update(void)
{
    RSDK_THIS(UITAZoneModule);

    UITAZoneModule_Setup();

    self->active = ACTIVE_NORMAL;

    if (self->disabled != self->wasDisabled) {
        UITAZoneModule_SetupText();
        self->wasDisabled = self->disabled;
    }

    self->drawPos = self->position;

#if !MANIA_USE_PLUS
    if (self->expandAmount != self->prevExpandAmount)
        self->prevExpandAmount = self->expandAmount;
    if (self->isExpanding) {
        self->expandTimer++;
        self->position.y = self->startPos.y + (self->expandAmount >> 2);
    }

    if (self->announceTimer) {
        --self->announceTimer;
        if (self->announceTimer == 120) {
            RSDK.PlaySfx(UIWidgets->sfxEvent, false, 0xFF);
        }
        if (self->announceTimer == 30) {
            if (self->fuzzDir == 1) {
                RSDK.PlaySfx(Announcer->sfxNewRecordTop, false, 0xFF);
            }
            else if (self->fuzzDir <= 3) {
                RSDK.PlaySfx(Announcer->sfxNewRecordMid, false, 0xFF);
            }
        }
    }

    self->drawPos   = self->position;
    self->drawPos.y = self->position.y - (self->prevExpandAmount >> 1);
#endif

    StateMachine_Run(self->state);

    self->zoneIconSprX++;
    if (self->zoneIconSprX >= 192)
        self->zoneIconSprX -= 192;

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->state == UITAZoneModule_State_Selected
        && (parent->buttons[parent->lastButtonID] != (EntityUIButton *)self || parent->state != UIControl_ProcessInputs)) {
        self->isSelected = false;
        self->state      = UITAZoneModule_State_NotSelected;
    }
}

void UITAZoneModule_LateUpdate(void) {}

void UITAZoneModule_StaticUpdate(void)
{
#if MANIA_USE_PLUS
    if (UITAZoneModule->showLBPrompt)
        UITAZoneModule->showLBPrompt = false;

    if (UITAZoneModule->isEncoreMode != TimeAttackMenu->encoreMode) {
        UITAZoneModule->showLBPrompt = true;
        UITAZoneModule->isEncoreMode = TimeAttackMenu->encoreMode;
    }
#else
    EntityUIButtonPrompt *prompt = UITAZoneModule->leaderboardsPrompt;
    if (prompt) {
        prompt->active  = ACTIVE_ALWAYS;
        prompt->visible = UITAZoneModule->showLBPrompt;
    }
#endif
}

void UITAZoneModule_Draw(void)
{
    UITAZoneModule_DrawBGShapes();
    UITAZoneModule_DrawModuleInfo();

#if !MANIA_USE_PLUS
    RSDK_THIS(UITAZoneModule);

    if (self->prevExpandAmount > 0) {
        RSDK.SetClipBounds(SceneInfo->currentScreenID, ((self->drawPos.x - 0x990000) >> 16) - ScreenInfo->position.x,
                           ((self->drawPos.y + 0x240000) >> 16) - 16 - ScreenInfo->position.y,
                           ((self->drawPos.x - 0x990000) >> 16) + 306 - ScreenInfo->position.x,
                           ((self->drawPos.y + 0x240000 + self->prevExpandAmount) >> 16) - ScreenInfo->position.y);

        UITAZoneModule_DrawExpandedView();

        RSDK.SetClipBounds(SceneInfo->currentScreenID, 0, 0, ScreenInfo->size.x, ScreenInfo->size.y);
    }
#endif
}

void UITAZoneModule_Create(void *data)
{
    RSDK_THIS(UITAZoneModule);

    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    self->processButtonCB = UIButton_ProcessButtonCB_Scroll;
    self->state           = UITAZoneModule_State_Init;

    if (!SceneInfo->inEditor) {
        RSDK.InitString(&self->text1Store, "", false);
        RSDK.InitString(&self->text2Store, "", false);
        RSDK.CopyString(&self->text1Store, &self->text1);
        RSDK.CopyString(&self->text2Store, &self->text2);
    }

    UITAZoneModule_SetupText();
}

void UITAZoneModule_StageLoad(void)
{
#if MANIA_USE_PLUS
    UITAZoneModule->isEncoreMode = false;
    UITAZoneModule->showLBPrompt = false;
#endif

    UITAZoneModule->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
}

void UITAZoneModule_Setup(void)
{
    RSDK_THIS(UITAZoneModule);

    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = 0;

    self->checkButtonEnterCB = UITAZoneModule_CheckButtonEnterCB;
    self->checkSelectedCB    = UITAZoneModule_CheckSelectedCB;

    if (self->processButtonCB == UIButton_ProcessButtonCB_Scroll) {
        self->touchPosSizeS.x = 0x1380000;
        self->touchPosSizeS.y = 0x4E0000;

        self->touchCB       = UIButton_ProcessTouchCB_Single;
        self->selectedCB    = UITAZoneModule_SelectedCB;
        self->failCB        = UITAZoneModule_FailCB;
        self->buttonEnterCB = UITAZoneModule_ButtonEnterCB;
        self->buttonLeaveCB = UITAZoneModule_ButtonLeaveCB;
    }
#if !MANIA_USE_PLUS
    else if (self->processButtonCB == UITAZoneModule_ProcessButtonCB_Expanded) {
        self->touchPosSizeM[0].x   = 0x9C0000;
        self->touchPosSizeM[0].y   = 0x2C0000;
        self->touchPosSizeM[1].x   = 0x9C0000;
        self->touchPosSizeM[1].y   = 0x2C0000;
        self->touchPosOffsetM[0].x = -0x4E0000;
        self->touchPosOffsetM[1].x = 0x4E0000;

        self->touchPosCallbacks[0] = UITAZoneModule_TouchCB_Left;
        self->touchPosCallbacks[1] = UITAZoneModule_TouchCB_Right;
        self->touchPosCount        = 2;

        self->buttonEnterCB = StateMachine_None;
        self->buttonLeaveCB = StateMachine_None;
        self->touchCB       = UIButton_ProcessTouchCB_Multi;
        self->selectedCB    = StateMachine_None;
        self->failCB        = StateMachine_None;
    }
#endif
    else {
        self->touchPosSizeS.x = 0;
        self->touchPosSizeS.y = 0;
        self->touchPosCount   = 0;

        self->selectedCB    = StateMachine_None;
        self->failCB        = StateMachine_None;
        self->buttonEnterCB = StateMachine_None;
        self->buttonLeaveCB = StateMachine_None;
    }
}

void UITAZoneModule_SetupText(void)
{
    RSDK_THIS(UITAZoneModule);

    if (!SceneInfo->inEditor) {
        if (!self->text1.chars)
            RSDK.InitString(&self->text1, "", 0);

        if (!self->text2.chars)
            RSDK.InitString(&self->text2, "", 0);

        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 3, &self->medLetterAnimator1, false, 0);
        RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 3, &self->medLetterAnimator2, false, 0);

        if (self->disabled) {
            RSDK.SetString(&self->text1, "???");
            RSDK.SetString(&self->text2, "");
            self->text2.length = 0;
        }
        else {
            RSDK.CopyString(&self->text1, &self->text1Store);
            RSDK.CopyString(&self->text2, &self->text2Store);
        }

        RSDK.SetSpriteString(UIWidgets->uiFrames, 3, &self->text1);
        RSDK.SetSpriteString(UIWidgets->uiFrames, 3, &self->text2);
    }

    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 10, &self->fuzzAnimator, false, 0);
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 11, &self->zoneIconsAnimator, true, self->zoneID);
}

void UITAZoneModule_DrawBGShapes(void)
{
    RSDK_THIS(UITAZoneModule);

    uint32 color = 0x5FA0B0;
#if MANIA_USE_PLUS
    if (self->isEncore)
        color = 0xF26C4F;
#endif

    int32 drawY = self->drawPos.y + 0x230000;
    UIWidgets_DrawRightTriangle(self->drawPos.x + 0x790000, drawY, -71, 88, 112, 224);

    RSDK.DrawRect(self->drawPos.x + 0x790000, drawY - 0x480000, 0x200000, 0x480000, 0x5870E0, 255, INK_NONE, false);
    RSDK.DrawRect(self->drawPos.x - 0x990000, self->drawPos.y - 0x1C0000, 0x1320000, 0x2C0000, 0, 255, INK_NONE, false);

    UIWidgets_DrawRightTriangle(self->drawPos.x + 0x990000, self->drawPos.y + 0x230000, -71, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

#if MANIA_USE_PLUS
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(self->position.x + 0x30000, self->position.y + 0x30000, 312, 78);

    if (self->isSelected)
        UIWidgets_DrawRectOutline_Flash(self->position.x, self->position.y, 312, 78);
    else
        UIWidgets_DrawRectOutline_Black(self->position.x, self->position.y, 312, 78);
#else
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(self->position.x + 0x30000, self->position.y + 0x30000, 312, 78 + (self->prevExpandAmount >> 16));

    if (self->isSelected || self->isExpanding)
        UIWidgets_DrawRectOutline_Flash(self->position.x, self->position.y, 312, 78 + (self->prevExpandAmount >> 16));
    else
        UIWidgets_DrawRectOutline_Black(self->position.x, self->position.y, 312, 78 + (self->prevExpandAmount >> 16));
#endif
}

void UITAZoneModule_DrawFGShapes(void)
{
    RSDK_THIS(UITAZoneModule);

    Vector2 drawPos = self->drawPos;
    uint32 color    = 0xF0F0F0;
#if MANIA_USE_PLUS
    if (self->isEncore)
        color = 0xF26C4F;
#endif

    drawPos.x -= 0x390000;
    drawPos.y += 0x170000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x840000, 0xD0000, color, 255, INK_NONE, false);

#if MANIA_USE_PLUS
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->timeAttackAnimator, true, (self->isEncore != 0) + 10);
#else
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->timeAttackAnimator, true, 10);
#endif
    RSDK.DrawSprite(&self->timeAttackAnimator, &drawPos, false);

#if MANIA_USE_PLUS
    drawPos = UIWidgets_DrawTriJoinRect(drawPos.x + 0x840000, drawPos.y, color, 0xF0D808);
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x400000, 0xD0000, 0xF0D808, 255, INK_NONE, false);
#else
    drawPos.x += 0x5C0000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 12, 0xF0, 0xF0, 0xF0);

    drawPos.x += 0xE0000;
    drawPos.y += 0xC0000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, -12, 0xE8, 0x28, 0x58);

    drawPos.y -= 0xC0000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x680000, 0xD0000, 0xE82858, 0xFF, INK_NONE, false);

    drawPos.x = self->drawPos.x + 0x430000;
    drawPos.y = (self->drawPos.y + 0xD0000) + 0x100000;
    RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->timeAttackAnimator, true, 9);
    RSDK.DrawSprite(&self->timeAttackAnimator, &drawPos, false);

    drawPos.x += 0x100000;
    drawPos.y -= 0x20000;
    if (SceneInfo->inEditor || !self->characterID) {
        UITAZoneModule_DrawTime(drawPos.x, drawPos.y, 0, 0, 0);
    }
    else {
        uint16 *records1 = TimeAttackData_GetRecordedTime(self->zoneID, 0, self->characterID, 1);
        uint16 *records2 = TimeAttackData_GetRecordedTime(self->zoneID, 1, self->characterID, 1);

        int32 time = records1[0] + records2[0];

        if (records1[0] || records2[0])
            UITAZoneModule_DrawTime(drawPos.x, drawPos.y, time / 6000, time % 6000 / 100, time % 100);
        else if (self->characterID != 3 && records1[0] && self->zoneID == 7)
            UITAZoneModule_DrawTime(drawPos.x, drawPos.y, time / 6000, time % 6000 / 100, time % 100);
        else
            UITAZoneModule_DrawTime(drawPos.x, drawPos.y, 0, 0, 0);
    }
#endif
}

void UITAZoneModule_DrawZonePreview(void)
{
    RSDK_THIS(UITAZoneModule);

    Vector2 drawPos;
    drawPos.x = self->drawPos.x - 0x690000;
    drawPos.y = self->drawPos.y;
    UIWidgets_DrawRectOutline_Black(drawPos.x, drawPos.y, 96, 72);

    if (SceneInfo->inEditor || !self->isSelected || self->disabled) {
        self->direction = self->fuzzDir;
        self->drawFX    = FX_FLIP;
        RSDK.DrawSprite(&self->fuzzAnimator, &drawPos, false);

        self->direction = FLIP_NONE;
        self->drawFX    = FX_NONE;
    }
    else {
        SpriteFrame *frame = RSDK.GetFrame(UITAZoneModule->aniFrames, 11, self->zoneID);
        frame->pivotX      = -45;
        frame->width       = 90;
        frame->sprX        = self->zoneIconSprX;

        if (self->zoneIconSprX <= 102) {
            RSDK.DrawSprite(&self->zoneIconsAnimator, &drawPos, false);
        }
        else {
            int32 width  = self->zoneIconSprX - 102;
            frame->width = 90 - width;
            RSDK.DrawSprite(&self->zoneIconsAnimator, &drawPos, false);

            frame->pivotX += frame->width;
            frame->sprX  = 0;
            frame->width = width;
            RSDK.DrawSprite(&self->zoneIconsAnimator, &drawPos, false);
        }
    }
}

void UITAZoneModule_DrawModuleInfo(void)
{
    RSDK_THIS(UITAZoneModule);

    Vector2 drawPos = self->position;

    UITAZoneModule_DrawFGShapes();
    UITAZoneModule_DrawZonePreview();

    drawPos.x = self->drawPos.x;
    drawPos.y = self->drawPos.y;
    if (self->state != UITAZoneModule_State_HasBeenSelected || !(self->timer & 2)) {
        if (!SceneInfo->inEditor) {
            drawPos.x = self->drawPos.x - 0x370000;

            if (self->text2.length) {
                drawPos.y = self->drawPos.y - 0x160000;
                RSDK.DrawText(&self->medLetterAnimator1, &drawPos, &self->text1, 0, self->text1.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);

                drawPos.y += 0x1C0000;
                drawPos.x += 0x200000;
                RSDK.DrawText(&self->medLetterAnimator2, &drawPos, &self->text2, 0, self->text2.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);
            }
            else {
                drawPos.y = self->drawPos.y - 0x60000;
                RSDK.DrawText(&self->medLetterAnimator1, &drawPos, &self->text1, 0, self->text1.length, ALIGN_LEFT, 0, INT_TO_VOID(2), NULL, false);
            }
        }
    }
}

void UITAZoneModule_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UITAZoneModule_SelectedCB(void)
{
    RSDK_THIS(UITAZoneModule);

    self->timer = 0;
    self->state = UITAZoneModule_State_HasBeenSelected;

#if !MANIA_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    parent->childHasFocus   = true;
    self->processButtonCB   = StateMachine_None;

    foreach_all(UITAZoneModule, module)
    {
        if (module != self)
            module->state = UITAZoneModule_State_MoveOffScreen;
    }
#endif

    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);

#if MANIA_USE_PLUS
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 4, (32 * (self->zoneID % 12)), 0, 224, 32);
    else
#endif
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 1, (32 * (self->zoneID % 12)), 0, 224, 32);
}

bool32 UITAZoneModule_CheckButtonEnterCB(void)
{
    RSDK_THIS(UITAZoneModule);

    return self->state == UITAZoneModule_State_Selected;
}

bool32 UITAZoneModule_CheckSelectedCB(void)
{
    RSDK_THIS(UITAZoneModule);

    return self->state == UITAZoneModule_State_HasBeenSelected;
}

void UITAZoneModule_ButtonEnterCB(void)
{
    RSDK_THIS(UITAZoneModule);

    self->isSelected = true;
    self->state      = UITAZoneModule_State_Selected;

#if !MANIA_USE_PLUS
    self->rank = 0;
#endif

#if MANIA_USE_PLUS
    if (TimeAttackMenu->encoreMode)
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 4, (32 * (self->zoneID % 12)), 0, 224, 32);
    else
#endif
        RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 1, (32 * (self->zoneID % 12)), 0, 224, 32);
}

void UITAZoneModule_ButtonLeaveCB(void)
{
    RSDK_THIS(UITAZoneModule);

    self->isSelected = false;
    self->state      = UITAZoneModule_State_NotSelected;
}

void UITAZoneModule_TransitionCB(void) { UIControl_MatchMenuTag("Leaderboards"); }

void UITAZoneModule_ShowLeaderboards_CB(void) { UITransition_StartTransition(UITAZoneModule_TransitionCB, 0); }

void UITAZoneModule_State_Init(void)
{
    RSDK_THIS(UITAZoneModule);

#if !MANIA_USE_PLUS
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    foreach_all(UIButtonPrompt, prompt)
    {
        if (UIControl_ContainsPos(parent, &prompt->position) && prompt->buttonID == 3)
            UITAZoneModule->leaderboardsPrompt = prompt;
    }
#endif

    self->state = UITAZoneModule_State_NotSelected;
    UITAZoneModule_State_NotSelected();
}

void UITAZoneModule_State_NotSelected(void)
{
    RSDK_THIS(UITAZoneModule);

    self->position.x = self->startPos.x;
    RSDK.ProcessAnimation(&self->fuzzAnimator);

    self->touchCB = UIButton_ProcessTouchCB_Single;
    self->fuzzDir = self->fuzzAnimator.frameID & 3;
}

void UITAZoneModule_State_Selected(void)
{
    RSDK_THIS(UITAZoneModule);

    RSDK.ProcessAnimation(&self->fuzzAnimator);
    self->fuzzDir = self->fuzzAnimator.frameID & 3;

#if MANIA_USE_PLUS
    if (UITAZoneModule->showLBPrompt) {
        if (TimeAttackMenu->encoreMode)
            RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 4, (32 * (self->zoneID % 12)), 0, 224, 32);
        else
            RSDK.CopyPalette(((self->zoneID % 12) >> 3) + 1, (32 * (self->zoneID % 12)), 0, 224, 32);
    }
#endif
}

void UITAZoneModule_State_HasBeenSelected(void)
{
    RSDK_THIS(UITAZoneModule);

    self->isSelected = true;
#if !MANIA_USE_PLUS
    self->touchCB = StateMachine_None;
#endif

    if (self->timer >= 30) {
#if !MANIA_USE_PLUS
        self->isExpanding            = true;
        self->expandTimer            = 0;
        self->timer                  = 0;
        self->rank                   = 0;
        self->actID                  = self->zoneID == 7 && self->characterID != 3;
        UITAZoneModule->showLBPrompt = true;
        self->state                  = UITAZoneModule_State_ExpandModule;
#else
        self->timer = 0;
        StateMachine_Run(self->actionCB);
        self->state = UITAZoneModule_State_Selected;
#endif
    }
    else {
        self->timer++;
    }
}

#if !MANIA_USE_PLUS
Vector2 UITAZoneModule_DrawTime(int32 x, int32 y, int32 minutes, int32 seconds, int32 milliseconds)
{
    RSDK_THIS(UITAZoneModule);
    char strBuf[16];

    if (minutes) {
        if (minutes != 99 || seconds != 99 || milliseconds != 99)
            sprintf(strBuf, "%02d:%02d;%02d", minutes, seconds, milliseconds);
        else
            sprintf(strBuf, "<<:<<;<<");
    }
    else {
        if (!seconds && !milliseconds)
            sprintf(strBuf, "<<:<<;<<");
        else
            sprintf(strBuf, "%02d:%02d;%02d", minutes, seconds, milliseconds);
    }

    Vector2 drawPos;
    drawPos.x = x;
    drawPos.y = y;
    for (int32 i = 0; i < 8; ++i) {
        if (!strBuf[i])
            break;

        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 8, &self->timeAttackAnimator, true, (uint8)(strBuf[i] - '0'));
        RSDK.DrawSprite(&self->timeAttackAnimator, &drawPos, false);

        drawPos.x += 0x80000;
    }

    return drawPos;
}

void UITAZoneModule_DrawActInfo_Expanded(void)
{
    RSDK_THIS(UITAZoneModule);

    Vector2 drawPos = self->drawPos;

    uint32 color1 = self->actID != 0 ? 0xE82858 : 0x000000;
    uint32 color2 = self->actID != 1 ? 0xE82858 : 0x000000;

    RSDK.DrawRect(drawPos.x - 0x990000, drawPos.y + 0x240000, 0x860000, 0x150000, color1, 0xFF, INK_NONE, false);

    int32 offset = abs(RSDK.Sin256(8 * MIN(self->expandTimer, 16))) >> 6;
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, 0);
    drawPos.x -= 0x630000;
    drawPos.y += 0x2E0000;

    bool32 disableActID = true;
    if (!self->actID) {
        drawPos.x -= offset << 16;
        drawPos.y -= offset << 16;

        if (self->state == UITAZoneModule_State_StartTimeAttackAttempt && (self->timer & 2)) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
            disableActID = false;
        }
    }

    if (disableActID) {
        if (self->zoneID != 7 || self->characterID == 3) {
            RSDK.DrawSprite(&self->textAnimator, &drawPos, false);

            if (!self->actID) {
                drawPos.x += offset << 16;
                drawPos.y += offset << 16;
            }
        }
    }

    Vector2 drawPos2, drawPos3;
    drawPos2.x = self->drawPos.x - 0x1A0000;
    drawPos2.y = self->drawPos.y + 0x2E0000;

    drawPos.x = self->drawPos.x - 0x130000;
    drawPos.y = self->drawPos.y + 0x240000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, 20, (color1 >> 16) & 0xFF, (color1 >> 8) & 0xFF, color1 & 0xFF);

    drawPos.x += 0x140000;
    drawPos.y += 0x140000;
    UIWidgets_DrawRightTriangle(drawPos.x, drawPos.y, -20, (color2 >> 16) & 0xFF, (color2 >> 8) & 0xFF, color2 & 0xFF);

    drawPos3.x = drawPos.x + 0x40000;
    drawPos3.y = drawPos.y - 0xA0000;
    drawPos.y -= 0x140000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x980000, 0x150000, color2, 255, INK_NONE, false);

    drawPos.x += 0x4B0000;
    drawPos.y += 0xA0000;

    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, 1);

    disableActID = true;
    if (self->actID == 1) {
        drawPos.x -= offset << 16;
        drawPos.y -= offset << 16;

        if (self->state == UITAZoneModule_State_StartTimeAttackAttempt && (self->timer & 2)) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
            disableActID = false;
        }
    }

    if (disableActID) {
        RSDK.DrawSprite(&self->textAnimator, &drawPos, false);

        if (self->actID == 1) {
            drawPos.x += offset << 16;
            drawPos.y += offset << 16;
        }
    }

    self->drawFX    = FX_FLIP;
    self->direction = FLIP_X;
    if (self->actID == 1)
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->textAnimator, true, 3);
    else
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->textAnimator, true, 2);

    if (!self->actID)
        drawPos2.x -= abs(RSDK.Sin256(2 * self->expandTimer) << 10);

    if (self->zoneID != 7 || self->characterID == 3)
        RSDK.DrawSprite(&self->textAnimator, &drawPos2, false);

    self->direction = FLIP_NONE;
    self->drawFX    = FX_NONE;
    if (self->actID == 1)
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->textAnimator, true, 2);
    else
        RSDK.SetSpriteAnimation(UITAZoneModule->aniFrames, 9, &self->textAnimator, true, 3);

    if (self->actID == 1)
        drawPos3.x -= abs(RSDK.Sin256(2 * self->expandTimer) << 10);

    RSDK.DrawSprite(&self->textAnimator, &drawPos3, false);
}

void UITAZoneModule_DrawExpandedView(void)
{
    RSDK_THIS(UITAZoneModule);

    int32 drawX      = self->drawPos.x;
    int32 drawY      = self->drawPos.y;
    Vector2 drawPos2 = self->drawPos;

    uint16 *records = TimeAttackData_GetRecordedTime(self->zoneID, self->actID, self->characterID, 1);

    RSDK.DrawRect(drawX - 0x990000, drawY + 0x240000, 0x1320000, 0x4D0000, 0x000000, 0xFF, INK_NONE, false);

    UITAZoneModule_DrawActInfo_Expanded();
    UIWidgets_DrawParallelogram(self->position.x - 0x40000, drawY + 0x448000, 0xE0, 0x17, 0x17, 0xD9, 0xAD, 0x4);

    drawPos2.x = drawX - 0x390000;
    drawPos2.y = drawY + 0x450000;
    if (self->rank != 1 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, 2);
        RSDK.DrawSprite(&self->textAnimator, &drawPos2, false);

        int32 time = 0;
        if (records)
            time = records[0];

        Vector2 pos;
        pos.x = drawX - 0x10000;
        pos.y = drawY + 0x420000;
        pos   = UITAZoneModule_DrawTime(pos.x, pos.y, time / 6000, time % 6000 / 100, time % 100);
    }

    drawY += 0x140000;

    UIWidgets_DrawParallelogram(self->position.x + 0x80000, drawY + 0x448000, 0xE0, 0x0F, 0x0F, 0x98, 0xC0, 0xC8);

    drawPos2.x = drawX - 0x290000;
    drawPos2.y = drawY + 0x460000;
    if (self->rank != 2 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, 3);
        RSDK.DrawSprite(&self->textAnimator, &drawPos2, 0);

        int32 time = 0;
        if (records)
            time = records[1];

        Vector2 pos;
        pos.x = drawX + 0x140000;
        pos.y = drawY + 0x420000;
        pos   = UITAZoneModule_DrawTime(pos.x, pos.y, time / 6000, time % 6000 / 100, time % 100);
    }

    UIWidgets_DrawParallelogram(self->position.x + 0x140000, drawY + 0x548000, 0xE0, 0xF, 0xF, 0xC0, 0x58, 0x01);

    drawPos2.x = drawX - 0x1A0000;
    drawPos2.y = drawY + 0x560000;
    if (self->rank != 3 || (UIControl->timer & 4)) {
        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 16, &self->textAnimator, true, 4);
        RSDK.DrawSprite(&self->textAnimator, &drawPos2, false);

        int32 time = 0;
        if (records)
            time = records[2];

        Vector2 pos;
        pos.x = drawX + 0x230000;
        pos.y = drawY + 0x520000;
        pos   = UITAZoneModule_DrawTime(pos.x, pos.y, time / 6000, time % 6000 / 100, time % 100);
    }
}

void UITAZoneModule_State_Expanded(void) {}

void UITAZoneModule_State_StartTimeAttackAttempt(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityMenuParam *param = MenuParam_GetParam();

    self->isSelected = true;

    if (self->timer >= 30) {
        TimeAttackData_Clear();

        param->zoneID = self->zoneID;
        param->actID  = self->actID;
        self->timer   = 0;

        StateMachine_Run(self->actionCB);

        self->state           = UITAZoneModule_State_Expanded;
        self->processButtonCB = StateMachine_None;
    }
    else {
        self->timer++;
    }
}

void UITAZoneModule_TouchCB_Left(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (self->zoneID != 7 || self->characterID == 3) {
        self->actID = 0;

        int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
        API_ResetInputSlotAssignments();
        API_AssignInputSlotToDevice(CONT_P1, id);

        parent->state = StateMachine_None;
        self->timer   = 0;
        self->state   = UITAZoneModule_State_StartTimeAttackAttempt;
        Music_Stop();
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
    }
}

void UITAZoneModule_TouchCB_Right(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

    int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
    API_ResetInputSlotAssignments();
    API_AssignInputSlotToDevice(CONT_P1, id);

    parent->state = StateMachine_None;
    self->timer   = 0;
    self->state   = UITAZoneModule_State_StartTimeAttackAttempt;

    Music_Stop();
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
}

void UITAZoneModule_ProcessButtonCB_Expanded(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (UIControl->anyLeftPress && self->actID == 1 && (self->zoneID != 7 || self->characterID == 3)) {
        self->actID = 0;
        self->rank  = 0;

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        self->expandTimer = 0;
    }
    if (UIControl->anyRightPress && !self->actID && (self->zoneID != 7 || self->characterID == 3)) {
        self->actID = 1;
        self->rank  = 0;

        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        self->expandTimer = 0;
    }
    else if (UIControl->anyConfirmPress) {
        int32 id = API_GetFilteredInputDeviceID(INPUT_NONE);
        API_ResetInputSlotAssignments();
        API_AssignInputSlotToDevice(CONT_P1, id);

        parent->state = StateMachine_None;
        self->timer   = 0;
        self->state   = UITAZoneModule_State_StartTimeAttackAttempt;

        Music_Stop();
        RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
    }
    else {
        if (MenuSetup && UIControl->anyYPress) {
            UITAZoneModule_ShowLeaderboards(self->characterID, self->zoneID, self->actID, false, UITAZoneModule_ShowLeaderboards_CB);

            RSDK.PlaySfx(UIWidgets->sfxAccept, false, 0xFF);
        }

        if (UIControl->anyBackPress) {
            self->timer           = 0;
            self->state           = UITAZoneModule_State_ContractModule;
            self->processButtonCB = StateMachine_None;
        }
    }
}

void UITAZoneModule_ShowLeaderboards(int32 player, int32 zone, int32 act, bool32 wasUser, void (*callback)(void))
{
    String string;
    INIT_STRING(string);

    Localization_GetString(&string, STR_CONNECTING);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&string);
    if (dialog) {
        UIDialog_Setup(dialog);
        MenuSetup->connectingDlg = dialog;
        MenuSetup_StartTransitionLB(callback, 120);

        uint16 *records                  = TimeAttackData_GetRecordedTime(zone, act, player, 1);
        EntityUILeaderboard *leaderboard = (EntityUILeaderboard *)MenuSetup->leaderboardWidget;

        int32 isUser = wasUser ? 0 : (*records != 0);

        leaderboard->taRecord        = *records;
        leaderboard->viewingUserRank = isUser;

        APICallback_FetchLeaderboardData(zone, act, player, 0, 100, isUser);
        UILeaderboard_SetupLeaderboard(leaderboard, player, zone, act);
    }
}

void UITAZoneModule_SetStartupModule(EntityUIControl *control, uint8 characterID, uint32 zoneID, uint8 actID, int32 score)
{
    EntityUITAZoneModule *module = (EntityUITAZoneModule *)control->buttons[zoneID];

    control->childHasFocus = true;
    control->buttonID      = zoneID;

    control->position.x = module->startPos.x;
    control->position.y = module->startPos.y;
    control->targetPos  = module->startPos;

    ScreenInfo->position.x = (module->startPos.x >> 16) - ScreenInfo->center.x;
    ScreenInfo->position.y = (control->position.y >> 16) - ScreenInfo->center.y;
    RSDK.CopyPalette((zoneID >> 3) + 1, 32 * zoneID, 0LL, 0xE0, 0x20);

    module->actID                = actID;
    module->state                = UITAZoneModule_State_ExpandModule;
    module->timer                = 16;
    module->isExpanding          = true;
    module->isSelected           = true;
    UITAZoneModule->showLBPrompt = true;

    if (score > 0) {
        module->announceTimer = 150;
        module->rank          = score;
    }

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)control->buttons[i];
        if (button != module) {
            button->state       = UITAZoneModule_State_Inactive;
            button->isExpanding = false;
            button->isSelected  = false;
            button->position.x  = button->startPos.x + (ScreenInfo->size.x << 16);
        }
    }

    EntityUIControl *zoneControl = (EntityUIControl *)MenuSetup->timeAttackZones;
    for (int32 i = 0; i < zoneControl->buttonCount; ++i) {
        EntityUITAZoneModule *button = (EntityUITAZoneModule *)zoneControl->buttons[i];
        button->characterID          = characterID;
    }
}

void UITAZoneModule_State_ExpandModule(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityUIControl *parent            = (EntityUIControl *)self->parent;
    EntityUIHeading *heading           = (EntityUIHeading *)parent->heading;
    EntityUIButtonPrompt *showLBPrompt = UITAZoneModule->leaderboardsPrompt;

    if (self->timer >= 16) {
        self->expandAmount = 0x4D0000;

        foreach_all(UIButtonPrompt, prompt)
        {
            foreach_all(UIButtonPrompt, prompt)
            {
                if (UIControl_ContainsPos(parent, &prompt->position)) {
                    if (prompt->startPos.y >> 16 < ScreenInfo->position.y) {
                        prompt->position.y = (ScreenInfo->position.y << 16) + 0x180000;

                        if (prompt == showLBPrompt)
                            showLBPrompt->position.y += 0x180000;
                    }
                }
            }
        }

        heading->position.y = (ScreenInfo->position.y << 16) + 0x1C0000;

        self->timer           = 0;
        self->state           = UITAZoneModule_State_Expanded;
        self->processButtonCB = UITAZoneModule_ProcessButtonCB_Expanded;
        self->touchCB         = UIButton_ProcessTouchCB_Multi;
    }
    else {
        if (heading->startPos.y >> 16 < ScreenInfo->position.y) {
            int32 pos = (ScreenInfo->position.y << 16) - 0x1C0000;

            if (self->timer) {
                if (16 * self->timer < 0x100)
                    pos += 0x3800 * (RSDK.Sin1024(16 * self->timer) >> 2);
                else
                    pos = (ScreenInfo->position.y << 16) + 0x1C0000;
            }

            heading->position.y = pos;
        }

        foreach_all(UIButtonPrompt, prompt)
        {
            if (UIControl_ContainsPos(parent, &prompt->position)) {
                if (!prompt->buttonID)
                    prompt->promptID = 4;

                if (prompt->startPos.y >> 16 < ScreenInfo->position.y) {
                    int32 offset1 = prompt == showLBPrompt ? 0x300000 : 0x180000;
                    int32 offset2 = prompt == showLBPrompt ? -0x180000 : -0x300000;

                    int32 pos = (ScreenInfo->position.y << 16) + offset2;
                    if (self->timer) {
                        if (16 * self->timer < 0x100)
                            pos += (RSDK.Sin1024(16 * self->timer) >> 2) * (((ScreenInfo->position.y << 16) + offset1 - pos) >> 8);
                        else
                            pos = TO_FIXED(ScreenInfo->position.y) + offset1;
                    }

                    prompt->position.y = pos;
                }
            }
        }

        self->expandAmount = 0;

        if (self->timer) {
            if ((16 * self->timer) < 0x100)
                self->expandAmount = 0x4D00 * (RSDK.Sin1024(16 * self->timer) >> 2);
            else
                self->expandAmount = 0x4D0000;
        }

        self->timer++;
    }
}

void UITAZoneModule_State_ContractModule(void)
{
    RSDK_THIS(UITAZoneModule);

    EntityUIControl *parent            = (EntityUIControl *)self->parent;
    EntityUIHeading *heading           = (EntityUIHeading *)parent->heading;
    EntityUIButtonPrompt *showLBPrompt = UITAZoneModule->leaderboardsPrompt;

    self->processButtonCB = StateMachine_None;
    if (self->timer >= 16) {
        if (self->timer == 16) {
            foreach_all(UITAZoneModule, module)
            {
                if (module != self)
                    module->state = UITAZoneModule_State_ComeBackOnScreen;
            }

            foreach_all(UIButtonPrompt, prompt)
            {
                if (UIControl_ContainsPos(parent, &prompt->position)) {
                    if (!prompt->buttonID)
                        prompt->promptID = 0;

                    if (prompt->startPos.y >> 16 < ScreenInfo->position.y)
                        prompt->position.y = prompt->startPos.y;
                }
            }

            UITAZoneModule->showLBPrompt = false;
            heading->position.y          = heading->startPos.y;

            self->position     = self->startPos;
            self->expandAmount = 0;
            self->isExpanding  = false;
            self->timer++;
        }
        else if (self->timer > 45) {
            parent->childHasFocus = false;
            self->timer           = 0;
            self->state           = UITAZoneModule_State_Selected;
            self->processButtonCB = UIButton_ProcessButtonCB_Scroll;
        }
        else {
            self->timer++;
        }
    }
    else {
        if (heading->startPos.y >> 16 < ScreenInfo->position.y) {
            int32 pos = (ScreenInfo->position.y << 16) + 0x1C0000;

            if (self->timer) {
                if (16 * self->timer < 0x100)
                    pos -= 0x3800 * (RSDK.Sin1024(16 * self->timer) >> 2);
                else
                    pos = (ScreenInfo->position.y << 16) - 0x1C0000;
            }

            heading->position.y = pos;
        }

        foreach_all(UIButtonPrompt, prompt)
        {
            if (UIControl_ContainsPos(parent, &prompt->position)) {
                if (prompt->startPos.y >> 16 < ScreenInfo->position.y) {

                    int32 offset1 = prompt == showLBPrompt ? -TO_FIXED(24) : -TO_FIXED(48);
                    int32 offset2 = prompt == showLBPrompt ? TO_FIXED(48) : TO_FIXED(24);

                    int32 pos = (ScreenInfo->position.y << 16) + offset2;
                    if (self->timer) {
                        if (16 * self->timer < 0x100)
                            pos += (RSDK.Sin1024(16 * self->timer) >> 2) * (((ScreenInfo->position.y << 16) + offset1 - pos) >> 8);
                        else
                            pos = TO_FIXED(ScreenInfo->position.y) + offset1;
                    }

                    prompt->position.y = pos;
                }
            }
        }

        self->expandAmount = 0x4D0000;

        if (self->timer) {
            if (16 * self->timer < 0x100)
                self->expandAmount -= 0x4D00 * (RSDK.Sin1024(16 * self->timer) >> 2);
            else
                self->expandAmount = 0;
        }

        self->timer++;
    }
}

void UITAZoneModule_State_MoveOffScreen(void)
{
    RSDK_THIS(UITAZoneModule);

    RSDK.ProcessAnimation(&self->fuzzAnimator);

    self->fuzzDir    = self->fuzzAnimator.frameID & 3;
    self->velocity.x = 0x200000;

    if ((self->position.x - self->startPos.x) >> 16 < ScreenInfo->size.x) {
        self->position.x += 0x200000;
        self->position.y += self->velocity.y;
    }

    if ((self->position.x - self->startPos.x) >> 16 >= ScreenInfo->size.x) {
        self->position.x = self->startPos.x + (ScreenInfo->size.x << 16);
        self->state      = UITAZoneModule_State_Inactive;
    }
}

void UITAZoneModule_State_Inactive(void)
{
    RSDK_THIS(UITAZoneModule);

    self->position.x = self->startPos.x + (ScreenInfo->size.x << 16);
}

void UITAZoneModule_State_ComeBackOnScreen(void)
{
    RSDK_THIS(UITAZoneModule);

    RSDK.ProcessAnimation(&self->fuzzAnimator);
    self->velocity.x = -0x200000;
    self->fuzzDir    = self->fuzzAnimator.frameID & 3;

    if (self->position.x > self->startPos.x) {
        self->position.x -= 0x200000;
        self->position.y += self->velocity.y;
    }

    if (self->position.x <= self->startPos.x) {
        self->position.x = self->startPos.x;
        self->state      = UITAZoneModule_State_NotSelected;
    }
}

#endif

#if RETRO_INCLUDE_EDITOR
void UITAZoneModule_EditorDraw(void)
{
    RSDK_THIS(UITAZoneModule);

    self->startPos = self->position;
    self->drawPos  = self->position;

    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 3, &self->medLetterAnimator1, false, 0);
    RSDK.SetSpriteAnimation(UIWidgets->uiFrames, 3, &self->medLetterAnimator2, false, 0);

    UITAZoneModule_SetupText();

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;

    UITAZoneModule_Draw();

#if !MANIA_USE_PLUS
    self->expandAmount     = self->debugExpand ? 0x4D0000 : 0;
    self->prevExpandAmount = self->expandAmount;

    if (self->prevExpandAmount > 0)
        UITAZoneModule_DrawExpandedView();
#endif
}

void UITAZoneModule_EditorLoad(void) { UITAZoneModule->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UITAZoneModule_Serialize(void)
{
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_UINT8, zoneID);
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_STRING, text1);
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_STRING, text2);
#if !MANIA_USE_PLUS
    RSDK_EDITABLE_VAR(UITAZoneModule, VAR_BOOL, debugExpand);
#endif
}
