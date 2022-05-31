
using namespace RSDK;

bool32 RSDK::SKU::disabledXInputDevices[PLAYER_COUNT];

void RSDK::SKU::InputDeviceXInput::UpdateInput()
{
    XINPUT_STATE *inputState = &this->inputState[this->activeState];
    if (disabledXInputDevices[this->controllerID]) {
        this->disabled = true;
    }
    else {
        this->disabled = false;
        if (!XInputGetState(this->controllerID, inputState)) {
            this->activeState ^= 1;

            int32 changedButtons = ~this->inputState[this->activeState].Gamepad.wButtons
                                   & (this->inputState[this->activeState].Gamepad.wButtons ^ inputState->Gamepad.wButtons);

            if (changedButtons)
                this->inactiveTimer[0] = 0;
            else
                ++this->inactiveTimer[0];

            if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
                this->inactiveTimer[1] = 0;
            else
                ++this->inactiveTimer[1];

            this->anyPress = changedButtons || ((inputState->Gamepad.sThumbLX ^ this->inputState[this->activeState].Gamepad.sThumbLX) & 0xC000) != 0
                             || ((inputState->Gamepad.sThumbLY ^ this->inputState[this->activeState].Gamepad.sThumbLY) & 0xC000) != 0
                             || ((inputState->Gamepad.sThumbRX ^ this->inputState[this->activeState].Gamepad.sThumbRX) & 0xC000) != 0
                             || ((inputState->Gamepad.sThumbRY ^ this->inputState[this->activeState].Gamepad.sThumbRY) & 0xC000) != 0;

            XINPUT_GAMEPAD *gamePad = &this->inputState[this->activeState].Gamepad;

            this->stateUp       = (gamePad->wButtons & KEYMASK_UP) != 0;
            this->stateDown     = (gamePad->wButtons & KEYMASK_DOWN) != 0;
            this->stateLeft     = (gamePad->wButtons & KEYMASK_LEFT) != 0;
            this->stateRight    = (gamePad->wButtons & KEYMASK_RIGHT) != 0;
            this->stateA        = (gamePad->wButtons & KEYMASK_A) != 0;
            this->stateB        = (gamePad->wButtons & KEYMASK_B) != 0;
            this->stateX        = (gamePad->wButtons & KEYMASK_X) != 0;
            this->stateY        = (gamePad->wButtons & KEYMASK_Y) != 0;
            this->stateStart    = (gamePad->wButtons & KEYMASK_START) != 0;
            this->stateSelect   = (gamePad->wButtons & KEYMASK_SELECT) != 0;
            this->stateBumper_L = (gamePad->wButtons & KEYMASK_BUMPERL) != 0;
            this->stateBumper_R = (gamePad->wButtons & KEYMASK_BUMPERR) != 0;
            this->stateStick_L  = (gamePad->wButtons & KEYMASK_STICKL) != 0;
            this->stateStick_R  = (gamePad->wButtons & KEYMASK_STICKR) != 0;

            this->hDelta_L = gamePad->sThumbLX;
            this->vDelta_L = gamePad->sThumbLY;

            float div      = sqrtf((this->hDelta_L * this->hDelta_L) + (this->vDelta_L * this->vDelta_L));
            this->hDelta_L = this->hDelta_L / div;
            this->vDelta_L = this->vDelta_L / div;
            if (div <= 7864.0) {
                this->hDelta_L = 0.0;
                this->vDelta_L = 0.0;
            }
            else {
                this->hDelta_L = this->hDelta_L * ((fminf(32767.0, div) - 7864.0) / 24903.0);
                this->vDelta_L = this->vDelta_L * ((fminf(32767.0, div) - 7864.0) / 24903.0);
            }

            this->hDelta_R = gamePad->sThumbRX;
            this->vDelta_R = gamePad->sThumbRY;

            div            = sqrtf((this->hDelta_R * this->hDelta_R) + (this->vDelta_R * this->vDelta_R));
            this->hDelta_R = this->hDelta_R / div;
            this->vDelta_R = this->vDelta_R / div;
            if (div <= 7864.0) {
                this->hDelta_R = 0.0;
                this->vDelta_R = 0.0;
            }
            else {
                this->hDelta_R = this->hDelta_R * ((fminf(32767.0, div) - 7864.0) / 24903.0);
                this->vDelta_R = this->vDelta_R * ((fminf(32767.0, div) - 7864.0) / 24903.0);
            }

            this->deltaBumper_L = this->stateBumper_L ? 1.0 : 0.0;

            this->deltaTrigger_L = gamePad->bLeftTrigger;
            if (this->deltaTrigger_L <= 30.0)
                this->deltaTrigger_L = 0.0;
            else
                this->deltaTrigger_L = (this->deltaTrigger_L - 30.0) / 225.0;

            this->deltaBumper_R = this->stateBumper_R ? 1.0 : 0.0;

            this->deltaTrigger_R = gamePad->bRightTrigger;
            if (this->deltaTrigger_R <= 30.0)
                this->deltaTrigger_R = 0.0;
            else
                this->deltaTrigger_R = (this->deltaTrigger_R - 30.0) / 225.0;
        }

        this->ProcessInput(CONT_ANY);
    }
}

void RSDK::SKU::InputDeviceXInput::ProcessInput(int32 controllerID)
{
    controller[controllerID].keyUp.press |= this->stateUp;
    controller[controllerID].keyDown.press |= this->stateDown;
    controller[controllerID].keyLeft.press |= this->stateLeft;
    controller[controllerID].keyRight.press |= this->stateRight;
    controller[controllerID].keyA.press |= this->stateA;
    controller[controllerID].keyB.press |= this->stateB;
    controller[controllerID].keyX.press |= this->stateX;
    controller[controllerID].keyY.press |= this->stateY;
    controller[controllerID].keyStart.press |= this->stateStart;
    controller[controllerID].keySelect.press |= this->stateSelect;

#if RETRO_REV02
    stickL[controllerID].keyStick.press |= this->stateStick_L;
    stickL[controllerID].hDelta = this->hDelta_L;
    stickL[controllerID].vDelta = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    stickR[controllerID].keyStick.press |= this->stateStick_R;
    stickR[controllerID].hDelta = this->hDelta_R;
    stickR[controllerID].vDelta = this->vDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > INPUT_DEADZONE;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyRight.press |= this->hDelta_R > INPUT_DEADZONE;

    triggerL[controllerID].keyBumper.press |= this->stateBumper_L;
    triggerL[controllerID].bumperDelta  = this->deltaBumper_L;
    triggerL[controllerID].triggerDelta = this->deltaTrigger_L;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].bumperDelta  = this->deltaBumper_R;
    triggerR[controllerID].triggerDelta = this->deltaTrigger_R;
#else
    controller[controllerID].keyStickL.press |= this->stateStick_L;
    stickL[controllerID].hDeltaL = this->hDelta_L;
    stickL[controllerID].vDeltaL = this->vDelta_L;

    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    controller[controllerID].keyStickR.press |= this->stateStick_R;
    stickL[controllerID].hDeltaL = this->hDelta_R;
    stickL[controllerID].vDeltaL = this->vDelta_R;

    controller[controllerID].keyBumperL.press |= this->stateBumper_L;
    stickL[controllerID].deadzone      = this->deltaBumper_L;
    stickL[controllerID].triggerDeltaL = this->deltaTrigger_L;

    controller[controllerID].keyBumperR.press |= this->stateBumper_R;
    stickL[controllerID].triggerDeltaR = this->deltaTrigger_R;
#endif
}

RSDK::SKU::InputDeviceXInput *RSDK::SKU::InitXInputDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceXInput();

    InputDeviceXInput *device = (InputDeviceXInput *)InputDevices[InputDeviceCount];

    for (int32 i = 0; i < PLAYER_COUNT; ++i) disabledXInputDevices[i] = false;

    device->gamePadType = (DEVICE_API_XINPUT << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_XBOX << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}

void RSDK::SKU::InitXInputAPI()
{
    char idString[16];

    sprintf_s(idString, (int32)sizeof(idString), "%s", "XInputDevice0");

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        idString[12] = '0' + i;

        uint32 id;
        GenerateHashCRC(&id, idString);

        InputDeviceXInput *device = NULL;
        for (int32 d = 0; d < InputDeviceCount; ++d) {
            if (InputDevices[d] && InputDevices[d]->inputID == id) {
                device = (InputDeviceXInput *)InputDevices[d];
                break;
            }
        }

        XINPUT_STATE pState;
        if (XInputGetState(i, &pState)) {
            if (device)
                RemoveInputDevice(device);
        }
        else if (!device) {
            device = InitXInputDevice(id);
            if (device)
                device->controllerID = i;
        }
    }
}

void RSDK::SKU::UpdateXInputDevices()
{
    for (int32 i = 0; i < PLAYER_COUNT; ++i) disabledXInputDevices[i] = false;
}