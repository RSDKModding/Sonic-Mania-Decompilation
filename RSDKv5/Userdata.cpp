#include "RetroEngine.hpp"

bool32 settingsChanged = false;

#if !RETRO_USE_PLUS
FunctionListEntry functionList[FUNCLIST_COUNT];
int functionListCount;
#endif

#if RETRO_USE_PLUS
DummyCore *dummmyCore = NULL;
DummyCore *userCore   = NULL;

DummyAchievements *achievements = NULL;
DummyLeaderboards *leaderboards = NULL;
DummyRichPresence *richPresence = NULL;
DummyStats *stats               = NULL;
DummyUserStorage *userStorage   = NULL;

UserDBStorage userDBStorage[RETRO_USERDB_MAX];
#endif

inline void nullUserFunc() {}

void initUserData()
{
#if RETRO_USE_PLUS
    if (!dummmyCore)
        dummmyCore = (DummyCore *)malloc(sizeof(DummyCore));
    MEM_ZEROP(dummmyCore);
#endif

    if (true) { // no steam or etc, so default to dummy funcs
#if RETRO_USE_PLUS
        userCore = dummmyCore;

        if (!achievements)
            achievements = (DummyAchievements *)malloc(sizeof(DummyAchievements));
        MEM_ZEROP(achievements);

        if (!leaderboards)
            leaderboards = (DummyLeaderboards *)malloc(sizeof(DummyLeaderboards));
        MEM_ZEROP(leaderboards);

        if (!richPresence)
            richPresence = (DummyRichPresence *)malloc(sizeof(DummyRichPresence));
        MEM_ZEROP(richPresence);

        if (!stats)
            stats = (DummyStats *)malloc(sizeof(DummyStats));
        MEM_ZEROP(stats);

        if (!userStorage)
            userStorage = (DummyUserStorage *)malloc(sizeof(DummyUserStorage));
        MEM_ZEROP(userStorage);
        // userStorage->active = true;

        userCore->unknown1             = nullUserFunc;
        userCore->unknown2             = nullUserFunc;
        userCore->unknown3             = nullUserFunc;
        userCore->unknown4             = nullUserFunc;
        userCore->SetupDebugValues     = setupUserDebugValues;
        userCore->UserInitUnknown1     = userInitUnknown1;
        userCore->UserInitUnknown2     = userInitUnknown2;
        userCore->GetUserLanguage      = GetUserLanguage;
        userCore->GetUserRegion        = GetUserRegion;
        userCore->GetUserPlatform      = GetUserPlatform;
        userCore->GetConfirmButtonFlip = GetConfirmButtonFlip;
        userCore->LaunchManual         = LaunchManual;
        userCore->ExitGame             = ExitGame;
        userCore->unknown14            = nullUserFunc;
        userCore->unknown15            = UserCoreUnknown15;
        userCore->CheckDLC             = checkDLC;
        userCore->ShowExtensionOverlay = ShowExtensionOverlay;

        userCore->values[0]   = (int *)&engine.hasPlus;
        userCore->values[1]   = &curSKU.platform;
        userCore->values[2]   = &curSKU.region;
        userCore->values[3]   = &curSKU.language;
        userCore->values[4]   = (int *)&engine.confirmFlip;
        userCore->debugValCnt = 5;

        achievements->InitUnknown1      = nullUserFunc;
        achievements->SetDebugValues    = nullUserFunc;
        achievements->InitUnknown2      = nullUserFunc;
        achievements->UnlockAchievement = TryUnlockAchievement;

        leaderboards->SetDebugValues   = nullUserFunc;
        leaderboards->InitUnknown1     = nullUserFunc;
        leaderboards->InitUnknown2     = nullUserFunc;
        leaderboards->unknown4         = (int (*)())nullUserFunc;
        leaderboards->FetchLeaderboard = (void (*)(int, int))nullUserFunc;
        leaderboards->unknown5         = nullUserFunc;
        leaderboards->TrackScore       = (void (*)(int, int, int))nullUserFunc;
        leaderboards->unknown7         = (int (*)())nullUserFunc;

        richPresence->SetDebugValues = nullUserFunc;
        richPresence->InitUnknown1   = nullUserFunc;
        richPresence->InitUnknown2   = nullUserFunc;
        richPresence->SetPresence    = SetPresence;

        stats->SetDebugValues = nullUserFunc;
        stats->InitUnknown1   = nullUserFunc;
        stats->InitUnknown2   = nullUserFunc;
        stats->TryTrackStat   = nullUserFunc; // TODO

        userStorage->InitUnknown1   = (int (*)())nullUserFunc;
        userStorage->SetDebugValues = (int (*)())nullUserFunc;
        userStorage->InitUnknown2   = (int (*)())nullUserFunc;
        userStorage->TryAuth        = TryAuth;
        userStorage->TryInitStorage = TryInitStorage;
        userStorage->GetUsername    = GetUserName;
        userStorage->LoadUserFile   = TryLoadUserFile;
        userStorage->SaveUserFile   = TrySaveUserFile;
        userStorage->DeleteUserFile = TryDeleteUserFile;
        userStorage->unknown8       = UserStorageUnknown8;
#endif

#if !RETRO_USE_PLUS
        SetFuncPtr("GetConfirmButtonFlip", GetConfirmButtonFlip);
        SetFuncPtr("LaunchManual", LaunchManual);
        SetFuncPtr("ExitGame", ExitGame);
        // SetFuncPtr("ClearAchievements", ClearAchievements);
        SetFuncPtr("UnlockAchievement", TryUnlockAchievement);
        SetFuncPtr("FetchLeaderboard", FetchLeaderboard);
        // SetFuncPtr("LeaderboardStatus", GetLeaderboardStatus);
        // SetFuncPtr("LeaderboardEntryCount", LeaderboardEntryCount);
        // SetFuncPtr("LeaderboardReadEntry", LeaderboardReadEntry);
        SetFuncPtr("TrackActClear", TrackActClear);
        SetFuncPtr("TrackTAClear", TrackTAClear);
        SetFuncPtr("TrackEnemyDefeat", TrackEnemyDefeat);
        SetFuncPtr("ClearPrerollErrors", ClearPrerollErrors);
        SetFuncPtr("TryAuth", TryAuth);
        SetFuncPtr("GetUserAuthStatus", GetUserAuthStatus);
        SetFuncPtr("TryInitStorage", TryInitStorage);
        SetFuncPtr("GetStorageStatus", GetUserStorageStatus);
        SetFuncPtr("GetUsername", GetUserName);
        SetFuncPtr("LoadUserFile", TryLoadUserFile);
        SetFuncPtr("SaveUserFile", TrySaveUserFile);
        SetFuncPtr("SaveSettingsINI", writeSettings);
        SetFuncPtr("GetUserLanguage", GetUserLanguage);
        // SetFuncPtr("ControllerIDForInputID", ControllerIDForInputID);
        // SetFuncPtr("MostRecentActiveControllerID", MostRecentActiveControllerID);
        // SetFuncPtr("AssignControllerID", AssignControllerID);
        // SetFuncPtr("ResetControllerAssignments", ResetControllerAssignments);
        // SetFuncPtr("InputIDIsDisconnected", InputIDIsDisconnected);
        // SetFuncPtr("GetControllerType", GetControllerType);
        SetFuncPtr("ShowSteamControllerOverlay", ShowExtensionOverlay);
        SetFuncPtr("SetRichPresence", SetPresence);
#endif
    }
}
void releaseUserData()
{
#if RETRO_USE_PLUS
    if (dummmyCore)
        free(dummmyCore);
    dummmyCore = NULL;

    if (achievements)
        free(achievements);
    achievements = NULL;

    if (leaderboards)
        free(leaderboards);
    leaderboards = NULL;

    if (richPresence)
        free(richPresence);
    richPresence = NULL;

    if (stats)
        free(stats);
    stats = NULL;

    if (userStorage)
        free(userStorage);
    userStorage = NULL;
#endif
}

int GetUserLanguage()
{
#if RETRO_USE_PLUS
    return curSKU.language;
#else
    return engineInfo.language;
#endif
}
int GetUserRegion()
{
#if RETRO_USE_PLUS
    return curSKU.region;
#else
    return engineInfo.region;
#endif
}
int GetUserPlatform()
{
#if RETRO_USE_PLUS
    return curSKU.platform; 
#else 
    return engineInfo.platformID;
#endif
}
int GetConfirmButtonFlip()
{
    printLog(SEVERITY_NONE, "DUMMY GetConfirmButtonFlip() -> %d", engine.confirmFlip);
    return true;
}
void LaunchManual() { printLog(SEVERITY_NONE, "DUMMY LaunchManual()"); }
void ExitGame() { engine.running = false; }

int ShowExtensionOverlay(byte overlay)
{
    printLog(SEVERITY_WARN, "Show Extension Overlay: %d", overlay);
    return 1;
}

void TryUnlockAchievement(const char *name) { printLog(SEVERITY_NONE, "DUMMY TryUnlockAchievement(%s)", name); }

void FetchLeaderboard(int a2, int a3) { printLog(SEVERITY_NONE, "DUMMY FetchLeaderboard(%d, %d)", a2, a3); }
void TrackScore(int a2, int a3, int a4) { printLog(SEVERITY_NONE, "DUMMY TrackScore(%d, %d, %d)", a2, a3, a4); }

void SetPresence(byte a2, TextInfo *info)
{
    char buffer[0xFF];
    char buffer2[0xFF];
    GetCString(buffer, info);
#if RETRO_USE_PLUS
    sprintf(buffer2, "DUMMY SetPresence(%d, %s) -> %s", a2, buffer, (richPresence->status != a2 ? "Successful Set" : "Redundant Set"));
#else
    sprintf(buffer2, "DUMMY SetPresence(%d, %s)", a2, buffer);
#endif
    printLog(SEVERITY_NONE, buffer2);
}

#if !RETRO_USE_PLUS
void TrackActClear() { printLog(SEVERITY_NONE, "DUMMY TrackActClear()"); }
void TrackTAClear(byte a1, byte a2, byte a3, int a4) { printLog(SEVERITY_NONE, "DUMMY TrackTAClear(%d,%d,%d,%d)", a1, a2, a3, a4); }
void TrackEnemyDefeat() { printLog(SEVERITY_NONE, "DUMMY TrackEnemyDefeat()"); }
void ClearPrerollErrors() { printLog(SEVERITY_NONE, "DUMMY ClearPrerollErrors()"); }
#endif

#if RETRO_USE_PLUS
const char *userDebugValNames[8] = { "Ext <PLUS>", "SYSTEM_PLATFORM", "SYSTEM_REGION", "SYSTEM_LANGUAGE", "SYS_CNFRM_FLIP" };
void setupUserDebugValues()
{
    achievements->SetDebugValues();
    leaderboards->SetDebugValues();
    richPresence->SetDebugValues();
    stats->SetDebugValues();
    userStorage->SetDebugValues();

    for (int i = 0; i < userCore->debugValCnt && debugValCnt < DEBUGVAL_MAX; ++i) {
        DebugValueInfo *val = &debugValues[debugValCnt++];
        StrCopy(val->name, userDebugValNames[i]);
        val->isSigned   = 0;
        val->valByteCnt = 4;
        val->value      = userCore->values[i];
        val->min        = 0;
        val->max        = 1;
    }
}
void userInitUnknown1()
{
    achievements->InitUnknown1();
    leaderboards->InitUnknown1();
    richPresence->InitUnknown1();
    stats->InitUnknown1();
    userStorage->InitUnknown1();
}
void userInitUnknown2()
{
    achievements->InitUnknown2();
    leaderboards->InitUnknown2();
    richPresence->InitUnknown2();
    stats->InitUnknown2();
    userStorage->InitUnknown2();
}
#endif

#if !RETRO_USE_PLUS
void SetFuncPtr(const char *name, void *ptr)
{
    if (functionListCount < FUNCLIST_COUNT) {
        uint hash[4];
        GEN_HASH(name, hash);
        for (int f = 0; f < functionListCount; ++f) {
            if (HASH_MATCH(hash, functionList[f].hash))
                return; // already exists, ignore this call
        }

        HASH_COPY(functionList[functionListCount].hash, hash);
        functionList[functionListCount].ptr = ptr;
        functionListCount++;
    }
}

void *GetFuncPtr(const char *name)
{
    if (!name)
        return NULL;

    uint hash[4];
    GEN_HASH(name, hash);
    for (int f = 0; f < functionListCount; ++f) {
        if (HASH_MATCH(hash, functionList[f].hash))
            return functionList[f].ptr;
    }

    if (engine.printConsole)
        printLog(SEVERITY_WARN, "API Function not found: %s", name);
    return NULL;
}
#endif

bool32 TryLoadUserFile(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int))
{
#if RETRO_USE_PLUS
    if (!userStorage->noSaveActive) {
#endif
        LoadUserFile(filename, buffer, bufSize);

        if (callback)
            callback(100);
#if RETRO_USE_PLUS
    }
    else {
        char buffer[0x100];
        sprintf(buffer, "TryLoadUserFile(%s, %p, %u, %p) failing due to noSave", filename, buffer, bufSize, callback);

        if (callback)
            callback(500);
    }
#endif

    return false;
}
bool32 TrySaveUserFile(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int), bool32 compress)
{
#if RETRO_USE_PLUS
    if (!userStorage->noSaveActive) {
#endif
        if (compress) {
            // compress lo
        }
        SaveUserFile(filename, buffer, bufSize);

        if (callback)
            callback(100);
#if RETRO_USE_PLUS
    }
    else {
        char buffer[0x100];
        sprintf(buffer, "TrySaveUserFile(%s, %p, %u, %p, %s) failing due to noSave", filename, buffer, bufSize, callback,
                compress ? "true" : "false");

        if (callback)
            callback(500);
    }
#endif

    return false;
}
bool32 TryDeleteUserFile(const char *filename, int (*callback)(int))
{
#if RETRO_USE_PLUS
    if (!userStorage->noSaveActive) {
#endif
        DeleteUserFile(filename);

        if (callback)
            callback(100);
#if RETRO_USE_PLUS
    }
    else {
        char buffer[0x100];
        sprintf(buffer, "TryDeleteUserFile(%s, %p) failing due to noSave", filename, callback);

        if (callback)
            callback(500);
    }
#endif

    return false;
}

void (*userFileCallback)();
void (*userFileCallback2)();
char userFiles[1024];

bool32 LoadUserFile(const char *filename, void *buffer, unsigned int bufSize)
{
    if (userFileCallback)
        userFileCallback();
    int len = (int)strlen(userFiles);
    sprintf(userFiles, "%s", filename);
    if (len >= 0x400) {
        // oh shit
    }
    printLog(SEVERITY_NONE, "Attempting to load user file: %s", userFiles);

    FileIO *file = fOpen(userFiles, "rb");
    if (file) {
        fSeek(file, 0, SEEK_END);
        int fSize = (int)fTell(file);
        fSeek(file, 0, SEEK_SET);
        int size = bufSize;
        if (bufSize > fSize)
            size = fSize;
        fRead(buffer, 1, size, file);
        fClose(file);
        if (userFileCallback2)
            userFileCallback2();
        return true;
    }
    else {
        if (userFileCallback2)
            userFileCallback2();
        printLog(SEVERITY_NONE, "Nope!");
    }
    return false;
}
bool32 SaveUserFile(const char *filename, void *buffer, unsigned int bufSize)
{
    if (userFileCallback)
        userFileCallback();
    int len = (int)strlen(userFiles);
    sprintf(userFiles, "%s", filename);
    if (len >= 0x400) {
        // oh shit
    }
    printLog(SEVERITY_NONE, "Attempting to save user file: %s", userFiles);

    FileIO *file = fOpen(userFiles, "rb");
    if (file) {
        fWrite(buffer, 1, bufSize, file);
        fClose(file);

        // encryption?

        if (userFileCallback2)
            userFileCallback2();
        return true;
    }
    else {
        if (userFileCallback2)
            userFileCallback2();
        printLog(SEVERITY_NONE, "Nope!");
    }
    return false;
}
bool32 DeleteUserFile(const char *filename)
{
    if (userFileCallback)
        userFileCallback();
    int len = (int)strlen(userFiles);
    sprintf(userFiles, "%s", filename);
    if (len >= 0x400) {
        // oh shit
    }
    printLog(SEVERITY_NONE, "Attempting to delete user file: %s", userFiles);
    int status = remove(userFiles);

    if (userFileCallback2)
        userFileCallback2();
    return status == 0;
}

#if RETRO_USE_PLUS
int LoadDBFromBuffer(UserDB *userDB, int totalSize, byte *writeBuffer)
{
    if (*(int *)writeBuffer != 0x80074B1E)
        return 0;
    writeBuffer += sizeof(int);
    writeBuffer += sizeof(int); // used size

    userDB->entryCount = *(ushort *)writeBuffer;
    writeBuffer += sizeof(ushort);

    userDB->columnCount = *writeBuffer;
    writeBuffer++;

    for (int c = 0; c < userDB->columnCount; ++c) {
        userDB->columnSizes[c] = *writeBuffer;
        writeBuffer++;

        strcpy_s(userDB->columnNames[c], 0x10, (char *)writeBuffer);
        writeBuffer += 0x10;

        GenerateCRC(&userDB->columnUUIDs[c], userDB->columnNames[c]);
    }

    for (int r = 0; r < RETRO_USERDB_ENTRY_MAX; ++r) {
        userDB->rows[r].uuid = *(uint *)writeBuffer;
        writeBuffer += sizeof(uint);

        memcpy(&userDB->rows[r].createTime, writeBuffer, sizeof(tm));
        writeBuffer += sizeof(tm);
        memcpy(&userDB->rows[r].changeTime, writeBuffer, sizeof(tm));
        writeBuffer += sizeof(tm);

        for (int c = 0; c < userDB->columnCount; ++c) {
            userDB->rows[r].values[c].size = *writeBuffer;
            writeBuffer++;

            memcpy(&userDB->rows[r].values[c].value, writeBuffer, userDB->rows[r].values[c].size);
            writeBuffer += userDB->rows[r].values[c].size;
        }
    }

    userDB->active = true;
    UpdateUserDBParents(userDB);
    return 1;
}

size_t GetUserDBWriteSize(UserDB *userDB)
{
    int colSize = 1;
    if (userDB->columnCount)
        colSize = (0x10 * userDB->columnCount) + userDB->columnCount + 1;
    size_t size = colSize + 10;

    for (int r = 0; r < RETRO_USERDB_ENTRY_MAX; ++r) {
        userDB->rows[r].parent = userDB;
        int rowSize            = (sizeof(tm) * 2) + sizeof(uint);
        for (int c = 0; c < userDB->columnCount; ++c) {
            rowSize += sizeof(byte) + userDB->rows[r].values[c].size;
        }
        size += rowSize;
    }

    return size;
}

void SaveDBToBuffer(UserDB *userDB, int totalSize, byte *writeBuffer)
{
    int size = 0;
    if (totalSize >= sizeof(int)) {
        size                = sizeof(int);
        *(int *)writeBuffer = 0x80074B1E; // signature
        writeBuffer += sizeof(int);
    }
    if (size + sizeof(int) <= totalSize) {
        size += sizeof(int);
        *(int *)writeBuffer = GetUserDBWriteSize(userDB); // used size
        writeBuffer += sizeof(int);
    }
    if (size + sizeof(ushort) <= totalSize) {
        size += sizeof(ushort);
        *(ushort *)writeBuffer = userDB->entryCount;
        writeBuffer += sizeof(ushort);
    }
    if (size + sizeof(byte) <= totalSize) {
        ++size;
        *writeBuffer++ = userDB->columnCount;
    }

    for (int c = 0; c < userDB->columnCount; ++c) {
        if (size + sizeof(byte) <= totalSize) {
            ++size;
            *writeBuffer++ = (byte)userDB->columnSizes[c];
        }
        if (size + 0x10 <= totalSize) {
            memset(writeBuffer, 0, 0x10 * sizeof(byte));
            strcpy_s((char *)writeBuffer, 0x10, userDB->columnNames[c]);
            size += 0x10;
            writeBuffer += 0x10;
        }
    }

    for (int r = 0; r < RETRO_USERDB_ENTRY_MAX; ++r) {
        if (size + sizeof(uint) <= totalSize) {
            size += sizeof(uint);
            *(uint *)writeBuffer = userDB->rows[r].uuid;
            writeBuffer += sizeof(uint);
        }
        if (size + sizeof(tm) <= totalSize) {
            memcpy(writeBuffer, &userDB->rows[r].createTime, sizeof(tm));
            size += sizeof(tm);
            writeBuffer += sizeof(tm);
        }
        if (size + sizeof(tm) <= totalSize) {
            memcpy(writeBuffer, &userDB->rows[r].changeTime, sizeof(tm));
            size += sizeof(tm);
            writeBuffer += sizeof(tm);
        }

        for (int c = 0; c < userDB->columnCount; ++c) {
            if (size + sizeof(byte) <= totalSize) {
                ++size;
                *writeBuffer++ = (byte)userDB->rows[r].values[c].size;
            }
            if (userDB->rows[r].values[c].size + size <= totalSize) {
                memcpy(writeBuffer, (byte *)&userDB->rows[r].values[c].value, userDB->rows[r].values[c].size);
                size += userDB->rows[r].values[c].size;
                writeBuffer += userDB->rows[r].values[c].size;
            }
        }
    }

    if (size < totalSize)
        memset(writeBuffer, 0, totalSize - size);
}

int UserDBLoadCB(ushort tableID, int status)
{
    if (status == 200) {
        int result = LoadDBFromBuffer(&userDBStorage->userDB[tableID], (int)userDBStorage, (byte*)userDBStorage->readBuffer[tableID]);
        if (result) {
            // sub_5EC5F0(&userDBStorage->userDB[v6].parent);
        }
    }
    else {
        ClearUserDB(tableID);
    }
    RemoveStorageEntry((void **)&userDBStorage->readBuffer[tableID]);

    if (userDBStorage->callbacks[tableID]) {
        int result                        = userDBStorage->callbacks[tableID](status);
        userDBStorage->callbacks[tableID] = NULL;
        return result;
    }
    return 0;
}

int UserDBSaveCB(ushort tableID, int status)
{
    RemoveStorageEntry((void**)&userDBStorage->writeBuffer[tableID]);
    if (status != 200)
        userDBStorage->userDB[tableID].valid = false;

    if (userDBStorage->callbacks[tableID]) {
        int result = userDBStorage->callbacks[tableID](status);
        userDBStorage->callbacks[tableID]                  = NULL;
        return result;
    }
    return 0;
}

ushort LoadUserDB(const char *filename, int (*callback)(int))
{
    int tableID = -1;
    uint uuid   = 0;
    GenerateCRC(&uuid, (char *)filename);
    for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
        if (uuid == userDBStorage->userDB[i].uuid)
            return i;
    }

    for (int i = 0; i < RETRO_USERDB_MAX; ++i) {
        if (!userDBStorage->userDB[i].loaded) {
            tableID = i;
            break;
        }
    }

    if (tableID == -1) {
        if (callback)
            callback(500);
        return -1;
    }

    UserDB *userDB = &userDBStorage->userDB[tableID];
    userDB->loaded = true;
    userDB->name   = filename;
    userDB->uuid   = uuid;
    AllocateStorage(sizeof(UserDB), (void **)&userDBStorage->readBuffer[tableID], DATASET_TMP, true);
    userDBStorage->loadCallback[tableID] = UserDBLoadCB;
    userDBStorage->callbacks[tableID] = callback;
    TryLoadUserFile(filename, userDBStorage->readBuffer[tableID], sizeof(UserDB), userDBStorage->saveCallback[tableID]);
    return tableID;
}
bool32 SaveUserDB(ushort tableID, int (*callback)(int))
{
    UserDB *userDB = &userDBStorage->userDB[tableID];

    bool32 success = false;
    if (userDB->active) {
        int totalSize = GetUserDBWriteSize(userDB);
        AllocateStorage(totalSize, (void **)&userDBStorage->writeBuffer[tableID], DATASET_TMP, true);
        SaveDBToBuffer(userDB, totalSize, (byte*)userDBStorage->writeBuffer[tableID]);
        userDBStorage->callbacks[tableID] = callback;
        userDBStorage->loadCallback[tableID] = UserDBSaveCB;
        success = TrySaveUserFile(userDB->name, userDBStorage->writeBuffer[tableID], totalSize, userDBStorage->saveCallback[tableID], true);
    }
    else {
        if (callback)
            success = callback(500);
    }
    return success;
}
#endif

int GetSettingsValue(int id)
{
    switch (id) {
        case 0: return !engine.isFullScreen; // windowed
        case 1: return !engine.borderless;   // bordered
        case 2: return engine.exclusiveFS;
        case 3: return engine.vsync;
        case 4: return engine.tripleBuffer;
        case 5: return engine.windowWidth;
        case 6: return engine.windowHeight;
        case 7: return engine.fsWidth;
        case 8: return engine.fsHeight;
        case 9: return engine.refreshRate;
        case 10: return engine.shaderSupport;
        case 11: return engine.shaderID;
        case 12: return engine.screenCount;
        case 13: return engine.dimTimer;
        case 14: return engine.streamsEnabled;
        case 15: return (int)(engine.streamVolume * 1024.0); 
        case 16: return (int)(engine.soundFXVolume * 1024.0); 
        case 17:
#if RETRO_USE_PLUS
            return curSKU.language; 
#else
            return engineInfo.language;
#endif
        case 20: return settingsChanged;
        default: break;
    }
    return 0;
}

void SetSettingsValue(int id, int val)
{
    bool32 bVal = val;
    switch (id) {
        case 0:
            if ((!engine.isFullScreen) != bVal) {
                settingsChanged     = true;
                engine.isFullScreen = !bVal;
            }
            break;
        case 1:
            if ((!engine.borderless) != bVal) {
                settingsChanged   = true;
                engine.borderless = !bVal;
            }
            break;
        case 2:
            if (engine.exclusiveFS != bVal) {
                settingsChanged    = 1;
                engine.exclusiveFS = bVal;
            }
            break;
        case 3:
            if (engine.vsync != bVal) {
                settingsChanged = true;
                engine.vsync    = bVal;
            }
            break;
        case 4:
            if (engine.tripleBuffer != bVal) {
                settingsChanged     = true;
                engine.tripleBuffer = bVal;
            }
            break;
        case 5:
            if (engine.windowWidth != val) {
                engine.windowWidth = val;
                settingsChanged    = true;
            }
            break;
        case 6:
            if (engine.windowHeight != val) {
                engine.windowHeight = val;
                settingsChanged     = true;
            }
            break;
        case 7: engine.fsWidth = val; break;
        case 8: engine.fsHeight = val; break;
        case 9: engine.refreshRate = val; break;
        case 10: engine.shaderSupport = val; break;
        case 11:
            if (engine.shaderID != val) {
                engine.shaderID = val;
                settingsChanged     = true;
            }
            break;
        case 12: engine.screenCount = val; break;
        case 13: engine.dimTimer = val; break;
        case 14:
            if (engine.streamsEnabled != bVal)
                settingsChanged = true;
            engine.streamsEnabled = bVal;
            break;
        case 15:
            if (engine.streamVolume != ((float)val * 0.0009765625)) {
                engine.streamVolume = val * 0.0009765625;
                settingsChanged     = true;
            }
            break;
        case 16:
            if (engine.soundFXVolume != ((float)val * 0.0009765625)) {
                engine.soundFXVolume = val * 0.0009765625;
                settingsChanged      = true;
            }
            break;
        case 17:
#if RETRO_USE_PLUS
            curSKU.language = val; 
#else
            engineInfo.language = val;
#endif
            break;
        case 18:
            // windowSettings = WindowSettings;
            // gameSettings   = GameSettings;
            // shaderSettings = ShaderSettings;
            // screenSettings = ScreenSettings;
            // inputSettings  = InputSettings;
            break;
        case 19:
            settingsChanged = true;
            // WindowSettings = windowSettings;
            // GameSettings   = gameSettings;
            // ShaderSettings = shaderSettings;
            // ScreenSettings = screenSettings;
            // InputSettings  = inputSettings;
            break;
        case 20: settingsChanged = val; break;
        case 21: writeSettings(val); break;
        default: return;
    }
}

void readSettings()
{

    dictionary *ini = iniparser_load("Settings.ini");

    const char *result = "";

#if RETRO_USE_PLUS
    curSKU.language = (int)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);
#else
    engineInfo.language = (int)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);
#endif

    result = iniparser_getstring(ini, "Game:dataFile", "Data.rsdk");
    if (CheckDataFile(result))
        engine.devMenu = iniparser_getboolean(ini, "Game:devMenu", false);
    else
        engine.devMenu = true;

    engine.startFullScreen = !iniparser_getboolean(ini, "Video:windowed", true);
    engine.borderless      = !iniparser_getboolean(ini, "Video:border", true);
    engine.exclusiveFS     = iniparser_getboolean(ini, "Video:exclusiveFS", false);
    engine.vsync           = iniparser_getboolean(ini, "Video:vsync", false);
    engine.tripleBuffer    = iniparser_getboolean(ini, "Video:tripleBuffering", false);

    result       = iniparser_getstring(ini, "Video:pixWidth", "424");
    SCREEN_XSIZE = (int)strtol(result, NULL, 0);

    engine.windowWidth   = (int)strtol(iniparser_getstring(ini, "Video:winWidth", "424"), NULL, 0);
    engine.windowHeight  = (int)strtol(iniparser_getstring(ini, "Video:winHeight", "240"), NULL, 0);
    engine.fsWidth       = (int)strtol(iniparser_getstring(ini, "Video:fsWidth", "0"), NULL, 0);
    engine.fsHeight      = (int)strtol(iniparser_getstring(ini, "Video:fsHeight", "0"), NULL, 0);
    engine.refreshRate   = (int)strtol(iniparser_getstring(ini, "Video:refreshRate", "60"), NULL, 0);
    engine.shaderSupport = iniparser_getboolean(ini, "Video:shaderSupport", true);
    engine.shaderID      = (int)strtol(iniparser_getstring(ini, "Video:screenShader", "0"), NULL, 0);

    engine.streamsEnabled = iniparser_getboolean(ini, "Audio:streamsEnabled", true);
    engine.streamVolume   = iniparser_getdouble(ini, "Audio:streamVolume", 1.0);
    engine.soundFXVolume  = iniparser_getdouble(ini, "Audio:sfxVolume", 1.0);

#if RETRO_USING_SDL2
    int defKeyMaps[PLAYER_COUNT][12] = {
        { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB },
        { SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6, SDL_SCANCODE_J, SDL_SCANCODE_J, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_LEFTBRACKET, SDL_SCANCODE_RIGHTBRACKET },
        { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN },
        { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
          SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN }
    };
    for (int i = 0; i < PLAYER_COUNT; ++i) {
        inputDevice.LSTICK_DEADZONE[i]   = 20000;
        inputDevice.RSTICK_DEADZONE[i]   = 20000;
        inputDevice.LTRIGGER_DEADZONE[i] = 20000;
        inputDevice.RTRIGGER_DEADZONE[i] = 20000;
    }
#else
    int defKeyMaps[PLAYER_COUNT][12] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
#endif

    for (int i = 0; i < PLAYER_COUNT; ++i) {
        char buffer[0x30];

        sprintf(buffer, "Keyboard Map %d:up", i);
        controller[i].keyUp.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][0]);
        sprintf(buffer, "Keyboard Map %d:dpwn", i);
        controller[i].keyDown.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][1]);
        sprintf(buffer, "Keyboard Map %d:left", i);
        controller[i].keyLeft.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][2]);
        sprintf(buffer, "Keyboard Map %d:right", i);
        controller[i].keyRight.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][3]);
        sprintf(buffer, "Keyboard Map %d:buttonA", i);
        controller[i].keyA.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][4]);
        sprintf(buffer, "Keyboard Map %d:buttonB", i);
        controller[i].keyB.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][5]);
        sprintf(buffer, "Keyboard Map %d:buttonC", i);
        controller[i].keyC.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][6]);
        sprintf(buffer, "Keyboard Map %d:buttonX", i);
        controller[i].keyX.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][7]);
        sprintf(buffer, "Keyboard Map %d:buttonY", i);
        controller[i].keyY.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][8]);
        sprintf(buffer, "Keyboard Map %d:buttonZ", i);
        controller[i].keyZ.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][9]);
        sprintf(buffer, "Keyboard Map %d:start", i);
        controller[i].keyStart.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][10]);
        sprintf(buffer, "Keyboard Map %d:select", i);
        controller[i].keySelect.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][11]);
    }
    iniparser_freedict(ini);
}
void writeSettings(bool32 writeToFile)
{
    if (settingsChanged || writeToFile) {
    }
}
