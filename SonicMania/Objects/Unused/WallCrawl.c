// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: WallCrawl Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectWallCrawl *WallCrawl = NULL;

void WallCrawl_Update(void)
{
    RSDK_THIS(WallCrawl);
    StateMachine_Run(self->state);
}

void WallCrawl_LateUpdate(void) {}

void WallCrawl_StaticUpdate(void) {}

void WallCrawl_Draw(void)
{
    RSDK_THIS(WallCrawl);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void WallCrawl_Create(void *data)
{
    RSDK_THIS(WallCrawl);

    self->drawFX |= FX_FLIP;
    self->startPos.x    = self->position.x;
    self->startPos.y    = self->position.y;
    self->startDir      = self->direction;
    self->visible       = true;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->playerPtr     = NULL;
    if (data) {
        self->drawGroup = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 3, &self->animator, true, 0);
        self->state = WallCrawl_State_Projectile;
    }
    else {
        self->drawGroup = Zone->objectDrawGroup[0] + 1;
        RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &self->animator, true, 0);
        self->state = WallCrawl_State_Init;
    }
}

void WallCrawl_StageLoad(void)
{
    WallCrawl->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/WallCrawl.bin", SCOPE_STAGE);

    WallCrawl->hitboxBadnik.left   = -14;
    WallCrawl->hitboxBadnik.top    = -14;
    WallCrawl->hitboxBadnik.right  = 14;
    WallCrawl->hitboxBadnik.bottom = 14;

    WallCrawl->hitboxRange.left   = -128;
    WallCrawl->hitboxRange.top    = -128;
    WallCrawl->hitboxRange.right  = 128;
    WallCrawl->hitboxRange.bottom = 128;

    WallCrawl->hitboxLaser.left   = -132;
    WallCrawl->hitboxLaser.top    = 8;
    WallCrawl->hitboxLaser.right  = 132;
    WallCrawl->hitboxLaser.bottom = -8;

    WallCrawl->hitboxProjectile.left   = -24;
    WallCrawl->hitboxProjectile.top    = -1;
    WallCrawl->hitboxProjectile.right  = 24;
    WallCrawl->hitboxProjectile.bottom = 9;

    DEBUGMODE_ADD_OBJ(WallCrawl);
}

void WallCrawl_DebugSpawn(void)
{
    RSDK_THIS(WallCrawl);
    EntityWallCrawl *wallCrawl = CREATE_ENTITY(WallCrawl, NULL, self->position.x, self->position.y);
    wallCrawl->direction       = self->direction;
    wallCrawl->startDir        = self->direction;
}

void WallCrawl_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void WallCrawl_HandlePlayerInteractions(void)
{
    RSDK_THIS(WallCrawl);
    foreach_active(Player, player)
    {
        if (!self->timer) {
            if (Player_CheckCollisionTouch(player, self, &WallCrawl->hitboxRange)) {
                if (self->playerPtr) {
                    if (abs(player->position.x - self->position.x) < abs(self->playerPtr->position.x - self->position.x))
                        self->playerPtr = player;
                }
                else {
                    self->playerPtr = player;
                }
            }
        }
        if (Player_CheckBadnikTouch(player, self, &WallCrawl->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void WallCrawl_CheckOffScreen(void)
{
    RSDK_THIS(WallCrawl);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        WallCrawl_Create(NULL);
    }
}

void WallCrawl_State_Init(void)
{
    RSDK_THIS(WallCrawl);

    int32 offsetY = -0xC0000;
    self->active  = ACTIVE_NORMAL;
    if (self->direction & FLIP_Y)
        offsetY = 0xC0000;

    int32 offsetX = -0xD0000;
    if (!(self->direction & FLIP_X))
        offsetX = 0xC0000;

    RSDK.ObjectTileGrip(self, Zone->collisionLayers, (2 * ((self->direction & FLIP_X) != 0) + 1), 0, offsetX, offsetY, 8);
    self->state = WallCrawl_State_Moving;
    WallCrawl_State_Moving();
}

void WallCrawl_State_Moving(void)
{
    RSDK_THIS(WallCrawl);

    if (self->timer)
        self->timer--;

    EntityPlayer *playerPtr = self->playerPtr;
    if (playerPtr) {
        if (Player_CheckCollisionTouch(playerPtr, self, &WallCrawl->hitboxRange)) {
            if (!Player_CheckCollisionTouch(playerPtr, self, &WallCrawl->hitboxLaser)) {
                self->animator.speed = 48;
                if (playerPtr->position.y <= self->position.y) {
                    self->direction &= ~FX_ROTATE;
                    self->velocity.y = -0x10000;
                }
                else {
                    self->direction |= FX_ROTATE;
                    self->velocity.y = 0x10000;
                }
            }
            else {
                int32 offsetX = -0x180000;
                self->state   = WallCrawl_State_Idle;
                if (!(self->direction & FLIP_X))
                    offsetX = 0x180000;
                self->idleTimer                                                                                      = 30;
                CREATE_ENTITY(WallCrawl, INT_TO_VOID(true), self->position.x + offsetX, self->position.y)->direction = self->direction;
                self->timer                                                                                          = 180;
                self->playerPtr                                                                                      = NULL;
            }
        }
        else {
            self->playerPtr = NULL;
        }
    }
    else {
        self->animator.speed = 24;
        if (!(self->direction & FLIP_Y))
            self->velocity.y = -0x8000;
        else
            self->velocity.y = 0x8000;
    }

    self->position.y += self->velocity.y;
    int32 offsetY = -0xC0000;
    self->active  = ACTIVE_NORMAL;
    if (self->direction & FLIP_Y)
        offsetY = 0xC0000;

    int32 offsetX = -0xD0000;
    if (!(self->direction & FLIP_X))
        offsetX = 0xC0000;
    if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, (2 * ((self->direction & 1) != 0) + 1), 0, offsetX, offsetY, 0)) {
        self->state     = WallCrawl_State_Idle;
        self->idleTimer = 30;
        if (self->playerPtr) {
            self->playerPtr = 0;
            self->timer     = 180;
        }
    }
    RSDK.ProcessAnimation(&self->animator);
    WallCrawl_HandlePlayerInteractions();
    WallCrawl_CheckOffScreen();
}

void WallCrawl_State_Idle(void)
{
    RSDK_THIS(WallCrawl);
    if (!--self->idleTimer) {
        if (abs(self->velocity.y) == 0x8000)
            self->direction ^= FLIP_Y;
        self->state = WallCrawl_State_Moving;
        WallCrawl_State_Moving();
    }
    else {
        WallCrawl_HandlePlayerInteractions();
    }
    WallCrawl_CheckOffScreen();
}

void WallCrawl_State_Projectile(void)
{
    RSDK_THIS(WallCrawl);

    RSDK.ProcessAnimation(&self->animator);
    if (self->direction & FLIP_X)
        self->position.x += 0x40000;
    else
        self->position.x -= 0x40000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &WallCrawl->hitboxProjectile)) {
            Player_Hurt(player, self);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void WallCrawl_EditorDraw(void)
{
    RSDK_THIS(WallCrawl);
    RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void WallCrawl_EditorLoad(void)
{
    WallCrawl->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/WallCrawl.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(WallCrawl, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void WallCrawl_Serialize(void) { RSDK_EDITABLE_VAR(WallCrawl, VAR_UINT8, direction); }
