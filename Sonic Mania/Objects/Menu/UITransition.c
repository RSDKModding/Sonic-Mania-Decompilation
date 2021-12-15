// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UITransition Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectUITransition *UITransition;

void UITransition_Update(void)
{
    RSDK_THIS(UITransition);
    StateMachine_Run(self->state);
}

void UITransition_LateUpdate(void) {}

void UITransition_StaticUpdate(void) {}

void UITransition_Draw(void)
{
    RSDK_THIS(UITransition);
    if (self->isTransitioning)
        UITransition_DrawShapes();
}

void UITransition_Create(void *data)
{
    RSDK_THIS(UITransition);
    self->active       = ACTIVE_ALWAYS;
    self->drawOrder    = 13;
    self->drawPos[0].x = 0;
    self->drawPos[0].y = 0;
    self->drawPos[1].x = 0;
    self->drawPos[1].y = 0;
    self->drawPos[2].x = 0;
    self->drawPos[2].y = 0;
    self->visible      = true;
    self->state        = UITransition_State_Setup;
}

void UITransition_StageLoad(void)
{
    foreach_all(UITransition, transition) { UITransition->activeTransition = (Entity *)transition; }
}

void UITransition_StartTransition(void (*callback)(void), int32 delay)
{
    EntityUITransition *transition = (EntityUITransition *)UITransition->activeTransition;
    if (transition->state == UITransition_State_Setup && !UIDialog->activeDialog) {
        transition->state      = UITransition_State_TransitionIn;
        transition->timer      = 0;
        transition->delay      = delay;
        transition->callback   = callback;
        transition->prevEntity = SceneInfo->entity;
    }

    EntityUIControl *control = UIControl_GetUIControl();
    if (control)
        control->selectionDisabled = true;
}

void UITransition_MatchNewTag(void)
{
    UIControl_MatchMenuTag(UITransition->newTag);
    UITransition->newTag = NULL;
}

void UITransition_SetNewTag(const char *text)
{
    EntityUITransition *transition = (EntityUITransition *)UITransition->activeTransition;
    if (transition->state == UITransition_State_Setup) {
        if (!UIDialog->activeDialog) {
            UITransition->newTag = (char *)text;
            UITransition_StartTransition(UITransition_MatchNewTag, 0);
        }
    }
}

void UITransition_DrawShapes(void)
{
    RSDK_THIS(UITransition);
    Vector2 positions[3];
    uint32 colours[3];

    colours[0] = 0xE48E00;
    colours[1] = 0x1888F0;
    colours[2] = 0xE82858;

    int32 screenCenterX = (ScreenInfo->position.x + ScreenInfo->centerX) << 16;
    int32 screenCenterY = (ScreenInfo->position.y + ScreenInfo->centerY) << 16;
    positions[0].x      = screenCenterX + self->drawPos[0].x - 0xF00000;
    positions[0].y      = screenCenterY + self->drawPos[0].y;
    positions[1].x      = screenCenterX + self->drawPos[1].x;
    positions[1].y      = screenCenterY + self->drawPos[1].y;
    positions[2].x      = screenCenterX + self->drawPos[2].x + 0xF00000;
    positions[2].y      = screenCenterY + self->drawPos[2].y;

    for (int32 i = 0; i < 3; ++i) {
        UIWidgets_DrawParallelogram(240, 0, 240, (colours[i] >> 16) & 0xFF, (colours[i] >> 8) & 0xFF, colours[i] & 0xFF, positions[i].x,
                                    positions[i].y);
    }
}

void UITransition_State_Setup(void)
{
    RSDK_THIS(UITransition);
    self->isTransitioning = false;
    self->timer           = 0;
}

void UITransition_State_TransitionIn(void)
{
    RSDK_THIS(UITransition);

    if (self->timer < self->delay)
        self->isTransitioning = false;
    if (self->timer > self->delay + 16) {
        self->drawPos[0].y = 0;
        self->drawPos[1].x = 0;
        self->drawPos[1].y = 0;
        self->drawPos[2].y = 0;
        self->drawPos[0].x = 0;
        self->drawPos[2].x = 0;
        self->timer        = 0;
        self->state        = UITransition_State_TransitionOut;
    }
    else {
        self->isTransitioning = true;
        int32 remain          = self->timer - self->delay;
        if (!remain)
            RSDK.PlaySfx(UIWidgets->sfxWoosh, false, 255);

        int32 offsets[3];
        offsets[0] = clampVal(remain, 0, 8);
        offsets[1] = clampVal(remain - 4, 0, 8);
        offsets[2] = clampVal(remain - 8, 0, 8);

        int32 val = 32 * offsets[0];
        if (val > 0) {
            if (val < 256) {
                self->drawPos[0].x = -0xF000 * val + 0xF00000;
                self->drawPos[0].y = 0xF000 * val - 0xF00000;
            }
            else {
                self->drawPos[0].x = 0;
                self->drawPos[0].y = 0;
            }
        }
        else {
            self->drawPos[0].x = 0xF00000;
            self->drawPos[0].y = -0xF00000;
        }

        val = 32 * offsets[1];
        if (val > 0) {
            if (val < 256) {
                self->drawPos[1].x = 0xF000 * val - 0xF00000;
                self->drawPos[1].y = -0xF000 * val + 0xF00000;
            }
            else {
                self->drawPos[1].x = 0;
                self->drawPos[1].y = 0;
            }
        }
        else {
            self->drawPos[1].x = -0xF00000;
            self->drawPos[1].y = 0xF00000;
        }

        val = 32 * offsets[2];
        if (val > 0) {
            if (val < 256) {
                self->drawPos[2].x = -0xF000 * val + 0xF00000;
                self->drawPos[2].y = 0xF000 * val - 0xF00000;
            }
            else {
                self->drawPos[2].x = 0;
                self->drawPos[2].y = 0;
            }
        }
        else {
            self->drawPos[2].x = 0xF00000;
            self->drawPos[2].y = -0xF00000;
        }
        ++self->timer;
    }
}

void UITransition_State_TransitionOut(void)
{
    RSDK_THIS(UITransition);

    if (self->timer >= 1) {
        if (self->timer > 16) {
            self->timer              = 0;
            self->isTransitioning    = false;
            self->drawPos[0].x       = -0xF00000;
            self->drawPos[0].y       = 0xF00000;
            self->drawPos[1].x       = 0xF00000;
            self->drawPos[1].y       = -0xF00000;
            self->drawPos[2].x       = -0xF00000;
            self->drawPos[2].y       = 0xF00000;
            EntityUIControl *control = UIControl_GetUIControl();
            if (control)
                control->selectionDisabled = false;
            self->state = UITransition_State_Setup;
        }
        else {
            self->isTransitioning = true;
            int32 offsets[3];

            offsets[0] = clampVal(self->timer - 1, 0, 8);
            offsets[1] = clampVal(self->timer - 4, 0, 8);
            offsets[2] = clampVal(self->timer - 1, 0, 8);

            int32 val = 32 * offsets[0];
            if (val > 0) {
                if (val < 256) {
                    self->drawPos[0].x = -0xF000 * val;
                    self->drawPos[0].y = 0xF000 * val;
                }
                else {
                    self->drawPos[0].x = -0xF00000;
                    self->drawPos[0].y = 0xF00000;
                }
            }
            else {
                self->drawPos[0].x = 0;
                self->drawPos[0].y = 0;
            }

            val = 32 * offsets[1];
            if (val > 0) {
                if (val < 256) {
                    self->drawPos[1].x = 0xF000 * val;
                    self->drawPos[1].y = -0xF000 * val;
                }
                else {
                    self->drawPos[1].x = 0xF00000;
                    self->drawPos[1].y = -0xF00000;
                }
            }
            else {
                self->drawPos[1].x = 0;
                self->drawPos[1].y = 0;
            }

            val = 32 * offsets[2];
            if (val > 0) {
                if (val < 256) {
                    self->drawPos[2].x = -0xF000 * val;
                    self->drawPos[2].y = 0x1E0000 * offsets[2];
                }
                else {
                    self->drawPos[2].x = -0xF00000;
                    self->drawPos[2].y = 0xF00000;
                }
            }
            else {
                self->drawPos[2].x = 0;
                self->drawPos[2].y = 0;
            }
            ++self->timer;
        }
    }
    else {
        self->isTransitioning                       = true;
        UIControl_GetUIControl()->selectionDisabled = false;
        if (self->callback && !UIDialog->activeDialog) {
            Entity *storeEntity = SceneInfo->entity;
            SceneInfo->entity   = self->prevEntity;
            self->callback();
            self->callback    = StateMachine_None;
            SceneInfo->entity = storeEntity;
        }
        if (MenuSetup) {
#if RETRO_USE_PLUS
            ManiaModeMenu_ChangeMenuTrack();
            ManiaModeMenu_SetBGColours();
#else
            MenuSetup_ChangeMenuTrack();
            MenuSetup_SetBGColours();
#endif
        }
        EntityUIControl *control   = UIControl_GetUIControl();
        control->selectionDisabled = true;
        ++self->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void UITransition_EditorDraw(void) {}

void UITransition_EditorLoad(void) {}
#endif

void UITransition_Serialize(void) {}
