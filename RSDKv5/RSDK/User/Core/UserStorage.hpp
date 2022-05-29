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

// no clue what this is...
// most signatures are recognisable like "SCN" for scenes
// but as far as I can tell, this one is just random numbers?
#define RETRO_USERDB_SIGNATURE (0x80074B1E)

// This is the base struct, it serves as the base for any API-specific stats
// This struct should never be removed
struct UserStorage {
    virtual void FrameInit(void) {}
    virtual void StageLoad(void) {}
    virtual void OnUnknownEvent(void) {}
    virtual int32 TryAuth(void) { return 0; }
    virtual int32 TryInitStorage(void) { return 0; }
    virtual bool32 GetUsername(String *userName) { return false; }
    virtual bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32)) { return false; }
    virtual bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress) { return false; }
    virtual bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32)) { return false; }
    virtual void ClearPrerollErrors(void) {}

    int32 authStatus    = 0;
    int32 storageStatus = 0;
    int32 saveStatus    = 0;
    int32 noSaveActive  = false;
};

static char dbVarTypes[18][16] = {
    "unknown", // unused (in Sonic Mania)
    "bool",    // unused (in Sonic Mania)
    "uint8",   // used (in Sonic Mania)
    "uint16",  // unused (in Sonic Mania)
    "uint32",  // used (in Sonic Mania)
    "uint64",  // unimplemented in RSDKv5
    "int8",    // unused (in Sonic Mania)
    "int16",   // unused (in Sonic Mania)
    "int32",   // unused (in Sonic Mania)
    "int64",   // unimplemented in RSDKv5
    "float",   // unused (in Sonic Mania)
    "double",  // unimplemented in RSDKv5
    "Vector2", // unimplemented in RSDKv5
    "Vector3", // unimplemented in RSDKv5
    "Vector4", // unimplemented in RSDKv5
    "Color",   // unused (in Sonic Mania)
    "String",  // unused (in Sonic Mania)
    "HashMD5"  // unimplemented in RSDKv5
};

enum DBVarTypes {
    DBVAR_UNKNOWN, // unused (in Sonic Mania)
    DBVAR_BOOL,    // unused (in Sonic Mania)
    DBVAR_UINT8,   // used (in Sonic Mania)
    DBVAR_UINT16,  // unused (in Sonic Mania)
    DBVAR_UINT32,  // used (in Sonic Mania)
    DBVAR_UINT64,  // unimplemented in RSDKv5
    DBVAR_INT8,    // unused (in Sonic Mania)
    DBVAR_INT16,   // unused (in Sonic Mania)
    DBVAR_INT32,   // unused (in Sonic Mania)
    DBVAR_INT64,   // unimplemented in RSDKv5
    DBVAR_FLOAT,   // unused (in Sonic Mania)
    DBVAR_DOUBLE,  // unimplemented in RSDKv5
    DBVAR_VECTOR2, // unimplemented in RSDKv5
    DBVAR_VECTOR3, // unimplemented in RSDKv5
    DBVAR_VECTOR4, // unimplemented in RSDKv5
    DBVAR_COLOR,   // unused (in Sonic Mania)
    DBVAR_STRING,  // unused (in Sonic Mania)
    DBVAR_HASHMD5, // unimplemented in RSDKv5
};

extern UserStorage *userStorage;

struct UserDB;
struct UserDBRow;

struct UserDBValue {
    UserDBRow *parent;
    uint8 size;
    uint8 data[0x10];
};

struct UserDBRow {
    UserDB *parent;
    uint32 uuid;
    tm createTime;
    tm changeTime;
    UserDBValue values[8];
};

struct UserDB {
    const char *name;
    uint32 uuid;
    uint8 loaded;
    uint8 active;
    uint8 valid;
    UserDB *parent;
    uint8 rowsChanged;
    RSDK::List<int32> sortedRowList;
    int32 sortedRowIDs[RETRO_USERDB_ROW_MAX];
    int32 sortedRowCount;
    int32 columnCount;
    int32 columnTypes[RETRO_USERDB_COL_MAX];
    char columnNames[RETRO_USERDB_COL_MAX][0x10];
    uint32 columnUUIDs[RETRO_USERDB_COL_MAX];
    uint16 rowCount;
    UserDBRow rows[RETRO_USERDB_ROW_MAX];
};

struct UserDBStorage {
    UserDB userDB[RETRO_USERDB_MAX];
    uint8 unknown1;
    int32 *writeBuffer[RETRO_USERDB_MAX];
    void *readBuffer[RETRO_USERDB_MAX];
    int32 (*loadCallback[RETRO_USERDB_MAX])(int32);
    int32 (*saveCallback[RETRO_USERDB_MAX])(int32);
    int32 (*userLoadCB[RETRO_USERDB_MAX])(uint16 table, int32 status);
    int32 (*userSaveCB[RETRO_USERDB_MAX])(uint16 table, int32 status);
    void (*callbacks[RETRO_USERDB_MAX])(int32);
    int32 unknown2[RETRO_USERDB_MAX];
};

extern UserDBStorage *userDBStorage;

inline int32 TryAuth(void) { return userStorage->TryAuth(); }
inline int32 TryInitStorage(void) { return userStorage->TryInitStorage(); }
inline bool32 GetUsername(String *userName) { return userStorage->GetUsername(userName); }
inline bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32))
{
    return userStorage->TryLoadUserFile(filename, buffer, size, callback);
}
inline bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32), bool32 compress)
{
    return userStorage->TrySaveUserFile(filename, buffer, size, callback, compress);
}
inline bool32 TryDeleteUserFile(const char *filename, int32 (*callback)(int32)) { return userStorage->TryDeleteUserFile(filename, callback); }
inline void ClearPrerollErrors(void) { userStorage->ClearPrerollErrors(); }

int32 GetUserAuthStatus();
int32 GetUserStorageStatus();
int32 GetSaveStatus();
void SetSaveStatusOK();
void SetSaveStatusForbidden();
void SetSaveStatusError();
void ClearSaveStatus();
void SetSaveStatusContinue();
void SetUserStorageNoSave(int32 state);
int32 GetUserStorageNoSave();

void InitUserStorageDB(UserDBStorage *storage);
void ReleaseUserStorageDB(UserDBStorage *storage);

// UserDB Management
uint16 InitUserDB(const char *name, ...);
uint16 LoadUserDB(const char *filename, void (*callback)(int32));
bool32 SaveUserDB(uint16 tableID, void (*callback)(int32));
void ClearUserDB(uint16 tableID);
void ClearAllUserDBs();
uint16 GetUserDBRowByID(uint16 tableID, uint32 uuid);

// UserDB Columns
bool32 AddUserDBColumn(UserDBRow *userDB, int32 type, char *name, void *value);
int32 GetDBColumnID(UserDB *userDB, const char *name);
bool32 GetUserDBColumn(UserDBRow *userDB, int32 type, char *name, void *value);

// UserDB Rows
int32 AddUserDBRow(uint16 tableID);
uint32 RemoveDBRow(uint16 tableID, uint32 rowID);
bool32 RemoveAllDBRows(uint16 tableID);
uint32 GetDBRowUUID(uint16 tableID, int32 rowID);

// UserDB Row Sorting
uint16 SetupUserDBRowSorting(uint16 tableID);
void SetupRowSortIDs(UserDB *userDB);
void UserDBRefreshRowSortList(UserDB *userDB);
int32 AddUserDBRowSortFilter(uint16 tableID, int32 type, const char *name, void *value);
int32 SortUserDBRows(uint16 tableID, int32 type, const char *name, bool32 active);
int32 GetSortedUserDBRowCount(uint16 tableID);
int32 GetSortedUserDBRowID(uint16 tableID, uint16 entryID);

// UserDB Values
void InitUserDBValues(UserDB *userDB, va_list list);
bool32 GetUserDBValue(uint16 tableID, uint32 rowID, int32 type, char *name, void *value);
bool32 SetUserDBValue(uint16 tableID, uint32 rowID, int32 type, char *name, void *value);
bool32 CheckDBValueMatch(UserDBValue *valueA, int32 row, int32 column);
void StoreUserDBValue(UserDBValue *value, int32 type, void *data);
void RetrieveUserDBValue(UserDBValue *value, int32 type, void *data);

// UserDB Misc
int32 GetUserDBRowsChanged(uint16 tableID);
void GetUserDBRowCreationTime(uint16 tableID, int32 entryID, char *buf, size_t size, char *format);
void UpdateUserDBParents(UserDB *userDB);
size_t GetUserDBWriteSize(UserDB *userDB);
bool32 LoadDBFromBuffer(UserDB *userDB, uint8 *buffer);
void SaveDBToBuffer(UserDB *userDB, int32 totalSize, uint8 *buffer);
void HandleNonMatchRowRemoval(UserDB *userDB, UserDBValue *value, int32 column);
void FilterSortedUserDBRows(UserDB *userDB, const char *name, void *value);
bool32 CompareUserDBValues(UserDBRow *row1, UserDBRow *row2, int32 type, char *name, bool32 flag);
void HandleUserDBSorting(UserDB *userDB, int32 type, char *name, bool32 flag);
uint32 CreateRowUUID(UserDB *userDB);

// User Storage CBs
int32 UserDBLoadCB(uint16 tableID, int32 status);
int32 UserDBSaveCB(uint16 tableID, int32 status);

int32 UserDBStorage_LoadCB1(int32 status);
int32 UserDBStorage_LoadCB2(int32 status);
int32 UserDBStorage_LoadCB3(int32 status);
int32 UserDBStorage_LoadCB4(int32 status);
int32 UserDBStorage_LoadCB5(int32 status);
int32 UserDBStorage_LoadCB6(int32 status);
int32 UserDBStorage_LoadCB7(int32 status);
int32 UserDBStorage_LoadCB8(int32 status);

int32 UserDBStorage_SaveCB1(int32 status);
int32 UserDBStorage_SaveCB2(int32 status);
int32 UserDBStorage_SaveCB3(int32 status);
int32 UserDBStorage_SaveCB4(int32 status);
int32 UserDBStorage_SaveCB5(int32 status);
int32 UserDBStorage_SaveCB6(int32 status);
int32 UserDBStorage_SaveCB7(int32 status);
int32 UserDBStorage_SaveCB8(int32 status);
#endif

extern void (*preLoadSaveFileCB)();
extern void (*postLoadSaveFileCB)();
extern char userFileDir[0x100];

inline void SetUserFileCallbacks(const char *userDir, void (*preCB)(void), void (*postCB)(void))
{
    preLoadSaveFileCB  = preCB;
    postLoadSaveFileCB = postCB;
    strcpy(userFileDir, userDir);
}

bool32 LoadUserFile(const char *filename, void *buffer, uint32 bufSize);
bool32 SaveUserFile(const char *filename, void *buffer, uint32 bufSize);
bool32 DeleteUserFile(const char *filename);

#if !RETRO_REV02
bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32));
bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, int32 (*callback)(int32));
#endif

void InitUserDirectory();

} // namespace SKU

} // namespace RSDK

#endif