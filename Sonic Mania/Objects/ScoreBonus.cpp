#include "../SonicMania.hpp"

ObjectScoreBonus *ScoreBonus;

void ScoreBonus_Update()
{
    EntityScoreBonus* entity = (EntityScoreBonus *)RSDK_sceneInfo->entity;
    entity->position.y -= 0x20000;
    entity->timer--;
    if (!entity->timer)
        RSDK.DestroyEntity(entity, 0, 0);
}

void ScoreBonus_LateUpdate()
{

}

void ScoreBonus_StaticUpdate()
{

}

void ScoreBonus_Draw()
{
    EntityScoreBonus *entity = (EntityScoreBonus *)RSDK_sceneInfo->entity;
    RSDK.DrawSprite(&entity->data, NULL, false);
}

void ScoreBonus_Create(void* data)
{
    EntityScoreBonus *entity = (EntityScoreBonus *)RSDK_sceneInfo->entity;
    RSDK.SetSpriteAnimation(ScoreBonus->spriteIndex, 0, &entity->data, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = 1;
        entity->drawOrder = Zone->drawOrderLow;
        entity->timer     = 24;
    }
}

void ScoreBonus_StageLoad()
{

}

void ScoreBonus_EditorDraw()
{

}

void ScoreBonus_EditorLoad()
{

}

void ScoreBonus_Serialize()
{

}
