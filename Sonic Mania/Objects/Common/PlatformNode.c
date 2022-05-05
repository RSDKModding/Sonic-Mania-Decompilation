// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PlatformNode Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectPlatformNode *PlatformNode;

void PlatformNode_Update(void) {}

void PlatformNode_LateUpdate(void) {}

void PlatformNode_StaticUpdate(void) {}

void PlatformNode_Draw(void) {}

void PlatformNode_Create(void *data) {}

void PlatformNode_StageLoad(void) {}

#if RETRO_INCLUDE_EDITOR
void PlatformNode_EditorDraw(void)
{
    RSDK_THIS(PlatformNode);

    RSDK.SetSpriteAnimation(PlatformNode->aniFrames, 0, &self->animator, false, 7);
    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PlatformNode_EditorLoad(void) { PlatformNode->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE); }
#endif

void PlatformNode_Serialize(void) { RSDK_EDITABLE_VAR(PlatformNode, VAR_ENUM, nodeFlag); }
