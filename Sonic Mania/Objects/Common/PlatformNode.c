#include "SonicMania.h"

ObjectPlatformNode *PlatformNode;

void PlatformNode_Update(void) {}

void PlatformNode_LateUpdate(void) {}

void PlatformNode_StaticUpdate(void) {}

void PlatformNode_Draw(void) {}

void PlatformNode_Create(void *data) {}

void PlatformNode_StageLoad(void) {}

void PlatformNode_EditorDraw(void)
{
    RSDK_THIS(PlatformNode);
    RSDK.SetSpriteAnimation(PlatformNode->aniFrames, 0, &entity->animator, false, 7);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void PlatformNode_EditorLoad(void)
{
    PlatformNode->aniFrames = RSDK.LoadSpriteAnimation("Editor/EditorIcons.bin", SCOPE_STAGE);
}

void PlatformNode_Serialize(void) { RSDK_EDITABLE_VAR(PlatformNode, VAR_ENUM, nodeFlag); }
