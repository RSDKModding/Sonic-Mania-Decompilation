// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: PuyoAttack Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectPuyoAttack *PuyoAttack;

void PuyoAttack_Update(void)
{
    RSDK_THIS(PuyoAttack);

    if (self->delay > 0) {
        if (--self->delay)
            return;

        self->visible = true;
    }

    RSDK.ProcessAnimation(&self->animator);

    int32 angle          = RSDK.ATan2((self->targetPos.x - self->position.x) >> 16, (self->targetPos.y - self->position.y) >> 16);
    int32 rot            = 2 * angle;
    int32 targetRotation = rot - self->rotation;

    if (abs(rot - self->rotation) >= abs(targetRotation - 0x200)) {
        if (abs(targetRotation - 0x200) < abs(targetRotation + 0x200))
            self->rotation += ((targetRotation - 0x200) >> 3);
        else
            self->rotation += ((targetRotation + 0x200) >> 3);
    }
    else {
        if (abs(rot - self->rotation) >= abs(targetRotation + 0x200))
            self->rotation += ((targetRotation + 0x200) >> 3);
        else
            self->rotation += (targetRotation >> 3);
    }

    self->rotation &= 0x1FF;
    self->position.x += self->radius * RSDK.Cos512(self->rotation);
    self->position.y += self->radius * RSDK.Sin512(self->rotation);

    if (self->radius < 1024)
        self->radius += 8;

    if (self->position.y < self->targetPos.y) {
        PuyoMatch_AddPuyoCombo(self->playerID, self->score);
        destroyEntity(self);
    }
}

void PuyoAttack_LateUpdate(void) {}

void PuyoAttack_StaticUpdate(void) {}

void PuyoAttack_Draw(void)
{
    RSDK_THIS(PuyoAttack);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void PuyoAttack_Create(void *data)
{
    RSDK_THIS(PuyoAttack);

    if (!SceneInfo->inEditor) {
        self->delay     = 16;
        self->radius    = 512;
        self->drawGroup = Zone->hudDrawGroup;

        self->playerID = VOID_TO_INT(data);
        self->rotation = self->playerID ? 0x140 : 0x40;
        self->active   = ACTIVE_NORMAL;
        RSDK.SetSpriteAnimation(PuyoAttack->aniFrames, self->playerID ^ 1, &self->animator, true, 0);
    }
}

void PuyoAttack_StageLoad(void)
{
    PuyoAttack->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE);

    PuyoAttack->sfxAttack = RSDK.GetSfx("Puyo/Attack.wav");
}

#if GAME_INCLUDE_EDITOR
void PuyoAttack_EditorDraw(void)
{
    RSDK_THIS(PuyoAttack);
    RSDK.SetSpriteAnimation(PuyoAttack->aniFrames, 0, &self->animator, true, 3);

    PuyoAttack_Draw();
}

void PuyoAttack_EditorLoad(void) { PuyoAttack->aniFrames = RSDK.LoadSpriteAnimation("Puyo/Combos.bin", SCOPE_STAGE); }
#endif

void PuyoAttack_Serialize(void) {}
