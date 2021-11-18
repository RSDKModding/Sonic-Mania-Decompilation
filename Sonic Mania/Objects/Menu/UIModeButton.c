#include "SonicMania.h"

ObjectUIModeButton *UIModeButton;

void UIModeButton_Update(void)
{
    RSDK_THIS(UIModeButton);

    self->touchPosStart.y = 0x380000;
    self->touchPosStart.x = 0x800000;
    self->touchPosStart.x += self->touchPosStart.y;
    self->touchPosStart.y += 0x60000;
    self->touchPosEnd.x = 0;
    self->touchPosEnd.y = -0x120000;
    if (self->textSpriteIndex != UIWidgets->textSpriteIndex || self->wasDisabled != self->disabled) {
        UIModeButton_Unknown1();
        self->textSpriteIndex = UIWidgets->textSpriteIndex;
        self->wasDisabled     = self->disabled;
    }
    StateMachine_Run(self->state);
}

void UIModeButton_LateUpdate(void) {}

void UIModeButton_StaticUpdate(void) {}

void UIModeButton_Draw(void)
{
    RSDK_THIS(UIModeButton);
    Vector2 drawPos;

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, (self->position.y >> 16) - ScreenInfo->position.y);
    drawPos = self->position;
    drawPos.y += self->field_118;
    RSDK.DrawSprite(&self->animator4, &drawPos, false);

    drawPos.x += self->field_11C;
    drawPos.y += self->field_11C >> 1;
    RSDK.DrawSprite(&self->animator2, &drawPos, false);

    drawPos = self->position;
    drawPos.y += self->field_114;
    RSDK.DrawSprite(&self->animator3, &drawPos, false);

    drawPos.x += self->field_11C;
    drawPos.y += self->field_11C >> 1;
    RSDK.DrawSprite(&self->animator1, &drawPos, false);

    RSDK.SetClipBounds(0, 0, 0, ScreenInfo->width, ScreenInfo->height);
    drawPos = self->position;
    drawPos.x -= self->field_11C;
    drawPos.y -= self->field_11C;
    UIWidgets_Unknown7(24, 128, 24, 240, 240, 240, drawPos.x, drawPos.y);

    drawPos = self->position;
    drawPos.x += self->field_11C;
    drawPos.y += self->field_11C;
    UIWidgets_Unknown7(24, 128, 24, 0, 0, 0, drawPos.x, drawPos.y);

    if (self->field_130) {
        drawPos = self->position;
        drawPos.y += self->field_110;
        RSDK.DrawSprite(&self->animator5, &drawPos, false);
    }
}

void UIModeButton_Create(void *data)
{
    RSDK_THIS(UIModeButton);
    if (!SceneInfo->inEditor) {
        self->visible         = true;
        self->drawOrder       = 2;
        self->active          = ACTIVE_BOUNDS;
        self->updateRange.x   = 0x800000;
        self->updateRange.y   = 0x400000;
        self->field_130       = true;
        self->field_114       = 0x280000;
        self->field_118       = 0x280000;
        self->processButtonCB = UIButton_Unknown6;
        self->touchCB         = UIButton_ProcessTouch;
        self->options3        = UIModeButton_Options3CB;
        self->failCB          = UIModeButton_FailCB;
        self->options5        = UIModeButton_Options5CB;
        self->options6        = UIModeButton_Options6CB;
        self->options7        = UIModeButton_Options7CB;
        self->options8        = UIModeButton_Options8CB;
        UIModeButton_Unknown1();
        self->textSpriteIndex = UIWidgets->textSpriteIndex;
    }
}

void UIModeButton_StageLoad(void) { UIModeButton->aniFrames = RSDK.LoadSpriteAnimation("UI/MainIcons.bin", SCOPE_STAGE); }

void UIModeButton_Unknown1(void)
{
    RSDK_THIS(UIModeButton);
    if (self->disabled) {
        RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 7, &self->animator5, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator1, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator3, true, 0);
        RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator4, true, 0);
    }
    else {
        switch (self->buttonID) {
            case 1:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &self->animator5, true, 1);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->animator1, true, 1);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->animator3, true, 1);
                RSDK.SetSpriteAnimation(0xFFFF, 1, &self->animator4, true, 0);
                break;
            case 2:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &self->animator5, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->animator1, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->animator2, true, 3);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->animator3, true, 2);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->animator4, true, 3);
                break;
            case 3:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &self->animator5, true, 3);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->animator1, true, 4);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->animator3, true, 4);
                RSDK.SetSpriteAnimation(0xFFFF, 1, &self->animator4, true, 0);
                break;
            default:
                RSDK.SetSpriteAnimation(UIWidgets->textSpriteIndex, 1, &self->animator5, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 0, &self->animator1, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &self->animator2, true, 0);
                RSDK.SetSpriteAnimation(UIModeButton->aniFrames, 1, &self->animator3, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 1, &self->animator4, true, 0);
                break;
        }
    }
}

bool32 UIModeButton_Options7CB(void)
{
    RSDK_THIS(UIModeButton);
    return self->state == UIModeButton_Unknown9;
}
bool32 UIModeButton_Options8CB(void)
{
    RSDK_THIS(UIModeButton);
    return self->state == UIModeButton_Unknown10;
}

void UIModeButton_Options5CB(void)
{
    RSDK_THIS(UIModeButton);
    if (self->state != UIModeButton_Unknown9) {
        self->field_110 = 0;
        self->field_114 = 0x180000;
        self->field_118 = 0x180000;
        self->field_11C = 0;
        self->field_120 = -0x20000;
        self->field_124 = -0x80000;
        self->field_128 = -0x80000;
        self->field_12C = -0x20000;
        self->state     = UIModeButton_Unknown9;
    }
}

void UIModeButton_Options3CB(void)
{
    RSDK_THIS(UIModeButton);
    EntityUIControl *parent = (EntityUIControl *)self->parent;
    if (self->buttonID == 2) {
#if RETRO_USE_PLUS
        int32 id = API_MostRecentActiveControllerID(1, 0, 5);
#else
        int32 id = API_MostRecentActiveControllerID(0);
#endif
        API_ResetControllerAssignments();
        API_AssignControllerID(1, id);
        API_AssignControllerID(2, CONT_AUTOASSIGN);
        API_AssignControllerID(3, CONT_AUTOASSIGN);
        API_AssignControllerID(4, CONT_AUTOASSIGN);
    }
    UITransition_StartTransition(self->options2, 14);
    if (self->stopMusic)
        RSDK.StopChannel(Music->channelID);
    self->flag         = false;
    self->state        = UIModeButton_Unknown10;
    parent->backoutTimer = 30;
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
}

void UIModeButton_FailCB(void) { RSDK.PlaySfx(UIWidgets->sfxFail, false, 255); }

void UIModeButton_Options6CB(void)
{
    RSDK_THIS(UIModeButton);
    self->state = UIModeButton_Unknown8;
}

void UIModeButton_Unknown8(void)
{
    RSDK_THIS(UIModeButton);

    if (self->field_110 < 0) {
        self->field_110 += 0x8000;
        if (self->field_110 > 0)
            self->field_110 = 0;
    }
    self->field_114 += (0x280000 - self->field_114) >> 3;
    self->field_118 += (0x280000 - self->field_118) >> 3;

    if (self->field_11C < 0) {
        self->field_11C += 0x10000;
        if (self->field_11C > 0)
            self->field_11C = 0;
    }
}

void UIModeButton_Unknown9(void)
{
    RSDK_THIS(UIModeButton);
    self->field_120 += 0x4000;
    self->field_110 += self->field_120;
    if (self->field_110 >= 0 && self->field_120 > 0) {
        self->field_110 = 0;
        self->field_120 = 0;
    }

    self->field_124 += 0xA000;
    self->field_114 += self->field_124;
    if (self->field_114 >= -0xC0000 && self->field_124 > 0) {
        self->field_114 = -0xC0000;
        self->field_124 = 0;
    }

    self->field_128 += 0x8000;
    self->field_118 += self->field_128;
    if (self->field_118 >= -0xC0000 && self->field_128 > 0) {
        self->field_118 = -0xC0000;
        self->field_128 = 0;
    }

    self->field_12C += 0x4800;
    self->field_11C += self->field_12C;
    if (self->field_11C >= -0x20000 && self->field_12C > 0) {
        self->field_11C = -0x20000;
        self->field_12C = 0;
    }
}

void UIModeButton_Unknown10(void)
{
    RSDK_THIS(UIModeButton);
    UIModeButton_Unknown9();
    if (++self->timer == 30) {
        self->timer = 0;
        self->state = UIModeButton_Unknown9;
    }
    self->field_130 = !((self->timer >> 1) & 1);
}

#if RETRO_INCLUDE_EDITOR
void UIModeButton_EditorDraw(void)
{
    RSDK_THIS(UIModeButton);
    self->visible         = true;
    self->drawOrder       = 2;
    self->updateRange.x   = 0x800000;
    self->updateRange.y   = 0x400000;
    self->field_130       = true;
    self->field_114       = 0x280000;
    self->field_118       = 0x280000;
    UIModeButton_Unknown1();
    self->textSpriteIndex = UIWidgets->textSpriteIndex;

    UIModeButton_Draw();
}

void UIModeButton_EditorLoad(void) { UIModeButton->aniFrames = RSDK.LoadSpriteAnimation("UI/MainIcons.bin", SCOPE_STAGE); }
#endif

void UIModeButton_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIModeButton, VAR_BOOL, disabled);
    RSDK_EDITABLE_VAR(UIModeButton, VAR_ENUM, buttonID);
    RSDK_EDITABLE_VAR(UIModeButton, VAR_BOOL, stopMusic);
}
