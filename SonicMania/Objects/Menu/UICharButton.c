// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UICharButton Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUICharButton *UICharButton;

void UICharButton_Update(void)
{
    RSDK_THIS(UICharButton);

    self->touchPosSizeS.x   = 0x600000;
    self->touchPosSizeS.y   = 0x600000;
    self->touchPosOffsetS.x = 0;
    self->touchPosOffsetS.y = 0;

    StateMachine_Run(self->state);

    if (self->isSelected) {
        self->triBounceVelocity -= 0x600;
        self->triBounceOffset += self->triBounceVelocity;

        if (self->triBounceOffset <= 0x10000 && self->triBounceVelocity < 0) {
            self->triBounceOffset   = 0x10000;
            self->triBounceVelocity = 0;
        }

        self->playerBounceVelocity -= 0x1800;
        self->playerBounceOffset += self->playerBounceVelocity;

        if (self->playerBounceOffset <= 0x8000 && self->playerBounceVelocity < 0) {
            self->playerBounceOffset   = 0x8000;
            self->playerBounceVelocity = 0;
        }
    }
    else if (self->state != UICharButton_State_Selected) {
        if (self->triBounceOffset > 0) {
            self->triBounceOffset -= 0x2000;

            if (self->triBounceOffset < 0)
                self->triBounceOffset = 0;
        }

        if (self->playerBounceOffset > 0) {
            self->playerBounceOffset -= 0x2000;

            if (self->playerBounceOffset < 0)
                self->playerBounceOffset = 0;
        }
    }

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    int32 id                = -1;
    for (int32 i = 0; i < parent->buttonCount; ++i) {
        if (self == (EntityUICharButton *)parent->buttons[i]) {
            id = i;
            break;
        }
    }

    if (self->isSelected && (parent->state != UIControl_ProcessInputs || parent->buttonID != id)) {
        self->isSelected = false;
        self->state      = UICharButton_State_HandleButtonLeave;
    }
}

void UICharButton_LateUpdate(void) {}

void UICharButton_StaticUpdate(void) {}

void UICharButton_Draw(void)
{
    RSDK_THIS(UICharButton);

    RSDK.DrawRect(self->position.x - 0x2D0000, self->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 0x7F, INK_BLEND, false);

    UICharButton_DrawBG();
    UICharButton_DrawOutlines();
    UICharButton_DrawPlayers();
}

void UICharButton_Create(void *data)
{
    RSDK_THIS(UICharButton);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = 2;
    self->visible       = true;
    self->drawFX        = FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x300000;

    self->processButtonCB    = UIButton_ProcessButtonCB_Scroll;
    self->touchCB            = UIButton_ProcessTouchCB_Single;
    self->selectedCB         = UICharButton_SelectedCB;
    self->failCB             = StateMachine_None;
    self->buttonEnterCB      = UICharButton_ButtonEnterCB;
    self->buttonLeaveCB      = UICharButton_ButtonLeaveCB;
    self->checkButtonEnterCB = UICharButton_CheckButtonEnterCB;
    self->checkSelectedCB    = UICharButton_CheckSelectedCB;
    self->state              = UICharButton_State_HandleButtonLeave;
}

void UICharButton_StageLoad(void) { UICharButton->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UICharButton_DrawOutlines(void)
{
    RSDK_THIS(UICharButton);

    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(self->position.x + 0x30000, self->position.y + 0x30000, 96, 96);

    if (self->isSelected)
        UIWidgets_DrawRectOutline_Flash(self->position.x, self->position.y, 96, 96);
    else
        UIWidgets_DrawRectOutline_Black(self->position.x, self->position.y, 96, 96);
}

void UICharButton_DrawBG(void)
{
    RSDK_THIS(UICharButton);

    UIWidgets_DrawRightTriangle(self->position.x - 0x2D0000, self->position.y - 0x2D0000, (self->triBounceOffset >> 11), 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-64 * self->triBounceOffset) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-44 * self->triBounceOffset) >> 16, 88, 112, 224);
}

void UICharButton_DrawPlayers(void)
{
    RSDK_THIS(UICharButton);

    Vector2 drawPos;
    drawPos = self->position;
    drawPos.x -= 0x2D0000;
    drawPos.y += 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);

    if (self->state != UICharButton_State_Selected || !(self->timer & 2)) {
        int32 frame = self->characterID;
#if MANIA_USE_PLUS
        if (self->characterID >= UICHARBUTTON_MIGHTY)
            frame = self->characterID + 1;
#endif
        RSDK.SetSpriteAnimation(UICharButton->aniFrames, 1, &self->playerAnimator, true, frame);
        RSDK.SetSpriteAnimation(UICharButton->aniFrames, 2, &self->shadowAnimator, true, frame);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        drawPos.x += 4 * self->playerBounceOffset;
        drawPos.y += 4 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);

        drawPos.x -= 8 * self->playerBounceOffset;
        drawPos.y -= 8 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);

        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->nameAnimator, true, self->characterID);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;
        RSDK.DrawSprite(&self->nameAnimator, &drawPos, false);
    }
}

void UICharButton_SelectedCB(void)
{
    RSDK_THIS(UICharButton);

    self->timer           = 0;
    self->state           = UICharButton_State_Selected;
    self->processButtonCB = StateMachine_None;

    UITransition_StartTransition(self->actionCB, 30);

    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->selectionDisabled = true;

    ((EntityUIControl *)self->parent)->backoutTimer = 30;

    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

bool32 UICharButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UICharButton);

    return self->state == UICharButton_State_HandleButtonEnter;
}

bool32 UICharButton_CheckSelectedCB(void)
{
    RSDK_THIS(UICharButton);

    return self->state == UICharButton_State_Selected;
}

void UICharButton_ButtonEnterCB(void)
{
    RSDK_THIS(UICharButton);

    if (!self->isSelected) {
        self->triBounceOffset      = 0;
        self->triBounceVelocity    = 0x4000;
        self->playerBounceOffset   = 0;
        self->playerBounceVelocity = 0x8000;

        self->isSelected = true;
        self->state      = UICharButton_State_HandleButtonEnter;
    }
}

void UICharButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UICharButton);

    self->isSelected = false;
    self->state      = UICharButton_State_HandleButtonLeave;
}

void UICharButton_State_HandleButtonLeave(void) {}

void UICharButton_State_HandleButtonEnter(void) {}

void UICharButton_State_Selected(void)
{
    RSDK_THIS(UICharButton);

    if (self->timer >= 30) {
        self->isSelected      = false;
        self->timer           = 0;
        self->state           = UICharButton_State_HandleButtonLeave;
        self->processButtonCB = UIButton_ProcessButtonCB_Scroll;
    }
    else {
        if (self->timer == 2) {
            switch (self->characterID) {
                case UICHARBUTTON_SONIC: RSDK.PlaySfx(Announcer->sfxSonic, false, 255); break;
                case UICHARBUTTON_TAILS: RSDK.PlaySfx(Announcer->sfxTails, false, 255); break;
                case UICHARBUTTON_KNUX: RSDK.PlaySfx(Announcer->sfxKnuckles, false, 255); break;
#if MANIA_USE_PLUS
                case UICHARBUTTON_MIGHTY: RSDK.PlaySfx(Announcer->sfxMighty, false, 255); break;
                case UICHARBUTTON_RAY: RSDK.PlaySfx(Announcer->sfxRay, false, 255); break;
#endif
                default: break;
            }
        }

        ++self->timer;
    }
}

#if GAME_INCLUDE_EDITOR
void UICharButton_EditorDraw(void)
{
    RSDK_THIS(UICharButton);

    self->inkEffect = self->disabled ? INK_BLEND : INK_NONE;

    UICharButton_Draw();
}

void UICharButton_EditorLoad(void)
{
    UICharButton->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UICharButton, characterID);
    RSDK_ENUM_VAR("Sonic", UICHARBUTTON_SONIC);
    RSDK_ENUM_VAR("Tails", UICHARBUTTON_TAILS);
    RSDK_ENUM_VAR("Knuckles", UICHARBUTTON_KNUX);
#if MANIA_USE_PLUS
    RSDK_ENUM_VAR("Mighty", UICHARBUTTON_MIGHTY);
    RSDK_ENUM_VAR("Ray", UICHARBUTTON_RAY);
#endif
}
#endif

void UICharButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICharButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UICharButton, VAR_UINT8, characterID);
}
