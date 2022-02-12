// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ForceSpin Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectForceSpin *ForceSpin;

void ForceSpin_Update(void)
{
    RSDK_THIS(ForceSpin);
    foreach_active(Player, player)
    {
        int32 x     = (player->position.x - self->position.x) >> 8;
        int32 y     = (player->position.y - self->position.y) >> 8;
        int32 scanX = (y * RSDK.Sin256(self->negAngle)) + (x * RSDK.Cos256(self->negAngle)) + self->position.x;
        int32 scanY = (y * RSDK.Cos256(self->negAngle)) - (x * RSDK.Sin256(self->negAngle)) + self->position.y;
        int32 pos   = ((player->velocity.y >> 8) * RSDK.Sin256(self->negAngle)) + (player->velocity.x >> 8) * RSDK.Cos256(self->negAngle);
        RSDK.Cos256(self->negAngle);
        RSDK.Sin256(self->negAngle);

        int32 xDif = abs(scanX - self->position.x);
        int32 yDif = abs(scanY - self->position.y);

        if (xDif < 0x180000 && yDif < self->size << 19) {
            if (scanX + pos >= self->position.x) {
                if (self->direction) {
                    if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                    }
                }
                else {
                    ForceSpin_SetPlayerState(player);
                }
            }
            else {
                if (!self->direction) {
                    if (player->state == Player_State_ForceRoll_Ground || player->state == Player_State_ForceRoll_Air) {
                        if (player->onGround)
                            player->state = Player_State_Roll;
                        else
                            player->state = Player_State_Air;
                        player->nextGroundState = StateMachine_None;
                        player->nextAirState    = StateMachine_None;
                    }
                }
                else {
                    ForceSpin_SetPlayerState(player);
                }
            }
        }
    }
    self->visible = DebugMode->debugActive;
}

void ForceSpin_LateUpdate(void) {}

void ForceSpin_StaticUpdate(void) {}

void ForceSpin_Draw(void) { ForceSpin_DrawSprites(); }

void ForceSpin_Create(void *data)
{
    RSDK_THIS(ForceSpin);
    RSDK.SetSpriteAnimation(ForceSpin->aniFrames, 0, &self->animator, true, 0);
    self->drawFX |= FX_FLIP;
    self->animator.frameID = 4;
    if (!SceneInfo->inEditor) {
        self->active = ACTIVE_BOUNDS;
        self->updateRange.x = abs(self->size * RSDK.Sin256(self->angle) << 11) + 0x200000;
        self->updateRange.y = abs(self->size * RSDK.Cos256(self->angle) << 11) + 0x200000;
        self->visible       = false;
        self->drawOrder     = Zone->drawOrderLow;
        self->negAngle      = (uint8) - (uint8)self->angle;
    }
}

void ForceSpin_StageLoad(void) { ForceSpin->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void ForceSpin_DrawSprites(void)
{
    Vector2 drawPos;

    RSDK_THIS(ForceSpin);
    drawPos.x = self->position.x;
    drawPos.y = self->position.y - (self->size << 19);
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }
}
void ForceSpin_SetPlayerState(void *plr)
{
    RSDK_THIS(ForceSpin);
    EntityPlayer *player = (EntityPlayer *)plr;
    if (player->state != Player_State_ForceRoll_Ground && player->state != Player_State_ForceRoll_Air) {
        if (player->animator.animationID != ANI_JUMP) {
            RSDK.PlaySfx(Player->sfxRoll, false, 0xFF);
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            if (!player->collisionMode && player->onGround)
                player->position.y += player->jumpOffset;
            player->pushing = 0;
        }

        player->nextAirState = StateMachine_None;
        if (player->onGround)
            player->state = Player_State_ForceRoll_Ground;
        else
            player->state = Player_State_ForceRoll_Air;

        player->nextGroundState = StateMachine_None;
        if (abs(player->groundVel) < 0x10000) {
            if (self->direction & FLIP_X)
                player->groundVel = -0x40000;
            else
                player->groundVel = 0x40000;
        }
    }
}

void ForceSpin_EditorDraw(void) { ForceSpin_DrawSprites(); }

void ForceSpin_EditorLoad(void) { ForceSpin->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void ForceSpin_Serialize(void)
{
    RSDK_EDITABLE_VAR(ForceSpin, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_INT32, angle);
}
