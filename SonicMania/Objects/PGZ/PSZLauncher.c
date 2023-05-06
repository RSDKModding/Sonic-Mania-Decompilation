// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZLauncher Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPSZLauncher *PSZLauncher;

void PSZLauncher_Update(void)
{
    RSDK_THIS(PSZLauncher);

    StateMachine_Run(self->state);
}

void PSZLauncher_LateUpdate(void) {}

void PSZLauncher_StaticUpdate(void) {}

void PSZLauncher_Draw(void)
{
    RSDK_THIS(PSZLauncher);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PSZLauncher_Create(void *data)
{
    RSDK_THIS(PSZLauncher);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[1] - 1;
    self->drawFX    = FX_FLIP;

    if (SceneInfo->inEditor && !self->power)
        self->power = 10;

    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = PSZLauncher_State_Init;
}

void PSZLauncher_StageLoad(void)
{
    PSZLauncher->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PSZLauncher.bin", SCOPE_STAGE);

    PSZLauncher->hitboxLaunch.left   = -32;
    PSZLauncher->hitboxLaunch.top    = 0;
    PSZLauncher->hitboxLaunch.right  = 32;
    PSZLauncher->hitboxLaunch.bottom = 1;

    DEBUGMODE_ADD_OBJ(PSZLauncher);
}

void PSZLauncher_DebugSpawn(void)
{
    RSDK_THIS(PSZLauncher);

    EntityPSZLauncher *launcher = CREATE_ENTITY(PSZLauncher, NULL, self->position.x, self->position.y);
    launcher->direction         = self->direction;
}

void PSZLauncher_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(PSZLauncher->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void PSZLauncher_State_Init(void)
{
    RSDK_THIS(PSZLauncher);

    RSDK.SetSpriteAnimation(PSZLauncher->aniFrames, 0, &self->animator, true, 0);
    self->stoodPlayers  = 0;
    self->activePlayers = 0;

    self->state = PSZLauncher_State_Active;

    PSZLauncher_HandlePlayerCollisions();
    PSZLauncher_HandlePlayerInteractions();
}

void PSZLauncher_HandlePlayerCollisions(void)
{
    RSDK_THIS(PSZLauncher);

    Hitbox hitboxStand;
    hitboxStand.left   = -32;
    hitboxStand.right  = 0;
    hitboxStand.bottom = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        int32 standPos = 31 - CLAMP(abs(player->position.x - self->position.x) >> 16, 0, 31);
        if ((self->direction == FLIP_NONE && player->position.x > self->position.x)
            || (self->direction == FLIP_X && player->position.x < self->position.x))
            standPos = 31;

        hitboxStand.top = -PSZLauncher->heightTable[standPos];
        if ((1 << playerID) & self->stoodPlayers)
            player->position.y += 0x10000;

        if (Player_CheckCollisionPlatform(player, self, &hitboxStand)) {
            self->stoodPlayers |= 1 << playerID;
            player->position.y &= 0xFFFF0000;
        }
        else {
            self->stoodPlayers &= ~(1 << playerID);
        }
    }
}

void PSZLauncher_HandlePlayerInteractions(void)
{
    RSDK_THIS(PSZLauncher);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (Player_CheckCollisionTouch(player, self, &PSZLauncher->hitboxLaunch)) {
            if (!((1 << playerID) & self->activePlayers) && !((1 << playerID) & self->stoodPlayers) && player->velocity.y <= 0) {
                self->activePlayers |= 1 << playerID;
                player->velocity.y = -0x10000 * self->power;
                player->velocity.x = 0;

                if (self->direction)
                    player->position.x = self->position.x - 0x100000;
                else
                    player->position.x = self->position.x + 0x100000;

                player->applyJumpCap     = false;
                player->jumpAbilityState = 0;
                player->collisionMode    = 0;
                player->groundVel        = 0;
                player->state            = Player_State_Air;
                player->onGround         = false;

                RSDK.PlaySfx(Player->sfxRelease, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }
}

void PSZLauncher_State_Active(void)
{
    PSZLauncher_HandlePlayerCollisions();
    PSZLauncher_HandlePlayerInteractions();
}

#if GAME_INCLUDE_EDITOR
void PSZLauncher_EditorDraw(void)
{
    RSDK_THIS(PSZLauncher);
    RSDK.SetSpriteAnimation(PSZLauncher->aniFrames, 0, &self->animator, true, 0);

    PSZLauncher_Draw();
}

void PSZLauncher_EditorLoad(void)
{
    PSZLauncher->aniFrames = RSDK.LoadSpriteAnimation("PSZ1/PSZLauncher.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PSZLauncher, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);
}
#endif

void PSZLauncher_Serialize(void)
{
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, power);
}
