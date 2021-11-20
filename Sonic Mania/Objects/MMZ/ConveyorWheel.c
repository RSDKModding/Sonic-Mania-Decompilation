#include "SonicMania.h"

ObjectConveyorWheel *ConveyorWheel;

void ConveyorWheel_Update(void)
{
    RSDK_THIS(ConveyorWheel);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &ConveyorWheel->hitbox)) {
            if (player->state == Player_State_KnuxGlideDrop || player->state == Player_State_GlideSlide)
                player->state = Player_State_Ground;
            if (self->direction) {
                if (player->groundVel > -0x40000)
                    player->groundVel = -0x40000;
                if (player->groundVel < -0xF0000)
                    player->groundVel = -0xF0000;
            }
            else {
                if (player->groundVel < 0x40000)
                    player->groundVel = 0x40000;
                if (player->groundVel > 0xF0000)
                    player->groundVel = 0xF0000;
            }
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
    self->drawOrder     = Zone->drawOrderHigh;
    RSDK.SetSpriteAnimation(ConveyorWheel->aniFrames, 0, &self->animator, true, 0);
}

void ConveyorWheel_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        ConveyorWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE);
    ConveyorWheel->hitbox.left   = -49;
    ConveyorWheel->hitbox.top    = -49;
    ConveyorWheel->hitbox.right  = 49;
    ConveyorWheel->hitbox.bottom = 49;
}

void ConveyorWheel_EditorDraw(void)
{
    RSDK_THIS(ConveyorWheel);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void ConveyorWheel_EditorLoad(void) { ConveyorWheel->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE); }

void ConveyorWheel_Serialize(void) { RSDK_EDITABLE_VAR(ConveyorWheel, VAR_UINT8, direction); }
