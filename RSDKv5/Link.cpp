#include "RetroEngine.hpp"

void *RSDKFunctionTable[FUNCTABLE_COUNT];

#if RETRO_REV02
void *APIFunctionTable[APITABLE_COUNT];

enum UserdataTableIDs {
    APITable_GetUserLanguage,
    APITable_GetConfirmButtonFlip,
    APITable_ExitGame,
    APITable_LaunchManual,
    APITable_IsOverlayEnabled,
    APITable_CheckDLC,
    APITable_ShowExtensionOverlay,
#if RETRO_VER_EGS
    APITable_ShowCheckoutPage,
    APITable_ShowEncorePage,
    APITable_EGS_Unknown4,
    APITable_RegisterHIDDevice,
#endif
    APITable_UnlockAchievement,
    APITable_GetAchievementsEnabled,
    APITable_SetAchievementsEnabled,
#if RETRO_VER_EGS
    APITable_CheckAchievementsEnabled,
    APITable_GetAchievementNames,
#endif
    APITable_LeaderboardsUnknown4,
#if RETRO_VER_EGS
    APITable_EGS_LeaderboardsUnknown4,
#endif
    APITable_FetchLeaderboard,
    APITable_TrackScore,
    APITable_GetLeaderboardsStatus,
    APITable_LeaderboardEntryCount,
    APITable_LeaderboardEntryLength,
    APITable_LoadNewLeaderboardEntries,
    APITable_ClearLeaderboardInfo,
    APITable_ReadLeaderboardEntry,
    APITable_SetPresence,
    APITable_TryTrackStat,
    APITable_GetStatsEnabled,
    APITable_SetStatsEnabled,
    APITable_ClearPrerollErrors,
    APITable_TryAuth,
    APITable_GetUserAuthStatus,
    APITable_GetUsername,
    APITable_TryInitStorage,
    APITable_GetUserStorageStatus,
    APITable_GetSaveStatus,
    APITable_ClearSaveStatus,
    APITable_SetSaveStatusContinue,
    APITable_SetSaveStatusOK,
    APITable_SetSaveStatusForbidden,
    APITable_SetSaveStatusError,
    APITable_SetUserStorageNoSave,
    APITable_GetUserStorageNoSave,
    APITable_LoadUserFile,
    APITable_SaveUserFile,
    APITable_DeleteUserFile,
    APITable_AddUserDBEntry,
    APITable_OpenUserDB,
    APITable_SaveUserDB,
    APITable_ClearUserDB,
    APITable_ClearAllUserDBs,
    APITable_SetupUserDBRowSorting,
    APITable_GetUserDBRowsChanged,
    APITable_AddUserDBRowSortFilter,
    APITable_SortUserDBRows,
    APITable_GetSortedUserDBRowCount,
    APITable_GetSortedUserDBRowID,
    APITable_AddUserDBRow,
    APITable_SetUserDBValue,
    APITable_GetUserDBValue,
    APITable_GetEntryUUID,
    APITable_GetUserDBRowByID,
    APITable_GetUserDBRowCreationTime,
    APITable_RemoveDBRow,
    APITable_RemoveAllDBEntries,
    APITable_Count,
};
#else
APITableEntry APIFunctionTable[APITABLE_COUNT];
int32 APIFunctionTableCount;
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
    FunctionTable_SetScene,
    FunctionTable_SetGameMode,
#if RETRO_REV02
    FunctionTable_SetHardResetFlag,
#endif
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectByName,
    FunctionTable_ClearCameras,
    FunctionTable_AddCamera,
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
    FunctionTable_RandSeeded,
    FunctionTable_SetRandSeed,
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
    FunctionTable_PrependText,
    FunctionTable_AppendString,
    FunctionTable_AppendText,
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
    FunctionTable_GetControllerType,
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
    FunctionTable_InputUnknown,
#endif
    FunctionTable_LoadUserFile,
    FunctionTable_SaveUserFile,
#if RETRO_REV02
    FunctionTable_PrintLog,
    FunctionTable_PrintString,
    FunctionTable_PrintText,
    FunctionTable_PrintIntegerUnsigned,
    FunctionTable_PrintInteger,
    FunctionTable_PrintFloat,
    FunctionTable_PrintVector2,
    FunctionTable_PrintHitbox,
#endif
    FunctionTable_SetActiveVariable,
    FunctionTable_AddEnumVariable,
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
#if RETRO_REV02
#define addToAPIFunctionTable(id, func) APIFunctionTable[id] = (void *)func;
#else
void addToAPIFunctionTable(const char *name, void *ptr)
{
    if (APIFunctionTableCount < APITABLE_COUNT) {
        RETRO_HASH(hash);
        GEN_HASH(name, hash);
        for (int f = 0; f < APIFunctionTableCount; ++f) {
            if (HASH_MATCH(hash, APIFunctionTable[f].hash))
                return; // already exists, ignore this call
        }

        HASH_COPY(APIFunctionTable[APIFunctionTableCount].hash, hash);
        APIFunctionTable[APIFunctionTableCount].ptr = ptr;
        APIFunctionTableCount++;
    }
}

void *GetAPIFunction(const char *name)
{
    if (!name)
        return NULL;

    uint hash[4];
    GEN_HASH(name, hash);
    for (int f = 0; f < APIFunctionTableCount; ++f) {
        if (HASH_MATCH(hash, APIFunctionTable[f].hash))
            return APIFunctionTable[f].ptr;
    }

    if (engine.printConsole)
        PrintLog(PRINT_POPUP, "API Function not found: %s", name);
    return NULL;
}
#endif

void setupFunctions()
{
#if RETRO_USE_MOD_LOADER
    if (RSDKFunctionTable[0]) {
        initModAPI();
        return;
    }
#endif
    CalculateTrigAngles();
    GenerateBlendLookupTable();
    InitSystemSurfaces();

    memset(RSDKFunctionTable, NULL, FUNCTABLE_COUNT * sizeof(void *));
#if RETRO_REV02
    memset(APIFunctionTable, NULL, APITABLE_COUNT * sizeof(void *));
#endif

#if RETRO_REV02
    // API functions
    addToAPIFunctionTable(APITable_GetUserLanguage, userCore->GetUserLanguage);
    addToAPIFunctionTable(APITable_GetConfirmButtonFlip, userCore->GetConfirmButtonFlip);
    addToAPIFunctionTable(APITable_ExitGame, userCore->ExitGame);
    addToAPIFunctionTable(APITable_LaunchManual, userCore->LaunchManual);
    addToAPIFunctionTable(APITable_IsOverlayEnabled, userCore->IsOverlayEnabled);
    addToAPIFunctionTable(APITable_CheckDLC, userCore->CheckDLC);
    addToAPIFunctionTable(APITable_ShowExtensionOverlay, userCore->ShowExtensionOverlay);
#if RETRO_VER_EGS
    addToAPIFunctionTable(APITable_ShowCheckoutPage, userCore->ShowCheckoutPage);
    addToAPIFunctionTable(APITable_ShowEncorePage, userCore->ShowEncorePage);
    addToAPIFunctionTable(APITable_EGS_Unknown4, userCore->EpicUnknown4);
    addToAPIFunctionTable(APITable_RegisterHIDDevice, userCore->RegisterHIDDevice);
#endif
    addToAPIFunctionTable(APITable_UnlockAchievement, achievements->UnlockAchievement);
    addToAPIFunctionTable(APITable_GetAchievementsEnabled, GetAchievementsEnabled);
    addToAPIFunctionTable(APITable_SetAchievementsEnabled, SetAchievementsEnabled);
#if RETRO_VER_EGS
    addToAPIFunctionTable(APITable_CheckAchievementsEnabled, achievements->CheckAchievementsEnabled);
    addToAPIFunctionTable(APITable_GetAchievementNames, achievements->GetAchievementNames);
#endif
    addToAPIFunctionTable(APITable_LeaderboardsUnknown4, NullFunc); // LeaderboardsUnknown4);
#if RETRO_VER_EGS
    addToAPIFunctionTable(APITable_EGS_LeaderboardsUnknown4, NullFunc); // EGS_LeaderboardsUnknown4);
#endif
    addToAPIFunctionTable(APITable_FetchLeaderboard, leaderboards->FetchLeaderboard);
    addToAPIFunctionTable(APITable_TrackScore, leaderboards->TrackScore);
    addToAPIFunctionTable(APITable_GetLeaderboardsStatus, leaderboards->GetStatus);
    addToAPIFunctionTable(APITable_LeaderboardEntryCount, LeaderboardEntryCount);
    addToAPIFunctionTable(APITable_LeaderboardEntryLength, LeaderboardEntryLength);
    addToAPIFunctionTable(APITable_LoadNewLeaderboardEntries, LoadNewLeaderboardEntries);
    addToAPIFunctionTable(APITable_ClearLeaderboardInfo, ClearLeaderboardInfo);
    addToAPIFunctionTable(APITable_ReadLeaderboardEntry, ReadLeaderboardEntry);
    addToAPIFunctionTable(APITable_SetPresence, richPresence->SetPresence);
    addToAPIFunctionTable(APITable_TryTrackStat, stats->TryTrackStat);
    addToAPIFunctionTable(APITable_GetStatsEnabled, GetStatsEnabled);
    addToAPIFunctionTable(APITable_SetStatsEnabled, SetStatsEnabled);
    addToAPIFunctionTable(APITable_ClearPrerollErrors, userStorage->ClearPrerollErrors);
    addToAPIFunctionTable(APITable_TryAuth, userStorage->TryAuth);
    addToAPIFunctionTable(APITable_GetUserAuthStatus, GetUserAuthStatus);
    addToAPIFunctionTable(APITable_GetUsername, userStorage->GetUsername);
    addToAPIFunctionTable(APITable_TryInitStorage, userStorage->TryInitStorage);
    addToAPIFunctionTable(APITable_GetUserStorageStatus, GetUserStorageStatus);
    addToAPIFunctionTable(APITable_GetSaveStatus, GetSaveStatus);
    addToAPIFunctionTable(APITable_ClearSaveStatus, ClearSaveStatus);
    addToAPIFunctionTable(APITable_SetSaveStatusContinue, SetSaveStatusContinue);
    addToAPIFunctionTable(APITable_SetSaveStatusOK, SetSaveStatusOK);
    addToAPIFunctionTable(APITable_SetSaveStatusForbidden, SetSaveStatusForbidden);
    addToAPIFunctionTable(APITable_SetSaveStatusError, SetSaveStatusError);
    addToAPIFunctionTable(APITable_SetUserStorageNoSave, SetUserStorageNoSave);
    addToAPIFunctionTable(APITable_GetUserStorageNoSave, GetUserStorageNoSave);
    addToAPIFunctionTable(APITable_LoadUserFile, userStorage->LoadUserFile);
    addToAPIFunctionTable(APITable_SaveUserFile, userStorage->SaveUserFile);
    addToAPIFunctionTable(APITable_DeleteUserFile, userStorage->DeleteUserFile);
    addToAPIFunctionTable(APITable_AddUserDBEntry, InitUserDB);
    addToAPIFunctionTable(APITable_OpenUserDB, LoadUserDB);
    addToAPIFunctionTable(APITable_SaveUserDB, SaveUserDB);
    addToAPIFunctionTable(APITable_ClearUserDB, ClearUserDB);
    addToAPIFunctionTable(APITable_ClearAllUserDBs, ClearAllUserDBs);
    addToAPIFunctionTable(APITable_SetupUserDBRowSorting, SetupUserDBRowSorting);
    addToAPIFunctionTable(APITable_GetUserDBRowsChanged, GetUserDBRowsChanged);
    addToAPIFunctionTable(APITable_AddUserDBRowSortFilter, AddUserDBRowSortFilter);
    addToAPIFunctionTable(APITable_SortUserDBRows, SortUserDBRows);
    addToAPIFunctionTable(APITable_GetSortedUserDBRowCount, GetSortedUserDBRowCount);
    addToAPIFunctionTable(APITable_GetSortedUserDBRowID, GetSortedUserDBRowID);
    addToAPIFunctionTable(APITable_AddUserDBRow, AddUserDBRow);
    addToAPIFunctionTable(APITable_SetUserDBValue, SetUserDBValue);
    addToAPIFunctionTable(APITable_GetUserDBValue, GetUserDBValue);
    addToAPIFunctionTable(APITable_GetEntryUUID, GetDBRowUUID);
    addToAPIFunctionTable(APITable_GetUserDBRowByID, GetUserDBRowByID);
    addToAPIFunctionTable(APITable_GetUserDBRowCreationTime, GetUserDBRowCreationTime);
    addToAPIFunctionTable(APITable_RemoveDBRow, RemoveDBRow);
    addToAPIFunctionTable(APITable_RemoveAllDBEntries, RemoveAllDBRows);
#else
    addToAPIFunctionTable("GetConfirmButtonFlip", GetConfirmButtonFlip);
    addToAPIFunctionTable("GetXYButtonFlip", GetXYButtonFlip);
    addToAPIFunctionTable("LaunchManual", LaunchManual);
    addToAPIFunctionTable("ExitGame", ExitGame);
    addToAPIFunctionTable("ClearAchievements", ClearAchievements);
    addToAPIFunctionTable("UnlockAchievement", TryUnlockAchievement);
    // addToAPIFunctionTable("FetchLeaderboard", FetchLeaderboard);
    // addToAPIFunctionTable("LeaderboardStatus", GetLeaderboardStatus);
    // addToAPIFunctionTable("LeaderboardEntryCount", LeaderboardEntryCount);
    // addToAPIFunctionTable("LeaderboardReadEntry", LeaderboardReadEntry);
    addToAPIFunctionTable("TrackActClear", TrackActClear);
    addToAPIFunctionTable("TrackTAClear", TrackTAClear);
    addToAPIFunctionTable("TrackEnemyDefeat", TrackEnemyDefeat);
    addToAPIFunctionTable("TrackGameProgress", TrackGameProgress);
    addToAPIFunctionTable("ClearPrerollErrors", ClearPrerollErrors);
    addToAPIFunctionTable("TryAuth", TryAuth);
    addToAPIFunctionTable("GetUserAuthStatus", GetUserAuthStatus);
    addToAPIFunctionTable("TryInitStorage", TryInitStorage);
    addToAPIFunctionTable("GetStorageStatus", GetUserStorageStatus);
    addToAPIFunctionTable("GetUsername", GetUserName);
    addToAPIFunctionTable("LoadUserFile", TryLoadUserFile);
    addToAPIFunctionTable("SaveUserFile", TrySaveUserFile);
    addToAPIFunctionTable("SaveSettingsINI", writeSettings);
    addToAPIFunctionTable("GetUserLanguage", GetUserLanguage);
    addToAPIFunctionTable("ControllerIDForInputID", ControllerIDForInputID);
    addToAPIFunctionTable("MostRecentActiveControllerID", MostRecentActiveControllerID);
    addToAPIFunctionTable("AssignControllerID", AssignControllerID);
    addToAPIFunctionTable("ResetControllerAssignments", ResetControllerAssignments);
    addToAPIFunctionTable("InputIDIsDisconnected", InputIDIsDisconnected);
    addToAPIFunctionTable("GetControllerType", GetControllerType);
    addToAPIFunctionTable("ShowSteamControllerOverlay", ShowExtensionOverlay);
    addToAPIFunctionTable("SetInputLEDColor", SetInputLEDColor);
    addToAPIFunctionTable("SetRichPresence", SetPresence);
#endif

    // Function Table
    addToRSDKFunctionTable(FunctionTable_InitGameOptions, RegisterGlobalVariables);
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
    addToRSDKFunctionTable(FunctionTable_SwapDrawLayers, SwapDrawListEntries);
    addToRSDKFunctionTable(FunctionTable_SetDrawLayerProperties, SetDrawLayerProperties);
    addToRSDKFunctionTable(FunctionTable_SetScene, SetScene);
    addToRSDKFunctionTable(FunctionTable_SetGameMode, SetEngineState);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_SetHardResetFlag, SetHardResetFlag);
#endif
    addToRSDKFunctionTable(FunctionTable_CheckValidScene, CheckValidStage);
    addToRSDKFunctionTable(FunctionTable_CheckSceneFolder, CheckSceneFolder);
    addToRSDKFunctionTable(FunctionTable_InitSceneLoad, InitSceneLoad);
    addToRSDKFunctionTable(FunctionTable_GetObjectByName, GetObjectByName);
    addToRSDKFunctionTable(FunctionTable_ClearCameras, ClearCameras);
    addToRSDKFunctionTable(FunctionTable_AddCamera, AddCamera);
#if !RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_GetFuncPtr, GetAPIFunction);
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
    addToRSDKFunctionTable(FunctionTable_Rand, GetRandomValue);
    addToRSDKFunctionTable(FunctionTable_RandSeeded, GetSeededRandomValue);
    addToRSDKFunctionTable(FunctionTable_SetRandSeed, SetRandSeed);
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
    addToRSDKFunctionTable(FunctionTable_PrependText, PrependText);
    addToRSDKFunctionTable(FunctionTable_AppendString, AppendString);
    addToRSDKFunctionTable(FunctionTable_AppendText, AppendText);
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
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_SetLookupTable, SetLookupTable);
#else
    //cant be bothered to change the enum name, but in rev01, this returns a ptr to the lookup table
    addToRSDKFunctionTable(FunctionTable_SetLookupTable, GetLookupTable);
#endif
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
    addToRSDKFunctionTable(FunctionTable_LoadAnimation, LoadSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_CreateAnimation, CreateSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_SetSpriteAnimation, SetSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_EditAnimation, EditSpriteAnimation);
    addToRSDKFunctionTable(FunctionTable_SetSpriteString, SetSpriteString);
    addToRSDKFunctionTable(FunctionTable_GetAnimation, GetSpriteAnimation);
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
    addToRSDKFunctionTable(FunctionTable_GetSFX, GetSfx);
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
    addToRSDKFunctionTable(FunctionTable_GetControllerType, GetControllerType);
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
    addToRSDKFunctionTable(FunctionTable_InputUnknown, InputUnknown);
#endif
    addToRSDKFunctionTable(FunctionTable_LoadUserFile, LoadUserFile);
    addToRSDKFunctionTable(FunctionTable_SaveUserFile, SaveUserFile);
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_PrintLog, PrintLog);
    addToRSDKFunctionTable(FunctionTable_PrintString, PrintString);
    addToRSDKFunctionTable(FunctionTable_PrintText, PrintText);
    addToRSDKFunctionTable(FunctionTable_PrintIntegerUnsigned, PrintIntegerUnsigned);
    addToRSDKFunctionTable(FunctionTable_PrintInteger, PrintInteger);
    addToRSDKFunctionTable(FunctionTable_PrintFloat, PrintFloat);
    addToRSDKFunctionTable(FunctionTable_PrintVector2, PrintVector2);
    addToRSDKFunctionTable(FunctionTable_PrintHitbox, PrintHitbox);
#endif
    addToRSDKFunctionTable(FunctionTable_SetActiveVariable, SetActiveVariable);
    addToRSDKFunctionTable(FunctionTable_AddEnumVariable, AddEnumVariable);
#if !RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_PrintMessage, PrintMessage);
#endif
#if RETRO_REV02
    addToRSDKFunctionTable(FunctionTable_ClearDebugValues, ClearDebugValues);
    addToRSDKFunctionTable(FunctionTable_SetDebugValue, SetDebugValue);
#endif

#if RETRO_USE_MOD_LOADER
    initModAPI(); //setup mods & the mod API table
#endif
}

void LinkGameLogic(GameInfo *info) { PrintLog(PRINT_POPUP, "Internal LinkGameLogic() function called, no logic will be linked"); }
