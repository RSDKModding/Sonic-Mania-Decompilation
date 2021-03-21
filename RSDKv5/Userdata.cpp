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

        userCore->values[0]   = &engine.hasPlus;
        userCore->values[1]   = &curSKU.platform;
        userCore->values[2]   = &curSKU.region;
        userCore->values[3]   = &curSKU.language;
        userCore->values[4]   = &engine.confirmFlip;
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
        // userStorage->LoadUserFile   = TryLoadUserFile;
        // userStorage->SaveUserFile   = TrySaveUserFile;
        // userStorage->DeleteUserFile = TryDeleteUserFile;
        userStorage->unknown8 = UserStorageUnknown8;
#endif

#if !RETRO_USE_PLUS
        SetFuncPtr("ExitGame", ExitGame);
        // SetFuncPtr("ClearAchievements", Engine_ClearAchievements);
        SetFuncPtr("UnlockAchievement", TryUnlockAchievement);
        SetFuncPtr("FetchLeaderboard", FetchLeaderboard);
        // SetFuncPtr("LeaderboardStatus", Engine_LeaderboardStatus);
        // SetFuncPtr("LeaderboardEntryCount", Engine_LeaderboardEntryCount);
        // SetFuncPtr("LeaderboardReadEntry", Engine_LeaderboardReadEntry);
        // SetFuncPtr("TrackActClear", Engine_TrackActClear);
        // SetFuncPtr("TrackTAClear", Engine_TrackTAClear);
        // SetFuncPtr("TrackEnemyDefeat", Engine_TrackEnemyDefeat);
        // SetFuncPtr("ClearPrerollErrors", Engine_ClearPrerollErrors);
        SetFuncPtr("TryAuth", TryAuth);
        // SetFuncPtr("GetUserAuthStatus", Engine_GetUserAuthStatus);
        SetFuncPtr("TryInitStorage", TryInitStorage);
        SetFuncPtr("GetStorageStatus", GetUserStorageStatus);
        SetFuncPtr("LoadUserFile", TryLoadUserFile);
        SetFuncPtr("SaveUserFile", TrySaveUserFile);
        SetFuncPtr("SaveSettingsINI", writeSettings);
        SetFuncPtr("GetUserLanguage", GetUserLanguage);
        // SetFuncPtr("ControllerIDForInputID", Engine_ControllerIDForInputID);
        // SetFuncPtr("MostRecentActiveControllerID", Engine_MostRecentActiveControllerID);
        // SetFuncPtr("AssignControllerID", Engine_AssignControllerID);
        // SetFuncPtr("ResetControllerAssignments", ResetControllerAssignments);
        // SetFuncPtr("InputIDIsDisconnected", Engine_InputIDIsDisconnected);
        // SetFuncPtr("GetControllerType", Engine_GetControllerType);
        // SetFuncPtr("ShowSteamControllerOverlay", Engine_ShowSteamControllerOverlay);
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

int GetUserLanguage() { return curSKU.language; }
int GetUserRegion() { return curSKU.region; }
int GetUserPlatform() { return curSKU.platform; }
#if RETRO_USE_PLUS
int GetConfirmButtonFlip()
{
    printConsole("DUMMY GetConfirmButtonFlip() -> 0\n");
    return 0;
}
void LaunchManual() { printConsole("DUMMY LaunchManual()\n"); }
#endif
void ExitGame() { engine.running = false; }

int ShowExtensionOverlay(byte overlay)
{
    printLog(SEVERITY_WARN, "Show Extension Overlay: %d", overlay);
    return 1;
}

void TryUnlockAchievement(const char *name) { printLog(SEVERITY_NONE, "DUMMY TryUnlockAchievement(%s)", name); }

void FetchLeaderboard(int a2, int a3) { printLog(SEVERITY_NONE, "DUMMY FetchLeaderboard(%d, %d)\n", a2, a3); }
void TrackScore(int a2, int a3, int a4) { printLog(SEVERITY_NONE, "DUMMY TrackScore(%d, %d, %d)\n", a2, a3, a4); }

void SetPresence(byte a2, TextInfo *info)
{
    char buffer[0xFF];
    char buffer2[0xFF];
    GetCString(buffer, info);
#if RETRO_USE_PLUS
    sprintf(buffer2, "DUMMY SetPresence(%d, %s) -> %s\n", a2, buffer, (richPresence->status != a2 ? "Successful Set" : "Redundant Set"));
#else
    sprintf(buffer2, "DUMMY SetPresence(%d, %s)\n", a2, buffer);
#endif
    printConsole(buffer2);
}

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

bool32 TryLoadUserFile(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int)) { return false; }
bool32 TrySaveUserFile(const char *filename, void *buffer, unsigned int bufSize, int (*callback)(int), bool32 compress) { return false; }

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
    // userDBStorage->field_2287C4[tableID] = sub_5ECD20;
    userDBStorage->callbacks[tableID] = callback;
    TryLoadUserFile(filename, userDBStorage->readBuffer[tableID], sizeof(UserDB), userDBStorage->saveCallback[tableID]);
    return tableID;
}
bool32 SaveUserDB(ushort tableID, int (*callback)(int))
{
    UserDB *userDB = &userDBStorage->userDB[tableID];

    bool32 success = false;
    if (userDB->active) {
        int totalSize = 0; // GetUserDBWriteSize(userDB);
        AllocateStorage(totalSize, (void **)&userDBStorage->writeBuffer[tableID], DATASET_TMP, true);
        // GetUserDBWriteBuffer(totalSize, userDBStorage->writeBuffer[tableID]);
        userDBStorage->callbacks[tableID] = callback;
        // userDBStorage->field_2287C4[tableID] = UserDB_FinishCallbacks;
        success = TrySaveUserFile(userDB->name, userDBStorage->writeBuffer[tableID], totalSize, userDBStorage->saveCallback[tableID], true);
    }
    else {
        if (callback)
            success = callback(500);
    }
    return success;
}

ushort GetUserDBByID(ushort tableID, uint uuid)
{
    if (tableID == -1)
        return -1;
    if (!uuid)
        return -1;
    UserDB *userDB = &userDBStorage->userDB[tableID];
    if (!userDB->active)
        return -1;
    if (!userDB->entryCount)
        return -1;

    for (int i = 0; i < userDB->entryCount; ++i) {
        if (uuid == userDB->entries[i].uuid) {
            return i;
        }
    }
    return -1;
}

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
        case 11: return engine.screenShader;
        case 12: return engine.shaderUnknown;
        case 13: return engine.dimTimer;
        case 14: return engine.streamsEnabled;
        case 15: return (int)(engine.streamVolume * 1024.0); break;
        case 16: return (int)(engine.soundFXVolume * 1024.0); break;
        case 17: return curSKU.language; break;
        case 20: return settingsChanged; break;
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
            if (engine.screenShader != val) {
                engine.screenShader = val;
                settingsChanged     = true;
            }
            break;
        case 12: engine.shaderUnknown = val; break;
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
        case 17: curSKU.language = val; break;
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

    curSKU.language = (int)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);

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
    engine.screenShader  = (int)strtol(iniparser_getstring(ini, "Video:refreshRate", "0"), NULL, 0);

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
