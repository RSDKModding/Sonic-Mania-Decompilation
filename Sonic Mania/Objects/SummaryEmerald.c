#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectSummaryEmerald *SummaryEmerald;

void SummaryEmerald_Update() {}

void SummaryEmerald_LateUpdate() {}

void SummaryEmerald_StaticUpdate() {}

void SummaryEmerald_Draw()
{
    RSDK_THIS(SummaryEmerald);
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void SummaryEmerald_Create(void *data)
{
    RSDK_THIS(SummaryEmerald);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = 3;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
    if (!RSDK_sceneInfo->inEditor) {
        if ((1 << entity->emeraldID) & SaveGame->saveRAM[28])
            RSDK.SetSpriteAnimation(SummaryEmerald->spriteIndex, 0, &entity->data, true, entity->emeraldID % 7);
        else
            RSDK.SetSpriteAnimation(SummaryEmerald->spriteIndex, 0, &entity->data, true, 7);
    }
}

void SummaryEmerald_StageLoad() { SummaryEmerald->spriteIndex = RSDK.LoadSpriteAnimation("Summary/SummaryEmerald.bin", SCOPE_STAGE); }

void SummaryEmerald_EditorDraw() {}

void SummaryEmerald_EditorLoad() {}

void SummaryEmerald_Serialize() { RSDK_EDITABLE_VAR(SummaryEmerald, VAR_ENUM, emeraldID); }
#endif
