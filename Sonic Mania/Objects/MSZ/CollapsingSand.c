#include "SonicMania.h"

ObjectCollapsingSand *CollapsingSand;

void CollapsingSand_Update(void)
{
    RSDK_THIS(CollapsingSand);
    entity->visible = DebugMode->debugActive;
    StateMachine_Run(entity->state);
}

void CollapsingSand_LateUpdate(void) {}

void CollapsingSand_StaticUpdate(void) {}

void CollapsingSand_Draw(void)
{
    RSDK_THIS(CollapsingSand);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y - 0x10000, 0xE0E0E0u, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xE0E0E0u, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + entity->size.y, 0xE0E0E0u, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xE0E0E0u, 0, INK_NONE,
                  false);
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);
    drawPos.x += entity->size.x;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);
    drawPos.y += entity->size.y;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);
    drawPos.x -= entity->size.x;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&CollapsingSand->animator, &drawPos, false);
}

void CollapsingSand_Create(void *data)
{
    RSDK_THIS(CollapsingSand);

    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->position.x &= 0xFFF80000;
    entity->position.y &= 0xFFF80000;
    if (!SceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->hitbox.left   = -(entity->size.x >> 17);
        entity->hitbox.top    = -48 - (entity->size.y >> 17);
        entity->hitbox.right  = entity->size.x >> 17;
        entity->hitbox.bottom = entity->size.y >> 17;
        entity->state         = CollapsingSand_State_CheckPlayerCollisions;
    }
}

void CollapsingSand_StageLoad(void)
{
    CollapsingSand->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SandCollapse.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &CollapsingSand->animator, true, 0);

    CollapsingSand->sfxSandFall = RSDK.GetSFX("MSZ/SandFall.wav");
}

void CollapsingSand_State_CheckPlayerCollisions(void)
{
    RSDK_THIS(CollapsingSand);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && player->onGround && player->collisionMode) {
            entity->state        = CollapsingSand_State_CollapseDelay;
            entity->collapseLeft = player->position.x <= entity->position.x;
            foreach_break;
        }
    }
}

void CollapsingSand_State_CollapseDelay(void)
{
    RSDK_THIS(CollapsingSand);

    if (entity->collapseTimer >= entity->delay) {
        entity->field_70      = entity->size.x >> 20;
        entity->field_74      = entity->size.y >> 20;
        entity->collapseTimer = 0;
        if (!entity->collapseLeft) {
            entity->state = CollapsingSand_State_CollapseRight;
            entity->tileX = ((entity->size.x >> 1) + entity->position.x - 0x80000) >> 20;
        }
        else {
            entity->state = CollapsingSand_State_CollapseLeft;
            entity->tileX = (entity->position.x - (entity->size.x >> 1) + 0x80000) >> 20;
        }
        RSDK.PlaySfx(CollapsingSand->sfxSandFall, false, 255);

        entity->tileY    = ((entity->size.y >> 1) + entity->position.y - 0x80000) >> 20;
        entity->tileMaxX = entity->tileX;
        entity->tileMaxY = (entity->position.y - (entity->size.y >> 1) + 0x80000) >> 20;
    }
    else {
        entity->collapseTimer++;
    }
}

void CollapsingSand_State_CollapseLeft(void)
{
    RSDK_THIS(CollapsingSand);

    if (entity->collapseTimer) {
        entity->collapseTimer = (entity->collapseTimer + 1) & 3;
    }
    else {
        if (entity->field_70 > 0) {
            int tx = entity->tileX;
            int ty = entity->tileY;
            if (tx >= entity->tileMaxX) {
                int x = (tx << 20) + 0x80000;
                int y = (ty << 20) + 0x80000;

                while (true) {
                    if (ty < entity->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                        if (ty == entity->tileY)
                            RSDK.SetTileInfo(Zone->fgLow, tx, ty, ((entity->field_70 & 1) + 168) | 0xF000);
                    }

                    y -= 0x100000;
                    tx--;
                    ty--;
                    x -= 0x100000;
                    if (tx < entity->tileMaxX)
                        break;
                }
            }

            ++entity->tileX;
            --entity->field_70;
            entity->collapseTimer = (entity->collapseTimer + 1) & 3;
        }
        else if (entity->field_74 <= 0) {
            foreach_all(ItemBox, itembox)
            {
                if (abs(itembox->position.x - entity->position.x) < entity->size.x >> 1) {
                    if (abs(itembox->position.y - entity->position.y >= 0) < entity->size.y >> 1 && itembox->state == ItemBox_State_Normal)
                        itembox->state = ItemBox_State_Falling;
                }
            }
            destroyEntity(entity);
        }
        else {
            int tx = entity->tileX;
            int ty = entity->tileY;

            if (tx >= entity->tileMaxX) {
                int x = (tx << 20) + 0x80000;
                int y = ((ty << 20) + 0x80000);

                while (true) {
                    if (ty < entity->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                    }

                    x -= 0x100000;
                    y -= 0x100000;
                    tx--;
                    ty--;
                    if (tx < entity->tileMaxX)
                        break;
                }
            }

            --entity->tileY;
            --entity->field_74;
            entity->collapseTimer = (entity->collapseTimer + 1) & 3;
        }
    }
}

void CollapsingSand_State_CollapseRight(void)
{
    RSDK_THIS(CollapsingSand);

    if (entity->collapseTimer) {
        entity->collapseTimer = (entity->collapseTimer + 1) & 3;
    }
    else {
        if (entity->field_70 > 0) {
            int tx = entity->tileX;
            int ty = entity->tileY;

            if (tx <= entity->tileMaxX) {
                int x = (tx << 20) + 0x80000;
                int y = ((ty << 20) + 0x80000);
                while (true) {
                    if (ty < entity->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                        if (ty == entity->tileY)
                            RSDK.SetTileInfo(Zone->fgLow, tx, ty, (169 - (entity->field_70 & 1)) | 0xF000);
                    }

                    x += 0x100000;
                    y -= 0x100000;
                    ++tx;
                    --ty;
                    if (tx > entity->tileMaxX)
                        break;
                }
            }

            --entity->tileX;
            --entity->field_70;
            entity->collapseTimer = (entity->collapseTimer + 1) & 3;
        }
        else if (entity->field_74 <= 0) {
            foreach_all(ItemBox, itembox)
            {
                if (abs(itembox->position.x - entity->position.x) < entity->size.x >> 1) {
                    if (abs(itembox->position.y - entity->position.y >= 0) < entity->size.y >> 1 && itembox->state == ItemBox_State_Normal)
                        itembox->state = ItemBox_State_Falling;
                }
            }
            destroyEntity(entity);
        }
        else {
            int tx = entity->tileX;
            int ty = entity->tileY;

            if (tx <= entity->tileMaxX) {
                int x = ((tx << 20) + 0x80000);
                int y = (ty << 20) + 0x80000;
                while (true) {
                    if (ty < entity->tileMaxY)
                        break;

                    bool32 flag = false;

                    uint16 tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgLow, tx, ty, 0xFFFF);
                        flag = 1;
                    }

                    tile = RSDK.GetTileInfo(Zone->fgHigh, tx, ty);
                    if ((tile & 0x3FF) >= 446 && (tile & 0x3FF) <= 554) {
                        RSDK.SetTileInfo(Zone->fgHigh, tx, ty, 0xFFFF);
                        flag = true;
                    }

                    if (flag) {
                        EntityDebris *debris = CREATE_ENTITY(Debris, Debris_State_Move, x, y);
                        RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &debris->animator, true, 0);
                        debris->drawOrder = Zone->drawOrderHigh;
                        debris->timer     = 44;
                    }

                    x += 0x100000;
                    y -= 0x100000;
                    ++tx;
                    --ty;

                    if (tx > entity->tileMaxX)
                        break;
                }
            }

            --entity->tileY;
            --entity->field_74;
            entity->collapseTimer = (entity->collapseTimer + 1) & 3;
        }
    }
}

void CollapsingSand_EditorDraw(void) { CollapsingSand_Draw(); }

void CollapsingSand_EditorLoad(void)
{
    CollapsingSand->aniFrames = RSDK.LoadSpriteAnimation("MSZ/SandCollapse.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingSand->aniFrames, 0, &CollapsingSand->animator, true, 0);
}

void CollapsingSand_Serialize(void)
{
    RSDK_EDITABLE_VAR(CollapsingSand, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(CollapsingSand, VAR_ENUM, delay);
}
