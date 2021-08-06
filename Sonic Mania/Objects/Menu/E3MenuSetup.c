#include "SonicMania.h"

#if !RETRO_USE_PLUS
ObjectE3MenuSetup *E3MenuSetup = NULL;

void E3MenuSetup_Update(void)
{
    RSDK_THIS(E3MenuSetup);
    StateMachine_Run(entity->state);
    if (entity->timer >= entity->timeOut) {
        StateMachine_Run(entity->timedState);
        destroyEntity(entity);
    }
    else {
        entity->timer++;
    }
}

void E3MenuSetup_LateUpdate(void) {}

void E3MenuSetup_StaticUpdate(void)
{
    if (!E3MenuSetup->flag) {
        E3MenuSetup_Unknown1();
        E3MenuSetup->flag = true;
    }
}

void E3MenuSetup_Draw(void)
{
    RSDK_THIS(E3MenuSetup);
    RSDK.FillScreen(entity->fadeColour, entity->fadeTimer, entity->fadeTimer - 128, entity->fadeTimer - 256);
}

void E3MenuSetup_Create(void *data)
{
    RSDK_THIS(E3MenuSetup);
    entity->active    = ACTIVE_NORMAL;
    entity->inkEffect = INK_BLEND;
    entity->direction = 14;
}

void E3MenuSetup_StageLoad(void) { RSDK.SetSettingsValue(SETTINGS_SHADERID, 1); }

void E3MenuSetup_Unknown1(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);
    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Char Select");
        if (RSDK.StringCompare(&info, &control->tag, false))
            E3MenuSetup->charSelControl = (Entity *)control;

        RSDK.PrependText(&info, "Zone");
        if (RSDK.StringCompare(&info, &control->tag, false))
            E3MenuSetup->zoneControl = (Entity *)control;
    }
    E3MenuSetup_Unknown2();
}

void E3MenuSetup_Unknown2(void)
{
    foreach_all(UICharButton, button)
    {
        if (button->parent == E3MenuSetup->charSelControl) {
            switch (button->characterID) {
                case 0: button->options2 = E3MenuSetup_Unknown5; break;
                case 1: button->options2 = E3MenuSetup_Unknown6; break;
                case 2: button->options2 = E3MenuSetup_Unknown7; break;
            }
        }
    }

    int id = 0;
    foreach_all(UITAZoneModule, module)
    {
        module->options2 = E3MenuSetup_ZoneModule_Unknown;
        if (!id) { // GHZ
            module->zoneID = 0;
            zone->actID = 1;
        }
        else if (id == 1) { // MSZ
            module->zoneID = 7;
            zone->actID = 1;
        }
    }
}

void E3MenuSetup_Unknown3(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    EntityUIControl *control = (EntityUIControl *)E3MenuSetup->zoneControl;
    globals->gameMode  = MODE_NOSAVE;
    globals->medalMods = 0;
    RSDK.LoadScene("Mania Mode", "");
    EntityUITAZoneModule *module = (EntityUITAZoneModule *)control->buttons[control->selectionID];
    param->zoneID                = module->zoneID;
    param->actID                 = module->actID;
    RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(param->zoneID, param->playerID, param->actID);
    switch (param->characterID) {
        default: break;
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_KNUCKLES; break;
    }
    RSDK.InitSceneLoad();
}

void E3MenuSetup_Unknown4(void)
{
    EntityE3MenuSetup *entity = CREATE_ENTITY(E3MenuSetup, NULL, 0xFFF00000, 0xFFF00000);
    entity->fadeColour        = 0x000000;
    entity->timeOut           = 32;
    entity->field_70          = 5;
    entity->state             = E3MenuSetup_Unknown8;
    entity->timedState        = E3MenuSetup_Unknown3;
}

// Sonic Sel
void E3MenuSetup_Unknown5(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->characterID = 1;
    EntityUIControl *control        = (EntityUIControl *)E3MenuSetup->zoneControl;
    for (int i = 0; i < control->buttonCount; ++i) {
        control->buttons[i]->animator.animationID = 1;
    }
    UIControl_MatchMenuTag("Zones");
}

// Tails Sel
void E3MenuSetup_Unknown6(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->characterID = 2;
    EntityUIControl *control        = (EntityUIControl *)E3MenuSetup->zoneControl;
    for (int i = 0; i < control->buttonCount; ++i) {
        control->buttons[i]->animator.animationID = 2;
    }
    UIControl_MatchMenuTag("Zones");
}

// Knux Sel
void E3MenuSetup_Unknown7(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->characterID = 3;
    EntityUIControl *control        = (EntityUIControl *)E3MenuSetup->zoneControl;
    for (int i = 0; i < control->buttonCount; ++i) {
        control->buttons[i]->animator.animationID = 3;
    }
    UIControl_MatchMenuTag("Zones");
}

void E3MenuSetup_Unknown8(void)
{
    RSDK_THIS(E3MenuSetup);
    entity->fadeTimer = clampVal(entity->timer << (entity->field_70 - 1), 0, 0x200);
}

void E3MenuSetup_EditorDraw(void) {}

void E3MenuSetup_EditorLoad(void) {}

void E3MenuSetup_Serialize(void) {}
#endif
