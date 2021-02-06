#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

#include "../RSDKv5/RetroEngine.hpp"

#if RETRO_PLATFORM == RETRO_WIN
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

#include "GameOptions.hpp"
#include "GameObjects.hpp"

extern SceneInfo *RSDK_SceneInfo;
extern SKUInfo *RSDK_SKU;
extern ScreenInfo *RSDK_Screens;

#endif //!SONIC_MANIA_H