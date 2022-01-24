#include "RetroEngine.hpp"

#if RETRO_USING_SDL2
InputManagerInfo InputManager;
#endif

InputDeviceBase InputDevices[INPUTDEVICE_COUNT];
int32 InputDeviceCount = 0;

int32 activeControllers[PLAYER_COUNT];
InputDevice *activeInputDevices[PLAYER_COUNT];

ControllerState controller[PLAYER_COUNT + 1];
AnalogState stickL[PLAYER_COUNT + 1];
AnalogState stickR[PLAYER_COUNT + 1];
TriggerState triggerL[PLAYER_COUNT + 1];
TriggerState triggerR[PLAYER_COUNT + 1];
TouchMouseData touchMouseData;

#if !RETRO_REV02
int mostRecentControllerID = -1;
#endif

#define normalize(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

#if RETRO_USING_SDL2

int32 winAPIToSDLMappings(int32 mapping)
{
    switch (mapping) {
        default: return SDL_SCANCODE_UNKNOWN;
        case VK_LBUTTON: return SDL_SCANCODE_UNKNOWN;
        case VK_RBUTTON: return SDL_SCANCODE_UNKNOWN;
        case VK_CANCEL: return SDL_SCANCODE_CANCEL;
        case VK_MBUTTON: return SDL_SCANCODE_UNKNOWN;
        case VK_XBUTTON1: return SDL_SCANCODE_UNKNOWN;
        case VK_XBUTTON2: return SDL_SCANCODE_UNKNOWN;
        case VK_BACK: return SDL_SCANCODE_UNKNOWN;
        case VK_TAB: return SDL_SCANCODE_TAB;
        case VK_CLEAR: return SDL_SCANCODE_CLEAR;
        case VK_RETURN: return SDL_SCANCODE_RETURN;
        case VK_SHIFT: return SDL_SCANCODE_UNKNOWN;
        case VK_CONTROL: return SDL_SCANCODE_UNKNOWN;
        case VK_MENU: return SDL_SCANCODE_MENU;
        case VK_PAUSE: return SDL_SCANCODE_PAUSE;
        case VK_CAPITAL: return SDL_SCANCODE_CAPSLOCK;
        case VK_KANA: return SDL_SCANCODE_UNKNOWN;
        case VK_JUNJA: return SDL_SCANCODE_UNKNOWN;
        case VK_FINAL: return SDL_SCANCODE_UNKNOWN;
        case VK_HANJA: return SDL_SCANCODE_UNKNOWN;
        case VK_ESCAPE: return SDL_SCANCODE_ESCAPE;
        case VK_CONVERT: return SDL_SCANCODE_UNKNOWN;
        case VK_NONCONVERT: return SDL_SCANCODE_UNKNOWN;
        case VK_ACCEPT: return SDL_SCANCODE_UNKNOWN;
        case VK_MODECHANGE: return SDL_SCANCODE_UNKNOWN;
        case VK_SPACE: return SDL_SCANCODE_SPACE;
        case VK_PRIOR: return SDL_SCANCODE_PAGEUP;
        case VK_NEXT: return SDL_SCANCODE_PAGEDOWN;
        case VK_END: return SDL_SCANCODE_END;
        case VK_HOME: return SDL_SCANCODE_HOME;
        case VK_LEFT: return SDL_SCANCODE_LEFT;
        case VK_UP: return SDL_SCANCODE_UP;
        case VK_RIGHT: return SDL_SCANCODE_RIGHT;
        case VK_DOWN: return SDL_SCANCODE_DOWN;
        case VK_SELECT: return SDL_SCANCODE_SELECT;
        case VK_PRINT: return SDL_SCANCODE_UNKNOWN;
        case VK_EXECUTE: return SDL_SCANCODE_EXECUTE;
        case VK_SNAPSHOT: return SDL_SCANCODE_PRINTSCREEN;
        case VK_INSERT: return SDL_SCANCODE_INSERT;
        case VK_DELETE: return SDL_SCANCODE_DELETE;
        case VK_HELP: return SDL_SCANCODE_HELP;
        case VK_0: return SDL_SCANCODE_0;
        case VK_1: return SDL_SCANCODE_1;
        case VK_2: return SDL_SCANCODE_2;
        case VK_3: return SDL_SCANCODE_3;
        case VK_4: return SDL_SCANCODE_4;
        case VK_5: return SDL_SCANCODE_5;
        case VK_6: return SDL_SCANCODE_6;
        case VK_7: return SDL_SCANCODE_7;
        case VK_8: return SDL_SCANCODE_8;
        case VK_9: return SDL_SCANCODE_9;
        case VK_A: return SDL_SCANCODE_A;
        case VK_B: return SDL_SCANCODE_B;
        case VK_C: return SDL_SCANCODE_C;
        case VK_D: return SDL_SCANCODE_D;
        case VK_E: return SDL_SCANCODE_E;
        case VK_F: return SDL_SCANCODE_F;
        case VK_G: return SDL_SCANCODE_G;
        case VK_H: return SDL_SCANCODE_H;
        case VK_I: return SDL_SCANCODE_I;
        case VK_J: return SDL_SCANCODE_J;
        case VK_K: return SDL_SCANCODE_K;
        case VK_L: return SDL_SCANCODE_L;
        case VK_M: return SDL_SCANCODE_M;
        case VK_N: return SDL_SCANCODE_N;
        case VK_O: return SDL_SCANCODE_O;
        case VK_P: return SDL_SCANCODE_P;
        case VK_Q: return SDL_SCANCODE_Q;
        case VK_R: return SDL_SCANCODE_R;
        case VK_S: return SDL_SCANCODE_S;
        case VK_T: return SDL_SCANCODE_T;
        case VK_U: return SDL_SCANCODE_U;
        case VK_V: return SDL_SCANCODE_V;
        case VK_W: return SDL_SCANCODE_W;
        case VK_X: return SDL_SCANCODE_X;
        case VK_Y: return SDL_SCANCODE_Y;
        case VK_Z: return SDL_SCANCODE_Z;
        case VK_LWIN: return SDL_SCANCODE_UNKNOWN;
        case VK_RWIN: return SDL_SCANCODE_UNKNOWN;
        case VK_APPS: return SDL_SCANCODE_UNKNOWN;
        case VK_SLEEP: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD0: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD1: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD2: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD3: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD4: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD5: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD6: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD7: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD8: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMPAD9: return SDL_SCANCODE_UNKNOWN;
        case VK_MULTIPLY: return SDL_SCANCODE_UNKNOWN;
        case VK_ADD: return SDL_SCANCODE_UNKNOWN;
        case VK_SEPARATOR: return SDL_SCANCODE_UNKNOWN;
        case VK_SUBTRACT: return SDL_SCANCODE_UNKNOWN;
        case VK_DECIMAL: return SDL_SCANCODE_UNKNOWN;
        case VK_DIVIDE: return SDL_SCANCODE_UNKNOWN;
        case VK_F1: return SDL_SCANCODE_F1;
        case VK_F2: return SDL_SCANCODE_F2;
        case VK_F3: return SDL_SCANCODE_F3;
        case VK_F4: return SDL_SCANCODE_F4;
        case VK_F5: return SDL_SCANCODE_F5;
        case VK_F6: return SDL_SCANCODE_F6;
        case VK_F7: return SDL_SCANCODE_F7;
        case VK_F8: return SDL_SCANCODE_F8;
        case VK_F9: return SDL_SCANCODE_F9;
        case VK_F10: return SDL_SCANCODE_F10;
        case VK_F11: return SDL_SCANCODE_F11;
        case VK_F12: return SDL_SCANCODE_F12;
        case VK_F13: return SDL_SCANCODE_F13;
        case VK_F14: return SDL_SCANCODE_F14;
        case VK_F15: return SDL_SCANCODE_F15;
        case VK_F16: return SDL_SCANCODE_F16;
        case VK_F17: return SDL_SCANCODE_F17;
        case VK_F18: return SDL_SCANCODE_F18;
        case VK_F19: return SDL_SCANCODE_F19;
        case VK_F20: return SDL_SCANCODE_F20;
        case VK_F21: return SDL_SCANCODE_F21;
        case VK_F22: return SDL_SCANCODE_F22;
        case VK_F23: return SDL_SCANCODE_F23;
        case VK_F24: return SDL_SCANCODE_F24;
        case VK_NAVIGATION_VIEW: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_MENU: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_UP: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_DOWN: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_LEFT: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_RIGHT: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_ACCEPT: return SDL_SCANCODE_UNKNOWN;
        case VK_NAVIGATION_CANCEL: return SDL_SCANCODE_UNKNOWN;
        case VK_NUMLOCK: return SDL_SCANCODE_UNKNOWN;
        case VK_SCROLL: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_NEC_EQUAL: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_FJ_MASSHOU: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_FJ_TOUROKU: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_FJ_LOYA: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_FJ_ROYA: return SDL_SCANCODE_UNKNOWN;
        case VK_LSHIFT: return SDL_SCANCODE_LSHIFT;
        case VK_RSHIFT: return SDL_SCANCODE_RSHIFT;
        case VK_LCONTROL: return SDL_SCANCODE_LCTRL;
        case VK_RCONTROL: return SDL_SCANCODE_RCTRL;
        case VK_LMENU: return SDL_SCANCODE_UNKNOWN;
        case VK_RMENU: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_BACK: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_FORWARD: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_REFRESH: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_STOP: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_SEARCH: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_FAVORITES: return SDL_SCANCODE_UNKNOWN;
        case VK_BROWSER_HOME: return SDL_SCANCODE_UNKNOWN;
        case VK_VOLUME_MUTE: return SDL_SCANCODE_UNKNOWN;
        case VK_VOLUME_DOWN: return SDL_SCANCODE_UNKNOWN;
        case VK_VOLUME_UP: return SDL_SCANCODE_UNKNOWN;
        case VK_MEDIA_NEXT_TRACK: return SDL_SCANCODE_UNKNOWN;
        case VK_MEDIA_PREV_TRACK: return SDL_SCANCODE_UNKNOWN;
        case VK_MEDIA_STOP: return SDL_SCANCODE_UNKNOWN;
        case VK_MEDIA_PLAY_PAUSE: return SDL_SCANCODE_UNKNOWN;
        case VK_LAUNCH_MAIL: return SDL_SCANCODE_UNKNOWN;
        case VK_LAUNCH_MEDIA_SELECT: return SDL_SCANCODE_UNKNOWN;
        case VK_LAUNCH_APP1: return SDL_SCANCODE_UNKNOWN;
        case VK_LAUNCH_APP2: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_1: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_PLUS: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_COMMA: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_MINUS: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_PERIOD: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_2: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_3: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_A: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_B: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_X: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_Y: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_SHOULDER: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_SHOULDER: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_TRIGGER: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_TRIGGER: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_DPAD_UP: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_DPAD_DOWN: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_DPAD_LEFT: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_DPAD_RIGHT: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_MENU: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_VIEW: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_THUMBSTICK_UP: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_THUMBSTICK_DOWN: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_LEFT_THUMBSTICK_LEFT: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_THUMBSTICK_UP: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT: return SDL_SCANCODE_UNKNOWN;
        case VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_4: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_5: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_6: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_7: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_8: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_AX: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_102: return SDL_SCANCODE_UNKNOWN;
        case VK_ICO_HELP: return SDL_SCANCODE_UNKNOWN;
        case VK_ICO_00: return SDL_SCANCODE_UNKNOWN;
        case VK_PROCESSKEY: return SDL_SCANCODE_UNKNOWN;
        case VK_ICO_CLEAR: return SDL_SCANCODE_UNKNOWN;
        case VK_PACKET: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_RESET: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_JUMP: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_PA1: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_PA2: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_PA3: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_WSCTRL: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_CUSEL: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_ATTN: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_FINISH: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_COPY: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_AUTO: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_ENLW: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_BACKTAB: return SDL_SCANCODE_UNKNOWN;
        case VK_ATTN: return SDL_SCANCODE_UNKNOWN;
        case VK_CRSEL: return SDL_SCANCODE_UNKNOWN;
        case VK_EXSEL: return SDL_SCANCODE_UNKNOWN;
        case VK_EREOF: return SDL_SCANCODE_UNKNOWN;
        case VK_PLAY: return SDL_SCANCODE_UNKNOWN;
        case VK_ZOOM: return SDL_SCANCODE_UNKNOWN;
        case VK_NONAME: return SDL_SCANCODE_UNKNOWN;
        case VK_PA1: return SDL_SCANCODE_UNKNOWN;
        case VK_OEM_CLEAR: return SDL_SCANCODE_UNKNOWN;
    }
}

int32 SDLToWinAPIMappings(int32 mapping)
{
    switch (mapping) {
        default: return VK_UNKNOWN;
        case SDL_SCANCODE_CANCEL: return VK_CANCEL;
        case SDL_SCANCODE_TAB: return VK_TAB;
        case SDL_SCANCODE_CLEAR: return VK_CLEAR;
        case SDL_SCANCODE_RETURN: return VK_RETURN;
        case SDL_SCANCODE_MENU: return VK_MENU;
        case SDL_SCANCODE_PAUSE: return VK_PAUSE;
        case SDL_SCANCODE_CAPSLOCK: return VK_CAPITAL;
        case SDL_SCANCODE_ESCAPE: return VK_ESCAPE;
        case SDL_SCANCODE_SPACE: return VK_SPACE;
        case SDL_SCANCODE_PAGEUP: return VK_PRIOR;
        case SDL_SCANCODE_PAGEDOWN: return VK_NEXT;
        case SDL_SCANCODE_END: return VK_END;
        case SDL_SCANCODE_HOME: return VK_HOME;
        case SDL_SCANCODE_LEFT: return VK_LEFT;
        case SDL_SCANCODE_UP: return VK_UP;
        case SDL_SCANCODE_RIGHT: return VK_RIGHT;
        case SDL_SCANCODE_DOWN: return VK_DOWN;
        case SDL_SCANCODE_SELECT: return VK_SELECT;
        case SDL_SCANCODE_EXECUTE: return VK_EXECUTE;
        case SDL_SCANCODE_PRINTSCREEN: return VK_SNAPSHOT;
        case SDL_SCANCODE_INSERT: return VK_INSERT;
        case SDL_SCANCODE_DELETE: return VK_DELETE;
        case SDL_SCANCODE_HELP: return VK_HELP;
        case SDL_SCANCODE_0: return VK_0;
        case SDL_SCANCODE_1: return VK_1;
        case SDL_SCANCODE_2: return VK_2;
        case SDL_SCANCODE_3: return VK_3;
        case SDL_SCANCODE_4: return VK_4;
        case SDL_SCANCODE_5: return VK_5;
        case SDL_SCANCODE_6: return VK_6;
        case SDL_SCANCODE_7: return VK_7;
        case SDL_SCANCODE_8: return VK_8;
        case SDL_SCANCODE_9: return VK_9;
        case SDL_SCANCODE_A: return VK_A;
        case SDL_SCANCODE_B: return VK_B;
        case SDL_SCANCODE_C: return VK_C;
        case SDL_SCANCODE_D: return VK_D;
        case SDL_SCANCODE_E: return VK_E;
        case SDL_SCANCODE_F: return VK_F;
        case SDL_SCANCODE_G: return VK_G;
        case SDL_SCANCODE_H: return VK_H;
        case SDL_SCANCODE_I: return VK_I;
        case SDL_SCANCODE_J: return VK_J;
        case SDL_SCANCODE_K: return VK_K;
        case SDL_SCANCODE_L: return VK_L;
        case SDL_SCANCODE_M: return VK_M;
        case SDL_SCANCODE_N: return VK_N;
        case SDL_SCANCODE_O: return VK_O;
        case SDL_SCANCODE_P: return VK_P;
        case SDL_SCANCODE_Q: return VK_Q;
        case SDL_SCANCODE_R: return VK_R;
        case SDL_SCANCODE_S: return VK_S;
        case SDL_SCANCODE_T: return VK_T;
        case SDL_SCANCODE_U: return VK_U;
        case SDL_SCANCODE_V: return VK_V;
        case SDL_SCANCODE_W: return VK_W;
        case SDL_SCANCODE_X: return VK_X;
        case SDL_SCANCODE_Y: return VK_Y;
        case SDL_SCANCODE_Z: return VK_Z;
        case SDL_SCANCODE_F1: return VK_F1;
        case SDL_SCANCODE_F2: return VK_F2;
        case SDL_SCANCODE_F3: return VK_F3;
        case SDL_SCANCODE_F4: return VK_F4;
        case SDL_SCANCODE_F5: return VK_F5;
        case SDL_SCANCODE_F6: return VK_F6;
        case SDL_SCANCODE_F7: return VK_F7;
        case SDL_SCANCODE_F8: return VK_F8;
        case SDL_SCANCODE_F9: return VK_F9;
        case SDL_SCANCODE_F10: return VK_F10;
        case SDL_SCANCODE_F11: return VK_F11;
        case SDL_SCANCODE_F12: return VK_F12;
        case SDL_SCANCODE_F13: return VK_F13;
        case SDL_SCANCODE_F14: return VK_F14;
        case SDL_SCANCODE_F15: return VK_F15;
        case SDL_SCANCODE_F16: return VK_F16;
        case SDL_SCANCODE_F17: return VK_F17;
        case SDL_SCANCODE_F18: return VK_F18;
        case SDL_SCANCODE_F19: return VK_F19;
        case SDL_SCANCODE_F20: return VK_F20;
        case SDL_SCANCODE_F21: return VK_F21;
        case SDL_SCANCODE_F22: return VK_F22;
        case SDL_SCANCODE_F23: return VK_F23;
        case SDL_SCANCODE_F24: return VK_F24;
        case SDL_SCANCODE_LSHIFT: return VK_LSHIFT;
        case SDL_SCANCODE_RSHIFT: return VK_RSHIFT;
        case SDL_SCANCODE_LCTRL: return VK_LCONTROL;
        case SDL_SCANCODE_RCTRL: return VK_RCONTROL;
    }
}

bool32 getControllerButton(InputDevice *device, uint8 buttonID)
{
    if (buttonID == (uint8)SDL_CONTROLLER_BUTTON_INVALID || !device)
        return false;

    if (SDL_GameControllerGetButton(((InputDeviceSDL *)device)->controllerPtr, (SDL_GameControllerButton)buttonID)) {
        return true;
    }

    return false;
}
#endif

void UpdateKeyboardInput(InputDevice *device)
{
    device->anyPress = false;
#if RETRO_USING_SDL2
    int keyCount         = 0;
    const byte *keyState = SDL_GetKeyboardState(&keyCount);

    uint8 keyboardID      = (device->gamePadType & 0xFF);
    InputState *buttons[] = {
        &controller[keyboardID].keyUp, &controller[keyboardID].keyDown, &controller[keyboardID].keyLeft,  &controller[keyboardID].keyRight,
        &controller[keyboardID].keyA,  &controller[keyboardID].keyB,    &controller[keyboardID].keyC,     &controller[keyboardID].keyX,
        &controller[keyboardID].keyY,  &controller[keyboardID].keyZ,    &controller[keyboardID].keyStart, &controller[keyboardID].keySelect,
    };

    bool32 confirmPress = false;
    for (int i = 0; i < KEY_MAX; i++) {
        if (keyState[winAPIToSDLMappings(buttons[i]->keyMap)]) {
            device->anyPress |= true;
            confirmPress |= (i == 4 || i == 10);
        }
    }

    if (device->anyPress)
        device->inactiveTimer[0] = 0;
    else
        device->inactiveTimer[0]++;

    if (confirmPress)
        device->inactiveTimer[1] = 0;
    else
        device->inactiveTimer[1]++;

#if !RETRO_REV02
    if (device->anyPress)
        mostRecentControllerID = device->controllerID;
#endif

#endif

    device->processInput(device, CONT_ANY);
}
void UpdateDeviceInput(InputDevice *device)
{
    device->anyPress = false;
#if RETRO_USING_SDL2
    int buttonMap[] = {
        SDL_CONTROLLER_BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_CONTROLLER_BUTTON_A,       SDL_CONTROLLER_BUTTON_B,         SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_Y,       SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_START,     SDL_CONTROLLER_BUTTON_BACK,
    };

    bool32 confirmPress = false;
    for (int i = 0; i < KEY_MAX; ++i) {
        if (getControllerButton(device, buttonMap[i])) {
            device->anyPress |= true;
            confirmPress |= (i == 4 || i == 10);
        }
    }

    InputDeviceSDL *sdlDevice = (InputDeviceSDL *)device;
    int delta                 = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_LEFTX);
    float hDeltaL             = 0;
    if (delta < 0)
        hDeltaL = -normalize(-delta, 1, 32768);
    else
        hDeltaL = normalize(delta, 0, 32767);

    delta         = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_LEFTY);
    float vDeltaL = 0;
    if (delta < 0)
        vDeltaL = -normalize(-delta, 1, 32768);
    else
        vDeltaL = normalize(delta, 0, 32767);
    vDeltaL = -vDeltaL;

    delta         = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_RIGHTX);
    float hDeltaR = 0;
    if (delta < 0)
        hDeltaR = -normalize(-delta, 1, 32768);
    else
        hDeltaR = normalize(delta, 0, 32767);

    delta = SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_RIGHTY);
    float vDeltaR = 0;
    if (delta < 0)
        vDeltaR = -normalize(-delta, 1, 32768);
    else
        vDeltaR = normalize(delta, 0, 32767);
    vDeltaR = -vDeltaR;

    float deltaTriggerL = normalize(SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_TRIGGERLEFT), 0, 32767);
    float deltaTriggerR = normalize(SDL_GameControllerGetAxis(sdlDevice->controllerPtr, SDL_CONTROLLER_AXIS_TRIGGERRIGHT), 0, 32767);

    stickL[CONT_ANY].deadzone   = 0.3;
    stickR[CONT_ANY].deadzone   = 0.3;
    triggerL[CONT_ANY].deadzone = 0.3;
    triggerR[CONT_ANY].deadzone = 0.3;

    if (vDeltaL > stickL[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (vDeltaL < -stickL[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (hDeltaL < -stickL[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (hDeltaL > stickL[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_LEFTSTICK)) 
        device->anyPress |= true;

    if (vDeltaR > stickR[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (vDeltaR < -stickR[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (hDeltaR < -stickR[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (hDeltaR > stickR[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_RIGHTSTICK)) 
        device->anyPress |= true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) 
        device->anyPress |= true;

    if (deltaTriggerL > triggerL[CONT_ANY].deadzone) 
        device->anyPress |= true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) 
        device->anyPress |= true;

    if (deltaTriggerR > triggerR[CONT_ANY].deadzone) 
        device->anyPress |= true;


    if (device->anyPress)
        device->inactiveTimer[0] = 0;
    else
        device->inactiveTimer[0]++;

    if (confirmPress)
        device->inactiveTimer[1] = 0;
    else
        device->inactiveTimer[1]++;

#if !RETRO_REV02
    if (device->anyPress || confirmPress)
        mostRecentControllerID = device->controllerID;
#endif
#endif

    device->processInput(device, CONT_ANY);
}

void ProcessKeyboardInput(InputDevice *device, int32 controllerID)
{
#if RETRO_USING_SDL2
    int keyCount = 0;
    const byte *keyState = SDL_GetKeyboardState(&keyCount);

    InputState *buttons[] = {
        &controller[controllerID].keyUp, &controller[controllerID].keyDown, &controller[controllerID].keyLeft,  &controller[controllerID].keyRight,
        &controller[controllerID].keyA,  &controller[controllerID].keyB,    &controller[controllerID].keyC,     &controller[controllerID].keyX,
        &controller[controllerID].keyY,  &controller[controllerID].keyZ,    &controller[controllerID].keyStart, &controller[controllerID].keySelect,
    };

    int lastKey = -1;
    for (int i = 0; i < keyCount; i++) {
        if (keyState[i]) {
            lastKey = i;
        }
    }

    uint8 keyboardID = (device->gamePadType & 0xFF);

    int32 *mappings[] = {
        &controller[keyboardID].keyUp.keyMap,    &controller[keyboardID].keyDown.keyMap,  &controller[keyboardID].keyLeft.keyMap,
        &controller[keyboardID].keyRight.keyMap, &controller[keyboardID].keyA.keyMap,     &controller[keyboardID].keyB.keyMap,
        &controller[keyboardID].keyC.keyMap,     &controller[keyboardID].keyX.keyMap,     &controller[keyboardID].keyY.keyMap,
        &controller[keyboardID].keyZ.keyMap,     &controller[keyboardID].keyStart.keyMap, &controller[keyboardID].keySelect.keyMap,
    };

    for (int i = 0; i < KEY_MAX; i++) {
        if (*mappings[i] == -1 && lastKey != -1)
            *mappings[i] = SDLToWinAPIMappings(lastKey);

        if (keyState[winAPIToSDLMappings(*mappings[i])]) {
            buttons[i]->press = true;
        }
    }

    for (int i = 0; i < keyCount && !InputManager.anyPress; i++) {
        if (keyState[i]) {
            InputManager.anyPress = true;
        }
    }
#endif
}
void ProcessDeviceInput(InputDevice *device, int32 controllerID)
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
        SDL_CONTROLLER_BUTTON_Y,       SDL_CONTROLLER_BUTTON_INVALID,   SDL_CONTROLLER_BUTTON_START,     SDL_CONTROLLER_BUTTON_BACK,
    };

    for (int i = 0; i < KEY_MAX; ++i) {
        if (getControllerButton(device, buttonMap[i])) 
            buttons[i]->press = true;
    }

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_LEFTSTICK)) 
        stickL[controllerID].keyStick.press = true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
        stickR[controllerID].keyStick.press = true;

    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX && !InputManager.anyPress; ++i) {
        if (getControllerButton(device, i)) {
            InputManager.anyPress = true;
        }
    }

    InputDeviceSDL *sdlDevice = (InputDeviceSDL *)device;
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
        stickL[controllerID].keyUp.press = true;

    if (stickL[controllerID].vDelta < -stickL[controllerID].deadzone)
        stickL[controllerID].keyDown.press = true;

    if (stickL[controllerID].hDelta < -stickL[controllerID].deadzone)
        stickL[controllerID].keyLeft.press = true;

    if (stickL[controllerID].hDelta > stickL[controllerID].deadzone)
        stickL[controllerID].keyRight.press = true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_LEFTSTICK))
        stickL[controllerID].keyStick.press = true;

    if (stickR[controllerID].vDelta > stickR[controllerID].deadzone)
        stickR[controllerID].keyUp.press = true;

    if (stickR[controllerID].vDelta < -stickR[controllerID].deadzone)
        stickR[controllerID].keyDown.press = true;

    if (stickR[controllerID].hDelta < -stickR[controllerID].deadzone)
        stickR[controllerID].keyLeft.press = true;

    if (stickR[controllerID].hDelta > stickR[controllerID].deadzone)
        stickR[controllerID].keyRight.press = true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_RIGHTSTICK))
        stickR[controllerID].keyStick.press = true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_LEFTSHOULDER))
        triggerL[controllerID].keyBumper.press = true;

    if (triggerL[controllerID].delta > triggerL[controllerID].deadzone)
        triggerL[controllerID].keyTrigger.press = true;

    if (getControllerButton(device, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER))
        triggerR[controllerID].keyBumper.press = true;

    if (triggerR[controllerID].delta > triggerR[controllerID].deadzone)
        triggerR[controllerID].keyTrigger.press = true;
#endif
}

InputDevice *InitKeyboardDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;
    InputDevice *device = &InputDevices[InputDeviceCount];
    if (device->active)
        return NULL;

    InputDeviceCount++;
    device->gamePadType = (DEVICE_FLAG_UNKNOWN1 << 16) | (DEVICE_TYPE_KEYBOARD << 8) | (DEVICE_KEYBOARD << 0);
    device->field_F     = false;
    device->inputID     = id;
    device->active      = true;

    for (int i = 0; i < 4; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = 0;
        }
    }
    return device;
}

void StartupKeyboardInput()
{
    char buffer[0x10];
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        sprintf(buffer, "KBDevice%d", i);
        uint id = 0;
        GenerateCRC(&id, buffer);

        InputDevice *device = InitKeyboardDevice(id);
        if (device) {
            device->controllerID = i;
            //add the keyboard "device" id to the type as its lowest byte
            device->gamePadType |= i + 1;
            device->updateInput  = UpdateKeyboardInput;
            device->processInput = ProcessKeyboardInput;
        }
    }
}

void InitInputDevice()
{
    if (activeInputDevices[0])
        return;
    char buffer[0x100];
#if RETRO_USING_SDL2
#if RETRO_PLATFORM == RETRO_SWITCH
    SDL_GameControllerAddMapping("53776974636820436f6e74726f6c6c65,Switch "
                                 "Controller,a:b0,b:b1,back:b11,dpdown:b15,dpleft:b12,dpright:b14,dpup:b13,leftshoulder:b6,leftstick:b4,lefttrigger:"
                                 "b8,leftx:a0,lefty:a1,rightshoulder:b7,rightstick:b5,righttrigger:b9,rightx:a2,righty:a3,start:b10,x:b3,y:b2,");
    // Support for extra controller types SDL doesn't recognise
#elif RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_UWP
    // if (!usingCWD)
    //    sprintf(buffer, "%s/controllerdb.txt", getResourcesPath());
    // else
    //    sprintf(buffer, "%scontrollerdb.txt", gamePath);
    sprintf(buffer, BASE_PATH "controllerdb.txt");
#else
    sprintf(buffer, BASE_PATH "controllerdb.txt");
#endif // ! RETRO_PLATFORM == RETRO_SWITCH
    FileIO *file = fOpen(buffer, "rb");
    if (file) {
        fClose(file);

        // SDL_GameControllerAddMapping()
        int nummaps = SDL_GameControllerAddMappingsFromFile(buffer);
        if (nummaps >= 0)
            PrintLog(PRINT_NORMAL, "loaded %d controller mappings from '%s'", buffer, nummaps);
    }
#endif // ! RETRO_USING_SDL2

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        activeControllers[i]  = CONT_AUTOASSIGN;
        activeInputDevices[i] = NULL;
    }

    StartupKeyboardInput();

    for (int c = 0; c < RSDK::SKU::gamePadCount; ++c) {
        //PrintLog(PRINT_NORMAL, "%s Detected - Vendor ID: %x ProductID: %x\n", gamePadMappings[c].name, gamePadMappings[c].vendorID,
        //         gamePadMappings[c].productID);
    }
}

void ClearInput()
{
    for (int i = 0; i <= PLAYER_COUNT; ++i) {
        if (i != 0 && activeControllers[i - 1] == CONT_UNASSIGNED)
            continue;

        controller[i].keyUp.press     = false;
        controller[i].keyDown.press   = false;
        controller[i].keyLeft.press   = false;
        controller[i].keyRight.press  = false;
        controller[i].keyA.press      = false;
        controller[i].keyB.press      = false;
        controller[i].keyC.press      = false;
        controller[i].keyX.press      = false;
        controller[i].keyY.press      = false;
        controller[i].keyZ.press      = false;
        controller[i].keyStart.press  = false;
        controller[i].keySelect.press = false;
        stickL[i].keyUp.press         = false;
        stickL[i].keyDown.press       = false;
        stickL[i].keyLeft.press       = false;
        stickL[i].keyRight.press      = false;
        stickL[i].keyStick.press      = false;
        stickR[i].keyUp.press         = false;
        stickR[i].keyDown.press       = false;
        stickR[i].keyLeft.press       = false;
        stickR[i].keyRight.press      = false;
        stickR[i].keyStick.press      = false;
        triggerL[i].keyBumper.press   = false;
        triggerL[i].keyTrigger.press  = false;
        triggerR[i].keyBumper.press   = false;
        triggerR[i].keyTrigger.press  = false;
    }
}

void ProcessInput()
{
    ClearInput();

    InputManager.anyPress = false;

    for (int i = 0; i < InputDeviceCount; ++i) {
        InputDevice *device = &InputDevices[i];
        if (device && device->updateInput)
            device->updateInput(device);
    }

    if (InputManager.anyPress)
        engine.dimTimer = 0;
    else if (engine.dimTimer < engine.dimLimit)
        ++engine.dimTimer;

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        int assign = activeControllers[i];
        if (assign && assign != CONT_UNASSIGNED) {
            if (assign == CONT_AUTOASSIGN) {
                int id               = GetControllerInputID();
                activeControllers[i] = id;
                if (id != -1)
                    AssignControllerID(i + 1, id);
            }
            else {
                InputDevice *device = activeInputDevices[i];
                if (device && device->inputID == assign && device->active && device->processInput)
                    device->processInput(device, i + 1);
            }
        }
    }

#if !RETRO_REV02
    HandleSpecialKeys();
#endif

    for (int c = 0; c <= PLAYER_COUNT; ++c) {
        if (c <= 0 || activeControllers[c - 1] != CONT_UNASSIGNED) {
            InputState *cont[] = {
                &controller[c].keyUp, &controller[c].keyDown, &controller[c].keyLeft,  &controller[c].keyRight,
                &controller[c].keyA,  &controller[c].keyB,    &controller[c].keyC,     &controller[c].keyX,
                &controller[c].keyY,  &controller[c].keyZ,    &controller[c].keyStart, &controller[c].keySelect,
            };
            InputState *lStick[] = { &stickL[c].keyUp, &stickL[c].keyDown, &stickL[c].keyLeft, &stickL[c].keyRight, &stickL[c].keyStick };
            InputState *rStick[] = { &stickR[c].keyUp, &stickR[c].keyDown, &stickR[c].keyLeft, &stickR[c].keyRight, &stickR[c].keyStick };

            InputState *lTrigger[] = { &triggerL[c].keyBumper, &triggerL[c].keyTrigger };
            InputState *rTrigger[] = { &triggerR[c].keyBumper, &triggerR[c].keyTrigger };

            for (int i = 0; i < 12; ++i) {
                if (cont[i]->press) {
                    if (cont[i]->down)
                        cont[i]->press = false;
                    else
                        cont[i]->down = true;
                }
                else
                    cont[i]->down = false;
            }

            for (int i = 0; i < 5; ++i) {
                if (lStick[i]->press) {
                    if (lStick[i]->down)
                        lStick[i]->press = false;
                    else
                        lStick[i]->down = true;
                }
                else
                    lStick[i]->down = false;

                if (rStick[i]->press) {
                    if (rStick[i]->down)
                        rStick[i]->press = false;
                    else
                        rStick[i]->down = true;
                }
                else
                    rStick[i]->down = false;
            }

            for (int i = 0; i < 2; ++i) {
                if (lTrigger[i]->press) {
                    if (lTrigger[i]->down)
                        lTrigger[i]->press = false;
                    else
                        lTrigger[i]->down = true;
                }
                else
                    lTrigger[i]->down = false;

                if (rTrigger[i]->press) {
                    if (rTrigger[i]->down)
                        rTrigger[i]->press = false;
                    else
                        rTrigger[i]->down = true;
                }
                else
                    rTrigger[i]->down = false;
            }
        }
    }

#if RETRO_USING_SDL2
#ifdef RETRO_USING_MOUSE
#if RETRO_USING_SDL2
    if (touchMouseData.count <= 1) { // Touch always takes priority over mouse
#endif
                                     //! RETRO_USING_SDL2
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
#if RETRO_USING_SDL2
    }
#endif //! RETRO_USING_SDL2
#endif //! RETRO_USING_MOUSE

    if (touchMouseData.count)
        engine.dimTimer = 0;
#endif
}

#if !RETRO_REV02
int32 specialKeyStates[4];
int32 prevSpecialKeyStates[4];
int32 buttonDownCount     = 0;
int32 prevButtonDownCount = 0;

void HandleSpecialKeys()
{
    if (specialKeyStates[0] || specialKeyStates[3]) {
        touchMouseData.pausePress              = !touchMouseData.pausePressActive;
        touchMouseData.pausePressActive        = true;
    }
    else {
        touchMouseData.pausePress       = false;
        touchMouseData.pausePressActive = false;
    }

    if (buttonDownCount) {
        touchMouseData.anyPressActive = true;
        touchMouseData.anyPress       = prevButtonDownCount < buttonDownCount;
    }
    else {
        touchMouseData.anyPress       = false;
        touchMouseData.anyPressActive = false;
    }
    prevButtonDownCount = buttonDownCount;
    for (int k = 0; k < 4; ++k) prevSpecialKeyStates[k] = specialKeyStates[k];
}
#endif

int32 GetControllerType(int32 inputID)
{
    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i].inputID == inputID) {
            return InputDevices[i].gamePadType;
        }
    }

#if RETRO_REV02
    return RSDK::SKU::userCore->GetDefaultGamepadType();
#else
    int32 platform = RSDK::gameVerInfo.platform;

    switch (platform) {
        case PLATFORM_SWITCH: return (DEVICE_FLAG_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_HANDHELD << 0);
        case PLATFORM_PC:
        case PLATFORM_DEV:
        default: return (DEVICE_FLAG_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0); break;
    }
#endif
}
