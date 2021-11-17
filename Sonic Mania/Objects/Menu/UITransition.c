#include "SonicMania.h"

ObjectUITransition *UITransition;

void UITransition_Update(void)
{
    RSDK_THIS(UITransition);
    StateMachine_Run(entity->state);
}

void UITransition_LateUpdate(void) {}

void UITransition_StaticUpdate(void) {}

void UITransition_Draw(void)
{
    RSDK_THIS(UITransition);
    if (entity->field_6C)
        UITransition_DrawShapes();
}

void UITransition_Create(void *data)
{
    RSDK_THIS(UITransition);
    entity->active       = ACTIVE_ALWAYS;
    entity->drawOrder    = 13;
    entity->drawPos[0].x = 0;
    entity->drawPos[0].y = 0;
    entity->drawPos[1].x = 0;
    entity->drawPos[1].y = 0;
    entity->drawPos[2].x = 0;
    entity->drawPos[2].y = 0;
    entity->visible      = true;
    entity->state        = UITransition_State_Setup;
}

void UITransition_StageLoad(void)
{
    foreach_all(UITransition, transition) { UITransition->activeTransition = (Entity *)transition; }
}

void UITransition_StartTransition(void (*callback)(void), int32 timeLimit)
{
    EntityUITransition *transition = (EntityUITransition *)UITransition->activeTransition;
    if (transition->state == UITransition_State_Setup && !UIDialog->activeDialog) {
        transition->state      = UITransition_State_TransitionIn;
        transition->timer      = 0;
        transition->timeLimit  = timeLimit;
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
    positions[0].x    = screenCenterX + entity->drawPos[0].x - 0xF00000;
    positions[0].y    = screenCenterY + entity->drawPos[0].y;
    positions[1].x    = screenCenterX + entity->drawPos[1].x;
    positions[1].y    = screenCenterY + entity->drawPos[1].y;
    positions[2].x    = screenCenterX + entity->drawPos[2].x + 0xF00000;
    positions[2].y    = screenCenterY + entity->drawPos[2].y;

    for (int32 i = 0; i < 3; ++i) {
        UIWidgets_Unknown7(240, 0, 240, (colours[i] >> 16) & 0xFF, (colours[i] >> 8) & 0xFF, colours[i] & 0xFF, positions[i].x, positions[i].y);
    }
}

void UITransition_State_Setup(void)
{
    RSDK_THIS(UITransition);
    entity->field_6C = false;
    entity->timer    = 0;
}

void UITransition_State_TransitionIn(void)
{
    RSDK_THIS(UITransition);

    if (entity->timer < entity->timeLimit)
        entity->field_6C = false;
    if (entity->timer > entity->timeLimit + 16) {
        entity->drawPos[0].y = 0;
        entity->drawPos[1].x = 0;
        entity->drawPos[1].y = 0;
        entity->drawPos[2].y = 0;
        entity->drawPos[0].x = 0;
        entity->drawPos[2].x = 0;
        entity->timer        = 0;
        entity->state        = UITransition_State_TransitionOut;
    }
    else {
        entity->field_6C = true;
        int32 remain       = entity->timer - entity->timeLimit;
        if (!remain)
            RSDK.PlaySfx(UIWidgets->sfxWoosh, false, 255);

        int32 offsets[3];
        offsets[0] = clampVal(remain, 0, 8);
        offsets[1] = clampVal(remain - 4, 0, 8);
        offsets[2] = clampVal(remain - 8, 0, 8);

        int32 val = 32 * offsets[0];
        if (val > 0) {
            if (val < 256) {
                entity->drawPos[0].x = -0xF000 * val + 0xF00000;
                entity->drawPos[0].y = 0xF000 * val - 0xF00000;
            }
            else {
                entity->drawPos[0].x = 0;
                entity->drawPos[0].y = 0;
            }
        }
        else {
            entity->drawPos[0].x = 0xF00000;
            entity->drawPos[0].y = -0xF00000;
        }

        val = 32 * offsets[1];
        if (val > 0) {
            if (val < 256) {
                entity->drawPos[1].x = 0xF000 * val - 0xF00000;
                entity->drawPos[1].y = -0xF000 * val + 0xF00000;
            }
            else {
                entity->drawPos[1].x = 0;
                entity->drawPos[1].y = 0;
            }
        }
        else {
            entity->drawPos[1].x = -0xF00000;
            entity->drawPos[1].y = 0xF00000;
        }

        val = 32 * offsets[2];
        if (val > 0) {
            if (val < 256) {
                entity->drawPos[2].x = -0xF000 * val + 0xF00000;
                entity->drawPos[2].y = 0xF000 * val - 0xF00000;
            }
            else {
                entity->drawPos[2].x = 0;
                entity->drawPos[2].y = 0;
            }
        }
        else {
            entity->drawPos[2].x = 0xF00000;
            entity->drawPos[2].y = -0xF00000;
        }
        ++entity->timer;
    }
}

void UITransition_State_TransitionOut(void)
{
    RSDK_THIS(UITransition);

    if (entity->timer >= 1) {
        if (entity->timer > 16) {
            entity->timer            = 0;
            entity->field_6C         = false;
            entity->drawPos[0].x     = -0xF00000;
            entity->drawPos[0].y     = 0xF00000;
            entity->drawPos[1].x     = 0xF00000;
            entity->drawPos[1].y     = -0xF00000;
            entity->drawPos[2].x     = -0xF00000;
            entity->drawPos[2].y     = 0xF00000;
            EntityUIControl *control = UIControl_GetUIControl();
            if (control)
                control->selectionDisabled = false;
            entity->state = UITransition_State_Setup;
        }
        else {
            entity->field_6C = true;
            int32 offsets[3];

            offsets[0] = clampVal(entity->timer - 1, 0, 8);
            offsets[1] = clampVal(entity->timer - 4, 0, 8);
            offsets[2] = clampVal(entity->timer - 1, 0, 8);

            int32 val = 32 * offsets[0];
            if (val > 0) {
                if (val < 256) {
                    entity->drawPos[0].x = -0xF000 * val;
                    entity->drawPos[0].y = 0xF000 * val;
                }
                else {
                    entity->drawPos[0].x = -0xF00000;
                    entity->drawPos[0].y = 0xF00000;
                }
            }
            else {
                entity->drawPos[0].x = 0;
                entity->drawPos[0].y = 0;
            }

            val = 32 * offsets[1];
            if (val > 0) {
                if (val < 256) {
                    entity->drawPos[1].x = 0xF000 * val;
                    entity->drawPos[1].y = -0xF000 * val;
                }
                else {
                    entity->drawPos[1].x = 0xF00000;
                    entity->drawPos[1].y = -0xF00000;
                }
            }
            else {
                entity->drawPos[1].x = 0;
                entity->drawPos[1].y = 0;
            }

            val = 32 * offsets[2];
            if (val > 0) {
                if (val < 256) {
                    entity->drawPos[2].x = -0xF000 * val;
                    entity->drawPos[2].y = 0x1E0000 * offsets[2];
                }
                else {
                    entity->drawPos[2].x = -0xF00000;
                    entity->drawPos[2].y = 0xF00000;
                }
            }
            else {
                entity->drawPos[2].x = 0;
                entity->drawPos[2].y = 0;
            }
            ++entity->timer;
        }
    }
    else {
        entity->field_6C                            = true;
        UIControl_GetUIControl()->selectionDisabled = false;
        if (entity->callback && !UIDialog->activeDialog) {
            Entity *storeEntity    = SceneInfo->entity;
            SceneInfo->entity = entity->prevEntity;
            entity->callback();
            entity->callback       = StateMachine_None;
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
        ++entity->timer;
    }
}

#if RETRO_INCLUDE_EDITOR
void UITransition_EditorDraw(void) {}

void UITransition_EditorLoad(void) {}
#endif

void UITransition_Serialize(void) {}
