// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoScore Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoScore *PuyoScore;

void PuyoScore_Update(void)
{
    RSDK_THIS(PuyoScore);

    self->visible = self->flashing ? !(Zone->timer & 4) : true;
}

void PuyoScore_LateUpdate(void) {}

void PuyoScore_StaticUpdate(void) {}

void PuyoScore_Draw(void)
{
    RSDK_THIS(PuyoScore);

    PuyoScore_DrawScore(self->score);
}

void PuyoScore_Create(void *data)
{
    RSDK_THIS(PuyoScore);

    self->active        = ACTIVE_NORMAL;
    self->drawGroup     = 10;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
}

void PuyoScore_StageLoad(void) { PuyoScore->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }

void PuyoScore_DrawScore(int32 score)
{
    RSDK_THIS(PuyoScore);

    char buffer[8];
    if (self->counter)
        sprintf_s(buffer, (int32)sizeof(buffer), "%d", score);
    else
        sprintf_s(buffer, (int32)sizeof(buffer), "%06d", score);

    Vector2 drawPos = self->position;
    for (int32 i = 0; i < 6; ++i) {
        if (!buffer[i])
            break;

        RSDK.SetSpriteAnimation(PuyoScore->aniFrames, (self->counter != false) + 2, &self->animator, true, (buffer[i] - '0'));
        RSDK.DrawSprite(&self->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }
}

#if GAME_INCLUDE_EDITOR
void PuyoScore_EditorDraw(void)
{
    RSDK_THIS(PuyoScore);

    if (self->counter)
        PuyoScore_DrawScore(self->playerID);
    else
        PuyoScore_DrawScore(self->playerID ? 67890 : 12345);
}

void PuyoScore_EditorLoad(void)
{
    PuyoScore->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PuyoScore, playerID);
    RSDK_ENUM_VAR("Player 1", PUYOGAME_PLAYER1);
    RSDK_ENUM_VAR("Player 2", PUYOGAME_PLAYER2);
}
#endif

void PuyoScore_Serialize(void)
{
    RSDK_EDITABLE_VAR(PuyoScore, VAR_ENUM, playerID);
    RSDK_EDITABLE_VAR(PuyoScore, VAR_BOOL, counter);
}
