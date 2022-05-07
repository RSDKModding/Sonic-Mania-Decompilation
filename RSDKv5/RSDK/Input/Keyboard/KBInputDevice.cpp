
int32 keyState[PLAYER_COUNT];

#if !RETRO_REV02
int32 specialKeyStates[4];
int32 prevSpecialKeyStates[4];
int32 buttonDownCount     = 0;
int32 prevButtonDownCount = 0;
#endif

#if RETRO_RENDERDEVICE_SDL2

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
        default: return KEYMAP_NO_MAPPING;
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
        case SDL_SCANCODE_LALT: return VK_LMENU;
        case SDL_SCANCODE_RALT: return VK_RMENU;
    }
}
#endif

InputDeviceKeyboard *InitKeyboardDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceKeyboard();

    InputDeviceKeyboard *device = (InputDeviceKeyboard *)InputDevices[InputDeviceCount];
    device->gamePadType = (DEVICE_API_KEYBOARD << 16) | (DEVICE_TYPE_KEYBOARD << 8) | (DEVICE_KEYBOARD << 0);
    device->disabled    = false;
    device->inputID     = id;
    device->active      = true;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}

void InputDeviceKeyboard::UpdateInput()
{
    if (!this->controllerID) {
#if RETRO_RENDERDEVICE_DIRECTX9 || RETRO_RENDERDEVICE_DIRECTX11
        tagPOINT cursorPos;
        GetCursorPos(&cursorPos);
        ScreenToClient(RenderDevice::windowHandle, &cursorPos);
#elif RETRO_RENDERDEVICE_SDL2
        Vector2 cursorPos;
        SDL_GetMouseState(&cursorPos.x, &cursorPos.y);
#endif

        float prevX = touchMouseData.x[0];
        float prevY = touchMouseData.y[0];

        touchMouseData.x[0] = (cursorPos.x - RSDK::videoSettings.viewportX) * RSDK::videoSettings.viewportW;
        touchMouseData.y[0] = (cursorPos.y - RSDK::videoSettings.viewportY) * RSDK::videoSettings.viewportH;

        if (touchMouseData.x[0] == prevX && touchMouseData.y[0] == prevY) {
            if (this->mouseHideTimer < 120 + 1) {
                if (++this->mouseHideTimer == 120) {
                    RenderDevice::ShowCursor(false);
                }
            }
        }
        else if (this->mouseHideTimer >= 120) {
            this->mouseHideTimer = 0;

            RenderDevice::ShowCursor(true);
        }
    }

    this->prevInputFlags = this->inputFlags;
    this->inputFlags     = keyState[this->controllerID];

    int32 changedKeys = ~this->prevInputFlags & (this->inputFlags ^ this->prevInputFlags);
    if (changedKeys) {
        this->inactiveTimer[0] = 0;
        this->anyPress         = true;
    }
    else {
        ++this->inactiveTimer[0];
        this->anyPress = 0;
    }

    if ((changedKeys & KEYMASK_A) || (changedKeys & KEYMASK_START))
        this->inactiveTimer[1] = 0;
    else
        ++this->inactiveTimer[1];

    this->stateUp     = (this->inputFlags & KEYMASK_UP) != 0;
    this->stateDown   = (this->inputFlags & KEYMASK_DOWN) != 0;
    this->stateLeft   = (this->inputFlags & KEYMASK_LEFT) != 0;
    this->stateRight  = (this->inputFlags & KEYMASK_RIGHT) != 0;
    this->stateA      = (this->inputFlags & KEYMASK_A) != 0;
    this->stateB      = (this->inputFlags & KEYMASK_B) != 0;
    this->stateC      = (this->inputFlags & KEYMASK_C) != 0;
    this->stateX      = (this->inputFlags & KEYMASK_X) != 0;
    this->stateY      = (this->inputFlags & KEYMASK_Y) != 0;
    this->stateZ      = (this->inputFlags & KEYMASK_Z) != 0;
    this->stateStart  = (this->inputFlags & KEYMASK_START) != 0;
    this->stateSelect = (this->inputFlags & KEYMASK_SELECT) != 0;

    this->ProcessInput(CONT_ANY);
}

void InputDeviceKeyboard::ProcessInput(int32 controllerID)
{
    ControllerState *cont = &controller[controllerID];

    cont->keyUp.press |= this->stateUp;
    cont->keyDown.press |= this->stateDown;
    cont->keyLeft.press |= this->stateLeft;
    cont->keyRight.press |= this->stateRight;
    cont->keyA.press |= this->stateA;
    cont->keyB.press |= this->stateB;
    cont->keyC.press |= this->stateC;
    cont->keyX.press |= this->stateX;
    cont->keyY.press |= this->stateY;
    cont->keyZ.press |= this->stateZ;
    cont->keyStart.press |= this->stateStart;
    cont->keySelect.press |= this->stateSelect;
}

void InitKeyboardInputAPI()
{
    char buffer[0x10];
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        sprintf(buffer, "KBDevice%d", i);
        uint id = 0;
        GenerateCRC(&id, buffer);

        InputDeviceKeyboard *device = InitKeyboardDevice(id);
        if (device) {
            device->controllerID = i;
            // add the keyboard "device" id to the type as its lowest byte
            device->gamePadType |= i + 1;
        }
    }
}

void UpdateKeyState(int32 keyCode)
{
#if RETRO_RENDERDEVICE_SDL2
    keyCode = SDLToWinAPIMappings(keyCode);
#endif

    // invalid key
    if (keyCode == KEYMAP_NO_MAPPING)
        return;

    for (int i = 1; i <= PLAYER_COUNT; ++i) {
        InputState *buttons[] = {
            &controller[i].keyUp, &controller[i].keyDown, &controller[i].keyLeft,  &controller[i].keyRight,
            &controller[i].keyA,  &controller[i].keyB,    &controller[i].keyC,     &controller[i].keyX,
            &controller[i].keyY,  &controller[i].keyZ,    &controller[i].keyStart, &controller[i].keySelect,
        };

        int32 keyMasks[] = {
            KEYMASK_UP, KEYMASK_DOWN, KEYMASK_LEFT, KEYMASK_RIGHT, KEYMASK_A,     KEYMASK_B,
            KEYMASK_C,  KEYMASK_X,    KEYMASK_Y,    KEYMASK_Z,     KEYMASK_START, KEYMASK_SELECT,
        };

        for (int k = 0; k < KEY_MAX; k++) {
            if (keyCode == buttons[k]->keyMap)
                keyState[i - 1] |= keyMasks[k];
            else if (buttons[k]->keyMap == -1)
                buttons[k]->keyMap = keyCode;
        }
    }
}

void ClearKeyState(int32 keyCode)
{
#if RETRO_RENDERDEVICE_SDL2
    keyCode = SDLToWinAPIMappings(keyCode);
#endif

    for (int i = 1; i <= PLAYER_COUNT; ++i) {
        InputState *buttons[] = {
            &controller[i].keyUp, &controller[i].keyDown, &controller[i].keyLeft,  &controller[i].keyRight,
            &controller[i].keyA,  &controller[i].keyB,    &controller[i].keyC,     &controller[i].keyX,
            &controller[i].keyY,  &controller[i].keyZ,    &controller[i].keyStart, &controller[i].keySelect,
        };

        int32 keyMasks[] = {
            KEYMASK_UP, KEYMASK_DOWN, KEYMASK_LEFT, KEYMASK_RIGHT, KEYMASK_A,     KEYMASK_B,
            KEYMASK_C,  KEYMASK_X,    KEYMASK_Y,    KEYMASK_Z,     KEYMASK_START, KEYMASK_SELECT,
        };

        for (int32 k = 0; k < KEY_MAX; k++) {
            if (keyCode == buttons[k]->keyMap)
                keyState[i - 1] &= ~keyMasks[k];
        }
    }
}

#if !RETRO_REV02
void HandleSpecialKeys()
{
    if (specialKeyStates[0] || specialKeyStates[3]) {
        touchMouseData.pausePress       = !touchMouseData.pausePressActive;
        touchMouseData.pausePressActive = true;
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
    for (int32 k = 0; k < 4; ++k) prevSpecialKeyStates[k] = specialKeyStates[k];
}
#endif