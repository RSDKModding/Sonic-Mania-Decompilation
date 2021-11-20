#include "SonicMania.h"

ObjectUIUsernamePopup *UIUsernamePopup;

void UIUsernamePopup_Update(void)
{
    RSDK_THIS(UIUsernamePopup);
    StateMachine_Run(self->state);
}

void UIUsernamePopup_LateUpdate(void) {}

void UIUsernamePopup_StaticUpdate(void) {}

void UIUsernamePopup_Draw(void)
{
    RSDK_THIS(UIUsernamePopup);
    if (self->drawFlag)
        UIUsernamePopup_DrawSprites();
}

void UIUsernamePopup_Create(void *data)
{
    RSDK_THIS(UIUsernamePopup);
    self->active       = ACTIVE_ALWAYS;
    self->drawOrder    = 13;
    self->posUnknown.x = 0;
    self->posUnknown.y = 0;
    self->dword6C      = 0x180000;
    self->visible      = true;
    self->state        = UIUsernamePopup_State_Setup;
}

void UIUsernamePopup_StageLoad(void)
{
    foreach_all(UIUsernamePopup, popup) { UIUsernamePopup->popup = (Entity *)popup; }
}

void UIUsernamePopup_ShowPopup(void)
{
    EntityUIUsernamePopup *entity = (EntityUIUsernamePopup *)UIUsernamePopup->popup;
    if (entity->state == UIUsernamePopup_State_Setup) {
        if (API_GetUsername(&entity->username)) {
#if RETRO_USE_PLUS
            RSDK.PrintText(0, &entity->username);
#endif
            RSDK.SetSpriteAnimation(UIWidgets->labelSpriteIndex, 0, &entity->animator, true, 0);
            RSDK.SetSpriteString(UIWidgets->labelSpriteIndex, 0, &entity->username);
            int32 width      = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &entity->username, 0, entity->username.textLength, 0);
            entity->state    = UIUsernamePopup_Unknown4;
            entity->timer    = 0;
            entity->field_68 = (width + 16) << 16;
            entity->timeOut  = 30;
        }
    }
}

void UIUsernamePopup_DrawSprites(void)
{
    RSDK_THIS(UIUsernamePopup);
    Vector2 drawPos;

    drawPos.x = self->posUnknown.x + self->dword6C + (ScreenInfo->position.x << 16) + (self->field_68 >> 1);
    drawPos.y =
        self->posUnknown.y + (ScreenInfo->centerY << 16) - (self->dword6C >> 1) + ((ScreenInfo->centerY + ScreenInfo->position.y) << 16);
    UIWidgets_DrawRhombus(self->dword6C >> 16, self->field_68 >> 16, self->dword6C >> 16, 16, 124, 16, drawPos.x, drawPos.y);

    int32 width = RSDK.GetStringWidth(UIWidgets->labelSpriteIndex, 0, &self->username, 0, self->username.textLength, 0);
    drawPos.y -= 0x10000;
    drawPos.x -= width << 15;
    RSDK.DrawText(&self->animator, &drawPos, &self->username, 0, self->username.textLength, ALIGN_LEFT, 0, 0, 0, false);
}

void UIUsernamePopup_State_Setup(void)
{
    RSDK_THIS(UIUsernamePopup);
    self->drawFlag = false;
    self->timer    = 0;
}

void UIUsernamePopup_Unknown4(void)
{
    RSDK_THIS(UIUsernamePopup);

    if (self->timer >= self->timeOut) {
        if (self->timer >= self->timeOut + 8) {
            self->posUnknown.x = 0;
            self->posUnknown.y = 0;
            self->timer        = 0;
            self->state        = UIUsernamePopup_Unknown5;
        }
        else {
            self->drawFlag = true;
            MathHelpers_Lerp1(&self->posUnknown, 32 * clampVal(self->timer - self->timeOut, 0, 8), -self->dword6C, self->dword6C, 0, 0);
            ++self->timer;
        }
    }
    else {
        self->drawFlag = false;
        self->timer++;
    }
}

void UIUsernamePopup_Unknown5(void)
{
    RSDK_THIS(UIUsernamePopup);
    if (self->timer >= 180) {
        self->timer = 0;
        self->state = UIUsernamePopup_Unknown6;
    }
    else {
        self->timer++;
    }
}

void UIUsernamePopup_Unknown6(void)
{
    RSDK_THIS(UIUsernamePopup);
    if (self->timer >= 8) {
        self->posUnknown.y = self->dword6C;
        self->timer        = 0;
        self->drawFlag     = false;
        self->posUnknown.x = -self->dword6C;
        self->state        = UIUsernamePopup_State_Setup;
    }
    else {
        self->drawFlag = true;

        int32 val = 32 * maxVal(self->timer, 0);
        if (val > 0) {
            if (val < 256) {
                self->posUnknown.x = val * (-self->dword6C >> 8);
                self->posUnknown.y = val * (self->dword6C >> 8);
            }
            else {
                self->posUnknown.x = -self->dword6C;
                self->posUnknown.y = self->dword6C;
            }
        }
        else {
            self->posUnknown.x = 0;
            self->posUnknown.y = 0;
        }
        ++self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIUsernamePopup_EditorDraw(void) {}

void UIUsernamePopup_EditorLoad(void) {}
#endif

void UIUsernamePopup_Serialize(void) {}
