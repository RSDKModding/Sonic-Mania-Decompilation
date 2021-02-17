#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

#include "../RSDKv5/RetroEngine.hpp"

#if RETRO_PLATFORM == RETRO_WIN
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

#define RETRO_USE_PLUS (0) //idk, might use this, mostly just a marker for what stuff plus/1.05 changed

enum GameDLC {
	DLC_PLUS
};

#include "GameOptions.hpp"
#include "GameObjects.hpp"

extern SceneInfo *RSDK_sceneInfo;
extern SKUInfo *RSDK_sku;
extern ScreenInfo *RSDK_screens;
extern ControllerState *RSDK_controller;
extern AnalogState *RSDK_stickL;
extern AnalogState *RSDK_stickR;
extern TriggerState *RSDK_triggerL;
extern TriggerState *RSDK_triggerR;
extern TouchMouseData *RSDK_touchMouse;

inline void Game_Print(const char *message, ...)
{
    if (!RSDK_sceneInfo->inEditor) {
        char buffer[0x100];

        // make the full string
        va_list args;
        va_start(args, message);
        vsprintf(buffer, message, args);
        RSDK.PrintString(SEVERITY_NONE, buffer);
    }
}

#endif //!SONIC_MANIA_H