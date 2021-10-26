#include "SonicMania.h"

ObjectPlayerProbe *PlayerProbe;

void PlayerProbe_Update(void)
{
    RSDK_THIS(PlayerProbe);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        int32 x        = (player->position.x - entity->position.x) >> 8;
        int32 y        = (player->position.y - entity->position.y) >> 8;
        int32 scanX    = (y * RSDK.Sin256(entity->negAngle)) + (x * RSDK.Cos256(entity->negAngle)) + entity->position.x;
        int32 scanY    = (y * RSDK.Cos256(entity->negAngle)) - (x * RSDK.Sin256(entity->negAngle)) + entity->position.y;
        int32 pos      = ((player->velocity.y >> 8) * RSDK.Sin256(entity->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(entity->negAngle);
        RSDK.Cos256(entity->negAngle);
        RSDK.Sin256(entity->negAngle);

        int32 xDif = scanX - entity->position.x >= 0 ? scanX - entity->position.x : entity->position.x - scanX;
        int32 yDif = scanY - entity->position.y >= 0 ? scanY - entity->position.y : entity->position.y - scanY;

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

void PlayerProbe_Create(void *data)
{
    RSDK_THIS(PlayerProbe);
    RSDK.SetSpriteAnimation(PlaneSwitch->spriteIndex, 0, &entity->animator, true, 0);

    entity->drawFX |= FX_FLIP;
    entity->active       = ACTIVE_BOUNDS;
    entity->animator.frameID = 4;

    int32 x = 0;
    if (entity->size * RSDK.Sin256(entity->angle) << 11 >= 0)
        x = entity->size * RSDK.Sin256(entity->angle) << 11;
    else
        x = -0x800 * entity->size * RSDK.Sin256(entity->angle);
    entity->updateRange.x = x + 0x200000;

    int32 y = 0;
    if (entity->size * RSDK.Cos256(entity->angle) << 11 >= 0)
        y = entity->size * RSDK.Cos256(entity->angle) << 11;
    else
        y = -0x800 * entity->size * RSDK.Cos256(entity->angle);
    entity->visible       = false;
    entity->updateRange.y = y + 0x200000;
    entity->drawOrder     = Zone->drawOrderLow;
    entity->activePlayers = 0;
    entity->negAngle      = (uint8)-entity->angle;
}

void PlayerProbe_StageLoad(void) { PlayerProbe->spriteIndex = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void PlayerProbe_Print(EntityPlayer *player)
{
    RSDK_THIS(PlayerProbe);
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

void PlayerProbe_Draw(void)
{
    Vector2 drawPos;
    RSDK_THIS(PlayerProbe);

    drawPos.x = RSDK_sceneInfo->entity->position.x;
    drawPos.y = entity->position.y;
    drawPos.y -= entity->size << 19;
    Zone_Unknown3(&entity->position, &drawPos, entity->angle);
    for (int32 i = 0; i < entity->size; ++i) {
        RSDK.DrawSprite(&entity->animator, &drawPos, 0);
        drawPos.x += RSDK.Sin256(entity->angle) << 12;
        drawPos.y += RSDK.Cos256(entity->angle) << 12;
    }

    if (RSDK_sceneInfo->inEditor) {
        int32 x2     = entity->position.x;
        int32 y2     = entity->position.y;
        int32 x1     = entity->position.x;
        int32 y1     = entity->position.y;
        uint8 angle = -(uint8)(entity->angle);
        if (entity->direction)
            angle = -0x80 - (uint8)(entity->angle);
        x2 += 0x5000 * RSDK.Cos256(angle);
        y2 += 0x5000 * RSDK.Sin256(angle);
        uint32 clr = 0xFF00FF;
        if (!entity->direction)
            clr = 0xFFFF;

        PlayerProbe_DrawEditor(clr, x1, y1, x2, y2);
    }
}

void PlayerProbe_DrawEditor(uint32 colour, int32 x1, int32 y1, int32 x2, int32 y2)
{
    RSDK.DrawLine(x1, y1, x2, y2, colour, 0x7F, INK_ADD, 0);
    int32 angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), (RSDK.Sin256(angle + 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, (RSDK.Cos256(angle - 12) << 12) + x2, (RSDK.Sin256(angle - 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
}

#if RETRO_INCLUDE_EDITOR
void PlayerProbe_EditorDraw(void) {}

void PlayerProbe_EditorLoad(void) {}
#endif

void PlayerProbe_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_INT32, angle);
}
