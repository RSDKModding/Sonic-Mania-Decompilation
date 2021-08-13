#include "SonicMania.h"

ObjectTimeAttackGate *TimeAttackGate;

void TimeAttackGate_Update(void)
{
    RSDK_THIS(TimeAttackGate);
    if (RSDK_GET_ENTITY(SLOT_PAUSEMENU, PauseMenu)->objectID != PauseMenu->objectID) {
        StateMachine_Run(entity->state);
    }
}

void TimeAttackGate_LateUpdate(void) {}

void TimeAttackGate_StaticUpdate(void)
{
    if (globals->gameMode == MODE_TIMEATTACK) {
        EntityPlayer *player = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);
        if (TimeAttackGate->field_1C || !TimeAttackGate->started) {
            player->airTimer = 0;
        }

        if (TimeAttackGate->suppressedTitlecard) {
            EntityTimeAttackGate *entity        = CREATE_ENTITY(TimeAttackGate, intToVoid(1), 0, 0);
            entity->state                       = TimeAttackGate_Unknown4;
            entity->fadeTimer                   = 532;
            TimeAttackGate->suppressedTitlecard = false;
        }
        if (DebugMode->debugActive)
            TimeAttackGate->debugEnabled = true;
    }
}

void TimeAttackGate_Draw(void)
{
    RSDK_THIS(TimeAttackGate);
    StateMachine_Run(entity->stateDraw);
}

void TimeAttackGate_Create(void *data)
{
    RSDK_THIS(TimeAttackGate);
    if (RSDK_sceneInfo->inEditor || globals->gameMode == MODE_TIMEATTACK) {
        RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 0, &entity->baseAnimator, true, 0);
        RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 1, &entity->topAnimator, true, 0);
        entity->visible = true;
        if (entity->finishLine)
            RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 4, &entity->finAnimator, true, 0);
        else
            RSDK.SetSpriteAnimation(TimeAttackGate->aniFrames, 3, &entity->finAnimator, true, 0);

        if (!RSDK_sceneInfo->inEditor) {
            if (data == intToVoid(1)) {
                entity->drawOrder = 14;
                entity->active    = ACTIVE_ALWAYS;
                entity->state     = TimeAttackGate_State_Restarter;
                entity->stateDraw = TimeAttackGate_StateDraw_Restarter;
            }
            else {
                entity->active        = ACTIVE_NORMAL;
                entity->updateRange.x = 0x400000;
                entity->updateRange.y = 0x400000;
                entity->drawOrder     = Zone->playerDrawLow + 1;

                int checkA = entity->boundsOffset.y + (entity->boundsSize.y >> 1);
                int checkB = entity->boundsOffset.y - (entity->boundsSize.y >> 1);

                if ((-0x10000 * entity->extendTop) < entity->boundsOffset.y - (entity->boundsSize.y >> 1))
                    checkB = (-0x10000 * entity->extendTop);

                if (entity->extendBottom << 16 < checkA)
                    checkA = entity->extendBottom << 16;

                if (abs(entity->boundsOffset.x - (entity->boundsSize.x >> 1)) > checkB)
                    entity->updateRange.x = abs(entity->boundsOffset.x - (entity->boundsSize.x >> 1));
                else
                    entity->updateRange.x = ((entity->boundsSize.x >> 1) + entity->boundsOffset.x) + 0x400000;

                if (abs(checkB) > checkA)
                    entity->updateRange.y = abs(checkB) + 0x400000;
                else
                    entity->updateRange.y = checkA + 0x400000;

                entity->scale.y   = 0x200;
                entity->state     = TimeAttackGate_State_Main;
                entity->stateDraw = TimeAttackGate_StateDraw_Main;
            }
        }
    }
    else {
        destroyEntity(entity);
    }
}

void TimeAttackGate_StageLoad(void)
{
    TimeAttackGate->dword30       = 0xFFFF;
    TimeAttackGate->aniFrames     = RSDK.LoadSpriteAnimation("Global/SpeedGate.bin", SCOPE_STAGE);
    TimeAttackGate->hitbox.left   = -8;
    TimeAttackGate->hitbox.top    = -44;
    TimeAttackGate->hitbox.right  = 8;
    TimeAttackGate->hitbox.bottom = 20;
    TimeAttackGate->sfxSignpost   = RSDK.GetSFX("Global/SignPost.wav");
    TimeAttackGate->sfxTeleport   = RSDK.GetSFX("Global/Teleport.wav");
    TimeAttackGate->started       = false;
    TimeAttackGate->debugEnabled  = false;
    RSDK_sceneInfo->timeEnabled   = false;
    if (globals->suppressTitlecard)
        TimeAttackGate->suppressedTitlecard = true;
}

void TimeAttackGate_HandleSpin(void)
{
    RSDK_THIS(TimeAttackGate);

    int top    = (TimeAttackGate->hitbox.top << 16) - (entity->extendTop << 16) + entity->position.y;
    int bottom = ((entity->extendBottom + TimeAttackGate->hitbox.bottom) << 16) + entity->position.y;

    foreach_active(Player, player)
    {
        if (!player->sidekick && !player->isGhost) {
            if (!entity->playerPos.x || !entity->playerPos.y) {
                foreach_break;
            }

            int vel = 0;
            if (player->onGround)
                vel = player->groundVel;
            else
                vel = player->velocity.x;
            if (vel >> 15
                && MathHelpers_Unknown12(player->position.x, player->position.y, entity->playerPos.x, entity->playerPos.y, entity->position.x, bottom,
                                         entity->position.x, top)) {
                int val = (vel >> 15) - 2;
                if ((vel >> 15) >= 0)
                    val = (vel >> 15) + 2;
                if (abs(val) > abs(entity->spinSpeed))
                    entity->spinSpeed = val;
                entity->spinTimer = 0;
            }
        }
    }

    entity->angle += entity->spinSpeed;
    bool32 flag = false;
    if (entity->spinSpeed <= 0) {
        if (entity->angle <= -512) {
            ++entity->spinTimer;
            entity->angle += 512;
            entity->spinSpeed += 4;
            if (entity->spinSpeed > -2)
                entity->spinSpeed = -2;
            flag = entity->spinSpeed == -2;
        }
    }
    else {
        if (entity->angle >= 512) {
            ++entity->spinTimer;
            entity->angle -= 512;
            entity->spinSpeed -= 4;
            if (entity->spinSpeed < 2)
                entity->spinSpeed = 2;
            flag = entity->spinSpeed == 2;
        }
    }

    if (flag) {
        entity->spinSpeed = 0;
        entity->angle     = 0;
    }
}

void TimeAttackGate_HandleStart(void)
{
    RSDK_THIS(TimeAttackGate);
    int top               = (TimeAttackGate->hitbox.top << 16) - (entity->extendTop << 16) + entity->position.y;
    int bottom            = ((entity->extendBottom + TimeAttackGate->hitbox.bottom) << 16) + entity->position.y;
    EntityPlayer *player1 = RSDK_GET_ENTITY(SLOT_PLAYER1, Player);

    if (MathHelpers_Unknown12(player1->position.x, player1->position.y, entity->playerPos.x, entity->playerPos.y, entity->position.x, bottom,
                              entity->position.x, top)) {
        entity->field_84 = 1;
        if (!entity->finishLine) {
            if (!TimeAttackGate->started) {
                RSDK.PlaySFX(TimeAttackGate->sfxSignpost, false, 255);
                TimeAttackGate->playerPtr       = player1;
                TimeAttackGate->started         = true;
                RSDK_sceneInfo->timeEnabled     = true;
                EntityTimeAttackGate *restarter = CREATE_ENTITY(TimeAttackGate, intToVoid(1), entity->position.x, entity->position.y);
                TimeAttackGate->activeEntity    = (Entity *)restarter;
                restarter->isPermanent          = true;
                StateMachine_Run(TimeAttackGate->startCB);
            }
        }
        else if (TimeAttackGate->started) {
            Music_FadeOut(0.025);
            Announcer_Unknown2(0);
            RSDK.PlaySFX(TimeAttackGate->sfxSignpost, false, 255);
            TimeAttackGate->playerPtr   = NULL;
            TimeAttackGate->started     = false;
            RSDK_sceneInfo->timeEnabled = false;
            StateMachine_Run(TimeAttackGate->endCB);
            if (!TimeAttackGate->debugEnabled)
                ActClear->isTimeAttack = true;
            TimeAttackGate_Unknown1();
        }
    }

    entity->playerPos.x = player1->position.x;
    entity->playerPos.y = player1->position.y;
}

void TimeAttackGate_Unknown1(void)
{
    if (!TimeAttackGate->debugEnabled) {
        if (ActClear)
            ActClear->field_10 = 1;
        if (UIWaitSpinner)
            UIWaitSpinner_Wait();

        EntityMenuParam *param = (EntityMenuParam *)globals->menuParam;
        int playerID = param->characterID;
        int zone     = param->zoneID;
        int time     = (RSDK_sceneInfo->milliseconds + 100 * (RSDK_sceneInfo->seconds + 60 * RSDK_sceneInfo->minutes));
        int act      = param->actID;
        int mode     = RSDK_sceneInfo->filter == SCN_FILTER_ENCORE;

        param->timeScore = TimeAttackData_AddTADBEntry(playerID, zone, act, mode, time, TimeAttackGate_LeaderboardCB);
        TimeAttackData_AddLeaderboardEntry(playerID, zone, act, mode, time);
    }
}

void TimeAttackGate_LeaderboardCB(int status)
{
    if (ActClear)
        ActClear->field_10 = 0;
    if (UIWaitSpinner)
        UIWaitSpinner_Wait2();
}

void TimeAttackGate_CheckTouch(void)
{
    RSDK_THIS(TimeAttackGate);
    int x = entity->boundsOffset.x + entity->position.x;
    int y = entity->boundsOffset.y + entity->position.y;

    Hitbox hitbox;
    hitbox.left   = (-entity->boundsSize.x) >> 17;
    hitbox.right  = entity->boundsSize.x >> 17;
    hitbox.top    = (-entity->boundsSize.y) >> 17;
    hitbox.bottom = entity->boundsSize.y >> 17;

    for (int i = 0; i < Player->playerCount; ++i) {
        EntityPlayer *player = RSDK_GET_ENTITY(i, Player);

        bool32 flag = false;
        if (entity->boundsSize.x <= 0 || entity->boundsSize.y <= 0) {
            flag = entity->position.x - player->position.x < 0x1000000;
        }
        else {
            int storeX         = entity->position.x;
            int storeY         = entity->position.y;
            entity->position.x = x;
            entity->position.y = y;
            flag               = Player_CheckCollisionTouch(player, entity, &hitbox);
            entity->position.x = storeX;
            entity->position.y = storeY;
        }

        if (flag) {
            Zone->screenBoundsL1[i] = (entity->position.x >> 16) - RSDK_screens[i].centerX;
            Zone->screenBoundsR1[i] = (entity->position.x >> 16) + RSDK_screens[i].centerX;
            if (entity->topBoundary)
                Zone->screenBoundsT1[i] = (entity->position.y) - RSDK_screens[i].centerY;
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
            if (!entity->finishLine) {
                if (!TimeAttackGate->started) {
                    foreach_active(HUD, hud)
                    {
                        if (hud)
                            hud->field_150 = 1;
                        foreach_break;
                    }
                }
            }
            else {
                if (entity->field_84 && player->stateInput) {
                    player->stateInput = StateMachine_None;
                    player->up         = false;
                    player->left       = false;
                    player->right      = false;
                    player->down       = false;
                    player->jumpPress  = false;
                    player->jumpHold   = false;
                }
                if (!entity->finishLine) {
                    if (!TimeAttackGate->started) {
                        foreach_active(HUD, hud)
                        {
                            if (hud)
                                hud->field_150 = 1;
                            foreach_break;
                        }
                    }
                }
            }
            entity->playerPos.x = player->position.x;
            entity->playerPos.y = player->position.y;
        }
    }

    if (entity->finishLine) {
        TimeAttackGate_CheckTouch();
        if (entity->field_84) {
            TimeAttackGate->field_1C = 1;
            if (entity->timer >= 180) {
                if (entity->timer == 180) {
                    Music_PlayTrack(TRACK_ACTCLEAR);
                    RSDK.ResetEntitySlot(SLOT_ACTCLEAR, ActClear->objectID, NULL);
                    ++entity->timer;
                }
            }
            else {
                entity->timer++;
            }
        }
    }
}

void TimeAttackGate_State_Restarter(void)
{
    RSDK_THIS(TimeAttackGate);
    if (TimeAttackGate->field_1C) {
        destroyEntity(entity);
    }
    else {
        EntityPlayer *player = TimeAttackGate->playerPtr;
        if (player) {
            if (player->state == Player_State_Die || player->state == Player_State_Drown) {
            LABEL_18:
                entity->restartTimer = 0;
                entity->radius       = (720 * entity->restartTimer / 35);
            }
            else {
                if (RSDK_controller[player->controllerID].keyY.press) {
                    entity->restartTimer = 0;
                }
                if (!RSDK_controller[player->controllerID].keyY.down) {
                    if (entity->restartTimer > 0) {
                        TimeAttackGate->dword30 = 0xFFFF;
                        RSDK.StopSFX(TimeAttackGate->sfxTeleport);
                        entity->restartTimer -= 4;
                    }
                    if (entity->restartTimer < 0)
                        entity->restartTimer = 0;
                    entity->radius = (720 * entity->restartTimer / 35);
                }
                else {
                    if (!entity->restartTimer)
                        TimeAttackGate->dword30 = RSDK.PlaySFX(TimeAttackGate->sfxTeleport, false, 255);

                    if (entity->restartTimer < 35) {
                        entity->restartTimer++;
                        entity->radius = (720 * entity->restartTimer / 35);
                    }
                    else {
                        if (entity->restartTimer != 35) {
                            entity->radius = (720 * entity->restartTimer / 35);
                        }
                        else {
                            entity->state              = NULL;
                            globals->suppressTitlecard = true;
                            for (int i = 0; i < CHANNEL_COUNT; ++i) {
                                if (i != Music->channelID && i != TimeAttackGate->dword30) {
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

void TimeAttackGate_Unknown4(void)
{
    RSDK_THIS(TimeAttackGate);
    if (entity->fadeTimer <= 0) {
        globals->suppressTitlecard = false;
        destroyEntity(entity);
    }
    else {
        entity->fadeTimer -= 32;
    }
}

void TimeAttackGate_StateDraw_Main(void)
{
    RSDK_THIS(TimeAttackGate);
    Vector2 drawPos;

    entity->rotation = entity->field_88;
    entity->drawFX   = FX_ROTATE;
    RSDK.DrawSprite(&entity->topAnimator, 0, false);
    RSDK.DrawSprite(&entity->baseAnimator, 0, false);

    entity->drawFX = FX_SCALE;
    drawPos.y      = entity->position.y;
    if (RSDK.Sin512(entity->angle & 0x7F) >= 0)
        entity->scale.x = RSDK.Sin512(entity->angle & 0x7F);
    else
        entity->scale.x = -RSDK.Sin512(entity->angle & 0x7F);
    drawPos.x                   = entity->position.x + 0x30000;
    entity->finAnimator.frameID = 1;
    RSDK.DrawSprite(&entity->finAnimator, &drawPos, false);

    if (RSDK.Cos512(entity->angle & 0x7F) >= 0)
        entity->scale.x = RSDK.Cos512(entity->angle & 0x7F);
    else
        entity->scale.x = -RSDK.Cos512(entity->angle & 0x7F);
    drawPos.x                   = entity->position.x - 0x30000;
    entity->finAnimator.frameID = 0;
    RSDK.DrawSprite(&entity->finAnimator, &drawPos, false);

    drawPos.x                   = 0x180 * RSDK.Cos512(2 * (entity->angle & 0x7F)) + entity->position.x;
    entity->finAnimator.frameID = 1;
    RSDK.DrawSprite(&entity->finAnimator, &drawPos, false);

    if (RSDK.Sin512(entity->angle & 0x7F) >= 0)
        entity->scale.x = RSDK.Sin512(entity->angle & 0x7F);
    else
        entity->scale.x = -RSDK.Sin512(entity->angle & 0x7F);
    drawPos.x                   = 0xB40 * RSDK.Cos512(entity->angle & 0x7F) + entity->position.x;
    entity->finAnimator.frameID = 2;
    RSDK.DrawSprite(&entity->finAnimator, &drawPos, false);

    if (RSDK.Sin512(entity->angle & 0x7F) >= 0)
        entity->scale.x = RSDK.Sin512(entity->angle & 0x7F);
    else
        entity->scale.x = -RSDK.Sin512(entity->angle & 0x7F);
    drawPos.x                   = 0x180 * RSDK.Cos512(2 * (entity->angle & 0x7F)) + entity->position.x;
    entity->finAnimator.frameID = 0;
    RSDK.DrawSprite(&entity->finAnimator, &drawPos, false);

    if (RSDK.Cos512(entity->angle & 0x7F) >= 0)
        entity->scale.x = RSDK.Cos512(entity->angle & 0x7F);
    else
        entity->scale.x = -RSDK.Cos512(entity->angle & 0x7F);
    drawPos.x                   = entity->position.x - 0xB40 * RSDK.Sin512(entity->angle & 0x7F);
    entity->finAnimator.frameID = 2;
    RSDK.DrawSprite(&entity->finAnimator, &drawPos, false);
}

void TimeAttackGate_StateDraw_Restarter(void)
{
    RSDK_THIS(TimeAttackGate);
    EntityPlayer *player = TimeAttackGate->playerPtr;
    if (player) {
        if (entity->radius > 0)
            RSDK.DrawCircle(player->position.x, player->position.y, entity->radius, 0xF0F0F0, (entity->restartTimer << 8) / 35, INK_ADD, false);
        if (entity->fadeTimer > 0)
            RSDK.FillScreen(0xFFFFFF, entity->fadeTimer, entity->fadeTimer - 128, entity->fadeTimer - 256);
    }
}

void TimeAttackGate_EditorDraw(void) {}

void TimeAttackGate_EditorLoad(void) {}

void TimeAttackGate_Serialize(void)
{
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_BOOL, finishLine);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_ENUM, extendTop);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_ENUM, extendBottom);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_VECTOR2, boundsSize);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_VECTOR2, boundsOffset);
    RSDK_EDITABLE_VAR(TimeAttackGate, VAR_BOOL, topBoundary);
}
