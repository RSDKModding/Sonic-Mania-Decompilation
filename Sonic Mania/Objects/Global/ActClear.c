// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ActClear Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled By Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "SonicMania.h"

ObjectActClear *ActClear;

void ActClear_Update(void) {}

void ActClear_LateUpdate(void)
{
    RSDK_THIS(ActClear);
    StateMachine_Run(self->state);
}

void ActClear_StaticUpdate(void) {}

void ActClear_Draw(void)
{
    Vector2 drawPos;
    Vector2 verts[4];
    int32 millisecsPtr = 0;
    int32 secsPtr      = 0;
    int32 minsPtr      = 0;
    int32 offset       = 0;
    RSDK_THIS(ActClear);

    drawPos.x  = self->gotThroughPos.x;
    drawPos.y  = self->gotThroughPos.y;
    verts[0].y = drawPos.y - 0x140000;
    verts[1].y = drawPos.y - 0x140000;
    verts[2].y = drawPos.y - 0x40000;
    verts[3].y = drawPos.y - 0x40000;
    if ((globals->playerID & 0xFF) == ID_KNUCKLES) {
        int32 center = (ScreenInfo->centerX << 16) + 0x100000;
        drawPos.x    = center + 2 * self->gotThroughPos.x;
        verts[0].x   = drawPos.x - 0x910000;
        verts[1].x   = drawPos.x + 0x340000;
        verts[2].x   = drawPos.x + 0x440000;
        verts[3].x   = drawPos.x - 0x810000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, INK_NONE);

        drawPos.x  = self->playerNamePos.x;
        drawPos.y  = self->playerNamePos.y;
        drawPos.x  = center + 2 * drawPos.x;
        verts[0].x = drawPos.x - 0x7C0000;
        verts[0].y = drawPos.y + 0x1C0000;
        verts[1].x = 0x500000 + drawPos.x;
        verts[2].x = 0x600000 + drawPos.x;
        verts[3].x = drawPos.x - 0x6C0000;
        verts[1].y = drawPos.y + 0x1C0000;
        verts[2].y = drawPos.y + 0x2C0000;
        verts[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, INK_NONE);

        drawPos.x = self->playerNamePos.x;
        drawPos.y = self->playerNamePos.y;
        drawPos.x += center;
        RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);
        self->gotThroughAnimator.frameID = 2;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);

        drawPos.x = self->gotThroughPos.x;
        drawPos.y = self->gotThroughPos.y;
        drawPos.x += center;
        self->gotThroughAnimator.frameID = 3;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);
        RSDK.DrawSprite(&self->actNumAnimator, &drawPos, true);

        offset = center - 0xA0000;
    }
    else {
        int32 center = (ScreenInfo->centerX << 16) + 0x100000;
        drawPos.x    = center + 2 * self->gotThroughPos.x;
        verts[0].x   = drawPos.x - 0x6D0000;
        verts[1].x   = drawPos.x + 0x340000;
        verts[2].x   = drawPos.x + 0x440000;
        verts[3].x   = drawPos.x - 0x5D0000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, INK_NONE);

        drawPos.x  = self->playerNamePos.x;
        drawPos.y  = self->playerNamePos.y;
        drawPos.x  = center + 2 * drawPos.x;
        verts[0].x = drawPos.x - 0x580000;
        verts[0].y = drawPos.y + 0x1C0000;
        verts[1].x = 0x500000 + drawPos.x;
        verts[2].x = 0x600000 + drawPos.x + 2;
        verts[3].x = drawPos.x - 0x480000;
        verts[1].y = drawPos.y + 0x1C0000;
        verts[2].y = drawPos.y + 0x2C0000;
        verts[3].y = drawPos.y + 0x2C0000;
        RSDK.DrawQuad(verts, 4, 0, 0, 0, 255, INK_NONE);

        drawPos.x = self->playerNamePos.x;
        drawPos.y = self->playerNamePos.y;
        drawPos.x += center;
        RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

#if RETRO_USE_PLUS
        if ((globals->playerID & 0xFF) == ID_MIGHTY)
            self->gotThroughAnimator.frameID = 2;
        else
#endif
            self->gotThroughAnimator.frameID = 0;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);
        drawPos.x = self->gotThroughPos.x;
        drawPos.y = self->gotThroughPos.y;
        drawPos.x += center;
        self->gotThroughAnimator.frameID = 1;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);
        RSDK.DrawSprite(&self->actNumAnimator, &drawPos, true);

        offset = center + 0x60000;
    }

    drawPos.x                 = offset + self->timeBonusPos.x - 0x5C0000;
    drawPos.y                 = self->timeBonusPos.y;
    self->hudElementsAnimator.frameID = 1;
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
    drawPos.x += 0x320000;
#if RETRO_USE_PLUS
    if (globals->gameMode != MODE_TIMEATTACK) {
#endif
        self->hudElementsAnimator.frameID = 8;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
#if RETRO_USE_PLUS
    }
#endif
    self->hudElementsAnimator.frameID = 10;
    drawPos.x += 0x340000;
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    int32 dx = 0x430000 + drawPos.x;
    int32 dy = drawPos.y + 0xE0000;
    drawPos.x += 0x430000;
    drawPos.y += 0xE0000;

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x = dx - 0x620000;
        drawPos.y = dy - 0xE0000;
        ActClear_DrawTime(&drawPos, SceneInfo->minutes, SceneInfo->seconds, SceneInfo->milliseconds);
    }
    else {
#endif
        ActClear_DrawNumbers(&drawPos, self->scoreBonus, 0);
#if RETRO_USE_PLUS
    }
#endif

    drawPos.x = offset + self->ringBonusPos.x - 0x5C0000;
    drawPos.y = self->ringBonusPos.y;
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK)
        self->hudElementsAnimator.frameID = 17;
    else
#endif
        self->hudElementsAnimator.frameID = 5;
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    dx = drawPos.x + 0x320000;
    drawPos.x += 0x320000;
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x                 = dx - 0xA0000;
        self->hudElementsAnimator.frameID = 1;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
        drawPos.x += 0xA0000;
    }
    else {
#endif
        self->hudElementsAnimator.frameID = 8;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
#if RETRO_USE_PLUS
    }
#endif

    self->hudElementsAnimator.frameID = 10;
    drawPos.x += 0x340000;
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    drawPos.x += 0x430000;
    drawPos.y += 0xE0000;
#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        TimeAttackData_GetTimeFromValue(self->time, &minsPtr, &secsPtr, &millisecsPtr);
        drawPos.x -= 0x620000;
        drawPos.y -= 0xE0000;
        if (!self->isNewRecord || (self->isNewRecord && (Zone->timer & 8)))
            ActClear_DrawTime(&drawPos, minsPtr, secsPtr, millisecsPtr);
    }
    else {
#endif
        ActClear_DrawNumbers(&drawPos, self->ringBonus, 0);
#if RETRO_USE_PLUS
    }
#endif

    if (self->showCoolBonus) {
        drawPos.x = self->coolBonusPos.x;
        drawPos.y = self->coolBonusPos.y;
        drawPos.x = offset + drawPos.x - 0x5C0000;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_TIMEATTACK)
            self->hudElementsAnimator.frameID = 18;
        else
#endif
            self->hudElementsAnimator.frameID = 15;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += 0x320000;
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_TIMEATTACK) {
#endif
            self->hudElementsAnimator.frameID = 8;
            RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
#if RETRO_USE_PLUS
        }
#endif

        self->hudElementsAnimator.frameID = 10;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += 0x430000;
        drawPos.y += 0xE0000;
#if RETRO_USE_PLUS
        if (globals->gameMode != MODE_TIMEATTACK) {
#endif
            ActClear_DrawNumbers(&drawPos, self->coolBonus, 0);
#if RETRO_USE_PLUS
        }
        else {
            if (!TimeAttackData->personalRank) {
                self->numbersAnimator.frameID = 16;
                RSDK.DrawSprite(&self->numbersAnimator, &drawPos, true);

                drawPos.x -= 0x90000;
            }
            else if (!self->achievedRank || (self->achievedRank && (Zone->timer & 8)))
                ActClear_DrawNumbers(&drawPos, TimeAttackData->personalRank, 0);
        }
#endif
    }

    drawPos.x = self->totalScorePos.x;
    drawPos.y = self->totalScorePos.y;

#if RETRO_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x                 = offset + self->totalScorePos.x - 0x5C0000;
        self->hudElementsAnimator.frameID = 19;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += 0x320000;
        self->hudElementsAnimator.frameID = 18;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        self->hudElementsAnimator.frameID = 10;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += 0x430000;
        drawPos.y += 0xE0000;
        if (!TimeAttackData->leaderboardRank) {
            self->numbersAnimator.frameID = 16;
            RSDK.DrawSprite(&self->numbersAnimator, &drawPos, true);
        }
        else {
            ActClear_DrawNumbers(&drawPos, TimeAttackData->leaderboardRank, 0);
        }
    }
    else {
#endif
        drawPos.x                 = offset + self->totalScorePos.x - 0x440000;
        self->hudElementsAnimator.frameID = 9;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        self->hudElementsAnimator.frameID = 10;
        drawPos.x += 0x340000;
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += 0x430000;
        drawPos.y += 0xE0000;
        ActClear_DrawNumbers(&drawPos, self->totalScore, 0);
#if RETRO_USE_PLUS
    }
#endif
}

void ActClear_Create(void *data)
{
    RSDK_THIS(ActClear);
    if (!SceneInfo->inEditor) {
#if RETRO_USE_PLUS
        ActClear->actClearActive = true;
#endif
        self->active           = ACTIVE_NORMAL;
        self->visible          = true;
        self->drawOrder        = Zone->hudDrawOrder;
        self->state            = ActClear_State_EnterText;
        self->stageFinishTimer = 0;
        self->newRecordTimer          = 0;
        EntityPlayer *player1    = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        self->playerPtr        = player1;

        if (Zone_GetZoneID() > ZONE_INVALID) {
            StatInfo stat;
            uint16 time = SceneInfo->milliseconds + 100 * (SceneInfo->seconds + 60 * SceneInfo->minutes);
#if RETRO_USE_PLUS
            switch (globals->playerID & 0xFF) {
                case ID_SONIC: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 1, player1->rings, player1->score, time); break;
                case ID_TAILS: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 2, player1->rings, player1->score, time); break;
                case ID_KNUCKLES: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 3, player1->rings, player1->score, time); break;
                case ID_MIGHTY: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 4, player1->rings, player1->score, time); break;
                default:
                    if ((globals->playerID & 0xFF) == ID_RAY)
                        TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 5, player1->rings, player1->score, time);
                    else
                        TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 1, player1->rings, player1->score, time);
                    break;
            }
            API.TryTrackStat(&stat);
#else
            switch (globals->playerID & 0xFF) {
                case ID_SONIC: APICallback_TrackActClear(Zone_GetZoneID(), Zone->actID, 1, player1->rings, player1->score, time); break;
                case ID_TAILS: APICallback_TrackActClear(Zone_GetZoneID(), Zone->actID, 2, player1->rings, player1->score, time); break;
                case ID_KNUCKLES: APICallback_TrackActClear(Zone_GetZoneID(), Zone->actID, 3, player1->rings, player1->score, time); break;
                default: break;
            }
#endif
        }

#if RETRO_USE_PLUS
        if (!ActClear->disableTimeBonus) {
#endif
            switch (SceneInfo->minutes) {
                case 0:
                    if (SceneInfo->seconds >= 30)
                        self->scoreBonus = SceneInfo->seconds < 45 ? 10000 : 5000;
                    else
                        self->scoreBonus = 50000;
                    break;
                case 1: self->scoreBonus = SceneInfo->seconds < 30 ? 4000 : 3000; break;
                case 2: self->scoreBonus = 2000; break;
                case 3: self->scoreBonus = 1000; break;
                case 4: self->scoreBonus = 500; break;
                case 5: self->scoreBonus = 100; break;
                case 9:
                    if (!SceneInfo->debugMode && globals->gameMode < MODE_TIMEATTACK && SceneInfo->seconds == 59) {
#if RETRO_USE_PLUS
                        if (globals->gameMode != MODE_ENCORE && !(globals->medalMods & getMod(MEDAL_NOTIMEOVER)))
#endif
                            self->scoreBonus = 100000;
                    }
                    break;
                default: break;
            }
#if RETRO_USE_PLUS
        }
#endif

        self->ringBonus      = 100 * player1->rings;
        self->coolBonus      = globals->coolBonus[0];
        globals->initCoolBonus = false;
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_TIMEATTACK) {
            EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
            self->time =
                TimeAttackData_GetScore(param->zoneID, param->characterID, param->actID, SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE), 1);
            self->achievedRank = false;
            self->isNewRecord  = false;
        }
#endif
        self->showCoolBonus      = true;
        self->playerNamePos.x  = 0xE00000;
        self->playerNamePos.y  = 0x580000;
        self->gotThroughPos.x = -0xE00000;
        self->gotThroughPos.y = 0x700000;
        self->timeBonusPos.x = 0x1E80000;
        self->timeBonusPos.y = 0x780000;
        self->ringBonusPos.x = 0x3080000;
        self->ringBonusPos.y = 0x880000;
        self->coolBonusPos.x = 0x4280000;
        self->coolBonusPos.y = 0x980000;
        self->totalScorePos.x = -0x5480000;
        self->totalScorePos.y = 0xC00000;
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 0, &self->hudElementsAnimator, true, 0);
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 1, &self->numbersAnimator, true, 0);
#if RETRO_USE_PLUS
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 0, &self->timeElementsAnimator, true, 12);
#endif
        switch (globals->playerID & 0xFF) {
            case ID_TAILS: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 1); break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 2); break;
#if RETRO_USE_PLUS
            case ID_MIGHTY: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 3); break;
            case ID_RAY: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 4); break;
#endif
            default: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 0); break;
        }
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 4, &self->gotThroughAnimator, true, 0);

        if (ActClear->actID <= 0)
            RSDK.SetSpriteAnimation(ActClear->aniFrames, 5, &self->actNumAnimator, true, Zone->actID);
        else
            RSDK.SetSpriteAnimation(ActClear->aniFrames, 5, &self->actNumAnimator, true, ActClear->actID - 1);
    }
}

void ActClear_StageLoad(void)
{
    ActClear->aniFrames     = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);
    ActClear->sfxScoreAdd   = RSDK.GetSfx("Global/ScoreAdd.wav");
    ActClear->sfxScoreTotal = RSDK.GetSfx("Global/ScoreTotal.wav");
#if RETRO_USE_PLUS
    ActClear->actClearActive = false;
    ActClear->sfxEvent       = RSDK.GetSfx("Special/Event.wav");
    ActClear->forceNoSave    = false;
#endif
}

#if RETRO_USE_PLUS
void ActClear_DrawTime(Vector2 *pos, int32 mins, int32 secs, int32 millisecs)
{
    Vector2 drawPos;
    RSDK_THIS(ActClear);
    if (!mins && !secs && !millisecs) {
        millisecs = -1;
        secs      = -1;
        mins      = -1;
    }

    drawPos.x = pos->x + 0x320000;
    drawPos.y = pos->y - 0x20000;
    RSDK.DrawSprite(&self->timeElementsAnimator, &drawPos, true);

    drawPos.x = 0x610000 + pos->x + 2;
    drawPos.y = pos->y + 0xE0000;
    ActClear_DrawNumbers(&drawPos, millisecs, 2);

    drawPos.x -= 0x90000;
    if (!mins && !secs && !millisecs)
        secs = -1;
    ActClear_DrawNumbers(&drawPos, secs, 2);

    drawPos.x -= 0x90000;
    if (!mins && !secs && !millisecs)
        mins = -1;
    ActClear_DrawNumbers(&drawPos, mins, 1);
}
#endif

void ActClear_DrawNumbers(Vector2 *pos, int32 value, int32 maxVals)
{
    RSDK_THIS(ActClear);
    int32 mult = 1;
    if (value >= 0) {
        if (!maxVals) {
            if (value <= 0) {
                maxVals = 1;
            }
            else {
                int32 i = value;
                do {
                    ++maxVals;
                    i /= 10;
                } while (i > 0);
            }
        }

        if (maxVals > 0) {
            int32 digit = 1;
            do {
                self->numbersAnimator.frameID = value / mult % 10;
                RSDK.DrawSprite(&self->numbersAnimator, pos, true);
                pos->x -= 0x90000;
                mult = 10 * digit;
                digit *= 10;
            } while (--maxVals);
        }
    }
    else if (maxVals > 0) {
        do {
            self->numbersAnimator.frameID = 16;
            RSDK.DrawSprite(&self->numbersAnimator, pos, true);
            pos->x -= 0x90000;
        } while (--maxVals);
    }
}
void ActClear_CheckPlayerVictory(void)
{
    foreach_active(Player, player)
    {
        if (player->state != Player_State_FlyIn && player->state != Player_State_JumpIn && player->state != Player_State_Victory
            && player->onGround) {
            player->state           = Player_State_Victory;
            player->nextAirState    = 0;
            player->nextGroundState = 0;
            if (Zone->forcePlayerOnScreenFlag)
                player->stateInput = StateMachine_None;
            RSDK.SetSpriteAnimation(player->aniFrames, ANI_VICTORY, &player->animator, true, 0);
        }
    }
}
void ActClear_SaveGameCallback(int32 success)
{
    UIWaitSpinner_FinishWait();
    ActClear->finishedSavingGame = false;
}

void ActClear_SetupForceOnScreenP2(void)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    if (player2) {
        if (player2->sidekick) {
            if (player2->state != Player_State_FlyIn && player2->state != Player_State_JumpIn) {
                if (player2->position.x <= (ScreenInfo->width + ScreenInfo->position.x) << 16
                    || abs(player2->position.y - player1->position.y) > 0x100000) {
                    Player->jumpInDelay    = 240;
                    Entity *entStore       = SceneInfo->entity;
                    SceneInfo->entity = (Entity *)player2;
                    Player_P2JumpBackIn();
                    SceneInfo->entity = entStore;
                    if (player2->state == Player_State_FlyIn || player2->state == Player_State_JumpIn || player2->state == Player_State_StartJumpIn) {
                        player2->active     = ACTIVE_NORMAL;
                        player2->position.y = ((ScreenInfo->position.y - 16) << 16);
                    }
                }
                else {
                    Player->jumpInDelay = -3600;
                }
            }
        }
    }
}

void ActClear_State_EnterText(void)
{
    RSDK_THIS(ActClear);

    if (self->playerNamePos.x > 0)
        self->playerNamePos.x -= 0x100000;

    if (self->gotThroughPos.x < 0)
        self->gotThroughPos.x += 0x100000;
    if (!self->timer && Zone->forcePlayerOnScreenFlag)
        ActClear_SetupForceOnScreenP2();

    if (++self->timer == 48) {
        self->timer = 0;
        self->state = ActClear_State_AdjustText;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_State_AdjustText(void)
{
    RSDK_THIS(ActClear);

    self->playerNamePos.y -= 0x8000;
    self->gotThroughPos.y -= 0x8000;
    if (++self->timer == 48) {
        self->timer = 0;
        self->state = ActClear_State_EnterBonuses;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_State_EnterBonuses(void)
{
    RSDK_THIS(ActClear);

    if (self->timeBonusPos.x > 0)
        self->timeBonusPos.x -= 0x100000;

    if (self->ringBonusPos.x > 0)
        self->ringBonusPos.x -= 0x100000;

    if (self->coolBonusPos.x > 0)
        self->coolBonusPos.x -= 0x100000;

    if (self->totalScorePos.x >= -0x80000) {
#if RETRO_USE_PLUS
        if (globals->gameMode == MODE_TIMEATTACK) {
            if (ActClear->isTimeAttack) {
                StateMachine_Run(ActClear->bufferMove_CB);
            }
            HUD->showTAPrompt        = true;
            ActClear->hasSavedReplay = false;
            self->newRecordTimer          = 240;
            self->state            = ActClear_State_ShowResultsTA;
            RSDK.SetScene("Presentation", "Menu");
        }
        else {
            self->state = ActClear_State_ScoreShownDelay;
        }
#else
        self->state = ActClear_State_ScoreShownDelay;
#endif
    }
    else {
        self->totalScorePos.x += 0x100000;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_State_ScoreShownDelay(void)
{
    RSDK_THIS(ActClear);
    if (++self->timer == 120) {
        self->timer = 0;
        self->state = ActClear_State_TallyScore;
    }
    ActClear_CheckPlayerVictory();
}

void ActClear_State_TallyScore(void)
{
    RSDK_THIS(ActClear);
    EntityPlayer *player = (EntityPlayer *)self->playerPtr;

    if (self->scoreBonus > 0) {
        self->totalScore += 100;
        self->scoreBonus -= 100;
        Player_GiveScore(player, 100);
    }

    if (self->ringBonus > 0) {
        self->totalScore += 100;
        self->ringBonus -= 100;
        Player_GiveScore(player, 100);
    }

    if (self->coolBonus > 0) {
        self->totalScore += 100;
        self->coolBonus -= 100;
        Player_GiveScore(player, 100);
    }

#if RETRO_USE_TOUCH_CONTROLS
    if (ControllerInfo[player->controllerID].keyA.press || ControllerInfo[player->controllerID].keyStart.press || TouchInfo->count) {
#else
    if (ControllerInfo[player->controllerID].keyA.press || ControllerInfo[player->controllerID].keyStart.press) {
#endif
        Player_GiveScore(player, self->scoreBonus + self->ringBonus + self->coolBonus);
        self->totalScore += self->scoreBonus + self->ringBonus + self->coolBonus;
        self->scoreBonus = 0;
        self->ringBonus  = 0;
        self->coolBonus  = 0;
    }

    if (self->scoreBonus + self->ringBonus + self->coolBonus <= 0) {
        self->timer = 0;
        self->state = ActClear_State_SaveGameProgress;
        RSDK.PlaySfx(ActClear->sfxScoreTotal, false, 255);
    }
    else if (++self->timer == 2) {
        self->timer = 0;
        RSDK.PlaySfx(ActClear->sfxScoreAdd, false, 255);
    }
    Music->nextTrack = -1;
    ActClear_CheckPlayerVictory();
}

void ActClear_State_SaveGameProgress(void)
{
    RSDK_THIS(ActClear);
    if (++self->timer == 120) {
        self->timer          = 0;
        globals->specialRingID = 0;
        if (ActClear->actID <= 0) {
            if (globals->gameMode == MODE_COMPETITION) {
                RSDK.SetScene("Presentation", "Menu");
            }
#if !RETRO_USE_PLUS
            else if (globals->gameMode == MODE_TIMEATTACK) {
                EntityMenuParam *param       = (EntityMenuParam *)globals->menuParam;
                ActClear->finishedSavingGame = true;
                uint8 characterID            = param->characterID;
                uint8 zoneID                 = param->zoneID;
                uint8 act                    = param->actID;

                int32 time = 6000 * SceneInfo->minutes + 100 * SceneInfo->seconds + SceneInfo->milliseconds;

                uint16 *record = TimeAttackData_GetRecordedTime(zoneID, act, characterID, 1);
                int32 rank     = 0;
                for (; rank < 3; ++rank) {
                    if (!record[rank] || time < record[rank])
                        break;
                }

                if (rank < 3) {
                    rank++;
                    TimeAttackData_SaveTATime(zoneID, act, characterID, rank, time);
                    APICallback_TrackTAClear(zoneID, act, characterID, time);
                    param->timeScore = rank;
                    SaveGame_SaveFile(ActClear_SaveGameCallback);
                }
                else {
                    ActClear->finishedSavingGame = false;
                }

                RSDK.SetScene("Presentation", "Menu");
            }
#endif
            else {
                globals->enableIntro = true;
                Player_SaveValues();
                SaveGame_ClearRestartData();
                StarPost_ResetStarPosts();
                if (Zone->actID > 0)
                    SaveGame->saveRAM->collectedSpecialRings = 0;
                SaveGame_SaveProgress();
#if RETRO_USE_PLUS
                if (globals->saveSlotID != NO_SAVE_SLOT && !ActClear->forceNoSave) {
#else
                if (globals->gameMode == MODE_MANIA) {
#endif
                    if (Zone_IsAct2())
                        GameProgress_MarkZoneCompleted(Zone_GetZoneID());
                    ActClear->finishedSavingGame = true;
                    SaveGame_SaveFile(ActClear_SaveGameCallback);
                }
                ++SceneInfo->listPos;
                if (!RSDK.CheckValidScene())
                    RSDK.SetScene("Presentation", "Title Screen");
            }
        }
        else {
            Player_SaveValues();
            SaveGame_ClearRestartData();
            StarPost_ResetStarPosts();
            SaveGame_SaveProgress();
#if RETRO_USE_PLUS
            if (globals->saveSlotID != NO_SAVE_SLOT && !ActClear->forceNoSave) {
#else
            if (globals->gameMode == MODE_MANIA) {
#endif
                ActClear->finishedSavingGame = true;
                SaveGame_SaveFile(ActClear_SaveGameCallback);
            }
        }
        if (ActClear->finishedSavingGame)
            UIWaitSpinner_StartWait();

        if (ActClear->finishedSavingGame) {
            self->state = ActClear_State_StartExitSequence;
        }
        else {
            if (ActClear->actID > 0 || Zone->stageFinishCallback) {
                self->state = ActClear_State_ExitActClear;
            }
            else {
                self->state = StateMachine_None;
                Zone_StartFadeOut(10, 0x000000);
            }
        }
    }
}

#if RETRO_USE_PLUS
void ActClear_State_ShowResultsTA(void)
{
    RSDK_THIS(ActClear);

    if (self->newRecordTimer > 0) {
        if (TimeAttackData->personalRank <= 0 || ReplayRecorder->hasSetupGhostVS) {
            --self->newRecordTimer;
        }
        else {
            if (self->newRecordTimer == 120) {
                if (TimeAttackData->personalRank == 1)
                    self->isNewRecord = true;
                self->achievedRank = true;
                RSDK.PlaySfx(ActClear->sfxEvent, false, 255);
            }

            if (self->newRecordTimer != 30) {
                --self->newRecordTimer;
            }
            else {
                if (TimeAttackData->personalRank == 1) {
                    RSDK.PlaySfx(Announcer->sfxNewRecordTop, false, 255);
                    --self->newRecordTimer;
                }
                else {
                    if (TimeAttackData->personalRank > 3) {
                        --self->newRecordTimer;
                    }
                    else {
                        RSDK.PlaySfx(Announcer->sfxNewRecordMid, false, 255);
                        --self->newRecordTimer;
                    }
                }
            }
        }
    }

    if (!ActClear->finishedSavingGame && !ActClear->disableResultsInput) {
#if RETRO_USE_TOUCH_CONTROLS
        for (int32 t = 0; t < TouchInfo->count; ++t) {
            int32 tx = (TouchInfo->x[t] * ScreenInfo->width);
            int32 ty = (TouchInfo->y[t] * ScreenInfo->height);

            if (TouchInfo->down[t]) {
                if (tx >= ScreenInfo->width - 0x80 && ty >= 0 && tx <= ScreenInfo->width && ty <= 0x40) {
                    ControllerInfo->keyY.press |= 1;
                    break;
                }
            }
        }

        ControllerInfo->keyStart.press |= TouchInfo->count && !ControllerInfo->keyY.press;
#endif

        if (ControllerInfo->keyY.press) {
            if (!ActClear->hasSavedReplay) {
                if (HUD->replaySaveEnabled) {
                    if (!UIDialog->activeDialog) {
                        if (API.CheckDLC(DLC_PLUS)) {
                            StateMachine_Run(ActClear->saveReplay_CB);
                            ActClear->hasSavedReplay      = true;
                            ActClear->disableResultsInput = true;
                            return;
                        }
                    }
                }
            }
        }

        if (ControllerInfo->keyStart.press) {
            RSDK.PlaySfx(UIWidgets->sfxAccept, false, 255);

            RSDK_THIS(ActClear);
            if (ActClear->actID > 0 || Zone->stageFinishCallback) {
                self->state = ActClear_State_ExitActClear;
            }
            else {
                self->state = StateMachine_None;
                Zone_StartFadeOut(10, 0x000000);
            }
        }
    }
}
#endif

void ActClear_State_StartExitSequence(void)
{
    if (ActClear->finishedSavingGame) {
        RSDK_THIS(ActClear);
        if (ActClear->actID > 0 || Zone->stageFinishCallback) {
            self->state = ActClear_State_ExitActClear;
        }
        else {
            self->state    = StateMachine_None;
            EntityZone *zone = RSDK_GET_ENTITY(SLOT_ZONE, Zone);
            zone->screenID   = 4;
            zone->timer      = 0;
            zone->fadeSpeed  = 10;
            zone->fadeColour = 0;
            zone->state      = Zone_State_Fadeout;
            zone->stateDraw  = Zone_Draw_Fade;
            zone->visible    = true;
            zone->drawOrder  = 15;
        }
    }
}

void ActClear_State_ExitActClear(void)
{
    RSDK_THIS(ActClear);
    self->playerNamePos.x += 0x200000;
    self->gotThroughPos.x -= 0x200000;
    if (self->playerNamePos.x > 0x400000)
        self->timeBonusPos.x += 0x200000;
    if (self->timeBonusPos.x > 0x400000)
        self->ringBonusPos.x += 0x200000;
    if (self->ringBonusPos.x > 0x400000)
        self->coolBonusPos.x += 0x200000;
    if (self->coolBonusPos.x > 0x400000)
        self->totalScorePos.x -= 0x200000;

    if (self->totalScorePos.x < -0x2000000) {
        if (ActClear->actID <= 0) {
            if (Zone->stageFinishCallback) {
                if (Zone->forcePlayerOnScreenFlag) {
                    self->timer = 0;
                    self->state = ActClear_State_ForcePlayerOnScreen;
                }
                else {
                    foreach_active(Animals, animal)
                    {
                        if (animal->behaviour == ANIMAL_BEHAVE_FOLLOW)
                            animal->behaviour = ANIMAL_BEHAVE_BOUNCEAROUND;
                    }
                    Zone->stageFinishCallback();
                    Zone->stageFinishCallback = NULL;
                }
            }
        }
        else {
            ActClear->finished           = true;
            ActClear->actID              = 0;
            SceneInfo->milliseconds = 0;
            SceneInfo->seconds      = 0;
            SceneInfo->minutes      = 0;
            foreach_active(Player, player)
            {
                player->ringExtraLife = 100;
                player->rings         = 0;
            }
        }
        if (self->state != ActClear_State_ForcePlayerOnScreen)
            destroyEntity(self);
    }
}

void ActClear_State_ForcePlayerOnScreen(void)
{
    RSDK_THIS(ActClear);
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);
    bool32 finishedP2     = false;
    bool32 finishedP1     = false;
    int32 screenOffX      = ScreenInfo->width - 16 + ScreenInfo->position.x;
    screenOffX <<= 16;
    player1->up        = false;
    player1->down      = false;
    player1->jumpPress = false;
    player1->jumpHold  = false;

    if (player1->position.x >= screenOffX) {
        player1->stateInput = StateMachine_None;
        player1->state      = Player_State_Ground;
        player1->groundVel  = -0x40000;
        player1->left       = true;
        player1->right      = false;
    }
    else {
        if ((!player1->onGround || player1->groundVel) && player1->position.x < screenOffX - (ScreenInfo->centerX << 15)) {
            player1->right = true;
            if (!player1->skidding) {
                if (!player1->left)
                    player1->right = false;
                else
                    player1->left = false;
            }
        }

        if (player1->onGround && !player1->groundVel)
            finishedP1 = true;
    }

    if (player2->objectID != Player->objectID) {
        finishedP2 = true;
    }
    else {
        player2->up        = false;
        player2->down      = false;
        player2->jumpPress = false;
        player2->jumpHold  = false;
        if (player2->state == Player_State_FlyIn || player2->state == Player_State_JumpIn) {
            if (player2->position.x < screenOffX) {
                if (player2->onGround && !player2->groundVel) {
                    RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, false, 0);
                    player2->direction = FLIP_NONE;
                    finishedP2         = true;
                }
            }
        }
        else if (player2->position.x >= screenOffX) {
            player2->stateInput = Player_ProcessP2Input_AI;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_RUN, &player2->animator, false, 0);
            player2->state     = Player_State_Ground;
            player2->groundVel = -0x40000;
            player2->left      = true;
            player2->right     = false;

            if (player2->position.x < screenOffX) {
                if (player2->onGround && !player2->groundVel) {
                    RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, false, 0);
                    player2->direction = FLIP_NONE;
                    finishedP2         = true;
                }
            }
        }
        else {
            if (player2->onGround && !player2->groundVel) {
                RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, false, 0);
                player2->direction = FLIP_NONE;
                finishedP2         = true;
            }
        }
    }

    if (finishedP1) {
        RSDK.SetSpriteAnimation(player1->aniFrames, ANI_IDLE, &player1->animator, false, 0);
        player1->direction = FLIP_NONE;
    }

    ++self->stageFinishTimer;
    if ((finishedP1 && finishedP2) || self->stageFinishTimer >= 900) {
        if (self->timer >= 10) {
            Player->jumpInDelay = 0;
            StateMachine_Run(Zone->stageFinishCallback);
            Zone->stageFinishCallback = StateMachine_None;
            destroyEntity(self);
        }
        else {
            self->timer++;
        }
    }
}

#if RETRO_INCLUDE_EDITOR
void ActClear_EditorDraw(void) {}

void ActClear_EditorLoad(void) {}
#endif

void ActClear_Serialize(void) {}
