#include "SonicMania.h"

ObjectScoreBonus *ScoreBonus;

void ScoreBonus_Update(void)
{
    RSDK_THIS(ScoreBonus);

    entity->position.y -= 0x20000;
    if (!--entity->timer)
        destroyEntity(entity);
}

void ScoreBonus_LateUpdate(void) {}

void ScoreBonus_StaticUpdate(void) {}

void ScoreBonus_Draw(void)
{
    RSDK_THIS(ScoreBonus);
    RSDK.DrawSprite(&entity->animator, NULL, false);
}

void ScoreBonus_Create(void *data)
{
    RSDK_THIS(ScoreBonus);
    RSDK.SetSpriteAnimation(ScoreBonus->aniFrames, 0, &entity->animator, true, 0);
    if (!RSDK_sceneInfo->inEditor) {
        entity->active    = ACTIVE_NORMAL;
        entity->visible   = true;
        entity->drawOrder = Zone->drawOrderLow;
        entity->timer     = 24;
    }
}

void ScoreBonus_StageLoad(void) { ScoreBonus->aniFrames = RSDK.LoadSpriteAnimation("Global/ScoreBonus.bin", SCOPE_STAGE); }

#if RETRO_INCLUDE_EDITOR
void ScoreBonus_EditorDraw(void) { ScoreBonus_Draw(); }

void ScoreBonus_EditorLoad(void) { ScoreBonus->aniFrames = RSDK.LoadSpriteAnimation("Global/ScoreBonus.bin", SCOPE_STAGE); }
#endif

void ScoreBonus_Serialize(void) {}
