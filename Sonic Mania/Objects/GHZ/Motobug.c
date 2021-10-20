#include "SonicMania.h"

ObjectMotobug *Motobug;

void Motobug_Update(void)
{
    RSDK_THIS(Motobug);
    StateMachine_Run(entity->state);
}

void Motobug_LateUpdate(void) {}

void Motobug_StaticUpdate(void) {}

void Motobug_Draw(void)
{
    RSDK_THIS(Motobug);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Motobug_Create(void *data)
{
    RSDK_THIS(Motobug);
    entity->visible = true;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->wasTurning    = true;
    if (data) {
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 3, &entity->animator, true, 0);
        entity->state = Motobug_State_Smoke;
    }
    else {
        entity->timer = 16;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->animator, true, 0);
        entity->state = Motobug_State_Move;
    }
}

void Motobug_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Motobug->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Motobug.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        Motobug->spriteIndex = RSDK.LoadSpriteAnimation("Blueprint/Motobug.bin", SCOPE_STAGE);

    Motobug->hitbox.left   = -14;
    Motobug->hitbox.top    = -14;
    Motobug->hitbox.right  = 14;
    Motobug->hitbox.bottom = 14;

    DEBUGMODE_ADD_OBJ(Motobug);
}

void Motobug_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void Motobug_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);
    CREATE_ENTITY(Motobug, 0, entity->position.x, entity->position.y);
}
void Motobug_CheckOnScreen(void)
{
    RSDK_THIS(Motobug);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Motobug_Create(NULL);
    }
}
void Motobug_CheckPlayerCollisions(void)
{
    RSDK_THIS(Motobug);
    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Motobug->hitbox))
            Player_CheckBadnikBreak(entity, player, true);
    }
}
void Motobug_State_Fall(void)
{
    RSDK_THIS(Motobug);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->wasTurning)
        entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, 0xF0000, 8)) {
        entity->wasTurning = true;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->animator, true, 0);
        entity->state = Motobug_State_HandleMove;
        Motobug_State_HandleMove();
    }
    else {
        Motobug_CheckPlayerCollisions();
        Motobug_CheckOnScreen();
    }
}
void Motobug_State_HandleMove(void)
{
    RSDK_THIS(Motobug);
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, 0xF0000, 8)) {
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 1, &entity->animator, true, 0);
        entity->turnTimer = 0;

        bool32 collided = false;
        if (entity->direction)
            collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, -0x10000, 0xF0000, 8);
        else
            collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0x10000, 0xF0000, 8);

        if (collided)
            entity->state = Motobug_State_Move2;
        else
            entity->state = Motobug_State_Fall;
    }

    if (!--entity->timer) {
        entity->timer = 16;
        EntityMotobug *smoke = CREATE_ENTITY(Motobug, intToVoid(true), entity->position.x, entity->position.y);
        if (entity->direction == FLIP_X)
            smoke->position.x -= 0x140000;
        else
            smoke->position.x += 0x140000;
    }
    RSDK.ProcessAnimation(&entity->animator);
    Motobug_CheckPlayerCollisions();
    Motobug_CheckOnScreen();
}
void Motobug_State_Move2(void)
{
    RSDK_THIS(Motobug);
    RSDK.ProcessAnimation(&entity->animator);

    bool32 collided = false;
    if (entity->direction)
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, -0x10000, 0xF0000, 8);
    else
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0x10000, 0xF0000, 8);
    if (collided) {
        ++entity->turnTimer;
        if (entity->turnTimer == 30) {
            RSDK.SetSpriteAnimation(Motobug->spriteIndex, 2, &entity->animator, true, 0);
            entity->state      = Motobug_State_Turn;
        }
    }
    else {
        entity->wasTurning = false;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->animator, true, 0);
        entity->state = Motobug_State_Fall;
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }
    Motobug_CheckPlayerCollisions();
    Motobug_CheckOnScreen();
}
void Motobug_State_Move(void)
{
    RSDK_THIS(Motobug);
    entity->active     = ACTIVE_NORMAL;
    entity->velocity.x = -0x10000;
    entity->velocity.y = 0;
    entity->state      = Motobug_State_HandleMove;
    Motobug_State_HandleMove();
}
void Motobug_State_Smoke(void)
{
    RSDK_THIS(Motobug);
    RSDK.ProcessAnimation(&entity->animator);
    if (entity->animator.frameID == entity->animator.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}
void Motobug_State_Turn(void)
{
    RSDK_THIS(Motobug);
    RSDK.ProcessAnimation(&entity->animator);

    bool32 collided = false;
    if (entity->direction)
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, -0x10000, 0xF0000, 8);
    else
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0x10000, 0xF0000, 8);

    if (collided) {
        if (entity->animator.frameID == entity->animator.frameCount - 1) {
            RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->animator, true, 0);
            entity->direction ^= FLIP_X;
            entity->velocity.x = -entity->velocity.x;
            entity->state      = Motobug_State_HandleMove;
            Motobug_State_HandleMove();
        }
        else {
            Motobug_CheckPlayerCollisions();
            Motobug_CheckOnScreen();
        }
    }
    else {
        entity->wasTurning = false;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->animator, true, 0);
        entity->state = Motobug_State_Fall;
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }
}

void Motobug_EditorDraw(void) { Motobug_Draw(); }

void Motobug_EditorLoad(void) {
    if (RSDK.CheckStageFolder("GHZ"))
        Motobug->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Motobug.bin", SCOPE_STAGE);
    else if (RSDK.CheckStageFolder("Blueprint"))
        Motobug->spriteIndex = RSDK.LoadSpriteAnimation("Blueprint/Motobug.bin", SCOPE_STAGE);
}

void Motobug_Serialize(void) {}
