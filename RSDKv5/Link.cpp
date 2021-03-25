#include "RetroEngine.hpp"

void *RSDKFunctionTable[FUNCTABLE_COUNT];

#if RETRO_USE_PLUS
void *userFunctionTable[UDATATABLE_COUNT];

enum UserdataTableIDs {
    UserdataTable_GetUserLanguage,
    UserdataTable_GetConfirmButtonFlip,
    UserdataTable_ExitGame,
    UserdataTable_LaunchManual,
    UserdataTable_Unknown4,
    UserdataTable_CheckDLC,
    UserdataTable_ClearAchievements,
    UserdataTable_UnlockAchievement,
    UserdataTable_GetAchievementsStatus,
    UserdataTable_SetAchievementsStatus,
    UserdataTable_LeaderboardsUnknown4,
    UserdataTable_FetchLeaderboard,
    UserdataTable_TrackScore,
    UserdataTable_LeaderboardsUnknown7,
    UserdataTable_LeaderboardEntryCount,
    UserdataTable_Missing2,
    UserdataTable_Unknown12,
    UserdataTable_Missing3,
    UserdataTable_ReadLeaderboardEntry,
    UserdataTable_SetPresence,
    UserdataTable_TryTrackStat,
    UserdataTable_GetStatsStatus,
    UserdataTable_SetStatsStatus,
    UserdataTable_Unknown16,
    UserdataTable_TryAuth,
    UserdataTable_GetUserAuthStatus,
    UserdataTable_GetUsername,
    UserdataTable_TryInitStorage,
    UserdataTable_UserStorageStatusUnknown1,
    UserdataTable_Unknown22,
    UserdataTable_Unknown23,
    UserdataTable_Unknown24,
    UserdataTable_Missing5,
    UserdataTable_Unknown25,
    UserdataTable_Unknown26,
    UserdataTable_SetUserStorageNoSave,
    UserdataTable_GetUserStorageNoSave,
    UserdataTable_LoadUserFile,
    UserdataTable_SaveUserFile,
    UserdataTable_DeleteUserFile,
    UserdataTable_AddUserDBEntry,
    UserdataTable_OpenUserDB,
    UserdataTable_SaveUserDB,
    UserdataTable_Unknown30,
    UserdataTable_Missing6,
    UserdataTable_Unknown31,
    UserdataTable_Unknown32,
    UserdataTable_Unknown33,
    UserdataTable_Unknown34,
    UserdataTable_Unknown35,
    UserdataTable_Unknown36,
    UserdataTable_Unknown37,
    UserdataTable_Unknown38,
    UserdataTable_Unknown39,
    UserdataTable_AddUserDB,
    UserdataTable_Unknown40,
    UserdataTable_GetTime,
    UserdataTable_Unknown42,
    UserdataTable_Unknown43,
    UserdataTable_Count,
};
#endif

enum FunctionTableIDs {
    FunctionTable_InitGameOptions,
    FunctionTable_CreateObject,
#if RETRO_USE_PLUS
    FunctionTable_CreateObjectContainer,
#endif
    FunctionTable_GetActiveEntities,
    FunctionTable_GetEntities,
    FunctionTable_BreakForeachLoop,
    FunctionTable_SetEditableVar,
    FunctionTable_GetObjectByID,
    FunctionTable_GetEntityID,
    FunctionTable_GetEntityCount,
    FunctionTable_GetDrawListRef,
    FunctionTable_GetDrawListRefPtr,
    FunctionTable_DestroyEntity,
    FunctionTable_ResetEntity,
    FunctionTable_SpawnEntity,
    FunctionTable_CopyEntity,
    FunctionTable_CheckOnScreen,
    FunctionTable_CheckPosOnScreen,
    FunctionTable_AddDrawListRef,
    FunctionTable_SwapDrawLayers,
    FunctionTable_SetDrawLayerProperties,
    FunctionTable_LoadScene,
    FunctionTable_SetGameMode,
#if RETRO_USE_PLUS
    FunctionTable_SetHardResetFlag,
#endif
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectByName,
    FunctionTable_ClearScreens,
    FunctionTable_AddScreen,
#if !RETRO_USE_PLUS
    FunctionTable_GetFuncPtr,
#endif
    FunctionTable_GetSettingsValue,
    FunctionTable_SetSettingsValue,
    FunctionTable_UpdateWindow,
    FunctionTable_Sin1024,
    FunctionTable_Cos1024,
    FunctionTable_ATan1024,
    FunctionTable_ASin1024,
    FunctionTable_ACos1024,
    FunctionTable_Sin512,
    FunctionTable_Cos512,
    FunctionTable_ATan512,
    FunctionTable_ASin512,
    FunctionTable_ACos512,
    FunctionTable_Sin256,
    FunctionTable_Cos256,
    FunctionTable_ATan256,
    FunctionTable_ASin256,
    FunctionTable_ACos256,
    FunctionTable_Rand,
    FunctionTable_Random,
    FunctionTable_SetRandKey,
    FunctionTable_ATan2,
    FunctionTable_SetIdentityMatrix,
    FunctionTable_MatrixMultiply,
    FunctionTable_MatrixTranslateXYZ,
    FunctionTable_MatrixScaleXYZ,
    FunctionTable_MatrixRotateX,
    FunctionTable_MatrixRotateY,
    FunctionTable_MatrixRotateZ,
    FunctionTable_MatrixRotateXYZ,
    FunctionTable_MatrixInverse,
    FunctionTable_MatrixCopy,
    FunctionTable_SetText,
    FunctionTable_CopyString,
    FunctionTable_PrependString,
    FunctionTable_AppendString,
    FunctionTable_Unknown67,
    FunctionTable_LoadStrings,
    FunctionTable_SplitStringList,
    FunctionTable_GetCString,
    FunctionTable_StringCompare,
    FunctionTable_GetWindowSettings,
    FunctionTable_Unknown71,
    FunctionTable_SetScreenSize,
    FunctionTable_SetClipBounds,
#if RETRO_USE_PLUS
    FunctionTable_SetScreenUnknown,
#endif
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetLookupTable,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    FunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
#if RETRO_USE_PLUS
    FunctionTable_LoadPalette,
#endif
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
#if RETRO_USE_PLUS
    FunctionTable_BlendColours,
#endif
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawQuad,
    FunctionTable_DrawTexturedQuad,
    FunctionTable_DrawSprite,
    FunctionTable_DrawDeformed,
    FunctionTable_DrawText,
    FunctionTable_DrawTile,
    FunctionTable_CopyTile,
    FunctionTable_DrawAniTile,
    FunctionTable_FillScreen,
    FunctionTable_LoadMesh,
    FunctionTable_Create3DScene,
    FunctionTable_Init3DScene,
    FunctionTable_SetAmbientUnknown,
    FunctionTable_SetDiffuseUnknown,
    FunctionTable_SetSpecularUnknown,
    FunctionTable_SetupMesh,
    FunctionTable_SetModelAnimation,
    FunctionTable_SetupMeshAnimation,
    FunctionTable_Draw3DScene,
    FunctionTable_LoadAnimation,
    FunctionTable_CreateAnimation,
    FunctionTable_SetSpriteAnimation,
    FunctionTable_EditAnimation,
    FunctionTable_SetSpriteString,
    FunctionTable_GetAnimation,
    FunctionTable_GetFrame,
    FunctionTable_GetHitbox,
    FunctionTable_GetFrameID,
    FunctionTable_GetStringWidth,
    FunctionTable_ProcessAnimation,
    FunctionTable_GetSceneLayer,
    FunctionTable_GetSceneLayerID,
    FunctionTable_GetLayerSize,
    FunctionTable_GetTileInfo,
    FunctionTable_SetTileInfo,
    FunctionTable_CopyTileLayer,
    FunctionTable_ProcessParallax,
    FunctionTable_GetLinePosPtrs,
    FunctionTable_CheckObjectCollisionTouch,
    FunctionTable_CheckObjectCollisionPoint,
    FunctionTable_CheckObjectCollisionBox,
    FunctionTable_CheckObjectCollisionPlatform,
    FunctionTable_ObjectTileCollision,
    FunctionTable_ObjectTileGrip,
    FunctionTable_ProcessTileCollisions,
    FunctionTable_GetTileAngle,
    FunctionTable_SetTileAngle,
    FunctionTable_GetTileBehaviour,
    FunctionTable_SetTileBehaviour,
    FunctionTable_GetSFX,
    FunctionTable_PlaySFX,
    FunctionTable_StopSFX,
    FunctionTable_PlayMusic,
    FunctionTable_SetSoundAttributes,
    FunctionTable_Unknown91,
    FunctionTable_PauseChannel,
    FunctionTable_ResumeChannel,
    FunctionTable_SoundPlaying,
    FunctionTable_ChannelPlaying,
    FunctionTable_TrackPlaying,
    FunctionTable_LoadVideo,
    FunctionTable_LoadImage,
#if RETRO_USE_PLUS
    FunctionTable_ControllerIDForInputID,
    FunctionTable_MostRecentActiveControllerID,
    FunctionTable_Unknown100,
    FunctionTable_Unknown101,
    FunctionTable_Missing21,
    FunctionTable_Missing22,
    FunctionTable_Missing23,
    FunctionTable_Missing24,
    FunctionTable_Missing25,
    FunctionTable_Missing26,
    FunctionTable_AssignControllerID,
    FunctionTable_InputIDIsDisconnected,
    FunctionTable_ResetControllerAssignments,
#endif
#if !RETRO_USE_PLUS
    FunctionTable_Unknown92,
#endif
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
#if RETRO_USE_PLUS
    FunctionTable_printLog,
    FunctionTable_printString,
    FunctionTable_printText,
    FunctionTable_printIntegerUnsigned,
    FunctionTable_printInteger,
    FunctionTable_printFloat,
    FunctionTable_printVector2,
    FunctionTable_printHitbox,
#endif
    FunctionTable_Unknown105,
    FunctionTable_Unknown106,
#if RETRO_USE_PLUS
    FunctionTable_ClearDebugValues,
    FunctionTable_SetDebugValue,
#endif
#if !RETRO_USE_PLUS
    FunctionTable_PrintMessage,
#endif
    FunctionTable_Count,
};

#if RETRO_USE_PLUS
SKUInfo curSKU;
UnknownInfo unknownInfo;
#else
EngineInfo engineInfo;
#endif

void NullFunc() {}

#define addToRSDKFunctionTable(id, func) RSDKFunctionTable[id] = (void *)func;
#define addToUserFunctionTable(id, func) userFunctionTable[id] = (void *)func;

void setupFunctions()
{
#if RETRO_USE_PLUS
    curSKU.platform = PLATFORM_DEV;
    curSKU.language = 0;
    curSKU.region   = 0;
#else
    engineInfo.platform = GAME_PLATFORM;
    engineInfo.language = 0;
    engineInfo.region   = 0;
#endif

    CalculateTrigAngles();
    GenerateBlendLookupTable();
    InitGFXSystem();

    memset(RSDKFunctionTable, NULL, FUNCTABLE_COUNT * sizeof(void *));
#if RETRO_USE_PLUS
    memset(userFunctionTable, NULL, UDATATABLE_COUNT * sizeof(void *));
#endif

#if RETRO_USE_PLUS
    // Userdata
    userFunctionTable[UserdataTable_GetUserLanguage]           = (void *)userCore->GetUserLanguage;
    userFunctionTable[UserdataTable_GetConfirmButtonFlip]      = (void *)userCore->GetConfirmButtonFlip;
    userFunctionTable[UserdataTable_ExitGame]                  = (void *)userCore->ExitGame;
    userFunctionTable[UserdataTable_LaunchManual]              = (void *)userCore->LaunchManual;
    userFunctionTable[UserdataTable_Unknown4]                  = (void *)userCore->unknown15;
    userFunctionTable[UserdataTable_CheckDLC]                  = (void *)userCore->CheckDLC;
    userFunctionTable[UserdataTable_ClearAchievements]         = (void *)userCore->ShowExtensionOverlay;
    userFunctionTable[UserdataTable_UnlockAchievement]         = (void *)achievements->UnlockAchievement;
    userFunctionTable[UserdataTable_GetAchievementsStatus]     = (void *)GetAchievementsStatus;
    userFunctionTable[UserdataTable_SetAchievementsStatus]     = (void *)SetAchievementsStatus;
    userFunctionTable[UserdataTable_LeaderboardsUnknown4]      = (void *)NullFunc;
    userFunctionTable[UserdataTable_FetchLeaderboard]          = (void *)leaderboards->FetchLeaderboard;
    userFunctionTable[UserdataTable_TrackScore]                = (void *)leaderboards->TrackScore;
    userFunctionTable[UserdataTable_LeaderboardsUnknown7]      = (void *)leaderboards->unknown7;
    userFunctionTable[UserdataTable_LeaderboardEntryCount]     = (void *)NullFunc;
    userFunctionTable[UserdataTable_Missing2]                  = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown12]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Missing3]                  = (void *)NullFunc;
    userFunctionTable[UserdataTable_ReadLeaderboardEntry]      = (void *)NullFunc;
    userFunctionTable[UserdataTable_SetPresence]               = (void *)richPresence->SetPresence;
    userFunctionTable[UserdataTable_TryTrackStat]              = (void *)stats->TryTrackStat;
    userFunctionTable[UserdataTable_GetStatsStatus]            = (void *)GetStatsStatus;
    userFunctionTable[UserdataTable_SetStatsStatus]            = (void *)SetStatsStatus;
    userFunctionTable[UserdataTable_Unknown16]                 = (void *)userStorage->unknown8;
    userFunctionTable[UserdataTable_TryAuth]                   = (void *)userStorage->TryAuth;
    userFunctionTable[UserdataTable_GetUserAuthStatus]         = (void *)GetUserStorageStatus;
    userFunctionTable[UserdataTable_GetUsername]               = (void *)userStorage->GetUsername;
    userFunctionTable[UserdataTable_TryInitStorage]            = (void *)userStorage->TryInitStorage;
    userFunctionTable[UserdataTable_UserStorageStatusUnknown1] = (void *)UserStorageStatusUnknown1;
    userFunctionTable[UserdataTable_Unknown22]                 = (void *)UserStorageStatusUnknown2;
    userFunctionTable[UserdataTable_Unknown23]                 = (void *)ClearUserStorageStatus;
    userFunctionTable[UserdataTable_Unknown24]                 = (void *)SetUserStorageStatus;
    userFunctionTable[UserdataTable_Missing5]                  = (void *)UserStorageStatusUnknown3;
    userFunctionTable[UserdataTable_Unknown25]                 = (void *)UserStorageStatusUnknown4;
    userFunctionTable[UserdataTable_Unknown26]                 = (void *)UserStorageStatusUnknown5;
    userFunctionTable[UserdataTable_SetUserStorageNoSave]      = (void *)SetUserStorageNoSave;
    userFunctionTable[UserdataTable_GetUserStorageNoSave]      = (void *)GetUserStorageNoSave;
    userFunctionTable[UserdataTable_LoadUserFile]              = (void *)userStorage->LoadUserFile;
    userFunctionTable[UserdataTable_SaveUserFile]              = (void *)userStorage->SaveUserFile;
    userFunctionTable[UserdataTable_DeleteUserFile]            = (void *)userStorage->DeleteUserFile;
    userFunctionTable[UserdataTable_AddUserDBEntry]            = (void *)NullFunc;
    userFunctionTable[UserdataTable_OpenUserDB]                = (void *)NullFunc;
    userFunctionTable[UserdataTable_SaveUserDB]                = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown30]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Missing6]                  = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown31]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown32]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown33]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown34]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown35]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown36]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown37]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown38]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown39]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_AddUserDB]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown40]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_GetTime]                   = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown42]                 = (void *)NullFunc;
    userFunctionTable[UserdataTable_Unknown43]                 = (void *)NullFunc;
#endif

    // Function Table
    RSDKFunctionTable[FunctionTable_InitGameOptions] = (void *)InitGameOptions;
    RSDKFunctionTable[FunctionTable_CreateObject]    = (void *)CreateObject;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_CreateObjectContainer] = (void *)CreateObjectContainer;
#endif
    RSDKFunctionTable[FunctionTable_GetActiveEntities]      = (void *)GetActiveEntities;
    RSDKFunctionTable[FunctionTable_GetEntities]            = (void *)GetEntities;
    RSDKFunctionTable[FunctionTable_BreakForeachLoop]       = (void *)BreakForeachLoop;
    RSDKFunctionTable[FunctionTable_SetEditableVar]         = (void *)SetEditableVar;
    RSDKFunctionTable[FunctionTable_GetObjectByID]          = (void *)GetObjectByID;
    RSDKFunctionTable[FunctionTable_GetEntityID]            = (void *)GetEntityID;
    RSDKFunctionTable[FunctionTable_GetEntityCount]         = (void *)GetEntityCount;
    RSDKFunctionTable[FunctionTable_GetDrawListRef]         = (void *)GetDrawListRef;
    RSDKFunctionTable[FunctionTable_GetDrawListRefPtr]      = (void *)GetDrawListRefPtr;
    RSDKFunctionTable[FunctionTable_DestroyEntity]          = (void *)ResetEntityPtr;
    RSDKFunctionTable[FunctionTable_ResetEntity]            = (void *)ResetEntitySlot;
    RSDKFunctionTable[FunctionTable_SpawnEntity]            = (void *)CreateEntity;
    RSDKFunctionTable[FunctionTable_CopyEntity]             = (void *)CopyEntity;
    RSDKFunctionTable[FunctionTable_CheckOnScreen]          = (void *)CheckOnScreen;
    RSDKFunctionTable[FunctionTable_CheckPosOnScreen]       = (void *)CheckPosOnScreen;
    RSDKFunctionTable[FunctionTable_AddDrawListRef]         = (void *)AddDrawListRef;
    RSDKFunctionTable[FunctionTable_SwapDrawLayers]         = (void *)SwapDrawLayers;
    RSDKFunctionTable[FunctionTable_SetDrawLayerProperties] = (void *)SetDrawLayerProperties;
    RSDKFunctionTable[FunctionTable_LoadScene]              = (void *)SetScene;
    RSDKFunctionTable[FunctionTable_SetGameMode]            = (void *)SetEngineState;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_SetHardResetFlag] = (void *)SetHardResetFlag;
#endif
    RSDKFunctionTable[FunctionTable_CheckValidScene]  = (void *)CheckValidStage;
    RSDKFunctionTable[FunctionTable_CheckSceneFolder] = (void *)CheckSceneFolder;
    RSDKFunctionTable[FunctionTable_InitSceneLoad]    = (void *)InitSceneLoad;
    RSDKFunctionTable[FunctionTable_GetObjectByName]  = (void *)GetObjectByName;
    RSDKFunctionTable[FunctionTable_ClearScreens]     = (void *)ClearScreens;
    RSDKFunctionTable[FunctionTable_AddScreen]        = (void *)AddScreen;
#if !RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_GetFuncPtr] = (void *)GetFuncPtr;
#endif
    RSDKFunctionTable[FunctionTable_GetSettingsValue]   = (void *)GetSettingsValue;
    RSDKFunctionTable[FunctionTable_SetSettingsValue]   = (void *)SetSettingsValue;
    RSDKFunctionTable[FunctionTable_UpdateWindow]       = (void *)UpdateWindow;
    RSDKFunctionTable[FunctionTable_Sin1024]            = (void *)sin1024;
    RSDKFunctionTable[FunctionTable_Cos1024]            = (void *)cos1024;
    RSDKFunctionTable[FunctionTable_ATan1024]           = (void *)tan1024;
    RSDKFunctionTable[FunctionTable_ASin1024]           = (void *)aSin1024;
    RSDKFunctionTable[FunctionTable_ACos1024]           = (void *)aCos1024;
    RSDKFunctionTable[FunctionTable_Sin512]             = (void *)sin512;
    RSDKFunctionTable[FunctionTable_Cos512]             = (void *)cos512;
    RSDKFunctionTable[FunctionTable_ATan512]            = (void *)tan512;
    RSDKFunctionTable[FunctionTable_ASin512]            = (void *)aSin512;
    RSDKFunctionTable[FunctionTable_ACos512]            = (void *)aCos512;
    RSDKFunctionTable[FunctionTable_Sin256]             = (void *)sin256;
    RSDKFunctionTable[FunctionTable_Cos256]             = (void *)cos256;
    RSDKFunctionTable[FunctionTable_ATan256]            = (void *)tan256;
    RSDKFunctionTable[FunctionTable_ASin256]            = (void *)aSin256;
    RSDKFunctionTable[FunctionTable_ACos256]            = (void *)aCos256;
    RSDKFunctionTable[FunctionTable_Rand]               = (void *)RSDK_random;
    RSDKFunctionTable[FunctionTable_Random]             = (void *)RSDK_random2;
    RSDKFunctionTable[FunctionTable_SetRandKey]         = (void *)setRandKey;
    RSDKFunctionTable[FunctionTable_ATan2]              = (void *)ArcTanLookup;
    RSDKFunctionTable[FunctionTable_SetIdentityMatrix]  = (void *)setIdentityMatrix;
    RSDKFunctionTable[FunctionTable_MatrixMultiply]     = (void *)matrixMultiply;
    RSDKFunctionTable[FunctionTable_MatrixTranslateXYZ] = (void *)matrixTranslateXYZ;
    RSDKFunctionTable[FunctionTable_MatrixScaleXYZ]     = (void *)matrixScaleXYZ;
    RSDKFunctionTable[FunctionTable_MatrixRotateX]      = (void *)matrixRotateX;
    RSDKFunctionTable[FunctionTable_MatrixRotateY]      = (void *)matrixRotateY;
    RSDKFunctionTable[FunctionTable_MatrixRotateZ]      = (void *)matrixRotateZ;
    RSDKFunctionTable[FunctionTable_MatrixRotateXYZ]    = (void *)matrixRotateXYZ;
    RSDKFunctionTable[FunctionTable_MatrixInverse]      = (void *)matrixInverse;
    RSDKFunctionTable[FunctionTable_MatrixCopy]         = (void *)matrixCopy;
    RSDKFunctionTable[FunctionTable_SetText]            = (void *)SetText;
    RSDKFunctionTable[FunctionTable_CopyString]         = (void *)CopyString;
    RSDKFunctionTable[FunctionTable_PrependString]      = (void *)NullFunc; // PrependString;
    RSDKFunctionTable[FunctionTable_AppendString]       = (void *)AppendString;
    RSDKFunctionTable[FunctionTable_Unknown67]          = (void *)NullFunc; // Unknown67;
    RSDKFunctionTable[FunctionTable_LoadStrings]        = (void *)LoadStrings;
    RSDKFunctionTable[FunctionTable_SplitStringList]    = (void *)SplitStringList;
    RSDKFunctionTable[FunctionTable_GetCString]         = (void *)GetCString;
    RSDKFunctionTable[FunctionTable_StringCompare]      = (void *)StringCompare;
    RSDKFunctionTable[FunctionTable_GetWindowSettings]  = (void *)NullFunc; // Unknown70;
    RSDKFunctionTable[FunctionTable_Unknown71]          = (void *)NullFunc; // Unknown71;
    RSDKFunctionTable[FunctionTable_SetScreenSize]      = (void *)SetScreenSize;
    RSDKFunctionTable[FunctionTable_SetClipBounds]      = (void *)SetClipBounds;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_SetScreenUnknown] = (void *)NullFunc; // SetScreenUnknown;
#endif
    RSDKFunctionTable[FunctionTable_LoadSpriteSheet]  = (void *)LoadSpriteSheet;
    RSDKFunctionTable[FunctionTable_SetLookupTable]   = (void *)SetLookupTable;
    RSDKFunctionTable[FunctionTable_SetPaletteMask]   = (void *)SetPaletteMask;
    RSDKFunctionTable[FunctionTable_SetPaletteEntry]  = (void *)SetPaletteEntry;
    RSDKFunctionTable[FunctionTable_GetPaletteEntry]  = (void *)GetPaletteEntry;
    RSDKFunctionTable[FunctionTable_SetActivePalette] = (void *)SetActivePalette;
    RSDKFunctionTable[FunctionTable_CopyPalette]      = (void *)CopyPalette;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_LoadPalette] = (void *)LoadPalette;
#endif
    RSDKFunctionTable[FunctionTable_RotatePalette]  = (void *)RotatePalette;
    RSDKFunctionTable[FunctionTable_SetLimitedFade] = (void *)SetPaletteFade;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_BlendColours] = (void *)BlendColours;
#endif
    RSDKFunctionTable[FunctionTable_DrawRect]                     = (void *)DrawRectangle;
    RSDKFunctionTable[FunctionTable_DrawLine]                     = (void *)DrawLine;
    RSDKFunctionTable[FunctionTable_DrawCircle]                   = (void *)DrawCircle;
    RSDKFunctionTable[FunctionTable_DrawCircleOutline]            = (void *)DrawCircleOutline;
    RSDKFunctionTable[FunctionTable_DrawQuad]                     = (void *)DrawQuad;
    RSDKFunctionTable[FunctionTable_DrawTexturedQuad]             = (void *)DrawBlendedQuad;
    RSDKFunctionTable[FunctionTable_DrawSprite]                   = (void *)DrawSprite;
    RSDKFunctionTable[FunctionTable_DrawDeformed]                 = (void *)DrawDeformedSprite;
    RSDKFunctionTable[FunctionTable_DrawText]                     = (void *)DrawText;
    RSDKFunctionTable[FunctionTable_DrawTile]                     = (void *)DrawTile;
    RSDKFunctionTable[FunctionTable_CopyTile]                     = (void *)CopyTile;
    RSDKFunctionTable[FunctionTable_DrawAniTile]                  = (void *)DrawAniTile;
    RSDKFunctionTable[FunctionTable_FillScreen]                   = (void *)FillScreen;
    RSDKFunctionTable[FunctionTable_LoadMesh]                     = (void *)LoadMesh;
    RSDKFunctionTable[FunctionTable_Create3DScene]                = (void *)Create3DScene;
    RSDKFunctionTable[FunctionTable_Init3DScene]                  = (void *)Init3DScene;
    RSDKFunctionTable[FunctionTable_SetAmbientUnknown]            = (void *)SetAmbientUnknown;
    RSDKFunctionTable[FunctionTable_SetDiffuseUnknown]            = (void *)SetDiffuseUnknown;
    RSDKFunctionTable[FunctionTable_SetSpecularUnknown]           = (void *)SetSpecularUnknown;
    RSDKFunctionTable[FunctionTable_SetupMesh]                    = (void *)SetupMesh;
    RSDKFunctionTable[FunctionTable_SetModelAnimation]            = (void *)SetModelAnimation;
    RSDKFunctionTable[FunctionTable_SetupMeshAnimation]           = (void *)SetupMeshAnimation;
    RSDKFunctionTable[FunctionTable_Draw3DScene]                  = (void *)Draw3DScene; // Draw3DScene;
    RSDKFunctionTable[FunctionTable_LoadAnimation]                = (void *)LoadAnimation;
    RSDKFunctionTable[FunctionTable_CreateAnimation]              = (void *)CreateAnimation;
    RSDKFunctionTable[FunctionTable_SetSpriteAnimation]           = (void *)SetSpriteAnimation;
    RSDKFunctionTable[FunctionTable_EditAnimation]                = (void *)EditAnimation;
    RSDKFunctionTable[FunctionTable_SetSpriteString]              = (void *)SetSpriteString;
    RSDKFunctionTable[FunctionTable_GetAnimation]                 = (void *)GetAnimation;
    RSDKFunctionTable[FunctionTable_GetFrame]                     = (void *)GetFrame;
    RSDKFunctionTable[FunctionTable_GetHitbox]                    = (void *)GetHitbox;
    RSDKFunctionTable[FunctionTable_GetFrameID]                   = (void *)GetFrameID;
    RSDKFunctionTable[FunctionTable_GetStringWidth]               = (void *)GetStringWidth;
    RSDKFunctionTable[FunctionTable_ProcessAnimation]             = (void *)ProcessAnimation;
    RSDKFunctionTable[FunctionTable_GetSceneLayer]                = (void *)GetSceneLayer;
    RSDKFunctionTable[FunctionTable_GetSceneLayerID]              = (void *)GetSceneLayerID;
    RSDKFunctionTable[FunctionTable_GetLayerSize]                 = (void *)GetLayerSize;
    RSDKFunctionTable[FunctionTable_GetTileInfo]                  = (void *)GetTileInfo;
    RSDKFunctionTable[FunctionTable_SetTileInfo]                  = (void *)SetTileInfo;
    RSDKFunctionTable[FunctionTable_CopyTileLayer]                = (void *)CopyTileLayout;
    RSDKFunctionTable[FunctionTable_ProcessParallax]              = (void *)ProcessParallax;
    RSDKFunctionTable[FunctionTable_GetLinePosPtrs]               = (void *)GetLinePositions;
    RSDKFunctionTable[FunctionTable_CheckObjectCollisionTouch]    = (void *)CheckObjectCollisionTouch;
    RSDKFunctionTable[FunctionTable_CheckObjectCollisionPoint]    = (void *)CheckObjectCollisionCircle;
    RSDKFunctionTable[FunctionTable_CheckObjectCollisionBox]      = (void *)CheckObjectCollisionBox;
    RSDKFunctionTable[FunctionTable_CheckObjectCollisionPlatform] = (void *)CheckObjectCollisionPlatform;
    RSDKFunctionTable[FunctionTable_ObjectTileCollision]          = (void *)ObjectTileCollision;
    RSDKFunctionTable[FunctionTable_ObjectTileGrip]               = (void *)ObjectTileGrip;
    RSDKFunctionTable[FunctionTable_ProcessTileCollisions]        = (void *)ProcessTileCollisions;
    RSDKFunctionTable[FunctionTable_GetTileAngle]                 = (void *)GetTileAngle;
    RSDKFunctionTable[FunctionTable_SetTileAngle]                 = (void *)SetTileAngle;
    RSDKFunctionTable[FunctionTable_GetTileBehaviour]             = (void *)GetTileBehaviour;
    RSDKFunctionTable[FunctionTable_SetTileBehaviour]             = (void *)SetTileBehaviour;
    RSDKFunctionTable[FunctionTable_GetSFX]                       = (void *)GetSFX;
    RSDKFunctionTable[FunctionTable_PlaySFX]                      = (void *)PlaySfx;
    RSDKFunctionTable[FunctionTable_StopSFX]                      = (void *)StopSfx;
    RSDKFunctionTable[FunctionTable_PlayMusic]                    = (void *)NullFunc; // PlayMusicFile;
    RSDKFunctionTable[FunctionTable_SetSoundAttributes]           = (void *)SetSoundAttributes;
    RSDKFunctionTable[FunctionTable_Unknown91]                    = (void *)SoundUnknown1;
    RSDKFunctionTable[FunctionTable_PauseChannel]                 = (void *)PauseChannel;
    RSDKFunctionTable[FunctionTable_ResumeChannel]                = (void *)ResumeChannel;
    RSDKFunctionTable[FunctionTable_SoundPlaying]                 = (void *)soundPlaying;
    RSDKFunctionTable[FunctionTable_ChannelPlaying]               = (void *)channelPlaying;
    RSDKFunctionTable[FunctionTable_TrackPlaying]                 = (void *)trackPlaying;
    RSDKFunctionTable[FunctionTable_LoadVideo]                    = (void *)NullFunc; // LoadVideo;
    RSDKFunctionTable[FunctionTable_LoadImage]                    = (void *)LoadImage;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_ControllerIDForInputID]       = (void *)NullFunc; // Unknown98;
    RSDKFunctionTable[FunctionTable_MostRecentActiveControllerID] = (void *)NullFunc; // Unknown99;
    RSDKFunctionTable[FunctionTable_Unknown100]                   = (void *)NullFunc; // Unknown100;
    RSDKFunctionTable[FunctionTable_Unknown101]                   = (void *)NullFunc; // Unknown101;
    RSDKFunctionTable[FunctionTable_Missing21]                    = (void *)NullFunc; // UserDataUnknown1;
    RSDKFunctionTable[FunctionTable_Missing22]                    = (void *)NullFunc; // UserDataUnknown2;
    RSDKFunctionTable[FunctionTable_Missing23]                    = (void *)NullFunc; // UserDataUnknown3;
    RSDKFunctionTable[FunctionTable_Missing24]                    = (void *)NullFunc; // UserDataUnknown4;
    RSDKFunctionTable[FunctionTable_Missing25]                    = (void *)NullFunc; // UserDataUnknown5;
    RSDKFunctionTable[FunctionTable_Missing26]                    = (void *)NullFunc; // UserDataUnknown6;
    RSDKFunctionTable[FunctionTable_AssignControllerID]           = (void *)NullFunc; // Unknown102;
    RSDKFunctionTable[FunctionTable_InputIDIsDisconnected]        = (void *)NullFunc; // Unknown103;
    RSDKFunctionTable[FunctionTable_ResetControllerAssignments]   = (void *)NullFunc; // Unknown104;
#endif
#if !RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_Unknown92] = (void *)NullFunc;
#endif
    RSDKFunctionTable[FunctionTable_LoadUserFile] = (void*)LoadUserFile;
    RSDKFunctionTable[FunctionTable_SaveUserFile] = (void*)SaveUserFile;
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_printLog]             = (void *)printLog;
    RSDKFunctionTable[FunctionTable_printString]          = (void *)printString;
    RSDKFunctionTable[FunctionTable_printText]            = (void *)printText;
    RSDKFunctionTable[FunctionTable_printIntegerUnsigned] = (void *)printIntegerUnsigned;
    RSDKFunctionTable[FunctionTable_printInteger]         = (void *)printInteger;
    RSDKFunctionTable[FunctionTable_printFloat]           = (void *)printFloat;
    RSDKFunctionTable[FunctionTable_printVector2]         = (void *)printVector2;
    RSDKFunctionTable[FunctionTable_printHitbox]          = (void *)printHitbox;
#endif
    RSDKFunctionTable[FunctionTable_Unknown105] = (void *)NullFunc; // UserDataUnknown7;
    RSDKFunctionTable[FunctionTable_Unknown106] = (void *)NullFunc; // UserDataUnknown8;
#if !RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_PrintMessage] = (void *)PrintMessage;
#endif
#if RETRO_USE_PLUS
    RSDKFunctionTable[FunctionTable_ClearDebugValues] = (void *)ClearDebugValues;
    RSDKFunctionTable[FunctionTable_SetDebugValue]    = (void *)SetDebugValue;
#endif
}

void LinkGameLogic(GameInfo *info) { printLog(SEVERITY_WARN, "Internal LinkGameLogic() function called, no logic will be linked"); }
