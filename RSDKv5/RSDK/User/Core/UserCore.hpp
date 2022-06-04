#ifndef USER_CORE_H
#define USER_CORE_H

namespace RSDK
{

namespace SKU
{

#undef STATUS_TIMEOUT

enum StatusCodes {
    STATUS_NONE      = 0,
    STATUS_CONTINUE  = 100,
    STATUS_OK        = 200,
    STATUS_FORBIDDEN = 403,
    STATUS_NOTFOUND  = 404,
    STATUS_ERROR     = 500,
    STATUS_NOWIFI    = 503,
    STATUS_TIMEOUT   = 504,
    STATUS_CORRUPT   = 505,
    STATUS_NOSPACE   = 506,
};

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

#define Unknown_pausePress  UnknownInfo->pausePress
#define Unknown_anyKeyPress UnknownInfo->anyPress
#else
#define Unknown_pausePress  TouchInfo->pausePress
#define Unknown_anyKeyPress TouchInfo->anyPress
#endif

#if RETRO_REV02

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserCore {
    virtual void Shutdown() {}
    virtual bool32 CheckAPIInitialized() { return true; }
    virtual bool32 CheckFocusLost() { return false; }
    virtual bool32 CheckEnginePause() { return false; }
    virtual void StageLoad();
    virtual void FrameInit();
    // I do not know what this is, both PC & Switch vers never call it as far as I can see
    virtual void OnUnknownEvent();
    virtual int32 GetUserLanguage() { return LANGUAGE_EN; }
    virtual int32 GetUserRegion() { return REGION_US; }
    virtual int32 GetUserPlatform() { return PLATFORM_PC; }
    virtual bool32 GetConfirmButtonFlip() { return false; }
    virtual void LaunchManual() {}
    virtual void ExitGame() {}
    virtual int32 GetDefaultGamepadType() { return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0); }
    virtual bool32 IsOverlayEnabled(uint32 inputID) { return false; }
    virtual bool32 CheckDLC(uint8 id)
    {
        if (id < 8)
            return values[id];
        else
            return 0;
    }
#if RETRO_VER_EGS
    virtual bool32 SetupExtensionOverlay() { return true; }
    virtual bool32 CanShowExtensionOverlay(int32 overlay) { return true; }
    virtual bool32 ShowExtensionOverlay(int32 overlay) { return false; }
    virtual bool32 CanShowAltExtensionOverlay(int32 overlay) { return false; }
    virtual bool32 ShowAltExtensionOverlay(int32 overlay) { return ShowExtensionOverlay(overlay); }
    virtual int32 GetConnectingStringID() { return -1; }
    virtual bool32 ShowLimitedVideoOptions(int32 id) { return false; }
    virtual void InitInputDevices() {}
    virtual void Unknown() {}
#else
    virtual bool32 ShowExtensionOverlay(uint8 overlay) { return false; }
#endif

    bool32 values[8] = { false, false, false, false, false, false, false, false };
    uint8 valueCount = 0;

    // Not Original, but I gotta store it somewhere /shrug
    uint8 focusState = 0;
};

extern UserCore *userCore;

// ====================
// API Cores
// ====================

// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyCore.hpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamCore.hpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSCore.hpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXCore.hpp"
#endif

#endif

void InitUserData();
void ReleaseUserCore();
void SaveUserData();

#if RETRO_REV02
void HandleUserStatuses();
#endif

#if RETRO_REV02
inline int32 GetUserLanguage() { return userCore->GetUserLanguage(); }
inline bool32 GetConfirmButtonFlip() { return userCore->GetConfirmButtonFlip(); }
inline void ExitGame() { userCore->ExitGame(); }
inline void LaunchManual() { userCore->LaunchManual(); }
inline bool32 IsOverlayEnabled(uint32 overlay) { return userCore->IsOverlayEnabled(overlay); }
inline bool32 CheckDLC(uint8 id) { return userCore->CheckDLC(id); }
inline bool32 ShowExtensionOverlay(int32 overlay) { return userCore->ShowExtensionOverlay(overlay); }
#else
bool32 GetConfirmButtonFlip();
void LaunchManual();
void ExitGame();
int32 GetDefaultGamepadType();
bool32 ShowExtensionOverlay(uint8 overlay);
#endif

bool32 GetXYButtonFlip();

#if RETRO_VER_EGS
inline bool32 SetupExtensionOverlay() { return userCore->SetupExtensionOverlay(); }
inline bool32 CanShowExtensionOverlay(int32 overlay) { return userCore->CanShowExtensionOverlay(overlay); }
inline bool32 CanShowAltExtensionOverlay(int32 overlay) { return userCore->CanShowAltExtensionOverlay(overlay); }
inline bool32 ShowAltExtensionOverlay(int32 overlay) { return userCore->ShowAltExtensionOverlay(overlay); }
inline int32 GetConnectingStringID() { return userCore->GetConnectingStringID(); }
inline void ShowLimitedVideoOptions(int32 id) { userCore->ShowLimitedVideoOptions(id); }
#endif

} // namespace SKU

#if !RETRO_USE_ORIGINAL_CODE
struct CustomSettings {
    int8 region;
    bool32 confirmButtonFlip;
    bool32 xyButtonFlip;
    bool32 enableControllerDebugging;
    bool32 disableFocusPause;
    int32 maxPixWidth;
    char username[0x80];
};

extern CustomSettings customSettings;
#endif

void LoadSettingsINI();
void SaveSettingsINI(bool32 writeToFile);

inline void WriteText(FileIO *file, const char *string, ...)
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
