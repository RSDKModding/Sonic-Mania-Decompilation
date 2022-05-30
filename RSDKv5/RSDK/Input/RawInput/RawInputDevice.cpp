
using namespace RSDK;

bool32 RSDK::SKU::HIDEnabled = false;

InputDevice *RSDK::SKU::rawInputDevices[INPUTDEVICE_COUNT];
int32 RSDK::SKU::rawInputDeviceCount = 0;

tagRAWINPUT RSDK::SKU::rawInputData;

void RSDK::SKU::InputDeviceRaw::UpdateInput()
{
    this->prevInputFlags = this->inputFlags;
    this->inputFlags     = this->activeButtons;

    int32 changedButtons = ~this->prevInputFlags & (this->prevInputFlags ^ this->inputFlags);
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

    this->stateUp        = (this->inputFlags & KEYMASK_UP) != 0;
    this->stateDown      = (this->inputFlags & KEYMASK_DOWN) != 0;
    this->stateLeft      = (this->inputFlags & KEYMASK_LEFT) != 0;
    this->stateRight     = (this->inputFlags & KEYMASK_RIGHT) != 0;
    this->stateA         = (this->inputFlags & KEYMASK_A) != 0;
    this->stateB         = (this->inputFlags & KEYMASK_B) != 0;
    this->stateC         = (this->inputFlags & KEYMASK_C) != 0;
    this->stateX         = (this->inputFlags & KEYMASK_X) != 0;
    this->stateY         = (this->inputFlags & KEYMASK_Y) != 0;
    this->stateZ         = (this->inputFlags & KEYMASK_Z) != 0;
    this->stateStart     = (this->inputFlags & KEYMASK_START) != 0;
    this->stateSelect    = (this->inputFlags & KEYMASK_SELECT) != 0;
    this->stateBumper_L  = (this->inputFlags & KEYMASK_BUMPERL) != 0;
    this->stateBumper_R  = (this->inputFlags & KEYMASK_BUMPERR) != 0;
    this->stateStick_L   = (this->inputFlags & KEYMASK_STICKL) != 0;
    this->stateStick_R   = (this->inputFlags & KEYMASK_STICKR) != 0;
    this->stateTrigger_L = (this->inputFlags & KEYMASK_TRIGGERL) != 0;
    this->stateTrigger_R = (this->inputFlags & KEYMASK_TRIGGERR) != 0;

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
    stickL[controllerID].keyUp.press |= this->vDelta_L > 0.3;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -0.3;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -0.3;
    stickL[controllerID].keyRight.press |= this->hDelta_L > 0.3;

    stickR[controllerID].keyStick.press |= this->stateStick_R;
    stickR[controllerID].hDelta = this->vDelta_R;
    stickR[controllerID].vDelta = this->hDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > 0.3;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -0.3;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -0.3;
    stickR[controllerID].keyRight.press |= this->hDelta_R > 0.3;

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
    stickL[controllerID].keyUp.press |= this->vDelta_L > 0.3;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -0.3;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -0.3;
    stickL[controllerID].keyRight.press |= this->hDelta_L > 0.3;

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
    RAWINPUTDEVICE pRawInputDevices;
    pRawInputDevices.hwndTarget  = RenderDevice::windowHandle;
    pRawInputDevices.usUsagePage = 1;
    pRawInputDevices.usUsage     = 5;
    pRawInputDevices.dwFlags     = 0;

    if (!RegisterRawInputDevices(&pRawInputDevices, 1, sizeof(pRawInputDevices)))
        PrintLog(PRINT_NORMAL, "Unable to Register HID Device.\n");

    HIDEnabled = true;
    InitRawInputAPI();
}

void RSDK::SKU::InitRawInputAPI()
{
    if (HIDEnabled) {
        rawInputDeviceCount = 0;

        UINT puiNumDevices;
        if (GetRawInputDeviceList(0, &puiNumDevices, sizeof(tagRAWINPUTDEVICELIST))) {
            PrintLog(PRINT_NORMAL, "Could not get Input Device Count.\n");
            return;
        }

        tagRAWINPUTDEVICELIST *pRawInputDeviceList;
        AllocateStorage(sizeof(tagRAWINPUTDEVICELIST) * puiNumDevices, (void **)&pRawInputDeviceList, DATASET_TMP, false);
        if (!pRawInputDeviceList) {
            PrintLog(PRINT_NORMAL, "Could not allocate memory for Input Device List.\n");
            return;
        }

        int32 rawInputListSize = GetRawInputDeviceList(pRawInputDeviceList, (PUINT)&puiNumDevices, sizeof(tagRAWINPUTDEVICELIST));
        if (rawInputListSize == -1) {
            PrintLog(PRINT_NORMAL, "Wrong Device Count.\n");
        }
        else {
            RID_DEVICE_INFO deviceInfo;
            deviceInfo.cbSize         = sizeof(RID_DEVICE_INFO);
            uint8 deviceID            = 0;
            InputDeviceRaw *devicePtr = GetRawInputDevice(&deviceID);
            if (devicePtr) {
                for (int32 i = 0; i < InputDeviceCount; ++i) {
                    if (!InputDevices[i])
                        return;

                    InputDeviceRaw *device = (InputDeviceRaw *)InputDevices[i];

                    if ((device->gamePadType >> 16) & 0xFF == DEVICE_API_RAWINPUT) {
                        int32 id = 0;
                        for (; id < rawInputListSize; ++id) {
                            if (device->deviceHandle == pRawInputDeviceList[id].hDevice)
                                break;
                        }

                        if (id == rawInputListSize)
                            RemoveInputDevice(device);
                    }
                }
            }

            for (int32 d = 0; d < rawInputListSize; ++d) {
                uint32 deviceInfoSize = deviceInfo.cbSize;
                GetRawInputDeviceInfoA(pRawInputDeviceList[d].hDevice, RIDI_DEVICEINFO, &deviceInfo, &deviceInfoSize);

                char deviceName[0x100];
                memset(deviceName, 0, sizeof(deviceName));
                uint32 deviceNameSize = sizeof(deviceName);
                GetRawInputDeviceInfoA(pRawInputDeviceList[d].hDevice, RIDI_DEVICENAME, deviceName, &deviceNameSize);

                if (deviceInfo.dwType == RIM_TYPEHID && deviceInfo.hid.usUsage == 5) {
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
                                    PrintLog(PRINT_NORMAL, "%s Detected - Vendor ID: %x ProductID: %x\n", gamePadMappings[g].name,
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

            PrintLog(PRINT_NORMAL, "Total HID GamePad Count: %d\n", rawInputDeviceCount);
        }

        RemoveStorageEntry((void **)&pRawInputDeviceList);
    }
}

void RSDK::SKU::UpdateRawInputButtonState(HRAWINPUT hRawInput)
{
    uint32 pcbSize;
    GetRawInputData(hRawInput, RID_INPUT, NULL, &pcbSize, sizeof(rawInputData.header));          // get size
    GetRawInputData(hRawInput, RID_INPUT, &rawInputData, &pcbSize, sizeof(rawInputData.header)); // get data

    for (int32 d = 0; d < rawInputDeviceCount; ++d) {
        InputDeviceRaw *device = (InputDeviceRaw *)rawInputDevices[d];

        if (device && device->deviceHandle == rawInputData.header.hDevice) {
            device->activeButtons = 0;
            for (int32 b = 0; b < 18; ++b) {
                int32 offset = device->buttons[b].offset;

                switch (device->buttons[b].mappingType) {
                    default:
                    case 0:
                    case 4:
                    case 5: break;

                    case 1:
                        if (((1 << (offset & 7)) & rawInputData.data.hid.bRawData[offset >> 3]) != 0)
                            device->activeButtons |= device->buttons[b].maskVal;
                        break;

                    case 2:
                        if (((1 << (offset & 7)) & rawInputData.data.hid.bRawData[offset >> 3]) == 0)
                            device->activeButtons |= device->buttons[b].maskVal;
                        break;

                    case 3:
                        switch (rawInputData.data.hid.bRawData[offset >> 3] & 0xF) {
                            case 0: device->activeButtons |= 1; break;
                            case 1: device->activeButtons |= 9; break;
                            case 2: device->activeButtons |= 8; break;
                            case 3: device->activeButtons |= 10; break;
                            case 4: device->activeButtons |= 2; break;
                            case 5: device->activeButtons |= 6; break;
                            case 6: device->activeButtons |= 4; break;
                            case 7: device->activeButtons |= 5; break;
                            default: break;
                        }
                        break;
                }
            }

            for (int32 b = 18; b < 24; ++b) {
                float delta  = 0;
                int32 offset = device->buttons[b].offset;

                switch (device->buttons[b].mappingType) {
                    default:
                    case 0:
                    case 1:
                    case 2:
                    case 3: break;

                    case 4: delta = (rawInputData.data.hid.bRawData[offset >> 3] - 128.0) * -0.0078125; break;
                    case 5: delta = (rawInputData.data.hid.bRawData[offset >> 3] - 128.0) * 0.0078125; break;
                }

                switch (b) {
                    case 18: device->triggerDeltaL = delta; break;
                    case 19: device->triggerDeltaR = delta; break;
                    case 20: device->hDelta_L = delta; break;
                    case 21: device->vDelta_L = delta; break;
                    case 22: device->hDelta_R = delta; break;
                    case 23: device->vDelta_R = delta; break;
                }
            }
        }
    }
}