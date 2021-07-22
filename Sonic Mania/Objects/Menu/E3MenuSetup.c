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
        /*if (button->parent == E3MenuSetup->charSelControl) {
            int id = button->id;
            if (id == 2) {
                button->selectCB = E3MenuSetup_Unknown7;
            }
            else if (id == 1) {
                button->selectCB = E3MenuSetup_Unknown6;
            }
            else if (!id) {
                button->selectCB = E3MenuSetup_Unknown5;
            }
        }*/
    }

    int id = 0;
    foreach_all(UITAZoneModule, zone)
    {
        // zone->selectCB = E3MenuSetup_ZoneModule_Unknown;
        if (!id) { // GHZ
            // zone->zoneID = 0;
            // zone->actID = 1;
        }
        else if (id == 1) { // MSZ
            // zone->zoneID = 7;
            // zone->actID = 1;
        }
    }
}

void E3MenuSetup_Unknown3(void)
{
    //globals->gameMode  = MODE_NOSAVE;
    globals->medalMods = 0;
    RSDK.LoadScene("Mania Mode", "");
    // EntityUIButton *button      = (EntityUIButton*) * ((_QWORD *)&Obj_E3MenuSetup->zoneControl[2].inBounds +
    // Obj_E3MenuSetup->zoneControl[1].scale.y); globals->menuParam[90] = *(unsigned __int8 *)(button + 332); globals->menuParam[91] = *(unsigned
    // __int8 *)(button + 438);
    int playerID = globals->menuParam[89];
    RSDK_sceneInfo->listPos += TimeAttackData_GetManiaListPos(globals->menuParam[90], playerID, globals->menuParam[91]);
    switch (playerID) {
        default: break;
        case 1: globals->playerID = ID_SONIC; break;
        case 2: globals->playerID = ID_TAILS; break;
        case 3: globals->playerID = ID_SONIC; break;
    }
    RSDK.InitSceneLoad();
}

void E3MenuSetup_Unknown4(void)
{
    EntityE3MenuSetup *entity = (EntityE3MenuSetup *)RSDK.CreateEntity(E3MenuSetup->objectID, NULL, 0xFFF00000, 0xFFF00000);
    entity->fadeColour        = 0x000000;
    entity->timeOut           = 32;
    entity->field_70          = 5;
    entity->state             = E3MenuSetup_Unknown8;
    entity->timedState        = E3MenuSetup_Unknown3;
}

// Sonic Sel
void E3MenuSetup_Unknown5(void)
{
    TimeAttackData_ClearOptions();
    globals->menuParam[89]   = 1;
    EntityUIControl *control = (EntityUIControl *)E3MenuSetup->zoneControl;
    // for (int i = 0; i < LOBYTE(v3[1].isPermanent); ++i) {
    //    *(_BYTE *)(*((_QWORD *)&v3[2].inBounds + v4++) + 437) = 1;
    //}
    UIControl_MatchMenuTag("Zones");
}

// Tails Sel
void E3MenuSetup_Unknown6(void)
{
    TimeAttackData_ClearOptions();
    globals->menuParam[89]   = 2;
    EntityUIControl *control = (EntityUIControl *)E3MenuSetup->zoneControl;
    // for (int i = 0; i < LOBYTE(v3[1].isPermanent); ++i) {
    //    *(_BYTE *)(*((_QWORD *)&v3[2].inBounds + v4++) + 437) = 2;
    //}
    UIControl_MatchMenuTag("Zones");
}

// Knux Sel
void E3MenuSetup_Unknown7(void)
{
    TimeAttackData_ClearOptions();
    globals->menuParam[89]   = 3;
    EntityUIControl *control = (EntityUIControl *)E3MenuSetup->zoneControl;
    // for (int i = 0; i < LOBYTE(v3[1].isPermanent); ++i) {
    //    *(_BYTE *)(*((_QWORD *)&v3[2].inBounds + v4++) + 437) = 3;
    //}
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
