// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Bridge Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectBridge *Bridge;

void Bridge_Update(void)
{
    RSDK_THIS(Bridge);

    if (self->activePlayerCount) {
        if (self->timer < 0x80)
            self->timer += 8;
    }
    else {
        if (self->timer) {
            self->playerPtr = (Entity *)-1;
            self->timer -= 8;
        }
        else {
            self->depression = 0;
        }
    }

    self->activePlayerCount = 0;
    self->bridgeDepth          = (self->depression * self->timer) >> 7;

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->state == Player_State_KnuxLedgePullUp)
            continue;

        if (player->position.x > self->startPos && player->position.x < self->endPos) {
            if (player != (EntityPlayer *)self->playerPtr) {
                if (!self->activePlayerCount)
                    self->stoodPos = player->position.x - self->startPos;

                if (player->velocity.y >= 0) {
                    Hitbox bridgeHitbox;
                    bridgeHitbox.left  = -0x400;
                    bridgeHitbox.right = 0x400;

                    int32 divisor = 0;
                    int32 ang     = 0;
                    if (player->position.x - self->startPos <= self->stoodPos) {
                        divisor = self->stoodPos;
                        ang     = (player->position.x - self->startPos) << 7;
                    }
                    else {
                        divisor = self->endPos - self->startPos - self->stoodPos;
                        ang     = (self->endPos - player->position.x) << 7;
                    }

                    int32 hitY = (self->bridgeDepth * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                    if (player->velocity.y >= 0x8000) {
                        bridgeHitbox.top    = (hitY >> 16);
                        bridgeHitbox.bottom = bridgeHitbox.top + 8;
                    }
                    else {
                        bridgeHitbox.bottom = (hitY >> 16);
                        bridgeHitbox.top    = (hitY >> 16) - 8;
                    }

                    if (Player_CheckCollisionTouch(player, self, &bridgeHitbox)) {
                        ++self->activePlayerCount;
                        player->position.y = hitY + self->position.y - (playerHitbox->bottom << 16);
                        if (!player->onGround) {
                            player->onGround  = true;
                            player->groundVel = player->velocity.x;
                        }
                        player->flailing = false;

                        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
                        if (player == player1) {
                            if (self->playerPtr != (Entity *)-1 && self->playerPtr != (Entity *)-2) {
                                int32 distance     = self->endPos - self->startPos;
                                self->stoodPos = player->position.x - self->startPos;
                                self->depression = (distance >> 13) * RSDK.Sin512((self->stoodPos >> 8) / (distance >> 16));
                                self->bridgeDepth = (self->depression * self->timer) >> 7;
                            }
                            self->playerPtr = (Entity *)player;
                            if (player->velocity.y < 0x10000)
                                self->timer = 0x80;
                        }
                        else {
                            if (self->playerPtr == (Entity *)-1) {
                                self->playerPtr = (Entity *)player;
                                if (player->velocity.y < 0x10000)
                                    self->timer = 0x80;
                            }
                            if (self->playerPtr == (Entity *)-2)
                                self->playerPtr = (Entity *)player;
                        }

                        if (!player->onGround) {
                            player->onGround  = true;
                            player->groundVel = player->velocity.x;
                        }
                        player->velocity.y = 0;
                        if (player->shield == SHIELD_FIRE && self->burnable)
                            Bridge_Burn((player->position.x - self->startPos) >> 20);
                    }
                }
            }
            else {
                self->stoodPos = player->position.x - self->startPos;
                int32 distance     = (self->endPos - self->startPos);
                self->depression = RSDK.Sin512((self->stoodPos >> 8) / (distance >> 16)) * (distance >> 13);

                if (player->position.y > self->position.y - 0x300000) {
                    if (player->velocity.y >= 0) {
                        ++self->activePlayerCount;
                        player->position.y = self->bridgeDepth + self->position.y - ((playerHitbox->bottom + 8) << 16);
                        if (!player->onGround) {
                            player->onGround  = true;
                            player->groundVel = player->velocity.x;
                        }
                        player->velocity.y = 0;
                        player->flailing   = false;
                        if (player->shield == SHIELD_FIRE && self->burnable)
                            Bridge_Burn((player->position.x - self->startPos) >> 20);
                    }
                    else {
                        self->playerPtr = (Entity *)-2;
                    }
                }
            }
        }
        else if (player == (EntityPlayer *)self->playerPtr) {
            self->timer     = 32;
            self->playerPtr = (Entity *)-2;
        }
    }

    if (self->burnOffset != 0xFF)
        Bridge_Burn(self->burnOffset);
}

void Bridge_LateUpdate(void) {}

void Bridge_StaticUpdate(void) {}

void Bridge_Draw(void)
{
    RSDK_THIS(Bridge);
    int32 id = 0;
    Vector2 drawPos;

    int32 size  = self->stoodPos >> 20;
    int32 ang   = 0x80000;
    drawPos.x = self->startPos + 0x80000;
    for (int32 i = 0; i < size; ++i) {
        drawPos.y = (self->bridgeDepth * RSDK.Sin512((ang << 7) / self->stoodPos) >> 9) + self->position.y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += 0x100000;
        ang += 0x100000;
    }
    id = size;

    drawPos.y = self->bridgeDepth + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, 0);
    drawPos.x += 0x100000;
    ++id;

    ang         = 0x80000;
    int32 divisor = self->endPos - self->startPos - self->stoodPos;
    drawPos.x   = self->endPos - 0x80000;
    for (; id < self->length; ++id) {
        drawPos.y = (self->bridgeDepth * RSDK.Sin512((ang << 7) / divisor) >> 9) + self->position.y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x -= 0x100000;
        ang += 0x100000;
    }
}

void Bridge_Create(void *data)
{
    RSDK_THIS(Bridge);
    self->visible = true;
    if (!SceneInfo->inEditor)
        ++self->length;
    self->drawOrder     = Zone->drawOrderLow;
    self->active        = ACTIVE_BOUNDS;
    int32 len               = self->length << 19;
    self->startPos      = self->position.x - len;
    self->endPos        = len + self->position.x;
    self->updateRange.x = len;
    self->updateRange.y = 0x800000;
    self->playerPtr     = (Entity *)-1;
    self->burnOffset      = 0xFF;
    RSDK.SetSpriteAnimation(Bridge->aniFrames, 0, &self->animator, true, 0);
}

void Bridge_StageLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckStageFolder("HCZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckStageFolder("LRZ1"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Bridge.bin", SCOPE_STAGE);
}

void Bridge_Burn(int32 offset)
{
    RSDK_THIS(Bridge);

    int32 size   = self->stoodPos >> 20;
    int32 spawnX = self->startPos + 0x80000;
    int32 off    = -offset;
    int32 ang    = 0x80000;
    for (int32 i = 0; i < size; ++i) {
        int32 sine = RSDK.Sin512((ang << 7) / self->stoodPos);
        CREATE_ENTITY(BurningLog, intToVoid(8 * abs(off++) + 16), spawnX, (self->bridgeDepth * sine >> 9) + self->position.y);
        ang += 0x100000;
        spawnX += 0x100000;
    }

    int32 id = size;
    CREATE_ENTITY(BurningLog, intToVoid(8 * abs(id++ - offset) + 16), spawnX, self->bridgeDepth + self->position.y);

    spawnX      = self->endPos - 0x80000;
    int32 divisor = self->endPos - self->startPos - self->stoodPos;
    ang         = 0x80000;
    if (id >= self->length) {
        destroyEntity(self);
    }
    else {
        off = offset - id;
        for (; id < self->length; ++id, --off) {
            int32 spawnY = (self->bridgeDepth * RSDK.Sin512((ang << 7) / divisor) >> 9) + self->position.y;
            CREATE_ENTITY(BurningLog, intToVoid(8 * abs(self->length - abs(off) - offset) + 16), spawnX, spawnY);
            ang += 0x100000;
            spawnX -= 0x100000;
        }
        destroyEntity(self);
    }
}

#if RETRO_INCLUDE_EDITOR
void Bridge_EditorDraw(void)
{
    RSDK_THIS(Bridge);
    int32 length        = self->length++;

    int32 len           = self->length << 19;
    self->startPos      = self->position.x - len;
    self->endPos        = len + self->position.x;
    self->updateRange.x = len;
    self->updateRange.y = 0x800000;

    Bridge_Draw();

    self->length = length;
}

void Bridge_EditorLoad(void)
{
    if (RSDK.CheckStageFolder("GHZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckStageFolder("HCZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckStageFolder("LRZ1"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Bridge.bin", SCOPE_STAGE);
}
#endif

void Bridge_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bridge, VAR_UINT8, length);
    RSDK_EDITABLE_VAR(Bridge, VAR_BOOL, burnable);
}
