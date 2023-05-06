// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: Letterboard Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectLetterboard *Letterboard;

void Letterboard_Update(void)
{
    RSDK_THIS(Letterboard);

    StateMachine_Run(self->state);
}

void Letterboard_LateUpdate(void) {}

void Letterboard_StaticUpdate(void) {}

void Letterboard_Draw(void)
{
    RSDK_THIS(Letterboard);

    self->scale.x = abs(RSDK.Cos512(self->angle));
    RSDK.DrawSprite(RSDK.Cos512(self->angle) >= 0 ? &self->animatorBack : &self->animatorFront, NULL, false);
}

void Letterboard_Create(void *data)
{
    RSDK_THIS(Letterboard);

    if (!SceneInfo->inEditor) {
        RSDK.SetSpriteAnimation(Letterboard->aniFrames, 0, &self->animatorBack, true, 0);
        RSDK.SetSpriteAnimation(Letterboard->aniFrames, 1, &self->animatorFront, true, 0);

        if (self->controller) {
            self->state = Letterboard_State_Controller;
        }
        else if (self->letterID) {
            self->state                 = Letterboard_State_CheckPlayerSpin;
            self->animatorFront.frameID = self->letterID - 1;
        }

        self->scale.x       = 0x200;
        self->scale.y       = 0x200;
        self->active        = ACTIVE_BOUNDS;
        self->updateRange.x = 0x400000;
        self->updateRange.y = 0x400000;
        self->visible       = true;
        self->drawGroup     = Zone->objectDrawGroup[0];
    }
}

void Letterboard_StageLoad(void)
{
    Letterboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Letterboard.bin", SCOPE_STAGE);

    Letterboard->hitboxBoard.left   = -12;
    Letterboard->hitboxBoard.top    = -12;
    Letterboard->hitboxBoard.right  = 12;
    Letterboard->hitboxBoard.bottom = 12;

    Letterboard->sfxLetterTurn = RSDK.GetSfx("Stage/LetterTurn.wav");
    Letterboard->sfxWin        = RSDK.GetSfx("Stage/Win.wav");
}

void Letterboard_State_Controller(void)
{
    RSDK_THIS(Letterboard);

    self->active = ACTIVE_BOUNDS;

    bool32 revealedAll = true;
    int32 slot         = SceneInfo->entitySlot + 1;

    for (int32 i = 0; i < self->letterID; ++i) {
        EntityLetterboard *letterboard = RSDK_GET_ENTITY(slot + i, Letterboard);
        if (letterboard->state)
            revealedAll = false;

        if (letterboard->state == Letterboard_State_Spun)
            self->active = ACTIVE_NORMAL;
    }

    if (revealedAll) {
        RSDK.PlaySfx(Letterboard->sfxWin, false, 0xFF);

        self->active = ACTIVE_BOUNDS;
        self->state  = StateMachine_None;
        if (globals->gameMode != MODE_COMPETITION)
            Player_GiveScore(RSDK_GET_ENTITY(SLOT_PLAYER1, Player), 100 * self->letterID);
    }
}

void Letterboard_State_CheckPlayerSpin(void)
{
    RSDK_THIS(Letterboard);

    foreach_active(Player, player)
    {
        self->spinSpeed = (abs(player->velocity.x) + abs(player->velocity.y)) >> 14;
        if (self->spinSpeed > 0) {
            if (Player_CheckCollisionTouch(player, self, &Letterboard->hitboxBoard)) {
                self->drawFX = FX_SCALE;
                self->state  = Letterboard_State_Spun;
                if (self->spinSpeed > 16)
                    self->spinSpeed = 16;
                if (self->spinSpeed < 8)
                    self->spinSpeed = 8;

                self->timer = 2;
                int32 slot  = SceneInfo->entitySlot;

                EntityLetterboard *letterboard = self;
                while (slot >= 0) {
                    letterboard = RSDK_GET_ENTITY(slot--, Letterboard);
                    if (letterboard->state == Letterboard_State_Controller)
                        break;
                }

                letterboard->active = ACTIVE_NORMAL;
                self->active        = ACTIVE_NORMAL;
                RSDK.PlaySfx(Letterboard->sfxLetterTurn, false, 255);
                foreach_break;
            }
        }
    }
}

void Letterboard_State_Spun(void)
{
    RSDK_THIS(Letterboard);

    int32 prevAngle = self->angle;
    self->angle     = (self->angle + self->spinSpeed) & 0x1FF;
    if (self->angle >= 0x100 && prevAngle < 0x100 && --self->timer <= 0) {
        self->active = ACTIVE_BOUNDS;
        self->angle  = 0x100;
        self->state  = StateMachine_None;
    }
}

#if GAME_INCLUDE_EDITOR
void Letterboard_EditorDraw(void)
{
    RSDK_THIS(Letterboard);

    RSDK.SetSpriteAnimation(Letterboard->aniFrames, 0, &self->animatorBack, true, 0);
    RSDK.SetSpriteAnimation(Letterboard->aniFrames, 1, &self->animatorFront, true, 0);

    if (!self->controller && self->letterID) {
        self->animatorFront.frameID = self->letterID - 1;
        RSDK.DrawSprite(&self->animatorFront, NULL, false);
    }
    else {
        RSDK.DrawSprite(&self->animatorBack, NULL, false);
    }

    if (self->controller && showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 slot = SceneInfo->entitySlot + 1;
        for (int32 i = 0; i < self->letterID; ++i) {
            EntityLetterboard *letterboard = RSDK_GET_ENTITY(slot + i, Letterboard);

            if (letterboard) {
                DrawHelpers_DrawArrow(self->position.x, self->position.y, letterboard->position.x, letterboard->position.y, 0xFFFF00, INK_NONE, 0xFF);
            }
        }

        RSDK_DRAWING_OVERLAY(false);
    }
}

void Letterboard_EditorLoad(void) { Letterboard->aniFrames = RSDK.LoadSpriteAnimation("SPZ2/Letterboard.bin", SCOPE_STAGE); }
#endif

void Letterboard_Serialize(void)
{
    RSDK_EDITABLE_VAR(Letterboard, VAR_BOOL, controller);
    RSDK_EDITABLE_VAR(Letterboard, VAR_UINT8, letterID);
}
