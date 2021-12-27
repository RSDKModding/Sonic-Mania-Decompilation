// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZ1Intro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSSZ1Intro *SSZ1Intro;

void SSZ1Intro_Update(void)
{
    void *states[] = { SSZ1Intro_CutsceneState_Unknown1, SSZ1Intro_CutsceneState_Unknown2, SSZ1Intro_CutsceneState_Unknown3, NULL };

    RSDK_THIS(SSZ1Intro);
    if (!self->activated) {
        if (!isMainGameMode() || !globals->enableIntro || PlayerHelpers_CheckStageReload()) {
            destroyEntity(SSZ1Intro->fxRuby);
            self->active = ACTIVE_NEVER;
        }
        else {
            self->activated = true;
            CutsceneSeq_StartSequence((Entity *)self, states);
        }
    }
}

void SSZ1Intro_LateUpdate(void) {}

void SSZ1Intro_StaticUpdate(void) {}

void SSZ1Intro_Draw(void) {}

void SSZ1Intro_Create(void *data)
{
    RSDK_THIS(SSZ1Intro);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NORMAL;
}

void SSZ1Intro_StageLoad(void)
{
    foreach_all(FXRuby, ruby) { SSZ1Intro->fxRuby = ruby; }
}

void SSZ1Intro_Unknown1(EntityPlayer *player1, EntityCutsceneSeq *host, EntityPlayer *player2, int32 offset)
{
    EntityPlayer *players[2];
    players[0] = player1;
    players[1] = player2;

    for (int32 i = 0, angle = 0; angle < 0x80; angle += 0x40, ++i) {
        EntityPlayer *playerPtr = players[i];
        if (!playerPtr)
            break;
        RSDK.SetSpriteAnimation(playerPtr->aniFrames, ANI_FAN, &playerPtr->animator, false, 0);
        playerPtr->position.x += (playerPtr->position.x - playerPtr->position.x) >> 3;
        playerPtr->position.y += (offset + 0xA00 * RSDK.Sin256(2 * (angle + host->timer - host->storedValue2)) - playerPtr->position.y) >> 3;
        playerPtr->state = Player_State_None;
    }
}

bool32 SSZ1Intro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);

    Entity *cutEntity    = host->cutsceneCurEntity;
    EntityFXRuby *fxRuby = SSZ1Intro->fxRuby;
    if (!host->timer) {
        CutsceneSeq_LockAllPlayerControl();
        player1->camera = 0;
        camera->position.y -= 0x40000;
        camera->targetPtr   = 0;
        player1->stateInput = StateMachine_None;
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        player1->onGround   = false;
        if (player2->objectID == Player->objectID) {
            player2->position.x -= 0x80000;
            player2->velocity.x = 0;
            player2->velocity.y = 0;
            player2->onGround   = false;
            player2->stateInput = StateMachine_None;
        }
    }
    camera->state = StateMachine_None;

    if (fxRuby->fadeBlack > 0) {
        fxRuby->fadeBlack -= 16;
        SSZ1Intro_Unknown1(player1, host, player2, cutEntity->position.y - 0x200000);
    }
    else if (fxRuby->fadeWhite > 0) {
        fxRuby->fadeWhite -= 16;
        SSZ1Intro_Unknown1(player1, host, player2, cutEntity->position.y - 0x200000);
    }
    else {
        if (!host->values[0]) {
            PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
            host->values[0] = true;
            fxRuby->state     = FXRuby_State_ShrinkRing;
        }

        if (fxRuby->outerRadius) {
            SSZ1Intro_Unknown1(player1, host, player2, cutEntity->position.y - 0x200000);
        }
        else {
            player1->state = Player_State_Air;
            if (player2->objectID == Player->objectID)
                player2->state = Player_State_Air;
            fxRuby->active = ACTIVE_NEVER;
            return true;
        }
    }
    return false;
}
bool32 SSZ1Intro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    if (player2->objectID == Player->objectID) {
        if (player1 && player2->onGround)
            return true;
    }
    else if (player1->onGround) {
        return true;
    }
    return false;
}
bool32 SSZ1Intro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);
        if (player2->objectID == Player->objectID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            player2->up = false;
        }
    }
    if (host->timer == 30) {
        player1->stateInput     = Player_ProcessP1Input;
        player1->tileCollisions = true;
        player1->onGround       = true;
        player1->state          = Player_State_Ground;
        player1->camera         = camera;
        camera->targetPtr       = (Entity *)player1;
        camera->state           = Camera_State_Follow;
        if (player2->objectID == Player->objectID) {
            player2->stateInput     = Player_ProcessP2Input_AI;
            player2->tileCollisions = true;
            player2->onGround       = true;
            player2->state          = Player_State_Ground;
        }

        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_State_Initial;
            titleCard->stateDraw = TitleCard_Draw_SlideIn;
        }

        Music_PlayTrack(TRACK_STAGE);
        return true;
    }
    return false;
}

#if RETRO_INCLUDE_EDITOR
void SSZ1Intro_EditorDraw(void)
{
    RSDK_THIS(SSZ1Intro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void SSZ1Intro_EditorLoad(void) {}
#endif

void SSZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(SSZ1Intro, VAR_VECTOR2, size); }
