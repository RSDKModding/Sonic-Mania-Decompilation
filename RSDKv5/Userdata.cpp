#include "RetroEngine.hpp"

#if !RETRO_REV02
FunctionListEntry functionList[FUNCLIST_COUNT];
int functionListCount;
#endif

#if RETRO_REV02
DummyCore *dummmyCore = NULL;
DummyCore *userCore   = NULL;

DummyAchievements *achievements = NULL;
DummyLeaderboards *leaderboards = NULL;
DummyRichPresence *richPresence = NULL;
DummyStats *stats               = NULL;
DummyUserStorage *userStorage   = NULL;

UserDBStorage userDBStorage[RETRO_USERDB_MAX];
#endif

GamePadMappings *gamePadMappings = NULL;
int gamePadCount                 = 0;

bool32 settingsChanged = false;
SettingsStorage settingsStorage;

inline void nullUserFunc() {}

void initUserData()
{
#if RETRO_REV02
    if (!dummmyCore)
        dummmyCore = (DummyCore *)malloc(sizeof(DummyCore));
    MEM_ZEROP(dummmyCore);
#endif

    if (true) { // no steam or etc, so default to dummy funcs
#if RETRO_REV02
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
        userCore->controllerUnknown    = controllerUnknown;
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

#if !RETRO_REV02
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
#if RETRO_REV02
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
#if RETRO_REV02
    return curSKU.language;
#else
    return gameVerInfo.language;
#endif
}
int GetUserRegion()
{
#if RETRO_REV02
    return curSKU.region;
#else
    return gameVerInfo.region;
#endif
}
int GetUserPlatform()
{
#if RETRO_REV02
    return curSKU.platform; 
#else 
    return gameVerInfo.platform;
#endif
}
int GetConfirmButtonFlip()
{
    printLog(SEVERITY_NONE, "DUMMY GetConfirmButtonFlip() -> %d", engine.confirmFlip);
    return true;
}
void LaunchManual() { printLog(SEVERITY_NONE, "DUMMY LaunchManual()"); }
void ExitGame() { engine.running = false; }
int controllerUnknown() { return 0; }

int ShowExtensionOverlay(byte overlay)
{
    printLog(SEVERITY_WARN, "Show Extension Overlay: %d", overlay);
    return 1;
}

void TryUnlockAchievement(const char *name) { printLog(SEVERITY_NONE, "DUMMY TryUnlockAchievement(%s)", name); }

void FetchLeaderboard(int a2, int a3) { printLog(SEVERITY_NONE, "DUMMY FetchLeaderboard(%d, %d)", a2, a3); }
void TrackScore(int a2, int a3, int a4) { printLog(SEVERITY_NONE, "DUMMY TrackScore(%d, %d, %d)", a2, a3, a4); }

void SetPresence(byte id, TextInfo *info)
{
    char buffer[0xFF];
    char buffer2[0xFF];
    GetCString(buffer, info);
#if RETRO_REV02
    sprintf(buffer2, "DUMMY SetPresence(%d, %s) -> %s", id, buffer, (richPresence->curID != id ? "Successful Set" : "Redundant Set"));
#else
    sprintf(buffer2, "DUMMY SetPresence(%d, %s)", id, buffer);
#endif
    printLog(SEVERITY_NONE, buffer2);
}

#if !RETRO_REV02
void TrackActClear() { printLog(SEVERITY_NONE, "DUMMY TrackActClear()"); }
void TrackTAClear(byte a1, byte a2, byte a3, int a4) { printLog(SEVERITY_NONE, "DUMMY TrackTAClear(%d,%d,%d,%d)", a1, a2, a3, a4); }
void TrackEnemyDefeat() { printLog(SEVERITY_NONE, "DUMMY TrackEnemyDefeat()"); }
void ClearPrerollErrors() { printLog(SEVERITY_NONE, "DUMMY ClearPrerollErrors()"); }
#endif

#if RETRO_REV02
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
        strcpy(val->name, userDebugValNames[i]);
        val->value      = userCore->values[i];
        val->min        = 0;

        if (i == 2) {
            val->type       = 2;
            val->valByteCnt = 1;
            val->max = REGION_EU;
        }
        else if (i == 3) {
            val->type       = 2;
            val->valByteCnt = 1;
            val->max = LANGUAGE_TC;
        }
        else {
            val->type       = 0;
            val->valByteCnt = 4;
            val->max = 1;
        }
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

#if !RETRO_REV02
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
#if RETRO_REV02
    if (!userStorage->noSaveActive) {
#endif
        LoadUserFile(filename, buffer, bufSize);

        if (callback)
            callback(100);
#if RETRO_REV02
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
#if RETRO_REV02
    if (!userStorage->noSaveActive) {
#endif
        if (compress) {
            // compress lo
        }
        SaveUserFile(filename, buffer, bufSize);

        if (callback)
            callback(100);
#if RETRO_REV02
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
#if RETRO_REV02
    if (!userStorage->noSaveActive) {
#endif
        DeleteUserFile(filename);

        if (callback)
            callback(100);
#if RETRO_REV02
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

#if RETRO_REV02
int LoadDBFromBuffer(UserDB *userDB, byte *writeBuffer)
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

        sprintf(userDB->columnNames[c], "%s", (char *)writeBuffer);
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
        *(int *)writeBuffer = (int)GetUserDBWriteSize(userDB); // used size
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
            sprintf((char *)writeBuffer, "%s", userDB->columnNames[c]);
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
        int result = LoadDBFromBuffer(&userDBStorage->userDB[tableID], (byte*)userDBStorage->readBuffer[tableID]);
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
        int totalSize = (int)GetUserDBWriteSize(userDB);
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
        case SETTINGS_WINDOWED: return !engine.isFullScreen; // windowed
        case SETTINGS_BORDERED: return !engine.borderless;   // bordered
        case SETTINGS_EXCLUSIVEFS: return engine.exclusiveFS;
        case SETTINGS_VSYNC: return engine.vsync;
        case SETTINGS_TRIPLEBUFFERED: return engine.tripleBuffer;
        case SETTINGS_WINDOW_WIDTH: return engine.windowWidth;
        case SETTINGS_WINDOW_HEIGHT: return engine.windowHeight;
        case SETTINGS_FSWIDTH: return engine.fsWidth;
        case SETTINGS_FSHEIGHT: return engine.fsHeight;
        case SETTINGS_REFRESHRATE: return engine.refreshRate;
        case SETTINGS_SHADERSUPPORT: return engine.shaderSupport;
        case SETTINGS_SHADERID: return engine.shaderID;
        case SETTINGS_SCREENCOUNT: return engine.screenCount;
#if RETRO_REV02
        case SETTINGS_DIMTIMER: return engine.dimTimer;
#endif
        case SETTINGS_STREAMSENABLED: return engine.streamsEnabled;
        case SETTINGS_STREAM_VOL: return (int)(engine.streamVolume * 1024.0); 
        case SETTINGS_SFX_VOL: return (int)(engine.soundFXVolume * 1024.0); 
        case SETTINGS_LANGUAGE:
#if RETRO_REV02
            return curSKU.language; 
#else
            return gameVerInfo.language;
#endif
        case SETTINGS_CHANGED: return settingsChanged;
        default: break;
    }
    return 0;
}

void SetSettingsValue(int id, int val)
{
    bool32 bVal = val;
    switch (id) {
        case SETTINGS_WINDOWED:
            if ((!engine.isFullScreen) != bVal) {
                settingsChanged     = true;
                engine.isFullScreen = !bVal;
            }
            break;
        case SETTINGS_BORDERED:
            if ((!engine.borderless) != bVal) {
                settingsChanged   = true;
                engine.borderless = !bVal;
            }
            break;
        case SETTINGS_EXCLUSIVEFS:
            if (engine.exclusiveFS != bVal) {
                settingsChanged    = 1;
                engine.exclusiveFS = bVal;
            }
            break;
        case SETTINGS_VSYNC:
            if (engine.vsync != bVal) {
                settingsChanged = true;
                engine.vsync    = bVal;
            }
            break;
        case SETTINGS_TRIPLEBUFFERED:
            if (engine.tripleBuffer != bVal) {
                settingsChanged     = true;
                engine.tripleBuffer = bVal;
            }
            break;
        case SETTINGS_WINDOW_WIDTH:
            if (engine.windowWidth != val) {
                engine.windowWidth = val;
                settingsChanged    = true;
            }
            break;
        case SETTINGS_WINDOW_HEIGHT:
            if (engine.windowHeight != val) {
                engine.windowHeight = val;
                settingsChanged     = true;
            }
            break;
        case SETTINGS_FSWIDTH: engine.fsWidth = val; break;
        case SETTINGS_FSHEIGHT: engine.fsHeight = val; break;
        case SETTINGS_REFRESHRATE: engine.refreshRate = val; break;
        case SETTINGS_SHADERSUPPORT: engine.shaderSupport = val; break;
        case SETTINGS_SHADERID:
            if (engine.shaderID != val) {
                engine.shaderID = val;
                settingsChanged     = true;
            }
            break;
        case SETTINGS_SCREENCOUNT: engine.screenCount = val; break;
#if RETRO_REV02
        case SETTINGS_DIMTIMER: engine.dimTimer = val; break;
#endif
        case SETTINGS_STREAMSENABLED:
            if (engine.streamsEnabled != bVal)
                settingsChanged = true;
            engine.streamsEnabled = bVal;
            break;
        case SETTINGS_STREAM_VOL:
            if (engine.streamVolume != (val / 1024.0f)) {
                engine.streamVolume = (float)val / 1024.0f;
                settingsChanged     = true;
            }
            break;
        case SETTINGS_SFX_VOL:
            if (engine.soundFXVolume != ((float)val / 1024.0f)) {
                engine.soundFXVolume = (float)val / 1024.0f;
                settingsChanged      = true;
            }
            break;
        case SETTINGS_LANGUAGE:
#if RETRO_REV02
            curSKU.language = val; 
#else
            gameVerInfo.language = val;
#endif
            break;
        case SETTINGS_STORE:
            settingsStorage.windowed      = engine.isFullScreen;
            settingsStorage.bordered      = engine.borderless;
            settingsStorage.exclusiveFS   = engine.exclusiveFS;
            settingsStorage.vsync         = engine.vsync;
            settingsStorage.shaderSupport = engine.shaderSupport;
            settingsStorage.fsWidth       = engine.fsWidth;
            settingsStorage.fsHeight      = engine.fsHeight;
            settingsStorage.gameHeight    = engine.gameHeight;
            settingsStorage.windowActive  = engine.windowActive;
            settingsStorage.shaderID      = engine.shaderID;
            settingsStorage.screenCount   = engine.screenCount;
            settingsStorage.dimTimer      = engine.dimTimer;
            settingsStorage.dimLimit      = engine.dimLimit;
            settingsStorage.dimMax        = engine.dimMax;
            settingsStorage.dimPercent    = engine.dimPercent;
            settingsStorage.refreshRate   = engine.refreshRate;
            settingsStorage.windowWidth   = engine.windowWidth;
            settingsStorage.windowHeight  = engine.windowHeight;
            settingsStorage.pixWidth      = pixWidth;
            //settingsStorage.mouseX        = 0;
            //settingsStorage.mouseY        = 0;
            //settingsStorage.field_8       = 0;
            //settingsStorage.field_C       = 0;
            break;
        case SETTINGS_RELOAD:
            settingsChanged               = true;

            engine.isFullScreen  = settingsStorage.windowed;
            engine.borderless    = settingsStorage.bordered;
            engine.exclusiveFS   = settingsStorage.exclusiveFS;
            engine.vsync         = settingsStorage.vsync;
            engine.shaderSupport = settingsStorage.shaderSupport;
            engine.fsWidth       = settingsStorage.fsWidth;
            engine.fsHeight      = settingsStorage.fsHeight;
            engine.gameHeight    = settingsStorage.gameHeight;
            engine.windowActive  = settingsStorage.windowActive;
            engine.shaderID      = settingsStorage.shaderID;
            engine.screenCount   = settingsStorage.screenCount;
            engine.dimTimer      = settingsStorage.dimTimer;
            engine.dimLimit      = settingsStorage.dimLimit;
            engine.dimMax        = settingsStorage.dimMax;
            engine.dimPercent    = settingsStorage.dimPercent;
            engine.refreshRate   = settingsStorage.refreshRate;
            engine.windowWidth   = settingsStorage.windowWidth;
            engine.windowHeight  = settingsStorage.windowHeight;
            pixWidth             = settingsStorage.pixWidth;
            //0                    = settingsStorage.mouseX;
            //0                    = settingsStorage.mouseY;
            //0                    = settingsStorage.field_8;
            //0                    = settingsStorage.field_C;
            break;
        case SETTINGS_CHANGED: settingsChanged = val; break;
        case SETTINGS_WRITE: writeSettings(val); break;
        default: return;
    }
}

char buttonNames[18][8] = { "U", "D", "L", "R", "START", "SELECT", "LSTICK", "RSTICK", "L1", "R1", "C", "Z", "A", "B", "X", "Y", "L2", "R2" };

void readSettings()
{
    engine.screenCount = 1;
    engine.gameHeight  = SCREEN_YSIZE;

    dictionary *ini = iniparser_load("Settings.ini");

    if (ini) {
        const char *result = "";

#if RETRO_REV02
        curSKU.language = iniparser_getint(ini, "Game:language", LANGUAGE_EN);
#else
        gameVerInfo.language = (int)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);
#endif

        // Consoles load the entire file and buffer it, while pc just io's the file when needed
        if (CheckDataFile(iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"), 0, RETRO_PLATFORM != RETRO_WIN))
            engine.devMenu = iniparser_getboolean(ini, "Game:devMenu", false);
        else
            engine.devMenu = true;

#if !RETRO_USE_ORIGINAL_CODE
        sprintf(gameLogicName, "%s", iniparser_getstring(ini, "Game:gameLogic", "Game"));
#else
    sprintf(gameLogicName, "Game"));
#endif

        engine.startFullScreen = !iniparser_getboolean(ini, "Video:windowed", true);
        engine.borderless      = !iniparser_getboolean(ini, "Video:border", true);
        engine.exclusiveFS     = iniparser_getboolean(ini, "Video:exclusiveFS", false);
        engine.vsync           = iniparser_getboolean(ini, "Video:vsync", false);
        engine.tripleBuffer    = iniparser_getboolean(ini, "Video:tripleBuffering", false);

        pixWidth = iniparser_getint(ini, "Video:pixWidth", DEFAULT_SCREEN_XSIZE);

        engine.windowWidth   = iniparser_getint(ini, "Video:winWidth", DEFAULT_SCREEN_XSIZE);
        engine.windowHeight  = iniparser_getint(ini, "Video:winHeight", SCREEN_YSIZE);
        engine.fsWidth       = iniparser_getint(ini, "Video:fsWidth", 0);
        engine.fsHeight      = iniparser_getint(ini, "Video:fsHeight", 0);
        engine.refreshRate   = iniparser_getint(ini, "Video:refreshRate", 60);
        engine.shaderSupport = iniparser_getboolean(ini, "Video:shaderSupport", true);
        engine.shaderID      = iniparser_getint(ini, "Video:screenShader", 0);

        engine.streamsEnabled = iniparser_getboolean(ini, "Audio:streamsEnabled", true);
        engine.streamVolume   = iniparser_getdouble(ini, "Audio:streamVolume", 0.8);
        engine.soundFXVolume  = iniparser_getdouble(ini, "Audio:sfxVolume", 1.0);

#if RETRO_USING_SDL2
        int defKeyMaps[PLAYER_COUNT + 1][12] = {
            { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN },
            { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D,
              SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB },
            { SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6, SDL_SCANCODE_J, SDL_SCANCODE_J, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_LEFTBRACKET, SDL_SCANCODE_RIGHTBRACKET },
            { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN },
            { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN }
        };
#else
    int defKeyMaps[PLAYER_COUNT][12] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                         { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
#endif

        for (int i = 1; i <= PLAYER_COUNT; ++i) {
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

        gamePadCount = 0;
        while (true) {
            char buffer[0x30];
            sprintf(buffer, "GamePad Map %d:name", gamePadCount + 1);
            if (strcmp(iniparser_getstring(ini, buffer, "optionNotFound"), "optionNotFound") != 0) {
                gamePadCount++;
            }
            else {
                break;
            }
        }

        AllocateStorage(sizeof(GamePadMappings) * gamePadCount, (void **)&gamePadMappings, DATASET_STG, true);

        for (int i = 1; i <= gamePadCount; ++i) {
            char buffer[0x30];
            char buffer2[0x100];

            sprintf(buffer, "GamePad Map %d:name", i);
            sprintf(gamePadMappings[i].name, "%s", iniparser_getstring(ini, buffer, 0));

            sprintf(buffer, "GamePad Map %d:type", i);
            gamePadMappings[i].type = iniparser_getint(ini, buffer, 0);

            sprintf(buffer, "GamePad Map %d:vendorID", i);
            gamePadMappings[i].vendorID = iniparser_getint(ini, buffer, 0);

            sprintf(buffer, "GamePad Map %d:productID", i);
            gamePadMappings[i].productID = iniparser_getint(ini, buffer, 0);

            sprintf(buffer, "GamePad Map %d:mappingTypes", i);
            sprintf(buffer2, "%s", iniparser_getstring(ini, buffer, 0));

            char *tok = strtok(buffer2, ", ");
            for (int b = 0; tok; ++b) {
                gamePadMappings[i].buttons[b].mappingType = atoi(tok);
                tok                                       = strtok(0, " ,.-");
            }

            sprintf(buffer, "GamePad Map %d:offsets", i);
            sprintf(buffer2, "%s", iniparser_getstring(ini, buffer, 0));

            tok = strtok(buffer2, ", ");
            for (int b = 0; tok; ++b) {
                gamePadMappings[i].buttons[b].offset = atoi(tok);
                tok                                  = strtok(0, " ,.-");
            }

            sprintf(buffer, "GamePad Map %d:maskVals", i);
            sprintf(buffer2, "%s", iniparser_getstring(ini, buffer, 0));
            tok = strtok(buffer2, ", ");
            for (int b = 0; tok; ++b) {
                int mask = 1;
                for (int m = 0; m < 18; ++m) {
                    if (strcmp(buttonNames[i], tok) == 0) {
                        gamePadMappings[i].buttons[b].maskVal = mask;
                        break;
                    }
                    mask <<= 1;
                }

                tok = strtok(0, " ,.-");
            }
        }

        iniparser_freedict(ini);
    }
    else {
        engine.startFullScreen = engine.startFullScreen = false;
        engine.borderless                               = false;
        engine.exclusiveFS                              = true;
        engine.vsync                                    = true;
        engine.tripleBuffer                             = false;
        engine.shaderSupport                            = true;
        pixWidth                                        = 424;
        engine.fsWidth                                  = 0;
        engine.windowWidth                              = pixWidth * 1;
        engine.fsHeight                                 = 0;
        engine.refreshRate                              = 0;
        engine.shaderID                                 = SHADER_NONE;
        engine.streamsEnabled                           = 1;
        engine.windowHeight                             = SCREEN_YSIZE * 1;
        engine.streamVolume                             = 1.0f;
        engine.soundFXVolume                            = 1.0f;
        engine.devMenu                                  = false;

#if RETRO_USING_SDL2
        int defKeyMaps[PLAYER_COUNT + 1][12] = {
            { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN },
            { SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT, SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D,
              SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_RETURN, SDL_SCANCODE_TAB },
            { SDL_SCANCODE_KP_8, SDL_SCANCODE_KP_5, SDL_SCANCODE_KP_4, SDL_SCANCODE_KP_6, SDL_SCANCODE_J, SDL_SCANCODE_J, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_U, SDL_SCANCODE_I, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_LEFTBRACKET, SDL_SCANCODE_RIGHTBRACKET },
            { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN },
            { SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN,
              SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN, SDL_SCANCODE_UNKNOWN }
        };
#else
        int defKeyMaps[PLAYER_COUNT][12] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
                                             { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
#endif

       for (int i = 1; i <= PLAYER_COUNT; ++i) {
            controller[i].keyUp.keyMap     = defKeyMaps[i][0];
            controller[i].keyDown.keyMap   = defKeyMaps[i][1];
            controller[i].keyLeft.keyMap   = defKeyMaps[i][2];
            controller[i].keyRight.keyMap  = defKeyMaps[i][3];
            controller[i].keyA.keyMap      = defKeyMaps[i][4];
            controller[i].keyB.keyMap      = defKeyMaps[i][5];
            controller[i].keyC.keyMap      = defKeyMaps[i][6];
            controller[i].keyX.keyMap      = defKeyMaps[i][7];
            controller[i].keyY.keyMap      = defKeyMaps[i][8];
            controller[i].keyZ.keyMap      = defKeyMaps[i][9];
            controller[i].keyStart.keyMap  = defKeyMaps[i][10];
            controller[i].keySelect.keyMap = defKeyMaps[i][11];
        }

        writeSettings(true);
        engine.devMenu = CheckDataFile("Data.rsdk", 0, RETRO_PLATFORM != RETRO_WIN);
    }
}

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

void writeSettings(bool32 writeToFile)
{
    //only done on windows and "dev", consoles use "options.bin"
#if RETRO_REV02
    if (curSKU.platform != PLATFORM_WIN && curSKU.platform != PLATFORM_DEV)
        return;
#else
    if (gameVerInfo.platform != PLATFORM_WIN && gameVerInfo.platform != PLATFORM_DEV)
        return;
#endif

    if (settingsChanged || writeToFile) {
        dictionary *ini = iniparser_load("Settings.ini");
        FileIO *file    = fOpen("Settings.ini", "wb");
        writeText(file, "; Retro Engine Config File\n\n");
        writeText(file, "[Game]\n");
        if (ini) {
            if (strcmp(iniparser_getstring(ini, "Game:dataFile", "optionNotFound"), "optionNotFound") != 0) {
                writeText(file, "dataFile=%s\n", iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"));
            }

#if !RETRO_USE_ORIGINAL_CODE
            if (strcmp(iniparser_getstring(ini, "Game:gameLogic", "optionNotFound"), "optionNotFound") != 0) {
                writeText(file, "gameLogic=%s\n", iniparser_getstring(ini, "Game:gameLogic", "Game"));
            }
#endif

            if (strcmp(iniparser_getstring(ini, "Game:devMenu", "optionNotFound"), "optionNotFound") != 0)
                writeText(file, "devMenu=%s\n", (engine.devMenu ? "y" : "n"));
        }

#if RETRO_REV02
        writeText(file, "language=%d\n", curSKU.language);
#else
        writeText(file, "language=%d\n", gameVerInfo.language);
#endif
        writeText(file, "\n[Video]\n");
        writeText(file, "; NB: Fullscreen Resolution can be explicitly set with values fsWidth and fsHeight\n");
        writeText(file, "; If not listed, fullscreen will just use the desktop resolution\n");
        writeText(file, "windowed=%s\n", (!engine.startFullScreen ? "y" : "n"));
        writeText(file, "border=%s\n", (!engine.borderless ? "y" : "n"));
        writeText(file, "exclusiveFS=%s\n", (engine.exclusiveFS ? "y" : "n"));
        writeText(file, "vsync=%s\n", (engine.vsync ? "y" : "n"));
        writeText(file, "tripleBuffering=%s\n", (engine.tripleBuffer ? "y" : "n"));
        if (ini) {
            if (!strcmp(iniparser_getstring(ini, "Video:pixWidth", "optionNotFound"), "optionNotFound") == 0)
                writeText(file, "pixWidth=%d\n", pixWidth);
        }
        writeText(file, "winWidth=%d\n", engine.windowWidth);
        writeText(file, "winHeight=%d\n", engine.windowHeight);
        if (engine.fsWidth > 0)
            writeText(file, "fsWidth=%d\n", engine.fsWidth);
        if (engine.fsHeight > 0)
            writeText(file, "fsHeight=%d\n", engine.fsHeight);
        if (engine.refreshRate > 0)
            writeText(file, "refreshRate=%d\n", engine.refreshRate);

        writeText(file, "shaderSupport=%s\n", (engine.shaderSupport ? "y" : "n"));
        writeText(file, "screenShader=%d\n", engine.shaderID);
        writeText(file, "\n[Audio]\n");
        writeText(file, "streamsEnabled=%s\n", (engine.streamsEnabled ? "y" : "n"));
        writeText(file, "streamVolume=%f\n", engine.streamVolume);
        writeText(file, "sfxVolume=%f\n", engine.soundFXVolume);

        for (int i = 1; i <= PLAYER_COUNT; ++i) {
            writeText(file, "\n[Keyboard Map %d]\n", i);
            writeText(file, "up=0x%x\n", controller[i].keyUp.keyMap);
            writeText(file, "down=0x%x\n", controller[i].keyDown.keyMap);
            writeText(file, "left=0x%x\n", controller[i].keyLeft.keyMap);
            writeText(file, "right=0x%x\n", controller[i].keyRight.keyMap);
            writeText(file, "buttonA=0x%x\n", controller[i].keyA.keyMap);
            writeText(file, "buttonB=0x%x\n", controller[i].keyB.keyMap);
            writeText(file, "buttonC=0x%x\n", controller[i].keyC.keyMap);
            writeText(file, "buttonX=0x%x\n", controller[i].keyX.keyMap);
            writeText(file, "buttonY=0x%x\n", controller[i].keyY.keyMap);
            writeText(file, "buttonZ=0x%x\n", controller[i].keyZ.keyMap);
            writeText(file, "start=0x%x\n", controller[i].keyStart.keyMap);
            writeText(file, "select=0x%x\n", controller[i].keySelect.keyMap);
        }

        for (int i = 0; i < gamePadCount; ++i) {
            writeText(file, "\n[Keyboard Map %d]\n", i + 1);
            writeText(file, "name=%s\n", gamePadMappings[i].name);
            writeText(file, "type=0x%x\n", gamePadMappings[i].type);
            writeText(file, "vendorID=0x%x\n", gamePadMappings[i].vendorID);
            writeText(file, "productID=0x%x\n", gamePadMappings[i].productID);

            writeText(file, "mappingTypes=");
            for (int b = 0; b < 24; ++b) {
                writeText(file, "%d,", gamePadMappings[i].buttons[b].mappingType);
            }
            writeText(file, "\n");
            writeText(file, "offsets=");
            for (int b = 0; b < 24; ++b) {
                writeText(file, "%d,", gamePadMappings[i].buttons[b].offset);
            }
            writeText(file, "\n");
            writeText(file, "maskVals=");
            for (int b = 0; b < 24; ++b) {
                if (gamePadMappings[i].buttons[b].maskVal) {
                    int m = 0;
                    while (true) {
                        if (1 << m == gamePadMappings[i].buttons[b].maskVal) {
                            writeText(file, "%d,", gamePadMappings[i].buttons[b].maskVal);
                            break;
                        }
                    }
                    if (m == 18) {
                        writeText(file, "?,");
                    }
                }
                else {
                    writeText(file, "?,");
                }
            }
            writeText(file, "\n");
        }

        iniparser_freedict(ini);
        fClose(file);
    }
}
