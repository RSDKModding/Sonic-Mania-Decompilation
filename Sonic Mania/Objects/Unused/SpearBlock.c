#include "SonicMania.h"

ObjectSpearBlock *SpearBlock = NULL;

void SpearBlock_Update(void)
{
    RSDK_THIS(SpearBlock);
    StateMachine_Run(entity->state);
    SpearBlock_CheckPlayerCollisions();

    if (!RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->state = SpearBlock_State_SetupSpears;
        SpearBlock_Create(NULL);
    }
}

void SpearBlock_LateUpdate(void) {}

void SpearBlock_StaticUpdate(void) {}

void SpearBlock_Draw(void)
{
    RSDK_THIS(SpearBlock);
    RSDK.DrawSprite(&entity->animator, &entity->spearPos, false);
    RSDK.DrawSprite(&SpearBlock->animator, NULL, false);
}

void SpearBlock_Create(void *data)
{
    RSDK_THIS(SpearBlock);
    entity->visible          = true;
    entity->drawOrder        = Zone->drawOrderLow;
    entity->spearPos.x       = entity->position.x;
    entity->spearPos.y       = entity->position.y;
    entity->active           = ACTIVE_BOUNDS;
    entity->updateRange.x    = 0x400000;
    entity->updateRange.y    = 0x400000;
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 1, &entity->animator, true, 0);
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 0, &SpearBlock->animator, true, 0);
    entity->state = SpearBlock_State_SetupSpears;
}

void SpearBlock_StageLoad(void)
{
    SpearBlock->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/SpearBlock.bin", SCOPE_STAGE);

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

void SpearBlock_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(SpearBlock->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}
void SpearBlock_DebugSpawn(void)
{
    RSDK_THIS(SpearBlock);
    RSDK.CreateEntity(SpearBlock->objectID, NULL, entity->position.x, entity->position.y);
}

void SpearBlock_CheckPlayerCollisions(void)
{
    RSDK_THIS(SpearBlock);
    foreach_active(Player, player) {
        Player_CheckCollisionBox(player, entity, &SpearBlock->blockHitbox);
        Vector2 storePos;
        storePos.x         = entity->position.x;
        storePos.y         = entity->position.y;
        entity->position.x = entity->spearPos.x;
        entity->position.y = entity->spearPos.y;

        if (Player_CheckCollisionTouch(player, entity, &SpearBlock->spearHitboxes[entity->animator.frameID])) {
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

void SpearBlock_State_SetupSpears(void)
{
    RSDK_THIS(SpearBlock);
    entity->active           = ACTIVE_NORMAL;
    entity->state            = SpearBlock_State_CheckSpearExtend;
    if (!(Zone->timer & 0x7F)) {
        int32 frameTimer       = (Zone->timer >> 7) + entity->spearDir;
        entity->timer        = 4;
        entity->state        = SpearBlock_State_ExtendSpears;
        entity->animator.frameID = (frameTimer & 3);
    }
}

void SpearBlock_State_CheckSpearExtend(void)
{
    RSDK_THIS(SpearBlock);
    if (!(Zone->timer & 0x7F)) {
        int32 frameTimer       = (Zone->timer >> 7) + entity->spearDir;
        entity->timer        = 4;
        entity->state        = SpearBlock_State_ExtendSpears;
        entity->animator.frameID = (frameTimer & 3);
    }
}

void SpearBlock_State_ExtendSpears(void)
{
    RSDK_THIS(SpearBlock);
    switch (entity->animator.frameID) {
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

void SpearBlock_State_CheckSpearRetract(void)
{
    RSDK_THIS(SpearBlock);
    if (!(Zone->timer & 0x3F)) {
        entity->timer = 4;
        entity->state = SpearBlock_State_RetractSpears;
    }
}

void SpearBlock_State_RetractSpears(void)
{
    RSDK_THIS(SpearBlock);
    switch (entity->animator.frameID) {
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

#if RETRO_INCLUDE_EDITOR
void SpearBlock_EditorDraw(void) {}

void SpearBlock_EditorLoad(void) {}
#endif

void SpearBlock_Serialize(void) { RSDK_EDITABLE_VAR(SpearBlock, VAR_UINT8, spearDir); }
