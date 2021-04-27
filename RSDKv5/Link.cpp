#include "RetroEngine.hpp"

void *RSDKFunctionTable[FUNCTABLE_COUNT];

#if RETRO_REV02
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
    UserdataTable_ClearUserDB,
    UserdataTable_ClearAllUserDBs,
    UserdataTable_Unknown31,
    UserdataTable_GetUserDBSatus,
    UserdataTable_Unknown33,
    UserdataTable_Unknown34,
    UserdataTable_GetUserDBUnknownCount,
    UserdataTable_GetUserDBUnknown,
    UserdataTable_Unknown37,
    UserdataTable_SetUserDBValue,
    UserdataTable_Unknown39,
    UserdataTable_GetEntryUUID,
    UserdataTable_GetUserDBByID,
    UserdataTable_GetUserDBCreationTime,
    UserdataTable_RemoveDBEntry,
    UserdataTable_RemoveAllDBEntries,
    UserdataTable_Count,
};
#endif

enum FunctionTableIDs {
    FunctionTable_InitGameOptions,
    FunctionTable_RegisterObject,
#if RETRO_REV02
    FunctionTable_RegisterObjectContainer,
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
    FunctionTable_ResetEntityPtr,
    FunctionTable_ResetEntitySlot,
    FunctionTable_CreateEntity,
    FunctionTable_CopyEntity,
    FunctionTable_CheckOnScreen,
    FunctionTable_CheckPosOnScreen,
    FunctionTable_AddDrawListRef,
    FunctionTable_SwapDrawLayers,
    FunctionTable_SetDrawLayerProperties,
    FunctionTable_LoadScene,
    FunctionTable_SetGameMode,
#if RETRO_REV02
    FunctionTable_SetHardResetFlag,
#endif
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectByName,
    FunctionTable_ClearScreens,
    FunctionTable_AddScreen,
#if !RETRO_REV02
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
    FunctionTable_GetDisplayInfo,
    FunctionTable_GetWindowSize,
    FunctionTable_SetScreenSize,
    FunctionTable_SetClipBounds,
#if RETRO_REV02
    FunctionTable_SetScreenSplitVerticies,
#endif
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetLookupTable,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    FunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
#if RETRO_REV02
    FunctionTable_LoadPalette,
#endif
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
#if RETRO_REV02
    FunctionTable_BlendColours,
#endif
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawFace,
    FunctionTable_DrawBlendedFace,
    FunctionTable_DrawSprite,
    FunctionTable_DrawDeformed,
    FunctionTable_DrawText,
    FunctionTable_DrawTile,
    FunctionTable_CopyTile,
    FunctionTable_DrawAniTile,
    FunctionTable_FillScreen,
    FunctionTable_LoadMesh,
    FunctionTable_Create3DScene,
    FunctionTable_Prepare3DScene,
    FunctionTable_SetDiffuseColour,
    FunctionTable_SetDiffuseIntensity,
    FunctionTable_SetSpecularIntensity,
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
    FunctionTable_GetScanlines,
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
    FunctionTable_SetChannelAttributes,
    FunctionTable_StopChannel,
    FunctionTable_PauseChannel,
    FunctionTable_ResumeChannel,
    FunctionTable_SfxPlaying,
    FunctionTable_ChannelActive,
    FunctionTable_GetChannelPos,
    FunctionTable_LoadVideo,
    FunctionTable_LoadImage,
#if RETRO_REV02
    FunctionTable_ControllerIDForInputID,
    FunctionTable_MostRecentActiveControllerID,
    FunctionTable_Unknown100,
    FunctionTable_GetAssignedControllerID,
    FunctionTable_GetAssignedUnknown,
    FunctionTable_Missing22,
    FunctionTable_Missing23,
    FunctionTable_Missing24,
    FunctionTable_Missing25,
    FunctionTable_Missing26,
    FunctionTable_AssignControllerID,
    FunctionTable_InputIDIsDisconnected,
    FunctionTable_ResetControllerAssignments,
#endif
#if !RETRO_REV02
    FunctionTable_Unknown92,
#endif
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
#if RETRO_REV02
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
#if RETRO_REV02
    FunctionTable_ClearDebugValues,
    FunctionTable_SetDebugValue,
#endif
#if !RETRO_REV02
    FunctionTable_PrintMessage,
#endif
    FunctionTable_Count,
};

#if RETRO_REV02
SKUInfo curSKU;
UnknownInfo unknownInfo;
#endif

GameVersionInfo gameVerInfo;

void NullFunc() {}

#define addToRSDKFunctionTable(id, func) RSDKFunctionTable[id] = (void *)func;
#define addToUserFunctionTable(id, func) userFunctionTable[id] = (void *)func;

void setupFunctions()
{
    int language = LANGUAGE_EN;
    int region = REGION_US;
    int platform = PLATFORM_DEV;
#if RETRO_PLATFORM == RETRO_WIN
    //platform = PLATFORM_WIN;
#elif RETRO_PLATFORM == RETRO_PS4
    platform = PLATFORM_PS4;
#elif RETRO_PLATFORM == RETRO_XB1
    platform = PLATFORM_XB1;
#elif RETRO_PLATFORM == RETRO_SWITCH
    platform = PLATFORM_SWITCH;
#endif
    
#if RETRO_REV02
    curSKU.platform = platform;
    curSKU.language = language;
    curSKU.region   = region;
#else
    gameVerInfo.platform = platform;
    gameVerInfo.language = language;
    gameVerInfo.region   = region;
#endif

    CalculateTrigAngles();
    GenerateBlendLookupTable();
    InitGFXSystem();

    memset(RSDKFunctionTable, NULL, FUNCTABLE_COUNT * sizeof(void *));
#if RETRO_REV02
    memset(userFunctionTable, NULL, UDATATABLE_COUNT * sizeof(void *));
#endif

#if RETRO_REV02
    // Userdata
    addToUserFunctionTable(UserdataTable_GetUserLanguage, userCore->GetUserLanguage);
    addToUserFunctionTable(UserdataTable_GetConfirmButtonFlip, userCore->GetConfirmButtonFlip);
    addToUserFunctionTable(UserdataTable_ExitGame, userCore->ExitGame);
    addToUserFunctionTable(UserdataTable_LaunchManual, userCore->LaunchManual);
    addToUserFunctionTable(UserdataTable_Unknown4, userCore->unknown15);
    addToUserFunctionTable(UserdataTable_CheckDLC, userCore->CheckDLC);
    addToUserFunctionTable(UserdataTable_ClearAchievements, userCore->ShowExtensionOverlay);
    addToUserFunctionTable(UserdataTable_UnlockAchievement, achievements->UnlockAchievement);
    addToUserFunctionTable(UserdataTable_GetAchievementsStatus, GetAchievementsStatus);
    addToUserFunctionTable(UserdataTable_SetAchievementsStatus, SetAchievementsStatus);
    addToUserFunctionTable(UserdataTable_LeaderboardsUnknown4, NullFunc);
    addToUserFunctionTable(UserdataTable_FetchLeaderboard, leaderboards->FetchLeaderboard);
    addToUserFunctionTable(UserdataTable_TrackScore, leaderboards->TrackScore);
    addToUserFunctionTable(UserdataTable_LeaderboardsUnknown7, leaderboards->unknown7);
    addToUserFunctionTable(UserdataTable_LeaderboardEntryCount, NullFunc);
    addToUserFunctionTable(UserdataTable_Missing2, NullFunc);
    addToUserFunctionTable(UserdataTable_Unknown12, NullFunc);
    addToUserFunctionTable(UserdataTable_Missing3, NullFunc);
    addToUserFunctionTable(UserdataTable_ReadLeaderboardEntry, NullFunc);
    addToUserFunctionTable(UserdataTable_SetPresence, richPresence->SetPresence);
    addToUserFunctionTable(UserdataTable_TryTrackStat, stats->TryTrackStat);
    addToUserFunctionTable(UserdataTable_GetStatsStatus, GetStatsStatus);
    addToUserFunctionTable(UserdataTable_SetStatsStatus, SetStatsStatus);
    addToUserFunctionTable(UserdataTable_Unknown16, userStorage->unknown8);
    addToUserFunctionTable(UserdataTable_TryAuth, userStorage->TryAuth);
    addToUserFunctionTable(UserdataTable_GetUserAuthStatus, GetUserStorageStatus);
    addToUserFunctionTable(UserdataTable_GetUsername, userStorage->GetUsername);
    addToUserFunctionTable(UserdataTable_TryInitStorage, userStorage->TryInitStorage);
    addToUserFunctionTable(UserdataTable_UserStorageStatusUnknown1, UserStorageStatusUnknown1);
    addToUserFunctionTable(UserdataTable_Unknown22, UserStorageStatusUnknown2);
    addToUserFunctionTable(UserdataTable_Unknown23, ClearUserStorageStatus);
    addToUserFunctionTable(UserdataTable_Unknown24, SetUserStorageStatus);
    addToUserFunctionTable(UserdataTable_Missing5, UserStorageStatusUnknown3);
    addToUserFunctionTable(UserdataTable_Unknown25, UserStorageStatusUnknown4);
    addToUserFunctionTable(UserdataTable_Unknown26, UserStorageStatusUnknown5);
    addToUserFunctionTable(UserdataTable_SetUserStorageNoSave, SetUserStorageNoSave);
    addToUserFunctionTable(UserdataTable_GetUserStorageNoSave, GetUserStorageNoSave);
    addToUserFunctionTable(UserdataTable_LoadUserFile, userStorage->LoadUserFile);
    addToUserFunctionTable(UserdataTable_SaveUserFile, userStorage->SaveUserFile);
    addToUserFunctionTable(UserdataTable_DeleteUserFile, userStorage->DeleteUserFile);
    addToUserFunctionTable(UserdataTable_AddUserDBEntry, InitUserDB);
    addToUserFunctionTable(UserdataTable_OpenUserDB, LoadUserDB);
    addToUserFunctionTable(UserdataTable_SaveUserDB, SaveUserDB);
    addToUserFunctionTable(UserdataTable_ClearUserDB, ClearUserDB);
    addToUserFunctionTable(UserdataTable_ClearAllUserDBs, ClearAllUserDBs);
    addToUserFunctionTable(UserdataTable_Unknown31, NullFunc); // Unknown31);
    addToUserFunctionTable(UserdataTable_GetUserDBSatus, GetUserDBStatus);
    addToUserFunctionTable(UserdataTable_Unknown33, NullFunc); // Unknown33);
    addToUserFunctionTable(UserdataTable_Unknown34, NullFunc); // Unknown34);
    addToUserFunctionTable(UserdataTable_GetUserDBUnknownCount, GetUserDBRowUnknownCount);
    addToUserFunctionTable(UserdataTable_GetUserDBUnknown, GetUserDBRowUnknown);
    addToUserFunctionTable(UserdataTable_Unknown37, AddUserDBEntry);
    addToUserFunctionTable(UserdataTable_SetUserDBValue, NullFunc); // SetUserDBValue);
    addToUserFunctionTable(UserdataTable_Unknown39, NullFunc);      // Unknown39);
    addToUserFunctionTable(UserdataTable_GetEntryUUID, GetUserDBRowUUID);
    addToUserFunctionTable(UserdataTable_GetUserDBByID, GetUserDBByID);
    addToUserFunctionTable(UserdataTable_GetUserDBCreationTime, GetUserDBCreationTime);
    addToUserFunctionTable(UserdataTable_RemoveDBEntry, RemoveDBEntry);
    addToUserFunctionTable(UserdataTable_RemoveAllDBEntries, RemoveAllDBEntries);
#endif

    // Function Table
    addToRSDKFunctionTable(FunctionTable_InitGameOptions, InitGameOptions);
    addToRSDKFunctionTable(FunctionTable_RegisterObject, RegisterObject);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_RegisterObjectContainer, RegisterObjectContainer);
#endif
    addToRSDKFunctionTable(FunctionTable_GetActiveEntities, GetActiveEntities);
    addToRSDKFunctionTable(FunctionTable_GetEntities, GetEntities);
    addToRSDKFunctionTable(FunctionTable_BreakForeachLoop, BreakForeachLoop);
    addToRSDKFunctionTable(FunctionTable_SetEditableVar, SetEditableVar);
    addToRSDKFunctionTable(FunctionTable_GetObjectByID, GetObjectByID);
    addToRSDKFunctionTable(FunctionTable_GetEntityID, GetEntityID);
    addToRSDKFunctionTable(FunctionTable_GetEntityCount, GetEntityCount);
    addToRSDKFunctionTable(FunctionTable_GetDrawListRef, GetDrawListRef);
    addToRSDKFunctionTable(FunctionTable_GetDrawListRefPtr, GetDrawListRefPtr);
    addToRSDKFunctionTable(FunctionTable_ResetEntityPtr, ResetEntityPtr);
    addToRSDKFunctionTable(FunctionTable_ResetEntitySlot, ResetEntitySlot);
    addToRSDKFunctionTable(FunctionTable_CreateEntity, CreateEntity);
    addToRSDKFunctionTable(FunctionTable_CopyEntity, CopyEntity);
    addToRSDKFunctionTable(FunctionTable_CheckOnScreen, CheckOnScreen);
    addToRSDKFunctionTable(FunctionTable_CheckPosOnScreen, CheckPosOnScreen);
    addToRSDKFunctionTable(FunctionTable_AddDrawListRef, AddDrawListRef);
    addToRSDKFunctionTable(FunctionTable_SwapDrawLayers, SwapDrawLayers);
    addToRSDKFunctionTable(FunctionTable_SetDrawLayerProperties, SetDrawLayerProperties);
    addToRSDKFunctionTable(FunctionTable_LoadScene, SetScene);
    addToRSDKFunctionTable(FunctionTable_SetGameMode, SetEngineState);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_SetHardResetFlag, SetHardResetFlag);
#endif
    addToRSDKFunctionTable(FunctionTable_CheckValidScene, CheckValidStage);
    addToRSDKFunctionTable(FunctionTable_CheckSceneFolder, CheckSceneFolder);
    addToRSDKFunctionTable(FunctionTable_InitSceneLoad, InitSceneLoad);
    addToRSDKFunctionTable(FunctionTable_GetObjectByName, GetObjectByName);
    addToRSDKFunctionTable(FunctionTable_ClearScreens, ClearCameras);
    addToRSDKFunctionTable(FunctionTable_AddScreen, AddCamera);
#if !RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_GetFuncPtr, GetFuncPtr);
#endif
    addToRSDKFunctionTable(FunctionTable_GetSettingsValue, GetSettingsValue);
    addToRSDKFunctionTable(FunctionTable_SetSettingsValue, SetSettingsValue);
    addToRSDKFunctionTable(FunctionTable_UpdateWindow, UpdateWindow);
    addToRSDKFunctionTable(FunctionTable_Sin1024, sin1024);
    addToRSDKFunctionTable(FunctionTable_Cos1024, cos1024);
    addToRSDKFunctionTable(FunctionTable_ATan1024, tan1024);
    addToRSDKFunctionTable(FunctionTable_ASin1024, aSin1024);
    addToRSDKFunctionTable(FunctionTable_ACos1024, aCos1024);
    addToRSDKFunctionTable(FunctionTable_Sin512, sin512);
    addToRSDKFunctionTable(FunctionTable_Cos512, cos512);
    addToRSDKFunctionTable(FunctionTable_ATan512, tan512);
    addToRSDKFunctionTable(FunctionTable_ASin512, aSin512);
    addToRSDKFunctionTable(FunctionTable_ACos512, aCos512);
    addToRSDKFunctionTable(FunctionTable_Sin256, sin256);
    addToRSDKFunctionTable(FunctionTable_Cos256, cos256);
    addToRSDKFunctionTable(FunctionTable_ATan256, tan256);
    addToRSDKFunctionTable(FunctionTable_ASin256, aSin256);
    addToRSDKFunctionTable(FunctionTable_ACos256, aCos256);
    addToRSDKFunctionTable(FunctionTable_Rand, RSDK_random);
    addToRSDKFunctionTable(FunctionTable_Random, RSDK_random2);
    addToRSDKFunctionTable(FunctionTable_SetRandKey, setRandKey);
    addToRSDKFunctionTable(FunctionTable_ATan2, ArcTanLookup);
    addToRSDKFunctionTable(FunctionTable_SetIdentityMatrix, setIdentityMatrix);
    addToRSDKFunctionTable(FunctionTable_MatrixMultiply, matrixMultiply);
    addToRSDKFunctionTable(FunctionTable_MatrixTranslateXYZ, matrixTranslateXYZ);
    addToRSDKFunctionTable(FunctionTable_MatrixScaleXYZ, matrixScaleXYZ);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateX, matrixRotateX);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateY, matrixRotateY);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateZ, matrixRotateZ);
    addToRSDKFunctionTable(FunctionTable_MatrixRotateXYZ, matrixRotateXYZ);
    addToRSDKFunctionTable(FunctionTable_MatrixInverse, matrixInverse);
    addToRSDKFunctionTable(FunctionTable_MatrixCopy, matrixCopy);
    addToRSDKFunctionTable(FunctionTable_SetText, SetText);
    addToRSDKFunctionTable(FunctionTable_CopyString, CopyString);
    addToRSDKFunctionTable(FunctionTable_PrependString, PrependString);
    addToRSDKFunctionTable(FunctionTable_AppendString, AppendString);
    addToRSDKFunctionTable(FunctionTable_Unknown67, Unknown67);
    addToRSDKFunctionTable(FunctionTable_LoadStrings, LoadStrings);
    addToRSDKFunctionTable(FunctionTable_SplitStringList, SplitStringList);
    addToRSDKFunctionTable(FunctionTable_GetCString, GetCString);
    addToRSDKFunctionTable(FunctionTable_StringCompare, StringCompare);
    addToRSDKFunctionTable(FunctionTable_GetDisplayInfo, GetDisplayInfo);
    addToRSDKFunctionTable(FunctionTable_GetWindowSize, GetWindowSize);   
    addToRSDKFunctionTable(FunctionTable_SetScreenSize, SetScreenSize);
    addToRSDKFunctionTable(FunctionTable_SetClipBounds, SetClipBounds);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_SetScreenSplitVerticies, SetScreenSplitVerticies);
#endif
    addToRSDKFunctionTable(FunctionTable_LoadSpriteSheet, LoadSpriteSheet);
    addToRSDKFunctionTable(FunctionTable_SetLookupTable, SetLookupTable);
    addToRSDKFunctionTable(FunctionTable_SetPaletteMask, SetPaletteMask);
    addToRSDKFunctionTable(FunctionTable_SetPaletteEntry, SetPaletteEntry);
    addToRSDKFunctionTable(FunctionTable_GetPaletteEntry, GetPaletteEntry);
    addToRSDKFunctionTable(FunctionTable_SetActivePalette, SetActivePalette);
    addToRSDKFunctionTable(FunctionTable_CopyPalette, CopyPalette);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_LoadPalette, LoadPalette);
#endif
    addToRSDKFunctionTable(FunctionTable_RotatePalette, RotatePalette);
    addToRSDKFunctionTable(FunctionTable_SetLimitedFade, SetPaletteFade);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_BlendColours, BlendColours);
#endif
    addToRSDKFunctionTable(FunctionTable_DrawRect, DrawRectangle);
    addToRSDKFunctionTable(FunctionTable_DrawLine, DrawLine);
    addToRSDKFunctionTable(FunctionTable_DrawCircle, DrawCircle);
    addToRSDKFunctionTable(FunctionTable_DrawCircleOutline, DrawCircleOutline);
    addToRSDKFunctionTable(FunctionTable_DrawFace, DrawFace);
    addToRSDKFunctionTable(FunctionTable_DrawBlendedFace, DrawBlendedFace);
    addToRSDKFunctionTable(FunctionTable_DrawSprite, DrawSprite);
    addToRSDKFunctionTable(FunctionTable_DrawDeformed, DrawDeformedSprite);
    addToRSDKFunctionTable(FunctionTable_DrawText, DrawText);
    addToRSDKFunctionTable(FunctionTable_DrawTile, DrawTile);
    addToRSDKFunctionTable(FunctionTable_CopyTile, CopyTile);
    addToRSDKFunctionTable(FunctionTable_DrawAniTile, DrawAniTile);
    addToRSDKFunctionTable(FunctionTable_FillScreen, FillScreen);
    addToRSDKFunctionTable(FunctionTable_LoadMesh, LoadMesh);
    addToRSDKFunctionTable(FunctionTable_Create3DScene, Create3DScene);
    addToRSDKFunctionTable(FunctionTable_Prepare3DScene, Prepare3DScene);
    addToRSDKFunctionTable(FunctionTable_SetDiffuseColour, SetDiffuseColour);
    addToRSDKFunctionTable(FunctionTable_SetDiffuseIntensity, SetDiffuseIntensity);
    addToRSDKFunctionTable(FunctionTable_SetSpecularIntensity, SetSpecularIntensity);
    addToRSDKFunctionTable(FunctionTable_SetupMesh, AddModelToScene);
    addToRSDKFunctionTable(FunctionTable_SetModelAnimation, SetMeshAnimation);
    addToRSDKFunctionTable(FunctionTable_SetupMeshAnimation, AddMeshFrameToScene);
    addToRSDKFunctionTable(FunctionTable_Draw3DScene, Draw3DScene);
    addToRSDKFunctionTable(FunctionTable_LoadAnimation, LoadAnimation);
    addToRSDKFunctionTable(FunctionTable_CreateAnimation, CreateAnimation);
    addToRSDKFunctionTable(FunctionTable_SetSpriteAnimation, SetSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_EditAnimation, EditAnimation);
    addToRSDKFunctionTable(FunctionTable_SetSpriteString, SetSpriteString);
    addToRSDKFunctionTable(FunctionTable_GetAnimation, GetAnimation);
    addToRSDKFunctionTable(FunctionTable_GetFrame, GetFrame);
    addToRSDKFunctionTable(FunctionTable_GetHitbox, GetHitbox);
    addToRSDKFunctionTable(FunctionTable_GetFrameID, GetFrameID);
    addToRSDKFunctionTable(FunctionTable_GetStringWidth, GetStringWidth);
    addToRSDKFunctionTable(FunctionTable_ProcessAnimation, ProcessAnimation);
    addToRSDKFunctionTable(FunctionTable_GetSceneLayer, GetSceneLayer);
    addToRSDKFunctionTable(FunctionTable_GetSceneLayerID, GetSceneLayerID);
    addToRSDKFunctionTable(FunctionTable_GetLayerSize, GetLayerSize);
    addToRSDKFunctionTable(FunctionTable_GetTileInfo, GetTileInfo);
    addToRSDKFunctionTable(FunctionTable_SetTileInfo, SetTileInfo);
    addToRSDKFunctionTable(FunctionTable_CopyTileLayer, CopyTileLayout);
    addToRSDKFunctionTable(FunctionTable_ProcessParallax, ProcessParallax);
    addToRSDKFunctionTable(FunctionTable_GetScanlines, GetScanlines);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionTouch, CheckObjectCollisionTouch);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionPoint, CheckObjectCollisionCircle);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionBox, CheckObjectCollisionBox);
    addToRSDKFunctionTable(FunctionTable_CheckObjectCollisionPlatform, CheckObjectCollisionPlatform);
    addToRSDKFunctionTable(FunctionTable_ObjectTileCollision, ObjectTileCollision);
    addToRSDKFunctionTable(FunctionTable_ObjectTileGrip, ObjectTileGrip);
    addToRSDKFunctionTable(FunctionTable_ProcessTileCollisions, ProcessTileCollisions);
    addToRSDKFunctionTable(FunctionTable_GetTileAngle, GetTileAngle);
    addToRSDKFunctionTable(FunctionTable_SetTileAngle, SetTileAngle);
    addToRSDKFunctionTable(FunctionTable_GetTileBehaviour, GetTileBehaviour);
    addToRSDKFunctionTable(FunctionTable_SetTileBehaviour, SetTileBehaviour);
    addToRSDKFunctionTable(FunctionTable_GetSFX, GetSFX);
    addToRSDKFunctionTable(FunctionTable_PlaySFX, PlaySfx);
    addToRSDKFunctionTable(FunctionTable_StopSFX, StopSfx);
    addToRSDKFunctionTable(FunctionTable_PlayMusic, PlayStream);
    addToRSDKFunctionTable(FunctionTable_SetChannelAttributes, SetChannelAttributes);
    addToRSDKFunctionTable(FunctionTable_StopChannel, StopChannel);
    addToRSDKFunctionTable(FunctionTable_PauseChannel, PauseChannel);
    addToRSDKFunctionTable(FunctionTable_ResumeChannel, ResumeChannel);
    addToRSDKFunctionTable(FunctionTable_SfxPlaying, SfxPlaying);
    addToRSDKFunctionTable(FunctionTable_ChannelActive, ChannelActive);
    addToRSDKFunctionTable(FunctionTable_GetChannelPos, GetChannelPos);
    addToRSDKFunctionTable(FunctionTable_LoadVideo, LoadVideo);
    addToRSDKFunctionTable(FunctionTable_LoadImage, LoadImage);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_ControllerIDForInputID, ControllerIDForInputID);
    addToRSDKFunctionTable(FunctionTable_MostRecentActiveControllerID, MostRecentActiveControllerID);
    addToRSDKFunctionTable(FunctionTable_Unknown100, Unknown100);
    addToRSDKFunctionTable(FunctionTable_GetAssignedControllerID, GetAssignedControllerID);
    addToRSDKFunctionTable(FunctionTable_GetAssignedUnknown, GetAssignedUnknown);
    addToRSDKFunctionTable(FunctionTable_Missing22, DoInputUnknown2);
    addToRSDKFunctionTable(FunctionTable_Missing23, DoInputUnknown3);
    addToRSDKFunctionTable(FunctionTable_Missing24, Missing24);
    addToRSDKFunctionTable(FunctionTable_Missing25, DoInputUnknown2_Active);
    addToRSDKFunctionTable(FunctionTable_Missing26, DoInputUnknown3_Active);
    addToRSDKFunctionTable(FunctionTable_AssignControllerID, AssignControllerID);
    addToRSDKFunctionTable(FunctionTable_InputIDIsDisconnected, InputIDIsDisconnected);
    addToRSDKFunctionTable(FunctionTable_ResetControllerAssignments, ResetControllerAssignments);
#endif
#if !RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_Unknown92, NullFunc);
#endif
    addToRSDKFunctionTable(FunctionTable_LoadUserFile, LoadUserFile);
    addToRSDKFunctionTable(FunctionTable_SaveUserFile, SaveUserFile);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_printLog, printLog);
    addToRSDKFunctionTable(FunctionTable_printString, printString);
    addToRSDKFunctionTable(FunctionTable_printText, printText);
    addToRSDKFunctionTable(FunctionTable_printIntegerUnsigned, printIntegerUnsigned);
    addToRSDKFunctionTable(FunctionTable_printInteger, printInteger);
    addToRSDKFunctionTable(FunctionTable_printFloat, printFloat);
    addToRSDKFunctionTable(FunctionTable_printVector2, printVector2);
    addToRSDKFunctionTable(FunctionTable_printHitbox, printHitbox);
#endif
    addToRSDKFunctionTable(FunctionTable_Unknown105, SetActiveVariable);
    addToRSDKFunctionTable(FunctionTable_Unknown106, AddEnumVar);
#if !RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_PrintMessage, PrintMessage);
#endif
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_ClearDebugValues, ClearDebugValues);
    addToRSDKFunctionTable(FunctionTable_SetDebugValue, SetDebugValue);
#endif
}

void LinkGameLogic(GameInfo *info) { printLog(SEVERITY_WARN, "Internal LinkGameLogic() function called, no logic will be linked"); }
