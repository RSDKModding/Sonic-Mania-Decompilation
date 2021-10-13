#include "SonicMania.h"

ObjectTTCutscene *TTCutscene;

void TTCutscene_Update(void)
{
    RSDK_THIS(TTCutscene);
    if (!entity->activated) {
        TTCutscene_StartCutscene();
        entity->activated = true;
    }
}

void TTCutscene_LateUpdate(void) {}

void TTCutscene_StaticUpdate(void) {}

void TTCutscene_Draw(void) {}

void TTCutscene_Create(void *data)
{
    RSDK_THIS(TTCutscene);

    entity->active  = ACTIVE_NORMAL;
    entity->visible = false;
    if (!entity->size.x)
        entity->size.x = 0x1A80000;
    if (!entity->size.y)
        entity->size.y = 0xF00000;

    entity->hitbox.left   = -entity->size.x >> 17;
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.top    = -entity->size.y >> 17;
    entity->hitbox.bottom = entity->size.y >> 17;
}

void TTCutscene_StageLoad(void)
{
    TTCutscene->fxFade = NULL;
    foreach_all(FXFade, fade)
    {
        TTCutscene->fxFade = fade;
        foreach_break;
    }
}

void TTCutscene_StartCutscene(void)
{
    void *cutsceneStates[6] = { TTCutscene_CutsceneState_Setup,  TTCutscene_CutsceneState_FlyIn,     TTCutscene_CutsceneState_Wait,
                                TTCutscene_CutsceneState_FlyOut, TTCutscene_CutsceneState_NextScene, NULL };

    RSDK_THIS(TTCutscene);
    CutsceneSeq_StartSequence((Entity *)entity, cutsceneStates);
    EntityCutsceneSeq *cutsceneSeq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);
    if (cutsceneSeq->objectID != TYPE_BLANK) {
        cutsceneSeq->skipType    = SKIPTYPE_CALLBACK;
        cutsceneSeq->skipCallback = TTCutscene_SkipCB;
    }
}

void TTCutscene_SkipCB(void)
{
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_ENCORE)
        RSDK.SetScene("Encore Mode", "Stardust Speedway Zone+ 1");
    else
#endif
        RSDK.SetScene("Mania Mode", "Stardust Speedway Zone 1");
    ++RSDK_sceneInfo->listPos;
}

bool32 TTCutscene_CutsceneState_Setup(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    if (!host->timer) {
        Zone->playerBoundActiveB[0] = 0;
        Zone->playerBoundActiveB[1] = 0;
        Zone->playerBoundActiveB[2] = 0;
        Zone->playerBoundActiveB[3] = 0;
        Zone->deathBoundary[0]      = 0x7FFF0000;
        Zone->deathBoundary[1]      = 0x7FFF0000;
        Zone->deathBoundary[2]      = 0x7FFF0000;
        Zone->deathBoundary[3]      = 0x7FFF0000;
        camera->targetPtr           = NULL;
        player1->camera             = NULL;
        player1->stateInput         = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        player1->state      = Player_State_None;
        player1->position.x = player1->position.x;
        player1->position.y = (RSDK_screens->position.y + 32 + RSDK_screens->height) << 16;
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_SPRINGTWIRL, &player1->playerAnimator, false, 0);

        if (player2->objectID == Player->objectID) {
            player1->position.x += 0x100000;
            player1->stateInput = StateMachine_None;
            player2->position.x = player1->position.x - 0x200000;
            player2->position.y = player1->position.y;
            player2->state      = Player_State_None;
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_SPRINGTWIRL, &player2->playerAnimator, false, 0);
        }
    }
    return host->timer == 64;
}

bool32 TTCutscene_CutsceneState_FlyIn(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    int32 scrY  = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
    int32 scrY2 = (RSDK_screens->position.y + RSDK_screens->height + 32) << 16;

    int32 timerP1 = host->timer;
    if (timerP1) {
        if (timerP1 >= 60) {
            player1->position.y = scrY;
        }
        else {
            Vector2 *pos = &player1->position;
            pos->y       = MathHelpers_Unknown5((timerP1 << 16) / 60, pos->x, scrY2, pos->x, scrY - 0x800000, pos->x, scrY, pos->x, scrY).y;
        }
    }
    else {
        player1->position.y = scrY2;
    }
    player1->velocity.x = 0;
    player1->velocity.y = 0;

    if (player2->objectID == Player->objectID) {
        int32 timerP2 = host->timer - 15;
        if (timerP2 > 0) {
            if (timerP2 >= 60) {
                player2->position.y = scrY;
            }
            else if (timerP2 < 60) {
                Vector2 *pos = &player2->position;
                pos->y       = MathHelpers_Unknown5((timerP2 << 16) / 60, pos->x, scrY2, pos->x, scrY - 0x800000, pos->x, scrY, pos->x, scrY).y;
            }
        }
        else {
            player2->position.y = scrY2;
        }
        player2->velocity.x = 0;
        player2->velocity.y = 0;
    }
    return host->timer == 75;
}

bool32 TTCutscene_CutsceneState_Wait(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    
    player1->position.y = (RSDK_screens->centerY + RSDK_screens->position.y) << 16;
    if (player2->objectID == Player->objectID)
        player2->position.y = (RSDK_screens->centerY + RSDK_screens->position.y) << 16;

    return host->timer == 100;
}

bool32 TTCutscene_CutsceneState_FlyOut(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);

    int32 scrY  = (RSDK_screens->position.y + RSDK_screens->centerY) << 16;
    int32 scrY2 = (RSDK_screens->position.y - 32) << 16;

    int32 timerP1 = host->timer;
    if (timerP1) {
        if (timerP1 >= 60) {
            player1->position.y = scrY2;
        }
        else {
            Vector2 *pos = &player1->position;
            pos->y       = MathHelpers_Unknown5((timerP1 << 16) / 60, pos->x, scrY, pos->x, scrY, pos->x, scrY + 0x800000, pos->x, scrY2).y;
        }
    }
    else {
        player1->position.y = scrY;
    }
    player1->velocity.x = 0;
    player1->velocity.y = 0;

    if (player2->objectID == Player->objectID) {
        int32 timerP2 = host->timer - 15;
        if (timerP2 > 0) {
            if (timerP2 >= 60) {
                player2->position.y = scrY2;
            }
            else {
                Vector2 *pos = &player2->position;
                pos->y       = MathHelpers_Unknown5((timerP2 << 16) / 60, pos->x, scrY, pos->x, scrY, pos->x, scrY + 0x800000, pos->x, scrY2).y;
            }
        }
        else {
            player2->position.y = scrY;
        }
        player2->velocity.x = 0;
        player2->velocity.y = 0;
    }

    if (timerP1 == 75)
        TTCutscene->fxFade->state = FXFade_State_FadeIn;
    return host->timer == 75;
}

bool32 TTCutscene_CutsceneState_NextScene(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player1);
    unused(player2);
    unused(camera);
    
    if (host->timer >= 64) {
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_ENCORE)
            RSDK.SetScene("Encore Mode", "Stardust Speedway Zone+ 1");
        else
#endif
            RSDK.SetScene("Mania Mode", "Stardust Speedway Zone 1");
        ++RSDK_sceneInfo->listPos;
        Zone_StartFadeOut(10, 0x000000);
        return true;
    }
    return false;
}

void TTCutscene_EditorDraw(void) {}

void TTCutscene_EditorLoad(void) {}

void TTCutscene_Serialize(void) { RSDK_EDITABLE_VAR(TTCutscene, VAR_VECTOR2, size); }
