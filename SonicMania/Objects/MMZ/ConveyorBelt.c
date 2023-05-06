// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ConveyorBelt Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    self->drawGroup     = Zone->objectDrawGroup[1];

    if (SceneInfo->inEditor) {
        if (!self->speed)
            self->speed = -8;

        if (!self->size.x)
            self->size.x = 0xC00000;

        if (!self->size.y)
            self->size.y = 0x200000;
    }

    self->hitbox.left   = -(self->size.x >> 17) - 16;
    self->hitbox.top    = -(self->size.y >> 17) - 16;
    self->hitbox.right  = (self->size.x >> 17) + 16;
    self->hitbox.bottom = (self->size.y >> 17) + 16;

    RSDK.SetSpriteAnimation(ConveyorBelt->aniFrames, 0, &self->animator, true, 0);
}

void ConveyorBelt_StageLoad(void)
{
    if (RSDK.CheckSceneFolder("MMZ"))
        ConveyorBelt->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE);
}

#if GAME_INCLUDE_EDITOR
void ConveyorBelt_EditorDraw(void)
{
    RSDK_THIS(ConveyorBelt);

    ConveyorBelt_Create(NULL);

    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &self->hitbox, FLIP_NONE, 0xFFFF00);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void ConveyorBelt_EditorLoad(void) { ConveyorBelt->aniFrames = RSDK.LoadSpriteAnimation("MMZ/ConveyorWheel.bin", SCOPE_STAGE); }
#endif

void ConveyorBelt_Serialize(void)
{
    RSDK_EDITABLE_VAR(ConveyorBelt, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(ConveyorBelt, VAR_INT32, speed);
}
