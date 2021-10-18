#include "SonicMania.h"

ObjectWallCrawl *WallCrawl = NULL;

void WallCrawl_Update(void)
{
    RSDK_THIS(WallCrawl);
    StateMachine_Run(entity->state);
}

void WallCrawl_LateUpdate(void) {}

void WallCrawl_StaticUpdate(void) {}

void WallCrawl_Draw(void)
{
    RSDK_THIS(WallCrawl);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void WallCrawl_Create(void *data)
{
    RSDK_THIS(WallCrawl);

    entity->drawFX |= FX_FLIP;
    entity->startPos.x    = entity->position.x;
    entity->startPos.y    = entity->position.y;
    entity->startDir      = entity->direction;
    entity->visible       = true;
    entity->active        = ACTIVE_BOUNDS;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->playerPtr     = NULL;
    if (data) {
        entity->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 3, &entity->animator, true, 0);
        entity->state = WallCrawl_State_Projectile;
    }
    else {
        entity->drawOrder = Zone->drawOrderLow + 1;
        RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &entity->animator, true, 0);
        entity->state = WallCrawl_State_Setup;
    }
}

void WallCrawl_StageLoad(void)
{
    WallCrawl->aniFrames               = RSDK.LoadSpriteAnimation("Blueprint/WallCrawl.bin", SCOPE_STAGE);
    WallCrawl->hitboxBadnik.left       = -14;
    WallCrawl->hitboxBadnik.top        = -14;
    WallCrawl->hitboxBadnik.right      = 14;
    WallCrawl->hitboxBadnik.bottom     = 14;
    WallCrawl->hitbox2.left            = -128;
    WallCrawl->hitbox2.top             = -128;
    WallCrawl->hitbox2.right           = 128;
    WallCrawl->hitbox2.bottom          = 128;
    WallCrawl->hitbox3.left            = -132;
    WallCrawl->hitbox3.top             = 8;
    WallCrawl->hitbox3.right           = 132;
    WallCrawl->hitbox3.bottom          = -8;
    WallCrawl->hitboxProjectile.left   = -24;
    WallCrawl->hitboxProjectile.top    = -1;
    WallCrawl->hitboxProjectile.right  = 24;
    WallCrawl->hitboxProjectile.bottom = 9;
    DEBUGMODE_ADD_OBJ(WallCrawl);
}

void WallCrawl_DebugSpawn(void)
{
    RSDK_THIS(WallCrawl);
    EntityWallCrawl *wallCrawl = CREATE_ENTITY(WallCrawl, NULL, entity->position.x, entity->position.y);
    wallCrawl->direction       = entity->direction;
    wallCrawl->startDir        = entity->direction;
}

void WallCrawl_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void WallCrawl_HandlePlayerInteractions(void)
{
    RSDK_THIS(WallCrawl);
    foreach_active(Player, player)
    {
        if (!entity->timer) {
            if (Player_CheckCollisionTouch(player, entity, &WallCrawl->hitbox2)) {
                if (entity->playerPtr) {
                    if (abs(player->position.x - entity->position.x) < abs(entity->playerPtr->position.x - entity->position.x))
                        entity->playerPtr = player;
                }
                else {
                    entity->playerPtr = player;
                }
            }
        }
        if (Player_CheckBadnikTouch(player, entity, &WallCrawl->hitboxBadnik))
            Player_CheckBadnikBreak(entity, player, true);
    }
}

void WallCrawl_CheckOnScreen(void)
{
    RSDK_THIS(WallCrawl);
    if (!RSDK.CheckOnScreen(RSDK_sceneInfo->entity, 0) && !RSDK.CheckPosOnScreen(&entity->startPos, &entity->updateRange)) {
        entity->position  = entity->startPos;
        entity->direction = entity->startDir;
        WallCrawl_Create(NULL);
    }
}

void WallCrawl_State_Setup(void)
{
    RSDK_THIS(WallCrawl);

    int32 offsetY    = -0xC0000;
    entity->active = ACTIVE_NORMAL;
    if (entity->direction & FLIP_Y)
        offsetY = 0xC0000;

    int32 offsetX = -0xD0000;
    if (!(entity->direction & FLIP_X))
        offsetX = 0xC0000;

    RSDK.ObjectTileGrip(entity, Zone->fgLayers, (2 * ((entity->direction & FLIP_X) != 0) + 1), 0, offsetX, offsetY, 8);
    entity->state = WallCrawl_State_Main;
    WallCrawl_State_Main();
}

void WallCrawl_State_Main(void)
{
    RSDK_THIS(WallCrawl);

    if (entity->timer)
        entity->timer--;
    EntityPlayer *playerPtr = entity->playerPtr;
    if (playerPtr) {
        if (Player_CheckCollisionTouch(playerPtr, entity, &WallCrawl->hitbox2)) {
            if (!Player_CheckCollisionTouch(playerPtr, entity, &WallCrawl->hitbox3)) {
                entity->animator.animationSpeed = 48;
                if (playerPtr->position.y <= entity->position.y) {
                    entity->direction &= ~FX_ROTATE;
                    entity->velocity.y = -0x10000;
                }
                else {
                    entity->direction |= FX_ROTATE;
                    entity->velocity.y = 0x10000;
                }
            }
            else {
                int32 offsetX   = -0x180000;
                entity->state = WallCrawl_State_Idle;
                if (!(entity->direction & FLIP_X))
                    offsetX = 0x180000;
                entity->idleTimer                                                                                       = 30;
                CREATE_ENTITY(WallCrawl, intToVoid(true), entity->position.x + offsetX, entity->position.y)->direction = entity->direction;
                entity->timer                                                                                          = 180;
                entity->playerPtr                                                                                      = NULL;
            }
        }
        else {
            entity->playerPtr = NULL;
        }
    }
    else {
        entity->animator.animationSpeed = 24;
        if (!(entity->direction & FLIP_Y))
            entity->velocity.y = -0x8000;
        else
            entity->velocity.y = 0x8000;
    }

    entity->position.y += entity->velocity.y;
    int32 offsetY    = -0xC0000;
    entity->active = ACTIVE_NORMAL;
    if (entity->direction & FLIP_Y)
        offsetY = 0xC0000;

    int32 offsetX = -0xD0000;
    if (!(entity->direction & FLIP_X))
        offsetX = 0xC0000;
    if (!RSDK.ObjectTileGrip(entity, Zone->fgLayers, (2 * ((entity->direction & 1) != 0) + 1), 0, offsetX, offsetY, 0)) {
        entity->state    = WallCrawl_State_Idle;
        entity->idleTimer = 30;
        if (entity->playerPtr) {
            entity->playerPtr = 0;
            entity->timer     = 180;
        }
    }
    RSDK.ProcessAnimation(&entity->animator);
    WallCrawl_HandlePlayerInteractions();
    WallCrawl_CheckOnScreen();
}

void WallCrawl_State_Idle(void)
{
    RSDK_THIS(WallCrawl);
    if (!--entity->idleTimer) {
        if (abs(entity->velocity.y) == 0x8000)
            entity->direction ^= FLIP_Y;
        entity->state = WallCrawl_State_Main;
        WallCrawl_State_Main();
    }
    else {
        WallCrawl_HandlePlayerInteractions();
    }
    WallCrawl_CheckOnScreen();
}

void WallCrawl_State_Projectile(void)
{
    RSDK_THIS(WallCrawl);

    RSDK.ProcessAnimation(&entity->animator);
    if (entity->direction & FLIP_X)
        entity->position.x += 0x40000;
    else
        entity->position.x -= 0x40000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &WallCrawl->hitboxProjectile)) {
            Player_CheckHit(player, entity);
        }
    }
}

void WallCrawl_EditorDraw(void) {}

void WallCrawl_EditorLoad(void) {}

void WallCrawl_Serialize(void) { RSDK_EDITABLE_VAR(WallCrawl, VAR_UINT8, direction); }
