// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UIWaitSpinner Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUIWaitSpinner *UIWaitSpinner;

void UIWaitSpinner_Update(void)
{
    RSDK_THIS(UIWaitSpinner);
    StateMachine_Run(self->state);

    self->visible = true;
    if (self->timer < 16) {
        if (self->timer == 8) {
            self->inkEffect = INK_BLEND;
        }
        else if (!self->timer) {
            self->visible = false;
        }
    }
    else {
        self->inkEffect = INK_NONE;
    }

    self->inkEffect = INK_ALPHA;
    if (16 * self->timer < 255)
        self->alpha = 16 * self->timer;
    else
        self->alpha = 255;
    RSDK.ProcessAnimation(&self->animator);
}

void UIWaitSpinner_LateUpdate(void) {}

void UIWaitSpinner_StaticUpdate(void) {}

void UIWaitSpinner_Draw(void)
{
    RSDK_THIS(UIWaitSpinner);
    Vector2 drawPos;

    drawPos.y = 0;
    drawPos.x = (ScreenInfo->width - 24) << 16;
    drawPos.y = (ScreenInfo->height - 24) << 16;
    RSDK.DrawSprite(&self->animator, &drawPos, true);
}

void UIWaitSpinner_Create(void *data)
{
    RSDK_THIS(UIWaitSpinner);
    self->active    = ACTIVE_ALWAYS;
    self->visible   = true;
    self->drawOrder = 15;
    RSDK.SetSpriteAnimation(UIWaitSpinner->aniFrames, 0, &self->animator, true, 0);
    self->state = UIWaitSpinner_State_Wait;
}

void UIWaitSpinner_StageLoad(void)
{
    UIWaitSpinner->timer         = 0;
    UIWaitSpinner->activeSpinner = NULL;
    UIWaitSpinner->aniFrames     = RSDK.LoadSpriteAnimation("UI/WaitSpinner.bin", SCOPE_STAGE);
}

void UIWaitSpinner_StartWait(void)
{
    ++UIWaitSpinner->timer;
    EntityUIWaitSpinner *activeSpinner = (EntityUIWaitSpinner *)UIWaitSpinner->activeSpinner;
    if (UIWaitSpinner->timer <= 0) {
        if (activeSpinner)
            activeSpinner->state = UIWaitSpinner_State_WaitAndDestroy;
    }
    else {
        if (!activeSpinner) {
            activeSpinner                = CREATE_ENTITY(UIWaitSpinner, NULL, 0, 0);
            activeSpinner->isPermanent   = true;
            UIWaitSpinner->activeSpinner = (Entity *)activeSpinner;
        }
        activeSpinner->state = UIWaitSpinner_State_Wait;
    }
}
void UIWaitSpinner_FinishWait(void)
{
    if (UIWaitSpinner->timer > 0) {
        UIWaitSpinner->timer--;
    }

    EntityUIWaitSpinner *activeSpinner = (EntityUIWaitSpinner *)UIWaitSpinner->activeSpinner;
    if (UIWaitSpinner->timer <= 0) {
        if (activeSpinner)
            activeSpinner->state = UIWaitSpinner_State_WaitAndDestroy;
    }
    else {
        if (!activeSpinner) {
            activeSpinner                = CREATE_ENTITY(UIWaitSpinner, NULL, 0, 0);
            activeSpinner->isPermanent   = true;
            UIWaitSpinner->activeSpinner = (Entity *)activeSpinner;
        }
        activeSpinner->state = UIWaitSpinner_State_Wait;
    }
}
void UIWaitSpinner_State_Wait(void)
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
void UIWaitSpinner_State_WaitAndDestroy(void)
{
    RSDK_THIS(UIWaitSpinner);
    if (self->timer <= 0) {
        UIWaitSpinner->activeSpinner = NULL;
        destroyEntity(self);
    }
    else if (self->fadedIn) {
        self->timer -= 3;
    }
    else if (self->timer >= 16) {
        self->timer   = 16;
        self->fadedIn = true;
    }
    else {
        self->timer += 3;
    }
}

#if RETRO_INCLUDE_EDITOR
void UIWaitSpinner_EditorDraw(void) {}

void UIWaitSpinner_EditorLoad(void) {}
#endif

void UIWaitSpinner_Serialize(void) {}
