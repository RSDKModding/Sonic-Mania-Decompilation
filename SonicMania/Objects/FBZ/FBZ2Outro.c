// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: FBZ2Outro Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

#if MANIA_USE_PLUS
ObjectFBZ2Outro *FBZ2Outro;

void FBZ2Outro_Update(void)
{
    RSDK_THIS(FBZ2Outro);

    FBZ2Outro_StartCutscene(self);
    self->active = ACTIVE_NEVER;
}

void FBZ2Outro_LateUpdate(void) {}

void FBZ2Outro_StaticUpdate(void) {}

void FBZ2Outro_Draw(void) {}

void FBZ2Outro_Create(void *data)
{
    RSDK_THIS(FBZ2Outro);

    self->active      = ACTIVE_NORMAL;
    self->isPermanent = true;
}

void FBZ2Outro_StageLoad(void) {}

void FBZ2Outro_StartCutscene(EntityFBZ2Outro *outro)
{
    CutsceneSeq_StartSequence(outro, FBZ2Outro_Cutscene_SetupGliders, FBZ2Outro_Cutscene_RunToGlider, FBZ2Outro_Cutscene_GlideAway,
                              StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_RELOADSCN, StateMachine_None);
#endif

    HUD_MoveOut();
}

bool32 FBZ2Outro_Cutscene_SetupGliders(EntityCutsceneSeq *host)
{
    CutsceneSeq_LockAllPlayerControl();

    foreach_active(Player, player)
    {
        player->state      = Player_State_Ground;
        player->stateInput = StateMachine_None;
        player->right      = true;
    }

    Vector2 size;
    RSDK.GetLayerSize(Zone->fgLayer[0], &size, true);
    size.x -= 128;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        Zone->cameraBoundsR[p]      = size.x;
        Zone->playerBoundActiveR[p] = false;
    }

    foreach_all(HangGlider, glider) { glider->active = ACTIVE_NORMAL; }

    return true;
}

bool32 FBZ2Outro_Cutscene_RunToGlider(EntityCutsceneSeq *host)
{
    foreach_active(Player, player)
    {
        player->jumpPress = false;

        if (player->animator.animationID == ANI_PUSH) {
            player->jumpPress = true;
            player->jumpHold  = true;
        }
        else {
            if (player->velocity.y > 0x20000)
                player->jumpHold = false;
        }

        if (player->sidekick) {
            foreach_active(HangGlider, glider)
            {
                if (player->onGround && glider->position.x - player->position.x < 0x1600000 && player->position.y - glider->position.y > 0x300000) {
                    player->jumpPress    = true;
                    player->jumpHold     = true;
                    player->jumpStrength = 0x70000;
                }
            }
        }
    }

    if (RSDK.GetEntityCount(Player->classID, true)) {
        foreach_active(HangGlider, glider)
        {
            if (glider->state == HangGlider_State_Glide) {
                foreach_active(FBZFan, fan)
                {
                    if (abs(fan->position.x - glider->position.x) < 0x400000 && fan->position.y - glider->position.y < 0xA00000)
                        glider->velocity.y -= 0x3000;
                }
            }
        }
    }
    else {
        for (int32 p = 0; p < Player->playerCount; ++p) Zone->cameraBoundsT[p] = Zone->cameraBoundsB[p] - ScreenInfo->size.y;

        return true;
    }

    return false;
}

bool32 FBZ2Outro_Cutscene_GlideAway(EntityCutsceneSeq *host)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    foreach_active(HangGlider, glider)
    {
        foreach_active(FBZFan, fan)
        {
            if (abs(fan->position.x - glider->position.x) < 0x400000 && fan->position.y - glider->position.y < 0xA00000)
                glider->velocity.y -= 0x3000;
        }

        if (glider->attachedPlayer == player1 && !glider->onScreen) {
            Zone_StartFadeOut(10, 0x000000);

            return true;
        }
    }

    return false;
}

#if GAME_INCLUDE_EDITOR
void FBZ2Outro_EditorDraw(void) {}

void FBZ2Outro_EditorLoad(void) {}
#endif

void FBZ2Outro_Serialize(void) {}
#endif
