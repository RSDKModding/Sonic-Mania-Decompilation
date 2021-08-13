#ifndef USERDATA_H
#define USERDATA_H

#if RETRO_REV02
#define RETRO_USERDB_MAX       (8)
#define RETRO_USERDB_VAL_MAX   (8)
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
    bool32 (*GetConfirmButtonFlip)(void);
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

    int *values[8];
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
    void (*UnlockAchievement)(const char *name);

    int status;
#if RETRO_VER_EGS

#endif
};

struct LeaderboardEntry {
    TextInfo username;
#if RETRO_REV02
    TextInfo userID;
#endif
    int globalRank;
    int score;
    bool32 isUser;
    int status;
};

struct DummyLeaderboards {
    void (*SetDebugValues)(void);
    void (*InitUnknown1)(void);
    void (*InitUnknown2)(void);
    int (*unknown4)(void);
#if RETRO_VER_EGS
    int (*unknown6)(void);
#endif
    void (*FetchLeaderboard)(const char *name, int id);
    void (*unknown5)(void);
    void (*TrackScore)(const char *name, int score, void (*callback)(int status, int rank));
    int (*GetStatus)(void);

    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int prevIsUser;
    int prevRank;
    int rankScore;
    int entryStart;
    int entryLength;
    LeaderboardEntry *entryPtrs[200];
    int status;
    int downloadStatus;
    int field_364;
    int list;
    int listSize;
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

struct DummyUserStorage {
    int (*InitUnknown1)(void);
    int (*SetDebugValues)(void);
    int (*InitUnknown2)(void);
    int (*TryAuth)(void);
    int (*TryInitStorage)(void);
    bool32 (*GetUsername)(TextInfo *);
    bool32 (*LoadUserFile)(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int));
    bool32 (*SaveUserFile)(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int), bool32 compress);
    bool32 (*DeleteUserFile)(const char *filename, int (*callback)(int));
    void (*unknown8)(void);

    int authStatus;
    int statusCode;
    int saveStatus;
    int noSaveActive;
    int field_14;
    int field_18;
    int field_1C[3];
};

extern DummyCore *dummmyCore;
extern DummyCore *userCore;

extern DummyAchievements *achievements;
extern DummyLeaderboards *leaderboards;
extern DummyRichPresence *richPresence;
extern DummyStats *stats;
extern DummyUserStorage *userStorage;

struct UserDB;
struct UserDBRow;

struct UserDBValue {
    UserDBRow *parent;
    byte size;
    byte data[0x10];
};

struct UserDBRow {
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
    int columnTypes[RETRO_USERDB_VAL_MAX];
    char columnNames[RETRO_USERDB_VAL_MAX][0x10];
    uint columnUUIDs[RETRO_USERDB_VAL_MAX];
    ushort entryCount;
    UserDBRow rows[RETRO_USERDB_ENTRY_MAX];
};

struct UserDBStorage {
    UserDB userDB[RETRO_USERDB_MAX];
    byte field_228740;
    int *writeBuffer[RETRO_USERDB_MAX];
    void *readBuffer[RETRO_USERDB_MAX];
    int (*loadCallback[RETRO_USERDB_MAX])(int);
    int (*saveCallback[RETRO_USERDB_MAX])(int);
    int (*userLoadCB[RETRO_USERDB_MAX])(ushort table, int status);
    int (*userSaveCB[RETRO_USERDB_MAX])(ushort table, int status);
    void (*callbacks[RETRO_USERDB_MAX])(int);
    int field_228824[RETRO_USERDB_MAX];
};

extern UserDBStorage *userDBStorage;
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

void initUserData();
void releaseUserData();

int GetUserLanguage();
int GetUserRegion();
int GetUserPlatform();
bool32 GetConfirmButtonFlip();
bool32 GetXYButtonFlip();
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
void ClearAchievements();
void TryUnlockAchievement(const char *name);

void FillDummyLeaderboardEntries();
void FetchLeaderboard(const char *name, int id);
void TrackScore(const char *name, int score, void (*callback)(int status, int rank));
Vector2 LeaderboardEntryCount();
LeaderboardEntry *ReadLeaderboardEntry(int entryID);
inline int GetLeaderboardStatus() { return leaderboards->status; }

void SetPresence(byte id, TextInfo *info);

void TryTrackStat(StatInfo *stat);

#if RETRO_REV02
inline int GetStatsStatus() { return stats->status; }
inline void SetStatsStatus(int status) { stats->status = status; }
#endif

inline int GetUserAuthStatus() { return STATUS_OK; }
inline int GetUserStorageStatus()
{
#if RETRO_REV02
    return userStorage->authStatus;
#else
    return STATUS_OK;
#endif
}
#if RETRO_REV02
inline int UserStorageStatusUnknown1()
{
    if (userStorage->saveStatus == STATUS_ERROR)
        return STATUS_ERROR;
    else
        return userStorage->statusCode;
}
inline int GetSaveStatus()
{
    if (userStorage->noSaveActive)
        return STATUS_OK;
    else
        return userStorage->saveStatus;
}
inline void SetSaveStatusOK()
{
    if (userStorage->saveStatus == STATUS_CONTINUE)
        userStorage->saveStatus = STATUS_OK;
}
inline void SetSaveStatusForbidden()
{
    if (userStorage->saveStatus == STATUS_CONTINUE)
        userStorage->saveStatus = STATUS_FORBIDDEN;
}
inline void SetSaveStatusError()
{
    if (userStorage->saveStatus == STATUS_CONTINUE)
        userStorage->saveStatus = STATUS_ERROR;
}
inline void ClearUserStorageStatus() { userStorage->saveStatus = STATUS_NONE; }
inline void SetUserStorageStatus() { userStorage->saveStatus = STATUS_CONTINUE; }
inline void SetUserStorageNoSave(int state) { userStorage->noSaveActive = state; }
inline int GetUserStorageNoSave() { return userStorage->noSaveActive; }
#endif

inline int TryAuth()
{
#if RETRO_REV02
    userStorage->authStatus = STATUS_OK;
    return userStorage->authStatus;
#else
    return STATUS_OK;
#endif
}
inline int TryInitStorage()
{
#if RETRO_REV02
    userStorage->saveStatus = STATUS_OK;
    return userStorage->saveStatus;
#else
    return STATUS_OK;
#endif
}
inline bool32 GetUserName(TextInfo *info)
{
    SetText(info, (char *)"IntegerGeorge802", false);
    return true;
}
#if RETRO_REV02
inline void ClearPrerollErrors()
{
    if (userStorage->authStatus != STATUS_OK)
        userStorage->authStatus = STATUS_NONE;

    userStorage->field_14 = 0;
    if (userStorage->saveStatus != STATUS_OK)
        userStorage->saveStatus = STATUS_NONE;
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
    while (true) {
        int type = va_arg(list, int);
        if (type == 0)
            break;

        userDB->columnTypes[cnt] = type;
        memset(userDB->columnNames[cnt], 0, 0x10);
        sprintf(userDB->columnNames[cnt], "%s", va_arg(list, const char *));
        GenerateCRC(&userDB->columnUUIDs[cnt], userDB->columnNames[cnt]);
        ++cnt;
    }

    userDB->columnCount = cnt;
    userDB->entryCount  = 0;
    memset(userDB->rows, 0, sizeof(UserDBRow) * RETRO_USERDB_ENTRY_MAX);
    UpdateUserDBParents(userDB);
    userDB->active = true;
    userDB->valid  = true;
}

inline ushort InitUserDB(const char *name, ...)
{
    int tableID = -1;
    uint uuid   = 0;
    GenerateCRC(&uuid, (char *)name);
    // for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
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
    // sub_5EC5F0(&userDBStorage->userDB[v6].parent);
    va_end(list);
    return tableID;
}

ushort LoadUserDB(const char *filename, void (*callback)(int));
bool32 SaveUserDB(ushort tableID, void (*callback)(int));

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
        userDB->uuid        = 0;
        userDB->entryCount  = 0;
        userDB->columnCount = 0;
        memset(userDB->columnTypes, 0, sizeof(userDB->columnTypes));
        memset(userDB->columnNames, 0, sizeof(userDB->columnNames));
        memset(userDB->columnUUIDs, 0, sizeof(userDB->columnUUIDs));
        memset(userDB->rows, 0, sizeof(userDB->rows));
        userDB->status = true;
    }
}

inline void ClearAllUserDBs()
{
    for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
        ClearUserDB(i);
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

inline int GetUserDBStatus(ushort tableID) { return userDBStorage->userDB[tableID].status; }

inline void GetUserDBCreationTime(ushort tableID, int entryID, char *buf, size_t size, char *format)
{
    if (tableID != 0xFFFF && entryID != 0xFFFF) {
        UserDB *userDB = &userDBStorage->userDB[tableID];
        if (userDB->active)
            strftime(buf, size, format, &userDB->rows[entryID].createTime);
    }
}

inline int GetUserDBRowUnknownCount(ushort tableID)
{
    if (tableID == 0xFFFF)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    return userDB->rowUnknownCount;
}

inline uint GetUserDBRowUUID(ushort tableID, ushort entry)
{
    if (tableID == 0xFFFF || entry == 0xFFFF)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    return userDB->rows[entry].uuid;
}

inline int GetUserDBRowUnknown(ushort tableID, ushort entryID)
{
    if (tableID == 0xFFFF)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active || userDB->status || entryID >= userDB->rowUnknownCount)
        return -1;

    return userDB->rowUnknown[entryID];
}

inline int AddUserDBEntry(ushort tableID)
{
    if (tableID == 0xFFFF)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return -1;

    if (userDB->entryCount >= RETRO_USERDB_ENTRY_MAX)
        return -1;

    UserDBRow *row = &userDB->rows[userDB->entryCount];
    // row->uuid = sub_5EBDF0(userDB);

    time_t timeInfo;
    time(&timeInfo);
    tm *tmA = localtime(&timeInfo);
    tm *tmB = localtime(&timeInfo);

    memcpy(&row->createTime, tmA, sizeof(tm));
    memcpy(&row->changeTime, tmB, sizeof(tm));

    memset(row->values, 0, sizeof(UserDBValue) * RETRO_USERDB_VAL_MAX);
    ++userDB->entryCount;
    userDB->valid = true;
    UpdateUserDBParents(userDB);
    userDB->status = true;
    return userDB->entryCount - 1;
}

inline void StoreUserDBValue(UserDBValue *value, int type, void *data)
{
    value->size  = 0;
    memset(value->data, 0, sizeof(value->data));
    if (data) {
        switch (type) {
            case 1:
            case 2:
            case 6:
                value->size = sizeof(byte);
                memcpy(value->data, data, sizeof(byte));
                break;
            case 3:
            case 7:
                value->size = sizeof(ushort);
                memcpy(value->data, data, sizeof(ushort));
                break;
            case 4:
            case 8:
            case 10:
            case 15:
                value->size = sizeof(int);
                memcpy(value->data, data, sizeof(int));
                break;
            case 16: {
                char *string = (char *)data;
                int len      = strlen(string);
                if (len < 15) {
                    value->size = len + 1;
                    int id      = 0;
                    while (string[id]) {
                        value->data[id] = string[id];
                    }
                }
                break;
            }
            default: break;
        }
    }
}

inline bool32 AddUserDBColumn(UserDBRow *userDB, int type, char *name, void *value)
{
    UserDB *db = userDB->parent;
    uint uuid  = 0;
    GenerateCRC(&uuid, name);

    for (int c = 0; c < db->columnCount; ++c) {
        if (db->columnUUIDs[c] == uuid) {
            if (c < 0 || type != db->columnTypes[c])
                return 0;
            StoreUserDBValue(&userDB->values[c], type, value);

            time_t t;
            time(&t);
            userDB->changeTime = *localtime(&t);
            return true;
        }
    }

    return false;
}

inline bool32 SetUserDBValue(ushort tableID, int rowID, int type, char *name, void *value)
{
    if (tableID == 0xFFFF || rowID == -1 || !userDBStorage->userDB[tableID].active)
        return false;
    
    return AddUserDBColumn(&userDBStorage->userDB[tableID].rows[rowID], type, name, value);
}

inline void RetrieveUserDBValue(UserDBValue *value, int type, void *data)
{
    byte *valData = (byte *)data;
    switch (type) {
        case 1:
        case 2:
        case 6:
            memcpy(valData, value->data, sizeof(byte));
            break;
        case 3:
        case 7: memcpy(valData, value->data, sizeof(ushort));
            break;
        case 4:
        case 8:
        case 10:
        case 15: memcpy(valData, value->data, sizeof(int));
            break;
        case 16: {
            memset(valData, 0, value->size + 1);
            char *string = (char *)data;
            for (int c = 0; c < value->size; ++c) {
                valData[c] = string[c];
            }
            break;
        }
        default: break;
    }
}

inline bool32 GetUserDBColumn(UserDBRow *userDB, int type, char *name, void *value)
{
    UserDB *db = userDB->parent;
    uint uuid  = 0;
    GenerateCRC(&uuid, name);

    for (int c = 0; c < db->columnCount; ++c) {
        if (db->columnUUIDs[c] == uuid) {
            if (c < 0 || type != db->columnTypes[c])
                return 0;
            RetrieveUserDBValue(&userDB->values[c], type, value);
            return true;
        }
    }

    return false;
}

inline bool32 GetUserDBValue(ushort tableID, int rowID, int type, char *name, void *value)
{
    if (tableID == 0xFFFF || rowID == -1 || !userDBStorage->userDB[tableID].active)
        return false;
    return GetUserDBColumn(&userDBStorage->userDB[tableID].rows[rowID], type, name, value);
}

inline uint RemoveDBEntry(ushort tableID, uint entryID)
{
    if (tableID == 0xFFFF || entryID == 0xFFFF)
        return 0;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    if (entryID < userDB->entryCount) {
        UserDBRow *entry = &userDB->rows[entryID];
        memset(entry, 0, sizeof(UserDBRow));

        int id = (ushort)(userDB->entryCount - entryID - 1);
        memcpy(entry, &entry[1], id * sizeof(UserDBRow));
        memset(&entry[id + 1], 0, sizeof(UserDBRow));

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
    memset(userDB->rows, 0, sizeof(UserDBRow) * RETRO_USERDB_ENTRY_MAX);
    UpdateUserDBParents(userDB);
    return true;
}

inline uint GetDBEntryUUID(ushort tableID, int entry)
{
    if (tableID == 0xFFFF || entry == 0xFFFF)
        return 0;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    return userDB->rows[entry].uuid;
}

inline int UserDBStorage_LoadCB1(int status)
{
    if (userDBStorage->userLoadCB[0])
        return userDBStorage->userLoadCB[0](0, status);
    return 0;
}
inline int UserDBStorage_LoadCB2(int status)
{
    if (userDBStorage->userLoadCB[1])
        return userDBStorage->userLoadCB[1](1, status);
    return 0;
}
inline int UserDBStorage_LoadCB3(int status)
{
    if (userDBStorage->userLoadCB[2])
        return userDBStorage->userLoadCB[2](2, status);
    return 0;
}
inline int UserDBStorage_LoadCB4(int status)
{
    if (userDBStorage->userLoadCB[3])
        return userDBStorage->userLoadCB[3](3, status);
    return 0;
}
inline int UserDBStorage_LoadCB5(int status)
{
    if (userDBStorage->userLoadCB[4])
        return userDBStorage->userLoadCB[4](4, status);
    return 0;
}
inline int UserDBStorage_LoadCB6(int status)
{
    if (userDBStorage->userLoadCB[5])
        return userDBStorage->userLoadCB[5](5, status);
    return 0;
}
inline int UserDBStorage_LoadCB7(int status)
{
    if (userDBStorage->userLoadCB[6])
        return userDBStorage->userLoadCB[6](6, status);
    return 0;
}
inline int UserDBStorage_LoadCB8(int status)
{
    if (userDBStorage->userLoadCB[7])
        return userDBStorage->userLoadCB[7](7, status);
    return 0;
}

inline int UserDBStorage_SaveCB1(int status)
{
    if (userDBStorage->userSaveCB[0])
        return userDBStorage->userSaveCB[0](0, status);
    return 0;
}
inline int UserDBStorage_SaveCB2(int status)
{
    if (userDBStorage->userSaveCB[1])
        return userDBStorage->userSaveCB[1](1, status);
    return 0;
}
inline int UserDBStorage_SaveCB3(int status)
{
    if (userDBStorage->userSaveCB[2])
        return userDBStorage->userSaveCB[2](2, status);
    return 0;
}
inline int UserDBStorage_SaveCB4(int status)
{
    if (userDBStorage->userSaveCB[3])
        return userDBStorage->userSaveCB[3](3, status);
    return 0;
}
inline int UserDBStorage_SaveCB5(int status)
{
    if (userDBStorage->userSaveCB[4])
        return userDBStorage->userSaveCB[4](4, status);
    return 0;
}
inline int UserDBStorage_SaveCB6(int status)
{
    if (userDBStorage->userSaveCB[5])
        return userDBStorage->userSaveCB[5](5, status);
    return 0;
}
inline int UserDBStorage_SaveCB7(int status)
{
    if (userDBStorage->userSaveCB[6])
        return userDBStorage->userSaveCB[6](6, status);
    return 0;
}
inline int UserDBStorage_SaveCB8(int status)
{
    if (userDBStorage->userSaveCB[7])
        return userDBStorage->userSaveCB[7](7, status);
    return 0;
}

inline void InitUserStorageDB(UserDBStorage *storage)
{
    storage->loadCallback[0] = UserDBStorage_LoadCB1;
    storage->loadCallback[1] = UserDBStorage_LoadCB2;
    storage->loadCallback[2] = UserDBStorage_LoadCB3;
    storage->loadCallback[3] = UserDBStorage_LoadCB4;
    storage->loadCallback[4] = UserDBStorage_LoadCB5;
    storage->loadCallback[5] = UserDBStorage_LoadCB6;
    storage->loadCallback[6] = UserDBStorage_LoadCB7;
    storage->loadCallback[7] = UserDBStorage_LoadCB8;
    storage->saveCallback[0] = UserDBStorage_SaveCB1;
    storage->saveCallback[1] = UserDBStorage_SaveCB2;
    storage->saveCallback[2] = UserDBStorage_SaveCB3;
    storage->saveCallback[3] = UserDBStorage_SaveCB4;
    storage->saveCallback[4] = UserDBStorage_SaveCB5;
    storage->saveCallback[5] = UserDBStorage_SaveCB6;
    storage->saveCallback[6] = UserDBStorage_SaveCB7;
    storage->saveCallback[7] = UserDBStorage_SaveCB8;
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
