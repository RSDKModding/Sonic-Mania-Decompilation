// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: ActClear Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

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
    RSDK_THIS(ActClear);

    Vector2 drawPos;
    Vector2 verts[4];
#if MANIA_USE_PLUS
    int32 milliseconds = 0;
    int32 seconds      = 0;
    int32 minutes      = 0;
#endif
    int32 offset = 0;

    drawPos.x  = self->gotThroughPos.x;
    drawPos.y  = self->gotThroughPos.y;
    verts[0].y = drawPos.y - TO_FIXED(20);
    verts[1].y = drawPos.y - TO_FIXED(20);
    verts[2].y = drawPos.y - TO_FIXED(4);
    verts[3].y = drawPos.y - TO_FIXED(4);
    if ((GET_CHARACTER_ID(1)) == ID_KNUCKLES) {
        int32 center = TO_FIXED(ScreenInfo->center.x + 16);

        drawPos.x  = 2 * self->gotThroughPos.x + center;
        verts[0].x = drawPos.x - TO_FIXED(145);
        verts[1].x = drawPos.x + TO_FIXED(52);
        verts[2].x = drawPos.x + TO_FIXED(68);
        verts[3].x = drawPos.x - TO_FIXED(129);
        RSDK.DrawFace(verts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

        drawPos.x  = 2 * self->playerNamePos.x + center;
        drawPos.y  = self->playerNamePos.y;
        verts[0].x = drawPos.x - TO_FIXED(124);
        verts[0].y = drawPos.y + TO_FIXED(28);
        verts[1].x = drawPos.x + TO_FIXED(80);
        verts[2].x = drawPos.x + TO_FIXED(96);
        verts[3].x = drawPos.x - TO_FIXED(108);
        verts[1].y = drawPos.y + TO_FIXED(28);
        verts[2].y = drawPos.y + TO_FIXED(44);
        verts[3].y = drawPos.y + TO_FIXED(44);
        RSDK.DrawFace(verts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

        drawPos.x = self->playerNamePos.x + center;
        drawPos.y = self->playerNamePos.y;
        RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

        self->gotThroughAnimator.frameID = 2;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);

        drawPos.x                        = self->gotThroughPos.x + center;
        drawPos.y                        = self->gotThroughPos.y;
        self->gotThroughAnimator.frameID = 3;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);
        RSDK.DrawSprite(&self->actNumAnimator, &drawPos, true);

        offset = center - TO_FIXED(10);
    }
    else {
        int32 center = TO_FIXED(ScreenInfo->center.x);

        drawPos.x  = 2 * self->gotThroughPos.x + center;
        verts[0].x = drawPos.x - TO_FIXED(109);
        verts[1].x = drawPos.x + TO_FIXED(52);
        verts[2].x = drawPos.x + TO_FIXED(68);
        verts[3].x = drawPos.x - TO_FIXED(93);
        RSDK.DrawFace(verts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

        drawPos.x  = 2 * self->playerNamePos.x + center;
        drawPos.y  = self->playerNamePos.y;
        verts[0].x = drawPos.x - TO_FIXED(88);
        verts[0].y = drawPos.y + TO_FIXED(28);
        verts[1].x = drawPos.x + TO_FIXED(80);
        verts[2].x = drawPos.x + TO_FIXED(96);
        verts[3].x = drawPos.x - TO_FIXED(72);
        verts[1].y = drawPos.y + TO_FIXED(28);
        verts[2].y = drawPos.y + TO_FIXED(44);
        verts[3].y = drawPos.y + TO_FIXED(44);
        RSDK.DrawFace(verts, 4, 0x00, 0x00, 0x00, 0xFF, INK_NONE);

        drawPos.x = self->playerNamePos.x + center;
        drawPos.y = self->playerNamePos.y;
        RSDK.DrawSprite(&self->playerNameAnimator, &drawPos, true);

#if MANIA_USE_PLUS
        if ((GET_CHARACTER_ID(1)) == ID_MIGHTY)
            self->gotThroughAnimator.frameID = 2;
        else
#endif
            self->gotThroughAnimator.frameID = 0;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);

        drawPos.x                        = self->gotThroughPos.x + center;
        drawPos.y                        = self->gotThroughPos.y;
        self->gotThroughAnimator.frameID = 1;
        RSDK.DrawSprite(&self->gotThroughAnimator, &drawPos, true);
        RSDK.DrawSprite(&self->actNumAnimator, &drawPos, true);

        offset = center + TO_FIXED(6);
    }

    // Draw "Time" Bonus Sprite
    drawPos.x                         = offset + self->timeBonusPos.x - TO_FIXED(92);
    drawPos.y                         = self->timeBonusPos.y;
    self->hudElementsAnimator.frameID = 1;
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    // Draw "Bonus" Sprite
    drawPos.x += TO_FIXED(50);
#if MANIA_USE_PLUS
    if (globals->gameMode != MODE_TIMEATTACK) {
#endif
        self->hudElementsAnimator.frameID = 8; // "Bonus"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
#if MANIA_USE_PLUS
    }
#endif

    // Draw Time Bonus BG thingy
    self->hudElementsAnimator.frameID = 10;
    drawPos.x += TO_FIXED(52);
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    drawPos.x += TO_FIXED(67);
    drawPos.y += TO_FIXED(14);

#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x -= TO_FIXED(98);
        drawPos.y -= TO_FIXED(14);
        // Draw Current Time
        ActClear_DrawTime(&drawPos, SceneInfo->minutes, SceneInfo->seconds, SceneInfo->milliseconds);
    }
    else {
#endif
        ActClear_DrawNumbers(&drawPos, self->timeBonus, 0); // Draw Time Bonus
#if MANIA_USE_PLUS
    }
#endif

    // Draw Ring Bonus
    drawPos.x = offset + self->ringBonusPos.x - TO_FIXED(92);
    drawPos.y = self->ringBonusPos.y;
#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK)
        self->hudElementsAnimator.frameID = 17; // "Best"
    else
#endif
        self->hudElementsAnimator.frameID = 5; // "Ring"
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    drawPos.x += TO_FIXED(50);
#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        drawPos.x -= TO_FIXED(10);
        self->hudElementsAnimator.frameID = 1; // "Time"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
        drawPos.x += TO_FIXED(10);
    }
    else {
#endif
        self->hudElementsAnimator.frameID = 8; // "Bonus"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
#if MANIA_USE_PLUS
    }
#endif

    // Draw Ring Bonus BG thingy
    self->hudElementsAnimator.frameID = 10;
    drawPos.x += TO_FIXED(52);
    RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

    drawPos.x += TO_FIXED(67);
    drawPos.y += TO_FIXED(14);
#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) { // Draw Best Time
        TimeAttackData_GetUnpackedTime(self->time, &minutes, &seconds, &milliseconds);
        drawPos.x -= TO_FIXED(98);
        drawPos.y -= TO_FIXED(14);
        if (!self->isNewRecord || (self->isNewRecord && (Zone->timer & 8)))
            ActClear_DrawTime(&drawPos, minutes, seconds, milliseconds);
    }
    else {
#endif
        ActClear_DrawNumbers(&drawPos, self->ringBonus, 0);
#if MANIA_USE_PLUS
    }
#endif

    if (self->showCoolBonus) { // Draw Cool Bonus
        drawPos.x = offset + self->coolBonusPos.x - TO_FIXED(92);
        drawPos.y = self->coolBonusPos.y;
#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_TIMEATTACK)
            self->hudElementsAnimator.frameID = 18; // "Rank"
        else
#endif
            self->hudElementsAnimator.frameID = 15; // "Cool"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += TO_FIXED(50);
#if MANIA_USE_PLUS
        if (globals->gameMode != MODE_TIMEATTACK) {
#endif
            self->hudElementsAnimator.frameID = 8; // "Bonus"
            RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);
#if MANIA_USE_PLUS
        }
#endif

        // Draw Cool Bonus BG thingy
        self->hudElementsAnimator.frameID = 10;
        drawPos.x += TO_FIXED(52);
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += TO_FIXED(67);
        drawPos.y += TO_FIXED(14);
#if MANIA_USE_PLUS
        if (globals->gameMode != MODE_TIMEATTACK) {
#endif
            ActClear_DrawNumbers(&drawPos, self->coolBonus, 0); // Draw Cool bonus
#if MANIA_USE_PLUS
        }
        else {
            // Draw Rank
            if (!TimeAttackData->personalRank) {
                self->numbersAnimator.frameID = 16;
                RSDK.DrawSprite(&self->numbersAnimator, &drawPos, true);

                drawPos.x -= TO_FIXED(9);
            }
            else if (!self->achievedRank || (self->achievedRank && (Zone->timer & 8)))
                ActClear_DrawNumbers(&drawPos, TimeAttackData->personalRank, 0);
        }
#endif
    }

    drawPos.x = self->totalScorePos.x;
    drawPos.y = self->totalScorePos.y;

#if MANIA_USE_PLUS
    if (globals->gameMode == MODE_TIMEATTACK) {
        // Draw World Rank
        drawPos.x                         = offset + self->totalScorePos.x - TO_FIXED(92);
        self->hudElementsAnimator.frameID = 19; // "World"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += TO_FIXED(50);
        self->hudElementsAnimator.frameID = 18; // "Rank"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        // Draw World Rank BG thingy
        self->hudElementsAnimator.frameID = 10;
        drawPos.x += TO_FIXED(52);
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        drawPos.x += TO_FIXED(67);
        drawPos.y += TO_FIXED(14);
        if (!TimeAttackData->leaderboardRank) {
            self->numbersAnimator.frameID = 16; // "-" (no rank)
            RSDK.DrawSprite(&self->numbersAnimator, &drawPos, true);
        }
        else {
            // Draw Rank
            ActClear_DrawNumbers(&drawPos, TimeAttackData->leaderboardRank, 0);
        }
    }
    else {
#endif
        // Draw Total Score
        drawPos.x                         = offset + self->totalScorePos.x - TO_FIXED(68);
        self->hudElementsAnimator.frameID = 9; // "Total"
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        // Draw Total Score BG thingy
        self->hudElementsAnimator.frameID = 10;
        drawPos.x += TO_FIXED(52);
        RSDK.DrawSprite(&self->hudElementsAnimator, &drawPos, true);

        // Draw Total Score
        drawPos.x += TO_FIXED(67);
        drawPos.y += TO_FIXED(14);
        ActClear_DrawNumbers(&drawPos, self->totalScore, 0);
#if MANIA_USE_PLUS
    }
#endif
}

void ActClear_Create(void *data)
{
    RSDK_THIS(ActClear);
    if (!SceneInfo->inEditor) {
#if MANIA_USE_PLUS
        ActClear->actClearActive = true;
#endif
        self->active           = ACTIVE_NORMAL;
        self->visible          = true;
        self->drawGroup        = Zone->hudDrawGroup;
        self->state            = ActClear_State_EnterText;
        self->stageFinishTimer = 0;
        self->newRecordTimer   = 0;

        EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        self->targetPlayer    = player1;

        if (Zone_GetZoneID() > ZONE_INVALID) {
            uint16 time = TimeAttackData_GetPackedTime(SceneInfo->minutes, SceneInfo->seconds, SceneInfo->milliseconds);

#if MANIA_USE_PLUS
            StatInfo stat;
            switch (GET_CHARACTER_ID(1)) {
                case ID_SONIC: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 1, time, player1->rings, player1->score); break;
                case ID_TAILS: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 2, time, player1->rings, player1->score); break;
                case ID_KNUCKLES: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 3, time, player1->rings, player1->score); break;
                case ID_MIGHTY: TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 4, time, player1->rings, player1->score); break;
                default:
                case ID_RAY:
                    if ((GET_CHARACTER_ID(1)) == ID_RAY)
                        TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 5, time, player1->rings, player1->score);
                    else
                        TimeAttackData_TrackActClear(&stat, Zone_GetZoneID(), Zone->actID, 1, time, player1->rings, player1->score);
                    break;
            }
            API.TryTrackStat(&stat);
#else
            switch (GET_CHARACTER_ID(1)) {
                case ID_SONIC: APICallback_TrackActClear(Zone_GetZoneID(), Zone->actID, 1, time, player1->rings, player1->score); break;
                case ID_TAILS: APICallback_TrackActClear(Zone_GetZoneID(), Zone->actID, 2, time, player1->rings, player1->score); break;
                case ID_KNUCKLES: APICallback_TrackActClear(Zone_GetZoneID(), Zone->actID, 3, time, player1->rings, player1->score); break;
                default: break;
            }
#endif
        }

#if MANIA_USE_PLUS
        if (!ActClear->disableTimeBonus) {
#endif
            switch (SceneInfo->minutes) {
                case 0:
                    if (SceneInfo->seconds >= 30)
                        self->timeBonus = SceneInfo->seconds < 45 ? 10000 : 5000;
                    else
                        self->timeBonus = 50000;
                    break;

                case 1: self->timeBonus = SceneInfo->seconds < 30 ? 4000 : 3000; break;
                case 2: self->timeBonus = 2000; break;
                case 3: self->timeBonus = 1000; break;
                case 4: self->timeBonus = 500; break;
                case 5: self->timeBonus = 100; break;

                case 9:
                    if (!SceneInfo->debugMode && globals->gameMode < MODE_TIMEATTACK && SceneInfo->seconds == 59) {
#if MANIA_USE_PLUS
                        if (globals->gameMode != MODE_ENCORE && !(globals->medalMods & MEDAL_NOTIMEOVER))
#endif
                            self->timeBonus = 100000;
                    }
                    break;

                default: break;
            }
#if MANIA_USE_PLUS
        }
#endif

        self->ringBonus        = 100 * player1->rings;
        self->coolBonus        = globals->coolBonus[0];
        globals->initCoolBonus = false;
#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_TIMEATTACK) {
            EntityMenuParam *param = MenuParam_GetParam();
            self->time =
                TimeAttackData_GetScore(param->zoneID, param->actID, param->characterID, SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE), 1);
            self->achievedRank = false;
            self->isNewRecord  = false;
        }
#endif
        self->showCoolBonus   = true;
        self->playerNamePos.x = TO_FIXED(224);
        self->playerNamePos.y = TO_FIXED(88);
        self->gotThroughPos.x = -TO_FIXED(224);
        self->gotThroughPos.y = TO_FIXED(112);
        self->timeBonusPos.x  = TO_FIXED(488);
        self->timeBonusPos.y  = TO_FIXED(120);
        self->ringBonusPos.x  = TO_FIXED(776);
        self->ringBonusPos.y  = TO_FIXED(136);
        self->coolBonusPos.x  = TO_FIXED(1064);
        self->coolBonusPos.y  = TO_FIXED(152);
        self->totalScorePos.x = -TO_FIXED(1352);
        self->totalScorePos.y = TO_FIXED(192);

        RSDK.SetSpriteAnimation(ActClear->aniFrames, 0, &self->hudElementsAnimator, true, 0);
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 1, &self->numbersAnimator, true, 0);
#if MANIA_USE_PLUS
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 0, &self->timeElementsAnimator, true, 12);
#endif

        switch (GET_CHARACTER_ID(1)) {
            default:
            case ID_SONIC: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 0); break;
            case ID_TAILS: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 1); break;
            case ID_KNUCKLES: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 2); break;
#if MANIA_USE_PLUS
            case ID_MIGHTY: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 3); break;
            case ID_RAY: RSDK.SetSpriteAnimation(ActClear->aniFrames, 3, &self->playerNameAnimator, true, 4); break;
#endif
        }
        RSDK.SetSpriteAnimation(ActClear->aniFrames, 4, &self->gotThroughAnimator, true, 0);

        // Used in cases like OOZ1 outro where the act clear actually happens in OOZ2
        if (ActClear->displayedActID <= 0)
            RSDK.SetSpriteAnimation(ActClear->aniFrames, 5, &self->actNumAnimator, true, Zone->actID);
        else
            RSDK.SetSpriteAnimation(ActClear->aniFrames, 5, &self->actNumAnimator, true, ActClear->displayedActID - 1);
    }
}

void ActClear_StageLoad(void)
{
    ActClear->aniFrames = RSDK.LoadSpriteAnimation("Global/HUD.bin", SCOPE_STAGE);

    ActClear->sfxScoreAdd   = RSDK.GetSfx("Global/ScoreAdd.wav");
    ActClear->sfxScoreTotal = RSDK.GetSfx("Global/ScoreTotal.wav");
#if MANIA_USE_PLUS
    ActClear->sfxEvent = RSDK.GetSfx("Special/Event.wav");

    ActClear->actClearActive = false;
    ActClear->forceNoSave    = false;
#endif
}

#if MANIA_USE_PLUS
void ActClear_DrawTime(Vector2 *drawPosPtr, int32 mins, int32 secs, int32 millisecs)
{
    RSDK_THIS(ActClear);

    Vector2 drawPos;
    if (!mins && !secs && !millisecs) {
        millisecs = -1;
        secs      = -1;
        mins      = -1;
    }

    // The ":" thing
    drawPos.x = drawPosPtr->x + TO_FIXED(50);
    drawPos.y = drawPosPtr->y - TO_FIXED(2);
    RSDK.DrawSprite(&self->timeElementsAnimator, &drawPos, true);

    // Miliseconds
    drawPos.x = drawPosPtr->x + TO_FIXED(97);
    drawPos.y = drawPosPtr->y + TO_FIXED(14);
    ActClear_DrawNumbers(&drawPos, millisecs, 2);

    // Seconds
    drawPos.x -= TO_FIXED(9);
    if (!mins && !secs && !millisecs)
        secs = -1;
    ActClear_DrawNumbers(&drawPos, secs, 2);

    // Minutes
    drawPos.x -= TO_FIXED(9);
    if (!mins && !secs && !millisecs)
        mins = -1;
    ActClear_DrawNumbers(&drawPos, mins, 1);
}
#endif

void ActClear_DrawNumbers(Vector2 *drawPos, int32 value, int32 digitCount)
{
    RSDK_THIS(ActClear);

    if (value >= 0) {
        if (!digitCount && value > 0) {
            int32 v = value;
            while (v > 0) {
                ++digitCount;
                v /= 10;
            }
        }
        else {
            if (!digitCount && !value)
                digitCount = 1;
        }

        if (digitCount > 0) {
            int32 digit = 1;
            while (digitCount--) {
                self->numbersAnimator.frameID = value / digit % 10;
                RSDK.DrawSprite(&self->numbersAnimator, drawPos, true);

                drawPos->x -= TO_FIXED(9);
                digit *= 10;
            }
        }
    }
    else {
        while (digitCount--) {
            self->numbersAnimator.frameID = 16;
            RSDK.DrawSprite(&self->numbersAnimator, drawPos, true);

            drawPos->x -= TO_FIXED(9);
        }
    }
}
void ActClear_CheckPlayerVictory(void)
{
    foreach_active(Player, player)
    {
        if (player->state != Player_State_FlyToPlayer && player->state != Player_State_ReturnToPlayer && player->state != Player_State_Victory
            && player->onGround) {
            player->state           = Player_State_Victory;
            player->nextAirState    = StateMachine_None;
            player->nextGroundState = StateMachine_None;
            if (Zone->shouldRecoverPlayers)
                player->stateInput = StateMachine_None;

            RSDK.SetSpriteAnimation(player->aniFrames, ANI_VICTORY, &player->animator, true, 0);
        }
    }
}

#if MANIA_USE_PLUS
void ActClear_SaveGameCallback(bool32 success)
{
    UIWaitSpinner_FinishWait();
    ActClear->isSavingGame = false;
}
#else
void ActClear_SaveGameCallback(void)
{
    UIWaitSpinner_FinishWait();
    ActClear->isSavingGame = false;
}
#endif

void ActClear_SetupRecoverPlayers(void)
{
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    if (player2 && player2->sidekick) {
        if (player2->state != Player_State_FlyToPlayer && player2->state != Player_State_ReturnToPlayer) {
            if (player2->position.x <= TO_FIXED(ScreenInfo->size.x + ScreenInfo->position.x)
                || abs(player2->position.y - player1->position.y) > TO_FIXED(16)) {
                Player->respawnTimer = 240;

                Entity *entStore  = SceneInfo->entity;
                SceneInfo->entity = (Entity *)player2;
                Player_HandleSidekickRespawn();
                SceneInfo->entity = entStore;

                if (player2->state == Player_State_FlyToPlayer || player2->state == Player_State_ReturnToPlayer
                    || player2->state == Player_State_HoldRespawn) {
                    player2->active     = ACTIVE_NORMAL;
                    player2->position.y = TO_FIXED(ScreenInfo->position.y - 16);
                }
            }
            else {
                Player->respawnTimer = -3600;
            }
        }
    }
}

void ActClear_State_EnterText(void)
{
    RSDK_THIS(ActClear);

    if (self->playerNamePos.x > 0)
        self->playerNamePos.x -= TO_FIXED(16);

    if (self->gotThroughPos.x < 0)
        self->gotThroughPos.x += TO_FIXED(16);

    if (!self->timer && Zone->shouldRecoverPlayers)
        ActClear_SetupRecoverPlayers();

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
        self->state = ActClear_State_EnterResults;
    }

    ActClear_CheckPlayerVictory();
}

void ActClear_State_EnterResults(void)
{
    RSDK_THIS(ActClear);

    if (self->timeBonusPos.x > 0)
        self->timeBonusPos.x -= TO_FIXED(16);

    if (self->ringBonusPos.x > 0)
        self->ringBonusPos.x -= TO_FIXED(16);

    if (self->coolBonusPos.x > 0)
        self->coolBonusPos.x -= TO_FIXED(16);

    if (self->totalScorePos.x < -TO_FIXED(8)) {
        self->totalScorePos.x += TO_FIXED(16);
    }
    else {
#if MANIA_USE_PLUS
        if (globals->gameMode == MODE_TIMEATTACK) {
            if (ActClear->bufferMoveEnabled) {
                StateMachine_Run(ActClear->bufferMove_CB);
            }

            HUD->showTAPrompt        = true;
            ActClear->hasSavedReplay = false;
            self->newRecordTimer     = 240;
            self->state              = ActClear_State_ShowResultsTA;
            RSDK.SetScene("Presentation", "Menu");
        }
        else {
            self->state = ActClear_State_ScoreShownDelay;
        }
#else
        self->state = ActClear_State_ScoreShownDelay;
#endif
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

    EntityPlayer *player = self->targetPlayer;

    if (self->timeBonus > 0) {
        self->totalScore += 100;
        self->timeBonus -= 100;
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

    if (ControllerInfo[player->controllerID].keyA.press || ControllerInfo[player->controllerID].keyStart.press) {
        Player_GiveScore(player, self->timeBonus + self->ringBonus + self->coolBonus);
        self->totalScore += self->timeBonus + self->ringBonus + self->coolBonus;
        self->timeBonus = 0;
        self->ringBonus = 0;
        self->coolBonus = 0;
    }

    if (self->timeBonus + self->ringBonus + self->coolBonus <= 0) {
        self->timer = 0;
        self->state = ActClear_State_SaveGameProgress;
        RSDK.PlaySfx(ActClear->sfxScoreTotal, false, 255);
    }
    else if (++self->timer == 2) {
        self->timer = 0;
        RSDK.PlaySfx(ActClear->sfxScoreAdd, false, 255);
    }

    Music->nextTrack = TRACK_NONE;
    ActClear_CheckPlayerVictory();
}

void ActClear_State_SaveGameProgress(void)
{
    RSDK_THIS(ActClear);

    if (++self->timer == 120) {
        self->timer            = 0;
        globals->specialRingID = 0;
        if (ActClear->displayedActID <= 0) {
            if (globals->gameMode == MODE_COMPETITION) {
                RSDK.SetScene("Presentation", "Menu");
            }
#if !MANIA_USE_PLUS
            else if (globals->gameMode == MODE_TIMEATTACK) {
                EntityMenuParam *param = MenuParam_GetParam();
                ActClear->isSavingGame = true;
                uint8 characterID      = param->characterID;
                uint8 zoneID           = param->zoneID;
                uint8 act              = param->actID;

                int32 time = 6000 * SceneInfo->minutes + 100 * SceneInfo->seconds + SceneInfo->milliseconds;

                uint16 *record = TimeAttackData_GetRecordedTime(zoneID, act, characterID, 1);
                int32 rank     = 0;
                for (; rank < 3; ++rank) {
                    if (!record[rank] || time < record[rank])
                        break;
                }

                if (rank < 3) {
                    rank++;
                    TimeAttackData_AddRecord(zoneID, act, characterID, rank, time);
                    APICallback_TrackTAClear(zoneID, act, characterID, time);
                    param->timeScore = rank;
                    SaveGame_SaveFile(ActClear_SaveGameCallback);
                }
                else {
                    ActClear->isSavingGame = false;
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
                    SaveGame_ClearCollectedSpecialRings();
                SaveGame_SaveProgress();
#if MANIA_USE_PLUS
                if (globals->saveSlotID != NO_SAVE_SLOT && !ActClear->forceNoSave) {
#else
                if (globals->gameMode == MODE_MANIA) {
#endif
                    if (Zone_IsZoneLastAct())
                        GameProgress_MarkZoneCompleted(Zone_GetZoneID());

                    ActClear->isSavingGame = true;
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

#if MANIA_USE_PLUS
            if (globals->saveSlotID != NO_SAVE_SLOT && !ActClear->forceNoSave) {
#else
            if (globals->gameMode == MODE_MANIA) {
#endif
                ActClear->isSavingGame = true;
                SaveGame_SaveFile(ActClear_SaveGameCallback);
            }
        }

        if (ActClear->isSavingGame)
            UIWaitSpinner_StartWait();

        self->state = ActClear_State_WaitForSave;
        ActClear_State_WaitForSave();
    }
}

#if MANIA_USE_PLUS
void ActClear_State_ShowResultsTA(void)
{
    RSDK_THIS(ActClear);

    if (self->newRecordTimer > 0) {
        if (TimeAttackData->personalRank > 0 && !ReplayRecorder->hasSetupGhostView) {
            if (self->newRecordTimer == 120) {
                if (TimeAttackData->personalRank == 1)
                    self->isNewRecord = true;

                self->achievedRank = true;
                RSDK.PlaySfx(ActClear->sfxEvent, false, 255);
            }

            if (self->newRecordTimer == 30) {
                if (TimeAttackData->personalRank == 1)
                    RSDK.PlaySfx(Announcer->sfxNewRecordTop, false, 255);
                else if (TimeAttackData->personalRank <= 3)
                    RSDK.PlaySfx(Announcer->sfxNewRecordMid, false, 255);
            }
        }
        --self->newRecordTimer;
    }

    if (!ActClear->isSavingGame && !ActClear->disableResultsInput) {
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

            ActClear_State_WaitForSave();
        }
    }
}
#endif

void ActClear_State_WaitForSave(void)
{
    RSDK_THIS(ActClear);

    if (!ActClear->isSavingGame) {
        if (ActClear->displayedActID > 0 || Zone->stageFinishCallback) {
            self->state = ActClear_State_ExitActClear;
        }
        else {
            self->state = StateMachine_None;
            Zone_StartFadeOut(10, 0x000000);
        }
    }
}

void ActClear_State_ExitActClear(void)
{
    RSDK_THIS(ActClear);

    self->playerNamePos.x += TO_FIXED(32);
    self->gotThroughPos.x -= TO_FIXED(32);

    if (self->playerNamePos.x > TO_FIXED(64))
        self->timeBonusPos.x += TO_FIXED(32);

    if (self->timeBonusPos.x > TO_FIXED(64))
        self->ringBonusPos.x += TO_FIXED(32);

    if (self->ringBonusPos.x > TO_FIXED(64))
        self->coolBonusPos.x += TO_FIXED(32);

    if (self->coolBonusPos.x > TO_FIXED(64))
        self->totalScorePos.x -= TO_FIXED(32);

    if (self->totalScorePos.x < -TO_FIXED(512)) {
        if (ActClear->displayedActID <= 0) {
            if (Zone->stageFinishCallback) {
                if (Zone->shouldRecoverPlayers) {
                    self->timer = 0;
                    self->state = ActClear_State_RecoverPlayers;
                }
                else {
                    foreach_active(Animals, animal)
                    {
                        if (animal->behaviour == ANIMAL_BEHAVE_FOLLOW)
                            animal->behaviour = ANIMAL_BEHAVE_FREE;
                    }
#if RETRO_USE_MOD_LOADER
                    StateMachine_Run(Zone->stageFinishCallback);
#else
                    Zone->stageFinishCallback();
#endif
                    Zone->stageFinishCallback = StateMachine_None;
                }
            }
        }
        else {
            ActClear->finished       = true;
            ActClear->displayedActID = 0;
            SceneInfo->milliseconds  = 0;
            SceneInfo->seconds       = 0;
            SceneInfo->minutes       = 0;
            foreach_active(Player, player)
            {
                player->ringExtraLife = 100;
                player->rings         = 0;
            }
        }

        if (self->state != ActClear_State_RecoverPlayers)
            destroyEntity(self);
    }
}

void ActClear_State_RecoverPlayers(void)
{
    RSDK_THIS(ActClear);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    EntityPlayer *player2 = RSDK_GET_ENTITY(SLOT_PLAYER2, Player);

    bool32 finishedP2  = false;
    bool32 finishedP1  = false;
    int32 screenOffX   = TO_FIXED(ScreenInfo->position.x + ScreenInfo->size.x - 16);
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
        if ((!player1->onGround || player1->groundVel) && player1->position.x < screenOffX - (ScreenInfo->center.x << 15)) {
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

    if (player2->classID != Player->classID) {
        finishedP2 = true;
    }
    else {
        player2->up        = false;
        player2->down      = false;
        player2->jumpPress = false;
        player2->jumpHold  = false;

        if (player2->state == Player_State_FlyToPlayer || player2->state == Player_State_ReturnToPlayer) {
            if (player2->position.x < screenOffX) {
                if (player2->onGround && !player2->groundVel) {
                    RSDK.SetSpriteAnimation(player2->aniFrames, ANI_IDLE, &player2->animator, false, 0);
                    player2->direction = FLIP_NONE;
                    finishedP2         = true;
                }
            }
        }
        else if (player2->position.x >= screenOffX) {
            player2->stateInput = Player_Input_P2_AI;
            RSDK.SetSpriteAnimation(player2->aniFrames, ANI_RUN, &player2->animator, false, 0);
            player2->state     = Player_State_Ground;
            player2->groundVel = -TO_FIXED(4);
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
            Player->respawnTimer = 0;
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
