#include "SonicMania.h"

ObjectCPZ1Intro *CPZ1Intro;

void CPZ1Intro_Update(void)
{
    void *states[] = {
        CPZ1Intro_Unknown5, CPZ1Intro_Unknown6, CPZ1Intro_Unknown7, CPZ1Intro_Unknown8, CPZ1Intro_Unknown9, CPZ1Intro_Unknown15, NULL
    };

    RSDK_THIS(CPZ1Intro);
    if (!entity->activated) {
        if (!isMainGameMode() || !globals->enableIntro || PlayerHelpers_CheckStageReload()) {
            destroyEntity(CPZ1Intro->fxRuby);
            entity->active = ACTIVE_NEVER;
        }
        else {
            entity->activated = true;
            CutsceneSeq_StartSequence((Entity *)entity, states);
            EntityCutsceneSeq *seq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
            if (seq->objectID)
                seq->skipType = SKIPTYPE_RELOADSCN;
        }
    }
}

void CPZ1Intro_LateUpdate(void) {}

void CPZ1Intro_StaticUpdate(void) {}

void CPZ1Intro_Draw(void) {}

void CPZ1Intro_Create(void *data)
{
    RSDK_THIS(CPZ1Intro);

    INIT_ENTITY(entity);
    CutsceneRules_SetupEntity(entity, &entity->size, &entity->hitbox);
    entity->active        = ACTIVE_NORMAL;
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
    CPZ1Intro->sfxChemDrop = RSDK.GetSFX("CPZ/ChemDrop.wav");
    CPZ1Intro->sfxDNABurst = RSDK.GetSFX("CPZ/DNABurst.wav");
}

void CPZ1Intro_Particle_CB(Entity *d)
{
    EntityDebris *debris = (EntityDebris *)d;
    debris->velocity.y += RSDK.Rand(-0x28000, -0x20000);
    RSDK.SetSpriteAnimation(CPZ1Intro->particleSpriteIndex, 1, &debris->animator, true, 0);
}

void CPZ1Intro_Unknown3(Entity *player1, Entity *cutSeq, Entity *player2, int32 val)
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
        int32 valY = (val + 0xA00 * RSDK.Sin256(2 * (angle + seq->timer - seq->field_68)) - player->position.y) >> 3;
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
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

    return player1->playerAnimator.frameID == player1->playerAnimator.frameCount - 1 && player1->playerAnimator.animationTimer == 30;
}

bool32 CPZ1Intro_CheckTailsAnimFinish(int32 timer)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

#if RETRO_USE_PLUS
    if (player1->playerAnimator.frameID == player1->playerAnimator.frameCount - 1 && player1->playerAnimator.animationTimer == 4) {
        RSDK.SetSpriteAnimation(player1->tailSpriteIndex, ANI_IDLE, &player1->tailAnimator, true, 0);
        player1->playerAnimator.animationID     = 0;
        player1->playerAnimator.prevAnimationID = 0;
        return true;
    }
#else
    if (timer == 60) {
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);
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

    return player1->playerAnimator.frameID == player1->playerAnimator.frameCount - 1 && player1->playerAnimator.animationTimer == 4;
}

#if RETRO_USE_PLUS
bool32 CPZ1Intro_CheckMightyAnimFinish(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    return player1->playerAnimator.frameID == player1->playerAnimator.frameCount - 1 && player1->playerAnimator.animationTimer == 4;
}

bool32 CPZ1Intro_CheckRayAnimFinish(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player2);
    unused(camera);

    return player1->playerAnimator.frameID == player1->playerAnimator.frameCount - 1 && player1->playerAnimator.animationTimer == 4;
}
#endif

bool32 CPZ1Intro_Unknown5(void *h)
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
        CPZ1Intro_Unknown3((Entity *)player1, (Entity *)host, (Entity *)player2, ent->position.y - 0x200000);
    }
    else {
        if (fxRuby->fadeWhite > 0) {
            fxRuby->fadeWhite -= 16;
            CPZ1Intro_Unknown3((Entity *)player1, (Entity *)host, (Entity *)player2, ent->position.y - 0x200000);
        }
        else {
            if (!host->field_6C[0]) {
                PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
                host->field_6C[0] = 1;
                fxRuby->state     = FXRuby_Unknown4;
            }

            if (fxRuby->outerRadius) {
                CPZ1Intro_Unknown3((Entity *)player1, (Entity *)host, (Entity *)player2, ent->position.y - 0x200000);
            }
            else {
                player1->state = Player_State_Air;
                if (player2->objectID == Player->objectID)
                    player2->state = Player_State_Air;
                fxRuby->active = ACTIVE_NEVER;
                return true;
            }
        }
    }

    return false;
}

bool32 CPZ1Intro_Unknown6(void *h)
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

bool32 CPZ1Intro_Unknown7(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    if (!host->timer) {
        player1->state = Player_State_None;
        if (player2->objectID == Player->objectID) {
            player2->state = Player_State_None;
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
        }
    }

    if (host->timer == 30) {
        if ((globals->playerID & 0xFF) == ID_SONIC)
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_BORED2, &player1->playerAnimator, false, 0);
        else
            RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_BORED1, &player1->playerAnimator, false, 0);
    }
    return host->timer == 60;
}

bool32 CPZ1Intro_Unknown8(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);


    if (!host->timer) {
        RSDK.PlaySfx(CPZ1Intro->sfxChemDrop, 0, 255);
        EntityDebris *debris =
            (EntityDebris *)RSDK.CreateEntity(Debris->objectID, 0, player1->position.x + 0x20000, (RSDK_screens->position.y - 8) << 16);
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
        RSDK.PlaySfx(CPZ1Intro->sfxDNABurst, 0, 255);
        ParticleHelpers_Unknown1(debris->position.x, playerY);
        destroyEntity(debris);
        if ((globals->playerID & 0xFFFFFF00) == ID_TAILS_ASSIST)
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_SKID, &player2->playerAnimator, true, 0);
        return true;
    }

    return false;
}

bool32 CPZ1Intro_Unknown9(void *h)
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
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->playerAnimator, true, 0);
                break;
            case ID_TAILS: 
                CPZ1Intro->playerAnimID = 1;
#if RETRO_USE_PLUS
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->playerAnimator, true, 0);
                RSDK.SetSpriteAnimation(0xFFFF, 0, &player1->tailAnimator, true, 0);
                player1->playerAnimator.animationID     = 0xFF;
                player1->playerAnimator.prevAnimationID = 0xFF;
#else
                RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_LOOKUP, &player1->playerAnimator, true, 0);
                player1->up    = true;
                player1->state = Player_State_LookUp;
#endif
                break;
            case ID_KNUCKLES:
                CPZ1Intro->playerAnimID = 2;
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->playerAnimator, true, 0);
                break;
#if RETRO_USE_PLUS
            case ID_MIGHTY:
                CPZ1Intro->playerAnimID = 3;
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->playerAnimator, true, 0);
                break;
            case ID_RAY:
                CPZ1Intro->playerAnimID = 4;
                RSDK.SetSpriteAnimation(CPZ1Intro->playerFrames, CPZ1Intro->playerAnimID, &player1->playerAnimator, true, 0);
                player1->state          = Player_State_None;
                player1->tileCollisions = false;
                break;
#endif
        }
    }

    if (host->timer == 60 && player2->objectID == Player->objectID) {
        RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_LOOKUP, &player2->playerAnimator, true, 0);
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

bool32 CPZ1Intro_Unknown15(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;
    RSDK_GET_PLAYER(player1, player2, camera);

    if (!host->timer) {
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_IDLE, &player1->playerAnimator, true, 0);
        if (player2->objectID == Player->objectID) {
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_IDLE, &player2->playerAnimator, true, 0);
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
            titlecard->state     = TitleCard_Unknown6;
            titlecard->stateDraw = TitleCard_StateDraw_Default;
            Music_PlayTrack(TRACK_STAGE);
            foreach_break;
        }
        return true;
    }

    return false;
}

void CPZ1Intro_EditorDraw(void)
{
    RSDK_THIS(CPZ1Intro);
    CutsceneRules_DrawCutsceneBounds(entity, &entity->size);
}

void CPZ1Intro_EditorLoad(void) {}

void CPZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(CPZ1Intro, VAR_VECTOR2, size); }
