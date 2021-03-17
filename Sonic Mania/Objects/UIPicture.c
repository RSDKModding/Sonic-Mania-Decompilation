#include "../SonicMania.h"

ObjectUIPicture *UIPicture;

void UIPicture_Update()
{
    EntityUIPicture *entity = (EntityUIPicture *)RSDK_sceneInfo->entity;
    RSDK.ProcessAnimation(&entity->data);
}

void UIPicture_LateUpdate()
{

}

void UIPicture_StaticUpdate()
{

}

void UIPicture_Draw()
{
    EntityUIPicture *entity = (EntityUIPicture *)RSDK_sceneInfo->entity;
    if (entity->zonePalette)
        RSDK.CopyPalette((entity->zonePalette >> 3) + 1, 32 * entity->zonePalette, 0, 224, 32);
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void UIPicture_Create(void* data)
{
    EntityUIPicture *entity = (EntityUIPicture *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(UIPicture->spriteIndex, entity->listID, &entity->data, true, entity->frameID);
    if (!RSDK_sceneInfo->inEditor) {
        if (RSDK.CheckStageFolder("Menu")) {
            entity->active  = ACTIVE_BOUNDS;
            entity->visible   = 1;
            entity->drawOrder = 2;
        }
        else {
            if (RSDK.CheckStageFolder("Logos") || RSDK.CheckStageFolder("LSelect") || RSDK.CheckStageFolder("Summary"))
                entity->active = ACTIVE_NORMAL;
            entity->visible   = true;
            entity->drawOrder = 2;
        }
    }
}

void UIPicture_StageLoad()
{
    if (RSDK.CheckStageFolder("Menu")) {
        UIPicture->spriteIndex = RSDK.LoadSpriteAnimation("UI/Picture.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Logos")) {
        UIPicture->spriteIndex = RSDK.LoadSpriteAnimation("Logos/Logos.bin", SCOPE_STAGE);
    }
}

void UIPicture_EditorDraw()
{

}

void UIPicture_EditorLoad()
{

}

void UIPicture_Serialize()
{
    RSDK_EDITABLE_VAR(UIPicture, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIPicture, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UIPicture, VAR_ENUM, tag);
    RSDK_EDITABLE_VAR(UIPicture, VAR_BOOL, zonePalette);
    RSDK_EDITABLE_VAR(UIPicture, VAR_UINT8, zoneID);
}

