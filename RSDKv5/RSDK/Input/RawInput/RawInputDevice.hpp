
namespace SKU
{

enum RawInputMappingTypes {
    RAWMAP_NONE,
    RAWMAP_ONBUTTONDOWN,
    RAWMAP_ONBUTTONUP,
    RAWMAP_DIRECTIONAL,
    RAWMAP_ANALOG_NEG,
    RAWMAP_ANALOG_POS,
};

enum RawInputButtonIDs {
    RAWBUTTON_UP,
    RAWBUTTON_DOWN,
    RAWBUTTON_LEFT,
    RAWBUTTON_RIGHT,
    RAWBUTTON_START,
    RAWBUTTON_SELECT,
    RAWBUTTON_STICKL,
    RAWBUTTON_STICKR,
    RAWBUTTON_L1,
    RAWBUTTON_R1,
    RAWBUTTON_C,
    RAWBUTTON_Z,
    RAWBUTTON_A,
    RAWBUTTON_B,
    RAWBUTTON_X,
    RAWBUTTON_Y,
    RAWBUTTON_L2,
    RAWBUTTON_R2,
    RAWBUTTON_TRIGGER_L,
    RAWBUTTON_TRIGGER_R,
    RAWBUTTON_HDELTA_L,
    RAWBUTTON_VDELTA_L,
    RAWBUTTON_HDELTA_R,
    RAWBUTTON_VDELTA_R,
    RAWBUTTON_COUNT,
};

struct InputDeviceRaw : InputDevice {
    void UpdateInput();
    void ProcessInput(int32 controllerID);

    HANDLE deviceHandle;

    int32 activeButtons;
    int32 buttonMasks;
    int32 prevButtonMasks;
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
    float unused1;
    float unused2;
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
void InitHIDDevices();
void UpdateHIDButtonStates(HRAWINPUT hRawInput);

} // namespace SKU