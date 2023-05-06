// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIWaitSpinner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectUIWaitSpinner *UIWaitSpinner;

void UIWaitSpinner_Update(void)
{
    RSDK_THIS(UIWaitSpinner);

    StateMachine_Run(self->state);

    self->visible = true;

    switch (self->timer) {
        case 0: self->visible = false; break;

        case 8:
            // overwritten just below lmao
            self->inkEffect = INK_BLEND;
            break;

        default:
            if (self->timer >= 16) // overwritten just below lmao
                self->inkEffect = INK_NONE;
            break;
    }

    self->inkEffect = INK_ALPHA;
    self->alpha     = 16 * self->timer;
    if (self->alpha >= 0x100)
        self->alpha = 0xFF;

    RSDK.ProcessAnimation(&self->animator);
}

void UIWaitSpinner_LateUpdate(void) {}

void UIWaitSpinner_StaticUpdate(void) {}

void UIWaitSpinner_Draw(void)
{
    RSDK_THIS(UIWaitSpinner);

    Vector2 drawPos;
    drawPos.x = (ScreenInfo->size.x - 24) << 16;
    drawPos.y = (ScreenInfo->size.y - 24) << 16;
    RSDK.DrawSprite(&self->animator, &drawPos, true);
}

void UIWaitSpinner_Create(void *data)
{
    RSDK_THIS(UIWaitSpinner);

    self->active    = ACTIVE_ALWAYS;
    self->visible   = true;
    self->drawGroup = 15;

    RSDK.SetSpriteAnimation(UIWaitSpinner->aniFrames, 0, &self->animator, true, 0);
    self->state = UIWaitSpinner_State_Show;
}

void UIWaitSpinner_StageLoad(void)
{
    UIWaitSpinner->timer         = 0;
    UIWaitSpinner->activeSpinner = NULL;

    UIWaitSpinner->aniFrames = RSDK.LoadSpriteAnimation("UI/WaitSpinner.bin", SCOPE_STAGE);
}

void UIWaitSpinner_StartWait(void)
{
    ++UIWaitSpinner->timer;

    EntityUIWaitSpinner *spinner = UIWaitSpinner->activeSpinner;

    if (UIWaitSpinner->timer <= 0) {
        if (spinner)
            spinner->state = UIWaitSpinner_State_Hide;
    }
    else {
        if (!spinner) {
            spinner              = CREATE_ENTITY(UIWaitSpinner, NULL, 0, 0);
            spinner->isPermanent = true;

            UIWaitSpinner->activeSpinner = spinner;
        }

        spinner->state = UIWaitSpinner_State_Show;
    }
}
void UIWaitSpinner_FinishWait(void)
{
    if (UIWaitSpinner->timer > 0)
        UIWaitSpinner->timer--;

    EntityUIWaitSpinner *spinner = UIWaitSpinner->activeSpinner;

    if (UIWaitSpinner->timer <= 0) {
        if (spinner)
            spinner->state = UIWaitSpinner_State_Hide;
    }
    else {
        if (!spinner) {
            spinner              = CREATE_ENTITY(UIWaitSpinner, NULL, 0, 0);
            spinner->isPermanent = true;

            UIWaitSpinner->activeSpinner = spinner;
        }

        spinner->state = UIWaitSpinner_State_Show;
    }
}
void UIWaitSpinner_State_Show(void)
{
    RSDK_THIS(UIWaitSpinner);

    if (self->timer >= 16) {
        self->timer   = 16;
        self->fadedIn = true;
    }
    else {
        self->timer += 3;
    }
}
void UIWaitSpinner_State_Hide(void)
{
    RSDK_THIS(UIWaitSpinner);

    if (self->timer <= 0) {
        UIWaitSpinner->activeSpinner = NULL;
        destroyEntity(self);
    }
    else if (self->fadedIn) {
        self->timer -= 3;
    }
    else {
        UIWaitSpinner_State_Show();
    }
}

#if GAME_INCLUDE_EDITOR
void UIWaitSpinner_EditorDraw(void) {}

void UIWaitSpinner_EditorLoad(void) {}
#endif

void UIWaitSpinner_Serialize(void) {}
