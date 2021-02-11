#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

#include "../RSDKv5/RetroEngine.hpp"

#if RETRO_PLATFORM == RETRO_WIN
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

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

#endif //!SONIC_MANIA_H