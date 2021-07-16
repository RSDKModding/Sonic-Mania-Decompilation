#include "../SonicMania.h"

ObjectCPZ2Outro *CPZ2Outro;

void CPZ2Outro_Update(void)
{
    RSDK_THIS(CPZ2Outro);
    if (!entity->activated) {
        CPZ2Outro_HandleCutsceneSeq();
        entity->activated = true;
    }
}

void CPZ2Outro_LateUpdate(void) {}

void CPZ2Outro_StaticUpdate(void) {}

void CPZ2Outro_Draw(void) {}

void CPZ2Outro_Create(void *data)
{
    RSDK_THIS(CPZ2Outro);

    entity->active        = ACTIVE_NEVER;
    entity->visible       = false;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!entity->size.x)
        entity->size.x = 0x1A80000;
    if (!entity->size.y)
        entity->size.y = 0xF00000;
    entity->updateRange.x = 0x800000 + entity->size.x;
    entity->updateRange.y = 0x800000 + entity->size.y;
    entity->hitbox.left   = -entity->size.x >> 17;
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.top    = -entity->size.y >> 17;
    entity->hitbox.bottom = entity->size.y >> 17;
}

void CPZ2Outro_StageLoad(void)
{
    CPZ2Outro->prisonPtr = NULL;
    foreach_all(EggPrison, prison)
    {
        CPZ2Outro->prisonPtr = (Entity *)prison;
        foreach_break;
    }
}

void CPZ2Outro_HandleCutsceneSeq(void)
{
    void *states[] = {
        CPZ2Outro_CutsceneSeq, NULL
    };

    RSDK_THIS(CPZ2Outro);
    EntityCutsceneSeq *seq = RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq);

    CutsceneSeq_StartSequence((Entity *)entity, states);
    if (seq->objectID)
        seq->skipState = 1;

    foreach_active(HUD, hud) { hud->state = HUD_Unknown6; }
}

bool32 CPZ2Outro_CutsceneSeq(void *h)
{
    EntityCutsceneSeq *host = (EntityCutsceneSeq *)h;

    RSDK_GET_PLAYER(player1, player2, camera);

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLow, &size, true);

    if (!host->timer) {
        ((EntityEggPrison *)CPZ2Outro->prisonPtr)->activated = true;
        Zone->screenBoundsT1[0]                              = 0;
        Zone->screenBoundsT1[1]                              = 0;
        Zone->screenBoundsR1[0]                              = size.x;
        Zone->screenBoundsR1[1]                              = size.x;
        Zone->playerBoundActiveR[0]                          = 0;
        Zone->playerBoundActiveR[1]                          = 0;
        CutsceneSeq_LockAllPlayerControl();
        player1->stateInput = StateMachine_None;
        player1->state      = Player_State_Ground;
        player1->groundVel  = 0;
        player1->right      = true;
        if (player2->objectID == Player->objectID) {
            player2->state      = Player_State_Ground;
            player2->stateInput = Player_ProcessP2Input_AI;
            player2->groundVel  = 0;
        }
    }

    if (player1->jumpPress)
        player1->jumpPress = false;

    if (player1->onGround && player1->position.x >= host->cutsceneCurEntity->position.x && !host->field_6C[0]) {
        player1->jumpPress = true;
        host->field_6C[0]  = 1;
    }

    if (player1->position.x > size.x << 16) {
        Zone_StartFadeOut(10, 0x000000);
        return true;
    }
    return false;
}

void CPZ2Outro_EditorDraw(void) {}

void CPZ2Outro_EditorLoad(void) {}

void CPZ2Outro_Serialize(void) { RSDK_EDITABLE_VAR(CPZ2Outro, VAR_VECTOR2, size); }
