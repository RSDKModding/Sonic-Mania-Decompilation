#include "../SonicMania.hpp"

ObjectBreakableWall *BreakableWall;

void BreakableWall_Update()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    if (entity->state)
        entity->state();
}

void BreakableWall_LateUpdate()
{

}

void BreakableWall_StaticUpdate()
{

}

void BreakableWall_Draw()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    if (entity->stateDraw)
        entity->stateDraw();
}

void BreakableWall_Create(void *data)
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    entity->gravityStrength     = 0x3800;
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
        entity->size.y = entity->size.y >> 0x10;
        entity->size.x = entity->size.x >> 0x10;
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
        int x                 = 8 * (entity->size.x >> 0x10);
        int y                 = 8 * (entity->size.y >> 0x10);
        entity->hitbox.right  = x;
        entity->hitbox.left   = -x;
        entity->hitbox.bottom = y;
        entity->hitbox.top    = -y;
    }
}

void BreakableWall_StageLoad()
{
    BreakableWall->spriteIndex = RSDK.LoadAnimation("Global/TicMark.bin", SCOPE_STAGE);
    RSDK.SetSpriteAnimation(BreakableWall->spriteIndex, 0, &BreakableWall->animData, true, 0);
    if (RSDK.CheckStageFolder("AIZ"))
        BreakableWall->sfx_Break = RSDK.GetSFX("Stage/LedgeBreak3.wav");
    else
        BreakableWall->sfx_Break = RSDK.GetSFX("Stage/LedgeBreak.wav");
    BreakableWall->farPlaneLayer = RSDK.GetSceneLayerID("Far Plane");
}

// States
void BreakableWall_State_BottomChunks()
{
    EntityBreakableWall *entity     = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->visible = DebugMode->active;
    BreakableWall_Break4();
}
void BreakableWall_State_BottomFull()
{
    EntityBreakableWall *entity     = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->visible = DebugMode->active;
    BreakableWall_Break5();
}
void BreakableWall_State_FallingTile()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    if (--entity->timer <= 0) {
        RSDK.SetTileInfo(entity->layerID, entity->tilePos.x, entity->tilePos.y, 0xFFFF);
        if (entity->drawOrder < Zone->drawOrderLow) {
            if (BreakableWall->farPlaneLayer != -1)
                RSDK.SetTileInfo(BreakableWall->farPlaneLayer, entity->tilePos.x, entity->tilePos.y, 0xFFFF);
        }
        entity->state     = BreakableWall_State_Tile;
        entity->stateDraw = BreakableWall_StateDraw_Tile;
    }
}
void BreakableWall_State_HWall()
{
    EntityBreakableWall *entity     = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->visible = DebugMode->active;
    BreakableWall_State_HandleHWall();
}
void BreakableWall_State_HandleHWall()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    EntityPlayer *player        = NULL;

    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
#if RETRO_USE_PLUS
        if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimData.animationID == ANI_DROPDASH)) {
#endif
            if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                bool32 flag = abs(player->groundVel) >= 0x48000 && player->onGround && player->playerAnimData.animationID == ANI_JUMP;

                if (player->shield == SHIELD_FIRE) {
                    EntityShield *shield = (EntityShield * )RSDK.GetEntityByID(Player->playerCount + RSDK.GetEntityID(player));
                    flag |= shield->data.animationID == 2;
                }

                switch (player->characterID) {
                    default: break;
                    case ID_SONIC:
                        flag |= player->playerAnimData.animationID == ANI_DROPDASH;
                        flag |= player->superState == 2;
                        break;
                    case ID_KNUCKLES: flag = true; break;
                }

                //if (player->state == Ice_State_FrozenPlayer) {
                //    flag |= abs(player->groundVel) >= 0x48000;
                //}

                if (flag && !player->sidekick) {
                    if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
                        BreakableWall_BreakUnknown(entity, player->position.x > entity->position.x);
                        if (player->characterID == ID_KNUCKLES) {
                            if (player->playerAnimData.animationID == ANI_FLY) {
                                player->glideSpeedStore -= player->glideSpeedStore >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                                if (abs(player->velocity.x) <= 0x30000) {
                                    RSDK.SetSpriteAnimation(player->spriteIndex, ANI_FLYTIRED, &player->playerAnimData, 0, 0);
                                    //player->state = Player_State_KnuxGlideDrop;
                                }
                            }
                            else if (player->playerAnimData.animationID == ANI_FLYLIFTTIRED) {
                                player->glideSpeedStore -= player->glideSpeedStore >> 2;
                                player->velocity.x -= player->velocity.x >> 2;
                            }
                        }
                        RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
                        RSDK.DestroyEntity(entity, 0, 0);
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
void BreakableWall_State_Tile()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += entity->gravityStrength;
    if (entity->velocity.x)
        entity->rotation += entity->dword88;
    if (entity->drawOrder >= Zone->drawOrderLow) {
        if (!RSDK.CheckOnScreen(entity, &entity->updateRange))
            RSDK.DestroyEntity(entity, 0, 0);
    }
    else {
        if (++entity->timer == 120)
            RSDK.DestroyEntity(entity, 0, 0);
    }
}
void BreakableWall_State_Top()
{
    EntityBreakableWall *entity     = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->visible = DebugMode->active;
    BreakableWall_Break1();
}
void BreakableWall_State_VerticalChunks()
{
    EntityBreakableWall *entity     = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    RSDK_sceneInfo->entity->visible = DebugMode->active;
    BreakableWall_Break2AND3();
}
// Draw States
void BreakableWall_StateDraw_BWall()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    Vector2 drawPos;
    drawPos.x = RSDK_sceneInfo->entity->position.x;
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
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
}
void BreakableWall_StateDraw_BWall2()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    Vector2 drawPos;
    drawPos.x = RSDK_sceneInfo->entity->position.x;
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
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
    drawPos.x += entity->size.x << 20;
    entity->direction = FLIP_X;
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
    drawPos.y += entity->size.y << 20;
    entity->direction = FLIP_XY;
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
    drawPos.x -= entity->size.x << 20;
    entity->direction = FLIP_Y;
    RSDK.DrawSprite(&BreakableWall->animData, &drawPos, 0);
}
void BreakableWall_StateDraw_Tile()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    entity->angle               = entity->rotation;
    RSDK.DrawTile(&entity->tileInfo, 1, 1, 0, 0, 0);
}
// Break
void BreakableWall_Break1()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    EntityPlayer *player        = NULL;

    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimData.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 flag = player->playerAnimData.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;
                        case ID_SONIC:
                            if (!flag)
                                flag = player->playerAnimData.animationID == ANI_DROPDASH;
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
                        int tx           = entity->position.x - (entity->size.x << 19) + 0x80000;
                        int ty           = entity->position.y - (entity->size.y << 19) + 0x80000;
                        int th           = entity->position.y - (entity->size.y << 19) + 0x80000 - ((entity->size.y << 19) + entity->position.y);
                        for (int y = 0; y < entity->size.y; ++y) {
                            int posY        = ty >> 20;
                            int speed       = 3 * abs(th);
                            int offsetX     = tx - entity->position.x;
                            int blockSpeedX = 2 * (tx - entity->position.x);
                            for (int x = 0; x < entity->size.x; ++x) {
                                int posX                       = tx >> 20;
                                EntityBreakableWall *tileChunk = (EntityBreakableWall *)RSDK.SpawnEntity(BreakableWall->objectID, (void *)1, tx, ty);
                                tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, posX, posY);
                                tileChunk->drawOrder           = entity->drawOrder;
                                int angle                      = RSDK.ATan2(blockSpeedX, th);
                                int spd                        = (speed + abs(offsetX)) >> 18;
                                tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
                                tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
                                RSDK.SetTileInfo(entity->priority, posX, posY, 0xFFFF);

                                if (entity->drawOrder < Zone->drawOrderLow) {
                                    if (BreakableWall->farPlaneLayer != -1)
                                        RSDK.SetTileInfo(BreakableWall->farPlaneLayer, posX, posY, 0xFFFF);
                                }

                                blockSpeedX += 0x200000;
                                tx += 0x100000;
                                offsetX += 0x100000;
                                tx += 0x100000;
                            }
                            ty += 0x100000;
                            th += 0x100000;
                        }

                        RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
                        BreakableWall_GiveScoreBonus(player);
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y -= 0x10000;
                        else
#endif
                            player->velocity.y = -0x30000;
                        RSDK.DestroyEntity(entity, 0, 0);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break2AND3()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    EntityPlayer *player        = NULL;

    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimData.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 flag = player->playerAnimData.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;
                        case ID_SONIC:
                            if (!flag)
                                flag = player->playerAnimData.animationID == ANI_DROPDASH;
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
                            RSDK.DestroyEntity(entity, 0, 0);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break4()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    EntityPlayer *player        = NULL;

    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimData.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 flag = player->playerAnimData.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;
                        case ID_SONIC:
                            if (!flag)
                                flag = player->playerAnimData.animationID == ANI_DROPDASH;
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
                        BreakableWall_BreakV();
                        int yVel            = player->velocity.y;
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
                            RSDK.DestroyEntity(entity, 0, 0);
                        player->velocity.y = yVel;
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_Break5()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    EntityPlayer *player        = NULL;

    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        if (Player_CheckCollisionBox(player, entity, &entity->hitbox)) {
#if RETRO_USE_PLUS
            if (!entity->onlyMighty || (player->characterID == ID_MIGHTY && player->playerAnimData.animationID == ANI_DROPDASH)) {
#endif
                if (!entity->onlyKnux || player->characterID == ID_KNUCKLES) {
                    bool32 flag = player->playerAnimData.animationID == ANI_JUMP;

                    switch (player->characterID) {
                        default: break;
                        case ID_SONIC:
                            if (!flag)
                                flag = player->playerAnimData.animationID == ANI_DROPDASH;
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
                        int tx               = entity->position.x - (entity->size.x << 19) + 0x80000;
                        int tw               = entity->position.x - (entity->size.x << 19) + 0x80000;
                        int ty               = entity->position.y - (entity->size.y << 19) + 0x80000;
                        int th               = (entity->size.y << 19) + entity->position.y;
                        int offsetY          = ty - th;
                        for (int y = 0; y < entity->size.y; ++y) {
                            int posY        = ty >> 20;
                            int speed       = 3 * abs(offsetY);
                            int offsetX     = tx - entity->position.x;
                            int blockSpeedX = 2 * (tx - entity->position.x);
                            for (int x = 0; x < entity->size.x; ++x) {
                                int posX                       = tx >> 20;
                                EntityBreakableWall *tileChunk = (EntityBreakableWall *)RSDK.SpawnEntity(BreakableWall->objectID, (void *)1, tx, ty);
                                tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, posX, posY);
                                tileChunk->drawOrder           = entity->drawOrder;
                                int angle                      = RSDK.ATan2(blockSpeedX, offsetY);
                                int spd                        = (speed + abs(offsetX)) >> 18;
                                tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
                                tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
                                RSDK.SetTileInfo(entity->priority, posX, posY, 0xFFFF);

                                if (entity->drawOrder < Zone->drawOrderLow) {
                                    if (BreakableWall->farPlaneLayer != -1)
                                        RSDK.SetTileInfo(BreakableWall->farPlaneLayer, posX, posY, 0xFFFF);
                                }

                                blockSpeedX += 0x200000;
                                tx += 0x100000;
                                offsetX += 0x100000;
                                tx += 0x100000;
                            }
                            ty += 0x100000;
                            tx = tw;
                            offsetY += 0x100000;
                        }

                        RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
                        BreakableWall_GiveScoreBonus(player);
#if RETRO_USE_PLUS
                        if (player->characterID == ID_MIGHTY && player->state == Player_State_MightyHammerDrop)
                            player->velocity.y -= 0x10000;
                        else
#endif
                            player->velocity.y = -0x30000;
                        RSDK.DestroyEntity(entity, 0, 0);
                    }
                }
#if RETRO_USE_PLUS
            }
#endif
        }
    }
}
void BreakableWall_BreakV()
{
    EntityBreakableWall *entity = (EntityBreakableWall *)RSDK_sceneInfo->entity;

    int sizeX                       = entity->size.x;
    int sizeY                       = entity->size.y;
    int posX                        = entity->position.x;
    int posY                        = entity->position.y;
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
    int curX   = entity->position.x - (entity->size.x << 19) + 0x80000;
    int ty     = entity->position.y - (entity->size.y << 19) + 0x80000;
    int th     = (entity->size.y << 19) + entity->position.y;

    int yAngle = ty - th;
    for (int y = 0; y < entity->size.y; ++y) {
        int tx     = curX;
        int xDif   = curX - startX;
        int tileY  = ty >> 20;
        int xAngle = 2 * xDif;
        int speed  = 3 * abs(yAngle);
        for (int x = 0; x < entity->size.x; ++x) {
            int tileX                      = tx >> 20;
            EntityBreakableWall *tileChunk = (EntityBreakableWall *)RSDK.SpawnEntity(BreakableWall->objectID, (void *)1, tx, ty);
            tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, tileX, tileY);
            tileChunk->drawOrder           = entity->drawOrder;

            int angle                      = RSDK.ATan2(xAngle, yAngle);
            int spd                        = (speed + abs(xDif)) >> 18;
            tileChunk->velocity.x += 40 * spd * RSDK.Cos256(angle);
            tileChunk->velocity.y += 40 * spd * RSDK.Sin256(angle);
            RSDK.SetTileInfo(entity->priority, tileX, tileY, 0xFFFF);
            if (entity->drawOrder < Zone->drawOrderLow) {
                if (BreakableWall->farPlaneLayer != -1)
                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, tileX, tileY, 0xFFFF);
            }

            tx += 0x100000;
            xDif += 0x100000;
            xAngle += 0x200000;
        }
        ty += 0x100000;
        yAngle += 0x100000;
    }

    entity->size.x          = sizeX;
    entity->size.y          = sizeY;
    entity->position.x = posX;
    entity->position.y = posY;
    RSDK.PlaySFX(BreakableWall->sfx_Break, 0, 255);
}
void BreakableWall_BreakUnknown(EntityBreakableWall *entity, byte flip) {
    int startX = entity->position.x;
    int startY = entity->position.y;
    int curX   = entity->position.x - (entity->size.x << 19) + 0x80000;
    int curY   = entity->position.y - (entity->size.y << 19) + 0x80000;
    switch (flip) {
        case FLIP_NONE: startX += entity->size.x << 19; break;
        case FLIP_X: startX -= entity->size.x << 19; break;
        case FLIP_Y: startY += entity->size.y << 19; break;
        case FLIP_XY: startY -= entity->size.y << 19; break;
        default: break;
    }
    for (int y = 0; y < entity->size.y; ++y) {
        int yDif = curY - startY;
        int xDif    = curX - startX;
        int tileY  = (yDif + startY) >> 20;
        int angleX = 2 * (curX - startX);
        for (int x = 0; x < entity->size.x; ++x) {
            int tileX                      = (xDif + startX) >> 20;
            EntityBreakableWall *tileChunk = (EntityBreakableWall *)RSDK.SpawnEntity(BreakableWall->objectID, (void *)1, xDif + startX, yDif + startY);
            tileChunk->tileInfo            = RSDK.GetTileInfo(entity->priority, tileX, tileY);
            tileChunk->drawOrder           = entity->drawOrder;

            switch (flip) {
                case FLIP_NONE:
                case FLIP_X: {
                    int angle  = RSDK.ATan2(angleX, yDif);
                    int angle2 = 0;
                    if (abs(xDif) > 0x80000) {
                        if (xDif + startX >= startX)
                            angle2 = RSDK.ATan2(0x80000, yDif);
                        else
                            angle2 = RSDK.ATan2(-0x80000, yDif);
                    }
                    else {
                        angle2 = RSDK.ATan2(xDif, yDif);
                    }
                    tileChunk->velocity.x = flip == FLIP_NONE ? -0x10000 : 0x10000;
                    tileChunk->velocity.y = 0x10000;
                    tileChunk->velocity.x += 40 * (((entity->size.y << 19) + 3 * abs(xDif) - abs(yDif)) >> 18) * RSDK.Cos256(angle);
                    tileChunk->velocity.y += 32 * ((abs(yDif) + abs(xDif) + 2 * abs(yDif)) >> 18) * RSDK.Sin256(angle2);
                    break;
                }
                case FLIP_Y: {
                    int angle = RSDK.ATan2(angleX, yDif);
                    tileChunk->velocity.x += 40 * ((abs(xDif) + 3 * abs(yDif)) >> 18) * RSDK.Cos256(angle);
                    tileChunk->velocity.y += 40 * ((abs(xDif) + 3 * abs(yDif)) >> 18) * RSDK.Sin256(angle);
                }
            }

            RSDK.SetTileInfo(entity->priority, tileX, tileY, 0xFFFF);
            if (entity->drawOrder < Zone->drawOrderLow) {
                if (BreakableWall->farPlaneLayer != -1)
                    RSDK.SetTileInfo(BreakableWall->farPlaneLayer, tileX, tileY, 0xFFFF);
            }
            xDif += 0x100000;
            angleX += 0x200000;
        }
        yDif += 0x100000;
    }
}

//Misc
void BreakableWall_GiveScoreBonus(void* plr)
{
    EntityPlayer *player         = (EntityPlayer *)plr;
    EntityBreakableWall *entity  = (EntityBreakableWall *)RSDK_sceneInfo->entity;
    EntityScoreBonus *scoreBonus = (EntityScoreBonus *)RSDK.SpawnEntity(ScoreBonus->objectID, 0, entity->position.x, entity->position.y);
    scoreBonus->drawOrder        = Zone->drawOrderHigh;
    scoreBonus->data.frameID     = player->scoreBonus;
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

void BreakableWall_EditorDraw()
{

}

void BreakableWall_EditorLoad()
{

}

void BreakableWall_Serialize()
{
    RSDK_EDITABLE_VAR(BreakableWall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyKnux);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_BOOL, onlyMighty);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_ENUM, priority);
    RSDK_EDITABLE_VAR(BreakableWall, VAR_VECTOR2, size);
}

