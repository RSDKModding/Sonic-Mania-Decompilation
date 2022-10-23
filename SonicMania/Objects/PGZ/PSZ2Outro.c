// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZ2Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPSZ2Outro *PSZ2Outro;

void PSZ2Outro_Update(void)
{
    RSDK_THIS(PSZ2Outro);

    CutsceneSeq_StartSequence(self, PSZ2Outro_Cutscene_SetupCameraMove, PSZ2Outro_Cutscene_HandleCameraMovement, PSZ2Outro_Cutscene_WalkIntoPlace,
                              PSZ2Outro_Cutscene_EnterRuby, PSZ2Outro_Cutscene_RubyActivated, PSZ2Outro_Cutscene_RubyWarp,
                              PSZ2Outro_Cutscene_LoadSSZ1, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN, StateMachine_None);
#endif

    HUD_MoveOut();
    self->active = ACTIVE_NEVER;
}

void PSZ2Outro_LateUpdate(void) {}

void PSZ2Outro_StaticUpdate(void) {}

void PSZ2Outro_Draw(void) {}

void PSZ2Outro_Create(void *data)
{
    RSDK_THIS(PSZ2Outro);

    if (!SceneInfo->inEditor) {
        foreach_all(PSZEggman, eggman) { self->eggman = eggman; }

        self->active  = ACTIVE_NORMAL;
        self->visible = false;
    }
}

void PSZ2Outro_StageLoad(void) {}

bool32 PSZ2Outro_Cutscene_SetupCameraMove(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);

    EntityPSZEggman *eggman = self->eggman;

    foreach_active(Player, player)
    {
        if (!player->sidekick) {
            player->stateInput = StateMachine_None;
            CutsceneSeq_LockAllPlayerControl();
            player->left      = false;
            player->right     = false;
            player->up        = false;
            player->down      = false;
            player->jumpPress = false;
        }
    }

    EntityCamera *camera = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
    camera->position.x   = (camera->boundsR + camera->boundsL) << 15;
    camera->boundsR += 512;
    Zone->playerBoundActiveR[0] = false;
    Zone->playerBoundActiveR[1] = false;

    FXRuby_SetupLayerDeformation();
    Camera_SetupLerp(CAMERA_LERP_SIN512, 0, eggman->position.x - 0x600000, camera->position.y, 2);
    Music_PlayTrack(TRACK_STAGE);

    return true;
}

bool32 PSZ2Outro_Cutscene_HandleCameraMovement(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);

    EntityPSZEggman *eggman = self->eggman;
    EntityCamera *camera    = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (host->timer == 180) {
        foreach_active(Player, player)
        {
            player->position.x = camera->position.x - (ScreenInfo->center.x << 16) - 0x400000;
            player->position.y = eggman->position.y;
            player->state      = Player_State_Ground;

            if (!player->sidekick)
                player->right = true;
        }

        return true;
    }

    return false;
}

bool32 PSZ2Outro_Cutscene_WalkIntoPlace(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);

    EntityPSZEggman *eggman = self->eggman;
    EntityCamera *camera    = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    foreach_active(Player, player)
    {
        player->direction = FLIP_NONE;

        if (player->groundVel > 0x30000)
            player->groundVel = 0x30000;

        if (!player->sidekick) {
            if (camera->position.x - player->position.x >= 0x600000) {
                player->left  = false;
                player->right = true;
            }
            else {
                if (!player->skidding) {
                    RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 1, &eggman->eggmanAnimator, false, 0);
                    eggman->state    = PSZEggman_State_TurnRound;
                    player->skidding = 24;
                }

                player->right = false;
            }
        }
    }

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    if (player1->skidding == 4) {
        foreach_active(Player, playerPtr)
        {
            playerPtr->groundVel = 0;
            playerPtr->skidding  = false;
        }

        return true;
    }

    return false;
}

bool32 PSZ2Outro_Cutscene_EnterRuby(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (RSDK.GetEntityCount(PhantomRuby->classID, true) > 0) {
        foreach_active(PhantomRuby, ruby) { self->ruby = ruby; }
    }

    if (self->ruby && self->ruby->state == PhantomRuby_State_Oscillate) {
        if (player2->classID == Player->classID) {
            player2->state = Player_State_Static;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_SKID, &player2->animator, false, 0);
        }

        return true;
    }

    return false;
}

bool32 PSZ2Outro_Cutscene_RubyActivated(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);

    EntityPSZEggman *eggman = self->eggman;

    if (!host->timer)
        PhantomRuby_SetupFlash(self->ruby);

    if (eggman->ruby) {
        PhantomRuby_PlaySfx(RUBYSFX_REDCUBE);
        return true;
    }

    return false;
}

bool32 PSZ2Outro_Cutscene_RubyWarp(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);                                                                                       \
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);                                                                                       \

    EntityPSZEggman *eggman = self->eggman;
    EntityFXRuby *fxRuby    = NULL;
    if (host->timer) {
        fxRuby = self->fxRuby;
    }
    else {
        fxRuby            = CREATE_ENTITY(FXRuby, NULL, eggman->position.x, eggman->position.y);
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
                    host->values[0]   = true;
                }
            }
            else {
                host->storedTimer = host->timer;
            }

            if (host->timer >= host->storedTimer + 52) {
                EntityPlayer *players[2] = {player1, player2};
                for (int32 i = 0; i < 2; ++i) {
                    EntityPlayer *player = players[i];
                    if (player->classID == Player->classID) {
                        RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->animator, false, 0);

                        int32 valX = (player->position.x - player->position.x) >> 3;
                        int32 valY =
                            (0xA00 * RSDK.Sin256(2 * ((i * 0x40) + host->timer - host->storedTimer)) + (eggman->position.y - 0x200000) - player->position.y)
                            >> 3;

                        player->position.x += valX;
                        player->position.y += valY;
                        player->state          = Player_State_Static;
                        player->tileCollisions = TILECOLLISION_NONE;
                        player->onGround       = false;
                    }
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

bool32 PSZ2Outro_Cutscene_LoadSSZ1(EntityCutsceneSeq *host)
{
    if (host->timer == 16) {
        RSDK.LoadScene();
        return true;
    }

    return false;
}

#if RETRO_INCLUDE_EDITOR
void PSZ2Outro_EditorDraw(void)
{
    RSDK_THIS(PSZ2Outro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void PSZ2Outro_EditorLoad(void) {}
#endif

void PSZ2Outro_Serialize(void) { RSDK_EDITABLE_VAR(PSZ2Outro, VAR_VECTOR2, size); }
