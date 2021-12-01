#include "SonicMania.h"

ObjectUIVsCharSelector *UIVsCharSelector;

void UIVsCharSelector_Update(void)
{
    RSDK_THIS(UIVsCharSelector);

    self->prevSelected = self->isSelected;
    if (self->textFrames != UIWidgets->textFrames || self->prevFrameID != self->frameID) {
        UIVsCharSelector_SetupText();
        self->textFrames  = UIWidgets->textFrames;
        self->prevFrameID = self->frameID;
    }

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
    else if (self->state != UIVsCharSelector_State_Selected) {
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

    self->triBounceOffset    = minVal(self->triBounceOffset, 0x13600);
    self->playerBounceOffset = minVal(self->playerBounceOffset, 0x11800);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if ((
#if RETRO_USE_PLUS
            self->state == UIVsCharSelector_State_HandlePlayerJoin ||
#endif
            self->state == UIVsCharSelector_State_Selected)
        && (parent->state != UIControl_ProcessInputs || parent->active != ACTIVE_ALWAYS || parent->selectionDisabled)) {
        self->isSelected = false;
        self->ready      = false;
        self->state      = UIVsCharSelector_State_CharSelect;
    }
}

void UIVsCharSelector_LateUpdate(void) {}

void UIVsCharSelector_StaticUpdate(void) {}

void UIVsCharSelector_Draw(void)
{
    RSDK_THIS(UIVsCharSelector);
    RSDK.DrawRect(self->position.x - 0x2D0000, self->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 127, INK_BLEND, false);
    UIVsCharSelector_DrawBG();
    UIVsCharSelector_DrawOutline();
    UIVsCharSelector_DrawPlayer();
}

void UIVsCharSelector_Create(void *data)
{
    RSDK_THIS(UIVsCharSelector);
    self->frameID         = self->playerID;
    self->active          = ACTIVE_BOUNDS;
    self->drawOrder       = 2;
    self->visible         = true;
    self->drawFX          = FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x300000;
    self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    self->state           = UIVsCharSelector_State_CharSelect;
    UIVsCharSelector_SetupText();
    self->textFrames = UIWidgets->textFrames;
}

void UIVsCharSelector_StageLoad(void) { UIVsCharSelector->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsCharSelector_SetupText(void)
{
    RSDK_THIS(UIVsCharSelector);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->playerIDAnimator, true, self->playerID + 8);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->waitingAnimator, true, 7);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->playerNameAnimator, true, self->frameID);
}

void UIVsCharSelector_DrawOutline(void)
{
    RSDK_THIS(UIVsCharSelector);
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(96, 96, self->position.x + 0x30000, self->position.y + 0x30000);
    if (self->isSelected)
        UIWidgets_DrawRectOutline_Flash(96, 96, self->position.x, self->position.y);
    else
        UIWidgets_DrawRectOutline_Black(96, 96, self->position.x, self->position.y);
}

void UIVsCharSelector_DrawBG(void)
{
    RSDK_THIS(UIVsCharSelector);
    UIWidgets_DrawRightTriangle(self->position.x - 0x2D0000, self->position.y - 0x2D0000, (self->triBounceOffset >> 11), 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-64 * self->triBounceOffset) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-44 * self->triBounceOffset) >> 16, 88, 112, 224);
}

void UIVsCharSelector_DrawPlayer(void)
{
    RSDK_THIS(UIVsCharSelector);
    Vector2 drawPos;

    drawPos.x = self->position.x - 0x2D0000;
    drawPos.y = self->position.y + 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);
    RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->edgeAnimator, true, 1);

    drawPos.x = self->position.x + 0x2D0000;
    drawPos.y = self->position.y - 0x2D0000;
    RSDK.DrawSprite(&self->edgeAnimator, &drawPos, false);

    drawPos.y += 0x80000;
    drawPos.x -= 0xA0000;
    RSDK.DrawSprite(&self->playerIDAnimator, &drawPos, false);

    if (self->state == UIVsCharSelector_State_WaitingForPlayer) {
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->playerAnimator, true, 7);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);
        RSDK.DrawSprite(&self->waitingAnimator, &drawPos, false);
    }
    else if (self->state != UIVsCharSelector_State_Selected || !(self->timer & 2)) {
        int32 frameID = self->frameID;
#if RETRO_USE_PLUS
        if (self->frameID > UICHARBUTTON_KNUX)
            frameID++;
#endif
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 1, &self->playerAnimator, true, frameID);
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 2, &self->shadowAnimator, true, frameID);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        drawPos.x += 4 * self->playerBounceOffset;
        drawPos.y += 4 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->shadowAnimator, &drawPos, false);

        drawPos.x -= 8 * self->playerBounceOffset;
        drawPos.y -= 8 * self->playerBounceOffset;
        RSDK.DrawSprite(&self->playerAnimator, &drawPos, false);

        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;
        RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, false);
    }

    if (!self->ready && self->state != UIVsCharSelector_State_WaitingForPlayer) {
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;
        if (RSDK.Sin256(2 * UIControl->timer) < 0)
            UIWidgets_DrawLeftRightArrows(drawPos.x, drawPos.y, (-RSDK.Sin256(2 * UIControl->timer) + 0x880) << 11);
        else
            UIWidgets_DrawLeftRightArrows(drawPos.x, drawPos.y, (RSDK.Sin256(2 * UIControl->timer) + 0x880) << 11);
    }
}

void UIVsCharSelector_ProcessButtonCB(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (parent->active == ACTIVE_ALWAYS) {
        int32 storedFrame = self->frameID;
        int32 inc         = 1;
        if (UIControl->leftPress[self->playerID]) {
            --self->frameID;
            inc                        = -1;
            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
        else if (UIControl->rightPress[self->playerID]) {
            ++self->frameID;
            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }

#if RETRO_USE_PLUS
        int32 max = UICHARBUTTON_KNUX + (API.CheckDLC(DLC_PLUS) ? 2 : 0);
#else
        int32 max = UICHARBUTTON_KNUX;
#endif
        max++;

        while (self->frameID < 0) {
            self->frameID += max;
        }

        while (self->frameID >= max) {
            self->frameID -= max;
        }

#if RETRO_GAMEVER != VER_100
        int32 activePlayers = 0;
        for (int32 i = 0; i < parent->buttonCount; ++i) {
            EntityUIVsCharSelector *button = (EntityUIVsCharSelector *)parent->buttons[i];
            if (button->state == UIVsCharSelector_State_Selected || button->processButtonCB == UIVsCharSelector_ProcessButtonCB_CharSelected)
                activePlayers |= (1 << button->frameID);
        }

        int32 frame = self->frameID;
        while ((1 << frame) & activePlayers) {
            frame += inc;
            if (frame < 0)
                frame += max;
            if (frame >= max)
                frame -= max;
            self->frameID = frame;
        }

        if (self->frameID != frame) {
            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;
        }
#endif

        if (storedFrame != self->frameID)
            UIVsCharSelector_SetupText();

        bool32 pressed = false;
        if (API_GetConfirmButtonFlip())
            pressed = ControllerInfo[self->playerID + 1].keyB.press;
        else
            pressed = ControllerInfo[self->playerID + 1].keyA.press;
        if (pressed) {
            self->timer           = 0;
            self->state           = UIVsCharSelector_State_Selected;
            self->processButtonCB = StateMachine_None;
            self->ready           = true;
            RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        }
        else if (!self->isSelected && !self->ready) {
            if (parent->active == ACTIVE_ALWAYS)
                UIVsCharSelector_State_ResetState();
        }
    }
}

void UIVsCharSelector_ProcessButtonCB_CharSelected(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    if (parent->active == ACTIVE_ALWAYS) {
        parent->targetPos.x = self->position.x;

        bool32 pressed = false;
        if (API_GetConfirmButtonFlip())
            pressed = ControllerInfo[self->playerID + 1].keyB.press;
        else
            pressed = ControllerInfo[self->playerID + 1].keyA.press;
        if (pressed) {
            self->timer = 0;
            UIVsCharSelector_State_ResetState();
            self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
            self->ready           = false;
        }
    }
}

void UIVsCharSelector_State_ResetState(void)
{
    RSDK_THIS(UIVsCharSelector);

#if RETRO_USE_PLUS
    self->state = UIVsCharSelector_State_HandlePlayerJoin;
#else
    self->state = UIVsCharSelector_State_CharSelect;
#endif
    if (!self->isSelected) {
        self->triBounceVelocity    = 0x4000;
        self->playerBounceOffset   = 0;
        self->playerBounceVelocity = 0x8000;
        self->isSelected           = true;
    }
}

void UIVsCharSelector_State_CharSelect(void)
{
    RSDK_THIS(UIVsCharSelector);

    int32 id = API_ControllerIDForInputID(self->playerID + 1);
#if RETRO_USE_PLUS
    if (!id || id == CONT_AUTOASSIGN) {
#else
    if (id + 1 <= 1) {
#endif
        self->state = UIVsCharSelector_State_WaitingForPlayer;
        UIVsCharSelector_State_WaitingForPlayer();
    }
}

void UIVsCharSelector_State_WaitingForPlayer(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->processButtonCB = StateMachine_None;
    self->isSelected      = false;

    int32 id = API_ControllerIDForInputID(self->playerID + 1);
#if RETRO_USE_PLUS
    self->ready    = false;
    int32 assigned = RSDK.GetAssignedControllerID(id);
    if (parent->active == ACTIVE_ALWAYS) {
        if (!id || (!assigned && id != CONT_AUTOASSIGN)) {
            API_AssignControllerID(self->playerID + 1, CONT_AUTOASSIGN);
        }
        else if (id != CONT_AUTOASSIGN) {
            self->playerBounceOffset   = 0;
            self->playerBounceVelocity = 0x8000;
            self->timer                = 4;
            self->state                = UIVsCharSelector_State_HandlePlayerJoin;
        }
    }
#else
    if (id >= 2) {
        self->state = UIVsCharSelector_State_CharSelect;
        self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    }
#endif
}

#if RETRO_USE_PLUS
void UIVsCharSelector_State_HandlePlayerJoin(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->isSelected = true;
    int32 id         = API_ControllerIDForInputID(self->playerID + 1);
    int32 assigned   = RSDK.GetAssignedControllerID(id);

    if (parent->active == ACTIVE_ALWAYS) {
        if (id && id != CONT_AUTOASSIGN && assigned) {
            if (self->timer > 0) {
                if (self->timer == 1)
                    self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
                self->timer--;
            }
        }
        else {
            self->state = UIVsCharSelector_State_WaitingForPlayer;
        }
    }
}
#endif

void UIVsCharSelector_State_Selected(void)
{
    RSDK_THIS(UIVsCharSelector);

    if (self->timer >= 30) {
        if (self->parent->active == ACTIVE_ALWAYS) {
            self->isSelected      = false;
            self->timer           = 0;
            self->state           = UIVsCharSelector_State_CharSelect;
            self->processButtonCB = UIVsCharSelector_ProcessButtonCB_CharSelected;
            StateMachine_Run(self->actionCB);
        }
    }
    else {
        if (self->timer == 2) {
            switch (self->frameID) {
                case UICHARBUTTON_SONIC: RSDK.PlaySfx(Announcer->sfxSonic, false, 255); break;
                case UICHARBUTTON_TAILS: RSDK.PlaySfx(Announcer->sfxTails, false, 255); break;
                case UICHARBUTTON_KNUX: RSDK.PlaySfx(Announcer->sfxKnuckles, false, 255); break;
#if RETRO_USE_PLUS
                case UICHARBUTTON_MIGHTY: RSDK.PlaySfx(Announcer->sfxMighty, false, 255); break;
                case UICHARBUTTON_RAY: RSDK.PlaySfx(Announcer->sfxRay, false, 255); break;
#endif
                default: break;
            }
        }
        ++self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIVsCharSelector_EditorDraw(void) { UIVsCharSelector_Draw(); }

void UIVsCharSelector_EditorLoad(void)
{
    UIVsCharSelector->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(UIVsCharSelector, playerID);
    RSDK_ENUM_VAR("Player 1", UIVSCHARSELECTOR_1P);
    RSDK_ENUM_VAR("Player 2", UIVSCHARSELECTOR_2P);
#if RETRO_USE_PLUS
    RSDK_ENUM_VAR("Player 3", UIVSCHARSELECTOR_3P);
    RSDK_ENUM_VAR("Player 4", UIVSCHARSELECTOR_4P);
#endif
}
#endif

void UIVsCharSelector_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_UINT8, playerID);
}
