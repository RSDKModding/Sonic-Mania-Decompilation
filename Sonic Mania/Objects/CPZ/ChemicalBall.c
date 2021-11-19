#include "SonicMania.h"

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
    RSDK.DrawSprite(&self->animator, &self->startPos, false);
}

void ChemicalBall_Create(void *data)
{
    RSDK_THIS(ChemicalBall);

    self->visible       = true;
    self->drawOrder     = Zone->drawOrderLow;
    self->startPos2.x   = self->position.x;
    self->startPos2.y   = self->position.y;
    self->active        = ACTIVE_BOUNDS;
    self->updateRange.x = 0x1000000;
    self->updateRange.y = 0x1000000;
    if (self->interval == 0)
        self->interval = -128;
    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    if (self->direction == FLIP_NONE)
        self->position.x += 0x320000;
    else
        self->position.x -= 0x320000;

    if (self->type == 1)
        self->position.y -= 0x480000;
    RSDK.SetSpriteAnimation(ChemicalBall->aniFrames, 0, &self->animator, true, 0);
    if (!self->type)
        self->state = ChemicalBall_MoveType0;
    else
        self->state = ChemicalBall_MoveType1;
}

void ChemicalBall_StageLoad(void)
{
    ChemicalBall->aniFrames     = RSDK.LoadSpriteAnimation("CPZ/ChemicalBall.bin", SCOPE_STAGE);
    ChemicalBall->hitbox.left   = -8;
    ChemicalBall->hitbox.top    = -8;
    ChemicalBall->hitbox.right  = 8;
    ChemicalBall->hitbox.bottom = 8;
    ChemicalBall->sfxBloop      = RSDK.GetSFX("Stage/Bloop.wav");
}

void ChemicalBall_CheckHit(void)
{
    RSDK_THIS(ChemicalBall);
    foreach_active(Player, player)
    {
        if (Player_CheckCollisionTouch(player, &self->startPos, &ChemicalBall->hitbox)) {
            Player_CheckHit(player, self);
        }
    }
}

void ChemicalBall_MoveType0(void)
{
    RSDK_THIS(ChemicalBall);

    int32 timer = (Zone->timer + self->intervalOffset) % (2 * self->interval);
    if (timer < self->interval == (self->direction != FLIP_NONE)) {
        if (!self->direction)
            timer -= self->interval;
        if (timer >= 96) {
            self->startPos.x = ChemicalBall->moveOffset1[0];
            self->startPos.y = ChemicalBall->moveOffset1[1];
        }
        else {
            self->startPos.x = ChemicalBall->moveOffset1[2 * (97 - timer)];
            self->startPos.y = ChemicalBall->moveOffset1[2 * (97 - timer) + 1];
        }
    }
    else {
        if (self->direction == FLIP_X)
            timer -= self->interval;
        if (timer >= 97) {
            self->startPos.x = ChemicalBall->moveOffset1[194];
            self->startPos.y = ChemicalBall->moveOffset1[195];
        }
        else {
            self->startPos.x = ChemicalBall->moveOffset1[2 * timer];
            self->startPos.y = ChemicalBall->moveOffset1[2 * timer + 1];
        }
    }

    self->startPos.x += self->position.x;
    self->startPos.y += self->position.y;
    if (!timer || timer == self->interval)
        RSDK.PlaySfx(ChemicalBall->sfxBloop, 0, 255);
    ChemicalBall_CheckHit();
}

void ChemicalBall_MoveType1(void)
{
    RSDK_THIS(ChemicalBall);
    int32 timer = (Zone->timer + self->intervalOffset) % self->interval;
    if (timer < 98) {
        if (timer >= 49) {
            if (!self->direction)
                self->startPos.x = -0x320000;
            else
                self->startPos.x = 0x2E0000;
            self->startPos.y = ChemicalBall->moveOffset2[97 - timer];
        }
        else {
            if (!self->direction)
                self->startPos.x = 0x2E0000;
            else
                self->startPos.x = -0x320000;
            self->startPos.y = ChemicalBall->moveOffset2[timer];
        }
    }
    self->startPos.x += self->position.x;
    self->startPos.y += self->position.y;
    if (timer == 27 || timer == 48)
        RSDK.PlaySfx(ChemicalBall->sfxBloop, 0, 255);
    ChemicalBall_CheckHit();
}

#if RETRO_INCLUDE_EDITOR
void ChemicalBall_EditorDraw(void)
{
    RSDK_THIS(ChemicalBall);

    self->startPos2.x   = self->position.x;
    self->startPos2.y   = self->position.y;
    self->startPos.x = self->position.x;
    self->startPos.y = self->position.y;
    if (self->direction == FLIP_NONE)
        self->startPos.x += 0x320000;
    else
        self->startPos.x -= 0x320000;

    if (self->type == 1)
        self->startPos.y -= 0x480000;

    RSDK.SetSpriteAnimation(ChemicalBall->aniFrames, 0, &self->animator, true, 0);

    ChemicalBall_Draw();
}

void ChemicalBall_EditorLoad(void) { ChemicalBall->aniFrames = RSDK.LoadSpriteAnimation("CPZ/ChemicalBall.bin", SCOPE_STAGE); }
#endif

void ChemicalBall_Serialize(void)
{
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, type);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, direction);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, interval);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_UINT8, intervalOffset);
    RSDK_EDITABLE_VAR(ChemicalBall, VAR_BOOL, master);
}
