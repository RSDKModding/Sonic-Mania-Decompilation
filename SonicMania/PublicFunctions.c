#define ADD_PUBLIC_FUNC(func) Mod.AddPublicFunction(#func, (void *)(func))

// NOTE:
// In the end we decided every func in mania is prolly important to *someone*
// so we just wrote up a script to include every single one of em here
// sorry if this lags any mods :)

void InitPublicFunctions()
{
    // AIZ/AIZEncoreTutorial
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(AIZEncoreTutorial_State_ShowTutBubble);
    ADD_PUBLIC_FUNC(AIZEncoreTutorial_State_EnterTutorial);
    ADD_PUBLIC_FUNC(AIZEncoreTutorial_State_ShowSwapTutorial);
    ADD_PUBLIC_FUNC(AIZEncoreTutorial_State_ExitTutorial);
    ADD_PUBLIC_FUNC(AIZEncoreTutorial_State_ReturnToCutscene);
#endif

    // AIZ/AIZKingClaw
    ADD_PUBLIC_FUNC(AIZKingClaw_HandleClawPositions);
    ADD_PUBLIC_FUNC(AIZKingClaw_State_Grab);

    // AIZ/AIZRockPile
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(AIZRockPile_SpawnRocks);
#endif

#if MANIA_USE_PLUS
    // AIZ/AIZSetup
    ADD_PUBLIC_FUNC(AIZSetup_BGSwitch_Jungle);
    ADD_PUBLIC_FUNC(AIZSetup_BGSwitch_Sky);
    ADD_PUBLIC_FUNC(AIZSetup_PlayerState_Static);
#endif
    ADD_PUBLIC_FUNC(AIZSetup_PlayerState_P2Enter);
    ADD_PUBLIC_FUNC(AIZSetup_HandleHeavyMovement);
    ADD_PUBLIC_FUNC(AIZSetup_SetupObjects);
    ADD_PUBLIC_FUNC(AIZSetup_GetCutsceneSetupPtr);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(AIZSetup_Cutscene_SkipCB);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneST_Setup);
#endif
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_EnterAIZ);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_EnterAIZJungle);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_EnterHeavies);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_P2FlyIn);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_EnterClaw);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_WatchClaw);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_RubyGrabbed);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_RubyAppear);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneSonic_RubyFX);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneK_Setup);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_Chillin);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_StartDrillin);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_Drillin);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_PrepareForTrouble);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_EnterThreat);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_HeaviesAppear);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_RubyImpact);
    ADD_PUBLIC_FUNC(AIZSetup_CutsceneKnux_RubyFX);
    ADD_PUBLIC_FUNC(AIZSetup_Cutscene_LoadGHZ);

    // AIZ/AIZTornado
    ADD_PUBLIC_FUNC(AIZTornado_HandleMovement);
    ADD_PUBLIC_FUNC(AIZTornado_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(AIZTornado_State_Move);

    // AIZ/AIZTornadoPath
    ADD_PUBLIC_FUNC(AIZTornadoPath_HandleMoveSpeed);
    ADD_PUBLIC_FUNC(AIZTornadoPath_State_SetTornadoSpeed);
    ADD_PUBLIC_FUNC(AIZTornadoPath_State_SetPlayerCamera);
    ADD_PUBLIC_FUNC(AIZTornadoPath_State_DisablePlayerInteractions);
    ADD_PUBLIC_FUNC(AIZTornadoPath_State_ExitTornadoSequence);
    ADD_PUBLIC_FUNC(AIZTornadoPath_State_PrepareCatchPlayer);
    ADD_PUBLIC_FUNC(AIZTornadoPath_State_CatchPlayer);

    // AIZ/Bloominator
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Bloominator_DebugSpawn);
    ADD_PUBLIC_FUNC(Bloominator_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Bloominator_CheckOffScreen);
    ADD_PUBLIC_FUNC(Bloominator_State_Idle);
    ADD_PUBLIC_FUNC(Bloominator_State_Firing);
    ADD_PUBLIC_FUNC(Bloominator_State_Init);
    ADD_PUBLIC_FUNC(Bloominator_State_Spikeball);
#endif

    // AIZ/EncoreIntro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(EncoreIntro_SetupEntities);
    ADD_PUBLIC_FUNC(EncoreIntro_SetupCutscene);
    ADD_PUBLIC_FUNC(EncoreIntro_SetupCutscenePart2);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_SetupAIZEncore);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_PlayerAppear);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_RubyAppear);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_PortalClose);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_Empty);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_BeginAIZEncore);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_AIZEncore);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_CapsuleFound);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_BuddySelect);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_ViewEncoreTutorial);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_MysticGetRuby);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_MysticStealRuby);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_MysticEscape);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_AIZEncoreTutorial);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_CameraPanToHBHPile);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_MysticPassRuby);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_KingActivate);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_RubyActivated);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_RubyWarp);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_LoadGHZ);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_AwaitSaveFinish);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_FadeOutAndReset);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_FadeInAndStart);
    ADD_PUBLIC_FUNC(EncoreIntro_Cutscene_SkipAndFadeOut);
    ADD_PUBLIC_FUNC(EncoreIntro_SaveGameCB);
    ADD_PUBLIC_FUNC(EncoreIntro_PhantomRuby_OscillateFX);
    ADD_PUBLIC_FUNC(EncoreIntro_PhantomRuby_EscapeRight);
    ADD_PUBLIC_FUNC(EncoreIntro_PhantomRuby_Fall);
    ADD_PUBLIC_FUNC(EncoreIntro_PhantomRuby_CapsuleRiseUp);
    ADD_PUBLIC_FUNC(EncoreIntro_PhantomRuby_CapsuleFallDown);
    ADD_PUBLIC_FUNC(EncoreIntro_PlayerState_BuddySel);
    ADD_PUBLIC_FUNC(EncoreIntro_PlayerState_HandleAir);
    ADD_PUBLIC_FUNC(EncoreIntro_PlayerInput_BuddySel);
#endif

    // AIZ/MonkeyDude
    ADD_PUBLIC_FUNC(MonkeyDude_DebugSpawn);
    ADD_PUBLIC_FUNC(MonkeyDude_State_Init);
    ADD_PUBLIC_FUNC(MonkeyDude_HandleBodyPart);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MonkeyDude_HandleStates);
#endif
    ADD_PUBLIC_FUNC(MonkeyDude_State_Laugh);
    ADD_PUBLIC_FUNC(MonkeyDude_State_MoveArm);
    ADD_PUBLIC_FUNC(MonkeyDude_State_MoveBody);
    ADD_PUBLIC_FUNC(MonkeyDude_StateBody_ArmRaise);
    ADD_PUBLIC_FUNC(MonkeyDude_StateBody_Throw);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MonkeyDude_State_Coconut);
#endif

    // AIZ/Rhinobot
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Rhinobot_DebugSpawn);
    ADD_PUBLIC_FUNC(Rhinobot_CheckTileCollisions);
    ADD_PUBLIC_FUNC(Rhinobot_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Rhinobot_CheckOffScreen);
    ADD_PUBLIC_FUNC(Rhinobot_Delay_Charge);
    ADD_PUBLIC_FUNC(Rhinobot_Delay_Skidding);
    ADD_PUBLIC_FUNC(Rhinobot_Delay_SkidFinish);
    ADD_PUBLIC_FUNC(Rhinobot_State_Init);
    ADD_PUBLIC_FUNC(Rhinobot_State_Moving);
    ADD_PUBLIC_FUNC(Rhinobot_State_Skidding);
    ADD_PUBLIC_FUNC(Rhinobot_State_Idle);
    ADD_PUBLIC_FUNC(Rhinobot_State_Fall);
#endif

    // AIZ/SchrodingersCapsule
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(SchrodingersCapsule_State_Init);
    ADD_PUBLIC_FUNC(SchrodingersCapsule_State_HandleBounds);
    ADD_PUBLIC_FUNC(SchrodingersCapsule_State_Activated);
    ADD_PUBLIC_FUNC(SchrodingersCapsule_State_Explode);
    ADD_PUBLIC_FUNC(SchrodingersCapsule_State_SetupActClear);
#endif

    // AIZ/Sweep
    ADD_PUBLIC_FUNC(Sweep_DebugSpawn);
    ADD_PUBLIC_FUNC(Sweep_CheckOffScreen);
    ADD_PUBLIC_FUNC(Sweep_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Sweep_CheckShoot);
    ADD_PUBLIC_FUNC(Sweep_State_Init);
    ADD_PUBLIC_FUNC(Sweep_State_Idle);
    ADD_PUBLIC_FUNC(Sweep_State_Dash);
    ADD_PUBLIC_FUNC(Sweep_State_Stop);
    ADD_PUBLIC_FUNC(Sweep_State_FiredShot);
    ADD_PUBLIC_FUNC(Sweep_State_Turn);
    ADD_PUBLIC_FUNC(Sweep_State_Projectile);

    // BSS/BSS_HUD
    ADD_PUBLIC_FUNC(BSS_HUD_DrawNumbers);

    // BSS/BSS_Message
    ADD_PUBLIC_FUNC(BSS_Message_State_GetBS);
    ADD_PUBLIC_FUNC(BSS_Message_State_GetBSWait);
    ADD_PUBLIC_FUNC(BSS_Message_State_Finished);
    ADD_PUBLIC_FUNC(BSS_Message_State_Perfect);
    ADD_PUBLIC_FUNC(BSS_Message_State_WaitPerfect);
    ADD_PUBLIC_FUNC(BSS_Message_State_MsgFinished);
    ADD_PUBLIC_FUNC(BSS_Message_TrackProgressCB);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(BSS_Message_State_SaveGameProgress);
#endif
    ADD_PUBLIC_FUNC(BSS_Message_State_LoadPrevScene);

    // BSS/BSS_Player
    ADD_PUBLIC_FUNC(BSS_Player_Input_P1);
    ADD_PUBLIC_FUNC(BSS_Player_Input_P2);

    // BSS/BSS_Setup
    ADD_PUBLIC_FUNC(BSS_Setup_GetStageID);
    ADD_PUBLIC_FUNC(BSS_Setup_SetupPalette);
    ADD_PUBLIC_FUNC(BSS_Setup_SetupFrustum);
    ADD_PUBLIC_FUNC(BSS_Setup_CollectRing);
    ADD_PUBLIC_FUNC(BSS_Setup_GetStartupInfo);
    ADD_PUBLIC_FUNC(BSS_Setup_State_GlobeJettison);
    ADD_PUBLIC_FUNC(BSS_Setup_HandleSteppedObjects);
    ADD_PUBLIC_FUNC(BSS_Setup_HandleCollectableMovement);
    ADD_PUBLIC_FUNC(BSS_Setup_State_GlobeEmerald);
    ADD_PUBLIC_FUNC(BSS_Setup_State_StartGlobeTeleport);
    ADD_PUBLIC_FUNC(BSS_Setup_State_GlobeExit);
    ADD_PUBLIC_FUNC(BSS_Setup_State_GlobeMoveZ);
    ADD_PUBLIC_FUNC(BSS_Setup_State_GlobeTurnLeft);
    ADD_PUBLIC_FUNC(BSS_Setup_State_GlobeTurnRight);
    ADD_PUBLIC_FUNC(BSS_Setup_State_FinishGlobeTeleport);
    ADD_PUBLIC_FUNC(BSS_Setup_CheckSphereValid);
    ADD_PUBLIC_FUNC(BSS_Setup_LaunchSpheres);
    ADD_PUBLIC_FUNC(BSS_Setup_SetupFinishSequence);
    ADD_PUBLIC_FUNC(BSS_Setup_ScanSphereChain_Up);
    ADD_PUBLIC_FUNC(BSS_Setup_ScanSphereChain_Down);
    ADD_PUBLIC_FUNC(BSS_Setup_ScanSphereChain_Left);
    ADD_PUBLIC_FUNC(BSS_Setup_ScanSphereChain_Right);
    ADD_PUBLIC_FUNC(BSS_Setup_GetChainedSphereCount);
    ADD_PUBLIC_FUNC(BSS_Setup_ProcessChain);

    // Common/BreakableWall
    ADD_PUBLIC_FUNC(BreakableWall_State_Tile);
    ADD_PUBLIC_FUNC(BreakableWall_State_FallingTile);
    ADD_PUBLIC_FUNC(BreakableWall_State_Wall);
    ADD_PUBLIC_FUNC(BreakableWall_State_Floor);
    ADD_PUBLIC_FUNC(BreakableWall_State_BurrowFloor);
    ADD_PUBLIC_FUNC(BreakableWall_State_BurrowFloorUp);
    ADD_PUBLIC_FUNC(BreakableWall_State_Ceiling);
    ADD_PUBLIC_FUNC(BreakableWall_Draw_Wall);
    ADD_PUBLIC_FUNC(BreakableWall_Draw_Floor);
    ADD_PUBLIC_FUNC(BreakableWall_Draw_Tile);
    ADD_PUBLIC_FUNC(BreakableWall_CheckBreak_Wall);
    ADD_PUBLIC_FUNC(BreakableWall_CheckBreak_Floor);
    ADD_PUBLIC_FUNC(BreakableWall_CheckBreak_BurrowFloor);
    ADD_PUBLIC_FUNC(BreakableWall_CheckBreak_BurrowFloorUp);
    ADD_PUBLIC_FUNC(BreakableWall_CheckBreak_Ceiling);
    ADD_PUBLIC_FUNC(BreakableWall_Break);
    ADD_PUBLIC_FUNC(BreakableWall_GiveScoreBonus);

    // Common/Button
    ADD_PUBLIC_FUNC(Button_CheckEggmanCollisions);
    ADD_PUBLIC_FUNC(Button_CheckPRiderCollisions);
    ADD_PUBLIC_FUNC(Button_HandleFloor);
    ADD_PUBLIC_FUNC(Button_HandleRoof);
    ADD_PUBLIC_FUNC(Button_HandleRWall);
    ADD_PUBLIC_FUNC(Button_HandleLWall);

    // Common/CollapsingPlatform
    ADD_PUBLIC_FUNC(CollapsingPlatform_State_Left);
    ADD_PUBLIC_FUNC(CollapsingPlatform_State_Right);
    ADD_PUBLIC_FUNC(CollapsingPlatform_State_Center);
    ADD_PUBLIC_FUNC(CollapsingPlatform_State_LeftRight);
    ADD_PUBLIC_FUNC(CollapsingPlatform_State_LeftRightCenter);

    // Common/Decoration
    ADD_PUBLIC_FUNC(Decoration_DrawSprite);

    // Common/Eggman
    ADD_PUBLIC_FUNC(Eggman_State_ProcessAnimation);
    ADD_PUBLIC_FUNC(Eggman_State_ProcessThenSet);
    ADD_PUBLIC_FUNC(Eggman_State_ProcessUntilEnd);
    ADD_PUBLIC_FUNC(Eggman_State_ProcessAirThenSet);
    ADD_PUBLIC_FUNC(Eggman_State_FallUntilTimerReset);
    ADD_PUBLIC_FUNC(Eggman_State_FallAndCollide);
    ADD_PUBLIC_FUNC(Eggman_State_WalkOffScreen);

    // Common/ForceSpin
    ADD_PUBLIC_FUNC(ForceSpin_DrawSprites);
    ADD_PUBLIC_FUNC(ForceSpin_SetPlayerState);

    // Common/ForceUnstick
    ADD_PUBLIC_FUNC(ForceUnstick_DrawSprites);

    // Common/ParallaxSprite
    ADD_PUBLIC_FUNC(ParallaxSprite_State_Normal);
    ADD_PUBLIC_FUNC(ParallaxSprite_State_Emitter);
    ADD_PUBLIC_FUNC(ParallaxSprite_State_Particle);
    ADD_PUBLIC_FUNC(ParallaxSprite_State_FadeIntoHalf);
    ADD_PUBLIC_FUNC(ParallaxSprite_State_FadeOut);

    // Common/Platform
    ADD_PUBLIC_FUNC(Platform_Collision_Platform);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid_NoCrush);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid_Hurt_Sides);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid_Hurt_Bottom);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid_Hurt_Top);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid_Hold);
    ADD_PUBLIC_FUNC(Platform_Collision_Solid_Barrel);
    ADD_PUBLIC_FUNC(Platform_Collision_Sticky);
    ADD_PUBLIC_FUNC(Platform_Collision_Tiles);
    ADD_PUBLIC_FUNC(Platform_Collision_Hurt);
    ADD_PUBLIC_FUNC(Platform_Collision_None);
    ADD_PUBLIC_FUNC(Platform_State_Fixed);
    ADD_PUBLIC_FUNC(Platform_State_Fall);
    ADD_PUBLIC_FUNC(Platform_State_Falling);
    ADD_PUBLIC_FUNC(Platform_State_Falling2);
    ADD_PUBLIC_FUNC(Platform_State_Hold);
    ADD_PUBLIC_FUNC(Platform_State_Linear);
    ADD_PUBLIC_FUNC(Platform_State_Circular);
    ADD_PUBLIC_FUNC(Platform_State_Swing);
    ADD_PUBLIC_FUNC(Platform_State_Clacker);
    ADD_PUBLIC_FUNC(Platform_State_Push_Init);
    ADD_PUBLIC_FUNC(Platform_State_Push);
    ADD_PUBLIC_FUNC(Platform_State_Push_SlideOffL);
    ADD_PUBLIC_FUNC(Platform_State_Push_SlideOffR);
    ADD_PUBLIC_FUNC(Platform_State_Push_Fall);
    ADD_PUBLIC_FUNC(Platform_State_Path);
    ADD_PUBLIC_FUNC(Platform_State_PathStop);
    ADD_PUBLIC_FUNC(Platform_State_Track);
    ADD_PUBLIC_FUNC(Platform_State_React);
    ADD_PUBLIC_FUNC(Platform_State_Hover_React);
    ADD_PUBLIC_FUNC(Platform_State_ReactMove);
    ADD_PUBLIC_FUNC(Platform_State_ReactSlow);
    ADD_PUBLIC_FUNC(Platform_State_Hover);
    ADD_PUBLIC_FUNC(Platform_State_ReactWait);
    ADD_PUBLIC_FUNC(Platform_State_DoorSlide);
    ADD_PUBLIC_FUNC(Platform_State_PathReact);
    ADD_PUBLIC_FUNC(Platform_State_Child);
    ADD_PUBLIC_FUNC(Platform_State_SwingReact);
    ADD_PUBLIC_FUNC(Platform_State_Swing2);
    ADD_PUBLIC_FUNC(Platform_State_SwingWait);
    ADD_PUBLIC_FUNC(Platform_State_SwingReturn);
    ADD_PUBLIC_FUNC(Platform_State_TrackReact);
    ADD_PUBLIC_FUNC(Platform_State_Track2);
    ADD_PUBLIC_FUNC(Platform_State_TrackWait);
    ADD_PUBLIC_FUNC(Platform_State_TrackReturn);
    ADD_PUBLIC_FUNC(Platform_State_DipRock);

    // Common/PlatformControl
    ADD_PUBLIC_FUNC(PlatformControl_ManagePlatformVelocity);

    // Common/Projectile
    ADD_PUBLIC_FUNC(Projectile_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Projectile_State_Move);
    ADD_PUBLIC_FUNC(Projectile_State_MoveGravity);

    // Common/SpinBooster
    ADD_PUBLIC_FUNC(SpinBooster_GetRollDir);
    ADD_PUBLIC_FUNC(SpinBooster_HandleRollDir);
    ADD_PUBLIC_FUNC(SpinBooster_ApplyRollVelocity);
    ADD_PUBLIC_FUNC(SpinBooster_DrawArrow);
    ADD_PUBLIC_FUNC(SpinBooster_DrawSprites);
    ADD_PUBLIC_FUNC(SpinBooster_HandleForceRoll);

    // Common/Water
    ADD_PUBLIC_FUNC(Water_DrawHook_ApplyWaterPalette);
    ADD_PUBLIC_FUNC(Water_DrawHook_RemoveWaterPalette);
    ADD_PUBLIC_FUNC(Water_SetupTagLink);
    ADD_PUBLIC_FUNC(Water_SpawnBubble);
    ADD_PUBLIC_FUNC(Water_SpawnCountDownBubble);
    ADD_PUBLIC_FUNC(Water_GetPlayerBubble);
    ADD_PUBLIC_FUNC(Water_HandleBubbleMovement);
    ADD_PUBLIC_FUNC(Water_State_BtnBigBubble);
    ADD_PUBLIC_FUNC(Water_PopBigBubble);
    ADD_PUBLIC_FUNC(Water_State_Water);
    ADD_PUBLIC_FUNC(Water_State_Pool);
    ADD_PUBLIC_FUNC(Water_State_Splash);
    ADD_PUBLIC_FUNC(Water_State_Bubble);
    ADD_PUBLIC_FUNC(Water_State_BubbleBreathed);
    ADD_PUBLIC_FUNC(Water_State_BigBubble);
    ADD_PUBLIC_FUNC(Water_State_Bubbler);
    ADD_PUBLIC_FUNC(Water_State_Countdown);
    ADD_PUBLIC_FUNC(Water_State_CountdownFollow);
    ADD_PUBLIC_FUNC(Water_State_HeightTrigger);
    ADD_PUBLIC_FUNC(Water_Draw_Water);
    ADD_PUBLIC_FUNC(Water_Draw_Pool);
    ADD_PUBLIC_FUNC(Water_Draw_Splash);
    ADD_PUBLIC_FUNC(Water_Draw_Countdown);
    ADD_PUBLIC_FUNC(Water_Draw_Bubbler);
    ADD_PUBLIC_FUNC(Water_Draw_Bubble);

    // Continue/ContinuePlayer
    ADD_PUBLIC_FUNC(ContinuePlayer_HandleDashAnim);
    ADD_PUBLIC_FUNC(ContinuePlayer_State_Idle);
    ADD_PUBLIC_FUNC(ContinuePlayer_State_ChargeDash);
    ADD_PUBLIC_FUNC(ContinuePlayer_State_DashRelease);

    // Continue/ContinueSetup
    ADD_PUBLIC_FUNC(ContinueSetup_State_FadeIn);
    ADD_PUBLIC_FUNC(ContinueSetup_State_HandleCountdown);
    ADD_PUBLIC_FUNC(ContinueSetup_State_ContinueGame);
    ADD_PUBLIC_FUNC(ContinueSetup_State_ReturnToMenu);

    // CPZ/AmoebaDroid
    ADD_PUBLIC_FUNC(AmoebaDroid_HandleSmallBlobMovement);
    ADD_PUBLIC_FUNC(AmoebaDroid_HandleSmallBlobRelease);
    ADD_PUBLIC_FUNC(AmoebaDroid_Hit);
    ADD_PUBLIC_FUNC(AmoebaDroid_Explode);
    ADD_PUBLIC_FUNC(AmoebaDroid_CheckHit);
    ADD_PUBLIC_FUNC(AmoebaDroid_CheckPlayerHit);
    ADD_PUBLIC_FUNC(AmoebaDroid_Draw_AmoebaDroid);
    ADD_PUBLIC_FUNC(AmoebaDroid_Draw_BigBlob);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SetupArena);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SetupWaterLevel);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_DropIn);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_DropIntoPool);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SurfaceFromPool);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_ChooseAttack);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SwimLeft);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SwimRight);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_ExitPool);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_BounceAttack);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_GatherBlobs);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SpinBlobs);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_BigBlob);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_SmallBlob);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_BigBlob_Disappear);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_PoolSplash_Delayed);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_PoolSplash);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_Destroyed);
    ADD_PUBLIC_FUNC(AmoebaDroid_State_DropSignPost);

    // CPZ/Ball
    ADD_PUBLIC_FUNC(Ball_DebugSpawn);
    ADD_PUBLIC_FUNC(Ball_HandleInteractions);
    ADD_PUBLIC_FUNC(Ball_CheckOffScreen);
    ADD_PUBLIC_FUNC(Ball_SpawnSplashes);
    ADD_PUBLIC_FUNC(Ball_State_Init);
    ADD_PUBLIC_FUNC(Ball_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(Ball_State_TargetingPlayer);
    ADD_PUBLIC_FUNC(Ball_State_ChemicalDrop);
    ADD_PUBLIC_FUNC(Ball_State_Splash);
    ADD_PUBLIC_FUNC(Ball_State_StraightMovement);
    ADD_PUBLIC_FUNC(Ball_State_Spawner);

    // CPZ/Bubbler
    ADD_PUBLIC_FUNC(Bubbler_DebugSpawn);
    ADD_PUBLIC_FUNC(Bubbler_HandleInteractions);
    ADD_PUBLIC_FUNC(Bubbler_HandleProjectileInteractions);
    ADD_PUBLIC_FUNC(Bubbler_CheckOffScreen);
    ADD_PUBLIC_FUNC(Bubbler_State_Init);
    ADD_PUBLIC_FUNC(Bubbler_State_MotherPatrol);
    ADD_PUBLIC_FUNC(Bubbler_State_FoundPlayer);
    ADD_PUBLIC_FUNC(Bubbler_State_AttackPlayer);
    ADD_PUBLIC_FUNC(Bubbler_StateProjectile_Seed);
    ADD_PUBLIC_FUNC(Bubbler_StateProjectile_Bubbler);

    // CPZ/CaterkillerJr
    ADD_PUBLIC_FUNC(CaterkillerJr_DebugSpawn);
    ADD_PUBLIC_FUNC(CaterkillerJr_SetupPositions);
    ADD_PUBLIC_FUNC(CaterkillerJr_State_SetupVelocities);
    ADD_PUBLIC_FUNC(CaterkillerJr_State_Move);

    // CPZ/ChemBubble
    ADD_PUBLIC_FUNC(ChemBubble_State_Appear);
    ADD_PUBLIC_FUNC(ChemBubble_State_Rising);
    ADD_PUBLIC_FUNC(ChemBubble_State_Surfaced);

    // CPZ/ChemicalBall
    ADD_PUBLIC_FUNC(ChemicalBall_CheckHit);
    ADD_PUBLIC_FUNC(ChemicalBall_State_MoveArc);
    ADD_PUBLIC_FUNC(ChemicalBall_State_MoveVertical);

    // CPZ/ChemicalPool
    ADD_PUBLIC_FUNC(ChemicalPool_ChangeState);
    ADD_PUBLIC_FUNC(ChemicalPool_ProcessDeformations);
    ADD_PUBLIC_FUNC(ChemicalPool_SetDeform);
    ADD_PUBLIC_FUNC(ChemicalPool_SpawnDebris);
    ADD_PUBLIC_FUNC(ChemicalPool_SetupColors);
    ADD_PUBLIC_FUNC(ChemicalPool_State_HarmfulBlue);
    ADD_PUBLIC_FUNC(ChemicalPool_State_Green);
    ADD_PUBLIC_FUNC(ChemicalPool_State_Cyan);
    ADD_PUBLIC_FUNC(ChemicalPool_State_Changing);

    // CPZ/CPZ1Intro
    ADD_PUBLIC_FUNC(CPZ1Intro_Particle_ChemDrop);
    ADD_PUBLIC_FUNC(CPZ1Intro_HandleRubyHover);
    ADD_PUBLIC_FUNC(CPZ1Intro_CheckSonicAnimFinish);
    ADD_PUBLIC_FUNC(CPZ1Intro_CheckTailsAnimFinish);
    ADD_PUBLIC_FUNC(CPZ1Intro_CheckKnuxAnimFinish);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(CPZ1Intro_CheckMightyAnimFinish);
    ADD_PUBLIC_FUNC(CPZ1Intro_CheckRayAnimFinish);
    ADD_PUBLIC_FUNC(CPZ1Intro_Cutscene_RubyWarp);
#endif
    ADD_PUBLIC_FUNC(CPZ1Intro_Cutscene_PostWarpDrop);
    ADD_PUBLIC_FUNC(CPZ1Intro_Cutscene_Waiting);
    ADD_PUBLIC_FUNC(CPZ1Intro_Cutscene_ChemicalDrop);
    ADD_PUBLIC_FUNC(CPZ1Intro_Cutscene_PlayerChemicalReact);
    ADD_PUBLIC_FUNC(CPZ1Intro_Cutscene_ReadyStage);

    // CPZ/CPZ2Outro
    ADD_PUBLIC_FUNC(CPZ2Outro_SetupCutscene);
    ADD_PUBLIC_FUNC(CPZ2Outro_Cutscene_Outro);

    // CPZ/CPZBoss
    ADD_PUBLIC_FUNC(CPZBoss_DrawHook_SetupPuyoHUD);
    ADD_PUBLIC_FUNC(CPZBoss_DrawHook_RemovePuyoHUD);
    ADD_PUBLIC_FUNC(CPZBoss_Explode_Eggman);
    ADD_PUBLIC_FUNC(CPZBoss_Explode_Player);
    ADD_PUBLIC_FUNC(CPZBoss_CheckMatchReset);
    ADD_PUBLIC_FUNC(CPZBoss_State_SetupArena);
    ADD_PUBLIC_FUNC(CPZBoss_State_EnterPlayer);
    ADD_PUBLIC_FUNC(CPZBoss_State_CheckPlayerReady);
    ADD_PUBLIC_FUNC(CPZBoss_State_PlayPlayerEnterAnim);
    ADD_PUBLIC_FUNC(CPZBoss_State_SetupMatch);
    ADD_PUBLIC_FUNC(CPZBoss_State_HandleMatch_Player);
    ADD_PUBLIC_FUNC(CPZBoss_State_HandleMatch_Eggman);
    ADD_PUBLIC_FUNC(CPZBoss_State_HandleMatchFinish_EggmanLose);
    ADD_PUBLIC_FUNC(CPZBoss_State_PlayerWin);
    ADD_PUBLIC_FUNC(CPZBoss_State_HandleMatchFinish_PlayerLose);
    ADD_PUBLIC_FUNC(CPZBoss_State_EggmanFall);
    ADD_PUBLIC_FUNC(CPZBoss_State_PlayerExit);
    ADD_PUBLIC_FUNC(CPZBoss_State_Destroyed);

    // CPZ/CPZSetup
    ADD_PUBLIC_FUNC(CPZSetup_BGSwitch_Act2BG);
    ADD_PUBLIC_FUNC(CPZSetup_BGSwitch_Act1BG);
    ADD_PUBLIC_FUNC(CPZSetup_StageFinish_EndAct1);
    ADD_PUBLIC_FUNC(CPZSetup_StageFinish_EndAct2);

    // CPZ/CPZShutter
    ADD_PUBLIC_FUNC(CPZShutter_State_Open);
    ADD_PUBLIC_FUNC(CPZShutter_State_Close);

    // CPZ/DNARiser
    ADD_PUBLIC_FUNC(DNARiser_State_BubbleBurst);
    ADD_PUBLIC_FUNC(DNARiser_SetupBurst);
    ADD_PUBLIC_FUNC(DNARiser_CalculateScale);
    ADD_PUBLIC_FUNC(DNARiser_State_Init);
    ADD_PUBLIC_FUNC(DNARiser_State_HandleInteractions);
    ADD_PUBLIC_FUNC(DNARiser_State_HelixRise);
    ADD_PUBLIC_FUNC(DNARiser_State_HelixBurst);
    ADD_PUBLIC_FUNC(DNARiser_State_ResetRiser);
    ADD_PUBLIC_FUNC(DNARiser_State_OrbSetup);
    ADD_PUBLIC_FUNC(DNARiser_State_OrbIdle);
    ADD_PUBLIC_FUNC(DNARiser_State_OrbFall);
    ADD_PUBLIC_FUNC(DNARiser_Draw_Main);
    ADD_PUBLIC_FUNC(DNARiser_Draw_Helix);

    // CPZ/Grabber
    ADD_PUBLIC_FUNC(Grabber_DebugSpawn);
    ADD_PUBLIC_FUNC(Grabber_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Grabber_CheckOffScreen);
    ADD_PUBLIC_FUNC(Grabber_HandleExplode);
    ADD_PUBLIC_FUNC(Grabber_State_Init);
    ADD_PUBLIC_FUNC(Grabber_State_CheckForGrab);
    ADD_PUBLIC_FUNC(Grabber_State_GrabDelay);
    ADD_PUBLIC_FUNC(Grabber_State_HandleTurn);
    ADD_PUBLIC_FUNC(Grabber_State_TryToGrab);
    ADD_PUBLIC_FUNC(Grabber_State_RiseUp);
    ADD_PUBLIC_FUNC(Grabber_State_GrabbedPlayer);
    ADD_PUBLIC_FUNC(Grabber_State_Struggle);
    ADD_PUBLIC_FUNC(Grabber_State_PlayerEscaped);

    // CPZ/OneWayDoor
    ADD_PUBLIC_FUNC(OneWayDoor_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(OneWayDoor_State_MoveDown);
    ADD_PUBLIC_FUNC(OneWayDoor_State_MoveUp);
    ADD_PUBLIC_FUNC(OneWayDoor_Draw_MMZ);
    ADD_PUBLIC_FUNC(OneWayDoor_Draw_CPZ);

    // CPZ/Reagent
    ADD_PUBLIC_FUNC(Reagent_State_CheckPoolCollisions);
    ADD_PUBLIC_FUNC(Reagent_State_ChangingPoolType);

    // CPZ/RotatingStair
    ADD_PUBLIC_FUNC(RotatingStair_State_Move);
    ADD_PUBLIC_FUNC(RotatingStair_State_Move_Intervals);

    // CPZ/SpeedBooster
    ADD_PUBLIC_FUNC(SpeedBooster_DebugSpawn);
    ADD_PUBLIC_FUNC(SpeedBooster_DebugDraw);
    ADD_PUBLIC_FUNC(SpeedBooster_State_SpeedBooster);
    ADD_PUBLIC_FUNC(SpeedBooster_HandleInteractions);
    ADD_PUBLIC_FUNC(SpeedBooster_State_SSZFire);
    ADD_PUBLIC_FUNC(SpeedBooster_State_SSZRecoil);
    ADD_PUBLIC_FUNC(SpeedBooster_State_SSZRetract);
    ADD_PUBLIC_FUNC(SpeedBooster_State_SSZBullet);

    // CPZ/Spiny
    ADD_PUBLIC_FUNC(Spiny_DebugSpawn);
    ADD_PUBLIC_FUNC(Spiny_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Spiny_CheckOffScreen);
    ADD_PUBLIC_FUNC(Spiny_State_Init);
    ADD_PUBLIC_FUNC(Spiny_State_Floor);
    ADD_PUBLIC_FUNC(Spiny_State_Shoot_Floor);
    ADD_PUBLIC_FUNC(Spiny_State_Wall);
    ADD_PUBLIC_FUNC(Spiny_State_Shoot_Wall);
    ADD_PUBLIC_FUNC(Spiny_State_Shot);
    ADD_PUBLIC_FUNC(Spiny_State_ShotDisappear);

    // CPZ/Springboard
    ADD_PUBLIC_FUNC(Springboard_DebugSpawn);

    // CPZ/Staircase
    ADD_PUBLIC_FUNC(Staircase_SfxCheck_HitBlocks);
    ADD_PUBLIC_FUNC(Staircase_State_Idle);
    ADD_PUBLIC_FUNC(Staircase_State_Wait);
    ADD_PUBLIC_FUNC(Staircase_State_MoveBlocks);
    ADD_PUBLIC_FUNC(Staircase_State_MovedBlocks);
    ADD_PUBLIC_FUNC(Staircase_Draw_Blocks);
    ADD_PUBLIC_FUNC(Staircase_Draw_Shake);

    // CPZ/StickyPlatform
    ADD_PUBLIC_FUNC(StickyPlatform_Interact);
    ADD_PUBLIC_FUNC(StickyPlatform_State_HandleMovement);
    ADD_PUBLIC_FUNC(StickyPlatform_State_MoveBack);
    ADD_PUBLIC_FUNC(StickyPlatform_State_MoveBackForth);
    ADD_PUBLIC_FUNC(StickyPlatform_State_Oscillating);

    // CPZ/TippingPlatform
    ADD_PUBLIC_FUNC(TippingPlatform_State_Tipping_Boss);
    ADD_PUBLIC_FUNC(TippingPlatform_State_RestorePlatform);
    ADD_PUBLIC_FUNC(TippingPlatform_State_Tipping);
    ADD_PUBLIC_FUNC(TippingPlatform_State_Restore);
    ADD_PUBLIC_FUNC(TippingPlatform_State_Tipping_Delay);

    // CPZ/TransportTube
    ADD_PUBLIC_FUNC(TransportTube_SetupDirections);
    ADD_PUBLIC_FUNC(TransportTube_HandleVelocityChange);
    ADD_PUBLIC_FUNC(TransportTube_State_ChangeDir);
    ADD_PUBLIC_FUNC(TransportTube_State_Entry);
    ADD_PUBLIC_FUNC(TransportTube_State_ToTargetEntity);
    ADD_PUBLIC_FUNC(TransportTube_State_TargetSeqNode);
    ADD_PUBLIC_FUNC(TransportTube_State_ChooseDir);
    ADD_PUBLIC_FUNC(TransportTube_State_Exit);

    // CPZ/TubeSpring
    ADD_PUBLIC_FUNC(TubeSpring_State_Idle);
    ADD_PUBLIC_FUNC(TubeSpring_State_Springing);
    ADD_PUBLIC_FUNC(TubeSpring_State_Pullback);
    ADD_PUBLIC_FUNC(TubeSpring_HandleInteractions);

    // CPZ/TwistedTubes
    ADD_PUBLIC_FUNC(TwistedTubes_State_HandleInteractions);
    ADD_PUBLIC_FUNC(TwistedTubes_State_PlayerEntryL);
    ADD_PUBLIC_FUNC(TwistedTubes_State_PlayerEntryR);
    ADD_PUBLIC_FUNC(TwistedTubes_State_FirstLoopR);
    ADD_PUBLIC_FUNC(TwistedTubes_State_TubeLoops);
    ADD_PUBLIC_FUNC(TwistedTubes_State_FirstLoopL);
    ADD_PUBLIC_FUNC(TwistedTubes_State_ExitL);
    ADD_PUBLIC_FUNC(TwistedTubes_State_ExitR);

    // Credits/CreditsSetup
    ADD_PUBLIC_FUNC(CreditsSetup_LoadCreditsStrings);

    // Credits/EncoreGoodEnd
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(EncoreGoodEnd_SetupDecorations);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_SetupPlayer);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_StatePlayer_MoveToPos);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_StatePlayer_EndingIdle);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_StatePlayer_EndingSonic);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_MoveToPlace);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_WaitForMovementFinish);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_ClinkGlasses);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_KingAppear);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_ThanksForPlaying);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_FinishCutscene);
    ADD_PUBLIC_FUNC(EncoreGoodEnd_Cutscene_SkipCB);
#endif

    // Credits/TAEmerald
    ADD_PUBLIC_FUNC(TAEmerald_State_Oscillate);
    ADD_PUBLIC_FUNC(TAEmerald_State_MoveCircle);

    // Credits/TryAgain
    ADD_PUBLIC_FUNC(TryAgain_State_Init);
    ADD_PUBLIC_FUNC(TryAgain_State_EnterEggman);
    ADD_PUBLIC_FUNC(TryAgain_State_EggmanLaugh);
    ADD_PUBLIC_FUNC(TryAgain_State_Stinger);

    // Credits/TryAgainE
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TryAgainE_SetupEmeralds);
    ADD_PUBLIC_FUNC(TryAgainE_State_Stinger);
#endif

    // Cutscene/ChaosEmerald
    ADD_PUBLIC_FUNC(ChaosEmerald_State_None);
    ADD_PUBLIC_FUNC(ChaosEmerald_State_Rotate);

    // Cutscene/CutsceneHBH
    ADD_PUBLIC_FUNC(CutsceneHBH_SetupColors);
    ADD_PUBLIC_FUNC(CutsceneHBH_SetupPalettes);
    ADD_PUBLIC_FUNC(CutsceneHBH_StorePalette);
    ADD_PUBLIC_FUNC(CutsceneHBH_RestorePalette);
    ADD_PUBLIC_FUNC(CutsceneHBH_LoadSprites);
    ADD_PUBLIC_FUNC(CutsceneHBH_GetEntity);
    ADD_PUBLIC_FUNC(CutsceneHBH_ShinobiJumpSetup);
    ADD_PUBLIC_FUNC(CutsceneHBH_ShinobiBounceSetup);
    ADD_PUBLIC_FUNC(CutsceneHBH_RiderSetup);
    ADD_PUBLIC_FUNC(CutsceneHBH_KingSetup);
    ADD_PUBLIC_FUNC(CutsceneHBH_KingTMZ2Setup);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_GunnerExit);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_ShinobiBounce);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_ShinobiJump);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_MysticExit);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_RiderMove);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_RiderExit);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_KingExit);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_KingFall);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_KingTMZ2_Fall);
    ADD_PUBLIC_FUNC(CutsceneHBH_State_KingTMZ2_Land);

    // Cutscene/CutsceneRules
    ADD_PUBLIC_FUNC(CutsceneRules_SetupEntity);
    ADD_PUBLIC_FUNC(CutsceneRules_DrawCutsceneBounds);

    // Cutscene/CutsceneSeq
    ADD_PUBLIC_FUNC(CutsceneSeq_NewState);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(CutsceneSeq_CheckSkip);
#endif
    ADD_PUBLIC_FUNC(CutsceneSeq_GetEntity);
    ADD_PUBLIC_FUNC(CutsceneSeq_LockPlayerControl);
    ADD_PUBLIC_FUNC(CutsceneSeq_LockAllPlayerControl);
    ADD_PUBLIC_FUNC(CutsceneSeq_StartSequence);

    // Cutscene/FXExpandRing
    ADD_PUBLIC_FUNC(FXExpandRing_State_FadeIn);
    ADD_PUBLIC_FUNC(FXExpandRing_State_Expand);
    ADD_PUBLIC_FUNC(FXExpandRing_State_FadeOut);

    // Cutscene/FXFade
    ADD_PUBLIC_FUNC(FXFade_StopAll);
    ADD_PUBLIC_FUNC(FXFade_State_FadeOut);
    ADD_PUBLIC_FUNC(FXFade_State_Wait);
    ADD_PUBLIC_FUNC(FXFade_State_FadeIn);
    ADD_PUBLIC_FUNC(FXFade_State_FadeInBlack);

    // Cutscene/FXRuby
    ADD_PUBLIC_FUNC(FXRuby_SetupLayerDeformation);
    ADD_PUBLIC_FUNC(FXRuby_HandleLayerDeform);
    ADD_PUBLIC_FUNC(FXRuby_State_Expanding);
    ADD_PUBLIC_FUNC(FXRuby_State_Shrinking);
    ADD_PUBLIC_FUNC(FXRuby_State_Idle);
    ADD_PUBLIC_FUNC(FXRuby_State_IncreaseStageDeform);
    ADD_PUBLIC_FUNC(FXRuby_State_DecreaseStageDeform);
    ADD_PUBLIC_FUNC(FXRuby_State_ShrinkAndDestroy);

    // Cutscene/FXSpinRay
    ADD_PUBLIC_FUNC(FXSpinRay_State_FadeIn);
    ADD_PUBLIC_FUNC(FXSpinRay_State_Spinning);
    ADD_PUBLIC_FUNC(FXSpinRay_State_FadeOut);

    // Cutscene/FXWaveRing
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(FXWaveRing_State_FadeIn);
    ADD_PUBLIC_FUNC(FXWaveRing_State_Wait);
    ADD_PUBLIC_FUNC(FXWaveRing_State_FadeOut);
#endif

#if MANIA_USE_PLUS
    // Cutscene/RubyPortal
    ADD_PUBLIC_FUNC(RubyPortal_HandleTileDestruction);
    ADD_PUBLIC_FUNC(RubyPortal_State_AwaitOpenTMZ2);
#endif
    ADD_PUBLIC_FUNC(RubyPortal_State_Opening);
    ADD_PUBLIC_FUNC(RubyPortal_State_Opened);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(RubyPortal_State_SaveGameState);
    ADD_PUBLIC_FUNC(RubyPortal_State_Open_WarpDoor);
#endif
    ADD_PUBLIC_FUNC(RubyPortal_State_Open_Cutscene);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(RubyPortal_State_EncoreEnd);
    ADD_PUBLIC_FUNC(RubyPortal_State_EncoreRampage);
#endif

    // ERZ/ERZGunner
    ADD_PUBLIC_FUNC(ERZGunner_HandleDudExhaust);
    ADD_PUBLIC_FUNC(ERZGunner_HandleMalfunctionDudExhaust);
    ADD_PUBLIC_FUNC(ERZGunner_SpawnDust);
    ADD_PUBLIC_FUNC(ERZGunner_HandleRotations);
    ADD_PUBLIC_FUNC(ERZGunner_CheckPlayerMissileCollisions);
    ADD_PUBLIC_FUNC(ERZGunner_CheckPlayerExplosionCollisions);
    ADD_PUBLIC_FUNC(ERZGunner_Hit);
    ADD_PUBLIC_FUNC(ERZGunner_Draw_Gunner);
    ADD_PUBLIC_FUNC(ERZGunner_Draw_RocketLaunch);
    ADD_PUBLIC_FUNC(ERZGunner_Draw_Rocket);
    ADD_PUBLIC_FUNC(ERZGunner_State_Idle);
    ADD_PUBLIC_FUNC(ERZGunner_State_LaunchRockets);
    ADD_PUBLIC_FUNC(ERZGunner_State_LaunchedRocket);
    ADD_PUBLIC_FUNC(ERZGunner_State_Mortar);
    ADD_PUBLIC_FUNC(ERZGunner_State_Napalm);
    ADD_PUBLIC_FUNC(ERZGunner_State_Dud_Active);
    ADD_PUBLIC_FUNC(ERZGunner_State_Dud_HitByPlayer);
    ADD_PUBLIC_FUNC(ERZGunner_State_Dud_Malfunction);
    ADD_PUBLIC_FUNC(ERZGunner_State_Dud_Explode);
    ADD_PUBLIC_FUNC(ERZGunner_State_NapalmExplosion);
    ADD_PUBLIC_FUNC(ERZGunner_State_MortarExplosion);

    // ERZ/ERZKing
    ADD_PUBLIC_FUNC(ERZKing_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(ERZKing_Hit);
    ADD_PUBLIC_FUNC(ERZKing_HandleFrames);
    ADD_PUBLIC_FUNC(ERZKing_Draw_Body);
    ADD_PUBLIC_FUNC(ERZKing_Draw_Arm);
    ADD_PUBLIC_FUNC(ERZKing_State_SetupArena);
    ADD_PUBLIC_FUNC(ERZKing_State_SetupBody);
    ADD_PUBLIC_FUNC(ERZKing_State_EnterKing);
    ADD_PUBLIC_FUNC(ERZKing_State_FlyAround);
    ADD_PUBLIC_FUNC(ERZKing_State_ChangeHBH);
    ADD_PUBLIC_FUNC(ERZKing_State_Arm);
    ADD_PUBLIC_FUNC(ERZKing_State_Explode);

    // ERZ/ERZMystic
    ADD_PUBLIC_FUNC(ERZMystic_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(ERZMystic_Hit);
    ADD_PUBLIC_FUNC(ERZMystic_SetupNewCupSwap);
    ADD_PUBLIC_FUNC(ERZMystic_Draw_CupSetup);
    ADD_PUBLIC_FUNC(ERZMystic_Draw_CupSwap);
    ADD_PUBLIC_FUNC(ERZMystic_State_Init);
    ADD_PUBLIC_FUNC(ERZMystic_State_SetupInitialCupPos);
    ADD_PUBLIC_FUNC(ERZMystic_State_MoveCupsDownwards);
    ADD_PUBLIC_FUNC(ERZMystic_State_RotateMiddleCup);
    ADD_PUBLIC_FUNC(ERZMystic_State_MoveMiddleCupToFloor);
    ADD_PUBLIC_FUNC(ERZMystic_State_PrepareCupSwap);
    ADD_PUBLIC_FUNC(ERZMystic_State_CupSwapping);
    ADD_PUBLIC_FUNC(ERZMystic_State_RevealMystic);
    ADD_PUBLIC_FUNC(ERZMystic_State_CupBlast);
    ADD_PUBLIC_FUNC(ERZMystic_State_MoveCupsToMystic);

    // ERZ/ERZOutro
    ADD_PUBLIC_FUNC(ERZOutro_SetEmeraldStates);
    ADD_PUBLIC_FUNC(ERZOutro_HandleRubyHover);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_AttackEggman);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_AttackRecoil);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_LoseEmeralds);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_OpenPortal);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_EnterPortal);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_FadeOut);
    ADD_PUBLIC_FUNC(ERZOutro_Cutscene_ShowEnding);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ERZOutro_SaveFileCB);
#else
    ADD_PUBLIC_FUNC(ERZOutro_SaveFileCB);
#endif

    // ERZ/ERZRider
    ADD_PUBLIC_FUNC(ERZRider_Draw_Rider);

    // ERZ/ERZSetup
    ADD_PUBLIC_FUNC(ERZSetup_Scanline_Sky);

    // ERZ/ERZShinobi
    ADD_PUBLIC_FUNC(ERZShinobi_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(ERZShinobi_Hit);
    ADD_PUBLIC_FUNC(ERZShinobi_HandleTileCollisions);
    ADD_PUBLIC_FUNC(ERZShinobi_State_Moving);

    // ERZ/ERZStart
    ADD_PUBLIC_FUNC(ERZStart_SetupObjects);
    ADD_PUBLIC_FUNC(ERZStart_HandlePlayerHover);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_FadeIn);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_ShrinkRubyWarpFX);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_EnterKing);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_KingMovingRuby);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_KingAttatchHornRuby);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_SetupEggmanReveal);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_EnterEggman);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_EggmanKingWrestling);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_PostWrestleFadeIn);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_ReturnCamToSonic);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_PreparePlayerTransform);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_PlayerTransform);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_StartFight);
    ADD_PUBLIC_FUNC(ERZStart_Cutscene_Fight);
    ADD_PUBLIC_FUNC(ERZStart_RubyHover);
    ADD_PUBLIC_FUNC(ERZStart_RubyMove);
    ADD_PUBLIC_FUNC(ERZStart_Player_HandleSuperDash);
    ADD_PUBLIC_FUNC(ERZStart_State_PlayerSuperFly);
    ADD_PUBLIC_FUNC(ERZStart_Player_StartSuperFly);
    ADD_PUBLIC_FUNC(ERZStart_State_PlayerRebound);

    // ERZ/KleptoMobile
    ADD_PUBLIC_FUNC(KleptoMobile_HandleAnimations);
    ADD_PUBLIC_FUNC(KleptoMobile_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(KleptoMobile_Hit);
    ADD_PUBLIC_FUNC(KleptoMobile_Explode);
    ADD_PUBLIC_FUNC(KleptoMobile_HandleFrames);
    ADD_PUBLIC_FUNC(KleptoMobile_SwitchToKing);
    ADD_PUBLIC_FUNC(KleptoMobile_Draw_KleptoMobile);
    ADD_PUBLIC_FUNC(KleptoMobile_State_SetupArena);
    ADD_PUBLIC_FUNC(KleptoMobile_State_CutsceneControlled);
    ADD_PUBLIC_FUNC(KleptoMobile_State_MoveAround);
    ADD_PUBLIC_FUNC(KleptoMobile_State_Hover);
    ADD_PUBLIC_FUNC(KleptoMobile_HandleArmPositions);
    ADD_PUBLIC_FUNC(KleptoMobile_HandleChargeFinish);
    ADD_PUBLIC_FUNC(KleptoMobile_State_FirstChargeAttack);
    ADD_PUBLIC_FUNC(KleptoMobile_State_NextChargeAttacks);
    ADD_PUBLIC_FUNC(KleptoMobile_State_Switch);
    ADD_PUBLIC_FUNC(KleptoMobile_State_HitFall);
    ADD_PUBLIC_FUNC(KleptoMobile_StateHand_Cutscene);
    ADD_PUBLIC_FUNC(KleptoMobile_StateHand_Boss);
    ADD_PUBLIC_FUNC(KleptoMobile_Draw_Hand);
    ADD_PUBLIC_FUNC(KleptoMobile_CheckPlayerCollisions_Arm);
    ADD_PUBLIC_FUNC(KleptoMobile_StateArm_Cutscene);
    ADD_PUBLIC_FUNC(KleptoMobile_StateArm_Idle);
    ADD_PUBLIC_FUNC(KleptoMobile_StateArm_BashAttack);
    ADD_PUBLIC_FUNC(KleptoMobile_StateArm_ChargeAttack);
    ADD_PUBLIC_FUNC(KleptoMobile_Draw_Arm);
    ADD_PUBLIC_FUNC(KleptoMobile_State_Destroyed);
    ADD_PUBLIC_FUNC(KleptoMobile_State_Explode);
    ADD_PUBLIC_FUNC(KleptoMobile_State_CutsceneExplode);

    // ERZ/PhantomEgg
    ADD_PUBLIC_FUNC(PhantomEgg_HandleAnimations);
    ADD_PUBLIC_FUNC(PhantomEgg_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PhantomEgg_Hit);
    ADD_PUBLIC_FUNC(PhantomEgg_Explode);
    ADD_PUBLIC_FUNC(PhantomEgg_HandleNextAttack);
    ADD_PUBLIC_FUNC(PhantomEgg_SetupWarpFX);
    ADD_PUBLIC_FUNC(PhantomEgg_HandlePhantomWarp);
    ADD_PUBLIC_FUNC(PhantomEgg_HandleReturnWarp);
    ADD_PUBLIC_FUNC(PhantomEgg_Scanline_WarpFX);
    ADD_PUBLIC_FUNC(PhantomEgg_Draw_Normal);
    ADD_PUBLIC_FUNC(PhantomEgg_Draw_Cracked);
    ADD_PUBLIC_FUNC(PhantomEgg_State_SetupArena);
    ADD_PUBLIC_FUNC(PhantomEgg_State_DimArena);
    ADD_PUBLIC_FUNC(PhantomEgg_State_EnterEggman);
    ADD_PUBLIC_FUNC(PhantomEgg_State_AdjustStartingPos);
    ADD_PUBLIC_FUNC(PhantomEgg_State_IntroHover);
    ADD_PUBLIC_FUNC(PhantomEgg_State_BeginFight);
    ADD_PUBLIC_FUNC(PhantomEgg_State_MoveAround);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_Jumped);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_JumpLand);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_JumpAttack);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_CableShock);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_PrepareWarp);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_GrabPlayers);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_HandleWarp);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_WarpAway);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Attack_WarpReturn);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Destroyed);
    ADD_PUBLIC_FUNC(PhantomEgg_State_Exploding);
    ADD_PUBLIC_FUNC(PhantomEgg_State_StartBadEnd);
    ADD_PUBLIC_FUNC(PhantomEgg_State_CrackOpen);
    ADD_PUBLIC_FUNC(PhantomEgg_State_CrackedExploding);
    ADD_PUBLIC_FUNC(PhantomEgg_State_StartGoodEnd);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PhantomEgg_SaveGameCB);
#else
    ADD_PUBLIC_FUNC(PhantomEgg_SaveGameCB);
#endif

    // ERZ/PhantomGunner
    ADD_PUBLIC_FUNC(PhantomGunner_HandleDudExhaust);
    ADD_PUBLIC_FUNC(PhantomGunner_HandleMalfunctionDudExhaust);
    ADD_PUBLIC_FUNC(PhantomGunner_SpawnDust);
    ADD_PUBLIC_FUNC(PhantomGunner_HandleRotations);
    ADD_PUBLIC_FUNC(PhantomGunner_CheckPlayerMissileCollisions);
    ADD_PUBLIC_FUNC(PhantomGunner_CheckPlayerExplosionCollisions);
    ADD_PUBLIC_FUNC(PhantomGunner_Hit);
    ADD_PUBLIC_FUNC(PhantomGunner_Draw_Gunner);
    ADD_PUBLIC_FUNC(PhantomGunner_Draw_RocketLaunch);
    ADD_PUBLIC_FUNC(PhantomGunner_Draw_Rocket);
    ADD_PUBLIC_FUNC(PhantomGunner_State_ResetState);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Idle);
    ADD_PUBLIC_FUNC(PhantomGunner_State_LaunchRockets);
    ADD_PUBLIC_FUNC(PhantomGunner_State_LaunchedRocket);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Mortar);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Napalm);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Dud_Active);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Dud_HitByPlayer);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Dud_Malfunction);
    ADD_PUBLIC_FUNC(PhantomGunner_State_Dud_Explode);
    ADD_PUBLIC_FUNC(PhantomGunner_State_NapalmExplosion);
    ADD_PUBLIC_FUNC(PhantomGunner_State_MortarExplosion);

    // ERZ/PhantomHand
    ADD_PUBLIC_FUNC(PhantomHand_CheckPlayerGrab);
    ADD_PUBLIC_FUNC(PhantomHand_State_Summon);
    ADD_PUBLIC_FUNC(PhantomHand_State_Appear);
    ADD_PUBLIC_FUNC(PhantomHand_State_GrabbedPlayer);
    ADD_PUBLIC_FUNC(PhantomHand_State_Clasp);
    ADD_PUBLIC_FUNC(PhantomHand_State_Crystalize);
    ADD_PUBLIC_FUNC(PhantomHand_State_Shine);
    ADD_PUBLIC_FUNC(PhantomHand_State_BreakApart);
    ADD_PUBLIC_FUNC(PhantomHand_State_Disappear);

    // ERZ/PhantomKing
    ADD_PUBLIC_FUNC(PhantomKing_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PhantomKing_Hit);
    ADD_PUBLIC_FUNC(PhantomKing_Explode);
    ADD_PUBLIC_FUNC(PhantomKing_HandleFrames);
    ADD_PUBLIC_FUNC(PhantomKing_HandleAttacks);
    ADD_PUBLIC_FUNC(PhantomKing_SwitchToEggman);
    ADD_PUBLIC_FUNC(PhantomKing_SetupKing);
    ADD_PUBLIC_FUNC(PhantomKing_Draw_Body);
    ADD_PUBLIC_FUNC(PhantomKing_Draw_Arm);
    ADD_PUBLIC_FUNC(PhantomKing_State_Initialize);
    ADD_PUBLIC_FUNC(PhantomKing_State_SetupArms);
    ADD_PUBLIC_FUNC(PhantomKing_State_EnterKing);
    ADD_PUBLIC_FUNC(PhantomKing_State_InitialHover);
    ADD_PUBLIC_FUNC(PhantomKing_State_TakeRubyAway);
    ADD_PUBLIC_FUNC(PhantomKing_State_RubyHoldHover);
    ADD_PUBLIC_FUNC(PhantomKing_State_WrestleEggman);
    ADD_PUBLIC_FUNC(PhantomKing_State_FlyAround);
    ADD_PUBLIC_FUNC(PhantomKing_State_Switch);
    ADD_PUBLIC_FUNC(PhantomKing_State_HitFall);
    ADD_PUBLIC_FUNC(PhantomKing_StateArm_Idle);
    ADD_PUBLIC_FUNC(PhantomKing_StateArm_WrestleEggman);
    ADD_PUBLIC_FUNC(PhantomKing_HandleArmMovement);
    ADD_PUBLIC_FUNC(PhantomKing_StateArm_PullBack);
    ADD_PUBLIC_FUNC(PhantomKing_StateArm_Point);
    ADD_PUBLIC_FUNC(PhantomKing_DestroyEntity);
    ADD_PUBLIC_FUNC(PhantomKing_State_Explode);
    ADD_PUBLIC_FUNC(PhantomKing_State_Destroyed);

    // ERZ/PhantomMissile
    ADD_PUBLIC_FUNC(PhantomMissile_GetTargetPos);
    ADD_PUBLIC_FUNC(PhantomMissile_HandleExhaust);
    ADD_PUBLIC_FUNC(PhantomMissile_State_Attached);
    ADD_PUBLIC_FUNC(PhantomMissile_State_PrepareLaunch);
    ADD_PUBLIC_FUNC(PhantomMissile_State_Launched);
    ADD_PUBLIC_FUNC(PhantomMissile_State_Attacking);
    ADD_PUBLIC_FUNC(PhantomMissile_State_Explode);
    ADD_PUBLIC_FUNC(PhantomMissile_State_Reattach);
    ADD_PUBLIC_FUNC(PhantomMissile_State_Destroyed);

    // ERZ/PhantomMystic
    ADD_PUBLIC_FUNC(PhantomMystic_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PhantomMystic_Hit);
    ADD_PUBLIC_FUNC(PhantomMystic_SetupNewCupSwap);
    ADD_PUBLIC_FUNC(PhantomMystic_Draw_CupSetup);
    ADD_PUBLIC_FUNC(PhantomMystic_Draw_CupSwap);
    ADD_PUBLIC_FUNC(PhantomMystic_State_Init);
    ADD_PUBLIC_FUNC(PhantomMystic_State_SetupInitialCupPos);
    ADD_PUBLIC_FUNC(PhantomMystic_State_MoveCupsDownwards);
    ADD_PUBLIC_FUNC(PhantomMystic_State_RotateMiddleCup);
    ADD_PUBLIC_FUNC(PhantomMystic_State_MoveMiddleCupToFloor);
    ADD_PUBLIC_FUNC(PhantomMystic_State_PrepareCupSwap);
    ADD_PUBLIC_FUNC(PhantomMystic_State_CupSwapping);
    ADD_PUBLIC_FUNC(PhantomMystic_State_RevealMystic);
    ADD_PUBLIC_FUNC(PhantomMystic_State_CupBlast);
    ADD_PUBLIC_FUNC(PhantomMystic_State_MoveCupsToMystic);

    // ERZ/PhantomRider
    ADD_PUBLIC_FUNC(PhantomRider_ProcessAutoScroll);
    ADD_PUBLIC_FUNC(PhantomRider_Hit);
    ADD_PUBLIC_FUNC(PhantomRider_Draw_Rider);
    ADD_PUBLIC_FUNC(PhantomRider_State_HandleBegin);
    ADD_PUBLIC_FUNC(PhantomRider_State_EnterRider);
    ADD_PUBLIC_FUNC(PhantomRider_State_InitialRace);
    ADD_PUBLIC_FUNC(PhantomRider_State_RacePlayer);
    ADD_PUBLIC_FUNC(PhantomRider_State_ExitRider);
    ADD_PUBLIC_FUNC(PhantomRider_State_Jimmy);

    // ERZ/PhantomRuby
    ADD_PUBLIC_FUNC(PhantomRuby_PlaySfx);
    ADD_PUBLIC_FUNC(PhantomRuby_SetupFlash);
    ADD_PUBLIC_FUNC(PhantomRuby_State_FinishedFlash);
    ADD_PUBLIC_FUNC(PhantomRuby_State_PlaySfx);
    ADD_PUBLIC_FUNC(PhantomRuby_State_Oscillate);
    ADD_PUBLIC_FUNC(PhantomRuby_State_MoveGravity);
    ADD_PUBLIC_FUNC(PhantomRuby_State_MoveRotateGravity);
    ADD_PUBLIC_FUNC(PhantomRuby_State_MoveRotateGravity_CheckGround);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PhantomRuby_State_MoveToPos);
    ADD_PUBLIC_FUNC(PhantomRuby_State_RotateToOrigin);
#endif

    // ERZ/PhantomShield
    ADD_PUBLIC_FUNC(PhantomShield_State_Appear);
    ADD_PUBLIC_FUNC(PhantomShield_State_Active);
    ADD_PUBLIC_FUNC(PhantomShield_State_Disappear);

    // ERZ/PhantomShinobi
    ADD_PUBLIC_FUNC(PhantomShinobi_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PhantomShinobi_Hit);
    ADD_PUBLIC_FUNC(PhantomShinobi_HandleTileCollisions);
    ADD_PUBLIC_FUNC(PhantomShinobi_ResetStates);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_EnterShinobi);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_AttackDelay);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_SetupAttack);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_Moving);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_PrepareFinAttack);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_ExtendFins);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_RetractFins);
    ADD_PUBLIC_FUNC(PhantomShinobi_State_FinishedAttack);
    ADD_PUBLIC_FUNC(PhantomShinobi_SfxCheck_ShinobiBlade);

    // ERZ/PKingAttack
    ADD_PUBLIC_FUNC(PKingAttack_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PKingAttack_State_OrbitAppear);
    ADD_PUBLIC_FUNC(PKingAttack_State_Orbiting);
    ADD_PUBLIC_FUNC(PKingAttack_State_OrbitLaunched);
    ADD_PUBLIC_FUNC(PKingAttack_State_Trail);
    ADD_PUBLIC_FUNC(PKingAttack_State_SmallBullet);

    // ERZ/RingField
    ADD_PUBLIC_FUNC(RingField_GetRingSpawnPos);

    // FBZ/BigSqueeze
    ADD_PUBLIC_FUNC(BigSqueeze_SfxCheck_Rumble);
    ADD_PUBLIC_FUNC(BigSqueeze_HandleWallCollisions);
    ADD_PUBLIC_FUNC(BigSqueeze_CheckPlayerCollisions_Vulnerable);
    ADD_PUBLIC_FUNC(BigSqueeze_CheckPlayerCollisions_Electrified);
    ADD_PUBLIC_FUNC(BigSqueeze_Hit);
    ADD_PUBLIC_FUNC(BigSqueeze_Explode);
    ADD_PUBLIC_FUNC(BigSqueeze_HandleBossMovement);
    ADD_PUBLIC_FUNC(BigSqueeze_SpawnDebris);
    ADD_PUBLIC_FUNC(BigSqueeze_Draw_Boss);
    ADD_PUBLIC_FUNC(BigSqueeze_Draw_Crusher);
    ADD_PUBLIC_FUNC(BigSqueeze_StateManager_SetupIntro);
    ADD_PUBLIC_FUNC(BigSqueeze_StateManager_SetupEggman);
    ADD_PUBLIC_FUNC(BigSqueeze_StateManager_SetupArena);
    ADD_PUBLIC_FUNC(BigSqueeze_StateManager_SetupBoss);
    ADD_PUBLIC_FUNC(BigSqueeze_StateBoss_Idle);
    ADD_PUBLIC_FUNC(BigSqueeze_StateBoss_Electrified);
    ADD_PUBLIC_FUNC(BigSqueeze_StateBoss_Destroyed);
    ADD_PUBLIC_FUNC(BigSqueeze_StateBoss_DropSignPost);
    ADD_PUBLIC_FUNC(BigSqueeze_StateManager_HandleOutro);
    ADD_PUBLIC_FUNC(BigSqueeze_StateManager_HandleBoss);
    ADD_PUBLIC_FUNC(BigSqueeze_StateCrusher_BeginCrushing);
    ADD_PUBLIC_FUNC(BigSqueeze_StateCrusher_Crushing);

    // FBZ/Blaster
    ADD_PUBLIC_FUNC(Blaster_DebugSpawn);
    ADD_PUBLIC_FUNC(Blaster_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Blaster_CheckOffScreen);
    ADD_PUBLIC_FUNC(Blaster_State_Init);
    ADD_PUBLIC_FUNC(Blaster_State_Move);
    ADD_PUBLIC_FUNC(Blaster_State_HandleTurn);
    ADD_PUBLIC_FUNC(Blaster_State_AttackPlayer);
    ADD_PUBLIC_FUNC(Blaster_State_MagnetAttract);
    ADD_PUBLIC_FUNC(Blaster_State_MagnetReleased);
    ADD_PUBLIC_FUNC(Blaster_State_BeginShot);
    ADD_PUBLIC_FUNC(Blaster_State_Shot);
    ADD_PUBLIC_FUNC(Blaster_State_Fall);
    ADD_PUBLIC_FUNC(Blaster_State_Shell);

    // FBZ/Clucker
    ADD_PUBLIC_FUNC(Clucker_DebugSpawn);
    ADD_PUBLIC_FUNC(Clucker_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Clucker_CheckOffScreen);
    ADD_PUBLIC_FUNC(Clucker_State_Init);
    ADD_PUBLIC_FUNC(Clucker_State_CheckForPlayer);
    ADD_PUBLIC_FUNC(Clucker_State_Appear);
    ADD_PUBLIC_FUNC(Clucker_State_ShootDelay);
    ADD_PUBLIC_FUNC(Clucker_State_Shoot);
    ADD_PUBLIC_FUNC(Clucker_State_Turn);
    ADD_PUBLIC_FUNC(Clucker_State_Destroyed);
    ADD_PUBLIC_FUNC(Clucker_State_Egg);

    // FBZ/Crane
    ADD_PUBLIC_FUNC(Crane_DebugSpawn);
    ADD_PUBLIC_FUNC(Crane_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Crane_CheckOffScreen);
    ADD_PUBLIC_FUNC(Crane_State_CheckOffScreen);
    ADD_PUBLIC_FUNC(Crane_State_Init);
    ADD_PUBLIC_FUNC(Crane_State_CheckForPlayers);
    ADD_PUBLIC_FUNC(Crane_State_LowerToGrab);
    ADD_PUBLIC_FUNC(Crane_State_CheckGrab);
    ADD_PUBLIC_FUNC(Crane_State_RiseUp);
    ADD_PUBLIC_FUNC(Crane_State_ToDest1stHalf);
    ADD_PUBLIC_FUNC(Crane_State_ToDest2ndHalf);
    ADD_PUBLIC_FUNC(Crane_State_DropDelay);
    ADD_PUBLIC_FUNC(Crane_State_ToStart1stHalf);
    ADD_PUBLIC_FUNC(Crane_State_ToStart2ndHalf);

    // FBZ/Cylinder
    ADD_PUBLIC_FUNC(Cylinder_State_TubeH);
    ADD_PUBLIC_FUNC(Cylinder_State_TubeV);
    ADD_PUBLIC_FUNC(Cylinder_State_Spiral);
    ADD_PUBLIC_FUNC(Cylinder_State_InkRoller);
    ADD_PUBLIC_FUNC(Cylinder_State_Pillar);
    ADD_PUBLIC_FUNC(Cylinder_PlayerState_InkRoller_Stand);
    ADD_PUBLIC_FUNC(Cylinder_PlayerState_InkRoller_Roll);
    ADD_PUBLIC_FUNC(Cylinder_PlayerState_Pillar);
    ADD_PUBLIC_FUNC(Cylinder_PlayerState_Spiral);

    // FBZ/ElectroMagnet
    ADD_PUBLIC_FUNC(ElectroMagnet_State_AwaitInterval);
    ADD_PUBLIC_FUNC(ElectroMagnet_State_MagnetActive);

    // FBZ/FBZ1Outro
    ADD_PUBLIC_FUNC(FBZ1Outro_StartCutscene);
    ADD_PUBLIC_FUNC(FBZ1Outro_HandleTrash);
    ADD_PUBLIC_FUNC(FBZ1Outro_DispenseTrash);
    ADD_PUBLIC_FUNC(FBZ1Outro_Cutscene_CrushTrash);
    ADD_PUBLIC_FUNC(FBZ1Outro_Cutscene_TrashDrop);
    ADD_PUBLIC_FUNC(FBZ1Outro_Cutscene_CraneRide);
    ADD_PUBLIC_FUNC(FBZ1Outro_Cutscene_PrepareFBZ2);

    // FBZ/FBZ2Outro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(FBZ2Outro_StartCutscene);
    ADD_PUBLIC_FUNC(FBZ2Outro_Cutscene_SetupGliders);
    ADD_PUBLIC_FUNC(FBZ2Outro_Cutscene_RunToGlider);
    ADD_PUBLIC_FUNC(FBZ2Outro_Cutscene_GlideAway);
#endif

    // FBZ/FBZMissile
    ADD_PUBLIC_FUNC(FBZMissile_StateLauncherV_Delay);
    ADD_PUBLIC_FUNC(FBZMissile_StateLauncherV_Launch);
    ADD_PUBLIC_FUNC(FBZMissile_StateLauncherH_Fire);
    ADD_PUBLIC_FUNC(FBZMissile_StateLauncherH_Prepare);
    ADD_PUBLIC_FUNC(FBZMissile_StateVertical_Rise);
    ADD_PUBLIC_FUNC(FBZMissile_StateVertical_Fall);
    ADD_PUBLIC_FUNC(FBZMissile_StateHorizontal_Move);
    ADD_PUBLIC_FUNC(FBZMissile_State_Hull);

    // FBZ/FBZSetup
    ADD_PUBLIC_FUNC(FBZSetup_ActTransitionLoad);
    ADD_PUBLIC_FUNC(FBZSetup_AddDynamicBG);
    ADD_PUBLIC_FUNC(FBZSetup_Scanline_BGInside);
    ADD_PUBLIC_FUNC(FBZSetup_BGSwitch_ShowInside1);
    ADD_PUBLIC_FUNC(FBZSetup_BGSwitch_ShowInside2);
    ADD_PUBLIC_FUNC(FBZSetup_BGSwitch_ShowInside1_NoStorm);
    ADD_PUBLIC_FUNC(FBZSetup_Trigger_ShowExterior);
    ADD_PUBLIC_FUNC(FBZSetup_Trigger_ShowInterior);
    ADD_PUBLIC_FUNC(FBZSetup_StageFinish_EndAct1);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(FBZSetup_StageFinish_EndAct2);
#endif

    // FBZ/FBZStorm
    ADD_PUBLIC_FUNC(FBZStorm_State_WaitForActive);
    ADD_PUBLIC_FUNC(FBZStorm_State_StormStart);
    ADD_PUBLIC_FUNC(FBZStorm_State_StormFinish);
    ADD_PUBLIC_FUNC(FBZStorm_State_Storming);
    ADD_PUBLIC_FUNC(FBZStorm_State_Thunder);

    // FBZ/FBZTrash
    ADD_PUBLIC_FUNC(FBZTrash_SummonOrbinautOrbs);
    ADD_PUBLIC_FUNC(FBZTrash_SummonOrbinaut);
    ADD_PUBLIC_FUNC(FBZTrash_State_LooseTrash);
    ADD_PUBLIC_FUNC(FBZTrash_State_ReactMagnet);
    ADD_PUBLIC_FUNC(FBZTrash_State_MoveToTarget);
    ADD_PUBLIC_FUNC(FBZTrash_State_OrbinautOrb);
    ADD_PUBLIC_FUNC(FBZTrash_State_OrbinautMove);

    // FBZ/FlameSpring
    ADD_PUBLIC_FUNC(FlameSpring_State_Spring);
    ADD_PUBLIC_FUNC(FlameSpring_State_Flame);
    ADD_PUBLIC_FUNC(FlameSpring_Draw_Spring);
    ADD_PUBLIC_FUNC(FlameSpring_Draw_Flame);

    // FBZ/FoldingPlatform
    ADD_PUBLIC_FUNC(FoldingPlatform_State_Appear);
    ADD_PUBLIC_FUNC(FoldingPlatform_State_Disappear);

    // FBZ/HangGlider
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(HangGlider_State_CheckGrab);
    ADD_PUBLIC_FUNC(HangGlider_State_Glide);
#endif

    // FBZ/HangPoint
    ADD_PUBLIC_FUNC(HangPoint_HandlePlayerMovement);

    // FBZ/Launcher
    ADD_PUBLIC_FUNC(Launcher_Collide_Normal);
    ADD_PUBLIC_FUNC(Launcher_State_Idle);
    ADD_PUBLIC_FUNC(Launcher_State_HandleLaunch);
    ADD_PUBLIC_FUNC(Launcher_State_ReturnToStart);

    // FBZ/MagPlatform
    ADD_PUBLIC_FUNC(MagPlatform_Collide_SolidAllHazardBottom);
    ADD_PUBLIC_FUNC(MagPlatform_State_Idle);
    ADD_PUBLIC_FUNC(MagPlatform_State_Rise);
    ADD_PUBLIC_FUNC(MagPlatform_State_Fall);

#if MANIA_USE_PLUS
    // FBZ/Mine
    ADD_PUBLIC_FUNC(Mine_CheckMightyHit);
#endif

    // FBZ/SpiderMobile
    ADD_PUBLIC_FUNC(SpiderMobile_HandleFallingMovement);
    ADD_PUBLIC_FUNC(SpiderMobile_HandleRisingMovement);
    ADD_PUBLIC_FUNC(SpiderMobile_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SpiderMobile_CheckSpikeHit);
    ADD_PUBLIC_FUNC(SpiderMobile_Hit);
    ADD_PUBLIC_FUNC(SpiderMobile_Explode);
    ADD_PUBLIC_FUNC(SpiderMobile_HandlePlatformMovement);
    ADD_PUBLIC_FUNC(SpiderMobile_HandleWebClimbArmMovement);
    ADD_PUBLIC_FUNC(SpiderMobile_HandleDestroyedArmMovement);
    ADD_PUBLIC_FUNC(SpiderMobile_HandleIdleArmMovement);
    ADD_PUBLIC_FUNC(SpiderMobile_UpdateLimbPositions);
    ADD_PUBLIC_FUNC(SpiderMobile_Draw_Body);
    ADD_PUBLIC_FUNC(SpiderMobile_Draw_Bumper);
    ADD_PUBLIC_FUNC(SpiderMobile_Draw_Cockpit);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_AwaitPlayer);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_SetupArena);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_InitialDrop);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_Rise);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_Climb);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_HandleOrbAttack);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_Destroyed);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_CockpitExplode);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_MovePlatformToEnd);
    ADD_PUBLIC_FUNC(SpiderMobile_StateBody_FinishedMovingPlatform);
    ADD_PUBLIC_FUNC(SpiderMobile_State_Bumper);
    ADD_PUBLIC_FUNC(SpiderMobile_State_Eggman);
    ADD_PUBLIC_FUNC(SpiderMobile_StateOrb_Charge);
    ADD_PUBLIC_FUNC(SpiderMobile_StateOrb_Fired);
    ADD_PUBLIC_FUNC(SpiderMobile_SfxCheck_Elevator);

    // FBZ/SpiralPlatform
    ADD_PUBLIC_FUNC(SpiralPlatform_State_Move);

    // FBZ/SwitchDoor
    ADD_PUBLIC_FUNC(SwitchDoor_DrawSprites);

    // FBZ/Technosqueek
    ADD_PUBLIC_FUNC(Technosqueek_DebugSpawn);
    ADD_PUBLIC_FUNC(Technosqueek_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(Technosqueek_CheckOffScreen);
    ADD_PUBLIC_FUNC(Technosqueek_State_Init);
    ADD_PUBLIC_FUNC(Technosqueek_State_MoveHorizontal);
    ADD_PUBLIC_FUNC(Technosqueek_State_TurnHorizontal);
    ADD_PUBLIC_FUNC(Technosqueek_State_MoveVertical);
    ADD_PUBLIC_FUNC(Technosqueek_State_TurnVertical);
    ADD_PUBLIC_FUNC(Technosqueek_State_Fall);

    // FBZ/TetherBall
    ADD_PUBLIC_FUNC(TetherBall_State_CheckPlayerAttach);
    ADD_PUBLIC_FUNC(TetherBall_State_SwingBall);
    ADD_PUBLIC_FUNC(TetherBall_State_FinishedSwing);

    // FBZ/Tuesday
    ADD_PUBLIC_FUNC(Tuesday_Hit);
    ADD_PUBLIC_FUNC(Tuesday_Explode);
    ADD_PUBLIC_FUNC(Tuesday_DrawElectricity);
    ADD_PUBLIC_FUNC(Tuesday_State_Controller);
    ADD_PUBLIC_FUNC(Tuesday_State_Node);
    ADD_PUBLIC_FUNC(Tuesday_State_Destroyed);
    ADD_PUBLIC_FUNC(Tuesday_State_Debris);

    // FBZ/TwistingDoor
    ADD_PUBLIC_FUNC(TwistingDoor_State_CheckOpen);
    ADD_PUBLIC_FUNC(TwistingDoor_State_Opening);
    ADD_PUBLIC_FUNC(TwistingDoor_State_Opened);
    ADD_PUBLIC_FUNC(TwistingDoor_State_Closing);

    // FBZ/WarpDoor
    ADD_PUBLIC_FUNC(WarpDoor_SetupPlayerCamera);
    ADD_PUBLIC_FUNC(WarpDoor_SetStageBounds);
    ADD_PUBLIC_FUNC(WarpDoor_SetWarpBounds);
    ADD_PUBLIC_FUNC(WarpDoor_CheckAllBounds);
    ADD_PUBLIC_FUNC(WarpDoor_SetupBoundaries);
    ADD_PUBLIC_FUNC(WarpDoor_DrawDebug);
    ADD_PUBLIC_FUNC(WarpDoor_SetupHitbox);

    // GHZ/Batbrain
    ADD_PUBLIC_FUNC(Batbrain_DebugSpawn);
    ADD_PUBLIC_FUNC(Batbrain_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Batbrain_CheckOffScreen);
    ADD_PUBLIC_FUNC(Batbrain_State_Init);
    ADD_PUBLIC_FUNC(Batbrain_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Batbrain_State_DropToPlayer);
    ADD_PUBLIC_FUNC(Batbrain_State_Fly);
    ADD_PUBLIC_FUNC(Batbrain_State_FlyToCeiling);

    // GHZ/Bridge
    ADD_PUBLIC_FUNC(Bridge_Burn);
    ADD_PUBLIC_FUNC(Bridge_HandleCollisions);

    // GHZ/BuzzBomber
    ADD_PUBLIC_FUNC(BuzzBomber_DebugSpawn);
    ADD_PUBLIC_FUNC(BuzzBomber_CheckOffScreen);
    ADD_PUBLIC_FUNC(BuzzBomber_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(BuzzBomber_State_Init);
    ADD_PUBLIC_FUNC(BuzzBomber_State_Flying);
    ADD_PUBLIC_FUNC(BuzzBomber_State_Idle);
    ADD_PUBLIC_FUNC(BuzzBomber_State_DetectedPlayer);
    ADD_PUBLIC_FUNC(BuzzBomber_State_ProjectileCharge);
    ADD_PUBLIC_FUNC(BuzzBomber_State_ProjectileShot);

    // GHZ/CheckerBall
    ADD_PUBLIC_FUNC(CheckerBall_DebugSpawn);
    ADD_PUBLIC_FUNC(CheckerBall_HandlePhysics);
    ADD_PUBLIC_FUNC(CheckerBall_HandlePlayerMovement);
    ADD_PUBLIC_FUNC(CheckerBall_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(CheckerBall_BadnikBreak);
    ADD_PUBLIC_FUNC(CheckerBall_HandleObjectCollisions);

    // GHZ/Chopper
    ADD_PUBLIC_FUNC(Chopper_DebugSpawn);
    ADD_PUBLIC_FUNC(Chopper_CheckOffScreen);
    ADD_PUBLIC_FUNC(Chopper_CheckPlayerCollisions_Jump);
    ADD_PUBLIC_FUNC(Chopper_CheckPlayerCollisions_Swim);
    ADD_PUBLIC_FUNC(Chopper_State_Init);
    ADD_PUBLIC_FUNC(Chopper_State_Jump);
    ADD_PUBLIC_FUNC(Chopper_State_Swim);
    ADD_PUBLIC_FUNC(Chopper_State_ChargeDelay);
    ADD_PUBLIC_FUNC(Chopper_State_Charge);

    // GHZ/Crabmeat
    ADD_PUBLIC_FUNC(Crabmeat_DebugSpawn);
    ADD_PUBLIC_FUNC(Crabmeat_CheckOffScreen);
    ADD_PUBLIC_FUNC(Crabmeat_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Crabmeat_State_Init);
    ADD_PUBLIC_FUNC(Crabmeat_State_Moving);
    ADD_PUBLIC_FUNC(Crabmeat_State_Shoot);
    ADD_PUBLIC_FUNC(Crabmeat_State_Projectile);

    // GHZ/DDWrecker
    ADD_PUBLIC_FUNC(DDWrecker_State_SetupArena);
    ADD_PUBLIC_FUNC(DDWrecker_State_InitChildren);
    ADD_PUBLIC_FUNC(DDWrecker_State_Assemble);
    ADD_PUBLIC_FUNC(DDWrecker_State_EnterWreckers);
    ADD_PUBLIC_FUNC(DDWrecker_State_AttackDelay);
    ADD_PUBLIC_FUNC(DDWrecker_State_SwingRight);
    ADD_PUBLIC_FUNC(DDWrecker_State_SwingLeft);
    ADD_PUBLIC_FUNC(DDWrecker_State_SwingMoveToCenter);
    ADD_PUBLIC_FUNC(DDWrecker_State_HandleSpinning);
    ADD_PUBLIC_FUNC(DDWrecker_State_SwingSlowDown);
    ADD_PUBLIC_FUNC(DDWrecker_State_PrepareBounceAttack);
    ADD_PUBLIC_FUNC(DDWrecker_State_SignalBounceAttackStart);
    ADD_PUBLIC_FUNC(DDWrecker_State_HandleBounceAttack);
    ADD_PUBLIC_FUNC(DDWrecker_State_EndBounceAttack);
    ADD_PUBLIC_FUNC(DDWrecker_StateBall_Vulnerable);
    ADD_PUBLIC_FUNC(DDWrecker_StateBall_Spiked);
    ADD_PUBLIC_FUNC(DDWrecker_StateBall_Partnerless);
    ADD_PUBLIC_FUNC(DDWrecker_Hit);
    ADD_PUBLIC_FUNC(DDWrecker_Spin);
    ADD_PUBLIC_FUNC(DDWrecker_Swing);
    ADD_PUBLIC_FUNC(DDWrecker_Explode);
    ADD_PUBLIC_FUNC(DDWrecker_State_Debris);
    ADD_PUBLIC_FUNC(DDWrecker_State_Die);
    ADD_PUBLIC_FUNC(DDWrecker_State_SpawnSignpost);

    // GHZ/DERobot
    ADD_PUBLIC_FUNC(DERobot_HandleScreenBounds);
    ADD_PUBLIC_FUNC(DERobot_HandleLegMovement);
    ADD_PUBLIC_FUNC(DERobot_HandleLegMovement2);
    ADD_PUBLIC_FUNC(DERobot_HandleArmMovement);
    ADD_PUBLIC_FUNC(DERobot_HandleTerrainDestruction);
    ADD_PUBLIC_FUNC(DERobot_DestroyTerrainFinal);
    ADD_PUBLIC_FUNC(DERobot_Hit);
    ADD_PUBLIC_FUNC(DERobot_Explode);
    ADD_PUBLIC_FUNC(DERobot_CheckPlayerCollisions_Body);
    ADD_PUBLIC_FUNC(DERobot_CheckPlayerCollisions_ArmExtend);
    ADD_PUBLIC_FUNC(DERobot_CheckPlayerCollisions_Hand);
    ADD_PUBLIC_FUNC(DERobot_CheckRubyGrabbed);
    ADD_PUBLIC_FUNC(DERobot_CheckPlayerCollisions_Bomb);
    ADD_PUBLIC_FUNC(DERobot_Draw_RelativeToParent);
    ADD_PUBLIC_FUNC(DERobot_Draw_Arm);
    ADD_PUBLIC_FUNC(DERobot_Draw_Simple);
    ADD_PUBLIC_FUNC(DERobot_Draw_FrontLeg);
    ADD_PUBLIC_FUNC(DERobot_Draw_Target);
    ADD_PUBLIC_FUNC(DERobot_State_ArmIdle);
    ADD_PUBLIC_FUNC(DERobot_State_ArmExtendPrepare);
    ADD_PUBLIC_FUNC(DERobot_State_ArmExtending);
    ADD_PUBLIC_FUNC(DERobot_State_ArmRetracting);
    ADD_PUBLIC_FUNC(DERobot_State_ArmDestroyed);
    ADD_PUBLIC_FUNC(DERobot_Cutscene_ActivateArm);
    ADD_PUBLIC_FUNC(DERobot_Cutscene_ReachForRuby);
    ADD_PUBLIC_FUNC(DERobot_Cutscene_GrabbedRuby);
    ADD_PUBLIC_FUNC(DERobot_Cutscene_ArmDeactivate);
    ADD_PUBLIC_FUNC(DERobot_State_CloseHeadHatch);
    ADD_PUBLIC_FUNC(DERobot_State_BombLaunched);
    ADD_PUBLIC_FUNC(DERobot_State_BombLanded);
    ADD_PUBLIC_FUNC(DERobot_State_BombExplode);
    ADD_PUBLIC_FUNC(DERobot_State_SetupArena);
    ADD_PUBLIC_FUNC(DERobot_State_SetupBoss);
    ADD_PUBLIC_FUNC(DERobot_State_Target);
    ADD_PUBLIC_FUNC(DERobot_State_SurpriseFall);
    ADD_PUBLIC_FUNC(DERobot_State_FallLand);
    ADD_PUBLIC_FUNC(DERobot_State_Walk);
    ADD_PUBLIC_FUNC(DERobot_State_ArmAttack);
    ADD_PUBLIC_FUNC(DERobot_State_Explode);
    ADD_PUBLIC_FUNC(DERobot_State_ExplodeTerrain);
    ADD_PUBLIC_FUNC(DERobot_State_Finish);
    ADD_PUBLIC_FUNC(DERobot_State_DebrisFall);
    ADD_PUBLIC_FUNC(DERobot_State_FinishBounds);
    ADD_PUBLIC_FUNC(DERobot_State_CutsceneExplode);

    // GHZ/Fireball
    ADD_PUBLIC_FUNC(Fireball_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Fireball_State_Spawner);
    ADD_PUBLIC_FUNC(Fireball_State_LauncherStatic);
    ADD_PUBLIC_FUNC(Fireball_State_LauncherGravity);
    ADD_PUBLIC_FUNC(Fireball_StateFireball_Spawner);
    ADD_PUBLIC_FUNC(Fireball_StateFireball_LauncherStatic);
    ADD_PUBLIC_FUNC(Fireball_StateFireball_LauncherGravity);
    ADD_PUBLIC_FUNC(Fireball_StateFireball_Dissipate);
    ADD_PUBLIC_FUNC(Fireball_Draw_Simple);

    // GHZ/GHZ2Outro
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_FinishActClear);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_JumpIntoHole);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_HoleSceneFadeIn);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_SpyOnEggman);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_BreakupGroup);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_RubyHover);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_StartRubyWarp);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_HandleRubyWarp);
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_LoadCPZ1);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(GHZ2Outro_Cutscene_SkipCB);
#endif

    // GHZ/GHZCutsceneK
    ADD_PUBLIC_FUNC(GHZCutsceneK_Cutscene_None);

    // GHZ/GHZCutsceneST
    ADD_PUBLIC_FUNC(GHZCutsceneST_SetupObjects);
    ADD_PUBLIC_FUNC(GHZCutsceneST_SetupKnuxCutscene);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(GHZCutsceneST_Cutscene_SkipCB);
    ADD_PUBLIC_FUNC(GHZCutsceneST_Cutscene_FadeIn);
#endif
    ADD_PUBLIC_FUNC(GHZCutsceneST_Cutscene_FinishRubyWarp);
    ADD_PUBLIC_FUNC(GHZCutsceneST_Cutscene_ExitHBH);
    ADD_PUBLIC_FUNC(GHZCutsceneST_Cutscene_SetupGHZ1);

    // GHZ/GHZSetup
    ADD_PUBLIC_FUNC(GHZSetup_SetupAct1BG);
    ADD_PUBLIC_FUNC(GHZSetup_StageFinish_EndAct1);
    ADD_PUBLIC_FUNC(GHZSetup_HandleActTransition);
    ADD_PUBLIC_FUNC(GHZSetup_BGSwitch_Outside_Act2);
    ADD_PUBLIC_FUNC(GHZSetup_BGSwitch_Caves_Act2);
    ADD_PUBLIC_FUNC(GHZSetup_BGSwitch_Outside_Act1);
    ADD_PUBLIC_FUNC(GHZSetup_BGSwitch_Caves_Act1);
    ADD_PUBLIC_FUNC(GHZSetup_StageFinish_EndAct2);

    // GHZ/Motobug
    ADD_PUBLIC_FUNC(Motobug_DebugSpawn);
    ADD_PUBLIC_FUNC(Motobug_CheckOffScreen);
    ADD_PUBLIC_FUNC(Motobug_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Motobug_State_Fall);
    ADD_PUBLIC_FUNC(Motobug_State_Move);
    ADD_PUBLIC_FUNC(Motobug_State_Idle);
    ADD_PUBLIC_FUNC(Motobug_State_Init);
    ADD_PUBLIC_FUNC(Motobug_State_Smoke);
    ADD_PUBLIC_FUNC(Motobug_State_Turn);

    // GHZ/Newtron
    ADD_PUBLIC_FUNC(Newtron_DebugSpawn);
    ADD_PUBLIC_FUNC(Newtron_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Newtron_CheckOffScreen);
    ADD_PUBLIC_FUNC(Newtron_GetTargetDir);
    ADD_PUBLIC_FUNC(Newtron_State_Init);
    ADD_PUBLIC_FUNC(Newtron_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Newtron_State_Appear);
    ADD_PUBLIC_FUNC(Newtron_State_StartFly);
    ADD_PUBLIC_FUNC(Newtron_State_Fly);
    ADD_PUBLIC_FUNC(Newtron_State_Shoot);
    ADD_PUBLIC_FUNC(Newtron_State_FadeAway);
    ADD_PUBLIC_FUNC(Newtron_State_Projectile);

    // GHZ/SpikeLog
    ADD_PUBLIC_FUNC(SpikeLog_State_Main);
    ADD_PUBLIC_FUNC(SpikeLog_State_Burn);

    // GHZ/Splats
    ADD_PUBLIC_FUNC(Splats_DebugSpawn);
    ADD_PUBLIC_FUNC(Splats_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Splats_CheckOffScreen);
    ADD_PUBLIC_FUNC(Splats_State_Init);
    ADD_PUBLIC_FUNC(Splats_State_BounceAround);
    ADD_PUBLIC_FUNC(Splats_State_SetupInkJar);
    ADD_PUBLIC_FUNC(Splats_State_InkJarSpawner);
    ADD_PUBLIC_FUNC(Splats_State_JumpOutOfJar);
    ADD_PUBLIC_FUNC(Splats_State_HandleBouncing);
    ADD_PUBLIC_FUNC(Splats_State_HandleLanding);
    ADD_PUBLIC_FUNC(Splats_State_NoMoreJumps);
    ADD_PUBLIC_FUNC(Splats_State_InkSplat);

    // GHZ/WaterfallSound
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(WaterfallSound_SfxCheck_WaterfallLoop);
    ADD_PUBLIC_FUNC(WaterfallSound_SfxUpdate_WaterfallLoop);
#endif

    // GHZ/ZipLine
    ADD_PUBLIC_FUNC(ZipLine_VSSwap_CheckBusy);
    ADD_PUBLIC_FUNC(ZipLine_GrabHandle);
    ADD_PUBLIC_FUNC(ZipLine_ForceReleasePlayers);
    ADD_PUBLIC_FUNC(ZipLine_GetJoinPos);
    ADD_PUBLIC_FUNC(ZipLine_State_Moving);

#if MANIA_USE_PLUS
    // Global/ActClear
    ADD_PUBLIC_FUNC(ActClear_DrawTime);
    ADD_PUBLIC_FUNC(ActClear_DrawNumbers);
#endif
    ADD_PUBLIC_FUNC(ActClear_CheckPlayerVictory);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ActClear_SaveGameCallback);
#else
    ADD_PUBLIC_FUNC(ActClear_SaveGameCallback);
    ADD_PUBLIC_FUNC(ActClear_SetupRecoverPlayers);
#endif
    ADD_PUBLIC_FUNC(ActClear_State_EnterText);
    ADD_PUBLIC_FUNC(ActClear_State_AdjustText);
    ADD_PUBLIC_FUNC(ActClear_State_EnterResults);
    ADD_PUBLIC_FUNC(ActClear_State_ScoreShownDelay);
    ADD_PUBLIC_FUNC(ActClear_State_TallyScore);
    ADD_PUBLIC_FUNC(ActClear_State_SaveGameProgress);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ActClear_State_ShowResultsTA);
    ADD_PUBLIC_FUNC(ActClear_State_WaitForSave);
#endif
    ADD_PUBLIC_FUNC(ActClear_State_ExitActClear);
    ADD_PUBLIC_FUNC(ActClear_State_RecoverPlayers);

    // Global/Animals
    ADD_PUBLIC_FUNC(Animals_CheckDirection);
    ADD_PUBLIC_FUNC(Animals_CheckPlatformCollision);
    ADD_PUBLIC_FUNC(Animals_CheckGroundCollision);
    ADD_PUBLIC_FUNC(Animals_State_Fall);
    ADD_PUBLIC_FUNC(Animals_State_Bounce);
    ADD_PUBLIC_FUNC(Animals_State_Fly);
    ADD_PUBLIC_FUNC(Animals_State_Placed);

    // Global/Announcer
    ADD_PUBLIC_FUNC(Announcer_StartCountdown);
    ADD_PUBLIC_FUNC(Announcer_AnnounceGoal);
    ADD_PUBLIC_FUNC(Announcer_Draw_Countdown);
    ADD_PUBLIC_FUNC(Announcer_Draw_Finished);
    ADD_PUBLIC_FUNC(Announcer_State_Countdown);
    ADD_PUBLIC_FUNC(Announcer_State_Finished);
    ADD_PUBLIC_FUNC(Announcer_State_AnnounceWinner);
    ADD_PUBLIC_FUNC(Announcer_State_AnnounceWinPlayer);

    // Global/APICallback
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(APICallback_SetRichPresence);
    ADD_PUBLIC_FUNC(APICallback_GetUserLanguage);
    ADD_PUBLIC_FUNC(APICallback_GetConfirmButtonFlip);
    ADD_PUBLIC_FUNC(APICallback_SetNoSaveEnabled);
    ADD_PUBLIC_FUNC(APICallback_SetNoSaveDisabled);
    ADD_PUBLIC_FUNC(APICallback_SaveUserFile);
    ADD_PUBLIC_FUNC(APICallback_SaveCB);
    ADD_PUBLIC_FUNC(APICallback_SaveSettingsINI);
    ADD_PUBLIC_FUNC(APICallback_ReadLeaderboardEntry);
    ADD_PUBLIC_FUNC(APICallback_NotifyAutoSave_OK);
    ADD_PUBLIC_FUNC(APICallback_NotifyAutoSave_CB);
    ADD_PUBLIC_FUNC(APICallback_PromptSavePreference_CB);
    ADD_PUBLIC_FUNC(APICallback_PromptSavePreference);
    ADD_PUBLIC_FUNC(APICallback_LoadUserFile);
    ADD_PUBLIC_FUNC(APICallback_LoadCB);
    ADD_PUBLIC_FUNC(APICallback_LeaderboardStatus);
    ADD_PUBLIC_FUNC(APICallback_LeaderboardEntryCount);
    ADD_PUBLIC_FUNC(APICallback_LaunchManual);
    ADD_PUBLIC_FUNC(APICallback_HandleCallback);
    ADD_PUBLIC_FUNC(APICallback_GetUserAuthStatus);
    ADD_PUBLIC_FUNC(APICallback_GetStorageStatus);
    ADD_PUBLIC_FUNC(APICallback_GetSaveStatus);
    ADD_PUBLIC_FUNC(APICallback_GetControllerType);
    ADD_PUBLIC_FUNC(APICallback_FetchLeaderboardData);
    ADD_PUBLIC_FUNC(APICallback_ExitGame);
    ADD_PUBLIC_FUNC(APICallback_ClearPrerollErrors);
    ADD_PUBLIC_FUNC(APICallback_CheckInputDisconnected);
    ADD_PUBLIC_FUNC(APICallback_InputIDIsDisconnected);
    ADD_PUBLIC_FUNC(APICallback_ControllerIDForInputID);
    ADD_PUBLIC_FUNC(APICallback_MostRecentActiveControllerID);
    ADD_PUBLIC_FUNC(APICallback_AssignControllerID);
    ADD_PUBLIC_FUNC(APICallback_ResetControllerAssignments);
    ADD_PUBLIC_FUNC(APICallback_TrackActClear);
    ADD_PUBLIC_FUNC(APICallback_TrackTAClear);
    ADD_PUBLIC_FUNC(APICallback_TrackEnemyDefeat);
    ADD_PUBLIC_FUNC(APICallback_TrackGameProgress);
    ADD_PUBLIC_FUNC(APICallback_TryAuth_No);
    ADD_PUBLIC_FUNC(APICallback_TryAuth_Yes);
    ADD_PUBLIC_FUNC(APICallback_TryAuth_CB);
    ADD_PUBLIC_FUNC(APICallback_TryAuth);
    ADD_PUBLIC_FUNC(APICallback_TryInitStorage);
    ADD_PUBLIC_FUNC(APICallback_GetUsername);
    ADD_PUBLIC_FUNC(APICallback_ClearAchievements);
    ADD_PUBLIC_FUNC(APICallback_UnlockAchievement);
    ADD_PUBLIC_FUNC(APICallback_CheckUserAuth_OK);
    ADD_PUBLIC_FUNC(APICallback_CheckUserAuth_CB);
    ADD_PUBLIC_FUNC(APICallback_TrackGameProgressCB);
    ADD_PUBLIC_FUNC(APICallback_GetNextNotif);
    ADD_PUBLIC_FUNC(APICallback_ManageNotifs);
    ADD_PUBLIC_FUNC(APICallback_CheckUnreadNotifs);
    ADD_PUBLIC_FUNC(APICallback_NotifyAutosave);
#endif

    // Global/BoundsMarker
    ADD_PUBLIC_FUNC(BoundsMarker_ApplyBounds);
    ADD_PUBLIC_FUNC(BoundsMarker_ApplyAllBounds);

    // Global/Camera
    ADD_PUBLIC_FUNC(Camera_SetCameraBounds);
    ADD_PUBLIC_FUNC(Camera_SetTargetEntity);
    ADD_PUBLIC_FUNC(Camera_ShakeScreen);
    ADD_PUBLIC_FUNC(Camera_HandleHBounds);
    ADD_PUBLIC_FUNC(Camera_HandleVBounds);
    ADD_PUBLIC_FUNC(Camera_SetupLerp);
    ADD_PUBLIC_FUNC(Camera_State_MapView);
    ADD_PUBLIC_FUNC(Camera_State_FollowXY);
    ADD_PUBLIC_FUNC(Camera_State_FollowX);
    ADD_PUBLIC_FUNC(Camera_State_FollowY);
    ADD_PUBLIC_FUNC(Camera_State_HandleLerp);

    // Global/Competition
    ADD_PUBLIC_FUNC(Competition_State_Manager);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Competition_ResetOptions);
    ADD_PUBLIC_FUNC(Competition_ClearMatchData);
    ADD_PUBLIC_FUNC(Competition_DeriveWinner);
    ADD_PUBLIC_FUNC(Competition_WinMatchFor);
#endif

    // Global/COverlay
    ADD_PUBLIC_FUNC(COverlay_DebugSpawn);
    ADD_PUBLIC_FUNC(COverlay_DrawTile);

    // Global/Debris
    ADD_PUBLIC_FUNC(Debris_CreateFromEntries);
    ADD_PUBLIC_FUNC(Debris_CreateFromEntries_UseOffset);
    ADD_PUBLIC_FUNC(Debris_State_Move);
    ADD_PUBLIC_FUNC(Debris_State_Fall);
    ADD_PUBLIC_FUNC(Debris_State_FallAndFlicker);

    // Global/DebugMode
    ADD_PUBLIC_FUNC(DebugMode_NullState);
    ADD_PUBLIC_FUNC(DebugMode_AddObject);

    // Global/DialogRunner
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(DialogRunner_HandleCallback);
    ADD_PUBLIC_FUNC(DialogRunner_NotifyAutoSave_CB);
    ADD_PUBLIC_FUNC(DialogRunner_NotifyAutoSave);
    ADD_PUBLIC_FUNC(DialogRunner_SetNoSaveDisabled);
    ADD_PUBLIC_FUNC(DialogRunner_SetNoSaveEnabled);
    ADD_PUBLIC_FUNC(DialogRunner_PromptSavePreference_CB);
    ADD_PUBLIC_FUNC(DialogRunner_CheckUserAuth_CB);
    ADD_PUBLIC_FUNC(DialogRunner_ManageNotifs);
    ADD_PUBLIC_FUNC(DialogRunner_TrackGameProgressCB);
    ADD_PUBLIC_FUNC(DialogRunner_GetNextNotif);
    ADD_PUBLIC_FUNC(DialogRunner_CheckUnreadNotifs);
    ADD_PUBLIC_FUNC(DialogRunner_NotifyAutosave);
    ADD_PUBLIC_FUNC(DialogRunner_GetUserAuthStatus);
    ADD_PUBLIC_FUNC(DialogRunner_PromptSavePreference);
    ADD_PUBLIC_FUNC(DialogRunner_CheckUserAuth_OK);
#endif

    // Global/Dust
    ADD_PUBLIC_FUNC(Dust_State_SpinDash);
    ADD_PUBLIC_FUNC(Dust_State_DustTrail);
    ADD_PUBLIC_FUNC(Dust_State_GlideTrail);
    ADD_PUBLIC_FUNC(Dust_State_DustPuff);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Dust_State_DustPuff_Collide);
#endif
    ADD_PUBLIC_FUNC(Dust_State_DustPuff_Friction);

    // Global/EggPrison
    ADD_PUBLIC_FUNC(EggPrison_HandleMovement);
    ADD_PUBLIC_FUNC(EggPrison_State_Opened);
    ADD_PUBLIC_FUNC(EggPrison_State_Init);
    ADD_PUBLIC_FUNC(EggPrison_State_Idle);
    ADD_PUBLIC_FUNC(EggPrison_State_Explode);
    ADD_PUBLIC_FUNC(EggPrison_State_SetupActClear);
    ADD_PUBLIC_FUNC(EggPrison_State_FlyOffScreen);

    // Global/GameOver
    ADD_PUBLIC_FUNC(GameOver_SaveGameCallback);
    ADD_PUBLIC_FUNC(GameOver_State_EnterLetters);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(GameOver_State_WaitComp);
#endif
    ADD_PUBLIC_FUNC(GameOver_State_Wait);
    ADD_PUBLIC_FUNC(GameOver_State_ExitLetters);

    // Global/HUD
    ADD_PUBLIC_FUNC(HUD_DrawNumbersBase10);
    ADD_PUBLIC_FUNC(HUD_DrawNumbersBase16);
    ADD_PUBLIC_FUNC(HUD_DrawNumbersHyperRing);
#if GAME_VERSION != VER_100
    ADD_PUBLIC_FUNC(HUD_GetButtonFrame);
    ADD_PUBLIC_FUNC(HUD_GetActionButtonFrames);
#endif
    ADD_PUBLIC_FUNC(HUD_State_MoveIn);
    ADD_PUBLIC_FUNC(HUD_State_MoveOut);
    ADD_PUBLIC_FUNC(HUD_MoveIn);
    ADD_PUBLIC_FUNC(HUD_MoveOut);
    ADD_PUBLIC_FUNC(HUD_EnableRingFlash);
    ADD_PUBLIC_FUNC(HUD_CharacterIndexFromID);

    // Global/InvisibleBlock
    ADD_PUBLIC_FUNC(InvisibleBlock_DrawSprites);

    // Global/ItemBox
    ADD_PUBLIC_FUNC(ItemBox_DebugSpawn);
    ADD_PUBLIC_FUNC(ItemBox_CheckHit);
    ADD_PUBLIC_FUNC(ItemBox_GivePowerup);
    ADD_PUBLIC_FUNC(ItemBox_Break);
    ADD_PUBLIC_FUNC(ItemBox_HandleFallingCollision);
    ADD_PUBLIC_FUNC(ItemBox_HandlePlatformCollision);
    ADD_PUBLIC_FUNC(ItemBox_HandleObjectCollisions);
    ADD_PUBLIC_FUNC(ItemBox_State_Broken);
    ADD_PUBLIC_FUNC(ItemBox_State_Break);
    ADD_PUBLIC_FUNC(ItemBox_State_IconFinish);
    ADD_PUBLIC_FUNC(ItemBox_State_Idle);
    ADD_PUBLIC_FUNC(ItemBox_State_Falling);
    ADD_PUBLIC_FUNC(ItemBox_State_Conveyor);

    // Global/Localization
    ADD_PUBLIC_FUNC(Localization_LoadStrings);
    ADD_PUBLIC_FUNC(Localization_GetString);
    ADD_PUBLIC_FUNC(Localization_GetZoneName);
    ADD_PUBLIC_FUNC(Localization_GetZoneInitials);

    // Global/Music
    ADD_PUBLIC_FUNC(Music_SetMusicTrack);
    ADD_PUBLIC_FUNC(Music_State_PlayOnLoad);
    ADD_PUBLIC_FUNC(Music_PlayJingle);
    ADD_PUBLIC_FUNC(Music_PlayTrack);
    ADD_PUBLIC_FUNC(Music_PlayTrackPtr);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Music_PlayAutoMusicQueuedTrack);
    ADD_PUBLIC_FUNC(Music_HandleMusicStack_Powerups);
    ADD_PUBLIC_FUNC(Music_CheckMusicStack_Active);
    ADD_PUBLIC_FUNC(Music_GetNextTrackStartPos);
    ADD_PUBLIC_FUNC(Music_JingleFadeOut);
#endif
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Music_FinishJingle);
    ADD_PUBLIC_FUNC(Music_ClearMusicStack);
    ADD_PUBLIC_FUNC(Music_TransitionTrack);
#endif
    ADD_PUBLIC_FUNC(Music_FadeOut);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Music_State_Jingle);
    ADD_PUBLIC_FUNC(Music_State_JingleFade);
    ADD_PUBLIC_FUNC(Music_State_FadeTrackIn);
#endif
    ADD_PUBLIC_FUNC(Music_State_StopOnFade);
    ADD_PUBLIC_FUNC(Music_State_PlayOnFade);
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Music_State_1UPJingle);
#endif

    // Global/PauseMenu
    ADD_PUBLIC_FUNC(PauseMenu_SetupMenu);
    ADD_PUBLIC_FUNC(PauseMenu_SetupTintTable);
    ADD_PUBLIC_FUNC(PauseMenu_AddButton);
    ADD_PUBLIC_FUNC(PauseMenu_ClearButtons);
    ADD_PUBLIC_FUNC(PauseMenu_HandleButtonPositions);
    ADD_PUBLIC_FUNC(PauseMenu_PauseSound);
    ADD_PUBLIC_FUNC(PauseMenu_ResumeSound);
    ADD_PUBLIC_FUNC(PauseMenu_StopSound);
    ADD_PUBLIC_FUNC(PauseMenu_FocusCamera);
    ADD_PUBLIC_FUNC(PauseMenu_UpdateCameras);
    ADD_PUBLIC_FUNC(PauseMenu_CheckAndReassignControllers);
    ADD_PUBLIC_FUNC(PauseMenu_IsDisconnected);
    ADD_PUBLIC_FUNC(PauseMenu_GetPlayerCount);
    ADD_PUBLIC_FUNC(PauseMenu_ResumeButtonCB);
    ADD_PUBLIC_FUNC(PauseMenu_RestartButtonCB);
    ADD_PUBLIC_FUNC(PauseMenu_ExitButtonCB);
    ADD_PUBLIC_FUNC(PauseMenu_RestartDialog_YesCB);
    ADD_PUBLIC_FUNC(PauseMenu_ExitDialog_YesCB);
    ADD_PUBLIC_FUNC(PauseMenu_RestartFadeCB);
    ADD_PUBLIC_FUNC(PauseMenu_ExitFadeCB);
    ADD_PUBLIC_FUNC(PauseMenu_ActionCB_Button);
    ADD_PUBLIC_FUNC(PauseMenu_State_SetupButtons);
    ADD_PUBLIC_FUNC(PauseMenu_State_StartPause);
    ADD_PUBLIC_FUNC(PauseMenu_State_StartPauseCompetition);
    ADD_PUBLIC_FUNC(PauseMenu_State_Paused);
    ADD_PUBLIC_FUNC(PauseMenu_State_ForcedPause);
    ADD_PUBLIC_FUNC(PauseMenu_State_ForcedPauseCompetition);
    ADD_PUBLIC_FUNC(PauseMenu_State_Resume);
    ADD_PUBLIC_FUNC(PauseMenu_State_ResumeCompetition);
    ADD_PUBLIC_FUNC(PauseMenu_State_ForcedResumeCompetition);
    ADD_PUBLIC_FUNC(PauseMenu_State_SetupTitleFade);
    ADD_PUBLIC_FUNC(PauseMenu_State_FadeToTitle);
    ADD_PUBLIC_FUNC(PauseMenu_State_HandleFadeout);
    ADD_PUBLIC_FUNC(PauseMenu_DrawPauseMenu);
    ADD_PUBLIC_FUNC(PauseMenu_Draw_RegularPause);
    ADD_PUBLIC_FUNC(PauseMenu_Draw_ForcePause);

    // Global/PlaneSwitch
    ADD_PUBLIC_FUNC(PlaneSwitch_DrawSprites);
    ADD_PUBLIC_FUNC(PlaneSwitch_CheckCollisions);

    // Global/Player
    ADD_PUBLIC_FUNC(Player_LoadSprites);
    ADD_PUBLIC_FUNC(Player_LoadSpritesVS);
    ADD_PUBLIC_FUNC(Player_SaveValues);
    ADD_PUBLIC_FUNC(Player_GiveScore);
    ADD_PUBLIC_FUNC(Player_GiveRings);
    ADD_PUBLIC_FUNC(Player_GiveLife);
    ADD_PUBLIC_FUNC(Player_ApplyShield);
    ADD_PUBLIC_FUNC(Player_ChangeCharacter);
    ADD_PUBLIC_FUNC(Player_TryTransform);
    ADD_PUBLIC_FUNC(Player_BlendSuperSonicColors);
    ADD_PUBLIC_FUNC(Player_BlendSuperTailsColors);
    ADD_PUBLIC_FUNC(Player_BlendSuperKnuxColors);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Player_BlendSuperMightyColors);
    ADD_PUBLIC_FUNC(Player_BlendSuperRayColors);
    ADD_PUBLIC_FUNC(Player_HandleSuperForm);
#endif
    ADD_PUBLIC_FUNC(Player_CheckP2KeyPress);
    ADD_PUBLIC_FUNC(Player_GetNearestPlayerX);
    ADD_PUBLIC_FUNC(Player_GetNearestPlayer);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Player_RemoveEncoreLeader);
    ADD_PUBLIC_FUNC(Player_ResetBoundaries);
#endif
    ADD_PUBLIC_FUNC(Player_HandleDeath);
    ADD_PUBLIC_FUNC(Player_HandleQuickRespawn);
    ADD_PUBLIC_FUNC(Player_GetHitbox);
    ADD_PUBLIC_FUNC(Player_GetAltHitbox);
    ADD_PUBLIC_FUNC(Player_CheckCollisionTouch);
    ADD_PUBLIC_FUNC(Player_CheckCollisionBox);
    ADD_PUBLIC_FUNC(Player_CheckCollisionPlatform);
    ADD_PUBLIC_FUNC(Player_Hurt);
    ADD_PUBLIC_FUNC(Player_HurtFlip);
    ADD_PUBLIC_FUNC(Player_ElementHurt);
    ADD_PUBLIC_FUNC(Player_CheckAttacking);
    ADD_PUBLIC_FUNC(Player_CheckBadnikTouch);
    ADD_PUBLIC_FUNC(Player_CheckBadnikBreak);
    ADD_PUBLIC_FUNC(Player_CheckBossHit);
    ADD_PUBLIC_FUNC(Player_ProjectileHurt);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Player_CheckMightyShellHit);
#endif
    ADD_PUBLIC_FUNC(Player_CheckItemBreak);
    ADD_PUBLIC_FUNC(Player_UpdatePhysicsState);
    ADD_PUBLIC_FUNC(Player_HandleGroundMovement);
    ADD_PUBLIC_FUNC(Player_HandleGroundRotation);
    ADD_PUBLIC_FUNC(Player_HandleAirRotation);
    ADD_PUBLIC_FUNC(Player_HandleAirMovement);
    ADD_PUBLIC_FUNC(Player_HandleAirFriction);
    ADD_PUBLIC_FUNC(Player_Action_Jump);
    ADD_PUBLIC_FUNC(Player_Action_Roll);
    ADD_PUBLIC_FUNC(Player_Action_Spindash);
    ADD_PUBLIC_FUNC(Player_Action_Peelout);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Player_SwapMainPlayer);
#endif
    ADD_PUBLIC_FUNC(Player_HandleRollDeceleration);
    ADD_PUBLIC_FUNC(Player_Hit);
    ADD_PUBLIC_FUNC(Player_CheckValidState);
    ADD_PUBLIC_FUNC(Player_HandleFlyCarry);
    ADD_PUBLIC_FUNC(Player_HandleSidekickRespawn);
    ADD_PUBLIC_FUNC(Player_State_StartSuper);
    ADD_PUBLIC_FUNC(Player_State_Static);
    ADD_PUBLIC_FUNC(Player_State_Ground);
    ADD_PUBLIC_FUNC(Player_State_Air);
    ADD_PUBLIC_FUNC(Player_State_Roll);
    ADD_PUBLIC_FUNC(Player_State_TubeRoll);
    ADD_PUBLIC_FUNC(Player_State_TubeAirRoll);
    ADD_PUBLIC_FUNC(Player_State_LookUp);
    ADD_PUBLIC_FUNC(Player_State_Crouch);
    ADD_PUBLIC_FUNC(Player_State_Spindash);
    ADD_PUBLIC_FUNC(Player_State_Peelout);
    ADD_PUBLIC_FUNC(Player_State_OuttaHere);
    ADD_PUBLIC_FUNC(Player_State_Transform);
    ADD_PUBLIC_FUNC(Player_State_Hurt);
    ADD_PUBLIC_FUNC(Player_State_Death);
    ADD_PUBLIC_FUNC(Player_State_Drown);
    ADD_PUBLIC_FUNC(Player_State_DropDash);
    ADD_PUBLIC_FUNC(Player_State_BubbleBounce);
    ADD_PUBLIC_FUNC(Player_State_TailsFlight);
    ADD_PUBLIC_FUNC(Player_State_FlyCarried);
    ADD_PUBLIC_FUNC(Player_State_KnuxGlideLeft);
    ADD_PUBLIC_FUNC(Player_State_KnuxGlideRight);
    ADD_PUBLIC_FUNC(Player_State_KnuxGlideDrop);
    ADD_PUBLIC_FUNC(Player_State_KnuxGlideSlide);
    ADD_PUBLIC_FUNC(Player_State_KnuxWallClimb);
    ADD_PUBLIC_FUNC(Player_State_KnuxLedgePullUp);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Player_State_MightyHammerDrop);
    ADD_PUBLIC_FUNC(Player_State_MightyUnspin);
    ADD_PUBLIC_FUNC(Player_SpawnMightyHammerdropDust);
    ADD_PUBLIC_FUNC(Player_CheckMightyUnspin);
    ADD_PUBLIC_FUNC(Player_State_RayGlide);
    ADD_PUBLIC_FUNC(Player_State_FlyToPlayer);
#endif
    ADD_PUBLIC_FUNC(Player_State_ReturnToPlayer);
    ADD_PUBLIC_FUNC(Player_State_HoldRespawn);
    ADD_PUBLIC_FUNC(Player_FinishedReturnToPlayer);
    ADD_PUBLIC_FUNC(Player_State_EncoreRespawn);
    ADD_PUBLIC_FUNC(Player_State_Victory);
    ADD_PUBLIC_FUNC(Player_State_Bubble);
    ADD_PUBLIC_FUNC(Player_State_WaterSlide);
    ADD_PUBLIC_FUNC(Player_State_TransportTube);
    ADD_PUBLIC_FUNC(Player_Gravity_False);
    ADD_PUBLIC_FUNC(Player_Gravity_True);
    ADD_PUBLIC_FUNC(Player_JumpAbility_Sonic);
    ADD_PUBLIC_FUNC(Player_JumpAbility_Tails);
    ADD_PUBLIC_FUNC(Player_JumpAbility_Knux);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Player_JumpAbility_Mighty);
    ADD_PUBLIC_FUNC(Player_JumpAbility_Ray);
    ADD_PUBLIC_FUNC(Player_SfxCheck_RayDive);
    ADD_PUBLIC_FUNC(Player_SfxCheck_RaySwoop);
    ADD_PUBLIC_FUNC(Player_SfxUpdate_RayDive);
    ADD_PUBLIC_FUNC(Player_SfxUpdate_RaySwoop);
    ADD_PUBLIC_FUNC(Player_Input_P1);
    ADD_PUBLIC_FUNC(Player_Input_P2_Delay);
    ADD_PUBLIC_FUNC(Player_Input_P2_AI);
    ADD_PUBLIC_FUNC(Player_Input_AI_SpindashPt1);
    ADD_PUBLIC_FUNC(Player_Input_AI_SpindashPt2);
    ADD_PUBLIC_FUNC(Player_Input_P2_Player);
#endif

    // Global/ReplayRecorder
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ReplayRecorder_TitleCardCB);
    ADD_PUBLIC_FUNC(ReplayRecorder_Resume);
    ADD_PUBLIC_FUNC(ReplayRecorder_StartCB);
    ADD_PUBLIC_FUNC(ReplayRecorder_FinishCB);
    ADD_PUBLIC_FUNC(ReplayRecorder_Buffer_Move);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveReplayDLG_NoCB);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveReplayDLG_YesCB);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveReplayDLG_CB);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveReplay);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveFile_Replay);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveCallback_ReplayDB);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveCallback_TimeAttackDB);
    ADD_PUBLIC_FUNC(ReplayRecorder_Buffer_PackInPlace);
    ADD_PUBLIC_FUNC(ReplayRecorder_Buffer_Unpack);
    ADD_PUBLIC_FUNC(ReplayRecorder_Buffer_LoadFile);
    ADD_PUBLIC_FUNC(ReplayRecorder_Buffer_SaveFile);
    ADD_PUBLIC_FUNC(ReplayRecorder_LoadReplayCallback);
    ADD_PUBLIC_FUNC(ReplayRecorder_SaveReplayCallback);
    ADD_PUBLIC_FUNC(ReplayRecorder_ConfigureGhost_CB);
    ADD_PUBLIC_FUNC(ReplayRecorder_SetupActions);
    ADD_PUBLIC_FUNC(ReplayRecorder_SetupWriteBuffer);
    ADD_PUBLIC_FUNC(ReplayRecorder_DrawGhostDisplay);
    ADD_PUBLIC_FUNC(ReplayRecorder_Record);
    ADD_PUBLIC_FUNC(ReplayRecorder_StartRecording);
    ADD_PUBLIC_FUNC(ReplayRecorder_Play);
    ADD_PUBLIC_FUNC(ReplayRecorder_Rewind);
    ADD_PUBLIC_FUNC(ReplayRecorder_Seek);
    ADD_PUBLIC_FUNC(ReplayRecorder_SeekFunc);
    ADD_PUBLIC_FUNC(ReplayRecorder_Stop);
    ADD_PUBLIC_FUNC(ReplayRecorder_SetGimmickState);
    ADD_PUBLIC_FUNC(ReplayRecorder_ForceApplyFramePtr);
    ADD_PUBLIC_FUNC(ReplayRecorder_ApplyFramePtr);
    ADD_PUBLIC_FUNC(ReplayRecorder_CheckPlayerGimmickState);
    ADD_PUBLIC_FUNC(ReplayRecorder_PackFrame);
    ADD_PUBLIC_FUNC(ReplayRecorder_PlayBackInput);
    ADD_PUBLIC_FUNC(ReplayRecorder_Pause);
    ADD_PUBLIC_FUNC(ReplayRecorder_PlayerState_PlaybackReplay);
    ADD_PUBLIC_FUNC(ReplayRecorder_State_SetupPlayback);
    ADD_PUBLIC_FUNC(ReplayRecorder_State_Playback);
    ADD_PUBLIC_FUNC(ReplayRecorder_State_Record);
    ADD_PUBLIC_FUNC(ReplayRecorder_Late_Playback);
    ADD_PUBLIC_FUNC(ReplayRecorder_Late_RecordFrames);
#endif

    // Global/Ring
    ADD_PUBLIC_FUNC(Ring_DebugSpawn);
    ADD_PUBLIC_FUNC(Ring_Collect);
    ADD_PUBLIC_FUNC(Ring_LoseRings);
    ADD_PUBLIC_FUNC(Ring_LoseHyperRings);
    ADD_PUBLIC_FUNC(Ring_FakeLoseRings);
    ADD_PUBLIC_FUNC(Ring_CheckPlatformCollisions);
    ADD_PUBLIC_FUNC(Ring_CheckObjectCollisions);
    ADD_PUBLIC_FUNC(Ring_State_Normal);
    ADD_PUBLIC_FUNC(Ring_State_Linear);
    ADD_PUBLIC_FUNC(Ring_State_Circular);
    ADD_PUBLIC_FUNC(Ring_State_Path);
    ADD_PUBLIC_FUNC(Ring_State_Track);
    ADD_PUBLIC_FUNC(Ring_State_Lost);
    ADD_PUBLIC_FUNC(Ring_State_LostFX);
    ADD_PUBLIC_FUNC(Ring_State_Big);
    ADD_PUBLIC_FUNC(Ring_State_Attracted);
    ADD_PUBLIC_FUNC(Ring_State_Sparkle);
    ADD_PUBLIC_FUNC(Ring_Draw_Normal);
    ADD_PUBLIC_FUNC(Ring_Draw_Oscillating);
    ADD_PUBLIC_FUNC(Ring_Draw_Sparkle);

#if MANIA_USE_PLUS
    // Global/SaveGame
    ADD_PUBLIC_FUNC(SaveGame_GetSaveRAM);
    ADD_PUBLIC_FUNC(SaveGame_GetDataPtr);
#else
    ADD_PUBLIC_FUNC(SaveGame_GetDataPtr);
    ADD_PUBLIC_FUNC(SaveGame_LoadSaveData);
#endif
    ADD_PUBLIC_FUNC(SaveGame_LoadFile);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(SaveGame_SaveFile);
#else
    ADD_PUBLIC_FUNC(SaveGame_SaveFile);
    ADD_PUBLIC_FUNC(SaveGame_SaveLoadedCB);
#endif
    ADD_PUBLIC_FUNC(SaveGame_SaveGameState);
    ADD_PUBLIC_FUNC(SaveGame_SaveProgress);
    ADD_PUBLIC_FUNC(SaveGame_ClearRestartData);
    ADD_PUBLIC_FUNC(SaveGame_SavePlayerState);
    ADD_PUBLIC_FUNC(SaveGame_LoadPlayerState);
    ADD_PUBLIC_FUNC(SaveGame_ResetPlayerState);
    ADD_PUBLIC_FUNC(SaveGame_LoadFile_CB);
    ADD_PUBLIC_FUNC(SaveGame_SaveFile_CB);
    ADD_PUBLIC_FUNC(SaveGame_AllChaosEmeralds);
    ADD_PUBLIC_FUNC(SaveGame_GetEmerald);
    ADD_PUBLIC_FUNC(SaveGame_SetEmerald);
    ADD_PUBLIC_FUNC(SaveGame_ClearCollectedSpecialRings);
    ADD_PUBLIC_FUNC(SaveGame_GetCollectedSpecialRing);
    ADD_PUBLIC_FUNC(SaveGame_SetCollectedSpecialRing);

    // Global/Shield
    ADD_PUBLIC_FUNC(Shield_State_Default);
    ADD_PUBLIC_FUNC(Shield_State_BubbleDrop);
    ADD_PUBLIC_FUNC(Shield_State_BubbleBounced);
    ADD_PUBLIC_FUNC(Shield_State_FireDash);
    ADD_PUBLIC_FUNC(Shield_State_LightningSparks);
    ADD_PUBLIC_FUNC(Shield_State_Insta);

    // Global/SignPost
    ADD_PUBLIC_FUNC(SignPost_DebugSpawn);
    ADD_PUBLIC_FUNC(SignPost_HandleSpin);
    ADD_PUBLIC_FUNC(SignPost_HandleSparkles);
    ADD_PUBLIC_FUNC(SignPost_HandleCamBounds);
    ADD_PUBLIC_FUNC(SignPost_CheckTouch);
    ADD_PUBLIC_FUNC(SignPost_State_Init);
    ADD_PUBLIC_FUNC(SignPost_State_AwaitTouch);
    ADD_PUBLIC_FUNC(SignPost_State_Spin);
    ADD_PUBLIC_FUNC(SignPost_State_SpinVS);
    ADD_PUBLIC_FUNC(SignPost_State_Launched);
    ADD_PUBLIC_FUNC(SignPost_State_Falling);
    ADD_PUBLIC_FUNC(SignPost_State_Done);

    // Global/Soundboard
    ADD_PUBLIC_FUNC(Soundboard_LoadSfx);

    // Global/SpecialRing
    ADD_PUBLIC_FUNC(SpecialRing_DebugSpawn);
    ADD_PUBLIC_FUNC(SpecialRing_State_Warp);
    ADD_PUBLIC_FUNC(SpecialRing_State_Flash);
    ADD_PUBLIC_FUNC(SpecialRing_State_Idle);

    // Global/SpeedGate
    ADD_PUBLIC_FUNC(SpeedGate_State_WaitForStart);
    ADD_PUBLIC_FUNC(SpeedGate_State_ProcessGate);
    ADD_PUBLIC_FUNC(SpeedGate_State_HandleFinished);

    // Global/Spikes
    ADD_PUBLIC_FUNC(Spikes_Draw_Global);
    ADD_PUBLIC_FUNC(Spikes_Draw_Stage);
    ADD_PUBLIC_FUNC(Spikes_Shatter);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Spikes_CheckHit);
#endif

    // Global/Spring
    ADD_PUBLIC_FUNC(Spring_State_Vertical);
    ADD_PUBLIC_FUNC(Spring_State_Horizontal);
    ADD_PUBLIC_FUNC(Spring_State_Diagonal);

    // Global/StarPost
    ADD_PUBLIC_FUNC(StarPost_DebugSpawn);
    ADD_PUBLIC_FUNC(StarPost_ResetStarPosts);
    ADD_PUBLIC_FUNC(StarPost_CheckBonusStageEntry);
    ADD_PUBLIC_FUNC(StarPost_CheckCollisions);
    ADD_PUBLIC_FUNC(StarPost_State_Idle);
    ADD_PUBLIC_FUNC(StarPost_State_Spinning);

    // Global/TimeAttackGate
    ADD_PUBLIC_FUNC(TimeAttackGate_HandleSpin);
    ADD_PUBLIC_FUNC(TimeAttackGate_HandleStart);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TimeAttackGate_AddRecord);
    ADD_PUBLIC_FUNC(TimeAttackGate_WaitSave_Leaderboards);
    ADD_PUBLIC_FUNC(TimeAttackGate_CheckTouch);
#endif
    ADD_PUBLIC_FUNC(TimeAttackGate_State_Gate);
    ADD_PUBLIC_FUNC(TimeAttackGate_State_Restarter);
    ADD_PUBLIC_FUNC(TimeAttackGate_State_Fadeout);
    ADD_PUBLIC_FUNC(TimeAttackGate_Draw_Gate);
    ADD_PUBLIC_FUNC(TimeAttackGate_Draw_Restarter);

#if MANIA_USE_PLUS
    // Global/TitleCard
    ADD_PUBLIC_FUNC(TitleCard_SetupColors);
    ADD_PUBLIC_FUNC(TitleCard_SetupVertices);
#endif
    ADD_PUBLIC_FUNC(TitleCard_SetupTitleWords);
    ADD_PUBLIC_FUNC(TitleCard_HandleWordMovement);
    ADD_PUBLIC_FUNC(TitleCard_HandleZoneCharMovement);
    ADD_PUBLIC_FUNC(TitleCard_HandleCamera);
    ADD_PUBLIC_FUNC(TitleCard_State_SetupBGElements);
    ADD_PUBLIC_FUNC(TitleCard_State_OpeningBG);
    ADD_PUBLIC_FUNC(TitleCard_State_EnterTitle);
    ADD_PUBLIC_FUNC(TitleCard_State_ShowingTitle);
    ADD_PUBLIC_FUNC(TitleCard_State_SlideAway);
    ADD_PUBLIC_FUNC(TitleCard_State_Supressed);
    ADD_PUBLIC_FUNC(TitleCard_Draw_SlideIn);
    ADD_PUBLIC_FUNC(TitleCard_Draw_ShowTitleCard);
    ADD_PUBLIC_FUNC(TitleCard_Draw_SlideAway);

    // Global/Zone
    ADD_PUBLIC_FUNC(Zone_GetZoneID);
    ADD_PUBLIC_FUNC(Zone_StoreEntities);
    ADD_PUBLIC_FUNC(Zone_ReloadStoredEntities);
    ADD_PUBLIC_FUNC(Zone_StartFadeOut);
    ADD_PUBLIC_FUNC(Zone_StartFadeIn);
    ADD_PUBLIC_FUNC(Zone_StartFadeOut_MusicFade);
    ADD_PUBLIC_FUNC(Zone_StartFadeOut_Competition);
    ADD_PUBLIC_FUNC(Zone_RotateOnPivot);
    ADD_PUBLIC_FUNC(Zone_ReloadScene);
    ADD_PUBLIC_FUNC(Zone_StartTeleportAction);
    ADD_PUBLIC_FUNC(Zone_ApplyWorldBounds);
    ADD_PUBLIC_FUNC(Zone_IsZoneLastAct);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Zone_GetListPos_EncoreMode);
    ADD_PUBLIC_FUNC(Zone_GetListPos_ManiaMode);
    ADD_PUBLIC_FUNC(Zone_Draw_Fade);
    ADD_PUBLIC_FUNC(Zone_State_FadeOut);
    ADD_PUBLIC_FUNC(Zone_State_FadeIn);
    ADD_PUBLIC_FUNC(Zone_State_FadeOut_Competition);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Zone_TitleCard_SupressCB);
    ADD_PUBLIC_FUNC(Zone_State_ReloadScene);
    ADD_PUBLIC_FUNC(Zone_State_FadeOut_Destroy);
#endif
    ADD_PUBLIC_FUNC(Zone_HandlePlayerSwap);
    ADD_PUBLIC_FUNC(Zone_State_SwapPlayers);
    ADD_PUBLIC_FUNC(Zone_State_HandleSwapFadeIn);

    // HCZ/Blastoid
    ADD_PUBLIC_FUNC(Blastoid_DebugSpawn);
    ADD_PUBLIC_FUNC(Blastoid_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Blastoid_State_Init);
    ADD_PUBLIC_FUNC(Blastoid_State_Body);
    ADD_PUBLIC_FUNC(Blastoid_State_Projectile);

    // HCZ/BreakBar
    ADD_PUBLIC_FUNC(BreakBar_DrawSprites);
    ADD_PUBLIC_FUNC(BreakBar_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(BreakBar_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(BreakBar_State_Init);
    ADD_PUBLIC_FUNC(BreakBar_State_Main);

    // HCZ/Buggernaut
    ADD_PUBLIC_FUNC(Buggernaut_DebugSpawn);
    ADD_PUBLIC_FUNC(Buggernaut_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Buggernaut_CheckOffScreen);
    ADD_PUBLIC_FUNC(Buggernaut_HandleTileCollisionsX);
    ADD_PUBLIC_FUNC(Buggernaut_HandleTileCollisionsY);
    ADD_PUBLIC_FUNC(Buggernaut_State_Init);
    ADD_PUBLIC_FUNC(Buggernaut_State_Idle);
    ADD_PUBLIC_FUNC(Buggernaut_State_FlyTowardTarget);
    ADD_PUBLIC_FUNC(Buggernaut_State_FlyAway);
    ADD_PUBLIC_FUNC(Buggernaut_State_Child);

    // HCZ/ButtonDoor
    ADD_PUBLIC_FUNC(ButtonDoor_SetupSize);
    ADD_PUBLIC_FUNC(ButtonDoor_SetupTagLink);
    ADD_PUBLIC_FUNC(ButtonDoor_DrawSprites);

    // HCZ/Current
    ADD_PUBLIC_FUNC(Current_SetupTagLink);
    ADD_PUBLIC_FUNC(Current_GetBubbleSpawnPosHorizontal);
    ADD_PUBLIC_FUNC(Current_GetBubbleSpawnPosVertical);
    ADD_PUBLIC_FUNC(Current_State_WaterLeft);
    ADD_PUBLIC_FUNC(Current_State_WaterRight);
    ADD_PUBLIC_FUNC(Current_State_WaterUp);
    ADD_PUBLIC_FUNC(Current_State_WaterDown);
    ADD_PUBLIC_FUNC(Current_State_PushLeft);
    ADD_PUBLIC_FUNC(Current_State_PushRight);
    ADD_PUBLIC_FUNC(Current_State_PushUp);
    ADD_PUBLIC_FUNC(Current_State_PushDown);
    ADD_PUBLIC_FUNC(Current_State_Child);
    ADD_PUBLIC_FUNC(Current_PlayerState_Left);
    ADD_PUBLIC_FUNC(Current_PlayerState_Right);
    ADD_PUBLIC_FUNC(Current_PlayerState_Up);
    ADD_PUBLIC_FUNC(Current_PlayerState_Down);

    // HCZ/DCEvent
    ADD_PUBLIC_FUNC(DCEvent_State_Collapse);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanBomber_AwaitPlayer);
    ADD_PUBLIC_FUNC(DCEvent_Input_MoveRight);
    ADD_PUBLIC_FUNC(DCEvent_Input_LookDown);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanBomber_WaitForLookDown);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanBomber_Swimming);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanBomber_PlaceBomb);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanBomber_PlacedAllBombs);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanSwim_AwaitPlayer);
    ADD_PUBLIC_FUNC(DCEvent_StateEggmanSwim_Swimming);
    ADD_PUBLIC_FUNC(DCEvent_State_Bomb);
    ADD_PUBLIC_FUNC(DCEvent_State_BombExplode);

    // HCZ/DiveEggman
    ADD_PUBLIC_FUNC(DiveEggman_Hit);
    ADD_PUBLIC_FUNC(DiveEggman_Explode);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_AwaitPlayer);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_Swimming);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_InWhirlpool);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_WhirlpoolRise);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_Falling);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_PlaceBomb);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_Destroyed);
    ADD_PUBLIC_FUNC(DiveEggman_StateEggman_Finish);
    ADD_PUBLIC_FUNC(DiveEggman_CheckNoBombExplode);
    ADD_PUBLIC_FUNC(DiveEggman_StateBomb_Idle);
    ADD_PUBLIC_FUNC(DiveEggman_StateBomb_InWhirlpool);
    ADD_PUBLIC_FUNC(DiveEggman_StateBomb_WhirlpoolRise);
    ADD_PUBLIC_FUNC(DiveEggman_StateBomb_Falling);

    // HCZ/Fan
    ADD_PUBLIC_FUNC(Fan_SetupTagLink);
    ADD_PUBLIC_FUNC(Fan_HandlePlayerInteractions_Top);
    ADD_PUBLIC_FUNC(Fan_HandlePlayerInteractions_Bottom);
    ADD_PUBLIC_FUNC(Fan_HandlePlayerInteractions_Left);
    ADD_PUBLIC_FUNC(Fan_HandlePlayerInteractions_Right);
    ADD_PUBLIC_FUNC(Fan_State_Started);
    ADD_PUBLIC_FUNC(Fan_State_Stopped);
    ADD_PUBLIC_FUNC(Fan_Activate_Interval);
    ADD_PUBLIC_FUNC(Fan_Activate_Button);
    ADD_PUBLIC_FUNC(Fan_Deactivate_Button);
    ADD_PUBLIC_FUNC(Fan_Activate_Platform);
    ADD_PUBLIC_FUNC(Fan_Activate);

    // HCZ/Gondola
    ADD_PUBLIC_FUNC(Gondola_GetWaterLevel);
    ADD_PUBLIC_FUNC(Gondola_HandleWaterFloating);
    ADD_PUBLIC_FUNC(Gondola_HandleTilting);
    ADD_PUBLIC_FUNC(Gondola_HandleMoveVelocity);
    ADD_PUBLIC_FUNC(Gondola_HandleTileCollisions);
    ADD_PUBLIC_FUNC(Gondola_HandlePlayerInteractions);

    // HCZ/HandLauncher
    ADD_PUBLIC_FUNC(HandLauncher_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(HandLauncher_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(HandLauncher_ReleasePlayers);
    ADD_PUBLIC_FUNC(HandLauncher_HandleGrabbedPlayers);
    ADD_PUBLIC_FUNC(HandLauncher_State_Init);
    ADD_PUBLIC_FUNC(HandLauncher_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(HandLauncher_State_TryGrabPlayer);
    ADD_PUBLIC_FUNC(HandLauncher_State_GrabbedPlayer);

    // HCZ/HangConveyor
    ADD_PUBLIC_FUNC(HangConveyor_DrawSprites);
    ADD_PUBLIC_FUNC(HangConveyor_SetupHitboxes);
    ADD_PUBLIC_FUNC(HangConveyor_HandlePlayerInteractions);

    // HCZ/HCZ1Intro
    ADD_PUBLIC_FUNC(HCZ1Intro_Cutscene_Intro);

    // HCZ/HCZOneWayDoor
    ADD_PUBLIC_FUNC(HCZOneWayDoor_SetupHitboxes);
    ADD_PUBLIC_FUNC(HCZOneWayDoor_DrawSprites);

    // HCZ/HCZSetup
    ADD_PUBLIC_FUNC(HCZSetup_Scanline_WaterLine);
    ADD_PUBLIC_FUNC(HCZSetup_StageFinish_EndAct1);
    ADD_PUBLIC_FUNC(HCZSetup_StageFinish_EndAct2);

    // HCZ/HCZSpikeBall
    ADD_PUBLIC_FUNC(HCZSpikeBall_HandleConveyorSetup);
    ADD_PUBLIC_FUNC(HCZSpikeBall_LinkToConveyor);
    ADD_PUBLIC_FUNC(HCZSpikeBall_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(HCZSpikeBall_HandleConveyorMovement);

    // HCZ/Jawz
    ADD_PUBLIC_FUNC(Jawz_DebugSpawn);
    ADD_PUBLIC_FUNC(Jawz_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Jawz_State_CheckPlayerTrigger);
    ADD_PUBLIC_FUNC(Jawz_State_Triggered);

    // HCZ/Jellygnite
    ADD_PUBLIC_FUNC(Jellygnite_DebugSpawn);
    ADD_PUBLIC_FUNC(Jellygnite_SetupAnimations);
    ADD_PUBLIC_FUNC(Jellygnite_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Jellygnite_HandlePlayerStruggle);
    ADD_PUBLIC_FUNC(Jellygnite_CheckInWater);
    ADD_PUBLIC_FUNC(Jellygnite_DrawBackTentacle);
    ADD_PUBLIC_FUNC(Jellygnite_DrawFrontTentacle);
    ADD_PUBLIC_FUNC(Jellygnite_State_Init);
    ADD_PUBLIC_FUNC(Jellygnite_State_Swimming);
    ADD_PUBLIC_FUNC(Jellygnite_State_GrabbedPlayer);
    ADD_PUBLIC_FUNC(Jellygnite_State_Explode);

    // HCZ/LaundroMobile
    ADD_PUBLIC_FUNC(LaundroMobile_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(LaundroMobile_Explode);
    ADD_PUBLIC_FUNC(LaundroMobile_HandleStageWrap);
    ADD_PUBLIC_FUNC(LaundroMobile_HandleRocketMovement);
    ADD_PUBLIC_FUNC(LaundroMobile_HandleEggmanAnimations);
    ADD_PUBLIC_FUNC(LaundroMobile_HandleTileCollisions);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_AwaitPlayer_Phase1);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_SetupArena_Phase1);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_EnterEggman_Phase1);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_StartupPropellers);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_HandlePhase1);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_Destroyed_Phase1);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_Explode_Phase1);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_WaitForLastStageWrap);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_AwaitPlayer_Phase2);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_SetupArena_Phase2);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_EnterEggman_Phase2);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_StartupRockets);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_SpeedUpRockets);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_RiseUpToAttack);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_Attacking);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_ReturnToLaundry);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_PrepareWhirlpool);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_StartupWhirlpool);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_WhirlpoolActive);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_PrepareRockets);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_Destroyed_Phase2);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBoss_Explode_Phase2);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(LaundroMobile_StageFinish_Wait);
    ADD_PUBLIC_FUNC(LaundroMobile_StateOutro_StartCutscene);
    ADD_PUBLIC_FUNC(LaundroMobile_StateOutro_Rumble);
    ADD_PUBLIC_FUNC(LaundroMobile_StateOutro_WaterGush);
    ADD_PUBLIC_FUNC(LaundroMobile_StateOutro_ExitHCZ);
    ADD_PUBLIC_FUNC(LaundroMobile_Draw_Boss);
#endif
    ADD_PUBLIC_FUNC(LaundroMobile_Draw_Boss_Destroyed);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBomb_Spawner);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBomb_Bomb_Idle);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBomb_Bomb_Activated);
    ADD_PUBLIC_FUNC(LaundroMobile_Draw_Simple);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBlock_Spawner);
    ADD_PUBLIC_FUNC(LaundroMobile_StateBlock_Block);
    ADD_PUBLIC_FUNC(LaundroMobile_State_Laundry);
    ADD_PUBLIC_FUNC(LaundroMobile_Draw_Laundry);
    ADD_PUBLIC_FUNC(LaundroMobile_State_DelayedSplash);

    // HCZ/MegaChopper
    ADD_PUBLIC_FUNC(MegaChopper_DebugSpawn);
    ADD_PUBLIC_FUNC(MegaChopper_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MegaChopper_CheckOffScreen);
    ADD_PUBLIC_FUNC(MegaChopper_Input_GrabbedP1);
    ADD_PUBLIC_FUNC(MegaChopper_Input_GrabbedP2);
    ADD_PUBLIC_FUNC(MegaChopper_Input_GrabbedP2_AI);
    ADD_PUBLIC_FUNC(MegaChopper_State_Init);
    ADD_PUBLIC_FUNC(MegaChopper_State_InWater);
    ADD_PUBLIC_FUNC(MegaChopper_State_OutOfWater);
    ADD_PUBLIC_FUNC(MegaChopper_State_Chopping);
    ADD_PUBLIC_FUNC(MegaChopper_State_ShakenOff);

    // HCZ/Pointdexter
    ADD_PUBLIC_FUNC(Pointdexter_DebugSpawn);
    ADD_PUBLIC_FUNC(Pointdexter_CheckOffScreen);
    ADD_PUBLIC_FUNC(Pointdexter_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Pointdexter_State_Init);
    ADD_PUBLIC_FUNC(Pointdexter_State_Swimming);

#if GAME_VERSION == VER_100
    // HCZ/PullChain
    ADD_PUBLIC_FUNC(PullChain_HandleDunkeyCode);
#endif

    // HCZ/ScrewMobile
    ADD_PUBLIC_FUNC(ScrewMobile_State_CheckPlayerEnter);
    ADD_PUBLIC_FUNC(ScrewMobile_State_PlayerRiding);
    ADD_PUBLIC_FUNC(ScrewMobile_State_BossFinished);
    ADD_PUBLIC_FUNC(ScrewMobile_State_Idle);
    ADD_PUBLIC_FUNC(ScrewMobile_Draw_ScrewMobile);
    ADD_PUBLIC_FUNC(ScrewMobile_StateDepthCharge_Active);
    ADD_PUBLIC_FUNC(ScrewMobile_StateDepthCharge_Debris);
    ADD_PUBLIC_FUNC(ScrewMobile_Draw_DepthCharge);

    // HCZ/Spear
    ADD_PUBLIC_FUNC(Spear_SetupHitboxes);

    // HCZ/TurboSpiker
    ADD_PUBLIC_FUNC(TurboSpiker_DebugSpawn);
#endif
    ADD_PUBLIC_FUNC(TurboSpiker_Hermit_Collide);
    ADD_PUBLIC_FUNC(TurboSpiker_Hermit_CheckOffScreen);
    ADD_PUBLIC_FUNC(TurboSpiker_HandleMovement);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Init);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Idle);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Hidden);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Moving);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Turning);
    ADD_PUBLIC_FUNC(TurboSpiker_State_FinishTurning);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Falling);
    ADD_PUBLIC_FUNC(TurboSpiker_State_FireShell);
    ADD_PUBLIC_FUNC(TurboSpiker_HandleSpikeCollisions);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Spike);
    ADD_PUBLIC_FUNC(TurboSpiker_State_Ember);

    // HCZ/TwistingSlide
    ADD_PUBLIC_FUNC(TwistingSlide_SetupHitboxes);

    // HCZ/WaterGush
    ADD_PUBLIC_FUNC(WaterGush_SetupHitboxes);
    ADD_PUBLIC_FUNC(WaterGush_DrawSprites);
    ADD_PUBLIC_FUNC(WaterGush_SpawnBrickDebris);

    // HCZ/Whirlpool
    ADD_PUBLIC_FUNC(WhirlPool_DrawSprites);
    ADD_PUBLIC_FUNC(Whirlpool_SetupBubbles);

    // Helpers/BadnikHelpers
    ADD_PUBLIC_FUNC(BadnikHelpers_Oscillate);

    // Helpers/ColorHelpers
    ADD_PUBLIC_FUNC(ColorHelpers_PackRGB);
    ADD_PUBLIC_FUNC(ColorHelpers_RGBToHSL);
    ADD_PUBLIC_FUNC(ColorHelpers_HSLToRGB);

    // Helpers/CompetitionSession
    ADD_PUBLIC_FUNC(CompetitionSession_GetSession);
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(CompetitionSession_ResetOptions);
    ADD_PUBLIC_FUNC(CompetitionSession_ClearMatchData);
    ADD_PUBLIC_FUNC(CompetitionSession_DeriveWinner);
    ADD_PUBLIC_FUNC(CompetitionSession_WinMatchFor);
#endif

    // Helpers/DrawHelpers
    ADD_PUBLIC_FUNC(DrawHelpers_DrawHitboxOutline);
    ADD_PUBLIC_FUNC(DrawHelpers_DrawArrowAdditive);
    ADD_PUBLIC_FUNC(DrawHelpers_DrawIsocelesTriangle);
    ADD_PUBLIC_FUNC(DrawHelpers_DrawCross);
    ADD_PUBLIC_FUNC(DrawHelpers_DrawArrow);
    ADD_PUBLIC_FUNC(DrawHelpers_DrawRectOutline);
    ADD_PUBLIC_FUNC(DrawHelpers_DrawArenaBounds);

    // Helpers/GameProgress
    ADD_PUBLIC_FUNC(GameProgress_GetNotifStringID);
    ADD_PUBLIC_FUNC(GameProgress_ShuffleBSSID);
    ADD_PUBLIC_FUNC(GameProgress_GetProgressRAM);
    ADD_PUBLIC_FUNC(GameProgress_GetZoneUnlocked);
    ADD_PUBLIC_FUNC(GameProgress_GetCompletionPercent);
    ADD_PUBLIC_FUNC(GameProgress_TrackGameProgress);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(GameProgress_ClearBSSSave);
#endif
    ADD_PUBLIC_FUNC(GameProgress_UnlockAll);
    ADD_PUBLIC_FUNC(GameProgress_LockAllSpecialClear);
    ADD_PUBLIC_FUNC(GameProgress_ClearProgress);
    ADD_PUBLIC_FUNC(GameProgress_MarkZoneCompleted);
    ADD_PUBLIC_FUNC(GameProgress_CheckZoneClear);
    ADD_PUBLIC_FUNC(GameProgress_GiveEmerald);
    ADD_PUBLIC_FUNC(GameProgress_GiveMedal);
    ADD_PUBLIC_FUNC(GameProgress_GiveEnding);
    ADD_PUBLIC_FUNC(GameProgress_PrintSaveProgress);
    ADD_PUBLIC_FUNC(GameProgress_CountUnreadNotifs);
    ADD_PUBLIC_FUNC(GameProgress_GetNextNotif);
    ADD_PUBLIC_FUNC(GameProgress_CheckUnlock);

    // Helpers/LogHelpers
    ADD_PUBLIC_FUNC(LogHelpers_Print);

    // Helpers/MathHelpers
    ADD_PUBLIC_FUNC(MathHelpers_LerpToPos);
    ADD_PUBLIC_FUNC(MathHelpers_Lerp);
    ADD_PUBLIC_FUNC(MathHelpers_LerpSin1024);
    ADD_PUBLIC_FUNC(MathHelpers_Lerp2Sin1024);
    ADD_PUBLIC_FUNC(MathHelpers_LerpSin512);
    ADD_PUBLIC_FUNC(MathHelpers_GetBezierPoint);
    ADD_PUBLIC_FUNC(MathHelpers_SquareRoot);
    ADD_PUBLIC_FUNC(MathHelpers_GetBezierCurveLength);
    ADD_PUBLIC_FUNC(MathHelpers_PointInHitbox);
    ADD_PUBLIC_FUNC(MathHelpers_GetInteractionDir);
    ADD_PUBLIC_FUNC(MathHelpers_CheckValidIntersect);
    ADD_PUBLIC_FUNC(MathHelpers_GetEdgeDistance);
    ADD_PUBLIC_FUNC(MathHelpers_ConstrainToBox);

    // Helpers/Options
    ADD_PUBLIC_FUNC(Options_GetOptionsRAM);
    ADD_PUBLIC_FUNC(Options_Reload);
    ADD_PUBLIC_FUNC(Options_GetWinSize);
    ADD_PUBLIC_FUNC(Options_LoadCallback);
    ADD_PUBLIC_FUNC(Options_LoadFile);
    ADD_PUBLIC_FUNC(Options_SaveFile);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Options_SetLanguage);
#endif
    ADD_PUBLIC_FUNC(Options_LoadValuesFromSettings);
    ADD_PUBLIC_FUNC(Options_LoadOptionsCallback);
    ADD_PUBLIC_FUNC(Options_SaveOptionsCallback);

    // Helpers/ParticleHelpers
    ADD_PUBLIC_FUNC(ParticleHelpers_SetupFallingParticles);

    // Helpers/PlayerHelpers
    ADD_PUBLIC_FUNC(CutsceneRules_IsAct1);
    ADD_PUBLIC_FUNC(CutsceneRules_IsAct2);
    ADD_PUBLIC_FUNC(CutsceneRules_IsIntroEnabled);
    ADD_PUBLIC_FUNC(CutsceneRules_IsAct1Regular);
    ADD_PUBLIC_FUNC(CutsceneRules_CheckStageReload);
    ADD_PUBLIC_FUNC(CutsceneRules_CheckPlayerPos);

    // Helpers/PlayerProbe
    ADD_PUBLIC_FUNC(PlayerProbe_Print);
    ADD_PUBLIC_FUNC(PlayerProbe_DrawSprites);
    ADD_PUBLIC_FUNC(PlayerProbe_DrawArrow);

    // Helpers/ReplayDB
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ReplayDB_CreateDB);
    ADD_PUBLIC_FUNC(ReplayDB_LoadDB);
    ADD_PUBLIC_FUNC(ReplayDB_SaveDB);
    ADD_PUBLIC_FUNC(ReplayDB_AddReplay);
    ADD_PUBLIC_FUNC(ReplayDB_DeleteReplay);
    ADD_PUBLIC_FUNC(ReplayDB_DeleteReplay_CB);
    ADD_PUBLIC_FUNC(ReplayDB_DeleteReplaySave_CB);
    ADD_PUBLIC_FUNC(ReplayDB_DeleteReplaySave2_CB);
    ADD_PUBLIC_FUNC(ReplayDB_LoadDBCallback);
    ADD_PUBLIC_FUNC(ReplayDB_SaveDBCallback);
    ADD_PUBLIC_FUNC(ReplayDB_LoadCallback);

    ADD_PUBLIC_FUNC(ReplayDB_Buffer_PackEntry);
    ADD_PUBLIC_FUNC(ReplayDB_Buffer_UnpackEntry);
#endif

#if MANIA_USE_PLUS
    // Helpers/TimeAttackData
    ADD_PUBLIC_FUNC(TimeAttackData_TrackActClear);
    ADD_PUBLIC_FUNC(TimeAttackData_TrackTAClear);
    ADD_PUBLIC_FUNC(TimeAttackData_TrackEnemyDefeat);
    ADD_PUBLIC_FUNC(TimeAttackData_Clear);
#endif
    ADD_PUBLIC_FUNC(TimeAttackData_GetManiaListPos);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TimeAttackData_GetEncoreListPos);
#endif
    ADD_PUBLIC_FUNC(TimeAttackData_GetUnpackedTime);
    ADD_PUBLIC_FUNC(TimeAttackData_GetRecordedTime);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TimeAttackData_CreateDB);
    ADD_PUBLIC_FUNC(TimeAttackData_LoadDB);
    ADD_PUBLIC_FUNC(TimeAttackData_SaveDB);
    ADD_PUBLIC_FUNC(TimeAttackData_LoadDBCallback);
    ADD_PUBLIC_FUNC(TimeAttackData_SaveDBCallback);
    ADD_PUBLIC_FUNC(TimeAttackData_MigrateLegacySaves);
    ADD_PUBLIC_FUNC(TimeAttackData_AddDBRow);
    ADD_PUBLIC_FUNC(TimeAttackData_AddRecord);
    ADD_PUBLIC_FUNC(TimeAttackData_GetScore);
    ADD_PUBLIC_FUNC(TimeAttackData_GetReplayID);
    ADD_PUBLIC_FUNC(TimeAttackData_ConfigureTableView);
    ADD_PUBLIC_FUNC(TimeAttackData_Leaderboard_GetRank);
    ADD_PUBLIC_FUNC(TimeAttackData_AddLeaderboardEntry);
    ADD_PUBLIC_FUNC(TimeAttackData_GetLeaderboardInfo);
#else
    ADD_PUBLIC_FUNC(TimeAttackData_AddRecord);
#endif

    // HPZ/Batbot
    ADD_PUBLIC_FUNC(Batbot_DebugSpawn);
    ADD_PUBLIC_FUNC(Batbot_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Batbot_State_Init);
    ADD_PUBLIC_FUNC(Batbot_State_Idle);
    ADD_PUBLIC_FUNC(Batbot_State_Attack);
    ADD_PUBLIC_FUNC(Batbot_State_SwoopLeft);
    ADD_PUBLIC_FUNC(Batbot_State_SwoopRight);

    // HPZ/Redz
    ADD_PUBLIC_FUNC(Redz_DebugSpawn);
    ADD_PUBLIC_FUNC(Redz_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Redz_State_Init);
    ADD_PUBLIC_FUNC(Redz_State_Walk);
    ADD_PUBLIC_FUNC(Redz_State_Turn);
    ADD_PUBLIC_FUNC(Redz_State_PrepareAttack);
    ADD_PUBLIC_FUNC(Redz_State_Attack);
    ADD_PUBLIC_FUNC(Redz_Flame_Setup);
    ADD_PUBLIC_FUNC(Redz_Flame_State);

    // HPZ/Stegway
    ADD_PUBLIC_FUNC(Stegway_DebugSpawn);
    ADD_PUBLIC_FUNC(Stegway_CheckOffScreen);
    ADD_PUBLIC_FUNC(Stegway_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Stegway_SetupAnims);
    ADD_PUBLIC_FUNC(Stegway_State_Init);
    ADD_PUBLIC_FUNC(Stegway_State_Moving);
    ADD_PUBLIC_FUNC(Stegway_State_Turn);
    ADD_PUBLIC_FUNC(Stegway_State_RevUp);
    ADD_PUBLIC_FUNC(Stegway_State_RevRelease);
    ADD_PUBLIC_FUNC(Stegway_State_Dash);

    // LRZ/BuckwildBall
    ADD_PUBLIC_FUNC(BuckwildBall_HandleTimerSfx);
    ADD_PUBLIC_FUNC(BuckwildBall_CheckOffScreen);
    ADD_PUBLIC_FUNC(BuckwildBall_SpawnDebris);
    ADD_PUBLIC_FUNC(BuckwildBall_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(BuckwildBall_HandleRollCrush);
    ADD_PUBLIC_FUNC(BuckwildBall_State_Init);
    ADD_PUBLIC_FUNC(BuckwildBall_State_Patrolling);
    ADD_PUBLIC_FUNC(BuckwildBall_State_AwaitDetection);
    ADD_PUBLIC_FUNC(BuckwildBall_State_Falling);
    ADD_PUBLIC_FUNC(BuckwildBall_State_Rolling);
    ADD_PUBLIC_FUNC(BuckwildBall_State_Debris);

    // LRZ/DashLift
    ADD_PUBLIC_FUNC(DashLift_CheckOffScreen);
    ADD_PUBLIC_FUNC(DashLift_State_Init);
    ADD_PUBLIC_FUNC(DashLift_State_HandleDash);

    // LRZ/Drillerdroid
    ADD_PUBLIC_FUNC(Drillerdroid_Hit);
    ADD_PUBLIC_FUNC(Drillerdroid_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Drillerdroid_Explode);
    ADD_PUBLIC_FUNC(Drillerdroid_SpawnDebris);
    ADD_PUBLIC_FUNC(Drillerdroid_State_SetupArena);
    ADD_PUBLIC_FUNC(Drillerdroid_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Dropping);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Landed);
    ADD_PUBLIC_FUNC(Drillerdroid_State_LandRecoil);
    ADD_PUBLIC_FUNC(Drillerdroid_State_DecideNextMove);
    ADD_PUBLIC_FUNC(Drillerdroid_State_PrepareJump);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Jumping);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Drilling);
    ADD_PUBLIC_FUNC(Drillerdroid_State_FinishDrilling);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Overheat);
    ADD_PUBLIC_FUNC(Drillerdroid_State_OverheatRecoil);
    ADD_PUBLIC_FUNC(Drillerdroid_State_JumpTargetDelay);
    ADD_PUBLIC_FUNC(Drillerdroid_State_PrepareJumpTarget);
    ADD_PUBLIC_FUNC(Drillerdroid_State_JumpTargeting);
    ADD_PUBLIC_FUNC(Drillerdroid_State_DecidingDropPos);
    ADD_PUBLIC_FUNC(Drillerdroid_State_DropFailReset);
    ADD_PUBLIC_FUNC(Drillerdroid_State_OverheatRecoil_DestroyedSegment);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Cooldown);
    ADD_PUBLIC_FUNC(Drillerdroid_State_ResetFromCooldown);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Destroyed);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Finish);
    ADD_PUBLIC_FUNC(Drillerdroid_State_DropSignPost);
    ADD_PUBLIC_FUNC(Drillerdroid_Draw_Boss);
    ADD_PUBLIC_FUNC(Drillerdroid_Draw_Simple);
    ADD_PUBLIC_FUNC(Drillerdroid_State_Target);
    ADD_PUBLIC_FUNC(Drillerdroid_Draw_Target);

    // LRZ/DrillerdroidO
    ADD_PUBLIC_FUNC(DrillerdroidO_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(DrillerdroidO_Explode);
    ADD_PUBLIC_FUNC(DrillerdroidO_SpawnDebris);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_SetupArena);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Dropping);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Landed);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_LandRecoil);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_BeginDrilling);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Drilling);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_FinishDrilling);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Overheat);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_OverheatRecoil);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_JumpTargetDelay);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_PrepareJumpTarget);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_JumpTargeting);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_DestroyRockPiles);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_NextLevelDelay);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_MoveToNextLevel);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_DecidingDropPos);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_OverheatRecoil_DestroyedSegment);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Cooldown);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_ResetFromCooldown);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Destroyed);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Finish);
    ADD_PUBLIC_FUNC(DrillerdroidO_Draw_Boss);
    ADD_PUBLIC_FUNC(DrillerdroidO_Draw_Simple);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_Target);
    ADD_PUBLIC_FUNC(DrillerdroidO_Draw_Target);
    ADD_PUBLIC_FUNC(DrillerdroidO_State_FireballEmitter);
    ADD_PUBLIC_FUNC(DrillerdroidO_Draw_FireballEmitter);

    // LRZ/Fireworm
    ADD_PUBLIC_FUNC(Fireworm_DebugSpawn);
    ADD_PUBLIC_FUNC(Fireworm_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Fireworm_CheckOffScreen);
    ADD_PUBLIC_FUNC(Fireworm_State_Init);
    ADD_PUBLIC_FUNC(Fireworm_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(Fireworm_State_HeadAppear);
    ADD_PUBLIC_FUNC(Fireworm_State_BodyAppear);
    ADD_PUBLIC_FUNC(Fireworm_State_FlyAround);

    // LRZ/Flamethrower
    ADD_PUBLIC_FUNC(Flamethrower_SetupOrientation);
    ADD_PUBLIC_FUNC(Flamethrower_GetHitbox);
    ADD_PUBLIC_FUNC(Flamethrower_CheckOffScreen);
    ADD_PUBLIC_FUNC(Flamethrower_HandleAnimations);
    ADD_PUBLIC_FUNC(Flamethrower_HandleAngles);
    ADD_PUBLIC_FUNC(Flamethrower_HandleTileCollisions);
    ADD_PUBLIC_FUNC(Flamethrower_CheckOutOfBounds);
    ADD_PUBLIC_FUNC(Flamethrower_CheckFlameCollisions);
    ADD_PUBLIC_FUNC(Flamethrower_CheckMouthCollisions);
    ADD_PUBLIC_FUNC(Flamethrower_State_Init);
    ADD_PUBLIC_FUNC(Flamethrower_State_AwaitInterval);
    ADD_PUBLIC_FUNC(Flamethrower_State_EmittingFlames);
    ADD_PUBLIC_FUNC(Flamethrower_State_SetupFireball);
    ADD_PUBLIC_FUNC(Flamethrower_State_Fireball);

    // LRZ/HeavyKing
    ADD_PUBLIC_FUNC(HeavyKing_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(HeavyKing_CheckPlayerCollisions_Charging);
    ADD_PUBLIC_FUNC(HeavyKing_Hit);
    ADD_PUBLIC_FUNC(HeavyKing_HandleClawMovement);
    ADD_PUBLIC_FUNC(HeavyKing_HandleAnimators);
    ADD_PUBLIC_FUNC(HeavyKing_HandleHoverMovement);
    ADD_PUBLIC_FUNC(HeavyKing_FindTargetEmerald);
    ADD_PUBLIC_FUNC(HeavyKing_StartLeap);
    ADD_PUBLIC_FUNC(HeavyKing_HandleAttackFinish);
    ADD_PUBLIC_FUNC(HeavyKing_CreateSpinRayFX);
    ADD_PUBLIC_FUNC(HeavyKing_CreateExpandRingFX);
    ADD_PUBLIC_FUNC(HeavyKing_CreateLaser);
    ADD_PUBLIC_FUNC(HeavyKing_CreateExpandRing);
    ADD_PUBLIC_FUNC(HeavyKing_State_SetupArena);
    ADD_PUBLIC_FUNC(HeavyKing_State_HandleCutsceneSetup);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_PlayerLookUp);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_EnterKing);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_ReturnCamToPlayer);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_GrabMasterEmerald);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_FinishThinking);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_GetHigherGround);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_Complaining);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_ChargeSpindash);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_AttackClaw);
    ADD_PUBLIC_FUNC(HeavyKing_StateCutscene_AttackRebound);
    ADD_PUBLIC_FUNC(HeavyKing_State_JumpToTargetEmerald);
    ADD_PUBLIC_FUNC(HeavyKing_State_Leaping);
    ADD_PUBLIC_FUNC(HeavyKing_State_LeapToMasterEmerald);
    ADD_PUBLIC_FUNC(HeavyKing_State_LandedOnMasterEmerald);
    ADD_PUBLIC_FUNC(HeavyKing_State_ChargeStart);
    ADD_PUBLIC_FUNC(HeavyKing_State_Charging);
    ADD_PUBLIC_FUNC(HeavyKing_State_ChargeStop);
    ADD_PUBLIC_FUNC(HeavyKing_State_PrepareHover);
    ADD_PUBLIC_FUNC(HeavyKing_State_StartHovering);
    ADD_PUBLIC_FUNC(HeavyKing_State_Hovering);
    ADD_PUBLIC_FUNC(HeavyKing_State_LaserAttack);
    ADD_PUBLIC_FUNC(HeavyKing_State_ExpandRingAttack);
    ADD_PUBLIC_FUNC(HeavyKing_State_TwinChargeAttack);
    ADD_PUBLIC_FUNC(HeavyKing_State_HitRecoil);
    ADD_PUBLIC_FUNC(HeavyKing_State_Destroyed);
    ADD_PUBLIC_FUNC(HeavyKing_State_Escape);
    ADD_PUBLIC_FUNC(HeavyKing_State_Finish);

    // LRZ/HeavyRider
    ADD_PUBLIC_FUNC(HeavyRider_SpawnDebris);
    ADD_PUBLIC_FUNC(HeavyRider_CheckObjectCollisions);
    ADD_PUBLIC_FUNC(HeavyRider_Hit);
    ADD_PUBLIC_FUNC(HeavyRider_Explode);
    ADD_PUBLIC_FUNC(HeavyRider_HandleTurn_ScreenEdges);
    ADD_PUBLIC_FUNC(HeavyRider_HandleTurn_ArenaEdges);
    ADD_PUBLIC_FUNC(HeavyRider_DecideNextAttack);
    ADD_PUBLIC_FUNC(HeavyRider_State_SetupArena);
    ADD_PUBLIC_FUNC(HeavyRider_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(HeavyRider_State_SetupRider);
    ADD_PUBLIC_FUNC(HeavyRider_State_Moving);
    ADD_PUBLIC_FUNC(HeavyRider_State_RampJump);
    ADD_PUBLIC_FUNC(HeavyRider_State_Charging);
    ADD_PUBLIC_FUNC(HeavyRider_State_ChargeDash);
    ADD_PUBLIC_FUNC(HeavyRider_State_Turning);
    ADD_PUBLIC_FUNC(HeavyRider_State_Destroyed);
    ADD_PUBLIC_FUNC(HeavyRider_State_Finish);
    ADD_PUBLIC_FUNC(HeavyRider_Draw_Boss);
    ADD_PUBLIC_FUNC(HeavyRider_State_PlaneSwitch);
    ADD_PUBLIC_FUNC(HeavyRider_Draw_PlaneSwitch);
    ADD_PUBLIC_FUNC(HeavyRider_State_ChargeTrigger);
    ADD_PUBLIC_FUNC(HeavyRider_Draw_Simple);
    ADD_PUBLIC_FUNC(HeavyRider_State_Fireball);
    ADD_PUBLIC_FUNC(HeavyRider_State_Puff);
    ADD_PUBLIC_FUNC(HeavyRider_StateJimmy_Idle);
    ADD_PUBLIC_FUNC(HeavyRider_StateJimmy_RevRelease);
    ADD_PUBLIC_FUNC(HeavyRider_StateJimmy_RevUp);

    // LRZ/Iwamodoki
    ADD_PUBLIC_FUNC(Iwamodoki_DebugSpawn);
    ADD_PUBLIC_FUNC(Iwamodoki_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(Iwamodoki_CheckOffScreen);
    ADD_PUBLIC_FUNC(Iwamodoki_State_Init);
    ADD_PUBLIC_FUNC(Iwamodoki_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(Iwamodoki_State_Appear);
    ADD_PUBLIC_FUNC(Iwamodoki_State_Charging);
    ADD_PUBLIC_FUNC(Iwamodoki_State_Explode);
    ADD_PUBLIC_FUNC(Iwamodoki_State_Debris);

    // LRZ/KingAttack
    ADD_PUBLIC_FUNC(KingAttack_SfxCheck_ElecIdle);
    ADD_PUBLIC_FUNC(KingAttack_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(KingAttack_HandleLaserPositions);
    ADD_PUBLIC_FUNC(KingAttack_State_Laser);
    ADD_PUBLIC_FUNC(KingAttack_State_LaserBlast_Delay);
    ADD_PUBLIC_FUNC(KingAttack_State_LaserBlast_Erupt);
    ADD_PUBLIC_FUNC(KingAttack_State_OrbitAppear);
    ADD_PUBLIC_FUNC(KingAttack_State_Orbiting);
    ADD_PUBLIC_FUNC(KingAttack_State_OrbitLaunched);
    ADD_PUBLIC_FUNC(KingAttack_State_Trail);
    ADD_PUBLIC_FUNC(KingAttack_State_LargeBullet_Appear);
    ADD_PUBLIC_FUNC(KingAttack_State_EnergyLine);
    ADD_PUBLIC_FUNC(KingAttack_State_LargeBullet_TwinShot);
    ADD_PUBLIC_FUNC(KingAttack_State_SmallBullet);
    ADD_PUBLIC_FUNC(KingAttack_State_SmallBullet_Impact);

    // LRZ/KingClaw
    ADD_PUBLIC_FUNC(KingClaw_HandleJointPositions);
    ADD_PUBLIC_FUNC(KingClaw_State_EnterClaw);
    ADD_PUBLIC_FUNC(KingClaw_State_Grab);
    ADD_PUBLIC_FUNC(KingClaw_State_LiftMasterEmerald);
    ADD_PUBLIC_FUNC(KingClaw_State_Swinging);
    ADD_PUBLIC_FUNC(KingClaw_State_LowerClaw);
    ADD_PUBLIC_FUNC(KingClaw_State_RaiseClaw);

    // LRZ/LavaFall
    ADD_PUBLIC_FUNC(LavaFall_State_Idle);
    ADD_PUBLIC_FUNC(LavaFall_State_LavaFall);
    ADD_PUBLIC_FUNC(LavaFall_State_Lava);

    // LRZ/LavaGeyser
    ADD_PUBLIC_FUNC(LavaGeyser_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(LavaGeyser_HandleSetup);
    ADD_PUBLIC_FUNC(LavaGeyser_State_Intervals);
    ADD_PUBLIC_FUNC(LavaGeyser_State_Init);
    ADD_PUBLIC_FUNC(LavaGeyser_State_ShowPlume);
    ADD_PUBLIC_FUNC(LavaGeyser_State_Erupting);
    ADD_PUBLIC_FUNC(LavaGeyser_State_Erupted);
    ADD_PUBLIC_FUNC(LavaGeyser_State_Recede);
    ADD_PUBLIC_FUNC(LavaGeyser_State_HandleFinish);

    // LRZ/LRZ1Intro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(LRZ1Intro_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(LRZ1Intro_HandlePlayerMovement);
    ADD_PUBLIC_FUNC(LRZ1Intro_HandleExplosions);
    ADD_PUBLIC_FUNC(LRZ1Intro_HandleExplosions);
    ADD_PUBLIC_FUNC(LRZ1Intro_State_SetupActors);
    ADD_PUBLIC_FUNC(LRZ1Intro_State_IntroDelay);
    ADD_PUBLIC_FUNC(LRZ1Intro_State_EnterSub);
    ADD_PUBLIC_FUNC(LRZ1Intro_State_RidingSub);
    ADD_PUBLIC_FUNC(LRZ1Intro_State_CrashedSub);
    ADD_PUBLIC_FUNC(LRZ1Intro_State_SubSinking);
#endif

    // LRZ/LRZ1Outro
    ADD_PUBLIC_FUNC(LRZ1Outro_StartCutscene);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct1_SetupActors);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct2_SetupActors);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct1_SetupDashLift);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct1_GoToDashLift);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct1_UsingDashLift);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct2_UsingDashLift);
    ADD_PUBLIC_FUNC(LRZ1Outro_CutsceneAct2_ExitDashLift);

    // LRZ/LRZ1Setup
    ADD_PUBLIC_FUNC(LRZ1Setup_StageFinish_EndAct1);
    ADD_PUBLIC_FUNC(LRZ1Setup_DrawHook_ApplyBGSmoothing);

    // LRZ/LRZ2Setup
    ADD_PUBLIC_FUNC(LRZ2Setup_HandleStageReload);
    ADD_PUBLIC_FUNC(LRZ2Setup_Trigger_StartOutro);
    ADD_PUBLIC_FUNC(LRZ2Setup_SetupTagLink);
    ADD_PUBLIC_FUNC(LRZ2Setup_GetTileInfo);

    // LRZ/LRZ3Cutscene
    ADD_PUBLIC_FUNC(LRZ3Cutscene_Cutscene_FadeIn);
    ADD_PUBLIC_FUNC(LRZ3Cutscene_Cutscene_RunRight);

    // LRZ/LRZ3Outro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(LRZ3Outro_HandleExplosions);
    ADD_PUBLIC_FUNC(LRZ3Outro_State_BlastOff);
    ADD_PUBLIC_FUNC(LRZ3Outro_State_RocketLaunch);
    ADD_PUBLIC_FUNC(LRZ3Outro_State_EnterLittlePlanet);
    ADD_PUBLIC_FUNC(LRZ3Outro_StageFinish_EndAct2ST);
    ADD_PUBLIC_FUNC(LRZ3Outro_Cutscene_StopPlayers);
    ADD_PUBLIC_FUNC(LRZ3Outro_Cutscene_LightUpLittlePlanet);
#endif

    // LRZ/LRZ3OutroK
    ADD_PUBLIC_FUNC(LRZ3OutroK_StartCutscene);
    ADD_PUBLIC_FUNC(LRZ3OutroK_Cutscene_RunToTeleporter);
    ADD_PUBLIC_FUNC(LRZ3OutroK_Cutscene_LandOnTeleporter);
    ADD_PUBLIC_FUNC(LRZ3OutroK_Cutscene_UseTeleporter);
    ADD_PUBLIC_FUNC(LRZ3OutroK_Cutscene_TeleporterActivated);

    // LRZ/LRZ3Setup
    ADD_PUBLIC_FUNC(LRZ3Setup_StageFinish_EndAct2K);

    // LRZ/LRZConvControl
    ADD_PUBLIC_FUNC(LRZConvControl_HandlePlayerTrigger);
    ADD_PUBLIC_FUNC(LRZConvControl_HandleButtonTrigger);

    // LRZ/LRZConvDropper
    ADD_PUBLIC_FUNC(LRZConvDropper_SetupDropperChildren);
    ADD_PUBLIC_FUNC(LRZConvDropper_HandleButtonDrop);

    // LRZ/LRZConveyor
    ADD_PUBLIC_FUNC(LRZConveyor_HandleBehaviour);
    ADD_PUBLIC_FUNC(LRZConveyor_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(LRZConveyor_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(LRZConveyor_DrawWheels);
    ADD_PUBLIC_FUNC(LRZConveyor_DrawDeformedLine);
    ADD_PUBLIC_FUNC(LRZConveyor_DrawConveyorLines);

    // LRZ/LRZConvItem
    ADD_PUBLIC_FUNC(LRZConvItem_SetupHitboxes);
    ADD_PUBLIC_FUNC(LRZConvItem_HandleLRZConvPhys);
    ADD_PUBLIC_FUNC(LRZConvItem_State_Rock);
    ADD_PUBLIC_FUNC(LRZConvItem_State_SpikeBall);

    // LRZ/LRZConvSwitch
    ADD_PUBLIC_FUNC(LRZConvSwitch_Calibrate);

    // LRZ/LRZFireball
    ADD_PUBLIC_FUNC(LRZFireball_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(LRZFireball_CheckOffScreen);
    ADD_PUBLIC_FUNC(LRZFireball_CheckTileCollisions);
    ADD_PUBLIC_FUNC(LRZFireball_State_Spawner);
    ADD_PUBLIC_FUNC(LRZFireball_State_LauncherStatic);
    ADD_PUBLIC_FUNC(LRZFireball_State_LauncherGravity);
    ADD_PUBLIC_FUNC(LRZFireball_StateFireball_Spawner);
    ADD_PUBLIC_FUNC(LRZFireball_StateFireball_LauncherStatic);
    ADD_PUBLIC_FUNC(LRZFireball_StateFireball_LauncherGravity);
    ADD_PUBLIC_FUNC(LRZFireball_Draw_Simple);

    // LRZ/LRZRockPile
    ADD_PUBLIC_FUNC(LRZRockPile_SetupDebris);
    ADD_PUBLIC_FUNC(LRZRockPile_SpawnRockDebris);
    ADD_PUBLIC_FUNC(LRZRockPile_State_Wall);
    ADD_PUBLIC_FUNC(LRZRockPile_State_Floor_TopSolid);
    ADD_PUBLIC_FUNC(LRZRockPile_State_Floor_AllSolid);
    ADD_PUBLIC_FUNC(LRZRockPile_State_Broken);

    // LRZ/LRZSpikeBall
    ADD_PUBLIC_FUNC(LRZSpikeBall_GetBaseFrameID);
    ADD_PUBLIC_FUNC(LRZSpikeBall_CheckPlayerBaseCollisions);
    ADD_PUBLIC_FUNC(LRZSpikeBall_CheckPlayerBallCollisions);
    ADD_PUBLIC_FUNC(LRZSpikeBall_State_Init);
    ADD_PUBLIC_FUNC(LRZSpikeBall_State_AwaitInterval);
    ADD_PUBLIC_FUNC(LRZSpikeBall_State_ChargeUp);
    ADD_PUBLIC_FUNC(LRZSpikeBall_State_LaunchedBall);

    // LRZ/LRZSpiral
    ADD_PUBLIC_FUNC(LRZSpiral_HandlePlayerExit);
    ADD_PUBLIC_FUNC(LRZSpiral_State_Cylinder);
    ADD_PUBLIC_FUNC(LRZSpiral_State_J_Curve);
    ADD_PUBLIC_FUNC(LRZSpiral_State_C_Curve);

    // LRZ/Rexon
    ADD_PUBLIC_FUNC(Rexon_DebugSpawn);
    ADD_PUBLIC_FUNC(Rexon_CheckOffScreen);
    ADD_PUBLIC_FUNC(Rexon_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Rexon_Destroy);
    ADD_PUBLIC_FUNC(Rexon_State_Init);
    ADD_PUBLIC_FUNC(Rexon_State_Hidden);
    ADD_PUBLIC_FUNC(Rexon_State_Rising);
    ADD_PUBLIC_FUNC(Rexon_State_Shooting);
    ADD_PUBLIC_FUNC(Rexon_State_Destroyed);
    ADD_PUBLIC_FUNC(Rexon_State_Explode);
    ADD_PUBLIC_FUNC(Rexon_State_Debris);
    ADD_PUBLIC_FUNC(Rexon_State_Projectile);

    // LRZ/RisingLava
    ADD_PUBLIC_FUNC(RisingLava_State_CheckRiseStart);
    ADD_PUBLIC_FUNC(RisingLava_State_RiseShake);
    ADD_PUBLIC_FUNC(RisingLava_State_StoppedRising);

    // LRZ/RockDrill
    ADD_PUBLIC_FUNC(RockDrill_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(RockDrill_Hit);
    ADD_PUBLIC_FUNC(RockDrill_Explode);
    ADD_PUBLIC_FUNC(RockDrill_SpawnDebris);
    ADD_PUBLIC_FUNC(RockDrill_State_Init);
    ADD_PUBLIC_FUNC(RockDrill_State_Drilling);
    ADD_PUBLIC_FUNC(RockDrill_State_Falling);
    ADD_PUBLIC_FUNC(RockDrill_State_Explode);

    // LRZ/SkyTeleporter
    ADD_PUBLIC_FUNC(SkyTeleporter_DrawTeleportBeam);

    // LRZ/SpikeCrusher
    ADD_PUBLIC_FUNC(SpikeCrusher_CheckOffScreen);
    ADD_PUBLIC_FUNC(SpikeCrusher_State_Init);
    ADD_PUBLIC_FUNC(SpikeCrusher_State_MovingDown);
    ADD_PUBLIC_FUNC(SpikeCrusher_State_Crushing);
    ADD_PUBLIC_FUNC(SpikeCrusher_State_CrushBounce);
    ADD_PUBLIC_FUNC(SpikeCrusher_State_ActivateFlames);
    ADD_PUBLIC_FUNC(SpikeCrusher_State_MovingUp);

    // LRZ/Stalactite
    ADD_PUBLIC_FUNC(Stalactite_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(Stalactite_State_Falling);
    ADD_PUBLIC_FUNC(Stalactite_State_Landed);
    ADD_PUBLIC_FUNC(Stalactite_State_Falling_Boss);
    ADD_PUBLIC_FUNC(Stalactite_State_Debris);

    // LRZ/ThoughtBubble
    ADD_PUBLIC_FUNC(ThoughtBubble_BubbleAppear);
    ADD_PUBLIC_FUNC(ThoughtBubble_HaveFirstThought);
    ADD_PUBLIC_FUNC(ThoughtBubble_HaveOtherThoughts);
    ADD_PUBLIC_FUNC(ThoughtBubble_BubbleDisappear);
    ADD_PUBLIC_FUNC(ThoughtBubble_DestroyBubble);

    // LRZ/Toxomister
    ADD_PUBLIC_FUNC(Toxomister_DebugSpawn);
    ADD_PUBLIC_FUNC(Toxomister_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Toxomister_CheckOffScreen);
    ADD_PUBLIC_FUNC(Toxomister_CheckCloudOnScreen);
    ADD_PUBLIC_FUNC(Toxomister_CheckPlayerCloudCollisions);
    ADD_PUBLIC_FUNC(Toxomister_State_Init);
    ADD_PUBLIC_FUNC(Toxomister_State_CreateClouds);
    ADD_PUBLIC_FUNC(Toxomister_StateCloud_FallDelay);
    ADD_PUBLIC_FUNC(Toxomister_StateCloud_FallToFloor);
    ADD_PUBLIC_FUNC(Toxomister_StateCloud_ReachedFloor);
    ADD_PUBLIC_FUNC(Toxomister_StateCloud_GrabbedPlayer);
    ADD_PUBLIC_FUNC(Toxomister_StateCloud_Dissipate);

    // LRZ/Turbine
    ADD_PUBLIC_FUNC(Turbine_State_Handles);
    ADD_PUBLIC_FUNC(Turbine_State_Spikes);

    // LRZ/TurretSwitch
    ADD_PUBLIC_FUNC(TurretSwitch_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TurretSwitch_Break);
    ADD_PUBLIC_FUNC(TurretSwitch_State_Init);
    ADD_PUBLIC_FUNC(TurretSwitch_State_Turret);
    ADD_PUBLIC_FUNC(TurretSwitch_State_Projectile);

    // LRZ/WalkerLegs
    ADD_PUBLIC_FUNC(WalkerLegs_DrawSprites);
    ADD_PUBLIC_FUNC(WalkerLegs_CheckOffScreen);
    ADD_PUBLIC_FUNC(WalkerLegs_HandlePlayerMovement);
    ADD_PUBLIC_FUNC(WalkerLegs_CheckObjectCrush);
    ADD_PUBLIC_FUNC(WalkerLegs_CheckStepTrigger);
    ADD_PUBLIC_FUNC(WalkerLegs_CheckTileCollisions);
    ADD_PUBLIC_FUNC(WalkerLegs_CheckStoodLava);
    ADD_PUBLIC_FUNC(WalkerLegs_CreateDebris);
    ADD_PUBLIC_FUNC(WalkerLegs_CreateSmoke);
    ADD_PUBLIC_FUNC(WalkerLegs_State_Init);
    ADD_PUBLIC_FUNC(WalkerLegs_State_Idle);
    ADD_PUBLIC_FUNC(WalkerLegs_State_Stepping);
    ADD_PUBLIC_FUNC(WalkerLegs_State_TryToReset);

    // Menu/CompetitionMenu
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(CompetitionMenu_Initialize);
    ADD_PUBLIC_FUNC(CompetitionMenu_SetupActions);
    ADD_PUBLIC_FUNC(CompetitionMenu_HandleMenuReturn);
    ADD_PUBLIC_FUNC(CompetitionMenu_HandleUnlocks);
    ADD_PUBLIC_FUNC(CompetitionMenu_HandleStartCompPrompt);
    ADD_PUBLIC_FUNC(CompetitionMenu_GetReadyPlayerCount);
    ADD_PUBLIC_FUNC(CompetitionMenu_GetTotalPlayerCount);
    ADD_PUBLIC_FUNC(CompetitionMenu_ResetControllerAssignments);
    ADD_PUBLIC_FUNC(CompetitionMenu_SetupSplitScreenChoices);
    ADD_PUBLIC_FUNC(CompetitionMenu_SetupSplitScreen);
    ADD_PUBLIC_FUNC(CompetitionMenu_SetupResultsUI);
    ADD_PUBLIC_FUNC(CompetitionMenu_GotoCompRules);
    ADD_PUBLIC_FUNC(CompetitionMenu_GotoCompZones);
    ADD_PUBLIC_FUNC(CompetitionMenu_VS_ProcessInputCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_VS_MenuSetupCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_Rules_MenuSetupCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_StartMatch);
    ADD_PUBLIC_FUNC(CompetitionMenu_ZoneButtonActionCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_RulesButton_ActionCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_GotoCompTotal);
    ADD_PUBLIC_FUNC(CompetitionMenu_Round_ProcessInputCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_Round_MenuSetupCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_GotoCompetition);
    ADD_PUBLIC_FUNC(CompetitionMenu_Results_ProcessInputCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_Results_MenuSetupCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_Results_MenuUpdateCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_ExitComp_TransitionCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_ExitComp_YesCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_CompRules_BackPressCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_CompZones_BackPressCB);
    ADD_PUBLIC_FUNC(CompetitionMenu_GotoPuyoVS);
#endif

    // Menu/DASetup
    ADD_PUBLIC_FUNC(DASetup_DisplayTrack);
    ADD_PUBLIC_FUNC(DASetup_HandleMedallionDebug);
    ADD_PUBLIC_FUNC(DASetup_SetupUI);
    ADD_PUBLIC_FUNC(DASetup_State_ManageControl);

    // Menu/DemoMenu
    ADD_PUBLIC_FUNC(DemoMenu_DrawStagePreview);
    ADD_PUBLIC_FUNC(DemoMenu_State_Appear);
    ADD_PUBLIC_FUNC(DemoMenu_State_Selection);
    ADD_PUBLIC_FUNC(DemoMenu_State_Load);
    ADD_PUBLIC_FUNC(DemoMenu_State_Disappear);

    // Menu/E3MenuSetup
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(E3MenuSetup_SetupUI);
    ADD_PUBLIC_FUNC(E3MenuSetup_SetupButtons);
    ADD_PUBLIC_FUNC(E3MenuSetup_Callback_LoadScene);
    ADD_PUBLIC_FUNC(E3MenuSetup_ActionCB_ZoneSelect);
    ADD_PUBLIC_FUNC(E3MenuSetup_ActionCB_Sonic);
    ADD_PUBLIC_FUNC(E3MenuSetup_ActionCB_Tails);
    ADD_PUBLIC_FUNC(E3MenuSetup_ActionCB_Knux);
    ADD_PUBLIC_FUNC(E3MenuSetup_State_FadeOut);
#endif

    // Menu/ExtrasMenu
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ExtrasMenu_Initialize);
    ADD_PUBLIC_FUNC(ExtrasMenu_HandleUnlocks);
    ADD_PUBLIC_FUNC(ExtrasMenu_SetupActions);
    ADD_PUBLIC_FUNC(ExtrasMenu_ProcessMedallionCheat);
    ADD_PUBLIC_FUNC(ExtrasMenu_CheckMedallionCheat);
    ADD_PUBLIC_FUNC(ExtrasMenu_ProcessInputs);
    ADD_PUBLIC_FUNC(ExtrasMenu_Start_Puyo_vsAI);
    ADD_PUBLIC_FUNC(ExtrasMenu_Puyo_vsAI_ActionCB);
    ADD_PUBLIC_FUNC(ExtrasMenu_Start_Puyo_vs2P);
    ADD_PUBLIC_FUNC(ExtrasMenu_Puyo_vs2P_ActionCB);
    ADD_PUBLIC_FUNC(ExtrasMenu_Start_Credits);
    ADD_PUBLIC_FUNC(ExtrasMenu_CreditsButton_ActionCB);
    ADD_PUBLIC_FUNC(ExtrasMenu_Start_DAGarden);
    ADD_PUBLIC_FUNC(ExtrasMenu_DAGarden_ActionCB);
    ADD_PUBLIC_FUNC(ExtrasMenu_Start_BSS_3K);
    ADD_PUBLIC_FUNC(ExtrasMenu_BSS_S3_ActionCB);
    ADD_PUBLIC_FUNC(ExtrasMenu_Start_BSS_Mania);
    ADD_PUBLIC_FUNC(ExtrasMenu_BSS_Mania_ActionCB);
#endif

#if MANIA_USE_PLUS
    // Menu/LevelSelect
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_AllEmeralds);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_ToggleSuperMusic);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_MaxContinues);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_MaxControl);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_RickyMode);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_SuperDash);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_SwapGameMode);
    ADD_PUBLIC_FUNC(LevelSelect_Cheat_UnlockAllMedals);
    ADD_PUBLIC_FUNC(LevelSelect_Draw_Fade);
#endif
    ADD_PUBLIC_FUNC(LevelSelect_State_Init);
    ADD_PUBLIC_FUNC(LevelSelect_State_FadeIn);
    ADD_PUBLIC_FUNC(LevelSelect_State_Navigate);
    ADD_PUBLIC_FUNC(LevelSelect_State_FadeOut);
    ADD_PUBLIC_FUNC(LevelSelect_ManagePlayerIcon);
    ADD_PUBLIC_FUNC(LevelSelect_SetLabelHighlighted);
    ADD_PUBLIC_FUNC(LevelSelect_HandleColumnChange);
    ADD_PUBLIC_FUNC(LevelSelect_HandleNewStagePos);

    // Menu/LogoSetup
    ADD_PUBLIC_FUNC(LogoSetup_ImageCallback);
    ADD_PUBLIC_FUNC(LogoSetup_State_CESAScreen);
    ADD_PUBLIC_FUNC(LogoSetup_State_ShowLogos);
    ADD_PUBLIC_FUNC(LogoSetup_State_FadeToNextLogos);
    ADD_PUBLIC_FUNC(LogoSetup_State_NextLogos);
    ADD_PUBLIC_FUNC(LogoSetup_Draw_Fade);

    // Menu/MainMenu
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MainMenu_Initialize);
    ADD_PUBLIC_FUNC(MainMenu_BackPressCB_ReturnToTitle);
    ADD_PUBLIC_FUNC(MainMenu_ExitGame);
    ADD_PUBLIC_FUNC(MainMenu_ExitButton_ActionCB);
    ADD_PUBLIC_FUNC(MainMenu_StartExitGame);
    ADD_PUBLIC_FUNC(MainMenu_MenuButton_ActionCB);
#if MANIA_USE_EGS
    ADD_PUBLIC_FUNC(MainMenu_BuyPlusDialogCB);
    ADD_PUBLIC_FUNC(MainMenu_HandleUnlocks);
#endif
    ADD_PUBLIC_FUNC(MainMenu_SetupActions);
    ADD_PUBLIC_FUNC(MainMenu_MenuSetupCB);
#endif

    // Menu/ManiaModeMenu
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(ManiaModeMenu_Initialize);
    ADD_PUBLIC_FUNC(ManiaModeMenu_InitAPI);
    ADD_PUBLIC_FUNC(ManiaModeMenu_InitLocalization);
    ADD_PUBLIC_FUNC(ManiaModeMenu_GetActiveMenu);
    ADD_PUBLIC_FUNC(ManiaModeMenu_ChangeMenuTrack);
    ADD_PUBLIC_FUNC(ManiaModeMenu_StartReturnToTitle);
    ADD_PUBLIC_FUNC(ManiaModeMenu_SetBGColors);
    ADD_PUBLIC_FUNC(ManiaModeMenu_ReturnToTitle);
    ADD_PUBLIC_FUNC(ManiaModeMenu_State_HandleTransition);
    ADD_PUBLIC_FUNC(ManiaModeMenu_HandleUnlocks);
    ADD_PUBLIC_FUNC(ManiaModeMenu_SetupActions);
    ADD_PUBLIC_FUNC(ManiaModeMenu_HandleMenuReturn);
#endif

    // Menu/MenuParam
    ADD_PUBLIC_FUNC(MenuParam_GetParam);

    // Menu/MenuSetup
    ADD_PUBLIC_FUNC(MenuSetup_StartTransition);
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MenuSetup_Initialize);
    ADD_PUBLIC_FUNC(MenuSetup_InitAPI);
    ADD_PUBLIC_FUNC(MenuSetup_SetupActions);
    ADD_PUBLIC_FUNC(MenuSetup_HandleUnlocks);
    ADD_PUBLIC_FUNC(MenuSetup_HandleMenuReturn);
    ADD_PUBLIC_FUNC(MenuSetup_GetActiveMenu);
    ADD_PUBLIC_FUNC(MenuSetup_ChangeMenuTrack);
    ADD_PUBLIC_FUNC(MenuSetup_SetBGColors);
    ADD_PUBLIC_FUNC(MenuSetup_MenuButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_StartTransitionLB);
    ADD_PUBLIC_FUNC(MenuSetup_BackPressCB_ReturnToTitle);
    ADD_PUBLIC_FUNC(MenuSetup_StartReturnToTitle);
    ADD_PUBLIC_FUNC(MenuSetup_ReturnToTitle);
    ADD_PUBLIC_FUNC(MenuSetup_ExitGame);
    ADD_PUBLIC_FUNC(MenuSetup_ExitGame_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_ExitGame_CB);
    ADD_PUBLIC_FUNC(MenuSetup_State_HandleTransition);
    ADD_PUBLIC_FUNC(MenuSetup_GetMedalMods);
    ADD_PUBLIC_FUNC(MenuSetup_OpenSaveSelectMenu);
    ADD_PUBLIC_FUNC(MenuSetup_SaveFileCB);
    ADD_PUBLIC_FUNC(MenuSetup_SaveSlot_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_SaveSel_MenuUpdateCB);
    ADD_PUBLIC_FUNC(MenuSetup_OpenSecretsMenu);
    ADD_PUBLIC_FUNC(MenuSetup_SaveSel_YPressCB);
    ADD_PUBLIC_FUNC(MenuSetup_TA_OpenZoneList_Sonic);
    ADD_PUBLIC_FUNC(MenuSetup_TA_OpenZoneList_Tails);
    ADD_PUBLIC_FUNC(MenuSetup_TA_OpenZoneList_Knux);
    ADD_PUBLIC_FUNC(MenuSetup_TA_TAZoneModule_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_TA_StartAttempt);
    ADD_PUBLIC_FUNC(MenuSetup_TA_Leaderboards_TransitionCB);
    ADD_PUBLIC_FUNC(MenuSetup_TA_Leaderboards_BackPressCB);
    ADD_PUBLIC_FUNC(MenuSetup_TA_Leaderboards_YPressCB);
    ADD_PUBLIC_FUNC(MenuSetup_State_SetupLeaderboards);
    ADD_PUBLIC_FUNC(MenuSetup_VS_OpenCompRules);
    ADD_PUBLIC_FUNC(MenuSetup_VS_OpenCompZones);
    ADD_PUBLIC_FUNC(MenuSetup_VS_ProcessButtonCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_MenuSetupCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_StartMatch);
    ADD_PUBLIC_FUNC(MenuSetup_VS_StartMatch_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_RulesButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_OpenCompTotal);
    ADD_PUBLIC_FUNC(MenuSetup_VS_Round_ProcessButtonCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_Round_MenuSetupCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_GotoCompetition);
    ADD_PUBLIC_FUNC(MenuSetup_VS_Total_ProcessButtonCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_Total_MenuSetupCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_Total_MenuUpdateCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_ExitComp_TransitionCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_BackoutFromVsCharSelect_CB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_BackoutFromVsCharSelect);
    ADD_PUBLIC_FUNC(MenuSetup_VS_ExitComp_YesCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_CompZones_BackPressCB);
    ADD_PUBLIC_FUNC(MenuSetup_VS_StartPuyoMatch);
    ADD_PUBLIC_FUNC(MenuSetup_Options_VideoMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Options_SoundMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Options_LanguageMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Options_ControlsMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Options_SetDefaultMappings_P1);
    ADD_PUBLIC_FUNC(MenuSetup_Options_SetDefaultMappings_P2);
    ADD_PUBLIC_FUNC(MenuSetup_Options_SetupKBControlsMenu);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_Win_MenuUpdateCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_Win_InitVideoOptionsMenu);
    ADD_PUBLIC_FUNC(MenuSetup_Options_OpenKBControlsMenu);
    ADD_PUBLIC_FUNC(MenuSetup_Options_MenuSetupCB);
    ADD_PUBLIC_FUNC(MenuSetup_Options_SaveOptionsCB_Load);
    ADD_PUBLIC_FUNC(MenuSetup_Options_LaunchManual);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsLanguage_LanguageButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_ShaderButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_WindowScaleButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_BorderlessButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_FullscreenButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_VSyncButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_TripleBufferButton_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_OptionsVideo_UISlider_ChangedCB);
    ADD_PUBLIC_FUNC(MenuSetup_OpenExtrasMenu_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_ProcessButtonCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Start_Puyo_vsAI);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Puyo_vsAI_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Start_Puyo_vs2P);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Puyo_vs2P_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Start_Credits);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Credits_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_StartDAGarden);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_DAGarden_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Start_BSS_3K);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_BSS_3K_ActionCB);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_Start_BSS_Mania);
    ADD_PUBLIC_FUNC(MenuSetup_Extras_BSS_Mania_ActionCB);
#endif

    // Menu/OptionsMenu
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(OptionsMenu_Initialize);
    ADD_PUBLIC_FUNC(OptionsMenu_HandleUnlocks);
    ADD_PUBLIC_FUNC(OptionsMenu_SetupActions);
    ADD_PUBLIC_FUNC(OptionsMenu_HandleMenuReturn);
    ADD_PUBLIC_FUNC(OptionsMenu_InitVideoOptionsMenu);
    ADD_PUBLIC_FUNC(OptionsMenu_VideoControl_Win_MenuUpdateCB);
    ADD_PUBLIC_FUNC(OptionsMenu_VideoControl_Win_YPressCB);
    ADD_PUBLIC_FUNC(OptionsMenu_DlgRunnerCB_RevertVideoChanges);
    ADD_PUBLIC_FUNC(OptionsMenu_VideoControl_Win_BackPressCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ApplyChangesDlg_BackPress_NoCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ApplyChangesDlg_Win_NoCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ApplyChangesDlg_NoCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ApplyChangesDlg_Win_YesCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ApplyChangesDlg_YesCB);
    ADD_PUBLIC_FUNC(OptionsMenu_VideoMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_SoundMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_LanguageMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_DataOptionsMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ControlsMenuButton_DefaultKB_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ControlsMenuButton_Default_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ControlsMenuButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_SetDefaultMappings);
    ADD_PUBLIC_FUNC(OptionsMenu_SetupKBControlsMenu);
    ADD_PUBLIC_FUNC(OptionsMenu_KeyboardIDButton_Win_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_MenuSetupCB);
    ADD_PUBLIC_FUNC(OptionsMenu_SaveOptionsCB_Load);
    ADD_PUBLIC_FUNC(OptionsMenu_TransitionCB_ReloadScene);
    ADD_PUBLIC_FUNC(OptionsMenu_SaveOptionsCB_Action);
    ADD_PUBLIC_FUNC(OptionsMenu_LanguageButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ShaderButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_WindowScaleButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_BorderlessButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_FullScreenButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_VSyncButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_TripleBufferButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_UISlider_ChangedCB);
    ADD_PUBLIC_FUNC(OptionsMenu_ShowManual);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseSaveDataCB);
    ADD_PUBLIC_FUNC(OptionsMenu_AreYouSureDlg_YesCB_EraseSaveGame);
    ADD_PUBLIC_FUNC(OptionsMenu_AreYouSureDlg_YesCB_EraseAllData);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseSaveGameButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_AreYouSureDlg_YesCB_EraseMedallions);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseMedallionsButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_AreYouSureDlg_YesCB_EraseTimeAttack);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseTimeAttackButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_AreYouSureDlg_YesCB_EraseReplays);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseReplaysCB);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseReplaysButton_ActionCB);
    ADD_PUBLIC_FUNC(OptionsMenu_EraseAllButton_ActionCB);
#endif

    // Menu/ThanksSetup
    ADD_PUBLIC_FUNC(ThanksSetup_HandleIconsPos);
    ADD_PUBLIC_FUNC(ThanksSetup_State_ThanksForPlaying);
    ADD_PUBLIC_FUNC(ThanksSetup_State_FlipOverIcon);
    ADD_PUBLIC_FUNC(ThanksSetup_State_Mania2017);
    ADD_PUBLIC_FUNC(ThanksSetup_State_FadeOut);
    ADD_PUBLIC_FUNC(ThanksSetup_Draw_Fade);

    // Menu/TimeAttackMenu
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TimeAttackMenu_Initialize);
    ADD_PUBLIC_FUNC(TimeAttackMenu_HandleUnlocks);
    ADD_PUBLIC_FUNC(TimeAttackMenu_SetupActions);
    ADD_PUBLIC_FUNC(TimeAttackMenu_HandleMenuReturn);
    ADD_PUBLIC_FUNC(TimeAttackMenu_SetEncoreLayouts);
    ADD_PUBLIC_FUNC(TimeAttackMenu_DeleteReplayActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ConfirmDeleteReplay_Yes_CB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_DeleteReplayCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_MenuUpdateCB_LB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_SetupLeaderboards);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ReplayCarousel_ActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_WatchReplay);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ReplayLoad_CB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_WatchReplayActionCB_ReplaysMenu);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ChallengeReplayActionCB_ReplaysMenu);
    ADD_PUBLIC_FUNC(TimeAttackMenu_WatchReplayCB_RanksMenu);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ChallengeReplayCB_RanksMenu);
    ADD_PUBLIC_FUNC(TimeAttackMenu_LoadScene_Fadeout);
    ADD_PUBLIC_FUNC(TimeAttackMenu_MenuSetupCB_Replay);
    ADD_PUBLIC_FUNC(TimeAttackMenu_MenuUpdateCB_Replay);
    ADD_PUBLIC_FUNC(TimeAttackMenu_MenuUpdateCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ReplayButton_ActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_YPressCB_Replay);
    ADD_PUBLIC_FUNC(TimeAttackMenu_SortReplayChoiceCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_SetupDetailsView);
    ADD_PUBLIC_FUNC(TimeAttackMenu_TAModule_ActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_StartTAAttempt);
    ADD_PUBLIC_FUNC(TimeAttackMenu_LoadScene);
    ADD_PUBLIC_FUNC(TimeAttackMenu_YPressCB_ZoneSel);
    ADD_PUBLIC_FUNC(TimeAttackMenu_BackPressCB_ZoneSel);
    ADD_PUBLIC_FUNC(TimeAttackMenu_YPressCB_Details);
    ADD_PUBLIC_FUNC(TimeAttackMenu_ResetTimes_YesCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_XPressCB_Details);
    ADD_PUBLIC_FUNC(TimeAttackMenu_TAZoneModule_ActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_RankButton_ActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_MenuSetupCB_Details);
    ADD_PUBLIC_FUNC(TimeAttackMenu_TAZoneModule_ChoiceChangeCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_CharButton_ActionCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_TransitionToDetailsCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_LeaderboardsBackPressCB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_YPressCB_LB);
    ADD_PUBLIC_FUNC(TimeAttackMenu_State_SetupLeaderboards);
    ADD_PUBLIC_FUNC(TimeAttackMenu_SetupLeaderboardsCarousel);
#endif

    // Menu/UIBackground
    ADD_PUBLIC_FUNC(UIBackground_DrawNormal);

    // Menu/UIButton
    ADD_PUBLIC_FUNC(UIButton_ManageChoices);
    ADD_PUBLIC_FUNC(UIButton_GetChoicePtr);
    ADD_PUBLIC_FUNC(UIButton_SetChoiceSelectionWithCB);
    ADD_PUBLIC_FUNC(UIButton_SetChoiceSelection);
    ADD_PUBLIC_FUNC(UIButton_GetActionCB);
    ADD_PUBLIC_FUNC(UIButton_FailCB);
    ADD_PUBLIC_FUNC(UIButton_ProcessButtonCB_Scroll);
    ADD_PUBLIC_FUNC(UIButton_ProcessTouchCB_Multi);
    ADD_PUBLIC_FUNC(UIButton_ProcessTouchCB_Single);
    ADD_PUBLIC_FUNC(UIButton_ProcessButtonCB);
    ADD_PUBLIC_FUNC(UIButton_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UIButton_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UIButton_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UIButton_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UIButton_SelectedCB);
    ADD_PUBLIC_FUNC(UIButton_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIButton_State_HandleButtonEnter);
    ADD_PUBLIC_FUNC(UIButton_State_Selected);

    // Menu/UIButtonPrompt
    ADD_PUBLIC_FUNC(UIButtonPrompt_GetButtonMappings);
    ADD_PUBLIC_FUNC(UIButtonPrompt_GetGamepadType);
    ADD_PUBLIC_FUNC(UIButtonPrompt_MappingsToFrame);
    ADD_PUBLIC_FUNC(UIButtonPrompt_CheckTouch);
    ADD_PUBLIC_FUNC(UIButtonPrompt_SetButtonSprites);
    ADD_PUBLIC_FUNC(UIButtonPrompt_State_CheckIfSelected);
    ADD_PUBLIC_FUNC(UIButtonPrompt_State_Selected);

    // Menu/UICarousel
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UICarousel_HandleScrolling);
    ADD_PUBLIC_FUNC(UICarousel_HandleButtonPositions);
#endif

    // Menu/UICharButton
    ADD_PUBLIC_FUNC(UICharButton_DrawOutlines);
    ADD_PUBLIC_FUNC(UICharButton_DrawBG);
    ADD_PUBLIC_FUNC(UICharButton_DrawPlayers);
    ADD_PUBLIC_FUNC(UICharButton_SelectedCB);
    ADD_PUBLIC_FUNC(UICharButton_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UICharButton_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UICharButton_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UICharButton_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UICharButton_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UICharButton_State_HandleButtonEnter);
    ADD_PUBLIC_FUNC(UICharButton_State_Selected);

    // Menu/UIChoice
    ADD_PUBLIC_FUNC(UIChoice_SetChoiceActive);
    ADD_PUBLIC_FUNC(UIChoice_SetChoiceInactive);
    ADD_PUBLIC_FUNC(UIChoice_TouchedCB_Left);
    ADD_PUBLIC_FUNC(UIChoice_TouchedCB_Right);
    ADD_PUBLIC_FUNC(UIChoice_CheckTouch);
    ADD_PUBLIC_FUNC(UIChoice_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIChoice_State_HandleButtonEnter);

    // Menu/UIControl
    ADD_PUBLIC_FUNC(UIControl_GetButtonID);
    ADD_PUBLIC_FUNC(UIControl_MenuChangeButtonInit);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIControl_SetActiveMenuButtonPrompts);
    ADD_PUBLIC_FUNC(UIControl_SetActiveMenu);
#endif
    ADD_PUBLIC_FUNC(UIControl_SetMenuLostFocus);
    ADD_PUBLIC_FUNC(UIControl_SetInactiveMenu);
    ADD_PUBLIC_FUNC(UIControl_SetupButtons);
    ADD_PUBLIC_FUNC(UIControl_GetUIControl);
    ADD_PUBLIC_FUNC(UIControl_isMoving);
    ADD_PUBLIC_FUNC(UIControl_MatchMenuTag);
    ADD_PUBLIC_FUNC(UIControl_HandleMenuChange);
    ADD_PUBLIC_FUNC(UIControl_HandleMenuLoseFocus);
    ADD_PUBLIC_FUNC(UIControl_ReturnToParentMenu);
    ADD_PUBLIC_FUNC(UIControl_ClearInputs);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIControl_SetTargetPos);
    ADD_PUBLIC_FUNC(UIControl_HandlePosition);
#endif
    ADD_PUBLIC_FUNC(UIControl_ProcessInputs);
    ADD_PUBLIC_FUNC(UIControl_ProcessButtonInput);

    // Menu/UICreditsText
    ADD_PUBLIC_FUNC(UICreditsText_SetText);
    ADD_PUBLIC_FUNC(UICreditsText_State_Init);
    ADD_PUBLIC_FUNC(UICreditsText_State_SetupCharPos);
    ADD_PUBLIC_FUNC(UICreditsText_State_MoveChars);
    ADD_PUBLIC_FUNC(UICreditsText_State_ScaleIn);
    ADD_PUBLIC_FUNC(UICreditsText_State_FadeIn);
    ADD_PUBLIC_FUNC(UICreditsText_SetupIdleDelay);
    ADD_PUBLIC_FUNC(UICreditsText_State_Idle);
    ADD_PUBLIC_FUNC(UICreditsText_State_ScaleOut);
    ADD_PUBLIC_FUNC(UICreditsText_State_FadeOut);

    // Menu/UIDialog
    ADD_PUBLIC_FUNC(UIDialog_CreateActiveDialog);
    ADD_PUBLIC_FUNC(UIDialog_SetupText);
    ADD_PUBLIC_FUNC(UIDialog_AddButton);
    ADD_PUBLIC_FUNC(UIDialog_Setup);
    ADD_PUBLIC_FUNC(UIDialog_CloseOnSel_HandleSelection);
    ADD_PUBLIC_FUNC(UIDialog_DrawBGShapes);
    ADD_PUBLIC_FUNC(UIDialog_HandleButtonPositions);
    ADD_PUBLIC_FUNC(UIDialog_Close);
    ADD_PUBLIC_FUNC(UIDialog_HandleAutoClose);
    ADD_PUBLIC_FUNC(UIDialog_ButtonActionCB);
    ADD_PUBLIC_FUNC(UIDialog_State_Appear);
    ADD_PUBLIC_FUNC(UIDialog_State_Idle);
    ADD_PUBLIC_FUNC(UIDialog_State_Close);
    ADD_PUBLIC_FUNC(UIDialog_CreateDialogOk);

    // Menu/UIDiorama
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIDiorama_ChangeDiorama);
    ADD_PUBLIC_FUNC(UIDiorama_SetText);
    ADD_PUBLIC_FUNC(UIDiorama_State_ManiaMode_Alt_Run);
    ADD_PUBLIC_FUNC(UIDiorama_State_ManiaMode_Alt_Jog);
    ADD_PUBLIC_FUNC(UIDiorama_State_ManiaMode_Alt_Loop);
    ADD_PUBLIC_FUNC(UIDiorama_State_PlusUpsell);
    ADD_PUBLIC_FUNC(UIDiorama_State_EncoreMode);
    ADD_PUBLIC_FUNC(UIDiorama_State_TimeAttack);
    ADD_PUBLIC_FUNC(UIDiorama_State_Competition);
    ADD_PUBLIC_FUNC(UIDiorama_State_Options);
    ADD_PUBLIC_FUNC(UIDiorama_State_Extras);
    ADD_PUBLIC_FUNC(UIDiorama_State_Exit);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_ManiaMode);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_PlusUpsell);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_EncoreMode);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_TimeAttack);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_Competition);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_Options);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_Extras);
    ADD_PUBLIC_FUNC(UIDiorama_Draw_Exit);
#endif

    // Menu/UIHeading
    ADD_PUBLIC_FUNC(UIHeading_LoadSprites);

    // Menu/UIInfoLabel
    ADD_PUBLIC_FUNC(UIInfoLabel_SetText);
    ADD_PUBLIC_FUNC(UIInfoLabel_SetString);
    ADD_PUBLIC_FUNC(UIInfoLabel_DrawSprites);

    // Menu/UIKeyBinder
    ADD_PUBLIC_FUNC(UIKeyBinder_GetButtonListID);
    ADD_PUBLIC_FUNC(UIKeyBinder_GetMappings);
    ADD_PUBLIC_FUNC(UIKeyBinder_SetMappings);
    ADD_PUBLIC_FUNC(UIKeyBinder_GetKeyNameFrameID);
    ADD_PUBLIC_FUNC(UIKeyBinder_DrawSprites);
    ADD_PUBLIC_FUNC(UIKeyBinder_ActionCB);
    ADD_PUBLIC_FUNC(UIKeyBinder_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UIKeyBinder_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UIKeyBinder_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UIKeyBinder_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UIKeyBinder_SelectedCB);
    ADD_PUBLIC_FUNC(UIKeyBinder_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIKeyBinder_State_HandleButtonEnter);
    ADD_PUBLIC_FUNC(UIKeyBinder_State_Selected);
#if GAME_VERSION != VER_100
    ADD_PUBLIC_FUNC(UIKeyBinder_MoveKeyToActionCB_No);
    ADD_PUBLIC_FUNC(UIKeyBinder_MoveKeyToActionCB_Yes);
#endif

    // Menu/UILeaderboard
    ADD_PUBLIC_FUNC(UILeaderboard_SetupEntrySprites);
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UILeaderboard_InitLeaderboard);
    ADD_PUBLIC_FUNC(UILeaderboard_SetupLeaderboard);
    ADD_PUBLIC_FUNC(UILeaderboard_LoadEntries);
#endif
    ADD_PUBLIC_FUNC(UILeaderboard_DrawPrimitives);
    ADD_PUBLIC_FUNC(UILeaderboard_DrawEntries);
    ADD_PUBLIC_FUNC(UILeaderboard_DrawZonePreview);
    ADD_PUBLIC_FUNC(UILeaderboard_DrawTime);
    ADD_PUBLIC_FUNC(UILeaderboard_DrawRank);
    ADD_PUBLIC_FUNC(UILeaderboard_State_Init);
    ADD_PUBLIC_FUNC(UILeaderboard_State_Unselected);
    ADD_PUBLIC_FUNC(UILeaderboard_State_Selected);
    ADD_PUBLIC_FUNC(UILeaderboard_ProcessButtonCB);

    // Menu/UIMedallionPanel
    ADD_PUBLIC_FUNC(UIMedallionPanel_DrawPanel);

    // Menu/UIModeButton
    ADD_PUBLIC_FUNC(UIModeButton_SetupSprites);
    ADD_PUBLIC_FUNC(UIModeButton_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UIModeButton_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UIModeButton_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UIModeButton_SelectedCB);
    ADD_PUBLIC_FUNC(UIModeButton_FailCB);
    ADD_PUBLIC_FUNC(UIModeButton_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UIModeButton_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIModeButton_State_HandleButtonEnter);
    ADD_PUBLIC_FUNC(UIModeButton_State_Selected);

    // Menu/UIOptionPanel
    ADD_PUBLIC_FUNC(UIOptionPanel_DrawBG);

    // Menu/UIPopover
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIPopover_CreatePopover);
    ADD_PUBLIC_FUNC(UIPopover_AddButton);
    ADD_PUBLIC_FUNC(UIPopover_Setup);
    ADD_PUBLIC_FUNC(UIPopover_DrawSprites);
    ADD_PUBLIC_FUNC(UIPopover_SetupButtonPositions);
    ADD_PUBLIC_FUNC(UIPopover_Close);
    ADD_PUBLIC_FUNC(UIPopover_BackPressCB);
    ADD_PUBLIC_FUNC(UIPopover_ButtonActionCB);
    ADD_PUBLIC_FUNC(UIPopover_State_Appear);
    ADD_PUBLIC_FUNC(UIPopover_State_Idle);
    ADD_PUBLIC_FUNC(UIPopover_State_Close);
#endif

    // Menu/UIRankButton
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIRankButton_SetRankText);
    ADD_PUBLIC_FUNC(UIRankButton_SetTimeAttackRank);
    ADD_PUBLIC_FUNC(UIRankButton_SetupLeaderboardRank);
    ADD_PUBLIC_FUNC(UIRankButton_DrawSprites);
    ADD_PUBLIC_FUNC(UIRankButton_DrawBackgroundShape);
    ADD_PUBLIC_FUNC(UIRankButton_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UIRankButton_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UIRankButton_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UIRankButton_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UIRankButton_FailCB);
    ADD_PUBLIC_FUNC(UIRankButton_SelectedCB);
    ADD_PUBLIC_FUNC(UIRankButton_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIRankButton_State_HandleButtonEnter);
    ADD_PUBLIC_FUNC(UIRankButton_State_Selected);
#endif

    // Menu/UIReplayCarousel
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIReplayCarousel_ProcessButtonCB);
    ADD_PUBLIC_FUNC(UIReplayCarousel_HandlePositions);
    ADD_PUBLIC_FUNC(UIReplayCarousel_SetupButtonCallbacks);
    ADD_PUBLIC_FUNC(UIReplayCarousel_HandleTouchPositions);
    ADD_PUBLIC_FUNC(UIReplayCarousel_TouchedCB);
    ADD_PUBLIC_FUNC(UIReplayCarousel_SetupVisibleReplayButtons);
    ADD_PUBLIC_FUNC(UIReplayCarousel_DrawBGShapes);
    ADD_PUBLIC_FUNC(UIReplayCarousel_DrawStageInfo);
    ADD_PUBLIC_FUNC(UIReplayCarousel_DrawZoneIcon);
    ADD_PUBLIC_FUNC(UIReplayCarousel_DrawReplayInfo);
    ADD_PUBLIC_FUNC(UIReplayCarousel_StartAction);
    ADD_PUBLIC_FUNC(UIReplayCarousel_SetUnselected);
    ADD_PUBLIC_FUNC(UIReplayCarousel_Draw_Loading);
    ADD_PUBLIC_FUNC(UIReplayCarousel_Draw_NoReplays);
    ADD_PUBLIC_FUNC(UIReplayCarousel_Draw_Carousel);
    ADD_PUBLIC_FUNC(UIReplayCarousel_State_Init);
    ADD_PUBLIC_FUNC(UIReplayCarousel_State_Unselected);
    ADD_PUBLIC_FUNC(UIReplayCarousel_State_Selected);
    ADD_PUBLIC_FUNC(UIReplayCarousel_State_StartAction);
#endif

#if GAME_VERSION != VER_100
    // Menu/UIResPicker
    ADD_PUBLIC_FUNC(UIResPicker_GetDisplayInfo);
    ADD_PUBLIC_FUNC(UIResPicker_ApplySettings);
    ADD_PUBLIC_FUNC(UIResPicker_ProcessButtonCB);
    ADD_PUBLIC_FUNC(UIResPicker_ProcessTouchCB);
    ADD_PUBLIC_FUNC(UIResPicker_TouchedCB_Left);
    ADD_PUBLIC_FUNC(UIResPicker_TouchedCB_Right);
    ADD_PUBLIC_FUNC(UIResPicker_SetChoiceActive);
    ADD_PUBLIC_FUNC(UIResPicker_SetChoiceInactive);
    ADD_PUBLIC_FUNC(UIResPicker_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIResPicker_State_HandleButtonEnter);
#endif

#if MANIA_USE_PLUS
    // Menu/UISaveSlot
    ADD_PUBLIC_FUNC(UISaveSlot_GetPlayerIDFromID);
    ADD_PUBLIC_FUNC(UISaveSlot_GetIDFromPlayerID);
    ADD_PUBLIC_FUNC(UISaveSlot_DrawPlayerIcons);
#endif
    ADD_PUBLIC_FUNC(UISaveSlot_DrawPlayerInfo);
    ADD_PUBLIC_FUNC(UISaveSlot_SetupButtonElements);
    ADD_PUBLIC_FUNC(UISaveSlot_SetupAnimators);
    ADD_PUBLIC_FUNC(UISaveSlot_LoadSaveInfo);
    ADD_PUBLIC_FUNC(UISaveSlot_HandleSaveIcons);
    ADD_PUBLIC_FUNC(UISaveSlot_DeleteDLG_CB);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UISaveSlot_DeleteSaveCB);
#else
    ADD_PUBLIC_FUNC(UISaveSlot_DeleteSaveCB);
    ADD_PUBLIC_FUNC(UISaveSlot_ProcessButtonCB);
#endif
    ADD_PUBLIC_FUNC(UISaveSlot_SelectedCB);
    ADD_PUBLIC_FUNC(UISaveSlot_NextCharacter);
    ADD_PUBLIC_FUNC(UISaveSlot_PrevCharacter);
    ADD_PUBLIC_FUNC(UISaveSlot_NextZone);
    ADD_PUBLIC_FUNC(UISaveSlot_PrevZone);
    ADD_PUBLIC_FUNC(UISaveSlot_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UISaveSlot_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UISaveSlot_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UISaveSlot_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UISaveSlot_HandleSaveIconChange);
    ADD_PUBLIC_FUNC(UISaveSlot_State_NotSelected);
    ADD_PUBLIC_FUNC(UISaveSlot_State_OtherWasSelected);
    ADD_PUBLIC_FUNC(UISaveSlot_State_NewSave);
    ADD_PUBLIC_FUNC(UISaveSlot_State_ActiveSave);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UISaveSlot_StateInput_NewSave);
    ADD_PUBLIC_FUNC(UISaveSlot_State_CompletedSave);
#endif
    ADD_PUBLIC_FUNC(UISaveSlot_State_Selected);

    // Menu/UIShifter
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIShifter_HandleShift);
#endif

    // Menu/UISlider
    ADD_PUBLIC_FUNC(UISlider_DrawBGShapes);
    ADD_PUBLIC_FUNC(UISlider_DrawSlider);
    ADD_PUBLIC_FUNC(UISlider_ButtonPressCB);
    ADD_PUBLIC_FUNC(UISlider_TouchCB);
    ADD_PUBLIC_FUNC(UISlider_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UISlider_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UISlider_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UISlider_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UISlider_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UISlider_State_HandleButtonEnter);

#if MANIA_USE_PLUS
    // Menu/UISubHeading
    ADD_PUBLIC_FUNC(UISubHeading_Initialize);
    ADD_PUBLIC_FUNC(UISubHeading_HandleUnlocks);
    ADD_PUBLIC_FUNC(UISubHeading_SetupActions);
    ADD_PUBLIC_FUNC(UISubHeading_HandleMenuReturn);
    ADD_PUBLIC_FUNC(UISubHeading_GetMedalMods);
    ADD_PUBLIC_FUNC(UISubHeading_SaveFileCB);
    ADD_PUBLIC_FUNC(UISubHeading_SecretsTransitionCB);
    ADD_PUBLIC_FUNC(UISubHeading_LeaveSecretsMenu);
    ADD_PUBLIC_FUNC(UISubHeading_SaveSel_MenuUpdateCB);
    ADD_PUBLIC_FUNC(UISubHeading_SaveSel_YPressCB);
    ADD_PUBLIC_FUNC(UISubHeading_SaveButton_ActionCB);
#endif

    // Menu/UITABanner
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UITABanner_SetupDetails);
    ADD_PUBLIC_FUNC(UITABanner_DrawBGShapes);
    ADD_PUBLIC_FUNC(UITABanner_DrawStageInfo);
    ADD_PUBLIC_FUNC(UITABanner_DrawZoneIcon);
    ADD_PUBLIC_FUNC(UITABanner_DrawBannerInfo);
#endif

    // Menu/UITAZoneModule
    ADD_PUBLIC_FUNC(UITAZoneModule_Setup);
    ADD_PUBLIC_FUNC(UITAZoneModule_SetupText);
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawBGShapes);
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawFGShapes);
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawZonePreview);
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawModuleInfo);
    ADD_PUBLIC_FUNC(UITAZoneModule_FailCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_SelectedCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_TransitionCB);
    ADD_PUBLIC_FUNC(UITAZoneModule_ShowLeaderboards_CB);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_Init);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_NotSelected);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_Selected);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_HasBeenSelected);
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawTime);
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawActInfo_Expanded);
    ADD_PUBLIC_FUNC(UITAZoneModule_DrawExpandedView);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_Expanded);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_StartTimeAttackAttempt);
    ADD_PUBLIC_FUNC(UITAZoneModule_TouchCB_Left);
    ADD_PUBLIC_FUNC(UITAZoneModule_TouchCB_Right);
    ADD_PUBLIC_FUNC(UITAZoneModule_ProcessButtonCB_Expanded);
    ADD_PUBLIC_FUNC(UITAZoneModule_ShowLeaderboards);
    ADD_PUBLIC_FUNC(UITAZoneModule_SetStartupModule);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_ExpandModule);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_ContractModule);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_MoveOffScreen);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_Inactive);
    ADD_PUBLIC_FUNC(UITAZoneModule_State_ComeBackOnScreen);
#endif

    // Menu/UITransition
    ADD_PUBLIC_FUNC(UITransition_StartTransition);
    ADD_PUBLIC_FUNC(UITransition_MatchNewTag);
    ADD_PUBLIC_FUNC(UITransition_SetNewTag);
    ADD_PUBLIC_FUNC(UITransition_DrawShapes);
    ADD_PUBLIC_FUNC(UITransition_State_Init);
    ADD_PUBLIC_FUNC(UITransition_State_TransitionIn);
    ADD_PUBLIC_FUNC(UITransition_State_TransitionOut);

    // Menu/UIUsernamePopup
    ADD_PUBLIC_FUNC(UIUsernamePopup_ShowPopup);
    ADD_PUBLIC_FUNC(UIUsernamePopup_DrawSprites);
    ADD_PUBLIC_FUNC(UIUsernamePopup_State_Init);
    ADD_PUBLIC_FUNC(UIUsernamePopup_State_Appear);
    ADD_PUBLIC_FUNC(UIUsernamePopup_State_Shown);
    ADD_PUBLIC_FUNC(UIUsernamePopup_State_Disappear);

    // Menu/UIVideo
    ADD_PUBLIC_FUNC(UIVideo_SkipCB);
    ADD_PUBLIC_FUNC(UIVideo_State_PlayVideo1);
    ADD_PUBLIC_FUNC(UIVideo_State_PlayVideo2);
    ADD_PUBLIC_FUNC(UIVideo_State_FinishPlayback);

    // Menu/UIVsCharSelector
    ADD_PUBLIC_FUNC(UIVsCharSelector_SetupText);
    ADD_PUBLIC_FUNC(UIVsCharSelector_DrawOutline);
    ADD_PUBLIC_FUNC(UIVsCharSelector_DrawBG);
    ADD_PUBLIC_FUNC(UIVsCharSelector_DrawPlayer);
    ADD_PUBLIC_FUNC(UIVsCharSelector_ProcessButtonCB);
    ADD_PUBLIC_FUNC(UIVsCharSelector_ProcessButtonCB_CharSelected);
    ADD_PUBLIC_FUNC(UIVsCharSelector_State_ResetState);
    ADD_PUBLIC_FUNC(UIVsCharSelector_State_CharSelect);
    ADD_PUBLIC_FUNC(UIVsCharSelector_State_WaitingForPlayer);
    ADD_PUBLIC_FUNC(UIVsCharSelector_State_HandlePlayerJoin);
    ADD_PUBLIC_FUNC(UIVsCharSelector_State_Selected);

    // Menu/UIVsResults
    ADD_PUBLIC_FUNC(UIVsResults_SetupSprites);
    ADD_PUBLIC_FUNC(UIVsResults_DrawOutline);
    ADD_PUBLIC_FUNC(UIVsResults_DrawBG);
    ADD_PUBLIC_FUNC(UIVsResults_DrawRow);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIVsResults_DrawTrophies);
    ADD_PUBLIC_FUNC(UIVsResults_DrawResults);
#endif
    ADD_PUBLIC_FUNC(UIVsResults_State_Blank);

    // Menu/UIVsRoundPicker
    ADD_PUBLIC_FUNC(UIVsRoundPicker_DrawText);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_SetText);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_Apply);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_ProcessButtonCB);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_ProcessTouchCB);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_TouchedCB_Left);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_TouchedCB_Right);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_SetChoiceActive);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_SetChoiceInactive);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIVsRoundPicker_State_HandleButtonEnter);

    // Menu/UIVsScoreboard
    ADD_PUBLIC_FUNC(UIVsScoreboard_SetupSprites);
    ADD_PUBLIC_FUNC(UIVsScoreboard_SetScores);
    ADD_PUBLIC_FUNC(UIVsScoreboard_DrawSprites);

    // Menu/UIVsZoneButton
    ADD_PUBLIC_FUNC(UIVsZoneButton_SetupAnimators);
    ADD_PUBLIC_FUNC(UIVsZoneButton_SetNameText);
    ADD_PUBLIC_FUNC(UIVsZoneButton_DrawOutline);
    ADD_PUBLIC_FUNC(UIVsZoneButton_DrawBG);
    ADD_PUBLIC_FUNC(UIVsZoneButton_DrawZoneIcon);
    ADD_PUBLIC_FUNC(UIVsZoneButton_DrawButton);
    ADD_PUBLIC_FUNC(UIVsZoneButton_CheckButtonEnterCB);
    ADD_PUBLIC_FUNC(UIVsZoneButton_CheckSelectedCB);
    ADD_PUBLIC_FUNC(UIVsZoneButton_SelectedCB);
    ADD_PUBLIC_FUNC(UIVsZoneButton_FailCB);
    ADD_PUBLIC_FUNC(UIVsZoneButton_ButtonEnterCB);
    ADD_PUBLIC_FUNC(UIVsZoneButton_ButtonLeaveCB);
    ADD_PUBLIC_FUNC(UIVsZoneButton_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIVsZoneButton_State_HandleButtonEnter);
    ADD_PUBLIC_FUNC(UIVsZoneButton_State_Selected);

    // Menu/UIWaitSpinner
    ADD_PUBLIC_FUNC(UIWaitSpinner_StartWait);
    ADD_PUBLIC_FUNC(UIWaitSpinner_FinishWait);
    ADD_PUBLIC_FUNC(UIWaitSpinner_State_Show);
    ADD_PUBLIC_FUNC(UIWaitSpinner_State_Hide);

    // Menu/UIWidgets
    ADD_PUBLIC_FUNC(UIWidgets_ApplyLanguage);
    ADD_PUBLIC_FUNC(UIWidgets_DrawRectOutline_Black);
    ADD_PUBLIC_FUNC(UIWidgets_DrawRectOutline_Blended);
    ADD_PUBLIC_FUNC(UIWidgets_DrawRectOutline_Flash);
    ADD_PUBLIC_FUNC(UIWidgets_DrawRightTriangle);
    ADD_PUBLIC_FUNC(UIWidgets_DrawEquilateralTriangle);
    ADD_PUBLIC_FUNC(UIWidgets_DrawParallelogram);
    ADD_PUBLIC_FUNC(UIWidgets_DrawUpDownArrows);
    ADD_PUBLIC_FUNC(UIWidgets_DrawLeftRightArrows);
    ADD_PUBLIC_FUNC(UIWidgets_DrawTriJoinRect);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(UIWidgets_DrawTime);
#endif

#if GAME_VERSION != VER_100
    // Menu/UIWinSize
    ADD_PUBLIC_FUNC(UIWinSize_SetupText);
    ADD_PUBLIC_FUNC(UIWinSize_ApplySettings);
    ADD_PUBLIC_FUNC(UIWinSize_ProcessButtonCB);
    ADD_PUBLIC_FUNC(UIWinSize_ProcessTouchCB);
    ADD_PUBLIC_FUNC(UIWinSize_TouchedCB_Left);
    ADD_PUBLIC_FUNC(UIWinSize_TouchedCB_Right);
    ADD_PUBLIC_FUNC(UIWinSize_SetChoiceActive);
    ADD_PUBLIC_FUNC(UIWinSize_SetChoiceInactive);
    ADD_PUBLIC_FUNC(UIWinSize_State_HandleButtonLeave);
    ADD_PUBLIC_FUNC(UIWinSize_State_HandleButtonEnter);
#endif

    // MMZ/BladePole
    ADD_PUBLIC_FUNC(BladePole_DrawSprites);
    ADD_PUBLIC_FUNC(BladePole_SetAnimation);
    ADD_PUBLIC_FUNC(BladePole_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(BladePole_State_TopBladeActive);
    ADD_PUBLIC_FUNC(BladePole_State_BottomBladeActive);

    // MMZ/BuzzSaw
    ADD_PUBLIC_FUNC(BuzzSaw_SfxCheck_SawSus);
    ADD_PUBLIC_FUNC(BuzzSaw_SfxUpdate_SawSus);
    ADD_PUBLIC_FUNC(BuzzSaw_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(BuzzSaw_State_Attatched);
    ADD_PUBLIC_FUNC(BuzzSaw_State_Stray_Waiting);
    ADD_PUBLIC_FUNC(BuzzSaw_State_Stray_Released);
    ADD_PUBLIC_FUNC(BuzzSaw_State_FreeMove_Reset);

    // MMZ/EggPistonsMKII
    ADD_PUBLIC_FUNC(EggPistonsMKII_CheckPlayerCollisions_Piston);
    ADD_PUBLIC_FUNC(EggPistonsMKII_CheckPlayerCollisions_EggPiston);
    ADD_PUBLIC_FUNC(EggPistonsMKII_CheckPlayerCollisions_Ball);
    ADD_PUBLIC_FUNC(EggPistonsMKII_CheckPlayerCollisions_Solid);
    ADD_PUBLIC_FUNC(EggPistonsMKII_Hit);
    ADD_PUBLIC_FUNC(EggPistonsMKII_Explode);
    ADD_PUBLIC_FUNC(EggPistonsMKII_GetNextPiston);
    ADD_PUBLIC_FUNC(EggPistonsMKII_SpawnElecBall);
    ADD_PUBLIC_FUNC(EggPistonsMKII_CheckPlayerAttacking);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_SetupArena);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_EnterBoss);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_PistonReveal);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_ClassicMode);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_StartPinchMode);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_PinchMode);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_Destroyed);
    ADD_PUBLIC_FUNC(EggPistonsMKII_State_Finish);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StatePiston_Idle);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StatePiston_Shaking);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StatePiston_BeginCrushing);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StatePiston_CrushExtend);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StatePiston_Retract);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StatePiston_Explode);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateOrbGenerator_Idle);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateOrbGenerator_Warning);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateOrb_MoveToTargetPos);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateOrb_Charging);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateOrb_Attacking);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateAlarm_Active);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateAlarm_Destroyed);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateBarrier_Solid);
    ADD_PUBLIC_FUNC(EggPistonsMKII_StateBarrier_Explode);

    // MMZ/FarPlane
    ADD_PUBLIC_FUNC(FarPlane_SetupEntities);
    ADD_PUBLIC_FUNC(FarPlane_SetEntityActivities);
    ADD_PUBLIC_FUNC(FarPlane_DrawHook_ApplyFarPlane);
    ADD_PUBLIC_FUNC(FarPlane_DrawHook_RemoveFarPlane);
    ADD_PUBLIC_FUNC(FarPlane_Scanline_FarPlaneView);

    // MMZ/Gachapandora
    ADD_PUBLIC_FUNC(Gachapandora_CheckPlayerCollisions_Prize);
    ADD_PUBLIC_FUNC(Gachapandora_Explode);
    ADD_PUBLIC_FUNC(Gachapandora_Player_StateInput_P1Grabbed);
    ADD_PUBLIC_FUNC(Gachapandora_Player_StateInput_P2PlayerGrabbed);
    ADD_PUBLIC_FUNC(Gachapandora_Player_StateInput_P2AIGrabbed);
    ADD_PUBLIC_FUNC(Gachapandora_HandleSparksAndDebris);
    ADD_PUBLIC_FUNC(Gachapandora_HandleAnimations);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_SetupArena);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_AwaitPlayer);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_EnterEggman);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_FloatAround);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_HandleSpun);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_Explode);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_PanicFlee);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_EnsureAllCapsulesDestroyed);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_LastDitchAttack);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_Defeated);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_EggmanFallOut);
    ADD_PUBLIC_FUNC(Gachapandora_StateBoss_Finish);
    ADD_PUBLIC_FUNC(Gachapandora_Draw_Boss);
    ADD_PUBLIC_FUNC(Gachapandora_Draw_BossDestroyed);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_CapsuleFall);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_ExitCapsule);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_Destroyed);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_DrillerCapsuleExitBounce);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_DrillerMove);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_FireDropperMove);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_AmyIdle);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_AmyWalk);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_AmyJump);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_AmyRebound);
    ADD_PUBLIC_FUNC(Gachapandora_StatePrize_AmyGrabbed);
    ADD_PUBLIC_FUNC(Gachapandora_Draw_Prize);
    ADD_PUBLIC_FUNC(Gachapandora_StateFireball_Falling);
    ADD_PUBLIC_FUNC(Gachapandora_StateFireball_BurnGround);
    ADD_PUBLIC_FUNC(Gachapandora_Draw_Simple);
    ADD_PUBLIC_FUNC(Gachapandora_StateDebris_Delay);
    ADD_PUBLIC_FUNC(Gachapandora_StateDebris_Falling);
    ADD_PUBLIC_FUNC(Gachapandora_StateDebris_BossDebris);
    ADD_PUBLIC_FUNC(Gachapandora_Draw_BossDebris);
    ADD_PUBLIC_FUNC(Gachapandora_StateSpark_Attatched);
    ADD_PUBLIC_FUNC(Gachapandora_StateSpark_Detatched);
    ADD_PUBLIC_FUNC(Gachapandora_StateEggman_Falling);
    ADD_PUBLIC_FUNC(Gachapandora_StateEggman_Escape);
    ADD_PUBLIC_FUNC(Gachapandora_StateEggman_RunAway);

    // MMZ/MatryoshkaBom
    ADD_PUBLIC_FUNC(MatryoshkaBom_DebugSpawn);
    ADD_PUBLIC_FUNC(MatryoshkaBom_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MatryoshkaBom_CheckOffScreen);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_Init);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_Walk);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_Stopped);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_Turn);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_FuseLit);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_ReleaseSmallerBuddy);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_Hatched);
    ADD_PUBLIC_FUNC(MatryoshkaBom_State_Shrapnel);

    // MMZ/MechaBu
    ADD_PUBLIC_FUNC(MechaBu_DebugSpawn);
    ADD_PUBLIC_FUNC(MechaBu_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MechaBu_CheckOffScreen);
    ADD_PUBLIC_FUNC(MechaBu_GetSawOffset);
    ADD_PUBLIC_FUNC(MechaBu_State_Init);
    ADD_PUBLIC_FUNC(MechaBu_State_Moving);
    ADD_PUBLIC_FUNC(MechaBu_State_Stopped);
    ADD_PUBLIC_FUNC(MechaBu_State_Falling);

    // MMZ/MMZ2Outro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MMZ2Outro_StartCutscene);
    ADD_PUBLIC_FUNC(MMZ2Outro_Cutscene_PowerDown);
    ADD_PUBLIC_FUNC(MMZ2Outro_Cutscene_Rumble);
    ADD_PUBLIC_FUNC(MMZ2Outro_Cutscene_CameraMoveToWindow);
    ADD_PUBLIC_FUNC(MMZ2Outro_Cutscene_PlayerMoveToWindow);
    ADD_PUBLIC_FUNC(MMZ2Outro_Cutscene_EnterMonarchEyes);
    ADD_PUBLIC_FUNC(MMZ2Outro_Cutscene_ViewMonarch);
#endif

    // MMZ/MMZLightning
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MMZLightning_State_BeginFadeIn);
    ADD_PUBLIC_FUNC(MMZLightning_State_FadeIn);
    ADD_PUBLIC_FUNC(MMZLightning_State_SetupLightningBig);
    ADD_PUBLIC_FUNC(MMZLightning_State_ShowLightningBig);
    ADD_PUBLIC_FUNC(MMZLightning_State_LightningBigFadeOut);
    ADD_PUBLIC_FUNC(MMZLightning_State_SetupLightningSmall);
    ADD_PUBLIC_FUNC(MMZLightning_State_LightningSmallFadeOut);
#endif

    // MMZ/MMZSetup
    ADD_PUBLIC_FUNC(MMZSetup_StageFinish_EndAct1);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MMZSetup_StageFinish_EndAct2);
#endif

    // MMZ/Piston
    ADD_PUBLIC_FUNC(Piston_Collide_Solid);
    ADD_PUBLIC_FUNC(Piston_State_WaitForInterval);
    ADD_PUBLIC_FUNC(Piston_StateMove_Down);
    ADD_PUBLIC_FUNC(Piston_StateMove_Down_Reverse);
    ADD_PUBLIC_FUNC(Piston_StateMove_Vertical);
    ADD_PUBLIC_FUNC(Piston_StateMove_Vertical_Reverse);
    ADD_PUBLIC_FUNC(Piston_StateMove_Up);
    ADD_PUBLIC_FUNC(Piston_StateMove_Up_Reverse);
    ADD_PUBLIC_FUNC(Piston_StateMove_Right);
    ADD_PUBLIC_FUNC(Piston_StateMove_Left);
    ADD_PUBLIC_FUNC(Piston_StateMove_Horizontal);
    ADD_PUBLIC_FUNC(Piston_StateMove_Horizontal_Reverse);
    ADD_PUBLIC_FUNC(Piston_StateActive_WaitForStood);
    ADD_PUBLIC_FUNC(Piston_StateActive_PreparingLaunch);
    ADD_PUBLIC_FUNC(Piston_StateActive_LaunchPlayers);
    ADD_PUBLIC_FUNC(Piston_StateActive_ReturnToStartPos);

    // MMZ/PlaneSeeSaw
    ADD_PUBLIC_FUNC(PlaneSeeSaw_State_WaitForPlayer);
    ADD_PUBLIC_FUNC(PlaneSeeSaw_State_PlayerPushDown);
    ADD_PUBLIC_FUNC(PlaneSeeSaw_State_Launch);
    ADD_PUBLIC_FUNC(PlaneSeeSaw_PlayerState_ToBG);
    ADD_PUBLIC_FUNC(PlaneSeeSaw_PlayerState_ToFG);

    // MMZ/PohBee
    ADD_PUBLIC_FUNC(PohBee_DebugSpawn);
    ADD_PUBLIC_FUNC(PohBee_CheckOffScreen);
    ADD_PUBLIC_FUNC(PohBee_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PohBee_DrawSprites);
    ADD_PUBLIC_FUNC(PohBee_GetSpikePos);
    ADD_PUBLIC_FUNC(PohBee_SetupHitboxes);
    ADD_PUBLIC_FUNC(PohBee_State_Init);
    ADD_PUBLIC_FUNC(PohBee_State_Move);

    // MMZ/RPlaneShifter
    ADD_PUBLIC_FUNC(RPlaneShifter_DrawSprites);
    ADD_PUBLIC_FUNC(RPlaneShifter_HandlePlaneShift);
    ADD_PUBLIC_FUNC(RPlaneShifter_State_Init);
    ADD_PUBLIC_FUNC(RPlaneShifter_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(RPlaneShifter_State_Spinning);
    ADD_PUBLIC_FUNC(RPlaneShifter_State_FinishSpin);

    // MMZ/Scarab
    ADD_PUBLIC_FUNC(Scarab_DebugSpawn);
    ADD_PUBLIC_FUNC(Scarab_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Scarab_HandleChildMove);
    ADD_PUBLIC_FUNC(Scarab_HandlePlayerGrab);
    ADD_PUBLIC_FUNC(Scarab_HandlePlayerRelease);
    ADD_PUBLIC_FUNC(Scarab_State_Init);
    ADD_PUBLIC_FUNC(Scarab_State_Move);
    ADD_PUBLIC_FUNC(Scarab_State_Wait);

    // MMZ/SizeLaser
    ADD_PUBLIC_FUNC(SizeLaser_SfxCheck_SizeLaser);
    ADD_PUBLIC_FUNC(SizeLaser_SetPlayerSize);
    ADD_PUBLIC_FUNC(SizeLaser_PlayerState_Resize);
    ADD_PUBLIC_FUNC(SizeLaser_PlayerState_GrowNormal);
    ADD_PUBLIC_FUNC(SizeLaser_PlayerState_ShrinkChibi);
    ADD_PUBLIC_FUNC(SizeLaser_PlayerState_GrowGiant);
    ADD_PUBLIC_FUNC(SizeLaser_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SizeLaser_State_Emitter);
    ADD_PUBLIC_FUNC(SizeLaser_State_Laser);
    ADD_PUBLIC_FUNC(SizeLaser_State_Impact);

    // MMZ/SpikeCorridor
    ADD_PUBLIC_FUNC(SpikeCorridor_SetupHitboxes);
    ADD_PUBLIC_FUNC(SpikeCorridor_HandleDrawing);
    ADD_PUBLIC_FUNC(SpikeCorridor_SetupNextSpikeRow);
    ADD_PUBLIC_FUNC(SpikeCorridor_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateDropper_Setup);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateDropper_CheckForPlayer);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateDropper_DropWarn);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateDropper_SpawnSpikes);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateDropper_DropWait);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateSpikes_Setup);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateSpikes_Fall);
    ADD_PUBLIC_FUNC(SpikeCorridor_StateSpikes_Land);
    ADD_PUBLIC_FUNC(SpikeCorridor_Draw_DropWarn);
    ADD_PUBLIC_FUNC(SpikeCorridor_Draw_Spikes);

    // MMZ/VanishPlatform
    ADD_PUBLIC_FUNC(VanishPlatform_State_Appear);
    ADD_PUBLIC_FUNC(VanishPlatform_State_Disappear);

    // MSZ/Armadiloid
    ADD_PUBLIC_FUNC(Armadiloid_DebugSpawn);
    ADD_PUBLIC_FUNC(Armadiloid_State_Init);
    ADD_PUBLIC_FUNC(Armadiloid_State_PlatformFlying);
    ADD_PUBLIC_FUNC(Armadiloid_PlatformShootDelay);
    ADD_PUBLIC_FUNC(Armadiloid_State_PlatformShoot);
    ADD_PUBLIC_FUNC(Armadiloid_State_Rider);

    // MSZ/Bumpalo
    ADD_PUBLIC_FUNC(Bumpalo_DebugSpawn);
    ADD_PUBLIC_FUNC(Bumpalo_CheckOffScreen);
    ADD_PUBLIC_FUNC(Bumpalo_BumpPlayer);
    ADD_PUBLIC_FUNC(Bumpalo_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Bumpalo_HandlePlatformCollisions);
    ADD_PUBLIC_FUNC(Bumpalo_HandleObjectCollisions);
    ADD_PUBLIC_FUNC(Bumpalo_State_Init);
    ADD_PUBLIC_FUNC(Bumpalo_State_Moving);
    ADD_PUBLIC_FUNC(Bumpalo_State_Idle);
    ADD_PUBLIC_FUNC(Bumpalo_State_Charging);
    ADD_PUBLIC_FUNC(Bumpalo_State_Turning);
    ADD_PUBLIC_FUNC(Bumpalo_State_Bumped);
    ADD_PUBLIC_FUNC(Bumpalo_State_Falling);

    // MSZ/Cactula
    ADD_PUBLIC_FUNC(Cactula_DebugSpawn);
    ADD_PUBLIC_FUNC(Cactula_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Cactula_SfxChecK_CactChopper);
    ADD_PUBLIC_FUNC(Cactula_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Cactula_State_Rising);
    ADD_PUBLIC_FUNC(Cactula_State_DropBomb);

    // MSZ/CollapsingSand
    ADD_PUBLIC_FUNC(CollapsingSand_State_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(CollapsingSand_State_CollapseDelay);
    ADD_PUBLIC_FUNC(CollapsingSand_State_CollapseLeft);
    ADD_PUBLIC_FUNC(CollapsingSand_State_CollapseRight);

    // MSZ/DBTower
    ADD_PUBLIC_FUNC(DBTower_CheckPlayerCollisions_Head);
    ADD_PUBLIC_FUNC(DBTower_Explode);
    ADD_PUBLIC_FUNC(DBTower_State_SetupArena);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(DBTower_State_Setup_Encore);
    ADD_PUBLIC_FUNC(DBTower_State_HandleBoss);
#endif
    ADD_PUBLIC_FUNC(DBTower_State_Destroyed);
    ADD_PUBLIC_FUNC(DBTower_State_Finish);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(DBTower_State_SpawnSignPost);
    ADD_PUBLIC_FUNC(DBTower_CheckPlayerCollisions_Body);
#endif
    ADD_PUBLIC_FUNC(DBTower_State_BodyBouncing);
    ADD_PUBLIC_FUNC(DBTower_State_BodyRolling);

    // MSZ/EggLoco
    ADD_PUBLIC_FUNC(EggLoco_SfxCheck_LocoChugga);
    ADD_PUBLIC_FUNC(EggLoco_SfxUpdate_LocoChugga);
    ADD_PUBLIC_FUNC(EggLoco_State_TootDelay);
    ADD_PUBLIC_FUNC(EggLoco_State_Whistle);
    ADD_PUBLIC_FUNC(EggLoco_State_Tooting);

    // MSZ/GiantPistol
    ADD_PUBLIC_FUNC(GiantPistol_State_AwaitPlayerEntry);
    ADD_PUBLIC_FUNC(GiantPistol_State_CloseChamber);
    ADD_PUBLIC_FUNC(GiantPistol_State_SpinGun);
    ADD_PUBLIC_FUNC(GiantPistol_State_Aiming);
    ADD_PUBLIC_FUNC(GiantPistol_State_FiredPlayers);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(GiantPistol_PlayerState_PistolAir);
    ADD_PUBLIC_FUNC(GiantPistol_PlayerState_PistolGround);
#endif

    // MSZ/Hatterkiller
    ADD_PUBLIC_FUNC(Hatterkiller_DebugSpawn);

    // MSZ/HeavyMystic
    ADD_PUBLIC_FUNC(HeavyMystic_SpawnParticleFX);
    ADD_PUBLIC_FUNC(HeavyMystic_HandleParticleFX);
    ADD_PUBLIC_FUNC(HeavyMystic_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(HeavyMystic_Hit);
    ADD_PUBLIC_FUNC(HeavyMystic_CheckPlayerCollisions_Fang);
    ADD_PUBLIC_FUNC(HeavyMystic_CheckPlayerCollisions_Bark);
    ADD_PUBLIC_FUNC(HeavyMystic_CheckPlayerCollisions_Bean);
    ADD_PUBLIC_FUNC(HeavyMystic_Explode);
    ADD_PUBLIC_FUNC(HeavyMystic_Scanline_Curtains);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_Setup);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_SetupArena);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_AwaitPlayer);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_EnterMystic);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_HandleAppearArc);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BeginShow);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_Idle);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_MoveIntoPlace);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_FinishedMagicTrick);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_PrepareMagicTrick);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_GoodDay);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_ConjureHatterkiller);
    ADD_PUBLIC_FUNC(HeavyMystic_StateMischief_Disappear);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_Destroyed);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_Finish);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_CloseCurtains);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_AwaitBoxOpened);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_EnterMystic);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_GoodDay);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BoxCloseDelay);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_AwaitBoxClosing);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_AwaitBoxClosed);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_Transforming);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_ShowRouge);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_MysticReveal);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_MoveToBoxY);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_MoveToBoxX);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_TransformBackIntoRouge);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_FangIdle);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_FangTell);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_FangHop);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_RougeHit);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BarkIdle);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BarkPounding);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BarkJump);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BeanIdle);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BeanBomb1Throw);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BeanBomb2Throw);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBoss_BeanJump);
    ADD_PUBLIC_FUNC(HeavyMystic_StateCork_Fired);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(HeavyMystic_StateCork_MightyRebound);
    ADD_PUBLIC_FUNC(HeavyMystic_State_Bomb);
#endif
    ADD_PUBLIC_FUNC(HeavyMystic_State_BarkDebris);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_AwaitCurtainRise);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_Idle);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_CloseDoors);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_OpenDoors);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_Transforming);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_TransformFinish);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_Dropping);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_ShowContents);
    ADD_PUBLIC_FUNC(HeavyMystic_StateBox_Reappear);
    ADD_PUBLIC_FUNC(HeavyMystic_Draw_BoxOpened);
    ADD_PUBLIC_FUNC(HeavyMystic_Draw_BoxTransforming);
    ADD_PUBLIC_FUNC(HeavyMystic_Draw_BoxClosed);

    // MSZ/LightBulb
    ADD_PUBLIC_FUNC(LightBulb_DebugSpawn);
    ADD_PUBLIC_FUNC(LightBulb_State_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(LightBulb_State_Destroyed);

    // MSZ/MSZ1KIntro
    ADD_PUBLIC_FUNC(MSZ1KIntro_Cutscene_SetupPlane);
    ADD_PUBLIC_FUNC(MSZ1KIntro_Cutscene_MagicianMischief);
    ADD_PUBLIC_FUNC(MSZ1KIntro_Cutscene_StartAct);

    // MSZ/MSZ2Cutscene
    ADD_PUBLIC_FUNC(MSZ2Cutscene_SetupCutscene);
    ADD_PUBLIC_FUNC(MSZ2Cutscene_GetPistolPtr);
    ADD_PUBLIC_FUNC(MSZ2Cutscene_Cutscene_GoToPistol);
    ADD_PUBLIC_FUNC(MSZ2Cutscene_Cutscene_EnterPistol);
    ADD_PUBLIC_FUNC(MSZ2Cutscene_Cutscene_PistolFired);
    ADD_PUBLIC_FUNC(MSZ2Cutscene_Cutscene_AppearInBG);

#if MANIA_USE_PLUS
    // MSZ/MSZCutsceneK
    ADD_PUBLIC_FUNC(MSZCutsceneK_Cutscene_SkipCB);
    ADD_PUBLIC_FUNC(MSZCutsceneK_StartCutscene);
#endif
    ADD_PUBLIC_FUNC(MSZCutsceneK_SetupP2);
    ADD_PUBLIC_FUNC(MSZCutsceneK_Cutscene_RidingTornado);
    ADD_PUBLIC_FUNC(MSZCutsceneK_Cutscene_KnockedOffTornado);

    // MSZ/MSZCutsceneST
    ADD_PUBLIC_FUNC(MSZCutsceneST_SetupCutscene);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_HandleSignPostLand);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_AwaitActFinish);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_EnterMystic);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_PrepareAmbush);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_RougesAmbush);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_ShowFang);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_ShowBean);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_ShowBark);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_Mayday);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_SetPlayerMSZ2SpawnPos);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_PanCameraToPlayer);
    ADD_PUBLIC_FUNC(MSZCutsceneST_Cutscene_SetupMSZ2);

    // MSZ/MSZSetup
    ADD_PUBLIC_FUNC(MSZSetup_SetBGScrollOrigin);
    ADD_PUBLIC_FUNC(MSZSetup_StoreBGParallax);
    ADD_PUBLIC_FUNC(MSZSetup_ReloadBGParallax);
    ADD_PUBLIC_FUNC(MSZSetup_ReloadBGParallax_Multiply);
    ADD_PUBLIC_FUNC(MSZSetup_State_ManageFade_ST);
    ADD_PUBLIC_FUNC(MSZSetup_State_ManageFade_K);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MSZSetup_State_ManageFade_E);
    ADD_PUBLIC_FUNC(MSZSetup_StageFinish_EndAct1ST);
#endif
    ADD_PUBLIC_FUNC(MSZSetup_StageFinish_EndAct1K);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MSZSetup_StageFinish_EndAct1E);
    ADD_PUBLIC_FUNC(MSZSetup_StageFinish_EndAct2);
#endif
    ADD_PUBLIC_FUNC(MSZSetup_Trigger_AwardAchievement);
    ADD_PUBLIC_FUNC(MSZSetup_HandleRestart);
    ADD_PUBLIC_FUNC(MSZSetup_State_SwitchPalettes);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MSZSetup_State_CheckFadeTrigger_E);
    ADD_PUBLIC_FUNC(MSZSetup_State_CheckTrainStart);
    ADD_PUBLIC_FUNC(MSZSetup_State_TrainStarting);
    ADD_PUBLIC_FUNC(MSZSetup_State_TrainSequence_MSZ1E);
    ADD_PUBLIC_FUNC(MSZSetup_State_Boss_MSZ1E);
    ADD_PUBLIC_FUNC(MSZSetup_State_AwaitActClearStart);
    ADD_PUBLIC_FUNC(MSZSetup_State_AwaitActClearFinish);
    ADD_PUBLIC_FUNC(MSZSetup_State_MoveToMSZ2Start);
    ADD_PUBLIC_FUNC(MSZSetup_State_AwaitPlayerStopped);
    ADD_PUBLIC_FUNC(MSZSetup_State_StoreMSZ1ScrollPos_E);
    ADD_PUBLIC_FUNC(MSZSetup_State_StoreMSZ1ScrollPos_ST);
#endif
    ADD_PUBLIC_FUNC(MSZSetup_PlayerState_Pilot);
    ADD_PUBLIC_FUNC(MSZSetup_PlayerState_PostCrashJumpIn);

    // MSZ/MSZSpotlight
    ADD_PUBLIC_FUNC(MSZSpotlight_State_Appear);
    ADD_PUBLIC_FUNC(MSZSpotlight_State_Circling);
    ADD_PUBLIC_FUNC(MSZSpotlight_State_Idle);
    ADD_PUBLIC_FUNC(MSZSpotlight_State_MoveToBox);
    ADD_PUBLIC_FUNC(MSZSpotlight_State_Disappear);

    // MSZ/Pinata
    ADD_PUBLIC_FUNC(Pinata_DebugSpawn);
    ADD_PUBLIC_FUNC(Pinata_State_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Pinata_State_Destroyed);

    // MSZ/Rattlekiller
    ADD_PUBLIC_FUNC(Rattlekiller_DebugSpawn);
    ADD_PUBLIC_FUNC(Rattlekiller_HandleSorting);

    // MSZ/RollerMKII
    ADD_PUBLIC_FUNC(RollerMKII_DebugSpawn);
    ADD_PUBLIC_FUNC(RollerMKII_CheckOffScreen);
    ADD_PUBLIC_FUNC(RollerMKII_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(RollerMKII_CheckPlayerCollisions_Rolling);
    ADD_PUBLIC_FUNC(RollerMKII_HandleObjectCollisions);
    ADD_PUBLIC_FUNC(RollerMKII_HandlePlatformCollisions);
    ADD_PUBLIC_FUNC(RollerMKII_HandleCollisions);
    ADD_PUBLIC_FUNC(RollerMKII_State_Init);
    ADD_PUBLIC_FUNC(RollerMKII_State_Idle);
    ADD_PUBLIC_FUNC(RollerMKII_State_SpinUp);
    ADD_PUBLIC_FUNC(RollerMKII_State_RollDelay);
    ADD_PUBLIC_FUNC(RollerMKII_State_Rolling_Air);
    ADD_PUBLIC_FUNC(RollerMKII_State_Rolling_Ground);
    ADD_PUBLIC_FUNC(RollerMKII_State_Bumped);

    // MSZ/SeeSaw
    ADD_PUBLIC_FUNC(SeeSaw_SetupHitbox);
    ADD_PUBLIC_FUNC(SeeSaw_State_OrbIdle);
    ADD_PUBLIC_FUNC(SeeSaw_State_None);
    ADD_PUBLIC_FUNC(SeeSaw_State_NoOrb);
    ADD_PUBLIC_FUNC(SeeSaw_State_OrbLaunched);

    // MSZ/SeltzerBottle
    ADD_PUBLIC_FUNC(SeltzerBottle_State_Spraying);
    ADD_PUBLIC_FUNC(SeltzerBottle_State_TryReset);

    // MSZ/SeltzerWater
    ADD_PUBLIC_FUNC(SeltzerWater_State_Sprayed);
    ADD_PUBLIC_FUNC(SeltzerWater_State_Falling);
    ADD_PUBLIC_FUNC(SeltzerWater_State_Splash);

    // MSZ/SwingRope
    ADD_PUBLIC_FUNC(SwingRope_DebugSpawn);

    // MSZ/Tornado
    ADD_PUBLIC_FUNC(Tornado_State_Init);
    ADD_PUBLIC_FUNC(Tornado_State_SetupMSZ1Intro);
    ADD_PUBLIC_FUNC(Tornado_State_MSZ1Intro);
    ADD_PUBLIC_FUNC(Tornado_State_KnuxKnockedOff);
    ADD_PUBLIC_FUNC(Tornado_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(Tornado_State_PlayerControlled);
    ADD_PUBLIC_FUNC(Tornado_State_Mayday);
    ADD_PUBLIC_FUNC(Tornado_State_FlyAway_Right);
    ADD_PUBLIC_FUNC(Tornado_State_FlyAway_Left);

    // MSZ/TornadoPath
    ADD_PUBLIC_FUNC(TornadoPath_SetupHitbox);
    ADD_PUBLIC_FUNC(TornadoPath_HandleMoveSpeed);
    ADD_PUBLIC_FUNC(TornadoPath_State_SetTornadoSpeed);
    ADD_PUBLIC_FUNC(TornadoPath_State_ReturnCamera);
    ADD_PUBLIC_FUNC(TornadoPath_State_DisablePlayerInteractions);
    ADD_PUBLIC_FUNC(TornadoPath_State_ExitTornadoSequence);
    ADD_PUBLIC_FUNC(TornadoPath_State_PrepareCatchPlayer);
    ADD_PUBLIC_FUNC(TornadoPath_State_CatchPlayer);
    ADD_PUBLIC_FUNC(TornadoPath_State_UberCaterkillerBossNode);
    ADD_PUBLIC_FUNC(TornadoPath_State_HandleUberCaterkillerBoss);
    ADD_PUBLIC_FUNC(TornadoPath_State_GoToStopNode);
    ADD_PUBLIC_FUNC(TornadoPath_State_SetupMSZ1CutsceneST);
    ADD_PUBLIC_FUNC(TornadoPath_State_FinishCrash);

    // MSZ/UberCaterkiller
    ADD_PUBLIC_FUNC(UberCaterkiller_DebugSpawn);
    ADD_PUBLIC_FUNC(UberCaterkiller_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(UberCaterkiller_Hit);
    ADD_PUBLIC_FUNC(UberCaterkiller_Explode);
    ADD_PUBLIC_FUNC(UberCaterkiller_HandleSegmentMoveFX);
    ADD_PUBLIC_FUNC(UberCaterkiller_SetupBodySegments);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_SetupArena);
    ADD_PUBLIC_FUNC(UberCaterkiller_PrepareMoveIntoBG);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_MoveIntoBG);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_MoveToTargetPos);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_PrepareHorizontalJump);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_HorizontalJump);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_PrepareBGJump);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_FirstJump);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_RepeatedJumps);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_Destroyed);
    ADD_PUBLIC_FUNC(UberCaterkiller_State_Finish);

    // MSZ/Vultron
    ADD_PUBLIC_FUNC(Vultron_DebugSpawn);
    ADD_PUBLIC_FUNC(Vultron_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Vultron_CheckOffScreen);
    ADD_PUBLIC_FUNC(Vultron_State_Init);
    ADD_PUBLIC_FUNC(Vultron_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Vultron_State_Hop);
    ADD_PUBLIC_FUNC(Vultron_State_Dive);
    ADD_PUBLIC_FUNC(Vultron_State_Flying);
    ADD_PUBLIC_FUNC(Vultron_State_Rise);
    ADD_PUBLIC_FUNC(Vultron_State_PrepareDive);
    ADD_PUBLIC_FUNC(Vultron_State_Targeting);

    // OOZ/Aquis
    ADD_PUBLIC_FUNC(Aquis_DebugSpawn);
    ADD_PUBLIC_FUNC(Aquis_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Aquis_CheckOffScreen);
    ADD_PUBLIC_FUNC(Aquis_State_Init);
    ADD_PUBLIC_FUNC(Aquis_State_Idle);
    ADD_PUBLIC_FUNC(Aquis_State_Moving);
    ADD_PUBLIC_FUNC(Aquis_State_Shoot);
    ADD_PUBLIC_FUNC(Aquis_State_Turning);
    ADD_PUBLIC_FUNC(Aquis_State_Flee);
    ADD_PUBLIC_FUNC(Aquis_State_Shot);

    // OOZ/BallCannon
    ADD_PUBLIC_FUNC(BallCannon_CheckPlayerEntry);
    ADD_PUBLIC_FUNC(BallCannon_State_Idle);
    ADD_PUBLIC_FUNC(BallCannon_State_Inserted);
    ADD_PUBLIC_FUNC(BallCannon_State_Turning);
    ADD_PUBLIC_FUNC(BallCannon_State_EjectPlayer);
    ADD_PUBLIC_FUNC(BallCannon_State_CorkBlocked);
    ADD_PUBLIC_FUNC(BallCannon_State_CorkOpened);
    ADD_PUBLIC_FUNC(BallCannon_State_CorkDebris);

    // OOZ/GasPlatform
    ADD_PUBLIC_FUNC(GasPlatform_PopPlatform);
    ADD_PUBLIC_FUNC(GasPlatform_State_Popped);
    ADD_PUBLIC_FUNC(GasPlatform_State_SpringCooldown);
    ADD_PUBLIC_FUNC(GasPlatform_State_Shaking);

    // OOZ/Hatch
    ADD_PUBLIC_FUNC(Hatch_State_SubEntryHatch);
    ADD_PUBLIC_FUNC(Hatch_State_PlayerEntered);
    ADD_PUBLIC_FUNC(Hatch_State_Descend);
    ADD_PUBLIC_FUNC(Hatch_State_MoveToDestPos);
    ADD_PUBLIC_FUNC(Hatch_State_Surfacing);
    ADD_PUBLIC_FUNC(Hatch_State_OpenHatchReleasePlayer);
    ADD_PUBLIC_FUNC(Hatch_State_CloseHatch);
    ADD_PUBLIC_FUNC(Hatch_State_FadeIn);
    ADD_PUBLIC_FUNC(Hatch_State_SubExit);
    ADD_PUBLIC_FUNC(Hatch_State_FadeOut);
    ADD_PUBLIC_FUNC(Hatch_State_SubExitHatch);

    // OOZ/MegaOctus
    ADD_PUBLIC_FUNC(MegaOctus_CheckPlayerCollisions_Body);
    ADD_PUBLIC_FUNC(MegaOctus_HandleEggmanAnim);
    ADD_PUBLIC_FUNC(MegaOctus_Hit);
    ADD_PUBLIC_FUNC(MegaOctus_Explode);
    ADD_PUBLIC_FUNC(MegaOctus_HandleDirectionChange);
    ADD_PUBLIC_FUNC(MegaOctus_State_SetupBounds);
    ADD_PUBLIC_FUNC(MegaOctus_State_SetupArena);
    ADD_PUBLIC_FUNC(MegaOctus_State_None);
    ADD_PUBLIC_FUNC(MegaOctus_State_EnterMegaOctus);
    ADD_PUBLIC_FUNC(MegaOctus_State_OpenHatchAndLaugh);
    ADD_PUBLIC_FUNC(MegaOctus_State_CloseHatch);
    ADD_PUBLIC_FUNC(MegaOctus_State_DiveIntoOil);
    ADD_PUBLIC_FUNC(MegaOctus_State_SpawnWeapons);
    ADD_PUBLIC_FUNC(MegaOctus_State_CannonThenSpawnOrbs);
    ADD_PUBLIC_FUNC(MegaOctus_State_Destroyed);
    ADD_PUBLIC_FUNC(MegaOctus_State_Finish);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_Body);
    ADD_PUBLIC_FUNC(MegaOctus_CheckPlayerCollisions_Harpoon);
    ADD_PUBLIC_FUNC(MegaOctus_State_HarpoonLeft);
    ADD_PUBLIC_FUNC(MegaOctus_State_HarpoonRight);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_HarpoonLeft);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_HarpoonRight);
    ADD_PUBLIC_FUNC(MegaOctus_CheckPlayerCollisions_Cannon);
    ADD_PUBLIC_FUNC(MegaOctus_StateCannon_RiseUp);
    ADD_PUBLIC_FUNC(MegaOctus_StateCannon_Idle);
    ADD_PUBLIC_FUNC(MegaOctus_StateCannon_FireLaser);
    ADD_PUBLIC_FUNC(MegaOctus_StateCannon_SinkDown);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_Cannon);
    ADD_PUBLIC_FUNC(MegaOctus_CheckPlayerCollisions_Orb);
    ADD_PUBLIC_FUNC(MegaOctus_StateOrb_Wait);
    ADD_PUBLIC_FUNC(MegaOctus_StateOrb_FireShot);
    ADD_PUBLIC_FUNC(MegaOctus_StateOrb_Idle);
    ADD_PUBLIC_FUNC(MegaOctus_StateOrb_Destroyed);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_Orb);
    ADD_PUBLIC_FUNC(MegaOctus_StateArm_WrapAroundPlatform);
    ADD_PUBLIC_FUNC(MegaOctus_StateArm_GrabbedPlatform);
    ADD_PUBLIC_FUNC(MegaOctus_StateArm_PullPlatformDown);
    ADD_PUBLIC_FUNC(MegaOctus_StateArm_RisePlatformUp);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_Arm_WrapAroundPlatformBase);
    ADD_PUBLIC_FUNC(MegaOctus_StateArm_GrabPlatform);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_Arm_WrapAroundPlatformTop);
    ADD_PUBLIC_FUNC(MegaOctus_State_Laser);
    ADD_PUBLIC_FUNC(MegaOctus_State_LaserFire);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_Laser);
    ADD_PUBLIC_FUNC(MegaOctus_State_Shot);
    ADD_PUBLIC_FUNC(MegaOctus_Draw_OrbShot);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MegaOctus_TilePlatformState_RiseOuttaOil);
#endif

    // OOZ/MeterDroid
    ADD_PUBLIC_FUNC(MeterDroid_CheckPlayerCollisions_NoWrench_UseFlip);
    ADD_PUBLIC_FUNC(MeterDroid_CheckPlayerCollisions_Wrench_NoFlip);
    ADD_PUBLIC_FUNC(MeterDroid_CheckPlayerCollisions_NoWrench_NoFlip);
    ADD_PUBLIC_FUNC(MeterDroid_Hit);
    ADD_PUBLIC_FUNC(MeterDroid_Explode);
    ADD_PUBLIC_FUNC(MeterDroid_FindTargetValve);
    ADD_PUBLIC_FUNC(MeterDroid_PopPlatforms);
    ADD_PUBLIC_FUNC(MeterDroid_Draw_Normal);
    ADD_PUBLIC_FUNC(MeterDroid_Draw_SpinningValve);
    ADD_PUBLIC_FUNC(MeterDroid_Draw_ThrownWrench);
    ADD_PUBLIC_FUNC(MeterDroid_State_Init);
    ADD_PUBLIC_FUNC(MeterDroid_State_StartFight);
    ADD_PUBLIC_FUNC(MeterDroid_State_Idle);
    ADD_PUBLIC_FUNC(MeterDroid_State_PickMoveDir);
    ADD_PUBLIC_FUNC(MeterDroid_State_MoveRight);
    ADD_PUBLIC_FUNC(MeterDroid_State_MoveLeft);
    ADD_PUBLIC_FUNC(MeterDroid_State_ThrowWrench);
    ADD_PUBLIC_FUNC(MeterDroid_State_ThrownWrench);
    ADD_PUBLIC_FUNC(MeterDroid_State_CaughtWrench);
    ADD_PUBLIC_FUNC(MeterDroid_State_MoveToValve);
    ADD_PUBLIC_FUNC(MeterDroid_State_MoveIntoBG);
    ADD_PUBLIC_FUNC(MeterDroid_State_TurningValve);
    ADD_PUBLIC_FUNC(MeterDroid_State_StopTurningValve);
    ADD_PUBLIC_FUNC(MeterDroid_State_WatchPlatformsPopUp);
    ADD_PUBLIC_FUNC(MeterDroid_State_Destroyed);
    ADD_PUBLIC_FUNC(MeterDroid_State_FinishAct);

    // OOZ/Octus
    ADD_PUBLIC_FUNC(Octus_DebugSpawn);
    ADD_PUBLIC_FUNC(Octus_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Octus_CheckOffScreen);
    ADD_PUBLIC_FUNC(Octus_State_Init);
    ADD_PUBLIC_FUNC(Octus_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Octus_State_JumpDelay);
    ADD_PUBLIC_FUNC(Octus_State_Jump);
    ADD_PUBLIC_FUNC(Octus_State_Shoot);
    ADD_PUBLIC_FUNC(Octus_State_Fall);
    ADD_PUBLIC_FUNC(Octus_State_Shot);

    // OOZ/OOZ1Outro
    ADD_PUBLIC_FUNC(OOZ1Outro_Cutscene_FadeIn);
    ADD_PUBLIC_FUNC(OOZ1Outro_Cutscene_PostActClearSetup);
    ADD_PUBLIC_FUNC(OOZ1Outro_Cutscene_FallIntoAct2);
    ADD_PUBLIC_FUNC(OOZ1Outro_Cutscene_BeginAct);

    // OOZ/OOZ2Outro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(OOZ2Outro_StageFinish_EndAct2);
    ADD_PUBLIC_FUNC(OOZ2Outro_State_SubFloat);
    ADD_PUBLIC_FUNC(OOZ2Outro_CheckSkip);
    ADD_PUBLIC_FUNC(OOZ2Outro_State_BoardSub);
    ADD_PUBLIC_FUNC(OOZ2Outro_State_SubActivate);
    ADD_PUBLIC_FUNC(OOZ2Outro_State_SubLaunch);
#endif

    // OOZ/OOZFlames
    ADD_PUBLIC_FUNC(OOZFlames_State_Appear);
    ADD_PUBLIC_FUNC(OOZFlames_State_Rise);

    // OOZ/OOZSetup
    ADD_PUBLIC_FUNC(OOZSetup_SfxCheck_Flame2);
    ADD_PUBLIC_FUNC(OOZSetup_SfxCheck_Slide);
    ADD_PUBLIC_FUNC(OOZSetup_SfxCheck_OilSwim);
    ADD_PUBLIC_FUNC(OOZSetup_Draw_Flames);
    ADD_PUBLIC_FUNC(OOZSetup_HandleActiveFlames);
    ADD_PUBLIC_FUNC(OOZSetup_StartFire);
    ADD_PUBLIC_FUNC(OOZSetup_Trigger_AwardAchievement);
    ADD_PUBLIC_FUNC(OOZSetup_PlayerState_OilPool);
    ADD_PUBLIC_FUNC(OOZSetup_PlayerState_OilStrip);
    ADD_PUBLIC_FUNC(OOZSetup_PlayerState_OilSlide);
    ADD_PUBLIC_FUNC(OOZSetup_PlayerState_OilFall);

    // OOZ/PushSpring
    ADD_PUBLIC_FUNC(PushSpring_Collide_Top);
    ADD_PUBLIC_FUNC(PushSpring_Collide_Bottom);
    ADD_PUBLIC_FUNC(PushSpring_Collide_Left);
    ADD_PUBLIC_FUNC(PushSpring_Collide_Right);
    ADD_PUBLIC_FUNC(PushSpring_Draw_Top);
    ADD_PUBLIC_FUNC(PushSpring_Draw_Bottom);
    ADD_PUBLIC_FUNC(PushSpring_Draw_Left);
    ADD_PUBLIC_FUNC(PushSpring_Draw_Right);
    ADD_PUBLIC_FUNC(PushSpring_State_WaitForPushed);
    ADD_PUBLIC_FUNC(PushSpring_State_BeingPushed);
    ADD_PUBLIC_FUNC(PushSpring_State_PushRecoil);

    // OOZ/Smog
    ADD_PUBLIC_FUNC(Smog_DrawHook_ApplySmogPalette);

    // OOZ/Sol
    ADD_PUBLIC_FUNC(Sol_DebugSpawn);
    ADD_PUBLIC_FUNC(Sol_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Sol_HandlePlayerHurt);
    ADD_PUBLIC_FUNC(Sol_HandleRotation);
    ADD_PUBLIC_FUNC(Sol_CheckOffScreen);
    ADD_PUBLIC_FUNC(Sol_State_Init);
    ADD_PUBLIC_FUNC(Sol_State_Moving);
    ADD_PUBLIC_FUNC(Sol_State_ShootingOrbs);
    ADD_PUBLIC_FUNC(Sol_State_NoOrbs);
    ADD_PUBLIC_FUNC(Sol_State_SmallFireball);
    ADD_PUBLIC_FUNC(Sol_State_ActiveFireball);
    ADD_PUBLIC_FUNC(Sol_State_FlameDissipate);
    ADD_PUBLIC_FUNC(Sol_State_FireballOilFlame);
    ADD_PUBLIC_FUNC(Sol_State_OilFlame);

    // PGZ/Acetone
    ADD_PUBLIC_FUNC(Acetone_DrawGustFX);
    ADD_PUBLIC_FUNC(Acetone_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Acetone_HandleGustCount);
    ADD_PUBLIC_FUNC(Acetone_HandleGustPos);
    ADD_PUBLIC_FUNC(Acetone_State_AwaitInterval);
    ADD_PUBLIC_FUNC(Acetone_State_Dispensing);
    ADD_PUBLIC_FUNC(Acetone_State_StopDispensing);

    // PGZ/Crate
    ADD_PUBLIC_FUNC(Crate_Break);
    ADD_PUBLIC_FUNC(Crate_MoveY);
    ADD_PUBLIC_FUNC(Crate_Collide);
    ADD_PUBLIC_FUNC(Crate_State_None);
    ADD_PUBLIC_FUNC(Crate_State_ApplyGravity);
    ADD_PUBLIC_FUNC(Crate_State_WaitToFall);
    ADD_PUBLIC_FUNC(Crate_State_Fall);

    // PGZ/Dragonfly
    ADD_PUBLIC_FUNC(Dragonfly_DebugSpawn);
    ADD_PUBLIC_FUNC(Dragonfly_DebugDraw);
    ADD_PUBLIC_FUNC(Dragonfly_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Dragonfly_State_Init);
    ADD_PUBLIC_FUNC(Dragonfly_State_Move);
    ADD_PUBLIC_FUNC(Dragonfly_State_Debris);

    // PGZ/FrostThrower
    ADD_PUBLIC_FUNC(FrostThrower_DrawGustFX);
    ADD_PUBLIC_FUNC(FrostThrower_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(FrostThrower_HandleGustCount);
    ADD_PUBLIC_FUNC(FrostThrower_HandleGustPos);
    ADD_PUBLIC_FUNC(FrostThrower_State_AwaitInterval);
    ADD_PUBLIC_FUNC(FrostThrower_State_Dispensing);
    ADD_PUBLIC_FUNC(FrostThrower_State_StopDispensing);

    // PGZ/HeavyShinobi
    ADD_PUBLIC_FUNC(HeavyShinobi_HandleAfterFX);
    ADD_PUBLIC_FUNC(HeavyShinobi_HandleSlash);
    ADD_PUBLIC_FUNC(HeavyShinobi_StartJump);
    ADD_PUBLIC_FUNC(HeavyShinobi_Explode);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Init);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_SetupArena);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_StartFight);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Idle);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Slash);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Jump);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Glitched);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Destroyed);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_Finished);
    ADD_PUBLIC_FUNC(HeavyShinobi_Draw_Shinobi);
    ADD_PUBLIC_FUNC(HeavyShinobi_StateSlash_Active);
    ADD_PUBLIC_FUNC(HeavyShinobi_Draw_Slash);
    ADD_PUBLIC_FUNC(HeavyShinobi_StateAsteron_Thrown);
    ADD_PUBLIC_FUNC(HeavyShinobi_StateAsteron_Debris);
    ADD_PUBLIC_FUNC(HeavyShinobi_StateAsteron_Explode);
    ADD_PUBLIC_FUNC(HeavyShinobi_Draw_Asteron);
    ADD_PUBLIC_FUNC(HeavyShinobi_State_AsteronSpike);
    ADD_PUBLIC_FUNC(HeavyShinobi_Draw_AsteronSpike);
    ADD_PUBLIC_FUNC(HeavyShinobi_StateBounds_WaitForPlayer);
    ADD_PUBLIC_FUNC(HeavyShinobi_StateBounds_Active);
    ADD_PUBLIC_FUNC(HeavyShinobi_Draw_Bounds);

    // PGZ/Ice
    ADD_PUBLIC_FUNC(Ice_VSSwap_CheckFrozen);
    ADD_PUBLIC_FUNC(Ice_FreezePlayer);
    ADD_PUBLIC_FUNC(Ice_CheckPlayerBlockSmashH);
    ADD_PUBLIC_FUNC(Ice_CheckPlayerBlockSmashV);
    ADD_PUBLIC_FUNC(Ice_PlayerState_Frozen);
    ADD_PUBLIC_FUNC(Ice_ShatterGenerator);
    ADD_PUBLIC_FUNC(Ice_FullShatter);
    ADD_PUBLIC_FUNC(Ice_BreakPlayerBlock);
    ADD_PUBLIC_FUNC(Ice_Shatter);
    ADD_PUBLIC_FUNC(Ice_TimeOver_CheckFrozen);
    ADD_PUBLIC_FUNC(Ice_UpdateBlockGravity);
    ADD_PUBLIC_FUNC(Ice_State_IceBlock);
    ADD_PUBLIC_FUNC(Ice_State_StartBlockFall);
    ADD_PUBLIC_FUNC(Ice_State_BlockFallDelay);
    ADD_PUBLIC_FUNC(Ice_State_IceBlockFall);
    ADD_PUBLIC_FUNC(Ice_State_PlayerBlock);
    ADD_PUBLIC_FUNC(Ice_State_Shard);
    ADD_PUBLIC_FUNC(Ice_Draw_IceBlock);
    ADD_PUBLIC_FUNC(Ice_Draw_PlayerBlock);
    ADD_PUBLIC_FUNC(Ice_Draw_Pillar);
    ADD_PUBLIC_FUNC(Ice_Draw_Shard);

    // PGZ/IceBomba
    ADD_PUBLIC_FUNC(IceBomba_DebugSpawn);
    ADD_PUBLIC_FUNC(IceBomba_CheckOffScreen);
    ADD_PUBLIC_FUNC(IceBomba_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(IceBomba_State_Init);
    ADD_PUBLIC_FUNC(IceBomba_State_Flying);
    ADD_PUBLIC_FUNC(IceBomba_State_Turning);
    ADD_PUBLIC_FUNC(IceBomba_State_FlyAway);
    ADD_PUBLIC_FUNC(IceBomba_State_Bomb);

    // PGZ/IceSpring
    ADD_PUBLIC_FUNC(IceSpring_Shatter);

    // PGZ/JuggleSaw
    ADD_PUBLIC_FUNC(JuggleSaw_DebugSpawn);
    ADD_PUBLIC_FUNC(JuggleSaw_DebugDraw);
    ADD_PUBLIC_FUNC(JuggleSaw_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(JuggleSaw_CheckOffScreen);
    ADD_PUBLIC_FUNC(JuggleSaw_StateCrab_Setup);
    ADD_PUBLIC_FUNC(JuggleSaw_StateCrab_Handle);
    ADD_PUBLIC_FUNC(JuggleSaw_StateCrab_ThrowSaw);
    ADD_PUBLIC_FUNC(JuggleSaw_StateSaw_Handle);
    ADD_PUBLIC_FUNC(JuggleSaw_StateSaw_Debris);

#if MANIA_USE_PLUS
    // PGZ/Newspaper
    ADD_PUBLIC_FUNC(Newspaper_HandleInteractions);
#endif

    // PGZ/PaperRoller
    ADD_PUBLIC_FUNC(PaperRoller_DrawPaperLines);
    ADD_PUBLIC_FUNC(PaperRoller_DrawRollers);
    ADD_PUBLIC_FUNC(PaperRoller_HandleRollerCollisions);
    ADD_PUBLIC_FUNC(PaperRoller_HandlePrintCollisions);

    // PGZ/PetalPile
    ADD_PUBLIC_FUNC(PetalPile_GetLeafPattern);
    ADD_PUBLIC_FUNC(PetalPile_State_Init);
    ADD_PUBLIC_FUNC(PetalPile_State_HandleInteractions);
    ADD_PUBLIC_FUNC(PetalPile_State_SetupEmitter);
    ADD_PUBLIC_FUNC(PetalPile_State_Emitter);
    ADD_PUBLIC_FUNC(PetalPile_StateLeaf_Setup);
    ADD_PUBLIC_FUNC(PetalPile_StateLeaf_Delay);
    ADD_PUBLIC_FUNC(PetalPile_StateLeaf_HandleVelocity);
    ADD_PUBLIC_FUNC(PetalPile_StateLeaf_Fall);
    ADD_PUBLIC_FUNC(PetalPile_Draw_Leaf);

    // PGZ/Press
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Press_CheckCanSuper);
#endif
    ADD_PUBLIC_FUNC(Press_Move);
    ADD_PUBLIC_FUNC(Press_State_Crush);
    ADD_PUBLIC_FUNC(Press_State_FinalCrush);
    ADD_PUBLIC_FUNC(Press_HandleMovement);
    ADD_PUBLIC_FUNC(Press_State_HandleCrates);
    ADD_PUBLIC_FUNC(Press_DrawHandle);

    // PGZ/PrintBlock
    ADD_PUBLIC_FUNC(PrintBlock_State_Appear);
    ADD_PUBLIC_FUNC(PrintBlock_State_Disappear);

    // PGZ/PSZ1Intro
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PSZ1Intro_HandleGliderJump);
    ADD_PUBLIC_FUNC(PSZ1Intro_Cutscene_SetupGliders);
    ADD_PUBLIC_FUNC(PSZ1Intro_Cutscene_GlideAndJump);
    ADD_PUBLIC_FUNC(PSZ1Intro_Cutscene_HandleLanding);
#endif

#if MANIA_USE_PLUS
    // PGZ/PSZ1Setup
    ADD_PUBLIC_FUNC(PSZ1Setup_BGSwitch_Inside);
    ADD_PUBLIC_FUNC(PSZ1Setup_BGSwitch_Outside);
    ADD_PUBLIC_FUNC(PSZ1Setup_Trigger_DeactivatePetalBehaviour);
#endif
    ADD_PUBLIC_FUNC(PSZ1Setup_Trigger_ActivatePetalBehaviour);
    ADD_PUBLIC_FUNC(PSZ1Setup_Trigger_AwardAchievement);
    ADD_PUBLIC_FUNC(PSZ1Setup_StageFinish_EndAct1);
    ADD_PUBLIC_FUNC(PSZ1Setup_LevelWrap_Top);
    ADD_PUBLIC_FUNC(PSZ1Setup_LevelWrap_Bottom);

    // PGZ/PSZ2Intro
    ADD_PUBLIC_FUNC(PSZ2Intro_Cutscene_HandleAct1Finish);
    ADD_PUBLIC_FUNC(PSZ2Intro_Cutscene_ShowActClear);
    ADD_PUBLIC_FUNC(PSZ2Intro_Cutscene_RunToAct2);
    ADD_PUBLIC_FUNC(PSZ2Intro_Cutscene_JogIntoPlace);

    // PGZ/PSZ2Outro
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_SetupCameraMove);
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_HandleCameraMovement);
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_WalkIntoPlace);
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_EnterRuby);
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_RubyActivated);
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_RubyWarp);
    ADD_PUBLIC_FUNC(PSZ2Outro_Cutscene_LoadSSZ1);

    // PGZ/PSZ2Setup
    ADD_PUBLIC_FUNC(PSZ2Setup_Trigger_ActivatePetalBehaviour);
    ADD_PUBLIC_FUNC(PSZ2Setup_Trigger_DeactivatePetalBehaviour);
    ADD_PUBLIC_FUNC(PSZ2Setup_ActTransitionLoad);
    ADD_PUBLIC_FUNC(PSZ2Setup_StageFinish_EndAct2);

    // PGZ/PSZDoor
    ADD_PUBLIC_FUNC(PSZDoor_GetRemainingDistance);

    // PGZ/PSZEggman
    ADD_PUBLIC_FUNC(PSZEggman_State_TurnRound);

    // PGZ/PSZLauncher
    ADD_PUBLIC_FUNC(PSZLauncher_DebugSpawn);
    ADD_PUBLIC_FUNC(PSZLauncher_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(PSZLauncher_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(PSZLauncher_State_Init);
    ADD_PUBLIC_FUNC(PSZLauncher_State_Active);

    // PGZ/Shiversaw
    ADD_PUBLIC_FUNC(Shiversaw_CheckSawHit);
    ADD_PUBLIC_FUNC(Shiversaw_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Shiversaw_SetupSawPos);
    ADD_PUBLIC_FUNC(Shiversaw_Hit);
    ADD_PUBLIC_FUNC(Shiversaw_Explode);
    ADD_PUBLIC_FUNC(Shiversaw_CheckBoxCollisions);
    ADD_PUBLIC_FUNC(Shiversaw_State_Entry);
    ADD_PUBLIC_FUNC(Shiversaw_State_SetupBounds);
    ADD_PUBLIC_FUNC(Shiversaw_State_EnterShiversaw);
    ADD_PUBLIC_FUNC(Shiversaw_State_HitRecoil_Tutorial);
    ADD_PUBLIC_FUNC(Shiversaw_State_Idle);
    ADD_PUBLIC_FUNC(Shiversaw_State_MoveToPlayer);
    ADD_PUBLIC_FUNC(Shiversaw_State_HitRecoil);
    ADD_PUBLIC_FUNC(Shiversaw_State_Destroyed);
    ADD_PUBLIC_FUNC(Shiversaw_State_StageFinishCB);
    ADD_PUBLIC_FUNC(Shiversaw_State_Explode);
    ADD_PUBLIC_FUNC(Shiversaw_State_Explode_NoTransition);
    ADD_PUBLIC_FUNC(Shiversaw_State_DropSignPost);
    ADD_PUBLIC_FUNC(Shiversaw_CheckCrateCollisions);
    ADD_PUBLIC_FUNC(Shiversaw_HandleSawMovement);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Active);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Targeting);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Extend);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Retract);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Destroyed);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Form);
    ADD_PUBLIC_FUNC(Shiversaw_StateSaw_Setup);
    ADD_PUBLIC_FUNC(Shiversaw_ProcessSawMovement);
    ADD_PUBLIC_FUNC(Shiversaw_StateDust_Debris);

    // PGZ/Shuriken
    ADD_PUBLIC_FUNC(Shuriken_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Shuriken_HandleSolidCollisions);
    ADD_PUBLIC_FUNC(Shuriken_State_Init);
    ADD_PUBLIC_FUNC(Shuriken_State_AwaitActivate);
    ADD_PUBLIC_FUNC(Shuriken_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Shuriken_State_ShootDelay);
    ADD_PUBLIC_FUNC(Shuriken_State_FireShuriken);
    ADD_PUBLIC_FUNC(Shuriken_State_ShurikenFired);
    ADD_PUBLIC_FUNC(Shuriken_State_Deactivate);
    ADD_PUBLIC_FUNC(Shuriken_State_InitShuriken);
    ADD_PUBLIC_FUNC(Shuriken_State_ShurikenThrown);
    ADD_PUBLIC_FUNC(Shuriken_State_ShurikenStuck);
    ADD_PUBLIC_FUNC(Shuriken_State_ShurikenFall);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Shuriken_State_ShurikenDebris);
#endif

    // PGZ/Snowflakes
    ADD_PUBLIC_FUNC(Snowflakes_HandleWrap);

    // PGZ/SP500
    ADD_PUBLIC_FUNC(SP500_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SP500_State_WaitForEntry);
    ADD_PUBLIC_FUNC(SP500_State_Finished);
    ADD_PUBLIC_FUNC(SP500_State_Activate);
    ADD_PUBLIC_FUNC(SP500_State_PrintFailed);
    ADD_PUBLIC_FUNC(SP500_State_PrintDelay);
    ADD_PUBLIC_FUNC(SP500_State_Printing);
    ADD_PUBLIC_FUNC(SP500_State_NextPrintRow);
    ADD_PUBLIC_FUNC(SP500_State_PrintFinished);
    ADD_PUBLIC_FUNC(SP500_State_MoveToTarget);

    // PGZ/SP500MkII
    ADD_PUBLIC_FUNC(SP500MkII_DrawDebugOverlay);
    ADD_PUBLIC_FUNC(SP500MkII_DrawPrinter);
    ADD_PUBLIC_FUNC(SP500MkII_DrawRails);
    ADD_PUBLIC_FUNC(SP500MkII_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SP500MkII_State_Init);
    ADD_PUBLIC_FUNC(SP500MkII_State_AwaitActivation);
    ADD_PUBLIC_FUNC(SP500MkII_State_PrintRow);
    ADD_PUBLIC_FUNC(SP500MkII_State_NextPrintRow);

    // PGZ/Turntable
    ADD_PUBLIC_FUNC(Turntable_SetupSize);

    // PGZ/WoodChipper
    ADD_PUBLIC_FUNC(WoodChipper_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(WoodChipper_State_Chipper);
    ADD_PUBLIC_FUNC(WoodChipper_State_Debris);

    // PGZ/Woodrow
    ADD_PUBLIC_FUNC(Woodrow_DebugSpawn);
    ADD_PUBLIC_FUNC(Woodrow_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Woodrow_CheckOffScreen);
    ADD_PUBLIC_FUNC(Woodrow_State_Init);
    ADD_PUBLIC_FUNC(Woodrow_State_Idle);
    ADD_PUBLIC_FUNC(Woodrow_State_MoveUp);
    ADD_PUBLIC_FUNC(Woodrow_State_MoveDown);
    ADD_PUBLIC_FUNC(Woodrow_State_Bomb);
    ADD_PUBLIC_FUNC(Woodrow_State_BombSpawner);

    // Pinball/PBL_Bumper
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Bumper_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(PBL_Bumper_State_CheckBumps);
    ADD_PUBLIC_FUNC(PBL_Bumper_State_Bumped);
    ADD_PUBLIC_FUNC(PBL_Bumper_State_FinishedBump);
#endif

    // Pinball/PBL_Camera
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Camera_HandleScreenPos);
    ADD_PUBLIC_FUNC(PBL_Camera_State_Normal);
#endif

    // Pinball/PBL_Crane
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Crane_HandlePrizes);
    ADD_PUBLIC_FUNC(PBL_Crane_Draw_CraneMachine);
    ADD_PUBLIC_FUNC(PBL_Crane_Draw_PrizeDisplay);
    ADD_PUBLIC_FUNC(PBL_Crane_Draw_Crane);
    ADD_PUBLIC_FUNC(PBL_Crane_State_CreatePrizes);
    ADD_PUBLIC_FUNC(PBL_Crane_State_DisplayPrizes);
    ADD_PUBLIC_FUNC(PBL_Crane_StatePrizeDisplay_Move);
    ADD_PUBLIC_FUNC(PBL_Crane_StateCrane_Lower);
    ADD_PUBLIC_FUNC(PBL_Crane_StateCrane_Move);
    ADD_PUBLIC_FUNC(PBL_Crane_StateCrane_Controlled);
    ADD_PUBLIC_FUNC(PBL_Crane_StateCrane_Drop);
    ADD_PUBLIC_FUNC(PBL_Crane_StateCrane_Grab);
    ADD_PUBLIC_FUNC(PBL_Crane_StateCrane_Rise);
    ADD_PUBLIC_FUNC(PBL_Crane_StatePrize_Bounce);
    ADD_PUBLIC_FUNC(PBL_Crane_StatePrize_Flash);
    ADD_PUBLIC_FUNC(PBL_Crane_StatePrize_PrizeGet);
#endif

    // Pinball/PBL_Flipper
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Flipper_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(PBL_Flipper_State_AwaitFlip);
    ADD_PUBLIC_FUNC(PBL_Flipper_State_RiseFlipper);
    ADD_PUBLIC_FUNC(PBL_Flipper_State_LowerFlipper);
#endif

    // Pinball/PBL_HUD
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_HUD_DisplayMessage);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_ScrollLeftSlow);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_ScrollLeftFast);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_ScrollRightSlow);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_ScrollRightFast);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_ShowMove);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_Flash);
    ADD_PUBLIC_FUNC(PBL_HUD_StateMessage_FlashThenCrane);
    ADD_PUBLIC_FUNC(PBL_HUD_Draw_Basic);
    ADD_PUBLIC_FUNC(PBL_HUD_Draw_Message);
    ADD_PUBLIC_FUNC(PBL_HUD_Draw_Score);
    ADD_PUBLIC_FUNC(PBL_HUD_State_RevealCrane);
    ADD_PUBLIC_FUNC(PBL_HUD_State_HideCrane);
#endif

    // Pinball/PBL_Player
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Player_Input_P1);
    ADD_PUBLIC_FUNC(PBL_Player_State_Launcher);
    ADD_PUBLIC_FUNC(PBL_Player_State_Ground);
    ADD_PUBLIC_FUNC(PBL_Player_State_Air);
#endif

    // Pinball/PBL_Ring
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Ring_GiveRing);
    ADD_PUBLIC_FUNC(PBL_Ring_State_Ring);
    ADD_PUBLIC_FUNC(PBL_Ring_State_RingSparkle);
#endif

    // Pinball/PBL_Setup
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_Setup_Scanline_TableLow);
    ADD_PUBLIC_FUNC(PBL_Setup_Scanline_TableHigh);
    ADD_PUBLIC_FUNC(PBL_Setup_Scanline_PinballBG);
    ADD_PUBLIC_FUNC(PBL_Setup_DrawHook_PrepareDrawingFX);
    ADD_PUBLIC_FUNC(PBL_Setup_ExitPinball);
    ADD_PUBLIC_FUNC(PBL_Setup_GiveScore);
    ADD_PUBLIC_FUNC(PBL_Setup_GiveLife);
    ADD_PUBLIC_FUNC(PBL_Setup_State_FadeIn);
    ADD_PUBLIC_FUNC(PBL_Setup_State_ManageStageExit);
    ADD_PUBLIC_FUNC(PBL_Setup_SaveAndChangeScene);
#endif

    // Pinball/PBL_TargetBumper
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(PBL_TargetBumper_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(PBL_TargetBumper_State_Active);
    ADD_PUBLIC_FUNC(PBL_TargetBumper_State_Reced);
    ADD_PUBLIC_FUNC(PBL_TargetBumper_State_Rise);
#endif

    // Puyo/PuyoAI
    ADD_PUBLIC_FUNC(PuyoAI_GetBeanPos);
    ADD_PUBLIC_FUNC(PuyoAI_PrepareAction);
    ADD_PUBLIC_FUNC(PuyoAI_GetChainComboSize);
    ADD_PUBLIC_FUNC(PuyoAI_SetupInputs);
    ADD_PUBLIC_FUNC(PuyoAI_Input_AI);

    // Puyo/PuyoBean
    ADD_PUBLIC_FUNC(PuyoBean_GetPuyoBean);
    ADD_PUBLIC_FUNC(PuyoBean_Input_Player);
    ADD_PUBLIC_FUNC(PuyoBean_DestroyPuyoBeans);
    ADD_PUBLIC_FUNC(PuyoBean_HandleBeanLinks);
    ADD_PUBLIC_FUNC(PuyoBean_CheckBeanLinks);
    ADD_PUBLIC_FUNC(PuyoBean_HandleMoveBounds);
    ADD_PUBLIC_FUNC(PuyoBean_CheckAIRotationDisabled);
    ADD_PUBLIC_FUNC(PuyoBean_CheckCollisions);
    ADD_PUBLIC_FUNC(PuyoBean_GetBeanChainRemovalCount);
    ADD_PUBLIC_FUNC(PuyoBean_GetAvaliableLinks);
    ADD_PUBLIC_FUNC(PuyoBean_CheckLinkPosAvaliable);
    ADD_PUBLIC_FUNC(PuyoBean_SetupBeanLinkTable);
    ADD_PUBLIC_FUNC(PuyoBean_GetColumnHeight);
    ADD_PUBLIC_FUNC(PuyoBean_CalculateStillPos);
    ADD_PUBLIC_FUNC(PuyoBean_State_PartnerControlled);
    ADD_PUBLIC_FUNC(PuyoBean_State_Controlled);
    ADD_PUBLIC_FUNC(PuyoBean_State_BeanIdle);
    ADD_PUBLIC_FUNC(PuyoBean_State_Falling);
    ADD_PUBLIC_FUNC(PuyoBean_State_BeanLand);
    ADD_PUBLIC_FUNC(PuyoBean_State_JunkLand);
    ADD_PUBLIC_FUNC(PuyoBean_State_JunkIdle);
    ADD_PUBLIC_FUNC(PuyoBean_State_JunkPopped);
    ADD_PUBLIC_FUNC(PuyoBean_State_BeginBeanPop);
    ADD_PUBLIC_FUNC(PuyoBean_State_BeanPop);
    ADD_PUBLIC_FUNC(PuyoBean_State_MatchLoseFall);

    // Puyo/PuyoGame
    ADD_PUBLIC_FUNC(PuyoGame_SetupStartingEntities);
    ADD_PUBLIC_FUNC(PuyoGame_SetLoser);
    ADD_PUBLIC_FUNC(PuyoGame_DestroyPuyoBeans);
    ADD_PUBLIC_FUNC(PuyoGame_SetupGameState);
    ADD_PUBLIC_FUNC(PuyoGame_CheckMatchFinish);
    ADD_PUBLIC_FUNC(PuyoGame_State_Init);
    ADD_PUBLIC_FUNC(PuyoGame_State_Wait);
    ADD_PUBLIC_FUNC(PuyoGame_State_SelectingLevel);
    ADD_PUBLIC_FUNC(PuyoGame_State_SetupRound);
    ADD_PUBLIC_FUNC(PuyoGame_State_SetupEntities);
    ADD_PUBLIC_FUNC(PuyoGame_State_HandleRound);
    ADD_PUBLIC_FUNC(PuyoGame_State_ShowRoundResults);
    ADD_PUBLIC_FUNC(PuyoGame_State_ShowMatchResults);
    ADD_PUBLIC_FUNC(PuyoGame_State_FadeToMenu);

    // Puyo/PuyoIndicator
    ADD_PUBLIC_FUNC(PuyoIndicator_ShowWinner);
    ADD_PUBLIC_FUNC(PuyoIndicator_ShowLoser);
    ADD_PUBLIC_FUNC(PuyoIndicator_ShowReady);

    // Puyo/PuyoLevelSelect
    ADD_PUBLIC_FUNC(PuyoLevelSelect_DrawSprites);
    ADD_PUBLIC_FUNC(PuyoLevelSelect_HandleMenuMovement);

    // Puyo/PuyoMatch
    ADD_PUBLIC_FUNC(PuyoMatch_AddPuyoCombo);
    ADD_PUBLIC_FUNC(PuyoMatch_SetupNextBeans);
    ADD_PUBLIC_FUNC(PuyoMatch_DropNextBeans);
    ADD_PUBLIC_FUNC(PuyoMatch_DropJunkBeans);
    ADD_PUBLIC_FUNC(PuyoMatch_DrawJunkBeanPreviews);
    ADD_PUBLIC_FUNC(PuyoMatch_State_HandleMatch);
    ADD_PUBLIC_FUNC(PuyoMatch_State_HandleCombos);
    ADD_PUBLIC_FUNC(PuyoMatch_State_HandleComboEnd);
    ADD_PUBLIC_FUNC(PuyoMatch_State_Lose);

    // Puyo/PuyoScore
    ADD_PUBLIC_FUNC(PuyoScore_DrawScore);

    // SBZ/Bomb
    ADD_PUBLIC_FUNC(Bomb_DebugSpawn);
    ADD_PUBLIC_FUNC(Bomb_CheckOffScreen);
    ADD_PUBLIC_FUNC(Bomb_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Bomb_State_Init);
    ADD_PUBLIC_FUNC(Bomb_State_Walk);
    ADD_PUBLIC_FUNC(Bomb_State_Idle);
    ADD_PUBLIC_FUNC(Bomb_State_Explode);
    ADD_PUBLIC_FUNC(Bomb_State_Shrapnel);

    // SBZ/Caterkiller
    ADD_PUBLIC_FUNC(Caterkiller_DebugSpawn);
    ADD_PUBLIC_FUNC(Caterkiller_CheckOffScreen);
    ADD_PUBLIC_FUNC(Caterkiller_CheckTileCollisions);
    ADD_PUBLIC_FUNC(Caterkiller_Draw_Body);
    ADD_PUBLIC_FUNC(Caterkiller_Draw_Segment);
    ADD_PUBLIC_FUNC(Caterkiller_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Caterkiller_CheckTileAngle);
    ADD_PUBLIC_FUNC(Caterkiller_State_Init);
    ADD_PUBLIC_FUNC(Caterkiller_State_Contract);
    ADD_PUBLIC_FUNC(Caterkiller_State_LiftHead);
    ADD_PUBLIC_FUNC(Caterkiller_State_Uncontract);
    ADD_PUBLIC_FUNC(Caterkiller_State_LowerHead);
    ADD_PUBLIC_FUNC(Caterkiller_StateSplit_Head);
    ADD_PUBLIC_FUNC(Caterkiller_StateSplit_Body);

    // SBZ/Orbinaut
    ADD_PUBLIC_FUNC(Orbinaut_DebugSpawn);
    ADD_PUBLIC_FUNC(Orbinaut_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Orbinaut_HandleRotation);
    ADD_PUBLIC_FUNC(Orbinaut_CheckOffScreen);
    ADD_PUBLIC_FUNC(Orbinaut_State_Init);
    ADD_PUBLIC_FUNC(Orbinaut_State_Moving);
    ADD_PUBLIC_FUNC(Orbinaut_State_ReleasingOrbs);
    ADD_PUBLIC_FUNC(Orbinaut_State_Orbless);
    ADD_PUBLIC_FUNC(Orbinaut_State_Orb);
    ADD_PUBLIC_FUNC(Orbinaut_State_OrbDebris);

    // SPZ/CableWarp
    ADD_PUBLIC_FUNC(CableWarp_State_CablePlug);
    ADD_PUBLIC_FUNC(CableWarp_State_CheckPlayerEntry);
    ADD_PUBLIC_FUNC(CableWarp_StateTransport_BeginEnter);
    ADD_PUBLIC_FUNC(CableWarp_StateTransport_Enter);
    ADD_PUBLIC_FUNC(CableWarp_StateTransport_MoveToNextNode);
    ADD_PUBLIC_FUNC(CableWarp_StateTransport_EndNode);
    ADD_PUBLIC_FUNC(CableWarp_StateTransport_Exit);

    // SPZ/Canista
    ADD_PUBLIC_FUNC(Canista_DebugSpawn);
    ADD_PUBLIC_FUNC(Canista_CheckPlayerBadnikCollisions);
    ADD_PUBLIC_FUNC(Canista_CheckOffScreen);
    ADD_PUBLIC_FUNC(Canista_State_Init);
    ADD_PUBLIC_FUNC(Canista_State_Moving);
    ADD_PUBLIC_FUNC(Canista_State_Idle);
    ADD_PUBLIC_FUNC(Canista_CheckPlayerProjectileCollisions);
    ADD_PUBLIC_FUNC(Canista_StateProjectile_Shot);
    ADD_PUBLIC_FUNC(Canista_StateProjectile_Fall);

    // SPZ/CircleBumper
    ADD_PUBLIC_FUNC(CircleBumper_DebugSpawn);
    ADD_PUBLIC_FUNC(CircleBumper_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(CircleBumper_Collide_Normal);
    ADD_PUBLIC_FUNC(CircleBumper_Collide_Bumped);
    ADD_PUBLIC_FUNC(CircleBumper_Move_Fixed);
    ADD_PUBLIC_FUNC(CircleBumper_Move_Linear);
    ADD_PUBLIC_FUNC(CircleBumper_Move_Circular);
    ADD_PUBLIC_FUNC(CircleBumper_Move_Path);
    ADD_PUBLIC_FUNC(CircleBumper_Move_Track);

    // SPZ/Clapperboard
    ADD_PUBLIC_FUNC(Clapperboard_Collide_Left);
    ADD_PUBLIC_FUNC(Clapperboard_Collide_Right);
    ADD_PUBLIC_FUNC(Clapperboard_State_Idle);
    ADD_PUBLIC_FUNC(Clapperboard_State_ClappingL);
    ADD_PUBLIC_FUNC(Clapperboard_State_ClapReboundL);
    ADD_PUBLIC_FUNC(Clapperboard_State_ClappingR);
    ADD_PUBLIC_FUNC(Clapperboard_State_ClapReboundR);

    // SPZ/DirectorChair
    ADD_PUBLIC_FUNC(DirectorChair_Collide_Chair);
    ADD_PUBLIC_FUNC(DirectorChair_State_Idle);
    ADD_PUBLIC_FUNC(DirectorChair_State_StartExtend);
    ADD_PUBLIC_FUNC(DirectorChair_State_Extend);
    ADD_PUBLIC_FUNC(DirectorChair_State_StartRetract);
    ADD_PUBLIC_FUNC(DirectorChair_State_Retract);

    // SPZ/EggJanken
    ADD_PUBLIC_FUNC(EggJanken_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(EggJanken_HandleMovement);
    ADD_PUBLIC_FUNC(EggJanken_Explode);
    ADD_PUBLIC_FUNC(EggJanken_ResetStates);
    ADD_PUBLIC_FUNC(EggJanken_SwapArmSwingDir);
    ADD_PUBLIC_FUNC(EggJanken_Result_PlayerWins);
    ADD_PUBLIC_FUNC(EggJanken_Result_PlayerLoses);
    ADD_PUBLIC_FUNC(EggJanken_Result_PlayerDraws);
    ADD_PUBLIC_FUNC(EggJanken_State_SetupArena);
    ADD_PUBLIC_FUNC(EggJanken_State_StartFight);
    ADD_PUBLIC_FUNC(EggJanken_State_EnterJanken);
    ADD_PUBLIC_FUNC(EggJanken_State_AwaitButtonPress);
    ADD_PUBLIC_FUNC(EggJanken_State_Opened);
    ADD_PUBLIC_FUNC(EggJanken_State_InitialArmExtend);
    ADD_PUBLIC_FUNC(EggJanken_State_InitialArmRaise);
    ADD_PUBLIC_FUNC(EggJanken_State_None);
    ADD_PUBLIC_FUNC(EggJanken_State_Destroyed);
    ADD_PUBLIC_FUNC(EggJanken_State_ButtonPressed);
    ADD_PUBLIC_FUNC(EggJanken_State_ResultPlayerWinner);
    ADD_PUBLIC_FUNC(EggJanken_State_HitShake);
    ADD_PUBLIC_FUNC(EggJanken_State_FinishedBeingHit);
    ADD_PUBLIC_FUNC(EggJanken_State_RaiseArms);
    ADD_PUBLIC_FUNC(EggJanken_State_SwingDropArms);
    ADD_PUBLIC_FUNC(EggJanken_State_WaitForArmAttackExtend);
    ADD_PUBLIC_FUNC(EggJanken_State_PrepareArmAttack);
    ADD_PUBLIC_FUNC(EggJanken_State_ArmAttack);
    ADD_PUBLIC_FUNC(EggJanken_State_FinishedArmAttack);
    ADD_PUBLIC_FUNC(EggJanken_State_ResultPlayerLoser);
    ADD_PUBLIC_FUNC(EggJanken_State_FlipOver);
    ADD_PUBLIC_FUNC(EggJanken_State_ExtendDropArms);
    ADD_PUBLIC_FUNC(EggJanken_State_DropTarget);
    ADD_PUBLIC_FUNC(EggJanken_State_Drop);
    ADD_PUBLIC_FUNC(EggJanken_State_DropArms);
    ADD_PUBLIC_FUNC(EggJanken_State_Dropped);
    ADD_PUBLIC_FUNC(EggJanken_State_RetractDropArms);
    ADD_PUBLIC_FUNC(EggJanken_State_RiseUp);
    ADD_PUBLIC_FUNC(EggJanken_State_FlipBackOver);
    ADD_PUBLIC_FUNC(EggJanken_Eyes_Setup);
    ADD_PUBLIC_FUNC(EggJanken_Eyes_ChangeSlots);
    ADD_PUBLIC_FUNC(EggJanken_Eyes_None);
    ADD_PUBLIC_FUNC(EggJanken_Arm_None);
    ADD_PUBLIC_FUNC(EggJanken_Arm_Idle);
    ADD_PUBLIC_FUNC(EggJanken_Arm_RetractArm);
    ADD_PUBLIC_FUNC(EggJanken_Arm_ExtendArm);
    ADD_PUBLIC_FUNC(EggJanken_Arm_StretchRetractArm);
    ADD_PUBLIC_FUNC(EggJanken_Arm_SwingArm);
    ADD_PUBLIC_FUNC(EggJanken_Arm_ArmAttack);
    ADD_PUBLIC_FUNC(EggJanken_Arm_Dropping);
    ADD_PUBLIC_FUNC(EggJanken_Draw_Closed);
    ADD_PUBLIC_FUNC(EggJanken_Draw_Active);
    ADD_PUBLIC_FUNC(EggJanken_Draw_Destroyed);

    // SPZ/EggTV
    ADD_PUBLIC_FUNC(EggTV_DrawScanlines);
    ADD_PUBLIC_FUNC(EggTV_DrawTV);

    // SPZ/FilmReel
    ADD_PUBLIC_FUNC(FilmReel_SpinLeft);
    ADD_PUBLIC_FUNC(FilmReel_SpinRight);

    // SPZ/Funnel
    ADD_PUBLIC_FUNC(Funnel_State_None);

    // SPZ/GreenScreen
    ADD_PUBLIC_FUNC(GreenScreen_DrawBG);
    ADD_PUBLIC_FUNC(GreenScreen_DrawBackgrounds);
    ADD_PUBLIC_FUNC(GreenScreen_DrawSprites);

    // SPZ/HeavyGunner
    ADD_PUBLIC_FUNC(HeavyGunner_SfxCheck_HeliProp);
    ADD_PUBLIC_FUNC(HeavyGunner_SfxCheck_HBHSurprise);
    ADD_PUBLIC_FUNC(HeavyGunner_SfxCheck_RocketBurn);
    ADD_PUBLIC_FUNC(HeavyGunner_HandleBGWrap);
    ADD_PUBLIC_FUNC(HeavyGunner_DestroyAllMissiles);
    ADD_PUBLIC_FUNC(HeavyGunner_Draw_Heli);
    ADD_PUBLIC_FUNC(HeavyGunner_Draw_EggRobo);
    ADD_PUBLIC_FUNC(HeavyGunner_Draw_Missile);
    ADD_PUBLIC_FUNC(HeavyGunner_Draw_Simple);
    ADD_PUBLIC_FUNC(HeavyGunner_Draw_FadeOut);
    ADD_PUBLIC_FUNC(HeavyGunner_StateManager_SetupArena);
    ADD_PUBLIC_FUNC(HeavyGunner_StateManager_HandleStageWrap);
    ADD_PUBLIC_FUNC(HeavyGunner_StateManager_HandlePathChange);
    ADD_PUBLIC_FUNC(HeavyGunner_Input_LockedP1);
    ADD_PUBLIC_FUNC(HeavyGunner_Input_LockedP2);
    ADD_PUBLIC_FUNC(HeavyGunner_Input_LockedP2_AI);
    ADD_PUBLIC_FUNC(HeavyGunner_StateEggRobo_Patrolling);
    ADD_PUBLIC_FUNC(HeavyGunner_StateEggRobo_FlyIn);
    ADD_PUBLIC_FUNC(HeavyGunner_StateEggRobo_ThrowGun);
    ADD_PUBLIC_FUNC(HeavyGunner_StateEggRobo_ThrownGun);
    ADD_PUBLIC_FUNC(HeavyGunner_StateGun_Thrown);
    ADD_PUBLIC_FUNC(HeavyGunner_StateGun_Grabbed);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_Launched);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_BlastOff);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_FindFloor);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_AttackPlayer);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_Malfunction);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_ReturnToSender);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_AttackRobo);
    ADD_PUBLIC_FUNC(HeavyGunner_StateMissile_AttackGunner);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_AwaitPlayer);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_WooshIn);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_FindFloor);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_HandleAttacks);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_ShotsFired);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_EscapeMissile);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_IncomingMissile);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_Exploding);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_ExplodeAndFall);
    ADD_PUBLIC_FUNC(HeavyGunner_StateEscapeHBH_Hover);
    ADD_PUBLIC_FUNC(HeavyGunner_StateEscapeHBH_FlyAway);
    ADD_PUBLIC_FUNC(HeavyGunner_StateHeli_FadeOutDestroy);

    // SPZ/LEDPanel
    ADD_PUBLIC_FUNC(LEDPanel_SetupActiveText);
    ADD_PUBLIC_FUNC(LEDPanel_SetupTextPos);
    ADD_PUBLIC_FUNC(LEDPanel_HandleCharacters);
    ADD_PUBLIC_FUNC(LEDPanel_StateText_Move);
    ADD_PUBLIC_FUNC(LEDPanel_StateText_Delay);
    ADD_PUBLIC_FUNC(LEDPanel_StateText_ChangeClipBounds);
    ADD_PUBLIC_FUNC(LEDPanel_StateText_WaitForSignal);

    // SPZ/Letterboard
    ADD_PUBLIC_FUNC(Letterboard_State_Controller);
    ADD_PUBLIC_FUNC(Letterboard_State_CheckPlayerSpin);
    ADD_PUBLIC_FUNC(Letterboard_State_Spun);

    // SPZ/LottoBall
    ADD_PUBLIC_FUNC(LottoBall_CheckOffScreen);
    ADD_PUBLIC_FUNC(LottoBall_State_FallIntoMachine);
    ADD_PUBLIC_FUNC(LottoBall_State_InMachine);
    ADD_PUBLIC_FUNC(LottoBall_State_Collected);
    ADD_PUBLIC_FUNC(LottoBall_State_CollectFall);
    ADD_PUBLIC_FUNC(LottoBall_State_SetupUIBall);
    ADD_PUBLIC_FUNC(LottoBall_State_EnterUIBall);
    ADD_PUBLIC_FUNC(LottoBall_State_ShowUIBall);

    // SPZ/LottoMachine
    ADD_PUBLIC_FUNC(LottoMachine_VSSwap_CheckBusy);
    ADD_PUBLIC_FUNC(LottoMachine_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(LottoMachine_CheckPlayerCollisions_Bottom);
    ADD_PUBLIC_FUNC(LottoMachine_HandleMotor);
    ADD_PUBLIC_FUNC(LottoMachine_SetupBalls);
    ADD_PUBLIC_FUNC(LottoMachine_SetupUIBalls);
    ADD_PUBLIC_FUNC(LottoMachine_GiveRings);
    ADD_PUBLIC_FUNC(LottoMachine_State_Startup);
    ADD_PUBLIC_FUNC(LottoMachine_State_HandleBallCollect);
    ADD_PUBLIC_FUNC(LottoMachine_State_CollectBall);
    ADD_PUBLIC_FUNC(LottoMachine_State_HandleBallCollected);
    ADD_PUBLIC_FUNC(LottoMachine_State_DropPlayers);
    ADD_PUBLIC_FUNC(LottoMachine_State_ReleasePlayers);
    ADD_PUBLIC_FUNC(LottoMachine_State_StopSpinning);

    // SPZ/LoveTester
    ADD_PUBLIC_FUNC(LoveTester_SetupHitboxes);
    ADD_PUBLIC_FUNC(LoveTester_SetupLightOffsets);
    ADD_PUBLIC_FUNC(LoveTester_DrawSprites);
    ADD_PUBLIC_FUNC(LoveTester_DrawTVDisplay);
    ADD_PUBLIC_FUNC(LoveTester_CheckPlayerCollisions_Solid);
    ADD_PUBLIC_FUNC(LoveTester_CheckPlayerCollisions_Entry);
    ADD_PUBLIC_FUNC(LoveTester_GiveScore);
    ADD_PUBLIC_FUNC(LoveTester_CreateHeartParticles);
    ADD_PUBLIC_FUNC(LoveTester_State_Init);
    ADD_PUBLIC_FUNC(LoveTester_State_WaitForActivated);
    ADD_PUBLIC_FUNC(LoveTester_State_SetupTopDisplay);
    ADD_PUBLIC_FUNC(LoveTester_State_SetupMatching);
    ADD_PUBLIC_FUNC(LoveTester_State_UnluckyMatch);
    ADD_PUBLIC_FUNC(LoveTester_State_GoodMatch);
    ADD_PUBLIC_FUNC(LoveTester_State_BadMatch);
    ADD_PUBLIC_FUNC(LoveTester_State_ReleasePlayers);
    ADD_PUBLIC_FUNC(LoveTester_State_HeartParticles);
    ADD_PUBLIC_FUNC(LoveTester_StateLights_FlashSlow);
    ADD_PUBLIC_FUNC(LoveTester_StateLights_FlashMed);
    ADD_PUBLIC_FUNC(LoveTester_StateLights_FlashFast);

    // SPZ/MicDrop
    ADD_PUBLIC_FUNC(MicDrop_DebugSpawn);
    ADD_PUBLIC_FUNC(MicDrop_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MicDrop_CheckOffScreen);
    ADD_PUBLIC_FUNC(MicDrop_State_Init);
    ADD_PUBLIC_FUNC(MicDrop_State_CheckForPlayer);
    ADD_PUBLIC_FUNC(MicDrop_State_DropDown);
    ADD_PUBLIC_FUNC(MicDrop_State_DropRecoil);
    ADD_PUBLIC_FUNC(MicDrop_State_Idle);
    ADD_PUBLIC_FUNC(MicDrop_State_Swinging);
    ADD_PUBLIC_FUNC(MicDrop_State_Electrify);

    // SPZ/PathInverter
    ADD_PUBLIC_FUNC(PathInverter_HandlePathSwitch);
    ADD_PUBLIC_FUNC(PathInverter_State_Horizontal);
    ADD_PUBLIC_FUNC(PathInverter_State_Vertical);

    // SPZ/PimPom
    ADD_PUBLIC_FUNC(PimPom_State_Single);
    ADD_PUBLIC_FUNC(PimPom_State_Horizontal);
    ADD_PUBLIC_FUNC(PimPom_State_Vertical);
    ADD_PUBLIC_FUNC(PimPom_Move_Fixed);
    ADD_PUBLIC_FUNC(PimPom_Move_Normal);
    ADD_PUBLIC_FUNC(PimPom_Move_Circle);
    ADD_PUBLIC_FUNC(PimPom_Move_Path);
    ADD_PUBLIC_FUNC(PimPom_Move_Track);

    // SPZ/PopcornKernel
    ADD_PUBLIC_FUNC(PopcornKernel_State_BounceAround);
    ADD_PUBLIC_FUNC(PopcornKernel_State_RiseUp);
    ADD_PUBLIC_FUNC(PopcornKernel_State_FallDown);

    // SPZ/PopcornMachine
    ADD_PUBLIC_FUNC(PopcornMachine_LinkPlayer);
    ADD_PUBLIC_FUNC(PopcornMachine_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(PopcornMachine_State_Idle);
    ADD_PUBLIC_FUNC(PopcornMachine_State_ShowDispenser);
    ADD_PUBLIC_FUNC(PopcornMachine_State_Shaking);
    ADD_PUBLIC_FUNC(PopcornMachine_State_HideDispenser);
    ADD_PUBLIC_FUNC(PopcornMachine_StateController_ReadyPlayer);
    ADD_PUBLIC_FUNC(PopcornMachine_StateController_RisePlayer);
    ADD_PUBLIC_FUNC(PopcornMachine_StateController_FirePlayer);
    ADD_PUBLIC_FUNC(PopcornMachine_StateController_HandleFinish);

    // SPZ/RockemSockem
    ADD_PUBLIC_FUNC(RockemSockem_HandleJointPositions);

    // SPZ/ShopWindow
    ADD_PUBLIC_FUNC(ShopWindow_State_Shard);
    ADD_PUBLIC_FUNC(ShopWindow_State_Shattered);
    ADD_PUBLIC_FUNC(ShopWindow_State_Silhouette);
    ADD_PUBLIC_FUNC(ShopWindow_Draw_Normal);
    ADD_PUBLIC_FUNC(ShopWindow_Draw_Shard);
    ADD_PUBLIC_FUNC(ShopWindow_Draw_Shattered);

    // SPZ/Shutterbug
    ADD_PUBLIC_FUNC(Shutterbug_DebugSpawn);
    ADD_PUBLIC_FUNC(Shutterbug_DebugDraw);
    ADD_PUBLIC_FUNC(Shutterbug_CheckOffScreen);
    ADD_PUBLIC_FUNC(Shutterbug_State_Init);
    ADD_PUBLIC_FUNC(Shutterbug_State_FlyAround);
    ADD_PUBLIC_FUNC(Shutterbug_State_ShakeFly);
    ADD_PUBLIC_FUNC(Shutterbug_State_FlyAway);
    ADD_PUBLIC_FUNC(Shutterbug_BounceX);
    ADD_PUBLIC_FUNC(Shutterbug_BounceY);
    ADD_PUBLIC_FUNC(Shutterbug_CheckFocus);
    ADD_PUBLIC_FUNC(Shutterbug_TryTakePicture);
    ADD_PUBLIC_FUNC(Shutterbug_HandleBodyAnim);

    // SPZ/SpinSign
    ADD_PUBLIC_FUNC(SpinSign_DebugSpawn);
    ADD_PUBLIC_FUNC(SpinSign_State_Spinning);
    ADD_PUBLIC_FUNC(SpinSign_State_SlowDown);
    ADD_PUBLIC_FUNC(SpinSign_Draw_SonicH);
    ADD_PUBLIC_FUNC(SpinSign_Draw_SonicV);
    ADD_PUBLIC_FUNC(SpinSign_Draw_ManiaH);
    ADD_PUBLIC_FUNC(SpinSign_Draw_ManiaV);

    // SPZ/SPZ1Intro
    ADD_PUBLIC_FUNC(SPZ1Intro_Cutscene_SetupAct);
    ADD_PUBLIC_FUNC(SPZ1Intro_Cutscene_ExitPipe);
    ADD_PUBLIC_FUNC(SPZ1Intro_Cutscene_BeginAct1);

    // SPZ/SPZ1Setup
    ADD_PUBLIC_FUNC(SPZ1Setup_StageFinish_EndAct1);

    // SPZ/SPZ2Outro
    ADD_PUBLIC_FUNC(SPZ2Outro_StartCutscene);
    ADD_PUBLIC_FUNC(SPZ2Outro_Cutscene_SetupFBZTV);
    ADD_PUBLIC_FUNC(SPZ2Outro_Cutscene_ExitStageRight);
    ADD_PUBLIC_FUNC(SPZ2Outro_DrawHook_PrepareWeatherTV);
    ADD_PUBLIC_FUNC(SPZ2Outro_Cutscene_AsSeenOnTV);
    ADD_PUBLIC_FUNC(SPZ2Outro_Cutscene_FBZFlyAway);

    // SPZ/SPZ2Setup
    ADD_PUBLIC_FUNC(SPZ2Setup_StageFinish_EndAct2);

    // SPZ/Tubinaut
    ADD_PUBLIC_FUNC(Tubinaut_DebugSpawn);
    ADD_PUBLIC_FUNC(Tubinaut_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Tubinaut_CheckAttacking);
    ADD_PUBLIC_FUNC(Tubinaut_OrbHit);
    ADD_PUBLIC_FUNC(Tubinaut_HandleRepel);
    ADD_PUBLIC_FUNC(Tubinaut_HandleOrbs);
    ADD_PUBLIC_FUNC(Tubinaut_CheckOffScreen);
    ADD_PUBLIC_FUNC(Tubinaut_State_Init);
    ADD_PUBLIC_FUNC(Tubinaut_State_Move);
    ADD_PUBLIC_FUNC(Tubinaut_Orb_Relax);
    ADD_PUBLIC_FUNC(Tubinaut_Orb_PrepareAttack);
    ADD_PUBLIC_FUNC(Tubinaut_Orb_Attack);
    ADD_PUBLIC_FUNC(Tubinaut_Orb_Cooldown);
    ADD_PUBLIC_FUNC(Tubinaut_Orb_BodyDeath);

    // SPZ/TVFlyingBattery
    ADD_PUBLIC_FUNC(TVFlyingBattery_DrawSection);
    ADD_PUBLIC_FUNC(TVFlyingBattery_DrawSprites);

    // SPZ/TVPole
    ADD_PUBLIC_FUNC(TVPole_State_CheckGrab);
    ADD_PUBLIC_FUNC(TVPole_State_ForceRelease);

    // SPZ/TVVan
    ADD_PUBLIC_FUNC(TVVan_HandleVanTilt);
    ADD_PUBLIC_FUNC(TVVan_Draw_Van_Low);
    ADD_PUBLIC_FUNC(TVVan_Draw_Van_High);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NE_SW);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NW_SE);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NW_SW);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NE_SE);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NE_NW);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_SE_SW);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_SW_N);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NW_S);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_SE_N);
    ADD_PUBLIC_FUNC(TVVan_Draw_Satellite_NE_S);
    ADD_PUBLIC_FUNC(TVVan_Draw_Radio);
    ADD_PUBLIC_FUNC(TVVan_Draw_ExitTV_Active);
    ADD_PUBLIC_FUNC(TVVan_Draw_ExitTV_Destroyed);
    ADD_PUBLIC_FUNC(TVVan_StateVan_Idle);
    ADD_PUBLIC_FUNC(TVVan_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TVVan_StateVan_BroadcastRadio);
    ADD_PUBLIC_FUNC(TVVan_StateVan_FinishedBroadcast);
    ADD_PUBLIC_FUNC(TVVan_StateRadio_StartBroadcast);
    ADD_PUBLIC_FUNC(TVVan_StateRadio_HandleMovement);
    ADD_PUBLIC_FUNC(TVVan_StateRadio_EnterSatellite1);
    ADD_PUBLIC_FUNC(TVVan_StateRadio_ExitSatellite2);
    ADD_PUBLIC_FUNC(TVVan_StateTV_Idle);
    ADD_PUBLIC_FUNC(TVVan_StateTV_Exploding);
    ADD_PUBLIC_FUNC(TVVan_StateTV_Destroyed);
    ADD_PUBLIC_FUNC(TVVan_StateRadio_EnterTV);

    // SPZ/WeatherMobile
    ADD_PUBLIC_FUNC(WeatherMobile_HandleAnimations);
    ADD_PUBLIC_FUNC(WeatherMobile_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(WeatherMobile_Hit);
    ADD_PUBLIC_FUNC(WeatherMobile_Explode);
    ADD_PUBLIC_FUNC(WeatherMobile_HandleDefeat);
    ADD_PUBLIC_FUNC(WeatherMobile_Draw_Eggman);
    ADD_PUBLIC_FUNC(WeatherMobile_Draw_Lights);
    ADD_PUBLIC_FUNC(WeatherMobile_Draw_Lightning);
    ADD_PUBLIC_FUNC(WeatherMobile_State_SetupArena);
    ADD_PUBLIC_FUNC(WeatherMobile_State_StartBoss);
    ADD_PUBLIC_FUNC(WeatherMobile_State_EnterEggman);
    ADD_PUBLIC_FUNC(WeatherMobile_State_HandleMovement);
    ADD_PUBLIC_FUNC(WeatherMobile_State_WindAttack);
    ADD_PUBLIC_FUNC(WeatherMobile_State_SunAttack);
    ADD_PUBLIC_FUNC(WeatherMobile_StateLights_EnterLights);
    ADD_PUBLIC_FUNC(WeatherMobile_StateLights_Shine);
    ADD_PUBLIC_FUNC(WeatherMobile_StateLights_DimAndDissapear);
    ADD_PUBLIC_FUNC(WeatherMobile_StateWindManager_SetupWind);
    ADD_PUBLIC_FUNC(WeatherMobile_StateWindManager_StrongWind);
    ADD_PUBLIC_FUNC(WeatherMobile_StateWindManager_FinishWind);
    ADD_PUBLIC_FUNC(WeatherMobile_State_Wind);
    ADD_PUBLIC_FUNC(WeatherMobile_State_RainManager);
    ADD_PUBLIC_FUNC(WeatherMobile_State_RainDrop);
    ADD_PUBLIC_FUNC(WeatherMobile_State_RainDropSplash);
    ADD_PUBLIC_FUNC(WeatherMobile_StateCloud_Idle);
    ADD_PUBLIC_FUNC(WeatherMobile_StateCloud_ShowTell);
    ADD_PUBLIC_FUNC(WeatherMobile_StateCloud_Strike);
    ADD_PUBLIC_FUNC(WeatherMobile_StateCloud_FadeAway);
    ADD_PUBLIC_FUNC(WeatherMobile_State_Defeated);
    ADD_PUBLIC_FUNC(WeatherMobile_State_DefeatFall);
    ADD_PUBLIC_FUNC(WeatherMobile_State_FleeRise);
    ADD_PUBLIC_FUNC(WeatherMobile_State_FleeAdjust);
    ADD_PUBLIC_FUNC(WeatherMobile_State_Flee);

    // SPZ/WeatherTV
    ADD_PUBLIC_FUNC(WeatherTV_ShutdownTV);
    ADD_PUBLIC_FUNC(WeatherTV_DrawTV);
    ADD_PUBLIC_FUNC(WeatherTV_DrawScanlines);
    ADD_PUBLIC_FUNC(WeatherTV_DrawTVBackground);
    ADD_PUBLIC_FUNC(WeatherTV_DrawNumbers);
    ADD_PUBLIC_FUNC(WeatherTV_CheckEggmanBusy);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_Off);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_TurningOn);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_EnterEggTVLogo);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_ExitEggTVLogo);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_ShowWeatherChannel);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_ShowCluckoid);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_SpinningCard);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_ShowCard);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_LightAttack);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_SuperHot);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_WindAttack);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_RainAttack);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_Buzzing);
    ADD_PUBLIC_FUNC(WeatherTV_Draw_Outro);
    ADD_PUBLIC_FUNC(WeatherTV_State_TurnOn);
    ADD_PUBLIC_FUNC(WeatherTV_State_FinishTurningOn);
    ADD_PUBLIC_FUNC(WeatherTV_State_EnterEggTVLogo);
    ADD_PUBLIC_FUNC(WeatherTV_State_ExitEggTVLogo);
    ADD_PUBLIC_FUNC(WeatherTV_State_ShowWeatherChannel);
    ADD_PUBLIC_FUNC(WeatherTV_State_ShowCluckoid);
    ADD_PUBLIC_FUNC(WeatherTV_State_ChooseNextAttack);
    ADD_PUBLIC_FUNC(WeatherTV_State_CardSpinning);
    ADD_PUBLIC_FUNC(WeatherTV_State_ShowCard);
    ADD_PUBLIC_FUNC(WeatherTV_State_BeginLightAttack);
    ADD_PUBLIC_FUNC(WeatherTV_State_HeatingUp);
    ADD_PUBLIC_FUNC(WeatherTV_State_BoilingHot);
    ADD_PUBLIC_FUNC(WeatherTV_State_CoolDown);
    ADD_PUBLIC_FUNC(WeatherTV_State_BeginWindAttack);
    ADD_PUBLIC_FUNC(WeatherTV_State_WindyWeather);
    ADD_PUBLIC_FUNC(WeatherTV_State_FinishWindAttack);
    ADD_PUBLIC_FUNC(WeatherTV_State_BeginRainAttack);
    ADD_PUBLIC_FUNC(WeatherTV_State_StartRaining);
    ADD_PUBLIC_FUNC(WeatherTV_State_StopRaining);
    ADD_PUBLIC_FUNC(WeatherTV_State_Outro);

    // SSZ/Beanstalk
    ADD_PUBLIC_FUNC(Beanstalk_GetNextNodeDistance);
    ADD_PUBLIC_FUNC(Beanstalk_GetRemainingDistance);
    ADD_PUBLIC_FUNC(Beanstalk_GetBezierInc);
    ADD_PUBLIC_FUNC(Beanstalk_DrawNodes);
    ADD_PUBLIC_FUNC(Beanstalk_DrawCreationNode);
    ADD_PUBLIC_FUNC(Beanstalk_HandleNodeMovement);
    ADD_PUBLIC_FUNC(Beanstalk_HandleNodeAppear);
    ADD_PUBLIC_FUNC(Beanstalk_CheckPlayerCollisions_Platform);
    ADD_PUBLIC_FUNC(Beanstalk_CheckPlayerCollisions_Chomper);
    ADD_PUBLIC_FUNC(Beanstalk_State_Init);
    ADD_PUBLIC_FUNC(Beanstalk_StateDirt_WaitForStart);
    ADD_PUBLIC_FUNC(Beanstalk_StateDirt_GrowthDelay);
    ADD_PUBLIC_FUNC(Beanstalk_StateDirt_Grow);
    ADD_PUBLIC_FUNC(Beanstalk_State_Node);
    ADD_PUBLIC_FUNC(Beanstalk_State_Platform);
    ADD_PUBLIC_FUNC(Beanstalk_State_Chomper);

    // SSZ/BouncePlant
    ADD_PUBLIC_FUNC(BoucePlant_SetupNodePositions);
    ADD_PUBLIC_FUNC(BoucePlant_GetNodeStandY);
    ADD_PUBLIC_FUNC(BoucePlant_GetNodeY);

    // SSZ/Constellation
    ADD_PUBLIC_FUNC(Constellation_SetupInfo);

    // SSZ/Dango
    ADD_PUBLIC_FUNC(Dango_DebugSpawn);
    ADD_PUBLIC_FUNC(Dango_CheckOffScreen);
    ADD_PUBLIC_FUNC(Dango_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Dango_HandleMovement);
    ADD_PUBLIC_FUNC(Dango_State_Init);
    ADD_PUBLIC_FUNC(Dango_State_Walking);
    ADD_PUBLIC_FUNC(Dango_State_Turning);
    ADD_PUBLIC_FUNC(Dango_State_Falling_Uncurled);
    ADD_PUBLIC_FUNC(Dango_State_Curling);
    ADD_PUBLIC_FUNC(Dango_State_Rolling);
    ADD_PUBLIC_FUNC(Dango_State_Falling_Curled);
    ADD_PUBLIC_FUNC(Dango_State_Uncurling);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Dango_StateTaunt_Setup);
    ADD_PUBLIC_FUNC(Dango_StateTaunt_RollIn);
    ADD_PUBLIC_FUNC(Dango_StateTaunt_KnockedRuby);
    ADD_PUBLIC_FUNC(Dango_StateTaunt_Taunting);
    ADD_PUBLIC_FUNC(Dango_StateTaunt_Turning);
    ADD_PUBLIC_FUNC(Dango_StateTaunt_RollOut);
#endif

    // SSZ/Fireflies
    ADD_PUBLIC_FUNC(Fireflies_State_Spawner);
    ADD_PUBLIC_FUNC(Fireflies_State_Firefly);

    // SSZ/Firework
    ADD_PUBLIC_FUNC(Firework_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Firework_HandlePlayerTimers);
    ADD_PUBLIC_FUNC(Firework_RemovePlayers);
    ADD_PUBLIC_FUNC(Firework_HandlePlayerRemoval);
    ADD_PUBLIC_FUNC(Firework_HandlePlayerJump);
    ADD_PUBLIC_FUNC(Firework_HandlePlayerControl);
    ADD_PUBLIC_FUNC(Firework_HandleMoveDir);
    ADD_PUBLIC_FUNC(Firework_HandleDebrisSpawn);
    ADD_PUBLIC_FUNC(Firework_HandleTileCollisions);
    ADD_PUBLIC_FUNC(Firework_HandleRideEnd);
    ADD_PUBLIC_FUNC(Firework_CheckOffScreen);
    ADD_PUBLIC_FUNC(Firework_HandleSparkAnimations);
    ADD_PUBLIC_FUNC(Firework_State_Init);
    ADD_PUBLIC_FUNC(Firework_State_AwaitPlayerRide);
    ADD_PUBLIC_FUNC(Firework_State_PlayerGrabDelay);
    ADD_PUBLIC_FUNC(Firework_State_Riding);
    ADD_PUBLIC_FUNC(Firework_State_Explode);
    ADD_PUBLIC_FUNC(Firework_State_ResetOffScreen);
    ADD_PUBLIC_FUNC(Firework_State_SparkSetup);
    ADD_PUBLIC_FUNC(Firework_State_Spark);

    // SSZ/FlowerPod
    ADD_PUBLIC_FUNC(FlowerPod_SpawnSeeds);
    ADD_PUBLIC_FUNC(FlowerPod_State_Pod);
    ADD_PUBLIC_FUNC(FlowerPod_State_Exploding);
    ADD_PUBLIC_FUNC(FlowerPod_State_Destroyed);
    ADD_PUBLIC_FUNC(FlowerPod_State_SeedFall);
    ADD_PUBLIC_FUNC(FlowerPod_State_SpawnBeanstalk);
    ADD_PUBLIC_FUNC(FlowerPod_State_FlowerGrow);
    ADD_PUBLIC_FUNC(FlowerPod_State_Flower);

    // SSZ/GigaMetal
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(GigaMetal_Draw_Cover);
    ADD_PUBLIC_FUNC(GigaMetal_Draw_Head);
    ADD_PUBLIC_FUNC(GigaMetal_Draw_Shoulder);
    ADD_PUBLIC_FUNC(GigaMetal_Draw_Arm);
    ADD_PUBLIC_FUNC(GigaMetal_Draw_LaserEdge);
    ADD_PUBLIC_FUNC(GigaMetal_Draw_Shard);
    ADD_PUBLIC_FUNC(GigaMetal_HandleCameraMovement);
    ADD_PUBLIC_FUNC(GigaMetal_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(GigaMetal_Hit);
    ADD_PUBLIC_FUNC(GigaMetal_StateBody_AwaitPlayer);
    ADD_PUBLIC_FUNC(GigaMetal_StateBody_SetupComponents);
    ADD_PUBLIC_FUNC(GigaMetal_StateBody_Transformed);
    ADD_PUBLIC_FUNC(GigaMetal_StateBody_Roar);
    ADD_PUBLIC_FUNC(GigaMetal_StateBody_Marching);
    ADD_PUBLIC_FUNC(GigaMetal_StateBody_Destroyed);
    ADD_PUBLIC_FUNC(GigaMetal_StateShoulder_Impact);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_Impact);
    ADD_PUBLIC_FUNC(GigaMetal_State_Laser);
    ADD_PUBLIC_FUNC(GigaMetal_StateLaser_Finish);
    ADD_PUBLIC_FUNC(GigaMetal_State_Shard);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_PrepareRoar);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_Roar);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_FinishRoar);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_PrepareLaser);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_ChargeLaser);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_FiringLaser);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_TargetingPlayer);
    ADD_PUBLIC_FUNC(GigaMetal_StateHead_ReleasingBombs);
    ADD_PUBLIC_FUNC(GigaMetal_HandleArmCollisions);
    ADD_PUBLIC_FUNC(GigaMetal_StateArm_Idle_Front);
    ADD_PUBLIC_FUNC(GigaMetal_StateArm_Idle_Behind);
    ADD_PUBLIC_FUNC(GigaMetal_StateArm_Marching);
#endif

    // SSZ/HiLoSign
    ADD_PUBLIC_FUNC(HiLoSign_DebugSpawn);
    ADD_PUBLIC_FUNC(HiLoSign_State_Spinning);
    ADD_PUBLIC_FUNC(HiLoSign_State_FinishSpinAdjust);
    ADD_PUBLIC_FUNC(HiLoSign_Draw_Horizontal);
    ADD_PUBLIC_FUNC(HiLoSign_Draw_Vertical);

    // SSZ/Hotaru
    ADD_PUBLIC_FUNC(Hotaru_DebugSpawn);
    ADD_PUBLIC_FUNC(Hotaru_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Hotaru_CheckOffScreen);
    ADD_PUBLIC_FUNC(Hotaru_State_Init);
    ADD_PUBLIC_FUNC(Hotaru_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(Hotaru_State_FoundPlayer);
    ADD_PUBLIC_FUNC(Hotaru_State_FlyOnScreen);
    ADD_PUBLIC_FUNC(Hotaru_State_AttachedToScreen);
    ADD_PUBLIC_FUNC(Hotaru_State_Charging);
    ADD_PUBLIC_FUNC(Hotaru_State_Attacking);
    ADD_PUBLIC_FUNC(Hotaru_State_FinishedAttacking);

    // SSZ/HotaruHiWatt
    ADD_PUBLIC_FUNC(HotaruHiWatt_SfxCheck_Zap);
    ADD_PUBLIC_FUNC(HotaruHiWatt_SfxCheck_HHWLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_SfxUpdate_HHWLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Hit);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Explode);
    ADD_PUBLIC_FUNC(HotaruHiWatt_CheckPlayerCollisions_Hotaru);
    ADD_PUBLIC_FUNC(HotaruHiWatt_CheckPlayerCollisions_Boss);
    ADD_PUBLIC_FUNC(HotaruHiWatt_CheckPlayerCollisions_MiniLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_CheckPlayerCollisions_BossLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_Hotaru);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_HotaruAttacking);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_Boss);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_BossLaserCharging);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_BossLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_FormingHHW);
    ADD_PUBLIC_FUNC(HotaruHiWatt_Draw_FlashAppear);
    ADD_PUBLIC_FUNC(HotaruHiWatt_SpawnPairHotarus);
    ADD_PUBLIC_FUNC(HotaruHiWatt_SetupHHWReappear);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_SetupArena);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_AwaitPlayer);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_Appear);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_FlyUp);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaru_MoveToTarget);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_FlashFadeOut);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_HiddenDimScreen);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_HiddenUndimScreen);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaru_DimScreen);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaru_Charging);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaru_Attacking);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaru_FinishedAttack);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaru_EndAttackSequence);
    ADD_PUBLIC_FUNC(HotaruHiWatt_State_MiniLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_State_MiniLaserStrike);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaruPair_DimScreen);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaruPair_PrepareAttack);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaruPair_Charging);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaruPair_AttackDelay);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaruPair_AttackMovingDown);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateHotaruPair_AttackMovingUp);
    ADD_PUBLIC_FUNC(HotaruHiWatt_State_SparkyOrb);
    ADD_PUBLIC_FUNC(HotaruHiWatt_State_SparkyOrb_FadeOut);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_DimScreen);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_CreateSmallHHWs);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_FormingHHW);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_FlashAppear);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_LaserAttackDelay);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_ChargingLaser);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_LaserAttack_Right);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_LaserAttack_Left);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_FinishedLaserAttack);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_Destroyed);
    ADD_PUBLIC_FUNC(HotaruHiWatt_StateBoss_Finished);

    // SSZ/HotaruMKII
    ADD_PUBLIC_FUNC(HotaruMKII_DebugSpawn);
    ADD_PUBLIC_FUNC(HotaruMKII_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(HotaruMKII_CheckOffScreen);
    ADD_PUBLIC_FUNC(HotaruMKII_HandleDistances);
    ADD_PUBLIC_FUNC(HotaruMKII_State_Init);
    ADD_PUBLIC_FUNC(HotaruMKII_State_CheckPlayerInRange);
    ADD_PUBLIC_FUNC(HotaruMKII_State_FlyAway);
    ADD_PUBLIC_FUNC(HotaruMKII_State_FlyOnScreen);
    ADD_PUBLIC_FUNC(HotaruMKII_State_AttackDelay);
    ADD_PUBLIC_FUNC(HotaruMKII_State_Charging);
    ADD_PUBLIC_FUNC(HotaruMKII_State_LaserAttack);
    ADD_PUBLIC_FUNC(HotaruMKII_State_Flash);
    ADD_PUBLIC_FUNC(HotaruMKII_State_Laser);
    ADD_PUBLIC_FUNC(HotaruMKII_State_LaserStrike);

    // SSZ/Kabasira
    ADD_PUBLIC_FUNC(Kabasira_DebugSpawn);
    ADD_PUBLIC_FUNC(Kabasira_HandleAnimations);
    ADD_PUBLIC_FUNC(Kabasira_DrawSegment);
    ADD_PUBLIC_FUNC(Kabasira_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Kabasira_CheckOffScreen);
    ADD_PUBLIC_FUNC(Kabasira_State_Init);
    ADD_PUBLIC_FUNC(Kabasira_State_Moving);
    ADD_PUBLIC_FUNC(Kabasira_State_LaunchedAttack);

    // SSZ/Kanabun
    ADD_PUBLIC_FUNC(Kanabun_DebugSpawn);
    ADD_PUBLIC_FUNC(Kanabun_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(Kanabun_CheckOffScreen);
    ADD_PUBLIC_FUNC(Kanabun_HandleMovement);
    ADD_PUBLIC_FUNC(Kanabun_State_Init);
    ADD_PUBLIC_FUNC(Kanabun_State_Moving);
    ADD_PUBLIC_FUNC(Kanabun_State_Turning);

    // SSZ/MetalSonic
    ADD_PUBLIC_FUNC(MetalSonic_HandleStageWrap);
    ADD_PUBLIC_FUNC(MetalSonic_ProcessBGParallax);
    ADD_PUBLIC_FUNC(MetalSonic_HandleVelocity);
    ADD_PUBLIC_FUNC(MetalSonic_HandleAnimDir);
#if !MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MetalSonic_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MetalSonic_Hit);
    ADD_PUBLIC_FUNC(MetalSonic_Explode);
#endif
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupArena);
    ADD_PUBLIC_FUNC(MetalSonic_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(MetalSonic_State_WaitForHologram);
    ADD_PUBLIC_FUNC(MetalSonic_State_Appear);
    ADD_PUBLIC_FUNC(MetalSonic_State_Land);
    ADD_PUBLIC_FUNC(MetalSonic_State_Taunt);
    ADD_PUBLIC_FUNC(MetalSonic_State_GetReady);
    ADD_PUBLIC_FUNC(MetalSonic_State_Ready);
    ADD_PUBLIC_FUNC(MetalSonic_State_Start);
    ADD_PUBLIC_FUNC(MetalSonic_State_EnterHoverMode);
    ADD_PUBLIC_FUNC(MetalSonic_State_Hovering);
    ADD_PUBLIC_FUNC(MetalSonic_State_PrepareAttack);
    ADD_PUBLIC_FUNC(MetalSonic_State_StartAttack);
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupBallAttack_Phase1);
    ADD_PUBLIC_FUNC(MetalSonic_State_BallAttack_Phase1);
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupElectricAttack_Phase1);
    ADD_PUBLIC_FUNC(MetalSonic_State_ElectricAttack_Phase1);
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupDashAttack_Phase1);
    ADD_PUBLIC_FUNC(MetalSonic_State_DashAttack_Phase1);
    ADD_PUBLIC_FUNC(MetalSonic_State_EnterPanel);
    ADD_PUBLIC_FUNC(MetalSonic_State_StartPanelSequence);
    ADD_PUBLIC_FUNC(MetalSonic_HandlePanelAttack);
    ADD_PUBLIC_FUNC(MetalSonic_State_OpenFactoryDoor);
    ADD_PUBLIC_FUNC(MetalSonic_State_HandleSilverSonics);
    ADD_PUBLIC_FUNC(MetalSonic_State_PanelExplosion);
    ADD_PUBLIC_FUNC(MetalSonic_State_ExitFactory);
    ADD_PUBLIC_FUNC(MetalSonic_State_PrepareFinalChase);
#if MANIA_USE_PLUS // Phase 3 (VS GigaMetal)
    ADD_PUBLIC_FUNC(MetalSonic_State_WaitForRuby);
    ADD_PUBLIC_FUNC(MetalSonic_State_ObtainRuby);
    ADD_PUBLIC_FUNC(MetalSonic_State_Transform);
    ADD_PUBLIC_FUNC(MetalSonic_State_Defeated);
#else // Phase 3 (VS Metal Sonic)
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupSpikeWall);
    ADD_PUBLIC_FUNC(MetalSonic_State_FlyToSpikeWall);
    ADD_PUBLIC_FUNC(MetalSonic_State_LandNearSpikeWall);
    ADD_PUBLIC_FUNC(MetalSonic_State_Taunt_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_Ready_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_StartSpikeWallMovement);
    ADD_PUBLIC_FUNC(MetalSonic_State_AccelerateSpikeWall);
    ADD_PUBLIC_FUNC(MetalSonic_State_Hover_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupBallAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_BallAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_FinishAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_SetupElectricAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_StartElectricAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_ElectricAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_DashAttack_Phase2);
    ADD_PUBLIC_FUNC(MetalSonic_State_Explode);
    ADD_PUBLIC_FUNC(MetalSonic_State_Defeated);
    ADD_PUBLIC_FUNC(MetalSonic_State_Finish);
    ADD_PUBLIC_FUNC(MetalSonic_State_None);
    ADD_PUBLIC_FUNC(MetalSonic_StateWall_Fall);
    ADD_PUBLIC_FUNC(MetalSonic_StateWall_Move);
#endif

    // SSZ/MSBomb
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(MSBomb_State_EnterBomb);
    ADD_PUBLIC_FUNC(MSBomb_State_SilverSonicExplode);
    ADD_PUBLIC_FUNC(MSBomb_State_Bouncing);
    ADD_PUBLIC_FUNC(MSBomb_State_Projectile);
#endif

    // SSZ/MSFactory
    ADD_PUBLIC_FUNC(MSFactory_State_SetupFactory);
    ADD_PUBLIC_FUNC(MSFactory_State_SetupMetalSonic);
    ADD_PUBLIC_FUNC(MSFactory_State_OpeningDoor);
    ADD_PUBLIC_FUNC(MSFactory_State_CreateSilverSonic);
    ADD_PUBLIC_FUNC(MSFactory_State_CloseDoor);

    // SSZ/MSHologram
    ADD_PUBLIC_FUNC(MSHologram_State_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MSHologram_State_Explode);
    ADD_PUBLIC_FUNC(MSHologram_State_Destroyed);

    // SSZ/MSOrb
    ADD_PUBLIC_FUNC(MSOrb_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MSOrb_State_Orb);
    ADD_PUBLIC_FUNC(MSOrb_Draw_Orb);

    // SSZ/MSPanel
    ADD_PUBLIC_FUNC(MSPanel_State_SetupPanel);
    ADD_PUBLIC_FUNC(MSPanel_State_Active);
    ADD_PUBLIC_FUNC(MSPanel_State_Explode);
    ADD_PUBLIC_FUNC(MSPanel_State_Rumbling);

    // SSZ/RTeleporter
    ADD_PUBLIC_FUNC(RTeleporter_State_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(RTeleporter_State_Destroyed);
    ADD_PUBLIC_FUNC(RTeleporter_Draw_Active);
    ADD_PUBLIC_FUNC(RTeleporter_Draw_Exploding);
    ADD_PUBLIC_FUNC(RTeleporter_Draw_Destroyed);

    // SSZ/SDashWheel
    ADD_PUBLIC_FUNC(SDashWheel_SfxCheck_MGZDoor);
    ADD_PUBLIC_FUNC(SDashWheel_SfxUpdate_MGZDoor);

    // SSZ/SilverSonic
    ADD_PUBLIC_FUNC(SilverSonic_HandleNextAttack);
    ADD_PUBLIC_FUNC(SilverSonic_CheckPlayerCollisions_Badnik);
    ADD_PUBLIC_FUNC(SilverSonic_CheckPlayerCollisions_Ball);
    ADD_PUBLIC_FUNC(SilverSonic_CheckPlayerCollisions_Arm);
    ADD_PUBLIC_FUNC(SilverSonic_State_Appear);
    ADD_PUBLIC_FUNC(SilverSonic_State_FinishedAttack);
    ADD_PUBLIC_FUNC(SilverSonic_State_ArmAttack);
    ADD_PUBLIC_FUNC(SilverSonic_State_Crouch);
    ADD_PUBLIC_FUNC(SilverSonic_State_Spindash);
    ADD_PUBLIC_FUNC(SilverSonic_State_Roll);
    ADD_PUBLIC_FUNC(SilverSonic_State_RollJump);
    ADD_PUBLIC_FUNC(SilverSonic_State_RollRebound);
    ADD_PUBLIC_FUNC(SilverSonic_State_BoostReady);
    ADD_PUBLIC_FUNC(SilverSonic_State_Boost_Air);
    ADD_PUBLIC_FUNC(SilverSonic_State_Boost_Ground);
    ADD_PUBLIC_FUNC(SilverSonic_State_FinishedBoost);
    ADD_PUBLIC_FUNC(SilverSonic_State_Explode);

    // SSZ/SpikeFlail
    ADD_PUBLIC_FUNC(SpikeFlail_SetupHitbox);
    ADD_PUBLIC_FUNC(SpikeFlail_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SpikeFlail_GetScale);

    // SSZ/SSZ1Intro
    ADD_PUBLIC_FUNC(SSZ1Intro_HandleRubyHover);
    ADD_PUBLIC_FUNC(SSZ1Intro_Cutscene_FinishRubyWarp);
    ADD_PUBLIC_FUNC(SSZ1Intro_Cutscene_HandeLanding);
    ADD_PUBLIC_FUNC(SSZ1Intro_Cutscene_BeginAct1);

    // SSZ/SSZ1Outro
    ADD_PUBLIC_FUNC(SSZ1Outro_Cutscene_TimeWarpRunway);
    ADD_PUBLIC_FUNC(SSZ1Outro_Cutscene_TimeWarp);
    ADD_PUBLIC_FUNC(SSZ1Outro_DestroyHotaru);
    ADD_PUBLIC_FUNC(SSZ1Outro_Trigger_DestroyHotarus);

    // SSZ/SSZ1Setup
    ADD_PUBLIC_FUNC(SSZ1Setup_StageFinish_EndAct1);

#if MANIA_USE_PLUS
    // SSZ/SSZ2Setup
    ADD_PUBLIC_FUNC(SSZ2Setup_StageFinish_EndAct2);
    ADD_PUBLIC_FUNC(SSZ2Setup_DrawHook_PrepareDrawingFX);
#endif
    ADD_PUBLIC_FUNC(SSZ2Setup_Scanline_BGTower);
    ADD_PUBLIC_FUNC(SSZ2Setup_Trigger_DestroyHotaruMKII);
    ADD_PUBLIC_FUNC(SSZ2Setup_Trigger_AwardAchievement);
    ADD_PUBLIC_FUNC(SSZ2Setup_Trigger_SSZ2BTransition);

#if MANIA_USE_PLUS
    // SSZ/SSZ3Cutscene
    ADD_PUBLIC_FUNC(SSZ3Cutscene_HandleRubyFX);
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneIntro_EnterStageLeft);
#endif
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneIntro_PlayerRunLeft);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneOutro_SetupOutro);
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneOutro_FollowRuby);
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneOutro_EnterRuby);
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneOutro_RubyActivate);
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneOutro_RubyWarp);
    ADD_PUBLIC_FUNC(SSZ3Cutscene_CutsceneOutro_LoadHCZ1);
#endif

    // SSZ/SSZEggman
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(SSZEggman_State_Init);
    ADD_PUBLIC_FUNC(SSZEggman_State_HoldingRuby);
    ADD_PUBLIC_FUNC(SSZEggman_State_ThrownRuby);
    ADD_PUBLIC_FUNC(SSZEggman_State_WatchMetalTransform);
    ADD_PUBLIC_FUNC(SSZEggman_State_FlyAway);
#endif

    // SSZ/SSZSpikeBall
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_AwaitInterval);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_H);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_V);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_H_Launch);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_V_Launch);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_Ball_MoveOut);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_Ball_Stopped);
    ADD_PUBLIC_FUNC(SSZSpikeBall_State_Ball_MoveBack);

    // SSZ/TimePost
    ADD_PUBLIC_FUNC(TimePost_Spin);
    ADD_PUBLIC_FUNC(TimePost_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TimePost_Particle_TimeSparkle);
    ADD_PUBLIC_FUNC(TimePost_HandleTimeSparkles);
    ADD_PUBLIC_FUNC(TimePost_State_Init);
    ADD_PUBLIC_FUNC(TimePost_State_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TimePost_State_Spinning);
    ADD_PUBLIC_FUNC(TimePost_State_FinishedSpin);

    // SSZ/TimeTravelSetup
    ADD_PUBLIC_FUNC(TimeTravelSetup_Particle_TimeSparkle);

    // SSZ/TTCutscene
    ADD_PUBLIC_FUNC(TTCutscene_StartCutscene);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TTCutscene_Cutscene_SkipCB);
    ADD_PUBLIC_FUNC(TTCutscene_Cutscene_Setup);
#endif
    ADD_PUBLIC_FUNC(TTCutscene_Cutscene_FlyIn);
    ADD_PUBLIC_FUNC(TTCutscene_Cutscene_Wait);
    ADD_PUBLIC_FUNC(TTCutscene_Cutscene_FlyOut);
    ADD_PUBLIC_FUNC(TTCutscene_Cutscene_NextScene);

    // SSZ/UncurlPlant
    ADD_PUBLIC_FUNC(UncurlPlant_CalculateDrawPositions);
    ADD_PUBLIC_FUNC(UncurlPlant_CalculatePositions);

    // SSZ/YoyoPulley
    ADD_PUBLIC_FUNC(YoyoPulley_UpdateHandlePos);
    ADD_PUBLIC_FUNC(YoyoPulley_DrawSprites);
    ADD_PUBLIC_FUNC(YoyoPulley_GetLength);

    // Summary/Summary
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(Summary_State_SetupText);
    ADD_PUBLIC_FUNC(Summary_State_FadeIn);
    ADD_PUBLIC_FUNC(Summary_State_Wait);
    ADD_PUBLIC_FUNC(Summary_State_FadeOut);
    ADD_PUBLIC_FUNC(Summary_SetTextString);
    ADD_PUBLIC_FUNC(Summary_GetPlayTime);
    ADD_PUBLIC_FUNC(Summary_LoadTimes);
#endif

    // Title/TitleBG
    ADD_PUBLIC_FUNC(TitleBG_SetupFX);
    ADD_PUBLIC_FUNC(TitleBG_Scanline_Clouds);
    ADD_PUBLIC_FUNC(TitleBG_Scanline_Island);

    // Title/TitleEggman
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TitleEggman_State_Dust);
    ADD_PUBLIC_FUNC(TitleEggman_State_Wait);
    ADD_PUBLIC_FUNC(TitleEggman_State_Move);
#endif

    // Title/TitleLogo
    ADD_PUBLIC_FUNC(TitleLogo_SetupPressStart);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TitleLogo_State_Ribbon);
    ADD_PUBLIC_FUNC(TitleLogo_State_PressButton);
    ADD_PUBLIC_FUNC(TitleLogo_State_HandleSetup);
    ADD_PUBLIC_FUNC(TitleLogo_State_PlusLogo);
    ADD_PUBLIC_FUNC(TitleLogo_State_PlusShine);
#endif

#if MANIA_USE_PLUS
    // Title/TitleSetup
    ADD_PUBLIC_FUNC(TitleSetup_HandleCheatInputs);
    ADD_PUBLIC_FUNC(TitleSetup_CheckCheatCode);
    ADD_PUBLIC_FUNC(TitleSetup_VideoSkipCB);
#endif
    ADD_PUBLIC_FUNC(TitleSetup_State_Wait);
    ADD_PUBLIC_FUNC(TitleSetup_State_AnimateUntilFlash);
    ADD_PUBLIC_FUNC(TitleSetup_State_FlashIn);
    ADD_PUBLIC_FUNC(TitleSetup_State_WaitForSonic);
    ADD_PUBLIC_FUNC(TitleSetup_State_SetupLogo);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TitleSetup_State_SetupPlusLogo);
    ADD_PUBLIC_FUNC(TitleSetup_State_WaitForEnter);
#endif
    ADD_PUBLIC_FUNC(TitleSetup_State_FadeToMenu);
    ADD_PUBLIC_FUNC(TitleSetup_State_FadeToVideo);
    ADD_PUBLIC_FUNC(TitleSetup_Draw_FadeBlack);
    ADD_PUBLIC_FUNC(TitleSetup_Draw_DrawRing);
    ADD_PUBLIC_FUNC(TitleSetup_Draw_Flash);

    // TMZ/BallHog
    ADD_PUBLIC_FUNC(BallHog_DebugSpawn);
    ADD_PUBLIC_FUNC(BallHog_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(BallHog_CheckOffScreen);
    ADD_PUBLIC_FUNC(BallHog_State_Init);
    ADD_PUBLIC_FUNC(BallHog_State_Idle);
    ADD_PUBLIC_FUNC(BallHog_State_Jump);
    ADD_PUBLIC_FUNC(BallHog_State_Land);
    ADD_PUBLIC_FUNC(BallHog_State_DropBomb);
    ADD_PUBLIC_FUNC(BallHog_State_Bomb);

    // TMZ/CrashTest
    ADD_PUBLIC_FUNC(CrashTest_SetupHitboxes);
    ADD_PUBLIC_FUNC(CrashTest_CheckOffScreen);
    ADD_PUBLIC_FUNC(CrashTest_HandlePlayerCrash);
    ADD_PUBLIC_FUNC(CrashTest_CheckPlayerCollisionsSolid);
    ADD_PUBLIC_FUNC(CrashTest_CheckPlayerRide);
    ADD_PUBLIC_FUNC(CrashTest_CheckPlayerJump);
    ADD_PUBLIC_FUNC(CrashTest_HandleLightAnims);
    ADD_PUBLIC_FUNC(CrashTest_State_Init);
    ADD_PUBLIC_FUNC(CrashTest_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(CrashTest_State_Rev);
    ADD_PUBLIC_FUNC(CrashTest_State_Move);
    ADD_PUBLIC_FUNC(CrashTest_State_Crashed);

    // TMZ/CrimsonEye
    ADD_PUBLIC_FUNC(CrimsonEye_Explode);
    ADD_PUBLIC_FUNC(CrimsonEye_Hit);
    ADD_PUBLIC_FUNC(CrimsonEye_SetupBG2Layer);
    ADD_PUBLIC_FUNC(CrimsonEye_DrawHook_DisableFGSilhouette);
    ADD_PUBLIC_FUNC(CrimsonEye_DrawHook_EnableFGSilhouette);
    ADD_PUBLIC_FUNC(CrimsonEye_SetArrowDir);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_SetupArena);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_AwaitPlayer);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_StartFight);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_CoreActive);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_HandleElevator);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_Explode);
    ADD_PUBLIC_FUNC(CrimsonEye_StateContainer_MoveElevatorToTMZ2Entry);
    ADD_PUBLIC_FUNC(CrimsonEye_Draw_Container);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_EnterBalls);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_SpinningBalls);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_LowerBalls);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_LoweredBalls);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_ExtendBallAttack);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_FinishExtendAttack);
    ADD_PUBLIC_FUNC(CrimsonEye_StateEye_RaiseBalls);
    ADD_PUBLIC_FUNC(CrimsonEye_ShootShot);
    ADD_PUBLIC_FUNC(CrimsonEye_SetupEyePositions);
    ADD_PUBLIC_FUNC(CrimsonEye_DestroyBall);
    ADD_PUBLIC_FUNC(CrimsonEye_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_ContainerActive);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_BreakOut);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_BrokenOut);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_Hovering);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_ImpactLift);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_Explode);
    ADD_PUBLIC_FUNC(CrimsonEye_StateCore_SpawnSignPost);
    ADD_PUBLIC_FUNC(CrimsonEye_Draw_Core);
    ADD_PUBLIC_FUNC(CrimsonEye_CheckPlayerCollisions_Ball);
    ADD_PUBLIC_FUNC(CrimsonEye_StateBall_Spinning);
    ADD_PUBLIC_FUNC(CrimsonEye_StateBall_Destroyed);
    ADD_PUBLIC_FUNC(CrimsonEye_Draw_Simple);
    ADD_PUBLIC_FUNC(CrimsonEye_StateSpike_Harmful);
    ADD_PUBLIC_FUNC(CrimsonEye_StateSpike_Debris);
    ADD_PUBLIC_FUNC(CrimsonEye_State_Shot);
    ADD_PUBLIC_FUNC(CrimsonEye_State_Arrow);
    ADD_PUBLIC_FUNC(CrimsonEye_Draw_Arrow);

    // TMZ/EscapeCar
    ADD_PUBLIC_FUNC(EscapeCar_StateMania_AwaitPlayer);
    ADD_PUBLIC_FUNC(EscapeCar_StateMania_EnteredCar);
    ADD_PUBLIC_FUNC(EscapeCar_StateMania_Ride);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(EscapeCar_StateEncore_Setup);
    ADD_PUBLIC_FUNC(EscapeCar_StateEncore_BadEnd);
    ADD_PUBLIC_FUNC(EscapeCar_StateEncore_GoodEnd);
#endif

    // TMZ/FlasherMKII
    ADD_PUBLIC_FUNC(FlasherMKII_DebugSpawn);
    ADD_PUBLIC_FUNC(FlasherMKII_CheckOffScreen);
    ADD_PUBLIC_FUNC(FlasherMKII_HandlePlayerCollisions);
    ADD_PUBLIC_FUNC(FlasherMKII_HandleHarmPlayerCollisions);
    ADD_PUBLIC_FUNC(FlasherMKII_State_Idle);
    ADD_PUBLIC_FUNC(FlasherMKII_State_Moving);
    ADD_PUBLIC_FUNC(FlasherMKII_State_WeakFlash);
    ADD_PUBLIC_FUNC(FlasherMKII_State_StrongFlash);
    ADD_PUBLIC_FUNC(FlasherMKII_State_FinishedFlashing);

    // TMZ/GymBar
    ADD_PUBLIC_FUNC(GymBar_DebugSpawn);
    ADD_PUBLIC_FUNC(GymBar_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(GymBar_HandleSwingJump);
    ADD_PUBLIC_FUNC(GymBar_PlayerState_SwingV);
    ADD_PUBLIC_FUNC(GymBar_PlayerState_Hang);
    ADD_PUBLIC_FUNC(GymBar_PlayerState_SwingH);

    // TMZ/LaunchSpring
    ADD_PUBLIC_FUNC(LaunchSpring_CheckFireworkActive);
    ADD_PUBLIC_FUNC(LaunchSpring_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(LaunchSpring_LaunchPlayer);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Spinning);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Spinning_GrabbedPlayer);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Spinning_ReadyToFire);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Spinning_FiredPlayer);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Spinning_ReadyUp);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Cannon);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Cannon_Fire_Automatic);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Cannon_Fire_Manual);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Cannon_FiredPlayer);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Rotating);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Rotate_Fire_Automatic);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Rotate_Fire_Manual);
    ADD_PUBLIC_FUNC(LaunchSpring_State_Rotating_FiredPlayer);

    // TMZ/MagnetSphere
    ADD_PUBLIC_FUNC(MagnetSphere_DebugSpawn);
    ADD_PUBLIC_FUNC(MagnetSphere_CheckPlayerCollision);
    ADD_PUBLIC_FUNC(MagnetSphere_MovePlayer);

    // TMZ/MetalArm
    ADD_PUBLIC_FUNC(MetalArm_SfxCheck_MetalArm);
    ADD_PUBLIC_FUNC(MetalArm_GetArmPosition);
    ADD_PUBLIC_FUNC(MetalArm_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(MetalArm_State_Idle);
    ADD_PUBLIC_FUNC(MetalArm_State_MoveToHold);
    ADD_PUBLIC_FUNC(MetalArm_State_Holding);
    ADD_PUBLIC_FUNC(MetalArm_State_MoveToStart);

    // TMZ/PopOut
    ADD_PUBLIC_FUNC(PopOut_SetupHitboxes);

    // TMZ/SentryBug
    ADD_PUBLIC_FUNC(SentryBug_DebugSpawn);
    ADD_PUBLIC_FUNC(SentryBug_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SentryBug_SetupOrbDropVelocity);
    ADD_PUBLIC_FUNC(SentryBug_StateOrbs_Attached);
    ADD_PUBLIC_FUNC(SentryBug_StateOrbs_Dropped);
    ADD_PUBLIC_FUNC(SentryBug_StateOrbs_BeginNetRotation);
    ADD_PUBLIC_FUNC(SentryBug_StateOrbs_RotateAroundNet);
    ADD_PUBLIC_FUNC(SentryBug_StateOrbs_ReturnToSlots);
    ADD_PUBLIC_FUNC(SentryBug_State_AwaitPlayer);
    ADD_PUBLIC_FUNC(SentryBug_State_DropOrbs);
    ADD_PUBLIC_FUNC(SentryBug_State_NetAppear);
    ADD_PUBLIC_FUNC(SentryBug_State_NetShrink);
    ADD_PUBLIC_FUNC(SentryBug_State_NetFlash);
    ADD_PUBLIC_FUNC(SentryBug_State_ReturnToSlots);

    // TMZ/TeeterTotter
    ADD_PUBLIC_FUNC(TeeterTotter_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TeeterTotter_ProcessSegmentGravity);
    ADD_PUBLIC_FUNC(TeeterTotter_HandleSegmentPositions);
    ADD_PUBLIC_FUNC(TeeterTotter_State_Init);
    ADD_PUBLIC_FUNC(TeeterTotter_State_Teeter);
    ADD_PUBLIC_FUNC(TeeterTotter_State_Fall);

    // TMZ/TMZ1Outro
    ADD_PUBLIC_FUNC(TMZ1Outro_CutsceneAct1_SetupPlayers);
    ADD_PUBLIC_FUNC(TMZ1Outro_CutsceneAct1_ElevatorRide);
    ADD_PUBLIC_FUNC(TMZ1Outro_CutsceneAct1_HeadForEntrance);
    ADD_PUBLIC_FUNC(TMZ1Outro_CutsceneAct1_EnterMonarch);
    ADD_PUBLIC_FUNC(TMZ1Outro_CutsceneAct2_BeginAct2);

    // TMZ/TMZ1Setup
    ADD_PUBLIC_FUNC(TMZ1Setup_BGSwitch_ShowSky);
    ADD_PUBLIC_FUNC(TMZ1Setup_BGSwitch_ShowLift);
    ADD_PUBLIC_FUNC(TMZ1Setup_StageFinish_EndAct1);
    ADD_PUBLIC_FUNC(TMZ1Setup_State_FadeToLift);
    ADD_PUBLIC_FUNC(TMZ1Setup_State_ShowOutsideBGs);
    ADD_PUBLIC_FUNC(TMZ1Setup_State_FadeOutCurrentBG);
    ADD_PUBLIC_FUNC(TMZ1Setup_State_ShowCityBG);
    ADD_PUBLIC_FUNC(TMZ1Setup_State_ShowSkyBG);
    ADD_PUBLIC_FUNC(TMZ1Setup_State_FadeIntoOutsideBGs);

    // TMZ/TMZ2Outro
    ADD_PUBLIC_FUNC(TMZ2Outro_SetupCutscene);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_SetupOutro);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_WatchEggman);
    ADD_PUBLIC_FUNC(TMZ2Outro_PlayerStateInput_Escape);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_EggmanFall);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_StartAlert);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_TimeToEscape);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_HurryToCar);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_StartFadeOut);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_StartRubyRampage);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_Panic);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_OuttaHere_BadEnd);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_OuttaHere);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_TeamEscape);
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_FadeOut);
#endif
    ADD_PUBLIC_FUNC(TMZ2Outro_Cutscene_FinishSequence);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(TMZ2Outro_SaveFileCB);
#else
    ADD_PUBLIC_FUNC(TMZ2Outro_SaveFileCB);
#endif

    // TMZ/TMZ2Setup
    ADD_PUBLIC_FUNC(TMZ2Setup_DrawHook_ApplyDynTilesPalette);
    ADD_PUBLIC_FUNC(TMZ2Setup_DrawHook_RemoveDynTilesPalette);
    ADD_PUBLIC_FUNC(TMZ2Setup_DrawDynTiles_Eggman);
    ADD_PUBLIC_FUNC(TMZ2Setup_DrawDynTiles_Ruby);

    // TMZ/TMZ3Setup
    ADD_PUBLIC_FUNC(TMZ3Setup_DrawHook_ApplyDynTilesPalette);
    ADD_PUBLIC_FUNC(TMZ3Setup_DrawHook_RemoveDynTilesPalette);

    // TMZ/TMZAlert
    ADD_PUBLIC_FUNC(TMZAlert_State_Activating);
    ADD_PUBLIC_FUNC(TMZAlert_State_Alerting);
    ADD_PUBLIC_FUNC(TMZAlert_State_ShuttingDown);

    // TMZ/TMZCable
    ADD_PUBLIC_FUNC(TMZCable_HandleDrawPositions);
    ADD_PUBLIC_FUNC(TMZCable_State_Idle);
    ADD_PUBLIC_FUNC(TMZCable_State_Charge);
    ADD_PUBLIC_FUNC(TMZCable_State_Live);
    ADD_PUBLIC_FUNC(TMZCable_State_Fade);
    ADD_PUBLIC_FUNC(TMZCable_State_Destroyed);

    // TMZ/TMZFlames
    ADD_PUBLIC_FUNC(TMZFlames_State_Delay);
    ADD_PUBLIC_FUNC(TMZFlames_State_EnterFlames);
    ADD_PUBLIC_FUNC(TMZFlames_State_FlamesEnlarge);

    // TMZ/TurboTurtle
    ADD_PUBLIC_FUNC(TurboTurtle_DebugSpawn);
    ADD_PUBLIC_FUNC(TurboTurtle_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TurboTurtle_SetupState);
    ADD_PUBLIC_FUNC(TurboTurtle_HandleFans);
    ADD_PUBLIC_FUNC(TurboTurtle_HandleFanParticles);
    ADD_PUBLIC_FUNC(TurboTurtle_SfxCheck_SmallFan);
    ADD_PUBLIC_FUNC(TurboTurtle_State_Init);
    ADD_PUBLIC_FUNC(TurboTurtle_State_Walking);
    ADD_PUBLIC_FUNC(TurboTurtle_State_TurnFanUp);
    ADD_PUBLIC_FUNC(TurboTurtle_State_FanTop);
    ADD_PUBLIC_FUNC(TurboTurtle_State_TurnFanRight);
    ADD_PUBLIC_FUNC(TurboTurtle_State_FanRight);
    ADD_PUBLIC_FUNC(TurboTurtle_State_TurnFanLeft);
    ADD_PUBLIC_FUNC(TurboTurtle_State_FanLeft);

    // TMZ/WallBumper
    ADD_PUBLIC_FUNC(WallBumper_DebugSpawn);
    ADD_PUBLIC_FUNC(WallBumper_HandleInteractions);

    // UFO/SpecialClear
    ADD_PUBLIC_FUNC(SpecialClear_DrawNumbers);
    ADD_PUBLIC_FUNC(SpecialClear_GiveScoreBonus);
#if MANIA_USE_PLUS
    ADD_PUBLIC_FUNC(SpecialClear_SaveCB);
#else
    ADD_PUBLIC_FUNC(SpecialClear_SaveCB);
    ADD_PUBLIC_FUNC(SpecialClear_State_SetupDelay);
#endif
    ADD_PUBLIC_FUNC(SpecialClear_State_EnterText);
    ADD_PUBLIC_FUNC(SpecialClear_State_AdjustText);
    ADD_PUBLIC_FUNC(SpecialClear_HandleEmeraldAppear);
    ADD_PUBLIC_FUNC(SpecialClear_State_EnterBonuses);
    ADD_PUBLIC_FUNC(SpecialClear_State_ScoreShownDelay);
    ADD_PUBLIC_FUNC(SpecialClear_State_TallyScore);
    ADD_PUBLIC_FUNC(SpecialClear_State_ShowTotalScore_Continues);
    ADD_PUBLIC_FUNC(SpecialClear_State_ShowTotalScore_NoContinues);
    ADD_PUBLIC_FUNC(SpecialClear_State_ExitFinishMessage);
    ADD_PUBLIC_FUNC(SpecialClear_State_EnterSuperMessage);
    ADD_PUBLIC_FUNC(SpecialClear_State_ShowSuperMessage);
    ADD_PUBLIC_FUNC(SpecialClear_State_ExitFadeOut);
    ADD_PUBLIC_FUNC(SpecialClear_State_ExitResults);

    // UFO/UFO_Camera
    ADD_PUBLIC_FUNC(UFO_Camera_HandleCamPos);
    ADD_PUBLIC_FUNC(UFO_Camera_State_Normal);
    ADD_PUBLIC_FUNC(UFO_Camera_State_CourseOut);
    ADD_PUBLIC_FUNC(UFO_Camera_State_UFOCaught);

    // UFO/UFO_Circuit
    ADD_PUBLIC_FUNC(UFO_Circuit_HandleSpeedSetup);
    ADD_PUBLIC_FUNC(UFO_Circuit_HandleNodeSpeeds);
    ADD_PUBLIC_FUNC(UFO_Circuit_CheckNodeChange);
    ADD_PUBLIC_FUNC(UFO_Circuit_State_UFO);
    ADD_PUBLIC_FUNC(UFO_Circuit_State_Caught);

    // UFO/UFO_HUD
    ADD_PUBLIC_FUNC(UFO_HUD_CheckLevelUp);
    ADD_PUBLIC_FUNC(UFO_HUD_LevelUpMach);
    ADD_PUBLIC_FUNC(UFO_HUD_DrawNumbers);

    // UFO/UFO_ItemBox
    ADD_PUBLIC_FUNC(UFO_ItemBox_State_HasContents);
    ADD_PUBLIC_FUNC(UFO_ItemBox_State_ShowContents);

    // UFO/UFO_Message
    ADD_PUBLIC_FUNC(UFO_Message_State_Appear);
    ADD_PUBLIC_FUNC(UFO_Message_State_ShowMsg);
    ADD_PUBLIC_FUNC(UFO_Message_State_Exit);

    // UFO/UFO_Player
    ADD_PUBLIC_FUNC(UFO_Player_Input_P1);
    ADD_PUBLIC_FUNC(UFO_Player_ChangeMachState);
    ADD_PUBLIC_FUNC(UFO_Player_HandleBumperTiles);
    ADD_PUBLIC_FUNC(UFO_Player_HandleSpeedUp);
    ADD_PUBLIC_FUNC(UFO_Player_State_Run);
    ADD_PUBLIC_FUNC(UFO_Player_State_Jump);
    ADD_PUBLIC_FUNC(UFO_Player_State_Springboard);
    ADD_PUBLIC_FUNC(UFO_Player_State_Trip);
    ADD_PUBLIC_FUNC(UFO_Player_State_CourseOut);
    ADD_PUBLIC_FUNC(UFO_Player_State_UFOCaught_Charge);
    ADD_PUBLIC_FUNC(UFO_Player_State_UFOCaught_Released);

    // UFO/UFO_Ring
    ADD_PUBLIC_FUNC(UFO_Ring_PlayRingSfx);
    ADD_PUBLIC_FUNC(UFO_Ring_LoseRings);
    ADD_PUBLIC_FUNC(UFO_Ring_State_NormalRing);
    ADD_PUBLIC_FUNC(UFO_Ring_State_Sparkle);
    ADD_PUBLIC_FUNC(UFO_Ring_State_LoseRing);

    // UFO/UFO_Setup
    ADD_PUBLIC_FUNC(UFO_Setup_DrawHook_PrepareDrawingFX);
    ADD_PUBLIC_FUNC(UFO_Setup_Scanline_Playfield);
    ADD_PUBLIC_FUNC(UFO_Setup_Scanline_3DFloor);
    ADD_PUBLIC_FUNC(UFO_Setup_Scanline_3DRoof);
    ADD_PUBLIC_FUNC(UFO_Setup_PlaySphereSfx);
    ADD_PUBLIC_FUNC(UFO_Setup_Finish_Win);
    ADD_PUBLIC_FUNC(UFO_Setup_Finish_Fail);
    ADD_PUBLIC_FUNC(UFO_Setup_Deform_UFO3);
    ADD_PUBLIC_FUNC(UFO_Setup_Deform_UFO4);
    ADD_PUBLIC_FUNC(UFO_Setup_Deform_UFO5);
    ADD_PUBLIC_FUNC(UFO_Setup_State_ShowStartMessage);
    ADD_PUBLIC_FUNC(UFO_Setup_State_FinishFadeout);
    ADD_PUBLIC_FUNC(UFO_Setup_State_HandleRingDrain);
    ADD_PUBLIC_FUNC(UFO_Setup_State_TimedOver);

    // UFO/UFO_Sphere
    ADD_PUBLIC_FUNC(UFO_Sphere_State_Fixed);
    ADD_PUBLIC_FUNC(UFO_Sphere_State_Bouncing);
    ADD_PUBLIC_FUNC(UFO_Sphere_State_Moving);
    ADD_PUBLIC_FUNC(UFO_Sphere_State_Collected);
    ADD_PUBLIC_FUNC(UFO_Sphere_State_HandleRespawn);
    ADD_PUBLIC_FUNC(UFO_Sphere_State_AnimateAndDestroy);

    // Unused/SpearBlock
    ADD_PUBLIC_FUNC(SpearBlock_DebugSpawn);
    ADD_PUBLIC_FUNC(SpearBlock_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(SpearBlock_State_SetupSpears);
    ADD_PUBLIC_FUNC(SpearBlock_State_SpearRetracted);
    ADD_PUBLIC_FUNC(SpearBlock_State_ExtendSpears);
    ADD_PUBLIC_FUNC(SpearBlock_State_SpearExtended);
    ADD_PUBLIC_FUNC(SpearBlock_State_RetractSpears);

    // Unused/TargetBumper
    ADD_PUBLIC_FUNC(TargetBumper_DebugSpawn);
    ADD_PUBLIC_FUNC(TargetBumper_CheckPlayerCollisions);
    ADD_PUBLIC_FUNC(TargetBumper_State_Idle);
    ADD_PUBLIC_FUNC(TargetBumper_State_Hit);

    // Unused/WallCrawl
    ADD_PUBLIC_FUNC(WallCrawl_DebugSpawn);
    ADD_PUBLIC_FUNC(WallCrawl_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(WallCrawl_CheckOffScreen);
    ADD_PUBLIC_FUNC(WallCrawl_State_Init);
    ADD_PUBLIC_FUNC(WallCrawl_State_Moving);
    ADD_PUBLIC_FUNC(WallCrawl_State_Idle);
    ADD_PUBLIC_FUNC(WallCrawl_State_Projectile);

    // Unused/Wisp
    ADD_PUBLIC_FUNC(Wisp_DebugSpawn);
    ADD_PUBLIC_FUNC(Wisp_HandlePlayerInteractions);
    ADD_PUBLIC_FUNC(Wisp_CheckOffScreen);
    ADD_PUBLIC_FUNC(Wisp_State_Init);
    ADD_PUBLIC_FUNC(Wisp_State_Idle);
    ADD_PUBLIC_FUNC(Wisp_State_FlyTowardTarget);
    ADD_PUBLIC_FUNC(Wisp_State_FlyAway);

}
