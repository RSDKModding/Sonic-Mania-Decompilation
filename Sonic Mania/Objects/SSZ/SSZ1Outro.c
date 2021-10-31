#include "SonicMania.h"

ObjectSSZ1Outro *SSZ1Outro;

void SSZ1Outro_Update(void)
{
    void *states[] = { SSZ1Outro_CutsceneState_Unknown1, SSZ1Outro_CutsceneState_Unknown2, NULL };

    RSDK_THIS(SSZ1Outro);
    if (!entity->activated) {
        entity->activated = true;
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
        CutsceneSeq_StartSequence((Entity *)entity, states);
    }
}

void SSZ1Outro_LateUpdate(void) {}

void SSZ1Outro_StaticUpdate(void) {}

void SSZ1Outro_Draw(void) {}

void SSZ1Outro_Create(void *data)
{
    RSDK_THIS(SSZ1Outro);

    INIT_ENTITY(entity);
    CutsceneRules_SetupEntity(entity, &entity->size, &entity->hitbox);
    entity->active = ACTIVE_NEVER;
}

void SSZ1Outro_StageLoad(void)
{
    foreach_all(RTeleporter, teleporter) { SSZ1Outro->teleporter = teleporter; }
}

bool32 SSZ1Outro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(camera);
    
    RSDK_THIS(SSZ1Outro);

    if (!host->timer) {
        foreach_all(FXFade, fxFade)
        {
            if (MathHelpers_PointInHitbox(entity->direction, entity->position.x, entity->position.y, &entity->hitbox, fxFade->position.x,
                                     fxFade->position.y)) {
                SSZ1Outro->fxFade = fxFade;
                foreach_break;
            }
        }

        Zone->screenBoundsR1[0]     = entity->hitbox.right + (entity->position.x >> 16);
        Zone->playerBoundActiveR[0] = false;
        Zone->screenBoundsR1[1]     = entity->hitbox.right + (entity->position.x >> 16);
        Zone->playerBoundActiveR[1] = false;
        Zone->screenBoundsR1[2]     = entity->hitbox.right + (entity->position.x >> 16);
        Zone->playerBoundActiveR[2] = false;
        Zone->screenBoundsR1[3]     = entity->hitbox.right + (entity->position.x >> 16);
        Zone->playerBoundActiveR[3] = false;

        CutsceneSeq_LockAllPlayerControl();
        player1->state      = Player_State_Ground;
        player1->stateInput = 0;
        if (player2->objectID == Player->objectID) {
            player2->state      = Player_State_Ground;
            player2->stateInput = StateMachine_None;
        }
    }

    player1->right = true;
    if (player1->position.x < SSZ1Outro->teleporter->position.x || !player1->onGround) {
        player1->nextAirState    = StateMachine_None;
        player1->nextGroundState = StateMachine_None;
        if (player1->groundVel > 0x40000)
            player1->groundVel = 0x40000;
        player1->groundVel = player1->groundVel;

        if (player1->velocity.x > 0x40000)
            player1->velocity.x = 0x40000;
        player1->velocity.x = player1->velocity.x;
    }

    if (player2->objectID == Player->objectID) {
        if (player1->position.x > player2->position.x)
            player2->right = true;

        if (player2->groundVel < player1->groundVel)
            player2->groundVel = player1->groundVel;

        if (player1->groundVel < player2->velocity.x)
            player2->velocity.x = player1->groundVel;

        player2->nextAirState    = StateMachine_None;
        player2->nextGroundState = StateMachine_None;
    }

    if (host->field_6C[0]) {
        player1->jumpPress = false;
        if (player1->onGround)
            player1->jumpHold = false;
    }
    else if (player1->position.x >= SSZ1Outro->teleporter->position.x - 0xC80000) {
        player1->jumpPress = true;
        player1->jumpHold  = true;
        host->field_6C[0]  = 1;
    }
    return player1->position.x >= entity->position.x + (entity->hitbox.right << 16);
}
bool32 SSZ1Outro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    if (host->timer == 10)
        SSZ1Outro->fxFade->state = FXFade_State_FadeIn;
    if (SSZ1Outro->fxFade->timer == 512) {
        RSDK.SetScene("Cutscenes", "SSZ Time Warp");
        RSDK.LoadScene();
        return true;
    }
    return false;
}

void SSZ1Outro_Unknown3(Entity *entity)
{
    CREATE_ENTITY(Animals, intToVoid(Animals->animalTypes[RSDK.Rand(0, 32) >> 4] + 1), entity->position.x, entity->position.y);
    CREATE_ENTITY(Explosion, intToVoid(EXPLOSION_ENEMY), entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
    RSDK.PlaySfx(Explosion->sfxDestroy, false, 255);
    destroyEntity(entity);
}

void SSZ1Outro_Unknown4(void)
{
    foreach_active(HotaruMKII, boss)
    {
        // if (!boss->field_A4)
        //    SSZ1Outro_Unknown3((Entity*)boss);
    }
}

#if RETRO_INCLUDE_EDITOR
void SSZ1Outro_EditorDraw(void)
{
    RSDK_THIS(SSZ1Outro);
    CutsceneRules_DrawCutsceneBounds(entity, &entity->size);
}

void SSZ1Outro_EditorLoad(void) {}
#endif

void SSZ1Outro_Serialize(void) { RSDK_EDITABLE_VAR(SSZ1Outro, VAR_VECTOR2, size); }
