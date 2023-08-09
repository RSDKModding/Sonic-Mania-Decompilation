// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZ2Intro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPSZ2Intro *PSZ2Intro;

void PSZ2Intro_Update(void)
{
    RSDK_THIS(PSZ2Intro);

    self->activated = true;

    CutsceneSeq_StartSequence(self, PSZ2Intro_Cutscene_HandleAct1Finish, PSZ2Intro_Cutscene_ShowActClear, PSZ2Intro_Cutscene_RunToAct2,
                              PSZ2Intro_Cutscene_JogIntoPlace, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

    self->active = ACTIVE_NEVER;
}

void PSZ2Intro_LateUpdate(void) {}

void PSZ2Intro_StaticUpdate(void) {}

void PSZ2Intro_Draw(void) {}

void PSZ2Intro_Create(void *data)
{
    RSDK_THIS(PSZ2Intro);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_BOUNDS;
}

void PSZ2Intro_StageLoad(void)
{
    PSZ2Intro->sfxExplosion3 = RSDK.GetSfx("Stage/Explosion3.wav");

    foreach_all(FXFade, fxFade)
    {
        PSZ2Intro->fxFade = fxFade;
        foreach_break;
    }

    if (!isMainGameMode() || !globals->atlEnabled || CutsceneRules_CheckStageReload()) {
        foreach_all(PSZ2Intro, intro) { destroyEntity(intro); }
    }
}

bool32 PSZ2Intro_Cutscene_HandleAct1Finish(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    EntityFXFade *fxFade = PSZ2Intro->fxFade;
    EntitySignPost *post = PSZ2Intro->signPost;

    if (!host->timer) {
        Zone->cameraBoundsR[0]      = 1024;
        Zone->cameraBoundsR[1]      = 1024;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveR[1] = true;
        player1->pushing            = false;
        if (player2->classID == Player->classID)
            player2->pushing = false;
    }

    if (host->values[0]) {
        if (host->timer - host->storedTimer == 30) {
            ActClear->displayedActID = 1;
            post->state              = SignPost_State_Falling;
            post->active             = ACTIVE_NORMAL;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 0xFF);

            return true;
        }
    }
    else if (!fxFade->timer) {
        host->values[0]   = true;
        host->storedTimer = host->timer;

        foreach_all(SignPost, signPost)
        {
            PSZ2Intro->signPost = signPost;
            foreach_break;
        }
    }

    return false;
}

bool32 PSZ2Intro_Cutscene_ShowActClear(EntityCutsceneSeq *host)
{
    if (ActClear->finished) {
#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif

        return true;
    }

    return false;
}

bool32 PSZ2Intro_Cutscene_RunToAct2(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (!host->timer) {
        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);
        Zone->cameraBoundsR[0] = size.x;
        Zone->cameraBoundsR[1] = size.x;

        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);
        player1->state      = Player_State_Ground;
        player1->up         = false;
        player1->stateInput = StateMachine_None;

        CutsceneSeq_LockAllPlayerControl();
        player1->jumpPress = false;
        player1->jumpHold  = false;
        player1->left      = false;
        player1->down      = false;

        if (player2->classID == Player->classID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player1->animator, true, 0);
            player2->state      = Player_State_Ground;
            player2->up         = false;
            player2->stateInput = Player_Input_P2_AI;
        }
    }

    if (player1->jumpPress)
        player1->jumpPress = false;

    if (abs(player1->pushing) >= 3) {
        player1->jumpPress = true;
        player1->jumpHold  = true;
    }

    if (player1->position.x >= 0x3800000) {
        player1->right         = false;
        Zone->cameraBoundsL[0] = 1024;
        Zone->cameraBoundsL[1] = 1024;
        return true;
    }
    else {
        player1->right = true;
    }

    return false;
}

bool32 PSZ2Intro_Cutscene_JogIntoPlace(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (ScreenInfo->position.x < Zone->cameraBoundsL[0]) {
        // Bug Details:
        // groundVel is set here, but the game doesn't make sure you're facing that direction
        // meaning you can be moving right but facing left, a simple player->direction = FLIP_NONE would fix this
        // though it'd be best to place it up where player->right = true; is set
        if (player1->groundVel < 0x20000)
            player1->groundVel = 0x20000;

        if (player2->classID == Player->classID) {
            if (player2->groundVel < 0x20000)
                player2->groundVel = 0x20000;
        }
    }
    else {
        player1->stateInput = Player_Input_P1;
        player1->state      = Player_State_Ground;

        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_State_SetupBGElements;
            titleCard->stateDraw = TitleCard_Draw_SlideIn;
            foreach_break;
        }

        Music_PlayTrack(TRACK_STAGE);
        return true;
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void PSZ2Intro_EditorDraw(void)
{
    RSDK_THIS(PSZ2Outro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void PSZ2Intro_EditorLoad(void) {}
#endif

void PSZ2Intro_Serialize(void) { RSDK_EDITABLE_VAR(PSZ2Intro, VAR_VECTOR2, size); }
