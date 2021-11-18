#include "SonicMania.h"

ObjectPSZ2Outro *PSZ2Outro;

void PSZ2Outro_Update(void)
{
    RSDK_THIS(PSZ2Outro);
    void *states[] = { PSZ2Outro_CutsceneState_Unknown1,      PSZ2Outro_CutsceneState_Unknown2,
                       PSZ2Outro_CutsceneState_Unknown3,      PSZ2Outro_CutsceneState_Unknown4,
                       PSZ2Outro_CutsceneState_Unknown5,      PSZ2Outro_CutsceneState_Unknown6,
                       PSZ2Outro_CutsceneState_LoadNextScene, NULL };

    CutsceneSeq_StartSequence((Entity *)self, states);
    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
    foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
    self->active = ACTIVE_NEVER;
}

void PSZ2Outro_LateUpdate(void) {}

void PSZ2Outro_StaticUpdate(void) {}

void PSZ2Outro_Draw(void) {}

void PSZ2Outro_Create(void *data)
{
    RSDK_THIS(PSZ2Outro);
    if (!SceneInfo->inEditor) {
        foreach_all(PSZEggman, eggman) { self->eggman = (Entity *)eggman; }

        self->active  = ACTIVE_NORMAL;
        self->visible = false;
    }
}

void PSZ2Outro_StageLoad(void) {}

bool32 PSZ2Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPSZEggman *eggman = (EntityPSZEggman *)self->eggman;

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
    Camera_SetupLerp(3, 0, eggman->position.x - 0x600000, camera->position.y, 2);
    Music_PlayTrack(TRACK_STAGE);
    return true;
}

bool32 PSZ2Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPSZEggman *eggman = (EntityPSZEggman *)self->eggman;
    EntityCamera *camera    = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (host->timer == 180) {
        foreach_active(Player, player)
        {
            player->position.x = camera->position.x - (ScreenInfo->centerX << 16) - 0x400000;
            player->position.y = eggman->position.y;
            player->state      = Player_State_Ground;
            if (!player->sidekick)
                player->right = true;
        }
        return true;
    }
    return false;
}

bool32 PSZ2Outro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPSZEggman *eggman = (EntityPSZEggman *)self->eggman;
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
                    RSDK.SetSpriteAnimation(PSZEggman->aniFrames, 1, &eggman->animator, false, 0);
                    eggman->state    = PSZEggman_Unknown1;
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

bool32 PSZ2Outro_CutsceneState_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (RSDK.GetEntityCount(PhantomRuby->objectID, true) > 0) {
        foreach_active(PhantomRuby, ruby) { self->ruby = ruby; }
    }

    if (self->ruby) {
        if (self->ruby->state == PhantomRuby_Unknown5) {
            if (player2->objectID == Player->objectID) {
                player2->state = Player_State_None;
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_SKID, &player2->playerAnimator, false, 0);
            }
            return true;
        }
    }
    return false;
}

bool32 PSZ2Outro_CutsceneState_Unknown5(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPSZEggman *eggman = (EntityPSZEggman *)self->eggman;

    if (!host->timer)
        PhantomRuby_Unknown2(self->ruby);
    if (eggman->ruby) {
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
        return true;
    }
    return false;
}

bool32 PSZ2Outro_CutsceneState_Unknown6(EntityCutsceneSeq *host)
{
    RSDK_THIS(PSZ2Outro);
    EntityPSZEggman *eggman = (EntityPSZEggman *)self->eggman;
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = self->fxRuby;
    }
    else {
        fxRuby            = CREATE_ENTITY(FXRuby, NULL, eggman->position.x, eggman->position.y);
        fxRuby->drawOrder = Zone->playerDrawHigh;
        self->fxRuby    = fxRuby;
        Camera_ShakeScreen(0, 4, 4);
        player1->drawOrder = Zone->playerDrawHigh + 1;
        if (player2->objectID == Player->objectID)
            player2->drawOrder = Zone->playerDrawHigh + 1;
    }

    if (!host->field_6C[0]) {
        if (fxRuby->flag) {
            if (host->field_68) {
                if (host->timer == host->field_68 + 48) {
                    fxRuby->field_74 = 64;
                    fxRuby->state    = FXRuby_Unknown6;
                    PhantomRuby_PlaySFX(4);
                    Camera_ShakeScreen(0, 4, 4);
                }
                else if (host->timer == host->field_68 + 180) {
                    fxRuby->field_74 = 32;
                    fxRuby->state    = FXRuby_Unknown6;
                    PhantomRuby_PlaySFX(1);
                    Camera_ShakeScreen(0, 4, 4);
                    Music_FadeOut(0.025);
                    host->field_68    = host->timer;
                    host->field_6C[0] = 1;
                }
            }
            else {
                host->field_68 = host->timer;
            }
            if (host->timer >= host->field_68 + 52) {
                int32 id = 0;
                for (int32 angle = 0; angle < 0x80; angle += 0x40) {
                    EntityPlayer *player = RSDK_GET_ENTITY(id++, Player);
                    if (!player || player->objectID == TYPE_BLANK)
                        break;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_FAN, &player->playerAnimator, false, 0);

                    int32 valX = (player->position.x - player->position.x) >> 3;
                    int32 valY =
                        (0xA00 * RSDK.Sin256(2 * (angle + host->timer - host->field_68)) + (eggman->position.y - 0x200000) - player->position.y) >> 3;

                    player->position.x += valX;
                    player->position.y += valY;
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
                if (host->timer == host->field_68 + 150)
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

bool32 PSZ2Outro_CutsceneState_LoadNextScene(EntityCutsceneSeq *host)
{
    if (host->timer == 16) {
        RSDK.LoadScene();
        return true;
    }
    return false;
}

void PSZ2Outro_EditorDraw(void) {}

void PSZ2Outro_EditorLoad(void) {}

void PSZ2Outro_Serialize(void) { RSDK_EDITABLE_VAR(PSZ2Outro, VAR_VECTOR2, size); }
