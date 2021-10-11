#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#define minVal(a, b)                      ((a) < (b) ? (a) : (b))
#define maxVal(a, b)                      ((a) > (b) ? (a) : (b))
#define clampVal(value, minimum, maximum) (((value) < (minimum)) ? (minimum) : (((value) > (maximum)) ? (maximum) : (value)))
#define fabs(a)                           ((a) > 0 ? (a) : -(a))

#define setBit(value, set, pos) ((value) ^= (-(int)(set) ^ (value)) & (1 << (pos)))
#define getBit(b, pos)          ((b) >> (pos) & 1)

#define intToVoid(x)   (void *)(size_t)(x)
#define floatToVoid(x) intToVoid(*(int *)&(x))
#define voidToInt(x)   (int)(size_t)(x)
#define voidToFloat(x) *(float *)&(x)

#define unused(x) (void)x

typedef uint colour;
typedef uint color;

#define RETRO_USE_MOD_LOADER (1)

#if RETRO_USE_MOD_LOADER
typedef enum {
    MODCB_GAME_STARTUP,
    MODCB_STAGELOAD,
    MODCB_ONUPDATE,
    MODCB_ONLATEUPDATE,
    MODCB_ONSTATICUPDATE,
    MODCB_ONDRAW,
    MODCB_STAGEUNLOAD,
} ModCallbackEvents;

typedef enum {
    SUPER_UPDATE,
    SUPER_LATEUPDATE,
    SUPER_STATICUPDATE,
    SUPER_DRAW,
    SUPER_CREATE,
    SUPER_STAGELOAD,
    SUPER_EDITORDRAW,
    SUPER_EDITORLOAD,
    SUPER_SERIALIZE
} ModSuper;

// Mod Table
typedef struct {
    void (*RegisterGlobals)(const char *globalsPath, void **globals, uint size);
    void *RegisterObject_FuncP;
    void *RegisterObject_STD;
    void *(*GetGlobals)();
    void (*Super)(int objectID, ModSuper callback, void *data);

    bool32 (*LoadModInfo)(const char *folder, TextInfo *name, TextInfo *description, TextInfo *version, bool32 *active);
    void *AddModCallback_FuncP;
    void *AddModCallback_STD;
    void *(*AddPublicFunction)(const char *folder, const char *functionName, void *functionPtr);
    void *(*GetPublicFunction)(const char *folder, const char *functionName);
    const char *(*GetModPath)(const char *id);

    bool32 (*GetSettingsBool)(const char *id, const char *key, bool32 fallback);
    int (*GetSettingsInteger)(const char *id, const char *key, int fallback);
    void (*GetSettingsString)(const char *id, const char *key, TextInfo *result, const char *fallback);

    void (*SetSettingsBool)(const char *id, const char *key, bool32 val);
    void (*SetSettingsInteger)(const char *id, const char *key, int val);
    void (*SetSettingsString)(const char *id, const char *key, TextInfo *val);

    void (*SaveSettings)(const char *id);

    bool32 (*GetConfigBool)(const char *id, const char *key, bool32 fallback);
    int (*GetConfigInteger)(const char *id, const char *key, int fallback);
    void (*GetConfigString)(const char *id, const char *key, TextInfo *result, const char *fallback);
    bool32 (*ForeachConfig)(const char *id, TextInfo *textInfo);
    bool32 (*ForeachConfigCategory)(const char *id, TextInfo *textInfo);

    Object *(*GetObject)(const char *name);
} ModFunctionTable;
#endif

#if RETRO_USE_PLUS
// Userdata Table
typedef struct {
    int (*GetUserLanguage)(void);
    bool32 (*GetConfirmButtonFlip)(void);
    void (*ExitGame)(void);
    void (*LaunchManual)(void);
    bool32 (*Unknown4)(byte inputID);
    bool32 (*CheckDLC)(GameDLC dlc);
    void (*ShowExtensionOverlay)(byte overlay);
#if RETRO_GAMEVER == VER_107
    void (*EGS_Checkout)(int a1);
    void (*ShowEncorePage)(int a1);
    void (*EGS_Unknown4)(int a1);
    void (*RegisterHIDDevice)(void);
#endif
    void (*UnlockAchievement)(const char *achName);
    void (*GetAchievementStatus)(void);
    void (*SetAchievementStatus)(int a1);
#if RETRO_GAMEVER == VER_107
    bool32 (*CheckAchievementsEnabled)(void);
    void (*GetAchievementNames)(TextInfo *names, int count);
#endif
    void (*LeaderboardsUnknown4)(void);
#if RETRO_GAMEVER == VER_107
    void (*EGS_LeaderboardUnknown1)(void);
#endif
    void (*FetchLeaderboard)(const char* name, int a2);
    void (*TrackScore)(const char *name, int score, void (*callback)(int status, int rank));
    int (*GetLeaderboardsStatus)(void);
    Vector2 (*LeaderboardEntryCount)(void);
    int (*GetLeaderboardUnknown2)(void);
    void (*Unknown12)(int a2, uint a3, int a4);
    void (*LeaderboardsUnknown8)(void);
    LeaderboardEntry *(*ReadLeaderboardEntry)(uint a1);
    void (*SetRichPresence)(int, TextInfo *text);
    void (*TryTrackStat)(StatInfo *stat);
    int (*GetStatsStatus)(void);
    void (*SetStatsStatus)(int a1);
    void (*ClearPrerollErrors)(void);
    void (*TryAuth)(void);
    int (*GetUserAuthStatus)(void);
    bool32 (*GetUsername)(TextInfo *text);
    void (*TryInitStorage)(void);
    int (*UserStorageStatusUnknown1)(void);
    int (*GetSaveStatus)(void);
    void (*ClearUserStorageStatus)(void);
    void (*SetUserStorageStatus)(void);
    void (*SetSaveStatusOK)(void);
    void (*SetSaveStatusForbidden)(void);
    void (*SetSaveStatusError)(void);
    void (*SetUserStorageNoSave)(bool32 state);
    bool32 (*GetUserStorageNoSave)(void);
    void (*LoadUserFile)(const char *name, int *data, int size, void (*callback)(int status));                  // load user file from game dir
    void (*SaveUserFile)(const char *name, int *data, int size, void (*callback)(int status), bool32 compress); // save user file to game dir
    void (*DeleteUserFile)(const char *filename, void (*callback)(int status));                                 // delete user file from game dir
    ushort (*InitUserDB)(const char *name, ...);
    ushort (*LoadUserDB)(const char *filename, void (*callback)(int status));
    void (*SaveUserDB)(ushort tableID, void (*callback)(int status));
    void (*ClearUserDB)(ushort tableID);
    void (*ClearAllUserDBs)(void);
    void (*SetupSortedUserDBRowIDs)(ushort tableID);
    bool32 (*GetUserDBRowsChanged)(ushort tableID);
    void (*Unknown33)(ushort tableID, int type, const char *name, void *value);
    void (*SortDBRows)(ushort tableID, int type, const char *name, bool32 flag);
    int (*GetSortedUserDBRowCount)(ushort tableID);
    int (*GetSortedUserDBRowID)(ushort tableID, ushort row);
    int (*AddUserDBRow)(ushort tableID);
    void (*SetUserDBValue)(ushort tableID, int row, int type, const char *name, void *value);
    void (*GetUserDBValue)(ushort tableID, int row, int type, const char *name, void *value);
    uint (*GetUserDBRowUUID)(ushort tableID, ushort row);
    int (*GetUserDBByID)(ushort tableID, uint uuid);
    void (*GetUserDBCreationTime)(ushort tableID, ushort row, char *buffer, uint sizeInBytes, const char *format);
    void (*RemoveDBRow)(ushort tableID, ushort row);
    void (*RemoveAllDBRows)(ushort tableID);
    // count: 59
} APIFunctionTable;
#endif

// Function Table
typedef struct {
    void (*InitGlobalVariables)(void **globals, int size);
    void (*RegisterObject)(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                           void (*staticUpdate)(void), void (*draw)(void), void (*create)(void *), void (*stageLoad)(void), void (*editorDraw)(void),
                           void (*editorLoad)(void), void (*serialize)(void));
#if RETRO_USE_PLUS
    void (*RegisterObjectContainer)(void **structPtr, const char *name, uint objectSize);
#endif
    bool32 (*GetActiveEntities)(ushort group, Entity **entity);
    bool32 (*GetEntities)(ushort type, Entity **entity);
    void (*BreakForeachLoop)(void);
    void (*SetEditableVar)(byte type, const char *name, byte object, int storeOffset);
    void *(*GetEntityByID)(ushort objectID);
    int (*GetEntityID)(void *entityPtr);
    int (*GetEntityCount)(ushort type, bool32 isActive);
    int (*GetDrawListRef)(byte layerID, ushort entityID);
    void *(*GetDrawListRefPtr)(byte layerID, ushort entityID);
    void (*ResetEntityPtr)(void *entity, ushort type, void *data);
    void (*ResetEntitySlot)(ushort slotID, ushort type, void *data);
    Entity *(*CreateEntity)(ushort type, void *data, int x, int y);
    void (*CopyEntity)(void *destEntity, void *srcEntity, bool32 clearSrcEntity);
    bool32 (*CheckOnScreen)(void *entity, Vector2 *range);
    bool32 (*CheckPosOnScreen)(Vector2 *pos, Vector2 *range);
    void (*AddDrawListRef)(byte layer, ushort entityID);
    void (*SwapDrawListEntries)(byte layer, ushort entryStart, ushort entryEnd, ushort count);
    void (*SetDrawLayerProperties)(byte layer, bool32 sorted, void (*callback)(void));
    void (*SetScene)(const char *categoryName, const char *sceneName);
    void (*SetGameMode)(byte mode);
#if RETRO_USE_PLUS
    void (*SetHardResetFlag)(bool32 set);
#endif
    bool32 (*CheckValidScene)(void);
    int (*CheckStageFolder)(const char *folderName);
    int (*LoadScene)(void);
    int (*GetObjectIDByName)(const char *name);
    void (*ClearCameras)(void);
    void (*AddCamera)(Vector2 *pos, int offsetX, int offsetY, bool32 worldRelative);
#if !RETRO_USE_PLUS
    void *(*GetAPIFunction)(const char *funcName);
#endif
    int (*GetSettingsValue)(int id);
    void (*SetSettingsValue)(int id, int val);
    void (*UpdateWindow)(void);
    int (*Sin1024)(int angle);
    int (*Cos1024)(int angle);
    int (*Tan1024)(int angle);
    int (*ASin1024)(int angle);
    int (*ACos1024)(int angle);
    int (*Sin512)(int angle);
    int (*Cos512)(int angle);
    int (*Tan512)(int angle);
    int (*ASin512)(int angle);
    int (*ACos512)(int angle);
    int (*Sin256)(int angle);
    int (*Cos256)(int angle);
    int (*Tan256)(int angle);
    int (*ASin256)(int angle);
    int (*ACos256)(int angle);
    int (*Rand)(int min, int max);
    int (*Random)(int min, int max, int *randKey);
    void (*SetRandKey)(int key);
    byte (*ATan2)(int x, int y);
    void (*SetIdentityMatrix)(Matrix *matrix);
    void (*MatrixMultiply)(Matrix *dest, Matrix *matrixA, Matrix *matrixB);
    void (*MatrixTranslateXYZ)(Matrix *matrix, int x, int y, int z, bool32 setIdentity);
    void (*MatrixScaleXYZ)(Matrix *matrix, int scaleX, int scaleY, int scaleZ);
    void (*MatrixRotateX)(Matrix *matrix, int angle);
    void (*MatrixRotateY)(Matrix *matrix, int angle);
    void (*MatrixRotateZ)(Matrix *matrix, int angle);
    void (*MatrixRotateXYZ)(Matrix *matrix, int angleX, int angleY, int angleZ);
    void (*MatrixInverse)(Matrix *dest, Matrix *matrix);
    void (*MatrixCopy)(Matrix *matDest, Matrix *matSrc);
    void (*SetText)(TextInfo *textInfo, const char *text, uint size);
    void (*CopyString)(TextInfo *dst, TextInfo *src);
    void (*PrependText)(TextInfo *info, const char *text);
    void (*AppendString)(TextInfo *info, TextInfo *str);
    void (*AppendText)(TextInfo *info, const char *text);
    void (*LoadStrings)(TextInfo *dst, const char *path, int);
    bool32 (*SplitStringList)(TextInfo *list, TextInfo *strings, int start, int end);
    void (*GetCString)(char *text, TextInfo *info);
    bool32 (*StringCompare)(TextInfo *strA, TextInfo *strB, bool32 exactMatch);
    void (*GetDisplayInfo)(int *displayID, int *width, int *height, int *refreshRate, TextInfo *text);
    void (*GetWindowSize)(int *width, int *height);
    int (*SetScreenSize)(byte screenID, ushort width, ushort height);
    void (*SetClipBounds)(byte screenID, int x1, int y1, int x2, int y2);
#if RETRO_USE_PLUS
    void (*SetScreenSplitVerticies)(sbyte p2_1, sbyte p2_2, sbyte p3_1, sbyte p3_2, sbyte p3_3);
#endif
    short (*LoadSpriteSheet)(const char *path, Scopes scope);
    void (*SetLookupTable)(ushort *tablePtr);
    void (*SetPaletteMask)(uint maskColour);
    void (*SetPaletteEntry)(byte paletteID, byte index, uint colour);
    uint (*GetPaletteEntry)(byte paletteID, byte index);
    void (*SetActivePalette)(byte newActivePal, int startLine, int endLine);
    void (*CopyPalette)(byte sourcePalette, byte srcPaletteStart, byte destinationPalette, byte destPaletteStart, ushort count);
#if RETRO_USE_PLUS
    void (*LoadPalette)(byte bank, const char *path, ushort rowFlags);
#endif
    void (*RotatePalette)(byte palID, byte startIndex, byte endIndex, bool32 right);
    void (*SetLimitedFade)(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, ushort blendAmount, int startIndex, int endIndex);
#if RETRO_USE_PLUS
    void (*BlendColours)(byte paletteID, byte *coloursA, byte *coloursB, int alpha, int index, int count);
#endif
    void (*DrawRect)(int x, int y, int width, int height, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawLine)(int x1, int y1, int x2, int y2, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawCircle)(int x, int y, int radius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawCircleOutline)(int x, int y, int innerRadius, int outerRadius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawQuad)(Vector2 *verticies, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect);
    void (*DrawBlendedQuad)(Vector2 *verticies, colour *vertColours, int vertCount, int alpha, InkEffects inkEffect);
    void (*DrawSprite)(Animator *animator, Vector2 *position, bool32 screenRelative);
    void (*DrawDeformedSprite)(ushort sheet, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawText)(Animator *animator, Vector2 *position, TextInfo *info, int startCharID, int endCharID, Alignments align, int spacing, int a8,
                     Vector2 *charPos, bool32 ScreenRelative);
    void (*DrawTile)(ushort *tileInfo, int countX, int countY, Vector2 *position, Vector2 *offset, bool32 screenRelative);
    void (*CopyTile)(void);
    void (*DrawAniTiles)(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height);
    void (*FillScreen)(uint colour, int alphaR, int alphaG, int alphaB);
    ushort (*LoadMesh)(const char *filename, byte scope);
    ushort (*Create3DScene)(const char *identifier, ushort faceCount, byte scope);
    void (*Prepare3DScene)(ushort index);
    void (*SetDiffuseColour)(ushort index, int x, int y, int z);
    void (*SetDiffuseIntensity)(ushort index, int x, int y, int z);
    void (*SetSpecularIntensity)(ushort index, int x, int y, int z);
    void (*AddModelTo3DScene)(ushort modelIndex, ushort sceneIndex, byte type, Matrix *mat1, Matrix *mat2, colour colour);
    void (*SetModelAnimation)(ushort modelAnim, Animator *animator, short animSpeed, byte loopIndex, bool32 forceApply, ushort frameID);
    void (*AddMeshFrameTo3DScene)(ushort modelID, ushort sceneID, Animator *animator, byte drawMode, Matrix *mat1, Matrix *mat, colour colour);
    void (*Draw3DScene)(ushort index);
    ushort (*LoadSpriteAnimation)(const char *path, Scopes scope);
    ushort (*CreateSpriteAnimation)(const char *filename, uint frameCount, uint animCount, Scopes scope);
    void (*SetSpriteAnimation)(ushort spriteIndex, ushort animationID, Animator *animator, bool32 forceApply, short frameID);
    void (*EditSpriteAnimation)(ushort spriteIndex, ushort animID, const char *name, int frameOffset, ushort frameCount, short animSpeed,
                                byte loopIndex, byte rotationFlag);
    void (*SetSpriteString)(ushort spriteIndex, ushort animID, TextInfo *info);
    void *(*GetSpriteAnimation)(ushort sprIndex, const char *name);
    SpriteFrame *(*GetFrame)(ushort sprIndex, ushort anim, int frame);
    Hitbox *(*GetHitbox)(Animator *animator, byte hitboxID);
    short (*GetFrameID)(Animator *animator);
    int (*GetStringWidth)(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length, int spacing);
    void (*ProcessAnimation)(Animator *animator);
    int (*GetSceneLayerID)(const char *name);
    TileLayer *(*GetSceneLayer)(int layerID);
    void (*GetLayerSize)(ushort layer, Vector2 *size, bool32 pixelSize);
    ushort (*GetTileInfo)(ushort layer, int x, int y);
    void (*SetTileInfo)(ushort layer, int x, int y, ushort tile);
    int (*CopyTileLayer)(ushort dstLayer, int startX1, int startY1, ushort srcLayer, int startX2, int startY2, int countX, int countY);
    void (*ProcessParallax)(TileLayer *TileLayer);
    ScanlineInfo *(*GetScanlines)(void);
    bool32 (*CheckObjectCollisionTouchBox)(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox);
    bool32 (*CheckObjectCollisionTouchCircle)(void *thisEntity, int thisOffset, void *otherEntity, int otherOffset);
    byte (*CheckObjectCollisionBox)(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox, bool32 setPos);
    bool32 (*CheckObjectCollisionPlatform)(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox, bool32 setPos);
    bool32 (*ObjectTileCollision)(void *entity, ushort collisionLayers, byte collisionMode, byte collisionPlane, int xOffset, int yOffset,
                                  bool32 setPos);
    bool32 (*ObjectTileGrip)(void *entity, ushort collisionLayers, byte collisionMode, byte collisionPlane, int xOffset, int yOffset,
                             int tolerance);
    void (*ProcessTileCollisions)(void *entity, Hitbox *outer, Hitbox *inner);
    int (*GetTileAngle)(ushort tileID, byte cPlane, byte cMode);
    void (*SetTileAngle)(ushort tileID, byte cPlane, byte cMode, byte value);
    byte (*GetTileBehaviour)(ushort tileID, byte cPlane);
    void (*SetTileBehaviour)(ushort tileID, byte cPlane, byte value);
    int (*GetSFX)(const char *path);
    int (*PlaySfx)(ushort sfx, int loop, int pan);
    void (*StopSFX)(ushort sfx);
    int (*PlayStream)(const char *filename, uint slot, uint startPos, uint loopPoint, bool32 loadASync);
    int (*SetChannelAttributes)(byte slot, float volume, float pan, float speed);
    void (*StopChannel)(byte slot);
    void (*PauseChannel)(byte slot);
    void (*ResumeChannel)(byte slot);
    bool32 (*IsSFXPlaying)(byte slot);
    bool32 (*ChannelActive)(byte slot);
    uint (*GetChannelPos)(byte slot);
    void (*LoadVideo)(const char *filename, double a2, bool32 (*skipCallback)(void));
    bool32 (*LoadImage)(const char *filename, double displayLength, double speed, bool32 (*skipCallback)(void));
#if RETRO_USE_PLUS
    int (*ControllerIDForInputID)(byte inputID);
    int (*MostRecentActiveControllerID)(int deviceID, int a2, uint a3);
    int (*GetControllerType)(int inputID);
    int (*GetAssignedControllerID)(int inputID);
    int (*GetAssignedUnknown)(int inputID);
    int (*DoInputUnknown2)(int inputID, int a2, int a3);
    int (*DoInputUnknown3)(int inputID, int a2, int a3);
    int (*Missing24)(void);
    int (*DoInputUnknown2_Active)(byte inputID, int a2, int a3);
    int (*DoInputUnknown3_Active)(byte inputID, int a2, int a3);
    void (*AssignControllerID)(byte inputID, int deviceID);
    bool32 (*InputIDIsDisconnected)(byte inputID);
    void (*ResetControllerAssignments)(void);
#endif
#if !RETRO_USE_PLUS
    void (*Unknown92)(int a1, int a2, int *a3);
#endif
    int (*LoadUserFile)(const char *filename, void *buffer, uint size); // load user file from exe dir
    int (*SaveUserFile)(const char *fileName, void *buffer, uint size); // save use file to exe dir
#if RETRO_USE_PLUS
    void (*PrintLog)(SeverityModes severity, const char *message, ...);
    void (*PrintString)(SeverityModes severity, const char *message);
    void (*PrintText)(SeverityModes severity, TextInfo *text);
    void (*PrintIntegerUnsigned)(SeverityModes severity, const char *message, uint integer);
    void (*PrintInteger)(SeverityModes severity, const char *message, int integer);
    void (*PrintFloat)(SeverityModes severity, const char *message, float f);
    void (*PrintVector2)(SeverityModes severity, const char *message, int x, int y);
    void (*PrintHitbox)(SeverityModes severity, const char *message, Hitbox *hitbox);
#endif
    void (*SetActiveVariable)(int objectID, const char *name);
    void (*AddVarEnumValue)(const char *name);
#if RETRO_USE_PLUS
    void (*ClearDebugValues)(void);
    void (*SetDebugValue)(const char *name, void *valPtr, DebugVarTypes type, int min, int max);
#endif
#if !RETRO_USE_PLUS
    void (*PrintMessage)(void *message, byte type);
#endif
} RSDKFunctionTable;


#if RETRO_USE_MOD_LOADER
extern ModFunctionTable Mod;
#endif
#if RETRO_USE_PLUS
extern APIFunctionTable API;
#endif
extern RSDKFunctionTable RSDK;

#include "All.h"

#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, (byte)object->objectID, offsetof(Entity##object, var))
#define RSDK_ACTIVE_VAR(object, var)         RSDK.SetActiveVariable(object->objectID, #var)
#define RSDK_ENUM_VAR(var)                   RSDK.AddVarEnumValue(#var)
#define RSDK_ADD_OBJECT(object)                                                                                                                      \
    RSDK.RegisterObject((Object **)&object, #object, sizeof(Entity##object), sizeof(Object##object), object##_Update, object##_LateUpdate,           \
                        object##_StaticUpdate, object##_Draw, object##_Create, object##_StageLoad, object##_EditorDraw, object##_EditorLoad,         \
                        object##_Serialize)
#if RETRO_USE_PLUS
#define RSDK_ADD_OBJECT_CONTAINER(object) RSDK.RegisterObjectContainer((void **)&object, #object, sizeof(Object##object))
#endif

#define RSDK_THIS(type)             Entity##type *entity = (Entity##type *)RSDK_sceneInfo->entity
#define RSDK_GET_ENTITY(slot, type) ((Entity##type *)RSDK.GetEntityByID(slot))
#define SPAWN_CHILD(obj, data)      Entity##obj *child = (Entity##obj *)RSDK.CreateEntity(obj->objectID, (void*)(data), entity->position.x, entity->position.y)
#define CREATE_ENTITY(obj, data, x, y) ((Entity##obj *)RSDK.CreateEntity(obj->objectID, data, x, y))

#define INIT_TEXTINFO(info)                                                                                                                          \
    info.text       = NULL;                                                                                                                          \
    info.textLength = 0;                                                                                                                             \
    info.length     = 0

#define foreach_active(type, entityOut)                                                                                                              \
    Entity##type *entityOut = NULL;                                                                                                                  \
    while (RSDK.GetActiveEntities(type->objectID, (Entity **)&entityOut))
#define foreach_all(type, entityOut)                                                                                                                 \
    Entity##type *entityOut = NULL;                                                                                                                  \
    while (RSDK.GetEntities(type->objectID, (Entity **)&entityOut))
#define foreach_break                                                                                                                                \
    RSDK.BreakForeachLoop();                                                                                                                         \
    break
#define foreach_return                                                                                                                               \
    RSDK.BreakForeachLoop();                                                                                                                         \
    return

// used mainly for cutscenes
#define RSDK_GET_PLAYER(p1, p2, cam)                                                                                                                 \
    EntityPlayer *p1  = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);                                                                                       \
    EntityPlayer *p2  = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);                                                                                       \
    EntityCamera *cam = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

#define destroyEntity(entity) RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL)

#if RETRO_USE_PLUS
#define isMainGameMode() (globals->gameMode == MODE_MANIA || globals->gameMode == MODE_ENCORE)
#else
#define isMainGameMode() (globals->gameMode == MODE_NOSAVE || globals->gameMode == MODE_MANIA)
#endif

DLLExport void LinkGameLogicDLL(GameInfo *gameInfo);
#if RETRO_USE_MOD_LOADER
DLLExport bool32 LinkModLogic(GameInfo *info, const char *id);
#endif

#endif //! GAMEOBJECTS_H
