// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ConveyorWheel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectConveyorWheel *ConveyorWheel;

void ConveyorWheel_Update(void)
{
    RSDK_THIS(ConveyorWheel);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &ConveyorWheel->hitbox)) {
            if (player->state == Player_State_KnuxGlideDrop || player->state == Player_State_KnuxGlideSlide)
                player->state = Player_State_Ground;

            if (self->direction)
                player->groundVel = CLAMP(player->groundVel, -0xF0000, -0x40000);
            else
                player->groundVel = CLAMP(player->groundVel, 0x40000, 0xF0000);
        }
    }

    self->angle = (self->angle + 4) & 0x1FF;
}

void ConveyorWheel_LateUpdate(void) {}

void ConveyorWheel_StaticUpdate(void) {}

void ConveyorWheel_Draw(void) {}

void ConveyorWheel_Create(void *data)
{
    RSDK_THIS(ConveyorWheel);

    self->drawFX |= FX_FLIP;
    self->active        = ACTIVE_BOUNDS;
    self->visible       = true;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->drawGroup     = Zone->objectDrawGroup[1];

    RSDK.SetSpriteAnimation(ConveyorWheel->aniFrames, 0, &self->animator, true, 0);
}

void ConveyorWheel_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        ConveyorWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE);

    ConveyorWheel->hitbox.left   = -49;
    ConveyorWheel->hitbox.top    = -49;
    ConveyorWheel->hitbox.right  = 49;
    ConveyorWheel->hitbox.bottom = 49;
}

#if GAME_INCLUDE_EDITOR
void ConveyorWheel_EditorDraw(void)
{
    RSDK_THIS(ConveyorWheel);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &ConveyorWheel->hitbox, FLIP_NONE, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void ConveyorWheel_EditorLoad(void)
{
    ConveyorWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE);

    ConveyorWheel->hitbox.left   = -49;
    ConveyorWheel->hitbox.top    = -49;
    ConveyorWheel->hitbox.right  = 49;
    ConveyorWheel->hitbox.bottom = 49;

    RSDK_ACTIVE_VAR(ConveyorWheel, direction);
    RSDK_ENUM_VAR("Right", FLIP_NONE);
    RSDK_ENUM_VAR("Left", FLIP_X);
}
#endif

void ConveyorWheel_Serialize(void) { RSDK_EDITABLE_VAR(ConveyorWheel, VAR_UINT8, direction); }
