
namespace SKU
{

struct InputDeviceRaw : InputDevice {
    void UpdateInput(void);
    void ProcessInput(int32 controllerID);

    HANDLE deviceHandle;

    int32 activeButtons;
    int32 inputFlags;
    int32 prevInputFlags;
    uint8 stateUp;
    uint8 stateDown;
    uint8 stateLeft;
    uint8 stateRight;
    uint8 stateA;
    uint8 stateB;
    uint8 stateC;
    uint8 stateX;
    uint8 stateY;
    uint8 stateZ;
    uint8 stateStart;
    uint8 stateSelect;
    uint8 stateBumper_L;
    uint8 stateBumper_R;
    uint8 stateStick_L;
    uint8 stateStick_R;
    uint8 stateTrigger_L;
    uint8 stateTrigger_R;
    float triggerDeltaL;
    float triggerDeltaR;
    float hDelta_L;
    float vDelta_L;
    float vDelta_R;
    float hDelta_R;
    int32 unknown1;
    int32 unknown2;
    GamePadButtonMap buttons[24];
};

extern bool32 HIDEnabled;

extern InputDevice *rawInputDevices[INPUTDEVICE_COUNT];
extern int32 rawInputDeviceCount;

extern tagRAWINPUT rawInputData;

InputDeviceRaw *InitRawInputDevice(uint32 id);

inline InputDeviceRaw *GetRawInputDevice(uint8 *deviceID)
{
    if (*deviceID >= InputDeviceCount)
        return NULL;

    for (int32 d = *deviceID; d < InputDeviceCount; ++d) {
        if (!InputDevices[d])
            continue;

        InputDeviceRaw *device = (InputDeviceRaw *)InputDevices[d];

        if (((device->gamePadType >> 16) & 0xFF) == DEVICE_API_RAWINPUT) {
            *deviceID = d;
            return device;
        }
    }

    return NULL;
}

void InitHIDAPI();
void InitRawInputAPI();
void UpdateRawInputButtonState(HRAWINPUT hRawInput);

} // namespace SKU