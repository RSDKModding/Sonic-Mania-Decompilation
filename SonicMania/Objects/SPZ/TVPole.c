// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TVPole Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTVPole *TVPole;

void TVPole_Update(void)
{
    RSDK_THIS(TVPole);

    StateMachine_Run(self->state);
}

void TVPole_LateUpdate(void) {}

void TVPole_StaticUpdate(void) {}

void TVPole_Draw(void)
{
    RSDK_THIS(TVPole);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.x += -0x40000 - ((8 * self->length) >> 1 << 16);
    self->animator.frameID = 0;
    RSDK.DrawSprite(&self->animator, &drawPos, false);

    drawPos.x += 0x80000;
    self->animator.frameID = 1;
    for (int32 i = 0; i < self->length; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }

    self->animator.frameID = 2;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
}

void TVPole_Create(void *data)
{
    RSDK_THIS(TVPole);

    self->active    = ACTIVE_BOUNDS;
    self->drawGroup = Zone->objectDrawGroup[0];
    self->visible   = true;
    self->drawFX    = FX_FLIP;
    if (!SceneInfo->inEditor) {
        self->updateRange.x = 0x1000000;
        self->updateRange.y = 0x1000000;

        int32 len           = (8 * self->length) >> 1;
        self->hitbox.left   = -8 - len;
        self->hitbox.right  = len + 8;
        self->hitbox.top    = -32;
        self->hitbox.bottom = -24;

        RSDK.SetSpriteAnimation(TVPole->aniFrames, 1, &self->animator, true, 2);
    }
}

void TVPole_StageLoad(void) { TVPole->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVPole.bin", SCOPE_STAGE); }

void TVPole_State_CheckGrab(void)
{
    RSDK_THIS(TVPole);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (self->playerTimers[playerID] > 0)
            self->playerTimers[playerID]--;

        if (!((1 << playerID) & self->activePlayers) && !self->playerTimers[playerID]) {
            if (Player_CheckCollisionTouch(player, self, &self->hitbox) && player->velocity.y <= 0) {
                self->activePlayers |= 1 << playerID;
                RSDK.PlaySfx(Player->sfxGrab, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_CLING, &player->animator, true, 0);
                player->direction ^= FLIP_X;
                player->rotation        = player->direction == FLIP_NONE ? 0x180 : 0x80;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->state           = Player_State_Static;
            }
        }

        if ((1 << playerID) & self->activePlayers) {
            player->position.y = self->position.y - 0x140000;

            if (player->left) {
                player->position.x -= 0x10000;

                int32 x = self->position.x + ((self->hitbox.left + 16) << 16);
                if (player->position.x < x)
                    player->position.x = x;
            }
            else if (player->right) {
                player->position.x += 0x10000;

                int32 x = self->position.x + ((self->hitbox.right - 16) << 16);
                if (player->position.x > x)
                    player->position.x = x;
            }

            if (player->jumpPress) {
                self->activePlayers &= ~(1 << playerID);
                self->playerTimers[playerID] = 60;
                player->onGround             = false;
                player->velocity.y           = -0x30000;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
                player->state = Player_State_Air;
            }
        }
    }
}

void TVPole_State_ForceRelease(void)
{
    RSDK_THIS(TVPole);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        self->playerTimers[playerID] = 0;
        if ((1 << playerID) & self->activePlayers) {
            player->onGround   = false;
            player->velocity.y = -0x30000;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, true, 0);
            player->state = Player_State_Air;
        }
    }

    self->state         = StateMachine_None;
    self->activePlayers = 0;
}

#if GAME_INCLUDE_EDITOR
void TVPole_EditorDraw(void)
{
    RSDK_THIS(TVPole);

    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    RSDK.SetSpriteAnimation(TVPole->aniFrames, 1, &self->animator, true, 2);

    TVPole_Draw();
}

void TVPole_EditorLoad(void) { TVPole->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/TVPole.bin", SCOPE_STAGE); }
#endif

void TVPole_Serialize(void) { RSDK_EDITABLE_VAR(TVPole, VAR_UINT16, length); }
