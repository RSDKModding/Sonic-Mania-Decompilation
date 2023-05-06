// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: AIZSetup Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectAIZSetup *AIZSetup;

void AIZSetup_Update(void) {}

void AIZSetup_LateUpdate(void) {}

void AIZSetup_StaticUpdate(void)
{
#if MANIA_USE_PLUS
    if (Zone->actID != 3 && !AIZSetup->hasSetupCutscene) {
#else
    if (!AIZSetup->hasSetupCutscene) {
#endif
        AIZSetup_SetupObjects();
        AIZSetup_GetCutsceneSetupPtr();
        AIZSetup->hasSetupCutscene = true;
    }

#if MANIA_USE_PLUS
    if (AIZSetup->background4) {
#endif
        if (ScreenInfo->position.x <= 4096)
            AIZSetup->background2->scrollPos = 0;
        else
            AIZSetup->background2->scrollPos = 0x1000000;

        if (ScreenInfo->position.x <= 8704) {
            AIZSetup->background2->drawGroup[0] = 0;
            AIZSetup->background3->drawGroup[0] = DRAWGROUP_COUNT;
        }
        else {
            AIZSetup->background2->drawGroup[0] = DRAWGROUP_COUNT;
            AIZSetup->background3->drawGroup[0] = 0;
        }
#if MANIA_USE_PLUS
    }
    else {
        if (!(Zone->timer & 3)) {
            ++RSDK.GetTileLayer(0)->deformationOffsetW;
        }

        if (!(Zone->timer & 1)) {
            for (int32 i = Zone->fgLayer[0]; i <= Zone->fgLayer[1]; ++i) {
                RSDK.GetTileLayer(i)->deformationOffsetW++;
            }
        }
    }
#endif

    if (!(Zone->timer % 5)) {
        RSDK.RotatePalette(0, 171, 174, true);
#if MANIA_USE_PLUS
        RSDK.RotatePalette(1, 171, 174, true);
        CutsceneHBH->paletteColors[43] = RSDK.GetPaletteEntry(0, 171);
        CutsceneHBH->paletteColors[44] = RSDK.GetPaletteEntry(0, 172);
        CutsceneHBH->paletteColors[45] = RSDK.GetPaletteEntry(0, 173);
        CutsceneHBH->paletteColors[46] = RSDK.GetPaletteEntry(0, 174);
#endif
    }

    if (!AIZSetup->playDrillSfx || RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID == PauseMenu->classID) {
        if (AIZSetup->playingDrillSfx) {
            RSDK.StopSfx(AIZSetup->sfxDrill);
            AIZSetup->playingDrillSfx = false;
        }
    }
    else if (!AIZSetup->playingDrillSfx) {
        RSDK.PlaySfx(AIZSetup->sfxDrill, 1, 255);
        AIZSetup->playingDrillSfx = true;
    }

#if MANIA_USE_PLUS
    --AIZSetup->bellPlantAniTimer;
    if (AIZSetup->bellPlantAniTimer < 0) {
        ++AIZSetup->bellPlantAniFrame;
        if (AIZSetup->bellPlantAniFramePos[AIZSetup->bellPlantAniFrame] == -1)
            AIZSetup->bellPlantAniFrame = 0;

        AIZSetup->bellPlantAniTimer = AIZSetup->bellPlantAniDurations[AIZSetup->bellPlantAniFrame];
        RSDK.DrawAniTiles(AIZSetup->aniTiles, 196, 16 * AIZSetup->bellPlantAniFramePos[AIZSetup->bellPlantAniFrame], 0, 16, 32);
    }

    --AIZSetup->curlPlant1AniTimer;
    if (AIZSetup->curlPlant1AniTimer < 0) {
        ++AIZSetup->curlPlant1AniFrame;
        if (AIZSetup->curlPlant1AniFramePos[AIZSetup->curlPlant1AniFrame] == -1)
            AIZSetup->curlPlant1AniFrame = 0;

        AIZSetup->curlPlant1AniTimer = AIZSetup->curlPlant1AniDurations[AIZSetup->curlPlant1AniFrame];
        RSDK.DrawAniTiles(AIZSetup->aniTiles, 198, 16 * AIZSetup->curlPlant1AniFramePos[AIZSetup->curlPlant1AniFrame], 32, 16, 48);
    }

    --AIZSetup->curlPlant2AniTimer;
    if (AIZSetup->curlPlant2AniTimer < 0) {
        ++AIZSetup->curlPlant2AniFrame;
        if (AIZSetup->curlPlant2AniFramePos[AIZSetup->curlPlant2AniFrame] == -1)
            AIZSetup->curlPlant2AniFrame = 0;

        AIZSetup->curlPlant2AniTimer = AIZSetup->curlPlant2AniDurations[AIZSetup->curlPlant2AniFrame];
        RSDK.DrawAniTiles(AIZSetup->aniTiles, 201, 16 * AIZSetup->curlPlant2AniFramePos[AIZSetup->curlPlant2AniFrame], 32, 16, 48);
    }
#endif
}

void AIZSetup_Draw(void) {}

void AIZSetup_Create(void *data) {}

void AIZSetup_StageLoad(void)
{
#if MANIA_USE_PLUS
    if (Zone->actID != 3)
#endif
        Zone->cameraBoundsB[0] = SCREEN_YSIZE;

    AIZSetup->hasSetupCutscene = false;

    AIZSetup->aniTiles   = RSDK.LoadSpriteSheet("AIZ/AniTiles.gif", SCOPE_STAGE);
    AIZSetup->knuxFrames = RSDK.LoadSpriteAnimation("Players/KnuxCutsceneAIZ.bin", SCOPE_STAGE);

#if MANIA_USE_PLUS
    if (RSDK.GetTileLayerID("Background 4") >= DRAWGROUP_COUNT) {
        // Bug Details:
        // AIZ->background4 doesn't get cleared here, so coming from AIZ Intro to AIZ Encore (same folder so object structs aren't reset)
        // leaves background4's tileLayer pointer intact, though pointing to the wrong layers
        // it also enables the behaviour in StaticUpdate that should only play in AIZ Intro
        // Fix:
        // AIZSetup->background4 = NULL;
        // (though you should prolly clear the other 3 as well)

        for (int32 i = Zone->fgLayer[0]; i <= Zone->fgLayer[1]; ++i) {
            int32 *deformData = RSDK.GetTileLayer(i)->deformationDataW;

            int32 angle = 0;
            for (int32 d = 0; d < 0x200; ++d) {
                deformData[d] = (2 * RSDK.Sin1024(angle)) >> 10;
                angle += 16;
            }

            memcpy(&deformData[0x200], &deformData[0], (0x200 * sizeof(int32)));
        }

        int32 *deformData = RSDK.GetTileLayer(0)->deformationDataW;
        for (int32 d = 0; d < 0x200; d += 16) {
            int32 id = MAX(d, 0);

            int32 angle  = 0;
            int32 deform = RSDK.Rand(0, 4);
            for (int32 i = 0; i < 16; ++i) {
                deformData[id + i] = (deform * RSDK.Sin1024(angle)) >> 10;
                angle += 64;
            }
        }
        memcpy(&deformData[0x200], &deformData[0], (0x200 * sizeof(int32)));
    }
    else {
#endif
        AIZSetup->background1 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 1"));
        AIZSetup->background2 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 2"));
        AIZSetup->background3 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 3"));
        AIZSetup->background4 = RSDK.GetTileLayer(RSDK.GetTileLayerID("Background 4"));

        for (int32 i = 0; i < AIZSetup->background2->scrollInfoCount; ++i) {
            int32 parallaxFactor                           = AIZSetup->background2->scrollInfo[i].parallaxFactor;
            AIZSetup->background2->scrollInfo[i].scrollPos = -TO_FIXED(0x700) - (TO_FIXED(34) * parallaxFactor);
        }

        for (int32 i = 0; i < AIZSetup->background3->scrollInfoCount; ++i) {
            int32 parallaxFactor                           = AIZSetup->background3->scrollInfo[i].parallaxFactor;
            AIZSetup->background3->scrollInfo[i].scrollPos = -TO_FIXED(0x700) - (TO_FIXED(34) * parallaxFactor);
        }
#if MANIA_USE_PLUS
    }
#endif

    Animals->animalTypes[0] = ANIMAL_FLICKY;
    Animals->animalTypes[1] = ANIMAL_RICKY;

    Music_SetMusicTrack("RubyPresence.ogg", TRACK_RUBYPRESENCE, 198457);

    AIZSetup->sfxDrill     = RSDK.GetSfx("LRZ/Drill.wav");
    AIZSetup->sfxBreak     = RSDK.GetSfx("Stage/LedgeBreak3.wav");
    AIZSetup->sfxImpact    = RSDK.GetSfx("Stage/Impact4.wav");
    AIZSetup->sfxHeliWoosh = RSDK.GetSfx("SPZ1/HeliWooshIn.wav");

    if (CHECK_CHARACTER_ID(ID_KNUCKLES, 1)) {
        foreach_all(AIZTornado, tornado) { destroyEntity(tornado); }
        foreach_all(AIZTornadoPath, node) { destroyEntity(node); }
    }

#if MANIA_USE_PLUS
    BGSwitch->switchCallback[AIZ_BG_JUNGLE] = AIZSetup_BGSwitch_Jungle;
    BGSwitch->switchCallback[AIZ_BG_SKY]    = AIZSetup_BGSwitch_Sky;

    BGSwitch->layerIDs[0] = AIZ_BG_JUNGLE;
    BGSwitch->layerIDs[1] = AIZ_BG_JUNGLE;
    BGSwitch->layerIDs[2] = AIZ_BG_JUNGLE;
    BGSwitch->layerIDs[3] = AIZ_BG_JUNGLE;

    RSDK.SetDrawGroupProperties(0, false, Water_DrawHook_ApplyWaterPalette);
    RSDK.SetDrawGroupProperties(Zone->hudDrawGroup, false, Water_DrawHook_RemoveWaterPalette);
    Water->waterPalette = 1;
#endif
}

#if MANIA_USE_PLUS
void AIZSetup_BGSwitch_Jungle(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = 0;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
}

void AIZSetup_BGSwitch_Sky(void)
{
    RSDK.GetTileLayer(0)->drawGroup[BGSwitch->screenID] = DRAWGROUP_COUNT;
    RSDK.GetTileLayer(1)->drawGroup[BGSwitch->screenID] = 0;
}
#endif

void AIZSetup_PlayerState_Static(void)
{
    RSDK_THIS(Player);
    self->active     = ACTIVE_NORMAL;
    self->visible    = false;
    self->position.x = 0;
    self->position.y = 0;
    self->stateInput = StateMachine_None;
}

void AIZSetup_PlayerState_P2Enter(void)
{
    RSDK_THIS(Player);
    self->active         = ACTIVE_NORMAL;
    self->visible        = true;
    self->stateInput     = Player_Input_P2_AI;
    Player->respawnTimer = 240;
    Player_HandleSidekickRespawn();
}

void AIZSetup_HandleHeavyMovement(void)
{
    EntityAIZKingClaw *claw = AIZSetup->claw;

    int32 x = -0x10000;
    foreach_all(AIZEggRobo, robo)
    {
        if (robo->forKnux && x >= robo->position.x)
            x = robo->position.x;
    }

    if (x < (ScreenInfo->position.x + ScreenInfo->size.x + 32) << 16) {
        foreach_all(AIZEggRobo, eggRobo)
        {
            if (eggRobo->forKnux)
                eggRobo->position.x += 0x40000;
        }
        claw->position.x += 0x40000;
        claw->position.y -= 0x4000;
    }
}

void AIZSetup_SetupObjects(void)
{
    foreach_all(AIZTornado, tornado)
    {
        AIZSetup->tornado = tornado;
        foreach_break;
    }

    foreach_all(Platform, platform)
    {
        if (!platform->frameID) {
            platform->drawGroup = Zone->objectDrawGroup[1] - 1;
            AIZSetup->platform  = platform;
            foreach_break;
        }
    }

    foreach_all(AIZKingClaw, claw)
    {
        AIZSetup->claw = claw;
        foreach_break;
    }

    foreach_all(PhantomRuby, ruby)
    {
        AIZSetup->phantomRuby = ruby;
        foreach_break;
    }

    int32 id = 0;
    foreach_all(Decoration, decoration) { AIZSetup->decorations[id++] = decoration; }
}

void AIZSetup_GetCutsceneSetupPtr(void)
{
    switch (GET_CHARACTER_ID(1)) {
        default:
        case ID_SONIC:
        case ID_TAILS:
#if MANIA_USE_PLUS
        case ID_MIGHTY:
        case ID_RAY:
#endif
            AIZSetup_CutsceneST_Setup();
            break;

        case ID_KNUCKLES: AIZSetup_CutsceneK_Setup(); break;
    }
}

#if MANIA_USE_PLUS
void AIZSetup_Cutscene_SkipCB(void)
{
#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "Green Hill Zone+ 1");
    else
#endif
        RSDK.SetScene("Mania Mode", "Green Hill Zone 1");
}
#endif

void AIZSetup_CutsceneST_Setup(void)
{
    RSDK_THIS(AIZSetup);
    CutsceneSeq_StartSequence(self, AIZSetup_CutsceneSonic_EnterAIZ, AIZSetup_CutsceneSonic_EnterAIZJungle, AIZSetup_CutsceneSonic_EnterHeavies,
                              AIZSetup_CutsceneSonic_P2FlyIn, AIZSetup_CutsceneSonic_EnterClaw, AIZSetup_CutsceneSonic_WatchClaw,
                              AIZSetup_CutsceneSonic_RubyGrabbed, AIZSetup_CutsceneSonic_RubyAppear, AIZSetup_CutsceneSonic_RubyFX,
                              AIZSetup_Cutscene_LoadGHZ, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_CALLBACK, AIZSetup_Cutscene_SkipCB);
#endif
}

bool32 AIZSetup_CutsceneSonic_EnterAIZ(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    EntityAIZTornado *tornado = AIZSetup->tornado;

    if (!host->timer) {
        CutsceneSeq_LockAllPlayerControl();
        player1->stateInput = StateMachine_None;
    }
    Zone->playerBoundActiveL[0] = false;

    if (player2->classID == Player->classID)
        player2->state = AIZSetup_PlayerState_Static;

    if (tornado->position.x < ScreenInfo->size.x << 16)
        camera->position.x = ScreenInfo->size.x << 16;

    return tornado->disableInteractions;
}
bool32 AIZSetup_CutsceneSonic_EnterAIZJungle(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    if (player1->position.x >= 0x27100000) {
        Music_TransitionTrack(TRACK_HBHMISCHIEF, 0.01);
        player1->right    = false;
        player1->left     = true;
        host->storedValue = player1->groundVel;
        return true;
    }
    else {
        player1->right = true;
    }
    return false;
}
bool32 AIZSetup_CutsceneSonic_EnterHeavies(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (player1->position.x >= 0x2A300000) {
        if (player1->groundVel <= 0) {
            if (!host->storedTimer) {
                player1->right    = false;
                player1->left     = false;
                host->storedTimer = host->timer;
            }
        }
        else {
            if (player2->classID == Player->classID) {
                player2->stateInput = StateMachine_None;
            }
            Player->targetLeaderPosition.x = player1->position.x - 0x200000;
            Player->targetLeaderPosition.y = player1->position.y;
            player1->state                 = Player_State_Ground;
            player1->right                 = false;
            player1->left                  = true;
        }
    }
    else {
        player1->state = Player_State_Static;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_SKID, &player1->animator, false, 0);
        player1->groundVel  = host->storedValue;
        player1->velocity.x = host->storedValue;
        player1->right      = false;
        player1->left       = false;
        if (player2 && player2->state == AIZSetup_PlayerState_Static)
            player2->state = AIZSetup_PlayerState_P2Enter;
    }

    if (host->storedTimer > 0 && host->timer > host->storedTimer + 15) {
        player1->direction = FLIP_NONE;
        return true;
    }

    return false;
}
bool32 AIZSetup_CutsceneSonic_P2FlyIn(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (player2->classID == Player->classID) {
        if (player2->onGround) {
            player2->stateInput = StateMachine_None;
            player2->direction  = FLIP_NONE;
            RSDK.SetSpriteAnimation(player2->aniFrames, 0, &player2->animator, true, 0);
            player2->state = Player_State_Static;
            return true;
        }
        else {
            Player->targetLeaderPosition.x = player1->position.x - 0x200000;
            Player->targetLeaderPosition.y = player1->position.y + 0x80000;
            player2->position.x            = Player->targetLeaderPosition.x;
            player2->direction             = FLIP_NONE;
        }
    }
    else {
        return host->timer > 10;
    }

    return false;
}
bool32 AIZSetup_CutsceneSonic_EnterClaw(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        player1->camera = NULL;
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, AIZSetup->platform->position.x - 0x400000, camera->position.y, 3);
        player1->stateInput = StateMachine_None;
        player1->state      = Player_State_Ground;
        player1->up         = true;
    }
    else {
        if (camera->position.x >= camera->endLerpPos.x) {
            if (host->storedTimer) {
                if (host->timer >= host->storedTimer + 60)
                    return true;
            }
            else {
                host->storedTimer = host->timer;
            }
        }
        else {
            player1->up = true;
            if (player2->classID == Player->classID) {
                player2->state = Player_State_Static;
            }
        }
    }
    return false;
}
bool32 AIZSetup_CutsceneSonic_WatchClaw(EntityCutsceneSeq *host)
{
    EntityAIZKingClaw *claw = AIZSetup->claw;

    if (claw->position.y < -0x1C0000) {
        claw->position.y += 0x10000;
    }
    else {
        if (host->storedTimer) {
            if (host->timer >= host->storedTimer + 60)
                return true;
        }
        else {
            host->storedTimer = host->timer;
            claw->state       = AIZKingClaw_State_Grab;
        }
    }

    return false;
}
bool32 AIZSetup_CutsceneSonic_RubyGrabbed(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player1);
    UNUSED(camera);

    EntityAIZKingClaw *claw  = AIZSetup->claw;
    EntityPlatform *platform = AIZSetup->platform;
    EntityPhantomRuby *ruby  = AIZSetup->phantomRuby;

    if (!host->timer) {
        claw->grabbedEntities[0] = (Entity *)platform;
        claw->grabbedEntities[1] = (Entity *)ruby;
        claw->grabCount          = 2;
    }

    if (claw->position.y > -0x520000) {
        AIZSetup->playDrillSfx = true;

        if (!(host->timer % 5))
            Camera_ShakeScreen(0, 0, 2);

        claw->position.y -= 0x4000;

        for (int32 i = 0; i < 3; ++i) {
            EntityDecoration *decor = AIZSetup->decorations[i];
            decor->drawFX |= FX_ROTATE;
            decor->rotation = ((2 * (!(i & 1)) - 1) * RSDK.Sin256(16 * host->timer)) >> 7;
        }
    }
    else {
        if (host->storedTimer) {
            if (host->values[0]) {
                if (!host->values[1] && host->timer >= host->storedTimer && host->timer == host->storedTimer) {
                    return true;
                }
            }
            else if (host->timer < host->storedTimer + 30) {
                Vector2 result   = MathHelpers_GetBezierPoint(((host->timer - host->storedTimer) << 16) / 30, claw->position.x, host->storedValue,
                                                            claw->position.x, host->storedValue - 0x500000, claw->position.x,
                                                            host->storedValue - 0x300000, claw->position.x, host->storedValue - 0x300000);
                claw->position.y = result.y;
            }
            else if (host->timer != host->storedTimer + 30) {
                if (!host->values[1] && host->timer >= host->storedTimer && host->timer == host->storedTimer) {
                    return true;
                }
            }
            else {
                host->values[0]   = 1;
                host->storedTimer = host->timer + 60;
            }
        }
        else {
            AIZSetup->playDrillSfx = false;

            host->storedTimer = host->timer;
            host->storedValue = claw->position.y;

            AIZSetup->decorations[0]->rotSpeed = 0;
            AIZSetup->decorations[1]->rotSpeed = 0;

            if (CHECK_CHARACTER_ID(ID_TAILS, 2))
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_SKID, &player2->animator, true, 0);

            RSDK.PlaySfx(AIZSetup->sfxBreak, false, 0x00);

            Music_TransitionTrack(TRACK_EGGMAN1, 0.05);
        }
    }

    return false;
}
bool32 AIZSetup_CutsceneSonic_RubyAppear(EntityCutsceneSeq *host)
{
    EntityPhantomRuby *ruby = AIZSetup->phantomRuby;

    if (!host->timer)
        PhantomRuby_SetupFlash(ruby);

    return ruby->flashFinished;
}
bool32 AIZSetup_CutsceneSonic_RubyFX(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntityPhantomRuby *ruby = AIZSetup->phantomRuby;

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = AIZSetup->fxRuby;
    }
    else {
        fxRuby            = CREATE_ENTITY(FXRuby, NULL, ruby->position.x, ruby->position.y);
        fxRuby->drawGroup = Zone->playerDrawGroup[1];
        AIZSetup->fxRuby  = fxRuby;
        Camera_ShakeScreen(0, 4, 4);
        player1->drawGroup = Zone->playerDrawGroup[1] + 1;
        if (player2->classID == Player->classID)
            player2->drawGroup = Zone->playerDrawGroup[1] + 1;
    }

    if (!host->values[0]) {
        if (fxRuby->fullyExpanded) {
            if (host->storedTimer) {
                if (host->timer == host->storedTimer + 30) {
                    fxRuby->delay = 64;
                    fxRuby->state = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
                    Camera_ShakeScreen(0, 4, 4);
                }
                else if (host->timer == host->storedTimer + 210) {
                    fxRuby->delay = 32;
                    fxRuby->state = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);
                    Camera_ShakeScreen(0, 4, 4);
                    Music_FadeOut(0.025);
                    host->storedTimer = host->timer;
                    host->values[0]   = true;
                }
            }
            else {
                host->storedTimer = host->timer;
            }

            if (host->timer >= host->storedTimer + 32) {
                int32 id = 0;
                for (int32 angle = 0; angle < 0x80; angle += 0x40) {
                    EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
                    if (!player || player->classID == TYPE_BLANK)
                        break;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);

                    int32 moveX = (player->position.x - player->position.x) >> 3;
                    int32 moveY = (0xA00 * RSDK.Sin256(2 * (host->timer + angle - host->storedTimer)) + ruby->position.y - player->position.y) >> 3;

                    player->position.x += moveX;
                    player->position.y += moveY;
                    player->state          = Player_State_Static;
                    player->tileCollisions = TILECOLLISION_NONE;
                    player->onGround       = false;
                }
            }
        }
    }
    else {
        if (fxRuby->fadeWhite >= 512) {
            if (fxRuby->fadeBlack >= 512) {
                if (host->timer == host->storedTimer + 150)
                    return true;
            }
            else {
                fxRuby->fadeBlack += 16;
            }
        }
        else {
            fxRuby->fadeWhite += 16;
        }
    }
    return false;
}

void AIZSetup_CutsceneK_Setup(void)
{
    RSDK_THIS(AIZSetup);
    CutsceneSeq_StartSequence(self, AIZSetup_CutsceneKnux_Chillin, AIZSetup_CutsceneKnux_StartDrillin, AIZSetup_CutsceneKnux_Drillin,
                              AIZSetup_CutsceneKnux_PrepareForTrouble, AIZSetup_CutsceneKnux_EnterThreat, AIZSetup_CutsceneKnux_HeaviesAppear,
                              AIZSetup_CutsceneKnux_RubyImpact, AIZSetup_CutsceneKnux_RubyFX, AIZSetup_Cutscene_LoadGHZ, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_CALLBACK, AIZSetup_Cutscene_SkipCB);
#endif
}

bool32 AIZSetup_CutsceneKnux_Chillin(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    if (!host->timer) {
        player1->state      = Player_State_Static;
        player1->stateInput = StateMachine_None;
        player1->direction  = FLIP_X;
        player1->drawFX     = FX_FLIP;
        RSDK.SetSpriteAnimation(AIZSetup->knuxFrames, 1, &player1->animator, true, 0);
    }

    return host->timer == 180;
}
bool32 AIZSetup_CutsceneKnux_StartDrillin(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    switch (host->timer) {
        case 0: RSDK.PlaySfx(AIZKingClaw->sfxClack, false, 0); break;

        case 10:
            RSDK.PlaySfx(AIZKingClaw->sfxWalkerLegs, false, 0);
            RSDK.SetSpriteAnimation(AIZSetup->knuxFrames, 2, &player1->animator, true, 0);
            break;

        case 40: return true;

        default: break;
    }

    return false;
}
bool32 AIZSetup_CutsceneKnux_Drillin(EntityCutsceneSeq *host)
{
    if (host->timer < 120) {
        AIZSetup->playDrillSfx = true;

        if (!(host->timer % 5))
            Camera_ShakeScreen(0, 0, 2);
    }
    else if (host->timer == 120) {
        AIZSetup->playDrillSfx = false;
        RSDK.PlaySfx(AIZSetup->sfxBreak, false, 0x00);
        Music_TransitionTrack(TRACK_HBHMISCHIEF, 0.02);
    }

    return host->timer == 200;
}
bool32 AIZSetup_CutsceneKnux_PrepareForTrouble(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    if (player1->animator.animationID == 3 && player1->animator.frameID == player1->animator.frameCount - 1) {
        RSDK.SetSpriteAnimation(AIZSetup->knuxFrames, 0, &player1->animator, true, 0);
    }

    if (!host->timer) {
        RSDK.SetSpriteAnimation(AIZSetup->knuxFrames, 3, &player1->animator, true, 0);
        foreach_active(Animals, animal)
        {
            animal->active    = ACTIVE_NORMAL;
            animal->behaviour = ANIMAL_BEHAVE_FREE;
        }
    }

    return host->timer == 60;
}
bool32 AIZSetup_CutsceneKnux_EnterThreat(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);

    if (!host->timer) {
        player1->camera = NULL;
        Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, player1->position.x - 0x600000, camera->position.y, 3);
    }
    else if (camera->position.x <= camera->endLerpPos.x) {
        if (host->storedTimer) {
            if (host->timer >= host->storedTimer + 60)
                return true;
        }
        else {
            host->storedTimer = host->timer;
        }
    }
    else {
        player1->state = Player_State_Static;
    }
    return false;
}
bool32 AIZSetup_CutsceneKnux_HeaviesAppear(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityAIZKingClaw *claw = AIZSetup->claw;
    EntityPhantomRuby *ruby = AIZSetup->phantomRuby;

    if (!host->timer) {
        claw->grabbedEntities[0] = (Entity *)AIZSetup->platform;
        claw->grabbedEntities[1] = (Entity *)ruby;
        claw->grabCount          = 2;
        claw->angle              = -16;
        claw->position.y         = -0x300000;
        claw->position.x         = 0x2F9C0000;
        ruby->sfx                = 0;
        PhantomRuby_SetupFlash(ruby);
        player1->drawGroup = Zone->playerDrawGroup[1] + 2;
        RSDK.PlaySfx(AIZSetup->sfxHeliWoosh, false, 0);
    }

    AIZSetup_HandleHeavyMovement();

    return ruby->position.x >= player1->position.x - 0x100000;
}
bool32 AIZSetup_CutsceneKnux_RubyImpact(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntityPhantomRuby *ruby = AIZSetup->phantomRuby;

    if (!host->timer) {
        RSDK.PlaySfx(AIZSetup->sfxImpact, false, 0x00);
        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
        Music_TransitionTrack(TRACK_EGGMAN1, 1.0);

        EntityFXRuby *fxRuby     = CREATE_ENTITY(FXRuby, NULL, ruby->position.x, ruby->position.y);
        fxRuby->drawGroup        = Zone->playerDrawGroup[1];
        AIZSetup->fxRuby         = fxRuby;
        player1->velocity.x      = 0x20000;
        player1->velocity.y      = -0x40000;
        player1->nextGroundState = StateMachine_None;
        player1->nextAirState    = StateMachine_None;
        player1->state           = Player_State_Static;
        player1->onGround        = false;
        player1->drawGroup       = Zone->playerDrawGroup[1] + 1;

        if (player2->classID == Player->classID)
            player2->drawGroup = Zone->playerDrawGroup[1] + 1;

        Camera_ShakeScreen(0, 4, 4);
    }

    AIZSetup_HandleHeavyMovement();

    if (player1->onGround) {
        if (player1->velocity.x > 0)
            player1->velocity.x -= 0x2000;
        if (player1->velocity.x < 0)
            player1->velocity.x = 0;
        player1->state = Player_State_Static;
        RSDK.SetSpriteAnimation(AIZSetup->knuxFrames, 5, &player1->animator, false, 0);
        return true;
    }
    else {
        player1->velocity.y += 0x3800;

        if (player1->velocity.x > 0)
            player1->velocity.x -= 0x1000;

        if (player1->velocity.x < 0)
            player1->velocity.x = 0;

        RSDK.SetSpriteAnimation(AIZSetup->knuxFrames, 4, &player1->animator, true, 0);
    }
    return false;
}
bool32 AIZSetup_CutsceneKnux_RubyFX(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    EntityFXRuby *fxRuby = AIZSetup->fxRuby;

    if (player1->velocity.x > 0)
        player1->velocity.x -= 0x1000;

    if (player1->velocity.x < 0)
        player1->velocity.x = 0;

    if (host->timer == 180) {
        fxRuby->delay = 32;
        fxRuby->state = FXRuby_State_IncreaseStageDeform;

        PhantomRuby_PlaySfx(RUBYSFX_ATTACK1);
        Camera_ShakeScreen(0, 4, 4);
        Music_FadeOut(0.025);

        host->storedTimer = host->timer;
        host->values[0]   = true;
    }

    AIZSetup_HandleHeavyMovement();

    if (host->values[0]) {
        if (fxRuby->fadeWhite < 0x200) {
            fxRuby->fadeWhite += 0x10;
        }
        else if (fxRuby->fadeBlack >= 0x200) {
            if (host->timer == host->storedTimer + 150)
                return true;
        }
        else {
            fxRuby->fadeBlack += 0x200;
        }
    }

    return false;
}

bool32 AIZSetup_Cutscene_LoadGHZ(EntityCutsceneSeq *host)
{
    UNUSED(host);

    RSDK.SetScene("Cutscenes", "Green Hill Zone");
    RSDK.LoadScene();

    return true;
}

#if GAME_INCLUDE_EDITOR
void AIZSetup_EditorDraw(void) {}

void AIZSetup_EditorLoad(void)
{
    RSDK_ACTIVE_VAR(BGSwitch, bgID);
    RSDK_ENUM_VAR("Jungle BG", AIZ_BG_JUNGLE);
    RSDK_ENUM_VAR("Sky BG", AIZ_BG_SKY);

    RSDK_ACTIVE_VAR(Decoration, type);
    RSDK_ENUM_VAR("Leaf 1", AIZ_DECOR_LEAF1);
    RSDK_ENUM_VAR("Leaf 2", AIZ_DECOR_LEAF2);
    RSDK_ENUM_VAR("Leaf 3", AIZ_DECOR_LEAF3);
    RSDK_ENUM_VAR("Leaf 4", AIZ_DECOR_LEAF4);
}
#endif

void AIZSetup_Serialize(void) {}
