#include "SonicMania.h"

ObjectInvisibleBlock *InvisibleBlock;

void InvisibleBlock_Update(void)
{
    RSDK_THIS(InvisibleBlock);
    foreach_active(Player, player)
    {
        if ((entity->planeFilter <= 0 || player->collisionPlane == (((uint8)entity->planeFilter - 1) & 1)) && (!entity->noChibi || !player->isChibi)) {
            switch (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
                case C_TOP:
                    if (!entity->noCrush)
                        player->collisionFlagV |= 1;
                    break;
                case C_LEFT:
                    if (!entity->noCrush)
                        player->collisionFlagH |= 1;
                    break;
                case C_RIGHT:
                    if (!entity->noCrush)
                        player->collisionFlagH |= 2;
                    break;
                case C_BOTTOM:
                    if (!entity->noCrush)
                        player->collisionFlagV |= 2;
                    break;
                default: break;
            }
        }
    }
    entity->visible = DebugMode->debugActive;
}

void InvisibleBlock_LateUpdate(void) {}

void InvisibleBlock_StaticUpdate(void) {}

void InvisibleBlock_Draw(void) { InvisibleBlock_DrawSprites(); }

void InvisibleBlock_Create(void *data)
{
    RSDK_THIS(InvisibleBlock);
    if (!RSDK_sceneInfo->inEditor) {
        if (entity->timeAttackOnly && globals->gameMode < MODE_TIMEATTACK) {
            destroyEntity(entity);
        }
        entity->visible = false;
        entity->active  = entity->activeNormal ? ACTIVE_NORMAL : ACTIVE_BOUNDS;

        entity->updateRange.x = (entity->width + 5) << 19;
        entity->updateRange.y = (entity->height + 5) << 19;

        entity->hitbox.right  = 8 * entity->width + 8;
        entity->hitbox.left   = -entity->hitbox.right;
        entity->hitbox.bottom = 8 * entity->height + 8;
        entity->hitbox.top    = -entity->hitbox.bottom;
        entity->drawOrder     = Zone->drawOrderHigh;
    }
}

void InvisibleBlock_StageLoad(void)
{
    InvisibleBlock->spriteIndex = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(InvisibleBlock->spriteIndex, 2, &InvisibleBlock->animator, true, 0);
    InvisibleBlock->animator.frameID = 10;
}

void InvisibleBlock_DrawSprites(void)
{
    RSDK_THIS(InvisibleBlock);
    Vector2 drawPos;
    drawPos.x                    = entity->position.x;
    drawPos.y                    = entity->position.y;
    drawPos.x -= entity->width << 19;
    drawPos.y -= entity->height << 19;

    for (int32 y = 0; y <= entity->height; ++y) {
        for (int32 x = 0; x <= entity->width; ++x) {
            RSDK.DrawSprite(&InvisibleBlock->animator, &drawPos, false);
            drawPos.x += 0x100000;
        }
        drawPos.x += -0x100000 - (entity->width << 20);
        drawPos.y += 0x100000;
    }
}

void InvisibleBlock_EditorDraw(void)
{
    RSDK_THIS(InvisibleBlock);
    entity->updateRange.x = (entity->width + 5) << 19;
    entity->updateRange.y = (entity->height + 5) << 19;

    InvisibleBlock_DrawSprites();
}

void InvisibleBlock_EditorLoad(void)
{
    InvisibleBlock->spriteIndex = RSDK.LoadSpriteAnimation("Global/ItemBox.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(InvisibleBlock->spriteIndex, 2, &InvisibleBlock->animator, true, 0);
    InvisibleBlock->animator.frameID = 10;

    RSDK_ACTIVE_VAR(InvisibleBlock, planeFilter);
    RSDK_ENUM_VAR(PLANEFILTER_NONE);
    RSDK_ENUM_VAR(PLANEFILTER_A);
    RSDK_ENUM_VAR(PLANEFILTER_B);
}

void InvisibleBlock_Serialize(void)
{
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_UINT8, width);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_UINT8, height);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_ENUM, planeFilter);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, noCrush);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, activeNormal);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, timeAttackOnly);
    RSDK_EDITABLE_VAR(InvisibleBlock, VAR_BOOL, noChibi);
}
