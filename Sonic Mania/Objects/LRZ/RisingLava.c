#include "SonicMania.h"

ObjectRisingLava *RisingLava;

void RisingLava_Update(void)
{
    RSDK_THIS(RisingLava);
    StateMachine_Run(entity->state);
}

void RisingLava_LateUpdate(void) {}

void RisingLava_StaticUpdate(void) {}

void RisingLava_Draw(void) {}

void RisingLava_Create(void *data)
{
    RSDK_THIS(RisingLava);
    if (!RSDK_sceneInfo->inEditor) {
        entity->hitbox.right  = entity->size.x >> 17;
        entity->hitbox.left   = -entity->hitbox.right;
        entity->hitbox.bottom = entity->size.y >> 17;
        entity->hitbox.top    = -entity->hitbox.bottom;

        entity->active        = ACTIVE_BOUNDS;
        entity->updateRange.x = 0x800000;
        entity->updateRange.y = 0x800000;
        entity->limit *= -0x10000;
        entity->state = RisingLava_State_CheckRiseStart;
    }
}

void RisingLava_StageLoad(void) { RisingLava->sfxRumble = RSDK.GetSFX("Stage/Rumble.wav"); }

void RisingLava_State_CheckRiseStart(void)
{
    RSDK_THIS(RisingLava);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && !player->sidekick) {
            TileLayer *move               = RSDK.GetSceneLayer(Zone->moveLayer);
            move->drawLayer[0]            = 5;
            move->scrollPos               = -entity->offset.y;
            move->scrollInfo[0].scrollPos = -entity->offset.x;
            entity->active                = ACTIVE_NORMAL;
            entity->state                 = RisingLava_State_RiseShake;
        }
    }
}

void RisingLava_State_RiseShake(void)
{
    RSDK_THIS(RisingLava);
    TileLayer *move = RSDK.GetSceneLayer(Zone->moveLayer);

    move->scrollPos += 0x8000;
    foreach_active(Player, player)
    {
        player->collisionLayers |= Zone->moveID;
        player->moveOffset.x = move->scrollInfo[0].scrollPos;
        player->moveOffset.y = move->scrollPos;
    }

    if (!(Zone->timer & 3))
        Camera_ShakeScreen(0, 0, 2);

    if (!(Zone->timer & 7))
        RSDK.PlaySfx(RisingLava->sfxRumble, false, 255);

    if (move->scrollPos > entity->limit)
        entity->state = RisingLava_State_StoppedRising;
}

void RisingLava_State_StoppedRising(void)
{
    RSDK_THIS(RisingLava);
    TileLayer *move = RSDK.GetSceneLayer(Zone->moveLayer);

    foreach_active(Player, player)
    {
        player->collisionLayers |= Zone->moveID;
        player->moveOffset.x = move->scrollInfo[0].scrollPos;
        player->moveOffset.y = move->scrollPos;
    }
}

#if RETRO_INCLUDE_EDITOR
void RisingLava_EditorDraw(void)
{
    RSDK_THIS(RisingLava);
    DrawHelpers_DrawRectOutline(0xFFFF00, entity->position.x, entity->position.y, entity->size.x, entity->size.y);
    DrawHelpers_DrawRectOutline(0xFFFF00, entity->offset.x, entity->offset.y, entity->size.x, entity->size.y);

    DrawHelpers_DrawArrow(0xFFFF00, entity->position.x, entity->position.y, entity->offset.x, entity->offset.y);
}

void RisingLava_EditorLoad(void) {}
#endif

void RisingLava_Serialize(void)
{
    RSDK_EDITABLE_VAR(RisingLava, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(RisingLava, VAR_VECTOR2, offset);
    RSDK_EDITABLE_VAR(RisingLava, VAR_ENUM, limit);
}
