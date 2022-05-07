int32 keyState[PLAYER_COUNT];

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

        touchMouseData.x[0] = (cursorPos.x - RSDK::gameSettings.viewportX) * RSDK::gameSettings.viewportW;
        touchMouseData.y[0] = (cursorPos.y - RSDK::gameSettings.viewportY) * RSDK::gameSettings.viewportH;

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
    if (keyCode == VK_UNKNOWN)
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
