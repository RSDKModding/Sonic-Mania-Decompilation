#include "SonicMania.h"

ObjectHCZ1Intro *HCZ1Intro;

void HCZ1Intro_Update(void)
{
    void *states[] = { HCZ1Intro_CutsceneState, NULL };

    RSDK_THIS(HCZ1Intro);
    if (!self->activated) {
        if (!isMainGameMode() || !globals->enableIntro || PlayerHelpers_CheckStageReload()) {
            self->active = ACTIVE_NEVER;
        }
        else {
            self->activated = true;
            CutsceneSeq_StartSequence((Entity *)self, states);
        }
    }
}

void HCZ1Intro_LateUpdate(void) {}

void HCZ1Intro_StaticUpdate(void) {}

void HCZ1Intro_Draw(void) {}

void HCZ1Intro_Create(void *data)
{
    RSDK_THIS(HCZ1Intro);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_NORMAL;
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
        player1->position.x = self->position.x;
        camera->position.x  = self->position.x;
        camera->position.y  = 0;
        player1->position.x -= 0x200000;
        player1->camera     = NULL;
        player1->onGround   = false;
        player1->state      = Player_State_Air;
        player1->stateInput = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_FLUME, &player1->animator, false, 0);
#else
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_HURT, &player1->animator, false, 0);
#endif
        if (player2->objectID == Player->objectID) {
            player2->position.x = player1->position.x;
            player2->position.y = player1->position.y;
            player2->state      = Player_State_Air;
            player2->onGround   = false;
            player2->stateInput = StateMachine_None;
            player2->position.x = player1->position.x - 0x200000;
#if RETRO_USE_PLUS
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_FLUME, &player2->animator, false, 0);
#else
            RSDK.SetSpriteAnimation(player1->aniFrames, ANI_HURT, &player1->animator, false, 0);
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

void HCZ1Intro_EditorDraw(void)
{
    RSDK_THIS(HCZ1Intro);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void HCZ1Intro_EditorLoad(void) {}

void HCZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(HCZ1Intro, VAR_VECTOR2, size); }
