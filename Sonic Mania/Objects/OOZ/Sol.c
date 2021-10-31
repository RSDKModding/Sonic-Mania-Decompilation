#include "SonicMania.h"

ObjectSol *Sol;

void Sol_Update(void)
{
    RSDK_THIS(Sol);
    StateMachine_Run(entity->state);
}

void Sol_LateUpdate(void) {}

void Sol_StaticUpdate(void) {}

void Sol_Draw(void)
{
    RSDK_THIS(Sol);
    for (int32 i = 0; i < Sol_MaxFlameOrbs; ++i) {
        if ((1 << i) & entity->activeOrbs)
            RSDK.DrawSprite(&entity->animator2, &entity->positions[i], false);
    }

    RSDK.DrawSprite(&entity->animator1, NULL, false);
}

void Sol_Create(void *data)
{
    RSDK_THIS(Sol);
    entity->visible   = true;
    entity->drawOrder = Zone->drawOrderLow;
    entity->drawFX    = FX_FLIP;
    if (data) {
        RSDK.SetSpriteAnimation(Sol->aniFrames, 1, &entity->animator1, true, 0);
        entity->active = ACTIVE_NORMAL;
        entity->drawFX |= FX_ROTATE;
        entity->updateRange.x = 0x1000000;
        entity->updateRange.y = 0x1000000;
        entity->state         = Sol_Unknown8;
    }
    else {
        entity->startPos      = entity->position;
        entity->startDir      = entity->direction;
        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->activeOrbs    = 2 | 8;
        RSDK.SetSpriteAnimation(Sol->aniFrames, 0, &entity->animator1, true, 0);
        RSDK.SetSpriteAnimation(Sol->aniFrames, 1, &entity->animator2, true, 0);
        entity->state = Sol_State_Setup;

        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
    }
}

void Sol_StageLoad(void)
{
    if (RSDK.CheckStageFolder("OOZ1") || RSDK.CheckStageFolder("OOZ2"))
        Sol->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Sol.bin", SCOPE_STAGE);
    Sol->hitbox1.left   = -8;
    Sol->hitbox1.top    = -8;
    Sol->hitbox1.right  = 8;
    Sol->hitbox1.bottom = 8;
    Sol->hitbox2.left   = -4;
    Sol->hitbox2.top    = -4;
    Sol->hitbox2.right  = 4;
    Sol->hitbox2.bottom = 4;
    DEBUGMODE_ADD_OBJ(Sol);
}

void Sol_DebugSpawn(void)
{
    RSDK_THIS(Sol);
    CREATE_ENTITY(Sol, NULL, entity->position.x, entity->position.y);
}

void Sol_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Sol->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void Sol_HandlePlayerInteractions(void)
{
    RSDK_THIS(Sol);

    int32 storeX = entity->position.x;
    int32 storeY = entity->position.y;
    for (int32 i = 0; i < Sol_MaxFlameOrbs; ++i) {
        if ((1 << i) & entity->activeOrbs) {
            entity->position.x = entity->positions[i].x;
            entity->position.y = entity->positions[i].y;
            foreach_active(Player, player)
            {
#if RETRO_USE_PLUS
                if (player->state != Player_State_MightyHammerDrop) {
#endif
                    Sol_HandlePlayerHurt();
#if RETRO_USE_PLUS
                }
#endif
            }
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;

    foreach_active(Player, player)
    {
        if (Player_CheckBadnikTouch(player, entity, &Sol->hitbox1) && Player_CheckBadnikBreak(entity, player, false)) {
            int32 angle = entity->angle;
            for (int32 i = 0; i < Sol_MaxFlameOrbs; ++i) {
                if ((1 << i) & entity->activeOrbs) {
                    entity->position.x = entity->positions[i].x;
                    entity->position.y = entity->positions[i].y;
                    EntitySol *sol     = CREATE_ENTITY(Sol, intToVoid(1), entity->positions[i].x, entity->positions[i].y);

                    sol->state = Sol_Unknown9;
#if RETRO_USE_PLUS
                    if (player->state == Player_State_MightyHammerDrop)
                        sol->interaction = false;
#endif
                    sol->velocity.x = 0x380 * RSDK.Cos256(angle);
                    sol->velocity.y = 0x380 * RSDK.Sin256(angle);
                }
                angle += (0x100 / Sol_MaxFlameOrbs);
            }
            destroyEntity(entity);
        }
    }
}

void Sol_HandlePlayerHurt(void)
{
    RSDK_THIS(Sol);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &Sol->hitbox2)) {
            Player_CheckElementalHit(player, entity, SHIELD_FIRE);
        }
    }
}

void Sol_HandleRotation(void)
{
    RSDK_THIS(Sol);

    int32 angle = entity->angle;
    if (entity->direction)
        entity->angle = (angle - 1) & 0xFF;
    else
        entity->angle = (angle + 1) & 0xFF;

    for (int32 i = 0; i < Sol_MaxFlameOrbs; ++i) {
        if ((1 << i) & entity->activeOrbs) {
            entity->positions[i].x = (RSDK.Cos256(angle) << 12) + entity->position.x;
            entity->positions[i].y = (RSDK.Sin256(angle) << 12) + entity->position.y;
        }
        angle += (0x100 / Sol_MaxFlameOrbs);
    }
}

void Sol_CheckOnScreen(void)
{
    RSDK_THIS(Sol);
    if (!RSDK.CheckOnScreen(entity, NULL) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        Sol_Create(NULL);
    }
}

void Sol_State_Setup(void)
{
    RSDK_THIS(Sol);
    entity->active = ACTIVE_NORMAL;
    entity->state  = Sol_Unknown5;
    Sol_Unknown5();
}

void Sol_Unknown5(void)
{
    RSDK_THIS(Sol);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.y = (RSDK.Sin256(entity->field_88) << 10) + entity->startPos.y;
    entity->field_88 += 4;
    entity->position.x += entity->velocity.x;
    Sol_HandleRotation();
    Sol_HandlePlayerInteractions();

    if (entity->fireOrbs) {
        EntityPlayer *playerPtr = NULL;
        int32 distanceX           = 0x7FFFFFFF;
        int32 distanceY           = 0x7FFFFFFF;
        foreach_active(Player, player)
        {
            if (abs(player->position.y - entity->position.y) < distanceY)
                distanceY = abs(player->position.y - entity->position.y);

            if (abs(player->position.y - entity->position.y) < 0x400000) {
                if (!playerPtr) {
                    if (abs(player->position.x - entity->position.x) < distanceX) {
                        distanceX = abs(player->position.x - entity->position.x);
                        playerPtr = player;
                    }
                }
                else {
                    if (abs(player->position.x - entity->position.x) < distanceX) {
                        distanceX = abs(player->position.x - entity->position.x);
                        playerPtr = player;
                    }
                }
            }
        }

        if (!playerPtr)
            playerPtr = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (distanceX <= 0x800000) {
            entity->state             = Sol_Unknown6;
            entity->animator1.frameID = 1;
        }

        entity->direction = playerPtr->position.x >= entity->position.x;
    }

    Sol_CheckOnScreen();
}

void Sol_Unknown6(void)
{
    RSDK_THIS(Sol);
    RSDK.ProcessAnimation(&entity->animator2);

    entity->position.y = (RSDK.Sin256(entity->field_88) << 10) + entity->startPos.y;
    entity->field_88 += 4;
    entity->position.x += entity->velocity.x;
    int32 angle = entity->angle;
    Sol_HandleRotation();

    for (int32 i = 0; i < Sol_MaxFlameOrbs; ++i) {
        if (angle == 64) {
            if ((1 << i) & entity->activeOrbs) {
                entity->activeOrbs &= ~(1 << i);
                EntitySol *sol = CREATE_ENTITY(Sol, intToVoid(1), entity->positions[i].x, entity->positions[i].y);
                if (entity->direction == FLIP_NONE)
                    sol->velocity.x = -0x20000;
                else
                    sol->velocity.x = 0x20000;
            }
            angle += (0x100 / Sol_MaxFlameOrbs);
        }
    }
    Sol_HandlePlayerInteractions();

    if (!entity->activeOrbs) {
        entity->state = Sol_Unknown7;
        if (entity->direction == FLIP_NONE)
            entity->velocity.x = -0x4000;
        else
            entity->velocity.x = 0x4000;
    }

    if (entity->animator1.animationTimer >= 0x10)
        entity->animator1.frameID = 2;
    else
        entity->animator1.animationTimer++;
    Sol_CheckOnScreen();
}

void Sol_Unknown7(void)
{
    RSDK_THIS(Sol);
    RSDK.ProcessAnimation(&entity->animator2);
    entity->position.y = (RSDK.Sin256(entity->field_88) << 10) + entity->startPos.y;
    entity->field_88 += 4;
    entity->position.x += entity->velocity.x;
    Sol_HandleRotation();
    Sol_HandlePlayerInteractions();
    Sol_CheckOnScreen();
}

void Sol_Unknown8(void)
{
    RSDK_THIS(Sol);
    RSDK.ProcessAnimation(&entity->animator1);
    entity->position.x += entity->velocity.x;
    if (RSDK.CheckOnScreen(entity, &entity->updateRange))
        Sol_HandlePlayerHurt();
    else
        destroyEntity(entity);
}

void Sol_Unknown9(void)
{
    RSDK_THIS(Sol);
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        entity->position.x += entity->velocity.x;
        entity->position.y += entity->velocity.y;
        entity->velocity.y += 0x3800;
        entity->rotation = 2 * RSDK.ATan2(entity->velocity.x >> 16, entity->velocity.y >> 16) + 384;

        int32 offsetX = RSDK.Sin512(512 - entity->rotation) << 10;
        int32 offsetY = RSDK.Cos512(512 - entity->rotation) << 10;
        int32 cmode   = 3 - (((entity->rotation - 64) >> 7) & 3);

        bool32 collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, cmode, 1, offsetX, offsetY, true);
        if (!collided) {
            collided = RSDK.ObjectTileCollision(entity, Zone->fgLayers, cmode, 0, offsetX, offsetY, true);
        }

        if (collided) {
            RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &entity->animator1, true, 0);
            entity->state    = Sol_Unknown10;
            entity->rotation = (entity->rotation + 64) & 0x180;
        }

        int32 spawnX  = entity->position.x + offsetX;
        int32 spawnY  = entity->position.y + offsetY;
        uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, spawnX >> 20, (spawnY - 0x10000) >> 20);
        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgLow, spawnX >> 20, (spawnY - 0x10000) >> 20);

        int32 behaviour = RSDK.GetTileBehaviour(tile, 0);
        if (((behaviour == 2 || behaviour == 3) && collided) || behaviour == 1) {
            entity->position.y = spawnY;
            entity->position.y -= 0x80000;
            entity->position.x = spawnX;
            entity->position.x -= 0x40000;
            entity->rotation   = 0;
            entity->velocity.x = -0x40000;
            entity->velocity.y = 0;
            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &entity->animator1, true, 0);
            entity->state = Sol_Unknown11;

            EntitySol *sol  = CREATE_ENTITY(Sol, intToVoid(1), spawnX, spawnY - 0x80000);
            sol->velocity.x = 0x40000;
            sol->velocity.y = 0;
            RSDK.SetSpriteAnimation(Sol->aniFrames, 3, &sol->animator1, true, 0);
            sol->state    = Sol_Unknown11;
            sol->field_88 = sol->position.x & 0xF00000;

            if (behaviour == 1) {
                entity->position.y = (entity->position.y & 0xFFF00000) + 0x20000;
                sol->position.y    = (sol->position.y & 0xFFF00000) + 0x20000;
                sol->state         = Sol_Unknown12;
                entity->state      = Sol_Unknown12;
            }
            else {
                entity->position.y -= 0x80000;
            }
        }

        if (entity->interaction)
            Sol_HandlePlayerHurt();
    }
    else {
        destroyEntity(entity);
    }
}

void Sol_Unknown10(void)
{
    RSDK_THIS(Sol);
    RSDK.ProcessAnimation(&entity->animator1);
    if (!RSDK.CheckOnScreen(entity, &entity->updateRange) || entity->animator1.frameID == entity->animator1.frameCount - 1) {
        destroyEntity(entity);
    }
}

void Sol_Unknown11(void)
{
    RSDK_THIS(Sol);
    if (RSDK.CheckOnScreen(RSDK_sceneInfo->entity, &RSDK_sceneInfo->entity->updateRange)) {
        bool32 collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 1, 0, 0x80000, 16);
        if (!collided)
            collided = RSDK.ObjectTileGrip(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, 0x80000, 16);

        if (collided) {
            uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, (entity->position.y + 0x90000) >> 20);
            if (tile == 0xFFFF)
                tile = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 20, (entity->position.y + 0x90000) >> 20);
            entity->rotation = 2 * RSDK.GetTileAngle(tile, 0, 0);
        }

        uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, (entity->position.y + 0x70000) >> 20);
        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 20, (entity->position.y + 0x70000) >> 20);

        int32 behaviour = RSDK.GetTileBehaviour(tile, 0);
        if (!behaviour || behaviour == 4) {
            if (collided) {
                RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &entity->animator1, true, 0);
                entity->state = Sol_Unknown10;
            }
            else {
                entity->state = Sol_Unknown9;
            }
        }
        else {
            entity->position.y -= 0x80000;
            if ((entity->position.x & 0xF00000) != entity->field_88)
                OOZSetup_Unknown6(entity->position.y & 0xFFFF0000, (entity->position.x & 0xFFF00000) + 0x70000, entity->rotation >> 1);
            entity->field_88 = entity->position.x & 0xF00000;
        }
        entity->position.x += entity->velocity.x;
        entity->position.y += 0x80000;
        RSDK.ProcessAnimation(&entity->animator1);
        Sol_HandlePlayerHurt();
    }
    else {
        destroyEntity(entity);
    }
}

void Sol_Unknown12(void)
{
    RSDK_THIS(Sol);
    if (RSDK.CheckOnScreen(entity, &entity->updateRange)) {
        uint16 tile = RSDK.GetTileInfo(Zone->fgHigh, entity->position.x >> 20, (entity->position.y + 0xF0000) >> 20);
        if (tile == 0xFFFF)
            tile = RSDK.GetTileInfo(Zone->fgLow, entity->position.x >> 20, (entity->position.y + 0xF0000) >> 20);

        if (RSDK.GetTileBehaviour(tile, 0) == 1) {
            if ((entity->position.x & 0xF00000) != entity->field_88)
                OOZSetup_Unknown6(entity->position.y & 0xFFFF0000, (entity->position.x & 0xFFF00000) + 0x70000, entity->rotation >> 1);
            entity->field_88 = entity->position.x & 0xF00000;
        }
        else {
            RSDK.SetSpriteAnimation(Sol->aniFrames, 2, &entity->animator1, true, 0);
            entity->state = Sol_Unknown10;
        }
        entity->position.x += entity->velocity.x;
        RSDK.ProcessAnimation(&entity->animator1);
        Sol_HandlePlayerHurt();
    }
    else {
        destroyEntity(entity);
    }
}

#if RETRO_INCLUDE_EDITOR
void Sol_EditorDraw(void)
{
    RSDK_THIS(Sol);

    int32 angle = entity->angle;
    Sol_HandleRotation();
    entity->angle = angle;

    Sol_Draw();
}

void Sol_EditorLoad(void) { Sol->aniFrames = RSDK.LoadSpriteAnimation("OOZ/Sol.bin", SCOPE_STAGE); }
#endif

void Sol_Serialize(void)
{
    RSDK_EDITABLE_VAR(Sol, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Sol, VAR_BOOL, fireOrbs);
}
