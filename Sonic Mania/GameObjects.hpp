#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

// Userdata Table
extern void *User_Unknown1;
extern void *User_GetConfirmButtonFlip;
extern void *User_Unknown2;
extern void *User_Unknown3;
extern void *User_Unknown4;
extern void *User_CheckDLC;
extern void *User_Unknown5;
extern void *User_UnlockAchievement;
extern void *User_Missing0;
extern void *User_Unknown6;
extern void *User_Unknown7;
extern void *User_Unknown8;
extern void *User_Unknown9;
extern void *User_Unknown10;
extern void *User_Unknown11;
extern void *User_Missing1;
extern void *User_Unknown12;
extern void *User_Missing2;
extern void *User_Unknown13;
extern void *User_PossiblyGetStrings;
extern void *User_Unknown14;
extern void *User_Missing3;
extern void *User_Unknown15;
extern void *User_Unknown16;
extern void *User_Unknown17;
extern void *User_Unknown18;
extern void *User_Unknown19;
extern void *User_Unknown20;
extern void *User_Unknown21;
extern void *User_Unknown22;
extern void *User_Unknown23;
extern void *User_Unknown24;
extern void *User_Missing4;
extern void *User_Unknown25;
extern void *User_Unknown26;
extern void *User_Unknown27;
extern void *User_Unknown28;
extern void *User_LoadUserFile;
extern void *User_SaveUserFile;
extern void *User_DeleteUserFile;
extern void *User_AddUserDBEntry;
extern int (*User_OpenUserDB)(void *, void *, void *);
extern void *User_SaveUserDB;
extern void *User_Unknown30;
extern void *User_Missing5;
extern void *User_Unknown31;
extern void *User_Unknown32;
extern void *User_Unknown33;
extern void *User_Unknown34;
extern void *User_Unknown35;
extern void *User_Unknown36;
extern void *User_Unknown37;
extern void *User_Unknown38;
extern void *User_Unknown39;
extern void *User_AddUserDB;
extern void *User_Unknown40;
extern void *User_Unknown41;
extern void *User_Unknown42;
extern void *User_Unknown43;

//Function Table
extern void (*RSDK_InitGameOptions)(void *options, int size);
extern void (*RSDK_CreateObject)(Object *structPtr, const char *name, uint entitySize, uint objectSize, void (*update)(void), void (*lateUpdate)(void),
                           void (*staticUpdate)(void), void (*draw)(void), void(__cdecl *create)(void *), void (*stageLoad)(void),
                           void (*editorDraw)(void), void (*editorLoad)(void), void (*serialize)(void));
extern void (*RSDK_CreateObjectContainer)(Object *structPtr, const char *name, uint objectSize);
extern bool32 (*RSDK_GetActiveObjects)(ushort group, Entity *entity);
extern bool32 (*RSDK_GetObjects)(ushort type, Entity *entity);
extern void (*RSDK_NextForEachLoop)(void);
extern void (*RSDK_SetEditableVar)(byte type, const char *name, byte object, int storeOffset);
extern int (*RSDK_GetObjectByID)(ushort objectID);
extern int (*RSDK_GetEntityID)(Entity *entityPtr);
extern int (*RSDK_GetEntityCount)(ushort type, bool32 isActive);
extern int (*RSDK_GetDrawListRef)(byte layerID, ushort entityID);
extern Entity *(*RSDK_GetDrawListRefPtr)(byte layerID, ushort entityID);
extern int (*RSDK_DestroyEntity)(Entity *entity, ushort type, void *data);
extern int (*RSDK_ResetEntity)(ushort slotID, ushort type, void *data);
extern int (*RSDK_SpawnEntity)(ushort type, void *data, int x, int y);
extern void (*RSDK_CopyEntity)(Entity *destEntity, Entity *srcEntity, bool32 clearSrcEntity);
extern void *RSDK_CheckOnScreen;
extern void *RSDK_CheckPosOnScreen;
extern void *RSDK_AddDrawListRef;
extern void *RSDK_SwapDrawLayers;
extern int (*RSDK_SetDrawLayerProperties)(void *, void *, void *);
extern int (*RSDK_LoadScene)(void *, void *);
extern void *RSDK_SetGameMode;
extern int (*RSDK_Unknown53)(void *);
extern void *RSDK_CheckValidScene;
extern int (*RSDK_CheckStageFolder)(char *folderName);
extern int (*RSDK_InitSceneLoad)(void);
extern void *RSDK_GetObjectIDByName;
extern void *RSDK_Unknown57;
extern int (*RSDK_Unknown58)(void *, void *, void *, void *);
extern void *RSDK_GetSettingsValue;
extern void *RSDK_SetSettingsValue;
extern void *RSDK_ResizeWindow;
extern int (*RSDK_Sin1024)(int angle);
extern int (*RSDK_Cos1024)(int angle);
extern int (*RSDK_ATan1024)(int angle);
extern int (*RSDK_ASin1024)(int angle);
extern int (*RSDK_ACos1024)(int angle);
extern int (*RSDK_Sin512)(int angle);
extern int (*RSDK_Cos512)(int angle);
extern int (*RSDK_ATan512)(int angle);
extern int (*RSDK_ASin512)(int angle);
extern int (*RSDK_ACos512)(int angle);
extern int (*RSDK_Sin256)(int angle);
extern int (*RSDK_Cos256)(int angle);
extern int (*RSDK_ATan256)(int angle);
extern int (*RSDK_ASin256)(int angle);
extern int (*RSDK_ACos256)(int angle);
extern int (*RSDK_Rand)(int min, int max);
extern int (*RSDK_Random)(int min, int max, int *randKey);
extern void *RSDK_SetRandKey;
extern byte (*RSDK_ATan2)(int x, int y);
extern void *RSDK_SetIdentityMatrix;
extern void *RSDK_MatrixMultiply;
extern void *RSDK_MatrixTranslateXYZ;
extern void *RSDK_MatrixScaleXYZ;
extern void *RSDK_MatrixRotateX;
extern void *RSDK_MatrixRotateY;
extern void *RSDK_MatrixRotateZ;
extern void *RSDK_MatrixRotateXYZ;
extern void *RSDK_MatrixInverse;
extern void *RSDK_MatrixCopy;
extern void *RSDK_SetText;
extern void *RSDK_Unknown64;
extern void *RSDK_Unknown65;
extern void *RSDK_Unknown66;
extern void *RSDK_Unknown67;
extern void *RSDK_LoadStrings;
extern void *RSDK_Unknown68;
extern void *RSDK_CopyString;
extern void *RSDK_Unknown69;
extern void *RSDK_Unknown70;
extern void *RSDK_Unknown71;
extern int (*RSDK_SetScreenSize)(void *, void *, void *, void *, void *);
extern void (*RSDK_SetClipBounds)(byte screenID, int x1, int y1, int x2, int y2);
extern void *RSDK_ScreenUnknown;
extern short (*RSDK_LoadSpriteSheet)(const char *path, Scopes scope);
extern void (*RSDK_SetInkValue)(uint value);
extern void (*RSDK_SetPaletteMask)(uint colour);
extern void (*RSDK_SetPaletteEntry)(byte paletteID, byte index, uint colour);
extern uint (*RSDK_GetPaletteEntry)(byte paletteID, byte index);
extern void (*RSDK_SetActivePalette)(byte newActivePal, int startLine, int endLine);
extern void (*RSDK_CopyPalette)(byte sourcePalette, byte srcPaletteStart, byte destinationPalette, byte destPaletteStart, ushort count);
extern void (*RSDK_LoadPalette)(const char *filePath, int paletteID, int startPaletteIndex, int startIndex, int endIndex);
extern void (*RSDK_RotatePalette)(byte palID, byte startIndex, byte endIndex, bool right);
extern void (*RSDK_SetLimitedFade)(byte destPaletteID, byte srcPaletteA, byte srcPaletteB, ushort blendAmount, int startIndex, int endIndex);
extern void *RSDK_DoPaletteSomething3;
extern void *RSDK_DrawRect;
extern void *RSDK_DrawLine;
extern void *RSDK_DrawCircle;
extern void *RSDK_DrawCircleOutline;
extern void *RSDK_DrawQuad;
extern void *RSDK_DrawTexturedQuad;
extern int (*RSDK_DrawSprite)(void *, void *, void *);
extern int (*RSDK_DrawUnknown)(void *, void *, void *);
extern void *RSDK_DrawText;
extern void *RSDK_DrawSprite3;
extern void *RSDK_DrawTile;
extern void *RSDK_DrawSheetFrame;
extern int (*RSDK_SetScreenFade)(int a, int r, int g, int b);
extern void *RSDK_LoadMesh;
extern void *RSDK_Create3DScene;
extern void *RSDK_Init3DScene;
extern void *RSDK_View_Something1;
extern void *RSDK_View_Something2;
extern void *RSDK_View_Something3;
extern void *RSDK_SetupMesh;
extern void (*RSDK_SetModelAnimation)(ushort modelAnim, EntityAnimationData *data, short animSpeed, byte loopIndex, bool forceApply, ushort frameID);
extern void *RSDK_SetupMeshAnimation;
extern void *RSDK_Draw3DScene;
extern short (*RSDK_LoadAnimation)(const char *path, Scopes scope);
extern short (*RSDK_AnimationDoSomething)();
extern void (*RSDK_SetSpriteAnimation)(ushort spriteIndex, ushort animationID, EntityAnimationData *data, bool forceApply, ushort frameID);
extern void *RSDK_DoSpriteAnimationSomething;
extern void *RSDK_SetSpriteString;
extern void *RSDK_DoAnimationListSomething;
extern SpriteFrame *(*RSDK_GetFrame)(ushort sprIndex, ushort anim, int frame);
extern Hitbox *(*RSDK_GetHitbox)(EntityAnimationData *data, byte hitboxID);
extern short (*RSDK_GetFrameID)(EntityAnimationData *data);
extern void *RSDK_Unknown82;
extern void (*RSDK_ProcessAnimation)(EntityAnimationData *data);
extern int (*RSDK_GetSceneLayerID)(void *);
extern void *(*RSDK_GetSceneLayerPtr)(void *);
extern void *RSDK_GetLayerSize;
extern void *RSDK_GetTileInfo;
extern void *RSDK_SetTileInfo;
extern int (*RSDK_CopyTileLayer)(void *, void *, void *, void *, void *, void *, void *, void *);
extern void *RSDK_ProcessParallax;
extern int (*RSDK_GetLinePosPtrs)(void);
extern void *RSDK_CheckObjectCollisionTouchBox;
extern void *RSDK_CheckObjectCollisionTouchCircle;
extern void *RSDK_CheckObjectCollisionBox;
extern void *RSDK_CheckObjectCollisionPlatform;
extern void *RSDK_ObjectTileCollision;
extern void *RSDK_ObjectTileGrip;
extern void *RSDK_ProcessPlayerTileCollisions;
extern void *RSDK_GetTileAngle;
extern void *RSDK_SetTileAngle;
extern void *RSDK_GetTileBehaviour;
extern void *RSDK_SetTileBehaviour;
extern int (*RSDK_GetSFX)(const char *path);
extern int (*RSDK_PlaySFX)(void *, void *, void *);
extern void *RSDK_StopSFX;
extern void *RSDK_PlayMusic;
extern int (*RSDK_SetSoundAttributes)(void *, void *, void *, void *);
extern void *RSDK_Unknown91;
extern void *RSDK_Unknown92;
extern void *RSDK_Unknown93;
extern void *RSDK_Unknown94;
extern void *RSDK_Unknown95;
extern void *RSDK_Unknown96;
extern void *RSDK_LoadVideo;
extern void *RSDK_LoadPNG;
extern void *RSDK_Unknown98;
extern void *RSDK_Unknown99;
extern void *RSDK_Unknown100;
extern void *RSDK_Unknown101;
extern void *RSDK_Missing21;
extern void *RSDK_Missing22;
extern void *RSDK_Missing23;
extern void *RSDK_Missing24;
extern void *RSDK_Missing25;
extern void *RSDK_Missing26;
extern void *RSDK_Unknown102;
extern void *RSDK_Unknown103;
extern void *RSDK_Unknown104;
extern void *RSDK_Missing27;
extern void *RSDK_Missing28;
extern void (*RSDK_printLog)(SeverityModes severity, const char *message, ...);
extern void (*RSDK_printString)(SeverityModes severity, const char *message);
extern void (*RSDK_printText)(SeverityModes severity, const char *message, TextInfo *text);
extern void (*RSDK_printIntegerUnsigned)(SeverityModes severity, const char *message, uint integer);
extern void (*RSDK_printInteger)(SeverityModes severity, const char *message, int integer);
extern void (*RSDK_printFloat)(SeverityModes severity, const char *message, float f);
extern void (*RSDK_printVector2)(SeverityModes severity, const char *message, int x, int y);
extern void (*RSDK_printHitbox)(SeverityModes severity, const char *message, Hitbox *hitbox);
extern void (*RSDK_Unknown105)();
extern void (*RSDK_Unknown106)();
extern void (*RSDK_ClearDebugValues)();
extern void (*RSDK_SetDebugValue)(const char *name, int valPtr, int type, int unknown1, int unknown2);

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
#include "Objects/OptionsMenu.hpp"
#include "Objects/OrbitSpike.hpp"
#include "Objects/PaintingEyes.hpp"
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

extern "C" {
DLLExport void LinkGameLogicDLL(GameInfo *gameInfo);
}


#endif //! GAMEOBJECTS_H
