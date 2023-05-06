// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Turntable Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTurntable *Turntable;

void Turntable_Update(void)
{
    RSDK_THIS(Turntable);

    self->angle += self->angleVel;

    if (self->angle < 0)
        self->angle += 0x400;
    self->angle %= 0x3FF;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (!((1 << playerID) & self->activePlayers)) {
            if (Player_CheckCollisionBox(player, self, &self->hitbox) == C_TOP) {

#if MANIA_USE_PLUS
                if (player->state == Player_State_MightyHammerDrop || player->state == Player_State_BubbleBounce)
                    continue;
#else
                if (player->state == Player_State_BubbleBounce)
                    continue;
#endif

                self->active = ACTIVE_NORMAL;
                self->activePlayers |= (1 << playerID);
                player->nextGroundState = StateMachine_None;
                player->nextAirState    = StateMachine_None;
                player->velocity.x      = 0;
                player->velocity.y      = 0;
                player->groundVel       = 0;
                player->onGround        = true;
                player->state           = Player_State_Static;

                int32 dist = FROM_FIXED(abs(player->position.x - self->position.x));

                if (dist >= 0x10) {
                    if (player->position.x <= self->position.x) {
                        self->playerAngles[playerID] = 0x200;
                        self->playerFrames[playerID] = 12;
                    }
                    else {
                        self->playerAngles[playerID] = 0x000;
                        self->playerFrames[playerID] = 0;
                    }
                }
                else {
                    int32 distY = 16 * (0x10000 - dist / 16);
                    int32 angY  = -16 * (0x10000 - dist / 16);

                    if (player->drawGroup != Zone->playerDrawGroup[0])
                        angY = distY;

                    int32 distX = player->position.x - self->position.x;
                    int32 angX  = player->position.x >= self->position.x ? distX : -distX;

                    dist                         = 0x10;
                    self->playerAngles[playerID] = 4 * RSDK.ATan2(angX, angY);
                    self->playerFrames[playerID] = 0;
                }

                self->playerDistance[playerID] = dist;
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_TWISTER, &player->animator, true, self->playerFrames[playerID]);
                Hitbox *hitbox     = Player_GetHitbox(player);
                player->position.y = self->position.y - (hitbox->bottom << 16) - (self->size.y >> 1);
            }
        }

        if ((1 << playerID) & self->activePlayers) {
            player->velocity.x           = 0;
            player->velocity.y           = 0;
            player->groundVel            = 0;
            self->playerAngles[playerID] = (self->angleVel + self->playerAngles[playerID]) & 0x3FF;
            player->position.x           = self->position.x;
            player->position.x += (RSDK.Cos1024(self->playerAngles[playerID]) << 6) * self->playerDistance[playerID];

            int32 frame = 0;
            if (player->direction)
                frame = 24 - self->playerAngles[playerID] / 42;
            else
                frame = self->playerAngles[playerID] / 42 % 24;

            player->drawGroup = self->playerAngles[playerID] < 0x200 ? Zone->playerDrawGroup[1] : Zone->playerDrawGroup[0];

            player->animator.frameID = (self->playerFrames[playerID] + frame) % -24;

            if (player->jumpPress)
                Player_Action_Jump(player);
            else if (player->animator.animationID == ANI_TWISTER && player->state == Player_State_Static)
                continue;

            self->activePlayers &= ~(1 << playerID);
        }
    }
}

void Turntable_LateUpdate(void) {}

void Turntable_StaticUpdate(void) {}

void Turntable_Draw(void)
{
    RSDK_THIS(Turntable);

    RSDK.SetSpriteAnimation(Turntable->aniFrames, self->type, &self->animator, true, 13 - (self->angle >> 1) / 9 % 14);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Turntable_Create(void *data)
{
    RSDK_THIS(Turntable);

    self->active        = ACTIVE_BOUNDS;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->origin        = self->position;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    if (!self->angleVel)
        self->angleVel = 6;

    Turntable_SetupSize();

    self->hitbox.right  = (self->size.x >> 17);
    self->hitbox.left   = -(self->size.x >> 17);
    self->hitbox.bottom = (self->size.y >> 17);
    self->hitbox.top    = -(self->size.y >> 17);

    RSDK.SetSpriteAnimation(Turntable->aniFrames, 0, &self->animator, true, 6);
}

void Turntable_StageLoad(void) { Turntable->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Turntable.bin", SCOPE_STAGE); }

void Turntable_SetupSize(void)
{
    RSDK_THIS(Turntable);

    RSDK.SetSpriteAnimation(Turntable->aniFrames, self->type, &self->animator, true, 0);

    switch (self->type) {
        case TURNTABLE_SMALL:
            self->size.x = 0x6A0000;
            self->size.y = 0x200000;
            break;

        case TURNTABLE_LARGE:
            self->size.x = 0xD40000;
            self->size.y = 0x300000;
            break;
    }
}

#if GAME_INCLUDE_EDITOR
void Turntable_EditorDraw(void) { Turntable_Draw(); }

void Turntable_EditorLoad(void)
{
    Turntable->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/Turntable.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Turntable, direction);
    RSDK_ENUM_VAR("(Unused)", FLIP_NONE); // FX_FLIP isn't even active on the animator
    // RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    // RSDK_ENUM_VAR("Flip X", FLIP_X);

    RSDK_ACTIVE_VAR(Turntable, type);
    RSDK_ENUM_VAR("Small", TURNTABLE_SMALL);
    RSDK_ENUM_VAR("Large", TURNTABLE_LARGE);
}
#endif

void Turntable_Serialize(void)
{
    RSDK_EDITABLE_VAR(Turntable, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Turntable, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Turntable, VAR_INT32, angleVel);
}
