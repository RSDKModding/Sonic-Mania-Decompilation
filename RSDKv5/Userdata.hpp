#ifndef USERDATA_H
#define USERDATA_H

enum StatusCodes { STATUS_NONE = 0, STATUS_CONTINUE = 100, STATUS_OK = 200, STATUS_FORBIDDEN = 403, STATUS_NOTFOUND = 404, STATUS_ERROR = 500 };

#if RETRO_REV02

struct DummyCore {
    void (*unknown1)(void);
    void (*unknown2)(void);
    void (*unknown3)(void);
    void (*unknown4)(void);
    void (*SetupDebugValues)(void);
    void (*UserInitUnknown1)(void);
    void (*UserInitUnknown2)(void);
    int (*GetUserLanguage)(void);
    int (*GetUserRegion)(void);
    int (*GetUserPlatform)(void);
    bool32 (*GetConfirmButtonFlip)(void);
    void (*LaunchManual)(void);
    void (*ExitGame)(void);
    int (*ControllerUnknown)(void);
    int (*unknown15)(byte inputID);
    int (*CheckDLC)(byte id);
    int (*ShowExtensionOverlay)(byte overlay);
#if RETRO_VER_EGS
    void (*EpicUnknown1)(void);
    bool32 (*Epic_Checkout)(int a1);
    int (*ShowEncorePage)(int a1);
    void (*EpicUnknown4)(int a1);
    void (*RegisterHIDDevice)(void);
    void (*EpicUnknown6)(void);
#endif

    int *values[8];
    byte debugValCnt;
    ushort field_25;
    byte field_27;
};

struct DummyRichPresence {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    void (*SetPresence)(byte a2, TextInfo *info);

    int curID;
};

struct StatInfo {
    byte statID;
    const char *name;
    void *data[64];
};

struct DummyStats {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    void (*TryTrackStat)(StatInfo *stat);

    int status;
};

extern DummyCore *dummmyCore;
extern DummyCore *userCore;

extern DummyRichPresence *richPresence;
extern DummyStats *stats;
#endif

void initUserData();
void releaseUserData();
void saveUserData();

int GetUserLanguage();
int GetUserRegion();
int GetUserPlatform();
bool32 GetConfirmButtonFlip();
bool32 GetXYButtonFlip();
void LaunchManual();
void ExitGame();
int ControllerUnknown();
#if RETRO_REV02
inline int checkDLC(byte dlcID)
{
    if (dlcID < 8)
        return *userCore->values[dlcID];
    else
        return 0;
}
#endif
inline int UserCoreUnknown15(byte inputID) { return 0; }
int ShowExtensionOverlay(byte overlay);
bool32 EGS_Checkout(int a1);
int ShowEncorePage(int a1);
void EGS_Unknown4(int a1);

void SetPresence(byte id, TextInfo *info);

void TryTrackStat(StatInfo *stat);

//Rev01 ver of TrackStat basically
#if !RETRO_REV02
void TrackActClear();
void TrackTAClear(byte a1, byte a2, byte a3, int a4);
void TrackEnemyDefeat();
void ClearPrerollErrors();
#endif

#if RETRO_REV02
inline int GetStatsStatus() { return stats->status; }
inline void SetStatsStatus(int status) { stats->status = status; }
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
