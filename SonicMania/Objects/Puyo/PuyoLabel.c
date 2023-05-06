// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoLabel Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoLabel *PuyoLabel;

void PuyoLabel_Update(void) {}

void PuyoLabel_LateUpdate(void) {}

void PuyoLabel_StaticUpdate(void) {}

void PuyoLabel_Draw(void)
{
    RSDK_THIS(PuyoLabel);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PuyoLabel_Create(void *data)
{
    RSDK_THIS(PuyoLabel);

    self->active        = ACTIVE_NORMAL;
    self->drawGroup     = 10;
    self->visible       = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;

    RSDK.SetSpriteAnimation(PuyoLabel->aniFrames, self->listID, &self->animator, true, self->frame);
}

void PuyoLabel_StageLoad(void) { PuyoLabel->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }

#if GAME_INCLUDE_EDITOR
void PuyoLabel_EditorDraw(void)
{
    RSDK_THIS(PuyoLabel);

    RSDK.SetSpriteAnimation(PuyoLabel->aniFrames, self->listID, &self->animator, true, self->frame);

    PuyoLabel_Draw();
}

void PuyoLabel_EditorLoad(void) { PuyoLabel->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoUI.bin", SCOPE_STAGE); }
#endif

void PuyoLabel_Serialize(void)
{
    RSDK_EDITABLE_VAR(PuyoLabel, VAR_ENUM, listID);
    RSDK_EDITABLE_VAR(PuyoLabel, VAR_ENUM, frame);
}
