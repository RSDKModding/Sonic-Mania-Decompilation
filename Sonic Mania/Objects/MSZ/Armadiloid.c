#include "SonicMania.h"

ObjectArmadiloid *Armadiloid;

void Armadiloid_Update(void)
{
    RSDK_THIS(Armadiloid);
    StateMachine_Run(entity->state);
}

void Armadiloid_LateUpdate(void) {}

void Armadiloid_StaticUpdate(void) {}

void Armadiloid_Draw(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.DrawSprite(&entity->animator3, NULL, false);
    RSDK.DrawSprite(&entity->animator2, NULL, false);
    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Armadiloid_Create(void *data)
{
    RSDK_THIS(Armadiloid);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->drawFX        = FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data)
        entity->type = 1;
    if (entity->type) {
        entity->hitbox.left   = -10;
        entity->hitbox.top    = -40;
        entity->hitbox.right  = 10;
        entity->hitbox.bottom = -20;
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 4, &entity->animator1, true, 0);
    }
    else {
        entity->hitbox.left   = -24;
        entity->hitbox.top    = -15;
        entity->hitbox.right  = 24;
        entity->hitbox.bottom = -7;
        entity->velocity.x    = 0x8000;
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 1, &entity->animator2, true, 0);
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 3, &entity->animator3, true, 0);
    }
    entity->state = Armadiloid_State_Setup;
}

void Armadiloid_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MSZ"))
        Armadiloid->aniFrames = RSDK.LoadSpriteAnimation("MSZ/Armadiloid.bin", SCOPE_STAGE);

    DEBUGMODE_ADD_OBJ(Armadiloid);

    Armadiloid->sfxShot = RSDK.GetSFX("Stage/Shot.wav");
}
void Armadiloid_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Armadiloid, NULL, entity->position.x, entity->position.y);
}

void Armadiloid_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Armadiloid_State_Setup(void)
{
    RSDK_THIS(Armadiloid);

    foreach_active(Armadiloid, armadiloid)
    {
        if (armadiloid != entity) {
            if (abs(entity->position.x - armadiloid->position.x) < 0x300000) {
                if (abs(entity->position.y - armadiloid->position.y) < 0x300000) {
                    if (entity->type) {
                        if (armadiloid->type != entity->type) {
                            entity->child = (Entity*)armadiloid;
                            entity->state = Armadiloid_State_Rider;
                            foreach_break;
                        }
                    }
                    else if (armadiloid->type) {
                        entity->parent = (Entity *)armadiloid;
                        entity->state  = Armadiloid_State_PlatformFlying;
                        foreach_break;
                    }
                }
            }
        }
    }
}

void Armadiloid_State_PlatformFlying(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->position.x += entity->velocity.x;
    if (entity->parent) {
        EntityPlayer *player = Player_GetNearestPlayerX();
        if (abs(entity->position.x - player->position.x) < 0x800000) {
            RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 2, &entity->animator2, true, 0);
            entity->state = Armadiloid_PlatformShootDelay;
        }
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox))
            player->position.x += entity->velocity.x;
    }
}

void Armadiloid_PlatformShootDelay(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&entity->animator3);
    entity->position.x += entity->velocity.x;

    if (entity->parent) {
        if (++entity->timer == 12) {
            entity->timer = 0;
            entity->state = Armadiloid_State_PlatformShoot;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox))
                player->position.x += entity->velocity.x;
        }
    }
    else {
        entity->timer = 0;
        entity->state = Armadiloid_State_PlatformFlying;
    }
}

void Armadiloid_State_PlatformShoot(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&entity->animator3);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.x += entity->velocity.x;

    if (entity->animator2.frameID != 4 || entity->timer) {
        if (entity->animator2.frameID == entity->animator2.frameCount - 1) {
            entity->timer = 0;
            RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 2, &entity->animator2, true, 0);
            entity->state = Armadiloid_State_PlatformFlying;
        }
    }
    else {
        entity->timer = 1;
        RSDK.PlaySfx(Armadiloid->sfxShot, false, 255);
        EntityProjectile *projectile = CREATE_ENTITY(Projectile, Projectile_State_Move, entity->position.x - 0x120000, entity->position.y + 0x90000);
        projectile->velocity.x       = -0x18000;
        projectile->drawOrder        = Zone->drawOrderLow;
        projectile->hitbox.left      = -4;
        projectile->hitbox.top       = -4;
        projectile->hitbox.right     = 4;
        projectile->hitbox.bottom    = 4;
        projectile->type             = 7;
        RSDK.SetSpriteAnimation(Armadiloid->aniFrames, 5, &projectile->animator, true, 0);
    }

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox))
            player->position.x += entity->velocity.x;
    }
}

void Armadiloid_State_Rider(void)
{
    RSDK_THIS(Armadiloid);
    RSDK.ProcessAnimation(&entity->animator1);

    EntityArmadiloid *child = (EntityArmadiloid *)entity->child;
    if (child) {
        entity->position.x = child->position.x;
        entity->position.y = child->position.y;
    }

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &entity->hitbox) && Player_CheckBadnikBreak(entity, player, true)) {
            child->parent = NULL;
        }
    }
}

void Armadiloid_EditorDraw(void) {}

void Armadiloid_EditorLoad(void) {}

void Armadiloid_Serialize(void) { RSDK_EDITABLE_VAR(Armadiloid, VAR_ENUM, type); }
