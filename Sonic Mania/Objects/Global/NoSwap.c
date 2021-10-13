#include "SonicMania.h"

#if RETRO_USE_PLUS
ObjectNoSwap *NoSwap;

void NoSwap_Update(void)
{
    RSDK_THIS(NoSwap);
    if (Player->playerCount) {
        Vector2 *pos = &entity->posPtr;
        int32 cnt      = 0;
        for (int32 p = 0; p < Player->playerCount; ++p) {
            EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

            if (!Player_CheckValidState(player)) {
                if (entity->noDeathSwap) {
                    if (player->state == Player_State_Die || player->state == Player_State_Drown) {
                        Player->cantSwap = true;
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
                    Player->cantSwap = true;
                    ++NoSwap->counter;
                }
                else {
                    ++cnt;
                }
            }
        }

        if (cnt == 1) {
            Player->cantSwap = true;
            ++NoSwap->counter;
        }
    }
}

void NoSwap_LateUpdate(void) {}

void NoSwap_StaticUpdate(void)
{
    if (NoSwap->counter)
        NoSwap->counter = 0;
    else
        Player->cantSwap = false;
}

void NoSwap_Draw(void)
{
    RSDK_THIS(NoSwap);

    RSDK.DrawLine(entity->position.x - 16 * entity->size.x, entity->position.y - 16 * entity->size.y, entity->position.x + 16 * entity->size.x,
                  entity->position.y - 16 * entity->size.y, 0xFF0000, 255, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - 16 * entity->size.x, entity->position.y + 16 * entity->size.y, entity->position.x + 16 * entity->size.x,
                  entity->position.y + 16 * entity->size.y, 0xFF0000, 255, INK_NONE, false);
    RSDK.DrawLine(entity->position.x - 16 * entity->size.x, entity->position.y - 16 * entity->size.y, entity->position.x - 16 * entity->size.x,
                  entity->position.y + 16 * entity->size.y, 0xFF0000, 255, INK_NONE, false);
    RSDK.DrawLine(entity->position.x + 16 * entity->size.x, entity->position.y - 16 * entity->size.y, entity->position.x + 16 * entity->size.x,
                  entity->position.y + 16 * entity->size.y, 0xFF0000, 255, INK_NONE, false);
}

void NoSwap_Create(void *data)
{
    RSDK_THIS(NoSwap);
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

void NoSwap_StageLoad(void) { NoSwap->active = ACTIVE_ALWAYS; }

void NoSwap_EditorDraw(void) {}

void NoSwap_EditorLoad(void) {}

void NoSwap_Serialize(void)
{
    RSDK_EDITABLE_VAR(NoSwap, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(NoSwap, VAR_BOOL, always);
    RSDK_EDITABLE_VAR(NoSwap, VAR_BOOL, noDeathSwap);
}
#endif
