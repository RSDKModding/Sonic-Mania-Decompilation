#include "SonicMania.h"

ObjectOOZ1Outro *OOZ1Outro;

void OOZ1Outro_Update(void)
{
    RSDK_THIS(OOZ1Outro);

    void *states[] = { OOZ1Outro_Unknown1, OOZ1Outro_Unknown2, OOZ1Outro_Unknown3, OOZ1Outro_Unknown4, NULL };

    if (isMainGameMode() && globals->enableIntro && !PlayerHelpers_CheckStageReload()) {
        entity->activated = true;
        CutsceneSeq_StartSequence((Entity*)entity, states);
    }
    entity->active = ACTIVE_NEVER;
}

void OOZ1Outro_LateUpdate(void) {}

void OOZ1Outro_StaticUpdate(void) {}

void OOZ1Outro_Draw(void) {}

void OOZ1Outro_Create(void *data)
{
    RSDK_THIS(OOZ1Outro);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active  = ACTIVE_NORMAL;
        entity->visible = false;
        if (!entity->size.x)
            entity->size.x = 0x1A80000;
        if (!entity->size.y)
            entity->size.y = 0xF00000;
        entity->updateRange.x += entity->size.x;
        entity->updateRange.y += entity->size.y;

        entity->hitbox.left   = -entity->size.x >> 17;
        entity->hitbox.right  = entity->size.x >> 17;
        entity->hitbox.top    = -entity->size.y >> 17;
        entity->hitbox.bottom = entity->size.y >> 17;
    }
}

void OOZ1Outro_StageLoad(void) { OOZ1Outro->sfxDrop = RSDK.GetSFX("Stage/Drop.wav"); }

bool32 OOZ1Outro_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(OOZ1Outro);

    if (host->timer) {
        if (host->fillTimerA > 0)
            host->fillTimerA -= 4;
    }
    else {
        host->fillTimerA = 512;
    }

    if (host->timer == 1) {
        foreach_all(Player, player)
        {
            player->position.x = entity->position.x;
            player->position.y = entity->position.y;
            player->position.y += (entity->size.y >> 1) - 0x400000;
        }

        entity->boundsR         = Zone->screenBoundsR1[0];
        entity->boundsT         = Zone->screenBoundsT1[0];
        entity->boundsB         = Zone->screenBoundsB1[0];
        Zone->screenBoundsL1[0] = (entity->position.x >> 16) - RSDK_screens->centerX;
        Zone->screenBoundsR1[0] = (entity->position.x >> 16) + RSDK_screens->centerX;
        Zone->screenBoundsT1[0] = (entity->position.y >> 16) - RSDK_screens->centerY;
        Zone->screenBoundsB1[0] = (entity->position.y >> 16) + RSDK_screens->centerY;
        EntityCamera *camera    = RSDK_GET_ENTITY(SLOT_CAMERA1, Camera);
        camera->boundsL         = Zone->screenBoundsL1[0];
        camera->boundsR         = Zone->screenBoundsR1[0];
        camera->boundsT         = Zone->screenBoundsT1[0];
        camera->boundsB         = Zone->screenBoundsB1[0];
        Smog->field_4           = true;
    }
    if (host->timer == 30) {
        ActClear->actID = 1;

        foreach_active(SignPost, signpost)
        {
            signpost->state  = SignPost_State_Fall;
            signpost->active = ACTIVE_NORMAL;
        }

        RSDK.PlaySfx(SignPost->sfx_Twinkle, false, 255);
        return true;
    }
    return false;
}

bool32 OOZ1Outro_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(OOZ1Outro);
    if (host->fillTimerA > 0)
        host->fillTimerA -= 4;

    CutsceneSeq_LockAllPlayerControl();
    if (ActClear->finished) {
        foreach_active(Player, player)
        {
            player->state          = Player_State_Air;
            player->onGround       = false;
            player->collisionPlane = 1;
            player->interaction    = false;
            player->velocity.y     = -0x20000;
            player->stateInput     = StateMachine_None;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_HURT, &player->playerAnimator, false, 0);
        }
        Zone->screenBoundsL1[0] = entity->boundsL;
        Zone->screenBoundsR1[0] = entity->boundsR;
        Zone->screenBoundsT1[0] = entity->boundsT;
        Zone->screenBoundsB1[0] = entity->boundsB;
        if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
            RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;
        return true;
    }
    return false;
}

bool32 OOZ1Outro_Unknown3(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (host->timer == 48) {
        player1->interaction = true;
        if (player2->objectID == Player->objectID)
            player2->interaction = true;
    }
    if (Smog->field_4 && player1->playerAnimator.animationID)
        return false;
    player1->direction  = FLIP_NONE;
    player1->stateInput = Player_ProcessP1Input;
    if (player2->objectID == Player->objectID)
        player2->stateInput = Player_ProcessP2Input_AI;
    return true;
}

bool32 OOZ1Outro_Unknown4(EntityCutsceneSeq *host)
{
    if (host->timer == 120) {
        globals->suppressTitlecard = true;
        foreach_all(TitleCard, titlecard)
        {
            titlecard->active    = ACTIVE_NORMAL;
            titlecard->state     = TitleCard_Unknown6;
            titlecard->stateDraw = TitleCard_StateDraw_Default;
            foreach_break;
        }

        Music_PlayTrack(TRACK_STAGE);
        return true;
    }
    return false;
}

void OOZ1Outro_EditorDraw(void)
{
    RSDK_THIS(OOZ2Outro);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x, drawPos.y, drawPos.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
}

void OOZ1Outro_EditorLoad(void) {}

void OOZ1Outro_Serialize(void) { RSDK_EDITABLE_VAR(OOZ1Outro, VAR_VECTOR2, size); }
