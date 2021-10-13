#include "SonicMania.h"

ObjectLargeGear *LargeGear;

void LargeGear_Update(void)
{
    RSDK_THIS(LargeGear);
    entity->angle = entity->rotOffset + entity->rotSpeed * Zone->timer;
    int32 storeX    = entity->position.x;
    int32 storeY    = entity->position.y;

    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);

        int32 id      = 0;
        int32 storeID = -1;

        bool32 flag = false;

        for (int32 angle = 0x180; angle < 0x380; angle += 0x40) {
            int32 ang = angle + entity->angle;
            int32 x   = (storeX + 0x2400 * RSDK.Cos512(ang)) & 0xFFFF0000;
            int32 y   = (storeY + 0x2400 * RSDK.Sin512(ang)) & 0xFFFF0000;

            if (((1 << playerID) & entity->activePlayers) && entity->playerIDs[playerID] == id) {
                player->position.x += x - entity->positions[id].x;
                player->position.y += y - entity->positions[id].y;
            }

            entity->position.x = x;
            entity->position.y = y;

            if (Player_CheckCollisionBox(player, entity, &LargeGear->hitbox1) == C_TOP) {
                flag    = true;
                storeID = id;
            }
            entity->positions[id].x   = x;
            entity->positions[id++].y = y;
        }
        entity->position.x = storeX;
        entity->position.y = storeY;

        if (Player_CheckCollisionBox(player, entity, &LargeGear->hitbox2) == C_TOP) {
            flag    = true;
            storeID = -1;
            entity->activePlayers |= (1 << playerID);
        }
        else {
            if (flag)
                entity->activePlayers |= (1 << playerID);
            else
                entity->activePlayers &= ~(1 << playerID);
        }

        entity->playerIDs[playerID] = storeID;
        if (flag) {
            Hitbox *playerHitbox = Player_GetHitbox(player);

            RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_RWALL, entity->collisionPlane, playerHitbox->left << 16, 0, true);
            RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_LWALL, entity->collisionPlane, playerHitbox->right << 16, 0, true);
            RSDK.ObjectTileCollision(player, player->collisionLayers, CMODE_FLOOR, entity->collisionPlane, playerHitbox->bottom << 16, 0, true);
        }
    }

    entity->position.x = storeX;
    entity->position.y = storeY;
    RSDK.ProcessAnimation(&entity->animator1);
}

void LargeGear_LateUpdate(void) {}

void LargeGear_StaticUpdate(void) {}

void LargeGear_Draw(void)
{
    RSDK_THIS(LargeGear);
    Vector2 drawPos;

    for (int32 i = 0; i < 0x200; i += 0x40) {
        entity->rotation = i + entity->angle;

        RSDK.SetSpriteAnimation(LargeGear->aniFrames, 1, &entity->animator2, true, 7 - abs(RSDK.Sin512(entity->rotation) / 73));

        RSDK.DrawSprite(&entity->animator2, NULL, false);
        RSDK.Cos512(entity->rotation + 0x180);
        RSDK.Sin512(entity->rotation + 0x180);
        entity->rotation = 0;
    }

    entity->rotation = 0;
    RSDK.DrawSprite(&entity->animator1, NULL, false);

    entity->rotation = entity->angle;
    RSDK.DrawSprite(&entity->animator4, NULL, false);

    entity->rotation = 0;
    RSDK.DrawSprite(&entity->animator3, NULL, false);

    for (int32 i = 0; i < 0x200; i += 0x80) {
        int32 angle = i + entity->angle;
        drawPos   = entity->position;

        drawPos.x += 0x1300 * RSDK.Cos512(angle);
        drawPos.y += 0x1300 * RSDK.Sin512(angle);
        RSDK.DrawSprite(&entity->animator5, &drawPos, false);
    }
}

void LargeGear_Create(void *data)
{
    RSDK_THIS(LargeGear);
    entity->active        = ACTIVE_BOUNDS;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->origin        = entity->position;
    entity->visible       = true;
    entity->drawFX        = FX_ROTATE | FX_FLIP;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 0, &entity->animator1, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 1, &entity->animator2, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 2, &entity->animator3, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 3, &entity->animator4, true, 0);
    RSDK.SetSpriteAnimation(LargeGear->aniFrames, 4, &entity->animator5, true, 0);
    entity->angle = entity->rotOffset;
}

void LargeGear_StageLoad(void)
{
    LargeGear->aniFrames = RSDK.LoadSpriteAnimation("TMZ1/LargeGear.bin", SCOPE_STAGE);

    LargeGear->hitbox1.left   = -10;
    LargeGear->hitbox1.top    = -10;
    LargeGear->hitbox1.right  = 10;
    LargeGear->hitbox1.bottom = 10;

    LargeGear->hitbox2.left   = -8;
    LargeGear->hitbox2.top    = -64;
    LargeGear->hitbox2.right  = 8;
    LargeGear->hitbox2.bottom = 64;
}

void LargeGear_EditorDraw(void) {}

void LargeGear_EditorLoad(void) {}

void LargeGear_Serialize(void)
{
    RSDK_EDITABLE_VAR(LargeGear, VAR_INT32, rotSpeed);
    RSDK_EDITABLE_VAR(LargeGear, VAR_INT32, rotOffset);
}
