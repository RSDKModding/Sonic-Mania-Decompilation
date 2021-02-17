#include "RetroEngine.hpp"

void *functionTable[FUNCTABLE_COUNT];
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
    UserdataTable_GetAchievementsActive,
    UserdataTable_SetAchievementsActive,
    UserdataTable_Unknown7,
    UserdataTable_FetchLeaderboard,
    UserdataTable_TrackScore,
    UserdataTable_Unknown10,
    UserdataTable_Unknown11,
    UserdataTable_Missing2,
    UserdataTable_Unknown12,
    UserdataTable_Missing3,
    UserdataTable_Unknown13,
    UserdataTable_SetPresence,
    UserdataTable_TryTrackStat,
    UserdataTable_GetStatsActive,
    UserdataTable_SetStatsActive,
    UserdataTable_Unknown16,
    UserdataTable_TryAuth,
    UserdataTable_GetUserStorageActive,
    UserdataTable_Unknown19,
    UserdataTable_TryInitStorage,
    UserdataTable_Unknown21,
    UserdataTable_Unknown22,
    UserdataTable_Unknown23,
    UserdataTable_Unknown24,
    UserdataTable_Missing5,
    UserdataTable_Unknown25,
    UserdataTable_Unknown26,
    UserdataTable_Unknown27,
    UserdataTable_Unknown28,
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
    UserdataTable_Unknown41,
    UserdataTable_Unknown42,
    UserdataTable_Unknown43,
    UserdataTable_Count,
};

enum FunctionTableIDs {
    FunctionTable_InitGameOptions,
    FunctionTable_CreateObject,
    FunctionTable_CreateObjectContainer,
    FunctionTable_GetActiveObjects,
    FunctionTable_GetObjects,
    FunctionTable_NextForeachLoop,
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
    FunctionTable_SetHardResetFlag,
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectByName,
    FunctionTable_ClearScreens,
    FunctionTable_AddScreen,
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
    FunctionTable_Unknown64,
    FunctionTable_Unknown65,
    FunctionTable_Unknown66,
    FunctionTable_Unknown67,
    FunctionTable_LoadStrings,
    FunctionTable_Unknown68,
    FunctionTable_CopyString,
    FunctionTable_Unknown69,
    FunctionTable_Unknown70,
    FunctionTable_Unknown71,
    FunctionTable_SetScreenSize,
    FunctionTable_SetClipBounds,
    FunctionTable_SetScreenUnknown,
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetLookupTable,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    FunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
    FunctionTable_LoadPalette,
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
    FunctionTable_BlendColours,
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
    FunctionTable_View_Something1,
    FunctionTable_View_Something2,
    FunctionTable_View_Something3,
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
    FunctionTable_GetSceneLayerID,
    FunctionTable_GetSceneLayer,
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
    FunctionTable_ProcessPlayerTileCollisions,
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
    FunctionTable_Unknown92,
    FunctionTable_Unknown93,
    FunctionTable_Unknown94,
    FunctionTable_Unknown95,
    FunctionTable_Unknown96,
    FunctionTable_LoadVideo,
    FunctionTable_LoadImage,
    FunctionTable_Unknown98,
    FunctionTable_Unknown99,
    FunctionTable_Unknown100,
    FunctionTable_Unknown101,
    FunctionTable_Unknown102,
    FunctionTable_Unknown103,
    FunctionTable_Unknown104,
    FunctionTable_Missing21,
    FunctionTable_Missing22,
    FunctionTable_Missing23,
    FunctionTable_Missing24,
    FunctionTable_Missing25,
    FunctionTable_Missing26,
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
    FunctionTable_printLog,
    FunctionTable_printString,
    FunctionTable_printText,
    FunctionTable_printIntegerUnsigned,
    FunctionTable_printInteger,
    FunctionTable_printFloat,
    FunctionTable_printVector2,
    FunctionTable_printHitbox,
    FunctionTable_Unknown105,
    FunctionTable_Unknown106,
    FunctionTable_ClearDebugValues,
    FunctionTable_SetDebugValue,
    FunctionTable_Count,
};

SKUInfo curSKU;

void NullFunc() {}

void setupFunctions()
{
    curSKU.platform = GAME_PLATFORM;
    curSKU.language = 0;
    curSKU.region   = 0;

    CalculateTrigAngles();
    GenerateBlendLookupTable();
    InitGFXSystem();

    memset(functionTable, 0, FUNCTABLE_COUNT * sizeof(void *));

    // Userdata
    userDataTable[UserdataTable_GetUserLanguage]       = userCore->GetUserLanguage;
    userDataTable[UserdataTable_GetConfirmButtonFlip]  = userCore->GetConfirmButtonFlip;
    userDataTable[UserdataTable_ExitGame]              = userCore->ExitGame;
    userDataTable[UserdataTable_LaunchManual]          = userCore->ExitGame;
    userDataTable[UserdataTable_Unknown4]              = NullFunc;
    userDataTable[UserdataTable_CheckDLC]              = userCore->CheckDLC;
    userDataTable[UserdataTable_ClearAchievements]     = NullFunc;
    userDataTable[UserdataTable_UnlockAchievement]     = achievements->UnlockAchievement;
    userDataTable[UserdataTable_GetAchievementsActive] = NullFunc;
    userDataTable[UserdataTable_SetAchievementsActive] = NullFunc;
    userDataTable[UserdataTable_Unknown7]              = NullFunc;
    userDataTable[UserdataTable_FetchLeaderboard]      = leaderboards->FetchLeaderboard;
    userDataTable[UserdataTable_TrackScore]            = leaderboards->TrackScore;
    userDataTable[UserdataTable_Unknown10]             = NullFunc;
    userDataTable[UserdataTable_Unknown11]             = NullFunc;
    userDataTable[UserdataTable_Missing2]              = NullFunc;
    userDataTable[UserdataTable_Unknown12]             = NullFunc;
    userDataTable[UserdataTable_Missing3]              = NullFunc;
    userDataTable[UserdataTable_Unknown13]             = NullFunc;
    userDataTable[UserdataTable_SetPresence]           = richPresence->SetPresence;
    userDataTable[UserdataTable_TryTrackStat]          = NullFunc;
    userDataTable[UserdataTable_GetStatsActive]        = NullFunc;
    userDataTable[UserdataTable_SetStatsActive]        = NullFunc;
    userDataTable[UserdataTable_Unknown16]             = NullFunc;
    userDataTable[UserdataTable_TryAuth]               = NullFunc;
    userDataTable[UserdataTable_GetUserStorageActive]  = NullFunc;
    userDataTable[UserdataTable_Unknown19]             = NullFunc;
    userDataTable[UserdataTable_TryInitStorage]        = NullFunc;
    userDataTable[UserdataTable_Unknown21]             = NullFunc;
    userDataTable[UserdataTable_Unknown22]             = NullFunc;
    userDataTable[UserdataTable_Unknown23]             = NullFunc;
    userDataTable[UserdataTable_Unknown24]             = NullFunc;
    userDataTable[UserdataTable_Missing5]              = NullFunc;
    userDataTable[UserdataTable_Unknown25]             = NullFunc;
    userDataTable[UserdataTable_Unknown26]             = NullFunc;
    userDataTable[UserdataTable_Unknown27]             = NullFunc;
    userDataTable[UserdataTable_Unknown28]             = NullFunc;
    userDataTable[UserdataTable_LoadUserFile]          = NullFunc;
    userDataTable[UserdataTable_SaveUserFile]          = NullFunc;
    userDataTable[UserdataTable_DeleteUserFile]        = NullFunc;
    userDataTable[UserdataTable_AddUserDBEntry]        = NullFunc;
    userDataTable[UserdataTable_OpenUserDB]            = NullFunc;
    userDataTable[UserdataTable_SaveUserDB]            = NullFunc;
    userDataTable[UserdataTable_Unknown30]             = NullFunc;
    userDataTable[UserdataTable_Missing6]              = NullFunc;
    userDataTable[UserdataTable_Unknown31]             = NullFunc;
    userDataTable[UserdataTable_Unknown32]             = NullFunc;
    userDataTable[UserdataTable_Unknown33]             = NullFunc;
    userDataTable[UserdataTable_Unknown34]             = NullFunc;
    userDataTable[UserdataTable_Unknown35]             = NullFunc;
    userDataTable[UserdataTable_Unknown36]             = NullFunc;
    userDataTable[UserdataTable_Unknown37]             = NullFunc;
    userDataTable[UserdataTable_Unknown38]             = NullFunc;
    userDataTable[UserdataTable_Unknown39]             = NullFunc;
    userDataTable[UserdataTable_AddUserDB]             = NullFunc;
    userDataTable[UserdataTable_Unknown40]             = NullFunc;
    userDataTable[UserdataTable_Unknown41]             = NullFunc;
    userDataTable[UserdataTable_Unknown42]             = NullFunc;
    userDataTable[UserdataTable_Unknown43]             = NullFunc;

    // Function Table
    functionTable[FunctionTable_InitGameOptions]              = InitGameOptions;
    functionTable[FunctionTable_CreateObject]                 = CreateObject;
    functionTable[FunctionTable_CreateObjectContainer]        = CreateObjectContainer;
    functionTable[FunctionTable_GetActiveObjects]             = GetActiveObjects;
    functionTable[FunctionTable_GetObjects]                   = GetObjects;
    functionTable[FunctionTable_NextForeachLoop]              = NextForeachLoop;
    functionTable[FunctionTable_SetEditableVar]               = SetEditableVar;
    functionTable[FunctionTable_GetObjectByID]                = GetObjectByID;
    functionTable[FunctionTable_GetEntityID]                  = GetEntityID;
    functionTable[FunctionTable_GetEntityCount]               = GetEntityCount;
    functionTable[FunctionTable_GetDrawListRef]               = GetDrawListRef;
    functionTable[FunctionTable_GetDrawListRefPtr]            = GetDrawListRefPtr;
    functionTable[FunctionTable_DestroyEntity]                = DestroyEntity;
    functionTable[FunctionTable_ResetEntity]                  = ResetEntity;
    functionTable[FunctionTable_SpawnEntity]                  = SpawnEntity;
    functionTable[FunctionTable_CopyEntity]                   = CopyEntity;
    functionTable[FunctionTable_CheckOnScreen]                = CheckOnScreen;
    functionTable[FunctionTable_CheckPosOnScreen]             = CheckPosOnScreen;
    functionTable[FunctionTable_AddDrawListRef]               = AddDrawListRef;
    functionTable[FunctionTable_SwapDrawLayers]               = SwapDrawLayers;
    functionTable[FunctionTable_SetDrawLayerProperties]       = SetDrawLayerProperties;
    functionTable[FunctionTable_LoadScene]                    = LoadSceneByName;
    functionTable[FunctionTable_SetGameMode]                  = SetEngineState;
    functionTable[FunctionTable_SetHardResetFlag]             = SetHardResetFlag;
    functionTable[FunctionTable_CheckValidScene]              = CheckValidStage;
    functionTable[FunctionTable_CheckSceneFolder]             = CheckSceneFolder;
    functionTable[FunctionTable_InitSceneLoad]                = InitSceneLoad;
    functionTable[FunctionTable_GetObjectByName]              = GetObjectByName;
    functionTable[FunctionTable_ClearScreens]                 = ClearScreens;
    functionTable[FunctionTable_AddScreen]                    = AddScreen;
    functionTable[FunctionTable_GetSettingsValue]             = GetSettingsValue;
    functionTable[FunctionTable_SetSettingsValue]             = SetSettingsValue;
    functionTable[FunctionTable_UpdateWindow]                 = UpdateWindow;
    functionTable[FunctionTable_Sin1024]                      = sin1024;
    functionTable[FunctionTable_Cos1024]                      = cos1024;
    functionTable[FunctionTable_ATan1024]                     = tan1024;
    functionTable[FunctionTable_ASin1024]                     = aSin1024;
    functionTable[FunctionTable_ACos1024]                     = aCos1024;
    functionTable[FunctionTable_Sin512]                       = sin512;
    functionTable[FunctionTable_Cos512]                       = cos512;
    functionTable[FunctionTable_ATan512]                      = tan512;
    functionTable[FunctionTable_ASin512]                      = aSin512;
    functionTable[FunctionTable_ACos512]                      = aCos512;
    functionTable[FunctionTable_Sin256]                       = sin256;
    functionTable[FunctionTable_Cos256]                       = cos256;
    functionTable[FunctionTable_ATan256]                      = tan256;
    functionTable[FunctionTable_ASin256]                      = aSin256;
    functionTable[FunctionTable_ACos256]                      = aCos256;
    functionTable[FunctionTable_Rand]                         = random;
    functionTable[FunctionTable_Random]                       = random2;
    functionTable[FunctionTable_SetRandKey]                   = setRandKey;
    functionTable[FunctionTable_ATan2]                        = ArcTanLookup;
    functionTable[FunctionTable_SetIdentityMatrix]            = setIdentityMatrix;
    functionTable[FunctionTable_MatrixMultiply]               = matrixMultiply;
    functionTable[FunctionTable_MatrixTranslateXYZ]           = matrixTranslateXYZ;
    functionTable[FunctionTable_MatrixScaleXYZ]               = matrixScaleXYZ;
    functionTable[FunctionTable_MatrixRotateX]                = matrixRotateX;
    functionTable[FunctionTable_MatrixRotateY]                = matrixRotateY;
    functionTable[FunctionTable_MatrixRotateZ]                = matrixRotateZ;
    functionTable[FunctionTable_MatrixRotateXYZ]              = matrixRotateXYZ;
    functionTable[FunctionTable_MatrixInverse]                = matrixInverse;
    functionTable[FunctionTable_MatrixCopy]                   = matrixCopy;
    functionTable[FunctionTable_SetText]                      = SetText;
    functionTable[FunctionTable_Unknown64]                    = NullFunc; // Unknown64;
    functionTable[FunctionTable_Unknown65]                    = NullFunc; // Unknown65;
    functionTable[FunctionTable_Unknown66]                    = NullFunc; // Unknown66;
    functionTable[FunctionTable_Unknown67]                    = NullFunc; // Unknown67;
    functionTable[FunctionTable_LoadStrings]                  = NullFunc; // LoadStrings;
    functionTable[FunctionTable_Unknown68]                    = NullFunc; // Unknown68;
    functionTable[FunctionTable_CopyString]                   = NullFunc; // CopyString;
    functionTable[FunctionTable_Unknown69]                    = NullFunc; // Unknown69;
    functionTable[FunctionTable_Unknown70]                    = NullFunc; // Unknown70;
    functionTable[FunctionTable_Unknown71]                    = NullFunc; // Unknown71;
    functionTable[FunctionTable_SetScreenSize]                = SetScreenSize;
    functionTable[FunctionTable_SetClipBounds]                = SetClipBounds;
    functionTable[FunctionTable_SetScreenUnknown]             = NullFunc; // SetScreenUnknown;
    functionTable[FunctionTable_LoadSpriteSheet]              = LoadSpriteSheet;
    functionTable[FunctionTable_SetLookupTable]               = SetLookupTable;
    functionTable[FunctionTable_SetPaletteMask]               = SetPaletteMask;
    functionTable[FunctionTable_SetPaletteEntry]              = SetPaletteEntry;
    functionTable[FunctionTable_GetPaletteEntry]              = GetPaletteEntry;
    functionTable[FunctionTable_SetActivePalette]             = SetActivePalette;
    functionTable[FunctionTable_CopyPalette]                  = CopyPalette;
    functionTable[FunctionTable_LoadPalette]                  = LoadPalette;
    functionTable[FunctionTable_RotatePalette]                = RotatePalette;
    functionTable[FunctionTable_SetLimitedFade]               = SetPaletteFade;
    functionTable[FunctionTable_BlendColours]                 = BlendColours;
    functionTable[FunctionTable_DrawRect]                     = DrawRectangle;
    functionTable[FunctionTable_DrawLine]                     = DrawLine;
    functionTable[FunctionTable_DrawCircle]                   = DrawCircle;
    functionTable[FunctionTable_DrawCircleOutline]            = DrawCircleOutline;
    functionTable[FunctionTable_DrawQuad]                     = DrawQuad;
    functionTable[FunctionTable_DrawTexturedQuad]             = DrawTexturedQuad;
    functionTable[FunctionTable_DrawSprite]                   = DrawSprite;
    functionTable[FunctionTable_DrawDeformed]                 = DrawDeformedSprite;
    functionTable[FunctionTable_DrawText]                     = DrawText;
    functionTable[FunctionTable_DrawTile]                     = DrawTile;
    functionTable[FunctionTable_CopyTile]                     = CopyTile;
    functionTable[FunctionTable_DrawAniTile]                  = DrawAniTile;
    functionTable[FunctionTable_FillScreen]                   = FillScreen;
    functionTable[FunctionTable_LoadMesh]                     = LoadMesh;
    functionTable[FunctionTable_Create3DScene]                = NullFunc; // Create3DScene;
    functionTable[FunctionTable_Init3DScene]                  = NullFunc; // Init3DScene;
    functionTable[FunctionTable_View_Something1]              = NullFunc; // View_Something1;
    functionTable[FunctionTable_View_Something2]              = NullFunc; // View_Something2;
    functionTable[FunctionTable_View_Something3]              = NullFunc; // View_Something3;
    functionTable[FunctionTable_SetupMesh]                    = NullFunc; // SetupMesh;
    functionTable[FunctionTable_SetModelAnimation]            = SetModelAnimation;
    functionTable[FunctionTable_SetupMeshAnimation]           = NullFunc; // SetupMeshAnimation;
    functionTable[FunctionTable_Draw3DScene]                  = NullFunc; // Draw3DScene;
    functionTable[FunctionTable_LoadAnimation]                = LoadAnimation;
    functionTable[FunctionTable_CreateAnimation]              = CreateAnimation;
    functionTable[FunctionTable_SetSpriteAnimation]           = SetSpriteAnimation;
    functionTable[FunctionTable_EditAnimation]                = EditAnimation;
    functionTable[FunctionTable_SetSpriteString]              = SetSpriteString;
    functionTable[FunctionTable_GetAnimation]                 = GetAnimation;
    functionTable[FunctionTable_GetFrame]                     = GetFrame;
    functionTable[FunctionTable_GetHitbox]                    = GetHitbox;
    functionTable[FunctionTable_GetFrameID]                   = GetFrameID;
    functionTable[FunctionTable_GetStringWidth]               = GetStringWidth;
    functionTable[FunctionTable_ProcessAnimation]             = ProcessAnimation;
    functionTable[FunctionTable_GetSceneLayer]                = GetSceneLayer;
    functionTable[FunctionTable_GetSceneLayerID]              = GetSceneLayerID;
    functionTable[FunctionTable_GetLayerSize]                 = GetLayerSize;
    functionTable[FunctionTable_GetTileInfo]                  = GetTileInfo;
    functionTable[FunctionTable_SetTileInfo]                  = SetTileInfo;
    functionTable[FunctionTable_CopyTileLayer]                = CopyTileLayout;
    functionTable[FunctionTable_ProcessParallax]              = ProcessParallax;
    functionTable[FunctionTable_GetLinePosPtrs]               = GetLinePositions;
    functionTable[FunctionTable_CheckObjectCollisionTouch]    = CheckObjectCollisionTouch;
    functionTable[FunctionTable_CheckObjectCollisionPoint]    = CheckObjectCollisionCircle;
    functionTable[FunctionTable_CheckObjectCollisionBox]      = CheckObjectCollisionBox;
    functionTable[FunctionTable_CheckObjectCollisionPlatform] = CheckObjectCollisionPlatform;
    functionTable[FunctionTable_ObjectTileCollision]          = ObjectTileCollision;
    functionTable[FunctionTable_ObjectTileGrip]               = ObjectTileGrip;
    functionTable[FunctionTable_ProcessPlayerTileCollisions]  = NullFunc; // ProcessTileCollisions;
    functionTable[FunctionTable_GetTileAngle]                 = GetTileAngle;
    functionTable[FunctionTable_SetTileAngle]                 = SetTileAngle;
    functionTable[FunctionTable_GetTileBehaviour]             = GetTileBehaviour;
    functionTable[FunctionTable_SetTileBehaviour]             = SetTileBehaviour;
    functionTable[FunctionTable_GetSFX]                       = GetSFX;
    functionTable[FunctionTable_PlaySFX]                      = NullFunc; // PlaySFX;
    functionTable[FunctionTable_StopSFX]                      = NullFunc; // StopSFX;
    functionTable[FunctionTable_PlayMusic]                    = NullFunc; // PlayMusicFile;
    functionTable[FunctionTable_SetSoundAttributes]           = NullFunc; // SetSoundAttributes;
    functionTable[FunctionTable_Unknown91]                    = NullFunc; // Unknown91;
    functionTable[FunctionTable_Unknown92]                    = NullFunc; // Unknown92;
    functionTable[FunctionTable_Unknown93]                    = NullFunc; // Unknown93;
    functionTable[FunctionTable_Unknown94]                    = NullFunc; // Unknown94;
    functionTable[FunctionTable_Unknown95]                    = NullFunc; // Unknown95;
    functionTable[FunctionTable_Unknown96]                    = NullFunc; // Unknown96;
    functionTable[FunctionTable_LoadVideo]                    = NullFunc; // LoadVideo;
    functionTable[FunctionTable_LoadImage]                    = LoadImage;
    functionTable[FunctionTable_Unknown98]                    = NullFunc; // Unknown98;
    functionTable[FunctionTable_Unknown99]                    = NullFunc; // Unknown99;
    functionTable[FunctionTable_Unknown100]                   = NullFunc; // Unknown100;
    functionTable[FunctionTable_Unknown101]                   = NullFunc; // Unknown101;
    functionTable[FunctionTable_Unknown102]                   = NullFunc; // Unknown102;
    functionTable[FunctionTable_Unknown103]                   = NullFunc; // Unknown103;
    functionTable[FunctionTable_Unknown104]                   = NullFunc; // Unknown104;
    functionTable[FunctionTable_Missing21]                    = NullFunc; // UserDataUnknown1;
    functionTable[FunctionTable_Missing22]                    = NullFunc; // UserDataUnknown2;
    functionTable[FunctionTable_Missing23]                    = NullFunc; // UserDataUnknown3;
    functionTable[FunctionTable_Missing24]                    = NullFunc; // UserDataUnknown4;
    functionTable[FunctionTable_Missing25]                    = NullFunc; // UserDataUnknown5;
    functionTable[FunctionTable_Missing26]                    = NullFunc; // UserDataUnknown6;
    functionTable[FunctionTable_LoadUserFile]                 = NullFunc; // LoadUserFile;
    functionTable[FunctionTable_SaveUserFile]                 = NullFunc; // SaveUserFile;
    functionTable[FunctionTable_printLog]                     = printLog;
    functionTable[FunctionTable_printString]                  = printString;
    functionTable[FunctionTable_printText]                    = printText;
    functionTable[FunctionTable_printIntegerUnsigned]         = printIntegerUnsigned;
    functionTable[FunctionTable_printInteger]                 = printInteger;
    functionTable[FunctionTable_printFloat]                   = printFloat;
    functionTable[FunctionTable_printVector2]                 = printVector2;
    functionTable[FunctionTable_printHitbox]                  = printHitbox;
    functionTable[FunctionTable_Unknown105]                   = NullFunc; // UserDataUnknown7;
    functionTable[FunctionTable_Unknown106]                   = NullFunc; // UserDataUnknown8;
    functionTable[FunctionTable_ClearDebugValues]             = ClearDebugValues;
    functionTable[FunctionTable_SetDebugValue]                = SetDebugValue;
}

void LinkGameLogic(GameInfo *info) { printLog(SEVERITY_WARN, "Internal LinkGameLogic() function called, no logic will be linked"); }