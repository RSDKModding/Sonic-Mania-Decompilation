#include "SonicMania.h"

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
    self->stateCollide     = Launcher_Unknown1;
    self->state            = Launcher_Unknown2;
}

void Launcher_StageLoad(void) { Launcher->sfxLaunch = RSDK.GetSFX("Stage/Launch.wav"); }

void Launcher_Unknown1(void)
{
    RSDK_THIS(Launcher);
    self->stoodPlayers = 0;
    Hitbox *hitbox       = RSDK.GetHitbox(&self->animator, 0);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, self, hitbox)) {
            self->stoodPlayers |= (1 << RSDK.GetEntityID(player));
            if (self->state == Launcher_Unknown2) {
                self->active = ACTIVE_NORMAL;
                if (self->direction == FLIP_NONE)
                    self->velocity.x = 0x10000;
                else
                    self->velocity.x = -0x10000;
                self->field_CC = 12;
                self->field_D0 = 4;
                RSDK.PlaySfx(Launcher->sfxLaunch, false, 255);
                self->state = Launcher_Unknown3;
            }

            if (self->state == Launcher_Unknown4) {
                player->position.x += self->collisionOffset.x;
            }
            else {
                player->position.x     = self->drawPos.x;
                player->direction      = self->direction;
                player->velocity.x     = 0;
                player->groundVel      = clampVal(self->velocity.x, -0x60000, 0x60000);
                player->pushing        = false;
                player->tileCollisions = true;
                if (player->state != Player_State_Roll)
                    player->state = Player_State_Ground;
            }
        }
        else if (((1 << RSDK.GetEntityID(player)) & self->stoodPlayers) && self->state != Launcher_Unknown4) {
            player->velocity.x = self->velocity.x;
            player->groundVel  = self->velocity.x;
        }
    }
}

void Launcher_Unknown2(void) {}

void Launcher_Unknown3(void)
{
    RSDK_THIS(Launcher);
    self->drawPos.x += self->velocity.x;

    if (--self->field_D0 >= 0) {
        self->velocity.x <<= 1;
    }

    if (--self->field_CC < 0) {
        foreach_active(Player, player)
        {
            if (((1 << RSDK.GetEntityID(player)) & self->stoodPlayers)) {
                player->groundVel  = self->velocity.x;
                player->velocity.x = self->velocity.x;
            }
        }
        self->state = Launcher_Unknown4;
    }
}

void Launcher_Unknown4(void)
{
    RSDK_THIS(Launcher);
    if (self->direction)
        self->drawPos.x += 0x10000;
    else
        self->drawPos.x -= 0x10000;

    if (self->drawPos.x == self->centerPos.x) {
        self->active = ACTIVE_BOUNDS;
        self->state  = Launcher_Unknown2;
    }
}

#if RETRO_INCLUDE_EDITOR
void Launcher_EditorDraw(void) {}

void Launcher_EditorLoad(void) {}
#endif

void Launcher_Serialize(void) { RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT8, direction); }
