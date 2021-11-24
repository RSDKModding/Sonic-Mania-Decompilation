#include "SonicMania.h"

ObjectPropellerShaft *PropellerShaft;

void PropellerShaft_Update(void)
{
    RSDK_THIS(PropellerShaft);
    foreach_active(Player, player)
    {
        int32 pID = RSDK.GetEntityID(player);

        if (((1 << pID) & self->activePlayers)) {
            if (!Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                player->state             = Player_State_Air;
                self->playerTimers[pID] = 30;
                self->activePlayers &= ~(1 << pID);
            }
            else {
                if (player->up) {
                    if (player->position.y > self->position.y - (self->size << 16) + 0x80000)
                        player->position.y -= 0x10000;
                }
                if (player->down) {
                    if (player->position.y < self->position.y + ((self->size - 8) << 16))
                        player->position.y += 0x10000;
                }
                if (player->jumpPress) {
                    self->activePlayers &= ~(1 << pID);
                    self->playerTimers[pID] = 30;
                    if (player->left)
                        player->velocity.x = -0x100000;
                    else
                        player->velocity.x = 0x100000;
                    player->velocity.y = -0x18000;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->drawOrder = Zone->playerDrawLow;
                    player->state     = Player_State_Air;
                }
            }
        }
        else {
            if (self->playerTimers[pID]) {
                self->playerTimers[pID]--;
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    self->activePlayers |= 1 << pID;
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;
                    player->position.x = self->position.x;
                    if (player->position.y < self->position.y - (self->size << 16) + 0x90000)
                        player->position.y = self->position.y - (self->size << 16) + 0x90000;
                    if (player->position.y > ((self->size - 9) << 16) + self->position.y)
                        player->position.y = ((self->size - 9) << 16) + self->position.y;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SHAFTSWING, &player->animator, false, 0);
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
        if (((1 << RSDK.GetEntityID(player)) & self->activePlayers)) {
            if (player->animator.frameID > 5)
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
    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->visible       = true;
        self->drawOrder     = Zone->drawOrderHigh;
        self->updateRange.y = self->size << 16;
        self->updateRange.x = 0x400000;
        self->hitbox.top    = -self->size;
        self->hitbox.left   = -8;
        self->hitbox.right  = 8;
        self->hitbox.bottom = self->size;
    }
}

void PropellerShaft_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void PropellerShaft_EditorDraw(void)
{
    RSDK_THIS(PropellerShaft);
    self->drawOrder = Zone->drawOrderHigh;

    if (showGizmos()) {
        DrawHelpers_DrawRectOutline(0xFFFF00, self->position.x, self->position.y, 16 << 16, self->size << 17);
    }
}

void PropellerShaft_EditorLoad(void) {}
#endif

void PropellerShaft_Serialize(void) { RSDK_EDITABLE_VAR(PropellerShaft, VAR_ENUM, size); }
