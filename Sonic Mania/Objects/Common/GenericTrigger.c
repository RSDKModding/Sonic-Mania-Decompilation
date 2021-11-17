#include "SonicMania.h"

ObjectGenericTrigger *GenericTrigger;

void GenericTrigger_Update(void)
{
    RSDK_THIS(GenericTrigger);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, entity, &entity->hitbox) && !player->sidekick) {
            GenericTrigger->playerID = player->playerID;
            if (GenericTrigger->callbacks[entity->triggerID])
                GenericTrigger->callbacks[entity->triggerID]();
        }
    }
}

void GenericTrigger_LateUpdate(void) {}

void GenericTrigger_StaticUpdate(void) {}

void GenericTrigger_Draw(void) {}

void GenericTrigger_Create(void *data)
{
    RSDK_THIS(GenericTrigger);
    if (!SceneInfo->inEditor) {
        entity->triggerID &= 0xF;
        entity->updateRange.x = entity->size.x;
        entity->updateRange.y = entity->size.y;
        entity->active        = ACTIVE_BOUNDS;
        entity->hitbox.left   = -(entity->size.x >> 0x10);
        entity->hitbox.right  = (entity->size.x >> 0x10);
        entity->hitbox.top    = -(entity->size.y >> 0x10);
        entity->hitbox.bottom = (entity->size.y >> 0x10);
    }
}

void GenericTrigger_StageLoad(void) {}

void GenericTrigger_EditorDraw(void)
{
    RSDK_THIS(GenericTrigger);
    entity->updateRange.x = entity->size.x;
    entity->updateRange.y = entity->size.y;

    RSDK.SetSpriteAnimation(GenericTrigger->aniFrames, 0, &entity->animator, true, 7);
    RSDK.DrawSprite(&entity->animator, NULL, false);

    DrawHelpers_DrawRectOutline(0xFFFF00, entity->position.x, entity->position.y, entity->size.x, entity->size.y);
}

void GenericTrigger_EditorLoad(void) { GenericTrigger->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }

void GenericTrigger_Serialize(void)
{
    RSDK_EDITABLE_VAR(GenericTrigger, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(GenericTrigger, VAR_UINT8, triggerID);
}
