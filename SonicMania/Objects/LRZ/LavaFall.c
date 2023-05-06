// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: LavaFall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLavaFall *LavaFall;

void LavaFall_Update(void)
{
    RSDK_THIS(LavaFall);

    StateMachine_Run(self->state);
}

void LavaFall_LateUpdate(void) {}

void LavaFall_StaticUpdate(void)
{
    if (LavaFall->lavaSfxTimer) {
        if (!LavaFall->playingLavaSfx) {
            RSDK.PlaySfx(LavaFall->sfxLava, 81870, 0xFF);
            LavaFall->playingLavaSfx = true;
        }

        LavaFall->lavaSfxTimer = 0;
    }
    else if (LavaFall->playingLavaSfx) {
        RSDK.StopSfx(LavaFall->sfxLava);
        LavaFall->playingLavaSfx = false;
    }
}

void LavaFall_Draw(void)
{
    RSDK_THIS(LavaFall);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void LavaFall_Create(void *data)
{
    RSDK_THIS(LavaFall);

    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x1000000;

    if (data) {
        self->active = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(LavaFall->aniFrames, 0, &self->animator, true, 0);
        self->state = LavaFall_State_Lava;
    }
    else {
        self->state = LavaFall_State_Idle;
    }
}

void LavaFall_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        LavaFall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaFall.bin", SCOPE_STAGE);

    LavaFall->hitbox.left   = -32;
    LavaFall->hitbox.top    = -32;
    LavaFall->hitbox.right  = 32;
    LavaFall->hitbox.bottom = 32;

    LavaFall->active = ACTIVE_ALWAYS;

    LavaFall->lavaSfxTimer   = 0;
    LavaFall->playingLavaSfx = false;

    LavaFall->sfxLava = RSDK.GetSfx("Stage/Lava.wav");
}

void LavaFall_State_Idle(void)
{
    RSDK_THIS(LavaFall);

    if (!((Zone->timer + self->intervalOffset) % self->interval)) {
        self->timer  = self->duration;
        self->active = ACTIVE_NORMAL;
        self->state  = LavaFall_State_LavaFall;
    }
}

void LavaFall_State_LavaFall(void)
{
    RSDK_THIS(LavaFall);

    if (--self->timer <= 0) {
        if (self->interval) {
            self->active = ACTIVE_BOUNDS;
            self->state  = LavaFall_State_Idle;
        }
        else {
            destroyEntity(self);
        }
    }

    ++LavaFall->lavaSfxTimer;
    if (++self->animator.timer == 8) {
        self->animator.timer = 0;
        CREATE_ENTITY(LavaFall, INT_TO_VOID(true), self->position.x, self->position.y);
    }
}

void LavaFall_State_Lava(void)
{
    RSDK_THIS(LavaFall);

    self->position.y += 0x70000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &LavaFall->hitbox))
            Player_ElementHurt(player, self, SHIELD_FIRE);
    }

    if (self->onScreen)
        ++LavaFall->lavaSfxTimer;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void LavaFall_EditorDraw(void)
{
    RSDK_THIS(LavaFall);

    RSDK.SetSpriteAnimation(LavaFall->aniFrames, 0, &self->animator, true, 0);

    LavaFall_Draw();
}

void LavaFall_EditorLoad(void) { LavaFall->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/LavaFall.bin", SCOPE_STAGE); }
#endif

void LavaFall_Serialize(void)
{
    RSDK_EDITABLE_VAR(LavaFall, VAR_UINT16, interval);
    RSDK_EDITABLE_VAR(LavaFall, VAR_UINT16, intervalOffset);
    RSDK_EDITABLE_VAR(LavaFall, VAR_UINT16, duration);
}
