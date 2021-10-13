#include "SonicMania.h"

ObjectGondola *Gondola;

void Gondola_Update(void)
{
    RSDK_THIS(Gondola);
    entity->field_94.x = -entity->field_8C.x;
    entity->field_94.y = -entity->field_8C.y;
    entity->field_84.x += entity->velocity.x;
    entity->field_84.y += entity->velocity.y;

    if (entity->activePlayers) {
        if (entity->field_9C < entity->field_A0) {
            entity->field_9C += 4;
        }
    }
    else if (entity->field_9C > 0) {
        entity->field_9C -= 4;
    }

    Gondola_Unknown2();
    Gondola_Unknown3();
    Gondola_Unknown4();
    if (entity->onGround && entity->velocity.y > 0)
        entity->velocity.y = 0;

    entity->field_8C.x = entity->field_84.x;
    entity->field_8C.y = entity->field_84.y;
    entity->field_8C.y += RSDK.Sin256(entity->field_9C) << 10;
    entity->field_8C.y += 384 * RSDK.Sin512(2 * entity->field_A4);
    if (!entity->onGround)
        ++entity->field_A4;
    entity->position.x = entity->field_8C.x;
    entity->position.y = entity->field_8C.y;
    entity->position.x &= 0xFFFF0000;
    entity->position.y &= 0xFFFF0000;
    Gondola_Unknown5();
    if (entity->onGround)
        entity->drawOrder = Zone->drawOrderLow;
    else
        entity->drawOrder = Zone->playerDrawLow;
    entity->field_94.x += entity->position.x;
    entity->field_94.y += entity->position.y;
    Gondola_Unknown6();
}

void Gondola_LateUpdate(void) {}

void Gondola_StaticUpdate(void) {}

void Gondola_Draw(void)
{
    RSDK_THIS(Gondola);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void Gondola_Create(void *data)
{
    RSDK_THIS(Gondola);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = Zone->playerDrawHigh;
    entity->field_68.x    = entity->position.x;
    entity->field_68.y    = entity->position.y;
    entity->field_8C.x    = entity->position.x;
    entity->field_8C.y    = entity->position.y;
    entity->field_84.x    = entity->position.x;
    entity->field_84.y    = entity->position.y;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    entity->drawOrder     = Zone->playerDrawLow;

    entity->hitbox.left   = -76;
    entity->hitbox.top    = -12;
    entity->hitbox.right  = 76;
    entity->hitbox.bottom = 4;
    RSDK.SetSpriteAnimation(Gondola->aniFrames, 0, &entity->animator, true, 0);
}

void Gondola_StageLoad(void) { Gondola->aniFrames = RSDK.LoadSpriteAnimation("HCZ/Gondola.bin", SCOPE_STAGE); }

int32 Gondola_GetWaterLevel(void)
{
    RSDK_THIS(Gondola);
    if (RSDK_sceneInfo->entity->position.y < Water->waterLevel) {
        foreach_active(Water, water)
        {
            if (water->type == 1 && RSDK.CheckObjectCollisionTouchBox(water, &water->hitbox, entity, &Water->hitbox)) {
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

    entity->field_80 = entity->field_84.y - waterLevel;
    entity->field_74 = abs(entity->field_84.y - waterLevel) < 0x40000;
    if (abs(entity->field_84.y - waterLevel) >= 0x40000) {
        if (entity->field_84.y - waterLevel < -0x80000) {
            entity->velocity.y += 0x3800;
        }
        else {
            if (entity->field_84.y <= waterLevel)
                entity->velocity.y = 0;
            else
                entity->velocity.y = -Water->waterMoveSpeed;
        }
    }
    else {
        if (Water->targetWaterLevel > Water->waterLevel) {
            entity->velocity.y = Water->waterMoveSpeed;
        }
        else {
            if (entity->field_84.y <= waterLevel)
                entity->velocity.y = 0;
            else
                entity->velocity.y = -Water->waterMoveSpeed;
        }
    }

    if (entity->minY) {
        if (entity->field_84.y <= entity->minY << 16) {
            entity->field_84.y = entity->minY << 16;
            if (entity->velocity.y < 0)
                entity->velocity.y = 0;
        }
    }

    if (entity->maxY) {
        if (entity->field_84.y >= entity->maxY << 16) {
            entity->field_84.y = entity->maxY << 16;
            if (entity->velocity.y > 0)
                entity->velocity.y = 0;
        }
    }
}

void Gondola_Unknown3(void)
{
    RSDK_THIS(Gondola);

    int32 desiredRotation = 0;
    if (!entity->onGround) {
        foreach_active(Player, player)
        {
            if (((1 << RSDK.GetEntityID(player)) & entity->activePlayers))
                desiredRotation += (player->position.x - entity->field_84.x) >> 21;
        }
    }

    if (entity->rotation < desiredRotation) {
        entity->rotation++;
    }
    else if (entity->rotation > desiredRotation) {
        entity->rotation--;
    }
}

void Gondola_Unknown4(void)
{
    RSDK_THIS(Gondola);
    if (!RSDK_sceneInfo->entity->onGround) {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);
            if (!player->sidekick) {
                if (((1 << playerID) & entity->activePlayers)) {
                    entity->velocity.x = (player->position.x - entity->field_84.x) >> 5;
                }
                else {
                    if (entity->velocity.x > 0) {
                        entity->velocity.x -= 512;
                        if (entity->velocity.x - 512 < 0)
                            entity->velocity.x = 0;
                    }
                    else if (entity->velocity.x < 0) {
                        entity->velocity.x += 512;
                        if (entity->velocity.x > 0)
                            entity->velocity.x = 0;
                    }
                }
            }
        }
    }
}

void Gondola_Unknown5(void)
{
    RSDK_THIS(Gondola);
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_FLOOR, 0, 0, entity->hitbox.bottom << 16, true)) {
        entity->field_A0 = 0;
        if (entity->velocity.y > 0)
            entity->velocity.y = 0;
        entity->onGround = true;
    }
    else {
        entity->field_A0 = 64;
        entity->onGround = false;
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_ROOF, 0, 0, entity->hitbox.top << 16, true) && entity->velocity.y < 0) {
        entity->velocity.y = 0;
    }
    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_RWALL, 0, (entity->hitbox.left - 8) << 16, 0, true)) {
        if (entity->velocity.x < 0)
            entity->velocity.x = 0;
        entity->flag = true;
    }
    else {
        entity->flag = false;
    }

    if (RSDK.ObjectTileCollision(entity, Zone->fgLayers, CMODE_LWALL, 0, (entity->hitbox.right + 8) << 16, 0, true)) {
        if (entity->velocity.x > 0)
            entity->velocity.x = 0;
        entity->field_8C.x = entity->position.x;
        entity->field_8C.y = entity->position.y;
        entity->flag2      = true;
    }
    else {
        entity->field_8C.x = entity->position.x;
        entity->field_8C.y = entity->position.y;
        entity->flag2      = false;
    }
}

void Gondola_Unknown6(void)
{
    RSDK_THIS(Gondola);
    if (entity->onGround) {
        entity->activePlayers = 0;
    }
    else {
        foreach_active(Player, player)
        {
            int32 playerID = RSDK.GetEntityID(player);
            if (((1 << playerID) & entity->activePlayers)) {
                player->position.x += entity->field_94.x;
                player->position.y += entity->field_94.y;
                player->position.y += 0x10000;
                if (!player->sidekick && !Gondola->hasAchievement) {
                    if (!Gondola->taggedBoatIDs[entity->boatID]) {
                        Gondola->taggedBoatIDs[entity->boatID] = 1;
                        if (Gondola->taggedBoatIDs[0] && Gondola->taggedBoatIDs[1] && Gondola->taggedBoatIDs[2]) {
#if RETRO_USE_PLUS
                            API.UnlockAchievement("ACH_HCZ");
#else
                            APICallback_UnlockAchievement("ACH_HCZ");
#endif
                            Gondola->hasAchievement = true;
                        }
                    }
                }
            }

            if (Player_CheckCollisionPlatform(player, entity, &entity->hitbox))
                entity->activePlayers |= (1 << playerID);
            else
                entity->activePlayers &= ~(1 << playerID);
        }
    }
}

void Gondola_EditorDraw(void) {}

void Gondola_EditorLoad(void) {}

void Gondola_Serialize(void)
{
    RSDK_EDITABLE_VAR(Gondola, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(Gondola, VAR_ENUM, minY);
    RSDK_EDITABLE_VAR(Gondola, VAR_ENUM, maxY);
    RSDK_EDITABLE_VAR(Gondola, VAR_UINT8, boatID);
}
