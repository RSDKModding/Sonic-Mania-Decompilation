#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectExtrasMenu *ExtrasMenu;

void ExtrasMenu_Update(void) {}

void ExtrasMenu_LateUpdate(void) {}

void ExtrasMenu_StaticUpdate(void) {}

void ExtrasMenu_Draw(void) {}

void ExtrasMenu_Create(void *data) {}

void ExtrasMenu_StageLoad(void) {}

void ExtrasMenu_Initialize(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    LogHelpers_Print("ManiaModeMenu_Initialize()");

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Extras");
        if (RSDK.StringCompare(&info, &control->tag, false))
            ExtrasMenu->extrasControl = (Entity *)control;
    }
}

void ExtrasMenu_Unknown2(void)
{
    EntityUIControl *control = (EntityUIControl *)ExtrasMenu->extrasControl;

    EntityUIButton *button = control->buttons[0];
    int32 unlock             = GameProgress_CheckUnlock(8);
    button->disabled       = !unlock;
    if (button->disabled)
        UIButton_ManageChoices(button);

    button           = control->buttons[1];
    unlock           = GameProgress_CheckUnlock(6);
    button->disabled = !unlock;
    if (button->disabled)
        UIButton_ManageChoices(button);

    button           = control->buttons[2];
    unlock           = GameProgress_CheckUnlock(7);
    button->disabled = !unlock && !globals->medallionDebug;
}

void ExtrasMenu_Unknown3(void)
{
    EntityUIControl *control      = (EntityUIControl *)ExtrasMenu->extrasControl;
    control->processButtonInputCB = ExtrasMenu_ProcessInputs;

    foreach_all(UIButton, button)
    {
        if (button->listID == 7) {
            if (button->frameID == 8) {
                button->options2  = ExtrasMenu_Callback_Credits;
                button->field_150 = 1;
            }
            if (button->listID == 7 && button->frameID == 4) {
                button->options2  = ExtrasMenu_Callback_DAGarden;
                button->field_150 = 1;
            }
        }
    }

    foreach_all(UIChoice, choice)
    {
        if (choice->listID == 7) {
            if (choice->frameID == 2) {
                choice->options2  = ExtrasMenu_Callback_Puyo_vsAI;
                choice->field_148 = 1;
            }
            if (choice->listID == 7) {
                if (choice->frameID == 3) {
                    choice->options2  = ExtrasMenu_Callback_Puyo_vsAI;
                    choice->field_148 = 1;
                }
                if (choice->listID == 7) {
                    if (choice->frameID == 6) {
                        choice->options2  = ExtrasMenu_Callback_BSS_3K;
                        choice->field_148 = 1;
                    }
                    if (choice->listID == 7 && choice->frameID == 7) {
                        choice->options2  = ExtrasMenu_Callback_BSS_Mania;
                        choice->field_148 = 1;
                    }
                }
            }
        }
    }
}

void ExtrasMenu_ProcessMedallionCheat(void)
{
    int32 key = 0;
    if (UIControl->keyLeft) {
        key = 1;
    }
    else if (UIControl->keyRight) {
        key = 2;
    }

    if (key) {
        for (int32 i = 0; i < 7; ++i) {
            ExtrasMenu->cheatCode[i] = ExtrasMenu->cheatCode[i + 1];
        }
        ExtrasMenu->cheatCode[7] = key;
    }
}

bool32 ExtrasMenu_CheckMedallionCheat(void)
{
    return ExtrasMenu->cheatCode[0] == 1 && ExtrasMenu->cheatCode[1] == 1 && ExtrasMenu->cheatCode[2] == 2 && ExtrasMenu->cheatCode[3] == 2
           && ExtrasMenu->cheatCode[4] == 1 && ExtrasMenu->cheatCode[5] == 2 && ExtrasMenu->cheatCode[6] == 1 && ExtrasMenu->cheatCode[7] == 2;
}

void ExtrasMenu_ProcessInputs(void)
{
    RSDK_THIS(UIControl);
    if (self->activeEntityID == 2) {
        ExtrasMenu_ProcessMedallionCheat();
        if (ExtrasMenu_CheckMedallionCheat()) {
            if (!globals->medallionDebug) {
                RSDK.PlaySfx(UIWidgets->sfxEvent, false, 255);
                globals->medallionDebug = true;
                ExtrasMenu_Unknown2();
            }
        }
    }
    UIControl_ProcessButtonInput();
}

void ExtrasMenu_Start_Puyo_vsAI(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 1;
    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

void ExtrasMenu_Callback_Puyo_vsAI(void)
{
    MenuSetup_StartTransition(ExtrasMenu_Start_Puyo_vsAI, 32);
}

void ExtrasMenu_Start_Puyo_vs2P(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 2;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 1;
    RSDK.SetScene("Extras", "Puyo Puyo");
    RSDK.LoadScene();
}

void ExtrasMenu_Callback_Puyo_vs2P(void) { MenuSetup_StartTransition(ExtrasMenu_Start_Puyo_vs2P, 32); }

void ExtrasMenu_Start_Credits(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 3;
    param->field_1A8 = 1;
    RSDK.SetScene("Presentation", "Credits");
    RSDK.LoadScene();
}

void ExtrasMenu_Callback_Credits(void)
{
    MenuSetup_StartTransition(ExtrasMenu_Start_Credits, 32);
}

void ExtrasMenu_Start_DAGarden(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    strcpy(param->menuTag, "Extras");
    param->selectionID = 2;
#if RETRO_USE_PLUS
    if (API.CheckDLC(DLC_PLUS))
        RSDK.SetScene("Extras", "D.A. Garden Plus");
    else
#endif
        RSDK.SetScene("Extras", "D.A. Garden");
    RSDK.LoadScene();
}

void ExtrasMenu_Callback_DAGarden(void) { MenuSetup_StartTransition(ExtrasMenu_Start_DAGarden, 32); }

void ExtrasMenu_Start_BSS_3K(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 0;
    RSDK.SetScene("Blue Spheres", "Random");
    RSDK.LoadScene();
}

void ExtrasMenu_Callback_BSS_3K(void) { MenuSetup_StartTransition(ExtrasMenu_Start_BSS_3K, 32); }

void ExtrasMenu_Start_BSS_Mania(void)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    TimeAttackData_ClearOptions();
    param->field_59 = 1;
    strcpy(param->menuTag, "Extras");
    param->selectionID = 0;
    RSDK.SetScene("Blue Spheres", "Random 2");
    RSDK.LoadScene();
}

void ExtrasMenu_Callback_BSS_Mania(void) { MenuSetup_StartTransition(ExtrasMenu_Start_BSS_Mania, 32); }

#if RETRO_INCLUDE_EDITOR
void ExtrasMenu_EditorDraw(void) {}

void ExtrasMenu_EditorLoad(void) {}
#endif

void ExtrasMenu_Serialize(void) {}
#endif
