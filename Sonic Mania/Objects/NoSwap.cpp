#include "../SonicMania.hpp"

#if RETRO_USE_PLUS
ObjectNoSwap *NoSwap;

void NoSwap_Update()
{
    EntityNoSwap *entity = (EntityNoSwap *)RSDK_sceneInfo->entity;
    if (Player->playerCount) {
        Vector2 *pos = &entity->posPtr;
        int cnt = 0;
        for (int p = 0; p < Player->playerCount; ++p) {
            EntityPlayer * player = (EntityPlayer *)RSDK.GetEntityByID(p);

            if (!Player_CheckValidState(player)) {
                if (entity->noDeathSwap) {
                    if (player->state == Player_State_Die || player->state == Player_State_Drown) {
                        Player->field_93C = true;
                        ++NoSwap->counter;
                    }
                }
            }
            else {
                pos->x = player->position.x;
                pos->y = player->position.y;
            }

            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                if (entity->always) {
                    Player->field_93C = true;
                    ++NoSwap->counter;
                }
                else {
                    ++cnt;
                }
            }
        }

        if (cnt == 1) {
            Player->field_93C = true;
            ++NoSwap->counter;
        }
    }
}

void NoSwap_LateUpdate()
{

}

void NoSwap_StaticUpdate()
{
    if (NoSwap->counter)
        NoSwap->counter = 0;
    else
        Player->field_93C = 0;
}

void NoSwap_Draw()
{
    EntityNoSwap *entity = (EntityNoSwap *)RSDK_sceneInfo->entity;

    RSDK.DrawLine(entity->position.x - 16 * entity->size.x, entity->position.y - 16 * entity->size.y, entity->position.x + 16 * entity->size.x,
                  entity->position.y - 16 * entity->size.y, 0xFF0000, 255, INK_NONE, 0);
    RSDK.DrawLine(entity->position.x - 16 * entity->size.x, entity->position.y + 16 * entity->size.y, entity->position.x + 16 * entity->size.x,
                  entity->position.y + 16 * entity->size.y, 0xFF0000, 255, INK_NONE, 0);
    RSDK.DrawLine(entity->position.x - 16 * entity->size.x, entity->position.y - 16 * entity->size.y, entity->position.x - 16 * entity->size.x,
                  entity->position.y + 16 * entity->size.y, 0xFF0000, 255, INK_NONE, 0);
    RSDK.DrawLine(entity->position.x + 16 * entity->size.x, entity->position.y - 16 * entity->size.y, entity->position.x + 16 * entity->size.x,
                  entity->position.y + 16 * entity->size.y, 0xFF0000, 255, INK_NONE, 0);
}

void NoSwap_Create(void* data)
{
    EntityNoSwap *entity = (EntityNoSwap *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->updateRange.x = 16 * entity->size.x;
        entity->updateRange.y = 16 * entity->size.y;

        entity->hitbox.left   = -(entity->size.x >> 12);
        entity->hitbox.top    = -(entity->size.y >> 12);
        entity->hitbox.right  = (entity->size.x >> 12);
        entity->hitbox.bottom = (entity->size.y >> 12);
        entity->active        = ACTIVE_BOUNDS;
    }
}

void NoSwap_StageLoad() { NoSwap->active = ACTIVE_ALWAYS; }

void NoSwap_EditorDraw()
{

}

void NoSwap_EditorLoad()
{

}

void NoSwap_Serialize()
{
    RSDK_EDITABLE_VAR(NoSwap, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(NoSwap, VAR_BOOL, always);
    RSDK_EDITABLE_VAR(NoSwap, VAR_BOOL, noDeathSwap);
}
#endif
