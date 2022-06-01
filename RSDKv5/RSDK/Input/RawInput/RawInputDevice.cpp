
#include <hidusage.h>

using namespace RSDK;

bool32 RSDK::SKU::HIDEnabled = false;

InputDevice *RSDK::SKU::rawInputDevices[INPUTDEVICE_COUNT];
int32 RSDK::SKU::rawInputDeviceCount = 0;

tagRAWINPUT RSDK::SKU::rawInputData;

void RSDK::SKU::InputDeviceRaw::UpdateInput()
{
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

    this->stateUp        = (this->buttonMasks & KEYMASK_UP) != 0;
    this->stateDown      = (this->buttonMasks & KEYMASK_DOWN) != 0;
    this->stateLeft      = (this->buttonMasks & KEYMASK_LEFT) != 0;
    this->stateRight     = (this->buttonMasks & KEYMASK_RIGHT) != 0;
    this->stateA         = (this->buttonMasks & KEYMASK_A) != 0;
    this->stateB         = (this->buttonMasks & KEYMASK_B) != 0;
    this->stateC         = (this->buttonMasks & KEYMASK_C) != 0;
    this->stateX         = (this->buttonMasks & KEYMASK_X) != 0;
    this->stateY         = (this->buttonMasks & KEYMASK_Y) != 0;
    this->stateZ         = (this->buttonMasks & KEYMASK_Z) != 0;
    this->stateStart     = (this->buttonMasks & KEYMASK_START) != 0;
    this->stateSelect    = (this->buttonMasks & KEYMASK_SELECT) != 0;
    this->stateBumper_L  = (this->buttonMasks & KEYMASK_BUMPERL) != 0;
    this->stateBumper_R  = (this->buttonMasks & KEYMASK_BUMPERR) != 0;
    this->stateStick_L   = (this->buttonMasks & KEYMASK_STICKL) != 0;
    this->stateStick_R   = (this->buttonMasks & KEYMASK_STICKR) != 0;
    this->stateTrigger_L = (this->buttonMasks & KEYMASK_TRIGGERL) != 0;
    this->stateTrigger_R = (this->buttonMasks & KEYMASK_TRIGGERR) != 0;

    this->ProcessInput(CONT_ANY);
}

void RSDK::SKU::InputDeviceRaw::ProcessInput(int32 controllerID)
{
    controller[controllerID].keyUp.press |= this->stateUp;
    controller[controllerID].keyDown.press |= this->stateDown;
    controller[controllerID].keyLeft.press |= this->stateLeft;
    controller[controllerID].keyRight.press |= this->stateRight;
    controller[controllerID].keyA.press |= this->stateA;
    controller[controllerID].keyB.press |= this->stateB;
    controller[controllerID].keyC.press |= this->stateC;
    controller[controllerID].keyX.press |= this->stateX;
    controller[controllerID].keyY.press |= this->stateY;
    controller[controllerID].keyZ.press |= this->stateZ;
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
    stickR[controllerID].hDelta = this->vDelta_R;
    stickR[controllerID].vDelta = this->hDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > INPUT_DEADZONE;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyRight.press |= this->hDelta_R > INPUT_DEADZONE;

    triggerL[controllerID].keyBumper.press |= this->stateBumper_L;
    triggerL[controllerID].keyTrigger.press |= this->stateTrigger_L;
    triggerL[controllerID].bumperDelta  = this->triggerDeltaL;
    triggerL[controllerID].triggerDelta = this->triggerDeltaL;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].keyTrigger.press |= this->stateTrigger_R;
    triggerR[controllerID].bumperDelta  = this->triggerDeltaR;
    triggerR[controllerID].triggerDelta = this->triggerDeltaR;
#else
    controller[controllerID].keyStickL.press |= this->stateStick_L;
    stickL[controllerID].hDeltaL = this->hDelta_L;
    stickL[controllerID].vDeltaL = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    controller[controllerID].keyStickR.press |= this->stateStick_R;
    stickL[controllerID].hDeltaR = this->vDelta_R;
    stickL[controllerID].vDeltaR = this->hDelta_R;

    controller[controllerID].keyBumperL.press |= this->stateBumper_L;
    controller[controllerID].keyTriggerL.press |= this->stateTrigger_L;
    stickL[controllerID].triggerDeltaL = this->triggerDeltaL;

    controller[controllerID].keyBumperR.press |= this->stateBumper_R;
    controller[controllerID].keyTriggerR.press |= this->stateTrigger_R;
    stickL[controllerID].triggerDeltaR = this->triggerDeltaR;
#endif
}

RSDK::SKU::InputDeviceRaw *RSDK::SKU::InitRawInputDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceRaw();

    InputDeviceRaw *device = (InputDeviceRaw *)InputDevices[InputDeviceCount];

    InputDeviceCount++;
    device->gamePadType = (DEVICE_API_RAWINPUT << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0);
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

void RSDK::SKU::InitHIDAPI()
{
    RAWINPUTDEVICE rawInputDevice = {};
    rawInputDevice.hwndTarget  = RenderDevice::windowHandle;
    rawInputDevice.usUsagePage = HID_USAGE_PAGE_GENERIC;
    rawInputDevice.usUsage     = HID_USAGE_GENERIC_GAMEPAD;
    rawInputDevice.dwFlags     = 0;

    if (!RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)))
        PrintLog(PRINT_NORMAL, "Unable to Register HID Device.");

    HIDEnabled = true;
    InitHIDDevices();
}

void RSDK::SKU::InitHIDDevices()
{
    if (HIDEnabled) {
        rawInputDeviceCount = 0;

        UINT puiNumDevices = 0;
        if (GetRawInputDeviceList(NULL, &puiNumDevices, sizeof(tagRAWINPUTDEVICELIST))) {
            PrintLog(PRINT_NORMAL, "Could not get Input Device Count.");
            return;
        }

        tagRAWINPUTDEVICELIST *pRawInputDeviceList;
        AllocateStorage(sizeof(tagRAWINPUTDEVICELIST) * puiNumDevices, (void **)&pRawInputDeviceList, DATASET_TMP, false);
        if (!pRawInputDeviceList) {
            PrintLog(PRINT_NORMAL, "Could not allocate memory for Input Device List.");
            return;
        }

        int32 rawInputListSize = GetRawInputDeviceList(pRawInputDeviceList, &puiNumDevices, sizeof(tagRAWINPUTDEVICELIST));
        if (rawInputListSize == -1) {
            PrintLog(PRINT_NORMAL, "Wrong Device Count.");
        }
        else {
            RID_DEVICE_INFO deviceInfo;
            deviceInfo.cbSize         = sizeof(RID_DEVICE_INFO);

            uint8 deviceID            = 0;
            InputDeviceRaw *devicePtr = GetRawInputDevice(&deviceID);
            if (devicePtr) {
                int32 id = 0;
                for (; id < rawInputListSize; ++id) {
                    if (devicePtr->deviceHandle == pRawInputDeviceList[id].hDevice)
                        break;
                }

                if (id == rawInputListSize)
                    RemoveInputDevice(devicePtr);
            }

            for (int32 d = 0; d < rawInputListSize; ++d) {
                uint32 deviceInfoSize = deviceInfo.cbSize;
                UINT allocatedBytes = GetRawInputDeviceInfo(pRawInputDeviceList[d].hDevice, RIDI_DEVICEINFO, &deviceInfo, &deviceInfoSize);

                char deviceName[0x100];
                memset(deviceName, 0, sizeof(deviceName));
                uint32 deviceNameSize = sizeof(deviceName);
                allocatedBytes        = GetRawInputDeviceInfoA(pRawInputDeviceList[d].hDevice, RIDI_DEVICENAME, deviceName, &deviceNameSize);

                if (deviceInfo.dwType == RIM_TYPEHID && deviceInfo.hid.usUsage == HID_USAGE_GENERIC_GAMEPAD) {
                    uint32 id;
                    GenerateHashCRC(&id, deviceName);

                    InputDeviceRaw *device = (InputDeviceRaw *)InputDeviceFromID(id);
                    if (!device) {
                        for (int32 g = 0; g < gamePadCount; ++g) {
                            if (gamePadMappings[g].productID == deviceInfo.hid.dwProductId) {
                                device = InitRawInputDevice(id);

                                if (device) {
                                    device->gamePadType |= gamePadMappings[g].type;
                                    device->deviceHandle = pRawInputDeviceList[d].hDevice;
                                    memcpy(device->buttons, gamePadMappings[g].buttons, sizeof(device->buttons));
                                    PrintLog(PRINT_NORMAL, "%s Detected - Vendor ID: %x ProductID: %x", gamePadMappings[g].name,
                                             deviceInfo.hid.dwVendorId, deviceInfo.hid.dwProductId);
                                    rawInputDevices[rawInputDeviceCount++] = device;
                                }
                            }
                        }
                    }
                    else {
                        rawInputDevices[rawInputDeviceCount++] = device;
                    }
                }
            }

            PrintLog(PRINT_NORMAL, "Total HID GamePad Count: %d", rawInputDeviceCount);
        }

        RemoveStorageEntry((void **)&pRawInputDeviceList);
    }
}

void RSDK::SKU::UpdateHIDButtonStates(HRAWINPUT hRawInput)
{
    uint32 pcbSize;
    GetRawInputData(hRawInput, RID_INPUT, NULL, &pcbSize, sizeof(rawInputData.header));          // get size
    GetRawInputData(hRawInput, RID_INPUT, &rawInputData, &pcbSize, sizeof(rawInputData.header)); // get data

    for (int32 d = 0; d < rawInputDeviceCount; ++d) {
        InputDeviceRaw *device = (InputDeviceRaw *)rawInputDevices[d];

        if (device && device->deviceHandle == rawInputData.header.hDevice) {
            device->activeButtons = 0;
            for (int32 b = 0; b < RAWBUTTON_TRIGGER_L; ++b) {
                int32 offset = device->buttons[b].offset;

                switch (device->buttons[b].mappingType) {
                    default:
                    case RAWMAP_NONE:
                    case RAWMAP_ANALOG_NEG:
                    case RAWMAP_ANALOG_POS: break;

                    case RAWMAP_ONBUTTONDOWN:
                        if (((1 << (offset & 7)) & rawInputData.data.hid.bRawData[offset >> 3]) != 0)
                            device->activeButtons |= device->buttons[b].maskVal;
                        break;

                    case RAWMAP_ONBUTTONUP:
                        if (((1 << (offset & 7)) & rawInputData.data.hid.bRawData[offset >> 3]) == 0)
                            device->activeButtons |= device->buttons[b].maskVal;
                        break;

                    case RAWMAP_DIRECTIONAL:
                        switch (rawInputData.data.hid.bRawData[offset >> 3] & 0xF) {
                            case 0: device->activeButtons |= KEYMASK_UP; break;
                            case 1: device->activeButtons |= KEYMASK_UP | KEYMASK_RIGHT; break;
                            case 2: device->activeButtons |= KEYMASK_RIGHT; break;
                            case 3: device->activeButtons |= KEYMASK_DOWN | KEYMASK_RIGHT; break;
                            case 4: device->activeButtons |= KEYMASK_DOWN; break;
                            case 5: device->activeButtons |= KEYMASK_UP | KEYMASK_DOWN; break;
                            case 6: device->activeButtons |= KEYMASK_LEFT; break;
                            case 7: device->activeButtons |= KEYMASK_UP | KEYMASK_LEFT; break;
                            default: break;
                        }
                        break;
                }
            }

            for (int32 b = RAWBUTTON_TRIGGER_L; b < RAWBUTTON_COUNT; ++b) {
                float delta  = 0;
                int32 offset = device->buttons[b].offset;

                switch (device->buttons[b].mappingType) {
                    default:
                    case RAWMAP_NONE:
                    case RAWMAP_ONBUTTONDOWN:
                    case RAWMAP_ONBUTTONUP:
                    case RAWMAP_DIRECTIONAL: break;

                    case RAWMAP_ANALOG_NEG: delta = (rawInputData.data.hid.bRawData[offset >> 3] - 128.0) * -(1.0 / 128); break;
                    case RAWMAP_ANALOG_POS: delta = (rawInputData.data.hid.bRawData[offset >> 3] - 128.0) * (1.0 / 128); break;
                }

                switch (b) {
                    case RAWBUTTON_TRIGGER_L: device->triggerDeltaL = delta; break;
                    case RAWBUTTON_TRIGGER_R: device->triggerDeltaR = delta; break;
                    case RAWBUTTON_HDELTA_L: device->hDelta_L = delta; break;
                    case RAWBUTTON_VDELTA_L: device->vDelta_L = delta; break;
                    case RAWBUTTON_HDELTA_R: device->hDelta_R = delta; break;
                    case RAWBUTTON_VDELTA_R: device->vDelta_R = delta; break;
                }
            }
        }
    }
}