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
    virtual void FrameInit()
    {
        if (!saveStatus) {
            if (authStatus == STATUS_ERROR || storageStatus == STATUS_ERROR)
                saveStatus = STATUS_ERROR;
            else if (storageStatus == STATUS_OK)
                saveStatus = STATUS_OK;
        }
    }
    virtual void StageLoad() { saveStatus = STATUS_NONE; }
    virtual void OnUnknownEvent() {}
    virtual int32 TryAuth() { return 0; }
    virtual int32 TryInitStorage() { return 0; }
    virtual bool32 GetUsername(String *userName) { return false; }
    virtual bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status)) { return false; }
    virtual bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compress) { return false; }
    virtual bool32 TryDeleteUserFile(const char *filename, void (*callback)(int32 status)) { return false; }
    virtual void ClearPrerollErrors() {}

    int32 authStatus    = STATUS_NONE;
    int32 storageStatus = STATUS_NONE;
    int32 saveStatus    = STATUS_NONE;
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
    UserDBValue() { memset(data, 0, sizeof(data)); }
    ~UserDBValue() {}

    bool32 CheckMatch(int32 row, int32 column);
    void Set(int32 type, void *data);
    void Get(int32 type, void *data);

    UserDBRow *parent = NULL;
    uint8 size        = 0;
    uint8 data[0x10];
};

struct UserDBRow {
    UserDBRow()
    {
        memset(&createTime, 0, sizeof(createTime));
        memset(&changeTime, 0, sizeof(changeTime));
        memset(values, 0, sizeof(values));
    }
    ~UserDBRow() {}

    bool32 AddValue(int32 type, char *name, void *value);
    bool32 GetValue(int32 type, char *name, void *value);

    bool32 Compare(UserDBRow *other, int32 type, char *name, bool32 sortAscending);

    UserDB *parent = NULL;
    uint32 uuid    = 0;
    tm createTime;
    tm changeTime;
    UserDBValue values[8];
};

struct UserDB {
    UserDB()
    {
        MEM_ZERO(sortedRowIDs);
        MEM_ZERO(columnTypes);
        MEM_ZERO(columnNames);
        MEM_ZERO(columnUUIDs);
        MEM_ZERO(rows);
    }
    ~UserDB() { sortedRowList.Clear(true); }

    void Init(va_list list);
    bool32 Load(uint8 *buffer);
    void Save(int32 totalSize, uint8 *buffer);
    void Clear();

    int32 GetColumnID(const char *name);

    int32 AddRow();
    uint16 GetRowByID(uint32 uuid);
    bool32 RemoveRow(uint32 row);
    bool32 RemoveAllRows();

    void FilterValues(UserDBValue *value, int32 column);
    void AddSortFilter(const char *name, void *value);

    void SetupRowSortIDs();
    void RefreshSortList();
    void SortRows(int32 type, char *name, bool32 sortAscending);

    uint32 CreateRowUUID();
    void Refresh();
    size_t GetSize();

    const char *name  = "";
    uint32 uuid       = 0;
    uint8 loaded      = false;
    uint8 active      = false;
    uint8 valid       = false;
    UserDB *parent    = NULL;
    uint8 rowsChanged = false;
    List<int32> sortedRowList;
    int32 sortedRowIDs[RETRO_USERDB_ROW_MAX];
    int32 sortedRowCount = 0;
    int32 columnCount    = 0;
    int32 columnTypes[RETRO_USERDB_COL_MAX];
    char columnNames[RETRO_USERDB_COL_MAX][0x10];
    uint32 columnUUIDs[RETRO_USERDB_COL_MAX];
    uint16 rowCount = 0;
    UserDBRow rows[RETRO_USERDB_ROW_MAX];
};

struct UserDBStorage {
    UserDBStorage();

    uint16 InitUserDB(const char *name, va_list list);
    uint16 LoadUserDB(const char *filename, void (*callback)(int32 status));
    bool32 SaveUserDB(uint16 tableID, void (*callback)(int32 status));
    void ClearUserDB(uint16 tableID);
    void ClearAllUserDBs();

    static bool32 LoadCB(uint16 tableID, int32 status);
    static bool32 SaveCB(uint16 tableID, int32 status);

    UserDB userDB[RETRO_USERDB_MAX];
    uint8 unknown;
    int32 *writeBuffer[RETRO_USERDB_MAX];
    void *readBuffer[RETRO_USERDB_MAX];
    void (*loadCallback[RETRO_USERDB_MAX])(int32 status);
    void (*saveCallback[RETRO_USERDB_MAX])(int32 status);
    bool32 (*dbLoadCB[RETRO_USERDB_MAX])(uint16 table, int32 status);
    bool32 (*dbSaveCB[RETRO_USERDB_MAX])(uint16 table, int32 status);
    void (*callbacks[RETRO_USERDB_MAX])(int32);
    int32 unused[RETRO_USERDB_MAX];
};

extern UserDBStorage *userDBStorage;

// ====================
// API Cores
// ====================

// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyStorage.hpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamStorage.hpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSStorage.hpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXStorage.hpp"
#endif

// =======================
// USER STORAGE
// =======================

inline int32 TryAuth() { return userStorage->TryAuth(); }
inline int32 TryInitStorage() { return userStorage->TryInitStorage(); }
inline bool32 GetUsername(String *userName) { return userStorage->GetUsername(userName); }
inline bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status))
{
    return userStorage->TryLoadUserFile(filename, buffer, size, callback);
}
inline bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status), bool32 compressed)
{
    return userStorage->TrySaveUserFile(filename, buffer, size, callback, compressed);
}
inline bool32 TryDeleteUserFile(const char *filename, void (*callback)(int32 status)) { return userStorage->TryDeleteUserFile(filename, callback); }
inline void ClearPrerollErrors() { userStorage->ClearPrerollErrors(); }

inline int32 GetUserAuthStatus() { return userStorage->authStatus; }
inline int32 GetUserStorageStatus()
{
    if (userStorage->saveStatus == STATUS_ERROR)
        return STATUS_ERROR;
    else
        return userStorage->storageStatus;
}

inline int32 GetSaveStatus()
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
inline void ClearSaveStatus() { userStorage->saveStatus = STATUS_NONE; }
inline void SetSaveStatusContinue() { userStorage->saveStatus = STATUS_CONTINUE; }
inline void SetUserStorageNoSave(int32 state) { userStorage->noSaveActive = state; }
inline int32 GetUserStorageNoSave() { return userStorage->noSaveActive; }

// =======================
// USER DB API
// =======================

// UserDB management
inline uint16 InitUserDB(const char *name, ...)
{
    va_list list;
    va_start(list, name);
    uint16 id = userDBStorage->InitUserDB(name, list);
    va_end(list);
    return id;
}
inline uint16 LoadUserDB(const char *filename, void (*callback)(int32 status)) { return userDBStorage->LoadUserDB(filename, callback); }
inline bool32 SaveUserDB(uint16 tableID, void (*callback)(int32 status)) { return userDBStorage->SaveUserDB(tableID, callback); }
inline void ClearUserDB(uint16 tableID) { userDBStorage->ClearUserDB(tableID); }
inline void ClearAllUserDBs() { userDBStorage->ClearAllUserDBs(); }

// UserDB rows
inline int32 AddUserDBRow(uint16 tableID)
{
    if (tableID == (uint16)-1)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return -1;

    return userDB->AddRow();
}
inline bool32 RemoveDBRow(uint16 tableID, uint32 rowID)
{
    if (tableID == (uint16)-1 || rowID == (uint16)-1)
        return false;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return false;

    return userDB->RemoveRow(rowID);
}
inline bool32 RemoveAllDBRows(uint16 tableID)
{
    if (tableID == (uint16)-1)
        return false;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return false;

    return userDB->RemoveAllRows();
}
inline uint16 GetUserDBRowByID(uint16 tableID, uint32 uuid)
{
    if (tableID == (uint16)-1 || !uuid)
        return -1;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return -1;

    return userDB->GetRowByID(uuid);
}

inline uint32 GetDBRowUUID(uint16 tableID, int32 rowID)
{
    if (tableID == (uint16)-1 || rowID == (uint16)-1)
        return 0;

    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return 0;

    return userDB->rows[rowID].uuid;
}

// UserDB Row Sorting
uint16 SetupUserDBRowSorting(uint16 tableID);
int32 AddUserDBRowSortFilter(uint16 tableID, int32 type, const char *name, void *value);
int32 SortUserDBRows(uint16 tableID, int32 type, const char *name, bool32 sortAscending);
int32 GetSortedUserDBRowCount(uint16 tableID);
int32 GetSortedUserDBRowID(uint16 tableID, uint16 entryID);

// UserDB Values
bool32 GetUserDBValue(uint16 tableID, uint32 rowID, int32 type, char *name, void *value);
bool32 SetUserDBValue(uint16 tableID, uint32 rowID, int32 type, char *name, void *value);

// UserDB Misc
int32 GetUserDBRowsChanged(uint16 tableID);
void GetUserDBRowCreationTime(uint16 tableID, int32 entryID, char *buf, size_t size, char *format);

// =======================
// USER DB CALLBACKS
// =======================

void UserDBStorage_LoadCB1(int32 status);
void UserDBStorage_LoadCB2(int32 status);
void UserDBStorage_LoadCB3(int32 status);
void UserDBStorage_LoadCB4(int32 status);
void UserDBStorage_LoadCB5(int32 status);
void UserDBStorage_LoadCB6(int32 status);
void UserDBStorage_LoadCB7(int32 status);
void UserDBStorage_LoadCB8(int32 status);

void UserDBStorage_SaveCB1(int32 status);
void UserDBStorage_SaveCB2(int32 status);
void UserDBStorage_SaveCB3(int32 status);
void UserDBStorage_SaveCB4(int32 status);
void UserDBStorage_SaveCB5(int32 status);
void UserDBStorage_SaveCB6(int32 status);
void UserDBStorage_SaveCB7(int32 status);
void UserDBStorage_SaveCB8(int32 status);
#endif

extern void (*preLoadSaveFileCB)();
extern void (*postLoadSaveFileCB)();
extern char userFileDir[0x100];

inline void SetUserFileCallbacks(const char *userDir, void (*preCB)(), void (*postCB)())
{
    preLoadSaveFileCB  = preCB;
    postLoadSaveFileCB = postCB;
    strcpy(userFileDir, userDir);
}

bool32 LoadUserFile(const char *filename, void *buffer, uint32 bufSize);
bool32 SaveUserFile(const char *filename, void *buffer, uint32 bufSize);
bool32 DeleteUserFile(const char *filename);

#if !RETRO_REV02
bool32 TryLoadUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status));
bool32 TrySaveUserFile(const char *filename, void *buffer, uint32 size, void (*callback)(int32 status));
#endif

void InitUserDirectory();

} // namespace SKU

} // namespace RSDK

#endif