#include "../SonicMania.h"

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
        int subtype           = (int)(size_t)data;
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
            entity->stateDraw = 0;
        }
    }
    else {
        entity->drawFX |= FX_FLIP;
        entity->visible       = 0;
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
                entity->state     = BreakableWall_State_HWall;
                entity->stateDraw = BreakableWall_StateDraw_BWall;
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
                entity->stateDraw = BreakableWall_StateDraw_BWall2;
                break;
            case 2:
            case 3:
                if (!entity->size.x)
                    entity->size.x = 2;
                entity->state     = BreakableWall_State_VerticalChunks;
                entity->stateDraw = BreakableWall_StateDraw_BWall2;
                break;
            case 4:
                if (!entity->size.x)
                    entity->size.x = 2;
                entity->state     = BreakableWall_State_BottomChunks;
                entity->stateDraw = BreakableWall_StateDraw_BWall2;
                break;
            case 5:
                if (!entity->size.x) {
                    entity->size.x = 2;
                    entity->size.y = 2;
                }
                entity->state     = BreakableWall_State_BottomFull;
                entity->stateDraw = BreakableWall_StateDraw_BWall2;
                break;
            default: break;
        }
        int x                 = 8 * entity->size.x;
        int y                 = 8 * entity->size.y;
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
    BreakableWall_Break4();
}
void BreakableWall_State_BottomFull(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_Break5();
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
void BreakableWall_State_HWall(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_State_HandleHWall();
}
void BreakableWall_State_HandleHWall(void)
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
                        flag |= player->superState == 2;
                        break;
                    case ID_KNUCKLES: flag = true; break;
                }

                if (player->state == Ice_State_FrozenPlayer) {
                    flag |= abs(player->groundVel) >= 0x48000;
                }

                if (flag && !player->sidekick) {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                        BreakableWall_BreakUnknown(entity, player->position.x > entity->position.x);
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
                        RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
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
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        if (++entity->timer == 120)
            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}
void BreakableWall_State_Top(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_Break1();
}
void BreakableWall_State_VerticalChunks(void)
{
    RSDK_THIS(BreakableWall);
    entity->visible = DebugMode->debugActive;
    BreakableWall_Break2AND3();
}
// Draw States
void BreakableWall_StateDraw_BWall(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x << 19;
    drawPos.y -= entity->size.y << 19;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), 0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, 0);
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
}
void BreakableWall_StateDraw_BWall2(void)
{
    RSDK_THIS(BreakableWall);
    Vector2 drawPos;
    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= entity->size.x << 19;
    drawPos.y -= entity->size.y << 19;
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, 0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x - 0x10000, drawPos.y - 0x10000, drawPos.x - 0x10000, drawPos.y + (entity->size.y << 20), 0xE0E0E0, 0, INK_NONE, 0);
    RSDK.DrawLine(drawPos.x + (entity->size.x << 20), drawPos.y - 0x10000, drawPos.x + (entity->size.x << 20), drawPos.y + (entity->size.y << 20),
                  0xE0E0E0, 0, INK_NONE, 0);
    entity->direction = FLIP_NONE;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animator, &drawPos, 0);
}
void BreakableWall_StateDraw_Tile(void)
{
    RSDK_THIS(BreakableWall);
    entity->angle = entity->rotation;
    RSDK.DrawTile(&entity->tileInfo, 1, 1, 0, 0, 0);
}
// Break
void BreakableWall_Break1(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
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
                        int ty           = entity->position.y - (entity->size.y << 19) + 0x80000;
                        int th           = entity->position.y - (entity->size.y << 19) + 0x80000 - ((entity->size.y << 19) + entity->position.y);
                        for (int y = 0; y < entity->size.y; ++y) {
                            int tx          = entity->position.x - (entity->size.x << 19) + 0x80000;
                            int posY        = ty >> 20;
                            int speed       = 3 * abs(th);
                            int offsetX     = tx - entity->position.x;
                            int blockSpeedX = 2 * (tx - entity->position.x);
                            for (int x = 0; x < entity->size.x; ++x) {
                                int posX                       = tx >> 20;
                                EntityBreakableWall *tileChunk = (EntityBreakableWall *)RSDK.CreateEntity(BreakableWall->objectID, (void *)1, tx, ty);
                                tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, posX, posY);
                                tileChunk->drawOrder           = entity->drawOrder;
                                int angle                      = RSDK.ATan2(blockSpeedX, th);
                                int spd                        = (speed + abs(offsetX)) >> 18;
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

                        RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
                        BreakableWall_GiveScoreBonus(player);
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y = velY - 0x10000;
                        else
#endif
                            player->velocity.y = -0x30000;
                        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break2AND3(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
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
                        BreakableWall_BreakV();
                        BreakableWall_GiveScoreBonus(player);

#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y -= 0x10000;
                        else
#endif
                            player->velocity.y = 0;
                        entity->hitbox.top += 8;
                        entity->hitbox.bottom -= 8;
                        --entity->size.y;
                        entity->position.y += 0x80000;
                        if (entity->size.y <= 0)
                            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break4(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int yVel = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 4) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimator.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    if (!player->sidekick) {
                        BreakableWall_BreakV();
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
                            RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
                        player->velocity.y = yVel;
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break5(void)
{
    RSDK_THIS(BreakableWall);

    foreach_active(Player, player)
    {
        int velY = player->velocity.y;
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox) == 4) {
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

                    player->onGround = false;
                    int tx           = entity->position.x - (entity->size.x << 19) + 0x80000;
                    int tw           = entity->position.x - (entity->size.x << 19) + 0x80000;
                    int ty           = entity->position.y - (entity->size.y << 19) + 0x80000;
                    int th           = (entity->size.y << 19) + entity->position.y;
                    int offsetY      = ty - th;
                    for (int y = 0; y < entity->size.y; ++y) {
                        int posY        = ty >> 20;
                        int speed       = 3 * abs(offsetY);
                        int offsetX     = tx - entity->position.x;
                        int blockSpeedX = 2 * (tx - entity->position.x);
                        for (int x = 0; x < entity->size.x; ++x) {
                            int posX                       = tx >> 20;
                            EntityBreakableWall *tileChunk = CREATE_ENTITY(BreakableWall, intToVoid(1), tx, ty);
                            tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, posX, posY);
                            tileChunk->drawOrder           = entity->drawOrder;
                            int angle                      = RSDK.ATan2(blockSpeedX, offsetY);
                            int spd                        = (speed + abs(offsetX)) >> 18;
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

                    RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
                    player->velocity.y = velY;
                    destroyEntity(entity);
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_BreakV()
{
    RSDK_THIS(BreakableWall);

    int sizeX = entity->size.x;
    int sizeY = entity->size.y;
    int posX  = entity->position.x;
    int posY  = entity->position.y;
    if (entity->state == BreakableWall_State_BottomChunks) {
        if (sizeY > 2)
            entity->size.y = 2;
        entity->position.y += (sizeY - entity->size.y) << 19;
    }
    else {
        entity->size.y = 1;
        entity->position.y += 0x80000 - (sizeY << 19);
    }

    int startX = entity->position.x;
    int endX   = entity->position.x - (entity->size.x << 19) + 0x80000;
    int curY     = entity->position.y - (entity->size.y << 19) + 0x80000;
    int startY     = (entity->size.y << 19) + entity->position.y;

    int distY = curY - startY;
    for (int y = 0; y < entity->size.y; ++y) {
        int tx     = endX;
        int distX   = endX - startX;
        int tileY  = curY >> 20;
        int angleX = 2 * distX;
        int speed  = 3 * abs(distY);
        for (int x = 0; x < entity->size.x; ++x) {
            int tileX                      = tx >> 20;
            EntityBreakableWall *tileChunk = (EntityBreakableWall *)RSDK.CreateEntity(BreakableWall->objectID, (void *)1, tx, curY);
            tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, tileX, tileY);
            tileChunk->drawOrder           = entity->drawOrder;

            int angle = RSDK.ATan2(angleX, distY);
            int spd   = (speed + abs(distX)) >> 18;
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
    RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
}
void BreakableWall_BreakUnknown(EntityBreakableWall *entity, byte flip)
{
    int startX = entity->position.x;
    int startY = entity->position.y;
    int endX   = entity->position.x - (entity->size.x << 19) + 0x80000;
    int endY   = entity->position.y - (entity->size.y << 19) + 0x80000;
    switch (flip) {
        case FLIP_NONE: startX += entity->size.x << 19; break;
        case FLIP_X: startX -= entity->size.x << 19; break;
        case FLIP_Y: startY += entity->size.y << 19; break;
        case FLIP_XY: startY -= entity->size.y << 19; break;
        default: break;
    }

    int curY = endY - startY;
    for (int y = 0; y < entity->size.y; ++y) {
        int curX   = endX - startX;
        int tileY  = (curY + startY) >> 20;
        int angleX = 2 * (endX - startX);
        for (int x = 0; x < entity->size.x; ++x) {
            int tileX = (curX + startX) >> 20;
            EntityBreakableWall *tileChunk =
                (EntityBreakableWall *)RSDK.CreateEntity(BreakableWall->objectID, (void *)1, curX + startX, curY + startY);
            tileChunk->tileInfo  = RSDK.GetTileInfo(entity->priority, tileX, tileY);
            tileChunk->drawOrder = entity->drawOrder;

            switch (flip) {
                case FLIP_NONE:
                case FLIP_X: {
                    int angle  = RSDK.ATan2(angleX, curY);
                    int angle2 = 0;
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
                    int angle = RSDK.ATan2(angleX, curY);
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
    EntityScoreBonus *scoreBonus = (EntityScoreBonus *)RSDK.CreateEntity(ScoreBonus->objectID, 0, entity->position.x, entity->position.y);
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

void BreakableWall_EditorDraw(void) {}

void BreakableWall_EditorLoad(void) {}

void BreakableWall_Serialize(void)
{
    RSDK_EDITABLE_VAR(BreakableWall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyKnux);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyMighty);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_ENUM, priority);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_VECTOR2, size);
}
