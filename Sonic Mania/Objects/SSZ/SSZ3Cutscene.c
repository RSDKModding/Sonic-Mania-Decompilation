// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZ3Cutscene Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectSSZ3Cutscene *SSZ3Cutscene;

void SSZ3Cutscene_Update(void)
{
    RSDK_THIS(SSZ3Cutscene);

    void *statesIntro[] = { SSZ3Cutscene_IntroState_Unknown1, SSZ3Cutscene_IntroState_Unknown2, NULL };
#if RETRO_USE_PLUS
    void *statesOutro[] = { SSZ3Cutscene_OutroState_Unknown1,
                        SSZ3Cutscene_OutroState_Unknown2,
                        SSZ3Cutscene_OutroState_Unknown3,
                        SSZ3Cutscene_OutroState_Unknown4,
                        SSZ3Cutscene_OutroState_Unknown5,
                        SSZ3Cutscene_OutroState_LoadNextScene,
                        NULL };

    if (!self->flag) {
        CutsceneSeq_StartSequence((Entity *)self, statesIntro);
        self->active = ACTIVE_NEVER;
    }
    else {
        CutsceneSeq_StartSequence((Entity *)self, statesOutro);
        foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
        self->active = ACTIVE_NEVER;
    }
#else
    CutsceneSeq_StartSequence((Entity *)self, statesIntro);
    self->active = ACTIVE_NEVER;
#endif
}

void SSZ3Cutscene_LateUpdate(void) {}

void SSZ3Cutscene_StaticUpdate(void) {}

void SSZ3Cutscene_Draw(void) {}

void SSZ3Cutscene_Create(void *data)
{
    RSDK_THIS(SSZ3Cutscene);
    self->active = ACTIVE_NORMAL;
#if RETRO_USE_PLUS
    self->flag   = voidToInt(data);
#endif
}

void SSZ3Cutscene_StageLoad(void) {}

#if RETRO_USE_PLUS
void SSZ3Cutscene_HandleRubyFX(void)
{
    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->position.y + (playerHitbox->bottom << 16) > 0x1E00000) {
            player->position.y = (480 - playerHitbox->bottom) << 16;
            player->velocity.y = 0;
            player->onGround   = true;
        }
    }
}
#endif

bool32 SSZ3Cutscene_IntroState_Unknown1(EntityCutsceneSeq *host)
{
#if RETRO_USE_PLUS
    if (!host->timer) {
#else
    if (host->timer >= 8) {
#endif
        foreach_active(Player, player)
        {
            player->groundVel  = 0x40000;
            player->velocity.x = 0x40000;
            if (!player->sidekick) {
                CutsceneSeq_LockAllPlayerControl();
                player->stateInput = StateMachine_None;
                player->left       = false;
                player->right      = true;
                player->up         = false;
                player->down       = false;
                player->jumpPress  = false;
            }
        }
    }
    if (host->timer >= 8) {
        globals->suppressAutoMusic = false;
        return true;
    }

    return false;
}
bool32 SSZ3Cutscene_IntroState_Unknown2(EntityCutsceneSeq *host)
{
    if (host->timer >= 40) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->stateInput = Player_ProcessP1Input;
        }
        return true;
    }
    return false;
}

#if RETRO_USE_PLUS
bool32 SSZ3Cutscene_OutroState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    foreach_active(Player, player)
    {
        player->state = Player_State_Ground;
        if (!player->sidekick) {
            player->stateInput = StateMachine_None;
            player->left       = false;
            player->right      = true;
            player->up         = false;
            player->down       = false;
            player->jumpPress  = false;
        }
    }

    FXRuby_SetupLayerDeformation();

    int32 pos  = ((ScreenInfo->width + ScreenInfo->position.x) >> 4) + 2;
    int32 pos2 = 0;
    if (pos < 220) {
        pos2                    = ((ScreenInfo->width + ScreenInfo->position.x) >> 4) + 2;
        Zone->cameraBoundsR[0] = 16 * (((ScreenInfo->width + ScreenInfo->position.x) >> 4) + 38);
    }
    else {
        pos                     = 0;
        Zone->cameraBoundsR[0] = 4672;
        pos2                    = 256;
    }
    Zone->playerBoundActiveR[0] = false;
    RSDK.CopyTileLayer(Zone->fgLow, pos, 16, Zone->fgLow, 187, 160, 36, 14);

#if RETRO_USE_PLUS 
    CREATE_ENTITY(MonarchPlans, NULL, (pos2 << 20) + 0xF00000, 0x1A80000);
    CREATE_ENTITY(SSZEggman, intToVoid(1), (pos2 << 20) + 0x2000000, 0x1C50000);
#endif

    foreach_all(PhantomRuby, ruby)
    {
        ruby->state      = PhantomRuby_State_Oscillate;
        ruby->startPos.x = (pos2 << 20) + 0x1D00000;
        ruby->startPos.y = 0x1A00000;
        ruby->position   = ruby->startPos;
        ruby->drawOrder  = Zone->drawOrderLow;
        self->ruby     = ruby;
    }

    return true;
}
bool32 SSZ3Cutscene_OutroState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);
    EntityPhantomRuby *ruby = self->ruby;
    EntityPlayer *player    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    SSZ3Cutscene_HandleRubyFX();
    if (abs(player->position.x - ruby->position.x) < 0x900000) {
        foreach_active(Player, player) { player->right = false; }
        foreach_active(EggTower, tower) { destroyEntity(tower); }
        return true;
    }
    return false;
}
bool32 SSZ3Cutscene_OutroState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    SSZ3Cutscene_HandleRubyFX();
    if (RSDK.GetEntityCount(PhantomRuby->objectID, true) > 0) {
        foreach_active(PhantomRuby, ruby) { self->ruby = ruby; }
    }

    EntityPhantomRuby *ruby = self->ruby;
    if (ruby) {
        if (ruby->state == PhantomRuby_State_Oscillate)
            return true;
    }
    return false;
}
bool32 SSZ3Cutscene_OutroState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    EntityPhantomRuby *ruby = self->ruby;
    SSZ3Cutscene_HandleRubyFX();
    if (!host->timer)
        PhantomRuby_SetupFlash(ruby);

    if (host->timer == 8) {
        foreach_active(Player, player)
        {
            if (player->sidekick) {
                player->state = Player_State_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_SKID, &player->animator, false, 0);
            }
            else {
                player->left = true;
            }
        }
    }
    else {
        foreach_active(Player, player)
        {
            if (player->state == Player_State_None) {
                player->groundVel -= player->groundVel >> 3;
                player->velocity.x -= player->velocity.x >> 3;
            }
            else if (player->direction == FLIP_X) {
                player->direction  = FLIP_NONE;
                player->left       = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->state      = Player_State_None;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
            }
        }
    }
    if (ruby->flashFinished) {
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
        foreach_active(Player, player)
        {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
            player->direction  = FLIP_NONE;
            player->left       = false;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;
            player->state      = Player_State_None;
        }
        return true;
    }

    return false;
}
bool32 SSZ3Cutscene_OutroState_Unknown5(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player1);
    unused(player2);
    unused(camera);
    
    RSDK_THIS(SSZ3Cutscene);
    EntityPhantomRuby *ruby = self->ruby;

    SSZ3Cutscene_HandleRubyFX();

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = self->fxRuby;
    }
    else {
        fxRuby            = CREATE_ENTITY(FXRuby, NULL, ruby->position.x, ruby->position.y);
        fxRuby->drawOrder = Zone->playerDrawHigh;
        self->fxRuby    = fxRuby;
        Camera_ShakeScreen(0, 4, 4);
        player1->drawOrder = Zone->playerDrawHigh + 1;
        if (player2->objectID == Player->objectID)
            player2->drawOrder = Zone->playerDrawHigh + 1;
    }

    if (!host->values[0]) {
        if (fxRuby->fullyExpanded) {
            if (host->storedValue2) {
                if (host->timer == host->storedValue2 + 48) {
                    fxRuby->delay = 64;
                    fxRuby->state    = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
                    Camera_ShakeScreen(0, 4, 4);
                }
                else if (host->timer == host->storedValue2 + 180) {
                    fxRuby->delay = 32;
                    fxRuby->state    = FXRuby_State_IncreaseStageDeform;
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
                    Camera_ShakeScreen(0, 4, 4);
                    Music_FadeOut(0.025);
                    host->storedValue2    = host->timer;
                    host->values[0] = 1;
                }
            }
            else {
                host->storedValue2 = host->timer;
            }

            if (host->timer >= host->storedValue2 + 52) {
                EntityPlayer *players[2];
                players[0] = player1;
                players[1] = player2;

                for (int32 i = 0, angle = 0; angle < 0x80; ++i, angle += 0x40) {
                    EntityPlayer *player = players[i];
                    if (!player)
                        break;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                    player->position.x +=
                        ((0xA00 * RSDK.Cos256(2 * (angle + host->timer - host->storedValue2)) + player->position.x) - player->position.x) >> 3;
                    player->position.y +=
                        (0xA00 * RSDK.Sin256(2 * (angle + host->timer - host->storedValue2)) + ruby->position.y - player->position.y) >> 3;
                    player->state          = Player_State_None;
                    player->tileCollisions = false;
                    player->onGround       = false;
                }
            }
        }
    }
    else {
        if (fxRuby->fadeWhite >= 512) {
            if (fxRuby->fadeBlack >= 512) {
                if (host->timer == host->storedValue2 + 150)
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
bool32 SSZ3Cutscene_OutroState_LoadNextScene(EntityCutsceneSeq *host)
{
    if (host->timer == 16) {
        RSDK.LoadScene();
        return true;
    }
    return false;
}
#endif

void SSZ3Cutscene_EditorDraw(void) {}

void SSZ3Cutscene_EditorLoad(void) {}

void SSZ3Cutscene_Serialize(void) {}
