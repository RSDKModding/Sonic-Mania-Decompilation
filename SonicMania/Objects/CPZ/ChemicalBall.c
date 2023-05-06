// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ChemicalBall Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectChemicalBall *ChemicalBall;

void ChemicalBall_Update(void)
{
    RSDK_THIS(ChemicalBall);

    if (globals->gameMode == MODE_TIMEATTACK && TimeAttackGate && TimeAttackGate->isFinished)
        destroyEntity(self);

    StateMachine_Run(self->state);
}

void ChemicalBall_LateUpdate(void) {}

void ChemicalBall_StaticUpdate(void) {}

void ChemicalBall_Draw(void)
{
    RSDK_THIS(ChemicalBall);

    RSDK.DrawSprite(&self->animator, &self->drawPos, false);
}

void ChemicalBall_Create(void *data)
{
    RSDK_THIS(ChemicalBall);

    self->visible       = true;
    self->drawGroup     = Zone->objectDrawGroup[0];
    self->startPos      = self->position;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;

    if (!self->interval)
        self->interval = 128;

    self->drawPos = self->position;

    if (self->direction == FLIP_NONE)
        self->position.x += 0x320000;
    else
        self->position.x -= 0x320000;

    if (self->type == CHEMICALBALL_VERTICAL)
        self->position.y -= 0x480000;

    RSDK.SetSpriteAnimation(ChemicalBall->aniFrames, 0, &self->animator, true, 0);

    if (!self->type)
        self->state = ChemicalBall_State_MoveArc;
    else
        self->state = ChemicalBall_State_MoveVertical;
}

void ChemicalBall_StageLoad(void)
{
    ChemicalBall->aniFrames = RSDK.LoadSpriteAnimation("CPZ/ChemicalBall.bin", SCOPE_STAGE);

    ChemicalBall->hitboxBall.left   = -8;
    ChemicalBall->hitboxBall.top    = -8;
    ChemicalBall->hitboxBall.right  = 8;
    ChemicalBall->hitboxBall.bottom = 8;

    ChemicalBall->sfxBloop = RSDK.GetSfx("Stage/Bloop.wav");
}

void ChemicalBall_CheckHit(void)
{
    RSDK_THIS(ChemicalBall);

    foreach_active(Player, player)
    {
        Vector2 posStore = self->position;
        self->position   = self->drawPos;
        if (Player_CheckCollisionTouch(player, self, &ChemicalBall->hitboxBall)) {
            self->position = posStore;
            Player_Hurt(player, self);
        }
        else {
            self->position = posStore;
        }
    }
}

void ChemicalBall_State_MoveArc(void)
{
    RSDK_THIS(ChemicalBall);

    int32 timer = (Zone->timer + self->intervalOffset) % (2 * self->interval);
    if (timer < self->interval == (self->direction != FLIP_NONE)) {
        if (!self->direction)
            timer -= self->interval;

        if (timer >= 96) {
            self->drawPos.x = ChemicalBall->arcOffsets[0];
            self->drawPos.y = ChemicalBall->arcOffsets[1];
        }
        else {
            self->drawPos.x = ChemicalBall->arcOffsets[2 * (97 - timer)];
            self->drawPos.y = ChemicalBall->arcOffsets[2 * (97 - timer) + 1];
        }
    }
    else {
        if (self->direction == FLIP_X)
            timer -= self->interval;

        if (timer >= 97) {
            self->drawPos.x = ChemicalBall->arcOffsets[194];
            self->drawPos.y = ChemicalBall->arcOffsets[195];
        }
        else {
            self->drawPos.x = ChemicalBall->arcOffsets[2 * timer];
            self->drawPos.y = ChemicalBall->arcOffsets[2 * timer + 1];
        }
    }

    self->drawPos.x += self->position.x;
    self->drawPos.y += self->position.y;

    if (!timer || timer == self->interval)
        RSDK.PlaySfx(ChemicalBall->sfxBloop, false, 255);

    ChemicalBall_CheckHit();
}

void ChemicalBall_State_MoveVertical(void)
{
    RSDK_THIS(ChemicalBall);

    int32 timer = (Zone->timer + self->intervalOffset) % self->interval;
    if (timer < 98) {
        if (timer >= 49) {
            if (!self->direction)
                self->drawPos.x = -0x320000;
            else
                self->drawPos.x = 0x2E0000;
            self->drawPos.y = ChemicalBall->moveOffsets[97 - timer];
        }
        else {
            if (!self->direction)
                self->drawPos.x = 0x2E0000;
            else
                self->drawPos.x = -0x320000;
            self->drawPos.y = ChemicalBall->moveOffsets[timer];
        }
    }
    self->drawPos.x += self->position.x;
    self->drawPos.y += self->position.y;

    if (timer == 27 || timer == 48)
        RSDK.PlaySfx(ChemicalBall->sfxBloop, false, 255);

    ChemicalBall_CheckHit();
}

#if GAME_INCLUDE_EDITOR
void ChemicalBall_EditorDraw(void)
{
    RSDK_THIS(ChemicalBall);

    RSDK.SetSpriteAnimation(ChemicalBall->aniFrames, 0, &self->animator, true, 0);

    self->drawPos = self->position;
    if (self->direction == FLIP_NONE)
        self->drawPos.x += 0x320000;
    else
        self->drawPos.x -= 0x320000;

    if (self->type == CHEMICALBALL_VERTICAL)
        self->drawPos.y -= 0x480000;

    ChemicalBall_Draw();

    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        self->drawPos   = self->position;
        self->inkEffect = INK_BLEND;
        ChemicalBall_Draw();
        self->inkEffect = INK_NONE;

        RSDK_DRAWING_OVERLAY(false);
    }
}

void ChemicalBall_EditorLoad(void)
{
    ChemicalBall->aniFrames = RSDK.LoadSpriteAnimation("CPZ/ChemicalBall.bin", SCOPE_STAGE);

    RSDK_ACTIVE_VAR(ChemicalBall, type);
    RSDK_ENUM_VAR("Arc", CHEMICALBALL_ARC);
    RSDK_ENUM_VAR("Vertical", CHEMICALBALL_VERTICAL);

    RSDK_ACTIVE_VAR(ChemicalBall, direction);
    RSDK_ENUM_VAR("No Flip", FLIP_NONE);
    RSDK_ENUM_VAR("Flipped", FLIP_X);
}
#endif

void ChemicalBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_BOOL, master); // unused, (looks to be set on the first instance of arc types)
}
