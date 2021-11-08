#include "SonicMania.h"

ObjectPuyoScore *PuyoScore;

void PuyoScore_Update(void)
{
    RSDK_THIS(PuyoScore);
    if (entity->flag)
        entity->visible = !(Zone->timer & 4);
    else
        entity->visible = 1;
}

void PuyoScore_LateUpdate(void) {}

void PuyoScore_StaticUpdate(void) {}

void PuyoScore_Draw(void)
{
    RSDK_THIS(PuyoScore);
    PuyoScore_DrawScore(entity->score);
}

void PuyoScore_Create(void *data)
{
    RSDK_THIS(PuyoScore);
    entity->active        = ACTIVE_NORMAL;
    entity->drawOrder     = 10;
    entity->visible       = true;
    entity->updateRange.x = 0x800000;
    entity->updateRange.y = 0x800000;
}

void PuyoScore_StageLoad(void) { PuyoScore->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }

void PuyoScore_DrawScore(int score)
{
    RSDK_THIS(PuyoScore);
    char buffer[8];
    Vector2 drawPos;

    if (entity->counter)
        sprintf(buffer, "%d", score);
    else
        sprintf(buffer, "%06d", score);
    drawPos = entity->position;
    for (int i = 0; i < 6; ++i) {
        if (!buffer[i])
            break;
        RSDK.SetSpriteAnimation(PuyoScore->aniFrames, (entity->counter != false) + 2, &entity->animator, true, (buffer[i] - '0'));
        RSDK.DrawSprite(&entity->animator, &drawPos, false);
        drawPos.x += 0x80000;
    }
}

#if RETRO_INCLUDE_EDITOR
void PuyoScore_EditorDraw(void)
{
    RSDK_THIS(PuyoScore);
    if (entity->counter)
        PuyoScore_DrawScore(entity->playerID);
    else
        PuyoScore_DrawScore(entity->playerID ? 67890 : 12345);
}

void PuyoScore_EditorLoad(void) { PuyoScore->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }
#endif

void PuyoScore_Serialize(void)
{
    RSDK_EDITABLE_VAR(PuyoScore, VAR_ENUM, playerID);
    RSDK_EDITABLE_VAR(PuyoScore, VAR_BOOL, counter);
}
