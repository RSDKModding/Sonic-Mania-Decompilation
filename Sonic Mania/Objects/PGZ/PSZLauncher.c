// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PSZLauncher Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

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
    self->drawOrder = Zone->drawOrderHigh - 1;
    self->drawFX    = FX_FLIP;
    if (SceneInfo->inEditor && !self->power)
        self->power = 10;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->state         = PSZLauncher_State_Setup;
}

void PSZLauncher_StageLoad(void)
{
    PSZLauncher->aniFrames     = RSDK.LoadSpriteAnimation("PSZ1/PSZLauncher.bin", SCOPE_STAGE);
    PSZLauncher->hitbox.left   = -32;
    PSZLauncher->hitbox.top    = 0;
    PSZLauncher->hitbox.right  = 32;
    PSZLauncher->hitbox.bottom = 1;
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
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void PSZLauncher_State_Setup(void)
{
    RSDK_THIS(PSZLauncher);
    RSDK.SetSpriteAnimation(PSZLauncher->aniFrames, 0, &self->animator, true, 0);
    self->activePlayers  = 0;
    self->activePlayers2 = 0;
    self->state          = PSZLauncher_Unknown6;
    PSZLauncher_Unknown3();
    PSZLauncher_Unknown4();
}

void PSZLauncher_Unknown3(void)
{
    RSDK_THIS(PSZLauncher);

    Hitbox hitbox;
    hitbox.left   = -32;
    hitbox.right  = 0;
    hitbox.bottom = 0;
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        int32 pos = 31 - clampVal(abs(player->position.x - self->position.x) >> 16, 0, 31);
        if ((self->direction == FLIP_NONE && player->position.x > self->position.x)
            || (self->direction == FLIP_X && player->position.x < self->position.x))
            pos = 31;

        hitbox.top = -PSZLauncher->heights[pos];
        if ((1 << playerID) & self->activePlayers)
            player->position.y += 0x10000;

        if (Player_CheckCollisionPlatform(player, self, &hitbox) == 1) {
            self->activePlayers |= (1 << playerID);
            player->position.y &= 0xFFFF0000;
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }
}

void PSZLauncher_Unknown4(void)
{
    RSDK_THIS(PSZLauncher);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        if (Player_CheckCollisionTouch(player, self, &PSZLauncher->hitbox)) {
            if (!((1 << playerID) & self->activePlayers2) && !((1 << playerID) & self->activePlayers) && player->velocity.y <= 0) {
                self->activePlayers2 |= (1 << playerID);
                player->velocity.y = -0x10000 * self->power;
                player->velocity.x = 0;
                if (self->direction)
                    player->position.x = self->position.x - 0x100000;
                else
                    player->position.x = self->position.x + 0x100000;
                player->jumpAbility      = 0;
                player->jumpAbilityTimer = 0;
                player->collisionMode    = 0;
                player->groundVel        = 0;
                player->state            = Player_State_Air;
                player->onGround         = false;
                RSDK.PlaySfx(Player->sfxRelease, false, 255);
                RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            }
        }
        else {
            self->activePlayers2 &= ~(1 << playerID);
        }
    }
}

void PSZLauncher_Unknown6(void)
{
    PSZLauncher_Unknown3();
    PSZLauncher_Unknown4();
}

#if RETRO_INCLUDE_EDITOR
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
    RSDK_ENUM_VAR("Up", FLIP_NONE);
    RSDK_ENUM_VAR("Down", FLIP_X);
}
#endif

void PSZLauncher_Serialize(void)
{
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PSZLauncher, VAR_UINT8, power);
}
