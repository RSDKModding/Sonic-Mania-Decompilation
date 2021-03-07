#include "SonicMania.hpp"

SceneInfo *RSDK_sceneInfo        = NULL;
SKUInfo *RSDK_sku                = NULL;
ScreenInfo *RSDK_screens         = NULL;
ControllerState *RSDK_controller = NULL;
AnalogState *RSDK_stickL         = NULL;
AnalogState *RSDK_stickR         = NULL;
TriggerState *RSDK_triggerL      = NULL;
TriggerState *RSDK_triggerR      = NULL;
TouchMouseData *RSDK_touchMouse  = NULL;

char textBuffer[0x400];

UserFunctionTable User;
RSDKFunctionTable RSDK;

void LinkGameLogicDLL(GameInfo *info)
{
    memset(&User, 0, sizeof(UserFunctionTable));
    memset(&RSDK, 0, sizeof(RSDKFunctionTable));
    
    if (info->functionPtrs)
        memcpy(&RSDK, info->functionPtrs, sizeof(RSDKFunctionTable));
    if (info->userdataPtrs)
        memcpy(&User, info->userdataPtrs, sizeof(UserFunctionTable));
    RSDK_sku        = info->currentSKU;
    RSDK_sceneInfo  = info->sceneInfo;
    RSDK_controller = info->controller;
    RSDK_stickL     = info->stickL;
    RSDK_stickR     = info->stickR;
    RSDK_triggerL   = info->triggerL;
    RSDK_triggerR   = info->triggerR;
    RSDK_touchMouse = info->touchMouse;
    // GameInfo_Unknown5      = (int)info->InputCount;
    RSDK_screens = info->screenInfo;
    RSDK.InitGameOptions((void**)&options, sizeof(GameOptions));

    defaultHitbox.left   = -10;
    defaultHitbox.top    = -20;
    defaultHitbox.right  = 10;
    defaultHitbox.bottom = 20;

    RSDK.CreateObject((Object**)&ActClear, "ActClear", sizeof(EntityActClear), sizeof(ObjectActClear), ActClear_Update, ActClear_LateUpdate, ActClear_StaticUpdate,
                 ActClear_Draw, ActClear_Create, ActClear_StageLoad, ActClear_EditorDraw, ActClear_EditorLoad, ActClear_Serialize);
    RSDK.CreateObject((Object**)&AIZEggRobo, "AIZEggRobo", sizeof(EntityAIZEggRobo), sizeof(ObjectAIZEggRobo), AIZEggRobo_Update, AIZEggRobo_LateUpdate,
                 AIZEggRobo_StaticUpdate, AIZEggRobo_Draw, AIZEggRobo_Create, AIZEggRobo_StageLoad, AIZEggRobo_EditorDraw, AIZEggRobo_EditorLoad,
                 AIZEggRobo_Serialize);
    RSDK.CreateObject((Object**)&AIZEncoreTutorial, "AIZEncoreTutorial", sizeof(EntityAIZEncoreTutorial), sizeof(ObjectAIZEncoreTutorial), AIZEncoreTutorial_Update,
                 AIZEncoreTutorial_LateUpdate, AIZEncoreTutorial_StaticUpdate, AIZEncoreTutorial_Draw, AIZEncoreTutorial_Create,
                 AIZEncoreTutorial_StageLoad, AIZEncoreTutorial_EditorDraw, AIZEncoreTutorial_EditorLoad, AIZEncoreTutorial_Serialize);
    RSDK.CreateObject((Object**)&AIZKingClaw, "AIZKingClaw", sizeof(EntityAIZKingClaw), sizeof(ObjectAIZKingClaw), AIZKingClaw_Update, AIZKingClaw_LateUpdate,
                 AIZKingClaw_StaticUpdate, AIZKingClaw_Draw, AIZKingClaw_Create, AIZKingClaw_StageLoad, AIZKingClaw_EditorDraw, AIZKingClaw_EditorLoad,
                 AIZKingClaw_Serialize);
    RSDK.CreateObject((Object**)&AIZRockPile, "AIZRockPile", sizeof(EntityAIZRockPile), sizeof(ObjectAIZRockPile), AIZRockPile_Update, AIZRockPile_LateUpdate,
                 AIZRockPile_StaticUpdate, AIZRockPile_Draw, AIZRockPile_Create, AIZRockPile_StageLoad, AIZRockPile_EditorDraw, AIZRockPile_EditorLoad,
                 AIZRockPile_Serialize);
    RSDK.CreateObject((Object**)&AIZSetup, "AIZSetup", sizeof(EntityAIZSetup), sizeof(ObjectAIZSetup), AIZSetup_Update, AIZSetup_LateUpdate, AIZSetup_StaticUpdate,
                 AIZSetup_Draw, AIZSetup_Create, AIZSetup_StageLoad, AIZSetup_EditorDraw, AIZSetup_EditorLoad, AIZSetup_Serialize);
    RSDK.CreateObject((Object**)&AIZTornado, "AIZTornado", sizeof(EntityAIZTornado), sizeof(ObjectAIZTornado), AIZTornado_Update, AIZTornado_LateUpdate,
                 AIZTornado_StaticUpdate, AIZTornado_Draw, AIZTornado_Create, AIZTornado_StageLoad, AIZTornado_EditorDraw, AIZTornado_EditorLoad,
                 AIZTornado_Serialize);
    RSDK.CreateObject((Object**)&AIZTornadoPath, "AIZTornadoPath", sizeof(EntityAIZTornadoPath), sizeof(ObjectAIZTornadoPath), AIZTornadoPath_Update,
                 AIZTornadoPath_LateUpdate, AIZTornadoPath_StaticUpdate, AIZTornadoPath_Draw, AIZTornadoPath_Create, AIZTornadoPath_StageLoad,
                 AIZTornadoPath_EditorDraw, AIZTornadoPath_EditorLoad, AIZTornadoPath_Serialize);
    RSDK.CreateObject((Object**)&AmoebaDroid, "AmoebaDroid", sizeof(EntityAmoebaDroid), sizeof(ObjectAmoebaDroid), AmoebaDroid_Update, AmoebaDroid_LateUpdate,
                 AmoebaDroid_StaticUpdate, AmoebaDroid_Draw, AmoebaDroid_Create, AmoebaDroid_StageLoad, AmoebaDroid_EditorDraw, AmoebaDroid_EditorLoad,
                 AmoebaDroid_Serialize);
    RSDK.CreateObject((Object**)&AnimalHBH, "AnimalHBH", sizeof(EntityAnimalHBH), sizeof(ObjectAnimalHBH), AnimalHBH_Update, AnimalHBH_LateUpdate,
                 AnimalHBH_StaticUpdate, AnimalHBH_Draw, AnimalHBH_Create, AnimalHBH_StageLoad, AnimalHBH_EditorDraw, AnimalHBH_EditorLoad,
                 AnimalHBH_Serialize);
    RSDK.CreateObject((Object**)&Animals, "Animals", sizeof(EntityAnimals), sizeof(ObjectAnimals), Animals_Update, Animals_LateUpdate, Animals_StaticUpdate,
                 Animals_Draw, Animals_Create, Animals_StageLoad, Animals_EditorDraw, Animals_EditorLoad, Animals_Serialize);
    RSDK.CreateObject((Object**)&Announcer, "Announcer", sizeof(EntityAnnouncer), sizeof(ObjectAnnouncer), Announcer_Update, Announcer_LateUpdate,
                 Announcer_StaticUpdate, Announcer_Draw, Announcer_Create, Announcer_StageLoad, Announcer_EditorDraw, Announcer_EditorLoad,
                 Announcer_Serialize);
    RSDK.CreateObject((Object**)&Aquis, "Aquis", sizeof(EntityAquis), sizeof(ObjectAquis), Aquis_Update, Aquis_LateUpdate, Aquis_StaticUpdate, Aquis_Draw,
                 Aquis_Create, Aquis_StageLoad, Aquis_EditorDraw, Aquis_EditorLoad, Aquis_Serialize);
    RSDK.CreateObject((Object**)&Armadiloid, "Armadiloid", sizeof(EntityArmadiloid), sizeof(ObjectArmadiloid), Armadiloid_Update, Armadiloid_LateUpdate,
                 Armadiloid_StaticUpdate, Armadiloid_Draw, Armadiloid_Create, Armadiloid_StageLoad, Armadiloid_EditorDraw, Armadiloid_EditorLoad,
                 Armadiloid_Serialize);
    RSDK.CreateObject((Object**)&BallCannon, "BallCannon", sizeof(EntityBallCannon), sizeof(ObjectBallCannon), BallCannon_Update, BallCannon_LateUpdate,
                 BallCannon_StaticUpdate, BallCannon_Draw, BallCannon_Create, BallCannon_StageLoad, BallCannon_EditorDraw, BallCannon_EditorLoad,
                 BallCannon_Serialize);
    RSDK.CreateObject((Object**)&BallHog, "BallHog", sizeof(EntityBallHog), sizeof(ObjectBallHog), BallHog_Update, BallHog_LateUpdate, BallHog_StaticUpdate,
                 BallHog_Draw, BallHog_Create, BallHog_StageLoad, BallHog_EditorDraw, BallHog_EditorLoad, BallHog_Serialize);
    RSDK.CreateObject((Object**)&BarStool, "BarStool", sizeof(EntityBarStool), sizeof(ObjectBarStool), BarStool_Update, BarStool_LateUpdate, BarStool_StaticUpdate,
                 BarStool_Draw, BarStool_Create, BarStool_StageLoad, BarStool_EditorDraw, BarStool_EditorLoad, BarStool_Serialize);
    RSDK.CreateObject((Object**)&Batbrain, "Batbrain", sizeof(EntityBatbrain), sizeof(ObjectBatbrain), Batbrain_Update, Batbrain_LateUpdate, Batbrain_StaticUpdate,
                 Batbrain_Draw, Batbrain_Create, Batbrain_StageLoad, Batbrain_EditorDraw, Batbrain_EditorLoad, Batbrain_Serialize);
    RSDK.CreateObject((Object**)&Beanstalk, "Beanstalk", sizeof(EntityBeanstalk), sizeof(ObjectBeanstalk), Beanstalk_Update, Beanstalk_LateUpdate,
                 Beanstalk_StaticUpdate, Beanstalk_Draw, Beanstalk_Create, Beanstalk_StageLoad, Beanstalk_EditorDraw, Beanstalk_EditorLoad,
                 Beanstalk_Serialize);
    RSDK.CreateObject((Object**)&BGSwitch, "BGSwitch", sizeof(EntityBGSwitch), sizeof(ObjectBGSwitch), BGSwitch_Update, BGSwitch_LateUpdate, BGSwitch_StaticUpdate,
                 BGSwitch_Draw, BGSwitch_Create, BGSwitch_StageLoad, BGSwitch_EditorDraw, BGSwitch_EditorLoad, BGSwitch_Serialize);
    RSDK.CreateObject((Object**)&BigSqueeze, "BigSqueeze", sizeof(EntityBigSqueeze), sizeof(ObjectBigSqueeze), BigSqueeze_Update, BigSqueeze_LateUpdate,
                 BigSqueeze_StaticUpdate, BigSqueeze_Draw, BigSqueeze_Create, BigSqueeze_StageLoad, BigSqueeze_EditorDraw, BigSqueeze_EditorLoad,
                 BigSqueeze_Serialize);
    RSDK.CreateObject((Object**)&BladePole, "BladePole", sizeof(EntityBladePole), sizeof(ObjectBladePole), BladePole_Update, BladePole_LateUpdate,
                 BladePole_StaticUpdate, BladePole_Draw, BladePole_Create, BladePole_StageLoad, BladePole_EditorDraw, BladePole_EditorLoad,
                 BladePole_Serialize);
    RSDK.CreateObject((Object**)&Blaster, "Blaster", sizeof(EntityBlaster), sizeof(ObjectBlaster), Blaster_Update, Blaster_LateUpdate, Blaster_StaticUpdate,
                 Blaster_Draw, Blaster_Create, Blaster_StageLoad, Blaster_EditorDraw, Blaster_EditorLoad, Blaster_Serialize);
    RSDK.CreateObject((Object**)&Blastoid, "Blastoid", sizeof(EntityBlastoid), sizeof(ObjectBlastoid), Blastoid_Update, Blastoid_LateUpdate, Blastoid_StaticUpdate,
                 Blastoid_Draw, Blastoid_Create, Blastoid_StageLoad, Blastoid_EditorDraw, Blastoid_EditorLoad, Blastoid_Serialize);
    RSDK.CreateObject((Object**)&Bloominator, "Bloominator", sizeof(EntityBloominator), sizeof(ObjectBloominator), Bloominator_Update, Bloominator_LateUpdate,
                 Bloominator_StaticUpdate, Bloominator_Draw, Bloominator_Create, Bloominator_StageLoad, Bloominator_EditorDraw, Bloominator_EditorLoad,
                 Bloominator_Serialize);
    RSDK.CreateObject((Object**)&BouncePlant, "BouncePlant", sizeof(EntityBouncePlant), sizeof(ObjectBouncePlant), BouncePlant_Update, BouncePlant_LateUpdate,
                 BouncePlant_StaticUpdate, BouncePlant_Draw, BouncePlant_Create, BouncePlant_StageLoad, BouncePlant_EditorDraw, BouncePlant_EditorLoad,
                 BouncePlant_Serialize);
    RSDK.CreateObject((Object**)&BoundsMarker, "BoundsMarker", sizeof(EntityBoundsMarker), sizeof(ObjectBoundsMarker), BoundsMarker_Update, BoundsMarker_LateUpdate,
                 BoundsMarker_StaticUpdate, BoundsMarker_Draw, BoundsMarker_Create, BoundsMarker_StageLoad, BoundsMarker_EditorDraw,
                 BoundsMarker_EditorLoad, BoundsMarker_Serialize);
    RSDK.CreateObject((Object**)&BreakableWall, "BreakableWall", sizeof(EntityBreakableWall), sizeof(ObjectBreakableWall), BreakableWall_Update,
                 BreakableWall_LateUpdate, BreakableWall_StaticUpdate, BreakableWall_Draw, BreakableWall_Create, BreakableWall_StageLoad,
                 BreakableWall_EditorDraw, BreakableWall_EditorLoad, BreakableWall_Serialize);
    RSDK.CreateObject((Object**)&BreakBar, "BreakBar", sizeof(EntityBreakBar), sizeof(ObjectBreakBar), BreakBar_Update, BreakBar_LateUpdate, BreakBar_StaticUpdate,
                 BreakBar_Draw, BreakBar_Create, BreakBar_StageLoad, BreakBar_EditorDraw, BreakBar_EditorLoad, BreakBar_Serialize);
    RSDK.CreateObject((Object**)&Bridge, "Bridge", sizeof(EntityBridge), sizeof(ObjectBridge), Bridge_Update, Bridge_LateUpdate, Bridge_StaticUpdate, Bridge_Draw,
                 Bridge_Create, Bridge_StageLoad, Bridge_EditorDraw, Bridge_EditorLoad, Bridge_Serialize);
    RSDK.CreateObject((Object**)&BSS_Collectable, "BSS_Collectable", sizeof(EntityBSS_Collectable), sizeof(ObjectBSS_Collectable), BSS_Collectable_Update,
                 BSS_Collectable_LateUpdate, BSS_Collectable_StaticUpdate, BSS_Collectable_Draw, BSS_Collectable_Create, BSS_Collectable_StageLoad,
                 BSS_Collectable_EditorDraw, BSS_Collectable_EditorLoad, BSS_Collectable_Serialize);
    RSDK.CreateObject((Object**)&BSS_Collected, "BSS_Collected", sizeof(EntityBSS_Collected), sizeof(ObjectBSS_Collected), BSS_Collected_Update,
                 BSS_Collected_LateUpdate, BSS_Collected_StaticUpdate, BSS_Collected_Draw, BSS_Collected_Create, BSS_Collected_StageLoad,
                 BSS_Collected_EditorDraw, BSS_Collected_EditorLoad, BSS_Collected_Serialize);
    RSDK.CreateObject((Object**)&BSS_Horizon, "BSS_Horizon", sizeof(EntityBSS_Horizon), sizeof(ObjectBSS_Horizon), BSS_Horizon_Update, BSS_Horizon_LateUpdate,
                 BSS_Horizon_StaticUpdate, BSS_Horizon_Draw, BSS_Horizon_Create, BSS_Horizon_StageLoad, BSS_Horizon_EditorDraw, BSS_Horizon_EditorLoad,
                 BSS_Horizon_Serialize);
    RSDK.CreateObject((Object**)&BSS_HUD, "BSS_HUD", sizeof(EntityBSS_HUD), sizeof(ObjectBSS_HUD), BSS_HUD_Update, BSS_HUD_LateUpdate, BSS_HUD_StaticUpdate,
                 BSS_HUD_Draw, BSS_HUD_Create, BSS_HUD_StageLoad, BSS_HUD_EditorDraw, BSS_HUD_EditorLoad, BSS_HUD_Serialize);
    RSDK.CreateObject((Object**)&BSS_Message, "BSS_Message", sizeof(EntityBSS_Message), sizeof(ObjectBSS_Message), BSS_Message_Update, BSS_Message_LateUpdate,
                 BSS_Message_StaticUpdate, BSS_Message_Draw, BSS_Message_Create, BSS_Message_StageLoad, BSS_Message_EditorDraw, BSS_Message_EditorLoad,
                 BSS_Message_Serialize);
    RSDK.CreateObject((Object**)&BSS_Palette, "BSS_Palette", sizeof(EntityBSS_Palette), sizeof(ObjectBSS_Palette), BSS_Palette_Update, BSS_Palette_LateUpdate,
                 BSS_Palette_StaticUpdate, BSS_Palette_Draw, BSS_Palette_Create, BSS_Palette_StageLoad, BSS_Palette_EditorDraw, BSS_Palette_EditorLoad,
                 BSS_Palette_Serialize);
    RSDK.CreateObject((Object**)&BSS_Player, "BSS_Player", sizeof(EntityBSS_Player), sizeof(ObjectBSS_Player), BSS_Player_Update, BSS_Player_LateUpdate,
                 BSS_Player_StaticUpdate, BSS_Player_Draw, BSS_Player_Create, BSS_Player_StageLoad, BSS_Player_EditorDraw, BSS_Player_EditorLoad,
                 BSS_Player_Serialize);
    RSDK.CreateObject((Object**)&BSS_Setup, "BSS_Setup", sizeof(EntityBSS_Setup), sizeof(ObjectBSS_Setup), BSS_Setup_Update, BSS_Setup_LateUpdate,
                 BSS_Setup_StaticUpdate, BSS_Setup_Draw, BSS_Setup_Create, BSS_Setup_StageLoad, BSS_Setup_EditorDraw, BSS_Setup_EditorLoad,
                 BSS_Setup_Serialize);
    RSDK.CreateObject((Object**)&BuckwildBall, "BuckwildBall", sizeof(EntityBuckwildBall), sizeof(ObjectBuckwildBall), BuckwildBall_Update, BuckwildBall_LateUpdate,
                 BuckwildBall_StaticUpdate, BuckwildBall_Draw, BuckwildBall_Create, BuckwildBall_StageLoad, BuckwildBall_EditorDraw,
                 BuckwildBall_EditorLoad, BuckwildBall_Serialize);
    RSDK.CreateObject((Object**)&Buggernaut, "Buggernaut", sizeof(EntityBuggernaut), sizeof(ObjectBuggernaut), Buggernaut_Update, Buggernaut_LateUpdate,
                 Buggernaut_StaticUpdate, Buggernaut_Draw, Buggernaut_Create, Buggernaut_StageLoad, Buggernaut_EditorDraw, Buggernaut_EditorLoad,
                 Buggernaut_Serialize);
    RSDK.CreateObject((Object**)&Bumpalo, "Bumpalo", sizeof(EntityBumpalo), sizeof(ObjectBumpalo), Bumpalo_Update, Bumpalo_LateUpdate, Bumpalo_StaticUpdate,
                 Bumpalo_Draw, Bumpalo_Create, Bumpalo_StageLoad, Bumpalo_EditorDraw, Bumpalo_EditorLoad, Bumpalo_Serialize);
    RSDK.CreateObject((Object**)&Bungee, "Bungee", sizeof(EntityBungee), sizeof(ObjectBungee), Bungee_Update, Bungee_LateUpdate, Bungee_StaticUpdate, Bungee_Draw,
                 Bungee_Create, Bungee_StageLoad, Bungee_EditorDraw, Bungee_EditorLoad, Bungee_Serialize);
    RSDK.CreateObject((Object**)&BurningLog, "BurningLog", sizeof(EntityBurningLog), sizeof(ObjectBurningLog), BurningLog_Update, BurningLog_LateUpdate,
                 BurningLog_StaticUpdate, BurningLog_Draw, BurningLog_Create, BurningLog_StageLoad, BurningLog_EditorDraw, BurningLog_EditorLoad,
                 BurningLog_Serialize);
    RSDK.CreateObject((Object**)&Button, "Button", sizeof(EntityButton), sizeof(ObjectButton), Button_Update, Button_LateUpdate, Button_StaticUpdate, Button_Draw,
                 Button_Create, Button_StageLoad, Button_EditorDraw, Button_EditorLoad, Button_Serialize);
    RSDK.CreateObject((Object**)&ButtonDoor, "ButtonDoor", sizeof(EntityButtonDoor), sizeof(ObjectButtonDoor), ButtonDoor_Update, ButtonDoor_LateUpdate,
                 ButtonDoor_StaticUpdate, ButtonDoor_Draw, ButtonDoor_Create, ButtonDoor_StageLoad, ButtonDoor_EditorDraw, ButtonDoor_EditorLoad,
                 ButtonDoor_Serialize);
    RSDK.CreateObject((Object**)&BuzzBomber, "BuzzBomber", sizeof(EntityBuzzBomber), sizeof(ObjectBuzzBomber), BuzzBomber_Update, BuzzBomber_LateUpdate,
                 BuzzBomber_StaticUpdate, BuzzBomber_Draw, BuzzBomber_Create, BuzzBomber_StageLoad, BuzzBomber_EditorDraw, BuzzBomber_EditorLoad,
                 BuzzBomber_Serialize);
    RSDK.CreateObject((Object**)&BuzzSaw, "BuzzSaw", sizeof(EntityBuzzSaw), sizeof(ObjectBuzzSaw), BuzzSaw_Update, BuzzSaw_LateUpdate, BuzzSaw_StaticUpdate,
                 BuzzSaw_Draw, BuzzSaw_Create, BuzzSaw_StageLoad, BuzzSaw_EditorDraw, BuzzSaw_EditorLoad, BuzzSaw_Serialize);
    RSDK.CreateObject((Object**)&CableWarp, "CableWarp", sizeof(EntityCableWarp), sizeof(ObjectCableWarp), CableWarp_Update, CableWarp_LateUpdate,
                 CableWarp_StaticUpdate, CableWarp_Draw, CableWarp_Create, CableWarp_StageLoad, CableWarp_EditorDraw, CableWarp_EditorLoad,
                 CableWarp_Serialize);
    RSDK.CreateObject((Object**)&Cactula, "Cactula", sizeof(EntityCactula), sizeof(ObjectCactula), Cactula_Update, Cactula_LateUpdate, Cactula_StaticUpdate,
                 Cactula_Draw, Cactula_Create, Cactula_StageLoad, Cactula_EditorDraw, Cactula_EditorLoad, Cactula_Serialize);
    RSDK.CreateObject((Object**)&Camera, "Camera", sizeof(EntityCamera), sizeof(ObjectCamera), Camera_Update, Camera_LateUpdate, Camera_StaticUpdate, Camera_Draw,
                 Camera_Create, Camera_StageLoad, Camera_EditorDraw, Camera_EditorLoad, Camera_Serialize);
    RSDK.CreateObject((Object**)&Canista, "Canista", sizeof(EntityCanista), sizeof(ObjectCanista), Canista_Update, Canista_LateUpdate, Canista_StaticUpdate,
                 Canista_Draw, Canista_Create, Canista_StageLoad, Canista_EditorDraw, Canista_EditorLoad, Canista_Serialize);
    RSDK.CreateObject((Object**)&CaterkillerJr, "CaterkillerJr", sizeof(EntityCaterkillerJr), sizeof(ObjectCaterkillerJr), CaterkillerJr_Update,
                 CaterkillerJr_LateUpdate, CaterkillerJr_StaticUpdate, CaterkillerJr_Draw, CaterkillerJr_Create, CaterkillerJr_StageLoad,
                 CaterkillerJr_EditorDraw, CaterkillerJr_EditorLoad, CaterkillerJr_Serialize);
    RSDK.CreateObject((Object**)&ChaosEmerald, "ChaosEmerald", sizeof(EntityChaosEmerald), sizeof(ObjectChaosEmerald), ChaosEmerald_Update, ChaosEmerald_LateUpdate,
                 ChaosEmerald_StaticUpdate, ChaosEmerald_Draw, ChaosEmerald_Create, ChaosEmerald_StageLoad, ChaosEmerald_EditorDraw,
                 ChaosEmerald_EditorLoad, ChaosEmerald_Serialize);
    RSDK.CreateObject((Object**)&CheckerBall, "CheckerBall", sizeof(EntityCheckerBall), sizeof(ObjectCheckerBall), CheckerBall_Update, CheckerBall_LateUpdate,
                 CheckerBall_StaticUpdate, CheckerBall_Draw, CheckerBall_Create, CheckerBall_StageLoad, CheckerBall_EditorDraw, CheckerBall_EditorLoad,
                 CheckerBall_Serialize);
    RSDK.CreateObject((Object**)&ChemBubble, "ChemBubble", sizeof(EntityChemBubble), sizeof(ObjectChemBubble), ChemBubble_Update, ChemBubble_LateUpdate,
                 ChemBubble_StaticUpdate, ChemBubble_Draw, ChemBubble_Create, ChemBubble_StageLoad, ChemBubble_EditorDraw, ChemBubble_EditorLoad,
                 ChemBubble_Serialize);
    RSDK.CreateObject((Object**)&ChemicalBall, "ChemicalBall", sizeof(EntityChemicalBall), sizeof(ObjectChemicalBall), ChemicalBall_Update, ChemicalBall_LateUpdate,
                 ChemicalBall_StaticUpdate, ChemicalBall_Draw, ChemicalBall_Create, ChemicalBall_StageLoad, ChemicalBall_EditorDraw,
                 ChemicalBall_EditorLoad, ChemicalBall_Serialize);
    RSDK.CreateObject((Object**)&ChemicalPool, "ChemicalPool", sizeof(EntityChemicalPool), sizeof(ObjectChemicalPool), ChemicalPool_Update, ChemicalPool_LateUpdate,
                 ChemicalPool_StaticUpdate, ChemicalPool_Draw, ChemicalPool_Create, ChemicalPool_StageLoad, ChemicalPool_EditorDraw,
                 ChemicalPool_EditorLoad, ChemicalPool_Serialize);
    RSDK.CreateObject((Object**)&Chopper, "Chopper", sizeof(EntityChopper), sizeof(ObjectChopper), Chopper_Update, Chopper_LateUpdate, Chopper_StaticUpdate,
                 Chopper_Draw, Chopper_Create, Chopper_StageLoad, Chopper_EditorDraw, Chopper_EditorLoad, Chopper_Serialize);
    RSDK.CreateObject((Object**)&CircleBumper, "CircleBumper", sizeof(EntityCircleBumper), sizeof(ObjectCircleBumper), CircleBumper_Update, CircleBumper_LateUpdate,
                 CircleBumper_StaticUpdate, CircleBumper_Draw, CircleBumper_Create, CircleBumper_StageLoad, CircleBumper_EditorDraw,
                 CircleBumper_EditorLoad, CircleBumper_Serialize);
    RSDK.CreateObject((Object**)&Clapperboard, "Clapperboard", sizeof(EntityClapperboard), sizeof(ObjectClapperboard), Clapperboard_Update, Clapperboard_LateUpdate,
                 Clapperboard_StaticUpdate, Clapperboard_Draw, Clapperboard_Create, Clapperboard_StageLoad, Clapperboard_EditorDraw,
                 Clapperboard_EditorLoad, Clapperboard_Serialize);
    RSDK.CreateObject((Object**)&Clucker, "Clucker", sizeof(EntityClucker), sizeof(ObjectClucker), Clucker_Update, Clucker_LateUpdate, Clucker_StaticUpdate,
                 Clucker_Draw, Clucker_Create, Clucker_StageLoad, Clucker_EditorDraw, Clucker_EditorLoad, Clucker_Serialize);
    RSDK.CreateObject((Object**)&CollapsingPlatform, "CollapsingPlatform", sizeof(EntityCollapsingPlatform), sizeof(ObjectCollapsingPlatform),
                 CollapsingPlatform_Update, CollapsingPlatform_LateUpdate, CollapsingPlatform_StaticUpdate, CollapsingPlatform_Draw,
                 CollapsingPlatform_Create, CollapsingPlatform_StageLoad, CollapsingPlatform_EditorDraw, CollapsingPlatform_EditorLoad,
                 CollapsingPlatform_Serialize);
    RSDK.CreateObject((Object**)&CollapsingSand, "CollapsingSand", sizeof(EntityCollapsingSand), sizeof(ObjectCollapsingSand), CollapsingSand_Update,
                 CollapsingSand_LateUpdate, CollapsingSand_StaticUpdate, CollapsingSand_Draw, CollapsingSand_Create, CollapsingSand_StageLoad,
                 CollapsingSand_EditorDraw, CollapsingSand_EditorLoad, CollapsingSand_Serialize);
    RSDK.CreateObject((Object**)&Competition, "Competition", sizeof(EntityCompetition), sizeof(ObjectCompetition), Competition_Update, Competition_LateUpdate,
                 Competition_StaticUpdate, Competition_Draw, Competition_Create, Competition_StageLoad, Competition_EditorDraw, Competition_EditorLoad,
                 Competition_Serialize);
    RSDK.CreateObject((Object**)&CompetitionMenu, "CompetitionMenu", sizeof(EntityCompetitionMenu), sizeof(ObjectCompetitionMenu), CompetitionMenu_Update,
                 CompetitionMenu_LateUpdate, CompetitionMenu_StaticUpdate, CompetitionMenu_Draw, CompetitionMenu_Create, CompetitionMenu_StageLoad,
                 CompetitionMenu_EditorDraw, CompetitionMenu_EditorLoad, CompetitionMenu_Serialize);
    RSDK.CreateObject((Object**)&Constellation, "Constellation", sizeof(EntityConstellation), sizeof(ObjectConstellation), Constellation_Update,
                 Constellation_LateUpdate, Constellation_StaticUpdate, Constellation_Draw, Constellation_Create, Constellation_StageLoad,
                 Constellation_EditorDraw, Constellation_EditorLoad, Constellation_Serialize);
    RSDK.CreateObject((Object**)&ContinuePlayer, "ContinuePlayer", sizeof(EntityContinuePlayer), sizeof(ObjectContinuePlayer), ContinuePlayer_Update,
                 ContinuePlayer_LateUpdate, ContinuePlayer_StaticUpdate, ContinuePlayer_Draw, ContinuePlayer_Create, ContinuePlayer_StageLoad,
                 ContinuePlayer_EditorDraw, ContinuePlayer_EditorLoad, ContinuePlayer_Serialize);
    RSDK.CreateObject((Object**)&ContinueSetup, "ContinueSetup", sizeof(EntityContinueSetup), sizeof(ObjectContinueSetup), ContinueSetup_Update,
                 ContinueSetup_LateUpdate, ContinueSetup_StaticUpdate, ContinueSetup_Draw, ContinueSetup_Create, ContinueSetup_StageLoad,
                 ContinueSetup_EditorDraw, ContinueSetup_EditorLoad, ContinueSetup_Serialize);
    RSDK.CreateObject((Object**)&ConveyorBelt, "ConveyorBelt", sizeof(EntityConveyorBelt), sizeof(ObjectConveyorBelt), ConveyorBelt_Update, ConveyorBelt_LateUpdate,
                 ConveyorBelt_StaticUpdate, ConveyorBelt_Draw, ConveyorBelt_Create, ConveyorBelt_StageLoad, ConveyorBelt_EditorDraw,
                 ConveyorBelt_EditorLoad, ConveyorBelt_Serialize);
    RSDK.CreateObject((Object**)&ConveyorPlatform, "ConveyorPlatform", sizeof(EntityConveyorPlatform), sizeof(ObjectConveyorPlatform), ConveyorPlatform_Update,
                 ConveyorPlatform_LateUpdate, ConveyorPlatform_StaticUpdate, ConveyorPlatform_Draw, ConveyorPlatform_Create,
                 ConveyorPlatform_StageLoad, ConveyorPlatform_EditorDraw, ConveyorPlatform_EditorLoad, ConveyorPlatform_Serialize);
    RSDK.CreateObject((Object**)&ConveyorWheel, "ConveyorWheel", sizeof(EntityConveyorWheel), sizeof(ObjectConveyorWheel), ConveyorWheel_Update,
                 ConveyorWheel_LateUpdate, ConveyorWheel_StaticUpdate, ConveyorWheel_Draw, ConveyorWheel_Create, ConveyorWheel_StageLoad,
                 ConveyorWheel_EditorDraw, ConveyorWheel_EditorLoad, ConveyorWheel_Serialize);
    RSDK.CreateObject((Object**)&CorkscrewPath, "CorkscrewPath", sizeof(EntityCorkscrewPath), sizeof(ObjectCorkscrewPath), CorkscrewPath_Update,
                 CorkscrewPath_LateUpdate, CorkscrewPath_StaticUpdate, CorkscrewPath_Draw, CorkscrewPath_Create, CorkscrewPath_StageLoad,
                 CorkscrewPath_EditorDraw, CorkscrewPath_EditorLoad, CorkscrewPath_Serialize);
    RSDK.CreateObject((Object**)&COverlay, "COverlay", sizeof(EntityCOverlay), sizeof(ObjectCOverlay), COverlay_Update, COverlay_LateUpdate, COverlay_StaticUpdate,
                 COverlay_Draw, COverlay_Create, COverlay_StageLoad, COverlay_EditorDraw, COverlay_EditorLoad, COverlay_Serialize);
    RSDK.CreateObject((Object**)&CPZ1Intro, "CPZ1Intro", sizeof(EntityCPZ1Intro), sizeof(ObjectCPZ1Intro), CPZ1Intro_Update, CPZ1Intro_LateUpdate,
                 CPZ1Intro_StaticUpdate, CPZ1Intro_Draw, CPZ1Intro_Create, CPZ1Intro_StageLoad, CPZ1Intro_EditorDraw, CPZ1Intro_EditorLoad,
                 CPZ1Intro_Serialize);
    RSDK.CreateObject((Object**)&CPZ2Outro, "CPZ2Outro", sizeof(EntityCPZ2Outro), sizeof(ObjectCPZ2Outro), CPZ2Outro_Update, CPZ2Outro_LateUpdate,
                 CPZ2Outro_StaticUpdate, CPZ2Outro_Draw, CPZ2Outro_Create, CPZ2Outro_StageLoad, CPZ2Outro_EditorDraw, CPZ2Outro_EditorLoad,
                 CPZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&CPZBoss, "CPZBoss", sizeof(EntityCPZBoss), sizeof(ObjectCPZBoss), CPZBoss_Update, CPZBoss_LateUpdate, CPZBoss_StaticUpdate,
                 CPZBoss_Draw, CPZBoss_Create, CPZBoss_StageLoad, CPZBoss_EditorDraw, CPZBoss_EditorLoad, CPZBoss_Serialize);
    RSDK.CreateObject((Object**)&CPZSetup, "CPZSetup", sizeof(EntityCPZSetup), sizeof(ObjectCPZSetup), CPZSetup_Update, CPZSetup_LateUpdate, CPZSetup_StaticUpdate,
                 CPZSetup_Draw, CPZSetup_Create, CPZSetup_StageLoad, CPZSetup_EditorDraw, CPZSetup_EditorLoad, CPZSetup_Serialize);
    RSDK.CreateObject((Object**)&CPZShutter, "CPZShutter", sizeof(EntityCPZShutter), sizeof(ObjectCPZShutter), CPZShutter_Update, CPZShutter_LateUpdate,
                 CPZShutter_StaticUpdate, CPZShutter_Draw, CPZShutter_Create, CPZShutter_StageLoad, CPZShutter_EditorDraw, CPZShutter_EditorLoad,
                 CPZShutter_Serialize);
    RSDK.CreateObject((Object**)&Crabmeat, "Crabmeat", sizeof(EntityCrabmeat), sizeof(ObjectCrabmeat), Crabmeat_Update, Crabmeat_LateUpdate, Crabmeat_StaticUpdate,
                 Crabmeat_Draw, Crabmeat_Create, Crabmeat_StageLoad, Crabmeat_EditorDraw, Crabmeat_EditorLoad, Crabmeat_Serialize);
    RSDK.CreateObject((Object**)&Crane, "Crane", sizeof(EntityCrane), sizeof(ObjectCrane), Crane_Update, Crane_LateUpdate, Crane_StaticUpdate, Crane_Draw,
                 Crane_Create, Crane_StageLoad, Crane_EditorDraw, Crane_EditorLoad, Crane_Serialize);
    RSDK.CreateObject((Object**)&CrashTest, "CrashTest", sizeof(EntityCrashTest), sizeof(ObjectCrashTest), CrashTest_Update, CrashTest_LateUpdate,
                 CrashTest_StaticUpdate, CrashTest_Draw, CrashTest_Create, CrashTest_StageLoad, CrashTest_EditorDraw, CrashTest_EditorLoad,
                 CrashTest_Serialize);
    RSDK.CreateObject((Object**)&Crate, "Crate", sizeof(EntityCrate), sizeof(ObjectCrate), Crate_Update, Crate_LateUpdate, Crate_StaticUpdate, Crate_Draw,
                 Crate_Create, Crate_StageLoad, Crate_EditorDraw, Crate_EditorLoad, Crate_Serialize);
    RSDK.CreateObject((Object**)&CreditsSetup, "CreditsSetup", sizeof(EntityCreditsSetup), sizeof(ObjectCreditsSetup), CreditsSetup_Update, CreditsSetup_LateUpdate,
                 CreditsSetup_StaticUpdate, CreditsSetup_Draw, CreditsSetup_Create, CreditsSetup_StageLoad, CreditsSetup_EditorDraw,
                 CreditsSetup_EditorLoad, CreditsSetup_Serialize);
    RSDK.CreateObject((Object**)&CrimsonEye, "CrimsonEye", sizeof(EntityCrimsonEye), sizeof(ObjectCrimsonEye), CrimsonEye_Update, CrimsonEye_LateUpdate,
                 CrimsonEye_StaticUpdate, CrimsonEye_Draw, CrimsonEye_Create, CrimsonEye_StageLoad, CrimsonEye_EditorDraw, CrimsonEye_EditorLoad,
                 CrimsonEye_Serialize);
    RSDK.CreateObject((Object**)&Current, "Current", sizeof(EntityCurrent), sizeof(ObjectCurrent), Current_Update, Current_LateUpdate, Current_StaticUpdate,
                 Current_Draw, Current_Create, Current_StageLoad, Current_EditorDraw, Current_EditorLoad, Current_Serialize);
    RSDK.CreateObject((Object**)&CutsceneHBH, "CutsceneHBH", sizeof(EntityCutsceneHBH), sizeof(ObjectCutsceneHBH), CutsceneHBH_Update, CutsceneHBH_LateUpdate,
                 CutsceneHBH_StaticUpdate, CutsceneHBH_Draw, CutsceneHBH_Create, CutsceneHBH_StageLoad, CutsceneHBH_EditorDraw, CutsceneHBH_EditorLoad,
                 CutsceneHBH_Serialize);
    RSDK.CreateObject((Object**)&CutsceneSeq, "CutsceneSeq", sizeof(EntityCutsceneSeq), sizeof(ObjectCutsceneSeq), CutsceneSeq_Update, CutsceneSeq_LateUpdate,
                 CutsceneSeq_StaticUpdate, CutsceneSeq_Draw, CutsceneSeq_Create, CutsceneSeq_StageLoad, CutsceneSeq_EditorDraw, CutsceneSeq_EditorLoad,
                 CutsceneSeq_Serialize);
    RSDK.CreateObject((Object**)&Cylinder, "Cylinder", sizeof(EntityCylinder), sizeof(ObjectCylinder), Cylinder_Update, Cylinder_LateUpdate, Cylinder_StaticUpdate,
                 Cylinder_Draw, Cylinder_Create, Cylinder_StageLoad, Cylinder_EditorDraw, Cylinder_EditorLoad, Cylinder_Serialize);
    RSDK.CreateObject((Object**)&DAControl, "DAControl", sizeof(EntityDAControl), sizeof(ObjectDAControl), DAControl_Update, DAControl_LateUpdate,
                 DAControl_StaticUpdate, DAControl_Draw, DAControl_Create, DAControl_StageLoad, DAControl_EditorDraw, DAControl_EditorLoad,
                 DAControl_Serialize);
    RSDK.CreateObject((Object**)&Dango, "Dango", sizeof(EntityDango), sizeof(ObjectDango), Dango_Update, Dango_LateUpdate, Dango_StaticUpdate, Dango_Draw,
                 Dango_Create, Dango_StageLoad, Dango_EditorDraw, Dango_EditorLoad, Dango_Serialize);
    RSDK.CreateObject((Object**)&DASetup, "DASetup", sizeof(EntityDASetup), sizeof(ObjectDASetup), DASetup_Update, DASetup_LateUpdate, DASetup_StaticUpdate,
                 DASetup_Draw, DASetup_Create, DASetup_StageLoad, DASetup_EditorDraw, DASetup_EditorLoad, DASetup_Serialize);
    RSDK.CreateObject((Object**)&DashLift, "DashLift", sizeof(EntityDashLift), sizeof(ObjectDashLift), DashLift_Update, DashLift_LateUpdate, DashLift_StaticUpdate,
                 DashLift_Draw, DashLift_Create, DashLift_StageLoad, DashLift_EditorDraw, DashLift_EditorLoad, DashLift_Serialize);
    RSDK.CreateObject((Object**)&DBTower, "DBTower", sizeof(EntityDBTower), sizeof(ObjectDBTower), DBTower_Update, DBTower_LateUpdate, DBTower_StaticUpdate,
                 DBTower_Draw, DBTower_Create, DBTower_StageLoad, DBTower_EditorDraw, DBTower_EditorLoad, DBTower_Serialize);
    RSDK.CreateObject((Object**)&DCEvent, "DCEvent", sizeof(EntityDCEvent), sizeof(ObjectDCEvent), DCEvent_Update, DCEvent_LateUpdate, DCEvent_StaticUpdate,
                 DCEvent_Draw, DCEvent_Create, DCEvent_StageLoad, DCEvent_EditorDraw, DCEvent_EditorLoad, DCEvent_Serialize);
    RSDK.CreateObject((Object**)&DDWrecker, "DDWrecker", sizeof(EntityDDWrecker), sizeof(ObjectDDWrecker), DDWrecker_Update, DDWrecker_LateUpdate,
                 DDWrecker_StaticUpdate, DDWrecker_Draw, DDWrecker_Create, DDWrecker_StageLoad, DDWrecker_EditorDraw, DDWrecker_EditorLoad,
                 DDWrecker_Serialize);
    RSDK.CreateObject((Object**)&Debris, "Debris", sizeof(EntityDebris), sizeof(ObjectDebris), Debris_Update, Debris_LateUpdate, Debris_StaticUpdate, Debris_Draw,
                 Debris_Create, Debris_StageLoad, Debris_EditorDraw, Debris_EditorLoad, Debris_Serialize);
    RSDK.CreateObject((Object**)&DebugMode, "DebugMode", sizeof(EntityDebugMode), sizeof(ObjectDebugMode), DebugMode_Update, DebugMode_LateUpdate,
                 DebugMode_StaticUpdate, DebugMode_Draw, DebugMode_Create, DebugMode_StageLoad, DebugMode_EditorDraw, DebugMode_EditorLoad,
                 DebugMode_Serialize);
    RSDK.CreateObject((Object**)&Decoration, "Decoration", sizeof(EntityDecoration), sizeof(ObjectDecoration), Decoration_Update, Decoration_LateUpdate,
                 Decoration_StaticUpdate, Decoration_Draw, Decoration_Create, Decoration_StageLoad, Decoration_EditorDraw, Decoration_EditorLoad,
                 Decoration_Serialize);
    RSDK.CreateObject((Object**)&DERobot, "DERobot", sizeof(EntityDERobot), sizeof(ObjectDERobot), DERobot_Update, DERobot_LateUpdate, DERobot_StaticUpdate,
                 DERobot_Draw, DERobot_Create, DERobot_StageLoad, DERobot_EditorDraw, DERobot_EditorLoad, DERobot_Serialize);
    RSDK.CreateObject((Object**)&DialogRunner, "DialogRunner", sizeof(EntityDialogRunner), sizeof(ObjectDialogRunner), DialogRunner_Update, DialogRunner_LateUpdate,
                 DialogRunner_StaticUpdate, DialogRunner_Draw, DialogRunner_Create, DialogRunner_StageLoad, DialogRunner_EditorDraw,
                 DialogRunner_EditorLoad, DialogRunner_Serialize);
    RSDK.CreateObject((Object**)&DirectorChair, "DirectorChair", sizeof(EntityDirectorChair), sizeof(ObjectDirectorChair), DirectorChair_Update,
                 DirectorChair_LateUpdate, DirectorChair_StaticUpdate, DirectorChair_Draw, DirectorChair_Create, DirectorChair_StageLoad,
                 DirectorChair_EditorDraw, DirectorChair_EditorLoad, DirectorChair_Serialize);
    RSDK.CreateObject((Object**)&DiveEggman, "DiveEggman", sizeof(EntityDiveEggman), sizeof(ObjectDiveEggman), DiveEggman_Update, DiveEggman_LateUpdate,
                 DiveEggman_StaticUpdate, DiveEggman_Draw, DiveEggman_Create, DiveEggman_StageLoad, DiveEggman_EditorDraw, DiveEggman_EditorLoad,
                 DiveEggman_Serialize);
    RSDK.CreateObject((Object**)&DNARiser, "DNARiser", sizeof(EntityDNARiser), sizeof(ObjectDNARiser), DNARiser_Update, DNARiser_LateUpdate, DNARiser_StaticUpdate,
                 DNARiser_Draw, DNARiser_Create, DNARiser_StageLoad, DNARiser_EditorDraw, DNARiser_EditorLoad, DNARiser_Serialize);
    RSDK.CreateObject((Object**)&DoorTrigger, "DoorTrigger", sizeof(EntityDoorTrigger), sizeof(ObjectDoorTrigger), DoorTrigger_Update, DoorTrigger_LateUpdate,
                 DoorTrigger_StaticUpdate, DoorTrigger_Draw, DoorTrigger_Create, DoorTrigger_StageLoad, DoorTrigger_EditorDraw, DoorTrigger_EditorLoad,
                 DoorTrigger_Serialize);
    RSDK.CreateObject((Object**)&Dragonfly, "Dragonfly", sizeof(EntityDragonfly), sizeof(ObjectDragonfly), Dragonfly_Update, Dragonfly_LateUpdate,
                 Dragonfly_StaticUpdate, Dragonfly_Draw, Dragonfly_Create, Dragonfly_StageLoad, Dragonfly_EditorDraw, Dragonfly_EditorLoad,
                 Dragonfly_Serialize);
    RSDK.CreateObject((Object**)&Drillerdroid, "Drillerdroid", sizeof(EntityDrillerdroid), sizeof(ObjectDrillerdroid), Drillerdroid_Update, Drillerdroid_LateUpdate,
                 Drillerdroid_StaticUpdate, Drillerdroid_Draw, Drillerdroid_Create, Drillerdroid_StageLoad, Drillerdroid_EditorDraw,
                 Drillerdroid_EditorLoad, Drillerdroid_Serialize);
    RSDK.CreateObject((Object**)&DrillerdroidO, "DrillerdroidO", sizeof(EntityDrillerdroidO), sizeof(ObjectDrillerdroidO), DrillerdroidO_Update,
                 DrillerdroidO_LateUpdate, DrillerdroidO_StaticUpdate, DrillerdroidO_Draw, DrillerdroidO_Create, DrillerdroidO_StageLoad,
                 DrillerdroidO_EditorDraw, DrillerdroidO_EditorLoad, DrillerdroidO_Serialize);
    RSDK.CreateObject((Object**)&Dust, "Dust", sizeof(EntityDust), sizeof(ObjectDust), Dust_Update, Dust_LateUpdate, Dust_StaticUpdate, Dust_Draw, Dust_Create,
                 Dust_StageLoad, Dust_EditorDraw, Dust_EditorLoad, Dust_Serialize);
    RSDK.CreateObject((Object**)&EggLoco, "EggLoco", sizeof(EntityEggLoco), sizeof(ObjectEggLoco), EggLoco_Update, EggLoco_LateUpdate, EggLoco_StaticUpdate,
                 EggLoco_Draw, EggLoco_Create, EggLoco_StageLoad, EggLoco_EditorDraw, EggLoco_EditorLoad, EggLoco_Serialize);
    RSDK.CreateObject((Object**)&Eggman, "Eggman", sizeof(EntityEggman), sizeof(ObjectEggman), Eggman_Update, Eggman_LateUpdate, Eggman_StaticUpdate, Eggman_Draw,
                 Eggman_Create, Eggman_StageLoad, Eggman_EditorDraw, Eggman_EditorLoad, Eggman_Serialize);
    RSDK.CreateObject((Object**)&EggPistonsMKII, "EggPistonsMKII", sizeof(EntityEggPistonsMKII), sizeof(ObjectEggPistonsMKII), EggPistonsMKII_Update,
                 EggPistonsMKII_LateUpdate, EggPistonsMKII_StaticUpdate, EggPistonsMKII_Draw, EggPistonsMKII_Create, EggPistonsMKII_StageLoad,
                 EggPistonsMKII_EditorDraw, EggPistonsMKII_EditorLoad, EggPistonsMKII_Serialize);
    RSDK.CreateObject((Object**)&EggPrison, "EggPrison", sizeof(EntityEggPrison), sizeof(ObjectEggPrison), EggPrison_Update, EggPrison_LateUpdate,
                 EggPrison_StaticUpdate, EggPrison_Draw, EggPrison_Create, EggPrison_StageLoad, EggPrison_EditorDraw, EggPrison_EditorLoad,
                 EggPrison_Serialize);
    RSDK.CreateObject((Object**)&EggTower, "EggTower", sizeof(EntityEggTower), sizeof(ObjectEggTower), EggTower_Update, EggTower_LateUpdate, EggTower_StaticUpdate,
                 EggTower_Draw, EggTower_Create, EggTower_StageLoad, EggTower_EditorDraw, EggTower_EditorLoad, EggTower_Serialize);
    RSDK.CreateObject((Object**)&EggTV, "EggTV", sizeof(EntityEggTV), sizeof(ObjectEggTV), EggTV_Update, EggTV_LateUpdate, EggTV_StaticUpdate, EggTV_Draw,
                 EggTV_Create, EggTV_StageLoad, EggTV_EditorDraw, EggTV_EditorLoad, EggTV_Serialize);
    RSDK.CreateObject((Object**)&ElectroMagnet, "ElectroMagnet", sizeof(EntityElectroMagnet), sizeof(ObjectElectroMagnet), ElectroMagnet_Update,
                 ElectroMagnet_LateUpdate, ElectroMagnet_StaticUpdate, ElectroMagnet_Draw, ElectroMagnet_Create, ElectroMagnet_StageLoad,
                 ElectroMagnet_EditorDraw, ElectroMagnet_EditorLoad, ElectroMagnet_Serialize);
    RSDK.CreateObject((Object**)&EncoreGoodEnd, "EncoreGoodEnd", sizeof(EntityEncoreGoodEnd), sizeof(ObjectEncoreGoodEnd), EncoreGoodEnd_Update,
                 EncoreGoodEnd_LateUpdate, EncoreGoodEnd_StaticUpdate, EncoreGoodEnd_Draw, EncoreGoodEnd_Create, EncoreGoodEnd_StageLoad,
                 EncoreGoodEnd_EditorDraw, EncoreGoodEnd_EditorLoad, EncoreGoodEnd_Serialize);
    RSDK.CreateObject((Object**)&EncoreIntro, "EncoreIntro", sizeof(EntityEncoreIntro), sizeof(ObjectEncoreIntro), EncoreIntro_Update, EncoreIntro_LateUpdate,
                 EncoreIntro_StaticUpdate, EncoreIntro_Draw, EncoreIntro_Create, EncoreIntro_StageLoad, EncoreIntro_EditorDraw, EncoreIntro_EditorLoad,
                 EncoreIntro_Serialize);
    RSDK.CreateObject((Object**)&EncoreRoute, "EncoreRoute", sizeof(EntityEncoreRoute), sizeof(ObjectEncoreRoute), EncoreRoute_Update, EncoreRoute_LateUpdate,
                 EncoreRoute_StaticUpdate, EncoreRoute_Draw, EncoreRoute_Create, EncoreRoute_StageLoad, EncoreRoute_EditorDraw, EncoreRoute_EditorLoad,
                 EncoreRoute_Serialize);
    RSDK.CreateObject((Object**)&ERZGunner, "ERZGunner", sizeof(EntityERZGunner), sizeof(ObjectERZGunner), ERZGunner_Update, ERZGunner_LateUpdate,
                 ERZGunner_StaticUpdate, ERZGunner_Draw, ERZGunner_Create, ERZGunner_StageLoad, ERZGunner_EditorDraw, ERZGunner_EditorLoad,
                 ERZGunner_Serialize);
    RSDK.CreateObject((Object**)&ERZKing, "ERZKing", sizeof(EntityERZKing), sizeof(ObjectERZKing), ERZKing_Update, ERZKing_LateUpdate, ERZKing_StaticUpdate,
                 ERZKing_Draw, ERZKing_Create, ERZKing_StageLoad, ERZKing_EditorDraw, ERZKing_EditorLoad, ERZKing_Serialize);
    RSDK.CreateObject((Object**)&ERZMystic, "ERZMystic", sizeof(EntityERZMystic), sizeof(ObjectERZMystic), ERZMystic_Update, ERZMystic_LateUpdate,
                 ERZMystic_StaticUpdate, ERZMystic_Draw, ERZMystic_Create, ERZMystic_StageLoad, ERZMystic_EditorDraw, ERZMystic_EditorLoad,
                 ERZMystic_Serialize);
    RSDK.CreateObject((Object**)&ERZOutro, "ERZOutro", sizeof(EntityERZOutro), sizeof(ObjectERZOutro), ERZOutro_Update, ERZOutro_LateUpdate, ERZOutro_StaticUpdate,
                 ERZOutro_Draw, ERZOutro_Create, ERZOutro_StageLoad, ERZOutro_EditorDraw, ERZOutro_EditorLoad, ERZOutro_Serialize);
    RSDK.CreateObject((Object**)&ERZRider, "ERZRider", sizeof(EntityERZRider), sizeof(ObjectERZRider), ERZRider_Update, ERZRider_LateUpdate, ERZRider_StaticUpdate,
                 ERZRider_Draw, ERZRider_Create, ERZRider_StageLoad, ERZRider_EditorDraw, ERZRider_EditorLoad, ERZRider_Serialize);
    RSDK.CreateObject((Object**)&ERZSetup, "ERZSetup", sizeof(EntityERZSetup), sizeof(ObjectERZSetup), ERZSetup_Update, ERZSetup_LateUpdate, ERZSetup_StaticUpdate,
                 ERZSetup_Draw, ERZSetup_Create, ERZSetup_StageLoad, ERZSetup_EditorDraw, ERZSetup_EditorLoad, ERZSetup_Serialize);
    RSDK.CreateObject((Object**)&ERZShinobi, "ERZShinobi", sizeof(EntityERZShinobi), sizeof(ObjectERZShinobi), ERZShinobi_Update, ERZShinobi_LateUpdate,
                 ERZShinobi_StaticUpdate, ERZShinobi_Draw, ERZShinobi_Create, ERZShinobi_StageLoad, ERZShinobi_EditorDraw, ERZShinobi_EditorLoad,
                 ERZShinobi_Serialize);
    RSDK.CreateObject((Object**)&ERZStart, "ERZStart", sizeof(EntityERZStart), sizeof(ObjectERZStart), ERZStart_Update, ERZStart_LateUpdate, ERZStart_StaticUpdate,
                 ERZStart_Draw, ERZStart_Create, ERZStart_StageLoad, ERZStart_EditorDraw, ERZStart_EditorLoad, ERZStart_Serialize);
    RSDK.CreateObject((Object**)&EscapeCar, "EscapeCar", sizeof(EntityEscapeCar), sizeof(ObjectEscapeCar), EscapeCar_Update, EscapeCar_LateUpdate,
                 EscapeCar_StaticUpdate, EscapeCar_Draw, EscapeCar_Create, EscapeCar_StageLoad, EscapeCar_EditorDraw, EscapeCar_EditorLoad,
                 EscapeCar_Serialize);
    RSDK.CreateObject((Object**)&Explosion, "Explosion", sizeof(EntityExplosion), sizeof(ObjectExplosion), Explosion_Update, Explosion_LateUpdate,
                 Explosion_StaticUpdate, Explosion_Draw, Explosion_Create, Explosion_StageLoad, Explosion_EditorDraw, Explosion_EditorLoad,
                 Explosion_Serialize);
    RSDK.CreateObject((Object**)&ExtrasMenu, "ExtrasMenu", sizeof(EntityExtrasMenu), sizeof(ObjectExtrasMenu), ExtrasMenu_Update, ExtrasMenu_LateUpdate,
                 ExtrasMenu_StaticUpdate, ExtrasMenu_Draw, ExtrasMenu_Create, ExtrasMenu_StageLoad, ExtrasMenu_EditorDraw, ExtrasMenu_EditorLoad,
                 ExtrasMenu_Serialize);
    RSDK.CreateObject((Object**)&Fan, "Fan", sizeof(EntityFan), sizeof(ObjectFan), Fan_Update, Fan_LateUpdate, Fan_StaticUpdate, Fan_Draw, Fan_Create, Fan_StageLoad,
                 Fan_EditorDraw, Fan_EditorLoad, Fan_Serialize);
    RSDK.CreateObject((Object**)&FarPlane, "FarPlane", sizeof(EntityFarPlane), sizeof(ObjectFarPlane), FarPlane_Update, FarPlane_LateUpdate, FarPlane_StaticUpdate,
                 FarPlane_Draw, FarPlane_Create, FarPlane_StageLoad, FarPlane_EditorDraw, FarPlane_EditorLoad, FarPlane_Serialize);
    RSDK.CreateObject((Object**)&FBZ1Outro, "FBZ1Outro", sizeof(EntityFBZ1Outro), sizeof(ObjectFBZ1Outro), FBZ1Outro_Update, FBZ1Outro_LateUpdate,
                 FBZ1Outro_StaticUpdate, FBZ1Outro_Draw, FBZ1Outro_Create, FBZ1Outro_StageLoad, FBZ1Outro_EditorDraw, FBZ1Outro_EditorLoad,
                 FBZ1Outro_Serialize);
    RSDK.CreateObject((Object**)&FBZ2Outro, "FBZ2Outro", sizeof(EntityFBZ2Outro), sizeof(ObjectFBZ2Outro), FBZ2Outro_Update, FBZ2Outro_LateUpdate,
                 FBZ2Outro_StaticUpdate, FBZ2Outro_Draw, FBZ2Outro_Create, FBZ2Outro_StageLoad, FBZ2Outro_EditorDraw, FBZ2Outro_EditorLoad,
                 FBZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&FBZFan, "FBZFan", sizeof(EntityFBZFan), sizeof(ObjectFBZFan), FBZFan_Update, FBZFan_LateUpdate, FBZFan_StaticUpdate, FBZFan_Draw,
                 FBZFan_Create, FBZFan_StageLoad, FBZFan_EditorDraw, FBZFan_EditorLoad, FBZFan_Serialize);
    RSDK.CreateObject((Object**)&FBZMissile, "FBZMissile", sizeof(EntityFBZMissile), sizeof(ObjectFBZMissile), FBZMissile_Update, FBZMissile_LateUpdate,
                 FBZMissile_StaticUpdate, FBZMissile_Draw, FBZMissile_Create, FBZMissile_StageLoad, FBZMissile_EditorDraw, FBZMissile_EditorLoad,
                 FBZMissile_Serialize);
    RSDK.CreateObject((Object**)&FBZSetup, "FBZSetup", sizeof(EntityFBZSetup), sizeof(ObjectFBZSetup), FBZSetup_Update, FBZSetup_LateUpdate, FBZSetup_StaticUpdate,
                 FBZSetup_Draw, FBZSetup_Create, FBZSetup_StageLoad, FBZSetup_EditorDraw, FBZSetup_EditorLoad, FBZSetup_Serialize);
    RSDK.CreateObject((Object**)&FBZSinkTrash, "FBZSinkTrash", sizeof(EntityFBZSinkTrash), sizeof(ObjectFBZSinkTrash), FBZSinkTrash_Update, FBZSinkTrash_LateUpdate,
                 FBZSinkTrash_StaticUpdate, FBZSinkTrash_Draw, FBZSinkTrash_Create, FBZSinkTrash_StageLoad, FBZSinkTrash_EditorDraw,
                 FBZSinkTrash_EditorLoad, FBZSinkTrash_Serialize);
    RSDK.CreateObject((Object**)&FBZStorm, "FBZStorm", sizeof(EntityFBZStorm), sizeof(ObjectFBZStorm), FBZStorm_Update, FBZStorm_LateUpdate, FBZStorm_StaticUpdate,
                 FBZStorm_Draw, FBZStorm_Create, FBZStorm_StageLoad, FBZStorm_EditorDraw, FBZStorm_EditorLoad, FBZStorm_Serialize);
    RSDK.CreateObject((Object**)&FBZTrash, "FBZTrash", sizeof(EntityFBZTrash), sizeof(ObjectFBZTrash), FBZTrash_Update, FBZTrash_LateUpdate, FBZTrash_StaticUpdate,
                 FBZTrash_Draw, FBZTrash_Create, FBZTrash_StageLoad, FBZTrash_EditorDraw, FBZTrash_EditorLoad, FBZTrash_Serialize);
    RSDK.CreateObject((Object**)&FernParallax, "FernParallax", sizeof(EntityFernParallax), sizeof(ObjectFernParallax), FernParallax_Update, FernParallax_LateUpdate,
                 FernParallax_StaticUpdate, FernParallax_Draw, FernParallax_Create, FernParallax_StageLoad, FernParallax_EditorDraw,
                 FernParallax_EditorLoad, FernParallax_Serialize);
    RSDK.CreateObject((Object**)&FilmProjector, "FilmProjector", sizeof(EntityFilmProjector), sizeof(ObjectFilmProjector), FilmProjector_Update,
                 FilmProjector_LateUpdate, FilmProjector_StaticUpdate, FilmProjector_Draw, FilmProjector_Create, FilmProjector_StageLoad,
                 FilmProjector_EditorDraw, FilmProjector_EditorLoad, FilmProjector_Serialize);
    RSDK.CreateObject((Object**)&FilmReel, "FilmReel", sizeof(EntityFilmReel), sizeof(ObjectFilmReel), FilmReel_Update, FilmReel_LateUpdate, FilmReel_StaticUpdate,
                 FilmReel_Draw, FilmReel_Create, FilmReel_StageLoad, FilmReel_EditorDraw, FilmReel_EditorLoad, FilmReel_Serialize);
    RSDK.CreateObject((Object**)&Fireflies, "Fireflies", sizeof(EntityFireflies), sizeof(ObjectFireflies), Fireflies_Update, Fireflies_LateUpdate,
                 Fireflies_StaticUpdate, Fireflies_Draw, Fireflies_Create, Fireflies_StageLoad, Fireflies_EditorDraw, Fireflies_EditorLoad,
                 Fireflies_Serialize);
    RSDK.CreateObject((Object**)&Firework, "Firework", sizeof(EntityFirework), sizeof(ObjectFirework), Firework_Update, Firework_LateUpdate, Firework_StaticUpdate,
                 Firework_Draw, Firework_Create, Firework_StageLoad, Firework_EditorDraw, Firework_EditorLoad, Firework_Serialize);
    RSDK.CreateObject((Object**)&Fireworm, "Fireworm", sizeof(EntityFireworm), sizeof(ObjectFireworm), Fireworm_Update, Fireworm_LateUpdate, Fireworm_StaticUpdate,
                 Fireworm_Draw, Fireworm_Create, Fireworm_StageLoad, Fireworm_EditorDraw, Fireworm_EditorLoad, Fireworm_Serialize);
    RSDK.CreateObject((Object**)&FlameSpring, "FlameSpring", sizeof(EntityFlameSpring), sizeof(ObjectFlameSpring), FlameSpring_Update, FlameSpring_LateUpdate,
                 FlameSpring_StaticUpdate, FlameSpring_Draw, FlameSpring_Create, FlameSpring_StageLoad, FlameSpring_EditorDraw, FlameSpring_EditorLoad,
                 FlameSpring_Serialize);
    RSDK.CreateObject((Object**)&Flamethrower, "Flamethrower", sizeof(EntityFlamethrower), sizeof(ObjectFlamethrower), Flamethrower_Update, Flamethrower_LateUpdate,
                 Flamethrower_StaticUpdate, Flamethrower_Draw, Flamethrower_Create, Flamethrower_StageLoad, Flamethrower_EditorDraw,
                 Flamethrower_EditorLoad, Flamethrower_Serialize);
    RSDK.CreateObject((Object**)&FlasherMKII, "FlasherMKII", sizeof(EntityFlasherMKII), sizeof(ObjectFlasherMKII), FlasherMKII_Update, FlasherMKII_LateUpdate,
                 FlasherMKII_StaticUpdate, FlasherMKII_Draw, FlasherMKII_Create, FlasherMKII_StageLoad, FlasherMKII_EditorDraw, FlasherMKII_EditorLoad,
                 FlasherMKII_Serialize);
    RSDK.CreateObject((Object**)&FlingRamp, "FlingRamp", sizeof(EntityFlingRamp), sizeof(ObjectFlingRamp), FlingRamp_Update, FlingRamp_LateUpdate,
                 FlingRamp_StaticUpdate, FlingRamp_Draw, FlingRamp_Create, FlingRamp_StageLoad, FlingRamp_EditorDraw, FlingRamp_EditorLoad,
                 FlingRamp_Serialize);
    RSDK.CreateObject((Object**)&Flipper, "Flipper", sizeof(EntityFlipper), sizeof(ObjectFlipper), Flipper_Update, Flipper_LateUpdate, Flipper_StaticUpdate,
                 Flipper_Draw, Flipper_Create, Flipper_StageLoad, Flipper_EditorDraw, Flipper_EditorLoad, Flipper_Serialize);
    RSDK.CreateObject((Object**)&FlowerPod, "FlowerPod", sizeof(EntityFlowerPod), sizeof(ObjectFlowerPod), FlowerPod_Update, FlowerPod_LateUpdate,
                 FlowerPod_StaticUpdate, FlowerPod_Draw, FlowerPod_Create, FlowerPod_StageLoad, FlowerPod_EditorDraw, FlowerPod_EditorLoad,
                 FlowerPod_Serialize);
    RSDK.CreateObject((Object**)&FoldingPlatform, "FoldingPlatform", sizeof(EntityFoldingPlatform), sizeof(ObjectFoldingPlatform), FoldingPlatform_Update,
                 FoldingPlatform_LateUpdate, FoldingPlatform_StaticUpdate, FoldingPlatform_Draw, FoldingPlatform_Create, FoldingPlatform_StageLoad,
                 FoldingPlatform_EditorDraw, FoldingPlatform_EditorLoad, FoldingPlatform_Serialize);
    RSDK.CreateObject((Object**)&ForceSpin, "ForceSpin", sizeof(EntityForceSpin), sizeof(ObjectForceSpin), ForceSpin_Update, ForceSpin_LateUpdate,
                 ForceSpin_StaticUpdate, ForceSpin_Draw, ForceSpin_Create, ForceSpin_StageLoad, ForceSpin_EditorDraw, ForceSpin_EditorLoad,
                 ForceSpin_Serialize);
    RSDK.CreateObject((Object**)&ForceUnstick, "ForceUnstick", sizeof(EntityForceUnstick), sizeof(ObjectForceUnstick), ForceUnstick_Update, ForceUnstick_LateUpdate,
                 ForceUnstick_StaticUpdate, ForceUnstick_Draw, ForceUnstick_Create, ForceUnstick_StageLoad, ForceUnstick_EditorDraw,
                 ForceUnstick_EditorLoad, ForceUnstick_Serialize);
    RSDK.CreateObject((Object**)&FrostThrower, "FrostThrower", sizeof(EntityFrostThrower), sizeof(ObjectFrostThrower), FrostThrower_Update, FrostThrower_LateUpdate,
                 FrostThrower_StaticUpdate, FrostThrower_Draw, FrostThrower_Create, FrostThrower_StageLoad, FrostThrower_EditorDraw,
                 FrostThrower_EditorLoad, FrostThrower_Serialize);
    RSDK.CreateObject((Object**)&Funnel, "Funnel", sizeof(EntityFunnel), sizeof(ObjectFunnel), Funnel_Update, Funnel_LateUpdate, Funnel_StaticUpdate, Funnel_Draw,
                 Funnel_Create, Funnel_StageLoad, Funnel_EditorDraw, Funnel_EditorLoad, Funnel_Serialize);
    RSDK.CreateObject((Object**)&FXExpandRing, "FXExpandRing", sizeof(EntityFXExpandRing), sizeof(ObjectFXExpandRing), FXExpandRing_Update, FXExpandRing_LateUpdate,
                 FXExpandRing_StaticUpdate, FXExpandRing_Draw, FXExpandRing_Create, FXExpandRing_StageLoad, FXExpandRing_EditorDraw,
                 FXExpandRing_EditorLoad, FXExpandRing_Serialize);
    RSDK.CreateObject((Object**)&FXFade, "FXFade", sizeof(EntityFXFade), sizeof(ObjectFXFade), FXFade_Update, FXFade_LateUpdate, FXFade_StaticUpdate, FXFade_Draw,
                 FXFade_Create, FXFade_StageLoad, FXFade_EditorDraw, FXFade_EditorLoad, FXFade_Serialize);
    RSDK.CreateObject((Object**)&FXRuby, "FXRuby", sizeof(EntityFXRuby), sizeof(ObjectFXRuby), FXRuby_Update, FXRuby_LateUpdate, FXRuby_StaticUpdate, FXRuby_Draw,
                 FXRuby_Create, FXRuby_StageLoad, FXRuby_EditorDraw, FXRuby_EditorLoad, FXRuby_Serialize);
    RSDK.CreateObject((Object**)&FXSpinRay, "FXSpinRay", sizeof(EntityFXSpinRay), sizeof(ObjectFXSpinRay), FXSpinRay_Update, FXSpinRay_LateUpdate,
                 FXSpinRay_StaticUpdate, FXSpinRay_Draw, FXSpinRay_Create, FXSpinRay_StageLoad, FXSpinRay_EditorDraw, FXSpinRay_EditorLoad,
                 FXSpinRay_Serialize);
    RSDK.CreateObject((Object**)&FXTrail, "FXTrail", sizeof(EntityFXTrail), sizeof(ObjectFXTrail), FXTrail_Update, FXTrail_LateUpdate, FXTrail_StaticUpdate,
                 FXTrail_Draw, FXTrail_Create, FXTrail_StageLoad, FXTrail_EditorDraw, FXTrail_EditorLoad, FXTrail_Serialize);
    RSDK.CreateObject((Object**)&FXWaveRing, "FXWaveRing", sizeof(EntityFXWaveRing), sizeof(ObjectFXWaveRing), FXWaveRing_Update, FXWaveRing_LateUpdate,
                 FXWaveRing_StaticUpdate, FXWaveRing_Draw, FXWaveRing_Create, FXWaveRing_StageLoad, FXWaveRing_EditorDraw, FXWaveRing_EditorLoad,
                 FXWaveRing_Serialize);
    RSDK.CreateObject((Object**)&Gachapandora, "Gachapandora", sizeof(EntityGachapandora), sizeof(ObjectGachapandora), Gachapandora_Update, Gachapandora_LateUpdate,
                 Gachapandora_StaticUpdate, Gachapandora_Draw, Gachapandora_Create, Gachapandora_StageLoad, Gachapandora_EditorDraw,
                 Gachapandora_EditorLoad, Gachapandora_Serialize);
    RSDK.CreateObject((Object**)&GameOver, "GameOver", sizeof(EntityGameOver), sizeof(ObjectGameOver), GameOver_Update, GameOver_LateUpdate, GameOver_StaticUpdate,
                 GameOver_Draw, GameOver_Create, GameOver_StageLoad, GameOver_EditorDraw, GameOver_EditorLoad, GameOver_Serialize);
    RSDK.CreateObjectContainer((Object **)&GameProgress, "GameProgress", sizeof(ObjectGameProgress));
    RSDK.CreateObject((Object**)&GasPlatform, "GasPlatform", sizeof(EntityGasPlatform), sizeof(ObjectGasPlatform), GasPlatform_Update, GasPlatform_LateUpdate,
                 GasPlatform_StaticUpdate, GasPlatform_Draw, GasPlatform_Create, GasPlatform_StageLoad, GasPlatform_EditorDraw, GasPlatform_EditorLoad,
                 GasPlatform_Serialize);
    RSDK.CreateObject((Object**)&GenericTrigger, "GenericTrigger", sizeof(EntityGenericTrigger), sizeof(ObjectGenericTrigger), GenericTrigger_Update,
                 GenericTrigger_LateUpdate, GenericTrigger_StaticUpdate, GenericTrigger_Draw, GenericTrigger_Create, GenericTrigger_StageLoad,
                 GenericTrigger_EditorDraw, GenericTrigger_EditorLoad, GenericTrigger_Serialize);
    RSDK.CreateObject((Object**)&GHZ2Outro, "GHZ2Outro", sizeof(EntityGHZ2Outro), sizeof(ObjectGHZ2Outro), GHZ2Outro_Update, GHZ2Outro_LateUpdate,
                 GHZ2Outro_StaticUpdate, GHZ2Outro_Draw, GHZ2Outro_Create, GHZ2Outro_StageLoad, GHZ2Outro_EditorDraw, GHZ2Outro_EditorLoad,
                 GHZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&GHZCutsceneK, "GHZCutsceneK", sizeof(EntityGHZCutsceneK), sizeof(ObjectGHZCutsceneK), GHZCutsceneK_Update, GHZCutsceneK_LateUpdate,
                 GHZCutsceneK_StaticUpdate, GHZCutsceneK_Draw, GHZCutsceneK_Create, GHZCutsceneK_StageLoad, GHZCutsceneK_EditorDraw,
                 GHZCutsceneK_EditorLoad, GHZCutsceneK_Serialize);
    RSDK.CreateObject((Object**)&GHZCutsceneST, "GHZCutsceneST", sizeof(EntityGHZCutsceneST), sizeof(ObjectGHZCutsceneST), GHZCutsceneST_Update,
                 GHZCutsceneST_LateUpdate, GHZCutsceneST_StaticUpdate, GHZCutsceneST_Draw, GHZCutsceneST_Create, GHZCutsceneST_StageLoad,
                 GHZCutsceneST_EditorDraw, GHZCutsceneST_EditorLoad, GHZCutsceneST_Serialize);
    RSDK.CreateObject((Object**)&GHZSetup, "GHZSetup", sizeof(EntityGHZSetup), sizeof(ObjectGHZSetup), GHZSetup_Update, GHZSetup_LateUpdate, GHZSetup_StaticUpdate,
                 GHZSetup_Draw, GHZSetup_Create, GHZSetup_StageLoad, GHZSetup_EditorDraw, GHZSetup_EditorLoad, GHZSetup_Serialize);
    RSDK.CreateObject((Object**)&GiantPistol, "GiantPistol", sizeof(EntityGiantPistol), sizeof(ObjectGiantPistol), GiantPistol_Update, GiantPistol_LateUpdate,
                 GiantPistol_StaticUpdate, GiantPistol_Draw, GiantPistol_Create, GiantPistol_StageLoad, GiantPistol_EditorDraw, GiantPistol_EditorLoad,
                 GiantPistol_Serialize);
    RSDK.CreateObject((Object**)&GigaMetal, "GigaMetal", sizeof(EntityGigaMetal), sizeof(ObjectGigaMetal), GigaMetal_Update, GigaMetal_LateUpdate,
                 GigaMetal_StaticUpdate, GigaMetal_Draw, GigaMetal_Create, GigaMetal_StageLoad, GigaMetal_EditorDraw, GigaMetal_EditorLoad,
                 GigaMetal_Serialize);
    RSDK.CreateObject((Object**)&Gondola, "Gondola", sizeof(EntityGondola), sizeof(ObjectGondola), Gondola_Update, Gondola_LateUpdate, Gondola_StaticUpdate,
                 Gondola_Draw, Gondola_Create, Gondola_StageLoad, Gondola_EditorDraw, Gondola_EditorLoad, Gondola_Serialize);
    RSDK.CreateObject((Object**)&Grabber, "Grabber", sizeof(EntityGrabber), sizeof(ObjectGrabber), Grabber_Update, Grabber_LateUpdate, Grabber_StaticUpdate,
                 Grabber_Draw, Grabber_Create, Grabber_StageLoad, Grabber_EditorDraw, Grabber_EditorLoad, Grabber_Serialize);
    RSDK.CreateObject((Object**)&GreenScreen, "GreenScreen", sizeof(EntityGreenScreen), sizeof(ObjectGreenScreen), GreenScreen_Update, GreenScreen_LateUpdate,
                 GreenScreen_StaticUpdate, GreenScreen_Draw, GreenScreen_Create, GreenScreen_StageLoad, GreenScreen_EditorDraw, GreenScreen_EditorLoad,
                 GreenScreen_Serialize);
    RSDK.CreateObject((Object**)&GymBar, "GymBar", sizeof(EntityGymBar), sizeof(ObjectGymBar), GymBar_Update, GymBar_LateUpdate, GymBar_StaticUpdate, GymBar_Draw,
                 GymBar_Create, GymBar_StageLoad, GymBar_EditorDraw, GymBar_EditorLoad, GymBar_Serialize);
    RSDK.CreateObject((Object**)&HandLauncher, "HandLauncher", sizeof(EntityHandLauncher), sizeof(ObjectHandLauncher), HandLauncher_Update, HandLauncher_LateUpdate,
                 HandLauncher_StaticUpdate, HandLauncher_Draw, HandLauncher_Create, HandLauncher_StageLoad, HandLauncher_EditorDraw,
                 HandLauncher_EditorLoad, HandLauncher_Serialize);
    RSDK.CreateObject((Object**)&HangConveyor, "HangConveyor", sizeof(EntityHangConveyor), sizeof(ObjectHangConveyor), HangConveyor_Update, HangConveyor_LateUpdate,
                 HangConveyor_StaticUpdate, HangConveyor_Draw, HangConveyor_Create, HangConveyor_StageLoad, HangConveyor_EditorDraw,
                 HangConveyor_EditorLoad, HangConveyor_Serialize);
    RSDK.CreateObject((Object**)&HangGlider, "HangGlider", sizeof(EntityHangGlider), sizeof(ObjectHangGlider), HangGlider_Update, HangGlider_LateUpdate,
                 HangGlider_StaticUpdate, HangGlider_Draw, HangGlider_Create, HangGlider_StageLoad, HangGlider_EditorDraw, HangGlider_EditorLoad,
                 HangGlider_Serialize);
    RSDK.CreateObject((Object**)&HangPoint, "HangPoint", sizeof(EntityHangPoint), sizeof(ObjectHangPoint), HangPoint_Update, HangPoint_LateUpdate,
                 HangPoint_StaticUpdate, HangPoint_Draw, HangPoint_Create, HangPoint_StageLoad, HangPoint_EditorDraw, HangPoint_EditorLoad,
                 HangPoint_Serialize);
    RSDK.CreateObject((Object**)&Hatch, "Hatch", sizeof(EntityHatch), sizeof(ObjectHatch), Hatch_Update, Hatch_LateUpdate, Hatch_StaticUpdate, Hatch_Draw,
                 Hatch_Create, Hatch_StageLoad, Hatch_EditorDraw, Hatch_EditorLoad, Hatch_Serialize);
    RSDK.CreateObject((Object**)&Hatterkiller, "Hatterkiller", sizeof(EntityHatterkiller), sizeof(ObjectHatterkiller), Hatterkiller_Update, Hatterkiller_LateUpdate,
                 Hatterkiller_StaticUpdate, Hatterkiller_Draw, Hatterkiller_Create, Hatterkiller_StageLoad, Hatterkiller_EditorDraw,
                 Hatterkiller_EditorLoad, Hatterkiller_Serialize);
    RSDK.CreateObject((Object**)&HCZ1Intro, "HCZ1Intro", sizeof(EntityHCZ1Intro), sizeof(ObjectHCZ1Intro), HCZ1Intro_Update, HCZ1Intro_LateUpdate,
                 HCZ1Intro_StaticUpdate, HCZ1Intro_Draw, HCZ1Intro_Create, HCZ1Intro_StageLoad, HCZ1Intro_EditorDraw, HCZ1Intro_EditorLoad,
                 HCZ1Intro_Serialize);
    RSDK.CreateObject((Object**)&HCZOneWayDoor, "HCZOneWayDoor", sizeof(EntityHCZOneWayDoor), sizeof(ObjectHCZOneWayDoor), HCZOneWayDoor_Update,
                 HCZOneWayDoor_LateUpdate, HCZOneWayDoor_StaticUpdate, HCZOneWayDoor_Draw, HCZOneWayDoor_Create, HCZOneWayDoor_StageLoad,
                 HCZOneWayDoor_EditorDraw, HCZOneWayDoor_EditorLoad, HCZOneWayDoor_Serialize);
    RSDK.CreateObject((Object**)&HCZSetup, "HCZSetup", sizeof(EntityHCZSetup), sizeof(ObjectHCZSetup), HCZSetup_Update, HCZSetup_LateUpdate, HCZSetup_StaticUpdate,
                 HCZSetup_Draw, HCZSetup_Create, HCZSetup_StageLoad, HCZSetup_EditorDraw, HCZSetup_EditorLoad, HCZSetup_Serialize);
    RSDK.CreateObject((Object**)&HCZSpikeBall, "HCZSpikeBall", sizeof(EntityHCZSpikeBall), sizeof(ObjectHCZSpikeBall), HCZSpikeBall_Update, HCZSpikeBall_LateUpdate,
                 HCZSpikeBall_StaticUpdate, HCZSpikeBall_Draw, HCZSpikeBall_Create, HCZSpikeBall_StageLoad, HCZSpikeBall_EditorDraw,
                 HCZSpikeBall_EditorLoad, HCZSpikeBall_Serialize);
    RSDK.CreateObject((Object**)&HeavyGunner, "HeavyGunner", sizeof(EntityHeavyGunner), sizeof(ObjectHeavyGunner), HeavyGunner_Update, HeavyGunner_LateUpdate,
                 HeavyGunner_StaticUpdate, HeavyGunner_Draw, HeavyGunner_Create, HeavyGunner_StageLoad, HeavyGunner_EditorDraw, HeavyGunner_EditorLoad,
                 HeavyGunner_Serialize);
    RSDK.CreateObject((Object**)&HeavyKing, "HeavyKing", sizeof(EntityHeavyKing), sizeof(ObjectHeavyKing), HeavyKing_Update, HeavyKing_LateUpdate,
                 HeavyKing_StaticUpdate, HeavyKing_Draw, HeavyKing_Create, HeavyKing_StageLoad, HeavyKing_EditorDraw, HeavyKing_EditorLoad,
                 HeavyKing_Serialize);
    RSDK.CreateObject((Object**)&HeavyMystic, "HeavyMystic", sizeof(EntityHeavyMystic), sizeof(ObjectHeavyMystic), HeavyMystic_Update, HeavyMystic_LateUpdate,
                 HeavyMystic_StaticUpdate, HeavyMystic_Draw, HeavyMystic_Create, HeavyMystic_StageLoad, HeavyMystic_EditorDraw, HeavyMystic_EditorLoad,
                 HeavyMystic_Serialize);
    RSDK.CreateObject((Object**)&HeavyRider, "HeavyRider", sizeof(EntityHeavyRider), sizeof(ObjectHeavyRider), HeavyRider_Update, HeavyRider_LateUpdate,
                 HeavyRider_StaticUpdate, HeavyRider_Draw, HeavyRider_Create, HeavyRider_StageLoad, HeavyRider_EditorDraw, HeavyRider_EditorLoad,
                 HeavyRider_Serialize);
    RSDK.CreateObject((Object**)&HeavyShinobi, "HeavyShinobi", sizeof(EntityHeavyShinobi), sizeof(ObjectHeavyShinobi), HeavyShinobi_Update, HeavyShinobi_LateUpdate,
                 HeavyShinobi_StaticUpdate, HeavyShinobi_Draw, HeavyShinobi_Create, HeavyShinobi_StageLoad, HeavyShinobi_EditorDraw,
                 HeavyShinobi_EditorLoad, HeavyShinobi_Serialize);
    RSDK.CreateObject((Object**)&HiLoSign, "HiLoSign", sizeof(EntityHiLoSign), sizeof(ObjectHiLoSign), HiLoSign_Update, HiLoSign_LateUpdate, HiLoSign_StaticUpdate,
                 HiLoSign_Draw, HiLoSign_Create, HiLoSign_StageLoad, HiLoSign_EditorDraw, HiLoSign_EditorLoad, HiLoSign_Serialize);
    RSDK.CreateObject((Object**)&Honkytonk, "Honkytonk", sizeof(EntityHonkytonk), sizeof(ObjectHonkytonk), Honkytonk_Update, Honkytonk_LateUpdate,
                 Honkytonk_StaticUpdate, Honkytonk_Draw, Honkytonk_Create, Honkytonk_StageLoad, Honkytonk_EditorDraw, Honkytonk_EditorLoad,
                 Honkytonk_Serialize);
    RSDK.CreateObject((Object**)&Hotaru, "Hotaru", sizeof(EntityHotaru), sizeof(ObjectHotaru), Hotaru_Update, Hotaru_LateUpdate, Hotaru_StaticUpdate, Hotaru_Draw,
                 Hotaru_Create, Hotaru_StageLoad, Hotaru_EditorDraw, Hotaru_EditorLoad, Hotaru_Serialize);
    RSDK.CreateObject((Object**)&HotaruHiWatt, "HotaruHiWatt", sizeof(EntityHotaruHiWatt), sizeof(ObjectHotaruHiWatt), HotaruHiWatt_Update, HotaruHiWatt_LateUpdate,
                 HotaruHiWatt_StaticUpdate, HotaruHiWatt_Draw, HotaruHiWatt_Create, HotaruHiWatt_StageLoad, HotaruHiWatt_EditorDraw,
                 HotaruHiWatt_EditorLoad, HotaruHiWatt_Serialize);
    RSDK.CreateObject((Object**)&HotaruMKII, "HotaruMKII", sizeof(EntityHotaruMKII), sizeof(ObjectHotaruMKII), HotaruMKII_Update, HotaruMKII_LateUpdate,
                 HotaruMKII_StaticUpdate, HotaruMKII_Draw, HotaruMKII_Create, HotaruMKII_StageLoad, HotaruMKII_EditorDraw, HotaruMKII_EditorLoad,
                 HotaruMKII_Serialize);
    RSDK.CreateObject((Object**)&HPZEmerald, "HPZEmerald", sizeof(EntityHPZEmerald), sizeof(ObjectHPZEmerald), HPZEmerald_Update, HPZEmerald_LateUpdate,
                 HPZEmerald_StaticUpdate, HPZEmerald_Draw, HPZEmerald_Create, HPZEmerald_StageLoad, HPZEmerald_EditorDraw, HPZEmerald_EditorLoad,
                 HPZEmerald_Serialize);
    RSDK.CreateObject((Object**)&HUD, "HUD", sizeof(EntityHUD), sizeof(ObjectHUD), HUD_Update, HUD_LateUpdate, HUD_StaticUpdate, HUD_Draw, HUD_Create, HUD_StageLoad,
                 HUD_EditorDraw, HUD_EditorLoad, HUD_Serialize);
    RSDK.CreateObject((Object**)&Ice, "Ice", sizeof(EntityIce), sizeof(ObjectIce), Ice_Update, Ice_LateUpdate, Ice_StaticUpdate, Ice_Draw, Ice_Create, Ice_StageLoad,
                 Ice_EditorDraw, Ice_EditorLoad, Ice_Serialize);
    RSDK.CreateObject((Object**)&IceBomba, "IceBomba", sizeof(EntityIceBomba), sizeof(ObjectIceBomba), IceBomba_Update, IceBomba_LateUpdate, IceBomba_StaticUpdate,
                 IceBomba_Draw, IceBomba_Create, IceBomba_StageLoad, IceBomba_EditorDraw, IceBomba_EditorLoad, IceBomba_Serialize);
    RSDK.CreateObject((Object**)&IceSpring, "IceSpring", sizeof(EntityIceSpring), sizeof(ObjectIceSpring), IceSpring_Update, IceSpring_LateUpdate,
                 IceSpring_StaticUpdate, IceSpring_Draw, IceSpring_Create, IceSpring_StageLoad, IceSpring_EditorDraw, IceSpring_EditorLoad,
                 IceSpring_Serialize);
    RSDK.CreateObject((Object**)&ImageTrail, "ImageTrail", sizeof(EntityImageTrail), sizeof(ObjectImageTrail), ImageTrail_Update, ImageTrail_LateUpdate,
                 ImageTrail_StaticUpdate, ImageTrail_Draw, ImageTrail_Create, ImageTrail_StageLoad, ImageTrail_EditorDraw, ImageTrail_EditorLoad, ImageTrail_Serialize);
    RSDK.CreateObject((Object **)&Ink, "Ink", sizeof(EntityInk), sizeof(ObjectInk), Ink_Update, Ink_LateUpdate, Ink_StaticUpdate, Ink_Draw,
                      Ink_Create, Ink_StageLoad, Ink_EditorDraw, Ink_EditorLoad, Ink_Serialize);
    RSDK.CreateObject((Object **)&InkWipe, "InkWipe", sizeof(EntityInkWipe), sizeof(ObjectInkWipe), InkWipe_Update, InkWipe_LateUpdate,
                      InkWipe_StaticUpdate, InkWipe_Draw, InkWipe_Create, InkWipe_StageLoad, InkWipe_EditorDraw, InkWipe_EditorLoad,
                      InkWipe_Serialize);
    RSDK.CreateObject((Object**)&InvincibleStars, "InvincibleStars", sizeof(EntityInvincibleStars), sizeof(ObjectInvincibleStars), InvincibleStars_Update,
                 InvincibleStars_LateUpdate, InvincibleStars_StaticUpdate, InvincibleStars_Draw, InvincibleStars_Create, InvincibleStars_StageLoad,
                 InvincibleStars_EditorDraw, InvincibleStars_EditorLoad, InvincibleStars_Serialize);
    RSDK.CreateObject((Object**)&InvisibleBlock, "InvisibleBlock", sizeof(EntityInvisibleBlock), sizeof(ObjectInvisibleBlock), InvisibleBlock_Update,
                 InvisibleBlock_LateUpdate, InvisibleBlock_StaticUpdate, InvisibleBlock_Draw, InvisibleBlock_Create, InvisibleBlock_StageLoad,
                 InvisibleBlock_EditorDraw, InvisibleBlock_EditorLoad, InvisibleBlock_Serialize);
    RSDK.CreateObject((Object**)&ItemBox, "ItemBox", sizeof(EntityItemBox), sizeof(ObjectItemBox), ItemBox_Update, ItemBox_LateUpdate, ItemBox_StaticUpdate,
                 ItemBox_Draw, ItemBox_Create, ItemBox_StageLoad, ItemBox_EditorDraw, ItemBox_EditorLoad, ItemBox_Serialize);
    RSDK.CreateObject((Object**)&Iwamodoki, "Iwamodoki", sizeof(EntityIwamodoki), sizeof(ObjectIwamodoki), Iwamodoki_Update, Iwamodoki_LateUpdate,
                 Iwamodoki_StaticUpdate, Iwamodoki_Draw, Iwamodoki_Create, Iwamodoki_StageLoad, Iwamodoki_EditorDraw, Iwamodoki_EditorLoad,
                 Iwamodoki_Serialize);
    RSDK.CreateObject((Object**)&JacobsLadder, "JacobsLadder", sizeof(EntityJacobsLadder), sizeof(ObjectJacobsLadder), JacobsLadder_Update, JacobsLadder_LateUpdate,
                 JacobsLadder_StaticUpdate, JacobsLadder_Draw, JacobsLadder_Create, JacobsLadder_StageLoad, JacobsLadder_EditorDraw,
                 JacobsLadder_EditorLoad, JacobsLadder_Serialize);
    RSDK.CreateObject((Object**)&Jawz, "Jawz", sizeof(EntityJawz), sizeof(ObjectJawz), Jawz_Update, Jawz_LateUpdate, Jawz_StaticUpdate, Jawz_Draw, Jawz_Create,
                 Jawz_StageLoad, Jawz_EditorDraw, Jawz_EditorLoad, Jawz_Serialize);
    RSDK.CreateObject((Object**)&Jellygnite, "Jellygnite", sizeof(EntityJellygnite), sizeof(ObjectJellygnite), Jellygnite_Update, Jellygnite_LateUpdate,
                 Jellygnite_StaticUpdate, Jellygnite_Draw, Jellygnite_Create, Jellygnite_StageLoad, Jellygnite_EditorDraw, Jellygnite_EditorLoad,
                 Jellygnite_Serialize);
    RSDK.CreateObject((Object**)&JuggleSaw, "JuggleSaw", sizeof(EntityJuggleSaw), sizeof(ObjectJuggleSaw), JuggleSaw_Update, JuggleSaw_LateUpdate,
                 JuggleSaw_StaticUpdate, JuggleSaw_Draw, JuggleSaw_Create, JuggleSaw_StageLoad, JuggleSaw_EditorDraw, JuggleSaw_EditorLoad,
                 JuggleSaw_Serialize);
    RSDK.CreateObject((Object**)&JunctionWheel, "JunctionWheel", sizeof(EntityJunctionWheel), sizeof(ObjectJunctionWheel), JunctionWheel_Update,
                 JunctionWheel_LateUpdate, JunctionWheel_StaticUpdate, JunctionWheel_Draw, JunctionWheel_Create, JunctionWheel_StageLoad,
                 JunctionWheel_EditorDraw, JunctionWheel_EditorLoad, JunctionWheel_Serialize);
    RSDK.CreateObject((Object**)&Kabasira, "Kabasira", sizeof(EntityKabasira), sizeof(ObjectKabasira), Kabasira_Update, Kabasira_LateUpdate, Kabasira_StaticUpdate,
                 Kabasira_Draw, Kabasira_Create, Kabasira_StageLoad, Kabasira_EditorDraw, Kabasira_EditorLoad, Kabasira_Serialize);
    RSDK.CreateObject((Object**)&Kanabun, "Kanabun", sizeof(EntityKanabun), sizeof(ObjectKanabun), Kanabun_Update, Kanabun_LateUpdate, Kanabun_StaticUpdate,
                 Kanabun_Draw, Kanabun_Create, Kanabun_StageLoad, Kanabun_EditorDraw, Kanabun_EditorLoad, Kanabun_Serialize);
    RSDK.CreateObject((Object**)&KingAttack, "KingAttack", sizeof(EntityKingAttack), sizeof(ObjectKingAttack), KingAttack_Update, KingAttack_LateUpdate,
                 KingAttack_StaticUpdate, KingAttack_Draw, KingAttack_Create, KingAttack_StageLoad, KingAttack_EditorDraw, KingAttack_EditorLoad,
                 KingAttack_Serialize);
    RSDK.CreateObject((Object**)&KingClaw, "KingClaw", sizeof(EntityKingClaw), sizeof(ObjectKingClaw), KingClaw_Update, KingClaw_LateUpdate, KingClaw_StaticUpdate,
                 KingClaw_Draw, KingClaw_Create, KingClaw_StageLoad, KingClaw_EditorDraw, KingClaw_EditorLoad, KingClaw_Serialize);
    RSDK.CreateObject((Object**)&KleptoMobile, "KleptoMobile", sizeof(EntityKleptoMobile), sizeof(ObjectKleptoMobile), KleptoMobile_Update, KleptoMobile_LateUpdate,
                 KleptoMobile_StaticUpdate, KleptoMobile_Draw, KleptoMobile_Create, KleptoMobile_StageLoad, KleptoMobile_EditorDraw,
                 KleptoMobile_EditorLoad, KleptoMobile_Serialize);
    RSDK.CreateObject((Object**)&LargeGear, "LargeGear", sizeof(EntityLargeGear), sizeof(ObjectLargeGear), LargeGear_Update, LargeGear_LateUpdate,
                 LargeGear_StaticUpdate, LargeGear_Draw, LargeGear_Create, LargeGear_StageLoad, LargeGear_EditorDraw, LargeGear_EditorLoad,
                 LargeGear_Serialize);
    RSDK.CreateObject((Object**)&Launcher, "Launcher", sizeof(EntityLauncher), sizeof(ObjectLauncher), Launcher_Update, Launcher_LateUpdate, Launcher_StaticUpdate,
                 Launcher_Draw, Launcher_Create, Launcher_StageLoad, Launcher_EditorDraw, Launcher_EditorLoad, Launcher_Serialize);
    RSDK.CreateObject((Object**)&LaunchSpring, "LaunchSpring", sizeof(EntityLaunchSpring), sizeof(ObjectLaunchSpring), LaunchSpring_Update, LaunchSpring_LateUpdate,
                 LaunchSpring_StaticUpdate, LaunchSpring_Draw, LaunchSpring_Create, LaunchSpring_StageLoad, LaunchSpring_EditorDraw,
                 LaunchSpring_EditorLoad, LaunchSpring_Serialize);
    RSDK.CreateObject((Object**)&LaundroMobile, "LaundroMobile", sizeof(EntityLaundroMobile), sizeof(ObjectLaundroMobile), LaundroMobile_Update,
                 LaundroMobile_LateUpdate, LaundroMobile_StaticUpdate, LaundroMobile_Draw, LaundroMobile_Create, LaundroMobile_StageLoad,
                 LaundroMobile_EditorDraw, LaundroMobile_EditorLoad, LaundroMobile_Serialize);
    RSDK.CreateObject((Object**)&LavaFall, "LavaFall", sizeof(EntityLavaFall), sizeof(ObjectLavaFall), LavaFall_Update, LavaFall_LateUpdate, LavaFall_StaticUpdate,
                 LavaFall_Draw, LavaFall_Create, LavaFall_StageLoad, LavaFall_EditorDraw, LavaFall_EditorLoad, LavaFall_Serialize);
    RSDK.CreateObject((Object**)&LavaGeyser, "LavaGeyser", sizeof(EntityLavaGeyser), sizeof(ObjectLavaGeyser), LavaGeyser_Update, LavaGeyser_LateUpdate,
                 LavaGeyser_StaticUpdate, LavaGeyser_Draw, LavaGeyser_Create, LavaGeyser_StageLoad, LavaGeyser_EditorDraw, LavaGeyser_EditorLoad,
                 LavaGeyser_Serialize);
    RSDK.CreateObject((Object**)&LEDPanel, "LEDPanel", sizeof(EntityLEDPanel), sizeof(ObjectLEDPanel), LEDPanel_Update, LEDPanel_LateUpdate, LEDPanel_StaticUpdate,
                 LEDPanel_Draw, LEDPanel_Create, LEDPanel_StageLoad, LEDPanel_EditorDraw, LEDPanel_EditorLoad, LEDPanel_Serialize);
    RSDK.CreateObject((Object**)&Letterboard, "Letterboard", sizeof(EntityLetterboard), sizeof(ObjectLetterboard), Letterboard_Update, Letterboard_LateUpdate,
                 Letterboard_StaticUpdate, Letterboard_Draw, Letterboard_Create, Letterboard_StageLoad, Letterboard_EditorDraw, Letterboard_EditorLoad,
                 Letterboard_Serialize);
    RSDK.CreateObject((Object**)&LevelSelect, "LevelSelect", sizeof(EntityLevelSelect), sizeof(ObjectLevelSelect), LevelSelect_Update, LevelSelect_LateUpdate,
                 LevelSelect_StaticUpdate, LevelSelect_Draw, LevelSelect_Create, LevelSelect_StageLoad, LevelSelect_EditorDraw, LevelSelect_EditorLoad,
                 LevelSelect_Serialize);
    RSDK.CreateObject((Object**)&LightBarrier, "LightBarrier", sizeof(EntityLightBarrier), sizeof(ObjectLightBarrier), LightBarrier_Update, LightBarrier_LateUpdate,
                 LightBarrier_StaticUpdate, LightBarrier_Draw, LightBarrier_Create, LightBarrier_StageLoad, LightBarrier_EditorDraw,
                 LightBarrier_EditorLoad, LightBarrier_Serialize);
    RSDK.CreateObject((Object**)&LightBulb, "LightBulb", sizeof(EntityLightBulb), sizeof(ObjectLightBulb), LightBulb_Update, LightBulb_LateUpdate,
                 LightBulb_StaticUpdate, LightBulb_Draw, LightBulb_Create, LightBulb_StageLoad, LightBulb_EditorDraw, LightBulb_EditorLoad,
                 LightBulb_Serialize);
    RSDK.CreateObject((Object**)&Localization, "Localization", sizeof(EntityLocalization), sizeof(ObjectLocalization), Localization_Update, Localization_LateUpdate,
                 Localization_StaticUpdate, Localization_Draw, Localization_Create, Localization_StageLoad, Localization_EditorDraw,
                 Localization_EditorLoad, Localization_Serialize);
    RSDK.CreateObject((Object**)&LocoSmoke, "LocoSmoke", sizeof(EntityLocoSmoke), sizeof(ObjectLocoSmoke), LocoSmoke_Update, LocoSmoke_LateUpdate,
                 LocoSmoke_StaticUpdate, LocoSmoke_Draw, LocoSmoke_Create, LocoSmoke_StageLoad, LocoSmoke_EditorDraw, LocoSmoke_EditorLoad,
                 LocoSmoke_Serialize);
    RSDK.CreateObject((Object**)&LogoSetup, "LogoSetup", sizeof(EntityLogoSetup), sizeof(ObjectLogoSetup), LogoSetup_Update, LogoSetup_LateUpdate,
                 LogoSetup_StaticUpdate, LogoSetup_Draw, LogoSetup_Create, LogoSetup_StageLoad, LogoSetup_EditorDraw, LogoSetup_EditorLoad,
                 LogoSetup_Serialize);
    RSDK.CreateObject((Object**)&LottoBall, "LottoBall", sizeof(EntityLottoBall), sizeof(ObjectLottoBall), LottoBall_Update, LottoBall_LateUpdate,
                 LottoBall_StaticUpdate, LottoBall_Draw, LottoBall_Create, LottoBall_StageLoad, LottoBall_EditorDraw, LottoBall_EditorLoad,
                 LottoBall_Serialize);
    RSDK.CreateObject((Object**)&LottoMachine, "LottoMachine", sizeof(EntityLottoMachine), sizeof(ObjectLottoMachine), LottoMachine_Update, LottoMachine_LateUpdate,
                 LottoMachine_StaticUpdate, LottoMachine_Draw, LottoMachine_Create, LottoMachine_StageLoad, LottoMachine_EditorDraw,
                 LottoMachine_EditorLoad, LottoMachine_Serialize);
    RSDK.CreateObject((Object**)&LoveTester, "LoveTester", sizeof(EntityLoveTester), sizeof(ObjectLoveTester), LoveTester_Update, LoveTester_LateUpdate,
                 LoveTester_StaticUpdate, LoveTester_Draw, LoveTester_Create, LoveTester_StageLoad, LoveTester_EditorDraw, LoveTester_EditorLoad,
                 LoveTester_Serialize);
    RSDK.CreateObject((Object**)&LRZ1Intro, "LRZ1Intro", sizeof(EntityLRZ1Intro), sizeof(ObjectLRZ1Intro), LRZ1Intro_Update, LRZ1Intro_LateUpdate,
                 LRZ1Intro_StaticUpdate, LRZ1Intro_Draw, LRZ1Intro_Create, LRZ1Intro_StageLoad, LRZ1Intro_EditorDraw, LRZ1Intro_EditorLoad,
                 LRZ1Intro_Serialize);
    RSDK.CreateObject((Object**)&LRZ1Outro, "LRZ1Outro", sizeof(EntityLRZ1Outro), sizeof(ObjectLRZ1Outro), LRZ1Outro_Update, LRZ1Outro_LateUpdate,
                 LRZ1Outro_StaticUpdate, LRZ1Outro_Draw, LRZ1Outro_Create, LRZ1Outro_StageLoad, LRZ1Outro_EditorDraw, LRZ1Outro_EditorLoad,
                 LRZ1Outro_Serialize);
    RSDK.CreateObject((Object**)&LRZ1Setup, "LRZ1Setup", sizeof(EntityLRZ1Setup), sizeof(ObjectLRZ1Setup), LRZ1Setup_Update, LRZ1Setup_LateUpdate,
                 LRZ1Setup_StaticUpdate, LRZ1Setup_Draw, LRZ1Setup_Create, LRZ1Setup_StageLoad, LRZ1Setup_EditorDraw, LRZ1Setup_EditorLoad,
                 LRZ1Setup_Serialize);
    RSDK.CreateObject((Object**)&LRZ2Setup, "LRZ2Setup", sizeof(EntityLRZ2Setup), sizeof(ObjectLRZ2Setup), LRZ2Setup_Update, LRZ2Setup_LateUpdate,
                 LRZ2Setup_StaticUpdate, LRZ2Setup_Draw, LRZ2Setup_Create, LRZ2Setup_StageLoad, LRZ2Setup_EditorDraw, LRZ2Setup_EditorLoad,
                 LRZ2Setup_Serialize);
    RSDK.CreateObject((Object**)&LRZ3Cutscene, "LRZ3Cutscene", sizeof(EntityLRZ3Cutscene), sizeof(ObjectLRZ3Cutscene), LRZ3Cutscene_Update, LRZ3Cutscene_LateUpdate,
                 LRZ3Cutscene_StaticUpdate, LRZ3Cutscene_Draw, LRZ3Cutscene_Create, LRZ3Cutscene_StageLoad, LRZ3Cutscene_EditorDraw,
                 LRZ3Cutscene_EditorLoad, LRZ3Cutscene_Serialize);
    RSDK.CreateObject((Object**)&LRZ3Outro, "LRZ3Outro", sizeof(EntityLRZ3Outro), sizeof(ObjectLRZ3Outro), LRZ3Outro_Update, LRZ3Outro_LateUpdate,
                 LRZ3Outro_StaticUpdate, LRZ3Outro_Draw, LRZ3Outro_Create, LRZ3Outro_StageLoad, LRZ3Outro_EditorDraw, LRZ3Outro_EditorLoad,
                 LRZ3Outro_Serialize);
    RSDK.CreateObject((Object**)&LRZ3OutroK, "LRZ3OutroK", sizeof(EntityLRZ3OutroK), sizeof(ObjectLRZ3OutroK), LRZ3OutroK_Update, LRZ3OutroK_LateUpdate,
                 LRZ3OutroK_StaticUpdate, LRZ3OutroK_Draw, LRZ3OutroK_Create, LRZ3OutroK_StageLoad, LRZ3OutroK_EditorDraw, LRZ3OutroK_EditorLoad,
                 LRZ3OutroK_Serialize);
    RSDK.CreateObject((Object**)&LRZ3Setup, "LRZ3Setup", sizeof(EntityLRZ3Setup), sizeof(ObjectLRZ3Setup), LRZ3Setup_Update, LRZ3Setup_LateUpdate,
                 LRZ3Setup_StaticUpdate, LRZ3Setup_Draw, LRZ3Setup_Create, LRZ3Setup_StageLoad, LRZ3Setup_EditorDraw, LRZ3Setup_EditorLoad,
                 LRZ3Setup_Serialize);
    RSDK.CreateObject((Object**)&LRZConvControl, "LRZConvControl", sizeof(EntityLRZConvControl), sizeof(ObjectLRZConvControl), LRZConvControl_Update,
                 LRZConvControl_LateUpdate, LRZConvControl_StaticUpdate, LRZConvControl_Draw, LRZConvControl_Create, LRZConvControl_StageLoad,
                 LRZConvControl_EditorDraw, LRZConvControl_EditorLoad, LRZConvControl_Serialize);
    RSDK.CreateObject((Object**)&LRZConvDropper, "LRZConvDropper", sizeof(EntityLRZConvDropper), sizeof(ObjectLRZConvDropper), LRZConvDropper_Update,
                 LRZConvDropper_LateUpdate, LRZConvDropper_StaticUpdate, LRZConvDropper_Draw, LRZConvDropper_Create, LRZConvDropper_StageLoad,
                 LRZConvDropper_EditorDraw, LRZConvDropper_EditorLoad, LRZConvDropper_Serialize);
    RSDK.CreateObject((Object**)&LRZConveyor, "LRZConveyor", sizeof(EntityLRZConveyor), sizeof(ObjectLRZConveyor), LRZConveyor_Update, LRZConveyor_LateUpdate,
                 LRZConveyor_StaticUpdate, LRZConveyor_Draw, LRZConveyor_Create, LRZConveyor_StageLoad, LRZConveyor_EditorDraw, LRZConveyor_EditorLoad,
                 LRZConveyor_Serialize);
    RSDK.CreateObject((Object**)&LRZConvItem, "LRZConvItem", sizeof(EntityLRZConvItem), sizeof(ObjectLRZConvItem), LRZConvItem_Update, LRZConvItem_LateUpdate,
                 LRZConvItem_StaticUpdate, LRZConvItem_Draw, LRZConvItem_Create, LRZConvItem_StageLoad, LRZConvItem_EditorDraw, LRZConvItem_EditorLoad,
                 LRZConvItem_Serialize);
    RSDK.CreateObject((Object**)&LRZConvSwitch, "LRZConvSwitch", sizeof(EntityLRZConvSwitch), sizeof(ObjectLRZConvSwitch), LRZConvSwitch_Update,
                 LRZConvSwitch_LateUpdate, LRZConvSwitch_StaticUpdate, LRZConvSwitch_Draw, LRZConvSwitch_Create, LRZConvSwitch_StageLoad,
                 LRZConvSwitch_EditorDraw, LRZConvSwitch_EditorLoad, LRZConvSwitch_Serialize);
    RSDK.CreateObject((Object**)&LRZFireball, "LRZFireball", sizeof(EntityLRZFireball), sizeof(ObjectLRZFireball), LRZFireball_Update, LRZFireball_LateUpdate,
                 LRZFireball_StaticUpdate, LRZFireball_Draw, LRZFireball_Create, LRZFireball_StageLoad, LRZFireball_EditorDraw, LRZFireball_EditorLoad,
                 LRZFireball_Serialize);
    RSDK.CreateObject((Object**)&LRZRockPile, "LRZRockPile", sizeof(EntityLRZRockPile), sizeof(ObjectLRZRockPile), LRZRockPile_Update, LRZRockPile_LateUpdate,
                 LRZRockPile_StaticUpdate, LRZRockPile_Draw, LRZRockPile_Create, LRZRockPile_StageLoad, LRZRockPile_EditorDraw, LRZRockPile_EditorLoad,
                 LRZRockPile_Serialize);
    RSDK.CreateObject((Object**)&LRZSpikeBall, "LRZSpikeBall", sizeof(EntityLRZSpikeBall), sizeof(ObjectLRZSpikeBall), LRZSpikeBall_Update, LRZSpikeBall_LateUpdate,
                 LRZSpikeBall_StaticUpdate, LRZSpikeBall_Draw, LRZSpikeBall_Create, LRZSpikeBall_StageLoad, LRZSpikeBall_EditorDraw,
                 LRZSpikeBall_EditorLoad, LRZSpikeBall_Serialize);
    RSDK.CreateObject((Object**)&LRZSpiral, "LRZSpiral", sizeof(EntityLRZSpiral), sizeof(ObjectLRZSpiral), LRZSpiral_Update, LRZSpiral_LateUpdate,
                 LRZSpiral_StaticUpdate, LRZSpiral_Draw, LRZSpiral_Create, LRZSpiral_StageLoad, LRZSpiral_EditorDraw, LRZSpiral_EditorLoad,
                 LRZSpiral_Serialize);
    RSDK.CreateObject((Object**)&MagnetSphere, "MagnetSphere", sizeof(EntityMagnetSphere), sizeof(ObjectMagnetSphere), MagnetSphere_Update, MagnetSphere_LateUpdate,
                 MagnetSphere_StaticUpdate, MagnetSphere_Draw, MagnetSphere_Create, MagnetSphere_StageLoad, MagnetSphere_EditorDraw,
                 MagnetSphere_EditorLoad, MagnetSphere_Serialize);
    RSDK.CreateObject((Object**)&MagPlatform, "MagPlatform", sizeof(EntityMagPlatform), sizeof(ObjectMagPlatform), MagPlatform_Update, MagPlatform_LateUpdate,
                 MagPlatform_StaticUpdate, MagPlatform_Draw, MagPlatform_Create, MagPlatform_StageLoad, MagPlatform_EditorDraw, MagPlatform_EditorLoad,
                 MagPlatform_Serialize);
    RSDK.CreateObject((Object**)&MagSpikeBall, "MagSpikeBall", sizeof(EntityMagSpikeBall), sizeof(ObjectMagSpikeBall), MagSpikeBall_Update, MagSpikeBall_LateUpdate,
                 MagSpikeBall_StaticUpdate, MagSpikeBall_Draw, MagSpikeBall_Create, MagSpikeBall_StageLoad, MagSpikeBall_EditorDraw,
                 MagSpikeBall_EditorLoad, MagSpikeBall_Serialize);
    RSDK.CreateObject((Object**)&MainMenu, "MainMenu", sizeof(EntityMainMenu), sizeof(ObjectMainMenu), MainMenu_Update, MainMenu_LateUpdate, MainMenu_StaticUpdate,
                 MainMenu_Draw, MainMenu_Create, MainMenu_StageLoad, MainMenu_EditorDraw, MainMenu_EditorLoad, MainMenu_Serialize);
    RSDK.CreateObject((Object**)&ManiaModeMenu, "ManiaModeMenu", sizeof(EntityManiaModeMenu), sizeof(ObjectManiaModeMenu), ManiaModeMenu_Update,
                 ManiaModeMenu_LateUpdate, ManiaModeMenu_StaticUpdate, ManiaModeMenu_Draw, ManiaModeMenu_Create, ManiaModeMenu_StageLoad,
                 ManiaModeMenu_EditorDraw, ManiaModeMenu_EditorLoad, ManiaModeMenu_Serialize);
    RSDK.CreateObject((Object**)&MatryoshkaBom, "MatryoshkaBom", sizeof(EntityMatryoshkaBom), sizeof(ObjectMatryoshkaBom), MatryoshkaBom_Update,
                 MatryoshkaBom_LateUpdate, MatryoshkaBom_StaticUpdate, MatryoshkaBom_Draw, MatryoshkaBom_Create, MatryoshkaBom_StageLoad,
                 MatryoshkaBom_EditorDraw, MatryoshkaBom_EditorLoad, MatryoshkaBom_Serialize);
    RSDK.CreateObject((Object**)&MechaBu, "MechaBu", sizeof(EntityMechaBu), sizeof(ObjectMechaBu), MechaBu_Update, MechaBu_LateUpdate, MechaBu_StaticUpdate,
                 MechaBu_Draw, MechaBu_Create, MechaBu_StageLoad, MechaBu_EditorDraw, MechaBu_EditorLoad, MechaBu_Serialize);
    RSDK.CreateObject((Object**)&MegaChopper, "MegaChopper", sizeof(EntityMegaChopper), sizeof(ObjectMegaChopper), MegaChopper_Update, MegaChopper_LateUpdate,
                 MegaChopper_StaticUpdate, MegaChopper_Draw, MegaChopper_Create, MegaChopper_StageLoad, MegaChopper_EditorDraw, MegaChopper_EditorLoad,
                 MegaChopper_Serialize);
    RSDK.CreateObject((Object**)&MegaOctus, "MegaOctus", sizeof(EntityMegaOctus), sizeof(ObjectMegaOctus), MegaOctus_Update, MegaOctus_LateUpdate,
                 MegaOctus_StaticUpdate, MegaOctus_Draw, MegaOctus_Create, MegaOctus_StageLoad, MegaOctus_EditorDraw, MegaOctus_EditorLoad,
                 MegaOctus_Serialize);
    RSDK.CreateObject((Object**)&MenuParam, "MenuParam", sizeof(EntityMenuParam), sizeof(ObjectMenuParam), MenuParam_Update, MenuParam_LateUpdate,
                 MenuParam_StaticUpdate, MenuParam_Draw, MenuParam_Create, MenuParam_StageLoad, MenuParam_EditorDraw, MenuParam_EditorLoad,
                 MenuParam_Serialize);
    RSDK.CreateObject((Object**)&MenuSetup, "MenuSetup", sizeof(EntityMenuSetup), sizeof(ObjectMenuSetup), MenuSetup_Update, MenuSetup_LateUpdate,
                 MenuSetup_StaticUpdate, MenuSetup_Draw, MenuSetup_Create, MenuSetup_StageLoad, MenuSetup_EditorDraw, MenuSetup_EditorLoad,
                 MenuSetup_Serialize);
    RSDK.CreateObject((Object**)&MetalArm, "MetalArm", sizeof(EntityMetalArm), sizeof(ObjectMetalArm), MetalArm_Update, MetalArm_LateUpdate, MetalArm_StaticUpdate,
                 MetalArm_Draw, MetalArm_Create, MetalArm_StageLoad, MetalArm_EditorDraw, MetalArm_EditorLoad, MetalArm_Serialize);
    RSDK.CreateObject((Object**)&MetalSonic, "MetalSonic", sizeof(EntityMetalSonic), sizeof(ObjectMetalSonic), MetalSonic_Update, MetalSonic_LateUpdate,
                 MetalSonic_StaticUpdate, MetalSonic_Draw, MetalSonic_Create, MetalSonic_StageLoad, MetalSonic_EditorDraw, MetalSonic_EditorLoad,
                 MetalSonic_Serialize);
    RSDK.CreateObject((Object**)&MeterDroid, "MeterDroid", sizeof(EntityMeterDroid), sizeof(ObjectMeterDroid), MeterDroid_Update, MeterDroid_LateUpdate,
                 MeterDroid_StaticUpdate, MeterDroid_Draw, MeterDroid_Create, MeterDroid_StageLoad, MeterDroid_EditorDraw, MeterDroid_EditorLoad,
                 MeterDroid_Serialize);
    RSDK.CreateObject((Object**)&MicDrop, "MicDrop", sizeof(EntityMicDrop), sizeof(ObjectMicDrop), MicDrop_Update, MicDrop_LateUpdate, MicDrop_StaticUpdate,
                 MicDrop_Draw, MicDrop_Create, MicDrop_StageLoad, MicDrop_EditorDraw, MicDrop_EditorLoad, MicDrop_Serialize);
    RSDK.CreateObject((Object**)&Mine, "Mine", sizeof(EntityMine), sizeof(ObjectMine), Mine_Update, Mine_LateUpdate, Mine_StaticUpdate, Mine_Draw, Mine_Create,
                 Mine_StageLoad, Mine_EditorDraw, Mine_EditorLoad, Mine_Serialize);
    RSDK.CreateObject((Object**)&MMZ2Outro, "MMZ2Outro", sizeof(EntityMMZ2Outro), sizeof(ObjectMMZ2Outro), MMZ2Outro_Update, MMZ2Outro_LateUpdate,
                 MMZ2Outro_StaticUpdate, MMZ2Outro_Draw, MMZ2Outro_Create, MMZ2Outro_StageLoad, MMZ2Outro_EditorDraw, MMZ2Outro_EditorLoad,
                 MMZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&MMZLightning, "MMZLightning", sizeof(EntityMMZLightning), sizeof(ObjectMMZLightning), MMZLightning_Update, MMZLightning_LateUpdate,
                 MMZLightning_StaticUpdate, MMZLightning_Draw, MMZLightning_Create, MMZLightning_StageLoad, MMZLightning_EditorDraw,
                 MMZLightning_EditorLoad, MMZLightning_Serialize);
    RSDK.CreateObject((Object**)&MMZSetup, "MMZSetup", sizeof(EntityMMZSetup), sizeof(ObjectMMZSetup), MMZSetup_Update, MMZSetup_LateUpdate, MMZSetup_StaticUpdate,
                 MMZSetup_Draw, MMZSetup_Create, MMZSetup_StageLoad, MMZSetup_EditorDraw, MMZSetup_EditorLoad, MMZSetup_Serialize);
    RSDK.CreateObject((Object**)&MMZWheel, "MMZWheel", sizeof(EntityMMZWheel), sizeof(ObjectMMZWheel), MMZWheel_Update, MMZWheel_LateUpdate, MMZWheel_StaticUpdate,
                 MMZWheel_Draw, MMZWheel_Create, MMZWheel_StageLoad, MMZWheel_EditorDraw, MMZWheel_EditorLoad, MMZWheel_Serialize);
    RSDK.CreateObject((Object**)&MonarchBG, "MonarchBG", sizeof(EntityMonarchBG), sizeof(ObjectMonarchBG), MonarchBG_Update, MonarchBG_LateUpdate,
                 MonarchBG_StaticUpdate, MonarchBG_Draw, MonarchBG_Create, MonarchBG_StageLoad, MonarchBG_EditorDraw, MonarchBG_EditorLoad,
                 MonarchBG_Serialize);
    RSDK.CreateObject((Object**)&MonarchPlans, "MonarchPlans", sizeof(EntityMonarchPlans), sizeof(ObjectMonarchPlans), MonarchPlans_Update, MonarchPlans_LateUpdate,
                 MonarchPlans_StaticUpdate, MonarchPlans_Draw, MonarchPlans_Create, MonarchPlans_StageLoad, MonarchPlans_EditorDraw,
                 MonarchPlans_EditorLoad, MonarchPlans_Serialize);
    RSDK.CreateObject((Object**)&MonkeyDude, "MonkeyDude", sizeof(EntityMonkeyDude), sizeof(ObjectMonkeyDude), MonkeyDude_Update, MonkeyDude_LateUpdate,
                 MonkeyDude_StaticUpdate, MonkeyDude_Draw, MonkeyDude_Create, MonkeyDude_StageLoad, MonkeyDude_EditorDraw, MonkeyDude_EditorLoad,
                 MonkeyDude_Serialize);
    RSDK.CreateObject((Object**)&Motobug, "Motobug", sizeof(EntityMotobug), sizeof(ObjectMotobug), Motobug_Update, Motobug_LateUpdate, Motobug_StaticUpdate,
                 Motobug_Draw, Motobug_Create, Motobug_StageLoad, Motobug_EditorDraw, Motobug_EditorLoad, Motobug_Serialize);
    RSDK.CreateObject((Object**)&MSBomb, "MSBomb", sizeof(EntityMSBomb), sizeof(ObjectMSBomb), MSBomb_Update, MSBomb_LateUpdate, MSBomb_StaticUpdate, MSBomb_Draw,
                 MSBomb_Create, MSBomb_StageLoad, MSBomb_EditorDraw, MSBomb_EditorLoad, MSBomb_Serialize);
    RSDK.CreateObject((Object**)&MSFactory, "MSFactory", sizeof(EntityMSFactory), sizeof(ObjectMSFactory), MSFactory_Update, MSFactory_LateUpdate,
                 MSFactory_StaticUpdate, MSFactory_Draw, MSFactory_Create, MSFactory_StageLoad, MSFactory_EditorDraw, MSFactory_EditorLoad,
                 MSFactory_Serialize);
    RSDK.CreateObject((Object**)&MSHologram, "MSHologram", sizeof(EntityMSHologram), sizeof(ObjectMSHologram), MSHologram_Update, MSHologram_LateUpdate,
                 MSHologram_StaticUpdate, MSHologram_Draw, MSHologram_Create, MSHologram_StageLoad, MSHologram_EditorDraw, MSHologram_EditorLoad,
                 MSHologram_Serialize);
    RSDK.CreateObject((Object**)&MSOrb, "MSOrb", sizeof(EntityMSOrb), sizeof(ObjectMSOrb), MSOrb_Update, MSOrb_LateUpdate, MSOrb_StaticUpdate, MSOrb_Draw,
                 MSOrb_Create, MSOrb_StageLoad, MSOrb_EditorDraw, MSOrb_EditorLoad, MSOrb_Serialize);
    RSDK.CreateObject((Object**)&MSPanel, "MSPanel", sizeof(EntityMSPanel), sizeof(ObjectMSPanel), MSPanel_Update, MSPanel_LateUpdate, MSPanel_StaticUpdate,
                 MSPanel_Draw, MSPanel_Create, MSPanel_StageLoad, MSPanel_EditorDraw, MSPanel_EditorLoad, MSPanel_Serialize);
    RSDK.CreateObject((Object**)&MSZ1KIntro, "MSZ1KIntro", sizeof(EntityMSZ1KIntro), sizeof(ObjectMSZ1KIntro), MSZ1KIntro_Update, MSZ1KIntro_LateUpdate,
                 MSZ1KIntro_StaticUpdate, MSZ1KIntro_Draw, MSZ1KIntro_Create, MSZ1KIntro_StageLoad, MSZ1KIntro_EditorDraw, MSZ1KIntro_EditorLoad,
                 MSZ1KIntro_Serialize);
    RSDK.CreateObject((Object**)&MSZ2Cutscene, "MSZ2Cutscene", sizeof(EntityMSZ2Cutscene), sizeof(ObjectMSZ2Cutscene), MSZ2Cutscene_Update, MSZ2Cutscene_LateUpdate,
                 MSZ2Cutscene_StaticUpdate, MSZ2Cutscene_Draw, MSZ2Cutscene_Create, MSZ2Cutscene_StageLoad, MSZ2Cutscene_EditorDraw,
                 MSZ2Cutscene_EditorLoad, MSZ2Cutscene_Serialize);
    RSDK.CreateObject((Object**)&MSZCutsceneK, "MSZCutsceneK", sizeof(EntityMSZCutsceneK), sizeof(ObjectMSZCutsceneK), MSZCutsceneK_Update, MSZCutsceneK_LateUpdate,
                 MSZCutsceneK_StaticUpdate, MSZCutsceneK_Draw, MSZCutsceneK_Create, MSZCutsceneK_StageLoad, MSZCutsceneK_EditorDraw,
                 MSZCutsceneK_EditorLoad, MSZCutsceneK_Serialize);
    RSDK.CreateObject((Object**)&MSZCutsceneST, "MSZCutsceneST", sizeof(EntityMSZCutsceneST), sizeof(ObjectMSZCutsceneST), MSZCutsceneST_Update,
                 MSZCutsceneST_LateUpdate, MSZCutsceneST_StaticUpdate, MSZCutsceneST_Draw, MSZCutsceneST_Create, MSZCutsceneST_StageLoad,
                 MSZCutsceneST_EditorDraw, MSZCutsceneST_EditorLoad, MSZCutsceneST_Serialize);
    RSDK.CreateObject((Object**)&MSZSetup, "MSZSetup", sizeof(EntityMSZSetup), sizeof(ObjectMSZSetup), MSZSetup_Update, MSZSetup_LateUpdate, MSZSetup_StaticUpdate,
                 MSZSetup_Draw, MSZSetup_Create, MSZSetup_StageLoad, MSZSetup_EditorDraw, MSZSetup_EditorLoad, MSZSetup_Serialize);
    RSDK.CreateObject((Object**)&MSZSpotlight, "MSZSpotlight", sizeof(EntityMSZSpotlight), sizeof(ObjectMSZSpotlight), MSZSpotlight_Update, MSZSpotlight_LateUpdate,
                 MSZSpotlight_StaticUpdate, MSZSpotlight_Draw, MSZSpotlight_Create, MSZSpotlight_StageLoad, MSZSpotlight_EditorDraw,
                 MSZSpotlight_EditorLoad, MSZSpotlight_Serialize);
    RSDK.CreateObject((Object**)&Music, "Music", sizeof(EntityMusic), sizeof(ObjectMusic), Music_Update, Music_LateUpdate, Music_StaticUpdate, Music_Draw,
                 Music_Create, Music_StageLoad, Music_EditorDraw, Music_EditorLoad, Music_Serialize);
    RSDK.CreateObject((Object**)&Newspaper, "Newspaper", sizeof(EntityNewspaper), sizeof(ObjectNewspaper), Newspaper_Update, Newspaper_LateUpdate,
                 Newspaper_StaticUpdate, Newspaper_Draw, Newspaper_Create, Newspaper_StageLoad, Newspaper_EditorDraw, Newspaper_EditorLoad,
                 Newspaper_Serialize);
    RSDK.CreateObject((Object**)&Newtron, "Newtron", sizeof(EntityNewtron), sizeof(ObjectNewtron), Newtron_Update, Newtron_LateUpdate, Newtron_StaticUpdate,
                 Newtron_Draw, Newtron_Create, Newtron_StageLoad, Newtron_EditorDraw, Newtron_EditorLoad, Newtron_Serialize);
    RSDK.CreateObject((Object**)&NoSwap, "NoSwap", sizeof(EntityNoSwap), sizeof(ObjectNoSwap), NoSwap_Update, NoSwap_LateUpdate, NoSwap_StaticUpdate, NoSwap_Draw,
                 NoSwap_Create, NoSwap_StageLoad, NoSwap_EditorDraw, NoSwap_EditorLoad, NoSwap_Serialize);
    RSDK.CreateObject((Object**)&Octus, "Octus", sizeof(EntityOctus), sizeof(ObjectOctus), Octus_Update, Octus_LateUpdate, Octus_StaticUpdate, Octus_Draw,
                 Octus_Create, Octus_StageLoad, Octus_EditorDraw, Octus_EditorLoad, Octus_Serialize);
    RSDK.CreateObject((Object**)&OneWayDoor, "OneWayDoor", sizeof(EntityOneWayDoor), sizeof(ObjectOneWayDoor), OneWayDoor_Update, OneWayDoor_LateUpdate,
                 OneWayDoor_StaticUpdate, OneWayDoor_Draw, OneWayDoor_Create, OneWayDoor_StageLoad, OneWayDoor_EditorDraw, OneWayDoor_EditorLoad,
                 OneWayDoor_Serialize);
    RSDK.CreateObject((Object**)&OOZ1Outro, "OOZ1Outro", sizeof(EntityOOZ1Outro), sizeof(ObjectOOZ1Outro), OOZ1Outro_Update, OOZ1Outro_LateUpdate,
                 OOZ1Outro_StaticUpdate, OOZ1Outro_Draw, OOZ1Outro_Create, OOZ1Outro_StageLoad, OOZ1Outro_EditorDraw, OOZ1Outro_EditorLoad,
                 OOZ1Outro_Serialize);
    RSDK.CreateObject((Object**)&OOZ2Outro, "OOZ2Outro", sizeof(EntityOOZ2Outro), sizeof(ObjectOOZ2Outro), OOZ2Outro_Update, OOZ2Outro_LateUpdate,
                 OOZ2Outro_StaticUpdate, OOZ2Outro_Draw, OOZ2Outro_Create, OOZ2Outro_StageLoad, OOZ2Outro_EditorDraw, OOZ2Outro_EditorLoad,
                 OOZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&OOZFlames, "OOZFlames", sizeof(EntityOOZFlames), sizeof(ObjectOOZFlames), OOZFlames_Update, OOZFlames_LateUpdate,
                 OOZFlames_StaticUpdate, OOZFlames_Draw, OOZFlames_Create, OOZFlames_StageLoad, OOZFlames_EditorDraw, OOZFlames_EditorLoad,
                 OOZFlames_Serialize);
    RSDK.CreateObject((Object**)&OOZSetup, "OOZSetup", sizeof(EntityOOZSetup), sizeof(ObjectOOZSetup), OOZSetup_Update, OOZSetup_LateUpdate, OOZSetup_StaticUpdate,
                 OOZSetup_Draw, OOZSetup_Create, OOZSetup_StageLoad, OOZSetup_EditorDraw, OOZSetup_EditorLoad, OOZSetup_Serialize);
    RSDK.CreateObject((Object**)&OptionsMenu, "OptionsMenu", sizeof(EntityOptionsMenu), sizeof(ObjectOptionsMenu), OptionsMenu_Update, OptionsMenu_LateUpdate,
                 OptionsMenu_StaticUpdate, OptionsMenu_Draw, OptionsMenu_Create, OptionsMenu_StageLoad, OptionsMenu_EditorDraw, OptionsMenu_EditorLoad, OptionsMenu_Serialize);
    RSDK.CreateObjectContainer((Object **)&Options, "Options", sizeof(ObjectOptions));
    RSDK.CreateObject((Object**)&OrbitSpike, "OrbitSpike", sizeof(EntityOrbitSpike), sizeof(ObjectOrbitSpike), OrbitSpike_Update, OrbitSpike_LateUpdate,
                 OrbitSpike_StaticUpdate, OrbitSpike_Draw, OrbitSpike_Create, OrbitSpike_StageLoad, OrbitSpike_EditorDraw, OrbitSpike_EditorLoad,
                 OrbitSpike_Serialize);
    RSDK.CreateObject((Object**)&PaintingEyes, "PaintingEyes", sizeof(EntityPaintingEyes), sizeof(ObjectPaintingEyes), PaintingEyes_Update, PaintingEyes_LateUpdate,
                 PaintingEyes_StaticUpdate, PaintingEyes_Draw, PaintingEyes_Create, PaintingEyes_StageLoad, PaintingEyes_EditorDraw, PaintingEyes_EditorLoad, PaintingEyes_Serialize);
    RSDK.CreateObject((Object **)&Palette, "Palette", sizeof(EntityPalette), sizeof(ObjectPalette), Palette_Update,
                      Palette_LateUpdate, Palette_StaticUpdate, Palette_Draw, Palette_Create, Palette_StageLoad,
                      Palette_EditorDraw, Palette_EditorLoad, Palette_Serialize);
    RSDK.CreateObject((Object**)&PaperRoller, "PaperRoller", sizeof(EntityPaperRoller), sizeof(ObjectPaperRoller), PaperRoller_Update, PaperRoller_LateUpdate,
                 PaperRoller_StaticUpdate, PaperRoller_Draw, PaperRoller_Create, PaperRoller_StageLoad, PaperRoller_EditorDraw, PaperRoller_EditorLoad,
                 PaperRoller_Serialize);
    RSDK.CreateObject((Object**)&ParallaxSprite, "ParallaxSprite", sizeof(EntityParallaxSprite), sizeof(ObjectParallaxSprite), ParallaxSprite_Update,
                 ParallaxSprite_LateUpdate, ParallaxSprite_StaticUpdate, ParallaxSprite_Draw, ParallaxSprite_Create, ParallaxSprite_StageLoad,
                 ParallaxSprite_EditorDraw, ParallaxSprite_EditorLoad, ParallaxSprite_Serialize);
    RSDK.CreateObject((Object**)&PathInverter, "PathInverter", sizeof(EntityPathInverter), sizeof(ObjectPathInverter), PathInverter_Update, PathInverter_LateUpdate,
                 PathInverter_StaticUpdate, PathInverter_Draw, PathInverter_Create, PathInverter_StageLoad, PathInverter_EditorDraw,
                 PathInverter_EditorLoad, PathInverter_Serialize);
    RSDK.CreateObject((Object**)&PauseMenu, "PauseMenu", sizeof(EntityPauseMenu), sizeof(ObjectPauseMenu), PauseMenu_Update, PauseMenu_LateUpdate,
                 PauseMenu_StaticUpdate, PauseMenu_Draw, PauseMenu_Create, PauseMenu_StageLoad, PauseMenu_EditorDraw, PauseMenu_EditorLoad,
                 PauseMenu_Serialize);
    RSDK.CreateObject((Object**)&PBL_Bumper, "PBL_Bumper", sizeof(EntityPBL_Bumper), sizeof(ObjectPBL_Bumper), PBL_Bumper_Update, PBL_Bumper_LateUpdate,
                 PBL_Bumper_StaticUpdate, PBL_Bumper_Draw, PBL_Bumper_Create, PBL_Bumper_StageLoad, PBL_Bumper_EditorDraw, PBL_Bumper_EditorLoad,
                 PBL_Bumper_Serialize);
    RSDK.CreateObject((Object**)&PBL_Camera, "PBL_Camera", sizeof(EntityPBL_Camera), sizeof(ObjectPBL_Camera), PBL_Camera_Update, PBL_Camera_LateUpdate,
                 PBL_Camera_StaticUpdate, PBL_Camera_Draw, PBL_Camera_Create, PBL_Camera_StageLoad, PBL_Camera_EditorDraw, PBL_Camera_EditorLoad,
                 PBL_Camera_Serialize);
    RSDK.CreateObject((Object**)&PBL_Crane, "PBL_Crane", sizeof(EntityPBL_Crane), sizeof(ObjectPBL_Crane), PBL_Crane_Update, PBL_Crane_LateUpdate,
                 PBL_Crane_StaticUpdate, PBL_Crane_Draw, PBL_Crane_Create, PBL_Crane_StageLoad, PBL_Crane_EditorDraw, PBL_Crane_EditorLoad,
                 PBL_Crane_Serialize);
    RSDK.CreateObject((Object**)&PBL_Flipper, "PBL_Flipper", sizeof(EntityPBL_Flipper), sizeof(ObjectPBL_Flipper), PBL_Flipper_Update, PBL_Flipper_LateUpdate,
                 PBL_Flipper_StaticUpdate, PBL_Flipper_Draw, PBL_Flipper_Create, PBL_Flipper_StageLoad, PBL_Flipper_EditorDraw, PBL_Flipper_EditorLoad,
                 PBL_Flipper_Serialize);
    RSDK.CreateObject((Object**)&PBL_HUD, "PBL_HUD", sizeof(EntityPBL_HUD), sizeof(ObjectPBL_HUD), PBL_HUD_Update, PBL_HUD_LateUpdate, PBL_HUD_StaticUpdate,
                 PBL_HUD_Draw, PBL_HUD_Create, PBL_HUD_StageLoad, PBL_HUD_EditorDraw, PBL_HUD_EditorLoad, PBL_HUD_Serialize);
    RSDK.CreateObject((Object**)&PBL_Player, "PBL_Player", sizeof(EntityPBL_Player), sizeof(ObjectPBL_Player), PBL_Player_Update, PBL_Player_LateUpdate,
                 PBL_Player_StaticUpdate, PBL_Player_Draw, PBL_Player_Create, PBL_Player_StageLoad, PBL_Player_EditorDraw, PBL_Player_EditorLoad,
                 PBL_Player_Serialize);
    RSDK.CreateObject((Object**)&PBL_Ring, "PBL_Ring", sizeof(EntityPBL_Ring), sizeof(ObjectPBL_Ring), PBL_Ring_Update, PBL_Ring_LateUpdate, PBL_Ring_StaticUpdate,
                 PBL_Ring_Draw, PBL_Ring_Create, PBL_Ring_StageLoad, PBL_Ring_EditorDraw, PBL_Ring_EditorLoad, PBL_Ring_Serialize);
    RSDK.CreateObject((Object**)&PBL_Sector, "PBL_Sector", sizeof(EntityPBL_Sector), sizeof(ObjectPBL_Sector), PBL_Sector_Update, PBL_Sector_LateUpdate,
                 PBL_Sector_StaticUpdate, PBL_Sector_Draw, PBL_Sector_Create, PBL_Sector_StageLoad, PBL_Sector_EditorDraw, PBL_Sector_EditorLoad,
                 PBL_Sector_Serialize);
    RSDK.CreateObject((Object**)&PBL_Setup, "PBL_Setup", sizeof(EntityPBL_Setup), sizeof(ObjectPBL_Setup), PBL_Setup_Update, PBL_Setup_LateUpdate,
                 PBL_Setup_StaticUpdate, PBL_Setup_Draw, PBL_Setup_Create, PBL_Setup_StageLoad, PBL_Setup_EditorDraw, PBL_Setup_EditorLoad,
                 PBL_Setup_Serialize);
    RSDK.CreateObject((Object**)&PBL_TargetBumper, "PBL_TargetBumper", sizeof(EntityPBL_TargetBumper), sizeof(ObjectPBL_TargetBumper), PBL_TargetBumper_Update,
                 PBL_TargetBumper_LateUpdate, PBL_TargetBumper_StaticUpdate, PBL_TargetBumper_Draw, PBL_TargetBumper_Create,
                 PBL_TargetBumper_StageLoad, PBL_TargetBumper_EditorDraw, PBL_TargetBumper_EditorLoad, PBL_TargetBumper_Serialize);
    RSDK.CreateObject((Object**)&PetalPile, "PetalPile", sizeof(EntityPetalPile), sizeof(ObjectPetalPile), PetalPile_Update, PetalPile_LateUpdate,
                 PetalPile_StaticUpdate, PetalPile_Draw, PetalPile_Create, PetalPile_StageLoad, PetalPile_EditorDraw, PetalPile_EditorLoad,
                 PetalPile_Serialize);
    RSDK.CreateObject((Object**)&PhantomEgg, "PhantomEgg", sizeof(EntityPhantomEgg), sizeof(ObjectPhantomEgg), PhantomEgg_Update, PhantomEgg_LateUpdate,
                 PhantomEgg_StaticUpdate, PhantomEgg_Draw, PhantomEgg_Create, PhantomEgg_StageLoad, PhantomEgg_EditorDraw, PhantomEgg_EditorLoad,
                 PhantomEgg_Serialize);
    RSDK.CreateObject((Object**)&PhantomGunner, "PhantomGunner", sizeof(EntityPhantomGunner), sizeof(ObjectPhantomGunner), PhantomGunner_Update,
                 PhantomGunner_LateUpdate, PhantomGunner_StaticUpdate, PhantomGunner_Draw, PhantomGunner_Create, PhantomGunner_StageLoad,
                 PhantomGunner_EditorDraw, PhantomGunner_EditorLoad, PhantomGunner_Serialize);
    RSDK.CreateObject((Object**)&PhantomHand, "PhantomHand", sizeof(EntityPhantomHand), sizeof(ObjectPhantomHand), PhantomHand_Update, PhantomHand_LateUpdate,
                 PhantomHand_StaticUpdate, PhantomHand_Draw, PhantomHand_Create, PhantomHand_StageLoad, PhantomHand_EditorDraw, PhantomHand_EditorLoad,
                 PhantomHand_Serialize);
    RSDK.CreateObject((Object**)&PhantomKing, "PhantomKing", sizeof(EntityPhantomKing), sizeof(ObjectPhantomKing), PhantomKing_Update, PhantomKing_LateUpdate,
                 PhantomKing_StaticUpdate, PhantomKing_Draw, PhantomKing_Create, PhantomKing_StageLoad, PhantomKing_EditorDraw, PhantomKing_EditorLoad,
                 PhantomKing_Serialize);
    RSDK.CreateObject((Object**)&PhantomMissile, "PhantomMissile", sizeof(EntityPhantomMissile), sizeof(ObjectPhantomMissile), PhantomMissile_Update,
                 PhantomMissile_LateUpdate, PhantomMissile_StaticUpdate, PhantomMissile_Draw, PhantomMissile_Create, PhantomMissile_StageLoad,
                 PhantomMissile_EditorDraw, PhantomMissile_EditorLoad, PhantomMissile_Serialize);
    RSDK.CreateObject((Object**)&PhantomMystic, "PhantomMystic", sizeof(EntityPhantomMystic), sizeof(ObjectPhantomMystic), PhantomMystic_Update,
                 PhantomMystic_LateUpdate, PhantomMystic_StaticUpdate, PhantomMystic_Draw, PhantomMystic_Create, PhantomMystic_StageLoad,
                 PhantomMystic_EditorDraw, PhantomMystic_EditorLoad, PhantomMystic_Serialize);
    RSDK.CreateObject((Object**)&PhantomRider, "PhantomRider", sizeof(EntityPhantomRider), sizeof(ObjectPhantomRider), PhantomRider_Update, PhantomRider_LateUpdate,
                 PhantomRider_StaticUpdate, PhantomRider_Draw, PhantomRider_Create, PhantomRider_StageLoad, PhantomRider_EditorDraw,
                 PhantomRider_EditorLoad, PhantomRider_Serialize);
    RSDK.CreateObject((Object**)&PhantomRuby, "PhantomRuby", sizeof(EntityPhantomRuby), sizeof(ObjectPhantomRuby), PhantomRuby_Update, PhantomRuby_LateUpdate,
                 PhantomRuby_StaticUpdate, PhantomRuby_Draw, PhantomRuby_Create, PhantomRuby_StageLoad, PhantomRuby_EditorDraw, PhantomRuby_EditorLoad,
                 PhantomRuby_Serialize);
    RSDK.CreateObject((Object**)&PhantomShield, "PhantomShield", sizeof(EntityPhantomShield), sizeof(ObjectPhantomShield), PhantomShield_Update,
                 PhantomShield_LateUpdate, PhantomShield_StaticUpdate, PhantomShield_Draw, PhantomShield_Create, PhantomShield_StageLoad,
                 PhantomShield_EditorDraw, PhantomShield_EditorLoad, PhantomShield_Serialize);
    RSDK.CreateObject((Object**)&PhantomShinobi, "PhantomShinobi", sizeof(EntityPhantomShinobi), sizeof(ObjectPhantomShinobi), PhantomShinobi_Update,
                 PhantomShinobi_LateUpdate, PhantomShinobi_StaticUpdate, PhantomShinobi_Draw, PhantomShinobi_Create, PhantomShinobi_StageLoad,
                 PhantomShinobi_EditorDraw, PhantomShinobi_EditorLoad, PhantomShinobi_Serialize);
    RSDK.CreateObject((Object**)&PimPom, "PimPom", sizeof(EntityPimPom), sizeof(ObjectPimPom), PimPom_Update, PimPom_LateUpdate, PimPom_StaticUpdate, PimPom_Draw,
                 PimPom_Create, PimPom_StageLoad, PimPom_EditorDraw, PimPom_EditorLoad, PimPom_Serialize);
    RSDK.CreateObject((Object**)&Pinata, "Pinata", sizeof(EntityPinata), sizeof(ObjectPinata), Pinata_Update, Pinata_LateUpdate, Pinata_StaticUpdate, Pinata_Draw,
                 Pinata_Create, Pinata_StageLoad, Pinata_EditorDraw, Pinata_EditorLoad, Pinata_Serialize);
    RSDK.CreateObject((Object**)&Piston, "Piston", sizeof(EntityPiston), sizeof(ObjectPiston), Piston_Update, Piston_LateUpdate, Piston_StaticUpdate, Piston_Draw,
                 Piston_Create, Piston_StageLoad, Piston_EditorDraw, Piston_EditorLoad, Piston_Serialize);
    RSDK.CreateObject((Object**)&PKingAttack, "PKingAttack", sizeof(EntityPKingAttack), sizeof(ObjectPKingAttack), PKingAttack_Update, PKingAttack_LateUpdate,
                 PKingAttack_StaticUpdate, PKingAttack_Draw, PKingAttack_Create, PKingAttack_StageLoad, PKingAttack_EditorDraw, PKingAttack_EditorLoad,
                 PKingAttack_Serialize);
    RSDK.CreateObject((Object**)&PlaneSeeSaw, "PlaneSeeSaw", sizeof(EntityPlaneSeeSaw), sizeof(ObjectPlaneSeeSaw), PlaneSeeSaw_Update, PlaneSeeSaw_LateUpdate,
                 PlaneSeeSaw_StaticUpdate, PlaneSeeSaw_Draw, PlaneSeeSaw_Create, PlaneSeeSaw_StageLoad, PlaneSeeSaw_EditorDraw, PlaneSeeSaw_EditorLoad,
                 PlaneSeeSaw_Serialize);
    RSDK.CreateObject((Object**)&PlaneSwitch, "PlaneSwitch", sizeof(EntityPlaneSwitch), sizeof(ObjectPlaneSwitch), PlaneSwitch_Update, PlaneSwitch_LateUpdate,
                 PlaneSwitch_StaticUpdate, PlaneSwitch_Draw, PlaneSwitch_Create, PlaneSwitch_StageLoad, PlaneSwitch_EditorDraw, PlaneSwitch_EditorLoad,
                 PlaneSwitch_Serialize);
    RSDK.CreateObject((Object**)&Platform, "Platform", sizeof(EntityPlatform), sizeof(ObjectPlatform), Platform_Update, Platform_LateUpdate, Platform_StaticUpdate,
                 Platform_Draw, Platform_Create, Platform_StageLoad, Platform_EditorDraw, Platform_EditorLoad, Platform_Serialize);
    RSDK.CreateObject((Object**)&PlatformControl, "PlatformControl", sizeof(EntityPlatformControl), sizeof(ObjectPlatformControl), PlatformControl_Update,
                 PlatformControl_LateUpdate, PlatformControl_StaticUpdate, PlatformControl_Draw, PlatformControl_Create, PlatformControl_StageLoad,
                 PlatformControl_EditorDraw, PlatformControl_EditorLoad, PlatformControl_Serialize);
    RSDK.CreateObject((Object**)&PlatformNode, "PlatformNode", sizeof(EntityPlatformNode), sizeof(ObjectPlatformNode), PlatformNode_Update, PlatformNode_LateUpdate,
                 PlatformNode_StaticUpdate, PlatformNode_Draw, PlatformNode_Create, PlatformNode_StageLoad, PlatformNode_EditorDraw,
                 PlatformNode_EditorLoad, PlatformNode_Serialize);
    RSDK.CreateObject((Object**)&Player, "Player", sizeof(EntityPlayer), sizeof(ObjectPlayer), Player_Update, Player_LateUpdate, Player_StaticUpdate, Player_Draw,
                 Player_Create, Player_StageLoad, Player_EditorDraw, Player_EditorLoad, Player_Serialize);
    RSDK.CreateObject((Object**)&PlayerProbe, "PlayerProbe", sizeof(EntityPlayerProbe), sizeof(ObjectPlayerProbe), PlayerProbe_Update, PlayerProbe_LateUpdate,
                 PlayerProbe_StaticUpdate, PlayerProbe_Draw, PlayerProbe_Create, PlayerProbe_StageLoad, PlayerProbe_EditorDraw, PlayerProbe_EditorLoad,
                 PlayerProbe_Serialize);
    RSDK.CreateObject((Object**)&PohBee, "PohBee", sizeof(EntityPohBee), sizeof(ObjectPohBee), PohBee_Update, PohBee_LateUpdate, PohBee_StaticUpdate, PohBee_Draw,
                 PohBee_Create, PohBee_StageLoad, PohBee_EditorDraw, PohBee_EditorLoad, PohBee_Serialize);
    RSDK.CreateObject((Object**)&Pointdexter, "Pointdexter", sizeof(EntityPointdexter), sizeof(ObjectPointdexter), Pointdexter_Update, Pointdexter_LateUpdate,
                 Pointdexter_StaticUpdate, Pointdexter_Draw, Pointdexter_Create, Pointdexter_StageLoad, Pointdexter_EditorDraw, Pointdexter_EditorLoad,
                 Pointdexter_Serialize);
    RSDK.CreateObject((Object**)&PopcornKernel, "PopcornKernel", sizeof(EntityPopcornKernel), sizeof(ObjectPopcornKernel), PopcornKernel_Update,
                 PopcornKernel_LateUpdate, PopcornKernel_StaticUpdate, PopcornKernel_Draw, PopcornKernel_Create, PopcornKernel_StageLoad,
                 PopcornKernel_EditorDraw, PopcornKernel_EditorLoad, PopcornKernel_Serialize);
    RSDK.CreateObject((Object**)&PopcornMachine, "PopcornMachine", sizeof(EntityPopcornMachine), sizeof(ObjectPopcornMachine), PopcornMachine_Update,
                 PopcornMachine_LateUpdate, PopcornMachine_StaticUpdate, PopcornMachine_Draw, PopcornMachine_Create, PopcornMachine_StageLoad,
                 PopcornMachine_EditorDraw, PopcornMachine_EditorLoad, PopcornMachine_Serialize);
    RSDK.CreateObject((Object**)&PopOut, "PopOut", sizeof(EntityPopOut), sizeof(ObjectPopOut), PopOut_Update, PopOut_LateUpdate, PopOut_StaticUpdate, PopOut_Draw,
                 PopOut_Create, PopOut_StageLoad, PopOut_EditorDraw, PopOut_EditorLoad, PopOut_Serialize);
    RSDK.CreateObject((Object**)&Press, "Press", sizeof(EntityPress), sizeof(ObjectPress), Press_Update, Press_LateUpdate, Press_StaticUpdate, Press_Draw,
                 Press_Create, Press_StageLoad, Press_EditorDraw, Press_EditorLoad, Press_Serialize);
    RSDK.CreateObject((Object**)&PrintBlock, "PrintBlock", sizeof(EntityPrintBlock), sizeof(ObjectPrintBlock), PrintBlock_Update, PrintBlock_LateUpdate,
                 PrintBlock_StaticUpdate, PrintBlock_Draw, PrintBlock_Create, PrintBlock_StageLoad, PrintBlock_EditorDraw, PrintBlock_EditorLoad,
                 PrintBlock_Serialize);
    RSDK.CreateObject((Object**)&Projectile, "Projectile", sizeof(EntityProjectile), sizeof(ObjectProjectile), Projectile_Update, Projectile_LateUpdate,
                 Projectile_StaticUpdate, Projectile_Draw, Projectile_Create, Projectile_StageLoad, Projectile_EditorDraw, Projectile_EditorLoad,
                 Projectile_Serialize);
    RSDK.CreateObject((Object**)&Propeller, "Propeller", sizeof(EntityPropeller), sizeof(ObjectPropeller), Propeller_Update, Propeller_LateUpdate,
                 Propeller_StaticUpdate, Propeller_Draw, Propeller_Create, Propeller_StageLoad, Propeller_EditorDraw, Propeller_EditorLoad,
                 Propeller_Serialize);
    RSDK.CreateObject((Object**)&PropellerShaft, "PropellerShaft", sizeof(EntityPropellerShaft), sizeof(ObjectPropellerShaft), PropellerShaft_Update,
                 PropellerShaft_LateUpdate, PropellerShaft_StaticUpdate, PropellerShaft_Draw, PropellerShaft_Create, PropellerShaft_StageLoad,
                 PropellerShaft_EditorDraw, PropellerShaft_EditorLoad, PropellerShaft_Serialize);
    RSDK.CreateObject((Object**)&PSZ1Intro, "PSZ1Intro", sizeof(EntityPSZ1Intro), sizeof(ObjectPSZ1Intro), PSZ1Intro_Update, PSZ1Intro_LateUpdate,
                 PSZ1Intro_StaticUpdate, PSZ1Intro_Draw, PSZ1Intro_Create, PSZ1Intro_StageLoad, PSZ1Intro_EditorDraw, PSZ1Intro_EditorLoad,
                 PSZ1Intro_Serialize);
    RSDK.CreateObject((Object**)&PSZ1Setup, "PSZ1Setup", sizeof(EntityPSZ1Setup), sizeof(ObjectPSZ1Setup), PSZ1Setup_Update, PSZ1Setup_LateUpdate,
                 PSZ1Setup_StaticUpdate, PSZ1Setup_Draw, PSZ1Setup_Create, PSZ1Setup_StageLoad, PSZ1Setup_EditorDraw, PSZ1Setup_EditorLoad,
                 PSZ1Setup_Serialize);
    RSDK.CreateObject((Object**)&PSZ2Intro, "PSZ2Intro", sizeof(EntityPSZ2Intro), sizeof(ObjectPSZ2Intro), PSZ2Intro_Update, PSZ2Intro_LateUpdate,
                 PSZ2Intro_StaticUpdate, PSZ2Intro_Draw, PSZ2Intro_Create, PSZ2Intro_StageLoad, PSZ2Intro_EditorDraw, PSZ2Intro_EditorLoad,
                 PSZ2Intro_Serialize);
    RSDK.CreateObject((Object**)&PSZ2Outro, "PSZ2Outro", sizeof(EntityPSZ2Outro), sizeof(ObjectPSZ2Outro), PSZ2Outro_Update, PSZ2Outro_LateUpdate,
                 PSZ2Outro_StaticUpdate, PSZ2Outro_Draw, PSZ2Outro_Create, PSZ2Outro_StageLoad, PSZ2Outro_EditorDraw, PSZ2Outro_EditorLoad,
                 PSZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&PSZ2Setup, "PSZ2Setup", sizeof(EntityPSZ2Setup), sizeof(ObjectPSZ2Setup), PSZ2Setup_Update, PSZ2Setup_LateUpdate,
                 PSZ2Setup_StaticUpdate, PSZ2Setup_Draw, PSZ2Setup_Create, PSZ2Setup_StageLoad, PSZ2Setup_EditorDraw, PSZ2Setup_EditorLoad,
                 PSZ2Setup_Serialize);
    RSDK.CreateObject((Object**)&PSZDoor, "PSZDoor", sizeof(EntityPSZDoor), sizeof(ObjectPSZDoor), PSZDoor_Update, PSZDoor_LateUpdate, PSZDoor_StaticUpdate,
                 PSZDoor_Draw, PSZDoor_Create, PSZDoor_StageLoad, PSZDoor_EditorDraw, PSZDoor_EditorLoad, PSZDoor_Serialize);
    RSDK.CreateObject((Object**)&PSZEggman, "PSZEggman", sizeof(EntityPSZEggman), sizeof(ObjectPSZEggman), PSZEggman_Update, PSZEggman_LateUpdate,
                 PSZEggman_StaticUpdate, PSZEggman_Draw, PSZEggman_Create, PSZEggman_StageLoad, PSZEggman_EditorDraw, PSZEggman_EditorLoad,
                 PSZEggman_Serialize);
    RSDK.CreateObject((Object**)&PSZLauncher, "PSZLauncher", sizeof(EntityPSZLauncher), sizeof(ObjectPSZLauncher), PSZLauncher_Update, PSZLauncher_LateUpdate,
                 PSZLauncher_StaticUpdate, PSZLauncher_Draw, PSZLauncher_Create, PSZLauncher_StageLoad, PSZLauncher_EditorDraw, PSZLauncher_EditorLoad,
                 PSZLauncher_Serialize);
    RSDK.CreateObject((Object**)&PullChain, "PullChain", sizeof(EntityPullChain), sizeof(ObjectPullChain), PullChain_Update, PullChain_LateUpdate,
                 PullChain_StaticUpdate, PullChain_Draw, PullChain_Create, PullChain_StageLoad, PullChain_EditorDraw, PullChain_EditorLoad,
                 PullChain_Serialize);
    RSDK.CreateObject((Object**)&PullSwitch, "PullSwitch", sizeof(EntityPullSwitch), sizeof(ObjectPullSwitch), PullSwitch_Update, PullSwitch_LateUpdate,
                 PullSwitch_StaticUpdate, PullSwitch_Draw, PullSwitch_Create, PullSwitch_StageLoad, PullSwitch_EditorDraw, PullSwitch_EditorLoad,
                 PullSwitch_Serialize);
    RSDK.CreateObject((Object**)&PushSpring, "PushSpring", sizeof(EntityPushSpring), sizeof(ObjectPushSpring), PushSpring_Update, PushSpring_LateUpdate,
                 PushSpring_StaticUpdate, PushSpring_Draw, PushSpring_Create, PushSpring_StageLoad, PushSpring_EditorDraw, PushSpring_EditorLoad,
                 PushSpring_Serialize);
    RSDK.CreateObject((Object**)&PuyoAI, "PuyoAI", sizeof(EntityPuyoAI), sizeof(ObjectPuyoAI), PuyoAI_Update, PuyoAI_LateUpdate, PuyoAI_StaticUpdate, PuyoAI_Draw,
                 PuyoAI_Create, PuyoAI_StageLoad, PuyoAI_EditorDraw, PuyoAI_EditorLoad, PuyoAI_Serialize);
    RSDK.CreateObject((Object**)&PuyoAttack, "PuyoAttack", sizeof(EntityPuyoAttack), sizeof(ObjectPuyoAttack), PuyoAttack_Update, PuyoAttack_LateUpdate,
                 PuyoAttack_StaticUpdate, PuyoAttack_Draw, PuyoAttack_Create, PuyoAttack_StageLoad, PuyoAttack_EditorDraw, PuyoAttack_EditorLoad,
                 PuyoAttack_Serialize);
    RSDK.CreateObject((Object**)&PuyoBean, "PuyoBean", sizeof(EntityPuyoBean), sizeof(ObjectPuyoBean), PuyoBean_Update, PuyoBean_LateUpdate, PuyoBean_StaticUpdate,
                 PuyoBean_Draw, PuyoBean_Create, PuyoBean_StageLoad, PuyoBean_EditorDraw, PuyoBean_EditorLoad, PuyoBean_Serialize);
    RSDK.CreateObject((Object**)&PuyoGame, "PuyoGame", sizeof(EntityPuyoGame), sizeof(ObjectPuyoGame), PuyoGame_Update, PuyoGame_LateUpdate, PuyoGame_StaticUpdate,
                 PuyoGame_Draw, PuyoGame_Create, PuyoGame_StageLoad, PuyoGame_EditorDraw, PuyoGame_EditorLoad, PuyoGame_Serialize);
    RSDK.CreateObject((Object**)&PuyoIndicator, "PuyoIndicator", sizeof(EntityPuyoIndicator), sizeof(ObjectPuyoIndicator), PuyoIndicator_Update,
                 PuyoIndicator_LateUpdate, PuyoIndicator_StaticUpdate, PuyoIndicator_Draw, PuyoIndicator_Create, PuyoIndicator_StageLoad,
                 PuyoIndicator_EditorDraw, PuyoIndicator_EditorLoad, PuyoIndicator_Serialize);
    RSDK.CreateObject((Object**)&PuyoLabel, "PuyoLabel", sizeof(EntityPuyoLabel), sizeof(ObjectPuyoLabel), PuyoLabel_Update, PuyoLabel_LateUpdate,
                 PuyoLabel_StaticUpdate, PuyoLabel_Draw, PuyoLabel_Create, PuyoLabel_StageLoad, PuyoLabel_EditorDraw, PuyoLabel_EditorLoad,
                 PuyoLabel_Serialize);
    RSDK.CreateObject((Object**)&PuyoLevelSelect, "PuyoLevelSelect", sizeof(EntityPuyoLevelSelect), sizeof(ObjectPuyoLevelSelect), PuyoLevelSelect_Update,
                 PuyoLevelSelect_LateUpdate, PuyoLevelSelect_StaticUpdate, PuyoLevelSelect_Draw, PuyoLevelSelect_Create, PuyoLevelSelect_StageLoad,
                 PuyoLevelSelect_EditorDraw, PuyoLevelSelect_EditorLoad, PuyoLevelSelect_Serialize);
    RSDK.CreateObject((Object**)&PuyoMatch, "PuyoMatch", sizeof(EntityPuyoMatch), sizeof(ObjectPuyoMatch), PuyoMatch_Update, PuyoMatch_LateUpdate,
                 PuyoMatch_StaticUpdate, PuyoMatch_Draw, PuyoMatch_Create, PuyoMatch_StageLoad, PuyoMatch_EditorDraw, PuyoMatch_EditorLoad,
                 PuyoMatch_Serialize);
    RSDK.CreateObject((Object**)&PuyoScore, "PuyoScore", sizeof(EntityPuyoScore), sizeof(ObjectPuyoScore), PuyoScore_Update, PuyoScore_LateUpdate,
                 PuyoScore_StaticUpdate, PuyoScore_Draw, PuyoScore_Create, PuyoScore_StageLoad, PuyoScore_EditorDraw, PuyoScore_EditorLoad,
                 PuyoScore_Serialize);
    RSDK.CreateObject((Object**)&Rattlekiller, "Rattlekiller", sizeof(EntityRattlekiller), sizeof(ObjectRattlekiller), Rattlekiller_Update, Rattlekiller_LateUpdate,
                 Rattlekiller_StaticUpdate, Rattlekiller_Draw, Rattlekiller_Create, Rattlekiller_StageLoad, Rattlekiller_EditorDraw,
                 Rattlekiller_EditorLoad, Rattlekiller_Serialize);
    RSDK.CreateObject((Object**)&Reagent, "Reagent", sizeof(EntityReagent), sizeof(ObjectReagent), Reagent_Update, Reagent_LateUpdate, Reagent_StaticUpdate,
                 Reagent_Draw, Reagent_Create, Reagent_StageLoad, Reagent_EditorDraw, Reagent_EditorLoad, Reagent_Serialize);
    RSDK.CreateObject((Object**)&ReplayDB, "ReplayDB", sizeof(EntityReplayDB), sizeof(ObjectReplayDB), ReplayDB_Update, ReplayDB_LateUpdate, ReplayDB_StaticUpdate,
                 ReplayDB_Draw, ReplayDB_Create, ReplayDB_StageLoad, ReplayDB_EditorDraw, ReplayDB_EditorLoad, ReplayDB_Serialize);
    RSDK.CreateObject((Object**)&ReplayRecorder, "ReplayRecorder", sizeof(EntityReplayRecorder), sizeof(ObjectReplayRecorder), ReplayRecorder_Update,
                 ReplayRecorder_LateUpdate, ReplayRecorder_StaticUpdate, ReplayRecorder_Draw, ReplayRecorder_Create, ReplayRecorder_StageLoad,
                 ReplayRecorder_EditorDraw, ReplayRecorder_EditorLoad, ReplayRecorder_Serialize);
    RSDK.CreateObject((Object**)&Rexon, "Rexon", sizeof(EntityRexon), sizeof(ObjectRexon), Rexon_Update, Rexon_LateUpdate, Rexon_StaticUpdate, Rexon_Draw,
                 Rexon_Create, Rexon_StageLoad, Rexon_EditorDraw, Rexon_EditorLoad, Rexon_Serialize);
    RSDK.CreateObject((Object**)&Rhinobot, "Rhinobot", sizeof(EntityRhinobot), sizeof(ObjectRhinobot), Rhinobot_Update, Rhinobot_LateUpdate, Rhinobot_StaticUpdate,
                 Rhinobot_Draw, Rhinobot_Create, Rhinobot_StageLoad, Rhinobot_EditorDraw, Rhinobot_EditorLoad, Rhinobot_Serialize);
    RSDK.CreateObject((Object**)&Ring, "Ring", sizeof(EntityRing), sizeof(ObjectRing), Ring_Update, Ring_LateUpdate, Ring_StaticUpdate, Ring_Draw, Ring_Create,
                 Ring_StageLoad, Ring_EditorDraw, Ring_EditorLoad, Ring_Serialize);
    RSDK.CreateObject((Object**)&RingField, "RingField", sizeof(EntityRingField), sizeof(ObjectRingField), RingField_Update, RingField_LateUpdate,
                 RingField_StaticUpdate, RingField_Draw, RingField_Create, RingField_StageLoad, RingField_EditorDraw, RingField_EditorLoad,
                 RingField_Serialize);
    RSDK.CreateObject((Object**)&RisingLava, "RisingLava", sizeof(EntityRisingLava), sizeof(ObjectRisingLava), RisingLava_Update, RisingLava_LateUpdate,
                 RisingLava_StaticUpdate, RisingLava_Draw, RisingLava_Create, RisingLava_StageLoad, RisingLava_EditorDraw, RisingLava_EditorLoad,
                 RisingLava_Serialize);
    RSDK.CreateObject((Object**)&RockDrill, "RockDrill", sizeof(EntityRockDrill), sizeof(ObjectRockDrill), RockDrill_Update, RockDrill_LateUpdate,
                 RockDrill_StaticUpdate, RockDrill_Draw, RockDrill_Create, RockDrill_StageLoad, RockDrill_EditorDraw, RockDrill_EditorLoad,
                 RockDrill_Serialize);
    RSDK.CreateObject((Object**)&RockemSockem, "RockemSockem", sizeof(EntityRockemSockem), sizeof(ObjectRockemSockem), RockemSockem_Update, RockemSockem_LateUpdate,
                 RockemSockem_StaticUpdate, RockemSockem_Draw, RockemSockem_Create, RockemSockem_StageLoad, RockemSockem_EditorDraw,
                 RockemSockem_EditorLoad, RockemSockem_Serialize);
    RSDK.CreateObject((Object**)&RollerMKII, "RollerMKII", sizeof(EntityRollerMKII), sizeof(ObjectRollerMKII), RollerMKII_Update, RollerMKII_LateUpdate,
                 RollerMKII_StaticUpdate, RollerMKII_Draw, RollerMKII_Create, RollerMKII_StageLoad, RollerMKII_EditorDraw, RollerMKII_EditorLoad,
                 RollerMKII_Serialize);
    RSDK.CreateObject((Object**)&RotatingSpikes, "RotatingSpikes", sizeof(EntityRotatingSpikes), sizeof(ObjectRotatingSpikes), RotatingSpikes_Update,
                 RotatingSpikes_LateUpdate, RotatingSpikes_StaticUpdate, RotatingSpikes_Draw, RotatingSpikes_Create, RotatingSpikes_StageLoad,
                 RotatingSpikes_EditorDraw, RotatingSpikes_EditorLoad, RotatingSpikes_Serialize);
    RSDK.CreateObject((Object**)&RotatingStair, "RotatingStair", sizeof(EntityRotatingStair), sizeof(ObjectRotatingStair), RotatingStair_Update,
                 RotatingStair_LateUpdate, RotatingStair_StaticUpdate, RotatingStair_Draw, RotatingStair_Create, RotatingStair_StageLoad,
                 RotatingStair_EditorDraw, RotatingStair_EditorLoad, RotatingStair_Serialize);
    RSDK.CreateObject((Object**)&RPlaneShifter, "RPlaneShifter", sizeof(EntityRPlaneShifter), sizeof(ObjectRPlaneShifter), RPlaneShifter_Update,
                 RPlaneShifter_LateUpdate, RPlaneShifter_StaticUpdate, RPlaneShifter_Draw, RPlaneShifter_Create, RPlaneShifter_StageLoad,
                 RPlaneShifter_EditorDraw, RPlaneShifter_EditorLoad, RPlaneShifter_Serialize);
    RSDK.CreateObject((Object**)&RTeleporter, "RTeleporter", sizeof(EntityRTeleporter), sizeof(ObjectRTeleporter), RTeleporter_Update, RTeleporter_LateUpdate,
                 RTeleporter_StaticUpdate, RTeleporter_Draw, RTeleporter_Create, RTeleporter_StageLoad, RTeleporter_EditorDraw, RTeleporter_EditorLoad,
                 RTeleporter_Serialize);
    RSDK.CreateObject((Object**)&RubyPortal, "RubyPortal", sizeof(EntityRubyPortal), sizeof(ObjectRubyPortal), RubyPortal_Update, RubyPortal_LateUpdate,
                 RubyPortal_StaticUpdate, RubyPortal_Draw, RubyPortal_Create, RubyPortal_StageLoad, RubyPortal_EditorDraw, RubyPortal_EditorLoad,
                 RubyPortal_Serialize);
    RSDK.CreateObjectContainer((Object **)&SaveGame, "SaveGame", sizeof(ObjectSaveGame));
    RSDK.CreateObject((Object**)&Scarab, "Scarab", sizeof(EntityScarab), sizeof(ObjectScarab), Scarab_Update, Scarab_LateUpdate, Scarab_StaticUpdate, Scarab_Draw, Scarab_Create, Scarab_StageLoad, Scarab_EditorDraw, Scarab_EditorLoad, Scarab_Serialize);
    RSDK.CreateObject((Object**)&SchrodingersCapsule, "SchrodingersCapsule", sizeof(EntitySchrodingersCapsule), sizeof(ObjectSchrodingersCapsule),
                 SchrodingersCapsule_Update, SchrodingersCapsule_LateUpdate, SchrodingersCapsule_StaticUpdate, SchrodingersCapsule_Draw,
                 SchrodingersCapsule_Create, SchrodingersCapsule_StageLoad, SchrodingersCapsule_EditorDraw, SchrodingersCapsule_EditorLoad,
                 SchrodingersCapsule_Serialize);
    RSDK.CreateObject((Object**)&ScoreBonus, "ScoreBonus", sizeof(EntityScoreBonus), sizeof(ObjectScoreBonus), ScoreBonus_Update, ScoreBonus_LateUpdate,
                 ScoreBonus_StaticUpdate, ScoreBonus_Draw, ScoreBonus_Create, ScoreBonus_StageLoad, ScoreBonus_EditorDraw, ScoreBonus_EditorLoad,
                 ScoreBonus_Serialize);
    RSDK.CreateObject((Object**)&ScrewMobile, "ScrewMobile", sizeof(EntityScrewMobile), sizeof(ObjectScrewMobile), ScrewMobile_Update, ScrewMobile_LateUpdate,
                 ScrewMobile_StaticUpdate, ScrewMobile_Draw, ScrewMobile_Create, ScrewMobile_StageLoad, ScrewMobile_EditorDraw, ScrewMobile_EditorLoad,
                 ScrewMobile_Serialize);
    RSDK.CreateObject((Object**)&SDashWheel, "SDashWheel", sizeof(EntitySDashWheel), sizeof(ObjectSDashWheel), SDashWheel_Update, SDashWheel_LateUpdate,
                 SDashWheel_StaticUpdate, SDashWheel_Draw, SDashWheel_Create, SDashWheel_StageLoad, SDashWheel_EditorDraw, SDashWheel_EditorLoad,
                 SDashWheel_Serialize);
    RSDK.CreateObject((Object**)&SeeSaw, "SeeSaw", sizeof(EntitySeeSaw), sizeof(ObjectSeeSaw), SeeSaw_Update, SeeSaw_LateUpdate, SeeSaw_StaticUpdate, SeeSaw_Draw,
                 SeeSaw_Create, SeeSaw_StageLoad, SeeSaw_EditorDraw, SeeSaw_EditorLoad, SeeSaw_Serialize);
    RSDK.CreateObject((Object**)&SeltzerBottle, "SeltzerBottle", sizeof(EntitySeltzerBottle), sizeof(ObjectSeltzerBottle), SeltzerBottle_Update,
                 SeltzerBottle_LateUpdate, SeltzerBottle_StaticUpdate, SeltzerBottle_Draw, SeltzerBottle_Create, SeltzerBottle_StageLoad,
                 SeltzerBottle_EditorDraw, SeltzerBottle_EditorLoad, SeltzerBottle_Serialize);
    RSDK.CreateObject((Object**)&SeltzerWater, "SeltzerWater", sizeof(EntitySeltzerWater), sizeof(ObjectSeltzerWater), SeltzerWater_Update, SeltzerWater_LateUpdate,
                 SeltzerWater_StaticUpdate, SeltzerWater_Draw, SeltzerWater_Create, SeltzerWater_StageLoad, SeltzerWater_EditorDraw,
                 SeltzerWater_EditorLoad, SeltzerWater_Serialize);
    RSDK.CreateObject((Object**)&SentryBug, "SentryBug", sizeof(EntitySentryBug), sizeof(ObjectSentryBug), SentryBug_Update, SentryBug_LateUpdate,
                 SentryBug_StaticUpdate, SentryBug_Draw, SentryBug_Create, SentryBug_StageLoad, SentryBug_EditorDraw, SentryBug_EditorLoad,
                 SentryBug_Serialize);
    RSDK.CreateObject((Object**)&Shield, "Shield", sizeof(EntityShield), sizeof(ObjectShield), Shield_Update, Shield_LateUpdate, Shield_StaticUpdate, Shield_Draw,
                 Shield_Create, Shield_StageLoad, Shield_EditorDraw, Shield_EditorLoad, Shield_Serialize);
    RSDK.CreateObject((Object**)&Shiversaw, "Shiversaw", sizeof(EntityShiversaw), sizeof(ObjectShiversaw), Shiversaw_Update, Shiversaw_LateUpdate,
                 Shiversaw_StaticUpdate, Shiversaw_Draw, Shiversaw_Create, Shiversaw_StageLoad, Shiversaw_EditorDraw, Shiversaw_EditorLoad,
                 Shiversaw_Serialize);
    RSDK.CreateObject((Object**)&ShopWindow, "ShopWindow", sizeof(EntityShopWindow), sizeof(ObjectShopWindow), ShopWindow_Update, ShopWindow_LateUpdate,
                 ShopWindow_StaticUpdate, ShopWindow_Draw, ShopWindow_Create, ShopWindow_StageLoad, ShopWindow_EditorDraw, ShopWindow_EditorLoad,
                 ShopWindow_Serialize);
    RSDK.CreateObject((Object**)&Shuriken, "Shuriken", sizeof(EntityShuriken), sizeof(ObjectShuriken), Shuriken_Update, Shuriken_LateUpdate, Shuriken_StaticUpdate,
                 Shuriken_Draw, Shuriken_Create, Shuriken_StageLoad, Shuriken_EditorDraw, Shuriken_EditorLoad, Shuriken_Serialize);
    RSDK.CreateObject((Object**)&Shutterbug, "Shutterbug", sizeof(EntityShutterbug), sizeof(ObjectShutterbug), Shutterbug_Update, Shutterbug_LateUpdate,
                 Shutterbug_StaticUpdate, Shutterbug_Draw, Shutterbug_Create, Shutterbug_StageLoad, Shutterbug_EditorDraw, Shutterbug_EditorLoad,
                 Shutterbug_Serialize);
    RSDK.CreateObject((Object**)&SideBarrel, "SideBarrel", sizeof(EntitySideBarrel), sizeof(ObjectSideBarrel), SideBarrel_Update, SideBarrel_LateUpdate,
                 SideBarrel_StaticUpdate, SideBarrel_Draw, SideBarrel_Create, SideBarrel_StageLoad, SideBarrel_EditorDraw, SideBarrel_EditorLoad,
                 SideBarrel_Serialize);
    RSDK.CreateObject((Object**)&SignPost, "SignPost", sizeof(EntitySignPost), sizeof(ObjectSignPost), SignPost_Update, SignPost_LateUpdate, SignPost_StaticUpdate,
                 SignPost_Draw, SignPost_Create, SignPost_StageLoad, SignPost_EditorDraw, SignPost_EditorLoad, SignPost_Serialize);
    RSDK.CreateObject((Object**)&SilverSonic, "SilverSonic", sizeof(EntitySilverSonic), sizeof(ObjectSilverSonic), SilverSonic_Update, SilverSonic_LateUpdate,
                 SilverSonic_StaticUpdate, SilverSonic_Draw, SilverSonic_Create, SilverSonic_StageLoad, SilverSonic_EditorDraw, SilverSonic_EditorLoad,
                 SilverSonic_Serialize);
    RSDK.CreateObject((Object**)&SizeLaser, "SizeLaser", sizeof(EntitySizeLaser), sizeof(ObjectSizeLaser), SizeLaser_Update, SizeLaser_LateUpdate,
                 SizeLaser_StaticUpdate, SizeLaser_Draw, SizeLaser_Create, SizeLaser_StageLoad, SizeLaser_EditorDraw, SizeLaser_EditorLoad,
                 SizeLaser_Serialize);
    RSDK.CreateObject((Object**)&SkyTeleporter, "SkyTeleporter", sizeof(EntitySkyTeleporter), sizeof(ObjectSkyTeleporter), SkyTeleporter_Update,
                 SkyTeleporter_LateUpdate, SkyTeleporter_StaticUpdate, SkyTeleporter_Draw, SkyTeleporter_Create, SkyTeleporter_StageLoad,
                 SkyTeleporter_EditorDraw, SkyTeleporter_EditorLoad, SkyTeleporter_Serialize);
    RSDK.CreateObject((Object**)&Smog, "Smog", sizeof(EntitySmog), sizeof(ObjectSmog), Smog_Update, Smog_LateUpdate, Smog_StaticUpdate, Smog_Draw, Smog_Create,
                 Smog_StageLoad, Smog_EditorDraw, Smog_EditorLoad, Smog_Serialize);
    RSDK.CreateObject((Object**)&Snowflakes, "Snowflakes", sizeof(EntitySnowflakes), sizeof(ObjectSnowflakes), Snowflakes_Update, Snowflakes_LateUpdate,
                 Snowflakes_StaticUpdate, Snowflakes_Draw, Snowflakes_Create, Snowflakes_StageLoad, Snowflakes_EditorDraw, Snowflakes_EditorLoad,
                 Snowflakes_Serialize);
    RSDK.CreateObject((Object**)&Sol, "Sol", sizeof(EntitySol), sizeof(ObjectSol), Sol_Update, Sol_LateUpdate, Sol_StaticUpdate, Sol_Draw, Sol_Create, Sol_StageLoad,
                 Sol_EditorDraw, Sol_EditorLoad, Sol_Serialize);
    RSDK.CreateObject((Object**)&Soundboard, "Soundboard", sizeof(EntitySoundboard), sizeof(ObjectSoundboard), Soundboard_Update, Soundboard_LateUpdate,
                 Soundboard_StaticUpdate, Soundboard_Draw, Soundboard_Create, Soundboard_StageLoad, Soundboard_EditorDraw, Soundboard_EditorLoad,
                 Soundboard_Serialize);
    RSDK.CreateObject((Object**)&SP500, "SP500", sizeof(EntitySP500), sizeof(ObjectSP500), SP500_Update, SP500_LateUpdate, SP500_StaticUpdate, SP500_Draw,
                 SP500_Create, SP500_StageLoad, SP500_EditorDraw, SP500_EditorLoad, SP500_Serialize);
    RSDK.CreateObject((Object**)&SP500MkII, "SP500MkII", sizeof(EntitySP500MkII), sizeof(ObjectSP500MkII), SP500MkII_Update, SP500MkII_LateUpdate,
                 SP500MkII_StaticUpdate, SP500MkII_Draw, SP500MkII_Create, SP500MkII_StageLoad, SP500MkII_EditorDraw, SP500MkII_EditorLoad,
                 SP500MkII_Serialize);
    RSDK.CreateObject((Object**)&SparkRail, "SparkRail", sizeof(EntitySparkRail), sizeof(ObjectSparkRail), SparkRail_Update, SparkRail_LateUpdate,
                 SparkRail_StaticUpdate, SparkRail_Draw, SparkRail_Create, SparkRail_StageLoad, SparkRail_EditorDraw, SparkRail_EditorLoad,
                 SparkRail_Serialize);
    RSDK.CreateObject((Object**)&Spear, "Spear", sizeof(EntitySpear), sizeof(ObjectSpear), Spear_Update, Spear_LateUpdate, Spear_StaticUpdate, Spear_Draw,
                 Spear_Create, Spear_StageLoad, Spear_EditorDraw, Spear_EditorLoad, Spear_Serialize);
    RSDK.CreateObject((Object**)&SpecialClear, "SpecialClear", sizeof(EntitySpecialClear), sizeof(ObjectSpecialClear), SpecialClear_Update, SpecialClear_LateUpdate,
                 SpecialClear_StaticUpdate, SpecialClear_Draw, SpecialClear_Create, SpecialClear_StageLoad, SpecialClear_EditorDraw,
                 SpecialClear_EditorLoad, SpecialClear_Serialize);
    RSDK.CreateObject((Object**)&SpecialRing, "SpecialRing", sizeof(EntitySpecialRing), sizeof(ObjectSpecialRing), SpecialRing_Update, SpecialRing_LateUpdate,
                 SpecialRing_StaticUpdate, SpecialRing_Draw, SpecialRing_Create, SpecialRing_StageLoad, SpecialRing_EditorDraw, SpecialRing_EditorLoad,
                 SpecialRing_Serialize);
    RSDK.CreateObject((Object**)&SpeedBooster, "SpeedBooster", sizeof(EntitySpeedBooster), sizeof(ObjectSpeedBooster), SpeedBooster_Update, SpeedBooster_LateUpdate,
                 SpeedBooster_StaticUpdate, SpeedBooster_Draw, SpeedBooster_Create, SpeedBooster_StageLoad, SpeedBooster_EditorDraw,
                 SpeedBooster_EditorLoad, SpeedBooster_Serialize);
    RSDK.CreateObject((Object**)&SpeedGate, "SpeedGate", sizeof(EntitySpeedGate), sizeof(ObjectSpeedGate), SpeedGate_Update, SpeedGate_LateUpdate,
                 SpeedGate_StaticUpdate, SpeedGate_Draw, SpeedGate_Create, SpeedGate_StageLoad, SpeedGate_EditorDraw, SpeedGate_EditorLoad,
                 SpeedGate_Serialize);
    RSDK.CreateObject((Object**)&SpiderMobile, "SpiderMobile", sizeof(EntitySpiderMobile), sizeof(ObjectSpiderMobile), SpiderMobile_Update, SpiderMobile_LateUpdate,
                 SpiderMobile_StaticUpdate, SpiderMobile_Draw, SpiderMobile_Create, SpiderMobile_StageLoad, SpiderMobile_EditorDraw,
                 SpiderMobile_EditorLoad, SpiderMobile_Serialize);
    RSDK.CreateObject((Object**)&SpikeCorridor, "SpikeCorridor", sizeof(EntitySpikeCorridor), sizeof(ObjectSpikeCorridor), SpikeCorridor_Update,
                 SpikeCorridor_LateUpdate, SpikeCorridor_StaticUpdate, SpikeCorridor_Draw, SpikeCorridor_Create, SpikeCorridor_StageLoad,
                 SpikeCorridor_EditorDraw, SpikeCorridor_EditorLoad, SpikeCorridor_Serialize);
    RSDK.CreateObject((Object**)&SpikeCrusher, "SpikeCrusher", sizeof(EntitySpikeCrusher), sizeof(ObjectSpikeCrusher), SpikeCrusher_Update, SpikeCrusher_LateUpdate,
                 SpikeCrusher_StaticUpdate, SpikeCrusher_Draw, SpikeCrusher_Create, SpikeCrusher_StageLoad, SpikeCrusher_EditorDraw,
                 SpikeCrusher_EditorLoad, SpikeCrusher_Serialize);
    RSDK.CreateObject((Object**)&SpikeFlail, "SpikeFlail", sizeof(EntitySpikeFlail), sizeof(ObjectSpikeFlail), SpikeFlail_Update, SpikeFlail_LateUpdate,
                 SpikeFlail_StaticUpdate, SpikeFlail_Draw, SpikeFlail_Create, SpikeFlail_StageLoad, SpikeFlail_EditorDraw, SpikeFlail_EditorLoad,
                 SpikeFlail_Serialize);
    RSDK.CreateObject((Object**)&SpikeLog, "SpikeLog", sizeof(EntitySpikeLog), sizeof(ObjectSpikeLog), SpikeLog_Update, SpikeLog_LateUpdate, SpikeLog_StaticUpdate,
                 SpikeLog_Draw, SpikeLog_Create, SpikeLog_StageLoad, SpikeLog_EditorDraw, SpikeLog_EditorLoad, SpikeLog_Serialize);
    RSDK.CreateObject((Object**)&Spikes, "Spikes", sizeof(EntitySpikes), sizeof(ObjectSpikes), Spikes_Update, Spikes_LateUpdate, Spikes_StaticUpdate, Spikes_Draw,
                 Spikes_Create, Spikes_StageLoad, Spikes_EditorDraw, Spikes_EditorLoad, Spikes_Serialize);
    RSDK.CreateObject((Object**)&SpinBooster, "SpinBooster", sizeof(EntitySpinBooster), sizeof(ObjectSpinBooster), SpinBooster_Update, SpinBooster_LateUpdate,
                 SpinBooster_StaticUpdate, SpinBooster_Draw, SpinBooster_Create, SpinBooster_StageLoad, SpinBooster_EditorDraw, SpinBooster_EditorLoad,
                 SpinBooster_Serialize);
    RSDK.CreateObject((Object**)&SpinSign, "SpinSign", sizeof(EntitySpinSign), sizeof(ObjectSpinSign), SpinSign_Update, SpinSign_LateUpdate, SpinSign_StaticUpdate,
                 SpinSign_Draw, SpinSign_Create, SpinSign_StageLoad, SpinSign_EditorDraw, SpinSign_EditorLoad, SpinSign_Serialize);
    RSDK.CreateObject((Object**)&Spiny, "Spiny", sizeof(EntitySpiny), sizeof(ObjectSpiny), Spiny_Update, Spiny_LateUpdate, Spiny_StaticUpdate, Spiny_Draw,
                 Spiny_Create, Spiny_StageLoad, Spiny_EditorDraw, Spiny_EditorLoad, Spiny_Serialize);
    RSDK.CreateObject((Object**)&SpiralPlatform, "SpiralPlatform", sizeof(EntitySpiralPlatform), sizeof(ObjectSpiralPlatform), SpiralPlatform_Update,
                 SpiralPlatform_LateUpdate, SpiralPlatform_StaticUpdate, SpiralPlatform_Draw, SpiralPlatform_Create, SpiralPlatform_StageLoad,
                 SpiralPlatform_EditorDraw, SpiralPlatform_EditorLoad, SpiralPlatform_Serialize);
    RSDK.CreateObject((Object**)&Splats, "Splats", sizeof(EntitySplats), sizeof(ObjectSplats), Splats_Update, Splats_LateUpdate, Splats_StaticUpdate, Splats_Draw,
                 Splats_Create, Splats_StageLoad, Splats_EditorDraw, Splats_EditorLoad, Splats_Serialize);
    RSDK.CreateObject((Object**)&Spring, "Spring", sizeof(EntitySpring), sizeof(ObjectSpring), Spring_Update, Spring_LateUpdate, Spring_StaticUpdate, Spring_Draw,
                 Spring_Create, Spring_StageLoad, Spring_EditorDraw, Spring_EditorLoad, Spring_Serialize);
    RSDK.CreateObject((Object**)&Springboard, "Springboard", sizeof(EntitySpringboard), sizeof(ObjectSpringboard), Springboard_Update, Springboard_LateUpdate,
                 Springboard_StaticUpdate, Springboard_Draw, Springboard_Create, Springboard_StageLoad, Springboard_EditorDraw, Springboard_EditorLoad,
                 Springboard_Serialize);
    RSDK.CreateObject((Object**)&SPZ1Intro, "SPZ1Intro", sizeof(EntitySPZ1Intro), sizeof(ObjectSPZ1Intro), SPZ1Intro_Update, SPZ1Intro_LateUpdate,
                 SPZ1Intro_StaticUpdate, SPZ1Intro_Draw, SPZ1Intro_Create, SPZ1Intro_StageLoad, SPZ1Intro_EditorDraw, SPZ1Intro_EditorLoad,
                 SPZ1Intro_Serialize);
    RSDK.CreateObject((Object**)&SPZ1Setup, "SPZ1Setup", sizeof(EntitySPZ1Setup), sizeof(ObjectSPZ1Setup), SPZ1Setup_Update, SPZ1Setup_LateUpdate,
                 SPZ1Setup_StaticUpdate, SPZ1Setup_Draw, SPZ1Setup_Create, SPZ1Setup_StageLoad, SPZ1Setup_EditorDraw, SPZ1Setup_EditorLoad,
                 SPZ1Setup_Serialize);
    RSDK.CreateObject((Object**)&SPZ2Outro, "SPZ2Outro", sizeof(EntitySPZ2Outro), sizeof(ObjectSPZ2Outro), SPZ2Outro_Update, SPZ2Outro_LateUpdate,
                 SPZ2Outro_StaticUpdate, SPZ2Outro_Draw, SPZ2Outro_Create, SPZ2Outro_StageLoad, SPZ2Outro_EditorDraw, SPZ2Outro_EditorLoad,
                 SPZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&SPZ2Setup, "SPZ2Setup", sizeof(EntitySPZ2Setup), sizeof(ObjectSPZ2Setup), SPZ2Setup_Update, SPZ2Setup_LateUpdate,
                 SPZ2Setup_StaticUpdate, SPZ2Setup_Draw, SPZ2Setup_Create, SPZ2Setup_StageLoad, SPZ2Setup_EditorDraw, SPZ2Setup_EditorLoad,
                 SPZ2Setup_Serialize);
    RSDK.CreateObject((Object**)&SSZ1Intro, "SSZ1Intro", sizeof(EntitySSZ1Intro), sizeof(ObjectSSZ1Intro), SSZ1Intro_Update, SSZ1Intro_LateUpdate,
                 SSZ1Intro_StaticUpdate, SSZ1Intro_Draw, SSZ1Intro_Create, SSZ1Intro_StageLoad, SSZ1Intro_EditorDraw, SSZ1Intro_EditorLoad,
                 SSZ1Intro_Serialize);
    RSDK.CreateObject((Object**)&SSZ1Outro, "SSZ1Outro", sizeof(EntitySSZ1Outro), sizeof(ObjectSSZ1Outro), SSZ1Outro_Update, SSZ1Outro_LateUpdate,
                 SSZ1Outro_StaticUpdate, SSZ1Outro_Draw, SSZ1Outro_Create, SSZ1Outro_StageLoad, SSZ1Outro_EditorDraw, SSZ1Outro_EditorLoad,
                 SSZ1Outro_Serialize);
    RSDK.CreateObject((Object**)&SSZ1Setup, "SSZ1Setup", sizeof(EntitySSZ1Setup), sizeof(ObjectSSZ1Setup), SSZ1Setup_Update, SSZ1Setup_LateUpdate,
                 SSZ1Setup_StaticUpdate, SSZ1Setup_Draw, SSZ1Setup_Create, SSZ1Setup_StageLoad, SSZ1Setup_EditorDraw, SSZ1Setup_EditorLoad,
                 SSZ1Setup_Serialize);
    RSDK.CreateObject((Object**)&SSZ2Setup, "SSZ2Setup", sizeof(EntitySSZ2Setup), sizeof(ObjectSSZ2Setup), SSZ2Setup_Update, SSZ2Setup_LateUpdate,
                 SSZ2Setup_StaticUpdate, SSZ2Setup_Draw, SSZ2Setup_Create, SSZ2Setup_StageLoad, SSZ2Setup_EditorDraw, SSZ2Setup_EditorLoad,
                 SSZ2Setup_Serialize);
    RSDK.CreateObject((Object**)&SSZ3Cutscene, "SSZ3Cutscene", sizeof(EntitySSZ3Cutscene), sizeof(ObjectSSZ3Cutscene), SSZ3Cutscene_Update, SSZ3Cutscene_LateUpdate,
                 SSZ3Cutscene_StaticUpdate, SSZ3Cutscene_Draw, SSZ3Cutscene_Create, SSZ3Cutscene_StageLoad, SSZ3Cutscene_EditorDraw,
                 SSZ3Cutscene_EditorLoad, SSZ3Cutscene_Serialize);
    RSDK.CreateObject((Object**)&SSZEggman, "SSZEggman", sizeof(EntitySSZEggman), sizeof(ObjectSSZEggman), SSZEggman_Update, SSZEggman_LateUpdate,
                 SSZEggman_StaticUpdate, SSZEggman_Draw, SSZEggman_Create, SSZEggman_StageLoad, SSZEggman_EditorDraw, SSZEggman_EditorLoad,
                 SSZEggman_Serialize);
    RSDK.CreateObject((Object**)&SSZSpikeBall, "SSZSpikeBall", sizeof(EntitySSZSpikeBall), sizeof(ObjectSSZSpikeBall), SSZSpikeBall_Update, SSZSpikeBall_LateUpdate,
                 SSZSpikeBall_StaticUpdate, SSZSpikeBall_Draw, SSZSpikeBall_Create, SSZSpikeBall_StageLoad, SSZSpikeBall_EditorDraw,
                 SSZSpikeBall_EditorLoad, SSZSpikeBall_Serialize);
    RSDK.CreateObject((Object**)&SSZSpotlight, "SSZSpotlight", sizeof(EntitySSZSpotlight), sizeof(ObjectSSZSpotlight), SSZSpotlight_Update, SSZSpotlight_LateUpdate,
                 SSZSpotlight_StaticUpdate, SSZSpotlight_Draw, SSZSpotlight_Create, SSZSpotlight_StageLoad, SSZSpotlight_EditorDraw,
                 SSZSpotlight_EditorLoad, SSZSpotlight_Serialize);
    RSDK.CreateObject((Object**)&Staircase, "Staircase", sizeof(EntityStaircase), sizeof(ObjectStaircase), Staircase_Update, Staircase_LateUpdate,
                 Staircase_StaticUpdate, Staircase_Draw, Staircase_Create, Staircase_StageLoad, Staircase_EditorDraw, Staircase_EditorLoad,
                 Staircase_Serialize);
    RSDK.CreateObject((Object**)&Stalactite, "Stalactite", sizeof(EntityStalactite), sizeof(ObjectStalactite), Stalactite_Update, Stalactite_LateUpdate,
                 Stalactite_StaticUpdate, Stalactite_Draw, Stalactite_Create, Stalactite_StageLoad, Stalactite_EditorDraw, Stalactite_EditorLoad,
                 Stalactite_Serialize);
    RSDK.CreateObject((Object**)&StarPost, "StarPost", sizeof(EntityStarPost), sizeof(ObjectStarPost), StarPost_Update, StarPost_LateUpdate, StarPost_StaticUpdate,
                 StarPost_Draw, StarPost_Create, StarPost_StageLoad, StarPost_EditorDraw, StarPost_EditorLoad, StarPost_Serialize);
    RSDK.CreateObject((Object**)&StickyPlatform, "StickyPlatform", sizeof(EntityStickyPlatform), sizeof(ObjectStickyPlatform), StickyPlatform_Update,
                 StickyPlatform_LateUpdate, StickyPlatform_StaticUpdate, StickyPlatform_Draw, StickyPlatform_Create, StickyPlatform_StageLoad,
                 StickyPlatform_EditorDraw, StickyPlatform_EditorLoad, StickyPlatform_Serialize);
    RSDK.CreateObject((Object**)&Summary, "Summary", sizeof(EntitySummary), sizeof(ObjectSummary), Summary_Update, Summary_LateUpdate, Summary_StaticUpdate,
                 Summary_Draw, Summary_Create, Summary_StageLoad, Summary_EditorDraw, Summary_EditorLoad, Summary_Serialize);
    RSDK.CreateObject((Object**)&SummaryEmerald, "SummaryEmerald", sizeof(EntitySummaryEmerald), sizeof(ObjectSummaryEmerald), SummaryEmerald_Update,
                 SummaryEmerald_LateUpdate, SummaryEmerald_StaticUpdate, SummaryEmerald_Draw, SummaryEmerald_Create, SummaryEmerald_StageLoad,
                 SummaryEmerald_EditorDraw, SummaryEmerald_EditorLoad, SummaryEmerald_Serialize);
    RSDK.CreateObject((Object**)&SuperSparkle, "SuperSparkle", sizeof(EntitySuperSparkle), sizeof(ObjectSuperSparkle), SuperSparkle_Update, SuperSparkle_LateUpdate,
                 SuperSparkle_StaticUpdate, SuperSparkle_Draw, SuperSparkle_Create, SuperSparkle_StageLoad, SuperSparkle_EditorDraw,
                 SuperSparkle_EditorLoad, SuperSparkle_Serialize);
    RSDK.CreateObject((Object**)&Sweep, "Sweep", sizeof(EntitySweep), sizeof(ObjectSweep), Sweep_Update, Sweep_LateUpdate, Sweep_StaticUpdate, Sweep_Draw,
                 Sweep_Create, Sweep_StageLoad, Sweep_EditorDraw, Sweep_EditorLoad, Sweep_Serialize);
    RSDK.CreateObject((Object**)&SwingRope, "SwingRope", sizeof(EntitySwingRope), sizeof(ObjectSwingRope), SwingRope_Update, SwingRope_LateUpdate,
                 SwingRope_StaticUpdate, SwingRope_Draw, SwingRope_Create, SwingRope_StageLoad, SwingRope_EditorDraw, SwingRope_EditorLoad,
                 SwingRope_Serialize);
    RSDK.CreateObject((Object**)&SwitchDoor, "SwitchDoor", sizeof(EntitySwitchDoor), sizeof(ObjectSwitchDoor), SwitchDoor_Update, SwitchDoor_LateUpdate,
                 SwitchDoor_StaticUpdate, SwitchDoor_Draw, SwitchDoor_Create, SwitchDoor_StageLoad, SwitchDoor_EditorDraw, SwitchDoor_EditorLoad,
                 SwitchDoor_Serialize);
    RSDK.CreateObject((Object**)&Syringe, "Syringe", sizeof(EntitySyringe), sizeof(ObjectSyringe), Syringe_Update, Syringe_LateUpdate, Syringe_StaticUpdate,
                 Syringe_Draw, Syringe_Create, Syringe_StageLoad, Syringe_EditorDraw, Syringe_EditorLoad, Syringe_Serialize);
    RSDK.CreateObject((Object**)&TAEmerald, "TAEmerald", sizeof(EntityTAEmerald), sizeof(ObjectTAEmerald), TAEmerald_Update, TAEmerald_LateUpdate,
                 TAEmerald_StaticUpdate, TAEmerald_Draw, TAEmerald_Create, TAEmerald_StageLoad, TAEmerald_EditorDraw, TAEmerald_EditorLoad,
                 TAEmerald_Serialize);
    RSDK.CreateObject((Object**)&Technosqueek, "Technosqueek", sizeof(EntityTechnosqueek), sizeof(ObjectTechnosqueek), Technosqueek_Update, Technosqueek_LateUpdate,
                 Technosqueek_StaticUpdate, Technosqueek_Draw, Technosqueek_Create, Technosqueek_StageLoad, Technosqueek_EditorDraw,
                 Technosqueek_EditorLoad, Technosqueek_Serialize);
    RSDK.CreateObject((Object**)&TeeterTotter, "TeeterTotter", sizeof(EntityTeeterTotter), sizeof(ObjectTeeterTotter), TeeterTotter_Update, TeeterTotter_LateUpdate,
                 TeeterTotter_StaticUpdate, TeeterTotter_Draw, TeeterTotter_Create, TeeterTotter_StageLoad, TeeterTotter_EditorDraw,
                 TeeterTotter_EditorLoad, TeeterTotter_Serialize);
    RSDK.CreateObject((Object**)&TetherBall, "TetherBall", sizeof(EntityTetherBall), sizeof(ObjectTetherBall), TetherBall_Update, TetherBall_LateUpdate,
                 TetherBall_StaticUpdate, TetherBall_Draw, TetherBall_Create, TetherBall_StageLoad, TetherBall_EditorDraw, TetherBall_EditorLoad,
                 TetherBall_Serialize);
    RSDK.CreateObject((Object**)&ThanksSetup, "ThanksSetup", sizeof(EntityThanksSetup), sizeof(ObjectThanksSetup), ThanksSetup_Update, ThanksSetup_LateUpdate,
                 ThanksSetup_StaticUpdate, ThanksSetup_Draw, ThanksSetup_Create, ThanksSetup_StageLoad, ThanksSetup_EditorDraw, ThanksSetup_EditorLoad,
                 ThanksSetup_Serialize);
    RSDK.CreateObject((Object**)&ThoughtBubble, "ThoughtBubble", sizeof(EntityThoughtBubble), sizeof(ObjectThoughtBubble), ThoughtBubble_Update,
                 ThoughtBubble_LateUpdate, ThoughtBubble_StaticUpdate, ThoughtBubble_Draw, ThoughtBubble_Create, ThoughtBubble_StageLoad,
                 ThoughtBubble_EditorDraw, ThoughtBubble_EditorLoad, ThoughtBubble_Serialize);
    RSDK.CreateObject((Object**)&TilePlatform, "TilePlatform", sizeof(EntityTilePlatform), sizeof(ObjectTilePlatform), TilePlatform_Update, TilePlatform_LateUpdate,
                 TilePlatform_StaticUpdate, TilePlatform_Draw, TilePlatform_Create, TilePlatform_StageLoad, TilePlatform_EditorDraw, TilePlatform_EditorLoad, TilePlatform_Serialize);
    RSDK.CreateObjectContainer((Object **)&TimeAttackData, "TimeAttackData", sizeof(ObjectTimeAttackData));
    RSDK.CreateObject((Object**)&TimeAttackGate, "TimeAttackGate", sizeof(EntityTimeAttackGate), sizeof(ObjectTimeAttackGate), TimeAttackGate_Update,
                 TimeAttackGate_LateUpdate, TimeAttackGate_StaticUpdate, TimeAttackGate_Draw, TimeAttackGate_Create, TimeAttackGate_StageLoad,
                 TimeAttackGate_EditorDraw, TimeAttackGate_EditorLoad, TimeAttackGate_Serialize);
    RSDK.CreateObject((Object**)&TimeAttackMenu, "TimeAttackMenu", sizeof(EntityTimeAttackMenu), sizeof(ObjectTimeAttackMenu), TimeAttackMenu_Update,
                 TimeAttackMenu_LateUpdate, TimeAttackMenu_StaticUpdate, TimeAttackMenu_Draw, TimeAttackMenu_Create, TimeAttackMenu_StageLoad,
                 TimeAttackMenu_EditorDraw, TimeAttackMenu_EditorLoad, TimeAttackMenu_Serialize);
    RSDK.CreateObject((Object**)&TimePost, "TimePost", sizeof(EntityTimePost), sizeof(ObjectTimePost), TimePost_Update, TimePost_LateUpdate, TimePost_StaticUpdate,
                 TimePost_Draw, TimePost_Create, TimePost_StageLoad, TimePost_EditorDraw, TimePost_EditorLoad, TimePost_Serialize);
    RSDK.CreateObject((Object**)&TimeTravelSetup, "TimeTravelSetup", sizeof(EntityTimeTravelSetup), sizeof(ObjectTimeTravelSetup), TimeTravelSetup_Update,
                 TimeTravelSetup_LateUpdate, TimeTravelSetup_StaticUpdate, TimeTravelSetup_Draw, TimeTravelSetup_Create, TimeTravelSetup_StageLoad,
                 TimeTravelSetup_EditorDraw, TimeTravelSetup_EditorLoad, TimeTravelSetup_Serialize);
    RSDK.CreateObject((Object**)&TippingPlatform, "TippingPlatform", sizeof(EntityTippingPlatform), sizeof(ObjectTippingPlatform), TippingPlatform_Update,
                 TippingPlatform_LateUpdate, TippingPlatform_StaticUpdate, TippingPlatform_Draw, TippingPlatform_Create, TippingPlatform_StageLoad,
                 TippingPlatform_EditorDraw, TippingPlatform_EditorLoad, TippingPlatform_Serialize);
    RSDK.CreateObject((Object**)&Title3DSprite, "Title3DSprite", sizeof(EntityTitle3DSprite), sizeof(ObjectTitle3DSprite), Title3DSprite_Update,
                 Title3DSprite_LateUpdate, Title3DSprite_StaticUpdate, Title3DSprite_Draw, Title3DSprite_Create, Title3DSprite_StageLoad,
                 Title3DSprite_EditorDraw, Title3DSprite_EditorLoad, Title3DSprite_Serialize);
    RSDK.CreateObject((Object**)&TitleBG, "TitleBG", sizeof(EntityTitleBG), sizeof(ObjectTitleBG), TitleBG_Update, TitleBG_LateUpdate, TitleBG_StaticUpdate,
                 TitleBG_Draw, TitleBG_Create, TitleBG_StageLoad, TitleBG_EditorDraw, TitleBG_EditorLoad, TitleBG_Serialize);
    RSDK.CreateObject((Object**)&TitleCard, "TitleCard", sizeof(EntityTitleCard), sizeof(ObjectTitleCard), TitleCard_Update, TitleCard_LateUpdate,
                 TitleCard_StaticUpdate, TitleCard_Draw, TitleCard_Create, TitleCard_StageLoad, TitleCard_EditorDraw, TitleCard_EditorLoad,
                 TitleCard_Serialize);
    RSDK.CreateObject((Object**)&TitleEggman, "TitleEggman", sizeof(EntityTitleEggman), sizeof(ObjectTitleEggman), TitleEggman_Update, TitleEggman_LateUpdate,
                 TitleEggman_StaticUpdate, TitleEggman_Draw, TitleEggman_Create, TitleEggman_StageLoad, TitleEggman_EditorDraw, TitleEggman_EditorLoad,
                 TitleEggman_Serialize);
    RSDK.CreateObject((Object**)&TitleLogo, "TitleLogo", sizeof(EntityTitleLogo), sizeof(ObjectTitleLogo), TitleLogo_Update, TitleLogo_LateUpdate,
                 TitleLogo_StaticUpdate, TitleLogo_Draw, TitleLogo_Create, TitleLogo_StageLoad, TitleLogo_EditorDraw, TitleLogo_EditorLoad,
                 TitleLogo_Serialize);
    RSDK.CreateObject((Object**)&TitleSetup, "TitleSetup", sizeof(EntityTitleSetup), sizeof(ObjectTitleSetup), TitleSetup_Update, TitleSetup_LateUpdate,
                 TitleSetup_StaticUpdate, TitleSetup_Draw, TitleSetup_Create, TitleSetup_StageLoad, TitleSetup_EditorDraw, TitleSetup_EditorLoad,
                 TitleSetup_Serialize);
    RSDK.CreateObject((Object**)&TitleSonic, "TitleSonic", sizeof(EntityTitleSonic), sizeof(ObjectTitleSonic), TitleSonic_Update, TitleSonic_LateUpdate,
                 TitleSonic_StaticUpdate, TitleSonic_Draw, TitleSonic_Create, TitleSonic_StageLoad, TitleSonic_EditorDraw, TitleSonic_EditorLoad,
                 TitleSonic_Serialize);
    RSDK.CreateObject((Object**)&TMZ1Outro, "TMZ1Outro", sizeof(EntityTMZ1Outro), sizeof(ObjectTMZ1Outro), TMZ1Outro_Update, TMZ1Outro_LateUpdate,
                 TMZ1Outro_StaticUpdate, TMZ1Outro_Draw, TMZ1Outro_Create, TMZ1Outro_StageLoad, TMZ1Outro_EditorDraw, TMZ1Outro_EditorLoad,
                 TMZ1Outro_Serialize);
    RSDK.CreateObject((Object**)&TMZ1Setup, "TMZ1Setup", sizeof(EntityTMZ1Setup), sizeof(ObjectTMZ1Setup), TMZ1Setup_Update, TMZ1Setup_LateUpdate,
                 TMZ1Setup_StaticUpdate, TMZ1Setup_Draw, TMZ1Setup_Create, TMZ1Setup_StageLoad, TMZ1Setup_EditorDraw, TMZ1Setup_EditorLoad,
                 TMZ1Setup_Serialize);
    RSDK.CreateObject((Object**)&TMZ2Outro, "TMZ2Outro", sizeof(EntityTMZ2Outro), sizeof(ObjectTMZ2Outro), TMZ2Outro_Update, TMZ2Outro_LateUpdate,
                 TMZ2Outro_StaticUpdate, TMZ2Outro_Draw, TMZ2Outro_Create, TMZ2Outro_StageLoad, TMZ2Outro_EditorDraw, TMZ2Outro_EditorLoad,
                 TMZ2Outro_Serialize);
    RSDK.CreateObject((Object**)&TMZ2Setup, "TMZ2Setup", sizeof(EntityTMZ2Setup), sizeof(ObjectTMZ2Setup), TMZ2Setup_Update, TMZ2Setup_LateUpdate,
                 TMZ2Setup_StaticUpdate, TMZ2Setup_Draw, TMZ2Setup_Create, TMZ2Setup_StageLoad, TMZ2Setup_EditorDraw, TMZ2Setup_EditorLoad,
                 TMZ2Setup_Serialize);
    RSDK.CreateObject((Object**)&TMZ3Setup, "TMZ3Setup", sizeof(EntityTMZ3Setup), sizeof(ObjectTMZ3Setup), TMZ3Setup_Update, TMZ3Setup_LateUpdate,
                 TMZ3Setup_StaticUpdate, TMZ3Setup_Draw, TMZ3Setup_Create, TMZ3Setup_StageLoad, TMZ3Setup_EditorDraw, TMZ3Setup_EditorLoad,
                 TMZ3Setup_Serialize);
    RSDK.CreateObject((Object**)&TMZAlert, "TMZAlert", sizeof(EntityTMZAlert), sizeof(ObjectTMZAlert), TMZAlert_Update, TMZAlert_LateUpdate, TMZAlert_StaticUpdate,
                 TMZAlert_Draw, TMZAlert_Create, TMZAlert_StageLoad, TMZAlert_EditorDraw, TMZAlert_EditorLoad, TMZAlert_Serialize);
    RSDK.CreateObject((Object**)&TMZBarrier, "TMZBarrier", sizeof(EntityTMZBarrier), sizeof(ObjectTMZBarrier), TMZBarrier_Update, TMZBarrier_LateUpdate,
                 TMZBarrier_StaticUpdate, TMZBarrier_Draw, TMZBarrier_Create, TMZBarrier_StageLoad, TMZBarrier_EditorDraw, TMZBarrier_EditorLoad,
                 TMZBarrier_Serialize);
    RSDK.CreateObject((Object**)&TMZCable, "TMZCable", sizeof(EntityTMZCable), sizeof(ObjectTMZCable), TMZCable_Update, TMZCable_LateUpdate, TMZCable_StaticUpdate,
                 TMZCable_Draw, TMZCable_Create, TMZCable_StageLoad, TMZCable_EditorDraw, TMZCable_EditorLoad, TMZCable_Serialize);
    RSDK.CreateObject((Object**)&TMZFlames, "TMZFlames", sizeof(EntityTMZFlames), sizeof(ObjectTMZFlames), TMZFlames_Update, TMZFlames_LateUpdate,
                 TMZFlames_StaticUpdate, TMZFlames_Draw, TMZFlames_Create, TMZFlames_StageLoad, TMZFlames_EditorDraw, TMZFlames_EditorLoad,
                 TMZFlames_Serialize);
    RSDK.CreateObject((Object**)&Tornado, "Tornado", sizeof(EntityTornado), sizeof(ObjectTornado), Tornado_Update, Tornado_LateUpdate, Tornado_StaticUpdate,
                 Tornado_Draw, Tornado_Create, Tornado_StageLoad, Tornado_EditorDraw, Tornado_EditorLoad, Tornado_Serialize);
    RSDK.CreateObject((Object**)&TornadoPath, "TornadoPath", sizeof(EntityTornadoPath), sizeof(ObjectTornadoPath), TornadoPath_Update, TornadoPath_LateUpdate,
                 TornadoPath_StaticUpdate, TornadoPath_Draw, TornadoPath_Create, TornadoPath_StageLoad, TornadoPath_EditorDraw, TornadoPath_EditorLoad,
                 TornadoPath_Serialize);
    RSDK.CreateObject((Object**)&Toxomister, "Toxomister", sizeof(EntityToxomister), sizeof(ObjectToxomister), Toxomister_Update, Toxomister_LateUpdate,
                 Toxomister_StaticUpdate, Toxomister_Draw, Toxomister_Create, Toxomister_StageLoad, Toxomister_EditorDraw, Toxomister_EditorLoad,
                 Toxomister_Serialize);
    RSDK.CreateObject((Object**)&TransportTube, "TransportTube", sizeof(EntityTransportTube), sizeof(ObjectTransportTube), TransportTube_Update,
                 TransportTube_LateUpdate, TransportTube_StaticUpdate, TransportTube_Draw, TransportTube_Create, TransportTube_StageLoad,
                 TransportTube_EditorDraw, TransportTube_EditorLoad, TransportTube_Serialize);
    RSDK.CreateObject((Object**)&TryAgain, "TryAgain", sizeof(EntityTryAgain), sizeof(ObjectTryAgain), TryAgain_Update, TryAgain_LateUpdate, TryAgain_StaticUpdate,
                 TryAgain_Draw, TryAgain_Create, TryAgain_StageLoad, TryAgain_EditorDraw, TryAgain_EditorLoad, TryAgain_Serialize);
    RSDK.CreateObject((Object**)&TryAgainE, "TryAgainE", sizeof(EntityTryAgainE), sizeof(ObjectTryAgainE), TryAgainE_Update, TryAgainE_LateUpdate,
                 TryAgainE_StaticUpdate, TryAgainE_Draw, TryAgainE_Create, TryAgainE_StageLoad, TryAgainE_EditorDraw, TryAgainE_EditorLoad,
                 TryAgainE_Serialize);
    RSDK.CreateObject((Object**)&TTCutscene, "TTCutscene", sizeof(EntityTTCutscene), sizeof(ObjectTTCutscene), TTCutscene_Update, TTCutscene_LateUpdate,
                 TTCutscene_StaticUpdate, TTCutscene_Draw, TTCutscene_Create, TTCutscene_StageLoad, TTCutscene_EditorDraw, TTCutscene_EditorLoad,
                 TTCutscene_Serialize);
    RSDK.CreateObject((Object**)&TubeSpring, "TubeSpring", sizeof(EntityTubeSpring), sizeof(ObjectTubeSpring), TubeSpring_Update, TubeSpring_LateUpdate,
                 TubeSpring_StaticUpdate, TubeSpring_Draw, TubeSpring_Create, TubeSpring_StageLoad, TubeSpring_EditorDraw, TubeSpring_EditorLoad,
                 TubeSpring_Serialize);
    RSDK.CreateObject((Object**)&Tubinaut, "Tubinaut", sizeof(EntityTubinaut), sizeof(ObjectTubinaut), Tubinaut_Update, Tubinaut_LateUpdate, Tubinaut_StaticUpdate,
                 Tubinaut_Draw, Tubinaut_Create, Tubinaut_StageLoad, Tubinaut_EditorDraw, Tubinaut_EditorLoad, Tubinaut_Serialize);
    RSDK.CreateObject((Object**)&Tuesday, "Tuesday", sizeof(EntityTuesday), sizeof(ObjectTuesday), Tuesday_Update, Tuesday_LateUpdate, Tuesday_StaticUpdate,
                 Tuesday_Draw, Tuesday_Create, Tuesday_StageLoad, Tuesday_EditorDraw, Tuesday_EditorLoad, Tuesday_Serialize);
    RSDK.CreateObject((Object**)&Turbine, "Turbine", sizeof(EntityTurbine), sizeof(ObjectTurbine), Turbine_Update, Turbine_LateUpdate, Turbine_StaticUpdate,
                 Turbine_Draw, Turbine_Create, Turbine_StageLoad, Turbine_EditorDraw, Turbine_EditorLoad, Turbine_Serialize);
    RSDK.CreateObject((Object**)&TurboSpiker, "TurboSpiker", sizeof(EntityTurboSpiker), sizeof(ObjectTurboSpiker), TurboSpiker_Update, TurboSpiker_LateUpdate,
                 TurboSpiker_StaticUpdate, TurboSpiker_Draw, TurboSpiker_Create, TurboSpiker_StageLoad, TurboSpiker_EditorDraw, TurboSpiker_EditorLoad,
                 TurboSpiker_Serialize);
    RSDK.CreateObject((Object**)&TurboTurtle, "TurboTurtle", sizeof(EntityTurboTurtle), sizeof(ObjectTurboTurtle), TurboTurtle_Update, TurboTurtle_LateUpdate,
                 TurboTurtle_StaticUpdate, TurboTurtle_Draw, TurboTurtle_Create, TurboTurtle_StageLoad, TurboTurtle_EditorDraw, TurboTurtle_EditorLoad,
                 TurboTurtle_Serialize);
    RSDK.CreateObject((Object**)&Turntable, "Turntable", sizeof(EntityTurntable), sizeof(ObjectTurntable), Turntable_Update, Turntable_LateUpdate,
                 Turntable_StaticUpdate, Turntable_Draw, Turntable_Create, Turntable_StageLoad, Turntable_EditorDraw, Turntable_EditorLoad,
                 Turntable_Serialize);
    RSDK.CreateObject((Object**)&TurretSwitch, "TurretSwitch", sizeof(EntityTurretSwitch), sizeof(ObjectTurretSwitch), TurretSwitch_Update, TurretSwitch_LateUpdate,
                 TurretSwitch_StaticUpdate, TurretSwitch_Draw, TurretSwitch_Create, TurretSwitch_StageLoad, TurretSwitch_EditorDraw,
                 TurretSwitch_EditorLoad, TurretSwitch_Serialize);
    RSDK.CreateObject((Object**)&TVFlyingBattery, "TVFlyingBattery", sizeof(EntityTVFlyingBattery), sizeof(ObjectTVFlyingBattery), TVFlyingBattery_Update,
                 TVFlyingBattery_LateUpdate, TVFlyingBattery_StaticUpdate, TVFlyingBattery_Draw, TVFlyingBattery_Create, TVFlyingBattery_StageLoad,
                 TVFlyingBattery_EditorDraw, TVFlyingBattery_EditorLoad, TVFlyingBattery_Serialize);
    RSDK.CreateObject((Object**)&TVPole, "TVPole", sizeof(EntityTVPole), sizeof(ObjectTVPole), TVPole_Update, TVPole_LateUpdate, TVPole_StaticUpdate, TVPole_Draw,
                 TVPole_Create, TVPole_StageLoad, TVPole_EditorDraw, TVPole_EditorLoad, TVPole_Serialize);
    RSDK.CreateObject((Object**)&TVVan, "TVVan", sizeof(EntityTVVan), sizeof(ObjectTVVan), TVVan_Update, TVVan_LateUpdate, TVVan_StaticUpdate, TVVan_Draw,
                 TVVan_Create, TVVan_StageLoad, TVVan_EditorDraw, TVVan_EditorLoad, TVVan_Serialize);
    RSDK.CreateObject((Object**)&TwistedTubes, "TwistedTubes", sizeof(EntityTwistedTubes), sizeof(ObjectTwistedTubes), TwistedTubes_Update, TwistedTubes_LateUpdate,
                 TwistedTubes_StaticUpdate, TwistedTubes_Draw, TwistedTubes_Create, TwistedTubes_StageLoad, TwistedTubes_EditorDraw,
                 TwistedTubes_EditorLoad, TwistedTubes_Serialize);
    RSDK.CreateObject((Object**)&TwistingDoor, "TwistingDoor", sizeof(EntityTwistingDoor), sizeof(ObjectTwistingDoor), TwistingDoor_Update, TwistingDoor_LateUpdate,
                 TwistingDoor_StaticUpdate, TwistingDoor_Draw, TwistingDoor_Create, TwistingDoor_StageLoad, TwistingDoor_EditorDraw,
                 TwistingDoor_EditorLoad, TwistingDoor_Serialize);
    RSDK.CreateObject((Object**)&TwistingSlide, "TwistingSlide", sizeof(EntityTwistingSlide), sizeof(ObjectTwistingSlide), TwistingSlide_Update,
                 TwistingSlide_LateUpdate, TwistingSlide_StaticUpdate, TwistingSlide_Draw, TwistingSlide_Create, TwistingSlide_StageLoad,
                 TwistingSlide_EditorDraw, TwistingSlide_EditorLoad, TwistingSlide_Serialize);
    RSDK.CreateObject((Object**)&UberCaterkiller, "UberCaterkiller", sizeof(EntityUberCaterkiller), sizeof(ObjectUberCaterkiller), UberCaterkiller_Update,
                 UberCaterkiller_LateUpdate, UberCaterkiller_StaticUpdate, UberCaterkiller_Draw, UberCaterkiller_Create, UberCaterkiller_StageLoad,
                 UberCaterkiller_EditorDraw, UberCaterkiller_EditorLoad, UberCaterkiller_Serialize);
    RSDK.CreateObject((Object**)&UFO_Camera, "UFO_Camera", sizeof(EntityUFO_Camera), sizeof(ObjectUFO_Camera), UFO_Camera_Update, UFO_Camera_LateUpdate,
                 UFO_Camera_StaticUpdate, UFO_Camera_Draw, UFO_Camera_Create, UFO_Camera_StageLoad, UFO_Camera_EditorDraw, UFO_Camera_EditorLoad,
                 UFO_Camera_Serialize);
    RSDK.CreateObject((Object**)&UFO_Circuit, "UFO_Circuit", sizeof(EntityUFO_Circuit), sizeof(ObjectUFO_Circuit), UFO_Circuit_Update, UFO_Circuit_LateUpdate,
                 UFO_Circuit_StaticUpdate, UFO_Circuit_Draw, UFO_Circuit_Create, UFO_Circuit_StageLoad, UFO_Circuit_EditorDraw, UFO_Circuit_EditorLoad,
                 UFO_Circuit_Serialize);
    RSDK.CreateObject((Object**)&UFO_Decoration, "UFO_Decoration", sizeof(EntityUFO_Decoration), sizeof(ObjectUFO_Decoration), UFO_Decoration_Update,
                 UFO_Decoration_LateUpdate, UFO_Decoration_StaticUpdate, UFO_Decoration_Draw, UFO_Decoration_Create, UFO_Decoration_StageLoad,
                 UFO_Decoration_EditorDraw, UFO_Decoration_EditorLoad, UFO_Decoration_Serialize);
    RSDK.CreateObject((Object**)&UFO_Dust, "UFO_Dust", sizeof(EntityUFO_Dust), sizeof(ObjectUFO_Dust), UFO_Dust_Update, UFO_Dust_LateUpdate, UFO_Dust_StaticUpdate,
                 UFO_Dust_Draw, UFO_Dust_Create, UFO_Dust_StageLoad, UFO_Dust_EditorDraw, UFO_Dust_EditorLoad, UFO_Dust_Serialize);
    RSDK.CreateObject((Object**)&UFO_HUD, "UFO_HUD", sizeof(EntityUFO_HUD), sizeof(ObjectUFO_HUD), UFO_HUD_Update, UFO_HUD_LateUpdate, UFO_HUD_StaticUpdate,
                 UFO_HUD_Draw, UFO_HUD_Create, UFO_HUD_StageLoad, UFO_HUD_EditorDraw, UFO_HUD_EditorLoad, UFO_HUD_Serialize);
    RSDK.CreateObject((Object**)&UFO_ItemBox, "UFO_ItemBox", sizeof(EntityUFO_ItemBox), sizeof(ObjectUFO_ItemBox), UFO_ItemBox_Update, UFO_ItemBox_LateUpdate,
                 UFO_ItemBox_StaticUpdate, UFO_ItemBox_Draw, UFO_ItemBox_Create, UFO_ItemBox_StageLoad, UFO_ItemBox_EditorDraw, UFO_ItemBox_EditorLoad,
                 UFO_ItemBox_Serialize);
    RSDK.CreateObject((Object**)&UFO_Message, "UFO_Message", sizeof(EntityUFO_Message), sizeof(ObjectUFO_Message), UFO_Message_Update, UFO_Message_LateUpdate,
                 UFO_Message_StaticUpdate, UFO_Message_Draw, UFO_Message_Create, UFO_Message_StageLoad, UFO_Message_EditorDraw, UFO_Message_EditorLoad,
                 UFO_Message_Serialize);
    RSDK.CreateObject((Object**)&UFO_Plasma, "UFO_Plasma", sizeof(EntityUFO_Plasma), sizeof(ObjectUFO_Plasma), UFO_Plasma_Update, UFO_Plasma_LateUpdate,
                 UFO_Plasma_StaticUpdate, UFO_Plasma_Draw, UFO_Plasma_Create, UFO_Plasma_StageLoad, UFO_Plasma_EditorDraw, UFO_Plasma_EditorLoad,
                 UFO_Plasma_Serialize);
    RSDK.CreateObject((Object**)&UFO_Player, "UFO_Player", sizeof(EntityUFO_Player), sizeof(ObjectUFO_Player), UFO_Player_Update, UFO_Player_LateUpdate,
                 UFO_Player_StaticUpdate, UFO_Player_Draw, UFO_Player_Create, UFO_Player_StageLoad, UFO_Player_EditorDraw, UFO_Player_EditorLoad,
                 UFO_Player_Serialize);
    RSDK.CreateObject((Object**)&UFO_Ring, "UFO_Ring", sizeof(EntityUFO_Ring), sizeof(ObjectUFO_Ring), UFO_Ring_Update, UFO_Ring_LateUpdate, UFO_Ring_StaticUpdate,
                 UFO_Ring_Draw, UFO_Ring_Create, UFO_Ring_StageLoad, UFO_Ring_EditorDraw, UFO_Ring_EditorLoad, UFO_Ring_Serialize);
    RSDK.CreateObject((Object**)&UFO_Setup, "UFO_Setup", sizeof(EntityUFO_Setup), sizeof(ObjectUFO_Setup), UFO_Setup_Update, UFO_Setup_LateUpdate,
                 UFO_Setup_StaticUpdate, UFO_Setup_Draw, UFO_Setup_Create, UFO_Setup_StageLoad, UFO_Setup_EditorDraw, UFO_Setup_EditorLoad,
                 UFO_Setup_Serialize);
    RSDK.CreateObject((Object**)&UFO_Shadow, "UFO_Shadow", sizeof(EntityUFO_Shadow), sizeof(ObjectUFO_Shadow), UFO_Shadow_Update, UFO_Shadow_LateUpdate,
                 UFO_Shadow_StaticUpdate, UFO_Shadow_Draw, UFO_Shadow_Create, UFO_Shadow_StageLoad, UFO_Shadow_EditorDraw, UFO_Shadow_EditorLoad,
                 UFO_Shadow_Serialize);
    RSDK.CreateObject((Object**)&UFO_SpeedLines, "UFO_SpeedLines", sizeof(EntityUFO_SpeedLines), sizeof(ObjectUFO_SpeedLines), UFO_SpeedLines_Update,
                 UFO_SpeedLines_LateUpdate, UFO_SpeedLines_StaticUpdate, UFO_SpeedLines_Draw, UFO_SpeedLines_Create, UFO_SpeedLines_StageLoad,
                 UFO_SpeedLines_EditorDraw, UFO_SpeedLines_EditorLoad, UFO_SpeedLines_Serialize);
    RSDK.CreateObject((Object**)&UFO_Sphere, "UFO_Sphere", sizeof(EntityUFO_Sphere), sizeof(ObjectUFO_Sphere), UFO_Sphere_Update, UFO_Sphere_LateUpdate,
                 UFO_Sphere_StaticUpdate, UFO_Sphere_Draw, UFO_Sphere_Create, UFO_Sphere_StageLoad, UFO_Sphere_EditorDraw, UFO_Sphere_EditorLoad,
                 UFO_Sphere_Serialize);
    RSDK.CreateObject((Object**)&UFO_Springboard, "UFO_Springboard", sizeof(EntityUFO_Springboard), sizeof(ObjectUFO_Springboard), UFO_Springboard_Update,
                 UFO_Springboard_LateUpdate, UFO_Springboard_StaticUpdate, UFO_Springboard_Draw, UFO_Springboard_Create, UFO_Springboard_StageLoad,
                 UFO_Springboard_EditorDraw, UFO_Springboard_EditorLoad, UFO_Springboard_Serialize);
    RSDK.CreateObject((Object**)&UFO_Water, "UFO_Water", sizeof(EntityUFO_Water), sizeof(ObjectUFO_Water), UFO_Water_Update, UFO_Water_LateUpdate,
                 UFO_Water_StaticUpdate, UFO_Water_Draw, UFO_Water_Create, UFO_Water_StageLoad, UFO_Water_EditorDraw, UFO_Water_EditorLoad,
                 UFO_Water_Serialize);
    RSDK.CreateObject((Object**)&UIBackground, "UIBackground", sizeof(EntityUIBackground), sizeof(ObjectUIBackground), UIBackground_Update, UIBackground_LateUpdate,
                 UIBackground_StaticUpdate, UIBackground_Draw, UIBackground_Create, UIBackground_StageLoad, UIBackground_EditorDraw,
                 UIBackground_EditorLoad, UIBackground_Serialize);
    RSDK.CreateObject((Object**)&UIButton, "UIButton", sizeof(EntityUIButton), sizeof(ObjectUIButton), UIButton_Update, UIButton_LateUpdate, UIButton_StaticUpdate,
                 UIButton_Draw, UIButton_Create, UIButton_StageLoad, UIButton_EditorDraw, UIButton_EditorLoad, UIButton_Serialize);
    RSDK.CreateObject((Object**)&UIButtonLabel, "UIButtonLabel", sizeof(EntityUIButtonLabel), sizeof(ObjectUIButtonLabel), UIButtonLabel_Update,
                 UIButtonLabel_LateUpdate, UIButtonLabel_StaticUpdate, UIButtonLabel_Draw, UIButtonLabel_Create, UIButtonLabel_StageLoad,
                 UIButtonLabel_EditorDraw, UIButtonLabel_EditorLoad, UIButtonLabel_Serialize);
    RSDK.CreateObject((Object**)&UIButtonPrompt, "UIButtonPrompt", sizeof(EntityUIButtonPrompt), sizeof(ObjectUIButtonPrompt), UIButtonPrompt_Update,
                 UIButtonPrompt_LateUpdate, UIButtonPrompt_StaticUpdate, UIButtonPrompt_Draw, UIButtonPrompt_Create, UIButtonPrompt_StageLoad,
                 UIButtonPrompt_EditorDraw, UIButtonPrompt_EditorLoad, UIButtonPrompt_Serialize);
    RSDK.CreateObject((Object**)&UICarousel, "UICarousel", sizeof(EntityUICarousel), sizeof(ObjectUICarousel), UICarousel_Update, UICarousel_LateUpdate,
                 UICarousel_StaticUpdate, UICarousel_Draw, UICarousel_Create, UICarousel_StageLoad, UICarousel_EditorDraw, UICarousel_EditorLoad,
                 UICarousel_Serialize);
    RSDK.CreateObject((Object**)&UICharButton, "UICharButton", sizeof(EntityUICharButton), sizeof(ObjectUICharButton), UICharButton_Update, UICharButton_LateUpdate,
                 UICharButton_StaticUpdate, UICharButton_Draw, UICharButton_Create, UICharButton_StageLoad, UICharButton_EditorDraw,
                 UICharButton_EditorLoad, UICharButton_Serialize);
    RSDK.CreateObject((Object**)&UIChoice, "UIChoice", sizeof(EntityUIChoice), sizeof(ObjectUIChoice), UIChoice_Update, UIChoice_LateUpdate, UIChoice_StaticUpdate,
                 UIChoice_Draw, UIChoice_Create, UIChoice_StageLoad, UIChoice_EditorDraw, UIChoice_EditorLoad, UIChoice_Serialize);
    RSDK.CreateObject((Object**)&UIControl, "UIControl", sizeof(EntityUIControl), sizeof(ObjectUIControl), UIControl_Update, UIControl_LateUpdate,
                 UIControl_StaticUpdate, UIControl_Draw, UIControl_Create, UIControl_StageLoad, UIControl_EditorDraw, UIControl_EditorLoad,
                 UIControl_Serialize);
    RSDK.CreateObject((Object**)&UICreditsText, "UICreditsText", sizeof(EntityUICreditsText), sizeof(ObjectUICreditsText), UICreditsText_Update,
                 UICreditsText_LateUpdate, UICreditsText_StaticUpdate, UICreditsText_Draw, UICreditsText_Create, UICreditsText_StageLoad,
                 UICreditsText_EditorDraw, UICreditsText_EditorLoad, UICreditsText_Serialize);
    RSDK.CreateObject((Object**)&UIDialog, "UIDialog", sizeof(EntityUIDialog), sizeof(ObjectUIDialog), UIDialog_Update, UIDialog_LateUpdate, UIDialog_StaticUpdate,
                 UIDialog_Draw, UIDialog_Create, UIDialog_StageLoad, UIDialog_EditorDraw, UIDialog_EditorLoad, UIDialog_Serialize);
    RSDK.CreateObject((Object**)&UIDiorama, "UIDiorama", sizeof(EntityUIDiorama), sizeof(ObjectUIDiorama), UIDiorama_Update, UIDiorama_LateUpdate,
                 UIDiorama_StaticUpdate, UIDiorama_Draw, UIDiorama_Create, UIDiorama_StageLoad, UIDiorama_EditorDraw, UIDiorama_EditorLoad,
                 UIDiorama_Serialize);
    RSDK.CreateObject((Object**)&UIHeading, "UIHeading", sizeof(EntityUIHeading), sizeof(ObjectUIHeading), UIHeading_Update, UIHeading_LateUpdate,
                 UIHeading_StaticUpdate, UIHeading_Draw, UIHeading_Create, UIHeading_StageLoad, UIHeading_EditorDraw, UIHeading_EditorLoad,
                 UIHeading_Serialize);
    RSDK.CreateObject((Object**)&UIInfoLabel, "UIInfoLabel", sizeof(EntityUIInfoLabel), sizeof(ObjectUIInfoLabel), UIInfoLabel_Update, UIInfoLabel_LateUpdate,
                 UIInfoLabel_StaticUpdate, UIInfoLabel_Draw, UIInfoLabel_Create, UIInfoLabel_StageLoad, UIInfoLabel_EditorDraw, UIInfoLabel_EditorLoad,
                 UIInfoLabel_Serialize);
    RSDK.CreateObject((Object**)&UIKeyBinder, "UIKeyBinder", sizeof(EntityUIKeyBinder), sizeof(ObjectUIKeyBinder), UIKeyBinder_Update, UIKeyBinder_LateUpdate,
                 UIKeyBinder_StaticUpdate, UIKeyBinder_Draw, UIKeyBinder_Create, UIKeyBinder_StageLoad, UIKeyBinder_EditorDraw, UIKeyBinder_EditorLoad,
                 UIKeyBinder_Serialize);
    RSDK.CreateObject((Object**)&UILeaderboard, "UILeaderboard", sizeof(EntityUILeaderboard), sizeof(ObjectUILeaderboard), UILeaderboard_Update,
                 UILeaderboard_LateUpdate, UILeaderboard_StaticUpdate, UILeaderboard_Draw, UILeaderboard_Create, UILeaderboard_StageLoad,
                 UILeaderboard_EditorDraw, UILeaderboard_EditorLoad, UILeaderboard_Serialize);
    RSDK.CreateObject((Object**)&UIMedallionPanel, "UIMedallionPanel", sizeof(EntityUIMedallionPanel), sizeof(ObjectUIMedallionPanel), UIMedallionPanel_Update,
                 UIMedallionPanel_LateUpdate, UIMedallionPanel_StaticUpdate, UIMedallionPanel_Draw, UIMedallionPanel_Create,
                 UIMedallionPanel_StageLoad, UIMedallionPanel_EditorDraw, UIMedallionPanel_EditorLoad, UIMedallionPanel_Serialize);
    RSDK.CreateObject((Object**)&UIModeButton, "UIModeButton", sizeof(EntityUIModeButton), sizeof(ObjectUIModeButton), UIModeButton_Update, UIModeButton_LateUpdate,
                 UIModeButton_StaticUpdate, UIModeButton_Draw, UIModeButton_Create, UIModeButton_StageLoad, UIModeButton_EditorDraw,
                 UIModeButton_EditorLoad, UIModeButton_Serialize);
    RSDK.CreateObject((Object**)&UIOptionPanel, "UIOptionPanel", sizeof(EntityUIOptionPanel), sizeof(ObjectUIOptionPanel), UIOptionPanel_Update,
                 UIOptionPanel_LateUpdate, UIOptionPanel_StaticUpdate, UIOptionPanel_Draw, UIOptionPanel_Create, UIOptionPanel_StageLoad,
                 UIOptionPanel_EditorDraw, UIOptionPanel_EditorLoad, UIOptionPanel_Serialize);
    RSDK.CreateObject((Object**)&UIPicture, "UIPicture", sizeof(EntityUIPicture), sizeof(ObjectUIPicture), UIPicture_Update, UIPicture_LateUpdate,
                 UIPicture_StaticUpdate, UIPicture_Draw, UIPicture_Create, UIPicture_StageLoad, UIPicture_EditorDraw, UIPicture_EditorLoad,
                 UIPicture_Serialize);
    RSDK.CreateObject((Object**)&UIPopover, "UIPopover", sizeof(EntityUIPopover), sizeof(ObjectUIPopover), UIPopover_Update, UIPopover_LateUpdate,
                 UIPopover_StaticUpdate, UIPopover_Draw, UIPopover_Create, UIPopover_StageLoad, UIPopover_EditorDraw, UIPopover_EditorLoad,
                 UIPopover_Serialize);
    RSDK.CreateObject((Object**)&UIRankButton, "UIRankButton", sizeof(EntityUIRankButton), sizeof(ObjectUIRankButton), UIRankButton_Update, UIRankButton_LateUpdate,
                 UIRankButton_StaticUpdate, UIRankButton_Draw, UIRankButton_Create, UIRankButton_StageLoad, UIRankButton_EditorDraw,
                 UIRankButton_EditorLoad, UIRankButton_Serialize);
    RSDK.CreateObject((Object**)&UIReplayCarousel, "UIReplayCarousel", sizeof(EntityUIReplayCarousel), sizeof(ObjectUIReplayCarousel), UIReplayCarousel_Update,
                 UIReplayCarousel_LateUpdate, UIReplayCarousel_StaticUpdate, UIReplayCarousel_Draw, UIReplayCarousel_Create,
                 UIReplayCarousel_StageLoad, UIReplayCarousel_EditorDraw, UIReplayCarousel_EditorLoad, UIReplayCarousel_Serialize);
    RSDK.CreateObject((Object**)&UIResPicker, "UIResPicker", sizeof(EntityUIResPicker), sizeof(ObjectUIResPicker), UIResPicker_Update, UIResPicker_LateUpdate,
                 UIResPicker_StaticUpdate, UIResPicker_Draw, UIResPicker_Create, UIResPicker_StageLoad, UIResPicker_EditorDraw, UIResPicker_EditorLoad,
                 UIResPicker_Serialize);
    RSDK.CreateObject((Object**)&UISaveSlot, "UISaveSlot", sizeof(EntityUISaveSlot), sizeof(ObjectUISaveSlot), UISaveSlot_Update, UISaveSlot_LateUpdate,
                 UISaveSlot_StaticUpdate, UISaveSlot_Draw, UISaveSlot_Create, UISaveSlot_StageLoad, UISaveSlot_EditorDraw, UISaveSlot_EditorLoad,
                 UISaveSlot_Serialize);
    RSDK.CreateObject((Object**)&UIShifter, "UIShifter", sizeof(EntityUIShifter), sizeof(ObjectUIShifter), UIShifter_Update, UIShifter_LateUpdate,
                 UIShifter_StaticUpdate, UIShifter_Draw, UIShifter_Create, UIShifter_StageLoad, UIShifter_EditorDraw, UIShifter_EditorLoad,
                 UIShifter_Serialize);
    RSDK.CreateObject((Object**)&UISlider, "UISlider", sizeof(EntityUISlider), sizeof(ObjectUISlider), UISlider_Update, UISlider_LateUpdate, UISlider_StaticUpdate,
                 UISlider_Draw, UISlider_Create, UISlider_StageLoad, UISlider_EditorDraw, UISlider_EditorLoad, UISlider_Serialize);
    RSDK.CreateObject((Object**)&UISubHeading, "UISubHeading", sizeof(EntityUISubHeading), sizeof(ObjectUISubHeading), UISubHeading_Update, UISubHeading_LateUpdate,
                 UISubHeading_StaticUpdate, UISubHeading_Draw, UISubHeading_Create, UISubHeading_StageLoad, UISubHeading_EditorDraw,
                 UISubHeading_EditorLoad, UISubHeading_Serialize);
    RSDK.CreateObject((Object**)&UITABanner, "UITABanner", sizeof(EntityUITABanner), sizeof(ObjectUITABanner), UITABanner_Update, UITABanner_LateUpdate,
                 UITABanner_StaticUpdate, UITABanner_Draw, UITABanner_Create, UITABanner_StageLoad, UITABanner_EditorDraw, UITABanner_EditorLoad,
                 UITABanner_Serialize);
    RSDK.CreateObject((Object**)&UITAZoneModule, "UITAZoneModule", sizeof(EntityUITAZoneModule), sizeof(ObjectUITAZoneModule), UITAZoneModule_Update,
                 UITAZoneModule_LateUpdate, UITAZoneModule_StaticUpdate, UITAZoneModule_Draw, UITAZoneModule_Create, UITAZoneModule_StageLoad,
                 UITAZoneModule_EditorDraw, UITAZoneModule_EditorLoad, UITAZoneModule_Serialize);
    RSDK.CreateObject((Object**)&UIText, "UIText", sizeof(EntityUIText), sizeof(ObjectUIText), UIText_Update, UIText_LateUpdate, UIText_StaticUpdate, UIText_Draw,
                 UIText_Create, UIText_StageLoad, UIText_EditorDraw, UIText_EditorLoad, UIText_Serialize);
    RSDK.CreateObject((Object**)&UITransition, "UITransition", sizeof(EntityUITransition), sizeof(ObjectUITransition), UITransition_Update, UITransition_LateUpdate,
                 UITransition_StaticUpdate, UITransition_Draw, UITransition_Create, UITransition_StageLoad, UITransition_EditorDraw,
                 UITransition_EditorLoad, UITransition_Serialize);
    RSDK.CreateObject((Object**)&UIUsernamePopup, "UIUsernamePopup", sizeof(EntityUIUsernamePopup), sizeof(ObjectUIUsernamePopup), UIUsernamePopup_Update,
                 UIUsernamePopup_LateUpdate, UIUsernamePopup_StaticUpdate, UIUsernamePopup_Draw, UIUsernamePopup_Create, UIUsernamePopup_StageLoad,
                 UIUsernamePopup_EditorDraw, UIUsernamePopup_EditorLoad, UIUsernamePopup_Serialize);
    RSDK.CreateObject((Object**)&UIVideo, "UIVideo", sizeof(EntityUIVideo), sizeof(ObjectUIVideo), UIVideo_Update, UIVideo_LateUpdate, UIVideo_StaticUpdate,
                 UIVideo_Draw, UIVideo_Create, UIVideo_StageLoad, UIVideo_EditorDraw, UIVideo_EditorLoad, UIVideo_Serialize);
    RSDK.CreateObject((Object**)&UIVsCharSelector, "UIVsCharSelector", sizeof(EntityUIVsCharSelector), sizeof(ObjectUIVsCharSelector), UIVsCharSelector_Update,
                 UIVsCharSelector_LateUpdate, UIVsCharSelector_StaticUpdate, UIVsCharSelector_Draw, UIVsCharSelector_Create,
                 UIVsCharSelector_StageLoad, UIVsCharSelector_EditorDraw, UIVsCharSelector_EditorLoad, UIVsCharSelector_Serialize);
    RSDK.CreateObject((Object**)&UIVsResults, "UIVsResults", sizeof(EntityUIVsResults), sizeof(ObjectUIVsResults), UIVsResults_Update, UIVsResults_LateUpdate,
                 UIVsResults_StaticUpdate, UIVsResults_Draw, UIVsResults_Create, UIVsResults_StageLoad, UIVsResults_EditorDraw, UIVsResults_EditorLoad,
                 UIVsResults_Serialize);
    RSDK.CreateObject((Object**)&UIVsRoundPicker, "UIVsRoundPicker", sizeof(EntityUIVsRoundPicker), sizeof(ObjectUIVsRoundPicker), UIVsRoundPicker_Update,
                 UIVsRoundPicker_LateUpdate, UIVsRoundPicker_StaticUpdate, UIVsRoundPicker_Draw, UIVsRoundPicker_Create, UIVsRoundPicker_StageLoad,
                 UIVsRoundPicker_EditorDraw, UIVsRoundPicker_EditorLoad, UIVsRoundPicker_Serialize);
    RSDK.CreateObject((Object**)&UIVsScoreboard, "UIVsScoreboard", sizeof(EntityUIVsScoreboard), sizeof(ObjectUIVsScoreboard), UIVsScoreboard_Update,
                 UIVsScoreboard_LateUpdate, UIVsScoreboard_StaticUpdate, UIVsScoreboard_Draw, UIVsScoreboard_Create, UIVsScoreboard_StageLoad,
                 UIVsScoreboard_EditorDraw, UIVsScoreboard_EditorLoad, UIVsScoreboard_Serialize);
    RSDK.CreateObject((Object**)&UIVsZoneButton, "UIVsZoneButton", sizeof(EntityUIVsZoneButton), sizeof(ObjectUIVsZoneButton), UIVsZoneButton_Update,
                 UIVsZoneButton_LateUpdate, UIVsZoneButton_StaticUpdate, UIVsZoneButton_Draw, UIVsZoneButton_Create, UIVsZoneButton_StageLoad,
                 UIVsZoneButton_EditorDraw, UIVsZoneButton_EditorLoad, UIVsZoneButton_Serialize);
    RSDK.CreateObject((Object**)&UIWaitSpinner, "UIWaitSpinner", sizeof(EntityUIWaitSpinner), sizeof(ObjectUIWaitSpinner), UIWaitSpinner_Update,
                 UIWaitSpinner_LateUpdate, UIWaitSpinner_StaticUpdate, UIWaitSpinner_Draw, UIWaitSpinner_Create, UIWaitSpinner_StageLoad,
                 UIWaitSpinner_EditorDraw, UIWaitSpinner_EditorLoad, UIWaitSpinner_Serialize);
    RSDK.CreateObject((Object**)&UIWidgets, "UIWidgets", sizeof(EntityUIWidgets), sizeof(ObjectUIWidgets), UIWidgets_Update, UIWidgets_LateUpdate,
                 UIWidgets_StaticUpdate, UIWidgets_Draw, UIWidgets_Create, UIWidgets_StageLoad, UIWidgets_EditorDraw, UIWidgets_EditorLoad,
                 UIWidgets_Serialize);
    RSDK.CreateObject((Object**)&UIWinSize, "UIWinSize", sizeof(EntityUIWinSize), sizeof(ObjectUIWinSize), UIWinSize_Update, UIWinSize_LateUpdate,
                 UIWinSize_StaticUpdate, UIWinSize_Draw, UIWinSize_Create, UIWinSize_StageLoad, UIWinSize_EditorDraw, UIWinSize_EditorLoad,
                 UIWinSize_Serialize);
    RSDK.CreateObject((Object**)&UncurlPlant, "UncurlPlant", sizeof(EntityUncurlPlant), sizeof(ObjectUncurlPlant), UncurlPlant_Update, UncurlPlant_LateUpdate,
                 UncurlPlant_StaticUpdate, UncurlPlant_Draw, UncurlPlant_Create, UncurlPlant_StageLoad, UncurlPlant_EditorDraw, UncurlPlant_EditorLoad,
                 UncurlPlant_Serialize);
    RSDK.CreateObject((Object**)&Valve, "Valve", sizeof(EntityValve), sizeof(ObjectValve), Valve_Update, Valve_LateUpdate, Valve_StaticUpdate, Valve_Draw,
                 Valve_Create, Valve_StageLoad, Valve_EditorDraw, Valve_EditorLoad, Valve_Serialize);
    RSDK.CreateObject((Object**)&VanishPlatform, "VanishPlatform", sizeof(EntityVanishPlatform), sizeof(ObjectVanishPlatform), VanishPlatform_Update,
                 VanishPlatform_LateUpdate, VanishPlatform_StaticUpdate, VanishPlatform_Draw, VanishPlatform_Create, VanishPlatform_StageLoad,
                 VanishPlatform_EditorDraw, VanishPlatform_EditorLoad, VanishPlatform_Serialize);
    RSDK.CreateObject((Object**)&Vultron, "Vultron", sizeof(EntityVultron), sizeof(ObjectVultron), Vultron_Update, Vultron_LateUpdate, Vultron_StaticUpdate,
                 Vultron_Draw, Vultron_Create, Vultron_StageLoad, Vultron_EditorDraw, Vultron_EditorLoad, Vultron_Serialize);
    RSDK.CreateObject((Object**)&WalkerLegs, "WalkerLegs", sizeof(EntityWalkerLegs), sizeof(ObjectWalkerLegs), WalkerLegs_Update, WalkerLegs_LateUpdate,
                 WalkerLegs_StaticUpdate, WalkerLegs_Draw, WalkerLegs_Create, WalkerLegs_StageLoad, WalkerLegs_EditorDraw, WalkerLegs_EditorLoad,
                 WalkerLegs_Serialize);
    RSDK.CreateObject((Object**)&WallBumper, "WallBumper", sizeof(EntityWallBumper), sizeof(ObjectWallBumper), WallBumper_Update, WallBumper_LateUpdate,
                 WallBumper_StaticUpdate, WallBumper_Draw, WallBumper_Create, WallBumper_StageLoad, WallBumper_EditorDraw, WallBumper_EditorLoad,
                 WallBumper_Serialize);
    RSDK.CreateObject((Object**)&WarpDoor, "WarpDoor", sizeof(EntityWarpDoor), sizeof(ObjectWarpDoor), WarpDoor_Update, WarpDoor_LateUpdate, WarpDoor_StaticUpdate,
                 WarpDoor_Draw, WarpDoor_Create, WarpDoor_StageLoad, WarpDoor_EditorDraw, WarpDoor_EditorLoad, WarpDoor_Serialize);
    RSDK.CreateObject((Object**)&Water, "Water", sizeof(EntityWater), sizeof(ObjectWater), Water_Update, Water_LateUpdate, Water_StaticUpdate, Water_Draw,
                 Water_Create, Water_StageLoad, Water_EditorDraw, Water_EditorLoad, Water_Serialize);
    RSDK.CreateObject((Object**)&WaterfallSound, "WaterfallSound", sizeof(EntityWaterfallSound), sizeof(ObjectWaterfallSound), WaterfallSound_Update,
                 WaterfallSound_LateUpdate, WaterfallSound_StaticUpdate, WaterfallSound_Draw, WaterfallSound_Create, WaterfallSound_StageLoad,
                 WaterfallSound_EditorDraw, WaterfallSound_EditorLoad, WaterfallSound_Serialize);
    RSDK.CreateObject((Object**)&WaterGush, "WaterGush", sizeof(EntityWaterGush), sizeof(ObjectWaterGush), WaterGush_Update, WaterGush_LateUpdate,
                 WaterGush_StaticUpdate, WaterGush_Draw, WaterGush_Create, WaterGush_StageLoad, WaterGush_EditorDraw, WaterGush_EditorLoad,
                 WaterGush_Serialize);
    RSDK.CreateObject((Object**)&WeatherMobile, "WeatherMobile", sizeof(EntityWeatherMobile), sizeof(ObjectWeatherMobile), WeatherMobile_Update,
                 WeatherMobile_LateUpdate, WeatherMobile_StaticUpdate, WeatherMobile_Draw, WeatherMobile_Create, WeatherMobile_StageLoad,
                 WeatherMobile_EditorDraw, WeatherMobile_EditorLoad, WeatherMobile_Serialize);
    RSDK.CreateObject((Object**)&WeatherTV, "WeatherTV", sizeof(EntityWeatherTV), sizeof(ObjectWeatherTV), WeatherTV_Update, WeatherTV_LateUpdate,
                 WeatherTV_StaticUpdate, WeatherTV_Draw, WeatherTV_Create, WeatherTV_StageLoad, WeatherTV_EditorDraw, WeatherTV_EditorLoad,
                 WeatherTV_Serialize);
    RSDK.CreateObject((Object**)&Whirlpool, "Whirlpool", sizeof(EntityWhirlpool), sizeof(ObjectWhirlpool), Whirlpool_Update, Whirlpool_LateUpdate,
                 Whirlpool_StaticUpdate, Whirlpool_Draw, Whirlpool_Create, Whirlpool_StageLoad, Whirlpool_EditorDraw, Whirlpool_EditorLoad,
                 Whirlpool_Serialize);
    RSDK.CreateObject((Object**)&WoodChipper, "WoodChipper", sizeof(EntityWoodChipper), sizeof(ObjectWoodChipper), WoodChipper_Update, WoodChipper_LateUpdate,
                 WoodChipper_StaticUpdate, WoodChipper_Draw, WoodChipper_Create, WoodChipper_StageLoad, WoodChipper_EditorDraw, WoodChipper_EditorLoad,
                 WoodChipper_Serialize);
    RSDK.CreateObject((Object**)&Woodrow, "Woodrow", sizeof(EntityWoodrow), sizeof(ObjectWoodrow), Woodrow_Update, Woodrow_LateUpdate, Woodrow_StaticUpdate,
                 Woodrow_Draw, Woodrow_Create, Woodrow_StageLoad, Woodrow_EditorDraw, Woodrow_EditorLoad, Woodrow_Serialize);
    RSDK.CreateObject((Object**)&YoyoPulley, "YoyoPulley", sizeof(EntityYoyoPulley), sizeof(ObjectYoyoPulley), YoyoPulley_Update, YoyoPulley_LateUpdate,
                 YoyoPulley_StaticUpdate, YoyoPulley_Draw, YoyoPulley_Create, YoyoPulley_StageLoad, YoyoPulley_EditorDraw, YoyoPulley_EditorLoad,
                 YoyoPulley_Serialize);
    RSDK.CreateObject((Object**)&ZipLine, "ZipLine", sizeof(EntityZipLine), sizeof(ObjectZipLine), ZipLine_Update, ZipLine_LateUpdate, ZipLine_StaticUpdate,
                 ZipLine_Draw, ZipLine_Create, ZipLine_StageLoad, ZipLine_EditorDraw, ZipLine_EditorLoad, ZipLine_Serialize);
    RSDK.CreateObject((Object**)&Zone, "Zone", sizeof(EntityZone), sizeof(ObjectZone), Zone_Update, Zone_LateUpdate, Zone_StaticUpdate, Zone_Draw, Zone_Create,
                 Zone_StageLoad, Zone_EditorDraw, Zone_EditorLoad, Zone_Serialize);
}
