#include "SonicMania.h"

ObjectBlaster *Blaster;

void Blaster_Update(void)
{
    RSDK_THIS(Blaster);
    StateMachine_Run(entity->state);
}

void Blaster_LateUpdate(void) {}

void Blaster_StaticUpdate(void) {}

void Blaster_Draw(void)
{
    RSDK_THIS(Blaster);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Blaster_Create(void *data)
{
    RSDK_THIS(Blaster);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        if (data == intToVoid(1)) {
            entity->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 4, &entity->animator, true, 0);
            entity->state = Blaster_State_Unknown5;
        }
        else if (data == intToVoid(2)) {
            entity->active = ACTIVE_NORMAL;
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 5, &entity->animator, true, 0);
            entity->state = Blaster_State_Unknown8;
        }
    }
    else {
        entity->startPos = entity->position;
        entity->startDir = entity->direction;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &entity->animator, true, 0);
        entity->state = Blaster_State_Setup;
    }
}

void Blaster_StageLoad(void)
{
    if (RSDK.CheckStageFolder("FBZ"))
        Blaster->aniFrames = RSDK.LoadSpriteAnimation("FBZ/Blaster.bin", SCOPE_STAGE);
    Blaster->hitboxBadnik.left       = -16;
    Blaster->hitboxBadnik.top        = -8;
    Blaster->hitboxBadnik.right      = 24;
    Blaster->hitboxBadnik.bottom     = 16;
    Blaster->hitboxRange.left        = -128;
    Blaster->hitboxRange.top         = -128;
    Blaster->hitboxRange.right       = 0;
    Blaster->hitboxRange.bottom      = 0;
    Blaster->hitboxProjectile.left   = -4;
    Blaster->hitboxProjectile.top    = -4;
    Blaster->hitboxProjectile.right  = 4;
    Blaster->hitboxProjectile.bottom = 4;

    DEBUGMODE_ADD_OBJ(Blaster);
}

void Blaster_DebugSpawn(void)
{
    RSDK_THIS(Blaster);
    CREATE_ENTITY(Blaster, NULL, entity->position.x, entity->position.y);
}

void Blaster_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Blaster_HandlePlayerInteractions(void)
{
    RSDK_THIS(Blaster);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Blaster->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
        if (entity->state != Blaster_State_Unknown3 && entity->state != Blaster_State_Unknown7 && entity->animator.animationID != 3) {
            if (Player_CheckCollisionTouch(player, entity, &Blaster->hitboxRange)) {
                entity->timer2 = 0;
                RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &entity->animator, true, 0);
                entity->state = Blaster_State_Unknown3;
            }
        }
    }
}

void Blaster_CheckOnScreen(void)
{
    RSDK_THIS(Blaster);
    if (!RSDK.CheckOnScreen(SceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Blaster_Create(NULL);
    }
}

void Blaster_State_Setup(void)
{
    RSDK_THIS(Blaster);
    entity->active = ACTIVE_NORMAL;
    if (entity->direction == FLIP_NONE)
        entity->velocity.x = -0x8000;
    else
        entity->velocity.x = 0x8000;
    entity->velocity.y = 0;
    entity->timer      = 256;
    entity->state      = Blaster_State_Unknown1;
    Blaster_State_Unknown1();
}

void Blaster_State_Unknown1(void)
{
    RSDK_THIS(Blaster);

    entity->position.x += entity->velocity.x;
    RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 8);
    if (!--entity->timer) {
        entity->state = Blaster_State_Unknown2;
        entity->timer = 30;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &entity->animator, true, 0);
    }
    else if (entity->velocity.x < 0 && !RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, -0xE0000, 0x140000, false)) {
        entity->state = Blaster_State_Unknown2;
        entity->timer = 30;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &entity->animator, true, 0);
    }
    else if (entity->velocity.x > 0 && !RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0xE0000, 0x140000, false)) {
        entity->state = Blaster_State_Unknown2;
        entity->timer = 30;
        RSDK.SetSpriteAnimation(Blaster->aniFrames, 0, &entity->animator, true, 0);
    }

    RSDK.ProcessAnimation(&entity->animator);
    Blaster_HandlePlayerInteractions();
    Blaster_CheckOnScreen();
}

void Blaster_State_Unknown2(void)
{
    RSDK_THIS(Blaster);

    RSDK.ProcessAnimation(&entity->animator);
    Blaster_HandlePlayerInteractions();
    Blaster_CheckOnScreen();

    switch (--entity->timer) {
        case 15: RSDK.SetSpriteAnimation(Blaster->aniFrames, 3, &entity->animator, true, 0); break;
        case 7:
            entity->direction ^= 1;
            entity->velocity.x = -entity->velocity.x;
            break;
        case 0:
            entity->state = Blaster_State_Unknown1;
            entity->timer = 512;
            Blaster_State_Unknown1();
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &entity->animator, true, 0);
            break;
    }
}

void Blaster_State_Unknown3(void)
{
    RSDK_THIS(Blaster);
    switch (++entity->timer2) {
        case 18: {
            EntityBlaster *projectile = NULL;
            if (entity->direction) {
                projectile             = CREATE_ENTITY(Blaster, intToVoid(1), entity->position.x + 0x170000, entity->position.y - 0x150000);
                projectile->velocity.x = 0x20000;
            }
            else {
                projectile             = CREATE_ENTITY(Blaster, intToVoid(1), entity->position.x - 0x170000, entity->position.y - 0x150000);
                projectile->velocity.x = -0x20000;
            }
            projectile->velocity.y = -0x48000;
            break;
        }
        case 20: RSDK.SetSpriteAnimation(Blaster->aniFrames, 2, &entity->animator, true, 0); break;
        case 24: {
            EntityBlaster *projectile = NULL;
            if (entity->direction) {
                projectile             = CREATE_ENTITY(Blaster, intToVoid(2), entity->position.x + 0x60000, entity->position.y - 0x30000);
                projectile->velocity.x = -0x10000;
            }
            else {
                projectile             = CREATE_ENTITY(Blaster, intToVoid(2), entity->position.x - 0x60000, entity->position.y - 0x30000);
                projectile->velocity.x = 0x10000;
            }
            projectile->velocity.y = -0x20000;
            break;
        }
        case 60:
            RSDK.SetSpriteAnimation(Blaster->aniFrames, 1, &entity->animator, true, 0);
            entity->state = Blaster_State_Unknown1;
            entity->timer = 512;
            break;
        default: break;
    }
    RSDK.ProcessAnimation(&entity->animator);
    Blaster_HandlePlayerInteractions();
    Blaster_CheckOnScreen();
}

void Blaster_State_MagnetAttract(void)
{
    RSDK_THIS(Blaster);

    entity->position.y += entity->velocity.y;
    entity->velocity.y -= 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, -0xC0000, 8))
        entity->velocity.y = 0;
    Blaster_HandlePlayerInteractions();

    if (RSDK.CheckOnScreen(SceneInfo->entity, NULL)) {
        entity->state = Blaster_State_Unknown4;
    }
    else {
        Blaster_CheckOnScreen();
        entity->state = Blaster_State_Unknown4;
    }
}

void Blaster_State_Unknown4(void)
{
    RSDK_THIS(Blaster);

    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    Blaster_HandlePlayerInteractions();
    Blaster_CheckOnScreen();

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        entity->velocity.y = 0;
        entity->state      = Blaster_State_Unknown1;
    }
    else {
        entity->state = Blaster_State_Unknown4;
    }
}

void Blaster_State_Unknown5(void)
{
    RSDK_THIS(Blaster);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == 2)
        entity->state = Blaster_State_Unknown6;
}

void Blaster_State_Unknown6(void)
{
    RSDK_THIS(Blaster);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        RSDK.ProcessAnimation(&entity->animator);
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &Blaster->hitboxProjectile)) {
                Player_CheckProjectileHit(player, entity);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Blaster_State_Unknown7(void)
{
    RSDK_THIS(Blaster);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x100000, 8)) {
        entity->startPos.x = entity->position.x;
        entity->startPos.y = entity->position.y;
        entity->direction  = entity->direction;
        entity->velocity.y = 0;
        entity->state      = Blaster_State_Setup;
    }
    Blaster_HandlePlayerInteractions();
}

void Blaster_State_Unknown8(void)
{
    RSDK_THIS(Blaster);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    RSDK.ProcessAnimation(&entity->animator);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
        destroyEntity(entity);
}

#if RETRO_INCLUDE_EDITOR
void Blaster_EditorDraw(void) {}

void Blaster_EditorLoad(void) {}
#endif

void Blaster_Serialize(void) { RSDK_EDITABLE_VAR(Blaster, VAR_UINT8, direction); }
