#include "RetroEngine.hpp"

void *functionTable[FUNCTABLE_COUNT];

enum UserdataTableIDs {
    UserdataTable_Unknown1,
    UserdataTable_GetConfirmButtonFlip,
    UserdataTable_Unknown2,
    UserdataTable_Unknown3,
    UserdataTable_Unknown4,
    UserdataTable_CheckDLC,
    UserdataTable_Unknown5,
    UserdataTable_UnlockAchievement,
    UserdataTable_Unknown6,
    UserdataTable_Unknown7,
    UserdataTable_Unknown8,
    UserdataTable_Unknown9,
    UserdataTable_Unknown10,
    UserdataTable_Unknown11,
    UserdataTable_Unknown12,
    UserdataTable_Unknown13,
    UserdataTable_PossiblyGetStrings,
    UserdataTable_Unknown14,
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
    FunctionTable_Unknown45,
    FunctionTable_SetEditableVar,
    FunctionTable_GetObjectByID,
    FunctionTable_GetEntityID,
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
    FunctionTable_CheckStageFolder,
    FunctionTable_InitSceneLoad,
    FunctionTable_GetObjectIDByName,
    FunctionTable_Unknown57,
    FunctionTable_Unknown58,
    FunctionTable_GetSettingsValue,
    FunctionTable_SetSettingsValue,
    FunctionTable_ResizeWindow,
    FunctionTable_Sin1024,
    FunctionTable_Cos1024,
    FunctionTable_Sin512,
    FunctionTable_Cos512,
    FunctionTable_Sin256,
    FunctionTable_Cos256,
    FunctionTable_Rand,
    FunctionTable_Random,
    FunctionTable_SetRandKey,
    FunctionTable_ATan2,
    FunctionTable_MatrixMultiply,
    FunctionTable_MatrixTranslateXYZ,
    FunctionTable_MatrixScaleXYZ,
    FunctionTable_MatrixRotateX,
    FunctionTable_MatrixRotateY,
    FunctionTable_MatrixRotateZ,
    FunctionTable_MatrixRotateXYZ,
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
    FunctionTable_SetClipBounds,
    FunctionTable_SetScreenFade,
    FunctionTable_LoadSpriteSheet,
    FunctionTable_SetInkValue,
    FunctionTable_SetPaletteMask,
    FunctionTable_SetPaletteEntry,
    uFunctionTable_GetPaletteEntry,
    FunctionTable_SetActivePalette,
    FunctionTable_CopyPalette,
    FunctionTable_LoadPalette,
    FunctionTable_RotatePalette,
    FunctionTable_SetLimitedFade,
    FunctionTable_DrawRect,
    FunctionTable_DrawLine,
    FunctionTable_DrawCircle,
    FunctionTable_DrawCircleOutline,
    FunctionTable_DrawQuad,
    FunctionTable_DrawTexturedQuad,
    FunctionTable_DrawSprite,
    FunctionTable_DrawUnknown,
    FunctionTable_DrawText,
    FunctionTable_DrawSprite3,
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
    FunctionTable_SetSpriteAnimation,
    FunctionTable_SetSpriteString,
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
    FunctionTable_GetTileBehaviour,
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
    FunctionTable_printLog,
    FunctionTable_printString,
    FunctionTable_printText,
    FunctionTable_printInteger,
    FunctionTable_printVector2,
    FunctionTable_Unknown105,
    FunctionTable_Unknown106,
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

    functionTable[FunctionTable_CreateObject]          = CreateObject;
    functionTable[FunctionTable_CreateObjectContainer] = CreateObjectContainer;
    // functionTable[FunctionTable_GetActiveObjects]   = GetActiveObjects;
    // functionTable[FunctionTable_GetObjects]   = GetObjects;
    // functionTable[FunctionTable_Unknown45]   = Unknown45;
    functionTable[FunctionTable_SetEditableVar] = SetEditableVar;
    functionTable[FunctionTable_GetObjectByID]  = GetObjectByID;
    functionTable[FunctionTable_GetEntityID]    = GetEntityID;
    // functionTable[FunctionTable_GetEntityNoMaybe] = GetEntityNoMaybe;
    functionTable[FunctionTable_GetDrawListRef]    = GetDrawListRef;
    functionTable[FunctionTable_GetDrawListRefPtr] = GetDrawListRefPtr;
    functionTable[FunctionTable_DestroyEntity]     = DestroyEntity;
    functionTable[FunctionTable_ResetEntity]       = ResetEntity;
    functionTable[FunctionTable_SpawnEntity]       = SpawnEntity;
    functionTable[FunctionTable_CopyEntity]        = CopyEntity;
    // functionTable[FunctionTable_CheckOnScreen] = CheckOnScreen;
    // functionTable[FunctionTable_CheckPosOnScreen] = CheckPosOnScreen;
    // functionTable[FunctionTable_AddDrawListRef] = AddDrawListRef;
    // functionTable[FunctionTable_SwapDrawLayers] = SwapDrawLayers;
    functionTable[FunctionTable_SetDrawLayerProperties] = SetDrawLayerProperties;
    // functionTable[FunctionTable_LoadGameScene] = LoadGameScene;
    functionTable[FunctionTable_SetGameMode] = SetEngineState;
    // functionTable[FunctionTable_Unknown53] = Unknown53;
    // functionTable[FunctionTable_CheckValidStage] = CheckValidStage;
    // functionTable[FunctionTable_CheckSceneFolder] = CheckSceneFolder;
    // functionTable[FunctionTable_InitSceneLoad] = InitSceneLoad;
    // functionTable[FunctionTable_GetObjectByName] = GetObjectByName;
    // functionTable[FunctionTable_ResetScreenCount] = ResetScreenCount;
    // functionTable[FunctionTable_AddScreen] = AddScreen;
    // functionTable[FunctionTable_GetSettingsValue] = GetSettingsValue;
    // functionTable[FunctionTable_SetSettingsValue] = SetSettingsValue;
    // functionTable[FunctionTable_ResizeWindow] = ResizeWindow;
    functionTable[FunctionTable_Sin1024] = sin1024;
    functionTable[FunctionTable_Cos1024] = cos1024;
    // functionTable[FunctionTable_ATan1024] = aTan1024;
    // functionTable[FunctionTable_ASin1024] = aSin1024;
    // functionTable[FunctionTable_ACos1024] = aCos1024;
    functionTable[FunctionTable_Sin512] = sin512;
    functionTable[FunctionTable_Cos512] = cos512;
    // functionTable[FunctionTable_ATan512] = aTan512;
    // functionTable[FunctionTable_ASin512] = aSin512;
    // functionTable[FunctionTable_ACos512] = aCos512;
    functionTable[FunctionTable_Sin256] = sin256;
    functionTable[FunctionTable_Cos256] = cos256;
    // functionTable[FunctionTable_ATan256]  = aTan256;
    // functionTable[FunctionTable_ASin256]  = aSin256;
    // functionTable[FunctionTable_ACos256]  = aCos256;
    functionTable[FunctionTable_Rand]       = random;
    functionTable[FunctionTable_Random]     = random2;
    functionTable[FunctionTable_SetRandKey] = setRandKey;
    functionTable[FunctionTable_ATan2]      = ArcTanLookup;
    // functionTable[FunctionTable_SetIdentityMatrix]   = SetIdentityMatrix;
    // functionTable[FunctionTable_3DSomething2]   = MatrixMultiply;
    // functionTable[FunctionTable_MatrixSomething]   = MatrixTranslateXYZ;
    // functionTable[FunctionTable_MatrixScaleXYZ]   = MatrixScaleXYZ;
    // functionTable[FunctionTable_MatrixRotateX]   = MatrixRotateX;
    // functionTable[FunctionTable_MatrixRotateY]   = MatrixRotateY;
    // functionTable[FunctionTable_MatrixRotateZ]   = MatrixRotateZ;
    // functionTable[FunctionTable_MatrixRotateXYZ]   = MatrixRotateXYZ;
    // functionTable[FunctionTable_MatrixInvert]   = MatrixInvert;
    // functionTable[FunctionTable_MatrixMultiply]   = MatrixCopy;
    // functionTable[FunctionTable_FuncUnknown1]   = FuncUnknown1;
    // functionTable[FunctionTable_FuncUnknown2]   = FuncUnknown2;
    // functionTable[FunctionTable_FuncUnknown3]   = FuncUnknown3;
    // functionTable[FunctionTable_FuncUnknown4]   = FuncUnknown4;
    // functionTable[FunctionTable_FuncUnknown5]   = FuncUnknown5;
    // functionTable[FunctionTable_LoadStrings]   = LoadStrings;
    // functionTable[FunctionTable_Unknown68]   = Unknown68;
    // functionTable[FunctionTable_SetMusicTrack]   = SetMusicTrack;
    // functionTable[FunctionTable_Unknown69]   = Unknown69;
    // functionTable[FunctionTable_Unknown70]   = Unknown70;
    // functionTable[FunctionTable_Unknown71]   = Unknown71;
    // functionTable[FunctionTable_SetScreenSize]   = SetScreenSize;
    functionTable[FunctionTable_SetClipBounds] = SetClipBounds;
    // functionTable[FunctionTable_SetScreenFade            ]    = SetScreenFade;
    functionTable[FunctionTable_LoadSpriteSheet]  = LoadSpriteSheet;
    functionTable[FunctionTable_SetInkValue]      = SetInkValue;
    functionTable[FunctionTable_SetPaletteMask]   = SetPaletteMask;
    functionTable[FunctionTable_SetPaletteEntry]  = SetPaletteEntry;
    functionTable[uFunctionTable_GetPaletteEntry] = GetPaletteEntry;
    functionTable[FunctionTable_SetActivePalette] = SetActivePalette;
    functionTable[FunctionTable_CopyPalette]      = CopyPalette;
    functionTable[FunctionTable_LoadPalette]      = LoadPalette;
    functionTable[FunctionTable_RotatePalette]    = RotatePalette;
    functionTable[FunctionTable_SetLimitedFade]   = SetPaletteFade;
    // functionTable[FunctionTable_DoPaletteSomething3]  = DoPaletteSomething3;
    // functionTable[FunctionTable_DrawRect]  = DrawRect;
    // functionTable[FunctionTable_DrawLine]  = DrawLine;
    // functionTable[FunctionTable_DrawCircle]  = DrawCircle;
    // functionTable[FunctionTable_DrawCircleOutline]  = DrawCircleOutline;
    // functionTable[FunctionTable_DrawQuad]  = DrawQuad;
    // functionTable[FunctionTable_DrawTexturedQuad]  = DrawTexturedQuad;
    // functionTable[FunctionTable_DrawSprite]  = DrawSprite;
    // functionTable[FunctionTable_DrawUnknown]  = DrawUnknown;
    // functionTable[FunctionTable_DrawText]  = DrawText;
    // functionTable[FunctionTable_DrawSprite3]  = DrawSprite3;
    // functionTable[FunctionTable_DrawTile]  = DrawTile;
    // functionTable[FunctionTable_DrawAniTiles]  = DrawAniTiles;
    // functionTable[FunctionTable_SetPaletteThing]  = SetPaletteThing;
    // functionTable[FunctionTable_LoadMesh]  = LoadMesh;
    // functionTable[FunctionTable_Create3DScene]  = Create3DScene;
    // functionTable[FunctionTable_3DSomething3]  = Init3DScene;
    // functionTable[FunctionTable_View_Something1]  = View_Something1;
    // functionTable[FunctionTable_View_Something2]  = View_Something2;
    // functionTable[FunctionTable_View_Something3]  = View_Something3;
    // functionTable[FunctionTable_SetupMesh]  = SetupMesh;
    functionTable[FunctionTable_SetModelAnimation] = SetModelAnimation;
    // functionTable[FunctionTable_DrawMeshSomething]       = SetupMeshAnimation;
    // functionTable[FunctionTable_Draw3DScene]       = Draw3DScene;
    functionTable[FunctionTable_LoadAnimation] = LoadAnimation;
    // functionTable[FunctionTable_AnimationDoSomething] = AnimationDoSomething;
    functionTable[FunctionTable_SetSpriteAnimation] = SetSpriteAnimation;
    // functionTable[FunctionTable_DoSpriteAnimationSomething]  = DoSpriteAnimationSomething;
    // functionTable[FunctionTable_DoAnimationListSomething]  = DoAnimationListSomething;
    // functionTable[FunctionTable_SetSpriteString]  = SetSpriteString;
    functionTable[FunctionTable_GetFrame]   = GetFrame;
    functionTable[FunctionTable_GetHitbox]  = GetHitbox;
    functionTable[FunctionTable_GetFrameID] = GetFrameID;
    // functionTable[FunctionTable_Unknown82]        = Unknown82;
    functionTable[FunctionTable_ProcessAnimation] = ProcessAnimation;
    // functionTable[FunctionTable_GetSceneLayerInfo] = GetSceneLayerInfo;
    // functionTable[FunctionTable_GetSceneLayerID] = GetSceneLayerID;
    // functionTable[FunctionTable_GetLayerSize] = GetLayerSize;
    // functionTable[FunctionTable_GetTileInfo] = GetTileInfo;
    // functionTable[FunctionTable_SetTileInfo] = SetTileInfo;
    // functionTable[FunctionTable_CopyTileLayout] = CopyTileLayout;
    // functionTable[FunctionTable_ProcessParallaxScroll] = ProcessParallaxScroll;
    // functionTable[FunctionTable_GetLinePositions] = GetLinePositions;
    // functionTable[FunctionTable_CheckObjectCollisionTouch] = CheckObjectCollisionTouch;
    // functionTable[FunctionTable_CheckObjectCollisionPoint] = CheckObjectCollisionPoint;
    // functionTable[FunctionTable_CheckObjectCollisionBox] = CheckObjectCollisionBox;
    // functionTable[FunctionTable_CheckObjectCollisionPlatform] = CheckObjectCollisionPlatform;
    // functionTable[FunctionTable_ObjectCollision] = ObjectTileCollision;
    // functionTable[FunctionTable_ObjectPathGrip] = ObjectTileGrip;
    // functionTable[FunctionTable_ProcessPlayerCollisions] = ProcessPlayerTileCollisions;
    // functionTable[FunctionTable_GetTileAngle] = GetTileAngle;
    // functionTable[FunctionTable_SetTileAngle] = SetTileAngle;
    // functionTable[FunctionTable_GetTileBehaviour] = GetTileBehaviour;
    // functionTable[FunctionTable_SetTileBehaviour] = SetTileBehaviour;
    // functionTable[FunctionTable_GetSoundFX] = GetSoundFX;
    // functionTable[FunctionTable_PlaySFX] = PlaySFX;
    // functionTable[FunctionTable_StopSFX] = StopSFX;
    // functionTable[FunctionTable_PlayMusicFile] = PlayMusicFile;
    // functionTable[FunctionTable_SetSoundAttributes] = SetSoundAttributes;
    // functionTable[FunctionTable_Unknown91] = Unknown91;
    // functionTable[FunctionTable_Unknown92] = Unknown92;
    // functionTable[FunctionTable_Unknown93] = Unknown93;
    // functionTable[FunctionTable_Unknown94] = Unknown94;
    // functionTable[FunctionTable_Unknown95] = Unknown95;
    // functionTable[FunctionTable_Unknown96] = Unknown96;
    // functionTable[FunctionTable_LoadVideo] = LoadVideo;
    // functionTable[FunctionTable_LoadPNG] = LoadPNG;
    // functionTable[FunctionTable_Unknown98] = Unknown98;
    // functionTable[FunctionTable_Unknown99] = Unknown99;
    // functionTable[FunctionTable_Unknown100] = Unknown100;
    // functionTable[FunctionTable_Unknown101] = Unknown101;
    // functionTable[FunctionTable_Unknown102] = Unknown102;
    // functionTable[FunctionTable_Unknown103] = Unknown103;
    // functionTable[FunctionTable_Unknown104] = Unknown104;

    // Userdata
    // functionTable[FunctionTable_UserDataUnknown1]         = UserDataUnknown1;
    // functionTable[FunctionTable_UserDataUnknown2]         = UserDataUnknown2;
    // functionTable[FunctionTable_UserDataUnknown3]         = UserDataUnknown3;
    // functionTable[FunctionTable_UserDataUnknown4]         = UserDataUnknown4;
    // functionTable[FunctionTable_UserDataUnknown5]         = UserDataUnknown5;
    // functionTable[FunctionTable_UserDataUnknown6]         = UserDataUnknown6;
    // functionTable[FunctionTable_LoadUserFile]         = LoadUserFile;
    // functionTable[FunctionTable_SaveUserFile]         = SaveUserFile;
    functionTable[FunctionTable_printLog]    = printLog;
    functionTable[FunctionTable_printString] = printString;
    functionTable[FunctionTable_printText]   = printText;
    // functionTable[FunctionTable_printIntegerUnsigned ] = printIntegerUnsigned;
    functionTable[FunctionTable_printInteger] = printInteger;
    // functionTable[FunctionTable_printFloat]         = printFloat;
    functionTable[FunctionTable_printVector2] = printVector2;
    // functionTable[FunctionTable_printHitbox]      = printHitbox;
    // functionTable[FunctionTable_UserDataUnknown7]           = UserDataUnknown7;
    // functionTable[FunctionTable_UserDataUnknown8]           = UserDataUnknown8;
    // functionTable[FunctionTable_ResetDebugValues]            = ResetDebugValues;
    functionTable[FunctionTable_SetDebugValue] = SetDebugValue;
}