// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Stalactite Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectStalactite *Stalactite;

void Stalactite_Update(void)
{
    RSDK_THIS(Stalactite);

    StateMachine_Run(self->state);
}

void Stalactite_LateUpdate(void) {}

void Stalactite_StaticUpdate(void) {}

void Stalactite_Draw(void)
{
    RSDK_THIS(Stalactite);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Stalactite_Create(void *data)
{
    RSDK_THIS(Stalactite);

    self->active        = ACTIVE_NORMAL;
    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(Stalactite->aniFrames, 0, &self->animator, true, 0);

    if (!data)
        self->state = Stalactite_State_AwaitPlayer;
    else
        self->state = data;
}

void Stalactite_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("LRZ1"))
        Stalactite->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Stalactite.bin", SCOPE_STAGE);

    Stalactite->hitboxStalactite.left   = -8;
    Stalactite->hitboxStalactite.top    = -16;
    Stalactite->hitboxStalactite.right  = 8;
    Stalactite->hitboxStalactite.bottom = 16;

    Stalactite->hitboxRange.left   = -4;
    Stalactite->hitboxRange.top    = -16;
    Stalactite->hitboxRange.right  = 4;
    Stalactite->hitboxRange.bottom = 256;

    Stalactite->sfxShoot = RSDK.GetSfx("Stage/Shoot1.wav");
}

void Stalactite_State_AwaitPlayer(void)
{
    RSDK_THIS(Stalactite);

    foreach_active(Player, player)
    {
        if (!player->sidekick && Player_CheckCollisionTouch(player, self, &Stalactite->hitboxRange))
            self->state = Stalactite_State_Falling;
    }
}

void Stalactite_State_Falling(void)
{
    RSDK_THIS(Stalactite);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Stalactite->hitboxStalactite)) {
#if MANIA_USE_PLUS
            if (Player_CheckMightyShellHit(player, self, -0x300, -0x400))
                self->state = Stalactite_State_Debris;
            else
#endif
                Player_Hurt(player, self);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        self->state = Stalactite_State_Landed;
        RSDK.PlaySfx(Stalactite->sfxShoot, false, 255);
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Stalactite_State_Landed(void)
{
    RSDK_THIS(Stalactite);

    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &Stalactite->hitboxStalactite); }
}

void Stalactite_State_Falling_Boss(void)
{
    RSDK_THIS(Stalactite);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Stalactite->hitboxStalactite)) {
#if MANIA_USE_PLUS
            if (Player_CheckMightyShellHit(player, self, -0x300, -0x400))
                self->state = Stalactite_State_Debris;
            else
#endif
                Player_Hurt(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Stalactite_State_Debris(void)
{
    RSDK_THIS(Stalactite);

    self->visible ^= true;

    RSDK.ProcessAnimation(&self->animator);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

#if GAME_INCLUDE_EDITOR
void Stalactite_EditorDraw(void) { Stalactite_Draw(); }

void Stalactite_EditorLoad(void) { Stalactite->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Stalactite.bin", SCOPE_STAGE); }
#endif

void Stalactite_Serialize(void) {}
