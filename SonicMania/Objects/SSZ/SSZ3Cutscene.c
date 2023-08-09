// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: SSZ3Cutscene Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectSSZ3Cutscene *SSZ3Cutscene;

void SSZ3Cutscene_Update(void)
{
    RSDK_THIS(SSZ3Cutscene);

#if MANIA_USE_PLUS
    if (self->playTransformCutscene) {
        CutsceneSeq_StartSequence(self, SSZ3Cutscene_CutsceneOutro_SetupOutro, SSZ3Cutscene_CutsceneOutro_FollowRuby,
                                  SSZ3Cutscene_CutsceneOutro_EnterRuby, SSZ3Cutscene_CutsceneOutro_RubyActivate, SSZ3Cutscene_CutsceneOutro_RubyWarp,
                                  SSZ3Cutscene_CutsceneOutro_LoadHCZ1, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif

        HUD_MoveOut();

        self->active = ACTIVE_NEVER;
    }
    else {
#endif
        CutsceneSeq_StartSequence(self, SSZ3Cutscene_CutsceneIntro_EnterStageLeft, SSZ3Cutscene_CutsceneIntro_PlayerRunLeft, StateMachine_None);

#if MANIA_USE_PLUS
        CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

        self->active = ACTIVE_NEVER;
#if MANIA_USE_PLUS
    }
#endif
}

void SSZ3Cutscene_LateUpdate(void) {}

void SSZ3Cutscene_StaticUpdate(void) {}

void SSZ3Cutscene_Draw(void) {}

void SSZ3Cutscene_Create(void *data)
{
    RSDK_THIS(SSZ3Cutscene);

    self->active = ACTIVE_NORMAL;
#if MANIA_USE_PLUS
    self->playTransformCutscene = VOID_TO_INT(data);
#endif
}

void SSZ3Cutscene_StageLoad(void) {}

#if MANIA_USE_PLUS
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

bool32 SSZ3Cutscene_CutsceneIntro_EnterStageLeft(EntityCutsceneSeq *host)
{
#if MANIA_USE_PLUS
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
bool32 SSZ3Cutscene_CutsceneIntro_PlayerRunLeft(EntityCutsceneSeq *host)
{
    if (host->timer >= 40) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->stateInput = Player_Input_P1;
        }

        return true;
    }

    return false;
}

#if MANIA_USE_PLUS
bool32 SSZ3Cutscene_CutsceneOutro_SetupOutro(EntityCutsceneSeq *host)
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

    int32 screenBoundsR = ((ScreenInfo->position.x + ScreenInfo->size.x) >> 4) + 2;
    int32 cutscenePos   = 0;
    if (screenBoundsR < 220) {
        cutscenePos            = ((ScreenInfo->size.x + ScreenInfo->position.x) >> 4) + 2;
        Zone->cameraBoundsR[0] = 16 * (((ScreenInfo->size.x + ScreenInfo->position.x) >> 4) + 38);
    }
    else {
        screenBoundsR          = 0;
        Zone->cameraBoundsR[0] = 4672;
        cutscenePos            = 256;
    }

    Zone->playerBoundActiveR[0] = false;
    RSDK.CopyTileLayer(Zone->fgLayer[0], screenBoundsR, 16, Zone->fgLayer[0], 187, 160, 36, 14);

#if MANIA_USE_PLUS
    CREATE_ENTITY(MonarchPlans, NULL, (cutscenePos << 20) + 0xF00000, 0x1A80000);
    CREATE_ENTITY(SSZEggman, INT_TO_VOID(true), (cutscenePos << 20) + 0x2000000, 0x1C50000);
#endif

    foreach_all(PhantomRuby, ruby)
    {
        ruby->state      = PhantomRuby_State_Oscillate;
        ruby->startPos.x = (cutscenePos << 20) + 0x1D00000;
        ruby->startPos.y = 0x1A00000;
        ruby->position   = ruby->startPos;
        ruby->drawGroup  = Zone->objectDrawGroup[0];
        self->ruby       = ruby;
    }

    return true;
}
bool32 SSZ3Cutscene_CutsceneOutro_FollowRuby(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    EntityPhantomRuby *ruby = self->ruby;
    EntityPlayer *player    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    SSZ3Cutscene_HandleRubyFX();

    if (abs(player->position.x - ruby->position.x) < 0x900000) {
        foreach_active(Player, playerPtr) { playerPtr->right = false; }

        foreach_active(EggTower, tower) { destroyEntity(tower); }

        return true;
    }

    return false;
}
bool32 SSZ3Cutscene_CutsceneOutro_EnterRuby(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    SSZ3Cutscene_HandleRubyFX();

    if (RSDK.GetEntityCount(PhantomRuby->classID, true) > 0) {
        foreach_active(PhantomRuby, ruby) { self->ruby = ruby; }
    }

    EntityPhantomRuby *ruby = self->ruby;
    if (ruby && ruby->state == PhantomRuby_State_Oscillate)
        return true;

    return false;
}
bool32 SSZ3Cutscene_CutsceneOutro_RubyActivate(EntityCutsceneSeq *host)
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
                player->state = Player_State_Static;
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
            if (player->state == Player_State_Static) {
                player->groundVel -= player->groundVel >> 3;
                player->velocity.x -= player->velocity.x >> 3;
            }
            else if (player->direction == FLIP_X) {
                player->direction  = FLIP_NONE;
                player->left       = false;
                player->velocity.x = 0;
                player->velocity.y = 0;
                player->groundVel  = 0;
                player->state      = Player_State_Static;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);
            }
        }
    }

    if (ruby->flashFinished) {
        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
        foreach_active(Player, player)
        {
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_IDLE, &player->animator, false, 0);

            player->direction  = FLIP_NONE;
            player->left       = false;
            player->velocity.x = 0;
            player->velocity.y = 0;
            player->groundVel  = 0;
            player->state      = Player_State_Static;
        }

        return true;
    }

    return false;
}
bool32 SSZ3Cutscene_CutsceneOutro_RubyWarp(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    // lmao
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player1);
    UNUSED(player2);
    UNUSED(camera);

    EntityPhantomRuby *ruby = self->ruby;

    SSZ3Cutscene_HandleRubyFX();

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
                    host->values[0]   = 1;
                }
            }
            else {
                host->storedTimer = host->timer;
            }

            if (host->timer >= host->storedTimer + 52) {
                EntityPlayer *players[2];
                players[0] = player1;
                players[1] = player2;

                for (int32 i = 0, angle = 0; angle < 0x80; ++i, angle += 0x40) {
                    EntityPlayer *player = players[i];
                    if (!player)
                        break;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);
                    player->position.x +=
                        ((0xA00 * RSDK.Cos256(2 * (angle + host->timer - host->storedTimer)) + player->position.x) - player->position.x) >> 3;
                    player->position.y +=
                        (0xA00 * RSDK.Sin256(2 * (angle + host->timer - host->storedTimer)) + ruby->position.y - player->position.y) >> 3;
                    player->state          = Player_State_Static;
                    player->tileCollisions = TILECOLLISION_NONE;
                    player->onGround       = false;
                }
            }
        }
    }
    else {
        if (fxRuby->fadeWhite >= 512) {
            if (fxRuby->fadeBlack >= 512) {
                if (host->timer == host->storedTimer + 150)
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
bool32 SSZ3Cutscene_CutsceneOutro_LoadHCZ1(EntityCutsceneSeq *host)
{
    if (host->timer == 16) {
        RSDK.LoadScene();

        return true;
    }

    return false;
}
#endif

#if GAME_INCLUDE_EDITOR
void SSZ3Cutscene_EditorDraw(void) {}

void SSZ3Cutscene_EditorLoad(void) {}
#endif

void SSZ3Cutscene_Serialize(void) {}
