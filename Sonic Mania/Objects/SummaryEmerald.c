#include "../SonicMania.h"

#if RETRO_USE_PLUS
ObjectSummaryEmerald *SummaryEmerald;

void SummaryEmerald_Update(void) {}

void SummaryEmerald_LateUpdate(void) {}

void SummaryEmerald_StaticUpdate(void) {}

void SummaryEmerald_Draw(void)
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

void SummaryEmerald_StageLoad(void) { SummaryEmerald->spriteIndex = RSDK.LoadSpriteAnimation("Summary/SummaryEmerald.bin", SCOPE_STAGE); }

void SummaryEmerald_EditorDraw(void) {}

void SummaryEmerald_EditorLoad(void) {}

void SummaryEmerald_Serialize(void) { RSDK_EDITABLE_VAR(SummaryEmerald, VAR_ENUM, emeraldID); }
#endif
