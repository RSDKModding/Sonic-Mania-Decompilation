// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: OOZ1Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectOOZ1Outro *OOZ1Outro;

void OOZ1Outro_Update(void)
{
    RSDK_THIS(OOZ1Outro);

    if (isMainGameMode() && globals->enableIntro && !CutsceneRules_CheckStageReload()) {
        self->activated = true;
        CutsceneSeq_StartSequence(self, OOZ1Outro_Cutscene_FadeIn, OOZ1Outro_Cutscene_PostActClearSetup, OOZ1Outro_Cutscene_FallIntoAct2,
                                  OOZ1Outro_Cutscene_BeginAct, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif
    }

    self->active = ACTIVE_NEVER;
}

void OOZ1Outro_LateUpdate(void) {}

void OOZ1Outro_StaticUpdate(void) {}

void OOZ1Outro_Draw(void) {}

void OOZ1Outro_Create(void *data)
{
    RSDK_THIS(OOZ1Outro);

    if (!SceneInfo->inEditor) {
        self->active  = ACTIVE_NORMAL;
        self->visible = false;
        int32 rangeX  = self->updateRange.x + self->size.x;
        int32 rangeY  = self->updateRange.y + self->size.y;

        CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);

        // This one's slightly weird, it doesn't set updateRange the same way for some reason, so lets keep that behaviour
        self->updateRange.x = rangeX;
        self->updateRange.y = rangeY;
    }
}

void OOZ1Outro_StageLoad(void) { OOZ1Outro->sfxDrop = RSDK.GetSfx("Stage/Drop.wav"); }

bool32 OOZ1Outro_Cutscene_FadeIn(EntityCutsceneSeq *host)
{
    RSDK_THIS(OOZ1Outro);

    if (host->timer) {
        if (host->fadeWhite > 0)
            host->fadeWhite -= 4;
    }
    else {
        host->fadeWhite = 512;
    }

    if (host->timer == 1) {
        foreach_all(Player, player)
        {
            player->position.x = self->position.x;
            player->position.y = self->position.y;
            player->position.y += (self->size.y >> 1) - 0x400000;
        }

        self->boundsR          = Zone->cameraBoundsR[0];
        self->boundsT          = Zone->cameraBoundsT[0];
        self->boundsB          = Zone->cameraBoundsB[0];
        Zone->cameraBoundsL[0] = (self->position.x >> 16) - ScreenInfo->center.x;
        Zone->cameraBoundsR[0] = (self->position.x >> 16) + ScreenInfo->center.x;
        Zone->cameraBoundsT[0] = (self->position.y >> 16) - ScreenInfo->center.y;
        Zone->cameraBoundsB[0] = (self->position.y >> 16) + ScreenInfo->center.y;
        EntityCamera *camera   = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->boundsL        = Zone->cameraBoundsL[0];
        camera->boundsR        = Zone->cameraBoundsR[0];
        camera->boundsT        = Zone->cameraBoundsT[0];
        camera->boundsB        = Zone->cameraBoundsB[0];
        Smog->forceEnabled     = true;
    }

    if (host->timer == 30) {
        ActClear->displayedActID = 1;

        foreach_active(SignPost, signpost)
        {
            signpost->state  = SignPost_State_Falling;
            signpost->active = ACTIVE_NORMAL;
        }

        RSDK.PlaySfx(SignPost->sfxTwinkle, false, 0xFF);
        return true;
    }
    return false;
}

bool32 OOZ1Outro_Cutscene_PostActClearSetup(EntityCutsceneSeq *host)
{
    RSDK_THIS(OOZ1Outro);

    if (host->fadeWhite > 0)
        host->fadeWhite -= 4;

    CutsceneSeq_LockAllPlayerControl();

    if (ActClear->finished) {
        foreach_active(Player, player)
        {
            player->state          = Player_State_Air;
            player->onGround       = false;
            player->collisionPlane = 1;
            player->interaction    = false;
            player->velocity.y     = -0x20000;
            player->stateInput     = StateMachine_None;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_HURT, &player->animator, false, 0);
        }

        Zone->cameraBoundsL[0] = self->boundsL;
        Zone->cameraBoundsR[0] = self->boundsR;
        Zone->cameraBoundsT[0] = self->boundsT;
        Zone->cameraBoundsB[0] = self->boundsB;

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif
        return true;
    }

    return false;
}

bool32 OOZ1Outro_Cutscene_FallIntoAct2(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (host->timer == 48) {
        player1->interaction = true;
        if (player2->classID == Player->classID)
            player2->interaction = true;
    }

    if (Smog->forceEnabled && player1->animator.animationID)
        return false;

    player1->direction  = FLIP_NONE;
    player1->stateInput = Player_Input_P1;

    if (player2->classID == Player->classID)
        player2->stateInput = Player_Input_P2_AI;

    return true;
}

bool32 OOZ1Outro_Cutscene_BeginAct(EntityCutsceneSeq *host)
{
    if (host->timer == 120) {
        globals->suppressTitlecard = true;

        foreach_all(TitleCard, titlecard)
        {
            titlecard->active    = ACTIVE_NORMAL;
            titlecard->state     = TitleCard_State_SetupBGElements;
            titlecard->stateDraw = TitleCard_Draw_SlideIn;
            foreach_break;
        }

        Music_PlayTrack(TRACK_STAGE);
        return true;
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void OOZ1Outro_EditorDraw(void)
{
    RSDK_THIS(OOZ1Outro);

    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void OOZ1Outro_EditorLoad(void) {}
#endif

void OOZ1Outro_Serialize(void) { RSDK_EDITABLE_VAR(OOZ1Outro, VAR_VECTOR2, size); }
