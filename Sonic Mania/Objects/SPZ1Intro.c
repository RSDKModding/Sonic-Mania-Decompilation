#include "../SonicMania.h"

ObjectSPZ1Intro *SPZ1Intro;

void SPZ1Intro_Update(void)
{
    void *states[] = { SPZ1Intro_CutsceneState_Unknown1, SPZ1Intro_CutsceneState_Unknown2, SPZ1Intro_CutsceneState_Unknown3, NULL };

    RSDK_THIS(SPZ1Intro);
    if (!entity->timer) {
        if (globals->gameMode && globals->gameMode != MODE_ENCORE || !globals->enableIntro || PlayerHelpers_CheckStageReload()) {
            entity->active = ACTIVE_NEVER;
        }
        else {
            entity->timer = 1;
            CutsceneSeq_StartSequence((Entity *)entity, states);
        }
    }
}

void SPZ1Intro_LateUpdate(void) {}

void SPZ1Intro_StaticUpdate(void) {}

void SPZ1Intro_Draw(void) {}

void SPZ1Intro_Create(void *data)
{
    RSDK_THIS(SPZ1Intro);
    entity->active  = ACTIVE_NORMAL;
    entity->visible = false;
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

void SPZ1Intro_StageLoad(void)
{
    SPZ1Intro->sfxGasPop = RSDK.GetSFX("OOZ/GasPop.wav");
    SPZ1Intro->sfxPon    = RSDK.GetSFX("Stage/Pon.wav");
    SPZ1Intro->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/ManholeCover.bin", SCOPE_STAGE);
}

bool32 SPZ1Intro_CutsceneState_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(SPZ1Intro);
    RSDK_GET_PLAYER(player1, player2, camera);
    Entity *curEnt = host->cutsceneCurEntity;
    if (!host->timer) {
        player1->position.x = entity->position.x;
        camera->position.x  = entity->position.x;
        player1->position.y += 0x250000;
        player1->camera         = StateMachine_None;
        player1->tileCollisions = false;
        player1->onGround       = false;
        player1->state          = Player_State_None;
        player1->stateInput     = StateMachine_None;
        CutsceneSeq_LockAllPlayerControl();
        RSDK.SetSpriteAnimation(player1->spriteIndex, ANI_JUMP, &player1->playerAnimator, false, 0);
        if (player2->objectID == Player->objectID) {
            player2->position.x     = player1->position.x;
            player2->position.y     = player1->position.y;
            player2->tileCollisions = false;
            player2->onGround       = false;
            player2->velocity.y     = -1;
            player2->state          = Player_State_None;
            player2->stateInput     = StateMachine_None;
            RSDK.SetSpriteAnimation(player2->spriteIndex, ANI_JUMP, &player2->playerAnimator, false, 0);
        }
        EntityDebris *debris  = (EntityDebris *)RSDK.CreateEntity(Debris->objectID, 0, curEnt->position.x, curEnt->position.y + 0x390000);
        debris->drawOrder     = Zone->uiDrawLow;
        debris->state         = StateMachine_None;
        debris->drawFX        = FX_SCALE | FX_ROTATE;
        debris->scale.x       = 0x200;
        debris->scale.y       = 0x200;
        debris->updateRange.x = 0x800000;
        debris->updateRange.y = 0x800000;
        RSDK.SetSpriteAnimation(SPZ1Intro->aniFrames, 0, &debris->data, true, 0);
        SPZ1Intro->debris = debris;
    }

    if (RSDK.GetEntityCount(TitleCard->objectID, 0) || RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID) {
        if (!host->field_6C[0]) {
            RSDK_sceneInfo->timeEnabled  = false;
            RSDK_sceneInfo->milliseconds = 0;
            camera->state                = StateMachine_None;
            return false;
        }
    }
    else if (!host->field_6C[0]) {
        host->field_6C[0] = 1;
        host->field_68    = host->timer;
        if (!host->field_6C[0]) {
            RSDK_sceneInfo->timeEnabled  = false;
            RSDK_sceneInfo->milliseconds = 0;
            camera->state                = StateMachine_None;
            return false;
        }
    }

    if (host->timer == 45)
        RSDK.PlaySFX(Player->sfx_Roll, 0, 255);
    if (host->timer == 90) {
        return true;
    }
    else {
        RSDK_sceneInfo->timeEnabled  = false;
        RSDK_sceneInfo->milliseconds = 0;
        camera->state                = StateMachine_None;
        return false;
    }
}

bool32 SPZ1Intro_CutsceneState_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(SPZ1Intro);
    RSDK_GET_PLAYER(player1, player2, camera);
    if (!host->timer) {
        player1->velocity.x      = 0;
        player1->velocity.y      = -524288;
        player1->state           = Player_State_Air;
        player1->nextAirState    = 0;
        player1->nextGroundState = 0;
        RSDK.PlaySFX(SPZ1Intro->sfxGasPop, 0, 255);
        RSDK.PlaySFX(SPZ1Intro->sfxPon, 0, 255);
        RSDK.StopSFX(Player->sfx_Roll);
        Camera_ShakeScreen(0, 0, 2);
        EntityDebris *debris = SPZ1Intro->debris;
        debris->state        = Debris_State_Fall;
        debris->velocity.y   = -0x60000;
        debris->velocity.x   = -0x18000;
        debris->gravity      = 0x3800;
        debris->scaleInc.x   = 8;
        debris->scaleInc.y   = 8;
        debris->rotSpeed     = -1;
        RSDK.SetSpriteAnimation(SPZ1Intro->aniFrames, 1, &debris->data, true, 0);
    }
    if (player1->velocity.y > 0)
        player1->tileCollisions = true;

    if (player2->objectID == Player->objectID) {
        if (host->timer == 10) {
            player2->velocity.x      = 0;
            player2->velocity.y      = -0x80000;
            player2->state           = Player_State_Air;
            player2->nextAirState    = StateMachine_None;
            player2->nextGroundState = StateMachine_None;
        }
        if (player2->velocity.y > 0)
            player2->tileCollisions = 1;
        if (!player1->onGround || !player2->onGround)
            return false;
    }
    else if (!player1->onGround) {
        return false;
    }
    return true;
}

bool32 SPZ1Intro_CutsceneState_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_THIS(SPZ1Intro);
    RSDK_GET_PLAYER(player1, player2, camera);
    if (!host->timer) {
        camera->targetPtr   = (Entity *)player1;
        camera->state       = Camera_State_Follow;
        player1->stateInput = Player_ProcessP1Input;
        player1->state      = Player_State_Ground;
        player1->camera     = camera;
        if (player2->objectID == Player->objectID)
            player2->stateInput = Player_ProcessP2Input;
        RSDK_sceneInfo->timeEnabled = true;
        return true;
    }
    return false;
}

void SPZ1Intro_EditorDraw(void) {}

void SPZ1Intro_EditorLoad(void) {}

void SPZ1Intro_Serialize(void) { RSDK_EDITABLE_VAR(SPZ1Intro, VAR_VECTOR2, size); }
