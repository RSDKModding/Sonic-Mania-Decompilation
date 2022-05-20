#include "RSDK/Core/RetroEngine.hpp"
#include "zlib/zlib.h"

using namespace RSDK;

#if RETRO_REV02
SKU::UserCore *RSDK::SKU::userCore = NULL;
#endif

bool32 RSDK::changedVideoSettings = false;
VideoSettings RSDK::videoSettings;
VideoSettings RSDK::videoSettingsBackup;

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
    // if (usingSteamAPI)
    //     userCore = InitSteamCore();
    // if (usingEGSAPI)
    //     userCore = InitEOSCore();
    // if (usingSwitchAPI)
    //     userCore = InitNXCore();

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

    int32 achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int32));
    bool32 loaded = false;
#if RETRO_REV02
    loaded = userStorage->TryLoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int32), NULL);
#else
    loaded               = LoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int32));
#endif
    for (int32 i = 0; i < (int32)achievementList.size(); ++i) {
        achievementList[i].achieved = achievementsRAM[i];
    }

    leaderboardList.clear();
    int32 leaderboardsRAM[0x200];
    memset(leaderboardsRAM, 0, 0x200 * sizeof(int32));
#if RETRO_REV02
    loaded = userStorage->TryLoadUserFile("Leaderboards.bin", leaderboardsRAM, sizeof(leaderboardsRAM), NULL);
#else
    loaded               = LoadUserFile("Leaderboards.bin", leaderboardsRAM, sizeof(leaderboardsRAM));
#endif
    if (loaded) {
        int32 pos = 1;
        for (int32 i = 0; i < leaderboardsRAM[0]; ++i) {
            leaderboardList.push_back(LeaderboardInfo());
            int32 len = leaderboardsRAM[pos++];
            memcpy(leaderboardList[i].name, &leaderboardsRAM[pos], len);
            int32 size = (len / 4) + (4 - ((len % 4) ? (len % 4) : 4));
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

        for (int32 i = 0; i < statCount; ++i) {
            StatInfo stat;
            memcpy(stat.data, &statsRAM[pos], sizeof(stat.data));
            pos += sizeof(stat.data);
        }
    }
    delete[] statsRAM;
}
void RSDK::SKU::ReleaseUserData()
{
    SaveUserData();

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

void RSDK::SKU::SaveUserData()
{
    int32 achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int32));
    for (int32 i = 0; i < (int32)achievementList.size(); ++i) {
        achievementsRAM[i] = achievementList[i].achieved;
    }
#if RETRO_REV02
    userStorage->TrySaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int32), NULL, false);
#else
    SaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int32));
#endif

    int32 leaderboardsRAM[0x100];
    memset(leaderboardsRAM, 0, 0x100 * sizeof(int32));
    leaderboardsRAM[0] = (int32)leaderboardList.size();
    int32 pos          = 1;
    for (int32 i = 0; i < (int32)leaderboardList.size(); ++i) {
        int32 len              = (int32)strlen(leaderboardList[i].name);
        leaderboardsRAM[pos++] = len;
        memcpy(&leaderboardsRAM[pos], leaderboardList[i].name, len);
        int32 size = (len / 4) + (4 - ((len % 4) ? (len % 4) : 4));
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

    ((uint32 *)statsRAM)[0] = (int32)statList.size();

    pos = sizeof(uint32);
    for (int32 i = 0; i < (int32)statList.size(); ++i) {
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
#if RETRO_RENDERDEVICE_SDL2
    // SDL_OpenURL("http://www.sonicthehedgehog.com/mania/manual");
    PrintLog(PRINT_NORMAL, "DUMMY LaunchManual()");
#else
    PrintLog(PRINT_NORMAL, "EMPTY LaunchManual()");
#endif
}
void RSDK::SKU::ExitGame() { RenderDevice::isRunning = false; }

int32 RSDK::SKU::GetDefaultGamepadType()
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

int32 RSDK::SKU::ShowExtensionOverlay(byte overlay)
{
    switch (overlay) {
        default: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d", overlay); break;
        case 0: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d (Plus Upsell Screen)", overlay); break;
    }
    return 1;
}
#endif

int32 RSDK::GetVideoSetting(int32 id)
{
    switch (id) {
        case VIDEOSETTING_WINDOWED: return videoSettings.windowed;
        case VIDEOSETTING_BORDERED: return videoSettings.bordered;
        case VIDEOSETTING_EXCLUSIVEFS: return videoSettings.exclusiveFS;
        case VIDEOSETTING_VSYNC: return videoSettings.vsync;
        case VIDEOSETTING_TRIPLEBUFFERED: return videoSettings.tripleBuffered;
        case VIDEOSETTING_WINDOW_WIDTH: return videoSettings.windowWidth;
        case VIDEOSETTING_WINDOW_HEIGHT: return videoSettings.windowHeight;
        case VIDEOSETTING_FSWIDTH: return videoSettings.fsWidth;
        case VIDEOSETTING_FSHEIGHT: return videoSettings.fsHeight;
        case VIDEOSETTING_REFRESHRATE: return videoSettings.refreshRate;
        case VIDEOSETTING_SHADERSUPPORT: return videoSettings.shaderSupport;
        case VIDEOSETTING_SHADERID: return videoSettings.shaderID;
        case VIDEOSETTING_SCREENCOUNT: return videoSettings.screenCount;
#if RETRO_REV02
        case VIDEOSETTING_DIMTIMER: return videoSettings.dimTimer;
#endif
        case VIDEOSETTING_STREAMSENABLED: return engine.streamsEnabled;
        case VIDEOSETTING_STREAM_VOL: return (int32)(engine.streamVolume * 1024.0);
        case VIDEOSETTING_SFX_VOL: return (int32)(engine.soundFXVolume * 1024.0);
        case VIDEOSETTING_LANGUAGE:
#if RETRO_REV02
            return SKU::curSKU.language;
#else
            return gameVerInfo.language;
#endif
        case VIDEOSETTING_CHANGED: return changedVideoSettings;

        default: break;
    }

    return 0;
}

void RSDK::SetVideoSetting(int32 id, int32 value)
{
    bool32 boolVal = value;
    switch (id) {
        case VIDEOSETTING_WINDOWED:
            if (videoSettings.windowed != boolVal) {
                videoSettings.windowed = boolVal;
                changedVideoSettings   = true;
            }
            break;

        case VIDEOSETTING_BORDERED:
            if (videoSettings.bordered != boolVal) {
                videoSettings.bordered = boolVal;
                changedVideoSettings   = true;
            }
            break;

        case VIDEOSETTING_EXCLUSIVEFS:
            if (videoSettings.exclusiveFS != boolVal) {
                videoSettings.exclusiveFS = boolVal;
                changedVideoSettings      = true;
            }
            break;

        case VIDEOSETTING_VSYNC:
            if (videoSettings.vsync != boolVal) {
                videoSettings.vsync  = boolVal;
                changedVideoSettings = true;
            }
            break;

        case VIDEOSETTING_TRIPLEBUFFERED:
            if (videoSettings.tripleBuffered != boolVal) {
                videoSettings.tripleBuffered = boolVal;
                changedVideoSettings         = true;
            }
            break;

        case VIDEOSETTING_WINDOW_WIDTH:
            if (videoSettings.windowWidth != value) {
                videoSettings.windowWidth = value;
                changedVideoSettings      = true;
            }
            break;

        case VIDEOSETTING_WINDOW_HEIGHT:
            if (videoSettings.windowHeight != value) {
                videoSettings.windowHeight = value;
                changedVideoSettings       = true;
            }
            break;

        case VIDEOSETTING_FSWIDTH: videoSettings.fsWidth = value; break;
        case VIDEOSETTING_FSHEIGHT: videoSettings.fsHeight = value; break;
        case VIDEOSETTING_REFRESHRATE: videoSettings.refreshRate = value; break;
        case VIDEOSETTING_SHADERSUPPORT: videoSettings.shaderSupport = value; break;
        case VIDEOSETTING_SHADERID:
            if (videoSettings.shaderID != value) {
                videoSettings.shaderID = value;
                changedVideoSettings   = true;
            }
            break;

        case VIDEOSETTING_SCREENCOUNT: videoSettings.screenCount = value; break;
#if RETRO_REV02
        case VIDEOSETTING_DIMTIMER: videoSettings.dimLimit = value; break;
#endif
        case VIDEOSETTING_STREAMSENABLED:
            if (engine.streamsEnabled != boolVal)
                changedVideoSettings = true;

            engine.streamsEnabled = boolVal;
            break;

        case VIDEOSETTING_STREAM_VOL:
            if (engine.streamVolume != (value / 1024.0f)) {
                engine.streamVolume  = (float)value / 1024.0f;
                changedVideoSettings = true;
            }
            break;

        case VIDEOSETTING_SFX_VOL:
            if (engine.soundFXVolume != ((float)value / 1024.0f)) {
                engine.soundFXVolume = (float)value / 1024.0f;
                changedVideoSettings = true;
            }
            break;

        case VIDEOSETTING_LANGUAGE:
#if RETRO_REV02
            SKU::curSKU.language = value;
#else
            gameVerInfo.language = value;
#endif
            break;

        case VIDEOSETTING_STORE: memcpy(&videoSettingsBackup, &videoSettings, sizeof(videoSettings)); break;

        case VIDEOSETTING_RELOAD:
            changedVideoSettings = true;
            memcpy(&videoSettings, &videoSettingsBackup, sizeof(videoSettingsBackup));
            break;

        case VIDEOSETTING_CHANGED: changedVideoSettings = boolVal; break;

        case VIDEOSETTING_WRITE: SaveSettingsINI(value); break;

        default: break;
    }
}
#if RETRO_PLATFORM == RETRO_ANDROID
#include <jni.h>
#endif

char buttonNames[18][8] = { "U", "D", "L", "R", "START", "SELECT", "LSTICK", "RSTICK", "L1", "R1", "C", "Z", "A", "B", "X", "Y", "L2", "R2" };

void RSDK::LoadSettingsINI()
{
    videoSettings.screenCount = 1;
    videoSettings.pixHeight   = SCREEN_YSIZE;
    videoSettings.windowState = WINDOWSTATE_UNINITIALIZED;

    int32 platform = PLATFORM_DEV;
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

    int32 defKeyMaps[PLAYER_COUNT + 1][12] = {
        { KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING,
          KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING },

        { VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, VK_A, VK_S, VK_D, VK_Q, VK_W, VK_E, VK_RETURN, VK_TAB },
        { VK_NUMPAD8, VK_NUMPAD5, VK_NUMPAD4, VK_NUMPAD6, VK_J, VK_J, KEYMAP_NO_MAPPING, VK_U, VK_I, KEYMAP_NO_MAPPING, VK_OEM_4, VK_OEM_6 },

        { KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING,
          KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING },

        { KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING,
          KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING, KEYMAP_NO_MAPPING }
    };

    if (ini) {
#if RETRO_REV02
        SKU::curSKU.language = iniparser_getint(ini, "Game:language", LANGUAGE_EN);
#else
        gameVerInfo.language = (int32)strtol(iniparser_getstring(ini, "Game:language", "0"), NULL, 0);
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

        videoSettings.windowed       = iniparser_getboolean(ini, "Video:windowed", true);
        videoSettings.bordered       = iniparser_getboolean(ini, "Video:border", true);
        videoSettings.exclusiveFS    = iniparser_getboolean(ini, "Video:exclusiveFS", false);
        videoSettings.vsync          = iniparser_getboolean(ini, "Video:vsync", false);
        videoSettings.tripleBuffered = iniparser_getboolean(ini, "Video:tripleBuffering", false);

        videoSettings.pixWidth = iniparser_getint(ini, "Video:pixWidth", DEFAULT_SCREEN_XSIZE);

        videoSettings.windowWidth   = iniparser_getint(ini, "Video:winWidth", DEFAULT_SCREEN_XSIZE);
        videoSettings.windowHeight  = iniparser_getint(ini, "Video:winHeight", SCREEN_YSIZE);
        videoSettings.fsWidth       = iniparser_getint(ini, "Video:fsWidth", 0);
        videoSettings.fsHeight      = iniparser_getint(ini, "Video:fsHeight", 0);
        videoSettings.refreshRate   = iniparser_getint(ini, "Video:refreshRate", 60);
        videoSettings.shaderSupport = iniparser_getboolean(ini, "Video:shaderSupport", true);
        videoSettings.shaderID      = iniparser_getint(ini, "Video:screenShader", 0);

        engine.streamsEnabled = iniparser_getboolean(ini, "Audio:streamsEnabled", true);
        engine.streamVolume   = iniparser_getdouble(ini, "Audio:streamVolume", 0.8);
        engine.soundFXVolume  = iniparser_getdouble(ini, "Audio:sfxVolume", 1.0);

        for (int32 i = 1; i <= PLAYER_COUNT; ++i) {
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

        AllocateStorage(sizeof(GamePadMappings) * gamePadCount, (void **)&gamePadMappings, DATASET_STG, true);

        for (int32 i = 1; i <= gamePadCount; ++i) {
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
            for (int32 b = 0; tok; ++b) {
                gamePadMappings[i].buttons[b].mappingType = atoi(tok);
                tok                                       = strtok(0, " ,.-");
            }

            sprintf(buffer, "GamePad Map %d:offsets", i);
            sprintf(buffer2, "%s", iniparser_getstring(ini, buffer, 0));

            tok = strtok(buffer2, ", ");
            for (int32 b = 0; tok; ++b) {
                gamePadMappings[i].buttons[b].offset = atoi(tok);
                tok                                  = strtok(0, " ,.-");
            }

            sprintf(buffer, "GamePad Map %d:maskVals", i);
            sprintf(buffer2, "%s", iniparser_getstring(ini, buffer, 0));
            tok = strtok(buffer2, ", ");
            for (int32 b = 0; tok; ++b) {
                int32 mask = 1;
                for (int32 m = 0; m < 18; ++m) {
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
        videoSettings.windowed       = true;
        videoSettings.bordered       = false;
        videoSettings.exclusiveFS    = true;
        videoSettings.vsync          = true;
        videoSettings.tripleBuffered = false;
        videoSettings.shaderSupport  = true;
        videoSettings.pixWidth       = DEFAULT_SCREEN_XSIZE;
        videoSettings.fsWidth        = 0;
        videoSettings.windowWidth    = videoSettings.pixWidth * 1;
        videoSettings.windowHeight   = SCREEN_YSIZE * 1;
        videoSettings.fsHeight       = 0;
        videoSettings.refreshRate    = 60;
        videoSettings.shaderID       = SHADER_NONE;
        engine.streamsEnabled        = true;
        engine.streamVolume          = 1.0f;
        engine.soundFXVolume         = 1.0f;
        engine.devMenu               = false;

        for (int32 i = 1; i <= PLAYER_COUNT; ++i) {
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

        SaveSettingsINI(true);
        engine.devMenu = CheckDataFile("Data.rsdk", 0, useBuffer);
    }
}

void RSDK::SaveSettingsINI(bool32 writeToFile)
{
    // only done on windows and "dev", consoles use "options.bin"
#if RETRO_REV02
    if (SKU::curSKU.platform != PLATFORM_PC && SKU::curSKU.platform != PLATFORM_DEV)
        return;
#else
    if (gameVerInfo.platform != PLATFORM_PC && gameVerInfo.platform != PLATFORM_DEV)
        return;
#endif

    if (changedVideoSettings || writeToFile) {
        char pathBuffer[0x100];
        sprintf(pathBuffer, "%sSettings.ini", SKU::userFileDir);

        dictionary *ini = iniparser_load(pathBuffer);
        FileIO *file    = fOpen(pathBuffer, "wb");

        // ================
        // SIGNATURE
        // ================
        WriteText(file, "; Retro Engine Config File\n\n");

        // ================
        // GAME
        // ================
        WriteText(file, "[Game]\n");
        if (ini) {
            if (strcmp(iniparser_getstring(ini, "Game:dataFile", "optionNotFound"), "optionNotFound") != 0) {
                WriteText(file, "dataFile=%s\n", iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"));
            }

            if (strcmp(iniparser_getstring(ini, "Game:devMenu", "optionNotFound"), "optionNotFound") != 0)
                WriteText(file, "devMenu=%s\n", (engine.devMenu ? "y" : "n"));

#if !RETRO_USE_ORIGINAL_CODE
            if (strcmp(iniparser_getstring(ini, "Game:gameLogic", "optionNotFound"), "optionNotFound") != 0)
                WriteText(file, "gameLogic=%s\n", iniparser_getstring(ini, "Game:gameLogic", "Game"));

            if (strcmp(iniparser_getstring(ini, "Game:confirmButtonFlip", "optionNotFound"), "optionNotFound") != 0)
                WriteText(file, "confirmButtonFlip=%s\n", (engine.confirmFlip ? "y" : "n"));
            if (strcmp(iniparser_getstring(ini, "Game:xyButtonFlip", "optionNotFound"), "optionNotFound") != 0)
                WriteText(file, "xyButtonFlip=%s\n", (engine.XYFlip ? "y" : "n"));
#endif
        }

#if RETRO_REV02
        WriteText(file, "language=%d\n", SKU::curSKU.language);
#else
        WriteText(file, "language=%d\n", gameVerInfo.language);
#endif

        // ================
        // VIDEO
        // ================
        WriteText(file, "\n[Video]\n");
        WriteText(file, "; NB: Fullscreen Resolution can be explicitly set with values fsWidth and fsHeight\n");
        WriteText(file, "; If not listed, fullscreen will just use the desktop resolution\n");
        WriteText(file, "windowed=%s\n", (videoSettings.windowed ? "y" : "n"));
        WriteText(file, "border=%s\n", (videoSettings.bordered ? "y" : "n"));
        WriteText(file, "exclusiveFS=%s\n", (videoSettings.exclusiveFS ? "y" : "n"));
        WriteText(file, "vsync=%s\n", (videoSettings.vsync ? "y" : "n"));
        WriteText(file, "tripleBuffering=%s\n", (videoSettings.tripleBuffered ? "y" : "n"));
        if (ini) {
            if (strcmp(iniparser_getstring(ini, "Video:pixWidth", "optionNotFound"), "optionNotFound") == 0)
                WriteText(file, "pixWidth=%d\n", videoSettings.pixWidth);
        }
        WriteText(file, "winWidth=%d\n", videoSettings.windowWidth);
        WriteText(file, "winHeight=%d\n", videoSettings.windowHeight);
        if (videoSettings.fsWidth > 0)
            WriteText(file, "fsWidth=%d\n", videoSettings.fsWidth);
        if (videoSettings.fsHeight > 0)
            WriteText(file, "fsHeight=%d\n", videoSettings.fsHeight);
        if (videoSettings.refreshRate > 0)
            WriteText(file, "refreshRate=%d\n", videoSettings.refreshRate);

        WriteText(file, "shaderSupport=%s\n", (videoSettings.shaderSupport ? "y" : "n"));
        WriteText(file, "screenShader=%d\n", videoSettings.shaderID);

        // ================
        // AUDIO
        // ================
        WriteText(file, "\n[Audio]\n");
        WriteText(file, "streamsEnabled=%s\n", (engine.streamsEnabled ? "y" : "n"));
        WriteText(file, "streamVolume=%f\n", engine.streamVolume);
        WriteText(file, "sfxVolume=%f\n", engine.soundFXVolume);

        // ==========================
        // OPTIONS (decomp only)
        // ==========================

        // ================
        // KE^YBOARD MAP
        // ================
        for (int32 i = 1; i <= PLAYER_COUNT; ++i) {
            WriteText(file, "\n[Keyboard Map %d]\n", i);
            WriteText(file, "up=0x%x\n", controller[i].keyUp.keyMap);
            WriteText(file, "down=0x%x\n", controller[i].keyDown.keyMap);
            WriteText(file, "left=0x%x\n", controller[i].keyLeft.keyMap);
            WriteText(file, "right=0x%x\n", controller[i].keyRight.keyMap);
            WriteText(file, "buttonA=0x%x\n", controller[i].keyA.keyMap);
            WriteText(file, "buttonB=0x%x\n", controller[i].keyB.keyMap);
            WriteText(file, "buttonC=0x%x\n", controller[i].keyC.keyMap);
            WriteText(file, "buttonX=0x%x\n", controller[i].keyX.keyMap);
            WriteText(file, "buttonY=0x%x\n", controller[i].keyY.keyMap);
            WriteText(file, "buttonZ=0x%x\n", controller[i].keyZ.keyMap);
            WriteText(file, "start=0x%x\n", controller[i].keyStart.keyMap);
            WriteText(file, "select=0x%x\n", controller[i].keySelect.keyMap);
        }

        for (int32 i = 0; i < gamePadCount; ++i) {
            WriteText(file, "\n[Keyboard Map %d]\n", i + 1);
            WriteText(file, "name=%s\n", gamePadMappings[i].name);
            WriteText(file, "type=0x%x\n", gamePadMappings[i].type);
            WriteText(file, "vendorID=0x%x\n", gamePadMappings[i].vendorID);
            WriteText(file, "productID=0x%x\n", gamePadMappings[i].productID);

            WriteText(file, "mappingTypes=");
            for (int32 b = 0; b < 24; ++b) {
                WriteText(file, "%d,", gamePadMappings[i].buttons[b].mappingType);
            }
            WriteText(file, "\n");
            WriteText(file, "offsets=");
            for (int32 b = 0; b < 24; ++b) {
                WriteText(file, "%d,", gamePadMappings[i].buttons[b].offset);
            }
            WriteText(file, "\n");
            WriteText(file, "maskVals=");
            for (int32 b = 0; b < 24; ++b) {
                if (gamePadMappings[i].buttons[b].maskVal) {
                    int32 m = 0;
                    while (true) {
                        if (1 << m == gamePadMappings[i].buttons[b].maskVal) {
                            WriteText(file, "%d,", gamePadMappings[i].buttons[b].maskVal);
                            break;
                        }
                    }

                    if (m == 18)
                        WriteText(file, "?,");
                }
                else {
                    WriteText(file, "?,");
                }
            }
            WriteText(file, "\n");
        }

        iniparser_freedict(ini);
        fClose(file);
    }
}
