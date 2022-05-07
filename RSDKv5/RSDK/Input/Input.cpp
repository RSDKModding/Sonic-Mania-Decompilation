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

GamePadMappings *gamePadMappings = NULL;
int32 gamePadCount               = 0;

#if !RETRO_REV02
int32 mostRecentControllerID = -1;
#endif

#if RETRO_INPUTDEVICE_KEYBOARD
#include "Keyboard/KBInputDevice.cpp"
#endif

#if RETRO_INPUTDEVICE_XINPUT
#include "XInput/XInputDevice.cpp"
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
#include "RawInput/RawInputDevice.cpp"
#endif

#if RETRO_INPUTDEVICE_STEAM
#include "Steam/SteamInputDevice.cpp"
#endif

#if RETRO_INPUTDEVICE_SDL2
#include "SDL2/SDL2InputDevice.cpp"
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

void InitInputDevices()
{
    if (activeInputDevices[0])
        return;

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        activeControllers[i]  = CONT_ANY;
        activeInputDevices[i] = NULL;
    }

#if RETRO_INPUTDEVICE_KEYBOARD
    InitKeyboardInputAPI();
#endif

#if RETRO_INPUTDEVICE_RAWINPUT
    InitHIDAPI();
#endif

#if RETRO_INPUTDEVICE_XINPUT
    InitXInputAPI();
#endif

#if RETRO_INPUTDEVICE_STEAM
    InitSteamInputAPI();
#endif

#if RETRO_INPUTDEVICE_SDL2
    InitSDL2InputAPI();
#endif
}

void ClearInput()
{
    for (int32 i = 0; i <= PLAYER_COUNT; ++i) {
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

        stickL[i].keyUp.press    = false;
        stickL[i].keyDown.press  = false;
        stickL[i].keyLeft.press  = false;
        stickL[i].keyRight.press = false;
        stickL[i].keyStick.press = false;

        stickR[i].keyUp.press    = false;
        stickR[i].keyDown.press  = false;
        stickR[i].keyLeft.press  = false;
        stickR[i].keyRight.press = false;
        stickR[i].keyStick.press = false;

        triggerL[i].keyBumper.press  = false;
        triggerL[i].keyTrigger.press = false;

        triggerR[i].keyBumper.press  = false;
        triggerR[i].keyTrigger.press = false;
    }
}

void ProcessInput()
{
    ClearInput();

    for (int32 i = 0; i < InputDeviceCount; ++i) {
        if (InputDevices[i])
            InputDevices[i]->UpdateInput();
    }

    for (int32 i = 0; i < PLAYER_COUNT; ++i) {
        int32 assign = activeControllers[i];
        if (assign && assign != CONT_UNASSIGNED) {
            if (assign == CONT_AUTOASSIGN) {
                int32 id             = GetControllerInputID();
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

#if !RETRO_REV02 && RETRO_INPUTDEVICE_KEYBOARD
    HandleSpecialKeys();
#endif

    for (int32 c = 0; c <= PLAYER_COUNT; ++c) {
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

            for (int32 i = 0; i < 12; ++i) {
                if (cont[i]->press) {
                    if (cont[i]->down)
                        cont[i]->press = false;
                    else
                        cont[i]->down = true;
                }
                else
                    cont[i]->down = false;
            }

            for (int32 i = 0; i < 5; ++i) {
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

            for (int32 i = 0; i < 2; ++i) {
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

int32 GetControllerType(int32 inputID)
{
    for (int32 i = 0; i < InputDeviceCount; ++i) {
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