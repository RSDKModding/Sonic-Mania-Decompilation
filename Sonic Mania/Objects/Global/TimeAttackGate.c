#include "SonicMania.h"

ObjectTimeAttackGate *TimeAttackGate;

void TimeAttackGate_Update(void)
{
    RSDK_THIS(TimeAttackGate);
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        StateMachine_Run(self->state);
    }
}

void TimeAttackGate_LateUpdate(void) {}

void TimeAttackGate_StaticUpdate(void)
{
    if (globals->gameMode == MODE_TIMEATTACK) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (TimeAttackGate->isFinished || !TimeAttackGate->started) {
            player->airTimer = 0;
        }

        if (TimeAttackGate->suppressedTitlecard) {
            EntityTimeAttackGate *entity        = CREATE_ENTITY(TimeAttackGate, intToVoid(true), 0, 0);
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
            if (data == intToVoid(true)) {
                self->drawOrder = 14;
                self->active    = ACTIVE_ALWAYS;
                self->state     = TimeAttackGate_State_Restarter;
                self->stateDraw = TimeAttackGate_Draw_Restarter;
            }
            else {
                self->active        = ACTIVE_NORMAL;
                self->updateRange.x = 0x400000;
                self->updateRange.y = 0x400000;
                self->drawOrder     = Zone->playerDrawLow + 1;

                int32 boundsBottom = self->boundsOffset.y + (self->boundsSize.y >> 1);
                int32 boundsTop    = self->boundsOffset.y - (self->boundsSize.y >> 1);

                if ((-0x10000 * self->extendTop) < self->boundsOffset.y - (self->boundsSize.y >> 1))
                    boundsTop = (-0x10000 * self->extendTop);

                if (self->extendBottom << 16 < boundsBottom)
                    boundsBottom = self->extendBottom << 16;

                if (abs(self->boundsOffset.x - (self->boundsSize.x >> 1)) > boundsTop)
                    self->updateRange.x = abs(self->boundsOffset.x - (self->boundsSize.x >> 1));
                else
                    self->updateRange.x = ((self->boundsSize.x >> 1) + self->boundsOffset.x) + 0x400000;

                if (abs(boundsTop) > boundsBottom)
                    self->updateRange.y = abs(boundsTop) + 0x400000;
                else
                    self->updateRange.y = boundsBottom + 0x400000;

                self->scale.y   = 0x200;
                self->state     = TimeAttackGate_State_Main;
                self->stateDraw = TimeAttackGate_Draw_Main;
            }
        }
    }
    else {
        destroyEntity(self);
    }
}

void TimeAttackGate_StageLoad(void)
{
    TimeAttackGate->teleportChannel = 0xFFFF;
    TimeAttackGate->aniFrames       = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);
    TimeAttackGate->hitbox.left     = -8;
    TimeAttackGate->hitbox.top      = -44;
    TimeAttackGate->hitbox.right    = 8;
    TimeAttackGate->hitbox.bottom   = 20;
    TimeAttackGate->sfxSignpost     = RSDK.GetSfx("Global/SignPost.wav");
    TimeAttackGate->sfxTeleport     = RSDK.GetSfx("Global/Teleport.wav");
    TimeAttackGate->started         = false;
    TimeAttackGate->disableRecords  = false;
    SceneInfo->timeEnabled          = false;
    if (globals->suppressTitlecard)
        TimeAttackGate->suppressedTitlecard = true;
}

void TimeAttackGate_HandleSpin(void)
{
    RSDK_THIS(TimeAttackGate);

    int32 top    = (TimeAttackGate->hitbox.top << 16) - (self->extendTop << 16) + self->position.y;
    int32 bottom = ((self->extendBottom + TimeAttackGate->hitbox.bottom) << 16) + self->position.y;

    foreach_active(Player, player)
    {
#if RETRO_USE_PLUS
        if (!player->sidekick && !player->isGhost) {
#else
        if (!player->sidekick) {
#endif
            if (!self->playerPos.x || !self->playerPos.y) {
                foreach_break;
            }

            int32 vel = 0;
            if (player->onGround)
                vel = player->groundVel;
            else
                vel = player->velocity.x;
            if (vel >> 15
                && MathHelpers_Unknown12(player->position.x, player->position.y, self->playerPos.x, self->playerPos.y, self->position.x, bottom,
                                         self->position.x, top)) {
                int32 val = (vel >> 15) - 2;
                if ((vel >> 15) >= 0)
                    val = (vel >> 15) + 2;
                if (abs(val) > abs(self->spinSpeed))
                    self->spinSpeed = val;
                self->spinTimer = 0;
            }
        }
    }

    self->angle += self->spinSpeed;
    bool32 flag = false;
    if (self->spinSpeed <= 0) {
        if (self->angle <= -512) {
            ++self->spinTimer;
            self->angle += 512;
            self->spinSpeed += 4;
            if (self->spinSpeed > -2)
                self->spinSpeed = -2;
            flag = self->spinSpeed == -2;
        }
    }
    else {
        if (self->angle >= 512) {
            ++self->spinTimer;
            self->angle -= 512;
            self->spinSpeed -= 4;
            if (self->spinSpeed < 2)
                self->spinSpeed = 2;
            flag = self->spinSpeed == 2;
        }
    }

    if (flag) {
        self->spinSpeed = 0;
        self->angle     = 0;
    }
}

void TimeAttackGate_HandleStart(void)
{
    RSDK_THIS(TimeAttackGate);
    int32 top             = (TimeAttackGate->hitbox.top << 16) - (self->extendTop << 16) + self->position.y;
    int32 bottom          = ((self->extendBottom + TimeAttackGate->hitbox.bottom) << 16) + self->position.y;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (MathHelpers_Unknown12(player1->position.x, player1->position.y, self->playerPos.x, self->playerPos.y, self->position.x, bottom,
                              self->position.x, top)) {
        self->hasFinished = true;
        if (!self->finishLine) {
            if (!TimeAttackGate->started) {
                RSDK.PlaySfx(TimeAttackGate->sfxSignpost, false, 255);
                TimeAttackGate->playerPtr       = player1;
                TimeAttackGate->started         = true;
                SceneInfo->timeEnabled          = true;
                EntityTimeAttackGate *restarter = CREATE_ENTITY(TimeAttackGate, intToVoid(1), self->position.x, self->position.y);
                TimeAttackGate->activeEntity    = (Entity *)restarter;
                restarter->isPermanent          = true;
#if RETRO_USE_PLUS
                StateMachine_Run(TimeAttackGate->startCB);
#endif
            }
        }
        else if (TimeAttackGate->started) {
            Music_FadeOut(0.025);
            Announcer_AnnounceGoal(0);
            RSDK.PlaySfx(TimeAttackGate->sfxSignpost, false, 255);
            TimeAttackGate->playerPtr = NULL;
            TimeAttackGate->started   = false;
            SceneInfo->timeEnabled    = false;
#if RETRO_USE_PLUS
            StateMachine_Run(TimeAttackGate->endCB);
#endif
            if (!TimeAttackGate->disableRecords)
                ActClear->isTimeAttack = true;
#if RETRO_USE_PLUS
            TimeAttackGate_Unknown1();
#endif
        }
    }

    self->playerPos.x = player1->position.x;
    self->playerPos.y = player1->position.y;
}

#if RETRO_USE_PLUS
void TimeAttackGate_Unknown1(void)
{
    if (!TimeAttackGate->disableRecords) {
        if (ActClear)
            ActClear->finishedSavingGame = true;
        if (UIWaitSpinner)
            UIWaitSpinner_Wait();

        EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
        int32 playerID         = param->characterID;
        int32 zone             = param->zoneID;
        int32 time             = (SceneInfo->milliseconds + 100 * (SceneInfo->seconds + 60 * SceneInfo->minutes));
        int32 act              = param->actID;
        int32 mode             = SceneInfo->filter == SCN_FILTER_ENCORE;

        param->timeAttackRank = TimeAttackData_AddTADBEntry(zone, playerID, act, mode, time, TimeAttackGate_LeaderboardCB);
        TimeAttackData_AddLeaderboardEntry(zone, playerID, act, mode, time);
    }
}
#endif

void TimeAttackGate_LeaderboardCB(int32 status)
{
    if (ActClear)
        ActClear->finishedSavingGame = false;
    if (UIWaitSpinner)
        UIWaitSpinner_Wait2();
}

void TimeAttackGate_CheckTouch(void)
{
    RSDK_THIS(TimeAttackGate);
    int32 x = self->boundsOffset.x + self->position.x;
    int32 y = self->boundsOffset.y + self->position.y;

    Hitbox hitbox;
    hitbox.left   = (-self->boundsSize.x) >> 17;
    hitbox.right  = self->boundsSize.x >> 17;
    hitbox.top    = (-self->boundsSize.y) >> 17;
    hitbox.bottom = self->boundsSize.y >> 17;

    for (int32 i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        bool32 flag = false;
        if (self->boundsSize.x <= 0 || self->boundsSize.y <= 0) {
            flag = self->position.x - player->position.x < 0x1000000;
        }
        else {
            int32 storeX     = self->position.x;
            int32 storeY     = self->position.y;
            self->position.x = x;
            self->position.y = y;
            flag             = Player_CheckCollisionTouch(player, self, &hitbox);
            self->position.x = storeX;
            self->position.y = storeY;
        }

        if (flag) {
            Zone->cameraBoundsL[i] = (self->position.x >> 16) - ScreenInfo[i].centerX;
            Zone->cameraBoundsR[i] = (self->position.x >> 16) + ScreenInfo[i].centerX;
            if (self->topBoundary)
                Zone->cameraBoundsT[i] = (self->position.y) - ScreenInfo[i].centerY;
        }
    }
}

void TimeAttackGate_State_Main(void)
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
                    RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, NULL);
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
        EntityPlayer *player = TimeAttackGate->playerPtr;
        if (player) {
            if (player->state == Player_State_Die || player->state == Player_State_Drown) {
                self->restartTimer = 0;
                self->radius       = (720 * self->restartTimer / 35);
            }
            else {
                if (ControllerInfo[player->controllerID].keyY.press) {
                    self->restartTimer = 0;
                }
                if (!ControllerInfo[player->controllerID].keyY.down) {
                    if (self->restartTimer > 0) {
                        TimeAttackGate->teleportChannel = 0xFFFF;
                        RSDK.StopSfx(TimeAttackGate->sfxTeleport);
                        self->restartTimer -= 4;
                    }
                    if (self->restartTimer < 0)
                        self->restartTimer = 0;
                    self->radius = (720 * self->restartTimer / 35);
                }
                else {
                    if (!self->restartTimer)
                        TimeAttackGate->teleportChannel = RSDK.PlaySfx(TimeAttackGate->sfxTeleport, false, 255);

                    if (self->restartTimer < 35) {
                        self->restartTimer++;
                        self->radius = (720 * self->restartTimer / 35);
                    }
                    else {
                        if (self->restartTimer != 35) {
                            self->radius = (720 * self->restartTimer / 35);
                        }
                        else {
                            self->state                = NULL;
                            globals->suppressTitlecard = true;
                            for (int32 i = 0; i < CHANNEL_COUNT; ++i) {
                                if (i != Music->channelID && i != TimeAttackGate->teleportChannel) {
                                    RSDK.StopChannel(i);
                                }
                            }
                            if (!Music->activeTrack)
                                globals->suppressAutoMusic = true;
                            RSDK.LoadScene();
                        }
                    }
                }
            }
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

void TimeAttackGate_Draw_Main(void)
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
    drawPos.x                 = self->position.x + 0x30000;
    self->finAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                 = self->position.x - 0x30000;
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
    EntityPlayer *player = TimeAttackGate->playerPtr;
    if (player) {
        if (self->radius > 0)
            RSDK.DrawCircle(player->position.x, player->position.y, self->radius, 0xF0F0F0, (self->restartTimer << 8) / 35, INK_ADD, false);
        if (self->fadeTimer > 0)
            RSDK.FillScreen(0xFFFFFF, self->fadeTimer, self->fadeTimer - 128, self->fadeTimer - 256);
    }
}

#if RETRO_INCLUDE_EDITOR
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
    drawPos.x                 = self->position.x + 0x30000;
    self->finAnimator.frameID = 1;
    RSDK.DrawSprite(&self->finAnimator, &drawPos, false);

    self->scale.x             = abs(RSDK.Cos512(self->angle & 0x7F));
    drawPos.x                 = self->position.x - 0x30000;
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

void TimeAttackGate_EditorLoad(void) { TimeAttackGate->aniFrames = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE); }
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
