// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GHZCutsceneK Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

// This Object is COMPLETELY unused, knux uses GHZCutsceneST anyways, so it's no surprise that its not finished either

ObjectGHZCutsceneK *GHZCutsceneK;

void GHZCutsceneK_Update(void)
{
    RSDK_THIS(GHZCutsceneK);

    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !player->sidekick) {
                CutsceneSeq_StartSequence(self, GHZCutsceneK_Cutscene_None, StateMachine_None);

#if MANIA_USE_PLUS
                CutsceneSeq_SetSkipType(SKIPTYPE_DISABLED);
#endif

                self->activated = true;
            }
        }
    }
}

void GHZCutsceneK_LateUpdate(void) {}

void GHZCutsceneK_StaticUpdate(void) {}

void GHZCutsceneK_Draw(void) {}

void GHZCutsceneK_Create(void *data)
{
    RSDK_THIS(GHZCutsceneST);

    INIT_ENTITY(self);
    CutsceneRules_SetupEntity(self, &self->size, &self->hitbox);
    self->active = ACTIVE_BOUNDS;
}

void GHZCutsceneK_StageLoad(void) {}

bool32 GHZCutsceneK_Cutscene_None(void)
{
    MANIA_GET_PLAYER(player1, player2, camera);
    UNUSED(player1);
    UNUSED(player2);
    UNUSED(camera);
    return false;
}

#if GAME_INCLUDE_EDITOR
void GHZCutsceneK_EditorDraw(void)
{
    RSDK_THIS(GHZCutsceneK);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void GHZCutsceneK_EditorLoad(void) {}
#endif

void GHZCutsceneK_Serialize(void) { RSDK_EDITABLE_VAR(GHZCutsceneK, VAR_VECTOR2, size); }
