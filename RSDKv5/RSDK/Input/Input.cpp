#include "RSDK/Core/RetroEngine.hpp"

InputDevice *InputDevices[INPUTDEVICE_COUNT];
int32 InputDeviceCount = 0;

int32 activeControllers[PLAYER_COUNT];
InputDevice *activeInputDevices[PLAYER_COUNT];

ControllerState controller[PLAYER_COUNT + 1];
AnalogState stickL[PLAYER_COUNT + 1];
AnalogState stickR[PLAYER_COUNT + 1];
TriggerState triggerL[PLAYER_COUNT + 1];
TriggerState triggerR[PLAYER_COUNT + 1];
TouchMouseData touchMouseData;

#if RETRO_INPUTDEVICE_XINPUT
bool32 disabledXInputDevices[PLAYER_COUNT];
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
bool32 HIDEnabled = false;

InputDevice *rawInputDevices[INPUTDEVICE_COUNT];
int32 rawInputDeviceCount = 0;

tagRAWINPUT rawInputData;
#endif

GamePadMappings *gamePadMappings = NULL;
int gamePadCount                 = 0;

#if !RETRO_REV02
int mostRecentControllerID = -1;
#endif

#define NORMALIZE(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

#if RETRO_INPUTDEVICE_KEYBOARD
#include "Keyboard/KBInputDevice.cpp"
#endif

#if RETRO_RENDERDEVICE_SDL2
bool32 getControllerButton(InputDeviceSDL *device, uint8 buttonID)
{
    if (buttonID == (uint8)SDL_CONTROLLER_BUTTON_INVALID || !device)
        return false;

    if (SDL_GameControllerGetButton(device->controllerPtr, (SDL_GameControllerButton)buttonID))
        return true;

    return false;
}
#endif

#if RETRO_INPUTDEVICE_XINPUT
void InputDeviceXInput::UpdateInput() {
    XINPUT_STATE *inputState = &this->inputState[this->activeState];
    if (disabledXInputDevices[this->controllerID]) {
        this->disabled = true;
    }
    else {
        this->disabled = false;
        if (!XInputGetState(this->controllerID, inputState)) {
            this->activeState ^= 1;

            int32 changedButtons = ~this->inputState[this->activeState].Gamepad.wButtons
                                   & (this->inputState[this->activeState].Gamepad.wButtons ^ inputState->Gamepad.wButtons);

            if (changedButtons)
                this->inactiveTimer[0] = 0;
            else
                ++this->inactiveTimer[0];

            if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
                this->inactiveTimer[1] = 0;
            else
                ++this->inactiveTimer[1];

            this->anyPress = changedButtons || ((inputState->Gamepad.sThumbLX ^ this->inputState[this->activeState].Gamepad.sThumbLX) & 0xC000) != 0
                             || ((inputState->Gamepad.sThumbLY ^ this->inputState[this->activeState].Gamepad.sThumbLY) & 0xC000) != 0
                             || ((inputState->Gamepad.sThumbRX ^ this->inputState[this->activeState].Gamepad.sThumbRX) & 0xC000) != 0
                             || ((inputState->Gamepad.sThumbRY ^ this->inputState[this->activeState].Gamepad.sThumbRY) & 0xC000) != 0;
            
            XINPUT_GAMEPAD *gamePad             = &this->inputState[this->activeState].Gamepad;

            this->stateUp       = (gamePad->wButtons & KEYMASK_UP) != 0;
            this->stateDown     = (gamePad->wButtons & KEYMASK_DOWN) != 0;
            this->stateLeft     = (gamePad->wButtons & KEYMASK_LEFT) != 0;
            this->stateRight    = (gamePad->wButtons & KEYMASK_RIGHT) != 0;
            this->stateA        = (gamePad->wButtons & KEYMASK_A) != 0;
            this->stateB        = (gamePad->wButtons & KEYMASK_B) != 0;
            this->stateX        = (gamePad->wButtons & KEYMASK_X) != 0;
            this->stateY        = (gamePad->wButtons & KEYMASK_Y) != 0;
            this->stateStart    = (gamePad->wButtons & KEYMASK_START) != 0;
            this->stateSelect   = (gamePad->wButtons & KEYMASK_SELECT) != 0;
            this->stateBumper_L = (gamePad->wButtons & KEYMASK_BUMPERL) != 0;
            this->stateBumper_R = (gamePad->wButtons & KEYMASK_BUMPERR) != 0;
            this->stateStick_L  = (gamePad->wButtons & KEYMASK_STICKL) != 0;
            this->stateStick_R  = (gamePad->wButtons & KEYMASK_STICKR) != 0;

            this->hDelta_L      = gamePad->sThumbLX;
            this->vDelta_L      = gamePad->sThumbLY;

            float div      = sqrtf((this->hDelta_L * this->hDelta_L) + (this->vDelta_L * this->vDelta_L));
            this->hDelta_L = this->hDelta_L / div;
            this->vDelta_L = this->vDelta_L / div;
            if (div <= 7864.0) {
                this->hDelta_L = 0.0;
                this->vDelta_L = 0.0;
            }
            else {
                this->hDelta_L = this->hDelta_L * ((fminf(32767.0, div) - 7864.0) / 24903.0);
                this->vDelta_L = this->vDelta_L * ((fminf(32767.0, div) - 7864.0) / 24903.0);
            }

            this->hDelta_R = gamePad->sThumbRX;
            this->vDelta_R = gamePad->sThumbRY;

            div            = sqrtf((this->hDelta_R * this->hDelta_R) + (this->vDelta_R * this->vDelta_R));
            this->hDelta_R = this->hDelta_R / div;
            this->vDelta_R = this->vDelta_R / div;
            if (div <= 7864.0) {
                this->hDelta_R = 0.0;
                this->vDelta_R = 0.0;
            }
            else {
                this->hDelta_R = this->hDelta_R * ((fminf(32767.0, div) - 7864.0) / 24903.0);
                this->vDelta_R = this->vDelta_R * ((fminf(32767.0, div) - 7864.0) / 24903.0);
            }

            if (this->stateBumper_L)
                this->deadzoneLTrigger = 1.0;
            else
                this->deadzoneLTrigger = 0.0;

            this->deltaLTrigger = gamePad->bLeftTrigger;
            if (this->deltaLTrigger <= 30.0)
                this->deltaLTrigger = 0.0;
            else
                this->deltaLTrigger = (this->deltaLTrigger - 30.0) / 225.0;

            if (this->stateBumper_R)
                this->deadzoneRTrigger = 1.0;
            else
                this->deadzoneRTrigger = 0.0;

            this->deltaRTrigger = gamePad->bRightTrigger;
            if (this->deltaRTrigger <= 30.0)
                this->deltaRTrigger = 0.0;
            else
                this->deltaRTrigger = (this->deltaRTrigger - 30.0) / 225.0;
        }
        return this->ProcessInput(CONT_ANY);
    }
}

void InputDeviceXInput::ProcessInput(int32 controllerID)
{
    controller[controllerID].keyUp.press |= this->stateUp;
    controller[controllerID].keyDown.press |= this->stateDown;
    controller[controllerID].keyLeft.press |= this->stateLeft;
    controller[controllerID].keyRight.press |= this->stateRight;
    controller[controllerID].keyA.press |= this->stateA;
    controller[controllerID].keyB.press |= this->stateB;
    controller[controllerID].keyX.press |= this->stateX;
    controller[controllerID].keyY.press |= this->stateY;
    controller[controllerID].keyStart.press |= this->stateStart;
    controller[controllerID].keySelect.press |= this->stateSelect;

    stickL[controllerID].keyStick.press |= this->stateStick_L;
    stickL[controllerID].hDelta = this->hDelta_L;
    stickL[controllerID].vDelta = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > 0.3;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -0.3;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -0.3;
    stickL[controllerID].keyRight.press |= this->hDelta_L > 0.3;

    stickR[controllerID].keyStick.press |= this->stateStick_R;
    stickR[controllerID].hDelta = this->hDelta_R;
    stickR[controllerID].vDelta = this->vDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > 0.3;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -0.3;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -0.3;
    stickR[controllerID].keyRight.press |= this->hDelta_R > 0.3;

    triggerL[controllerID].keyBumper.press |= this->stateBumper_L;
    triggerL[controllerID].deadzone = this->deadzoneLTrigger;
    triggerL[controllerID].delta    = this->deltaLTrigger;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].deadzone = this->deadzoneRTrigger;
    triggerR[controllerID].delta    = this->deltaRTrigger;
}
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
void InputDeviceRaw::UpdateInput()
{
    this->prevInputFlags = this->inputFlags;
    this->inputFlags     = this->activeButtons;

    int32 changedButtons = ~this->prevInputFlags & (this->prevInputFlags ^ this->inputFlags);
    if (changedButtons) {
        this->inactiveTimer[0] = 0;
        this->anyPress         = true;
    }
    else {
        ++this->inactiveTimer[0];
        this->anyPress = false;
    }

    if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
        this->inactiveTimer[1] = 0;
    else
        ++this->inactiveTimer[1];

    this->stateUp        = (this->inputFlags & KEYMASK_UP) != 0;
    this->stateDown      = (this->inputFlags & KEYMASK_DOWN) != 0;
    this->stateLeft      = (this->inputFlags & KEYMASK_LEFT) != 0;
    this->stateRight     = (this->inputFlags & KEYMASK_RIGHT) != 0;
    this->stateA         = (this->inputFlags & KEYMASK_A) != 0;
    this->stateB         = (this->inputFlags & KEYMASK_B) != 0;
    this->stateC         = (this->inputFlags & KEYMASK_C) != 0;
    this->stateX         = (this->inputFlags & KEYMASK_X) != 0;
    this->stateY         = (this->inputFlags & KEYMASK_Y) != 0;
    this->stateZ         = (this->inputFlags & KEYMASK_Z) != 0;
    this->stateStart     = (this->inputFlags & KEYMASK_START) != 0;
    this->stateSelect    = (this->inputFlags & KEYMASK_SELECT) != 0;
    this->stateBumper_L  = (this->inputFlags & KEYMASK_BUMPERL) != 0;
    this->stateBumper_R  = (this->inputFlags & KEYMASK_BUMPERR) != 0;
    this->stateStick_L    = (this->inputFlags & KEYMASK_STICKL) != 0;
    this->stateStick_R    = (this->inputFlags & KEYMASK_STICKR) != 0;
    this->stateTrigger_L = (this->inputFlags & KEYMASK_TRIGGERL) != 0;
    this->stateTrigger_R = (this->inputFlags & KEYMASK_TRIGGERR) != 0;
    
    this->ProcessInput(CONT_ANY);
}

void InputDeviceRaw::ProcessInput(int32 controllerID)
{
    controller[controllerID].keyUp.press |= this->stateUp;
    controller[controllerID].keyDown.press |= this->stateDown;
    controller[controllerID].keyLeft.press |= this->stateLeft;
    controller[controllerID].keyRight.press |= this->stateRight;
    controller[controllerID].keyA.press |= this->stateA;
    controller[controllerID].keyB.press |= this->stateB;
    controller[controllerID].keyC.press |= this->stateC;
    controller[controllerID].keyX.press |= this->stateX;
    controller[controllerID].keyY.press |= this->stateY;
    controller[controllerID].keyZ.press |= this->stateZ;
    controller[controllerID].keyStart.press |= this->stateStart;
    controller[controllerID].keySelect.press |= this->stateSelect;

    stickL[controllerID].keyStick.press |= this->stateStick_L;
    stickL[controllerID].hDelta = this->hDelta_L;
    stickL[controllerID].vDelta = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > 0.3;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -0.3;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -0.3;
    stickL[controllerID].keyRight.press |= this->hDelta_L > 0.3;

    stickR[controllerID].keyStick.press |= this->stateStick_R;
    stickR[controllerID].hDelta = this->vDelta_R;
    stickR[controllerID].vDelta = this->hDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > 0.3;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -0.3;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -0.3;
    stickR[controllerID].keyRight.press |= this->hDelta_R > 0.3;

    triggerL[controllerID].keyBumper.press |= this->stateBumper_L;
    triggerL[controllerID].keyTrigger.press |= this->stateTrigger_L;
    triggerL[controllerID].deadzone = this->triggerDeltaL;
    triggerL[controllerID].delta    = this->triggerDeltaL;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].keyTrigger.press |= this->stateTrigger_R;
    triggerR[controllerID].deadzone = this->triggerDeltaR;
    triggerR[controllerID].delta    = this->triggerDeltaR;
}
#endif

#if RETRO_INPUTDEVICE_SDL2
void InputDeviceSDL::UpdateInput()
{
    int32 buttonMap[] = {
        SDL_CONTROLLER_BUTTON_DPAD_UP,   SDL_CONTROLLER_BUTTON_DPAD_DOWN,  SDL_CONTROLLER_BUTTON_DPAD_LEFT,    SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_CONTROLLER_BUTTON_A,         SDL_CONTROLLER_BUTTON_B,          SDL_CONTROLLER_BUTTON_INVALID,      SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_Y,         SDL_CONTROLLER_BUTTON_INVALID,    SDL_CONTROLLER_BUTTON_START,        SDL_CONTROLLER_BUTTON_BACK,
        SDL_CONTROLLER_BUTTON_LEFTSTICK, SDL_CONTROLLER_BUTTON_RIGHTSTICK, SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    };

    int32 keyMasks[] = { KEYMASK_UP, KEYMASK_DOWN, KEYMASK_LEFT,  KEYMASK_RIGHT,  KEYMASK_A,      KEYMASK_B,      KEYMASK_C,       KEYMASK_X,
                         KEYMASK_Y,  KEYMASK_Z,    KEYMASK_START, KEYMASK_SELECT, KEYMASK_STICKL, KEYMASK_STICKR, KEYMASK_BUMPERL, KEYMASK_BUMPERR };

    this->prevInputFlags = this->inputFlags;

    float prevHDeltaL       = hDelta_L;
    float prevVDeltaL       = vDelta_L;
    float prevHDeltaR       = hDelta_R;
    float prevVDeltaR       = vDelta_R;
    float prevTriggerDeltaL = triggerDeltaL;
    float prevTriggerDeltaR = triggerDeltaR;

    this->inputFlags = 0;
    for (int32 i = 0; i < KEY_MAX + 4; ++i) {
        if (getControllerButton(this, buttonMap[i]))
            this->inputFlags |= keyMasks[i];
    }

    int32 delta                 = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_LEFTX);
    if (delta < 0)
        hDelta_L = -NORMALIZE(-delta, 1, 32768);
    else
        hDelta_L = NORMALIZE(delta, 0, 32767);

    delta         = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_LEFTY);
    if (delta < 0)
        vDelta_L = -NORMALIZE(-delta, 1, 32768);
    else
        vDelta_L = NORMALIZE(delta, 0, 32767);
    vDelta_L = -vDelta_L;

    delta         = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_RIGHTX);
    if (delta < 0)
        hDelta_R = -NORMALIZE(-delta, 1, 32768);
    else
        hDelta_R = NORMALIZE(delta, 0, 32767);

    delta = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_RIGHTY);
    if (delta < 0)
        vDelta_R = -NORMALIZE(-delta, 1, 32768);
    else
        vDelta_R = NORMALIZE(delta, 0, 32767);
    vDelta_R = -vDelta_R;

    triggerDeltaL = NORMALIZE(SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_TRIGGERLEFT), 0, 32767);
    triggerDeltaR = NORMALIZE(SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_TRIGGERRIGHT), 0, 32767);

    int32 changedButtons = ~this->prevInputFlags & (this->prevInputFlags ^ this->inputFlags);

    if (changedButtons || hDelta_L != prevHDeltaL || vDelta_L != prevVDeltaL || hDelta_R != prevHDeltaR || vDelta_R != prevVDeltaR
        || triggerDeltaL != prevTriggerDeltaL || triggerDeltaR != prevTriggerDeltaR) {
        this->inactiveTimer[0] = 0;
        this->anyPress         = true;
    }
    else {
        ++this->inactiveTimer[0];
        this->anyPress = false;
    }

    if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
        this->inactiveTimer[1] = 0;
    else
        ++this->inactiveTimer[1];

    this->stateUp        = (this->inputFlags & KEYMASK_UP) != 0;
    this->stateDown      = (this->inputFlags & KEYMASK_DOWN) != 0;
    this->stateLeft      = (this->inputFlags & KEYMASK_LEFT) != 0;
    this->stateRight     = (this->inputFlags & KEYMASK_RIGHT) != 0;
    this->stateA         = (this->inputFlags & KEYMASK_A) != 0;
    this->stateB         = (this->inputFlags & KEYMASK_B) != 0;
    this->stateC         = (this->inputFlags & KEYMASK_C) != 0;
    this->stateX         = (this->inputFlags & KEYMASK_X) != 0;
    this->stateY         = (this->inputFlags & KEYMASK_Y) != 0;
    this->stateZ         = (this->inputFlags & KEYMASK_Z) != 0;
    this->stateStart     = (this->inputFlags & KEYMASK_START) != 0;
    this->stateSelect    = (this->inputFlags & KEYMASK_SELECT) != 0;
    this->stateBumper_L  = (this->inputFlags & KEYMASK_BUMPERL) != 0;
    this->stateBumper_R  = (this->inputFlags & KEYMASK_BUMPERR) != 0;
    this->stateStick_L   = (this->inputFlags & KEYMASK_STICKL) != 0;
    this->stateStick_R   = (this->inputFlags & KEYMASK_STICKR) != 0;

#if !RETRO_REV02
    if ((changedButtons & KEYMASK_A) || (changedButtons & KEYMASK_START))
        mostRecentControllerID = device->controllerID;
#endif

    ProcessInput(CONT_ANY);
}

void InputDeviceSDL::ProcessInput(int32 controllerID)
{
    controller[controllerID].keyUp.press |= this->stateUp;
    controller[controllerID].keyDown.press |= this->stateDown;
    controller[controllerID].keyLeft.press |= this->stateLeft;
    controller[controllerID].keyRight.press |= this->stateRight;
    controller[controllerID].keyA.press |= this->stateA;
    controller[controllerID].keyB.press |= this->stateB;
    controller[controllerID].keyC.press |= this->stateC;
    controller[controllerID].keyX.press |= this->stateX;
    controller[controllerID].keyY.press |= this->stateY;
    controller[controllerID].keyZ.press |= this->stateZ;
    controller[controllerID].keyStart.press |= this->stateStart;
    controller[controllerID].keySelect.press |= this->stateSelect;

    stickL[controllerID].keyStick.press |= this->stateStick_L;
    stickL[controllerID].hDelta = this->hDelta_L;
    stickL[controllerID].vDelta = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > 0.3;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -0.3;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -0.3;
    stickL[controllerID].keyRight.press |= this->hDelta_L > 0.3;

    stickR[controllerID].keyStick.press |= this->stateStick_R;
    stickR[controllerID].hDelta = this->vDelta_R;
    stickR[controllerID].vDelta = this->hDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > 0.3;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -0.3;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -0.3;
    stickR[controllerID].keyRight.press |= this->hDelta_R > 0.3;

    triggerL[controllerID].keyBumper.press |= this->stateBumper_L;
    triggerL[controllerID].keyTrigger.press |= this->triggerDeltaL > 0.3;
    triggerL[controllerID].deadzone = this->triggerDeltaL;
    triggerL[controllerID].delta    = this->triggerDeltaL;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].keyTrigger.press |= this->triggerDeltaR > 0.3;
    triggerR[controllerID].deadzone = this->triggerDeltaR;
    triggerR[controllerID].delta    = this->triggerDeltaR;
}

void InputDeviceSDL::CloseDevice()
{
    this->active               = false;
    this->assignedControllerID = false;
    SDL_GameControllerClose(this->controllerPtr);
    this->controllerPtr = NULL;
}
#endif

#if RETRO_INPUTDEVICE_XINPUT
InputDeviceXInput *InitXInputDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceXInput();

    InputDeviceXInput *device = (InputDeviceXInput *)InputDevices[InputDeviceCount];

    for (int32 i = 0; i < PLAYER_COUNT; ++i) disabledXInputDevices[i] = false;

    device->gamePadType = (DEVICE_API_XINPUT << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_XBOX << 0);
    device->disabled     = false;
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
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
InputDeviceRaw* InitRawInputDevice(uint32 id)
{
    if (InputDeviceCount == INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceRaw();

    InputDeviceRaw *device = (InputDeviceRaw *)InputDevices[InputDeviceCount];

    InputDeviceCount++;
    device->gamePadType = (DEVICE_API_RAWINPUT << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0);
    device->disabled     = false;
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
#endif

#if RETRO_INPUTDEVICE_SDL2
InputDeviceSDL *InitSDL2InputDevice(uint32 id, uint8 controllerID)
{
    if (InputDeviceCount >= INPUTDEVICE_COUNT)
        return NULL;

    if (InputDevices[InputDeviceCount] && InputDevices[InputDeviceCount]->active)
        return NULL;

    if (InputDevices[InputDeviceCount])
        delete InputDevices[InputDeviceCount];

    InputDevices[InputDeviceCount] = new InputDeviceSDL();

    InputDeviceSDL *device = (InputDeviceSDL *)InputDevices[InputDeviceCount];

    device->controllerPtr = SDL_GameControllerOpen(controllerID);

    const char *name = SDL_GameControllerName(device->controllerPtr);

    byte controllerType = DEVICE_XBOX;
    if (strstr(name, "Xbox"))
        controllerType = DEVICE_XBOX;
    else if (strstr(name, "PS4") || strstr(name, "PS5"))
        controllerType = DEVICE_PS4;
    else if (strstr(name, "Nintendo") || strstr(name, "Switch"))
        controllerType = DEVICE_SWITCH_PRO;
    else if (strstr(name, "Saturn"))
        controllerType = DEVICE_SATURN;

    device->active      = true;
    device->disabled    = false;
    device->gamePadType = (DEVICE_API_SDL2 << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (controllerType << 0);
    device->inputID     = id;

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}
#endif

void RemoveInputDevice(InputDevice *targetDevice)
{
    if (targetDevice) {
        for (int32 d = 0; d < InputDeviceCount; ++d) {
            if (InputDevices[d] && InputDevices[d] == targetDevice) {
                uint32 inputID = targetDevice->inputID;
                targetDevice->CloseDevice();
                InputDeviceCount--;

                delete InputDevices[d];
                InputDevices[d] = NULL;

                for (int32 id = d + 1; id < InputDeviceCount; ++id) InputDevices[id - 1] = InputDevices[id];

                for (int32 id = 0; id < PLAYER_COUNT; ++id) {
                    if (activeControllers[id] == inputID)
                        activeInputDevices[id] = NULL;
                }

                for (int32 id = 0; id < PLAYER_COUNT; ++id) {
                    for (int32 c = 0; c < InputDeviceCount; ++c) {
                        if (InputDevices[c] && InputDevices[c]->inputID == activeControllers[id]) {
                            if (activeInputDevices[id] != InputDevices[c])
                                activeInputDevices[id] = InputDevices[c];
                        }
                    }
                }
            }
        }
    }
}

#if RETRO_INPUTDEVICE_XINPUT
void InitXInputAPI()
{
    char idString[16];  
    
    sprintf(idString, "%s", "XInputDevice0");

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        idString[12] = '0' + i;

        uint32 id;
        GenerateCRC(&id, idString);

        InputDeviceXInput *device = NULL;
        for (int32 d = 0; d < InputDeviceCount; ++d) {
            if (InputDevices[d] && InputDevices[d]->inputID == id) {
                device = (InputDeviceXInput *)InputDevices[d];
                break;
            }
        }

        XINPUT_STATE pState;
        if (XInputGetState(i, &pState)) {
            if (device)
                RemoveInputDevice(device);
        }
        else if (!device) {
            device = InitXInputDevice(id);
            if (device)
                device->controllerID = i;
        }
    }
}
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
void InitHIDAPI()
{
    RAWINPUTDEVICE pRawInputDevices;
    pRawInputDevices.hwndTarget  = RenderDevice::windowHandle;
    pRawInputDevices.usUsagePage = 1;
    pRawInputDevices.usUsage     = 5;
    pRawInputDevices.dwFlags     = 0;

    if (!RegisterRawInputDevices(&pRawInputDevices, 1, sizeof(pRawInputDevices)))
        PrintLog(PRINT_NORMAL, "Unable to Register HID Device.\n");

    HIDEnabled = true;
    InitRawInputAPI();
}

void InitRawInputAPI()
{
    if (HIDEnabled) {
        rawInputDeviceCount = 0;

        UINT puiNumDevices;
        if (GetRawInputDeviceList(0, &puiNumDevices, sizeof(tagRAWINPUTDEVICELIST))) {
            PrintLog(PRINT_NORMAL, "Could not get Input Device Count.\n");
            return;
        }

        tagRAWINPUTDEVICELIST *pRawInputDeviceList;
        RSDK::AllocateStorage(sizeof(tagRAWINPUTDEVICELIST) * puiNumDevices, (void **)&pRawInputDeviceList, RSDK::DATASET_TMP, false);
        if (!pRawInputDeviceList) {
            PrintLog(PRINT_NORMAL, "Could not allocate memory for Input Device List.\n");
            return;
        }

        int32 rawInputListSize = GetRawInputDeviceList(pRawInputDeviceList, (PUINT)&puiNumDevices, sizeof(tagRAWINPUTDEVICELIST));
        if (rawInputListSize == -1) {
            PrintLog(PRINT_NORMAL, "Wrong Device Count.\n");
        }
        else {

            RID_DEVICE_INFO deviceInfo;
            deviceInfo.cbSize         = sizeof(RID_DEVICE_INFO);
            uint8 deviceID            = 0;
            InputDeviceRaw *devicePtr = GetRawInputDevice(&deviceID);
            if (devicePtr) {
                for (int32 i = 0; i < InputDeviceCount; ++i) {
                    if (!InputDevices[i])
                        return;

                    InputDeviceRaw *device = (InputDeviceRaw *)InputDevices[i];

                    if ((device->gamePadType >> 16) & 0xFF == DEVICE_API_RAWINPUT) {
                        int32 id = 0;
                        for (; id < rawInputListSize; ++id) {
                            if (device->deviceHandle == pRawInputDeviceList[id].hDevice)
                                break;
                        }

                        if (id == rawInputListSize)
                            RemoveInputDevice(device);
                    }
                }
            }

            for (int32 d = 0; d < rawInputListSize; ++d) {
                uint32 deviceInfoSize = deviceInfo.cbSize;
                GetRawInputDeviceInfoA(pRawInputDeviceList[d].hDevice, RIDI_DEVICEINFO, &deviceInfo, &deviceInfoSize);

                char deviceName[0x100];
                memset(deviceName, 0, sizeof(deviceName));
                uint32 deviceNameSize = sizeof(deviceName);
                GetRawInputDeviceInfoA(pRawInputDeviceList[d].hDevice, RIDI_DEVICENAME, deviceName, &deviceNameSize);

                if (deviceInfo.dwType == RIM_TYPEHID && deviceInfo.hid.usUsage == 5) {
                    uint32 id;
                    GenerateCRC(&id, deviceName);

                    InputDeviceRaw *device = (InputDeviceRaw *)InputDeviceFromID(id);
                    if (!device) {
                        for (int32 g = 0; g < gamePadCount; ++g) {
                            if (gamePadMappings[g].productID == deviceInfo.hid.dwProductId) {
                                device = InitRawInputDevice(id);

                                if (device) {
                                    device->gamePadType |= gamePadMappings[g].type;
                                    device->deviceHandle = pRawInputDeviceList[d].hDevice;
                                    memcpy(device->buttons, gamePadMappings[g].buttons, sizeof(device->buttons));
                                    PrintLog(PRINT_NORMAL, "%s Detected - Vendor ID: %x ProductID: %x\n", gamePadMappings[g].name,
                                             deviceInfo.mouse.dwId, deviceInfo.mouse.dwNumberOfButtons);
                                    rawInputDevices[rawInputDeviceCount++] = device;
                                }
                            }
                        }
                    }
                    else {
                        rawInputDevices[rawInputDeviceCount++] = device;
                    }
                }

            }

            PrintLog(PRINT_NORMAL, "Total HID GamePad Count: %d\n", rawInputDeviceCount);
        }

        RSDK::RemoveStorageEntry((void **)&pRawInputDeviceList);
    }
}
#endif


#if RETRO_INPUTDEVICE_SDL2
void InitSDL2InputAPI()
{
    char buffer[0x100];
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

        int32 nummaps = SDL_GameControllerAddMappingsFromFile(buffer);
        if (nummaps >= 0)
            PrintLog(PRINT_NORMAL, "loaded %d controller mappings from '%s'", buffer, nummaps);
    }
}
#endif // ! RETRO_INPUTDEVICE_SDL2

void InitInputDevices()
{
    if (activeInputDevices[0])
        return;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        activeControllers[i]  = CONT_AUTOASSIGN;
        activeInputDevices[i] = NULL;
    }

#if RETRO_INPUTDEVICE_KEYBOARD
    InitKeyboardInputAPI();
#endif

#if RETRO_INPUTDEVICE_DIRECTX9
    InitHIDAPI();
#endif

#if RETRO_INPUTDEVICE_XINPUT
    InitXInputAPI();
#endif

#if RETRO_INPUTDEVICE_STEAM
    // InitSteamInputAPI();
#endif

#if RETRO_INPUTDEVICE_SDL2
    InitSDL2InputAPI();
#endif
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

    for (int i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i])
            InputDevices[i]->UpdateInput();
    }

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
                if (device && device->inputID == assign && device->active)
                    device->ProcessInput(i + 1);
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
        if (InputDevices[i] && InputDevices[i]->inputID == inputID)
            return InputDevices[i]->gamePadType;
    }

#if RETRO_REV02
    return RSDK::SKU::userCore->GetDefaultGamepadType();
#else
    int32 platform = RSDK::gameVerInfo.platform;

    switch (platform) {
        case PLATFORM_SWITCH: return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_HANDHELD << 0);
        case PLATFORM_PC:
        case PLATFORM_DEV:
        default: return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0); break;
    }
#endif
}

#if RETRO_INPUTDEVICE_XINPUT
void UpdateXInputDevices()
{
    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        disabledXInputDevices[i] = false;
    }
}
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
void UpdateRawInputButtonState(HRAWINPUT hRawInput)
{
    uint32 pcbSize;
    GetRawInputData(hRawInput, RID_INPUT, NULL, &pcbSize, sizeof(rawInputData.header)); // get size
    GetRawInputData(hRawInput, RID_INPUT, &rawInputData, &pcbSize, sizeof(rawInputData.header)); // get data

    for (int32 d = 0; d < rawInputDeviceCount; ++d) {
        InputDeviceRaw *device = (InputDeviceRaw *)rawInputDevices[d];

        if (device && device->deviceHandle == rawInputData.header.hDevice) {
            device->activeButtons = 0;
            for (int32 b = 0; b < 18; ++b) {
                int32 offset = device->buttons[b].offset;

                switch (device->buttons[b].mappingType) {
                    default:
                    case 0:
                    case 4:
                    case 5: break;

                    case 1:
                        if (((1 << (offset & 7)) & rawInputData.data.hid.bRawData[offset >> 3]) != 0)
                            device->activeButtons |= device->buttons[b].maskVal;
                        break;

                    case 2:
                        if (((1 << (offset & 7)) & rawInputData.data.hid.bRawData[offset >> 3]) == 0)
                            device->activeButtons |= device->buttons[b].maskVal;
                        break;

                    case 3:
                        switch (rawInputData.data.hid.bRawData[offset >> 3] & 0xF) {
                            case 0: device->activeButtons |= 1; break;
                            case 1: device->activeButtons |= 9; break;
                            case 2: device->activeButtons |= 8; break;
                            case 3: device->activeButtons |= 10; break;
                            case 4: device->activeButtons |= 2; break;
                            case 5: device->activeButtons |= 6; break;
                            case 6: device->activeButtons |= 4; break;
                            case 7: device->activeButtons |= 5; break;
                            default: break;
                        }
                        break;
                }
            }

            for (int32 b = 18; b < 24; ++b) {
                float delta  = 0;
                int32 offset = device->buttons[b].offset;

                switch (device->buttons[b].mappingType) {
                    default:
                    case 0:
                    case 1:
                    case 2:
                    case 3: break;

                    case 4: delta = (rawInputData.data.hid.bRawData[offset >> 3] - 128.0) * -0.0078125; break;
                    case 5: delta = (rawInputData.data.hid.bRawData[offset >> 3] - 128.0) * 0.0078125; break;
                }

                switch (b) {
                    case 18: device->triggerDeltaL = delta; break;
                    case 19: device->triggerDeltaR = delta; break;
                    case 20: device->hDelta_L = delta; break;
                    case 21: device->vDelta_L = delta; break;
                    case 22: device->hDelta_R = delta; break;
                    case 23: device->vDelta_R = delta; break;
                }
            }
        }
    }
}
#endif