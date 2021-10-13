#include "SonicMania.h"

ObjectCaterkiller *Caterkiller = NULL;

void Caterkiller_Update(void)
{
    RSDK_THIS(Caterkiller);
    StateMachine_Run(entity->state);
}

void Caterkiller_LateUpdate(void) {}

void Caterkiller_StaticUpdate(void) {}

void Caterkiller_Draw(void)
{
    RSDK_THIS(Caterkiller);
    StateMachine_Run(entity->stateDraw);
}

void Caterkiller_Create(void *data)
{
    RSDK_THIS(Caterkiller);
    entity->visible = true;
    if (entity->planeFilter > 0 && ((entity->planeFilter - 1) & 2))
        entity->drawOrder = Zone->drawOrderHigh;
    else
        entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX |= FX_FLIP;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (data) {
        entity->state = (void (*)(void))data;
        if (entity->state == Caterkiller_State_Split_Head)
            RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &entity->animator, true, 0);
        else
            RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 1, &entity->animator, true, 0);
        entity->stateDraw = Caterkiller_StateDraw_Split;
    }
    else {
        entity->startPos = entity->position;
        entity->startDir = entity->direction;

        entity->headOffset = 0;
        int32 offset         = 0xC0000;
        if (entity->startDir)
            offset = -0xC0000;
        int32 posX = entity->position.x;
        for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
            posX += offset;
            entity->bodyPosition[i].x = posX;
            entity->bodyPosition[i].y = entity->position.y;
            entity->bodyDirection[i]  = entity->direction;
        }

        entity->timer = 0;
        RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &entity->animator, true, 0);
        RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 1, &entity->animator2, true, 0);
        entity->state     = Caterkiller_Unknown5;
        entity->stateDraw = Caterkiller_StateDraw_Body;
    }
}

void Caterkiller_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        Caterkiller->aniFrames = RSDK.LoadSpriteAnimation("MMZ/Caterkiller.bin", SCOPE_STAGE);
    Caterkiller->hitbox.left   = -8;
    Caterkiller->hitbox.top    = -8;
    Caterkiller->hitbox.right  = 8;
    Caterkiller->hitbox.bottom = 8;
    DEBUGMODE_ADD_OBJ(Caterkiller);
}

void Caterkiller_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Caterkiller->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Caterkiller_DebugSpawn(void)
{
    RSDK_THIS(Caterkiller);
    RSDK.CreateEntity(Caterkiller->objectID, 0, entity->position.x, entity->position.y);
}

void Caterkiller_CheckOnScreen(void)
{
    RSDK_THIS(Caterkiller);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Caterkiller_Create(NULL);
    }
}

void Caterkiller_CheckTileCollisions(void)
{
    RSDK_THIS(Caterkiller);
    int32 storeX = 0;
    int32 storeY = 0;

    if (entity->state == Caterkiller_Unknown7) {
        storeX = entity->position.x;
        storeY = entity->position.y;
    }
    else {
        if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8))
            entity->direction ^= 1;
        storeX = entity->position.x;
        storeY = entity->position.y;
        if (Caterkiller_CheckTileAngle(entity->position.x, entity->position.y, entity->direction))
            entity->direction ^= 1;
    }

    for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
        if (entity->state != Caterkiller_Unknown9 || i != (Caterkiller_BodyCount - 1)) {
            entity->position.x = entity->bodyPosition[i].x;
            entity->position.y = entity->bodyPosition[i].y;
            if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 8))
                entity->bodyDirection[i] = entity->direction;
            entity->bodyPosition[i].y = entity->position.y;
            if (Caterkiller_CheckTileAngle(entity->bodyPosition[i].x, entity->bodyPosition[i].y, entity->bodyDirection[i]))
                entity->bodyDirection[i] = entity->direction;
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
}

void Caterkiller_StateDraw_Body(void)
{
    RSDK_THIS(Caterkiller);
    Vector2 drawPos;

    int32 storeDir = entity->direction;
    for (int32 i = Caterkiller_BodyCount - 1; i >= 0; --i) {
        drawPos.x = entity->bodyPosition[i].x;
        drawPos.y = entity->bodyPosition[i].y;
        drawPos.y -= entity->bodyOffset[i] << 15;
        entity->direction = entity->bodyDirection[i];
        RSDK.DrawSprite(&entity->animator2, &drawPos, false);
    }

    drawPos = entity->position;
    drawPos.y -= entity->headOffset << 15;
    entity->direction = storeDir;
    RSDK.DrawSprite(&entity->animator, &drawPos, false);
}

void Caterkiller_StateDraw_Split(void)
{
    RSDK_THIS(Caterkiller);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Caterkiller_HandlePlayerInteractions(void)
{
    RSDK_THIS(Caterkiller);
    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;

    foreach_active(Player, player)
    {
        if (entity->planeFilter <= 0 || player->collisionPlane == ((entity->planeFilter - 1) & 1)) {
            if (Player_CheckBadnikHit(player, entity, &Caterkiller->hitbox)) {
                Player_CheckBadnikBreak(entity, player, true);
            }
            else {
                for (int32 i = 0; i < Caterkiller_BodyCount; ++i) {
                    entity->position.x = entity->bodyPosition[i].x;
                    entity->position.y = entity->bodyPosition[i].y;

                    if (Player_CheckCollisionTouch(player, entity, &Caterkiller->hitbox)) {
                        Player_CheckHit(player, entity);

                        for (int32 d = 0; d < Caterkiller_BodyCount + 1; ++d) {
                            int32 spawnX       = storeX;
                            int32 spawnY       = storeY;
                            int32 spawnDir     = entity->direction;
                            void *spawnState = Caterkiller_State_Split_Head;
                            if (d) {
                                spawnX     = entity->bodyPosition[d - 1].x;
                                spawnY     = entity->bodyPosition[d - 1].y;
                                spawnDir   = entity->bodyDirection[d - 1];
                                spawnState = Caterkiller_State_Split_Body;
                            }

                            EntityCaterkiller *debris = CREATE_ENTITY(Caterkiller, spawnState, spawnX, spawnY);
                            debris->direction         = spawnDir;
                            if (!debris->direction)
                                debris->velocity.x = (d & 1) ? -0x18000 : -0x20000;
                            else
                                debris->velocity.x = (d & 1) ? 0x18000 : 0x20000;

                            if ((d & 3) >= 2)
                                debris->velocity.x = -debris->velocity.x;

                            debris->velocity.y       = -0x40000;
                            if (!d)
                                debris->animator.frameID = entity->animator.frameID;
                            debris->planeFilter      = entity->planeFilter;
                            debris->drawOrder        = entity->drawOrder;
                        }

                        destroyEntity(entity);
                        entity->active = ACTIVE_NEVER2;
                        break;
                    }
                }
            }
            entity->position.x = storeX;
            entity->position.y = storeY;
        }
    }
}

bool32 Caterkiller_CheckTileAngle(int32 x, int32 y, int32 dir)
{
    int32 ty      = (y >> 16) + 8;
    int32 tx      = x >> 16;
    uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, x >> 16, ty);
    if (tile == 0xFFFF)
        tile = RSDK.GetTileInfo(Zone->fgLow, tx, ty);
    uint8 angle = RSDK.GetTileAngle(tile, 0, 0);

    if (dir) {
        if ((uint8)(angle + 127) <= 0x66)
            return true;
    }
    else if ((uint8)(angle - 25) <= 0x66) {
        return true;
    }
    return false;
}

void Caterkiller_Unknown5(void)
{
    RSDK_THIS(Caterkiller);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Caterkiller_Unknown6;

    Caterkiller_Unknown6();
}

void Caterkiller_Unknown6(void)
{
    RSDK_THIS(Caterkiller);
    if (entity->timer) {
        entity->timer--;
        Caterkiller_HandlePlayerInteractions();
        Caterkiller_CheckOnScreen();
    }
    else {
        entity->timer            = 15;
        entity->animator.frameID = 1;
        entity->state            = Caterkiller_Unknown7;
        Caterkiller_Unknown7();
    }
}

void Caterkiller_Unknown7(void)
{
    RSDK_THIS(Caterkiller);
    if (entity->timer) {
        entity->timer--;
        for (int32 b = 0; b < Caterkiller_BodyCount; ++b) {
            if (entity->bodyDirection[b])
                entity->bodyPosition[b].x += 0x4000 * (b + 1);
            else
                entity->bodyPosition[b].x -= 0x4000 * (b + 1);
        }
        ++entity->headOffset;
        ++entity->bodyOffset[Caterkiller_BodyCount / 2];
    }
    else {
        entity->timer = 7;
        entity->state = Caterkiller_Unknown8;
    }
    Caterkiller_CheckTileCollisions();
    Caterkiller_HandlePlayerInteractions();
    Caterkiller_CheckOnScreen();
}

void Caterkiller_Unknown8(void)
{
    RSDK_THIS(Caterkiller);
    if (entity->timer) {
        entity->timer--;
        Caterkiller_HandlePlayerInteractions();
        Caterkiller_CheckOnScreen();
    }
    else {
        entity->timer            = 15;
        entity->animator.frameID = 0;
        entity->state            = Caterkiller_Unknown9;
        Caterkiller_Unknown9();
    }
}

void Caterkiller_Unknown9(void)
{
    RSDK_THIS(Caterkiller);
    if (entity->timer) {
        entity->timer--;

        int32 mult = 1;
        for (int32 b = Caterkiller_BodyCount - 2; b >= 0; --b) {
            if (entity->bodyDirection[b])
                entity->bodyPosition[b].x += 0x4000 * mult;
            else
                entity->bodyPosition[b].x -= 0x4000 * mult;
            ++mult;
        }

        if (entity->direction)
            entity->position.x += 0x4000 * mult;
        else
            entity->position.x -= 0x4000 * mult;

        --entity->headOffset;
        --entity->bodyOffset[Caterkiller_BodyCount / 2];
    }
    else {
        entity->timer = 7;
        entity->state = Caterkiller_Unknown6;
    }
    Caterkiller_CheckTileCollisions();
    Caterkiller_HandlePlayerInteractions();
    Caterkiller_CheckOnScreen();
}

void Caterkiller_State_Split_Head(void)
{
    RSDK_THIS(Caterkiller);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        if (entity->velocity.y > 0 && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 4)) {
            entity->velocity.y = -0x40000;
        }

        foreach_active(Player, player)
        {
            if ((entity->planeFilter <= 0 || player->collisionPlane == ((entity->planeFilter - 1) & 1))
                && Player_CheckBadnikHit(player, entity, &Caterkiller->hitbox)) {
                Player_CheckBadnikBreak(entity, player, true);
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Caterkiller_State_Split_Body(void)
{
    RSDK_THIS(Caterkiller);
    entity->position.x += entity->velocity.x;
    entity->position.y += entity->velocity.y;
    entity->velocity.y += 0x3800;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        if (entity->velocity.y > 0 && RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 4)) {
            entity->velocity.y = -0x40000;
        }

        foreach_active(Player, player)
        {
            if (entity->planeFilter <= 0 || player->collisionPlane == ((entity->planeFilter - 1) & 1)) {
                if (Player_CheckCollisionTouch(player, entity, &Caterkiller->hitbox)) {
                    Player_CheckHit(player, entity);
                }
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void Caterkiller_EditorDraw(void) {}

void Caterkiller_EditorLoad(void) {}

void Caterkiller_Serialize(void) { RSDK_EDITABLE_VAR(Caterkiller, VAR_ENUM, planeFilter); }
