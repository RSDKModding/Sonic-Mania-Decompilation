#include "SonicMania.h"

ObjectLRZ1Outro *LRZ1Outro;

void LRZ1Outro_Update(void)
{
    RSDK_THIS(LRZ1Outro);
    LRZ1Outro_StartCutscene();
    entity->active = ACTIVE_NEVER;
}

void LRZ1Outro_LateUpdate(void) {}

void LRZ1Outro_StaticUpdate(void) {}

void LRZ1Outro_Draw(void) {}

void LRZ1Outro_Create(void *data)
{
    RSDK_THIS(LRZ1Outro);
    entity->active = ACTIVE_NORMAL;
}

void LRZ1Outro_StageLoad(void) {}

void LRZ1Outro_StartCutscene(void)
{
    RSDK_THIS(LRZ1Outro);

    if (Zone->actID) {
        void *states[] = { LRZ1Outro_CutsceneState2_Unknown1, LRZ1Outro_CutsceneState2_Unknown2, LRZ1Outro_CutsceneState2_Unknown3, NULL };
        CutsceneSeq_StartSequence((Entity *)entity, states);
    }
    else {
        void *states[] = { LRZ1Outro_CutsceneState1_Unknown1, LRZ1Outro_CutsceneState1_Unknown2, LRZ1Outro_CutsceneState1_Unknown3,
                           LRZ1Outro_CutsceneState1_Unknown4, NULL };
        CutsceneSeq_StartSequence((Entity *)entity, states);
    }

    if (RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->objectID)
        RSDK_GET_ENTITY(SLOT_CUTSCENESEQ, CutsceneSeq)->skipType = SKIPTYPE_RELOADSCN;

    for (int i = 0; i < 0x100; ++i) {
        RSDK.SetPaletteEntry(5, i, 0x000000);
    }
}

bool32 LRZ1Outro_CutsceneState1_Unknown1(EntityCutsceneSeq *host)
{
    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state      = Player_State_Ground;
        player->stateInput = 0;
    }

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLow, &size, true);
    for (int p = 0; p < Player->playerCount; ++p) {
        Zone->screenBoundsR1[p] = size.x;
    }

    return true;
}

bool32 LRZ1Outro_CutsceneState2_Unknown1(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    foreach_all(Player, player)
    {
        player->onGround      = true;
        player->groundedStore = true;
        player->direction     = FLIP_X;
        RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SPINDASH, &player->playerAnimator, true, 0);
        player->state      = Player_State_Spindash;
        player->stateInput = 0;
        player->down       = true;

        if (player->camera) {
            player->camera->position.x = player->position.x;
            player->camera->position.y = player->position.y;
            player->camera->offset.x   = 0;
            player->camera->offset.y   = 0;
        }
    }

    foreach_all(DashLift, lift) { entity->lift = lift; }

    entity->timer = 256;
    RSDK.SetLimitedFade(0, 1, 5, 256, 128, 256);
    LRZ2Setup->palTimer2 = 0;
    return true;
}

bool32 LRZ1Outro_CutsceneState1_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    foreach_active(Player, player)
    {
        player->left      = false;
        player->right     = true;
        player->jumpPress = false;
        player->jumpHold  = true;
        if (player->onGround == true) {
            if (player->playerAnimator.animationID == ANI_PUSH)
                player->jumpPress = true;
            if (RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_FLOOR, 0, 0x100000, 0x160000, false) == false)
                player->jumpPress = true;
        }
    }

    entity->lift = NULL;
    foreach_active(DashLift, lift) { entity->lift = lift; }

    return RSDK.GetEntityCount(DashLift->objectID, true) > 0;
}

bool32 LRZ1Outro_CutsceneState1_Unknown3(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);
    EntityDashLift *lift = entity->lift;

    foreach_active(Player, player)
    {
        player->jumpHold  = false;
        player->jumpPress = false;
        if (player->playerAnimator.animationID == ANI_PUSH)
            player->jumpPress = true;
        if (lift->position.x - player->position.x < 0x600000 && player->right && player->onGround) {
            player->jumpPress = true;
        }
        if (player->position.x >= lift->position.x) {
            player->position.x = lift->position.x;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->right      = false;
            player->direction  = FLIP_X;
            if (!entity->timer && !player->sidekick)
                entity->timer = 1;
        }
    }

    if (entity->timer > 0)
        entity->timer++;
    if (entity->timer >= 60) {
        foreach_active(Player, player)
        {
            player->jumpHold  = false;
            player->jumpPress = false;
            player->down      = true;
        }
        RSDK.CopyPalette(0, 128, 1, 128, 128);
        return true;
    }
    return false;
}

bool32 LRZ1Outro_CutsceneState1_Unknown4(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);
    EntityDashLift *lift = entity->lift;

    foreach_active(Player, player)
    {
        player->jumpPress = false;
        if (!(Zone->timer & 0xF) && host->timer < 60)
            player->jumpPress = true;

        if (player->camera) {
            player->camera->offset.x >>= 1;
            player->camera->offset.y >>= 1;
        }
    }

    if (host->timer > 120) {
        entity->timer += 2;
        RSDK.SetLimitedFade(0, 1, 5, entity->timer, 128, 256);
        if (entity->timer > 256) {
            globals->suppressTitlecard = true;
            globals->suppressAutoMusic = true;
            Zone_StoreEntities(lift->position.x, lift->position.y);
            RSDK.LoadScene();
            return true;
        }
    }
    return false;
}

bool32 LRZ1Outro_CutsceneState2_Unknown2(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);
    EntityDashLift *lift = entity->lift;

    foreach_active(Player, player)
    {
        player->jumpPress = false;
        if (!(Zone->timer & 0xF) && host->timer < 60)
            player->jumpPress = true;
    }

    if (lift->drawPos.y < lift->amplitude.y) {
        if (entity->timer > 0)
            entity->timer -= 2;
        RSDK.SetLimitedFade(0, 1, 5, entity->timer, 128, 256);
        LRZ2Setup->palTimer2 = 0;
    }
    else {
        foreach_active(Player, player)
        {
            player->onGround   = false;
            player->down       = false;
            player->jumpPress  = false;
            player->direction  = FLIP_NONE;
            player->velocity.x = 0x20000;
            player->groundVel  = 0x20000;
            player->velocity.y = -0x50000;
            player->state      = Player_State_Air;
            RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, true, 0);
        }
        return true;
    }
    return false;
}

bool32 LRZ1Outro_CutsceneState2_Unknown3(EntityCutsceneSeq *host)
{
    bool32 flag = true;
    foreach_active(Player, player)
    {
        if (player->onGround) {
            player->velocity.x = 0;
            player->groundVel  = 0;
        }
        else {
            player->velocity.x = 0x20000;
            flag               = false;
        }
    }

    if (flag) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->stateInput = Player_ProcessP1Input;
            else
                player->stateInput = Player_ProcessP2Input_AI;
        }

        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_Unknown6;
            titleCard->stateDraw = TitleCard_StateDraw_Default;
            foreach_break;
        }

        globals->suppressAutoMusic = false;
        Music_PlayTrack(TRACK_STAGE);
        return true;
    }
    return false;
}

void LRZ1Outro_EditorDraw(void) {}

void LRZ1Outro_EditorLoad(void) {}

void LRZ1Outro_Serialize(void) {}
