#include "SonicMania.h"

ObjectCollapsingPlatform *CollapsingPlatform;

void CollapsingPlatform_Update(void)
{
    RSDK_THIS(CollapsingPlatform);
    entity->visible = false;
    if (DebugMode)
        entity->visible = DebugMode->debugActive;

    if (entity->collapseDelay) {
#if RETRO_USE_PLUS
        if (Player) {
            foreach_active(Player, player)
            {
                if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1) {
                    StateMachine_Run(entity->state);
                    RSDK.PlaySfx(CollapsingPlatform->sfx_Crumble, 0, 255);
                    if (entity->respawn) {
                        entity->collapseDelay = 0;
                        entity->playerPos.x   = 0;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    foreach_return;
                }
            }
        }
#endif
        if (--entity->collapseDelay == 0) {
            StateMachine_Run(entity->state);
            RSDK.PlaySfx(CollapsingPlatform->sfx_Crumble, 0, 255);
            if (entity->respawn) {
                entity->collapseDelay = 0;
                entity->playerPos.x   = 0;
            }
            else {
                destroyEntity(entity);
            }
        }
    }
    else if (Player) {
        entity->direction = FLIP_NONE;
        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)
#if RETRO_USE_PLUS
                && (!entity->mightyOnly || (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop))
#endif
                && !player->sidekick && player->onGround && !player->collisionMode && !entity->eventOnly && entity->delay < 0xFFFF) {
                entity->playerPos.x = player->position.x;
#if RETRO_USE_PLUS
                if (player->characterID == ID_MIGHTY && player->jumpAbilityTimer > 1) {
                    StateMachine_Run(entity->state);
                    RSDK.PlaySfx(CollapsingPlatform->sfx_Crumble, 0, 255);
                    if (entity->respawn) {
                        entity->collapseDelay = 0;
                        entity->playerPos.x   = 0;
                    }
                    else {
                        destroyEntity(entity);
                    }
                    foreach_break;
                }
#endif
            }
        }

        if (entity->playerPos.x) {
            entity->collapseDelay = entity->delay;
            if (!entity->delay) {
                StateMachine_Run(entity->state);
                RSDK.PlaySfx(CollapsingPlatform->sfx_Crumble, 0, 255);
                if (entity->respawn) {
                    entity->collapseDelay = 0;
                    entity->playerPos.x   = 0;
                }
                else {
                    destroyEntity(entity);
                }
            }
        }
    }
    else if (entity->playerPos.x) {
        entity->collapseDelay = entity->delay;
        if (!entity->delay) {
            StateMachine_Run(entity->state);
            RSDK.PlaySfx(CollapsingPlatform->sfx_Crumble, 0, 255);
            if (entity->respawn) {
                entity->collapseDelay = 0;
                entity->playerPos.x   = 0;
            }
            else {
                destroyEntity(entity);
            }
        }
    }
}

void CollapsingPlatform_LateUpdate(void) {}

void CollapsingPlatform_StaticUpdate(void) {}

void CollapsingPlatform_Draw(void)
{
    RSDK_THIS(CollapsingPlatform);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xE0E0E0, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + entity->size.y, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xE0E0E0, 0, INK_NONE,
                  false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x += entity->size.x;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.y += entity->size.y;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x -= entity->size.x;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);
}

void CollapsingPlatform_Create(void *data)
{
    RSDK_THIS(CollapsingPlatform);

    entity->visible = true;
    entity->position.x &= 0xFFF80000;
    entity->position.y &= 0xFFF80000;
    entity->drawFX |= FX_FLIP;
    entity->drawOrder = Zone->drawOrderLow;
    if (entity->targetLayer == 0) {
        entity->targetLayer = Zone->fgLow;
        entity->drawOrder   = Zone->drawOrderLow;
    }
    else {
        entity->targetLayer = Zone->fgHigh;
        entity->drawOrder   = Zone->drawOrderHigh;
    }

    if (!RSDK_sceneInfo->inEditor) {
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        int32 xOff              = (entity->position.x >> 20) - (entity->size.x >> 21);
        int32 yOff              = (entity->position.y >> 20) - (entity->size.y >> 21);

        if ((entity->size.y & 0xFFF00000) && !(entity->size.y & 0xFFF00000 & 0x80000000)) {
            int32 sx = entity->size.x >> 20;
            int32 sy = entity->size.y >> 20;
            for (int32 y = 0; y < sy; ++y) {
                for (int32 x = 0; x < sx; ++x) {
                    entity->storedTiles[x + y * (entity->size.x >> 20)] = RSDK.GetTileInfo(entity->targetLayer, x + xOff, y + yOff);
                }
            }
        }

        entity->hitbox.right  = entity->size.x >> 17;
        entity->hitbox.left   = -(entity->size.x >> 17);
        entity->hitbox.bottom = entity->size.y >> 17;
        entity->hitbox.top    = -16 - entity->hitbox.bottom;
    }
    switch (entity->type) {
        default:
        case 0: entity->state = CollapsingPlatform_State_Left; break;
        case 1: entity->state = CollapsingPlatform_State_Right; break;
        case 2: entity->state = CollapsingPlatform_State_Center; break;
        case 3: entity->state = CollapsingPlatform_State_LeftOrRight; break;
        case 4: entity->state = CollapsingPlatform_State_PlayerPos; break;
    }
}

void CollapsingPlatform_StageLoad(void)
{
    CollapsingPlatform->spriteIndex = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingPlatform->spriteIndex, 0, &CollapsingPlatform->animator, true, 0);
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        CollapsingPlatform->shift = 1;

    if (RSDK.CheckStageFolder("FBZ") || RSDK.CheckStageFolder("HCZ") || RSDK.CheckStageFolder("LRZ1") || RSDK.CheckStageFolder("LRZ2")
        || RSDK.CheckStageFolder("AIZ")) {
        CollapsingPlatform->sfx_Crumble = RSDK.GetSFX("Stage/LedgeBreak3.wav");
    }
    else {
        CollapsingPlatform->sfx_Crumble = RSDK.GetSFX("Stage/LedgeBreak.wav");
    }
}

void CollapsingPlatform_State_Left(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = entity->storedTiles;
    int32 startTX   = (entity->position.x >> 20) - (entity->size.x >> 21);
    int32 startTY   = (entity->position.y >> 20) - (entity->size.y >> 21);
    int32 tx        = entity->position.x - (entity->size.x >> 1) + 0x80000;
    int32 ty        = (entity->position.y - (entity->size.y >> 1)) + 0x80000;

    int32 sx = entity->size.x >> 20;
    int32 sy = entity->size.y >> 20;

    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(2), tx, ty);
            tx += 0x100000;
            tileChunk->layerID   = entity->targetLayer;
            tileChunk->tileInfo  = *tiles;
            tileChunk->drawOrder = entity->drawOrder;
            tileChunk->tilePos.y = y + startTY;
            tileChunk->tilePos.x = x + startTX;
            int32 timerX           = x >> CollapsingPlatform->shift;
            int32 timerY           = y >> CollapsingPlatform->shift;
            tileChunk->timer     = 3 * (sy + 2 * timerX - timerY);
            ++tiles;
        }
        tx -= entity->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_Right(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = entity->storedTiles;
    int32 startTX   = (entity->position.x >> 20) - (entity->size.x >> 21);
    int32 startTY   = (entity->position.y >> 20) - (entity->size.y >> 21);
    int32 tx        = entity->position.x - (entity->size.x >> 1) + 0x80000;
    int32 ty        = (entity->position.y - (entity->size.y >> 1)) + 0x80000;

    int32 timerSX   = entity->size.x >> CollapsingPlatform->shift >> 20;

    int32 sx = entity->size.x >> 20;
    int32 sy = entity->size.y >> 20;
    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(2), tx, ty);
            tx += 0x100000;
            tileChunk->layerID   = entity->targetLayer;
            tileChunk->tileInfo  = *tiles;
            tileChunk->drawOrder = entity->drawOrder;
            tileChunk->tilePos.y = y + startTY;
            tileChunk->tilePos.x = x + startTX;
            int32 timerX           = x >> CollapsingPlatform->shift;
            int32 timerY           = y >> CollapsingPlatform->shift;
            tileChunk->timer     = 3 * (sy + 2 * (timerSX - timerX) - timerY);
            ++tiles;
        }
        tx -= entity->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_Center(void)
{
    RSDK_THIS(CollapsingPlatform);

    uint16 *tiles = entity->storedTiles;
    int32 startTX   = (entity->position.x >> 20) - (entity->size.x >> 21);
    int32 startTY   = (entity->position.y >> 20) - (entity->size.y >> 21);
    int32 tx        = entity->position.x - (entity->size.x >> 1) + 0x80000;
    int32 ty        = (entity->position.y - (entity->size.y >> 1)) + 0x80000;

    int32 timerSX = entity->size.x >> CollapsingPlatform->shift >> 20;
    int32 timerSY = entity->size.y >> CollapsingPlatform->shift >> 20;

    int32 sx = entity->size.x >> 20;
    int32 sy = entity->size.y >> 20;
    for (int32 y = 0; y < sy; ++y) {
        for (int32 x = 0; x < sx; ++x) {
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(2), tx, ty);
            tx += 0x100000;
            tileChunk->layerID   = entity->targetLayer;
            tileChunk->tileInfo  = *tiles;
            tileChunk->drawOrder = entity->drawOrder;
            tileChunk->tilePos.x = x + startTX;
            tileChunk->tilePos.y = y + startTY;
            int32 timerX           = abs((timerSX >> 1) - (x >> CollapsingPlatform->shift));
            int32 timerY           = y >> CollapsingPlatform->shift;
            tileChunk->timer     = 3 * (timerSY + 2 * timerX - timerY);
            if (!(timerSX & 1) && x >> CollapsingPlatform->shift < (timerSX >> 1))
                tileChunk->timer -= 6;
            ++tiles;
        }
        tx -= entity->size.x;
        ty += 0x100000;
    }
}
void CollapsingPlatform_State_LeftOrRight(void)
{
    RSDK_THIS(CollapsingPlatform);
    int32 px = entity->playerPos.x;
    int32 x  = entity->position.x;

    if (px < x)
        CollapsingPlatform_State_Left();
    else
        CollapsingPlatform_State_Right();
}
void CollapsingPlatform_State_PlayerPos(void)
{
    RSDK_THIS(CollapsingPlatform);
    int32 px = entity->playerPos.x;
    int32 x  = entity->position.x;

    if (abs(px - x) < entity->size.x / 6) {
        CollapsingPlatform_State_Center();
    }
    else {
        if (px < x)
            CollapsingPlatform_State_Left();
        else
            CollapsingPlatform_State_Right();
    }
}

void CollapsingPlatform_EditorDraw(void)
{
    RSDK_THIS(CollapsingPlatform);
    Vector2 drawPos;

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x >> 1;
    drawPos.y -= entity->size.y >> 1;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y - 0x10000, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, entity->size.y + drawPos.y, drawPos.x + entity->size.x, entity->size.y + drawPos.y, 0xFFFF00, 0, INK_NONE,
                  false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + entity->size.x, drawPos.y - 0x10000, drawPos.x + entity->size.x, drawPos.y + entity->size.y, 0xFFFF00, 0, INK_NONE,
                  false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x += entity->size.x;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.y += entity->size.y;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);

    drawPos.x -= entity->size.x;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&CollapsingPlatform->animator, &drawPos, false);
}

void CollapsingPlatform_EditorLoad(void)
{
    CollapsingPlatform->spriteIndex = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(CollapsingPlatform->spriteIndex, 0, &CollapsingPlatform->animator, true, 0);
}

void CollapsingPlatform_Serialize(void)
{
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_BOOL, respawn);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_UINT16, targetLayer);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_ENUM, delay);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_BOOL, eventOnly);
    RSDK_EDITABLE_VAR(CollapsingPlatform, VAR_BOOL, mightyOnly);
}
