// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlayerProbe Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPlayerProbe *PlayerProbe;

void PlayerProbe_Update(void)
{
    RSDK_THIS(PlayerProbe);
    foreach_active(Player, player)
    {
        int32 playerID = RSDK.GetEntityID(player);
        int32 x        = (player->position.x - self->position.x) >> 8;
        int32 y        = (player->position.y - self->position.y) >> 8;
        int32 scanX    = (y * RSDK.Sin256(self->negAngle)) + (x * RSDK.Cos256(self->negAngle)) + self->position.x;
        int32 scanY    = (y * RSDK.Cos256(self->negAngle)) - (x * RSDK.Sin256(self->negAngle)) + self->position.y;
        int32 pos      = ((player->velocity.y >> 8) * RSDK.Sin256(self->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(self->negAngle);
        RSDK.Cos256(self->negAngle);
        RSDK.Sin256(self->negAngle);

        int32 xDif = scanX - self->position.x >= 0 ? scanX - self->position.x : self->position.x - scanX;
        int32 yDif = scanY - self->position.y >= 0 ? scanY - self->position.y : self->position.y - scanY;

        if (xDif < 0x180000 && yDif < self->size << 19) {
            if (scanX + pos >= self->position.x) {
                if (!self->direction) {
                    if (!((1 << playerID) & self->activePlayers))
                        PlayerProbe_Print(player);
                    self->activePlayers |= (1 << playerID);
                }
            }
            else {
                if (self->direction) {
                    if (!((1 << playerID) & self->activePlayers))
                        PlayerProbe_Print(player);
                    self->activePlayers |= (1 << playerID);
                }
            }
        }
        else {
            self->activePlayers &= ~(1 << playerID);
        }
    }
    self->visible = DebugMode->debugActive;
}

void PlayerProbe_LateUpdate(void) {}

void PlayerProbe_StaticUpdate(void) {}

void PlayerProbe_Draw(void) { PlayerProbe_DrawSprites(); }

void PlayerProbe_Create(void *data)
{
    RSDK_THIS(PlayerProbe);
    RSDK.SetSpriteAnimation(PlaneSwitch->aniFrames, 0, &self->animator, true, 0);

    self->drawFX |= FX_FLIP;
    self->active           = ACTIVE_BOUNDS;
    self->animator.frameID = 4;

    self->updateRange.x = abs(self->size * RSDK.Sin256(self->angle) << 11) + 0x200000;
    self->updateRange.y = abs(self->size * RSDK.Cos256(self->angle) << 11) + 0x200000;
    self->visible       = false;
    self->drawOrder     = Zone->drawOrderLow;
    self->activePlayers = 0;
    self->negAngle      = (uint8) - (self->angle & 0xFF);
}

void PlayerProbe_StageLoad(void)
{
    PlayerProbe->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);
}

void PlayerProbe_Print(EntityPlayer *player)
{
    RSDK_THIS(PlayerProbe);
    if (!SceneInfo->inEditor) {
        LogHelpers_Print("====================");
        LogHelpers_Print("= Begin Probe      =");
        LogHelpers_Print("====================");
        if (self->direction)
            LogHelpers_Print("self->direction = S/U");
        else
            LogHelpers_Print("self->direction = U/S");
        LogHelpers_Print("self->angle = %i", self->angle);
        LogHelpers_Print("Cos256(self->angle) = %i", RSDK.Cos256(self->angle));
        LogHelpers_Print("Sin256(self->angle) = %i", RSDK.Sin256(self->angle));
        LogHelpers_Print("====================");
        if (player->direction)
            LogHelpers_Print("self->direction = FACING_LEFT");
        else
            LogHelpers_Print("self->direction = FACING_RIGHT");
        LogHelpers_Print("playerPtr->groundVel = %i", player->groundVel);
        LogHelpers_Print("playerPtr->angle = %i", player->angle);
        LogHelpers_Print("playerPtr->collisionMode = %i", player->collisionMode);
        LogHelpers_Print("playerPtr->onGround = %i", player->onGround);
        LogHelpers_Print("====================");
        LogHelpers_Print("= End Probe        =");
        LogHelpers_Print("====================");
    }
}

void PlayerProbe_DrawSprites(void)
{
    RSDK_THIS(PlayerProbe);
    Vector2 drawPos;

    drawPos.x = self->position.x;
    drawPos.y = self->position.y;
    drawPos.y -= self->size << 19;
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);
    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }

    if (SceneInfo->inEditor) {
        int32 x2    = self->position.x;
        int32 y2    = self->position.y;
        int32 x1    = self->position.x;
        int32 y1    = self->position.y;
        uint8 angle = -(uint8)(self->angle);
        if (self->direction)
            angle = -0x80 - (uint8)(self->angle);
        x2 += 0x5000 * RSDK.Cos256(angle);
        y2 += 0x5000 * RSDK.Sin256(angle);
        uint32 clr = 0xFF00FF;
        if (!self->direction)
            clr = 0xFFFF;

        PlayerProbe_DrawArrow(x1, y1, x2, y2, clr);
    }
}

void PlayerProbe_DrawArrow(int32 x1, int32 y1, int32 x2, int32 y2, uint32 colour)
{
    int32 angle = RSDK.ATan2(x1 - x2, y1 - y2);
    RSDK.DrawLine(x1, y1, x2, y2, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, x2 + (RSDK.Cos256(angle + 12) << 12), (RSDK.Sin256(angle + 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
    RSDK.DrawLine(x2, y2, (RSDK.Cos256(angle - 12) << 12) + x2, (RSDK.Sin256(angle - 12) << 12) + y2, colour, 0x7F, INK_ADD, false);
}

#if RETRO_INCLUDE_EDITOR
void PlayerProbe_EditorDraw(void)
{
    RSDK_THIS(PlayerProbe);
    self->updateRange.x = abs(self->size * RSDK.Sin256(self->angle) << 11) + 0x200000;
    self->updateRange.y = abs(self->size * RSDK.Cos256(self->angle) << 11) + 0x200000;
    self->visible       = false;
    self->drawOrder     = Zone->drawOrderLow;
    self->activePlayers = 0;
    self->negAngle      = (uint8) - (self->angle & 0xFF);

    PlayerProbe_Draw();
}

void PlayerProbe_EditorLoad(void)
{
    PlayerProbe->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PlayerProbe, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flip X", FLIP_X);
}
#endif

void PlayerProbe_Serialize(void)
{
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(PlayerProbe, VAR_INT32, angle);
}
