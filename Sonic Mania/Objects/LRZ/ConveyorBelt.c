// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ConveyorBelt Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectConveyorBelt *ConveyorBelt;

void ConveyorBelt_Update(void)
{
    RSDK_THIS(ConveyorBelt);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox)) {
            if (player->onGround)
                player->position.x += self->speed << 14;
        }
    }
}

void ConveyorBelt_LateUpdate(void) {}

void ConveyorBelt_StaticUpdate(void) {}

void ConveyorBelt_Draw(void) {}

void ConveyorBelt_Create(void *data)
{
    RSDK_THIS(ConveyorBelt);
    self->active        = ACTIVE_BOUNDS;
    self->visible       = false;
    self->updateRange.x = 0x400000;
    self->updateRange.y = 0x400000;
    self->drawOrder     = Zone->drawOrderHigh;
    if (SceneInfo->inEditor) {
        if (!self->speed)
            self->speed = -8;
        if (!self->size.x)
            self->size.x = 0xC00000;
        if (!self->size.y)
            self->size.y = 0x200000;
    }

    self->hitbox.left   = (-self->size.x >> 17) - 16;
    self->hitbox.right  = (self->size.x >> 17) + 16;
    self->hitbox.bottom = (self->size.y >> 17) + 16;
    self->hitbox.top    = -(self->size.y >> 17) - 16;
    RSDK.SetSpriteAnimation(ConveyorBelt->aniFrames, 0, &self->animator, true, 0);
}

void ConveyorBelt_StageLoad(void)
{
    if (RSDK.CheckStageFolder("MMZ"))
        ConveyorBelt->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE);
}

#if RETRO_INCLUDE_EDITOR
void ConveyorBelt_EditorDraw(void) {}

void ConveyorBelt_EditorLoad(void) {}
#endif

void ConveyorBelt_Serialize(void)
{
    RSDK_EDITABLE_VAR(ConveyorBelt, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(ConveyorBelt, VAR_INT32, speed);
}
