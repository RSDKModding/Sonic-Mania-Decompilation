// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OptionsMenu Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectOptionsMenu *OptionsMenu;

void OptionsMenu_Update(void) {}

void OptionsMenu_LateUpdate(void) {}

void OptionsMenu_StaticUpdate(void) {}

void OptionsMenu_Draw(void) {}

void OptionsMenu_Create(void *data) {}

void OptionsMenu_StageLoad(void) {}

void OptionsMenu_Initialize(void)
{
    LogHelpers_Print("ManiaModeMenu_Initialize()");

    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIControl, control)
    {
        RSDK.PrependText(&info, "Options");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->optionsControl = (Entity *)control;
        RSDK.PrependText(&info, "Language");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->languageControl = (Entity *)control;
        RSDK.PrependText(&info, "Language Old");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->languageControl_Legacy = (Entity *)control;
        RSDK.PrependText(&info, "Video");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->videoControl = (Entity *)control;
        RSDK.PrependText(&info, "Video WIN");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->videoControl_Windows = (Entity *)control;
        RSDK.PrependText(&info, "Sound");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->soundControl = (Entity *)control;
        RSDK.PrependText(&info, "Controls WIN");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_Windows = (Entity *)control;
        RSDK.PrependText(&info, "Controls KB");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_KB = (Entity *)control;
        RSDK.PrependText(&info, "Controls PS4");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_PS4 = (Entity *)control;
        RSDK.PrependText(&info, "Controls XB1");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_XB1 = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_NX = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX Grip");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_NXGrip = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX Joycon");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_NXJoycon = (Entity *)control;
        RSDK.PrependText(&info, "Controls NX Pro");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->controlsControl_NXPro = (Entity *)control;
        RSDK.PrependText(&info, "Data Options");
        if (RSDK.StringCompare(&info, &control->tag, false))
            OptionsMenu->dataOptionsControl = (Entity *)control;
    }

    Hitbox hitbox;
    foreach_all(UIButtonPrompt, prompt)
    {
        EntityUIControl *control = (EntityUIControl *)OptionsMenu->optionsControl;

        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, control->startPos.x - control->cameraOffset.x, control->startPos.y - control->cameraOffset.y,
                                      &hitbox, prompt->position.x, prompt->position.y)
            && prompt->buttonID == 3)
            OptionsMenu->prompt = (Entity *)prompt;
    }

    foreach_all(UIDiorama, diorama)
    {
        EntityUIControl *control = (EntityUIControl *)OptionsMenu->videoControl;

        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, control->startPos.x - control->cameraOffset.x, control->startPos.y - control->cameraOffset.y,
                                      &hitbox, diorama->position.x, diorama->position.y)) {
            OptionsMenu->diorama = (Entity *)diorama;
            diorama->parent      = (Entity *)OptionsMenu->videoControl;
        }
    }

    foreach_all(UIInfoLabel, label)
    {
        EntityUIControl *control = (EntityUIControl *)OptionsMenu->dataOptionsControl;

        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, control->startPos.x - control->cameraOffset.x, control->startPos.y - control->cameraOffset.y,
                                      &hitbox, label->position.x, label->position.y))
            OptionsMenu->label = (Entity *)label;
    }
}

void OptionsMenu_HandleUnlocks(void)
{
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->dataOptionsControl;

    control->buttons[5]   = control->buttons[4];
    control->buttonCount = 6;
    if (!API.CheckDLC(DLC_PLUS)) {
        EntityUIButton *button = control->buttons[2];
        button->invisible      = true;
        button->visible        = false;

        button               = control->buttons[3];
        control->buttons[2] = button;
        button->position.x   = control->buttons[4]->position.x;
        button->size.x       = control->buttons[4]->size.x;
    }
}

void OptionsMenu_SetupActions(void)
{
    EntityUIControl *optionsControl      = (EntityUIControl *)OptionsMenu->optionsControl;
    EntityUIControl *languageControl     = (EntityUIControl *)OptionsMenu->languageControl;
    EntityUIControl *languageControl_old = (EntityUIControl *)OptionsMenu->languageControl_Legacy;
    EntityUIControl *videoControl        = (EntityUIControl *)OptionsMenu->videoControl;
    EntityUIControl *controlsControl_Win = (EntityUIControl *)OptionsMenu->controlsControl_Windows;
    EntityUIControl *videoControl_Win    = (EntityUIControl *)OptionsMenu->videoControl_Windows;
    EntityUIControl *soundControl        = (EntityUIControl *)OptionsMenu->soundControl;
    EntityUIControl *dataControl         = (EntityUIControl *)OptionsMenu->dataOptionsControl;

    Hitbox hitbox;
    TextInfo info;
    INIT_TEXTINFO(info);

    foreach_all(UIButton, button)
    {
        int32 posX      = controlsControl_Win->startPos.x - controlsControl_Win->cameraOffset.x;
        int32 posY      = controlsControl_Win->startPos.y - controlsControl_Win->cameraOffset.y;
        hitbox.right  = controlsControl_Win->size.x >> 17;
        hitbox.left   = -(controlsControl_Win->size.x >> 17);
        hitbox.bottom = controlsControl_Win->size.y >> 17;
        hitbox.top    = -(controlsControl_Win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->actionCB = OptionsMenu_KeyboardIDButton_Win_ActionCB;

        posX = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY = optionsControl->startPos.y - optionsControl->cameraOffset.y;

        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.y >> 17);
        hitbox.bottom = optionsControl->size.x >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 3) {
            button->actionCB = OptionsMenu_LanguageMenuButton_ActionCB;
        }

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && !button->frameID)
            button->actionCB = OptionsMenu_VideoMenuButton_ActionCB;

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 1) {
            button->actionCB = OptionsMenu_SoundMenuButton_ActionCB;
        }

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y)) {
            if (button->listID == 3 && button->frameID == 2) {
                button->actionCB = OptionsMenu_ControlsMenuButton_ActionCB;
                if (sku_platform == PLATFORM_DEV || sku_platform == PLATFORM_PC)
                    button->transition = false;
            }
        }

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 4) {
            button->actionCB = OptionsMenu_DataOptionsMenuButton_ActionCB;
        }

        posX          = languageControl->startPos.x - languageControl->cameraOffset.x;
        posY          = languageControl->startPos.y - languageControl->cameraOffset.y;
        hitbox.right  = languageControl->size.x >> 17;
        hitbox.left   = -(languageControl->size.x >> 17);
        hitbox.bottom = languageControl->size.y >> 17;
        hitbox.top    = -(languageControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->actionCB = OptionsMenu_LanguageButton_ActionCB;

        posX          = languageControl_old->startPos.x - languageControl_old->cameraOffset.x;
        posY          = languageControl_old->startPos.y - languageControl_old->cameraOffset.y;
        hitbox.right  = languageControl_old->size.x >> 17;
        hitbox.left   = -(languageControl_old->size.x >> 17);
        hitbox.bottom = languageControl_old->size.y >> 17;
        hitbox.top    = -(languageControl_old->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->actionCB = OptionsMenu_LanguageButton_ActionCB;

        posX          = videoControl->startPos.x - videoControl->cameraOffset.x;
        posY          = videoControl->startPos.y - videoControl->cameraOffset.y;
        hitbox.right  = videoControl->size.x >> 17;
        hitbox.left   = -(videoControl->size.x >> 17);
        hitbox.bottom = videoControl->size.y >> 17;
        hitbox.top    = -(videoControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && !button->frameID)
            button->choiceChangeCB = OptionsMenu_ShaderButton_ActionCB;

        posX          = controlsControl_Win->startPos.x - controlsControl_Win->cameraOffset.x;
        posY          = controlsControl_Win->startPos.y - controlsControl_Win->cameraOffset.y;
        hitbox.right  = controlsControl_Win->size.x >> 17;
        hitbox.left   = -(controlsControl_Win->size.x >> 17);
        hitbox.bottom = controlsControl_Win->size.y >> 17;
        hitbox.top    = -(controlsControl_Win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17
            && button->frameID == 1) {
            button->actionCB = OptionsMenu_SetDefaultMappings;
        }

        posX          = videoControl_Win->startPos.x - videoControl_Win->cameraOffset.x;
        posY          = videoControl_Win->startPos.y - videoControl_Win->cameraOffset.y;
        hitbox.right  = videoControl_Win->size.x >> 17;
        hitbox.left   = -(videoControl_Win->size.x >> 17);
        hitbox.bottom = videoControl_Win->size.y >> 17;
        hitbox.top    = -(videoControl_Win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17) {
            switch (button->frameID) {
                case 2: button->choiceChangeCB = OptionsMenu_ShaderButton_ActionCB; break;
                case 7: button->choiceChangeCB = OptionsMenu_WindowScaleButton_ActionCB; break;
                case 13: button->choiceChangeCB = OptionsMenu_BorderlessButton_ActionCB; break;
                case 14: button->choiceChangeCB = OptionsMenu_FullScreenButton_ActionCB; break;
                case 15: button->choiceChangeCB = OptionsMenu_VSyncButton_ActionCB; break;
                case 16: button->choiceChangeCB = OptionsMenu_TripleBufferButton_ActionCB; break;
            }
        }

        posX          = dataControl->startPos.x - dataControl->cameraOffset.x;
        posY          = dataControl->startPos.y - dataControl->cameraOffset.y;
        hitbox.right  = dataControl->size.x >> 17;
        hitbox.left   = -(dataControl->size.x >> 17);
        hitbox.bottom = dataControl->size.y >> 17;
        hitbox.top    = -(dataControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 18) {
            switch (button->frameID) {
                case 0: button->actionCB = OptionsMenu_EraseSaveGameButton_ActionCB; break;
                case 1: button->actionCB = OptionsMenu_EraseMedallionsButton_ActionCB; break;
                case 2: button->actionCB = OptionsMenu_EraseTimeAttackButton_ActionCB; break;
                case 3: button->actionCB = OptionsMenu_EraseReplaysButton_ActionCB; break;
                case 4: button->actionCB = OptionsMenu_EraseAllButton_ActionCB; break;
            }
        }
    }

    foreach_all(UISlider, slider)
    {
        hitbox.right  = soundControl->size.x >> 17;
        hitbox.left   = -(soundControl->size.x >> 17);
        hitbox.bottom = soundControl->size.y >> 17;
        hitbox.top    = -(soundControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, soundControl->startPos.x - soundControl->cameraOffset.x,
                                      soundControl->startPos.y - soundControl->cameraOffset.y, &hitbox, slider->position.x, slider->position.y)
            && slider->listID == 5)
            slider->sliderChangedCB = OptionsMenu_UISlider_ChangedCB;
    }

    optionsControl->menuSetupCB = OptionsMenu_MenuSetupCB;

    EntityUIInfoLabel *label = (EntityUIInfoLabel *)OptionsMenu->label;
    Localization_GetString(&info, STR_SELECTDATATOERASE);
    UIInfoLabel_SetString(label, &info);

    videoControl_Win->menuUpdateCB     = OptionsMenu_VideoControl_Win_MenuUpdateCB;
    videoControl_Win->yPressCB         = OptionsMenu_VideoControl_Win_YPressCB;
    videoControl_Win->backPressCB      = OptionsMenu_VideoControl_Win_BackPressCB;

    if (sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV) {
        optionsControl->yPressCB = OptionsMenu_ShowManual;
    }
    else {
        EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)OptionsMenu->prompt;
        prompt->visible              = false;
    }
}

void OptionsMenu_HandleMenuReturn(void)
{
    EntityUIControl *languageControl     = (EntityUIControl *)OptionsMenu->languageControl;
    EntityUIControl *languageControl_old = (EntityUIControl *)OptionsMenu->languageControl_Legacy;
    EntityUIControl *videoControl        = (EntityUIControl *)OptionsMenu->videoControl;
    EntityUIControl *soundControl        = (EntityUIControl *)OptionsMenu->soundControl;

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        OptionsMenu_InitVideoOptionsMenu();

    EntityUIButton *button = videoControl->buttons[0];
    UIButton_SetChoiceSelection(button, RSDK.GetSettingsValue(SETTINGS_SHADERID));

    EntityUISlider *slider = (EntityUISlider *)soundControl->buttons[0];
    slider->sliderPos      = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);

    slider = (EntityUISlider *)soundControl->buttons[1];
    slider->sliderPos     = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);

    languageControl->startingID     = Localization->language;
    languageControl->buttonID = Localization->language;

    languageControl_old->startingID     = Localization->language;
    languageControl_old->buttonID = Localization->language;
}
void OptionsMenu_InitVideoOptionsMenu(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        EntityUIControl *videoControl_Win = (EntityUIControl *)OptionsMenu->videoControl_Windows;
        EntityOptions *options            = (EntityOptions *)globals->optionsRAM;
        Options_GetWinSize();

        int32 vals[7];

        vals[0] = RSDK.GetSettingsValue(SETTINGS_SHADERID);
        vals[1] = options->windowSize;
        vals[2] = RSDK.GetSettingsValue(SETTINGS_BORDERED);

        vals[3] = 0;
        if (!RSDK.GetSettingsValue(SETTINGS_WINDOWED) || options->windowSize)
            vals[3] = 1;

        vals[4] = 0;
        vals[5] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
        vals[6] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);

        for (int32 i = 0; i < videoControl_Win->buttonCount; ++i) {
            EntityUIButton *button = videoControl_Win->buttons[i];

            if (i == 4) {
                EntityUIResPicker *child = (EntityUIResPicker*)UIButton_GetChoicePtr(button, button->selection);
                UIResPicker_GetDisplayInfo(child);
            }
            else if (i == 1) {
                EntityUIWinSize *child = (EntityUIWinSize *)UIButton_GetChoicePtr(button, button->selection);
                child->selection       = RSDK.GetSettingsValue(SETTINGS_WINDOW_HEIGHT) / SCREEN_YSIZE;
            }
            else if (button->selection != vals[i]) {
                UIButton_SetChoiceSelection(button, vals[i]);
            }
        }
    }
}

void OptionsMenu_VideoControl_Win_MenuUpdateCB(void) { OptionsMenu_InitVideoOptionsMenu(); }

void OptionsMenu_VideoControl_Win_YPressCB(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    if (RSDK.GetSettingsValue(SETTINGS_CHANGED)) {
        RSDK.UpdateWindow();
        Localization_GetString(&info, STR_VIDEOCHANGESAPPLIED);
        UIDialog_CreateDialogYesNo(&info, OptionsMenu_ApplyChangesDlg_Win_YesCB, OptionsMenu_ApplyChangesDlg_Win_NoCB, true, true);
    }
}
void OptionsMenu_DlgRunnerCB_RevertVideoChanges(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    RSDK.UpdateWindow();
    Localization_GetString(&info, STR_VIDEOCHANGESAPPLIED);
    UIDialog_CreateDialogYesNo(&info, OptionsMenu_ApplyChangesDlg_YesCB, OptionsMenu_ApplyChangesDlg_NoCB, true, true);
}

bool32 OptionsMenu_VideoControl_Win_BackPressCB(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    if (RSDK.GetSettingsValue(SETTINGS_CHANGED)) {
        Localization_GetString(&info, STR_APPLYCHANGEDSETTINGS);
        UIDialog_CreateDialogYesNo(&info, OptionsMenu_ApplyChangesDlg_YesCB, OptionsMenu_ApplyChangesDlg_BackPress_NoCB, true, true);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) 
            return true;
    }
    else {
        UITransition_StartTransition(UIControl_ReturnToParentMenu, 0);
    }
    return false;
}

void OptionsMenu_ApplyChangesDlg_BackPress_NoCB(void)
{
    EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_HandleCallback, 0, 0);
    dialogRunner->callback           = OptionsMenu_DlgRunnerCB_RevertVideoChanges;
    dialogRunner->timer              = 0;
    dialogRunner->isPermanent        = true;
}

void OptionsMenu_ApplyChangesDlg_Win_NoCB(void)
{
    RSDK.SetSettingsValue(SETTINGS_RELOAD, false);
    RSDK.UpdateWindow();
    OptionsMenu_InitVideoOptionsMenu();
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
}

void OptionsMenu_ApplyChangesDlg_NoCB(void)
{
    RSDK.SetSettingsValue(SETTINGS_RELOAD, false);
    RSDK.UpdateWindow();
    OptionsMenu_InitVideoOptionsMenu();
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    UITransition_StartTransition(UIControl_ReturnToParentMenu, 0);
}

void OptionsMenu_ApplyChangesDlg_Win_YesCB(void)
{
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    RSDK.SetSettingsValue(SETTINGS_WRITE, true);
    RSDK.SetSettingsValue(SETTINGS_STORE, false);
}

void OptionsMenu_ApplyChangesDlg_YesCB(void)
{
    RSDK.SetSettingsValue(SETTINGS_RELOAD, false);
    OptionsMenu_InitVideoOptionsMenu();
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    UITransition_StartTransition(UIControl_ReturnToParentMenu, 0);
}

void OptionsMenu_VideoMenuButton_ActionCB(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        RSDK.SetSettingsValue(SETTINGS_STORE, 0);
        UIControl_MatchMenuTag("Video WIN");
    }
    else {
        UIControl_MatchMenuTag("Video");
    }
}
void OptionsMenu_SoundMenuButton_ActionCB(void) { UIControl_MatchMenuTag("Sound"); }
void OptionsMenu_LanguageMenuButton_ActionCB(void) { UIControl_MatchMenuTag("Language"); }
void OptionsMenu_DataOptionsMenuButton_ActionCB(void) { UIControl_MatchMenuTag("Data Options"); }
void OptionsMenu_ControlsMenuButton_DefaultKB_ActionCB(void) { UIControl_MatchMenuTag("Controls WIN"); }
void OptionsMenu_ControlsMenuButton_Default_ActionCB(void) { UIControl_MatchMenuTag("Controls XB1"); }

void OptionsMenu_ControlsMenuButton_ActionCB(void)
{
    int32 id   = RSDK.MostRecentActiveControllerID(1, 0, 5);
    int32 gamepadType = RSDK.GetControllerType(id);

    TextInfo info;
    INIT_TEXTINFO(info);

    switch ((gamepadType >> 8) & 0xFF) {
        default:
            switch (gamepadType & 0xFF) {
                case DEVICE_XBOX: UITransition_SetNewTag("Controls XB1"); break;
                case DEVICE_PS4: UITransition_SetNewTag("Controls PS4"); break;
                case DEVICE_SATURN: UITransition_SetNewTag("Controls WIN"); break;
                case DEVICE_SWITCH_HANDHELD: UITransition_SetNewTag("Controls NX"); break;
                case DEVICE_SWITCH_JOY_GRIP: UITransition_SetNewTag("Controls NX Grip"); break;
                case DEVICE_SWITCH_JOY_L:
                case DEVICE_SWITCH_JOY_R: UITransition_SetNewTag("Controls NX Joycon"); break;
                case DEVICE_SWITCH_PRO: UITransition_SetNewTag("Controls NX Pro"); break;
                default: UITransition_StartTransition(OptionsMenu_ControlsMenuButton_Default_ActionCB, 0); break;
            }
            break;
        case DEVICE_TYPE_KEYBOARD: UITransition_StartTransition(OptionsMenu_ControlsMenuButton_DefaultKB_ActionCB, 0); break;
        case DEVICE_TYPE_STEAMOVERLAY:
            if (!API.IsOverlayEnabled(id)) {
                Localization_GetString(&info, STR_STEAMOVERLAYUNAVALIABLE);
                UIDialog_CreateDialogOk(&info, StateMachine_None, true);
            }
            break;
    }
}

void OptionsMenu_SetDefaultMappings(void)
{
    ControllerInfo[1].keyUp.keyMap     = 38;
    ControllerInfo[1].keyDown.keyMap   = 40;
    ControllerInfo[1].keyLeft.keyMap   = 37;
    ControllerInfo[1].keyRight.keyMap  = 39;
    ControllerInfo[1].keyA.keyMap      = 65;
    ControllerInfo[1].keyB.keyMap      = 83;
    ControllerInfo[1].keyC.keyMap      = 0;
    ControllerInfo[1].keyX.keyMap      = 81;
    ControllerInfo[1].keyY.keyMap      = 87;
    ControllerInfo[1].keyZ.keyMap      = 0;
    ControllerInfo[1].keyStart.keyMap  = 13;
    ControllerInfo[1].keySelect.keyMap = 9;
    ControllerInfo[2].keyUp.keyMap     = 104;
    ControllerInfo[2].keyDown.keyMap   = 101;
    ControllerInfo[2].keyLeft.keyMap   = 100;
    ControllerInfo[2].keyRight.keyMap  = 102;
    ControllerInfo[2].keyA.keyMap      = 74;
    ControllerInfo[2].keyB.keyMap      = 75;
    ControllerInfo[2].keyC.keyMap      = 0;
    ControllerInfo[2].keyX.keyMap      = 85;
    ControllerInfo[2].keyY.keyMap      = 73;
    ControllerInfo[2].keyZ.keyMap      = 0;
    ControllerInfo[2].keyStart.keyMap  = 219;
    ControllerInfo[2].keySelect.keyMap = 221;
    RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
}

void OptionsMenu_SetupKBControlsMenu(int32 playerID)
{
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->controlsControl_KB;
    foreach_all(UISubHeading, subHeading)
    {
        Hitbox hitbox;
        hitbox.right  = control->size.x >> 17;
        hitbox.left   = -(control->size.x >> 17);
        hitbox.bottom = control->size.y >> 17;
        hitbox.top    = -(control->size.y >> 17);

        if (MathHelpers_PointInHitbox(FLIP_NONE, control->startPos.x - control->cameraOffset.x, control->startPos.y - control->cameraOffset.y,
                                      &hitbox, subHeading->position.x, subHeading->position.y)) {
            subHeading->frameID = playerID + 19;
            foreach_break;
        }
    }

    for (int32 i = 0; i < control->buttonCount; ++i) {
        EntityUIKeyBinder *binder = (EntityUIKeyBinder *)control->buttons[i];
        if (binder->objectID == UIKeyBinder->objectID)
           binder->inputID = playerID;
    }
}

void OptionsMenu_KeyboardIDButton_Win_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->controlsControl_Windows;

    for (int32 i = 0; i < control->buttonCount; ++i) {
        if (self == control->buttons[i]) {
            OptionsMenu_SetupKBControlsMenu(i);
            UIControl_MatchMenuTag("Controls KB");
        }
    }
}

void OptionsMenu_MenuSetupCB(void)
{
    if (Options->changed) {
        UIWaitSpinner_StartWait();
        Options_SaveOptionsBin(OptionsMenu_SaveOptionsCB_Load);
    }

    EntityUIControl *control = (EntityUIControl *)OptionsMenu->optionsControl;
    EntityUIButton *eraseDataButton   = control->buttons[4];
    if (eraseDataButton) {
        eraseDataButton->disabled = checkNoSave;
    }
}

void OptionsMenu_SaveOptionsCB_Load(bool32 success) { UIWaitSpinner_FinishWait(); }

void OptionsMenu_TransitionCB_ReloadScene(void)
{
    globals->suppressAutoMusic = true;
    RSDK.SetHardResetFlag(true);
    RSDK.LoadScene();
}

void OptionsMenu_SaveOptionsCB_Action(bool32 success)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    UIWaitSpinner_FinishWait();
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->languageControl;
    control->startingID      = control->buttonID;

    TimeAttackData_Clear();
    strcpy(param->menuTag, "Options");
    MenuSetup_StartTransition(OptionsMenu_TransitionCB_ReloadScene, 32);
}

void OptionsMenu_LanguageButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)self->parent;
    Options_SetLanguage(control->buttonID);
    Localization->language     = control->buttonID;
    control->selectionDisabled = true;
    UIWaitSpinner_StartWait();
    Options_SaveOptionsBin(OptionsMenu_SaveOptionsCB_Action);
}

void OptionsMenu_ShaderButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options  = (EntityOptions *)globals->optionsRAM;
    options->screenShader = self->selection;
    options->overrideShader       = true;
    RSDK.SetSettingsValue(SETTINGS_SHADERID, self->selection);
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    Options->changed = true;
}

void OptionsMenu_WindowScaleButton_ActionCB(void)
{
    RSDK_THIS(UIButton);

    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    if (self->selection != 4) {
        RSDK.SetSettingsValue(SETTINGS_WINDOW_WIDTH, WIDE_SCR_XSIZE * (self->selection + 1));
        RSDK.SetSettingsValue(SETTINGS_WINDOW_HEIGHT, SCREEN_YSIZE * (self->selection + 1));

        options->windowSize = self->selection;
        Options->changed    = true;
    }
}

void OptionsMenu_BorderlessButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options  = (EntityOptions *)globals->optionsRAM;

    options->windowBorder = self->selection;
    RSDK.SetSettingsValue(SETTINGS_BORDERED, self->selection);
    Options->changed = true;
}

void OptionsMenu_FullScreenButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->windowed = self->selection ^ 1;
    RSDK.SetSettingsValue(SETTINGS_WINDOWED, self->selection ^ 1);
    Options->changed = true;
}

void OptionsMenu_VSyncButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->vSync = self->selection;
    RSDK.SetSettingsValue(SETTINGS_VSYNC, self->selection);
    Options->changed = true;
}

void OptionsMenu_TripleBufferButton_ActionCB(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->tripleBuffering = self->selection;
    RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, self->selection);
    Options->changed = true;
}

void OptionsMenu_UISlider_ChangedCB(void)
{
    RSDK_THIS(UISlider);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    // what the hell is up with this???????
    // it'd only ever be 0 or 1 why are F1,F2,F4,F5 & FC options?????
    // this is a CB for the slider why are the boolean values here???
    bool32 value = self->frameID != 1;
    switch (value) {
        case 0xF1:
            options->windowed = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_WINDOWED, options->windowed);
            break;
        case 0xF2:
            options->windowBorder = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_BORDERED, options->windowBorder);
            break;
        case 0xF4:
            options->vSync = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_VSYNC, options->vSync);
            break;
        case 0xF5:
            options->tripleBuffering = self->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, options->tripleBuffering);
            break;
        case 0xFC:
            options->screenShader   = self->sliderPos;
            options->overrideShader = true;
            RSDK.SetSettingsValue(SETTINGS_SHADERID, options->screenShader);
            RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
            break;
        case 0:
            options->volMusic         = self->sliderPos;
            options->overrideMusicVol = true;
            RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, options->volMusic);
            break;
        case 1:
            options->volSfx         = self->sliderPos;
            options->overrideSfxVol = true;
            RSDK.SetSettingsValue(SETTINGS_SFX_VOL, options->volSfx);
            break;
        default: break;
    }
    Options->changed = true;
}

void OptionsMenu_ShowManual(void)
{
    RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);
    API.LaunchManual();
}

void OptionsMenu_EraseSaveDataCB(int32 status)
{
    TextInfo message;
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->dataOptionsControl;

    UIWaitSpinner_FinishWait();
    if (status) {
        ManiaModeMenu_StartReturnToTitle();
    }
    else {
        control->selectionDisabled = false;
        RSDK.SetText(&message, "ERROR ERASING DATA.", 0);
        UIDialog_CreateDialogOk(&message, StateMachine_None, true);
    }
}

void OptionsMenu_AreYouSureDlg_YesCB_EraseSaveGame(void)
{
    // Bug Details:
    // memset should be 0x400 * sizeof(int32), not 0x400
    // difference is 0x400 int32s vs 0x400 uint8s

    for (int32 i = 0; i < 8; ++i) {
        int32 *saveRAM = SaveGame_GetDataPtr(i, false);
        memset(saveRAM, 0, 0x400);
    }

    for (int32 i = 0; i < 3; ++i) {
        int32 *saveRAM = SaveGame_GetDataPtr(i, true);
        memset(saveRAM, 0, 0x400);
    }

    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues = 0;
    if (!checkNoSave && SaveGame->saveRAM && globals->saveLoaded == STATUS_OK) {
        SaveGame->saveEntityPtr = SceneInfo->entity;
        SaveGame->saveCallback  = OptionsMenu_EraseSaveDataCB;
        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
    }
    else {
        OptionsMenu_EraseSaveDataCB(false);
    }
}

void OptionsMenu_AreYouSureDlg_YesCB_EraseAllData(void)
{
    for (int32 i = 0; i < 8; ++i) {
        int32 *saveRAM = SaveGame_GetDataPtr(i, false);
        memset(saveRAM, 0, 0x400);
    }

    for (int32 i = 0; i < 3; ++i) {
        int32 *saveRAM = SaveGame_GetDataPtr(i, true);
        memset(saveRAM, 0, 0x400);
    }
    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues = 0;
    GameProgress_ClearProgress();
    API.RemoveAllDBRows(globals->taTableID);
    if (!checkNoSave && SaveGame->saveRAM && globals->saveLoaded == STATUS_OK) {
        SaveGame->saveEntityPtr = SceneInfo->entity;
        SaveGame->saveCallback  = OptionsMenu_EraseSaveDataCB;
        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
    }
    else {
        OptionsMenu_EraseSaveDataCB(false);
    }
}

void OptionsMenu_EraseSaveGameButton_ActionCB(void)
{
    TextInfo message;
    Localization_GetString(&message, STR_AREYOUSURESAVE);
    UIDialog_CreateDialogYesNo(&message, OptionsMenu_AreYouSureDlg_YesCB_EraseSaveGame, StateMachine_None, true, true);
}

void OptionsMenu_AreYouSureDlg_YesCB_EraseMedallions(void)
{
    EntityUIControl *control   = (EntityUIControl *)OptionsMenu->dataOptionsControl;
    control->selectionDisabled = true;
    GameProgress_ClearBSSSave();
    SaveGame_SaveFile(OptionsMenu_EraseSaveDataCB);
}

void OptionsMenu_EraseMedallionsButton_ActionCB(void)
{
    TextInfo message;
    INIT_TEXTINFO(message);

    Localization_GetString(&message, STR_AREYOUSURESAVE);
    UIDialog_CreateDialogYesNo(&message, OptionsMenu_AreYouSureDlg_YesCB_EraseMedallions, StateMachine_None, true, true);
}

void OptionsMenu_AreYouSureDlg_YesCB_EraseTimeAttack(void)
{
    EntityUIControl *control   = (EntityUIControl *)OptionsMenu->dataOptionsControl;
    control->selectionDisabled = true;
    UIWaitSpinner_StartWait();
    API.RemoveAllDBRows(globals->taTableID);
    TimeAttackData_SaveTimeAttackDB(OptionsMenu_EraseSaveDataCB);
    LogHelpers_Print("TimeAttack table ID = %d, status = %d", globals->taTableID, globals->taTableLoaded);
}

void OptionsMenu_EraseTimeAttackButton_ActionCB(void)
{
    TextInfo message;
    INIT_TEXTINFO(message);

    Localization_GetString(&message, STR_AREYOUSURESAVE);
    UIDialog_CreateDialogYesNo(&message, OptionsMenu_AreYouSureDlg_YesCB_EraseTimeAttack, StateMachine_None, true, true);
}

void OptionsMenu_AreYouSureDlg_YesCB_EraseReplays(void)
{
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->dataOptionsControl;

    if (!control->selectionDisabled) {
        control->selectionDisabled = true;
        UIWaitSpinner_StartWait();
    }
    API.SetupUserDBRowSorting(globals->replayTableID);
    API.SetupUserDBRowSorting(globals->taTableID);
    if (API.GetSortedUserDBRowCount(globals->replayTableID) <= 0) {
        ReplayRecorder_SaveReplayDB(OptionsMenu_EraseReplaysCB);
    }
    else {
        int32 row = API.GetSortedUserDBRowID(globals->replayTableID, 0);
        ReplayRecorder_DeleteReplay(row, OptionsMenu_EraseReplaysCB, true);
    }
}

void OptionsMenu_EraseReplaysCB(bool32 success) { TimeAttackData_SaveTimeAttackDB(OptionsMenu_EraseSaveDataCB); }

void OptionsMenu_EraseReplaysButton_ActionCB(void)
{
    TextInfo message;
    INIT_TEXTINFO(message);

    if (API.CheckDLC(DLC_PLUS)) {
        Localization_GetString(&message, STR_AREYOUSURESAVE);
        UIDialog_CreateDialogYesNo(&message, OptionsMenu_AreYouSureDlg_YesCB_EraseReplays, StateMachine_None, true, true);
    }
    else {
        Localization_GetString(&message, STR_ENCOREREQUIRED);
        UIDialog_CreateDialogOk(&message, StateMachine_None, true);
    }
}

void OptionsMenu_EraseAllButton_ActionCB(void)
{
    TextInfo message;
    INIT_TEXTINFO(message);

    Localization_GetString(&message, STR_AREYOUSURESAVE);
    UIDialog_CreateDialogYesNo(&message, OptionsMenu_AreYouSureDlg_YesCB_EraseAllData, StateMachine_None, true, true);
}

#if RETRO_INCLUDE_EDITOR
void OptionsMenu_EditorDraw(void) {}

void OptionsMenu_EditorLoad(void) {}
#endif

void OptionsMenu_Serialize(void) {}
#endif
