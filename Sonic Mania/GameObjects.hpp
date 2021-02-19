#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

// Userdata Table
struct UserFunctionTable {
    void (*Unknown1)(void);
    void (*GetConfirmButtonFlip)(void);
    void (*Unknown2)(void);
    void (*Unknown3)(void);
    void (*Unknown4)(int a1);
    bool32 (*CheckDLC)(GameDLC dlc);
    void (*Unknown5)(int a1);
    void (*UnlockAchievement)(const char *achName);
    void (*Missing0)(void);
    void (*Unknown6)(int a1);
    void (*Unknown7)(void);
    void (*Unknown8)(int a1, int a2);
    void (*Unknown9)(int a1, int a2, int a3);
    void (*Unknown10)(void);
    void (*Unknown11)(void);
    void (*Missing1)(void);
    void (*Unknown12)(int a2, uint a3, int a4);
    void (*Missing2)(void);
    void (*Unknown13)(uint a1);
    void (*PossiblyGetStrings)(void *, void *);
    void (*Unknown14)(int a1);
    void (*Missing3)(void);
    void (*Unknown15)(int a1);
    void (*Unknown16)(void);
    void (*Unknown17)(void);
    void (*Unknown18)(void);
    void (*Unknown19)(int a1);
    void (*Unknown20)(void);
    void (*Unknown21)(void);
    void (*Unknown22)(void);
    void (*Unknown23)(void);
    void (*Unknown24)(void);
    void (*Missing4)(void);
    void (*Unknown25)(void);
    void (*Unknown26)(void);
    void (*Unknown27)(int a1);
    void (*Unknown28)(void);
    void (*LoadUserFile)(const char *name, int *data, int size, void (*callback)(int status));
    void (*SaveUserFile)(const char *name, int *data, int size, void (*callback)(int status), bool32 compress);
    void (*DeleteUserFile)(const char *filename, void (*callback)(int status));
    void (*OpenUserDB)(void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *, void *);
    void (*LoadReplayDB)(const char *filename, void (*callback)(int status));
    void (*SaveReplayDB)(ushort tableID, void (*callback)(int status));
    void (*Unknown30)(ushort a1);
    void (*Missing5)(void);
    void (*Unknown31)(ushort a1);
    void (*Unknown32)(ushort a1);
    void (*Unknown33)(ushort a1, int a2, int a3, int a4);
    void (*Unknown34)(ushort a1, int a2, int a3, int a4);
    void (*Unknown35)(ushort a1);
    void (*Unknown36)(ushort a1, ushort a2);
    void (*Unknown37)(ushort a1);
    void (*Unknown38)(void *, void *, void *, void *, void *);
    void (*Unknown39)(short a1, int a2, int a3, int a4, void *dst);
    void (*AddUserDB)(void *, void *);
    void (*Unknown40)(ushort a1, int a2);
    void (*GetTimeFormat)(ushort a1, int a2, const char *Buf, uint sizeInBytes, const char *format);
    void (*Unknown42)(ushort a1, int a2);
    void (*Unknown43)(short a1);
    // count: 59
};

// Function Table
struct RSDKFunctionTable {
    void (*InitGameOptions)(void *options, int size);
    void (*CreateObject)(Object **structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                         void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void),
                         void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void));
    void (*CreateObjectContainer)(Object **structPtr, const char *name, uint objectSize);
    bool32 (*GetActiveObjects)(ushort group, Entity **entity);
    bool32 (*GetObjects)(ushort type, Entity **entity);
    void (*NextForEachLoop)(void);
    void (*SetEditableVar)(byte type, const char *name, byte object, int storeOffset);
    Entity *(*GetObjectByID)(ushort objectID);
    int (*GetEntityID)(Entity *entityPtr);
    int (*GetEntityCount)(ushort type, bool32 isActive);
    int (*GetDrawListRef)(byte layerID, ushort entityID);
    Entity *(*GetDrawListRefPtr)(byte layerID, ushort entityID);
    int (*DestroyEntity)(Entity *entity, ushort type, void *data);
    int (*ResetEntity)(ushort slotID, ushort type, void *data);
    Entity* (*SpawnEntity)(ushort type, void *data, int x, int y);
    void (*CopyEntity)(void *destEntity, Entity *srcEntity, bool32 clearSrcEntity);
    void (*CheckOnScreen)(void);
    void (*CheckPosOnScreen)(void);
    void (*AddDrawListRef)(void);
    void (*SwapDrawLayers)(void);
    void (*SetDrawLayerProperties)(byte layer, int active, void (*initDrawPtr)(void));
    void (*LoadScene)(const char* categoryName, const char* sceneName);
    void (*SetGameMode)(byte mode);
    void (*SetHardResetFlag)(bool set);
    void (*CheckValidScene)(void);
    int (*CheckStageFolder)(const char *folderName);
    int (*InitSceneLoad)(void);
    int (*GetObjectIDByName)(const char* name);
    void (*ClearScreens)(void);
    void (*AddScreen)(int x, int y, int w, int h);
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
    void (*MatrixTranslateXYZ)(Matrix *matrix, short x, short y, short z, bool32 setIdentity);
    void (*MatrixScaleXYZ)(Matrix *matrix, short scaleX, short scaleY, short scaleZ);
    void (*MatrixRotateX)(Matrix *matrix, short angle);
    void (*MatrixRotateY)(Matrix *matrix, short angle);
    void (*MatrixRotateZ)(Matrix *matrix, short angle);
    void (*MatrixRotateXYZ)(Matrix *matrix, short angleX, short angleY, short angleZ);
    void (*MatrixInverse)(Matrix *dest, Matrix *matrix);
    void (*MatrixCopy)(Matrix *matDest, Matrix *matSrc);
    void (*SetText)(TextInfo *textInfo, char *text, uint size);
    void (*Unknown64)(void);
    void (*Unknown65)(void);
    void (*Unknown66)(void);
    void (*Unknown67)(void);
    void (*LoadStrings)(void);
    void (*Unknown68)(void);
    void (*CopyString)(char *text, TextInfo *info);
    void (*Unknown69)(void);
    void (*Unknown70)(void);
    void (*Unknown71)(void);
    int (*SetScreenSize)(void *, void *, void *, void *, void *);
    void (*SetClipBounds)(byte screenID, int x1, int y1, int x2, int y2);
    void (*SetScreenUnknown)(byte, byte, byte, byte, byte);
    short (*LoadSpriteSheet)(const char *path, Scopes scope);
    void (*SetLookupTable)(ushort *tablePtr);
    void (*SetPaletteMask)(uint maskColour);
    void (*SetPaletteEntry)(byte paletteID, byte index, uint colour);
    uint (*GetPaletteEntry)(byte paletteID, byte index);
    void (*SetActivePalette)(byte newActivePal, int startLine, int endLine);
    void (*CopyPalette)(byte sourcePalette, byte srcPaletteStart, byte destinationPalette, byte destPaletteStart, ushort count);
    void (*LoadPalette)(byte bank, char *path, ushort rowFlags);
    void (*RotatePalette)(byte palID, byte startIndex, byte endIndex, bool right);
    void (*SetLimitedFade)(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, ushort blendAmount, int startIndex, int endIndex);
    void (*BlendColours)(byte paletteID, byte *coloursA, byte *coloursB, int alpha, int index, int count);
    void (*DrawRect)(int x, int y, int width, int height, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawLine)(int x1, int y1, int x2, int y2, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawCircle)(int x, int y, int radius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawCircleOutline)(int x, int y, int innerRadius, int outerRadius, uint colour, int alpha, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawQuad)(Vector2 *verticies, int vertCount, int r, int g, int b, int alpha, InkEffects inkEffect);
    void (*DrawTexturedQuad)(Vector2 *verticies, Vector2 *vertexUVs, int vertCount, int alpha, InkEffects inkEffect);
    void (*DrawSprite)(EntityAnimationData *data, Vector2 *position, bool32 screenRelative);
    void (*DrawDeformedSprite)(ushort sheet, InkEffects inkEffect, bool32 screenRelative);
    void (*DrawText)(EntityAnimationData *data, Vector2 *position, TextInfo *info, int endFrame, int textLength, FlipFlags direction, int a7, int a8,
                     int a9, bool32 ScreenRelative);
    void (*DrawTile)(ushort *tileInfo, int countX, int countY, Entity *entityPtr, Vector2 *position, bool32 screenRelative);
    void (*CopyTile)(void);
    void (*DrawAniTile)(ushort sheetID, ushort tileIndex, ushort srcX, ushort srcY, ushort width, ushort height);
    int (*FillScreen)(int a1, int a2, int a3, int a4);
    void (*LoadMesh)(void);
    void (*Create3DScene)(void);
    void (*Init3DScene)(void);
    void (*View_Something1)(void);
    void (*View_Something2)(void);
    void (*View_Something3)(void);
    void (*SetupMesh)(void);
    void (*SetModelAnimation)(ushort modelAnim, EntityAnimationData *data, short animSpeed, byte loopIndex, bool forceApply, ushort frameID);
    void (*SetupMeshAnimation)(void);
    void (*Draw3DScene)(void);
    ushort (*LoadAnimation)(const char *path, Scopes scope);
    ushort (*CreateAnimation)(const char *filename, uint frameCount, uint animCount, Scopes scope);
    void (*SetSpriteAnimation)(ushort spriteIndex, ushort animationID, EntityAnimationData *data, bool forceApply, ushort frameID);
    void (*EditAnimation)(ushort spriteIndex, ushort animID, const char *name, int frameOffset, ushort frameCount, short animSpeed, byte loopIndex,
                          byte rotationFlag);
    void (*SetSpriteString)(ushort spriteIndex, ushort animID, TextInfo *info);
    void (*GetAnimation)(ushort sprIndex, const char *name);
    SpriteFrame *(*GetFrame)(ushort sprIndex, ushort anim, int frame);
    Hitbox *(*GetHitbox)(EntityAnimationData *data, byte hitboxID);
    short (*GetFrameID)(EntityAnimationData *data);
    void (*GetStringWidth)(ushort sprIndex, ushort animID, TextInfo *info, int startIndex, int length, int spacing);
    void (*ProcessAnimation)(EntityAnimationData *data);
    TileLayer *(*GetSceneLayer)(int layerID);
    int (*GetSceneLayerID)(const char *name);
    void (*GetLayerSize)(ushort layer, Vector2 *size, bool32 pixelSize);
    void (*GetTileInfo)(ushort layer, int x, int y);
    void (*SetTileInfo)(ushort layer, int x, int y, ushort tile);
    int (*CopyTileLayer)(ushort dstLayer, int startX1, int startY1, ushort srcLayer, int startX2, int startY2, int countX, int countY);
    void (*ProcessParallax)(TileLayer *TileLayer);
    ScanlineInfo* (*GetScanlines)(void);
    bool32 (*CheckObjectCollisionTouchBox)(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox);
    bool32 (*CheckObjectCollisionTouchCircle)(Entity *thisEntity, int thisOffset, Entity *otherEntity, int otherOffset);
    int (*CheckObjectCollisionBox)(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setPos);
    bool32 (*CheckObjectCollisionPlatform)(Entity *thisEntity, Hitbox *thisHitbox, Entity *otherEntity, Hitbox *otherHitbox, bool32 setPos);
    bool32 (*ObjectTileCollision)(Entity *entity, ushort collisionLayers, byte collisionMode, byte collisionPlane, int xOffset, int yOffset,
                                bool32 setPos);
    bool32 (*ObjectTileGrip)(Entity *entity, ushort collisionLayers, byte collisionMode, byte collisionPlane, int xOffset, int yOffset,
                             sbyte tolerance);
    void (*ProcessTileCollisions)(Entity *entity, Hitbox *outer, Hitbox *inner);
    void (*GetTileAngle)(ushort tileID, byte cPlane, byte cMode);
    void (*SetTileAngle)(ushort tileID, byte cPlane, byte cMode, byte value);
    void (*GetTileBehaviour)(ushort tileID, byte cPlane);
    void (*SetTileBehaviour)(ushort tileID, byte cPlane, byte value);
    int (*GetSFX)(const char *path);
    int (*PlaySFX)(ushort sfx, int loop, int pan);
    void (*StopSFX)(ushort sfx);
    void (*PlayMusic)(const char *filename, uint slot, int a3, uint loopStart, bool32 loop);
    int (*SetSoundAttributes)(byte a1, uint slot, float volume, float pan, float playbackSpeed);
    void (*Unknown91)(int a1);
    void (*Unknown92)(int a1);
    void (*Unknown93)(int a1);
    void (*Unknown94)(short a1);
    void (*Unknown95)(int a1);
    void (*Unknown96)(int a1);
    void (*LoadVideo)(const char *filename, int64 a2, int (*a3)(void));
    bool32 (*LoadImage)(const char *filename, double displayLength, double speed, bool32 (*skipCallback)(void));
    void (*Unknown98)(int a1, int a2, uint a3);
    void (*Unknown99)(int a1, int a2, uint a3);
    void (*Unknown100)(int a1);
    void (*Unknown101)(int a1);
    void (*Missing21)(int a1);
    void (*Missing22)(int a1, int a2, int a3);
    void (*Missing23)(int a1, int a2, int a3);
    void (*Missing24)(void);
    void (*Missing25)(byte a1, int a2, int a3);
    void (*Missing26)(byte a1, int a2, int a3);
    void (*Unknown102)(byte a1, int a2);
    void (*Unknown103)(byte a1);
    void (*Unknown104)(void);
    void (*LoadUserFile)(const char *filename, void *buffer, int filesize);
    void (*SaveUserFile)(const char *fileName, void *buffer, uint size);
    void (*printLog)(SeverityModes severity, const char *message, ...);
    void (*PrintString)(SeverityModes severity, const char *message);
    void (*printText)(SeverityModes severity, const char *message, TextInfo *text);
    void (*printIntegerUnsigned)(SeverityModes severity, const char *message, uint integer);
    void (*PrintInteger)(SeverityModes severity, const char *message, int integer);
    void (*printFloat)(SeverityModes severity, const char *message, float f);
    void (*printVector2)(SeverityModes severity, const char *message, int x, int y);
    void (*printHitbox)(SeverityModes severity, const char *message, Hitbox *hitbox);
    void (*Unknown105)(void);
    void (*Unknown106)(void);
    void (*ClearDebugValues)(void);
    void (*SetDebugValue)(const char *name, int valPtr, int type, int unknown1, int unknown2);
};

extern UserFunctionTable User;
extern RSDKFunctionTable RSDK;

#include "Objects/ActClear.hpp"
#include "Objects/AIZEggRobo.hpp"
#include "Objects/AIZEncoreTutorial.hpp"
#include "Objects/AIZKingClaw.hpp"
#include "Objects/AIZRockPile.hpp"
#include "Objects/AIZSetup.hpp"
#include "Objects/AIZTornado.hpp"
#include "Objects/AIZTornadoPath.hpp"
#include "Objects/AmoebaDroid.hpp"
#include "Objects/AnimalHBH.hpp"
#include "Objects/Animals.hpp"
#include "Objects/Announcer.hpp"
#include "Objects/Aquis.hpp"
#include "Objects/Armadiloid.hpp"
#include "Objects/BallCannon.hpp"
#include "Objects/BallHog.hpp"
#include "Objects/BarStool.hpp"
#include "Objects/Batbrain.hpp"
#include "Objects/Beanstalk.hpp"
#include "Objects/BGSwitch.hpp"
#include "Objects/BigSqueeze.hpp"
#include "Objects/BladePole.hpp"
#include "Objects/Blaster.hpp"
#include "Objects/Blastoid.hpp"
#include "Objects/Bloominator.hpp"
#include "Objects/BouncePlant.hpp"
#include "Objects/BoundsMarker.hpp"
#include "Objects/BreakableWall.hpp"
#include "Objects/BreakBar.hpp"
#include "Objects/Bridge.hpp"
#include "Objects/BSS_Collectable.hpp"
#include "Objects/BSS_Collected.hpp"
#include "Objects/BSS_Horizon.hpp"
#include "Objects/BSS_HUD.hpp"
#include "Objects/BSS_Message.hpp"
#include "Objects/BSS_Palette.hpp"
#include "Objects/BSS_Player.hpp"
#include "Objects/BSS_Setup.hpp"
#include "Objects/BuckwildBall.hpp"
#include "Objects/Buggernaut.hpp"
#include "Objects/Bumpalo.hpp"
#include "Objects/Bungee.hpp"
#include "Objects/BurningLog.hpp"
#include "Objects/Button.hpp"
#include "Objects/ButtonDoor.hpp"
#include "Objects/BuzzBomber.hpp"
#include "Objects/BuzzSaw.hpp"
#include "Objects/CableWarp.hpp"
#include "Objects/Cactula.hpp"
#include "Objects/Camera.hpp"
#include "Objects/Canista.hpp"
#include "Objects/CaterkillerJr.hpp"
#include "Objects/ChaosEmerald.hpp"
#include "Objects/CheckerBall.hpp"
#include "Objects/ChemBubble.hpp"
#include "Objects/ChemicalBall.hpp"
#include "Objects/ChemicalPool.hpp"
#include "Objects/Chopper.hpp"
#include "Objects/CircleBumper.hpp"
#include "Objects/Clapperboard.hpp"
#include "Objects/Clucker.hpp"
#include "Objects/CollapsingPlatform.hpp"
#include "Objects/CollapsingSand.hpp"
#include "Objects/Competition.hpp"
#include "Objects/CompetitionMenu.hpp"
#include "Objects/Constellation.hpp"
#include "Objects/ContinuePlayer.hpp"
#include "Objects/ContinueSetup.hpp"
#include "Objects/ConveyorBelt.hpp"
#include "Objects/ConveyorPlatform.hpp"
#include "Objects/ConveyorWheel.hpp"
#include "Objects/CorkscrewPath.hpp"
#include "Objects/COverlay.hpp"
#include "Objects/CPZ1Intro.hpp"
#include "Objects/CPZ2Outro.hpp"
#include "Objects/CPZBoss.hpp"
#include "Objects/CPZSetup.hpp"
#include "Objects/CPZShutter.hpp"
#include "Objects/Crabmeat.hpp"
#include "Objects/Crane.hpp"
#include "Objects/CrashTest.hpp"
#include "Objects/Crate.hpp"
#include "Objects/CreditsSetup.hpp"
#include "Objects/CrimsonEye.hpp"
#include "Objects/Current.hpp"
#include "Objects/CutsceneHBH.hpp"
#include "Objects/CutsceneSeq.hpp"
#include "Objects/Cylinder.hpp"
#include "Objects/DAControl.hpp"
#include "Objects/Dango.hpp"
#include "Objects/DASetup.hpp"
#include "Objects/DashLift.hpp"
#include "Objects/DBTower.hpp"
#include "Objects/DCEvent.hpp"
#include "Objects/DDWrecker.hpp"
#include "Objects/Debris.hpp"
#include "Objects/DebugMode.hpp"
#include "Objects/Decoration.hpp"
#include "Objects/DERobot.hpp"
#include "Objects/DialogRunner.hpp"
#include "Objects/DirectorChair.hpp"
#include "Objects/DiveEggman.hpp"
#include "Objects/DNARiser.hpp"
#include "Objects/DoorTrigger.hpp"
#include "Objects/Dragonfly.hpp"
#include "Objects/Drillerdroid.hpp"
#include "Objects/DrillerdroidO.hpp"
#include "Objects/Dust.hpp"
#include "Objects/EggLoco.hpp"
#include "Objects/Eggman.hpp"
#include "Objects/EggPistonsMKII.hpp"
#include "Objects/EggPrison.hpp"
#include "Objects/EggTower.hpp"
#include "Objects/EggTV.hpp"
#include "Objects/ElectroMagnet.hpp"
#include "Objects/EncoreGoodEnd.hpp"
#include "Objects/EncoreIntro.hpp"
#include "Objects/EncoreRoute.hpp"
#include "Objects/ERZGunner.hpp"
#include "Objects/ERZKing.hpp"
#include "Objects/ERZMystic.hpp"
#include "Objects/ERZOutro.hpp"
#include "Objects/ERZRider.hpp"
#include "Objects/ERZSetup.hpp"
#include "Objects/ERZShinobi.hpp"
#include "Objects/ERZStart.hpp"
#include "Objects/EscapeCar.hpp"
#include "Objects/Explosion.hpp"
#include "Objects/ExtrasMenu.hpp"
#include "Objects/Fan.hpp"
#include "Objects/FarPlane.hpp"
#include "Objects/FBZ1Outro.hpp"
#include "Objects/FBZ2Outro.hpp"
#include "Objects/FBZFan.hpp"
#include "Objects/FBZMissile.hpp"
#include "Objects/FBZSetup.hpp"
#include "Objects/FBZSinkTrash.hpp"
#include "Objects/FBZStorm.hpp"
#include "Objects/FBZTrash.hpp"
#include "Objects/FernParallax.hpp"
#include "Objects/FilmProjector.hpp"
#include "Objects/FilmReel.hpp"
#include "Objects/Fireflies.hpp"
#include "Objects/Firework.hpp"
#include "Objects/Fireworm.hpp"
#include "Objects/FlameSpring.hpp"
#include "Objects/Flamethrower.hpp"
#include "Objects/FlasherMKII.hpp"
#include "Objects/FlingRamp.hpp"
#include "Objects/Flipper.hpp"
#include "Objects/FlowerPod.hpp"
#include "Objects/FoldingPlatform.hpp"
#include "Objects/ForceSpin.hpp"
#include "Objects/ForceUnstick.hpp"
#include "Objects/FrostThrower.hpp"
#include "Objects/Funnel.hpp"
#include "Objects/FXExpandRing.hpp"
#include "Objects/FXFade.hpp"
#include "Objects/FXRuby.hpp"
#include "Objects/FXSpinRay.hpp"
#include "Objects/FXTrail.hpp"
#include "Objects/FXWaveRing.hpp"
#include "Objects/Gachapandora.hpp"
#include "Objects/GameProgress.hpp"
#include "Objects/GameOver.hpp"
#include "Objects/GasPlatform.hpp"
#include "Objects/GenericTrigger.hpp"
#include "Objects/GHZ2Outro.hpp"
#include "Objects/GHZCutsceneK.hpp"
#include "Objects/GHZCutsceneST.hpp"
#include "Objects/GHZSetup.hpp"
#include "Objects/GiantPistol.hpp"
#include "Objects/GigaMetal.hpp"
#include "Objects/Gondola.hpp"
#include "Objects/Grabber.hpp"
#include "Objects/GreenScreen.hpp"
#include "Objects/GymBar.hpp"
#include "Objects/HandLauncher.hpp"
#include "Objects/HangConveyor.hpp"
#include "Objects/HangGlider.hpp"
#include "Objects/HangPoint.hpp"
#include "Objects/Hatch.hpp"
#include "Objects/Hatterkiller.hpp"
#include "Objects/HCZ1Intro.hpp"
#include "Objects/HCZOneWayDoor.hpp"
#include "Objects/HCZSetup.hpp"
#include "Objects/HCZSpikeBall.hpp"
#include "Objects/HeavyGunner.hpp"
#include "Objects/HeavyKing.hpp"
#include "Objects/HeavyMystic.hpp"
#include "Objects/HeavyRider.hpp"
#include "Objects/HeavyShinobi.hpp"
#include "Objects/HiLoSign.hpp"
#include "Objects/Honkytonk.hpp"
#include "Objects/Hotaru.hpp"
#include "Objects/HotaruHiWatt.hpp"
#include "Objects/HotaruMKII.hpp"
#include "Objects/HPZEmerald.hpp"
#include "Objects/HUD.hpp"
#include "Objects/Ice.hpp"
#include "Objects/IceBomba.hpp"
#include "Objects/IceSpring.hpp"
#include "Objects/ImageTrail.hpp"
#include "Objects/Ink.hpp"
#include "Objects/InkWipe.hpp"
#include "Objects/InvincibleStars.hpp"
#include "Objects/InvisibleBlock.hpp"
#include "Objects/ItemBox.hpp"
#include "Objects/Iwamodoki.hpp"
#include "Objects/JacobsLadder.hpp"
#include "Objects/Jawz.hpp"
#include "Objects/Jellygnite.hpp"
#include "Objects/JuggleSaw.hpp"
#include "Objects/JunctionWheel.hpp"
#include "Objects/Kabasira.hpp"
#include "Objects/Kanabun.hpp"
#include "Objects/KingAttack.hpp"
#include "Objects/KingClaw.hpp"
#include "Objects/KleptoMobile.hpp"
#include "Objects/LargeGear.hpp"
#include "Objects/Launcher.hpp"
#include "Objects/LaunchSpring.hpp"
#include "Objects/LaundroMobile.hpp"
#include "Objects/LavaFall.hpp"
#include "Objects/LavaGeyser.hpp"
#include "Objects/LEDPanel.hpp"
#include "Objects/Letterboard.hpp"
#include "Objects/LevelSelect.hpp"
#include "Objects/LightBarrier.hpp"
#include "Objects/LightBulb.hpp"
#include "Objects/Localization.hpp"
#include "Objects/LocoSmoke.hpp"
#include "Objects/LogoSetup.hpp"
#include "Objects/LottoBall.hpp"
#include "Objects/LottoMachine.hpp"
#include "Objects/LoveTester.hpp"
#include "Objects/LRZ1Intro.hpp"
#include "Objects/LRZ1Outro.hpp"
#include "Objects/LRZ1Setup.hpp"
#include "Objects/LRZ2Setup.hpp"
#include "Objects/LRZ3Cutscene.hpp"
#include "Objects/LRZ3Outro.hpp"
#include "Objects/LRZ3OutroK.hpp"
#include "Objects/LRZ3Setup.hpp"
#include "Objects/LRZConvControl.hpp"
#include "Objects/LRZConvDropper.hpp"
#include "Objects/LRZConveyor.hpp"
#include "Objects/LRZConvItem.hpp"
#include "Objects/LRZConvSwitch.hpp"
#include "Objects/LRZFireball.hpp"
#include "Objects/LRZRockPile.hpp"
#include "Objects/LRZSpikeBall.hpp"
#include "Objects/LRZSpiral.hpp"
#include "Objects/MagnetSphere.hpp"
#include "Objects/MagPlatform.hpp"
#include "Objects/MagSpikeBall.hpp"
#include "Objects/MainMenu.hpp"
#include "Objects/ManiaModeMenu.hpp"
#include "Objects/MatryoshkaBom.hpp"
#include "Objects/MechaBu.hpp"
#include "Objects/MegaChopper.hpp"
#include "Objects/MegaOctus.hpp"
#include "Objects/MenuParam.hpp"
#include "Objects/MenuSetup.hpp"
#include "Objects/MetalArm.hpp"
#include "Objects/MetalSonic.hpp"
#include "Objects/MeterDroid.hpp"
#include "Objects/MicDrop.hpp"
#include "Objects/Mine.hpp"
#include "Objects/MMZ2Outro.hpp"
#include "Objects/MMZLightning.hpp"
#include "Objects/MMZSetup.hpp"
#include "Objects/MMZWheel.hpp"
#include "Objects/MonarchBG.hpp"
#include "Objects/MonarchPlans.hpp"
#include "Objects/MonkeyDude.hpp"
#include "Objects/Motobug.hpp"
#include "Objects/MSBomb.hpp"
#include "Objects/MSFactory.hpp"
#include "Objects/MSHologram.hpp"
#include "Objects/MSOrb.hpp"
#include "Objects/MSPanel.hpp"
#include "Objects/MSZ1KIntro.hpp"
#include "Objects/MSZ2Cutscene.hpp"
#include "Objects/MSZCutsceneK.hpp"
#include "Objects/MSZCutsceneST.hpp"
#include "Objects/MSZSetup.hpp"
#include "Objects/MSZSpotlight.hpp"
#include "Objects/Music.hpp"
#include "Objects/Newspaper.hpp"
#include "Objects/Newtron.hpp"
#include "Objects/NoSwap.hpp"
#include "Objects/Octus.hpp"
#include "Objects/OneWayDoor.hpp"
#include "Objects/OOZ1Outro.hpp"
#include "Objects/OOZ2Outro.hpp"
#include "Objects/OOZFlames.hpp"
#include "Objects/OOZSetup.hpp"
#include "Objects/Options.hpp"
#include "Objects/OptionsMenu.hpp"
#include "Objects/OrbitSpike.hpp"
#include "Objects/PaintingEyes.hpp"
#include "Objects/Palette.hpp"
#include "Objects/PaperRoller.hpp"
#include "Objects/ParallaxSprite.hpp"
#include "Objects/PathInverter.hpp"
#include "Objects/PauseMenu.hpp"
#include "Objects/PBL_Bumper.hpp"
#include "Objects/PBL_Camera.hpp"
#include "Objects/PBL_Crane.hpp"
#include "Objects/PBL_Flipper.hpp"
#include "Objects/PBL_HUD.hpp"
#include "Objects/PBL_Player.hpp"
#include "Objects/PBL_Ring.hpp"
#include "Objects/PBL_Sector.hpp"
#include "Objects/PBL_Setup.hpp"
#include "Objects/PBL_TargetBumper.hpp"
#include "Objects/PetalPile.hpp"
#include "Objects/PhantomEgg.hpp"
#include "Objects/PhantomGunner.hpp"
#include "Objects/PhantomHand.hpp"
#include "Objects/PhantomKing.hpp"
#include "Objects/PhantomMissile.hpp"
#include "Objects/PhantomMystic.hpp"
#include "Objects/PhantomRider.hpp"
#include "Objects/PhantomRuby.hpp"
#include "Objects/PhantomShield.hpp"
#include "Objects/PhantomShinobi.hpp"
#include "Objects/PimPom.hpp"
#include "Objects/Pinata.hpp"
#include "Objects/Piston.hpp"
#include "Objects/PKingAttack.hpp"
#include "Objects/PlaneSeeSaw.hpp"
#include "Objects/PlaneSwitch.hpp"
#include "Objects/Platform.hpp"
#include "Objects/PlatformControl.hpp"
#include "Objects/PlatformNode.hpp"
#include "Objects/Player.hpp"
#include "Objects/PlayerProbe.hpp"
#include "Objects/PohBee.hpp"
#include "Objects/Pointdexter.hpp"
#include "Objects/PopcornKernel.hpp"
#include "Objects/PopcornMachine.hpp"
#include "Objects/PopOut.hpp"
#include "Objects/Press.hpp"
#include "Objects/PrintBlock.hpp"
#include "Objects/Projectile.hpp"
#include "Objects/Propeller.hpp"
#include "Objects/PropellerShaft.hpp"
#include "Objects/PSZ1Intro.hpp"
#include "Objects/PSZ1Setup.hpp"
#include "Objects/PSZ2Intro.hpp"
#include "Objects/PSZ2Outro.hpp"
#include "Objects/PSZ2Setup.hpp"
#include "Objects/PSZDoor.hpp"
#include "Objects/PSZEggman.hpp"
#include "Objects/PSZLauncher.hpp"
#include "Objects/PullChain.hpp"
#include "Objects/PullSwitch.hpp"
#include "Objects/PushSpring.hpp"
#include "Objects/PuyoAI.hpp"
#include "Objects/PuyoAttack.hpp"
#include "Objects/PuyoBean.hpp"
#include "Objects/PuyoGame.hpp"
#include "Objects/PuyoIndicator.hpp"
#include "Objects/PuyoLabel.hpp"
#include "Objects/PuyoLevelSelect.hpp"
#include "Objects/PuyoMatch.hpp"
#include "Objects/PuyoScore.hpp"
#include "Objects/Rattlekiller.hpp"
#include "Objects/Reagent.hpp"
#include "Objects/ReplayDB.hpp"
#include "Objects/ReplayRecorder.hpp"
#include "Objects/Rexon.hpp"
#include "Objects/Rhinobot.hpp"
#include "Objects/Ring.hpp"
#include "Objects/RingField.hpp"
#include "Objects/RisingLava.hpp"
#include "Objects/RockDrill.hpp"
#include "Objects/RockemSockem.hpp"
#include "Objects/RollerMKII.hpp"
#include "Objects/RotatingSpikes.hpp"
#include "Objects/RotatingStair.hpp"
#include "Objects/RPlaneShifter.hpp"
#include "Objects/RTeleporter.hpp"
#include "Objects/RubyPortal.hpp"
#include "Objects/SaveGame.hpp"
#include "Objects/Scarab.hpp"
#include "Objects/SchrodingersCapsule.hpp"
#include "Objects/ScoreBonus.hpp"
#include "Objects/ScrewMobile.hpp"
#include "Objects/SDashWheel.hpp"
#include "Objects/SeeSaw.hpp"
#include "Objects/SeltzerBottle.hpp"
#include "Objects/SeltzerWater.hpp"
#include "Objects/SentryBug.hpp"
#include "Objects/Shield.hpp"
#include "Objects/Shiversaw.hpp"
#include "Objects/ShopWindow.hpp"
#include "Objects/Shuriken.hpp"
#include "Objects/Shutterbug.hpp"
#include "Objects/SideBarrel.hpp"
#include "Objects/SignPost.hpp"
#include "Objects/SilverSonic.hpp"
#include "Objects/SizeLaser.hpp"
#include "Objects/SkyTeleporter.hpp"
#include "Objects/Smog.hpp"
#include "Objects/Snowflakes.hpp"
#include "Objects/Sol.hpp"
#include "Objects/Soundboard.hpp"
#include "Objects/SP500.hpp"
#include "Objects/SP500MkII.hpp"
#include "Objects/SparkRail.hpp"
#include "Objects/Spear.hpp"
#include "Objects/SpecialClear.hpp"
#include "Objects/SpecialRing.hpp"
#include "Objects/SpeedBooster.hpp"
#include "Objects/SpeedGate.hpp"
#include "Objects/SpiderMobile.hpp"
#include "Objects/SpikeCorridor.hpp"
#include "Objects/SpikeCrusher.hpp"
#include "Objects/SpikeFlail.hpp"
#include "Objects/SpikeLog.hpp"
#include "Objects/Spikes.hpp"
#include "Objects/SpinBooster.hpp"
#include "Objects/SpinSign.hpp"
#include "Objects/Spiny.hpp"
#include "Objects/SpiralPlatform.hpp"
#include "Objects/Splats.hpp"
#include "Objects/Spring.hpp"
#include "Objects/Springboard.hpp"
#include "Objects/SPZ1Intro.hpp"
#include "Objects/SPZ1Setup.hpp"
#include "Objects/SPZ2Outro.hpp"
#include "Objects/SPZ2Setup.hpp"
#include "Objects/SSZ1Intro.hpp"
#include "Objects/SSZ1Outro.hpp"
#include "Objects/SSZ1Setup.hpp"
#include "Objects/SSZ2Setup.hpp"
#include "Objects/SSZ3Cutscene.hpp"
#include "Objects/SSZEggman.hpp"
#include "Objects/SSZSpikeBall.hpp"
#include "Objects/SSZSpotlight.hpp"
#include "Objects/Staircase.hpp"
#include "Objects/Stalactite.hpp"
#include "Objects/StarPost.hpp"
#include "Objects/StickyPlatform.hpp"
#include "Objects/Summary.hpp"
#include "Objects/SummaryEmerald.hpp"
#include "Objects/SuperSparkle.hpp"
#include "Objects/Sweep.hpp"
#include "Objects/SwingRope.hpp"
#include "Objects/SwitchDoor.hpp"
#include "Objects/Syringe.hpp"
#include "Objects/TAEmerald.hpp"
#include "Objects/Technosqueek.hpp"
#include "Objects/TeeterTotter.hpp"
#include "Objects/TetherBall.hpp"
#include "Objects/ThanksSetup.hpp"
#include "Objects/ThoughtBubble.hpp"
#include "Objects/TilePlatform.hpp"
#include "Objects/TimeAttackData.hpp"
#include "Objects/TimeAttackGate.hpp"
#include "Objects/TimeAttackMenu.hpp"
#include "Objects/TimePost.hpp"
#include "Objects/TimeTravelSetup.hpp"
#include "Objects/TippingPlatform.hpp"
#include "Objects/Title3DSprite.hpp"
#include "Objects/TitleBG.hpp"
#include "Objects/TitleCard.hpp"
#include "Objects/TitleEggman.hpp"
#include "Objects/TitleLogo.hpp"
#include "Objects/TitleSetup.hpp"
#include "Objects/TitleSonic.hpp"
#include "Objects/TMZ1Outro.hpp"
#include "Objects/TMZ1Setup.hpp"
#include "Objects/TMZ2Outro.hpp"
#include "Objects/TMZ2Setup.hpp"
#include "Objects/TMZ3Setup.hpp"
#include "Objects/TMZAlert.hpp"
#include "Objects/TMZBarrier.hpp"
#include "Objects/TMZCable.hpp"
#include "Objects/TMZFlames.hpp"
#include "Objects/Tornado.hpp"
#include "Objects/TornadoPath.hpp"
#include "Objects/Toxomister.hpp"
#include "Objects/TransportTube.hpp"
#include "Objects/TryAgain.hpp"
#include "Objects/TryAgainE.hpp"
#include "Objects/TTCutscene.hpp"
#include "Objects/TubeSpring.hpp"
#include "Objects/Tubinaut.hpp"
#include "Objects/Tuesday.hpp"
#include "Objects/Turbine.hpp"
#include "Objects/TurboSpiker.hpp"
#include "Objects/TurboTurtle.hpp"
#include "Objects/Turntable.hpp"
#include "Objects/TurretSwitch.hpp"
#include "Objects/TVFlyingBattery.hpp"
#include "Objects/TVPole.hpp"
#include "Objects/TVVan.hpp"
#include "Objects/TwistedTubes.hpp"
#include "Objects/TwistingDoor.hpp"
#include "Objects/TwistingSlide.hpp"
#include "Objects/UberCaterkiller.hpp"
#include "Objects/UFO_Camera.hpp"
#include "Objects/UFO_Circuit.hpp"
#include "Objects/UFO_Decoration.hpp"
#include "Objects/UFO_Dust.hpp"
#include "Objects/UFO_HUD.hpp"
#include "Objects/UFO_ItemBox.hpp"
#include "Objects/UFO_Message.hpp"
#include "Objects/UFO_Plasma.hpp"
#include "Objects/UFO_Player.hpp"
#include "Objects/UFO_Ring.hpp"
#include "Objects/UFO_Setup.hpp"
#include "Objects/UFO_Shadow.hpp"
#include "Objects/UFO_SpeedLines.hpp"
#include "Objects/UFO_Sphere.hpp"
#include "Objects/UFO_Springboard.hpp"
#include "Objects/UFO_Water.hpp"
#include "Objects/UIBackground.hpp"
#include "Objects/UIButton.hpp"
#include "Objects/UIButtonLabel.hpp"
#include "Objects/UIButtonPrompt.hpp"
#include "Objects/UICarousel.hpp"
#include "Objects/UICharButton.hpp"
#include "Objects/UIChoice.hpp"
#include "Objects/UIControl.hpp"
#include "Objects/UICreditsText.hpp"
#include "Objects/UIDialog.hpp"
#include "Objects/UIDiorama.hpp"
#include "Objects/UIHeading.hpp"
#include "Objects/UIInfoLabel.hpp"
#include "Objects/UIKeyBinder.hpp"
#include "Objects/UILeaderboard.hpp"
#include "Objects/UIMedallionPanel.hpp"
#include "Objects/UIModeButton.hpp"
#include "Objects/UIOptionPanel.hpp"
#include "Objects/UIPicture.hpp"
#include "Objects/UIPopover.hpp"
#include "Objects/UIRankButton.hpp"
#include "Objects/UIReplayCarousel.hpp"
#include "Objects/UIResPicker.hpp"
#include "Objects/UISaveSlot.hpp"
#include "Objects/UIShifter.hpp"
#include "Objects/UISlider.hpp"
#include "Objects/UISubHeading.hpp"
#include "Objects/UITABanner.hpp"
#include "Objects/UITAZoneModule.hpp"
#include "Objects/UIText.hpp"
#include "Objects/UITransition.hpp"
#include "Objects/UIUsernamePopup.hpp"
#include "Objects/UIVideo.hpp"
#include "Objects/UIVsCharSelector.hpp"
#include "Objects/UIVsResults.hpp"
#include "Objects/UIVsRoundPicker.hpp"
#include "Objects/UIVsScoreboard.hpp"
#include "Objects/UIVsZoneButton.hpp"
#include "Objects/UIWaitSpinner.hpp"
#include "Objects/UIWidgets.hpp"
#include "Objects/UIWinSize.hpp"
#include "Objects/UncurlPlant.hpp"
#include "Objects/Valve.hpp"
#include "Objects/VanishPlatform.hpp"
#include "Objects/Vultron.hpp"
#include "Objects/WalkerLegs.hpp"
#include "Objects/WallBumper.hpp"
#include "Objects/WarpDoor.hpp"
#include "Objects/Water.hpp"
#include "Objects/WaterfallSound.hpp"
#include "Objects/WaterGush.hpp"
#include "Objects/WeatherMobile.hpp"
#include "Objects/WeatherTV.hpp"
#include "Objects/Whirlpool.hpp"
#include "Objects/WoodChipper.hpp"
#include "Objects/Woodrow.hpp"
#include "Objects/YoyoPulley.hpp"
#include "Objects/ZipLine.hpp"
#include "Objects/Zone.hpp"


#define RSDK_EDITABLE_VAR(object, type, var) RSDK.SetEditableVar(type, #var, object->objectID, offsetof(Entity##object, var))

extern "C" {
DLLExport void LinkGameLogicDLL(GameInfo *gameInfo);
}

#endif //! GAMEOBJECTS_H
