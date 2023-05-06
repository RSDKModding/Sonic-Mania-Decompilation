// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PropellerShaft Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPropellerShaft *PropellerShaft;

void PropellerShaft_Update(void)
{
    RSDK_THIS(PropellerShaft);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if ((1 << playerID) & self->activePlayers) {
            if (!Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                player->state                = Player_State_Air;
                self->playerTimers[playerID] = 30;
                self->activePlayers &= ~(1 << playerID);
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
                    self->activePlayers &= ~(1 << playerID);
                    self->playerTimers[playerID] = 30;
                    player->velocity.x           = player->left ? -0x100000 : 0x100000;
                    player->velocity.y           = -0x18000;
                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
                    player->drawGroup = Zone->playerDrawGroup[0];
                    player->state     = Player_State_Air;
                }
            }
        }
        else {
            if (self->playerTimers[playerID]) {
                self->playerTimers[playerID]--;
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
                    self->activePlayers |= 1 << playerID;
                    RSDK.PlaySfx(Player->sfxGrab, false, 255);
                    player->velocity.x = 0;
                    player->velocity.y = 0;
                    player->groundVel  = 0;

                    player->position.x = self->position.x;
                    if (player->position.y < self->position.y - (self->size << 16) + 0x90000)
                        player->position.y = self->position.y - (self->size << 16) + 0x90000;

                    if (player->position.y > ((self->size - 9) << 16) + self->position.y)
                        player->position.y = ((self->size - 9) << 16) + self->position.y;

                    RSDK.SetSpriteAnimation(player->aniFrames, ANI_SHAFT_SWING, &player->animator, false, 0);
                    player->rotation        = 0;
                    player->direction       = FLIP_NONE;
                    player->onGround        = false;
                    player->groundedStore   = false;
                    player->nextGroundState = StateMachine_None;
                    player->nextAirState    = StateMachine_None;
                    player->state           = Player_State_Static;
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
        if (((1 << RSDK.GetEntitySlot(player)) & self->activePlayers)) {
            if (player->animator.frameID > 5)
                player->drawGroup = Zone->playerDrawGroup[0] - 3;
            else
                player->drawGroup = Zone->playerDrawGroup[0];
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
        self->drawGroup     = Zone->objectDrawGroup[1];
        self->updateRange.x = 0x400000;
        self->updateRange.y = self->size << 16;

        self->hitbox.left   = -8;
        self->hitbox.top    = -self->size;
        self->hitbox.right  = 8;
        self->hitbox.bottom = self->size;
    }
}

void PropellerShaft_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void PropellerShaft_EditorDraw(void)
{
    RSDK_THIS(PropellerShaft);
    self->drawGroup = Zone->objectDrawGroup[1];

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, 16 << 16, self->size << 17, 0xFFFF00);
}

void PropellerShaft_EditorLoad(void) {}
#endif

void PropellerShaft_Serialize(void) { RSDK_EDITABLE_VAR(PropellerShaft, VAR_ENUM, size); }
