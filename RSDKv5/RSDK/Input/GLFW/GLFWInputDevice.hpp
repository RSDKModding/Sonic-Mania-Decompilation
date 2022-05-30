
namespace SKU
{

struct InputDeviceGLFW : InputDevice {
    void UpdateInput(void);
    void ProcessInput(int32 controllerID);
    void CloseDevice();

    uint32 currentState;
    GLFWgamepadstate states[2];
    uint32 jid;

    bool32 swapABXY = false;
};

InputDeviceGLFW *InitGLFWInputDevice(uint32 id, uint8 controllerID);
void InitGLFWInputAPI();

} // namespace SKU