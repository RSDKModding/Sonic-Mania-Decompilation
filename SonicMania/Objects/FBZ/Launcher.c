// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Launcher Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLauncher *Launcher;

void Launcher_Update(void)
{
    RSDK_THIS(Launcher);

    Platform_Update();

    RSDK.ProcessAnimation(&self->animator);
}

void Launcher_LateUpdate(void) {}

void Launcher_StaticUpdate(void) {}

void Launcher_Draw(void)
{
    RSDK_THIS(Launcher);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void Launcher_Create(void *data)
{
    RSDK_THIS(Launcher);

    self->active = ACTIVE_BOUNDS;
    Platform_Create(NULL);

    RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
    self->drawFX           = FX_FLIP;
    self->animator.frameID = 4;
    self->stateCollide     = Launcher_Collide_Normal;
    self->state            = Launcher_State_Idle;
}

void Launcher_StageLoad(void) { Launcher->sfxLaunch = RSDK.GetSfx("Stage/Launch.wav"); }

void Launcher_Collide_Normal(void)
{
    RSDK_THIS(Launcher);
    int32 stoodPlayers = self->stoodPlayers;
    self->stoodPlayers = 0;
    Hitbox *hitbox     = RSDK.GetHitbox(&self->animator, 0);

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntitySlot(player);

        if (Player_CheckCollisionPlatform(player, self, hitbox)) {
            self->stoodPlayers |= 1 << playerID;

            if (self->state == Launcher_State_Idle) {
                self->active       = ACTIVE_NORMAL;
                self->velocity.x   = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
                self->releaseDelay = 12;
                self->accelTimer   = 4;
                RSDK.PlaySfx(Launcher->sfxLaunch, false, 255);
                self->state = Launcher_State_HandleLaunch;
            }

            if (self->state == Launcher_State_ReturnToStart) {
                player->position.x += self->collisionOffset.x;
            }
            else {
                player->position.x     = self->drawPos.x;
                player->direction      = self->direction;
                player->velocity.x     = 0;
                player->groundVel      = CLAMP(self->velocity.x, -0x60000, 0x60000);
                player->pushing        = false;
                player->tileCollisions = TILECOLLISION_DOWN;
                if (player->state != Player_State_Roll)
                    player->state = Player_State_Ground;
            }
        }
        else if (((1 << playerID) & stoodPlayers) && self->state != Launcher_State_ReturnToStart) {
            player->velocity.x = self->velocity.x;
            player->groundVel  = self->velocity.x;
        }
    }
}

void Launcher_State_Idle(void) {}

void Launcher_State_HandleLaunch(void)
{
    RSDK_THIS(Launcher);

    self->drawPos.x += self->velocity.x;

    if (--self->accelTimer >= 0)
        self->velocity.x <<= 1;

    if (--self->releaseDelay < 0) {
        foreach_active(Player, player)
        {
            if (((1 << RSDK.GetEntitySlot(player)) & self->stoodPlayers)) {
                player->groundVel  = self->velocity.x;
                player->velocity.x = self->velocity.x;
            }
        }

        self->state = Launcher_State_ReturnToStart;
    }
}

void Launcher_State_ReturnToStart(void)
{
    RSDK_THIS(Launcher);

    self->drawPos.x += self->direction ? 0x10000 : -0x10000;
    if (self->drawPos.x == self->centerPos.x) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Launcher_State_Idle;
    }
}

#if GAME_INCLUDE_EDITOR
void Launcher_EditorDraw(void)
{
    RSDK_THIS(Launcher);

    RSDK.SetSpriteAnimation(Platform->aniFrames, 1, &self->animator, true, 0);
    self->drawFX           = FX_FLIP;
    self->animator.frameID = 4;
    self->drawPos          = self->position;

    Launcher_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->velocity.x   = self->direction == FLIP_NONE ? 0x10000 : -0x10000;
        self->releaseDelay = 12;

        self->accelTimer = 4;
        while (--self->releaseDelay >= 0) {
            self->drawPos.x += self->velocity.x;

            if (--self->accelTimer >= 0)
                self->velocity.x <<= 1;
        }

        self->inkEffect = INK_BLEND;
        Launcher_Draw();
        self->inkEffect = INK_NONE;

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->drawPos.x, self->drawPos.y, 0x00FF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Launcher_EditorLoad(void)
{

    RSDK_ACTIVE_VAR(Launcher, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void Launcher_Serialize(void) { RSDK_EDITABLE_VAR(Launcher, VAR_UINT8, direction); }
