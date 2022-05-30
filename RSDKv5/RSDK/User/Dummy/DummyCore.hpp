#if RETRO_REV02

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamUserCore" would be added and "userCore" would be set to that instead
struct DummyCore : UserCore {
    void StageLoad();
    bool32 CheckFocusLost();
    int32 GetUserLanguage();
    int32 GetUserRegion();
    int32 GetUserPlatform();
    bool32 GetConfirmButtonFlip();
    void LaunchManual();
    void ExitGame();
    int32 GetDefaultGamepadType();
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
    // CheckDLC(id)
    int32 ShowExtensionOverlay(uint8 overlay);
#if RETRO_VER_EGS
    void EpicUnknown1() {}
    bool32 ShowCheckoutPage(int32 a1);
    int32 ShowEncorePage(int32 a1);
    void EpicUnknown4(int32 a1);
    void RegisterHIDDevice() {}
    void EpicUnknown6() {}
#endif

    uint16 field_25;
    uint8 field_27;
};

extern DummyCore *dummyCore;

DummyCore *InitDummyCore();

void HandleUserStatuses();

#endif

// these are rev02 only but keeping em helps organization
uint32 GetAPIValueID(const char *identifier, int32 charIndex);
int32 GetAPIValue(uint32 id);
