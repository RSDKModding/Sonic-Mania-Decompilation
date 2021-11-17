#include "SonicMania.h"

ObjectBomb *Bomb = NULL;

void Bomb_Update(void)
{
    RSDK_THIS(Bomb);
    StateMachine_Run(entity->state);
}

void Bomb_LateUpdate(void) {}

void Bomb_StaticUpdate(void) {}

void Bomb_Draw(void)
{
    RSDK_THIS(Bomb);
    Vector2 drawPos;

    if (entity->state == Bomb_State_Explode) {
        drawPos.x = entity->position.x;
        drawPos.y = entity->fuseOffset + entity->position.y;
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Bomb_Create(void *data)
{
    RSDK_THIS(Bomb);

    entity->visible = true;
    if (entity->planeFilter > 0 && ((entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 4, &entity->animator, true, 0);
        entity->state = Bomb_State_Shrapnel;
    }
    else {
        entity->startDir = entity->direction;
        entity->startPos = entity->position;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x1000;
        else
            entity->velocity.x = 0x1000;
        entity->drawFX |= FX_FLIP;
        entity->timer = 0x600;
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 1, &entity->animator, true, 0);
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 3, &entity->animator2, true, 0);
        entity->state = Bomb_State_Setup;
    }
}

void Bomb_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        Bomb->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Bomb.bin", SCOPE_STAGE);
    Bomb->hitboxHurt.left       = -12;
    Bomb->hitboxHurt.top        = -18;
    Bomb->hitboxHurt.right      = 12;
    Bomb->hitboxHurt.bottom     = 18;
    Bomb->hitboxRange.left      = -96;
    Bomb->hitboxRange.top       = -96;
    Bomb->hitboxRange.right     = 96;
    Bomb->hitboxRange.bottom    = 96;
    Bomb->hitboxShrapnel.left   = -6;
    Bomb->hitboxShrapnel.top    = -6;
    Bomb->hitboxShrapnel.right  = 6;
    Bomb->hitboxShrapnel.bottom = 6;
    Bomb->sfxExplosion          = RSDK.GetSFX("Stage/Explosion.wav");
    DEBUGMODE_ADD_OBJ(Bomb);
}

void Bomb_DebugSpawn(void)
{
    RSDK_THIS(Bomb);
    CREATE_ENTITY(Bomb, NULL, entity->position.x, entity->position.y);
}

void Bomb_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Bomb->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Bomb_CheckOnScreen(void)
{
    RSDK_THIS(Bomb);
    if (!RSDK.CheckOnScreen(SceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Bomb_Create(NULL);
    }
}

void Bomb_CheckPlayerCollisions(void)
{
    RSDK_THIS(Bomb);

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == ((entity->planeFilter - 1) & 1)) {
            if (entity->state != Bomb_State_Explode) {
                if (Player_CheckCollisionTouch(player, entity, &Bomb->hitboxRange)) {
                    RSDK.SetSpriteAnimation(Bomb->aniFrames, 2, &entity->animator, true, 0);
                    entity->timer = 144;
                    entity->state = Bomb_State_Explode;
                }
            }
            if (Player_CheckCollisionTouch(player, entity, &Bomb->hitboxHurt)) {
                Player_CheckHit(player, entity);
            }
        }
    }
}

void Bomb_State_Setup(void)
{
    RSDK_THIS(Bomb);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Bomb_State_Walk;
    Bomb_State_Walk();
}

void Bomb_State_Walk(void)
{
    RSDK_THIS(Bomb);
    entity->position.x += entity->velocity.x;
    if (!--entity->timer) {
        entity->timer = 180;
        entity->state = Bomb_State_Idle;
    }
    else {
        bool32 flag = false;
        if ((entity->direction & 2))
            flag = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_RWALL, 0, 0, -0x100000, 2);
        else
            flag = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 2);
        if (!flag) {
            entity->timer = 180;
            RSDK.SetSpriteAnimation(Bomb->aniFrames, 0, &entity->animator, true, 0);
            entity->state = Bomb_State_Idle;
        }
    }

    RSDK.ProcessAnimation(&entity->animator);
    Bomb_CheckPlayerCollisions();
    Bomb_CheckOnScreen();
}

void Bomb_State_Idle(void)
{
    RSDK_THIS(Bomb);
    if (!--entity->timer) {
        entity->direction ^= 1;
        entity->velocity.x = -entity->velocity.x;
        entity->timer      = 0x600;
        RSDK.SetSpriteAnimation(Bomb->aniFrames, 1, &entity->animator, true, 0);
        entity->state = Bomb_State_Walk;
    }

    RSDK.ProcessAnimation(&entity->animator);
    Bomb_CheckPlayerCollisions();
    Bomb_CheckOnScreen();
}

void Bomb_State_Explode(void)
{
    RSDK_THIS(Bomb);
    if ((entity->direction & 2))
        entity->fuseOffset -= 0x1000;
    else
        entity->fuseOffset += 0x1000;
    if (--entity->timer > 0) {
        RSDK.ProcessAnimation(&entity->animator);
        RSDK.ProcessAnimation(&entity->animator2);
        Bomb_CheckPlayerCollisions();
        Bomb_CheckOnScreen();
    }
    else {
        RSDK.PlaySfx(Bomb->sfxExplosion, false, 255);
        EntityBomb *debris  = CREATE_ENTITY(Bomb, intToVoid(true), entity->position.x, entity->position.y);
        debris->velocity.x  = -0x20000;
        debris->velocity.y  = -0x30000;
        debris->planeFilter = entity->planeFilter;
        debris->drawOrder   = entity->drawOrder;

        debris              = CREATE_ENTITY(Bomb, intToVoid(true), entity->position.x, entity->position.y);
        debris->velocity.x  = -0x10000;
        debris->velocity.y  = -0x20000;
        debris->planeFilter = entity->planeFilter;
        debris->drawOrder   = entity->drawOrder;

        debris              = CREATE_ENTITY(Bomb, intToVoid(true), entity->position.x, entity->position.y);
        debris->velocity.x  = 0x20000;
        debris->velocity.y  = -0x30000;
        debris->planeFilter = entity->planeFilter;
        debris->drawOrder   = entity->drawOrder;

        debris              = CREATE_ENTITY(Bomb, intToVoid(true), entity->position.x, entity->position.y);
        debris->velocity.x  = 0x10000;
        debris->velocity.y  = -0x20000;
        debris->planeFilter = entity->planeFilter;
        debris->drawOrder   = entity->drawOrder;

        EntityExplosion *explosion = CREATE_ENTITY(Explosion, intToVoid(1), entity->position.x, entity->position.y);
        explosion->planeFilter     = entity->planeFilter;
        explosion->drawOrder       = entity->drawOrder + 1;

        destroyEntity(entity);
    }
}

void Bomb_State_Shrapnel(void)
{
    RSDK_THIS(Bomb);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x1800;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        RSDK.ProcessAnimation(&entity->animator);

        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == ((entity->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, entity, &Bomb->hitboxShrapnel)) {
                    Player_CheckProjectileHit(player, entity);
                }
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void Bomb_EditorDraw(void)
{
    RSDK_THIS(Bomb);
    Bomb_Draw();
}

void Bomb_EditorLoad(void)
{
    Bomb->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Bomb.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Bomb, planeFilter);
    RSDK_ENUM_VAR("No Filter", PLANEFILTER_NONE);
    RSDK_ENUM_VAR("Plane A", PLANEFILTER_A);
    RSDK_ENUM_VAR("Plane B", PLANEFILTER_B);

    RSDK_ACTIVE_VAR(Bomb, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void Bomb_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bomb, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Bomb, VAR_ENUM, planeFilter);
}
