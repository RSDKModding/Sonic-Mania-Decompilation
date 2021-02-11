#include "../SonicMania.hpp"

ObjectUIPicture *UIPicture;

void UIPicture_Update()
{

}

void UIPicture_LateUpdate()
{

}

void UIPicture_StaticUpdate()
{

}

void UIPicture_Draw()
{

}

void UIPicture_Create(void* data)
{

}

void UIPicture_StageLoad()
{

}

void UIPicture_EditorDraw()
{

}

void UIPicture_EditorLoad()
{

}

void UIPicture_Serialize()
{
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "listID", UIPicture->objectID, offsetof(EntityUIPicture, listID));
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "frameID", UIPicture->objectID, offsetof(EntityUIPicture, frameID));
    RSDK_SetEditableVar(ATTRIBUTE_VAR, "tag", UIPicture->objectID, offsetof(EntityUIPicture, tag));
    RSDK_SetEditableVar(ATTRIBUTE_BOOL, "zonePalette", UIPicture->objectID, offsetof(EntityUIPicture, zonePalette));
    RSDK_SetEditableVar(ATTRIBUTE_U8, "zoneID", UIPicture->objectID, offsetof(EntityUIPicture, zoneID));
}

