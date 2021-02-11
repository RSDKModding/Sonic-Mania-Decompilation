#include "RetroEngine.hpp"

void *functionTable[FUNCTABLE_COUNT];
void *userDataTable[UDATATABLE_COUNT];

enum UserdataTableIDs {
    UserdataTable_Unknown1,
    UserdataTable_GetConfirmButtonFlip,
    UserdataTable_Unknown2,
    UserdataTable_Unknown3,
    UserdataTable_Unknown4,
    UserdataTable_CheckDLC,
    UserdataTable_Unknown5,
    UserdataTable_UnlockAchievement,
    UserdataTable_Missing1,
    UserdataTable_Unknown6,
    UserdataTable_Unknown7,
    UserdataTable_Unknown8,
    UserdataTable_Unknown9,
    UserdataTable_Unknown10,
    UserdataTable_Unknown11,
    UserdataTable_Missing2,
    UserdataTable_Unknown12,
    UserdataTable_Missing3,
    UserdataTable_Unknown13,
    UserdataTable_PossiblyGetStrings,
    UserdataTable_Unknown14,
    UserdataTable_Missing4,
    UserdataTable_Unknown15,
    UserdataTable_Unknown16,
    UserdataTable_Unknown17,
    UserdataTable_Unknown18,
    UserdataTable_Unknown19,
    UserdataTable_Unknown20,
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
    FunctionTable_Unknown53,
    FunctionTable_CheckValidScene,
    FunctionTable_CheckSceneFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectByName,
    FunctionTable_ClearScreens,
    FunctionTable_AddScreen,
    FunctionTable_GetSettingsValue,
    FunctionTable_SetSettingsValue,
    FunctionTable_ResizeWindow,
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
    FunctionTable_SetScreenFade,
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
    FunctionTable_DoPaletteSomething3,
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawQuad,
    FunctionTable_DrawTexturedQuad,
    FunctionTable_DrawSprite,
    FunctionTable_DrawUnknown,
    FunctionTable_DrawText,
    FunctionTable_DrawTile,
    FunctionTable_CopyTile,
    FunctionTable_DrawAniTile,
    FunctionTable_SetPaletteThing,
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
    FunctionTable_AnimationDoSomething,
    FunctionTable_SetSpriteAnimation,
    FunctionTable_DoSpriteAnimationSomething,
    FunctionTable_SetSpriteString,
    FunctionTable_DoAnimationListSomething,
    FunctionTable_GetFrame,
    FunctionTable_GetHitbox,
    FunctionTable_GetFrameID,
    FunctionTable_Unknown82,
    FunctionTable_ProcessAnimation,
    FunctionTable_GetSceneLayerID,
    FunctionTable_GetSceneLayerPtr,
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
    FunctionTable_LoadPNG,
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
    userDataTable[UserdataTable_Unknown1]             = NULL;
    userDataTable[UserdataTable_GetConfirmButtonFlip] = NULL;
    userDataTable[UserdataTable_Unknown2]             = NULL;
    userDataTable[UserdataTable_Unknown3]             = NULL;
    userDataTable[UserdataTable_Unknown4]             = NULL;
    userDataTable[UserdataTable_CheckDLC]             = NULL;
    userDataTable[UserdataTable_Unknown5]             = NULL;
    userDataTable[UserdataTable_UnlockAchievement]    = NULL;
    userDataTable[UserdataTable_Missing1]             = NULL;
    userDataTable[UserdataTable_Unknown6]             = NULL;
    userDataTable[UserdataTable_Unknown7]             = NULL;
    userDataTable[UserdataTable_Unknown8]             = NULL;
    userDataTable[UserdataTable_Unknown9]             = NULL;
    userDataTable[UserdataTable_Unknown10]            = NULL;
    userDataTable[UserdataTable_Unknown11]            = NULL;
    userDataTable[UserdataTable_Missing2]             = NULL;
    userDataTable[UserdataTable_Unknown12]            = NULL;
    userDataTable[UserdataTable_Missing3]             = NULL;
    userDataTable[UserdataTable_Unknown13]            = NULL;
    userDataTable[UserdataTable_PossiblyGetStrings]   = NULL;
    userDataTable[UserdataTable_Unknown14]            = NULL;
    userDataTable[UserdataTable_Missing4]             = NULL;
    userDataTable[UserdataTable_Unknown15]            = NULL;
    userDataTable[UserdataTable_Unknown16]            = NULL;
    userDataTable[UserdataTable_Unknown17]            = NULL;
    userDataTable[UserdataTable_Unknown18]            = NULL;
    userDataTable[UserdataTable_Unknown19]            = NULL;
    userDataTable[UserdataTable_Unknown20]            = NULL;
    userDataTable[UserdataTable_Unknown21]            = NULL;
    userDataTable[UserdataTable_Unknown22]            = NULL;
    userDataTable[UserdataTable_Unknown23]            = NULL;
    userDataTable[UserdataTable_Unknown24]            = NULL;
    userDataTable[UserdataTable_Missing5]             = NULL;
    userDataTable[UserdataTable_Unknown25]            = NULL;
    userDataTable[UserdataTable_Unknown26]            = NULL;
    userDataTable[UserdataTable_Unknown27]            = NULL;
    userDataTable[UserdataTable_Unknown28]            = NULL;
    userDataTable[UserdataTable_LoadUserFile]         = NULL;
    userDataTable[UserdataTable_SaveUserFile]         = NULL;
    userDataTable[UserdataTable_DeleteUserFile]       = NULL;
    userDataTable[UserdataTable_AddUserDBEntry]       = NULL;
    userDataTable[UserdataTable_OpenUserDB]           = NULL;
    userDataTable[UserdataTable_SaveUserDB]           = NULL;
    userDataTable[UserdataTable_Unknown30]            = NULL;
    userDataTable[UserdataTable_Missing6]             = NULL;
    userDataTable[UserdataTable_Unknown31]            = NULL;
    userDataTable[UserdataTable_Unknown32]            = NULL;
    userDataTable[UserdataTable_Unknown33]            = NULL;
    userDataTable[UserdataTable_Unknown34]            = NULL;
    userDataTable[UserdataTable_Unknown35]            = NULL;
    userDataTable[UserdataTable_Unknown36]            = NULL;
    userDataTable[UserdataTable_Unknown37]            = NULL;
    userDataTable[UserdataTable_Unknown38]            = NULL;
    userDataTable[UserdataTable_Unknown39]            = NULL;
    userDataTable[UserdataTable_AddUserDB]            = NULL;
    userDataTable[UserdataTable_Unknown40]            = NULL;
    userDataTable[UserdataTable_Unknown41]            = NULL;
    userDataTable[UserdataTable_Unknown42]            = NULL;
    userDataTable[UserdataTable_Unknown43]            = NULL;

    //Function Table
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
    functionTable[FunctionTable_CheckOnScreen]                = NULL; // CheckOnScreen;
    functionTable[FunctionTable_CheckPosOnScreen]             = NULL; // CheckPosOnScreen;
    functionTable[FunctionTable_AddDrawListRef]               = NULL; // AddDrawListRef;
    functionTable[FunctionTable_SwapDrawLayers]               = NULL; // SwapDrawLayers;
    functionTable[FunctionTable_SetDrawLayerProperties]       = SetDrawLayerProperties;
    functionTable[FunctionTable_LoadScene]                    = NULL; // LoadScene;
    functionTable[FunctionTable_SetGameMode]                  = SetEngineState;
    functionTable[FunctionTable_Unknown53]                    = NULL; // Unknown53;
    functionTable[FunctionTable_CheckValidScene]              = CheckValidStage;
    functionTable[FunctionTable_CheckSceneFolder]             = CheckSceneFolder;
    functionTable[FunctionTable_InitSceneLoad]                = InitSceneLoad;
    functionTable[FunctionTable_GetObjectByName]              = GetObjectByName;
    functionTable[FunctionTable_ClearScreens]                 = ClearScreens;
    functionTable[FunctionTable_AddScreen]                    = AddScreen;
    functionTable[FunctionTable_GetSettingsValue]             = NULL; // GetSettingsValue;
    functionTable[FunctionTable_SetSettingsValue]             = NULL; // SetSettingsValue;
    functionTable[FunctionTable_ResizeWindow]                 = NULL; // ResizeWindow;
    functionTable[FunctionTable_Sin1024]                      = sin1024;
    functionTable[FunctionTable_Cos1024]                      = cos1024;
    functionTable[FunctionTable_ATan1024]                     = aTan1024;
    functionTable[FunctionTable_ASin1024]                     = aSin1024;
    functionTable[FunctionTable_ACos1024]                     = aCos1024;
    functionTable[FunctionTable_Sin512]                       = sin512;
    functionTable[FunctionTable_Cos512]                       = cos512;
    functionTable[FunctionTable_ATan512]                      = aTan512;
    functionTable[FunctionTable_ASin512]                      = aSin512;
    functionTable[FunctionTable_ACos512]                      = aCos512;
    functionTable[FunctionTable_Sin256]                       = sin256;
    functionTable[FunctionTable_Cos256]                       = cos256;
    functionTable[FunctionTable_ATan256]                      = aTan256;
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
    functionTable[FunctionTable_SetText]                      = SetText; // SetText;
    functionTable[FunctionTable_Unknown64]                    = NULL; // Unknown64;
    functionTable[FunctionTable_Unknown65]                    = NULL; // Unknown65;
    functionTable[FunctionTable_Unknown66]                    = NULL; // Unknown66;
    functionTable[FunctionTable_Unknown67]                    = NULL; // Unknown67;
    functionTable[FunctionTable_LoadStrings]                  = NULL; // LoadStrings;
    functionTable[FunctionTable_Unknown68]                    = NULL; // Unknown68;
    functionTable[FunctionTable_CopyString]                   = NULL; // CopyString;
    functionTable[FunctionTable_Unknown69]                    = NULL; // Unknown69;
    functionTable[FunctionTable_Unknown70]                    = NULL; // Unknown70;
    functionTable[FunctionTable_Unknown71]                    = NULL; // Unknown71;
    functionTable[FunctionTable_SetScreenSize]                = SetScreenSize;
    functionTable[FunctionTable_SetClipBounds]                = SetClipBounds;
    functionTable[FunctionTable_SetScreenFade]                = NULL; // SetScreenFade;
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
    functionTable[FunctionTable_DoPaletteSomething3]          = NULL; // DoPaletteSomething3;
    functionTable[FunctionTable_DrawRect]                     = DrawRectangle;
    functionTable[FunctionTable_DrawLine]                     = DrawLine;
    functionTable[FunctionTable_DrawCircle]                   = DrawCircle;
    functionTable[FunctionTable_DrawCircleOutline]            = DrawCircleOutline;
    functionTable[FunctionTable_DrawQuad]                     = DrawQuad;
    functionTable[FunctionTable_DrawTexturedQuad]             = DrawTexturedQuad;
    functionTable[FunctionTable_DrawSprite]                   = DrawSprite;
    functionTable[FunctionTable_DrawUnknown]                  = // DrawUnknown;
    functionTable[FunctionTable_DrawText]                     = DrawText;
    functionTable[FunctionTable_DrawTile]                     = DrawTile;
    functionTable[FunctionTable_CopyTile]                     = NULL; // CopyTile;
    functionTable[FunctionTable_DrawAniTile]                  = DrawAniTile;
    functionTable[FunctionTable_SetPaletteThing]              = NULL; // SetPaletteThing;
    functionTable[FunctionTable_LoadMesh]                     = NULL; // LoadMesh;
    functionTable[FunctionTable_Create3DScene]                = NULL; // Create3DScene;
    functionTable[FunctionTable_Init3DScene]                  = NULL; // Init3DScene;
    functionTable[FunctionTable_View_Something1]              = NULL; // View_Something1;
    functionTable[FunctionTable_View_Something2]              = NULL; // View_Something2;
    functionTable[FunctionTable_View_Something3]              = NULL; // View_Something3;
    functionTable[FunctionTable_SetupMesh]                    = NULL; // SetupMesh;
    functionTable[FunctionTable_SetModelAnimation]            = SetModelAnimation;
    functionTable[FunctionTable_SetupMeshAnimation]           = NULL; // SetupMeshAnimation;
    functionTable[FunctionTable_Draw3DScene]                  = NULL; // Draw3DScene;
    functionTable[FunctionTable_LoadAnimation]                = LoadAnimation;
    functionTable[FunctionTable_AnimationDoSomething]         = NULL; // AnimationDoSomething;
    functionTable[FunctionTable_SetSpriteAnimation]           = SetSpriteAnimation;
    functionTable[FunctionTable_DoSpriteAnimationSomething]   = NULL; // DoSpriteAnimationSomething;
    functionTable[FunctionTable_SetSpriteString]              = NULL; // SetSpriteString;
    functionTable[FunctionTable_DoAnimationListSomething]     = NULL; // DoAnimationListSomething;
    functionTable[FunctionTable_GetFrame]                     = GetFrame;
    functionTable[FunctionTable_GetHitbox]                    = GetHitbox;
    functionTable[FunctionTable_GetFrameID]                   = GetFrameID;
    functionTable[FunctionTable_Unknown82]                    = NULL; // Unknown82;
    functionTable[FunctionTable_ProcessAnimation]             = ProcessAnimation;
    functionTable[FunctionTable_GetSceneLayerPtr]             = NULL; // GetSceneLayerPtr;
    functionTable[FunctionTable_GetSceneLayerID]              = NULL; // GetSceneLayerID;
    functionTable[FunctionTable_GetLayerSize]                 = NULL; // GetLayerSize;
    functionTable[FunctionTable_GetTileInfo]                  = NULL; // GetTileInfo;
    functionTable[FunctionTable_SetTileInfo]                  = NULL; // SetTileInfo;
    functionTable[FunctionTable_CopyTileLayer]                = NULL; // CopyTileLayout;
    functionTable[FunctionTable_ProcessParallax]              = NULL; // ProcessParallaxScroll;
    functionTable[FunctionTable_GetLinePosPtrs]               = NULL; // GetLinePositions;
    functionTable[FunctionTable_CheckObjectCollisionTouch]    = NULL; // CheckObjectCollisionTouch;
    functionTable[FunctionTable_CheckObjectCollisionPoint]    = NULL; // CheckObjectCollisionPoint;
    functionTable[FunctionTable_CheckObjectCollisionBox]      = NULL; // CheckObjectCollisionBox;
    functionTable[FunctionTable_CheckObjectCollisionPlatform] = NULL; // CheckObjectCollisionPlatform;
    functionTable[FunctionTable_ObjectTileCollision]          = NULL; // ObjectTileCollision;
    functionTable[FunctionTable_ObjectTileGrip]               = NULL; // ObjectTileGrip;
    functionTable[FunctionTable_ProcessPlayerTileCollisions]  = NULL; // ProcessPlayerTileCollisions;
    functionTable[FunctionTable_GetTileAngle]                 = NULL; // GetTileAngle;
    functionTable[FunctionTable_SetTileAngle]                 = NULL; // SetTileAngle;
    functionTable[FunctionTable_GetTileBehaviour]             = NULL; // GetTileBehaviour;
    functionTable[FunctionTable_SetTileBehaviour]             = NULL; // SetTileBehaviour;
    functionTable[FunctionTable_GetSFX]                       = NULL; // GetSoundFX;
    functionTable[FunctionTable_PlaySFX]                      = NULL; // PlaySFX;
    functionTable[FunctionTable_StopSFX]                      = NULL; // StopSFX;
    functionTable[FunctionTable_PlayMusic]                    = NULL; // PlayMusicFile;
    functionTable[FunctionTable_SetSoundAttributes]           = NULL; // SetSoundAttributes;
    functionTable[FunctionTable_Unknown91]                    = NULL; // Unknown91;
    functionTable[FunctionTable_Unknown92]                    = NULL; // Unknown92;
    functionTable[FunctionTable_Unknown93]                    = NULL; // Unknown93;
    functionTable[FunctionTable_Unknown94]                    = NULL; // Unknown94;
    functionTable[FunctionTable_Unknown95]                    = NULL; // Unknown95;
    functionTable[FunctionTable_Unknown96]                    = NULL; // Unknown96;
    functionTable[FunctionTable_LoadVideo]                    = NULL; // LoadVideo;
    functionTable[FunctionTable_LoadPNG]                      = NULL; // LoadPNG;
    functionTable[FunctionTable_Unknown98]                    = NULL; // Unknown98;
    functionTable[FunctionTable_Unknown99]                    = NULL; // Unknown99;
    functionTable[FunctionTable_Unknown100]                   = NULL; // Unknown100;
    functionTable[FunctionTable_Unknown101]                   = NULL; // Unknown101;
    functionTable[FunctionTable_Unknown102]                   = NULL; // Unknown102;
    functionTable[FunctionTable_Unknown103]                   = NULL; // Unknown103;
    functionTable[FunctionTable_Unknown104]                   = NULL; // Unknown104;
    functionTable[FunctionTable_Missing21]                    = NULL; // UserDataUnknown1;
    functionTable[FunctionTable_Missing22]                    = NULL; // UserDataUnknown2;
    functionTable[FunctionTable_Missing23]                    = NULL; // UserDataUnknown3;
    functionTable[FunctionTable_Missing24]                    = NULL; // UserDataUnknown4;
    functionTable[FunctionTable_Missing25]                    = NULL; // UserDataUnknown5;
    functionTable[FunctionTable_Missing26]                    = NULL; // UserDataUnknown6;
    functionTable[FunctionTable_LoadUserFile]                 = NULL; // LoadUserFile;
    functionTable[FunctionTable_SaveUserFile]                 = NULL; // SaveUserFile;
    functionTable[FunctionTable_printLog]                     = printLog;
    functionTable[FunctionTable_printString]                  = printString;
    functionTable[FunctionTable_printText]                    = printText;
    functionTable[FunctionTable_printIntegerUnsigned]         = printIntegerUnsigned;
    functionTable[FunctionTable_printInteger]                 = printInteger;
    functionTable[FunctionTable_printFloat]                   = printFloat;
    functionTable[FunctionTable_printVector2]                 = printVector2;
    functionTable[FunctionTable_printHitbox]                  = printHitbox;
    functionTable[FunctionTable_Unknown105]                   = NULL; // UserDataUnknown7;
    functionTable[FunctionTable_Unknown106]                   = NULL; // UserDataUnknown8;
    functionTable[FunctionTable_ClearDebugValues]             = ClearDebugValues;
    functionTable[FunctionTable_SetDebugValue]                = SetDebugValue;
}


void LinkGameLogic(GameInfo *info) { printLog(SEVERITY_WARN, "Internal LinkGameLogic() function called, no logic will be linked"); }