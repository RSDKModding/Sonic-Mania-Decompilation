#include "RSDK/Core/RetroEngine.hpp"

using namespace RSDK;

void *RSDK::RSDKFunctionTable[FunctionTable_Count];

#if RETRO_REV02
void *RSDK::APIFunctionTable[APITable_Count];

RSDK::SKU::SKUInfo RSDK::SKU::curSKU;
RSDK::SKU::UnknownInfo RSDK::SKU::unknownInfo;
#else
RSDK::APITableEntry RSDK::APIFunctionTable[APITABLE_COUNT];
int32 RSDK::APIFunctionTableCount;
#endif

RSDK::GameVersionInfo RSDK::gameVerInfo;

void NullFunc() {}

#define ADD_RSDK_FUNCTION(id, func) RSDKFunctionTable[id] = (void *)func;
#if RETRO_REV02
#define ADD_API_FUNCTION(id, func) APIFunctionTable[id] = (void *)func;
#else
#define ADD_API_FUNCTION(name, func) SetAPIFunction(name, (void *)func);

void SetAPIFunction(const char *name, void *ptr)
{
    if (APIFunctionTableCount < APITABLE_COUNT) {
        RETRO_HASH_MD5(hash);
        GEN_HASH_MD5(name, hash);

        for (int32 f = 0; f < APIFunctionTableCount; ++f) {
            if (HASH_MATCH_MD5(hash, APIFunctionTable[f].hash))
                return; // already exists, ignore this call
        }

        HASH_COPY_MD5(APIFunctionTable[APIFunctionTableCount].hash, hash);
        APIFunctionTable[APIFunctionTableCount].ptr = ptr;
        APIFunctionTableCount++;
    }
}

void *GetAPIFunction(const char *name)
{
    if (!name)
        return NULL;

    RETRO_HASH_MD5(hash);
    GEN_HASH_MD5(name, hash);

    for (int32 f = 0; f < APIFunctionTableCount; ++f) {
        if (HASH_MATCH_MD5(hash, APIFunctionTable[f].hash))
            return APIFunctionTable[f].ptr;
    }

    if (engine.consoleEnabled)
        PrintLog(PRINT_POPUP, "API Function not found: %s", name);

    return NULL;
}
#endif

void RSDK::SetupFunctionTables()
{
    using namespace RSDK;
    using namespace RSDK::SKU;

#if RETRO_USE_MOD_LOADER
    if (RSDKFunctionTable[0]) {
        InitModAPI();
        return;
    }
#endif

    CalculateTrigAngles();
    GenerateBlendLookupTable();
    InitSystemSurfaces();

    memset(RSDKFunctionTable, 0, sizeof(RSDKFunctionTable));
#if RETRO_REV02
    memset(APIFunctionTable, 0, sizeof(APIFunctionTable));
#endif

#if RETRO_REV02

    // ============================
    // API Function Table (Rev02)
    // ============================

    // API Core
    ADD_API_FUNCTION(APITable_GetUserLanguage, GetUserLanguage);
    ADD_API_FUNCTION(APITable_GetConfirmButtonFlip, GetConfirmButtonFlip);
    ADD_API_FUNCTION(APITable_ExitGame, ExitGame);
    ADD_API_FUNCTION(APITable_LaunchManual, LaunchManual);
    ADD_API_FUNCTION(APITable_IsOverlayEnabled, IsOverlayEnabled);
    ADD_API_FUNCTION(APITable_CheckDLC, CheckDLC);
#if RETRO_VER_EGS
    ADD_API_FUNCTION(APITable_SetupExtensionOverlay, SetupExtensionOverlay);
    ADD_API_FUNCTION(APITable_CanShowExtensionOverlay, CanShowExtensionOverlay);
#endif
    ADD_API_FUNCTION(APITable_ShowExtensionOverlay, ShowExtensionOverlay);
#if RETRO_VER_EGS
    ADD_API_FUNCTION(APITable_CanShowAltExtensionOverlay, CanShowAltExtensionOverlay);
    ADD_API_FUNCTION(APITable_ShowAltExtensionOverlay, ShowAltExtensionOverlay);
    ADD_API_FUNCTION(APITable_GetConnectingStringID, GetConnectingStringID);
    ADD_API_FUNCTION(APITable_ShowLimitedVideoOptions, ShowLimitedVideoOptions);
#endif

    // Achievements
    ADD_API_FUNCTION(APITable_UnlockAchievement, TryUnlockAchievement);
    ADD_API_FUNCTION(APITable_GetAchievementsEnabled, GetAchievementsEnabled);
    ADD_API_FUNCTION(APITable_SetAchievementsEnabled, SetAchievementsEnabled);
#if RETRO_VER_EGS
    ADD_API_FUNCTION(APITable_CheckAchievementsEnabled, CheckAchievementsEnabled);
    ADD_API_FUNCTION(APITable_SetAchievementNames, SetAchievementNames);
#endif

    // Leaderboards
    ADD_API_FUNCTION(APITable_InitLeaderboards, InitLeaderboards);
#if RETRO_VER_EGS
    ADD_API_FUNCTION(APITable_CheckLeaderboardsEnabled, CheckLeaderboardsEnabled);
#endif
    ADD_API_FUNCTION(APITable_FetchLeaderboard, FetchLeaderboard);
    ADD_API_FUNCTION(APITable_TrackScore, TrackScore);
    ADD_API_FUNCTION(APITable_GetLeaderboardsStatus, GetLeaderboardsStatus);
    ADD_API_FUNCTION(APITable_LeaderboardEntryViewSize, LeaderboardEntryViewSize);
    ADD_API_FUNCTION(APITable_LeaderboardEntryLoadSize, LeaderboardEntryLoadSize);
    ADD_API_FUNCTION(APITable_LoadLeaderboardEntries, LoadLeaderboardEntries);
    ADD_API_FUNCTION(APITable_ResetLeaderboardInfo, ResetLeaderboardInfo);
    ADD_API_FUNCTION(APITable_ReadLeaderboardEntry, ReadLeaderboardEntry);

    // Rich Presence
    ADD_API_FUNCTION(APITable_SetPresence, SetPresence);

    // Stats
    ADD_API_FUNCTION(APITable_TryTrackStat, TryTrackStat);
    ADD_API_FUNCTION(APITable_GetStatsEnabled, GetStatsEnabled);
    ADD_API_FUNCTION(APITable_SetStatsEnabled, SetStatsEnabled);

    // Authorization & Storage
    ADD_API_FUNCTION(APITable_ClearPrerollErrors, ClearPrerollErrors);
    ADD_API_FUNCTION(APITable_TryAuth, TryAuth);
    ADD_API_FUNCTION(APITable_GetUserAuthStatus, GetUserAuthStatus);
    ADD_API_FUNCTION(APITable_GetUsername, GetUsername);
    ADD_API_FUNCTION(APITable_TryInitStorage, TryInitStorage);
    ADD_API_FUNCTION(APITable_GetUserStorageStatus, GetUserStorageStatus);

    // Saving
    ADD_API_FUNCTION(APITable_GetSaveStatus, GetSaveStatus);
    ADD_API_FUNCTION(APITable_ClearSaveStatus, ClearSaveStatus);
    ADD_API_FUNCTION(APITable_SetSaveStatusContinue, SetSaveStatusContinue);
    ADD_API_FUNCTION(APITable_SetSaveStatusOK, SetSaveStatusOK);
    ADD_API_FUNCTION(APITable_SetSaveStatusForbidden, SetSaveStatusForbidden);
    ADD_API_FUNCTION(APITable_SetSaveStatusError, SetSaveStatusError);
    ADD_API_FUNCTION(APITable_SetUserStorageNoSave, SetUserStorageNoSave);
    ADD_API_FUNCTION(APITable_GetUserStorageNoSave, GetUserStorageNoSave);

    // User File Management
    ADD_API_FUNCTION(APITable_LoadUserFile, TryLoadUserFile);     // load user file from game dir
    ADD_API_FUNCTION(APITable_SaveUserFile, TrySaveUserFile);     // save user file to game dir
    ADD_API_FUNCTION(APITable_DeleteUserFile, TryDeleteUserFile); // delete user file from game dir

    // User DBs
    ADD_API_FUNCTION(APITable_InitUserDB, InitUserDB);
    ADD_API_FUNCTION(APITable_OpenUserDB, LoadUserDB);
    ADD_API_FUNCTION(APITable_SaveUserDB, SaveUserDB);
    ADD_API_FUNCTION(APITable_ClearUserDB, ClearUserDB);
    ADD_API_FUNCTION(APITable_ClearAllUserDBs, ClearAllUserDBs);
    ADD_API_FUNCTION(APITable_SetupUserDBRowSorting, SetupUserDBRowSorting);
    ADD_API_FUNCTION(APITable_GetUserDBRowsChanged, GetUserDBRowsChanged);
    ADD_API_FUNCTION(APITable_AddUserDBRowSortFilter, AddUserDBRowSortFilter);
    ADD_API_FUNCTION(APITable_SortUserDBRows, SortUserDBRows);
    ADD_API_FUNCTION(APITable_GetSortedUserDBRowCount, GetSortedUserDBRowCount);
    ADD_API_FUNCTION(APITable_GetSortedUserDBRowID, GetSortedUserDBRowID);
    ADD_API_FUNCTION(APITable_AddUserDBRow, AddUserDBRow);
    ADD_API_FUNCTION(APITable_SetUserDBValue, SetUserDBValue);
    ADD_API_FUNCTION(APITable_GetUserDBValue, GetUserDBValue);
    ADD_API_FUNCTION(APITable_GetRowUUID, GetDBRowUUID);
    ADD_API_FUNCTION(APITable_GetUserDBRowByID, GetUserDBRowByID);
    ADD_API_FUNCTION(APITable_GetUserDBRowCreationTime, GetUserDBRowCreationTime);
    ADD_API_FUNCTION(APITable_RemoveDBRow, RemoveDBRow);
    ADD_API_FUNCTION(APITable_RemoveAllDBRows, RemoveAllDBRows);
#else
    // ============================
    // API Functions (Rev01)
    // ============================

    // API Core
    ADD_API_FUNCTION("GetConfirmButtonFlip", GetConfirmButtonFlip);
    // APICallback uses the sku one anyways if this isn't set, this is only needed if it needs to interact with the backend APIs
    // ADD_API_FUNCTION("GetUserLanguage", GetUserLanguage);
    ADD_API_FUNCTION("GetXYButtonFlip", GetXYButtonFlip);
    ADD_API_FUNCTION("LaunchManual", LaunchManual);
    ADD_API_FUNCTION("ExitGame", ExitGame);

    // Achievements
    ADD_API_FUNCTION("ClearAchievements", ClearAchievements);
    ADD_API_FUNCTION("UnlockAchievement", TryUnlockAchievement);

    // Leaderboards
    // ADD_API_FUNCTION("FetchLeaderboard", FetchLeaderboard);
    // ADD_API_FUNCTION("LeaderboardStatus", GetLeaderboardStatus);
    // ADD_API_FUNCTION("LeaderboardEntryCount", LeaderboardEntryCount);
    // ADD_API_FUNCTION("LeaderboardReadEntry", LeaderboardReadEntry);

    // Rich Presence
    // ADD_API_FUNCTION("SetRichPresence", SetPresence);

    // Stats
    ADD_API_FUNCTION("TrackActClear", TrackActClear);
    ADD_API_FUNCTION("TrackTAClear", TrackTAClear);
    ADD_API_FUNCTION("TrackEnemyDefeat", TrackEnemyDefeat);
    ADD_API_FUNCTION("TrackGameProgress", TrackGameProgress);

    // Authorization & Storage
    // ADD_API_FUNCTION("ClearPrerollErrors", ClearPrerollErrors); // Dummy behaviour is managed by APICallback
    // ADD_API_FUNCTION("TryAuth", TryAuth); // Dummy behaviour is managed by APICallback
    // ADD_API_FUNCTION("GetUserAuthStatus", GetUserAuthStatus); // Dummy behaviour is managed by APICallback
    // ADD_API_FUNCTION("TryInitStorage", TryInitStorage); // Dummy behaviour is managed by APICallback
    // ADD_API_FUNCTION("GetStorageStatus", GetUserStorageStatus); // Dummy behaviour is managed by APICallback
    // ADD_API_FUNCTION("GetUsername", GetUsername); // APICallback sets the dummy one anyways if this isn't set

    // User File Management
    ADD_API_FUNCTION("LoadUserFile", TryLoadUserFile);
    ADD_API_FUNCTION("SaveUserFile", TrySaveUserFile);
    ADD_API_FUNCTION("SaveSettingsINI", SaveSettingsINI);

    // Input
    ADD_API_FUNCTION("ControllerIDForInputID", ControllerIDForInputID);
    ADD_API_FUNCTION("MostRecentActiveControllerID", MostRecentActiveControllerID);
    ADD_API_FUNCTION("AssignControllerID", AssignControllerID);
    ADD_API_FUNCTION("ResetControllerAssignments", ResetControllerAssignments);
    ADD_API_FUNCTION("InputIDIsDisconnected", InputIDIsDisconnected);
    ADD_API_FUNCTION("GetControllerType", GetControllerType);
    ADD_API_FUNCTION("ShowSteamControllerOverlay", ShowExtensionOverlay);
    ADD_API_FUNCTION("SetInputLEDColor", SetInputLEDColor);
#endif

    // ============================
    // RSDK Function Table
    // ============================

    // Registration
    ADD_RSDK_FUNCTION(FunctionTable_RegisterGlobalVariables, RegisterGlobalVariables);
    ADD_RSDK_FUNCTION(FunctionTable_RegisterObject, RegisterObject);
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_RegisterStaticVariables, RegisterStaticVariables);
#endif

    // Entities & Objects
    ADD_RSDK_FUNCTION(FunctionTable_GetActiveEntities, GetActiveEntities);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntities, GetEntities);
    ADD_RSDK_FUNCTION(FunctionTable_BreakForeachLoop, BreakForeachLoop);
    ADD_RSDK_FUNCTION(FunctionTable_SetEditableVar, SetEditableVar);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntity, GetEntity);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntitySlot, GetEntitySlot);
    ADD_RSDK_FUNCTION(FunctionTable_GetEntityCount, GetEntityCount);
    ADD_RSDK_FUNCTION(FunctionTable_GetDrawListRef, GetDrawListRef);
    ADD_RSDK_FUNCTION(FunctionTable_GetDrawListRefPtr, GetDrawListRefPtr);
    ADD_RSDK_FUNCTION(FunctionTable_ResetEntityPtr, ResetEntityPtr);
    ADD_RSDK_FUNCTION(FunctionTable_ResetEntitySlot, ResetEntitySlot);
    ADD_RSDK_FUNCTION(FunctionTable_CreateEntity, CreateEntity);
    ADD_RSDK_FUNCTION(FunctionTable_CopyEntity, CopyEntity);
    ADD_RSDK_FUNCTION(FunctionTable_CheckOnScreen, CheckOnScreen);
    ADD_RSDK_FUNCTION(FunctionTable_CheckPosOnScreen, CheckPosOnScreen);
    ADD_RSDK_FUNCTION(FunctionTable_AddDrawListRef, AddDrawListRef);
    ADD_RSDK_FUNCTION(FunctionTable_SwapDrawListEntries, SwapDrawListEntries);
    ADD_RSDK_FUNCTION(FunctionTable_SetDrawLayerProperties, SetDrawLayerProperties);

    // Scene Management
    ADD_RSDK_FUNCTION(FunctionTable_SetScene, SetScene);
    ADD_RSDK_FUNCTION(FunctionTable_SetEngineState, SetEngineState);
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_ForceHardReset, ForceHardReset);
#endif
    ADD_RSDK_FUNCTION(FunctionTable_CheckValidScene, CheckValidStage);
    ADD_RSDK_FUNCTION(FunctionTable_CheckSceneFolder, CheckSceneFolder);
    ADD_RSDK_FUNCTION(FunctionTable_InitSceneLoad, InitSceneLoad);
    ADD_RSDK_FUNCTION(FunctionTable_FindObject, FindObject);

    // Cameras
    ADD_RSDK_FUNCTION(FunctionTable_ClearCameras, ClearCameras);
    ADD_RSDK_FUNCTION(FunctionTable_AddCamera, AddCamera);

    // API (Rev01 only)
#if !RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_GetAPIFunction, GetAPIFunction);
#endif

    // Window/Video Settings
    ADD_RSDK_FUNCTION(FunctionTable_GetVideoSetting, GetVideoSetting);
    ADD_RSDK_FUNCTION(FunctionTable_SetVideoSetting, SetVideoSetting);
    ADD_RSDK_FUNCTION(FunctionTable_UpdateWindow, UpdateGameWindow)

    // Math
    ADD_RSDK_FUNCTION(FunctionTable_Sin1024, Sin1024);
    ADD_RSDK_FUNCTION(FunctionTable_Cos1024, Cos1024);
    ADD_RSDK_FUNCTION(FunctionTable_ATan1024, Tan1024);
    ADD_RSDK_FUNCTION(FunctionTable_ASin1024, ASin1024);
    ADD_RSDK_FUNCTION(FunctionTable_ACos1024, ACos1024);
    ADD_RSDK_FUNCTION(FunctionTable_Sin512, Sin512);
    ADD_RSDK_FUNCTION(FunctionTable_Cos512, Cos512);
    ADD_RSDK_FUNCTION(FunctionTable_ATan512, Tan512);
    ADD_RSDK_FUNCTION(FunctionTable_ASin512, ASin512);
    ADD_RSDK_FUNCTION(FunctionTable_ACos512, ACos512);
    ADD_RSDK_FUNCTION(FunctionTable_Sin256, Sin256);
    ADD_RSDK_FUNCTION(FunctionTable_Cos256, Cos256);
    ADD_RSDK_FUNCTION(FunctionTable_ATan256, Tan256);
    ADD_RSDK_FUNCTION(FunctionTable_ASin256, ASin256);
    ADD_RSDK_FUNCTION(FunctionTable_ACos256, ACos256);
    ADD_RSDK_FUNCTION(FunctionTable_Rand, GetRandomValue);
    ADD_RSDK_FUNCTION(FunctionTable_RandSeeded, GetSeededRandomValue);
    ADD_RSDK_FUNCTION(FunctionTable_SetRandSeed, SetRandSeed);
    ADD_RSDK_FUNCTION(FunctionTable_ATan2, ArcTanLookup);

    // Matrices
    ADD_RSDK_FUNCTION(FunctionTable_SetIdentityMatrix, SetIdentityMatrix);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixMultiply, MatrixMultiply);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixTranslateXYZ, MatrixTranslateXYZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixScaleXYZ, MatrixScaleXYZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateX, MatrixRotateX);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateY, MatrixRotateY);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateZ, MatrixRotateZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixRotateXYZ, MatrixRotateXYZ);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixInverse, MatrixInverse);
    ADD_RSDK_FUNCTION(FunctionTable_MatrixCopy, MatrixCopy);

    // Strings
    ADD_RSDK_FUNCTION(FunctionTable_InitString, InitString);
    ADD_RSDK_FUNCTION(FunctionTable_CopyString, CopyString);
    ADD_RSDK_FUNCTION(FunctionTable_SetString, SetString);
    ADD_RSDK_FUNCTION(FunctionTable_AppendString, AppendString);
    ADD_RSDK_FUNCTION(FunctionTable_AppendText, AppendText);
    ADD_RSDK_FUNCTION(FunctionTable_LoadStringList, LoadStringList);
    ADD_RSDK_FUNCTION(FunctionTable_SplitStringList, SplitStringList);
    ADD_RSDK_FUNCTION(FunctionTable_GetCString, GetCString);
    ADD_RSDK_FUNCTION(FunctionTable_CompareStrings, CompareStrings);

    // Screens & Displays
    ADD_RSDK_FUNCTION(FunctionTable_GetDisplayInfo, GetDisplayInfo);
    ADD_RSDK_FUNCTION(FunctionTable_GetWindowSize, GetWindowSize);
    ADD_RSDK_FUNCTION(FunctionTable_SetScreenSize, SetScreenSize);
    ADD_RSDK_FUNCTION(FunctionTable_SetClipBounds, SetClipBounds);
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_SetScreenRenderVertices, SetScreenRenderVertices);
#endif

    // Spritesheets
    ADD_RSDK_FUNCTION(FunctionTable_LoadSpriteSheet, LoadSpriteSheet);

    // Palettes & Colors
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_SetTintLookupTable, SetTintLookupTable);
#else
    // cant be bothered to change the enum name lol
    ADD_RSDK_FUNCTION(FunctionTable_SetTintLookupTable, GetTintLookupTable);
#endif
    ADD_RSDK_FUNCTION(FunctionTable_SetPaletteMask, SetPaletteMask);
    ADD_RSDK_FUNCTION(FunctionTable_SetPaletteEntry, SetPaletteEntry);
    ADD_RSDK_FUNCTION(FunctionTable_GetPaletteEntry, GetPaletteEntry);
    ADD_RSDK_FUNCTION(FunctionTable_SetActivePalette, SetActivePalette);
    ADD_RSDK_FUNCTION(FunctionTable_CopyPalette, CopyPalette);
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_LoadPalette, LoadPalette);
#endif
    ADD_RSDK_FUNCTION(FunctionTable_RotatePalette, RotatePalette);
    ADD_RSDK_FUNCTION(FunctionTable_SetLimitedFade, SetPaletteFade);
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_BlendColors, BlendColors);
#endif

    // Drawing
    ADD_RSDK_FUNCTION(FunctionTable_DrawRect, DrawRectangle);
    ADD_RSDK_FUNCTION(FunctionTable_DrawLine, DrawLine);
    ADD_RSDK_FUNCTION(FunctionTable_DrawCircle, DrawCircle);
    ADD_RSDK_FUNCTION(FunctionTable_DrawCircleOutline, DrawCircleOutline);
    ADD_RSDK_FUNCTION(FunctionTable_DrawFace, DrawFace);
    ADD_RSDK_FUNCTION(FunctionTable_DrawBlendedFace, DrawBlendedFace);
    ADD_RSDK_FUNCTION(FunctionTable_DrawSprite, DrawSprite);
    ADD_RSDK_FUNCTION(FunctionTable_DrawDeformedSprite, DrawDeformedSprite);
    ADD_RSDK_FUNCTION(FunctionTable_DrawString, DrawString);
    ADD_RSDK_FUNCTION(FunctionTable_DrawTile, DrawTile);
    ADD_RSDK_FUNCTION(FunctionTable_CopyTile, CopyTile);
    ADD_RSDK_FUNCTION(FunctionTable_DrawAniTile, DrawAniTile);
    ADD_RSDK_FUNCTION(FunctionTable_FillScreen, FillScreen);

    // Meshes & 3D Scenes
    ADD_RSDK_FUNCTION(FunctionTable_LoadMesh, LoadMesh);
    ADD_RSDK_FUNCTION(FunctionTable_Create3DScene, Create3DScene);
    ADD_RSDK_FUNCTION(FunctionTable_Prepare3DScene, Prepare3DScene);
    ADD_RSDK_FUNCTION(FunctionTable_SetDiffuseColor, SetDiffuseColor);
    ADD_RSDK_FUNCTION(FunctionTable_SetDiffuseIntensity, SetDiffuseIntensity);
    ADD_RSDK_FUNCTION(FunctionTable_SetSpecularIntensity, SetSpecularIntensity);
    ADD_RSDK_FUNCTION(FunctionTable_AddModelToScene, AddModelToScene);
    ADD_RSDK_FUNCTION(FunctionTable_SetModelAnimation, SetMeshAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_AddMeshFrameToScene, AddMeshFrameToScene);
    ADD_RSDK_FUNCTION(FunctionTable_Draw3DScene, Draw3DScene);

    // Sprite Animations & Frames
    ADD_RSDK_FUNCTION(FunctionTable_LoadSpriteAnimation, LoadSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_CreateSpriteAnimation, CreateSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_SetSpriteAnimation, SetSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_EditSpriteAnimation, EditSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_SetSpriteString, SetSpriteString);
    ADD_RSDK_FUNCTION(FunctionTable_FindSpriteAnimation, FindSpriteAnimation);
    ADD_RSDK_FUNCTION(FunctionTable_GetFrame, GetFrame);
    ADD_RSDK_FUNCTION(FunctionTable_GetHitbox, GetHitbox);
    ADD_RSDK_FUNCTION(FunctionTable_GetFrameID, GetFrameID);
    ADD_RSDK_FUNCTION(FunctionTable_GetStringWidth, GetStringWidth);
    ADD_RSDK_FUNCTION(FunctionTable_ProcessAnimation, ProcessAnimation);

    // Tile Layers
    ADD_RSDK_FUNCTION(FunctionTable_GetTileLayer, GetTileLayer);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileLayerID, GetTileLayerID);
    ADD_RSDK_FUNCTION(FunctionTable_GetLayerSize, GetLayerSize);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileInfo, GetTileInfo);
    ADD_RSDK_FUNCTION(FunctionTable_SetTileInfo, SetTileInfo);
    ADD_RSDK_FUNCTION(FunctionTable_CopyTileLayer, CopyTileLayout);
    ADD_RSDK_FUNCTION(FunctionTable_ProcessParallax, ProcessParallax);
    ADD_RSDK_FUNCTION(FunctionTable_GetScanlines, GetScanlines);

    // Object & Tile Collisions
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionTouch, CheckObjectCollisionTouch);
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionCircle, CheckObjectCollisionCircle);
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionBox, CheckObjectCollisionBox);
    ADD_RSDK_FUNCTION(FunctionTable_CheckObjectCollisionPlatform, CheckObjectCollisionPlatform);
    ADD_RSDK_FUNCTION(FunctionTable_ObjectTileCollision, ObjectTileCollision);
    ADD_RSDK_FUNCTION(FunctionTable_ObjectTileGrip, ObjectTileGrip);
    ADD_RSDK_FUNCTION(FunctionTable_ProcessObjectMovement, ProcessObjectMovement);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileAngle, GetTileAngle);
    ADD_RSDK_FUNCTION(FunctionTable_SetTileAngle, SetTileAngle);
    ADD_RSDK_FUNCTION(FunctionTable_GetTileFlags, GetTileFlags);
    ADD_RSDK_FUNCTION(FunctionTable_SetTileFlags, SetTileFlags);

    // Audio
    ADD_RSDK_FUNCTION(FunctionTable_GetSfx, GetSfx);
    ADD_RSDK_FUNCTION(FunctionTable_PlaySfx, PlaySfx);
    ADD_RSDK_FUNCTION(FunctionTable_StopSfx, StopSfx);
    ADD_RSDK_FUNCTION(FunctionTable_PlayMusic, PlayStream);
    ADD_RSDK_FUNCTION(FunctionTable_SetChannelAttributes, SetChannelAttributes);
    ADD_RSDK_FUNCTION(FunctionTable_StopChannel, StopChannel);
    ADD_RSDK_FUNCTION(FunctionTable_PauseChannel, PauseChannel);
    ADD_RSDK_FUNCTION(FunctionTable_ResumeChannel, ResumeChannel);
    ADD_RSDK_FUNCTION(FunctionTable_SfxPlaying, SfxPlaying);
    ADD_RSDK_FUNCTION(FunctionTable_ChannelActive, ChannelActive);
    ADD_RSDK_FUNCTION(FunctionTable_GetChannelPos, GetChannelPos);

    // Videos & "HD Images"
    ADD_RSDK_FUNCTION(FunctionTable_LoadVideo, LoadVideo);
    ADD_RSDK_FUNCTION(FunctionTable_LoadImage, LoadImage);

    // Input
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_ControllerIDForInputID, ControllerIDForInputID);
    ADD_RSDK_FUNCTION(FunctionTable_MostRecentActiveControllerID, MostRecentActiveControllerID);
    ADD_RSDK_FUNCTION(FunctionTable_GetControllerType, GetControllerType);
    ADD_RSDK_FUNCTION(FunctionTable_GetAssignedControllerID, GetAssignedControllerID);
    ADD_RSDK_FUNCTION(FunctionTable_GetInputUnknown, GetInputUnknown);
    ADD_RSDK_FUNCTION(FunctionTable_InputUnknown1, InputUnknown1);
    ADD_RSDK_FUNCTION(FunctionTable_InputUnknown2, InputUnknown2);
    ADD_RSDK_FUNCTION(FunctionTable_GetControllerUnknown, GetControllerUnknown);
    ADD_RSDK_FUNCTION(FunctionTable_ControllerUnknown1, ControllerUnknown1);
    ADD_RSDK_FUNCTION(FunctionTable_ControllerUnknown2, ControllerUnknown2);
    ADD_RSDK_FUNCTION(FunctionTable_AssignControllerID, AssignControllerID);
    ADD_RSDK_FUNCTION(FunctionTable_InputIDIsConnected, InputIDIsConnected);
    ADD_RSDK_FUNCTION(FunctionTable_ResetControllerAssignments, ResetControllerAssignments);
#endif
#if !RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_GetUnknownInputValue, GetUnknownInputValue);
#endif

    // User File Management
    ADD_RSDK_FUNCTION(FunctionTable_LoadUserFile, LoadUserFile); // load user file from exe dir
    ADD_RSDK_FUNCTION(FunctionTable_SaveUserFile, SaveUserFile); // save use file to exe dir

    // Printing (Rev02)
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_PrintLog, PrintLog);
    ADD_RSDK_FUNCTION(FunctionTable_PrintText, PrintText);
    ADD_RSDK_FUNCTION(FunctionTable_PrintString, PrintString);
    ADD_RSDK_FUNCTION(FunctionTable_PrintUInt32, PrintUInt32);
    ADD_RSDK_FUNCTION(FunctionTable_PrintInt32, PrintInt32);
    ADD_RSDK_FUNCTION(FunctionTable_PrintFloat, PrintFloat);
    ADD_RSDK_FUNCTION(FunctionTable_PrintVector2, PrintVector2);
    ADD_RSDK_FUNCTION(FunctionTable_PrintHitbox, PrintHitbox);
#endif

    // Editor
    ADD_RSDK_FUNCTION(FunctionTable_SetActiveVariable, SetActiveVariable);
    ADD_RSDK_FUNCTION(FunctionTable_AddEnumVariable, AddEnumVariable);

    // Debugging
#if RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_ClearDebugValues, ClearViewableVariables);
    ADD_RSDK_FUNCTION(FunctionTable_SetDebugValue, AddViewableVariable);
#endif

    // Printing (Rev01)
#if !RETRO_REV02
    ADD_RSDK_FUNCTION(FunctionTable_PrintMessage, PrintMessage);
#endif

#if RETRO_USE_MOD_LOADER
    InitModAPI(); // setup mods & the mod API table
#endif
}

#if RETRO_REV02
void RSDK::LinkGameLogic(void *info) { PrintLog(PRINT_POPUP, "Internal LinkGameLogic() function called, no logic will be linked"); }
#else
void RSDK::LinkGameLogic(GameInfo info) { PrintLog(PRINT_POPUP, "Internal LinkGameLogic() function called, no logic will be linked"); }
#endif
