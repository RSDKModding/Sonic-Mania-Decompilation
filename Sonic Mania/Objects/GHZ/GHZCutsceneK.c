#include "SonicMania.h"

ObjectGHZCutsceneK *GHZCutsceneK;

void GHZCutsceneK_Update(void)
{
    void *states[2] = { GHZCutsceneK_Unknown, NULL };

    RSDK_THIS(GHZCutsceneK);
    if (!self->activated) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !player->sidekick) {
                CutsceneSeq_StartSequence((Entity *)self, states);
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

bool32 GHZCutsceneK_Unknown(void)
{
    RSDK_GET_PLAYER(player1, player2, camera);
    unused(player1);
    unused(player2);
    unused(camera);
    return false;
}

#if RETRO_INCLUDE_EDITOR
void GHZCutsceneK_EditorDraw(void)
{
    RSDK_THIS(GHZCutsceneK);
    CutsceneRules_DrawCutsceneBounds(self, &self->size);
}

void GHZCutsceneK_EditorLoad(void) {}
#endif

void GHZCutsceneK_Serialize(void) { RSDK_EDITABLE_VAR(GHZCutsceneK, VAR_VECTOR2, size); }
