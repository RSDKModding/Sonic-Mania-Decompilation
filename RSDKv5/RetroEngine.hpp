#ifndef RETROENGINE_H
#define RETROENGINE_H

// Disables POSIX use c++ name blah blah stuff
#pragma warning(disable : 4996)

// ================
// STANDARD LIBS
// ================
#include <stdio.h>
#include <string.h>
#include <cmath>

// ================
// STANDARD TYPES
// ================
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned int bool32;

enum GamePlatforms {
    PLATFORM_WIN    = 0,
    PLATFORM_PS4    = 1,
    PLATFORM_XB1    = 2,
    PLATFORM_SWITCH = 3,
    PLATFORM_DEV    = 0xFF,
};

#define RETRO_WIN   (0)
#define RETRO_OSX   (1)
#define RETRO_LINUX (2)

#define RETRO_PLATFORM   (RETRO_WIN)
#define GAME_PLATFORM  (PLATFORM_WIN)

#define SCREEN_YSIZE (240)

#define BASE_PATH ""

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_OSX || RETRO_PLATFORM == RETRO_LINUX
#define RETRO_USING_SDL2 (1)
#else // Since its an else & not an elif these platforms probably aren't supported yet
#define RETRO_USING_SDL2 (0)
#endif

#if RETRO_PLATFORM == RETRO_WIN || RETRO_PLATFORM == RETRO_UWP
#if RETRO_USING_SDL2
#include <SDL.h>
#endif
#include <vorbis/vorbisfile.h>
#endif

// Utils
#include "iniparser/iniparser.h"

#include "Math.hpp"
#include "Text.hpp"
#include "Reader.hpp"
#include "Animation.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "Object.hpp"
#include "Palette.hpp"
#include "Drawing.hpp"
#include "Shader.hpp"
#include "Scene3D.hpp"
#include "Collision.hpp"
#include "Scene.hpp"
#include "Sprite.hpp"
#include "Userdata.hpp"
#include "Debug.hpp"
#include "Link.hpp"

#include "DefaultObject.hpp"
#include "DevOutput.hpp"

enum EngineStates {
    ENGINESTATE_LOAD             = 0,
    ENGINESTATE_REGULAR          = 1,
    ENGINESTATE_PAUSED           = 2,
    ENGINESTATE_FROZEN           = 3,
    ENGINESTATE_LOAD_STEPOVER    = 4,
    ENGINESTATE_REGULAR_STEPOVER = 5,
    ENGINESTATE_PAUSED_STEPOVER  = 6,
    ENGINESTATE_FROZEN_STEPOVER  = 7,
    ENGINESTATE_DEVMENU          = 8,
    ENGINESTATE_VIDEOPLAYBACK    = 9,
    ENGINESTATE_SHOWPNG          = 0x0A,
    ENGINESTATE_ERRORMSG         = 0x0B,
    ENGINESTATE_ERRORMSG_FATAL   = 0x0C,
    ENGINESTATE_NULL             = 0x0D,
};

struct RetroEngine {
    RetroEngine() {}
    char gameName[0x80];
    char gameSubName[0x80];
    char gameVersion[0x20];

    bool useExternalCode = true;

    bool devMenu   = false;
    bool printConsole = true;
    int startList  = 0;
    int startStage = 0;

    int engineMode          = ENGINESTATE_LOAD;
    int prevEngineMode      = ENGINESTATE_LOAD;
    bool running            = false;
    int gameSpeed           = 1;
    int fastForwardSpeed    = 8;
    bool frameStep          = false;
    bool masterPaused       = false;
    bool showPaletteOverlay = false;

    bool isFullScreen = false;

    bool startFullScreen = false;
    bool borderless      = false;
    bool vsync           = false;
    int windowScale      = 2;

#if RETRO_USING_SDL2
    SDL_Window *window     = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Texture *screenBuffer[SCREEN_MAX];

    SDL_Event sdlEvents;
#endif
};

extern RetroEngine engine;

void initRetroEngine();
void runRetroEngine();

void startGameObjects();

void LoadGameConfig();
void InitScriptSystem();

inline void SetEngineState(byte state)
{
    if (state < 4) {
        engine.engineMode = state | engine.engineMode & 4;
    }
}

#include "Link.hpp"

#endif //! RETROENGINE_H
