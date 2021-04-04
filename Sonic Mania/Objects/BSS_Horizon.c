#include "../SonicMania.h"

ObjectBSS_Horizon *BSS_Horizon;

void BSS_Horizon_Update(void)
{

}

void BSS_Horizon_LateUpdate(void)
{

}

void BSS_Horizon_StaticUpdate(void)
{

}

void BSS_Horizon_Draw(void)
{
    Vector2 drawPos; 
    drawPos.y                   = 0;
    EntityBSS_Horizon *entity   = (EntityBSS_Horizon *)RSDK_sceneInfo->entity;

    drawPos.x                   = RSDK_screens->centerX << 16;
    entity->horizonData.frameID = 0;
    entity->alpha          = BSS_Palette->alpha1;
    entity->direction      = FLIP_NONE;
    RSDK.DrawSprite(&entity->horizonData, &drawPos, true);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->horizonData, &drawPos, true);

    entity->horizonData.frameID = 1;
    entity->alpha          = BSS_Palette->alpha2;
    entity->direction      = FLIP_NONE;
    RSDK.DrawSprite(&entity->horizonData, &drawPos, true);

    entity->direction = FLIP_X;
    RSDK.DrawSprite(&entity->horizonData, &drawPos, true);
}

void BSS_Horizon_Create(void* data)
{
    EntityBSS_Horizon *entity = (EntityBSS_Horizon *)RSDK_sceneInfo->entity;
    if (!RSDK_sceneInfo->inEditor) {
        entity->active               = ACTIVE_NORMAL;
        entity->inkEffect            = INK_ADD;
        entity->visible              = true;
        entity->drawOrder            = 2;
        entity->drawFX               = FX_FLIP;
        RSDK.SetSpriteAnimation(BSS_Horizon->spriteIndex, 0, &entity->horizonData, true, 0);
    }
}

void BSS_Horizon_StageLoad(void)
{
    BSS_Horizon->spriteIndex = RSDK.LoadSpriteAnimation("SpecialBS/Horizon.bin", SCOPE_STAGE);
    RSDK.ResetEntitySlot(SLOT_BSS_HORIZON, BSS_Horizon->objectID, 0);
}

void BSS_Horizon_EditorDraw(void)
{

}

void BSS_Horizon_EditorLoad(void)
{

}

void BSS_Horizon_Serialize(void)
{

}

