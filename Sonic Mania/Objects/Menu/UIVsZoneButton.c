#include "SonicMania.h"

ObjectUIVsZoneButton *UIVsZoneButton;

void UIVsZoneButton_Update(void)
{
    RSDK_THIS(UIVsZoneButton);

    self->touchPosStart.x = 0x500000;
    self->touchPosStart.y = 0x380000;
    self->touchPosEnd.x   = 0;
    self->touchPosEnd.y   = 0;

    self->disabled = self->obfuscate || self->xOut;
    if (self->obfuscate != self->prevObfuscate) {
        UIVsZoneButton_SetNameText();
        self->prevObfuscate = self->obfuscate;
    }

    StateMachine_Run(self->state);

    if (self->flag) {
        self->field_124 -= 0x600;
        self->field_12C += self->field_124;
        if (self->field_12C <= 0x10000 && self->field_124 < 0) {
            self->field_12C = 0x10000;
            self->field_124 = 0;
        }

        self->field_130 -= 0x1800;
        self->field_134 += self->field_130;
        if (self->field_134 <= 0x8000 && self->field_130 < 0) {
            self->field_134 = 0x8000;
            self->field_130 = 0;
        }
    }
    else if (self->state != UIVsZoneButton_Unknown15) {
        if (self->field_12C > 0)
            self->field_12C = self->field_12C >> 1;
        if (self->field_134 > 0)
            self->field_134 = self->field_134 >> 1;
    }
    if (self->zoneID == 12 || self->xOut)
        RSDK.ProcessAnimation(&self->zoneAnimator);

    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->state == UIVsZoneButton_Unknown14) {
        if (parent->state != UIControl_ProcessInputs) {
            self->flag  = false;
            self->flag  = false;
            self->state = UIVsZoneButton_Unknown13;
        }
        else {
            int32 id = -1;
            for (int32 i = 0; i < parent->buttonCount; ++i) {
                if (self == (EntityUIVsZoneButton *)parent->buttons[i]) {
                    id = i;
                    break;
                }
            }

            if (parent->activeEntityID != id) {
                self->flag  = false;
                self->flag  = false;
                self->state = UIVsZoneButton_Unknown13;
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
    RSDK.DrawRect(self->position.x - 0x250000, self->position.y - 0x190000, 0x500000, 0x380000, 0xFFFFFF, 127, INK_BLEND, false);
    UIVsZoneButton_Unknown4();
    UIVsZoneButton_Unknown3();
    UIVsZoneButton_Unknown6();
    if (self->state != UIVsZoneButton_Unknown15 || !(self->timer & 2))
        UIVsZoneButton_Unknown5();
}

void UIVsZoneButton_Create(void *data)
{
    RSDK_THIS(UIVsZoneButton);
    self->active          = ACTIVE_BOUNDS;
    self->drawOrder       = 2;
    self->visible         = 1;
    self->drawFX          = FX_FLIP;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x300000;
    self->processButtonCB = UIButton_Unknown6;
    self->touchCB         = UIButton_ProcessTouch;
    self->options3        = UIVsZoneButton_Options3CB;
    self->failCB          = UIVsZoneButton_FailCB;
    self->options5        = UIVsZoneButton_Options5CB;
    self->options6        = UIVsZoneButton_Options6CB;
    self->options7        = UIVsZoneButton_Options7CB;
    self->options8        = UIVsZoneButton_Options8CB;
    self->state           = UIVsZoneButton_Unknown13;
    UIVsZoneButton_SetupAnimators();
    UIVsZoneButton_SetNameText();
}

void UIVsZoneButton_StageLoad(void)
{
    UIVsZoneButton->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsZoneButton->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);
    UIVsZoneButton->sfxFail    = RSDK.GetSFX("Stage/Fail.wav");
}

void UIVsZoneButton_SetupAnimators(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (SceneInfo->inEditor || self->zoneID == 12 || self->xOut)
        RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 16, &self->zoneAnimator, false, 0);
    else
        RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 17, &self->zoneAnimator, false, self->zoneID);
    RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 14, &self->animator1, false, 5);
    RSDK.SetSpriteAnimation(UIVsZoneButton->aniFrames, 14, &self->animator2, false, 6);

    self->field_118 = 0x300000;
    self->field_11C = 0xD0000;
    self->field_120 = 13;
}

void UIVsZoneButton_SetNameText(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &self->textAnimator, true, 0);
        if (self->obfuscate) {
            RSDK.PrependText(&self->nameText, "???");
            self->nameText.textLength = 3;
        }
        else {
            Localization_SetZoneNameShort(&self->nameText, self->zoneID);
            if (self->zoneID < 12) {
                char buffer[0x10];
                sprintf(buffer, " %d", self->act + 1);
                RSDK.AppendText(&self->nameText, buffer);
            }
        }
        RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &self->nameText);
    }
}

void UIVsZoneButton_Unknown3(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (!SceneInfo->inEditor)
        UIWidgets_DrawRectOutline_Blended(56, 80, self->position.x + 0x30000, self->position.y + 0x30000);
    if (self->flag)
        UIWidgets_DrawRectOutline_Flash(56, 80, self->position.x, self->position.y);
    else
        UIWidgets_DrawRectOutline_Black(56, 80, self->position.x, self->position.y);
}

void UIVsZoneButton_Unknown4(void)
{
    RSDK_THIS(UIVsZoneButton);
    UIWidgets_DrawRightTriangle(self->position.x - 0x250000, self->position.y - 0x190000, (23 * self->field_12C) >> 16, 232, 40, 88);
    UIWidgets_DrawRightTriangle(self->position.x + 0x250000, self->position.y + 0x180000, (-29 * self->field_12C) >> 16, 96, 160, 176);
    UIWidgets_DrawRightTriangle(self->position.x + 0x250000, self->position.y + 0x180000, (-17 * self->field_12C) >> 16, 88, 112, 224);
}

void UIVsZoneButton_Unknown5(void)
{
    RSDK_THIS(UIVsZoneButton);
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;

    if (parent && (parent->active == ACTIVE_ALWAYS || parent->dialogHasFocus) && RSDK.CheckOnScreen(parent, &parent->updateRange)
        && !SceneInfo->inEditor && self->zoneID != 12 && !self->xOut) {
        RSDK.CopyPalette(((self->zoneID >> 3) + 1), (32 * self->zoneID), 0, 224, 32);
    }
    RSDK.DrawRect(drawPos.x - 0x180000, drawPos.y - 0x140000, 0x300000, 0x280000, 0, 255, INK_BLEND, false);

    drawPos.x += self->field_13C;
    drawPos.y += self->field_13C;
    RSDK.DrawRect(drawPos.x - 0x180000, drawPos.y - 0x140000, 0x300000, 0x280000, 0xFFFFFF, 255, INK_NONE, false);
    if (self->xOut) {
        int32 dir           = self->direction;
        self->direction = self->zoneDirection;
        RSDK.DrawSprite(&self->zoneAnimator, &drawPos, false);

        self->direction = dir;
        drawPos.x += 0x10000;
        drawPos.y += 0x10000;
        self->inkEffect = INK_BLEND;
        RSDK.DrawSprite(&self->animator2, &drawPos, false);

        self->inkEffect = INK_NONE;
        drawPos.x -= 0x10000;
        drawPos.y -= 0x10000;
        RSDK.DrawSprite(&self->animator1, &drawPos, false);
    }
    else if (self->zoneID == 12) {
        int32 dir           = self->direction;
        self->direction = self->zoneDirection;
        RSDK.DrawSprite(&self->zoneAnimator, &drawPos, false);

        self->direction = dir;
    }
    else {
        RSDK.DrawSprite(&self->zoneAnimator, &drawPos, false);
    }
}

void UIVsZoneButton_Unknown6(void)
{
    RSDK_THIS(UIVsZoneButton);
    Vector2 drawPos;

    int32 size  = (self->field_11C + self->field_118) >> 16;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y + 0x280000;

    UIWidgets_DrawRhombus(self->field_11C >> 16, size, self->field_120, 0xF0, 0xF0, 0xF0, drawPos.x, drawPos.y);
    drawPos.x += self->field_13C;
    drawPos.y += self->field_13C;
    UIWidgets_DrawRhombus(self->field_11C >> 16, size, self->field_120, 0, 0, 0, drawPos.x, drawPos.y);

    if (!SceneInfo->inEditor) {
        int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->nameText, 0, self->nameText.textLength, 0);
        drawPos.y -= 0x10000;
        drawPos.x -= width << 15;
        RSDK.DrawText(&self->textAnimator, &drawPos, &self->nameText, 0, self->nameText.textLength, ALIGN_LEFT, 0, 0, 0, false);
    }
}

bool32 UIVsZoneButton_Options7CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    return self->state == UIVsZoneButton_Unknown14;
}

bool32 UIVsZoneButton_Options8CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    return self->state == UIVsZoneButton_Unknown15;
}

void UIVsZoneButton_Options3CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    EntityMenuParam *param  = (EntityMenuParam *)globals->menuParam;

    parent->state = StateMachine_None;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
    RSDK.StopChannel(Music->channelID);
    TimeAttackData_ClearOptions();
    param->vsZoneID = self->zoneID;
    param->vsActID  = self->act;
    self->state   = UIVsZoneButton_Unknown15;
}

void UIVsZoneButton_FailCB(void) { RSDK.PlaySfx(UIVsZoneButton->sfxFail, false, 255); }

void UIVsZoneButton_Options5CB(void)
{
    RSDK_THIS(UIVsZoneButton);

    self->state = UIVsZoneButton_Unknown14;
    if (!self->flag) {
        self->field_13C = 0;
        self->field_138 = -0x20000;
        self->field_124 = 0x4000;
        self->field_130 = 0x8000;
        self->flag      = true;
    }
}

void UIVsZoneButton_Options6CB(void)
{
    RSDK_THIS(UIVsZoneButton);
    self->flag  = false;
    self->state = UIVsZoneButton_Unknown13;
}

void UIVsZoneButton_Unknown13(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (self->zoneID == 12 || self->xOut)
        self->zoneDirection = self->zoneAnimator.frameID & 3;

    if (self->field_13C) {
        int32 dist = -(self->field_13C / abs(self->field_13C));
        self->field_13C += dist << 16;
        if (dist < 0) {
            if (self->field_13C < 0) {
                self->field_13C = 0;
            }
            else if (dist > 0 && self->field_13C > 0)
                self->field_13C = 0;
        }
        else if (dist > 0 && self->field_13C > 0)
            self->field_13C = 0;
    }
}

void UIVsZoneButton_Unknown14(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (self->zoneID == 12 || self->xOut)
        self->zoneDirection = self->zoneAnimator.frameID & 3;

    self->field_138 += 0x4800;
    self->field_13C += self->field_138;
    if (self->field_13C >= -0x20000 && self->field_138 > 0) {
        self->field_13C = -0x20000;
        self->field_138 = 0;
    }
}

void UIVsZoneButton_Unknown15(void)
{
    RSDK_THIS(UIVsZoneButton);
    if (self->zoneID == 12 || self->xOut)
        self->zoneDirection = self->zoneAnimator.frameID & 3;

    if (self->timer >= 30) {
        self->flag            = false;
        self->timer           = 0;
        self->state           = UIVsZoneButton_Unknown13;
        self->processButtonCB = UIButton_Unknown6;
        StateMachine_Run(self->options2);
    }
    else {
        self->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIVsZoneButton_EditorDraw(void) { UIVsZoneButton_Draw(); }

void UIVsZoneButton_EditorLoad(void)
{
    UIVsZoneButton->aniFrames  = RSDK.LoadSpriteAnimation("UI/SaveSelect.bin", SCOPE_STAGE);
    UIVsZoneButton->textFrames = RSDK.LoadSpriteAnimation("UI/TextEN.bin", SCOPE_STAGE);
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
