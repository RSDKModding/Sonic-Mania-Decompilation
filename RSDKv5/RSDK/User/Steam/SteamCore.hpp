#if RETRO_REV02

struct SteamCore : UserCore {
    void Shutdown()
    {
        // closes steam API
    }
    bool32 CheckAPIInitialized()
    {
        // check if steam is running
        return true;
    }
    bool32 CheckFocusLost()
    {
        // return field_38;
        return false;
    }
    void FrameInit()
    {
        UserCore::StageLoad();
        // SteamAPI_RunCallbacks()
    }
    int32 GetUserLanguage()
    {
        // gets the language from steam
        return LANGUAGE_EN;
    }
    int32 GetUserRegion() { return REGION_US; }
    int32 GetUserPlatform() { return PLATFORM_PC; }
    bool32 GetConfirmButtonFlip() { return false; }
    void LaunchManual() {}
    void ExitGame() { RenderDevice::isRunning = false; }
    bool32 IsOverlayEnabled(uint32 overlay)
    {
        for (int32 i = 0; i < InputDeviceCount; ++i) {
            if (InputDevices[i] && InputDevices[i]->inputID == overlay) {
                if (((InputDevices[i]->gamePadType >> 16) & 0xFF) != DEVICE_API_STEAM)
                    return false;

                return false; // not implemented, sorry!
            }
        }

        return false;
    }
    bool32 ShowExtensionOverlay(uint8 overlay)
    {
        // show steam overlay
        return true;
    }

    bool32 initialized = false;
};

SteamCore *InitSteamCore();
#endif
