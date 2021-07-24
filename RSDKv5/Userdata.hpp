#ifndef USERDATA_H
#define USERDATA_H

#if RETRO_REV02
#define RETRO_USERDB_MAX (8)
#define RETRO_USERDB_VAL_MAX (8)
#define RETRO_USERDB_ENTRY_MAX (0x400)

enum StatusCodes { STATUS_NONE = 0, STATUS_CONTINUE = 100, STATUS_OK = 200, STATUS_FORBIDDEN = 403, STATUS_NOTFOUND = 404, STATUS_ERROR = 500 };

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
    int (*GetConfirmButtonFlip)(void);
    void (*LaunchManual)(void);
    void (*ExitGame)(void);
    int (*controllerUnknown)(void);
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

    int* values[8];
    byte debugValCnt;
    ushort field_25;
    byte field_27;
};

struct DummyAchievements {
    void (*InitUnknown1)(void);
    void (*SetDebugValues)(void);
    void (*InitUnknown2)(void);
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
    bool32 (*CheckAchievementsEnabled)(void);
    void (*GetAchievementNames)(TextInfo *names, int count);
    TextInfo *(*GetAchievementText)(TextInfo *info);
    TextInfo *(*GetAchievementName)(TextInfo *info, uint id);
    bool32 (*Unknown8)(void);
    int (*GetNextAchievementID)(void);
    void (*RemoveLastAchievementID)(void);
#endif
    void (*UnlockAchievement)(const char* name);

    int status;
#if RETRO_VER_EGS

#endif
};

struct DummyLeaderboards {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    int (*unknown4)(void);  
#if RETRO_VER_EGS
    int (*unknown6)(void);
#endif
    void (*FetchLeaderboard)(int a2, int a3);
    void (*unknown5)(void);
    void (*TrackScore)(int a2, int a3, int a4);
    int (*unknown7)(void);

    byte field_4;
    byte field_5;
    byte field_6;
    byte field_7;
    byte field_8;
    byte field_9;
    byte field_A;
    byte field_B;
    byte field_C;
    byte field_D;
    byte field_E;
    byte field_F;
    byte field_10;
    ushort field_11;
    byte field_13;
    byte field_14;
    byte field_15;
    byte field_16;
    byte field_17;
    byte field_18;
    byte field_19;
    byte field_1A;
    byte field_1B;
    byte field_1C;
    byte field_1D;
    byte field_1E;
    byte field_1F;
    byte field_20[848];
};

struct DummyRichPresence {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    void (*SetPresence)(byte a2, TextInfo *info);

    int curID;
};

struct DummyStats {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    void (*TryTrackStat)(void);

    int status;
};

struct DummyUserStorage {
    int (*InitUnknown1)(void);
    int (*SetDebugValues)(void);
    int (*InitUnknown2)(void);
    int (*TryAuth)(void);
    int (*TryInitStorage)(void);
    int (*GetUsername)(TextInfo*);
    bool32 (*LoadUserFile)(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int));
    bool32 (*SaveUserFile)(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int), bool32 compress);
    bool32 (*DeleteUserFile)(const char *filename, int (*callback)(int));
    void (*unknown8)(void);

    int authStatusCode;
    int storageStatusCode;
    int statusCode;
    int noSaveActive;
    int field_14;
    int field_18;
    char field_1C[12];
};

extern DummyCore *dummmyCore;
extern DummyCore *userCore;

extern DummyAchievements *achievements;
extern DummyLeaderboards *leaderboards;
extern DummyRichPresence *richPresence;
extern DummyStats *stats;
extern DummyUserStorage *userStorage;

struct UserDB;
struct UserDBEntry;

struct UserDBValue {
    UserDBEntry *parent;
    int type;
    int size;
    int value;
    int field_10;
    int field_14;
};

struct UserDBEntry {
    UserDB *parent;
    uint uuid;
    tm createTime;
    tm changeTime;
    UserDBValue values[8];
};

struct UserDB {
    const char *name;
    uint uuid;
    byte loaded;
    byte active;
    byte valid;
    byte field_B;
    UserDB *field_C;
    byte status;
    int field_14;
    int field_18;
    int rowUnknown[RETRO_USERDB_ENTRY_MAX];
    int rowUnknownCount;
    int columnCount;
    int columnSizes[RETRO_USERDB_VAL_MAX];
    char columnNames[RETRO_USERDB_VAL_MAX][0x10];
    uint columnUUIDs[RETRO_USERDB_VAL_MAX];
    ushort entryCount;
    UserDBEntry rows[RETRO_USERDB_ENTRY_MAX];
};

struct UserDBStorage {
    UserDB userDB[RETRO_USERDB_MAX];
    int field_228740;
    int *writeBuffer[RETRO_USERDB_MAX];
    void *readBuffer[RETRO_USERDB_MAX];
    int (*saveCallback[RETRO_USERDB_MAX])(int);
    int (*field_2287A4[RETRO_USERDB_MAX])(int);
    int (*loadCallback[RETRO_USERDB_MAX])(ushort table, int status);
    int field_2287E4[RETRO_USERDB_MAX];
    int (*callbacks[RETRO_USERDB_MAX])(int);
    int field_228824[RETRO_USERDB_MAX];
};

extern UserDBStorage userDBStorage[RETRO_USERDB_MAX];
#endif


#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
extern bool32 achievementsEnabled;
extern ushort achievementAniFrames[2];
extern Animator achievementAnimator[2];
extern TextInfo achievementText[2];
extern int achievementTextWidth[2];
extern int achievementID;
extern int achievementsDelay;
extern int achievementsDrawn;
extern int achievementStrW;
extern int achievementStrX;
extern bool32 achievementsLoaded;
extern bool32 achievementDrawFlag;
extern bool32 achievementUnknownFlag;

void LoadAchievementAssets();
void ProcessAchievements();
void DrawAchievements();
#endif

struct StatInfo {
    byte statID;
    const char *name;
    void *data[64];
};


void initUserData();
void releaseUserData();

int GetUserLanguage();
int GetUserRegion();
int GetUserPlatform();
int GetConfirmButtonFlip();
void LaunchManual();
void ExitGame();
int controllerUnknown();
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

#if RETRO_REV02
inline int GetAchievementsStatus() { return achievements->status; }
inline void SetAchievementsStatus(int status) { achievements->status = status; }
#endif
inline bool32 CheckAchievementsEnabled() { return true; }
inline bool32 AchivementsUnknown8() { return true; }
void GetAchievementNames(TextInfo *names, int count);
TextInfo *GetAchievementText(TextInfo *info);
TextInfo *GetAchievementName(TextInfo *info, uint id);
int GetNextAchievementID(void);
void RemoveLastAchievementID(void);
void TryUnlockAchievement(const char *name);

void FetchLeaderboard(int a2, int a3);
void TrackScore(int a2, int a3, int a4);

void SetPresence(byte a2, TextInfo *info);

#if RETRO_REV02
inline int GetStatsStatus() { return stats->status; }
inline void SetStatsStatus(int status) { stats->status = status; }
#endif

inline int GetUserAuthStatus() { return STATUS_OK; }
inline int GetUserStorageStatus()
{
#if RETRO_REV02
    return userStorage->authStatusCode; 
#else
    return STATUS_OK;
#endif
}
#if RETRO_REV02
inline int UserStorageStatusUnknown1()
{
    if (userStorage->statusCode == STATUS_ERROR)
        return STATUS_ERROR;
    else
        return userStorage->statusCode;
}
inline int UserStorageStatusUnknown2()
{
    if (userStorage->noSaveActive)
        return STATUS_OK;
    else
        return userStorage->statusCode;
}
inline int UserStorageStatusUnknown3()
{
    if (userStorage->statusCode == STATUS_CONTINUE)
        userStorage->statusCode = STATUS_OK;
    return userStorage->statusCode;
}
inline int UserStorageStatusUnknown4()
{
    if (userStorage->statusCode == STATUS_CONTINUE)
        userStorage->statusCode = STATUS_FORBIDDEN;
    return userStorage->statusCode;
}
inline int UserStorageStatusUnknown5()
{
    if (userStorage->statusCode == STATUS_CONTINUE)
        userStorage->statusCode = STATUS_ERROR;
    return userStorage->statusCode;
}
inline int ClearUserStorageStatus()
{
    userStorage->statusCode = STATUS_NONE;
    return userStorage->statusCode;
}
inline int SetUserStorageStatus()
{
    userStorage->statusCode = STATUS_CONTINUE;
    return userStorage->statusCode;
}
inline int SetUserStorageNoSave(int state)
{
    userStorage->noSaveActive = state;
    return userStorage->noSaveActive;
}
inline int GetUserStorageNoSave() { return userStorage->noSaveActive; }
#endif

inline int TryAuth()
{
#if RETRO_REV02
    userStorage->authStatusCode = STATUS_OK;
    return userStorage->authStatusCode;
#else
    return STATUS_OK;
#endif
}
inline int TryInitStorage()
{
#if RETRO_REV02
    userStorage->storageStatusCode = STATUS_OK;
    return userStorage->storageStatusCode;
#else
    return 0;
#endif
}
inline int GetUserName(TextInfo *info)
{
    SetText(info, (char*)"IntegerGeorge802", 0);
    return true;
}
#if RETRO_REV02
inline void UserStorageUnknown8()
{
    if (userStorage->authStatusCode != STATUS_OK)
        userStorage->authStatusCode = STATUS_NONE;

    userStorage->field_14 = 0;
    if (userStorage->storageStatusCode != STATUS_OK)
        userStorage->storageStatusCode = STATUS_NONE;
}
#endif

#if !RETRO_REV02
void TrackActClear();
void TrackTAClear(byte a1, byte a2, byte a3, int a4);
void TrackEnemyDefeat();
void ClearPrerollErrors();
#endif

void setupUserDebugValues();
void userInitUnknown1();
void userInitUnknown2();

#if !RETRO_REV02
#define FUNCLIST_COUNT (0x30)
struct FunctionListEntry {
    void *ptr;
    uint hash[4];
};

extern FunctionListEntry functionList[FUNCLIST_COUNT];
extern int functionListCount;

void SetFuncPtr(const char *name, void *ptr);
void *GetFuncPtr(const char *name);
#endif

extern void (*userFileCallback)();
extern void (*userFileCallback2)();
extern char userFileDir[0x100];

inline void SetUserFileCallbacks(const char *userDir, void (*callback1)(void), void (*callback2)(void))
{
    userFileCallback  = callback1;
    userFileCallback2 = callback2;
    strcpy(userFileDir, userDir);
}

bool32 TryLoadUserFile(const char *filename, void *buffer, uint bufSize, int (*callback)(int));
bool32 TrySaveUserFile(const char *filename, void *buffer, uint bufSize, int (*callback)(int), bool32 compress);
bool32 TryDeleteUserFile(const char *filename, int (*callback)(int));

bool32 LoadUserFile(const char *filename, void *buffer, uint bufSize);
bool32 SaveUserFile(const char *filename, void *buffer, uint bufSize);
bool32 DeleteUserFile(const char *filename);

#if RETRO_REV02
inline void UpdateUserDBParents(UserDB *userDB)
{
    for (int r = 0; r < RETRO_USERDB_ENTRY_MAX; ++r) {
        userDB->rows[r].parent = userDB;
        for (int c = 0; c < userDB->columnCount; ++c) {
            userDB->rows[r].values[c].parent = &userDB->rows[r];
        }
    }
}

inline void InitUserDBValues(UserDB *userDB, va_list list)
{
    int cnt = 0;
    while ((int *)list) {
        userDB->columnSizes[cnt] = va_arg(list, int);
        memcpy(userDB->columnNames[cnt], 0, 0x10);
        sprintf(userDB->columnNames[cnt], "%s", va_arg(list, const char*));
        GenerateCRC(&userDB->columnUUIDs[cnt], userDB->columnNames[cnt]);
        ++cnt;
    }

    userDB->columnCount = cnt;
    userDB->entryCount  = 0;
    memset(userDB->rows, 0, sizeof(UserDBEntry) * RETRO_USERDB_ENTRY_MAX);
    UpdateUserDBParents(userDB);
    userDB->active = true;
    userDB->valid  = true;
}

inline ushort InitUserDB(const char *name, ...)
{
    int tableID = -1;
    uint uuid   = 0;
    GenerateCRC(&uuid, (char *)name);
    //for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
    //    if (uuid == userDBStorage->userDB[i].uuid)
    //        return i;
    //}

    for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
        if (!userDBStorage->userDB[i].loaded) {
            tableID = i;
            break;
        }
    }

    va_list list;
    va_start(list, name);

    userDBStorage->userDB[tableID].loaded = true;
    userDBStorage->userDB[tableID].name   = name;
    GenerateCRC(&userDBStorage->userDB[tableID].uuid, (char *)name);
    InitUserDBValues(&userDBStorage->userDB[tableID], list);
    //sub_5EC5F0(&userDBStorage->userDB[v6].parent);
    va_end(list);
    return tableID;
}

ushort LoadUserDB(const char *filename, int (*callback)(int));
bool32 SaveUserDB(ushort tableID, int (*callback)(int));

inline void ClearUserDB(ushort tableID)
{
    if (tableID == 0xFFFF)
        return;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->loaded) {
        /// sub_5EB5D0((_DWORD **)&userDB->field_14);
        userDB->loaded      = false;
        userDB->active      = false;
        userDB->valid       = false;
        userDB->entryCount  = 0;
        userDB->columnCount = 0;
        memset(&userDB->columnSizes, 0, sizeof(int) * RETRO_USERDB_VAL_MAX);
        memset(&userDB->columnNames, 0, (0x10 * sizeof(char)) * RETRO_USERDB_VAL_MAX);
        memset(&userDB->columnUUIDs, 0, sizeof(uint) * RETRO_USERDB_VAL_MAX);
        memset(userDB->rows, 0, 0x44000u);
        userDB->status = true;
    }
}

inline void ClearAllUserDBs()
{
    for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
        UserDB *userDB = &userDBStorage->userDB[i];
        if (userDB->loaded) {
            /// sub_5EB5D0((_DWORD **)&userDB->field_14);
            userDB->loaded      = false;
            userDB->active      = false;
            userDB->valid       = false;
            userDB->entryCount  = 0;
            userDB->columnCount = 0;
            memset(&userDB->columnSizes, 0, sizeof(int) * RETRO_USERDB_VAL_MAX);
            memset(&userDB->columnNames, 0, (0x10 * sizeof(char)) * RETRO_USERDB_VAL_MAX);
            memset(&userDB->columnUUIDs, 0, sizeof(uint) * RETRO_USERDB_VAL_MAX);
            memset(userDB->rows, 0, sizeof(UserDBEntry) * RETRO_USERDB_ENTRY_MAX);
            userDB->status = true;
        }
    }
}

inline ushort GetUserDBByID(ushort tableID, uint uuid)
{
    if (tableID == 0xFFFF)
        return -1;
    if (!uuid)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return -1;
    if (!userDB->entryCount)
        return -1;

    for (int i = 0; i < userDB->entryCount; ++i) {
        if (uuid == userDB->rows[i].uuid) {
            return i;
        }
    }
    return -1;
}

inline void GetUserDBCreationTime(ushort tableID, int entryID, char *buf, size_t size, char *format)
{
    if (tableID != 0xFFFF && entryID != 0xFFFF) {
        UserDB* userDB = &userDBStorage->userDB[tableID];
        if (userDB->active)
            strftime(buf, size, format, &userDB->rows[entryID].createTime);
    }
}

inline int GetUserDBRowUnknownCount(ushort tableID)
{
    if (tableID == 0xFFFF)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->active)
        return 0;

    return userDB->rowUnknownCount;
}

inline uint GetUserDBRowUUID(ushort tableID, ushort entry)
{
    if (tableID == 0xFFFF || entry == 0xFFFF)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->active)
        return 0;

    return userDB->rows[entry].uuid;
}

inline int GetUserDBRowUnknown(ushort tableID, ushort entryID)
{
    if (tableID == 0xFFFF)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active || userDB->status || entryID >userDB->rowUnknownCount - 1)
        return -1;

    return userDB->rowUnknown[entryID];
}

inline int AddUserDBEntry(ushort tableID)
{
    if (tableID == 0xFFFF)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->active)
        return -1;

    if (userDB->entryCount >= RETRO_USERDB_ENTRY_MAX)
        return -1;

    UserDBEntry *row = &userDB->rows[userDB->entryCount];
    //row->uuid = sub_5EBDF0(userDB);
    time_t t;
    tm *tmA                                = localtime(&t);
    tm *tmB                                = localtime(&t);

    memcpy(&row->createTime, tmA, sizeof(tm));
    memcpy(&row->changeTime, tmB, sizeof(tm));

    memset(row->values, 0, sizeof(UserDBValue) * RETRO_USERDB_VAL_MAX);
    ++userDB->entryCount;
    userDB->valid = true;
    UpdateUserDBParents(userDB);
    userDB->status = true;
    return userDB->entryCount - 1;
}

inline uint RemoveDBEntry(ushort tableID, uint entryID)
{
    if (tableID == 0xFFFF || entryID == 0xFFFF)
        return 0;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->active)
        return 0;

    if (entryID < userDB->entryCount) {
        UserDBEntry *entry = &userDB->rows[entryID];
        memset(entry, 0, sizeof(UserDBEntry));

        int id = (ushort)(userDB->entryCount - entryID - 1);
        memcpy(entry, &entry[1], id * sizeof(UserDBEntry));
        memset(&entry[id + 1], 0, sizeof(UserDBEntry));

        --userDB->entryCount;
        userDB->valid = true;
        UpdateUserDBParents(userDB);
        userDB->status = true;
    }
    return true;
}

inline bool32 RemoveAllDBEntries(ushort tableID)
{
    if (tableID == 0xFFFF)
        return 0;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;
    userDB->entryCount = 0;
    memset(userDB->rows, 0, sizeof(UserDBEntry) * RETRO_USERDB_ENTRY_MAX);
    UpdateUserDBParents(userDB);
    return true;
}

inline int GetUserDBStatus(ushort tableID) { return userDBStorage->userDB[tableID].status; }

inline uint GetDBEntryUUID(ushort tableID, int entry)
{
    if (tableID == 0xFFFF || entry == 0xFFFF)
        return 0;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (userDB->active)
        return 0;

    return userDB->rows[entry].uuid;
}
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

#endif
