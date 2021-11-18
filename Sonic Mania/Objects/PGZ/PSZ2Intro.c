#include "SonicMania.h"

ObjectPSZ2Intro *PSZ2Intro;

void PSZ2Intro_Update(void)
{
    RSDK_THIS(PSZ2Intro);
    void *states[] = { PSZ2Intro_CutsceneState_Unknown1, PSZ2Intro_CutsceneState_Unknown2, PSZ2Intro_CutsceneState_Unknown3,
                       PSZ2Intro_CutsceneState_Unknown4, NULL };

    self->activated = true;
    CutsceneSeq_StartSequence((Entity *)self, states);
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
    PSZ2Intro->sfxExplosion3 = RSDK.GetSFX("Stage/Explosion3.wav");
    foreach_all(FXFade, fxFade)
    {
        PSZ2Intro->fxFade = fxFade;
        foreach_break;
    }

    if (!isMainGameMode() || !globals->atlEnabled || PlayerHelpers_CheckStageReload()) {
        foreach_all(PSZ2Intro, intro) { destroyEntity(intro); }
    }
}

bool32 PSZ2Intro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    
    EntityFXFade *fxFade = PSZ2Intro->fxFade;
    EntitySignPost *post = (EntitySignPost *)PSZ2Intro->signPost;
    if (!host->timer) {
        Zone->screenBoundsR1[0]     = 1024;
        Zone->screenBoundsR1[1]     = 1024;
        Zone->playerBoundActiveR[0] = true;
        Zone->playerBoundActiveR[1] = true;
        player1->pushing            = false;
        if (player2->objectID == Player->objectID)
            player2->pushing = false;
    }
    if (host->field_6C[0]) {
        if (host->timer - host->field_68 == 30) {
            ActClear->actID = 1;
            post->state     = SignPost_State_Fall;
            post->active    = ACTIVE_NORMAL;
            RSDK.PlaySfx(SignPost->sfxTwinkle, false, 255);
            return true;
        }
    }
    else if (!fxFade->timer) {
        host->field_6C[0] = 1;
        host->field_68    = host->timer;
        foreach_all(SignPost, post)
        {
            PSZ2Intro->signPost = (Entity *)post;
            foreach_break;
        }
    }
    return false;
}

bool32 PSZ2Intro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    if (ActClear->finished) {
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
        return true;
    }
    return false;
}

bool32 PSZ2Intro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    
    if (!host->timer) {
        Vector2 size;
        RSDK.GetLayerSize(Zone->fgLow, &size, true);
        Zone->screenBoundsR1[0] = size.x;
        Zone->screenBoundsR1[1] = size.x;
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->playerAnimator, true, 0);
        player1->state      = Player_State_Ground;
        player1->up         = false;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        player1->jumpPress = false;
        player1->jumpHold  = false;
        player1->left      = false;
        player1->down      = false;
        if (player2->objectID == Player->objectID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player1->playerAnimator, true, 0);
            player2->state      = Player_State_Ground;
            player2->up         = false;
            player2->stateInput = Player_ProcessP2Input_AI;
        }
    }
    if (player1->jumpPress)
        player1->jumpPress = false;

    if (abs(player1->pushing) >= 3) {
        player1->jumpPress = true;
        player1->jumpHold  = true;
    }
    if (player1->position.x >= 0x3800000) {
        player1->right          = false;
        Zone->screenBoundsL1[0] = 1024;
        Zone->screenBoundsL1[1] = 1024;
        return true;
    }
    else {
        player1->right = true;
    }
    return false;
}

bool32 PSZ2Intro_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    if (ScreenInfo->position.x < Zone->screenBoundsL1[0]) {
        if (player1->groundVel < 0x20000)
            player1->groundVel = 0x20000;
        if (player2->objectID == Player->objectID) {
            if (player2->groundVel < 0x20000)
                player2->groundVel = 0x20000;
        }
    }
    else {
        player1->stateInput = Player_ProcessP1Input;
        player1->state      = Player_State_Ground;
        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_Unknown6;
            titleCard->stateDraw = TitleCard_StateDraw_Default;
            foreach_break;
        }
        Music_PlayTrack(TRACK_STAGE);
        return true;
    }
    return false;
}

#if RETRO_INCLUDE_EDITOR
void PSZ2Intro_EditorDraw(void)
{
    RSDK_THIS(PSZ2Outro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void PSZ2Intro_EditorLoad(void) {}
#endif

void PSZ2Intro_Serialize(void) { RSDK_EDITABLE_VAR(PSZ2Intro, VAR_VECTOR2, size); }
