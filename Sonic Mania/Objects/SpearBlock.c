#include "../SonicMania.h"

ObjectSpearBlock *SpearBlock = NULL;

void SpearBlock_Update()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
    SpearBlock_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->state = SpearBlock_State_SetupSpears;
        SpearBlock_Create(NULL);
    }
}

void SpearBlock_LateUpdate() {}

void SpearBlock_StaticUpdate() {}

void SpearBlock_Draw()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, &entity->spearPos, 0);
    RSDK.DrawSprite(&SpearBlock->data, 0, 0);
}

void SpearBlock_Create(void *data)
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    entity->visible          = true;
    entity->drawOrder        = Zone->drawOrderLow;
    entity->spearPos.x       = entity->position.x;
    entity->spearPos.y       = entity->position.y;
    entity->active           = ACTIVE_BOUNDS;
    entity->updateRange.x    = 0x400000;
    entity->updateRange.y    = 0x400000;
    RSDK.SetSpriteAnimation(SpearBlock->spriteIndex, 1, &entity->data, true, 0);
    RSDK.SetSpriteAnimation(SpearBlock->spriteIndex, 0, &SpearBlock->data, true, 0);
    entity->state = SpearBlock_State_SetupSpears;
}

void SpearBlock_StageLoad()
{
    SpearBlock->spriteIndex = RSDK.LoadSpriteAnimation("MTZ/SpearBlock.bin", SCOPE_STAGE);

    SpearBlock->blockHitbox.left        = -16;
    SpearBlock->blockHitbox.top         = -16;
    SpearBlock->blockHitbox.right       = 16;
    SpearBlock->blockHitbox.bottom      = 16;
    SpearBlock->spearHitboxes[0].left   = -4;
    SpearBlock->spearHitboxes[0].top    = -16;
    SpearBlock->spearHitboxes[0].right  = 4;
    SpearBlock->spearHitboxes[0].bottom = 16;
    SpearBlock->spearHitboxes[1].left   = -16;
    SpearBlock->spearHitboxes[1].top    = -4;
    SpearBlock->spearHitboxes[1].right  = 16;
    SpearBlock->spearHitboxes[1].bottom = 4;
    SpearBlock->spearHitboxes[2].left   = -4;
    SpearBlock->spearHitboxes[2].top    = -16;
    SpearBlock->spearHitboxes[2].right  = 4;
    SpearBlock->spearHitboxes[2].bottom = 16;
    SpearBlock->spearHitboxes[3].left   = -16;
    SpearBlock->spearHitboxes[3].top    = -4;
    SpearBlock->spearHitboxes[3].right  = 16;
    SpearBlock->spearHitboxes[3].bottom = 4;

    DebugMode_AddObject(SpearBlock->objectID, SpearBlock_DebugDraw, SpearBlock_DebugSpawn);
}

void SpearBlock_DebugDraw()
{
    RSDK.SetSpriteAnimation(SpearBlock->spriteIndex, 0, &DebugMode->debugData, true, 0);
    RSDK.DrawSprite(&DebugMode->debugData, 0, 0);
}
void SpearBlock_DebugSpawn()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    RSDK.CreateEntity(SpearBlock->objectID, 0, entity->position.x, entity->position.y);
}

void SpearBlock_CheckPlayerCollisions()
{
    EntityPlayer *player     = NULL;
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        Player_CheckCollisionBox(player, entity, &SpearBlock->blockHitbox);
        Vector2 storePos;
        storePos.x         = entity->position.x;
        storePos.y         = entity->position.y;
        entity->position.x = entity->spearPos.x;
        entity->position.y = entity->spearPos.y;

        if (Player_CheckCollisionTouch(player, entity, &SpearBlock->spearHitboxes[entity->data.frameID])) {
            if (player->state != Player_State_Hit && player->state != Player_State_Die && player->state != Player_State_Drown
                && !player->invincibleTimer && player->blinkTimer <= 0) {
                if (player->position.x > entity->position.x)
                    player->velocity.x = 0x20000;
                else
                    player->velocity.x = -0x20000;
                Player_Hit(player);
            }
        }
        entity->position.x = storePos.x;
        entity->position.y = storePos.y;
    }
}

void SpearBlock_State_SetupSpears()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    entity->active           = ACTIVE_NORMAL;
    entity->state            = SpearBlock_State_CheckSpearExtend;
    if (Zone->timer & 0x7F)
        return;
    int frameTimer       = (Zone->timer >> 7) + entity->spearDir;
    entity->timer        = 4;
    entity->state        = SpearBlock_State_ExtendSpears;
    entity->data.frameID = (frameTimer & 3);
}

void SpearBlock_State_CheckSpearExtend()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    if (Zone->timer & 0x7F)
        return;
    int frameTimer       = (Zone->timer >> 7) + entity->spearDir;
    entity->timer        = 4;
    entity->state        = SpearBlock_State_ExtendSpears;
    entity->data.frameID = (frameTimer & 3);
}

void SpearBlock_State_ExtendSpears()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    switch (entity->data.frameID) {
        case FLIP_NONE: entity->spearPos.y -= 0x80000; break;
        case FLIP_X: entity->spearPos.x += 0x80000; break;
        case FLIP_Y: entity->spearPos.y += 0x80000; break;
        case FLIP_XY: entity->spearPos.x -= 0x80000; break;
        default: break;
    }

    entity->timer--;
    if (!entity->timer)
        entity->state = SpearBlock_State_CheckSpearRetract;
}

void SpearBlock_State_CheckSpearRetract()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    if (Zone->timer & 0x3F)
        return;
    entity->timer = 4;
    entity->state = SpearBlock_State_RetractSpears;
}

void SpearBlock_State_RetractSpears()
{
    EntitySpearBlock *entity = (EntitySpearBlock *)RSDK_sceneInfo->entity;
    switch (entity->data.frameID) {
        case FLIP_NONE: entity->spearPos.y += 0x80000; break;
        case FLIP_X: entity->spearPos.x -= 0x80000; break;
        case FLIP_Y: entity->spearPos.y -= 0x80000; break;
        case FLIP_XY: entity->spearPos.x += 0x80000; break;
        default: break;
    }

    entity->timer--;
    if (!entity->timer)
        entity->state = SpearBlock_State_CheckSpearExtend;
}

void SpearBlock_EditorDraw() {}

void SpearBlock_EditorLoad() {}

void SpearBlock_Serialize() { RSDK_EDITABLE_VAR(SpearBlock, VAR_UINT8, spearDir); }
