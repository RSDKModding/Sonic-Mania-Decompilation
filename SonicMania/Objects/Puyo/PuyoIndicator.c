// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoIndicator Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoIndicator *PuyoIndicator;

void PuyoIndicator_Update(void)
{
    RSDK_THIS(PuyoIndicator);

    self->visible = false;

    if (self->state) {
        StateMachine_Run(self->state);

        RSDK.ProcessAnimation(&self->animator);
    }
}

void PuyoIndicator_LateUpdate(void) {}

void PuyoIndicator_StaticUpdate(void) {}

void PuyoIndicator_Draw(void)
{
    RSDK_THIS(PuyoIndicator);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PuyoIndicator_Create(void *data)
{
    RSDK_THIS(PuyoIndicator);

    self->active    = ACTIVE_NORMAL;
    self->drawGroup = Zone->objectDrawGroup[1];
    self->startPos  = self->position;
    self->visible   = true;
    self->drawFX    = FX_SCALE | FX_FLIP;
    self->scale.x   = 0x200;
    self->scale.y   = 0x200;
}

void PuyoIndicator_StageLoad(void) { PuyoIndicator->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoIndicator.bin", SCOPE_STAGE); }

void PuyoIndicator_ShowWinner(void)
{
    RSDK_THIS(PuyoIndicator);

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->visible = true;

    RSDK.SetSpriteAnimation(PuyoIndicator->aniFrames, 0, &self->animator, false, 0);

    self->position = self->startPos;
    self->scale.x  = 0x200;
    self->scale.y  = 0x200;
    self->scale.x += (RSDK.Sin512(8 * Zone->timer) >> 3) + 0x20;
    self->scale.y += (RSDK.Sin512(8 * Zone->timer) >> 3) + 0x20;
}

void PuyoIndicator_ShowLoser(void)
{
    RSDK_THIS(PuyoIndicator);

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->visible = true;

    RSDK.SetSpriteAnimation(PuyoIndicator->aniFrames, 1, &self->animator, false, 0);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y + 0x20000;
    self->position.y += RSDK.Sin256(4 * Zone->timer) << 10;
}

void PuyoIndicator_ShowReady(void)
{
    RSDK_THIS(PuyoIndicator);

    self->scale.x = 0x200;
    self->scale.y = 0x200;
    self->visible = true;

    RSDK.SetSpriteAnimation(PuyoIndicator->aniFrames, 2, &self->animator, false, 0);

    self->position.x = self->startPos.x;
    self->position.y = self->startPos.y;
}

#if GAME_INCLUDE_EDITOR
void PuyoIndicator_EditorDraw(void)
{
    RSDK_THIS(PuyoIndicator);

    self->drawFX   = FX_FLIP;
    self->startPos = self->position;
    self->playerID ? PuyoIndicator_ShowLoser() : PuyoIndicator_ShowWinner();
    self->scale.x = 0x200;
    self->scale.y = 0x200;

    PuyoIndicator_Draw();

    self->position = self->startPos;
}

void PuyoIndicator_EditorLoad(void)
{
    PuyoIndicator->aniFrames = RSDK.LoadSpriteAnimation("Puyo/PuyoIndicator.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(PuyoIndicator, playerID);
    RSDK_ENUM_VAR("Player 1", PUYOGAME_PLAYER1);
    RSDK_ENUM_VAR("Player 2", PUYOGAME_PLAYER2);
}
#endif

void PuyoIndicator_Serialize(void) { RSDK_EDITABLE_VAR(PuyoIndicator, VAR_UINT8, playerID); }
