// ---------------------------------------------------------------------
// RSDK Project: Sonic Mania
// Object Description: TimeAttackGate Object
// Object Author: Christian Whitehead/Simon Thomley/Hunter Bridges
// Decompiled by: Rubberduckycooly & RMGRich
// ---------------------------------------------------------------------

#include "Game.h"

ObjectTimeAttackGate *TimeAttackGate;

void TimeAttackGate_Update(void)
{
    RSDK_THIS(TimeAttackGate);

    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->classID != PauseMenu->classID) {
        StateMachine_Run(self->state);
    }
}

void TimeAttackGate_LateUpdate(void) {}

void TimeAttackGate_StaticUpdate(void)
{
    if (globals->gameMode == MODE_TIMEATTACK) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

        if (TimeAttackGate->isFinished || !TimeAttackGate->started)
            player->drownTimer = 0;

        if (TimeAttackGate->suppressedTitlecard) {
            EntityTimeAttackGate *entity        = CREATE_ENTITY(TimeAttackGate, INT_TO_VOID(true), 0, 0);
            entity->state                       = TimeAttackGate_State_Fadeout;
            entity->fadeTimer                   = 532;
            TimeAttackGate->suppressedTitlecard = false;
        }

        if (DebugMode->debugActive)
            TimeAttackGate->disableRecords = true;
    }
}

void TimeAttackGate_Draw(void)
{
    RSDK_THIS(TimeAttackGate);

    StateMachine_Run(self->stateDraw);
}

void TimeAttackGate_Create(void *data)
{
    RSDK_THIS(TimeAttackGate);

    if (SceneInfo->inEditor || globals->gameMode == MODE_TIMEATTACK) {
        RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 0, &self->baseAnimator, true, 0);
        RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 1, &self->topAnimator, true, 0);

        self->visible = true;
        if (self->finishLine)
            RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 4, &self->finAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 3, &self->finAnimator, true, 0);

        if (!SceneInfo->inEditor) {
            if (data == INT_TO_VOID(true)) {
                self->drawGroup = 14;
                self->active    = ACTIVE_ALWAYS;
                self->state     = TimeAttackGate_State_Restarter;
                self->stateDraw = TimeAttackGate_Draw_Restarter;
            }
            else {
                self->active        = ACTIVE_NORMAL;
                self->updateRange.x = TO_FIXED(64);
                self->updateRange.y = TO_FIXED(64);
                self->drawGroup     = Zone->playerDrawGroup[0] + 1;

                int32 left   = self->boundsOffset.x - (self->boundsSize.x >> 1);
                int32 top    = self->boundsOffset.y - (self->boundsSize.y >> 1);
                int32 right  = self->boundsOffset.x + (self->boundsSize.x >> 1);
                int32 bottom = self->boundsOffset.y + (self->boundsSize.y >> 1);

                int32 extendTop    = -(self->extendTop << 16);
                int32 extendBottom = (self->extendBottom << 16);

                if (extendTop < top)
                    top = extendTop;

                if (extendBottom < bottom)
                    bottom = extendBottom;

                if (abs(left) > right)
                    self->updateRange.x = abs(left);
                else
                    self->updateRange.x = right;

                if (abs(top) > bottom)
                    self->updateRange.y = abs(top);
                else
                    self->updateRange.y = bottom;

                self->updateRange.x += TO_FIXED(64);
                self->updateRange.y += TO_FIXED(64);

                self->scale.y   = 0x200;
                self->state     = TimeAttackGate_State_Gate;
                self->stateDraw = TimeAttackGate_Draw_Gate;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void TimeAttackGate_StageLoad(void)
{
    TimeAttackGate->teleportChannel = -1;

    TimeAttackGate->aniFrames = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);

    TimeAttackGate->hitboxGate.left   = -8;
    TimeAttackGate->hitboxGate.top    = -44;
    TimeAttackGate->hitboxGate.right  = 8;
    TimeAttackGate->hitboxGate.bottom = 20;

    TimeAttackGate->sfxSignpost = RSDK.GetSfx("Global/SignPost.wav");
    TimeAttackGate->sfxTeleport = RSDK.GetSfx("Global/Teleport.wav");

    TimeAttackGate->started        = false;
    TimeAttackGate->disableRecords = false;
    SceneInfo->timeEnabled         = false;

    if (globals->suppressTitlecard)
        TimeAttackGate->suppressedTitlecard = true;
}

void TimeAttackGate_HandleSpin(void)
{
    RSDK_THIS(TimeAttackGate);

    int32 top    = self->position.y + ((TimeAttackGate->hitboxGate.top - self->extendTop) << 16);
    int32 bottom = self->position.y + ((TimeAttackGate->hitboxGate.bottom + self->extendBottom) << 16);

    foreach_active(Player, player)
    {
#if MANIA_USE_PLUS
        if (!player->sidekick && !player->isGhost) {
#else
        if (!player->sidekick) {
#endif
            if (!self->playerPos.x || !self->playerPos.y) {
                foreach_break;
            }

            int32 xVel = player->onGround ? player->groundVel : player->velocity.x;
            if (xVel >> 15
                && MathHelpers_CheckPositionOverlap(player->position.x, player->position.y, self->playerPos.x, self->playerPos.y, self->position.x,
                                                    bottom, self->position.x, top)) {
                int32 spinSpeed = (xVel >> 15) - 2;
                if ((xVel >> 15) >= 0)
                    spinSpeed = (xVel >> 15) + 2;

                if (abs(spinSpeed) > abs(self->spinSpeed))
                    self->spinSpeed = spinSpeed;

                self->spinTimer = 0;
            }
        }
    }

    bool32 finishedSpinning = false;

    self->angle += self->spinSpeed;
    if (self->spinSpeed <= 0) {
        if (self->angle <= -0x200) {
            ++self->spinTimer;
            self->angle += 0x200;

            self->spinSpeed += 4;
            if (self->spinSpeed > -2)
                self->spinSpeed = -2;

            finishedSpinning = self->spinSpeed == -2;
        }
    }
    else {
        if (self->angle >= 0x200) {
            ++self->spinTimer;
            self->angle -= 0x200;

            self->spinSpeed -= 4;
            if (self->spinSpeed < 2)
                self->spinSpeed = 2;

            finishedSpinning = self->spinSpeed == 2;
        }
    }

    if (finishedSpinning) {
        self->spinSpeed = 0;
        self->angle     = 0;
    }
}

void TimeAttackGate_HandleStart(void)
{
    RSDK_THIS(TimeAttackGate);

    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
    int32 top             = ((TimeAttackGate->hitboxGate.top - self->extendTop) << 16) + self->position.y;
    int32 bottom          = ((TimeAttackGate->hitboxGate.bottom + self->extendBottom) << 16) + self->position.y;

    if (MathHelpers_CheckPositionOverlap(player1->position.x, player1->position.y, self->playerPos.x, self->playerPos.y, self->position.x, bottom,
                                         self->position.x, top)) {
        self->hasFinished = true;
        if (!self->finishLine) {
            if (!TimeAttackGate->started) {
                RSDK.PlaySfx(TimeAttackGate->sfxSignpost, false, 255);

                TimeAttackGate->triggerPlayer = player1;
                TimeAttackGate->started       = true;
                SceneInfo->timeEnabled        = true;

                EntityTimeAttackGate *restarter = CREATE_ENTITY(TimeAttackGate, INT_TO_VOID(true), self->position.x, self->position.y);
                TimeAttackGate->restartManager  = restarter;
                restarter->isPermanent          = true;

#if MANIA_USE_PLUS
                StateMachine_Run(TimeAttackGate->startCB);
#endif
            }
        }
        else if (TimeAttackGate->started) {
            Music_FadeOut(0.025);
            Announcer_AnnounceGoal(0);

            RSDK.PlaySfx(TimeAttackGate->sfxSignpost, false, 255);
            TimeAttackGate->triggerPlayer = NULL;
            TimeAttackGate->started       = false;
            SceneInfo->timeEnabled        = false;

#if MANIA_USE_PLUS
            StateMachine_Run(TimeAttackGate->endCB);
#endif

            if (!TimeAttackGate->disableRecords)
                ActClear->bufferMoveEnabled = true;

#if MANIA_USE_PLUS
            TimeAttackGate_AddRecord();
#endif
        }
    }

    self->playerPos.x = player1->position.x;
    self->playerPos.y = player1->position.y;
}

#if MANIA_USE_PLUS
void TimeAttackGate_AddRecord(void)
{
    if (!TimeAttackGate->disableRecords) {
        if (ActClear)
            ActClear->isSavingGame = true;

        if (UIWaitSpinner)
            UIWaitSpinner_StartWait();

        EntityMenuParam *param = MenuParam_GetParam();
        int32 characterID      = param->characterID;
        int32 zoneID           = param->zoneID;
        int32 score            = SceneInfo->milliseconds + 100 * (SceneInfo->seconds + 60 * SceneInfo->minutes);
        int32 act              = param->actID;
        bool32 encore          = SceneInfo->filter == (FILTER_BOTH | FILTER_ENCORE);

        param->timeAttackRank = TimeAttackData_AddRecord(zoneID, act, characterID, encore, score, TimeAttackGate_WaitSave_Leaderboards);
        TimeAttackData_AddLeaderboardEntry(zoneID, act, characterID, encore, score);
    }
}

void TimeAttackGate_WaitSave_Leaderboards(bool32 success)
{
    if (ActClear)
        ActClear->isSavingGame = false;

    if (UIWaitSpinner)
        UIWaitSpinner_FinishWait();
}
#endif

void TimeAttackGate_CheckTouch(void)
{
    RSDK_THIS(TimeAttackGate);

    int32 x = self->position.x + self->boundsOffset.x;
    int32 y = self->position.y + self->boundsOffset.y;

    Hitbox hitbox;
    hitbox.left   = -(self->boundsSize.x >> 17);
    hitbox.right  = self->boundsSize.x >> 17;
    hitbox.top    = -(self->boundsSize.y >> 17);
    hitbox.bottom = self->boundsSize.y >> 17;

    for (int32 p = 0; p < Player->playerCount; ++p) {
        EntityPlayer *player = RSDK_GET_ENTITY(p, Player);

        bool32 passedGate = false;
        if (self->boundsSize.x <= 0 || self->boundsSize.y <= 0) {
            passedGate = self->position.x - player->position.x < TO_FIXED(256);
        }
        else {
            int32 storeX     = self->position.x;
            int32 storeY     = self->position.y;
            self->position.x = x;
            self->position.y = y;
            passedGate       = Player_CheckCollisionTouch(player, self, &hitbox);
            self->position.x = storeX;
            self->position.y = storeY;
        }

        if (passedGate) {
            Zone->cameraBoundsL[p] = (self->position.x >> 16) - ScreenInfo[p].center.x;
            Zone->cameraBoundsR[p] = (self->position.x >> 16) + ScreenInfo[p].center.x;

            if (self->topBoundary)
                Zone->cameraBoundsT[p] = (self->position.y >> 16) - ScreenInfo[p].center.y;
        }
    }
}

void TimeAttackGate_State_Gate(void)
{
    RSDK_THIS(TimeAttackGate);

    TimeAttackGate_HandleSpin();
    TimeAttackGate_HandleStart();

    foreach_active(Player, player)
    {
        player->lives = 1;

        if (!player->sidekick) {
            if (!self->finishLine) {
                if (!TimeAttackGate->started) {
                    foreach_active(HUD, hud)
                    {
                        if (hud)
                            hud->enableTimeFlash = true;
                        foreach_break;
                    }
                }
            }
            else {
                if (self->hasFinished && player->stateInput) {
                    player->stateInput = StateMachine_None;
                    player->up         = false;
                    player->left       = false;
                    player->right      = false;
                    player->down       = false;
                    player->jumpPress  = false;
                    player->jumpHold   = false;
                }

                if (!self->finishLine && !TimeAttackGate->started) {
                    foreach_active(HUD, hud)
                    {
                        if (hud)
                            hud->enableTimeFlash = true;
                        foreach_break;
                    }
                }
            }

            self->playerPos.x = player->position.x;
            self->playerPos.y = player->position.y;
        }
    }

    if (self->finishLine) {
        TimeAttackGate_CheckTouch();

        if (self->hasFinished) {
            TimeAttackGate->isFinished = true;

            if (self->timer >= 180) {
                if (self->timer == 180) {
                    Music_PlayTrack(TRACK_ACTCLEAR);
                    RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->classID, NULL);
                    ++self->timer;
                }
            }
            else {
                self->timer++;
            }
        }
    }
}

void TimeAttackGate_State_Restarter(void)
{
    RSDK_THIS(TimeAttackGate);

    if (TimeAttackGate->isFinished) {
        destroyEntity(self);
    }
    else {
        EntityPlayer *player = TimeAttackGate->triggerPlayer;

        if (player) {
            if (player->state == Player_State_Death || player->state == Player_State_Drown) {
                self->restartTimer = 0;
            }
            else {
                if (ControllerInfo[player->controllerID].keyY.press)
                    self->restartTimer = 0;

                if (!ControllerInfo[player->controllerID].keyY.down) {
                    if (self->restartTimer > 0) {
                        TimeAttackGate->teleportChannel = -1;
                        RSDK.StopSfx(TimeAttackGate->sfxTeleport);
                        self->restartTimer -= 4;
                    }

                    if (self->restartTimer < 0)
                        self->restartTimer = 0;
                }
                else {
                    if (!self->restartTimer)
                        TimeAttackGate->teleportChannel = RSDK.PlaySfx(TimeAttackGate->sfxTeleport, false, 255);

                    if (self->restartTimer == 35) {
                        self->state                = NULL;
                        globals->suppressTitlecard = true;

                        for (int32 c = 0; c < CHANNEL_COUNT; ++c) {
                            if (c != Music->channelID && c != TimeAttackGate->teleportChannel)
                                RSDK.StopChannel(c);
                        }

                        if (!Music->activeTrack)
                            globals->suppressAutoMusic = true;

                        RSDK.LoadScene();
                    }
                    else if (self->restartTimer < 35)
                        self->restartTimer++;
                }
            }

            self->radius = (720 * self->restartTimer / 35);
        }
    }
}

void TimeAttackGate_State_Fadeout(void)
{
    RSDK_THIS(TimeAttackGate);

    if (self->fadeTimer <= 0) {
        globals->suppressTitlecard = false;
        destroyEntity(self);
    }
    else {
        self->fadeTimer -= 32;
    }
}

void TimeAttackGate_Draw_Gate(void)
{
    RSDK_THIS(TimeAttackGate);
    Vector2 drawPos;

    self->rotation = self->baseRotation;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->topAnimator, NULL, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    self->drawFX = FX_SCALE;
    drawPos.y    = self->position.y;

    self->scale.x             = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = self->position.x + TO_FIXED(3);
    self->finAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                 = self->position.x - TO_FIXED(3);
    self->finAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    drawPos.x                 = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = 0xB40 * RSDK.Cos512(self->angle & 0x7F) + self->position.x;
    self->finAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                 = self->position.x - 0xB40 * RSDK.Sin512(self->angle & 0x7F);
    self->finAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);
}

void TimeAttackGate_Draw_Restarter(void)
{
    RSDK_THIS(TimeAttackGate);

    EntityPlayer *player = TimeAttackGate->triggerPlayer;
    if (player) {
        if (self->radius > 0)
            RSDK.DrawCircle(player->position.x, player->position.y, self->radius, 0xF0F0F0, (self->restartTimer << 8) / 35, INK_ADD, false);

        if (self->fadeTimer > 0)
            RSDK.FillScreen(0xFFFFFF, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
    }
}

#if GAME_INCLUDE_EDITOR
void TimeAttackGate_EditorDraw(void)
{
    RSDK_THIS(TimeAttackGate);
    Vector2 drawPos;

    if (self->finishLine)
        RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 4, &self->finAnimator, true, 0);
    else
        RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 3, &self->finAnimator, true, 0);

    self->rotation = self->baseRotation;
    self->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&self->topAnimator, NULL, false);
    RSDK.DrawSprite(&self->baseAnimator, NULL, false);

    self->drawFX  = FX_SCALE;
    drawPos.y     = self->position.y;
    self->scale.y = 0x200;

    self->scale.x             = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = self->position.x + TO_FIXED(3);
    self->finAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                 = self->position.x - TO_FIXED(3);
    self->finAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    drawPos.x                 = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = 0xB40 * RSDK.Cos512(self->angle & 0x7F) + self->position.x;
    self->finAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Sin512(self->angle & 0x7F));
    drawPos.x                 = 0x180 * RSDK.Cos512(2 * (self->angle & 0x7F)) + self->position.x;
    self->finAnimator.frameID = 0;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                 = self->position.x - 0xB40 * RSDK.Sin512(self->angle & 0x7F);
    self->finAnimator.frameID = 2;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->updateRange.x = TO_FIXED(64);
    self->updateRange.y = TO_FIXED(64);
    if (showGizmos()) {
        RSDK_DRAWING_OVERLAY(true);

        int32 left   = self->boundsOffset.x - (self->boundsSize.x >> 1);
        int32 top    = self->boundsOffset.y - (self->boundsSize.y >> 1);
        int32 right  = self->boundsOffset.x + (self->boundsSize.x >> 1);
        int32 bottom = self->boundsOffset.y + (self->boundsSize.y >> 1);

        int32 extendTop    = -(self->extendTop << 16);
        int32 extendBottom = (self->extendBottom << 16);

        if (extendTop < top)
            top = extendTop;

        if (extendBottom < bottom)
            bottom = extendBottom;

        if (abs(left) > right)
            self->updateRange.x = abs(left);
        else
            self->updateRange.x = right;

        if (abs(top) > bottom)
            self->updateRange.y = abs(top);
        else
            self->updateRange.y = bottom;

        self->updateRange.x += TO_FIXED(64);
        self->updateRange.y += TO_FIXED(64);

        DrawHelpers_DrawArenaBounds(FROM_FIXED(left), FROM_FIXED(top), FROM_FIXED(right + 64), FROM_FIXED(bottom + 64), 1 | 2 | 4 | 8, 0xFFFF00);

        Hitbox hitbox = TimeAttackGate->hitboxGate;
        hitbox.top    = TimeAttackGate->hitboxGate.top - self->extendTop;
        hitbox.bottom = TimeAttackGate->hitboxGate.bottom + self->extendBottom;
        DrawHelpers_DrawHitboxOutline(self->position.x, self->position.y, &hitbox, FLIP_NONE, 0xFF0000);

        RSDK_DRAWING_OVERLAY(false);
    }
}

void TimeAttackGate_EditorLoad(void)
{
    TimeAttackGate->aniFrames = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);

    TimeAttackGate->hitboxGate.left   = -8;
    TimeAttackGate->hitboxGate.top    = -44;
    TimeAttackGate->hitboxGate.right  = 8;
    TimeAttackGate->hitboxGate.bottom = 20;
}
#endif

void TimeAttackGate_Serialize(void)
{
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_BOOL, finishLine);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_ENUM, extendTop);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_ENUM, extendBottom);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_VECTOR2, boundsSize);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_VECTOR2, boundsOffset);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_BOOL, topBoundary);
}
