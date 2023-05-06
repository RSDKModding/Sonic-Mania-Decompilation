// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: MainMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectMainMenu *MainMenu;

void MainMenu_Update(void) {}

void MainMenu_LateUpdate(void) {}

void MainMenu_StaticUpdate(void)
{
    EntityUIControl *control = MainMenu->menuControl;

    if (control && control->active) {
        EntityUIDiorama *diorama         = MainMenu->diorama;
        MainMenu->confirmPrompt->visible = ((int32)(control->shifter->shiftOffset.y & 0xFFFF0000)) > -0x700000;
        EntityUIButton *button           = control->buttons[control->lastButtonID];

        if (button) {
            switch (button->frameID) {
                case 0: diorama->dioramaID = UIDIORAMA_MANIAMODE; break;
                case 1: diorama->dioramaID = UIDIORAMA_TIMEATTACK; break;
                case 2: diorama->dioramaID = UIDIORAMA_COMPETITION; break;
                case 3: diorama->dioramaID = UIDIORAMA_OPTIONS; break;
                case 4: diorama->dioramaID = UIDIORAMA_EXTRAS; break;
                case 5: diorama->dioramaID = UIDIORAMA_ENCOREMODE; break;
                case 6: diorama->dioramaID = UIDIORAMA_PLUSUPSELL; break;
                case 7: diorama->dioramaID = UIDIORAMA_EXIT; break;
                default: break;
            }

            if (button->disabled)
                diorama->timer = 12;
        }
    }
}

void MainMenu_Draw(void) {}

void MainMenu_Create(void *data) {}

void MainMenu_StageLoad(void) {}

void MainMenu_Initialize(void)
{
    String tag;
    INIT_STRING(tag);
    RSDK.SetString(&tag, "Main Menu");

    foreach_all(UIControl, control)
    {
        if (RSDK.CompareStrings(&tag, &control->tag, false)) {
            MainMenu->menuControl = control;
            control->backPressCB  = MainMenu_BackPressCB_ReturnToTitle;
        }
    }

    EntityUIControl *menuControl = MainMenu->menuControl;

    foreach_all(UIButtonPrompt, prompt)
    {
        if (UIControl_ContainsPos(menuControl, &prompt->position) && prompt->buttonID == 0)
            MainMenu->confirmPrompt = prompt;
    }

    foreach_all(UIDiorama, diorama)
    {
        if (UIControl_ContainsPos(menuControl, &diorama->position)) {
            MainMenu->diorama = diorama;
            diorama->parent   = menuControl;
        }
    }

#if MANIA_USE_PLUS
    int32 button1Frame = 1; // Time Attack
    int32 button2Frame = 2; // Competition
    int32 button3Frame = 3; // Options
    int32 button4Frame = 4; // Extras
    int32 button5Frame = 6; // Buy Plus

    bool32 button3StopMus    = false; // Options button does NOT stop music
    bool32 button5Transition = false; // Buy Plus Does NOT do a transition

    if (API.CheckDLC(DLC_PLUS)) {
        button1Frame = 5; // Encore Mode
        button2Frame = 1; // Time Attack
        button3Frame = 2; // Competition
        button4Frame = 3; // Options
        button5Frame = 4; // Extras

        button3StopMus    = true; // Competition button stops music
        button5Transition = true; // Extras Does a transition
    }

    EntityUIButton *buttonManiaMode = menuControl->buttons[0];
    buttonManiaMode->frameID        = 0;
    buttonManiaMode->transition     = true;
    buttonManiaMode->stopMusic      = true;

    EntityUIButton *buttonEncoreMode = menuControl->buttons[1];
    buttonEncoreMode->frameID        = button1Frame;
    buttonEncoreMode->transition     = true;
    buttonEncoreMode->stopMusic      = true;

    EntityUIButton *buttonTimeAttack = menuControl->buttons[2];
    buttonTimeAttack->frameID        = button2Frame;
    buttonTimeAttack->transition     = true;
    buttonTimeAttack->stopMusic      = true;

    EntityUIButton *buttonCompetition = menuControl->buttons[3];
    buttonCompetition->frameID        = button3Frame;
    buttonCompetition->transition     = true;
    buttonCompetition->stopMusic      = button3StopMus;

    EntityUIButton *buttonOptions = menuControl->buttons[4];
    buttonOptions->frameID        = button4Frame;
    buttonOptions->transition     = true;
    buttonOptions->stopMusic      = false;

    EntityUIButton *buttonExtras = menuControl->buttons[5];
    buttonExtras->frameID        = button5Frame;
    buttonExtras->transition     = button5Transition;
    buttonExtras->stopMusic      = false;

    EntityUIButton *buttonExit = menuControl->buttons[6];
    buttonExit->frameID        = 7;
    buttonExit->transition     = false;
    buttonExit->stopMusic      = false;
#endif
}

bool32 MainMenu_BackPressCB_ReturnToTitle(void)
{
    ManiaModeMenu_StartReturnToTitle();

    return true;
}

void MainMenu_ExitGame(void) { API.ExitGame(); }

void MainMenu_ExitButton_ActionCB(void)
{
    String msg;
    Localization_GetString(&msg, STR_QUITWARNING);

    UIDialog_CreateDialogYesNo(&msg, MainMenu_StartExitGame, StateMachine_None, true, true);
}

void MainMenu_StartExitGame(void)
{
    if (UIControl_GetUIControl())
        UIControl_GetUIControl()->state = StateMachine_None;

    Music_FadeOut(0.02);

    MenuSetup_StartTransition(MainMenu_ExitGame, 64);
}

void MainMenu_MenuButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    switch (self->frameID) {
        case 0: // Mania Mode
            if (API_GetNoSave()) {
                UIControl_MatchMenuTag("No Save Mode");
            }
            else {
                EntityUIControl *saveSelect = ManiaModeMenu->saveSelectMenu;
                saveSelect->buttonID        = 7;
#if MANIA_USE_PLUS
                saveSelect->menuWasSetup           = false;
                ManiaModeMenu->saveSelLastButtonID = -1;

                for (int32 i = 0; i < saveSelect->buttonCount; ++i) {
                    Entity *store     = SceneInfo->entity;
                    SceneInfo->entity = (Entity *)saveSelect->buttons[i];
                    UISaveSlot_HandleSaveIconChange();
                    SceneInfo->entity = store;
                }
#endif
                UIControl_MatchMenuTag("Save Select");
            }
            break;

        case 1: // Time Attack
            if (API.CheckDLC(DLC_PLUS)) {
                EntityUIControl *control = TimeAttackMenu->timeAttackControl;
                control->buttonID        = 0;
                control->menuWasSetup    = false;
                UIControl_MatchMenuTag("Time Attack");
            }
            else {
                EntityUIControl *control = TimeAttackMenu->timeAttackControl_Legacy;
                control->buttonID        = 0;
                control->menuWasSetup    = false;
                UIControl_MatchMenuTag("Time Attack Legacy");
            }
            break;

        case 2: // Competition
            if (API.CheckDLC(DLC_PLUS))
                UIControl_MatchMenuTag("Competition");
            else
                UIControl_MatchMenuTag("Competition Legacy");
            break;

        case 3: // Options
            UIControl_MatchMenuTag("Options");
            break;

        case 4: // Extras
            UIControl_MatchMenuTag("Extras");
            break;

        case 5: // Encore
            if (API_GetNoSave()) {
                UIControl_MatchMenuTag("No Save Encore");
            }
            else {
                EntityUIControl *encoreSaveSel = ManiaModeMenu->encoreSaveSelect;
                encoreSaveSel->buttonID        = 1;
                encoreSaveSel->menuWasSetup    = false;
                for (int32 i = 0; i < encoreSaveSel->buttonCount; ++i) {
                    Entity *store     = SceneInfo->entity;
                    SceneInfo->entity = (Entity *)encoreSaveSel->buttons[i];
                    UISaveSlot_HandleSaveIconChange();
                    SceneInfo->entity = store;
                }
                UIControl_MatchMenuTag("Encore Mode");
            }
            break;

        case 6: // Buy Plus DLC
#if MANIA_USE_EGS
            API.SetupExtensionOverlay();

            if (!API.CheckDLC(DLC_PLUS)) {
                if (!API.CanShowExtensionOverlay(0) || !API.ShowExtensionOverlay(0)) {
                    if (API.CanShowAltExtensionOverlay(0)) {
                        int32 connectingMessage = API.GetConnectingStringID();
                        if (connectingMessage >= 0) {
                            String message;
                            INIT_STRING(message);
                            Localization_GetString(&message, connectingMessage);

                            UIDialog_CreateDialogOkCancel(&message, MainMenu_BuyPlusDialogCB, StateMachine_None, true, true);
                        }
                        else {
                            API.ShowAltExtensionOverlay(0);
                        }
                    }
                }
            }
#else
            API.ShowExtensionOverlay(0);
#endif
            break;

        default: break;
    }
}

#if MANIA_USE_EGS
void MainMenu_BuyPlusDialogCB(void) { API.ShowAltExtensionOverlay(0); }
#endif

void MainMenu_HandleUnlocks(void)
{
    EntityUIControl *control = MainMenu->menuControl;

    EntityUIButton *taButton = API.CheckDLC(DLC_PLUS) ? control->buttons[2] : control->buttons[1];
    taButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_TIMEATTACK);

    EntityUIButton *compButton = API.CheckDLC(DLC_PLUS) ? control->buttons[3] : control->buttons[2];
    compButton->disabled       = !GameProgress_CheckUnlock(GAMEPROGRESS_UNLOCK_COMPETITION);
}

void MainMenu_SetupActions(void)
{
    foreach_all(UIButton, button)
    {
        if (button->listID == 1) {
            if (button->frameID == 7) {
                if (sku_platform != PLATFORM_PC && sku_platform != PLATFORM_DEV) {
                    EntityUIControl *control = MainMenu->menuControl;

                    // Remove "Exit" Button if not on PC or DEV platform
                    destroyEntity(button);
                    --control->buttonCount;
                    --control->rowCount;
                    control->buttons[6] = NULL;
                }
                else {
                    button->actionCB = MainMenu_ExitButton_ActionCB;
                }
            }
            else {
                button->actionCB = MainMenu_MenuButton_ActionCB;
            }
        }
    }

    MainMenu->menuControl->menuSetupCB = MainMenu_MenuSetupCB;
}

void MainMenu_MenuSetupCB(void) { MainMenu->diorama->lastDioramaID = -1; }

#if GAME_INCLUDE_EDITOR
void MainMenu_EditorDraw(void) {}

void MainMenu_EditorLoad(void) {}
#endif

void MainMenu_Serialize(void) {}
#endif
