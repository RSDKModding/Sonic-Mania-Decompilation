#include "../SonicMania.hpp"

ObjectPlaneSwitch *PlaneSwitch;

void PlaneSwitch_Update()
{
    EntityPlayer *player = NULL;
    EntityPlaneSwitch *entity = (EntityPlaneSwitch *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveObjects(Player->objectID, (Entity **)&player)) {
        int x    = (player->position.x - entity->position.x) >> 8;
        int y    = (player->position.y - entity->position.y) >> 8;
        int scanX = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int scanY = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int pos       = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);
        if (entity->onPath && !player->onGround) {
            //Next loop, this one doesn't count
        }
        else {
            int xDif = scanX - entity->position.x >= 0 ? scanX - entity->position.x : entity->position.x - scanX;
            int yDif = scanY - entity->position.y >= 0 ? scanY - entity->position.y : entity->position.y - scanY;

            if (xDif < 0x180000 && yDif < entity->size << 19) {
                if (scanX + pos >= entity->position.x) {
                    player->collisionPlane = (entity->flags >> 3) & 1;
                    if (!(entity->flags & 4)) {
                        player->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        player->drawOrder = Zone->uiDrawLow;
                    }
                }
                else {
                    player->collisionPlane = (entity->flags >> 1) & 1;
                    if (!(entity->flags & 1)) {
                        player->drawOrder = Zone->playerDrawLow;
                    }
                    else {
                        player->drawOrder = Zone->uiDrawLow;
                    }
                }
            }
        }
    }
    entity->visible = DebugMode->active;
}

void PlaneSwitch_LateUpdate()
{

}

void PlaneSwitch_StaticUpdate()
{

}

void PlaneSwitch_Draw()
{
    Vector2 drawPos;
    EntityPlaneSwitch *entity = (EntityPlaneSwitch *)RSDK_sceneInfo->entity;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.x -= 0x80000;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3(entity, &drawPos, entity->angle);

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

void PlaneSwitch_Create(void* data)
{
    EntityPlaneSwitch *entity = (EntityPlaneSwitch *)RSDK_sceneInfo->entity;
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
        entity->negAngle      = (byte)-entity->angle;
    }
}

void PlaneSwitch_StageLoad() { PlaneSwitch->spriteIndex = RSDK.LoadAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void PlaneSwitch_EditorDraw()
{

}

void PlaneSwitch_EditorLoad()
{

}

void PlaneSwitch_Serialize()
{
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, flags);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_BOOL, onPath);
}

