#include "../SonicMania.h"

ObjectUIMedallionPanel *UIMedallionPanel;

void UIMedallionPanel_Update(void) {}

void UIMedallionPanel_LateUpdate(void) {}

void UIMedallionPanel_StaticUpdate(void) {}

void UIMedallionPanel_Draw(void)
{
#if RETRO_USE_PLUS
    if (!User.GetUserStorageNoSave())
#else
    if (!globals->noSave)
#endif
        UIMedallionPanel_DrawPanel();
}

void UIMedallionPanel_Create(void *data)
{
    EntityUIMedallionPanel *entity = (EntityUIMedallionPanel *)RSDK_sceneInfo->entity;
    entity->active                 = ACTIVE_BOUNDS;
    entity->drawOrder              = 2;
    entity->visible                = 1;
    entity->drawFX                 = FX_FLIP;
    entity->updateRange.x          = 0x800000;
    entity->updateRange.y          = 0x800000;
}

void UIMedallionPanel_StageLoad(void) { UIMedallionPanel->spriteIndex = RSDK.LoadSpriteAnimation("UI/MedallionPanel.bin", SCOPE_STAGE); }

void UIMedallionPanel_DrawPanel(void)
{
    EntityUIMedallionPanel *entity = (EntityUIMedallionPanel *)RSDK_sceneInfo->entity;
    Vector2 drawPos;

    int *savePtr = NULL;
    if (!RSDK_sceneInfo->inEditor) {
        if (globals->saveLoaded == STATUS_OK) {
#if RETRO_USE_PLUS
            if (!User.GetUserStorageNoSave())
#else
            if (globals->noSave)
#endif
            savePtr = &globals->saveRAM[0x900];
        }
    }

    for (int m = 0; m < 0x20; ++m) {
        drawPos.x = (entity->position.x - 0x310000) + 0xE0000 * (m % 8);
        drawPos.y = (entity->position.y - 0x150000) + 0xE0000 * (m / 8);
        if (RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->data, true, (m & 1) + 1);
        }
        else if (savePtr) {
            bool32 flag = false;
            if (m >= savePtr[71]) {
                if (m < savePtr[72])
                    flag = true;
                RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->data, true, flag);
            }
            else {
                RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->data, true, 2);
            }
        }
        else {
            RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->data, true, 0);
        }
        if (!RSDK_sceneInfo->inEditor && !entity->data.frameID)
            RSDK.DrawCircleOutline(drawPos.x, drawPos.y, 5, 6, 0, 255, INK_BLEND, 0);
        drawPos.x += 0x10000;
        RSDK.DrawSprite(&entity->data, &drawPos, 0);
    }
}

void UIMedallionPanel_EditorDraw(void) {}

void UIMedallionPanel_EditorLoad(void) {}

void UIMedallionPanel_Serialize(void) {}
