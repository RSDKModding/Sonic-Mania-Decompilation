#include "SonicMania.h"

ObjectAnnouncer *Announcer;

void Announcer_Update(void)
{
    RSDK_THIS(Announcer);
    StateMachine_Run(self->state);
}

void Announcer_LateUpdate(void) {}

void Announcer_StaticUpdate(void) {}

void Announcer_Draw(void)
{
    RSDK_THIS(Announcer);
    StateMachine_Run(self->stateDraw);
}

void Announcer_Create(void *data)
{
    RSDK_THIS(Announcer);
    self->active        = ACTIVE_NORMAL;
    self->drawOrder     = 13;
    self->visible       = true;
    self->drawFX        = FX_FLIP | FX_SCALE;
    self->isPermanent   = true;
    self->updateRange.x = 0x800000;
    self->updateRange.y = 0x800000;
}

void Announcer_StageLoad(void)
{
    Announcer->finishedCountdown = false;
    if (RSDK.CheckStageFolder("Menu")) {
#if !RETRO_USE_PLUS
        Announcer->sfxNewRecordTop = RSDK.GetSFX("VO/NewRecordTop.wav");
        Announcer->sfxNewRecordMid = RSDK.GetSFX("VO/NewRecordMid.wav");
#endif
        Announcer->sfxSonic        = RSDK.GetSFX("VO/Sonic.wav");
        Announcer->sfxTails        = RSDK.GetSFX("VO/Tails.wav");
        Announcer->sfxKnuckles     = RSDK.GetSFX("VO/Knuckles.wav");
#if RETRO_USE_PLUS
        Announcer->sfxMighty       = RSDK.GetSFX("VO/Mighty.wav");
        Announcer->sfxRay          = RSDK.GetSFX("VO/Ray.wav");
#endif
        Announcer->sfxTheWinnerIs  = RSDK.GetSFX("VO/TheWinnerIs.wav");
        Announcer->sfxPlayer1      = RSDK.GetSFX("VO/Player1.wav");
        Announcer->sfxPlayer2      = RSDK.GetSFX("VO/Player2.wav");
#if RETRO_USE_PLUS
        Announcer->sfxPlayer3      = RSDK.GetSFX("VO/Player3.wav");
        Announcer->sfxPlayer4      = RSDK.GetSFX("VO/Player4.wav");
#endif
        Announcer->sfxSonicWins    = RSDK.GetSFX("VO/SonicWins.wav");
        Announcer->sfxTailsWins    = RSDK.GetSFX("VO/TailsWins.wav");
        Announcer->sfxKnuxWins     = RSDK.GetSFX("VO/KnuxWins.wav");
#if RETRO_USE_PLUS
        Announcer->sfxMightyWins   = RSDK.GetSFX("VO/MightyWins.wav");
        Announcer->sfxRayWins      = RSDK.GetSFX("VO/RayWins.wav");
        Announcer->sfxItsADraw     = RSDK.GetSFX("VO/ItsADraw.wav");
        Announcer->sfxItsADraw_Set = RSDK.GetSFX("VO/ItsADraw_Set.wav");
        LogHelpers_Print("sfxDrawRound = %d", Announcer->sfxItsADraw);
        LogHelpers_Print("sfxDrawSet = %d", Announcer->sfxItsADraw_Set);
#endif
    }
    else if (globals->gameMode == MODE_COMPETITION) {
        Announcer->aniFrames = RSDK.LoadSpriteAnimation("Global/Announcer.bin", SCOPE_STAGE);
        Announcer->sfxThree   = RSDK.GetSFX("VO/Three.wav");
        Announcer->sfxTwo     = RSDK.GetSFX("VO/Two.wav");
        Announcer->sfxOne     = RSDK.GetSFX("VO/One.wav");
        Announcer->sfxGo      = RSDK.GetSFX("VO/Go.wav");
        Announcer->sfxGoal    = RSDK.GetSFX("VO/Goal.wav");
    }
    else if (globals->gameMode == MODE_TIMEATTACK) {
        Announcer->aniFrames = RSDK.LoadSpriteAnimation("Global/Announcer.bin", SCOPE_STAGE);
#if RETRO_USE_PLUS
        Announcer->sfxNewRecordTop = RSDK.GetSFX("VO/NewRecordTop.wav");
        Announcer->sfxNewRecordMid = RSDK.GetSFX("VO/NewRecordMid.wav");
#endif
        Announcer->sfxGoal         = RSDK.GetSFX("VO/Goal.wav");
    }
}

void Announcer_StartCountdown(void)
{
    Announcer->finishedCountdown = false;
    EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
    announcer->state     = Announcer_State_CountDown;
    announcer->stateDraw = Announcer_Unknown3;
    announcer->playerID  = 3;
    announcer->scale.x   = 0x200;
    announcer->scale.y   = 0x200;
}
void Announcer_AnnounceGoal(int32 screen)
{
    EntityAnnouncer *announcer = CREATE_ENTITY(Announcer, NULL, 0, 0);
    announcer->state           = Announcer_Unknown6;
    announcer->stateDraw       = Announcer_Unknown4;
    announcer->screen          = screen;
    RSDK.SetSpriteAnimation(Announcer->aniFrames, 0, &announcer->animator, true, 0);
    RSDK.PlaySfx(Announcer->sfxGoal, false, 255);
}
void Announcer_Unknown3(void)
{
    Vector2 drawPos;
    RSDK_THIS(Announcer);
    drawPos.y = (ScreenInfo->centerY - 32) << 16;
    drawPos.x = ScreenInfo->centerX << 16;
    drawPos.x += self->pos.x;
    drawPos.y += self->pos.y;
    RSDK.DrawSprite(&self->animator, &drawPos, true);

    if (self->playerID > 0) {
        self->inkEffect = INK_NONE;

        int32 frame                         = 0;
        EntityCompetitionSession *session = (EntityCompetitionSession *)globals->competitionSession;
        switch (session->characterFlags[SceneInfo->currentScreenID]) {
            case ID_TAILS: frame = 1; break;
            case ID_KNUCKLES: frame = 2; break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: frame = 3; break;
            case ID_RAY: frame = 4; break;
#endif
            default: frame = 0; break;
        }
        RSDK.SetSpriteAnimation(Announcer->aniFrames, 2, &self->animator2, true, frame);

        drawPos.x = ScreenInfo->centerX << 16;
        drawPos.y = (ScreenInfo->centerY + 48) << 16;
        RSDK.DrawSprite(&self->animator2, &drawPos, true);
        self->inkEffect = INK_ALPHA;
    }
}
void Announcer_Unknown4(void)
{
    Vector2 drawPos;
    RSDK_THIS(Announcer);

    if (SceneInfo->currentScreenID == self->screen) {
        drawPos.x = ScreenInfo->centerX << 16;
        drawPos.y = (ScreenInfo->centerY - 32) << 16;
        drawPos.x += self->pos.x;
        drawPos.y += self->pos.y;
        RSDK.DrawSprite(&self->animator, &drawPos, true);
    }
}
void Announcer_State_CountDown(void)
{
    RSDK_THIS(Announcer);

    self->inkEffect = INK_ALPHA;
    if (self->playerID <= 0) {
        if (self->timer >= 60) {
            destroyEntity(self);
        }
        else {
            if (!self->timer) {
                RSDK.PlaySfx(Announcer->sfxGo, false, 255);
                RSDK.SetSpriteAnimation(Announcer->aniFrames, 1, &self->animator, true, 3);
            }

            int32 val = 0;
            if (self->timer - 15 > 0)
                val = (self->timer - 15) << 9;
            self->alpha = 512 - val / 45;
            self->timer++;
        }
    }
    else {
        if (self->timer >= 45) {
            self->timer = 0;
            self->playerID--;
            if (!self->playerID) {
                Announcer->finishedCountdown = true;
                SceneInfo->timeEnabled  = true;
            }
        }
        else {

            if (!self->timer) {
                switch (self->playerID) {
                    default: break;
                    case 1:
                        RSDK.PlaySfx(Announcer->sfxOne, false, 255);
                        RSDK.SetSpriteAnimation(Announcer->aniFrames, 1, &self->animator, true, 2);
                        break;
                    case 2:
                        RSDK.PlaySfx(Announcer->sfxTwo, false, 255);
                        RSDK.SetSpriteAnimation(Announcer->aniFrames, 1, &self->animator, true, 1);
                        break;
                    case 3:
                        RSDK.PlaySfx(Announcer->sfxThree, false, 255);
                        RSDK.SetSpriteAnimation(Announcer->aniFrames, 1, &self->animator, true, 0);
                        break;
                }
            }
            self->pos.x = 0;
            self->pos.y = 0;
            self->alpha = 512 - (self->timer << 9) / 45;
            self->timer++;
        }
    }
}
void Announcer_Unknown6(void)
{
    RSDK_THIS(Announcer);
    self->scale.x = 0x200;
    self->scale.y = 0x200;
    if (self->timer >= 16) {
        if (self->timer >= 76) {
            if (self->timer >= 92) {
                destroyEntity(self);
            }
            else {
                self->visible = true;
                MathHelpers_Lerp1(&self->pos, ((self->timer - 76) << 8) / 16, 0, 0, ScreenInfo->width << 16, 0);
                ++self->timer;
            }
        }
        else {
            ++self->timer;
            self->pos.x = 0;
            self->pos.y = 0;
        }
    }
    else {
        self->visible = true;
        int32 t           = 16 * self->timer;
        int32 val         = -0x10000 * ScreenInfo->width;
        if (t > 0) {
            if (t < 256)
                self->pos.x = val + t * (-val >> 8);
            else
                self->pos.x = 0;
            self->pos.y = 0;
            ++self->timer;
        }
        else {
            self->pos.x = val;
            self->pos.y = 0;
            ++self->timer;
        }
    }
}
void Announcer_State_AnnounceWinner(void)
{
    RSDK_THIS(Announcer);
    if (self->timer >= 150) {
        switch (self->playerID) {
            case 0: RSDK.PlaySfx(Announcer->sfxPlayer1, 0, 255); break;
            case 1: RSDK.PlaySfx(Announcer->sfxPlayer2, 0, 255); break;
#if RETRO_USE_PLUS
            case 2: RSDK.PlaySfx(Announcer->sfxPlayer3, 0, 255); break;
            case 3: RSDK.PlaySfx(Announcer->sfxPlayer4, 0, 255); break;
#endif
            default: break;
        }
        destroyEntity(self);
    }
    else {
        if (self->timer == 30)
            RSDK.PlaySfx(Announcer->sfxTheWinnerIs, 0, 255);
        ++self->timer;
    }
}
void Announcer_State_AnnounceDraw(void)
{
    RSDK_THIS(Announcer);
    if (self->timer < 30) {
        self->timer++;
    }
    else {
        if (!self->playerID) {
            RSDK.PlaySfx(Announcer->sfxItsADraw, 0, 255);
        }
        else if (self->playerID == 1) {
            RSDK.PlaySfx(Announcer->sfxItsADraw_Set, 0, 255);
        }
        destroyEntity(self);
    }
}
void Announcer_State_AnnounceWinPlayer(void)
{
    RSDK_THIS(Announcer);
    if (self->timer >= 30) {
        switch (self->playerID) {
            case ID_SONIC: RSDK.PlaySfx(Announcer->sfxSonicWins, 0, 255); break;
            case ID_TAILS: RSDK.PlaySfx(Announcer->sfxTailsWins, 0, 255); break;
            case ID_KNUCKLES: RSDK.PlaySfx(Announcer->sfxKnuxWins, 0, 255); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: RSDK.PlaySfx(Announcer->sfxMightyWins, 0, 255); break;
            case ID_RAY: RSDK.PlaySfx(Announcer->sfxRayWins, 0, 255); break;
#endif
            default: break;
        }
        destroyEntity(self);
    }
    else {
        self->timer++;
    }
}

#if RETRO_INCLUDE_EDITOR
void Announcer_EditorDraw(void) {}

void Announcer_EditorLoad(void) {}
#endif

void Announcer_Serialize(void) {}
