#include "SonicMania.h"

ObjectUICharButton *UICharButton;

void UICharButton_Update(void)
{
    RSDK_THIS(UICharButton);
    self->touchPosStart.x = 0x600000;
    self->touchPosStart.y = 0x600000;
    self->touchPosEnd.x   = 0;
    self->touchPosEnd.y   = 0;
    StateMachine_Run(self->state);

    if (self->flag) {
        self->field_108 -= 0x600;
        self->field_10C += self->field_108;
        if (self->field_10C <= 0x10000 && self->field_108 < 0) {
            self->field_10C = 0x10000;
            self->field_108 = 0;
        }

        self->field_110 -= 0x1800;
        self->field_114 += self->field_110;
        if (self->field_114 <= 0x8000 && self->field_110 < 0) {
            self->field_114 = 0x8000;
            self->field_110 = 0;
        }
    }
    else if (self->state != UICharButton_Unknown11) {
        if (self->field_10C > 0) {
            self->field_10C -= 0x2000;
            if (self->field_10C < 0)
                self->field_10C = 0;
        }

        if (self->field_114 > 0) {
            self->field_114 -= 0x2000;
            if (self->field_114 < 0)
                self->field_114 = 0;
        }
    }

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    int32 id                  = -1;
    for (int32 i = 0; i < parent->buttonCount; ++i) {
        if (self == (EntityUICharButton *)parent->buttons[i]) {
            id = i;
            break;
        }
    }

    if (self->flag && (parent->state != UIControl_ProcessInputs || parent->buttonID != id)) {
        self->flag  = false;
        self->state = UICharButton_Unknown9;
    }
}

void UICharButton_LateUpdate(void) {}

void UICharButton_StaticUpdate(void) {}

void UICharButton_Draw(void)
{
    RSDK_THIS(UICharButton);
    RSDK.DrawRect(self->position.x - 0x2D0000, self->position.y - 0x2D0000, 0x600000, 0x600000, 0xFFFFFF, 127, INK_BLEND, false);
    UICharButton_Unknown2();
    UICharButton_Unknown1();
    UICharButton_Unknown3();
}

void UICharButton_Create(void *data)
{
    RSDK_THIS(UICharButton);
    self->active          = ACTIVE_BOUNDS;
    self->drawOrder       = 2;
    self->visible         = true;
    self->drawFX          = FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x300000;
    self->processButtonCB = UIButton_ProcessButtonCB_Alt;
    self->touchCB            = UIButton_ProcessTouchCB;
    self->selectedCB           = UICharButton_SelectedCB;
    self->failCB             = StateMachine_None;
    self->buttonEnterCB      = UICharButton_ButtonEnterCB;
    self->buttonLeaveCB      = UICharButton_ButtonLeaveCB;
    self->checkButtonEnterCB = UICharButton_CheckButtonEnterCB;
    self->checkSelectedCB    = UICharButton_CheckSelectedCB;
    self->state              = UICharButton_Unknown9;
}

void UICharButton_StageLoad(void) { UICharButton->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }

void UICharButton_Unknown1(void)
{
    RSDK_THIS(UICharButton);
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(96, 96, self->position.x + 0x30000, self->position.y + 0x30000);

    if (self->flag)
        UIWidgets_DrawRectOutline_Flash(96, 96, self->position.x, self->position.y);
    else
        UIWidgets_DrawRectOutline_Black(96, 96, self->position.x, self->position.y);
}

void UICharButton_Unknown2(void)
{
    RSDK_THIS(UICharButton);
    UIWidgets_DrawRightTriangle(self->position.x - 0x2D0000, self->position.y - 0x2D0000, (self->field_10C >> 11), 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-64 * self->field_10C) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x2D0000, self->position.y + 0x2C0000, (-44 * self->field_10C) >> 16, 88, 112, 224);
}

void UICharButton_Unknown3(void)
{
    RSDK_THIS(UICharButton);
    Vector2 drawPos;

    drawPos = self->position;
    drawPos.x -= 0x2D0000;
    drawPos.y += 0x180000;
    RSDK.DrawRect(drawPos.x, drawPos.y, 0x5A0000, 0x100000, 0, 255, INK_NONE, false);

    if (self->state != UICharButton_Unknown11 || !(self->timer & 2)) {
        int32 frame = self->characterID;
        if (self->characterID > 2)
            frame = self->characterID + 1;
        RSDK.SetSpriteAnimation(UICharButton->aniFrames, 1, &self->animator1, true, frame);
        RSDK.SetSpriteAnimation(UICharButton->aniFrames, 2, &self->animator2, true, frame);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y - 0x80000;
        drawPos.x += 4 * self->field_114;
        drawPos.y += 4 * self->field_114;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        drawPos.x -= 8 * self->field_114;
        drawPos.y -= 8 * self->field_114;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);

        RSDK.SetSpriteAnimation(UIWidgets->textFrames, 8, &self->animator3, true, self->characterID);
        drawPos.x = self->position.x;
        drawPos.y = self->position.y + 0x200000;
        RSDK.DrawSprite(&self->animator3, &drawPos, false);
    }
}

void UICharButton_SelectedCB(void)
{
    RSDK_THIS(UICharButton);
    self->timer           = 0;
    self->state           = UICharButton_Unknown11;
    self->processButtonCB = 0;
    UITransition_StartTransition(self->actionCB, 30);
    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->selectionDisabled = true;
    ((EntityUIControl *)self->parent)->backoutTimer = 30;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

bool32 UICharButton_CheckButtonEnterCB(void)
{
    RSDK_THIS(UICharButton);
    return self->state == UICharButton_Unknown10;
}

bool32 UICharButton_CheckSelectedCB(void)
{
    RSDK_THIS(UICharButton);
    return self->state == UICharButton_Unknown11;
}

void UICharButton_ButtonEnterCB(void)
{
    RSDK_THIS(UICharButton);
    if (!self->flag) {
        self->field_10C = 0;
        self->field_108 = 0x4000;
        self->field_114 = 0;
        self->field_110 = 0x8000;
        self->flag      = true;
        self->state     = UICharButton_Unknown10;
    }
}

void UICharButton_ButtonLeaveCB(void)
{
    RSDK_THIS(UICharButton);
    self->flag  = false;
    self->state = UICharButton_Unknown9;
}

void UICharButton_Unknown9(void) {}

void UICharButton_Unknown10(void) {}

void UICharButton_Unknown11(void)
{
    RSDK_THIS(UICharButton);
    if (self->timer >= 30) {
        self->flag            = false;
        self->timer           = 0;
        self->state           = UICharButton_Unknown9;
        self->processButtonCB = UIButton_ProcessButtonCB_Alt;
    }
    else {
        if (self->timer == 2) {
            switch (self->characterID) {
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
void UICharButton_EditorDraw(void) { UICharButton_Draw(); }

void UICharButton_EditorLoad(void) { UICharButton->aniFrames = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE); }
#endif

void UICharButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UICharButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UICharButton, VAR_UINT8, characterID);
}
