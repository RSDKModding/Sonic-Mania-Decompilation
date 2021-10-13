#include "SonicMania.h"

ObjectProjectile *Projectile;

void Projectile_Update(void)
{
    RSDK_THIS(Projectile);
    StateMachine_Run(entity->state);
    if ((entity->drawFX & FX_ROTATE))
        entity->rotation = (entity->rotation + entity->field_70) & 0x1FF;
    RSDK.ProcessAnimation(&entity->animator);
}

void Projectile_LateUpdate(void) {}

void Projectile_StaticUpdate(void) {}

void Projectile_Draw(void)
{
    RSDK_THIS(Projectile);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Projectile_Create(void *data)
{
    RSDK_THIS(Projectile);
    entity->active  = ACTIVE_NORMAL;
    entity->visible = true;
    entity->state   = (void (*)(void))data;
}

void Projectile_StageLoad(void) {}

void Projectile_Unknown1(void)
{
    RSDK_THIS(Projectile);
    foreach_active(Player, player)
    {
        switch (entity->field_5C) {
            case 1:
                if (entity->field_60) {
                    Player_CheckProjectileHit(player, entity);
                }
                else if (player->shield != SHIELD_FIRE) {
                    Player_CheckHit(player, entity);
                }
                break;
            case 2:
                if (entity->field_60) {
                    Player_CheckProjectileHit(player, entity);
                }
                else if (player->shield != SHIELD_LIGHTNING) {
                    Player_CheckHit(player, entity);
                }
                break;
            case 4:
            case 7: {
                int32 anim = player->playerAnimator.animationID;
                if (entity->field_60
#if RETRO_USE_PLUS
                    || (player->characterID == ID_MIGHTY && (anim == ANI_CROUCH || anim == ANI_JUMP || anim == ANI_SPINDASH || anim == ANI_DROPDASH))
#endif
                ) {
                    if (Player_CheckProjectileHit(player, entity)) {
                        entity->gravityStrength = 0x3800;
                        entity->state           = Projectile_Unknown3;
                    }
                }
                else {
                    Player_CheckHit(player, entity);
                }
                break;
            }
            default: continue;
        }
    }
}

void Projectile_Unknown2(void)
{
    RSDK_THIS(Projectile);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    if (entity->timer2 <= 0)
        Projectile_Unknown1();
    else
        entity->timer2--;

    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
    else {
        if (!entity->timer)
            destroyEntity(entity);
    }
}

void Projectile_Unknown3(void)
{
    RSDK_THIS(Projectile);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravityStrength;
    if (entity->timer2 <= 0)
        Projectile_Unknown1();
    else
        entity->timer2--;

    if (entity->timer <= 0) {
        if (!RSDK.CheckOnScreen(entity, NULL))
            destroyEntity(entity);
    }
    else {
        if (!entity->timer)
            destroyEntity(entity);
    }
}

void Projectile_EditorDraw(void) {}

void Projectile_EditorLoad(void) {}

void Projectile_Serialize(void) {}
