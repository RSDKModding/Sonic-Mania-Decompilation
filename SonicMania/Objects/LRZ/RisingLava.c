// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: RisingLava Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectRisingLava *RisingLava;

void RisingLava_Update(void)
{
    RSDK_THIS(RisingLava);

    StateMachine_Run(self->state);
}

void RisingLava_LateUpdate(void) {}

void RisingLava_StaticUpdate(void) {}

void RisingLava_Draw(void) {}

void RisingLava_Create(void *data)
{
    RSDK_THIS(RisingLava);

    if (!SceneInfo->inEditor) {
        self->hitbox.left   = -(self->size.x >> 17);
        self->hitbox.top    = -(self->size.y >> 17);
        self->hitbox.right  = self->size.x >> 17;
        self->hitbox.bottom = self->size.y >> 17;

        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x800000;
        self->updateRange.y = 0x800000;
        self->limit *= -0x10000;
        self->state = RisingLava_State_CheckRiseStart;
    }
}

void RisingLava_StageLoad(void) { RisingLava->sfxRumble = RSDK.GetSfx("Stage/Rumble.wav"); }

void RisingLava_State_CheckRiseStart(void)
{
    RSDK_THIS(RisingLava);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !player->sidekick) {
            TileLayer *move               = RSDK.GetTileLayer(Zone->moveLayer);
            move->drawGroup[0]            = 5;
            move->scrollInfo[0].scrollPos = -self->offset.x;
            move->scrollPos               = -self->offset.y;
            self->active                  = ACTIVE_NORMAL;
            self->state                   = RisingLava_State_RiseShake;
        }
    }
}

void RisingLava_State_RiseShake(void)
{
    RSDK_THIS(RisingLava);

    TileLayer *move = RSDK.GetTileLayer(Zone->moveLayer);

    move->scrollPos += 0x8000;
    foreach_active(Player, player)
    {
        player->collisionLayers |= Zone->moveLayerMask;
        player->moveLayerPosition.x = move->scrollInfo[0].scrollPos;
        player->moveLayerPosition.y = move->scrollPos;
    }

    if (!(Zone->timer & 3))
        Camera_ShakeScreen(0, 0, 2);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(RisingLava->sfxRumble, false, 255);

    if (move->scrollPos > self->limit)
        self->state = RisingLava_State_StoppedRising;
}

void RisingLava_State_StoppedRising(void)
{
    TileLayer *move = RSDK.GetTileLayer(Zone->moveLayer);

    foreach_active(Player, player)
    {
        player->collisionLayers |= Zone->moveLayerMask;
        player->moveLayerPosition.x = move->scrollInfo[0].scrollPos;
        player->moveLayerPosition.y = move->scrollPos;
    }
}

#if GAME_INCLUDE_EDITOR
void RisingLava_EditorDraw(void)
{
    RSDK_THIS(RisingLava);

    DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x, self->size.y, 0xFFFF00);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->offset.x, self->offset.y, self->size.x, self->size.y, 0xFFFF00);

        DrawHelpers_DrawArrow(self->position.x, self->position.y, self->offset.x, self->offset.y, 0xFFFF00, INK_NONE, 0xFF);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void RisingLava_EditorLoad(void) {}
#endif

void RisingLava_Serialize(void)
{
    RSDK_EDITABLE_VAR(RisingLava, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(RisingLava, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(RisingLava, VAR_ENUM, limit);
}
