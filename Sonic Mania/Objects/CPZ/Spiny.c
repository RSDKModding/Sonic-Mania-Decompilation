#include "SonicMania.h"

ObjectSpiny *Spiny;

void Spiny_Update(void)
{
    RSDK_THIS(Spiny);
    StateMachine_Run(entity->state);
}

void Spiny_LateUpdate(void) {}

void Spiny_StaticUpdate(void) {}

void Spiny_Draw(void)
{
    RSDK_THIS(Spiny);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Spiny_Create(void *data)
{
    RSDK_THIS(Spiny);
    entity->visible       = true;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        entity->drawOrder = Zone->drawOrderLow - 1;
        RSDK.SetSpriteAnimation(Spiny->aniFrames, 4, &entity->animator, true, 0);
        entity->state = Spiny_State_Shot;
    }
    else {
        entity->startPos = entity->position;
        entity->startDir = entity->direction;
        entity->drawFX   = FX_FLIP;
        if (!entity->type) {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &entity->animator, true, 0);
            entity->state = Spiny_State_Setup;
            if (!(entity->direction & 1))
                entity->velocity.x = -0x4000;
            else
                entity->velocity.x = 0x4000;
        }
        else {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 2, &entity->animator, true, 0);
            entity->state = Spiny_State_Setup;
            if (!(entity->direction & 2))
                entity->velocity.y = -0x4000;
            else
                entity->velocity.y = 0x4000;
        }
    }
}

void Spiny_StageLoad(void)
{
    Spiny->aniFrames          = RSDK.LoadSpriteAnimation("CPZ/Spiny.bin", SCOPE_STAGE);
    Spiny->hitboxSpiny.left   = -12;
    Spiny->hitboxSpiny.top    = -15;
    Spiny->hitboxSpiny.right  = 12;
    Spiny->hitboxSpiny.bottom = 15;
    Spiny->hitboxRange.left   = -96;
    Spiny->hitboxRange.top    = -256;
    Spiny->hitboxRange.right  = 96;
    Spiny->hitboxRange.bottom = 256;
    Spiny->hitboxShot.left    = -4;
    Spiny->hitboxShot.top     = -4;
    Spiny->hitboxShot.right   = 4;
    Spiny->hitboxShot.bottom  = 4;
    DEBUGMODE_ADD_OBJ(Spiny);
    Spiny->sfxShot = RSDK.GetSFX("Stage/Shot.wav");
}

void Spiny_DebugSpawn(void)
{
    RSDK_THIS(Spiny);
    EntitySpiny *spiny = CREATE_ENTITY(Spiny, NULL, entity->position.x, entity->position.y);
    spiny->direction   = entity->direction;
}

void Spiny_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Spiny_CheckPlayerCollisions(void)
{
    RSDK_THIS(Spiny);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Spiny->hitboxSpiny))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void Spiny_CheckOnScreen(void)
{
    RSDK_THIS(Spiny);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->timer2    = 0;
        entity->timer     = 128;
        entity->direction = entity->startDir;
        Spiny_Create(NULL);
    }
}

void Spiny_State_Setup(void)
{
    RSDK_THIS(Spiny);

    entity->active = ACTIVE_NORMAL;
    if (!entity->type) {
        entity->state = Spiny_State_Floor;
        Spiny_State_Floor();
    }
    else {
        entity->state = Spiny_State_Wall;
        Spiny_State_Wall();
    }
}

void Spiny_State_Floor(void)
{
    RSDK_THIS(Spiny);
    RSDK.ProcessAnimation(&entity->animator);
    if (++entity->timer2 >= 256) {
        int32 velStore   = -entity->velocity.x;
        entity->timer2 = 0;
        entity->direction ^= 1u;
        entity->timer      = 128;
        entity->velocity.x = velStore;
    }

    int32 distance = 0x7FFFFFFF;
    entity->position.x += entity->velocity.x;
    if (entity->timer) {
        entity->timer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Spiny->hitboxRange)) {
                entity->timer = 40;
                entity->state = Spiny_State_Shoot_Floor;
                RSDK.SetSpriteAnimation(Spiny->aniFrames, 1, &entity->animator, true, 0);
                if (abs(player->position.x - entity->position.x) < distance) {
                    distance = abs(player->position.x - entity->position.x);
                    if (player->position.x > entity->position.x)
                        entity->shotSpeed = 0x10000;
                    else
                        entity->shotSpeed = -0x10000;
                }
            }
        }
    }
    Spiny_CheckPlayerCollisions();
    Spiny_CheckOnScreen();
}

void Spiny_State_Shoot_Floor(void)
{
    RSDK_THIS(Spiny);
    RSDK.ProcessAnimation(&entity->animator);
    if (--entity->timer == 20) {
        RSDK.PlaySfx(Spiny->sfxShot, 0, 255);
        EntitySpiny *shot = CREATE_ENTITY(Spiny, intToVoid(true), entity->position.x, entity->position.y);
        shot->velocity.x  = entity->shotSpeed;
        if (!(entity->direction & 2))
            shot->velocity.y = -0x30000;
        else
            shot->velocity.y = 0x20000;
        Spiny_CheckPlayerCollisions();
    }
    else {
        if (entity->timer < 0) {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 0, &entity->animator, true, 0);
            entity->timer = 64;
            entity->state = Spiny_State_Floor;
        }
        Spiny_CheckPlayerCollisions();
    }
    Spiny_CheckOnScreen();
}

void Spiny_State_Wall(void)
{
    RSDK_THIS(Spiny);
    RSDK.ProcessAnimation(&entity->animator);
    if (++entity->timer2 >= 256) {
        entity->timer2 = 0;
        entity->direction ^= 2u;
        entity->timer      = 128;
        entity->velocity.y = -entity->velocity.y;
    }

    entity->position.y += entity->velocity.y;
    if (entity->timer) {
        entity->timer--;
    }
    else {
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Spiny->hitboxRange)) {
                entity->timer = 40;
                entity->state = Spiny_State_Shoot_Wall;
                RSDK.SetSpriteAnimation(Spiny->aniFrames, 3, &entity->animator, true, 0);

                if (!(entity->direction & 1))
                    entity->shotSpeed = -0x30000;
                else
                    entity->shotSpeed = 0x30000;
            }
        }
    }
    Spiny_CheckPlayerCollisions();
    Spiny_CheckOnScreen();
}

void Spiny_State_Shoot_Wall(void)
{
    RSDK_THIS(Spiny);

    RSDK.ProcessAnimation(&entity->animator);
    if (--entity->timer == 20) {
        RSDK.PlaySfx(Spiny->sfxShot, 0, 255);
        CREATE_ENTITY(Spiny, intToVoid(true), entity->position.x, entity->position.y)->velocity.x = entity->shotSpeed;
        Spiny_CheckPlayerCollisions();
    }
    else {
        if (entity->timer < 0) {
            RSDK.SetSpriteAnimation(Spiny->aniFrames, 2, &entity->animator, true, 0);
            entity->timer = 64;
            entity->state = Spiny_State_Wall;
        }
        Spiny_CheckPlayerCollisions();
    }
    Spiny_CheckOnScreen();
}

void Spiny_State_Shot(void)
{
    RSDK_THIS(Spiny);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x2000;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        RSDK.ProcessAnimation(&entity->animator);
        if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0, true)) {
            entity->inkEffect |= INK_ADD;
            entity->alpha = 0x100;
            entity->state = Spiny_State_ShotDisappear;
        }

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Spiny->hitboxShot)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Spiny_State_ShotDisappear(void)
{
    RSDK_THIS(Spiny);
    RSDK.ProcessAnimation(&entity->animator);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->alpha -= 32;
    if (entity->alpha <= 0)
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Spiny_EditorDraw(void) {}

void Spiny_EditorLoad(void) {}
#endif

void Spiny_Serialize(void)
{
    RSDK_EDITABLE_VAR(Spiny, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(Spiny, VAR_UINT8, direction);
}
