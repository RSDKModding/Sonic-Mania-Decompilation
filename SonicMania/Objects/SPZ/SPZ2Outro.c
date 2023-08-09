// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SPZ2Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSPZ2Outro *SPZ2Outro;

void SPZ2Outro_Update(void)
{
    RSDK_THIS(SPZ2Outro);

    if (!self->activated) {
        SPZ2Outro_StartCutscene();
        self->activated = true;
    }
}

void SPZ2Outro_LateUpdate(void) {}

void SPZ2Outro_StaticUpdate(void) {}

void SPZ2Outro_Draw(void) {}

void SPZ2Outro_Create(void *data)
{
    RSDK_THIS(SPZ2Outro);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NEVER;
}

void SPZ2Outro_StageLoad(void)
{
    SPZ2Outro->unused = 0;

    SPZ2Outro->weatherTV = NULL;
    foreach_all(TVFlyingBattery, tvFlyingBattery)
    {
        SPZ2Outro->tvFlyingBattery = tvFlyingBattery;
        foreach_break;
    }

    SPZ2Outro->weatherTV = NULL;
    foreach_all(WeatherTV, weatherTV)
    {
        SPZ2Outro->weatherTV = weatherTV;
        foreach_break;
    }
}

void SPZ2Outro_StartCutscene(void)
{
    RSDK_THIS(SPZ2Outro);

    CutsceneSeq_StartSequence(self, SPZ2Outro_Cutscene_SetupFBZTV, SPZ2Outro_Cutscene_ExitStageRight, SPZ2Outro_Cutscene_AsSeenOnTV,
                              SPZ2Outro_Cutscene_FBZFlyAway, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif

    HUD_MoveOut();
}

bool32 SPZ2Outro_Cutscene_SetupFBZTV(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);

    EntityTVFlyingBattery *tvFlyingBattery = SPZ2Outro->tvFlyingBattery;
    EntityWeatherTV *weatherTV             = SPZ2Outro->weatherTV;

    if (!host->timer) {
        foreach_all(EggPrison, prisonPtr)
        {
            SPZ2Outro->prison = prisonPtr;
            foreach_break;
        }

        EntityEggPrison *prison     = SPZ2Outro->prison;
        prison->notSolid            = true;
        prison->drawGroup           = Zone->playerDrawGroup[1];
        tvFlyingBattery->position.y = 0x81E0000;
        tvFlyingBattery->active     = ACTIVE_NORMAL;
        weatherTV->stateDraw        = WeatherTV_Draw_Outro;
        weatherTV->state            = WeatherTV_State_Outro;
        weatherTV->timer            = 0;
        RSDK.AddDrawListRef(Zone->objectDrawGroup[1], RSDK.GetEntitySlot(weatherTV));
        WeatherTV->useHighLayer = true;
        camera->target          = NULL;
        player1->camera         = NULL;
#if GAME_VERSION != VER_100
        Player->disableP2KeyCheck = true;
#endif

        CutsceneSeq_LockAllPlayerControl();
        player1->stateInput = StateMachine_None;
        player1->jumpPress  = false;
        player1->jumpHold   = false;
        player1->up         = false;
        player1->down       = false;
        player1->left       = false;
        player1->right      = false;

        if (player2->classID == Player->classID) {
            player2->stateInput = StateMachine_None;
            player2->jumpPress  = false;
            player2->jumpHold   = false;
            player2->up         = false;
            player2->down       = false;
            player2->left       = false;
            player2->right      = false;
        }
    }

    if (player2->classID == Player->classID) {
        player2->stateInput = StateMachine_None;
        player2->jumpPress  = false;
        player2->jumpHold   = false;
        player2->down       = false;
        player2->left       = false;
        player2->right      = false;
    }

    if (host->timer == 15) {
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_LOOK_UP, &player1->animator, true, 0);
        player1->state     = Player_State_LookUp;
        player1->up        = true;
        player1->direction = player1->position.x > weatherTV->position.x;

        if (player2->classID == Player->classID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_LOOK_UP, &player2->animator, true, 0);
            player2->state     = Player_State_LookUp;
            player2->up        = true;
            player2->direction = player2->position.x > weatherTV->position.x;
        }
    }

    if (host->timer == 120) {
        Zone->playerBoundActiveR[0] = false;
        if (!player2->onGround || player2->state == Player_State_FlyToPlayer || player2->state == Player_State_ReturnToPlayer
            || player2->state == Player_State_Static || player2->state == Player_State_HoldRespawn) {
            SPZ2Outro->ignoreP2 = true;
        }
        else {
            Zone->playerBoundActiveR[1] = false;
        }

        return true;
    }

    return false;
}

bool32 SPZ2Outro_Cutscene_ExitStageRight(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (!host->timer) {
        RSDK.SetSpriteAnimation(player1->aniFrames, 0, &player1->animator, true, 0);
        player1->state = Player_State_Ground;
        player1->up    = false;
        player1->right = true;

        if (player2) {
            if (player2->classID == Player->classID && !SPZ2Outro->ignoreP2) {
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
                player2->state      = Player_State_Ground;
                player2->up         = false;
                player2->stateInput = Player_Input_P2_AI;
            }
        }
    }

    int32 posX = ((ScreenInfo->size.x + ScreenInfo->position.x) << 16) + 0x100000;
    if (player1->position.x > posX) {
        player1->right = false;
        if (player2->classID != Player->classID || SPZ2Outro->ignoreP2 || player2->position.x > posX) {
            EntityShield *shield = RSDK_GET_ENTITY(player1->playerID + Player->playerCount, Shield);
            if (shield->classID == Shield->classID) {
                player1->shield = SHIELD_NONE;
                destroyEntity(shield);
            }

            return true;
        }
    }

    return false;
}

void SPZ2Outro_DrawHook_PrepareWeatherTV(void)
{
    EntityWeatherTV *weatherTV = SPZ2Outro->weatherTV;
    int32 x                    = (weatherTV->position.x >> 16) - ScreenInfo->position.x;
    int32 y                    = (weatherTV->position.y >> 16) - ScreenInfo->position.y;
    RSDK.SetClipBounds(0, x - 96, y - 64, x + 96, y + 64);
}

// State where they are on da TV and jump onto FBZ
bool32 SPZ2Outro_Cutscene_AsSeenOnTV(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);
    EntityTVFlyingBattery *tvFlyingBattery = SPZ2Outro->tvFlyingBattery;
    EntityWeatherTV *weatherTV             = SPZ2Outro->weatherTV;

    if (!host->timer) {
        RSDK.SetDrawGroupProperties(Zone->playerDrawGroup[0], false, SPZ2Outro_DrawHook_PrepareWeatherTV);
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_RUN, &player1->animator, true, 0);
        player1->drawGroup       = Zone->playerDrawGroup[0];
        player1->state           = Player_State_Static;
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        player1->position.x      = weatherTV->position.x - 0x700000;
        player1->position.y      = weatherTV->position.y + 0x140000;
        host->storedTimer        = player1->position.y;
        player1->onGround        = false;
        player1->right           = false;
        player1->velocity.x      = 0x10000;
        player1->velocity.y      = 0;
        player1->scale.x         = 0x100;
        player1->scale.y         = 0x100;
        player1->drawFX          = FX_SCALE | FX_FLIP;

        if (player2 && player2->classID == Player->classID && !SPZ2Outro->ignoreP2) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_RUN, &player2->animator, true, 0);
            player2->drawGroup       = Zone->playerDrawGroup[0];
            player2->state           = Player_State_Static;
            player2->nextAirState    = StateMachine_None;
            player2->nextGroundState = StateMachine_None;
            player2->position.x      = player1->position.x - 0x100000;
            player2->position.y      = player1->position.y;
            player2->onGround        = false;
            player2->stateInput      = StateMachine_None;
            player2->right           = false;
            player2->velocity.x      = 0x10000;
            player2->velocity.y      = 0;
            player2->scale.x         = 0x100;
            player2->scale.y         = 0x100;
            player2->drawFX          = FX_SCALE | FX_FLIP;
        }
    }

    EntityPlayer *players[2] = { player1, player2 };

    bool32 finished = true;
    for (int32 p = 0; p < 2; ++p) {
        if (p != 1 || (players[p]->classID == Player->classID && !SPZ2Outro->ignoreP2)) {
            EntityPlayer *player = players[p];

            if (player->position.x >= weatherTV->position.x && !player->velocity.y && player->animator.animationID == ANI_RUN) {
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                RSDK.PlaySfx(Player->sfxJump, false, 255);
                player->velocity.y = -0x20000;
            }

            if (player->position.y <= tvFlyingBattery->position.y - 0x80000 && player->animator.animationID == ANI_JUMP) {
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_CLING, &player->animator, true, 0);
                player->velocity.y = 0;
                player->direction  = FLIP_X;
            }

            if (player->animator.animationID == ANI_CLING)
                player->position.y = tvFlyingBattery->position.y - 0x80000;

            finished = finished && player->animator.animationID == ANI_CLING;
        }
    }

    return finished;
}

bool32 SPZ2Outro_Cutscene_FBZFlyAway(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntityTVFlyingBattery *tvFlyingBattery = SPZ2Outro->tvFlyingBattery;

    if (host->timer == 30)
        Zone_StartFadeOut(10, 0x000000);

    int32 volume = MAX(30 - host->timer, 0);
    if (host->timer - 30 > 90)
        volume = -90;

    RSDK.SetChannelAttributes(TVFlyingBattery->flyoverChannel, (volume + 90) / 90.0, 0.0, 1.0);

    if (player1->animator.animationID == ANI_CLING)
        player1->position.y = tvFlyingBattery->position.y - 0x80000;

    if (player2->classID == Player->classID && !SPZ2Outro->ignoreP2 && player2->animator.animationID == ANI_CLING)
        player2->position.y = tvFlyingBattery->position.y - 0x80000;

    return false;
}

#if GAME_INCLUDE_EDITOR
void SPZ2Outro_EditorDraw(void)
{
    RSDK_THIS(SPZ2Outro);

    if (showGizmos())
        CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void SPZ2Outro_EditorLoad(void) {}
#endif

void SPZ2Outro_Serialize(void) { RSDK_EDITABLE_VAR(SPZ2Outro, VAR_VECTOR2, size); }
