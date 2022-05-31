
namespace SKU
{

struct InputDeviceNX : InputDevice {
    void ProcessInput(int32 controllerID);

    int64 buttonMasks;
    float hDelta_L;
    float hDelta_R;
    float vDelta_L;
    float vDelta_R;
    int32 npadType;
};

struct InputDeviceNXHandheld : InputDeviceNX {
    void UpdateInput();
};

struct InputDeviceNXJoyL : InputDeviceNX {
    void UpdateInput();
};

struct InputDeviceNXJoyR : InputDeviceNX {
    void UpdateInput();
};

struct InputDeviceNXJoyGrip : InputDeviceNX {
    void UpdateInput();
};

struct InputDeviceNXPro : InputDeviceNX {
    void UpdateInput();
};

extern int32 currentNXControllerType;

InputDeviceNXHandheld *InitNXHandheldInputDevice(uint32 id, int32 type);
InputDeviceNXJoyL *InitNXJoyLInputDevice(uint32 id, int32 type);
InputDeviceNXJoyR *InitNXJoyRInputDevice(uint32 id, int32 type);
InputDeviceNXJoyGrip *InitNXJoyGripInputDevice(uint32 id, int32 type);
InputDeviceNXPro *InitNXProInputDevice(uint32 id, int32 type);

void InitNXInputAPI();
void ProcessNXInputDevices();

} // namespace SKU