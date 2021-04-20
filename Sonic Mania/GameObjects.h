#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#define maxVal(a, b) (a >= b ? a : b)
#define minVal(a, b) (a <= b ? a : b)

#define intToVoid(x) (void*)(size_t)(x)
#define voidToInt(x) (int)(size_t)(x)

// Userdata Table
typedef struct {
    int (*GetUserLanguage)(void);
    bool32 (*GetConfirmButtonFlip)(void);
    void (*ExitGame)(void);
    void (*LaunchManual)(void);
    void (*Unknown4)(byte inputID);
    bool32 (*CheckDLC)(GameDLC dlc);
    void (*ShowExtensionOverlay)(byte overlay);
    void (*UnlockAchievement)(const char *achName);
    void (*GetAchievementStatus)(void);
    void (*SetAchievementStatus)(int a1);
    void (*LeaderboardsUnknown4)(void);
    void (*FetchLeaderboard)(int a1, int a2);
    void (*TrackScore)(int a1, int a2, int a3);
    void (*GetLeaderboardsUnknown)(void);
    void (*LeaderboardEntryCount)(void);
    int (*GetLeaderboardUnknown2)(void);
    void (*Unknown12)(int a2, uint a3, int a4);
    void (*LeaderboardsUnknown8)(void);
    void (*ReadLeaderboardEntry)(uint a1);
    void (*SetRichPresence)(int, TextInfo *text);
    void (*TryTrackStat)(StatInfo *stat);
    int (*GetStatsStatus)(void);
    void (*SetStatsStatus)(int a1);
    void (*UserStorageUnknown8)(void);
    void (*TryAuth)(void);
    int (*GetUserAuthStatus)(void);
    void (*GetUsername)(TextInfo *text);
    void (*TryInitStorage)(void);
    int (*UserStorageStatusUnknown1)(void);
    int (*UserStorageStatusUnknown2)(void);
    void (*ClearUserStorageStatus)(void);
    void (*SetUserStorageStatus)(void);
    int (*UserStorageStatusUnknown3)(void);
    int (*UserStorageStatusUnknown4)(void);
    int (*UserStorageStatusUnknown5)(void);
    void (*SetUserStorageNoSave)(bool32 state);
    bool32 (*GetUserStorageNoSave)(void);
    void (*LoadUserFile)(const char *name, int *data, int size, void (*callback)(int status)); //load user file from game dir
    void (*SaveUserFile)(const char *name, int *data, int size, void (*callback)(int status), bool32 compress); // save user file to game dir
    void (*DeleteUserFile)(const char *filename, void (*callback)(int status));                                 // delete user file from game dir
    ushort (*InitUserDB)(const char *name, ...);
    ushort (*LoadUserDB)(const char *filename, int (*callback)(int status));
    void (*SaveUserDB)(ushort tableID, int (*callback)(int status));
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
} UserFunctionTable;

// Function Table
typedef struct {
    void (*InitGlobalVariables)(void **globals, int size);
    void (*RegisterObject)(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                         void (*staticUpdate)(void), void (*draw)(void), void(*create)(void *), void (*stageLoad)(void),
                         void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void));
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
    int (*ResetEntityPtr)(void *entity, ushort type, void *data);
    int (*ResetEntitySlot)(ushort slotID, ushort type, void *data);
    Entity* (*CreateEntity)(ushort type, void *data, int x, int y);
    void (*CopyEntity)(void *destEntity, void *srcEntity, bool32 clearSrcEntity);
    bool32 (*CheckOnScreen)(void *entity, Vector2 *range);
    bool32 (*CheckPosOnScreen)(Vector2 *pos, Vector2 *range);
    void (*AddDrawListRef)(byte layer, ushort entityID);
    void (*SwapDrawListEntries)(byte layer, ushort entryStart, ushort entryEnd, ushort count);
    void (*SetDrawLayerProperties)(byte layer, bool32 sorted, void (*callback)(void));
    void (*LoadScene)(const char* categoryName, const char* sceneName);
    void (*SetGameMode)(byte mode);
#if RETRO_USE_PLUS
    void (*SetHardResetFlag)(bool32 set);
#endif
    bool32 (*CheckValidScene)(void);
    int (*CheckStageFolder)(const char *folderName);
    int (*InitSceneLoad)(void);
    int (*GetObjectIDByName)(const char* name);
    void (*ClearCameras)(void);
    void (*AddCamera)(Vector2 *pos, int offsetX, int offsetY, bool32 worldRelative);
#if !RETRO_USE_PLUS
    void *(*GetFuncPtr)(const char *funcName);
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
    void (*PrependString)(TextInfo *info, const char *str);
    void (*AppendString)(TextInfo *info, const char *str);
    void (*Unknown67)(TextInfo *, TextInfo *);
    void (*LoadStrings)(TextInfo *dst, const char* path, int);
    void (*SplitStringList)(TextInfo *list, TextInfo *strings, int start, int end);
    void (*GetCString)(char *text, TextInfo *info);
    void (*StringCompare)(TextInfo *strA, TextInfo *strB, bool32 flag);
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
    void (*DrawSprite)(Animator *data, Vector2 *position, bool32 screenRelative);
    void (*DrawDeformedSprite)(ushort sheet, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawText)(Animator *data, Vector2 *position, TextInfo *info, int endFrame, int textLength, Alignments align, int spacing, int a8,
                     Vector2 *charPos, bool32 ScreenRelative);
    void (*DrawTile)(ushort *tileInfo, int countX, int countY, void *entityPtr, Vector2 *position, bool32 screenRelative);
    void (*CopyTile)(void);
    void (*DrawAniTiles)(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height);
    void (*FillScreen)(int a1, int a2, int a3, int a4);
    ushort (*LoadMesh)(const char* filename, byte scope);
    ushort (*Create3DScene)(const char* identifier, ushort faceCount, byte scope);
    void (*Prepare3DScene)(ushort index);
    void (*SetAmbientColour)(ushort index, int x, int y, int z);
    void (*SetDiffuseColour)(ushort index, int x, int y, int z);
    void (*SetSpecularColour)(ushort index, int x, int y, int z);
    void (*AddModelTo3DScene)(ushort modelIndex, ushort sceneIndex, byte type, Matrix *mat1, Matrix *mat2, colour colour);
    void (*SetModelAnimation)(ushort modelAnim, Animator *data, short animSpeed, byte loopIndex, bool32 forceApply, ushort frameID);
    void (*AddMeshFrameTo3DScene)(ushort modelID, ushort sceneID, Animator *data, byte drawMode, Matrix *mat1, Matrix *mat, colour colour);
    void (*Draw3DScene)(ushort index);
    ushort (*LoadSpriteAnimation)(const char *path, Scopes scope);
    ushort (*CreateSpriteAnimation)(const char *filename, uint frameCount, uint animCount, Scopes scope);
    void (*SetSpriteAnimation)(ushort spriteIndex, ushort animationID, Animator *data, bool32 forceApply, ushort frameID);
    void (*EditSpriteAnimation)(ushort spriteIndex, ushort animID, const char *name, int frameOffset, ushort frameCount, short animSpeed, byte loopIndex,
                          byte rotationFlag);
    void (*SetSpriteString)(ushort spriteIndex, ushort animID, TextInfo *info);
    void *(*GetSpriteAnimation)(ushort sprIndex, const char *name);
    SpriteFrame *(*GetFrame)(ushort sprIndex, ushort anim, int frame);
    Hitbox *(*GetHitbox)(Animator *data, byte hitboxID);
    short (*GetFrameID)(Animator *data);
    int (*GetStringWidth)(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length, int spacing);
    void (*ProcessAnimation)(Animator *data);
    int (*GetSceneLayerID)(const char *name);
    TileLayer *(*GetSceneLayer)(int layerID);
    void (*GetLayerSize)(ushort layer, Vector2 *size, bool32 pixelSize);
    ushort (*GetTileInfo)(ushort layer, int x, int y);
    void (*SetTileInfo)(ushort layer, int x, int y, ushort tile);
    int (*CopyTileLayer)(ushort dstLayer, int startX1, int startY1, ushort srcLayer, int startX2, int startY2, int countX, int countY);
    void (*ProcessParallax)(TileLayer *TileLayer);
    ScanlineInfo* (*GetScanlines)(void);
    bool32 (*CheckObjectCollisionTouchBox)(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox);
    bool32 (*CheckObjectCollisionTouchCircle)(void *thisEntity, int thisOffset, void *otherEntity, int otherOffset);
    byte (*CheckObjectCollisionBox)(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox, bool32 setPos);
    bool32 (*CheckObjectCollisionPlatform)(void *thisEntity, Hitbox *thisHitbox, void *otherEntity, Hitbox *otherHitbox, bool32 setPos);
    bool32 (*ObjectTileCollision)(void *entity, ushort collisionLayers, byte collisionMode, byte collisionPlane, int xOffset, int yOffset,
                                bool32 setPos);
    bool32 (*ObjectTileGrip)(void *entity, ushort collisionLayers, byte collisionMode, byte collisionPlane, int xOffset, int yOffset,
                             sbyte tolerance);
    void (*ProcessTileCollisions)(void *entity, Hitbox *outer, Hitbox *inner);
    int (*GetTileAngle)(ushort tileID, byte cPlane, byte cMode);
    void (*SetTileAngle)(ushort tileID, byte cPlane, byte cMode, byte value);
    byte (*GetTileBehaviour)(ushort tileID, byte cPlane);
    void (*SetTileBehaviour)(ushort tileID, byte cPlane, byte value);
    int (*GetSFX)(const char *path);
    int (*PlaySFX)(ushort sfx, int loop, int pan);
    void (*StopSFX)(ushort sfx);
    int (*PlayStream)(const char *filename, uint slot, int a3, uint loopPoint, bool32 loadASync);
    int (*SetChannelAttributes)(byte slot, float volume, float pan, float playbackSpeed);
    void (*StopChannel)(byte slot);
    void (*PauseChannel)(byte slot);
    void (*ResumeChannel)(byte slot);
    bool32 (*IsSFXPlaying)(byte slot);
    bool32 (*ChannelActive)(byte slot);
    int (*GetChannelPos)(byte slot);
    void (*LoadVideo)(const char *filename, double a2, bool32 (*skipCallback)(void));
    bool32 (*LoadImage)(const char *filename, double displayLength, double speed, bool32 (*skipCallback)(void));
#if RETRO_USE_PLUS
    int (*ControllerIDForInputID)(byte controllerID);
    int (*MostRecentActiveControllerID)(int inputID, int a2, uint a3);
    int (*Unknown100)(int inputID);
    int (*GetAssignedControllerID)(int inputID);
    int (*GetAssignedUnknown)(int inputID);
    int (*DoInputUnknown2)(int inputID, int a2, int a3);
    int (*DoInputUnknown3)(int inputID, int a2, int a3);
    int (*Missing24)(void);
    int (*DoInputUnknown2_Active)(byte controllerID, int a2, int a3);
    int (*DoInputUnknown3_Active)(byte controllerID, int a2, int a3);
    void (*AssignControllerID)(byte controllerID, int a2);
    bool32 (*InputIDIsDisconnected)(byte controllerID);
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
    void (*AddVarEnumValue)(const char* name);
#if RETRO_USE_PLUS
    void (*ClearDebugValues)(void);
    void (*SetDebugValue)(const char *name, void *valPtr, int type, int unknown1, int unknown2);
#endif
#if !RETRO_USE_PLUS
    void (*PrintMessage)(void *message, byte type);
#endif
} RSDKFunctionTable;

#if RETRO_USE_PLUS
extern UserFunctionTable User;
#endif
extern RSDKFunctionTable RSDK;

#include "Objects/Acetone.h"
#include "Objects/ActClear.h"
#include "Objects/AIZEggRobo.h"
#include "Objects/AIZEncoreTutorial.h"
#include "Objects/AIZKingClaw.h"
#include "Objects/AIZRockPile.h"
#include "Objects/AIZSetup.h"
#include "Objects/AIZTornado.h"
#include "Objects/AIZTornadoPath.h"
#include "Objects/AmoebaDroid.h"
#include "Objects/AnimalHBH.h"
#include "Objects/Animals.h"
#include "Objects/Announcer.h"
#include "Objects/APICallback.h"
#include "Objects/Aquis.h"
#include "Objects/Armadiloid.h"
#include "Objects/BadnikHelpers.h"
#include "Objects/Ball.h"
#include "Objects/BallCannon.h"
#include "Objects/BallHog.h"
#include "Objects/BarStool.h"
#include "Objects/Batbot.h"
#include "Objects/Batbrain.h"
#include "Objects/Beanstalk.h"
#include "Objects/BGSwitch.h"
#include "Objects/BigSqueeze.h"
#include "Objects/BladePole.h"
#include "Objects/Blaster.h"
#include "Objects/Blastoid.h"
#include "Objects/Bloominator.h"
#include "Objects/Bomb.h"
#include "Objects/BouncePlant.h"
#include "Objects/BoundsMarker.h"
#include "Objects/BreakableWall.h"
#include "Objects/BreakBar.h"
#include "Objects/Bridge.h"
#include "Objects/BSS_Collectable.h"
#include "Objects/BSS_Collected.h"
#include "Objects/BSS_Horizon.h"
#include "Objects/BSS_HUD.h"
#include "Objects/BSS_Message.h"
#include "Objects/BSS_Palette.h"
#include "Objects/BSS_Player.h"
#include "Objects/BSS_Setup.h"
#include "Objects/Bubbler.h"
#include "Objects/BuckwildBall.h"
#include "Objects/Buggernaut.h"
#include "Objects/Bumpalo.h"
#include "Objects/Bungee.h"
#include "Objects/BurningLog.h"
#include "Objects/Button.h"
#include "Objects/ButtonDoor.h"
#include "Objects/BuzzBomber.h"
#include "Objects/BuzzSaw.h"
#include "Objects/CableWarp.h"
#include "Objects/Cactula.h"
#include "Objects/Camera.h"
#include "Objects/Canista.h"
#include "Objects/Caterkiller.h"
#include "Objects/CaterkillerJr.h"
#include "Objects/ChaosEmerald.h"
#include "Objects/CheckerBall.h"
#include "Objects/ChemBubble.h"
#include "Objects/ChemicalBall.h"
#include "Objects/ChemicalPool.h"
#include "Objects/Chopper.h"
#include "Objects/CircleBumper.h"
#include "Objects/Clapperboard.h"
#include "Objects/Clucker.h"
#include "Objects/CollapsingPlatform.h"
#include "Objects/CollapsingSand.h"
#include "Objects/ColorHelpers.h"
#include "Objects/Competition.h"
#include "Objects/CompetitionMenu.h"
#include "Objects/CompetitionSession.h"
#include "Objects/Constellation.h"
#include "Objects/ContinuePlayer.h"
#include "Objects/ContinueSetup.h"
#include "Objects/ConveyorBelt.h"
#include "Objects/ConveyorPlatform.h"
#include "Objects/ConveyorWheel.h"
#include "Objects/CorkscrewPath.h"
#include "Objects/COverlay.h"
#include "Objects/CPZ1Intro.h"
#include "Objects/CPZ2Outro.h"
#include "Objects/CPZBoss.h"
#include "Objects/CPZSetup.h"
#include "Objects/CPZShutter.h"
#include "Objects/Crabmeat.h"
#include "Objects/Crane.h"
#include "Objects/CrashTest.h"
#include "Objects/Crate.h"
#include "Objects/CreditsSetup.h"
#include "Objects/CrimsonEye.h"
#include "Objects/Current.h"
#include "Objects/CutsceneHBH.h"
#include "Objects/CutsceneRules.h"
#include "Objects/CutsceneSeq.h"
#include "Objects/Cylinder.h"
#include "Objects/DAControl.h"
#include "Objects/Dango.h"
#include "Objects/DASetup.h"
#include "Objects/DashLift.h"
#include "Objects/DBTower.h"
#include "Objects/DCEvent.h"
#include "Objects/DDWrecker.h"
#include "Objects/Debris.h"
#include "Objects/DebugMode.h"
#include "Objects/Decoration.h"
#include "Objects/DemoMenu.h"
#include "Objects/DERobot.h"
#include "Objects/DialogRunner.h"
#include "Objects/DirectorChair.h"
#include "Objects/DiveEggman.h"
#include "Objects/DNARiser.h"
#include "Objects/DoorTrigger.h"
#include "Objects/Dragonfly.h"
#include "Objects/DrawHelpers.h"
#include "Objects/Drillerdroid.h"
#include "Objects/DrillerdroidO.h"
#include "Objects/Dust.h"
#include "Objects/E3MenuSetup.h"
#include "Objects/EggJanken.h"
#include "Objects/EggJankenPart.h"
#include "Objects/EggLoco.h"
#include "Objects/Eggman.h"
#include "Objects/EggPistonsMKII.h"
#include "Objects/EggPrison.h"
#include "Objects/EggTower.h"
#include "Objects/EggTV.h"
#include "Objects/ElectroMagnet.h"
#include "Objects/EncoreGoodEnd.h"
#include "Objects/EncoreIntro.h"
#include "Objects/EncoreRoute.h"
#include "Objects/ERZGunner.h"
#include "Objects/ERZKing.h"
#include "Objects/ERZMystic.h"
#include "Objects/ERZOutro.h"
#include "Objects/ERZRider.h"
#include "Objects/ERZSetup.h"
#include "Objects/ERZShinobi.h"
#include "Objects/ERZStart.h"
#include "Objects/EscapeCar.h"
#include "Objects/Explosion.h"
#include "Objects/ExtrasMenu.h"
#include "Objects/Fan.h"
#include "Objects/FarPlane.h"
#include "Objects/FBZ1Outro.h"
#include "Objects/FBZ2Outro.h"
#include "Objects/FBZFan.h"
#include "Objects/FBZMissile.h"
#include "Objects/FBZSetup.h"
#include "Objects/FBZSinkTrash.h"
#include "Objects/FBZStorm.h"
#include "Objects/FBZTrash.h"
#include "Objects/FernParallax.h"
#include "Objects/FilmProjector.h"
#include "Objects/FilmReel.h"
#include "Objects/Fireball.h"
#include "Objects/Fireflies.h"
#include "Objects/Firework.h"
#include "Objects/Fireworm.h"
#include "Objects/FlameSpring.h"
#include "Objects/Flamethrower.h"
#include "Objects/FlasherMKII.h"
#include "Objects/FlingRamp.h"
#include "Objects/Flipper.h"
#include "Objects/FlowerPod.h"
#include "Objects/FoldingPlatform.h"
#include "Objects/ForceSpin.h"
#include "Objects/ForceUnstick.h"
#include "Objects/FrostThrower.h"
#include "Objects/Funnel.h"
#include "Objects/FXExpandRing.h"
#include "Objects/FXFade.h"
#include "Objects/FXRuby.h"
#include "Objects/FXSpinRay.h"
#include "Objects/FXTrail.h"
#include "Objects/FXWaveRing.h"
#include "Objects/Gachapandora.h"
#include "Objects/GameOver.h"
#include "Objects/GameProgress.h"
#include "Objects/GasPlatform.h"
#include "Objects/GenericTrigger.h"
#include "Objects/GHZ2Outro.h"
#include "Objects/GHZCutsceneK.h"
#include "Objects/GHZCutsceneST.h"
#include "Objects/GHZSetup.h"
#include "Objects/GiantPistol.h"
#include "Objects/GigaMetal.h"
#include "Objects/Gondola.h"
#include "Objects/Grabber.h"
#include "Objects/GreenScreen.h"
#include "Objects/GymBar.h"
#include "Objects/HandLauncher.h"
#include "Objects/HangConveyor.h"
#include "Objects/HangGlider.h"
#include "Objects/HangPoint.h"
#include "Objects/Hatch.h"
#include "Objects/Hatterkiller.h"
#include "Objects/HCZ1Intro.h"
#include "Objects/HCZOneWayDoor.h"
#include "Objects/HCZSetup.h"
#include "Objects/HCZSpikeBall.h"
#include "Objects/HeavyGunner.h"
#include "Objects/HeavyKing.h"
#include "Objects/HeavyMystic.h"
#include "Objects/HeavyRider.h"
#include "Objects/HeavyShinobi.h"
#include "Objects/HiLoSign.h"
#include "Objects/Honkytonk.h"
#include "Objects/Hotaru.h"
#include "Objects/HotaruHiWatt.h"
#include "Objects/HotaruMKII.h"
#include "Objects/HPZEmerald.h"
#include "Objects/HUD.h"
#include "Objects/Ice.h"
#include "Objects/IceBomba.h"
#include "Objects/IceSpring.h"
#include "Objects/ImageTrail.h"
#include "Objects/Ink.h"
#include "Objects/InkWipe.h"
#include "Objects/InvincibleStars.h"
#include "Objects/InvisibleBlock.h"
#include "Objects/ItemBox.h"
#include "Objects/Iwamodoki.h"
#include "Objects/JacobsLadder.h"
#include "Objects/Jawz.h"
#include "Objects/Jellygnite.h"
#include "Objects/JuggleSaw.h"
#include "Objects/JunctionWheel.h"
#include "Objects/Kabasira.h"
#include "Objects/Kanabun.h"
#include "Objects/KingAttack.h"
#include "Objects/KingClaw.h"
#include "Objects/KleptoMobile.h"
#include "Objects/LargeGear.h"
#include "Objects/Launcher.h"
#include "Objects/LaunchSpring.h"
#include "Objects/LaundroMobile.h"
#include "Objects/LavaFall.h"
#include "Objects/LavaGeyser.h"
#include "Objects/LEDPanel.h"
#include "Objects/Letterboard.h"
#include "Objects/LevelSelect.h"
#include "Objects/LightBarrier.h"
#include "Objects/LightBulb.h"
#include "Objects/Localization.h"
#include "Objects/LocoSmoke.h"
#include "Objects/LogHelpers.h"
#include "Objects/LogoSetup.h"
#include "Objects/LottoBall.h"
#include "Objects/LottoMachine.h"
#include "Objects/LoveTester.h"
#include "Objects/LRZ1Intro.h"
#include "Objects/LRZ1Outro.h"
#include "Objects/LRZ1Setup.h"
#include "Objects/LRZ2Setup.h"
#include "Objects/LRZ3Cutscene.h"
#include "Objects/LRZ3Outro.h"
#include "Objects/LRZ3OutroK.h"
#include "Objects/LRZ3Setup.h"
#include "Objects/LRZConvControl.h"
#include "Objects/LRZConvDropper.h"
#include "Objects/LRZConveyor.h"
#include "Objects/LRZConvItem.h"
#include "Objects/LRZConvSwitch.h"
#include "Objects/LRZFireball.h"
#include "Objects/LRZRockPile.h"
#include "Objects/LRZSpikeBall.h"
#include "Objects/LRZSpiral.h"
#include "Objects/MagnetSphere.h"
#include "Objects/MagPlatform.h"
#include "Objects/MagSpikeBall.h"
#include "Objects/MainMenu.h"
#include "Objects/ManiaModeMenu.h"
#include "Objects/MathHelpers.h"
#include "Objects/MatryoshkaBom.h"
#include "Objects/MechaBu.h"
#include "Objects/MegaChopper.h"
#include "Objects/MegaOctus.h"
#include "Objects/MenuParam.h"
#include "Objects/MenuSetup.h"
#include "Objects/MetalArm.h"
#include "Objects/MetalSonic.h"
#include "Objects/MeterDroid.h"
#include "Objects/MicDrop.h"
#include "Objects/Mine.h"
#include "Objects/MMZ2Outro.h"
#include "Objects/MMZLightning.h"
#include "Objects/MMZSetup.h"
#include "Objects/MMZWheel.h"
#include "Objects/MonarchBG.h"
#include "Objects/MonarchPlans.h"
#include "Objects/MonkeyDude.h"
#include "Objects/Motobug.h"
#include "Objects/MSBomb.h"
#include "Objects/MSFactory.h"
#include "Objects/MSHologram.h"
#include "Objects/MSOrb.h"
#include "Objects/MSPanel.h"
#include "Objects/MSZ1KIntro.h"
#include "Objects/MSZ2Cutscene.h"
#include "Objects/MSZCutsceneK.h"
#include "Objects/MSZCutsceneST.h"
#include "Objects/MSZSetup.h"
#include "Objects/MSZSpotlight.h"
#include "Objects/Music.h"
#include "Objects/Newspaper.h"
#include "Objects/Newtron.h"
#include "Objects/NoSwap.h"
#include "Objects/Octus.h"
#include "Objects/OneWayDoor.h"
#include "Objects/OOZ1Outro.h"
#include "Objects/OOZ2Outro.h"
#include "Objects/OOZFlames.h"
#include "Objects/OOZSetup.h"
#include "Objects/Options.h"
#include "Objects/OptionsMenu.h"
#include "Objects/Orbinaut.h"
#include "Objects/OrbitSpike.h"
#include "Objects/PaintingEyes.h"
#include "Objects/Palette.h"
#include "Objects/PaperRoller.h"
#include "Objects/ParallaxSprite.h"
#include "Objects/ParticleHelpers.h"
#include "Objects/PathInverter.h"
#include "Objects/PauseMenu.h"
#include "Objects/PBL_Bumper.h"
#include "Objects/PBL_Camera.h"
#include "Objects/PBL_Crane.h"
#include "Objects/PBL_Flipper.h"
#include "Objects/PBL_HUD.h"
#include "Objects/PBL_Player.h"
#include "Objects/PBL_Ring.h"
#include "Objects/PBL_Sector.h"
#include "Objects/PBL_Setup.h"
#include "Objects/PBL_TargetBumper.h"
#include "Objects/Pendulum.h"
#include "Objects/PetalPile.h"
#include "Objects/PhantomEgg.h"
#include "Objects/PhantomGunner.h"
#include "Objects/PhantomHand.h"
#include "Objects/PhantomKing.h"
#include "Objects/PhantomMissile.h"
#include "Objects/PhantomMystic.h"
#include "Objects/PhantomRider.h"
#include "Objects/PhantomRuby.h"
#include "Objects/PhantomShield.h"
#include "Objects/PhantomShinobi.h"
#include "Objects/PimPom.h"
#include "Objects/Pinata.h"
#include "Objects/Piston.h"
#include "Objects/PKingAttack.h"
#include "Objects/PlaneSeeSaw.h"
#include "Objects/PlaneSwitch.h"
#include "Objects/Platform.h"
#include "Objects/PlatformControl.h"
#include "Objects/PlatformNode.h"
#include "Objects/Player.h"
#include "Objects/PlayerHelpers.h"
#include "Objects/PlayerProbe.h"
#include "Objects/PohBee.h"
#include "Objects/Pointdexter.h"
#include "Objects/PopcornKernel.h"
#include "Objects/PopcornMachine.h"
#include "Objects/PopOut.h"
#include "Objects/Press.h"
#include "Objects/PrintBlock.h"
#include "Objects/Projectile.h"
#include "Objects/Propeller.h"
#include "Objects/PropellerShaft.h"
#include "Objects/PSZ1Intro.h"
#include "Objects/PSZ1Setup.h"
#include "Objects/PSZ2Intro.h"
#include "Objects/PSZ2Outro.h"
#include "Objects/PSZ2Setup.h"
#include "Objects/PSZDoor.h"
#include "Objects/PSZEggman.h"
#include "Objects/PSZLauncher.h"
#include "Objects/PullChain.h"
#include "Objects/PullSwitch.h"
#include "Objects/PushSpring.h"
#include "Objects/PuyoAI.h"
#include "Objects/PuyoAttack.h"
#include "Objects/PuyoBean.h"
#include "Objects/PuyoGame.h"
#include "Objects/PuyoIndicator.h"
#include "Objects/PuyoLabel.h"
#include "Objects/PuyoLevelSelect.h"
#include "Objects/PuyoMatch.h"
#include "Objects/PuyoScore.h"
#include "Objects/Rattlekiller.h"
#include "Objects/Reagent.h"
#include "Objects/Redz.h"
#include "Objects/ReplayDB.h"
#include "Objects/ReplayRecorder.h"
#include "Objects/Rexon.h"
#include "Objects/Rhinobot.h"
#include "Objects/Ring.h"
#include "Objects/RingField.h"
#include "Objects/RisingLava.h"
#include "Objects/RockDrill.h"
#include "Objects/RockemSockem.h"
#include "Objects/RollerMKII.h"
#include "Objects/RotatingSpikes.h"
#include "Objects/RotatingStair.h"
#include "Objects/RPlaneShifter.h"
#include "Objects/RTeleporter.h"
#include "Objects/RubyPortal.h"
#include "Objects/SaveGame.h"
#include "Objects/Scarab.h"
#include "Objects/SchrodingersCapsule.h"
#include "Objects/ScoreBonus.h"
#include "Objects/ScrewMobile.h"
#include "Objects/SDashWheel.h"
#include "Objects/SeeSaw.h"
#include "Objects/SeltzerBottle.h"
#include "Objects/SeltzerWater.h"
#include "Objects/SentryBug.h"
#include "Objects/Shield.h"
#include "Objects/Shiversaw.h"
#include "Objects/ShopWindow.h"
#include "Objects/Shuriken.h"
#include "Objects/Shutterbug.h"
#include "Objects/SideBarrel.h"
#include "Objects/SignPost.h"
#include "Objects/SilverSonic.h"
#include "Objects/SizeLaser.h"
#include "Objects/SkyTeleporter.h"
#include "Objects/Smog.h"
#include "Objects/Snowflakes.h"
#include "Objects/Sol.h"
#include "Objects/Soundboard.h"
#include "Objects/SP500.h"
#include "Objects/SP500MkII.h"
#include "Objects/SparkRail.h"
#include "Objects/Spear.h"
#include "Objects/SpearBlock.h"
#include "Objects/SpecialClear.h"
#include "Objects/SpecialRing.h"
#include "Objects/SpeedBooster.h"
#include "Objects/SpeedGate.h"
#include "Objects/SpiderMobile.h"
#include "Objects/SpikeCorridor.h"
#include "Objects/SpikeCrusher.h"
#include "Objects/SpikeFlail.h"
#include "Objects/SpikeLog.h"
#include "Objects/Spikes.h"
#include "Objects/SpinBooster.h"
#include "Objects/SpinSign.h"
#include "Objects/Spiny.h"
#include "Objects/SpiralPlatform.h"
#include "Objects/Splats.h"
#include "Objects/Spring.h"
#include "Objects/Springboard.h"
#include "Objects/SPZ1Intro.h"
#include "Objects/SPZ1Setup.h"
#include "Objects/SPZ2Outro.h"
#include "Objects/SPZ2Setup.h"
#include "Objects/SSZ1Intro.h"
#include "Objects/SSZ1Outro.h"
#include "Objects/SSZ1Setup.h"
#include "Objects/SSZ2Setup.h"
#include "Objects/SSZ3Cutscene.h"
#include "Objects/SSZEggman.h"
#include "Objects/SSZSpikeBall.h"
#include "Objects/SSZSpotlight.h"
#include "Objects/Staircase.h"
#include "Objects/Stalactite.h"
#include "Objects/StarPost.h"
#include "Objects/Stegway.h"
#include "Objects/StickyPlatform.h"
#include "Objects/Summary.h"
#include "Objects/SummaryEmerald.h"
#include "Objects/SuperSparkle.h"
#include "Objects/Sweep.h"
#include "Objects/SwingRope.h"
#include "Objects/SwitchDoor.h"
#include "Objects/Syringe.h"
#include "Objects/TAEmerald.h"
#include "Objects/TargetBumper.h"
#include "Objects/Technosqueek.h"
#include "Objects/TeeterTotter.h"
#include "Objects/TetherBall.h"
#include "Objects/ThanksSetup.h"
#include "Objects/ThoughtBubble.h"
#include "Objects/TilePlatform.h"
#include "Objects/TimeAttackData.h"
#include "Objects/TimeAttackGate.h"
#include "Objects/TimeAttackMenu.h"
#include "Objects/TimePost.h"
#include "Objects/TimeTravelSetup.h"
#include "Objects/TippingPlatform.h"
#include "Objects/Title3DSprite.h"
#include "Objects/TitleBG.h"
#include "Objects/TitleCard.h"
#include "Objects/TitleEggman.h"
#include "Objects/TitleLogo.h"
#include "Objects/TitleSetup.h"
#include "Objects/TitleSonic.h"
#include "Objects/TMZ1Outro.h"
#include "Objects/TMZ1Setup.h"
#include "Objects/TMZ2Outro.h"
#include "Objects/TMZ2Setup.h"
#include "Objects/TMZ3Setup.h"
#include "Objects/TMZAlert.h"
#include "Objects/TMZBarrier.h"
#include "Objects/TMZCable.h"
#include "Objects/TMZFlames.h"
#include "Objects/Tornado.h"
#include "Objects/TornadoPath.h"
#include "Objects/Toxomister.h"
#include "Objects/TransportTube.h"
#include "Objects/TryAgain.h"
#include "Objects/TryAgainE.h"
#include "Objects/TTCutscene.h"
#include "Objects/TubeSpring.h"
#include "Objects/Tubinaut.h"
#include "Objects/Tuesday.h"
#include "Objects/Turbine.h"
#include "Objects/TurboSpiker.h"
#include "Objects/TurboTurtle.h"
#include "Objects/Turntable.h"
#include "Objects/TurretSwitch.h"
#include "Objects/TVFlyingBattery.h"
#include "Objects/TVPole.h"
#include "Objects/TVVan.h"
#include "Objects/TwistedTubes.h"
#include "Objects/TwistingDoor.h"
#include "Objects/TwistingSlide.h"
#include "Objects/UberCaterkiller.h"
#include "Objects/UFO_Camera.h"
#include "Objects/UFO_Circuit.h"
#include "Objects/UFO_Decoration.h"
#include "Objects/UFO_Dust.h"
#include "Objects/UFO_HUD.h"
#include "Objects/UFO_ItemBox.h"
#include "Objects/UFO_Message.h"
#include "Objects/UFO_Plasma.h"
#include "Objects/UFO_Player.h"
#include "Objects/UFO_Ring.h"
#include "Objects/UFO_Setup.h"
#include "Objects/UFO_Shadow.h"
#include "Objects/UFO_SpeedLines.h"
#include "Objects/UFO_Sphere.h"
#include "Objects/UFO_Springboard.h"
#include "Objects/UFO_Water.h"
#include "Objects/UIBackground.h"
#include "Objects/UIButton.h"
#include "Objects/UIButtonLabel.h"
#include "Objects/UIButtonPrompt.h"
#include "Objects/UICarousel.h"
#include "Objects/UICharButton.h"
#include "Objects/UIChoice.h"
#include "Objects/UIControl.h"
#include "Objects/UICreditsText.h"
#include "Objects/UIDialog.h"
#include "Objects/UIDiorama.h"
#include "Objects/UIHeading.h"
#include "Objects/UIInfoLabel.h"
#include "Objects/UIKeyBinder.h"
#include "Objects/UILeaderboard.h"
#include "Objects/UIMedallionPanel.h"
#include "Objects/UIModeButton.h"
#include "Objects/UIOptionPanel.h"
#include "Objects/UIPicture.h"
#include "Objects/UIPopover.h"
#include "Objects/UIRankButton.h"
#include "Objects/UIReplayCarousel.h"
#include "Objects/UIResPicker.h"
#include "Objects/UISaveSlot.h"
#include "Objects/UIShifter.h"
#include "Objects/UISlider.h"
#include "Objects/UISubHeading.h"
#include "Objects/UITABanner.h"
#include "Objects/UITAZoneModule.h"
#include "Objects/UIText.h"
#include "Objects/UITransition.h"
#include "Objects/UIUsernamePopup.h"
#include "Objects/UIVideo.h"
#include "Objects/UIVsCharSelector.h"
#include "Objects/UIVsResults.h"
#include "Objects/UIVsRoundPicker.h"
#include "Objects/UIVsScoreboard.h"
#include "Objects/UIVsZoneButton.h"
#include "Objects/UIWaitSpinner.h"
#include "Objects/UIWidgets.h"
#include "Objects/UIWinSize.h"
#include "Objects/UncurlPlant.h"
#include "Objects/Valve.h"
#include "Objects/VanishPlatform.h"
#include "Objects/Vultron.h"
#include "Objects/WalkerLegs.h"
#include "Objects/WallBumper.h"
#include "Objects/WallCrawl.h"
#include "Objects/WarpDoor.h"
#include "Objects/Water.h"
#include "Objects/WaterfallSound.h"
#include "Objects/WaterGush.h"
#include "Objects/WeatherMobile.h"
#include "Objects/WeatherTV.h"
#include "Objects/Whirlpool.h"
#include "Objects/Wisp.h"
#include "Objects/WoodChipper.h"
#include "Objects/Woodrow.h"
#include "Objects/YoyoPulley.h"
#include "Objects/ZipLine.h"
#include "Objects/Zone.h"

#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, object->objectID, offsetof(Entity##object, var))
#define RSDK_ACTIVE_VAR(object, var)         RSDK.SetActiveVariable(object->objectID, #var)
#define RSDK_ADD_OBJECT(object)                                                                                                                      \
    RSDK.RegisterObject((Object **)&object, #object, sizeof(Entity##object), sizeof(Object##object), object##_Update, object##_LateUpdate,             \
                      object##_StaticUpdate, object##_Draw, object##_Create, object##_StageLoad, object##_EditorDraw, object##_EditorLoad,           \
                      object##_Serialize)
#if RETRO_USE_PLUS
#define RSDK_ADD_OBJECT_CONTAINER(object) RSDK.RegisterObjectContainer((void **)&object, #object, sizeof(Object##object))
#endif

#define RSDK_THIS(type) Entity##type *entity = (Entity##type *)RSDK_sceneInfo->entity
#define RSDK_GET_ENTITY(slot, type) ((Entity##type *)RSDK.GetEntityByID(slot))

#define foreach_active(type, entityOut)                                                                                                              \
    Entity##type *entityOut = NULL;                                                                                                                  \
    while (RSDK.GetActiveEntities(type->objectID, (Entity **)&entityOut))
#define foreach_all(type, entityOut)                                                                                                                 \
    Entity##type *entityOut = NULL;                                                                                                                  \
    while (RSDK.GetEntities(type->objectID, (Entity **)&entityOut))
#define foreach_break                                                                                                                                \
    RSDK.BreakForeachLoop();                                                                                                                         \
    break
#define foreach_return                                                                                                                                \
    RSDK.BreakForeachLoop();                                                                                                                         \
    return


DLLExport void LinkGameLogicDLL(GameInfo *gameInfo);

#endif //! GAMEOBJECTS_H
