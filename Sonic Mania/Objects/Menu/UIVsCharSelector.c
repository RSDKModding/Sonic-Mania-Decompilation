#include "SonicMania.h"

ObjectUIVsCharSelector *UIVsCharSelector;

void UIVsCharSelector_Update(void)
{
    RSDK_THIS(UIVsCharSelector);

    self->prevFlag = self->flag;
    if (self->textFrames != UIWidgets->textFrames || self->prevFrameID != self->frameID) {
        UIVsCharSelector_SetupText();
        self->textFrames = UIWidgets->textFrames;
        self->prevFrameID     = self->frameID;
    }

    StateMachine_Run(self->state);

    if (self->flag) {
        self->field_110 -= 0x600;
        self->field_114 += self->field_110;
        if (self->field_114 <= 0x10000 && self->field_110 < 0) {
            self->field_114 = 0x10000;
            self->field_110 = 0;
        }

        self->field_118 -= 0x1800;
        self->field_11C += self->field_118;
        if (self->field_11C <= 0x8000 && self->field_118 < 0) {
            self->field_11C = 0x8000;
            self->field_118 = 0;
        }
    }
    else if (self->state != UIVsCharSelector_Unknown11) {
        if (self->field_114 > 0) {
            self->field_114 -= 0x2000;
            if (self->field_114 < 0)
                self->field_114 = 0;
        }

        if (self->field_11C > 0) {
            self->field_11C -= 0x2000;
            if (self->field_11C < 0)
                self->field_11C = 0;
        }
    }

    self->field_114 = minVal(self->field_114, 0x13600);
    self->field_11C = minVal(self->field_11C, 0x11800);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if ((
#if RETRO_USE_PLUS
            self->state == UIVsCharSelector_Unknown10 ||
#endif
            self->state == UIVsCharSelector_Unknown11)
        && (parent->state != UIControl_ProcessInputs || parent->active != ACTIVE_ALWAYS || parent->selectionDisabled)) {
        self->flag      = false;
        self->ready = false;
        self->state     = UIVsCharSelector_Unknown8;
    }
}

void UIVsCharSelector_LateUpdate(void) {}

void UIVsCharSelector_StaticUpdate(void) {}

void UIVsCharSelector_Draw(void)
{
    RSDK_THIS(UIVsCharSelector);
    RSDK.DrawRect(self->position.x - 0x2D0000, self->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 127, INK_BLEND, false);
    UIVsCharSelector_Unknown3();
    UIVsCharSelector_Unknown2();
    UIVsCharSelector_Unknown4();
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
    self->state           = UIVsCharSelector_Unknown8;
    UIVsCharSelector_SetupText();
    self->textFrames = UIWidgets->textFrames;
}

void UIVsCharSelector_StageLoad(void) { UIVsCharSelector->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UIVsCharSelector_SetupText(void)
{
    RSDK_THIS(UIVsCharSelector);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->animator5, true, self->playerID + 8);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 12, &self->animator6, true, 7);
    RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->animator3, true, self->frameID);
}

void UIVsCharSelector_Unknown2(void)
{
    RSDK_THIS(UIVsCharSelector);
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(96, 96, self->position.x + 0x30000, self->position.y + 0x30000);
    if (self->flag)
        UIWidgets_DrawRectOutline_Flash(96, 96, self->position.x, self->position.y);
    else
        UIWidgets_DrawRectOutline_Black(96, 96, self->position.x, self->position.y);
}

void UIVsCharSelector_Unknown3(void)
{
    RSDK_THIS(UIVsCharSelector);
    UIWidgets_DrawRightTriangle(self->position.x - 0x2D0000, self->position.y - 0x2D0000, (self->field_114 >> 11), 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-64 * self->field_114) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-44 * self->field_114) >> 16, 88, 112, 224);
}

void UIVsCharSelector_Unknown4(void)
{
    RSDK_THIS(UIVsCharSelector);
    Vector2 drawPos;

    drawPos.x = self->position.x - 0x2D0000;
    drawPos.y = self->position.y + 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);
    RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->animator4, true, 1);

    drawPos.x = self->position.x + 0x2D0000;
    drawPos.y = self->position.y - 0x2D0000;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.y += 0x80000;
    drawPos.x -= 0xA0000;
    RSDK.DrawSprite(&self->animator5, &drawPos, false);

    if (self->state == UIVsCharSelector_Unknown9) {
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 14, &self->animator1, true, 7);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
        RSDK.DrawSprite(&self->animator6, &drawPos, false);
    }
    else if (self->state != UIVsCharSelector_Unknown11 || !(self->timer & 2)) {
        int32 frameID = self->frameID;
        if (self->frameID > 2)
            frameID++;
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 1, &self->animator1, true, frameID);
        RSDK.SetSpriteAnimation(UIVsCharSelector->aniFrames, 2, &self->animator2, true, frameID);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        drawPos.x += 4 * self->field_11C;
        drawPos.y += 4 * self->field_11C;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        drawPos.x -= 8 * self->field_11C;
        drawPos.y -= 8 * self->field_11C;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
    }

    if (!self->ready && self->state != UIVsCharSelector_Unknown9) {
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
            inc               = -1;
            self->field_11C = 0;
            self->field_118 = 0x8000;
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }
        else if (UIControl->rightPress[self->playerID]) {
            ++self->frameID;
            self->field_11C = 0;
            self->field_118 = 0x8000;
            RSDK.PlaySfx(UIWidgets->sfxBleep, false, 255);
        }

#if RETRO_USE_PLUS
        int32 max = 3 + (API.CheckDLC(DLC_PLUS) ? 2 : 0);
#else
        int32 max = 3;
#endif
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
            if (button->state == UIVsCharSelector_Unknown11 || button->processButtonCB == UIVsCharSelector_ProcessButtonCB_Alt)
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
            self->field_11C = 0;
            self->field_118 = 0x8000;
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
            self->state           = UIVsCharSelector_Unknown11;
            self->processButtonCB = StateMachine_None;
            self->ready           = true;
            RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
        }
        else if (!self->flag && !self->ready) {
            if (parent->active == ACTIVE_ALWAYS)
                UIVsCharSelector_Unknown7();
        }
    }
}

void UIVsCharSelector_ProcessButtonCB_Alt(void)
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
            UIVsCharSelector_Unknown7();
            self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
            self->ready           = false;
        }
    }
}

void UIVsCharSelector_Unknown7(void)
{
    RSDK_THIS(UIVsCharSelector);

#if RETRO_USE_PLUS
    self->state = UIVsCharSelector_Unknown10;
#else
    self->state = UIVsCharSelector_Unknown8;
#endif
    if (!self->flag) {
        self->field_110 = 0x4000;
        self->field_11C = 0;
        self->field_118 = 0x8000;
        self->flag      = true;
    }
}

void UIVsCharSelector_Unknown8(void)
{
    RSDK_THIS(UIVsCharSelector);

    int32 id = API_ControllerIDForInputID(self->playerID + 1);
#if RETRO_USE_PLUS
    if (!id || id == CONT_AUTOASSIGN) {
#else
    if (id + 1 <= 1) {
#endif
        self->state = UIVsCharSelector_Unknown9;
        UIVsCharSelector_Unknown9();
    }
}

void UIVsCharSelector_Unknown9(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->processButtonCB = StateMachine_None;
    self->flag            = false;

    int32 id = API_ControllerIDForInputID(self->playerID + 1);
#if RETRO_USE_PLUS
    self->ready = false;
    int32 assigned = RSDK.GetAssignedControllerID(id);
    if (parent->active == ACTIVE_ALWAYS) {
        if (!id) {
            API_AssignControllerID(self->playerID + 1, CONT_AUTOASSIGN);
        }
        else if (!assigned && id != CONT_AUTOASSIGN) {
            API_AssignControllerID(self->playerID + 1, CONT_AUTOASSIGN);
        }
        else if (id != CONT_AUTOASSIGN) {
            self->field_11C = 0;
            self->field_118 = 0x8000;
            self->timer     = 4;
            self->state     = UIVsCharSelector_Unknown10;
        }
    }
#else
    if (id >= 2) {
        self->state = UIVsCharSelector_Unknown8;
        self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
    }
#endif
}

#if RETRO_USE_PLUS
void UIVsCharSelector_Unknown10(void)
{
    RSDK_THIS(UIVsCharSelector);
    EntityUIControl *parent = (EntityUIControl *)self->parent;

    self->flag = true;
    int32 id       = API_ControllerIDForInputID(self->playerID + 1);
    int32 assigned = RSDK.GetAssignedControllerID(id);

    if (parent->active == ACTIVE_ALWAYS) {
        if (id && id != CONT_AUTOASSIGN && assigned) {
            if (self->timer > 0) {
                if (self->timer == 1)
                    self->processButtonCB = UIVsCharSelector_ProcessButtonCB;
                self->timer--;
            }
        }
        else {
            self->state = UIVsCharSelector_Unknown9;
        }
    }
}
#endif

void UIVsCharSelector_Unknown11(void)
{
    RSDK_THIS(UIVsCharSelector);

    if (self->timer >= 30) {
        if (self->parent->active == ACTIVE_ALWAYS) {
            self->flag            = false;
            self->timer           = 0;
            self->state           = UIVsCharSelector_Unknown8;
            self->processButtonCB = UIVsCharSelector_ProcessButtonCB_Alt;
            StateMachine_Run(self->actionCB);
        }
    }
    else {
        if (self->timer == 2) {
            switch (self->frameID) {
                case 0: RSDK.PlaySfx(Announcer->sfxSonic, false, 255); break;
                case 1: RSDK.PlaySfx(Announcer->sfxTails, false, 255); break;
                case 2: RSDK.PlaySfx(Announcer->sfxKnuckles, false, 255); break;
#if RETRO_USE_PLUS
                case 3: RSDK.PlaySfx(Announcer->sfxMighty, false, 255); break;
                case 4: RSDK.PlaySfx(Announcer->sfxRay, false, 255); break;
#endif
                default: break;
            }
        }
        ++self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIVsCharSelector_EditorDraw(void) { UIVsCharSelector_Draw(); }

void UIVsCharSelector_EditorLoad(void) {}
#endif

void UIVsCharSelector_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIVsCharSelector, VAR_UINT8, playerID);
}
