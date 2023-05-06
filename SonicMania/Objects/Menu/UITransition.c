// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: UITransition Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    self->drawGroup    = 13;
    self->drawPos[0].x = 0;
    self->drawPos[0].y = 0;
    self->drawPos[1].x = 0;
    self->drawPos[1].y = 0;
    self->drawPos[2].x = 0;
    self->drawPos[2].y = 0;

    self->visible = true;
    self->state   = UITransition_State_Init;
}

void UITransition_StageLoad(void)
{
    foreach_all(UITransition, transition) { UITransition->activeTransition = (Entity *)transition; }
}

void UITransition_StartTransition(void (*callback)(void), int32 delay)
{
    EntityUITransition *transition = (EntityUITransition *)UITransition->activeTransition;

    if (transition->state == UITransition_State_Init && !UIDialog->activeDialog) {
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

    if (transition->state == UITransition_State_Init) {
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
    uint32 colors[3];

    colors[0] = 0xE48E00;
    colors[1] = 0x1888F0;
    colors[2] = 0xE82858;

    int32 screenCenterX = (ScreenInfo->position.x + ScreenInfo->center.x) << 16;
    int32 screenCenterY = (ScreenInfo->position.y + ScreenInfo->center.y) << 16;
    positions[0].x      = screenCenterX + self->drawPos[0].x - 0xF00000;
    positions[0].y      = screenCenterY + self->drawPos[0].y;
    positions[1].x      = screenCenterX + self->drawPos[1].x;
    positions[1].y      = screenCenterY + self->drawPos[1].y;
    positions[2].x      = screenCenterX + self->drawPos[2].x + 0xF00000;
    positions[2].y      = screenCenterY + self->drawPos[2].y;

    for (int32 i = 0; i < 3; ++i) {
        UIWidgets_DrawParallelogram(positions[i].x, positions[i].y, 0, SCREEN_YSIZE, SCREEN_YSIZE, (colors[i] >> 16) & 0xFF, (colors[i] >> 8) & 0xFF,
                                    colors[i] & 0xFF);
    }
}

void UITransition_State_Init(void)
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

        int32 remain = self->timer - self->delay;
        if (!remain)
            RSDK.PlaySfx(UIWidgets->sfxWoosh, false, 255);

        int32 offsets[3];
        offsets[0] = CLAMP(remain, 0, 8);
        offsets[1] = CLAMP(remain - 4, 0, 8);
        offsets[2] = CLAMP(remain - 8, 0, 8);

        int32 percent = 32 * offsets[0];
        if (percent > 0) {
            if (percent < 256) {
                self->drawPos[0].x = -0xF000 * percent + 0xF00000;
                self->drawPos[0].y = 0xF000 * percent - 0xF00000;
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

        percent = 32 * offsets[1];
        if (percent > 0) {
            if (percent < 256) {
                self->drawPos[1].x = 0xF000 * percent - 0xF00000;
                self->drawPos[1].y = -0xF000 * percent + 0xF00000;
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

        percent = 32 * offsets[2];
        if (percent > 0) {
            if (percent < 256) {
                self->drawPos[2].x = -0xF000 * percent + 0xF00000;
                self->drawPos[2].y = 0xF000 * percent - 0xF00000;
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
            self->timer           = 0;
            self->isTransitioning = false;
            self->drawPos[0].x    = -0xF00000;
            self->drawPos[0].y    = 0xF00000;
            self->drawPos[1].x    = 0xF00000;
            self->drawPos[1].y    = -0xF00000;
            self->drawPos[2].x    = -0xF00000;
            self->drawPos[2].y    = 0xF00000;

            EntityUIControl *control = UIControl_GetUIControl();
            if (control)
                control->selectionDisabled = false;

            self->state = UITransition_State_Init;
        }
        else {
            self->isTransitioning = true;
            int32 offsets[3];

            offsets[0] = CLAMP(self->timer - 1, 0, 8);
            offsets[1] = CLAMP(self->timer - 4, 0, 8);
            offsets[2] = CLAMP(self->timer - 1, 0, 8);

            int32 percent = 32 * offsets[0];
            if (percent > 0) {
                if (percent < 256) {
                    self->drawPos[0].x = -0xF000 * percent;
                    self->drawPos[0].y = 0xF000 * percent;
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

            percent = 32 * offsets[1];
            if (percent > 0) {
                if (percent < 256) {
                    self->drawPos[1].x = 0xF000 * percent;
                    self->drawPos[1].y = -0xF000 * percent;
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

            percent = 32 * offsets[2];
            if (percent > 0) {
                if (percent < 256) {
                    self->drawPos[2].x = -0xF000 * percent;
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
#if RETRO_USE_MOD_LOADER
            StateMachine_Run(self->callback);
#else
            self->callback();
#endif
            self->callback    = StateMachine_None;
            SceneInfo->entity = storeEntity;
        }

        if (MenuSetup) {
#if MANIA_USE_PLUS
            ManiaModeMenu_ChangeMenuTrack();
            ManiaModeMenu_SetBGColors();
#else
            MenuSetup_ChangeMenuTrack();
            MenuSetup_SetBGColors();
#endif
        }

        EntityUIControl *control   = UIControl_GetUIControl();
        control->selectionDisabled = true;
        ++self->timer;
    }
}

#if GAME_INCLUDE_EDITOR
void UITransition_EditorDraw(void) {}

void UITransition_EditorLoad(void) {}
#endif

void UITransition_Serialize(void) {}
