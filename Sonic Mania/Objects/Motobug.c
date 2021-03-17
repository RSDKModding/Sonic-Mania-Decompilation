#include "../SonicMania.h"

ObjectMotobug *Motobug;

void Motobug_Update()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void Motobug_LateUpdate() {}

void Motobug_StaticUpdate() {}

void Motobug_Draw()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void Motobug_Create(void *data)
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    entity->visible       = true;
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
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 3, &entity->data, true, 0);
        entity->state = Motobug_State_Smoke;
    }
    else {
        entity->timer = 16;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->data, true, 0);
        entity->state = Motobug_State_Move;
    }
}

void Motobug_StageLoad()
{
    if (RSDK.CheckStageFolder("GHZ")) {
        Motobug->spriteIndex = RSDK.LoadSpriteAnimation("GHZ/Motobug.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Blueprint")) {
        Motobug->spriteIndex = RSDK.LoadSpriteAnimation("Blueprint/Motobug.bin", SCOPE_STAGE);
    }
    Motobug->hitbox.left   = -14;
    Motobug->hitbox.top    = -14;
    Motobug->hitbox.right  = 14;
    Motobug->hitbox.bottom = 14;

    DEBUGMODE_ADD_OBJ(Motobug);
}

void Motobug_DebugDraw()
{
    RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}
void Motobug_DebugSpawn()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    RSDK.CreateEntity(Motobug->objectID, 0, entity->position.x, entity->position.y);
}
void Motobug_CheckOnScreen()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position.x = entity->startPos.x;
        entity->position.y = entity->startPos.y;
        entity->direction  = entity->startDir;
        Motobug_Create(NULL);
    }
}
void Motobug_CheckPlayerCollisions()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    EntityPlayer *player  = NULL;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        if (Player_CheckBadnikHit(player, entity, &Motobug->hitbox))
            Player_CheckBadnikBreak((Entity*)entity, player, true);
    }
}
void Motobug_State_Fall()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    if (entity->wasTurning)
        entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += +0x3800;
    if (RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, 0xF0000, 8)) {
        entity->wasTurning = true;
        entity->velocity.y = 0;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->data, true, 0);
        entity->state = Motobug_State_HandleMove;
        Motobug_State_HandleMove();
    }
    else {
        Motobug_CheckPlayerCollisions();
        Motobug_CheckOnScreen();
    }
}
void Motobug_State_HandleMove()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    entity->position.x += RSDK_sceneInfo->entity->velocity.x;
    if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0, 0xF0000, 8)) {
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 1, &entity->data, true, 0);
        entity->field_5C = 0;

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
        Entity *smoke = (Entity *)RSDK.CreateEntity(Motobug->objectID, (void *)1, entity->position.x, entity->position.y);
        if (entity->direction == FLIP_X)
            smoke->position.x -= 0x140000;
        else
            smoke->position.x += 0x140000;
    }
    RSDK.ProcessAnimation(&entity->data);
    Motobug_CheckPlayerCollisions();
    Motobug_CheckOnScreen();
}
void Motobug_State_Move2()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);

    bool32 collided = false;
    if (entity->direction)
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, -0x10000, 0xF0000, 8);
    else
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0x10000, 0xF0000, 8);
    if (collided) {
        if (entity->data.frameID == entity->data.frameCount - 1) {
            RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->data, true, 0);
            entity->direction ^= FLIP_X;
            entity->velocity.x = -entity->velocity.x;
            entity->state      = Motobug_State_Turn;
            Motobug_State_HandleMove();
        }
        else {
            Motobug_CheckPlayerCollisions();
            Motobug_CheckOnScreen();
        }
    }
    else {
        entity->wasTurning = false;
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->data, true, 0);
        entity->state = Motobug_State_Fall;
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }
}
void Motobug_State_Move()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    entity->active        = ACTIVE_NORMAL;
    entity->velocity.x    = -0x10000;
    entity->velocity.y    = 0;
    entity->state         = Motobug_State_HandleMove;
    Motobug_State_HandleMove();
}
void Motobug_State_Smoke()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
    if (entity->data.frameID == entity->data.frameCount - 1)
        RSDK.ResetEntityPtr(entity, 0, 0);
}
void Motobug_State_Turn()
{
    EntityMotobug *entity = (EntityMotobug *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);

    bool32 collided = false;
    if (entity->direction)
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, -0x10000, 0xF0000, 8);
    else
        collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, 0, 0, 0x10000, 0xF0000, 8);

    if (collided) {
        if (entity->data.frameID == entity->data.frameCount - 1) {
            RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->data, true, 0);
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
        RSDK.SetSpriteAnimation(Motobug->spriteIndex, 0, &entity->data, true, 0);
        entity->state = Motobug_State_Fall;
        entity->direction ^= FLIP_X;
        entity->velocity.x = -entity->velocity.x;
    }
}

void Motobug_EditorDraw() {}

void Motobug_EditorLoad() {}

void Motobug_Serialize() {}
