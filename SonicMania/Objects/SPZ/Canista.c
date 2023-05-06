// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Canista Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectCanista *Canista;

// This object looks like it was based on Unused/WallCrawl
// Would explain WallCrawl's presence in the blueprint sheet, being a "proto" form of this

void Canista_Update(void)
{
    RSDK_THIS(Canista);

    StateMachine_Run(self->state);
}

void Canista_LateUpdate(void) {}

void Canista_StaticUpdate(void) {}

void Canista_Draw(void)
{
    RSDK_THIS(Canista);

    RSDK.DrawSprite(&self->mainAnimator, NULL, false);
    RSDK.DrawSprite(&self->tapeAnimator, NULL, false);
    RSDK.DrawSprite(&self->cannonAnimator, NULL, false);
}

void Canista_Create(void *data)
{
    RSDK_THIS(Canista);

    self->visible   = true;
    self->drawGroup = Zone->objectDrawGroup[0] + 1;

    if (data) {
        self->drawFX |= FX_FLIP;
        self->inkEffect     = INK_ALPHA;
        self->alpha         = 0x100;
        self->stopTimer     = 90;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->active        = ACTIVE_NORMAL;
        self->drawGroup     = Zone->objectDrawGroup[0];
        RSDK.SetSpriteAnimation(Canista->aniFrames, 3, &self->mainAnimator, true, 0);
        self->state = Canista_StateProjectile_Shot;
    }
    else {
        self->drawFX |= FX_ROTATE | FX_FLIP;
        self->shootOffset <<= 16;
        self->shootSize <<= 16;

        self->startPos      = self->position;
        self->startDir      = self->direction;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;

        self->detectedPlayer      = NULL;
        self->hitboxDetect.left   = -self->detectSize.x >> 17;
        self->hitboxDetect.top    = -self->detectSize.y >> 17;
        self->hitboxDetect.right  = self->detectSize.x >> 17;
        self->hitboxDetect.bottom = self->detectSize.y >> 17;

        RSDK.SetSpriteAnimation(Canista->aniFrames, 0, &self->mainAnimator, true, 0);
        RSDK.SetSpriteAnimation(Canista->aniFrames, 1, &self->tapeAnimator, true, 0);
        RSDK.SetSpriteAnimation(Canista->aniFrames, 2, &self->cannonAnimator, true, 0);
        self->state = Canista_State_Init;
    }
}

void Canista_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Canista.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ2"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Canista.bin", SCOPE_STAGE);

    Canista->hitboxBadnik.left   = -14;
    Canista->hitboxBadnik.top    = -14;
    Canista->hitboxBadnik.right  = 14;
    Canista->hitboxBadnik.bottom = 14;

    Canista->hitboxRange.left   = -128;
    Canista->hitboxRange.top    = -128;
    Canista->hitboxRange.right  = 128;
    Canista->hitboxRange.bottom = 128;

    Canista->hitboxDetect.left   = -132;
    Canista->hitboxDetect.top    = 48;
    Canista->hitboxDetect.right  = 132;
    Canista->hitboxDetect.bottom = 49;

    Canista->hitboxProjectile.left   = -26;
    Canista->hitboxProjectile.top    = -5;
    Canista->hitboxProjectile.right  = -16;
    Canista->hitboxProjectile.bottom = 5;

    DEBUGMODE_ADD_OBJ(Canista);

    Canista->sfxPon = RSDK.GetSfx("Stage/Pon.wav");
}

void Canista_DebugSpawn(void)
{
    RSDK_THIS(DebugMode);

    EntityCanista *canista = CREATE_ENTITY(Canista, NULL, self->position.x, self->position.y);
    canista->direction     = self->direction;
    canista->startDir      = self->direction;
}

void Canista_DebugDraw(void)
{
    RSDK.SetSpriteAnimation(Canista->aniFrames, 0, &DebugMode->animator, true, 0);
    RSDK.DrawSprite(&DebugMode->animator, NULL, false);
}

void Canista_CheckPlayerBadnikCollisions(void)
{
    RSDK_THIS(Canista);

    int32 storeX = self->position.x;
    int32 storeY = self->position.y;

    foreach_active(Player, player)
    {
        if (!self->detectDelay) {
            if (self->triggerMode) {
                self->position.x = self->startPos.x + self->detectOffset.x;
                self->position.y = self->startPos.y + self->detectOffset.y;
                if (Player_CheckCollisionTouch(player, self, &self->hitboxDetect)) {
                    if (self->detectedPlayer) {
                        if (abs(self->position.x - player->position.x) < abs(self->position.x - self->detectedPlayer->position.x))
                            self->detectedPlayer = player;
                    }
                    else {
                        self->detectedPlayer = player;
                    }
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &Canista->hitboxDetect)) {
                    if (self->detectedPlayer) {
                        if (abs(self->position.x - player->position.x) < abs(self->position.x - self->detectedPlayer->position.x))
                            self->detectedPlayer = player;
                    }
                    else {
                        self->detectedPlayer = player;
                    }
                }
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;

        if (Player_CheckBadnikTouch(player, self, &Canista->hitboxBadnik))
            Player_CheckBadnikBreak(player, self, true);
    }
}

void Canista_CheckOffScreen(void)
{
    RSDK_THIS(Canista);

    if (!RSDK.CheckOnScreen(self, NULL) && !RSDK.CheckPosOnScreen(&self->startPos, &self->updateRange)) {
        self->position  = self->startPos;
        self->direction = self->startDir;
        self->shootOffset >>= 16;

        Canista_Create(NULL);
    }
}

void Canista_State_Init(void)
{
    RSDK_THIS(Canista);

    self->active = ACTIVE_NORMAL;
    int32 offset = !(self->direction & FLIP_X) ? 0x100000 : -0x110000;

    RSDK.ObjectTileGrip(self, Zone->collisionLayers, (2 * ((self->direction & FLIP_X) != 0) + 1), 0, offset, -0x180000, 8);
    self->moveDir       = 0;
    self->timer         = 0;
    self->stopTimer     = 0;
    self->updateRange.y = 0x2000000;
    self->state         = Canista_State_Moving;
    Canista_State_Moving();
}

void Canista_State_Moving(void)
{
    RSDK_THIS(Canista);

    if (self->detectDelay)
        self->detectDelay--;

    EntityPlayer *player = self->detectedPlayer;
    if (player) {
        int32 storeX = self->position.x;
        int32 storeY = self->position.y;

        if (self->triggerMode) {
            self->position.x = self->startPos.x + self->detectOffset.x;
            self->position.y = self->startPos.y + self->detectOffset.y;

            if (!Player_CheckCollisionTouch(player, self, &self->hitboxDetect))
                self->detectedPlayer = NULL;
        }
        else {
            if (!Player_CheckCollisionTouch(player, self, &Canista->hitboxRange))
                self->detectedPlayer = NULL;
        }

        if (self->detectedPlayer) {
            if (self->triggerMode) {
                int32 playerY = player->position.y & 0xFFFF0000;
                int32 shootY  = (storeY + self->shootOffset - self->shootSize) & 0xFFFF0000;

                if (playerY < shootY || playerY > shootY) {
                    self->mainAnimator.speed = 128;
                    self->velocity.y         = ((self->shootOffset + storeY) & 0xFFFF0000) < (player->position.y & 0xFFFF0000) ? 0x10000 : -0x10000;
                }
                else {
                    self->state          = Canista_State_Idle;
                    self->stopTimer      = 60;
                    self->detectedPlayer = NULL;
                    self->detectDelay    = 180;
                }
            }
            else {
                if (Player_CheckCollisionTouch(player, self, &Canista->hitboxDetect)) {
                    self->state          = Canista_State_Idle;
                    self->stopTimer      = 60;
                    self->detectedPlayer = NULL;
                    self->detectDelay    = 180;
                }
                else {
                    self->mainAnimator.speed = 128;
                    if (player->position.y - 0x400000 > storeY)
                        self->velocity.y = 0x10000;
                    else
                        self->velocity.y = -0x1000;
                }
            }
        }

        self->position.x = storeX;
        self->position.y = storeY;
    }
    else {
        self->mainAnimator.speed = 85;
        self->velocity.y         = self->moveDir ? -0x8000 : 0x8000;
    }

    self->position.y += self->velocity.y;
    int32 tapeRotation = self->velocity.y >> 12;
    if (self->direction)
        self->rotation += tapeRotation;
    else
        self->rotation -= tapeRotation;

    int32 offsetX = !(self->direction & FLIP_X) ? 0x100000 : -0x110000;
    int32 offsetY = ((self->velocity.y >> 31) & 0xFFD40000) + 0x140000;
    if (!RSDK.ObjectTileGrip(self, Zone->collisionLayers, (2 * ((self->direction & FLIP_X) != 0) + CMODE_LWALL), 0, offsetX, offsetY, 0)) {
        self->state     = Canista_State_Idle;
        self->stopTimer = 30;
        if (self->detectedPlayer) {
            self->detectedPlayer = NULL;
            self->detectDelay    = 180;
        }
    }

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (self->timer) {
        if (--self->timer == 1) {
            self->cannonAnimator.frameID = 3;
        }
    }
    else {
        self->cannonAnimator.timer   = (self->cannonAnimator.timer + (abs(self->velocity.y) >> 15)) % 7;
        self->cannonAnimator.frameID = (self->cannonAnimator.timer / 3) & 1;
    }

    Canista_CheckPlayerBadnikCollisions();
    Canista_CheckOffScreen();
}

void Canista_State_Idle(void)
{
    RSDK_THIS(Canista);

    if (!--self->stopTimer) {
        if (abs(self->velocity.y) == 0x8000)
            self->moveDir ^= 1;

        self->state = Canista_State_Moving;
        Canista_State_Moving();
    }
    else {
        if (self->stopTimer == 30) {
            int32 offsetX = !(self->direction & FLIP_X) ? 0x180000 : -0x180000;

            EntityCanista *shot = CREATE_ENTITY(Canista, INT_TO_VOID(true), offsetX + self->position.x, self->position.y);
            shot->direction     = self->direction;
            shot->velocity.x    = shot->direction == FLIP_X ? 0x40000 : -0x40000;

            shot             = CREATE_ENTITY(Canista, INT_TO_VOID(true), offsetX + self->position.x, self->position.y);
            shot->direction  = self->direction;
            shot->velocity.x = shot->direction == FLIP_X ? 0x4C000 : -0x4C000;

            shot             = CREATE_ENTITY(Canista, INT_TO_VOID(true), offsetX + self->position.x, self->position.y);
            shot->direction  = self->direction;
            shot->velocity.x = shot->direction == FLIP_X ? 0x58000 : -0x58000;

            shot             = CREATE_ENTITY(Canista, INT_TO_VOID(true), offsetX + self->position.x, self->position.y);
            shot->direction  = self->direction;
            shot->velocity.x = shot->direction == FLIP_X ? 0x64000 : -0x64000;

            self->cannonAnimator.frameID = 2;
            self->timer                  = 4;
            RSDK.PlaySfx(Canista->sfxPon, false, 0xFF);
        }

        Canista_CheckPlayerBadnikCollisions();
        Canista_CheckOffScreen();
    }

    if (self->timer) {
        if (--self->timer == 1) {
            self->cannonAnimator.frameID = 3;
        }
    }
    else {
        self->cannonAnimator.frameID = (self->cannonAnimator.timer / 3) & 1;
    }

    Canista_CheckOffScreen();
}

void Canista_CheckPlayerProjectileCollisions(void)
{
    RSDK_THIS(Canista);

    if (self->alpha >= 0x80) {

        foreach_active(Player, player)
        {
            if (Player_CheckCollisionTouch(player, self, &Canista->hitboxProjectile)) {
#if MANIA_USE_PLUS
                if (Player_CheckMightyShellHit(player, self, -0x400, -0x400)) {
                    self->stopTimer   = 0;
                    self->interaction = false;
                }
                else
#endif
                    Player_Hurt(player, self);
            }
        }
    }

    RSDK.ProcessAnimation(&self->mainAnimator);

    if (!RSDK.CheckOnScreen(self, &self->updateRange))
        destroyEntity(self);
}

void Canista_StateProjectile_Shot(void)
{
    RSDK_THIS(Canista);

    self->position.x += self->velocity.x;

    if (self->direction == FLIP_X) {
        self->velocity.x -= 0x2000;
        if (self->velocity.x < 0) {
            self->startPos   = self->position;
            self->velocity.x = 0;
            self->rotation   = 6;
            self->angle      = 2;

            RSDK.SetSpriteAnimation(Canista->aniFrames, 4, &self->mainAnimator, true, 0);
            self->state = Canista_StateProjectile_Fall;
        }
    }
    else {
        self->velocity.x += 0x2000;
        if (self->velocity.x > 0) {
            self->startPos   = self->position;
            self->velocity.x = 0;
            self->rotation   = 6;
            self->angle      = 2;

            RSDK.SetSpriteAnimation(Canista->aniFrames, 4, &self->mainAnimator, true, 0);
            self->state = Canista_StateProjectile_Fall;
        }
    }

    Canista_CheckPlayerProjectileCollisions();
}

void Canista_StateProjectile_Fall(void)
{
    RSDK_THIS(Canista);

    self->angle += self->rotation;
    if (!(Zone->timer & 0x1F) && RSDK.Rand(0, 2) == 1)
        self->rotation = -self->rotation;

    self->startPos.x += self->velocity.x;
    self->startPos.y += self->velocity.y;
    self->velocity.y += 0x800;

    if (self->velocity.y > 0x10000) {
        self->velocity.y -= 0x8000;
        if (self->velocity.y < 0x10000)
            self->velocity.y = 0x10000;
    }

    if (self->velocity.x) {
        if (self->velocity.x <= 0) {
            self->velocity.x += 0x2000;
            if (self->velocity.x > 0)
                self->velocity.x = 0;
        }
        else {
            self->velocity.x -= 0x2000;
            if (self->velocity.x < 0)
                self->velocity.x = 0;
        }
    }

    self->position.x = (RSDK.Cos256(self->angle) << 9) + self->startPos.x;
    self->position.y = (RSDK.Sin256(self->angle) << 9) + self->startPos.y;

    if (self->stopTimer) {
        self->stopTimer--;
    }
    else {
        self->alpha -= 4;
        if (self->alpha <= 0)
            destroyEntity(self);
    }

    Canista_CheckPlayerProjectileCollisions();
}

#if GAME_INCLUDE_EDITOR
void Canista_EditorDraw(void)
{
    RSDK_THIS(Canista);

    Canista_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        if (self->triggerMode) {
            self->hitboxDetect.left   = -self->detectSize.x >> 17;
            self->hitboxDetect.top    = -self->detectSize.y >> 17;
            self->hitboxDetect.right  = self->detectSize.x >> 17;
            self->hitboxDetect.bottom = self->detectSize.y >> 17;

            DrawHelpers_DrawHitboxOutline(self->position.x + self->detectOffset.x, self->position.y + self->detectOffset.y, &self->hitboxDetect,
                                          self->direction, 0xFF0000);
        }
        else {
            DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &Canista->hitboxDetect, self->direction, 0xFF0000);
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Canista_EditorLoad(void)
{
    if (RSDK.CheckSceneFolder("SPZ1"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ1/Canista.bin", SCOPE_STAGE);
    else if (RSDK.CheckSceneFolder("SPZ2"))
        Canista->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Canista.bin", SCOPE_STAGE);

    Canista->hitboxRange.left   = -128;
    Canista->hitboxRange.top    = -128;
    Canista->hitboxRange.right  = 128;
    Canista->hitboxRange.bottom = 128;

    Canista->hitboxDetect.left   = -132;
    Canista->hitboxDetect.top    = 48;
    Canista->hitboxDetect.right  = 132;
    Canista->hitboxDetect.bottom = 49;

    RSDK_ACTIVE_VAR(Canista, direction);
    RSDK_ENUM_VAR("Left", FLIP_NONE);
    RSDK_ENUM_VAR("Right", FLIP_X);

    RSDK_ACTIVE_VAR(Canista, triggerMode);
    RSDK_ENUM_VAR("Use Static Size", CANISTA_TRIGGER_STATIC);
    RSDK_ENUM_VAR("Use Detect Size", CANISTA_TRIGGER_DETECT);
}
#endif

void Canista_Serialize(void)
{
    RSDK_EDITABLE_VAR(Canista, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Canista, VAR_UINT8, triggerMode);
    RSDK_EDITABLE_VAR(Canista, VAR_VECTOR2, detectOffset);
    RSDK_EDITABLE_VAR(Canista, VAR_VECTOR2, detectSize);
    RSDK_EDITABLE_VAR(Canista, VAR_ENUM, shootOffset);
    RSDK_EDITABLE_VAR(Canista, VAR_ENUM, shootSize);
}
