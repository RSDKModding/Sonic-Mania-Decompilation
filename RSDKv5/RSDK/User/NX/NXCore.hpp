#if RETRO_REV02

struct NXCore : UserCore {
    void FrameInit();
    bool32 CheckAPIInitialized() { return true; }
    bool32 CheckFocusLost() { return focusState != AppletFocusState_InFocus; }
    bool32 CheckEnginePause() { return false; }
    int32 GetUserLanguage()
    {
        printf("desiredLanguage: %s\n", "en");
        // get system language
        return LANGUAGE_EN;
    }
    int32 GetUserRegion() { return REGION_US; }
    int32 GetUserPlatform() { return PLATFORM_SWITCH; }
    bool32 GetConfirmButtonFlip() { return true; }
    void LaunchManual()
    {
        const char *manualURL = "";
        switch (curSKU.language) {
            default:
            case LANGUAGE_EN: manualURL = "https://www.sonicthehedgehog.com/mania/manual/en/"; break;
            case LANGUAGE_FR: manualURL = "https://www.sonicthehedgehog.com/mania/manual/fr/"; break;
            case LANGUAGE_IT: manualURL = "https://www.sonicthehedgehog.com/mania/manual/it/"; break;
            case LANGUAGE_GE: manualURL = "https://www.sonicthehedgehog.com/mania/manual/de/"; break;
            case LANGUAGE_SP: manualURL = "https://www.sonicthehedgehog.com/mania/manual/es/"; break;
            case LANGUAGE_JP: manualURL = "https://manual.sega.jp/sonicmania/"; break;
            case LANGUAGE_KO: manualURL = "https://manual.sega.jp/sonicmania/kr/"; break;
            case LANGUAGE_SC: manualURL = "https://manual.sega.jp/sonicmania/ct/"; break;
            case LANGUAGE_TC: manualURL = "https://www.sonicthehedgehog.com/mania/manual/en/"; break;
        }

        // open manualURL
    }
    int32 GetDefaultGamepadType() { return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_PRO << 0); }
    bool32 IsOverlayEnabled(uint32 inputID) { return false; }
    bool32 CheckDLC(uint8 id)
    {
        // check we have the DLC
        return false;
    }
    bool32 ShowExtensionOverlay(uint8 overlay)
    {
        // open plus DLC page
        return true;
    }
};

NXCore *InitNXCore();
#endif
