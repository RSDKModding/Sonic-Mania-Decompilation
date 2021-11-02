#include "SonicMania.h"

ObjectTurretSwitch *TurretSwitch;

void TurretSwitch_Update(void)
{
    RSDK_THIS(TurretSwitch);

    StateMachine_Run(entity->state);
    RSDK.ProcessAnimation(&entity->animator);
}

void TurretSwitch_LateUpdate(void) {}

void TurretSwitch_StaticUpdate(void) {}

void TurretSwitch_Draw(void)
{
    RSDK_THIS(TurretSwitch);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void TurretSwitch_Create(void *data)
{
    RSDK_THIS(TurretSwitch);

    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = true;
    entity->drawFX        = FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (voidToInt(data) == 1) {
        RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 1, &entity->animator, true, 0);
        entity->state     = TurretSwitch_State_Projectile;
        entity->drawOrder = Zone->drawOrderLow;
    }
    else {
        entity->hitbox.left        = -12;
        entity->hitbox.top         = -12;
        entity->hitbox.right       = 12;
        entity->hitbox.bottom      = 12;
        entity->hitboxRange.left   = -12;
        entity->hitboxRange.top    = -140;
        entity->hitboxRange.right  = 140;
        entity->hitboxRange.bottom = 140;
        entity->state              = TurretSwitch_State_Setup;
        entity->drawOrder          = Zone->drawOrderHigh;
    }
}

void TurretSwitch_StageLoad(void)
{
    TurretSwitch->aniFrames               = RSDK.LoadSpriteAnimation("LRZ1/TurretSwitch.bin", SCOPE_STAGE);
    TurretSwitch->hitboxProjectile.left   = -3;
    TurretSwitch->hitboxProjectile.top    = -3;
    TurretSwitch->hitboxProjectile.right  = 3;
    TurretSwitch->hitboxProjectile.bottom = 3;
    TurretSwitch->sfxShot                 = RSDK.GetSFX("Stage/Shot.wav");
}

void TurretSwitch_CheckPlayerCollisions(void)
{
    RSDK_THIS(TurretSwitch);

    foreach_active(Player, player)
    {
        if (Player_CheckAttacking(player, entity) || player->state == Ice_State_FrozenPlayer) {
            if (Player_CheckCollisionTouch(player, entity, &ItemBox->hitbox)) {
                TurretSwitch_Break(entity, player);
                foreach_break;
            }
        }
        else {
#if RETRO_USE_PLUS
            if (player->characterID != ID_MIGHTY || player->playerAnimator.animationID != ANI_CROUCH)
#endif
                Player_CheckHit(player, entity);
            foreach_break;
        }
    }
}

void TurretSwitch_Break(EntityTurretSwitch *entity, EntityPlayer *player)
{
    player->velocity.y = -(player->velocity.y + 2 * player->gravityStrength);
    RSDK.CreateEntity(Explosion->objectID, 0, entity->position.x, entity->position.y)->drawOrder = Zone->drawOrderHigh;
    RSDK.PlaySfx(ItemBox->sfx_Destroy, false, 255);
    entity->visible   = 0;
    entity->state     = 0;
    entity->activated = true;
    entity->field_74  = true;
    entity->field_64  = true;
    entity->field_68  = true;
    entity->field_70  = true;
}

void TurretSwitch_State_Setup(void)
{
    RSDK_THIS(TurretSwitch);

    RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 0, &entity->animator, true, 0);
    entity->state = TurretSwitch_State_Turret;
}

void TurretSwitch_State_Turret(void)
{
    RSDK_THIS(TurretSwitch);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitboxRange)) {
            if (!entity->timer) {
                EntityTurretSwitch *projectile = CREATE_ENTITY(TurretSwitch, intToVoid(1), entity->position.x, entity->position.y);
                projectile->velocity.y         = 0x20000;
                if (entity->direction == FLIP_NONE) {
                    projectile->position.x += 0x80000;
                    projectile->velocity.x = 0x20000;
                }
                else {
                    projectile->position.x -= 0x80000;
                    projectile->velocity.x = -0x20000;
                }
                projectile->position.y += 0x80000;
                RSDK.PlaySfx(TurretSwitch->sfxShot, false, 255);
                entity->timer = 40;
            }
            foreach_break;
        }
    }

    if (entity->timer > 0)
        entity->timer--;
    TurretSwitch_CheckPlayerCollisions();
}

void TurretSwitch_State_Projectile(void)
{
    RSDK_THIS(TurretSwitch);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL)) {
        destroyEntity(entity);
    }
    else {
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, TurretSwitch->hitboxProjectile.bottom << 13, 4)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, TurretSwitch->hitboxProjectile.right << 13, 0, 4)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, TurretSwitch->hitboxProjectile.top << 13, 4)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, TurretSwitch->hitboxProjectile.left << 13, 0, 4)) {
            destroyEntity(entity);
        }
        else {
            entity->position.x += entity->velocity.x;
            entity->position.y += entity->velocity.y;

            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, entity, &TurretSwitch->hitboxProjectile))
                    Player_CheckProjectileHit(player, entity);
            }
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void TurretSwitch_EditorDraw(void)
{
    RSDK_THIS(TurretSwitch);

    RSDK.SetSpriteAnimation(TurretSwitch->aniFrames, 0, &entity->animator, false, 0);
    TurretSwitch_Draw();
}

void TurretSwitch_EditorLoad(void) { TurretSwitch->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/TurretSwitch.bin", SCOPE_STAGE); }
#endif

void TurretSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(TurretSwitch, VAR_UINT8, tag);
    RSDK_EDITABLE_VAR(TurretSwitch, VAR_UINT8, direction);
}
