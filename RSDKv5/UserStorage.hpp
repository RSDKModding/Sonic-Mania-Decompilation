#ifndef USER_STORAGE_H
#define USER_STORAGE_H

#define RETRO_USERDB_MAX     (8)
#define RETRO_USERDB_COL_MAX (8)
#define RETRO_USERDB_ROW_MAX (0x400)

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
    UserDB *parent;
    byte status;
    List<int> unknown;
    int rowUnknown[RETRO_USERDB_ROW_MAX];
    int rowUnknownCount;
    int columnCount;
    int columnTypes[RETRO_USERDB_COL_MAX];
    char columnNames[RETRO_USERDB_COL_MAX][0x10];
    uint columnUUIDs[RETRO_USERDB_COL_MAX];
    ushort rowCount;
    UserDBRow rows[RETRO_USERDB_ROW_MAX];
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

void setupUserDebugValues();
void userInitUnknown1();
void userInitUnknown2();

int GetUserAuthStatus();
int GetUserStorageStatus();
#if RETRO_REV02
int UserStorageStatusUnknown1();
int GetSaveStatus();
void SetSaveStatusOK();
void SetSaveStatusForbidden();
void SetSaveStatusError();
void ClearUserStorageStatus();
void SetUserStorageStatus();
void SetUserStorageNoSave(int state);
int GetUserStorageNoSave();
#endif

int TryAuth();
int TryInitStorage();
bool32 GetUserName(TextInfo *info);
#if RETRO_REV02
void ClearPrerollErrors();
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
void InitUserStorageDB(UserDBStorage *storage);
void ReleaseUserStorageDB(UserDBStorage *storage);

// UserDB Management
ushort InitUserDB(const char *name, ...);
ushort LoadUserDB(const char *filename, void (*callback)(int));
bool32 SaveUserDB(ushort tableID, void (*callback)(int));
void ClearUserDB(ushort tableID);
void ClearAllUserDBs();
ushort GetUserDBByID(ushort tableID, uint uuid);

// UserDB Columns
bool32 AddUserDBColumn(UserDBRow *userDB, int type, char *name, void *value);
int GetDBColumnID(UserDB *userDB, const char *name);
bool32 GetUserDBColumn(UserDBRow *userDB, int type, char *name, void *value);

// UserDB Rows
int AddUserDBRow(ushort tableID);
uint RemoveDBRow(ushort tableID, uint rowID);
bool32 RemoveAllDBRows(ushort tableID);
uint GetDBRowUUID(ushort tableID, int rowID);

// UserDB Row Unknowns
ushort InitDBRowUnknown(ushort tableID);
void SetupRowUnknown(UserDB *userDB);
void UserDBRefreshRowUnknown(UserDB *userDB);
int UserDBUnknown33(ushort tableID, int type, const char *name, void *value);
int UserDBUnknown34(ushort tableID, int type, const char *name, bool32 active);
int GetUserDBRowUnknownCount(ushort tableID);
int GetUserDBRowUnknown(ushort tableID, ushort entryID);

// UserDB Values
void InitUserDBValues(UserDB *userDB, va_list list);
bool32 GetUserDBValue(ushort tableID, int rowID, int type, char *name, void *value);
bool32 SetUserDBValue(ushort tableID, int rowID, int type, char *name, void *value);
bool32 CheckDBValueMatch(UserDBValue *valueA, int row, int column);
void StoreUserDBValue(UserDBValue *value, int type, void *data);
void RetrieveUserDBValue(UserDBValue *value, int type, void *data);

// UserDB Misc
int GetUserDBStatus(ushort tableID);
void GetUserDBCreationTime(ushort tableID, int entryID, char *buf, size_t size, char *format);
void UpdateUserDBParents(UserDB *userDB);
size_t GetUserDBWriteSize(UserDB *userDB);
int LoadDBFromBuffer(UserDB *userDB, byte *buffer);
void SaveDBToBuffer(UserDB *userDB, int totalSize, byte *buffer);
void RemoveNonMatchingDBValues(UserDB *userDB, UserDBValue *a2, int column);
void RemoveNonMatchingRows(UserDB *userDB, const char *name, void *value);
void UserDBUnknown34_Func(UserDB *userDB, int size, const char *name, bool32 valueActive);
uint CreateRowUUID(UserDB *userDB);

// User Storage CBs
int UserDBLoadCB(ushort tableID, int status);
int UserDBSaveCB(ushort tableID, int status);

int UserDBStorage_LoadCB1(int status);
int UserDBStorage_LoadCB2(int status);
int UserDBStorage_LoadCB3(int status);
int UserDBStorage_LoadCB4(int status);
int UserDBStorage_LoadCB5(int status);
int UserDBStorage_LoadCB6(int status);
int UserDBStorage_LoadCB7(int status);
int UserDBStorage_LoadCB8(int status);

int UserDBStorage_SaveCB1(int status);
int UserDBStorage_SaveCB2(int status);
int UserDBStorage_SaveCB3(int status);
int UserDBStorage_SaveCB4(int status);
int UserDBStorage_SaveCB5(int status);
int UserDBStorage_SaveCB6(int status);
int UserDBStorage_SaveCB7(int status);
int UserDBStorage_SaveCB8(int status);
#endif

#endif