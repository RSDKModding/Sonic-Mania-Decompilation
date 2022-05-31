
namespace SKU
{

extern int32 keyState[PLAYER_COUNT];

struct InputDeviceKeyboard : InputDevice {
    void UpdateInput();
    void ProcessInput(int32 controllerID);

    uint16 buttonMasks;
    uint16 prevButtonMasks;
    uint8 controllerID;
    int32 mouseHideTimer;
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
};

void InitKeyboardInputAPI();
InputDeviceKeyboard *InitKeyboardDevice(uint32 id);

void UpdateKeyState(int32 keyCode);
void ClearKeyState(int32 keyCode);

#if !RETRO_REV02
extern int32 specialKeyStates[4];
extern int32 prevSpecialKeyStates[4];
extern int32 buttonDownCount;
extern int32 prevButtonDownCount;

void HandleSpecialKeys();
#endif

} // namespace SKU
