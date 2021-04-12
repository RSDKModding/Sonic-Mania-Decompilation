#include "RetroEngine.hpp"

#if RETRO_USING_SDL2
InputDevice inputDevice;
#endif

sbyte activeControllers[PLAYER_COUNT];
InputDevice *activeInputDevices[PLAYER_COUNT];

ControllerState controller[PLAYER_COUNT + 1];
AnalogState stickL[PLAYER_COUNT + 1];
AnalogState stickR[PLAYER_COUNT + 1];
TriggerState triggerL[PLAYER_COUNT + 1];
TriggerState triggerR[PLAYER_COUNT + 1];
TouchMouseData touchMouseData;

#if RETRO_USING_SDL2
bool32 getControllerButton(byte inputID, byte buttonID)
{
    if (SDL_GameControllerGetButton(inputDevice.controllers[inputID], (SDL_GameControllerButton)buttonID)) {
        return true;
    }

    //switch (buttonID) {
    //    default: break;
    //    case SDL_CONTROLLER_BUTTON_ZL: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_TRIGGERLEFT) > inputDevice.LTRIGGER_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_ZR: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > inputDevice.RTRIGGER_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_LSTICK_UP: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_LEFTY) < -inputDevice.LSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_LSTICK_DOWN: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_LEFTY) > inputDevice.LSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_LSTICK_LEFT: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_LEFTX) < -inputDevice.LSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_LSTICK_RIGHT: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_LEFTX) > inputDevice.LSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_RSTICK_UP: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_RIGHTY) < -inputDevice.RSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_RSTICK_DOWN: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_RIGHTY) > inputDevice.RSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_RSTICK_LEFT: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_RIGHTX) < -inputDevice.RSTICK_DEADZONE[inputID];
    //    case SDL_CONTROLLER_BUTTON_RSTICK_RIGHT: return SDL_GameControllerGetAxis(inputDevice.controllers[inputID], SDL_CONTROLLER_AXIS_RIGHTX) > inputDevice.RSTICK_DEADZONE[inputID];
    //}

    return false;
}
#endif

void InputDevice::ProcessInput()
{
#if RETRO_USING_SDL2
    int keyCount         = 0;
    const byte *keyState = SDL_GetKeyboardState(&keyCount);

    bool32 anyPress = false;

    InputState *anyButtons[] = {
        &controller[0].keyUp, &controller[0].keyDown, &controller[0].keyLeft,  &controller[0].keyRight,
        &controller[0].keyA,  &controller[0].keyB,    &controller[0].keyC,     &controller[0].keyX,
        &controller[0].keyY,  &controller[0].keyZ,    &controller[0].keyStart, &controller[0].keySelect,
    };

    for (int c = 1; c < PLAYER_COUNT + 1; ++c) {
        InputState *buttons[] = {
            &controller[c].keyUp, &controller[c].keyDown, &controller[c].keyLeft,  &controller[c].keyRight,
            &controller[c].keyA,  &controller[c].keyB,    &controller[c].keyC,     &controller[c].keyX,
            &controller[c].keyY,  &controller[c].keyZ,    &controller[c].keyStart, &controller[c].keySelect,
        };
        int buttonMap[] = {
            SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
            SDL_CONTROLLER_BUTTON_A,       SDL_CONTROLLER_BUTTON_B,         SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_X,
            SDL_CONTROLLER_BUTTON_Y,       SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_START,     SDL_CONTROLLER_BUTTON_GUIDE,
        };

        if (inputDevice.inputType[c - 1] == 0) {
            for (int i = 0; i < KEY_MAX; i++) {
                if (keyState[buttons[i]->keyMap]) {
                    buttons[i]->setHeld();
                }
                else if (buttons[i]->down) {
                    buttons[i]->setReleased();
                }
            }
        }
        else if (inputDevice.inputType[c - 1] == 1) {
            for (int i = 0; i < KEY_MAX; i++) {
                if (getControllerButton(c - 1, buttonMap[i])) {
                    buttons[i]->setHeld();
                }
                else if (buttons[i]->down) {
                    buttons[i]->setReleased();
                }
            }
        }

        bool32 isPressed = false;
        for (int i = 0; i < KEY_MAX; i++) {
            if (keyState[buttons[i]->keyMap]) {
                isPressed = true;
                break;
            }
        }
        if (isPressed)
            inputDevice.inputType[c - 1] = 0;

        isPressed = false;
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
            if (getControllerButton(c - 1, i)) {
                isPressed = true;
                break;
            }
        }
        if (isPressed)
            inputDevice.inputType[c - 1] = 1;

        for (int i = 0; i < keyCount && !anyPress; i++) {
            if (keyState[i]) {
                anyPress = true;
            }
        }
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX && !anyPress; i++) {
            if (getControllerButton(c - 1, i)) {
                anyPress = true;
            }
        }

        if (anyPress) {
            engine.dimTimer = 0;
        }
        else if (engine.dimTimer < engine.dimLimit) {
            ++engine.dimTimer;
        }
    }

    for (int i = 0; i < 12; ++i) {
        anyButtons[i]->press = false;
        anyButtons[i]->down = false;
    }
    for (int c = 1; c < PLAYER_COUNT + 1; ++c) {
        InputState *buttons[] = {
            &controller[c].keyUp, &controller[c].keyDown, &controller[c].keyLeft,  &controller[c].keyRight,
            &controller[c].keyA,  &controller[c].keyB,    &controller[c].keyC,     &controller[c].keyX,
            &controller[c].keyY,  &controller[c].keyZ,    &controller[c].keyStart, &controller[c].keySelect,
        };

        for (int i = 0; i < 12; ++i) {
            anyButtons[i]->press |= buttons[i]->press;
            anyButtons[i]->down  |= buttons[i]->down;
        }
    }


#ifdef RETRO_USING_MOUSE
#if RETRO_USING_SDL2
    if (SDL_GetNumTouchFingers(SDL_GetTouchDevice(RETRO_TOUCH_DEVICE)) <= 0) { // Touch always takes priority over mouse
#endif                                                                         //! RETRO_USING_SDL2
        int mx = 0, my = 0;
        SDL_GetMouseState(&mx, &my);

        if ((mx == touchMouseData.x[0] * engine.windowWidth && my == touchMouseData.y[0] * engine.windowHeight)) {
            ++inputDevice.mouseHideTimer;
            if (inputDevice.mouseHideTimer == 120) {
                SDL_ShowCursor(false);
            }
        }
        else {
            if (inputDevice.mouseHideTimer >= 120)
                SDL_ShowCursor(true);
            inputDevice.mouseHideTimer = 0;
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
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        activeControllers[i]  = CONT_UNASSIGNED;
        activeInputDevices[i] = NULL;
    }
}

void ProcessInput() {

    inputDevice.ProcessInput();

}