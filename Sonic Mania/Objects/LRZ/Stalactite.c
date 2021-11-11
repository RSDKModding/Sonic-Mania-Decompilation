#include "SonicMania.h"

ObjectStalactite *Stalactite;

void Stalactite_Update(void)
{
    RSDK_THIS(Stalactite);
    StateMachine_Run(entity->state);
}

void Stalactite_LateUpdate(void) {}

void Stalactite_StaticUpdate(void) {}

void Stalactite_Draw(void)
{
    RSDK_THIS(Stalactite);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Stalactite_Create(void *data)
{
    RSDK_THIS(Stalactite);

    entity->active        = ACTIVE_NORMAL;
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    RSDK.SetSpriteAnimation(Stalactite->aniFrames, 0, &entity->animator, true, 0);

    if (!data)
        entity->state = Stalactite_State_Unknown1;
    else
        entity->state = data;
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
        if (!player->sidekick && Player_CheckCollisionTouch(player, entity, &Stalactite->hitbox2)) {
            entity->state = Stalactite_State_Unknown5;
        }
    }
}

void Stalactite_State_Unknown2(void)
{
    RSDK_THIS(Stalactite);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Stalactite->hitbox1)) {
#if RETRO_USE_PLUS
            if (Player_CheckMightyShellHit(player, entity))
                entity->state = Stalactite_State_Unknown5;
#endif
            else
                Player_CheckHit(player, entity);
        }
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, true)) {
        entity->state = Stalactite_State_Unknown3;
        RSDK.PlaySfx(Stalactite->sfxShoot, false, 255);
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void Stalactite_State_Unknown3(void)
{
    RSDK_THIS(Stalactite);

    foreach_active(Player, player) { Player_CheckCollisionBox(player, entity, &Stalactite->hitbox1); }
}

void Stalactite_State_Unknown4(void)
{
    RSDK_THIS(Stalactite);

    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Stalactite->hitbox1)) {
#if RETRO_USE_PLUS
            if (Player_CheckMightyShellHit(player, entity))
                entity->state = Stalactite_State_Unknown5;
#endif
            else
                Player_CheckHit(player, entity);
        }
    }

    if (!RSDK.CheckOnScreen(entity, NULL))
        destroyEntity(entity);
}

void Stalactite_State_Unknown5(void)
{
    RSDK_THIS(Stalactite);

    entity->visible ^= true;
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Stalactite_EditorDraw(void) { Stalactite_Draw(); }

void Stalactite_EditorLoad(void) { Stalactite->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Stalactite.bin", SCOPE_STAGE); }
#endif

void Stalactite_Serialize(void) {}
