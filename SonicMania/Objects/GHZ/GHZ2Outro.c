// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GHZ2Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGHZ2Outro *GHZ2Outro;

void GHZ2Outro_Update(void)
{
    RSDK_THIS(GHZ2Outro);

    if (!self->activated) {
        CutsceneSeq_StartSequence(self, GHZ2Outro_Cutscene_HoleSceneFadeIn, GHZ2Outro_Cutscene_SpyOnEggman, GHZ2Outro_Cutscene_BreakupGroup,
                                  GHZ2Outro_Cutscene_RubyHover, GHZ2Outro_Cutscene_StartRubyWarp, GHZ2Outro_Cutscene_HandleRubyWarp,
                                  GHZ2Outro_Cutscene_LoadCPZ1, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_CALLBACK, GHZ2Outro_Cutscene_SkipCB);
#endif

        self->active = ACTIVE_NEVER;
    }
    else {
        CutsceneSeq_StartSequence(self, GHZ2Outro_Cutscene_FinishActClear, GHZ2Outro_Cutscene_JumpIntoHole, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN, StateMachine_None);
#endif

        HUD_MoveOut();
        self->active = ACTIVE_NEVER;
    }
}

void GHZ2Outro_LateUpdate(void) {}

void GHZ2Outro_StaticUpdate(void) {}

void GHZ2Outro_Draw(void) {}

void GHZ2Outro_Create(void *data)
{
    RSDK_THIS(GHZ2Outro);

    if (!SceneInfo->inEditor) {
        self->activated = data != NULL;

        foreach_all(DERobot, robot) { self->DERobot = robot; }
        foreach_all(Eggman, eggman) { self->eggman = eggman; }

        self->active  = ACTIVE_NORMAL;
        self->visible = false;
    }
}

void GHZ2Outro_StageLoad(void)
{
    GHZ2Outro->sfxRocketJet   = RSDK.GetSfx("Stage/RocketJet.wav");
    GHZ2Outro->sfxShinobiJump = RSDK.GetSfx("PSZ/ShinobiJump.wav");
    GHZ2Outro->sfxHeliWoosh   = RSDK.GetSfx("SPZ1/HeliWooshIn.wav");
}

bool32 GHZ2Outro_Cutscene_FinishActClear(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    EntityDERobot *robot = self->DERobot;
    robot->state         = StateMachine_None;
    robot->active        = ACTIVE_NEVER;

    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state = Player_State_Ground;
        if (!player->sidekick) {
            player->stateInput = StateMachine_None;
            player->left       = true;
            player->right      = false;
            player->up         = false;
            player->down       = false;
            player->jumpPress  = false;
        }
    }

    Zone->deathBoundary[0] += 0x4000 << 16;
    Zone->deathBoundary[1] += 0x4000 << 16;
    Music_PlayTrack(TRACK_STAGE);

    foreach_active(EggPrison, prison) { prison->state = EggPrison_State_FlyOffScreen; }

    return true;
}
bool32 GHZ2Outro_Cutscene_JumpIntoHole(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    EntityDERobot *robot = self->DERobot;

    foreach_active(Player, player)
    {
        player->jumpPress = false;

        if (player->onGround) {
            if (player->angle > 8 && player->angle < 64 && player->groundVel > -0x20000)
                player->groundVel = -0x20000;

            if (player->position.x - robot->position.x < 0x600000) {
                player->jumpPress = true;
                player->jumpHold  = true;
            }
        }

        if (player->position.x < robot->position.x + 0x200000)
            player->velocity.x -= player->velocity.x >> 4;

        if (player->position.x < robot->position.x - 0x200000)
            player->position.x = robot->position.x - 0x200000;
    }

    if (player->position.y > (ScreenInfo->position.y + 0x200) << 16) {
        foreach_active(Player, playerPtr) { playerPtr->active = ACTIVE_NEVER; }
        RSDK.SetScene("Cutscenes", "Green Hill Zone 2");

        Zone_StartFadeOut(0x10, 0x000000);
        return true;
    }

    return false;
}

bool32 GHZ2Outro_Cutscene_HoleSceneFadeIn(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    if (host->timer >= 8) {
        CutsceneSeq_LockAllPlayerControl();

        foreach_active(Player, player)
        {
            player->stateInput = StateMachine_None;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
        }

        foreach_all(CutsceneHBH, cutsceneHBH) { cutsceneHBH->drawGroup = Zone->objectDrawGroup[0]; }

        foreach_all(DERobot, robot)
        {
            robot->state   = StateMachine_None;
            robot->active  = ACTIVE_NORMAL;
            robot->visible = true;
        }

        EntityDERobot *deRobot = self->DERobot;
        deRobot->state         = DERobot_State_CutsceneExplode;

        CutsceneHBH_ShinobiBounceSetup();
        CutsceneHBH_KingSetup();
        CutsceneHBH_RiderSetup();

        EntityEggman *eggman = self->eggman;
        RSDK.SetSpriteAnimation(Eggman->aniFrames, 9, &eggman->animator, true, 0);
        eggman->direction = FLIP_NONE;
        eggman->state     = Eggman_State_ProcessAnimation;

        foreach_all(PhantomRuby, ruby)
        {
            self->phantomRuby = ruby;
            ruby->state       = PhantomRuby_State_MoveRotateGravity;
            ruby->drawFX      = FX_ROTATE;
        }

        FXRuby_SetupLayerDeformation();
        return true;
    }

    return false;
}
bool32 GHZ2Outro_Cutscene_SpyOnEggman(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    foreach_active(Player, player)
    {
        if (player->sidekick) {
            if (host->timer >= 60) {
                if (host->timer == 60) {
                    player->state = Player_State_Ground;
                    player->right = true;
                }
            }
            else {
                player->groundVel = 0;
                if (player->onGround && player->groundedStore) {
                    player->state = Player_State_Static;
                    if (player->characterID == ID_TAILS)
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_BALANCE_2, &player->animator, false, 0);
                    else
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
                }
            }
        }
        else if (player->onGround && player->groundedStore) {
            player->right = true;
        }

        if (player->position.x > self->position.x - 0x180000 * player->playerID - 0xE00000) {
            player->right     = FLIP_NONE;
            player->direction = FLIP_NONE;
            if (!player->skidding && player->groundVel > 0) {
                player->skidding = 12;
            }

            player->groundVel -= player->groundVel >> 4;
            if (player->groundVel < 0x10000)
                player->skidding = 0;
        }
    }

    if (host->timer == 90)
        Music_TransitionTrack(TRACK_HBHMISCHIEF, 0.0125);

    if (host->timer == 120)
        Camera_SetupLerp(CAMERA_LERP_SIN512, 0, self->position.x, self->position.y, 4);

    if (host->timer >= 208) {
        foreach_active(Player, playerPtr)
        {
            playerPtr->down   = true;
            playerPtr->camera = NULL;
        }
        return true;
    }
    return false;
}
bool32 GHZ2Outro_Cutscene_BreakupGroup(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    EntityEggman *eggman = self->eggman;

    switch (host->timer) {
        default: break;

        case 90:
            RSDK.SetSpriteAnimation(Eggman->aniFrames, 5, &eggman->animator, true, 2);
            eggman->timer      = 30;
            eggman->velocity.x = 0;
            eggman->velocity.y = -0x30000;
            eggman->nextState  = Eggman_State_ProcessUntilEnd;
            eggman->state      = Eggman_State_FallUntilTimerReset;
            eggman->animID     = 3;
            break;

        case 160: {
            foreach_active(DERobot, robot)
            {
                if (robot->aniID == DEROBOT_ARM && robot->frameID == 2) {
                    robot->state = DERobot_Cutscene_ActivateArm;
                }
            }

            RSDK.PlaySfx(DERobot->sfxButton2, false, 255);
            break;
        }

        case 240:
            eggman->state  = Eggman_State_ProcessThenSet;
            eggman->animID = 2;
            Music_SetMusicTrack("RubyPresence.ogg", TRACK_RUBYPRESENCE, 198457);
            Music_TransitionTrack(TRACK_EGGMAN1, 0.025);
            break;

        case 272: {
            EntityCutsceneHBH *mystic = CutsceneHBH_GetEntity(HBH_MYSTIC);
            if (mystic)
                mystic->state = CutsceneHBH_State_MysticExit;

            Camera_SetupLerp(CAMERA_LERP_NORMAL, 0, self->position.x, self->position.y - 0x400000, 1);
            RSDK.PlaySfx(GHZ2Outro->sfxRocketJet, false, 0xFF);
            break;
        }

        case 288: {
            EntityCutsceneHBH *rider = CutsceneHBH_GetEntity(HBH_RIDER);
            if (rider) {
                if (rider->direction == FLIP_X) {
                    rider->timer = 0;
                    RSDK.SetSpriteAnimation(rider->aniFrames, 2, &rider->mainAnimator, true, 0);
                }
                rider->state = CutsceneHBH_State_RiderExit;
            }

            RSDK.PlaySfx(GHZ2Outro->sfxHeliWoosh, false, 255);
            break;
        }

        case 304: {
            CutsceneHBH_ShinobiJumpSetup();

            RSDK.PlaySfx(GHZ2Outro->sfxShinobiJump, false, 255);
            break;
        }

        case 320: {
            EntityCutsceneHBH *gunner = CutsceneHBH_GetEntity(HBH_GUNNER);
            if (gunner) {
                RSDK.SetSpriteAnimation(gunner->aniFrames, 4, &gunner->mainAnimator, true, 0);
                gunner->state = CutsceneHBH_State_GunnerExit;
            }

            EntityCutsceneHBH *king = CutsceneHBH_GetEntity(HBH_KING);
            if (king) {
                king->direction ^= FLIP_X;
                king->state = CutsceneHBH_State_KingExit;
            }

            RSDK.PlaySfx(GHZ2Outro->sfxRocketJet, false, 0xFF);
            foreach_active(Player, player) { player->down = false; }
            EntityPhantomRuby *ruby = self->phantomRuby;
            ruby->startPos.x        = ruby->position.x;
            ruby->startPos.y        = ruby->position.y;
            ruby->state             = PhantomRuby_State_Oscillate;
            return true;
        }
    }

    return false;
}
bool32 GHZ2Outro_Cutscene_RubyHover(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);
    EntityPlayer *player2   = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    EntityPhantomRuby *ruby = self->phantomRuby;

    if (ruby) {
        if (ruby->state == PhantomRuby_State_Oscillate) {
            if (player2->classID == Player->classID && player2->characterID == ID_TAILS) {
                player2->state = Player_State_Static;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_SKID, &player2->animator, false, 0);
            }
            return true;
        }
    }

    return false;
}
bool32 GHZ2Outro_Cutscene_StartRubyWarp(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    EntityPhantomRuby *ruby = self->phantomRuby;
    if (!host->timer)
        PhantomRuby_SetupFlash(ruby);

    if (ruby->flashFinished) {
        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
        return true;
    }

    return false;
}
bool32 GHZ2Outro_Cutscene_HandleRubyWarp(EntityCutsceneSeq *host)
{
    RSDK_THIS(GHZ2Outro);

    EntityPhantomRuby *ruby = self->phantomRuby;
    EntityPlayer *player1   = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2   = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = self->fxRuby;
    }
    else {
        fxRuby            = CREATE_ENTITY(FXRuby, NULL, ruby->position.x, ruby->position.y);
        fxRuby->drawGroup = Zone->playerDrawGroup[1];
        self->fxRuby      = fxRuby;
        Camera_ShakeScreen(0, 4, 4);
        player1->drawGroup = Zone->playerDrawGroup[1] + 1;
        if (player2->classID == Player->classID)
            player2->drawGroup = Zone->playerDrawGroup[1] + 1;
    }

    if (!host->values[0]) {
        if (fxRuby->fullyExpanded) {
            if (host->storedTimer) {
                if (host->timer == host->storedTimer + 48) {
                    fxRuby->delay = 64;
                    fxRuby->state = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
                    Camera_ShakeScreen(0, 4, 4);
                }
                else if (host->timer == host->storedTimer + 180) {
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

            if (host->timer >= host->storedTimer + 52) {
                int32 id = 0;
                for (int32 angle = 0; angle < 0x80; angle += 0x10) {
                    EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
                    if (!player || player->classID == TYPE_BLANK)
                        break;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);

                    int32 valX = (ruby->position.x - 0x400000) - player->position.x;
                    int32 valY = (ruby->position.y - (0xA00000 + 944)) - player->position.y;

                    player->position.x += ((RSDK.Cos256(2 * (angle + host->timer - host->storedTimer)) << 12) + valX) >> 5;
                    player->position.y += ((RSDK.Sin256(2 * (angle + host->timer - host->storedTimer)) << 12) + valY) >> 5;
                    player->state          = Player_State_Static;
                    player->tileCollisions = TILECOLLISION_NONE;
                    player->onGround       = false;
                }
            }
        }
    }
    else {
        if (fxRuby->fadeWhite >= 0x200) {
            if (fxRuby->fadeBlack >= 0x200) {
                if (host->timer == host->storedTimer + 150)
                    return true;
            }
            else {
                fxRuby->fadeBlack += 0x10;
            }
        }
        else {
            fxRuby->fadeWhite += 0x10;
        }
    }

    return false;
}

bool32 GHZ2Outro_Cutscene_LoadCPZ1(EntityCutsceneSeq *host)
{
    if (host->timer == 16) {
        globals->enableIntro = true;

#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetScene("Encore Mode", "Chemical Plant Zone+ 1");
        else
#endif
            RSDK.SetScene("Mania Mode", "Chemical Plant Zone 1");

        RSDK.LoadScene();
        return true;
    }
    return false;
}

#if MANIA_USE_PLUS
void GHZ2Outro_Cutscene_SkipCB(void)
{
#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "Chemical Plant Zone+ 1");
    else
#endif
        RSDK.SetScene("Mania Mode", "Chemical Plant Zone 1");
}
#endif

#if GAME_INCLUDE_EDITOR
void GHZ2Outro_EditorDraw(void)
{
    RSDK_THIS(GHZ2Outro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void GHZ2Outro_EditorLoad(void) {}
#endif

void GHZ2Outro_Serialize(void) { RSDK_EDITABLE_VAR(GHZ2Outro, VAR_VECTOR2, size); }
