#ifndef INPUT_H
#define INPUT_H

#define PLAYER_COUNT (4)

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
    InputState key1;
    InputState key2;
    float unknown1;
    float unknown2;
};

struct TouchMouseData {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    byte count;
};

#if RETRO_USING_SDL2
struct InputDevice {
    int inputType[PLAYER_COUNT];

    int LSTICK_DEADZONE[PLAYER_COUNT];
    int RSTICK_DEADZONE[PLAYER_COUNT];
    int LTRIGGER_DEADZONE[PLAYER_COUNT];
    int RTRIGGER_DEADZONE[PLAYER_COUNT];

    SDL_GameController *controllers[PLAYER_COUNT];

    int mouseHideTimer = 0;

    void ProcessInput();
};

extern InputDevice inputDevice;

inline void controllerInit(byte controllerID)
{
    inputDevice.inputType[controllerID]  = 1;
    inputDevice.controllers[controllerID] = SDL_GameControllerOpen(controllerID);
};

inline void controllerClose(byte controllerID)
{
    if (controllerID >= PLAYER_COUNT)
        return;
    inputDevice.inputType[controllerID] = 0;
}
#endif

extern sbyte activeControllers[PLAYER_COUNT];
extern InputDevice *activeInputDevices[PLAYER_COUNT];

extern ControllerState controller[PLAYER_COUNT + 1];
extern AnalogState stickL[PLAYER_COUNT + 1];
extern AnalogState stickR[PLAYER_COUNT + 1];
extern TriggerState triggerL[PLAYER_COUNT + 1];
extern TriggerState triggerR[PLAYER_COUNT + 1];
extern TouchMouseData touchMouseData;

inline int Missing24() { return 0xFFFF; }

#endif
