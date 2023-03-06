#include "Game.h"

// -------------------------
// ENGINE VARIABLES
// -------------------------

RSDKFunctionTable RSDK;
#if MANIA_USE_PLUS
APIFunctionTable API;
#endif
#if RETRO_USE_MOD_LOADER
ModFunctionTable Mod;

DLLExport ModVersionInfo modInfo = { RETRO_REVISION, GAME_VERSION, RETRO_MOD_LOADER_VER };

const char *modID = "SonicMania";
#endif

int32 RSDKRevision = RETRO_REVISION;

RSDKSceneInfo *SceneInfo = NULL;

RSDKGameInfo *GameInfo = NULL;
#if MANIA_USE_PLUS
RSDKSKUInfo *SKU = NULL;
#endif

RSDKControllerState *ControllerInfo = NULL;
RSDKAnalogState *AnalogStickInfoL   = NULL; // should be called "AnalogStickInfo" for Pre-Plus but its easier to be consistent this way
#if MANIA_USE_PLUS
RSDKAnalogState *AnalogStickInfoR = NULL;
RSDKTriggerState *TriggerInfoL    = NULL;
RSDKTriggerState *TriggerInfoR    = NULL;
#endif
RSDKTouchInfo *TouchInfo = NULL;

#if MANIA_USE_PLUS
RSDKUnknownInfo *UnknownInfo = NULL;
#endif

RSDKScreenInfo *ScreenInfo = NULL;

// -------------------------
// GAME VARIABLES
// -------------------------

GlobalVariables *globals;

#if RETRO_REV0U
void GlobalVariables_InitCB(GlobalVariables *globals)
{
    memset(globals, 0, sizeof(GlobalVariables));

    globals->saveSlotID = NO_SAVE_SLOT;

    globals->presenceID = -1;

#if MANIA_USE_PLUS
    globals->replayTableID = (uint16)-1;
    globals->taTableID     = (uint16)-1;

    globals->stock          = (ID_RAY << 16) | (ID_KNUCKLES << 8) | ID_TAILS;
    globals->characterFlags = ID_SONIC | ID_TAILS | ID_KNUCKLES | ID_MIGHTY | ID_RAY;

    globals->superMusicEnabled = true;
#endif
}
#endif

void InitGameLogic(void);
#if RETRO_USE_MOD_LOADER
void InitModAPI(void);
#endif

#include "GameMain.h"

#if MANIA_USE_PLUS
void LinkGameLogicDLL(EngineInfo *info)
{
    memset(&API, 0, sizeof(APIFunctionTable));
    memset(&RSDK, 0, sizeof(RSDKFunctionTable));

    if (info->functionTable)
        memcpy(&RSDK, info->functionTable, sizeof(RSDKFunctionTable));

    if (info->APITable)
        memcpy(&API, info->APITable, sizeof(APIFunctionTable));

#if RETRO_USE_MOD_LOADER
    if (info->modTable)
        memcpy(&Mod, info->modTable, sizeof(ModFunctionTable));
#endif

    GameInfo       = info->gameInfo;
    SKU            = info->currentSKU;
    SceneInfo      = info->sceneInfo;
    ControllerInfo = info->controllerInfo;

    AnalogStickInfoL = info->stickInfoL;
    AnalogStickInfoR = info->stickInfoR;
    TriggerInfoL     = info->triggerInfoL;
    TriggerInfoR     = info->triggerInfoR;
    TouchInfo        = info->touchInfo;

    UnknownInfo = info->unknownInfo;
    ScreenInfo  = info->screenInfo;

    InitGameLogic();
}
#else
void LinkGameLogicDLL(EngineInfo info)
{
    // Actual params are:
    // void LinkGameLogicDLL(void *functionTable, RSDKGameInfo *gameInfo, RSDKSceneInfo *sceneInfo, RSDKControllerState *controllerInfo,
    //                       RSDKAnalogState *stickInfoL, RSDKTouchInfo *touchInfo, RSDKScreenInfo *screenInfo)
    // But by passing EngineInfo by value we can create a sorta "container" for it, to keep things consistent & easy to manage

    memset(&RSDK, 0, sizeof(RSDKFunctionTable));

    if (info.functionTable)
        memcpy(&RSDK, info.functionTable, sizeof(RSDKFunctionTable));

#if RETRO_USE_MOD_LOADER
    if (info.modTable)
        memcpy(&Mod, info.modTable, sizeof(ModFunctionTable));
#endif

    GameInfo         = info.gameInfo;
    SceneInfo        = info.sceneInfo;
    ControllerInfo   = info.controllerInfo;
    AnalogStickInfoL = info.stickInfoL;
    TouchInfo        = info.touchInfo;
    ScreenInfo       = info.screenInfo;

    InitGameLogic();
}
#endif

// This is actually part of "LinkGameLogicDLL" but since we have 2 versions of it, its easier to use shared code this way
void InitGameLogic(void)
{
#if RETRO_REV0U
    RSDK.RegisterGlobalVariables((void **)&globals, sizeof(GlobalVariables), (void(*)(void*))GlobalVariables_InitCB);
#else
    RSDK.RegisterGlobalVariables((void **)&globals, sizeof(GlobalVariables));
#endif

    RSDK_REGISTER_OBJECT(Acetone);
    RSDK_REGISTER_OBJECT(ActClear);
    RSDK_REGISTER_OBJECT(AIZEggRobo);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(AIZEncoreTutorial);
#endif
    RSDK_REGISTER_OBJECT(AIZKingClaw);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(AIZRockPile);
#endif
    RSDK_REGISTER_OBJECT(AIZSetup);
    RSDK_REGISTER_OBJECT(AIZTornado);
    RSDK_REGISTER_OBJECT(AIZTornadoPath);
    RSDK_REGISTER_OBJECT(AmoebaDroid);
    RSDK_REGISTER_OBJECT(AnimalHBH);
    RSDK_REGISTER_OBJECT(Animals);
    RSDK_REGISTER_OBJECT(Announcer);
#if !MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(APICallback);
#endif
    RSDK_REGISTER_OBJECT(Aquis);
    RSDK_REGISTER_OBJECT(Armadiloid);
    RSDK_REGISTER_OBJECT(BadnikHelpers);
    RSDK_REGISTER_OBJECT(Ball);
    RSDK_REGISTER_OBJECT(BallCannon);
    RSDK_REGISTER_OBJECT(BallHog);
    RSDK_REGISTER_OBJECT(BarStool);
    RSDK_REGISTER_OBJECT(Batbot);
    RSDK_REGISTER_OBJECT(Batbrain);
    RSDK_REGISTER_OBJECT(Beanstalk);
    RSDK_REGISTER_OBJECT(BGSwitch);
    RSDK_REGISTER_OBJECT(BigSqueeze);
    RSDK_REGISTER_OBJECT(BladePole);
    RSDK_REGISTER_OBJECT(Blaster);
    RSDK_REGISTER_OBJECT(Blastoid);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(Bloominator);
#endif
    RSDK_REGISTER_OBJECT(Bomb);
    RSDK_REGISTER_OBJECT(BouncePlant);
    RSDK_REGISTER_OBJECT(BoundsMarker);
    RSDK_REGISTER_OBJECT(BreakableWall);
    RSDK_REGISTER_OBJECT(BreakBar);
    RSDK_REGISTER_OBJECT(Bridge);
    RSDK_REGISTER_OBJECT(BSS_Collectable);
    RSDK_REGISTER_OBJECT(BSS_Collected);
    RSDK_REGISTER_OBJECT(BSS_Horizon);
    RSDK_REGISTER_OBJECT(BSS_HUD);
    RSDK_REGISTER_OBJECT(BSS_Message);
    RSDK_REGISTER_OBJECT(BSS_Palette);
    RSDK_REGISTER_OBJECT(BSS_Player);
    RSDK_REGISTER_OBJECT(BSS_Setup);
    RSDK_REGISTER_OBJECT(Bubbler);
    RSDK_REGISTER_OBJECT(BuckwildBall);
    RSDK_REGISTER_OBJECT(Buggernaut);
    RSDK_REGISTER_OBJECT(Bumpalo);
    RSDK_REGISTER_OBJECT(Bungee);
    RSDK_REGISTER_OBJECT(BurningLog);
    RSDK_REGISTER_OBJECT(Button);
    RSDK_REGISTER_OBJECT(ButtonDoor);
    RSDK_REGISTER_OBJECT(BuzzBomber);
    RSDK_REGISTER_OBJECT(BuzzSaw);
    RSDK_REGISTER_OBJECT(CableWarp);
    RSDK_REGISTER_OBJECT(Cactula);
    RSDK_REGISTER_OBJECT(Camera);
    RSDK_REGISTER_OBJECT(Canista);
    RSDK_REGISTER_OBJECT(Caterkiller);
    RSDK_REGISTER_OBJECT(CaterkillerJr);
    RSDK_REGISTER_OBJECT(ChaosEmerald);
    RSDK_REGISTER_OBJECT(CheckerBall);
    RSDK_REGISTER_OBJECT(ChemBubble);
    RSDK_REGISTER_OBJECT(ChemicalBall);
    RSDK_REGISTER_OBJECT(ChemicalPool);
    RSDK_REGISTER_OBJECT(Chopper);
    RSDK_REGISTER_OBJECT(CircleBumper);
    RSDK_REGISTER_OBJECT(Clapperboard);
    RSDK_REGISTER_OBJECT(Clucker);
    RSDK_REGISTER_OBJECT(CollapsingPlatform);
    RSDK_REGISTER_OBJECT(CollapsingSand);
    RSDK_REGISTER_OBJECT(ColorHelpers);
    RSDK_REGISTER_OBJECT(Competition);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(CompetitionMenu);
#endif
    RSDK_REGISTER_OBJECT(CompetitionSession);
    RSDK_REGISTER_OBJECT(Constellation);
    RSDK_REGISTER_OBJECT(ContinuePlayer);
    RSDK_REGISTER_OBJECT(ContinueSetup);
    RSDK_REGISTER_OBJECT(ConveyorBelt);
    RSDK_REGISTER_OBJECT(ConveyorPlatform);
    RSDK_REGISTER_OBJECT(ConveyorWheel);
    RSDK_REGISTER_OBJECT(CorkscrewPath);
    RSDK_REGISTER_OBJECT(COverlay);
    RSDK_REGISTER_OBJECT(CPZ1Intro);
    RSDK_REGISTER_OBJECT(CPZ2Outro);
    RSDK_REGISTER_OBJECT(CPZBoss);
    RSDK_REGISTER_OBJECT(CPZSetup);
    RSDK_REGISTER_OBJECT(CPZShutter);
    RSDK_REGISTER_OBJECT(Crabmeat);
    RSDK_REGISTER_OBJECT(Crane);
    RSDK_REGISTER_OBJECT(CrashTest);
    RSDK_REGISTER_OBJECT(Crate);
    RSDK_REGISTER_OBJECT(CreditsSetup);
    RSDK_REGISTER_OBJECT(CrimsonEye);
    RSDK_REGISTER_OBJECT(Current);
    RSDK_REGISTER_OBJECT(CutsceneHBH);
    RSDK_REGISTER_OBJECT(CutsceneRules);
    RSDK_REGISTER_OBJECT(CutsceneSeq);
    RSDK_REGISTER_OBJECT(Cylinder);
    RSDK_REGISTER_OBJECT(DAControl);
    RSDK_REGISTER_OBJECT(Dango);
    RSDK_REGISTER_OBJECT(DASetup);
    RSDK_REGISTER_OBJECT(DashLift);
    RSDK_REGISTER_OBJECT(DBTower);
    RSDK_REGISTER_OBJECT(DCEvent);
    RSDK_REGISTER_OBJECT(DDWrecker);
    RSDK_REGISTER_OBJECT(Debris);
    RSDK_REGISTER_OBJECT(DebugMode);
    RSDK_REGISTER_OBJECT(Decoration);
    RSDK_REGISTER_OBJECT(DemoMenu);
    RSDK_REGISTER_OBJECT(DERobot);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(DialogRunner);
#endif
    RSDK_REGISTER_OBJECT(DirectorChair);
    RSDK_REGISTER_OBJECT(DiveEggman);
    RSDK_REGISTER_OBJECT(DNARiser);
    RSDK_REGISTER_OBJECT(DoorTrigger);
    RSDK_REGISTER_OBJECT(Dragonfly);
    RSDK_REGISTER_OBJECT(DrawHelpers);
    RSDK_REGISTER_OBJECT(Drillerdroid);
    RSDK_REGISTER_OBJECT(DrillerdroidO);
    RSDK_REGISTER_OBJECT(Dust);
#if !MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(E3MenuSetup);
#endif
    RSDK_REGISTER_OBJECT(EggJanken);
    RSDK_REGISTER_OBJECT(EggJankenPart);
    RSDK_REGISTER_OBJECT(EggLoco);
    RSDK_REGISTER_OBJECT(Eggman);
    RSDK_REGISTER_OBJECT(EggPistonsMKII);
    RSDK_REGISTER_OBJECT(EggPrison);
    RSDK_REGISTER_OBJECT(EggTower);
    RSDK_REGISTER_OBJECT(EggTV);
    RSDK_REGISTER_OBJECT(ElectroMagnet);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(EncoreGoodEnd);
    RSDK_REGISTER_OBJECT(EncoreIntro);
    RSDK_REGISTER_OBJECT(EncoreRoute);
#endif
    RSDK_REGISTER_OBJECT(ERZGunner);
    RSDK_REGISTER_OBJECT(ERZKing);
    RSDK_REGISTER_OBJECT(ERZMystic);
    RSDK_REGISTER_OBJECT(ERZOutro);
    RSDK_REGISTER_OBJECT(ERZRider);
    RSDK_REGISTER_OBJECT(ERZSetup);
    RSDK_REGISTER_OBJECT(ERZShinobi);
    RSDK_REGISTER_OBJECT(ERZStart);
    RSDK_REGISTER_OBJECT(EscapeCar);
    RSDK_REGISTER_OBJECT(Explosion);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(ExtrasMenu);
#endif
    RSDK_REGISTER_OBJECT(Fan);
    RSDK_REGISTER_OBJECT(FarPlane);
    RSDK_REGISTER_OBJECT(FBZ1Outro);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(FBZ2Outro);
#endif
    RSDK_REGISTER_OBJECT(FBZFan);
    RSDK_REGISTER_OBJECT(FBZMissile);
    RSDK_REGISTER_OBJECT(FBZSetup);
    RSDK_REGISTER_OBJECT(FBZSinkTrash);
    RSDK_REGISTER_OBJECT(FBZStorm);
    RSDK_REGISTER_OBJECT(FBZTrash);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(FernParallax);
#endif
    RSDK_REGISTER_OBJECT(FilmProjector);
    RSDK_REGISTER_OBJECT(FilmReel);
    RSDK_REGISTER_OBJECT(Fireball);
    RSDK_REGISTER_OBJECT(Fireflies);
    RSDK_REGISTER_OBJECT(Firework);
    RSDK_REGISTER_OBJECT(Fireworm);
    RSDK_REGISTER_OBJECT(FlameSpring);
    RSDK_REGISTER_OBJECT(Flamethrower);
    RSDK_REGISTER_OBJECT(FlasherMKII);
    RSDK_REGISTER_OBJECT(FlingRamp);
    RSDK_REGISTER_OBJECT(Flipper);
    RSDK_REGISTER_OBJECT(FlowerPod);
    RSDK_REGISTER_OBJECT(FoldingPlatform);
    RSDK_REGISTER_OBJECT(ForceSpin);
    RSDK_REGISTER_OBJECT(ForceUnstick);
    RSDK_REGISTER_OBJECT(FrostThrower);
    RSDK_REGISTER_OBJECT(Funnel);
    RSDK_REGISTER_OBJECT(FXExpandRing);
    RSDK_REGISTER_OBJECT(FXFade);
    RSDK_REGISTER_OBJECT(FXRuby);
    RSDK_REGISTER_OBJECT(FXSpinRay);
    RSDK_REGISTER_OBJECT(FXTrail);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(FXWaveRing);
#endif
    RSDK_REGISTER_OBJECT(Gachapandora);
    RSDK_REGISTER_OBJECT(GameOver);
#if MANIA_USE_PLUS
    RSDK_REGISTER_STATIC_VARIABLES(GameProgress);
#else
    RSDK_REGISTER_OBJECT(GameProgress);
#endif
    RSDK_REGISTER_OBJECT(GasPlatform);
    RSDK_REGISTER_OBJECT(GenericTrigger);
    RSDK_REGISTER_OBJECT(GHZ2Outro);
    RSDK_REGISTER_OBJECT(GHZCutsceneK);
    RSDK_REGISTER_OBJECT(GHZCutsceneST);
    RSDK_REGISTER_OBJECT(GHZSetup);
    RSDK_REGISTER_OBJECT(GiantPistol);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(GigaMetal);
#endif
    RSDK_REGISTER_OBJECT(Gondola);
    RSDK_REGISTER_OBJECT(Grabber);
    RSDK_REGISTER_OBJECT(GreenScreen);
    RSDK_REGISTER_OBJECT(GymBar);
    RSDK_REGISTER_OBJECT(HandLauncher);
    RSDK_REGISTER_OBJECT(HangConveyor);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(HangGlider);
#endif
    RSDK_REGISTER_OBJECT(HangPoint);
    RSDK_REGISTER_OBJECT(Hatch);
    RSDK_REGISTER_OBJECT(Hatterkiller);
    RSDK_REGISTER_OBJECT(HCZ1Intro);
    RSDK_REGISTER_OBJECT(HCZOneWayDoor);
    RSDK_REGISTER_OBJECT(HCZSetup);
    RSDK_REGISTER_OBJECT(HCZSpikeBall);
    RSDK_REGISTER_OBJECT(HeavyGunner);
    RSDK_REGISTER_OBJECT(HeavyKing);
    RSDK_REGISTER_OBJECT(HeavyMystic);
    RSDK_REGISTER_OBJECT(HeavyRider);
    RSDK_REGISTER_OBJECT(HeavyShinobi);
    RSDK_REGISTER_OBJECT(HiLoSign);
    RSDK_REGISTER_OBJECT(Honkytonk);
    RSDK_REGISTER_OBJECT(Hotaru);
    RSDK_REGISTER_OBJECT(HotaruHiWatt);
    RSDK_REGISTER_OBJECT(HotaruMKII);
    RSDK_REGISTER_OBJECT(HPZEmerald);
    RSDK_REGISTER_OBJECT(HUD);
    RSDK_REGISTER_OBJECT(Ice);
    RSDK_REGISTER_OBJECT(IceBomba);
    RSDK_REGISTER_OBJECT(IceSpring);
    RSDK_REGISTER_OBJECT(ImageTrail);
    RSDK_REGISTER_OBJECT(Ink);
    RSDK_REGISTER_OBJECT(InkWipe);
    RSDK_REGISTER_OBJECT(InvincibleStars);
    RSDK_REGISTER_OBJECT(InvisibleBlock);
    RSDK_REGISTER_OBJECT(ItemBox);
    RSDK_REGISTER_OBJECT(Iwamodoki);
    RSDK_REGISTER_OBJECT(JacobsLadder);
    RSDK_REGISTER_OBJECT(Jawz);
    RSDK_REGISTER_OBJECT(Jellygnite);
    RSDK_REGISTER_OBJECT(JuggleSaw);
    RSDK_REGISTER_OBJECT(JunctionWheel);
    RSDK_REGISTER_OBJECT(Kabasira);
    RSDK_REGISTER_OBJECT(Kanabun);
    RSDK_REGISTER_OBJECT(KingAttack);
    RSDK_REGISTER_OBJECT(KingClaw);
    RSDK_REGISTER_OBJECT(KleptoMobile);
    RSDK_REGISTER_OBJECT(LargeGear);
    RSDK_REGISTER_OBJECT(Launcher);
    RSDK_REGISTER_OBJECT(LaunchSpring);
    RSDK_REGISTER_OBJECT(LaundroMobile);
    RSDK_REGISTER_OBJECT(LavaFall);
    RSDK_REGISTER_OBJECT(LavaGeyser);
    RSDK_REGISTER_OBJECT(LEDPanel);
    RSDK_REGISTER_OBJECT(Letterboard);
    RSDK_REGISTER_OBJECT(LevelSelect);
    RSDK_REGISTER_OBJECT(LightBarrier);
    RSDK_REGISTER_OBJECT(LightBulb);
    RSDK_REGISTER_OBJECT(Localization);
    RSDK_REGISTER_OBJECT(LocoSmoke);
    RSDK_REGISTER_OBJECT(LogHelpers);
    RSDK_REGISTER_OBJECT(LogoSetup);
    RSDK_REGISTER_OBJECT(LottoBall);
    RSDK_REGISTER_OBJECT(LottoMachine);
    RSDK_REGISTER_OBJECT(LoveTester);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(LRZ1Intro);
#endif
    RSDK_REGISTER_OBJECT(LRZ1Outro);
    RSDK_REGISTER_OBJECT(LRZ1Setup);
    RSDK_REGISTER_OBJECT(LRZ2Setup);
    RSDK_REGISTER_OBJECT(LRZ3Cutscene);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(LRZ3Outro);
#endif
    RSDK_REGISTER_OBJECT(LRZ3OutroK);
    RSDK_REGISTER_OBJECT(LRZ3Setup);
    RSDK_REGISTER_OBJECT(LRZConvControl);
    RSDK_REGISTER_OBJECT(LRZConvDropper);
    RSDK_REGISTER_OBJECT(LRZConveyor);
    RSDK_REGISTER_OBJECT(LRZConvItem);
    RSDK_REGISTER_OBJECT(LRZConvSwitch);
    RSDK_REGISTER_OBJECT(LRZFireball);
    RSDK_REGISTER_OBJECT(LRZRockPile);
    RSDK_REGISTER_OBJECT(LRZSpikeBall);
    RSDK_REGISTER_OBJECT(LRZSpiral);
    RSDK_REGISTER_OBJECT(MagnetSphere);
    RSDK_REGISTER_OBJECT(MagPlatform);
    RSDK_REGISTER_OBJECT(MagSpikeBall);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(MainMenu);
    RSDK_REGISTER_OBJECT(ManiaModeMenu);
#endif
    RSDK_REGISTER_OBJECT(MathHelpers);
    RSDK_REGISTER_OBJECT(MatryoshkaBom);
    RSDK_REGISTER_OBJECT(MechaBu);
    RSDK_REGISTER_OBJECT(MegaChopper);
    RSDK_REGISTER_OBJECT(MegaOctus);
    RSDK_REGISTER_OBJECT(MenuParam);
    RSDK_REGISTER_OBJECT(MenuSetup);
    RSDK_REGISTER_OBJECT(MetalArm);
    RSDK_REGISTER_OBJECT(MetalSonic);
    RSDK_REGISTER_OBJECT(MeterDroid);
    RSDK_REGISTER_OBJECT(MicDrop);
    RSDK_REGISTER_OBJECT(Mine);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(MMZ2Outro);
    RSDK_REGISTER_OBJECT(MMZLightning);
#endif
    RSDK_REGISTER_OBJECT(MMZSetup);
    RSDK_REGISTER_OBJECT(MMZWheel);
    RSDK_REGISTER_OBJECT(MonarchBG);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(MonarchPlans);
#endif
    RSDK_REGISTER_OBJECT(MonkeyDude);
    RSDK_REGISTER_OBJECT(Motobug);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(MSBomb);
#endif
    RSDK_REGISTER_OBJECT(MSFactory);
    RSDK_REGISTER_OBJECT(MSHologram);
    RSDK_REGISTER_OBJECT(MSOrb);
    RSDK_REGISTER_OBJECT(MSPanel);
    RSDK_REGISTER_OBJECT(MSZ1KIntro);
    RSDK_REGISTER_OBJECT(MSZ2Cutscene);
    RSDK_REGISTER_OBJECT(MSZCutsceneK);
    RSDK_REGISTER_OBJECT(MSZCutsceneST);
    RSDK_REGISTER_OBJECT(MSZSetup);
    RSDK_REGISTER_OBJECT(MSZSpotlight);
    RSDK_REGISTER_OBJECT(Music);
    RSDK_REGISTER_OBJECT(Newspaper);
    RSDK_REGISTER_OBJECT(Newtron);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(NoSwap);
#endif
    RSDK_REGISTER_OBJECT(Octus);
    RSDK_REGISTER_OBJECT(OneWayDoor);
    RSDK_REGISTER_OBJECT(OOZ1Outro);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(OOZ2Outro);
#endif
    RSDK_REGISTER_OBJECT(OOZFlames);
    RSDK_REGISTER_OBJECT(OOZSetup);
#if MANIA_USE_PLUS
    RSDK_REGISTER_STATIC_VARIABLES(Options);
    RSDK_REGISTER_OBJECT(OptionsMenu);
#else
    RSDK_REGISTER_OBJECT(Options);
#endif
    RSDK_REGISTER_OBJECT(Orbinaut);
    RSDK_REGISTER_OBJECT(OrbitSpike);
    RSDK_REGISTER_OBJECT(PaintingEyes);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(Palette);
#endif
    RSDK_REGISTER_OBJECT(PaperRoller);
    RSDK_REGISTER_OBJECT(ParallaxSprite);
    RSDK_REGISTER_OBJECT(ParticleHelpers);
    RSDK_REGISTER_OBJECT(PathInverter);
    RSDK_REGISTER_OBJECT(PauseMenu);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(PBL_Bumper);
    RSDK_REGISTER_OBJECT(PBL_Camera);
    RSDK_REGISTER_OBJECT(PBL_Crane);
    RSDK_REGISTER_OBJECT(PBL_Flipper);
    RSDK_REGISTER_OBJECT(PBL_HUD);
    RSDK_REGISTER_OBJECT(PBL_Player);
    RSDK_REGISTER_OBJECT(PBL_Ring);
    RSDK_REGISTER_OBJECT(PBL_Sector);
    RSDK_REGISTER_OBJECT(PBL_Setup);
    RSDK_REGISTER_OBJECT(PBL_TargetBumper);
#endif
    RSDK_REGISTER_OBJECT(Pendulum);
    RSDK_REGISTER_OBJECT(PetalPile);
    RSDK_REGISTER_OBJECT(PhantomEgg);
    RSDK_REGISTER_OBJECT(PhantomGunner);
    RSDK_REGISTER_OBJECT(PhantomHand);
    RSDK_REGISTER_OBJECT(PhantomKing);
    RSDK_REGISTER_OBJECT(PhantomMissile);
    RSDK_REGISTER_OBJECT(PhantomMystic);
    RSDK_REGISTER_OBJECT(PhantomRider);
    RSDK_REGISTER_OBJECT(PhantomRuby);
    RSDK_REGISTER_OBJECT(PhantomShield);
    RSDK_REGISTER_OBJECT(PhantomShinobi);
    RSDK_REGISTER_OBJECT(PimPom);
    RSDK_REGISTER_OBJECT(Pinata);
    RSDK_REGISTER_OBJECT(Piston);
    RSDK_REGISTER_OBJECT(PKingAttack);
    RSDK_REGISTER_OBJECT(PlaneSeeSaw);
    RSDK_REGISTER_OBJECT(PlaneSwitch);
    RSDK_REGISTER_OBJECT(Platform);
    RSDK_REGISTER_OBJECT(PlatformControl);
    RSDK_REGISTER_OBJECT(PlatformNode);
    RSDK_REGISTER_OBJECT(Player);
    RSDK_REGISTER_OBJECT(PlayerHelpers);
    RSDK_REGISTER_OBJECT(PlayerProbe);
    RSDK_REGISTER_OBJECT(PohBee);
    RSDK_REGISTER_OBJECT(Pointdexter);
    RSDK_REGISTER_OBJECT(PopcornKernel);
    RSDK_REGISTER_OBJECT(PopcornMachine);
    RSDK_REGISTER_OBJECT(PopOut);
    RSDK_REGISTER_OBJECT(Press);
    RSDK_REGISTER_OBJECT(PrintBlock);
    RSDK_REGISTER_OBJECT(Projectile);
    RSDK_REGISTER_OBJECT(Propeller);
    RSDK_REGISTER_OBJECT(PropellerShaft);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(PSZ1Intro);
#endif
    RSDK_REGISTER_OBJECT(PSZ1Setup);
    RSDK_REGISTER_OBJECT(PSZ2Intro);
    RSDK_REGISTER_OBJECT(PSZ2Outro);
    RSDK_REGISTER_OBJECT(PSZ2Setup);
    RSDK_REGISTER_OBJECT(PSZDoor);
    RSDK_REGISTER_OBJECT(PSZEggman);
    RSDK_REGISTER_OBJECT(PSZLauncher);
    RSDK_REGISTER_OBJECT(PullChain);
    RSDK_REGISTER_OBJECT(PullSwitch);
    RSDK_REGISTER_OBJECT(PushSpring);
    RSDK_REGISTER_OBJECT(PuyoAI);
    RSDK_REGISTER_OBJECT(PuyoAttack);
    RSDK_REGISTER_OBJECT(PuyoBean);
    RSDK_REGISTER_OBJECT(PuyoGame);
    RSDK_REGISTER_OBJECT(PuyoIndicator);
    RSDK_REGISTER_OBJECT(PuyoLabel);
    RSDK_REGISTER_OBJECT(PuyoLevelSelect);
    RSDK_REGISTER_OBJECT(PuyoMatch);
    RSDK_REGISTER_OBJECT(PuyoScore);
    RSDK_REGISTER_OBJECT(Rattlekiller);
    RSDK_REGISTER_OBJECT(Reagent);
    RSDK_REGISTER_OBJECT(Redz);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(ReplayDB);
    RSDK_REGISTER_OBJECT(ReplayRecorder);
#endif
    RSDK_REGISTER_OBJECT(Rexon);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(Rhinobot);
#endif
    RSDK_REGISTER_OBJECT(Ring);
    RSDK_REGISTER_OBJECT(RingField);
    RSDK_REGISTER_OBJECT(RisingLava);
    RSDK_REGISTER_OBJECT(RockDrill);
    RSDK_REGISTER_OBJECT(RockemSockem);
    RSDK_REGISTER_OBJECT(RollerMKII);
    RSDK_REGISTER_OBJECT(RotatingSpikes);
    RSDK_REGISTER_OBJECT(RotatingStair);
    RSDK_REGISTER_OBJECT(RPlaneShifter);
    RSDK_REGISTER_OBJECT(RTeleporter);
    RSDK_REGISTER_OBJECT(RubyPortal);
#if MANIA_USE_PLUS
    RSDK_REGISTER_STATIC_VARIABLES(SaveGame);
#else
    RSDK_REGISTER_OBJECT(SaveGame);
#endif
    RSDK_REGISTER_OBJECT(Scarab);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(SchrodingersCapsule);
#endif
    RSDK_REGISTER_OBJECT(ScoreBonus);
    RSDK_REGISTER_OBJECT(ScrewMobile);
    RSDK_REGISTER_OBJECT(SDashWheel);
    RSDK_REGISTER_OBJECT(SeeSaw);
    RSDK_REGISTER_OBJECT(SeltzerBottle);
    RSDK_REGISTER_OBJECT(SeltzerWater);
    RSDK_REGISTER_OBJECT(SentryBug);
    RSDK_REGISTER_OBJECT(Shield);
    RSDK_REGISTER_OBJECT(Shiversaw);
    RSDK_REGISTER_OBJECT(ShopWindow);
    RSDK_REGISTER_OBJECT(Shuriken);
    RSDK_REGISTER_OBJECT(Shutterbug);
    RSDK_REGISTER_OBJECT(SideBarrel);
    RSDK_REGISTER_OBJECT(SignPost);
    RSDK_REGISTER_OBJECT(SilverSonic);
    RSDK_REGISTER_OBJECT(SizeLaser);
    RSDK_REGISTER_OBJECT(SkyTeleporter);
    RSDK_REGISTER_OBJECT(Smog);
    RSDK_REGISTER_OBJECT(Snowflakes);
    RSDK_REGISTER_OBJECT(Sol);
    RSDK_REGISTER_OBJECT(Soundboard);
    RSDK_REGISTER_OBJECT(SP500);
    RSDK_REGISTER_OBJECT(SP500MkII);
    RSDK_REGISTER_OBJECT(SparkRail);
    RSDK_REGISTER_OBJECT(Spear);
    RSDK_REGISTER_OBJECT(SpearBlock);
    RSDK_REGISTER_OBJECT(SpecialClear);
    RSDK_REGISTER_OBJECT(SpecialRing);
    RSDK_REGISTER_OBJECT(SpeedBooster);
    RSDK_REGISTER_OBJECT(SpeedGate);
    RSDK_REGISTER_OBJECT(SpiderMobile);
    RSDK_REGISTER_OBJECT(SpikeCorridor);
    RSDK_REGISTER_OBJECT(SpikeCrusher);
    RSDK_REGISTER_OBJECT(SpikeFlail);
    RSDK_REGISTER_OBJECT(SpikeLog);
    RSDK_REGISTER_OBJECT(Spikes);
    RSDK_REGISTER_OBJECT(SpinBooster);
    RSDK_REGISTER_OBJECT(SpinSign);
    RSDK_REGISTER_OBJECT(Spiny);
    RSDK_REGISTER_OBJECT(SpiralPlatform);
    RSDK_REGISTER_OBJECT(Splats);
    RSDK_REGISTER_OBJECT(Spring);
    RSDK_REGISTER_OBJECT(Springboard);
    RSDK_REGISTER_OBJECT(SPZ1Intro);
    RSDK_REGISTER_OBJECT(SPZ1Setup);
    RSDK_REGISTER_OBJECT(SPZ2Outro);
    RSDK_REGISTER_OBJECT(SPZ2Setup);
    RSDK_REGISTER_OBJECT(SSZ1Intro);
    RSDK_REGISTER_OBJECT(SSZ1Outro);
    RSDK_REGISTER_OBJECT(SSZ1Setup);
    RSDK_REGISTER_OBJECT(SSZ2Setup);
    RSDK_REGISTER_OBJECT(SSZ3Cutscene);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(SSZEggman);
#endif
    RSDK_REGISTER_OBJECT(SSZSpikeBall);
    RSDK_REGISTER_OBJECT(SSZSpotlight);
    RSDK_REGISTER_OBJECT(Staircase);
    RSDK_REGISTER_OBJECT(Stalactite);
    RSDK_REGISTER_OBJECT(StarPost);
    RSDK_REGISTER_OBJECT(Stegway);
    RSDK_REGISTER_OBJECT(StickyPlatform);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(Summary);
    RSDK_REGISTER_OBJECT(SummaryEmerald);
#endif
    RSDK_REGISTER_OBJECT(SuperSparkle);
    RSDK_REGISTER_OBJECT(Sweep);
    RSDK_REGISTER_OBJECT(SwingRope);
    RSDK_REGISTER_OBJECT(SwitchDoor);
    RSDK_REGISTER_OBJECT(Syringe);
    RSDK_REGISTER_OBJECT(TAEmerald);
    RSDK_REGISTER_OBJECT(TargetBumper);
    RSDK_REGISTER_OBJECT(Technosqueek);
    RSDK_REGISTER_OBJECT(TeeterTotter);
    RSDK_REGISTER_OBJECT(TetherBall);
    RSDK_REGISTER_OBJECT(ThanksSetup);
    RSDK_REGISTER_OBJECT(ThoughtBubble);
    RSDK_REGISTER_OBJECT(TilePlatform);
#if MANIA_USE_PLUS
    RSDK_REGISTER_STATIC_VARIABLES(TimeAttackData);
#else
    RSDK_REGISTER_OBJECT(TimeAttackData);
#endif
    RSDK_REGISTER_OBJECT(TimeAttackGate);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(TimeAttackMenu);
#endif
    RSDK_REGISTER_OBJECT(TimePost);
    RSDK_REGISTER_OBJECT(TimeTravelSetup);
    RSDK_REGISTER_OBJECT(TippingPlatform);
    RSDK_REGISTER_OBJECT(Title3DSprite);
    RSDK_REGISTER_OBJECT(TitleBG);
    RSDK_REGISTER_OBJECT(TitleCard);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(TitleEggman);
#endif
    RSDK_REGISTER_OBJECT(TitleLogo);
    RSDK_REGISTER_OBJECT(TitleSetup);
    RSDK_REGISTER_OBJECT(TitleSonic);
    RSDK_REGISTER_OBJECT(TMZ1Outro);
    RSDK_REGISTER_OBJECT(TMZ1Setup);
    RSDK_REGISTER_OBJECT(TMZ2Outro);
    RSDK_REGISTER_OBJECT(TMZ2Setup);
    RSDK_REGISTER_OBJECT(TMZ3Setup);
    RSDK_REGISTER_OBJECT(TMZAlert);
    RSDK_REGISTER_OBJECT(TMZBarrier);
    RSDK_REGISTER_OBJECT(TMZCable);
    RSDK_REGISTER_OBJECT(TMZFlames);
    RSDK_REGISTER_OBJECT(Tornado);
    RSDK_REGISTER_OBJECT(TornadoPath);
    RSDK_REGISTER_OBJECT(Toxomister);
    RSDK_REGISTER_OBJECT(TransportTube);
    RSDK_REGISTER_OBJECT(TryAgain);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(TryAgainE);
#endif
    RSDK_REGISTER_OBJECT(TTCutscene);
    RSDK_REGISTER_OBJECT(TubeSpring);
    RSDK_REGISTER_OBJECT(Tubinaut);
    RSDK_REGISTER_OBJECT(Tuesday);
    RSDK_REGISTER_OBJECT(Turbine);
    RSDK_REGISTER_OBJECT(TurboSpiker);
    RSDK_REGISTER_OBJECT(TurboTurtle);
    RSDK_REGISTER_OBJECT(Turntable);
    RSDK_REGISTER_OBJECT(TurretSwitch);
    RSDK_REGISTER_OBJECT(TVFlyingBattery);
    RSDK_REGISTER_OBJECT(TVPole);
    RSDK_REGISTER_OBJECT(TVVan);
    RSDK_REGISTER_OBJECT(TwistedTubes);
    RSDK_REGISTER_OBJECT(TwistingDoor);
    RSDK_REGISTER_OBJECT(TwistingSlide);
    RSDK_REGISTER_OBJECT(UberCaterkiller);
    RSDK_REGISTER_OBJECT(UFO_Camera);
    RSDK_REGISTER_OBJECT(UFO_Circuit);
    RSDK_REGISTER_OBJECT(UFO_Decoration);
    RSDK_REGISTER_OBJECT(UFO_Dust);
    RSDK_REGISTER_OBJECT(UFO_HUD);
    RSDK_REGISTER_OBJECT(UFO_ItemBox);
    RSDK_REGISTER_OBJECT(UFO_Message);
    RSDK_REGISTER_OBJECT(UFO_Plasma);
    RSDK_REGISTER_OBJECT(UFO_Player);
    RSDK_REGISTER_OBJECT(UFO_Ring);
    RSDK_REGISTER_OBJECT(UFO_Setup);
    RSDK_REGISTER_OBJECT(UFO_Shadow);
    RSDK_REGISTER_OBJECT(UFO_SpeedLines);
    RSDK_REGISTER_OBJECT(UFO_Sphere);
    RSDK_REGISTER_OBJECT(UFO_Springboard);
    RSDK_REGISTER_OBJECT(UFO_Water);
    RSDK_REGISTER_OBJECT(UIBackground);
    RSDK_REGISTER_OBJECT(UIButton);
    RSDK_REGISTER_OBJECT(UIButtonLabel);
    RSDK_REGISTER_OBJECT(UIButtonPrompt);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(UICarousel);
#endif
    RSDK_REGISTER_OBJECT(UICharButton);
    RSDK_REGISTER_OBJECT(UIChoice);
    RSDK_REGISTER_OBJECT(UIControl);
    RSDK_REGISTER_OBJECT(UICreditsText);
    RSDK_REGISTER_OBJECT(UIDialog);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(UIDiorama);
#endif
    RSDK_REGISTER_OBJECT(UIHeading);
    RSDK_REGISTER_OBJECT(UIInfoLabel);
    RSDK_REGISTER_OBJECT(UIKeyBinder);
    RSDK_REGISTER_OBJECT(UILeaderboard);
    RSDK_REGISTER_OBJECT(UIMedallionPanel);
    RSDK_REGISTER_OBJECT(UIModeButton);
    RSDK_REGISTER_OBJECT(UIOptionPanel);
    RSDK_REGISTER_OBJECT(UIPicture);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(UIPopover);
    RSDK_REGISTER_OBJECT(UIRankButton);
    RSDK_REGISTER_OBJECT(UIReplayCarousel);
#endif
#if GAME_VERSION != VER_100
    RSDK_REGISTER_OBJECT(UIResPicker);
#endif
    RSDK_REGISTER_OBJECT(UISaveSlot);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(UIShifter);
#endif
    RSDK_REGISTER_OBJECT(UISlider);
    RSDK_REGISTER_OBJECT(UISubHeading);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(UITABanner);
#endif
    RSDK_REGISTER_OBJECT(UITAZoneModule);
    RSDK_REGISTER_OBJECT(UIText);
    RSDK_REGISTER_OBJECT(UITransition);
    RSDK_REGISTER_OBJECT(UIUsernamePopup);
    RSDK_REGISTER_OBJECT(UIVideo);
    RSDK_REGISTER_OBJECT(UIVsCharSelector);
    RSDK_REGISTER_OBJECT(UIVsResults);
    RSDK_REGISTER_OBJECT(UIVsRoundPicker);
    RSDK_REGISTER_OBJECT(UIVsScoreboard);
    RSDK_REGISTER_OBJECT(UIVsZoneButton);
    RSDK_REGISTER_OBJECT(UIWaitSpinner);
    RSDK_REGISTER_OBJECT(UIWidgets);
#if GAME_VERSION != VER_100
    RSDK_REGISTER_OBJECT(UIWinSize);
#endif
    RSDK_REGISTER_OBJECT(UncurlPlant);
    RSDK_REGISTER_OBJECT(Valve);
    RSDK_REGISTER_OBJECT(VanishPlatform);
    RSDK_REGISTER_OBJECT(Vultron);
    RSDK_REGISTER_OBJECT(WalkerLegs);
    RSDK_REGISTER_OBJECT(WallBumper);
    RSDK_REGISTER_OBJECT(WallCrawl);
    RSDK_REGISTER_OBJECT(WarpDoor);
    RSDK_REGISTER_OBJECT(Water);
#if MANIA_USE_PLUS
    RSDK_REGISTER_OBJECT(WaterfallSound);
#endif
    RSDK_REGISTER_OBJECT(WaterGush);
    RSDK_REGISTER_OBJECT(WeatherMobile);
    RSDK_REGISTER_OBJECT(WeatherTV);
    RSDK_REGISTER_OBJECT(Whirlpool);
    RSDK_REGISTER_OBJECT(Wisp);
    RSDK_REGISTER_OBJECT(WoodChipper);
    RSDK_REGISTER_OBJECT(Woodrow);
    RSDK_REGISTER_OBJECT(YoyoPulley);
    RSDK_REGISTER_OBJECT(ZipLine);
    RSDK_REGISTER_OBJECT(Zone);

#if RETRO_USE_MOD_LOADER
    InitModAPI();
#endif
}

#if RETRO_USE_MOD_LOADER
#include "PublicFunctions.c"

void InitModAPI(void)
{
    // Init Public Functions
    InitPublicFunctions();
}

bool32 LinkModLogic(EngineInfo *info, const char *id)
{
#if RETRO_REV02
    LinkGameLogicDLL(info);
#else
    LinkGameLogicDLL(*info);
#endif
    return true;
}
#endif