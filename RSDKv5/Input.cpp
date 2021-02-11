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
bool getControllerButton(byte inputID, byte buttonID)
{
    if (SDL_GameControllerGetButton(inputDevice.controllers[inputID], (SDL_GameControllerButton)buttonID)) {
        return true;
    }

    //switch (buttonID) {
    //    default: break;
    //    case SDL_CONTROLLER_BUTTON_ZL: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > LTRIGGER_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_ZR: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > RTRIGGER_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_LSTICK_UP: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -LSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_LSTICK_DOWN: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > LSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_LSTICK_LEFT: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -LSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_LSTICK_RIGHT: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > LSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_RSTICK_UP: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -RSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_RSTICK_DOWN: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > RSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_RSTICK_LEFT: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) < -RSTICK_DEADZONE;
    //    case SDL_CONTROLLER_BUTTON_RSTICK_RIGHT: return SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > RSTICK_DEADZONE;
    //}

    return false;
}
#endif

void InputDevice::ProcessInput()
{
#if RETRO_USING_SDL2
    const byte *keyState = SDL_GetKeyboardState(NULL);

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
                    buttons[i]->press = !buttons[i]->down;
                    buttons[i]->down  = true;
                }
                else if (buttons[i]->down) {
                    buttons[i]->press = false;
                    buttons[i]->down  = false;
                }
            }
        }
        else if (inputDevice.inputType[c] == 1) {
            for (int i = 0; i < KEY_MAX; i++) {
                if (getControllerButton(c, buttonMap[i])) {
                    buttons[i]->press = !buttons[i]->down;
                    buttons[i]->down  = true;
                }
                else if (buttons[i]->down) {
                    buttons[i]->press = false;
                    buttons[i]->down  = false;
                }
            }
        }

        bool isPressed = false;
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
    }
#endif
}