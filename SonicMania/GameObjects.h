#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#ifdef _MSC_VER
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif

// -------------------------
// OBJECTS
// -------------------------

#include "All.h"

#if RETRO_REV02
DLLExport void LinkGameLogicDLL(EngineInfo *info);
#else
DLLExport void LinkGameLogicDLL(EngineInfo info);
#endif

#if RETRO_USE_MOD_LOADER
DLLExport bool32 LinkModLogic(EngineInfo *info, const char *id);
#endif

// custom-added extra little bit of info to let the engine know what RSDK version it is
DLLExport extern int32 RSDKRevision;

#endif //! GAMEOBJECTS_H
