#include "SonicMania.h"

ObjectGHZCutsceneK *GHZCutsceneK;

void GHZCutsceneK_Update(void)
{
    void *states[2] = { GHZCutsceneK_Unknown, NULL };

    RSDK_THIS(GHZCutsceneK);
    if (!entity->timer) {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && !player->sidekick) {
                CutsceneSeq_StartSequence((Entity *)entity, states);
                entity->timer = 1;
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
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = false;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
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
void GHZCutsceneK_EditorDraw(void) {}

void GHZCutsceneK_EditorLoad(void) {}
#endif

void GHZCutsceneK_Serialize(void) { RSDK_EDITABLE_VAR(GHZCutsceneK, VAR_VECTOR2, size); }
