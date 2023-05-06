// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: E3MenuSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if !MANIA_USE_PLUS
ObjectE3MenuSetup *E3MenuSetup = NULL;

void E3MenuSetup_Update(void)
{
    RSDK_THIS(E3MenuSetup);

    StateMachine_Run(self->state);

    if (self->timer >= self->delay) {
        StateMachine_Run(self->callback);

        destroyEntity(self);
    }
    else {
        self->timer++;
    }
}

void E3MenuSetup_LateUpdate(void) {}

void E3MenuSetup_StaticUpdate(void)
{
    if (!E3MenuSetup->initialized) {
        E3MenuSetup_SetupUI();
        E3MenuSetup->initialized = true;
    }
}

void E3MenuSetup_Draw(void)
{
    RSDK_THIS(E3MenuSetup);

    RSDK.FillScreen(self->fadeColor, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
}

void E3MenuSetup_Create(void *data)
{
    RSDK_THIS(E3MenuSetup);

    self->active    = ACTIVE_NORMAL;
    self->visible   = true;
    self->drawGroup = 14;
}

void E3MenuSetup_StageLoad(void) { RSDK.SetVideoSetting(VIDEOSETTING_SHADERID, 1); }

void E3MenuSetup_SetupUI(void)
{
    String string;
    INIT_STRING(string);

    foreach_all(UIControl, control)
    {
        RSDK.SetString(&string, "Char Select");
        if (RSDK.CompareStrings(&string, &control->tag, false))
            E3MenuSetup->charSelControl = control;

        RSDK.SetString(&string, "Zone");
        if (RSDK.CompareStrings(&string, &control->tag, false))
            E3MenuSetup->zoneControl = control;
    }

    E3MenuSetup_SetupButtons();
}

void E3MenuSetup_SetupButtons(void)
{
    foreach_all(UICharButton, button)
    {
        if (button->parent == (Entity *)E3MenuSetup->charSelControl) {
            switch (button->characterID) {
                case 0: button->actionCB = E3MenuSetup_ActionCB_Sonic; break;
                case 1: button->actionCB = E3MenuSetup_ActionCB_Tails; break;
                case 2: button->actionCB = E3MenuSetup_ActionCB_Knux; break;
            }
        }
    }

    int32 id = 0;
    foreach_all(UITAZoneModule, module)
    {
        module->actionCB = E3MenuSetup_ActionCB_ZoneSelect;

        if (id == 0) { // GHZ2
            module->zoneID = 0;
            module->actID  = 1;
        }
        else if (id == 1) { // MSZ2
            module->zoneID = 7;
            module->actID  = 1;
        }
        ++id;
    }
}

void E3MenuSetup_Callback_LoadScene(void)
{
    EntityMenuParam *param   = MenuParam_GetParam();
    EntityUIControl *control = E3MenuSetup->zoneControl;

    globals->gameMode  = MODE_NOSAVE;
    globals->medalMods = 0;
    RSDK.SetScene("Mania Mode", "");

    EntityUITAZoneModule *module = (EntityUITAZoneModule *)control->buttons[control->buttonID];
    param->zoneID                = module->zoneID;
    param->actID                 = module->actID;

    SceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->characterID, param->actID);
    switch (param->characterID) {
        default: break;
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
    }

    RSDK.LoadScene();
}

void E3MenuSetup_ActionCB_ZoneSelect(void)
{
    EntityE3MenuSetup *entity = CREATE_ENTITY(E3MenuSetup, NULL, 0xFFF0 << 16, 0xFFF0 << 16);

    entity->fadeColor = 0x000000;
    entity->delay     = 32;
    entity->fadeSpeed = 5;
    entity->state     = E3MenuSetup_State_FadeOut;
    entity->callback  = E3MenuSetup_Callback_LoadScene;
}

// Sonic Sel
void E3MenuSetup_ActionCB_Sonic(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();
    param->characterID = 1;

    EntityUIControl *control = E3MenuSetup->zoneControl;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        control->buttons[i]->animator.animationID = 1;
    }

    UIControl_MatchMenuTag("Zones");
}

// Tails Sel
void E3MenuSetup_ActionCB_Tails(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();
    param->characterID = 2;

    EntityUIControl *control = E3MenuSetup->zoneControl;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        control->buttons[i]->animator.animationID = 2;
    }

    UIControl_MatchMenuTag("Zones");
}

// Knux Sel
void E3MenuSetup_ActionCB_Knux(void)
{
    EntityMenuParam *param = MenuParam_GetParam();

    TimeAttackData_Clear();
    param->characterID = 3;

    EntityUIControl *control = E3MenuSetup->zoneControl;
    for (int32 i = 0; i < control->buttonCount; ++i) {
        control->buttons[i]->animator.animationID = 3;
    }

    UIControl_MatchMenuTag("Zones");
}

void E3MenuSetup_State_FadeOut(void)
{
    RSDK_THIS(E3MenuSetup);

    self->fadeTimer = CLAMP(self->timer << (self->fadeSpeed - 1), 0, 0x200);
}

#if GAME_INCLUDE_EDITOR
void E3MenuSetup_EditorDraw(void) {}

void E3MenuSetup_EditorLoad(void) {}
#endif

void E3MenuSetup_Serialize(void) {}
#endif
