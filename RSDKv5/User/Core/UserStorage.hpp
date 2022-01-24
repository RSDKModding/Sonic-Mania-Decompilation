#ifndef USER_STORAGE_H
#define USER_STORAGE_H

namespace RSDK
{

namespace SKU
{

#if RETRO_REV02

#define RETRO_USERDB_MAX     (8)
#define RETRO_USERDB_COL_MAX (8)
#define RETRO_USERDB_ROW_MAX (0x400)

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserStorage {
    virtual void FrameInit(void) {}
    virtual void StageLoad(void) {}
    virtual void OnUnknownEvent(void) {}
    virtual int32 TryAuth(void) { return 0; }
    virtual int32 TryInitStorage(void) { return 0; }
    virtual bool32 GetUsername(TextInfo *name) { return false; }
    virtual bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32)) { return false; }
    virtual bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress) { return false; }
    virtual bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32)) { return false; }
    virtual void ClearPrerollErrors(void) {}

    int32 authStatus    = 0;
    int32 storageStatus = 0;
    int32 saveStatus    = 0;
    int32 noSaveActive  = false;
};

// This is the "dummy" struct, it serves as the base in the event a suitable API isn't loaded (such as in this decomp)
// This struct should never be removed, other structs such as "SteamUserStorage" would be added and "userStorage" would be set to that instead
struct DummyUserStorage : UserStorage {
    int32 TryAuth(void);
    int32 TryInitStorage(void);
    bool32 GetUsername(TextInfo *name);
    bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32));
    bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress);
    bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32));
    void ClearPrerollErrors(void);

    int32 authTime        = 0;
    int32 storageInitTime = 0;
    int32 unknown[2];
};

enum DBVarTypes {
    DBVAR_INVALID,
    DBVAR_UNKNOWN1,
    DBVAR_UINT8,
    DBVAR_UINT16,
    DBVAR_UINT32,
    DBVAR_UNUSED1,
    DBVAR_INT8,
    DBVAR_INT16,
    DBVAR_INT32,
    DBVAR_UNUSED2,
    DBVAR_FLOAT,
    DBVAR_UNUSED3,
    DBVAR_UNUSED4,
    DBVAR_UNUSED5,
    DBVAR_UNUSED6,
    DBVAR_UNKNOWN2,
    DBVAR_STRING,
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
    byte rowsChanged;
    RSDK::List<int> sortedRowList;
    int sortedRowIDs[RETRO_USERDB_ROW_MAX];
    int sortedRowCount;
    int columnCount;
    int columnTypes[RETRO_USERDB_COL_MAX];
    char columnNames[RETRO_USERDB_COL_MAX][0x10];
    uint columnUUIDs[RETRO_USERDB_COL_MAX];
    ushort rowCount;
    UserDBRow rows[RETRO_USERDB_ROW_MAX];
};

struct UserDBStorage {
    UserDB userDB[RETRO_USERDB_MAX];
    byte unknown1;
    int *writeBuffer[RETRO_USERDB_MAX];
    void *readBuffer[RETRO_USERDB_MAX];
    int (*loadCallback[RETRO_USERDB_MAX])(int);
    int (*saveCallback[RETRO_USERDB_MAX])(int);
    int (*userLoadCB[RETRO_USERDB_MAX])(ushort table, int status);
    int (*userSaveCB[RETRO_USERDB_MAX])(ushort table, int status);
    void (*callbacks[RETRO_USERDB_MAX])(int);
    int unknown2[RETRO_USERDB_MAX];
};

extern UserDBStorage *userDBStorage;

inline int TryAuth(void) { return userStorage->TryAuth(); }
inline int TryInitStorage(void) { return userStorage->TryInitStorage(); }
inline bool32 GetUsername(TextInfo *name) { return userStorage->GetUsername(name); }
inline bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32))
{
    return userStorage->TryLoadUserFile(filename, buffer, size, callback);
}
inline bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress)
{
    return userStorage->TrySaveUserFile(filename, buffer, size, callback, compress);
}
inline bool32 TryDeleteUserFile(const char *filename, int (*callback)(int32)) { return userStorage->TryDeleteUserFile(filename, callback); }
inline void ClearPrerollErrors(void) { userStorage->ClearPrerollErrors(); }

int GetUserAuthStatus();
int GetUserStorageStatus();
int GetSaveStatus();
void SetSaveStatusOK();
void SetSaveStatusForbidden();
void SetSaveStatusError();
void ClearSaveStatus();
void SetSaveStatusContinue();
void SetUserStorageNoSave(int state);
int GetUserStorageNoSave();

void InitUserStorageDB(UserDBStorage *storage);
void ReleaseUserStorageDB(UserDBStorage *storage);

// UserDB Management
ushort InitUserDB(const char *name, ...);
ushort LoadUserDB(const char *filename, void (*callback)(int));
bool32 SaveUserDB(ushort tableID, void (*callback)(int));
void ClearUserDB(ushort tableID);
void ClearAllUserDBs();
ushort GetUserDBRowByID(ushort tableID, uint uuid);

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
ushort SetupUserDBRowSorting(ushort tableID);
void SetupRowSortIDs(UserDB *userDB);
void UserDBRefreshRowUnknown(UserDB *userDB);
int AddUserDBRowSortFilter(ushort tableID, int type, const char *name, void *value);
int SortUserDBRows(ushort tableID, int type, const char *name, bool32 active);
int GetSortedUserDBRowCount(ushort tableID);
int GetSortedUserDBRowID(ushort tableID, ushort entryID);

// UserDB Values
void InitUserDBValues(UserDB *userDB, va_list list);
bool32 GetUserDBValue(ushort tableID, uint rowID, int type, char *name, void *value);
bool32 SetUserDBValue(ushort tableID, uint rowID, int type, char *name, void *value);
bool32 CheckDBValueMatch(UserDBValue *valueA, int row, int column);
void StoreUserDBValue(UserDBValue *value, int type, void *data);
void RetrieveUserDBValue(UserDBValue *value, int type, void *data);

// UserDB Misc
int GetUserDBRowsChanged(ushort tableID);
void GetUserDBRowCreationTime(ushort tableID, int entryID, char *buf, size_t size, char *format);
void UpdateUserDBParents(UserDB *userDB);
size_t GetUserDBWriteSize(UserDB *userDB);
bool32 LoadDBFromBuffer(UserDB *userDB, byte *buffer);
void SaveDBToBuffer(UserDB *userDB, int totalSize, byte *buffer);
void HandleNonMatchRowRemoval(UserDB *userDB, UserDBValue *a2, int column);
void FilterSortedUserDBRows(UserDB *userDB, const char *name, void *value);
bool32 CompareUserDBValues(UserDBRow *row1, UserDBRow *row2, int type, char *name, bool32 flag);
void HandleUserDBSorting(UserDB *userDB, int type, char *name, bool32 flag);
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

extern void (*userFileCallback)();
extern void (*userFileCallback2)();
extern char userFileDir[0x100];

inline void SetUserFileCallbacks(const char *userDir, void (*callback1)(void), void (*callback2)(void))
{
    userFileCallback  = callback1;
    userFileCallback2 = callback2;
    strcpy(userFileDir, userDir);
}

bool32 LoadUserFile(const char *filename, void *buffer, uint bufSize);
bool32 SaveUserFile(const char *filename, void *buffer, uint bufSize);
bool32 DeleteUserFile(const char *filename);

#if !RETRO_REV02
bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32));
bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32));
#endif

} // namespace SKU

} // namespace RSDK

#endif