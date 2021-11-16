#include "SonicMania.h"

ObjectLauncher *Launcher;

void Launcher_Update(void)
{
    RSDK_THIS(Launcher);
    Platform_Update();
    RSDK.ProcessAnimation(&entity->animator);
}

void Launcher_LateUpdate(void) {}

void Launcher_StaticUpdate(void) {}

void Launcher_Draw(void)
{
    RSDK_THIS(Launcher);
    RSDK.DrawSprite(&entity->animator, &entity->drawPos, false);
}

void Launcher_Create(void *data)
{
    RSDK_THIS(Launcher);
    entity->active = ACTIVE_BOUNDS;
    Platform_Create(NULL);
    RSDK.SetSpriteAnimation(Platform->spriteIndex, 1, &entity->animator, true, 0);
    entity->drawFX           = FX_FLIP;
    entity->animator.frameID = 4;
    entity->stateCollide     = Launcher_Unknown1;
    entity->state            = Launcher_Unknown2;
}

void Launcher_StageLoad(void) { Launcher->sfxLaunch = RSDK.GetSFX("Stage/Launch.wav"); }

void Launcher_Unknown1(void)
{
    RSDK_THIS(Launcher);
    entity->stoodPlayers = 0;
    Hitbox *hitbox       = RSDK.GetHitbox(&entity->animator, 0);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, hitbox)) {
            entity->stoodPlayers |= (1 << RSDK.GetEntityID(player));
            if (entity->state == Launcher_Unknown2) {
                entity->active = ACTIVE_NORMAL;
                if (entity->direction == FLIP_NONE)
                    entity->velocity.x = 0x10000;
                else
                    entity->velocity.x = -0x10000;
                entity->field_CC = 12;
                entity->field_D0 = 4;
                RSDK.PlaySfx(Launcher->sfxLaunch, false, 255);
                entity->state = Launcher_Unknown3;
            }

            if (entity->state == Launcher_Unknown4) {
                player->position.x += entity->collisionOffset.x;
            }
            else {
                player->position.x     = entity->drawPos.x;
                player->direction      = entity->direction;
                player->velocity.x     = 0;
                player->groundVel      = clampVal(entity->velocity.x, -0x60000, 0x60000);
                player->pushing        = false;
                player->tileCollisions = true;
                if (player->state != Player_State_Roll)
                    player->state = Player_State_Ground;
            }
        }
        else if (((1 << RSDK.GetEntityID(player)) & entity->stoodPlayers) && entity->state != Launcher_Unknown4) {
            player->velocity.x = entity->velocity.x;
            player->groundVel  = entity->velocity.x;
        }
    }
}

void Launcher_Unknown2(void) {}

void Launcher_Unknown3(void)
{
    RSDK_THIS(Launcher);
    entity->drawPos.x += entity->velocity.x;

    if (--entity->field_D0 >= 0) {
        entity->velocity.x <<= 1;
    }

    if (--entity->field_CC < 0) {
        foreach_active(Player, player)
        {
            if (((1 << RSDK.GetEntityID(player)) & entity->stoodPlayers)) {
                player->groundVel  = entity->velocity.x;
                player->velocity.x = entity->velocity.x;
            }
        }
        entity->state = Launcher_Unknown4;
    }
}

void Launcher_Unknown4(void)
{
    RSDK_THIS(Launcher);
    if (entity->direction)
        entity->drawPos.x += 0x10000;
    else
        entity->drawPos.x -= 0x10000;

    if (entity->drawPos.x == entity->centerPos.x) {
        entity->active = ACTIVE_BOUNDS;
        entity->state  = Launcher_Unknown2;
    }
}

#if RETRO_INCLUDE_EDITOR
void Launcher_EditorDraw(void) {}

void Launcher_EditorLoad(void) {}
#endif

void Launcher_Serialize(void) { RSDK_EDITABLE_VAR(FoldingPlatform, VAR_UINT8, direction); }
