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

struct InputDevice {
    int gamePadType;
    int inputID;
    byte active;
    byte assignedControllerID;
    byte field_E;
    byte field_F;
    byte field_10;
    byte field_11;
    byte field_12;
    byte field_13;
    int field_14[4];
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

extern uint activeControllers[PLAYER_COUNT];
extern InputDevice *activeInputDevices[PLAYER_COUNT];

extern ControllerState controller[PLAYER_COUNT + 1];
extern AnalogState stickL[PLAYER_COUNT + 1];
extern AnalogState stickR[PLAYER_COUNT + 1];
extern TriggerState triggerL[PLAYER_COUNT + 1];
extern TriggerState triggerR[PLAYER_COUNT + 1];
extern TouchMouseData touchMouseData;

#if RETRO_USING_SDL2
inline InputDevice* controllerInit(byte controllerID)
{
    if (InputDeviceCount >= INPUTDEVICE_COUNT)
        return NULL;
    InputDeviceSDL *device = (InputDeviceSDL *)&InputDevices[InputDeviceCount++];
    device->controllerPtr  = SDL_GameControllerOpen(controllerID);
    uint id;
    char buffer[0x20];
    sprintf(buffer, "%s%d", "SDLDevice", controllerID);
    GenerateCRC(&id, buffer);
    device->active         = true;
    device->field_F        = 0;
    device->gamePadType    = 0x00000;
    device->inputID        = id;

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
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

void InitInputDevice();
void ProcessInput();
void ProcessDeviceInput(InputDevice* device, int controllerID);

inline int controllerUnknown2(int a2, int a3) { return 0; }
inline int controllerUnknown3(int a2, int a3) { return 0; }

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
        if (InputDevices[i].active && !InputDevices[i].field_F && !InputDevices[i].assignedControllerID && !InputDevices[i].field_10) {
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

inline int MostRecentActiveControllerID(int a1, int a2, uint a3)
{
    int v3           = -1;
    int v4           = -1;
    int inputID      = 0;
    int inputIDStore = 0;
    if (a3)
        v3 = a3;

    if (InputDeviceCount) {
        for (int i = 0; i < InputDeviceCount; ++i) {
            if (InputDevices[i].active && !InputDevices[i].field_F && (InputDevices[i].assignedControllerID != 1 || a2 != 1)) {
                if (InputDevices[i].field_14[a1] < v4) {
                    v4 = InputDevices[i].field_14[a1];
                    if (InputDevices[i].field_14[a1] <= v3)
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
        if (InputDevices[i].active && !InputDevices[i].field_F && (InputDevices[i].assignedControllerID != 1 || a2 != 1)) {
            return InputDevices[i].inputID;
        }
    }

    return inputIDStore;
}

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
    if (controllerID < PLAYER_COUNT) {
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
