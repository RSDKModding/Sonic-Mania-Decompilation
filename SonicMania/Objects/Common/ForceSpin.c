// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ForceSpin Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectForceSpin *ForceSpin;

void ForceSpin_Update(void)
{
    RSDK_THIS(ForceSpin);

    foreach_active(Player, player)
    {
        Vector2 pivotPos = player->position;
        Vector2 pivotVel = player->velocity;

        Zone_RotateOnPivot(&pivotPos, &self->position, self->negAngle);
        Zone_RotateOnPivot(&pivotVel, &self->velocity, self->negAngle);

        if (abs(pivotPos.x - self->position.x) < TO_FIXED(24) && abs(pivotPos.y - self->position.y) < self->size << 19) {
            if (pivotPos.x >= self->position.x) {
                if (self->direction) {
                    if (player->state == Player_State_TubeRoll || player->state == Player_State_TubeAirRoll) {
                        player->state           = player->onGround ? Player_State_Roll : Player_State_Air;
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
                    if (player->state == Player_State_TubeRoll || player->state == Player_State_TubeAirRoll) {
                        player->state           = player->onGround ? Player_State_Roll : Player_State_Air;
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
    self->animator.frameID = 4;
    self->drawFX |= FX_FLIP;

    if (!SceneInfo->inEditor) {
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = TO_FIXED(32) + abs(self->size * RSDK.Sin256(self->angle) << 11);
        self->updateRange.y = TO_FIXED(32) + abs(self->size * RSDK.Cos256(self->angle) << 11);
        self->visible       = false;
        self->drawGroup     = Zone->objectDrawGroup[0];
        self->negAngle      = -self->angle & 0xFF;
    }
}

void ForceSpin_StageLoad(void) { ForceSpin->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }

void ForceSpin_DrawSprites(void)
{
    RSDK_THIS(ForceSpin);

    Vector2 drawPos;
    drawPos.x = self->position.x;
    drawPos.y = self->position.y - (self->size << 19);
    Zone_RotateOnPivot(&drawPos, &self->position, self->angle);

    for (int32 i = 0; i < self->size; ++i) {
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += RSDK.Sin256(self->angle) << 12;
        drawPos.y += RSDK.Cos256(self->angle) << 12;
    }
}
void ForceSpin_SetPlayerState(EntityPlayer *player)
{
    RSDK_THIS(ForceSpin);

    if (player->state != Player_State_TubeRoll && player->state != Player_State_TubeAirRoll) {
        if (player->animator.animationID != ANI_JUMP) {
            RSDK.PlaySfx(Player->sfxRoll, false, 0xFF);
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_JUMP, &player->animator, false, 0);
            if (!player->collisionMode && player->onGround)
                player->position.y += player->jumpOffset;
            player->pushing = 0;
        }

        player->state           = player->onGround ? Player_State_TubeRoll : Player_State_TubeAirRoll;
        player->nextGroundState = StateMachine_None;
        player->nextAirState    = StateMachine_None;

        if (abs(player->groundVel) < 0x10000) {
            if (self->direction & FLIP_X)
                player->groundVel = -0x40000;
            else
                player->groundVel = 0x40000;
        }
    }
}

#if GAME_INCLUDE_EDITOR
void ForceSpin_EditorDraw(void)
{
    RSDK_THIS(ForceSpin);

    self->updateRange.x = TO_FIXED(32) + abs(self->size * RSDK.Sin256(self->angle) << 11);
    self->updateRange.y = TO_FIXED(32) + abs(self->size * RSDK.Cos256(self->angle) << 11);

    ForceSpin_DrawSprites();
}

void ForceSpin_EditorLoad(void)
{
    ForceSpin->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(ForceSpin, direction);
    RSDK_ENUM_VAR("Enter From Right, Exit From Left", FLIP_NONE);
    RSDK_ENUM_VAR("Enter From Left, Exit From Right", FLIP_X);
}
#endif

void ForceSpin_Serialize(void)
{
    RSDK_EDITABLE_VAR(ForceSpin, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_ENUM, size);
    RSDK_EDITABLE_VAR(ForceSpin, VAR_INT32, angle);
}
