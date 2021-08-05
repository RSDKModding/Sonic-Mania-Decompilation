#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#define minVal(a, b)                      ((a) < (b) ? (a) : (b))
#define maxVal(a, b)                      ((a) > (b) ? (a) : (b))
#define clampVal(value, minimum, maximum) (((value) < (minimum)) ? (minimum) : (((value) > (maximum)) ? (maximum) : (value)))
#define fabs(a)                           ((a) > 0 ? (a) : -(a))

#define intToVoid(x)   (void *)(size_t)(x)
#define floatToVoid(x) intToVoid(*(int *)&(x))
#define voidToInt(x)   (int)(size_t)(x)
#define voidToFloat(x) *(float *)&(x)

#define unused(x) (void)x

typedef uint colour;
typedef uint color;

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
    void (*FetchLeaderboard)(int a1, int a2);
    void (*TrackScore)(const char *name, int score, void (*callback)(int status, int rank));
    void (*GetLeaderboardsUnknown)(void);
    void (*LeaderboardEntryCount)(void);
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
    void (*Unknown31)(ushort tableID);
    int (*GetUserDBStatus)(ushort tableID);
    void (*Unknown33)(ushort tableID, int a2, const char *name, void *value);
    void (*Unknown34)(ushort tableID, int a2, const char *name, void *value);
    int (*GetUserDBUnknownCount)(ushort tableID);
    int (*GetUserDBUnknown)(ushort tableID, ushort entryID);
    int (*AddUserDBEntry)(ushort tableID);
    void (*SetUserDBValue)(ushort tableID, int a2, int a3, const char *name, void *value);
    void (*Unknown39)(ushort tableID, int a2, int a3, const char *name, void *value);
    uint (*GetUserDBEntryUUID)(ushort tableID, ushort entryID);
    void (*GetUserDBByID)(ushort tableID, uint uuid);
    void (*GetUserDBCreationTime)(ushort tableID, ushort entry, char *buf, uint sizeInBytes, const char *format);
    void (*RemoveDBEntry)(ushort tableID, ushort entry);
    void (*RemoveAllDBEntries)(ushort tableID);
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
    void (*LoadScene)(const char *categoryName, const char *sceneName);
    void (*SetGameMode)(byte mode);
#if RETRO_USE_PLUS
    void (*SetHardResetFlag)(bool32 set);
#endif
    bool32 (*CheckValidScene)(void);
    int (*CheckStageFolder)(const char *folderName);
    int (*InitSceneLoad)(void);
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
    bool32 (*StringCompare)(TextInfo *strA, TextInfo *strB, bool32 flag);
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
    void (*DrawText)(Animator *animator, Vector2 *position, TextInfo *info, int endFrame, int textLength, Alignments align, int spacing, int a8,
                     Vector2 *charPos, bool32 ScreenRelative);
    void (*DrawTile)(ushort *tileInfo, int countX, int countY, void *entityPtr, Vector2 *position, bool32 screenRelative);
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
    int (*PlaySFX)(ushort sfx, int loop, int pan);
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

#if RETRO_USE_PLUS
extern APIFunctionTable API;
#endif
extern RSDKFunctionTable RSDK;

#include "PGZ/Acetone.h"
#include "Global/ActClear.h"
#include "AIZ/AIZEggRobo.h"
#include "AIZ/AIZEncoreTutorial.h"
#include "AIZ/AIZKingClaw.h"
#include "AIZ/AIZRockPile.h"
#include "AIZ/AIZSetup.h"
#include "AIZ/AIZTornado.h"
#include "AIZ/AIZTornadoPath.h"
#include "CPZ/AmoebaDroid.h"
#include "Credits/AnimalHBH.h"
#include "Global/Animals.h"
#include "Global/Announcer.h"
#include "Global/APICallback.h"
#include "OOZ/Aquis.h"
#include "MSZ/Armadiloid.h"
#include "Helpers/BadnikHelpers.h"
#include "CPZ/Ball.h"
#include "OOZ/BallCannon.h"
#include "TMZ/BallHog.h"
#include "MSZ/BarStool.h"
#include "HPZ/Batbot.h"
#include "GHZ/Batbrain.h"
#include "SSZ/Beanstalk.h"
#include "Common/BGSwitch.h"
#include "FBZ/BigSqueeze.h"
#include "MMZ/BladePole.h"
#include "FBZ/Blaster.h"
#include "HCZ/Blastoid.h"
#include "AIZ/Bloominator.h"
#include "SBZ/Bomb.h"
#include "SSZ/BouncePlant.h"
#include "Global/BoundsMarker.h"
#include "Common/BreakableWall.h"
#include "HCZ/BreakBar.h"
#include "GHZ/Bridge.h"
#include "BSS/BSS_Collectable.h"
#include "BSS/BSS_Collected.h"
#include "BSS/BSS_Horizon.h"
#include "BSS/BSS_HUD.h"
#include "BSS/BSS_Message.h"
#include "BSS/BSS_Palette.h"
#include "BSS/BSS_Player.h"
#include "BSS/BSS_Setup.h"
#include "CPZ/Bubbler.h"
#include "LRZ/BuckwildBall.h"
#include "HCZ/Buggernaut.h"
#include "MSZ/Bumpalo.h"
#include "SSZ/Bungee.h"
#include "GHZ/BurningLog.h"
#include "Common/Button.h"
#include "HCZ/ButtonDoor.h"
#include "GHZ/BuzzBomber.h"
#include "MMZ/BuzzSaw.h"
#include "SPZ/CableWarp.h"
#include "MSZ/Cactula.h"
#include "Global/Camera.h"
#include "SPZ/Canista.h"
#include "SBZ/Caterkiller.h"
#include "CPZ/CaterkillerJr.h"
#include "Cutscene/ChaosEmerald.h"
#include "GHZ/CheckerBall.h"
#include "CPZ/ChemBubble.h"
#include "CPZ/ChemicalBall.h"
#include "CPZ/ChemicalPool.h"
#include "GHZ/Chopper.h"
#include "SPZ/CircleBumper.h"
#include "SPZ/Clapperboard.h"
#include "FBZ/Clucker.h"
#include "Common/CollapsingPlatform.h"
#include "MSZ/CollapsingSand.h"
#include "Helpers/ColorHelpers.h"
#include "Global/Competition.h"
#include "Menu/CompetitionMenu.h"
#include "Common/CompetitionSession.h"
#include "SSZ/Constellation.h"
#include "Continue/ContinuePlayer.h"
#include "Continue/ContinueSetup.h"
#include "LRZ/ConveyorBelt.h"
#include "LRZ/ConveyorPlatform.h"
#include "MMZ/ConveyorWheel.h"
#include "GHZ/CorkscrewPath.h"
#include "Global/COverlay.h"
#include "CPZ/CPZ1Intro.h"
#include "CPZ/CPZ2Outro.h"
#include "CPZ/CPZBoss.h"
#include "CPZ/CPZSetup.h"
#include "CPZ/CPZShutter.h"
#include "GHZ/Crabmeat.h"
#include "FBZ/Crane.h"
#include "TMZ/CrashTest.h"
#include "PGZ/Crate.h"
#include "Credits/CreditsSetup.h"
#include "TMZ/CrimsonEye.h"
#include "LRZ/Current.h"
#include "Cutscene/CutsceneHBH.h"
#include "Cutscene/CutsceneRules.h"
#include "Cutscene/CutsceneSeq.h"
#include "FBZ/Cylinder.h"
#include "Menu/DAControl.h"
#include "SSZ/Dango.h"
#include "Menu/DASetup.h"
#include "LRZ/DashLift.h"
#include "MSZ/DBTower.h"
#include "HCZ/DCEvent.h"
#include "GHZ/DDWrecker.h"
#include "Global/Debris.h"
#include "Global/DebugMode.h"
#include "Common/Decoration.h"
#include "Menu/DemoMenu.h"
#include "GHZ/DERobot.h"
#include "Global/DialogRunner.h"
#include "SPZ/DirectorChair.h"
#include "HCZ/DiveEggman.h"
#include "CPZ/DNARiser.h"
#include "PGZ/DoorTrigger.h"
#include "PGZ/Dragonfly.h"
#include "Helpers/DrawHelpers.h"
#include "LRZ/Drillerdroid.h"
#include "LRZ/DrillerdroidO.h"
#include "Global/Dust.h"
#include "Menu/E3MenuSetup.h"
#include "SPZ/EggJanken.h"
#include "SPZ/EggJankenPart.h"
#include "MSZ/EggLoco.h"
#include "Common/Eggman.h"
#include "MMZ/EggPistonsMKII.h"
#include "Global/EggPrison.h"
#include "SSZ/EggTower.h"
#include "SPZ/EggTV.h"
#include "FBZ/ElectroMagnet.h"
#include "Credits/EncoreGoodEnd.h"
#include "AIZ/EncoreIntro.h"
#include "Global/EncoreRoute.h"
#include "ERZ/ERZGunner.h"
#include "ERZ/ERZKing.h"
#include "ERZ/ERZMystic.h"
#include "ERZ/ERZOutro.h"
#include "ERZ/ERZRider.h"
#include "ERZ/ERZSetup.h"
#include "ERZ/ERZShinobi.h"
#include "ERZ/ERZStart.h"
#include "TMZ/EscapeCar.h"
#include "Global/Explosion.h"
#include "Menu/ExtrasMenu.h"
#include "FBZ/Fan.h"
#include "MMZ/FarPlane.h"
#include "FBZ/FBZ1Outro.h"
#include "FBZ/FBZ2Outro.h"
#include "FBZ/FBZFan.h"
#include "FBZ/FBZMissile.h"
#include "FBZ/FBZSetup.h"
#include "FBZ/FBZSinkTrash.h"
#include "FBZ/FBZStorm.h"
#include "FBZ/FBZTrash.h"
#include "AIZ/FernParallax.h"
#include "SPZ/FilmProjector.h"
#include "SPZ/FilmReel.h"
#include "GHZ/Fireball.h"
#include "SSZ/Fireflies.h"
#include "SSZ/Firework.h"
#include "LRZ/Fireworm.h"
#include "FBZ/FlameSpring.h"
#include "LRZ/Flamethrower.h"
#include "TMZ/FlasherMKII.h"
#include "Common/FlingRamp.h"
#include "MSZ/Flipper.h"
#include "SSZ/FlowerPod.h"
#include "FBZ/FoldingPlatform.h"
#include "Common/ForceSpin.h"
#include "Common/ForceUnstick.h"
#include "PGZ/FrostThrower.h"
#include "SPZ/Funnel.h"
#include "Cutscene/FXExpandRing.h"
#include "Cutscene/FXFade.h"
#include "Cutscene/FXRuby.h"
#include "Cutscene/FXSpinRay.h"
#include "Cutscene/FXTrail.h"
#include "Cutscene/FXWaveRing.h"
#include "MMZ/Gachapandora.h"
#include "Global/GameOver.h"
#include "Helpers/GameProgress.h"
#include "OOZ/GasPlatform.h"
#include "Common/GenericTrigger.h"
#include "GHZ/GHZ2Outro.h"
#include "GHZ/GHZCutsceneK.h"
#include "GHZ/GHZCutsceneST.h"
#include "GHZ/GHZSetup.h"
#include "MSZ/GiantPistol.h"
#include "SSZ/GigaMetal.h"
#include "HCZ/Gondola.h"
#include "CPZ/Grabber.h"
#include "SPZ/GreenScreen.h"
#include "TMZ/GymBar.h"
#include "HCZ/HandLauncher.h"
#include "HCZ/HangConveyor.h"
#include "FBZ/HangGlider.h"
#include "FBZ/HangPoint.h"
#include "OOZ/Hatch.h"
#include "MSZ/Hatterkiller.h"
#include "HCZ/HCZ1Intro.h"
#include "HCZ/HCZOneWayDoor.h"
#include "HCZ/HCZSetup.h"
#include "HCZ/HCZSpikeBall.h"
#include "SPZ/HeavyGunner.h"
#include "LRZ/HeavyKing.h"
#include "MSZ/HeavyMystic.h"
#include "LRZ/HeavyRider.h"
#include "PGZ/HeavyShinobi.h"
#include "SSZ/HiLoSign.h"
#include "MSZ/Honkytonk.h"
#include "SSZ/Hotaru.h"
#include "SSZ/HotaruHiWatt.h"
#include "SSZ/HotaruMKII.h"
#include "LRZ/HPZEmerald.h"
#include "Global/HUD.h"
#include "PGZ/Ice.h"
#include "PGZ/IceBomba.h"
#include "PGZ/IceSpring.h"
#include "Global/ImageTrail.h"
#include "PGZ/Ink.h"
#include "PGZ/InkWipe.h"
#include "Global/InvincibleStars.h"
#include "Global/InvisibleBlock.h"
#include "Global/ItemBox.h"
#include "LRZ/Iwamodoki.h"
#include "TMZ/JacobsLadder.h"
#include "HPZ/Jawz.h"
#include "HCZ/Jellygnite.h"
#include "PGZ/JuggleSaw.h"
#include "SSZ/JunctionWheel.h"
#include "SSZ/Kabasira.h"
#include "SSZ/Kanabun.h"
#include "LRZ/KingAttack.h"
#include "LRZ/KingClaw.h"
#include "ERZ/KleptoMobile.h"
#include "TMZ/LargeGear.h"
#include "FBZ/Launcher.h"
#include "TMZ/LaunchSpring.h"
#include "HCZ/LaundroMobile.h"
#include "LRZ/LavaFall.h"
#include "LRZ/LavaGeyser.h"
#include "SPZ/LEDPanel.h"
#include "SPZ/Letterboard.h"
#include "Menu/LevelSelect.h"
#include "FBZ/LightBarrier.h"
#include "MSZ/LightBulb.h"
#include "Global/Localization.h"
#include "MSZ/LocoSmoke.h"
#include "Helpers/LogHelpers.h"
#include "Menu/LogoSetup.h"
#include "SPZ/LottoBall.h"
#include "SPZ/LottoMachine.h"
#include "SPZ/LoveTester.h"
#include "LRZ/LRZ1Intro.h"
#include "LRZ/LRZ1Outro.h"
#include "LRZ/LRZ1Setup.h"
#include "LRZ/LRZ2Setup.h"
#include "LRZ/LRZ3Cutscene.h"
#include "LRZ/LRZ3Outro.h"
#include "LRZ/LRZ3OutroK.h"
#include "LRZ/LRZ3Setup.h"
#include "LRZ/LRZConvControl.h"
#include "LRZ/LRZConvDropper.h"
#include "LRZ/LRZConveyor.h"
#include "LRZ/LRZConvItem.h"
#include "LRZ/LRZConvSwitch.h"
#include "LRZ/LRZFireball.h"
#include "LRZ/LRZRockPile.h"
#include "LRZ/LRZSpikeBall.h"
#include "LRZ/LRZSpiral.h"
#include "TMZ/MagnetSphere.h"
#include "FBZ/MagPlatform.h"
#include "FBZ/MagSpikeBall.h"
#include "Menu/MainMenu.h"
#include "Menu/ManiaModeMenu.h"
#include "Helpers/MathHelpers.h"
#include "MMZ/MatryoshkaBom.h"
#include "MMZ/MechaBu.h"
#include "HCZ/MegaChopper.h"
#include "OOZ/MegaOctus.h"
#include "Menu/MenuParam.h"
#include "Menu/MenuSetup.h"
#include "TMZ/MetalArm.h"
#include "SSZ/MetalSonic.h"
#include "OOZ/MeterDroid.h"
#include "SPZ/MicDrop.h"
#include "FBZ/Mine.h"
#include "MMZ/MMZ2Outro.h"
#include "MMZ/MMZLightning.h"
#include "MMZ/MMZSetup.h"
#include "MMZ/MMZWheel.h"
#include "TMZ/MonarchBG.h"
#include "SSZ/MonarchPlans.h"
#include "AIZ/MonkeyDude.h"
#include "GHZ/Motobug.h"
#include "SSZ/MSBomb.h"
#include "SSZ/MSFactory.h"
#include "SSZ/MSHologram.h"
#include "SSZ/MSOrb.h"
#include "SSZ/MSPanel.h"
#include "MSZ/MSZ1KIntro.h"
#include "MSZ/MSZ2Cutscene.h"
#include "MSZ/MSZCutsceneK.h"
#include "MSZ/MSZCutsceneST.h"
#include "MSZ/MSZSetup.h"
#include "MSZ/MSZSpotlight.h"
#include "Global/Music.h"
#include "PGZ/Newspaper.h"
#include "GHZ/Newtron.h"
#include "Global/NoSwap.h"
#include "OOZ/Octus.h"
#include "CPZ/OneWayDoor.h"
#include "OOZ/OOZ1Outro.h"
#include "OOZ/OOZ2Outro.h"
#include "OOZ/OOZFlames.h"
#include "OOZ/OOZSetup.h"
#include "Helpers/Options.h"
#include "Menu/OptionsMenu.h"
#include "SBZ/Orbinaut.h"
#include "LRZ/OrbitSpike.h"
#include "MSZ/PaintingEyes.h"
#include "Common/Palette.h"
#include "PGZ/PaperRoller.h"
#include "Common/ParallaxSprite.h"
#include "Helpers/ParticleHelpers.h"
#include "SPZ/PathInverter.h"
#include "Global/PauseMenu.h"
#include "Pinball/PBL_Bumper.h"
#include "Pinball/PBL_Camera.h"
#include "Pinball/PBL_Crane.h"
#include "Pinball/PBL_Flipper.h"
#include "Pinball/PBL_HUD.h"
#include "Pinball/PBL_Player.h"
#include "Pinball/PBL_Ring.h"
#include "Pinball/PBL_Sector.h"
#include "Pinball/PBL_Setup.h"
#include "Pinball/PBL_TargetBumper.h"
#include "Unused/Pendulum.h"
#include "PGZ/PetalPile.h"
#include "ERZ/PhantomEgg.h"
#include "ERZ/PhantomGunner.h"
#include "ERZ/PhantomHand.h"
#include "ERZ/PhantomKing.h"
#include "ERZ/PhantomMissile.h"
#include "ERZ/PhantomMystic.h"
#include "ERZ/PhantomRider.h"
#include "ERZ/PhantomRuby.h"
#include "ERZ/PhantomShield.h"
#include "ERZ/PhantomShinobi.h"
#include "SPZ/PimPom.h"
#include "MSZ/Pinata.h"
#include "MMZ/Piston.h"
#include "ERZ/PKingAttack.h"
#include "MMZ/PlaneSeeSaw.h"
#include "Global/PlaneSwitch.h"
#include "Common/Platform.h"
#include "Common/PlatformControl.h"
#include "Common/PlatformNode.h"
#include "Player/Player.h"
#include "Player/PlayerHelpers.h"
#include "Player/PlayerProbe.h"
#include "MMZ/PohBee.h"
#include "HCZ/Pointdexter.h"
#include "SPZ/PopcornKernel.h"
#include "SPZ/PopcornMachine.h"
#include "TMZ/PopOut.h"
#include "PGZ/Press.h"
#include "PGZ/PrintBlock.h"
#include "Common/Projectile.h"
#include "FBZ/Propeller.h"
#include "FBZ/PropellerShaft.h"
#include "PGZ/PSZ1Intro.h"
#include "PGZ/PSZ1Setup.h"
#include "PGZ/PSZ2Intro.h"
#include "PGZ/PSZ2Outro.h"
#include "PGZ/PSZ2Setup.h"
#include "PGZ/PSZDoor.h"
#include "PGZ/PSZEggman.h"
#include "PGZ/PSZLauncher.h"
#include "HCZ/PullChain.h"
#include "OOZ/PullSwitch.h"
#include "OOZ/PushSpring.h"
#include "Puyo/PuyoAI.h"
#include "Puyo/PuyoAttack.h"
#include "Puyo/PuyoBean.h"
#include "Puyo/PuyoGame.h"
#include "Puyo/PuyoIndicator.h"
#include "Puyo/PuyoLabel.h"
#include "Puyo/PuyoLevelSelect.h"
#include "Puyo/PuyoMatch.h"
#include "Puyo/PuyoScore.h"
#include "MSZ/Rattlekiller.h"
#include "CPZ/Reagent.h"
#include "HPZ/Redz.h"
#include "Helpers/ReplayDB.h"
#include "Global/ReplayRecorder.h"
#include "LRZ/Rexon.h"
#include "AIZ/Rhinobot.h"
#include "Global/Ring.h"
#include "ERZ/RingField.h"
#include "LRZ/RisingLava.h"
#include "LRZ/RockDrill.h"
#include "SPZ/RockemSockem.h"
#include "MSZ/RollerMKII.h"
#include "MSZ/RotatingSpikes.h"
#include "CPZ/RotatingStair.h"
#include "MMZ/RPlaneShifter.h"
#include "SSZ/RTeleporter.h"
#include "Cutscene/RubyPortal.h"
#include "Global/SaveGame.h"
#include "MMZ/Scarab.h"
#include "AIZ/SchrodingersCapsule.h"
#include "Global/ScoreBonus.h"
#include "HCZ/ScrewMobile.h"
#include "SSZ/SDashWheel.h"
#include "MSZ/SeeSaw.h"
#include "MSZ/SeltzerBottle.h"
#include "MSZ/SeltzerWater.h"
#include "TMZ/SentryBug.h"
#include "Global/Shield.h"
#include "PGZ/Shiversaw.h"
#include "SPZ/ShopWindow.h"
#include "PGZ/Shuriken.h"
#include "SPZ/Shutterbug.h"
#include "MSZ/SideBarrel.h"
#include "Global/SignPost.h"
#include "SSZ/SilverSonic.h"
#include "MMZ/SizeLaser.h"
#include "LRZ/SkyTeleporter.h"
#include "OOZ/Smog.h"
#include "PGZ/Snowflakes.h"
#include "OOZ/Sol.h"
#include "Global/Soundboard.h"
#include "PGZ/SP500.h"
#include "PGZ/SP500MkII.h"
#include "SSZ/SparkRail.h"
#include "HCZ/Spear.h"
#include "Unused/SpearBlock.h"
#include "UFO/SpecialClear.h"
#include "Global/SpecialRing.h"
#include "CPZ/SpeedBooster.h"
#include "Global/SpeedGate.h"
#include "FBZ/SpiderMobile.h"
#include "MMZ/SpikeCorridor.h"
#include "LRZ/SpikeCrusher.h"
#include "SSZ/SpikeFlail.h"
#include "GHZ/SpikeLog.h"
#include "Global/Spikes.h"
#include "Common/SpinBooster.h"
#include "SPZ/SpinSign.h"
#include "CPZ/Spiny.h"
#include "FBZ/SpiralPlatform.h"
#include "GHZ/Splats.h"
#include "Global/Spring.h"
#include "CPZ/Springboard.h"
#include "SPZ/SPZ1Intro.h"
#include "SPZ/SPZ1Setup.h"
#include "SPZ/SPZ2Outro.h"
#include "SPZ/SPZ2Setup.h"
#include "SSZ/SSZ1Intro.h"
#include "SSZ/SSZ1Outro.h"
#include "SSZ/SSZ1Setup.h"
#include "SSZ/SSZ2Setup.h"
#include "SSZ/SSZ3Cutscene.h"
#include "SSZ/SSZEggman.h"
#include "SSZ/SSZSpikeBall.h"
#include "SSZ/SSZSpotlight.h"
#include "CPZ/Staircase.h"
#include "LRZ/Stalactite.h"
#include "Global/StarPost.h"
#include "HPZ/Stegway.h"
#include "CPZ/StickyPlatform.h"
#include "Summary/Summary.h"
#include "Summary/SummaryEmerald.h"
#include "Global/SuperSparkle.h"
#include "AIZ/Sweep.h"
#include "MSZ/SwingRope.h"
#include "OOZ/SwitchDoor.h"
#include "CPZ/Syringe.h"
#include "Menu/TAEmerald.h"
#include "Unused/TargetBumper.h"
#include "FBZ/Technosqueek.h"
#include "TMZ/TeeterTotter.h"
#include "FBZ/TetherBall.h"
#include "Menu/ThanksSetup.h"
#include "LRZ/ThoughtBubble.h"
#include "Common/TilePlatform.h"
#include "Helpers/TimeAttackData.h"
#include "Global/TimeAttackGate.h"
#include "Menu/TimeAttackMenu.h"
#include "SSZ/TimePost.h"
#include "SSZ/TimeTravelSetup.h"
#include "CPZ/TippingPlatform.h"
#include "Title/Title3DSprite.h"
#include "Title/TitleBG.h"
#include "Global/TitleCard.h"
#include "Title/TitleEggman.h"
#include "Title/TitleLogo.h"
#include "Title/TitleSetup.h"
#include "Title/TitleSonic.h"
#include "TMZ/TMZ1Outro.h"
#include "TMZ/TMZ1Setup.h"
#include "TMZ/TMZ2Outro.h"
#include "TMZ/TMZ2Setup.h"
#include "TMZ/TMZ3Setup.h"
#include "TMZ/TMZAlert.h"
#include "TMZ/TMZBarrier.h"
#include "TMZ/TMZCable.h"
#include "TMZ/TMZFlames.h"
#include "MSZ/Tornado.h"
#include "MSZ/TornadoPath.h"
#include "LRZ/Toxomister.h"
#include "CPZ/TransportTube.h"
#include "Credits/TryAgain.h"
#include "Credits/TryAgainE.h"
#include "SSZ/TTCutscene.h"
#include "CPZ/TubeSpring.h"
#include "SPZ/Tubinaut.h"
#include "FBZ/Tuesday.h"
#include "LRZ/Turbine.h"
#include "HCZ/TurboSpiker.h"
#include "TMZ/TurboTurtle.h"
#include "PGZ/Turntable.h"
#include "LRZ/TurretSwitch.h"
#include "SPZ/TVFlyingBattery.h"
#include "SPZ/TVPole.h"
#include "SPZ/TVVan.h"
#include "CPZ/TwistedTubes.h"
#include "FBZ/TwistingDoor.h"
#include "HCZ/TwistingSlide.h"
#include "MSZ/UberCaterkiller.h"
#include "UFO/UFO_Camera.h"
#include "UFO/UFO_Circuit.h"
#include "UFO/UFO_Decoration.h"
#include "UFO/UFO_Dust.h"
#include "UFO/UFO_HUD.h"
#include "UFO/UFO_ItemBox.h"
#include "UFO/UFO_Message.h"
#include "UFO/UFO_Plasma.h"
#include "UFO/UFO_Player.h"
#include "UFO/UFO_Ring.h"
#include "UFO/UFO_Setup.h"
#include "UFO/UFO_Shadow.h"
#include "UFO/UFO_SpeedLines.h"
#include "UFO/UFO_Sphere.h"
#include "UFO/UFO_Springboard.h"
#include "UFO/UFO_Water.h"
#include "Menu/UIBackground.h"
#include "Menu/UIButton.h"
#include "Menu/UIButtonLabel.h"
#include "Menu/UIButtonPrompt.h"
#include "Menu/UICarousel.h"
#include "Menu/UICharButton.h"
#include "Menu/UIChoice.h"
#include "Menu/UIControl.h"
#include "Menu/UICreditsText.h"
#include "Menu/UIDialog.h"
#include "Menu/UIDiorama.h"
#include "Menu/UIHeading.h"
#include "Menu/UIInfoLabel.h"
#include "Menu/UIKeyBinder.h"
#include "Menu/UILeaderboard.h"
#include "Menu/UIMedallionPanel.h"
#include "Menu/UIModeButton.h"
#include "Menu/UIOptionPanel.h"
#include "Menu/UIPicture.h"
#include "Menu/UIPopover.h"
#include "Menu/UIRankButton.h"
#include "Menu/UIReplayCarousel.h"
#include "Menu/UIResPicker.h"
#include "Menu/UISaveSlot.h"
#include "Menu/UIShifter.h"
#include "Menu/UISlider.h"
#include "Menu/UISubHeading.h"
#include "Menu/UITABanner.h"
#include "Menu/UITAZoneModule.h"
#include "Menu/UIText.h"
#include "Menu/UITransition.h"
#include "Menu/UIUsernamePopup.h"
#include "Menu/UIVideo.h"
#include "Menu/UIVsCharSelector.h"
#include "Menu/UIVsResults.h"
#include "Menu/UIVsRoundPicker.h"
#include "Menu/UIVsScoreboard.h"
#include "Menu/UIVsZoneButton.h"
#include "Menu/UIWaitSpinner.h"
#include "Menu/UIWidgets.h"
#include "Menu/UIWinSize.h"
#include "SSZ/UncurlPlant.h"
#include "OOZ/Valve.h"
#include "MMZ/VanishPlatform.h"
#include "MSZ/Vultron.h"
#include "LRZ/WalkerLegs.h"
#include "TMZ/WallBumper.h"
#include "Unused/WallCrawl.h"
#include "FBZ/WarpDoor.h"
#include "Common/Water.h"
#include "GHZ/WaterfallSound.h"
#include "HCZ/WaterGush.h"
#include "SPZ/WeatherMobile.h"
#include "SPZ/WeatherTV.h"
#include "HCZ/Whirlpool.h"
#include "Unused/Wisp.h"
#include "PGZ/WoodChipper.h"
#include "PGZ/Woodrow.h"
#include "SSZ/YoyoPulley.h"
#include "GHZ/ZipLine.h"
#include "Global/Zone.h"

#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, (byte)object->objectID, offsetof(Entity##object, var))
#define RSDK_ACTIVE_VAR(object, var)         RSDK.SetActiveVariable(object->objectID, #var)
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

#endif //! GAMEOBJECTS_H
