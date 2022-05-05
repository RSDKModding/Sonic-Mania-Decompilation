#include "RSDK/Core/RetroEngine.hpp"
#include "zlib/zlib.h"

#if RETRO_REV02
RSDK::SKU::UserCore *RSDK::SKU::userCore = NULL;
#endif

bool32 RSDK::settingsChanged = false;
RSDK::SettingsStorage RSDK::gameSettings;
RSDK::SettingsStorage RSDK::settingsStorage;

void RSDK::SKU::InitUserData()
{
    int32 language     = GetAPIValue(GetAPIValueID("SYSTEM_LANGUAGE", 0));
    int32 region       = GetAPIValue(GetAPIValueID("SYSTEM_REGION", 0));
    int32 platform     = GetAPIValue(GetAPIValueID("SYSTEM_PLATFORM", 0));
    engine.confirmFlip = GetAPIValue(GetAPIValueID("SYSTEM_CONFIRM_FLIP", 0));

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX || RETRO_PLATFORM == RETRO_UWP                       \
    || RETRO_PLATFORM == RETRO_iOS
    // platform = PLATFORM_PC;
#elif RETRO_PLATFORM == RETRO_PS4
    platform             = PLATFORM_PS4;
#elif RETRO_PLATFORM == RETRO_XB1
    platform = PLATFORM_XB1;
#elif RETRO_PLATFORM == RETRO_SWITCH || RETRO_PLATFORM == RETRO_ANDROID
    platform = PLATFORM_SWITCH;
#endif

#if RETRO_REV02
    curSKU.platform = platform;
    curSKU.language = language;
    curSKU.region   = region;
#else
    gameVerInfo.platform = platform;
    gameVerInfo.language = language;
    gameVerInfo.region   = region;
#endif

    int32 value = GetAPIValue(GetAPIValueID("SYSTEM_PLATFORM", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_REGION", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_LANGUAGE", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_CONFIRM_FLIP", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_LEADERBOARD_LOAD_TIME", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_LEADERBOARD_STATUS", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_AUTH_STATUS", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_STATUS", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_AUTH_TIME", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_INIT_TIME", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_LOAD_TIME", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_SAVE_TIME", 0));
    value       = GetAPIValue(GetAPIValueID("SYSTEM_USERSTORAGE_STORAGE_DELETE_TIME", 0));

#if RETRO_REV02
    if (dummyCore)
        delete dummyCore;
    dummyCore = InitDummyCore();

    // Initalize platform-specific subsystems here

    // Examples
    if (false)
        userCore = InitSteamCore();
    if (false)
        userCore = InitEOSCore();
    if (false)
        userCore = InitNXCore();

    if (!userCore) // no platform core, so default to dummy funcs
        userCore = dummyCore;
    else if (dummyCore) {
        delete dummyCore;
        dummyCore = nullptr;
    }

    if (!userDBStorage)
        userDBStorage = (UserDBStorage *)malloc(sizeof(UserDBStorage));

    InitUserStorageDB(userDBStorage);
#endif

    // Add achievements
    achievementList.clear();
    achievementStack.clear();
    RegisterAchievement("ACH_GOLD_MEDAL", "No Way? No Way!", "Collect gold medallions in Blue Spheres Bonus stage");
    RegisterAchievement("ACH_SILVER_MEDAL", "Full Medal Jacket", "Collect silver medallions in Blue Spheres Bonus stage");
    RegisterAchievement("ACH_EMERALDS", "Magnificent Seven", "Collect all seven Chaos Emeralds");
    RegisterAchievement("ACH_GAME_CLEARED", "See You Next Game", "Achieve any ending");
    RegisterAchievement("ACH_STARPOST", "Superstar", "Spin the Star Post!");
    RegisterAchievement("ACH_SIGNPOST", "That's a Two-fer", "Find the hidden item boxes at the end of the Zone");
    RegisterAchievement("ACH_GHZ", "Now It Can't Hurt You Anymore", "What would happen if you cross a bridge with a fire shield?");
    RegisterAchievement("ACH_CPZ", "Triple Trouble", "Try for a 3 chain combo!");
    RegisterAchievement("ACH_SPZ", "The Most Famous Hedgehog in the World", "Have your photos taken in Studiopolis Zone");
    RegisterAchievement("ACH_FBZ", "Window Shopping", "Let the wind take you through");
    RegisterAchievement("ACH_PGZ", "Crate Expectations", "Wreak havoc at the propaganda factory");
    RegisterAchievement("ACH_SSZ", "King of Speed", "Get through Stardust Speedway Zone as quickly as possible");
    RegisterAchievement("ACH_HCZ", "Boat Enthusiast", "Try pushing a barrel to see how far it goes");
    RegisterAchievement("ACH_MSZ", "The Password is \"Special Stage\"", "Try pushing a barrel to see how far it goes");
    RegisterAchievement("ACH_OOZ", "Secret Sub", "You might have to submerge to find it");
    RegisterAchievement("ACH_LRZ", "Without a Trace", "Barrel through the lava, don't let anything stop you");
    RegisterAchievement("ACH_MMZ", "Collect 'Em All", "Gotta gacha 'em all");
    RegisterAchievement("ACH_TMZ", "Professional Hedgehog", "That elusive perfect run, only a professional can achieve");

    int achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int));
    bool32 loaded = false;
#if RETRO_REV02
    loaded = userStorage->TryLoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int), NULL);
#else
    loaded               = LoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int));
#endif
    for (int i = 0; i < (int)achievementList.size(); ++i) {
        achievementList[i].achieved = achievementsRAM[i];
    }

    leaderboardList.clear();
    int leaderboardsRAM[0x200];
    memset(leaderboardsRAM, 0, 0x200 * sizeof(int));
#if RETRO_REV02
    loaded = userStorage->TryLoadUserFile("Leaderboards.bin", leaderboardsRAM, sizeof(leaderboardsRAM), NULL);
#else
    loaded               = LoadUserFile("Leaderboards.bin", leaderboardsRAM, sizeof(leaderboardsRAM));
#endif
    if (loaded) {
        int32 pos = 1;
        for (int i = 0; i < leaderboardsRAM[0]; ++i) {
            leaderboardList.push_back(LeaderboardInfo());
            int len = leaderboardsRAM[pos++];
            memcpy(leaderboardList[i].name, &leaderboardsRAM[pos], len);
            int size = (len / 4) + (4 - ((len % 4) ? (len % 4) : 4));
            pos += size;
            leaderboardList[i].score = leaderboardsRAM[pos++];
        }
    }

    statList.clear();
    uint8 *statsRAM = new uint8[0x1000 * sizeof(StatInfo)];
#if RETRO_REV02
    loaded = userStorage->TryLoadUserFile("Stats.bin", statsRAM, 0x1000 * sizeof(StatInfo), NULL);
#else
    loaded               = LoadUserFile("Stats.bin", statsRAM, 0x1000 * sizeof(StatInfo));
#endif
    if (loaded) {
        uint32 statCount = *((uint32 *)statsRAM);
        int32 pos        = sizeof(uint32);

        for (int i = 0; i < statCount; ++i) {
            StatInfo stat;
            memcpy(stat.data, &statsRAM[pos], sizeof(stat.data));
            pos += sizeof(stat.data);
        }
    }
    delete[] statsRAM;
}
void RSDK::SKU::releaseUserData()
{
    saveUserData();

#if RETRO_REV02

    if (achievements)
        delete achievements;
    achievements = nullptr;

    if (leaderboards)
        delete leaderboards;
    leaderboards = nullptr;

    if (richPresence)
        delete richPresence;
    richPresence = nullptr;

    if (stats)
        delete stats;
    stats = nullptr;

    if (userStorage)
        delete userStorage;
    userStorage = nullptr;

    ReleaseUserStorageDB(userDBStorage);

    if (userDBStorage)
        free(userDBStorage);
    userDBStorage = nullptr;

    if (userCore) {
        userCore->Shutdown();
        delete userCore;
    }

    dummyCore = nullptr;
    userCore  = nullptr;
#endif
}

void RSDK::SKU::saveUserData()
{
    int achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int));
    for (int i = 0; i < (int)achievementList.size(); ++i) {
        achievementsRAM[i] = achievementList[i].achieved;
    }
#if RETRO_REV02
    userStorage->TrySaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int), NULL, false);
#else
    SaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int));
#endif

    int32 leaderboardsRAM[0x100];
    memset(leaderboardsRAM, 0, 0x100 * sizeof(int32));
    leaderboardsRAM[0] = (int)leaderboardList.size();
    int32 pos          = 1;
    for (int32 i = 0; i < (int)leaderboardList.size(); ++i) {
        int32 len              = (int32)strlen(leaderboardList[i].name);
        leaderboardsRAM[pos++] = len;
        memcpy(&leaderboardsRAM[pos], leaderboardList[i].name, len);
        int size = (len / 4) + (4 - ((len % 4) ? (len % 4) : 4));
        pos += size;
        leaderboardsRAM[pos++] = leaderboardList[i].score;
    }
#if RETRO_REV02
    userStorage->TrySaveUserFile("Leaderboards.bin", leaderboardsRAM, sizeof(leaderboardsRAM), NULL, false);
#else
    SaveUserFile("Leaderboards.bin", leaderboardsRAM, sizeof(leaderboardsRAM));
#endif

    uint8 *statsRAM = new uint8[0x100 * sizeof(StatInfo)];
    memset(statsRAM, 0, 0x100 * sizeof(StatInfo));

    ((uint32 *)statsRAM)[0] = (int)statList.size();

    pos = sizeof(uint32);
    for (int i = 0; i < (int)statList.size(); ++i) {
        memcpy(&statsRAM[pos], statList[i].data, sizeof(statList[i].data));
        pos += sizeof(statList[i].data);
    }
#if RETRO_REV02
    userStorage->TrySaveUserFile("Stats.bin", statsRAM, sizeof(0x1000 * sizeof(StatInfo)), NULL, false);
#else
    SaveUserFile("Stats.bin", statsRAM, sizeof(0x1000 * sizeof(StatInfo)));
#endif
    delete[] statsRAM;
}

// Found this in Switch 1.00, doesn't seem to show up in rev02 variants but its neat nonetheless
bool32 RSDK::SKU::GetXYButtonFlip() { return engine.XYFlip; }

#if RETRO_REV02
void RSDK::SKU::UserCore::StageLoad()
{
    achievements->StageLoad();
    leaderboards->StageLoad();
    richPresence->StageLoad();
    stats->StageLoad();
    userStorage->StageLoad();
}
void RSDK::SKU::UserCore::FrameInit()
{
    achievements->FrameInit();
    leaderboards->FrameInit();
    richPresence->FrameInit();
    stats->FrameInit();
    userStorage->FrameInit();
}
void RSDK::SKU::UserCore::OnUnknownEvent()
{
    achievements->OnUnknownEvent();
    leaderboards->OnUnknownEvent();
    richPresence->OnUnknownEvent();
    stats->OnUnknownEvent();
    userStorage->OnUnknownEvent();
}
#else
bool32 RSDK::SKU::GetConfirmButtonFlip() { return engine.confirmFlip; }
void RSDK::SKU::LaunchManual()
{
    // LaunchManual() just opens the mania manual URL, thats it
#if RETRO_USING_SDL2
    // SDL_OpenURL("http://www.sonicthehedgehog.com/mania/manual");
    PrintLog(PRINT_NORMAL, "DUMMY LaunchManual()");
#else
    PrintLog(PRINT_NORMAL, "EMPTY LaunchManual()");
#endif
}
void RSDK::SKU::ExitGame() { engine.running = false; }

int RSDK::SKU::GetDefaultGamepadType()
{
#if RETRO_REV02
    int32 platform = curSKU.platform = PLATFORM_SWITCH;
#else
    int32 platform = gameVerInfo.platform = PLATFORM_SWITCH;
#endif

    switch (platform) {
        case PLATFORM_SWITCH: return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (DEVICE_SWITCH_HANDHELD << 0);
        case PLATFORM_PC:
        case PLATFORM_DEV:
        default: return (DEVICE_API_NONE << 16) | (DEVICE_TYPE_CONTROLLER << 8) | (0 << 0); break;
    }
}

int RSDK::SKU::ShowExtensionOverlay(byte overlay)
{
    switch (overlay) {
        default: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d", overlay); break;
        case 0: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d (Plus Upsell Screen)", overlay); break;
    }
    return 1;
}
#endif

int RSDK::GetSettingsValue(int id)
{
    switch (id) {
        case SETTINGS_WINDOWED: return gameSettings.windowed;
        case SETTINGS_BORDERED: return gameSettings.bordered;
        case SETTINGS_EXCLUSIVEFS: return gameSettings.exclusiveFS;
        case SETTINGS_VSYNC: return gameSettings.vsync;
        case SETTINGS_TRIPLEBUFFERED: return gameSettings.tripleBuffered;
        case SETTINGS_WINDOW_WIDTH: return gameSettings.windowWidth;
        case SETTINGS_WINDOW_HEIGHT: return gameSettings.windowHeight;
        case SETTINGS_FSWIDTH: return gameSettings.fsWidth;
        case SETTINGS_FSHEIGHT: return gameSettings.fsHeight;
        case SETTINGS_REFRESHRATE: return gameSettings.refreshRate;
        case SETTINGS_SHADERSUPPORT: return gameSettings.shaderSupport;
        case SETTINGS_SHADERID: return gameSettings.shaderID;
        case SETTINGS_SCREENCOUNT: return gameSettings.screenCount;
#if RETRO_REV02
        case SETTINGS_DIMTIMER: return gameSettings.dimTimer;
#endif
        case SETTINGS_STREAMSENABLED: return engine.streamsEnabled;
        case SETTINGS_STREAM_VOL: return (int)(engine.streamVolume * 1024.0);
        case SETTINGS_SFX_VOL: return (int)(engine.soundFXVolume * 1024.0);
        case SETTINGS_LANGUAGE:
#if RETRO_REV02
            return SKU::curSKU.language;
#else
            return gameVerInfo.language;
#endif
        case SETTINGS_CHANGED: return settingsChanged;
        default: break;
    }
    return 0;
}

void RSDK::SetSettingsValue(int id, int val)
{
    bool32 bVal = val;
    switch (id) {
        case SETTINGS_WINDOWED:
            if (gameSettings.windowed != bVal) {
                gameSettings.windowed = bVal;
                settingsChanged       = true;
            }
            break;
        case SETTINGS_BORDERED:
            if (gameSettings.bordered != bVal) {
                gameSettings.bordered = bVal;
                settingsChanged       = true;
            }
            break;
        case SETTINGS_EXCLUSIVEFS:
            if (gameSettings.exclusiveFS != bVal) {
                gameSettings.exclusiveFS = bVal;
                settingsChanged          = true;
            }
            break;
        case SETTINGS_VSYNC:
            if (gameSettings.vsync != bVal) {
                gameSettings.vsync = bVal;
                settingsChanged    = true;
            }
            break;
        case SETTINGS_TRIPLEBUFFERED:
            if (gameSettings.tripleBuffered != bVal) {
                gameSettings.tripleBuffered = bVal;
                settingsChanged             = true;
            }
            break;
        case SETTINGS_WINDOW_WIDTH:
            if (gameSettings.windowWidth != val) {
                gameSettings.windowWidth = val;
                settingsChanged          = true;
            }
            break;
        case SETTINGS_WINDOW_HEIGHT:
            if (gameSettings.windowHeight != val) {
                gameSettings.windowHeight = val;
                settingsChanged           = true;
            }
            break;
        case SETTINGS_FSWIDTH: gameSettings.fsWidth = val; break;
        case SETTINGS_FSHEIGHT: gameSettings.fsHeight = val; break;
        case SETTINGS_REFRESHRATE: gameSettings.refreshRate = val; break;
        case SETTINGS_SHADERSUPPORT: gameSettings.shaderSupport = val; break;
        case SETTINGS_SHADERID:
            if (gameSettings.shaderID != val) {
                gameSettings.shaderID = val;
                settingsChanged       = true;
            }
            break;
        case SETTINGS_SCREENCOUNT: gameSettings.screenCount = val; break;
#if RETRO_REV02
        case SETTINGS_DIMTIMER: gameSettings.dimLimit = val; break;
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
            SKU::curSKU.language = val;
#else
            gameVerInfo.language = val;
#endif
            break;
        case SETTINGS_STORE: memcpy(&settingsStorage, &gameSettings, sizeof(gameSettings)); break;
        case SETTINGS_RELOAD:
            settingsChanged = true;
            memcpy(&gameSettings, &settingsStorage, sizeof(settingsStorage));
            break;
        case SETTINGS_CHANGED: settingsChanged = val; break;
        case SETTINGS_WRITE: writeSettings(val); break;
        default: break;
    }
}
#if RETRO_PLATFORM == RETRO_ANDROID
#include <jni.h>
#endif

char buttonNames[18][8] = { "U", "D", "L", "R", "START", "SELECT", "LSTICK", "RSTICK", "L1", "R1", "C", "Z", "A", "B", "X", "Y", "L2", "R2" };

void RSDK::readSettings()
{
    gameSettings.screenCount = 1;
    gameSettings.pixHeight   = SCREEN_YSIZE;
    gameSettings.windowState = WINDOWSTATE_UNINITIALIZED;

    int platform = PLATFORM_DEV;
#if RETRO_REV02
    platform = SKU::curSKU.platform;
#else
    platform = gameVerInfo.platform;
#endif

    // Consoles load the entire file and buffer it, while pc just io's the file when needed
    bool32 useBuffer = !(platform == PLATFORM_PC || platform == PLATFORM_DEV);
    char pathBuffer[0x100];
    sprintf(pathBuffer, "%sSettings.ini", SKU::userFileDir);

    dictionary *ini = iniparser_load(pathBuffer);

    int defKeyMaps[PLAYER_COUNT + 1][12] = { { VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN,
                                               VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN },
                                             { VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_A, VK_S, VK_D, VK_Q, VK_W, VK_E, VK_RETURN, VK_TAB },
                                             { VK_NUMPAD8, VK_NUMPAD5, VK_NUMPAD4, VK_NUMPAD6, VK_J, VK_J, VK_UNKNOWN, VK_U, VK_I, VK_UNKNOWN,
                                               VK_OEM_4, VK_OEM_6 },
                                             { VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN,
                                               VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN },
                                             { VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN,
                                               VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN, VK_UNKNOWN } };

    if (ini) {
#if RETRO_REV02
        SKU::curSKU.language = iniparser_getint(ini, "Game:language", LANGUAGE_EN);
#else
        gameVerInfo.language = (int)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);
#endif

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

        gameSettings.windowed       = iniparser_getboolean(ini, "Video:windowed", true);
        gameSettings.bordered       = iniparser_getboolean(ini, "Video:border", true);
        gameSettings.exclusiveFS    = iniparser_getboolean(ini, "Video:exclusiveFS", false);
        gameSettings.vsync          = iniparser_getboolean(ini, "Video:vsync", false);
        gameSettings.tripleBuffered = iniparser_getboolean(ini, "Video:tripleBuffering", false);

        gameSettings.pixWidth = iniparser_getint(ini, "Video:pixWidth", DEFAULT_SCREEN_XSIZE);

        gameSettings.windowWidth   = iniparser_getint(ini, "Video:winWidth", DEFAULT_SCREEN_XSIZE);
        gameSettings.windowHeight  = iniparser_getint(ini, "Video:winHeight", SCREEN_YSIZE);
        gameSettings.fsWidth       = iniparser_getint(ini, "Video:fsWidth", 0);
        gameSettings.fsHeight      = iniparser_getint(ini, "Video:fsHeight", 0);
        gameSettings.refreshRate   = iniparser_getint(ini, "Video:refreshRate", 60);
        gameSettings.shaderSupport = iniparser_getboolean(ini, "Video:shaderSupport", true);
        gameSettings.shaderID      = iniparser_getint(ini, "Video:screenShader", 0);

        engine.streamsEnabled = iniparser_getboolean(ini, "Audio:streamsEnabled", true);
        engine.streamVolume   = iniparser_getdouble(ini, "Audio:streamVolume", 0.8);
        engine.soundFXVolume  = iniparser_getdouble(ini, "Audio:sfxVolume", 1.0);

        for (int i = 1; i <= PLAYER_COUNT; ++i) {
            char buffer[0x30];

            sprintf(buffer, "Keyboard Map %d:up", i);
            controller[i].keyUp.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][0]);
            sprintf(buffer, "Keyboard Map %d:down", i);
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

        RSDK::AllocateStorage(sizeof(GamePadMappings) * gamePadCount, (void **)&gamePadMappings, RSDK::DATASET_STG, true);

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
        gameSettings.windowed       = true;
        gameSettings.bordered       = false;
        gameSettings.exclusiveFS    = true;
        gameSettings.vsync          = true;
        gameSettings.tripleBuffered = false;
        gameSettings.shaderSupport  = true;
        gameSettings.pixWidth       = 424;
        gameSettings.fsWidth        = 0;
        gameSettings.windowWidth    = gameSettings.pixWidth * 1;
        gameSettings.windowHeight   = SCREEN_YSIZE * 1;
        gameSettings.fsHeight       = 0;
        gameSettings.refreshRate    = 60;
        gameSettings.shaderID       = SHADER_NONE;
        engine.streamsEnabled       = true;
        engine.streamVolume         = 1.0f;
        engine.soundFXVolume        = 1.0f;
        engine.devMenu              = false;

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
        engine.devMenu = CheckDataFile("Data.rsdk", 0, useBuffer);
    }
}

void RSDK::writeSettings(bool32 writeToFile)
{
    // only done on windows and "dev", consoles use "options.bin"
#if RETRO_REV02
    if (SKU::curSKU.platform != PLATFORM_PC && SKU::curSKU.platform != PLATFORM_DEV)
        return;
#else
    if (gameVerInfo.platform != PLATFORM_PC && gameVerInfo.platform != PLATFORM_DEV)
        return;
#endif

    if (settingsChanged || writeToFile) {
        char pathBuffer[0x100];
        sprintf(pathBuffer, "%sSettings.ini", SKU::userFileDir);

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
        writeText(file, "language=%d\n", SKU::curSKU.language);
#else
        writeText(file, "language=%d\n", gameVerInfo.language);
#endif
        writeText(file, "\n[Video]\n");
        writeText(file, "; NB: Fullscreen Resolution can be explicitly set with values fsWidth and fsHeight\n");
        writeText(file, "; If not listed, fullscreen will just use the desktop resolution\n");
        writeText(file, "windowed=%s\n", (gameSettings.windowed ? "y" : "n"));
        writeText(file, "border=%s\n", (gameSettings.bordered ? "y" : "n"));
        writeText(file, "exclusiveFS=%s\n", (gameSettings.exclusiveFS ? "y" : "n"));
        writeText(file, "vsync=%s\n", (gameSettings.vsync ? "y" : "n"));
        writeText(file, "tripleBuffering=%s\n", (gameSettings.tripleBuffered ? "y" : "n"));
        if (ini) {
            if (strcmp(iniparser_getstring(ini, "Video:pixWidth", "optionNotFound"), "optionNotFound") == 0)
                writeText(file, "pixWidth=%d\n", gameSettings.pixWidth);
        }
        writeText(file, "winWidth=%d\n", gameSettings.windowWidth);
        writeText(file, "winHeight=%d\n", gameSettings.windowHeight);
        if (gameSettings.fsWidth > 0)
            writeText(file, "fsWidth=%d\n", gameSettings.fsWidth);
        if (gameSettings.fsHeight > 0)
            writeText(file, "fsHeight=%d\n", gameSettings.fsHeight);
        if (gameSettings.refreshRate > 0)
            writeText(file, "refreshRate=%d\n", gameSettings.refreshRate);

        writeText(file, "shaderSupport=%s\n", (gameSettings.shaderSupport ? "y" : "n"));
        writeText(file, "screenShader=%d\n", gameSettings.shaderID);
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

                    if (m == 18)
                        writeText(file, "?,");
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
