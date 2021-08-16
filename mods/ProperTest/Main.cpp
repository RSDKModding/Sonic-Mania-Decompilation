#include "ProperTest.h"
#include "GameOptions.h"

Hitbox defaultHitbox;

GlobalVariables *globals;

SceneInfo *RSDK_sceneInfo = NULL;
EngineInfo *RSDK_info;
#if RETRO_USE_PLUS
SKUInfo *RSDK_sku = NULL;
#endif
ControllerState *RSDK_controller = NULL;
AnalogState *RSDK_stickL         = NULL;
#if RETRO_USE_PLUS
AnalogState *RSDK_stickR    = NULL;
TriggerState *RSDK_triggerL = NULL;
TriggerState *RSDK_triggerR = NULL;
#endif
TouchMouseData *RSDK_touchMouse = NULL;
#if RETRO_USE_PLUS
UnknownInfo *RSDK_unknown = NULL;
#endif
ScreenInfo *RSDK_screens = NULL;

RSDKFunctionTable RSDK;
#if RETRO_USE_PLUS
APIFunctionTable API;
#endif
#if RETRO_USE_MOD_LOADER
ModFunctionTable Mod;
#endif

bool32 LinkModLogic(GameInfo* info, const char* id) {
#if RETRO_USE_PLUS
    memset(&API, 0, sizeof(APIFunctionTable));
#endif
    memset(&RSDK, 0, sizeof(RSDKFunctionTable));

    if (info->functionPtrs)
        memcpy(&RSDK, info->functionPtrs, sizeof(RSDKFunctionTable));
#if RETRO_USE_PLUS
    if (info->APIPtrs)
        memcpy(&API, info->APIPtrs, sizeof(APIFunctionTable));
#endif
#if RETRO_USE_MOD_LOADER
    if (info->modPtrs)
        memcpy(&Mod, info->modPtrs, sizeof(ModFunctionTable));
#endif

    RSDK_info = info->engineInfo; // GameInfo
#if RETRO_USE_PLUS
    RSDK_sku = info->currentSKU;
#endif
    RSDK_sceneInfo  = info->sceneInfo; // StageInfo
    RSDK_controller = info->controller;
    RSDK_stickL     = info->stickL;
#if RETRO_USE_PLUS
    RSDK_stickR   = info->stickR;
    RSDK_triggerL = info->triggerL;
    RSDK_triggerR = info->triggerR;
#endif
    RSDK_touchMouse = info->touchMouse;
#if RETRO_USE_PLUS
    RSDK_unknown = info->unknown;
#endif
    RSDK_screens = info->screenInfo;
    globals = (GlobalVariables*)Mod.GetGlobals();

    defaultHitbox.left   = -10;
    defaultHitbox.top    = -20;
    defaultHitbox.right  = 10;
    defaultHitbox.bottom = 20;

    //they won
    //Mod.RegisterObject((Object **)&Spikes, "Spikes", sizeof(EntitySpikes), sizeof(ObjectSpikes), Spikes_Update, 0, 0, 0, 0, Spikes_StageLoad, 0, 0, 0, 0);
    Mod.RegisterObject(NULL, "TitleCard", sizeof(EntityTitleCard), 0, 0, 0, 0, TitleCard_Draw, TitleCard_Create, 0, 0, 0, 0, 0);
    LoadFromSettings(id);
    return true;
}