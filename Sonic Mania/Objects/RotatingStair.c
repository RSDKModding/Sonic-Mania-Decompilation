#include "../SonicMania.h"

ObjectRotatingStair *RotatingStair;

void RotatingStair_Update(void) { Platform_Update(); }

void RotatingStair_LateUpdate(void) {}

void RotatingStair_StaticUpdate(void) {}

void RotatingStair_Draw(void) {}

void RotatingStair_Create(void *data) {}

void RotatingStair_StageLoad(void) { RotatingStair->val = 0; }

void RotatingStair_EditorDraw(void) {}

void RotatingStair_EditorLoad(void) {}

void RotatingStair_Serialize(void)
{
    // manual override, it's usually "type"
    RSDK.SetEditableVar(VAR_ENUM, "mode", RotatingStair->objectID, offsetof(EntityRotatingStair, type)); 
    RSDK_EDITABLE_VAR(RotatingStair, VAR_VECTOR2, amplitude);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_ENUM, childCount);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(RotatingStair, VAR_UINT16, oscOff);
}
