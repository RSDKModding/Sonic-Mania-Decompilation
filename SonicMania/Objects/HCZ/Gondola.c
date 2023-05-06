// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Gondola Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGondola *Gondola;

void Gondola_Update(void)
{
    RSDK_THIS(Gondola);

    self->collisionOffset.x = -self->drawPos.x;
    self->collisionOffset.y = -self->drawPos.y;
    self->centerPos.x += self->velocity.x;
    self->centerPos.y += self->velocity.y;

    if (self->activePlayers) {
        if (self->stoodAngle < self->maxStoodAngle)
            self->stoodAngle += 4;
    }
    else {
        if (self->stoodAngle > 0)
            self->stoodAngle -= 4;
    }

    Gondola_HandleWaterFloating();
    Gondola_HandleTilting();
    Gondola_HandleMoveVelocity();

    if (self->onGround && self->velocity.y > 0)
        self->velocity.y = 0;

    self->drawPos.x = self->centerPos.x + (RSDK.Sin256(self->stoodAngle) << 10);
    self->drawPos.y = self->centerPos.y + (RSDK.Sin512(self->floatAngle * 2) * 0x180);

    if (!self->onGround)
        ++self->floatAngle;

    self->position.x = self->drawPos.x & 0xFFFF0000;
    self->position.y = self->drawPos.y & 0xFFFF0000;
    Gondola_HandleTileCollisions();

    self->drawGroup = self->onGround ? Zone->objectDrawGroup[0] : Zone->playerDrawGroup[0];
    self->collisionOffset.x += self->position.x;
    self->collisionOffset.y += self->position.y;
    Gondola_HandlePlayerInteractions();
}

void Gondola_LateUpdate(void) {}

void Gondola_StaticUpdate(void) {}

void Gondola_Draw(void)
{
    RSDK_THIS(Gondola);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void Gondola_Create(void *data)
{
    RSDK_THIS(Gondola);

    self->active    = ACTIVE_NORMAL;
    self->drawGroup = Zone->playerDrawGroup[1];

    self->startPos  = self->position;
    self->drawPos   = self->position;
    self->centerPos = self->position;

    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawGroup     = Zone->playerDrawGroup[0];

    self->hitbox.left   = -76;
    self->hitbox.top    = -12;
    self->hitbox.right  = 76;
    self->hitbox.bottom = 4;

    RSDK.SetSpriteAnimation(Gondola->aniFrames, 0, &self->animator, true, 0);
}

void Gondola_StageLoad(void) { Gondola->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Gondola.bin", SCOPE_STAGE); }

int32 Gondola_GetWaterLevel(void)
{
    RSDK_THIS(Gondola);

    if (self->position.y < Water->waterLevel) {
        foreach_active(Water, water)
        {
            if (water->type == WATER_POOL && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Water->hitboxPoint)) {
                return water->position.y + (water->hitbox.top << 16);
            }
        }
    }

    return Water->waterLevel;
}

void Gondola_HandleWaterFloating(void)
{
    RSDK_THIS(Gondola);

    int32 waterLevel = Gondola_GetWaterLevel();

    self->waterDistance = self->centerPos.y - waterLevel;
    self->waterInRange  = abs(self->centerPos.y - waterLevel) < 0x40000;

    if (abs(self->centerPos.y - waterLevel) >= 0x40000) {
        if (self->centerPos.y - waterLevel < -0x80000) {
            self->velocity.y += 0x3800;
        }
        else {
            if (self->centerPos.y <= waterLevel)
                self->velocity.y = 0;
            else
                self->velocity.y = -Water->waterMoveSpeed;
        }
    }
    else {
        if (Water->targetWaterLevel > Water->waterLevel) {
            self->velocity.y = Water->waterMoveSpeed;
        }
        else {
            if (self->centerPos.y <= waterLevel)
                self->velocity.y = 0;
            else
                self->velocity.y = -Water->waterMoveSpeed;
        }
    }

    if (self->minY) {
        if (self->centerPos.y <= self->minY << 16) {
            self->centerPos.y = self->minY << 16;

            if (self->velocity.y < 0)
                self->velocity.y = 0;
        }
    }

    if (self->maxY) {
        if (self->centerPos.y >= self->maxY << 16) {
            self->centerPos.y = self->maxY << 16;

            if (self->velocity.y > 0)
                self->velocity.y = 0;
        }
    }
}

void Gondola_HandleTilting(void)
{
    RSDK_THIS(Gondola);

    int32 targetRotation = 0;

    if (!self->onGround) {
        foreach_active(Player, player)
        {
            if (((1 << RSDK.GetEntitySlot(player)) & self->activePlayers))
                targetRotation += (player->position.x - self->centerPos.x) >> 21;
        }
    }

    if (self->rotation < targetRotation)
        self->rotation++;
    else if (self->rotation > targetRotation)
        self->rotation--;
}

void Gondola_HandleMoveVelocity(void)
{
    RSDK_THIS(Gondola);

    if (!self->onGround) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (!player->sidekick) {
                if (((1 << playerID) & self->activePlayers)) {
                    self->velocity.x = (player->position.x - self->centerPos.x) >> 5;
                }
                else {
                    if (self->velocity.x > 0) {
                        self->velocity.x -= 0x200;

                        if (self->velocity.x < 0)
                            self->velocity.x = 0;
                    }
                    else if (self->velocity.x < 0) {
                        self->velocity.x += 0x200;

                        if (self->velocity.x > 0)
                            self->velocity.x = 0;
                    }
                }
            }
        }
    }
}

void Gondola_HandleTileCollisions(void)
{
    RSDK_THIS(Gondola);

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_FLOOR, 0, 0, self->hitbox.bottom << 16, true)) {
        self->maxStoodAngle = 0;

        if (self->velocity.y > 0)
            self->velocity.y = 0;

        self->onGround = true;
    }
    else {
        self->maxStoodAngle = 64;
        self->onGround      = false;
    }

    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_ROOF, 0, 0, self->hitbox.top << 16, true) && self->velocity.y < 0) {
        self->velocity.y = 0;
    }

    // RWall = Right side of the wall, so this would collide with the *left* side of the boat
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_RWALL, 0, (self->hitbox.left - 8) << 16, 0, true)) {
        if (self->velocity.x < 0)
            self->velocity.x = 0;

        self->stoppedL = true;
    }
    else {
        self->stoppedL = false;
    }

    // LWall = Left side of the wall, so this would collide with the *right* side of the boat
    if (RSDK.ObjectTileCollision(self, Zone->collisionLayers, CMODE_LWALL, 0, (self->hitbox.right + 8) << 16, 0, true)) {
        if (self->velocity.x > 0)
            self->velocity.x = 0;

        self->stoppedR = true;
    }
    else {
        self->stoppedR = false;
    }

    self->drawPos.x = self->position.x;
    self->drawPos.y = self->position.y;
}

void Gondola_HandlePlayerInteractions(void)
{
    RSDK_THIS(Gondola);

    if (self->onGround) {
        self->activePlayers = 0;
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntitySlot(player);

            if (((1 << playerID) & self->activePlayers)) {
                player->position.x += self->collisionOffset.x;
                player->position.y += self->collisionOffset.y + 0x10000;

                if (!player->sidekick && !Gondola->hasAchievement) {
                    if (!Gondola->taggedBoatIDs[self->boatID]) {
                        Gondola->taggedBoatIDs[self->boatID] = true;

                        if (Gondola->taggedBoatIDs[0] && Gondola->taggedBoatIDs[1] && Gondola->taggedBoatIDs[2]) {
                            API_UnlockAchievement(&achievementList[ACH_HCZ]);
                            Gondola->hasAchievement = true;
                        }
                    }
                }
            }

            if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
                self->activePlayers |= 1 << playerID;
            else
                self->activePlayers &= ~(1 << playerID);
        }
    }
}

#if GAME_INCLUDE_EDITOR
void Gondola_EditorDraw(void) { Gondola_Draw(); }

void Gondola_EditorLoad(void)
{
    Gondola->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Gondola.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(Gondola, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);

    RSDK_ACTIVE_VAR(Gondola, boatID);
    RSDK_ENUM_VAR("Boat 1", 0);
    RSDK_ENUM_VAR("Boat 2", 1);
    RSDK_ENUM_VAR("Boat 3", 2);
}
#endif

void Gondola_Serialize(void)
{
    RSDK_EDITABLE_VAR(Gondola, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Gondola, VAR_ENUM, minY);
    RSDK_EDITABLE_VAR(Gondola, VAR_ENUM, maxY);
    RSDK_EDITABLE_VAR(Gondola, VAR_UINT8, boatID);
}
