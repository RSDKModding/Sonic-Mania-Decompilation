#include "SonicMania.h"

ObjectPlaneSwitch *PlaneSwitch;

void PlaneSwitch_Update(void)
{
    RSDK_THIS(PlaneSwitch);
    foreach_active(Player, player)
    {
        int32 x     = (player->position.x - entity->position.x) >> 8;
        int32 y     = (player->position.y - entity->position.y) >> 8;
        int32 scanX = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int32 scanY = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int32 pos   = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);
        if (!(entity->onPath && !player->onGround)) {
            int32 xDif = abs(scanX - entity->position.x);
            int32 yDif = abs(scanY - entity->position.y);

            if (xDif < 0x180000 && yDif < entity->size << 19) {
                if (scanX + pos >= entity->position.x) {
                    player->collisionPlane = (entity->flags >> 3) & 1;
                    if (!(entity->flags & 4))
                        player->drawOrder = Zone->playerDrawLow;
                    else
                        player->drawOrder = Zone->playerDrawHigh;
                }
                else {
                    player->collisionPlane = (entity->flags >> 1) & 1;
                    if (!(entity->flags & 1))
                        player->drawOrder = Zone->playerDrawLow;
                    else
                        player->drawOrder = Zone->playerDrawHigh;
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
    Zone_Unknown3(&entity->position, &drawPos, entity->angle);

    entity->animator.frameID = entity->flags & 3;
    for (int32 i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    drawPos.x = entity->position.x + 0x80000;
    drawPos.y = entity->position.y - (entity->size << 19);

    int32 dx    = (drawPos.x + 0x80000 - entity->position.x) >> 8;
    int32 dy    = (drawPos.y - entity->position.y) >> 8;
    drawPos.x = (dy * RSDK.Sin256(entity->angle)) + dx * RSDK.Cos256(entity->angle) + entity->position.x;
    drawPos.y = (dy * RSDK.Cos256(entity->angle)) - dx * RSDK.Sin256(entity->angle) + entity->position.y;

    entity->animator.frameID = (entity->flags >> 2) & 3;
    for (int32 i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }
}

void PlaneSwitch_Create(void *data)
{
    RSDK_THIS(PlaneSwitch);
    RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active = ACTIVE_BOUNDS;

        int32 x = 0;
        if (entity->size * RSDK.Sin256(entity->angle) << 11 >= 0)
            x = entity->size * RSDK.Sin256(entity->angle) << 11;
        else
            x = -(entity->size * RSDK.Sin256(entity->angle) << 11);
        entity->updateRange.x = x + 0x200000;

        int32 y = 0;
        if (entity->size * RSDK.Cos256(entity->angle) << 11 >= 0)
            y = entity->size * RSDK.Cos256(entity->angle) << 11;
        else
            y = -(entity->size * RSDK.Cos256(entity->angle) << 11);

        entity->visible       = false;
        entity->updateRange.y = y + 0x200000;
        entity->drawOrder     = Zone->drawOrderLow;
        entity->negAngle      = (uint8) - (uint8)entity->angle;
    }
}

void PlaneSwitch_StageLoad(void) { PlaneSwitch->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void PlaneSwitch_EditorDraw(void) { PlaneSwitch_Draw(); }

void PlaneSwitch_EditorLoad(void) { PlaneSwitch_StageLoad(); }
#endif

void PlaneSwitch_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, flags);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_INT32, angle);
    RSDK_EDITABLE_VAR(PlaneSwitch, VAR_BOOL, onPath);
}
