// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ1Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZ1Outro *LRZ1Outro;

void LRZ1Outro_Update(void)
{
    RSDK_THIS(LRZ1Outro);

    LRZ1Outro_StartCutscene();
    self->active = ACTIVE_NEVER;
}

void LRZ1Outro_LateUpdate(void) {}

void LRZ1Outro_StaticUpdate(void) {}

void LRZ1Outro_Draw(void) {}

void LRZ1Outro_Create(void *data)
{
    RSDK_THIS(LRZ1Outro);

    self->active = ACTIVE_NORMAL;
}

void LRZ1Outro_StageLoad(void) {}

void LRZ1Outro_StartCutscene(void)
{
    RSDK_THIS(LRZ1Outro);

    if (Zone->actID) {
        CutsceneSeq_StartSequence(self, LRZ1Outro_CutsceneAct2_SetupActors, LRZ1Outro_CutsceneAct2_UsingDashLift, LRZ1Outro_CutsceneAct2_ExitDashLift,
                                  StateMachine_None);
    }
    else {
        CutsceneSeq_StartSequence(self, LRZ1Outro_CutsceneAct1_SetupActors, LRZ1Outro_CutsceneAct1_SetupDashLift, LRZ1Outro_CutsceneAct1_GoToDashLift,
                                  LRZ1Outro_CutsceneAct1_UsingDashLift, StateMachine_None);
    }

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN);
#endif

    for (int32 i = 0; i < 0x100; ++i) RSDK.SetPaletteEntry(5, i, 0x000000);
}

bool32 LRZ1Outro_CutsceneAct1_SetupActors(EntityCutsceneSeq *host)
{
    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state      = Player_State_Ground;
        player->stateInput = StateMachine_None;
    }

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);

    for (int32 p = 0; p < Player->playerCount; ++p) Zone->cameraBoundsR[p] = size.x;

    return true;
}

bool32 LRZ1Outro_CutsceneAct2_SetupActors(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    foreach_all(Player, player)
    {
        player->onGround      = true;
        player->groundedStore = true;
        player->direction     = FLIP_X;
        RSDK.SetSpriteAnimation(player->aniFrames, ANI_SPINDASH, &player->animator, true, 0);
        player->state      = Player_State_Spindash;
        player->stateInput = StateMachine_None;
        player->down       = true;

        if (player->camera) {
            player->camera->position.x = player->position.x;
            player->camera->position.y = player->position.y;
            player->camera->offset.x   = 0;
            player->camera->offset.y   = 0;
        }
    }

    foreach_all(DashLift, lift) { self->lift = lift; }

    self->timer = 256;
    RSDK.SetLimitedFade(0, 1, 5, 0x100, 128, 256);
    LRZ2Setup->conveyorPalTimer = 0;

    return true;
}

bool32 LRZ1Outro_CutsceneAct1_SetupDashLift(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    foreach_active(Player, player)
    {
        player->left      = false;
        player->right     = true;
        player->jumpPress = false;
        player->jumpHold  = true;

        if (player->onGround) {
            if (player->animator.animationID == ANI_PUSH)
                player->jumpPress = true;

            if (!RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_FLOOR, 0, 0x100000, 0x160000, false))
                player->jumpPress = true;
        }
    }

    self->lift = NULL;
    foreach_active(DashLift, lift) { self->lift = lift; }

    return RSDK.GetEntityCount(DashLift->classID, true) > 0;
}

bool32 LRZ1Outro_CutsceneAct1_GoToDashLift(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    EntityDashLift *lift = self->lift;

    foreach_active(Player, player)
    {
        player->jumpHold  = false;
        player->jumpPress = false;

        if (player->animator.animationID == ANI_PUSH)
            player->jumpPress = true;

        if (lift->position.x - player->position.x < 0x600000 && player->right && player->onGround)
            player->jumpPress = true;

        if (player->position.x >= lift->position.x) {
            player->position.x = lift->position.x;
            player->velocity.x = 0;
            player->groundVel  = 0;
            player->right      = false;
            player->direction  = FLIP_X;

            if (!self->timer && !player->sidekick)
                self->timer = 1;
        }
    }

    if (self->timer > 0)
        self->timer++;

    if (self->timer >= 60) {
        foreach_active(Player, playerPtr)
        {
            playerPtr->jumpHold  = false;
            playerPtr->jumpPress = false;
            playerPtr->down      = true;
        }

        RSDK.CopyPalette(0, 128, 1, 128, 128);
        return true;
    }

    return false;
}

bool32 LRZ1Outro_CutsceneAct1_UsingDashLift(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    EntityDashLift *lift = self->lift;

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
        self->timer += 2;
        RSDK.SetLimitedFade(0, 1, 5, self->timer, 128, 256);

        if (self->timer > 256) {
            globals->suppressTitlecard = true;
            globals->suppressAutoMusic = true;

            Zone_StoreEntities(lift->position.x, lift->position.y);
            RSDK.LoadScene();

            return true;
        }
    }
    return false;
}

bool32 LRZ1Outro_CutsceneAct2_UsingDashLift(EntityCutsceneSeq *host)
{
    RSDK_THIS(LRZ1Outro);

    EntityDashLift *lift = self->lift;

    foreach_active(Player, playerPtr)
    {
        playerPtr->jumpPress = false;
        if (!(Zone->timer & 0xF) && host->timer < 60)
            playerPtr->jumpPress = true;
    }

    if (lift->drawPos.y < lift->amplitude.y) {
        if (self->timer > 0)
            self->timer -= 2;

        RSDK.SetLimitedFade(0, 1, 5, self->timer, 128, 256);

        LRZ2Setup->conveyorPalTimer = 0;
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

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
        }

        return true;
    }

    return false;
}

bool32 LRZ1Outro_CutsceneAct2_ExitDashLift(EntityCutsceneSeq *host)
{
    bool32 landedOnGround = true;

    foreach_active(Player, player)
    {
        if (player->onGround) {
            player->velocity.x = 0;
            player->groundVel  = 0;
        }
        else {
            player->velocity.x = 0x20000;
            landedOnGround     = false;
        }
    }

    if (landedOnGround) {
        foreach_active(Player, playerPtr)
        {
            if (!playerPtr->sidekick)
                playerPtr->stateInput = Player_Input_P1;
            else
                playerPtr->stateInput = Player_Input_P2_AI;
        }

        foreach_all(TitleCard, titleCard)
        {
            titleCard->active    = ACTIVE_NORMAL;
            titleCard->state     = TitleCard_State_SetupBGElements;
            titleCard->stateDraw = TitleCard_Draw_SlideIn;
            foreach_break;
        }

        globals->suppressAutoMusic = false;
        Music_PlayTrack(TRACK_STAGE);

        return true;
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void LRZ1Outro_EditorDraw(void) {}

void LRZ1Outro_EditorLoad(void) {}
#endif

void LRZ1Outro_Serialize(void) {}
