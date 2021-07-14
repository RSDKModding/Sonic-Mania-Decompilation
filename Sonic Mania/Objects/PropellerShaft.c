#include "../SonicMania.h"

ObjectPropellerShaft *PropellerShaft;

void PropellerShaft_Update(void)
{
    RSDK_THIS(PropellerShaft);
    foreach_active(Player, player)
    {
        int pID = RSDK.GetEntityID(player);

        if (((1 << pID) & entity->activePlayers)) {
            if (!Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                player->state             = Player_State_Air;
                entity->playerTimers[pID] = 30;
                entity->activePlayers &= ~(1 << pID);
            }
            else {
                if (player->up) {
                    if (player->position.y > entity->position.y - (entity->size << 16) + 0x80000)
                        player->position.y -= 0x10000;
                }
                if (player->down) {
                    if (player->position.y < entity->position.y + ((entity->size - 8) << 16))
                        player->position.y += 0x10000;
                }
                if (player->jumpPress) {
                    entity->activePlayers &= ~(1 << pID);
                    entity->playerTimers[pID] = 30;
                    if (player->left)
                        player->velocity.x = -0x100000;
                    else
                        player->velocity.x = 0x100000;
                    player->velocity.y = -0x18000;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_JUMP, &player->playerAnimator, false, 0);
                    player->drawOrder = Zone->playerDrawLow;
                    player->state     = Player_State_Air;
                }
            }
        }
        else {
            if (entity->playerTimers[pID]) {
                entity->playerTimers[pID]--;
            }
            else {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                    entity->activePlayers |= 1 << pID;
                    RSDK.PlaySFX(Player->sfx_Grab, 0, 255);
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    player->position.x = entity->position.x;
                    if (player->position.y < entity->position.y - (entity->size << 16) + 0x90000)
                        player->position.y = entity->position.y - (entity->size << 16) + 0x90000;
                    if (player->position.y > ((entity->size - 9) << 16) + entity->position.y)
                        player->position.y = ((entity->size - 9) << 16) + entity->position.y;
                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_SHAFTSWING, &player->playerAnimator, false, 0);
                    player->rotation        = 0;
                    player->direction       = FLIP_NONE;
                    player->onGround        = false;
                    player->groundedStore   = false;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->state           = Player_State_None;
                }
            }
        }
    }
}

void PropellerShaft_LateUpdate(void)
{
    RSDK_THIS(PropellerShaft);
    foreach_active(Player, player)
    {
        if (((1 << RSDK.GetEntityID(player)) & entity->activePlayers)) {
            if (player->playerAnimator.frameID > 5)
                player->drawOrder = Zone->playerDrawLow - 3;
            else
                player->drawOrder = Zone->playerDrawLow;
        }
    }
}

void PropellerShaft_StaticUpdate(void) {}

void PropellerShaft_Draw(void) {}

void PropellerShaft_Create(void *data)
{
    RSDK_THIS(PropellerShaft);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->visible       = true;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->updateRange.y = entity->size << 16;
        entity->updateRange.x = 0x400000;
        entity->hitbox.top    = -entity->size;
        entity->hitbox.left   = -8;
        entity->hitbox.right  = 8;
        entity->hitbox.bottom = entity->size;
    }
}

void PropellerShaft_StageLoad(void) {}

void PropellerShaft_EditorDraw(void) {}

void PropellerShaft_EditorLoad(void) {}

void PropellerShaft_Serialize(void) { RSDK_EDITABLE_VAR(PropellerShaft, VAR_ENUM, size); }
