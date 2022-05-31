
using namespace RSDK;

void InputDeviceSteam::UpdateInput()
{
    // TODO (maybe): document the actual steamAPI code that sets these values

    this->prevButtonMasks = this->buttonMasks;
    this->buttonMasks     = this->activeButtons;

    int32 changedButtons = ~this->prevButtonMasks & (this->prevButtonMasks ^ this->buttonMasks);
    if (changedButtons) {
        this->inactiveTimer[0] = 0;
        this->anyPress         = true;
    }
    else {
        ++this->inactiveTimer[0];
        this->anyPress = false;
    }

    if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
        this->inactiveTimer[1] = 0;
    else
        ++this->inactiveTimer[1];

    this->ProcessInput(CONT_ANY);
}

void InputDeviceSteam::ProcessInput(int32 controllerID)
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
}

InputDeviceSteam *RSDK::SKU::InitSteamInputDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceSteam();

    InputDeviceSteam *device = (InputDeviceSteam *)InputDevices[InputDeviceCount];

    device->gamePadType = (DEVICE_API_STEAM << 16) | (DEVICE_TYPE_STEAMOVERLAY << 8) | (DEVICE_KEYBOARD << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;

    device->keyMasks[0] = KEYMASK_UP;
    device->keyMasks[1] = KEYMASK_DOWN;
    device->keyMasks[2] = KEYMASK_LEFT;
    device->keyMasks[3] = KEYMASK_RIGHT;
    device->keyMasks[4] = KEYMASK_A;
    device->keyMasks[5] = KEYMASK_B;
    device->keyMasks[6] = KEYMASK_X;
    device->keyMasks[7] = KEYMASK_Y;
    device->keyMasks[8] = KEYMASK_START;

    device->buttonMasks     = 0;
    device->prevButtonMasks = 0;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}

void RSDK::SKU::InitSteamInputAPI()
{
    // TODO (maybe): document this?
}