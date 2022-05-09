#ifndef USER_CORE_H
#define USER_CORE_H

namespace RSDK
{

void RegisterAchievement(const char *identifier, const char *name, const char *desc);

namespace SKU
{

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
    bool32 anyKeyPress;
    int32 unknown10;
};

extern SKUInfo curSKU;
extern UnknownInfo unknownInfo;

#define Unknown_pausePress UnknownInfo->pausePress
#define Unknown_anyKeyPress   UnknownInfo->anyPress
#else
#define Unknown_pausePress TouchInfo->pausePress
#define Unknown_anyKeyPress   TouchInfo->anyPress
#endif

#if RETRO_REV02

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserCore {
    virtual void Shutdown(void) {}
    virtual bool32 CheckAPIInitialized(void) { return true; }
    virtual bool32 CheckFocusLost(void) { return false; }
    // I do not know what this is, both PC & Switch vers have it as return false always so
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
    virtual int32 GetDefaultGamepadType(void) { return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0); }
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

    // Not Original, but I gotta store it somewhere /shrug
    uint8 focusState = 0;
};

extern UserCore *userCore;
#endif

void InitUserData();
void releaseUserData();
void saveUserData();

#if RETRO_REV02
void HandleUserStatuses();
#endif

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

} // namespace SKU

struct SettingsStorage {
    uint8 windowed;
    uint8 bordered;
    uint8 exclusiveFS;
    uint8 vsync;
    uint8 tripleBuffered;
    uint8 shaderSupport;
    int32 fsWidth;
    int32 fsHeight;
    int32 refreshRate;
    int32 windowWidth;
    int32 windowHeight;
    int32 pixWidth;
    int32 pixHeight;
    int32 windowState;
    int32 shaderID;
    int32 screenCount;
    uint32 dimTimer;
    int32 dimLimit;
    float dimMax;
    float dimPercent;
    float viewportW;
    float viewportH;
    float viewportX;
    float viewportY;
};

enum VideoSettings {
    VIDEOSETTING_WINDOWED,
    VIDEOSETTING_BORDERED,
    VIDEOSETTING_EXCLUSIVEFS,
    VIDEOSETTING_VSYNC,
    VIDEOSETTING_TRIPLEBUFFERED,
    VIDEOSETTING_WINDOW_WIDTH,
    VIDEOSETTING_WINDOW_HEIGHT,
    VIDEOSETTING_FSWIDTH,
    VIDEOSETTING_FSHEIGHT,
    VIDEOSETTING_REFRESHRATE,
    VIDEOSETTING_SHADERSUPPORT,
    VIDEOSETTING_SHADERID,
    VIDEOSETTING_SCREENCOUNT,
#if RETRO_REV02
    VIDEOSETTING_DIMTIMER,
#endif
    VIDEOSETTING_STREAMSENABLED,
    VIDEOSETTING_STREAM_VOL,
    VIDEOSETTING_SFX_VOL,
    VIDEOSETTING_LANGUAGE,
    VIDEOSETTING_STORE,
    VIDEOSETTING_RELOAD,
    VIDEOSETTING_CHANGED,
    VIDEOSETTING_WRITE,
};

extern SettingsStorage videoSettings;
extern SettingsStorage videoSettingsBackup;
extern bool32 changedVideoSettings;

int32 GetVideoSetting(int32 id);
void SetVideoSetting(int32 id, int32 val);

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

} // namespace RSDK

#endif // !USER_CORE_H
