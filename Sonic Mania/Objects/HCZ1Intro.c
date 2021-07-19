#include "../SonicMania.h"

ObjectHCZ1Intro *HCZ1Intro;

void HCZ1Intro_Update(void)
{
    void *states[] = { HCZ1Intro_CutsceneState, NULL };

    RSDK_THIS(HCZ1Intro);
    if (!entity->activated) {
        if (!isMainGameMode() || !globals->enableIntro || PlayerHelpers_CheckStageReload()) {
            entity->active = ACTIVE_NEVER;
        }
        else {
            entity->activated = true;
            CutsceneSeq_StartSequence((Entity *)entity, states);
        }
    }
}

void HCZ1Intro_LateUpdate(void) {}

void HCZ1Intro_StaticUpdate(void) {}

void HCZ1Intro_Draw(void) {}

void HCZ1Intro_Create(void *data)
{
    RSDK_THIS(HCZ1Intro);

    entity->active        = ACTIVE_NORMAL;
    entity->visible       = false;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!entity->size.x)
        entity->size.x = 0x1A80000;
    if (!entity->size.y)
        entity->size.y = 0xF00000;
    entity->updateRange.x = 0x800000 + entity->size.x;
    entity->updateRange.y = 0x800000 + entity->size.y;

    entity->hitbox.left   = -(entity->size.x >> 17);
    entity->hitbox.right  = entity->size.x >> 17;
    entity->hitbox.top    = -(entity->size.y >> 17);
    entity->hitbox.bottom = entity->size.y >> 17;
}

void HCZ1Intro_StageLoad(void)
{
    if (isMainGameMode() && globals->enableIntro && !PlayerHelpers_CheckStageReload() && !Zone->actID) {
        Water->field_D4 = 1;
    }
}

bool32 HCZ1Intro_CutsceneState(EntityCutsceneSeq *host)
{
    RSDK_THIS(HCZ1Intro);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    EntityCamera *camera  = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);

    if (!host->timer) {
        player1->position.x = entity->position.x;
        camera->position.x  = entity->position.x;
        camera->position.y  = 0;
        player1->position.x -= 0x200000;
        player1->camera     = NULL;
        player1->onGround   = false;
        player1->state      = Player_State_Air;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_FLUME, &player1->playerAnimator, false, 0);
#endif
        if (player2->objectID == Player->objectID) {
            player2->position.x = player1->position.x;
            player2->position.y = player1->position.y;
            player2->state      = Player_State_Air;
            player2->onGround   = false;
            player2->stateInput = StateMachine_None;
            player2->position.x = player1->position.x - 0x200000;
#if RETRO_USE_PLUS
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_FLUME, &player2->playerAnimator, false, 0);
#endif
        }
    }

    if (host->timer >= 8) {
        if (host->timer == 8) {
            camera->targetPtr   = (Entity *)player1;
            camera->state       = Camera_State_Follow;
            player1->stateInput = Player_ProcessP1Input;
            player1->camera     = camera;
            Water->field_D4     = 0;
        }
    }
    else {
        player1->position.y = 0;
        player1->velocity.x = 0;
        player1->velocity.y = 0;
        if (player2->objectID == Player->objectID) {
            player2->position.y = 0;
            player2->velocity.x = 0;
            player2->velocity.y = 0;
        }
    }

    if (RSDK.GetEntityCount(TitleCard->objectID, false) || RSDK_GET_ENTITY(SLOT_ACTCLEAR, TitleCard)->objectID) {
        if (!host->field_6C[0])
            return false;
    }
    else if (!host->field_6C[0]) {
        host->field_6C[0] = 1;
        host->field_68    = host->timer;
    }

    if (host->timer >= 8) {
        if (player2->objectID != Player->objectID)
            return true;
        if (player2->underwater) {
            player2->stateInput = Player_ProcessP2Input_AI;
            return true;
        }
    }
    return false;
}

void HCZ1Intro_EditorDraw(void) {}

void HCZ1Intro_EditorLoad(void) {}

void HCZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(HCZ1Intro, VAR_VECTOR2, size); }
