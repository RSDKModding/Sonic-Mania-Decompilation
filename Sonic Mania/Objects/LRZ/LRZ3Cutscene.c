// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LRZ3Cutscene Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectLRZ3Cutscene *LRZ3Cutscene;

void LRZ3Cutscene_Update(void)
{
    void *states[] = { LRZ3Cutscene_Unknown1, LRZ3Cutscene_Unknown2, NULL };

    RSDK_THIS(FBZMissile);
    CutsceneSeq_StartSequence((Entity *)self, states);
    self->active = ACTIVE_NEVER;
}

void LRZ3Cutscene_LateUpdate(void) {}

void LRZ3Cutscene_StaticUpdate(void) {}

void LRZ3Cutscene_Draw(void) {}

void LRZ3Cutscene_Create(void *data)
{
    RSDK_THIS(FBZMissile);
    self->active = ACTIVE_NORMAL;
}

bool32 LRZ3Cutscene_Unknown1(EntityCutsceneSeq *host)
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

bool32 LRZ3Cutscene_Unknown2(EntityCutsceneSeq *host)
{
    if (host->timer >= 60) {
        foreach_active(Player, player)
        {
            if (!player->sidekick)
                player->stateInput = Player_ProcessP1Input;
        }
        return true;
    }
    return false;
}

void LRZ3Cutscene_StageLoad(void) {}

void LRZ3Cutscene_EditorDraw(void) {}

void LRZ3Cutscene_EditorLoad(void) {}

void LRZ3Cutscene_Serialize(void) {}
