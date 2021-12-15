// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: CPZ1Intro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectCPZ1Intro *CPZ1Intro;

void CPZ1Intro_Update(void)
{
    void *states[] = {
        CPZ1Intro_Cutscene_RubyWarp, CPZ1Intro_Cutscene_PostWarpDrop, CPZ1Intro_Cutscene_Waiting, CPZ1Intro_Cutscene_ChemicalDrop, CPZ1Intro_Cutscene_PlayerChemicalReact, CPZ1Intro_Cutscene_ReadyStage, NULL
    };

    RSDK_THIS(CPZ1Intro);
    if (!self->activated) {
        if (!isMainGameMode() || !globals->enableIntro || PlayerHelpers_CheckStageReload()) {
            destroyEntity(CPZ1Intro->fxRuby);
            self->active = ACTIVE_NEVER;
        }
        else {
            self->activated = true;
            CutsceneSeq_StartSequence((Entity *)self, states);
#if RETRO_USE_PLUS
            EntityCutsceneSeq *seq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
            if (seq->objectID)
                seq->skipType = SKIPTYPE_RELOADSCN;
#endif
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
    self->active        = ACTIVE_NORMAL;
}

void CPZ1Intro_StageLoad(void)
{
    CPZ1Intro->playerFrames        = RSDK.LoadSpriteAnimation("Players/CutsceneCPZ.bin", SCOPE_STAGE);
    CPZ1Intro->particleSpriteIndex = RSDK.LoadSpriteAnimation("CPZ/Particles.bin", SCOPE_STAGE);
    foreach_all(FXRuby, fxRuby)
    {
        CPZ1Intro->fxRuby = (Entity *)fxRuby;
        foreach_break;
    }
    CPZ1Intro->sfxChemDrop = RSDK.GetSfx("CPZ/ChemDrop.wav");
    CPZ1Intro->sfxDNABurst = RSDK.GetSfx("CPZ/DNABurst.wav");
}

void CPZ1Intro_Particle_CB(Entity *d)
{
    EntityDebris *debris = (EntityDebris *)d;
    debris->velocity.y += RSDK.Rand(-0x28000, -0x20000);
    RSDK.SetSpriteAnimation(CPZ1Intro->particleSpriteIndex, 1, &debris->animator, true, 0);
}

void CPZ1Intro_HandleRubyHover(Entity *player1, Entity *cutSeq, Entity *player2, int32 targetY)
{
    EntityPlayer *players[2];
    players[0]             = (EntityPlayer *)player1;
    players[1]             = (EntityPlayer *)player2;
    EntityCutsceneSeq *seq = (EntityCutsceneSeq *)cutSeq;

    int32 id = 0;
    for (int32 angle = 0; angle < 0x80; angle += 0x40) {
        EntityPlayer *player = players[id++];
        if (!player)
            break;

        int32 valX = (player->position.x - player->position.x) >> 3;
        int32 valY = (targetY + 0xA00 * RSDK.Sin256(2 * (angle + seq->timer - seq->storedValue2)) - player->position.y) >> 3;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
        player->position.x += valX;
        player->position.y += valY;
        player->state = Player_State_None;
    }
}

bool32 CPZ1Intro_CheckSonicAnimFinish(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 30;
}

bool32 CPZ1Intro_CheckTailsAnimFinish(int32 timer)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

#if RETRO_USE_PLUS
    if (player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4) {
        RSDK.SetSpriteAnimation(player1->tailSpriteIndex, ANI_IDLE, &player1->tailAnimator, true, 0);
        player1->animator.animationID     = 0;
        player1->animator.prevAnimationID = 0;
        return true;
    }
#else
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
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4;
}

#if RETRO_USE_PLUS
bool32 CPZ1Intro_CheckMightyAnimFinish(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4;
}

bool32 CPZ1Intro_CheckRayAnimFinish(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    return player1->animator.frameID == player1->animator.frameCount - 1 && player1->animator.timer == 4;
}
#endif

bool32 CPZ1Intro_Cutscene_RubyWarp(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);

    Entity *ent          = host->cutsceneCurEntity;
    EntityFXRuby *fxRuby = (EntityFXRuby *)CPZ1Intro->fxRuby;
    if (!host->timer) {
        player1->camera = NULL;
        camera->position.y -= 0x40000;
        camera->targetPtr   = 0;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
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
    }
    else {
        if (fxRuby->fadeWhite > 0) {
            fxRuby->fadeWhite -= 16;
        }
        else {
            if (!host->values[0]) {
                PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
                host->values[0] = 1;
                fxRuby->state     = FXRuby_State_ShrinkRing;
            }

            if (!fxRuby->outerRadius) {
                player1->state = Player_State_Air;
                if (player2->objectID == Player->objectID)
                    player2->state = Player_State_Air;
                fxRuby->active = ACTIVE_NEVER;
                return true;
            }
        }
    }
    CPZ1Intro_HandleRubyHover((Entity *)player1, (Entity *)host, (Entity *)player2, ent->position.y - 0x200000);

    return false;
}

bool32 CPZ1Intro_Cutscene_PostWarpDrop(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(host);
    unused(camera);


    if (player2->objectID == Player->objectID) {
        if (player1->onGround && player2->onGround)
            return true;
    }
    else {
        if (player1->onGround)
            return true;
    }
    return false;
}

bool32 CPZ1Intro_Cutscene_Waiting(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    if (!host->timer) {
        player1->state = Player_State_None;
        if (player2->objectID == Player->objectID) {
            player2->state = Player_State_None;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, true, 0);
        }
    }

    if (host->timer == 30) {
        if (checkPlayerID(ID_SONIC, 1))
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BORED2, &player1->animator, false, 0);
        else
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_BORED1, &player1->animator, false, 0);
    }
    return host->timer == 60;
}

bool32 CPZ1Intro_Cutscene_ChemicalDrop(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);


    if (!host->timer) {
        RSDK.PlaySfx(CPZ1Intro->sfxChemDrop, false, 255);
        EntityDebris *debris  = CREATE_ENTITY(Debris, NULL, player1->position.x + 0x20000, (ScreenInfo->position.y - 8) << 16);
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        debris->active        = ACTIVE_NORMAL;
        debris->drawOrder     = Zone->playerDrawHigh;
        debris->state         = Debris_State_Fall;
        debris->gravity       = 0x3800;
        RSDK.SetSpriteAnimation(CPZ1Intro->particleSpriteIndex, 0, &debris->animator, true, 0);
        CPZ1Intro->debris = (Entity *)debris;
    }

    EntityDebris *debris = (EntityDebris *)CPZ1Intro->debris;
    Hitbox *playerHitbox = Player_GetHitbox(player1);

    int32 playerY = player1->position.y + ((playerHitbox->top + 2) << 16);
    if (debris->position.y >= playerY) {
        RSDK.PlaySfx(CPZ1Intro->sfxDNABurst, false, 255);
        ParticleHelpers_SetupFallingParticles(debris->position.x, playerY);
        destroyEntity(debris);
        if (checkPlayerID(ID_TAILS, 2))
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_SKID, &player2->animator, true, 0);
        return true;
    }

    return false;
}

bool32 CPZ1Intro_Cutscene_PlayerChemicalReact(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    if (!host->timer) {
        switch (globals->playerID & 0xFF) {
            case ID_SONIC:
                CPZ1Intro->playerAnimID = 0;
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                break;
            case ID_TAILS: 
                CPZ1Intro->playerAnimID = 1;
#if RETRO_USE_PLUS
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &player1->tailAnimator, true, 0);
                player1->animator.animationID     = 0xFF;
                player1->animator.prevAnimationID = 0xFF;
#else
                RSDK.SetSpriteAnimation(player1->aniFrames, ANI_LOOKUP, &player1->animator, true, 0);
                player1->up    = true;
                player1->state = Player_State_LookUp;
#endif
                break;
            case ID_KNUCKLES:
                CPZ1Intro->playerAnimID = 2;
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                CPZ1Intro->playerAnimID = 3;
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                break;
            case ID_RAY:
                CPZ1Intro->playerAnimID = 4;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->animator, true, 0);
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                break;
#endif
        }
    }

    if (host->timer == 60 && player2->objectID == Player->objectID) {
        RSDK.SetSpriteAnimation(player2->aniFrames, ANI_LOOKUP, &player2->animator, true, 0);
        player2->up    = true;
        player2->state = Player_State_LookUp;
    }

    switch (CPZ1Intro->playerAnimID) {
        case 0: return CPZ1Intro_CheckSonicAnimFinish();
        case 1: return CPZ1Intro_CheckTailsAnimFinish(host->timer);
        case 2: return CPZ1Intro_CheckKnuxAnimFinish();
#if RETRO_USE_PLUS
        case 3: return CPZ1Intro_CheckMightyAnimFinish();
        case 4: return CPZ1Intro_CheckRayAnimFinish();
#endif
        default: break;
    }
    return false;
}

bool32 CPZ1Intro_Cutscene_ReadyStage(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
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

        foreach_all(TitleCard, titlecard)
        {
            titlecard->active    = ACTIVE_NORMAL;
            titlecard->state     = TitleCard_State_Initial;
            titlecard->stateDraw = TitleCard_Draw_Default;
            Music_PlayTrack(TRACK_STAGE);
            foreach_break;
        }
        return true;
    }

    return false;
}

#if RETRO_INCLUDE_EDITOR
void CPZ1Intro_EditorDraw(void)
{
    RSDK_THIS(CPZ1Intro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void CPZ1Intro_EditorLoad(void) {}
#endif

void CPZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(CPZ1Intro, VAR_VECTOR2, size); }
