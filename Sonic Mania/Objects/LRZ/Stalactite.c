#include "SonicMania.h"

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
    self->drawOrder     = Zone->drawOrderLow;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(Stalactite->aniFrames, 0, &self->animator, true, 0);

    if (!data)
        self->state = Stalactite_State_Unknown1;
    else
        self->state = data;
}

void Stalactite_StageLoad(void)
{
    if (RSDK.CheckStageFolder("LRZ1"))
        Stalactite->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Stalactite.bin", SCOPE_STAGE);

    Stalactite->hitbox1.left   = -8;
    Stalactite->hitbox1.top    = -16;
    Stalactite->hitbox1.right  = 8;
    Stalactite->hitbox1.bottom = 16;

    Stalactite->hitbox2.left   = -4;
    Stalactite->hitbox2.top    = -16;
    Stalactite->hitbox2.right  = 4;
    Stalactite->hitbox2.bottom = 256;

    Stalactite->sfxShoot = RSDK.GetSFX("Stage/Shoot1.wav");
}

void Stalactite_State_Unknown1(void)
{
    RSDK_THIS(Stalactite);

    foreach_active(Player, player)
    {
        if (!player->sidekick && Player_CheckCollisionTouch(player, self, &Stalactite->hitbox2)) {
            self->state = Stalactite_State_Unknown5;
        }
    }
}

void Stalactite_State_Unknown2(void)
{
    RSDK_THIS(Stalactite);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Stalactite->hitbox1)) {
#if RETRO_USE_PLUS
            if (Player_CheckMightyShellHit(player, self))
                self->state = Stalactite_State_Unknown5;
            else
#endif
                Player_CheckHit(player, self);
        }
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        self->state = Stalactite_State_Unknown3;
        RSDK.PlaySfx(Stalactite->sfxShoot, false, 255);
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Stalactite_State_Unknown3(void)
{
    RSDK_THIS(Stalactite);

    foreach_active(Player, player) { Player_CheckCollisionBox(player, self, &Stalactite->hitbox1); }
}

void Stalactite_State_Unknown4(void)
{
    RSDK_THIS(Stalactite);

    self->position.x += self->velocity.x;
    self->position.y += self->velocity.y;
    self->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &Stalactite->hitbox1)) {
#if RETRO_USE_PLUS
            if (Player_CheckMightyShellHit(player, self))
                self->state = Stalactite_State_Unknown5;
            else
#endif
                Player_CheckHit(player, self);
        }
    }

    if (!RSDK.CheckOnScreen(self, NULL))
        destroyEntity(self);
}

void Stalactite_State_Unknown5(void)
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

#if RETRO_INCLUDE_EDITOR
void Stalactite_EditorDraw(void) { Stalactite_Draw(); }

void Stalactite_EditorLoad(void) { Stalactite->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Stalactite.bin", SCOPE_STAGE); }
#endif

void Stalactite_Serialize(void) {}
