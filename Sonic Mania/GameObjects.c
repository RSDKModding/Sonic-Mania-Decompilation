#include "SonicMania.h"

SceneInfo *RSDK_sceneInfo = NULL;
EngineInfo *RSDK_info;
#if RETRO_USE_PLUS
SKUInfo *RSDK_sku = NULL;
#endif
ControllerState *RSDK_controller = NULL;
AnalogState *RSDK_stickL         = NULL;
#if RETRO_USE_PLUS
AnalogState *RSDK_stickR    = NULL;
TriggerState *RSDK_triggerL = NULL;
TriggerState *RSDK_triggerR = NULL;
#endif
TouchMouseData *RSDK_touchMouse = NULL;
#if RETRO_USE_PLUS
UnknownInfo *RSDK_unknown = NULL;
#endif
ScreenInfo *RSDK_screens = NULL;

RSDKFunctionTable RSDK;
#if RETRO_USE_PLUS
APIFunctionTable API;
#endif
#if RETRO_USE_MOD_LOADER
ModFunctionTable Mod;

#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void*)(func))
#endif

void LinkGameLogicDLL(GameInfo *info)
{
#if RETRO_USE_PLUS
    memset(&API, 0, sizeof(APIFunctionTable));
#endif
    memset(&RSDK, 0, sizeof(RSDKFunctionTable));

    if (info->functionPtrs)
        memcpy(&RSDK, info->functionPtrs, sizeof(RSDKFunctionTable));
#if RETRO_USE_PLUS
    if (info->APIPtrs)
        memcpy(&API, info->APIPtrs, sizeof(APIFunctionTable));
#endif
#if RETRO_USE_MOD_LOADER
    if (info->modPtrs)
        memcpy(&Mod, info->modPtrs, sizeof(ModFunctionTable));
#endif

    RSDK_info = info->engineInfo; //GameInfo
#if RETRO_USE_PLUS
    RSDK_sku  = info->currentSKU;
#endif
    RSDK_sceneInfo  = info->sceneInfo; //StageInfo
    RSDK_controller = info->controller;
    RSDK_stickL     = info->stickL;
#if RETRO_USE_PLUS
    RSDK_stickR   = info->stickR;
    RSDK_triggerL = info->triggerL;
    RSDK_triggerR = info->triggerR;
#endif
    RSDK_touchMouse = info->touchMouse;
#if RETRO_USE_PLUS
    RSDK_unknown = info->unknown;
#endif
    RSDK_screens = info->screenInfo;
    RSDK.InitGlobalVariables((void **)&globals, sizeof(GlobalVariables));

    defaultHitbox.left   = -10;
    defaultHitbox.top    = -20;
    defaultHitbox.right  = 10;
    defaultHitbox.bottom = 20;

    RSDK_ADD_OBJECT(Acetone);
    RSDK_ADD_OBJECT(ActClear);
    RSDK_ADD_OBJECT(AIZEggRobo);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(AIZEncoreTutorial);
#endif
    RSDK_ADD_OBJECT(AIZKingClaw);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(AIZRockPile);
#endif
    RSDK_ADD_OBJECT(AIZSetup);
    RSDK_ADD_OBJECT(AIZTornado);
    RSDK_ADD_OBJECT(AIZTornadoPath);
    RSDK_ADD_OBJECT(AmoebaDroid);
    RSDK_ADD_OBJECT(AnimalHBH);
    RSDK_ADD_OBJECT(Animals);
    RSDK_ADD_OBJECT(Announcer);
#if !RETRO_USE_PLUS
    RSDK_ADD_OBJECT(APICallback);
#endif
    RSDK_ADD_OBJECT(Aquis);
    RSDK_ADD_OBJECT(Armadiloid);
    RSDK_ADD_OBJECT(BadnikHelpers);
    RSDK_ADD_OBJECT(Ball);
    RSDK_ADD_OBJECT(BallCannon);
    RSDK_ADD_OBJECT(BallHog);
    RSDK_ADD_OBJECT(BarStool);
    RSDK_ADD_OBJECT(Batbot);
    RSDK_ADD_OBJECT(Batbrain);
    RSDK_ADD_OBJECT(Beanstalk);
    RSDK_ADD_OBJECT(BGSwitch);
    RSDK_ADD_OBJECT(BigSqueeze);
    RSDK_ADD_OBJECT(BladePole);
    RSDK_ADD_OBJECT(Blaster);
    RSDK_ADD_OBJECT(Blastoid);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(Bloominator);
#endif
    RSDK_ADD_OBJECT(Bomb);
    RSDK_ADD_OBJECT(BouncePlant);
    RSDK_ADD_OBJECT(BoundsMarker);
    RSDK_ADD_OBJECT(BreakableWall);
    RSDK_ADD_OBJECT(BreakBar);
    RSDK_ADD_OBJECT(Bridge);
    RSDK_ADD_OBJECT(BSS_Collectable);
    RSDK_ADD_OBJECT(BSS_Collected);
    RSDK_ADD_OBJECT(BSS_Horizon);
    RSDK_ADD_OBJECT(BSS_HUD);
    RSDK_ADD_OBJECT(BSS_Message);
    RSDK_ADD_OBJECT(BSS_Palette);
    RSDK_ADD_OBJECT(BSS_Player);
    RSDK_ADD_OBJECT(BSS_Setup);
    RSDK_ADD_OBJECT(Bubbler);
    RSDK_ADD_OBJECT(BuckwildBall);
    RSDK_ADD_OBJECT(Buggernaut);
    RSDK_ADD_OBJECT(Bumpalo);
    RSDK_ADD_OBJECT(Bungee);
    RSDK_ADD_OBJECT(BurningLog);
    RSDK_ADD_OBJECT(Button);
    RSDK_ADD_OBJECT(ButtonDoor);
    RSDK_ADD_OBJECT(BuzzBomber);
    RSDK_ADD_OBJECT(BuzzSaw);
    RSDK_ADD_OBJECT(CableWarp);
    RSDK_ADD_OBJECT(Cactula);
    RSDK_ADD_OBJECT(Camera);
    RSDK_ADD_OBJECT(Canista);
    RSDK_ADD_OBJECT(Caterkiller);
    RSDK_ADD_OBJECT(CaterkillerJr);
    RSDK_ADD_OBJECT(ChaosEmerald);
    RSDK_ADD_OBJECT(CheckerBall);
    RSDK_ADD_OBJECT(ChemBubble);
    RSDK_ADD_OBJECT(ChemicalBall);
    RSDK_ADD_OBJECT(ChemicalPool);
    RSDK_ADD_OBJECT(Chopper);
    RSDK_ADD_OBJECT(CircleBumper);
    RSDK_ADD_OBJECT(Clapperboard);
    RSDK_ADD_OBJECT(Clucker);
    RSDK_ADD_OBJECT(CollapsingPlatform);
    RSDK_ADD_OBJECT(CollapsingSand);
    RSDK_ADD_OBJECT(ColorHelpers);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(Competition);
    RSDK_ADD_OBJECT(CompetitionMenu);
#endif
    RSDK_ADD_OBJECT(CompetitionSession);
    RSDK_ADD_OBJECT(Constellation);
    RSDK_ADD_OBJECT(ContinuePlayer);
    RSDK_ADD_OBJECT(ContinueSetup);
    RSDK_ADD_OBJECT(ConveyorBelt);
    RSDK_ADD_OBJECT(ConveyorPlatform);
    RSDK_ADD_OBJECT(ConveyorWheel);
    RSDK_ADD_OBJECT(CorkscrewPath);
    RSDK_ADD_OBJECT(COverlay);
    RSDK_ADD_OBJECT(CPZ1Intro);
    RSDK_ADD_OBJECT(CPZ2Outro);
    RSDK_ADD_OBJECT(CPZBoss);
    RSDK_ADD_OBJECT(CPZSetup);
    RSDK_ADD_OBJECT(CPZShutter);
    RSDK_ADD_OBJECT(Crabmeat);
    RSDK_ADD_OBJECT(Crane);
    RSDK_ADD_OBJECT(CrashTest);
    RSDK_ADD_OBJECT(Crate);
    RSDK_ADD_OBJECT(CreditsSetup);
    RSDK_ADD_OBJECT(CrimsonEye);
    RSDK_ADD_OBJECT(Current);
    RSDK_ADD_OBJECT(CutsceneHBH);
    RSDK_ADD_OBJECT(CutsceneRules);
    RSDK_ADD_OBJECT(CutsceneSeq);
    RSDK_ADD_OBJECT(Cylinder);
    RSDK_ADD_OBJECT(DAControl);
    RSDK_ADD_OBJECT(Dango);
    RSDK_ADD_OBJECT(DASetup);
    RSDK_ADD_OBJECT(DashLift);
    RSDK_ADD_OBJECT(DBTower);
    RSDK_ADD_OBJECT(DCEvent);
    RSDK_ADD_OBJECT(DDWrecker);
    RSDK_ADD_OBJECT(Debris);
    RSDK_ADD_OBJECT(DebugMode);
    RSDK_ADD_OBJECT(Decoration);
    RSDK_ADD_OBJECT(DemoMenu);
    RSDK_ADD_OBJECT(DERobot);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(DialogRunner);
#endif
    RSDK_ADD_OBJECT(DirectorChair);
    RSDK_ADD_OBJECT(DiveEggman);
    RSDK_ADD_OBJECT(DNARiser);
    RSDK_ADD_OBJECT(DoorTrigger);
    RSDK_ADD_OBJECT(Dragonfly);
    RSDK_ADD_OBJECT(DrawHelpers);
    RSDK_ADD_OBJECT(Drillerdroid);
    RSDK_ADD_OBJECT(DrillerdroidO);
    RSDK_ADD_OBJECT(Dust);
#if !RETRO_USE_PLUS
    RSDK_ADD_OBJECT(E3MenuSetup);
#endif
    RSDK_ADD_OBJECT(EggJanken);
    RSDK_ADD_OBJECT(EggJankenPart);
    RSDK_ADD_OBJECT(EggLoco);
    RSDK_ADD_OBJECT(Eggman);
    RSDK_ADD_OBJECT(EggPistonsMKII);
    RSDK_ADD_OBJECT(EggPrison);
    RSDK_ADD_OBJECT(EggTower);
    RSDK_ADD_OBJECT(EggTV);
    RSDK_ADD_OBJECT(ElectroMagnet);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(EncoreGoodEnd);
    RSDK_ADD_OBJECT(EncoreIntro);
    RSDK_ADD_OBJECT(EncoreRoute);
#endif
    RSDK_ADD_OBJECT(ERZGunner);
    RSDK_ADD_OBJECT(ERZKing);
    RSDK_ADD_OBJECT(ERZMystic);
    RSDK_ADD_OBJECT(ERZOutro);
    RSDK_ADD_OBJECT(ERZRider);
    RSDK_ADD_OBJECT(ERZSetup);
    RSDK_ADD_OBJECT(ERZShinobi);
    RSDK_ADD_OBJECT(ERZStart);
    RSDK_ADD_OBJECT(EscapeCar);
    RSDK_ADD_OBJECT(Explosion);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(ExtrasMenu);
#endif
    RSDK_ADD_OBJECT(Fan);
    RSDK_ADD_OBJECT(FarPlane);
    RSDK_ADD_OBJECT(FBZ1Outro);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(FBZ2Outro);
#endif
    RSDK_ADD_OBJECT(FBZFan);
    RSDK_ADD_OBJECT(FBZMissile);
    RSDK_ADD_OBJECT(FBZSetup);
    RSDK_ADD_OBJECT(FBZSinkTrash);
    RSDK_ADD_OBJECT(FBZStorm);
    RSDK_ADD_OBJECT(FBZTrash);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(FernParallax);
#endif
    RSDK_ADD_OBJECT(FilmProjector);
    RSDK_ADD_OBJECT(FilmReel);
    RSDK_ADD_OBJECT(Fireball);
    RSDK_ADD_OBJECT(Fireflies);
    RSDK_ADD_OBJECT(Firework);
    RSDK_ADD_OBJECT(Fireworm);
    RSDK_ADD_OBJECT(FlameSpring);
    RSDK_ADD_OBJECT(Flamethrower);
    RSDK_ADD_OBJECT(FlasherMKII);
    RSDK_ADD_OBJECT(FlingRamp);
    RSDK_ADD_OBJECT(Flipper);
    RSDK_ADD_OBJECT(FlowerPod);
    RSDK_ADD_OBJECT(FoldingPlatform);
    RSDK_ADD_OBJECT(ForceSpin);
    RSDK_ADD_OBJECT(ForceUnstick);
    RSDK_ADD_OBJECT(FrostThrower);
    RSDK_ADD_OBJECT(Funnel);
    RSDK_ADD_OBJECT(FXExpandRing);
    RSDK_ADD_OBJECT(FXFade);
    RSDK_ADD_OBJECT(FXRuby);
    RSDK_ADD_OBJECT(FXSpinRay);
    RSDK_ADD_OBJECT(FXTrail);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(FXWaveRing);
#endif
    RSDK_ADD_OBJECT(Gachapandora);
    RSDK_ADD_OBJECT(GameOver);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT_CONTAINER(GameProgress);
#else
    // RSDK_ADD_OBJECT(GameProgress); //TODO: lol
#endif
    RSDK_ADD_OBJECT(GasPlatform);
    RSDK_ADD_OBJECT(GenericTrigger);
    RSDK_ADD_OBJECT(GHZ2Outro);
    RSDK_ADD_OBJECT(GHZCutsceneK);
    RSDK_ADD_OBJECT(GHZCutsceneST);
    RSDK_ADD_OBJECT(GHZSetup);
    RSDK_ADD_OBJECT(GiantPistol);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(GigaMetal);
#endif
    RSDK_ADD_OBJECT(Gondola);
    RSDK_ADD_OBJECT(Grabber);
    RSDK_ADD_OBJECT(GreenScreen);
    RSDK_ADD_OBJECT(GymBar);
    RSDK_ADD_OBJECT(HandLauncher);
    RSDK_ADD_OBJECT(HangConveyor);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(HangGlider);
#endif
    RSDK_ADD_OBJECT(HangPoint);
    RSDK_ADD_OBJECT(Hatch);
    RSDK_ADD_OBJECT(Hatterkiller);
    RSDK_ADD_OBJECT(HCZ1Intro);
    RSDK_ADD_OBJECT(HCZOneWayDoor);
    RSDK_ADD_OBJECT(HCZSetup);
    RSDK_ADD_OBJECT(HCZSpikeBall);
    RSDK_ADD_OBJECT(HeavyGunner);
    RSDK_ADD_OBJECT(HeavyKing);
    RSDK_ADD_OBJECT(HeavyMystic);
    RSDK_ADD_OBJECT(HeavyRider);
    RSDK_ADD_OBJECT(HeavyShinobi);
    RSDK_ADD_OBJECT(HiLoSign);
    RSDK_ADD_OBJECT(Honkytonk);
    RSDK_ADD_OBJECT(Hotaru);
    RSDK_ADD_OBJECT(HotaruHiWatt);
    RSDK_ADD_OBJECT(HotaruMKII);
    RSDK_ADD_OBJECT(HPZEmerald);
    RSDK_ADD_OBJECT(HUD);
    RSDK_ADD_OBJECT(Ice);
    RSDK_ADD_OBJECT(IceBomba);
    RSDK_ADD_OBJECT(IceSpring);
    RSDK_ADD_OBJECT(ImageTrail);
    RSDK_ADD_OBJECT(Ink);
    RSDK_ADD_OBJECT(InkWipe);
    RSDK_ADD_OBJECT(InvincibleStars);
    RSDK_ADD_OBJECT(InvisibleBlock);
    RSDK_ADD_OBJECT(ItemBox);
    RSDK_ADD_OBJECT(Iwamodoki);
    RSDK_ADD_OBJECT(JacobsLadder);
    RSDK_ADD_OBJECT(Jawz);
    RSDK_ADD_OBJECT(Jellygnite);
    RSDK_ADD_OBJECT(JuggleSaw);
    RSDK_ADD_OBJECT(JunctionWheel);
    RSDK_ADD_OBJECT(Kabasira);
    RSDK_ADD_OBJECT(Kanabun);
    RSDK_ADD_OBJECT(KingAttack);
    RSDK_ADD_OBJECT(KingClaw);
    RSDK_ADD_OBJECT(KleptoMobile);
    RSDK_ADD_OBJECT(LargeGear);
    RSDK_ADD_OBJECT(Launcher);
    RSDK_ADD_OBJECT(LaunchSpring);
    RSDK_ADD_OBJECT(LaundroMobile);
    RSDK_ADD_OBJECT(LavaFall);
    RSDK_ADD_OBJECT(LavaGeyser);
    RSDK_ADD_OBJECT(LEDPanel);
    RSDK_ADD_OBJECT(Letterboard);
    RSDK_ADD_OBJECT(LevelSelect);
    RSDK_ADD_OBJECT(LightBarrier);
    RSDK_ADD_OBJECT(LightBulb);
    RSDK_ADD_OBJECT(Localization);
    RSDK_ADD_OBJECT(LocoSmoke);
    RSDK_ADD_OBJECT(LogHelpers);
    RSDK_ADD_OBJECT(LogoSetup);
    RSDK_ADD_OBJECT(LottoBall);
    RSDK_ADD_OBJECT(LottoMachine);
    RSDK_ADD_OBJECT(LoveTester);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(LRZ1Intro);
#endif
    RSDK_ADD_OBJECT(LRZ1Outro);
    RSDK_ADD_OBJECT(LRZ1Setup);
    RSDK_ADD_OBJECT(LRZ2Setup);
    RSDK_ADD_OBJECT(LRZ3Cutscene);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(LRZ3Outro);
#endif
    RSDK_ADD_OBJECT(LRZ3OutroK);
    RSDK_ADD_OBJECT(LRZ3Setup);
    RSDK_ADD_OBJECT(LRZConvControl);
    RSDK_ADD_OBJECT(LRZConvDropper);
    RSDK_ADD_OBJECT(LRZConveyor);
    RSDK_ADD_OBJECT(LRZConvItem);
    RSDK_ADD_OBJECT(LRZConvSwitch);
    RSDK_ADD_OBJECT(LRZFireball);
    RSDK_ADD_OBJECT(LRZRockPile);
    RSDK_ADD_OBJECT(LRZSpikeBall);
    RSDK_ADD_OBJECT(LRZSpiral);
    RSDK_ADD_OBJECT(MagnetSphere);
    RSDK_ADD_OBJECT(MagPlatform);
    RSDK_ADD_OBJECT(MagSpikeBall);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(MainMenu);
    RSDK_ADD_OBJECT(ManiaModeMenu);
#endif
    RSDK_ADD_OBJECT(MathHelpers);
    RSDK_ADD_OBJECT(MatryoshkaBom);
    RSDK_ADD_OBJECT(MechaBu);
    RSDK_ADD_OBJECT(MegaChopper);
    RSDK_ADD_OBJECT(MegaOctus);
    RSDK_ADD_OBJECT(MenuParam);
    RSDK_ADD_OBJECT(MenuSetup);
    RSDK_ADD_OBJECT(MetalArm);
    RSDK_ADD_OBJECT(MetalSonic);
    RSDK_ADD_OBJECT(MeterDroid);
    RSDK_ADD_OBJECT(MicDrop);
    RSDK_ADD_OBJECT(Mine);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(MMZ2Outro);
    RSDK_ADD_OBJECT(MMZLightning);
#endif
    RSDK_ADD_OBJECT(MMZSetup);
    RSDK_ADD_OBJECT(MMZWheel);
    RSDK_ADD_OBJECT(MonarchBG);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(MonarchPlans);
#endif
    RSDK_ADD_OBJECT(MonkeyDude);
    RSDK_ADD_OBJECT(Motobug);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(MSBomb);
#endif
    RSDK_ADD_OBJECT(MSFactory);
    RSDK_ADD_OBJECT(MSHologram);
    RSDK_ADD_OBJECT(MSOrb);
    RSDK_ADD_OBJECT(MSPanel);
    RSDK_ADD_OBJECT(MSZ1KIntro);
    RSDK_ADD_OBJECT(MSZ2Cutscene);
    RSDK_ADD_OBJECT(MSZCutsceneK);
    RSDK_ADD_OBJECT(MSZCutsceneST);
    RSDK_ADD_OBJECT(MSZSetup);
    RSDK_ADD_OBJECT(MSZSpotlight);
    RSDK_ADD_OBJECT(Music);
    RSDK_ADD_OBJECT(Newspaper);
    RSDK_ADD_OBJECT(Newtron);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(NoSwap);
#endif
    RSDK_ADD_OBJECT(Octus);
    RSDK_ADD_OBJECT(OneWayDoor);
    RSDK_ADD_OBJECT(OOZ1Outro);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(OOZ2Outro);
#endif
    RSDK_ADD_OBJECT(OOZFlames);
    RSDK_ADD_OBJECT(OOZSetup);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT_CONTAINER(Options);
    RSDK_ADD_OBJECT(OptionsMenu);
#else
    RSDK_ADD_OBJECT(Options);
#endif
    RSDK_ADD_OBJECT(Orbinaut);
    RSDK_ADD_OBJECT(OrbitSpike);
    RSDK_ADD_OBJECT(PaintingEyes);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(Palette);
#endif
    RSDK_ADD_OBJECT(PaperRoller);
    RSDK_ADD_OBJECT(ParallaxSprite);
    RSDK_ADD_OBJECT(ParticleHelpers);
    RSDK_ADD_OBJECT(PathInverter);
    RSDK_ADD_OBJECT(PauseMenu);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(PBL_Bumper);
    RSDK_ADD_OBJECT(PBL_Camera);
    RSDK_ADD_OBJECT(PBL_Crane);
    RSDK_ADD_OBJECT(PBL_Flipper);
    RSDK_ADD_OBJECT(PBL_HUD);
    RSDK_ADD_OBJECT(PBL_Player);
    RSDK_ADD_OBJECT(PBL_Ring);
    RSDK_ADD_OBJECT(PBL_Sector);
    RSDK_ADD_OBJECT(PBL_Setup);
    RSDK_ADD_OBJECT(PBL_TargetBumper);
#endif
    RSDK_ADD_OBJECT(Pendulum);
    RSDK_ADD_OBJECT(PetalPile);
    RSDK_ADD_OBJECT(PhantomEgg);
    RSDK_ADD_OBJECT(PhantomGunner);
    RSDK_ADD_OBJECT(PhantomHand);
    RSDK_ADD_OBJECT(PhantomKing);
    RSDK_ADD_OBJECT(PhantomMissile);
    RSDK_ADD_OBJECT(PhantomMystic);
    RSDK_ADD_OBJECT(PhantomRider);
    RSDK_ADD_OBJECT(PhantomRuby);
    RSDK_ADD_OBJECT(PhantomShield);
    RSDK_ADD_OBJECT(PhantomShinobi);
    RSDK_ADD_OBJECT(PimPom);
    RSDK_ADD_OBJECT(Pinata);
    RSDK_ADD_OBJECT(Piston);
    RSDK_ADD_OBJECT(PKingAttack);
    RSDK_ADD_OBJECT(PlaneSeeSaw);
    RSDK_ADD_OBJECT(PlaneSwitch);
    RSDK_ADD_OBJECT(Platform);
    RSDK_ADD_OBJECT(PlatformControl);
    RSDK_ADD_OBJECT(PlatformNode);
    RSDK_ADD_OBJECT(Player);
    RSDK_ADD_OBJECT(PlayerHelpers);
    RSDK_ADD_OBJECT(PlayerProbe);
    RSDK_ADD_OBJECT(PohBee);
    RSDK_ADD_OBJECT(Pointdexter);
    RSDK_ADD_OBJECT(PopcornKernel);
    RSDK_ADD_OBJECT(PopcornMachine);
    RSDK_ADD_OBJECT(PopOut);
    RSDK_ADD_OBJECT(Press);
    RSDK_ADD_OBJECT(PrintBlock);
    RSDK_ADD_OBJECT(Projectile);
    RSDK_ADD_OBJECT(Propeller);
    RSDK_ADD_OBJECT(PropellerShaft);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(PSZ1Intro);
#endif
    RSDK_ADD_OBJECT(PSZ1Setup);
    RSDK_ADD_OBJECT(PSZ2Intro);
    RSDK_ADD_OBJECT(PSZ2Outro);
    RSDK_ADD_OBJECT(PSZ2Setup);
    RSDK_ADD_OBJECT(PSZDoor);
    RSDK_ADD_OBJECT(PSZEggman);
    RSDK_ADD_OBJECT(PSZLauncher);
    RSDK_ADD_OBJECT(PullChain);
    RSDK_ADD_OBJECT(PullSwitch);
    RSDK_ADD_OBJECT(PushSpring);
    RSDK_ADD_OBJECT(PuyoAI);
    RSDK_ADD_OBJECT(PuyoAttack);
    RSDK_ADD_OBJECT(PuyoBean);
    RSDK_ADD_OBJECT(PuyoGame);
    RSDK_ADD_OBJECT(PuyoIndicator);
    RSDK_ADD_OBJECT(PuyoLabel);
    RSDK_ADD_OBJECT(PuyoLevelSelect);
    RSDK_ADD_OBJECT(PuyoMatch);
    RSDK_ADD_OBJECT(PuyoScore);
    RSDK_ADD_OBJECT(Rattlekiller);
    RSDK_ADD_OBJECT(Reagent);
    RSDK_ADD_OBJECT(Redz);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(ReplayDB);
    RSDK_ADD_OBJECT(ReplayRecorder);
#endif
    RSDK_ADD_OBJECT(Rexon);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(Rhinobot);
#endif
    RSDK_ADD_OBJECT(Ring);
    RSDK_ADD_OBJECT(RingField);
    RSDK_ADD_OBJECT(RisingLava);
    RSDK_ADD_OBJECT(RockDrill);
    RSDK_ADD_OBJECT(RockemSockem);
    RSDK_ADD_OBJECT(RollerMKII);
    RSDK_ADD_OBJECT(RotatingSpikes);
    RSDK_ADD_OBJECT(RotatingStair);
    RSDK_ADD_OBJECT(RPlaneShifter);
    RSDK_ADD_OBJECT(RTeleporter);
    RSDK_ADD_OBJECT(RubyPortal);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT_CONTAINER(SaveGame);
#else
    RSDK_ADD_OBJECT(SaveGame);
#endif
    RSDK_ADD_OBJECT(Scarab);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(SchrodingersCapsule);
#endif
    RSDK_ADD_OBJECT(ScoreBonus);
    RSDK_ADD_OBJECT(ScrewMobile);
    RSDK_ADD_OBJECT(SDashWheel);
    RSDK_ADD_OBJECT(SeeSaw);
    RSDK_ADD_OBJECT(SeltzerBottle);
    RSDK_ADD_OBJECT(SeltzerWater);
    RSDK_ADD_OBJECT(SentryBug);
    RSDK_ADD_OBJECT(Shield);
    RSDK_ADD_OBJECT(Shiversaw);
    RSDK_ADD_OBJECT(ShopWindow);
    RSDK_ADD_OBJECT(Shuriken);
    RSDK_ADD_OBJECT(Shutterbug);
    RSDK_ADD_OBJECT(SideBarrel);
    RSDK_ADD_OBJECT(SignPost);
    RSDK_ADD_OBJECT(SilverSonic);
    RSDK_ADD_OBJECT(SizeLaser);
    RSDK_ADD_OBJECT(SkyTeleporter);
    RSDK_ADD_OBJECT(Smog);
    RSDK_ADD_OBJECT(Snowflakes);
    RSDK_ADD_OBJECT(Sol);
    RSDK_ADD_OBJECT(Soundboard);
    RSDK_ADD_OBJECT(SP500);
    RSDK_ADD_OBJECT(SP500MkII);
    RSDK_ADD_OBJECT(SparkRail);
    RSDK_ADD_OBJECT(Spear);
    RSDK_ADD_OBJECT(SpearBlock);
    RSDK_ADD_OBJECT(SpecialClear);
    RSDK_ADD_OBJECT(SpecialRing);
    RSDK_ADD_OBJECT(SpeedBooster);
    RSDK_ADD_OBJECT(SpeedGate);
    RSDK_ADD_OBJECT(SpiderMobile);
    RSDK_ADD_OBJECT(SpikeCorridor);
    RSDK_ADD_OBJECT(SpikeCrusher);
    RSDK_ADD_OBJECT(SpikeFlail);
    RSDK_ADD_OBJECT(SpikeLog);
    RSDK_ADD_OBJECT(Spikes);
    RSDK_ADD_OBJECT(SpinBooster);
    RSDK_ADD_OBJECT(SpinSign);
    RSDK_ADD_OBJECT(Spiny);
    RSDK_ADD_OBJECT(SpiralPlatform);
    RSDK_ADD_OBJECT(Splats);
    RSDK_ADD_OBJECT(Spring);
    RSDK_ADD_OBJECT(Springboard);
    RSDK_ADD_OBJECT(SPZ1Intro);
    RSDK_ADD_OBJECT(SPZ1Setup);
    RSDK_ADD_OBJECT(SPZ2Outro);
    RSDK_ADD_OBJECT(SPZ2Setup);
    RSDK_ADD_OBJECT(SSZ1Intro);
    RSDK_ADD_OBJECT(SSZ1Outro);
    RSDK_ADD_OBJECT(SSZ1Setup);
    RSDK_ADD_OBJECT(SSZ2Setup);
    RSDK_ADD_OBJECT(SSZ3Cutscene);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(SSZEggman);
#endif
    RSDK_ADD_OBJECT(SSZSpikeBall);
    RSDK_ADD_OBJECT(SSZSpotlight);
    RSDK_ADD_OBJECT(Staircase);
    RSDK_ADD_OBJECT(Stalactite);
    RSDK_ADD_OBJECT(StarPost);
    RSDK_ADD_OBJECT(Stegway);
    RSDK_ADD_OBJECT(StickyPlatform);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(Summary);
    RSDK_ADD_OBJECT(SummaryEmerald);
#endif
    RSDK_ADD_OBJECT(SuperSparkle);
    RSDK_ADD_OBJECT(Sweep);
    RSDK_ADD_OBJECT(SwingRope);
    RSDK_ADD_OBJECT(SwitchDoor);
    RSDK_ADD_OBJECT(Syringe);
    RSDK_ADD_OBJECT(TAEmerald);
    RSDK_ADD_OBJECT(TargetBumper);
    RSDK_ADD_OBJECT(Technosqueek);
    RSDK_ADD_OBJECT(TeeterTotter);
    RSDK_ADD_OBJECT(TetherBall);
    RSDK_ADD_OBJECT(ThanksSetup);
    RSDK_ADD_OBJECT(ThoughtBubble);
    RSDK_ADD_OBJECT(TilePlatform);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT_CONTAINER(TimeAttackData);
#else
    RSDK_ADD_OBJECT(TimeAttackData);
#endif
    RSDK_ADD_OBJECT(TimeAttackGate);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(TimeAttackMenu);
#endif
    RSDK_ADD_OBJECT(TimePost);
    RSDK_ADD_OBJECT(TimeTravelSetup);
    RSDK_ADD_OBJECT(TippingPlatform);
    RSDK_ADD_OBJECT(Title3DSprite);
    RSDK_ADD_OBJECT(TitleBG);
    RSDK_ADD_OBJECT(TitleCard);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(TitleEggman);
#endif
    RSDK_ADD_OBJECT(TitleLogo);
    RSDK_ADD_OBJECT(TitleSetup);
    RSDK_ADD_OBJECT(TitleSonic);
    RSDK_ADD_OBJECT(TMZ1Outro);
    RSDK_ADD_OBJECT(TMZ1Setup);
    RSDK_ADD_OBJECT(TMZ2Outro);
    RSDK_ADD_OBJECT(TMZ2Setup);
    RSDK_ADD_OBJECT(TMZ3Setup);
    RSDK_ADD_OBJECT(TMZAlert);
    RSDK_ADD_OBJECT(TMZBarrier);
    RSDK_ADD_OBJECT(TMZCable);
    RSDK_ADD_OBJECT(TMZFlames);
    RSDK_ADD_OBJECT(Tornado);
    RSDK_ADD_OBJECT(TornadoPath);
    RSDK_ADD_OBJECT(Toxomister);
    RSDK_ADD_OBJECT(TransportTube);
    RSDK_ADD_OBJECT(TryAgain);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(TryAgainE);
#endif
    RSDK_ADD_OBJECT(TTCutscene);
    RSDK_ADD_OBJECT(TubeSpring);
    RSDK_ADD_OBJECT(Tubinaut);
    RSDK_ADD_OBJECT(Tuesday);
    RSDK_ADD_OBJECT(Turbine);
    RSDK_ADD_OBJECT(TurboSpiker);
    RSDK_ADD_OBJECT(TurboTurtle);
    RSDK_ADD_OBJECT(Turntable);
    RSDK_ADD_OBJECT(TurretSwitch);
    RSDK_ADD_OBJECT(TVFlyingBattery);
    RSDK_ADD_OBJECT(TVPole);
    RSDK_ADD_OBJECT(TVVan);
    RSDK_ADD_OBJECT(TwistedTubes);
    RSDK_ADD_OBJECT(TwistingDoor);
    RSDK_ADD_OBJECT(TwistingSlide);
    RSDK_ADD_OBJECT(UberCaterkiller);
    RSDK_ADD_OBJECT(UFO_Camera);
    RSDK_ADD_OBJECT(UFO_Circuit);
    RSDK_ADD_OBJECT(UFO_Decoration);
    RSDK_ADD_OBJECT(UFO_Dust);
    RSDK_ADD_OBJECT(UFO_HUD);
    RSDK_ADD_OBJECT(UFO_ItemBox);
    RSDK_ADD_OBJECT(UFO_Message);
    RSDK_ADD_OBJECT(UFO_Plasma);
    RSDK_ADD_OBJECT(UFO_Player);
    RSDK_ADD_OBJECT(UFO_Ring);
    RSDK_ADD_OBJECT(UFO_Setup);
    RSDK_ADD_OBJECT(UFO_Shadow);
    RSDK_ADD_OBJECT(UFO_SpeedLines);
    RSDK_ADD_OBJECT(UFO_Sphere);
    RSDK_ADD_OBJECT(UFO_Springboard);
    RSDK_ADD_OBJECT(UFO_Water);
    RSDK_ADD_OBJECT(UIBackground);
    RSDK_ADD_OBJECT(UIButton);
    RSDK_ADD_OBJECT(UIButtonLabel);
    RSDK_ADD_OBJECT(UIButtonPrompt);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(UICarousel);
#endif
    RSDK_ADD_OBJECT(UICharButton);
    RSDK_ADD_OBJECT(UIChoice);
    RSDK_ADD_OBJECT(UIControl);
    RSDK_ADD_OBJECT(UICreditsText);
    RSDK_ADD_OBJECT(UIDialog);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(UIDiorama);
#endif
    RSDK_ADD_OBJECT(UIHeading);
    RSDK_ADD_OBJECT(UIInfoLabel);
    RSDK_ADD_OBJECT(UIKeyBinder);
    RSDK_ADD_OBJECT(UILeaderboard);
    RSDK_ADD_OBJECT(UIMedallionPanel);
    RSDK_ADD_OBJECT(UIModeButton);
    RSDK_ADD_OBJECT(UIOptionPanel);
    RSDK_ADD_OBJECT(UIPicture);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(UIPopover);
    RSDK_ADD_OBJECT(UIRankButton);
    RSDK_ADD_OBJECT(UIReplayCarousel);
#endif
    RSDK_ADD_OBJECT(UIResPicker);
    RSDK_ADD_OBJECT(UISaveSlot);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(UIShifter);
#endif
    RSDK_ADD_OBJECT(UISlider);
    RSDK_ADD_OBJECT(UISubHeading);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(UITABanner);
#endif
    RSDK_ADD_OBJECT(UITAZoneModule);
    RSDK_ADD_OBJECT(UIText);
    RSDK_ADD_OBJECT(UITransition);
    RSDK_ADD_OBJECT(UIUsernamePopup);
    RSDK_ADD_OBJECT(UIVideo);
    RSDK_ADD_OBJECT(UIVsCharSelector);
    RSDK_ADD_OBJECT(UIVsResults);
    RSDK_ADD_OBJECT(UIVsRoundPicker);
    RSDK_ADD_OBJECT(UIVsScoreboard);
    RSDK_ADD_OBJECT(UIVsZoneButton);
    RSDK_ADD_OBJECT(UIWaitSpinner);
    RSDK_ADD_OBJECT(UIWidgets);
    RSDK_ADD_OBJECT(UIWinSize);
    RSDK_ADD_OBJECT(UncurlPlant);
    RSDK_ADD_OBJECT(Valve);
    RSDK_ADD_OBJECT(VanishPlatform);
    RSDK_ADD_OBJECT(Vultron);
    RSDK_ADD_OBJECT(WalkerLegs);
    RSDK_ADD_OBJECT(WallBumper);
    RSDK_ADD_OBJECT(WallCrawl);
    RSDK_ADD_OBJECT(WarpDoor);
    RSDK_ADD_OBJECT(Water);
#if RETRO_USE_PLUS
    RSDK_ADD_OBJECT(WaterfallSound);
#endif
    RSDK_ADD_OBJECT(WaterGush);
    RSDK_ADD_OBJECT(WeatherMobile);
    RSDK_ADD_OBJECT(WeatherTV);
    RSDK_ADD_OBJECT(Whirlpool);
    RSDK_ADD_OBJECT(Wisp);
    RSDK_ADD_OBJECT(WoodChipper);
    RSDK_ADD_OBJECT(Woodrow);
    RSDK_ADD_OBJECT(YoyoPulley);
    RSDK_ADD_OBJECT(ZipLine);
    RSDK_ADD_OBJECT(Zone);

#if RETRO_USE_MOD_LOADER
    if (info->modPtrs) {
        ADD_PUBLIC_FUNC(Player_State_None);
        ADD_PUBLIC_FUNC(Player_State_Ground);
        ADD_PUBLIC_FUNC(Player_State_Air);
        ADD_PUBLIC_FUNC(Player_State_Roll);
        ADD_PUBLIC_FUNC(Player_State_ForceRoll_Ground);
        ADD_PUBLIC_FUNC(Player_State_ForceRoll_Air);
        ADD_PUBLIC_FUNC(Player_State_LookUp);
        ADD_PUBLIC_FUNC(Player_State_Crouch);
        ADD_PUBLIC_FUNC(Player_State_Spindash);
        ADD_PUBLIC_FUNC(Player_State_Peelout);
        ADD_PUBLIC_FUNC(Player_State_OuttaHere);
        ADD_PUBLIC_FUNC(Player_State_Transform);
        ADD_PUBLIC_FUNC(Player_State_Hit);
        ADD_PUBLIC_FUNC(Player_State_Die);
        ADD_PUBLIC_FUNC(Player_State_Drown);
        ADD_PUBLIC_FUNC(Player_State_DropDash);
        ADD_PUBLIC_FUNC(Player_State_BubbleBounce);
        ADD_PUBLIC_FUNC(Player_State_TailsFlight);
        ADD_PUBLIC_FUNC(Player_State_FlyCarried);
        ADD_PUBLIC_FUNC(Player_State_KnuxGlideLeft);
        ADD_PUBLIC_FUNC(Player_State_KnuxGlideRight);
        ADD_PUBLIC_FUNC(Player_State_KnuxGlideDrop);
        ADD_PUBLIC_FUNC(Player_State_GlideSlide);
        ADD_PUBLIC_FUNC(Player_State_KnuxWallClimb);
        ADD_PUBLIC_FUNC(Player_State_KnuxLedgePullUp);
#if RETRO_USE_PLUS
        ADD_PUBLIC_FUNC(Player_State_MightyHammerDrop);
        ADD_PUBLIC_FUNC(Player_State_MightyUnspin);
        ADD_PUBLIC_FUNC(Player_SpawnMightyHammerdropDust);
        ADD_PUBLIC_FUNC(Player_CheckMightyUnspin);
        ADD_PUBLIC_FUNC(Player_State_RayGlide);
#endif
        ADD_PUBLIC_FUNC(Player_State_FlyIn);
        ADD_PUBLIC_FUNC(Player_State_JumpIn);
        ADD_PUBLIC_FUNC(Player_State_StartJumpIn);
        ADD_PUBLIC_FUNC(Player_EndFlyJumpIn);
        ADD_PUBLIC_FUNC(Player_State_EncoreRespawn);
        ADD_PUBLIC_FUNC(Player_State_Victory);
        ADD_PUBLIC_FUNC(Player_State_Bubble);
        ADD_PUBLIC_FUNC(Player_State_WaterSlide);

        ADD_PUBLIC_FUNC(Player_SonicJumpAbility);
        ADD_PUBLIC_FUNC(Player_TailsJumpAbility);
        ADD_PUBLIC_FUNC(Player_KnuxJumpAbility);
#if RETRO_USE_PLUS
        ADD_PUBLIC_FUNC(Player_MightyJumpAbility);
        ADD_PUBLIC_FUNC(Player_RayJumpAbility);

        ADD_PUBLIC_FUNC(Player_CheckRayDiving);
        ADD_PUBLIC_FUNC(Player_CheckRaySwooping);
#endif

        ADD_PUBLIC_FUNC(Player_ChangePhysicsState);
        ADD_PUBLIC_FUNC(Player_HandleGroundMovement);
        ADD_PUBLIC_FUNC(Player_HandleGroundRotation);
        ADD_PUBLIC_FUNC(Player_HandleAirMovement);
        ADD_PUBLIC_FUNC(Player_HandleAirFriction);
        ADD_PUBLIC_FUNC(Player_StartJump);
        ADD_PUBLIC_FUNC(Player_StartRoll);
        ADD_PUBLIC_FUNC(Player_SwapMainPlayer);
        ADD_PUBLIC_FUNC(Player_StartPeelout);
        ADD_PUBLIC_FUNC(Player_HandleRollDeceleration);
        ADD_PUBLIC_FUNC(Player_Hit);
        ADD_PUBLIC_FUNC(Player_CheckValidState);
        ADD_PUBLIC_FUNC(Player_CheckStartFlyCarry);
        ADD_PUBLIC_FUNC(Player_P2JumpBackIn);
        ADD_PUBLIC_FUNC(Player_ForceSuperTransform);

        ADD_PUBLIC_FUNC(Player_GiveScore);
        ADD_PUBLIC_FUNC(Player_GiveRings);
        ADD_PUBLIC_FUNC(Player_GiveLife);
        ADD_PUBLIC_FUNC(Player_ApplyShieldEffect);
        ADD_PUBLIC_FUNC(Player_ChangeCharacter);
        ADD_PUBLIC_FUNC(Player_CheckGoSuper);
        ADD_PUBLIC_FUNC(Player_LoseRings);
        ADD_PUBLIC_FUNC(Player_LoseHyperRings);
        ADD_PUBLIC_FUNC(Player_HandleDeath);
        ADD_PUBLIC_FUNC(Player_ResetState);

        ADD_PUBLIC_FUNC(Player_GetHitbox);
        ADD_PUBLIC_FUNC(Player_CheckCollisionTouch);
        ADD_PUBLIC_FUNC(Player_CheckCollisionBox);
        ADD_PUBLIC_FUNC(Player_CheckCollisionPlatform);

        ADD_PUBLIC_FUNC(Player_CheckHit);
        ADD_PUBLIC_FUNC(Player_CheckHitFlip);
        ADD_PUBLIC_FUNC(Player_CheckBadnikHit);
        ADD_PUBLIC_FUNC(Player_CheckBadnikBreak);
        ADD_PUBLIC_FUNC(Player_CheckBossHit);
        ADD_PUBLIC_FUNC(Player_CheckProjectileHit);
        ADD_PUBLIC_FUNC(Player_CheckHit2);

        ADD_PUBLIC_FUNC(DebugMode_AddObject);

        ADD_PUBLIC_FUNC(Ice_State_FrozenPlayer);

        ADD_PUBLIC_FUNC(PlayerHelpers_CheckAct1);
        ADD_PUBLIC_FUNC(PlayerHelpers_CheckAct2);
        ADD_PUBLIC_FUNC(PlayerHelpers_CheckIntro);
        ADD_PUBLIC_FUNC(PlayerHelpers_CheckAct1Regular);
        ADD_PUBLIC_FUNC(PlayerHelpers_CheckStageReload);
        ADD_PUBLIC_FUNC(PlayerHelpers_CheckPlayerPos);

        ADD_PUBLIC_FUNC(GameProgress_GetNotifStringID);
        ADD_PUBLIC_FUNC(GameProgress_ShuffleBSSID);
        ADD_PUBLIC_FUNC(GameProgress_GetGameProgress);
        ADD_PUBLIC_FUNC(GameProgress_GetZoneUnlocked);
        ADD_PUBLIC_FUNC(GameProgress_GetCompletionPercent);
        ADD_PUBLIC_FUNC(GameProgress_TrackGameProgress);
        ADD_PUBLIC_FUNC(GameProgress_UnlockAllMedals);
        ADD_PUBLIC_FUNC(GameProgress_MarkZoneCompleted);
        ADD_PUBLIC_FUNC(GameProgress_CheckZoneClear);
        ADD_PUBLIC_FUNC(GameProgress_GiveEmerald);
        ADD_PUBLIC_FUNC(GameProgress_GiveMedal);
        ADD_PUBLIC_FUNC(GameProgress_GiveEnding);
        ADD_PUBLIC_FUNC(GameProgress_PrintSaveProgress);
        ADD_PUBLIC_FUNC(GameProgress_CountUnreadNotifs);
        ADD_PUBLIC_FUNC(GameProgress_GetNextNotif);
        ADD_PUBLIC_FUNC(GameProgress_CheckUnlock);

        ADD_PUBLIC_FUNC(MathHelpers_Lerp1);
        ADD_PUBLIC_FUNC(MathHelpers_Lerp2);
        ADD_PUBLIC_FUNC(MathHelpers_Lerp3);
        ADD_PUBLIC_FUNC(MathHelpers_Lerp4);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown5);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown6);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown7);
        ADD_PUBLIC_FUNC(MathHelpers_PointInHitbox);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown9);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown10);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown11);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown12);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown13);
        ADD_PUBLIC_FUNC(MathHelpers_Unknown14);

        ADD_PUBLIC_FUNC(Options_Reload);
        ADD_PUBLIC_FUNC(Options_GetWinSize);
        ADD_PUBLIC_FUNC(Options_LoadCallback);
        ADD_PUBLIC_FUNC(Options_LoadOptionsBin);
        ADD_PUBLIC_FUNC(Options_SaveOptionsBin);
        ADD_PUBLIC_FUNC(Options_SetLanguage);
        ADD_PUBLIC_FUNC(Options_Unknown1);
        ADD_PUBLIC_FUNC(Options_LoadOptionsCallback);
        ADD_PUBLIC_FUNC(Options_SaveOptionsCallback);

        ADD_PUBLIC_FUNC(Camera_SetCameraBounds);
        ADD_PUBLIC_FUNC(Camera_SetTargetEntity);
        ADD_PUBLIC_FUNC(Camera_ShakeScreen);
        ADD_PUBLIC_FUNC(Camera_HandleHBounds);
        ADD_PUBLIC_FUNC(Camera_HandleVBounds);
        ADD_PUBLIC_FUNC(Camera_SetupLerp);

        ADD_PUBLIC_FUNC(Camera_State_Roam);
        ADD_PUBLIC_FUNC(Camera_State_Follow);
        ADD_PUBLIC_FUNC(Camera_State_HLock);
        ADD_PUBLIC_FUNC(Camera_State_VLock);
        ADD_PUBLIC_FUNC(Camera_State_HandleLerp);

        ADD_PUBLIC_FUNC(Dust_State_DropDash);
        ADD_PUBLIC_FUNC(Dust_State_HammerDrop);
        ADD_PUBLIC_FUNC(Dust_State_GlideSlide);
        ADD_PUBLIC_FUNC(Dust_State_Skid);
        ADD_PUBLIC_FUNC(Dust_State_Spindash);
        ADD_PUBLIC_FUNC(Dust_State_EggLoco);

        ADD_PUBLIC_FUNC(Debris_State_Fall);
        ADD_PUBLIC_FUNC(Debris_State_LightningSpark);
        ADD_PUBLIC_FUNC(Debris_State_FallAndFlicker);
        ADD_PUBLIC_FUNC(Debris_FallFlickerAnimSetup);
        ADD_PUBLIC_FUNC(Debris_FallFlickerSetup);

        ADD_PUBLIC_FUNC(Music_SetMusicTrack);
        ADD_PUBLIC_FUNC(Music_State_PlayMusic);
        ADD_PUBLIC_FUNC(Music_PlayMusicTrack);
        ADD_PUBLIC_FUNC(Music_PlayTrack);
        ADD_PUBLIC_FUNC(Music_PlayTrackPtr);

        ADD_PUBLIC_FUNC(Soundboard_LoadSFX);

        ADD_PUBLIC_FUNC(Zone_GetZoneID);
        ADD_PUBLIC_FUNC(Zone_StoreEntities);
        ADD_PUBLIC_FUNC(Zone_ReloadStoredEntities);
        ADD_PUBLIC_FUNC(Zone_StartFadeOut);
        ADD_PUBLIC_FUNC(Zone_StartFadeOutRestart);
        ADD_PUBLIC_FUNC(Zone_StartTeleportAction);
        ADD_PUBLIC_FUNC(Zone_ApplyWorldBounds);
        ADD_PUBLIC_FUNC(Zone_IsAct2);
        ADD_PUBLIC_FUNC(Zone_GetEncoreStageID);
        ADD_PUBLIC_FUNC(Zone_GetManiaStageID);
    }
#endif
}

#if RETRO_USE_MOD_LOADER
bool32 LinkModLogic(GameInfo* info, const char* id) {
    LinkGameLogicDLL(info);
    return true;
}
#endif

#if !RETRO_STANDALONE 
int32 RSDK_main(int32 argc, char **argv, void (*linkLogicPtr)(void* info));

int32 SDL_main(int32 argc, char *argv[]) { return RSDK_main(argc, argv, LinkGameLogicDLL); }
#endif
