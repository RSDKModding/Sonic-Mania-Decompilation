// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIVsZoneButton Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIVsZoneButton *UIVsZoneButton;

void UIVsZoneButton_Update(void)
{
    RSDK_THIS(UIVsZoneButton);

    self->touchPosSizeS.x   = 0x500000;
    self->touchPosSizeS.y   = 0x380000;
    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = 0;

    self->disabled = self->obfuscate || self->xOut;

    if (self->obfuscate != self->prevObfuscate) {
        UIVsZoneButton_SetNameText();
        self->prevObfuscate = self->obfuscate;
    }

    StateMachine_Run(self->state);

    if (self->isSelected) {
        self->triBounceVelocity -= 0x600;
        self->triBounceOffset += self->triBounceVelocity;

        if (self->triBounceOffset <= 0x10000 && self->triBounceVelocity < 0) {
            self->triBounceOffset   = 0x10000;
            self->triBounceVelocity = 0;
        }

        self->unusedBounceVelocity -= 0x1800;
        self->unusedBounceOffset += self->unusedBounceVelocity;

        if (self->unusedBounceOffset <= 0x8000 && self->unusedBounceVelocity < 0) {
            self->unusedBounceOffset   = 0x8000;
            self->unusedBounceVelocity = 0;
        }
    }
    else if (self->state != UIVsZoneButton_State_Selected) {
        if (self->triBounceOffset > 0)
            self->triBounceOffset = self->triBounceOffset >> 1;
        if (self->unusedBounceOffset > 0)

            self->unusedBounceOffset = self->unusedBounceOffset >> 1;
    }

    if (self->zoneID == UIVSZONEBUTTON_FUZZ || self->xOut)
        RSDK.ProcessAnimation(&self->zoneAnimator);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->state == UIVsZoneButton_State_HandleButtonEnter) {
        if (parent->state != UIControl_ProcessInputs) {
            self->isSelected = false;
            self->state      = UIVsZoneButton_State_HandleButtonLeave;
        }
        else {
            int32 id = -1;
            for (int32 i = 0; i < parent->buttonCount; ++i) {
                if (self == (EntityUIVsZoneButton *)parent->buttons[i]) {
                    id = i;
                    break;
                }
            }

            if (parent->buttonID != id) {
                self->isSelected = false;
                self->state      = UIVsZoneButton_State_HandleButtonLeave;
            }
        }
    }
}

void UIVsZoneButton_LateUpdate(void) {}

void UIVsZoneButton_StaticUpdate(void) {}

void UIVsZoneButton_Draw(void)
{
    RSDK_THIS(UIVsZoneButton);

    UIVsZoneButton_SetupAnimators();

    RSDK.DrawRect(self->position.x - 0x250000, self->position.y - 0x190000, 0x500000, 0x380000, 0xFFFFFF, 0x7F, INK_BLEND, false);

    UIVsZoneButton_DrawBG();
    UIVsZoneButton_DrawOutline();
    UIVsZoneButton_DrawButton();

    if (self->state != UIVsZoneButton_State_Selected || !(self->timer & 2))
        UIVsZoneButton_DrawZoneIcon();
}

void UIVsZoneButton_Create(void *data)
{
    RSDK_THIS(UIVsZoneButton);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    self->processButtonCB    = UIButton_ProcessButtonCB_Scroll;
    self->touchCB            = UIButton_ProcessTouchCB_Single;
    self->selectedCB         = UIVsZoneButton_SelectedCB;
    self->failCB             = UIVsZoneButton_FailCB;
    self->buttonEnterCB      = UIVsZoneButton_ButtonEnterCB;
    self->buttonLeaveCB      = UIVsZoneButton_ButtonLeaveCB;
    self->checkButtonEnterCB = UIVsZoneButton_CheckButtonEnterCB;
    self->checkSelectedCB    = UIVsZoneButton_CheckSelectedCB;
    self->state              = UIVsZoneButton_State_HandleButtonLeave;

    UIVsZoneButton_SetupAnimators();
    UIVsZoneButton_SetNameText();
}

void UIVsZoneButton_StageLoad(void)
{
    UIVsZoneButton->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsZoneButton->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);

    UIVsZoneButton->sfxFail = RSDK.GetSfx("Stage/Fail.wav");
}

void UIVsZoneButton_SetupAnimators(void)
{
    RSDK_THIS(UIVsZoneButton);

    if (SceneInfo->inEditor || self->zoneID == UIVSZONEBUTTON_FUZZ || self->xOut)
        RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 16, &self->zoneAnimator, false, 0);
    else
        RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 17, &self->zoneAnimator, false, self->zoneID);

    RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 14, &self->redCrossAnimator, false, 5);
    RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 14, &self->blackCrossAnimator, false, 6);

    self->size.x     = 0x300000;
    self->size.y     = 0xD0000;
    self->bgEdgeSize = 13;
}

void UIVsZoneButton_SetNameText(void)
{
    RSDK_THIS(UIVsZoneButton);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &self->textAnimator, true, 0);

        if (self->obfuscate) {
            RSDK.SetString(&self->nameText, "???");
            self->nameText.length = 3;
        }
        else {
            Localization_GetZoneInitials(&self->nameText, self->zoneID);

            if (self->zoneID < UIVSZONEBUTTON_FUZZ) {
                char buffer[0x10];
                sprintf_s(buffer, (int32)sizeof(buffer), " %d", self->act + 1);
                RSDK.AppendText(&self->nameText, buffer);
            }
        }

        RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &self->nameText);
    }
}

void UIVsZoneButton_DrawOutline(void)
{
    RSDK_THIS(UIVsZoneButton);

    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(self->position.x + 0x30000, self->position.y + 0x30000, 80, 56);

    if (self->isSelected)
        UIWidgets_DrawRectOutline_Flash(self->position.x, self->position.y, 80, 56);
    else
        UIWidgets_DrawRectOutline_Black(self->position.x, self->position.y, 80, 56);
}

void UIVsZoneButton_DrawBG(void)
{
    RSDK_THIS(UIVsZoneButton);

    UIWidgets_DrawRightTriangle(self->position.x - 0x250000, self->position.y - 0x190000, (23 * self->triBounceOffset) >> 16, 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x250000, self->position.y + 0x180000, (-29 * self->triBounceOffset) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x250000, self->position.y + 0x180000, (-17 * self->triBounceOffset) >> 16, 88, 112, 224);
}

void UIVsZoneButton_DrawZoneIcon(void)
{
    RSDK_THIS(UIVsZoneButton);

    EntityUIControl *parent = (EntityUIControl *)self->parent;

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    if (parent && (parent->active == ACTIVE_ALWAYS || parent->dialogHasFocus) && RSDK.CheckOnScreen(parent, &parent->updateRange)
        && !SceneInfo->inEditor && self->zoneID != UIVSZONEBUTTON_FUZZ && !self->xOut) {
        RSDK.CopyPalette(((self->zoneID >> 3) + 1), (32 * self->zoneID), 0, 224, 32);
    }

    RSDK.DrawRect(drawPos.x - 0x180000, drawPos.y - 0x140000, 0x300000, 0x280000, 0x000000, 0xFF, INK_BLEND, false);

    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    RSDK.DrawRect(drawPos.x - 0x180000, drawPos.y - 0x140000, 0x300000, 0x280000, 0xFFFFFF, 0xFF, INK_NONE, false);

    if (self->xOut) {
        int32 dir       = self->direction;
        self->direction = self->zoneDirection;
        RSDK.DrawSprite(&self->zoneAnimator, &drawPos, false);

        self->direction = dir;
        drawPos.x += 0x10000;
        drawPos.y += 0x10000;
        self->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&self->blackCrossAnimator, &drawPos, false);

        self->inkEffect = INK_NONE;
        drawPos.x -= 0x10000;
        drawPos.y -= 0x10000;
        RSDK.DrawSprite(&self->redCrossAnimator, &drawPos, false);
    }
    else if (self->zoneID == UIVSZONEBUTTON_FUZZ) {
        int32 dir       = self->direction;
        self->direction = self->zoneDirection;
        RSDK.DrawSprite(&self->zoneAnimator, &drawPos, false);

        self->direction = dir;
    }
    else {
        RSDK.DrawSprite(&self->zoneAnimator, &drawPos, false);
    }
}

void UIVsZoneButton_DrawButton(void)
{
    RSDK_THIS(UIVsZoneButton);

    Vector2 drawPos;
    int32 width = (self->size.y + self->size.x) >> 16;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x280000;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, 0xF0, 0xF0, 0xF0);

    drawPos.x += self->buttonBounceOffset;
    drawPos.y += self->buttonBounceOffset;
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, width, self->size.y >> 16, self->bgEdgeSize, 0x00, 0x00, 0x00);

    if (!SceneInfo->inEditor) {
        drawPos.x -= RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->nameText, 0, self->nameText.length, 0) << 15;
        drawPos.y -= 0x10000;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->nameText, 0, self->nameText.length, ALIGN_LEFT, 0, NULL, NULL, false);
    }
}

bool32 UIVsZoneButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UIVsZoneButton);

    return self->state == UIVsZoneButton_State_HandleButtonEnter;
}

bool32 UIVsZoneButton_CheckSelectedCB(void)
{
    RSDK_THIS(UIVsZoneButton);

    return self->state == UIVsZoneButton_State_Selected;
}

void UIVsZoneButton_SelectedCB(void)
{
    RSDK_THIS(UIVsZoneButton);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    EntityMenuParam *param  = MenuParam_GetParam();

    parent->state = StateMachine_None;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
    Music_Stop();

    TimeAttackData_Clear();
    param->vsZoneID = self->zoneID;
    param->vsActID  = self->act;

    self->state = UIVsZoneButton_State_Selected;
}

void UIVsZoneButton_FailCB(void) { RSDK.PlaySfx(UIVsZoneButton->sfxFail, false, 255); }

void UIVsZoneButton_ButtonEnterCB(void)
{
    RSDK_THIS(UIVsZoneButton);

    self->state = UIVsZoneButton_State_HandleButtonEnter;

    if (!self->isSelected) {
        self->buttonBounceOffset   = 0;
        self->buttonBounceVelocity = -0x20000;
        self->triBounceVelocity    = 0x4000;
        self->unusedBounceVelocity = 0x8000;
        self->isSelected           = true;
    }
}

void UIVsZoneButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UIVsZoneButton);

    self->isSelected = false;
    self->state      = UIVsZoneButton_State_HandleButtonLeave;
}

void UIVsZoneButton_State_HandleButtonLeave(void)
{
    RSDK_THIS(UIVsZoneButton);

    if (self->zoneID == UIVSZONEBUTTON_FUZZ || self->xOut)
        self->zoneDirection = self->zoneAnimator.frameID & 3;

    if (self->buttonBounceOffset) {
        int32 dist = -(self->buttonBounceOffset / abs(self->buttonBounceOffset));
        self->buttonBounceOffset += dist << 16;

        if (dist < 0) {
            if (self->buttonBounceOffset < 0)
                self->buttonBounceOffset = 0;
            else if (dist > 0 && self->buttonBounceOffset > 0)
                self->buttonBounceOffset = 0;
        }
        else if (dist > 0 && self->buttonBounceOffset > 0)
            self->buttonBounceOffset = 0;
    }
}

void UIVsZoneButton_State_HandleButtonEnter(void)
{
    RSDK_THIS(UIVsZoneButton);

    if (self->zoneID == UIVSZONEBUTTON_FUZZ || self->xOut)
        self->zoneDirection = self->zoneAnimator.frameID & 3;

    self->buttonBounceVelocity += 0x4800;
    self->buttonBounceOffset += self->buttonBounceVelocity;

    if (self->buttonBounceOffset >= -0x20000 && self->buttonBounceVelocity > 0) {
        self->buttonBounceOffset   = -0x20000;
        self->buttonBounceVelocity = 0;
    }
}

void UIVsZoneButton_State_Selected(void)
{
    RSDK_THIS(UIVsZoneButton);

    if (self->zoneID == UIVSZONEBUTTON_FUZZ || self->xOut)
        self->zoneDirection = self->zoneAnimator.frameID & 3;

    if (self->timer >= 30) {
        self->isSelected      = false;
        self->timer           = 0;
        self->state           = UIVsZoneButton_State_HandleButtonLeave;
        self->processButtonCB = UIButton_ProcessButtonCB_Scroll;
        StateMachine_Run(self->actionCB);
    }
    else {
        self->timer++;
    }
}

#if GAME_INCLUDE_EDITOR
void UIVsZoneButton_EditorDraw(void)
{
    RSDK_THIS(UIVsZoneButton);

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;

    UIVsZoneButton_Draw();
}

void UIVsZoneButton_EditorLoad(void)
{
    UIVsZoneButton->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsZoneButton->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIVsZoneButton, zoneID);
    RSDK_ENUM_VAR("GHZ", UIVSZONEBUTTON_GHZ);
    RSDK_ENUM_VAR("CPZ", UIVSZONEBUTTON_CPZ);
    RSDK_ENUM_VAR("SPZ", UIVSZONEBUTTON_SPZ);
    RSDK_ENUM_VAR("FBZ", UIVSZONEBUTTON_FBZ);
    RSDK_ENUM_VAR("PGZ", UIVSZONEBUTTON_PGZ);
    RSDK_ENUM_VAR("SSZ", UIVSZONEBUTTON_SSZ);
    RSDK_ENUM_VAR("HCZ", UIVSZONEBUTTON_HCZ);
    RSDK_ENUM_VAR("MSZ", UIVSZONEBUTTON_MSZ);
    RSDK_ENUM_VAR("OOZ", UIVSZONEBUTTON_OOZ);
    RSDK_ENUM_VAR("LRZ", UIVSZONEBUTTON_LRZ);
    RSDK_ENUM_VAR("MMZ", UIVSZONEBUTTON_MMZ);
    RSDK_ENUM_VAR("TMZ", UIVSZONEBUTTON_TMZ);
    RSDK_ENUM_VAR("Fuzz", UIVSZONEBUTTON_FUZZ);
}
#endif

void UIVsZoneButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_ENUM, zoneID);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_ENUM, act);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_BOOL, xOut);
    RSDK_EDITABLE_VAR(UIVsZoneButton, VAR_BOOL, obfuscate);
}
