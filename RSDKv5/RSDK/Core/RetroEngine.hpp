#ifndef RETROENGINE_H
#define RETROENGINE_H

// ================
// STANDARD LIBS
// ================
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <ctime>

// ================
// STANDARD TYPES
// ================
typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

typedef uint32 bool32;

typedef uint32 color;

typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef signed long long int64;
typedef unsigned long long uint64;

enum GamePlatforms {
    PLATFORM_PC     = 0,
    PLATFORM_PS4    = 1,
    PLATFORM_XB1    = 2,
    PLATFORM_SWITCH = 3,
    PLATFORM_DEV    = 0xFF,
};

enum GameLanguages {
    LANGUAGE_EN,
    LANGUAGE_FR,
    LANGUAGE_IT,
    LANGUAGE_GE,
    LANGUAGE_SP,
    LANGUAGE_JP,
    LANGUAGE_KO,
    LANGUAGE_SC,
    LANGUAGE_TC,
};

enum GameRegions {
    REGION_US,
    REGION_JP,
    REGION_EU,
};

#define RETRO_USE_ORIGINAL_CODE (0)

#ifndef RETRO_STANDALONE
#define RETRO_STANDALONE (1)
#endif

#define RETRO_WIN     (0)
#define RETRO_PS4     (1)
#define RETRO_XB1     (2)
#define RETRO_SWITCH  (3)
#define RETRO_OSX     (4)
#define RETRO_LINUX   (5)
#define RETRO_iOS     (6)
#define RETRO_ANDROID (7)
#define RETRO_UWP     (8)

#if defined _WIN32

#if defined WINAPI_FAMILY
#if WINAPI_FAMILY != WINAPI_FAMILY_APP
#define RETRO_PLATFORM (RETRO_WIN)
#else
#define RETRO_PLATFORM (RETRO_UWP)
#endif
#else
#define RETRO_PLATFORM (RETRO_WIN)
#endif

#elif defined __APPLE__
#define RETRO_USING_MOUSE
#define RETRO_USING_TOUCH
#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR
#define RETRO_PLATFORM (RETRO_iOS)
#elif TARGET_OS_IPHONE
#define RETRO_PLATFORM (RETRO_iOS)
#elif TARGET_OS_MAC
#define RETRO_PLATFORM (RETRO_OSX)
#else
#error "Unknown Apple platform"
#endif
#elif defined __ANDROID__
#define RETRO_PLATFORM (RETRO_ANDROID)
#elif defined __linux__
#define RETRO_PLATFORM (RETRO_LINUX)
#elif defined __SWITCH__
#define RETRO_PLATFORM (RETRO_SWITCH)
#else
#define RETRO_PLATFORM (RETRO_WIN)
#endif

#define SCREEN_XMAX  (1280)
#define SCREEN_YSIZE (240)

#define BASE_PATH ""

// ============================
// RENDER DEVICE BACKENDS
// ============================
#define RETRO_RENDERDEVICE_SDL2      (0)
#define RETRO_RENDERDEVICE_DIRECTX9  (0)
#define RETRO_RENDERDEVICE_DIRECTX11 (0)
// CUSTOM
#define RETRO_RENDERDEVICE_OPENGL3   (0)

// ============================
// AUDIO DEVICE BACKENDS
// ============================
#define RETRO_AUDIODEVICE_SDL2   (0)
#define RETRO_AUDIODEVICE_XAUDIO (0)

// ============================
// INPUT DEVICE BACKENDS
// ============================
#define RETRO_INPUTDEVICE_KEYBOARD (1)
#define RETRO_INPUTDEVICE_XINPUT   (0)
#define RETRO_INPUTDEVICE_RAWINPUT (0)
#define RETRO_INPUTDEVICE_STEAM    (0)
#define RETRO_INPUTDEVICE_SDL2     (0)

#define DEFAULT_SCREEN_XSIZE (424)
#define DEFAULT_FULLSCREEN   false
#define RETRO_USING_MOUSE
#define RETRO_USING_TOUCH

// Determines if the engine is RSDKv5 rev01 (all versions pre-plus) or rev02 (all versions post-plus)
#define RETRO_REV02 (1)
// Determines if the engine should use EGS features like achievements or not (must be rev02)
#define RETRO_VER_EGS (RETRO_REV02 && 0)

// enables only EGS's ingame achievements popup without enabling anything else
#define RETRO_USE_DUMMY_ACHIEVEMENTS (1 && RETRO_REV02)

// enables the use of the mod loader
#define RETRO_USE_MOD_LOADER (!RETRO_USE_ORIGINAL_CODE && 1)

#if RETRO_PLATFORM == RETRO_WIN

#ifdef RSDK_USE_SDL2
#undef RETRO_RENDERDEVICE_SDL2
#define RETRO_RENDERDEVICE_SDL2 (1)

#undef RETRO_AUDIODEVICE_SDL2
#define RETRO_AUDIODEVICE_SDL2 (1)

#undef RETRO_INPUTDEVICE_SDL2
#define RETRO_INPUTDEVICE_SDL2 (1)
#else //! RSDK_USE_SDL2

#ifdef RSDK_USE_DX9
#undef RETRO_RENDERDEVICE_DIRECTX9
#define RETRO_RENDERDEVICE_DIRECTX9 (1)

#undef RETRO_INPUTDEVICE_XINPUT
#define RETRO_INPUTDEVICE_XINPUT (1)

#undef RETRO_INPUTDEVICE_RAWINPUT
#define RETRO_INPUTDEVICE_RAWINPUT (1)

#elif defined(RSDK_USE_GL3)
#undef RETRO_RENDERDEVICE_OPENGL3
#define RETRO_RENDERDEVICE_OPENGL3 (1)

#undef RETRO_INPUTDEVICE_GLFW
#define RETRO_INPUTDEVICE_GLFW (1)
#else
#error One of RSDK_USE_SDL2, RSDK_USE_DX9, or RSDK_USE_GL3 must be defined.
#endif //! RSDK_USE_DX9

#undef RETRO_AUDIODEVICE_XAUDIO
#define RETRO_AUDIODEVICE_XAUDIO (1)

#endif //! RSDK_USE_SDL2


#elif RETRO_PLATFORM == RETRO_XB1

#undef RETRO_RENDERDEVICE_DIRECTX11
#define RETRO_RENDERDEVICE_DIRECTX11 (1)

#elif RETRO_PLATFORM == RETRO_LINUX

#ifdef RSDK_USE_SDL2
#undef RETRO_RENDERDEVICE_SDL2
#define RETRO_RENDERDEVICE_SDL2 (1)
#undef RETRO_AUDIODEVICE_SDL2
#define RETRO_AUDIODEVICE_SDL2 (1)
#undef RETRO_INPUTDEVICE_SDL2
#define RETRO_INPUTDEVICE_SDL2 (1)

#elif defined(RSDK_USE_GL3)
#undef RETRO_RENDERDEVICE_OPENGL3
#define RETRO_RENDERDEVICE_OPENGL3 (1)
#undef RETRO_INPUTDEVICE_GLFW
#define RETRO_INPUTDEVICE_GLFW (1)
#undef RETRO_AUDIODEVICE_SDL2
#define RETRO_AUDIODEVICE_SDL2 (1)

#else
#error RSDK_USE_SDL2 or RSDK_USE_GL3 must be defined.
#endif //! RSDK_USE_SDL2

#elif RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_iOS || RETRO_PLATFORM == RETRO_ANDROID                 \
    || RETRO_PLATFORM == RETRO_SWITCH

#undef RETRO_RENDERDEVICE_SDL2
#define RETRO_RENDERDEVICE_SDL2 (1)

#undef RETRO_AUDIODEVICE_SDL2
#define RETRO_AUDIODEVICE_SDL2 (1)

#undef RETRO_INPUTDEVICE_SDL2
#define RETRO_INPUTDEVICE_SDL2 (1)

#endif

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_UWP


#if RETRO_AUDIODEVICE_XAUDIO
#include <XAudio2.h>
#endif

#if RETRO_INPUTDEVICE_XINPUT
#include <Xinput.h>
#endif

#if RETRO_RENDERDEVICE_DIRECTX9
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <timeapi.h>
#include <commctrl.h>
#include <dbt.h>

#include <d3d9.h>

#undef LoadImage
#elif RETRO_RENDERDEVICE_OPENGL3
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#endif

#if RETRO_RENDERDEVICE_SDL2
#ifdef USING_VCPKG
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif // ! USING_VCPKG
#endif // ! RETRO_RENDERDEVICE_SDL2

#include <theora/theoradec.h>
#endif // ! RETRO_WIN

#if RETRO_PLATFORM == RETRO_OSX
#include <SDL2/SDL.h>
#include <Theora/Theora.h>

#include "cocoaHelpers.hpp"
#elif RETRO_PLATFORM == RETRO_iOS
#include <SDL2/SDL.h>

#include "cocoaHelpers.hpp"
#elif RETRO_PLATFORM == RETRO_LINUX

#if RETRO_RENDERDEVICE_OPENGL3
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#endif

#if RETRO_AUDIODEVICE_SDL2
#include <SDL2/SDL.h>
#endif // ! RETRO_RENDERDEVICE_SDL2

#include <theora/theoradec.h>

#elif RETRO_PLATFORM == RETRO_ANDROID
#include <SDL.h>
#include <theora/theora.h>

//#include "androidHelpers.hpp"
#undef RETRO_STANDALONE
#define RETRO_STANDALONE (0)
#undef RETRO_USING_MOUSE
#elif RETRO_PLATFORM == RETRO_SWITCH
#include <SDL2/SDL.h>
#include <theora/theora.h>

#undef RETRO_USING_MOUSE
#endif

enum EngineStates {
    ENGINESTATE_LOAD,
    ENGINESTATE_REGULAR,
    ENGINESTATE_PAUSED,
    ENGINESTATE_FROZEN,
    ENGINESTATE_STEPOVER = 4,
    ENGINESTATE_DEVMENU  = 8,
    ENGINESTATE_VIDEOPLAYBACK,
    ENGINESTATE_SHOWIMAGE,
#if RETRO_REV02
    ENGINESTATE_ERRORMSG,
    ENGINESTATE_ERRORMSG_FATAL,
#endif
    ENGINESTATE_NONE,
};

// Utils
#include "iniparser/iniparser.h"

enum SeverityModes {
    PRINT_NORMAL,
    PRINT_POPUP,
    PRINT_ERROR,
    PRINT_FATAL,
};

// Engine
#include "RSDK/Storage/Storage.hpp"
#include "RSDK/Core/Math.hpp"
#include "RSDK/Storage/Text.hpp"
#include "RSDK/Core/Reader.hpp"
#include "RSDK/Graphics/Animation.hpp"
#include "RSDK/Audio/Audio.hpp"
#include "RSDK/Input/Input.hpp"
#include "RSDK/Scene/Object.hpp"
#include "RSDK/Graphics/Palette.hpp"
#include "RSDK/Graphics/Drawing.hpp"
#include "RSDK/Graphics/Scene3D.hpp"
#include "RSDK/Scene/Collision.hpp"
#include "RSDK/Scene/Scene.hpp"
#include "RSDK/Graphics/Sprite.hpp"
#include "RSDK/Graphics/Video.hpp"
#include "RSDK/User/Core/UserAchievements.hpp"
#include "RSDK/User/Core/UserLeaderboards.hpp"
#include "RSDK/User/Core/UserStats.hpp"
#include "RSDK/User/Core/UserPresence.hpp"
#include "RSDK/User/Core/UserStorage.hpp"
#include "RSDK/User/Core/UserCore.hpp"
#include "RSDK/Dev/Debug.hpp"
#include "RSDK/Core/Link.hpp"
#if RETRO_USE_MOD_LOADER
#include "RSDK/Core/ModAPI.hpp"
#endif

// APIs

// Dummy API
#include "RSDK/User/Dummy/DummyCore.hpp"

// Steam API
#include "RSDK/User/Steam/SteamCore.hpp"

// Epic Games API
#include "RSDK/User/EOS/EOSCore.hpp"

// Switch API
#include "RSDK/User/NX/NXCore.hpp"

// Default Objects
#include "RSDK/Scene/Objects/DefaultObject.hpp"
#if RETRO_REV02
#include "RSDK/Scene/Objects/DevOutput.hpp"
#endif

struct RetroEngine {
    RetroEngine() {}

#if RETRO_STANDALONE
    bool32 useExternalCode = true;
#else
    bool32 useExternalCode = false;
#endif

    bool32 devMenu        = false;
    bool32 consoleEnabled = false;

    bool32 hasPlus     = true;  // are sonic mania plus features enabled?
    bool32 confirmFlip = false; // swaps A/B, used for nintendo and etc controllers
    bool32 XYFlip      = false; // swaps X/Y, used for nintendo and etc controllers

    uint8 focusState = 0;

    bool32 initialized = false;
    bool32 hardPause   = false;

    int32 storedShaderID      = SHADER_NONE;
    int32 storedState         = ENGINESTATE_LOAD;
    int32 gameSpeed           = 1;
    int32 fastForwardSpeed    = 8;
    bool32 frameStep          = false;
    bool32 showPaletteOverlay = false;
    bool32 drawLayerVisible[DRAWGROUP_COUNT];

    // Image/Video support
    double displayTime           = 0.0;
    double videoStartDelay       = 0.0;
    double imageDelta            = 0.0;
    bool32 (*skipCallback)(void) = NULL;

    bool32 streamsEnabled = true;
    float streamVolume    = 1.0;
    float soundFXVolume   = 1.0;
};

extern RetroEngine engine;

#if RETRO_REV02
typedef void (*LogicLinkHandle)(RSDK::GameInfo *info);
#else
typedef void (*LogicLinkHandle)(RSDK::GameInfo info);
#endif

extern LogicLinkHandle linkGameLogic;

int32 RunRetroEngine(int32 argc, char *argv[]);
void ProcessEngine();

void ParseArguments(int32 argc, char *argv[]);

void StartGameObjects();

#if RETRO_USE_MOD_LOADER
void LoadXMLObjects();
void LoadXMLSoundFX();
int32 LoadXMLStages(int32 mode, int32 gcListCount, int32 gcStageCount);
#endif

void LoadGameConfig();
void InitGameLink();

void ProcessDebugCommands();

inline void SetEngineState(uint8 state)
{
    bool32 stepOver = (sceneInfo.state & ENGINESTATE_STEPOVER) == ENGINESTATE_STEPOVER;
    sceneInfo.state = state;
    if (stepOver)
        sceneInfo.state |= ENGINESTATE_STEPOVER;
}

extern int32 *globalVarsPtr;

inline void RegisterGlobalVariables(void **globals, int32 size)
{
    RSDK::AllocateStorage(size, globals, RSDK::DATASET_STG, true);
    globalVarsPtr = (int32 *)*globals;
}

#include "Link.hpp"

#endif //! RETROENGINE_H
