// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Bridge Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectBridge *Bridge;

void Bridge_Update(void)
{
    RSDK_THIS(Bridge);

    if (self->stoodEntityCount) {
        if (self->timer < 0x80)
            self->timer += 8;
    }
    else {
        if (self->timer) {
            self->stoodEntity = (Entity *)-1;
            self->timer -= 8;
        }
        else {
            self->depression = 0;
        }
    }

    self->stoodEntityCount = 0;
    self->bridgeDepth      = (self->depression * self->timer) >> 7;

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->state == Player_State_KnuxLedgePullUp)
            continue;

        Bridge_HandleCollisions(player, self, playerHitbox, true, true);
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

    int32 size = self->stoodPos >> 20;
    int32 ang  = 0x80000;
    drawPos.x  = self->startPos + 0x80000;
    for (int32 i = 0; i < size; ++i) {
        drawPos.y = (self->bridgeDepth * RSDK.Sin512((ang << 7) / self->stoodPos) >> 9) + self->position.y;
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += 0x100000;
        ang += 0x100000;
    }
    id = size;

    drawPos.y = self->bridgeDepth + self->position.y;
    RSDK.DrawSprite(&self->animator, &drawPos, false);
    drawPos.x += 0x100000;
    ++id;

    ang           = 0x80000;
    int32 divisor = self->endPos - self->startPos - self->stoodPos;
    drawPos.x     = self->endPos - 0x80000;
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
    ++self->length;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->active        = ACTIVE_BOUNDS;
    int32 len           = self->length << 19;
    self->startPos      = self->position.x - len;
    self->endPos        = len + self->position.x;
    self->updateRange.x = len;
    self->updateRange.y = 0x800000;
    self->stoodEntity   = (Entity *)-1;
    self->burnOffset    = 0xFF;
    RSDK.SetSpriteAnimation(Bridge->aniFrames, 0, &self->animator, true, 0);
}

void Bridge_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("GHZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("HCZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("LRZ1"))
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
        CREATE_ENTITY(BurningLog, INT_TO_VOID(8 * abs(off++) + 16), spawnX, (self->bridgeDepth * sine >> 9) + self->position.y);
        ang += 0x100000;
        spawnX += 0x100000;
    }

    int32 id = size;
    CREATE_ENTITY(BurningLog, INT_TO_VOID(8 * abs(id++ - offset) + 16), spawnX, self->bridgeDepth + self->position.y);

    spawnX        = self->endPos - 0x80000;
    int32 divisor = self->endPos - self->startPos - self->stoodPos;
    ang           = 0x80000;
    if (id < self->length) {
        off = offset - id;
        for (; id < self->length; ++id, --off) {
            int32 spawnY = (self->bridgeDepth * RSDK.Sin512((ang << 7) / divisor) >> 9) + self->position.y;
            CREATE_ENTITY(BurningLog, INT_TO_VOID(8 * abs(self->length - abs(off) - offset) + 16), spawnX, spawnY);
            ang += 0x100000;
            spawnX -= 0x100000;
        }
    }
    destroyEntity(self);
}

bool32 Bridge_HandleCollisions(void *e, EntityBridge *self, Hitbox *entityHitbox, bool32 updateVars, bool32 isPlayer)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    // use EntityPlayer as the type so we can access player variables if needed
    // if `isPlayer` is false, then only base entity variables will be accessed
    EntityPlayer *entity = (EntityPlayer *)e;

    bool32 bridgeCollided = false;

    if (entity->position.x > self->startPos && entity->position.x < self->endPos) {
        if (entity != self->stoodEntity) {
            if (updateVars && !self->stoodEntityCount)
                self->stoodPos = entity->position.x - self->startPos;

            if (entity->velocity.y >= 0) {
                Hitbox hitboxBridge;
                hitboxBridge.left  = -0x400;
                hitboxBridge.right = 0x400;

                int32 divisor = 0;
                int32 ang     = 0;
                if (entity->position.x - self->startPos <= self->stoodPos) {
                    divisor = self->stoodPos;
                    ang     = (entity->position.x - self->startPos) << 7;
                }
                else {
                    divisor = self->endPos - self->startPos - self->stoodPos;
                    ang     = (self->endPos - entity->position.x) << 7;
                }

                int32 hitY = (self->bridgeDepth * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                if (entity->velocity.y >= 0x8000) {
                    hitboxBridge.top    = (hitY >> 16);
                    hitboxBridge.bottom = hitboxBridge.top + 8;
                }
                else {
                    hitboxBridge.bottom = (hitY >> 16);
                    hitboxBridge.top    = hitboxBridge.bottom - 8;
                }

                bool32 collided = false;
                if (isPlayer)
                    collided = Player_CheckCollisionTouch(entity, self, &hitboxBridge);
                else
                    collided = RSDK.CheckObjectCollisionTouchBox(self, &hitboxBridge, entity, entityHitbox);

                if (collided) {
                    entity->position.y = hitY + self->position.y - (entityHitbox->bottom << 16);

                    if (updateVars) {
                        ++self->stoodEntityCount;
                        if (!entity->onGround) {
                            entity->onGround  = true;
                            entity->groundVel = entity->velocity.x;
                        }

                        if (isPlayer)
                            entity->flailing = false;

                        if (entity == player1) {
                            if (self->stoodEntity != (void *)-1 && self->stoodEntity != (void *)-2) {
                                int32 distance    = self->endPos - self->startPos;
                                self->stoodPos    = entity->position.x - self->startPos;
                                self->depression  = (distance >> 13) * RSDK.Sin512((self->stoodPos >> 8) / (distance >> 16));
                                self->bridgeDepth = (self->depression * self->timer) >> 7;
                            }

                            self->stoodEntity = entity;
                            if (entity->velocity.y < 0x10000)
                                self->timer = 0x80;
                        }
                        else {
                            if (self->stoodEntity == (void *)-1) {
                                self->stoodEntity = entity;
                                if (entity->velocity.y < 0x10000)
                                    self->timer = 0x80;
                            }

                            if (self->stoodEntity == (void *)-2)
                                self->stoodEntity = entity;
                        }

                        if (!entity->onGround) {
                            entity->onGround  = true;
                            entity->groundVel = entity->velocity.x;
                        }

                        entity->velocity.y = 0;

                        if (isPlayer) {
                            if (entity->shield == SHIELD_FIRE && self->burnable)
                                Bridge_Burn((entity->position.x - self->startPos) >> 20);
                        }
                    }

                    bridgeCollided = true;
                }
            }
        }
        else if (updateVars) {
            self->stoodPos   = entity->position.x - self->startPos;
            int32 distance   = (self->endPos - self->startPos);
            self->depression = RSDK.Sin512((self->stoodPos >> 8) / (distance >> 16)) * (distance >> 13);

            if (entity->position.y > self->position.y - 0x300000) {
                if (entity->velocity.y >= 0) {
                    ++self->stoodEntityCount;
                    entity->position.y = self->position.y + self->bridgeDepth - ((entityHitbox->bottom + 8) << 16);

                    if (!entity->onGround) {
                        entity->onGround  = true;
                        entity->groundVel = entity->velocity.x;
                    }

                    entity->velocity.y = 0;
                    if (isPlayer) {
                        entity->flailing = false;
                        if (entity->shield == SHIELD_FIRE && self->burnable)
                            Bridge_Burn((entity->position.x - self->startPos) >> 20);
                    }

                    bridgeCollided = true;
                }
                else {
                    self->stoodEntity = (void *)-2;
                }
            }
        }
    }
    else if (entity == self->stoodEntity) {
        self->timer       = 32;
        self->stoodEntity = (void *)-2;
    }

    return bridgeCollided;
}

#if GAME_INCLUDE_EDITOR
void Bridge_EditorDraw(void)
{
    RSDK_THIS(Bridge);
    int32 length = self->length++;

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
    if (RSDK.CheckSceneFolder("GHZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("GHZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("HCZ"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Bridge.bin", SCOPE_STAGE);
    if (RSDK.CheckSceneFolder("LRZ1"))
        Bridge->aniFrames = RSDK.LoadSpriteAnimation("LRZ1/Bridge.bin", SCOPE_STAGE);
}
#endif

void Bridge_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bridge, VAR_UINT8, length);
    RSDK_EDITABLE_VAR(Bridge, VAR_BOOL, burnable);
}
