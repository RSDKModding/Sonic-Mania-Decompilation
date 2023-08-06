// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ3Cutscene Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLRZ3Cutscene *LRZ3Cutscene;

// INFO: This is the LRZ3 Intro

void LRZ3Cutscene_Update(void)
{
    RSDK_THIS(LRZ3Cutscene);

    CutsceneSeq_StartSequence(self, LRZ3Cutscene_Cutscene_FadeIn, LRZ3Cutscene_Cutscene_RunRight, StateMachine_None);

#if MANIA_USE_PLUS
    CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

    self->active = ACTIVE_NEVER;
}

void LRZ3Cutscene_LateUpdate(void) {}

void LRZ3Cutscene_StaticUpdate(void) {}

void LRZ3Cutscene_Draw(void) {}

void LRZ3Cutscene_Create(void *data)
{
    RSDK_THIS(LRZ3Cutscene);

    self->active = ACTIVE_NORMAL;
}

bool32 LRZ3Cutscene_Cutscene_FadeIn(EntityCutsceneSeq *host)
{
    if (!host->timer) {
        foreach_active(Player, player)
        {
            player->groundVel  = 0x40000;
            player->velocity.x = 0x40000;

            if (!player->sidekick) {
                player->stateInput = StateMachine_None;
                CutsceneSeq_LockAllPlayerControl();
                player->left      = false;
                player->right     = true;
                player->up        = false;
                player->down      = false;
                player->jumpPress = false;
            }
        }
    }

    if (host->timer >= 8) {
        if (globals->suppressTitlecard) {
            foreach_all(TitleCard, titlecard)
            {
                destroyEntity(titlecard);
                foreach_break;
            }

            globals->suppressTitlecard = false;
            globals->suppressAutoMusic = false;
        }

        return true;
    }

    return false;
}

bool32 LRZ3Cutscene_Cutscene_RunRight(EntityCutsceneSeq *host)
{
    if (host->timer >= 60) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->stateInput = Player_Input_P1;
        }

        return true;
    }

    return false;
}

void LRZ3Cutscene_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void LRZ3Cutscene_EditorDraw(void) {}

void LRZ3Cutscene_EditorLoad(void) {}
#endif

void LRZ3Cutscene_Serialize(void) {}
