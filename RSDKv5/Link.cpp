#include "RetroEngine.hpp"

void *functionTable[FUNCTABLE_COUNT];

#if RETRO_USE_PLUS
void *userDataTable[UDATATABLE_COUNT];

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

    memset(functionTable, NULL, FUNCTABLE_COUNT * sizeof(void *));
#if RETRO_USE_PLUS
    memset(userDataTable, NULL, UDATATABLE_COUNT * sizeof(void *));
#endif

#if RETRO_USE_PLUS
    // Userdata
    userDataTable[UserdataTable_GetUserLanguage]           = (void *)userCore->GetUserLanguage;
    userDataTable[UserdataTable_GetConfirmButtonFlip]      = (void *)userCore->GetConfirmButtonFlip;
    userDataTable[UserdataTable_ExitGame]                  = (void *)userCore->ExitGame;
    userDataTable[UserdataTable_LaunchManual]              = (void *)userCore->LaunchManual;
    userDataTable[UserdataTable_Unknown4]                  = (void *)userCore->unknown15;
    userDataTable[UserdataTable_CheckDLC]                  = (void *)userCore->CheckDLC;
    userDataTable[UserdataTable_ClearAchievements]         = (void *)userCore->ShowExtensionOverlay;
    userDataTable[UserdataTable_UnlockAchievement]         = (void *)achievements->UnlockAchievement;
    userDataTable[UserdataTable_GetAchievementsStatus]     = (void *)GetAchievementsStatus;
    userDataTable[UserdataTable_SetAchievementsStatus]     = (void *)SetAchievementsStatus;
    userDataTable[UserdataTable_LeaderboardsUnknown4]      = (void *)NullFunc;
    userDataTable[UserdataTable_FetchLeaderboard]          = (void *)leaderboards->FetchLeaderboard;
    userDataTable[UserdataTable_TrackScore]                = (void *)leaderboards->TrackScore;
    userDataTable[UserdataTable_LeaderboardsUnknown7]      = (void *)leaderboards->unknown7;
    userDataTable[UserdataTable_LeaderboardEntryCount]     = (void *)NullFunc;
    userDataTable[UserdataTable_Missing2]                  = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown12]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Missing3]                  = (void *)NullFunc;
    userDataTable[UserdataTable_ReadLeaderboardEntry]      = (void *)NullFunc;
    userDataTable[UserdataTable_SetPresence]               = (void *)richPresence->SetPresence;
    userDataTable[UserdataTable_TryTrackStat]              = (void *)stats->TryTrackStat;
    userDataTable[UserdataTable_GetStatsStatus]            = (void *)GetStatsStatus;
    userDataTable[UserdataTable_SetStatsStatus]            = (void *)SetStatsStatus;
    userDataTable[UserdataTable_Unknown16]                 = (void *)userStorage->unknown8;
    userDataTable[UserdataTable_TryAuth]                   = (void *)userStorage->TryAuth;
    userDataTable[UserdataTable_GetUserAuthStatus]         = (void *)GetUserStorageStatus;
    userDataTable[UserdataTable_GetUsername]               = (void *)userStorage->GetUsername;
    userDataTable[UserdataTable_TryInitStorage]            = (void *)userStorage->TryInitStorage;
    userDataTable[UserdataTable_UserStorageStatusUnknown1] = (void *)UserStorageStatusUnknown1;
    userDataTable[UserdataTable_Unknown22]                 = (void *)UserStorageStatusUnknown2;
    userDataTable[UserdataTable_Unknown23]                 = (void *)ClearUserStorageStatus;
    userDataTable[UserdataTable_Unknown24]                 = (void *)SetUserStorageStatus;
    userDataTable[UserdataTable_Missing5]                  = (void *)UserStorageStatusUnknown3;
    userDataTable[UserdataTable_Unknown25]                 = (void *)UserStorageStatusUnknown4;
    userDataTable[UserdataTable_Unknown26]                 = (void *)UserStorageStatusUnknown5;
    userDataTable[UserdataTable_SetUserStorageNoSave]      = (void *)SetUserStorageNoSave;
    userDataTable[UserdataTable_GetUserStorageNoSave]      = (void *)GetUserStorageNoSave;
    userDataTable[UserdataTable_LoadUserFile]              = (void *)userStorage->LoadUserFile;
    userDataTable[UserdataTable_SaveUserFile]              = (void *)userStorage->SaveUserFile;
    userDataTable[UserdataTable_DeleteUserFile]            = (void *)userStorage->DeleteUserFile;
    userDataTable[UserdataTable_AddUserDBEntry]            = (void *)NullFunc;
    userDataTable[UserdataTable_OpenUserDB]                = (void *)NullFunc;
    userDataTable[UserdataTable_SaveUserDB]                = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown30]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Missing6]                  = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown31]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown32]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown33]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown34]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown35]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown36]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown37]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown38]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown39]                 = (void *)NullFunc;
    userDataTable[UserdataTable_AddUserDB]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown40]                 = (void *)NullFunc;
    userDataTable[UserdataTable_GetTime]                   = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown42]                 = (void *)NullFunc;
    userDataTable[UserdataTable_Unknown43]                 = (void *)NullFunc;
#endif

    // Function Table
    functionTable[FunctionTable_InitGameOptions] = (void *)InitGameOptions;
    functionTable[FunctionTable_CreateObject]    = (void *)CreateObject;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_CreateObjectContainer] = (void *)CreateObjectContainer;
#endif
    functionTable[FunctionTable_GetActiveEntities]      = (void *)GetActiveEntities;
    functionTable[FunctionTable_GetEntities]            = (void *)GetEntities;
    functionTable[FunctionTable_BreakForeachLoop]       = (void *)BreakForeachLoop;
    functionTable[FunctionTable_SetEditableVar]         = (void *)SetEditableVar;
    functionTable[FunctionTable_GetObjectByID]          = (void *)GetObjectByID;
    functionTable[FunctionTable_GetEntityID]            = (void *)GetEntityID;
    functionTable[FunctionTable_GetEntityCount]         = (void *)GetEntityCount;
    functionTable[FunctionTable_GetDrawListRef]         = (void *)GetDrawListRef;
    functionTable[FunctionTable_GetDrawListRefPtr]      = (void *)GetDrawListRefPtr;
    functionTable[FunctionTable_DestroyEntity]          = (void *)ResetEntityPtr;
    functionTable[FunctionTable_ResetEntity]            = (void *)ResetEntitySlot;
    functionTable[FunctionTable_SpawnEntity]            = (void *)CreateEntity;
    functionTable[FunctionTable_CopyEntity]             = (void *)CopyEntity;
    functionTable[FunctionTable_CheckOnScreen]          = (void *)CheckOnScreen;
    functionTable[FunctionTable_CheckPosOnScreen]       = (void *)CheckPosOnScreen;
    functionTable[FunctionTable_AddDrawListRef]         = (void *)AddDrawListRef;
    functionTable[FunctionTable_SwapDrawLayers]         = (void *)SwapDrawLayers;
    functionTable[FunctionTable_SetDrawLayerProperties] = (void *)SetDrawLayerProperties;
    functionTable[FunctionTable_LoadScene]              = (void *)SetScene;
    functionTable[FunctionTable_SetGameMode]            = (void *)SetEngineState;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_SetHardResetFlag] = (void *)SetHardResetFlag;
#endif
    functionTable[FunctionTable_CheckValidScene]  = (void *)CheckValidStage;
    functionTable[FunctionTable_CheckSceneFolder] = (void *)CheckSceneFolder;
    functionTable[FunctionTable_InitSceneLoad]    = (void *)InitSceneLoad;
    functionTable[FunctionTable_GetObjectByName]  = (void *)GetObjectByName;
    functionTable[FunctionTable_ClearScreens]     = (void *)ClearScreens;
    functionTable[FunctionTable_AddScreen]        = (void *)AddScreen;
#if !RETRO_USE_PLUS
    functionTable[FunctionTable_GetFuncPtr] = (void *)GetFuncPtr;
#endif
    functionTable[FunctionTable_GetSettingsValue]   = (void *)GetSettingsValue;
    functionTable[FunctionTable_SetSettingsValue]   = (void *)SetSettingsValue;
    functionTable[FunctionTable_UpdateWindow]       = (void *)UpdateWindow;
    functionTable[FunctionTable_Sin1024]            = (void *)sin1024;
    functionTable[FunctionTable_Cos1024]            = (void *)cos1024;
    functionTable[FunctionTable_ATan1024]           = (void *)tan1024;
    functionTable[FunctionTable_ASin1024]           = (void *)aSin1024;
    functionTable[FunctionTable_ACos1024]           = (void *)aCos1024;
    functionTable[FunctionTable_Sin512]             = (void *)sin512;
    functionTable[FunctionTable_Cos512]             = (void *)cos512;
    functionTable[FunctionTable_ATan512]            = (void *)tan512;
    functionTable[FunctionTable_ASin512]            = (void *)aSin512;
    functionTable[FunctionTable_ACos512]            = (void *)aCos512;
    functionTable[FunctionTable_Sin256]             = (void *)sin256;
    functionTable[FunctionTable_Cos256]             = (void *)cos256;
    functionTable[FunctionTable_ATan256]            = (void *)tan256;
    functionTable[FunctionTable_ASin256]            = (void *)aSin256;
    functionTable[FunctionTable_ACos256]            = (void *)aCos256;
    functionTable[FunctionTable_Rand]               = (void *)RSDK_random;
    functionTable[FunctionTable_Random]             = (void *)RSDK_random2;
    functionTable[FunctionTable_SetRandKey]         = (void *)setRandKey;
    functionTable[FunctionTable_ATan2]              = (void *)ArcTanLookup;
    functionTable[FunctionTable_SetIdentityMatrix]  = (void *)setIdentityMatrix;
    functionTable[FunctionTable_MatrixMultiply]     = (void *)matrixMultiply;
    functionTable[FunctionTable_MatrixTranslateXYZ] = (void *)matrixTranslateXYZ;
    functionTable[FunctionTable_MatrixScaleXYZ]     = (void *)matrixScaleXYZ;
    functionTable[FunctionTable_MatrixRotateX]      = (void *)matrixRotateX;
    functionTable[FunctionTable_MatrixRotateY]      = (void *)matrixRotateY;
    functionTable[FunctionTable_MatrixRotateZ]      = (void *)matrixRotateZ;
    functionTable[FunctionTable_MatrixRotateXYZ]    = (void *)matrixRotateXYZ;
    functionTable[FunctionTable_MatrixInverse]      = (void *)matrixInverse;
    functionTable[FunctionTable_MatrixCopy]         = (void *)matrixCopy;
    functionTable[FunctionTable_SetText]            = (void *)SetText;
    functionTable[FunctionTable_CopyString]         = (void *)CopyString;
    functionTable[FunctionTable_PrependString]      = (void *)NullFunc; // PrependString;
    functionTable[FunctionTable_AppendString]       = (void *)AppendString;
    functionTable[FunctionTable_Unknown67]          = (void *)NullFunc; // Unknown67;
    functionTable[FunctionTable_LoadStrings]        = (void *)LoadStrings;
    functionTable[FunctionTable_SplitStringList]    = (void *)SplitStringList;
    functionTable[FunctionTable_GetCString]         = (void *)GetCString;
    functionTable[FunctionTable_StringCompare]      = (void *)StringCompare;
    functionTable[FunctionTable_GetWindowSettings]  = (void *)NullFunc; // Unknown70;
    functionTable[FunctionTable_Unknown71]          = (void *)NullFunc; // Unknown71;
    functionTable[FunctionTable_SetScreenSize]      = (void *)SetScreenSize;
    functionTable[FunctionTable_SetClipBounds]      = (void *)SetClipBounds;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_SetScreenUnknown] = (void *)NullFunc; // SetScreenUnknown;
#endif
    functionTable[FunctionTable_LoadSpriteSheet]  = (void *)LoadSpriteSheet;
    functionTable[FunctionTable_SetLookupTable]   = (void *)SetLookupTable;
    functionTable[FunctionTable_SetPaletteMask]   = (void *)SetPaletteMask;
    functionTable[FunctionTable_SetPaletteEntry]  = (void *)SetPaletteEntry;
    functionTable[FunctionTable_GetPaletteEntry]  = (void *)GetPaletteEntry;
    functionTable[FunctionTable_SetActivePalette] = (void *)SetActivePalette;
    functionTable[FunctionTable_CopyPalette]      = (void *)CopyPalette;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_LoadPalette] = (void *)LoadPalette;
#endif
    functionTable[FunctionTable_RotatePalette]  = (void *)RotatePalette;
    functionTable[FunctionTable_SetLimitedFade] = (void *)SetPaletteFade;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_BlendColours] = (void *)BlendColours;
#endif
    functionTable[FunctionTable_DrawRect]                     = (void *)DrawRectangle;
    functionTable[FunctionTable_DrawLine]                     = (void *)DrawLine;
    functionTable[FunctionTable_DrawCircle]                   = (void *)DrawCircle;
    functionTable[FunctionTable_DrawCircleOutline]            = (void *)DrawCircleOutline;
    functionTable[FunctionTable_DrawQuad]                     = (void *)DrawQuad;
    functionTable[FunctionTable_DrawTexturedQuad]             = (void *)DrawBlendedQuad;
    functionTable[FunctionTable_DrawSprite]                   = (void *)DrawSprite;
    functionTable[FunctionTable_DrawDeformed]                 = (void *)DrawDeformedSprite;
    functionTable[FunctionTable_DrawText]                     = (void *)DrawText;
    functionTable[FunctionTable_DrawTile]                     = (void *)DrawTile;
    functionTable[FunctionTable_CopyTile]                     = (void *)CopyTile;
    functionTable[FunctionTable_DrawAniTile]                  = (void *)DrawAniTile;
    functionTable[FunctionTable_FillScreen]                   = (void *)FillScreen;
    functionTable[FunctionTable_LoadMesh]                     = (void *)LoadMesh;
    functionTable[FunctionTable_Create3DScene]                = (void *)Create3DScene;
    functionTable[FunctionTable_Init3DScene]                  = (void *)Init3DScene;
    functionTable[FunctionTable_SetAmbientUnknown]            = (void *)SetAmbientUnknown;
    functionTable[FunctionTable_SetDiffuseUnknown]            = (void *)SetDiffuseUnknown;
    functionTable[FunctionTable_SetSpecularUnknown]           = (void *)SetSpecularUnknown;
    functionTable[FunctionTable_SetupMesh]                    = (void *)SetupMesh;
    functionTable[FunctionTable_SetModelAnimation]            = (void *)SetModelAnimation;
    functionTable[FunctionTable_SetupMeshAnimation]           = (void *)SetupMeshAnimation;
    functionTable[FunctionTable_Draw3DScene]                  = (void *)Draw3DScene; // Draw3DScene;
    functionTable[FunctionTable_LoadAnimation]                = (void *)LoadAnimation;
    functionTable[FunctionTable_CreateAnimation]              = (void *)CreateAnimation;
    functionTable[FunctionTable_SetSpriteAnimation]           = (void *)SetSpriteAnimation;
    functionTable[FunctionTable_EditAnimation]                = (void *)EditAnimation;
    functionTable[FunctionTable_SetSpriteString]              = (void *)SetSpriteString;
    functionTable[FunctionTable_GetAnimation]                 = (void *)GetAnimation;
    functionTable[FunctionTable_GetFrame]                     = (void *)GetFrame;
    functionTable[FunctionTable_GetHitbox]                    = (void *)GetHitbox;
    functionTable[FunctionTable_GetFrameID]                   = (void *)GetFrameID;
    functionTable[FunctionTable_GetStringWidth]               = (void *)GetStringWidth;
    functionTable[FunctionTable_ProcessAnimation]             = (void *)ProcessAnimation;
    functionTable[FunctionTable_GetSceneLayer]                = (void *)GetSceneLayer;
    functionTable[FunctionTable_GetSceneLayerID]              = (void *)GetSceneLayerID;
    functionTable[FunctionTable_GetLayerSize]                 = (void *)GetLayerSize;
    functionTable[FunctionTable_GetTileInfo]                  = (void *)GetTileInfo;
    functionTable[FunctionTable_SetTileInfo]                  = (void *)SetTileInfo;
    functionTable[FunctionTable_CopyTileLayer]                = (void *)CopyTileLayout;
    functionTable[FunctionTable_ProcessParallax]              = (void *)ProcessParallax;
    functionTable[FunctionTable_GetLinePosPtrs]               = (void *)GetLinePositions;
    functionTable[FunctionTable_CheckObjectCollisionTouch]    = (void *)CheckObjectCollisionTouch;
    functionTable[FunctionTable_CheckObjectCollisionPoint]    = (void *)CheckObjectCollisionCircle;
    functionTable[FunctionTable_CheckObjectCollisionBox]      = (void *)CheckObjectCollisionBox;
    functionTable[FunctionTable_CheckObjectCollisionPlatform] = (void *)CheckObjectCollisionPlatform;
    functionTable[FunctionTable_ObjectTileCollision]          = (void *)ObjectTileCollision;
    functionTable[FunctionTable_ObjectTileGrip]               = (void *)ObjectTileGrip;
    functionTable[FunctionTable_ProcessTileCollisions]        = (void *)ProcessTileCollisions;
    functionTable[FunctionTable_GetTileAngle]                 = (void *)GetTileAngle;
    functionTable[FunctionTable_SetTileAngle]                 = (void *)SetTileAngle;
    functionTable[FunctionTable_GetTileBehaviour]             = (void *)GetTileBehaviour;
    functionTable[FunctionTable_SetTileBehaviour]             = (void *)SetTileBehaviour;
    functionTable[FunctionTable_GetSFX]                       = (void *)GetSFX;
    functionTable[FunctionTable_PlaySFX]                      = (void *)PlaySfx;
    functionTable[FunctionTable_StopSFX]                      = (void *)StopSfx;
    functionTable[FunctionTable_PlayMusic]                    = (void *)NullFunc; // PlayMusicFile;
    functionTable[FunctionTable_SetSoundAttributes]           = (void *)SetSoundAttributes;
    functionTable[FunctionTable_Unknown91]                    = (void *)SoundUnknown1;
    functionTable[FunctionTable_PauseChannel]                 = (void *)PauseChannel;
    functionTable[FunctionTable_ResumeChannel]                = (void *)ResumeChannel;
    functionTable[FunctionTable_SoundPlaying]                 = (void *)soundPlaying;
    functionTable[FunctionTable_ChannelPlaying]               = (void *)channelPlaying;
    functionTable[FunctionTable_TrackPlaying]                 = (void *)trackPlaying;
    functionTable[FunctionTable_LoadVideo]                    = (void *)NullFunc; // LoadVideo;
    functionTable[FunctionTable_LoadImage]                    = (void *)LoadImage;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_ControllerIDForInputID]       = (void *)NullFunc; // Unknown98;
    functionTable[FunctionTable_MostRecentActiveControllerID] = (void *)NullFunc; // Unknown99;
    functionTable[FunctionTable_Unknown100]                   = (void *)NullFunc; // Unknown100;
    functionTable[FunctionTable_Unknown101]                   = (void *)NullFunc; // Unknown101;
    functionTable[FunctionTable_Missing21]                    = (void *)NullFunc; // UserDataUnknown1;
    functionTable[FunctionTable_Missing22]                    = (void *)NullFunc; // UserDataUnknown2;
    functionTable[FunctionTable_Missing23]                    = (void *)NullFunc; // UserDataUnknown3;
    functionTable[FunctionTable_Missing24]                    = (void *)NullFunc; // UserDataUnknown4;
    functionTable[FunctionTable_Missing25]                    = (void *)NullFunc; // UserDataUnknown5;
    functionTable[FunctionTable_Missing26]                    = (void *)NullFunc; // UserDataUnknown6;
    functionTable[FunctionTable_AssignControllerID]           = (void *)NullFunc; // Unknown102;
    functionTable[FunctionTable_InputIDIsDisconnected]        = (void *)NullFunc; // Unknown103;
    functionTable[FunctionTable_ResetControllerAssignments]   = (void *)NullFunc; // Unknown104;
#endif
#if !RETRO_USE_PLUS
    functionTable[FunctionTable_Unknown92] = (void *)NullFunc;
#endif
    functionTable[FunctionTable_LoadUserFile] = LoadUserFile;
    functionTable[FunctionTable_SaveUserFile] = SaveUserFile;
#if RETRO_USE_PLUS
    functionTable[FunctionTable_printLog]             = (void *)printLog;
    functionTable[FunctionTable_printString]          = (void *)printString;
    functionTable[FunctionTable_printText]            = (void *)printText;
    functionTable[FunctionTable_printIntegerUnsigned] = (void *)printIntegerUnsigned;
    functionTable[FunctionTable_printInteger]         = (void *)printInteger;
    functionTable[FunctionTable_printFloat]           = (void *)printFloat;
    functionTable[FunctionTable_printVector2]         = (void *)printVector2;
    functionTable[FunctionTable_printHitbox]          = (void *)printHitbox;
#endif
    functionTable[FunctionTable_Unknown105] = (void *)NullFunc; // UserDataUnknown7;
    functionTable[FunctionTable_Unknown106] = (void *)NullFunc; // UserDataUnknown8;
#if !RETRO_USE_PLUS
    functionTable[FunctionTable_PrintMessage] = (void *)PrintMessage;
#endif
#if RETRO_USE_PLUS
    functionTable[FunctionTable_ClearDebugValues] = (void *)ClearDebugValues;
    functionTable[FunctionTable_SetDebugValue]    = (void *)SetDebugValue;
#endif
}

void LinkGameLogic(GameInfo *info) { printLog(SEVERITY_WARN, "Internal LinkGameLogic() function called, no logic will be linked"); }
