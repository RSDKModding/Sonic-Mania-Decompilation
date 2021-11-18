#include "SonicMania.h"

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
        self->drawOrder = Zone->drawOrderLow;
        RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 3, &self->animator, true, 0);
        self->state = WallCrawl_State_Projectile;
    }
    else {
        self->drawOrder = Zone->drawOrderLow + 1;
        RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &self->animator, true, 0);
        self->state = WallCrawl_State_Setup;
    }
}

void WallCrawl_StageLoad(void)
{
    WallCrawl->aniFrames                = RSDK.LoadSpriteAnimation("Blueprint/WallCrawl.bin", SCOPE_STAGE);
    WallCrawl->hitbox.left              = -14;
    WallCrawl->hitbox.top               = -14;
    WallCrawl->hitbox.right             = 14;
    WallCrawl->hitbox.bottom            = 14;
    WallCrawl->checkbox.left            = -128;
    WallCrawl->checkbox.top             = -128;
    WallCrawl->checkbox.right           = 128;
    WallCrawl->checkbox.bottom          = 128;
    WallCrawl->attackbox.left           = -132;
    WallCrawl->attackbox.top            = 8;
    WallCrawl->attackbox.right          = 132;
    WallCrawl->attackbox.bottom         = -8;
    WallCrawl->projectileHurtbox.left   = -24;
    WallCrawl->projectileHurtbox.top    = -1;
    WallCrawl->projectileHurtbox.right  = 24;
    WallCrawl->projectileHurtbox.bottom = 9;
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
    RSDK.DrawSprite(&DebugMode->animator, 0, false);
}

void WallCrawl_HandlePlayerInteractions(void)
{
    RSDK_THIS(WallCrawl);
    foreach_active(Player, player)
    {
        if (!self->timer) {
            if (Player_CheckCollisionTouch(player, self, &WallCrawl->checkbox)) {
                if (self->playerPtr) {
                    if (abs(player->position.x - self->position.x) < abs(self->playerPtr->position.x - self->position.x))
                        self->playerPtr = player;
                }
                else {
                    self->playerPtr = player;
                }
            }
        }
        if (Player_CheckBadnikTouch(player, self, &WallCrawl->hitbox))
            Player_CheckBadnikBreak(self, player, true);
    }
}

void WallCrawl_CheckOnScreen(void)
{
    RSDK_THIS(WallCrawl);
    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        WallCrawl_Create(NULL);
    }
}

void WallCrawl_State_Setup(void)
{
    RSDK_THIS(WallCrawl);

    int32 offsetY    = -0xC0000;
    self->active = ACTIVE_NORMAL;
    if (self->direction & FLIP_Y)
        offsetY = 0xC0000;

    int32 offsetX = -0xD0000;
    if (!(self->direction & FLIP_X))
        offsetX = 0xC0000;

    RSDK.ObjectTileGrip(self, Zone->fgLayers, (2 * ((self->direction & FLIP_X) != 0) + 1), 0, offsetX, offsetY, 8);
    self->state = WallCrawl_State_Main;
    WallCrawl_State_Main();
}

void WallCrawl_State_Main(void)
{
    RSDK_THIS(WallCrawl);

    if (self->timer)
        self->timer--;
    EntityPlayer *playerPtr = self->playerPtr;
    if (playerPtr) {
        if (Player_CheckCollisionTouch(playerPtr, self, &WallCrawl->checkbox)) {
            if (!Player_CheckCollisionTouch(playerPtr, self, &WallCrawl->attackbox)) {
                self->animator.animationSpeed = 48;
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
                int32 offsetX   = -0x180000;
                self->state = WallCrawl_State_Idle;
                if (!(self->direction & FLIP_X))
                    offsetX = 0x180000;
                self->idleTimer                                                                                       = 30;
                CREATE_ENTITY(WallCrawl, intToVoid(true), self->position.x + offsetX, self->position.y)->direction = self->direction;
                self->timer                                                                                          = 180;
                self->playerPtr                                                                                      = NULL;
            }
        }
        else {
            self->playerPtr = NULL;
        }
    }
    else {
        self->animator.animationSpeed = 24;
        if (!(self->direction & FLIP_Y))
            self->velocity.y = -0x8000;
        else
            self->velocity.y = 0x8000;
    }

    self->position.y += self->velocity.y;
    int32 offsetY    = -0xC0000;
    self->active = ACTIVE_NORMAL;
    if (self->direction & FLIP_Y)
        offsetY = 0xC0000;

    int32 offsetX = -0xD0000;
    if (!(self->direction & FLIP_X))
        offsetX = 0xC0000;
    if (!RSDK.ObjectTileGrip(self, Zone->fgLayers, (2 * ((self->direction & 1) != 0) + 1), 0, offsetX, offsetY, 0)) {
        self->state    = WallCrawl_State_Idle;
        self->idleTimer = 30;
        if (self->playerPtr) {
            self->playerPtr = 0;
            self->timer     = 180;
        }
    }
    RSDK.ProcessAnimation(&self->animator);
    WallCrawl_HandlePlayerInteractions();
    WallCrawl_CheckOnScreen();
}

void WallCrawl_State_Idle(void)
{
    RSDK_THIS(WallCrawl);
    if (!--self->idleTimer) {
        if (abs(self->velocity.y) == 0x8000)
            self->direction ^= FLIP_Y;
        self->state = WallCrawl_State_Main;
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

    RSDK.ProcessAnimation(&self->animator);
    if (self->direction & FLIP_X)
        self->position.x += 0x40000;
    else
        self->position.x -= 0x40000;

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &WallCrawl->projectileHurtbox)) {
            Player_CheckHit(player, self);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void WallCrawl_EditorDraw(void)
{
    RSDK_THIS(WallCrawl);
    RSDK.SetSpriteAnimation(WallCrawl->aniFrames, 2, &self->animator, true, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void WallCrawl_EditorLoad(void) {
    WallCrawl->aniFrames = RSDK.LoadSpriteAnimation("Blueprint/WallCrawl.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(WallCrawl, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
    RSDK_ENUM_VAR("Flip Y", FLIP_Y);
    RSDK_ENUM_VAR("Flip XY", FLIP_XY);
}
#endif

void WallCrawl_Serialize(void) { RSDK_EDITABLE_VAR(WallCrawl, VAR_UINT8, direction); }
