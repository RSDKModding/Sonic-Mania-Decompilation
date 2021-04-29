#include "RetroEngine.hpp"

#if RETRO_USING_SDL2
InputManagerInfo InputManager;
#endif

InputDeviceBase InputDevices[INPUTDEVICE_COUNT];
int InputDeviceCount = 0;

uint activeControllers[PLAYER_COUNT];
InputDevice *activeInputDevices[PLAYER_COUNT];

ControllerState controller[PLAYER_COUNT + 1];
AnalogState stickL[PLAYER_COUNT + 1];
AnalogState stickR[PLAYER_COUNT + 1];
TriggerState triggerL[PLAYER_COUNT + 1];
TriggerState triggerR[PLAYER_COUNT + 1];
TouchMouseData touchMouseData;

#define normalize(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

#if RETRO_USING_SDL2
bool32 getControllerButton(InputDevice* device, byte buttonID)
{
    if (buttonID == SDL_CONTROLLER_BUTTON_INVALID || !device)
        return false;

    if (SDL_GameControllerGetButton(((InputDeviceSDL *)device)->controllerPtr, (SDL_GameControllerButton)buttonID)) {
        return true;
    }

    return false;
}
#endif

void ProcessKeyboardInput(int controllerID)
{
#if RETRO_USING_SDL2
    int keyCount         = 0;
    const byte *keyState = SDL_GetKeyboardState(&keyCount);

    for (int i = 0; i < 12; ++i) InputManager.keyPress[controllerID - 1][i] = false;

    InputState *buttons[] = {
        &controller[controllerID].keyUp, &controller[controllerID].keyDown, &controller[controllerID].keyLeft,  &controller[controllerID].keyRight,
        &controller[controllerID].keyA,  &controller[controllerID].keyB,    &controller[controllerID].keyC,     &controller[controllerID].keyX,
        &controller[controllerID].keyY,  &controller[controllerID].keyZ,    &controller[controllerID].keyStart, &controller[controllerID].keySelect,
    };

    for (int i = 0; i < KEY_MAX; i++) {
        if (keyState[buttons[i]->keyMap]) {
            InputManager.keyPress[controllerID - 1][i] = true;
            buttons[i]->setHeld();
        }
    }

    for (int i = 0; i < keyCount && !InputManager.anyPress; i++) {
        if (keyState[i]) {
            InputManager.anyPress = true;
        }
    }
#endif
}
void ProcessDeviceInput(InputDevice *device, int controllerID)
{
#if RETRO_USING_SDL2
    InputState *buttons[] = {
        &controller[controllerID].keyUp, &controller[controllerID].keyDown, &controller[controllerID].keyLeft,  &controller[controllerID].keyRight,
        &controller[controllerID].keyA,  &controller[controllerID].keyB,    &controller[controllerID].keyC,     &controller[controllerID].keyX,
        &controller[controllerID].keyY,  &controller[controllerID].keyZ,    &controller[controllerID].keyStart, &controller[controllerID].keySelect,
    };
    int buttonMap[] = {
        SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_CONTROLLER_BUTTON_A,       SDL_CONTROLLER_BUTTON_B,         SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_Y,       SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_START,     SDL_CONTROLLER_BUTTON_GUIDE,
    };

    // dont update buttons from controllers if a key was pressed
    bool32 keyboardInput = false;
    for (int i = 0; i < 12; ++i) keyboardInput |= InputManager.keyPress[controllerID - 1][i];

    for (int i = 0; i < KEY_MAX && !keyboardInput; ++i) {
        if (getControllerButton(device, buttonMap[i])) {
            buttons[i]->setHeld();
            InputManager.keyPress[controllerID - 1][i] = true;
        }
    }

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_LEFTSTICK)) {
        stickL[controllerID].keyStick.setHeld();
    }
    else {
        stickL[controllerID].keyStick.setReleased();
    }

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_RIGHTSTICK)) {
        stickR[controllerID].keyStick.setHeld();
    }
    else {
        stickR[controllerID].keyStick.setReleased();
    }

    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX && !InputManager.anyPress; ++i) {
        if (getControllerButton(device, i)) {
            InputManager.anyPress = true;
        }
    }

    InputDeviceSDL *sdlDevice    = (InputDeviceSDL *)device;
    int delta                 = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_LEFTX);
    if (delta < 0)
        stickL[controllerID].hDelta = -normalize(-delta, 1, 32768);
    else
        stickL[controllerID].hDelta = normalize(delta, 0, 32767);

    delta = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_LEFTY);
    if (delta < 0)
        stickL[controllerID].vDelta = -normalize(-delta, 1, 32768);
    else
        stickL[controllerID].vDelta = normalize(delta, 0, 32767);
    stickL[controllerID].vDelta = -stickL[controllerID].vDelta;
    
    delta = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_RIGHTX);
    if (delta < 0)
        stickR[controllerID].hDelta = -normalize(-delta, 1, 32768);
    else
        stickR[controllerID].hDelta = normalize(delta, 0, 32767);

    delta = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_RIGHTY);
    if (delta < 0)
        stickR[controllerID].vDelta = -normalize(-delta, 1, 32768);
    else
        stickR[controllerID].vDelta = normalize(delta, 0, 32767);
    stickR[controllerID].vDelta = -stickR[controllerID].vDelta;

    triggerL[controllerID].delta = normalize(SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_TRIGGERLEFT), 0, 32767);
    triggerR[controllerID].delta = normalize(SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_TRIGGERRIGHT), 0, 32767);

    if (stickL[controllerID].vDelta > stickL[controllerID].deadzone)
        stickL[controllerID].keyUp.setHeld();
    else
        stickL[controllerID].keyUp.setReleased();

    if (stickL[controllerID].vDelta < -stickL[controllerID].deadzone)
        stickL[controllerID].keyDown.setHeld();
    else
        stickL[controllerID].keyDown.setReleased();

    if (stickL[controllerID].hDelta < -stickL[controllerID].deadzone)
        stickL[controllerID].keyLeft.setHeld();
    else
        stickL[controllerID].keyLeft.setReleased();

    if (stickL[controllerID].hDelta > stickL[controllerID].deadzone)
        stickL[controllerID].keyRight.setHeld();
    else
        stickL[controllerID].keyRight.setReleased();


    if (stickR[controllerID].vDelta > stickR[controllerID].deadzone)
        stickR[controllerID].keyUp.setHeld();
    else
        stickR[controllerID].keyUp.setReleased();

    if (stickR[controllerID].vDelta < -stickR[controllerID].deadzone)
        stickR[controllerID].keyDown.setHeld();
    else
        stickR[controllerID].keyDown.setReleased();

    if (stickR[controllerID].hDelta < -stickR[controllerID].deadzone)
        stickR[controllerID].keyLeft.setHeld();
    else
        stickR[controllerID].keyLeft.setReleased();

    if (stickR[controllerID].hDelta > stickR[controllerID].deadzone)
        stickR[controllerID].keyRight.setHeld();
    else
        stickR[controllerID].keyRight.setReleased();


    if (triggerL[controllerID].delta < -triggerL[controllerID].deadzone)
        triggerL[controllerID].keyL.setHeld();
    else
        triggerL[controllerID].keyL.setReleased();

    if (triggerL[controllerID].delta > triggerL[controllerID].deadzone)
        triggerL[controllerID].keyR.setHeld();
    else
        triggerL[controllerID].keyR.setReleased();


    if (triggerR[controllerID].delta < -triggerR[controllerID].deadzone)
        triggerR[controllerID].keyL.setHeld();
    else
        triggerR[controllerID].keyL.setReleased();

    if (triggerR[controllerID].delta > triggerR[controllerID].deadzone)
        triggerR[controllerID].keyR.setHeld();
    else
        triggerR[controllerID].keyR.setReleased();
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
        activeControllers[i]  = CONT_AUTOASSIGN;
        activeInputDevices[i] = NULL;

        sprintf(buffer, "SDLDevice%d", i);
        //GenerateCRC(&device->inputID, buffer);
    }
}

void ProcessInput()
{
    InputManager.anyPress = false;

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        ProcessKeyboardInput(i + 1);

        int assign = activeControllers[i];
        if (assign && assign != CONT_UNASSIGNED) {
            if (assign == CONT_AUTOASSIGN) {
                uint id               = GetControllerInputID();
                activeControllers[i] = id;
                if (id < -1)
                    AssignControllerID(i + 1, id);
            }
            else {
                InputDevice *device = activeInputDevices[i];
                if (device && device->inputID == assign && device->active)
                    ProcessDeviceInput(device, i + 1);
            }
        }

        InputState *buttons[] = {
            &controller[i + 1].keyUp, &controller[i + 1].keyDown, &controller[i + 1].keyLeft,  &controller[i + 1].keyRight,
            &controller[i + 1].keyA,  &controller[i + 1].keyB,    &controller[i + 1].keyC,     &controller[i + 1].keyX,
            &controller[i + 1].keyY,  &controller[i + 1].keyZ,    &controller[i + 1].keyStart, &controller[i + 1].keySelect,
        };
        for (int c = 0; c < 12; ++c) {
            if (!InputManager.keyPress[i][c])
                buttons[c]->setReleased();
        }
    }


    InputState *anyController[] = {
        &controller[0].keyUp, &controller[0].keyDown, &controller[0].keyLeft,  &controller[0].keyRight,
        &controller[0].keyA,  &controller[0].keyB,    &controller[0].keyC,     &controller[0].keyX,
        &controller[0].keyY,  &controller[0].keyZ,    &controller[0].keyStart, &controller[0].keySelect,
    };
    InputState *anyLStick[] = { &stickL[0].keyUp, &stickL[0].keyDown, &stickL[0].keyLeft, &stickL[0].keyRight, &stickL[0].keyStick };
    InputState *anyRStick[] = { &stickR[0].keyUp, &stickR[0].keyDown, &stickR[0].keyLeft, &stickR[0].keyRight, &stickR[0].keyStick };

    InputState *anyLTrigger[] = { &triggerL[0].keyL, &triggerL[0].keyR };
    InputState *anyRTrigger[] = { &triggerR[0].keyL, &triggerR[0].keyR };

    for (int i = 0; i < 12; ++i) {
        anyController[i]->press = false;
        anyController[i]->down  = false;
    }

    for (int i = 0; i < 5; ++i) {
        anyLStick[i]->press = false;
        anyLStick[i]->down  = false;

        anyRStick[i]->press = false;
        anyRStick[i]->down  = false;
    }

    for (int i = 0; i < 2; ++i) {
        anyLTrigger[i]->press = false;
        anyLTrigger[i]->down  = false;

        anyRTrigger[i]->press = false;
        anyRTrigger[i]->down  = false;
    }


    for (int c = 1; c < PLAYER_COUNT + 1; ++c) {
        InputState *cont[] = {
            &controller[c].keyUp, &controller[c].keyDown, &controller[c].keyLeft,  &controller[c].keyRight,
            &controller[c].keyA,  &controller[c].keyB,    &controller[c].keyC,     &controller[c].keyX,
            &controller[c].keyY,  &controller[c].keyZ,    &controller[c].keyStart, &controller[c].keySelect,
        };
        InputState *lStick[] = { &stickL[0].keyUp, &stickL[0].keyDown, &stickL[0].keyLeft, &stickL[0].keyRight, &stickL[0].keyStick };
        InputState *rStick[] = { &stickR[0].keyUp, &stickR[0].keyDown, &stickR[0].keyLeft, &stickR[0].keyRight, &stickR[0].keyStick };

        InputState *lTrigger[] = { &triggerL[0].keyL, &triggerL[0].keyR };
        InputState *rTrigger[] = { &triggerR[0].keyL, &triggerR[0].keyR };

        for (int i = 0; i < 12; ++i) {
            anyController[i]->press |= cont[i]->press;
            anyController[i]->down |= cont[i]->down;
        }

        for (int i = 0; i < 5; ++i) {
            anyLStick[i]->press |= lStick[i]->press;
            anyLStick[i]->down |= lStick[i]->down;

            anyRStick[i]->press |= rStick[i]->press;
            anyRStick[i]->down |= rStick[i]->down;
        }

        for (int i = 0; i < 2; ++i) {
            anyLTrigger[i]->press |= lTrigger[i]->press;
            anyLTrigger[i]->down |= lTrigger[i]->down;

            anyRTrigger[i]->press |= rTrigger[i]->press;
            anyRTrigger[i]->down |= rTrigger[i]->down;
        }
    }

    if (InputManager.anyPress) {
        engine.dimTimer = 0;
    }
    else if (engine.dimTimer < engine.dimLimit) {
        ++engine.dimTimer;
    }

#if RETRO_USING_SDL2
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

int GetGamePadType(int inputID)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return InputDevices[i].gamePadType;
        }
    }

    return userCore->controllerUnknown();
}
