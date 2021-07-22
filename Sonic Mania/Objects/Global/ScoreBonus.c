#include "SonicMania.h"

ObjectScoreBonus *ScoreBonus;

void ScoreBonus_Update(void)
{
    EntityScoreBonus* entity = (EntityScoreBonus *)RSDK_sceneInfo->entity;
    entity->position.y -= 0x20000;
    entity->timer--;
    if (!entity->timer)
        RSDK.ResetEntityPtr(entity, 0, 0);
}

void ScoreBonus_LateUpdate(void)
{

}

void ScoreBonus_StaticUpdate(void)
{

}

void ScoreBonus_Draw(void)
{
    EntityScoreBonus *entity = (EntityScoreBonus *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void ScoreBonus_Create(void* data)
{
    EntityScoreBonus *entity = (EntityScoreBonus *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(ScoreBonus->spriteIndex, 0, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
        entity->timer     = 24;
    }
}

void ScoreBonus_StageLoad(void) { ScoreBonus->spriteIndex = RSDK.LoadSpriteAnimation("Global/ScoreBonus.bin", SCOPE_STAGE); }

void ScoreBonus_EditorDraw(void)
{

}

void ScoreBonus_EditorLoad(void)
{

}

void ScoreBonus_Serialize(void)
{

}

