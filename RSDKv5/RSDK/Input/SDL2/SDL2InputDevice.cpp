
using namespace RSDK;

#define NORMALIZE(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

bool32 getControllerButton(RSDK::SKU::InputDeviceSDL *device, uint8 buttonID)
{
    if (buttonID == (uint8)SDL_CONTROLLER_BUTTON_INVALID || !device)
        return false;

    if (SDL_GameControllerGetButton(device->controllerPtr, (SDL_GameControllerButton)buttonID))
        return true;

    return false;
}

void RSDK::SKU::InputDeviceSDL::UpdateInput()
{
    int32 buttonMap[] = {
        SDL_CONTROLLER_BUTTON_DPAD_UP,   SDL_CONTROLLER_BUTTON_DPAD_DOWN,  SDL_CONTROLLER_BUTTON_DPAD_LEFT,    SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
        SDL_CONTROLLER_BUTTON_A,         SDL_CONTROLLER_BUTTON_B,          SDL_CONTROLLER_BUTTON_INVALID,      SDL_CONTROLLER_BUTTON_X,
        SDL_CONTROLLER_BUTTON_Y,         SDL_CONTROLLER_BUTTON_INVALID,    SDL_CONTROLLER_BUTTON_START,        SDL_CONTROLLER_BUTTON_BACK,
        SDL_CONTROLLER_BUTTON_LEFTSTICK, SDL_CONTROLLER_BUTTON_RIGHTSTICK, SDL_CONTROLLER_BUTTON_LEFTSHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    };

    int32 keyMasks[] = { KEYMASK_UP, KEYMASK_DOWN, KEYMASK_LEFT,  KEYMASK_RIGHT,  KEYMASK_A,      KEYMASK_B,      KEYMASK_C,       KEYMASK_X,
                         KEYMASK_Y,  KEYMASK_Z,    KEYMASK_START, KEYMASK_SELECT, KEYMASK_STICKL, KEYMASK_STICKR, KEYMASK_BUMPERL, KEYMASK_BUMPERR };

    this->prevButtonMasks = this->buttonMasks;

    float prevHDeltaL       = hDelta_L;
    float prevVDeltaL       = vDelta_L;
    float prevHDeltaR       = hDelta_R;
    float prevVDeltaR       = vDelta_R;
    float prevTriggerDeltaL = triggerDeltaL;
    float prevTriggerDeltaR = triggerDeltaR;

    this->buttonMasks = 0;
    for (int32 i = 0; i < KEY_MAX + 4; ++i) {
        if (getControllerButton(this, buttonMap[i]))
            this->buttonMasks |= keyMasks[i];
    }

    int32 delta = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_LEFTX);
    if (delta < 0)
        hDelta_L = -NORMALIZE(-delta, 1, 32768);
    else
        hDelta_L = NORMALIZE(delta, 0, 32767);

    delta = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_LEFTY);
    if (delta < 0)
        vDelta_L = -NORMALIZE(-delta, 1, 32768);
    else
        vDelta_L = NORMALIZE(delta, 0, 32767);
    vDelta_L = -vDelta_L;

    delta = SDL_GameControllerGetAxis(controllerPtr, SDL_CONTROLLER_AXIS_RIGHTX);
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

    bumperDeltaL = (this->buttonMasks & KEYMASK_BUMPERL) != 0 ? 1.0 : 0.0;
    bumperDeltaR = (this->buttonMasks & KEYMASK_BUMPERR) != 0 ? 1.0 : 0.0;

    int32 changedButtons = ~this->prevButtonMasks & (this->prevButtonMasks ^ this->buttonMasks);

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

    this->stateUp       = (this->buttonMasks & KEYMASK_UP) != 0;
    this->stateDown     = (this->buttonMasks & KEYMASK_DOWN) != 0;
    this->stateLeft     = (this->buttonMasks & KEYMASK_LEFT) != 0;
    this->stateRight    = (this->buttonMasks & KEYMASK_RIGHT) != 0;
    this->stateA        = (this->buttonMasks & (swapABXY ? KEYMASK_B : KEYMASK_A)) != 0;
    this->stateB        = (this->buttonMasks & (swapABXY ? KEYMASK_A : KEYMASK_B)) != 0;
    this->stateC        = (this->buttonMasks & KEYMASK_C) != 0;
    this->stateX        = (this->buttonMasks & (swapABXY ? KEYMASK_Y : KEYMASK_X)) != 0;
    this->stateY        = (this->buttonMasks & (swapABXY ? KEYMASK_X : KEYMASK_Y)) != 0;
    this->stateZ        = (this->buttonMasks & KEYMASK_Z) != 0;
    this->stateStart    = (this->buttonMasks & KEYMASK_START) != 0;
    this->stateSelect   = (this->buttonMasks & KEYMASK_SELECT) != 0;
    this->stateBumper_L = (this->buttonMasks & KEYMASK_BUMPERL) != 0;
    this->stateBumper_R = (this->buttonMasks & KEYMASK_BUMPERR) != 0;
    this->stateStick_L  = (this->buttonMasks & KEYMASK_STICKL) != 0;
    this->stateStick_R  = (this->buttonMasks & KEYMASK_STICKR) != 0;

    ProcessInput(CONT_ANY);
}

void RSDK::SKU::InputDeviceSDL::ProcessInput(int32 controllerID)
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

#if RETRO_REV02
    stickL[controllerID].keyStick.press |= this->stateStick_L;
    stickL[controllerID].hDelta = this->hDelta_L;
    stickL[controllerID].vDelta = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    stickR[controllerID].keyStick.press |= this->stateStick_R;
    stickR[controllerID].hDelta = this->vDelta_R;
    stickR[controllerID].vDelta = this->hDelta_R;
    stickR[controllerID].keyUp.press |= this->vDelta_R > INPUT_DEADZONE;
    stickR[controllerID].keyDown.press |= this->vDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyLeft.press |= this->hDelta_R < -INPUT_DEADZONE;
    stickR[controllerID].keyRight.press |= this->hDelta_R > INPUT_DEADZONE;

    triggerL[controllerID].keyBumper.press |= this->stateBumper_L;
    triggerL[controllerID].keyTrigger.press |= this->triggerDeltaL > INPUT_DEADZONE;
    triggerL[controllerID].bumperDelta  = this->bumperDeltaL;
    triggerL[controllerID].triggerDelta = this->triggerDeltaL;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].keyTrigger.press |= this->triggerDeltaR > INPUT_DEADZONE;
    triggerR[controllerID].bumperDelta  = this->bumperDeltaR;
    triggerR[controllerID].triggerDelta = this->triggerDeltaR;
#else
    controller[controllerID].keyStickL.press |= this->stateStick_L;
    stickL[controllerID].hDeltaL = this->hDelta_L;
    stickL[controllerID].vDeltaL = this->vDelta_L;
    stickL[controllerID].keyUp.press |= this->vDelta_L > INPUT_DEADZONE;
    stickL[controllerID].keyDown.press |= this->vDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyLeft.press |= this->hDelta_L < -INPUT_DEADZONE;
    stickL[controllerID].keyRight.press |= this->hDelta_L > INPUT_DEADZONE;

    controller[controllerID].keyStickR.press |= this->stateStick_R;
    stickL[controllerID].hDeltaR = this->vDelta_R;
    stickL[controllerID].vDeltaR = this->hDelta_R;

    controller[controllerID].keyBumperL.press |= this->stateBumper_L;
    controller[controllerID].keyTriggerL.press |= this->triggerDeltaL > INPUT_DEADZONE;
    stickL[controllerID].triggerDeltaL = this->triggerDeltaL;

    controller[controllerID].keyBumperR.press |= this->stateBumper_R;
    controller[controllerID].keyTriggerR.press |= this->triggerDeltaR > INPUT_DEADZONE;
    stickL[controllerID].triggerDeltaR = this->triggerDeltaR;
#endif
}

void RSDK::SKU::InputDeviceSDL::CloseDevice()
{
    this->active               = false;
    this->assignedControllerID = false;
    SDL_GameControllerClose(this->controllerPtr);
    this->controllerPtr = NULL;
}

RSDK::SKU::InputDeviceSDL *RSDK::SKU::InitSDL2InputDevice(uint32 id, uint8 controllerID)
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

    device->swapABXY     = false;
    uint8 controllerType = DEVICE_XBOX;

    if (strstr(name, "Xbox"))
        controllerType = DEVICE_XBOX;
    else if (strstr(name, "PS4") || strstr(name, "PS5"))
        controllerType = DEVICE_PS4;
    else if (strstr(name, "Nintendo") || strstr(name, "Switch")) {
        controllerType = DEVICE_SWITCH_PRO;
        device->swapABXY = true;
    }
    else if (strstr(name, "Saturn"))
        controllerType = DEVICE_SATURN;

    device->active      = true;
    device->disabled    = false;
    device->gamePadType = (DEVICE_API_SDL2 << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (controllerType << 0);
    device->inputID     = id;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        if (activeControllers[i] == id) {
            activeInputDevices[i]        = device;
            device->assignedControllerID = true;
        }
    }

    InputDeviceCount++;
    return device;
}

void RSDK::SKU::InitSDL2InputAPI()
{
    SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    return;

    // TODO: this
    for (int32 g = 0; g < gamePadCount; ++g) {
        char mappingBuffer[0x100];

        sprintf_s(mappingBuffer, sizeof(mappingBuffer),
                  "%d,%s,"
                  "a:b1,b:b2,y:b3,x:b0,start:b9,guide:b12,back:b8,"
                  "dpup:hI%f,dpleft:h%f,dpdown:h%f,dpright:h%f,"
                  "leftshoulder:b4,rightshoulder:b5,leftstick:b10,rightstick:b11,"
                  "leftx:a0,lefty:a1,rightx:a2,righty:a3,lefttrigger:b6,righttrigger:b7",
                  gamePadMappings[g].productID, gamePadMappings[g].name, INPUT_DEADZONE, INPUT_DEADZONE, INPUT_DEADZONE, INPUT_DEADZONE);

        if (SDL_GameControllerAddMapping(mappingBuffer) >= 0) {
            // char deviceName[0x100];
            // memset(deviceName, 0, sizeof(deviceName));
            // 
            // uint32 id;
            // GenerateHashCRC(&id, deviceName);
            // device = InitSDL2InputDevice(id);
            // 
            // device->gamePadType |= gamePadMappings[g].type;
            // memcpy(device->buttons, gamePadMappings[g].buttons, sizeof(device->buttons));
            // PrintLog(PRINT_NORMAL, "%s Detected - Vendor ID: %x ProductID: %x", gamePadMappings[g].name, deviceInfo.mouse.dwId,
            //                deviceInfo.mouse.dwNumberOfButtons);
        }
    }
}
