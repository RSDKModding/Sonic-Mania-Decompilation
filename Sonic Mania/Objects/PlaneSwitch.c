#include "../SonicMania.h"

ObjectPlaneSwitch *PlaneSwitch;

void PlaneSwitch_Update(void)
{
    RSDK_THIS(PlaneSwitch);
    foreach_active(Player, player)
    {
        int x     = (player->position.x - entity->position.x) >> 8;
        int y     = (player->position.y - entity->position.y) >> 8;
        int scanX = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int scanY = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int pos   = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);
        if (!(entity->onPath && !player->onGround)) {
            int xDif = abs(scanX - entity->position.x);
            int yDif = abs(scanY - entity->position.y);

            if (xDif < 0x180000 && yDif < entity->size << 19) {
                if (scanX + pos >= entity->position.x) {
                    player->collisionPlane = (entity->flags >> 3) & 1;
                    if (!(entity->flags & 4)) {
                        player->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        player->drawOrder = Zone->playerDrawHigh;
                    }
                }
                else {
                    player->collisionPlane = (entity->flags >> 1) & 1;
                    if (!(entity->flags & 1)) {
                        player->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        player->drawOrder = Zone->playerDrawHigh;
                    }
                }
            }
        }
    }
    entity->visible = DebugMode->debugActive;
}

void PlaneSwitch_LateUpdate(void) {}

void PlaneSwitch_StaticUpdate(void) {}

void PlaneSwitch_Draw(void)
{
    Vector2 drawPos;
    RSDK_THIS(PlaneSwitch);

    drawPos.x = entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= 0x80000;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3((Entity *)entity, &drawPos, entity->angle);

    entity->data.frameID = entity->flags & 3;
    for (int i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->data, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    drawPos.x = entity->position.x + 0x80000;
    drawPos.y = entity->position.y - (entity->size << 19);

    int dx    = (drawPos.x + 0x80000 - entity->position.x) >> 8;
    int dy    = (drawPos.y - entity->position.y) >> 8;
    drawPos.x = (dy * RSDK.Sin256(entity->angle)) + dx * RSDK.Cos256(entity->angle) + entity->position.x;
    drawPos.y = (dy * RSDK.Cos256(entity->angle)) - dx * RSDK.Sin256(entity->angle) + entity->position.y;

    entity->data.frameID = (entity->flags >> 2) & 3;
    for (int i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->data, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }
}

void PlaneSwitch_Create(void *data)
{
    RSDK_THIS(PlaneSwitch);
    RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->data, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_BOUNDS;

        int x = 0;
        if (entity->size * RSDK.Sin256(entity->angle) << 11 >= 0)
            x = entity->size * RSDK.Sin256(entity->angle) << 11;
        else
            x = -0x800 * entity->size * RSDK.Sin256(entity->angle);
        entity->updateRange.x = x + 0x200000;

        int y = 0;
        if (entity->size * RSDK.Cos256(entity->angle) << 11 >= 0)
            y = entity->size * RSDK.Cos256(entity->angle) << 11;
        else
            y = -0x800 * entity->size * RSDK.Cos256(entity->angle);
        entity->visible       = false;
        entity->updateRange.y = y + 0x200000;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->negAngle      = (byte) - (byte)entity->angle;
    }
}

void PlaneSwitch_StageLoad(void) { PlaneSwitch->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void PlaneSwitch_EditorDraw(void) {}

void PlaneSwitch_EditorLoad(void) {}

void PlaneSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, flags);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_BOOL, onPath);
}
