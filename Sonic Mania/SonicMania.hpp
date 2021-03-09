#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

#define RETRO_USE_PLUS (1) //idk, might use this, mostly just a marker for what stuff plus/1.05 changed

#include "GameLink.hpp"

#ifdef _MSC_VER
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

enum GameDLC {
#if RETRO_USE_PLUS
	DLC_PLUS
#endif
};

#include "GameOptions.hpp"
#include "GameObjects.hpp"

extern SceneInfo *RSDK_sceneInfo;
#if RETRO_USE_PLUS
extern char *RSDK_name;
extern SKUInfo *RSDK_sku;
#endif
#if !RETRO_USE_PLUS
extern EngineInfo *RSDK_info;
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

#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */
inline void Game_Print(const char *message, ...)
{
    if (!RSDK_sceneInfo->inEditor) {
        char buffer[0x100];

        // make the full string
        va_list args;
        va_start(args, message);
        vsprintf(buffer, message, args);
#if RETRO_USE_PLUS
        RSDK.PrintString(SEVERITY_NONE, buffer);
#else
        RSDK.PrintMessage((void*)message, 0);
#endif
    }
}

#endif //!SONIC_MANIA_H
