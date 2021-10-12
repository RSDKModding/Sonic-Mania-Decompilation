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

void OptionsMenu_Unknown2(void)
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

void OptionsMenu_Unknown3(void)
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
        int posX      = controlsControl_Win->startPos.x - controlsControl_Win->cameraOffset.x;
        int posY      = controlsControl_Win->startPos.y - controlsControl_Win->cameraOffset.y;
        hitbox.right  = controlsControl_Win->size.x >> 17;
        hitbox.left   = -(controlsControl_Win->size.x >> 17);
        hitbox.bottom = controlsControl_Win->size.y >> 17;
        hitbox.top    = -(controlsControl_Win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->options2 = OptionsMenu_Unknown24;

        posX = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY = optionsControl->startPos.y - optionsControl->cameraOffset.y;

        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.y >> 17);
        hitbox.bottom = optionsControl->size.x >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 3) {
            button->options2 = OptionsMenu_Unknown17;
        }

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && !button->frameID)
            button->options2 = OptionsMenu_Unknown15;

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && button->frameID == 1) {
            button->options2 = OptionsMenu_Unknown16;
        }

        posX          = optionsControl->startPos.x - optionsControl->cameraOffset.x;
        posY          = optionsControl->startPos.y - optionsControl->cameraOffset.y;
        hitbox.right  = optionsControl->size.x >> 17;
        hitbox.left   = -(optionsControl->size.x >> 17);
        hitbox.bottom = optionsControl->size.y >> 17;
        hitbox.top    = -(optionsControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y)) {
            if (button->listID == 3 && button->frameID == 2) {
                button->options2 = OptionsMenu_Unknown21;
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
            button->options2 = OptionsMenu_Unknown18;
        }

        posX          = languageControl->startPos.x - languageControl->cameraOffset.x;
        posY          = languageControl->startPos.y - languageControl->cameraOffset.y;
        hitbox.right  = languageControl->size.x >> 17;
        hitbox.left   = -(languageControl->size.x >> 17);
        hitbox.bottom = languageControl->size.y >> 17;
        hitbox.top    = -(languageControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->options2 = OptionsMenu_Unknown30;

        posX          = languageControl_old->startPos.x - languageControl_old->cameraOffset.x;
        posY          = languageControl_old->startPos.y - languageControl_old->cameraOffset.y;
        hitbox.right  = languageControl_old->size.x >> 17;
        hitbox.left   = -(languageControl_old->size.x >> 17);
        hitbox.bottom = languageControl_old->size.y >> 17;
        hitbox.top    = -(languageControl_old->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y))
            button->options2 = OptionsMenu_Unknown30;

        posX          = videoControl->startPos.x - videoControl->cameraOffset.x;
        posY          = videoControl->startPos.y - videoControl->cameraOffset.y;
        hitbox.right  = videoControl->size.x >> 17;
        hitbox.left   = -(videoControl->size.x >> 17);
        hitbox.bottom = videoControl->size.y >> 17;
        hitbox.top    = -(videoControl->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 3
            && !button->frameID)
            button->choiceChangeCB = OptionsMenu_Unknown31;

        posX          = controlsControl_Win->startPos.x - controlsControl_Win->cameraOffset.x;
        posY          = controlsControl_Win->startPos.y - controlsControl_Win->cameraOffset.y;
        hitbox.right  = controlsControl_Win->size.x >> 17;
        hitbox.left   = -(controlsControl_Win->size.x >> 17);
        hitbox.bottom = controlsControl_Win->size.y >> 17;
        hitbox.top    = -(controlsControl_Win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17
            && button->frameID == 1) {
            button->options2 = OptionsMenu_Unknown22;
        }

        posX          = videoControl_Win->startPos.x - videoControl_Win->cameraOffset.x;
        posY          = videoControl_Win->startPos.y - videoControl_Win->cameraOffset.y;
        hitbox.right  = videoControl_Win->size.x >> 17;
        hitbox.left   = -(videoControl_Win->size.x >> 17);
        hitbox.bottom = videoControl_Win->size.y >> 17;
        hitbox.top    = -(videoControl_Win->size.y >> 17);
        if (MathHelpers_PointInHitbox(FLIP_NONE, posX, posY, &hitbox, button->position.x, button->position.y) && button->listID == 17) {
            switch (button->frameID) {
                case 2: button->choiceChangeCB = OptionsMenu_Unknown31; break;
                case 7: button->choiceChangeCB = OptionsMenu_Unknown32; break;
                case 13: button->choiceChangeCB = OptionsMenu_Unknown33; break;
                case 14: button->choiceChangeCB = OptionsMenu_Unknown34; break;
                case 15: button->choiceChangeCB = OptionsMenu_Unknown35; break;
                case 16: button->choiceChangeCB = OptionsMenu_Unknown36; break;
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
                case 0: button->options2 = OptionsMenu_Unknown39; break;
                case 1: button->options2 = OptionsMenu_Unknown41; break;
                case 2: button->options2 = OptionsMenu_Unknown43; break;
                case 3: button->options2 = OptionsMenu_Unknown46; break;
                case 4: button->options2 = OptionsMenu_EraseAll_Confirm; break;
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

    optionsControl->unknownCallback3 = OptionsMenu_Unknown25;

    EntityUIInfoLabel *label = (EntityUIInfoLabel *)OptionsMenu->label;
    Localization_GetString(&info, STR_SELECTDATATOERASE);
    UIInfoLabel_SetString(label, &info);

    videoControl_Win->unknownCallback4 = OptionsMenu_Unknown6;
    videoControl_Win->yPressCB         = OptionsMenu_Unknown7;
    videoControl_Win->backPressCB      = OptionsMenu_Unknown9;

    if (sku_platform == PLATFORM_SWITCH || sku_platform == PLATFORM_DEV) {
        optionsControl->yPressCB = OptionsMenu_ShowManual;
    }
    else {
        EntityUIButtonPrompt *prompt = (EntityUIButtonPrompt *)OptionsMenu->prompt;
        prompt->visible              = false;
    }
}

void OptionsMenu_Unknown4(void)
{
    EntityUIControl *languageControl     = (EntityUIControl *)OptionsMenu->languageControl;
    EntityUIControl *languageControl_old = (EntityUIControl *)OptionsMenu->languageControl_Legacy;
    EntityUIControl *videoControl        = (EntityUIControl *)OptionsMenu->videoControl;
    EntityUIControl *soundControl        = (EntityUIControl *)OptionsMenu->soundControl;

    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV)
        OptionsMenu_Unknown5();

    EntityUIButton *button = videoControl->buttons[0];
    UIButton_SetChoiceSelection(button, RSDK.GetSettingsValue(SETTINGS_SHADERID));

    EntityUISlider *slider = (EntityUISlider *)soundControl->buttons[0];
    slider->sliderPos      = RSDK.GetSettingsValue(SETTINGS_STREAM_VOL);

    slider = (EntityUISlider *)soundControl->buttons[1];
    slider->sliderPos     = RSDK.GetSettingsValue(SETTINGS_SFX_VOL);

    languageControl->startingID     = Localization->language;
    languageControl->activeEntityID = Localization->language;

    languageControl_old->startingID     = Localization->language;
    languageControl_old->activeEntityID = Localization->language;
}
void OptionsMenu_Unknown5(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        EntityUIControl *videoControl_Win = (EntityUIControl *)OptionsMenu->videoControl_Windows;
        EntityOptions *options            = (EntityOptions *)globals->optionsRAM;
        Options_GetWinSize();

        int vals[7];

        vals[0] = RSDK.GetSettingsValue(SETTINGS_SHADERID);
        vals[1] = options->windowSize;
        vals[2] = RSDK.GetSettingsValue(SETTINGS_BORDERED);

        vals[3] = 0;
        if (!RSDK.GetSettingsValue(SETTINGS_WINDOWED) || options->windowSize)
            vals[3] = 1;

        vals[4] = 0;
        vals[5] = RSDK.GetSettingsValue(SETTINGS_VSYNC);
        vals[6] = RSDK.GetSettingsValue(SETTINGS_TRIPLEBUFFERED);

        for (int i = 0; i < videoControl_Win->buttonCount; ++i) {
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

void OptionsMenu_Unknown6(void) { OptionsMenu_Unknown5(); }

void OptionsMenu_Unknown7(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    if (RSDK.GetSettingsValue(SETTINGS_CHANGED)) {
        RSDK.UpdateWindow();
        Localization_GetString(&info, STR_VIDEOCHANGESAPPLIED);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) {
            dialog->timer = 900;
            UIDialog_AddButton(DIALOG_NO, dialog, OptionsMenu_Unknown11, true);
            UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_Unknown13, true);
            UIDialog_Setup(dialog);
        }
    }
}
void OptionsMenu_Unknown8(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    RSDK.UpdateWindow();
    Localization_GetString(&info, STR_VIDEOCHANGESAPPLIED);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        dialog->timer = 900;
        UIDialog_AddButton(DIALOG_NO, dialog, OptionsMenu_Unknown12, true);
        UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_Unknown14, true);
        UIDialog_Setup(dialog);
    }
}

bool32 OptionsMenu_Unknown9(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    if (RSDK.GetSettingsValue(SETTINGS_CHANGED)) {
        Localization_GetString(&info, STR_APPLYCHANGEDSETTINGS);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) {
            UIDialog_AddButton(DIALOG_NO, dialog, OptionsMenu_Unknown14, true);
            UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_Unknown10, true);
            UIDialog_Setup(dialog);
            return true;
        }
    }
    else {
        UITransition_StartTransition(UIControl_Unknown13, 0);
    }
    return false;
}

void OptionsMenu_Unknown10(void)
{
    EntityDialogRunner *dialogRunner = CREATE_ENTITY(DialogRunner, DialogRunner_HandleCallback, 0, 0);
    dialogRunner->callback           = OptionsMenu_Unknown8;
    dialogRunner->timer              = 0;
    dialogRunner->isPermanent        = true;
}

void OptionsMenu_Unknown11(void)
{
    RSDK.SetSettingsValue(SETTINGS_RELOAD, 0);
    RSDK.UpdateWindow();
    OptionsMenu_Unknown5();
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
}

void OptionsMenu_Unknown12(void)
{
    RSDK.SetSettingsValue(SETTINGS_RELOAD, 0);
    RSDK.UpdateWindow();
    OptionsMenu_Unknown5();
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    UITransition_StartTransition(UIControl_Unknown13, 0);
}

void OptionsMenu_Unknown13(void)
{
    RSDK.SetSettingsValue(SETTINGS_CHANGED, 0);
    // RSDK.SetSettingsValue(SETTINGS_SETTINGSCHANGED, 1);
    RSDK.SetSettingsValue(SETTINGS_STORE, 0);
}

void OptionsMenu_Unknown14(void)
{
    RSDK.SetSettingsValue(SETTINGS_RELOAD, 0);
    OptionsMenu_Unknown5();
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    UITransition_StartTransition(UIControl_Unknown13, 0);
}

void OptionsMenu_Unknown15(void)
{
    if (sku_platform == PLATFORM_PC || sku_platform == PLATFORM_DEV) {
        RSDK.SetSettingsValue(SETTINGS_STORE, 0);
        UIControl_MatchMenuTag("Video WIN");
    }
    else {
        UIControl_MatchMenuTag("Video");
    }
}
void OptionsMenu_Unknown16(void) { UIControl_MatchMenuTag("Sound"); }
void OptionsMenu_Unknown17(void) { UIControl_MatchMenuTag("Language"); }
void OptionsMenu_Unknown18(void) { UIControl_MatchMenuTag("Data Options"); }
void OptionsMenu_Unknown19(void) { UIControl_MatchMenuTag("Controls WIN"); }
void OptionsMenu_Unknown20(void) { UIControl_MatchMenuTag("Controls XB1"); }

void OptionsMenu_Unknown21(void)
{
    int id   = RSDK.MostRecentActiveControllerID(1, 0, 5);
    int type = RSDK.GetControllerType(id);

    TextInfo info;
    INIT_TEXTINFO(info);

    if ((type & 0xFF00) == 256) {
        UITransition_StartTransition(OptionsMenu_Unknown19, 0);
    }
    else if ((type & 0xFF00) == 1024) {
        if (!API.Unknown4(id)) {
            Localization_GetString(&info, STR_STEAMOVERLAYUNAVALIABLE);
            EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
            if (dialog) {
                UIDialog_AddButton(DIALOG_OK, dialog, NULL, true);
                UIDialog_Setup(dialog);
            }
        }
    }
    else {
        switch (type) {
            case 1: UITransition_SetNewTag("Controls XB1"); break;
            case 2: UITransition_SetNewTag("Controls PS4"); break;
            case 3: UITransition_SetNewTag("Controls WIN"); break;
            case 4: UITransition_SetNewTag("Controls NX"); break;
            case 5: UITransition_SetNewTag("Controls NX Grip"); break;
            case 6:
            case 7: UITransition_SetNewTag("Controls NX Joycon"); break;
            case 8: UITransition_SetNewTag("Controls NX Pro"); break;
            default: UITransition_StartTransition(OptionsMenu_Unknown20, 0); break;
        }
    }
}

void OptionsMenu_Unknown22(void)
{
    RSDK_controller[1].keyUp.keyMap     = 38;
    RSDK_controller[1].keyDown.keyMap   = 40;
    RSDK_controller[1].keyLeft.keyMap   = 37;
    RSDK_controller[1].keyRight.keyMap  = 39;
    RSDK_controller[1].keyA.keyMap      = 65;
    RSDK_controller[1].keyB.keyMap      = 83;
    RSDK_controller[1].keyC.keyMap      = 0;
    RSDK_controller[1].keyX.keyMap      = 81;
    RSDK_controller[1].keyY.keyMap      = 87;
    RSDK_controller[1].keyZ.keyMap      = 0;
    RSDK_controller[1].keyStart.keyMap  = 13;
    RSDK_controller[1].keySelect.keyMap = 9;
    RSDK_controller[2].keyUp.keyMap     = 104;
    RSDK_controller[2].keyDown.keyMap   = 101;
    RSDK_controller[2].keyLeft.keyMap   = 100;
    RSDK_controller[2].keyRight.keyMap  = 102;
    RSDK_controller[2].keyA.keyMap      = 74;
    RSDK_controller[2].keyB.keyMap      = 75;
    RSDK_controller[2].keyC.keyMap      = 0;
    RSDK_controller[2].keyX.keyMap      = 85;
    RSDK_controller[2].keyY.keyMap      = 73;
    RSDK_controller[2].keyZ.keyMap      = 0;
    RSDK_controller[2].keyStart.keyMap  = 219;
    RSDK_controller[2].keySelect.keyMap = 221;
    RSDK.SetSettingsValue(SETTINGS_CHANGED, true);
}

void OptionsMenu_Unknown23(int id)
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
            subHeading->frameID = id + 19;
            foreach_break;
        }
    }

    for (int i = 0; i < control->buttonCount; ++i) {
        EntityUIKeyBinder *binder = (EntityUIKeyBinder *)control->buttons[i];
        if (binder->objectID == UIKeyBinder->objectID)
           binder->inputID = id;
    }
}

void OptionsMenu_Unknown24(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->controlsControl_Windows;

    for (int i = 0; i < control->buttonCount; ++i) {
        if (entity == control->buttons[i]) {
            OptionsMenu_Unknown23(i);
            UIControl_MatchMenuTag("Controls KB");
        }
    }
}

void OptionsMenu_Unknown25(void)
{
    if (Options->state) {
        UIWaitSpinner_Wait();
        Options_SaveOptionsBin(OptionsMenu_Unknown27);
    }

    EntityUIControl *control = (EntityUIControl *)OptionsMenu->optionsControl;
    EntityUIButton *button   = control->buttons[4];
    if (button) {
        button->disabled = API.GetUserStorageNoSave();
    }
}

void OptionsMenu_Unknown27(int status) { UIWaitSpinner_Wait2(); }

void OptionsMenu_Unknown28(void)
{
    globals->suppressAutoMusic = true;
    RSDK.SetHardResetFlag(true);
    RSDK.LoadScene();
}

void OptionsMenu_Unknown29(int status)
{
    EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
    UIWaitSpinner_Wait2();
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->languageControl;
    control->startingID      = control->activeEntityID;

    TimeAttackData_ClearOptions();
    strcpy(param->menuTag, "Options");
    MenuSetup_StartTransition(OptionsMenu_Unknown28, 32);
}

void OptionsMenu_Unknown30(void)
{
    RSDK_THIS(UIButton);
    EntityUIControl *control = (EntityUIControl *)entity->parent;
    Options_SetLanguage(control->activeEntityID);
    Localization->language     = control->activeEntityID;
    control->selectionDisabled = true;
    UIWaitSpinner_Wait();
    Options_SaveOptionsBin(OptionsMenu_Unknown29);
}

void OptionsMenu_Unknown31(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options  = (EntityOptions *)globals->optionsRAM;
    options->screenShader = entity->selection;
    options->field_60 = true;
    RSDK.SetSettingsValue(SETTINGS_SHADERID, entity->selection);
    RSDK.SetSettingsValue(SETTINGS_CHANGED, false);
    Options->state = 1;
}

void OptionsMenu_Unknown32(void)
{
    RSDK_THIS(UIButton);

    EntityOptions *options = (EntityOptions *)globals->optionsRAM;
    if (entity->selection != 4) {
        RSDK.SetSettingsValue(SETTINGS_WINDOW_WIDTH, 424 * (entity->selection + 1));
        RSDK.SetSettingsValue(SETTINGS_WINDOW_HEIGHT, SCREEN_YSIZE * (entity->selection + 1));

        options->windowSize = entity->selection;
        Options->state   = 1;
    }
}

void OptionsMenu_Unknown33(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options  = (EntityOptions *)globals->optionsRAM;

    options->windowBorder = entity->selection;
    RSDK.SetSettingsValue(SETTINGS_BORDERED, entity->selection);
    Options->state = 1;
}

void OptionsMenu_Unknown34(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->windowed = entity->selection ^ 1;
    RSDK.SetSettingsValue(SETTINGS_WINDOWED, entity->selection ^ 1);
    Options->state = 1;
}

void OptionsMenu_Unknown35(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->vSync = entity->selection;
    RSDK.SetSettingsValue(SETTINGS_VSYNC, entity->selection);
    Options->state = 1;
}

void OptionsMenu_Unknown36(void)
{
    RSDK_THIS(UIButton);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    options->tripleBuffering = entity->selection;
    RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, entity->selection);
    Options->state = 1;
}

void OptionsMenu_UISlider_ChangedCB(void)
{
    RSDK_THIS(UISlider);
    EntityOptions *options = (EntityOptions *)globals->optionsRAM;

    // what the hell is up with this???????
    // it'd only ever be 0 or 1 why are F1,F2,F4,F5 & FC options?????
    // this is a CB for the slider why are the boolean values here???
    bool32 value = entity->frameID != 1;
    switch (value) {
        case 0xF1:
            options->windowed = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_WINDOWED, options->windowed);
            Options->state = 1;
            break;
        case 0xF2:
            options->windowBorder = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_BORDERED, options->windowBorder);
            Options->state = 1;
            break;
        case 0xF4:
            options->vSync = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_VSYNC, options->vSync);
            Options->state = 1;
            break;
        case 0xF5:
            options->tripleBuffering = entity->sliderPos;
            RSDK.SetSettingsValue(SETTINGS_TRIPLEBUFFERED, options->tripleBuffering);
            Options->state = 1;
            break;
        case 0xFC:
            options->screenShader = entity->sliderPos;
            options->field_60   = true;
            RSDK.SetSettingsValue(SETTINGS_SHADERID, options->screenShader);
            RSDK.SetSettingsValue(SETTINGS_CHANGED, 0);
            Options->state = 1;
            break;
        case 0:
            options->volMusic = entity->sliderPos;
            options->field_68   = 1;
            RSDK.SetSettingsValue(SETTINGS_STREAM_VOL, options->volMusic);
            Options->state = 1;
            break;
        case 1:
            options->volSfx   = entity->sliderPos;
            options->field_70   = true;
            RSDK.SetSettingsValue(SETTINGS_SFX_VOL, options->volSfx);
            Options->state = 1;
            break;
        default: Options->state = 1; break;
    }
}

void OptionsMenu_ShowManual(void)
{
    RSDK.PlaySfx(UIWidgets->sfx_Accept, false, 255);
    API.LaunchManual();
}

void OptionsMenu_EraseSaveDataCB(int status)
{
    TextInfo info;
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->dataOptionsControl;

    UIWaitSpinner_Wait2();
    if (status) {
        ManiaModeMenu_StartReturnToTitle();
    }
    else {
        control->selectionDisabled = false;
        RSDK.SetText(&info, "ERROR ERASING DATA.", 0);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) {
            UIDialog_AddButton(2, dialog, NULL, true);
            UIDialog_Setup(dialog);
        }
    }
}

void OptionsMenu_EraseAllSaveData(void)
{
    for (int i = 0; i < 8; ++i) {
        int *saveRAM = SaveGame_GetDataPtr(i, false);
        memset(saveRAM, 0, 0x400);
    }

    for (int i = 0; i < 3; ++i) {
        int *saveRAM = SaveGame_GetDataPtr(i, true);
        memset(saveRAM, 0, 0x400);
    }

    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues = 0;
    if (!checkNoSave && SaveGame->saveRAM && globals->saveLoaded == STATUS_OK) {
        SaveGame->saveEntityPtr = RSDK_sceneInfo->entity;
        SaveGame->saveCallback  = OptionsMenu_EraseSaveDataCB;
        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
    }
    else {
        OptionsMenu_EraseSaveDataCB(false);
    }
}

void OptionsMenu_EraseAllData(void)
{
    for (int i = 0; i < 8; ++i) {
        int *saveRAM = SaveGame_GetDataPtr(i, false);
        memset(saveRAM, 0, 0x400);
    }

    for (int i = 0; i < 3; ++i) {
        int *saveRAM = SaveGame_GetDataPtr(i, true);
        memset(saveRAM, 0, 0x400);
    }
    memset(globals->noSaveSlot, 0, 0x400);
    globals->continues = 0;
    GameProgress_ClearProgress();
    API.RemoveAllDBRows(globals->taTableID);
    if (!checkNoSave && SaveGame->saveRAM && globals->saveLoaded == STATUS_OK) {
        SaveGame->saveEntityPtr = RSDK_sceneInfo->entity;
        SaveGame->saveCallback  = OptionsMenu_EraseSaveDataCB;
        API_SaveUserFile("SaveData.bin", globals->saveRAM, 0x10000, SaveGame_SaveFile_CB, false);
    }
    else {
        OptionsMenu_EraseSaveDataCB(false);
    }
}

void OptionsMenu_Unknown39(void)
{
    TextInfo info;
    Localization_GetString(&info, STR_AREYOUSURESAVE);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, true);
        UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_EraseAllSaveData, true);
        UIDialog_Setup(dialog);
    }
}

void OptionsMenu_Unknown40(void)
{
    EntityUIControl *control   = (EntityUIControl *)OptionsMenu->dataOptionsControl;
    control->selectionDisabled = true;
    GameProgress_ClearBSSSave();
    SaveGame_SaveFile(OptionsMenu_EraseSaveDataCB);
}

void OptionsMenu_Unknown41(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_AREYOUSURESAVE);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, 0, true);
        UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_Unknown40, true);
        UIDialog_Setup(dialog);
    }
}

void OptionsMenu_Unknown42(void)
{
    EntityUIControl *control   = (EntityUIControl *)OptionsMenu->dataOptionsControl;
    control->selectionDisabled = true;
    UIWaitSpinner_Wait();
    API.RemoveAllDBRows(globals->taTableID);
    TimeAttackData_SaveTimeAttackDB(OptionsMenu_EraseSaveDataCB);
    LogHelpers_Print("TimeAttack table ID = %d, status = %d", globals->taTableID, globals->taTableLoaded);
}

void OptionsMenu_Unknown43(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_AREYOUSURESAVE);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, true);
        UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_Unknown42, true);
        UIDialog_Setup(dialog);
    }
}

void OptionsMenu_Unknown44(void)
{
    EntityUIControl *control = (EntityUIControl *)OptionsMenu->dataOptionsControl;

    if (!control->selectionDisabled) {
        control->selectionDisabled = true;
        UIWaitSpinner_Wait();
    }
    API.SetupSortedUserDBRowIDs(globals->replayTableID);
    API.SetupSortedUserDBRowIDs(globals->taTableID);
    if (API.GetSortedUserDBRowCount(globals->replayTableID) <= 0) {
        ReplayRecorder_SaveReplayDB(OptionsMenu_Unknown45);
    }
    else {
        int id = API.GetSortedUserDBRowID(globals->replayTableID, 0);
        ReplayRecorder_DeleteTimeAttackRow(id, OptionsMenu_Unknown45, 1);
    }
}

void OptionsMenu_Unknown45(bool32 status) { TimeAttackData_SaveTimeAttackDB(OptionsMenu_EraseSaveDataCB); }

void OptionsMenu_Unknown46(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    if (API.CheckDLC(DLC_PLUS)) {
        Localization_GetString(&info, STR_AREYOUSURESAVE);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) {
            UIDialog_AddButton(DIALOG_NO, dialog, NULL, true);
            UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_Unknown44, true);
            UIDialog_Setup(dialog);
        }
    }
    else {
        Localization_GetString(&info, STR_ENCOREREQUIRED);
        EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
        if (dialog) {
            UIDialog_AddButton(DIALOG_OK, dialog, NULL, true);
            UIDialog_Setup(dialog);
        }
    }
}

void OptionsMenu_EraseAll_Confirm(void)
{
    TextInfo info;
    INIT_TEXTINFO(info);

    Localization_GetString(&info, STR_AREYOUSURESAVE);
    EntityUIDialog *dialog = UIDialog_CreateActiveDialog(&info);
    if (dialog) {
        UIDialog_AddButton(DIALOG_NO, dialog, NULL, true);
        UIDialog_AddButton(DIALOG_YES, dialog, OptionsMenu_EraseAllData, true);
        UIDialog_Setup(dialog);
    }
}

void OptionsMenu_EditorDraw(void) {}

void OptionsMenu_EditorLoad(void) {}

void OptionsMenu_Serialize(void) {}
#endif
