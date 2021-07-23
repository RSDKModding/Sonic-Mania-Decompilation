#include "SonicMania.h"

ObjectSSZ3Cutscene *SSZ3Cutscene;

void SSZ3Cutscene_Update(void)
{
    void *states[]  = { SSZ3Cutscene_CutsceneState_Unknown1, SSZ3Cutscene_CutsceneState_Unknown2, SSZ3Cutscene_CutsceneState_Unknown4,
                       SSZ3Cutscene_CutsceneState_Unknown5, SSZ3Cutscene_CutsceneState_Unknown6, SSZ3Cutscene_CutsceneState_Unknown7,
                       SSZ3Cutscene_CutsceneState_Unknown8, SSZ3Cutscene_CutsceneState_Unknown9, NULL };
    void *states2[] = { SSZ3Cutscene_CutsceneState_Unknown4,
                        SSZ3Cutscene_CutsceneState_Unknown5,
                        SSZ3Cutscene_CutsceneState_Unknown6,
                        SSZ3Cutscene_CutsceneState_Unknown7,
                        SSZ3Cutscene_CutsceneState_Unknown8,
                        SSZ3Cutscene_CutsceneState_Unknown9,
                        NULL };

    RSDK_THIS(SSZ3Cutscene);
    if (!entity->flag) {
        CutsceneSeq_StartSequence((Entity *)entity, states);
        entity->active = ACTIVE_NEVER;
    }
    else {
        CutsceneSeq_StartSequence((Entity *)entity, states2);
        foreach_active(HUD, hud) { hud->state = HUD_State_GoOffScreen; }
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
        entity->active = ACTIVE_NEVER;
    }
}

void SSZ3Cutscene_LateUpdate(void) {}

void SSZ3Cutscene_StaticUpdate(void) {}

void SSZ3Cutscene_Draw(void) {}

void SSZ3Cutscene_Create(void *data)
{
    RSDK_THIS(SSZ3Cutscene);
    entity->active = ACTIVE_NORMAL;
    entity->flag   = voidToInt(data);
}

void SSZ3Cutscene_StageLoad(void) {}

void SSZ3Cutscene_Unknown(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);

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

bool32 SSZ3Cutscene_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    if (!host->timer) {
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
bool32 SSZ3Cutscene_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    if (host->timer >= 40) {
        foreach_active(Player, player)
        {
            if (!player->sidekick) {
                player->stateInput = Player_ProcessP1Input;
            }
        }
        return true;
    }
    return false;
}
bool32 SSZ3Cutscene_CutsceneState_Unknown4(EntityCutsceneSeq *host)
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

    FXRuby_Unknown1();

    int pos  = ((RSDK_screens->width + RSDK_screens->position.x) >> 4) + 2;
    int pos2 = 0;
    if (pos < 220) {
        pos2                    = ((RSDK_screens->width + RSDK_screens->position.x) >> 4) + 2;
        Zone->screenBoundsR1[0] = 16 * (((RSDK_screens->width + RSDK_screens->position.x) >> 4) + 38);
    }
    else {
        pos                     = 0;
        Zone->screenBoundsR1[0] = 4672;
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
        ruby->state      = PhantomRuby_Unknown5;
        ruby->startPos.x = (pos2 << 20) + 0x1D00000;
        ruby->startPos.y = 0x1A00000;
        ruby->position   = ruby->startPos;
        ruby->drawOrder  = Zone->drawOrderLow;
        entity->ruby     = ruby;
    }

    return true;
}
bool32 SSZ3Cutscene_CutsceneState_Unknown5(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);

    RSDK_THIS(SSZ3Cutscene);
    EntityPhantomRuby *ruby = entity->ruby;
    EntityPlayer *player    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    SSZ3Cutscene_Unknown();
    if (abs(player->position.x - ruby->position.x) < 0x900000) {
        foreach_active(Player, player) { player->right = false; }
        foreach_active(EggTower, tower) { destroyEntity(tower); }
        return true;
    }
    return false;
}
bool32 SSZ3Cutscene_CutsceneState_Unknown6(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    SSZ3Cutscene_Unknown();
    if (RSDK.GetEntityCount(PhantomRuby->objectID, true) > 0) {
        foreach_active(PhantomRuby, ruby) { entity->ruby = ruby; }
    }

    EntityPhantomRuby *ruby = entity->ruby;
    if (ruby) {
        if (ruby->state == PhantomRuby_Unknown5)
            return true;
    }
    return false;
}
bool32 SSZ3Cutscene_CutsceneState_Unknown7(EntityCutsceneSeq *host)
{
    RSDK_THIS(SSZ3Cutscene);

    EntityPhantomRuby *ruby = entity->ruby;
    SSZ3Cutscene_Unknown();
    if (!host->timer)
        PhantomRuby_Unknown2(ruby);

    if (host->timer == 8) {
        foreach_active(Player, player)
        {
            if (player->sidekick) {
                player->state = Player_State_None;
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SKID, &player->playerAnimator, false, 0);
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
                RSDK.SetSpriteAnimation(player->spriteIndex, ANI_IDLE, &player->playerAnimator, false, 0);
            }
        }
    }
    if (ruby->flag) {
        PhantomRuby_PlaySFX(RUBYSFX_REDCUBE);
        foreach_active(Player, player)
        {
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_IDLE, &player->playerAnimator, false, 0);
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
bool32 SSZ3Cutscene_CutsceneState_Unknown8(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    RSDK_THIS(SSZ3Cutscene);
    EntityPhantomRuby *ruby = entity->ruby;

    SSZ3Cutscene_Unknown();

    EntityFXRuby *fxRuby = NULL;
    if (host->timer) {
        fxRuby = entity->fxRuby;
    }
    else {
        fxRuby            = CREATE_ENTITY(FXRuby, NULL, ruby->position.x, ruby->position.y);
        fxRuby->drawOrder = Zone->playerDrawHigh;
        entity->fxRuby    = fxRuby;
        Camera_ShakeScreen(4, 0, 4);
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
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK4);
                    Camera_ShakeScreen(4, 0, 4);
                }
                else if (host->timer == host->field_68 + 180) {
                    fxRuby->field_74 = 32;
                    fxRuby->state    = FXRuby_Unknown6;
                    PhantomRuby_PlaySFX(RUBYSFX_ATTACK1);
                    Camera_ShakeScreen(4, 0, 4);
                    Music_FadeOut(0.025);
                    host->field_68    = host->timer;
                    host->field_6C[0] = 1;
                }
            }
            else {
                host->field_68 = host->timer;
            }

            if (host->timer >= host->field_68 + 52) {
                EntityPlayer *players[2];
                players[0] = player1;
                players[1] = player2;

                for (int i = 0, angle = 0; angle < 0x80; ++i, angle += 0x40) {
                    EntityPlayer *player = players[i];
                    if (!player)
                        break;

                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FAN, &player->playerAnimator, false, 0);
                    player->position.x +=
                        ((0xA00 * RSDK.Cos256(2 * (angle + host->timer - host->field_68)) + player->position.x) - player->position.x) >> 3;
                    player->position.y +=
                        (0xA00 * RSDK.Sin256(2 * (angle + host->timer - host->field_68)) + ruby->position.y - player->position.y) >> 3;
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
bool32 SSZ3Cutscene_CutsceneState_Unknown9(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);

    if (host->timer == 16) {
        RSDK.InitSceneLoad();
        return true;
    }
    return false;
}

void SSZ3Cutscene_EditorDraw(void) {}

void SSZ3Cutscene_EditorLoad(void) {}

void SSZ3Cutscene_Serialize(void) {}
