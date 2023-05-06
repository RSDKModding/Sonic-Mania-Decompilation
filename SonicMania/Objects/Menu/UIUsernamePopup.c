// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIUsernamePopup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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

    if (self->isVisible)
        UIUsernamePopup_DrawSprites();
}

void UIUsernamePopup_Create(void *data)
{
    RSDK_THIS(UIUsernamePopup);

    self->active    = ACTIVE_ALWAYS;
    self->drawGroup = 13;
    self->drawPos.x = 0;
    self->drawPos.y = 0;
    self->size.y    = 0x180000;
    self->visible   = true;
    self->state     = UIUsernamePopup_State_Init;
}

void UIUsernamePopup_StageLoad(void)
{
    foreach_all(UIUsernamePopup, popup) { UIUsernamePopup->popup = (Entity *)popup; }
}

void UIUsernamePopup_ShowPopup(void)
{
    EntityUIUsernamePopup *entity = (EntityUIUsernamePopup *)UIUsernamePopup->popup;
    if (entity->state == UIUsernamePopup_State_Init) {
        if (API_GetUsername(&entity->username)) {
#if MANIA_USE_PLUS
            LogHelpers_PrintString(&entity->username);
#endif
            RSDK.SetSpriteAnimation(UIWidgets->fontFrames, 0, &entity->animator, true, 0);
            RSDK.SetSpriteString(UIWidgets->fontFrames, 0, &entity->username);
            int32 width    = RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &entity->username, 0, entity->username.length, 0);
            entity->state  = UIUsernamePopup_State_Appear;
            entity->timer  = 0;
            entity->size.x = (width + 16) << 16;
            entity->delay  = 30;
        }
    }
}

void UIUsernamePopup_DrawSprites(void)
{
    RSDK_THIS(UIUsernamePopup);

    Vector2 drawPos;
    drawPos.x = self->drawPos.x + self->size.y + (ScreenInfo->position.x << 16) + (self->size.x >> 1);
    drawPos.y = self->drawPos.y + (ScreenInfo->center.y << 16) - (self->size.y >> 1) + ((ScreenInfo->center.y + ScreenInfo->position.y) << 16);
    UIWidgets_DrawParallelogram(drawPos.x, drawPos.y, self->size.x >> 16, self->size.y >> 16, self->size.y >> 16, 0x10, 0x7C, 0x10);

    int32 width = RSDK.GetStringWidth(UIWidgets->fontFrames, 0, &self->username, 0, self->username.length, 0);
    drawPos.y -= 0x10000;
    drawPos.x -= width << 15;
    RSDK.DrawText(&self->animator, &drawPos, &self->username, 0, self->username.length, ALIGN_LEFT, 0, 0, 0, false);
}

void UIUsernamePopup_State_Init(void)
{
    RSDK_THIS(UIUsernamePopup);

    self->isVisible = false;
    self->timer     = 0;
}

void UIUsernamePopup_State_Appear(void)
{
    RSDK_THIS(UIUsernamePopup);

    if (self->timer >= self->delay) {
        if (self->timer >= self->delay + 8) {
            self->drawPos.x = 0;
            self->drawPos.y = 0;
            self->timer     = 0;
            self->state     = UIUsernamePopup_State_Shown;
        }
        else {
            self->isVisible = true;
            MathHelpers_Lerp(&self->drawPos, 32 * CLAMP(self->timer - self->delay, 0, 8), -self->size.y, self->size.y, 0, 0);
            ++self->timer;
        }
    }
    else {
        self->isVisible = false;
        self->timer++;
    }
}

void UIUsernamePopup_State_Shown(void)
{
    RSDK_THIS(UIUsernamePopup);

    if (self->timer >= 180) {
        self->timer = 0;
        self->state = UIUsernamePopup_State_Disappear;
    }
    else {
        self->timer++;
    }
}

void UIUsernamePopup_State_Disappear(void)
{
    RSDK_THIS(UIUsernamePopup);

    if (self->timer >= 8) {
        self->drawPos.y = self->size.y;
        self->timer     = 0;
        self->isVisible = false;
        self->drawPos.x = -self->size.y;
        self->state     = UIUsernamePopup_State_Init;
    }
    else {
        self->isVisible = true;

        int32 percent = 0x20 * MAX(self->timer++, 0);
        MathHelpers_LerpToPos(&self->drawPos, percent, -self->size.y, self->size.y);
    }
}

#if GAME_INCLUDE_EDITOR
void UIUsernamePopup_EditorDraw(void)
{
    RSDK_THIS(UIUsernamePopup);

    self->drawGroup = 13;
    self->drawPos.x = 0;
    self->drawPos.y = 0;
    self->size.x    = 0;
    self->size.y    = 0x180000;

    UIWidgets_DrawParallelogram(self->position.x, self->position.y, self->size.x >> 16, self->size.y >> 16, self->size.y >> 16, 0x10, 0x7C, 0x10);
}

void UIUsernamePopup_EditorLoad(void) {}
#endif

void UIUsernamePopup_Serialize(void) {}
