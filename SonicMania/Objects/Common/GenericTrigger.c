// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: GenericTrigger Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectGenericTrigger *GenericTrigger;

void GenericTrigger_Update(void)
{
    RSDK_THIS(GenericTrigger);

    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, self, &self->hitbox) && !player->sidekick) {
            GenericTrigger->playerID = player->playerID;
            StateMachine_Run(GenericTrigger->callbacks[self->triggerID]);
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
        self->triggerID &= 0xF;

        self->updateRange.x = self->size.x;
        self->updateRange.y = self->size.y;
        self->active        = ACTIVE_BOUNDS;

        self->hitbox.left   = -(self->size.x >> 16);
        self->hitbox.right  = (self->size.x >> 16);
        self->hitbox.top    = -(self->size.y >> 16);
        self->hitbox.bottom = (self->size.y >> 16);
    }
}

void GenericTrigger_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void GenericTrigger_EditorDraw(void)
{
    RSDK_THIS(GenericTrigger);

    self->updateRange.x = self->size.x;
    self->updateRange.y = self->size.y;

    RSDK.SetSpriteAnimation(GenericTrigger->aniFrames, 0, &self->animator, true, 7);
    RSDK.DrawSprite(&self->animator, NULL, false);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        DrawHelpers_DrawRectOutline(self->position.x, self->position.y, self->size.x << 1, self->size.y << 1, 0xFFFF00);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void GenericTrigger_EditorLoad(void) { GenericTrigger->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void GenericTrigger_Serialize(void)
{
    RSDK_EDITABLE_VAR(GenericTrigger, VAR_VECTOR2, size);
    RSDK_EDITABLE_VAR(GenericTrigger, VAR_UINT8, triggerID);
}
