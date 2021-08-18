#include "SonicMania.h"

ObjectUIMedallionPanel *UIMedallionPanel;

void UIMedallionPanel_Update(void) {}

void UIMedallionPanel_LateUpdate(void) {}

void UIMedallionPanel_StaticUpdate(void) {}

void UIMedallionPanel_Draw(void)
{
    if (!checkNoSave)
        UIMedallionPanel_DrawPanel();
}

void UIMedallionPanel_Create(void *data)
{
    RSDK_THIS(UIMedallionPanel);
    entity->active                 = ACTIVE_BOUNDS;
    entity->drawOrder              = 2;
    entity->visible                = true;
    entity->drawFX                 = FX_FLIP;
    entity->updateRange.x          = 0x800000;
    entity->updateRange.y          = 0x800000;
}

void UIMedallionPanel_StageLoad(void) { UIMedallionPanel->spriteIndex = RSDK.LoadSpriteAnimation("UI/MedallionPanel.bin", SCOPE_STAGE); }

void UIMedallionPanel_DrawPanel(void)
{
    RSDK_THIS(UIMedallionPanel);
    Vector2 drawPos;

    EntityGameProgress *progress = GameProgress_GetGameProgress();
    for (int m = 0; m < 0x20; ++m) {
        drawPos.x = (entity->position.x - 0x310000) + 0xE0000 * (m % 8);
        drawPos.y = (entity->position.y - 0x150000) + 0xE0000 * (m / 8);
        if (RSDK_sceneInfo->inEditor) {
            RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->animator, true, (m & 1) + 1);
        }
        else if (progress) {
            RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->animator, true, progress->medals[m]);
        }
        else {
            RSDK.SetSpriteAnimation(UIMedallionPanel->spriteIndex, 0, &entity->animator, true, 0);
        }

        if (!RSDK_sceneInfo->inEditor && !entity->animator.frameID)
            RSDK.DrawCircleOutline(drawPos.x, drawPos.y, 5, 6, 0, 255, INK_BLEND, false);
        drawPos.x += 0x10000;
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
    }
}

void UIMedallionPanel_EditorDraw(void) {}

void UIMedallionPanel_EditorLoad(void) {}

void UIMedallionPanel_Serialize(void) {}
