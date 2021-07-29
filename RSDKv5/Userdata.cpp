#include "RetroEngine.hpp"
#include "zlib/zlib.h"

#if RETRO_REV02
DummyCore *dummmyCore = NULL;
DummyCore *userCore   = NULL;

DummyAchievements *achievements = NULL;
DummyLeaderboards *leaderboards = NULL;
DummyRichPresence *richPresence = NULL;
DummyStats *stats               = NULL;
DummyUserStorage *userStorage   = NULL;

UserDBStorage *userDBStorage = NULL;
#endif

// Start custom achievement code
// this is added because we don't have access to any store APIs that would otherwise use this feature
#include <vector>
struct AchievementInfo {
    const char *identifier;
    const char *name;
    const char *description;
    bool32 achieved;
};

std::vector<AchievementInfo> achievementList;
std::vector<int> achievementStack;

void addAchievement(const char *identifier, const char *name, const char *desc)
{
    AchievementInfo info;
    info.identifier  = identifier;
    info.name        = name;
    info.description = desc;
    info.achieved    = false;
    achievementList.push_back(info);
}

// End custom achievement code

// Start custom leaderboard code
// this is added because we don't have access to any store APIs that would otherwise use this feature
struct LeaderboardInfo {
    const char *name;
    int id;
};

std::vector<LeaderboardInfo> leaderboardList;

// End custom leaderboard code

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

        if (!userDBStorage)
            userDBStorage = (UserDBStorage *)malloc(sizeof(UserDBStorage));
        MEM_ZEROP(userDBStorage);

        InitUserStorageDB(userDBStorage);

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
#if RETRO_VER_EGS
        userCore->EpicUnknown1      = nullUserFunc;
        userCore->Epic_Checkout     = EGS_Checkout;
        userCore->ShowEncorePage    = ShowEncorePage;
        userCore->EpicUnknown4      = EGS_Unknown4;
        userCore->RegisterHIDDevice = nullUserFunc;
        userCore->EpicUnknown6      = nullUserFunc;
#endif

        userCore->values[0]   = (int *)&engine.hasPlus;
        userCore->values[1]   = &curSKU.platform;
        userCore->values[2]   = &curSKU.region;
        userCore->values[3]   = &curSKU.language;
        userCore->values[4]   = (int *)&engine.confirmFlip;
        userCore->debugValCnt = 5;

        achievements->InitUnknown1   = nullUserFunc;
        achievements->SetDebugValues = nullUserFunc;
        achievements->InitUnknown2   = nullUserFunc;
#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
        achievements->CheckAchievementsEnabled = CheckAchievementsEnabled;
        achievements->GetAchievementNames      = GetAchievementNames;
        achievements->GetAchievementText       = GetAchievementText;
        achievements->GetAchievementName       = GetAchievementName;
        achievements->Unknown8                 = AchivementsUnknown8;
        achievements->GetNextAchievementID     = GetNextAchievementID;
        achievements->RemoveLastAchievementID  = RemoveLastAchievementID;
#endif
        achievements->UnlockAchievement = TryUnlockAchievement;

        leaderboards->SetDebugValues = nullUserFunc;
        leaderboards->InitUnknown1   = nullUserFunc;
        leaderboards->InitUnknown2   = nullUserFunc;
        leaderboards->unknown4       = (int (*)())nullUserFunc;
#if RETRO_VER_EGS
        leaderboards->unknown6 = (int (*)())nullUserFunc;
#endif
        leaderboards->FetchLeaderboard = (void (*)(int, int))nullUserFunc;
        leaderboards->unknown5         = nullUserFunc;
        leaderboards->TrackScore       = (void (*)(int, int, int))nullUserFunc;
        leaderboards->GetStatus        = GetLeaderboardStatus;

        richPresence->SetDebugValues = nullUserFunc;
        richPresence->InitUnknown1   = nullUserFunc;
        richPresence->InitUnknown2   = nullUserFunc;
        richPresence->SetPresence    = SetPresence;

        stats->SetDebugValues = nullUserFunc;
        stats->InitUnknown1   = nullUserFunc;
        stats->InitUnknown2   = nullUserFunc;
        stats->TryTrackStat   = TryTrackStat;

        userStorage->InitUnknown1   = (int (*)())nullUserFunc;
        userStorage->SetDebugValues = (int (*)())nullUserFunc;
        userStorage->InitUnknown2   = (int (*)())nullUserFunc;
        userStorage->TryAuth        = TryAuth;
        userStorage->TryInitStorage = TryInitStorage;
        userStorage->GetUsername    = GetUserName;
        userStorage->LoadUserFile   = TryLoadUserFile;
        userStorage->SaveUserFile   = TrySaveUserFile;
        userStorage->DeleteUserFile = TryDeleteUserFile;
        userStorage->unknown8       = ClearPrerollErrors;

        achievements->status    = STATUS_OK;
        stats->status           = STATUS_OK;
        userStorage->authStatus = STATUS_OK;
        userStorage->saveStatus = STATUS_OK;
        userStorage->statusCode = STATUS_OK;
#endif
    }

    // Add achievements
    achievementList.clear();
    achievementStack.clear();
    addAchievement("ACH_GOLD_MEDAL", "No Way? No Way!", "Collect gold medallions in Blue Spheres Bonus stage");
    addAchievement("ACH_SILVER_MEDAL", "Full Medal Jacket", "Collect silver medallions in Blue Spheres Bonus stage");
    addAchievement("ACH_EMERALDS", "Magnificent Seven", "Collect all seven Chaos Emeralds");
    addAchievement("ACH_GAME_CLEARED", "See You Next Game", "Achieve any ending");
    addAchievement("ACH_STARPOST", "Superstar", "Spin the Star Post!");
    addAchievement("ACH_SIGNPOST", "That's a Two-fer", "Find the hidden item boxes at the end of the Zone");
    addAchievement("ACH_GHZ", "Now It Can't Hurt You Anymore", "What would happen if you cross a bridge with a fire shield?");
    addAchievement("ACH_CPZ", "Triple Trouble", "Try for a 3 chain combo!");
    addAchievement("ACH_SPZ", "The Most Famous Hedgehog in the World", "Have your photos taken in Studiopolis Zone");
    addAchievement("ACH_FBZ", "Window Shopping", "Let the wind take you through");
    addAchievement("ACH_PGZ", "Crate Expectations", "Wreak havoc at the propaganda factory");
    addAchievement("ACH_SSZ", "King of Speed", "Get through Stardust Speedway Zone as quickly as possible");
    addAchievement("ACH_HCZ", "Boat Enthusiast", "Try pushing a barrel to see how far it goes");
    addAchievement("ACH_MSZ", "The Password is \"Special Stage\"", "Try pushing a barrel to see how far it goes");
    addAchievement("ACH_OOZ", "Secret Sub", "You might have to submerge to find it");
    addAchievement("ACH_LRZ", "Without a Trace", "Barrel through the lava, don't let anything stop you");
    addAchievement("ACH_MMZ", "Collect 'Em All", "Gotta gacha 'em all");
    addAchievement("ACH_TMZ", "Professional Hedgehog", "That elusive perfect run, only a professional can achieve");

    int achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int));
#if RETRO_REV02
    userStorage->LoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int), NULL);
#else
    LoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int));
#endif
    for (int i = 0; i < (int)achievementList.size(); ++i) {
        achievementList[i].achieved = achievementsRAM[i];
    }
}
void releaseUserData()
{
    int achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int));
    for (int i = 0; i < (int)achievementList.size(); ++i) {
        achievementsRAM[i] = achievementList[i].achieved;
    }
#if RETRO_REV02
    userStorage->SaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int), NULL, false);
#else
    SaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int));
#endif

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
bool32 GetConfirmButtonFlip()
{
    // printLog(PRINT_NORMAL, "DUMMY GetConfirmButtonFlip() -> %d", engine.confirmFlip);
    return engine.confirmFlip;
}
bool32 GetXYButtonFlip()
{
    // printLog(PRINT_NORMAL, "DUMMY GetXYButtonFlip() -> %d", engine.XYFlip);
    return engine.XYFlip;
}
void LaunchManual()
{
    printLog(PRINT_NORMAL, "DUMMY LaunchManual()");
    // TODO(?): open this url: http://www.sonicthehedgehog.com/mania/manual
}
void ExitGame() { engine.running = false; }
int controllerUnknown() { return 0; }

int ShowExtensionOverlay(byte overlay)
{
    printLog(PRINT_POPUP, "Show Extension Overlay: %d", overlay);
    return 1;
}
bool32 EGS_Checkout(int a1)
{
    printLog(PRINT_POPUP, "Checkout(%d)");
    return true;
}
int ShowEncorePage(int a1)
{
    printLog(PRINT_POPUP, "Show EncorePage Overlay: %d", a1);
    return 1;
}
void EGS_Unknown4(int a1) { printLog(PRINT_POPUP, "EGS_Unknown4(%d)", a1); }

void ClearAchievements() { printLog(PRINT_NORMAL, "DUMMY ClearAchievements()"); }

void TryUnlockAchievement(const char *name)
{
    printLog(PRINT_NORMAL, "DUMMY TryUnlockAchievement(%s)", name);

    int i = 0;
    for (; i < (int)achievementList.size(); ++i) {
        if (strcmp(name, achievementList[i].identifier) == 0) {
            if (!achievementList[i].achieved) {
                achievementStack.push_back(i);
                printLog(PRINT_NORMAL, "Unlocked Achievement: (%s, %d)", name, i);
                achievementList[i].achieved = true;
            }
            break;
        }
    }

    if (i == achievementList.size())
        printLog(PRINT_NORMAL, "Failed to Unlock Achievement: (%s)", name);
}

void GetAchievementNames(TextInfo *names, int count)
{
    int i = 0;
    for (; i < count && i < (int)achievementStack.size(); ++i) {
        SetText(&names[i], (char *)achievementList[i].name, 0);
    }
    for (; i < count; ++i) {
        SetText(&names[i], (char *)"Dummy Achievement", 0);
    }
}

TextInfo *GetAchievementText(TextInfo *info)
{
    SetText(info, (char *)"Achievement!", 0);
    return info;
}
TextInfo *GetAchievementName(TextInfo *info, uint id)
{
    id--;
    if (id <= achievementList.size())
        SetText(info, (char *)achievementList[id].name, 0);
    return info;
}

int GetNextAchievementID(void)
{
    if (achievementStack.size() > 0)
        return achievementStack[0] + 1;
    else
        return 0;
}

void RemoveLastAchievementID(void)
{
    if (achievementStack.size() > 0)
        achievementStack.erase(achievementStack.begin());
}

void FetchLeaderboard(const char* name, int id) { printLog(PRINT_NORMAL, "DUMMY FetchLeaderboard(%s, %d)", name, id); }
void TrackScore(const char *name, int score, void (*callback)(int status, int rank))
{
    printLog(PRINT_NORMAL, "DUMMY TrackScore(%s, %d, %p)", name, score, callback);
}

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
    printLog(PRINT_NORMAL, buffer2);
    richPresence->curID = id;
}

#if !RETRO_REV02
void TrackActClear(byte zoneID, byte actID, byte playerID, int score, int rings, int time)
{
    printLog(PRINT_NORMAL, "DUMMY TrackActClear(%d, %d, %d, %d, %d, %d)", zoneID, actID, playerID, score, rings, time);
}
void TrackTAClear(byte zoneID, byte actID, byte playerID, int time)
{
    printLog(PRINT_NORMAL, "DUMMY TrackTAClear(%d, %d, %d, %d)", zoneID, actID, playerID, time);
}
void TrackEnemyDefeat(byte zoneID, byte actID, byte playerID, int entityX, int entityY)
{
    printLog(PRINT_NORMAL, "DUMMY TrackEnemyDefeat(%d, %d, %d, %d, %d)", zoneID, actID, playerID, entityX, entityY);
}
void TrackGameProgress(float percent)
{
    printLog(PRINT_NORMAL, "DUMMY TrackGameProgress() -> %f percent complete", percent * 100);
}
void ClearPrerollErrors() { printLog(PRINT_NORMAL, "DUMMY ClearPrerollErrors()"); }
#else
#define voidToInt(x)   (int)(size_t)(x)
#define voidToFloat(x) *(float *)&(x)

void TryTrackStat(StatInfo *stat)
{
    if (stats->status) {
        printLog(PRINT_NORMAL, "Tracking Stat: %s (%d)", stat->name, stat->statID);

        switch (stat->statID) {
            case 0: {
                char *zoneName = (char *)stat->data[0];
                char *actName = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                int val = voidToInt(stat->data[3]);
                int time = voidToInt(stat->data[4]);
                int rings = voidToInt(stat->data[5]);
                int score = voidToInt(stat->data[6]);
                printLog(PRINT_NORMAL, "DUMMY TrackActClear(%s, %s, %s, %d, %d, %d, %d)", zoneName, actName, playerName, val, score, rings, time);
                break;
            }
            case 1: {
                char *zoneName = (char *)stat->data[0];
                char *actName = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                char *mode = (char *)stat->data[3];
                int time = voidToInt(stat->data[4]);
                printLog(PRINT_NORMAL, "DUMMY TrackTAClear(%s, %s, %s, %s, %d)", zoneName, actName, playerName, mode, time);
                break;
            }
            case 2: {
                char *zoneName = (char *)stat->data[0];
                char *actName = (char *)stat->data[1];
                char *playerName = (char *)stat->data[2];
                bool32 encore = voidToInt(stat->data[3]);
                int enemyX = voidToInt(stat->data[4]);
                int enemyY = voidToInt(stat->data[5]);
                printLog(PRINT_NORMAL, "DUMMY TrackEnemyDefeat(%s, %s, %s, %s, %d, %d)", zoneName, actName, playerName, encore ? "true" : "false",
                         enemyX, enemyY);
                break;
            }
            case 3: printLog(PRINT_NORMAL, "DUMMY TrackGameProgress() -> %f percent complete", voidToFloat(stat->data[0]) * 100); break;
        }
    }
    else {
        printLog(PRINT_NORMAL, "Track stat SKIPPED. Stats are disabled.");
    }
}
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
        val->value = userCore->values[i];
        val->min   = 0;

        if (i == 2) {
            val->type       = 2;
            val->valByteCnt = 1;
            val->max        = REGION_EU;
        }
        else if (i == 3) {
            val->type       = 2;
            val->valByteCnt = 1;
            val->max        = LANGUAGE_TC;
        }
        else {
            val->type       = 0;
            val->valByteCnt = 4;
            val->max        = 1;
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

bool32 TryLoadUserFile(const char *filename, void *buffer, uint bufSize, int (*callback)(int))
{
    bool32 success = false;
    memset(buffer, 0, bufSize);
#if RETRO_REV02
    if (!userStorage->noSaveActive) {
#endif
        success = LoadUserFile(filename, buffer, bufSize);

        if (bufSize >= 4) {
            byte *bufTest       = (byte *)buffer;
            //quick and dirty zlib check
            if (bufTest[0] == 0x78 && ((bufTest[1] == 0x01 && bufTest[2] == 0xED) || bufTest[1] == 0x9C)) {
                uint destLen = bufSize;

                byte *compData = NULL;
                AllocateStorage(bufSize, (void **)&compData, DATASET_TMP, false);
                memcpy(compData, buffer, bufSize);

                uncompress((Bytef *)buffer, (uLongf *)&destLen, compData, bufSize);

                RemoveStorageEntry((void **)&compData);
            }
        }

        if (callback)
            callback(STATUS_OK);
#if RETRO_REV02
    }
    else {
        char buffer[0x100];
        sprintf(buffer, "TryLoadUserFile(%s, %p, %u, %p) failing due to noSave", filename, buffer, bufSize, callback);

        if (callback)
            callback(STATUS_ERROR);
    }
#endif

    return success;
}
bool32 TrySaveUserFile(const char *filename, void *buffer, uint bufSize, int (*callback)(int), bool32 compressData)
{
    bool32 success = false;
#if RETRO_REV02
    if (!userStorage->noSaveActive) {
#endif
        if (compressData) {
            int *cbuf = NULL; 
            AllocateStorage(bufSize, (void **)&cbuf, DATASET_TMP, false);

            long long clen = bufSize;
            compress((Bytef *)cbuf, (uLongf *)&clen, (Bytef *)buffer, (uLong)bufSize);

            success = SaveUserFile(filename, cbuf, clen);

            RemoveStorageEntry((void **)&cbuf);
        }
        else {
            success = SaveUserFile(filename, buffer, bufSize);
        }

        if (callback)
            callback(STATUS_OK);
#if RETRO_REV02
    }
    else {
        char buffer[0x100];
        sprintf(buffer, "TrySaveUserFile(%s, %p, %u, %p, %s) failing due to noSave", filename, buffer, bufSize, callback,
                compressData ? "true" : "false");

        if (callback)
            callback(STATUS_ERROR);
    }
#endif

    return success;
}
bool32 TryDeleteUserFile(const char *filename, int (*callback)(int))
{
#if RETRO_REV02
    if (!userStorage->noSaveActive) {
#endif
        DeleteUserFile(filename);

        if (callback)
            callback(STATUS_OK);
#if RETRO_REV02
    }
    else {
        char buffer[0x100];
        sprintf(buffer, "TryDeleteUserFile(%s, %p) failing due to noSave", filename, callback);

        if (callback)
            callback(STATUS_ERROR);
    }
#endif

    return false;
}

void (*userFileCallback)();
void (*userFileCallback2)();
char userFileDir[0x100];

bool32 LoadUserFile(const char *filename, void *buffer, uint bufSize)
{
    if (userFileCallback)
        userFileCallback();
    int len = (int)strlen(userFileDir);
    sprintf(userFileDir, "%s", filename);
    if (len >= 0x400) {
        // oh shit
    }
    printLog(PRINT_NORMAL, "Attempting to load user file: %s", userFileDir);

    FileIO *file = fOpen(userFileDir, "rb");
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
        printLog(PRINT_NORMAL, "Nope!");
    }
    return false;
}
bool32 SaveUserFile(const char *filename, void *buffer, uint bufSize)
{
    if (userFileCallback)
        userFileCallback();
    int len = (int)strlen(userFileDir);
    sprintf(userFileDir, "%s", filename);
    if (len >= 0x400) {
        // oh shit
    }
    printLog(PRINT_NORMAL, "Attempting to save user file: %s", userFileDir);

    FileIO *file = fOpen(userFileDir, "wb");
    if (file) {
        fWrite(buffer, 1, bufSize, file);
        fClose(file);

        if (userFileCallback2)
            userFileCallback2();
        return true;
    }
    else {
        if (userFileCallback2)
            userFileCallback2();
        printLog(PRINT_NORMAL, "Nope!");
    }
    return false;
}
bool32 DeleteUserFile(const char *filename)
{
    if (userFileCallback)
        userFileCallback();
    int len = (int)strlen(userFileDir);
    sprintf(userFileDir, "%s", filename);
    if (len >= 0x400) {
        // oh shit
    }
    printLog(PRINT_NORMAL, "Attempting to delete user file: %s", userFileDir);
    int status = remove(userFileDir);

    if (userFileCallback2)
        userFileCallback2();
    return status == 0;
}

#if RETRO_REV02
int LoadDBFromBuffer(UserDB *userDB, byte *buffer)
{
    uint signature = *(uint *)buffer;
    if (signature != 0x80074B1E)
        return 0;
    buffer += sizeof(int);
    buffer += sizeof(int); // used size

    userDB->entryCount = *(ushort *)buffer;
    buffer += sizeof(ushort);

    userDB->columnCount = *buffer;
    buffer++;

    for (int c = 0; c < userDB->columnCount; ++c) {
        userDB->columnTypes[c] = *buffer;
        buffer++;

        sprintf(userDB->columnNames[c], "%s", (char *)buffer);
        buffer += 0x10;

        GenerateCRC(&userDB->columnUUIDs[c], userDB->columnNames[c]);
    }

    for (int r = 0; r < RETRO_USERDB_ENTRY_MAX; ++r) {
        userDB->rows[r].uuid = *(uint *)buffer;
        buffer += sizeof(uint);

        memcpy(&userDB->rows[r].createTime, buffer, sizeof(tm));
        buffer += sizeof(tm);
        memcpy(&userDB->rows[r].changeTime, buffer, sizeof(tm));
        buffer += sizeof(tm);

        for (int c = 0; c < userDB->columnCount; ++c) {
            userDB->rows[r].values[c].size = *buffer;
            buffer++;

            memcpy(&userDB->rows[r].values[c].data, buffer, userDB->rows[r].values[c].size);
            buffer += userDB->rows[r].values[c].size;
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

void SaveDBToBuffer(UserDB *userDB, int totalSize, byte *buffer)
{
    int size = 0;
    if (totalSize >= sizeof(int)) {
        size           = sizeof(int);
        *(int *)buffer = 0x80074B1E; // signature
        buffer += sizeof(int);
    }
    if (size + sizeof(int) <= totalSize) {
        size += sizeof(int);
        *(int *)buffer = (int)GetUserDBWriteSize(userDB); // used size
        buffer += sizeof(int);
    }
    if (size + sizeof(ushort) <= totalSize) {
        size += sizeof(ushort);
        *(ushort *)buffer = userDB->entryCount;
        buffer += sizeof(ushort);
    }
    if (size + sizeof(byte) <= totalSize) {
        ++size;
        *buffer++ = userDB->columnCount;
    }

    for (int c = 0; c < userDB->columnCount; ++c) {
        if (size + sizeof(byte) <= totalSize) {
            ++size;
            *buffer++ = (byte)userDB->columnTypes[c];
        }
        if (size + 0x10 <= totalSize) {
            memset(buffer, 0, 0x10 * sizeof(byte));
            sprintf((char *)buffer, "%s", userDB->columnNames[c]);
            size += 0x10;
            buffer += 0x10;
        }
    }

    for (int r = 0; r < RETRO_USERDB_ENTRY_MAX; ++r) {
        if (size + sizeof(uint) <= totalSize) {
            size += sizeof(uint);
            *(uint *)buffer = userDB->rows[r].uuid;
            buffer += sizeof(uint);
        }
        if (size + sizeof(tm) <= totalSize) {
            memcpy(buffer, &userDB->rows[r].createTime, sizeof(tm));
            size += sizeof(tm);
            buffer += sizeof(tm);
        }
        if (size + sizeof(tm) <= totalSize) {
            memcpy(buffer, &userDB->rows[r].changeTime, sizeof(tm));
            size += sizeof(tm);
            buffer += sizeof(tm);
        }

        for (int c = 0; c < userDB->columnCount; ++c) {
            if (size + sizeof(byte) <= totalSize) {
                ++size;
                *buffer++ = (byte)userDB->rows[r].values[c].size;
            }
            if (userDB->rows[r].values[c].size + size <= totalSize) {
                memcpy(buffer, userDB->rows[r].values[c].data, userDB->rows[r].values[c].size);
                size += userDB->rows[r].values[c].size;
                buffer += userDB->rows[r].values[c].size;
            }
        }
    }

    if (size < totalSize)
        memset(buffer, 0, totalSize - size);
}

int UserDBLoadCB(ushort tableID, int status)
{
    if (status == STATUS_OK) {
        int result = LoadDBFromBuffer(&userDBStorage->userDB[tableID], (byte *)userDBStorage->readBuffer[tableID]);
        if (result) {
            // sub_5EC5F0(&userDBStorage->userDB[v6].parent);
        }
    }
    else {
        ClearUserDB(tableID);
    }
    RemoveStorageEntry((void **)&userDBStorage->readBuffer[tableID]);

    if (userDBStorage->callbacks[tableID]) {
        userDBStorage->callbacks[tableID](status);
        userDBStorage->callbacks[tableID] = NULL;
        return 1;
    }
    return 0;
}

int UserDBSaveCB(ushort tableID, int status)
{
    RemoveStorageEntry((void **)&userDBStorage->writeBuffer[tableID]);
    if (status != STATUS_OK)
        userDBStorage->userDB[tableID].valid = false;

    if (userDBStorage->callbacks[tableID]) {
        userDBStorage->callbacks[tableID](status);
        userDBStorage->callbacks[tableID] = NULL;
        return 1;
    }
    return 0;
}

ushort LoadUserDB(const char *filename, void (*callback)(int))
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
            callback(STATUS_ERROR);
        return -1;
    }

    UserDB *userDB = &userDBStorage->userDB[tableID];
    userDB->loaded = true;
    userDB->name   = filename;
    userDB->uuid   = uuid;
    AllocateStorage(sizeof(UserDB), (void **)&userDBStorage->readBuffer[tableID], DATASET_TMP, true);
    userDBStorage->userLoadCB[tableID] = UserDBLoadCB;
    userDBStorage->callbacks[tableID]  = callback;
    TryLoadUserFile(filename, userDBStorage->readBuffer[tableID], sizeof(UserDB), userDBStorage->loadCallback[tableID]);
    return tableID;
}
bool32 SaveUserDB(ushort tableID, void (*callback)(int))
{
    UserDB *userDB = &userDBStorage->userDB[tableID];

    bool32 success = false;
    if (userDB->active) {
        int totalSize = (int)GetUserDBWriteSize(userDB);
        AllocateStorage(totalSize, (void **)&userDBStorage->writeBuffer[tableID], DATASET_TMP, true);
        SaveDBToBuffer(userDB, totalSize, (byte *)userDBStorage->writeBuffer[tableID]);
        userDBStorage->userSaveCB[tableID] = UserDBSaveCB;
        userDBStorage->callbacks[tableID]  = callback;
        success = TrySaveUserFile(userDB->name, userDBStorage->writeBuffer[tableID], totalSize, userDBStorage->saveCallback[tableID], true);
    }
    else {
        if (callback)
            callback(STATUS_ERROR);
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
                settingsChanged = true;
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
            // settingsStorage.mouseX        = 0;
            // settingsStorage.mouseY        = 0;
            // settingsStorage.field_8       = 0;
            // settingsStorage.field_C       = 0;
            break;
        case SETTINGS_RELOAD:
            settingsChanged = true;

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
            // 0                    = settingsStorage.mouseX;
            // 0                    = settingsStorage.mouseY;
            // 0                    = settingsStorage.field_8;
            // 0                    = settingsStorage.field_C;
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

    char pathBuffer[0x100];
    sprintf(pathBuffer, "%sSettings.ini", userFileDir);

    dictionary *ini = iniparser_load(pathBuffer);

    if (ini) {
#if RETRO_REV02
        curSKU.language = iniparser_getint(ini, "Game:language", LANGUAGE_EN);
#else
        gameVerInfo.language = (int)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);
#endif

        // Consoles load the entire file and buffer it, while pc just io's the file when needed
        bool32 useBuffer = !((RETRO_PLATFORM == RETRO_WIN) || (RETRO_PLATFORM == RETRO_OSX));
        if (CheckDataFile(iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"), 0, useBuffer))
            engine.devMenu = iniparser_getboolean(ini, "Game:devMenu", false);
        else
            engine.devMenu = true;

#if !RETRO_USE_ORIGINAL_CODE
        sprintf(gameLogicName, "%s", iniparser_getstring(ini, "Game:gameLogic", "Game"));

        engine.confirmFlip = iniparser_getboolean(ini, "Game:confirmButtonFlip", false);
        engine.XYFlip      = iniparser_getboolean(ini, "Game:xyButtonFlip", false);
#else
        sprintf(gameLogicName, "Game"));
        engine.confirmFlip = false;
        engine.XYFlip = false;
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
              SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_RETURN, SDL_SCANCODE_LSHIFT },
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
    // only done on windows and "dev", consoles use "options.bin"
#if RETRO_REV02
    if (curSKU.platform != PLATFORM_PC && curSKU.platform != PLATFORM_DEV)
        return;
#else
    if (gameVerInfo.platform != PLATFORM_PC && gameVerInfo.platform != PLATFORM_DEV)
        return;
#endif

    if (settingsChanged || writeToFile) {
        char pathBuffer[0x100];
        sprintf(pathBuffer, "%sSettings.ini", userFileDir);

        dictionary *ini = iniparser_load(pathBuffer);
        FileIO *file    = fOpen(pathBuffer, "wb");
        writeText(file, "; Retro Engine Config File\n\n");
        writeText(file, "[Game]\n");
        if (ini) {
            if (strcmp(iniparser_getstring(ini, "Game:dataFile", "optionNotFound"), "optionNotFound") != 0) {
                writeText(file, "dataFile=%s\n", iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"));
            }

            if (strcmp(iniparser_getstring(ini, "Game:devMenu", "optionNotFound"), "optionNotFound") != 0)
                writeText(file, "devMenu=%s\n", (engine.devMenu ? "y" : "n"));

#if !RETRO_USE_ORIGINAL_CODE
            if (strcmp(iniparser_getstring(ini, "Game:gameLogic", "optionNotFound"), "optionNotFound") != 0)
                writeText(file, "gameLogic=%s\n", iniparser_getstring(ini, "Game:gameLogic", "Game"));

            if (strcmp(iniparser_getstring(ini, "Game:confirmButtonFlip", "optionNotFound"), "optionNotFound") != 0)
                writeText(file, "confirmButtonFlip=%s\n", (engine.confirmFlip ? "y" : "n"));
            if (strcmp(iniparser_getstring(ini, "Game:xyButtonFlip", "optionNotFound"), "optionNotFound") != 0)
                writeText(file, "xyButtonFlip=%s\n", (engine.XYFlip ? "y" : "n"));
#endif
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
            if (strcmp(iniparser_getstring(ini, "Video:pixWidth", "optionNotFound"), "optionNotFound") == 0)
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

#if RETRO_VER_EGS || RETRO_USE_DUMMY_ACHIEVEMENTS
bool32 achievementsEnabled = true;
ushort achievementAniFrames[2];
Animator achievementAnimator[2];
TextInfo achievementText[2];
int achievementTextWidth[2];
int achievementID             = 0;
int achievementsDelay         = 0;
int achievementsDrawn         = 0;
int achievementStrW           = 0;
int achievementStrX           = 0;
bool32 achievementsLoaded     = false;
bool32 achievementDrawFlag    = false;
bool32 achievementUnknownFlag = false;

void LoadAchievementAssets()
{
    if (achievementsEnabled) {
        if (achievements) {
            if (achievements->CheckAchievementsEnabled()) {
                if (achievements->Unknown8()) {
                    if (achievementUnknownFlag) {
                        achievementUnknownFlag = false;
                        achievementID          = 0;
                        achievementDrawFlag    = false;
                        achievementsDelay      = 0;
                        achievementsDrawn      = 0;
                    }
                    else {
                        if (achievementID)
                            achievementsDelay = 180;
                    }

                    achievementsLoaded = !(CheckSceneFolder("Logos") || CheckSceneFolder("Title"));

                    if (achievementsLoaded) {
                        achievementAniFrames[0] = LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
                        SetSpriteAnimation(achievementAniFrames[0], 0, &achievementAnimator[0], true, 0);

                        achievementAniFrames[1] = LoadSpriteAnimation("UI/SmallFont.bin", SCOPE_STAGE);
                        SetSpriteAnimation(achievementAniFrames[1], 0, &achievementAnimator[1], true, 0);
                    }
                }
                else {
                    achievementsLoaded = false;
                }
            }
        }
    }
}
void ProcessAchievements()
{
    if (achievementsEnabled && achievements && achievements->CheckAchievementsEnabled()) {
        if (achievementsLoaded) {
            if (!achievementID) {
                achievementID = achievements->GetNextAchievementID();
                if (achievementID) {
                    achievementDrawFlag = true;
                    achievementsDelay   = 180;
                    achievementsDrawn   = false;
                    achievements->RemoveLastAchievementID();

                    TextInfo buffer;
                    CopyString(&achievementText[0], achievements->GetAchievementText(&buffer));
                    CopyString(&achievementText[1], achievements->GetAchievementName(&buffer, achievementID));
                    if (curSKU.language == LANGUAGE_JP) {
                        achievementTextWidth[0] = 13 * achievementText[0].textLength;
                        achievementTextWidth[1] = 13 * achievementText[1].textLength;
                    }
                    else {
                        achievementTextWidth[0] =
                            GetStringWidth(achievementAniFrames[0], 0, &achievementText[0], 0, achievementText[0].textLength, 0);
                        achievementTextWidth[1] =
                            GetStringWidth(achievementAniFrames[1], 0, &achievementText[1], 0, achievementText[1].textLength, 0);
                    }

                    achievementStrW = maxVal(achievementTextWidth[0], achievementTextWidth[1]) + 16;
                    achievementStrX = achievementTextWidth[1] > achievementTextWidth[0] ? 20 : 0;
                }
            }

            if (achievementsDrawn) {
                if (!--achievementsDelay) {
                    achievementID       = 0;
                    achievementDrawFlag = false;
                }
            }
        }
    }
}
void DrawAchievements()
{
    if (achievementsEnabled && achievements && achievements->CheckAchievementsEnabled()) {
        if (achievementsLoaded && achievementDrawFlag && achievementID) {
            Vector2 drawPos;

            TextInfo buffer;
            CopyString(&achievementText[0], achievements->GetAchievementText(&buffer));
            CopyString(&achievementText[1], achievements->GetAchievementName(&buffer, achievementID));

            int drawX = achievementStrX + currentScreen->width - achievementStrW;
            DrawRectangle(drawX, currentScreen->height - 40, achievementStrW - achievementStrX, 40, 0xFF107C, 255, INK_NONE, true);

            Vector2 vertices[3];
            vertices[0].x = (drawX - 40) << 16;
            vertices[1].y = (currentScreen->height - 40) << 16;
            vertices[0].y = currentScreen->height << 16;
            vertices[1].x = drawX << 16;
            vertices[2].x = drawX << 16;
            vertices[2].y = currentScreen->height << 16;
            DrawFace(vertices, 3, 255, 16, 124, 255, INK_NONE);

            drawPos.x = (drawX - achievementStrX + achievementStrW - 8) << 16;
            drawPos.y = vertices[1].y + 0xA0000;
            SetSpriteString(achievementAniFrames[0], 0, &achievementText[0]);
            DrawText(&achievementAnimator[0], &drawPos, &achievementText[0], 0, achievementText[0].textLength, ALIGN_CENTER, 0, 0, NULL, true);

            vertices[1].y += 0x1C0000;

            drawPos.x = (drawX - achievementStrX + achievementStrW - 8) << 16;
            drawPos.y = vertices[1].y;
            SetSpriteString(achievementAniFrames[1], 0, &achievementText[1]);
            DrawText(&achievementAnimator[1], &drawPos, &achievementText[1], 0, achievementText[1].textLength, ALIGN_CENTER, 0, 0, NULL, true);

            achievementsDrawn = true;
        }
    }
}

#endif
