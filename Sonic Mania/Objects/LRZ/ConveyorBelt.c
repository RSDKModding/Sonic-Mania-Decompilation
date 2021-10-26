#include "SonicMania.h"

ObjectConveyorBelt *ConveyorBelt;

void ConveyorBelt_Update(void)
{
    RSDK_THIS(ConveyorBelt);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox)) {
            if (player->onGround)
                player->position.x += entity->speed << 14;
        }
    }
}

void ConveyorBelt_LateUpdate(void) {}

void ConveyorBelt_StaticUpdate(void) {}

void ConveyorBelt_Draw(void) {}

void ConveyorBelt_Create(void *data)
{
    RSDK_THIS(ConveyorBelt);
    entity->active        = ACTIVE_BOUNDS;
    entity->visible       = false;
    entity->updateRange.x = 0x400000;
    entity->updateRange.y = 0x400000;
    entity->drawOrder     = Zone->drawOrderHigh;
    if (RSDK_sceneInfo->inEditor) {
        if (!entity->speed)
            entity->speed = -8;
        if (!entity->size.x)
            entity->size.x = 0xC00000;
        if (!entity->size.y)
            entity->size.y = 0x200000;
    }

    entity->hitbox.left   = (-entity->size.x >> 17) - 16;
    entity->hitbox.right  = (entity->size.x >> 17) + 16;
    entity->hitbox.bottom = (entity->size.y >> 17) + 16;
    entity->hitbox.top    = -(entity->size.y >> 17) - 16;
    RSDK.SetSpriteAnimation(ConveyorBelt->aniFrames, 0, &entity->animator, true, 0);
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
