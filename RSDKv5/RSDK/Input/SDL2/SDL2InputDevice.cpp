
using namespace RSDK::SKU;

#define NORMALIZE(val, minVal, maxVal) ((float)(val) - (float)(minVal)) / ((float)(maxVal) - (float)(minVal))

bool32 getControllerButton(InputDeviceSDL *device, uint8 buttonID)
{
    if (buttonID == (uint8)SDL_CONTROLLER_BUTTON_INVALID || !device)
        return false;

    if (SDL_GameControllerGetButton(device->controllerPtr, (SDL_GameControllerButton)buttonID))
        return true;

    return false;
}

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

    this->stateUp       = (this->inputFlags & KEYMASK_UP) != 0;
    this->stateDown     = (this->inputFlags & KEYMASK_DOWN) != 0;
    this->stateLeft     = (this->inputFlags & KEYMASK_LEFT) != 0;
    this->stateRight    = (this->inputFlags & KEYMASK_RIGHT) != 0;
    this->stateA        = (this->inputFlags & swapABXY ? KEYMASK_B : KEYMASK_A) != 0;
    this->stateB        = (this->inputFlags & swapABXY ? KEYMASK_A : KEYMASK_B) != 0;
    this->stateC        = (this->inputFlags & KEYMASK_C) != 0;
    this->stateX        = (this->inputFlags & swapABXY ? KEYMASK_Y : KEYMASK_X) != 0;
    this->stateY        = (this->inputFlags & swapABXY ? KEYMASK_X : KEYMASK_Y) != 0;
    this->stateZ        = (this->inputFlags & KEYMASK_Z) != 0;
    this->stateStart    = (this->inputFlags & KEYMASK_START) != 0;
    this->stateSelect   = (this->inputFlags & KEYMASK_SELECT) != 0;
    this->stateBumper_L = (this->inputFlags & KEYMASK_BUMPERL) != 0;
    this->stateBumper_R = (this->inputFlags & KEYMASK_BUMPERR) != 0;
    this->stateStick_L  = (this->inputFlags & KEYMASK_STICKL) != 0;
    this->stateStick_R  = (this->inputFlags & KEYMASK_STICKR) != 0;

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
    triggerL[controllerID].bumperDelta  = this->triggerDeltaL;
    triggerL[controllerID].triggerDelta = this->triggerDeltaL;

    triggerR[controllerID].keyBumper.press |= this->stateBumper_R;
    triggerR[controllerID].keyTrigger.press |= this->triggerDeltaR > 0.3;
    triggerR[controllerID].bumperDelta  = this->triggerDeltaR;
    triggerR[controllerID].triggerDelta = this->triggerDeltaR;
}

void InputDeviceSDL::CloseDevice()
{
    this->active               = false;
    this->assignedControllerID = false;
    SDL_GameControllerClose(this->controllerPtr);
    this->controllerPtr = NULL;
}

InputDeviceSDL *RSDK::SKU::InitSDL2InputDevice(uint32 id, uint8 controllerID)
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
    char buffer[0x100];
    SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
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
            RSDK::PrintLog(PRINT_NORMAL, "loaded %d controller mappings from '%s'", buffer, nummaps);
    }
}