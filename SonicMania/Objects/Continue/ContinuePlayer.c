// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ContinuePlayer Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectContinuePlayer *ContinuePlayer;

void ContinuePlayer_Update(void)
{
    RSDK_THIS(ContinuePlayer);

    RSDK.ProcessAnimation(&self->animator);

    StateMachine_Run(self->state);
}

void ContinuePlayer_LateUpdate(void) {}

void ContinuePlayer_StaticUpdate(void) {}

void ContinuePlayer_Draw(void)
{
    RSDK_THIS(ContinuePlayer);

    RSDK.DrawSprite(&self->animator, NULL, false);
}

void ContinuePlayer_Create(void *data)
{
    RSDK_THIS(ContinuePlayer);

    if (!SceneInfo->inEditor) {
        self->visible   = true;
        self->drawGroup = 1;
        self->active    = ACTIVE_NORMAL;

        if (!self->isPlayer2) {
            switch (GET_CHARACTER_ID(1)) {
                default:
                case ID_SONIC:
                    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_SONIC, &self->animator, true, 0);

                    if (globals->playerID & ID_TAILS_ASSIST)
                        self->position.x -= 0x100000;
                    break;

                case ID_TAILS:
                    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_TAILS, &self->animator, true, 0);
                    self->position.y += 0x40000;
                    break;

                case ID_KNUCKLES: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_KNUX, &self->animator, true, 0); break;

#if MANIA_USE_PLUS
                case ID_MIGHTY: RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_MIGHTY, &self->animator, true, 0); break;

                case ID_RAY:
                    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_RAY, &self->animator, true, 0);
                    self->position.y += 0x40000;
                    break;
#endif
            }

            self->timer     = 60;
            self->aniFrames = ContinuePlayer->playerAniFrames;
        }
        else {
            RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, CONTPLR_ANI_IDLE_AI, &self->animator, true, 0);

            if (!(globals->playerID & ID_TAILS_ASSIST))
                self->active = ACTIVE_NEVER;

            self->timer     = 76;
            self->aniFrames = ContinuePlayer->tailAniFrames;
        }
    }
}

void ContinuePlayer_StageLoad(void)
{
    ContinuePlayer->aniFrames = RSDK.LoadSpriteAnimation("Players/Continue.bin", SCOPE_STAGE);

    if (globals->playerID == ID_NONE)
        globals->playerID = ID_DEFAULT_PLAYER;

    switch (GET_CHARACTER_ID(1)) {
        default:
        case ID_SONIC: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Sonic.bin", SCOPE_STAGE); break;
        case ID_TAILS: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE); break;
        case ID_KNUCKLES: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Knux.bin", SCOPE_STAGE); break;
#if MANIA_USE_PLUS
        case ID_MIGHTY: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Mighty.bin", SCOPE_STAGE); break;
        case ID_RAY: ContinuePlayer->playerAniFrames = RSDK.LoadSpriteAnimation("Players/Ray.bin", SCOPE_STAGE); break;
#endif
    }

    ContinuePlayer->tailAniFrames = RSDK.LoadSpriteAnimation("Players/Tails.bin", SCOPE_STAGE);

    ContinuePlayer->sfxRoll    = RSDK.GetSfx("Global/Roll.wav");
    ContinuePlayer->sfxCharge  = RSDK.GetSfx("Global/Charge.wav");
    ContinuePlayer->sfxRelease = RSDK.GetSfx("Global/Release.wav");
}

void ContinuePlayer_HandleDashAnim(void)
{
    RSDK_THIS(ContinuePlayer);

    if (self->groundVel >= 0x40000) {
        if (self->groundVel >= 0x60000) {
            if (self->groundVel >= 0xA0000) {
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_DASH, &self->animator, false, 0);
                self->animator.speed = 256;
            }
            else {
                RSDK.SetSpriteAnimation(self->aniFrames, ANI_RUN, &self->animator, false, 1);

                self->animator.speed = (self->groundVel >> 12) + 96;
                if (self->animator.speed > 0x100)
                    self->animator.speed = 0x100;
            }
        }
        else {
            RSDK.SetSpriteAnimation(self->aniFrames, ANI_JOG, &self->animator, false, 0);
            int32 vel            = (self->groundVel >> 16);
            self->animator.speed = 4 * ((vel << 1) + vel) + 64;
        }
    }
    else {
        RSDK.SetSpriteAnimation(self->aniFrames, ANI_WALK, &self->animator, false, 0);
        self->animator.speed = (self->groundVel >> 12) + 48;
    }
}

void ContinuePlayer_State_Idle(void)
{
    RSDK_THIS(ContinuePlayer);

    if (self->timer <= 0) {
        ContinuePlayer_HandleDashAnim();
        self->state = ContinuePlayer_State_ChargeDash;

        RSDK.PlaySfx(ContinuePlayer->sfxRoll, false, 255);
    }
    else {
        self->timer--;
    }
}

void ContinuePlayer_State_ChargeDash(void)
{
    RSDK_THIS(ContinuePlayer);

    if (self->groundVel >= 0xC0000) {
        RSDK.PlaySfx(ContinuePlayer->sfxRelease, false, 255);

        self->state = ContinuePlayer_State_DashRelease;
    }
    else {
        self->groundVel += 0x4000;
    }

    ContinuePlayer_HandleDashAnim();
}

void ContinuePlayer_State_DashRelease(void)
{
    RSDK_THIS(ContinuePlayer);

    self->position.x += self->groundVel;
}

#if GAME_INCLUDE_EDITOR
void ContinuePlayer_EditorDraw(void)
{
    RSDK_THIS(ContinuePlayer);

    RSDK.SetSpriteAnimation(ContinuePlayer->aniFrames, self->isPlayer2 ? CONTPLR_ANI_IDLE_SONIC : CONTPLR_ANI_IDLE_AI, &self->animator, true, 0);

    ContinuePlayer_Draw();
}

void ContinuePlayer_EditorLoad(void) { ContinuePlayer->aniFrames = RSDK.LoadSpriteAnimation("Players/Continue.bin", SCOPE_STAGE); }
#endif

void ContinuePlayer_Serialize(void) { RSDK_EDITABLE_VAR(ContinuePlayer, VAR_BOOL, isPlayer2); }
