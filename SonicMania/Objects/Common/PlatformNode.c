// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlatformNode Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPlatformNode *PlatformNode;

void PlatformNode_Update(void) {}

void PlatformNode_LateUpdate(void) {}

void PlatformNode_StaticUpdate(void) {}

void PlatformNode_Draw(void) {}

void PlatformNode_Create(void *data) {}

void PlatformNode_StageLoad(void) {}

#if GAME_INCLUDE_EDITOR
void PlatformNode_EditorDraw(void)
{
    RSDK_THIS(PlatformNode);

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);
        EntityPlatformNode *next = RSDK_GET_ENTITY(SceneInfo->entitySlot + 1, PlatformNode);
        if (next && next->classID == PlatformNode->classID) {
            RSDK.DrawLine(self->position.x, self->position.y, next->position.x, next->position.y, 0xFFFF00, 0xFF, INK_NONE, false);
        }
        RSDK_DRAWING_OVERLAY(false);
    }

    RSDK.SetSpriteAnimation(PlatformNode->aniFrames, 0, &self->animator, false, 0);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PlatformNode_EditorLoad(void) { PlatformNode->aniFrames = RSDK.LoadSpriteAnimation("Global/PlaneSwitch.bin", SCOPE_STAGE); }
#endif

void PlatformNode_Serialize(void) { RSDK_EDITABLE_VAR(PlatformNode, VAR_ENUM, nodeFlag); }
