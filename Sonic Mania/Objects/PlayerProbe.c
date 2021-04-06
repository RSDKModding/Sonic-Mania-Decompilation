#include "../SonicMania.h"

ObjectPlayerProbe *PlayerProbe;

void PlayerProbe_Update(void)
{
    EntityPlayer *player      = NULL;
    EntityPlayerProbe *entity = (EntityPlayerProbe *)RSDK_sceneInfo->entity;
    while (RSDK.GetActiveEntities(Player->objectID, (Entity **)&player)) {
        int playerID = RSDK.GetEntityID(player);
        int x        = (player->position.x - entity->position.x) >> 8;
        int y        = (player->position.y - entity->position.y) >> 8;
        int scanX    = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int scanY    = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int pos      = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);

        int xDif = scanX - entity->position.x >= 0 ? scanX - entity->position.x : entity->position.x - scanX;
        int yDif = scanY - entity->position.y >= 0 ? scanY - entity->position.y : entity->position.y - scanY;

        if (xDif < 0x180000 && yDif < entity->size << 19) {
            if (scanX + pos >= entity->position.x) {
                if (!entity->direction) {
                    if (!((1 << playerID) & entity->activePlayers))
                        PlayerProbe_Print(player);
                    entity->activePlayers |= (1 << playerID);
                }
            }
            else {
                if (entity->direction) {
                    if (!((1 << playerID) & entity->activePlayers))
                        PlayerProbe_Print(player);
                    entity->activePlayers |= (1 << playerID);
                }
            }
        }
        else {
            entity->activePlayers &= ~(1 << playerID);
        }
    }
    entity->visible = DebugMode->debugActive;
}

void PlayerProbe_LateUpdate(void) {}

void PlayerProbe_StaticUpdate(void) {}

void PlayerProbe_Draw(void)
{
    Vector2 drawPos;
    EntityPlayerProbe *entity = (EntityPlayerProbe *)RSDK_sceneInfo->entity;

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3((Entity *)entity, &drawPos, entity->angle);
    for (int i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->data, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    if (RSDK_sceneInfo->inEditor) {
        int x2     = entity->position.x;
        int y2     = entity->position.y;
        int x1     = entity->position.x;
        int y1     = entity->position.y;
        byte angle = -(byte)(entity->angle);
        if (entity->direction)
            angle = -0x80 - (byte)(entity->angle);
        x2 += 0x5000 * RSDK.Cos256(angle);
        y2 += 0x5000 * RSDK.Sin256(angle);
        PlayerProbe_DrawEditor(x1, y1, x2, y2);
    }
}

void PlayerProbe_DrawEditor(int x1, int y1, int x2, int y2)
{
    uint colour = 0xFFFFFF; // wasn't assigned in ida so...
    RSDK.DrawLine(x1, y1, x2, y2, colour, 0x7F, INK_ADD, 0);
    int angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), (RSDK.Sin256(angle + 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, (RSDK.Cos256(angle - 12) << 12) + x2, (RSDK.Sin256(angle - 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
}

void PlayerProbe_DrawDebug2(uint colour, byte dir, int x, int y, Hitbox *hitbox)
{
    short left, top, right, bottom;

    if (dir & FLIP_X) {
        right = -hitbox->right;
        left  = -hitbox->left;
    }
    else {
        left  = hitbox->left;
        right = hitbox->right;
    }

    if (dir & FLIP_Y) {
        top    = -hitbox->top;
        bottom = -hitbox->bottom;
    }
    else {
        top    = hitbox->top;
        bottom = hitbox->bottom;
    }

    RSDK.DrawLine(x + (left << 16), (top << 16) + y, x + (right << 16), (top << 16) + y, colour, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (right << 16), y + (top << 16), x + (right << 16), y + (bottom << 16), colour, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (right << 16), y + (top << 16), x + (left << 16), y + (bottom << 16), colour, 0xFF, INK_NONE, false);
    RSDK.DrawLine(x + (left << 16), y + (top << 16), x + (left << 16), y + (top << 16), colour, 0xFF, INK_NONE, false);
}

void PlayerProbe_DrawDebug3(int x1, int y1, int x2, int y2)
{
    uint colour = 0xFFFFFF; // wasn't assigned in ida so...
    RSDK.DrawLine(x1, y1, x2, y2, colour, 0x7F, INK_ADD, 0);
    int angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), (RSDK.Sin256(angle + 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, (RSDK.Cos256(angle - 12) << 12) + x2, (RSDK.Sin256(angle - 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
}

void PlayerProbe_DrawDebug4(uint colour, int a2, int alpha, int x1, int y1, int x2, int y2)
{
    Vector2 verts[3];

    int angle  = RSDK.ATan2(x2 - x1, y2 - y1);
    verts[0].x = x2;
    verts[0].y = x2;
    verts[1].x = (RSDK.Cos256(angle + 64) << 9) + x1;
    verts[1].y = (RSDK.Sin256(angle + 64) << 9) + y1;
    verts[2].x = (RSDK.Cos256(angle - 64) << 9) + x1;
    verts[2].y = (RSDK.Sin256(angle - 64) << 9) + y1;

    if (RSDK_sceneInfo->inEditor) {
        RSDK.DrawLine(verts[0].x, verts[0].y, verts[1].x, verts[1].y, colour, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[1].x, verts[1].y, verts[2].x, verts[2].y, colour, 0xFF, INK_NONE, false);
        RSDK.DrawLine(verts[2].x, verts[2].y, verts[0].x, verts[0].y, colour, 0xFF, INK_NONE, false);
    }
    else {
        verts[0].x -= RSDK_screens->position.x << 16;
        verts[0].y -= RSDK_screens->position.y << 16;
        verts[1].x -= RSDK_screens->position.x << 16;
        verts[1].y -= RSDK_screens->position.y << 16;
        verts[2].x -= RSDK_screens->position.x << 16;
        verts[2].y -= RSDK_screens->position.y << 16;
        RSDK.DrawQuad(verts, 3, (colour >> 16) & 0xFF, (colour >> 8) & 0xFF, (colour >> 0) & 0xFF, alpha, INK_ALPHA);
    }
}

void PlayerProbe_DrawDebug5(int x, int y)
{
    uint colour = 0xFFFFFF; // wasn't assigned in ida so...
    if (x || y) {
        RSDK.DrawLine(x - 0x100000, y - 0x100000, x + 0x100000, y + 0x100000, colour, 0x7F, INK_NONE, 0);
        RSDK.DrawLine(x + 0x100000, y - 0x100000, x - 0x100000, y + 0x100000, colour, 0x7F, INK_NONE, 0);
    }
}

void PlayerProbe_Create(void *data)
{
    EntityPlayerProbe *entity = (EntityPlayerProbe *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->data, true, 0);

    entity->drawFX |= FX_FLIP;
    entity->active       = ACTIVE_BOUNDS;
    entity->data.frameID = 4;

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
    entity->activePlayers = 0;
    entity->negAngle      = (byte)-entity->angle;
}

void PlayerProbe_StageLoad(void) { PlayerProbe->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void PlayerProbe_Print(EntityPlayer *player)
{
    EntityPlayerProbe *entity = (EntityPlayerProbe *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        LogHelpers_Print("====================");
        LogHelpers_Print("= Begin Probe      =");
        LogHelpers_Print("====================");
        if (entity->direction)
            LogHelpers_Print("self->direction = S/U");
        else
            LogHelpers_Print("self->direction = U/S");
        LogHelpers_Print("self->angle = %i", entity->angle);
        LogHelpers_Print("Cos256(self->angle) = %i", RSDK.Cos256(entity->angle));
        LogHelpers_Print("Sin256(self->angle) = %i", RSDK.Sin256(entity->angle));
        LogHelpers_Print("====================");
        if (player->direction)
            LogHelpers_Print("self->direction = FACING_LEFT");
        else
            LogHelpers_Print("self->direction = FACING_RIGHT");
        LogHelpers_Print("playerPtr->groundVel = %1", player->groundVel);
        LogHelpers_Print("playerPtr->angle = %i", player->angle);
        LogHelpers_Print("playerPtr->collisionMode = %i", player->collisionMode);
        LogHelpers_Print("playerPtr->onGround = %i", player->onGround);
        LogHelpers_Print("====================");
        LogHelpers_Print("= End Probe        =");
        LogHelpers_Print("====================");
    }
}

void PlayerProbe_EditorDraw(void) {}

void PlayerProbe_EditorLoad(void) {}

void PlayerProbe_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_INT32, angle);
}
