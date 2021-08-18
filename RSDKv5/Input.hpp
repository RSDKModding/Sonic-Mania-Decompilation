#ifndef INPUT_H
#define INPUT_H

#define PLAYER_COUNT (4)
#define INPUTDEVICE_COUNT (0x10)

enum ControllerIDs {
    CONT_UNASSIGNED = -2,
    CONT_AUTOASSIGN = -1,
    CONT_ANY        = 0,
    CONT_P1         = 1,
    CONT_P2         = 2,
    CONT_P3         = 3,
    CONT_P4         = 4,
};

enum InputDeviceTypes {
    DEVICE_TYPE_UNKNOWN    = 0,
    DEVICE_TYPE_KEYBOARD   = 1,
    DEVICE_TYPE_CONTROLLER = 2,
};

enum InputDeviceIDs {
    DEVICE_KEYBOARD        = 0,
    DEVICE_XBOX            = 1,
    DEVICE_PS4             = 2,
    DEVICE_SATURN          = 3,
    DEVICE_SWITCH          = 4,
    DEVICE_SWITCH_PRO      = 5,
    DEVICE_SWITCH_JOY_L    = 6,
    DEVICE_SWITCH_JOY_R    = 7,
    DEVICE_SWITCH_JOY_GRIP = 8,
};

enum InputDeviceFlags {
    DEVICE_FLAG_UNKNOWN1    = 1,
    DEVICE_FLAG_UNKNOWN2    = 2,
    DEVICE_FLAG_UNKNOWN3    = 3,
    DEVICE_FLAG_UNKNOWN4    = 4,
};

enum ControllerKeys {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_START,
    KEY_SELECT,
    KEY_MAX,
};

enum WinMappings {
    VK_LBUTTON                         = 0x01,
    VK_RBUTTON                         = 0x02,
    VK_CANCEL                          = 0x03,
    VK_MBUTTON                         = 0x04,
    VK_XBUTTON1                        = 0x05,
    VK_XBUTTON2                        = 0x06,
    VK_BACK                            = 0x08,
    VK_TAB                             = 0x09,
    VK_CLEAR                           = 0x0C,
    VK_RETURN                          = 0x0D,
    VK_SHIFT                           = 0x10,
    VK_CONTROL                         = 0x11,
    VK_MENU                            = 0x12,
    VK_PAUSE                           = 0x13,
    VK_CAPITAL                         = 0x14,
    VK_KANA                            = 0x15,
    VK_HANGEUL                         = 0x15,
    VK_HANGUL                          = 0x15,
    VK_JUNJA                           = 0x17,
    VK_FINAL                           = 0x18,
    VK_HANJA                           = 0x19,
    VK_KANJI                           = 0x19,
    VK_ESCAPE                          = 0x1B,
    VK_CONVERT                         = 0x1C,
    VK_NONCONVERT                      = 0x1D,
    VK_ACCEPT                          = 0x1E,
    VK_MODECHANGE                      = 0x1F,
    VK_SPACE                           = 0x20,
    VK_PRIOR                           = 0x21,
    VK_NEXT                            = 0x22,
    VK_END                             = 0x23,
    VK_HOME                            = 0x24,
    VK_LEFT                            = 0x25,
    VK_UP                              = 0x26,
    VK_RIGHT                           = 0x27,
    VK_DOWN                            = 0x28,
    VK_SELECT                          = 0x29,
    VK_PRINT                           = 0x2A,
    VK_EXECUTE                         = 0x2B,
    VK_SNAPSHOT                        = 0x2C,
    VK_INSERT                          = 0x2D,
    VK_DELETE                          = 0x2E,
    VK_HELP                            = 0x2F,
    VK_0                               = 0x30,
    VK_1                               = 0x31,
    VK_2                               = 0x32,
    VK_3                               = 0x33,
    VK_4                               = 0x34,
    VK_5                               = 0x35,
    VK_6                               = 0x36,
    VK_7                               = 0x37,
    VK_8                               = 0x38,
    VK_9                               = 0x39,
    VK_A                               = 0x41,
    VK_B                               = 0x42,
    VK_C                               = 0x43,
    VK_D                               = 0x44,
    VK_E                               = 0x45,
    VK_F                               = 0x46,
    VK_G                               = 0x47,
    VK_H                               = 0x48,
    VK_I                               = 0x49,
    VK_J                               = 0x4A,
    VK_K                               = 0x4B,
    VK_L                               = 0x4C,
    VK_M                               = 0x4D,
    VK_N                               = 0x4E,
    VK_O                               = 0x4F,
    VK_P                               = 0x50,
    VK_Q                               = 0x51,
    VK_R                               = 0x52,
    VK_S                               = 0x53,
    VK_T                               = 0x54,
    VK_U                               = 0x55,
    VK_V                               = 0x56,
    VK_W                               = 0x57,
    VK_X                               = 0x58,
    VK_Y                               = 0x59,
    VK_Z                               = 0x5A,
    VK_LWIN                            = 0x5B,
    VK_RWIN                            = 0x5C,
    VK_APPS                            = 0x5D,
    VK_SLEEP                           = 0x5F,
    VK_NUMPAD0                         = 0x60,
    VK_NUMPAD1                         = 0x61,
    VK_NUMPAD2                         = 0x62,
    VK_NUMPAD3                         = 0x63,
    VK_NUMPAD4                         = 0x64,
    VK_NUMPAD5                         = 0x65,
    VK_NUMPAD6                         = 0x66,
    VK_NUMPAD7                         = 0x67,
    VK_NUMPAD8                         = 0x68,
    VK_NUMPAD9                         = 0x69,
    VK_MULTIPLY                        = 0x6A,
    VK_ADD                             = 0x6B,
    VK_SEPARATOR                       = 0x6C,
    VK_SUBTRACT                        = 0x6D,
    VK_DECIMAL                         = 0x6E,
    VK_DIVIDE                          = 0x6F,
    VK_F1                              = 0x70,
    VK_F2                              = 0x71,
    VK_F3                              = 0x72,
    VK_F4                              = 0x73,
    VK_F5                              = 0x74,
    VK_F6                              = 0x75,
    VK_F7                              = 0x76,
    VK_F8                              = 0x77,
    VK_F9                              = 0x78,
    VK_F10                             = 0x79,
    VK_F11                             = 0x7A,
    VK_F12                             = 0x7B,
    VK_F13                             = 0x7C,
    VK_F14                             = 0x7D,
    VK_F15                             = 0x7E,
    VK_F16                             = 0x7F,
    VK_F17                             = 0x80,
    VK_F18                             = 0x81,
    VK_F19                             = 0x82,
    VK_F20                             = 0x83,
    VK_F21                             = 0x84,
    VK_F22                             = 0x85,
    VK_F23                             = 0x86,
    VK_F24                             = 0x87,
    VK_NAVIGATION_VIEW                 = 0x88,
    VK_NAVIGATION_MENU                 = 0x89,
    VK_NAVIGATION_UP                   = 0x8A,
    VK_NAVIGATION_DOWN                 = 0x8B,
    VK_NAVIGATION_LEFT                 = 0x8C,
    VK_NAVIGATION_RIGHT                = 0x8D,
    VK_NAVIGATION_ACCEPT               = 0x8E,
    VK_NAVIGATION_CANCEL               = 0x8F,
    VK_NUMLOCK                         = 0x90,
    VK_SCROLL                          = 0x91,
    VK_OEM_NEC_EQUAL                   = 0x92,
    VK_OEM_FJ_JISHO                    = 0x92,
    VK_OEM_FJ_MASSHOU                  = 0x93,
    VK_OEM_FJ_TOUROKU                  = 0x94,
    VK_OEM_FJ_LOYA                     = 0x95,
    VK_OEM_FJ_ROYA                     = 0x96,
    VK_LSHIFT                          = 0xA0,
    VK_RSHIFT                          = 0xA1,
    VK_LCONTROL                        = 0xA2,
    VK_RCONTROL                        = 0xA3,
    VK_LMENU                           = 0xA4,
    VK_RMENU                           = 0xA5,
    VK_BROWSER_BACK                    = 0xA6,
    VK_BROWSER_FORWARD                 = 0xA7,
    VK_BROWSER_REFRESH                 = 0xA8,
    VK_BROWSER_STOP                    = 0xA9,
    VK_BROWSER_SEARCH                  = 0xAA,
    VK_BROWSER_FAVORITES               = 0xAB,
    VK_BROWSER_HOME                    = 0xAC,
    VK_VOLUME_MUTE                     = 0xAD,
    VK_VOLUME_DOWN                     = 0xAE,
    VK_VOLUME_UP                       = 0xAF,
    VK_MEDIA_NEXT_TRACK                = 0xB0,
    VK_MEDIA_PREV_TRACK                = 0xB1,
    VK_MEDIA_STOP                      = 0xB2,
    VK_MEDIA_PLAY_PAUSE                = 0xB3,
    VK_LAUNCH_MAIL                     = 0xB4,
    VK_LAUNCH_MEDIA_SELECT             = 0xB5,
    VK_LAUNCH_APP1                     = 0xB6,
    VK_LAUNCH_APP2                     = 0xB7,
    VK_OEM_1                           = 0xBA,
    VK_OEM_PLUS                        = 0xBB,
    VK_OEM_COMMA                       = 0xBC,
    VK_OEM_MINUS                       = 0xBD,
    VK_OEM_PERIOD                      = 0xBE,
    VK_OEM_2                           = 0xBF,
    VK_OEM_3                           = 0xC0,
    VK_GAMEPAD_A                       = 0xC3,
    VK_GAMEPAD_B                       = 0xC4,
    VK_GAMEPAD_X                       = 0xC5,
    VK_GAMEPAD_Y                       = 0xC6,
    VK_GAMEPAD_RIGHT_SHOULDER          = 0xC7,
    VK_GAMEPAD_LEFT_SHOULDER           = 0xC8,
    VK_GAMEPAD_LEFT_TRIGGER            = 0xC9,
    VK_GAMEPAD_RIGHT_TRIGGER           = 0xCA,
    VK_GAMEPAD_DPAD_UP                 = 0xCB,
    VK_GAMEPAD_DPAD_DOWN               = 0xCC,
    VK_GAMEPAD_DPAD_LEFT               = 0xCD,
    VK_GAMEPAD_DPAD_RIGHT              = 0xCE,
    VK_GAMEPAD_MENU                    = 0xCF,
    VK_GAMEPAD_VIEW                    = 0xD0,
    VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON  = 0xD1,
    VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2,
    VK_GAMEPAD_LEFT_THUMBSTICK_UP      = 0xD3,
    VK_GAMEPAD_LEFT_THUMBSTICK_DOWN    = 0xD4,
    VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT   = 0xD5,
    VK_GAMEPAD_LEFT_THUMBSTICK_LEFT    = 0xD6,
    VK_GAMEPAD_RIGHT_THUMBSTICK_UP     = 0xD7,
    VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN   = 0xD8,
    VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT  = 0xD9,
    VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT   = 0xDA,
    VK_OEM_4                           = 0xDB,
    VK_OEM_5                           = 0xDC,
    VK_OEM_6                           = 0xDD,
    VK_OEM_7                           = 0xDE,
    VK_OEM_8                           = 0xDF,
    VK_OEM_AX                          = 0xE1,
    VK_OEM_102                         = 0xE2,
    VK_ICO_HELP                        = 0xE3,
    VK_ICO_00                          = 0xE4,
    VK_PROCESSKEY                      = 0xE5,
    VK_ICO_CLEAR                       = 0xE6,
    VK_PACKET                          = 0xE7,
    VK_OEM_RESET                       = 0xE9,
    VK_OEM_JUMP                        = 0xEA,
    VK_OEM_PA1                         = 0xEB,
    VK_OEM_PA2                         = 0xEC,
    VK_OEM_PA3                         = 0xED,
    VK_OEM_WSCTRL                      = 0xEE,
    VK_OEM_CUSEL                       = 0xEF,
    VK_OEM_ATTN                        = 0xF0,
    VK_OEM_FINISH                      = 0xF1,
    VK_OEM_COPY                        = 0xF2,
    VK_OEM_AUTO                        = 0xF3,
    VK_OEM_ENLW                        = 0xF4,
    VK_OEM_BACKTAB                     = 0xF5,
    VK_ATTN                            = 0xF6,
    VK_CRSEL                           = 0xF7,
    VK_EXSEL                           = 0xF8,
    VK_EREOF                           = 0xF9,
    VK_PLAY                            = 0xFA,
    VK_ZOOM                            = 0xFB,
    VK_NONAME                          = 0xFC,
    VK_PA1                             = 0xFD,
    VK_OEM_CLEAR                       = 0xFE,
    VK_UNKNOWN                         = 0xFF,
};

struct InputDevice {
    void (*updateInput)(InputDevice *device);
    void (*processInput)(InputDevice *device, int inputID);
    int gamePadType;
    int inputID;
    byte active;
    byte assignedControllerID;
    byte field_E;
    byte field_F;
    byte anyPress;
    int inactiveTimer[2];
    int field_1C;
    byte controllerID;
};

struct InputDeviceBase : InputDevice {
    byte data[0x3DC];
};

#if RETRO_USING_SDL2
struct InputDeviceSDL : InputDevice {
    SDL_GameController *controllerPtr;
};
#endif

struct InputState {
    bool32 down;
    bool32 press;
    int keyMap;

    inline void setHeld()
    {
        press = !down;
        down  = true;
    }
    inline void setReleased()
    {
        press = false;
        down  = false;
    }
};

struct ControllerState {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyA;
    InputState keyB;
    InputState keyC;
    InputState keyX;
    InputState keyY;
    InputState keyZ;
    InputState keyStart;
    InputState keySelect;
};

struct AnalogState {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyStick;
    float deadzone;
    float hDelta;
    float vDelta;
};

struct TriggerState {
    InputState keyL;
    InputState keyR;
    float deadzone;
    float delta;
};

struct TouchMouseData {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    byte count;
};

#if RETRO_USING_SDL2
struct InputManagerInfo {
    int mouseHideTimer = 0;
    bool32 keyPress[PLAYER_COUNT][12];
    bool32 anyPress;
};

extern InputManagerInfo InputManager;
#endif

extern InputDeviceBase InputDevices[INPUTDEVICE_COUNT];
extern int InputDeviceCount;

extern int activeControllers[PLAYER_COUNT];
extern InputDevice *activeInputDevices[PLAYER_COUNT];

extern ControllerState controller[PLAYER_COUNT + 1];
extern AnalogState stickL[PLAYER_COUNT + 1];
extern AnalogState stickR[PLAYER_COUNT + 1];
extern TriggerState triggerL[PLAYER_COUNT + 1];
extern TriggerState triggerR[PLAYER_COUNT + 1];
extern TouchMouseData touchMouseData;

void InitInputDevice();
void ProcessInput();

void UpdateKeyboardInput(InputDevice *device);
void UpdateDeviceInput(InputDevice *device);

void ProcessKeyboardInput(InputDevice* device, int controllerID);
void ProcessDeviceInput(InputDevice* device, int controllerID);

inline int controllerUnknown2(int a2, int a3) { return 0; }
inline int controllerUnknown3(int a2, int a3) { return 0; }

#if RETRO_USING_SDL2
inline InputDevice *controllerInit(byte controllerID)
{
    if (InputDeviceCount >= INPUTDEVICE_COUNT)
        return NULL;
    InputDeviceSDL *device = (InputDeviceSDL *)&InputDevices[InputDeviceCount++];
    device->controllerPtr  = SDL_GameControllerOpen(controllerID);
    uint id;
    char buffer[0x20];
    sprintf(buffer, "%s%d", "SDLDevice", controllerID);
    GenerateCRC(&id, buffer);
    device->active       = true;
    device->field_F      = false;
    device->gamePadType  = (DEVICE_FLAG_UNKNOWN2 << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_XBOX << 0);
    device->inputID      = id;
    device->updateInput  = UpdateDeviceInput;
    device->processInput = ProcessDeviceInput;

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == (uint)id) {
            activeInputDevices[i]        = (InputDevice *)device;
            device->assignedControllerID = i + 1;
        }
    }

    return device;
};

inline void controllerClose(byte controllerID)
{
    if (controllerID >= INPUTDEVICE_COUNT || InputDeviceCount <= 0)
        return;

    uint id;
    char buffer[0x20];
    sprintf(buffer, "%s%d", "SDLDevice", controllerID);
    GenerateCRC(&id, buffer);

    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == id && InputDevices[i].active) {
            InputDeviceSDL *device = (InputDeviceSDL *)&InputDevices[i];
            device->active         = false;
            SDL_GameControllerClose(device->controllerPtr);
            device->controllerPtr = NULL;
            InputDeviceCount--;
        }
    }
}
#endif

inline InputDevice *InputDeviceFromID(int inputID)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return &InputDevices[i];
        }
    }
    return NULL;
}
inline int GetControllerInputID()
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].active && !InputDevices[i].field_F && !InputDevices[i].assignedControllerID && InputDevices[i].anyPress) {
            return InputDevices[i].inputID;
        }
    }
    return -1;
}

inline int ControllerIDForInputID(byte inputID)
{
    byte i = inputID - 1;
    if (i < PLAYER_COUNT)
        return activeControllers[i];
    return 0;
}

#if RETRO_REV02
inline int MostRecentActiveControllerID(int a1, int a2, uint a3)
{
    uint v3          = -1;
    uint recentTimer = -1;
    int inputID      = 0;
    int inputIDStore = 0;
    if (a3)
        v3 = a3;

    if (InputDeviceCount) {
        for (int i = 0; i < InputDeviceCount; ++i) {
            if (InputDevices[i].active && !InputDevices[i].field_F && (!InputDevices[i].assignedControllerID || !a2)) {
                if (InputDevices[i].inactiveTimer[a1] < recentTimer) {
                    recentTimer = InputDevices[i].inactiveTimer[a1];
                    if (InputDevices[i].inactiveTimer[a1] <= v3)
                        inputID = InputDevices[i].inputID;
                    inputIDStore = InputDevices[i].inputID;
                }
            }
        }

        if (inputID)
            return inputID;
    }
    if (inputIDStore)
        return inputIDStore;

    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].active && !InputDevices[i].field_F && (!InputDevices[i].assignedControllerID || !a2)) {
            return InputDevices[i].inputID;
        }
    }

    return inputIDStore;
}
#else
inline int MostRecentActiveControllerID(int a2)
{
    uint v3          = -1;
    uint recentTimer = -1;
    int inputID      = 0;
    int inputIDStore = 0;

    if (InputDeviceCount) {
        for (int i = 0; i < InputDeviceCount; ++i) {
            if (InputDevices[i].active && !InputDevices[i].field_F && (!InputDevices[i].assignedControllerID || !a2)) {
                if (InputDevices[i].inactiveTimer[0] < recentTimer) {
                    recentTimer = InputDevices[i].inactiveTimer[0];
                    if (InputDevices[i].inactiveTimer[0] <= v3)
                        inputID = InputDevices[i].inputID;
                    inputIDStore = InputDevices[i].inputID;
                }
            }
        }

        if (inputID)
            return inputID;
    }
    if (inputIDStore)
        return inputIDStore;

    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].active && !InputDevices[i].field_F && (!InputDevices[i].assignedControllerID || !a2)) {
            return InputDevices[i].inputID;
        }
    }

    return inputIDStore;
}
#endif

int GetGamePadType(int inputID);

inline int GetAssignedControllerID(int inputID)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return InputDevices[i].assignedControllerID;
        }
    }

    return 0;
}

inline int GetAssignedUnknown(int inputID)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            break; // what
        }
    }

    return 0xFFFF;
}

inline int DoInputUnknown2(int inputID, int a2, int a3)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return controllerUnknown2(a2, a3);
        }
    }

    return 0;
}

inline int DoInputUnknown3(int inputID, int a2, int a3)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return controllerUnknown3(a2, a3);
        }
    }

    return 0;
}

inline int Missing24() { return 0xFFFF; }

inline int DoInputUnknown2_Active(int inputID, int a2, int a3)
{
    if (inputID < PLAYER_COUNT) {
        if (activeControllers[inputID]) {
            //return activeInputDevices[inputID]->controllerUnknown2(a2, a3);
        }
    }

    return 0;
}

inline int DoInputUnknown3_Active(int inputID, int a2, int a3)
{
    if (inputID < PLAYER_COUNT) {
        if (activeControllers[inputID]) {
            //return activeInputDevices[inputID]->controllerUnknown3(a2, a3);
        }
    }

    return 0;
}

inline void AssignControllerID(sbyte controllerID, int inputID)
{
    int contID = controllerID - 1;
    if (contID < PLAYER_COUNT) {
        if (inputID && inputID != CONT_AUTOASSIGN) {
            if (inputID == CONT_UNASSIGNED) {
                activeControllers[contID] = CONT_UNASSIGNED;
            }
            else {
                for (int i = 0; i < InputDeviceCount; ++i) {
                    if (InputDevices[i].inputID == inputID) {
                        InputDevices[i].assignedControllerID = true;
                        activeControllers[contID]          = inputID;
                        activeInputDevices[contID]         = &InputDevices[i];
                        break;
                    }
                }
            }
        }
        else {
            InputDevice *device = InputDeviceFromID(activeControllers[contID]);
            if (device)
                device->assignedControllerID = false;
            activeControllers[contID] = inputID;
        }
    }
}

inline bool32 InputIDIsDisconnected(byte inputID)
{
    if (inputID < PLAYER_COUNT)
        return activeControllers[inputID] != CONT_ANY;
    return false;
}

inline void ResetControllerAssignments()
{
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        activeControllers[i]  = CONT_ANY;
        activeInputDevices[i] = NULL;
    }

    for (int i = 0; i < InputDeviceCount; ++i) {
        InputDevices[i].assignedControllerID = false;
    }
}

inline void SetInputLEDColor() {
    //empty
}

#endif
