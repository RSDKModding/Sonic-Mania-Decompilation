
// NOTE: I'm using enums & structs from the libNX homebrew lib, but they should have the same values as the official APIs

using namespace RSDK::SKU;

HidNpadIdType npadTypes[5] = { HidNpadIdType_Handheld, HidNpadIdType_No1, HidNpadIdType_No2, HidNpadIdType_No3, HidNpadIdType_No4 };
int32 nxVibrateDeviceCount = 0;

int32 lastNPadType       = -1;
int32 activeNPadTypes[5] = { -1, -1, -1, -1, -1 };

uint8 inactiveNPadCount = 0;
uint8 nPadIDs[]         = { 2, 3, 4, 5, 6, 7, 8, 9 };
uint8 storedNPadIDs[]   = { 0, 0, 0, 0, 0 };

int32 RSDK::SKU::currentNXControllerType = (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_HANDHELD << 0);

void UpdateInputNX(InputDeviceNX *device, int64 *buttonMasks, int32 *hDelta, int32 *vDelta)
{
    int32 prevButtonMasks = device->buttonMasks;
    device->buttonMasks   = *buttonMasks;

    device->hDelta_L = hDelta[0] + 128.0;
    device->vDelta_L = vDelta[0] + 128.0;

    float div        = sqrtf((device->hDelta_L * device->hDelta_L) + (device->vDelta_L * device->vDelta_L));
    device->hDelta_L = device->hDelta_L / div;
    device->vDelta_L = device->vDelta_L / div;
    if (div <= 15728.0) {
        device->hDelta_L = 0.0;
        device->vDelta_L = 0.0;
    }
    else {
        device->hDelta_L = device->hDelta_L * ((fminf(div, 127.0) + -15728.0) / 17039.0);
        device->vDelta_L = device->vDelta_L * ((fminf(div, 127.0) + -15728.0) / 17039.0);
    }

    device->hDelta_R = hDelta[1] + 128.0;
    device->vDelta_R = vDelta[1] + 128.0;

    div        = sqrtf((device->hDelta_R * device->hDelta_R) + (device->vDelta_R * device->vDelta_R));
    device->hDelta_R = device->hDelta_R / div;
    device->vDelta_R = device->vDelta_R / div;
    if (div <= 15728.0) {
        device->hDelta_R = 0.0;
        device->vDelta_R = 0.0;
    }
    else {
        device->hDelta_R = device->hDelta_R * ((fminf(div, 127.0) + -15728.0) / 17039.0);
        device->vDelta_R = device->vDelta_R * ((fminf(div, 127.0) + -15728.0) / 17039.0);
    }

    int32 changedButtons = device->buttonMasks & ~prevButtonMasks;

    if (changedButtons) {
        currentNXControllerType  = device->gamePadType;
        device->inactiveTimer[0] = 0;
    }
    else {
        ++device->inactiveTimer[0];
    }

    if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
        device->inactiveTimer[1] = 0;
    else
        ++device->inactiveTimer[1];

    device->anyPress = changedButtons;

    device->ProcessInput(CONT_ANY);
}

void InputDeviceNXHandheld::UpdateInput()
{
    int32 vDelta[2];
    int32 hDelta[2];
    HidNpadHandheldState state;

    hidGetNpadStatesHandheld(this->npadType, &state, 1);
    hDelta[0] = -state.analog_stick_l.x;
    hDelta[1] = -state.analog_stick_r.x;
    vDelta[0] = -state.analog_stick_l.y;
    vDelta[1] = -state.analog_stick_r.y;

    UpdateInputNX(this, &state.buttons, hDelta, vDelta);
}

void InputDeviceNXJoyL::UpdateInput()
{
    int32 vDelta[2];
    int32 hDelta[2];
    HidNpadHandheldState state;

    hidGetNpadStatesJoyLeft(this->npadType, &state, 1);
    hDelta[0] = -state.analog_stick_l.x;
    hDelta[1] = -state.analog_stick_r.x;
    vDelta[0] = -state.analog_stick_l.y;
    vDelta[1] = -state.analog_stick_r.y;

    int64 correctedButtonMasks = state.buttons;

    // "Rotate" the buttons
    correctedButtonMasks |= (state.buttons & HidNpadButton_Down) ? HidNpadButton_A : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_Left) ? HidNpadButton_B : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_Right) ? HidNpadButton_X : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_Up) ? HidNpadButton_Y : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_StickL) ? HidNpadButton_StickL : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_Plus) ? HidNpadButton_Plus : 0;

    correctedButtonMasks &= ~HidNpadButton_L;
    if (state.buttons & HidNpadButton_LeftSL)
        correctedButtonMasks |= HidNpadButton_L;

    correctedButtonMasks &= ~HidNpadButton_R;
    if (state.buttons & HidNpadButton_LeftSR)
        correctedButtonMasks |= HidNpadButton_R;

    UpdateInputNX(this, &correctedButtonMasks, hDelta, vDelta);
    if (state.buttons & HidNpadButton_L) {
        // if (this->npadType >= HidNpadIdType_No4)
        //     OnAssertionFailure(2, "", "", "", 0);

        if (lastNPadType <= 3 && activeNPadTypes[lastNPadType] == 4)
            hidMergeSingleJoyAsDualJoy(activeNPadTypes[lastNPadType], this->npadType);
        else
            lastNPadType = this->npadType;
    }
}

void InputDeviceNXJoyR::UpdateInput()
{
    int32 vDelta[2];
    int32 hDelta[2];
    HidNpadHandheldState state;

    hidGetNpadStatesJoyRight(this->npadType, &state, 1);
    hDelta[0] = -state.analog_stick_l.x;
    hDelta[1] = -state.analog_stick_r.x;
    vDelta[0] = -state.analog_stick_l.y;
    vDelta[1] = -state.analog_stick_r.y;

    int64 correctedButtonMasks = state.buttons;

    // "Rotate" the buttons
    correctedButtonMasks |= (state.buttons & HidNpadButton_A) ? HidNpadButton_B : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_B) ? HidNpadButton_Y : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_X) ? HidNpadButton_A : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_Y) ? HidNpadButton_X : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_StickR) ? HidNpadButton_StickL : 0;
    correctedButtonMasks |= (state.buttons & HidNpadButton_Plus) ? HidNpadButton_Plus : 0;

    correctedButtonMasks &= ~HidNpadButton_L;
    if (state.buttons & HidNpadButton_RightSL)
        correctedButtonMasks |= HidNpadButton_L;

    correctedButtonMasks &= ~HidNpadButton_R;
    if (state.buttons & HidNpadButton_RightSR)
        correctedButtonMasks |= HidNpadButton_R;

    UpdateInputNX(this, &correctedButtonMasks, hDelta, vDelta);
    if (state.buttons & HidNpadButton_R) {
        // if (this->npadType >= HidNpadIdType_No4)
        //     OnAssertionFailure(2, "", "", "", 0);

        if (lastNPadType <= 3 && activeNPadTypes[lastNPadType] == 3)
            hidMergeSingleJoyAsDualJoy(activeNPadTypes[lastNPadType], this->npadType);
        else
            lastNPadType = this->npadType;
    }
}

void InputDeviceNXJoyGrip::UpdateInput()
{
    int32 vDelta[2];
    int32 hDelta[2];
    HidNpadHandheldState state;

    hidGetNpadStatesJoyDual(this->npadType, &state, 1);
    hDelta[0] = -state.analog_stick_l.x;
    hDelta[1] = -state.analog_stick_r.x;
    vDelta[0] = -state.analog_stick_l.y;
    vDelta[1] = -state.analog_stick_r.y;

    UpdateInputNX(this, &state.buttons, hDelta, vDelta);

    // if (this->npadType >= HidNpadIdType_No4)
    //     OnAssertionFailure(2, "", "", "", 0);

    int32 masks = (int32)state.buttons;
    if ((masks & HidNpadButton_LeftSL) && (masks & HidNpadButton_LeftSR)) {
        for (int32 id = HidNpadIdType_No1; id <= HidNpadIdType_No4; ++id) {
            if (!hidGetNpadStyleSet(id)) {
                hidSetNpadJoyAssignmentModeSingleByDefault(id);
                return;
            }
        }

        hidSetNpadJoyAssignmentModeSingle(this->npadType, HidNpadJoyDeviceType_Left);
    }
    else if ((masks & HidNpadButton_RightSL) && (masks & HidNpadButton_RightSR)) {
        for (int32 id = HidNpadIdType_No1; id <= HidNpadIdType_No4; ++id) {
            if (!hidGetNpadStyleSet(id)) {
                hidSetNpadJoyAssignmentModeSingleByDefault(id);
                return;
            }
        }

        hidSetNpadJoyAssignmentModeSingle(this->npadType, HidNpadJoyDeviceType_Right);
    }
}

void InputDeviceNXPro::UpdateInput()
{
    int32 vDelta[2];
    int32 hDelta[2];
    HidNpadHandheldState state;

    hidGetNpadStatesFullKey(this->npadType, &state, 1);
    hDelta[0] = -state.analog_stick_l.x;
    hDelta[1] = -state.analog_stick_r.x;
    vDelta[0] = -state.analog_stick_l.y;
    vDelta[1] = -state.analog_stick_r.y;

    UpdateInputNX(this, &state.buttons, hDelta, vDelta);
}

void InputDeviceNX::ProcessInput(int32 controllerID)
{
    int32 buttonMasks = (int32)this->buttonMasks;

    controller[controllerID].keyUp.press |= (buttonMasks & HidNpadButton_Up) != 0;
    controller[controllerID].keyDown.press |= (buttonMasks & HidNpadButton_Down) != 0;
    controller[controllerID].keyLeft.press |= (buttonMasks & HidNpadButton_Left) != 0;
    controller[controllerID].keyRight.press |= (buttonMasks & HidNpadButton_Right) != 0;
    controller[controllerID].keyA.press |= (buttonMasks & HidNpadButton_B) != 0;
    controller[controllerID].keyB.press |= (buttonMasks & HidNpadButton_A) != 0;
    controller[controllerID].keyX.press |= (buttonMasks & HidNpadButton_Y) != 0;
    controller[controllerID].keyY.press |= (buttonMasks & HidNpadButton_X) != 0;
    controller[controllerID].keyStart.press |= (buttonMasks & HidNpadButton_Plus) != 0;
    controller[controllerID].keySelect.press |= (buttonMasks & HidNpadButton_Minus) != 0;

#if RETRO_REV02
    stickL[controllerID].keyStick.press |= (buttonMasks & HidNpadButton_StickL) != 0;
    stickL[controllerID].hDelta = this->hDelta_L;
    stickL[controllerID].vDelta = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    stickR[controllerID].keyStick.press |= (buttonMasks & HidNpadButton_StickR) != 0;
    stickR[controllerID].hDelta = this->hDelta_R;
    stickR[controllerID].vDelta = this->vDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > INPUT_DEADZONE;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyRight.press |= this->hDelta_R > INPUT_DEADZONE;

    triggerL[controllerID].keyBumper.press |= (buttonMasks & HidNpadButton_L) != 0;
    triggerL[controllerID].keyTrigger.press |= (buttonMasks & HidNpadButton_ZL) != 0;
    triggerL[controllerID].bumperDelta  = triggerL[controllerID].keyBumper.press ? 1.0 : 0.0;
    triggerL[controllerID].triggerDelta = triggerL[controllerID].keyTrigger.press ? 1.0 : 0.0;

    triggerR[controllerID].keyBumper.press |= (buttonMasks & HidNpadButton_R) != 0;
    triggerR[controllerID].keyTrigger.press |= (buttonMasks & HidNpadButton_ZR) != 0;
    triggerR[controllerID].bumperDelta  = triggerR[controllerID].keyBumper.press ? 1.0 : 0.0;
    triggerR[controllerID].triggerDelta = triggerR[controllerID].keyTrigger.press ? 1.0 : 0.0;
#else
    controller[controllerID].keyStickL.press |= (buttonMasks & HidNpadButton_StickL) != 0;
    stickL[controllerID].hDeltaL = this->hDelta_L;
    stickL[controllerID].vDeltaL = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    controller[controllerID].keyStickR.press |= (buttonMasks & HidNpadButton_StickR) != 0;
    stickL[controllerID].hDeltaL = this->hDelta_R;
    stickL[controllerID].vDeltaL = this->vDelta_R;

    controller[controllerID].keyBumperL.press |= (buttonMasks & HidNpadButton_L) != 0;
    controller[controllerID].keyTriggerL.press |= (buttonMasks & HidNpadButton_ZL) != 0;
    stickL[controllerID].triggerDeltaL = controller[controllerID].keyTriggerL.press ? 1.0 : 0.0;

    controller[controllerID].keyBumperR.press |= (buttonMasks & HidNpadButton_R) != 0;
    controller[controllerID].keyTriggerR.press |= (buttonMasks & HidNpadButton_ZR) != 0;
    stickL[controllerID].triggerDeltaR = controller[controllerID].keyTriggerR.press ? 1.0 : 0.0;
#endif
}

InputDeviceNXHandheld *RSDK::SKU::InitNXHandheldInputDevice(uint32 id, int32 type)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceNXHandheld();

    InputDeviceNXHandheld *device = (InputDeviceNXHandheld *)InputDevices[InputDeviceCount];

    device->gamePadType = (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_HANDHELD << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;
    device->npadType    = type;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}

InputDeviceNXJoyL *RSDK::SKU::InitNXJoyLInputDevice(uint32 id, int32 type)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceNXJoyL();

    InputDeviceNXJoyL *device = (InputDeviceNXJoyL *)InputDevices[InputDeviceCount];

    device->gamePadType = (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_JOY_L << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;
    device->npadType    = type;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}
InputDeviceNXJoyR *RSDK::SKU::InitNXJoyRInputDevice(uint32 id, int32 type)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceNXJoyR();

    InputDeviceNXJoyR *device = (InputDeviceNXJoyR *)InputDevices[InputDeviceCount];

    device->gamePadType = (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_JOY_R << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;
    device->npadType    = type;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}
InputDeviceNXJoyGrip *RSDK::SKU::InitNXJoyGripInputDevice(uint32 id, int32 type)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceNXJoyGrip();

    InputDeviceNXJoyGrip *device = (InputDeviceNXJoyGrip *)InputDevices[InputDeviceCount];

    device->gamePadType = (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_JOY_GRIP << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;
    device->npadType    = type;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}
InputDeviceNXPro *RSDK::SKU::InitNXProInputDevice(uint32 id, int32 type)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceNXPro();

    InputDeviceNXPro *device = (InputDeviceNXPro *)InputDevices[InputDeviceCount];

    device->gamePadType = (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_PRO << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;
    device->npadType    = type;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}

void RSDK::SKU::InitNXInputAPI()
{
    hidInitializeNpad();
    hidInitializeTouchScreen();
    hidSetSupportedNpadStyleSet(HidNpadStyleTag_NpadFullKey | HidNpadStyleTag_NpadHandheld | HidNpadStyleTag_NpadJoyDual | HidNpadStyleTag_NpadJoyLeft
                                | HidNpadStyleTag_NpadJoyRight);
    hidSetSupportedNpadIdType(npadTypes, 5);

    hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);

    ProcessInput();

    // Unsure about this one, hope its right :)
    HidVibrationDeviceHandle handles[8];
    hidInitializeVibrationDevices(handles, 8, HidNpadIdType_No3, HidNpadStyleTag_NpadFullKey);
    nxVibrateDeviceCount = 8;
}

void RSDK::SKU::ProcessNXInputDevices()
{
    lastNPadType = -1;

    // check for lost connections
    for (int32 d = 4; d >= 1; --d) {
        int32 type = activeNPadTypes[d];

        // if no controller (and there was one before), remove!
        if (type != -1 && !hidGetNpadStyleSet(npadTypes[d])) {
            int32 prevID     = storedNPadIDs[d];
            storedNPadIDs[d] = 0;
            if (prevID)
                nPadIDs[inactiveNPadCount++] = storedNPadIDs[d];

            InputDevice *device = InputDeviceFromID(prevID);
            if (device)
                RemoveInputDevice(device);

            activeNPadTypes[d] = -1;
        }
    }

    // check for changed controller types
    for (int32 d = 4; d >= 1; --d) {
        int32 type = activeNPadTypes[d];

        // if what it is now, isn't what we "remembered", change!
        if (type != -1 && !(hidGetNpadStyleSet(npadTypes[d]) & (1 << (type & 0x1F)))) {
            int32 prevID     = storedNPadIDs[d];
            storedNPadIDs[d] = 0;
            if (prevID)
                nPadIDs[inactiveNPadCount++] = storedNPadIDs[d];

            InputDevice *device = InputDeviceFromID(prevID);
            if (device)
                RemoveInputDevice(device);

            activeNPadTypes[d] = -1;
        }
    }

    // update input device states & etc
    for (int32 c = 0; c < 5; ++c) {
        uint32 styleSet = hidGetNpadStyleSet(npadTypes[c]);

        if (activeNPadTypes[c] == -1) {
            InputDevice *device = NULL;
            uint32 id           = 0;

            if (!device && (styleSet & HidNpadStyleTag_NpadHandheld)) {
                id = 0;
                if (inactiveNPadCount) {
                    id               = nPadIDs[--inactiveNPadCount];
                    storedNPadIDs[c] = id;
                }

                device = InputDeviceFromID(id);
                if (!device)
                    device = InitNXHandheldInputDevice(id, npadTypes[c]);

                activeNPadTypes[c] = 1;
            }

            if (!device && (styleSet & HidNpadStyleTag_NpadJoyLeft)) {
                id = 0;
                if (inactiveNPadCount) {
                    id               = nPadIDs[--inactiveNPadCount];
                    storedNPadIDs[c] = id;
                }

                device = InputDeviceFromID(id);
                if (!device)
                    device = InitNXJoyLInputDevice(id, npadTypes[c]);

                if (device) {
                    hidSetNpadJoyAssignmentModeSingleByDefault(npadTypes[c]);
                    hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
                }

                activeNPadTypes[c] = 3;
            }

            if (!device && (styleSet & HidNpadStyleTag_NpadJoyRight)) {
                id = 0;
                if (inactiveNPadCount) {
                    id               = nPadIDs[--inactiveNPadCount];
                    storedNPadIDs[c] = id;
                }

                device = InputDeviceFromID(id);
                if (!device)
                    device = InitNXJoyRInputDevice(id, npadTypes[c]);

                if (device) {
                    hidSetNpadJoyAssignmentModeSingleByDefault(npadTypes[c]);
                    hidSetNpadJoyHoldType(HidNpadJoyHoldType_Horizontal);
                }

                activeNPadTypes[c] = 4;
            }

            if (!device && (styleSet & HidNpadStyleTag_NpadJoyDual)) {
                id = 0;
                if (inactiveNPadCount) {
                    id               = nPadIDs[--inactiveNPadCount];
                    storedNPadIDs[c] = id;
                }

                device = InputDeviceFromID(id);
                if (!device)
                    device = InitNXJoyGripInputDevice(id, npadTypes[c]);

                activeNPadTypes[c] = 2;
            }

            if (!device && (styleSet & HidNpadStyleTag_NpadFullKey)) {
                id = 0;
                if (inactiveNPadCount) {
                    id               = nPadIDs[--inactiveNPadCount];
                    storedNPadIDs[c] = id;
                }

                device = InputDeviceFromID(id);
                if (!device)
                    device = InitNXProInputDevice(id, npadTypes[c]);

                activeNPadTypes[c] = 0;
            }
        }
    }

    HidTouchScreenState touchState;
    hidGetTouchScreenStates(&touchState, 1);

    touchInfo.count = 0;
    for (int32 f = 0; f < touchState.count; ++f) {
        if (!touchState.touches[f].finger_id) {
            touchInfo.down[0] = true;
            touchInfo.x[0]    = touchState.touches[0].x / 1280.0;
            touchInfo.y[0]    = touchState.touches[0].y / 720.0;
            touchInfo.count++;
        }
    }
}