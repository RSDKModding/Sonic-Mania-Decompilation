#include "SonicMania.h"

ObjectBlastoid *Blastoid;

void Blastoid_Update(void)
{
    RSDK_THIS(Blastoid);
    StateMachine_Run(entity->state);
}

void Blastoid_LateUpdate(void) {}

void Blastoid_StaticUpdate(void) {}

void Blastoid_Draw(void)
{
    RSDK_THIS(Blastoid);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Blastoid_Create(void *data)
{
    RSDK_THIS(Blastoid);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    if (!RSDK_sceneInfo->inEditor) {
        if (data) {
            entity->active        = ACTIVE_NORMAL;
            entity->updateRange.x = 0x200000;
            entity->updateRange.y = 0x200000;
            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 1, &entity->animator, true, 0);
            entity->state = Blastoid_State_Projectile;
        }
        else {
            entity->active        = ACTIVE_BOUNDS;
            entity->updateRange.x = 0x800000;
            entity->updateRange.y = 0x800000;
            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &entity->animator, true, 1);
            entity->state = Blastoid_State_Setup;
        }
    }
}

void Blastoid_StageLoad(void)
{
    if (RSDK.CheckStageFolder("HCZ"))
        Blastoid->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Blastoid.bin", SCOPE_STAGE);
    Blastoid->hitboxBody.left         = -12;
    Blastoid->hitboxBody.top          = -9;
    Blastoid->hitboxBody.right        = 12;
    Blastoid->hitboxBody.bottom       = 12;
    Blastoid->hitboxProjectile.left   = -3;
    Blastoid->hitboxProjectile.top    = -3;
    Blastoid->hitboxProjectile.right  = 3;
    Blastoid->hitboxProjectile.bottom = 3;
    Blastoid->sfxShot                 = RSDK.GetSFX("Stage/Shot.wav");
    DEBUGMODE_ADD_OBJ(Blastoid);
}

void Blastoid_DebugSpawn(void)
{
    RSDK_THIS(Blastoid);
    CREATE_ENTITY(Blastoid, NULL, entity->position.x, entity->position.y);
}

void Blastoid_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Blastoid_CheckPlayerCollisions(void)
{
    RSDK_THIS(Blastoid);

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikHit(player, entity, &Blastoid->hitboxBody) && Player_CheckBadnikBreak(entity, player, false)) {
            EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, CollapsingPlatform);
            if (platform->objectID == CollapsingPlatform->objectID) {
                platform->active        = ACTIVE_NORMAL;
                platform->collapseDelay = 30;
                platform->playerPos.x   = entity->position.x;
            }
            destroyEntity(entity);
        }
    }
}

void Blastoid_State_Setup(void)
{
    RSDK_THIS(Blastoid);
    RSDK_sceneInfo->entity->active     = ACTIVE_NORMAL;
    EntityCollapsingPlatform *platform = RSDK_GET_ENTITY(RSDK_sceneInfo->entitySlot - 1, CollapsingPlatform);
    if (platform->objectID == CollapsingPlatform->objectID)
        platform->active = ACTIVE_NEVER;
    entity->state = Blastoid_State_Body;
}

void Blastoid_State_Body(void)
{
    RSDK_THIS(Blastoid);

    RSDK.ProcessAnimation(&entity->animator);
    switch (++entity->timer) {
        case 1:
        case 16:
        case 31: {
            EntityBlastoid *projectile = CREATE_ENTITY(Blastoid, intToVoid(true), entity->position.x, entity->position.y);
            if ((entity->direction & 1)) {
                projectile->position.x += 0x100000;
                projectile->velocity.x = 0x20000;
            }
            else {
                projectile->position.x -= 0x100000;
                projectile->velocity.x = -0x20000;
            }
            if ((entity->direction & 2) != 0) {
                projectile->position.y += 0x60000;
                projectile->velocity.y = 0x10000;
            }
            else {
                projectile->position.y -= 0x60000;
                projectile->velocity.y = -0x10000;
            }
            RSDK.SetSpriteAnimation(Blastoid->aniFrames, 0, &entity->animator, true, 0);
            RSDK.PlaySFX(Blastoid->sfxShot, false, 255);
            break;
        }
        case 121: entity->timer = 0; break;
        default: break;
    }
    Blastoid_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, NULL)) {
        entity->timer  = 0;
        entity->active = ACTIVE_BOUNDS;
    }
}

void Blastoid_State_Projectile(void)
{
    RSDK_THIS(Blastoid);
    if (RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL)) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        RSDK.ProcessAnimation(&entity->animator);

        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, Blastoid->hitboxProjectile.top << 13, 4)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, Blastoid->hitboxProjectile.left << 13, 0, 4)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, Blastoid->hitboxProjectile.bottom << 13, 4)
            || RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, Blastoid->hitboxProjectile.right << 13, 0, 4)) {
            destroyEntity(entity);
            return;
        }

        foreach_active(Player, player)
        {
            int shield = player->shield;
            if (Water_GetPlayerBubble(player))
                player->shield = SHIELD_BUBBLE;
            if (Player_CheckCollisionTouch(player, entity, &Blastoid->hitboxProjectile))
                Player_CheckProjectileHit(player, entity);
            player->shield = shield;
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Blastoid_EditorDraw(void) {}

void Blastoid_EditorLoad(void) {}

void Blastoid_Serialize(void) { RSDK_EDITABLE_VAR(Blastoid, VAR_UINT8, direction); }
