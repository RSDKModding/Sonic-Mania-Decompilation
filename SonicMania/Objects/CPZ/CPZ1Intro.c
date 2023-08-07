// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CPZ1Intro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCPZ1Intro *CPZ1Intro;

void CPZ1Intro_Update(void)
{
    RSDK_THIS(CPZ1Intro);

    if (!self->activated) {
        if (!isMainGameMode() || !globals->enableIntro || CutsceneRules_CheckStageReload()) {
            destroyEntity(CPZ1Intro->fxRuby);
            self->active = ACTIVE_NEVER;
        }
        else {
            self->activated = true;
            CPZ1Intro_SetupCutscene();
        }
    }
}

void CPZ1Intro_LateUpdate(void) {}

void CPZ1Intro_StaticUpdate(void) {}

void CPZ1Intro_Draw(void) {}

void CPZ1Intro_Create(void *data)
{
    RSDK_THIS(CPZ1Intro);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NORMAL;
}

void CPZ1Intro_StageLoad(void)
{
    CPZ1Intro->playerFrames   = RSDK.LoadSpriteAnimation("Players/CutsceneCPZ.bin", SCOPE_STAGE);
    CPZ1Intro->particleFrames = RSDK.LoadSpriteAnimation("CPZ/Particles.bin", SCOPE_STAGE);

    foreach_all(FXRuby, fxRuby)
    {
        CPZ1Intro->fxRuby = fxRuby;
        foreach_break;
    }

    CPZ1Intro->sfxChemDrop = RSDK.GetSfx("CPZ/ChemDrop.wav");
    CPZ1Intro->sfxDNABurst = RSDK.GetSfx("CPZ/DNABurst.wav");
}

void CPZ1Intro_SetupCutscene(void)
{
    RSDK_THIS(CPZ1Intro);

    CutsceneSeq_StartSequence(self, CPZ1Intro_Cutscene_RubyWarp, CPZ1Intro_Cutscene_PostWarpDrop, CPZ1Intro_Cutscene_Waiting,
                              CPZ1Intro_Cutscene_ChemicalDrop, CPZ1Intro_Cutscene_PlayerChemicalReact, CPZ1Intro_Cutscene_ReadyStage,
                              StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif
}

void CPZ1Intro_Particle_ChemDrop(EntityDebris *debris)
{
    debris->velocity.y += RSDK.Rand(-0x28000, -0x20000);
    RSDK.SetSpriteAnimation(CPZ1Intro->particleFrames, 1, &debris->animator, true, 0);
}

void CPZ1Intro_HandleRubyHover(EntityCutsceneSeq *cutsceneSequence, EntityPlayer *player1, EntityPlayer *player2, int32 targetY)
{
    EntityPlayer *players[2];
    players[0] = player1;
    players[1] = player2;

    int32 id = 0;
    for (int32 angle = 0; angle < 0x80; angle += 0x40) {
        EntityPlayer *player = players[id++];
        if (!player)
            break;

        int32 velX = (player->position.x - player->position.x) >> 3;
        int32 velY = (targetY + 0xA00 * RSDK.Sin256(2 * (angle + cutsceneSequence->timer - cutsceneSequence->storedTimer)) - player->position.y) >> 3;
        player->position.x += velX;
        player->position.y += velY;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
        player->state = Player_State_Static;
    }
}

bool32 CPZ1Intro_CheckSonicAnimFinish(void)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 30;
}

bool32 CPZ1Intro_CheckTailsAnimFinish(int32 timer)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

#if MANIA_USE_PLUS
    if (player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4) {
        RSDK.SetSpriteAnimation(player1->tailFrames, ANI_IDLE, &player1->tailAnimator, true, 0);
        player1->animator.animationID     = 0;
        player1->animator.prevAnimationID = 0;
        return true;
    }
#else
    // Same Code as 2P AI, it's unknown why tails' anim was cut in pre-plus, but it's in plus so hooray
    if (timer == 60) {
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);
        player1->up = false;
        return true;
    }
#endif
    return false;
}

bool32 CPZ1Intro_CheckKnuxAnimFinish(void)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4;
}

#if MANIA_USE_PLUS
bool32 CPZ1Intro_CheckMightyAnimFinish(void)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4;
}

bool32 CPZ1Intro_CheckRayAnimFinish(void)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player2);
    UNUSED(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4;
}
#endif

bool32 CPZ1Intro_Cutscene_RubyWarp(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);

    EntityFXRuby *fxRuby = CPZ1Intro->fxRuby;
    if (!host->timer) {
        player1->camera = NULL;
        camera->position.y -= 0x40000;
        camera->target      = 0;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        player1->onGround   = false;
        if (player2->classID == Player->classID) {
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
    }
    else {
        if (fxRuby->fadeWhite > 0) {
            fxRuby->fadeWhite -= 16;
        }
        else {
            if (!host->values[0]) {
                PhantomRuby_PlaySfx(RUBYSFX_ATTACK4);
                host->values[0] = 1;
                fxRuby->state   = FXRuby_State_Shrinking;
            }

            if (!fxRuby->outerRadius) {
                player1->state = Player_State_Air;
                if (player2->classID == Player->classID)
                    player2->state = Player_State_Air;
                fxRuby->active = ACTIVE_NEVER;
                return true;
            }
        }
    }

    CPZ1Intro_HandleRubyHover(host, player1, player2, host->activeEntity->position.y - 0x200000);

    return false;
}

bool32 CPZ1Intro_Cutscene_PostWarpDrop(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(host);
    UNUSED(camera);

    if (player2->classID == Player->classID) {
        if (player1->onGround && player2->onGround)
            return true;
    }
    else {
        if (player1->onGround)
            return true;
    }

    return false;
}

bool32 CPZ1Intro_Cutscene_Waiting(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (!host->timer) {
        player1->state = Player_State_Static;
        if (player2->classID == Player->classID) {
            player2->state = Player_State_Static;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
        }
    }

    if (host->timer == 30) {
        if (CHECK_CHARACTER_ID(ID_SONIC, 1))
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BORED_2, &player1->animator, false, 0);
        else
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BORED_1, &player1->animator, false, 0);
    }

    return host->timer == 60;
}

bool32 CPZ1Intro_Cutscene_ChemicalDrop(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (!host->timer) {
        RSDK.PlaySfx(CPZ1Intro->sfxChemDrop, false, 255);
        EntityDebris *debris    = CREATE_ENTITY(Debris, NULL, player1->position.x + 0x20000, (ScreenInfo->position.y - 8) << 16);
        debris->updateRange.x   = 0x800000;
        debris->updateRange.y   = 0x800000;
        debris->active          = ACTIVE_NORMAL;
        debris->drawGroup       = Zone->playerDrawGroup[1];
        debris->state           = Debris_State_Fall;
        debris->gravityStrength = 0x3800;
        RSDK.SetSpriteAnimation(CPZ1Intro->particleFrames, 0, &debris->animator, true, 0);
        CPZ1Intro->debris = debris;
    }

    EntityDebris *debris = CPZ1Intro->debris;
    Hitbox *playerHitbox = Player_GetHitbox(player1);

    int32 playerY = player1->position.y + ((playerHitbox->top + 2) << 16);
    if (debris->position.y >= playerY) {
        RSDK.PlaySfx(CPZ1Intro->sfxDNABurst, false, 255);
        ParticleHelpers_SetupFallingParticles(debris->position.x, playerY, CPZ1Intro_Particle_ChemDrop);
        destroyEntity(debris);

        if (CHECK_CHARACTER_ID(ID_TAILS, 2))
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_SKID, &player2->animator, true, 0);

        return true;
    }

    return false;
}

bool32 CPZ1Intro_Cutscene_PlayerChemicalReact(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(camera);

    if (!host->timer) {
        switch (GET_CHARACTER_ID(1)) {
            case ID_SONIC:
                CPZ1Intro->playerAnimID = 0;
                player1->state          = Player_State_Static;
                player1->tileCollisions = TILECOLLISION_NONE;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                break;

            case ID_TAILS: CPZ1Intro->playerAnimID = 1;
#if MANIA_USE_PLUS
                player1->state          = Player_State_Static;
                player1->tileCollisions = TILECOLLISION_NONE;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                RSDK.SetSpriteAnimation(-1, 0, &player1->tailAnimator, true, 0);
                player1->animator.animationID     = 0xFF;
                player1->animator.prevAnimationID = 0xFF;
#else
                // Same Code as 2P AI, it's unknown why tails' anim was cut in pre-plus, but it's in plus so hooray
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_LOOK_UP, &player1->animator, true, 0);
                player1->up    = true;
                player1->state = Player_State_LookUp;
#endif

                break;
            case ID_KNUCKLES:
                CPZ1Intro->playerAnimID = 2;
                player1->state          = Player_State_Static;
                player1->tileCollisions = TILECOLLISION_NONE;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                break;

#if MANIA_USE_PLUS
            case ID_MIGHTY:
                CPZ1Intro->playerAnimID = 3;
                player1->state          = Player_State_Static;
                player1->tileCollisions = TILECOLLISION_NONE;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                break;

            case ID_RAY:
                CPZ1Intro->playerAnimID = 4;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                player1->state          = Player_State_Static;
                player1->tileCollisions = TILECOLLISION_NONE;
                break;
#endif
        }
    }

    if (host->timer == 60 && player2->classID == Player->classID) {
        RSDK.SetSpriteAnimation(player2->aniFrames, ANI_LOOK_UP, &player2->animator, true, 0);
        player2->up    = true;
        player2->state = Player_State_LookUp;
    }

    switch (CPZ1Intro->playerAnimID) {
        case 0: return CPZ1Intro_CheckSonicAnimFinish();
        case 1: return CPZ1Intro_CheckTailsAnimFinish(host->timer);
        case 2: return CPZ1Intro_CheckKnuxAnimFinish();
#if MANIA_USE_PLUS
        case 3: return CPZ1Intro_CheckMightyAnimFinish();
        case 4: return CPZ1Intro_CheckRayAnimFinish();
#endif
        default: break;
    }

    return false;
}

bool32 CPZ1Intro_Cutscene_ReadyStage(EntityCutsceneSeq *host)
{
    MANIA_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, true, 0);

        if (player2->classID == Player->classID) {
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
            player2->up = false;
        }
    }

    if (host->timer == 30) {
        player1->stateInput     = Player_Input_P1;
        player1->tileCollisions = TILECOLLISION_DOWN;
        player1->onGround       = true;
        player1->state          = Player_State_Ground;
        player1->camera         = camera;
        camera->target          = (Entity *)player1;
        camera->state           = Camera_State_FollowXY;
        if (player2->classID == Player->classID) {
            player2->stateInput     = Player_Input_P2_AI;
            player2->tileCollisions = TILECOLLISION_DOWN;
            player2->onGround       = true;
            player2->state          = Player_State_Ground;
        }

        foreach_all(TitleCard, titlecard)
        {
            titlecard->active    = ACTIVE_NORMAL;
            titlecard->state     = TitleCard_State_SetupBGElements;
            titlecard->stateDraw = TitleCard_Draw_SlideIn;
            Music_PlayTrack(TRACK_STAGE);
            foreach_break;
        }

        return true;
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void CPZ1Intro_EditorDraw(void)
{
    RSDK_THIS(CPZ1Intro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void CPZ1Intro_EditorLoad(void) {}
#endif

void CPZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(CPZ1Intro, VAR_VECTOR2, size); }
