#include "SonicMania.h"

ObjectBreakableWall *BreakableWall;

void BreakableWall_Update(void)
{
    RSDK_THIS(BreakableWall);
    StateMachine_Run(entity->state);
}

void BreakableWall_LateUpdate(void) {}

void BreakableWall_StaticUpdate(void) {}

void BreakableWall_Draw(void)
{
    RSDK_THIS(BreakableWall);
    StateMachine_Run(entity->stateDraw);
}

void BreakableWall_Create(void *data)
{
    RSDK_THIS(BreakableWall);
    entity->gravityStrength = 0x3800;
    if (data) {
        int32 subtype         = voidToInt(data);
        entity->visible       = true;
        entity->updateRange.x = 0x100000;
        entity->updateRange.y = 0x100000;
        entity->active        = ACTIVE_NORMAL;
        entity->drawFX        = 2 * (entity->velocity.x != 0) + 1;
        entity->dword88       = RSDK.Rand(-8, 8);
        if (subtype == 1) {
            entity->state     = BreakableWall_State_Tile;
            entity->stateDraw = BreakableWall_StateDraw_Tile;
        }
        else {
            entity->state     = BreakableWall_State_FallingTile;
            entity->stateDraw = StateMachine_None;
        }
    }
    else {
        entity->drawFX |= FX_FLIP;
        entity->visible       = false;
        entity->drawOrder     = Zone->drawOrderHigh;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        if (entity->priority == 0)
            entity->priority = Zone->fgHigh;
        else
            entity->priority = Zone->fgLow;
        entity->size.y >>= 0x10;
        entity->size.x >>= 0x10;
        switch (entity->type) {
            case 0:
                entity->state     = BreakableWall_State_BreakableSides;
                entity->stateDraw = BreakableWall_StateDraw_Outline;
                if (!entity->size.x) {
                    entity->size.x = 2;
                    entity->size.y = 4;
                }
                break;
            case 1:
                if (!entity->size.x) {
                    entity->size.x = 2;
                    entity->size.y = 2;
                }
                entity->state     = BreakableWall_State_Top;
                entity->stateDraw = BreakableWall_StateDraw_Outline2;
                break;
            case 2:
            case 3:
                if (!entity->size.x)
                    entity->size.x = 2;
                entity->state     = BreakableWall_State_TopChunks;
                entity->stateDraw = BreakableWall_StateDraw_Outline2;
                break;
            case 4:
                if (!entity->size.x)
                    entity->size.x = 2;
                entity->state     = BreakableWall_State_BottomChunks;
                entity->stateDraw = BreakableWall_StateDraw_Outline2;
                break;
            case 5:
                if (!entity->size.x) {
                    entity->size.x = 2;
                    entity->size.y = 2;
                }
                entity->state     = BreakableWall_State_BottomFull;
                entity->stateDraw = BreakableWall_StateDraw_Outline2;
                break;
            default: break;
        }
        int32 x                 = 8 * entity->size.x;
        int32 y                 = 8 * entity->size.y;
        entity->hitbox.right  = x;
        entity->hitbox.left   = -x;
        entity->hitbox.bottom = y;
        entity->hitbox.top    = -y;
    }
}

void BreakableWall_StageLoad(void)
{
    BreakableWall->spriteIndex = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(BreakableWall->spriteIndex, 0, &BreakableWall->animator, true, 0);
    if (RSDK.CheckStageFolder("AIZ"))
        BreakableWall->sfx_Break = RSDK.GetSFX("Stage/LedgeBreak3.wav");
    else
        BreakableWall->sfx_Break = RSDK.GetSFX("Stage/LedgeBreak.wav");
    BreakableWall->farPlaneLayer = RSDK.GetSceneLayerID("Far Plane");
}

// States
void BreakableWall_State_BottomChunks(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_HandleBottomBreak_Chunks();
}
void BreakableWall_State_BottomFull(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_HandleBottomBreak_All();
}
void BreakableWall_State_FallingTile(void)
{
    RSDK_THIS(BreakableWall);
    if (--entity->timer <= 0) {
        RSDK.SetTileInfo(entity->layerID, entity->tilePos.x, entity->tilePos.y, 0xFFFF);
        if (entity->drawOrder < Zone->drawOrderLow) {
            if (BreakableWall->farPlaneLayer != 0xFFFF)
                RSDK.SetTileInfo(BreakableWall->farPlaneLayer, entity->tilePos.x, entity->tilePos.y, 0xFFFF);
        }
        entity->state     = BreakableWall_State_Tile;
        entity->stateDraw = BreakableWall_StateDraw_Tile;
    }
}
void BreakableWall_State_BreakableSides(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_HandleSidesBreak();
}
void BreakableWall_State_Tile(void)
{
    RSDK_THIS(BreakableWall);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravityStrength;
    if (entity->velocity.x)
        entity->rotation += entity->dword88;
    if (entity->drawOrder >= Zone->drawOrderLow) {
        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            destroyEntity(entity);
    }
    else {
        if (++entity->timer == 120)
            destroyEntity(entity);
    }
}
void BreakableWall_State_Top(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_HandleTopBreak_All();
}
void BreakableWall_State_TopChunks(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_HandleTopBreak_Chunks();
}

// Draw States
void BreakableWall_StateDraw_Outline(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x << 19;
    drawPos.y -= entity->size.y << 19;

    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);
}
void BreakableWall_StateDraw_Outline2(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x << 19;
    drawPos.y -= entity->size.y << 19;

    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), 0xE0E0E0, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);
}
void BreakableWall_StateDraw_Tile(void)
{
    RSDK_THIS(BreakableWall);
    entity->angle = entity->rotation;
    RSDK.DrawTile(&entity->tileInfo, 1, 1, NULL, NULL, false);
}

// Breaking
void BreakableWall_HandleTopBreak_All(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 1) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 flag = player->playerAnimator.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;
                        case ID_SONIC:
                            if (!flag)
                                flag = player->playerAnimator.animationID == ANI_DROPDASH;
                            break;
                        case ID_KNUCKLES: flag = true; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY:
                            if (!flag)
                                flag = player->state == Player_State_MightyHammerDrop;
                            break;
#endif
                    }

                    if (player->groundedStore && player->collisionMode != CMODE_LWALL && player->collisionMode != CMODE_RWALL)
                        flag = false;

                    if (flag && !player->sidekick) {
                        player->onGround = false;
                        int32 ty           = entity->position.y - (entity->size.y << 19) + 0x80000;
                        int32 th           = entity->position.y - (entity->size.y << 19) + 0x80000 - ((entity->size.y << 19) + entity->position.y);
                        for (int32 y = 0; y < entity->size.y; ++y) {
                            int32 tx          = entity->position.x - (entity->size.x << 19) + 0x80000;
                            int32 posY        = ty >> 20;
                            int32 speed       = 3 * abs(th);
                            int32 offsetX     = tx - entity->position.x;
                            int32 blockSpeedX = 2 * (tx - entity->position.x);
                            for (int32 x = 0; x < entity->size.x; ++x) {
                                int32 posX                       = tx >> 20;
                                EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(1), tx, ty);
                                tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, posX, posY);
                                tileChunk->drawOrder           = entity->drawOrder;
                                int32 angle                      = RSDK.ATan2(blockSpeedX, th);
                                int32 spd                        = (speed + abs(offsetX)) >> 18;
                                tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
                                tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
                                RSDK.SetTileInfo(entity->priority, posX, posY, 0xFFFF);

                                if (entity->drawOrder < Zone->drawOrderLow) {
                                    if (BreakableWall->farPlaneLayer != 0xFFFF)
                                        RSDK.SetTileInfo(BreakableWall->farPlaneLayer, posX, posY, 0xFFFF);
                                }

                                blockSpeedX += 0x200000;
                                tx += 0x100000;
                                offsetX += 0x100000;
                            }
                            ty += 0x100000;
                            th += 0x100000;
                        }

                        RSDK.PlaySfx(BreakableWall->sfx_Break, 0, 255);
                        BreakableWall_GiveScoreBonus(player);
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - 0x10000;
                        else
#endif
                            player->velocity.y = -0x30000;
                        destroyEntity(entity);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleTopBreak_Chunks(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 1 && !player->sidekick
            && ((player->collisionPlane == 1 && entity->type != 2) || entity->type == 2)) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 flag = player->playerAnimator.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;
                        case ID_SONIC:
                            if (!flag)
                                flag = player->playerAnimator.animationID == ANI_DROPDASH;
                            break;
                        case ID_KNUCKLES: flag = true; break;
#if RETRO_USE_PLUS
                        case ID_MIGHTY:
                            if (!flag)
                                flag = player->state == Player_State_MightyHammerDrop;
                            break;
#endif
                    }

                    if (player->groundedStore && player->collisionMode != CMODE_LWALL && player->collisionMode != CMODE_RWALL)
                        flag = false;

                    if (flag && !player->sidekick) {
                        player->onGround = 0;
                        BreakableWall_HandleBlockBreak_V();
                        BreakableWall_GiveScoreBonus(player);

#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - 0x10000;
                        else
#endif
                            player->velocity.y = 0;
                        entity->hitbox.top += 8;
                        entity->hitbox.bottom -= 8;
                        --entity->size.y;
                        entity->position.y += 0x80000;
                        if (entity->size.y <= 0)
                            destroyEntity(entity);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleSidesBreak(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
#if RETRO_USE_PLUS
        if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
#endif
            if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                bool32 flag = abs(player->groundVel) >= 0x48000 && player->onGround && player->playerAnimator.animationID == ANI_JUMP;

                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = RSDK_GET_ENTITY(Player->playerCount + RSDK.GetEntityID(player), Shield);
                    flag |= shield->animator.animationID == 2;
                }

                switch (player->characterID) {
                    default: break;
                    case ID_SONIC:
                        flag |= player->playerAnimator.animationID == ANI_DROPDASH;
                        flag |= player->superState == SUPERSTATE_SUPER;
                        break;
                    case ID_KNUCKLES: flag = true; break;
                }

                if (player->state == Ice_State_FrozenPlayer) {
                    flag |= abs(player->groundVel) >= 0x48000;
                }

                if (flag && !player->sidekick) {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                        BreakableWall_HandleBlockBreak_H(entity, player->position.x > entity->position.x);
                        if (player->characterID == ID_KNUCKLES) {
                            if (player->playerAnimator.animationID == ANI_FLY) {
                                player->abilitySpeed -= player->abilitySpeed >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                                if (abs(player->velocity.x) <= 0x30000) {
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimator, 0, 0);
                                    player->state = Player_State_KnuxGlideDrop;
                                }
                            }
                            else if (player->playerAnimator.animationID == ANI_FLYLIFTTIRED) {
                                player->abilitySpeed -= player->abilitySpeed >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                            }
                        }
                        RSDK.PlaySfx(BreakableWall->sfx_Break, 0, 255);
                        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                    }
                    continue; // skip to next loop, so we dont do the box collision
                }
            }
#if RETRO_USE_PLUS
        }
#endif
        Player_CheckCollisionBox(player, entity, &entity->hitbox);
    }
}
void BreakableWall_HandleBottomBreak_Chunks(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 yVel = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 4) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    if (!player->sidekick) {
                        BreakableWall_HandleBlockBreak_V();
                        player->velocity.y = 0;

                        if (entity->size.y < 2) {
                            entity->hitbox.top += 8;
                            entity->size.y -= 1;
                            entity->hitbox.bottom -= 8;
                            entity->position.y -= 0x80000;
                        }
                        else {
                            entity->hitbox.top += 16;
                            entity->size.y -= 2;
                            entity->hitbox.bottom -= 16;
                            entity->position.y -= 0x100000;
                        }

                        if (entity->size.y <= 0)
                            destroyEntity(entity);
                        player->velocity.y = yVel;
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleBottomBreak_All(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int32 velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 4) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    player->onGround = false;
                    int32 tx           = entity->position.x - (entity->size.x << 19) + 0x80000;
                    int32 tw           = entity->position.x - (entity->size.x << 19) + 0x80000;
                    int32 ty           = entity->position.y - (entity->size.y << 19) + 0x80000;
                    int32 th           = (entity->size.y << 19) + entity->position.y;
                    int32 offsetY      = ty - th;
                    for (int32 y = 0; y < entity->size.y; ++y) {
                        int32 posY        = ty >> 20;
                        int32 speed       = 3 * abs(offsetY);
                        int32 offsetX     = tx - entity->position.x;
                        int32 blockSpeedX = 2 * (tx - entity->position.x);
                        for (int32 x = 0; x < entity->size.x; ++x) {
                            int32 posX                       = tx >> 20;
                            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(1), tx, ty);
                            tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, posX, posY);
                            tileChunk->drawOrder           = entity->drawOrder;
                            int32 angle                      = RSDK.ATan2(blockSpeedX, offsetY);
                            int32 spd                        = (speed + abs(offsetX)) >> 18;
                            tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
                            tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
                            RSDK.SetTileInfo(entity->priority, posX, posY, 0xFFFF);

                            if (entity->drawOrder < Zone->drawOrderLow) {
                                if (BreakableWall->farPlaneLayer != 0xFFFF)
                                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, posX, posY, 0xFFFF);
                            }

                            blockSpeedX += 0x200000;
                            tx += 0x100000;
                            offsetX += 0x100000;
                        }
                        ty += 0x100000;
                        tx = tw;
                        offsetY += 0x100000;
                    }

                    RSDK.PlaySfx(BreakableWall->sfx_Break, 0, 255);
                    player->velocity.y = velY;
                    destroyEntity(entity);
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_HandleBlockBreak_V(void)
{
    RSDK_THIS(BreakableWall);

    int32 sizeX = entity->size.x;
    int32 sizeY = entity->size.y;
    int32 posX  = entity->position.x;
    int32 posY  = entity->position.y;
    if (entity->state == BreakableWall_State_BottomChunks) {
        if (sizeY > 2)
            entity->size.y = 2;
        entity->position.y += (sizeY - entity->size.y) << 19;
    }
    else {
        entity->size.y = 1;
        entity->position.y += 0x80000 - (sizeY << 19);
    }

    int32 startX = entity->position.x;
    int32 endX   = entity->position.x - (entity->size.x << 19) + 0x80000;
    int32 curY     = entity->position.y - (entity->size.y << 19) + 0x80000;
    int32 startY     = (entity->size.y << 19) + entity->position.y;

    int32 distY = curY - startY;
    for (int32 y = 0; y < entity->size.y; ++y) {
        int32 tx     = endX;
        int32 distX   = endX - startX;
        int32 tileY  = curY >> 20;
        int32 angleX = 2 * distX;
        int32 speed  = 3 * abs(distY);
        for (int32 x = 0; x < entity->size.x; ++x) {
            int32 tileX                      = tx >> 20;
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(1), tx, curY);
            tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, tileX, tileY);
            tileChunk->drawOrder           = entity->drawOrder;

            int32 angle = RSDK.ATan2(angleX, distY);
            int32 spd   = (speed + abs(distX)) >> 18;
            tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
            tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
            RSDK.SetTileInfo(entity->priority, tileX, tileY, 0xFFFF);
            if (entity->drawOrder < Zone->drawOrderLow) {
                if (BreakableWall->farPlaneLayer != 0xFFFF)
                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, tileX, tileY, 0xFFFF);
            }

            tx += 0x100000;
            distX += 0x100000;
            angleX += 0x200000;
        }
        curY += 0x100000;
        distY += 0x100000;
    }

    entity->size.x     = sizeX;
    entity->size.y     = sizeY;
    entity->position.x = posX;
    entity->position.y = posY;
    RSDK.PlaySfx(BreakableWall->sfx_Break, false, 255);
}
void BreakableWall_HandleBlockBreak_H(EntityBreakableWall *entity, uint8 flip)
{
    int32 startX = entity->position.x;
    int32 startY = entity->position.y;
    int32 endX   = entity->position.x - (entity->size.x << 19) + 0x80000;
    int32 endY   = entity->position.y - (entity->size.y << 19) + 0x80000;
    switch (flip) {
        case FLIP_NONE: startX += entity->size.x << 19; break;
        case FLIP_X: startX -= entity->size.x << 19; break;
        case FLIP_Y: startY += entity->size.y << 19; break;
        case FLIP_XY: startY -= entity->size.y << 19; break;
        default: break;
    }

    int32 curY = endY - startY;
    for (int32 y = 0; y < entity->size.y; ++y) {
        int32 curX   = endX - startX;
        int32 tileY  = (curY + startY) >> 20;
        int32 angleX = 2 * (endX - startX);
        for (int32 x = 0; x < entity->size.x; ++x) {
            int32 tileX = (curX + startX) >> 20;
            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(1), curX + startX, curY + startY);
            tileChunk->tileInfo  = RSDK.GetTileInfo(entity->priority, tileX, tileY);
            tileChunk->drawOrder = entity->drawOrder;

            switch (flip) {
                case FLIP_NONE:
                case FLIP_X: {
                    int32 angle  = RSDK.ATan2(angleX, curY);
                    int32 angle2 = 0;
                    if (abs(curX) > 0x80000) {
                        if (curX + startX >= startX)
                            angle2 = RSDK.ATan2(0x80000, curY);
                        else
                            angle2 = RSDK.ATan2(-0x80000, curY);
                    }
                    else {
                        angle2 = RSDK.ATan2(curX, curY);
                    }
                    tileChunk->velocity.x = flip == FLIP_NONE ? -0x10000 : 0x10000;
                    tileChunk->velocity.y = 0x10000;
                    tileChunk->velocity.x += 40 * (((entity->size.y << 19) + 3 * abs(curX) - abs(curY)) >> 18) * RSDK.Cos256(angle);
                    tileChunk->velocity.y += 32 * ((abs(curY) + abs(curX) + 2 * abs(curY)) >> 18) * RSDK.Sin256(angle2);
                    break;
                }
                case FLIP_Y: {
                    int32 angle = RSDK.ATan2(angleX, curY);
                    tileChunk->velocity.x += 40 * ((abs(curX) + 3 * abs(curY)) >> 18) * RSDK.Cos256(angle);
                    tileChunk->velocity.y += 40 * ((abs(curX) + 3 * abs(curY)) >> 18) * RSDK.Sin256(angle);
                }
            }

            RSDK.SetTileInfo(entity->priority, tileX, tileY, 0xFFFF);
            if (entity->drawOrder < Zone->drawOrderLow) {
                if (BreakableWall->farPlaneLayer != 0xFFFF)
                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, tileX, tileY, 0xFFFF);
            }
            curX += 0x10 << 16;
            angleX += 0x200000;
        }
        curY += 0x10 << 16;
    }
}

// Misc
void BreakableWall_GiveScoreBonus(void *plr)
{
    EntityPlayer *player = (EntityPlayer *)plr;
    RSDK_THIS(BreakableWall);
    EntityScoreBonus *scoreBonus = CREATE_ENTITY(ScoreBonus, NULL, entity->position.x, entity->position.y);
    scoreBonus->drawOrder        = Zone->drawOrderHigh;
    scoreBonus->animator.frameID     = player->scoreBonus;
    switch (player->scoreBonus) {
        case 0: Player_GiveScore(player, 100); break;
        case 1: Player_GiveScore(player, 200); break;
        case 2: Player_GiveScore(player, 500); break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE: Player_GiveScore(player, 1000); break;
        case 0xF: Player_GiveScore(player, 10000); break;
        default: break;
    }
    if (player->scoreBonus < 15)
        player->scoreBonus++;
}

void BreakableWall_EditorDraw(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x << 19;
    drawPos.y -= entity->size.y << 19;

    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), 0xFFFF00, 0, INK_NONE, false);
    RSDK.DrawLine(drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xFFFF00, 0, INK_NONE, false);

    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);

    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, false);
}

void BreakableWall_EditorLoad(void)
{
    BreakableWall->spriteIndex = RSDK.LoadSpriteAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(BreakableWall->spriteIndex, 0, &BreakableWall->animator, true, 0);
}

void BreakableWall_Serialize(void)
{
    RSDK_EDITABLE_VAR(BreakableWall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyKnux);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyMighty);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_ENUM, priority);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_VECTOR2, size);
}
