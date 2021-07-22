#include "SonicMania.h"

ObjectUIPicture *UIPicture;

void UIPicture_Update(void)
{
    RSDK_THIS(UIPicture);
    RSDK.ProcessAnimation(&entity->animator);
}

void UIPicture_LateUpdate(void) {}

void UIPicture_StaticUpdate(void) {}

void UIPicture_Draw(void)
{
    RSDK_THIS(UIPicture);
    if (entity->zonePalette)
        RSDK.CopyPalette((entity->zonePalette >> 3) + 1, 32 * entity->zonePalette, 0, 224, 32);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void UIPicture_Create(void *data)
{
    RSDK_THIS(UIPicture);
    RSDK.SetSpriteAnimation(UIPicture->spriteIndex, entity->listID, &entity->animator, true, entity->frameID);
    if (!RSDK_sceneInfo->inEditor) {
        if (RSDK.CheckStageFolder("Menu")) {
            entity->active    = ACTIVE_BOUNDS;
            entity->visible   = true;
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

void UIPicture_StageLoad(void)
{
    if (RSDK.CheckStageFolder("Menu")) {
        UIPicture->spriteIndex = RSDK.LoadSpriteAnimation("UI/Picture.bin", SCOPE_STAGE);
    }
    else if (RSDK.CheckStageFolder("Logos")) {
        UIPicture->spriteIndex = RSDK.LoadSpriteAnimation("Logos/Logos.bin", SCOPE_STAGE);
    }
}

void UIPicture_EditorDraw(void) {}

void UIPicture_EditorLoad(void) {}

void UIPicture_Serialize(void)
{
    RSDK_EDITABLE_VAR(UIPicture, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(UIPicture, VAR_ENUM, frameID);
    RSDK_EDITABLE_VAR(UIPicture, VAR_ENUM, tag);
    RSDK_EDITABLE_VAR(UIPicture, VAR_BOOL, zonePalette);
    RSDK_EDITABLE_VAR(UIPicture, VAR_UINT8, zoneID);
}
