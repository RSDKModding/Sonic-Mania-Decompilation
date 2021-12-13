#ifndef USERDATA_H
#define USERDATA_H

enum StatusCodes { STATUS_NONE = 0, STATUS_CONTINUE = 100, STATUS_OK = 200, STATUS_FORBIDDEN = 403, STATUS_NOTFOUND = 404, STATUS_ERROR = 500 };

#define API_TypeOf(strct, type) ((type *)strct)

#if RETRO_REV02
struct SKUInfo {
    int32 platform;
    int32 language;
    int32 region;
};

struct UnknownInfo {
    int32 unknown1;
    int32 unknown2;
    int32 unknown3;
    int32 unknown4;
    bool32 pausePress;
    int32 unknown5;
    int32 unknown6;
    int32 unknown7;
    int32 unknown8;
    int32 unknown9;
    bool32 anyPress;
    int32 unknown10;
};

extern SKUInfo curSKU;
extern UnknownInfo unknownInfo;

#define Unknown_pausePress UnknownInfo->pausePress
#define Unknown_anyPress   UnknownInfo->anyPress
#else
#define Unknown_pausePress TouchInfo->pausePress
#define Unknown_anyPress   TouchInfo->anyPress
#endif

#if RETRO_REV02

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserCore {
    virtual void Shutdown(void) {}
    virtual bool32 CheckAPIInitialized(void) { return true; }
    virtual bool32 CheckFocusLost(void) { return false; }
    //I do not know what this is, both PC & Switch vers have it as return false always so
    virtual bool32 CheckEnginePause(void) { return false; }
    virtual void StageLoad(void);
    virtual void FrameInit(void);
    // I do not know what this is, both PC & Switch vers never call it as far as I can see
    virtual void OnUnknownEvent(void);
    virtual int32 GetUserLanguage(void) { return curSKU.language; }
    virtual int32 GetUserRegion(void) { return curSKU.region; }
    virtual int32 GetUserPlatform(void) { return curSKU.platform; }
    virtual bool32 GetConfirmButtonFlip(void) { return false; }
    virtual void LaunchManual(void) {}
    virtual void ExitGame(void) {}
    virtual int32 GetDefaultGamepadType(void) { return 0; }
    virtual bool32 IsOverlayEnabled(uint32 inputID) { return false; }
    virtual bool32 CheckDLC(byte id)
    {
        if (id < 8)
            return *values[id];
        else
            return 0;
    }
    virtual int32 ShowExtensionOverlay(byte overlay) { return 0; }
#if RETRO_VER_EGS
    virtual void EpicUnknown1(void) {}
    virtual bool32 ShowCheckoutPage(int a1) { return false; }
    virtual int32 ShowEncorePage(int a1) { return 0; }
    virtual void EpicUnknown4(int a1) {}
    virtual void RegisterHIDDevice(void) {}
    virtual void EpicUnknown6(void) {}
#endif

    int *values[8];
    byte valueCount = 0;

    //Not Original, but I gotta store it somewhere /shrug
    uint8 focusState = 0;
};

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamUserCore" would be added and "userCore" would be set to that instead
struct DummyCore : public UserCore {
    bool32 CheckFocusLost(void);
    bool32 GetConfirmButtonFlip(void);
    void LaunchManual(void);
    void ExitGame(void);
    int32 GetDefaultGamepadType(void);
    bool32 IsOverlayEnabled(uint32 overlay)
    {
        for (int i = 0; i < InputDeviceCount; ++i) {
            if (InputDevices[i].inputID == overlay) {
                uint8 flag = InputDevices[i].gamePadType >> 16;
                if (flag != DEVICE_FLAG_STEAMOVERLAY) {
                    return false;
                }

                return false; // sorry!
            }
        }

        return false;
    }
    // CheckDLC(id)
    int32 ShowExtensionOverlay(byte overlay);
#if RETRO_VER_EGS
    void EpicUnknown1(void) {}
    bool32 ShowCheckoutPage(int a1);
    int32 ShowEncorePage(int a1);
    void EpicUnknown4(int a1);
    void RegisterHIDDevice(void) {}
    void EpicUnknown6(void) {}
#endif

    uint16 field_25;
    uint8 field_27;
};

extern DummyCore *dummyCore;
extern UserCore *userCore;
#endif

void InitUserData();
void releaseUserData();
void saveUserData();

#if RETRO_REV02
DummyCore *InitDummyCore();

//Platform-Specific Cores go here
#endif

#if RETRO_REV02
void HandleUserStatuses();
#endif

//these are rev02 only but keeping em helps organization
uint32 GetAPIValueID(const char *identifier, int charIndex);
int32 GetAPIValue(uint32 id);

#if RETRO_REV02
inline int32 GetUserLanguage(void) { return userCore->GetUserLanguage(); }
inline bool32 GetConfirmButtonFlip(void) { return userCore->GetConfirmButtonFlip(); }
inline void ExitGame(void) { userCore->ExitGame(); }
inline void LaunchManual(void) { userCore->LaunchManual(); }
inline bool32 IsOverlayEnabled(uint32 overlay) { return userCore->IsOverlayEnabled(overlay); }
inline bool32 CheckDLC(byte id) { return userCore->CheckDLC(id); }
inline int32 ShowExtensionOverlay(byte overlay) { return userCore->ShowExtensionOverlay(overlay); }
#else
bool32 GetConfirmButtonFlip();
void LaunchManual();
void ExitGame();
int GetDefaultGamepadType();
int ShowExtensionOverlay(byte overlay);
#endif

bool32 GetXYButtonFlip();

#if RETRO_VER_EGS
inline bool32 ShowCheckoutPage(int a1) { return userCore->ShowCheckoutPage(a1); }
inline int32 ShowEncorePage(int a1) { return userCore->ShowEncorePage(a1); }
inline void EpicUnknown4(int a1) { userCore->EpicUnknown4(a1); }
inline void RegisterHIDDevice(void) { userCore->RegisterHIDDevice(); }
#endif

struct SettingsStorage {
    byte windowed;
    byte bordered;
    byte exclusiveFS;
    byte vsync;
    byte shaderSupport;
    int fsWidth;
    int fsHeight;
    int gameHeight;
    int windowActive;
    int shaderID;
    int screenCount;
    int dimTimer;
    int dimLimit;
    float dimMax;
    float dimPercent;
    int refreshRate;
    int windowWidth;
    int windowHeight;
    int pixWidth;
    int mouseX;
    int mouseY;
    int field_8;
    int field_C;
};

struct GamePadButtonMap {
    int maskVal;
    short mappingType;
    short offset;
};

struct GamePadMappings {
    char name[0x40];
    GamePadButtonMap buttons[24];
    int vendorID;
    int productID;
    int type;
};

extern GamePadMappings *gamePadMappings;
extern int gamePadCount;

enum SettingsValues {
    SETTINGS_WINDOWED,
    SETTINGS_BORDERED,
    SETTINGS_EXCLUSIVEFS,
    SETTINGS_VSYNC,
    SETTINGS_TRIPLEBUFFERED,
    SETTINGS_WINDOW_WIDTH,
    SETTINGS_WINDOW_HEIGHT,
    SETTINGS_FSWIDTH,
    SETTINGS_FSHEIGHT,
    SETTINGS_REFRESHRATE,
    SETTINGS_SHADERSUPPORT,
    SETTINGS_SHADERID,
    SETTINGS_SCREENCOUNT,
#if RETRO_REV02
    SETTINGS_DIMTIMER,
#endif
    SETTINGS_STREAMSENABLED,
    SETTINGS_STREAM_VOL,
    SETTINGS_SFX_VOL,
    SETTINGS_LANGUAGE,
    SETTINGS_STORE,
    SETTINGS_RELOAD,
    SETTINGS_CHANGED,
    SETTINGS_WRITE,
};

extern SettingsStorage settingsStorage;
extern bool32 settingsChanged;

int GetSettingsValue(int id);
void SetSettingsValue(int id, int val);

void readSettings();
void writeSettings(bool32 writeToFile);

inline void writeText(FileIO *file, const char *string, ...)
{
    // make the full string
    char buffer[0x100];
    va_list args;
    va_start(args, string);
    vsprintf(buffer, string, args);
    sprintf(buffer, "%s", buffer);
    va_end(args);

    fWrite(buffer, sizeof(char), strlen(buffer), file);
}

#endif
