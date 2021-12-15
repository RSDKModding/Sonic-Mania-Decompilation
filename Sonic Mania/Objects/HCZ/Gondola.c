// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Gondola Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectGondola *Gondola;

void Gondola_Update(void)
{
    RSDK_THIS(Gondola);
    self->field_94.x = -self->field_8C.x;
    self->field_94.y = -self->field_8C.y;
    self->field_84.x += self->velocity.x;
    self->field_84.y += self->velocity.y;

    if (self->activePlayers) {
        if (self->field_9C < self->field_A0) {
            self->field_9C += 4;
        }
    }
    else if (self->field_9C > 0) {
        self->field_9C -= 4;
    }

    Gondola_Unknown2();
    Gondola_Unknown3();
    Gondola_Unknown4();
    if (self->onGround && self->velocity.y > 0)
        self->velocity.y = 0;

    self->field_8C.x = self->field_84.x;
    self->field_8C.y = self->field_84.y;
    self->field_8C.y += RSDK.Sin256(self->field_9C) << 10;
    self->field_8C.y += 384 * RSDK.Sin512(2 * self->field_A4);
    if (!self->onGround)
        ++self->field_A4;
    self->position.x = self->field_8C.x;
    self->position.y = self->field_8C.y;
    self->position.x &= 0xFFFF0000;
    self->position.y &= 0xFFFF0000;
    Gondola_Unknown5();
    if (self->onGround)
        self->drawOrder = Zone->drawOrderLow;
    else
        self->drawOrder = Zone->playerDrawLow;
    self->field_94.x += self->position.x;
    self->field_94.y += self->position.y;
    Gondola_Unknown6();
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
    self->active        = ACTIVE_NORMAL;
    self->drawOrder     = Zone->playerDrawHigh;
    self->field_68.x    = self->position.x;
    self->field_68.y    = self->position.y;
    self->field_8C.x    = self->position.x;
    self->field_8C.y    = self->position.y;
    self->field_84.x    = self->position.x;
    self->field_84.y    = self->position.y;
    self->visible       = true;
    self->drawFX        = FX_ROTATE | FX_FLIP;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
    self->drawOrder     = Zone->playerDrawLow;

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
            if (water->type == WATER_RECT && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, self, &Water->hitbox)) {
                return water->position.y + (water->hitbox.top << 16);
            }
        }
    }
    return Water->waterLevel;
}

void Gondola_Unknown2(void)
{
    RSDK_THIS(Gondola);
    int32 waterLevel = Gondola_GetWaterLevel();

    self->field_80 = self->field_84.y - waterLevel;
    self->field_74 = abs(self->field_84.y - waterLevel) < 0x40000;
    if (abs(self->field_84.y - waterLevel) >= 0x40000) {
        if (self->field_84.y - waterLevel < -0x80000) {
            self->velocity.y += 0x3800;
        }
        else {
            if (self->field_84.y <= waterLevel)
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
            if (self->field_84.y <= waterLevel)
                self->velocity.y = 0;
            else
                self->velocity.y = -Water->waterMoveSpeed;
        }
    }

    if (self->minY) {
        if (self->field_84.y <= self->minY << 16) {
            self->field_84.y = self->minY << 16;
            if (self->velocity.y < 0)
                self->velocity.y = 0;
        }
    }

    if (self->maxY) {
        if (self->field_84.y >= self->maxY << 16) {
            self->field_84.y = self->maxY << 16;
            if (self->velocity.y > 0)
                self->velocity.y = 0;
        }
    }
}

void Gondola_Unknown3(void)
{
    RSDK_THIS(Gondola);

    int32 desiredRotation = 0;
    if (!self->onGround) {
        foreach_active(Player, player)
        {
            if (((1 << RSDK.GetEntityID(player)) & self->activePlayers))
                desiredRotation += (player->position.x - self->field_84.x) >> 21;
        }
    }

    if (self->rotation < desiredRotation) {
        self->rotation++;
    }
    else if (self->rotation > desiredRotation) {
        self->rotation--;
    }
}

void Gondola_Unknown4(void)
{
    RSDK_THIS(Gondola);
    if (!self->onGround) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);
            if (!player->sidekick) {
                if (((1 << playerID) & self->activePlayers)) {
                    self->velocity.x = (player->position.x - self->field_84.x) >> 5;
                }
                else {
                    if (self->velocity.x > 0) {
                        self->velocity.x -= 512;
                        if (self->velocity.x - 512 < 0)
                            self->velocity.x = 0;
                    }
                    else if (self->velocity.x < 0) {
                        self->velocity.x += 512;
                        if (self->velocity.x > 0)
                            self->velocity.x = 0;
                    }
                }
            }
        }
    }
}

void Gondola_Unknown5(void)
{
    RSDK_THIS(Gondola);
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_FLOOR, 0, 0, self->hitbox.bottom << 16, true)) {
        self->field_A0 = 0;
        if (self->velocity.y > 0)
            self->velocity.y = 0;
        self->onGround = true;
    }
    else {
        self->field_A0 = 64;
        self->onGround = false;
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_ROOF, 0, 0, self->hitbox.top << 16, true) && self->velocity.y < 0) {
        self->velocity.y = 0;
    }
    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_RWALL, 0, (self->hitbox.left - 8) << 16, 0, true)) {
        if (self->velocity.x < 0)
            self->velocity.x = 0;
        self->flag = true;
    }
    else {
        self->flag = false;
    }

    if (RSDK.ObjectTileCollision(self, Zone->fgLayers, CMODE_LWALL, 0, (self->hitbox.right + 8) << 16, 0, true)) {
        if (self->velocity.x > 0)
            self->velocity.x = 0;
        self->field_8C.x = self->position.x;
        self->field_8C.y = self->position.y;
        self->flag2      = true;
    }
    else {
        self->field_8C.x = self->position.x;
        self->field_8C.y = self->position.y;
        self->flag2      = false;
    }
}

void Gondola_Unknown6(void)
{
    RSDK_THIS(Gondola);
    if (self->onGround) {
        self->activePlayers = 0;
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);
            if (((1 << playerID) & self->activePlayers)) {
                player->position.x += self->field_94.x;
                player->position.y += self->field_94.y;
                player->position.y += 0x10000;
                if (!player->sidekick && !Gondola->hasAchievement) {
                    if (!Gondola->taggedBoatIDs[self->boatID]) {
                        Gondola->taggedBoatIDs[self->boatID] = 1;
                        if (Gondola->taggedBoatIDs[0] && Gondola->taggedBoatIDs[1] && Gondola->taggedBoatIDs[2]) {
                            API_UnlockAchievement("ACH_HCZ");
                            Gondola->hasAchievement = true;
                        }
                    }
                }
            }

            if (Player_CheckCollisionPlatform(player, self, &self->hitbox))
                self->activePlayers |= (1 << playerID);
            else
                self->activePlayers &= ~(1 << playerID);
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void Gondola_EditorDraw(void) { Gondola_Draw(); }

void Gondola_EditorLoad(void) { Gondola->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Gondola.bin", SCOPE_STAGE); }
#endif

void Gondola_Serialize(void)
{
    RSDK_EDITABLE_VAR(Gondola, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Gondola, VAR_ENUM, minY);
    RSDK_EDITABLE_VAR(Gondola, VAR_ENUM, maxY);
    RSDK_EDITABLE_VAR(Gondola, VAR_UINT8, boatID);
}
