#include "RSDK/Core/RetroEngine.hpp"

#include "iniparser/iniparser.h"


// ====================
// API Cores
// ====================

namespace RSDK::SKU
{
// Dummy API
#if RETRO_USERCORE_DUMMY
#include "RSDK/User/Dummy/DummyCore.cpp"
#endif

// Steam API
#if RETRO_USERCORE_STEAM
#include "RSDK/User/Steam/SteamCore.cpp"
#endif

// Epic Games API
#if RETRO_USERCORE_EOS
#include "RSDK/User/EOS/EOSCore.cpp"
#endif

// Switch API
#if RETRO_USERCORE_NX
#include "RSDK/User/NX/NXCore.cpp"
#endif

} // namespace RSDK::SKU

using namespace RSDK;

#if RETRO_REV02
SKU::UserCore *RSDK::SKU::userCore = NULL;
#endif

void RSDK::SKU::InitUserData()
{
    InitUserDirectory();

#if RETRO_REV02
#if RETRO_USERCORE_DUMMY
    if (dummyCore)
        delete dummyCore;
    dummyCore = InitDummyCore();
#endif

    // Initalize platform-specific subsystems here

#if RETRO_USERCORE_STEAM
    userCore = InitSteamCore();
#endif

#if RETRO_USERCORE_EOS
    userCore = InitEOSCore();
#endif

#if RETRO_USERCORE_NX
    userCore = InitNXCore();
#endif

#if RETRO_USERCORE_DUMMY
    if (!userCore) { // no platform core, so default to dummy funcs
        userCore = dummyCore;
    }
    else if (dummyCore) {
        delete dummyCore;
        dummyCore = nullptr;
    }
#endif

    if (!userDBStorage)
        userDBStorage = new UserDBStorage;
#endif

#if RETRO_REV02
    curSKU.platform    = userCore->GetUserPlatform();
    curSKU.language    = userCore->GetUserLanguage();
    curSKU.region      = userCore->GetUserRegion();
    engine.confirmFlip = userCore->GetConfirmButtonFlip();
#else

#if RETRO_PLATFORM == RETRO_PS4
    gameVerInfo.platform = PLATFORM_PS4;
#elif RETRO_PLATFORM == RETRO_XB1
    gameVerInfo.platform = PLATFORM_XB1;
#elif RETRO_PLATFORM == RETRO_SWITCH || RETRO_PLATFORM == RETRO_ANDROID
    gameVerInfo.platform = PLATFORM_SWITCH;
#endif

    // default to PC (or dev if dev stuff is enabled)
    gameVerInfo.platform = engine.devMenu ? PLATFORM_DEV : PLATFORM_PC;
    gameVerInfo.language = LANGUAGE_EN;
    gameVerInfo.region   = REGION_US;
    engine.confirmFlip   = false;
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

#if RETRO_USERCORE_DUMMY
    int32 achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int32));
    bool32 loaded = false;
    loaded        = LoadUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int32));
    for (int32 i = 0; i < (int32)achievementList.size(); ++i) {
        achievementList[i].achieved = achievementsRAM[i];
    }
#endif
}
void RSDK::SKU::ReleaseUserCore()
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

    if (userDBStorage)
        delete userDBStorage;
    userDBStorage = nullptr;

    if (userCore) {
        userCore->Shutdown();
        delete userCore;
    }

#if RETRO_USERCORE_DUMMY
    dummyCore = nullptr;
#endif
    userCore  = nullptr;
#endif
}

void RSDK::SKU::SaveUserData()
{
#if RETRO_USERCORE_DUMMY
    int32 achievementsRAM[0x100];
    memset(achievementsRAM, 0, 0x100 * sizeof(int32));
    for (int32 i = 0; i < (int32)achievementList.size(); ++i) {
        achievementsRAM[i] = achievementList[i].achieved;
    }
    SaveUserFile("Achievements.bin", achievementsRAM, 0x100 * sizeof(int32));
#endif
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

bool32 RSDK::SKU::ShowExtensionOverlay(uint8 overlay)
{
    switch (overlay) {
        default: PrintLog(PRINT_POPUP, "Show Extension Overlay: %d", overlay); break;
    }
    return false;
}
#endif

#if RETRO_PLATFORM == RETRO_ANDROID
#include <jni.h>
#endif

#if !RETRO_USE_ORIGINAL_CODE
CustomSettings RSDK::customSettings;
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
    sprintf_s(pathBuffer, (int32)sizeof(pathBuffer), "%sSettings.ini", SKU::userFileDir);

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
        gameVerInfo.language = iniparser_getint(ini, "Game:language", LANGUAGE_EN);
#endif

        engine.devMenu = true;
        if (LoadDataPack(iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"), 0, useBuffer))
            engine.devMenu = iniparser_getboolean(ini, "Game:devMenu", false);

#if !RETRO_USE_ORIGINAL_CODE

        customSettings.region                    = iniparser_getint(ini, "Game:region", -1);
        customSettings.confirmButtonFlip         = iniparser_getboolean(ini, "Game:confirmButtonFlip", false);
        customSettings.xyButtonFlip              = iniparser_getboolean(ini, "Game:xyButtonFlip", false);
        customSettings.enableControllerDebugging = iniparser_getboolean(ini, "Game:enableControllerDebugging", false);
        customSettings.disableFocusPause         = iniparser_getboolean(ini, "Game:disableFocusPause", false);

        sprintf_s(gameLogicName, (int32)sizeof(gameLogicName), "%s", iniparser_getstring(ini, "Game:gameLogic", "Game"));
        sprintf_s(customSettings.username, (int32)sizeof(customSettings.username), "%s", iniparser_getstring(ini, "Game:username", ""));

        if (customSettings.region >= 0) {
#if RETRO_REV02
            SKU::curSKU.region = customSettings.region;
#else
            gameVerInfo.region = customSettings.region;
#endif
        }

        engine.confirmFlip = customSettings.confirmButtonFlip;
        engine.XYFlip      = customSettings.xyButtonFlip;
#else
        sprintf_s(gameLogicName, (int32)sizeof(gameLogicName), "Game"));
#endif

        videoSettings.windowed       = iniparser_getboolean(ini, "Video:windowed", true);
        videoSettings.bordered       = iniparser_getboolean(ini, "Video:border", true);
        videoSettings.exclusiveFS    = iniparser_getboolean(ini, "Video:exclusiveFS", false);
        videoSettings.vsync          = iniparser_getboolean(ini, "Video:vsync", false);
        videoSettings.tripleBuffered = iniparser_getboolean(ini, "Video:tripleBuffering", false);

        videoSettings.pixWidth = iniparser_getint(ini, "Video:pixWidth", DEFAULT_PIXWIDTH);

        videoSettings.windowWidth   = iniparser_getint(ini, "Video:winWidth", DEFAULT_PIXWIDTH);
        videoSettings.windowHeight  = iniparser_getint(ini, "Video:winHeight", SCREEN_YSIZE);
        videoSettings.fsWidth       = iniparser_getint(ini, "Video:fsWidth", 0);
        videoSettings.fsHeight      = iniparser_getint(ini, "Video:fsHeight", 0);
        videoSettings.refreshRate   = iniparser_getint(ini, "Video:refreshRate", 60);
        videoSettings.shaderSupport = iniparser_getboolean(ini, "Video:shaderSupport", true);
        videoSettings.shaderID      = iniparser_getint(ini, "Video:screenShader", SHADER_NONE);

#if !RETRO_USE_ORIGINAL_CODE
        customSettings.maxPixWidth = iniparser_getint(ini, "Video:maxPixWidth", DEFAULT_PIXWIDTH);
#endif

        engine.streamsEnabled = iniparser_getboolean(ini, "Audio:streamsEnabled", true);
        engine.streamVolume   = iniparser_getdouble(ini, "Audio:streamVolume", 0.8);
        engine.soundFXVolume  = iniparser_getdouble(ini, "Audio:sfxVolume", 1.0);

        for (int32 i = 1; i <= PLAYER_COUNT; ++i) {
            char buffer[0x30];

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:up", i);
            controller[i].keyUp.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][0]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:down", i);
            controller[i].keyDown.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][1]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:left", i);
            controller[i].keyLeft.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][2]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:right", i);
            controller[i].keyRight.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][3]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:buttonA", i);
            controller[i].keyA.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][4]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:buttonB", i);
            controller[i].keyB.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][5]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:buttonC", i);
            controller[i].keyC.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][6]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:buttonX", i);
            controller[i].keyX.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][7]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:buttonY", i);
            controller[i].keyY.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][8]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:buttonZ", i);
            controller[i].keyZ.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][9]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:start", i);
            controller[i].keyStart.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][10]);

            sprintf_s(buffer, (int32)sizeof(buffer), "Keyboard Map %d:select", i);
            controller[i].keySelect.keyMap = iniparser_getint(ini, buffer, defKeyMaps[i][11]);
        }

        gamePadCount = 0;
        while (true) {
            char buffer[0x30];
            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:name", gamePadCount + 1);
            if (strcmp(iniparser_getstring(ini, buffer, ";unknown;"), ";unknown;") != 0) {
                gamePadCount++;
            }
            else {
                break;
            }
        }

        AllocateStorage(sizeof(GamePadMappings) * gamePadCount, (void **)&gamePadMappings, DATASET_STG, true);

        for (int32 i = 1; i <= gamePadCount; ++i) {
            char buffer[0x30];
            char mappings[0x100];

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:name", i);
            sprintf_s(gamePadMappings[i].name, (int32)sizeof(gamePadMappings[i].name), "%s", iniparser_getstring(ini, buffer, 0));

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:type", i);
            gamePadMappings[i].type = iniparser_getint(ini, buffer, 0);

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:vendorID", i);
            gamePadMappings[i].vendorID = iniparser_getint(ini, buffer, 0);

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:productID", i);
            gamePadMappings[i].productID = iniparser_getint(ini, buffer, 0);

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:mappingTypes", i);
            sprintf_s(mappings, (int32)sizeof(mappings), "%s", iniparser_getstring(ini, buffer, 0));

            char *tok = strtok(mappings, ", ");
            for (int32 b = 0; tok; ++b) {
                gamePadMappings[i].buttons[b].mappingType = atoi(tok);
                tok                                       = strtok(0, " ,.-");
            }

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:offsets", i);
            sprintf_s(mappings, (int32)sizeof(mappings), "%s", iniparser_getstring(ini, buffer, 0));

            tok = strtok(mappings, ", ");
            for (int32 b = 0; tok; ++b) {
                gamePadMappings[i].buttons[b].offset = atoi(tok);
                tok                                  = strtok(0, " ,.-");
            }

            sprintf_s(buffer, (int32)sizeof(buffer), "GamePad Map %d:maskVals", i);
            sprintf_s(mappings, (int32)sizeof(mappings), "%s", iniparser_getstring(ini, buffer, 0));
            tok = strtok(mappings, ", ");
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
        videoSettings.pixWidth       = DEFAULT_PIXWIDTH;
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
        engine.devMenu = LoadDataPack("Data.rsdk", 0, useBuffer);
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
        sprintf_s(pathBuffer, (int32)sizeof(pathBuffer), "%sSettings.ini", SKU::userFileDir);

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
            if (strcmp(iniparser_getstring(ini, "Game:dataFile", ";unknown;"), ";unknown;") != 0) {
                WriteText(file, "dataFile=%s\n", iniparser_getstring(ini, "Game:dataFile", "Data.rsdk"));
            }

            if (strcmp(iniparser_getstring(ini, "Game:devMenu", ";unknown;"), ";unknown;") != 0)
                WriteText(file, "devMenu=%s\n", (engine.devMenu ? "y" : "n"));

#if !RETRO_USE_ORIGINAL_CODE
            if (strcmp(iniparser_getstring(ini, "Game:gameLogic", ";unknown;"), ";unknown;") != 0)
                WriteText(file, "gameLogic=%s\n", iniparser_getstring(ini, "Game:gameLogic", "Game"));

            WriteText(file, "confirmButtonFlip=%s\n", (customSettings.confirmButtonFlip ? "y" : "n"));
            WriteText(file, "xyButtonFlip=%s\n", (customSettings.xyButtonFlip ? "y" : "n"));

            WriteText(file, "enableControllerDebugging=%s\n", (customSettings.enableControllerDebugging ? "y" : "n"));

            WriteText(file, "; Determines if the engine should pause when window focus is lost or not\n");
            WriteText(file, "disableFocusPause=%s\n", (customSettings.disableFocusPause ? "y" : "n"));

            if (strcmp(iniparser_getstring(ini, "Game:username", ";unknown;"), ";unknown;") != 0)
                WriteText(file, "username=%s\n", iniparser_getstring(ini, "Game:username", ""));

            WriteText(file, "; if -1, the game will decide what region to use, if 0 or higher, forces a specific region\n");
            WriteText(file, "region=%d\n", customSettings.region);
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
            if (strcmp(iniparser_getstring(ini, "Video:pixWidth", ";unknown;"), ";unknown;") == 0)
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

#if !RETRO_USE_ORIGINAL_CODE
        WriteText(file, "; Maximum width the screen will be allowed to be. A value of 0 will disable the maximum width\n");
        WriteText(file, "maxPixWidth=%d\n", customSettings.maxPixWidth);
#endif

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
