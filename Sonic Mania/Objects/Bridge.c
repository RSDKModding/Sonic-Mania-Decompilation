#include "../SonicMania.h"

ObjectBridge *Bridge;

void Bridge_Update(void)
{
    RSDK_THIS(Bridge);

    if (entity->activePlayerCount) {
        if (entity->timer < 0x80)
            entity->timer += 8;
    }
    else if (entity->timer) {
        entity->playerPtr = (Entity *)-1;
        entity->timer -= 8;
    }
    else {
        entity->field_68 = 0;
    }

    entity->activePlayerCount = 0;
    entity->field_6C          = (entity->field_68 * entity->timer) >> 7;

    foreach_active(Player, player)
    {
        Hitbox *playerHitbox = Player_GetHitbox(player);
        if (player->state == Player_State_KnuxLedgePullUp)
            continue;
        int startPos = entity->startPos;
        int endPos   = entity->endPos;
        if (player->position.x <= entity->startPos || player->position.x >= endPos) {
            if (player == (EntityPlayer *)entity->playerPtr) {
                entity->timer     = 32;
                entity->playerPtr = (Entity *)-2;
            }
        }
        else {
            if (player != (EntityPlayer *)entity->playerPtr) {
                if (!entity->activePlayerCount)
                    entity->stoodPos = player->position.x - entity->startPos;
                if (player->velocity.y >= 0) {
                    Hitbox bridgeHitbox;
                    bridgeHitbox.left  = -0x400;
                    bridgeHitbox.right = 0x400;
                    int divisor        = entity->stoodPos;
                    int ang            = 0;
                    if (player->position.x - entity->startPos <= divisor) {
                        ang = (player->position.x - entity->startPos) << 7;
                    }
                    else {
                        divisor = endPos - entity->stoodPos - entity->startPos;
                        ang     = (endPos - player->position.x) << 7;
                    }

                    int hitY = (entity->field_6C * RSDK.Sin512(ang / divisor) >> 9) - 0x80000;
                    if (player->velocity.y >= 0x8000) {
                        bridgeHitbox.top    = (hitY >> 16);
                        bridgeHitbox.bottom = bridgeHitbox.top + 8;
                    }
                    else {
                        bridgeHitbox.bottom = (hitY >> 16);
                        bridgeHitbox.top    = (hitY >> 16) - 8;
                    }
                    if (Player_CheckCollisionTouch(player, entity, &bridgeHitbox)) {
                        ++entity->activePlayerCount;
                        player->position.y = hitY + entity->position.y - (playerHitbox->bottom << 16);
                        if (!player->onGround) {
                            player->onGround  = true;
                            player->groundVel = player->velocity.x;
                        }
                        player->flailing = false;

                        EntityPlayer *player1 = (EntityPlayer *)RSDK.GetEntityByID(SLOT_PLAYER1);
                        if (player == player1) {
                            if (entity->playerPtr != (Entity *)-1 && entity->playerPtr != (Entity *)-2) {
                                entity->stoodPos = player->position.x - entity->startPos;
                                int val          = ((entity->endPos - entity->startPos) >> 13)
                                          * RSDK.Sin512((entity->stoodPos >> 8) / ((entity->endPos - entity->startPos) >> 16));
                                entity->field_68 = val;
                                entity->field_6C = (val * entity->timer) >> 7;
                            }
                            entity->playerPtr = (Entity *)player;
                            if (player->velocity.y < 0x10000)
                                entity->timer = -0x80;
                        }
                        else {
                            if (entity->playerPtr == (Entity *)-1) {
                                entity->playerPtr = (Entity *)player;
                                if (player->velocity.y < 0x10000)
                                    entity->timer = -0x80;
                            }
                            if (entity->playerPtr == (Entity *)-2)
                                entity->playerPtr = (Entity *)player;
                        }
                        if (!player->onGround) {
                            player->onGround  = true;
                            player->groundVel = player->velocity.x;
                        }
                        player->velocity.y = 0;
                        if (player->shield == SHIELD_FIRE && entity->burnable == 1)
                            Bridge_Burn((player->position.x - entity->startPos) >> 20);
                    }
                }
            }
            else {
                entity->stoodPos = player->position.x - entity->startPos;
                entity->field_68 =
                    RSDK.Sin512((entity->stoodPos >> 8) / ((endPos - entity->startPos) >> 16)) * ((entity->endPos - entity->startPos) >> 13);

                if (player->position.y > entity->position.y - 0x300000) {
                    if (player->velocity.y >= 0) {
                        ++entity->activePlayerCount;
                        player->position.y = entity->field_6C + entity->position.y - ((playerHitbox->bottom + 8) << 16);
                        if (player->onGround == false) {
                            player->onGround  = 1;
                            player->groundVel = player->velocity.x;
                        }
                        player->velocity.y = 0;
                        player->flailing   = 0;
                        if (player->shield == SHIELD_FIRE && entity->burnable)
                            Bridge_Burn((player->position.x - entity->startPos) >> 20);
                    }
                    else {
                        entity->playerPtr = (Entity *)-2;
                    }
                }
            }
        }
    }

    if (entity->field_60 != 0xFF)
        Bridge_Burn(entity->field_60);
}

void Bridge_LateUpdate(void) {}

void Bridge_StaticUpdate(void) {}

void Bridge_Draw(void)
{
    RSDK_THIS(Bridge);
    int id = 0;
    Vector2 drawPos;

    int size  = entity->stoodPos >> 20;
    int ang   = 0x80000;
    drawPos.x = entity->startPos + 0x80000;
    for (int i = 0; i < size; ++i) {
        drawPos.y = (entity->field_6C * RSDK.Sin512((ang << 7) / entity->stoodPos) >> 9) + entity->position.y;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += 0x100000;
        ang += 0x100000;
    }
    id = size;

    drawPos.y = entity->field_6C + entity->position.y;
    RSDK.DrawSprite(&entity->animator, &drawPos, 0);
    drawPos.x += 0x100000;
    ++id;

    ang         = 0x80000;
    int divisor = entity->endPos - entity->startPos - entity->stoodPos;
    drawPos.x   = entity->endPos - 0x80000;
    for (; id < entity->length; ++id) {
        drawPos.y = (entity->field_6C * RSDK.Sin512((ang << 7) / divisor) >> 9) + entity->position.y;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x -= 0x100000;
        ang += 0x100000;
    }
}

void Bridge_Create(void *data)
{
    RSDK_THIS(Bridge);
    entity->visible = true;
    ++entity->length;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->active        = ACTIVE_BOUNDS;
    int len               = entity->length << 19;
    entity->startPos      = entity->position.x - len;
    entity->endPos        = len + entity->position.x;
    entity->updateRange.x = len;
    entity->updateRange.y = 0x800000;
    entity->playerPtr     = (Entity *)-1;
    entity->field_60      = 0xFF;
    RSDK.SetSpriteAnimation(Bridge->aniFrames, 0, &entity->animator, true, 0);
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

void Bridge_Burn(int offset)
{
    RSDK_THIS(Bridge);

    int size   = entity->stoodPos >> 20;
    int spawnX = entity->startPos + 0x80000;
    int off    = -offset;
    int ang    = 0x80000;
    for (int i = 0; i < size; ++i) {
        int sine = RSDK.Sin512((ang << 7) / entity->stoodPos);
        RSDK.CreateEntity(BurningLog->objectID, intToVoid(8 * abs(off++) + 16), spawnX, (entity->field_6C * sine >> 9) + entity->position.y);
        ang += 0x100000;
        spawnX += 0x100000;
    }

    int id = size;
    RSDK.CreateEntity(BurningLog->objectID, intToVoid(8 * abs(id++ - offset) + 16), spawnX, entity->field_6C + entity->position.y);

    spawnX      = entity->endPos - 0x80000;
    int divisor = entity->endPos - entity->startPos - entity->stoodPos;
    ang         = 0x80000;
    if (id >= entity->length) {
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
    else {
        off = offset - id;
        for (; id < entity->length; ++id, --off) {
            int spawnY = (entity->field_6C * RSDK.Sin512((ang << 7) / divisor) >> 9) + entity->position.y;
            RSDK.CreateEntity(BurningLog->objectID, intToVoid(8 * abs(entity->length - abs(off) - offset) + 16), spawnX, spawnY);
            ang += 0x100000;
            spawnX -= 0x100000;
        }
        RSDK.ResetEntityPtr(entity, TYPE_BLANK, NULL);
    }
}

void Bridge_EditorDraw(void) {}

void Bridge_EditorLoad(void) {}

void Bridge_Serialize(void)
{
    RSDK_EDITABLE_VAR(Bridge, VAR_UINT8, length);
    RSDK_EDITABLE_VAR(Bridge, VAR_BOOL, burnable);
}
