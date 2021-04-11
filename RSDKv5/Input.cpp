#include "RetroEngine.hpp"

#if RETRO_USING_SDL2
InputDevice inputDevice;
#endif

ControllerState controller[PLAYER_COUNT];
AnalogState stickL[PLAYER_COUNT];
AnalogState stickR[PLAYER_COUNT];
TriggerState triggerL[PLAYER_COUNT];
TriggerState triggerR[PLAYER_COUNT];
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

    for (int c = 0; c < PLAYER_COUNT; ++c) {
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

        if (inputDevice.inputType[c] == 0) {
            for (int i = 0; i < KEY_MAX; i++) {
                if (keyState[buttons[i]->keyMap]) {
                    buttons[i]->setHeld();
                }
                else if (buttons[i]->down) {
                    buttons[i]->setReleased();
                }
            }
        }
        else if (inputDevice.inputType[c] == 1) {
            for (int i = 0; i < KEY_MAX; i++) {
                if (getControllerButton(c, buttonMap[i])) {
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
            inputDevice.inputType[c] = 0;

        isPressed = false;
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
            if (getControllerButton(c, i)) {
                isPressed = true;
                break;
            }
        }
        if (isPressed)
            inputDevice.inputType[c] = 1;

        for (int i = 0; i < keyCount && !anyPress; i++) {
            if (keyState[i]) {
                anyPress = true;
            }
        }
        for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX && !anyPress; i++) {
            if (getControllerButton(c, i)) {
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

    int mx = 0, my = 0;
    SDL_GetMouseState(&mx, &my);

    if (mx == touchMouseData.x[0] * engine.windowWidth && my == touchMouseData.y[0] * engine.windowHeight) {
        ++mouseHideTimer;
        if (mouseHideTimer >= 120) {
            mouseHideTimer = 0;
            SDL_ShowCursor(false);
        }
    }
    else if (mouseHideTimer >= 120) {
        mouseHideTimer = 0;
        SDL_ShowCursor(true);
    }

    if (touchMouseData.count)
        engine.dimTimer = 0;
#endif
}