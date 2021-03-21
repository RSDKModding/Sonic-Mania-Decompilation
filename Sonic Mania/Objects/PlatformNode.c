#include "../SonicMania.h"

ObjectPlatformNode *PlatformNode;

void PlatformNode_Update() {}

void PlatformNode_LateUpdate() {}

void PlatformNode_StaticUpdate() {}

void PlatformNode_Draw() {}

void PlatformNode_Create(void *data) {}

void PlatformNode_StageLoad() {}

void PlatformNode_EditorDraw() {}

void PlatformNode_EditorLoad()
{
    // Dont have any code to prove this is what it was like, but its a 50/50 chance
    // PlatformMode->spriteIndex = RSDK.LoadSpriteAnimation("some path to a bin", SCOPE_STAGE);

    // EntityPlatformNode *entity = NULL;
    // while (RSDK.GetEntities(PlatformNode->objectID, &entity)) {
    //    RSDK.SetSpriteAnimation(PlatformNode->spriteIndex, 0, &entity->data, true, 0);
    //}
}

void PlatformNode_Serialize() { RSDK_EDITABLE_VAR(PlatformNode, VAR_ENUM, nodeFlag); }
