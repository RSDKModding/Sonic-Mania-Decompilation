#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

#define VER_100 0 // 1.00 (initial console release)
#define VER_103 3 // 1.03 (pc release/ console patch)
#define VER_105 5 // 1.04/1.05
#define VER_106 6 // 1.06 (steam denuvo removal update)
#define VER_107 7 // 1.07 (EGS/Origin releases)

#define RETRO_GAMEVER VER_106

#define RETRO_USE_PLUS (RETRO_GAMEVER >= VER_105)
#define RETRO_USE_EGS  (RETRO_GAMEVER == VER_107)

#define RETRO_USE_MOD_LOADER (1)

// used to manage standalone (RSDKv5.exe & Game.dll) and combined (Game.exe) modes
#ifndef RETRO_STANDALONE
#define RETRO_STANDALONE (1)
#endif

// slightly misleading, this controls adding touch controls to players
#define RETRO_USE_TOUCH_CONTROLS (1)

#include "GameLink.h"

#ifdef _MSC_VER
#define DLLExport       __declspec(dllexport)
#define setAlignment(x) __declspec(align(x))
#else
#define DLLExport
#define setAlignment(x)
#endif

typedef enum {
#if RETRO_USE_PLUS
    DLC_PLUS
#endif
} GameDLC;

#include "GameOptions.h"
#include "GameObjects.h"

extern SceneInfo *RSDK_sceneInfo;
extern EngineInfo *RSDK_info;
#if RETRO_USE_PLUS
extern SKUInfo *RSDK_sku;
#endif
extern ControllerState *RSDK_controller;
extern AnalogState *RSDK_stickL;
#if RETRO_USE_PLUS
extern AnalogState *RSDK_stickR;
extern TriggerState *RSDK_triggerL;
extern TriggerState *RSDK_triggerR;
#endif
extern TouchMouseData *RSDK_touchMouse;
#if RETRO_USE_PLUS
extern UnknownInfo *RSDK_unknown;
#endif
extern ScreenInfo *RSDK_screens;

#endif //!SONIC_MANIA_H
