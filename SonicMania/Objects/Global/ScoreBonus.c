// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ScoreBonus Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectScoreBonus *ScoreBonus;

void ScoreBonus_Update(void)
{
    RSDK_THIS(ScoreBonus);

    self->position.y -= TO_FIXED(2);

    if (!--self->timer)
        destroyEntity(self);
}

void ScoreBonus_LateUpdate(void) {}

void ScoreBonus_StaticUpdate(void) {}

void ScoreBonus_Draw(void)
{
    RSDK_THIS(ScoreBonus);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void ScoreBonus_Create(void *data)
{
    RSDK_THIS(ScoreBonus);

    RSDK.SetSpriteAnimation(ScoreBonus->aniFrames, 0, &self->animator, true, 0);
    if (!SceneInfo->inEditor) {
        self->active    = ACTIVE_NORMAL;
        self->visible   = true;
        self->drawGroup = Zone->objectDrawGroup[0];
        self->timer     = 24;
    }
}

void ScoreBonus_StageLoad(void) { ScoreBonus->aniFrames = RSDK.LoadSpriteAnimation("Global/ScoreBonus.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void ScoreBonus_EditorDraw(void) { ScoreBonus_Draw(); }

void ScoreBonus_EditorLoad(void) { ScoreBonus->aniFrames = RSDK.LoadSpriteAnimation("Global/ScoreBonus.bin", SCOPE_STAGE); }
#endif

void ScoreBonus_Serialize(void) {}
