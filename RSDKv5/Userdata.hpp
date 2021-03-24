#ifndef USERDATA_H
#define USERDATA_H

#if RETRO_USE_PLUS
#define RETRO_USERDB_MAX (8)
#define RETRO_USERDB_VAL_MAX (8)
#define RETRO_USERDB_ENTRY_MAX (0x400)

struct DummyCore {
    void (*unknown1)();
    void (*unknown2)();
    void (*unknown3)();
    void (*unknown4)();
    void (*SetupDebugValues)();
    void (*UserInitUnknown1)();
    void (*UserInitUnknown2)();
    int (*GetUserLanguage)();
    int (*GetUserRegion)();
    int (*GetUserPlatform)();
    int (*GetConfirmButtonFlip)();
    void (*LaunchManual)();
    void (*ExitGame)();
    void (*unknown14)();
    int (*unknown15)(byte inputID);
    bool32 (*CheckDLC)(byte id);
    int (*ShowExtensionOverlay)(byte overlay);

    int* values[8];
    byte debugValCnt;
    ushort field_25;
    byte field_27;
};

struct DummyAchievements {
    void (*InitUnknown1)();
    void (*SetDebugValues)();
    void (*InitUnknown2)();
    void (*UnlockAchievement)(const char* name);

    int status;
};

struct DummyLeaderboards {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    int (*unknown4)(void);
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

    int status;
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

struct UserDBValue {
    int size;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
};

struct UserDBEntry {
    UserDB *parent;
    uint uuid;
    tm timeStampA;
    tm timeStampB;
    UserDBValue values[8];
};

struct UserDB {
    const char *name;
    int uuid;
    byte loaded;
    byte active;
    byte field_A;
    byte field_B;
    UserDB *field_C;
    byte status;
    byte field_11;
    byte field_12;
    byte field_13;
    int field_14;
    int field_18;
    int entryUnknown[RETRO_USERDB_ENTRY_MAX];
    short entryUnknownCount;
    byte field_101E;
    byte field_101F;
    int valueCount;
    int valueIDs[RETRO_USERDB_VAL_MAX];
    byte valueNames[RETRO_USERDB_VAL_MAX][0x10];
    int userDB[RETRO_USERDB_VAL_MAX];
    ushort entryCount;
    short field_10E6;
    UserDBEntry entries[RETRO_USERDB_ENTRY_MAX];
};

struct UserDBStorage {
    UserDB userDB[RETRO_USERDB_MAX];
    int field_228740;
    int *writeBuffer[RETRO_USERDB_MAX];
    void *readBuffer[RETRO_USERDB_MAX];
    int (*saveCallback[RETRO_USERDB_MAX])(int);
    int (*field_2287A4[RETRO_USERDB_MAX])(int);
    int (*field_2287C4[RETRO_USERDB_MAX])(int, int);
    int field_2287E4[RETRO_USERDB_MAX];
    int (*callbacks[RETRO_USERDB_MAX])(int);
    int field_228824[RETRO_USERDB_MAX];
};

extern UserDBStorage userDBStorage[RETRO_USERDB_MAX];
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
#if RETRO_USE_PLUS
inline bool32 checkDLC(byte dlcID)
{
    if (dlcID < 8)
        return *userCore->values[dlcID];
    else
        return 0;
}
#endif
inline int UserCoreUnknown15(byte inputID) { return 0; }
int ShowExtensionOverlay(byte overlay);

inline int GetAchievementsStatus() { return achievements->status; }
inline void SetAchievementsStatus(int status) { achievements->status = status; }
void TryUnlockAchievement(const char *name);

void FetchLeaderboard(int a2, int a3);
void TrackScore(int a2, int a3, int a4);

void SetPresence(byte a2, TextInfo *info);

inline int GetStatsStatus() { return stats->status; }
inline void SetStatsStatus(int status) { stats->status = status; }

inline int GetUserStorageStatus() { return userStorage->storageStatusCode; }
inline int UserStorageStatusUnknown1()
{
    if (userStorage->statusCode == 500)
        return 500;
    else
        return userStorage->statusCode;
}
inline int UserStorageStatusUnknown2()
{
    if (userStorage->noSaveActive)
        return 200;
    else
        return userStorage->statusCode;
}
inline int UserStorageStatusUnknown3()
{
    if (userStorage->statusCode == 100)
        userStorage->statusCode = 200;
    return userStorage->statusCode;
}
inline int UserStorageStatusUnknown4()
{
    if (userStorage->statusCode == 100)
        userStorage->statusCode = 403;
    return userStorage->statusCode;
}
inline int UserStorageStatusUnknown5()
{
    if (userStorage->statusCode == 100)
        userStorage->statusCode = 500;
    return userStorage->statusCode;
}
inline int ClearUserStorageStatus()
{
    userStorage->statusCode = 0;
    return userStorage->statusCode;
}
inline int SetUserStorageStatus()
{
    userStorage->statusCode = 100;
    return userStorage->statusCode;
}
inline int SetUserStorageNoSave(int state)
{
    userStorage->noSaveActive = state;
    return userStorage->noSaveActive;
}
inline int GetUserStorageNoSave() { return userStorage->noSaveActive; }

inline int TryAuth()
{
    userStorage->authStatusCode = 200;
    return true;
}
inline int TryInitStorage()
{
    userStorage->storageStatusCode = 200;
    return true;
}
inline int GetUserName(TextInfo *info)
{
    SetText(info, (char*)"IntegerGeorge802", 0);
    return true;
}
inline void UserStorageUnknown8()
{
    if (userStorage->authStatusCode != 200)
        userStorage->authStatusCode = 0;

    userStorage->field_14 = 0;
    if (userStorage->storageStatusCode != 200)
        userStorage->storageStatusCode = 0;
}

void setupUserDebugValues();
void userInitUnknown1();
void userInitUnknown2();

#if !RETRO_USE_PLUS
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

bool32 TryLoadUserFile(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int));
bool32 TrySaveUserFile(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int), bool32 compress);
bool32 TryDeleteUserFile(const char *filename, int (*callback)(int));

bool32 LoadUserFile(const char *filename, void *buffer, unsigned int bufSize);
bool32 SaveUserFile(const char *filename, void *buffer, unsigned int bufSize);
bool32 DeleteUserFile(const char *filename);

ushort LoadUserDB(const char *filename, int (*callback)(int));
bool32 SaveUserDB(ushort tableID, int (*callback)(int));

ushort GetUserDBByID(ushort tableID, uint uuid);

inline bool32 Engine_UserUnknown43(ushort tableID)
{
    if (tableID == (ushort)-1)
        return false;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return false;
    userDB->entryCount = 0;
    memset(userDB->entries, 0, RETRO_USERDB_ENTRY_MAX * sizeof(UserDBEntry));
    //Engine_UserDB_UpdateValueSizes(userDB);
    return 1;
}

extern bool32 settingsChanged;

int GetSettingsValue(int id);
void SetSettingsValue(int id, int val);

void readSettings();
void writeSettings(bool32 writeToFile);

#endif
