#include "RetroEngine.hpp"

#if RETRO_USING_SDL2
InputManagerInfo InputManager;
#endif

InputDevice InputDevices[INPUTDEVICE_COUNT];
int InputDeviceCount = 0;

sbyte activeControllers[PLAYER_COUNT];
InputDevice *activeInputDevices[PLAYER_COUNT];

ControllerState controller[PLAYER_COUNT + 1];
AnalogState stickL[PLAYER_COUNT + 1];
AnalogState stickR[PLAYER_COUNT + 1];
TriggerState triggerL[PLAYER_COUNT + 1];
TriggerState triggerR[PLAYER_COUNT + 1];
TouchMouseData touchMouseData;

#if RETRO_USING_SDL2
bool32 getControllerButton(byte deviceID, byte buttonID)
{
    if (SDL_GameControllerGetButton(InputManager.controllers[deviceID], (SDL_GameControllerButton)buttonID)) {
        return true;
    }

    return false;
}
#endif

void InputManagerInfo::ProcessInput()
{
#if RETRO_USING_SDL2
    int keyCount         = 0;
    const byte *keyState = SDL_GetKeyboardState(&keyCount);

    bool32 anyPress = false;
    bool32 keyPress[PLAYER_COUNT];

    for (int c = 1; c < PLAYER_COUNT + 1; ++c) {
        keyPress[c - 1] = false;
        InputState *buttons[] = {
            &controller[c].keyUp, &controller[c].keyDown, &controller[c].keyLeft,  &controller[c].keyRight,
            &controller[c].keyA,  &controller[c].keyB,    &controller[c].keyC,     &controller[c].keyX,
            &controller[c].keyY,  &controller[c].keyZ,    &controller[c].keyStart, &controller[c].keySelect,
        };

        for (int i = 0; i < KEY_MAX; i++) {
            if (keyState[buttons[i]->keyMap]) {
                buttons[i]->setHeld();
                keyPress[c - 1] = true;
            }
            else if (buttons[i]->down) {
                buttons[i]->setReleased();
            }
        }

        for (int i = 0; i < keyCount && !anyPress; i++) {
            if (keyState[i]) {
                anyPress = true;
            }
        }
    }

    for (int c = 0; c < InputDeviceCount; ++c) {
        InputDevice *device = &InputDevices[c];

        InputState *buttons[] = {
            &controller[device->inputID].keyUp, &controller[device->inputID].keyDown, &controller[device->inputID].keyLeft,  &controller[device->inputID].keyRight,
            &controller[device->inputID].keyA,  &controller[device->inputID].keyB,    &controller[device->inputID].keyC,     &controller[device->inputID].keyX,
            &controller[device->inputID].keyY,  &controller[device->inputID].keyZ,    &controller[device->inputID].keyStart, &controller[device->inputID].keySelect,
        };
        int buttonMap[] = {
            SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
            SDL_CONTROLLER_BUTTON_A,       SDL_CONTROLLER_BUTTON_B,         SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_X,
            SDL_CONTROLLER_BUTTON_Y,       SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_START,     SDL_CONTROLLER_BUTTON_GUIDE,
        };

        //dont update buttons from controllers if a key was pressed
        for (int i = 0; i < KEY_MAX && !keyPress[device->inputID]; i++) {
            if (getControllerButton(c, buttonMap[i])) {
                buttons[i]->setHeld();
            }
            else if (buttons[i]->down) {
                buttons[i]->setReleased();
            }
        }

        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX && !anyPress; i++) {
            if (getControllerButton(c, i)) {
                anyPress = true;
            }
        }
    }

    if (anyPress) {
        engine.dimTimer = 0;
    }
    else if (engine.dimTimer < engine.dimLimit) {
        ++engine.dimTimer;
    }

#ifdef RETRO_USING_MOUSE
#if RETRO_USING_SDL2
    if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                         //! RETRO_USING_SDL2
        int mx = 0, my = 0;
        SDL_GetMouseState(&mx, &my);

        if ((mx == touchMouseData.x[0] * engine.windowWidth && my == touchMouseData.y[0] * engine.windowHeight)) {
            ++InputManager.mouseHideTimer;
            if (InputManager.mouseHideTimer == 120) {
                SDL_ShowCursor(false);
            }
        }
        else {
            if (InputManager.mouseHideTimer >= 120)
                SDL_ShowCursor(true);
            InputManager.mouseHideTimer = 0;
        }
        touchMouseData.x[0] = mx / (float)engine.windowWidth;
        touchMouseData.y[0] = my / (float)engine.windowHeight;
        // touchMouseData.count = 1;
#if RETRO_USING_SDL2
    }
#endif //! RETRO_USING_SDL2
#endif //! RETRO_USING_MOUSE

    if (touchMouseData.count)
        engine.dimTimer = 0;
#endif
}

void InitInputDevice()
{
    char buffer[0x100];
#if RETRO_USING_SDL2
    // Support for extra controller types SDL doesn't recognise
#if RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP
    //if (!usingCWD)
    //    sprintf(buffer, "%s/controllerdb.txt", getResourcesPath());
    //else
    //    sprintf(buffer, "%scontrollerdb.txt", gamePath);
    sprintf(buffer, BASE_PATH "controllerdb.txt");
#else
    sprintf(buffer, BASE_PATH "controllerdb.txt");
#endif
    FileIO *file = fOpen(buffer, "rb");
    if (file) {
        fClose(file);

        //SDL_GameControllerAddMapping()
        int nummaps = SDL_GameControllerAddMappingsFromFile(buffer);
        if (nummaps >= 0)
            printLog(SEVERITY_NONE, "loaded %d controller mappings from '%s'", buffer, nummaps);
    }
#endif

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        activeControllers[i]  = CONT_UNASSIGNED;
        activeInputDevices[i] = NULL;

        sprintf(buffer, "SDLInputDevice%d", i);
        //GenerateCRC(&device->inputID, buffer);
    }
}

void ProcessInput() {

    InputManager.ProcessInput();

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        int assign = activeControllers[i];
        if (assign && assign != CONT_UNASSIGNED) {
            if (assign == CONT_AUTOASSIGN) {
                int id                   = GetControllerInputID();
                activeControllers[i] = id;
                if (id < -1)
                    AssignControllerID(i + 1, id);
            }
            else {
                //InputDevice *device = activeInputDevices[i];
                //if (device && device->inputID == assign && device->active)
                //    device->GetInput(i + 1);
            }
        }
    }


    InputState *anyButtons[] = {
        &controller[0].keyUp, &controller[0].keyDown, &controller[0].keyLeft,  &controller[0].keyRight,
        &controller[0].keyA,  &controller[0].keyB,    &controller[0].keyC,     &controller[0].keyX,
        &controller[0].keyY,  &controller[0].keyZ,    &controller[0].keyStart, &controller[0].keySelect,
    };

    for (int i = 0; i < 12; ++i) {
        anyButtons[i]->press = false;
        anyButtons[i]->down  = false;
    }
    for (int c = 1; c < PLAYER_COUNT + 1; ++c) {
        InputState *buttons[] = {
            &controller[c].keyUp, &controller[c].keyDown, &controller[c].keyLeft,  &controller[c].keyRight,
            &controller[c].keyA,  &controller[c].keyB,    &controller[c].keyC,     &controller[c].keyX,
            &controller[c].keyY,  &controller[c].keyZ,    &controller[c].keyStart, &controller[c].keySelect,
        };

        for (int i = 0; i < 12; ++i) {
            anyButtons[i]->press |= buttons[i]->press;
            anyButtons[i]->down |= buttons[i]->down;
        }
    }
}

int Unknown100(int inputID)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return InputDevices[i].activeInputIDs;
        }
    }

    return userCore->controllerUnknown();
}
